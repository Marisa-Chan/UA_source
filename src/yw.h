#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include "engine_gfx.h"
#include "engine_input.h"
#include "engine_miles.h"
#include "bitmap.h"
#include "ilbm.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "base.h"
#include "sklt.h"
#include "wav.h"
#include "windp.h"
#include "ypabact.h"

#include "lstvw.h"

#include "glob_funcs.h"

extern class_stored ypaworld_class_off;

struct NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;
struct NC_STACK_button;

struct vhclBases;
struct cityBases;
struct subSec;
struct secType;
struct VhclProto;
struct WeapProto;
struct BuildProto;
struct roboProto;

struct usr_str
{
    int id;
    const char *pstring;
};

struct audiotrack_adv
{
    int field_0;
    int field_4;
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
    int p1;
    int p2;
    int p3;
    int p4;
    int p5;
    int p6;
    int p7;
};

struct langDll_node : public nnode
{
    char langDllName[32];
};

struct profilesNode : public nnode
{
    int pStatus_3;
    char field_C;
    char profile_subdir[32];
    int16_t field_2D;
};


struct netType1
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    char field_40;
    char field_41;
    char field_42;
    char field_43;
    int field_44;
    int field_48;
    int field_4C;
    char field_50[64];
};

struct netType2
{
    char field_0[128];
    char field_80;
    char field_81;
    char field_82;
    char field_83;
    char field_84;
    int field_85;
    int field_89;
    int field_8D;
    int field_91;
    int field_95;
};

struct UserData
{
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    char user_name[34];

    _NC_STACK_ypaworld *p_ypaworld;
    struC5 *field_3A;
    int field_3E;
    int field_42;
    int field_46;
    int field_4A;
    int field_4E;

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
    listview input_listview;
    int field_D36;
    int field_D3A;
    int inp_joystick;
    int field_D42;
    int inp_altjoystick;
    int field_D4A;
    int field_D4E;
    int field_D52;

    int16_t field_D5A;
    int16_t field_0xd5c;

    int field_D5E;
    NC_STACK_button *video_button;
    listview video_listvw;
    int game_default_res;
    nlist video_mode_list;
    int field_FBE;
    listview d3d_listvw;
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
    int enemyindicator;
    int field_13BE;
    int field_13C2;
    NC_STACK_button *disk_button;
    listview disk_listvw;
    int field_1612;
    char usernamedir[300];
    int usernamedir_len;
    int16_t field_0x1744;
    ncDir *opened_dir;
    nlist files_list;
    char field_1756;
    char snd__flags1;
    int16_t field_0x1758;
    int16_t field_175A;
    int16_t field_0x175c;
    int16_t field_175E;
    int16_t field_0x1760;
    NC_STACK_button *locale_button;
    nlist lang_dlls;
    listview local_listvw;
    langDll_node *default_lang_dll;
    langDll_node *prev_lang;

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
    listview network_listvw;
    int16_t field_1C2E;
    int16_t field_0x1c30;
    int16_t field_1C32;
    int16_t field_0x1c34;

    int field_1C36;
    int16_t field_1C3A;
    int16_t field_0x1c3c;
    int field_1C3E;
    char field_1C42[64];

    int16_t field_1C84;
    int16_t field_1C86;
    const char *field_0x1c88;

    char callSIGN[64];

    char field_1CD5;
    char field_1CD6;
    char field_1CD7;

    int field_0x1cd8;
    int field_0x1cdc;

    char field_1CE8;
    char field_1CE9;
    char field_1CEA;

    int field_1CEF;

    int16_t field_1CF7;
    char field_1CF9[33][64];
    usr_str map_descriptions[256];
    int map_descriptions_count;

    int field_2888;
    int field_288C;

    netType2 netTP2[8];
    netType1 netTP1[4];
    NC_STACK_button *confirm_button;
    int field_0x2fb4;

    int field_0x2fbc;
    int field_0x2fc0;
    int field_0x2fc4;

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

    int field_5457;
    int field_545B;
};

struct recorder
{
    int field_0;
};

struct cellArea
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    char owner;
    char sec_type;
    char field_2E;
    char field_2F;
    int field_30;
    int field_34;
    char field_38;
    char field_39;
    char field_3A;
    char field_3B;
    nlist field_3C;
    float sector_height_meters;
    float smooth_height;
};

struct keysec
{
    int x;
    int y;
    cellArea *cell;
};

struct gateProto
{
    int field_0;
    int sec_x;
    int sec_y;
    int closed_bp;
    int opened_bp;
    int target_level_count;
    int target_levels[8];
    int keySectors_count;
    keysec keySectors[16];
    int mb_status;
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
};

struct mapRobo
{
    int16_t owner;
    int16_t vehicle;
    char gap_4[4];
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
};

struct gemProto
{
    int16_t field_0;
    int16_t building;
    int16_t sec_x;
    int16_t sec_y;
    int mb_status;
    char script[64];
    char msg_default[128];
    int16_t nw_vproto_num_1;
    int16_t nw_vproto_num_2;
    int16_t nw_vproto_num_3;
    int16_t nw_vproto_num_4;
    int16_t nw_bproto_num_1;
    int16_t nw_bproto_num_2;
    int16_t nw_bproto_num_3;
    int16_t nw_bproto_num_4;
    int16_t begin_action__line;
    int16_t end_action__line;
    int type;
};

struct dbmapProto
{
    int16_t size_x;
    int16_t size_y;
    char name[32];
};

struct mapProto
{
    char flags;
    char gap_1[3];
    int setNumber;
    int event_loop;
    int secXsize;
    int secYsize;
    int slow_connection;
    char sky[64];
    char typ[64];
    char own[64];
    char hgt[64];
    char blg[64];
    int mapRobos_count;
    mapRobo mapRobos[8];
    int squad_count;
    squadProto squads[96];
    char palettes[8][64];
    int mbmaps_count;
    dbmapProto mbmaps[4];
    int dbmap_count;
    dbmapProto dbmaps[4];
};

struct yw_buddy
{
    int commandid;
    int16_t type;
    int16_t field_6;
    int energy;
};

struct supetItemProto
{
    int type;
    int field_4;
    int countdown;
    int field_C;
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
};

struct stru_2d90
{
    char map_name[64];
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
    char movie[256];
    char win_movie[256];
    char lose_movie[256];
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
    char title[128];
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
    char title[128];
};

struct big_ypa_Brf
{
    NC_STACK_ilbm *mbmap_img;
    NC_STACK_ilbm *briefing_map;
    bitmap_intern *typ_map_bitm;
    stru_2d90 s2d90;
    mapProto map_prototype;
    int field_2E68;
    int field_2E6C;
    int field_2E70;
    int field_2E74;
    int field_2E78;
    int field_2E7C;
    int field_2E80;
    int field_2E84;
    const char *field_2E88;
    const char *LEVEL_BRIEF_INFO;
    brf_obj brf_objs;
    rstr_arg204 field_2F40;
    float field_2F64;
    float field_2F68;
    float field_2F6C;
    float field_2F70;
    float field_2F74;
    float field_2F78;
    float field_2F7C;
    float field_2F80;
    int field_2F84;
    int tp2_count;
    int field_2F8C;
    int field_2F90;
    brf_t2 tp2[32];
    base77Func field_4174;
    int field_419C;
    int field_41A0;
    NC_STACK_sklt *wireless_db[4];
    skeleton_64_stru *wireless_db_skels[4];
    NC_STACK_bitmap *copy2_of_ownmap;
    NC_STACK_bitmap *copy2_of_typmap;
    bitmap_intern *copy2_of_ownmap_bitm;
    bitmap_intern *copy2_of_typmap_bitm;
    int field_41D4;

    char movie[256];
};


struct bkg_pct
{
    int16_t size_x;
    int16_t size_y;
    char map_name[32];
};

struct mapINFO
{
    int field_0;
    float field_4;
    float field_8;
    float field_C;
    float field_10;
    int field_14[2];
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
    int bg_n;
    bkg_pct background_map[4];
    bkg_pct rollover_map[4];
    bkg_pct finished_map[4];
    bkg_pct enabled_map[4];
    bkg_pct mask_map[4];
    bkg_pct tut_background_map[4];
    bkg_pct tut_rollover_map[4];
    bkg_pct tut_mask_map[4];
    bkg_pct menu_map[4];
    bkg_pct input_map[4];
    bkg_pct settings_map[4];
    bkg_pct network_map[4];
    bkg_pct locale_map[4];
    bkg_pct save_map[4];
    bkg_pct about_map[4];
    bkg_pct help_map[4];
    bkg_pct brief_map[4];
    bkg_pct debrief_map[4];
    mapINFO mapInfos[256];
    NC_STACK_ilbm *ilbm_menu_map;
    NC_STACK_ilbm *ilbm_mask_map;
    NC_STACK_ilbm *ilbm_rollover_map;
    NC_STACK_ilbm *ilbm_finished_map;
    NC_STACK_ilbm *ilbm_enabled_map;
    int field_BE38;
};

struct rgbiColor
{
    BYTE r;
    BYTE g;
    BYTE b;
    BYTE i;
    DWORD color;
};

struct yw_movies
{
    char movies_names_present[9];
    char game_intro[256];
    char lose_extro[256];
    char win_extro[256];
    char user_intro[256];
    char kyt_intro[256];
    char taer_intro[256];
    char myk_intro[256];
    char sulg_intro[256];
    char black_intro[256];
};

struct slurp
{
    NC_STACK_base *skeletons_bas;
    skeleton_64_stru *skeleton_internal;
};

struct slurp2
{
    NC_STACK_sklt *skeleton;
    skeleton_64_stru *skeleton_internal;
};

struct save_status
{
    int p1;
    int p2;
    int16_t p3;
    int16_t p4;
    int16_t p5;
    int16_t p6;
    int pX[8];
};

struct _NC_STACK_ypaworld
{
    NC_STACK_ypaworld *self_full;
    UserData *GameShell;

    int sectors_maxX;
    int sectors_maxY;
    int sectors_maxX2;
    int sectors_maxY2;
    cellArea *cells;

    float map_Width_meters;
    float map_Height_meters;

    int set_number;
    NC_STACK_base *additionalSet;
    nlist field_48;
    nlist field_54;
    vhclBases *vhcls_models;
    cityBases *legos;
    subSec *subSectors;
    secType *secTypes;
    VhclProto *VhclProtos;
    WeapProto *WeaponProtos;
    BuildProto *BuildProtos;
    roboProto *RoboProtos;

    int16_t field_130[256];
    BYTE sqrt_table[64][64];

    NC_STACK_base *sky_loaded_base;
    int field_1368;
    NC_STACK_base *additionalBeeBox;
    NC_STACK_sklt *colsub_sklt;
    NC_STACK_sklt *colcomp_sklt;
    skeleton_64_stru *colsub_sklt_intrn;
    skeleton_64_stru *colcomp_sklt_intrn;
    NC_STACK_ilbm *tracyrmp_ilbm;
    NC_STACK_ilbm *shadermp_ilbm;
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

    char *buildDate;
    int field_1624;

    tiles_stru *tiles[92];
    nlist field_17a0;
    int16_t screen_width;
    int16_t screen_height;

    int field_17bc;
    int field_17c0;
    int field_17c4;
    int field_17c8;
    const char **tooltips;
    rgbiColor iniColors[70];

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

    NC_STACK_ilbm *pointers[11];
    bitmap_intern *pointers__bitm[11];
    int field_1b1c;

    __NC_STACK_ypabact *field_1b80;

    int field_2424;

    recorder *sceneRecorder;

    gemProto gems[8];

    int last_modify_vhcl;
    int last_modify_weapon;
    int last_modify_build;

    stru_LevelNet *LevelNet;
    stru_2d90 *field_2d90;
    big_ypa_Brf brief;

    int superbomb_wall_vproto;
    int superbomb_center_vproto;
    int field_7278;


    char lang_name[32];
    char *lang_strings;
    char *very_big_array__p_begin;
    char *lang_strings__end;
    char **string_pointers;
    char **string_pointers_p2;
    NC_STACK_ilbm *typ_map;
    NC_STACK_ilbm *own_map;
    NC_STACK_ilbm *blg_map;
    NC_STACK_ilbm *hgt_map;

    NC_STACK_bitmap *copyof_typemap;
    NC_STACK_bitmap *copyof_ownermap;

    int field_739A;

    char field_73CE;
    char snd__cdsound;

    save_status robo_map_status;
    save_status robo_finder_status;
    save_status vhcl_map_status;
    save_status vhcl_finder_status;
    int fxnumber;

    NC_STACK_windp *windp;

    int field_757E;

    int field_7586;

    int field_759A;
    int field_759E;
    char field_75A2; //array 64?

    char field_75E2[64]; //array 64?


    int netgame_exclusivegem;

    player_status playerstatus[8];

    int maxroboenergy;
    int maxreloadconst;

    yw_movies movies;
    int field_81AB;
    const char *field_81AF;

    int one_game_res;
    int shell_default_res;
    int game_default_res;

    float max_impulse;

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
};

struct NC_STACK_ypaworld : public NC_STACK_base
{
    _NC_STACK_ypaworld stack__ypaworld;
};


struct vhclBases
{
    NC_STACK_base *base;
    base_1c_struct *trigo;
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
    skeleton_64_stru *sklt_obj_intern;
    skeleton_64_stru *selected_sklt_intern;
    char field_10;
    char field_11;
    char field_12;
    char field_13;
    uint8_t field_14[16];
    lego_xyz field_24[16];
};

struct subSec
{
    int field_0;
    BYTE field_4;
    char field_5;
    char field_6;
    char field_7;
    int field_8;
};

struct secType
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
    subSec *field_4[3][3];
};

struct destFX
{
    char type_flag;
    char p1;
    float p2;
    float p3;
    float p4;
};

struct sndExt
{
    int field_0;
    int16_t field_4;
    int16_t field_6;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

struct vhclSndFX
{
    char sample_name[32];
    char extSampleNames[8][32];
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    int field_13C;
    int field_140;
    int16_t volume;
    int16_t pitch;
    int16_t time;
    int16_t slot;
    float mag0;
    float mag1;
    int16_t shk_time;
    int16_t shk_slot;
    float shk_mag0;
    float shk_mag1;
    float mute;
    float x;
    float y;
    float z;
    int extCount;
    sndExt sndExts[8];
};

struct __attribute__((packed)) VhclProto
{
    char model_id;
    char disable_enable_bitmask;
    char weapon;
    char field_3;
    int field_4;
    char mgun;
    char type_icon;
    char name[126];
    char field_88;
    int16_t vp_normal;
    int16_t vp_fire;
    int16_t vp_dead;
    int16_t vp_wait;
    int16_t vp_megadeth;
    int16_t vp_genesis;
    destFX dest_fx[16];
    vhclSndFX sndFX[12];
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
    char gap_1D8E[3];
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
    int16_t destFxCount;
    int16_t num_weapons;
    float gun_power;
    float gun_radius;
    int kill_after_shot;
    float scale_fx_p0;
    float scale_fx_p1;
    float scale_fx_p2;
    int scale_fx_p3;
    int16_t scale_fx_pXX;
    int16_t field_1DE7;
    int16_t field_1DE9;
    int16_t field_1DEB;
    int16_t field_1DED;
    int16_t field_1DEF;
    int16_t field_1DF1;
    int16_t field_1DF3;
    int16_t field_1DF5;
    int16_t field_1DF7;
    int16_t field_1DF9;
    int16_t field_1DFB;
    int16_t field_1DFD;
    int16_t field_1DFF;
    int16_t field_1E01;
    int16_t field_1E03;
    int16_t field_1E05;
    int16_t field_1E07;
    int16_t field_1E09;
    int16_t field_1E0B;
    int16_t field_1E0D;
    int16_t field_1E0F;
    int16_t field_1E11;
    int16_t field_1E13;
    int16_t field_1E15;
    int16_t field_1E17;
    int16_t field_1E19;
    int16_t field_1E1B;
    int16_t field_1E1D;
    int16_t field_1E1F;
    int16_t field_1E21;
    int16_t field_1E23;
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
    stack_vals *stack_pointer__position;
    stack_vals stak[8];
};

struct WeapProto
{
    char field_0;
    char enable_mask;
    int16_t model_id;
    char type_icon;
    char name[127];
    int16_t vp_normal;
    int16_t vp_fire;
    int16_t vp_dead;
    int16_t vp_wait;
    int16_t vp_megadeth;
    int16_t vp_genesis;
    int destFxCount;
    destFX dfx[16];
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
    stack_vals *pointer;
    stack_vals stack[8];
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
};

struct BuildProto
{
    uint8_t sec_type;
    char enable_mask;
    char model_id;
    char power;
    char type_icon;
    char name[127];
    int energy;
    vhclSndFX sndfx;
    buildSbact sbacts[8];
};

struct roboGun
{
    float robo_gun_pos_x;
    float robo_gun_pos_y;
    float robo_gun_pos_z;
    float robo_gun_dir_x;
    float robo_gun_dir_y;
    float robo_gun_dir_z;
    int field_18;
    char robo_gun_name[32];
    char robo_gun_type;
};

struct roboColl
{
    float robo_coll_radius;
    float robo_coll_x;
    float robo_coll_y;
    float robo_coll_z;
    int field_10;
    int field_14;
    int field_18;
};

struct roboProto
{
    float robo_viewer_x;
    float robo_viewer_y;
    float robo_viewer_z;
    mat3x3 matrix;
    int field_30;
    int field_34;
    float robo_viewer_max_up;
    float robo_viewer_max_down;
    float robo_viewer_max_side;
    roboGun guns[8];
    char robo_num_guns;
    float robo_dock_x;
    float robo_dock_y;
    float robo_dock_z;
    int field_239;
    char robo_coll_num;
    roboColl roboColls[16];
};

int load_fonts_and_icons(_NC_STACK_ypaworld *yw);
int yw_LoadSet(_NC_STACK_ypaworld *yw, int setID);


struct yw_arg172
{
    char *usertxt;
    char *field_4;
    int field_8;
    UserData *usr;
    int field_10;
};

struct yw_arg181_a
{
    int fld_0;
    int fld_1;
    int fld_2;
    int fld_3;
    int16_t fld_4_1;
    int16_t fld_4_2;
};

struct yw_arg181_b
{
    int fld_0;
    int fld_1;
    int fld_2;
    int fld_3;
    char fld_4[64];
};

struct yw_arg181
{
    yw_arg181_a *field_0;
    int field_4;
    char *field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
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

#endif
