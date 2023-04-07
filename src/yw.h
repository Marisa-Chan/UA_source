#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include <string.h>
#include <array>
#include <string>

#include "types.h"
#include "memstream.h"

#include "common/plane.h"

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

#include "world/consts.h"
#include "world/protos.h"
#include "world/tools.h"
#include "world/parsers.h"
#include "world/saveparsers.h"
#include "world/nparticle.h"

#include "gui/uamsgbox.h"

#include "world/luaevents.h"
#include "locale/locale.h"



#define YW_RENDER_SECTORS_DEF   5

class NC_STACK_ypaworld;
class NC_STACK_button;
class NC_STACK_windp;

struct TLego;
struct TSubSectorDesc;
struct TSectorDesc;
struct map_event;
struct uamessage_base;
struct uamessage_vhclData;
struct uamessage_vhclDataE;
struct uamessage_vhclDataI;


struct TSectorCollision;

namespace UIWidgets {
    enum MAIN_MENU_WIDGET_IDS{
        BTN_SAVE_LOAD = 1001,
        BTN_INPUT_SETTINGS = 1003,
        BTN_OPTIONS = 1004,
        BTN_QUIT = 1007,
        BTN_LANGUAGE = 1008,
        BTN_MULTIPLAYER = 1016,
        BTN_HELP = 1017,
        BTN_SINGLE_PLAYER = 1018
        
    };
    enum MAIN_MENU_EVENT_IDS{
        ALL_DOWN = 1251,
        BTN_SINGLE_PLAYER_UP = 1024,
        BTN_MULTIPLAYER_UP = 1022,
        BTN_INPUT_SETTINGS_UP = 1007,
        BTN_OPTIONS_UP = 1005,
        BTN_SAVE_LOAD_UP = 1001,
        BTN_LANGUAGE_UP = 1011,
        BTN_HELP_UP = 1025,
        BTN_QUIT_UP = 1013
    };
    enum NETWORK_MENU_WIDGET_IDS{
        TXT_MENU_TITLE = 1204,
        TXT_MENU_DESCR_LINE1 = 1222,
        TXT_MENU_DESCR_LINE2 = 1223,
        BTN_CREATE_SESSTION = 1202,
        TXTBOX = 1200
    };

}

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
    OWNER_0         = 0,
    OWNER_1         = 1,
    OWNER_RESIST    = OWNER_1,
    OWNER_RESIST_BIT = (1 << OWNER_RESIST),
    OWNER_2         = 2,
    OWNER_SULG      = OWNER_2,
    OWNER_SULG_BIT  = (1 << OWNER_SULG),
    OWNER_3         = 3,
    OWNER_MYKO      = OWNER_3,
    OWNER_MYKO_BIT  = (1 << OWNER_MYKO),
    OWNER_4         = 4,
    OWNER_TAER      = OWNER_4,
    OWNER_TAER_BIT  = (1 << OWNER_TAER),
    OWNER_5         = 5,
    OWNER_BLACK     = OWNER_5,
    OWNER_BLACK_BIT  = (1 << OWNER_BLACK),
    OWNER_6         = 6,
    OWNER_GHOR      = OWNER_6,
    OWNER_GHOR_BIT  = (1 << OWNER_GHOR),
    OWNER_7         = 7,
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
    PREF_JOYDISABLE     =  (1 << 0), //    4,
    PREF_FFDISABLE      =  (1 << 1), //    8,
    PREF_ENEMYINDICATOR =  (1 << 2), // 0x20,
    PREF_SOFTMOUSE      =  (1 << 3), // 0x40,
    PREF_CDMUSICDISABLE =  (1 << 4), // 0x100,
    PREF_ALTJOYSTICK    =  (1 << 5), // 0x200,
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
    GFX_FLAG_FARVIEW            = (1 << 0),
    GFX_FLAG_SKYRENDER          = (1 << 1),
    GFX_FLAG_SOFTMOUSE          = (1 << 2),
    GFX_FLAG_DRAWPRIMITIVES     = (1 << 3),
    GFX_FLAG_16BITTEXTURE       = (1 << 4),
    GFX_FLAG_WINDOWED           = (1 << 5),
};

enum SAVE_DATA_FLAG
{
    SDF_USER     = (1 << 0),
    SDF_INPUT    = (1 << 1),
    SDF_VIDEO    = (1 << 2),
    SDF_SOUND    = (1 << 3),
    SDF_SCORE    = (1 << 4),
    SDF_SHELL    = (1 << 5),
    SDF_PROTO    = (1 << 6),
    SDF_BUDDY    = (1 << 7),
    
    SDF_ALL      = (SDF_USER | SDF_INPUT | SDF_VIDEO | SDF_SOUND | SDF_SCORE | SDF_SHELL | SDF_PROTO | SDF_BUDDY),
};

enum SOUND_FLAGS
{
    SF_INVERTLR  = (1 << 0),
    SF_CDSOUND   = (1 << 4),
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







struct ProfilesNode
{
    int totalElapsedTime = 0;
    uint8_t fraction = 0;
    std::string name;
};

typedef std::list<ProfilesNode> ProfileList;




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
        ACTION_REPLAY     = 5
    };

    int8_t action = ACTION_NONE;
    int32_t params[5] = {0};
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
        NETSCREEN_ENTER_IP = 5,
        NETSCREEN_CHOOSE_MAP  = 3,
        NETSCREEN_INSESSION = 4,
    };
    
    enum
    {
        ICHG_JOYSTICK       = (1 << 0),
        ICHG_FORCEFEEDBACK  = (1 << 1),
        ICHG_ALTJOYSTICK    = (1 << 2)
    };

public:
    struct TInputConf
    {
        uint8_t Type = 0;
        int16_t KeyID = 0;
        int16_t PKeyCode = 0;
        int16_t NKeyCode = 0;
        int16_t PKeyCodeBkp = 0;
        int16_t NKeyCodeBkp = 0;
        int16_t PKeyCodeDef = 0;
        int16_t NKeyCodeDef = 0;
        uint8_t SetFlags = 0; // Flags what key is setted (first/second)
        
        TInputConf() = default;        
        TInputConf(uint8_t type, int16_t keyID, int16_t pKey, int16_t nKey = 0)
        : Type(type), KeyID(keyID), PKeyCode(pKey), NKeyCode(nKey)
        , PKeyCodeBkp(pKey), NKeyCodeBkp(nKey), PKeyCodeDef(pKey), NKeyCodeDef(nKey)
        {};
        
        enum {
            IF_FIRST  = (1 << 0),
            IF_SECOND = (1 << 1)
        };
    };

public:
    bool HasInited = false;
    bool IgnoreScoreSaving = false;
    bool GameIsOver = false; // If it's opened after complete game
    bool ResetInputPeriod = false;
    std::string UserName;

    NC_STACK_ypaworld *p_YW = NULL;
    TInputState *Input = NULL;
    int32_t DTime     = 0;
    uint32_t GlobalTime = 0;

    int EnvMode = 0; 
    bool EnvModeChanged = false;

    bool returnToTitle;

    TSndCarrier samples1_info;
    std::array<NC_STACK_sample *, World::SOUND_ID_MAX> samples1 = Common::ArrayInit<NC_STACK_sample *, World::SOUND_ID_MAX>(NULL);
    
    NC_STACK_button *sub_bar_button;

    NC_STACK_button *titel_button;
    NC_STACK_button *button_input_button;
    GuiList input_listview;
    int32_t inpListActiveElement;
    bool confFirstKey; // What key we will config for sliders (first or second)
    bool joystickEnabled;
    bool confJoystickEnabled; // Apply this to joystickEnabled on press "OK"
    bool altJoystickEnabled;
    bool confAltJoystickEnabled; // Apply this to altJoystickEnabled on press "OK"
    bool confFFEnabled;
    bool keyCatchMode; // mode for catch input to configure key
    uint8_t inputChangedParts;
    
    NC_STACK_button *video_button;
    GuiList video_listvw;
    int game_default_res;

    GFX::GfxMode _gfxMode;
    
    int _gfxModeIndex;
    GuiList d3d_listvw;
    
    std::string win3d_guid;
    std::string win3d_name;
    std::string conf3DGuid;
    std::string conf3DName;
    
    int16_t fxnumber;
    int16_t confFxNumber;
    uint8_t GFXFlags;
    uint8_t confGFXFlags;

    
    uint8_t soundFlags;
    uint8_t confSoundFlags;
    
    int16_t soundVolume;
    int16_t confSoundVolume;
    
    int16_t musicVolume;
    int16_t confMusicVolume;
    
    bool enemyIndicator;
    bool confEnemyIndicator;
    
    int _settingsChangeOptions;
    
    NC_STACK_button *disk_button;
    GuiList disk_listvw;
    int32_t diskListActiveElement;
    
    std::string userNameDir;
    int16_t userNameDirCursor;
    
    int16_t diskScreenMode;
    
    ProfileList profiles;

    uint8_t savedDataFlags; // SDF flags - data types loaded from settings

    bool diskEnterFromMapSelect;
    
    NC_STACK_button *locale_button;
    Engine::StringList lang_dlls;
    GuiList local_listvw;
    std::string *default_lang_dll;
    std::string *prev_lang;
    
    NC_STACK_button *about_button;
    uint32_t aboutDlgLastKeyTime;
    int8_t aboutDlgKeyCount;

    NC_STACK_button *network_button;
    GuiList network_listvw;

    int16_t netListY;

    int16_t netSelMode;
    int16_t netSel;
    int nInputMode;
    std::string netName;

    int16_t netNameCurPos;
    int16_t netLevelID;
    std::string netLevelName;

    std::string netPlayerName;
    uint32_t netCRC;
    int32_t takTime;
    uint8_t netPlayerOwner;
    uint8_t FreeFraction;
    uint8_t SelectedFraction;
    
    bool netGameCanStart;
    bool isHost;
    bool modemAskSession;

    uint32_t msgcount;
    bool isWelcmd;
    bool rdyStart;
    bool remoteMode;
    bool disconnected;
    bool blocked;
    
    int32_t problemTimer;

    std::deque<std::string> msgBuffers;
    std::string lastSender;

    struct TMapDescription
    {
        int id = 0;
        std::string pstring;
        
        TMapDescription() = default;
        TMapDescription(int __id, const std::string &__pstring)
        : id(__id), pstring(__pstring) {};
        
        static bool compare(const TMapDescription &a1, const TMapDescription &a2)
        {
            return StriCmp(a1.pstring, a2.pstring) > 0;
        }
    };
    
    std::vector< TMapDescription > mapDescriptions;

    bool noSent;
    bool sentAQ; // sended annonce of quit
    
    uint8_t netProblemOwner;
    std::string netProblemName;
    int32_t netProblem;
    int32_t netAllOk;
    int32_t update_time_norm;
    int32_t flush_time_norm;
    int32_t kickTime;
    int32_t latencyCheck;
    int32_t netProblemCountDown;
    int32_t netAllOkCountDown;
    uint32_t deadCheckTime;
    int32_t sendScoreCountDown;
    
    struct TNetPlayerLobbyData
    {
        std::string Name;
        std::string Msg;
        uint8_t  NetFraction = 0;
        bool     IsTrouble   = false;
        uint8_t  Owner       = 0;
        bool     Ready       = false;
        bool     Welcomed    = false;
        int32_t  UpdateCountDown = 0;
        uint32_t DataChecksum   = 0;
    };

    struct TNetPlayerData
    {
        std::string Name;
        uint8_t  Owner       = 0;
        bool     Ready       = false;
        uint8_t DestroyFlags = 0;
        uint8_t Status       = 0;
        bool ConnectionProblem = false;
        uint32_t LastMessageTime = 0;
        uint32_t TimeStamp   = 0;
        uint32_t MessageCount = 0;
        int32_t ProblemCountDown = 0;
        int32_t Latency      = 0;
    };
    
    
    std::array<TNetPlayerData, World::CVFractionsCount> netPlayers;
    std::array<TNetPlayerLobbyData, World::CVMaxNetPlayers> lobbyPlayers;
    
    
    NC_STACK_button *confirm_button;
    Gui::UABlockMsgBox *_menuMsgBox = NULL;
    
    uint8_t confirmMode;
    int _menuMsgBoxCode = 0;

    EnvAction envAction;

    std::array<TInputConf, World::INPUT_BIND_MAX>  InputConfig;
    std::array<std::string, World::INPUT_BIND_MAX> InputConfigTitle;

    /* SGM save exist
       0 - not checked
       1 - exist
       2 - not exist, but checked*/
    int8_t sgmSaveExist = 0; 
    
    int16_t shelltrack;
    int16_t missiontrack;
    int16_t loadingtrack;
    int16_t debriefingtrack;
    
    struct TAudioTrackDelays
    {
        int32_t min_delay = 0;
        int32_t max_delay = 0;
    };
    
    TAudioTrackDelays shelltrack__adv;
    TAudioTrackDelays missiontrack__adv;
    TAudioTrackDelays loadingtrack__adv;
    TAudioTrackDelays debriefingtrack__adv;
    
    std::vector<std::string> snaps;

    uint32_t lastInputEvent = 0;
    
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
    void ShowSaveLoadMenu();
    void ShowInputSettings();
    void ShowLanguageMenu();
    void yw_returnToTitle();
    void InputPageCancel();
    void InputConfCancel();
    void sub_46A3C0();
    void sub_46C914();
    void sub_46C748();
    void sub_46B0E0();
    void ExitFromLanguageMenu();

    void GameShellUiHandleInput();
    void ConnectToServer(std::string connStr);
    void ShowToolTip(int id);
    void ExitFromNetworkToMain();
    void yw_NetPrintStartInfo();

    void AfterMapChoose();
    void yw_NetOKProvider();
    void yw_JoinNetGame();
    void JoinLobbyLessGame();
    void ypaworld_func151__sub7();
    void yw_FractionInit();
    void yw_netcleanup();
    void sub_46DC1C();
    void sub_46D960();
    void ShowOptionsMenu();
    void GameShellUiOpenNetwork();
    int ypaworld_func158__sub0__sub7();
    void ShowConfirmDialog(int a2, const std::string &txt1, const std::string &txt2, int a5);
    void ShowMenuMsgBox(int code, const std::string &txt1, const std::string &txt2, bool OkOnly);
    void sub_46D2B4();
    void InputConfCopyToBackup();
    void InputConfigRestoreDefault();
    void sub_46C5F0();
    void  UpdateSelected3DDevFromList();
    void sub_46A7F8();
    void ShowAbout();
    
    bool SaveBuildProtoState();
    void SaveSettings();
    

    bool  ShellSoundsLoad();

    static int InputIndexFromConfig(uint32_t type, uint32_t index);
    static bool IsHasSGM(const std::string &username, int id);
    static bool IsHasRestartForLevel(const std::string &username, int id);
    
    bool IsWindowedFlag()
    {
        return (GFXFlags & World::GFX_FLAG_WINDOWED) != 0;
    }

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

struct TGameRecorder
{
    enum
    {
        RESERVE_SIZE = 1024,
    };
    
    enum OBJ_TYPE
    {
        OBJ_TYPE_NONE = 0,
        OBJ_TYPE_MISSILE = 1,
        OBJ_TYPE_VEHICLE = 2
    };
    
    struct __attribute__((packed)) TSndState
    {
        uint16_t active = 0;
        int16_t pitch = 0;
    };

    IFFile mfile;
    uint16_t seqn = 0;
    uint16_t level_id = 0;
    int32_t frame_id = 0;
    int32_t time = 0;
    uint32_t ctrl_bact_id = 0;

    std::vector<NC_STACK_ypabact *> bacts;
    std::vector<trec_bct> oinf;
    std::vector<TSndState> sound_status;
    //std::vector<???> field_20;
    std::vector<uint8_t> ainf;

    const int32_t max_bacts = 1024;
    //int32_t field_2C = 0;
    uint32_t bacts_count = 0;
    //int32_t field_34 = 0;
    int32_t ainf_size = 0;
    int32_t do_record = 0;
    int32_t field_40 = 0;
    vec3d field_44;
    mat3x3 rotation_matrix;
    int32_t field_74 = 0;
    int32_t field_78 = 0;
    int32_t field_7C = 0;
    int32_t field_80 = 0;
    uint32_t field_84 = 0;
    std::string filename;
    
    
    TGameRecorder()
    {
        bacts.resize(RESERVE_SIZE);
        oinf.resize(RESERVE_SIZE);
        sound_status.resize(RESERVE_SIZE);
        ainf.resize(RESERVE_SIZE * 2 * sizeof(TSndState));
    }
    
    static bool BactSortCompare(NC_STACK_ypabact * const &a, const NC_STACK_ypabact * const &b)
    {
        if (a->_gid < b->_gid)
            return true;
        return false;
    }
};

enum CELL_PFLAGS
{
    CELL_PFLAGS_IN_CLST = 1, // Sector in close list
    CELL_PFLAGS_IN_OLST = 2  // Sector in open list
};

struct cellArea
{    
    int32_t Id = 0;
    Common::Point CellId;

    float addit_cost = 0.0; // Additional cost for ground units
    uint8_t pf_flags = 0; // Pathfind flags
    float cost_to_this = 0.0;
    float cost_to_target = 0.0;
    cellArea *pf_treeup = NULL;
 
    enum CF
    {
        // Not playable sector, used as border
        CF_BORDER = (1 << 0),
    };
    
    uint8_t Flags = 0;

    uint8_t owner = 0;
    int32_t type_id = 0; // Index in array
    uint8_t SectorType = 0; // Complex (3x3) or simple
    int32_t energy_power = 0; // Cell electric power
    Common::PlaneArray<int16_t, 3, 3> buildings_health = Common::PlaneArray<int16_t, 3, 3>::ArrayInit(0);
    Common::PlaneArray<NC_STACK_base::Instance *, 3, 3> BldVPOpts = Common::PlaneArray<NC_STACK_base::Instance *, 3, 3>::ArrayInit(NULL);
    uint8_t view_mask = 0; // Who can view this sector (mask)
    uint8_t UnhideMask = 0; // Who can unhide other fractions (mask)
    
    enum PTYPE
    {
        PT_NONE            = 0,  // Oridnary sector 
        PT_CONSTRUCTING    = 1,  // Building construct process 
        PT_POWERSTATION    = 2,
        PT_BUILDINGS       = 3,
        PT_TECHUPGRADE     = 4,
        PT_GATECLOSED      = 5,
        PT_GATEOPENED      = 6,
        PT_TECHDEACTIVE    = 7,
        PT_STOUDSON        = 8
    };
    uint8_t PurposeType = PT_NONE;
    int32_t PurposeIndex = 0;
    
    World::RefBactList unitsList; // Units in this sector
    vec3d CenterPos;
    float height = 0.0;
    float averg_height = 0.0;
    
    cellArea() : unitsList(this, NC_STACK_ypabact::GetCellRefNode) 
    { BldVPOpts.fill(NULL); };
    
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
    
    inline static int32_t ViewMask(int ownr) { return (1 << ownr); }
    
    bool IsUnhideFor(int owant) const { return (ViewMask(owant) & UnhideMask) != 0; }
    
    void AddUnhideMask(int ownr) { UnhideMask |= ViewMask(ownr); }
    
    void DelUnhideMask(int ownr) { UnhideMask &= ~ViewMask(ownr); }
    
    
    int GetEnergy()
    {
        if ( SectorType == 1 )
            return buildings_health.At(0, 0);

        int e = 0;
        for (auto hlth : buildings_health)
            e += hlth;
        
        return e;
    }
    
    inline bool IsBorder() const
    {
        return (Flags & CF_BORDER) != 0;
    }
    
    inline bool IsGamePlaySector() const
    {
        return (Flags & CF_BORDER) == 0;
    }
};

struct TCellFillerCh : TObjectCache
{
    int8_t Id1 = -1;
    int8_t Id2 = -1;
    float  Heights[4] = {0.};
    
    void FreeVBO();
    void MakeVBO();
    
    ~TCellFillerCh();
};

struct TMapKeySector
{
    Common::Point CellId;
    cellArea *PCell = NULL;
};

struct TMapGate
{
    cellArea *PCell = NULL;
    Common::Point CellId;
    int ClosedBldID = 0;
    int OpenBldID = 0;
    std::vector<int> PassToLevels;
    std::vector<TMapKeySector> KeySectors;
    int MbStatus = 0;
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

struct TMapGem
{
    int16_t BuildingID = 0;
    Common::Point CellId;
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
    Common::Point CellId;
    int32_t InactiveBldID = 0;
    int32_t ActiveBldID = 0;
    int32_t TriggerBldID = 0;
    std::vector<TMapKeySector> KeySectors;
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
    std::vector<TMapGate> Gates;
    std::vector<TMapSuperItem> SuperItems;
    std::array<int, 8> JodieFoster = Common::ArrayInit<int, 8>(0);
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
    
    std::array<NC_STACK_sklt *, 4> VectorGfx = Common::ArrayInit<NC_STACK_sklt *, 4>(NULL);
    Common::PlaneBytes OwnMap;
    Common::PlaneBytes TypMap;
    //int _owner;
    uint32_t LastFrameTimeStamp = 0;
    std::array<World::TPlayerStatus, World::CVFractionsCount> StatsGlobal;
    std::array<World::TPlayerStatus, World::CVFractionsCount> StatsIngame;
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
            MbmapImg->Delete();
            MbmapImg = NULL;
        }

        if ( BriefingMapImg )
        {
            BriefingMapImg->Delete();
            BriefingMapImg = NULL;
        }
        
        for ( NC_STACK_sklt* &gfx : VectorGfx )
        {
            if (gfx)
            {
                gfx->Delete();
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
    
    inline bool IsFraction(int32_t id) const { return (FractionsBits & (1 << id)) != 0;}
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
    int field_0 = 0;
    NC_STACK_sklt *sklts[14] = {NULL};
    UAskeleton::Data *sklts_intern[14] = {NULL};
    int field_72 = 0;
    int field_76 = 0;
    int field_7A = 0;
    NC_STACK_ypabact *field_7E = NULL;
    int field_82 = 0;
    float field_86 = 0.0;
    float field_8A = 0.0;
    float field_8E = 0.0;
    float field_92 = 0.0;
    int field_96 = 0;
    int field_9A = 0;
    int field_9E = 0;
    float cl1_r = 0.0;
    float cl1_g = 0.0;
    float cl1_b = 0.0;
    float cl2_r = 0.0;
    float cl2_g = 0.0;
    float cl2_b = 0.0;
};


struct TMFWinStatus
{
    bool Valid = false;
    bool IsOpen = false;
    Common::PointRect Rect;
    std::array<int32_t, 8> Data = Common::ArrayInit<int32_t, 8>(0);
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
            Sample->Delete();
        
    }
    
    void Reset()
    {
        SFXEngine::SFXe.StopCarrier(&Carrier);
        
        if (Sample)
            Sample->Delete();
        
        Carrier.Resize(1);
        Priority = -1;
        Sample = NULL;
        Unit = NULL;
    }
};

struct TPowerStationInfo
{
    Common::Point CellId; 
    cellArea *pCell = NULL;
    int32_t Power = 0;
    int32_t EffectivePower = 0;
    
    operator Common::Point() { return CellId; }
};

struct EnergyAccum
{
    uint8_t Owner = 0;
    int32_t Energy = 0;
};

struct TLego
{
    NC_STACK_base *Base = NULL;
    NC_STACK_skeleton *CollisionSkelet = NULL;
    NC_STACK_skeleton *UseCollisionSkelet = NULL;
    uint8_t Shield = 0; // Percents
    uint8_t GUIElementID = 0;
    
    /* Explode object and position */
    struct ExFX
    {
        uint8_t Index = 0;
        uint8_t ObjectID = 0;
        vec3d Position;
    };

    std::vector<ExFX> Explosions;
};

struct TSubSectorDesc
{
    int32_t StartHealth = 0;
    std::array<uint8_t, 4> HPModels = Common::ArrayInit<uint8_t, 4>(0); //Building health models 0 - 100%hp, 3 - 0%hp
};

struct TSectorDesc
{
    uint8_t SectorType = 0;
    uint8_t SurfaceType = 0;
    uint8_t GUIElementID = 0;
    Common::PlaneArray<TSubSectorDesc *, 3, 3> SubSectors = Common::PlaneArray<TSubSectorDesc *, 3, 3>::ArrayInit(0);
};





struct yw_arg172
{
    const char *usertxt;
    const char *field_4;
    int UsageFlags;
    UserData *usr;
    int InitGameShell;
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
    Common::Point CellId;
    cellArea *pcell;
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
    int owner;
    int blg_ID;
    int field_C;
    Common::Point CellId;
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
    struct TConstructInfo
    {
        Common::Point CellID;
        int Time = 0;
        int EndTime = 0;
        int Owner = 0;
        uint8_t BuildID = 0;
    };
    
    struct TNetGameEvent
    {
        int8_t EventType = 0;
        uint32_t TimeStamp = 0;
        std::string PlayerName;
        std::string Player2Name;
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
    virtual bool CreateTitleControls();
    virtual bool CreateSubBarControls();
    virtual bool CreateConfirmControls();
    virtual bool CreateInputControls();
    virtual bool CreateVideoControls();
    virtual bool CreateDiskControls();
    virtual bool CreateLocaleControls();
    virtual bool CreateAboutControls();
    virtual bool CreateNetworkControls();
    virtual bool OpenGameShell();
    virtual void CloseGameShell();
    virtual void ProcessGameShell();
    virtual void ypaworld_func159(yw_arg159 *arg);
    virtual void ypaworld_func160(void *arg);
    virtual size_t ypaworld_func161(yw_arg161 *arg);
    virtual size_t ypaworld_func162(const std::string &fname);
    virtual void ypaworld_func163(base_64arg *arg);
    virtual void ypaworld_func164();
    virtual void ypaworld_func165(yw_arg165 *arg);
    virtual size_t ypaworld_func166(const std::string &langname);
    virtual void UpdateGameShell();
    virtual size_t ypaworld_func168(NC_STACK_ypabact *pbact);
    virtual size_t LoadGame(const std::string &saveFile);
    virtual size_t SaveGame(const std::string &saveFile);
    virtual bool SaveSettings(UserData *usr, const std::string &fileName, uint32_t sdfMask);
    virtual size_t LoadSettings(const std::string &fileName, const std::string &userName, uint32_t sdfMask, bool updateGameShell, bool playIntro = false);
    virtual bool ReloadInput(size_t id);
    virtual size_t SetGameShellVideoMode(bool windowed);
    virtual size_t ReloadLanguage();
    virtual void ypaworld_func176(yw_arg176 *arg);
    virtual void ypaworld_func177(yw_arg177 *arg);
    virtual size_t ypaworld_func179(yw_arg161 *arg);
    virtual void ypaworld_func180(yw_arg180 *arg);
    virtual bool NetSendMessage(uamessage_base *data, size_t dataSize, const std::string &recvID, bool garantee);
    virtual bool NetBroadcastMessage(uamessage_base *data, size_t dataSize, bool garantee);

    virtual size_t ypaworld_func183(yw_arg161 *arg);
    virtual void HistoryEventAdd(const World::History::Record &arg);
    virtual void ypaworld_func185(const void *arg);

    NC_STACK_ypaworld();
    virtual ~NC_STACK_ypaworld() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
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
    virtual void setYW_dontRender(bool);

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
    virtual std::vector<World::TWeapProto> &GetWeaponsProtos() { return _weaponProtos; };
    virtual std::vector<World::TBuildingProto> &GetBuildProtos() { return _buildProtos; };
    virtual std::vector<World::TVhclProto> &GetVhclProtos() { return _vhclProtos; };
    virtual std::vector<World::TRoboProto> &GetRoboProtos() { return _roboProtos; };
    virtual int getYW_lvlFinished();
    virtual int getYW_screenW();
    virtual int getYW_screenH();
    virtual TLevelInfo &GetLevelInfo();
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

    void CameraPrepareRender(TGameRecorder *rcrd, NC_STACK_ypabact *bact, TInputState *inpt);
    bool IsAnyInput(TInputState *struc);


    void GameShellUiOpenNetwork(); // On main menu "Multiplayer" press
    void GameShellBkgProcess();
    void GameShellBlitBkg(NC_STACK_bitmap *bitm);
    void GameShellInitBkgMode(int mode);
    bool GameShellInitBkg();

    int  InputConfigLoadDefault();

    static uint32_t PointToUint32(const Common::Point &point) 
    { return (0xFFFF & point.y) << 16 | (0xFFFF & point.x); }

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
    
    void DoSectorsEnergyRecalc();
    void RecalcSectorsPowerForPS(const TPowerStationInfo &ps);
    
    void sub_4D12D8(int id, int a3);
    void sub_4D1594(int id);
    void sub_4D1444(int id);
    int LoadingParseSaveFile(const std::string &filename);
    void LoadingUnitsRefresh();
    int ParseSettingsFile(const std::string &fname, uint32_t sdfMask);
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
    NC_STACK_ypabact * FindBactByCmdOwn(uint32_t commandID, char owner);
    
    
    bool yw_createRobos(const std::vector<MapRobo> &Robos);
    bool yw_NetSetHostStations(const std::vector<MapRobo> &Robos);
    void yw_InitSquads(const std::vector<MapSquad> &squads);
    
    
    NC_STACK_ypabact *yw_createUnit(int model_id);
    void sb_0x456384(const Common::Point &cellId, int ownerid2, int blg_id, int a7);
    void SetupPowerStationInfo(cellArea *cell, int power);
    void ResetAccumMap();
    
    void CellSetOwner(cellArea *cell, uint8_t owner);
    
    void DestroyAllGunsInSector(cellArea *cell);
    bool BuildingConstructBegin(cellArea *cell, uint8_t buildingID, int owner, int cTime);
    void BuildingConstructUpdate(int dtime);
    bool IsAnyBuildingProcess(int owner) const;
    
    void debug_info_draw(TInputState *inpt);
    void debug_count_units();
    void ProfileCalcValues();
    
    
    void SendCRC(int lvlid);
    
    void UpdateGuiSettings();
    void LoadGuiFonts();
    void CreateNewGuiElements();
    void DeleteNewGuiElements();
    
    void HistoryAktCreate(NC_STACK_ypabact *bact);
    void HistoryAktKill(NC_STACK_ypabact *bact);
    
    
    int sub_449678(TInputState *struc, int kkode);
    
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
    
    void ypaworld_func64__sub7(TInputState *inpt);
    void ypaworld_func64__sub7__sub4(TInputState *inpt);
    void ypaworld_func64__sub7__sub4__sub0(int a2);
    void ypaworld_func64__sub7__sub3__sub4(NC_STACK_ypabact *bact);
    bool ypaworld_func64__sub7__sub3__sub2();
    
    void yw_MouseSelect(TInputState *arg);
    void ypaworld_func64__sub21__sub1__sub0(TInputState *arg);
    
    void yw_MAP_MouseSelect(TClickBoxInf *winp);
    
    void ypaworld_func64__sub21__sub1__sub3__sub0(TClickBoxInf *winp);
    void yw_3D_MouseSelect(TClickBoxInf *winp);
    
    void ypaworld_func64__sub21(TInputState *arg);
    void ypaworld_func64__sub21__sub7();
    float sub_4498F4();
    
    int ypaworld_func64__sub7__sub3__sub1(TClickBoxInf *winpt);
    void yw_SMAN_MouseSelect(TClickBoxInf *winp);
    void SquadManager_InputHandle(TInputState *inpt);
    NC_STACK_ypabact * sub_4C7B0C(int sqid, int a3);
    
    bool recorder_create_camera();
    void recorder_updateObjectList(TGameRecorder *rcrd, float a5, int period);
    void recorder_updateObject(NC_STACK_ypabact *bact, trec_bct *oinf, TGameRecorder::TSndState *ssnd, float a5, float a6);
    void recorder_set_bact_pos(NC_STACK_ypabact *bact, const vec3d &pos);
    int recorder_go_to_frame(TGameRecorder *rcrd, int wanted_frame_id);
    void recorder_store_bact(TGameRecorder *rcrd, World::RefBactList &bct_lst);
    void recorder_store_bact( TGameRecorder *rcrd, World::MissileList &bct_lst);
    void recorder_world_to_frame(TGameRecorder *rcrd);
    void recorder_write_frame();
    NC_STACK_ypabact *recorder_newObject(trec_bct *oinf);
    void ypaworld_func163__sub1(TGameRecorder *rcrd, int a3);
    
    void SetShowingTooltip(int32_t id)
    {
        if ( id > _toolTipId ) // avoid flickr of tooltips
        {
            _toolTipHotKeyId = -1;
            _toolTipId = id;
        }
    }
    
    void SetShowingTooltipWithHotkey(int32_t id, int32_t hotkey)
    {
        if ( id > _toolTipId ) // avoid flickr of tooltips
        {
            _toolTipHotKeyId = hotkey;
            _toolTipId = id;
        }
    }
    
    std::string GetTooltipString() const
    {
        return Locale::Text::Tooltip(_toolTipId);
    }
    
    void ypaworld_func64__sub1(TInputState *inpt);
    void ypaworld_func64__sub21__sub5(int arg);
    
    
    bool ypaworld_func64__sub21__sub6(TClickBoxInf *winp);
    int ypaworld_func64__sub21__sub4(TInputState *arg, int a3);
    int ypaworld_func64__sub21__sub3();
    int UserActBuildCheck();
    int sb_0x4d3d44(TClickBoxInf *winp);
    void ypaworld_func64__sub7__sub6(TInputState *inpt);
    
    int yw_MouseFindCreationPoint(TClickBoxInf *winp);
    
    void sb_0x4c87fc(const std::string &a2, GuiBase *lstvw);
    void NetSendExitMsg(uint8_t normExit);
    void sub_449DE8(const std::string &a2);
    
    void LoadKeyNames();
    
    NC_STACK_ypabact *GetDefaultCmdr() const;
    NC_STACK_ypabact *GetNextCmdrByCmdId(uint32_t cmdrId) const;
    NC_STACK_ypabact *GetNextSquad() const;
    
    
    void InitGates();
    void UpdatePowerEnergy();
    void PowerStationErase(cellArea *cell);
    void CellSetNewOwner(cellArea *cell, NC_STACK_ypabact *a5, int newOwner);
    void CellCheckHealth(cellArea *cell, int a5, NC_STACK_ypabact *a6);
    void InitBuddies();
    void InitSuperItems();
    bool LoadBlgMap(const std::string &mapName);
    bool LoadHightMap(const std::string &mapName);
    bool LoadOwnerMap(const std::string &mapName);
    bool LoadTypeMap(const std::string &mapName);
    
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
    
    void SetFixWeaponRadius( bool fix ) { _fixWeaponRadius = fix; };
    
    
    void VoiceMessageUpdate();
    void VoiceMessagePlayFile(const std::string &flname, NC_STACK_ypabact *unit, int a5);
    void VoiceMessagePlayMsg(NC_STACK_ypabact *unit, int priority, int msgID);
    void VoiceMessageCalcPositionToUnit();
    
    bool ProtosInit();
    void ProtosFreeSounds();
    
    void FreeGameDataCursors();
    
    void SetCmdrIdToSelect(int32_t id) { _cmdrIdToSelect = id; };
        
    World::ParticleSystem &ParticleSystem() { return _particles; };
    
    int32_t GetLegoBld(const cellArea *cell, int bldX, int bldY);
    int32_t GetLegoBld(const Common::Point &cell, int bldX, int bldY);
    
    void ClearOverrideModels();
    void LoadOverrideModels();
    
    std::string GetVehicleName(uint32_t id) const
    {
        return Locale::Text::VehicleName(id, _vhclProtos[ id ].name);
    }
    
    std::string GetVehicleName(const World::TVhclProto &proto) const
    {
        if (proto.Index != -1)
            return Locale::Text::VehicleName(proto.Index, proto.name);
        
        return proto.name;
    }
    
    std::string GetBuildingName(uint32_t id, bool net = false) const
    {
        if (net)
            return Locale::Text::NetBuildingName(id, _buildProtos[ id ].Name);
        else
            return Locale::Text::BuildingName(id, _buildProtos[ id ].Name);
    }
    
    std::string GetBuildingName(const World::TBuildingProto &proto, bool net = false) const
    {
        if (proto.Index != -1)
        {
            if (net)
                return Locale::Text::NetBuildingName(proto.Index, proto.Name);
            else
                return Locale::Text::BuildingName(proto.Index, proto.Name);
        }
        
        return proto.Name;
    }
    
    std::string GetLevelName(uint32_t id) const
    {
        return Locale::Text::LevelName(id, _globalMapRegions.MapRegions[ id ].MapName);
    }
    
    std::string GetLevelName(const TLevelInfo &lvl) const
    {
        return Locale::Text::LevelName(lvl.LevelID, lvl.MapName);
    }
    
    bool IsHidden(uint32_t owner) const
    {
        return (_hiddenFractions & (1 << owner)) != 0;
    }
    
    int8_t GetPlayerOwner() const { return _playerOwner; }

public:
    //Data
    static constexpr const char * __ClassName = "ypaworld.class";
  
    
    
    UserData *_GameShell = NULL;
    
    Common::Point _mapSize;

    Common::PlaneVector<cellArea> _cells;
    Common::PlaneVector<TCellFillerCh> _cellsVFCache;
    Common::PlaneVector<TCellFillerCh> _cellsHFCache;

    vec2d _mapLength;

    Common::PlaneVector<EnergyAccum> _energyAccumMap;
    std::map<int32_t, TPowerStationInfo> _powerStations;
    
    int32_t _nextPSForUpdate = 0;
    int32_t _setId = 0;
    
    NC_STACK_base *_setData = NULL;
    World::RefBactList _unitsList;
    World::RefBactList _deadCacheList;
    std::vector<NC_STACK_base *> _vhclModels;
    std::array<TLego, 256> _legoArray; 
    std::array<TSubSectorDesc, 256> _subSectorArray;
    std::array<TSectorDesc, 256> _secTypeArray;  
    
    std::vector<World::TVhclProto> _vhclProtos;
    std::vector<World::TWeapProto> _weaponProtos;
    std::vector<World::TBuildingProto> _buildProtos;
    std::vector<World::TRoboProto> _roboProtos;
    
    std::list<NC_STACK_base *> _overrideModels;
    
    std::map<int32_t, TConstructInfo> _inBuildProcess; // Buildings in creation process
    std::array<int16_t, 256> _buildHealthModelId = Common::ArrayInit<int16_t, 256>(0);
    Common::PlaneArray<uint8_t, 64, 64> _sqrtTable = Common::PlaneArray<uint8_t, 64, 64>::ArrayInit(0);
    
    NC_STACK_ypabact *_viewerBact = NULL;
    vec3d _viewerPosition;
    mat3x3 _viewerRotation;
    
    NC_STACK_base *_skyObject  = NULL;
    
    int32_t _renderSectors = 0; // Render distance in sectors 
    
    NC_STACK_base *_beeBox = NULL;
    NC_STACK_skeleton *_colsubSkeleton  = NULL;
    NC_STACK_skeleton *_colcompSkeleton  = NULL;
    NC_STACK_bitmap *_tracyBitmap  = NULL;
    NC_STACK_bitmap *_shadermpBitmap  = NULL;
    
    bool _doNotRender = false;
    
    Common::PlaneArray<NC_STACK_base *, 6, 6> _fillersHorizontal = Common::PlaneArray<NC_STACK_base *, 6, 6>::ArrayInit(NULL);
    Common::PlaneArray<NC_STACK_base *, 6, 6> _fillersVertical = Common::PlaneArray<NC_STACK_base *, 6, 6>::ArrayInit(NULL);
    
    NC_STACK_skeleton *_fillerSide = NULL;
    NC_STACK_skeleton *_fillerCross = NULL;
    
    int32_t _normalVizLimit = 0;
    int32_t _normalFadeLength = 0;
    int32_t _skyVizLimit = 0;
    int32_t _skyFadeLength = 0;
    int32_t _skyHeight = 0;
    bool _skyRender = true;
    bool _doEnergyRecalc = true; // Update powerstations energy give

    int32_t audio_volume = 0;
    
    bool _hasSGMSave = false;
    bool _hasRSTSave = false;
    
    bool _gamePaused = false;
    uint32_t _gamePausedTimeStamp = 0;
    
    uint32_t _timeStamp = 0;
    int32_t _frameTime = 0;
    uint32_t _framesElapsed = 0;
    
    std::string _buildDate; 
    
    bool _playerHSDestroyed = false;
    
    bool _fireBtnIsDown = false; // true - fire btn is down, contigues
    bool _fireBtnDownHappen = false; // true happen on down, single
    bool _guiLoaded = false;
    
    std::array<TileMap *, 92> _guiTiles = Common::ArrayInit<TileMap *, 92>(NULL);
    
    GuiBaseList _guiActive;
    
    Common::Point _screenSize;

    bool _guiDragging = false;
    GuiBase *_guiDragElement = NULL;
    Common::Point _guiDragPos;
    bool _guiDragDefaultMouse = false;

    bool _mouseGrabbed = false; // Grab mouse for unit steer-turn
    
    int32_t _toolTipId = 0;
    int32_t _toolTipHotKeyId = -1; // Used to display hotkey name
    
    std::array<SDL_Color, World::COLOR_MAX_NUMBER> _iniColors;
    
    uint32_t _msgTimestampGates = 0;
    uint32_t _msgTimestampHSReturn = 0;
    uint32_t _msgTimestampEnemySector = 0;
    uint32_t _msgTimestampPSUnderAtk = 0;
    
    uint32_t _vehicleTakenControlTimestamp = 0;
    uint32_t _vehicleTakenCommandId = 0;

    bool _invulnerable = false;
    
    int32_t _fontH = 0;
    int32_t _fontDefCloseW = 0;
    int32_t _fontVBScrollW = 0;
    int32_t _fontHScrollH = 0;
    int32_t _fontBorderW = 0;
    int32_t _fontBorderH = 2;
    int32_t _fontVScrollH = 0;
    
    int32_t _iconOrderW = 0;
    int32_t _iconOrderH = 0;
    int32_t _iconHelpW = 0;
    int32_t _iconHelpH = 0;
    
    int32_t _upScreenBorder = 0;
    int32_t _downScreenBorder = 0; 
    
    uint32_t _guiActFlags = 0;
    uint32_t _doAction = World::DOACTION_0;
    
    cellArea *_cellOnMouse = NULL;
    vec3d _cellMouseIsectPos; // intersect position in cell

    vec3d _mouseVector; // normalized mouse cursor vector
    
    NC_STACK_ypabact *_bactOnMouse = NULL;
    float _mouseSelDistance = 0.0;
    Common::Point _prevMousePos;
    NC_STACK_ypabact *_bactPrevClicked = NULL; // used for dblclick
    
    bool _makingWaypointsMode = false;
    int32_t _waypointCount = 0;
    vec3d _firstWaypointPosition;

    std::array<NC_STACK_bitmap *, 11> _mousePointers = Common::ArrayInit<NC_STACK_bitmap *, 11>(NULL);
 
    bool _mouseCursorHidden = false;
    Common::Point _mouseCursorHidePos;
    
    update_msg _updateMessage;
    
    int8_t _showDebugMode = 0; // debug info draw modes
    
    int32_t _polysCount = 0;
    int32_t _polysDraw = 0;
    uint16_t _FPS = 0;
    
    bool _playerInHSGun = false;
    uint8_t _ownerOldCellUserUnit = 0; // Used to check entering enemy sector
    
    NC_STACK_ypabact *_userRobo = NULL;
    NC_STACK_ypabact *_userUnit = NULL;
    
    std::array<uint32_t, 8> _countSectorsPerOwner = Common::ArrayInit<uint32_t, 8>(0);
    std::array<uint32_t, 8>  _countUnitsPerOwner = Common::ArrayInit<uint32_t, 8>(0);
    std::array<float, 8> _reloadRatioClamped = Common::ArrayInit<float, 8>(0.0); // clamped to 0.0..1.0
    std::array<float, 8> _reloadRatioPositive = Common::ArrayInit<float, 8>(0.0); // 0 and positive
    
    std::vector<NC_STACK_ypabact *> _cmdrsRemap;
    uint32_t _activeCmdrID = 0; // active CMDR ID
    int32_t _activeCmdrRemapIndex = 0; // index in _cmdrsRemap
    int32_t _activeCmdrKidsCount = 0; // count of alive units in active squad
    int32_t _cmdrIdToSelect = -1;
    
    uint32_t _prevUnitId = 0;
    
    NC_STACK_ypabact *_lastMsgSender = NULL;
    
    int32_t _screenShotCount = 0;
    
    bool _screenShotSeq = false;
    int32_t _screenShotSeqId = 0; //Screenshoting sequence id
    int32_t _screenShotSeqFrame = 0; //Screenshoting frame
    
    TGameRecorder *_replayPlayer = NULL; // For play replay
    TGameRecorder *_replayRecorder = NULL; // For record replay
    
    bact_hudi _guiVisor;
    
    std::vector<TMapGem> _techUpgrades; // tech upgrades in level
    int32_t _upgradeId = 0;
    uint32_t _upgradeTimeStamp = 0;
    int32_t _upgradeVehicleId = 0;
    int32_t _upgradeWeaponId = 0;
    int32_t _upgradeBuildId = 0;

    TMapRegionsNet _globalMapRegions; // level selector
    TLevelInfo _levelInfo; // current level information
    
    TBriefengScreen _briefScreen;
    
    // History
    Common::BlocksStream _history;
    World::History::Frame _historyLastFrame;
    bool      _historyLastIsTimeStamp = false;
    
    int32_t _stoudsonWaveVehicleId = 0;
    int32_t _stoudsonCenterVehicleId = 0; // Not used :-/
    
    bool _prepareDebrief = false; // prepare debrief and do it? or exit without
    bool _gameWasNetGame = false;
    uint8_t _userOwnerIdWasInNetGame = 0;
    
    Common::PlaneBytes _lvlTypeMap;
    Common::PlaneBytes _lvlOwnMap;
    Common::PlaneBytes _lvlBuildingsMap;
    Common::PlaneBytes _lvlHeightMap;

    // copy of map datas on level start, for debrief
    Common::PlaneBytes _lvlPrimevalTypeMap;
    Common::PlaneBytes _lvlPrimevalOwnMap;
    
    
    sklt_wis _hud;
    
    bool _shellConfIsParsed = false;
    uint32_t  _preferences = 0;
    TMFWinStatus _roboMapStatus;
    TMFWinStatus _roboFinderStatus;
    TMFWinStatus _vhclMapStatus;
    TMFWinStatus _vhclFinderStatus;
    
    int32_t _fxLimit = 0;
    
    std::array<int32_t, World::CVFractionsCount> _dbgSquadCounter = Common::ArrayInit<int32_t, World::CVFractionsCount>(0);
    std::array<int32_t, World::CVFractionsCount> _dbgVehicleCounter = Common::ArrayInit<int32_t, World::CVFractionsCount>(0);
    std::array<int32_t, World::CVFractionsCount> _dbgFlakCounter = Common::ArrayInit<int32_t, World::CVFractionsCount>(0);
    std::array<int32_t, World::CVFractionsCount> _dbgRoboCounter = Common::ArrayInit<int32_t, World::CVFractionsCount>(0);
    std::array<int32_t, World::CVFractionsCount> _dbgWeaponCounter = Common::ArrayInit<int32_t, World::CVFractionsCount>(0);
    
    int32_t _dbgTotalSquadCount = 0;
    int32_t _dbgTotalSquadCountMax = 0;
    int32_t _dbgTotalVehicleCount = 0;
    int32_t _dbgTotalVehicleCountMax = 0;
    int32_t _dbgTotalFlakCount = 0;
    int32_t _dbgTotalFlakCountMax = 0;
    int32_t _dbgTotalRoboCount = 0;
    int32_t _dbgTotalRoboCountMax = 0;
    int32_t _dbgTotalWeaponCount = 0;
    int32_t _dbgTotalWeaponCountMax = 0;
    
    uint32_t _ffTimeStamp = 0;
    int32_t _ffEffectType = 0;
    float _ffPeriod = 0.0;
    float _ffMagnitude = 0.0;
    
    NC_STACK_windp *_netDriver = NULL;
    uint32_t _netUpdateTimeStamp = 0;
    bool _isNetGame = false;
    bool _isNetGameStarted = false;
    int32_t _netFlushTimer = 0;

    bool _netInfoOverkill = false;

    int32_t _netStartTimer = 0;
    bool _netInterpolate = false;
    
    std::string _netTcpAddress; // your tcp address string (for printing)

    bool _netExclusiveGem = false;
    
    bool _netChatSystem = false;

    
    enum PFID
    {
        PFID_FPS        = 0,
        PFID_FRAMETIME  = 1,
        PFID_MARKTIME   = 2, // time elapsed for change view mask of sectors
        PFID_GUITIME    = 3,
        PFID_UPDATETIME = 4,
        PFID_RENDERTIME = 5,
        PFID_NETTIME    = 6,
        PFID_POLYGONS   = 7,
        PFID_NEWGUITIME = 8,
        
        PFID_MAX        = 9
    };
    
    int32_t _profileFramesCount = 0;   
    std::array<uint32_t, PFID_MAX> _profileVals = Common::ArrayInit<uint32_t, PFID_MAX>(0);
    std::array<uint32_t, PFID_MAX> _profileMax = Common::ArrayInit<uint32_t, PFID_MAX>(0);
    std::array<uint32_t, PFID_MAX> _profileMin = Common::ArrayInit<uint32_t, PFID_MAX>(0);
    std::array<uint32_t, PFID_MAX> _profileTotal = Common::ArrayInit<uint32_t, PFID_MAX>(0);
    
    std::array<World::TPlayerStatus, World::CVFractionsCount> _playersStats;
    std::array<World::TPlayerStatus, World::CVFractionsCount> _gameplayStats;
    
    int32_t _maxRoboEnergy = 0;
    int32_t _maxReloadConst = 0;
    
    TSingleVoiceMessage _voiceMessage;
    
    bool _joyIgnoreX = false;
    bool _joyIgnoreY = false;
    bool _joyIgnoreZ = false;
    
    float _oldJoyX = 0.0; // input slider 12
    float _oldJoyY = 0.0; // input slider 13
    float _oldJoyZ = 0.0; // input slider 14
    float _oldJoyHatX = 0.0; // input slider 15
    float _oldJoyHatY = 0.0; // input slider 16
    
    std::array<std::string, World::MOVIE_MAX_NUMBER> _movies;
    
    int8_t _firstContactFaction = 0;
    
    std::string _helpURL;
    std::string _helpSavedURL; 
    
    bool _oneGameRes = false;
    Common::Point _shellDefaultRes;
    Common::Point _gameDefaultRes;
    
    GFX::GfxMode _gfxMode;
    Common::Point _shellGfxMode;

    float _maxImpulse = 0.0;
    

    TNetGameEvent _netEvent;
    
    
    float _vehicleSectorRatio = 0.0; // not used
    uint32_t _defaultUnitLimit = 0;
    uint32_t _defaultUnitLimitArg = 0;
    uint32_t _defaultUnitLimitType = 0;
    uint32_t _levelUnitLimit = 0;
    uint32_t _levelUnitLimitArg = 0;
    uint32_t _levelUnitLimitType = 0;
    
    int16_t _kbdLastKeyHit = Input::KC_NONE;
    int32_t _tipOfDayId = 0;
    int32_t _beamEnergyStart = 0;
    int32_t _beamEnergyAdd = 0;
    int32_t _beamEnergyCapacity = 0;
    int32_t _beamEnergyCurrent = 0;
    
    bool _easyCheatKeys = false;

    int8_t _playerOwner = 0;
        
    std::string _initScriptFilePath;

    std::string _luaScriptName;
    
    /* Allow to build multiple buildings at the same time 
       Can be set in
       world: misc scope  - "multi_building" 
       level: level scope  - "multi_building" */
    bool _allowMultiBuildWorld = false;
    bool _allowMultiBuildLevel = false;
    
    /* Fix original bug */
    bool _fixWeaponRadius = false;
    
    /* */
    uint32_t _worldHiddenFractions = World::OWNER_BLACK_BIT;
    uint32_t _hiddenFractions = World::OWNER_BLACK_BIT;

protected:

    /** On saved game load variables **/
    bool _extraViewEnable = false; // If you seat in robo gun
    int _extraViewNumber = -1; // robo gun index
    
    World::ParticleSystem _particles;

    World::LuaEvents *_script = NULL;
};

#endif
