#include "includes.h"
#include "engine_input.h"
#include "input.h"

INPEngine INPe;

Common::Ini::KeyList input_keyconfig
{
    Common::Ini::Key("input.debug", Common::Ini::KT_BOOL), 		                        //0
    Common::Ini::Key("input.timer", Common::Ini::KT_WORD), 		        //1
    Common::Ini::Key("input.wimp", Common::Ini::KT_WORD), 		            //2
    Common::Ini::Key("input.keyboard", Common::Ini::KT_WORD), 		    //3
    Common::Ini::Key("input.button[0]", Common::Ini::KT_STRING), 		 //4
    Common::Ini::Key("input.button[1]", Common::Ini::KT_STRING), 		 //5
    Common::Ini::Key("input.button[2]", Common::Ini::KT_STRING), 		 //6
    Common::Ini::Key("input.button[3]", Common::Ini::KT_STRING), 		 //7
    Common::Ini::Key("input.button[4]", Common::Ini::KT_STRING), 		 //8
    Common::Ini::Key("input.button[5]", Common::Ini::KT_STRING), 		 //9
    Common::Ini::Key("input.button[6]", Common::Ini::KT_STRING), 		 //10
    Common::Ini::Key("input.button[7]", Common::Ini::KT_STRING), 		 //11
    Common::Ini::Key("input.button[8]", Common::Ini::KT_STRING), 		 //12
    Common::Ini::Key("input.button[9]", Common::Ini::KT_STRING), 		 //13
    Common::Ini::Key("input.button[10]", Common::Ini::KT_STRING), 		 //14
    Common::Ini::Key("input.button[11]", Common::Ini::KT_STRING), 		 //15
    Common::Ini::Key("input.button[12]", Common::Ini::KT_STRING), 		 //16
    Common::Ini::Key("input.button[13]", Common::Ini::KT_STRING), 		 //17
    Common::Ini::Key("input.button[14]", Common::Ini::KT_STRING), 		 //18
    Common::Ini::Key("input.button[15]", Common::Ini::KT_STRING), 		 //19
    Common::Ini::Key("input.button[16]", Common::Ini::KT_STRING), 		 //20
    Common::Ini::Key("input.button[17]", Common::Ini::KT_STRING), 		 //21
    Common::Ini::Key("input.button[18]", Common::Ini::KT_STRING), 		 //22
    Common::Ini::Key("input.button[19]", Common::Ini::KT_STRING), 		 //23
    Common::Ini::Key("input.button[20]", Common::Ini::KT_STRING), 		 //24
    Common::Ini::Key("input.button[21]", Common::Ini::KT_STRING), 		 //25
    Common::Ini::Key("input.button[22]", Common::Ini::KT_STRING), 		 //26
    Common::Ini::Key("input.button[23]", Common::Ini::KT_STRING), 		 //27
    Common::Ini::Key("input.button[24]", Common::Ini::KT_STRING), 		 //28
    Common::Ini::Key("input.button[25]", Common::Ini::KT_STRING), 		 //29
    Common::Ini::Key("input.button[26]", Common::Ini::KT_STRING), 		 //30
    Common::Ini::Key("input.button[27]", Common::Ini::KT_STRING), 		 //31
    Common::Ini::Key("input.button[28]", Common::Ini::KT_STRING), 		 //32
    Common::Ini::Key("input.button[29]", Common::Ini::KT_STRING), 		 //33
    Common::Ini::Key("input.button[30]", Common::Ini::KT_STRING), 		 //34
    Common::Ini::Key("input.button[31]", Common::Ini::KT_STRING), 		 //35
    Common::Ini::Key("input.slider[0]", Common::Ini::KT_STRING), 		 //36
    Common::Ini::Key("input.slider[1]", Common::Ini::KT_STRING), 		 //37
    Common::Ini::Key("input.slider[2]", Common::Ini::KT_STRING), 		 //38
    Common::Ini::Key("input.slider[3]", Common::Ini::KT_STRING), 		 //39
    Common::Ini::Key("input.slider[4]", Common::Ini::KT_STRING), 		 //40
    Common::Ini::Key("input.slider[5]", Common::Ini::KT_STRING), 		 //41
    Common::Ini::Key("input.slider[6]", Common::Ini::KT_STRING), 		 //42
    Common::Ini::Key("input.slider[7]", Common::Ini::KT_STRING), 		 //43
    Common::Ini::Key("input.slider[8]", Common::Ini::KT_STRING), 		 //44
    Common::Ini::Key("input.slider[9]", Common::Ini::KT_STRING), 		 //45
    Common::Ini::Key("input.slider[10]", Common::Ini::KT_STRING), 		 //46
    Common::Ini::Key("input.slider[11]", Common::Ini::KT_STRING), 		 //47
    Common::Ini::Key("input.slider[12]", Common::Ini::KT_STRING), 		 //48
    Common::Ini::Key("input.slider[13]", Common::Ini::KT_STRING), 		 //49
    Common::Ini::Key("input.slider[14]", Common::Ini::KT_STRING), 		 //50
    Common::Ini::Key("input.slider[15]", Common::Ini::KT_STRING), 		 //51
    Common::Ini::Key("input.slider[16]", Common::Ini::KT_STRING), 		 //52
    Common::Ini::Key("input.slider[17]", Common::Ini::KT_STRING), 		 //53
    Common::Ini::Key("input.slider[18]", Common::Ini::KT_STRING), 		 //54
    Common::Ini::Key("input.slider[19]", Common::Ini::KT_STRING), 		 //55
    Common::Ini::Key("input.slider[20]", Common::Ini::KT_STRING), 		 //56
    Common::Ini::Key("input.slider[21]", Common::Ini::KT_STRING), 		 //57
    Common::Ini::Key("input.slider[22]", Common::Ini::KT_STRING), 		 //58
    Common::Ini::Key("input.slider[23]", Common::Ini::KT_STRING), 		 //59
    Common::Ini::Key("input.slider[24]", Common::Ini::KT_STRING), 		 //60
    Common::Ini::Key("input.slider[25]", Common::Ini::KT_STRING), 		 //61
    Common::Ini::Key("input.slider[26]", Common::Ini::KT_STRING), 		 //62
    Common::Ini::Key("input.slider[27]", Common::Ini::KT_STRING), 		 //63
    Common::Ini::Key("input.slider[28]", Common::Ini::KT_STRING), 		 //64
    Common::Ini::Key("input.slider[29]", Common::Ini::KT_STRING), 		 //65
    Common::Ini::Key("input.slider[30]", Common::Ini::KT_STRING), 		 //66
    Common::Ini::Key("input.slider[31]", Common::Ini::KT_STRING), 		 //67
    Common::Ini::Key("input.hotkey[0]", Common::Ini::KT_WORD), 		 //68
    Common::Ini::Key("input.hotkey[1]", Common::Ini::KT_WORD), 		 //69
    Common::Ini::Key("input.hotkey[2]", Common::Ini::KT_WORD), 		 //70
    Common::Ini::Key("input.hotkey[3]", Common::Ini::KT_WORD), 		 //71
    Common::Ini::Key("input.hotkey[4]", Common::Ini::KT_WORD), 		 //72
    Common::Ini::Key("input.hotkey[5]", Common::Ini::KT_WORD), 		 //73
    Common::Ini::Key("input.hotkey[6]", Common::Ini::KT_WORD), 		 //74
    Common::Ini::Key("input.hotkey[7]", Common::Ini::KT_WORD), 		 //75
    Common::Ini::Key("input.hotkey[8]", Common::Ini::KT_WORD), 		 //76
    Common::Ini::Key("input.hotkey[9]", Common::Ini::KT_WORD), 		 //77
    Common::Ini::Key("input.hotkey[10]", Common::Ini::KT_WORD), 		 //78
    Common::Ini::Key("input.hotkey[11]", Common::Ini::KT_WORD), 		 //79
    Common::Ini::Key("input.hotkey[12]", Common::Ini::KT_WORD), 		 //80
    Common::Ini::Key("input.hotkey[13]", Common::Ini::KT_WORD), 		 //81
    Common::Ini::Key("input.hotkey[14]", Common::Ini::KT_WORD), 		 //82
    Common::Ini::Key("input.hotkey[15]", Common::Ini::KT_WORD), 		 //83
    Common::Ini::Key("input.hotkey[16]", Common::Ini::KT_WORD), 		 //84
    Common::Ini::Key("input.hotkey[17]", Common::Ini::KT_WORD), 		 //85
    Common::Ini::Key("input.hotkey[18]", Common::Ini::KT_WORD), 		 //86
    Common::Ini::Key("input.hotkey[19]", Common::Ini::KT_WORD), 		 //87
    Common::Ini::Key("input.hotkey[20]", Common::Ini::KT_WORD), 		 //88
    Common::Ini::Key("input.hotkey[21]", Common::Ini::KT_WORD), 		 //89
    Common::Ini::Key("input.hotkey[22]", Common::Ini::KT_WORD), 		 //90
    Common::Ini::Key("input.hotkey[23]", Common::Ini::KT_WORD), 		 //91
    Common::Ini::Key("input.hotkey[24]", Common::Ini::KT_WORD), 		 //92
    Common::Ini::Key("input.hotkey[25]", Common::Ini::KT_WORD), 		 //93
    Common::Ini::Key("input.hotkey[26]", Common::Ini::KT_WORD), 		 //94
    Common::Ini::Key("input.hotkey[27]", Common::Ini::KT_WORD), 		 //95
    Common::Ini::Key("input.hotkey[28]", Common::Ini::KT_WORD), 		 //96
    Common::Ini::Key("input.hotkey[29]", Common::Ini::KT_WORD), 		 //97
    Common::Ini::Key("input.hotkey[30]", Common::Ini::KT_WORD), 		 //98
    Common::Ini::Key("input.hotkey[31]", Common::Ini::KT_WORD), 		 //99
    Common::Ini::Key("input.hotkey[32]", Common::Ini::KT_WORD), 		 //100
    Common::Ini::Key("input.hotkey[33]", Common::Ini::KT_WORD), 		 //101
    Common::Ini::Key("input.hotkey[34]", Common::Ini::KT_WORD), 		 //102
    Common::Ini::Key("input.hotkey[35]", Common::Ini::KT_WORD), 		 //103
    Common::Ini::Key("input.hotkey[36]", Common::Ini::KT_WORD), 		 //104
    Common::Ini::Key("input.hotkey[37]", Common::Ini::KT_WORD), 		 //105
    Common::Ini::Key("input.hotkey[38]", Common::Ini::KT_WORD), 		 //106
    Common::Ini::Key("input.hotkey[39]", Common::Ini::KT_WORD), 		 //107
    Common::Ini::Key("input.hotkey[40]", Common::Ini::KT_WORD), 		 //108
    Common::Ini::Key("input.hotkey[41]", Common::Ini::KT_WORD), 		 //109
    Common::Ini::Key("input.hotkey[42]", Common::Ini::KT_WORD), 		 //110
    Common::Ini::Key("input.hotkey[43]", Common::Ini::KT_WORD), 		 //111
    Common::Ini::Key("input.hotkey[44]", Common::Ini::KT_WORD), 		 //112
    Common::Ini::Key("input.hotkey[45]", Common::Ini::KT_WORD), 		 //113
    Common::Ini::Key("input.hotkey[46]", Common::Ini::KT_WORD), 		 //114
    Common::Ini::Key("input.hotkey[47]", Common::Ini::KT_WORD) 		     //115
};


int INPEngine::init()
{
    NC_STACK_winp::initfirst();

    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &input_keyconfig);

    input_class = Nucleus::CInit<NC_STACK_input>();

    if ( !input_class )
    {
        ypa_log_out("input.engine: ERROR: could not create input.class object.\n");
        return 0;
    }

    std::string val = input_keyconfig[1].Get<std::string>(); // input.timer

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_TIMER, val) )
            ypa_log_out("input.engine: WARNING: Timer object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Timer driver defined in prefs file.\n");
    }

    val = input_keyconfig[2].Get<std::string>(); // input.wimp

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_WIMP, val) )
            ypa_log_out("input.engine: WARNING: Wimp object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Wimp driver defined in prefs file.\n");
    }

    val = input_keyconfig[3].Get<std::string>(); // input.keyboard

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_KBD, val) )
            ypa_log_out("input.engine: WARNING: Keyboard object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Keyboard driver defined in prefs file.\n");
    }

    for (int i = 0; i < 32; i++)
    {
        val = input_keyconfig[4 + i].Get<std::string>(); // input_buttons

        if ( !val.empty() )
        {
            if ( ! input_class->SetInputExpression(false, i, val) )
                ypa_log_out("input.engine: WARNING: Button[%d] object creation failed.\n", i);
        }
    }

    for (int i = 0; i < 32; i++)
    {
        val = input_keyconfig[36 + i].Get<std::string>(); // input_sliders

        if ( !val.empty() )
        {
            if ( ! input_class->SetInputExpression(true, i, val) )
                ypa_log_out("input.engine: WARNING: Slider[%d] object creation failed.\n", i);
        }
    }

    for (int i = 0; i < 48; i++)
    {
        val = input_keyconfig[68 + i].Get<std::string>(); // input_hotkeys

        if ( !val.empty() )
        {
            if ( ! input_class->SetHotKey(i, val) )
                ypa_log_out("input.engine: WARNING: Hotkey[%d] (%s) not accepted.\n", i, val.c_str());
        }
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
