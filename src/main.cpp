#define GLOBAL
#include "includes.h"
#include "engine_gfx.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "ilbm.h"

#include "font.h"
#include "yw.h"

#include "button.h"

#include "engine_gfx.h"
#include "engine_input.h"

int dword_513638 = 0;

int sub_4107FC(UserData *usr)
{
    yw_arg172 arg171;

    arg171.usr = usr;
    arg171.usertxt = "settings.tmp";
    arg171.field_4 = usr->user_name;
    arg171.field_8 = 193;
    arg171.field_10 = 0;

    return call_method(ypaworld, 171, &arg171);
}

void sub_410628()
{
    char buf[300];
    sprintf(buf, "%s/user.txt", userdata.user_name);

    yw_arg172 arg171;
    arg171.usertxt = buf;
    arg171.field_4 = userdata.user_name;
    arg171.usr = &userdata;
    arg171.field_8 = 255;
    arg171.field_10 = 0;

    call_method(ypaworld, 171, &arg171);

    FILE *fil = FOpen("env:user.def", "w");
    if ( fil )
    {
        strcpy(buf, userdata.user_name);
        fwrite(buf, strlen(buf), 1, fil);
        FClose(fil);
    }
}

int sb_0x411324__sub0()
{
    call_method(ypaworld, 64, &world_update_arg);

    stru_2d90 *var_2d90;
    call_vtbl(ypaworld, 3, 0x8000201A, &var_2d90, 0);

    char buf[200];

    switch( var_2d90->field_40 )
    {
    case 1:
    case 2:
    {
        call_method(ypaworld, 151, 0);

        if ( dword_513638 || var_2d90->field_40 == 2 )
        {
            yw_arg172 arg172;
            arg172.usr = &userdata;
            arg172.usertxt = "settings.tmp";
            arg172.field_10 = 0;
            arg172.field_4 = userdata.user_name;
            arg172.field_8 = 0xC1;

            if ( !call_method(ypaworld, 172, &arg172) )
                return 0;

            dword_513638 = 0;
        }

        int v0 = 1;

        if ( userdata.field_1CEA )
            v0 = 0;

        if ( userdata.field_46 == 6 )
        {
            userdata.field_4E = 1;
            userdata.field_46 = 5;
        }
        else
        {
            userdata.field_4E = 0;
        }

        dword_520400 = 0;
        world_update_arg.field_0 = 0;

        userdata.field_5457 = 0;
        userdata.field_0xc = 1;

        if ( call_method(ypaworld, 156, &userdata) )
        {
            dword_520400 = 1;
            sub_412D28(&input_states);

            if (!v0)
                return 0;
        }
        else
        {
            ypa_log_out("GameShell-Error!!!\n");
            call_method(ypaworld, 155, &userdata);
            return 0;
        }
    }
    break;

    case 4:
    {
        call_method(ypaworld, 151, 0);

        sprintf(buf, "save:%s/%d.rst", userdata.user_name, var_2d90->levelID);

        yw_arg169 arg169;
        arg169.saveFile = buf;
        arg169.usr = &userdata;

        if ( !call_method(ypaworld, 169, &arg169) )
            ypa_log_out("Warning, load error\n");

        sub_412D28(&input_states);
    }
    break;

    case 6:
    {
        yw_arg169 arg169;
        arg169.usr = &userdata;
        arg169.saveFile = buf;

        sprintf(buf, "save:%s/%d.sgm", userdata.user_name, 0);

        if ( !call_method(ypaworld, 170, &arg169) )
            ypa_log_out("Warning, Save error\n");

        sub_412D28(&input_states);
    }
    break;

    case 7:
    {
        call_method(ypaworld, 151, 0);

        yw_arg169 arg169;
        arg169.usr = &userdata;
        arg169.saveFile = buf;
        sprintf(buf, "save:%s/%d.sgm", userdata.user_name, 0);

        if ( !call_method(ypaworld, 169, &arg169) )
            ypa_log_out("Warning, load error\n");

        sub_412D28(&input_states);
    }
    break;

    default:
        break;
    }

    return 1;
}

int sb_0x411324__sub2()
{
    dprintf("MAKE ME %s\n","sb_0x411324__sub0");
    return 0;
}

int sb_0x411324__sub1()
{
    userdata.field_42 = world_update_arg.field_0;
    userdata.field_3E = world_update_arg.field_4;
    userdata.field_3A = &input_states;

    call_method(ypaworld, 158, &userdata);

    int result = 1;

    if ( userdata.field_0x2fbc == 1 )
        return 0;
    else if ( userdata.field_0x2fbc == 2 )
    {
        sub_410628();
        call_method(ypaworld, 157, &userdata);

        dword_520400 = 0;

        if ( !sub_4107FC(&userdata) )
            return 0;

        yw_arg161 v22;
        v22.lvlID = userdata.field_0x2fc0;
        v22.field_4 = 0;

        if ( !call_method(ypaworld, 183, &v22) )
        {
            ypa_log_out("Sorry, unable to init this level!\n");

            call_method(ypaworld, 155, &userdata);
            result = 0;
        }
        dword_520400 = 2;
        sub_412D28(&input_states);
    }
    else if ( userdata.field_0x2fbc == 3 )
    {
        dword_520400 = 0;

        stru_2d90 *a4;
        call_vtbl(ypaworld, 3, 0x8000201A, &a4, 0);

        sub_410628();
        call_method(ypaworld, 157, &userdata);

        if ( !sub_4107FC(&userdata) )
            return 0;

        char a1[200];
        yw_arg169 arg169;
        arg169.saveFile = a1;
        arg169.usr = &userdata;

        sprintf(a1, "save:%s/%d.sgm", userdata.user_name, 0);

        if ( !call_method(ypaworld, 169, &arg169) )
        {
            ypa_log_out("Error while loading level (level %d, User %s\n", a4->levelID, userdata.user_name);

            call_method(ypaworld, 155, &userdata);
            result = 0;
        }
        dword_520400 = 2;
        sub_412D28(&input_states);
    }
    else if ( userdata.field_0x2fbc == 4 )
    {

    }
    else if ( userdata.field_0x2fbc == 5 )
    {

    }

    return result;
}


int sb_0x411324()
{
    memset(&input_states, 0, sizeof(struC5));
    sub_412D28(&input_states);

    if ( userdata.field_0x10 )
    {
        input_states.period = 1;
        userdata.field_0x10 = 0;
    }

    world_update_arg.field_4 = input_states.period + 1;
    world_update_arg.field_8 = &input_states;

    world_update_arg.field_0 += input_states.period + 1;
    input_states.period++;

    if ( dword_520400 == 1 )
    {
        return sb_0x411324__sub1();
    }
    else if ( dword_520400 == 2 )
    {
        return sb_0x411324__sub0();
    }
    else if ( dword_520400 == 3 )
    {
        return sb_0x411324__sub2();
    }
    return 1;
}

glob_funcs *init_classesLists_and_variables()
{
    memset(&engines, 0, sizeof(engines));
    init_list(&engines.allocated_class_list);
    init_list(&engines.stru_525D68);
    init_mc_res_class_engine_strings(MC_TYPE_RES, NULL);
    init_mc_res_class_engine_strings(MC_TYPE_CLASS, NULL);
    init_mc_res_class_engine_strings(MC_TYPE_ENGINE, NULL);
    ypa_log__ypa_general_log();
    init_d3dlog();
    init_dinputlog();
    if ( set_classes_list() )
    {
        return &off_51363C;
    }
    else
    {
        return NULL;
    }
}

int add_to_params_list(const char *a1)
{
    if ( engines.some_params_count >= 31 )
        return 0;

    engines.some_params_pointers[engines.some_params_count] = a1;
    engines.some_params_count++;
    return 1;
}



void deinit_globl_engines()
{
    if ( ptform_engine )
        deinit_engine(MC2_TFORM_ENGINE);
    if ( pinput_engine )
        deinit_engine(MC2_INPUT_ENGINE);
    if ( paudio_engine )
        deinit_engine(MC2_AUDIO_ENGINE);
    if ( pgfx_engine )
        deinit_engine(MC2_GFX_ENGINE);
    if ( point_to_off_51363C )
        sb_0x411c08();
}

int WinMain__sub0__sub0()
{
    ypaworld = 0;
    dword_520400 = 0;
    memset(&userdata, 0, sizeof(UserData));
    memset(&input_states, 0, sizeof(struC5));
    memset(&world_update_arg, 0, sizeof(base_64arg));

    point_to_off_51363C = init_classesLists_and_variables();
    if ( !point_to_off_51363C )
    {
        sub_412038("Couldn't open Nucleus!");
        deinit_globl_engines();
        return 0;
    }

    add_to_params_list("gfx.display  = win3d.class");
    add_to_params_list("gfx.display2 = windd.class");
    add_to_params_list("gfx.engine     = gfx.engine");
    add_to_params_list("tform.engine   = tform_ng.engine");
    add_to_params_list("input.engine   = input.engine");
    add_to_params_list("input.wimp     = winp");
    add_to_params_list("input.keyboard = winp");
    add_to_params_list("input.slider[10] = winp:mousex");
    add_to_params_list("input.slider[11] = winp:mousey");
    add_to_params_list("input.slider[12] = winp:joyx winp:joyrudder");
    add_to_params_list("input.slider[13] = winp:joyy");
    add_to_params_list("input.slider[14] = winp:joythrottle");
    add_to_params_list("input.slider[15] = winp:joyhatx");
    add_to_params_list("input.slider[16] = winp:joyhaty");
    add_to_params_list("input.button[16] = winp:joyb0");
    add_to_params_list("input.button[17] = winp:joyb1");
    add_to_params_list("input.button[18] = winp:joyb2");
    add_to_params_list("input.button[19] = winp:joyb3");
    add_to_params_list("input.button[20] = winp:joyb4");
    add_to_params_list("input.button[21] = winp:joyb5");
    add_to_params_list("input.button[22] = winp:joyb6");
    add_to_params_list("input.button[23] = winp:joyb7");
    pgfx_engine = init_engine(MC2_GFX_ENGINE);
    paudio_engine = init_engine(MC2_AUDIO_ENGINE);
    pinput_engine = init_engine(MC2_INPUT_ENGINE);
    ptform_engine = init_engine(MC2_TFORM_ENGINE);

    if ( !paudio_engine )
    {
        sub_412038("Couldn't open audio engine!");
        deinit_globl_engines();
        return 0;
    }

    if ( !pgfx_engine )
    {
        sub_412038("Couldn't open gfx engine!");
        deinit_globl_engines();
        return 0;
    }
    if ( !ptform_engine )
    {
        sub_412038("Couldn't open tform engine!");
        deinit_globl_engines();
        return 0;
    }
    if ( !pinput_engine )
    {
        sub_412038("Couldn't open input engine!");
        deinit_globl_engines();
        return 0;
    }

////HACK, don't use not used variables
//	int width, height;
//    windd__window_params *window_p;
//    size_t v3;
//
//	pgfx_engine->getter(0x80003003, &width, 0x80003004, &height, 0x80003002, &v3, 0x80003007, &window_p, 0);
//	pinput_engine->setter(0x80001004, width, 0x80001005, height, 0x80001006, v3, 0x80001007, window_p, 0);

    windd__window_params *window_p;
    pgfx_engine->getter(0x80003007, &window_p, 0);
    pinput_engine->setter(0x80001007, window_p, 0);

    return 1;
}

int yw_initGameWithSettings()
{
    FILE *user_def = FOpen("env:user.def", "r");

    char a1[300];

    if ( user_def )
    {
        char v11[300];

        fgets(v11, 300, user_def);

        sprintf(a1, "save:%s/user.txt", v11);

        FILE *user_txt = FOpen(a1, "r");

        if ( user_txt )
        {
            FClose(user_txt);

            strcpy(userdata.user_name, v11);
            sprintf(a1, "%s/user.txt", v11);
        }
        else
        {
            ypa_log_out("Warning: default user file doesn't exist (%s)\n", a1);
            strcpy(a1, "sdu7/user.txt");
            strcpy(userdata.user_name, "SDU7");
        }

        FClose(user_def);
    }
    else
    {
        strcpy(a1, "sdu7/user.txt");
        strcpy(userdata.user_name, "SDU7");
        ypa_log_out("Warning: No default user set\n");
    }

    userdata.field_1612 = -1;

    int v8 = 1;
    profilesNode *node = (profilesNode *)userdata.files_list.head;

    while ( node->next )
    {
        if ( !strcasecmp(node->profile_subdir, userdata.user_name) )
        {
            userdata.field_1612 = v8;
            break;
        }

        v8++;
        node = (profilesNode *)node->next;
    }

    yw_arg172 v13;

    v13.usertxt = a1;
    v13.usr = &userdata;
    v13.field_4 = userdata.user_name;
    v13.field_8 = 255;
    v13.field_10 = 1;

    return call_method(ypaworld, 172, &v13) != 0;
}

void ReadSnapsDir()
{
    ncDir *dir = OpenDir("env/snaps/");

    if ( dir )
    {
        dirEntry entr;
        while ( ReadDir(dir, &entr) )
        {
            if ( !(entr.field_0 & 1) && userdata.snap_count < 32 && !strnicmp(entr.e_name, "demo", 4) )
            {
                sprintf( userdata.snaps[ userdata.snap_count ], "env/snaps/%s", entr.e_name);
                userdata.snap_count++;
            }
        }
        CloseDir(dir);
    }
}

int WinMain__sub0__sub1()
{
    char buildDate[256];
    strcpy(buildDate, "Jul 09 1998  23:52:47");
//    strcpy(buildDate, __DATE__);
//    strcat(buildDate, " ");
//    strcat(buildDate, __TIME__);

    ypaworld = (NC_STACK_ypaworld *)init_get_class("ypaworld.class", 0x8000201D, buildDate, 0);
    if ( !ypaworld )
    {
        ypa_log_out("Unable to init ypaworld.class\n");
        return 0;
    }

    if ( !call_method(ypaworld, 154, &userdata) )
    {
        ypa_log_out("Unable to init shell structure\n");
        return 0;
    }

    if ( !yw_initGameWithSettings() )
    {
        ypa_log_out("Unable to init game with default settings\n");
        return 0;
    }

    if ( !userdata.field_0x0 && !call_method(ypaworld, 156, &userdata))
    {
        ypa_log_out("Error: Unable to open Gameshell\n");
        return 0;
    }

    dword_520400 = 1;
    ReadSnapsDir();

    return 1;
}

int WinMain__sub0()
{
    if ( WinMain__sub0__sub0() )
    {
        if ( WinMain__sub0__sub1() )
            return 1;
        deinit_globl_engines();
    }

    return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nCmdShow)
{
    struct tagMSG Msg;

    ghInstance = hInstance;
    gCmdShow = nCmdShow;
    ghWnd = 0;
    gpcmdline = lpCmdLine;

    HANDLE UAMUTEX = CreateMutex(0, 0, "UA Running Test Mutex");

    if ( UAMUTEX && GetLastError() == ERROR_ALREADY_EXISTS )
    {
        CloseHandle(UAMUTEX);
        return 0;
    }


    CoInitialize(NULL);

    strcpy(cmdline_copy, gpcmdline);

    if ( !WinMain__sub0() )
    {
        CoUninitialize();
        if ( UAMUTEX )
            CloseHandle(UAMUTEX);
        return 0;
    }

    do
    {
        while ( 1 )
        {
            while ( PeekMessage(&Msg, 0, 0, 0, 0) )
            {
                if ( !GetMessage(&Msg, 0, 0, 0) )
                {
////					sub_4113E8();
                    CoUninitialize();
                    return Msg.wParam;
                }
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
            if ( !IsIconic(ghWnd) )
                break;
            WaitMessage();
        }
    }
    while ( sb_0x411324());

    if ( ghWnd )
        DestroyWindow(ghWnd);
////	sub_4113E8();

    CoUninitialize();
    return Msg.wParam;

}
