#define GLOBAL
#include "system/fsmgr.h"
#include "system/system.h"
#include "env.h"

#include "includes.h"
#include "system/gfx.h"
#include "system/inpt.h"
#include "ade.h"
#include "area.h"
#include "amesh.h"
#include "bitmap.h"
#include "bmpAnm.h"
#include "base.h"
#include "ilbm.h"
#include "particle.h"
#include "embed.h"
#include "network.h"
#include "windp.h"
#include "ypabact.h"
#include "ypatank.h"
#include "ypacar.h"
#include "ypaflyer.h"
#include "yparobo.h"
#include "ypaufo.h"
#include "3ds.h"
#include "image.h"


#include "font.h"
#include "yw.h"

#include "button.h"



#include "gui/widget.h"
#include "gui/uawidgets.h"
#include "gui/uamsgbox.h"
#include "gui/uaempty.h"
#include "system/movie.h"


int dword_513638 = 0;
int dword_51362C = 0;
int dword_513630 = 0;
std::string buildDate;

int tform_inited = 0;
int audio_inited = 0;
int input_inited = 0;

Common::Ini::KeyList tuneKeys
{
    Common::Ini::Key("tune.weapon_radius", Common::Ini::KT_BOOL)               //0
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
    std::string buf = fmt::sprintf("%s/user.txt", userdata.user_name);
    yw_arg172 arg171;
    arg171.usertxt = buf.c_str();
    arg171.field_4 = userdata.user_name.c_str();
    arg171.usr = &userdata;
    arg171.field_8 = 255;
    arg171.field_10 = 0;

    ypaworld->ypaworld_func171(&arg171);

    FSMgr::FileHandle *fil = uaOpenFile("env:user.def", "w");
    if ( fil )
    {
        fil->write(userdata.user_name.c_str(), userdata.user_name.size());
        delete fil;
    }
}

int sb_0x411324__sub0()
{
    ypaworld->base_func64(&world_update_arg);

    LevelInfo *var_2d90 = ypaworld->getYW_levelInfo();

    switch( var_2d90->State )
    {
    case 1:
    case 2:
    {
        ypaworld->DeleteLevel();

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
            INPe.QueryInput(&input_states);

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
        ypaworld->DeleteLevel();

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.rst", userdata.user_name, var_2d90->LevelID) ) )
            ypa_log_out("Warning, load error\n");

        INPe.QueryInput(&input_states);
    }
    break;

    case 6:
    {
        if ( !ypaworld->SaveGame( fmt::sprintf("save:%s/%d.sgm", userdata.user_name, 0) ) )
            ypa_log_out("Warning, Save error\n");

        INPe.QueryInput(&input_states);
    }
    break;

    case 7:
    {
        ypaworld->DeleteLevel();

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.sgm", userdata.user_name, 0) ) )
            ypa_log_out("Warning, load error\n");

        INPe.QueryInput(&input_states);
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

            INPe.QueryInput(&input_states);

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

    if ( arg->field_8->KbdLastHit == Input::KC_N )
    {
        arg165.field_0 = 4;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_P )
    {
        arg165.field_0 = 2;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_R )
    {
        arg165.field_0 = 3;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_S )
    {
        arg165.field_0 = 1;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_V )
    {
        arg165.frame = -10;
        arg165.field_0 = 7;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_B )
    {
        arg165.field_0 = 5;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_M )
    {
        arg165.frame = 10;
        arg165.field_0 = 7;
    }
    else if ( arg->field_8->KbdLastHit == Input::KC_SPACE || arg->field_8->KbdLastHit == Input::KC_ESCAPE )
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

        input_states.Clear();

        INPe.QueryInput(&input_states);
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
        INPe.QueryInput(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_LOAD )
    {
        dword_520400 = 0;

        LevelInfo *a4 = ypaworld->getYW_levelInfo();

        sub_410628();
        ypaworld->ypaworld_func157(&userdata);

        if ( !sub_4107FC(&userdata) )
            return 0;

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.sgm", userdata.user_name, 0) ) )
        {
            ypa_log_out("Error while loading level (level %d, User %s\n", a4->LevelID, userdata.user_name.c_str());

            ypaworld->ypaworld_func155(&userdata);
            return 0;
        }
        dword_520400 = 2;
        INPe.QueryInput(&input_states);
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
        INPe.QueryInput(&input_states);
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

        INPe.QueryInput(&input_states);
    }

    return 1;
}


int sb_0x411324()
{
    input_states.Clear();
    INPe.QueryInput(&input_states);

    if ( userdata.field_0x10 )
    {
        input_states.Period = 1;
        userdata.field_0x10 = 0;
    }

    input_states.Period++;
    
    world_update_arg.DTime = input_states.Period;
    world_update_arg.field_8 = &input_states;

    world_update_arg.TimeStamp += input_states.Period;
    

    // If mouse captured, enable releative mouse control
    if (ypaworld->_mouseGrabbed)
        System::SetReleativeMouse(true);
    else
        System::SetReleativeMouse(false);
    
    Gui::Root::Instance.TimersUpdate( input_states.Period );

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

    Nucleus::ClassList.push_back(NC_STACK_nucleus::description);
    Nucleus::ClassList.push_back(NC_STACK_rsrc::description);
    Nucleus::ClassList.push_back(NC_STACK_bitmap::description);
    Nucleus::ClassList.push_back(NC_STACK_skeleton::description);
    Nucleus::ClassList.push_back(NC_STACK_ilbm::description);
    Nucleus::ClassList.push_back(NC_STACK_sklt::description);
    Nucleus::ClassList.push_back(NC_STACK_ade::description);
    Nucleus::ClassList.push_back(NC_STACK_area::description);
    Nucleus::ClassList.push_back(NC_STACK_base::description);
    Nucleus::ClassList.push_back(NC_STACK_bmpanim::description);
    Nucleus::ClassList.push_back(NC_STACK_amesh::description);
    Nucleus::ClassList.push_back(NC_STACK_particle::description);
    Nucleus::ClassList.push_back(NC_STACK_embed::description);
    Nucleus::ClassList.push_back(NC_STACK_idev::description);
    Nucleus::ClassList.push_back(NC_STACK_input::description);
    Nucleus::ClassList.push_back(NC_STACK_itimer::description);
    Nucleus::ClassList.push_back(NC_STACK_iwimp::description);
    Nucleus::ClassList.push_back(NC_STACK_sample::description);
    Nucleus::ClassList.push_back(NC_STACK_wav::description);
    Nucleus::ClassList.push_back(NC_STACK_button::description);
    Nucleus::ClassList.push_back(NC_STACK_network::description);
    Nucleus::ClassList.push_back(NC_STACK_winp::description);
    Nucleus::ClassList.push_back(NC_STACK_wintimer::description);
    Nucleus::ClassList.push_back(NC_STACK_windp::description);
    Nucleus::ClassList.push_back(NC_STACK_ypaworld::description);
    Nucleus::ClassList.push_back(NC_STACK_ypabact::description);
    Nucleus::ClassList.push_back(NC_STACK_ypatank::description);
    Nucleus::ClassList.push_back(NC_STACK_yparobo::description);
    Nucleus::ClassList.push_back(NC_STACK_ypamissile::description);
    Nucleus::ClassList.push_back(NC_STACK_ypaflyer::description);
    Nucleus::ClassList.push_back(NC_STACK_ypacar::description);
    Nucleus::ClassList.push_back(NC_STACK_ypaufo::description);
    Nucleus::ClassList.push_back(NC_STACK_ypagun::description);
    Nucleus::ClassList.push_back(NC_STACK_3ds::description);
    Nucleus::ClassList.push_back(NC_STACK_image::description);

    return 1;
}

void deinit_globl_engines()
{
    if ( tform_inited )
        TF::Engine.Deinit();
    if ( input_inited )
        INPe.deinit();
    if ( audio_inited )
        SFXEngine::SFXe.deinit();

    ypa_log_out("Nucleus shutdown:\n");
}

int WinMain__sub0__sub0()
{
    ypaworld = 0;
    dword_520400 = 0;
    userdata.clear();
    input_states.Clear();
    memset(&world_update_arg, 0, sizeof(world_update_arg));

    if ( !init_classesLists_and_variables() )
    {
        sub_412038("Couldn't open Nucleus!");
        deinit_globl_engines();
        return 0;
    }

    Common::Env.AddGlobalIniKey("gfx.display  = win3d.class");
    Common::Env.AddGlobalIniKey("gfx.display2 = windd.class");
    Common::Env.AddGlobalIniKey("gfx.engine     = gfx.engine");
    Common::Env.AddGlobalIniKey("tform.engine   = tform_ng.engine");
    Common::Env.AddGlobalIniKey("input.engine   = input.engine");
    Common::Env.AddGlobalIniKey("input.wimp     = winp");
    Common::Env.AddGlobalIniKey("input.keyboard = winp");
    Common::Env.AddGlobalIniKey("input.slider[10] = winp:mousex");
    Common::Env.AddGlobalIniKey("input.slider[11] = winp:mousey");
    Common::Env.AddGlobalIniKey("input.slider[12] = winp:joyx winp:joyrudder");
    Common::Env.AddGlobalIniKey("input.slider[13] = winp:joyy");
    Common::Env.AddGlobalIniKey("input.slider[14] = winp:joythrottle");
    Common::Env.AddGlobalIniKey("input.slider[15] = winp:joyhatx");
    Common::Env.AddGlobalIniKey("input.slider[16] = winp:joyhaty");
    Common::Env.AddGlobalIniKey("input.button[16] = winp:joyb0");
    Common::Env.AddGlobalIniKey("input.button[17] = winp:joyb1");
    Common::Env.AddGlobalIniKey("input.button[18] = winp:joyb2");
    Common::Env.AddGlobalIniKey("input.button[19] = winp:joyb3");
    Common::Env.AddGlobalIniKey("input.button[20] = winp:joyb4");
    Common::Env.AddGlobalIniKey("input.button[21] = winp:joyb5");
    Common::Env.AddGlobalIniKey("input.button[22] = winp:joyb6");
    Common::Env.AddGlobalIniKey("input.button[23] = winp:joyb7");

    audio_inited = SFXEngine::SFXe.init();
    input_inited = INPe.init();
    tform_inited = TF::Engine.Init();

    if ( !audio_inited )
    {
        sub_412038("Couldn't open audio engine!");
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
        std::string line;
        user_def->ReadLine(&line);

        a1 = fmt::sprintf("save:%s/user.txt", line);

        FSMgr::FileHandle *user_txt = uaOpenFile(a1, "r");

        if ( user_txt )
        {
            delete user_txt;

            userdata.user_name = line;
            a1 = fmt::sprintf("%s/user.txt", line);
        }
        else
        {
            ypa_log_out("Warning: default user file doesn't exist (%s)\n", a1.c_str());
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
            ypaworld->DeleteLevel();
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
    buildDate = "Jul 09 1998  23:52:47";
//    strcpy(buildDate, __DATE__);
//    strcat(buildDate, " ");
//    strcat(buildDate, __TIME__);

    ypaworld = Nucleus::CInit<NC_STACK_ypaworld>( { {NC_STACK_ypaworld::YW_ATT_BUILD_DATE, std::string(buildDate)} } );
    
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
                Common::Env.SetPrefix("env", argv[i + 1]);
            }
        }


        if ( WinMain__sub0__sub1() )
            return 1;
        deinit_globl_engines();
    }

    return 0;
}

uint32_t maxTicks = 1000/60; // init on 60FPS

int main(int argc, char *argv[])
{
//	HANDLE UAMUTEX = CreateMutex(0, 0, "UA Running Test Mutex");
//
//	if ( UAMUTEX && GetLastError() == ERROR_ALREADY_EXISTS )
//	{
//		CloseHandle(UAMUTEX);
//		return 0;
//	}
    
    FSMgr::iDir::setBaseDir("");
    System::Init();
    GFX::Engine.Init();
    System::Movie.Init();
    
    Gui::UA::Init();

    if ( !WinMain__sub0(argc, argv) )
        return 0;

    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &tuneKeys);

    fixWeaponRadius = tuneKeys[0].Get<bool>();

    uint32_t ticks = 0;
    
    Gui::Root::Instance.SetHwCompose(true);
    ypaworld->LoadGuiFonts();
    ypaworld->CreateNewGuiElements();
    
    
    //Gui::Root::Instance.AddPortal( Common::Point(640, 480), Common::Rect(0, 0, 300, 300));
    
    // New gui test windows
    /*Gui::UAWindow *smpl = new Gui::UAWindow("Test1", Common::PointRect(100, 100, 200, 300), 
        Gui::UAWindow::FLAG_WND_RESIZEABLE | 
        Gui::UAWindow::FLAG_WND_VSCROLL | 
        Gui::UAWindow::FLAG_WND_CLOSE );
    smpl->SetEnable(true);
    smpl->SetAlpha(190);

    Gui::Root::Instance.AddWidget(smpl);   
    
    Gui::UAWindow *smpl2 = new Gui::UAWindow("Test2", Common::PointRect(0, 0, 50, 60), 
        Gui::UAWindow::FLAG_WND_RESIZEABLE | 
        Gui::UAWindow::FLAG_WND_VSCROLL | 
        Gui::UAWindow::FLAG_WND_CLOSE |
        Gui::UAWindow::FLAG_WND_HELP | 
        Gui::UAWindow::FLAG_WND_MAXM |
        Gui::UAWindow::FLAG_WND_HSCROLL);
    smpl2->SetEnable(true);*/
    //smpl2->SetAlpha(190);
    
    //scl->MoveTo(100, 100);
    //scl->ResizeWH(300, 360);
    
    //Gui::Root::Instance.AddWidgetPortal(0, smpl2);  
    //Gui::Root::Instance.AddWidget(smpl2);  
    //smpl->AddChild(smpl2);
    
   
    

//    int fps = 0;
//    uint32_t fpstick = SDL_GetTicks() + 1000;

    while ( true )
    {

        if (GFX::Engine.FpsMaxTicks == 0)
        {
            if ( !sb_0x411324() )
                break;

            if ( System::ProcessEvents() )
                break;
        }
        else
        {
            uint32_t curTick = SDL_GetTicks();

            if (curTick >= ticks)
            {
                if ( !sb_0x411324() )
                    break;

                if ( System::ProcessEvents() )
                    break;

                ticks = curTick;

                uint32_t diffTick = SDL_GetTicks() - curTick;

                if (diffTick < GFX::Engine.FpsMaxTicks)
                {
                    uint16_t delay = GFX::Engine.FpsMaxTicks - diffTick;
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
    
    ypaworld->DeleteNewGuiElements();
    
    sub_4113E8();
    Gui::UA::Deinit();
    
    System::Deinit();

    return 0;
}


bool tuneGetWeaponRadiusFix()
{
    return fixWeaponRadius;
}
