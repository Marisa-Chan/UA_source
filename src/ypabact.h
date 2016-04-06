#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"
#include "engine_tform.h"
#include "base.h"


// !!!! if period is small, then this never happen
#define BACT_MIN_ANGLE 0.0002

extern class_stored ypabact_class_off;

struct NC_STACK_ypabact;
struct __NC_STACK_ypabact;

struct NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;

struct cellArea;


struct bact_node : public nnode
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;
};

struct bact_6bd
{
    float field_0;
    xyz field_4;
    mat3x3 matrx;
    int field_34;
    vhclBases vp;
};

struct __NC_STACK_ypabact : public nnode
{
    NC_STACK_ypabact *self;
    int field_c;
    int field_10;
    cellArea *p_cell_area;
    float field_18;
    float field_1c;
    int16_t field_20;
    int16_t field_22;
    int field_24;
    int ypabact__id;
    uint8_t id;
    char field_2D;
    int field_2E;
    NC_STACK_ypabact *field_32; // parent robo?
    NC_STACK_ypabact *parent_bacto;
    __NC_STACK_ypabact *parent_bact;
    nlist list2;
    bact_node list_node;
    samples_collection1 field_5A;
    int field_3B2;
    int field_3B6;
    int field_3BA;
    float max_pitch;
    int energy;
    int energy_2;
    int reload_const_or_energy2;
    int16_t field_3CE;
    char shield;
    char field_3D1;
    char radar;
    uint8_t owner;
    char field_3D4;
    char field_3D5;
    int field_3D6;
    int field_3DA;
    char field_3DE;
    char field_3DF;
    int field_3e0;
    int field_3e4;
    __NC_STACK_ypabact *field_3e8;
    xyz field_3ec;
    __NC_STACK_ypabact *field_3f8;
    xyz field_3fc;

    float adist_sector;
    float adist_bact;
    float sdist_sector;
    float sdist_bact;
    xyz field_418[32];
    int16_t field_598;
    int16_t field_59A;
    int field_59c;
    uint8_t field_5a0;
    int field_5A1;
    int field_5A5;
    float mass;
    float force;
    float airconst;
    float airconst2;
    float maxrot;
    xyz viewer;
    mat3x3 field_5C9;
    float field_5ED;
    float field_5F1;
    float viewer_max_up;
    float viewer_max_down;
    float viewer_max_side;
    float field_601;
    xyz field_605;
    float field_611;

    xyz field_621;
    xyz field_62D;
    xyz field_639;
    xyz field_645;
    mat3x3 field_651;
    float height;
    float field_679;
    xyz field_67D;
    vhclBases vp_normal;
    vhclBases vp_fire;
    vhclBases vp_wait;
    vhclBases vp_dead;
    vhclBases vp_megadeth;
    vhclBases vp_genesis;
    int field_6B9;
    bact_6bd field_6BD[3];

    destFX dest_fx[16];
    float radius;
    float vwr_radius;
    float overeof;
    float vwr_overeof;
    float pos_x_cntr;

    float pos_y_cntr;

    base_1c_struct field_87D;
    int field_915;
    int field_919;
    int field_91D;
    int field_921;
    int field_925;
    int field_929;
    int field_92D;
    int field_931;
    int field_935;
    int field_939;
    int field_93D;
    int field_941;
    int field_945;

    int field_94D;
    int field_951;
    int field_955;
    int field_959;
    int field_95D;
    xyz field_961;
    int weapon;
    char field_96E;
    int mgun;
    char num_weapons;
    nlist list3;
    int field_97D;
    float fire_x;
    float fire_y;
    float fire_z;
    float gun_angle;
    float gun_angle2;
    float field_995;
    float gun_radius;
    float gun_power;
    int field_9A1;
    int field_9A5;
    int kill_after_shot;
    float field_9AD;
    __NC_STACK_ypabact *field_9B1;
    int16_t field_9B5;
    int16_t field_9B6;
    int field_9b8;

    float field_a14;
    float field_a18;
    float field_a1c;
    int field_a20;
    int field_a24;
    int field_a28;
    NC_STACK_base *models_bases[32];
    base_1c_struct *models_trigo[32];

    int field_B34;
    NC_STACK_ypaworld *wrld;
    _NC_STACK_ypaworld * field_B3C;
    vhclBases current_vp;
    nlist field_B48;

    bact_node field_B54;
    bact_node field_B64;
    int field_B74;
};

struct NC_STACK_ypabact : public NC_STACK_nucleus
{
    __NC_STACK_ypabact stack__ypabact;
};

struct bact_arg73
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;
    nlist *list;
};

struct bact_arg80
{
    xyz pos;
    int field_C;
};

struct bact_arg119
{
    int field_0;
    int field_4;
    int field_8;
};

struct ypabact_arg65
{
    int field_0;
    int field_4;
    struC5 *inpt;
    int numid;
    int field_10;
    int field_14;
    int field_18;
    __NC_STACK_ypabact *field_1C;
    cellArea *field_20;
    int field_24;
    int field_28;
    int field_2C;
    xyz field_30;
    __NC_STACK_ypabact *field_3C;
    int field_40;
};

struct bact_arg67
{
    int field_0;
    int field_4;
    __NC_STACK_ypabact *field_8;
    xyz targ;
};

struct bact_arg124
{
    float field_0;
    float field_4;
    float field_8;
    float field_C;
    int16_t field_10;
    int16_t field_12;
    xyz field_14[32];
};

struct bact_arg84
{
    int energy;
    __NC_STACK_ypabact *unit;
};

struct bact_arg88
{
    xyz pos1;
    xyz pos2;
};

struct bact_arg94
{
    int field_0;
    xyz pos1;
    xyz pos2;
};

struct bact_arg74
{
    float field_0;
    xyz vec;
    int flag;
};

struct bact_arg81
{
    int enrg_type;
    int enrg_sum;
};

struct bact_arg90
{
    __NC_STACK_ypabact *unit;
    __NC_STACK_ypabact *ret_unit;
    int field_8;
    float pos_x;
    float pos_z;
};

struct bact_arg109
{
    int field_0;
    __NC_STACK_ypabact *field_4;
};

struct bact_arg92
{
    xyz pos;
    int energ1;
    int energ2;
    int field_14;
};

struct bact_hudi
{
    int field_0;
    int field_4;
    float field_8;
    float field_C;
    float field_10;
    float field_14;
    __NC_STACK_ypabact *field_18;
};

struct bact_arg105
{
    xyz field_0;
    float field_C;
    int field_10;
};

struct bact_arg106
{
    int field_0;
    xyz field_4;
    __NC_STACK_ypabact *ret_bact;
};

struct bact_arg79
{
    xyz field_0;
    xyz fire_point;
    int field_18;
    __NC_STACK_ypabact *field_1C;
    xyz field_20;
    int field_2C;
    int field_30;
    int field_34;
};

struct bact_arg75
{
    xyz field_0;
    __NC_STACK_ypabact *bct;
    int field_x;
    float field_14;
    int field_18;
};

struct bact_arg110
{
    int field_one;
    int field_two;
};

struct bact_arg86
{
    int field_one;
    int field_two;
};

struct bact_arg101
{
    xyz pos;
    int unkn;
    float radius;
};

struct bact_arg83
{
    int energ;
    xyz pos;
    xyz pos2;
    float force;
    float mass;
};


void sub_493DB0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, NC_STACK_ypaworld *ywo);

#endif // YBACT_H_INCLUDED
