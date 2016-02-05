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


int sb_0x44ca90(_NC_STACK_ypaworld *yw, mapProto *mapProto, int levelID, int a5);
int cells_mark_type(_NC_STACK_ypaworld *yw, const char *a2);
int cells_mark_owner(_NC_STACK_ypaworld *yw, const char *a2);
int cells_mark_hight(_NC_STACK_ypaworld *yw, const char *a2);
int yw_createRobos(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int robos_count, mapRobo *robo);
int sub_44B9B8(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, const char *a3);
void ypaworld_func161__sub1(_NC_STACK_ypaworld *yw, unsigned int a2, squadProto *a3);
void sub_471CA0(_NC_STACK_ypaworld *yw);
void sub_44C248(NC_STACK_ypaworld *yw, _NC_STACK_ypaworld *a2);
void sub_472130(_NC_STACK_ypaworld *yw);
void sub_4D1084(_NC_STACK_ypaworld *yw);
void sub_44F748(_NC_STACK_ypaworld *yw);
int sb_0x451034(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw);
void sb_0x44fc60(_NC_STACK_ypaworld *yw, cellArea *cell, int secX, int secY, int a5, int a6);
void splashScreen_OutText(_NC_STACK_ypaworld *yw, NC_STACK_win3d *w3d, const char *txt, int x, int y);
void sub_44DBF8(_NC_STACK_ypaworld *yw, int _dx, int _dz, int _dxx, int _dzz, struct_44dbf8 *a6, int flags);
void sub_44E07C(_NC_STACK_ypaworld *yw, struct_44dbf8 *arg);
void sub_44D8B8(ypaworld_arg136 *arg, struct_44dbf8 *loc);

void ypaworld_func2__sub0__sub0(_NC_STACK_ypaworld *yw);
void ypaworld_func2__sub0__sub1(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2);

NC_STACK_ypabact *yw_createUnit(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, int model_id);
void sub_44BF34(vhclSndFX *sndfx);


void sb_0x4d7c08(NC_STACK_ypaworld *ywo, _NC_STACK_ypaworld *yw, base_64arg *bs64, int a2);

#endif
