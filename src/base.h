#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

#include "engine_tform.h"
#include "skeleton.h"


class NC_STACK_base;
class NC_STACK_skeleton;
class NC_STACK_ade;

struct base_node: public nnode
{
    NC_STACK_base *self_full;
};

struct clss_node: public nnode
{
    NC_STACK_nucleus *obj;
};

struct base_1c_struct
{
    base_1c_struct *parent_1c;
    xyz grp_1;
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
    polys *rndrSTK_cur;
    polysDat *argSTK_cur;
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
    char flags;
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

struct __attribute__((packed)) STRC_base
{
    int16_t p1;
    xyz grp_1;
    float grp3_p1;
    float grp3_p2;
    float grp3_p3;
    float scale_x;
    float scale_y;
    float scale_z;
    int16_t rot_x;
    int16_t rot_y;
    int16_t rot_z;
    int16_t grp5_p1;
    int16_t grp5_p2;
    int16_t grp5_p3;
    int16_t p17;
    int16_t p18;
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

struct baseRender_msg
{
    int field_0;
    int field_4;
    polys *rndrSTK_cur;
    polysDat *argSTK_cur;
    polysDat *argSTK_end;
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

void sub_430A20(base_1c_struct *s3d);
base_1c_struct *sub_430A28();
void sub_430A38(base_1c_struct *s3d);

struct vhclBases
{
    NC_STACK_base *base;
    base_1c_struct *trigo;
};


class NC_STACK_base: public NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual size_t base_func64(base_64arg *arg);
    virtual size_t base_func65(NC_STACK_base **kid);
    virtual size_t base_func66(base_66_arg_struct *prnt_info);
    virtual size_t base_func67(NC_STACK_base **arg);
    virtual size_t base_func68(flag_xyz *arg);
    virtual size_t base_func69(flag_xyz *arg);
    virtual size_t base_func70(flag_xyz2 *arg);
    virtual size_t base_func71(flag_xyz2 *arg);
    virtual size_t base_func72(flag_xyz *arg);
    virtual size_t base_func73(base_64arg *arg);
    virtual size_t base_func77(baseRender_msg *arg);
    virtual size_t base_func78(base_64arg *arg);
    virtual size_t base_func79(NC_STACK_base **arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_base() {
        memset(&stack__base, 0, sizeof(stack__base));
    };
    virtual ~NC_STACK_base() {};

    virtual const char * getClassName() {
        return "base.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_base();
    };

    enum BASE_FLAG
    {
        BASE_FLAG_MOVING = 1,
        BASE_FLAG_ROTATING = 2,
        BASE_FLAG_MAINKID = 4,
        BASE_FLAG_RENDERALL = 8,
        BASE_FLAG_TERMCOLL = 0x10,
        BASE_FLAG_INPUTHANDLE = 0x20,
        BASE_FLAG_MAINOBJT = 0x40,
        BASE_FLAG_EMBDRSRC = 0x80
    };

    enum BASE_ATT
    {
        BASE_ATT_SKELET = 0x80001000,
        BASE_ATT_ADE = 0x80001001,
        BASE_ATT_PARENTFOLLOW = 0x80001002,
        BASE_ATT_VISLIMIT = 0x80001004,
        BASE_ATT_AMBIENTLIGHT = 0x80001005,
        BASE_ATT_RENDERALL = 0x80001006,
        BASE_ATT_INPUTHANDLE = 0x80001008,
        BASE_ATT_X = 0x80001009,
        BASE_ATT_Y = 0x8000100A,
        BASE_ATT_Z = 0x8000100B,
        BASE_ATT_VX = 0x8000100C,
        BASE_ATT_VY = 0x8000100D,
        BASE_ATT_VZ = 0x8000100E,
        BASE_ATT_AX = 0x8000100F,
        BASE_ATT_AY = 0x80001010,
        BASE_ATT_AZ = 0x80001011,
        BASE_ATT_RX = 0x80001012,
        BASE_ATT_RY = 0x80001013,
        BASE_ATT_RZ = 0x80001014,
        BASE_ATT_SX = 0x80001015,
        BASE_ATT_SY = 0x80001016,
        BASE_ATT_SZ = 0x80001017,
        BASE_ATT_ADELIST = 0x80001018,
        BASE_ATT_PTRANSFORM = 0x80001019,
        BASE_ATT_KIDSLIST = 0x8000101A,
        BASE_ATT_KIDNODE = 0x8000101B,
        BASE_ATT_RENDERPARAMS = 0x8000101D,
        BASE_ATT_MAINKID = 0x8000101E,
        BASE_ATT_MAINOBJT = 0x8000101F,
        BASE_ATT_RENDERSTACK = 0x80001020,
        BASE_ATT_ARGSTACK = 0x80001021,
        BASE_ATT_ENDARGSTACK = 0x80001022,
        BASE_ATT_FADELEN = 0x80001023,
        BASE_ATT_STATIC = 0x80001024,
        BASE_ATT_EMBDRSRC = 0x80001025
    };

    virtual void setBASE_skeleton(NC_STACK_skeleton *);
    virtual void setBASE_ADE(NC_STACK_ade *);
    virtual void setBASE_parentFollow(int);
    virtual void setBASE_visLimit(int);
    virtual void setBASE_ambientLight(int);
    virtual void setBASE_renderAll(int);
    virtual void setBASE_inputHandle(int);
    virtual void setBASE_fadeLength(int);
    virtual void setBASE_static(int);
    virtual void setBASE_embdRsrc(int);

    virtual NC_STACK_skeleton *getBASE_skeleton();
    virtual int getBASE_parentFollow();
    virtual int getBASE_visLimit();
    virtual int getBASE_ambientLight();
    virtual int getBASE_renderAll();
    virtual int getBASE_inputHandle();
    virtual float getBASE_x();
    virtual float getBASE_y();
    virtual float getBASE_z();
    virtual float getBASE_vx();
    virtual float getBASE_vy();
    virtual float getBASE_vz();
    virtual int getBASE_ax();
    virtual int getBASE_ay();
    virtual int getBASE_az();
    virtual int getBASE_rx();
    virtual int getBASE_ry();
    virtual int getBASE_rz();
    virtual float getBASE_sx();
    virtual float getBASE_sy();
    virtual float getBASE_sz();
    virtual nlist *getBASE_adeList();
    virtual base_1c_struct *getBASE_pTransform();
    virtual nlist *getBASE_kidList();
    virtual base_node *getBASE_kidNode();
    virtual area_arg_65 *getBASE_renderParams();
    virtual int getBASE_mainKid();
    virtual int getBASE_mainObjt();
    virtual polys *getBASE_renderStack();
    virtual void *getBASE_argStack();
    virtual void *getBASE_endArgStack();
    virtual int getBASE_fadeLength();
    virtual int getBASE_static();
    virtual int getBASE_embdRsrc();


    int base_func0__sub0(stack_vals *stak);
    void base_setter(stack_vals *stak);
    void base_getter(stack_vals *stak);


    //Data
    static const NewClassDescr description;

    __NC_STACK_base stack__base;
};

#endif // BASE_H_INCLUDED
