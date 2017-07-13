#ifndef SKELETON_H_INCLUDED
#define SKELETON_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

#define SKELETON_MAX_VERTEX 10

struct skeleton_type1: public xyz
{
    int flags;
};

// Geometry plane Ax + Bx + Cz + D = 0
// A,B,C plane vector normal n

struct Polygon
{
    int16_t num_vertices;
    int16_t v[SKELETON_MAX_VERTEX];

    float A;
    float B;
    float C;
    float D;
};

struct skeleton_64_stru
{
    skeleton_type1 *POO;
    int16_t POO_NUM;
    int16_t sen_count;

    Polygon *pol_entries;

    int pol_count;
    skeleton_type1 *sen_entries;
    int field_14;
    skeleton_type1 *type2;
};

struct __NC_STACK_skeleton
{
    skeleton_64_stru *data;
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

struct TForm3D;

struct skeleton_arg_132
{
    TForm3D *base_1c;
    TForm3D *glob_1c;
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
    int polyID;
    int field_4;
    polysDatSub *rndrArg;
    tUtV *texCoords;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    float field_20;
};

class NC_STACK_skeleton: public NC_STACK_rsrc
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func65(rsrc **pres);
    virtual __NC_STACK_skeleton * skeleton_func128(stack_vals *);
    virtual size_t skeleton_func129(skeleton_129_arg *arg);
    virtual size_t skeleton_func130(skeleton_130_arg *arg);
    virtual size_t skeleton_func131(int *arg);
    virtual size_t skeleton_func132(skeleton_arg_132 *arg);
    virtual void * skeleton_func133(skeleton_arg133 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_skeleton() {
        memset(&stack__skeleton, 0, sizeof(stack__skeleton));
    };
    virtual ~NC_STACK_skeleton() {};

    virtual const char * getClassName() {
        return "skeleton.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_skeleton();
    };

    enum SKEL_ATT
    {
        SKEL_ATT_PSKELET = 0x80002000,
        SKEL_ATT_POINTSCNT = 0x80002001,
        SKEL_ATT_SENCNT = 0x80002002,
        SKEL_ATT_POLYCNT = 0x80002003,
        SKEL_ATT_POLYPNTCNT = 0x80002004
    };

    virtual skeleton_64_stru *getSKEL_pSkelet();
    virtual int getSKEL_pntCount();
    virtual int getSKEL_senCount();
    virtual int getSKEL_polyCount();


    //Data
    static const NewClassDescr description;

    __NC_STACK_skeleton stack__skeleton;
};

#endif // SKELETON_H_INCLUDED
