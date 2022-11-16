#ifndef WORLD_STRIDS_H
#define WORLD_STRIDS_H


namespace World {

enum LOCALE_BLOCK 
{
    LBL_TIPS = 800,
};

enum TIP
{
    TIP_NONE        = 0,
    
    TIP_GUI_GIVEORDERS      = 1,
    TIP_GUI_GOINTOGUNS      = 2,
    TIP_GUI_NEWSQUAD        = 3,
    TIP_GUI_ADDSQUAD        = 4,
    TIP_GUI_TAKECONTROL     = 5,
    TIP_GUI_BUILD           = 6,
    TIP_GUI_TELEPORT        = 7,
    TIP_GUI_MAPWND          = 8,
    TIP_GUI_FINDERWND       = 9,
    TIP_GUI_MSGWND          = 10,
    TIP_GUI_ENERGYWND       = 11,
    
    TIP_GUI_AGGR_0          = 12,
    TIP_GUI_AGGR_1          = 13,
    TIP_GUI_AGGR_2          = 14,
    TIP_GUI_AGGR_3          = 15,
    TIP_GUI_AGGR_4          = 16,
    
    TIP_GUI_GOTOHS          = 17,
    TIP_GUI_GOTOCMDR        = 18,
    TIP_GUI_NEXTUNIT        = 19,
    TIP_GUI_NEXTCMDR        = 20,
    
    TIP_GUI_PAUSECANCEL     = 21,
    
    TIP_MAP_DETAIL          = 22,
    TIP_MAP_OWNERS          = 23,
    TIP_MAP_EINFO           = 24,
    TIP_MAP_UNLOCK          = 25,
    TIP_MAP_LOCKVIEWER      = 26,
    TIP_MAP_LOCKSQUAD       = 27,
    TIP_MAP_ZOOMOUT         = 28,
    TIP_MAP_ZOOMIN          = 29,
    TIP_MAP_SIZE            = 30,
    
    TIP_FINDER_WAITORDER    = 31,
    TIP_FINDER_FIGHTING     = 32,
    TIP_FINDER_MOVE         = 33,
    TIP_FINDER_ESCAPE       = 34,
    
    TIP_ENERGY_RELOAD       = 35,
    TIP_ENERGY_MAIN         = 36,
    TIP_ENERGY_CREATION     = 37,
    TIP_ENERGY_BUILDING     = 38,
    TIP_ENERGY_TELEPORT     = 39,
    
    TIP_SEL_ANOTHERACT      = 40,
    TIP_SEL_DEFENSEGUN      = 41,
    TIP_SEL_VEHICLETYPE     = 42,
    TIP_SEL_BUILDTYPE       = 43,
    
    TIP_HUD_SHOWHIDE        = 44,
    TIP_CTRLASTSNDR         = 45,
    
    TIP_COUNTINSQUAD        = 46,
    TIP_ANALYZER            = 47,
    TIP_ONLINEHELP          = 48,
    
    TIP_DO_SELECT           = 64,
    TIP_DO_MOVE             = 65,
    TIP_DO_ATKSECTOR        = 66,
    TIP_DO_ATKVHCL          = 67,
    TIP_DO_NEWSQUAD         = 68,
    TIP_DO_ADDSQUAD         = 69,
    TIP_DO_CONTROL          = 70,
    TIP_DO_BUILD            = 71,
    TIP_DO_TELEPORT         = 72,
    
    TIP_MENU_EXITDEBRIEF    = 73,
    TIP_MENU_GOTOLOADSAVE   = 74,
    TIP_MENU_SEL3DDEV       = 75,
    TIP_MENU_SENDMSG        = 76,
    TIP_MENU_ALTJOY         = 77,
    TIP_MENU_16BITTEX       = 78,
    TIP_MENU_DRAWPRIMITIVE  = 79,
    
    TIP_MOUSECAPTURED       = 80,
    
    TIP_UNITLIMIT           = 81,
    
    TIP_MENU_WINDOWED       = 85,
    
    TIP_ERR_NOTARGET        = 96,
    TIP_ERR_NOROOM          = 97,
    TIP_ERR_NOENERGY        = 98,
    TIP_ERR_NOTOWNED        = 99,
    TIP_ERR_BUILD_TOOFAR    = 100,
    TIP_ERR_BUILD_TOOCLOSE  = 101,
    TIP_ERR_BUILD_INPROC    = 102,
    TIP_ERR_UNITSINSECTOR   = 103,
    TIP_ERR_SECTORJAGGY     = 104,
    TIP_ERR_LOCUNKNOWN      = 105,
    
    TIP_MENU_LVLSELECTMAP   = 106,
    TIP_MENU_GOTRAIN        = 107,
    TIP_MENU_LANGCHOOSE     = 108,
    TIP_MENU_LANGEXIT       = 109,
    TIP_MENU_SELNEWLEVEL    = 110,
    TIP_MENU_SENDTOALL      = 111,
    TIP_MENU_SELRESIST      = 112,
    TIP_MENU_SELGHOR        = 113,
    TIP_MENU_SELMYKO        = 114,
    TIP_MENU_SELTAER        = 115,
    TIP_MENU_READY          = 116,
    TIP_MENU_NOTREADY       = 117,
    
    TIP_MENU_LOADUSER       = 118,
    TIP_MENU_SAVEUSER       = 119,
    
    TIP_MENU_INPUTSCREEN    = 120,
    TIP_MENU_VIDEOSCREEN    = 121,
    TIP_MENU_SOUNDSCREEN    = 122,
    TIP_MENU_PROFILESCREEN  = 123,
    TIP_MENU_LANGSCREEN     = 124,
    
    TIP_MENU_BRIEFCTRL      = 125,
    TIP_MENU_BRIEFSTOP      = 126,
    TIP_MENU_LOADLAST       = 127,
    TIP_MENU_BRIEFFAST      = 128,
    TIP_MENU_BRIEFRESET     = 129,
    TIP_MENU_PLAYLEVEL      = 130,
    TIP_MENU_QUIT           = 131,
    TIP_MENU_PRESSADDS      = 132,
    TIP_MENU_USEKEY         = 133,
    TIP_MENU_SOUNDLEFT      = 134,
    TIP_MENU_SOUNDRIGHT     = 135,
    TIP_MENU_NETSCREEN      = 136,
    TIP_MENU_HELP           = 137,
    TIP_MENU_TOMAIN         = 138,
    TIP_MENU_BACKTOLVLSEL   = 139,
    TIP_MENU_NETCLOSE       = 140,
    TIP_MENU_NETHELP        = 141,
    TIP_MENU_NETSELPROV     = 142,
    TIP_MENU_NETPLNAME      = 143,
    TIP_MENU_NETSELLVL      = 144,
    TIP_MENU_NETSELSESS     = 145,
    TIP_MENU_NETSTART       = 146,
    TIP_MENU_NETBACKPROV    = 147,
    TIP_MENU_NETBACKPLAYER  = 148,
    TIP_MENU_NETBACKSESS    = 149,

    TIP_MENU_151            = 151,
    TIP_MENU_152            = 152,
    TIP_MENU_153            = 153,
    TIP_MENU_154            = 154,
    TIP_MENU_155            = 155,
    TIP_MENU_156            = 156,
    TIP_MENU_157            = 157,
    TIP_MENU_158            = 158,
    TIP_MENU_159            = 159,
    TIP_MENU_160            = 160,
    TIP_MENU_161            = 161,
    TIP_MENU_162            = 162,
    TIP_MENU_163            = 163,
    TIP_MENU_164            = 164,
    TIP_MENU_165            = 165,
    TIP_MENU_166            = 166,
    TIP_MENU_167            = 167,
    
    TIP_PROFILE_DEL         = 168,
    TIP_PFOFILE_NEW         = 169,
    TIP_PFOFILE_LOADTHIS    = 170,
    TIP_PFOFILE_SAVETHIS    = 171,
    TIP_PFOFILE_DELTHIS     = 172,
    TIP_PFOFILE_NEWTHIS     = 173,
    TIP_PFOFILE_CLOSE       = 174,
    TIP_PROFILE_CANCELLOAD  = 175,
    TIP_PROFILE_CANCELSAVE  = 176,
    TIP_PROFILE_CANCELDEL   = 177,
    TIP_PROFILE_CANCELNEW   = 178,
    
    TIP_CONF_CLOSE          = 179,
    TIP_CONF_APPLY          = 180,
    TIP_CONF_RESOLUTION     = 181,
    TIP_CONF_FXSND          = 182,
    TIP_CONF_CHANNELS       = 183,
    TIP_CONF_FARDEPTH       = 184,
    TIP_CONF_SKY            = 185,
    TIP_CONF_FILTERING      = 186,
    TIP_CONF_MUSIC          = 187,
    TIP_CONF_HWSWMOUSE      = 188,
    TIP_CONF_ENEMYSIGNS     = 189,
    TIP_CONF_FXVOLUME       = 190,
    TIP_CONF_MUSICVOLUME    = 191,
    TIP_CONF_EXPLODES       = 192,
    
    TIP_INPUT_APPLY         = 193,
    TIP_INPUT_CANCEL        = 194,
    TIP_INPUT_RESET         = 195,
    TIP_INPUT_USEJOY        = 196,
    TIP_INPUT_FFJOY         = 197,
    TIP_INPUT_REMOVEKEY     = 198,
    
    TIP_MAX
};


}


#endif