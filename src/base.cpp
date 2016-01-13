#include "includes.h"

#include "nucleas.h"

#include "utils.h"
#include "engine_input.h"

#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "base.h"
#include "engine_tform.h"
#include "skeleton.h"
#include "ade.h"

#include "engine_gfx.h"


stored_functions *classvtbl_get_base();
class_return * class_set_base(int, ...);

stored_functions base_class_vtbl(class_set_base);


class_stored base_class_off (NULL, NULL, "MC2classes:base.class", classvtbl_get_base);


stored_functions *classvtbl_get_base()
{
    return &base_class_vtbl;
}

CLASSFUNC base_funcs[1024];
int baseIDcounter;

char stru_55B200[0x41A00];
char stru_59CC00[0x41A0];
void *off_514F30 = &stru_55B200[0x41900];

base_1c_struct *dword_546DC0;


// rot_x == 0   rot_y == 0   rot_z == 0
void scale_rot_0(base_1c_struct *scrot)
{
    scrot->scale_rotation.m00 = scrot->scale_x;
    scrot->scale_rotation.m01 = 0;
    scrot->scale_rotation.m02 = 0;

    scrot->scale_rotation.m10 = 0;
    scrot->scale_rotation.m11 = scrot->scale_y;
    scrot->scale_rotation.m12 = 0;

    scrot->scale_rotation.m20 = 0;
    scrot->scale_rotation.m21 = 0;
    scrot->scale_rotation.m22 = scrot->scale_z;
}

// rot_x != 0   rot_y == 0   rot_z == 0
void scale_rot_1(base_1c_struct *scrot)
{
    SinCos *angl = &SinCos_table[scrot->rot_x >> 16];

    scrot->scale_rotation.m00 = scrot->scale_x;
    scrot->scale_rotation.m01 = 0;
    scrot->scale_rotation.m02 = 0;

    scrot->scale_rotation.m10 = 0;
    scrot->scale_rotation.m11 = scrot->scale_y * angl->cos;
    scrot->scale_rotation.m12 = scrot->scale_y * -angl->sin;

    scrot->scale_rotation.m20 = 0;
    scrot->scale_rotation.m21 = scrot->scale_z * angl->sin;
    scrot->scale_rotation.m22 = scrot->scale_z * angl->cos;

}

// rot_x == 0   rot_y != 0   rot_z == 0
void scale_rot_2(base_1c_struct *scrot)
{
    SinCos *angl = &SinCos_table[scrot->rot_y >> 16];

    scrot->scale_rotation.m00 = scrot->scale_x * angl->cos;
    scrot->scale_rotation.m01 = 0;
    scrot->scale_rotation.m02 = scrot->scale_x * angl->sin;

    scrot->scale_rotation.m10 = 0;
    scrot->scale_rotation.m11 = scrot->scale_y;
    scrot->scale_rotation.m12 = 0;

    scrot->scale_rotation.m20 = -angl->sin * scrot->scale_z;
    scrot->scale_rotation.m21 = 0;
    scrot->scale_rotation.m22 = scrot->scale_z * angl->cos;
}

// rot_x != 0   rot_y != 0   rot_z == 0
void scale_rot_3(base_1c_struct *scrot)
{
    SinCos *angl_x = &SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_y = &SinCos_table[scrot->rot_y >> 16];

    scrot->scale_rotation.m00 = scrot->scale_x * angl_y->cos;
    scrot->scale_rotation.m01 = 0;
    scrot->scale_rotation.m02 = scrot->scale_x * angl_y->sin;

    scrot->scale_rotation.m10 = angl_x->sin * angl_y->sin * scrot->scale_y;
    scrot->scale_rotation.m11 = scrot->scale_y * angl_x->cos;
    scrot->scale_rotation.m12 = -angl_x->sin * angl_y->cos * scrot->scale_y;

    scrot->scale_rotation.m20 = -angl_x->cos * angl_y->sin * scrot->scale_z;
    scrot->scale_rotation.m21 = scrot->scale_z * angl_x->sin;
    scrot->scale_rotation.m22 = angl_x->cos * angl_y->cos * scrot->scale_z;
}

// rot_x == 0   rot_y == 0   rot_z != 0
void scale_rot_4(base_1c_struct *scrot)
{
    SinCos *angl = &SinCos_table[scrot->rot_z >> 16];

    scrot->scale_rotation.m00 = scrot->scale_x * angl->cos;
    scrot->scale_rotation.m01 = -angl->sin * scrot->scale_x;
    scrot->scale_rotation.m02 = 0;

    scrot->scale_rotation.m10 = scrot->scale_y * angl->sin;
    scrot->scale_rotation.m11 = scrot->scale_y * angl->cos;
    scrot->scale_rotation.m12 = 0;

    scrot->scale_rotation.m20 = 0;
    scrot->scale_rotation.m21 = 0;
    scrot->scale_rotation.m22 = scrot->scale_z;
}

// rot_x != 0   rot_y == 0   rot_z != 0
void scale_rot_5(base_1c_struct *scrot)
{
    SinCos *angl_x = &SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_z = &SinCos_table[scrot->rot_z >> 16];

    scrot->scale_rotation.m00 = scrot->scale_x * angl_z->cos;
    scrot->scale_rotation.m01 = -angl_z->sin * angl_x->cos * scrot->scale_x;
    scrot->scale_rotation.m02 = angl_z->sin * angl_x->sin * scrot->scale_x;

    scrot->scale_rotation.m10 = scrot->scale_y * angl_z->sin;
    scrot->scale_rotation.m11 = angl_z->cos * angl_x->cos * scrot->scale_y;
    scrot->scale_rotation.m12 = -angl_z->cos * angl_x->sin * scrot->scale_y;

    scrot->scale_rotation.m20 = 0;
    scrot->scale_rotation.m21 = scrot->scale_z * angl_x->sin;
    scrot->scale_rotation.m22 = scrot->scale_z * angl_x->cos;
}

// rot_x == 0   rot_y != 0   rot_z != 0
void scale_rot_6(base_1c_struct *scrot)
{
    SinCos *angl_y = &SinCos_table[scrot->rot_y >> 16];
    SinCos *angl_z = &SinCos_table[scrot->rot_z >> 16];

    scrot->scale_rotation.m00 = angl_z->cos * angl_y->cos * scrot->scale_x;
    scrot->scale_rotation.m01 = -angl_z->sin * scrot->scale_x;
    scrot->scale_rotation.m02 = angl_z->cos * angl_y->sin * scrot->scale_x;

    scrot->scale_rotation.m10 = angl_z->sin * angl_y->cos * scrot->scale_y;
    scrot->scale_rotation.m11 = scrot->scale_y * angl_z->cos;
    scrot->scale_rotation.m12 = angl_z->sin * angl_y->sin * scrot->scale_y;

    scrot->scale_rotation.m20 = -angl_y->sin * scrot->scale_z;
    scrot->scale_rotation.m21 = 0;
    scrot->scale_rotation.m22 = scrot->scale_z * angl_y->cos;

}

// rot_x != 0   rot_y != 0   rot_z != 0
void scale_rot_7(base_1c_struct *scrot)
{
    SinCos *angl_x = &SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_y = &SinCos_table[scrot->rot_y >> 16];
    SinCos *angl_z = &SinCos_table[scrot->rot_z >> 16];


    scrot->scale_rotation.m00 = (angl_z->cos * angl_y->cos - angl_z->sin * angl_x->sin * angl_y->sin) * scrot->scale_x;
    scrot->scale_rotation.m01 = -angl_z->sin * angl_x->cos * scrot->scale_x;
    scrot->scale_rotation.m02 = (angl_z->cos * angl_y->sin + angl_z->sin * angl_x->sin * angl_y->cos) * scrot->scale_x;

    scrot->scale_rotation.m10 = (angl_z->sin * angl_y->cos + angl_z->cos * angl_x->sin * angl_y->sin) * scrot->scale_y;
    scrot->scale_rotation.m11 = angl_z->cos * angl_x->cos * scrot->scale_y;
    scrot->scale_rotation.m12 = (angl_z->sin * angl_y->sin - angl_z->cos * angl_x->sin * angl_y->cos) * scrot->scale_y;

    scrot->scale_rotation.m20 = -angl_x->cos * angl_y->sin * scrot->scale_z;
    scrot->scale_rotation.m21 = scrot->scale_z * angl_x->sin;
    scrot->scale_rotation.m22 = angl_x->cos * angl_y->cos * scrot->scale_z;
}


void make_scale_rotation__matrix(base_1c_struct *scrot)
{
    typedef void (*matrfunc)(base_1c_struct *);
    matrfunc off_5151E0[8] = {scale_rot_0, scale_rot_1, scale_rot_2, scale_rot_3, scale_rot_4, scale_rot_5, scale_rot_6, scale_rot_7};

    BYTE func_id = 0;

    if ( scrot->rot_x )
        func_id |= 1;

    if ( scrot->rot_y )
        func_id |= 2;

    if ( scrot->rot_z )
        func_id |= 4;

    off_5151E0[func_id](scrot);
}

int base_func0__sub0(__NC_STACK_base *base, stack_vals *stak)
{
    base->field_D8 = 4096;

    base->field_DC.field_2C = 3496.0;
    base->field_DC.field_30 = 600.0;
    base->field_DC.field_34 = 255;

    base->field_4 |= 8;
    base->params3D.field_94 |= 1;


    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80001000:
                if (stk->value)
                {
                    if ( base->OBJ_SKELETON )
                        delete_class_obj(base->OBJ_SKELETON);

                    base->OBJ_SKELETON = (NC_STACK_skeleton *)stk->value;
                    call_vtbl(base->OBJ_SKELETON, 3, 0x80002000, &base->field_DC.field_20, 0);
                }
                break;

            case 0x80001001:
                if (stk->value)
                    call_vtbl((NC_STACK_class *)stk->value, 64, &base->ADES);

                break;

            case 0x80001002:
                if (stk->value)
                    base->params3D.field_94 |= 1;
                else
                    base->params3D.field_94 &= 0xFFFFFFFE;

                break;

            case 0x80001004:
                base->field_D8 = stk->value;
                base->field_DC.field_2C = stk->value - base->field_DC.field_30;

                break;

            case 0x80001005:
                base->field_DC.field_34 = stk->value;

                break;

            case 0x80001006:
                if (stk->value)
                    base->field_4 |= 8;
                else
                    base->field_4 &= 0xFFFFFFF7;

                break;

            case 0x80001008:
                if (stk->value)
                    base->field_4 |= 0x20;
                else
                    base->field_4 &= 0xFFFFFFDF;

                break;

            case 0x80001023:
                base->field_DC.field_30 = stk->value;
                base->field_DC.field_2C = base->field_D8 - stk->value;

                break;

            case 0x80001024:
                if (stk->value)
                    base->params3D.field_94 |= 2;
                else
                    base->params3D.field_94 &= 0xFFFFFFFD;

                break;

            case 0x80001025:
                if (stk->value)
                    base->field_4 |= 0x80;
                else
                    base->field_4 &= 0xFFFFFF7F;

                break;
            }
            stk++;
        }
    }

    return 1;
}

NC_STACK_base * base_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_base *obj = (NC_STACK_base *)call_parent(zis, clss, 0, stak);

    if ( !obj )
        return NULL;


    __NC_STACK_base *base = &obj->stack__base;

    base->ID = baseIDcounter;
    baseIDcounter++;

    init_list(&base->ADES);
    init_list(&base->KIDS);

    base->params3D.scale_x = 1.0;
    base->params3D.scale_y = 1.0;
    base->params3D.scale_z = 1.0;

    base->kid_node.self_full = obj;

    make_scale_rotation__matrix(&base->params3D);

    if ( !base_func0__sub0(base, stak) )
    {
        call_method(obj, 1);
        return NULL;
    }

    return obj;
}

size_t base_func1(NC_STACK_base *obj, class_stru *zis, stack_vals *stak)
{
    __NC_STACK_base *base = &obj->stack__base;

    if (base->OBJ_SKELETON)
        delete_class_obj(base->OBJ_SKELETON);

    while ( 1 )
    {
        clss_node *tmp = (clss_node *)base->ADES.head;

        if ( !tmp->next )
            break;

        delete_class_obj(tmp->obj); // this objects delete self from kids parent list
    }

    if ( base->parent_base )
        Remove(&base->kid_node);

    while ( 1 )
    {
        clss_node *tmp = (clss_node *)base->KIDS.head;

        if ( !tmp->next )
            break;

        delete_class_obj(tmp->obj); // this objects delete self from kids parent list
    }

    if ( base->OBJT )
        delete_class_obj(base->OBJT);

    return call_parent(zis, obj, 1, stak);
}

void base_setter(__NC_STACK_base *base, stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80001000:
                if (stk->value)
                {
                    if ( base->OBJ_SKELETON )
                        delete_class_obj(base->OBJ_SKELETON);

                    base->OBJ_SKELETON = (NC_STACK_skeleton *)stk->value;
                    base->field_DC.field_20 = NULL;
                    call_vtbl(base->OBJ_SKELETON, 3, 0x80002000, &base->field_DC.field_20, 0);
                }
                break;

            case 0x80001001:
                if (stk->value)
                {
                    //call_vtbl((NC_STACK_class *)stk->value, 64, &base->ADES);
                    nlist *tmp = &base->ADES;
                    call_method((NC_STACK_class *)stk->value, 64, &tmp);
                }
                break;

            case 0x80001002:
                if (stk->value)
                    base->params3D.field_94 |= 1;
                else
                    base->params3D.field_94 &= 0xFFFFFFFE;

                break;

            case 0x80001004:
                base->field_D8 = stk->value;
                base->field_DC.field_2C = stk->value - base->field_DC.field_30;

                break;

            case 0x80001005:
                base->field_DC.field_34 = stk->value;

                break;

            case 0x80001006:
                if (stk->value)
                    base->field_4 |= 8;
                else
                    base->field_4 &= 0xFFFFFFF7;

                break;

            case 0x80001008:
                if (stk->value)
                    base->field_4 |= 0x20;
                else
                    base->field_4 &= 0xFFFFFFDF;

                break;

            case 0x80001023:
                base->field_DC.field_30 = stk->value;
                base->field_DC.field_2C = base->field_D8 - stk->value;

                break;

            case 0x80001024:
                if (stk->value)
                    base->params3D.field_94 |= 2;
                else
                    base->params3D.field_94 &= 0xFFFFFFFD;

                break;

            case 0x80001025:
                if (stk->value)
                    base->field_4 |= 0x80;
                else
                    base->field_4 &= 0xFFFFFF7F;

                break;
            }
            stk++;
        }
    }
}

size_t base_func2(NC_STACK_base *obj, class_stru *zis, stack_vals *stak)
{
    __NC_STACK_base *base = &obj->stack__base;

    base_setter(base, stak);

    return call_parent(zis, obj, 2, stak);
}

void base_getter(NC_STACK_base *, __NC_STACK_base *base, stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;
            case 0x80001000:
                *(NC_STACK_skeleton **)stk->value = base->OBJ_SKELETON;
                break;
            case 0x80001002:
                if ( !(base->params3D.field_94 & 1) )
                    *(int *)stk->value = 0;
                else
                    *(int *)stk->value = 1;
                break;
            case 0x80001003:
            case 0x80001007:
                *(int *)stk->value = 0;
                break;
            case 0x80001004:
                *(int *)stk->value = base->field_D8;
                break;
            case 0x80001005:
                *(int *)stk->value = base->field_DC.field_34;
                break;
            case 0x80001006:
                if ( !(base->field_4 & 8) )
                    *(int *)stk->value = 0;
                else
                    *(int *)stk->value = 1;
                break;
            case 0x80001008:
                if ( !(base->field_4 & 0x20) )
                    *(int *)stk->value = 0;
                else
                    *(int *)stk->value = 1;
                break;
            case 0x80001009:
                *(float *)stk->value = base->params3D.grp1_p1;
                break;
            case 0x8000100A:
                *(float *)stk->value = base->params3D.grp1_p2;
                break;
            case 0x8000100B:
                *(float *)stk->value = base->params3D.grp1_p3;
                break;
            case 0x8000100C:
                *(float *)stk->value = base->params3D.grp3_p1;
                break;
            case 0x8000100D:
                *(float *)stk->value = base->params3D.grp3_p2;
                break;
            case 0x8000100E:
                *(float *)stk->value = base->params3D.grp3_p3;
                break;
            case 0x8000100F:
                *(int *)stk->value = base->params3D.rot_x >> 16;
                break;
            case 0x80001010:
                *(int *)stk->value = base->params3D.rot_y >> 16;
                break;
            case 0x80001011:
                *(int *)stk->value = base->params3D.rot_z >> 16;
                break;
            case 0x80001012:
                *(int *)stk->value = base->params3D.grp5_p1 >> 6;
                break;
            case 0x80001013:
                *(int *)stk->value = base->params3D.grp5_p2 >> 6;
                break;
            case 0x80001014:
                *(int *)stk->value = base->params3D.grp5_p3 >> 6;
                break;
            case 0x80001015:
                *(float *)stk->value = base->params3D.scale_x;
                break;
            case 0x80001016:
                *(float *)stk->value = base->params3D.scale_y;
                break;
            case 0x80001017:
                *(float *)stk->value = base->params3D.scale_z;
                break;
            case 0x80001018:
                *(nlist **)stk->value = &base->ADES;
                break;
            case 0x80001019:
                *(base_1c_struct **)stk->value = &base->params3D;
                break;
            case 0x8000101A:
                *(nlist **)stk->value = &base->KIDS;
                break;
            case 0x8000101B:
                *(base_node **)stk->value = &base->kid_node;
                break;
            case 0x8000101D:
                *(area_arg_65 **)stk->value = &base->field_DC;
                break;
            case 0x8000101E:
                *(int *)stk->value = (base->field_4 & 4) != 0;
                break;
            case 0x8000101F:
                *(int *)stk->value = (base->field_4 & 0x40) != 0;
                break;
            case 0x80001020:
                *(void **)stk->value = stru_59CC00;
                break;
            case 0x80001021:
                *(void **)stk->value = stru_55B200;
                break;
            case 0x80001022:
                *(void **)stk->value = off_514F30;
                break;
            case 0x80001023:
                *(int *)stk->value = base->field_DC.field_30;
                break;
            case 0x80001024:
                if ( !(base->params3D.field_94 & 2) )
                    *(int *)stk->value = 0;
                else
                    *(int *)stk->value = 1;
                break;
            case 0x80001025:
                if ( base->field_4 & 0x80 )
                    *(int *)stk->value = 1;
                else
                    *(int *)stk->value = 0;
                break;
            }
            stk++;
        }
    }
}

size_t base_func3(NC_STACK_base *obj, class_stru *zis, stack_vals *stak)
{

    __NC_STACK_base *base = &obj->stack__base;

    base_getter(obj, base, stak);

    return call_parent(zis, obj, 3, stak);
}

int base_READ_STRC(NC_STACK_base *obj, __NC_STACK_base *, MFILE *mfile)
{
    if ( obj )
    {
        stack_vals stk[10];
        stack_vals *v4 = stk;
        STRC_base dst;

        if ( mfread(mfile, &dst, sizeof(STRC_base)) )
        {
            dst.p1 = SWAP16(dst.p1);

            dst.grp1_p1 = SWAP32F(dst.grp1_p1);
            dst.grp1_p2 = SWAP32F(dst.grp1_p2);
            dst.grp1_p3 = SWAP32F(dst.grp1_p3);

            dst.grp3_p1 = SWAP32F(dst.grp3_p1);
            dst.grp3_p2 = SWAP32F(dst.grp3_p2);
            dst.grp3_p3 = SWAP32F(dst.grp3_p3);

            dst.scale_x = SWAP32F(dst.scale_x);
            dst.scale_y = SWAP32F(dst.scale_y);
            dst.scale_z = SWAP32F(dst.scale_z);

            dst.rot_x = SWAP16(dst.rot_x);
            dst.rot_y = SWAP16(dst.rot_y);
            dst.rot_z = SWAP16(dst.rot_z);

            dst.grp5_p1 = SWAP16(dst.grp5_p1);
            dst.grp5_p2 = SWAP16(dst.grp5_p2);
            dst.grp5_p3 = SWAP16(dst.grp5_p3);

            dst.p17 = SWAP16(dst.p17);
            dst.p18 = SWAP16(dst.p18);

            dst.p19 = SWAP32(dst.p19);
            dst.p20 = SWAP32(dst.p20);


            if ( dst.p1 >= 1 )
            {
                stk[0].id = 0x80001006;
                stk[0].value = (dst.p17 & 8) != 0;
                stk[1].id = 0x80001007;
                stk[1].value = (dst.p17 & 0x10) != 0;
                stk[2].id = 0x80001008;
                stk[2].value = (dst.p17 & 0x20) != 0;
                stk[3].id = 0x80001002;
                stk[3].value = (dst.p17 & 0x40) != 0;
                stk[4].id = 0x80001003;
                stk[4].value = dst.p18;
                stk[5].id = 0x80001004;
                stk[5].value = dst.p19;
                stk[6].id = 0x80001005;
                stk[6].value = dst.p20;

                v4 = &stk[7];

                NC_STACK_base *zzz[2] = {NULL, NULL};

                if ( dst.p17 & 4 )
                    call_method(obj, 67, zzz);

                flag_xyz v38;

                v38.flag = 7;
                v38.x = dst.grp1_p1;
                v38.y = dst.grp1_p2;
                v38.z = dst.grp1_p3;

                call_method(obj, 68, &v38);


                v38.x = dst.scale_x;
                v38.y = dst.scale_y;
                v38.z = dst.scale_z;

                call_method(obj, 72, &v38); // SET_SCALE ?

                if ( dst.p17 & 1 )
                {
                    v38.x = dst.grp3_p1;
                    v38.y = dst.grp3_p2;
                    v38.z = dst.grp3_p3;
                }
                else
                {
                    v38.flag = 8;
                }

                call_method(obj, 69, &v38);

                flag_xyz2 v39;

                v39.flag = 7;

                v39.x = dst.rot_x;
                v39.y = dst.rot_y;
                v39.z = dst.rot_z;

                call_method(obj, 70, &v39);

                if ( dst.p17 & 2 )
                {
                    v39.x = dst.grp5_p1;
                    v39.y = dst.grp5_p2;
                    v39.z = dst.grp5_p3;
                }
                else
                {
                    v39.flag = 8;
                }

                call_method(obj, 71, &v39);
            }
            v4->id = 0;

            call_method(obj, 2, stk);
            return 1;
        }
        else
            return 0;
    }
    return 0;
}

int base_READ_ADES(NC_STACK_base *obj, __NC_STACK_base *, MFILE *mfile)
{
    if (!obj)
        return 0;

    while ( 1 )
    {
        int v5 = read_next_IFF(mfile, 2);
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            NC_STACK_class *objt = READ_OBJT(mfile);
            if ( !objt )
                return 0;

            stack_vals v7[2];

            v7[0].id = 0x80001001;
            v7[0].value = (size_t)objt;
            v7[1].id = 0;
            v7[1].value = 0;

            call_method(obj, 2, v7);
        }
        else
        {
            read_default(mfile);
        }
    }
    return 1;
}

int base_READ_KIDS(NC_STACK_base *obj, __NC_STACK_base *, MFILE *mfile)
{
    if (!obj)
        return 0;

    int kidid = 0;

    while ( 1 )
    {
        int v5 = read_next_IFF(mfile, 2);
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            NC_STACK_class *objt = READ_OBJT(mfile);
            if ( !objt )
            {
                return 0;
            }
            kidid++;

            call_method(obj, 65, &objt);
        }
        else
        {
            read_default(mfile);
        }
    }
    return 1;
}


NC_STACK_base *base_func5(class_stru *clss, class_stru *zis, MFILE **file)
{
    MFILE *mfile = *file;
    NC_STACK_base *obj = NULL;
    __NC_STACK_base *base = NULL;
    int STRC_readed = 0;

    while ( 1 )
    {
        int iff_res = read_next_IFF(mfile, 2);

        if ( iff_res == -2 )
            break;

        if ( iff_res )
        {
            if ( obj )
                call_method(obj, 1);
            return NULL;
        }

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);



        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj = (NC_STACK_base *)call_parent(zis, clss, 5, (stack_vals *)file);

            if ( !obj )
                return NULL;

            base = &obj->stack__base;

            init_list(&base->ADES);
            init_list(&base->KIDS);

            base->params3D.scale_x = 1.0;
            base->params3D.scale_y = 1.0;
            base->params3D.scale_z = 1.0;

            base->ID = baseIDcounter;

            baseIDcounter++;

            base->kid_node.self_full = obj;

            make_scale_rotation__matrix(&base->params3D);

            base->field_D8 = 4096;
            base->field_DC.field_2C = 3496.0;
            base->field_DC.field_30 = 600.0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            STRC_readed = 1;

            if ( !base_READ_STRC(obj, base, mfile) )
            {
                call_method(obj, 1);
                return NULL;
            }
            read_next_IFF(mfile, 2);
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( obj )
            {
                if ( STRC_readed )
                {
                    NC_STACK_class *objt = READ_OBJT(mfile);
                    if ( !objt )
                    {
                        call_method(obj, 1);
                        return NULL;
                    }
                    call_vtbl(obj, 2, 0x80001000, objt, 0);
                }
                else
                {
                    base->OBJT = (NC_STACK_nucleus *)READ_OBJT(mfile);
                }
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADES )
        {
            if ( !base_READ_ADES(obj, base, mfile) )
            {
                call_method(obj, 1);
                return NULL;
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_KIDS )
        {
            if ( !base_READ_KIDS(obj, base, mfile) )
            {
                call_method(obj, 1);
                return NULL;
            }
        }
        else
        {
            read_default(mfile);
        }
    }
    return obj;
}

size_t base_func6(NC_STACK_base *obj, class_stru *zis, MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_base *base = &obj->stack__base;

    if ( sub_412FC0(*file, TAG_BASE, TAG_FORM, -1) )
        return 0;

    if ( !call_parent(zis, obj, 6, (stack_vals *)file) )
        return 0;

    if ( base->field_4 & 0x80 )
    {
        NC_STACK_class *embd = init_get_class("embed.class", 0);
        if ( embd )
        {
            if ( !sub_4117F8(embd, mfile) )
            {
                delete_class_obj(embd);
                return 0;
            }
            delete_class_obj(embd);
        }
    }

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    STRC_base a1;

    a1.p17 = 0;
    a1.p1 = SWAP16(1);

    if ( base->field_4 & 1 )
        a1.p17 = 1;

    if ( base->field_4 & 2 )
        a1.p17 |= 2;

    if ( base->field_4 & 0x40 )
        a1.p17 |= 4;

    if ( base->field_4 & 8 )
        a1.p17 |= 8;

    if ( base->field_4 & 0x10 )
        a1.p17 |= 0x10;

    if ( base->field_4 & 0x20 )
        a1.p17 |= 0x20;

    if ( base->params3D.field_94 & 1 )
        a1.p17 |= 0x40;

    a1.p17 = SWAP16(a1.p17);

    a1.grp1_p1 = SWAP32F(base->params3D.grp1_p1);
    a1.grp1_p2 = SWAP32F(base->params3D.grp1_p2);
    a1.grp1_p3 = SWAP32F(base->params3D.grp1_p3);

    a1.grp3_p1 = SWAP32F(base->params3D.grp3_p1);
    a1.grp3_p2 = SWAP32F(base->params3D.grp3_p2);
    a1.grp3_p3 = SWAP32F(base->params3D.grp3_p3);

    a1.scale_x = SWAP32F(base->params3D.scale_x);
    a1.scale_y = SWAP32F(base->params3D.scale_y);
    a1.scale_z = SWAP32F(base->params3D.scale_z);

    a1.rot_x = SWAP16(base->params3D.rot_x >> 16);
    a1.rot_y = SWAP16(base->params3D.rot_y >> 16);
    a1.rot_z = SWAP16(base->params3D.rot_z >> 16);

    a1.grp5_p1 = SWAP16(base->params3D.grp5_p1 >> 6);
    a1.grp5_p2 = SWAP16(base->params3D.grp5_p2 >> 6);
    a1.grp5_p3 = SWAP16(base->params3D.grp5_p3 >> 6);

    a1.p18 = 0;

    a1.p19 = base->field_DC.field_2C + base->field_DC.field_30;
    a1.p19 = SWAP32(a1.p19);

    a1.p20 = SWAP32(base->field_DC.field_34);

    sub_413564(mfile, sizeof(STRC_base), &a1);
    sub_413290(mfile);

    if ( base->OBJ_SKELETON )
    {
        if ( !sub_4117F8(base->OBJ_SKELETON, mfile) )
            return 0;

        if ( base->ADES.head->next )
        {
            sub_412FC0(mfile, TAG_ADES, TAG_FORM, -1);
            for ( __NC_STACK_ade *cur_ade = (__NC_STACK_ade *)base->ADES.head; cur_ade->next; cur_ade = (__NC_STACK_ade *)cur_ade->next )
            {
                if ( !sub_4117F8(cur_ade->self, mfile) )
                    return 0;
            }
            sub_413290(mfile);
        }
    }

    if ( base->KIDS.head->next )
    {
        sub_412FC0(mfile, TAG_KIDS, TAG_FORM, -1);
        for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
        {
            if ( !sub_4117F8(kid->self_full, mfile) )
                return 0;
        }
        sub_413290(mfile);
    }

    return sub_413290(mfile) == 0;
}

void sub_430A38(base_1c_struct *s3d)
{
    base_1c_struct *prnt_s3d = s3d->parent_1c;

    if ( s3d->parent_1c && s3d->field_94 & 1 )
    {
        s3d->field_10 = prnt_s3d->field_58.m00 * s3d->grp1_p1 + prnt_s3d->field_58.m01 * s3d->grp1_p2 + prnt_s3d->field_58.m02 * s3d->grp1_p3 + prnt_s3d->field_10;
        s3d->field_14 = prnt_s3d->field_58.m10 * s3d->grp1_p1 + prnt_s3d->field_58.m11 * s3d->grp1_p2 + prnt_s3d->field_58.m12 * s3d->grp1_p3 + prnt_s3d->field_14;
        s3d->field_18 = prnt_s3d->field_58.m20 * s3d->grp1_p1 + prnt_s3d->field_58.m21 * s3d->grp1_p2 + prnt_s3d->field_58.m22 * s3d->grp1_p3 + prnt_s3d->field_18;
        s3d->field_58.m00 = prnt_s3d->field_58.m00 * s3d->scale_rotation.m00 + prnt_s3d->field_58.m01 * s3d->scale_rotation.m10 + prnt_s3d->field_58.m02 * s3d->scale_rotation.m20;
        s3d->field_58.m01 = prnt_s3d->field_58.m00 * s3d->scale_rotation.m01 + prnt_s3d->field_58.m01 * s3d->scale_rotation.m11 + prnt_s3d->field_58.m02 * s3d->scale_rotation.m21;
        s3d->field_58.m02 = prnt_s3d->field_58.m00 * s3d->scale_rotation.m02 + prnt_s3d->field_58.m01 * s3d->scale_rotation.m12 + prnt_s3d->field_58.m02 * s3d->scale_rotation.m22;
        s3d->field_58.m10 = prnt_s3d->field_58.m10 * s3d->scale_rotation.m00 + prnt_s3d->field_58.m11 * s3d->scale_rotation.m10 + prnt_s3d->field_58.m12 * s3d->scale_rotation.m20;
        s3d->field_58.m11 = prnt_s3d->field_58.m10 * s3d->scale_rotation.m01 + prnt_s3d->field_58.m11 * s3d->scale_rotation.m11 + prnt_s3d->field_58.m12 * s3d->scale_rotation.m21;
        s3d->field_58.m12 = prnt_s3d->field_58.m10 * s3d->scale_rotation.m02 + prnt_s3d->field_58.m11 * s3d->scale_rotation.m12 + prnt_s3d->field_58.m12 * s3d->scale_rotation.m22;
        s3d->field_58.m20 = prnt_s3d->field_58.m20 * s3d->scale_rotation.m00 + prnt_s3d->field_58.m21 * s3d->scale_rotation.m10 + prnt_s3d->field_58.m22 * s3d->scale_rotation.m20;
        s3d->field_58.m21 = prnt_s3d->field_58.m20 * s3d->scale_rotation.m01 + prnt_s3d->field_58.m21 * s3d->scale_rotation.m11 + prnt_s3d->field_58.m22 * s3d->scale_rotation.m21;
        s3d->field_58.m22 = prnt_s3d->field_58.m20 * s3d->scale_rotation.m02 + prnt_s3d->field_58.m21 * s3d->scale_rotation.m12 + prnt_s3d->field_58.m22 * s3d->scale_rotation.m22;
    }
    else
    {
        s3d->field_10 = s3d->grp1_p1;
        s3d->field_14 = s3d->grp1_p2;
        s3d->field_18 = s3d->grp1_p3;

        memcpy(&s3d->field_58, &s3d->scale_rotation, sizeof(mat3x3));
    }
}


int sub_430880(const void * a, const void * b)
{
    polys *a1 = (polys *)a;
    polys *a2 = (polys *)b;

    return a1->range - a2->range;
}


size_t base_func64(NC_STACK_base *obj, class_stru *, base_64arg *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    base_1c_struct *glob_1c = sub_430A28();

    if ( glob_1c )
        sub_430A38(glob_1c);

    call_method(obj, 73, arg);

    base77Func base77;
    base77.field_0 = arg->field_4;
    base77.field_4 = arg->field_0;
    base77.curOutPoly = (polys *)stru_59CC00;
    base77.polysData = (polysDat *)stru_55B200;
    base77.polysData_end = (polysDat *)off_514F30;
    base77.field_14 = arg->field_C;
    base77.field_18 = 1000;
    base77.field_1C = base->ID;

    call_method(obj, 77, &base77);

    arg->field_C = base77.field_14;

    int num = base77.curOutPoly - (polys *)stru_59CC00;

    arg->field_10 += num;

    if ( num > 1 )
        qsort(stru_59CC00, num, sizeof(polys), sub_430880);

    NC_STACK_win3d *win3d;
    gfxEngine__getter(0x8000300D, &win3d, 0);


    call_method(win3d, 257, 0);
    call_vtbl(win3d, 2, 0x80003001, 0, 0);
    call_method(win3d, 192, 0);
    call_method(win3d, 213, 0);

    for (int i = 0; i < num; i++)
    {
        polys *pol = ((polys *)stru_59CC00) + i;
        pol->data->render_func(pol->data);
    }

    call_method(win3d, 214, 0);
    call_method(win3d, 258, 0);

    return 1;
}

// Fill parent info struct and push it to kid
size_t base_func65(NC_STACK_base *obj, class_stru *, NC_STACK_base **kid)
{
    __NC_STACK_base *base = &obj->stack__base;

    base_66_arg_struct v6; // Parent info

    v6.parent = obj;
    v6.KIDS = &base->KIDS;

    if ( base->OBJ_SKELETON )
        v6.parent_field_1c = &base->params3D;
    else
        v6.parent_field_1c = NULL;

    call_method(*kid , 66, &v6);

    return 1;
}

// Add object to parent kids list
size_t base_func66(NC_STACK_base *obj, class_stru *, base_66_arg_struct *prnt_info)
{
    __NC_STACK_base *base = &obj->stack__base;

    if ( base->parent_base )
    {
        Remove(&base->kid_node);

        if ( base->field_4 & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = (NC_STACK_base *)-1;
            v7[1] = NULL;
            call_method(base->parent_base, 67, &v7);
        }
    }

    base->parent_base = prnt_info->parent;
    if ( prnt_info->parent )
    {
        AddTail(prnt_info->KIDS, &base->kid_node);
        if ( base->field_4 & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = obj;
            v7[1] = obj;

            call_method(base->parent_base, 67, &v7);
        }
    }
    base->params3D.parent_1c = prnt_info->parent_field_1c;

    return 1;
}

void sub_430A20(base_1c_struct *s3d)
{
    dword_546DC0 = s3d;
}

base_1c_struct *sub_430A28()
{
    return dword_546DC0;
}

size_t base_func67(NC_STACK_base *obj, class_stru *, NC_STACK_base **arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    if (arg[0] == NULL)
    {
        base->field_b8 = NULL;
        base->field_4 |= 0x44;
        base->field_bc = obj;

        if ( base->parent_base )
        {
            arg[0] = obj;
            arg[1] = obj;

            call_method(base->parent_base, 67, arg);
        }
        sub_430A20(&base->params3D);
    }
    else if ((int)arg[0] == -1)
    {
        base->field_bc = 0;
        base->field_b8 = NULL;
        base->field_4 &= 0xBB;

        if ( !base->parent_base )
            return 1;

        call_method(base->parent_base, 67, arg);
    }
    else if (arg[0])
    {
        base->field_b8 = arg[0];
        base->field_bc = arg[1];

        base->field_4 |= 4;
        base->field_4 &= 0xBF;

        if ( !base->parent_base )
            return 1;

        arg[0] = obj;

        call_method(base->parent_base, 67, arg);
    }

    return 1;
}

size_t base_func68(NC_STACK_base *obj, class_stru *, flag_xyz *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->grp1_p1 += arg->x;
        if ( flg & 2 )
            s3d->grp1_p2 += arg->y;
        if ( flg & 4 )
            s3d->grp1_p3 += arg->z;
    }
    else
    {
        if ( flg & 1 )
            s3d->grp1_p1 = arg->x;
        if ( flg & 2 )
            s3d->grp1_p2 = arg->y;
        if ( flg & 4 )
            s3d->grp1_p3 = arg->z;
    }
    return 1;
}

size_t base_func69(NC_STACK_base *obj, class_stru *, flag_xyz *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 8 )
    {
        base->field_4 &= 0xFE;
    }
    else
    {
        base->field_4 |= 1;

        if ( flg & 0x10 )
        {
            if ( flg & 1 )
                s3d->grp3_p1 += arg->x;
            if ( flg & 2 )
                s3d->grp3_p2 += arg->y;
            if ( flg & 4 )
                s3d->grp3_p3 += arg->z;
        }
        else
        {
            if ( flg & 1 )
                s3d->grp3_p1 = arg->x;
            if ( flg & 2 )
                s3d->grp3_p2 = arg->y;
            if ( flg & 4 )
                s3d->grp3_p3 = arg->z;
        }
    }

    return 1;
}

size_t base_func70(NC_STACK_base *obj, class_stru *, flag_xyz2 *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->rot_x = (s3d->rot_x + (arg->x << 16)) % (360 << 16);
        if ( flg & 2 )
            s3d->rot_y = (s3d->rot_y + (arg->y << 16)) % (360 << 16);
        if ( flg & 4 )
            s3d->rot_z = (s3d->rot_z + (arg->z << 16)) % (360 << 16);
    }
    else
    {
        if ( flg & 1 )
            s3d->rot_x = arg->x << 16;
        if ( flg & 2 )
            s3d->rot_y = arg->y << 16;
        if ( flg & 4 )
            s3d->rot_z = arg->z << 16;
    }

    make_scale_rotation__matrix(s3d);

    return 1;
}

size_t base_func71(NC_STACK_base *obj, class_stru *, flag_xyz2 *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 8 )
    {
        base->field_4 &= 0xFD;
    }
    else
    {
        base->field_4 |= 1;

        if ( flg & 0x10 )
        {
            if ( flg & 1 )
                s3d->grp5_p1 = (s3d->grp5_p1 + (arg->x << 6)) % (360 << 16);
            if ( flg & 2 )
                s3d->grp5_p2 = (s3d->grp5_p2 + (arg->y << 6)) % (360 << 16);
            if ( flg & 4 )
                s3d->grp5_p3 = (s3d->grp5_p3 + (arg->z << 6)) % (360 << 16);
        }
        else
        {
            if ( flg & 1 )
                s3d->grp5_p1 = arg->x << 6;
            if ( flg & 2 )
                s3d->grp5_p2 = arg->y << 6;
            if ( flg & 4 )
                s3d->grp5_p3 = arg->z << 6;
        }
    }

    return 1;
}

size_t base_func72(NC_STACK_base *obj, class_stru *, flag_xyz *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->scale_x *= arg->x;
        if ( flg & 2 )
            s3d->scale_y *= arg->y;
        if ( flg & 4 )
            s3d->scale_z *= arg->z;
    }
    else
    {
        if ( flg & 1 )
            s3d->scale_x = arg->x;
        if ( flg & 2 )
            s3d->scale_y = arg->y;
        if ( flg & 4 )
            s3d->scale_z = arg->z;
    }

    make_scale_rotation__matrix(s3d);

    return 1;
}

size_t base_func73(NC_STACK_base *obj, class_stru *, base73arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func73");
    __NC_STACK_base *base = &obj->stack__base;

    if ( arg->field_0 != base->field_8 )
    {
        base->field_8 = arg->field_0;

        if ( base->OBJ_SKELETON )
        {
            if ( base->field_4 & 0x20 )
                call_method(obj, 78, arg);

            if ( base->field_4 & 1 )
            {
                base->params3D.grp1_p1 += base->params3D.grp3_p1 * arg->field_4;
                base->params3D.grp1_p2 += base->params3D.grp3_p2 * arg->field_4;
                base->params3D.grp1_p3 += base->params3D.grp3_p3 * arg->field_4;
            }

            if ( base->field_4 & 2 )
            {
                int a,b,c;
                a = (base->params3D.grp5_p1 * arg->field_4 + base->params3D.rot_x) % (360 << 16);
                b = (base->params3D.grp5_p2 * arg->field_4 + base->params3D.rot_y) % (360 << 16);
                c = (base->params3D.grp5_p3 * arg->field_4 + base->params3D.rot_z) % (360 << 16);

                base->params3D.rot_x = a + (a < 0 ? (360 << 16) : 0);
                base->params3D.rot_y = b + (b < 0 ? (360 << 16) : 0);
                base->params3D.rot_z = c + (c < 0 ? (360 << 16) : 0);

                make_scale_rotation__matrix(&base->params3D);
            }
        }
    }

    for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
        call_method(kid->self_full, 73, arg);

    return 1;
}

size_t base_func77(NC_STACK_base *obj, class_stru *, base77Func *arg)
{
    __NC_STACK_base *base = &obj->stack__base;

    int v12 = 0;

    if ( base->OBJ_SKELETON )
    {
        if ( !(base->field_4 & 0x40) )
        {
            sub_430A38(&base->params3D);

            skeleton_arg_132 skel132;

            skel132.base_1c = &base->params3D;

            skel132.glob_1c = sub_430A28();

            skel132.field_8 = arg->field_20;
            skel132.field_C = arg->field_24;


            v12 = call_method(base->OBJ_SKELETON, 132, &skel132);

            if ( v12 )
            {
                base->field_DC.field_0 = arg->field_1C;
                base->field_DC.field_C = arg->field_4;
                base->field_DC.field_10 = arg->field_0;
                base->field_DC.field_24 = arg->field_20;
                base->field_DC.field_28 = arg->field_24;
                base->field_DC.outPolys = arg->curOutPoly;
                base->field_DC.polyDat = arg->polysData;
                base->field_DC.glob_1c = skel132.glob_1c;
                base->field_DC.base_1c = skel132.base_1c;

                base->field_DC.OBJ_SKELETON = base->OBJ_SKELETON;
                base->field_DC.field_38 = 0;

                __NC_STACK_ade *cur_ade = (__NC_STACK_ade *)base->ADES.head;

                while(cur_ade->next)
                {
                    if ( base->field_DC.polyDat >= arg->polysData_end )
                        break;

                    if ( (arg->field_14 + base->field_DC.field_38) >= arg->field_18 )
                        break;

                    call_method(cur_ade->self, 65, &base->field_DC);

                    cur_ade = (__NC_STACK_ade *)cur_ade->next;
                }

                arg->field_14 += base->field_DC.field_38;
                arg->curOutPoly = base->field_DC.outPolys;
                arg->polysData = base->field_DC.polyDat;
            }
        }
    }

    for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
    {
        if ( call_method(kid->self_full, 77, arg) )
            v12 = 1;
    }

    return v12;
}

size_t base_func78(NC_STACK_base *obj, class_stru *, base73arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func78");

    __NC_STACK_base *base = &obj->stack__base;

    base->field_4 |= 3;

    if ( arg->dword8->dword91 & 0x80000000 )
    {
        base->params3D.rot_y = 0;
        base->params3D.rot_z = 0;
        base->params3D.rot_x = 0;
    }

    base->params3D.grp5_p1 = -arg->dword8->float8D * 16384.0;
    base->params3D.grp5_p2 = -arg->dword8->float89 * 16384.0;
    base->params3D.grp5_p3 = -arg->dword8->float85 * 16384.0;

    base->params3D.grp3_p1 = -arg->dword8->float81 * base->params3D.scale_rotation.m20;
    base->params3D.grp3_p2 = 0;
    base->params3D.grp3_p3 = -arg->dword8->float81 * base->params3D.scale_rotation.m22;

    base->params3D.grp3_p1 += -arg->dword8->float7D * base->params3D.scale_rotation.m20;
    base->params3D.grp3_p2 += -arg->dword8->float7D * base->params3D.scale_rotation.m21;
    base->params3D.grp3_p3 += -arg->dword8->float7D * base->params3D.scale_rotation.m22;

    return 1;
}

size_t base_func79(NC_STACK_base *obj, class_stru *, NC_STACK_base **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func79");

    __NC_STACK_base *base = &obj->stack__base;
    NC_STACK_base *field_bc = base->field_bc;

    if ( field_bc )
    {
        NC_STACK_base *v4[2];
        v4[0] = (NC_STACK_base *)-1;
        v4[1] = NULL;

        call_method(field_bc, 67, v4);
    }

    if ( arg[0] )
    {
        NC_STACK_base *v4[2];
        v4[0] = NULL;
        v4[1] = NULL;

        call_method(arg[0], 67, v4);
    }

    return 1;
}

class_return base_class_descr;

class_return * class_set_base(int , ...)
{

    memset(base_funcs, 0, sizeof(CLASSFUNC) * 1024);

    base_funcs[0] = (CLASSFUNC)base_func0;
    base_funcs[1] = (CLASSFUNC)base_func1;
    base_funcs[2] = (CLASSFUNC)base_func2;
    base_funcs[3] = (CLASSFUNC)base_func3;
    base_funcs[5] = (CLASSFUNC)base_func5;
    base_funcs[6] = (CLASSFUNC)base_func6;
    base_funcs[64] = (CLASSFUNC)base_func64;
    base_funcs[65] = (CLASSFUNC)base_func65;
    base_funcs[66] = (CLASSFUNC)base_func66;
    base_funcs[67] = (CLASSFUNC)base_func67;
    base_funcs[68] = (CLASSFUNC)base_func68;
    base_funcs[69] = (CLASSFUNC)base_func69;
    base_funcs[70] = (CLASSFUNC)base_func70;
    base_funcs[71] = (CLASSFUNC)base_func71;
    base_funcs[72] = (CLASSFUNC)base_func72;
    base_funcs[73] = (CLASSFUNC)base_func73;
    base_funcs[77] = (CLASSFUNC)base_func77;
    base_funcs[78] = (CLASSFUNC)base_func78;
    base_funcs[79] = (CLASSFUNC)base_func79;

    baseIDcounter = 1;

    base_class_descr.parent = "nucleus.class";

    base_class_descr.vtbl = base_funcs;
    ////base_class_descr.varSize = sizeof(__NC_STACK_base);
    base_class_descr.varSize = sizeof(NC_STACK_base) - offsetof(NC_STACK_base, stack__base); //// HACK
    base_class_descr.field_A = 0;
    return &base_class_descr;
}
