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
#include "embed.h"

#include "engine_gfx.h"



const NewClassDescr NC_STACK_base::description("base.class", &newinstance);



static const int __argsize = (sizeof(polysDat) + 12 * (sizeof(xyz) + sizeof(tUtV) + sizeof(float)) ); // Up to 12 vertexes for one render object

polys renderStack[2100];
uint8_t renderArgStack [2100 * __argsize];
uint8_t *renderArgStackEND = renderArgStack + sizeof(renderArgStack) - 2 * __argsize;


base_1c_struct *dword_546DC0;
int baseIDcounter = 1;


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
    SinCos *angl = &TFEngine::SinCos_table[scrot->rot_x >> 16];

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
    SinCos *angl = &TFEngine::SinCos_table[scrot->rot_y >> 16];

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
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->rot_y >> 16];

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
    SinCos *angl = &TFEngine::SinCos_table[scrot->rot_z >> 16];

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
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->rot_z >> 16];

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
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->rot_y >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->rot_z >> 16];

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
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->rot_x >> 16];
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->rot_y >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->rot_z >> 16];


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

int NC_STACK_base::base_func0__sub0(stack_vals *stak)
{
    stack__base.field_D8 = 4096;

    stack__base.field_DC.field_2C = 3496.0;
    stack__base.field_DC.field_30 = 600.0;
    stack__base.field_DC.field_34 = 255;

    stack__base.flags |= BASE_FLAG_RENDERALL;
    stack__base.params3D.field_94 |= 1;


    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BASE_ATT_SKELET:
                setBASE_skeleton((NC_STACK_skeleton *)stk->value.p_data);
                break;

            case BASE_ATT_ADE:
                setBASE_ADE((NC_STACK_ade *)stk->value.p_data);
                break;

            case BASE_ATT_PARENTFOLLOW:
                setBASE_parentFollow(stk->value.i_data);
                break;

            case BASE_ATT_VISLIMIT:
                setBASE_visLimit(stk->value.i_data);
                break;

            case BASE_ATT_AMBIENTLIGHT:
                setBASE_ambientLight(stk->value.i_data);
                break;

            case BASE_ATT_RENDERALL:
                setBASE_renderAll(stk->value.i_data);
                break;

            case BASE_ATT_INPUTHANDLE:
                setBASE_inputHandle (stk->value.i_data);
                break;

            case BASE_ATT_FADELEN:
                setBASE_fadeLength(stk->value.i_data);
                break;

            case BASE_ATT_STATIC:
                setBASE_static (stk->value.i_data);
                break;

            case BASE_ATT_EMBDRSRC:
                setBASE_embdRsrc(stk->value.i_data);
                break;
            }
            stk++;
        }
    }

    return 1;
}

size_t NC_STACK_base::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;


    __NC_STACK_base *base = &stack__base;

    base->ID = baseIDcounter;
    baseIDcounter++;

    init_list(&base->ADES);
    init_list(&base->KIDS);

    base->params3D.scale_x = 1.0;
    base->params3D.scale_y = 1.0;
    base->params3D.scale_z = 1.0;

    base->kid_node.self_full = this;

    make_scale_rotation__matrix(&base->params3D);

    if ( !base_func0__sub0(stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_base::func1(stack_vals *stak)
{
    __NC_STACK_base *base = &stack__base;

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

    return NC_STACK_nucleus::func1(stak);
}

void NC_STACK_base::base_setter(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BASE_ATT_SKELET:
                setBASE_skeleton((NC_STACK_skeleton *)stk->value.p_data);
                break;

            case BASE_ATT_ADE:
                setBASE_ADE((NC_STACK_ade *)stk->value.p_data);
                break;

            case BASE_ATT_PARENTFOLLOW:
                setBASE_parentFollow(stk->value.i_data);
                break;

            case BASE_ATT_VISLIMIT:
                setBASE_visLimit(stk->value.i_data);
                break;

            case BASE_ATT_AMBIENTLIGHT:
                setBASE_ambientLight(stk->value.i_data);
                break;

            case BASE_ATT_RENDERALL:
                setBASE_renderAll(stk->value.i_data);
                break;

            case BASE_ATT_INPUTHANDLE:
                setBASE_inputHandle (stk->value.i_data);
                break;

            case BASE_ATT_FADELEN:
                setBASE_fadeLength(stk->value.i_data);
                break;

            case BASE_ATT_STATIC:
                setBASE_static (stk->value.i_data);
                break;

            case BASE_ATT_EMBDRSRC:
                setBASE_embdRsrc(stk->value.i_data);
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_base::func2(stack_vals *stak)
{
    base_setter(stak);

    return NC_STACK_nucleus::func2(stak);
}

void NC_STACK_base::base_getter(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;
            case BASE_ATT_SKELET:
                *(NC_STACK_skeleton **)stk->value.p_data = getBASE_skeleton();
                break;
            case BASE_ATT_PARENTFOLLOW:
                *(int *)stk->value.p_data = getBASE_parentFollow();
                break;
            case 0x80001003:
            case 0x80001007:
                *(int *)stk->value.p_data = 0;
                break;
            case BASE_ATT_VISLIMIT:
                *(int *)stk->value.p_data = getBASE_visLimit();
                break;
            case BASE_ATT_AMBIENTLIGHT:
                *(int *)stk->value.p_data = getBASE_ambientLight();
                break;
            case BASE_ATT_RENDERALL:
                *(int *)stk->value.p_data = getBASE_renderAll();
                break;
            case BASE_ATT_INPUTHANDLE:
                *(int *)stk->value.p_data = getBASE_inputHandle();
                break;
            case BASE_ATT_X:
                *(float *)stk->value.p_data = getBASE_x();
                break;
            case BASE_ATT_Y:
                *(float *)stk->value.p_data = getBASE_y();
                break;
            case BASE_ATT_Z:
                *(float *)stk->value.p_data = getBASE_z();
                break;
            case BASE_ATT_VX:
                *(float *)stk->value.p_data = getBASE_vx();
                break;
            case BASE_ATT_VY:
                *(float *)stk->value.p_data = getBASE_vy();
                break;
            case BASE_ATT_VZ:
                *(float *)stk->value.p_data = getBASE_vz();
                break;
            case BASE_ATT_AX:
                *(int *)stk->value.p_data = getBASE_ax();
                break;
            case BASE_ATT_AY:
                *(int *)stk->value.p_data = getBASE_ay();
                break;
            case BASE_ATT_AZ:
                *(int *)stk->value.p_data = getBASE_az();
                break;
            case BASE_ATT_RX:
                *(int *)stk->value.p_data = getBASE_rx();
                break;
            case BASE_ATT_RY:
                *(int *)stk->value.p_data = getBASE_ry();
                break;
            case BASE_ATT_RZ:
                *(int *)stk->value.p_data = getBASE_rz();
                break;
            case BASE_ATT_SX:
                *(float *)stk->value.p_data = getBASE_sx();
                break;
            case BASE_ATT_SY:
                *(float *)stk->value.p_data = getBASE_sy();
                break;
            case BASE_ATT_SZ:
                *(float *)stk->value.p_data = getBASE_sz();
                break;
            case BASE_ATT_ADELIST:
                *(nlist **)stk->value.p_data = getBASE_adeList();
                break;
            case BASE_ATT_PTRANSFORM:
                *(base_1c_struct **)stk->value.p_data = getBASE_pTransform();
                break;
            case BASE_ATT_KIDSLIST:
                *(nlist **)stk->value.p_data = getBASE_kidList();
                break;
            case BASE_ATT_KIDNODE:
                *(base_node **)stk->value.p_data = getBASE_kidNode();
                break;
            case BASE_ATT_RENDERPARAMS:
                *(area_arg_65 **)stk->value.p_data = getBASE_renderParams();
                break;
            case BASE_ATT_MAINKID:
                *(int *)stk->value.p_data = getBASE_mainKid();
                break;
            case BASE_ATT_MAINOBJT:
                *(int *)stk->value.p_data = getBASE_mainObjt();
                break;
            case BASE_ATT_RENDERSTACK:
                *(void **)stk->value.p_data = getBASE_renderStack();
                break;
            case BASE_ATT_ARGSTACK:
                *(void **)stk->value.p_data = getBASE_argStack();
                break;
            case BASE_ATT_ENDARGSTACK:
                *(void **)stk->value.p_data = getBASE_endArgStack();
                break;
            case BASE_ATT_FADELEN:
                *(int *)stk->value.p_data = getBASE_fadeLength();
                break;
            case BASE_ATT_STATIC:
                *(int *)stk->value.p_data = getBASE_static();
                break;
            case BASE_ATT_EMBDRSRC:
                *(int *)stk->value.p_data = getBASE_embdRsrc();
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_base::func3(stack_vals *stak)
{
    base_getter(stak);

    return NC_STACK_nucleus::func3(stak);
}

int base_READ_STRC(NC_STACK_base *obj, __NC_STACK_base *, MFILE *mfile)
{
    if ( obj )
    {
        STRC_base dst;

        if ( mfread(mfile, &dst, sizeof(STRC_base)) )
        {
            dst.p1 = SWAP16(dst.p1);

            dst.grp_1.sx = SWAP32F(dst.grp_1.sx);
            dst.grp_1.sy = SWAP32F(dst.grp_1.sy);
            dst.grp_1.sz = SWAP32F(dst.grp_1.sz);

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
                NC_STACK_base *zzz[2] = {NULL, NULL};

                if ( dst.p17 & 4 )
                    obj->base_func67(zzz);

                flag_xyz v38;

                v38.flag = 7;
                v38.x = dst.grp_1.sx;
                v38.y = dst.grp_1.sy;
                v38.z = dst.grp_1.sz;

                obj->base_func68(&v38);


                v38.x = dst.scale_x;
                v38.y = dst.scale_y;
                v38.z = dst.scale_z;

                obj->base_func72(&v38); // SET_SCALE ?

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

                obj->base_func69(&v38);

                flag_xyz2 v39;

                v39.flag = 7;

                v39.x = dst.rot_x;
                v39.y = dst.rot_y;
                v39.z = dst.rot_z;

                obj->base_func70(&v39);

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

                obj->base_func71(&v39);

                obj->setBASE_renderAll( (dst.p17 & 8) != 0 );

//				stk[1].id = 0x80001007;
//				stk[1].value = (dst.p17 & 0x10) != 0;

                obj->setBASE_inputHandle( (dst.p17 & 0x20) != 0 );
                obj->setBASE_parentFollow( (dst.p17 & 0x40) != 0 );

//				stk[4].id = 0x80001003;
//				stk[4].value = dst.p18;

                obj->setBASE_visLimit( dst.p19 );
                obj->setBASE_ambientLight( dst.p20 );

            }
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
            NC_STACK_ade *ade = dynamic_cast<NC_STACK_ade *>(READ_OBJT(mfile));
            if ( !ade )
                return 0;

            obj->setBASE_ADE(ade);
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
            NC_STACK_base *objt = dynamic_cast<NC_STACK_base *>( READ_OBJT(mfile) );
            if ( !objt )
            {
                return 0;
            }
            kidid++;

            obj->base_func65(&objt);
        }
        else
        {
            read_default(mfile);
        }
    }
    return 1;
}


size_t NC_STACK_base::func5(MFILE **file)
{
    MFILE *mfile = *file;
    int obj_ok = 0;
    __NC_STACK_base *base = NULL;
    int STRC_readed = 0;

    while ( 1 )
    {
        int iff_res = read_next_IFF(mfile, 2);

        if ( iff_res == -2 )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                func1(NULL);
            return 0;
        }

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);



        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj_ok = NC_STACK_nucleus::func5(file);

            if ( !obj_ok )
                return 0;

            base = &stack__base;

            init_list(&base->ADES);
            init_list(&base->KIDS);

            base->params3D.scale_x = 1.0;
            base->params3D.scale_y = 1.0;
            base->params3D.scale_z = 1.0;

            base->ID = baseIDcounter;

            baseIDcounter++;

            base->kid_node.self_full = this;

            make_scale_rotation__matrix(&base->params3D);

            base->field_D8 = 4096;
            base->field_DC.field_2C = 3496.0;
            base->field_DC.field_30 = 600.0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            STRC_readed = 1;

            if ( !base_READ_STRC(this, base, mfile) )
            {
                func1(NULL);
                return 0;
            }
            read_next_IFF(mfile, 2);
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( obj_ok )
            {
                if ( STRC_readed )
                {
                    NC_STACK_skeleton *skel = dynamic_cast<NC_STACK_skeleton *>(READ_OBJT(mfile));
                    if ( !skel )
                    {
                        func1(NULL);
                        return 0;
                    }
                    setBASE_skeleton(skel);
                }
                else
                {
                    base->OBJT = (NC_STACK_nucleus *)READ_OBJT(mfile);
                }
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADES )
        {
            if ( !base_READ_ADES(this, base, mfile) )
            {
                func1(NULL);
                return 0;
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_KIDS )
        {
            if ( !base_READ_KIDS(this, base, mfile) )
            {
                func1(NULL);
                return 0;
            }
        }
        else
        {
            read_default(mfile);
        }
    }
    return obj_ok;
}

size_t NC_STACK_base::func6(MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_base *base = &stack__base;

    if ( sub_412FC0(*file, TAG_BASE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    if ( base->flags & 0x80 )
    {
        NC_STACK_embed *embd = dynamic_cast<NC_STACK_embed *>(init_get_class("embed.class", NULL));
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

    if ( base->flags & 1 )
        a1.p17 = 1;

    if ( base->flags & 2 )
        a1.p17 |= 2;

    if ( base->flags & 0x40 )
        a1.p17 |= 4;

    if ( base->flags & 8 )
        a1.p17 |= 8;

    if ( base->flags & 0x10 )
        a1.p17 |= 0x10;

    if ( base->flags & 0x20 )
        a1.p17 |= 0x20;

    if ( base->params3D.field_94 & 1 )
        a1.p17 |= 0x40;

    a1.p17 = SWAP16(a1.p17);

    a1.grp_1.sx = SWAP32F(base->params3D.grp_1.sx);
    a1.grp_1.sy = SWAP32F(base->params3D.grp_1.sy);
    a1.grp_1.sz = SWAP32F(base->params3D.grp_1.sz);

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
        s3d->field_10 = prnt_s3d->field_58.m00 * s3d->grp_1.sx + prnt_s3d->field_58.m01 * s3d->grp_1.sy + prnt_s3d->field_58.m02 * s3d->grp_1.sz + prnt_s3d->field_10;
        s3d->field_14 = prnt_s3d->field_58.m10 * s3d->grp_1.sx + prnt_s3d->field_58.m11 * s3d->grp_1.sy + prnt_s3d->field_58.m12 * s3d->grp_1.sz + prnt_s3d->field_14;
        s3d->field_18 = prnt_s3d->field_58.m20 * s3d->grp_1.sx + prnt_s3d->field_58.m21 * s3d->grp_1.sy + prnt_s3d->field_58.m22 * s3d->grp_1.sz + prnt_s3d->field_18;
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
        s3d->field_10 = s3d->grp_1.sx;
        s3d->field_14 = s3d->grp_1.sy;
        s3d->field_18 = s3d->grp_1.sz;

        memcpy(&s3d->field_58, &s3d->scale_rotation, sizeof(mat3x3));
    }
}


int sub_430880(const void * a, const void * b)
{
    polys *a1 = (polys *)a;
    polys *a2 = (polys *)b;

    return a1->range - a2->range;
}


size_t NC_STACK_base::base_func64(base_64arg *arg)
{
    __NC_STACK_base *base = &stack__base;

    base_1c_struct *glob_1c = sub_430A28();

    if ( glob_1c )
        sub_430A38(glob_1c);

    base_func73(arg);

    base77Func base77;
    base77.field_0 = arg->field_4;
    base77.field_4 = arg->field_0;
    base77.rndrSTK_cur = renderStack;
    base77.argSTK_cur = (polysDat *)renderArgStack;
    base77.argSTK_end = (polysDat *)renderArgStackEND;
    base77.field_14 = arg->field_C;
    base77.field_18 = 1000;
    base77.field_1C = base->ID;

    base_func77(&base77);

    arg->field_C = base77.field_14;

    int num = base77.rndrSTK_cur - renderStack;

    arg->field_10 += num;

    if ( num > 1 )
        qsort(renderStack, num, sizeof(polys), sub_430880);

    NC_STACK_win3d *win3d;
    win3d = GFXe.getC3D();


    win3d->display_func257(NULL);
    win3d->setRSTR_BGpen(0);
    win3d->raster_func192(NULL);
    win3d->raster_func213(NULL);

    for (int i = 0; i < num; i++)
    {
        polys *pol = renderStack + i;
        pol->data->render_func(pol->data);
    }

    win3d->raster_func214(NULL);
    win3d->display_func258(NULL);

    return 1;
}

// Fill parent info struct and push it to kid
size_t NC_STACK_base::base_func65(NC_STACK_base **kid)
{
    __NC_STACK_base *base = &stack__base;

    base_66_arg_struct v6; // Parent info

    v6.parent = this;
    v6.KIDS = &base->KIDS;

    if ( base->OBJ_SKELETON )
        v6.parent_field_1c = &base->params3D;
    else
        v6.parent_field_1c = NULL;

    (*kid)->base_func66(&v6);

    return 1;
}

// Add object to parent kids list
size_t NC_STACK_base::base_func66(base_66_arg_struct *prnt_info)
{
    __NC_STACK_base *base = &stack__base;

    if ( base->parent_base )
    {
        Remove(&base->kid_node);

        if ( base->flags & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = (NC_STACK_base *)-1;
            v7[1] = NULL;
            base->parent_base->base_func67(v7);
        }
    }

    base->parent_base = prnt_info->parent;
    if ( prnt_info->parent )
    {
        AddTail(prnt_info->KIDS, &base->kid_node);
        if ( base->flags & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = this;
            v7[1] = this;

            base->parent_base->base_func67(v7);
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

size_t NC_STACK_base::base_func67(NC_STACK_base **arg)
{
    __NC_STACK_base *base = &stack__base;

    if (arg[0] == NULL)
    {
        base->field_b8 = NULL;
        base->flags |= 0x44;
        base->field_bc = this;

        if ( base->parent_base )
        {
            arg[0] = this;
            arg[1] = this;

            base->parent_base->base_func67(arg);
        }
        sub_430A20(&base->params3D);
    }
    else if ((int)arg[0] == -1)
    {
        base->field_bc = 0;
        base->field_b8 = NULL;
        base->flags &= 0xBB;

        if ( !base->parent_base )
            return 1;

        base->parent_base->base_func67(arg);
    }
    else if (arg[0])
    {
        base->field_b8 = arg[0];
        base->field_bc = arg[1];

        base->flags |= 4;
        base->flags &= 0xBF;

        if ( !base->parent_base )
            return 1;

        arg[0] = this;

        base->parent_base->base_func67(arg);
    }

    return 1;
}

size_t NC_STACK_base::base_func68(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->grp_1.sx += arg->x;
        if ( flg & 2 )
            s3d->grp_1.sy += arg->y;
        if ( flg & 4 )
            s3d->grp_1.sz += arg->z;
    }
    else
    {
        if ( flg & 1 )
            s3d->grp_1.sx = arg->x;
        if ( flg & 2 )
            s3d->grp_1.sy = arg->y;
        if ( flg & 4 )
            s3d->grp_1.sz = arg->z;
    }
    return 1;
}

size_t NC_STACK_base::base_func69(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 8 )
    {
        base->flags &= 0xFE;
    }
    else
    {
        base->flags |= 1;

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

size_t NC_STACK_base::base_func70(flag_xyz2 *arg)
{
    __NC_STACK_base *base = &stack__base;

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

size_t NC_STACK_base::base_func71(flag_xyz2 *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    base_1c_struct *s3d = &base->params3D;

    if ( arg->flag & 8 )
    {
        base->flags &= 0xFD;
    }
    else
    {
        base->flags |= 1;

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

size_t NC_STACK_base::base_func72(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

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

size_t NC_STACK_base::base_func73(base_64arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func73");
    __NC_STACK_base *base = &stack__base;

    if ( arg->field_0 != base->field_8 )
    {
        base->field_8 = arg->field_0;

        if ( base->OBJ_SKELETON )
        {
            if ( base->flags & 0x20 )
                base_func78(arg);

            if ( base->flags & 1 )
            {
                base->params3D.grp_1.sx += base->params3D.grp3_p1 * arg->field_4;
                base->params3D.grp_1.sy += base->params3D.grp3_p2 * arg->field_4;
                base->params3D.grp_1.sz += base->params3D.grp3_p3 * arg->field_4;
            }

            if ( base->flags & 2 )
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
        kid->self_full->base_func73(arg);

    return 1;
}

size_t NC_STACK_base::base_func77(base77Func *arg)
{
    __NC_STACK_base *base = &stack__base;

    int v12 = 0;

    if ( base->OBJ_SKELETON )
    {
        if ( !(base->flags & 0x40) )
        {
            sub_430A38(&base->params3D);

            skeleton_arg_132 skel132;

            skel132.base_1c = &base->params3D;

            skel132.glob_1c = sub_430A28();

            skel132.field_8 = arg->field_20;
            skel132.field_C = arg->field_24;


            v12 = base->OBJ_SKELETON->skeleton_func132(&skel132);

            if ( v12 )
            {
                base->field_DC.field_0 = arg->field_1C;
                base->field_DC.field_C = arg->field_4;
                base->field_DC.field_10 = arg->field_0;
                base->field_DC.field_24 = arg->field_20;
                base->field_DC.field_28 = arg->field_24;
                base->field_DC.rndrSTK_cur = arg->rndrSTK_cur;
                base->field_DC.argSTK_cur = arg->argSTK_cur;
                base->field_DC.glob_1c = skel132.glob_1c;
                base->field_DC.base_1c = skel132.base_1c;

                base->field_DC.OBJ_SKELETON = base->OBJ_SKELETON;
                base->field_DC.field_38 = 0;

                __NC_STACK_ade *cur_ade = (__NC_STACK_ade *)base->ADES.head;

                while(cur_ade->next)
                {
                    if ( base->field_DC.argSTK_cur >= arg->argSTK_end )
                        break;

                    if ( (arg->field_14 + base->field_DC.field_38) >= arg->field_18 )
                        break;

                    cur_ade->self->ade_func65(&base->field_DC);

                    cur_ade = (__NC_STACK_ade *)cur_ade->next;
                }

                arg->field_14 += base->field_DC.field_38;
                arg->rndrSTK_cur = base->field_DC.rndrSTK_cur;
                arg->argSTK_cur = base->field_DC.argSTK_cur;
            }
        }
    }

    for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
    {
        if ( kid->self_full->base_func77(arg) )
            v12 = 1;
    }

    return v12;
}

size_t NC_STACK_base::base_func78(base_64arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func78");

    __NC_STACK_base *base = &stack__base;

    base->flags |= 3;

    if ( arg->field_8->but_flags & 0x80000000 )
    {
        base->params3D.rot_y = 0;
        base->params3D.rot_z = 0;
        base->params3D.rot_x = 0;
    }

    base->params3D.grp5_p1 = -arg->field_8->sliders_vars[31] * 16384.0;
    base->params3D.grp5_p2 = -arg->field_8->sliders_vars[30] * 16384.0;
    base->params3D.grp5_p3 = -arg->field_8->sliders_vars[29] * 16384.0;

    base->params3D.grp3_p1 = -arg->field_8->sliders_vars[28] * base->params3D.scale_rotation.m20;
    base->params3D.grp3_p2 = 0;
    base->params3D.grp3_p3 = -arg->field_8->sliders_vars[28] * base->params3D.scale_rotation.m22;

    base->params3D.grp3_p1 += -arg->field_8->sliders_vars[27] * base->params3D.scale_rotation.m20;
    base->params3D.grp3_p2 += -arg->field_8->sliders_vars[27] * base->params3D.scale_rotation.m21;
    base->params3D.grp3_p3 += -arg->field_8->sliders_vars[27] * base->params3D.scale_rotation.m22;

    return 1;
}

size_t NC_STACK_base::base_func79(NC_STACK_base **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func79");

    __NC_STACK_base *base = &stack__base;
    NC_STACK_base *field_bc = base->field_bc;

    if ( field_bc )
    {
        NC_STACK_base *v4[2];
        v4[0] = (NC_STACK_base *)-1;
        v4[1] = NULL;

        field_bc->base_func67(v4);
    }

    if ( arg[0] )
    {
        NC_STACK_base *v4[2];
        v4[0] = NULL;
        v4[1] = NULL;

        (*arg)->base_func67(v4);
    }

    return 1;
}



void NC_STACK_base::setBASE_skeleton(NC_STACK_skeleton *skel)
{
    if (skel)
    {
        if ( stack__base.OBJ_SKELETON )
            delete_class_obj(stack__base.OBJ_SKELETON);

        stack__base.OBJ_SKELETON = skel;
        stack__base.field_DC.field_20 = skel->getSKEL_pSkelet();
    }
}

void NC_STACK_base::setBASE_ADE(NC_STACK_ade *ade)
{
    if (ade)
    {
        nlist *tmp = &stack__base.ADES;
        ade->ade_func64(&tmp);
    }
}

void NC_STACK_base::setBASE_parentFollow(int follow)
{
    if ( follow )
        stack__base.params3D.field_94 |= 1;
    else
        stack__base.params3D.field_94 &= ~1;
}

void NC_STACK_base::setBASE_visLimit(int val)
{
    stack__base.field_D8 = val;
    stack__base.field_DC.field_2C = val - stack__base.field_DC.field_30;
}

void NC_STACK_base::setBASE_ambientLight(int val)
{
    stack__base.field_DC.field_34 = val;
}

void NC_STACK_base::setBASE_renderAll(int rndr)
{
    if (rndr)
        stack__base.flags |= BASE_FLAG_RENDERALL;
    else
        stack__base.flags &= ~BASE_FLAG_RENDERALL;
}

void NC_STACK_base::setBASE_inputHandle(int hndl)
{
    if (hndl)
        stack__base.flags |= BASE_FLAG_INPUTHANDLE;
    else
        stack__base.flags &= ~BASE_FLAG_INPUTHANDLE;
}

void NC_STACK_base::setBASE_fadeLength(int len)
{
    stack__base.field_DC.field_30 = len;
    stack__base.field_DC.field_2C = stack__base.field_D8 - len;
}

void NC_STACK_base::setBASE_static(int stic)
{
    if (stic)
        stack__base.params3D.field_94 |= 2;
    else
        stack__base.params3D.field_94 &= ~2;
}

void NC_STACK_base::setBASE_embdRsrc(int embd)
{
    if (embd)
        stack__base.flags |= BASE_FLAG_EMBDRSRC;
    else
        stack__base.flags &= ~BASE_FLAG_EMBDRSRC;
}

NC_STACK_skeleton *NC_STACK_base::getBASE_skeleton()
{
    return stack__base.OBJ_SKELETON;
}

int NC_STACK_base::getBASE_parentFollow()
{
    if ( (stack__base.params3D.field_94 & 1) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_visLimit()
{
    return stack__base.field_D8;
}

int NC_STACK_base::getBASE_ambientLight()
{
    return stack__base.field_DC.field_34;
}

int NC_STACK_base::getBASE_renderAll()
{
    if ( (stack__base.flags & BASE_FLAG_RENDERALL) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_inputHandle()
{
    if ( (stack__base.flags & BASE_FLAG_INPUTHANDLE) )
        return 1;
    return 0;
}

float NC_STACK_base::getBASE_x()
{
    return stack__base.params3D.grp_1.sx;
}

float NC_STACK_base::getBASE_y()
{
    return stack__base.params3D.grp_1.sy;
}

float NC_STACK_base::getBASE_z()
{
    return stack__base.params3D.grp_1.sz;
}

float NC_STACK_base::getBASE_vx()
{
    return stack__base.params3D.grp3_p1;
}

float NC_STACK_base::getBASE_vy()
{
    return stack__base.params3D.grp3_p2;
}

float NC_STACK_base::getBASE_vz()
{
    return stack__base.params3D.grp3_p3;
}

int NC_STACK_base::getBASE_ax()
{
    return stack__base.params3D.rot_x >> 16;
}

int NC_STACK_base::getBASE_ay()
{
    return stack__base.params3D.rot_y >> 16;
}

int NC_STACK_base::getBASE_az()
{
    return stack__base.params3D.rot_z >> 16;
}

int NC_STACK_base::getBASE_rx()
{
    return stack__base.params3D.grp5_p1 >> 6;
}

int NC_STACK_base::getBASE_ry()
{
    return stack__base.params3D.grp5_p2 >> 6;
}

int NC_STACK_base::getBASE_rz()
{
    return stack__base.params3D.grp5_p3 >> 6;
}

float NC_STACK_base::getBASE_sx()
{
    return stack__base.params3D.scale_x;
}

float NC_STACK_base::getBASE_sy()
{
    return stack__base.params3D.scale_y;
}

float NC_STACK_base::getBASE_sz()
{
    return stack__base.params3D.scale_z;
}

nlist *NC_STACK_base::getBASE_adeList()
{
    return &stack__base.ADES;
}

base_1c_struct *NC_STACK_base::getBASE_pTransform()
{
    return &stack__base.params3D;
}

nlist *NC_STACK_base::getBASE_kidList()
{
    return &stack__base.KIDS;
}

base_node *NC_STACK_base::getBASE_kidNode()
{
    return &stack__base.kid_node;
}

area_arg_65 *NC_STACK_base::getBASE_renderParams()
{
    return &stack__base.field_DC;
}

int NC_STACK_base::getBASE_mainKid()
{
    return (stack__base.flags & BASE_FLAG_MAINKID) != 0;
}

int NC_STACK_base::getBASE_mainObjt()
{
    return (stack__base.flags & BASE_FLAG_MAINOBJT) != 0;
}

polys *NC_STACK_base::getBASE_renderStack()
{
    return renderStack;
}

void *NC_STACK_base::getBASE_argStack()
{
    return renderArgStack;
}

void *NC_STACK_base::getBASE_endArgStack()
{
    return renderArgStackEND;
}

int NC_STACK_base::getBASE_fadeLength()
{
    return stack__base.field_DC.field_30;
}

int NC_STACK_base::getBASE_static()
{
    if ( (stack__base.params3D.field_94 & 2) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_embdRsrc()
{
    if ( (stack__base.flags & BASE_FLAG_EMBDRSRC) )
        return 1;
    return 0;
}




size_t NC_STACK_base::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return (size_t)func2( (stack_vals *)data );
    case 3:
        return (size_t)func3( (stack_vals *)data );
    case 5:
        return (size_t)func5( (MFILE **)data );
    case 6:
        return (size_t)func6( (MFILE **)data );
    case 64:
        return (size_t)base_func64( (base_64arg *)data );
    case 65:
        return (size_t)base_func65( (NC_STACK_base **)data );
    case 66:
        return (size_t)base_func66( (base_66_arg_struct *)data );
    case 67:
        return (size_t)base_func67( (NC_STACK_base **)data );
    case 68:
        return (size_t)base_func68( (flag_xyz *)data );
    case 69:
        return (size_t)base_func69( (flag_xyz *)data );
    case 70:
        return (size_t)base_func70( (flag_xyz2 *)data );
    case 71:
        return (size_t)base_func71( (flag_xyz2 *)data );
    case 72:
        return (size_t)base_func72( (flag_xyz *)data );
    case 73:
        return (size_t)base_func73( (base_64arg *)data );
    case 77:
        return (size_t)base_func77( (base77Func *)data );
    case 78:
        return (size_t)base_func78( (base_64arg *)data );
    case 79:
        return (size_t)base_func79( (NC_STACK_base **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
