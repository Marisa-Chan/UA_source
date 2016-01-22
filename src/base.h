#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

#include "engine_tform.h"
#include "skeleton.h"

extern class_stored base_class_off;


struct NC_STACK_base;
struct NC_STACK_skeleton;

struct base_node: public nnode
{
    NC_STACK_base *self_full;
};

struct clss_node: public nnode
{
    NC_STACK_class *obj;
};

struct base_1c_struct
{
    base_1c_struct *parent_1c;
    float grp1_p1;
    float grp1_p2;
    float grp1_p3;
    float field_10;
    float field_14;
    float field_18;
    float grp3_p1;
    float grp3_p2;
    float grp3_p3;
    float scale_x;
    float scale_y;
    float scale_z;
    mat3x3 scale_rotation;
    mat3x3 field_58;
    int rot_x;
    int rot_y;
    int rot_z;
    int grp5_p1;
    int grp5_p2;
    int grp5_p3;
    int field_94;
};

struct polysDat
{
    void (*render_func)(void *);
    polysDatSub datSub;
};

struct polys
{
    int range;
    polysDat *data;
};

struct area_arg_65
{
    int field_0;
    polys *outPolys;
    polysDat *polyDat;
    int field_C;
    int field_10;
    base_1c_struct *glob_1c;
    base_1c_struct *base_1c;
    NC_STACK_skeleton *OBJ_SKELETON;
    skeleton_64_stru *field_20;
    float field_24;
    float field_28;
    float field_2C;
    float field_30;
    int field_34;
    int field_38;
};

struct __NC_STACK_base
{
    int ID;
    char field_4;
    char field_5;
    char field_6;
    char field_7;
    int field_8;
    NC_STACK_skeleton *OBJ_SKELETON;
    nlist ADES;
    base_1c_struct params3D;
    NC_STACK_base *parent_base;
    NC_STACK_base *field_b8;
    NC_STACK_base *field_bc;
    nlist KIDS;
    base_node kid_node;
    int field_D8;
    area_arg_65 field_DC;
    NC_STACK_nucleus *OBJT;
};

struct NC_STACK_base : public NC_STACK_nucleus
{
    __NC_STACK_base stack__base;
};

struct __attribute__((packed)) STRC_base
{
    __int16 p1;
    float grp1_p1;
    float grp1_p2;
    float grp1_p3;
    float grp3_p1;
    float grp3_p2;
    float grp3_p3;
    float scale_x;
    float scale_y;
    float scale_z;
    __int16 rot_x;
    __int16 rot_y;
    __int16 rot_z;
    __int16 grp5_p1;
    __int16 grp5_p2;
    __int16 grp5_p3;
    __int16 p17;
    __int16 p18;
    int p19;
    int p20;
};

struct flag_xyz
{
    int flag;
    float x;
    float y;
    float z;
};

struct flag_xyz2
{
    int flag;
    int x;
    int y;
    int z;
};

struct base_66_arg_struct //Parent info struct
{
    NC_STACK_base *parent;
    base_1c_struct *parent_field_1c;
    nlist *KIDS;
};

struct base77Func
{
    int field_0;
    int field_4;
    polys *curOutPoly;
    polysDat *polysData;
    polysDat *polysData_end;
    int field_14;
    int field_18;
    int field_1C;
    float field_20;
    float field_24;
};


struct base_64arg
{
    int field_0;
    int field_4;
    struC5 *field_8;
    int field_C;
    int field_10;
};

struct base73arg_sub
{
    BYTE gap0[125];
    float float7D;
    float float81;
    float float85;
    float float89;
    float float8D;
    DWORD dword91;
};

struct base73arg
{
    int field_0;
    int field_4;
    base73arg_sub *dword8;
};


void sub_430A20(base_1c_struct *s3d);
base_1c_struct *sub_430A28();
void sub_430A38(base_1c_struct *s3d);

#endif // BASE_H_INCLUDED
