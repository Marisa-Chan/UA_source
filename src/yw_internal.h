#ifndef  YWINT_H_INCLUDED
#define YWINT_H_INCLUDED

#include "def_parser.h"

struct Key_stru
{
    const char *title_by_language;
    const char *short_name;
    char KEYCODE;
    char field_9;
    char field_A;
    char field_B;
};

struct video_mode_node : public nnode
{
    char name[128];
    int sort_id;
    int16_t width;
    int16_t height;
};

struct struct_44dbf8
{
    UAskeleton::Data *sklt;
    float pos_x;
    float pos_y;
    float pos_z;
    int sec_x;
    int sec_y;
    cellArea *p_cell;
    int16_t field_1C;
    int16_t field_1E;
};

// For rendering
struct stru_a3
{
    cellArea *p_cell;
    int dword4;
    int dword8;
    float x;
    float y;
    float z;
    float smooth_height;
};

struct yw_137loc
{
    int field_0;
    vec3d pos;
};


int VhclProtoParser(scrCallBack *);
int WeaponProtoParser(scrCallBack *);
int BuildProtoParser(scrCallBack *);

int MoviesParser(scrCallBack *arg);
int BkgParser(scrCallBack *arg);
int Colors_Parser(scrCallBack *arg);
int Misc_Parse(scrCallBack *arg);
int SuperItem_Parse(scrCallBack *arg);

int LevelDataParser(scrCallBack *arg);
int MapParseSizes(scrCallBack *arg);
int parse_map_robos(scrCallBack *arg);

int LevelSquadParser(scrCallBack *arg);
int LevelGatesParser(scrCallBack *arg);
int LevelMbMapParser(scrCallBack *arg);
int LevelGemParser(scrCallBack *arg);
int LevelEnableParser(scrCallBack *arg);
int LevelMapsParser(scrCallBack *arg);
int LeveldbMapParser(scrCallBack *arg);
int LevelSuperItemsParser(scrCallBack *arg);

int yw_InitLevelNet(_NC_STACK_ypaworld *yw);
int yw_InitNetwork(_NC_STACK_ypaworld *yw);

void yw_setIniColor(_NC_STACK_ypaworld *yw, int color_id, int r, int g, int b, int i);
int yw_parse_color(_NC_STACK_ypaworld *yw, int color_number, char *color_string);
int yw_ScanLevels(_NC_STACK_ypaworld *yw);


int sub_4DA354(_NC_STACK_ypaworld *yw, const char *filename);
int yw_InitTooltips(_NC_STACK_ypaworld *yw);

int loadTOD(_NC_STACK_ypaworld *yw, const char *fname);
int writeTOD(_NC_STACK_ypaworld *yw, const char *fname, int tod);

void ypaworld_func158__sub4__sub0(_NC_STACK_ypaworld *yw, NC_STACK_bitmap *bitm);

void sb_0x4eb94c(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc, int object_id, int a5);
void ypaworld_func158__DrawVehicle(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc);

void set_keys_vals(_NC_STACK_ypaworld *yw);
void yw_draw_input_list(_NC_STACK_ypaworld *yw, UserData *usr);

int yw_loadSky(_NC_STACK_ypaworld *yw, const char *skyname);
//void yw_renderSky_test(_NC_STACK_ypaworld *yw, base77Func *rndr_params);

void fill_videmodes_list(UserData *usr);
void listSaveDir(UserData *usr, const char *saveDir);
int parseSaveUser(scrCallBack *arg);

void listLocaleDir(UserData *usr, const char *dirname);

int  ShellSoundsLoad(UserData *usr);

void ypaworld_func154__sub0(_NC_STACK_ypaworld *yw);

void ypaworld_func156__sub1(UserData *usr);
void sub_46C524(UserData *usr);
void yw_netcleanup(_NC_STACK_ypaworld *yw);

void sub_46D2B4(NC_STACK_ypaworld *obj, UserData *usr);
int parseSaveInput(scrCallBack *arg);


void ypaworld_func158__sub0(_NC_STACK_ypaworld *yw, UserData *usr);

void ypaworld_func158__sub4__sub1(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt);

void sub_4491A0(_NC_STACK_ypaworld *yw, const char *movie_fname);

void yw_freeDebrief(_NC_STACK_ypaworld *yw);
void yw_calcPlayerScore(_NC_STACK_ypaworld *yw);

int sub_4DA41C(_NC_STACK_ypaworld *yw, mapProto *mapp, const char *fname);

void ypaworld_func158__sub4__sub1__sub4(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt);

NC_STACK_base *load_set_base();

int sub_44A12C(_NC_STACK_ypaworld *yw, NC_STACK_base *base);
int yw_parse_lego(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil, NC_STACK_base *base);
int yw_parse_subSect(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_parse_sektor(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int sub_44A97C(_NC_STACK_ypaworld *yw, NC_STACK_base *base);

NC_STACK_bitmap * sub_44816C(NC_STACK_bitmap *src, const char *name);

void ypaworld_func158__sub3(_NC_STACK_ypaworld *yw, UserData *usr);

int sub_449678(_NC_STACK_ypaworld *yw, struC5 *struc, int kkode);
void sub_46D0F8(const char *path);

int init_prototypes(_NC_STACK_ypaworld *yw);
void sub_457BC0(UserData *usr);

int parseSaveVideo(scrCallBack *arg);
void sub_46D370(NC_STACK_ypaworld *obj, int a2);
int parseSaveSound(scrCallBack *arg);
int parseSaveLevelStatus(scrCallBack *arg);
int parseSaveBuddy(scrCallBack *arg);
int parseSaveShell(scrCallBack *arg);

int yw_write_callSign(_NC_STACK_ypaworld *yw, const char *filename, const char *callsign);
int yw_write_user(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_input(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_sound(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_video(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_level_status(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw, int lvlid);
int yw_write_levels_statuses(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw);
int yw_write_buddies(FSMgr::FileHandle *fil, _NC_STACK_ypaworld *yw);
int yw_write_shell(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_item_modifers(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);

void sb_0x44ac24(_NC_STACK_ypaworld *yw);
void sub_4EAC80(_NC_STACK_ypaworld *yw);

void sub_4811E8(_NC_STACK_ypaworld *yw, int id);


int sb_0x44ca90(_NC_STACK_ypaworld *yw, mapProto *mapProto, int levelID, int a5);
int cells_mark_type(_NC_STACK_ypaworld *yw, const char *a2);
int cells_mark_owner(_NC_STACK_ypaworld *yw, const char *a2);
int cells_mark_hight(_NC_STACK_ypaworld *yw, const char *a2);
int yw_createRobos(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int robos_count, mapRobo *robo);
int sub_44B9B8(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, const char *a3);
void yw_InitSquads(_NC_STACK_ypaworld *yw, int cnt, squadProto *squads);
void yw_InitBuddies(_NC_STACK_ypaworld *yw);
void yw_InitTechUpgradeBuildings(NC_STACK_ypaworld *yw, _NC_STACK_ypaworld *a2);
void yw_InitGates(_NC_STACK_ypaworld *yw);
void yw_InitSuperItems(_NC_STACK_ypaworld *yw);
void sub_44F748(_NC_STACK_ypaworld *yw);
int sb_0x451034(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw);
void splashScreen_OutText(_NC_STACK_ypaworld *yw, NC_STACK_display *w3d, const char *txt, int x, int y);
void sub_44DBF8(_NC_STACK_ypaworld *yw, int _dx, int _dz, int _dxx, int _dzz, struct_44dbf8 *a6, int flags);
void sub_44E07C(_NC_STACK_ypaworld *yw, struct_44dbf8 *arg);
void sub_44D8B8(ypaworld_arg136 *arg, struct_44dbf8 *loc);

void ypaworld_func2__sub0__sub0(_NC_STACK_ypaworld *yw);
void ypaworld_func2__sub0__sub1(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2);

NC_STACK_ypabact *yw_createUnit(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int model_id);
void sub_44BF34(vhclSndFX *sndfx);


void sb_0x4d7c08(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, base_64arg *bs64, int a2);
void sb_0x44fc60(_NC_STACK_ypaworld *yw, cellArea *cell, int secX, int secY, int a5, yw_arg129 *a6);
void  sub_44F500(_NC_STACK_ypaworld *yw, int id);

void sb_0x456384(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int x, int y, int ownerid2, int blg_id, int a7);

void ypaworld_func148__sub0(_NC_STACK_ypaworld *yw, int x, int y);
int ypaworld_func148__sub1(_NC_STACK_ypaworld *yw, int id, int a4, int x, int y, int ownerID2, char blg_ID);

void ypaworld_func137__sub0(ypaworld_arg137 *arg, struct_44dbf8 *a2);


void create_squad_man(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0(_NC_STACK_ypaworld *yw);

void draw_tooltip(_NC_STACK_ypaworld *yw);

void sub_4C40AC(_NC_STACK_ypaworld *yw);
__NC_STACK_ypabact *sub_449518(_NC_STACK_ypaworld *yw);


struct squadMan
{
    GuiList lstvw;
    __NC_STACK_ypabact *squads[24];
    int field_2A8;
    int field_2AC;
    int field_2B0;
    int field_2B4;
    int field_2B8;
    __NC_STACK_ypabact *field_2BC;
    int16_t field_2C0;
    int16_t field_2C2;
    int16_t field_2C4;
    int16_t field_2C6;
    int16_t field_2C8;
    int16_t field_2CA;
    int field_2CC;
    int field_2D0;
    int field_2D4;
    int field_2D8;
    int field_2DC;
};

struct energPanel : public GuiBase
{
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    int field_1DC;
    int field_1E0;
    int field_1E4;
};

struct bzd_t1
{
    int i;
    int d;
};

struct bzd : public GuiBase
{
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    uint8_t field_1DC[256];
    uint8_t field_2DC[256];
    uint8_t field_3DC[256];
    bzd_t1 field_4DC[512];
    int field_8DC;
    int field_8E0;
    int field_8E4;
    int field_8E8;
    int field_8EC;
    int field_8F0;
    int field_8F4;
    int field_8F8;
    int field_8FC;
    int field_900;
    int field_904;
    int field_908;
    int field_90C;
    int field_910;
    int field_914;
    int field_918;
    int field_91C;
    int field_920;
};

struct inflog_msg
{
    int id;
    int field_4;
    int field_8;
    char txt[128];
};

struct yw_infolog
{
    GuiList window;
    int field_248;
    int field_24C;
    int field_250;
    int field_254;
    int msg_count;
    inflog_msg msgs[64];
    uint32_t field_255C;
    int field_2560;
    int field_2564;
    int field_2568;
    int field_256C;
};

struct tehMap : GuiBase
{
    int16_t field_1CC;
    int16_t field_1CE;
    int16_t field_1D0;
    int16_t field_1D2;
    int16_t field_1D4;
    int16_t field_1D6;
    float field_1D8;
    float field_1DC;
    float field_1E0;
    float field_1E4;
    int field_1E8;
    char field_1EC;
    char field_1ED;
    char field_1EE;
    char field_1EF;
    int field_1F0;
    int field_1F4;
    int field_1F8;
    int field_1FC;
    int field_200;
    int field_204;
    int16_t field_208;
    int16_t field_20A;
    int16_t field_20C;
    int16_t field_20E;
    int16_t field_210;
    int16_t field_212;
    int16_t field_214;
    int16_t field_216;
    int16_t field_218;
    int16_t field_21A;
    int16_t field_21C;
    int16_t field_21E;
    float field_220;
    float field_224;
    int field_228;
    int field_22C;
    int field_230;
    int field_234;
    int field_238;
    int field_23C;
    int field_240;
    int field_244;
    int field_248;
    int field_24C;
    int field_250;
    float field_254;
    float field_258;
    float field_25C;
    float field_260;
    float field_264;
};

struct debrif_t1
{
    int owner;
    int status;
};

struct evnt
{
    int field_0;
    int (* field_4)(_NC_STACK_ypaworld *);
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C[8];
};

struct map_event
{
    int event_loop_id;
    int field_4;
    int field_8;
    int field_C;
    int event_count;
    evnt evnts[16];
};

void ypaworld_func64__sub7(_NC_STACK_ypaworld *yw, struC5 *inpt);
void ypaworld_func64__sub8(NC_STACK_ypaworld *, _NC_STACK_ypaworld *yw);
void ypaworld_func64__sub14(_NC_STACK_ypaworld *yw);

void ypaworld_func64__sub15(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub16(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub17(_NC_STACK_ypaworld *yw);

int sub_47B388(int a1, const char *a2);

void ypaworld_func159__real(NC_STACK_ypaworld *obj, yw_arg159 *arg);
void sb_0x4d7c08__sub0__sub0(_NC_STACK_ypaworld *yw);


typedef void ( *wis_color_func)(_NC_STACK_ypaworld *, float, float, float, float, uint32_t *, uint32_t *);

int sb_0x4d7c08__sub0__sub0__sub0(_NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub4(_NC_STACK_ypaworld *yw);
void yw_RenderHUDTarget(_NC_STACK_ypaworld *yw, sklt_wis *wis);

void yw_RenderCursorOverUnit(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact);

char * yw_RenderOverlayCursors(_NC_STACK_ypaworld *yw, char *cur);
char *yw_RenderUnitLifeBar(_NC_STACK_ypaworld *yw, char *cur, __NC_STACK_ypabact *bact);

void yw_RenderHUDRadare(_NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub2(_NC_STACK_ypaworld *yw);

void ypaworld_func64__sub20(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int dtime);
void ypaworld_func64__sub6(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub5(_NC_STACK_ypaworld *yw);

void sub_44F958(_NC_STACK_ypaworld *yw, cellArea *cell, char secX, char secY, uint8_t owner);

void ypaworld_func64__sub21(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, struC5 *arg);

void ypaworld_func64__sub1(_NC_STACK_ypaworld *yw, struC5 *inpt);

int sub_47EDDC(yw_f726c *hist, int bufsize);

void sub_4D12D8(_NC_STACK_ypaworld *yw, int id, int a3);
void sub_4D1594(_NC_STACK_ypaworld *yw, int id);
void sub_4D1444(_NC_STACK_ypaworld *yw, int id);
void sb_0x47b028(_NC_STACK_ypaworld *yw, bact_node *bct1, bact_node *bct2, int a3);

//Saves protos
int sb_0x479f4c(scrCallBack *scr);
int sub_479E30(scrCallBack *scr);
int sub_479D20(scrCallBack *scr);
int sub_479C40(scrCallBack *scr);
int sub_479B98(scrCallBack *scr);
int sub_479AB0(scrCallBack *scr);
int sub_479A30(scrCallBack *scr);
int sub_47965C(scrCallBack *scr);
int sub_479770(scrCallBack *scr);
int sub_4798D0(scrCallBack *scr);
int sub_47925C(scrCallBack *scr);
int sb_0x47f2d8(scrCallBack *scr);
int sub_4795B0(scrCallBack *scr);
int sub_4792D0(scrCallBack *scr);

int ypaworld_func64__sub4(_NC_STACK_ypaworld *yw, base_64arg *arg);
void ypaworld_func64__sub2(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub19(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub9(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub23(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub3(_NC_STACK_ypaworld *yw);
void sub_44A094(_NC_STACK_ypaworld *yw);

void ypaworld_func64__sub11(_NC_STACK_ypaworld *yw);
void sb_0x447720(_NC_STACK_ypaworld *yw, struC5 *inpt);
void recorder_update_time(_NC_STACK_ypaworld *yw, int dtime);
void recorder_write_frame(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub22(_NC_STACK_ypaworld *yw);
void ypaworld_func151__sub5(_NC_STACK_ypaworld *yw);

int yw_write_levelnum(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_ownermap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_buildmap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_energymap(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_units(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_wunderinfo(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_kwfactor(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_globals(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_superbomb(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_history(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_masks(_NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);

void sub_471AB8(_NC_STACK_ypaworld *yw);
void sub_47DB04(_NC_STACK_ypaworld *yw, char a2);

void ypaworld_func151__sub6(_NC_STACK_ypaworld *yw);
void ypaworld_func151__sub2(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw);
void sub_4C8EB4(_NC_STACK_ypaworld *yw, bact_node *bct);
void ypaworld_func151__sub0(_NC_STACK_ypaworld *yw);
void ypaworld_func151__sub1(_NC_STACK_ypaworld *yw);

NC_STACK_bitmap * loadDisk_screen(_NC_STACK_ypaworld *yw);
void draw_splashScreen(_NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen);
void deleteSplashScreen(_NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen);

void ypaworld_func184__sub0(_NC_STACK_ypaworld *yw, yw_f726c *hist_list, yw_arg184 *arg);
void yw_score(_NC_STACK_ypaworld *yw, yw_arg184 *arg, player_status *pl_status);
uint8_t *yw_histbf_read_evnt(uint8_t *st, yw_arg184 *arg);

void yw_RenderVector2D(_NC_STACK_ypaworld *yw, UAskeleton::Data *wire, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, uint32_t coloooor, wis_color_func color_func, wis_color_func color_func2, bool aspectCorrection = false);
uint32_t yw_GetColor(_NC_STACK_ypaworld *yw, int color_id);
void yw_debriefUpdate(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt);
void sub_4D6958(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *unit, samples_collection1 *collection);

int sub_4C885C();
void sb_0x4c87fc(_NC_STACK_ypaworld *yw, const char *a2, GuiBase *lstvw);
void sub_4476AC(_NC_STACK_ypaworld *yw);

int recorder_create_camera(_NC_STACK_ypaworld *yw);
int recorder_go_to_frame(_NC_STACK_ypaworld *yw, recorder *rcrd, int wanted_frame_id);
int recorder_open_replay(recorder *rcrd);
void recorder_stoprec(_NC_STACK_ypaworld *yw);

void debug_info_draw(_NC_STACK_ypaworld *yw, struC5 *inpt);
void ypaworld_func163__sub1(_NC_STACK_ypaworld *yw, recorder *rcrd, int a3);
void ypaworld_func163__sub2(_NC_STACK_ypaworld *yw, recorder *rcrd, __NC_STACK_ypabact *bact, struC5 *inpt);

char * sub_4E4F80(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float x, float y, int value, int maxval, int valCH, int valBG, const char *a10, const char *a11, int flag = 0);
void sub_4D0C24(_NC_STACK_ypaworld *yw, const char *a1, const char *a2);

void sub_4F1B34(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact);
void sub_4F1A60(__NC_STACK_ypabact *bact);
void sub_4D9550(_NC_STACK_ypaworld *yw, int arg);
void sub_47C1EC(_NC_STACK_ypaworld *yw, gemProto *gemProt, int *a3, int *a4);

void rotmat_to_euler(mat3x3 *mat, vec3d *out);
void euler_to_rotmat(vec3d *euler, mat3x3 *out);

void yw_NetMsgHndlLoop(_NC_STACK_ypaworld *yw);
void yw_NetDrawStats(_NC_STACK_ypaworld *yw);
#endif
