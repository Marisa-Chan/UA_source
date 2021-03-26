#include "../common.h"
#include "../ini.h"
#include "../log.h"
#include "inpt.h"
#include "../input.h"
#include "inivals.h"

INPEngine INPe;

int INPEngine::init()
{
    NC_STACK_winp::initfirst();

    System::IniConf::ReadFromNucleusIni();

    input_class = Nucleus::CInit<NC_STACK_input>();

    if ( !input_class )
    {
        ypa_log_out("input.engine: ERROR: could not create input.class object.\n");
        return 0;
    }

    std::string val = System::IniConf::InputTimer.Get<std::string>(); // input.timer

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_TIMER, val) )
            ypa_log_out("input.engine: WARNING: Timer object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Timer driver defined in prefs file.\n");
    }

    val = System::IniConf::InputWimp.Get<std::string>(); // input.wimp

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_WIMP, val) )
            ypa_log_out("input.engine: WARNING: Wimp object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Wimp driver defined in prefs file.\n");
    }

    val = System::IniConf::InputKeyboard.Get<std::string>(); // input.keyboard

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_KBD, val) )
            ypa_log_out("input.engine: WARNING: Keyboard object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Keyboard driver defined in prefs file.\n");
    }
    
    Common::Ini::Key *Buttons[] = {
        &System::IniConf::InputButton0
    ,   &System::IniConf::InputButton1
    ,   &System::IniConf::InputButton2
    ,   &System::IniConf::InputButton3
    ,   &System::IniConf::InputButton4
    ,   &System::IniConf::InputButton5
    ,   &System::IniConf::InputButton6
    ,   &System::IniConf::InputButton7
    ,   &System::IniConf::InputButton8
    ,   &System::IniConf::InputButton9
    ,   &System::IniConf::InputButton10
    ,   &System::IniConf::InputButton11
    ,   &System::IniConf::InputButton12
    ,   &System::IniConf::InputButton13
    ,   &System::IniConf::InputButton14
    ,   &System::IniConf::InputButton15
    ,   &System::IniConf::InputButton16
    ,   &System::IniConf::InputButton17
    ,   &System::IniConf::InputButton18
    ,   &System::IniConf::InputButton19
    ,   &System::IniConf::InputButton20
    ,   &System::IniConf::InputButton21
    ,   &System::IniConf::InputButton22
    ,   &System::IniConf::InputButton23
    ,   &System::IniConf::InputButton24
    ,   &System::IniConf::InputButton25
    ,   &System::IniConf::InputButton26
    ,   &System::IniConf::InputButton27
    ,   &System::IniConf::InputButton28
    ,   &System::IniConf::InputButton29
    ,   &System::IniConf::InputButton30
    ,   &System::IniConf::InputButton31 };

    int i = 0;
    for (Common::Ini::Key *k : Buttons)
    {
        val = k->Get<std::string>(); // input_buttons

        if ( !val.empty() )
        {
            if ( ! input_class->SetInputExpression(false, i, val) )
                ypa_log_out("input.engine: WARNING: Button[%d] object creation failed.\n", i);
        }
        i++;
    }
    
    Common::Ini::Key *Sliders[] = {
        &System::IniConf::InputSlider0
    ,   &System::IniConf::InputSlider1
    ,   &System::IniConf::InputSlider2
    ,   &System::IniConf::InputSlider3
    ,   &System::IniConf::InputSlider4
    ,   &System::IniConf::InputSlider5
    ,   &System::IniConf::InputSlider6
    ,   &System::IniConf::InputSlider7
    ,   &System::IniConf::InputSlider8
    ,   &System::IniConf::InputSlider9
    ,   &System::IniConf::InputSlider10
    ,   &System::IniConf::InputSlider11
    ,   &System::IniConf::InputSlider12
    ,   &System::IniConf::InputSlider13
    ,   &System::IniConf::InputSlider14
    ,   &System::IniConf::InputSlider15
    ,   &System::IniConf::InputSlider16
    ,   &System::IniConf::InputSlider17
    ,   &System::IniConf::InputSlider18
    ,   &System::IniConf::InputSlider19
    ,   &System::IniConf::InputSlider20
    ,   &System::IniConf::InputSlider21
    ,   &System::IniConf::InputSlider22
    ,   &System::IniConf::InputSlider23
    ,   &System::IniConf::InputSlider24
    ,   &System::IniConf::InputSlider25
    ,   &System::IniConf::InputSlider26
    ,   &System::IniConf::InputSlider27
    ,   &System::IniConf::InputSlider28
    ,   &System::IniConf::InputSlider29
    ,   &System::IniConf::InputSlider30
    ,   &System::IniConf::InputSlider31
    };

    i = 0;
    for (Common::Ini::Key *k : Sliders)
    {
        val = k->Get<std::string>(); // input_sliders

        if ( !val.empty() )
        {
            if ( ! input_class->SetInputExpression(true, i, val) )
                ypa_log_out("input.engine: WARNING: Slider[%d] object creation failed.\n", i);
        }
        i++;
    }
    
    Common::Ini::Key *Hotkeys[] = {   
        &System::IniConf::InputHotkey0
    ,   &System::IniConf::InputHotkey1
    ,   &System::IniConf::InputHotkey2
    ,   &System::IniConf::InputHotkey3
    ,   &System::IniConf::InputHotkey4
    ,   &System::IniConf::InputHotkey5
    ,   &System::IniConf::InputHotkey6
    ,   &System::IniConf::InputHotkey7
    ,   &System::IniConf::InputHotkey8
    ,   &System::IniConf::InputHotkey9
    ,   &System::IniConf::InputHotkey10
    ,   &System::IniConf::InputHotkey11
    ,   &System::IniConf::InputHotkey12
    ,   &System::IniConf::InputHotkey13
    ,   &System::IniConf::InputHotkey14
    ,   &System::IniConf::InputHotkey15
    ,   &System::IniConf::InputHotkey16
    ,   &System::IniConf::InputHotkey17
    ,   &System::IniConf::InputHotkey18
    ,   &System::IniConf::InputHotkey19
    ,   &System::IniConf::InputHotkey20
    ,   &System::IniConf::InputHotkey21
    ,   &System::IniConf::InputHotkey22
    ,   &System::IniConf::InputHotkey23
    ,   &System::IniConf::InputHotkey24
    ,   &System::IniConf::InputHotkey25
    ,   &System::IniConf::InputHotkey26
    ,   &System::IniConf::InputHotkey27
    ,   &System::IniConf::InputHotkey28
    ,   &System::IniConf::InputHotkey29
    ,   &System::IniConf::InputHotkey30
    ,   &System::IniConf::InputHotkey31
    ,   &System::IniConf::InputHotkey32
    ,   &System::IniConf::InputHotkey33
    ,   &System::IniConf::InputHotkey34
    ,   &System::IniConf::InputHotkey35
    ,   &System::IniConf::InputHotkey36
    ,   &System::IniConf::InputHotkey37
    ,   &System::IniConf::InputHotkey38
    ,   &System::IniConf::InputHotkey39
    ,   &System::IniConf::InputHotkey40
    ,   &System::IniConf::InputHotkey41
    ,   &System::IniConf::InputHotkey42
    ,   &System::IniConf::InputHotkey43
    ,   &System::IniConf::InputHotkey44
    ,   &System::IniConf::InputHotkey45
    ,   &System::IniConf::InputHotkey46
    ,   &System::IniConf::InputHotkey47
    };

    i = 0;
    for (Common::Ini::Key *k : Hotkeys)
    {
        val = k->Get<std::string>(); // input_hotkeys

        if ( !val.empty() )
        {
            if ( ! input_class->SetHotKey(i, val) )
                ypa_log_out("input.engine: WARNING: Hotkey[%d] (%s) not accepted.\n", i, val.c_str());
        }
        i++;
    }

    return 1;
}

void INPEngine::deinit()
{
    if ( input_class )
    {
        delete_class_obj(input_class);
        input_class = NULL;
    }
}

NC_STACK_input *INPEngine::getPInput()
{
    return input_class;
}


void INPEngine::QueryInput(InputState *a1)
{
    input_class->QueryInput(a1);
}

void INPEngine::AddClickBoxFront(ClickBox *box)
{
    input_class->wimp_addClickNodeFront(box);
}

void INPEngine::AddClickBoxBack(ClickBox *box)
{
    input_class->wimp_addClickNodeBack(box);
}

void INPEngine::RemClickBox(ClickBox *box)
{
    input_class->wimp_remClickNode(box);
}


void InputState::Clear()
{
    Period = 0;
    KbdLastDown = Input::KC_NONE;
    KbdLastHit   = Input::KC_NONE;
    HotKeyID     = -1;
    chr          = 0;

    for ( float &sld : Sliders )
        sld = 0.0;

    Buttons.clear();
    ClickInf.clear();
}
