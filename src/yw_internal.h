#ifndef  YWINT_H_INCLUDED
#define YWINT_H_INCLUDED

struct TSectorCollision
{
    UAskeleton::Data *sklt = NULL;
    vec3d pos;
    Common::Point Cell;
    int16_t CollisionType = 0;
    int16_t Flags = 0;
};

// For rendering
struct TRenderingSector
{
    cellArea *p_cell;
    int dword4;
    int dword8;
    float x;
    float y;
    float z;
    float smooth_height;
};


int yw_InitNetwork(NC_STACK_ypaworld *yw);




int loadTOD(NC_STACK_ypaworld *yw, const char *fname);
int writeTOD(NC_STACK_ypaworld *yw, const char *fname, int tod);

void sb_0x4eb94c(NC_STACK_ypaworld *yw, TBriefengScreen *brf, InputState *struc, int a5);
void ypaworld_func158__DrawVehicle(NC_STACK_ypaworld *yw, TBriefengScreen *brf, InputState *struc);

void set_keys_vals(NC_STACK_ypaworld *yw);
void yw_draw_input_list(NC_STACK_ypaworld *yw, UserData *usr);

int yw_loadSky(NC_STACK_ypaworld *yw, const std::string &skyname);
//void yw_renderSky_test(_NC_STACK_ypaworld *yw, base77Func *rndr_params);



void listLocaleDir(UserData *usr, const char *dirname);



void PlayIntroMovie(NC_STACK_ypaworld *yw);

void ypaworld_func156__sub1(UserData *usr);





void yw_calcPlayerScore(NC_STACK_ypaworld *yw);

int sub_4DA41C(NC_STACK_ypaworld *yw, TLevelDescription *mapp, const char *fname);

void ypaworld_func158__sub4__sub1__sub4(NC_STACK_ypaworld *yw, UserData *usr, InputState *inpt);

NC_STACK_base *load_set_base();

int sub_44A12C(NC_STACK_ypaworld *yw, NC_STACK_base *base);
int yw_parse_lego(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil, NC_STACK_base *base);
int yw_parse_subSect(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_parse_sektor(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int sub_44A97C(NC_STACK_ypaworld *yw, NC_STACK_base *base);

NC_STACK_bitmap * sub_44816C(NC_STACK_bitmap *src, const char *name);

void ypaworld_func158__sub3(NC_STACK_ypaworld *yw, UserData *usr);

void sub_46D0F8(const std::string &path);





int yw_write_callSign(NC_STACK_ypaworld *yw, const char *filename, const char *callsign);
int yw_write_user(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_input(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_sound(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_video(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_level_status(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw, int lvlid);
int yw_write_levels_statuses(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw);
int yw_write_buddies(FSMgr::FileHandle *fil, NC_STACK_ypaworld *yw);
int yw_write_shell(FSMgr::FileHandle *fil, UserData *usr);
int yw_write_item_modifers(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);

void sb_0x44ac24(NC_STACK_ypaworld *yw);





int sb_0x451034(NC_STACK_ypaworld *yw);
void splashScreen_OutText(NC_STACK_ypaworld *yw, const std::string &txt, int x, int y);


void ypaworld_func2__sub0__sub0(NC_STACK_ypaworld *yw);
void ypaworld_func2__sub0__sub1(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact1, NC_STACK_ypabact *bact2);




void ypaworld_func148__sub0(NC_STACK_ypaworld *yw, int x, int y);
int ypaworld_func148__sub1(NC_STACK_ypaworld *yw, int id, int a4, int x, int y, int ownerID2, char blg_ID);


void create_squad_man(NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0(NC_STACK_ypaworld *yw);

void draw_tooltip(NC_STACK_ypaworld *yw);

void sub_4C40AC(NC_STACK_ypaworld *yw);


class squadMan: public GuiList
{
public:
    NC_STACK_ypabact *squads[24];
    int field_2A8;
    int field_2AC;
    int field_2B0;
    int field_2B4;
    int field_2B8;
    NC_STACK_ypabact *field_2BC;
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

class energPanel : public GuiBase
{
public:
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

class bzd : public GuiBase
{
public:
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

class yw_infolog : public GuiList
{
public:
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

class tehMap : public GuiBase
{
public:
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
    int32_t MapViewMask;
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


void ypaworld_func64__sub7(NC_STACK_ypaworld *yw, InputState *inpt);
void ypaworld_func64__sub8(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub14(NC_STACK_ypaworld *yw);

void ypaworld_func64__sub15(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub16(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub17(NC_STACK_ypaworld *yw);

int sub_47B388(int a1, const std::string &a2);

void sb_0x4d7c08__sub0__sub0(NC_STACK_ypaworld *yw);


typedef void ( *wis_color_func)(NC_STACK_ypaworld *, float, float, float, float, SDL_Color *, SDL_Color *);

int sb_0x4d7c08__sub0__sub0__sub0(NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub4(NC_STACK_ypaworld *yw);
void yw_RenderHUDTarget(NC_STACK_ypaworld *yw, sklt_wis *wis);

void yw_RenderCursorOverUnit(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact);

char * yw_RenderOverlayCursors(NC_STACK_ypaworld *yw, char *cur);
char *yw_RenderUnitLifeBar(NC_STACK_ypaworld *yw, char *cur, NC_STACK_ypabact *bact);

void yw_RenderHUDRadare(NC_STACK_ypaworld *yw);
void sb_0x4d7c08__sub0__sub2(NC_STACK_ypaworld *yw);

void ypaworld_func64__sub20(NC_STACK_ypaworld *ywo, int dtime);
void ypaworld_func64__sub6(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub5(NC_STACK_ypaworld *yw);

void ypaworld_func64__sub21(NC_STACK_ypaworld *yw, InputState *arg);




int ypaworld_func64__sub4(NC_STACK_ypaworld *yw, base_64arg *arg);
void ypaworld_func64__sub2(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub19(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub9(NC_STACK_ypaworld *yw);

void ypaworld_func64__sub3(NC_STACK_ypaworld *yw);
void sub_44A094(NC_STACK_ypaworld *yw);

void FFeedback_Update(NC_STACK_ypaworld *yw);
void sb_0x447720(NC_STACK_ypaworld *yw, InputState *inpt);
void recorder_update_time(NC_STACK_ypaworld *yw, int dtime);
void recorder_write_frame(NC_STACK_ypaworld *yw);

int yw_write_levelnum(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_ownermap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_buildmap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
void yw_write_energymap(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_units(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_wunderinfo(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_kwfactor(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_globals(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_superbomb(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_history(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);
int yw_write_masks(NC_STACK_ypaworld *yw, FSMgr::FileHandle *fil);

void sub_471AB8(NC_STACK_ypaworld *yw);
void sub_47DB04(NC_STACK_ypaworld *yw, char a2);

void ypaworld_func151__sub1(NC_STACK_ypaworld *yw);

NC_STACK_bitmap * loadDisk_screen(NC_STACK_ypaworld *yw);
void draw_splashScreen(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen);
void deleteSplashScreen(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen);

void yw_RenderVector2D(NC_STACK_ypaworld *yw, UAskeleton::Data *wire, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, SDL_Color coloooor, wis_color_func color_func, wis_color_func color_func2, bool aspectCorrection = false);
void yw_debriefUpdate(NC_STACK_ypaworld *yw, InputState *inpt);

int sub_4C885C();
void sub_4476AC(NC_STACK_ypaworld *yw);

int recorder_create_camera(NC_STACK_ypaworld *yw);
int recorder_go_to_frame(NC_STACK_ypaworld *yw, recorder *rcrd, int wanted_frame_id);
int recorder_open_replay(recorder *rcrd);
void recorder_stoprec(NC_STACK_ypaworld *yw);

void ypaworld_func163__sub1(NC_STACK_ypaworld *yw, recorder *rcrd, int a3);

char * sub_4E4F80(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float x, float y, int value, int maxval, int valCH, int valBG, const std::string &txt1, const std::string &txt2, int flag = 0);
void sub_4D0C24(NC_STACK_ypaworld *yw, const std::string &a1, const std::string &a2);


void sub_4D9550(NC_STACK_ypaworld *yw, int arg);
void sub_47C1EC(NC_STACK_ypaworld *yw, MapGem *gemProt, int *a3, int *a4);

void yw_NetMsgHndlLoop(NC_STACK_ypaworld *yw);
void yw_NetDrawStats(NC_STACK_ypaworld *yw);
#endif
