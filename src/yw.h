#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include <string.h>

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
#include "windp.h"
#include "ypabact.h"
#include "ypamissile.h"
#include "ypagun.h"

#include "lstvw.h"

#include "input.h"

#include "glob_funcs.h"


#define YW_RENDER_SECTORS_DEF   5


class NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;
class NC_STACK_button;

struct vhclBases;
struct cityBases;
struct subSec;
struct secType;
struct VhclProto;
struct WeapProto;
struct BuildProto;
struct roboProto;
struct map_event;

struct usr_str
{
    int id;
    const char *pstring;
};

struct audiotrack_adv
{
    int min_delay;
    int max_delay;
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

    player_status()
    {
        p1 = 0;
        p2 = 0;
        p3 = 0;
        p4 = 0;
        p5 = 0;
        p6 = 0;
        p7 = 0;
    }
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
    int latency;
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
    GuiList input_listview;
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
    GuiList video_listvw;
    int game_default_res;
    nlist video_mode_list;
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
    int enemyindicator;
    int field_13BE;
    int field_13C2;
    NC_STACK_button *disk_button;
    GuiList disk_listvw;
    int field_1612;
    char usernamedir[300];
    int usernamedir_len;
    int16_t field_0x1744;
    FSMgr::DirIter *opened_dir;
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
    GuiList local_listvw;
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
    GuiList network_listvw;
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

    char field_0x1cd4;
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

    int field_283F;

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

    int netsend_speed;
    int netrecv_speed;

    int net_packet_size;
};

struct trec_bct
{
    int bact_id;
    xyz pos;
    int8_t rot_x;
    int8_t rot_y;
    int8_t rot_z;
    int vp_id;
    int bact_type;
    int vhcl_id;
};

struct recorder
{
    MFILE *mfile;
    uint16_t seqn;
    uint16_t level_id;
    int frame_id;
    int time;
    int ctrl_bact_id;

    __NC_STACK_ypabact **bacts;
    trec_bct *oinf;
    uint16_t *sound_status;
    void *field_20;
    uint8_t *ainf;

    int max_bacts;
    int field_2C;
    int bacts_count;
    int field_34;
    int ainf_size;
    int do_record;
    int field_40;
    xyz field_44;
    mat3x3 rotation_matrix;
    int field_74;
    int field_78;
    int field_7C;
    int field_80;
    int field_84;
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

    void init()
    {
        field_0 = 0;
        building = 0;
        sec_x = 0;
        sec_y = 0;
        mb_status = 0;
        memset(script, 0, sizeof(script));
        memset(msg_default, 0, sizeof(msg_default));
        nw_vproto_num_1 = 0;
        nw_vproto_num_2 = 0;
        nw_vproto_num_3 = 0;
        nw_vproto_num_4 = 0;
        nw_bproto_num_1 = 0;
        nw_bproto_num_2 = 0;
        nw_bproto_num_3 = 0;
        nw_bproto_num_4 = 0;
        begin_action__line = 0;
        end_action__line = 0;
        type = 0;
    }

    gemProto()
    {
        init();
    };
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

struct brf_t1
{
    int16_t field_0;
    int16_t last_vhcl;
    int16_t last_weapon;
    int16_t last_build;
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
    ua_fRect field_2F64;
    ua_fRect field_2F74;
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
    int field_41D8;
    player_status copy_of_playerstatus[8];
    player_status field_42BC[8];
    char movie[256];
    int tp1_count;
    brf_t1 tp1[7];
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
    int field_14[2]; //Was ist das?
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

    slurp()
    {
        skeletons_bas = NULL;
        skeleton_internal = NULL;
    }
};

struct slurp2
{
    NC_STACK_sklt *skeleton;
    skeleton_64_stru *skeleton_internal;

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
    skeleton_64_stru *sklts_intern[14];
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
    int pX[8];

    save_status()
    {
        p1 = 0;
        p2 = 0;
        p3 = 0;
        p4 = 0;
        p5 = 0;
        p6 = 0;
        memset(pX, 0, sizeof(pX));
    }
};

struct yw_81cb
{
    int field_0;
    int field_4;
    char field_8[128];
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
    BuildProto *BuildProtos;
    roboProto *RoboProtos;
    yw_f80 field_80[8];
    int16_t build_hp_ref[256];
    uint8_t sqrt_table[64][64];
    __NC_STACK_ypabact *current_bact;
    xyz field_1334;
    mat3x3 field_1340;
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

    int audio_volume;
    int field_1604;
    int field_1608;
    int field_160c;
    int field_1610;
    int field_1614;
    int field_1618;
    int field_161c;
    char *buildDate;
    int field_1624;
    int16_t field_1628;
    int16_t field_162A;
    int field_162c;
    tiles_stru *tiles[92];
    nlist field_17a0;
    int16_t screen_width;
    int16_t screen_height;
    int field_17b0;
    GuiBase *field_17b4;
    shortPoint field_17b8;
    int field_17bc;
    int field_17c0; // Grab mouse for unit steer-turn
    int field_17c4;
    int field_17c8;
    const char **tooltips;
    rgbiColor iniColors[70];
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
    xyz field_1a6c;

    xyz field_1a7c;

    xyz field_1a8c;
    __NC_STACK_ypabact *field_1a98;
    float field_1a9c;
    int field_1aa0;
    int field_1aa4;
    int field_1aa8;
    __NC_STACK_ypabact *field_1aac;
    int field_1ab0;
    int field_1ab4;
    xyz field_1ab8;

    NC_STACK_ilbm *pointers[11];
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
    NC_STACK_ypabact *field_1b78;
    NC_STACK_ypabact *field_1b7c;
    __NC_STACK_ypabact *field_1b80;
    __NC_STACK_ypabact *field_1b84;
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
    int field_241c;
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
    NC_STACK_ilbm *typ_map;
    NC_STACK_ilbm *own_map;
    NC_STACK_ilbm *blg_map;
    NC_STACK_ilbm *hgt_map;

    NC_STACK_bitmap *copyof_typemap;
    NC_STACK_bitmap *copyof_ownermap;
    sklt_wis wis_skeletons;
    int field_739A;

    char field_73CE;
    char snd__cdsound;

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
    int field_756A;
    int field_756E;

    NC_STACK_windp *windp;

    int field_757E;

    int field_7586;

    int field_7592;

    int field_759A;
    int field_759E;
    char field_75A2; //array 64?

    char field_75E2[64]; //array 64?


    int netgame_exclusivegem;
    int field_7626;

    int p_1_grp_cnt;
    int p_1_grp[4][8];
    player_status playerstatus[8];
    player_status field_7796[8];
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
    yw_movies movies;
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


struct __attribute__((packed)) vhclSndFX
{
    char sample_name[32];
    char extSampleNames[8][32];
    NC_STACK_wav *single_sample;
    NC_STACK_wav *wavs[8];
    int16_t volume;
    int16_t pitch;
    sndFXprm sndPrm;
    sndFXprm2 sndPrm_shk;
    sndExtends extS;
};

struct __attribute__((packed)) VhclProto
{
    char model_id;
    char disable_enable_bitmask;
    int8_t weapon;
    uint8_t field_3;
    int field_4;
    char mgun;
    uint8_t type_icon;
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
    int16_t scale_fx_pXX[33];
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
    uint8_t model_id;
    uint8_t power;
    uint8_t type_icon;
    char name[127];
    int energy;
    vhclSndFX sndfx;
    buildSbact sbacts[8];
};

struct roboGun
{
    xyz pos;
    xyz dir;
    NC_STACK_ypabact *gun_obj;
    char robo_gun_name[32];
    uint8_t robo_gun_type;
};

struct roboProto
{
    xyz viewer;
    mat3x3 matrix;
    int field_30;
    int field_34;
    float robo_viewer_max_up;
    float robo_viewer_max_down;
    float robo_viewer_max_side;
    roboGun guns[8];
    char robo_num_guns;
    xyz dock;
    rbcolls coll;
};

int load_fonts_and_icons(_NC_STACK_ypaworld *yw);
int yw_LoadSet(_NC_STACK_ypaworld *yw, int setID);


struct yw_arg172
{
    const char *usertxt;
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
    void *value;
    int val_size;
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

struct ypaworld_arg146
{
    int vehicle_id;
    xyz pos;
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
    int field_0;
    float pos_x;
    float pos_y;
    float pos_z;
    int field_10;
    float field_14;
    float field_18;
    float field_1C;
    int field_20;
    float field_24;
    int field_28;
    float field_2C;
    float field_30;
    float field_34;
    int field_38;
    skeleton_64_stru *field_3C;
    int field_40;
};

struct yw_137col
{
    int field_0;
    xyz pos1;
    xyz pos2;
    int field_1C;
};

struct ypaworld_arg137
{
    int field_0;
    xyz pos;
    int field_10;
    xyz pos2;
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
    xyz pos;
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
    xyz pos;
    int field_14;
    xyz field_18;
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
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *arg);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
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
    virtual void ypaworld_func151(stack_vals *arg);
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
    virtual void ypaworld_func164(void *arg);
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
    virtual void ypaworld_func181(void *arg);
    virtual void ypaworld_func182(void *arg);
    virtual size_t ypaworld_func183(yw_arg161 *arg);
    virtual void ypaworld_func184(yw_arg184 *arg);
    virtual void ypaworld_func185(void *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypaworld()
    {
    };
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


    int yw_initAttrs(stack_vals *stak);
    void ypaworld_func2__sub0(stack_vals *stak);
    void ypaworld_func3__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    _NC_STACK_ypaworld stack__ypaworld;

};

__NC_STACK_ypabact * sub_48C244(NC_STACK_ypaworld *ywo, int a2, char owner);

#endif
