#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "engine_input.h"
#include "input.h"


const Nucleus::ClassDescr NC_STACK_input::description("input.class", &newinstance);


size_t NC_STACK_input::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    for (InputNodeList &lst : _buttons)
        lst.clear();
    
    for (InputNodeList &lst : _sliders)
        lst.clear();
    
    for (int16_t &k : _hotKeys)
        k = Input::KEY_NONE;

    return 1;
}

size_t NC_STACK_input::func1()
{
    if ( _timer )
        delete_class_obj(_timer);

    if ( _wimp )
        delete_class_obj(_wimp);

    if ( _keyboard )
        delete_class_obj(_keyboard);

    for (InputNodeList &lst : _buttons)
        FreeKNodes(&lst);
    
    for (InputNodeList &lst : _sliders)
        FreeKNodes(&lst);
    
    return NC_STACK_nucleus::func1();
}

std::string NC_STACK_input::ParseInputNodeNext(const std::string &inputStr, InputNodeList *lst)
{
    if (!lst)
        return std::string();
    
    lst->emplace_back();
    
    InputNode &node = lst->back();

    std::string *out = &node.DriverName;
    bool finishParse = false;

    for( size_t i = 0; i < inputStr.size(); i++ )
    {
        switch(inputStr[i])
        {
            case '\t':
            case ' ':
                if (finishParse)
                    return inputStr.substr(i);
                break;
                
            case '#':
                if (finishParse)
                    return inputStr.substr(i);
                
                node.Flags |= InputNode::FLAG_FSLIDE;
                break;
            
            case '&':
                if (finishParse)
                    return inputStr.substr(i);;

                node.Flags |= InputNode::FLAG_AND;
                break;
                
            case ':':
                finishParse = true;
                out = &node.KeyName;
                break;
            
            case '|':
                if (finishParse)
                    return inputStr.substr(i);;

                node.Flags |= InputNode::FLAG_OR;
                break;
            
            case '~':
                if (finishParse)
                    return inputStr.substr(i);;

                node.Flags |= InputNode::FLAG_INV;
                break;
            
            default:
                (*out) += inputStr[i];
        }
    }
    return std::string();
}

bool NC_STACK_input::SetInputExpression(bool slider, uint32_t index, const std::string &keysStr)
{
    InputNodeList *pLst;
    if ( slider )
        pLst = &_sliders.at(index);
    else
        pLst = &_buttons.at(index);

    if ( !keysStr.empty() )
    {
        FreeKNodes(pLst);

        std::string keysString = keysStr;

        while (!keysString.empty())
            keysString = ParseInputNodeNext(keysString, pLst);

        for ( InputNode &node : *pLst )
        {
            if ( !node.DriverName.empty() && !node.KeyName.empty() )
            {
                node.DriverObj = Nucleus::CTFInit<NC_STACK_idev>(node.DriverName + ".class");
                if ( node.DriverObj )
                    node.IsSlider = node.DriverObj->BindKey(node.KeyName);
                else
                    ypa_log_out("input.class, WARN: Unknown driver '%s'.\n", node.DriverName.c_str());
            }
        }
    }
    return true;
}

bool NC_STACK_input::InitDriver(uint8_t type, const std::string &val)
{
    switch ( type )
    {
    case Input::ITYPE_WIMP:                                     // input.wimp
        if ( _wimp )
            delete_class_obj(_wimp);

        _wimp = Nucleus::CTFInit<NC_STACK_iwimp>(val + ".class");
        if ( !_wimp )
            return false;
        break;

    case Input::ITYPE_TIMER:                                   // input.timer
        if ( _timer )
            delete_class_obj(_timer);

        _timer = Nucleus::CTFInit<NC_STACK_itimer>(val + ".class");
        if ( !_timer )
            return false;
        break;

    case Input::ITYPE_KBD:
        if ( _keyboard )
            delete_class_obj(_keyboard);

        _keyboard = Nucleus::CTFInit<NC_STACK_idev>(val + ".class");
        if ( !_keyboard )
            return false;
        break;

    default:
        return false;
        break;
    }
    return true;
}

void NC_STACK_input::UpdateList(InputNodeList *lst, bool *btn, float *slider)
{
    bool btmp = true;
    float ftmp = 0.0;
    
    if (btn)
        btmp = *btn;
    if (slider)
        ftmp = *slider;
    
    for ( InputNode &node : *lst )
    {
        if ( node.DriverObj )
        {
            if (node.IsSlider || (node.Flags & InputNode::FLAG_FSLIDE) )
            {
                if ( node.Flags & InputNode::FLAG_INV )
                    ftmp -= node.DriverObj->GetSlider();
                else
                    ftmp += node.DriverObj->GetSlider();
            }
            else
            {
                bool state = node.DriverObj->GetState();

                if ( node.Flags & InputNode::FLAG_INV )
                    state = !state;

                if ( node.Flags & InputNode::FLAG_OR )
                    btmp |= state;
                else
                    btmp &= state;
            }
        }
    }
    
    if (btn)
        *btn = btmp;
    if (slider)
        *slider = ftmp;
}

void NC_STACK_input::QueryInput(InputState *arg)
{
    arg->Clear();

    if ( _timer )
        arg->Period = _timer->itimer_func64();
    else
        arg->Period = 20;

    if ( _wimp && _wimp->HasFocus() )
    {
        if ( _keyboard )
            _keyboard->QueryKeyboard(arg);
        
        arg->HotKeyID = CheckHotKey(arg->KbdLastHit);
        
        _wimp->CheckClick(&arg->ClickInf);

        for (size_t i = 0; i < _buttons.size(); i++)
        {
            bool state = true;
            UpdateList(&_buttons[i], &state);

            if (state)
                arg->Buttons.Set(i);
        }

        for (size_t i = 0; i < _sliders.size(); i++)
        {
            bool state = true;
            float pos = 0.0;
            UpdateList(&_sliders[i], &state, &pos);

            if (state)
                arg->Sliders[i] = pos;
        }
    }
    else
    {
        for ( InputNodeList &lst : _sliders )
            UpdateList(&lst);
    }
}

bool NC_STACK_input::SetHotKey(uint16_t id, const std::string &keyname)
{
    if ( id < _hotKeys.size() )
    {
        int16_t kId = GetKeyIDByName(keyname);
        if (kId == -1)
            return false;
        
        if ( KeyMatrix.at(kId).IsSlider == false )
        {
            _hotKeys[ id ] = kId;
            return true;
        }
    }
    return false;
}

void NC_STACK_input::wimp_addClickNodeFront(ClickBox *box)
{
    _wimp->AddClickBoxFront(box);
}

void NC_STACK_input::wimp_addClickNodeBack(ClickBox *box)
{
    _wimp->AddClickBoxBack(box);
}

void NC_STACK_input::wimp_remClickNode(ClickBox *box)
{
    _wimp->RemoveClickBox(box);
}


void NC_STACK_input::ForceFeedback(uint8_t state, uint8_t effID, float p1, float p2, float p3, float p4)
{
    _wimp->ForceFeedBack(state, effID, p1, p2, p3, p4);
}


void NC_STACK_input::ResetSlider(int sldr)
{
    for( InputNode &node : _sliders.at(sldr) )
    {
        if (node.DriverObj)
            node.DriverObj->ResetSlider();
    }
}

void NC_STACK_input::FreeKNodes(InputNodeList *lst)
{
    while (!lst->empty())
    {
        InputNode &nod = lst->front();

        if (nod.DriverObj)
            Nucleus::Delete(nod.DriverObj);

        lst->pop_front();
    }
}


int16_t NC_STACK_input::GetKeyIDByName(const std::string &name)
{
    for( Input::KeyMapName &m : KeyNamesTable )
    {
        if ( !StriCmp(m.Name, name) )
            return m.ID;
    }
    
    for( Input::KeyMapName &m : KeyAltNamesTable )
    {
        if ( !StriCmp(m.Name, name) )
            return m.ID;
    }
    
    return -1;
}

int16_t NC_STACK_input::CheckHotKey(int16_t key)
{
    if ( key == Input::KEY_NONE )
        return -1;

    for (size_t i = 0; i < _hotKeys.size(); i++)
    {
        if (_hotKeys[i] == key)
            return i;
    }
    return -1;
}

int16_t NC_STACK_input::GetHotKeyID(int16_t keycode)
{
    for ( size_t i = 0; i < _hotKeys.size(); i++ )
    {
        if ( _hotKeys[i] == keycode )
            return i;
    }
    return -1;
}

int16_t NC_STACK_input::GetHotKey(uint16_t id)
{
    if (id < _hotKeys.size())
        return _hotKeys[id];
    
    return Input::KEY_NONE;
}

std::array<Input::KeyInfo, Input::KEY_MAX> NC_STACK_input::KeyMatrix {{
    {Input::KEY_NONE, false},       {Input::KEY_ESCAPE, false},         
    {Input::KEY_SPACE, false},      {Input::KEY_UP, false},      
    {Input::KEY_DOWN, false},       {Input::KEY_LEFT, false},      
    {Input::KEY_RIGHT, false},      {Input::KEY_F1, false},              
    {Input::KEY_F2, false},         {Input::KEY_F3, false},
    {Input::KEY_F4, false},         {Input::KEY_F5, false},              
    {Input::KEY_F6, false},         {Input::KEY_F7, false},      
    {Input::KEY_F8, false},         {Input::KEY_F9, false},          
    {Input::KEY_F10, false},        {Input::KEY_F11, false},            
    {Input::KEY_F12, false},        {Input::KEY_BACKSPACE, false},
    {Input::KEY_TAB, false},        {Input::KEY_CLEAR, false},        
    {Input::KEY_RETURN, false},     {Input::KEY_CTRL, false},  
    {Input::KEY_SHIFT, false},      {Input::KEY_ALT, false},           
    {Input::KEY_PAUSE, false},      {Input::KEY_PGUP, false},
    {Input::KEY_PGDOWN, false},     {Input::KEY_END, false},            
    {Input::KEY_HOME, false},       {Input::KEY_SELECT, false},
    {Input::KEY_EXECUTE, false},    {Input::KEY_SNAPSHOT, false},
    {Input::KEY_INSERT, false},     {Input::KEY_DELETE, false},         
    {Input::KEY_HELP, false},       {Input::KEY_1, false},
    {Input::KEY_2, false},          {Input::KEY_3, false},                  
    {Input::KEY_4, false},          {Input::KEY_5, false},          
    {Input::KEY_6, false},          {Input::KEY_7, false},              
    {Input::KEY_8, false},          {Input::KEY_9, false},                  
    {Input::KEY_0, false},          {Input::KEY_A, false},
    {Input::KEY_B, false},          {Input::KEY_C, false},                  
    {Input::KEY_D, false},          {Input::KEY_E, false},          
    {Input::KEY_F, false},          {Input::KEY_G, false},              
    {Input::KEY_H, false},          {Input::KEY_I, false},                  
    {Input::KEY_J, false},          {Input::KEY_K, false},
    {Input::KEY_L, false},          {Input::KEY_M, false},                  
    {Input::KEY_N, false},          {Input::KEY_O, false},          
    {Input::KEY_P, false},          {Input::KEY_Q, false},              
    {Input::KEY_R, false},          {Input::KEY_S, false},                  
    {Input::KEY_T, false},          {Input::KEY_U, false},
    {Input::KEY_V, false},          {Input::KEY_W, false},                  
    {Input::KEY_X, false},          {Input::KEY_Y, false},          
    {Input::KEY_Z, false},          {Input::KEY_NUM0, false},   
    {Input::KEY_NUM1, false},       {Input::KEY_NUM2, false},       
    {Input::KEY_NUM3, false},       {Input::KEY_NUM4, false},
    {Input::KEY_NUM5, false},       {Input::KEY_NUM6, false},       
    {Input::KEY_NUM7, false},       {Input::KEY_NUM8, false}, 
    {Input::KEY_NUM9, false},       {Input::KEY_NUMMUL, false},
    {Input::KEY_NUMPLUS, false},    {Input::KEY_NUMDOT, false},     
    {Input::KEY_NUMMINUS, false},   {Input::KEY_NUMENTER, false},
    {Input::KEY_NUMDIV, false},     {Input::KEY_EXTRA1, false},   
    {Input::KEY_EXTRA2, false},     {Input::KEY_EXTRA3, false},
    {Input::KEY_EXTRA4, false},     {Input::KEY_EXTRA5, false},   
    {Input::KEY_EXTRA6, false},     {Input::KEY_EXTRA7, false},       
    {Input::KEY_EXTRA8, false},     {Input::KEY_EXTRA9, false},
    {Input::KEY_EXTRA10, false},    {Input::KEY_EXTRA11, false},      
    {Input::KEY_EXTRA12, false},    {Input::KEY_EXTRA13, false},
    {Input::KEY_EXTRA14, false},    {Input::KEY_EXTRA15, false},
    {Input::KEY_EXTRA16, false},    {Input::KEY_EXTRA17, false},        
    {Input::KEY_EXTRA18, false},    {Input::KEY_LMB, false},
    {Input::KEY_RMB, false},        {Input::KEY_MMB, false},                
    {Input::KEY_MOUSEX, true},      {Input::KEY_MOUSEY, true},     
    {Input::KEY_JOYB0, false},      {Input::KEY_JOYB1, false},          
    {Input::KEY_JOYB2, false},      {Input::KEY_JOYB3, false},              
    {Input::KEY_JOYB4, false},      {Input::KEY_JOYB5, false},
    {Input::KEY_JOYB6, false},      {Input::KEY_JOYB7, false},              
    {Input::KEY_JOYX, true},        {Input::KEY_JOYY, true},      
    {Input::KEY_JOYTHROTTLE, true}, {Input::KEY_JOYHATX, true},         
    {Input::KEY_JOYHATY, true},     {Input::KEY_JOYRUDDER, true}
}};

std::array<Input::KeyMapName, Input::KEY_MAX> NC_STACK_input::KeyNamesTable {{
    {"nop", Input::KEY_NONE},        {"esc", Input::KEY_ESCAPE},         
    {"space", Input::KEY_SPACE},     {"up", Input::KEY_UP},      
    {"down", Input::KEY_DOWN},       {"left", Input::KEY_LEFT},      
    {"right", Input::KEY_RIGHT},     {"f1", Input::KEY_F1},              
    {"f2", Input::KEY_F2},           {"f3", Input::KEY_F3},
    {"f4", Input::KEY_F4},           {"f5", Input::KEY_F5},              
    {"f6", Input::KEY_F6},           {"f7", Input::KEY_F7},      
    {"f8", Input::KEY_F8},           {"f9", Input::KEY_F9},          
    {"f10", Input::KEY_F10},         {"f11", Input::KEY_F11},            
    {"f12", Input::KEY_F12},         {"bs", Input::KEY_BACKSPACE},
    {"tab", Input::KEY_TAB},         {"clear", Input::KEY_CLEAR},        
    {"return", Input::KEY_RETURN},   {"ctrl", Input::KEY_CTRL},    
    {"shift", Input::KEY_SHIFT},     {"alt", Input::KEY_ALT},           
    {"pause", Input::KEY_PAUSE},     {"pageup", Input::KEY_PGUP},
    {"pagedown", Input::KEY_PGDOWN}, {"end", Input::KEY_END},            
    {"home", Input::KEY_HOME},       {"select", Input::KEY_SELECT},
    {"execute", Input::KEY_EXECUTE}, {"snapshot", Input::KEY_SNAPSHOT},
    {"ins", Input::KEY_INSERT},      {"del", Input::KEY_DELETE},         
    {"help", Input::KEY_HELP},       {"1", Input::KEY_1},
    {"2", Input::KEY_2},             {"3", Input::KEY_3},                  
    {"4", Input::KEY_4},             {"5", Input::KEY_5},          
    {"6", Input::KEY_6},             {"7", Input::KEY_7},              
    {"8", Input::KEY_8},             {"9", Input::KEY_9},                  
    {"0", Input::KEY_0},             {"a", Input::KEY_A},
    {"b", Input::KEY_B},             {"c", Input::KEY_C},                  
    {"d", Input::KEY_D},             {"e", Input::KEY_E},          
    {"f", Input::KEY_F},             {"g", Input::KEY_G},              
    {"h", Input::KEY_H},             {"i", Input::KEY_I},                  
    {"j", Input::KEY_J},             {"k", Input::KEY_K},
    {"l", Input::KEY_L},             {"m", Input::KEY_M},                  
    {"n", Input::KEY_N},             {"o", Input::KEY_O},          
    {"p", Input::KEY_P},             {"q", Input::KEY_Q},              
    {"r", Input::KEY_R},             {"s", Input::KEY_S},                  
    {"t", Input::KEY_T},             {"u", Input::KEY_U},
    {"v", Input::KEY_V},             {"w", Input::KEY_W},                  
    {"x", Input::KEY_X},             {"y", Input::KEY_Y},          
    {"z", Input::KEY_Z},             {"num0", Input::KEY_NUM0},   
    {"num1", Input::KEY_NUM1},       {"num2", Input::KEY_NUM2},       
    {"num3", Input::KEY_NUM3},       {"num4", Input::KEY_NUM4},
    {"num5", Input::KEY_NUM5},       {"num6", Input::KEY_NUM6},       
    {"num7", Input::KEY_NUM7},       {"num8", Input::KEY_NUM8}, 
    {"num9", Input::KEY_NUM9},       {"nummul", Input::KEY_NUMMUL},
    {"numplus", Input::KEY_NUMPLUS}, {"numdot", Input::KEY_NUMDOT},     
    {"numminus", Input::KEY_NUMMINUS},{"enter", Input::KEY_NUMENTER},
    {"numdiv", Input::KEY_NUMDIV},   {"extra1", Input::KEY_EXTRA1},   
    {"extra2", Input::KEY_EXTRA2},   {"extra3", Input::KEY_EXTRA3},
    {"extra4", Input::KEY_EXTRA4},   {"extra5", Input::KEY_EXTRA5},   
    {"extra6", Input::KEY_EXTRA6},   {"extra7", Input::KEY_EXTRA7},       
    {"extra8", Input::KEY_EXTRA8},   {"extra9", Input::KEY_EXTRA9},
    {"extra10", Input::KEY_EXTRA10}, {"extra11", Input::KEY_EXTRA11},      
    {"extra12", Input::KEY_EXTRA12}, {"extra13", Input::KEY_EXTRA13},
    {"extra14", Input::KEY_EXTRA14}, {"extra15", Input::KEY_EXTRA15},
    {"extra16", Input::KEY_EXTRA16}, {"extra17", Input::KEY_EXTRA17},        
    {"extra18", Input::KEY_EXTRA18}, {"lmb", Input::KEY_LMB},
    {"rmb", Input::KEY_RMB},         {"mmb", Input::KEY_MMB},                
    {"mousex", Input::KEY_MOUSEX},   {"mousey", Input::KEY_MOUSEY},     
    {"joyb0", Input::KEY_JOYB0},     {"joyb1", Input::KEY_JOYB1},          
    {"joyb2", Input::KEY_JOYB2},     {"joyb3", Input::KEY_JOYB3},              
    {"joyb4", Input::KEY_JOYB4},     {"joyb5", Input::KEY_JOYB5},
    {"joyb6", Input::KEY_JOYB6},     {"joyb7", Input::KEY_JOYB7},              
    {"joyx", Input::KEY_JOYX},       {"joyy", Input::KEY_JOYY},      
    {"joythrottle", Input::KEY_JOYTHROTTLE},{"joyhatx", Input::KEY_JOYHATX},         
    {"joyhaty", Input::KEY_JOYHATY}, {"joyrudder", Input::KEY_JOYRUDDER}
}};

std::vector<Input::KeyMapName> NC_STACK_input::KeyAltNamesTable ({
    {"rshift", Input::KEY_SHIFT},    {"lshift", Input::KEY_SHIFT},
    {"lctrl", Input::KEY_CTRL},      {"rctrl", Input::KEY_CTRL},
    {"lalt", Input::KEY_ALT},        {"ralt", Input::KEY_CTRL},
});

std::array<std::string, Input::KEY_MAX> NC_STACK_input::KeyTitle;
