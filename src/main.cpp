#define GLOBAL
#include "system/fsmgr.h"
#include "system/system.h"
#include "env.h"

#include "includes.h"
#include "system/gfx.h"
#include "system/inpt.h"
#include "winp.h"
#include "wintimer.h"

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
#include "system/inivals.h"
#include "obj3d.h"


int dword_513638 = 0;
int dword_51362C = 0;
int dword_513630 = 0;
std::string buildDate;

int tform_inited = 0;
int audio_inited = 0;
int input_inited = 0;

enum GAME_SCREEN_MODE {
    GAME_SCREEN_MODE_UNKNOWN = 0,
    GAME_SCREEN_MODE_MENU = 1,
    GAME_SCREEN_MODE_GAME = 2,
    GAME_SCREEN_MODE_REPLAY = 3
};

GAME_SCREEN_MODE GameScreenMode;
UserData userdata;

int ProcessGameplayFrame()
{
    ypaworld->Process(&world_update_arg);

    const TLevelInfo &levelInfo = ypaworld->GetLevelInfo();

    switch( levelInfo.State )
    {
    case TLevelInfo::STATE_COMPLETED:
    case TLevelInfo::STATE_ABORTED:
    {
        ypaworld->DeleteLevel();

        if ( dword_513638 || levelInfo.State == TLevelInfo::STATE_ABORTED )
        {
            if ( !ypaworld->LoadSettings("settings.tmp", 
                                         userdata.UserName,
                                         (World::SDF_BUDDY | World::SDF_PROTO | World::SDF_USER),
                                         false))
                return 0;

            dword_513638 = 0;
        }

        int v0 = 1;

        if ( userdata.remoteMode )
            v0 = 0;

        if ( userdata.EnvMode == ENVMODE_NETPLAY )
        {
            userdata.returnToTitle = true;
            userdata.EnvMode = ENVMODE_SINGLEPLAY;
        }
        else
        {
            userdata.returnToTitle = false;
        }

        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;
        world_update_arg.TimeStamp = 0;

        userdata.lastInputEvent = 0;
        userdata.GameIsOver = true;

        if ( ypaworld->OpenGameShell() )
        {
            GameScreenMode = GAME_SCREEN_MODE_MENU;
            Input::Engine.QueryInput(&input_states);

            if (!v0)
                return 0;
        }
        else
        {
            ypa_log_out("GameShell-Error!!!\n");
            ypaworld->DeinitGameShell();
            return 0;
        }
    }
    break;

    case TLevelInfo::STATE_RESTART:
    {
        ypaworld->DeleteLevel();

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.rst", userdata.UserName, levelInfo.LevelID) ) )
            ypa_log_out("Warning, load error\n");

        Input::Engine.QueryInput(&input_states);
    }
    break;

    case TLevelInfo::STATE_SAVE:
    {
        if ( !ypaworld->SaveGame( fmt::sprintf("save:%s/%d.sgm", userdata.UserName, 0) ) )
            ypa_log_out("Warning, Save error\n");

        Input::Engine.QueryInput(&input_states);
    }
    break;

    case TLevelInfo::STATE_LOAD:
    {
        ypaworld->DeleteLevel();

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.sgm", userdata.UserName, 0) ) )
            ypa_log_out("Warning, load error\n");

        Input::Engine.QueryInput(&input_states);
    }
    break;

    default:
        break;
    }

    return 1;
}

std::string sub_4107A0(uint32_t a1)
{
    if ( userdata.snaps.empty() )
        return std::string();

    int v2 = a1 % userdata.snaps.size();

    if ( dword_51362C == v2 )
    {
        if ( v2 )
        {
            v2 = v2 - 1;
        }
        else if ( (userdata.snaps.size() - 1) > 0 )
        {
            v2 = 1;
        }
    }

    dword_51362C = v2;
    return userdata.snaps[v2];
}

int sb_0x411324__sub2__sub0(base_64arg *arg)
{
    if ( (userdata.p_YW->_replayPlayer->field_74 - 3) <= userdata.p_YW->_replayPlayer->frame_id  &&  dword_513630 )
    {
        ypaworld->ypaworld_func164();

        if ( !ypaworld->ypaworld_func162( sub_4107A0(arg->TimeStamp) ) )
        {
            GameScreenMode = GAME_SCREEN_MODE_MENU;

            Input::Engine.QueryInput(&input_states);

            return 0;
        }

        dword_513630 = 1;
        GameScreenMode = GAME_SCREEN_MODE_REPLAY;
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

int ProcessReplayFrame()
{
    if ( !sb_0x411324__sub2__sub0(&world_update_arg) )
    {
        ypaworld->ypaworld_func164();

        if ( dword_513638 )
        {
            if ( !ypaworld->LoadSettings("settings.tmp",
                                         userdata.UserName,
                                         (World::SDF_BUDDY | World::SDF_PROTO | World::SDF_USER),
                                         false) )
                return 0;

            dword_513638 = 0;
        }

        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;
        world_update_arg.TimeStamp = 0;
        userdata.lastInputEvent = 0;

        if ( !ypaworld->OpenGameShell() )
        {
            ypa_log_out("GameShell-Error!!!\n");
            ypaworld->DeinitGameShell();

            return 0;
        }

        userdata.p_YW->_levelInfo.State = TLevelInfo::STATE_MENU;

        GameScreenMode = GAME_SCREEN_MODE_MENU;

        input_states = TInputState();

        Input::Engine.QueryInput(&input_states);
    }
    return 1;
}

int ProcessMenuFrame()
{
    userdata.GlobalTime = world_update_arg.TimeStamp;
    userdata.DTime = world_update_arg.DTime;
    userdata.Input = &input_states;

    ypaworld->ProcessGameShell();
    
    if ( userdata.envAction.action == EnvAction::ACTION_QUIT )
        return 0;
    else if ( userdata.envAction.action == EnvAction::ACTION_PLAY )
    {
        userdata.SaveSettings();
        ypaworld->CloseGameShell();

        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;

        if ( !userdata.SaveBuildProtoState() )
            return 0;

        yw_arg161 v22;
        v22.lvlID = userdata.envAction.params[0];
        v22.field_4 = 0;

        if ( !ypaworld->ypaworld_func183(&v22) )
        {
            ypa_log_out("Sorry, unable to init this level!\n");

            ypaworld->DeinitGameShell();
            return 0;
        }
        GameScreenMode = GAME_SCREEN_MODE_GAME;
        Input::Engine.QueryInput(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_LOAD )
    {
        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;

        const TLevelInfo &a4 = ypaworld->GetLevelInfo();

        userdata.SaveSettings();
        ypaworld->CloseGameShell();

        if ( !userdata.SaveBuildProtoState() )
            return 0;

        if ( !ypaworld->LoadGame( fmt::sprintf("save:%s/%d.sgm", userdata.UserName, 0) ) )
        {
            ypa_log_out("Error while loading level (level %d, User %s\n", a4.LevelID, userdata.UserName.c_str());

            ypaworld->DeinitGameShell();
            return 0;
        }
        GameScreenMode = GAME_SCREEN_MODE_GAME;
        Input::Engine.QueryInput(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_NETPLAY )
    {
        userdata.SaveSettings();

        if ( !userdata.SaveBuildProtoState() )
            return 0;

        dword_513638 = 1;

        ypaworld->CloseGameShell();

        yw_arg161 v22;
        v22.lvlID = userdata.envAction.params[0];
        v22.field_4 = 0;

        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;

        if ( !ypaworld->ypaworld_func179(&v22) )
        {
            ypa_log_out("Sorry, unable to init this level for network!\n");
            ypaworld->DeinitGameShell();

            return 0;
        }

        GameScreenMode = GAME_SCREEN_MODE_GAME;
        Input::Engine.QueryInput(&input_states);
    }
    else if ( userdata.envAction.action == EnvAction::ACTION_REPLAY )
    {
        std::string repname = sub_4107A0(world_update_arg.TimeStamp);
        if ( repname.empty() )
        {
            userdata.lastInputEvent = world_update_arg.TimeStamp;
            return 1;
        }

        if ( !userdata.SaveBuildProtoState() )
            return 0;

        dword_513638 = 1;

        ypaworld->CloseGameShell();

        GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;

        if ( ypaworld->ypaworld_func162(repname) )
        {
            dword_513630 = 1;
            GameScreenMode = GAME_SCREEN_MODE_REPLAY;
        }
        else
        {
            ypa_log_out("Sorry, unable to init player!\n");
            world_update_arg.TimeStamp = 0;
            userdata.lastInputEvent = 0;

            if ( !ypaworld->OpenGameShell() )
            {
                ypa_log_out("GameShell-Error!!!\n");
                ypaworld->DeinitGameShell();
                return 0;
            }

            GameScreenMode = GAME_SCREEN_MODE_MENU;
        }

        Input::Engine.QueryInput(&input_states);
    }

    return 1;
}


int ProcessNextFrame()
{
    input_states = TInputState();
    Input::Engine.QueryInput(&input_states);

    if ( userdata.ResetInputPeriod )
    {
        input_states.Period = 1;
        userdata.ResetInputPeriod = false;
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

    if ( GameScreenMode == GAME_SCREEN_MODE_MENU )
    {
        return ProcessMenuFrame();
    }
    else if ( GameScreenMode == GAME_SCREEN_MODE_GAME )
    {
        return ProcessGameplayFrame();
    }
    else if ( GameScreenMode == GAME_SCREEN_MODE_REPLAY )
    {
        return ProcessReplayFrame();
    }
    return 1;
}

int init_classesLists_and_variables()
{
    ypa_log__ypa_general_log();
    init_d3dlog();
    init_dinputlog();

    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_nucleus>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_rsrc>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_bitmap>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_skeleton>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ilbm>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_sklt>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ade>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_area>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_base>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_bmpanim>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_amesh>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_particle>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_embed>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_idev>() );
    //Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_input>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_itimer>() );
    //Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_iwimp>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_sample>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_wav>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_button>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_network>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_winp>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_wintimer>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_windp>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypaworld>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypabact>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypatank>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_yparobo>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypamissile>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypaflyer>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypacar>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypaufo>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_ypagun>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_3ds>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_image>() );
    Nucleus::ClassList.push_back( Nucleus::MakeClassDescr<NC_STACK_Obj3D>() );

    return 1;
}

void deinit_globl_engines()
{
    if ( tform_inited )
        TF::Engine.Deinit();
    if ( input_inited )
        Input::Engine.Deinit();
    if ( audio_inited )
        SFXEngine::SFXe.deinit();

    ypa_log_out("Nucleus shutdown:\n");
}

int WinMain__sub0__sub0()
{
    ypaworld = 0;
    GameScreenMode = GAME_SCREEN_MODE_UNKNOWN;
    userdata.clear();
    input_states = TInputState();
    world_update_arg = base_64arg();

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
    input_inited = Input::Engine.Init();
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
    FSMgr::FileHandle *user_def = uaOpenFileAlloc("env:user.def", "r");

    std::string settingsFileName;

    if ( user_def )
    {
        std::string line;
        user_def->ReadLine(&line);

        settingsFileName = fmt::sprintf("save:%s/user.txt", line);

        FSMgr::FileHandle *user_txt = uaOpenFileAlloc(settingsFileName, "r");

        if ( user_txt )
        {
            delete user_txt;

            userdata.UserName = line;
            settingsFileName = fmt::sprintf("%s/user.txt", line);
        }
        else
        {
            ypa_log_out("Warning: default user file doesn't exist (%s)\n", settingsFileName.c_str());
            settingsFileName = fmt::sprintf("sdu7/user.txt");
            userdata.UserName = "SDU7";
        }

        delete user_def;
    }
    else
    {
        settingsFileName = fmt::sprintf("sdu7/user.txt");
        userdata.UserName = "SDU7";
        ypa_log_out("Warning: No default user set\n");
    }

    userdata.diskListActiveElement = -1;

    int v8 = 1;
    for ( ProfileList::iterator it = userdata.profiles.begin(); it != userdata.profiles.end(); it++ )
    {
        if ( !StriCmp(it->name, userdata.UserName) )
        {
            userdata.diskListActiveElement = v8;
            break;
        }

        v8++;
    }

    return ypaworld->LoadSettings(settingsFileName,
                                  userdata.UserName,
                                  World::SDF_ALL,
                                  true, true) != 0;
}

void ReadSnapsDir()
{
    FSMgr::DirIter dir = uaOpenDir("env:snaps/");

    if ( dir )
    {
        FSMgr::iNode *entr;
        while ( dir.getNext(&entr) )
        {
            if ( entr->getType() == FSMgr::iNode::NTYPE_FILE && !StriCmp(entr->getName().substr(0, 4), "demo") )
                userdata.snaps.push_back( fmt::sprintf("env:snaps/%s", entr->getName()) );
        }
    }
}

void sub_4113E8()
{
    if ( ypaworld )
    {
        if ( GameScreenMode == GAME_SCREEN_MODE_GAME )
        {
            ypaworld->DeleteLevel();
            ypaworld->DeinitGameShell();
        }
        else if ( GameScreenMode == GAME_SCREEN_MODE_MENU )
        {
            userdata.SaveSettings();
            ypaworld->CloseGameShell();
            ypaworld->DeinitGameShell();
        }

        ypaworld->Delete();
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

    if ( !ypaworld->InitGameShell(&userdata) )
    {
        ypa_log_out("Unable to init shell structure\n");
        return 0;
    }

    if ( !yw_initGameWithSettings() )
    {
        ypa_log_out("Unable to init game with default settings\n");
        return 0;
    }

    if ( !userdata.HasInited && !ypaworld->OpenGameShell())
    {
        ypa_log_out("Error: Unable to open Gameshell\n");
        return 0;
    }

    GameScreenMode = GAME_SCREEN_MODE_MENU;
    ReadSnapsDir();

    return 1;
}

void HandleMods()
{
    std::string modname;
    
    std::vector<std::string> &cmdl = System::GetCmdLineArray();
    int i = System::FindCmdLineArg("-mod");
    if (i >= 0 && i + 1 < cmdl.size())
        modname = cmdl[i + 1];
    
    if (modname.empty())
        return;
    
    FSMgr::iNode *node = FSMgr::iDir::findNode("mods");
    
    if (!node || node->getType() != FSMgr::iNode::NTYPE_DIR)
        return;
    
    node->Detach();
    
    FSMgr::iNode *imod = ((FSMgr::iDir *)node)->getNode(modname);
    
    if (!imod || imod->getType() != FSMgr::iNode::NTYPE_DIR)
    {
        delete node;
        return;
    }
    
    imod->Detach();
    delete node;
    
    FSMgr::iDir *mod = dynamic_cast<FSMgr::iDir *>(imod);
    if (!mod)
    {
        delete imod;
        return;
    }

    // Detach original basepath "save" and "env" dirs
    FSMgr::iDir::GetRoot()->Detach("save");
    FSMgr::iDir::GetRoot()->Detach("env");
    
    // Make sure save and env created in mod dir and complete replace it
    FSMgr::iDir::GetRoot()->addNode( mod->MakeDir("save") );
    FSMgr::iDir::GetRoot()->addNode( mod->MakeDir("env") );
    
    FSMgr::iDir::GetRoot()->Override( mod );
    
    delete mod;
    
    World::AllowMods(true);
}

int WinMain__sub0()
{
    if ( WinMain__sub0__sub0() )
    {
        std::vector<std::string> &cmdl = System::GetCmdLineArray();
        int i = System::FindCmdLineArg("-env");
        if (i >= 0 && i + 1 < cmdl.size())
            Common::Env.SetPrefix("env", cmdl[i + 1]);

        if ( WinMain__sub0__sub1() )
            return 1;
        deinit_globl_engines();
    }

    return 0;
}

uint32_t maxTicks = 1000/60; // init on 60FPS

int main(int argc, char *argv[])
{
    for(int i = 0; i < argc; ++i)
        System::AddCmdLine( std::string(argv[i]) );
    
    System::IniConf::Init();
    FSMgr::iDir::setBaseDir("");
    
    HandleMods();
    
    System::IniConf::ReadFromNucleusIni();
    bool gfxVbo = System::IniConf::GfxVBO.Get<bool>();
    
    if (System::IniConf::DevMode.Get<bool>() || System::FindCmdLineArg("-devmode") >= 0)
        World::AllowMods(true);        
    
    System::Init(!gfxVbo);
    
    GFX::Engine.Init();
    System::Movie.Init();
    
    Gui::UA::Init();

    if ( !WinMain__sub0() )
        return 0;

    System::IniConf::ReadFromNucleusIni();

    ypaworld->SetFixWeaponRadius( System::IniConf::TuneWeaponRadius.Get<bool>() );

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
            if ( !ProcessNextFrame() )
                break;

            if ( System::ProcessEvents() )
                break;
        }
        else
        {
            uint32_t curTick = SDL_GetTicks();

            if (curTick >= ticks)
            {
                if ( !ProcessNextFrame() )
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
