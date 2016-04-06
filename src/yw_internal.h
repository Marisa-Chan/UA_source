#ifndef  YWINT_H_INCLUDED
#define YWINT_H_INCLUDED

extern polys *p_outPolys;
extern polysDat *p_polysdata;
extern polysDat *p_polysdata_end;

struct lvlnet_t1
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
};

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
    skeleton_64_stru *sklt;
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
    xyz pos;
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

void ypaworld_func158__sub4__sub0(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *bitm);

void sb_0x4eb94c(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc, int object_id, int a5);
void ypaworld_func158__DrawVehicle(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc);

void set_keys_vals(_NC_STACK_ypaworld *yw);
int yw_draw_input_list(_NC_STACK_ypaworld *yw, UserData *usr);

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

void sub_4EE04C(_NC_STACK_ypaworld *yw);
void sub_4F0FFC(_NC_STACK_ypaworld *yw);

int sub_4DA41C(_NC_STACK_ypaworld *yw, mapProto *mapp, const char *fname);

int sub_4D7BFC(const void *a1, const void *a2);

void ypaworld_func158__sub4__sub1__sub4(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *inpt);

NC_STACK_base *load_set_base();

int sub_44A12C(_NC_STACK_ypaworld *yw, NC_STACK_base *base);
int yw_parse_lego(_NC_STACK_ypaworld *yw, FILE *fil, NC_STACK_base *base);
int yw_parse_subSect(_NC_STACK_ypaworld *yw, FILE *fil);
int yw_parse_sektor(_NC_STACK_ypaworld *yw, FILE *fil);
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
int yw_write_user(FILE *fil, UserData *usr);
int yw_write_input(FILE *fil, UserData *usr);
int yw_write_sound(FILE *fil, UserData *usr);
int yw_write_video(FILE *fil, UserData *usr);
int yw_write_levels_statuses(FILE *fil, _NC_STACK_ypaworld *yw);
int yw_write_buddies(FILE *fil, _NC_STACK_ypaworld *yw);
int yw_write_shell(FILE *fil, UserData *usr);
int yw_write_item_modifers(_NC_STACK_ypaworld *yw, FILE *fil);

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
void splashScreen_OutText(_NC_STACK_ypaworld *yw, NC_STACK_win3d *w3d, const char *txt, int x, int y);
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
    listview lstvw;
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

struct energPanel : public listbase
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
    char i;
    char d;
};

struct bzd : public listbase
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
    listview window;
    int field_248;
    int field_24C;
    int field_250;
    int field_254;
    int msg_count;
    inflog_msg msgs[64];
    int field_255C;
    int field_2560;
    int field_2564;
    int field_2568;
    int field_256C;
};

struct tehMap : listbase
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

void ypaworld_func64__sub7(_NC_STACK_ypaworld *yw, struC5 *inpt);
void ypaworld_func64__sub8(NC_STACK_ypaworld *, _NC_STACK_ypaworld *yw);
void ypaworld_func64__sub14(_NC_STACK_ypaworld *yw);

void ypaworld_func64__sub15(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub16(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub17(_NC_STACK_ypaworld *yw);

int sub_47B388(int a1, const char *a2);

void ypaworld_func159__real(NC_STACK_ypaworld *obj, class_stru *zis, yw_arg159 *arg);
void sb_0x4d7c08__sub0__sub0(_NC_STACK_ypaworld *yw);


typedef void ( *wis_color_func)(_NC_STACK_ypaworld *, float, float, float, float, uint32_t *, uint32_t *);

int sb_0x4d7c08__sub0__sub0__sub0(_NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub4(_NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub4__sub1__sub1(_NC_STACK_ypaworld *yw, sklt_wis *wis);

void sub_4E3570(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact);

char * sb_0x4d7c08__sub0__sub4__sub0(_NC_STACK_ypaworld *yw, char *cur);
char *sub_4E2B5C(_NC_STACK_ypaworld *yw, char *cur, __NC_STACK_ypabact *bact);

void sb_0x4d7c08__sub0__sub4__sub2(_NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub2(_NC_STACK_ypaworld *yw);

void ypaworld_func64__sub20(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int dtime);
void ypaworld_func64__sub6(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub5(_NC_STACK_ypaworld *yw);

void sub_44F958(_NC_STACK_ypaworld *yw, cellArea *cell, char secX, char secY, uint8_t owner);

void ypaworld_func64__sub21(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, struC5 *arg);

void ypaworld_func64__sub1(_NC_STACK_ypaworld *yw, struC5 *inpt);

#endif
