#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include <string.h>
#include <array>

#include "types.h"

#include "engine_gfx.h"
#include "engine_input.h"
#include "engine_miles.h"
#include "bitmap.h"
#include "ilbm.h"
#include "display.h"
#include "win3d.h"
#include "base.h"
#include "sklt.h"
#include "wav.h"
#include "ypabact.h"
#include "ypamissile.h"
#include "ypagun.h"

#include "lstvw.h"

#include "input.h"

#include "glob_funcs.h"

#include "world/parsers.h"
#include "world/saveparsers.h"


#define YW_RENDER_SECTORS_DEF   5

class NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;
class NC_STACK_button;
class NC_STACK_windp;

struct vhclBases;
struct cityBases;
struct subSec;
struct secType;
struct VhclProto;
struct WeapProto;
struct BuildProto;
struct roboProto;
struct map_event;
struct uamessage_base;


namespace World
{

enum COLOR_IDS
{
	COLOR_OWNER_0 = 0,
	COLOR_OWNER_1 = 1,
	COLOR_OWNER_2 = 2,
	COLOR_OWNER_3 = 3,
	COLOR_OWNER_4 = 4,
	COLOR_OWNER_5 = 5,
	COLOR_OWNER_6 = 6,
	COLOR_OWNER_7 = 7,

	COLOR_MAP_DIRECTION 	= 8,
	COLOR_MAP_PRIMTARGET 	= 9,
	COLOR_MAP_SECTARGET 	= 10,
	COLOR_MAP_COMMANDER 	= 11,
	COLOR_MAP_DRAGBOX 		= 12,
	COLOR_MAP_VIEWER 		= 13,

	COLOR_HUD_WEAPON_0 			= 20,
	COLOR_HUD_COMPASS_CMDVEC_0 	= 21,
	COLOR_HUD_COMPASS_PRIMTGT_0 = 22,
	COLOR_HUD_COMPASS_LOCKTGT_0 = 23,
	COLOR_HUD_COMPASS_0 		= 24,
	COLOR_HUD_VEHICLE_0 		= 25,
	COLOR_HUD_VISOR_MG_0 		= 26,
	COLOR_HUD_VISOR_LOCKED_0 	= 27,
	COLOR_HUD_VISOR_AUTONOM_0 	= 28,

	COLOR_HUD_WEAPON_1 			= 29,
	COLOR_HUD_COMPASS_CMDVEC_1 	= 30,
	COLOR_HUD_COMPASS_PRIMTGT_1 = 31,
	COLOR_HUD_COMPASS_LOCKTGT_1 = 32,
	COLOR_HUD_COMPASS_1 		= 33,
	COLOR_HUD_VEHICLE_1 		= 34,
	COLOR_HUD_VISOR_MG_1 		= 35,
	COLOR_HUD_VISOR_LOCKED_1 	= 36,
	COLOR_HUD_VISOR_AUTONOM_1 	= 37,

	COLOR_BRIEF_LINES 	= 50,

	COLOR_TEXT_DEFAULT 		= 60,
	COLOR_TEXT_LIST 		= 61,
	COLOR_TEXT_LIST_SEL		= 62,
	COLOR_TEXT_TOOLTIP		= 63,
	COLOR_TEXT_MESSAGE		= 64,
	COLOR_TEXT_HUD			= 65,
	COLOR_TEXT_BRIEFING		= 66,
	COLOR_TEXT_DEBRIEFING	= 67,
	COLOR_TEXT_BUTTON		= 68,

	COLOR_MAX_NUMBER		= 70,
};

enum MOVIE_IDS
{
	MOVIE_INTRO		= 0,
	MOVIE_LOSE		= 1,
	MOVIE_WIN		= 2,
	MOVIE_USER		= 3,
	MOVIE_KYT		= 4,
	MOVIE_TAER		= 5,
	MOVIE_MYK		= 6,
	MOVIE_SULG		= 7,
	MOVIE_BLACK		= 8,

	MOVIE_MAX_NUMBER = 9
};

enum PREFERENCES_FLAGS
{
    PREF_JOYDISABLE     =     4,
    PREF_FFDISABLE      =     8,
    PREF_ENEMYINDICATOR =  0x20,
    PREF_SOFTMOUSE      =  0x40,
    PREF_CDMUSICDISABLE = 0x100,
    PREF_ALTJOYSTICK    = 0x200,
};

enum KEYCONF
{
    KEYC_TYPE_BUTTON        = 1,
    KEYC_TYPE_SLIDER        = 2,
    KEYC_TYPE_HOTKEY        = 3,

    KEYC_PAUSE      = 1,
    KEYC_QUIT       = 2,
    KEYC_DRIVE_DIR  = 3,
    KEYC_DRIVE_SPEED= 4,
    KEYC_GUN_HEIGHT = 5,
    KEYC_FLY_HEIGHT = 6,
    KEYC_FLY_SPEED  = 7,
    KEYC_FLY_DIR    = 8,
    KEYC_BRAKE      = 9,
    KEYC_FIRE       = 10,
    KEYC_CAMFIRE    = 11,
    KEYC_GUN        = 12,
    KEYC_SET_COMM   = 13,
    KEYC_HUD        = 14,
    KEYC_AUTOPILOT  = 15,
    KEYC_ORDER      = 16,
    KEYC_NEW        = 17,
    KEYC_ADD        = 18,
    KEYC_SQ_MANAGE  = 19,
    KEYC_AGGR_1     = 20,
    KEYC_AGGR_2     = 21,
    KEYC_AGGR_3     = 22,
    KEYC_AGGR_4     = 23,
    KEYC_AGGR_5     = 24,
    KEYC_MAP        = 25,
    KEYC_WAPOINT    = 26,
    KEYC_LANDLAYER  = 27,
    KEYC_OWNER      = 28,
    KEYC_HEIGHT     = 29,
    KEYC_MINIMAP    = 30,
    KEYC_LOCKVIEW   = 31,
    KEYC_ZOOMIN     = 32,
    KEYC_ZOOMOUT    = 33,
    KEYC_LOG_WND    = 34,
    KEYC_CONTROL    = 35,
    KEYC_LAST_SEAT  = 36,
    KEYC_ATTACK     = 37,
    KEYC_TO_HOST    = 38,
    KEYC_TO_COMM    = 39,
    KEYC_NEXT_UNIT  = 40,
    KEYC_NEXT_COMM  = 41,
    KEYC_LAST_MSG   = 42,
    KEYC_TO_ALL     = 43,
    KEYC_HELP       = 44,
    KEYC_ANALYZER   = 45,

    KEYC_KEY_NUMBER = 46,
};

enum SOUND_ID
{
    SOUND_ID_VOLUME = 0,
    SOUND_ID_RIGHT  = 1,
    SOUND_ID_LEFT   = 2,
    SOUND_ID_BUTTON = 3,
    SOUND_ID_QUIT   = 4,
    SOUND_ID_SLIDER = 5,
    SOUND_ID_WELCOME = 6,
    SOUND_ID_MENUOPEN = 7,
    SOUND_ID_OVERLEVEL = 8,
    SOUND_ID_LEVELSEL = 9,
    SOUND_ID_TEXTAPPEAR = 10,
    SOUND_ID_OBJAPPEAR = 11,
    SOUND_ID_SECTCONQ = 12,
    SOUND_ID_VHCLDESTR = 13,
    SOUND_ID_BLDGCONQ = 14,
    SOUND_ID_TIMERCOUNT = 15,

    SOUND_ID_SELECT = 0,
    SOUND_ID_ERROR  = 1,
    SOUND_ID_ATTEN  = 2,
    SOUND_ID_SECRET = 3,
    SOUND_ID_PLASMA = 4,
};

enum GFX_FLAG
{
    GFX_FLAG_FARVIEW            =    1,
    GFX_FLAG_SKYRENDER          =    2,
    GFX_FLAG_SOFTMOUSE          =    4,
    GFX_FLAG_DRAWPRIMITIVES     =    8,
    GFX_FLAG_16BITTEXTURE       = 0x10,
};

enum SAVE_DATA_FLAG
{
    SDF_USER     =    1,
    SDF_INPUT    =    2,
    SDF_VIDEO    =    4,
    SDF_SOUND    =    8,
    SDF_SCORE    = 0x10,
    SDF_SHELL    = 0x20,
    SDF_PROTO    = 0x40,
    SDF_BUDDY    = 0x80,
};

enum SOUND_FLAGS
{
    SF_INVERTLR  =    1,
    SF_CDSOUND   = 0x10,
};


class AssignParser : public ScriptParser::DataHandler
{
public:
	virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
	virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_assign"); };

	static ScriptParser::DataHandler *MakeParser() {return new AssignParser;};
};

};

struct video_mode_node
{
    std::string name;
    int sort_id;
    int16_t width;
    int16_t height;
};

typedef std::list<video_mode_node> VideoModesList;


struct usr_str
{
    int id;
    const char *pstring;

    usr_str()
    {
        id = 0;
        pstring = NULL;
    }
};

struct audiotrack_adv
{
    int min_delay;
    int max_delay;
    audiotrack_adv() : min_delay(0), max_delay(0) {};
};

struct inp_key_setting
{
    int16_t inp_type;
    int16_t keyID;
    int16_t KeyCode;
    int16_t slider_neg;
    int16_t field_8;
    int16_t field_A;
    int16_t field_C;
    int16_t field_E;
    int field_10;
    const char *slider_name;
};

struct player_status
{
    int destroyed;
    int destroyedByUser;
    int elapsedTime;
    int sectorsTaked;
    int score;
    int power;
    int upgrades;

    player_status()
    {
    	clear();
    }

    void clear()
    {
    	destroyed = 0;
        destroyedByUser = 0;
        elapsedTime = 0;
        sectorsTaked = 0;
        score = 0;
        power = 0;
        upgrades = 0;
    }
};

struct ProfilesNode
{
    int totalElapsedTime;
    char fraction;
    std::string name;

    ProfilesNode() : totalElapsedTime(0), fraction(0) {};
};

typedef std::list<ProfilesNode> ProfileList;


struct netType1
{
    char msg[64];
    uint8_t Fraction;
    uint8_t trbl;
    uint8_t owner;
    uint8_t rdyStart;
    uint8_t welcmd;
    uint8_t cd;
    uint8_t p[2];
    int w84upd;
    uint32_t checksum;
    char name[64];
};

struct netType2
{
    char name[64];
    char msg[64];
    uint8_t rdyStart;
    uint8_t fraction;
    uint8_t isKilled;
    uint8_t status;
    uint8_t connProblem;
    uint32_t lastMsgTime;
    uint32_t tstamp;
    uint32_t msgCnt;
    int problemCnt;
    int latency;
};

enum
{
    ENVMODE_TITLE = 1,
    ENVMODE_INPUT = 2,
    ENVMODE_SETTINGS = 3,
    ENVMODE_TUTORIAL = 4,
    ENVMODE_SINGLEPLAY = 5,
    ENVMODE_NETPLAY = 6,
    ENVMODE_SELLOCALE = 7,
    ENVMODE_ABOUT = 8,
    ENVMODE_SELPLAYER = 9,
    ENVMODE_HELP = 10
};

struct EnvAction
{
    enum
    {
        ACTION_NONE     = 0,
        ACTION_QUIT     = 1,
        ACTION_PLAY     = 2,
        ACTION_LOAD     = 3,
        ACTION_NETPLAY  = 4,
        ACTION_DEMO     = 5
    };

    int action;
    int params[5];

    EnvAction() : action(ACTION_NONE), params{0, 0, 0, 0, 0} {};
};

class UserData
{
friend class World::Parsers::ShellSoundParser;

public:
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    std::string user_name;

    NC_STACK_ypaworld *p_YW;
    _NC_STACK_ypaworld *p_ypaworld;
    struC5 *_input;
    int frameTime;
    uint32_t glblTime;

    int envMode;
    bool envModeChanged;

    bool returnToTitle;

    samples_collection1 samples1_info;
    NC_STACK_wav *samples1[16];
    samples_collection1 samples2_info;
    NC_STACK_wav *samples2[16];
    samples_collection1 field_782;
    NC_STACK_wav *field_ADA;
    NC_STACK_button *sub_bar_button;
    int field_AE2;
    NC_STACK_button *titel_button;
    NC_STACK_button *button_input_button;
    GuiList input_listview;
    int field_D36;
    int field_D3A;
    bool inp_joystick;
    int field_D42;
    bool inp_altjoystick;
    int field_D4A;
    int field_D4E;
    int field_D52;

    int16_t field_D5A;
    int16_t field_0xd5c;

    int field_D5E;
    NC_STACK_button *video_button;
    GuiList video_listvw;
    int game_default_res;
    VideoModesList video_mode_list;
    int field_FBE;
    GuiList d3d_listvw;
    char win3d_guid[100];
    char win3d_name[300];

    const char *field_139A;
    const char *field_139E;
    int16_t fxnumber;
    int16_t field_0x13a4;
    char GFX_flags;
    int16_t field_0x13a8;
    int16_t field_13AA;
    int16_t field_0x13ac;
    char snd__flags2;

    int16_t field_0x13b0;
    int16_t snd__volume;
    int16_t field_0x13b4;
    int16_t snd__cdvolume;
    int16_t field_0x13b8;
    bool enemyindicator;
    int field_13BE;
    int field_13C2;
    NC_STACK_button *disk_button;
    GuiList disk_listvw;
    int field_1612;
    char usernamedir[300];
    int usernamedir_len;
    int16_t field_0x1744;
    //FSMgr::DirIter *opened_dir;
    ProfileList profiles;
    char field_1756;
    uint8_t _saveDataFlags;
    int16_t field_0x1758;
    int16_t field_175A;
    int16_t field_0x175c;
    int16_t field_175E;
    int16_t field_0x1760;
    NC_STACK_button *locale_button;
    Engine::StringList lang_dlls;
    GuiList local_listvw;
    std::string *default_lang_dll;
    std::string *prev_lang;

    int16_t field_19C6;
    int16_t field_0x19c8;
    int field_19CA;
    int lang_dlls_count;
    NC_STACK_button *about_button;
    int field_19D6;
    int field_19DA;
    int16_t field_19DE;
    int16_t field_0x19e0;
    NC_STACK_button *network_button;
    GuiList network_listvw;
    int16_t field_1C2E;
    int16_t field_0x1c30;
    int16_t field_1C32;
    int16_t field_0x1c34;

    int field_1C36;
    int16_t netSelMode;
    int16_t netSel;
    int nInputMode;
    char netName[64];

    int16_t netNameCurPos;
    int16_t netLevelID;
    const char *netLevelName;

    std::string callSIGN;
    uint32_t netCRC;
    int32_t takTime;
    uint8_t netPlayerOwner;
    char FreeFraction;
    char SelectedFraction;
    char field_1CD7;
    int isHost;
    int modemAskSession;

    uint32_t msgcount;
    char field_1CE8;
    char rdyStart;
    char remoteMode;
    uint32_t disconnected;
    int blocked;
    uint32_t problemCnt;
    int16_t msgBuffLine;
    char msgBuffers[32][64];
    char lastSender[64];
    usr_str map_descriptions[256];
    int map_descriptions_count;
    uint32_t noSent;
    int sentAQ;
    uint8_t netProblemOwner;
    char netProblemName[64];
    int32_t netProblem;
    uint32_t netAllOk;
    int update_time_norm;
    int flush_time_norm;
    int kickTime;
    uint32_t latencyCheck;
    uint32_t netProblemCount;
    uint32_t netAllOkCount;
    uint32_t deadCheck;
    uint32_t sendScore;
    netType2 players[8];
    netType1 players2[4];
    NC_STACK_button *confirm_button;
    int field_0x2fb4;

    EnvAction envAction;

    inp_key_setting keyConfig[46];

    int16_t field_3426;
    int16_t shelltrack;
    int16_t missiontrack;
    int16_t loadingtrack;
    int16_t debriefingtrack;
    audiotrack_adv shelltrack__adv;
    audiotrack_adv missiontrack__adv;
    audiotrack_adv loadingtrack__adv;
    audiotrack_adv debriefingtrack__adv;
    char snaps[32][256];
    int16_t snap_count;
    uint8_t cd;
    uint32_t last_cdchk;
    uint32_t lastInputEvent;
    uint32_t WaitForDemo;
    uint32_t netsend_count;
    uint32_t netrecv_count;
    uint32_t netrecv_time;
    uint32_t netsend_speed;
    uint32_t netrecv_speed;
    uint32_t netrecv_bpsMin;
    uint32_t netrecv_bpsMax;
    uint32_t netrecv_bpsAvr;
    uint32_t netsend_bpsMin;
    uint32_t netsend_bpsMax;
    uint32_t netsend_bpsAvr;
    uint32_t net_gcnt;
    uint32_t net_packet_size;
    uint32_t net_packet_min;
    uint32_t net_packet_max;
    uint32_t net_packet_cnt;
    uint32_t net_packet_avr;

public:
    void clear();

    void sb_0x46ca74();
    void sb_0x46cdf8();
    void sb_0x46aa8c();
    void sub_46C3E4();
    void ypaworld_func158__sub0__sub1();
    void ypaworld_func158__sub0__sub3();
    void yw_returnToTitle();
    void sb_0x46a8c0();
    void sb_0x46a8c0__sub0();
    void sub_46A3C0();
    void sub_46C914();
    void sub_46C748();
    void sub_46B0E0();
    void sub_46AA0C();

    void GameShellUiHandleInput();
    void sub_4DE248(int id);
    void sub_46D698();
    void yw_NetPrintStartInfo();
    void yw_CheckCDs();
    void sub_46B328();
    void yw_NetOKProvider();
    void yw_JoinNetGame();
    int ypaworld_func158__sub0__sub8(const char**, const char**);
    void ypaworld_func151__sub7();
    void yw_FractionInit();
    void yw_netcleanup();
    void sub_46DC1C();
    void sub_46D960();
    void ypaworld_func158__sub0__sub2();
    void GameShellUiOpenNetwork();
    int ypaworld_func158__sub0__sub7();
    void sub_46D9E0(int a2, const char *txt1, const char *txt2, int a5);
    void sub_46D2B4();
    void sub_457BC0();
    void ypaworld_func158__sub0__sub0();
    void sub_46C5F0(int a2);
    void  ypaworld_func158__sub0__sub5(int a2);
    void sub_46A7F8();
    void ypaworld_func158__sub0__sub4();

    bool  ShellSoundsLoad();

    static int KeyIndexFromConfig(uint32_t type, uint32_t index);

protected:
    bool LoadSample(int block, int sampleID, const std::string &file);
};

struct trec_bct
{
    uint32_t bact_id;
    vec3d pos;
    int8_t rot_x;
    int8_t rot_y;
    int8_t rot_z;
    int vp_id;
    int objType;
    int vhcl_id;
};

struct recorder
{
    enum OBJ_TYPE
    {
        OBJ_TYPE_NONE = 0,
        OBJ_TYPE_MISSILE = 1,
        OBJ_TYPE_VEHICLE = 2
    };

    IFFile *mfile;
    uint16_t seqn;
    uint16_t level_id;
    int32_t frame_id;
    int32_t time;
    uint32_t ctrl_bact_id;

    __NC_STACK_ypabact **bacts;
    trec_bct *oinf;
    uint16_t *sound_status;
    void *field_20;
    uint8_t *ainf;

    int32_t max_bacts;
    int32_t field_2C;
    int32_t bacts_count;
    int32_t field_34;
    int32_t ainf_size;
    int32_t do_record;
    int32_t field_40;
    vec3d field_44;
    mat3x3 rotation_matrix;
    int32_t field_74;
    int32_t field_78;
    int32_t field_7C;
    int32_t field_80;
    uint32_t field_84;
    char filename[64];
};

enum CELL_PFLAGS
{
    CELL_PFLAGS_IN_CLST = 1, // Sector in close list
    CELL_PFLAGS_IN_OLST = 2  // Sector in open list
};

struct cellArea : public nnode
{
    int pos_x;
    int pos_y;

    char addit_cost; // Additional cost for ground units
    char pf_flags; // Pathfind flags
    float cost_to_this;
    float cost_to_target;
    nlist pf_treelist;
    nnode pf_treenode;
    cellArea *pf_treeup;

    uint8_t owner;
    uint8_t type_id; // Index in array
    char comp_type; // Complex (3x3) or simple
    int energy_power; // Cell electric power
    uint8_t buildings_health[3][3];
    uint8_t view_mask; // Who can view this sector (mask)
    char w_type;
    uint8_t w_id;
    nlist units_list; // Units in this sector
    float height;
    float averg_height;
};

struct keysec
{
    int x;
    int y;
    cellArea *cell;

    keysec()
    {
    	clear();
    }

    void clear()
    {
    	x = 0;
    	y = 0;
    	cell = NULL;
    }
};

struct gateProto
{
    cellArea *pcell;
    int sec_x;
    int sec_y;
    int closed_bp;
    int opened_bp;
    int target_level_count;
    int target_levels[8];
    int keySectors_count;
    keysec keySectors[16];
    int mb_status;

    gateProto()
    {
    	clear();
    }

    void clear()
    {
    	pcell = NULL;
		sec_x = 0;
		sec_y = 0;
		closed_bp = 0;
		opened_bp = 0;
		target_level_count = 0;

		for (auto &x : target_levels)
			x = 0;

		keySectors_count = 0;

		for (auto &x : keySectors)
			x.clear();

		mb_status = 0;
    }
};

struct squadProto
{
    int field_0;
    int useable;
    int owner;
    int vehicle;
    int num;
    float pos_x;
    int field_18;
    float pos_z;
    int mb_status;

    squadProto()
    {
        clear();
    }

    void clear()
    {
        field_0 = 0;
        useable = 0;
        owner = 0;
        vehicle = 0;
        num = 0;
        pos_x = 0.;
        field_18 = 0;
        pos_z = 0.;
        mb_status = 0;
    }
};

struct MapRobo
{
    int16_t owner;
    int16_t vehicle;
    float pos_x;
    float pos_y;
    float pos_z;
    int energy;
    char con_budget;
    char rad_budget;
    char pow_budget;
    char def_budget;
    int reload_const;
    char saf_budget;
    char rec_budget;
    char cpl_budget;
    char rob_budget;
    int mb_status;
    int16_t viewangle;
    int saf_delay;
    int pow_delay;
    int rad_delay;
    int cpl_delay;
    int def_delay;
    int con_delay;
    int rec_delay;
    int rob_delay;

    MapRobo()
    {
        clear();
    }

    void clear()
    {
        owner = 0;
        vehicle = 0;
        pos_x = 0.;
        pos_y = 0.;
        pos_z = 0.;
        energy = 0;
        con_budget = 0;
        rad_budget = 0;
        pow_budget = 0;
        def_budget = 0;
        reload_const = 0;
        saf_budget = 0;
        rec_budget = 0;
        cpl_budget = 0;
        rob_budget = 0;
        mb_status = 0;
        viewangle = 0;
        saf_delay = 0;
        pow_delay = 0;
        rad_delay = 0;
        cpl_delay = 0;
        def_delay = 0;
        con_delay = 0;
        rec_delay = 0;
        rob_delay = 0;
    }
};

struct gemProto
{
    int16_t field_0;
    int16_t building;
    int16_t sec_x;
    int16_t sec_y;
    int mb_status;
    std::string script;
    std::string msg_default;
    int16_t nw_vproto_num_1;
    int16_t nw_vproto_num_2;
    int16_t nw_vproto_num_3;
    int16_t nw_vproto_num_4;
    int16_t nw_bproto_num_1;
    int16_t nw_bproto_num_2;
    int16_t nw_bproto_num_3;
    int16_t nw_bproto_num_4;
    Engine::StringList actions;
    int type;

    void clear()
    {
        field_0 = 0;
        building = 0;
        sec_x = 0;
        sec_y = 0;
        mb_status = 0;
        script.clear();
        msg_default.clear();
        nw_vproto_num_1 = 0;
        nw_vproto_num_2 = 0;
        nw_vproto_num_3 = 0;
        nw_vproto_num_4 = 0;
        nw_bproto_num_1 = 0;
        nw_bproto_num_2 = 0;
        nw_bproto_num_3 = 0;
        nw_bproto_num_4 = 0;
        actions.clear();
        type = 0;
    }

    gemProto()
    {
        clear();
    };
};

struct dbmapProto
{
    int16_t size_x;
    int16_t size_y;
    std::string name;

    dbmapProto()
    {
        clear();
    }

    void clear()
    {
        size_x = 0;
        size_y = 0;
        name.clear();
    }
};

struct mapProto
{
    char flags;
    int setNumber;
    int event_loop;
    int secXsize;
    int secYsize;
    int slow_connection;
    std::string sky;
    std::string typ;
    std::string own;
    std::string hgt;
    std::string blg;
    int mapRobos_count;
    MapRobo mapRobos[8];
    int squad_count;
    squadProto squads[96];
    std::string palettes[8];
    int mbmaps_count;
    dbmapProto mbmaps[4];
    int dbmap_count;
    dbmapProto dbmaps[4];

    int playerOwner; //Firsts host station owner, for correct XP brief

    mapProto()
    {
        clear();
    }

    void clear()
    {
        flags = 0;
        setNumber = 0;
        event_loop = 0;
        secXsize = 0;
        secYsize = 0;
        slow_connection = 0;

        sky.clear();
        typ.clear();
        own.clear();
        hgt.clear();
        blg.clear();

        mapRobos_count = 0;

        for (auto &r : mapRobos)
            r.clear();

        squad_count = 0;

        for (auto &s : squads)
            s.clear();

        for (auto &p : palettes)
            p.clear();

        mbmaps_count = 0;

        for (auto &m : mbmaps)
            m.clear();

        dbmap_count = 0;

        for (auto &d : dbmaps)
            d.clear();
    }
};

struct yw_buddy
{
    int commandid;
    int16_t type;
    int16_t field_6;
    int energy;

    yw_buddy()
    {
    	clear();
    }

    void clear()
    {
    	commandid = 0;
		type = 0;
		field_6 = 0;
		energy = 0;
    }
};

struct supetItemProto
{
    int type;
    int field_4;
    int countdown;
    cellArea *pcell;
    int sec_x;
    int sec_y;
    int inactive_bp;
    int active_bp;
    int trigger_bp;
    int keySectors_count;
    keysec keySectors[16];
    int mb_status;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
    int field_108;

    supetItemProto()
    {
    	clear();
    }

    void clear()
    {
    	type = 0;
		field_4 = 0;
		countdown = 0;
		pcell = NULL;
		sec_x = 0;
		sec_y = 0;
		inactive_bp = 0;
		active_bp = 0;
		trigger_bp = 0;
		keySectors_count = 0;

		for ( auto &x : keySectors )
			x.clear();

		mb_status = 0;
		field_EC = 0;
		field_F0 = 0;
		field_F4 = 0;
		field_F8 = 0;
		field_FC = 0;
		field_100 = 0;
		field_104 = 0;
		field_108 = 0;
    }
};

struct stru_2d90
{
    std::string map_name;
    int field_40;
    int levelID;
    int field_48;
    int field_4C;
    int amb_track_p0;
    int amb_track_p1;
    int amb_track_p2;
    int ownerMap__has_vehicles;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int buddies_count;
    yw_buddy buddies[128];
    int gate_count;
    gateProto gates[8];
    int supetItems_count;
    supetItemProto supetItems[8];
    int jodiefoster[8];
    std::string movie;
    std::string win_movie;
    std::string lose_movie;

    stru_2d90()
    {
        clear();
    }

    void clear()
    {
        map_name.clear();
		field_40 = 0;
		levelID = 0;
		field_48 = 0;
		field_4C = 0;
		amb_track_p0 = 0;
		amb_track_p1 = 0;
		amb_track_p2 = 0;
		ownerMap__has_vehicles = 0;
		field_60 = 0;
		field_64 = 0;
		field_68 = 0;
		field_6C = 0;
		field_70 = 0;
		field_74 = 0;
		buddies_count = 0;

		for ( auto &x : buddies )
			x.clear();

		gate_count = 0;

		for ( auto &x : gates )
			x.clear();

		supetItems_count = 0;

		for ( auto &x : supetItems )
			x.clear();

		for ( auto &x : jodiefoster )
			x = 0;

		movie.clear();
		win_movie.clear();
		lose_movie.clear();
    }

    void operator=(const stru_2d90 &b)
    {
        map_name = b.map_name;
		field_40 = b.field_40;
		levelID = b.levelID;
		field_48 = b.field_48;
		field_4C = b.field_4C;
		amb_track_p0 = b.amb_track_p0;
		amb_track_p1 = b.amb_track_p1;
		amb_track_p2 = b.amb_track_p2;
		ownerMap__has_vehicles = b.ownerMap__has_vehicles;
		field_60 = b.field_60;
		field_64 = b.field_64;
		field_68 = b.field_68;
		field_6C = b.field_6C;
		field_70 = b.field_70;
		field_74 = b.field_74;
		buddies_count = b.buddies_count;

		for (int i = 0; i < 128; i++)
			buddies[i] = b.buddies[i];

		gate_count = b.gate_count;

		for (int i = 0; i < 8; i++)
			gates[i] = b.gates[i];

		supetItems_count = b.supetItems_count;

		for (int i = 0; i < 8; i++)
			supetItems[i] = b.supetItems[i];

		for (int i = 0; i < 8; i++)
			jodiefoster[i] = b.jodiefoster[i];

		movie = b.movie;
		win_movie = b.win_movie;
		lose_movie = b.lose_movie;
    }
};

struct brf_obj
{
    int field_0;
    int object_id;
    int field_8;
    int field_C;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    float xpos;
    int field_24;
    float ypos;
    int field_2C;
    std::string title;

    brf_obj()
    {
    	clear();
    }

    void clear()
    {
    	field_0 = 0;
		object_id = 0;
		field_8 = 0;
		field_C = 0;
		field_10 = 0.;
		field_14 = 0.;
		field_18 = 0.;
		field_1C = 0.;
		xpos = 0.;
		field_24 = 0;
		ypos = 0.;
		field_2C = 0;
		title.clear();
    }
};

struct brf_t2
{
    float xpos;
    float ypos;
    int16_t field_8;
    int16_t vehicle_id;
    int field_C;
    int field_D;
    int field_E;
    std::string title;

    brf_t2()
    {
    	clear();
    }

    void clear()
    {
    	xpos = 0.;
		ypos = 0.;
		field_8 = 0;
		vehicle_id = 0;
		field_C = 0;
		field_D = 0;
		field_E = 0;
		title.clear();
    }
};

struct brf_t1
{
    int16_t field_0;
    int16_t last_vhcl;
    int16_t last_weapon;
    int16_t last_build;

    brf_t1()
    {
    	clear();
    }
    void clear()
    {
    	field_0 = 0;
		last_vhcl = 0;
		last_weapon = 0;
		last_build = 0;
    }
};

struct big_ypa_Brf
{
    NC_STACK_bitmap *mbmap_img;
    NC_STACK_bitmap *briefing_map;
    stru_2d90 s2d90;
    mapProto map_prototype;
    int briefStage;
    int field_2E6C;
    int activeElement;
    int elementsCount;
    int startTime;
    int currTime;
    int field_2E80;
    int field_2E84;
    std::string field_2E88;
    std::string LEVEL_BRIEF_INFO;
    brf_obj brf_objs;
    rstr_arg204 field_2F40;
    ua_fRect field_2F64;
    ua_fRect field_2F74;
    int field_2F84;
    int tp2_count;
    int mouseSelectedElement;
    int field_2F90;
    brf_t2 tp2[32];
    baseRender_msg field_4174;
    int field_419C;
    int field_41A0;
    NC_STACK_sklt *wireless_db[4];
    UAskeleton::Data *wireless_db_skels[4];
    Common::PlaneBytes *copy2_of_ownmap;
    Common::PlaneBytes *copy2_of_typmap;
    int field_41D4;
    int field_41D8;
    player_status copy_of_playerstatus[8];
    player_status field_42BC[8];
    std::string movie;
    int tp1_count;
    brf_t1 tp1[7];

    big_ypa_Brf()
    {
    	clear();
    }

    void clear()
    {
		mbmap_img = NULL;
		briefing_map = NULL;
		s2d90.clear();
		map_prototype.clear();
		briefStage = 0;
		field_2E6C = 0;
		activeElement = 0;
		elementsCount = 0;
		startTime = 0;
		currTime = 0;
		field_2E80 = 0;
		field_2E84 = 0;
		field_2E88.clear();
		LEVEL_BRIEF_INFO.clear();
		brf_objs.clear();
		field_2F40.clear();
		field_2F64 = ua_fRect();
		field_2F74 = ua_fRect();
		field_2F84 = 0;
		tp2_count = 0;
		mouseSelectedElement = 0;
		field_2F90 = 0;

		for (auto &x : tp2)
			x.clear();

		field_4174.clear();
		field_419C = 0;
		field_41A0 = 0;

		for (auto &x : wireless_db)
			x = NULL;

		for (auto &x : wireless_db_skels)
			x = NULL;

		copy2_of_ownmap = NULL;
		copy2_of_typmap = NULL;
		field_41D4 = 0;
		field_41D8 = 0;

		for (auto &x : copy_of_playerstatus)
			x.clear();

		for (auto &x : field_42BC)
			x.clear();

		movie.clear();
		tp1_count = 0;

		for (auto &x : tp1)
			x.clear();
    }
};

struct mapINFO
{
    int field_0;
    char mapPath[64];
    char map_name[64];
    ua_fRect field_9C;
    char robos_count;
    char fractions_mask;
    char secXsize;
    char secYsize;
    int slow_connection;
};

struct stru_LevelNet
{
	enum
	{
		MAX_NUM = 4
	};

	struct bkg_pct
	{
		int16_t size_x;
		int16_t size_y;
		std::string map_name;

	};

    int bg_n;
    bkg_pct background_map[MAX_NUM];
    bkg_pct rollover_map[MAX_NUM];
    bkg_pct finished_map[MAX_NUM];
    bkg_pct enabled_map[MAX_NUM];
    bkg_pct mask_map[MAX_NUM];
    bkg_pct tut_background_map[MAX_NUM];
    bkg_pct tut_rollover_map[MAX_NUM];
    bkg_pct tut_mask_map[MAX_NUM];
    bkg_pct menu_map[MAX_NUM];
    bkg_pct input_map[MAX_NUM];
    bkg_pct settings_map[MAX_NUM];
    bkg_pct network_map[MAX_NUM];
    bkg_pct locale_map[MAX_NUM];
    bkg_pct save_map[MAX_NUM];
    bkg_pct about_map[MAX_NUM];
    bkg_pct help_map[MAX_NUM];
    bkg_pct brief_map[MAX_NUM];
    bkg_pct debrief_map[MAX_NUM];
    mapINFO mapInfos[256];
    NC_STACK_bitmap *ilbm_menu_map;
    NC_STACK_bitmap *ilbm_mask_map;
    NC_STACK_bitmap *ilbm_rollover_map;
    NC_STACK_bitmap *ilbm_finished_map;
    NC_STACK_bitmap *ilbm_enabled_map;
    int field_BE38;
};

struct rgbiColor
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t i;
    uint32_t color;

    rgbiColor()
    {
        r = 0;
        g = 0;
        b = 0;
        i = 0;
        color = 0;
    }
};

class yw_movie : public std::string
{
public:
	bool _present;

	yw_movie() : _present(false) {};
	void clear()
	{
		std::string::clear();
		_present = false;
	}

	operator bool() const
	{
		return _present;
	}

	yw_movie& operator=(bool b)
	{
		_present = b;
		return *this;
	}

	yw_movie& operator=(const std::string &b)
	{
		std::string::operator=(b);
		return *this;
	}
};

struct slurp
{
    NC_STACK_base *skeletons_bas;
    UAskeleton::Data *skeleton_internal;

    slurp()
    {
        skeletons_bas = NULL;
        skeleton_internal = NULL;
    }
};

struct slurp2
{
    NC_STACK_sklt *skeleton;
    UAskeleton::Data *skeleton_internal;

    slurp2()
    {
        skeleton = NULL;
        skeleton_internal = NULL;
    }
};

struct sklt_wis
{
    int field_0;
    NC_STACK_sklt *sklts[14];
    UAskeleton::Data *sklts_intern[14];
    int field_72;
    int field_76;
    int field_7A;
    __NC_STACK_ypabact *field_7E;
    int field_82;
    float field_86;
    float field_8A;
    float field_8E;
    float field_92;
    int field_96;
    int field_9A;
    int field_9E;
    float cl1_r;
    float cl1_g;
    float cl1_b;
    float cl2_r;
    float cl2_g;
    float cl2_b;
};


struct save_status
{
    int p1;
    int p2;
    int16_t p3;
    int16_t p4;
    int16_t p5;
    int16_t p6;
    std::array<int, 8> pX;

    save_status()
    {
        clear();
    }

    void clear()
    {
        p1 = 0;
        p2 = 0;
        p3 = 0;
        p4 = 0;
        p5 = 0;
        p6 = 0;
        for (auto &t : pX)
            t = 0;
    }
};

struct yw_81cb
{
    int field_0;
    int field_4;
    char field_8[128];
    char field_C[128];
};

struct yw_samples
{
    int field_0;
    samples_collection1 field_4;
    NC_STACK_wav *field_35C;
    __NC_STACK_ypabact *field_360;
};

struct yw_f80
{
    int field_0;
    int field_4;
    int field_8;
    int x;
    int y;
    char ownerID2;
    uint8_t blg_ID;

    yw_f80()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0;
        x = 0;
        y = 0;
        ownerID2 = 0;
        blg_ID = 0;
    }
};

struct yw_field34
{
    int16_t x;
    int16_t y;
    cellArea *p_cell;
    int power;
    int power_2;
};

struct yw_f30
{
    char owner;
    uint8_t field_1;
};

struct yw_f726c
{
    nlist lst;
    int field_C;
    int field_10;
    int numNodes;
    uint8_t *last_bufStart;
    uint8_t *field_1C;
    uint8_t *last_bufEnd;

    int sub_47EDDC(int bufsize);
};

struct yw_f726c_nod : public nnode
{
    uint8_t *bufStart;
    uint8_t *bufEnd;
};

struct _NC_STACK_ypaworld
{
    NC_STACK_ypaworld *self_full;
    UserData *GameShell;
    base_64arg *b64_parms;
    int sectors_maxX;
    int sectors_maxY;
    int sectors_maxX2;
    int sectors_maxY2;
    cellArea *cells;

    float map_Width_meters;
    float map_Height_meters;
    yw_f30 *field_30;
    yw_field34 *field_34;
    int field_38;
    int field_3c;
    int set_number;
    NC_STACK_base *additionalSet;
    nlist bact_list;
    nlist dead_cache;
    vhclBases *vhcls_models;
    cityBases *legos;
    subSec *subSectors;
    secType *secTypes;
    VhclProto *VhclProtos;
    WeapProto *WeaponProtos;
    std::vector<BuildProto> BuildProtos;
    std::vector<roboProto> RoboProtos;
    yw_f80 field_80[8];
    int16_t build_hp_ref[256];
    uint8_t sqrt_table[64][64];
    __NC_STACK_ypabact *current_bact;
    vec3d field_1334;
    mat3x3 field_1340;
    NC_STACK_base *sky_loaded_base;
    int field_1368;
    NC_STACK_base *additionalBeeBox;
    NC_STACK_sklt *colsub_sklt;
    NC_STACK_sklt *colcomp_sklt;
    UAskeleton::Data *colsub_sklt_intrn;
    UAskeleton::Data *colcomp_sklt_intrn;
    NC_STACK_bitmap *tracyrmp_ilbm;
    NC_STACK_bitmap *shadermp_ilbm;
    NC_STACK_win3d *win3d;
    int field_138c;
    int str17_NOT_FALSE;
    slurp slurps1[6][6];
    slurp slurps2[6][6];
    slurp2 ColSide;
    slurp2 ColCross;
    int field_15e4;
    int field_15e8;
    int field_15ec;
    int field_15f0;
    int field_15f4;
    int field_15f8;
    int field_15fc;

    int audio_volume;
    int field_1604;
    int field_1608;
    int field_160c;
    int field_1610;
    int timeStamp;
    int field_1618;
    int field_161c;
    char *buildDate;
    int field_1624;
    int16_t field_1628;
    int16_t field_162A;
    int GUI_OK;
    tiles_stru *tiles[92];
    GuiBaseList field_17a0;
    int16_t screen_width;
    int16_t screen_height;

    int isDragging;
    GuiBase *draggingItem;
    shortPoint draggingPos;
    bool draggingLock;

    int field_17c0; // Grab mouse for unit steer-turn
    int field_17c4;
    int field_17c8;
    const char **tooltips;
    rgbiColor iniColors[World::COLOR_MAX_NUMBER];
    int field_1a00;
    int field_1a04;
    int field_1a08;
    int field_1a0c;
    int field_1a10;

    int field_1a1c;
    int field_1a20;
    int font_default_h;
    int font_default_w__a;
    int font_yscrl_bkg_w;
    int font_xscrl_h;
    int font_default_w__b;
    int field_1a38;
    int font_yscrl_h;
    int icon_order__w;
    int icon_order__h;
    int icon_help__w;
    int icon_help__h;
    int icon_energy__h;
    int icon0___h;
    int field_1a58;
    int field_1a5c;
    cellArea *field_1a60;
    int field_1a64;
    int field_1A66;
    int field_1a68; //Network?
    vec3d field_1a6c;

    vec3d field_1a7c;

    vec3d field_1a8c;
    __NC_STACK_ypabact *field_1a98;
    float field_1a9c;
    int field_1aa0;
    int field_1aa4;
    int field_1aa8;
    __NC_STACK_ypabact *field_1aac;
    int field_1ab0;
    int field_1ab4;
    vec3d field_1ab8;

    NC_STACK_bitmap *pointers[11];
    bitmap_intern *pointers__bitm[11];
    int field_1b1c;
    int field_1b20; // saved mouse x
    int field_1b22; // saved mouse y
    update_msg field_1b24;
    int16_t field_1b68; // debug info draw
    int16_t field_1B6A;
    int16_t field_1b6c;
    uint16_t field_1B6E;
    int field_1b70;
    int field_1b74;
    NC_STACK_ypabact *UserRobo;
    NC_STACK_ypabact *UserUnit;
    __NC_STACK_ypabact *URBact;
    __NC_STACK_ypabact *UUBact;
    nlist *field_1b88;
    int sectors_count_by_owner[8];
    int field_1bac[8];
    float field_1bcc[8];
    float field_1bec[8];
    __NC_STACK_ypabact *field_1c0c[512];
    int field_240c;
    int field_2410;
    int field_2414;
    int field_2418;
    uint32_t field_241c;
    __NC_STACK_ypabact *field_2420;
    int field_2424;
    int do_screenshooting;
    int screenshot_seq_id; //Screenshoting sequence id
    int screenshot_seq_frame_id; //Screenshoting frame id
    recorder *replayer; // For play replay
    recorder *sceneRecorder; // For record replay
    bact_hudi hudi;
    gemProto gems[8];
    int field_2b78;
    int field_2b7c;
    int last_modify_vhcl;
    int last_modify_weapon;
    int last_modify_build;

    stru_LevelNet *LevelNet;
    stru_2d90 *field_2d90;
    big_ypa_Brf brief;
    yw_f726c *history;
    int superbomb_wall_vproto;
    int superbomb_center_vproto;
    int field_7278;
    int field_727c;
    int field_7280;
    char lang_name[32];
    char *lang_strings;
    char *very_big_array__p_begin;
    char *lang_strings__end;
    char **string_pointers;
    char **string_pointers_p2;
    Common::PlaneBytes *typ_map;
    Common::PlaneBytes *own_map;
    Common::PlaneBytes *blg_map;
    Common::PlaneBytes *hgt_map;

    Common::PlaneBytes *copyof_typemap;
    Common::PlaneBytes *copyof_ownermap;
    sklt_wis wis_skeletons;
    int field_739A;

    uint32_t  field_73CE;

    save_status robo_map_status;
    save_status robo_finder_status;
    save_status vhcl_map_status;
    save_status vhcl_finder_status;
    int fxnumber;
    int dbg_num_sqd_counter[8];
    int dbg_num_vhcl_counter[8];
    int dbg_num_flk_counter[8];
    int dbg_num_robo_counter[8];
    int dbg_num_wpn_counter[8];
    int dbg_num_sqd;
    int dbg_num_sqd_max;
    int dbg_num_vhcl;
    int dbg_num_vhcl_max;
    int dbg_num_flk;
    int dbg_num_flk_max;
    int dbg_num_robo;
    int dbg_num_robo_max;
    int dbg_num_wpn;
    int dbg_num_wpn_max;
    NC_STACK_input *input_class;
    int field_7562;
    int field_7566;
    float field_756A;
    float field_756E;
    userdata_sample_info *field_7572;
    NC_STACK_windp *windp;
    uint32_t netUpdateTime;
    int isNetGame;
    uint32_t netGameStarted;
    int field_7586;

    int netInfoOverkill;

    int netStartTime;
    int netInterpolate;
    int field_759E;
    char field_75A2; //array 64?

    char field_75E2[64]; //array 64?


    int netgame_exclusivegem;
    int field_7626;

    int p_1_grp_cnt;
    int p_1_grp[4][8];
    std::array<player_status, 8> playerstatus;
    player_status ingamePlayerStatus[8];
    int maxroboenergy;
    int maxreloadconst;
    yw_samples *samples;
    int field_7882;
    int field_7886;
    int field_788A;
    float field_788E; //input sliders
    float field_7892; //input sliders
    float field_7896; //input sliders
    float field_789A; //input sliders
    float field_789E; //input sliders
    std::array<yw_movie, World::MOVIE_MAX_NUMBER> movies;
    int field_81AB;
    const char *field_81AF;
    const char *field_81B3;
    int one_game_res;
    int shell_default_res;
    int game_default_res;
    map_event *map_events;
    float max_impulse;
    yw_81cb field_81CB;
    float vehicle_sector_ratio_1;
    int unit_limit;
    int unit_limit_arg;
    int unit_limit_type;
    int unit_limit_1;
    int unit_limit_arg_1;
    int unit_limit_type_1;
    int field_826F;
    int TOD_ID;
    int beam_energy_start;
    int beam_energy_add;
    int beamenergy;
    int field_8283;
    int easy_cheat_keys;

    std::string initScriptLoc;
    int playerOwner;



    _NC_STACK_ypaworld();

    void SendCRC(int lvlid);
};

struct lego_xyz
{
    int field_0;
    float pos_x;
    float pos_y;
    float pos_z;
};

struct cityBases
{
    NC_STACK_base *base;
    NC_STACK_sklt *sklt_obj;
    UAskeleton::Data *sklt_obj_intern;
    UAskeleton::Data *selected_sklt_intern;
    char field_10;
    char field_11;
    char field_12;
    char field_13;
    uint8_t field_14[16];
    lego_xyz field_24[16];
};

struct subSec
{
    int build_health;
    uint8_t health_models[4]; //Building health models 0 - 100%hp, 3 - 0%hp
    int field_8;
};

struct secType
{
    char field_0;
    uint8_t field_1;
    char field_2;
    char field_3;
    subSec *buildings[3][3];
};


struct vhclSndFX
{
    std::string sample_name;
    std::array<std::string, 8> extSampleNames;
    NC_STACK_wav *single_sample;
    std::array<NC_STACK_wav *, 8> wavs;
    int16_t volume;
    int16_t pitch;
    sndFXprm sndPrm;
    sndFXprm2 sndPrm_shk;
    sndExtends extS;

    vhclSndFX()
    {
        clear();
    }

    void clear()
    {
        sample_name.clear();
        single_sample = NULL;

        for (auto &t : extSampleNames)
            t.clear();

        for (auto &t : wavs)
            t = NULL;

        volume = 0;
        pitch = 0;

        sndPrm.clear();
        sndPrm_shk.clear();
        extS.clear();
    }
};

struct VhclProto
{
    char model_id;
    char disable_enable_bitmask;
    int8_t weapon;
    int field_4;
    char mgun;
    uint8_t type_icon;
    std::string name;
    int16_t vp_normal;
    int16_t vp_fire;
    int16_t vp_dead;
    int16_t vp_wait;
    int16_t vp_megadeth;
    int16_t vp_genesis;
    std::array<destFX, 16> dest_fx;
    std::array<vhclSndFX, 12> sndFX;
    int vo_type;
    float max_pitch;
    int16_t field_1D6D;
    int16_t field_1D6F;
    int shield;
    int energy;
    int field_1D79;
    float adist_sector;
    float adist_bact;
    float sdist_sector;
    float sdist_bact;
    char radar;
    float mass;
    float force;
    float airconst;
    float maxrot;
    float height;
    float radius;
    float overeof;
    float vwr_radius;
    float vwr_overeof;
    float gun_angle;
    float fire_x;
    float fire_y;
    float fire_z;
    uint16_t destFxCount;
    int16_t num_weapons;
    float gun_power;
    float gun_radius;
    int kill_after_shot;
    float scale_fx_p0;
    float scale_fx_p1;
    float scale_fx_p2;
    int scale_fx_p3;
    std::array<int16_t, 32> scale_fx_pXX;
    char job_fighttank;
    char job_fighthelicopter;
    char job_fightflyer;
    char job_fightrobo;
    char job_conquer;
    char job_reconnoitre;
    NC_STACK_sklt *wireframe;
    NC_STACK_sklt *hud_wireframe;
    NC_STACK_sklt *mg_wireframe;
    NC_STACK_sklt *wpn_wireframe_1;
    NC_STACK_sklt *wpn_wireframe_2;
    IDVList initParams;

    VhclProto()
    {
        clear();
    }

    void clear()
    {
        model_id = 0;
        disable_enable_bitmask = 0;
        weapon = 0;
        field_4 = 0;
        mgun = 0;
        type_icon = 0;
        name.clear();
        vp_normal = 0;
        vp_fire = 0;
        vp_dead = 0;
        vp_wait = 0;
        vp_megadeth = 0;
        vp_genesis = 0;

        for (auto &x : dest_fx)
            x.clear();

        for (auto &x : sndFX)
            x.clear();

        vo_type = 0;
        max_pitch = 0.0;
        field_1D6D = 0;
        field_1D6F = 0;
        shield = 0;
        energy = 0;
        field_1D79 = 0;
        adist_sector = 0.0;
        adist_bact = 0.0;
        sdist_sector = 0.0;
        sdist_bact = 0.0;
        radar = 0;
        mass = 0.0;
        force = 0.0;
        airconst = 0.0;
        maxrot = 0.0;
        height = 0.0;
        radius = 0.0;
        overeof = 0.0;
        vwr_radius = 0.0;
        vwr_overeof = 0.0;
        gun_angle = 0.0;
        fire_x = 0.0;
        fire_y = 0.0;
        fire_z = 0.0;
        destFxCount = 0;
        num_weapons = 0;
        gun_power = 0.0;
        gun_radius = 0.0;
        kill_after_shot = 0;
        scale_fx_p0 = 0.0;
        scale_fx_p1 = 0.0;
        scale_fx_p2 = 0.0;
        scale_fx_p3 = 0;

        for (auto &x : scale_fx_pXX)
            x = 0;

        job_fighttank = 0;
        job_fighthelicopter = 0;
        job_fightflyer = 0;
        job_fightrobo = 0;
        job_conquer = 0;
        job_reconnoitre = 0;
        wireframe = NULL;
        hud_wireframe = NULL;
        mg_wireframe = NULL;
        wpn_wireframe_1 = NULL;
        wpn_wireframe_2 = NULL;

        initParams.clear();
    }
};

struct WeapProto
{
    char field_0;
    char enable_mask;
    int16_t model_id;
    char type_icon;
    std::string name;
    int16_t vp_normal;
    int16_t vp_fire;
    int16_t vp_dead;
    int16_t vp_wait;
    int16_t vp_megadeth;
    int16_t vp_genesis;
    int destFxCount;
    std::array<destFX, 16> dfx;
    vhclSndFX sndFXes[3];
    int field_870;
    int field_874;
    int energy;
    int field_87C;
    int life_time;
    int life_time_nt;
    int drive_time;
    int delay_time;
    int field_890;
    int field_894;
    int shot_time;
    int shot_time_user;
    int salve_shots;
    int salve_delay;
    float energy_heli;
    float energy_tank;
    float energy_flyer;
    float energy_robo;
    float radius_heli;
    float radius_tank;
    float radius_flyer;
    float radius_robo;
    float mass;
    float force;
    float airconst;
    float maxrot;
    int field_8D8;
    float radius;
    float overeof;
    float vwr_radius;
    float vwr_overeof;
    float start_speed;
    NC_STACK_sklt *wireframe;
    IDVList initParams;

    WeapProto()
    {
        clear();
    }

    void clear()
    {
        field_0 = 0;
        enable_mask = 0;
        model_id = 0;
        type_icon = 0;
        name.clear();
        vp_normal = 0;
        vp_fire = 0;
        vp_dead = 0;
        vp_wait = 0;
        vp_megadeth = 0;
        vp_genesis = 0;
        destFxCount = 0;

        for (auto &f: dfx)
            f.clear();

        for (auto &s: sndFXes)
            s.clear();

        field_870 = 0;
        field_874 = 0;
        energy = 0;
        field_87C = 0;
        life_time = 0;
        life_time_nt = 0;
        drive_time = 0;
        delay_time = 0;
        field_890 = 0;
        field_894 = 0;
        shot_time = 0;
        shot_time_user = 0;
        salve_shots = 0;
        salve_delay = 0;
        energy_heli = 0.0;
        energy_tank = 0.0;
        energy_flyer = 0.0;
        energy_robo = 0.0;
        radius_heli = 0.0;
        radius_tank = 0.0;
        radius_flyer = 0.0;
        radius_robo = 0.0;
        mass = 0.0;
        force = 0.0;
        airconst = 0.0;
        maxrot = 0.0;
        field_8D8 = 0;
        radius = 0.0;
        overeof = 0.0;
        vwr_radius = 0.0;
        vwr_overeof = 0.0;
        start_speed = 0.0;
        wireframe = NULL;
    }
};

struct buildSbact
{
    int sbact_vehicle;
    int field_4;
    float sbact_pos_x;
    float sbact_pos_y;
    float sbact_pos_z;
    int field_14;
    float sbact_dir_x;
    float sbact_dir_y;
    float sbact_dir_z;

    buildSbact()
    {
        clear();
    }

    void clear()
    {
        sbact_vehicle = 0;
        field_4 = 0;
        sbact_pos_x = 0.;
        sbact_pos_y = 0.;
        sbact_pos_z = 0.;
        field_14 = 0;
        sbact_dir_x = 0.;
        sbact_dir_y = 0.;
        sbact_dir_z = 0.;
    }
};

struct BuildProto
{
    uint8_t sec_type;
    uint8_t enable_mask;
    uint8_t model_id;
    uint8_t power;
    uint8_t type_icon;
    std::string name;
    int energy;
    vhclSndFX sndfx;
    std::array<buildSbact, 8> sbacts;

    BuildProto()
    {
        clear();
    }

    void clear()
    {
        sec_type = 0;
        enable_mask = 0;
        model_id = 0;
        power = 0;
        type_icon = 0;
        name.clear();
        energy = 0;
        sndfx.clear();

        for (auto &x : sbacts)
            x.clear();
    }
};

struct roboGun
{
    vec3d pos;
    vec3d dir;
    NC_STACK_ypabact *gun_obj;
    std::string robo_gun_name;
    uint8_t robo_gun_type;

    roboGun()
    {
        clear();
    }

    void clear()
    {
        pos = vec3d();
        dir = vec3d();
        gun_obj = NULL;
        robo_gun_name.clear();
        robo_gun_type = 0;
    }

    roboGun& operator=(const roboGun &b)
    {
        pos = b.pos;
        dir = b.dir;
        gun_obj = b.gun_obj;
        robo_gun_name = b.robo_gun_name;
        robo_gun_type = b.robo_gun_type;
        return *this;
    }
};

struct roboProto
{
    vec3d viewer;
    mat3x3 matrix;
    int field_30;
    int field_34;
    float robo_viewer_max_up;
    float robo_viewer_max_down;
    float robo_viewer_max_side;
    std::array<roboGun, 8> guns;
    int8_t robo_num_guns;
    vec3d dock;
    rbcolls coll;

    roboProto()
    {
        clear();
    }

    void clear()
    {
        viewer = vec3d();
        matrix = mat3x3();
        field_30 = 0;
        field_34 = 0;
        robo_viewer_max_up = 0.;
        robo_viewer_max_down = 0.;
        robo_viewer_max_side = 0.;

        for( auto &g : guns )
            g.clear();

        robo_num_guns = 0;
        dock = vec3d();
        coll.clear();
    }
};

int load_fonts_and_icons(_NC_STACK_ypaworld *yw);
int yw_LoadSet(_NC_STACK_ypaworld *yw, int setID);


struct yw_arg172
{
    const char *usertxt;
    const char *field_4;
    int field_8;
    UserData *usr;
    int field_10;
};

struct yw_arg181
{
    uamessage_base *data;
    int dataSize;
    const char *senderID;
    int senderFlags;
    char *recvID;
    int recvFlags;
    int garant;
};

struct yw_arg161
{
    int lvlID;
    int field_4;
};

struct yw_174arg
{
    int resolution;
    int make_changes;
};

struct yw_arg169
{
    UserData *usr;
    const char *saveFile;
};

struct ypaworld_arg146
{
    int vehicle_id;
    vec3d pos;
};

struct yw_130arg
{
    float pos_x;
    float pos_z;
    int sec_x;
    int sec_z;
    cellArea *pcell;
    float pos_x_cntr;
    float pos_y_cntr;
};

struct ypaworld_arg136
{
    vec3d stPos;
    vec3d vect;
    bool isect;
    float tVal;
    vec3d isectPos;
    int polyID;
    UAskeleton::Data *skel;
    int flags;

    ypaworld_arg136()
    {
        isect = false;
        tVal = 0.0;
        polyID = 0;
        skel = NULL;
        flags = 0;
    }
};

struct yw_137col
{
    vec3d pos1;
    vec3d pos2;
};

struct ypaworld_arg137
{
    int field_0;
    vec3d pos;
    int field_10;
    vec3d pos2;
    float radius;
    yw_137col *collisions;
    int coll_max;
    int coll_count;
    int field_30;
};

struct yw_arg180
{
    int effects_type;
    float field_4;
    float field_8;
    float field_C;
};

struct ypaworld_arg148
{
    int ownerID;
    int ownerID2;
    int blg_ID;
    int field_C;
    int x;
    int y;
    int field_18;
};

struct yw_arg129
{
    int field_0;
    vec3d pos;
    int field_10;
    int field_14;
    __NC_STACK_ypabact *unit;
};

struct yw_arg184
{
    char type;

    union
    {
        struct
        {
            int field_1;
        } t15;

        struct
        {
            int secX;
            int secY;
            uint8_t owner;
        } t26;

        struct
        {
            uint8_t field_1;
            int16_t field_2;
            uint8_t field_4;
            uint8_t field_5;
        } t34;

        struct
        {
            int secX;
            int secY;
            uint8_t owner;
            int field_4;
            int field_5;
            int16_t last_vhcl;
            int16_t last_weapon;
            int16_t last_build;
        } t7;
    };
};

struct yw_arg159
{
    __NC_STACK_ypabact *unit;
    int field_4;
    const char *txt;
    int field_C;
};

struct yw_arg176
{
    int owner;
    float field_4;
    float field_8;
};

struct yw_arg177
{
    __NC_STACK_ypabact *bact;
    int field_4;
};

struct yw_arg150
{
    __NC_STACK_ypabact *unit;
    int field_4;
    vec3d pos;
    int field_14;
    vec3d field_18;
    __NC_STACK_ypabact *field_24;
    float field_28;
};

struct yw_arg165
{
    int field_0;
    int frame;
};

class NC_STACK_ypaworld: public NC_STACK_base
{
friend class UserData;
friend class World::Parsers::UserParser;
friend class World::Parsers::SaveRoboParser;
friend class World::Parsers::SaveSquadParser;
friend class World::Parsers::SaveGemParser;
friend class World::Parsers::VhclProtoParser;
friend class World::Parsers::WeaponProtoParser;
friend class World::Parsers::BuildProtoParser;
friend class World::Parsers::SaveExtraViewParser;
friend class World::Parsers::SaveKwFactorParser;
friend class World::Parsers::SaveGlobalsParser;
friend class World::Parsers::SaveOwnerMapParser;
friend class World::Parsers::SaveBuildingMapParser;
friend class World::Parsers::SaveEnergyMapParser;
friend class World::Parsers::SaveLevelNumParser;
friend class World::Parsers::LevelStatusParser;
friend class World::Parsers::SaveHistoryParser;
friend class World::Parsers::SaveMasksParser;
friend class World::Parsers::SaveSuperBombParser;



public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual size_t func3(IDVList &stak);
    virtual size_t base_func64(base_64arg *arg);
    virtual void ypaworld_func129(yw_arg129 *arg);
    virtual size_t ypaworld_func130(yw_130arg *arg);
    virtual void ypaworld_func131(__NC_STACK_ypabact *bact);
    virtual void ypaworld_func132(void *arg);
    virtual void ypaworld_func133(void *arg);
    virtual void ypaworld_func134(NC_STACK_ypabact *bact);
    virtual void ypaworld_func135(void *arg);
    virtual void ypaworld_func136(ypaworld_arg136 *arg);
    virtual void ypaworld_func137(ypaworld_arg137 *arg);
    virtual void ypaworld_func138(void *arg);
    virtual void ypaworld_func139(GuiBase *lstvw);
    virtual void ypaworld_func140(GuiBase *lstvw);
    virtual void ypaworld_func143(void *arg);
    virtual void ypaworld_func144(NC_STACK_ypabact *bacto);
    virtual size_t ypaworld_func145(__NC_STACK_ypabact *bact);
    virtual NC_STACK_ypabact * ypaworld_func146(ypaworld_arg146 *vhcl_id);
    virtual NC_STACK_ypamissile * ypaworld_func147(ypaworld_arg146 *arg);
    virtual size_t ypaworld_func148(ypaworld_arg148 *arg);
    virtual void ypaworld_func149(ypaworld_arg136 *arg);
    virtual void ypaworld_func150(yw_arg150 *arg);
    virtual void ypaworld_func151();
    virtual void ypaworld_func153(bact_hudi *arg);
    virtual size_t ypaworld_func154(UserData *usr);
    virtual void ypaworld_func155(UserData *usr);
    virtual size_t ypaworld_func156(UserData *usr);
    virtual void ypaworld_func157(UserData *usr);
    virtual void ypaworld_func158(UserData *usr);
    virtual void ypaworld_func159(yw_arg159 *arg);
    virtual void ypaworld_func160(void *arg);
    virtual size_t ypaworld_func161(yw_arg161 *arg);
    virtual size_t ypaworld_func162(const char *fname);
    virtual void ypaworld_func163(base_64arg *arg);
    virtual void ypaworld_func164();
    virtual void ypaworld_func165(yw_arg165 *arg);
    virtual size_t ypaworld_func166(const char **langname);
    virtual void ypaworld_func167(UserData *usr);
    virtual size_t ypaworld_func168(__NC_STACK_ypabact **pbact);
    virtual size_t ypaworld_func169(yw_arg169 *arg);
    virtual size_t ypaworld_func170(yw_arg169 *arg);
    virtual size_t ypaworld_func171(yw_arg172 *arg);
    virtual size_t ypaworld_func172(yw_arg172 *arg);
    virtual size_t ypaworld_func173(UserData *usr);
    virtual size_t ypaworld_func174(yw_174arg *arg);
    virtual size_t ypaworld_func175(UserData *usr);
    virtual void ypaworld_func176(yw_arg176 *arg);
    virtual void ypaworld_func177(yw_arg177 *arg);
    virtual size_t ypaworld_func179(yw_arg161 *arg);
    virtual void ypaworld_func180(yw_arg180 *arg);
    virtual bool ypaworld_func181(void *arg);
    virtual void ypaworld_func182(void *arg);
    virtual size_t ypaworld_func183(yw_arg161 *arg);
    virtual void ypaworld_func184(yw_arg184 *arg);
    virtual void ypaworld_func185(void *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypaworld();
    virtual ~NC_STACK_ypaworld() {};

    virtual const char * getClassName()
    {
        return "ypaworld.class";
    };

    static NC_STACK_nucleus * newinstance()
    {
        return new NC_STACK_ypaworld();
    };

    enum YW_ATT
    {
        YW_ATT_MAPMAX_X = 0x80002000,
        YW_ATT_MAPMAX_Y = 0x80002001,
        YW_ATT_MAPSIZE_X = 0x80002002,
        YW_ATT_MAPSIZE_Y = 0x80002003,
//        YW_ATT_SECTORSIZE_X = 0x80002004,
//        YW_ATT_SECTORSIZE_Y = 0x80002005,
        YW_ATT_NORMVISLIMIT = 0x80002007,
        YW_ATT_FADELENGTH = 0x80002008,
        YW_ATT_SKYVISLIMIT = 0x80002009,
        YW_ATT_SKYFADELENGTH = 0x8000200A,
        YW_ATT_SKYHEIGHT = 0x8000200B,
        YW_ATT_SKYRENDER = 0x8000200C,
        YW_ATT_DOENERGYRECALC = 0x8000200D,
        YW_ATT_VISSECTORS = 0x8000200E,
        YW_ATT_USERHOST = 0x80002010,
        YW_ATT_USERVEHICLE = 0x80002011,
        YW_ATT_WPNPROTOS = 0x80002012,
        YW_ATT_BUILDPROTOS = 0x80002013,
        YW_ATT_VHCLPROTOS = 0x80002014,
        YW_ATT_LVLFINISHED = 0x80002015,
        YW_ATT_SCREEN_W = 0x80002016,
        YW_ATT_SCREEN_H = 0x80002017,
        YW_ATT_LOCALE_STRINGS = 0x80002018,
        YW_ATT_LVL_INFO = 0x8000201A,
        YW_ATT_DESTROY_FX = 0x8000201B,
        YW_ATT_PNET = 0x8000201C,
        YW_ATT_BUILD_DATE = 0x8000201D,
        YW_ATT_DONT_RENDER = 0x8000201E,
        YW_ATT_INVULNERABLE = 0x8000201F
    };

    virtual void setYW_normVisLimit(int);
    virtual void setYW_fadeLength(int);
    virtual void setYW_skyVisLimit(int);
    virtual void setYW_skyFadeLength(int);
    virtual void setYW_skyHeight(int);
    virtual void setYW_skyRender(int);
    virtual void setYW_doEnergyRecalc(int);
    virtual void setYW_visSectors(int);
    virtual void setYW_userHostStation(NC_STACK_ypabact *);
    virtual void setYW_userVehicle(NC_STACK_ypabact *);
    virtual void setYW_screenW(int);
    virtual void setYW_screenH(int);
    virtual void setYW_dontRender(int);

    virtual int getYW_mapMaxX();
    virtual int getYW_mapMaxY();
    virtual int getYW_mapSizeX();
    virtual int getYW_mapSizeY();
    virtual int getYW_normVisLimit();
    virtual int getYW_fadeLength();
    virtual int getYW_skyHeight();
    virtual int getYW_skyRender();
    virtual int getYW_doEnergyRecalc();
    virtual int getYW_visSectors();
    virtual NC_STACK_ypabact *getYW_userHostStation();
    virtual NC_STACK_ypabact *getYW_userVehicle();
    virtual WeapProto *getYW_weaponProtos();
    virtual BuildProto *getYW_buildProtos();
    virtual VhclProto *getYW_vhclProtos();
    virtual int getYW_lvlFinished();
    virtual int getYW_screenW();
    virtual int getYW_screenH();
    virtual char **getYW_localeStrings();
    virtual stru_2d90 *getYW_levelInfo();
    virtual int getYW_destroyFX();
    virtual NC_STACK_windp *getYW_pNET();
    virtual int getYW_invulnerable();

protected:
    int LVLoaderCommon(mapProto &mapp, int levelID, int a5);
    void FFeedback_Init();
    void FFeedback_StopAll();
    void FFeedback_VehicleChanged();
    void FFeedback_Update();

    void GUI_Close();

    void CameraPrepareRender(recorder *rcrd, __NC_STACK_ypabact *bact, struC5 *inpt);
    bool IsAnyInput(struC5 *struc);


    void GameShellUiOpenNetwork(); // On main menu "Multiplayer" press
    void GameShellBkgProcess();
    void GameShellBlitBkg(NC_STACK_bitmap *bitm);
    void GameShellInitBkgMode(int mode);
    bool GameShellInitBkg();

    int  ypaworld_func158__sub0__sub0__sub0();

public:
    void GuiWinToFront(GuiBase *);
    void GuiWinOpen(GuiBase *);
    void GuiWinClose(GuiBase *);


    void setIniColor(int color_id, int r, int g, int b, int i);
	bool ParseColorString(int color_id, const std::string &color_string);


	void SetFarView(bool farvw);


//protected:
	void sub_4491A0(const std::string &movie_fname);
    bool sub_4DA354(const std::string &filename);
    bool sb_0x4e1a88__sub0__sub0(mapProto *mapp, const std::string &fname);
    void ypaworld_func158__sub4__sub1();
    size_t ypaworld_func158__sub4__sub1__sub5();
    int ypaworld_func158__sub4__sub1__sub5__sub0(mapProto *mapproto, const std::string &filename);
    int sub_4DA41C(mapProto *mapp, const std::string &fname);
    int ypaworld_func158__sub4__sub1__sub3(int lvlid);
    int ypaworld_func158__sub4__sub1__sub3__sub0();
    void yw_ActivateWunderstein(cellArea *cell, int a3);
    void yw_InitTechUpgradeBuildings();
    void sub_4D12D8(int id, int a3);
    void sub_4D1594(int id);
    void sub_4D1444(int id);
    int ypaworld_func169__sub1(const std::string &filename);
    void ypaworld_func169__sub2();
    int ypaworld_func172__sub0(const std::string &fname, int parsers_mask);
    void listSaveDir(const std::string &saveDir);
    int yw_InitLevelNet();
    int yw_ParseWorldIni(const std::string &filename);
    bool sb_0x4e1a88__sub0(const std::string &fname, bool multiplayer);
    int sb_0x4e1a88(bool multiplayer);
    int yw_ScanLevels();
    int yw_RestoreVehicleData();
    void sub_471AB8();

public:
    //Data
    static const Nucleus::ClassDescr description;

    _NC_STACK_ypaworld ypaworld;


protected:

    /** On saved game load variables **/
    bool _extraViewEnable; // If you seat in robo gun
    int _extraViewNumber; // robo gun index

};

__NC_STACK_ypabact * sub_48C244(NC_STACK_ypaworld *ywo, int a2, char owner);

#endif
