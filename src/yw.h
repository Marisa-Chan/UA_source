#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include <string.h>
#include <array>
#include <string>

#include "types.h"
#include "memstream.h"

#include "system/gfx.h"
#include "system/inpt.h"
#include "system/sound.h"
#include "bitmap.h"
#include "ilbm.h"
#include "base.h"
#include "sklt.h"
#include "wav.h"
#include "ypabact.h"
#include "ypamissile.h"
#include "ypagun.h"

#include "lstvw.h"
#include "listnode.h"

#include "input.h"

#include "glob_funcs.h"

#include "world/consts.h"
#include "world/protos.h"
#include "world/tools.h"
#include "world/parsers.h"
#include "world/saveparsers.h"
#include "world/nparticle.h"

#include "gui/uamsgbox.h"

#include "world/luaevents.h"



#define YW_RENDER_SECTORS_DEF   5

class NC_STACK_ypaworld;
class NC_STACK_button;
class NC_STACK_windp;

struct cityBases;
struct subSec;
struct secType;
struct map_event;
struct uamessage_base;
struct uamessage_vhclData;
struct uamessage_vhclDataE;
struct uamessage_vhclDataI;


struct TSectorCollision;


namespace World
{
   
enum
{
    FRACTION_MAXCOUNT = 8,
};
    
enum BLIST_TYPES
{
    BLIST_UNITS = 1, // Used in world for store all bacts
    BLIST_CACHE = 2, // Used in world for store cached dead bacts
    BLIST_KIDS  = 3, // Used in bact class for store kid units
};

enum OWNERID
{
    OWNER_NOCHANGE  = -2,
    OWNER_RECALC    = -1,
    OWNER_UNKNOW    = 0,
};

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

enum INPUT_BIND
{
    INPUT_BIND_TYPE_BUTTON        = 1,
    INPUT_BIND_TYPE_SLIDER        = 2,
    INPUT_BIND_TYPE_HOTKEY        = 3,

    INPUT_BIND_PAUSE      = 1,
    INPUT_BIND_QUIT       = 2,
    INPUT_BIND_DRIVE_DIR  = 3,
    INPUT_BIND_DRIVE_SPEED= 4,
    INPUT_BIND_GUN_HEIGHT = 5,
    INPUT_BIND_FLY_HEIGHT = 6,
    INPUT_BIND_FLY_SPEED  = 7,
    INPUT_BIND_FLY_DIR    = 8,
    INPUT_BIND_BRAKE      = 9,
    INPUT_BIND_FIRE       = 10,
    INPUT_BIND_CAMFIRE    = 11,
    INPUT_BIND_GUN        = 12,
    INPUT_BIND_SET_COMM   = 13,
    INPUT_BIND_HUD        = 14,
    INPUT_BIND_AUTOPILOT  = 15,
    INPUT_BIND_ORDER      = 16,
    INPUT_BIND_NEW        = 17,
    INPUT_BIND_ADD        = 18,
    INPUT_BIND_SQ_MANAGE  = 19,
    INPUT_BIND_AGGR_1     = 20,
    INPUT_BIND_AGGR_2     = 21,
    INPUT_BIND_AGGR_3     = 22,
    INPUT_BIND_AGGR_4     = 23,
    INPUT_BIND_AGGR_5     = 24,
    INPUT_BIND_MAP        = 25,
    INPUT_BIND_WAPOINT    = 26,
    INPUT_BIND_LANDLAYER  = 27,
    INPUT_BIND_OWNER      = 28,
    INPUT_BIND_HEIGHT     = 29,
    INPUT_BIND_MINIMAP    = 30,
    INPUT_BIND_LOCKVIEW   = 31,
    INPUT_BIND_ZOOMIN     = 32,
    INPUT_BIND_ZOOMOUT    = 33,
    INPUT_BIND_LOG_WND    = 34,
    INPUT_BIND_CONTROL    = 35,
    INPUT_BIND_LAST_SEAT  = 36,
    INPUT_BIND_ATTACK     = 37,
    INPUT_BIND_TO_HOST    = 38,
    INPUT_BIND_TO_COMM    = 39,
    INPUT_BIND_NEXT_UNIT  = 40,
    INPUT_BIND_NEXT_COMM  = 41,
    INPUT_BIND_LAST_MSG   = 42,
    INPUT_BIND_TO_ALL     = 43,
    INPUT_BIND_HELP       = 44,
    INPUT_BIND_ANALYZER   = 45,

    INPUT_BIND_MAX        = 46,
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

    SOUND_ID_SELECT = 16,
    SOUND_ID_ERROR  = 17,
    SOUND_ID_ATTEN  = 18,
    SOUND_ID_SECRET = 19,
    SOUND_ID_PLASMA = 20,
    
    SOUND_ID_CHAT = 21,
    
    SOUND_ID_MAX
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

enum UPGRADE_TYPES
{
    UPGRADE_NONE       = 0,
    UPGRADE_WEAPON     = 1,
    UPGRADE_ARMOR      = 2,
    UPGRADE_VEHICLE    = 3,
    UPGRADE_BUILDING   = 4,
    UPGRADE_RADARE     = 5,
    UPGRADE_BLDVHCL    = 6,
    UPGRADE_GENERIC    = 7,
};



class AssignParser : public ScriptParser::DataHandler
{
public:
	virtual int Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2);
	virtual bool IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) { return !StriCmp(word, "begin_assign"); };

	static ScriptParser::DataHandler *MakeParser() {return new AssignParser;};
};


enum DOACTION
{
    DOACTION_0 = 0,
    DOACTION_1,
    DOACTION_2,
    DOACTION_ADD_UNIT1,
    DOACTION_ADD_UNIT2,
    DOACTION_5,
    DOACTION_6,
    DOACTION_7,
    DOACTION_8,
    DOACTION_9,
    DOACTION_10,
    DOACTION_WP_BEGIN,
    DOACTION_WP_ADD,
    DOACTION_WP_CYCLE,
    DOACTION_19
};




namespace Log
{
enum LOGMSG
{
    LOGMSG_UPGRADE_WEAPON   = 25,
    LOGMSG_UPGRADE_ARMOR    = 26,
    LOGMSG_UPGRADE_VEHICLE  = 27,
    LOGMSG_UPGRADE_BUILDING = 28,
    LOGMSG_UPGRADE_RADARE   = 78,
    LOGMSG_UPGRADE_BLDVHCL  = 79,
    LOGMSG_UPGRADE_GENERIC  = 80,
};

inline uint32_t GetUpgradeLogID(uint8_t upg)
{
    switch ( upg )
    {
    case UPGRADE_WEAPON:    return LOGMSG_UPGRADE_WEAPON;
    case UPGRADE_ARMOR:     return LOGMSG_UPGRADE_ARMOR;
    case UPGRADE_VEHICLE:   return LOGMSG_UPGRADE_VEHICLE;
    case UPGRADE_BUILDING:  return LOGMSG_UPGRADE_BUILDING;
    case UPGRADE_RADARE:    return LOGMSG_UPGRADE_RADARE;
    case UPGRADE_BLDVHCL:   return LOGMSG_UPGRADE_BLDVHCL;
    default:                return UPGRADE_GENERIC;
    }
}
}

}



#include "world/history.h"


struct usr_str
{
    int id = 0;
    std::string pstring;
};

struct audiotrack_adv
{
    int min_delay;
    int max_delay;
    audiotrack_adv() : min_delay(0), max_delay(0) {};
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
    enum
    {
        NETSCREEN_MODE_SELECT = 0,
        NETSCREEN_SESSION_SELECT = 1, // Only on dedicated server
        NETSCREEN_ENTER_NAME  = 2,
        NETSCREEN_CHOOSE_MAP  = 3,
        NETSCREEN_INSESSION = 4,
    };

public:
    struct TInputConf
    {
        uint8_t Type;
        int16_t KeyID;
        int16_t PKeyCode;
        int16_t NKeyCode;
        int16_t PKeyCodeBkp;
        int16_t NKeyCodeBkp;
        int16_t PKeyCodeDef;
        int16_t NKeyCodeDef;
        uint8_t SetFlags;
        
        TInputConf()
        : Type(0), KeyID(0), PKeyCode(0), NKeyCode(0)
        , PKeyCodeBkp(0), NKeyCodeBkp(0), PKeyCodeDef(0), NKeyCodeDef(0)
        , SetFlags(0) {};
        
        TInputConf(uint8_t type, int16_t keyID, int16_t pKey, int16_t nKey = 0)
        : Type(type), KeyID(keyID), PKeyCode(pKey), NKeyCode(nKey)
        , PKeyCodeBkp(pKey), NKeyCodeBkp(nKey), PKeyCodeDef(pKey), NKeyCodeDef(nKey)
        , SetFlags(0) {};
    };

public:
    bool HasInited = false;
    bool IgnoreScoreSaving = false;
    bool GameIsOver = false; // If it's opened after complete game
    bool ResetInputPeriod = false;
    std::string UserName;

    NC_STACK_ypaworld *p_YW = NULL;
    InputState *Input = NULL;
    int32_t DTime     = 0;
    uint32_t GlobalTime = 0;

    int EnvMode = 0; 
    bool EnvModeChanged = false;

    bool returnToTitle;

    TSndCarrier samples1_info;
    std::array<NC_STACK_sample *, World::SOUND_ID_MAX> samples1 = {{NULL}};
    
    NC_STACK_button *sub_bar_button;

    NC_STACK_button *titel_button;
    NC_STACK_button *button_input_button;
    GuiList input_listview;
    size_t field_D36;
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

    GFX::GfxMode _gfxMode;
    
    int _gfxModeIndex;
    GuiList d3d_listvw;
    char win3d_guid[100];
    char win3d_name[300];

    const char *field_139A;
    std::string field_139E;
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
    int _settingsChangeOptions;
    NC_STACK_button *disk_button;
    GuiList disk_listvw;
    int field_1612;
    std::string usernamedir;
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
    std::string netName;

    int16_t netNameCurPos;
    int16_t netLevelID;
    std::string netLevelName;

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
    int32_t latencyCheck;
    int32_t netProblemCount;
    int32_t netAllOkCount;
    uint32_t deadCheck;
    int32_t sendScore;
    netType2 players[8];
    netType1 players2[4];
    NC_STACK_button *confirm_button;
    Gui::UABlockMsgBox *_menuMsgBox = NULL;
    int field_0x2fb4;
    int _menuMsgBoxCode = 0;
    std::string _connString;

    EnvAction envAction;

    std::array<TInputConf, World::INPUT_BIND_MAX>  InputConfig;
    std::array<std::string, World::INPUT_BIND_MAX> InputConfigTitle;

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
    
    bool HideBlackSect = true;

public:
    void clear();

    void sb_0x46ca74();
    void sb_0x46cdf8();
    void sb_0x46aa8c();
    void sub_46C3E4();
    void ypaworld_func158__sub0__sub1();
    void ypaworld_func158__sub0__sub3();
    void yw_returnToTitle();
    void InputPageCancel();
    void InputConfCancel();
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
    void AfterMapChoose();
    void yw_NetOKProvider();
    void yw_JoinNetGame();
    void JoinLobbyLessGame();
    int ypaworld_func158__sub0__sub8(std::string *, std::string *);
    void ypaworld_func151__sub7();
    void yw_FractionInit();
    void yw_netcleanup();
    void sub_46DC1C();
    void sub_46D960();
    void ypaworld_func158__sub0__sub2();
    void GameShellUiOpenNetwork();
    int ypaworld_func158__sub0__sub7();
    void sub_46D9E0(int a2, const std::string &txt1, const std::string &txt2, int a5);
    void ShowMenuMsgBox(int code, const std::string &txt1, const std::string &txt2, bool OkOnly);
    void sub_46D2B4();
    void InputConfCopyToBackup();
    void InputConfigRestoreDefault();
    void sub_46C5F0();
    void  ypaworld_func158__sub0__sub5(int a2);
    void sub_46A7F8();
    void ypaworld_func158__sub0__sub4();

    bool  ShellSoundsLoad();

    static int InputIndexFromConfig(uint32_t type, uint32_t index);

protected:
    bool LoadSample(int sampleID, const std::string &file);
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

    IFFile mfileYOYO;
    uint16_t seqn;
    uint16_t level_id;
    int32_t frame_id;
    int32_t time;
    uint32_t ctrl_bact_id;

    NC_STACK_ypabact **bacts;
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

struct cellArea
{
    int32_t Id = 0;
    Common::Point PosID;

    char addit_cost; // Additional cost for ground units
    char pf_flags; // Pathfind flags
    float cost_to_this;
    float cost_to_target;
    cellArea *pf_treeup;

    uint8_t owner;
    int32_t type_id; // Index in array
    char comp_type; // Complex (3x3) or simple
    int32_t energy_power; // Cell electric power
    Common::PlaneArray<int16_t, 3, 3> buildings_health = {0};
    Common::PlaneArray<NC_STACK_base::Instance *, 3, 3> BldVPOpts = {NULL};
    uint8_t view_mask; // Who can view this sector (mask)
    char w_type;
    int32_t w_id;
    World::RefBactList unitsList; // Units in this sector
    float height;
    float averg_height;
    
    cellArea() : unitsList(this, NC_STACK_ypabact::GetCellRefNode) 
    { 
        BldVPOpts.fill(NULL);
        
        clear();
    };
    ~cellArea()
    {
        for (NC_STACK_base::Instance* opts : BldVPOpts)
        {
            if (opts)
                delete opts;
        }
    }
    
    bool IsCanSee(int owant) const
    {
        return (ViewMask(owant) & view_mask) != 0;
    }
    
    void AddToViewMask(int ownr)
    {
        view_mask |= ViewMask(ownr);
    }
    
    void DelFromViewMask(int ownr)
    {
        view_mask &= ~ViewMask(ownr);
    }
    
    inline static int32_t ViewMask(int ownr)
    {
        return (1 << ownr);
    }
    
    int GetEnergy()
    {
        if ( comp_type == 1 )
            return buildings_health.At(0, 0);

        int e = 0;
        for (auto hlth : buildings_health)
            e += hlth;
        
        return e;
    }
    
    void clear()
    {
        addit_cost = 0;
        pf_flags = 0;
        cost_to_this = 0.0;
        cost_to_target = 0.0;
        //nlist pf_treelist;
        //nnode pf_treenode;
        pf_treeup = NULL;

        owner = 0;
        type_id = 0;
        comp_type = 0;
        energy_power = 0;
        
        buildings_health.fill(0);
                
        view_mask = 0;
        w_type = 0;
        w_id = 0;
        
        unitsList.clear();
        
        height = 0.0;
        averg_height = 0.0;
    }
};

struct TCellFillerCh : TObjectCache
{
    int8_t Id1 = -1;
    int8_t Id2 = -1;
    float  Heights[4] = {0.};
};

struct MapKeySector
{
    int x;
    int y;
    cellArea *PCell;

    MapKeySector()
    {
    	clear();
    }

    void clear()
    {
    	x = 0;
    	y = 0;
    	PCell = NULL;
    }
};

struct MapGate
{
    cellArea *PCell;
    int SecX;
    int SecY;
    int ClosedBldID;
    int OpenBldID;
    std::vector<int> PassToLevels;
    std::vector<MapKeySector> KeySectors;
    int MbStatus;

    MapGate()
    {
    	clear();
    }

    void clear()
    {
    	PCell = NULL;
        SecX = 0;
        SecY = 0;
        ClosedBldID = 0;
        OpenBldID = 0;

        PassToLevels.clear();
        
        KeySectors.clear();

        MbStatus = 0;
    }
    
    operator Common::Point() const
    {
        return Common::Point(SecX, SecY);
    }
};

struct MapSquad
{
    bool Useable;
    int Owner;
    int VhclID;
    int Count;
    float X;
    float Z;
    int MbStatus;

    MapSquad()
    {
        clear();
    }

    void clear()
    {
        Useable = false;
        Owner = 0;
        VhclID = 0;
        Count = 0;
        X = 0.;
        Z = 0.;
        MbStatus = 0;
    }
};

struct MapRobo
{
    int16_t Owner;
    int16_t VhclID;
    vec3d Pos;
    int Energy;
    int ConBudget;
    int RadBudget;
    int PowBudget;
    int DefBudget;
    int ReloadConst;
    int SafBudget;
    int RecBudget;
    int CplBudget;
    int RobBudget;
    int MbStatus;
    int16_t ViewAngle;
    int SafDelay;
    int PowDelay;
    int RadDelay;
    int CplDelay;
    int DefDelay;
    int ConDelay;
    int RecDelay;
    int RobDelay;

    MapRobo()
    {
        Owner = 0;
        VhclID = 0;
        Pos = vec3d();
        Energy = 0;
        ConBudget = 0;
        RadBudget = 0;
        PowBudget = 0;
        DefBudget = 0;
        ReloadConst = 0;
        SafBudget = 0;
        RecBudget = 0;
        CplBudget = 0;
        RobBudget = 0;
        MbStatus = 0;
        ViewAngle = 0;
        SafDelay = 0;
        PowDelay = 0;
        RadDelay = 0;
        CplDelay = 0;
        DefDelay = 0;
        ConDelay = 0;
        RecDelay = 0;
        RobDelay = 0;
    }
};

struct MapGem
{
    int16_t BuildingID = 0;
    int16_t SecX = 0;
    int16_t SecY = 0;
    int MbStatus = 0;
    std::string ScriptFile;
    std::string MsgDefault;
    int16_t NwVprotoNum1 = 0;
    int16_t NwVprotoNum2 = 0;
    int16_t NwVprotoNum3 = 0;
    int16_t NwVprotoNum4 = 0;
    int16_t NwBprotoNum1 = 0;
    int16_t NwBprotoNum2 = 0;
    int16_t NwBprotoNum3 = 0;
    int16_t NwBprotoNum4 = 0;
    Engine::StringList ActionsList;
    int Type = 0;
    
    operator Common::Point() const { return Common::Point(SecX, SecY); };
};

struct TBkgPicInfo
{
    Common::Point Size;
    std::string PicName;
};

struct TLevelDescription
{
    enum BITS
    {
        BIT_SET = (1 << 0),
        BIT_SKY = (1 << 1),
        BIT_TYP = (1 << 2),
        BIT_OWN = (1 << 3),
        BIT_HGT = (1 << 4),
        BIT_BLG = (1 << 5),
        BIT_END = (1 << 6),
        
        BIT_ALL = (BIT_SET | BIT_SKY | BIT_TYP | BIT_OWN | BIT_HGT | BIT_BLG | BIT_END),
    };
    
    uint32_t ReadedPartsBits = 0;
    int32_t SetID = 0;
    int32_t EventLoopID = 0;
    Common::Point MapSize;
    bool SlowConnection = false;
    std::string SkyStr;
    std::string TypStr;
    std::string OwnStr;
    std::string HgtStr;
    std::string BlgStr;
    std::vector<MapRobo> Robos;
    std::vector<MapSquad> Squads;
    std::array<std::string, 8> Palettes;
    std::vector<TBkgPicInfo> Mbmaps;
    std::vector<TBkgPicInfo> Dbmaps;

    int32_t PlayerOwner = 0; //Firsts host station owner, for correct XP brief
    
    bool IsOk() const { return (ReadedPartsBits & BIT_ALL) == BIT_ALL; }
};

struct TMapBuddy
{
    int32_t CommandID = 0;
    int16_t Type = 0;
    int32_t Energy = 0;

    TMapBuddy() = default;
    
    TMapBuddy( int32_t cmdID, int16_t tp, int32_t e)
    : CommandID(cmdID), Type(tp), Energy(e)
    {}
};

struct TMapSuperItem
{
    enum
    {
        TYPE_BOMB = 1,
        TYPE_WAVE = 2,
        
        STATE_INACTIVE = 0,
        STATE_ACTIVE   = 1,
        STATE_STOPPED  = 2,
        STATE_TRIGGED  = 3,
    };
    
    int Type = 0;
    int State = STATE_INACTIVE;
    int32_t TimerValue = 0;
    cellArea *PCell = NULL;
    Common::Point Sector;
    int32_t InactiveBldID = 0;
    int32_t ActiveBldID = 0;
    int32_t TriggerBldID = 0;
    std::vector<MapKeySector> KeySectors;
    int MbStatus = 0;
    int32_t ActiveTime = 0;
    int32_t TriggerTime = 0;
    uint32_t ActivateOwner = 0;
    int32_t CountDown = 0;
    int32_t LastTenSec = 0;
    int32_t LastSec = 0;
    int32_t CurrentRadius = 0; // Current radius of the propagation wave
    int32_t LastRadius = 0;
};

struct TLevelInfo
{
    enum STATE
    {
        STATE_PLAYING    = 0,
        STATE_COMPLETED  = 1,
        STATE_ABORTED    = 2,
        STATE_PAUSED     = 3,
        STATE_RESTART    = 4,
        STATE_BRIEFING   = 5,
        STATE_SAVE       = 6,
        STATE_LOAD       = 7,
        STATE_MENU       = 8,
        STATE_DEBRIEFING = 9,
    };
    
    std::string MapName;
    int State = STATE_PLAYING;
    int LevelID = 0;
    int Mode = 0;      // 0 - Normal, 1 - Replay
    int GateCompleteID = 0; // Index of gate through level complete
    int MusicTrack = 0;
    int MusicTrackMinDelay = 0;
    int MusicTrackMaxDelay = 0;
    int OwnerMask = 0;
    int UserMask = 0;

    std::vector<TMapBuddy> Buddies;
    std::vector<MapGate> Gates;
    std::vector<TMapSuperItem> SuperItems;
    std::array<int, 8> JodieFoster = {{0}};
    std::string MovieStr;
    std::string MovieWinStr;
    std::string MovieLoseStr;
};


struct TBriefObject
{
    enum
    {
        TYPE_NONE    = 0,
        TYPE_SECTOR  = 1,
        TYPE_VEHICLE = 2
    };
    
    vec2d Pos;
    int16_t ObjType = TYPE_NONE;
    int16_t ID = 0;
    int TileSet = 0;
    int TileID = 0;
    int Color = 0;
    std::string Title;
    
    NC_STACK_base::Instance *VP = NULL; // Must not be copied

    TBriefObject() = default;
    
    TBriefObject(int16_t tp, int16_t oid, float sx, float sy, int tset, int tid, int clr, const std::string &ttl)
    {
    	Pos.x = sx;
        Pos.y = sy;
        ObjType = tp;
        ID = oid;
        TileSet = tset;
        TileID = tid;
        Color = clr;
        Title = ttl;
    }
    
    TBriefObject(const TBriefObject &b)
    {
    	Pos = b.Pos;
        ObjType = b.ObjType;
        ID = b.ID;
        TileSet = b.TileSet;
        TileID = b.TileID;
        Color = b.Color;
        Title = b.Title;
    }
    
    TBriefObject(TBriefObject &&b) = default; //Pointer also can be moved
    
    ~TBriefObject()
    {
        Common::DeleteAndNull(&VP);
    }
    
    TBriefObject &operator=(const TBriefObject &b)
    {
        Pos = b.Pos;
        ObjType = b.ObjType;
        ID = b.ID;
        TileSet = b.TileSet;
        TileID = b.TileID;
        Color = b.Color;
        Title = b.Title;
        Common::DeleteAndNull(&VP);
        return *this;
    }

    bool operator==(const TBriefObject &b) const
    {
        return ObjType == b.ObjType && ID == b.ID && Pos == b.Pos;
    }
    
    operator bool() const
    {
        return ObjType != TYPE_NONE;
    }
};

struct TBriefengScreen
{
    enum
    {
        STAGE_NONE        = 0,
        STAGE_PLAYLEVEL   = 1,
        STAGE_CANCEL      = 2,
        
        STAGE_LOADED      = 4,
        STAGE_SCALING     = 5,
        STAGE_SCALEEND    = 6,
        
        STAGE_PLAYER_ST   = 7,
        STAGE_PLAYER_RN   = 8,
        STAGE_PLAYER_END  = 9,
        
        STAGE_KEYS_ST     = 10,
        STAGE_KEYS_RN     = 11,
        STAGE_KEYS_END    = 12,
        
        STAGE_TECH_ST     = 13,
        STAGE_TECH_RN     = 14,
        STAGE_TECH_END    = 15,
        
        STAGE_ENMHS_ST    = 16,
        STAGE_ENMHS_RN    = 17,
        STAGE_ENMHS_END   = 18,
        
        STAGE_ENMFRC_ST   = 19,
        STAGE_ENMFRC_RN   = 20,
        STAGE_ENMFRC_END  = 21,
        
        STAGE_BUDDY_ST    = 22,
        STAGE_BUDDY_RN    = 23,
        STAGE_BUDDY_END   = 24,
        
        STAGE_GATE_ST     = 25,
        STAGE_GATE_RN     = 26,
        STAGE_GATE_END    = 27,
        
        STAGE_MOVIE       = 28,
        
        
        // TimerStatus
        TIMER_NORMAL      = 0,
        TIMER_STOP        = 1,
        TIMER_FAST        = 2,
        TIMER_RESTART     = 3,
    };
    
    NC_STACK_bitmap *MbmapImg = NULL;
    NC_STACK_bitmap *BriefingMapImg = NULL;
    TLevelDescription Desc;
    int Stage = STAGE_NONE;
    int TimerStatus = TIMER_NORMAL;
    int32_t ActiveElementID = 0; // In current heap
    int32_t ElementsCount = 0;
    int32_t StartTime = 0;
    int32_t CurrTime = 0;
    int32_t TextTime = 0;
    int32_t PreTextTime = 0;
    std::string ObjDescription;
    std::string BriefingText;
    
    GFX::rstr_arg204 MapBlitParams;
    Common::FRect MapBlitStart;
    Common::FRect MapBlitEnd;

    int32_t SelectedObjID = 0;
    
    bool AddObjectsFlag = false;
    TBriefObject ViewingObject;
    Common::FRect ViewingObjectRect;
    int32_t ViewingObjectAngle = 0;
    uint32_t ViewingObjectStartTime = 0;
    
    std::vector<TBriefObject> Objects;
    
    baseRender_msg ObjRenderParams;

    bool ZoomFromGate = false;
    
    std::array<NC_STACK_sklt *, 4> VectorGfx = {{NULL}};
    Common::PlaneBytes OwnMap;
    Common::PlaneBytes TypMap;
    //int _owner;
    uint32_t LastFrameTimeStamp = 0;
    std::array<World::TPlayerStatus, 8> StatsGlobal;
    std::array<World::TPlayerStatus, 8> StatsIngame;
    std::string MovieStr;
    std::vector<World::History::Upgrade> Upgrades;
    
    ~TBriefengScreen()
    {
        UnloadRes();
    }
    
    void UnloadRes()
    {
        if ( MbmapImg )
        {
            Nucleus::Delete(MbmapImg);
            MbmapImg = NULL;
        }

        if ( BriefingMapImg )
        {
            Nucleus::Delete(BriefingMapImg);
            BriefingMapImg = NULL;
        }
        
        for ( NC_STACK_sklt* &gfx : VectorGfx )
        {
            if (gfx)
            {
                Nucleus::Delete(gfx);
                gfx = NULL;
            }
        }
        
        OwnMap.Clear();

        TypMap.Clear();
    }

    void Clear()
    {
        UnloadRes();

        Desc = TLevelDescription();
        Stage = STAGE_NONE;
        TimerStatus = TIMER_NORMAL;
        ActiveElementID = 0;
        ElementsCount = 0;
        StartTime = 0;
        CurrTime = 0;
        TextTime = 0;
        PreTextTime = 0;
        ObjDescription.clear();
        BriefingText.clear();
        
        ViewingObject = TBriefObject();
        
        ViewingObjectRect = Common::FRect();
        ViewingObjectAngle = 0;
        ViewingObjectStartTime = 0;
        
        MapBlitParams = GFX::rstr_arg204();
        MapBlitStart = Common::FRect();
        MapBlitEnd = Common::FRect();
        AddObjectsFlag = false;
        SelectedObjID = 0;

        Objects.clear();

        ObjRenderParams = baseRender_msg();
        ZoomFromGate = false;

        //_owner = 0;
        LastFrameTimeStamp = 0;

        StatsGlobal.fill( World::TPlayerStatus() );
        StatsIngame.fill( World::TPlayerStatus() );

        MovieStr.clear();
        Upgrades.clear();
    }
};

struct TMapRegionInfo
{
    enum
    {
        STATUS_NONE      = 0,
        STATUS_DISABLED  = 1,
        STATUS_ENABLED   = 2,
        STATUS_COMPLETED = 3,
        STATUS_NETWORK   = 4
    };
    int Status = 0;
    std::string MapDirectory;
    std::string MapName;
    Common::FRect Rect;
    uint32_t RoboCount = 0;
    uint32_t FractionsBits = 0;
    Common::Point MapSize;
    bool SlowConnection = false;
};

struct TMapRegionsNet
{
    enum
    {
        MAX_NUM = 4
    };

    int NumSets = 0;
    std::array<TBkgPicInfo, MAX_NUM> background_map;
    std::array<TBkgPicInfo, MAX_NUM> rollover_map;
    std::array<TBkgPicInfo, MAX_NUM> finished_map;
    std::array<TBkgPicInfo, MAX_NUM> enabled_map;
    std::array<TBkgPicInfo, MAX_NUM> mask_map;
    std::array<TBkgPicInfo, MAX_NUM> tut_background_map;
    std::array<TBkgPicInfo, MAX_NUM> tut_rollover_map;
    std::array<TBkgPicInfo, MAX_NUM> tut_mask_map;
    std::array<TBkgPicInfo, MAX_NUM> menu_map;
    std::array<TBkgPicInfo, MAX_NUM> input_map;
    std::array<TBkgPicInfo, MAX_NUM> settings_map;
    std::array<TBkgPicInfo, MAX_NUM> network_map;
    std::array<TBkgPicInfo, MAX_NUM> locale_map;
    std::array<TBkgPicInfo, MAX_NUM> save_map;
    std::array<TBkgPicInfo, MAX_NUM> about_map;
    std::array<TBkgPicInfo, MAX_NUM> help_map;
    std::array<TBkgPicInfo, MAX_NUM> brief_map;
    std::array<TBkgPicInfo, MAX_NUM> debrief_map;
    
    std::array<TMapRegionInfo, 256> MapRegions;
    NC_STACK_bitmap *MenuImage = NULL;
    NC_STACK_bitmap *MaskImage = NULL;
    NC_STACK_bitmap *RolloverImage = NULL;
    NC_STACK_bitmap *FinishedImage = NULL;
    NC_STACK_bitmap *EnabledImage = NULL;
    
    size_t SelectedRegion = 0;
    
    ~TMapRegionsNet()
    {
        UnloadImages();
    }
    
    void UnloadImages();
};


struct sklt_wis
{
    int field_0;
    NC_STACK_sklt *sklts[14];
    UAskeleton::Data *sklts_intern[14];
    int field_72;
    int field_76;
    int field_7A;
    NC_STACK_ypabact *field_7E;
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

struct TSingleVoiceMessage
{
    int Priority = -1;
    TSndCarrier Carrier;
    NC_STACK_sample *Sample = NULL;
    NC_STACK_ypabact *Unit = NULL;
    
    TSingleVoiceMessage()
    {
        Carrier.Resize(1);
    }
    
    ~TSingleVoiceMessage()
    {
        SFXEngine::SFXe.StopCarrier(&Carrier);
        
        if (Sample)
            Nucleus::Delete(Sample);
        
    }
    
    void Reset()
    {
        SFXEngine::SFXe.StopCarrier(&Carrier);
        
        if (Sample)
            Nucleus::Delete(Sample);
        
        Carrier.Resize(1);
        Priority = -1;
        Sample = NULL;
        Unit = NULL;
    }
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

struct PowerStationRef
{
    Common::Point Cell; 
    cellArea *pCell = NULL;
    int32_t Power = 0;
    int32_t EffectivePower = 0;
    
    operator Common::Point() { return Cell; }
};

struct EnergyAccum
{
    uint8_t Owner = 0;
    int32_t Energy = 0;
};

struct lego_xyz
{
    int field_0 = 0;
    float pos_x = 0.0;
    float pos_y = 0.0;
    float pos_z = 0.0;
};

struct cityBases
{
    NC_STACK_base *base = NULL;
    NC_STACK_sklt *sklt_obj = NULL;
    UAskeleton::Data *sklt_obj_intern = NULL;
    UAskeleton::Data *selected_sklt_intern = NULL;
    char field_10 = 0;
    char field_11 = 0;
    char field_12 = 0;
    char field_13 = 0;
    std::array<uint8_t, 16> field_14 = {{0}};
    std::array<lego_xyz, 16> field_24;
};

struct subSec
{
    int build_health = 0;
    std::array<uint8_t, 4> health_models = {{0}}; //Building health models 0 - 100%hp, 3 - 0%hp
    int field_8 = 0;
};

struct secType
{
    char field_0 = 0;
    uint8_t field_1 = 0;
    char field_2 = 0;
    char field_3 = 0;
    Common::PlaneArray<subSec *, 3, 3> buildings = {0};
};





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
    const char *recvID;
    int recvFlags;
    int garant;
};

struct yw_arg161
{
    int lvlID;
    int field_4;
};

struct ypaworld_arg146
{
    size_t vehicle_id;
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
    int OwnerID;
    NC_STACK_ypabact *unit;
};

struct yw_arg159
{
    NC_STACK_ypabact *unit;
    int Priority;
    std::string txt;
    int MsgID;
};

struct yw_arg176
{
    int owner;
    float field_4;
    float field_8;
};

struct yw_arg177
{
    NC_STACK_ypabact *bact;
    int field_4;
};

struct yw_arg150
{
    NC_STACK_ypabact *unit;
    int field_4;
    vec3d pos;
    int field_14;
    vec3d field_18;
    NC_STACK_ypabact *field_24;
    float field_28;
};

struct yw_arg165
{
    int field_0;
    int frame;
};

struct TRenderingSector;

class NC_STACK_ypaworld: public NC_STACK_nucleus //NC_STACK_base
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
friend class World::Parsers::SaveLuaScriptParser;

public:
    enum
    {
        NUM_BUILD_PROTO = 128,
        NUM_VHCL_PROTO  = 256,
        NUM_WEAPON_PROTO = 128,
        NUM_ROBO_PROTO = 16,   //reserve
        
    };

public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t Process(base_64arg *arg);
    virtual void ypaworld_func129(yw_arg129 *arg);
    virtual size_t GetSectorInfo(yw_130arg *arg);
    virtual void ypaworld_func131(NC_STACK_ypabact *bact);
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
    virtual size_t ypaworld_func145(NC_STACK_ypabact *bact);
    virtual NC_STACK_ypabact * ypaworld_func146(ypaworld_arg146 *vhcl_id);
    virtual NC_STACK_ypamissile * ypaworld_func147(ypaworld_arg146 *arg);
    virtual size_t ypaworld_func148(ypaworld_arg148 *arg);
    virtual void ypaworld_func149(ypaworld_arg136 *arg);
    virtual void ypaworld_func150(yw_arg150 *arg);
    virtual void DeleteLevel();
    virtual void ypaworld_func153(bact_hudi *arg);
    virtual bool InitGameShell(UserData *usr);
    virtual void DeinitGameShell();
    virtual bool OpenGameShell();
    virtual void CloseGameShell();
    virtual void ProcessGameShell();
    virtual void ypaworld_func159(yw_arg159 *arg);
    virtual void ypaworld_func160(void *arg);
    virtual size_t ypaworld_func161(yw_arg161 *arg);
    virtual size_t ypaworld_func162(const char *fname);
    virtual void ypaworld_func163(base_64arg *arg);
    virtual void ypaworld_func164();
    virtual void ypaworld_func165(yw_arg165 *arg);
    virtual size_t ypaworld_func166(const std::string &langname);
    virtual void UpdateGameShell();
    virtual size_t ypaworld_func168(NC_STACK_ypabact *pbact);
    virtual size_t LoadGame(const std::string &saveFile);
    virtual size_t SaveGame(const std::string &saveFile);
    virtual size_t ypaworld_func171(yw_arg172 *arg);
    virtual size_t ypaworld_func172(yw_arg172 *arg, bool playIntro = false);
    virtual bool ReloadInput(size_t id);
    virtual size_t SetGameShellVideoMode(bool windowed);
    virtual size_t ReloadLanguage();
    virtual void ypaworld_func176(yw_arg176 *arg);
    virtual void ypaworld_func177(yw_arg177 *arg);
    virtual size_t ypaworld_func179(yw_arg161 *arg);
    virtual void ypaworld_func180(yw_arg180 *arg);
    virtual bool ypaworld_func181(yw_arg181 *arg);
    virtual void ypaworld_func182(void *arg);
    virtual size_t ypaworld_func183(yw_arg161 *arg);
    virtual void HistoryEventAdd(const World::History::Record &arg);
    virtual void ypaworld_func185(const void *arg);

    NC_STACK_ypaworld();
    virtual ~NC_STACK_ypaworld() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
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
        YW_ATT_BUILD_DATE = 0x8000201D,     //std::string
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

    virtual int getYW_mapSizeX();
    virtual int getYW_mapSizeY();
    
    Common::Point GetMapSize();
    
    virtual int getYW_normVisLimit();
    virtual int getYW_fadeLength();
    virtual int getYW_skyHeight();
    virtual int getYW_skyRender();
    virtual int getYW_doEnergyRecalc();
    virtual int getYW_visSectors();
    virtual NC_STACK_ypabact *getYW_userHostStation();
    virtual NC_STACK_ypabact *getYW_userVehicle();
    virtual std::vector<World::TWeapProto> &GetWeaponsProtos() { return WeaponProtos; };
    virtual std::vector<World::TBuildingProto> &GetBuildProtos() { return BuildProtos; };
    virtual std::vector<World::TVhclProto> &GetVhclProtos() { return VhclProtos; };
    virtual std::vector<World::TRoboProto> &GetRoboProtos() { return RoboProtos; };
    virtual int getYW_lvlFinished();
    virtual int getYW_screenW();
    virtual int getYW_screenH();
    virtual TLevelInfo &getYW_levelInfo();
    virtual int getYW_destroyFX();
    virtual NC_STACK_windp *getYW_pNET();
    virtual int getYW_invulnerable();

protected:
    int LevelCommonLoader(TLevelDescription *mapp, int levelID, int a5);
    void FFeedback_Init();
    void FFeedback_StopAll();
    void FFeedback_VehicleChanged();
    void FFeedback_Update();

    void GUI_Close();

    void CameraPrepareRender(recorder *rcrd, NC_STACK_ypabact *bact, InputState *inpt);
    bool IsAnyInput(InputState *struc);


    void GameShellUiOpenNetwork(); // On main menu "Multiplayer" press
    void GameShellBkgProcess();
    void GameShellBlitBkg(NC_STACK_bitmap *bitm);
    void GameShellInitBkgMode(int mode);
    bool GameShellInitBkg();

    int  InputConfigLoadDefault();

public:
    void GuiWinToFront(GuiBase *);
    void GuiWinOpen(GuiBase *);
    void GuiWinClose(GuiBase *);


    SDL_Color GetColor(int color_id);
    bool ParseColorString(int color_id, const std::string &color_string);


    void SetFarView(bool farvw);
    
    int TestVehicle(int protoID, int job);


//protected:
    void sub_4491A0(const std::string &movie_fname);
    bool LoadProtosScript(const std::string &filename);
    bool sb_0x4e1a88__sub0__sub0(TLevelDescription *mapp, const std::string &fname);
    void ypaworld_func158__sub4__sub1();
    bool InitDebrief();
    void FreeDebrief();
    int ypaworld_func158__sub4__sub1__sub5__sub0(TLevelDescription *mapproto, const std::string &filename);
    int sub_4DA41C(TLevelDescription *mapp, const std::string &fname);
    bool InitBriefing(int lvlid);
    void FreeBriefing();
    void FreeBriefDataSet();
    int ypaworld_func158__sub4__sub1__sub3__sub0();
    void yw_ActivateWunderstein(cellArea *cell, int a3);
    void yw_InitTechUpgradeBuildings();
    void sub_4D12D8(int id, int a3);
    void sub_4D1594(int id);
    void sub_4D1444(int id);
    int LoadingParseSaveFile(const std::string &filename);
    void LoadingUnitsRefresh();
    int ypaworld_func172__sub0(const std::string &fname, int parsers_mask);
    void listSaveDir(const std::string &saveDir);
    bool InitMapRegionsNet();
    int yw_ParseWorldIni(const std::string &filename);
    bool sb_0x4e1a88__sub0(const std::string &fname, bool multiplayer);
    int sb_0x4e1a88(bool multiplayer);
    int yw_ScanLevels();
    int yw_RestoreVehicleData();
    void EnableLevelPasses();
    int load_fonts_and_icons();
    int yw_LoadSet(int setID);
    
    void FreeLegos();
    void FreeFillers();
    
    void DrawMapRegionsTutorial();
    void DrawMapRegions();
    
    void RenderGame(base_64arg *bs64, int a2);
    void RenderAdditionalBeeBox(Common::Point sect, TRenderingSector *sct, baseRender_msg *bs77);
    void RenderSector(TRenderingSector *sct, baseRender_msg *bs77);
    void yw_renderSky(baseRender_msg *rndr_params);
    void RenderSuperItems(baseRender_msg *arg);
    void RenderSuperWave(vec2d pos, vec2d fromPos, baseRender_msg *arg);
    void RenderFillers(baseRender_msg *arg);
    
    void PrepareFiller(cellArea *sct, cellArea *sct2, float v9h, float v8h, bool vertical, TCellFillerCh *out, bool force = false);
    void PrepareAllFillers();
    
    bool IsVisibleMapPos(vec2d pos);
    
    //PRT - Position Robo Target
    void RefreshUnitPRT(NC_STACK_ypabact *unit, NC_STACK_ypabact *robo, bool isRobo); 
    NC_STACK_ypabact * sb_0x47b028__sub0(uint32_t bactid); // CHECK IT
    
    void yw_processVhclDataMsgs(uamessage_vhclData *msg, NC_STACK_ypabact *host_node);
    NC_STACK_ypabact * yw_getHostByOwner(uint8_t owner);
    void yw_netApplyVhclDataE(NC_STACK_ypabact *bact, uamessage_vhclDataE *dat, int id, uint32_t timestamp);
    void yw_netApplyVhclDataI(NC_STACK_ypabact *bact, uamessage_vhclDataI *dat, int id, uint32_t timestamp);
    void NetRemove(NC_STACK_ypabact *bct);
    void NetReleaseMissiles(NC_STACK_ypabact *bact);
    void sub_4F1BE8(NC_STACK_ypabact *bct);
    void BriefingSetObject(const TBriefObject &obj, bool doAdd);
    
    
    static TileMap * yw_LoadFont(const std::string &fontname);
    static TileMap * yw_LoadTileSet(const std::string &bitmap, Common::Point chrSz, Common::Point delta, Common::Point cr, Common::Point offset);
    NC_STACK_ypabact * FindBactByCmdOwn(int commandID, char owner);
    
    
    bool yw_createRobos(const std::vector<MapRobo> &Robos);
    bool yw_NetSetHostStations(const std::vector<MapRobo> &Robos);
    void yw_InitSquads(const std::vector<MapSquad> &squads);
    
    
    NC_STACK_ypabact *yw_createUnit(int model_id);
    void sb_0x456384(int x, int y, int ownerid2, int blg_id, int a7);
    void SetupPowerStationRef(const Common::Point &sec, int power);
    void sb_0x456384__sub0__sub0();
    
    void CellSetOwner(cellArea *cell, char secX, char secY, uint8_t owner);
    int ypaworld_func148__sub1(int id, int a4, int x, int y, int ownerID2, char blg_ID);
    
    void debug_info_draw(InputState *inpt);
    void debug_count_units();
    
    
    void SendCRC(int lvlid);
    
    void UpdateGuiSettings();
    void LoadGuiFonts();
    void CreateNewGuiElements();
    void DeleteNewGuiElements();
    
    void HistoryAktCreate(NC_STACK_ypabact *bact);
    void HistoryAktKill(NC_STACK_ypabact *bact);
    
    
    int sub_449678(InputState *struc, int kkode);
    
    bool yw_write_units(FSMgr::FileHandle *fil);
    bool yw_write_robo(NC_STACK_yparobo *robo, FSMgr::FileHandle *fil);
    
    void ypaworld_func64__sub6__sub0();
    
    void ypaworld_func64__sub19__sub2__sub0__sub0(uint8_t activate, float a5, float a6, float a7);
    void ypaworld_func64__sub19__sub2__sub0(int id);
    void ypaworld_func64__sub19__sub2(int id);
    void ypaworld_func64__sub19__sub1(int id);
    void ypaworld_func64__sub19__sub0(int id);
    void ypaworld_func64__sub19();
    void sub_4D16C4(int id);
    bool sub_4D1230(int id, int a3);
    bool sub_4D11C0(int id, int owner);
    bool sub_4D12A0(int owner);
    
    void sub_4C40AC();
    NC_STACK_ypabact *GetLastMsgSender();
    std::string GetLocaleString(int32_t id, const std::string &def) const;
    bool LngFileLoad(const std::string &filename);

    
    void ypaworld_func64__sub7(InputState *inpt);
    void ypaworld_func64__sub7__sub4(InputState *inpt);
    void ypaworld_func64__sub7__sub4__sub0(int a2);
    void ypaworld_func64__sub7__sub3__sub4(NC_STACK_ypabact *bact);
    bool ypaworld_func64__sub7__sub3__sub2();
    
    void yw_MouseSelect(InputState *arg);
    void ypaworld_func64__sub21__sub1__sub0(InputState *arg);
    
    void yw_MAP_MouseSelect(ClickBoxInf *winp);
    
    void ypaworld_func64__sub21__sub1__sub3__sub0(ClickBoxInf *winp);
    void yw_3D_MouseSelect(ClickBoxInf *winp);
    
    void ypaworld_func64__sub21(InputState *arg);
    void ypaworld_func64__sub21__sub7();
    float sub_4498F4();
    
    int ypaworld_func64__sub7__sub3__sub1(ClickBoxInf *winpt);
    void yw_SMAN_MouseSelect(ClickBoxInf *winp);
    void SquadManager_InputHandle(InputState *inpt);
    NC_STACK_ypabact * sub_4C7B0C(int sqid, int a3);
    
    bool recorder_create_camera();
    void recorder_updateObjectList(recorder *rcrd, float a5, int period);
    void recorder_updateObject(NC_STACK_ypabact *bact, trec_bct *oinf, uint16_t *ssnd, float a5, float a6);
    void recorder_set_bact_pos(NC_STACK_ypabact *bact, const vec3d &pos);
    int recorder_go_to_frame(recorder *rcrd, int wanted_frame_id);
    void recorder_store_bact(recorder *rcrd, World::RefBactList &bct_lst);
    void recorder_store_bact( recorder *rcrd, World::MissileList &bct_lst);
    void recorder_world_to_frame(recorder *rcrd);
    void recorder_write_frame();
    NC_STACK_ypabact *recorder_newObject(trec_bct *oinf);
    void ypaworld_func163__sub1(recorder *rcrd, int a3);
    
    void sub_4811E8(int id);
    void ypaworld_func64__sub1(InputState *inpt);
    void ypaworld_func64__sub21__sub5(int arg);
    
    
    bool ypaworld_func64__sub21__sub6(ClickBoxInf *winp);
    int ypaworld_func64__sub21__sub4(InputState *arg, int a3);
    int ypaworld_func64__sub21__sub3();
    int ypaworld_func64__sub21__sub2();
    int sb_0x4d3d44(ClickBoxInf *winp);
    void ypaworld_func64__sub7__sub6(InputState *inpt);
    
    int yw_MouseFindCreationPoint(ClickBoxInf *winp);
    
    void sb_0x4c87fc(const std::string &a2, GuiBase *lstvw);
    void sub_47DB04(char a2);
    void sub_449DE8(const std::string &a2);
    
    void LoadKeyNames();
    
    
    void InitGates();
    void UpdatePowerEnergy();
    void PowerStationErase(size_t id);
    void CellSetNewOwner(int secX, int secY, cellArea *cell, NC_STACK_ypabact *a5, int newOwner);
    void CellCheckHealth(cellArea *cell, int secX, int secY, int a5, NC_STACK_ypabact *a6);
    void InitBuddies();
    void InitSuperItems();
    bool LoadBlgMap(const std::string &mapName);
    bool LoadHightMap(const std::string &mapName);
    bool LoadOwnerMap(const std::string &mapName);
    bool LoadTypeMap(const std::string &mapName);
    
    void SetLangDefault();
    
    void PlayIntroMovie();    
    
    void SituationAnalyzer();
    
    cellArea *GetSector(int32_t x, int32_t y);
    cellArea *GetSector(const Common::Point &sec);
    cellArea *GetSector(size_t id);
    cellArea &SectorAt(int32_t x, int32_t y);
    cellArea &SectorAt(const Common::Point &sec);
    cellArea &SectorAt(size_t id);
    
    Common::PlaneVector<cellArea> &Sectors();
    
    void SetMapSize(const Common::Point &sz);
    bool IsGamePlaySector(const Common::Point &sz) const;
    bool IsSector(const Common::Point &sz) const;
    bool IsSectorBorder(const Common::Point &sz, int border) const;
    int sb_0x4f8f64__sub3__sub0(const Common::Point &sector);
    TSectorCollision sub_44DBF8(int _dx, int _dz, int _dxx, int _dzz, int flags);
    void sub_44E07C(TSectorCollision &arg);
    void sub_44D8B8(ypaworld_arg136 *arg, const TSectorCollision &loc);
    void ypaworld_func137__sub0(ypaworld_arg137 *arg, const TSectorCollision &a2);
    
    
    void VoiceMessageUpdate();
    void VoiceMessagePlayFile(const std::string &flname, NC_STACK_ypabact *unit, int a5);
    void VoiceMessagePlayMsg(NC_STACK_ypabact *unit, int priority, int msgID);
    void VoiceMessageCalcPositionToUnit();
    
    bool ProtosInit();
    void ProtosFreeSounds();
    
    
    void InitTooltips();
    
    World::ParticleSystem &ParticleSystem() { return _particles; };
    
    int32_t GetLegoBld(const cellArea *cell, int bldX, int bldY);
    int32_t GetLegoBld(const Common::Point &cell, int bldX, int bldY);
    
    void ClearOverrideModels();
    void LoadOverrideModels();

public:
    //Data
    static const Nucleus::ClassDescr description;
  
    
    
    UserData *GameShell;
    base_64arg *b64_parms;
    Common::Point _mapSize;

    Common::PlaneVector<cellArea> _cells;
    Common::PlaneVector<TCellFillerCh> _cellsVFCache;
    Common::PlaneVector<TCellFillerCh> _cellsHFCache;

    vec2d _mapLength;

    Common::PlaneVector<EnergyAccum> _energyAccumMap;
    std::vector<PowerStationRef> _powerStations;
    int _nextPSForUpdate;
    int set_number;
    NC_STACK_base *additionalSet;
    World::RefBactList _unitsList;
    World::RefBactList _deadCacheList;
    std::vector<NC_STACK_base *> vhcls_models;
    std::array<cityBases, 256> legos;
    std::array<subSec, 256> subSectors;
    std::array<secType, 256> secTypes;
    std::vector<World::TVhclProto> VhclProtos;
    std::vector<World::TWeapProto> WeaponProtos;
    std::vector<World::TBuildingProto> BuildProtos;
    std::vector<World::TRoboProto> RoboProtos;
    std::list<NC_STACK_base *> _Override;
    yw_f80 field_80[8];
    int16_t build_hp_ref[256];
    uint8_t sqrt_table[64][64];
    NC_STACK_ypabact *current_bact;
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
    int field_138c;
    int str17_NOT_FALSE;
    Common::PlaneArray<NC_STACK_base *, 6, 6> FillersHorizontal = {NULL};
    Common::PlaneArray<NC_STACK_base *, 6, 6> FillersVertical = {NULL};
    NC_STACK_skeleton *FillerSide = NULL;
    NC_STACK_skeleton *FillerCross = NULL;
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
    std::string buildDate;
    int field_1624;
    int16_t field_1628;
    int16_t field_162A;
    int GUI_OK;
    std::array<TileMap *, 92> tiles;
    GuiBaseList field_17a0;
    int16_t screen_width;
    int16_t screen_height;

    int isDragging;
    GuiBase *draggingItem;
    Common::Point draggingPos;
    bool draggingLock;

    int _mouseGrabbed; // Grab mouse for unit steer-turn
    int field_17c4;
    int field_17c8;
    std::vector<std::string> tooltips;
    std::array<SDL_Color, World::COLOR_MAX_NUMBER> _iniColors;
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
    NC_STACK_ypabact *field_1a98;
    float field_1a9c;
    int field_1aa0;
    int field_1aa4;
    int field_1aa8;
    NC_STACK_ypabact *field_1aac;
    int field_1ab0;
    int field_1ab4;
    vec3d field_1ab8;

    NC_STACK_bitmap *pointers[11];
    ResBitmap *pointers__bitm[11];
    int field_1b1c;
    int field_1b20; // saved mouse x
    int field_1b22; // saved mouse y
    update_msg field_1b24;
    int8_t _showDebugMode = 0; // debug info draw
    int32_t field_1B6A;
    int32_t field_1b6c;
    uint16_t field_1B6E;
    int field_1b70;
    int field_1b74;
    NC_STACK_ypabact *UserRobo;
    NC_STACK_ypabact *UserUnit;
    World::RefBactList *_UserRoboKidsList;
    int sectors_count_by_owner[8];
    int field_1bac[8];
    float field_1bcc[8];
    float field_1bec[8];
    NC_STACK_ypabact *_cmdrsRemap[512];
    int _activeCmdrID;
    int _activeCmdrRemapIndex;
    int _cmdrsCount;
    int _kidsCount;
    uint32_t field_241c;
    NC_STACK_ypabact *_lastMsgSender;
    int field_2424;
    int do_screenshooting;
    int screenshot_seq_id; //Screenshoting sequence id
    int screenshot_seq_frame_id; //Screenshoting frame id
    recorder *replayer; // For play replay
    recorder *sceneRecorder; // For record replay
    bact_hudi hudi;
    std::vector<MapGem> _Gems;
    int field_2b78;
    int field_2b7c;
    int last_modify_vhcl;
    int last_modify_weapon;
    int last_modify_build;

    TMapRegionsNet _mapRegions;
    TLevelInfo _levelInfo;
    
    TBriefengScreen brief;
    
    // History
    Common::BlocksStream _history;
    World::History::Frame _historyLastFrame;
    bool      _historyLastIsTimeStamp;
    
    int superbomb_wall_vproto;
    int superbomb_center_vproto;
    int field_7278;
    int field_727c;
    int field_7280;
    std::string _localeName;
    std::vector<std::string> _localeStrings;
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
    TSoundSource *field_7572;
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
    int _profile2DDraw;
    std::array<World::TPlayerStatus, 8> playerstatus;
    std::array<World::TPlayerStatus, 8> ingamePlayerStatus;
    int _maxRoboEnergy;
    int _maxReloadConst;
    TSingleVoiceMessage _voiceMessage;
    int field_7882;
    int field_7886;
    int field_788A;
    float field_788E; //input sliders
    float field_7892; //input sliders
    float field_7896; //input sliders
    float field_789A; //input sliders
    float field_789E; //input sliders
    std::array<std::string, World::MOVIE_MAX_NUMBER> movies;
    int field_81AB;
    std::string field_81AF;
    std::string field_81B3;
    bool one_game_res = false;
    int shell_default_res;
    int game_default_res;
    
    GFX::GfxMode _gfxMode;
    Common::Point _shellGfxMode;
    bool _gfxWindowed = false;

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
    int _currentBeamLoad;
    int easy_cheat_keys;

    std::string initScriptLoc;
    int playerOwner;
    
    std::string _luaScriptName;

protected:

    /** On saved game load variables **/
    bool _extraViewEnable; // If you seat in robo gun
    int _extraViewNumber; // robo gun index
    
    World::ParticleSystem _particles;

    World::LuaEvents *_script = NULL;
};

#endif
