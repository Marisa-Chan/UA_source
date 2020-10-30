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
        k = Input::KC_NONE;

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
    if ( key == Input::KC_NONE )
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
    
    return Input::KC_NONE;
}

std::array<Input::KeyInfo, Input::KC_MAX> NC_STACK_input::KeyMatrix {{
    {Input::KC_NONE, false},       {Input::KC_ESCAPE, false},         
    {Input::KC_SPACE, false},      {Input::KC_UP, false},      
    {Input::KC_DOWN, false},       {Input::KC_LEFT, false},      
    {Input::KC_RIGHT, false},      {Input::KC_F1, false},              
    {Input::KC_F2, false},         {Input::KC_F3, false},
    {Input::KC_F4, false},         {Input::KC_F5, false},              
    {Input::KC_F6, false},         {Input::KC_F7, false},      
    {Input::KC_F8, false},         {Input::KC_F9, false},          
    {Input::KC_F10, false},        {Input::KC_F11, false},            
    {Input::KC_F12, false},        {Input::KC_BACKSPACE, false},
    {Input::KC_TAB, false},        {Input::KC_CLEAR, false},        
    {Input::KC_RETURN, false},     {Input::KC_CTRL, false},  
    {Input::KC_SHIFT, false},      {Input::KC_ALT, false},           
    {Input::KC_PAUSE, false},      {Input::KC_PGUP, false},
    {Input::KC_PGDOWN, false},     {Input::KC_END, false},            
    {Input::KC_HOME, false},       {Input::KC_SELECT, false},
    {Input::KC_EXECUTE, false},    {Input::KC_SNAPSHOT, false},
    {Input::KC_INSERT, false},     {Input::KC_DELETE, false},         
    {Input::KC_HELP, false},       {Input::KC_1, false},
    {Input::KC_2, false},          {Input::KC_3, false},                  
    {Input::KC_4, false},          {Input::KC_5, false},          
    {Input::KC_6, false},          {Input::KC_7, false},              
    {Input::KC_8, false},          {Input::KC_9, false},                  
    {Input::KC_0, false},          {Input::KC_A, false},
    {Input::KC_B, false},          {Input::KC_C, false},                  
    {Input::KC_D, false},          {Input::KC_E, false},          
    {Input::KC_F, false},          {Input::KC_G, false},              
    {Input::KC_H, false},          {Input::KC_I, false},                  
    {Input::KC_J, false},          {Input::KC_K, false},
    {Input::KC_L, false},          {Input::KC_M, false},                  
    {Input::KC_N, false},          {Input::KC_O, false},          
    {Input::KC_P, false},          {Input::KC_Q, false},              
    {Input::KC_R, false},          {Input::KC_S, false},                  
    {Input::KC_T, false},          {Input::KC_U, false},
    {Input::KC_V, false},          {Input::KC_W, false},                  
    {Input::KC_X, false},          {Input::KC_Y, false},          
    {Input::KC_Z, false},          {Input::KC_NUM0, false},   
    {Input::KC_NUM1, false},       {Input::KC_NUM2, false},       
    {Input::KC_NUM3, false},       {Input::KC_NUM4, false},
    {Input::KC_NUM5, false},       {Input::KC_NUM6, false},       
    {Input::KC_NUM7, false},       {Input::KC_NUM8, false}, 
    {Input::KC_NUM9, false},       {Input::KC_NUMMUL, false},
    {Input::KC_NUMPLUS, false},    {Input::KC_NUMDOT, false},     
    {Input::KC_NUMMINUS, false},   {Input::KC_NUMENTER, false},
    {Input::KC_NUMDIV, false},     {Input::KC_EXTRA1, false},   
    {Input::KC_EXTRA2, false},     {Input::KC_EXTRA3, false},
    {Input::KC_EXTRA4, false},     {Input::KC_EXTRA5, false},   
    {Input::KC_EXTRA6, false},     {Input::KC_EXTRA7, false},       
    {Input::KC_EXTRA8, false},     {Input::KC_EXTRA9, false},
    {Input::KC_EXTRA10, false},    {Input::KC_EXTRA11, false},      
    {Input::KC_EXTRA12, false},    {Input::KC_EXTRA13, false},
    {Input::KC_EXTRA14, false},    {Input::KC_EXTRA15, false},
    {Input::KC_EXTRA16, false},    {Input::KC_EXTRA17, false},        
    {Input::KC_EXTRA18, false},    {Input::KC_LMB, false},
    {Input::KC_RMB, false},        {Input::KC_MMB, false},                
    {Input::KC_MOUSEX, true},      {Input::KC_MOUSEY, true},     
    {Input::KC_JOYB0, false},      {Input::KC_JOYB1, false},          
    {Input::KC_JOYB2, false},      {Input::KC_JOYB3, false},              
    {Input::KC_JOYB4, false},      {Input::KC_JOYB5, false},
    {Input::KC_JOYB6, false},      {Input::KC_JOYB7, false},              
    {Input::KC_JOYX, true},        {Input::KC_JOYY, true},      
    {Input::KC_JOYTHROTTLE, true}, {Input::KC_JOYHATX, true},         
    {Input::KC_JOYHATY, true},     {Input::KC_JOYRUDDER, true}
}};

std::array<Input::KeyMapName, Input::KC_MAX> NC_STACK_input::KeyNamesTable {{
    {"nop", Input::KC_NONE},        {"esc", Input::KC_ESCAPE},         
    {"space", Input::KC_SPACE},     {"up", Input::KC_UP},      
    {"down", Input::KC_DOWN},       {"left", Input::KC_LEFT},      
    {"right", Input::KC_RIGHT},     {"f1", Input::KC_F1},              
    {"f2", Input::KC_F2},           {"f3", Input::KC_F3},
    {"f4", Input::KC_F4},           {"f5", Input::KC_F5},              
    {"f6", Input::KC_F6},           {"f7", Input::KC_F7},      
    {"f8", Input::KC_F8},           {"f9", Input::KC_F9},          
    {"f10", Input::KC_F10},         {"f11", Input::KC_F11},            
    {"f12", Input::KC_F12},         {"bs", Input::KC_BACKSPACE},
    {"tab", Input::KC_TAB},         {"clear", Input::KC_CLEAR},        
    {"return", Input::KC_RETURN},   {"ctrl", Input::KC_CTRL},    
    {"rshift", Input::KC_SHIFT},     {"alt", Input::KC_ALT},           
    {"pause", Input::KC_PAUSE},     {"pageup", Input::KC_PGUP},
    {"pagedown", Input::KC_PGDOWN}, {"end", Input::KC_END},            
    {"home", Input::KC_HOME},       {"select", Input::KC_SELECT},
    {"execute", Input::KC_EXECUTE}, {"snapshot", Input::KC_SNAPSHOT},
    {"ins", Input::KC_INSERT},      {"del", Input::KC_DELETE},         
    {"help", Input::KC_HELP},       {"1", Input::KC_1},
    {"2", Input::KC_2},             {"3", Input::KC_3},                  
    {"4", Input::KC_4},             {"5", Input::KC_5},          
    {"6", Input::KC_6},             {"7", Input::KC_7},              
    {"8", Input::KC_8},             {"9", Input::KC_9},                  
    {"0", Input::KC_0},             {"a", Input::KC_A},
    {"b", Input::KC_B},             {"c", Input::KC_C},                  
    {"d", Input::KC_D},             {"e", Input::KC_E},          
    {"f", Input::KC_F},             {"g", Input::KC_G},              
    {"h", Input::KC_H},             {"i", Input::KC_I},                  
    {"j", Input::KC_J},             {"k", Input::KC_K},
    {"l", Input::KC_L},             {"m", Input::KC_M},                  
    {"n", Input::KC_N},             {"o", Input::KC_O},          
    {"p", Input::KC_P},             {"q", Input::KC_Q},              
    {"r", Input::KC_R},             {"s", Input::KC_S},                  
    {"t", Input::KC_T},             {"u", Input::KC_U},
    {"v", Input::KC_V},             {"w", Input::KC_W},                  
    {"x", Input::KC_X},             {"y", Input::KC_Y},          
    {"z", Input::KC_Z},             {"num0", Input::KC_NUM0},   
    {"num1", Input::KC_NUM1},       {"num2", Input::KC_NUM2},       
    {"num3", Input::KC_NUM3},       {"num4", Input::KC_NUM4},
    {"num5", Input::KC_NUM5},       {"num6", Input::KC_NUM6},       
    {"num7", Input::KC_NUM7},       {"num8", Input::KC_NUM8}, 
    {"num9", Input::KC_NUM9},       {"nummul", Input::KC_NUMMUL},
    {"numplus", Input::KC_NUMPLUS}, {"numdot", Input::KC_NUMDOT},     
    {"numminus", Input::KC_NUMMINUS},{"enter", Input::KC_NUMENTER},
    {"numdiv", Input::KC_NUMDIV},   {"extra1", Input::KC_EXTRA1},   
    {"extra2", Input::KC_EXTRA2},   {"extra3", Input::KC_EXTRA3},
    {"extra4", Input::KC_EXTRA4},   {"extra5", Input::KC_EXTRA5},   
    {"extra6", Input::KC_EXTRA6},   {"extra7", Input::KC_EXTRA7},       
    {"extra8", Input::KC_EXTRA8},   {"extra9", Input::KC_EXTRA9},
    {"extra10", Input::KC_EXTRA10}, {"extra11", Input::KC_EXTRA11},      
    {"extra12", Input::KC_EXTRA12}, {"extra13", Input::KC_EXTRA13},
    {"extra14", Input::KC_EXTRA14}, {"extra15", Input::KC_EXTRA15},
    {"extra16", Input::KC_EXTRA16}, {"extra17", Input::KC_EXTRA17},        
    {"extra18", Input::KC_EXTRA18}, {"lmb", Input::KC_LMB},
    {"rmb", Input::KC_RMB},         {"mmb", Input::KC_MMB},                
    {"mousex", Input::KC_MOUSEX},   {"mousey", Input::KC_MOUSEY},     
    {"joyb0", Input::KC_JOYB0},     {"joyb1", Input::KC_JOYB1},          
    {"joyb2", Input::KC_JOYB2},     {"joyb3", Input::KC_JOYB3},              
    {"joyb4", Input::KC_JOYB4},     {"joyb5", Input::KC_JOYB5},
    {"joyb6", Input::KC_JOYB6},     {"joyb7", Input::KC_JOYB7},              
    {"joyx", Input::KC_JOYX},       {"joyy", Input::KC_JOYY},      
    {"joythrottle", Input::KC_JOYTHROTTLE},{"joyhatx", Input::KC_JOYHATX},         
    {"joyhaty", Input::KC_JOYHATY}, {"joyrudder", Input::KC_JOYRUDDER}
}};

std::vector<Input::KeyMapName> NC_STACK_input::KeyAltNamesTable ({
    {"shift", Input::KC_SHIFT},    {"lshift", Input::KC_SHIFT},
    {"lctrl", Input::KC_CTRL},      {"rctrl", Input::KC_CTRL},
    {"lalt", Input::KC_ALT},        {"ralt", Input::KC_CTRL},
});

std::array<std::string, Input::KC_MAX> NC_STACK_input::KeyTitle;
