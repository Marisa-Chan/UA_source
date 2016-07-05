#ifndef YROBO_H_INCLUDED
#define YROBO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored yparobo_class_off;

struct NC_STACK_yparobo;

struct robo_t1
{
    int field_0;
    int field_4;
};

struct __NC_STACK_yparobo
{
    NC_STACK_ypaworld *wrld;
    _NC_STACK_ypaworld *wrld_yw;
    __NC_STACK_ypabact *bact_internal;
    float field_c;
    rbcolls coll;
    float field_1D5;

    char field_1DA;
    int field_1DB;
    int field_1DF;
    int field_1E3;
    char field_1E7;
    char field_1e8;
    char field_1E9;
    char field_1EA;
    char field_1EB;
    char field_1ec;
    char field_1ED;
    char field_1EE;
    int dock_energ;
    int dock_cnt;
    int dock_user;
    int dock_time;
    xyz dock_tgt_pos;
    __NC_STACK_ypabact *dock_tgt_bact;
    char dock_tgType;
    char dock_aggr;
    int dock_tgt_comm_id;
    xyz dock_pos;
    int field_221;
    cellArea *field_225;  //For AI
    int field_229;  //For AI
    int field_22D;
    int field_231;
    int field_235; //??
    int field_239;
    cellArea *field_23D;  //For AI
    int field_241;  //For AI
    int field_245;
    int field_249;
    int field_24D; //??
    int field_251;
    cellArea *field_255;  //For AI
    int field_259;  //For AI
    int field_25D;
    int field_261;
    int field_265; //??
    int field_269;
    cellArea *field_26D;  //For AI
    int field_271;  //For AI
    int field_275;  //For AI
    int field_279;
    int field_27D;
    int field_281; //??
    int field_285;
    cellArea *field_289;  //For AI
    int field_28D;  //For AI
    int field_291;
    int field_295;
    int field_299; //??
    int field_29D;
    cellArea *field_2A1;  //For AI
    int field_2A5;  //For AI
    int field_2A9;  //For AI
    int field_2AD;
    int field_2B1; //??
    int field_2B5;
    cellArea *field_2B9;  //For AI
    int field_2BD;  //For AI
    int field_2C1;  //For AI
    int field_2C5;
    int field_2C9; //??
    int field_2CD;
    cellArea *field_2D1;  //For AI
    int field_2D5;  //For AI
    int field_2D9;  //For AI
    int field_2DD;
    int field_2E1; //??
    int field_2E5;  //For AI
    cellArea *field_2E9;  //For AI

    int field_2ED;  //For AI
    int field_2F1;  //For AI
    int field_2F5;  //For AI
    cellArea *field_2F9;  //For AI
    int field_2FD;  //For AI
    cellArea *pcell;

    int field_309;
    roboGun guns[8];

    int field_4F5; //??
    int field_4F9; //??
    int field_4FD; //??

    char field_501;
    char field_502;
    char field_503;

    int field_505;
    int field_509;
    int field_50D;
    int field_511;
    xyz field_515;
    int field_521;
    robo_t1 field_525[16];
    int field_5A5;
    int field_5A9;
};

struct NC_STACK_yparobo : public NC_STACK_ypabact
{
    __NC_STACK_yparobo stack__yparobo;
};

struct robo_arg128
{
    int flags;
    BactTarget tgt;
    char tgType;
    xyz tgt_pos;
    int prim_comm_id;
    int comm_id;
    float distance;
    __NC_STACK_ypabact *comm_bact;
    NC_STACK_ypabact *comm_bacto;
};

struct robo_arg133
{
    int type;
    int num;
    xyz pos;
    int field_14;
    int *hetero_vehicles;
};

struct robo_arg134
{
    __NC_STACK_ypabact *unit;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
};

struct robo_t2
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

struct robo_loct1
{
    int energ;
    float distance;
    int aggr;
    char tgType;
    xyz tgt_pos;
    __NC_STACK_ypabact *tgt_bact;
    int command_id;
    int forbidden;
    int want;
    int good;
    int bad;
    int job;
};

void ypabact_func65__sub0(__NC_STACK_ypabact *bact);

#endif // YROBO_H_INCLUDED
