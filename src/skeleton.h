#ifndef SKELETON_H_INCLUDED
#define SKELETON_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

extern class_stored skeleton_class_off;

struct skeleton_type1
{
    int field_0;
    xyz pos3f;
};

struct __attribute__((packed)) pol_indixes
{
    int16_t num_vertices;
    int16_t v1;
    int16_t v2;
    int16_t v3;
    ////may contain more than 3 or less
};

struct pol_entries2
{
    float field_0;
    float field_4;
    float field_8;
    float field_C;
};

struct skeleton_64_stru
{
    skeleton_type1 *POO;
    __int16 POO_NUM;
    __int16 sen_count;


    // it's one piece of data contain 2 parts
    // 1: pointers to pol_indixes
    // 2: array of pol_indixes (with variable number of verts)
    pol_indixes **pol_entries;

    int pol_count;
    skeleton_type1 *sen_entries;
    int field_14;
    pol_entries2 *triangles;
    skeleton_type1 *type2;
};

struct __NC_STACK_skeleton
{
    skeleton_64_stru *data;
};

struct NC_STACK_skeleton : public NC_STACK_rsrc
{
    __NC_STACK_skeleton stack__skeleton;
};

struct skeleton_129_arg
{
    skeleton_64_stru *skeleton;
    int sen_count;
};

struct skeleton_130_arg
{
    skeleton_64_stru *skeleton;
    int pol_count;
    int num_indexes;
};

struct base_1c_struct;

struct skeleton_arg_132
{
    base_1c_struct *base_1c;
    base_1c_struct *glob_1c;
    float field_8;
    float field_C;
};

struct skeleton_intern133
{
    skeleton_type1 *skt1;
    skeleton_type1 *skt2;
    tUtV *uv1;
    tUtV *uv2;
    int field_10;
    float field_14;
    float field_18;
};

struct skeleton_intern133_sb
{
    skeleton_intern133 intern133;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
};

struct skeleton_arg133
{
    int field_0;
    int field_4;
    polysDatSub *polysubDat;
    tUtV *field_C;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    float field_20;
};

#endif // SKELETON_H_INCLUDED
