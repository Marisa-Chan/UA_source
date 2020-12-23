#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <string>
#include "includes.h"
#include "yw.h"

#include "yw_internal.h"

#include "button.h"
#include "font.h"
#include "yparobo.h"
#include "windp.h"
#include "yw_net.h"
#include "gui/uacommon.h"


const Nucleus::ClassDescr NC_STACK_ypaworld::description("ypaworld.class", &newinstance);

Common::Ini::KeyList ypaworld_keys
{
    Common::Ini::Key("netgame.exclusivegem", Common::Ini::KT_BOOL, true),
    Common::Ini::Key("net.waitstart",        Common::Ini::KT_DIGIT, (int32_t)150000),
    Common::Ini::Key("net.kickoff",          Common::Ini::KT_DIGIT, (int32_t)20000),
    Common::Ini::Key("game.debug",           Common::Ini::KT_BOOL)
};

int word_5A50C2;
int word_5A50AC;
int word_5A50B0;
int dword_5A50B2;
int dword_5A50B2_h;
int word_5A50AE;
int word_5A50BC;
int word_5A50BA;
int word_5A50BE;
int word_5A50C0;

int dword_5A50B6;
int dword_5A50B6_h;

char **ypaworld__string_pointers;

GuiList stru_5C91D0;

uint32_t bact_id = 0x10000;

// method 169
int dword_5A7A80;

NC_STACK_ypaworld::NC_STACK_ypaworld()
: _unitsList(this, NC_STACK_ypabact::GetKidRefNode, World::BLIST_UNITS)
, _deadCacheList(this, NC_STACK_ypabact::GetKidRefNode, World::BLIST_CACHE)
, _history(4096)
{
    GameShell = NULL;
    b64_parms = NULL;
    _mapAbsMaxX = 0;
    _mapAbsMaxY = 0;
    _mapWidth = 0;
    _mapHeight = 0;
    _cells = NULL;

    map_Width_meters = 0.0;
    map_Height_meters = 0.0;
    field_30 = NULL;
    _powerStations = NULL;
    _powerStationsCount = 0;
    _lastUpdatedPowerStationID = 0;
    set_number = 0;
    additionalSet = NULL;
//nlist bact_list;
//nlist dead_cache;
    vhcls_models = NULL;
    legos = NULL;
    subSectors = NULL;
    secTypes = NULL;
    VhclProtos = NULL;
    WeaponProtos = NULL;
    BuildProtos.clear();
    RoboProtos.clear();
//yw_f80 field_80[8];

    memset(build_hp_ref, 0, sizeof(build_hp_ref));
    memset(sqrt_table, 0, sizeof(sqrt_table));

    current_bact = NULL;
    field_1334 = vec3d();
    field_1340 = mat3x3();
    sky_loaded_base = NULL;
    field_1368 = 0;
    additionalBeeBox = NULL;
    colsub_sklt = NULL;
    colcomp_sklt = NULL;
    colsub_sklt_intrn = NULL;
    colcomp_sklt_intrn = NULL;
    tracyrmp_ilbm = NULL;
    shadermp_ilbm = NULL;
    _win3d = NULL;
    field_138c = 0;

//slurp slurps1[6][6];
//slurp slurps2[6][6];
//slurp2 ColSide;
//slurp2 ColCross;

    field_15e4 = 0;
    field_15e8 = 0;
    field_15ec = 0;
    field_15f0 = 0;
    field_15f4 = 0;
    field_15f8 = 0;
    field_15fc = 0;

    audio_volume = 0;
    field_1604 = 0;
    field_1608 = 0;
    field_160c = 0;
    field_1610 = 0;
    timeStamp = 0;
    field_1618 = 0;
    field_161c = 0;
    buildDate = "";
    field_1624 = 0;
    field_1628 = 0;
    field_162A = 0;
    GUI_OK = 0;

    for ( auto &x : tiles )
        x = NULL;

//nlist field_17a0;
    screen_width = 0;
    screen_height = 0;
    isDragging = 0;
    draggingItem = NULL;
//shortPoint field_17b8;
    draggingLock = 0;
    _mouseGrabbed = 0; // Grab mouse for unit steer-turn
    field_17c4 = 0;
    field_17c8 = -1;
    tooltips = NULL;
//rgbiColor iniColors[70];
    field_1a00 = 0;
    field_1a04 = 0;
    field_1a08 = 0;
    field_1a0c = 0;
    field_1a10 = 0;

    field_1a1c = 0;
    field_1a20 = 0;
    font_default_h = 0;
    font_default_w__a = 0;
    font_yscrl_bkg_w = 0;
    font_xscrl_h = 0;
    font_default_w__b = 0;
    field_1a38 = 0;
    font_yscrl_h = 0;
    icon_order__w = 0;
    icon_order__h = 0;
    icon_help__w = 0;
    icon_help__h = 0;
    icon_energy__h = 0;
    icon0___h = 0;
    field_1a58 = 0;
    field_1a5c = 0;
    field_1a60 = NULL;
    field_1a64 = 0;
    field_1A66 = 0;
    field_1a68 = 0; //Network?
//xyz field_1a6c;

//xyz field_1a7c;

//xyz field_1a8c;
    field_1a98 = NULL;
    field_1a9c = 0.0;
    field_1aa0 = 0;
    field_1aa4 = 0;
    field_1aa8 = 0;
    field_1aac = NULL;
    field_1ab0 = 0;
    field_1ab4 = 0;
//xyz field_1ab8;

    memset(pointers, 0, sizeof(pointers));
    memset(pointers__bitm, 0, sizeof(pointers__bitm));

    field_1b1c = 0;
    field_1b20 = 0; // saved mouse x
    field_1b22 = 0; // saved mouse y
//ypabact_arg65 field_1b24;
    field_1b68 = 0; // debug info draw
    field_1B6A = 0;
    field_1b6c = 0;
    field_1B6E = 0;
    field_1b70 = 0;
    field_1b74 = 0;
    UserRobo = NULL;
    UserUnit = NULL;
    _UserRoboKidsList = NULL;

    memset(sectors_count_by_owner, 0, sizeof(sectors_count_by_owner));

    for (int i = 0; i < 8; i++)
    {
        field_1bac[i] = 0;
        field_1bcc[i] = 0.0;
        field_1bec[i] = 0.0;
    }

    memset(_cmdrsRemap, 0, sizeof(_cmdrsRemap));

    _activeCmdrID = 0;
    _activeCmdrRemapIndex = 0;
    _cmdrsCount = 0;
    _kidsCount = 0;
    field_241c = 0;
    _lastMsgSender = NULL;
    field_2424 = 0;
    do_screenshooting = 0;
    screenshot_seq_id = 0; //Screenshoting sequence id
    screenshot_seq_frame_id = 0; //Screenshoting frame id
    replayer = NULL; // For play replay
    sceneRecorder = NULL; // For record replay

    field_2b78 = 0;
    field_2b7c = 0;
    last_modify_vhcl = 0;
    last_modify_weapon = 0;
    last_modify_build = 0;

    LevelNet = NULL;
    _levelInfo = NULL;

    brief.clear();
    
    _historyLastIsTimeStamp = false;

    
    superbomb_wall_vproto = 0;
    superbomb_center_vproto = 0;
    field_7278 = 0;
    field_727c = 0;
    field_7280 = 0;
    memset(lang_name, 0, sizeof(lang_name));
    lang_strings = NULL;
    very_big_array__p_begin = NULL;
    lang_strings__end = NULL;
    string_pointers = NULL;
    string_pointers_p2 = NULL;
    typ_map = NULL;
    own_map = NULL;
    blg_map = NULL;
    hgt_map = NULL;

    copyof_typemap = NULL;
    copyof_ownermap = NULL;
    memset(&wis_skeletons, 0, sizeof(wis_skeletons)); //hacky
    field_739A = 0;

    field_73CE = 0;

//	save_status robo_map_status;
//	save_status robo_finder_status;
//	save_status vhcl_map_status;
//	save_status vhcl_finder_status;
    fxnumber = 0;
    for (int i = 0; i < 8; i++)
    {
        dbg_num_sqd_counter[i] = 0;
        dbg_num_vhcl_counter[i] = 0;
        dbg_num_flk_counter[i] = 0;
        dbg_num_robo_counter[i] = 0;
        dbg_num_wpn_counter[i] = 0;
    }

    dbg_num_sqd = 0;
    dbg_num_sqd_max = 0;
    dbg_num_vhcl = 0;
    dbg_num_vhcl_max = 0;
    dbg_num_flk = 0;
    dbg_num_flk_max = 0;
    dbg_num_robo = 0;
    dbg_num_robo_max = 0;
    dbg_num_wpn = 0;
    dbg_num_wpn_max = 0;
    input_class = NULL;
    field_7562 = 0;
    field_7566 = 0;
    field_756A = 0;
    field_756E = 0;
    field_7572 = NULL;
    windp = NULL;
    netUpdateTime = 0;
    isNetGame = 0;
    netGameStarted = 0;
    field_7586 = 0;

    netInfoOverkill = 0;

    netInterpolate = 0;
    field_759E = 0;
    field_75A2 = 0; //array 64?

    memset(field_75E2, 0, sizeof(field_75E2)); //array 64?


    netgame_exclusivegem = 0;
    field_7626 = 0;

    p_1_grp_cnt = 0;
    memset(p_1_grp, 0, sizeof(p_1_grp));
//	player_status playerstatus[8];
//	player_status field_7796[8];
    _maxRoboEnergy = 0;
    _maxReloadConst = 0;
    samples = NULL;
    field_7882 = 0;
    field_7886 = 0;
    field_788A = 0;
    field_788E = 0.0; //input sliders
    field_7892 = 0.0; //input sliders
    field_7896 = 0.0; //input sliders
    field_789A = 0.0; //input sliders
    field_789E = 0.0; //input sliders

    for (std::string &movie : movies)
		movie.clear();

    field_81AB = 0;
    field_81AF = NULL;
    field_81B3 = NULL;
    one_game_res = 0;
    shell_default_res = 0;
    game_default_res = 0;
    map_events = NULL;
    max_impulse = 0.0;

    memset(&field_81CB, 0, sizeof(field_81CB));

    vehicle_sector_ratio_1 = 0.0;
    unit_limit = 0;
    unit_limit_arg = 0;
    unit_limit_type = 0;
    unit_limit_1 = 0;
    unit_limit_arg_1 = 0;
    unit_limit_type_1 = 0;
    field_826F = Input::KC_NONE;
    TOD_ID = 0;
    beam_energy_start = 0;
    beam_energy_add = 0;
    beamenergy = 0;
    field_8283 = 0;
    easy_cheat_keys = 0;

    playerOwner = 0;
    
    
    
    _extraViewEnable = false;
    _extraViewNumber = -1;
}

namespace World
{
int AssignParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
	if ( !StriCmp(p1, "end") )
	{
		return ScriptParser::RESULT_SCOPE_END;
	}
	else if ( !p1.empty() && !p2.empty() )
	{
		set_prefix_replacement(p1, p2);
		ypa_log_out("parsing assign.txt: set assign %s to %s\n", p1.c_str(), p2.c_str());
		return ScriptParser::RESULT_OK;
	}
	else
	{
		return ScriptParser::RESULT_BAD_DATA;
	}

    return ScriptParser::RESULT_UNKNOWN;
}

bool ParseAssignFile(const std::string &file)
{
	ScriptParser::HandlersList hndls{
		AssignParser::MakeParser()
	};

	return ScriptParser::ParseFile(file, hndls, 0);
}

}





void sub_4711E0(NC_STACK_ypaworld *yw)
{
    yw->very_big_array__p_begin = yw->lang_strings;
    yw->lang_strings__end = yw->lang_strings + 0x20000;

    memset(yw->lang_strings, 0, 0x20000);
    memset(yw->string_pointers, 0, sizeof(char *) * 2600);

    memset(yw->lang_name, 0, sizeof(yw->lang_name));
    strcpy(yw->lang_name, "default");
}

int yw_InitLocale(NC_STACK_ypaworld *yw)
{
    int v3 = 0;
    yw->lang_strings = (char *)AllocVec(0x20000, 1);

    if ( yw->lang_strings )
    {
        yw->string_pointers = (char **)AllocVec(sizeof(char *) * 2600, 65537);

        if ( yw->string_pointers )
        {
            yw->string_pointers_p2 = yw->string_pointers;
            sub_4711E0(yw);
            v3 = 1;
        }
    }

    if ( !v3 )
    {
////	sub_4D99C4(); // free language dll

        if ( yw->lang_strings )
            nc_FreeMem(yw->lang_strings);

        if ( yw->string_pointers )
            nc_FreeMem(yw->string_pointers);

        yw->very_big_array__p_begin = NULL;
        yw->lang_strings__end = NULL;
        yw->string_pointers = NULL;
        yw->lang_strings = NULL;
    }
    return v3;
}

bool NC_STACK_ypaworld::sub_4DA354(const std::string &filename)
{
    std::string buf = get_prefix_replacement("rsrc");
    set_prefix_replacement("rsrc", "data:");

    ScriptParser::HandlersList parsers {
        new World::Parsers::VhclProtoParser(this),
        new World::Parsers::WeaponProtoParser(this),
        new World::Parsers::BuildProtoParser(this)
    };

    bool res = ScriptParser::ParseFile(filename, parsers, ScriptParser::FLAG_NO_SCOPE_SKIP);
    set_prefix_replacement("rsrc", buf);

    return res;
}

int init_prototypes(NC_STACK_ypaworld *yw)
{
    yw->VhclProtos = new VhclProto[256];
    yw->WeaponProtos = new WeapProto[128];
    yw->BuildProtos.resize(128);
    yw->RoboProtos.resize(16);

    if ( yw->VhclProtos && yw->WeaponProtos )
    {
        if ( yw->sub_4DA354(yw->initScriptLoc) )
            return 1;
    }

    return 0;
}


recorder *recorder_allocate()
{
    recorder *rcrd = (recorder *)AllocVec(sizeof(recorder), 65537);

    if ( !rcrd )
        return NULL;

    rcrd->bacts = (NC_STACK_ypabact **)AllocVec(sizeof(NC_STACK_ypabact *) * 1024, 1);
    rcrd->oinf = (trec_bct *)AllocVec(sizeof(trec_bct) * 1024, 1);
    rcrd->sound_status = (uint16_t *)AllocVec(sizeof(uint16_t) * 2 * 1024, 1);
    rcrd->field_20 = AllocVec(0x4000, 1);
    rcrd->ainf = (uint8_t *)AllocVec(0x2000, 1);

    if ( !rcrd->bacts || !rcrd->oinf || !rcrd->sound_status || !rcrd->field_20 || !rcrd->ainf )
    {
        if ( rcrd->bacts )
            nc_FreeMem(rcrd->bacts);

        if ( rcrd->oinf )
            nc_FreeMem(rcrd->oinf);

        if ( rcrd->sound_status )
            nc_FreeMem(rcrd->sound_status);

        if ( rcrd->field_20 )
            nc_FreeMem(rcrd->field_20);

        if ( rcrd->ainf )
            nc_FreeMem(rcrd->ainf);

        nc_FreeMem(rcrd);

        return NULL;
    }

    rcrd->field_2C = 1024;
    rcrd->max_bacts = 1024;
    return rcrd;
}

int yw_InitSceneRecorder(NC_STACK_ypaworld *yw)
{
    yw->sceneRecorder = recorder_allocate();
    return yw->sceneRecorder != 0;
}

void yw_setInitScriptLoc(NC_STACK_ypaworld *yw)
{
    bool ok = false;
    FSMgr::FileHandle *fil = uaOpenFile("env:startup.def", "r");

    if (fil)
    {
        std::string line;
        if ( fil->ReadLine(&line) )
        {
            size_t en = line.find_first_of("\n;");
            if (en != std::string::npos)
                line.erase(en);

            yw->initScriptLoc = line;
            ok = true;
        }


        delete fil;
    }

    if (!ok)
        yw->initScriptLoc = "data:scripts/startup.scr";
}

size_t NC_STACK_ypaworld::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
    {
        ypa_log_out("yw_main.c/OM_NEW: _supermethoda() failed!\n");
        return 0;
    }

    set_prefix_replacement("rsrc", "mc2res");
    set_prefix_replacement("data", "Data");
    set_prefix_replacement("save", "Save");
    set_prefix_replacement("help", "Help");
    set_prefix_replacement("mov", "Data:mov");
    set_prefix_replacement("levels", "Levels");
    set_prefix_replacement("mbpix", "levels:mbpix");
    set_prefix_replacement("locale", "locale");
    set_prefix_replacement("scripts", "data:scripts");

    if ( !World::ParseAssignFile("env:assign.txt") )
        ypa_log_out("Warning, no env:assign.txt script.\n");

    yw_setInitScriptLoc(this);

    if ( !yw_InitLocale(this) )
    {
        ypa_log_out("yw_main.c/OM_NEW: yw_InitLocale() failed!\n");
        func1();
        return 0;
    }

    const char *langname = "language";
    ypaworld_func166(&langname); // Load lang strings

//		if ( !make_CD_CHECK(1, 1, v10, v9) )
//		{
//			func1();
//			return NULL;
//		}

    vhcls_models = (vhclBases *)AllocVec(sizeof(vhclBases) * 512, 65537);
    legos = (cityBases *)AllocVec(sizeof(cityBases) * 256, 65537);
    subSectors = (subSec *)AllocVec(sizeof(subSec) * 256, 65537);
    secTypes = (secType *)AllocVec(sizeof(secType) * 256, 65537);

    if ( !vhcls_models || !legos || !subSectors || !secTypes )
    {
        func1();
        return 0;
    }

    if ( !init_prototypes(this) )
    {
        ypa_log_out("ERROR: couldn't initialize prototypes.\n");
        func1();
        return 0;
    }

    screen_width = GFXEngine::GFXe.getScreenW();
    screen_height = GFXEngine::GFXe.getScreenH();

    _unitsList.clear();
    field_17a0.clear();
    _deadCacheList.clear();


    fxnumber = 16;
    field_1368 = stak.Get<int32_t>(YW_ATT_VISSECTORS, YW_RENDER_SECTORS_DEF);
    field_15e4 = stak.Get<int32_t>(YW_ATT_NORMVISLIMIT, 1400);
    field_15e8 = stak.Get<int32_t>(YW_ATT_FADELENGTH, 600);
    field_15ec = stak.Get<int32_t>(YW_ATT_SKYVISLIMIT, 4200);
    field_15f0 = stak.Get<int32_t>(YW_ATT_SKYFADELENGTH, 1100);
    _mapAbsMaxX = stak.Get<int32_t>(YW_ATT_MAPMAX_X, 64);
    _mapAbsMaxY = stak.Get<int32_t>(YW_ATT_MAPMAX_Y, 64);
    _mapWidth = _mapAbsMaxX;
    _mapHeight = _mapAbsMaxY;
    field_15f4 = stak.Get<int32_t>(YW_ATT_SKYHEIGHT, -550);
    field_15f8 = stak.Get<int32_t>(YW_ATT_SKYRENDER, 1);
    field_15fc = stak.Get<int32_t>(YW_ATT_DOENERGYRECALC, 1);

    buildDate = stak.Get<std::string>(YW_ATT_BUILD_DATE, "");
    
    for (char &chr : buildDate)
    {
        chr = toupper(chr);
        if ( chr < ' ' || chr > 'Z' )
            chr = '*';
    }



    build_hp_ref[0] = 3;
    for (int i = 1; i <= 100; i++ )
        build_hp_ref[i] = 2;
    for (int i = 101; i <= 200; i++ )
        build_hp_ref[i] = 1;
    for (int i = 201; i < 256; i++ )
        build_hp_ref[i] = 0;

    for (int j = 0; j < 64; j++)
    {
        for (int i = 0; i < 64; i++)
        {
            sqrt_table[j][i] = sqrt(j * j + i * i);
        }
    }

    _cells = new cellArea[_mapAbsMaxX * _mapAbsMaxY];

    if ( !_cells )
    {
        ypa_log_out("yw_main.c/OM_NEW: alloc of cell area failed!\n");
        func1();
        return 0;
    }
    if ( !yw_InitSceneRecorder(this) )
    {
        ypa_log_out("yw_main.c/OM_NEW: init scene recorder failed!\n");
        func1();
        return 0;
    }
    if ( !yw_InitTooltips(this) )
    {
        ypa_log_out("yw_main.c/OM_NEW: yw_InitTooltips() failed!\n");
        func1();
        return 0;
    }

    one_game_res = 1;
    shell_default_res = (640 << 12) | 480;
    game_default_res = (640 << 12) | 480;

    if ( !yw_InitLevelNet() )
    {
        ypa_log_out("yw_main.c/OM_NEW: yw_InitLevelNet() failed!\n");
        func1();
        return 0;
    }

    field_73CE |= 0x10;

    if ( !yw_InitNetwork(this) )
    {
        ypa_log_out("yw_main.c/OM_NEW: yw_InitNetwork() failed!\n");
        func1();
        return 0;
    }

    field_138c = 0;
    
    UpdateGuiSettings();

    return 1;
}


size_t NC_STACK_ypaworld::func1()
{
    dprintf("MAKE ME %s\n","ypaworld_func1");
    return NC_STACK_nucleus::func1();
}

void sub_445230(NC_STACK_ypaworld *yw)
{
    if ( yw->current_bact->getBACT_extraViewer() )
    {
        NC_STACK_ypabact *v4 = yw->current_bact;

        yw->field_1334 = v4->_position + v4->_rotation.Transpose().Transform(v4->_viewer_position);

        yw->field_1340 = yw->current_bact->_viewer_rotation;
    }
    else
    {
        yw->field_1334 = yw->current_bact->_position;

        yw->field_1340 = yw->current_bact->_rotation;
    }
}

size_t NC_STACK_ypaworld::base_func64(base_64arg *arg)
{
    extern GuiList gui_lstvw; //In yw_game_ui.cpp
    extern GuiList lstvw2; //In yw_game_ui.cpp
    extern bool SPEED_DOWN_NET; //In yw_net.cpp

    if ( (gui_lstvw.IsClosed() && lstvw2.IsClosed())
            || (arg->field_8->KbdLastHit != Input::KC_RETURN && arg->field_8->KbdLastHit != Input::KC_ESCAPE) )
    {
        field_826F = Input::KC_NONE;
    }
    else
    {
        field_826F = arg->field_8->KbdLastHit;
        arg->field_8->KbdLastHit = Input::KC_NONE;
        arg->field_8->KbdLastDown = Input::KC_NONE;
        arg->field_8->HotKeyID = -1;
    }

    if ( !ypaworld_func64__sub4(this, arg) )
    {
        uint32_t v92 = profiler_begin();

        _win3d = GFXEngine::GFXe.getC3D();

        field_7626 = 0;
        b64_parms = arg;

        if ( do_screenshooting )
        {
            arg->TimeStamp -= arg->DTime;
            arg->field_8->Period = 40;
            arg->DTime = 40;
            arg->TimeStamp += arg->DTime;
        }

        if ( UserUnit )
        {
            if ( field_161c == 1 )
            {
                field_1334 = UserUnit->_position;
                field_1340 = UserUnit->_rotation;
            }

            vec3d a3 = UserUnit->_fly_dir * UserUnit->_fly_dir_length;

            SFXEngine::SFXe.sub_423EFC(arg->DTime, field_1334, a3, field_1340);
        }

        if ( field_161c == 1 )
        {
            yw_arg159 arg159;
            arg159.unit = UserRobo;
            arg159.field_4 = 128;
            arg159.txt = NULL;
            arg159.field_C = 41;

            ypaworld_func159(&arg159);
        }

        ypaworld_func64__sub6(this);
        ypaworld_func64__sub2(this);

        if ( !field_138c )
        {
            ypaworld_func64__sub1(arg->field_8); //Precompute input (add mouse turn)

            ClickBoxInf *winp = &arg->field_8->ClickInf;

            if ( field_1b1c )
            {
                if ( winp->move.ScreenPos.x != field_1b20 || winp->move.ScreenPos.y != field_1b22 )
                    field_1b1c = 0;
            }
            else
            {
                if ( arg->field_8->KbdLastHit != Input::KC_NONE )
                {
                    if ( arg->field_8->KbdLastHit != Input::KC_LMB && arg->field_8->KbdLastHit != Input::KC_RMB && arg->field_8->KbdLastHit != Input::KC_MMB && !(arg->field_8->Buttons.Is(4)) )
                    {
                        field_1b1c = 1;
                        field_1b20 = winp->move.ScreenPos.x;
                        field_1b22 = winp->move.ScreenPos.y;
                    }
                }
            }
        }

        timeStamp += arg->DTime;
        field_1618 = arg->DTime;
        field_161c++;

        field_1b24.user_action = 0;
        field_1b24.gTime = timeStamp;
        field_1b24.frameTime = arg->DTime;
        field_1b24.units_count = 0;
        field_1b24.inpt = arg->field_8;
        field_1B6E = 1024 / arg->DTime;
        p_1_grp[0][0] = field_1B6E;

        HistoryEventAdd(World::History::Frame(timeStamp));

        uint32_t v22 = profiler_begin();
        
        if ( isNetGame )
            yw_NetMsgHndlLoop(this);

        if ( !isNetGame || _levelInfo->State != 2 )
        {
            p_1_grp[0][6] = profiler_end(v22);

            uint32_t v23 = profiler_begin();

            for (int i = 0; i < _mapHeight * _mapWidth; i++)
            {
                cellArea *tmp = _cells + i;

                tmp->view_mask = 1 << tmp->owner;
            }

            for (NC_STACK_ypabact* &unit : _unitsList)
                unit->MarkSectorsForView();

            p_1_grp[0][2] = profiler_end(v23);

            if ( !field_138c )
            {
                _win3d->BeginFrame();
                _win3d->setRSTR_BGpen(0);
                _win3d->raster_func192(NULL);
            }

            ypaworld_func64__sub15(this);
            ypaworld_func64__sub16(this);
            ypaworld_func64__sub17(this);
            sub_4C40AC();
            ypaworld_func64__sub9(this);
            ypaworld_func64__sub19();
            ypaworld_func64__sub20(this, arg->DTime);

            if ( !field_138c )
            {
                uint32_t v33 = profiler_begin();

                ypaworld_func64__sub8(this);
                ypaworld_func64__sub7(arg->field_8);

                sub_445230(this);

                ypaworld_func64__sub14(this);
                ypaworld_func64__sub21(arg->field_8);

                p_1_grp[0][3] = profiler_end(v33);
            }

            if ( field_15fc )
                ypaworld_func64__sub5(this);

            if ( sceneRecorder->do_record )
                recorder_update_time(this, arg->DTime);

            hudi.field_0 = 0;
            hudi.field_4 = 0;

            field_1b74 = UserUnit->_pSector->owner;

            uint32_t v37 = profiler_begin();

            for ( NC_STACK_ypabact *unit : _unitsList.safe_iter() )
            {
                if (isNetGame && unit != UserRobo && unit->_bact_type == BACT_TYPES_ROBO)
                    unit->NetUpdate(&field_1b24);
                else
                    unit->Update(&field_1b24);

                field_1b24.units_count++;
            }

            p_1_grp[0][4] = profiler_end(v37);

            sub_445230(this);

            uint32_t v41 = profiler_begin();

            if ( isNetGame )
            {
                if ( arg->DTime == 1 )
                    field_7586 -= 20;
                else
                    field_7586 -= arg->DTime;

                if ( field_7586 <= 0 )
                {
                    windp_arg82 arg82;
                    arg82.senderID = GameShell->callSIGN.c_str();
                    arg82.senderFlags = 1;
                    arg82.receiverFlags = 2;
                    arg82.receiverID = 0;
                    arg82.guarant = 0;

                    uint32_t v44 = windp->FlushBuffer(arg82);

                    GameShell->netsend_count += v44;

                    if ( !GameShell->net_packet_min || v44 < GameShell->net_packet_min )
                        GameShell->net_packet_min = v44;

                    if ( v44 > GameShell->net_packet_max )
                        GameShell->net_packet_max = v44;

                    if ( v44 )
                        GameShell->net_packet_cnt++;

                    if ( SPEED_DOWN_NET )
                        field_7586 = 1500;
                    else
                        field_7586 = GameShell->flush_time_norm;
                }
            }
            p_1_grp[0][6] += profiler_end(v41);

            if ( UserUnit )
            {
                if ( GameShell )
                {
                    GameShell->samples1_info.field_0 = UserUnit->_position;
                    GameShell->samples2_info.field_0 = UserUnit->_position;

                    SFXEngine::SFXe.sb_0x4242e0(&GameShell->samples1_info);
                    SFXEngine::SFXe.sb_0x4242e0(&GameShell->samples2_info);
                }
            }

            ypaworld_func64__sub22(this); // scene events
            ypaworld_func64__sub23(this); // update sound messages

            if ( isNetGame ) // update additional sounds of netplay
            {
                if ( UserUnit )
                    GameShell->field_782.field_0 = UserUnit->_position;

                SFXEngine::SFXe.sb_0x4242e0(&GameShell->field_782);
            }

            const mat3x3 &v57 = SFXEngine::SFXe.sb_0x424c74();
            TFEngine::TForm3D *v58 = TFEngine::Engine.GetViewPoint();

            v58->locSclRot = v57 * v58->locSclRot;

            if ( sceneRecorder->do_record )
                recorder_write_frame();

            ypaworld_func64__sub3(this);

            if ( !field_138c )
            {
                uint32_t v62 = profiler_begin();

                if ( UserUnit->_sectX || UserUnit->_sectY )
                {
                    sb_0x4d7c08(this, arg, 1);

                    if ( isNetGame )
                        yw_NetDrawStats(this);
                }

                _win3d->EndFrame();

                p_1_grp[0][5] = profiler_end(v62);
            }

            FFeedback_Update(); // Do vibrate joystick

            sb_0x447720(this, arg->field_8); // Snaps/ start/stop recording

            p_1_grp[0][1] = profiler_end(v92);
            p_1_grp[0][7] = field_1b6c;

            sub_44A094(this);

            if ( !field_138c )
            {
                if ( field_7626 )
                {
                    field_138c = 1;

                    dprintf("MAKE ME %s (multiplayer part Messaging)\n", "ypaworld_func64");

//          v66 = get_lang_string(ypaworld__string_pointers, 650, "MESSAGE TO");
//
//          sprintf(&v68, "%s %s", v66, yw->field_762E);
//
//          memset(&dlgBox, 0, 40);
//
//          dlgBox.title = &v68;
//          dlgBox.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
//          dlgBox.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
//          dlgBox.result = 0;
//          dlgBox.timer_context = yw;
//          dlgBox.maxLen = 64;
//          dlgBox.time = 250;
//          dlgBox.startText = "";
//          dlgBox.timer_func = sub_44674C;
//
//          yw->win3d->windd_func322(&dlgBox);
//
                    field_138c = 0;
//
//          if ( dlgBox.result )
//            sub_4D9418(yw, dlgBox.result, yw->field_762E, yw->field_762A);
                }
            }

            if ( field_81AF )
            {
                const char *help_link = field_81AF;
                field_81AF = 0;

                ypaworld_func185(&help_link); //launch online help
            }

            //exit(1);
        }
    }

    return 1;
}

void sub_47C1EC(NC_STACK_ypaworld *yw, MapGem *gemProt, int *a3, int *a4)
{
    switch ( yw->GameShell->netPlayerOwner )
    {
    case 1:
        *a3 = gemProt->NwVprotoNum1;
        *a4 = gemProt->NwBprotoNum1;
        break;

    case 6:
        *a3 = gemProt->NwVprotoNum2;
        *a4 = gemProt->NwBprotoNum2;
        break;

    case 3:
        *a3 = gemProt->NwVprotoNum3;
        *a4 = gemProt->NwBprotoNum3;
        break;

    case 4:
        *a3 = gemProt->NwVprotoNum4;
        *a4 = gemProt->NwBprotoNum4;
        break;

    default:
        *a3 = 0;
        *a4 = 0;
        break;
    }
}

void sub_47C29C(NC_STACK_ypaworld *yw, cellArea *cell, int a3)
{
    MapGem &gem = yw->_Gems[a3];

    int a3a, a4;
    sub_47C1EC(yw, &gem, &a3a, &a4);

    yw->field_2b78 = a3;
    yw->field_2b7c = yw->timeStamp;
    yw->last_modify_vhcl = a3a;
    yw->last_modify_weapon = 0;
    yw->last_modify_build = a4;

    if ( a3a )
    {
        yw->VhclProtos[a3a].disable_enable_bitmask = 0;
        yw->VhclProtos[a3a].disable_enable_bitmask |= 1 << yw->GameShell->netPlayerOwner;
    }

    if ( a4 )
    {
        yw->BuildProtos[a4].EnableMask = 0;
        yw->BuildProtos[a4].EnableMask |= 1 << yw->GameShell->netPlayerOwner;
    }

    std::string v11 = get_lang_string(yw->string_pointers_p2, 221, "TECHNOLOGY UPGRADE!\n");

    yw_arg159 v14;
    v14.txt = v11.c_str();
    v14.unit = 0;
    v14.field_4 = 48;

    if ( gem.Type )
        v14.field_C = World::Log::GetUpgradeLogID(gem.Type);
    else
        v14.field_C = 0;

    yw->ypaworld_func159(&v14);

    if ( yw->isNetGame )
    {
        if ( yw->netgame_exclusivegem )
        {
            uamessage_upgrade upMsg;
            upMsg.msgID = UAMSG_UPGRADE;
            upMsg.owner = yw->GameShell->netPlayerOwner;
            upMsg.enable = 1;
            upMsg.upgradeID = a3;

            yw_arg181 arg181;
            arg181.recvID = 0;
            arg181.dataSize = sizeof(upMsg);
            arg181.garant = 1;
            arg181.recvFlags = 2;
            arg181.data = &upMsg;

            yw->ypaworld_func181(&arg181);
        }
    }

    cell->w_type = 7;
}

void ypaworld_func129__sub1(NC_STACK_ypaworld *yw, cellArea *cell, int a3)
{
    MapGem &gem = yw->_Gems[a3];

    int a3a;
    int a4;

    sub_47C1EC(yw, &gem, &a3a, &a4);

    if ( a3a )
        yw->VhclProtos[a3a].disable_enable_bitmask = 0;

    if ( a4 )
        yw->BuildProtos[a4].EnableMask = 0;

    std::string v13 = get_lang_string(yw->string_pointers_p2, 229, "TECH-UPGRADE LOST!  ");
    v13 += gem.MsgDefault;

    yw_arg159 arg159;
    arg159.unit = 0;
    arg159.field_4 = 80;
    arg159.txt = v13.c_str();
    arg159.field_C = 29;

    yw->ypaworld_func159(&arg159);

    if ( yw->isNetGame )
    {
        uamessage_upgrade upMsg;
        upMsg.msgID = UAMSG_UPGRADE;
        upMsg.owner = yw->GameShell->netPlayerOwner;
        upMsg.enable = 0;
        upMsg.upgradeID = a3;

        yw_arg181 arg181;
        arg181.recvFlags = 2;
        arg181.dataSize = sizeof(upMsg);
        arg181.recvID = 0;
        arg181.garant = 1;
        arg181.data = &upMsg;

        yw->ypaworld_func181(&arg181);
    }

    cell->w_id = 0;
    cell->w_type = 0;
}

void NC_STACK_ypaworld::yw_ActivateWunderstein(cellArea *cell, int gemid)
{
    last_modify_vhcl = 0;
    last_modify_build = 0;
    last_modify_weapon = 0;

    field_2b78 = gemid;
    field_2b7c = timeStamp;

    MapGem &gem = _Gems[gemid];

    if ( !gem.ScriptFile.empty() )
    {
        if ( !sub_4DA354(gem.ScriptFile) )
            ypa_log_out("yw_ActivateWunderstein: ERROR parsing script %s.\n", gem.ScriptFile.c_str());
    }
    else
    {
        std::string tmp = get_prefix_replacement("rsrc");
        set_prefix_replacement("rsrc", "data:");

        ScriptParser::HandlersList parsers {
            new World::Parsers::VhclProtoParser(this),
            new World::Parsers::WeaponProtoParser(this),
            new World::Parsers::BuildProtoParser(this)
        };

        ScriptParser::ParseStringList(gem.ActionsList, parsers, ScriptParser::FLAG_NO_SCOPE_SKIP);
        set_prefix_replacement("rsrc", tmp);
    }

    std::string txt = get_lang_string(string_pointers_p2, 221, "TECHNOLOGY UPGRADE!\n");

    yw_arg159 arg159;
    arg159.unit = NULL;
    arg159.field_4 = 48;
    arg159.txt = txt.c_str();

    if ( gem.Type )
        arg159.field_C = World::Log::GetUpgradeLogID(gem.Type);
    else
        arg159.field_C = 0;

    ypaworld_func159(&arg159);

    cell->w_type = 7;
}

void sub_44FD6C(NC_STACK_ypaworld *yw, cellArea *cell, int secX, int secY, int a5, int a6, int a7)
{
    NC_STACK_ypabact *cbact = yw->current_bact;

    int v8 = abs(cbact->_sectX - secX);
    int v9 = abs(cbact->_sectY - secY);

    if ( v8 + v9 <= (yw->field_1368 - 1) / 2 )
    {
        const cityBases &v10 = yw->legos[  yw->secTypes[cell->type_id].buildings[a5][a6]->health_models[a7]  ];

        float v32 = secX * 1200.0 + 600.0;
        float v27 = -(secY * 1200.0 + 600.0);

        if ( cell->comp_type != 1 )
        {
            v32 += (a5 - 1) * 300.0;
            v27 += (a6 - 1) * 300.0;
        }

        for (int i = 0; i < yw->fxnumber; i++)
        {
            if ( !v10.field_14[i] )
                break;

            ypaworld_arg146 arg146;
            arg146.vehicle_id = v10.field_14[i];
            arg146.pos.x = v32 + v10.field_24[i].pos_x;
            arg146.pos.y = cell->height + v10.field_24[i].pos_y;
            arg146.pos.z = v27 + v10.field_24[i].pos_z;

            NC_STACK_ypabact *boom = yw->ypaworld_func146(&arg146);

            if ( boom )
            {
                boom->_owner = 0;

                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_DEAD;
                arg78.setFlags = BACT_STFLAG_DEATH1;
                arg78.unsetFlags = 0;
                boom->SetState(&arg78);

                yw->ypaworld_func134(boom);

                bact_arg83 arg83;
                arg83.pos = arg146.pos;
                arg83.energ = 40000;
                arg83.pos2.x = v10.field_24[i].pos_x;
                arg83.pos2.y = -150.0;
                arg83.pos2.z = v10.field_24[i].pos_z;

                float tmp = arg83.pos2.length();

                if ( tmp > 0.1 )
                    arg83.pos2 /= tmp;

                arg83.force = 30.0;
                arg83.mass = 50.0;

                boom->ApplyImpulse(&arg83);
            }
        }
    }
}

void ypaworld_func129__sub0(NC_STACK_ypaworld *yw, cellArea *cell, yw_arg129 *arg)
{
    if ( cell->w_type == 2 )
    {
        if ( cell->owner == yw->UserRobo->_owner )
        {
            if ( arg->unit )
            {
                if ( yw->UserRobo->_owner != arg->unit->_owner && yw->timeStamp - yw->field_1a1c > 5000 )
                {
                    yw->field_1a1c = yw->timeStamp;

                    yw_arg159 arg159;
                    arg159.unit = NULL;
                    arg159.field_4 = 77;
                    arg159.txt = NULL;
                    arg159.field_C = 33;

                    yw->ypaworld_func159(&arg159);
                }
            }
        }
    }
}



void NC_STACK_ypaworld::ypaworld_func129(yw_arg129 *arg)
{
    int secX = arg->pos.x / 1200.0;
    int secY = -arg->pos.z / 1200.0;

    if ( secX > 0 && _mapWidth - 1 > secX && secY > 0 && _mapHeight - 1 > secY )
    {
        cellArea *cell = &_cells[secX + _mapWidth * secY];

        int v8 = (int)(arg->pos.x / 150.0) % 8;

        int v10;

        if ( v8 < 3 )
            v10 = 1;
        else if ( v8 < 5)
            v10 = 2;
        else
            v10 = 3;

        v8 = (int)(-arg->pos.z / 150.0) % 8;

        int v14;

        if ( v8 < 3 )
            v14 = 1;
        else if ( v8 < 5)
            v14 = 2;
        else
            v14 = 3;

        if ( v10 && v14 && cell->w_type != 1 )
        {
            int v38;
            int a5;

            if ( cell->comp_type == 1 )
            {
                v38 = 0;
                a5 = 0;
            }
            else
            {
                a5 = v10 - 1;
                v38 = 2 - (v14 - 1);
            }

            int v16 = cell->buildings_health[a5][v38];

            int v34 = v16 - arg->field_10 * (100 - legos[  secTypes[cell->type_id].buildings[a5][v38]->health_models[  build_hp_ref[v16]  ]  ].field_10 ) / 100 / 400;

            if ( v34 < 0 )
                v34 = 0;
            else if ( v34 > 255 )
                v34 = 255;

            int v18 = build_hp_ref[v16];
            int v36 = build_hp_ref[v34];

            if ( v18 > v36 )
            {
                while ( v18 > v36 )
                {
                    sub_44FD6C(this, cell, secX, secY, a5, v38, v18);

                    v18--;
                }
            }
            else if ( v18 < v36 )
            {
                while ( v18 < v36 )
                {
                    sub_44FD6C(this, cell, secX, secY, a5, v38, v18);

                    v18++;
                }
            }

            cell->buildings_health[a5][v38] = v34;

            ypaworld_func129__sub0(this, cell, arg);

            CellCheckHealth(cell, secX, secY, arg->field_14, arg);

            if ( cell->w_type == 4 )
            {
                if ( UserRobo && UserRobo->_owner == cell->owner )
                {
                    if ( isNetGame )
                        sub_47C29C(this, cell, cell->w_id);
                    else
                        yw_ActivateWunderstein(cell, cell->w_id);                  

                    HistoryEventAdd( World::History::Upgrade(secX, secY, cell->owner, _Gems[ field_2b78 ].Type, last_modify_vhcl, last_modify_weapon, last_modify_build) );
                }
            }
            else if ( cell->w_type == 7 )
            {
                if ( isNetGame )
                {
                    int v27 = 0;

                    for (int i = 0; i < 3; i++)
                    {
                        for (int j = 0; j < 3; j++)
                        {
                            v27 += cell->buildings_health[i][j];
                        }
                    }

                    if ( !v27 )
                        ypaworld_func129__sub1(this, cell, cell->w_id);
                }
            }
        }
    }
}


size_t NC_STACK_ypaworld::ypaworld_func130(yw_130arg *arg)
{
    arg->sec_x = arg->pos_x / 1200;
    arg->sec_z = -arg->pos_z / 1200;

    arg->pos_x_cntr = arg->pos_x - arg->sec_x * 1200.0 + -600.0;
    arg->pos_y_cntr = arg->pos_z + arg->sec_z * 1200.0 + 600.0;

    if ( arg->sec_x < 0 || arg->sec_z < 0 || arg->sec_x >= _mapWidth || arg->sec_z >= _mapHeight )
    {
        ypa_log_out("YWM_GETSECTORINFO %d %d max: %d %d\n", arg->sec_x, arg->sec_z, _mapWidth, _mapHeight);
        ypa_log_out("YWM_GETSECTORINFO ausserhalb!!!\n");

        arg->pcell = NULL;

        return 0;
    }

    arg->pcell = &_cells[_mapWidth * arg->sec_z + arg->sec_x];
    return 1;
}


void NC_STACK_ypaworld::ypaworld_func131(NC_STACK_ypabact *bact)
{
    current_bact = bact;

    setYW_userVehicle(bact);
}


void NC_STACK_ypaworld::ypaworld_func132(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func132");
}


void NC_STACK_ypaworld::ypaworld_func133(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func133");
}


void NC_STACK_ypaworld::ypaworld_func134(NC_STACK_ypabact *bact)
{
    newMaster_msg arg73;

    arg73.bact = NULL;
    arg73.list = &_unitsList;

    bact->SetNewMaster(&arg73);
}


void NC_STACK_ypaworld::ypaworld_func135(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func135");
}


void NC_STACK_ypaworld::ypaworld_func136(ypaworld_arg136 *arg)
{
    arg->tVal = 2.0;
    arg->isect = 0;

    vec3d stpos = arg->stPos;

    float pos_xx = stpos.x + arg->vect.x;
    float pos_zz = stpos.z + arg->vect.z;

    int dx = (stpos.x + 150) / 300;
    int dxx = (pos_xx + 150) / 300;

    int dz = (-stpos.z + 150) / 300;

    int dzz = (-pos_zz + 150) / 300;

    int elems = 0;
    struct_44dbf8 a6[4];


    if ( dx == dxx && dz == dzz )
    {
        elems = 1;
        a6[0].field_1C = 0;
        sub_44DBF8(this, dx, dz, dx, dz, a6[0], arg->flags);
    }
    else if ( dx == dxx || dz == dzz )
    {
        elems = 2;
        a6[0].field_1C = 0;
        a6[1].field_1C = 0;
        sub_44DBF8(this, dx, dz, dx,  dz,  a6[0], arg->flags);
        sub_44DBF8(this, dx, dz, dxx, dzz, a6[1], arg->flags);
    }
    else
    {
        elems = 4;
        a6[0].field_1C = 0;
        a6[1].field_1C = 0;
        a6[2].field_1C = 0;
        a6[3].field_1C = 0;
        sub_44DBF8(this, dx, dz, dx,  dz,  a6[0], arg->flags);
        sub_44DBF8(this, dx, dz, dx,  dzz, a6[1], arg->flags);
        sub_44DBF8(this, dx, dz, dxx, dz,  a6[2], arg->flags);
        sub_44DBF8(this, dx, dz, dxx, dzz, a6[3], arg->flags);
    }

    for (int i = 0; i < elems; i++)
    {
        if ( a6[i].field_1C )
        {
            if ( a6[i].field_1C != 1)
                sub_44E07C(this, a6[i]);

            arg->stPos = stpos - a6[i].pos;

            sub_44D8B8(arg, a6[i]);

            if ( arg->isect )
                break;
        }
    }
}


void NC_STACK_ypaworld::ypaworld_func137(ypaworld_arg137 *arg)
{
    arg->coll_count = 0;

    vec3d pos = arg->pos;

    int dxx = (pos.x + 150) / 300;
    int dzz = (-pos.z + 150) / 300;
    int xxmr = (pos.x - arg->radius + 150) / 300;
    int zzmr = (-(pos.z - arg->radius) + 150) / 300;
    int xxpr = (pos.x + arg->radius + 150) / 300;
    int zzpr = (-(pos.z + arg->radius) + 150) / 300;

    struct_44dbf8 a6;

    for (int i = 0; i < 9; i++)
    {
        a6.field_1C = 0;

        switch ( i )
        {
        case 0:
            sub_44DBF8(this, dxx, dzz, dxx, dzz, a6, arg->field_30);
            break;

        case 1:
            if ( dxx != xxmr )
                sub_44DBF8(this, dxx, dzz, xxmr, dzz, a6, arg->field_30);
            break;

        case 2:
            if ( dxx != xxpr )
                sub_44DBF8(this, dxx, dzz, xxpr, dzz, a6, arg->field_30);
            break;

        case 3:
            if ( dzz != zzmr )
                sub_44DBF8(this, dxx, dzz, dxx, zzmr, a6, arg->field_30);
            break;

        case 4:
            if ( dzz != zzpr )
                sub_44DBF8(this, dxx, dzz, dxx, zzpr, a6, arg->field_30);
            break;

        case 5:
            if ( dxx != xxmr && dzz != zzmr )
                sub_44DBF8(this, dxx, dzz, xxmr, zzmr, a6, arg->field_30);
            break;

        case 6:
            if ( dxx != xxpr && dzz != zzmr )
                sub_44DBF8(this, dxx, dzz, xxpr, zzmr, a6, arg->field_30);
            break;

        case 7:
            if ( dxx != xxpr && dzz != zzpr )
                sub_44DBF8(this, dxx, dzz, xxpr, zzpr, a6, arg->field_30);
            break;

        case 8:
            if ( dxx != xxmr && dzz != zzpr )
                sub_44DBF8(this, dxx, dzz, xxmr, zzpr, a6, arg->field_30);
            break;
        }

        if ( a6.field_1C )
        {
            if ( a6.field_1C != 1 )
                sub_44E07C(this, a6);

            arg->pos = pos - a6.pos;

            ypaworld_func137__sub0(arg, a6);
        }
    }
}


void NC_STACK_ypaworld::ypaworld_func138(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func138");
}


void NC_STACK_ypaworld::ypaworld_func139(GuiBase *lstvw)
{
    if ( !(lstvw->flags & GuiBase::FLAG_WITH_ICON) )
        lstvw->flags &= ~GuiBase::FLAG_ICONIFED;

    if ( lstvw->flags & GuiBase::FLAG_IN_LIST )
        ypaworld_func140(lstvw);

    lstvw->Attach(field_17a0);

    lstvw->flags |= GuiBase::FLAG_IN_LIST;

    if ( lstvw->flags & GuiBase::FLAG_WITH_ICON )
        lstvw->iconBox.pobject = lstvw;

    lstvw->pobject = lstvw;

    if ( lstvw->flags & GuiBase::FLAG_ICONIFED )
        INPe.AddClickBoxFront(&lstvw->iconBox);
    else if ( lstvw->IsOpen() )
        INPe.AddClickBoxFront(lstvw);
}


void NC_STACK_ypaworld::ypaworld_func140(GuiBase *lstvw)
{
    if ( lstvw->flags & GuiBase::FLAG_IN_LIST )
    {
        lstvw->Detach();

        lstvw->flags &= ~GuiBase::FLAG_IN_LIST;

        if ( lstvw->flags & GuiBase::FLAG_ICONIFED )
            INPe.RemClickBox(&lstvw->iconBox);
        else if ( lstvw->IsOpen() )
            INPe.RemClickBox(lstvw);
    }
}


void NC_STACK_ypaworld::ypaworld_func143(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func143");
}


void NC_STACK_ypaworld::ypaworld_func144(NC_STACK_ypabact *bacto)
{
    if ( bacto->_bact_type == BACT_TYPES_MISSLE )
    {
        if ( bacto->_primTtype )
            ypa_log_out("OH NO! The DEATH CACHE BUG is back!\n");
    }

    SFXEngine::SFXe.sub_423DD8(&bacto->_soundcarrier);

    newMaster_msg cache;
    cache.bact = NULL;
    cache.list = &_deadCacheList;

    bacto->SetNewMaster(&cache);

    bact_arg80 v6;
    v6.pos.x = 600.0;
    v6.pos.y = -50000.0;
    v6.pos.z = -600.0;
    v6.field_C = 2;

    bacto->SetPosition(&v6);

    bacto->_status_flg |= BACT_STFLAG_NORENDER;
}


size_t NC_STACK_ypaworld::ypaworld_func145(NC_STACK_ypabact *bact)
{
    if ( current_bact )
    {
        int v6 = abs(current_bact->_sectX - bact->_sectX);
        int v7 = abs(current_bact->_sectY - bact->_sectY);

        if ( v6 + v7 <= (field_1368 - 1) / 2 )
            return 1;
    }

    for ( NC_STACK_ypabact* &station : _unitsList ) //Robos
    {
        if ( station->_status_flg & BACT_STFLAG_ISVIEW )
        {
            int v10 = abs(station->_sectX - bact->_sectX);
            int v11 = abs(station->_sectY - bact->_sectY);

            if ( v10 + v11 <= (field_1368 - 1) / 2 )
                return 1;
        }

        
        for ( NC_STACK_ypabact* &comm : station->_kidList ) // Squad comms
        {
            if ( comm->_status_flg & BACT_STFLAG_ISVIEW )
            {
                int v10 = abs(comm->_sectX - bact->_sectX);
                int v11 = abs(comm->_sectY - bact->_sectY);

                if ( v10 + v11 <= (field_1368 - 1) / 2 )
                    return 1;
            }


            for ( NC_STACK_ypabact* &unit : comm->_kidList ) // Squad units
            {
                if ( unit->_status_flg & BACT_STFLAG_ISVIEW )
                {
                    int v10 = abs(unit->_sectX - bact->_sectX);
                    int v11 = abs(unit->_sectY - bact->_sectY);

                    if ( v10 + v11 <= (field_1368 - 1) / 2 )
                        return 1;
                }
            }
        }
    }

    return 0;
}


NC_STACK_ypabact * NC_STACK_ypaworld::ypaworld_func146(ypaworld_arg146 *vhcl_id)
{
    if ( vhcl_id->vehicle_id > 256 )
        return NULL;

    VhclProto *vhcl = &VhclProtos[vhcl_id->vehicle_id];

    NC_STACK_ypabact *bacto = yw_createUnit(vhcl->model_id);

    if ( bacto )
    {
        bacto->_energy = vhcl->energy;
        bacto->_energy_max = vhcl->energy;
        bacto->_shield = vhcl->shield;
        bacto->_mass = vhcl->mass;
        bacto->_force = vhcl->force;
        bacto->_maxrot = vhcl->maxrot;
        bacto->_height = vhcl->height;
        bacto->_radius = vhcl->radius;
        bacto->_viewer_radius = vhcl->vwr_radius;
        bacto->_overeof = vhcl->overeof;
        bacto->_viewer_overeof = vhcl->vwr_overeof;
        bacto->_airconst = vhcl->airconst;
        bacto->_airconst_static = vhcl->airconst;
        bacto->_adist_sector = vhcl->adist_sector;
        bacto->_adist_bact = vhcl->adist_bact;
        bacto->_sdist_sector = vhcl->sdist_sector;
        bacto->_sdist_bact = vhcl->sdist_bact;
        bacto->_radar = vhcl->radar;
        bacto->_gun_radius = vhcl->gun_radius;
        bacto->_gun_power = vhcl->gun_power;
        bacto->_pitch_max = vhcl->max_pitch;
        bacto->_vehicleID = vhcl_id->vehicle_id;
        bacto->_weapon = vhcl->weapon;

        if ( vhcl->weapon == -1 )
            bacto->_weapon_flags = 0;
        else
            bacto->_weapon_flags = WeaponProtos[ vhcl->weapon ].model_id;

        bacto->_mgun = vhcl->mgun;
        bacto->_fire_pos.x = vhcl->fire_x;
        bacto->_fire_pos.y = vhcl->fire_y;
        bacto->_fire_pos.z = vhcl->fire_z;
        bacto->_gun_angle = vhcl->gun_angle;
        bacto->_gun_angle_user = vhcl->gun_angle;
        bacto->_num_weapons = vhcl->num_weapons;
        bacto->_kill_after_shot = vhcl->kill_after_shot;
        bacto->_vp_normal.base = vhcls_models[ vhcl->vp_normal ].base;
        bacto->_vp_normal.trigo = vhcls_models[ vhcl->vp_normal ].trigo;
        bacto->_vp_fire.base = vhcls_models[ vhcl->vp_fire ].base;
        bacto->_vp_fire.trigo = vhcls_models[ vhcl->vp_fire ].trigo;
        bacto->_vp_dead.base = vhcls_models[ vhcl->vp_dead ].base;
        bacto->_vp_dead.trigo = vhcls_models[ vhcl->vp_dead ].trigo;
        bacto->_vp_wait.base = vhcls_models[ vhcl->vp_wait ].base;
        bacto->_vp_wait.trigo = vhcls_models[ vhcl->vp_wait ].trigo;
        bacto->_vp_megadeth.base = vhcls_models[ vhcl->vp_megadeth ].base;
        bacto->_vp_megadeth.trigo = vhcls_models[ vhcl->vp_megadeth ].trigo;
        bacto->_vp_genesis.base = vhcls_models[ vhcl->vp_genesis ].base;
        bacto->_vp_genesis.trigo = vhcls_models[ vhcl->vp_genesis ].trigo;

        bacto->_destroyFX = vhcl->dest_fx;

        memset(bacto->_vp_fx_models, 0, sizeof(NC_STACK_base *) * 32);
        memset(bacto->_vp_fx_tform, 0, sizeof(TFEngine::TForm3D *) * 32);

        bacto->_scale_start = vhcl->scale_fx_p0;
        bacto->_scale_speed = vhcl->scale_fx_p1;
        bacto->_scale_accel = vhcl->scale_fx_p2;
        bacto->_scale_duration = vhcl->scale_fx_p3;
        bacto->_scale_pos = 0;

        for (int i = 0; vhcl->scale_fx_pXX[ i ]; i++ )
        {
            bacto->_vp_fx_models[i] = vhcls_models[ vhcl->scale_fx_pXX[ i ] ].base;
            bacto->_vp_fx_tform[i] = vhcls_models[ vhcl->scale_fx_pXX[ i ] ].trigo;

            bacto->_status_flg |= BACT_STFLAG_SEFFECT;
        }

        SFXEngine::SFXe.sub_423DB0(&bacto->_soundcarrier);

        for (int i = 0; i < 12; i++)
            sub_44BF34(&vhcl->sndFX[i]);

        for (int i = 0; i < 12; i++)
        {
            userdata_sample_info *smpl_inf = &bacto->_soundcarrier.samples_data[ i ];

            smpl_inf->volume = vhcl->sndFX[i].volume;
            smpl_inf->pitch = vhcl->sndFX[i].pitch;

            if ( i <= 3 || (i >= 7 && i <= 8) )
                smpl_inf->flags |= 1;

            if ( vhcl->sndFX[i].single_sample )
                smpl_inf->psampl = vhcl->sndFX[i].single_sample->getSMPL_pSample();
            else
                smpl_inf->psampl = 0;

            if ( vhcl->sndFX[i].sndPrm.slot )
            {
                smpl_inf->paletteFX = &vhcl->sndFX[i].sndPrm;
                smpl_inf->flags |= 8;
            }
            else
            {
                smpl_inf->flags &= ~8;
            }

            if ( vhcl->sndFX[i].sndPrm_shk.slot )
            {
                smpl_inf->shakeFX = &vhcl->sndFX[i].sndPrm_shk;
                smpl_inf->flags |= 0x40;
            }
            else
            {
                smpl_inf->flags &= ~0x40;
            }

            if ( vhcl->sndFX[i].extS.cnt )
            {
                smpl_inf->smplExt = &vhcl->sndFX[i].extS; //CHECK IT
                smpl_inf->flags |= 0x200;
            }
            else
            {
                smpl_inf->flags &= ~0x200;
            }
        }

        bacto->_pitch = bacto->_soundcarrier.samples_data[0].pitch;
        bacto->_volume = bacto->_soundcarrier.samples_data[0].volume;

        bacto->func2(vhcl->initParams);

        bact_arg80 arg80;
        arg80.pos = vhcl_id->pos;
        arg80.field_C = 0;
        bacto->SetPosition(&arg80);

        setState_msg arg119;
        arg119.newStatus = BACT_STATUS_NORMAL;
        arg119.setFlags = 0;
        arg119.unsetFlags = 0;
        bacto->SetStateInternal(&arg119);
    }

    return bacto;
}


NC_STACK_ypamissile * NC_STACK_ypaworld::ypaworld_func147(ypaworld_arg146 *arg)
{
    if ( arg->vehicle_id > 128 )
        return NULL;

    WeapProto *wproto = &WeaponProtos[arg->vehicle_id];

    if ( !(wproto->model_id & 1) )
        return NULL;

    NC_STACK_ypamissile *wobj = dynamic_cast<NC_STACK_ypamissile *>( yw_createUnit(wproto->field_0) );

    if ( !wobj )
        return NULL;

    wobj->_energy = wproto->energy;
    wobj->_energy_max = wproto->energy;
    wobj->_shield = 0;
    wobj->_mass = wproto->mass;
    wobj->_force = wproto->force;
    wobj->_maxrot = wproto->maxrot;
    wobj->_height = wproto->field_8D8;
    wobj->_radius = wproto->radius;
    wobj->_viewer_radius = wproto->vwr_radius;
    wobj->_overeof = wproto->overeof;
    wobj->_viewer_overeof = wproto->vwr_overeof;
    wobj->_airconst = wproto->airconst;
    wobj->_airconst_static = wproto->airconst;
    wobj->_adist_sector = wproto->field_890;
    wobj->_adist_bact = wproto->field_894;
    wobj->_vehicleID = arg->vehicle_id;
    wobj->_weapon = 0;

    wobj->_vp_normal =   vhcls_models[wproto->vp_normal];
    wobj->_vp_fire =     vhcls_models[wproto->vp_fire];
    wobj->_vp_dead =     vhcls_models[wproto->vp_dead];
    wobj->_vp_wait =     vhcls_models[wproto->vp_wait];
    wobj->_vp_megadeth = vhcls_models[wproto->vp_megadeth];
    wobj->_vp_genesis =  vhcls_models[wproto->vp_genesis];

    for (int i = 0; i < 16; i++)
        wobj->_destroyFX[i] = wproto->dfx[i];

    int missileType;

    switch(wproto->model_id)
    {
    case 1:
        missileType = NC_STACK_ypamissile::MISL_BOMB;
        break;

    case 7:
        missileType = NC_STACK_ypamissile::MISL_TARGETED;
        break;

    case 11:
        missileType = NC_STACK_ypamissile::MISL_OBSAVOID;
        break;

    case 17:
        missileType = NC_STACK_ypamissile::MISL_GRENADE;
        break;

    default:
        missileType = NC_STACK_ypamissile::MISL_DIRECT;
        break;
    }

    wobj->setMISS_lifeTime(wproto->life_time);
    wobj->setMISS_delay(wproto->delay_time);
    wobj->setMISS_driveTime(wproto->drive_time);
    wobj->setMISS_type(missileType);
    wobj->setMISS_powHeli(wproto->energy_heli * 1000.0);
    wobj->setMISS_powTank(wproto->energy_tank * 1000.0);
    wobj->setMISS_powFlyer(wproto->energy_flyer * 1000.0);
    wobj->setMISS_powRobo(wproto->energy_robo * 1000.0);

    if (tuneGetWeaponRadiusFix())
    {
        wobj->setMISS_radHeli(wproto->radius_heli);
        wobj->setMISS_radTank(wproto->radius_tank);
        wobj->setMISS_radFlyer(wproto->radius_flyer);
        wobj->setMISS_radRobo(wproto->radius_robo);
    }
    else
    {
        wobj->setMISS_radHeli(0.0);
        wobj->setMISS_radTank(0.0);
        wobj->setMISS_radFlyer(0.0);
        wobj->setMISS_radRobo(0.0);
    }

    SFXEngine::SFXe.sub_423DB0(&wobj->_soundcarrier);

    for (int i = 0; i < 3; i++)
        sub_44BF34(&wproto->sndFXes[i]);

    for (int i = 0; i < 3; i++)
    {
        userdata_sample_info *v25 = &wobj->_soundcarrier.samples_data[i];

        v25->volume = wproto->sndFXes[i].volume;
        v25->pitch = wproto->sndFXes[i].pitch;

        if ( i == 0 )
            v25->flags |= 1;

        if ( wproto->sndFXes[i].single_sample )
            v25->psampl = wproto->sndFXes[i].single_sample->getSMPL_pSample();
        else
            v25->psampl = 0;

        if ( wproto->sndFXes[i].sndPrm.slot )
        {
            v25->flags |= 8;
            v25->paletteFX = &wproto->sndFXes[i].sndPrm;
        }
        else
        {
            v25->flags &= ~8;
        }

        if ( wproto->sndFXes[i].sndPrm_shk.slot )
        {
            v25->flags |= 0x40;
            v25->shakeFX = &wproto->sndFXes[i].sndPrm_shk;
        }
        else
        {
            v25->flags &= ~0x40;
        }

        if ( wproto->sndFXes[i].extS.cnt )
        {
            v25->flags |= 0x200;
            v25->smplExt = &wproto->sndFXes[i].extS;
        }
        else
        {
            v25->flags &= ~0x200;
        }
    }

    wobj->func2(wproto->initParams);

    bact_arg80 arg80;

    arg80.pos = arg->pos;
    arg80.field_C = 1;

    wobj->SetPosition(&arg80);

    setState_msg arg119;

    arg119.setFlags = 0;
    arg119.unsetFlags = 0;
    arg119.newStatus = BACT_STATUS_NORMAL;

    wobj->SetStateInternal(&arg119);

    return wobj;
}


size_t NC_STACK_ypaworld::ypaworld_func148(ypaworld_arg148 *arg)
{
    int y = arg->y;
    int x = arg->x;

    cellArea *cell = &_cells[x + y * _mapWidth];

    bool UserInSec = false;

    for ( NC_STACK_ypabact* &node : cell->unitsList )
    {
        
        if ( UserUnit == node || node->_bact_type == BACT_TYPES_ROBO)
        {
            UserInSec = true;
            break;
        }
    }

    if ( UserUnit  &&  cell == UserUnit->_pSector )
        UserInSec = true;

    if ( cell->w_type == 1 )
        return 0;
    else if ( UserInSec  && !arg->field_C )
        return 0;
    else if ( cell->w_type == 2 )
        PowerStationErase(cell->w_id);
    else if ( (cell->w_type == 4 || cell->w_type == 5 || cell->w_type == 6) && !arg->field_C )
        return 0;
    else if ( cell->w_type == 7 && isNetGame )
        return 0;

    if ( arg->field_C )
    {
        sb_0x456384(x, y, arg->ownerID2, arg->blg_ID, arg->field_18 & 1);
    }
    else
    {
        ypaworld_func148__sub0(this, x, y);

        if ( !ypaworld_func148__sub1(arg->ownerID, 3000, x, y, arg->ownerID2, arg->blg_ID) )
            return 0;
    }

    return 1;
}


void NC_STACK_ypaworld::ypaworld_func149(ypaworld_arg136 *arg)
{
    arg->tVal = 2.0;
    arg->isect = 0;

    vec3d stpos = arg->stPos;

    int v33 = (stpos.x + 150.0) / 75.0 * 16384.0;
    int v34 = (stpos.z - 150.0) / 75.0 * 16384.0;

    float v36 = fabs(arg->vect.x);
    float v39 = fabs(arg->vect.z);

    int v31;
    int v32;
    float v37;

    if ( v36 != 0.0 || v39 != 0.0 )
    {
        if ( v36 <= v39 )
        {
            v32 = (v36 / v39 * 16384.0);

            if ( arg->vect.x < 0.0 )
                v32 = -v32;

            if ( arg->vect.z >= 0.0 )
                v31 = 16384;
            else
                v31 = -16384;

            v37 = v39;
        }
        else
        {
            if ( arg->vect.x >= 0.0 )
                v32 = 16384;
            else
                v32 = -16384;

            v31 = (v39 / v36 * 16384.0);

            if ( arg->vect.z < 0.0 )
                v31 = -v31;

            v37 = v36;
        }
    }
    else
    {
        v31 = 0;
        v37 = 0.0;
        v32 = 0;
    }

    int a2a = v33 >> 16;
    int a3a = -v34 >> 16;

    struct_44dbf8 a6;
    int v10, v11;

    do // Don't like this :E
    {
        a6.field_1C = 0;

        v10 = v33 >> 16;
        v11 = -v34 >> 16;

        sub_44DBF8(this, a2a, a3a, v10, v11, a6, arg->flags);

        if ( a6.field_1C )
        {
            if ( a6.field_1C != 1 )
                sub_44E07C(this, a6);

            arg->stPos = stpos - a6.pos;

            sub_44D8B8(arg, a6);

            if ( arg->isect )
                return;
        }

        do // Don't like this :E
        {
            v37 -= 75.0;
            v33 += v32;
            v34 += v31;
        }
        while ( v33 >> 16 == v10 && -v34 >> 16 == v11 && v37 > 0.0 );

    }
    while( v37 > 0.0 );

    int v24 = ((int)((arg->vect.x + stpos.x + 150.0) / 75.0 * 16384.0)) >> 16;
    int v27 = ((int)((arg->vect.z + stpos.z - 150.0) / 75.0 * 16384.0)) >> 16;

    if ( v24 != v10 || -v27 != v11 )
    {
        a6.field_1C = 0;

        sub_44DBF8(this, a2a, a3a, v24, -v27, a6, arg->flags);

        if ( a6.field_1C )
        {
            if ( a6.field_1C != 1 )
                sub_44E07C(this, a6);

            arg->stPos = stpos - a6.pos;

            sub_44D8B8(arg, a6);
        }
    }
}


void NC_STACK_ypaworld::ypaworld_func150(yw_arg150 *arg)
{
    arg->field_24 = NULL;

    int v6 = arg->pos.x / 300.0 * 16384.0;
    int v7 = arg->pos.z / 300.0 * 16384.0;

    float v47 = fabs(arg->field_18.x);
    float v46 = fabs(arg->field_18.z);

    arg->field_28 = arg->field_18.length();

    int v27, v28, v35;

    if ( v47 != 0.0 || v46 != 0.0 )
    {
        if ( v47 <= v46 )
        {
            v27 = (v47 * 16384.0 / v46);

            if ( arg->field_18.x < 0.0 )
                v27 = -v27;

            if ( arg->field_18.z >= 0.0 )
                v28 = 16384;
            else
                v28 = -16384;

            v35 = v46;
        }
        else
        {
            if ( arg->field_18.x >= 0.0 )
                v27 = 16384;
            else
                v27 = -16384;

            v28 = (v46 * 16384.0 / v47);

            if ( arg->field_18.z < 0.0 )
                v28 = -v28;

            v35 = v47;
        }
    }
    else
    {
        v28 = 0;
        v35 = 0;
        v27 = 0;
    }

    vec3d v41 = arg->field_18 / arg->field_28;

    while ( v35 >= 0 )
    {
        int v12 = -v7 >> 16;
        int v29 = v6 >> 16;

        if ( v29 >= 0 && v29 < _mapWidth && v12 >= 0 && v12 < _mapHeight )
        {
            for ( NC_STACK_ypabact* &sect_bacts : _cells[v29 + v12 * _mapWidth].unitsList )
            {
                if ( sect_bacts != arg->unit && sect_bacts->_status != BACT_STATUS_DEAD )
                {
                    if ( !(arg->unit == UserUnit && field_1b70) || sect_bacts != UserRobo )
                    {
                        vec3d v36 = sect_bacts->_position - arg->pos;
                        vec3d v16 = v41 * v36;

                        if ( v16.length() < sect_bacts->_radius )
                        {
                            float v30 = v36.length();

                            if ( v41.dot(v36) / v30 > 0.0 )
                            {
                                if ( arg->field_28 > v30 - sect_bacts->_radius )
                                {
                                    arg->field_24 = sect_bacts;
                                    arg->field_28 = v30 - sect_bacts->_radius;
                                }
                            }
                        }
                    }
                }
            }
        }

        if ( arg->field_24 )
            break;

        do
        {
            v6 += v27;
            v7 += v28;
            v35 -= 300;
        }
        while ( (v6 >> 16) == v29 && (-v7 >> 16) == v12 && v35 >= 0 );
    }
}


void NC_STACK_ypaworld::DeleteLevel()
{
    sub_471AB8();

    if ( _levelInfo->State == 1 )
    {
        field_7278 = 1;
        if ( field_81AB )
            _levelInfo->JodieFoster[ field_81AB ] = 1;
    }
    else
    {
        field_7278 = 0;
    }

    if ( _levelInfo->State == 1 )
    {
        if ( GameShell )
        {
            FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("save:%s/sgisold.txt", GameShell->user_name), "w");

            if ( fil )
                delete fil;

            std::string buf = fmt::sprintf("%s/user.txt", GameShell->user_name);

            yw_arg172 arg171;
            arg171.usertxt = buf.c_str();
            arg171.field_4 = GameShell->user_name.c_str();
            arg171.usr = GameShell;
            arg171.field_10 = 0;
            arg171.field_8 = 255;

            ypaworld_func171(&arg171);

            fil = uaOpenFile("env:user.def", "w");

            if ( fil )
            {
                fil->printf(GameShell->user_name);
                delete fil;
            }
        }
    }

    if ( isNetGame )
    {
        if ( !GameShell->sentAQ )
            sub_47DB04(0);

        GameShell->ypaworld_func151__sub7();
        GameShell->yw_netcleanup();

        field_7278 = 1;
        field_727c = 1;

        if ( UserUnit )
            field_7280 = UserUnit->_owner;
    }
    else
    {
        field_7280 = 0;
        field_727c = 0;
    }

    if ( UserUnit )
        playerOwner = UserUnit->_owner;
    else
        playerOwner = 0;

    if ( sceneRecorder->do_record )
        recorder_stoprec(this);

    do_screenshooting = 0;
    sceneRecorder->do_record = 0;

    NC_STACK_bitmap *disk = loadDisk_screen(this);

    if ( disk )
    {
        draw_splashScreen(this, disk);
        deleteSplashScreen(this, disk);
    }

    ypaworld_func151__sub5(this);
    ypaworld_func151__sub6(this);

    SFXEngine::SFXe.setMasterVolume(audio_volume);

    GUI_Close();

    if ( sky_loaded_base )
    {
        delete_class_obj(sky_loaded_base);
        sky_loaded_base = NULL;
    }

    int plowner;
    if ( UserRobo )
        plowner = UserRobo->_owner;
    else
        plowner = 0;

    while ( !_deadCacheList.empty() )
    {
        NC_STACK_ypabact *bct = _deadCacheList.front();

        if ( field_727c )
        {
            if ( plowner != bct->_owner && bct->_bact_type == BACT_TYPES_ROBO )
                NetRemove(bct);
        }

        Nucleus::Delete(bct);
    }

    while ( !_unitsList.empty() )
    {
        NC_STACK_ypabact *bct = _unitsList.front();

        if ( field_727c )
        {
            if ( plowner != bct->_owner && bct->_bact_type == BACT_TYPES_ROBO )
                NetRemove(bct);
        }

        Nucleus::Delete(bct);
    }

    // NetRemove can fill deadcache, so clean it again
    while ( !_deadCacheList.empty() )
        Nucleus::Delete(_deadCacheList.front());

    ypaworld_func151__sub0(this);

    sb_0x44ac24(this);

    ypaworld_func151__sub1(this);

    if ( typ_map )
    {
        delete typ_map;
        typ_map = NULL;
    }

    if ( own_map )
    {
        delete own_map;
        own_map = NULL;
    }

    if ( blg_map )
    {
        delete blg_map;
        blg_map = NULL;
    }

    if ( hgt_map )
    {
        delete hgt_map;
        hgt_map = NULL;
    }

    if ( GameShell )
    {
        GameShell->samples1_info.field_0 = vec3d(0.0, 0.0, 0.0);

        GameShell->samples2_info.field_0 = vec3d(0.0, 0.0, 0.0);
    }

    if ( field_727c )
    {
        if ( VhclProtos )
        {
            if ( WeaponProtos )
            {
                sub_4DA354(initScriptLoc);
            }
        }
    }
}


void NC_STACK_ypaworld::ypaworld_func153(bact_hudi *arg)
{
    hudi = *arg;
}

void UserData::sub_46D2B4()
{
    NC_STACK_input *input_class = INPe.getPInput();

    int v10 = field_D36;

    for (int i = 0; i <= 48; i++)
        input_class->SetHotKey(i, "nop");

    for (int i = 1; i <= 45; i++)
    {
        field_D36 = i;
        p_YW->ypaworld_func173(this);
    }

    field_D36 = v10;
}


size_t NC_STACK_ypaworld::ypaworld_func154(UserData *usr)
{
    GameShell = usr;
    usr->p_ypaworld = this;
    usr->p_YW = this;

    _levelInfo->State = 8;
    usr->envMode = ENVMODE_TITLE;

    Common::Ini::ParseIniFile(DefaultIniFile, &ypaworld_keys);

    netgame_exclusivegem = ypaworld_keys[0].Get<bool>();

    ypaworld__string_pointers = getYW_localeStrings();

    usr->profiles.clear();
    usr->video_mode_list.clear();
    usr->lang_dlls.clear();

    LoadKeyNames();

    fill_videmodes_list(usr);
    listSaveDir("save:");
    listLocaleDir(usr, "locale");


    usr->usernamedir = "NEWUSER";
    usr->usernamedir_len = usr->usernamedir.size();

    usr->field_0x8 = 1;
    usr->field_1612 = 0;
    usr->field_D36 = 1;   

    usr->samples2_info.field_0 = vec3d(0.0, 0.0, 0.0);
    usr->samples2_info.field_C = vec3d(0.0, 0.0, 0.0);

    usr->samples1_info.field_0 = usr->samples2_info.field_0;
    usr->samples1_info.field_C = usr->samples2_info.field_C;

    for (int i = 0; i < 16; i++)
    {
        usr->samples2_info.samples_data[i].volume = 127;
        usr->samples2_info.samples_data[i].pitch = 0;
        usr->samples1_info.samples_data[i].volume = usr->samples2_info.samples_data[i].volume;
        usr->samples1_info.samples_data[i].pitch = usr->samples2_info.samples_data[i].pitch;
    }

    SFXEngine::SFXe.sub_423DB0(&usr->samples1_info);
    SFXEngine::SFXe.sub_423DB0(&usr->samples2_info);
    SFXEngine::SFXe.sub_423DB0(&usr->field_782);

    if ( !usr->ShellSoundsLoad() )
    {
        ypa_log_out("Error: Unable to load from Shell.ini\n");
        return 0;
    }

    usr->field_FBE = 0;

    usr->InputConfig[World::INPUT_BIND_DRIVE_DIR]   = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 3,  Input::KC_RIGHT, Input::KC_LEFT);
    usr->InputConfig[World::INPUT_BIND_DRIVE_SPEED] = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 4,  Input::KC_UP, Input::KC_DOWN);
    usr->InputConfig[World::INPUT_BIND_FLY_DIR]     = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 0,  Input::KC_RIGHT, Input::KC_LEFT);
    usr->InputConfig[World::INPUT_BIND_FLY_HEIGHT]  = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 1,  Input::KC_UP, Input::KC_DOWN);
    usr->InputConfig[World::INPUT_BIND_FLY_SPEED]   = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 2,  Input::KC_CTRL, Input::KC_SHIFT);
    usr->InputConfig[World::INPUT_BIND_GUN_HEIGHT]  = UserData::TInputConf(World::INPUT_BIND_TYPE_SLIDER, 5,  Input::KC_A, Input::KC_Y);
    usr->InputConfig[World::INPUT_BIND_FIRE]        = UserData::TInputConf(World::INPUT_BIND_TYPE_BUTTON, 0,  Input::KC_SPACE);
    usr->InputConfig[World::INPUT_BIND_CAMFIRE]     = UserData::TInputConf(World::INPUT_BIND_TYPE_BUTTON, 1,  Input::KC_TAB);
    usr->InputConfig[World::INPUT_BIND_GUN]         = UserData::TInputConf(World::INPUT_BIND_TYPE_BUTTON, 2,  Input::KC_RETURN);
    usr->InputConfig[World::INPUT_BIND_BRAKE]       = UserData::TInputConf(World::INPUT_BIND_TYPE_BUTTON, 3,  Input::KC_NUM0);
    usr->InputConfig[World::INPUT_BIND_HUD]         = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 25, Input::KC_V);
    usr->InputConfig[World::INPUT_BIND_NEW]         = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 2,  Input::KC_N);
    usr->InputConfig[World::INPUT_BIND_ADD]         = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 3,  Input::KC_A);
    usr->InputConfig[World::INPUT_BIND_ORDER]       = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 0,  Input::KC_O);
    usr->InputConfig[World::INPUT_BIND_ATTACK]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 1,  Input::KC_SPACE);
    usr->InputConfig[World::INPUT_BIND_CONTROL]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 4,  Input::KC_C);
    usr->InputConfig[World::INPUT_BIND_AUTOPILOT]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 7,  Input::KC_G);
    usr->InputConfig[World::INPUT_BIND_MAP]         = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 8,  Input::KC_M);
    usr->InputConfig[World::INPUT_BIND_SQ_MANAGE]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 9,  Input::KC_F);
    usr->InputConfig[World::INPUT_BIND_LANDLAYER]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 10, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_OWNER]       = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 11, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_HEIGHT]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 12, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_LOCKVIEW]    = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 14, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_ZOOMIN]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 16, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_ZOOMOUT]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 17, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_MINIMAP]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 18, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_NEXT_COMM]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 20, Input::KC_F1);
    usr->InputConfig[World::INPUT_BIND_TO_HOST]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 21, Input::KC_F2);
    usr->InputConfig[World::INPUT_BIND_NEXT_UNIT]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 22, Input::KC_F3);
    usr->InputConfig[World::INPUT_BIND_TO_COMM]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 23, Input::KC_F4);
    usr->InputConfig[World::INPUT_BIND_QUIT]        = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 24, Input::KC_ESCAPE);
    usr->InputConfig[World::INPUT_BIND_LOG_WND]     = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 27, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_LAST_MSG]    = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 31, Input::KC_BACKSPACE);
    usr->InputConfig[World::INPUT_BIND_PAUSE]       = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 32, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_TO_ALL]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 37, Input::KC_NUM5);
    usr->InputConfig[World::INPUT_BIND_AGGR_1]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 38, Input::KC_1);
    usr->InputConfig[World::INPUT_BIND_AGGR_2]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 39, Input::KC_2);
    usr->InputConfig[World::INPUT_BIND_AGGR_3]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 40, Input::KC_3);
    usr->InputConfig[World::INPUT_BIND_AGGR_4]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 41, Input::KC_4);
    usr->InputConfig[World::INPUT_BIND_AGGR_5]      = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 42, Input::KC_5);
    usr->InputConfig[World::INPUT_BIND_WAPOINT]     = UserData::TInputConf(World::INPUT_BIND_TYPE_BUTTON, 4,  Input::KC_SHIFT);
    usr->InputConfig[World::INPUT_BIND_HELP]        = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 43, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_LAST_SEAT]   = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 44, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_SET_COMM]    = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 45, Input::KC_NONE);
    usr->InputConfig[World::INPUT_BIND_ANALYZER]    = UserData::TInputConf(World::INPUT_BIND_TYPE_HOTKEY, 46, Input::KC_NONE);

    usr->sub_46D2B4();
    
    usr->field_1756 = -2;

    windp_arg87 v67;

    if (!windp->GetRemoteStart(&v67) )
    {
        ypa_log_out("Error while remote start check\n");
        return  0;
    }

    if ( v67.isClient )
    {
        GameShell->callSIGN = v67.callSIGN;

        if ( v67.isHoster )
            GameShell->isHost = 1;
        else
            GameShell->isHost = 0;

        GameShell->remoteMode = 1;

        usr->netLevelID = 0;
        usr->envMode = ENVMODE_NETPLAY;

        windp_arg79 v68;

        if ( usr->isHost )
        {
            v68.mode = 0;
            v68.ID = 0;

            while ( windp->GetPlayerData(&v68) && StriCmp(v68.name, usr->callSIGN) )
                v68.ID++;

            usr->players2[v68.ID].rdyStart = 1;
            usr->rdyStart = 1;
            usr->netSelMode = UserData::NETSCREEN_CHOOSE_MAP;
        }
        else
        {
            usr->netSelMode = UserData::NETSCREEN_INSESSION;
        }

        v68.mode = 0;
        v68.ID = 0;

        while ( windp->GetPlayerData(&v68) )
        {
            strncpy(GameShell->players2[v68.ID].name, v68.name.c_str(), 64);
            v68.ID++;
        }
        usr->update_time_norm = 400;
        usr->flush_time_norm = 400;
    }
    else
    {
        GameShell->remoteMode = 0;
    }

    usr->WaitForDemo = 200000;

    if ( !usr->remoteMode )
        ypaworld_func154__sub0(this);

    return 1;
}


void NC_STACK_ypaworld::ypaworld_func155(UserData *usr)
{
    usr->yw_netcleanup();

    usr->profiles.clear();

    usr->video_mode_list.clear();

    usr->lang_dlls.clear();

    SFXEngine::SFXe.sub_424CC8();

    for (int i = 0; i < 16; i++)
    {
        if (usr->samples1[i])
        {
            delete_class_obj(usr->samples1[i]);
            usr->samples1[i] = NULL;
        }

        if (usr->samples2[i])
        {
            delete_class_obj(usr->samples2[i]);
            usr->samples2[i] = NULL;
        }
    }

    if ( usr->field_ADA )
    {
        delete_class_obj(usr->field_ADA);
        usr->field_ADA = NULL;
    }

    SFXEngine::SFXe.sub_423DD8(&usr->samples1_info);
    SFXEngine::SFXe.sub_423DD8(&usr->samples2_info);
    SFXEngine::SFXe.sub_423DD8(&usr->field_782);
}



void sub_4E6FEC(NC_STACK_ypaworld *yw)
{
    stru_LevelNet *lvlnet;

    lvlnet = yw->LevelNet;
    if ( lvlnet->ilbm_menu_map )
    {
        delete_class_obj(lvlnet->ilbm_menu_map);
        lvlnet->ilbm_menu_map = NULL;
    }
    if ( lvlnet->ilbm_rollover_map )
    {
        delete_class_obj(lvlnet->ilbm_rollover_map);
        lvlnet->ilbm_rollover_map = NULL;
    }
    if ( lvlnet->ilbm_finished_map )
    {
        delete_class_obj(lvlnet->ilbm_finished_map);
        lvlnet->ilbm_finished_map = NULL;
    }
    if ( lvlnet->ilbm_enabled_map )
    {
        delete_class_obj(lvlnet->ilbm_enabled_map);
        lvlnet->ilbm_enabled_map = NULL;
    }
    if ( lvlnet->ilbm_mask_map )
    {
        delete_class_obj(lvlnet->ilbm_mask_map);
        lvlnet->ilbm_mask_map = NULL;
    }
}


void sb_0x4e75e8__sub1(NC_STACK_ypaworld *yw, int mode)
{
    int v37 = 1;

    if ( yw->LevelNet->bg_n )
    {
        std::string oldRsrc = get_prefix_replacement("rsrc");

        set_prefix_replacement("rsrc", "levels:");

        int v38 = 0;
        int v39 = 65535;
        for (int i = 0; i < yw->LevelNet->bg_n; i++)
        {

            int xx = (yw->LevelNet->background_map[i].size_x - yw->screen_width);
            int yy = (yw->LevelNet->background_map[i].size_y - yw->screen_height);

            int sq = sqrt(xx * xx + yy * yy);

            if (sq < v39)
            {
                v38 = i;
                v39 = sq;
            }
        }

        std::string menu_map;
        std::string rollover_map;
        std::string mask_map;
        std::string finished_map;
        std::string enabled_map;

        NC_STACK_bitmap *ilbm_menu_map  = NULL;
        NC_STACK_bitmap *ilbm_rollover_map = NULL;
        NC_STACK_bitmap *ilbm_mask_map = NULL;
        NC_STACK_bitmap *ilbm_finished_map = NULL;
        NC_STACK_bitmap *ilbm_enabled_map = NULL;

        switch ( mode )
        {
        case ENVMODE_TITLE:
        case ENVMODE_INPUT:
        case ENVMODE_SETTINGS:
        case ENVMODE_NETPLAY:
        case ENVMODE_SELLOCALE:
        case ENVMODE_ABOUT:
        case ENVMODE_SELPLAYER:
        case ENVMODE_HELP:
            menu_map  = yw->LevelNet->menu_map[v38].map_name;
            rollover_map = yw->LevelNet->settings_map[v38].map_name;
            break;
        case ENVMODE_TUTORIAL:
            menu_map  = yw->LevelNet->tut_background_map[v38].map_name;
            mask_map = yw->LevelNet->tut_mask_map[v38].map_name;
            rollover_map = yw->LevelNet->tut_rollover_map[v38].map_name;
            break;
        case ENVMODE_SINGLEPLAY:
            menu_map  = yw->LevelNet->background_map[v38].map_name;
            rollover_map = yw->LevelNet->rollover_map[v38].map_name;
            finished_map = yw->LevelNet->finished_map[v38].map_name;
            mask_map = yw->LevelNet->mask_map[v38].map_name;
            enabled_map = yw->LevelNet->enabled_map[v38].map_name;
            break;
        default:
            break;
        }

        if ( !menu_map.empty() )
        {
            ilbm_menu_map = Nucleus::CInit<NC_STACK_ilbm>({
                {NC_STACK_rsrc::RSRC_ATT_NAME, menu_map},
                {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1} } );
            if ( !ilbm_menu_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", menu_map.c_str());
                v37 = 0;
            }
        }

        if ( !rollover_map.empty() )
        {
            ilbm_rollover_map = Nucleus::CInit<NC_STACK_ilbm>({
                {NC_STACK_rsrc::RSRC_ATT_NAME, rollover_map},
                {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1} });
            if ( !ilbm_rollover_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", rollover_map.c_str());
                v37 = 0;
            }
        }

        if ( !finished_map.empty() )
        {
            ilbm_finished_map = Nucleus::CInit<NC_STACK_ilbm>({
                {NC_STACK_rsrc::RSRC_ATT_NAME, finished_map},
                {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1} });
            if ( !ilbm_finished_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", finished_map.c_str());
                v37 = 0;
            }
        }

        if ( !enabled_map.empty() )
        {
            ilbm_enabled_map = Nucleus::CInit<NC_STACK_ilbm>({
                {NC_STACK_rsrc::RSRC_ATT_NAME, enabled_map},
                {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1} });
            if ( !ilbm_enabled_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", enabled_map.c_str());
                v37 = 0;
            }
        }
        if ( !mask_map.empty() )
        {
            ilbm_mask_map = Nucleus::CInit<NC_STACK_ilbm>({{NC_STACK_rsrc::RSRC_ATT_NAME, mask_map}});
            if ( !ilbm_mask_map )
            {
                ypa_log_out("world.ini: Could not load %s\n", mask_map.c_str());
                v37 = 0;
            }
        }

        set_prefix_replacement("rsrc", oldRsrc);

        if ( !v37 )
        {
            if ( ilbm_menu_map )
            {
                delete_class_obj(ilbm_menu_map);
                ilbm_menu_map = NULL;
            }
            if ( ilbm_rollover_map )
            {
                delete_class_obj(ilbm_rollover_map);
                ilbm_rollover_map = NULL;
            }
            if ( ilbm_finished_map )
            {
                delete_class_obj(ilbm_finished_map);
                ilbm_finished_map = NULL;
            }
            if ( ilbm_enabled_map )
            {
                delete_class_obj(ilbm_enabled_map);
                ilbm_enabled_map = NULL;
            }
            if ( ilbm_mask_map )
            {
                delete_class_obj(ilbm_mask_map);
                ilbm_mask_map = NULL;
            }
        }
        sub_4E6FEC(yw);
        yw->LevelNet->ilbm_menu_map = ilbm_menu_map;
        yw->LevelNet->ilbm_mask_map = ilbm_mask_map;
        yw->LevelNet->ilbm_rollover_map = ilbm_rollover_map;
        yw->LevelNet->ilbm_finished_map = ilbm_finished_map;
        yw->LevelNet->ilbm_enabled_map = ilbm_enabled_map;
    }
}

void sb_0x4e75e8__sub0(NC_STACK_ypaworld *yw)
{
    ua_dRect regions[256];

    if ( yw->LevelNet->ilbm_mask_map )
    {
        for (int i = 0; i < 256; i++)
        {
            regions[i].x1 = 10000;
            regions[i].y1 = 10000;
            regions[i].x2 = -10000;
            regions[i].y2 = -10000;
        }

        ResBitmap *bitm = yw->LevelNet->ilbm_mask_map->GetResBmp();

        SDL_LockSurface(bitm->swTex);
        for (int y = 0; y < bitm->height; y++ )
        {
            uint8_t *ln = ((uint8_t *)bitm->swTex->pixels + y * bitm->swTex->pitch);

            for (int x = 0; x < bitm->width; x++)
            {

                int v7 = ln[x];
                if ( v7 < 256 )
                {
                    ua_dRect *v8 = &regions[v7];

                    if ( x < v8->x1 )
                        v8->x1 = x;

                    if ( x > v8->x2 )
                        v8->x2 = x;

                    if ( y < v8->y1 )
                        v8->y1 = y;

                    if ( y > v8->y2 )
                        v8->y2 = y;
                }
            }
        }

        for (int i = 0; i < 256; i++)
        {
            mapINFO *minf = &yw->LevelNet->mapInfos[i];

            if ( minf->field_0 && minf->field_0 != 4 && regions[i].x1 != 10000 )
            {
                minf->field_9C.x1 = 2.0 * ((float)(regions[i].x1) / (float)bitm->width) + -1.0;
                minf->field_9C.x2 = 2.0 * ((float)(regions[i].x2) / (float)bitm->width) + -1.0;
                minf->field_9C.y1 = 2.0 * ((float)(regions[i].y1) / (float)bitm->height) + -1.0;
                minf->field_9C.y2 = 2.0 * ((float)(regions[i].y2) / (float)bitm->height) + -1.0;
            }
            else
            {
                minf->field_9C.x2 = 0;
                minf->field_9C.y1 = 0;
                minf->field_9C.y2 = 0;
                minf->field_9C.x1 = 0;
            }
        }
        
        SDL_UnlockSurface(bitm->swTex);
    }
}

void NC_STACK_ypaworld::GameShellInitBkgMode(int mode)
{
    sb_0x4e75e8__sub1(this, mode);
    if ( mode == ENVMODE_TUTORIAL || mode == ENVMODE_SINGLEPLAY )
    {
        field_81AB = 0;
        brief.Stage = 0;
        LevelNet->field_BE38 = 0;

        sb_0x4e75e8__sub0(this);

        TOD_ID = loadTOD(this, "tod.def");

        int v6 = TOD_ID + 1;

        if ( (v6 + 2490) > 2512 )
            v6 = 0;
        writeTOD(this, "tod.def", v6);
    }
}

bool NC_STACK_ypaworld::GameShellInitBkg()
{
    NC_STACK_display *win3d = GFXEngine::GFXe.getC3D();

    ua_dRect v5;
    v5.x1 = -(screen_width >> 1);
    v5.x2 = screen_width >> 1;
    v5.y1 = -(screen_height >> 1);
    v5.y2 = screen_height >> 1;

    win3d->raster_func211(&v5);
    GameShellInitBkgMode(GameShell->envMode);
    return true;
}

size_t NC_STACK_ypaworld::ypaworld_func156(UserData *usr)
{
    if ( !one_game_res )
    {
        yw_174arg v247;
        v247.resolution = shell_default_res;
        v247.make_changes = 0;
        ypaworld_func174(&v247);
    }

    if ( !yw_LoadSet(46) )
    {
        ypa_log_out("Unable to load set for shell\n");
        return 0;
    }

    usr->field_3426 = 0;
    usr->field_D3A = 1;
    usr->lastInputEvent = 0;
    usr->p_ypaworld->icon_energy__h = 0;
    usr->field_D52 = 0;
    usr->p_ypaworld->field_81AF = NULL;
    usr->blocked = 0;

    if ( usr->default_lang_dll )
    {
        const char *v237 = usr->default_lang_dll->c_str();
        if ( ! ypaworld_func166(&v237) )
            ypa_log_out("Warning: Catalogue not found\n");
    }
    else
    {
        ypa_log_out("Warning: No Language selected, use default set\n");
    }

    ypaworld_func156__sub1(usr);

    if ( !GameShellInitBkg() )
    {
        ypa_log_out("Could not init level select stuff!\n");
        return 0;
    }

    _win3d = GFXEngine::GFXe.getC3D();

    if ( !_win3d )
    {
        ypa_log_out("No GfxObject in OpengameShell!\n");
        return 0;
    }

    displ_arg263 v233;

    v233.bitm = pointers__bitm[0];
    v233.pointer_id = 1;

    _win3d->display_func263(&v233);

    fill_videmodes_list(usr);

    wdd_func324arg v227;
    v227.name = NULL;
    v227.guid = NULL;
    v227.currr = 0;

    int v261 = 0;
    int v3 = 0;

    while ( 1 )
    {
        _win3d->windd_func324(&v227);
        if ( !v227.name )
            break;

        if ( v227.name )
        {
            if ( v227.currr & 1 )
            {
                strcpy(usr->win3d_guid, v227.guid);

                if ( !strcmp(v227.name, "software") )
                {
                    strcpy(usr->win3d_name, get_lang_string(string_pointers_p2, 2472, "2472 = Software"));
                }
                else
                {
                    strcpy(usr->win3d_name, v227.name);
                }

                v3 = v261;
            }
            v261++;
        }
    }

    if ( usr->GFX_flags & 4 )
    {
        usr->p_ypaworld->_win3d->setWDD_cursor(1);
    }
    else
    {
        usr->p_ypaworld->_win3d->setWDD_cursor(0);
    }

    LoadKeyNames();

    usr->InputConfigTitle[World::INPUT_BIND_PAUSE]       = get_lang_string(ypaworld__string_pointers, 544, "PAUSE");
    usr->InputConfigTitle[World::INPUT_BIND_QUIT]        = get_lang_string(ypaworld__string_pointers, 536, "QUIT");
    usr->InputConfigTitle[World::INPUT_BIND_DRIVE_DIR]   = get_lang_string(ypaworld__string_pointers, 500, "DRIVE DIR");
    usr->InputConfigTitle[World::INPUT_BIND_DRIVE_SPEED] = get_lang_string(ypaworld__string_pointers, 501, "DRIVE SPEED");
    usr->InputConfigTitle[World::INPUT_BIND_GUN_HEIGHT]  = get_lang_string(ypaworld__string_pointers, 511, "GUN HEIGHT");
    usr->InputConfigTitle[World::INPUT_BIND_FLY_HEIGHT]  = get_lang_string(ypaworld__string_pointers, 502, "FLY HEIGHT");
    usr->InputConfigTitle[World::INPUT_BIND_FLY_SPEED]   = get_lang_string(ypaworld__string_pointers, 503, "FLY SPEED");
    usr->InputConfigTitle[World::INPUT_BIND_FLY_DIR]     = get_lang_string(ypaworld__string_pointers, 504, "FLY DIR");
    usr->InputConfigTitle[World::INPUT_BIND_BRAKE]       = get_lang_string(ypaworld__string_pointers, 505, "STOP");
    usr->InputConfigTitle[World::INPUT_BIND_FIRE]        = get_lang_string(ypaworld__string_pointers, 506, "FIRE");
    usr->InputConfigTitle[World::INPUT_BIND_CAMFIRE]     = get_lang_string(ypaworld__string_pointers, 507, "FIRE VIEW");
    usr->InputConfigTitle[World::INPUT_BIND_GUN]         = get_lang_string(ypaworld__string_pointers, 508, "FIRE GUN");
    usr->InputConfigTitle[World::INPUT_BIND_SET_COMM]    = get_lang_string(ypaworld__string_pointers, 561, "MAKE CURRENT VEHICLE COMMANDER");
    usr->InputConfigTitle[World::INPUT_BIND_HUD]         = get_lang_string(ypaworld__string_pointers, 541, "HEADUP DISPLAY");
    usr->InputConfigTitle[World::INPUT_BIND_AUTOPILOT]   = get_lang_string(ypaworld__string_pointers, 520, "AUTOPILOT");
    usr->InputConfigTitle[World::INPUT_BIND_ORDER]       = get_lang_string(ypaworld__string_pointers, 513, "ORDER");
    usr->InputConfigTitle[World::INPUT_BIND_NEW]         = get_lang_string(ypaworld__string_pointers, 515, "NEW");
    usr->InputConfigTitle[World::INPUT_BIND_ADD]         = get_lang_string(ypaworld__string_pointers, 516, "ADD");
    usr->InputConfigTitle[World::INPUT_BIND_SQ_MANAGE]   = get_lang_string(ypaworld__string_pointers, 522, "FINDER");
    usr->InputConfigTitle[World::INPUT_BIND_AGGR_1]      = get_lang_string(ypaworld__string_pointers, 553, "AGGR: COME BACK");
    usr->InputConfigTitle[World::INPUT_BIND_AGGR_2]      = get_lang_string(ypaworld__string_pointers, 554, "AGGR: FIGHT TARGET");
    usr->InputConfigTitle[World::INPUT_BIND_AGGR_3]      = get_lang_string(ypaworld__string_pointers, 555, "AGGR: FIGHT ENEMIES TOO");
    usr->InputConfigTitle[World::INPUT_BIND_AGGR_4]      = get_lang_string(ypaworld__string_pointers, 556, "AGGR: CONQUER ALL ENEMY AREA TOO");
    usr->InputConfigTitle[World::INPUT_BIND_AGGR_5]      = get_lang_string(ypaworld__string_pointers, 557, "AGGR: GO AMOK");
    usr->InputConfigTitle[World::INPUT_BIND_MAP]         = get_lang_string(ypaworld__string_pointers, 521, "MAP");
    usr->InputConfigTitle[World::INPUT_BIND_WAPOINT]     = get_lang_string(ypaworld__string_pointers, 558, "SELECT WAYPOINT");
    usr->InputConfigTitle[World::INPUT_BIND_LANDLAYER]   = get_lang_string(ypaworld__string_pointers, 523, "LANDSCAPE");
    usr->InputConfigTitle[World::INPUT_BIND_OWNER]       = get_lang_string(ypaworld__string_pointers, 524, "OWNER");
    usr->InputConfigTitle[World::INPUT_BIND_HEIGHT]      = get_lang_string(ypaworld__string_pointers, 525, "HEIGHT");
    usr->InputConfigTitle[World::INPUT_BIND_MINIMAP]     = get_lang_string(ypaworld__string_pointers, 531, "MAP MINI");
    usr->InputConfigTitle[World::INPUT_BIND_LOCKVIEW]    = get_lang_string(ypaworld__string_pointers, 527, "LOCK VIEWER");
    usr->InputConfigTitle[World::INPUT_BIND_ZOOMIN]      = get_lang_string(ypaworld__string_pointers, 529, "ZOOM IN");
    usr->InputConfigTitle[World::INPUT_BIND_ZOOMOUT]     = get_lang_string(ypaworld__string_pointers, 530, "ZOOM OUT");
    usr->InputConfigTitle[World::INPUT_BIND_LOG_WND]     = get_lang_string(ypaworld__string_pointers, 538, "LOGWIN");
    usr->InputConfigTitle[World::INPUT_BIND_CONTROL]     = get_lang_string(ypaworld__string_pointers, 517, "CONTROL");
    usr->InputConfigTitle[World::INPUT_BIND_LAST_SEAT]   = get_lang_string(ypaworld__string_pointers, 560, "GOTO LAST OCCUPIED VEHICLE");
    usr->InputConfigTitle[World::INPUT_BIND_ATTACK]      = get_lang_string(ypaworld__string_pointers, 514, "FIGHT");
    usr->InputConfigTitle[World::INPUT_BIND_TO_HOST]     = get_lang_string(ypaworld__string_pointers, 533, "TO ROBO");
    usr->InputConfigTitle[World::INPUT_BIND_TO_COMM]     = get_lang_string(ypaworld__string_pointers, 535, "TO COMMANDER");
    usr->InputConfigTitle[World::INPUT_BIND_NEXT_UNIT]   = get_lang_string(ypaworld__string_pointers, 534, "NEXT MAN");
    usr->InputConfigTitle[World::INPUT_BIND_NEXT_COMM]   = get_lang_string(ypaworld__string_pointers, 532, "NEXT COM");
    usr->InputConfigTitle[World::INPUT_BIND_LAST_MSG]    = get_lang_string(ypaworld__string_pointers, 543, "JUMP TO LASTMSG-SENDER");
    usr->InputConfigTitle[World::INPUT_BIND_TO_ALL]      = get_lang_string(ypaworld__string_pointers, 552, "MESSAGE TO ALL PLAYERS");
    usr->InputConfigTitle[World::INPUT_BIND_HELP]        = get_lang_string(ypaworld__string_pointers, 559, "HELP");
    usr->InputConfigTitle[World::INPUT_BIND_ANALYZER]    = get_lang_string(ypaworld__string_pointers, 562, "SITUATION ANALYZER");

    int v259_4;

    if ( screen_width < 512 )
    {
        word_5A50C0 = 2;
        word_5A50C2 = 2;
        v259_4 = 8;
        word_5A50AC = 250;
        dword_5A50B2 = 210;
        word_5A50AE = 200;
        word_5A50BC = 220;
        word_5A50BA = 300;
        word_5A50BE = 270;
    }
    else
    {
        word_5A50C0 = 3;
        word_5A50C2 = 3;
        v259_4 = 16;
        word_5A50AC = 450;
        dword_5A50B2 = 380;
        word_5A50AE = 280;
        word_5A50BC = 390;
        word_5A50BA = 500;
        word_5A50BE = 480;
    }

    int v278_4 = screen_width * 0.7;
    int v285 = screen_height * 0.8;

    int v278 = (screen_width - v278_4) / 2;

    int v273 = font_default_h;
    if ( screen_width >= 512 )
        v273 += (screen_height - 384) / 2;

    int v267 = 0;
    int v269;

    if ( screen_width < 512 )
        v269 = v285 - font_default_h;
    else
        v269 = v285 - font_default_h - (screen_height - 384) / 2;

    int v270 = ( v278_4 - 2 * word_5A50C0 )/ 3;

    int v258 = v269;

    int v262 = v270;

    int v264 = word_5A50C0 + v270;
    int v274 = 2 * word_5A50C0 + 2 * v270;

    int v276 = v269;
    int v298 = (v278_4 - 2 * word_5A50C0) / 3;

    usr->titel_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)0},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)0},
        {NC_STACK_button::BTN_ATT_W, (int32_t)screen_width},
        {NC_STACK_button::BTN_ATT_H, (int32_t)screen_height} } );
    if ( !usr->titel_button )
    {
        ypa_log_out("Unable to create Titel-Button-Object\n");
        return 0;
    }

    int v70 = 0;
    NC_STACK_button::button_64_arg btn_64arg;

    btn_64arg.tileset_down = 19;
    btn_64arg.tileset_up = 18;
    btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
    btn_64arg.field_3A = 30;
    btn_64arg.xpos = screen_width * 0.3328125;
    btn_64arg.ypos = screen_height * 0.2291666666666666;
    btn_64arg.width = screen_width / 3;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 80, "GAME");
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 1251;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 1018;
    btn_64arg.up_id = 1024;
    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->titel_button->button_func64(&btn_64arg) )
    {
        btn_64arg.ypos = screen_height * 0.3083333333333334;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 81, "NETWORK");
        btn_64arg.caption2 = 0;
        btn_64arg.up_id = 1022;
        btn_64arg.pressed_id = 0;
        btn_64arg.down_id = 1251;
        btn_64arg.button_id = 1016;

        if ( usr->titel_button->button_func64(&btn_64arg) )
        {
            btn_64arg.xpos = screen_width * 0.3328125;
            btn_64arg.ypos = screen_height * 0.4333333333333334;
            btn_64arg.width = screen_width / 3;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 83, "INPUT");
            btn_64arg.caption2 = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.down_id = 1251;
            btn_64arg.button_id = 1003;
            btn_64arg.up_id = 1007;

            if ( usr->titel_button->button_func64(&btn_64arg) )
            {
                btn_64arg.ypos = screen_height * 0.5125;
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 84, "SETTINGS");
                btn_64arg.caption2 = 0;
                btn_64arg.up_id = 1005;
                btn_64arg.pressed_id = 0;
                btn_64arg.down_id = 1251;
                btn_64arg.button_id = 1004;

                if ( usr->titel_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.ypos = screen_height * 0.5916666666666667;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 85, "PLAYER");
                    btn_64arg.caption2 = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.down_id = 1251;
                    btn_64arg.up_id = 1001;
                    btn_64arg.button_id = 1001;

                    if ( usr->titel_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.xpos = screen_width * 0.890625;
                        btn_64arg.ypos = screen_height * 0.9583333333333334;
                        btn_64arg.width = screen_width * 0.1;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 86, "LOCALE");
                        btn_64arg.caption2 = 0;
                        btn_64arg.up_id = 1011;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.down_id = 1251;
                        btn_64arg.button_id = 1008;

                        if ( usr->titel_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.xpos = screen_width * 0.3328125;
                            btn_64arg.ypos = screen_height * 0.7166666666666667;
                            btn_64arg.width = screen_width / 3;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 87, "HELP");
                            btn_64arg.caption2 = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.down_id = 1251;
                            btn_64arg.button_id = 1017;
                            btn_64arg.up_id = 1025;

                            if ( usr->titel_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.ypos = screen_height * 0.7958333333333333;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 88, "QUIT");
                                btn_64arg.caption2 = 0;
                                btn_64arg.up_id = 1013;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.down_id = 1251;
                                btn_64arg.button_id = 1007;

                                if ( usr->titel_button->button_func64(&btn_64arg) )
                                    v70 = 1;
                            }
                        }
                    }
                }
            }
        }
    }

    if ( !v70 )
    {
        ypa_log_out("Unable to add button to Titel\n");
        return 0;
    }

    NC_STACK_button::button_66arg v228;

    if ( usr->lang_dlls_count <= 1 )
    {
        v228.field_4 = 0;
        v228.butID = 1008;
        usr->titel_button->button_func67(&v228);
    }

    usr->titel_button->Hide();

    dword_5A50B6_h = screen_width / 4 - 20;

    usr->sub_bar_button = Nucleus::CInit<NC_STACK_button>({ 
        {NC_STACK_button::BTN_ATT_X, (int32_t)0},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)(screen_height - font_default_h)},
        {NC_STACK_button::BTN_ATT_W, (int32_t)screen_width},
        {NC_STACK_button::BTN_ATT_H, (int32_t)font_default_h}});

    if ( !usr->sub_bar_button )
    {
        ypa_log_out("Unable to create Button-Object\n");
        return 0;
    }

    v70 = 0;

    btn_64arg.tileset_down = 19;
    btn_64arg.field_3A = 30;
    btn_64arg.ypos = 0;
    btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
    btn_64arg.tileset_up = 18;
    btn_64arg.xpos = dword_5A50B6_h + word_5A50C0;
    btn_64arg.width = dword_5A50B6_h;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 640, "REWIND");
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 1251;
    btn_64arg.pressed_id = 0;
    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
    btn_64arg.button_id = 1011;
    btn_64arg.up_id = 1016;

    if ( usr->sub_bar_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = 2 * (word_5A50C0 + dword_5A50B6_h);
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 641, "STEP FORWARD");
        btn_64arg.caption2 = 0;
        btn_64arg.down_id = 0;
        btn_64arg.up_id = 1020;
        btn_64arg.pressed_id = 1018;
        btn_64arg.button_id = 1013;

        if ( usr->sub_bar_button->button_func64(&btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 643, "START GAME");
            btn_64arg.caption2 = 0;
            btn_64arg.up_id = 1019;
            btn_64arg.pressed_id = 0;
            btn_64arg.down_id = 1251;
            btn_64arg.button_id = 1014;

            if ( usr->sub_bar_button->button_func64(&btn_64arg) )
            {
                btn_64arg.xpos = (screen_width - 3 * dword_5A50B6_h - 2 * word_5A50C0);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2422, "GOTO LOADSAVE");
                btn_64arg.caption2 = 0;
                btn_64arg.pressed_id = 0;
                btn_64arg.down_id = 1251;
                btn_64arg.button_id = 1020;
                btn_64arg.up_id = 1026;

                if ( usr->sub_bar_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.xpos = (screen_width - 2 * dword_5A50B6_h - word_5A50C0);
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 642, "LOAD GAME");
                    btn_64arg.caption2 = 0;
                    btn_64arg.up_id = 1021;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.down_id = 1251;
                    btn_64arg.button_id = 1015;

                    if ( usr->sub_bar_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.xpos = screen_width - dword_5A50B6_h;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 644, "GO BACK");
                        btn_64arg.caption2 = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.down_id = 1251;
                        btn_64arg.button_id = 1019;
                        btn_64arg.up_id = 1013;

                        if ( usr->sub_bar_button->button_func64(&btn_64arg) )
                        {
                            v70 = 1;
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add button to sub-bar\n");
        return 0;
    }

    if ( usr->field_3426 != 1 )
    {
        v228.butID = 1015;
        v228.field_4 = 0;
        usr->sub_bar_button->button_func67(&v228);
    }

    v228.field_4 = 0;
    v228.butID = 1014;
    usr->sub_bar_button->button_func67(&v228);

    v228.butID = 1013;
    usr->sub_bar_button->button_func67(&v228);

    v228.butID = 1011;
    usr->sub_bar_button->button_func67(&v228);

    usr->sub_bar_button->Hide();


    usr->confirm_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)0},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)0},
        {NC_STACK_button::BTN_ATT_W, (int32_t)screen_width},
        {NC_STACK_button::BTN_ATT_H, (int32_t)screen_height}} );
    if ( !usr->confirm_button )
    {
        ypa_log_out("Unable to create Confirm-Button-Object\n");
        return 0;
    }

    btn_64arg.tileset_up = 18;
    btn_64arg.tileset_down = 19;
    btn_64arg.field_3A = 30;
    btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
    btn_64arg.xpos = screen_width * 0.25;
    btn_64arg.ypos = screen_height * 0.53125;
    btn_64arg.width = screen_width * 0.125;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
    btn_64arg.caption2 = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
    btn_64arg.up_id = 1350;
    btn_64arg.down_id = 1251;
    btn_64arg.button_id = 1300;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->confirm_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = screen_width * 0.625;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
        btn_64arg.up_id = 1351;
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1301;
        btn_64arg.down_id = 1251;
        btn_64arg.pressed_id = 0;

        if ( usr->confirm_button->button_func64(&btn_64arg) )
        {
            btn_64arg.tileset_down = 16;
            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
            btn_64arg.tileset_up = 16;
            btn_64arg.field_3A = 16;
            btn_64arg.xpos = screen_width * 0.25;
            btn_64arg.ypos = screen_height * 0.4375;
            btn_64arg.caption = " ";
            btn_64arg.caption2 = 0;
            btn_64arg.down_id = 0;
            btn_64arg.up_id = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.flags = NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
            btn_64arg.button_id = 1302;
            btn_64arg.width = screen_width * 0.5;
            btn_64arg.txt_r = iniColors[60].r;
            btn_64arg.txt_g = iniColors[60].g;
            btn_64arg.txt_b = iniColors[60].b;

            if ( usr->confirm_button->button_func64(&btn_64arg) )
            {
                btn_64arg.button_id = 1303;
                btn_64arg.ypos = screen_height * 0.46875;
                btn_64arg.caption = " ";
                btn_64arg.flags = NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                btn_64arg.caption2 = 0;

                usr->confirm_button->button_func64(&btn_64arg);
            }
        }
    }

    v228.butID = 1300;
    usr->confirm_button->button_func67(&v228);

    v228.butID = 1301;
    usr->confirm_button->button_func67(&v228);

    usr->confirm_button->Hide();

    dword_5A50B2_h = v278_4 - font_yscrl_bkg_w;

    GuiList::tInit args;
    args.resizeable = false;
    args.numEntries = 45;
    args.shownEntries = 8;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 8;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = dword_5A50B2_h;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = false;
    args.keyboardInput = true;

    if ( !usr->input_listview.Init(this, args) )
    {
        ypa_log_out("Unable to create Input-ListView\n");
        return 0;
    }

    usr->input_listview.x = v278;
    usr->input_listview.y = v273 + (word_5A50C2 + font_default_h) * 4;

    usr->field_D5A = v278;
    usr->field_0xd5c = v273;

    
    usr->button_input_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_D5A},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_0xd5c},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_D5A)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_0xd5c)}});
    if ( !usr->button_input_button )
    {
        ypa_log_out("Unable to create Input-Button\n");
        return 0;
    }


    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.xpos = 0;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 309, "INPUT SETTINGS");
    btn_64arg.down_id = 0;
    btn_64arg.caption2 = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 1057;
    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->button_input_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 310, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.pressed_id = 0;
        btn_64arg.button_id = 1058;
        btn_64arg.txt_r = iniColors[60].r;
        btn_64arg.txt_g = iniColors[60].g;
        btn_64arg.txt_b = iniColors[60].b;

        if ( usr->button_input_button->button_func64(&btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (font_default_h + word_5A50C2);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 311, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.pressed_id = 0;
            btn_64arg.button_id = 1059;

            if ( usr->button_input_button->button_func64(&btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C2 + font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 312, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.pressed_id = 0;
                btn_64arg.button_id = 1060;

                if ( usr->button_input_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.tileset_down = 19;
                    btn_64arg.field_3A = 30;
                    btn_64arg.tileset_up = 18;
                    btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                    btn_64arg.xpos = v278_4 / 6;
                    btn_64arg.caption = "g";
                    btn_64arg.caption2 = "g";
                    btn_64arg.up_id = 1051;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.flags = 0;
                    btn_64arg.ypos = 6 * word_5A50C2 + 14 * font_default_h;
                    btn_64arg.width = v259_4;
                    btn_64arg.down_id = 1050;
                    btn_64arg.button_id = 1050;

                    if ( usr->button_input_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.tileset_down = 16;
                        btn_64arg.tileset_up = 16;
                        btn_64arg.field_3A = 16;
                        btn_64arg.xpos = (v259_4 + word_5A50C0 + v278_4 / 6);
                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                        btn_64arg.width = (v278_4 / 2 - word_5A50C0);
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 305, "JOYSTICK");
                        btn_64arg.button_id = 2;
                        btn_64arg.caption2 = 0;
                        btn_64arg.down_id = 0;
                        btn_64arg.up_id = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                        btn_64arg.txt_r = iniColors[60].r;
                        btn_64arg.txt_g = iniColors[60].g;
                        btn_64arg.txt_b = iniColors[60].b;

                        if ( usr->button_input_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.tileset_down = 19;
                            btn_64arg.field_3A = 30;
                            btn_64arg.tileset_up = 18;
                            btn_64arg.caption = "g";
                            btn_64arg.caption2 = "g";
                            btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                            btn_64arg.xpos = word_5A50C0 + (v278_4 / 2);
                            btn_64arg.width = v259_4;
                            btn_64arg.down_id = 1058;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.button_id = 1061;
                            btn_64arg.up_id = 1059;
                            btn_64arg.flags = 0;

                            if ( usr->button_input_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.tileset_down = 16;
                                btn_64arg.tileset_up = 16;
                                btn_64arg.field_3A = 16;
                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                btn_64arg.xpos = (v259_4 + (v278_4 / 2) + 2 * word_5A50C0);
                                btn_64arg.width = ((v278_4 / 2) - word_5A50C0);
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2433, "ALTERNATE JOYSTICK MODEL");
                                btn_64arg.caption2 = 0;
                                btn_64arg.down_id = 0;
                                btn_64arg.up_id = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                btn_64arg.button_id = 2;
                                btn_64arg.txt_r = iniColors[60].r;
                                btn_64arg.txt_g = iniColors[60].g;
                                btn_64arg.txt_b = iniColors[60].b;

                                if ( usr->button_input_button->button_func64(&btn_64arg) )
                                {
                                    btn_64arg.tileset_down = 19;
                                    btn_64arg.field_3A = 30;
                                    btn_64arg.tileset_up = 18;
                                    btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                    btn_64arg.xpos = v278_4 / 3;
                                    btn_64arg.caption = "g";
                                    btn_64arg.caption2 = "g";
                                    btn_64arg.up_id = 1055;
                                    btn_64arg.button_id = 1055;
                                    btn_64arg.ypos = 7 * word_5A50C2 + (15 * font_default_h);
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.width = v259_4;
                                    btn_64arg.flags = 0;
                                    btn_64arg.down_id = 1056;

                                    if ( usr->button_input_button->button_func64(&btn_64arg) )
                                    {
                                        btn_64arg.tileset_down = 16;
                                        btn_64arg.tileset_up = 16;
                                        btn_64arg.field_3A = 16;
                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                        btn_64arg.xpos = (v259_4 + (v278_4 / 3) + word_5A50C0);
                                        btn_64arg.width = v278_4 / 2;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 306, "DISABLE FORCE FEEDBACK");
                                        btn_64arg.button_id = 2;
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.down_id = 0;
                                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                        btn_64arg.up_id = 0;
                                        btn_64arg.pressed_id = 0;

                                        if ( usr->button_input_button->button_func64(&btn_64arg) )
                                        {
                                            btn_64arg.tileset_down = 19;
                                            btn_64arg.tileset_up = 18;
                                            btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                                            btn_64arg.field_3A = 30;
                                            btn_64arg.xpos = v278_4 / 6;
                                            btn_64arg.ypos = 5 * word_5A50C2 + 13 * font_default_h;
                                            btn_64arg.width = (v278_4 / 3 - word_5A50C0);
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 307, "SWITCH OFF");
                                            btn_64arg.down_id = 1251;
                                            btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.pressed_id = 0;
                                            btn_64arg.up_id = 1057;
                                            btn_64arg.button_id = 1056;
                                            btn_64arg.txt_r = iniColors[68].r;
                                            btn_64arg.txt_g = iniColors[68].g;
                                            btn_64arg.txt_b = iniColors[68].b;

                                            if ( usr->button_input_button->button_func64(&btn_64arg) )
                                            {
                                                btn_64arg.xpos = word_5A50C0 + v278_4 / 2;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 13, "RESET");
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.up_id = 1053;
                                                btn_64arg.button_id = 1053;

                                                if ( usr->button_input_button->button_func64(&btn_64arg) )
                                                {
                                                    btn_64arg.xpos = v267;
                                                    btn_64arg.ypos = v269;
                                                    btn_64arg.width = v270;
                                                    btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                                    btn_64arg.caption2 = 0;
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.button_id = 1051;
                                                    btn_64arg.up_id = 1052;
                                                    btn_64arg.down_id = 1251;

                                                    if ( usr->button_input_button->button_func64(&btn_64arg) )
                                                    {
                                                        btn_64arg.xpos = v274;
                                                        btn_64arg.ypos = v258;
                                                        btn_64arg.width = v262;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                        btn_64arg.up_id = 1250;
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.button_id = 1052;
                                                        btn_64arg.pressed_id = 0;

                                                        if ( usr->button_input_button->button_func64(&btn_64arg) )
                                                        {
                                                            btn_64arg.xpos = v264;
                                                            btn_64arg.ypos = v276;
                                                            btn_64arg.width = v298;
                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                            btn_64arg.up_id = 1054;
                                                            btn_64arg.button_id = 1054;
                                                            btn_64arg.caption2 = 0;
                                                            btn_64arg.pressed_id = 0;

                                                            if ( usr->button_input_button->button_func64(&btn_64arg) )
                                                            {
                                                                v70 = 1;
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add input-button\n");
        return 0;
    }

    usr->button_input_button->Hide();

    int v294 = v278_4 - 3 * word_5A50C0 - font_yscrl_bkg_w;
    int v94 = (v278_4 - 3 * word_5A50C0 - font_yscrl_bkg_w) * 0.6;


    args.Init();
    args.resizeable = false;
    args.numEntries = usr->video_mode_list.size();
    args.shownEntries = 4;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 4;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = v94;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = true;
    args.keyboardInput = true;

    if ( !usr->video_listvw.Init(this, args) )
    {
        ypa_log_out("Unable to create Game-Video-Menu\n");
        return 0;
    }

    args.Init();
    args.resizeable = false;
    args.numEntries = v261;
    args.shownEntries = 4;
    args.firstShownEntry = 0;
    args.selectedEntry = v3;
    args.maxShownEntries = 4;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = v94;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = true;
    args.keyboardInput = true;

    if ( !usr->d3d_listvw.Init(this, args) )
    {
        ypa_log_out("Unable to create D3D-Menu\n");
        return 0;
    }

    usr->field_13AA = v278;
    usr->field_0x13ac = v273;

    usr->video_button = Nucleus::CInit<NC_STACK_button>({
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_13AA},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_0x13ac},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_13AA)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_0x13ac)}});

    if ( !usr->video_button )
    {
        ypa_log_out("Unable to create Video-Button\n");
        return 0;
    }

    int v98 = v294 * 0.4;
    int v99 = v278 + word_5A50C0 + v98;

    usr->video_listvw.x = v99;
    usr->video_listvw.y = 6 * word_5A50C2 + 6 * font_default_h + v273;

    usr->d3d_listvw.x = v99;
    usr->d3d_listvw.y = 7 * word_5A50C2 + 7 * font_default_h + v273;

    v70 = 0;

    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
    btn_64arg.xpos = 0;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 327, "GAME SETTINGS");
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
    btn_64arg.button_id = 1168;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->video_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + font_default_h;
        btn_64arg.width = v278_4;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 328, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1169;
        btn_64arg.txt_r = iniColors[60].r;
        btn_64arg.txt_g = iniColors[60].g;
        btn_64arg.txt_b = iniColors[60].b;

        if ( usr->video_button->button_func64(&btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (font_default_h + word_5A50C2);
            btn_64arg.width = v278_4;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 329, "3");
            btn_64arg.button_id = 1170;
            btn_64arg.caption2 = 0;

            if ( usr->video_button->button_func64(&btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.width = v278_4;
                btn_64arg.ypos = 3 * (font_default_h + word_5A50C2);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 330, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1171;

                if ( usr->video_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.tileset_down = 16;
                    btn_64arg.tileset_up = 16;
                    btn_64arg.field_3A = 16;
                    btn_64arg.xpos = 0;
                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                    btn_64arg.ypos = 5 * (font_default_h + word_5A50C2);
                    btn_64arg.width = v98;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 340, "RESOLUTION SHELL");
                    btn_64arg.caption2 = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.up_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.button_id = 2;
                    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_TEXT;
                    btn_64arg.txt_r = iniColors[60].r;
                    btn_64arg.txt_g = iniColors[60].g;
                    btn_64arg.txt_b = iniColors[60].b;

                    if ( usr->video_button->button_func64(&btn_64arg) )
                    {
                        std::string name;
                        for (const auto &nod : usr->video_mode_list)
                        {
                            if (nod.sort_id == game_default_res)
                            {
                                name = nod.name;
                                break;
                            }
                        }

                        btn_64arg.tileset_down = 19;
                        btn_64arg.field_3A = 30;
                        btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                        btn_64arg.caption = name.c_str();
                        btn_64arg.caption2 = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.tileset_up = 18;
                        btn_64arg.down_id = 1100;
                        btn_64arg.button_id = 1156;
                        btn_64arg.xpos = word_5A50C0 + v294 * 0.4;
                        btn_64arg.up_id = 1101;
                        btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                        btn_64arg.width = v294 * 0.6;
                        btn_64arg.txt_r = iniColors[68].r;
                        btn_64arg.txt_g = iniColors[68].g;
                        btn_64arg.txt_b = iniColors[68].b;

                        if ( usr->video_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.tileset_down = 16;
                            btn_64arg.tileset_up = 16;
                            btn_64arg.field_3A = 16;
                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                            btn_64arg.xpos = 0;
                            btn_64arg.ypos = 2 * (3 * (word_5A50C2 + font_default_h));
                            btn_64arg.width = v294 * 0.4;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 352, "SELECT 3D DEVICE");
                            btn_64arg.caption2 = 0;
                            btn_64arg.down_id = 0;
                            btn_64arg.up_id = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_TEXT;
                            btn_64arg.button_id = 2;
                            btn_64arg.txt_r = iniColors[60].r;
                            btn_64arg.txt_g = iniColors[60].g;
                            btn_64arg.txt_b = iniColors[60].b;

                            if ( usr->video_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.width = v294 * 0.6;
                                btn_64arg.tileset_down = 19;
                                btn_64arg.field_3A = 30;
                                btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                btn_64arg.down_id = 1134;
                                btn_64arg.up_id = 1135;
                                btn_64arg.tileset_up = 18;
                                btn_64arg.caption2 = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.xpos = word_5A50C0 + v294 * 0.4;
                                btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                btn_64arg.caption = usr->win3d_name;
                                btn_64arg.button_id = 1172;
                                btn_64arg.txt_r = iniColors[68].r;
                                btn_64arg.txt_g = iniColors[68].g;
                                btn_64arg.txt_b = iniColors[68].b;

                                if ( usr->video_button->button_func64(&btn_64arg) )
                                {
                                    int v117 = dword_5A50B2 - 6 * word_5A50C0 - 2 * v259_4;

                                    btn_64arg.tileset_down = 19;
                                    btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                    btn_64arg.tileset_up = 18;
                                    btn_64arg.field_3A = 30;
                                    btn_64arg.xpos = 0;
                                    btn_64arg.caption = "g";
                                    btn_64arg.caption2 = "g";
                                    btn_64arg.down_id = 1102;
                                    btn_64arg.width = v259_4;
                                    btn_64arg.up_id = 1103;
                                    btn_64arg.ypos = 7 * (word_5A50C2 + font_default_h);
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.flags = 0;
                                    btn_64arg.button_id = 1157;

                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                    {
                                        int v120 = v117 / 2;

                                        btn_64arg.tileset_down = 16;
                                        btn_64arg.tileset_up = 16;
                                        btn_64arg.field_3A = 16;
                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                        btn_64arg.width = v120;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 344, "FAR VIEW");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.down_id = 0;
                                        btn_64arg.up_id = 0;
                                        btn_64arg.pressed_id = 0;
                                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                        btn_64arg.button_id = 2;
                                        btn_64arg.txt_r = iniColors[60].r;
                                        btn_64arg.txt_g = iniColors[60].g;
                                        btn_64arg.txt_b = iniColors[60].b;

                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                        {
                                            btn_64arg.tileset_down = 19;
                                            btn_64arg.tileset_up = 18;
                                            btn_64arg.width = v259_4;
                                            btn_64arg.caption = "g";
                                            btn_64arg.caption2 = "g";
                                            btn_64arg.field_3A = 30;
                                            btn_64arg.up_id = 1107;
                                            btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                            btn_64arg.down_id = 1106;
                                            btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;
                                            btn_64arg.pressed_id = 0;
                                            btn_64arg.flags = 0;
                                            btn_64arg.button_id = 1160;

                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                            {
                                                btn_64arg.tileset_down = 16;
                                                btn_64arg.tileset_up = 16;
                                                btn_64arg.field_3A = 16;
                                                btn_64arg.width = v120;
                                                btn_64arg.xpos = 4 * word_5A50C0 + v120 + 2 * v259_4;
                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 345, "HEAVEN");
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.down_id = 0;
                                                btn_64arg.up_id = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.button_id = 2;
                                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                {
                                                    btn_64arg.tileset_down = 19;
                                                    btn_64arg.tileset_up = 18;
                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                    btn_64arg.xpos = 0;
                                                    btn_64arg.field_3A = 30;
                                                    btn_64arg.width = v259_4;
                                                    btn_64arg.caption = "g";
                                                    btn_64arg.caption2 = "g";
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.ypos = 8 * (font_default_h + word_5A50C2);
                                                    btn_64arg.down_id = 1132;
                                                    btn_64arg.up_id = 1133;
                                                    btn_64arg.button_id = 1165;
                                                    btn_64arg.flags = 0;

                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                    {
                                                        btn_64arg.tileset_down = 16;
                                                        btn_64arg.tileset_up = 16;
                                                        btn_64arg.field_3A = 16;
                                                        btn_64arg.width = v120;
                                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 350, "SW MOUSEPOINTER");
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.down_id = 0;
                                                        btn_64arg.up_id = 0;
                                                        btn_64arg.pressed_id = 0;
                                                        btn_64arg.button_id = 2;
                                                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                        {
                                                            btn_64arg.width = v259_4;
                                                            btn_64arg.tileset_down = 19;
                                                            btn_64arg.tileset_up = 18;
                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                            btn_64arg.field_3A = 30;
                                                            btn_64arg.down_id = 1130;
                                                            btn_64arg.pressed_id = 0;
                                                            btn_64arg.flags = 0;
                                                            btn_64arg.caption = "g";
                                                            btn_64arg.caption2 = "g";
                                                            btn_64arg.up_id = 1131;
                                                            btn_64arg.button_id = 1166;
                                                            btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;

                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                            {
                                                                btn_64arg.tileset_down = 16;
                                                                btn_64arg.tileset_up = 16;
                                                                btn_64arg.field_3A = 16;
                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                btn_64arg.xpos = 4 * word_5A50C0 + v120 + 2 * v259_4;
                                                                btn_64arg.width = v120;
                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2432, "OPENGL LIKE (:-)");
                                                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                btn_64arg.caption2 = 0;
                                                                btn_64arg.down_id = 0;
                                                                btn_64arg.up_id = 0;
                                                                btn_64arg.pressed_id = 0;
                                                                btn_64arg.button_id = 2;

                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                {
                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                    btn_64arg.xpos = v259_4 + word_5A50C0;
                                                                    btn_64arg.ypos = 9 * (word_5A50C2 + font_default_h);
                                                                    btn_64arg.width = v120;
                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2431, "USE 16BIT TEXTURE");
                                                                    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                    btn_64arg.caption2 = 0;
                                                                    btn_64arg.down_id = 0;
                                                                    btn_64arg.up_id = 0;
                                                                    btn_64arg.pressed_id = 0;
                                                                    btn_64arg.button_id = 0;

                                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                                    {
                                                                        btn_64arg.width = v259_4;
                                                                        btn_64arg.tileset_down = 19;
                                                                        btn_64arg.tileset_up = 18;
                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                                        btn_64arg.pressed_id = 0;
                                                                        btn_64arg.flags = 0;
                                                                        btn_64arg.field_3A = 30;
                                                                        btn_64arg.xpos = 0;
                                                                        btn_64arg.button_id = 1150;
                                                                        btn_64arg.caption = "g";
                                                                        btn_64arg.caption2 = "g";
                                                                        btn_64arg.down_id = 1113;
                                                                        btn_64arg.up_id = 1114;

                                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                                        {
                                                                            btn_64arg.tileset_down = 16;
                                                                            btn_64arg.tileset_up = 16;
                                                                            btn_64arg.field_3A = 16;
                                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                            btn_64arg.xpos = v120 + 2 * v259_4 + 4 * word_5A50C0;
                                                                            btn_64arg.width = v120;
                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 326, "ENABLE CD AUDIO");
                                                                            btn_64arg.caption2 = 0;
                                                                            btn_64arg.down_id = 0;
                                                                            btn_64arg.up_id = 0;
                                                                            btn_64arg.pressed_id = 0;
                                                                            btn_64arg.button_id = 0;
                                                                            btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                                            {
                                                                                btn_64arg.width = v259_4;
                                                                                btn_64arg.tileset_down = 19;
                                                                                btn_64arg.tileset_up = 18;
                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                                                btn_64arg.field_3A = 30;
                                                                                btn_64arg.down_id = 1128;
                                                                                btn_64arg.pressed_id = 0;
                                                                                btn_64arg.flags = 0;
                                                                                btn_64arg.caption = "g";
                                                                                btn_64arg.caption2 = "g";
                                                                                btn_64arg.up_id = 1129;
                                                                                btn_64arg.button_id = 1164;
                                                                                btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;

                                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                {
                                                                                    btn_64arg.tileset_down = 19;
                                                                                    btn_64arg.field_3A = 30;
                                                                                    btn_64arg.tileset_up = 18;
                                                                                    btn_64arg.xpos = 0;
                                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                                                    btn_64arg.width = v259_4;
                                                                                    btn_64arg.caption = "g";
                                                                                    btn_64arg.caption2 = "g";
                                                                                    btn_64arg.ypos = 10 * (word_5A50C2 + font_default_h);
                                                                                    btn_64arg.down_id = 1126;
                                                                                    btn_64arg.pressed_id = 0;
                                                                                    btn_64arg.button_id = 1163;
                                                                                    btn_64arg.up_id = 1127;
                                                                                    btn_64arg.flags = 0;

                                                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                    {
                                                                                        btn_64arg.tileset_down = 16;
                                                                                        btn_64arg.tileset_up = 16;
                                                                                        btn_64arg.field_3A = 16;
                                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                        btn_64arg.xpos = v259_4 + word_5A50C0;
                                                                                        btn_64arg.width = v120 - v259_4;
                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 325, "ENEMY INDICATOR");
                                                                                        btn_64arg.caption2 = 0;
                                                                                        btn_64arg.down_id = 0;
                                                                                        btn_64arg.up_id = 0;
                                                                                        btn_64arg.pressed_id = 0;
                                                                                        btn_64arg.button_id = 0;
                                                                                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                        {
                                                                                            btn_64arg.tileset_down = 16;
                                                                                            btn_64arg.tileset_up = 16;
                                                                                            btn_64arg.field_3A = 16;
                                                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                            btn_64arg.xpos = v120 + 2 * v259_4 + 4 * word_5A50C0;
                                                                                            btn_64arg.width = v120;
                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 323, "INVERT LEFT-RIGHT DIVISION ");
                                                                                            btn_64arg.caption2 = 0;
                                                                                            btn_64arg.down_id = 0;
                                                                                            btn_64arg.up_id = 0;
                                                                                            btn_64arg.pressed_id = 0;
                                                                                            btn_64arg.button_id = 0;
                                                                                            btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                            {
                                                                                                btn_64arg.width = v259_4;
                                                                                                btn_64arg.tileset_down = 19;
                                                                                                btn_64arg.tileset_up = 18;
                                                                                                btn_64arg.pressed_id = 0;
                                                                                                btn_64arg.flags = 0;
                                                                                                btn_64arg.caption = "g";
                                                                                                btn_64arg.caption2 = "g";
                                                                                                btn_64arg.field_3A = 30;
                                                                                                btn_64arg.button_id = 1151;
                                                                                                btn_64arg.xpos = 3 * word_5A50C0 + v259_4 + v120;
                                                                                                btn_64arg.down_id = 1112;
                                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                                                                btn_64arg.up_id = 1111;

                                                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                {
                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                    btn_64arg.field_3A = 16;
                                                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                    btn_64arg.xpos = 0;
                                                                                                    btn_64arg.ypos = 11 * (font_default_h + word_5A50C2);
                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 343, "DESTRUCTION FX");
                                                                                                    btn_64arg.caption2 = 0;
                                                                                                    btn_64arg.down_id = 0;
                                                                                                    btn_64arg.up_id = 0;
                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                    btn_64arg.button_id = 2;
                                                                                                    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                    {
                                                                                                        NC_STACK_button::Slider v225;

                                                                                                        v225.value = 8;
                                                                                                        v225.max = 16;
                                                                                                        v225.min = 0;

                                                                                                        btn_64arg.caption2 = 0;
                                                                                                        btn_64arg.tileset_down = 18;
                                                                                                        btn_64arg.tileset_up = 18;
                                                                                                        btn_64arg.field_3A = 30;
                                                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_SLIDER;
                                                                                                        btn_64arg.pressed_id = 1110;
                                                                                                        btn_64arg.button_id = 1159;
                                                                                                        btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                        btn_64arg.caption = " ";
                                                                                                        btn_64arg.down_id = 1108;
                                                                                                        btn_64arg.flags = 0;
                                                                                                        btn_64arg.field_34 = &v225;
                                                                                                        btn_64arg.up_id = 1109;
                                                                                                        btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;

                                                                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                        {
                                                                                                            btn_64arg.tileset_down = 16;
                                                                                                            btn_64arg.tileset_up = 16;
                                                                                                            btn_64arg.field_3A = 16;
                                                                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                            btn_64arg.caption2 = 0;
                                                                                                            btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                            btn_64arg.down_id = 0;
                                                                                                            btn_64arg.up_id = 0;
                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                            btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                            btn_64arg.button_id = 1158;
                                                                                                            btn_64arg.caption = " 4";
                                                                                                            btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;

                                                                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                            {
                                                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                                btn_64arg.xpos = 0;
                                                                                                                btn_64arg.ypos = 12 * (word_5A50C2 + font_default_h);
                                                                                                                btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 321, "FX VOLUME");
                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                btn_64arg.down_id = 0;
                                                                                                                btn_64arg.up_id = 0;
                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                btn_64arg.button_id = 2;
                                                                                                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                                                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                {
                                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;

                                                                                                                    v225.min = 1;
                                                                                                                    v225.max = 127;
                                                                                                                    v225.value = 100;

                                                                                                                    btn_64arg.field_3A = 30;
                                                                                                                    btn_64arg.tileset_down = 18;
                                                                                                                    btn_64arg.tileset_up = 18;
                                                                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_SLIDER;
                                                                                                                    btn_64arg.caption2 = 0;
                                                                                                                    btn_64arg.button_id = 1152;
                                                                                                                    btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                    btn_64arg.caption = " ";
                                                                                                                    btn_64arg.down_id = 1115;
                                                                                                                    btn_64arg.up_id = 1117;
                                                                                                                    btn_64arg.field_34 = &v225;
                                                                                                                    btn_64arg.pressed_id = 1116;
                                                                                                                    btn_64arg.flags = 0;

                                                                                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                    {
                                                                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                                        btn_64arg.tileset_down = 16;
                                                                                                                        btn_64arg.tileset_up = 16;
                                                                                                                        btn_64arg.field_3A = 16;
                                                                                                                        btn_64arg.caption = "4";
                                                                                                                        btn_64arg.button_id = 1153;
                                                                                                                        btn_64arg.caption2 = 0;
                                                                                                                        btn_64arg.xpos = (2 * word_5A50C0) + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                                        btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                                        btn_64arg.down_id = 0;
                                                                                                                        btn_64arg.flags = NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                                                                                                        btn_64arg.up_id = 0;
                                                                                                                        btn_64arg.pressed_id = 0;

                                                                                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                        {
                                                                                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                                            btn_64arg.xpos = 0;
                                                                                                                            btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                            btn_64arg.ypos = 13 * (word_5A50C2 + font_default_h);
                                                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 324, "CD VOLUME");
                                                                                                                            btn_64arg.caption2 = 0;
                                                                                                                            btn_64arg.down_id = 0;
                                                                                                                            btn_64arg.up_id = 0;
                                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                                            btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                                                                            btn_64arg.button_id = 2;

                                                                                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                            {
                                                                                                                                btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.55;
                                                                                                                                v225.min = 1;
                                                                                                                                v225.max = 127;
                                                                                                                                v225.value = 100;

                                                                                                                                btn_64arg.tileset_down = 18;
                                                                                                                                btn_64arg.tileset_up = 18;
                                                                                                                                btn_64arg.up_id = 1120;
                                                                                                                                btn_64arg.field_3A = 30;
                                                                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_SLIDER;
                                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                                btn_64arg.down_id = 1118;
                                                                                                                                btn_64arg.xpos = word_5A50C0 + (dword_5A50B2 - 5 * word_5A50C0) * 0.3;
                                                                                                                                btn_64arg.caption = " ";
                                                                                                                                btn_64arg.pressed_id = 1119;
                                                                                                                                btn_64arg.field_34 = &v225;
                                                                                                                                btn_64arg.flags = 0;
                                                                                                                                btn_64arg.button_id = 1154;

                                                                                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                                {
                                                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                                                    btn_64arg.field_3A = 16;
                                                                                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                                                    btn_64arg.caption = "4";
                                                                                                                                    btn_64arg.caption2 = 0;
                                                                                                                                    btn_64arg.down_id = 0;
                                                                                                                                    btn_64arg.up_id = 0;
                                                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                                                    btn_64arg.xpos = (2 * word_5A50C0) + (dword_5A50B2 - 5 * word_5A50C0) * 0.85;
                                                                                                                                    btn_64arg.width = (dword_5A50B2 - 5 * word_5A50C0) * 0.15;
                                                                                                                                    btn_64arg.button_id = 1155;
                                                                                                                                    btn_64arg.flags = NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;

                                                                                                                                    if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                                    {
                                                                                                                                        btn_64arg.tileset_up = 18;
                                                                                                                                        btn_64arg.field_3A = 30;
                                                                                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                                                                                                                                        btn_64arg.xpos = v267;
                                                                                                                                        btn_64arg.ypos = v269;
                                                                                                                                        btn_64arg.width = v270;
                                                                                                                                        btn_64arg.tileset_down = 19;
                                                                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                                                                                                                        btn_64arg.up_id = 1124;
                                                                                                                                        btn_64arg.caption2 = 0;
                                                                                                                                        btn_64arg.down_id = 0;
                                                                                                                                        btn_64arg.pressed_id = 0;
                                                                                                                                        btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                                                                                                                        btn_64arg.button_id = 1161;
                                                                                                                                        btn_64arg.txt_r = iniColors[68].r;
                                                                                                                                        btn_64arg.txt_g = iniColors[68].g;
                                                                                                                                        btn_64arg.txt_b = iniColors[68].b;

                                                                                                                                        if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                                        {
                                                                                                                                            btn_64arg.xpos = v274;
                                                                                                                                            btn_64arg.ypos = v258;
                                                                                                                                            btn_64arg.width = v262;
                                                                                                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                                                                                                            btn_64arg.up_id = 1250;
                                                                                                                                            btn_64arg.caption2 = 0;
                                                                                                                                            btn_64arg.down_id = 0;
                                                                                                                                            btn_64arg.pressed_id = 0;
                                                                                                                                            btn_64arg.button_id = 1167;

                                                                                                                                            if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                                            {
                                                                                                                                                btn_64arg.xpos = v264;
                                                                                                                                                btn_64arg.ypos = v276;
                                                                                                                                                btn_64arg.width = v298;
                                                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                                                                                                                btn_64arg.up_id = 1125;
                                                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                                                btn_64arg.down_id = 0;
                                                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                                                btn_64arg.button_id = 1162;

                                                                                                                                                if ( usr->video_button->button_func64(&btn_64arg) )
                                                                                                                                                {
                                                                                                                                                    v70 = 1;
                                                                                                                                                }
                                                                                                                                            }
                                                                                                                                        }
                                                                                                                                    }
                                                                                                                                }
                                                                                                                            }
                                                                                                                        }
                                                                                                                    }
                                                                                                                }
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add video-button\n");
        return 0;
    }

    NC_STACK_button::button_66arg v229;
    v229.butID = 1151;
    v229.field_4 = ((usr->snd__flags2 & 1) == 0) + 1;

    usr->video_button->button_func73(&v229);


    usr->video_button->Hide();

    word_5A50B0 = v278_4;

    args.Init();
    args.resizeable = false;
    args.numEntries = usr->profiles.size();
    args.shownEntries = 10;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 10;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = v278_4;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = false;
    args.keyboardInput = true;

    if ( !usr->disk_listvw.Init(this, args) )
    {
        ypa_log_out("Unable to create disk-listview\n");
        return 0;
    }

    usr->field_0x175c = v278;
    usr->field_175E = v273;

    usr->disk_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_0x175c},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_175E},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_0x175c)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_175E)}} );

    if ( !usr->disk_button )
    {
        ypa_log_out("Unable to create disk-buttonobject\n");
        return 0;
    }

    usr->field_0x1758 = v278;
    usr->field_175A = 4 * (word_5A50C2 + font_default_h) + v273;

    usr->disk_listvw.x = usr->field_0x1758;
    usr->disk_listvw.y = usr->field_175A;

    usr->usernamedir = usr->user_name;

    usr->usernamedir_len = usr->usernamedir.size();

    std::string v223 = usr->usernamedir;

    if ( usr->field_0x1744 )
        v223 += "h";

    v70 = 0;

    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;

    btn_64arg.xpos = 0;
    btn_64arg.ypos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 367, "LOAD, CREATE OR DELETE PLAYER");
    btn_64arg.caption2 = 0;
    btn_64arg.button_id = 1108;
    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->disk_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C0 + font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 368, "2");
        btn_64arg.button_id = 1109;
        btn_64arg.caption2 = 0;
        btn_64arg.txt_r = iniColors[60].r;
        btn_64arg.txt_g = iniColors[60].g;
        btn_64arg.txt_b = iniColors[60].b;

        if ( usr->disk_button->button_func64(&btn_64arg))
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (font_default_h + word_5A50C0);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 369, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.button_id = 1110;

            if ( usr->disk_button->button_func64(&btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C0 + font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 370, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1111;

                if ( usr->disk_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.tileset_down = 17;
                    btn_64arg.tileset_up = 17;
                    btn_64arg.field_3A = 17;
                    btn_64arg.xpos = 0;
                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                    btn_64arg.width = v278_4;
                    btn_64arg.caption2 = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.up_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.caption = v223.c_str();
                    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                    btn_64arg.button_id = 1100;
                    btn_64arg.ypos = 6 * word_5A50C0 + 14 * font_default_h;

                    if ( usr->disk_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.tileset_down = 19;
                        btn_64arg.tileset_up = 18;
                        btn_64arg.field_3A = 30;
                        btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                        btn_64arg.xpos = word_5A50C0 + (v278_4 - 3 * word_5A50C0) * 0.25;
                        btn_64arg.ypos = 7 * word_5A50C0 + 15 * font_default_h;
                        btn_64arg.width = (v278_4 - 3 * word_5A50C0) * 0.25;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 360, "LOAD");
                        btn_64arg.down_id = 1251;
                        btn_64arg.up_id = 1160;
                        btn_64arg.caption2 = 0;
                        btn_64arg.pressed_id = 0;
                        btn_64arg.button_id = 1101;
                        btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                        btn_64arg.txt_r = iniColors[68].r;
                        btn_64arg.txt_g = iniColors[68].g;
                        btn_64arg.txt_b = iniColors[68].b;

                        if ( usr->disk_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.xpos = (3 * word_5A50C0) + (v278_4 - 3 * word_5A50C0) * 0.75;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 362, "DELETE");
                            btn_64arg.caption2 = 0;
                            btn_64arg.up_id = 1161;
                            btn_64arg.button_id = 1102;

                            if ( usr->disk_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.xpos = 0;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 363, "NEW GAME");
                                btn_64arg.button_id = 1103;
                                btn_64arg.caption2 = 0;
                                btn_64arg.up_id = 1162;

                                if ( usr->disk_button->button_func64(&btn_64arg) )
                                {
                                    btn_64arg.xpos = (2 * word_5A50C0) + (v278_4 - 3 * word_5A50C0) * 0.5;
                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 361, "SAVE");
                                    btn_64arg.button_id = 1104;
                                    btn_64arg.caption2 = 0;
                                    btn_64arg.up_id = 1163;

                                    if ( usr->disk_button->button_func64(&btn_64arg) )
                                    {
                                        btn_64arg.xpos = v267;
                                        btn_64arg.ypos = v269;
                                        btn_64arg.width = v270;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.down_id = 1251;
                                        btn_64arg.button_id = 1105;
                                        btn_64arg.pressed_id = 0;
                                        btn_64arg.up_id = 1164;

                                        if ( usr->disk_button->button_func64(&btn_64arg) )
                                        {
                                            btn_64arg.ypos = v258;
                                            btn_64arg.width = v262;
                                            btn_64arg.xpos = v274;
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                            btn_64arg.button_id = 1107;
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.up_id = 1250;

                                            if ( usr->disk_button->button_func64(&btn_64arg) )
                                            {
                                                btn_64arg.ypos = v276;
                                                btn_64arg.width = v298;
                                                btn_64arg.xpos = v264;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                                                btn_64arg.button_id = 1106;
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.up_id = 1165;

                                                if ( usr->disk_button->button_func64(&btn_64arg) )
                                                {
                                                    v70 = 1;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
        ypa_log_out("Unable to add button to disk-buttonobject\n");


    usr->disk_button->Hide();

    v228.field_4 = 0;
    v228.butID = 1105;
    usr->disk_button->button_func67(&v228);

    args.Init();
    args.resizeable = false;
    args.numEntries = 10;
    args.shownEntries = 10;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 10;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = v278_4 - font_yscrl_bkg_w;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = false;
    args.keyboardInput = true;


    if ( !usr->local_listvw.Init(this, args) )
    {
        ypa_log_out("Unable to create local-listview\n");
        return 0;
    }

    usr->field_19C6 = v278;
    usr->field_0x19c8 = v273;


    usr->locale_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_19C6},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_0x19c8},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_19C6)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_0x19c8)}} );

    if ( !usr->locale_button )
    {
        ypa_log_out("Unable to create locale-buttonobject\n");
        return 0;
    }

    usr->local_listvw.x = v278;
    usr->local_listvw.y = 4 * (word_5A50C2 + font_default_h) + v273;

    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
    btn_64arg.xpos = 0;
    btn_64arg.width = v278_4;
    btn_64arg.field_3A = 30;
    btn_64arg.ypos = 0;
    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 395, "SELECT NEW LANGUAGE");
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
    btn_64arg.button_id = 1253;
    btn_64arg.txt_r = iniColors[68].r;
    btn_64arg.txt_g = iniColors[68].g;
    btn_64arg.txt_b = iniColors[68].b;

    if ( usr->locale_button->button_func64(&btn_64arg) )
    {
        btn_64arg.xpos = 0;
        btn_64arg.ypos = word_5A50C2 + font_default_h;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 396, "2");
        btn_64arg.caption2 = 0;
        btn_64arg.button_id = 1254;
        btn_64arg.txt_r = iniColors[60].r;
        btn_64arg.txt_g = iniColors[60].g;
        btn_64arg.txt_b = iniColors[60].b;

        if ( usr->locale_button->button_func64(&btn_64arg) )
        {
            btn_64arg.xpos = 0;
            btn_64arg.ypos = 2 * (font_default_h + word_5A50C2);
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 397, "3");
            btn_64arg.caption2 = 0;
            btn_64arg.button_id = 1255;

            if ( usr->locale_button->button_func64(&btn_64arg) )
            {
                btn_64arg.xpos = 0;
                btn_64arg.ypos = 3 * (word_5A50C2 + font_default_h);
                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 398, "4");
                btn_64arg.caption2 = 0;
                btn_64arg.button_id = 1256;

                if ( usr->locale_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.tileset_down = 19;
                    btn_64arg.tileset_up = 18;
                    btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                    btn_64arg.field_3A = 30;
                    btn_64arg.xpos = v267 - (usr->field_19C6 - v278);
                    btn_64arg.ypos = v269 - (usr->field_0x19c8 - v273);
                    btn_64arg.width = v270;
                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2, "OK");
                    btn_64arg.caption2 = 0;
                    btn_64arg.down_id = 0;
                    btn_64arg.pressed_id = 0;
                    btn_64arg.up_id = 1300;
                    btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                    btn_64arg.button_id = 1250;
                    btn_64arg.txt_r = iniColors[68].r;
                    btn_64arg.txt_g = iniColors[68].g;
                    btn_64arg.txt_b = iniColors[68].b;

                    if ( usr->locale_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.xpos = v274 - (usr->field_19C6 - v278);
                        btn_64arg.ypos = v258 - (usr->field_0x19c8 - v273);
                        btn_64arg.width = v262;
                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                        btn_64arg.button_id = 1252;
                        btn_64arg.caption2 = 0;
                        btn_64arg.down_id = 0;
                        btn_64arg.up_id = 1250;
                        btn_64arg.pressed_id = 0;

                        if ( usr->locale_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.xpos = v264 - (usr->field_19C6 - v278);
                            btn_64arg.ypos = v276 - (usr->field_0x19c8 - v273);
                            btn_64arg.width = v298;
                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                            btn_64arg.caption2 = 0;
                            btn_64arg.down_id = 0;
                            btn_64arg.pressed_id = 0;
                            btn_64arg.up_id = 1301;
                            btn_64arg.button_id = 1251;

                            if ( usr->locale_button->button_func64(&btn_64arg) )
                                v70 = 1;
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add locale-button\n");
        return 0;
    }

    usr->locale_button->Hide();

    usr->field_19DE = 0;
    usr->field_0x19e0 = v273;

    usr->about_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_19DE},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_0x19e0},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_19DE)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_0x19e0)}} );

    if ( !usr->about_button )
    {
        ypa_log_out("Unable to create sound-buttonobject\n");
        return 0;
    }

    v70 = 0;
    btn_64arg.tileset_down = 16;
    btn_64arg.tileset_up = 16;
    btn_64arg.field_3A = 16;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
    btn_64arg.xpos = 0;
    btn_64arg.width = (screen_width - 4 * word_5A50C0);
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.button_id = 2;
    btn_64arg.ypos = word_5A50C2 + font_default_h;
    btn_64arg.flags = NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
    btn_64arg.caption = "Fuer den Kauf dieses erzgebirgischen Qualitaetsspielzeuges bedanken sich";

    if ( usr->about_button->button_func64(&btn_64arg) )
    {
        btn_64arg.ypos = 2 * (font_default_h + word_5A50C2);
        btn_64arg.caption = "Bernd Beyreuther,";

        if ( usr->about_button->button_func64(&btn_64arg) )
        {
            btn_64arg.ypos = 3 * (word_5A50C2 + font_default_h);
            btn_64arg.caption = "Andre 'Floh' Weissflog, Andreas Flemming,";

            if ( usr->about_button->button_func64(&btn_64arg) )
            {
                btn_64arg.ypos = 4 * (font_default_h + word_5A50C2);
                btn_64arg.caption = "Stefan 'Metzel Hetzel' Karau, Sylvius Lack,";

                if ( usr->about_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.ypos = 5 * (word_5A50C2 + font_default_h);
                    btn_64arg.caption = "Dietmar 'Didi' Koebelin, Nico Nitsch, Steffen Priebus, ";

                    if ( usr->about_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.ypos = 6 * (font_default_h + word_5A50C2);
                        btn_64arg.caption = "Stefan Warias, Henrik Volkening und";

                        if ( usr->about_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.ypos = 7 * (word_5A50C2 + font_default_h);
                            btn_64arg.caption = "Uta Kapp";

                            if ( usr->about_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.ypos = 8 * (font_default_h + word_5A50C2);
                                btn_64arg.caption = " ";

                                if ( usr->about_button->button_func64(&btn_64arg) )
                                {
                                    btn_64arg.ypos = 9 * (font_default_h + word_5A50C2);
                                    btn_64arg.caption = "Unser Dank gilt:";

                                    if ( usr->about_button->button_func64(&btn_64arg) )
                                    {
                                        btn_64arg.ypos = 10 * (font_default_h + word_5A50C2);
                                        btn_64arg.caption = "dem gesamten Microsoft Team, besonders";

                                        if ( usr->about_button->button_func64(&btn_64arg) )
                                        {
                                            btn_64arg.ypos = 11 * (word_5A50C2 + font_default_h);
                                            btn_64arg.caption = "Michael Lyons, Jonathan Sposato und Earnest Yuen";

                                            if ( usr->about_button->button_func64(&btn_64arg) )
                                            {
                                                btn_64arg.ypos = 12 * (font_default_h + word_5A50C2);
                                                btn_64arg.caption = "weiterhin";

                                                if ( usr->about_button->button_func64(&btn_64arg) )
                                                {
                                                    btn_64arg.ypos = 13 * (font_default_h + word_5A50C2);
                                                    btn_64arg.caption = "Robert Birker, Andre 'Goetz' Blechschmidt, Jan Blechschmidt, Stephan Bludau,";

                                                    if ( usr->about_button->button_func64(&btn_64arg) )
                                                    {
                                                        btn_64arg.ypos = 14 * (font_default_h + word_5A50C2);
                                                        btn_64arg.caption = "Andre Kunth, Markus Lorenz, Dirk Mansbart";

                                                        if ( usr->about_button->button_func64(&btn_64arg) )
                                                        {
                                                            btn_64arg.ypos = 15 * (word_5A50C2 + font_default_h);
                                                            btn_64arg.caption = "und natuerlich";

                                                            if ( usr->about_button->button_func64(&btn_64arg) )
                                                            {
                                                                btn_64arg.ypos = 16 * (font_default_h + word_5A50C2);
                                                                btn_64arg.caption = "        GoldEd - dPaint - SAS/C";

                                                                if ( usr->about_button->button_func64(&btn_64arg) )
                                                                {
                                                                    v70 = 1;
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add about-button\n");
        return 0;
    }

    usr->about_button->Hide();

    dword_5A50B6 = v278_4 - font_yscrl_bkg_w;

    args.Init();
    args.resizeable = false;
    args.numEntries = 12;
    args.shownEntries = 12;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 12;
    args.withIcon = false;
    args.entryHeight = font_default_h;
    args.entryWidth = dword_5A50B2_h;
    args.enabled = true;
    args.vborder = field_1a38;
    args.instantInput = false;
    args.keyboardInput = true;

    if ( !usr->network_listvw.Init(this, args) )
    {
        ypa_log_out("Unable to create network-listview\n");
        return 0;
    }

    usr->field_1C32 = v278;
    usr->field_0x1c34 = v273 - font_default_h;

    usr->network_button = Nucleus::CInit<NC_STACK_button>( {
        {NC_STACK_button::BTN_ATT_X, (int32_t)usr->field_1C32},
        {NC_STACK_button::BTN_ATT_Y, (int32_t)usr->field_0x1c34},
        {NC_STACK_button::BTN_ATT_W, (int32_t)(screen_width - usr->field_1C32)},
        {NC_STACK_button::BTN_ATT_H, (int32_t)(screen_height - usr->field_0x1c34)}});
    if ( !usr->network_button )
    {
        ypa_log_out("Unable to create network-buttonobject\n");
        return 0;
    }

    usr->field_1C2E = usr->field_1C32;
    usr->field_0x1c30 = 3 * (word_5A50C2 + font_default_h) + usr->field_0x1c34;

    usr->network_listvw.x = usr->field_1C2E;
    usr->network_listvw.y = usr->field_0x1c30;

    btn_64arg.tileset_down = 17;
    btn_64arg.tileset_up = 17;
    btn_64arg.field_3A = 17;
    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
    btn_64arg.xpos = 0;
    btn_64arg.caption = "???";
    btn_64arg.caption2 = 0;
    btn_64arg.down_id = 0;
    btn_64arg.up_id = 0;
    btn_64arg.pressed_id = 0;
    btn_64arg.ypos = 14 * (word_5A50C2 + font_default_h);
    btn_64arg.button_id = 1200;
    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
    btn_64arg.width = dword_5A50B6 * 0.8;
    btn_64arg.txt_r = iniColors[60].r;
    btn_64arg.txt_g = iniColors[60].g;
    btn_64arg.txt_b = iniColors[60].b;

    v70 = 0;

    if ( usr->network_button->button_func64(&btn_64arg) )
    {
        btn_64arg.tileset_down = 19;
        btn_64arg.tileset_up = 18;
        btn_64arg.xpos = word_5A50C0 + dword_5A50B6 * 0.8;
        btn_64arg.field_3A = 30;
        btn_64arg.width = dword_5A50B6 * 0.2 - word_5A50C0;
        btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 405, "SEND");
        btn_64arg.caption2 = 0;
        btn_64arg.up_id = 1210;
        btn_64arg.pressed_id = 0;
        btn_64arg.button_id = 1225;
        btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
        btn_64arg.txt_r = iniColors[68].r;
        btn_64arg.txt_g = iniColors[68].g;
        btn_64arg.txt_b = iniColors[68].b;

        if ( usr->network_button->button_func64(&btn_64arg) )
        {
            int v284 = ((dword_5A50B6 - 3 * word_5A50C0) * 0.25 - 3 * word_5A50C0) * 0.25;

            TileMap *v198 = GFXEngine::GFXe.getTileset(8);

            btn_64arg.tileset_down = 16;
            btn_64arg.tileset_up = 16;
            btn_64arg.field_3A = 16;
            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
            btn_64arg.ypos = (15 * (word_5A50C2 + font_default_h));
            btn_64arg.xpos = 0;
            btn_64arg.width = dword_5A50B6 * 0.4 - 2 * word_5A50C0;
            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 424, "SELECT YOUR RACE");
            btn_64arg.caption2 = 0;
            btn_64arg.down_id = 0;
            btn_64arg.flags = NC_STACK_button::FLAG_TEXT | NC_STACK_button::FLAG_RALIGN;
            btn_64arg.button_id = 1220;
            btn_64arg.txt_r = iniColors[60].r;
            btn_64arg.txt_g = iniColors[60].g;
            btn_64arg.txt_b = iniColors[60].b;

            if ( usr->network_button->button_func64(&btn_64arg) )
            {
                btn_64arg.tileset_down = 8;
                btn_64arg.tileset_up = 8;
                btn_64arg.field_3A = 30;
                btn_64arg.button_type = 4;
                btn_64arg.xpos += btn_64arg.width + 2 * word_5A50C0;
                btn_64arg.width = v198->map[65].w;
                btn_64arg.caption = "A";
                btn_64arg.caption2 = "B";
                btn_64arg.button_id = 1206;
                btn_64arg.down_id = 1204;
                btn_64arg.flags = 0;

                if ( usr->network_button->button_func64(&btn_64arg) )
                {
                    btn_64arg.caption = "C";
                    btn_64arg.caption2 = "D";
                    btn_64arg.down_id = 1205;
                    btn_64arg.button_id = 1207;
                    btn_64arg.xpos += v284 + word_5A50C0;

                    if ( usr->network_button->button_func64(&btn_64arg) )
                    {
                        btn_64arg.caption = "E";
                        btn_64arg.caption2 = "F";
                        btn_64arg.down_id = 1206;
                        btn_64arg.button_id = 1208;
                        btn_64arg.xpos += v284 + word_5A50C0;

                        if ( usr->network_button->button_func64(&btn_64arg) )
                        {
                            btn_64arg.caption = "G";
                            btn_64arg.caption2 = "H";
                            btn_64arg.down_id = 1207;
                            btn_64arg.button_id = 1209;
                            btn_64arg.xpos += v284 + word_5A50C0;

                            if ( usr->network_button->button_func64(&btn_64arg) )
                            {
                                btn_64arg.tileset_down = 19;
                                btn_64arg.tileset_up = 18;
                                btn_64arg.field_3A = 30;
                                btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                                btn_64arg.xpos += v284 + 2 * word_5A50C0;
                                btn_64arg.width = dword_5A50B2_h - btn_64arg.xpos;
                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 401, "BACK");
                                btn_64arg.caption2 = 0;
                                btn_64arg.pressed_id = 0;
                                btn_64arg.button_id = 1205;
                                btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                btn_64arg.up_id = 1203;
                                btn_64arg.down_id = 1251;
                                btn_64arg.txt_r = iniColors[68].r;
                                btn_64arg.txt_g = iniColors[68].g;
                                btn_64arg.txt_b = iniColors[68].b;

                                if ( usr->network_button->button_func64(&btn_64arg) )
                                {
                                    btn_64arg.tileset_down = 16;
                                    btn_64arg.xpos = 0;
                                    btn_64arg.ypos = 0;
                                    btn_64arg.tileset_up = 16;
                                    btn_64arg.field_3A = 16;
                                    btn_64arg.width = dword_5A50B2_h;
                                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 410, "SELECT PROVIDER");
                                    btn_64arg.caption2 = 0;
                                    btn_64arg.down_id = 0;
                                    btn_64arg.up_id = 0;
                                    btn_64arg.button_id = 1204;
                                    btn_64arg.pressed_id = 0;
                                    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;

                                    if ( usr->network_button->button_func64(&btn_64arg) )
                                    {
                                        btn_64arg.xpos = 0;
                                        btn_64arg.ypos = word_5A50C0 + font_default_h;
                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 425, "2");
                                        btn_64arg.caption2 = 0;
                                        btn_64arg.button_id = 1222;
                                        btn_64arg.txt_r = iniColors[60].r;
                                        btn_64arg.txt_g = iniColors[60].g;
                                        btn_64arg.txt_b = iniColors[60].b;

                                        if ( usr->network_button->button_func64(&btn_64arg) )
                                        {
                                            btn_64arg.xpos = 0;
                                            btn_64arg.ypos = 2 * (font_default_h + word_5A50C0);
                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 426, "3");
                                            btn_64arg.caption2 = 0;
                                            btn_64arg.button_id = 1223;

                                            if ( usr->network_button->button_func64(&btn_64arg) )
                                            {
                                                btn_64arg.tileset_down = 19;
                                                btn_64arg.tileset_up = 18;
                                                btn_64arg.button_type = NC_STACK_button::TYPE_BUTTON;
                                                btn_64arg.field_3A = 30;
                                                btn_64arg.xpos = dword_5A50B6 * 0.3;
                                                btn_64arg.ypos = (word_5A50C0 + font_default_h) * 15.2;
                                                btn_64arg.width = dword_5A50B6 * 0.4;
                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 402, "NEW");
                                                btn_64arg.button_id = 1202;
                                                btn_64arg.flags = NC_STACK_button::FLAG_BORDER | NC_STACK_button::FLAG_CENTER | NC_STACK_button::FLAG_TEXT;
                                                btn_64arg.down_id = 1251;
                                                btn_64arg.up_id = 1201;
                                                btn_64arg.caption2 = 0;
                                                btn_64arg.pressed_id = 0;
                                                btn_64arg.txt_r = iniColors[68].r;
                                                btn_64arg.txt_g = iniColors[68].g;
                                                btn_64arg.txt_b = iniColors[68].b;

                                                if ( usr->network_button->button_func64(&btn_64arg) )
                                                {
                                                    btn_64arg.xpos = v267;
                                                    btn_64arg.ypos = v269 + font_default_h;
                                                    btn_64arg.width = v270;
                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 400, "NEXT");
                                                    btn_64arg.caption2 = 0;
                                                    btn_64arg.pressed_id = 0;
                                                    btn_64arg.button_id = 1201;
                                                    btn_64arg.up_id = 1200;

                                                    if ( usr->network_button->button_func64(&btn_64arg) )
                                                    {
                                                        btn_64arg.xpos = v274;
                                                        btn_64arg.ypos = v258 + font_default_h;
                                                        btn_64arg.width = v262;
                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 20, "HELP");
                                                        btn_64arg.caption2 = 0;
                                                        btn_64arg.up_id = 1250;
                                                        btn_64arg.pressed_id = 0;
                                                        btn_64arg.button_id = 1218;

                                                        if ( usr->network_button->button_func64(&btn_64arg) )
                                                        {
                                                            btn_64arg.xpos = v264;
                                                            btn_64arg.ypos = v276 + font_default_h;
                                                            btn_64arg.width = v298;
                                                            btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 403, "CANCEL");
                                                            btn_64arg.caption2 = 0;
                                                            btn_64arg.up_id = 1202;
                                                            btn_64arg.pressed_id = 0;
                                                            btn_64arg.button_id = 1203;

                                                            if ( usr->network_button->button_func64(&btn_64arg) )
                                                            {
                                                                int v204;

                                                                if ( screen_width < 512 )
                                                                    v204 = 6 * v259_4;
                                                                else
                                                                    v204 = 4 * v259_4;

                                                                btn_64arg.tileset_down = 16;
                                                                btn_64arg.tileset_up = 16;
                                                                btn_64arg.field_3A = 16;
                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                btn_64arg.xpos = v204 + v259_4;
                                                                btn_64arg.ypos = 4 * (font_default_h + word_5A50C0);
                                                                btn_64arg.caption = " ";
                                                                btn_64arg.width = dword_5A50B2_h - v204 - v259_4;
                                                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                btn_64arg.caption2 = 0;
                                                                btn_64arg.down_id = 0;
                                                                btn_64arg.up_id = 0;
                                                                btn_64arg.pressed_id = 0;
                                                                btn_64arg.button_id = 1210;
                                                                btn_64arg.txt_r = iniColors[60].r;
                                                                btn_64arg.txt_g = iniColors[60].g;
                                                                btn_64arg.txt_b = iniColors[60].b;

                                                                if ( usr->network_button->button_func64(&btn_64arg) )
                                                                {
                                                                    btn_64arg.ypos = 5 * (word_5A50C0 + font_default_h);
                                                                    btn_64arg.button_id = 1211;

                                                                    if ( usr->network_button->button_func64(&btn_64arg) )
                                                                    {
                                                                        btn_64arg.ypos = 6 * (word_5A50C0 + font_default_h);
                                                                        btn_64arg.button_id = 1212;

                                                                        if ( usr->network_button->button_func64(&btn_64arg) )
                                                                        {
                                                                            btn_64arg.ypos = 7 * (word_5A50C0 + font_default_h);
                                                                            btn_64arg.button_id = 1213;

                                                                            if ( usr->network_button->button_func64(&btn_64arg) )
                                                                            {
                                                                                btn_64arg.tileset_down = 8;
                                                                                btn_64arg.tileset_up = 8;
                                                                                btn_64arg.field_3A = 8;
                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                btn_64arg.xpos = 0;
                                                                                btn_64arg.width = v204;
                                                                                btn_64arg.caption2 = 0;
                                                                                btn_64arg.down_id = 0;
                                                                                btn_64arg.up_id = 0;
                                                                                btn_64arg.ypos = 4 * (font_default_h + word_5A50C0);
                                                                                btn_64arg.pressed_id = 0;
                                                                                btn_64arg.caption = " ";
                                                                                btn_64arg.flags = 0;
                                                                                btn_64arg.button_id = 1214;
                                                                                btn_64arg.txt_r = iniColors[60].r;
                                                                                btn_64arg.txt_g = iniColors[60].g;
                                                                                btn_64arg.txt_b = iniColors[60].b;

                                                                                if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                {
                                                                                    btn_64arg.ypos = 5 * (font_default_h + word_5A50C0);
                                                                                    btn_64arg.button_id = 1215;

                                                                                    if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                    {
                                                                                        btn_64arg.ypos = 6 * (word_5A50C0 + font_default_h);
                                                                                        btn_64arg.button_id = 1216;

                                                                                        if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                        {
                                                                                            btn_64arg.ypos = 7 * (word_5A50C0 + font_default_h);
                                                                                            btn_64arg.button_id = 1217;

                                                                                            if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                            {
                                                                                                btn_64arg.tileset_down = 19;
                                                                                                btn_64arg.tileset_up = 18;
                                                                                                btn_64arg.xpos = v267;
                                                                                                btn_64arg.field_3A = 30;
                                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CHECKBX;
                                                                                                btn_64arg.width = v259_4;
                                                                                                btn_64arg.caption = "g";
                                                                                                btn_64arg.caption2 = "g";
                                                                                                btn_64arg.pressed_id = 0;
                                                                                                btn_64arg.button_id = 1219;
                                                                                                btn_64arg.ypos = v269 + font_default_h;
                                                                                                btn_64arg.down_id = 1208;
                                                                                                btn_64arg.flags = 0;
                                                                                                btn_64arg.up_id = 1209;

                                                                                                if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                                {
                                                                                                    btn_64arg.tileset_down = 16;
                                                                                                    btn_64arg.tileset_up = 16;
                                                                                                    btn_64arg.field_3A = 16;
                                                                                                    btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                    btn_64arg.xpos += word_5A50C0 + v259_4;
                                                                                                    btn_64arg.width = v270 - v259_4 - word_5A50C0;
                                                                                                    btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 409, "READY");
                                                                                                    btn_64arg.caption2 = 0;
                                                                                                    btn_64arg.down_id = 0;
                                                                                                    btn_64arg.up_id = 0;
                                                                                                    btn_64arg.pressed_id = 0;
                                                                                                    btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                                                    btn_64arg.button_id = 1221;

                                                                                                    if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                                    {
                                                                                                        btn_64arg.xpos = 0;
                                                                                                        btn_64arg.tileset_down = 16;
                                                                                                        btn_64arg.tileset_up = 16;
                                                                                                        btn_64arg.field_3A = 16;
                                                                                                        btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                        btn_64arg.ypos = 3 * (font_default_h + word_5A50C0);
                                                                                                        btn_64arg.width = dword_5A50B6 * 0.3;
                                                                                                        btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 600, "YOU PLAY");
                                                                                                        btn_64arg.caption2 = 0;
                                                                                                        btn_64arg.down_id = 0;
                                                                                                        btn_64arg.up_id = 0;
                                                                                                        btn_64arg.pressed_id = 0;
                                                                                                        btn_64arg.button_id = 1227;
                                                                                                        btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                                                        btn_64arg.txt_r = iniColors[68].r;
                                                                                                        btn_64arg.txt_g = iniColors[68].g;
                                                                                                        btn_64arg.txt_b = iniColors[68].b;

                                                                                                        if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                                        {
                                                                                                            btn_64arg.xpos = dword_5A50B6 * 0.3;
                                                                                                            btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                            btn_64arg.width = dword_5A50B6 * 0.7;
                                                                                                            btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                                                            btn_64arg.button_id = 1226;
                                                                                                            btn_64arg.caption = "...";

                                                                                                            if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                                            {
                                                                                                                btn_64arg.tileset_down = 16;
                                                                                                                btn_64arg.tileset_up = 16;
                                                                                                                btn_64arg.field_3A = 16;
                                                                                                                btn_64arg.button_type = NC_STACK_button::TYPE_CAPTION;
                                                                                                                btn_64arg.xpos = 0;
                                                                                                                btn_64arg.ypos = (14 * (word_5A50C2 + font_default_h));
                                                                                                                btn_64arg.width = dword_5A50B2_h;
                                                                                                                btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 2402, "PRESS SPACEBAR TO UPDATE SESSION LIST");
                                                                                                                btn_64arg.caption2 = 0;
                                                                                                                btn_64arg.down_id = 0;
                                                                                                                btn_64arg.up_id = 0;
                                                                                                                btn_64arg.pressed_id = 0;
                                                                                                                btn_64arg.button_id = 1228;
                                                                                                                btn_64arg.flags = NC_STACK_button::FLAG_TEXT;
                                                                                                                btn_64arg.txt_r = iniColors[60].r;
                                                                                                                btn_64arg.txt_g = iniColors[60].g;
                                                                                                                btn_64arg.txt_b = iniColors[60].b;

                                                                                                                if ( usr->network_button->button_func64(&btn_64arg) )
                                                                                                                    v70 = 1;
                                                                                                            }
                                                                                                        }
                                                                                                    }
                                                                                                }
                                                                                            }
                                                                                        }
                                                                                    }
                                                                                }
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if ( !v70 )
    {
        ypa_log_out("Unable to add network-button\n");
        return 0;
    }

    v228.butID = 1210;
    v228.field_4 = 0;
    usr->network_button->button_func67(&v228);

    v228.butID = 1211;
    usr->network_button->button_func67(&v228);

    v228.butID = 1212;
    usr->network_button->button_func67(&v228);

    v228.butID = 1213;
    usr->network_button->button_func67(&v228);

    v228.butID = 1214;
    usr->network_button->button_func67(&v228);

    v228.butID = 1215;
    usr->network_button->button_func67(&v228);

    v228.butID = 1216;
    usr->network_button->button_func67(&v228);

    v228.butID = 1217;
    usr->network_button->button_func67(&v228);

    usr->network_button->Hide();

    switch (usr->envMode)
    {
    default:
        usr->titel_button->Show();
        break;
    case ENVMODE_TUTORIAL:
    case ENVMODE_SINGLEPLAY:
        usr->sub_bar_button->Show();

        if ( usr->field_0xc )
        {
            NC_STACK_button::button_66arg v231;
            v231.field_4 = 0;
            v231.butID = 1014;

            usr->sub_bar_button->button_func67(&v231);

            v231.butID = 1013;
            usr->sub_bar_button->button_func67(&v231);
        }
        break;
    case ENVMODE_NETPLAY:
        usr->network_button->Show();
        break;
    }


    ypaworld_func167(usr);

    SFXEngine::SFXe.startSound(&usr->samples1_info, 6);

    usr->field_0x0 = 1;

    usr->field_0x4 = 1;
    if ( usr->remoteMode )
    {
        usr->GameShellUiOpenNetwork();
        usr->p_ypaworld->isNetGame = 1;
        usr->FreeFraction = 14;
        usr->SelectedFraction = 1;
    }
    else
    {
        usr->yw_netcleanup();
        usr->netSelMode = UserData::NETSCREEN_MODE_SELECT;
    }

    usr->netSel = -1;

    if ( usr->p_ypaworld->field_73CE & World::PREF_CDMUSICDISABLE )
    {
        SFXEngine::SFXe.StopMusicTrack();
        SFXEngine::SFXe.SetMusicTrack(usr->shelltrack, usr->shelltrack__adv.min_delay, usr->shelltrack__adv.max_delay);
        SFXEngine::SFXe.PlayMusicTrack();
    }

    return 1;
}


void ypaworld_func157__sub0(NC_STACK_ypaworld *yw)
{
    sub_4E6FEC(yw);

    if ( yw->GameShell->envMode == ENVMODE_TUTORIAL || yw->GameShell->envMode == ENVMODE_SINGLEPLAY )
    {
        if ( yw->_levelInfo->State == 5 )
        {
            sub_4EAC80(yw);
        }
        else if ( yw->_levelInfo->State == 9 )
        {
            yw_freeDebrief(yw);
        }
    }
}

void NC_STACK_ypaworld::ypaworld_func157(UserData *usr)
{
    if ( usr->field_0x0 )
    {
        if ( usr->confirm_button )
        {
            usr->confirm_button->Hide();
            delete_class_obj(usr->confirm_button);
        }
        usr->confirm_button = NULL;

        if ( usr->sub_bar_button )
        {
            usr->sub_bar_button->Hide();
            delete_class_obj(usr->sub_bar_button);
        }
        usr->sub_bar_button = NULL;

        if ( usr->titel_button )
        {
            usr->titel_button->Hide();
            delete_class_obj(usr->titel_button);
        }
        usr->titel_button = 0;

        if ( usr->button_input_button )
        {
            if ( usr->input_listview.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->input_listview );
            usr->input_listview.Free();

            usr->button_input_button->Hide();
            delete_class_obj(usr->button_input_button);
            usr->button_input_button = NULL;
        }

        if ( usr->video_button )
        {
            if ( usr->video_listvw.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->video_listvw );
            usr->video_listvw.Free();

            if ( usr->d3d_listvw.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->d3d_listvw );
            usr->d3d_listvw.Free();

            usr->video_button->Hide();
            delete_class_obj(usr->video_button);
            usr->video_button = NULL;
        }

        if ( usr->disk_button )
        {
            if ( usr->disk_listvw.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->disk_listvw );
            usr->disk_listvw.Free();

            usr->disk_button->Hide();
            delete_class_obj(usr->disk_button);
            usr->disk_button = NULL;
        }

        if ( usr->locale_button )
        {
            if ( usr->local_listvw.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->local_listvw );
            usr->local_listvw.Free();

            usr->locale_button->Hide();
            delete_class_obj(usr->locale_button);
            usr->locale_button = NULL;
        }

        if ( usr->about_button )
        {
            usr->about_button->Hide();
            delete_class_obj(usr->about_button);
            usr->about_button = NULL;
        }

        if ( usr->network_button )
        {
            if ( usr->network_listvw.IsOpen() )
                usr->p_YW->GuiWinClose( &usr->network_listvw );
            usr->network_listvw.Free();

            usr->network_button->Hide();
            delete_class_obj(usr->network_button);
            usr->network_button = NULL;
        }

        SFXEngine::SFXe.sub_424CC8();

        ypaworld_func157__sub0(this);

        sb_0x44ac24(this);

        //nullsub_7();

        usr->field_0x0 = 0;
    }
}

//Draw bkg or briefing
void NC_STACK_ypaworld::GameShellBkgProcess()
{
    if ( GameShell->envModeChanged )
        GameShellInitBkgMode(GameShell->envMode);

    switch(GameShell->envMode)
    {
    case ENVMODE_TUTORIAL:
    case ENVMODE_SINGLEPLAY:
        ypaworld_func158__sub4__sub1();
        break;

    case ENVMODE_TITLE:
        GameShellBlitBkg(LevelNet->ilbm_menu_map);
        break;

    default:
        GameShellBlitBkg(LevelNet->ilbm_rollover_map);
        break;
    }
}

void draw_tooltip(NC_STACK_ypaworld *yw)
{
    if ( yw->_mouseGrabbed || (yw->field_17c4 && !yw->field_1b1c) )
    {
        const char *tooltip = get_lang_string(yw->string_pointers_p2, yw->field_17c4 + 800, yw->tooltips[ yw->field_17c4 ]);
        int v15 = -(yw->font_default_h + yw->icon0___h + yw->font_default_h / 4);
        std::string v2;
        
        if ( yw->field_17c8 != -1 )
        {
            NC_STACK_input *inpt = INPe.getPInput();

            int16_t keycode = inpt->GetHotKey(yw->field_17c8);

            if ( keycode != Input::KC_NONE )
            {
                if ( yw->GameShell && !NC_STACK_input::KeyTitle.at(keycode).empty())
                {
                    v2 = fmt::sprintf("[%s]", NC_STACK_input::KeyTitle.at(keycode));
                    v15 = -(yw->icon0___h + 2 * yw->font_default_h + yw->font_default_h / 4);
                }
            }
        }

        char buf[1024];

        char *pos = buf;

        FontUA::select_tileset(&pos, 15);
        FontUA::set_xpos(&pos, 0);
        FontUA::set_ypos(&pos, v15);

        if ( !v2.empty() )
        {
            FontUA::set_txtColor(&pos, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);

            pos = FontUA::FormateCenteredSkipableItem(yw->tiles[15], pos, v2.c_str(), yw->screen_width);

            FontUA::next_line(&pos);
        }

        FontUA::set_txtColor(&pos, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

        pos = FontUA::FormateCenteredSkipableItem(yw->tiles[15], pos, tooltip, yw->screen_width);

        FontUA::set_end(&pos);

        w3d_a209 v10;

        v10.cmdbuf = buf;
        v10.includ = 0;

        yw->_win3d->raster_func209(&v10);
    }
    yw->field_17c8 = -1;
    yw->field_17c4 = 0;
}

//Make screenshot
void sub_4476AC(NC_STACK_ypaworld *yw)
{
    NC_STACK_win3d *w3d = GFXEngine::GFXe.getC3D();

    w3d->SaveScreenshot(fmt::sprintf("env:snaps/f_%04d", yw->field_2424));
    
    yw->field_2424++;
}

//FIXME
int PrevMouseX = 0;
int PrevMouseY = 0;

bool NC_STACK_ypaworld::IsAnyInput(InputState *struc)
{
    bool click = (struc->ClickInf.flag & ~ClickBoxInf::FLAG_OK);
    bool mousemove = struc->ClickInf.move.ScreenPos.x != PrevMouseX || PrevMouseY != struc->ClickInf.move.ScreenPos.y;

    PrevMouseX = struc->ClickInf.move.ScreenPos.x;
    PrevMouseY = struc->ClickInf.move.ScreenPos.y;

    return struc->KbdLastDown != Input::KC_NONE || struc->KbdLastHit != Input::KC_NONE || struc->HotKeyID >= 0 || click || mousemove;
}

void NC_STACK_ypaworld::ypaworld_func158(UserData *usr)
{
    usr->envAction.action = EnvAction::ACTION_NONE;

    SFXEngine::SFXe.sub_423EFC(usr->frameTime, vec3d(0.0), vec3d(0.0), mat3x3::Ident());

    _win3d = GFXEngine::GFXe.getC3D();

    _win3d->BeginFrame();

    int oldMode = usr->envMode;
    usr->envModeChanged = false;

    usr->GameShellUiHandleInput();

    if ( oldMode != usr->envMode )
        usr->envModeChanged = true;

    GameShellBkgProcess();

    draw_tooltip(this);

    ypaworld_func158__sub3(this, usr);

    if ( isNetGame )
    {
        field_7586 -= usr->frameTime;
        if ( field_7586 <= 0 )
        {
            windp_arg82 arg82;
            arg82.senderFlags = 1;
            arg82.senderID = usr->callSIGN.c_str();
            arg82.receiverFlags = 2;
            arg82.receiverID = 0;
            arg82.guarant = 1;

            windp->FlushBuffer(arg82);

            field_7586 = 100;
        }
    }

    SFXEngine::SFXe.sb_0x4242e0(&usr->samples1_info);
    SFXEngine::SFXe.sb_0x4242e0(&usr->samples2_info);
    SFXEngine::SFXe.sb_0x4242e0(&usr->field_782);

    SFXEngine::SFXe.sb_0x424c74();

//  if ( usr->field_0x4 )
//    nullsub_7();

    _win3d->EndFrame();

    if ( usr->field_0x4 )
    {
        usr->field_0x4 = 0;
//    nullsub_7();
    }

    if ( sub_449678(usr->_input, Input::KC_NUMMUL) )
        sub_4476AC(this);

    if ( usr->netSelMode == UserData::NETSCREEN_INSESSION )
    {
        yw_CheckCRCs(this);
        usr->yw_CheckCDs();
    }

    if ( IsAnyInput(usr->_input) )
        usr->lastInputEvent = usr->glblTime;

    if ( (usr->glblTime - usr->lastInputEvent) > usr->WaitForDemo && usr->envMode == ENVMODE_TITLE )
        usr->envAction.action = EnvAction::ACTION_DEMO;

    usr->field_0xc = 0;

    if ( field_81AF )
    {
        const char *v22 = field_81AF;
        ypaworld_func185(&v22);

        field_81AF = NULL;
    }
}


void NC_STACK_ypaworld::ypaworld_func159(yw_arg159 *arg)
{
    ypaworld_func159__real(this, arg);
}


void NC_STACK_ypaworld::ypaworld_func160(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func160");
}

// Load Level
size_t NC_STACK_ypaworld::ypaworld_func161(yw_arg161 *arg)
{
    int ok = 0;
    LevelDesc mapp;

    if ( LevelCommonLoader(&mapp, arg->lvlID, arg->field_4) )
    {
        if ( LoadTypeMap(mapp.TypStr) )
        {
            if ( LoadOwnerMap( mapp.OwnStr) )
            {
                if ( LoadHightMap(mapp.HgtStr.c_str()) )
                {
                    if ( yw_createRobos(mapp.Robos) )
                    {
                        if ( LoadBlgMap(mapp.BlgStr) )
                        {
                            if ( _levelInfo->Mode != 1 )
                            {
                                yw_InitSquads(mapp.Squads);
                                InitBuddies();

                                for (int yy = 0; yy < _mapHeight; yy++)
                                {
                                    for (int xx = 0; xx < _mapWidth; xx++)
                                    {
                                        cellArea *cell = &_cells[xx + yy * _mapWidth];
                                        CellCheckHealth(cell, xx, yy, 255, NULL);
                                    }
                                }

                                yw_InitTechUpgradeBuildings();
                                InitGates();
                                InitSuperItems();
                                UpdatePowerEnergy();
                            }

                            if ( sb_0x451034(this) )
                                ok = 1;
                        }
                    }
                }
            }
        }
    }

    if ( !ok )
    {
        printf("Load level not OK\n");
        DeleteLevel();
    }

    return ok;
}


size_t NC_STACK_ypaworld::ypaworld_func162(const char *fname)
{
    replayer = recorder_allocate();

    if ( !replayer )
        return 0;

    recorder *repl = replayer;

    strcpy(repl->filename, fname);
    timeStamp = 0;

    if ( !recorder_open_replay(repl) )
        return 0;

    while ( repl->mfile->parse() != IFFile::IFF_ERR_EOC )
    {
        IFFile::Context *v13 = repl->mfile->getCurrentChunk();

        if ( v13->TAG == TAG_SINF )
        {
            repl->mfile->readU16L(repl->seqn);
            repl->mfile->readU16L(repl->level_id);
            repl->mfile->parse();
        }
        else if ( v13->TAG != TAG_FORM || v13->TAG_EXTENSION != TAG_FRAM )
        {
            repl->mfile->skipChunk();
        }
        else
        {
            repl->field_74++;
            repl->mfile->skipChunk();
        }
    }

    if ( repl->mfile )
    {
        delete repl->mfile;
        repl->mfile = NULL;
    }

    yw_arg161 arg161;
    arg161.field_4 = 1;
    arg161.lvlID = repl->level_id;

    if ( !ypaworld_func161(&arg161) )
        return 0;

    if ( !recorder_create_camera() )
    {
        ypa_log_out("PLAYER ERROR: could not create virtual camera!\n");
        ypaworld_func164();

        return 0;
    }

    repl->field_44 = vec3d(0.0, 0.0, 0.0);
    repl->rotation_matrix = mat3x3::Ident();

    if ( !recorder_go_to_frame(repl, 0) )
    {
        ypa_log_out("PLAYER ERROR: could not position on 1st frame!\n");
        ypaworld_func164();
        return 0;
    }

    yw_arg165 arg165;

    arg165.field_0 = 2;
    arg165.frame = 0;
    ypaworld_func165(&arg165);

    arg165.field_0 = 20;
    arg165.frame = 0;
    ypaworld_func165(&arg165);

    return 1;
}


void NC_STACK_ypaworld::ypaworld_func163(base_64arg *arg)
{
    recorder *repl = replayer;
    uint32_t v33 = profiler_begin();

    _win3d = GFXEngine::GFXe.getC3D();

    b64_parms = arg;
    field_161c++;
    field_1b24.user_action = 0;
    field_1b24.gTime = arg->TimeStamp;
    field_1b24.frameTime = arg->DTime;
    field_1b24.units_count = 0;
    field_1b24.inpt = arg->field_8;
    field_1B6E = 1024 / arg->DTime;

    p_1_grp[0][0] = field_1B6E;

    _win3d->BeginFrame();

    _win3d->setRSTR_BGpen(0);

    _win3d->raster_func192(NULL);

    sub_4C40AC();

    hudi.field_0 = 0;
    hudi.field_4 = 0;

    if ( repl->field_7C != 1 )
        ypaworld_func163__sub1(repl, arg->DTime);

    CameraPrepareRender(repl, UserUnit, arg->field_8);

    vec3d a3a = UserUnit->_fly_dir * UserUnit->_fly_dir_length;

    SFXEngine::SFXe.sub_423EFC(arg->DTime, UserUnit->_position, a3a, UserUnit->_rotation);

    for ( NC_STACK_ypabact* &bct : UserUnit->_kidList )
    {
        bct->_tForm.locPos = bct->_position;

        bct->_tForm.locSclRot = bct->_rotation.Transpose();

        bct->_soundcarrier.field_0 = bct->_position;

        bct->_soundcarrier.field_C = bct->_fly_dir * bct->_fly_dir_length;

        SFXEngine::SFXe.sb_0x4242e0(&bct->_soundcarrier);
    }

    const mat3x3 &v25 = SFXEngine::SFXe.sb_0x424c74();
    TFEngine::TForm3D *v26 = TFEngine::Engine.GetViewPoint();

    v26->locSclRot = v25 * v26->locSclRot;

    uint32_t v28 = profiler_begin();

    sb_0x4d7c08(this, arg, 0);

    debug_info_draw(arg->field_8);

    _win3d->EndFrame();

    p_1_grp[0][5] = profiler_end(v28);

    sb_0x447720(this, arg->field_8);

    p_1_grp[0][1] = profiler_end(v33);
    p_1_grp[0][7] = field_1b6c;

    sub_44A094(this);
}



void NC_STACK_ypaworld::ypaworld_func164()
{
    if ( replayer )
    {
        if ( replayer->mfile )
        {
            delete replayer->mfile;

            replayer->mfile = NULL;
        }

        DeleteLevel();

        if ( replayer->oinf )
            nc_FreeMem(replayer->oinf);

        if ( replayer->sound_status )
            nc_FreeMem(replayer->sound_status);

        if ( replayer->field_20 )
            nc_FreeMem(replayer->field_20);

        if ( replayer->ainf )
            nc_FreeMem(replayer->ainf);

        nc_FreeMem(replayer);

        replayer = NULL;
    }
}


void NC_STACK_ypaworld::ypaworld_func165(yw_arg165 *arg)
{
    recorder *repl = replayer;

    if ( (repl->field_80 == 18 || repl->field_80 == 19 || repl->field_80 == 20) && (arg->field_0 == 16 || arg->field_0 == 17) )
    {
        repl->field_44 = UserUnit->_position;
        repl->rotation_matrix = UserUnit->_rotation;
    }

    if ( arg->field_0 == 1 || arg->field_0 == 2 )
    {
        repl->field_7C = arg->field_0;
    }
    else if ( arg->field_0 == 3 )
    {
        recorder_go_to_frame(repl, arg->frame);
    }
    else if ( arg->field_0 == 4 )
    {
        recorder_go_to_frame(repl, repl->frame_id + 1);
    }
    else if ( arg->field_0 == 5 )
    {
        recorder_go_to_frame(repl, repl->frame_id - 1);
    }
    else if ( arg->field_0 == 7 )
    {
        recorder_go_to_frame(repl, repl->frame_id + arg->frame);
    }
    else if ( arg->field_0 == 16 || arg->field_0 == 17 )
    {
        repl->field_84 = 0;
        repl->field_80 = arg->field_0;
    }
    else if ( arg->field_0 == 18 || arg->field_0 == 19 || arg->field_0 == 20 )
    {
        repl->field_80 = arg->field_0;
        repl->field_84 = arg->frame;

        repl->field_44 = vec3d(0.0, 0.0, 0.0);
        repl->rotation_matrix = mat3x3::Ident();
    }
    else
    {
        repl->field_7C = 1;
    }
}


char * sb_0x471428__sub0(char *a1, const char *a2)
{
    const char *tmp = a2;

    while ( *tmp )
    {
        if (tmp[0] == '\\')
        {
            if (tmp[1] == '\\')
            {
                *a1 = '\n';
                tmp++;
            }
        }
        else
            *a1 = *tmp;

        a1++;
        tmp++;
    }
    *a1 = 0;
    a1++;
    return a1;
}

int simple_lang_parser(NC_STACK_ypaworld *yw, const std::string &filename)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "r");
    if ( !fil )
        return 0;

    char buf[4096];

    bool multiline = false;
    int strid = -1;

    while (fil->gets(buf, 4096))
    {
        char *line_end = strpbrk(buf, "\n\r");
        if ( line_end )
            *line_end = 0;

        int line_sz = strlen(buf);

        if (multiline && strid != -1)
        {
            multiline = false;

            if ( buf[line_sz - 1] == '\\' )
            {
                buf[line_sz - 1] = '\n';
                multiline = true;
            }

            if (yw->very_big_array__p_begin + line_sz < yw->lang_strings__end)
            {
                yw->very_big_array__p_begin = sb_0x471428__sub0(yw->very_big_array__p_begin - 1, buf);
            }
            else
            {
                ypa_log_out("Locale parser: buffer overflow at id [%d].\n", strid);
            }
        }
        else
        {
            strid = -1;
            multiline = false;

            int phase = 0;

            char *tmp1 = buf;
            char *tmp2 = NULL;

            while( phase >= 0 && phase < 4  && *tmp1 )
            {
                switch(phase)
                {
                case 0:
                    if (strchr(" \t", *tmp1) == NULL )
                    {
                        tmp2 = tmp1;
                        phase++;
                    }
                    break;

                case 1:
                    if (strchr("= \t", *tmp1) != NULL )
                    {
                        *tmp1 = 0;
                        strid = strtol(tmp2, NULL, 0);

                        tmp2 = NULL;

                        if (strid >= 0 && strid < 2600 )
                            phase++;
                        else
                            phase = -1;
                    }
                    break;

                case 2:
                    if ( *tmp1 == '=' && tmp1[1] != 0 )
                    {
                        tmp2 = tmp1 + 1;
                        phase++;
                    }
                    break;

                case 3:
                    if ( strchr(" \t", *tmp1) == NULL )
                    {
                        tmp2 = tmp1;
                        phase++;
                    }
                    break;

                default:
                    break;
                }

                tmp1++;
            }

            if ( phase >= 2 )
            {
                if (tmp2)
                {
                    if (yw->very_big_array__p_begin + (line_sz - (tmp2 - buf)) + 1 < yw->lang_strings__end)
                    {
                        if ( buf[line_sz - 1] == '\\' )
                        {
                            buf[line_sz - 1] = '\n';
                            multiline = true;
                        }

                        yw->string_pointers[strid] = yw->very_big_array__p_begin;
                        yw->very_big_array__p_begin = sb_0x471428__sub0(yw->very_big_array__p_begin, tmp2);
                    }
                    else
                        ypa_log_out("Locale parser: buffer overflow at id [%d].\n", strid);
                }
                else
                {
                    yw->string_pointers[strid] = yw->very_big_array__p_begin;
                    yw->very_big_array__p_begin = sb_0x471428__sub0(yw->very_big_array__p_begin, " ");
                }
            }
        }
    }

    delete fil;
    return 1;
}

int load_lang_lng(NC_STACK_ypaworld *yw, const std::string &lang)
{
    std::string buf = fmt::sprintf("locale:%s.lng", lang);

    if ( !simple_lang_parser(yw, buf) )
    {
        ypa_log_out("ERROR: Could not load language file '%s'!!!\n", buf);
        return 0;
    }

    return 1;
}

size_t NC_STACK_ypaworld::ypaworld_func166(const char **langname)
{
    sub_4711E0(this);
    strcpy(lang_name, *langname);

    int v19 = 0;
//// We do not use this dll-garbage
//  char buf[256];
//  char LibFileName[256];
//  sprintf(buf, "locale/%s.dll", yw->lang_name);
//  sub_412810(buf, LibFileName, 256);
//  if ( load_lang_dll(LibFileName, yw->lang_strings, yw->lang_strings__end, yw->string_pointers, 2600) )
//    v19 = 1;

    if ( v19 || load_lang_lng(this, lang_name) )
    {
        NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

        const char *v11 = NULL;

        if ( screen_width >= 512 )
            v11 = get_lang_string(string_pointers_p2, 15, "MS Sans Serif,12,400,0");
        else
            v11 = get_lang_string(string_pointers_p2, 16, "Arial,8,400,0");

        win3d->LoadFontByDescr(v11);
        Gui::UA::LoadFont(v11);

        return 1;
    }

    sub_4711E0(this);
    return 0;
}


void ypaworld_func167__sub0(UserData *usr)
{
    NC_STACK_button::button_66arg v9;
    v9.butID = 1050;
    v9.field_4 = ((usr->p_ypaworld->field_73CE & 4) != 0) + 1;

    usr->button_input_button->button_func73(&v9);

    v9.butID = 1061;
    v9.field_4 = (usr->inp_altjoystick == false) + 1;
    usr->button_input_button->button_func73(&v9);

    v9.butID = 1055;
    v9.field_4 = ((usr->p_ypaworld->field_73CE & 8) != 0) + 1;
    usr->button_input_button->button_func73(&v9);

    if ( usr->field_D36 )
    {
        int v7 = usr->field_D36 - 1;
        int v8 = usr->input_listview.maxShownEntries + usr->input_listview.firstShownEntries;

        if ( v7 >= usr->input_listview.firstShownEntries && v7 < v8 )
        {
            if ( v8 > usr->input_listview.numEntries )
                usr->input_listview.firstShownEntries = usr->input_listview.numEntries - usr->input_listview.maxShownEntries;

            if ( usr->input_listview.firstShownEntries < 0 )
                usr->input_listview.firstShownEntries = 0;
        }
        else if ( usr->input_listview.numEntries - v7 <= usr->input_listview.maxShownEntries )
        {
            usr->input_listview.firstShownEntries = usr->input_listview.numEntries - usr->input_listview.maxShownEntries;
        }
        else
        {
            usr->input_listview.firstShownEntries = v7;
        }
    }
}

// Update menu values
void NC_STACK_ypaworld::ypaworld_func167(UserData *usr)
{
    if ( usr->field_1612 )
    {
        usr->disk_listvw.PosOnSelected(usr->field_1612 - 1);

        NC_STACK_button::button_66arg v18;
        v18.field_4 = 1;
        v18.butID = 1101;
        usr->disk_button->button_func66(&v18);

        v18.butID = 1102;
        usr->disk_button->button_func66(&v18);

        v18.butID = 1103;
        usr->disk_button->button_func66(&v18);
    }
    else
    {
        NC_STACK_button::button_66arg v18;
        v18.field_4 = 1;
        v18.butID = 1101;
        usr->disk_button->button_func67(&v18);

        v18.butID = 1102;
        usr->disk_button->button_func67(&v18);

        v18.butID = 1103;
        usr->disk_button->button_func67(&v18);
    }

    NC_STACK_button::button_66arg v16;
    v16.butID = 1151;
    v16.field_4 = ((usr->snd__flags2 & 1) == 0) + 1;

    usr->video_button->button_func73(&v16);


    v16.butID = 1150;
    v16.field_4 = ((usr->GFX_flags & 0x10) == 0) + 1;
    usr->video_button->button_func73(&v16);

    v16.field_4 = ((usr->snd__flags2 & 0x10) == 0) + 1;
    v16.butID = 1164;
    usr->video_button->button_func73(&v16);

    NC_STACK_button::Slider *tmp = usr->video_button->button_func74(1152);
    tmp->value = usr->snd__volume;

    usr->video_button->button_func75(1152);

    tmp = usr->video_button->button_func74(1154);
    tmp->value = usr->snd__cdvolume;

    usr->video_button->button_func75(1154);

    v16.butID = 1163;
    v16.field_4 = (usr->enemyindicator == 0) + 1;
    usr->video_button->button_func73(&v16);

    v16.butID = 1157;
    v16.field_4 = ((usr->GFX_flags & 1) == 0) + 1;
    usr->video_button->button_func73(&v16);

    v16.field_4 = ((usr->GFX_flags & 2) == 0) + 1;
    v16.butID = 1160;
    usr->video_button->button_func73(&v16);

    v16.butID = 1165;
    v16.field_4 = ((usr->GFX_flags & 4) == 0) + 1;
    usr->video_button->button_func73(&v16);

    v16.butID = 1166;
    v16.field_4 = ((usr->GFX_flags & 8) == 0) + 1;
    usr->video_button->button_func73(&v16);

    std::string name;
    for (const auto &nod : usr->video_mode_list)
    {
        if (usr->p_ypaworld->game_default_res == nod.sort_id )
        {
            name = nod.name;
            break;
        }
    }

    usr->video_button->button_func71(1156, name);

    tmp = usr->video_button->button_func74(1159);
    tmp->value = usr->fxnumber;

    usr->video_button->button_func75(1159);

    ypaworld_func167__sub0(usr);
}


size_t NC_STACK_ypaworld::ypaworld_func168(NC_STACK_ypabact *bact)
{
    if ( bact->_bact_type == BACT_TYPES_GUN || bact->_bact_type == BACT_TYPES_MISSLE )
        return 1;

    if ( bact->_owner == UserRobo->_owner )
    {
        cellArea *cell = bact->_pSector;

        if ( cell->w_type == 6 )
        {
            if ( UserRobo == bact )
            {
                _levelInfo->State = 1;
                _levelInfo->GateCompleteID = cell->w_id;
            }
            else
            {
                field_8283 += (bact->_energy_max + 99) / 100;

                if ( field_8283 < beamenergy )
                    _levelInfo->Buddies.push_back( MapBuddy( bact->_commandID, bact->_vehicleID, bact->_energy ) );
                else
                    return 0;
            }
        }
    }
    return 1;
}

int NC_STACK_ypaworld::LoadingParseSaveFile(const std::string &filename)
{
    int lvlnum;
    ScriptParser::HandlersList parsers
    {
        new World::Parsers::UserParser(this),
        new World::Parsers::SaveRoboParser(this),
        new World::Parsers::SaveSquadParser(this), // commander and units
        new World::Parsers::SaveGemParser(this),
        new World::Parsers::VhclProtoParser(this),
        new World::Parsers::WeaponProtoParser(this),
        new World::Parsers::BuildProtoParser(this),
        new World::Parsers::SaveExtraViewParser(this),
        new World::Parsers::SaveKwFactorParser(this),
        new World::Parsers::SaveGlobalsParser(this),
        new World::Parsers::SaveOwnerMapParser(this),
        new World::Parsers::SaveBuildingMapParser(this),
        new World::Parsers::SaveEnergyMapParser(this),
        new World::Parsers::SaveLevelNumParser(this, &lvlnum),
        new World::Parsers::LevelStatusParser(this, true),
        new World::Parsers::SaveHistoryParser(this),
        new World::Parsers::SaveMasksParser(this),
        new World::Parsers::SaveSuperBombParser(this),
    };

    return ScriptParser::ParseFile(filename, parsers, ScriptParser::FLAG_NO_SCOPE_SKIP);
}

void NC_STACK_ypaworld::LoadingUnitsRefresh()
{
    for ( NC_STACK_ypabact* &station : _unitsList )
    {
        RefreshUnitPRT(station, station, true);

        for ( NC_STACK_ypabact* &commander : station->_kidList )
        {
            RefreshUnitPRT(commander, station, false);

            for ( NC_STACK_ypabact* &slave : commander->_kidList )
                RefreshUnitPRT(slave, station, false);
        }
    }

    if ( _extraViewEnable )
    {
        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(UserRobo);

        if ( robo->_roboGuns[_extraViewNumber].gun_obj )
        {
            robo->_roboGuns[_extraViewNumber].gun_obj->setBACT_viewer(true);
            robo->_roboGuns[_extraViewNumber].gun_obj->setBACT_inputting(true);
        }
    }
}

size_t NC_STACK_ypaworld::LoadGame(const std::string &saveFile)
{
    bool loadOK = false;

    if ( !uaFileExist(saveFile) )
        return 1;

    if ( saveFile.find(".sgm") != std::string::npos || saveFile.find(".SGM") != std::string::npos )
    {
        _maxReloadConst = 0;
        _maxRoboEnergy = 0;
    }

    int lvlnum;
    
    ScriptParser::HandlersList parsers
    {
        new World::Parsers::SaveLevelNumParser(this, &lvlnum),
    };

    ScriptParser::ParseFile(saveFile, parsers, 0);

    _extraViewNumber = -1;
    _extraViewEnable = false;

    LevelDesc mapp;

    if ( LevelCommonLoader(&mapp, lvlnum, 0) )
    {
        if ( LoadTypeMap(mapp.TypStr) )
        {
            if ( LoadOwnerMap(mapp.OwnStr) )
            {
                if ( LoadHightMap(mapp.HgtStr) )
                {
                    if ( LoadBlgMap(mapp.BlgStr) )
                        loadOK = true;
                }
            }
        }
    }

    if ( !loadOK )
    {
        DeleteLevel();
        return 0;
    }

    _levelInfo->OwnerMask = 0;
    _levelInfo->UserMask = 0;

    bact_id = 0x10000;
    dword_5A7A80 = 0;

    InitSuperItems();

    if ( copyof_typemap )
    {
        delete copyof_typemap;
        copyof_typemap = NULL;
    }

    if ( copyof_ownermap )
    {
        delete copyof_ownermap;
        copyof_ownermap = NULL;
    }

    if ( typ_map )
        copyof_typemap = typ_map->Copy();

    if ( own_map )
        copyof_ownermap = own_map->Copy();

    if ( !LoadingParseSaveFile(saveFile) )
        return 0;

    dword_5A7A80++;
    bact_id++;

    if ( UserRobo )
        dynamic_cast<NC_STACK_yparobo *>(UserRobo) ->setROBO_commCount(dword_5A7A80);

    LoadingUnitsRefresh();

    if ( saveFile.find(".fin") != std::string::npos || saveFile.find(".FIN") != std::string::npos )
        InitBuddies();

    for(int y = 0; y < _mapHeight; y++)
    {
        for(int x = 0; x < _mapWidth; x++)
        {
            cellArea *cell = &_cells[x + y * _mapWidth];
            CellCheckHealth(cell, x, y, 255, NULL);
        }
    }

    InitGates();
    UpdatePowerEnergy();

    if ( !sb_0x451034(this) )
        return 0;

    return 1;
}


size_t NC_STACK_ypaworld::SaveGame(const std::string &saveFile)
{
    bool write_ok = true;

    if ( saveFile.find(".sgm") != std::string::npos || saveFile.find(".SGM") != std::string::npos )
        uaDeleteFile( fmt::sprintf("save:%s/sgisold.txt", GameShell->user_name) );

    int write_modifers;
    int write_user;
    int write_level_statuses;

    bool isfin_save = saveFile.find(".fin") != std::string::npos || saveFile.find(".FIN") != std::string::npos;

    if ( isfin_save )
    {
        _maxRoboEnergy = UserRobo->_energy_max;
        write_modifers = 0;
        write_user = 0;
        write_level_statuses = 0;
        _maxReloadConst = UserRobo->_reload_const;
    }
    else
    {
        write_level_statuses = 1;
        write_modifers = 1;
        write_user = 1;
    }
    
    // Force to write last frame timestamp into history
    if (_historyLastIsTimeStamp)
        _history.Write( _historyLastFrame.MakeByteArray() );

    FSMgr::FileHandle *fil = uaOpenFile( saveFile, "w");

    if ( !fil )
    {
        ypa_log_out("Unable to open savegame file\n");
        return 0;
    }

    if ( write_modifers )
    {
        if ( !yw_write_item_modifers(this, fil) )
            write_ok = false;
    }

    if ( write_user )
    {
        if ( !yw_write_user(fil, GameShell) )
            write_ok = false;
    }

    if ( write_ok )
    {
        if ( yw_write_levelnum(this, fil) )
        {
            yw_write_ownermap(this, fil);
            yw_write_buildmap(this, fil);
            yw_write_energymap(this, fil);

            if ( yw_write_units(fil) )
            {
                if ( yw_write_wunderinfo(this, fil) )
                {
                    if ( yw_write_kwfactor(this, fil) )
                    {
                        if ( yw_write_globals(this, fil) )
                        {
                            if ( yw_write_superbomb(this, fil) )
                                write_ok = true;
                        }
                    }
                }
            }
        }
    }

    if ( write_ok && write_level_statuses )
    {
        for (int i = 0; i < 256; i++)
        {
            if ( LevelNet->mapInfos[i].field_0 )
            {
                if ( !yw_write_level_status(fil, this, i) )
                {
                    write_ok = false;
                    break;
                }
            }
        }
    }

    if ( write_ok )
    {
        if ( !isfin_save )
        {
            yw_write_history(this, fil);
            yw_write_masks(this, fil);
        }
    }

    delete fil;
    return write_ok;
}


// Saving game
size_t NC_STACK_ypaworld::ypaworld_func171(yw_arg172 *arg)
{
    UserData *usr = arg->usr;

    if ( !arg->usertxt || !arg->field_4 )
    {
        ypa_log_out("No names for save action\n");
        return 0;
    }

    FSMgr::FileHandle *sfil = uaOpenFile(fmt::sprintf("save:%s", arg->usertxt), "w");

    if ( !sfil )
        return 1;

    if ( arg->field_8 & 1 && !yw_write_user(sfil, usr) )
    {
        ypa_log_out("Unable to write user data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 2 && !yw_write_input(sfil, usr) )
    {
        ypa_log_out("Unable to write input data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 8 && !yw_write_sound(sfil, usr) )
    {
        ypa_log_out("Unable to write sound data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 4 && !yw_write_video(sfil, usr) )
    {
        ypa_log_out("Unable to write video data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 0x10 && !yw_write_levels_statuses(sfil, usr->p_ypaworld) )
    {
        ypa_log_out("Unable to write score data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 0x80 && !yw_write_buddies(sfil, usr->p_ypaworld) )
    {
        ypa_log_out("Unable to write buddies to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 0x20 && !yw_write_shell(sfil, usr) )
    {
        ypa_log_out("Unable to write shell data to file\n");
        delete sfil;
        return 0;
    }

    if ( arg->field_8 & 0x40 && !yw_write_item_modifers(usr->p_ypaworld, sfil) )
    {
        ypa_log_out("Unable to write build info to file\n");
        delete sfil;
        return 0;
    }

    delete sfil;
    return 1;
}



int NC_STACK_ypaworld::ypaworld_func172__sub0(const std::string &fname, int parsers_mask)
{
    ScriptParser::HandlersList parsers;
    if ( parsers_mask & World::SDF_USER )
        parsers += new World::Parsers::UserParser(this);

    if ( parsers_mask & World::SDF_INPUT )
        parsers += new World::Parsers::InputParser(this);

    if ( parsers_mask & World::SDF_VIDEO )
        parsers += new World::Parsers::VideoParser(this);

    if ( parsers_mask & World::SDF_SOUND )
        parsers += new World::Parsers::SoundParser(this);

    if ( parsers_mask & World::SDF_SCORE )
        parsers += new World::Parsers::LevelStatusParser(this, true);

    if ( parsers_mask & World::SDF_BUDDY )
        parsers += new World::Parsers::BuddyParser(this);

    if ( parsers_mask & World::SDF_SHELL )
        parsers += new World::Parsers::ShellParser(this);

    if ( parsers_mask & World::SDF_PROTO )
    {
        parsers += new World::Parsers::VhclProtoParser(this);
        parsers += new World::Parsers::WeaponProtoParser(this);
        parsers += new World::Parsers::BuildProtoParser(this);
    }

    return ScriptParser::ParseFile(fname, parsers, 0);
}

// Load user save
size_t NC_STACK_ypaworld::ypaworld_func172(yw_arg172 *arg)
{
    UserData *usr = arg->usr;

    if ( arg->field_8 & 0x10 )
    {
        if ( usr->field_0x8 )
        {
            usr->field_0x8 = 0;
        }
        else
        {
            std::string tmpStr = fmt::sprintf("%s/user.txt", usr->user_name);

            yw_arg172 v12;
            v12.usr = usr;
            v12.usertxt = tmpStr.c_str();
            v12.field_10 = 0;
            v12.field_4 = usr->user_name.c_str();
            v12.field_8 = 255;

            ypaworld_func171(&v12);
        }
    }

    usr->_saveDataFlags = 0;

    if ( arg->field_8 & 0x80 )
        _levelInfo->Buddies.clear();

    if ( !ypaworld_func172__sub0(fmt::sprintf("save:%s", arg->usertxt), arg->field_8) )
    {
        ypa_log_out("Error while loading information from %s\n", arg->usertxt);
        return 0;
    }

    if ( (arg->field_10 & 1) && !usr->field_0x0 && !ypaworld_func156(usr) ) // Init menus
    {
        ypa_log_out("Unable to open GameShell\n");
        return 0;
    }

    if ( arg->field_8 & 0x10 && usr->_saveDataFlags & 0x10 )
    {
        usr->user_name = arg->field_4;
    }

    if ( arg->field_8 & 2 )
        usr->InputConfCopyToBackup();

    if ( arg->field_10 & 1 )
        ypaworld_func167(usr); // Update menu values

    return 1;
}


size_t NC_STACK_ypaworld::ypaworld_func173(UserData *usr)
{
    std::string keyConfStr;

    if ( usr->field_D36 < 1 || usr->field_D36 >= usr->InputConfig.size() )
        return 0;

    UserData::TInputConf &kconf = usr->InputConfig.at(usr->field_D36);

    if ( NC_STACK_input::KeyNamesTable.at(kconf.PKeyCode).Name.empty() )
        return 0;

    if ( kconf.Type == World::INPUT_BIND_TYPE_SLIDER && NC_STACK_input::KeyNamesTable.at(kconf.NKeyCode).Name.empty() )
        return 0;

    if ( kconf.Type == World::INPUT_BIND_TYPE_SLIDER )
    {
        keyConfStr += "~#";
        keyConfStr += "winp:";
        keyConfStr += NC_STACK_input::KeyNamesTable.at(kconf.NKeyCode).Name;
        keyConfStr += " #";
        keyConfStr += "winp:";
    }
    else if ( kconf.Type == World::INPUT_BIND_TYPE_BUTTON )
    {
        keyConfStr += "winp:";
    }

    if ( NC_STACK_input::KeyNamesTable.at(kconf.PKeyCode).Name.empty() )
        return 0;

    keyConfStr += NC_STACK_input::KeyNamesTable.at(kconf.PKeyCode).Name;

    NC_STACK_input *v38 = INPe.getPInput();

    if ( kconf.Type == World::INPUT_BIND_TYPE_HOTKEY )
    {
        if ( !v38->SetHotKey(kconf.KeyID, keyConfStr) )
            ypa_log_out("input.engine: WARNING: Hotkey[%d] (%s) not accepted.\n", kconf.KeyID, keyConfStr.c_str());
    }
    else
    {
        if ( kconf.Type == World::INPUT_BIND_TYPE_BUTTON )
        {
            if ( !v38->SetInputExpression(false, kconf.KeyID, keyConfStr) )
                ypa_log_out("input.engine: WARNING: Button[%d] (%s) not accepted.\n", kconf.KeyID, keyConfStr.c_str());
        }
        else
        {
            if ( !v38->SetInputExpression(true, kconf.KeyID, keyConfStr) )
                ypa_log_out("input.engine: WARNING: Slider[%d] (%s) not accepted.\n", kconf.KeyID, keyConfStr.c_str());
        }
    }
    kconf.NKeyCodeBkp = kconf.NKeyCode;
    kconf.PKeyCodeBkp = kconf.PKeyCode;
    return 1;
}


size_t NC_STACK_ypaworld::ypaworld_func174(yw_174arg *arg)
{
    UserData *usr = GameShell;

    NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

    int current_resolution;

    current_resolution = win3d->getDISP_displID();

    if ( arg->resolution == current_resolution && !arg->make_changes )
        return 1;

    int v6;

    if ( usr->field_0x0 )
    {
        ypaworld_func157(usr);
        v6 = 1;
    }
    else
    {
        v6 = 0;
    }

    GFXEngine::GFXe.setResolution( arg->resolution );

    screen_width = GFXEngine::GFXe.getScreenW();
    screen_height = GFXEngine::GFXe.getScreenH();

    if ( v6 && !ypaworld_func156(usr))
    {
        ypa_log_out("Warning: Unable to open GameShell with mode %d\n", arg->resolution);

        GFXEngine::GFXe.setResolution( usr->p_ypaworld->shell_default_res );

        screen_width = GFXEngine::GFXe.getScreenW();
        screen_height = GFXEngine::GFXe.getScreenH();

        if ( !ypaworld_func156(usr) )
        {
            return 0;
        }
    }

    win3d = GFXEngine::GFXe.getC3D();

    if ( usr->GFX_flags & 4 )
    {
        win3d->setWDD_cursor(1);
    }
    else
    {
        win3d->setWDD_cursor(0);
    }

    if ( screen_width >= 512 )
    {
        win3d->LoadFontByDescr( get_lang_string(string_pointers_p2, 15, "MS Sans Serif,12,400,0") );
        Gui::UA::LoadFont( get_lang_string(string_pointers_p2, 15, "MS Sans Serif,12,400,0") );
    }
    else
    {
        win3d->LoadFontByDescr( get_lang_string(string_pointers_p2, 16, "Arial,8,400,0") );
        Gui::UA::LoadFont( get_lang_string(string_pointers_p2, 16, "Arial,8,400,0") );
    }

    return 1;
}


size_t NC_STACK_ypaworld::ypaworld_func175(UserData *usr)
{
    if ( !usr->default_lang_dll )
    {
        ypa_log_out("Set Language, but no language selected\n");
        return 0;
    }

    int v6;

    if ( usr->field_0x0 )
    {
        ypaworld_func157(usr);
        v6 = 1;
    }
    else
    {
        v6 = 0;
    }

    const char *v7 = usr->default_lang_dll->c_str();
    if ( !ypaworld_func166(&v7) )
        ypa_log_out("Warning: SETLANGUAGE failed\n");

    if ( v6 && !ypaworld_func156(usr) )
    {
        ypa_log_out("Unable to open GameShell\n");
        return 0;
    }

    return 1;
}


void NC_STACK_ypaworld::ypaworld_func176(yw_arg176 *arg)
{
    arg->field_4 = field_1bcc[arg->owner];
    arg->field_8 = field_1bec[arg->owner];
}


void NC_STACK_ypaworld::ypaworld_func177(yw_arg177 *arg)
{
    //Reown sectors for new owner

    if ( !arg->field_4 ) //New owner
        return;

    if ( arg->bact == UserRobo )
        field_1624 = 1;

    if ( UserRobo )
    {
        if ( UserRobo->_owner == arg->field_4 )
            beamenergy += beam_energy_add;
    }

    for ( NC_STACK_ypabact* &unit : _unitsList )
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && unit != arg->bact && arg->bact->_owner == unit->_owner )
            return;
    }

    for (int i = 0; i < _mapHeight; i++)
    {
        for (int j = 0; j < _mapWidth; j++)
        {
            cellArea *v9 = _cells + _mapWidth * i + j;

            if ( v9->owner == arg->bact->_owner )
                CellSetOwner(v9, j, i, arg->field_4);
        }
    }

    if ( !UserRobo )
        return;

    if ( UserRobo->_owner != arg->field_4 )
        return;

    for (int i = 0; i < _mapHeight; i++)
    {
        for (int j = 0; j < _mapWidth; j++)
        {
            cellArea *v15 = _cells + _mapWidth * i + j;

            if ( v15->w_type == 4 && v15->owner == UserRobo->_owner )
            {

                if ( isNetGame )
                    sub_47C29C(this, v15, v15->w_id);
                else
                    yw_ActivateWunderstein(v15, v15->w_id);
                
                HistoryEventAdd( World::History::Upgrade(j, i, v15->owner, _Gems[ field_2b78 ].Type, last_modify_vhcl, last_modify_weapon, last_modify_build) );
            }

        }
    }
}

//179 method in yw_net


void NC_STACK_ypaworld::ypaworld_func180(yw_arg180 *arg)
{
    if ( field_739A )
    {
        if ( field_73CE & (4 | 8) )
            return;
    }

    switch ( arg->effects_type )
    {
    case 0:
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_MISSILEFIRE);

        break;

    case 1:
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_GRENADEFIRE);

        break;

    case 2:
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_BOMBFIRE);

        break;

    case 3:
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_MINIGUN);

        break;

    case 4:
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_STOP, NC_STACK_winp::FF_TYPE_MINIGUN);

        break;

    case 5:
    {
        NC_STACK_ypabact *bct = UserUnit;
        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_COLLISION,
            arg->field_4, 0.0,
            (arg->field_C - bct->_position.z) * bct->_rotation.m02 + (arg->field_8 - bct->_position.x) * bct->_rotation.m00,
            -((arg->field_8 - bct->_position.x) * bct->_rotation.m20 + (arg->field_C - bct->_position.z) * bct->_rotation.m22));

    }
    break;

    default:
        break;
    }
}


bool NC_STACK_ypaworld::ypaworld_func181(yw_arg181 *arg)
{
    if (GameShell->noSent)
        return false;
    
    arg->data->tstamp = timeStamp;
    
    if ( GameShell->netPlayerOwner )
    {
        if ( arg->recvFlags == 2 && arg->data->msgID != UAMSG_VHCLENERGY )
        {
            arg->data->msgCnt = GameShell->msgcount;
            GameShell->msgcount++; 
        }
    }
    
    arg->senderFlags = 1;
    arg->senderID = GameShell->callSIGN.c_str();
    return windp->Send(arg);
}


void NC_STACK_ypaworld::ypaworld_func182(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func182");
}

int ypaworld_func183__sub0(int lvlID, const char *userName)
{
    FSMgr::FileHandle *fil = uaOpenFile( fmt::sprintf("save:%s/%d.fin", userName, lvlID) , "r");

    if ( !fil )
        return 0;

    delete fil;
    return 1;
}

// Advanced Create Level
size_t NC_STACK_ypaworld::ypaworld_func183(yw_arg161 *arg)
{
    int v6;

    if ( LevelNet->mapInfos[ arg->lvlID ].field_0 == 3 && ypaworld_func183__sub0(arg->lvlID, GameShell->user_name.c_str()) )
    {
        std::string savename = fmt::sprintf("save:%s/%d.fin", GameShell->user_name, arg->lvlID);
        v6 = LoadGame(savename);

        if ( !v6 )
            ypa_log_out("Warning: in YWM_ADVANCEDCREATELEVEL: YWM_LOADGAME of %s failed!\n", savename.c_str());

        UserRobo->_energy = UserRobo->_energy_max;

        if ( map_events )
            map_events->event_loop_id = 0;
    }
    else
    {
        v6 = ypaworld_func161(arg);

        if ( !v6 )
            ypa_log_out("Warning: in YWM_ADVANCEDCREATELEVEL: YWM_CREATELEVEL %d failed!\n", arg->lvlID);
    }

    if ( v6 )
    {
        if ( !SaveGame(fmt::sprintf("save:%s/%d.rst", GameShell->user_name, _levelInfo->LevelID)) )
            ypa_log_out("Warning: could not create restart file for level %d, user %s.\n", _levelInfo->LevelID, GameShell->user_name.c_str());
    }

    if ( copyof_typemap )
    {
        delete copyof_typemap;
        copyof_typemap = NULL;
    }

    if ( copyof_ownermap )
    {
        delete copyof_ownermap;
        copyof_ownermap = NULL;
    }

    if ( typ_map )
        copyof_typemap = typ_map->Copy();

    if ( own_map )
        copyof_ownermap = own_map->Copy();

    return v6;
}


void NC_STACK_ypaworld::HistoryEventAdd(const World::History::Record &arg)
{
    switch ( arg.type )
    {
    case World::History::TYPE_FRAME: // Do not write timestamp every frame, wait for any another data
        _historyLastIsTimeStamp = true;
        _historyLastFrame = static_cast<const World::History::Frame&>(arg);
        break;
                
    case World::History::TYPE_CONQ:
    case World::History::TYPE_VHCLKILL:
    case World::History::TYPE_VHCLCREATE:
    case World::History::TYPE_POWERST:
    case World::History::TYPE_UPGRADE:
        
        if (_historyLastIsTimeStamp) // If
            _history.Write(_historyLastFrame.MakeByteArray());
            
        _history.Write(arg.MakeByteArray());
        
        _historyLastIsTimeStamp = false;
        
        if (GameShell && GameShell->isHost )
            arg.AddScore(&ingamePlayerStatus);
        break;

    default:
        break;
    }

    
}


void NC_STACK_ypaworld::ypaworld_func185(void *arg)
{
    dprintf("MAKE ME %s\n","ypaworld_func185");
}


void NC_STACK_ypaworld::setYW_normVisLimit(int limit)
{
    field_15e4 = limit;
}

void NC_STACK_ypaworld::setYW_fadeLength(int len)
{
    field_15e8 = len;
}

void NC_STACK_ypaworld::setYW_skyVisLimit(int limit)
{
    field_15ec = limit;
}

void NC_STACK_ypaworld::setYW_skyFadeLength(int len)
{
    field_15f0 = len;
}

void NC_STACK_ypaworld::setYW_skyHeight(int hght)
{
    field_15f4 = hght;
}

void NC_STACK_ypaworld::setYW_skyRender(int dorender)
{
    field_15f8 = dorender;
}

void NC_STACK_ypaworld::setYW_doEnergyRecalc(int doRecalc)
{
    field_15fc = doRecalc;
}

void NC_STACK_ypaworld::setYW_visSectors(int visSectors)
{
    field_1368 = visSectors;
}

void NC_STACK_ypaworld::setYW_userHostStation(NC_STACK_ypabact *host)
{
    UserRobo = host;
    _UserRoboKidsList = &UserRobo->_kidList;
}

void NC_STACK_ypaworld::setYW_userVehicle(NC_STACK_ypabact *bact)
{
    if ( bact != UserUnit )
    {
        NC_STACK_ypabact *oldpBact = UserUnit;

        if ( oldpBact )
            field_241c = oldpBact->_gid;

        UserUnit = bact;

        field_1a0c = timeStamp;
        field_1a10 = UserUnit->_commandID;
        draggingLock = 0;

        if ( UserUnit->_bact_type == BACT_TYPES_ROBO )
        {
            field_7886 = 1;
            field_7882 = 1;
        }

        FFeedback_VehicleChanged();

        if ( oldpBact )
            ypaworld_func2__sub0__sub1(this, oldpBact, UserUnit);
    }
}

void NC_STACK_ypaworld::setYW_screenW(int w)
{
    screen_width = w;
}

void NC_STACK_ypaworld::setYW_screenH(int h)
{
    screen_height = h;
}

void NC_STACK_ypaworld::setYW_dontRender(int drndr)
{
    field_138c = drndr;
}




int NC_STACK_ypaworld::getYW_mapMaxX()
{
    return _mapAbsMaxX;
}

int NC_STACK_ypaworld::getYW_mapMaxY()
{
    return _mapAbsMaxY;
}

int NC_STACK_ypaworld::getYW_mapSizeX()
{
    return _mapWidth;
}

int NC_STACK_ypaworld::getYW_mapSizeY()
{
    return _mapHeight;
}

int NC_STACK_ypaworld::getYW_normVisLimit()
{
    return field_15e4;
}

int NC_STACK_ypaworld::getYW_fadeLength()
{
    return field_15e8;
}

int NC_STACK_ypaworld::getYW_skyHeight()
{
    return field_15f4;
}

int NC_STACK_ypaworld::getYW_skyRender()
{
    return field_15f8;
}

int NC_STACK_ypaworld::getYW_doEnergyRecalc()
{
    return field_15fc;
}

int NC_STACK_ypaworld::getYW_visSectors()
{
    return field_1368;
}

NC_STACK_ypabact *NC_STACK_ypaworld::getYW_userHostStation()
{
    return UserRobo;
}

NC_STACK_ypabact *NC_STACK_ypaworld::getYW_userVehicle()
{
    return UserUnit;
}

WeapProto *NC_STACK_ypaworld::getYW_weaponProtos()
{
    return WeaponProtos;
}

TBuildingProto *NC_STACK_ypaworld::getYW_buildProtos()
{
    return BuildProtos.data();
}

VhclProto *NC_STACK_ypaworld::getYW_vhclProtos()
{
    return VhclProtos;
}

int NC_STACK_ypaworld::getYW_lvlFinished()
{
    if ( _levelInfo->State != 1 && _levelInfo->State != 2 )
        return 0;

    return 1;
}

int NC_STACK_ypaworld::getYW_screenW()
{
    return screen_width;
}

int NC_STACK_ypaworld::getYW_screenH()
{
    return screen_height;
}

char **NC_STACK_ypaworld::getYW_localeStrings()
{
    return string_pointers;
}

LevelInfo *NC_STACK_ypaworld::getYW_levelInfo()
{
    return _levelInfo;
}

int NC_STACK_ypaworld::getYW_destroyFX()
{
    return fxnumber;
}

NC_STACK_windp *NC_STACK_ypaworld::getYW_pNET()
{
    return windp;
}

int NC_STACK_ypaworld::getYW_invulnerable()
{
    return field_1a20;
}



int NC_STACK_ypaworld::TestVehicle(int protoID, int job)
{
    VhclProto &proto = VhclProtos[ protoID ];

    WeapProto *wpn;

    if ( proto.weapon == -1 )
        wpn = NULL;
    else
        wpn = &WeaponProtos[ proto.weapon ];

    switch ( job )
    {
    case 1:
        if ( (proto.mgun == -1 && !wpn) || proto.model_id == 7 )
            return -1;

        return proto.job_fightrobo;
        break;

    case 2:
        if ( (proto.mgun == -1 && !wpn) || proto.model_id == 7 )
            return -1;

        return proto.job_fighttank;
        break;

    case 4:
        if ( (proto.mgun == -1 && !wpn) || proto.model_id == 7 )
            return -1;

        return proto.job_fighthelicopter;
        break;

    case 3:
        if ( (proto.mgun == -1 && !wpn) || proto.model_id == 7 )
            return -1;

        return proto.job_fightflyer;
        break;

    case 5:
        return proto.job_reconnoitre;
        break;

    case 6:
        if ( !wpn || proto.model_id == 7 )
            return -1;

        return proto.job_conquer;
        break;

    default:
        break;
    }
    return -1;
}


void NC_STACK_ypaworld::UpdateGuiSettings()
{
    Gui::UA::_UATextColor.a = 255;
    Gui::UA::_UATextColor.r = iniColors[60].r;
    Gui::UA::_UATextColor.g = iniColors[60].g;
    Gui::UA::_UATextColor.b = iniColors[60].b;

    

    /*for (uint8_t i = 0; i < ypaworld.tiles.size(); i++)
        Gui::UA::_UATiles[i] = ypaworld.tiles[i];*/
}

void NC_STACK_ypaworld::LoadGuiFonts()
{
    std::string old = SetPathKeys("rsrc", "data:set46");

    Gui::UA::_UATiles[Gui::UA::TILESET_46MAPC16] = yw_LoadFont("mapcur16.font"); //18
    Gui::UA::_UATiles[Gui::UA::TILESET_46MAPC32] = yw_LoadFont("mapcur32.font"); //19
    Gui::UA::_UATiles[Gui::UA::TILESET_46ENERGY] = yw_LoadFont("energy.font"); //30
    
    SetPathKeys("rsrc", "data:fonts");
    Gui::UA::_UATiles[Gui::UA::TILESET_DEFAULT]     = yw_LoadFont("default.font"); //0
    Gui::UA::_UATiles[Gui::UA::TILESET_MENUGRAY]    = yw_LoadFont("menugray.font"); //6
    Gui::UA::_UATiles[Gui::UA::TILESET_ICONNS]      = yw_LoadFont("icon_ns.font"); //24
    Gui::UA::_UATiles[Gui::UA::TILESET_ICONPS]      = yw_LoadFont("icon_ps.font"); //25
    Gui::UA::_UATiles[Gui::UA::TILESET_MAPHORZ]     = yw_LoadFont("maphorz.font"); //11
    Gui::UA::_UATiles[Gui::UA::TILESET_MAPVERT]     = yw_LoadFont("mapvert.font"); //12
    Gui::UA::_UATiles[Gui::UA::TILESET_MAPVERT1]    = yw_LoadFont("mapvert1.font"); //13

    SetPathKeys("rsrc", old);    
}
