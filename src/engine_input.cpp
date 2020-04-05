#include "includes.h"
#include "engine_input.h"
#include "input.h"

INPEngine INPe;

char input_timer[128];
char input_wimp[128];
char input_keyboard[128];
char input_buttons[32][128];
char input_sliders[32][128];
char input_hotkeys[48][128];

key_value_stru input_keyconfig[116] =
{
    {"input.debug", KEY_TYPE_BOOL, 0}, 		                        //0
    {"input.timer", KEY_TYPE_STRING1, input_timer}, 		        //1
    {"input.wimp", KEY_TYPE_STRING1, input_wimp}, 		            //2
    {"input.keyboard", KEY_TYPE_STRING1, input_keyboard}, 		    //3
    {"input.button[0]", KEY_TYPE_STRING2, &input_buttons[0]}, 		 //4
    {"input.button[1]", KEY_TYPE_STRING2, &input_buttons[1]}, 		 //5
    {"input.button[2]", KEY_TYPE_STRING2, &input_buttons[2]}, 		 //6
    {"input.button[3]", KEY_TYPE_STRING2, &input_buttons[3]}, 		 //7
    {"input.button[4]", KEY_TYPE_STRING2, &input_buttons[4]}, 		 //8
    {"input.button[5]", KEY_TYPE_STRING2, &input_buttons[5]}, 		 //9
    {"input.button[6]", KEY_TYPE_STRING2, &input_buttons[6]}, 		 //10
    {"input.button[7]", KEY_TYPE_STRING2, &input_buttons[7]}, 		 //11
    {"input.button[8]", KEY_TYPE_STRING2, &input_buttons[8]}, 		 //12
    {"input.button[9]", KEY_TYPE_STRING2, &input_buttons[9]}, 		 //13
    {"input.button[10]", KEY_TYPE_STRING2, &input_buttons[10]}, 		 //14
    {"input.button[11]", KEY_TYPE_STRING2, &input_buttons[11]}, 		 //15
    {"input.button[12]", KEY_TYPE_STRING2, &input_buttons[12]}, 		 //16
    {"input.button[13]", KEY_TYPE_STRING2, &input_buttons[13]}, 		 //17
    {"input.button[14]", KEY_TYPE_STRING2, &input_buttons[14]}, 		 //18
    {"input.button[15]", KEY_TYPE_STRING2, &input_buttons[15]}, 		 //19
    {"input.button[16]", KEY_TYPE_STRING2, &input_buttons[16]}, 		 //20
    {"input.button[17]", KEY_TYPE_STRING2, &input_buttons[17]}, 		 //21
    {"input.button[18]", KEY_TYPE_STRING2, &input_buttons[18]}, 		 //22
    {"input.button[19]", KEY_TYPE_STRING2, &input_buttons[19]}, 		 //23
    {"input.button[20]", KEY_TYPE_STRING2, &input_buttons[20]}, 		 //24
    {"input.button[21]", KEY_TYPE_STRING2, &input_buttons[21]}, 		 //25
    {"input.button[22]", KEY_TYPE_STRING2, &input_buttons[22]}, 		 //26
    {"input.button[23]", KEY_TYPE_STRING2, &input_buttons[23]}, 		 //27
    {"input.button[24]", KEY_TYPE_STRING2, &input_buttons[24]}, 		 //28
    {"input.button[25]", KEY_TYPE_STRING2, &input_buttons[25]}, 		 //29
    {"input.button[26]", KEY_TYPE_STRING2, &input_buttons[26]}, 		 //30
    {"input.button[27]", KEY_TYPE_STRING2, &input_buttons[27]}, 		 //31
    {"input.button[28]", KEY_TYPE_STRING2, &input_buttons[28]}, 		 //32
    {"input.button[29]", KEY_TYPE_STRING2, &input_buttons[29]}, 		 //33
    {"input.button[30]", KEY_TYPE_STRING2, &input_buttons[30]}, 		 //34
    {"input.button[31]", KEY_TYPE_STRING2, &input_buttons[31]}, 		 //35
    {"input.slider[0]", KEY_TYPE_STRING2, &input_sliders[0]}, 		 //36
    {"input.slider[1]", KEY_TYPE_STRING2, &input_sliders[1]}, 		 //37
    {"input.slider[2]", KEY_TYPE_STRING2, &input_sliders[2]}, 		 //38
    {"input.slider[3]", KEY_TYPE_STRING2, &input_sliders[3]}, 		 //39
    {"input.slider[4]", KEY_TYPE_STRING2, &input_sliders[4]}, 		 //40
    {"input.slider[5]", KEY_TYPE_STRING2, &input_sliders[5]}, 		 //41
    {"input.slider[6]", KEY_TYPE_STRING2, &input_sliders[6]}, 		 //42
    {"input.slider[7]", KEY_TYPE_STRING2, &input_sliders[7]}, 		 //43
    {"input.slider[8]", KEY_TYPE_STRING2, &input_sliders[8]}, 		 //44
    {"input.slider[9]", KEY_TYPE_STRING2, &input_sliders[9]}, 		 //45
    {"input.slider[10]", KEY_TYPE_STRING2, &input_sliders[10]}, 		 //46
    {"input.slider[11]", KEY_TYPE_STRING2, &input_sliders[11]}, 		 //47
    {"input.slider[12]", KEY_TYPE_STRING2, &input_sliders[12]}, 		 //48
    {"input.slider[13]", KEY_TYPE_STRING2, &input_sliders[13]}, 		 //49
    {"input.slider[14]", KEY_TYPE_STRING2, &input_sliders[14]}, 		 //50
    {"input.slider[15]", KEY_TYPE_STRING2, &input_sliders[15]}, 		 //51
    {"input.slider[16]", KEY_TYPE_STRING2, &input_sliders[16]}, 		 //52
    {"input.slider[17]", KEY_TYPE_STRING2, &input_sliders[17]}, 		 //53
    {"input.slider[18]", KEY_TYPE_STRING2, &input_sliders[18]}, 		 //54
    {"input.slider[19]", KEY_TYPE_STRING2, &input_sliders[19]}, 		 //55
    {"input.slider[20]", KEY_TYPE_STRING2, &input_sliders[20]}, 		 //56
    {"input.slider[21]", KEY_TYPE_STRING2, &input_sliders[21]}, 		 //57
    {"input.slider[22]", KEY_TYPE_STRING2, &input_sliders[22]}, 		 //58
    {"input.slider[23]", KEY_TYPE_STRING2, &input_sliders[23]}, 		 //59
    {"input.slider[24]", KEY_TYPE_STRING2, &input_sliders[24]}, 		 //60
    {"input.slider[25]", KEY_TYPE_STRING2, &input_sliders[25]}, 		 //61
    {"input.slider[26]", KEY_TYPE_STRING2, &input_sliders[26]}, 		 //62
    {"input.slider[27]", KEY_TYPE_STRING2, &input_sliders[27]}, 		 //63
    {"input.slider[28]", KEY_TYPE_STRING2, &input_sliders[28]}, 		 //64
    {"input.slider[29]", KEY_TYPE_STRING2, &input_sliders[29]}, 		 //65
    {"input.slider[30]", KEY_TYPE_STRING2, &input_sliders[30]}, 		 //66
    {"input.slider[31]", KEY_TYPE_STRING2, &input_sliders[31]}, 		 //67
    {"input.hotkey[0]", KEY_TYPE_STRING1, &input_hotkeys[0]}, 		 //68
    {"input.hotkey[1]", KEY_TYPE_STRING1, &input_hotkeys[1]}, 		 //69
    {"input.hotkey[2]", KEY_TYPE_STRING1, &input_hotkeys[2]}, 		 //70
    {"input.hotkey[3]", KEY_TYPE_STRING1, &input_hotkeys[3]}, 		 //71
    {"input.hotkey[4]", KEY_TYPE_STRING1, &input_hotkeys[4]}, 		 //72
    {"input.hotkey[5]", KEY_TYPE_STRING1, &input_hotkeys[5]}, 		 //73
    {"input.hotkey[6]", KEY_TYPE_STRING1, &input_hotkeys[6]}, 		 //74
    {"input.hotkey[7]", KEY_TYPE_STRING1, &input_hotkeys[7]}, 		 //75
    {"input.hotkey[8]", KEY_TYPE_STRING1, &input_hotkeys[8]}, 		 //76
    {"input.hotkey[9]", KEY_TYPE_STRING1, &input_hotkeys[9]}, 		 //77
    {"input.hotkey[10]", KEY_TYPE_STRING1, &input_hotkeys[10]}, 		 //78
    {"input.hotkey[11]", KEY_TYPE_STRING1, &input_hotkeys[11]}, 		 //79
    {"input.hotkey[12]", KEY_TYPE_STRING1, &input_hotkeys[12]}, 		 //80
    {"input.hotkey[13]", KEY_TYPE_STRING1, &input_hotkeys[13]}, 		 //81
    {"input.hotkey[14]", KEY_TYPE_STRING1, &input_hotkeys[14]}, 		 //82
    {"input.hotkey[15]", KEY_TYPE_STRING1, &input_hotkeys[15]}, 		 //83
    {"input.hotkey[16]", KEY_TYPE_STRING1, &input_hotkeys[16]}, 		 //84
    {"input.hotkey[17]", KEY_TYPE_STRING1, &input_hotkeys[17]}, 		 //85
    {"input.hotkey[18]", KEY_TYPE_STRING1, &input_hotkeys[18]}, 		 //86
    {"input.hotkey[19]", KEY_TYPE_STRING1, &input_hotkeys[19]}, 		 //87
    {"input.hotkey[20]", KEY_TYPE_STRING1, &input_hotkeys[20]}, 		 //88
    {"input.hotkey[21]", KEY_TYPE_STRING1, &input_hotkeys[21]}, 		 //89
    {"input.hotkey[22]", KEY_TYPE_STRING1, &input_hotkeys[22]}, 		 //90
    {"input.hotkey[23]", KEY_TYPE_STRING1, &input_hotkeys[23]}, 		 //91
    {"input.hotkey[24]", KEY_TYPE_STRING1, &input_hotkeys[24]}, 		 //92
    {"input.hotkey[25]", KEY_TYPE_STRING1, &input_hotkeys[25]}, 		 //93
    {"input.hotkey[26]", KEY_TYPE_STRING1, &input_hotkeys[26]}, 		 //94
    {"input.hotkey[27]", KEY_TYPE_STRING1, &input_hotkeys[27]}, 		 //95
    {"input.hotkey[28]", KEY_TYPE_STRING1, &input_hotkeys[28]}, 		 //96
    {"input.hotkey[29]", KEY_TYPE_STRING1, &input_hotkeys[29]}, 		 //97
    {"input.hotkey[30]", KEY_TYPE_STRING1, &input_hotkeys[30]}, 		 //98
    {"input.hotkey[31]", KEY_TYPE_STRING1, &input_hotkeys[31]}, 		 //99
    {"input.hotkey[32]", KEY_TYPE_STRING1, &input_hotkeys[32]}, 		 //100
    {"input.hotkey[33]", KEY_TYPE_STRING1, &input_hotkeys[33]}, 		 //101
    {"input.hotkey[34]", KEY_TYPE_STRING1, &input_hotkeys[34]}, 		 //102
    {"input.hotkey[35]", KEY_TYPE_STRING1, &input_hotkeys[35]}, 		 //103
    {"input.hotkey[36]", KEY_TYPE_STRING1, &input_hotkeys[36]}, 		 //104
    {"input.hotkey[37]", KEY_TYPE_STRING1, &input_hotkeys[37]}, 		 //105
    {"input.hotkey[38]", KEY_TYPE_STRING1, &input_hotkeys[38]}, 		 //106
    {"input.hotkey[39]", KEY_TYPE_STRING1, &input_hotkeys[39]}, 		 //107
    {"input.hotkey[40]", KEY_TYPE_STRING1, &input_hotkeys[40]}, 		 //108
    {"input.hotkey[41]", KEY_TYPE_STRING1, &input_hotkeys[41]}, 		 //109
    {"input.hotkey[42]", KEY_TYPE_STRING1, &input_hotkeys[42]}, 		 //110
    {"input.hotkey[43]", KEY_TYPE_STRING1, &input_hotkeys[43]}, 		 //111
    {"input.hotkey[44]", KEY_TYPE_STRING1, &input_hotkeys[44]}, 		 //112
    {"input.hotkey[45]", KEY_TYPE_STRING1, &input_hotkeys[45]}, 		 //113
    {"input.hotkey[46]", KEY_TYPE_STRING1, &input_hotkeys[46]}, 		 //114
    {"input.hotkey[47]", KEY_TYPE_STRING1, &input_hotkeys[47]} 		     //115
};


int INPEngine::init()
{
    NC_STACK_winp::initfirst();

    memset(input_timer, 0, sizeof(input_timer));
    memset(input_wimp, 0, sizeof(input_wimp));
    memset(input_keyboard, 0, sizeof(input_keyboard));
    memset(input_buttons, 0, sizeof(input_buttons));
    memset(input_sliders, 0, sizeof(input_sliders));
    memset(input_hotkeys, 0, sizeof(input_hotkeys));

    get_keyvalue_from_ini(0, input_keyconfig, 116);

    input_class = Nucleus::CInit<NC_STACK_input>();

    if ( !input_class )
    {
        ypa_log_out("input.engine: ERROR: could not create input.class object.\n");
        return 0;
    }

    std::string val = (const char *)input_keyconfig[1].value.pval; // input.timer

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_TIMER, val) )
            ypa_log_out("input.engine: WARNING: Timer object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Timer driver defined in prefs file.\n");
    }

    val = (const char *)input_keyconfig[2].value.pval; // input.wimp

    if ( !val.empty() )
    {
        if ( ! input_class->InitDriver(Input::ITYPE_WIMP, val) )
            ypa_log_out("input.engine: WARNING: Wimp object creation failed.\n");
    }
    else
    {
        ypa_log_out("input.engine: WARNING: no Wimp driver defined in prefs file.\n");
    }

    val = (const char *)input_keyconfig[3].value.pval; // input.keyboard

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
        val = (const char *)input_keyconfig[4 + i].value.pval; // input_buttons

        if ( !val.empty() )
        {
            if ( ! input_class->input_func64(Input::ITYPE_BUTTON, i, val) )
                ypa_log_out("input.engine: WARNING: Button[%d] object creation failed.\n", i);
        }
    }

    for (int i = 0; i < 32; i++)
    {
        val = (const char *)input_keyconfig[36 + i].value.pval; // input_sliders

        if ( !val.empty() )
        {
            if ( ! input_class->input_func64(Input::ITYPE_SLIDER, i, val) )
                ypa_log_out("input.engine: WARNING: Slider[%d] object creation failed.\n", i);
        }
    }

    for (int i = 0; i < 48; i++)
    {
        val = (const char *)input_keyconfig[68 + i].value.pval; // input_hotkeys

        if ( !val.empty() )
        {
            winp_68arg arg68;
            arg68.keyname = val;
            arg68.id = i;

            if ( ! input_class->keyb_setHotkey(&arg68) )
                ypa_log_out("input.engine: WARNING: Hotkey[%d] (%s) not accepted.\n", i, val);
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


void INPEngine::sub_412D28(struC5 *a1)
{
    input_class->input_func65(a1);
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

