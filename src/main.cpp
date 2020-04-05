#define GLOBAL
#include "fsmgr.h"
#include "wrapSDL.h"
#include "includes.h"
#include "engine_gfx.h"
#include "bitmap.h"
#include "display.h"
#include "win3d.h"
#include "ilbm.h"

#include "font.h"
#include "yw.h"

#include "button.h"

#include "engine_gfx.h"
#include "engine_input.h"

#include "gui/widget.h"
#include "gui/uawidgets.h"


int dword_513638 = 0;
int dword_51362C = 0;
int dword_513630 = 0;
char buildDate[256];

int gfx_inited = 0;
int tform_inited = 0;
int audio_inited = 0;
int input_inited = 0;

key_value_stru tuneKeys[1] =
{
    {"tune.weapon_radius", KEY_TYPE_BOOL, 0}               //0
};

static bool fixWeaponRadius = false;


int sub_4107FC(UserData *usr)
{
    yw_arg172 arg171;

    arg171.usr = usr;
    arg171.usertxt = "settings.tmp";
    arg171.field_4 = usr->user_name.c_str();
    arg171.field_8 = 193;
    arg171.field_10 = 0;

    return ypaworld->ypaworld_func171(&arg171);
}

void sub_410628()
{
    char buf[300];
    sprintf(buf, "%s/user.txt", userdata.user_name.c_str());

    yw_arg172 arg171;
    arg171.usertxt = buf;
    arg171.field_4 = userdata.user_name.c_str();
    arg171.usr = &userdata;
    arg171.field_8 = 255;
    arg171.field_10 = 0;

    ypaworld->ypaworld_func171(&arg171);

    FSMgr::FileHandle *fil = uaOpenFile("env:user.def", "w");
    if ( fil )
    {
        strcpy(buf, userdata.user_name.c_str());
        fil->write(buf, strlen(buf));
        delete fil;
    }
}

int sb_0x411324__sub0()
{
    ypaworld->base_func64(&world_update_arg);

    LevelInfo *var_2d90 = ypaworld->getYW_levelInfo();

    char buf[200];

    switch( var_2d90->State )
    {
    case 1:
    case 2:
    {
        ypaworld->ypaworld_func151();

        if ( dword_513638 || var_2d90->State == 2 )
        {
            yw_arg172 arg172;
            arg172.usr = &userdata;
            arg172.usertxt = "settings.tmp";
            arg172.field_10 = 0;
            arg172.field_4 = userdata.user_name.c_str();
            arg172.field_8 = 0xC1;

            if ( !ypaworld->ypaworld_func172(&arg172) )
                return 0;

            dword_513638 = 0;
        }

        int v0 = 1;

        if ( userdata.remoteMode )
            v0 = 0;

        if ( userdata.envMode == ENVMODE_NETPLAY )
        {
            userdata.returnToTitle = true;
            userdata.envMode = ENVMODE_SINGLEPLAY;
        }
        else
        {
            userdata.returnToTitle = false;
        }

        dword_520400 = 0;
        world_update_arg.TimeStamp = 0;

        userdata.lastInputEvent = 0;
        userdata.field_0xc = 1;

        if ( ypaworld->ypaworld_func156(&userdata) )
        {
            dword_520400 = 1;
            INPe.sub_412D28(&input_states);

            if (!v0)
                return 0;
        }
        else
        {
            ypa_log_out("GameShell-Error!!!\n");
            ypaworld->ypaworld_func155(&userdata);
            return 0;
        }
    }
    break;

    case 4:
    {
        ypaworld->ypaworld_func151();

        sprintf(buf, "save:%s/%d.rst", userdata.user_name.c_str(), var_2d90->LevelID);

        yw_arg169 arg169;
        arg169.saveFile = buf;
        arg169.usr = &userdata;

        if ( !ypaworld->ypaworld_func169(&arg169) )
            ypa_log_out("Warning, load error\n");

        INPe.sub_412D28(&input_states);
    }
    break;

    case 6:
    {
        yw_arg169 arg169;
        arg169.usr = &userdata;
        arg169.saveFile = buf;

        sprintf(buf, "save:%s/%d.sgm", userdata.user_name.c_str(), 0);

        if ( !ypaworld->ypaworld_func170(&arg169) )
            ypa_log_out("Warning, Save error\n");

        INPe.sub_412D28(&input_states);
    }
    break;

    case 7:
    {
        ypaworld->ypaworld_func151();

        yw_arg169 arg169;
        arg169.usr = &userdata;
        arg169.saveFile = buf;
        sprintf(buf, "save:%s/%d.sgm", userdata.user_name.c_str(), 0);

        if ( !ypaworld->ypaworld_func169(&arg169) )
            ypa_log_out("Warning, load error\n");

        INPe.sub_412D28(&input_states);
    }
    break;

    default:
        break;
    }

    return 1;
}

char * sub_4107A0(int a1)
{
    if ( !userdata.snap_count )
        return NULL;

    int v2 = a1 % userdata.snap_count;

    if ( dword_51362C == v2 )
    {
        if ( v2 )
        {
            v2 = v2 - 1;
        }
        else if ( (userdata.snap_count - 1) > 0 )
        {
            v2 = 1;
        }
    }

    dword_51362C = v2;
    return userdata.snaps[v2];
}

int sb_0x411324__sub2__sub0(base_64arg *arg)
{
    if ( (userdata.p_ypaworld->replayer->field_74 - 3) <= userdata.p_ypaworld->replayer->frame_id  &&  dword_513630 )
    {
        char *gin_and_tonic = sub_4107A0(arg->TimeStamp);

        ypaworld->ypaworld_func164();

        if ( !ypaworld->ypaworld_func162(gin_and_tonic) )
        {
            dword_520400 = 1;

            INPe.sub_412D28(&input_states);

            return 0;
        }

        dword_513630 = 1;
        dword_520400 = 3;
    }

    ypaworld->ypaworld_func163(arg);

    yw_arg165 arg165;
    arg165.field_0 = 0;
    arg165.frame = 0;

    int cont_play = 1;

    if ( arg->field_8->downed_key == 'N' )
    {
        arg165.field_0 = 4;
    }
    else if ( arg->field_8->downed_key == 'P' )
    {
        arg165.field_0 = 2;
    }
    else if ( arg->field_8->downed_key == 'R' )
    {
        arg165.field_0 = 3;
    }
    else if ( arg->field_8->downed_key == 'S' )
    {
        arg165.field_0 = 1;
    }
    else if ( arg->field_8->downed_key == 'V' )
    {
        arg165.frame = -10;
        arg165.field_0 = 7;
    }
    else if ( arg->field_8->downed_key == 'B' )
    {
        arg165.field_0 = 5;
    }
    else if ( arg->field_8->downed_key == 'M' )
    {
        arg165.frame = 10;
        arg165.field_0 = 7;
    }
    else if ( arg->field_8->downed_key == UAVK_SPACE || arg->field_8->downed_key == UAVK_ESCAPE )
    {
        cont_play = 0;
    }

    if ( arg165.field_0 )
        ypaworld->ypaworld_func165(&arg165);

    return cont_play;
}

int sb_0x411324__sub2()
{
    if ( !sb_0x411324__sub2__sub0(&world_update_arg) )
    {
        ypaworld->ypaworld_func164();

        if ( dword_513638 )
        {
            yw_arg172 arg172;
            arg172.usr = &userdata;
            arg172.usertxt = "settings.tmp";
            arg172.field_10 = 0;
            arg172.field_4 = userdata.user_name.c_str();
            arg172.field_8 = 193;

            if ( !ypaworld->ypaworld_func172(&arg172) )
                return 0;

            dword_513638 = 0;
        }

        dword_520400 = 0;
        world_update_arg.TimeStamp = 0;
        userdata.lastInputEvent = 0;

        if ( !ypaworld->ypaworld_func156(&userdata) )
        {
            ypa_log_out("GameShell-Error!!!\n");
            ypaworld->ypaworld_func155(&userdata);

            return 0;
        }

        userdata.p_ypaworld->_levelInfo->State = 8;

        dword_520400 = 1;

        memset(&input_states, 0, sizeof(struC5));

        INPe.sub_412D28(&input_states);
    }
    return 1;
}

int sb_0x411324__sub1()
{
    userdata.glblTime = world_update_arg.TimeStamp;
    userdata.frameTime = world_update_arg.DTime;
    userdata._input = &input_states;

    ypaworld->ypaworld_func158(&userdata);

    if ( userdata.envAction.action == EnvAction::ACTION_QUIT )
        return 0;
    else if ( userdata.envAction.action == EnvAction::ACTION_PLAY )
    {
        sub_410628();
        ypaworld->ypaworld_func157(&userdata);

        dword_520400 = 0;

        if ( !sub_4107FC(&userdata) )
            return 0;

        yw_arg161 v22;
        v22.lvlID = userdata.envAction.params[0];
        v22.field_4 = 0;

        if ( !ypaworld->ypaworld_func183(&v22) )
        {
            ypa_log_out("Sorry, unable to init this level!\n");

            ypaworld->ypaworld_func155(&userdata);
            return 0;
        }
        dword_520400 = 2;
        INPe.sub_412D28(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_LOAD )
    {
        dword_520400 = 0;

        LevelInfo *a4 = ypaworld->getYW_levelInfo();

        sub_410628();
        ypaworld->ypaworld_func157(&userdata);

        if ( !sub_4107FC(&userdata) )
            return 0;

        char a1[200];
        yw_arg169 arg169;
        arg169.saveFile = a1;
        arg169.usr = &userdata;

        sprintf(a1, "save:%s/%d.sgm", userdata.user_name.c_str(), 0);

        if ( !ypaworld->ypaworld_func169(&arg169) )
        {
            ypa_log_out("Error while loading level (level %d, User %s\n", a4->LevelID, userdata.user_name.c_str());

            ypaworld->ypaworld_func155(&userdata);
            return 0;
        }
        dword_520400 = 2;
        INPe.sub_412D28(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_NETPLAY )
    {
        sub_410628();

        if ( !sub_4107FC(&userdata) )
            return 0;

        dword_513638 = 1;

        ypaworld->ypaworld_func157(&userdata);

        yw_arg161 v22;
        v22.lvlID = userdata.envAction.params[0];
        v22.field_4 = 0;

        dword_520400 = 0;

        if ( !ypaworld->ypaworld_func179(&v22) )
        {
            ypa_log_out("Sorry, unable to init this level for network!\n");
            ypaworld->ypaworld_func155(&userdata);

            return 0;
        }

        dword_520400 = 2;
        INPe.sub_412D28(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_DEMO )
    {
        char *repname = sub_4107A0(world_update_arg.TimeStamp);
        if ( !repname )
        {
            userdata.lastInputEvent = world_update_arg.TimeStamp;
            return 1;
        }

        if ( !sub_4107FC(&userdata) )
            return 0;

        dword_513638 = 1;

        ypaworld->ypaworld_func157(&userdata);

        dword_520400 = 0;

        if ( ypaworld->ypaworld_func162(repname) )
        {
            dword_513630 = 1;
            dword_520400 = 3;
        }
        else
        {
            ypa_log_out("Sorry, unable to init player!\n");
            world_update_arg.TimeStamp = 0;
            userdata.lastInputEvent = 0;

            if ( !ypaworld->ypaworld_func156(&userdata) )
            {
                ypa_log_out("GameShell-Error!!!\n");
                ypaworld->ypaworld_func155(&userdata);
                return 0;
            }

            dword_520400 = 1;
        }

        INPe.sub_412D28(&input_states);
    }

    return 1;
}


int sb_0x411324()
{
    memset(&input_states, 0, sizeof(struC5));
    INPe.sub_412D28(&input_states);

    if ( userdata.field_0x10 )
    {
        input_states.period = 1;
        userdata.field_0x10 = 0;
    }

    input_states.period++;
    
    world_update_arg.DTime = input_states.period;
    world_update_arg.field_8 = &input_states;

    world_update_arg.TimeStamp += input_states.period;
    

    // If mouse captured, enable releative mouse control
    if (ypaworld->field_17c0)
        SDLWRAP_releativeMouse(true);
    else
        SDLWRAP_releativeMouse(false);
    
    Gui::Root::Instance.TimersUpdate( input_states.period );

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

int init_classesLists_and_variables()
{
    ypa_log__ypa_general_log();
    init_d3dlog();
    init_dinputlog();

    if ( !set_classes_list() )
        return 0;

    return 1;
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
    if ( tform_inited )
        TFEngine::Engine.deinit();
    if ( input_inited )
        INPe.deinit();
    if ( audio_inited )
        SFXEngine::SFXe.deinit();
    if ( gfx_inited )
        GFXEngine::GFXe.deinit();

    sb_0x411c08();
}

int WinMain__sub0__sub0()
{
    ypaworld = 0;
    dword_520400 = 0;
    userdata.clear();
    memset(&input_states, 0, sizeof(struC5));
    memset(&world_update_arg, 0, sizeof(base_64arg));

    if ( !init_classesLists_and_variables() )
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

    gfx_inited = GFXEngine::GFXe.init();
    audio_inited = SFXEngine::SFXe.init();
    input_inited = INPe.init();
    tform_inited = TFEngine::Engine.init();

    if ( !audio_inited )
    {
        sub_412038("Couldn't open audio engine!");
        deinit_globl_engines();
        return 0;
    }

    if ( !gfx_inited )
    {
        sub_412038("Couldn't open gfx engine!");
        deinit_globl_engines();
        return 0;
    }
    if ( !tform_inited )
    {
        sub_412038("Couldn't open tform engine!");
        deinit_globl_engines();
        return 0;
    }
    if ( !input_inited )
    {
        sub_412038("Couldn't open input engine!");
        deinit_globl_engines();
        return 0;
    }

    return 1;
}

int yw_initGameWithSettings()
{
    FSMgr::FileHandle *user_def = uaOpenFile("env:user.def", "r");

    std::string a1;

    if ( user_def )
    {
        char v11[300];

        user_def->gets(v11, 300);

        a1 = fmt::sprintf("save:%s/user.txt", v11);

        FSMgr::FileHandle *user_txt = uaOpenFile(a1, "r");

        if ( user_txt )
        {
            delete user_txt;

            userdata.user_name = v11;
            a1 = fmt::sprintf("%s/user.txt", v11);
        }
        else
        {
            ypa_log_out("Warning: default user file doesn't exist (%s)\n", a1);
            a1 = fmt::sprintf("sdu7/user.txt");
            userdata.user_name = "SDU7";
        }

        delete user_def;
    }
    else
    {
        a1 = fmt::sprintf("sdu7/user.txt");
        userdata.user_name = "SDU7";
        ypa_log_out("Warning: No default user set\n");
    }

    userdata.field_1612 = -1;

    int v8 = 1;
    for ( ProfileList::iterator it = userdata.profiles.begin(); it != userdata.profiles.end(); it++ )
    {
        if ( !StriCmp(it->name, userdata.user_name) )
        {
            userdata.field_1612 = v8;
            break;
        }

        v8++;
    }

    yw_arg172 v13;

    v13.usertxt = a1.c_str();
    v13.usr = &userdata;
    v13.field_4 = userdata.user_name.c_str();
    v13.field_8 = 255;
    v13.field_10 = 1;

    return ypaworld->ypaworld_func172(&v13) != 0;
}

void ReadSnapsDir()
{
    FSMgr::DirIter dir = uaOpenDir("env:snaps/");

    if ( dir )
    {
        FSMgr::iNode *entr;
        while ( dir.getNext(&entr) )
        {
            if ( entr->getType() == FSMgr::iNode::NTYPE_FILE && userdata.snap_count < 32 && !strnicmp(entr->getName().c_str(), "demo", 4) )
            {
                sprintf( userdata.snaps[ userdata.snap_count ], "env:snaps/%s", entr->getName().c_str());
                userdata.snap_count++;
            }
        }
    }
}

void sub_4113E8()
{
    if ( ypaworld )
    {
        if ( dword_520400 == 2 )
        {
            ypaworld->ypaworld_func151();
            ypaworld->ypaworld_func155(&userdata);
        }
        else if ( dword_520400 == 1 )
        {
            sub_410628();
            ypaworld->ypaworld_func157(&userdata);
            ypaworld->ypaworld_func155(&userdata);
        }

        delete_class_obj(ypaworld);
    }

    deinit_globl_engines();
}

int WinMain__sub0__sub1()
{
    strcpy(buildDate, "Jul 09 1998  23:52:47");
//    strcpy(buildDate, __DATE__);
//    strcat(buildDate, " ");
//    strcat(buildDate, __TIME__);

    IDVList init_vals;
    init_vals.Add(NC_STACK_ypaworld::YW_ATT_BUILD_DATE, buildDate);

    ypaworld = Nucleus::CInit<NC_STACK_ypaworld>(init_vals);
    
    Gui::UA::yw = ypaworld;
    
    if ( !ypaworld )
    {
        ypa_log_out("Unable to init ypaworld.class\n");
        return 0;
    }

    if ( !ypaworld->ypaworld_func154(&userdata) )
    {
        ypa_log_out("Unable to init shell structure\n");
        return 0;
    }

    if ( !yw_initGameWithSettings() )
    {
        ypa_log_out("Unable to init game with default settings\n");
        return 0;
    }

    if ( !userdata.field_0x0 && !ypaworld->ypaworld_func156(&userdata))
    {
        ypa_log_out("Error: Unable to open Gameshell\n");
        return 0;
    }

    dword_520400 = 1;
    ReadSnapsDir();

    return 1;
}

int WinMain__sub0(int argc, char *argv[])
{
    if ( WinMain__sub0__sub0() )
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcasecmp(argv[i], "-env") == 0 && i + 1 < argc)
            {
                set_prefix_replacement("env", argv[i + 1]);
            }
        }


        if ( WinMain__sub0__sub1() )
            return 1;
        deinit_globl_engines();
    }

    return 0;
}

uint32_t maxTicks = 1000/60; // init on 60FPS

void fpsLimitter(int value)
{
    if (value > 1000)
        maxTicks = 0;
    else if (value <= 0)
        maxTicks = 0;
    else
        maxTicks = 1000/value;
}

int main(int argc, char *argv[])
{
//	HANDLE UAMUTEX = CreateMutex(0, 0, "UA Running Test Mutex");
//
//	if ( UAMUTEX && GetLastError() == ERROR_ALREADY_EXISTS )
//	{
//		CloseHandle(UAMUTEX);
//		return 0;
//	}

    Gui::UA::Init();
    FSMgr::iDir::setBaseDir("");
    SDLWRAP_INIT();

    if ( !WinMain__sub0(argc, argv) )
        return 0;

    get_keyvalue_from_ini(NULL, tuneKeys, 1);

    fixWeaponRadius = tuneKeys[0].value.val;

    uint32_t ticks = 0;
    
    Gui::Root::Instance.SetHwCompose(true);
    ypaworld->LoadGuiFonts();
    
    // New gui test windows
    /*Gui::UAWindow *smpl = new Gui::UAWindow("Test1", Common::PointRect(100, 100, 200, 300), 
        Gui::UAWindow::FLAG_WND_RESIZEABLE | 
        Gui::UAWindow::FLAG_WND_VSCROLL | 
        Gui::UAWindow::FLAG_WND_CLOSE );
    smpl->SetEnable(true);
    smpl->SetAlpha(190);

    Gui::Root::Instance.AddWidget(smpl);
    
    Gui::UAWindow *smpl2 = new Gui::UAWindow("Test2", Common::PointRect(115, 120, 50, 60), 
        Gui::UAWindow::FLAG_WND_RESIZEABLE | 
        Gui::UAWindow::FLAG_WND_VSCROLL | 
        Gui::UAWindow::FLAG_WND_CLOSE |
        Gui::UAWindow::FLAG_WND_HELP | 
        Gui::UAWindow::FLAG_WND_MAXM |
        Gui::UAWindow::FLAG_WND_HSCROLL);
    smpl2->SetEnable(true);
    smpl2->SetAlpha(190);
    
    Gui::Root::Instance.AddWidget(smpl2);*/
    

//    int fps = 0;
//    uint32_t fpstick = SDL_GetTicks() + 1000;

    while ( true )
    {

        if (maxTicks == 0)
        {
            if ( !sb_0x411324() )
                break;

            if ( SDLWRAP_UPDATE() )
                break;
        }
        else
        {
            uint32_t curTick = SDL_GetTicks();

            if (curTick >= ticks)
            {
                if ( !sb_0x411324() )
                    break;

                if ( SDLWRAP_UPDATE() )
                    break;

                ticks = curTick;

                uint32_t diffTick = SDL_GetTicks() - curTick;

                if (diffTick < maxTicks)
                {
                    uint16_t delay = maxTicks - diffTick;
                    ticks += delay;
                    SDL_Delay(delay);
                }
            }
            else
                SDL_Delay(1);
        }

//        fps++;
//        if (SDL_GetTicks() > fpstick)
//        {
//            printf("fps %d\n", fps);
//            fpstick = SDL_GetTicks() + 1000;
//            fps = 0;
//        }

    }
    
    Gui::UA::Deinit();
    sub_4113E8();

    SDLWRAP_DEINIT();

    return 0;
}


bool tuneGetWeaponRadiusFix()
{
    return fixWeaponRadius;
}
