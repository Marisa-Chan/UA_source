#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"
#include "engine_tform.h"
#include "base.h"

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
    char id;

    int field_2E;
    int field_32;
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
    char owner;
    char field_3D4;
    char field_3D5;
    int field_3D6;
    int field_3DA;
    char field_3DE;
    char field_3DF;
    int field_3e0;

    float adist_sector;
    float adist_bact;
    float sdist_sector;
    float sdist_bact;

    int field_59c;

    int field_5A5;
    float mass;
    float force;
    float airconst;
    float airconst2;
    float maxrot;
    float field_5BD;
    float field_5C1;
    float field_5C5;
    mat3x3 field_5C9;
    float field_5ED;

    float field_605;
    float field_609;
    float field_60D;
    float field_611;

    xyz field_621;
    xyz field_62D;
    float field_639;
    float field_63D;
    float field_641;

    mat3x3 field_651;
    float height;
    float field_679;
    float field_67D;
    float field_681;
    float field_685;
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

    int field_941;
    int field_945;

    int field_94D;
    int field_951;

    int field_959;
    int field_95D;
    int field_961;
    int field_965;
    int field_969;
    char weapon;
    char field_96E;
    char mgun;
    char num_weapons;
    nlist list3;
    int field_97D;
    float fire_x;
    float fire_y;
    float fire_z;
    float gun_angle;
    float gun_angle2;
    int field_995;
    float gun_radius;
    float gun_power;
    int field_9A1;
    int field_9A5;
    int kill_after_shot;
    float field_9AD;
    int field_9B1;

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
};

#endif // YBACT_H_INCLUDED
