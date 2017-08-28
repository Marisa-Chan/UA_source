#include <algorithm>

#include "includes.h"

#include "nucleas.h"

#include "utils.h"
#include "engine_input.h"

#include "bitmap.h"
#include "display.h"
#include "win3d.h"
#include "base.h"
#include "engine_tform.h"
#include "skeleton.h"
#include "ade.h"
#include "embed.h"

#include "engine_gfx.h"

const NewClassDescr NC_STACK_base::description("base.class", &newinstance);
RenderStack NC_STACK_base::renderStack;

RenderStack::RenderStack()
{
    heaps.clear();
    que.clear();

    currentElement = 0;
}

RenderStack::~RenderStack()
{
    for(size_t i = 0; i < heaps.size(); i++)
        delete[] heaps[i];

    heaps.clear();
    que.clear();
}

polysDat *RenderStack::get()
{
    if (currentElement >= heaps.size() * heapSize)
    {
        heaps.push_back( new polysDat[heapSize] );
        que.resize(heaps.size() * heapSize);
    }

    int heapID = currentElement / heapSize;
    int elmID = currentElement % heapSize;

    que[currentElement] = &(heaps[heapID])[elmID];

    return &(heaps[heapID])[elmID];
}

void RenderStack::commit()
{
    currentElement++;

    if (currentElement >= heaps.size() * heapSize)
    {
        heaps.push_back( new polysDat[heapSize] );
        que.resize(heaps.size() * heapSize);
    }
}

size_t RenderStack::getSize()
{
    return currentElement;
}

void RenderStack::clear(bool dealloc)
{
    if (dealloc)
    {
        for (size_t i = 0; i < heaps.size(); i++)
            delete[] heaps[i];

        heaps.clear();
        que.clear();
    }

    currentElement = 0;
}

bool RenderStack::compare(polysDat *a, polysDat *b)
{
    return a->range < b->range;
}

bool RenderStack::comparePrio(polysDat *a, polysDat *b)
{
    if ((a->datSub.renderFlags | b->datSub.renderFlags) & NC_STACK_display::RFLAGS_SKY )
    {
        if ( (a->datSub.renderFlags & b->datSub.renderFlags) & NC_STACK_display::RFLAGS_SKY )
            return a->range > b->range;
        else if ( b->datSub.renderFlags & NC_STACK_display::RFLAGS_SKY )
            return false;
    }
    else if ((a->datSub.renderFlags | b->datSub.renderFlags) & NC_STACK_display::RFLAGS_FALLOFF )
    {
        if ( (a->datSub.renderFlags & b->datSub.renderFlags) & NC_STACK_display::RFLAGS_FALLOFF )
            return a->range > b->range;
        else if ( b->datSub.renderFlags & NC_STACK_display::RFLAGS_FALLOFF )
            return false;
    }

    return true;
}

void RenderStack::render(bool sorting, tCompare _func, bool Clear)
{
    std::deque<polysDat *>::iterator qEnd = que.begin() + currentElement;

    if (sorting && currentElement > 1)
    {
        if (_func)
            std::stable_sort(que.begin(), qEnd, _func);
        else
            std::stable_sort(que.begin(), qEnd, compare);

        qEnd = que.begin() + currentElement;
    }

    for(std::deque<polysDat *>::iterator it = que.begin(); it != qEnd; it++)
    {
        polysDat *pol = *it;
        pol->render_func( pol );
    }

    if (Clear)
        clear();
}



TForm3D *dword_546DC0;
int baseIDcounter = 1;


// rot_x == 0   rot_y == 0   rot_z == 0
void scale_rot_0(TForm3D *scrot)
{
    scrot->locSclRot.m00 = scrot->scale.sx;
    scrot->locSclRot.m01 = 0;
    scrot->locSclRot.m02 = 0;

    scrot->locSclRot.m10 = 0;
    scrot->locSclRot.m11 = scrot->scale.sy;
    scrot->locSclRot.m12 = 0;

    scrot->locSclRot.m20 = 0;
    scrot->locSclRot.m21 = 0;
    scrot->locSclRot.m22 = scrot->scale.sz;
}

// rot_x != 0   rot_y == 0   rot_z == 0
void scale_rot_1(TForm3D *scrot)
{
    SinCos *angl = &TFEngine::SinCos_table[scrot->ax >> 16];

    scrot->locSclRot.m00 = scrot->scale.sx;
    scrot->locSclRot.m01 = 0;
    scrot->locSclRot.m02 = 0;

    scrot->locSclRot.m10 = 0;
    scrot->locSclRot.m11 = scrot->scale.sy * angl->cos;
    scrot->locSclRot.m12 = scrot->scale.sy * -angl->sin;

    scrot->locSclRot.m20 = 0;
    scrot->locSclRot.m21 = scrot->scale.sz * angl->sin;
    scrot->locSclRot.m22 = scrot->scale.sz * angl->cos;

}

// rot_x == 0   rot_y != 0   rot_z == 0
void scale_rot_2(TForm3D *scrot)
{
    SinCos *angl = &TFEngine::SinCos_table[scrot->ay >> 16];

    scrot->locSclRot.m00 = scrot->scale.sx * angl->cos;
    scrot->locSclRot.m01 = 0;
    scrot->locSclRot.m02 = scrot->scale.sx * angl->sin;

    scrot->locSclRot.m10 = 0;
    scrot->locSclRot.m11 = scrot->scale.sy;
    scrot->locSclRot.m12 = 0;

    scrot->locSclRot.m20 = -angl->sin * scrot->scale.sz;
    scrot->locSclRot.m21 = 0;
    scrot->locSclRot.m22 = scrot->scale.sz * angl->cos;
}

// rot_x != 0   rot_y != 0   rot_z == 0
void scale_rot_3(TForm3D *scrot)
{
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->ax >> 16];
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->ay >> 16];

    scrot->locSclRot.m00 = scrot->scale.sx * angl_y->cos;
    scrot->locSclRot.m01 = 0;
    scrot->locSclRot.m02 = scrot->scale.sx * angl_y->sin;

    scrot->locSclRot.m10 = angl_x->sin * angl_y->sin * scrot->scale.sy;
    scrot->locSclRot.m11 = scrot->scale.sy * angl_x->cos;
    scrot->locSclRot.m12 = -angl_x->sin * angl_y->cos * scrot->scale.sy;

    scrot->locSclRot.m20 = -angl_x->cos * angl_y->sin * scrot->scale.sz;
    scrot->locSclRot.m21 = scrot->scale.sz * angl_x->sin;
    scrot->locSclRot.m22 = angl_x->cos * angl_y->cos * scrot->scale.sz;
}

// rot_x == 0   rot_y == 0   rot_z != 0
void scale_rot_4(TForm3D *scrot)
{
    SinCos *angl = &TFEngine::SinCos_table[scrot->az >> 16];

    scrot->locSclRot.m00 = scrot->scale.sx * angl->cos;
    scrot->locSclRot.m01 = -angl->sin * scrot->scale.sx;
    scrot->locSclRot.m02 = 0;

    scrot->locSclRot.m10 = scrot->scale.sy * angl->sin;
    scrot->locSclRot.m11 = scrot->scale.sy * angl->cos;
    scrot->locSclRot.m12 = 0;

    scrot->locSclRot.m20 = 0;
    scrot->locSclRot.m21 = 0;
    scrot->locSclRot.m22 = scrot->scale.sz;
}

// rot_x != 0   rot_y == 0   rot_z != 0
void scale_rot_5(TForm3D *scrot)
{
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->ax >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->az >> 16];

    scrot->locSclRot.m00 = scrot->scale.sx * angl_z->cos;
    scrot->locSclRot.m01 = -angl_z->sin * angl_x->cos * scrot->scale.sx;
    scrot->locSclRot.m02 = angl_z->sin * angl_x->sin * scrot->scale.sx;

    scrot->locSclRot.m10 = scrot->scale.sy * angl_z->sin;
    scrot->locSclRot.m11 = angl_z->cos * angl_x->cos * scrot->scale.sy;
    scrot->locSclRot.m12 = -angl_z->cos * angl_x->sin * scrot->scale.sy;

    scrot->locSclRot.m20 = 0;
    scrot->locSclRot.m21 = scrot->scale.sz * angl_x->sin;
    scrot->locSclRot.m22 = scrot->scale.sz * angl_x->cos;
}

// rot_x == 0   rot_y != 0   rot_z != 0
void scale_rot_6(TForm3D *scrot)
{
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->ay >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->az >> 16];

    scrot->locSclRot.m00 = angl_z->cos * angl_y->cos * scrot->scale.sx;
    scrot->locSclRot.m01 = -angl_z->sin * scrot->scale.sx;
    scrot->locSclRot.m02 = angl_z->cos * angl_y->sin * scrot->scale.sx;

    scrot->locSclRot.m10 = angl_z->sin * angl_y->cos * scrot->scale.sy;
    scrot->locSclRot.m11 = scrot->scale.sy * angl_z->cos;
    scrot->locSclRot.m12 = angl_z->sin * angl_y->sin * scrot->scale.sy;

    scrot->locSclRot.m20 = -angl_y->sin * scrot->scale.sz;
    scrot->locSclRot.m21 = 0;
    scrot->locSclRot.m22 = scrot->scale.sz * angl_y->cos;

}

// rot_x != 0   rot_y != 0   rot_z != 0
void scale_rot_7(TForm3D *scrot)
{
    SinCos *angl_x = &TFEngine::SinCos_table[scrot->ax >> 16];
    SinCos *angl_y = &TFEngine::SinCos_table[scrot->ay >> 16];
    SinCos *angl_z = &TFEngine::SinCos_table[scrot->az >> 16];


    scrot->locSclRot.m00 = (angl_z->cos * angl_y->cos - angl_z->sin * angl_x->sin * angl_y->sin) * scrot->scale.sx;
    scrot->locSclRot.m01 = -angl_z->sin * angl_x->cos * scrot->scale.sx;
    scrot->locSclRot.m02 = (angl_z->cos * angl_y->sin + angl_z->sin * angl_x->sin * angl_y->cos) * scrot->scale.sx;

    scrot->locSclRot.m10 = (angl_z->sin * angl_y->cos + angl_z->cos * angl_x->sin * angl_y->sin) * scrot->scale.sy;
    scrot->locSclRot.m11 = angl_z->cos * angl_x->cos * scrot->scale.sy;
    scrot->locSclRot.m12 = (angl_z->sin * angl_y->sin - angl_z->cos * angl_x->sin * angl_y->cos) * scrot->scale.sy;

    scrot->locSclRot.m20 = -angl_x->cos * angl_y->sin * scrot->scale.sz;
    scrot->locSclRot.m21 = scrot->scale.sz * angl_x->sin;
    scrot->locSclRot.m22 = angl_x->cos * angl_y->cos * scrot->scale.sz;
}


void make_scale_rotation__matrix(TForm3D *scrot)
{
    typedef void (*matrfunc)(TForm3D *);
    matrfunc off_5151E0[8] = {scale_rot_0, scale_rot_1, scale_rot_2, scale_rot_3, scale_rot_4, scale_rot_5, scale_rot_6, scale_rot_7};

    uint8_t func_id = 0;

    if ( scrot->ax )
        func_id |= 1;

    if ( scrot->ay )
        func_id |= 2;

    if ( scrot->az )
        func_id |= 4;

    off_5151E0[func_id](scrot);
}

int NC_STACK_base::base_func0__sub0(stack_vals *stak)
{
    stack__base.visLimit = 4096;

    stack__base.renderMsg.fadeStart = 3496.0;
    stack__base.renderMsg.fadeLength = 600.0;
    stack__base.renderMsg.ambientLight = 255;

    stack__base.flags |= BASE_FLAG_RENDERALL;
    stack__base.transform.flags |= 1;


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

    base->transform.scale.sx = 1.0;
    base->transform.scale.sy = 1.0;
    base->transform.scale.sz = 1.0;

    base->kid_node.self_full = this;

    make_scale_rotation__matrix(&base->transform);

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

    if ( base->parent )
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
                *(TForm3D **)stk->value.p_data = getBASE_pTransform();
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
//                *(void **)stk->value.p_data = getBASE_renderStack();
                break;
            case BASE_ATT_ARGSTACK:
//                *(void **)stk->value.p_data = getBASE_argStack();
                break;
            case BASE_ATT_ENDARGSTACK:
//                *(void **)stk->value.p_data = getBASE_endArgStack();
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

int base_READ_STRC(NC_STACK_base *obj, __NC_STACK_base *, IFFile *mfile)
{
    if ( obj )
    {
        bool readOK = true;

        STRC_base dst;

        readOK &= mfile->readS16B(dst.version);

        readOK &= mfile->readFloatB(dst.pos.sx);
        readOK &= mfile->readFloatB(dst.pos.sy);
        readOK &= mfile->readFloatB(dst.pos.sz);

        readOK &= mfile->readFloatB(dst.vec.sx);
        readOK &= mfile->readFloatB(dst.vec.sy);
        readOK &= mfile->readFloatB(dst.vec.sz);

        readOK &= mfile->readFloatB(dst.scale.sx);
        readOK &= mfile->readFloatB(dst.scale.sy);
        readOK &= mfile->readFloatB(dst.scale.sz);

        readOK &= mfile->readS16B(dst.ax);
        readOK &= mfile->readS16B(dst.ay);
        readOK &= mfile->readS16B(dst.az);

        readOK &= mfile->readS16B(dst.rx);
        readOK &= mfile->readS16B(dst.ry);
        readOK &= mfile->readS16B(dst.rz);

        readOK &= mfile->readS16B(dst.attFlags);
        readOK &= mfile->readS16B(dst._un1);

        readOK &= mfile->readS32B(dst.visLimit);
        readOK &= mfile->readS32B(dst.ambientLight);

        if (!readOK)
            return 0;

        if ( dst.version >= 1 )
        {
            NC_STACK_base *zzz[2] = {NULL, NULL};

            if ( dst.attFlags & 4 )
                obj->base_func67(zzz);

            flag_xyz v38;

            v38.flag = 7;
            v38.x = dst.pos.sx;
            v38.y = dst.pos.sy;
            v38.z = dst.pos.sz;

            obj->base_func68(&v38);


            v38.x = dst.scale.sx;
            v38.y = dst.scale.sy;
            v38.z = dst.scale.sz;

            obj->base_func72(&v38); // SET_SCALE ?

            if ( dst.attFlags & 1 )
            {
                v38.x = dst.vec.sx;
                v38.y = dst.vec.sy;
                v38.z = dst.vec.sz;
            }
            else
            {
                v38.flag = 8;
            }

            obj->base_func69(&v38);

            flag_xyz2 v39;

            v39.flag = 7;

            v39.x = dst.ax;
            v39.y = dst.ay;
            v39.z = dst.az;

            obj->base_func70(&v39);

            if ( dst.attFlags & 2 )
            {
                v39.x = dst.rx;
                v39.y = dst.ry;
                v39.z = dst.rz;
            }
            else
            {
                v39.flag = 8;
            }

            obj->base_func71(&v39);

            obj->setBASE_renderAll( (dst.attFlags & 8) != 0 );

//				stk[1].id = 0x80001007;
//				stk[1].value = (dst.p17 & 0x10) != 0;

            obj->setBASE_inputHandle( (dst.attFlags & 0x20) != 0 );
            obj->setBASE_parentFollow( (dst.attFlags & 0x40) != 0 );

//				stk[4].id = 0x80001003;
//				stk[4].value = dst.p18;

            obj->setBASE_visLimit( dst.visLimit );
            obj->setBASE_ambientLight( dst.ambientLight );

        }
        return 1;
    }
    return 0;
}

int base_READ_ADES(NC_STACK_base *obj, __NC_STACK_base *, IFFile *mfile)
{
    if (!obj)
        return 0;

    while ( 1 )
    {
        int v5 = mfile->parse();
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            NC_STACK_ade *ade = dynamic_cast<NC_STACK_ade *>(READ_OBJT(mfile));
            if ( !ade )
                return 0;

            obj->setBASE_ADE(ade);
        }
        else
        {
            mfile->skipChunk();
        }
    }
    return 1;
}

int base_READ_KIDS(NC_STACK_base *obj, __NC_STACK_base *, IFFile *mfile)
{
    if (!obj)
        return 0;

    int kidid = 0;

    while ( 1 )
    {
        int v5 = mfile->parse();
        if ( v5 == -2 )
            break;
        if ( v5 )
            return 0;

        IFFile::Context *chunk = mfile->getCurrentChunk();

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
            mfile->skipChunk();
        }
    }
    return 1;
}


size_t NC_STACK_base::func5(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;
    __NC_STACK_base *base = NULL;
    int STRC_readed = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == IFFile::IFF_ERR_EOC )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                func1(NULL);
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();



        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj_ok = NC_STACK_nucleus::func5(file);

            if ( !obj_ok )
                return 0;

            base = &stack__base;

            init_list(&base->ADES);
            init_list(&base->KIDS);

            base->transform.scale.sx = 1.0;
            base->transform.scale.sy = 1.0;
            base->transform.scale.sz = 1.0;

            base->ID = baseIDcounter;

            baseIDcounter++;

            base->kid_node.self_full = this;

            make_scale_rotation__matrix(&base->transform);

            base->visLimit = 4096;
            base->renderMsg.fadeStart = 3496.0;
            base->renderMsg.fadeLength = 600.0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            STRC_readed = 1;

            if ( !base_READ_STRC(this, base, mfile) )
            {
                func1(NULL);
                return 0;
            }
            mfile->parse();
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
            mfile->skipChunk();
        }
    }
    return obj_ok;
}

size_t NC_STACK_base::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_base *base = &stack__base;

    if ( mfile->pushChunk(TAG_BASE, TAG_FORM, -1) )
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

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1); // version
    mfile->writeFloatB(base->transform.locPos.sx);
    mfile->writeFloatB(base->transform.locPos.sy);
    mfile->writeFloatB(base->transform.locPos.sz);

    mfile->writeFloatB(base->transform.vec.sx);
    mfile->writeFloatB(base->transform.vec.sy);
    mfile->writeFloatB(base->transform.vec.sz);

    mfile->writeFloatB(base->transform.scale.sx);
    mfile->writeFloatB(base->transform.scale.sy);
    mfile->writeFloatB(base->transform.scale.sz);

    mfile->writeS16B(base->transform.ax >> 16);
    mfile->writeS16B(base->transform.ay >> 16);
    mfile->writeS16B(base->transform.az >> 16);

    mfile->writeS16B(base->transform.rx >> 6);
    mfile->writeS16B(base->transform.ry >> 6);
    mfile->writeS16B(base->transform.rz >> 6);

    int16_t attFlags = 0;
    if ( base->flags & 1 )
        attFlags = 1;

    if ( base->flags & 2 )
        attFlags |= 2;

    if ( base->flags & 0x40 )
        attFlags |= 4;

    if ( base->flags & 8 )
        attFlags |= 8;

    if ( base->flags & 0x10 )
        attFlags |= 0x10;

    if ( base->flags & 0x20 )
        attFlags |= 0x20;

    if ( base->transform.flags & 1 )
        attFlags |= 0x40;

    mfile->writeS16B(attFlags);

    mfile->writeS16B(0); // _un1

    mfile->writeS32B(base->renderMsg.fadeStart + base->renderMsg.fadeLength); // visLimit
    mfile->writeS32B(base->renderMsg.ambientLight); // ambientLight

    mfile->popChunk();

    if ( base->OBJ_SKELETON )
    {
        if ( !sub_4117F8(base->OBJ_SKELETON, mfile) )
            return 0;

        if ( base->ADES.head->next )
        {
            mfile->pushChunk(TAG_ADES, TAG_FORM, -1);
            for ( __NC_STACK_ade *cur_ade = (__NC_STACK_ade *)base->ADES.head; cur_ade->next; cur_ade = (__NC_STACK_ade *)cur_ade->next )
            {
                if ( !sub_4117F8(cur_ade->self, mfile) )
                    return 0;
            }
            mfile->popChunk();
        }
    }

    if ( base->KIDS.head->next )
    {
        mfile->pushChunk(TAG_KIDS, TAG_FORM, -1);
        for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
        {
            if ( !sub_4117F8(kid->self_full, mfile) )
                return 0;
        }
        mfile->popChunk();
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

void sub_430A38(TForm3D *s3d)
{
    TForm3D *prnt_s3d = s3d->parent_1c;

    if ( s3d->parent_1c && s3d->flags & 1 )
    {
        s3d->globPos.sx = prnt_s3d->globSclRot.m00 * s3d->locPos.sx + prnt_s3d->globSclRot.m01 * s3d->locPos.sy + prnt_s3d->globSclRot.m02 * s3d->locPos.sz + prnt_s3d->globPos.sx;
        s3d->globPos.sy = prnt_s3d->globSclRot.m10 * s3d->locPos.sx + prnt_s3d->globSclRot.m11 * s3d->locPos.sy + prnt_s3d->globSclRot.m12 * s3d->locPos.sz + prnt_s3d->globPos.sy;
        s3d->globPos.sz = prnt_s3d->globSclRot.m20 * s3d->locPos.sx + prnt_s3d->globSclRot.m21 * s3d->locPos.sy + prnt_s3d->globSclRot.m22 * s3d->locPos.sz + prnt_s3d->globPos.sz;
        s3d->globSclRot.m00 = prnt_s3d->globSclRot.m00 * s3d->locSclRot.m00 + prnt_s3d->globSclRot.m01 * s3d->locSclRot.m10 + prnt_s3d->globSclRot.m02 * s3d->locSclRot.m20;
        s3d->globSclRot.m01 = prnt_s3d->globSclRot.m00 * s3d->locSclRot.m01 + prnt_s3d->globSclRot.m01 * s3d->locSclRot.m11 + prnt_s3d->globSclRot.m02 * s3d->locSclRot.m21;
        s3d->globSclRot.m02 = prnt_s3d->globSclRot.m00 * s3d->locSclRot.m02 + prnt_s3d->globSclRot.m01 * s3d->locSclRot.m12 + prnt_s3d->globSclRot.m02 * s3d->locSclRot.m22;
        s3d->globSclRot.m10 = prnt_s3d->globSclRot.m10 * s3d->locSclRot.m00 + prnt_s3d->globSclRot.m11 * s3d->locSclRot.m10 + prnt_s3d->globSclRot.m12 * s3d->locSclRot.m20;
        s3d->globSclRot.m11 = prnt_s3d->globSclRot.m10 * s3d->locSclRot.m01 + prnt_s3d->globSclRot.m11 * s3d->locSclRot.m11 + prnt_s3d->globSclRot.m12 * s3d->locSclRot.m21;
        s3d->globSclRot.m12 = prnt_s3d->globSclRot.m10 * s3d->locSclRot.m02 + prnt_s3d->globSclRot.m11 * s3d->locSclRot.m12 + prnt_s3d->globSclRot.m12 * s3d->locSclRot.m22;
        s3d->globSclRot.m20 = prnt_s3d->globSclRot.m20 * s3d->locSclRot.m00 + prnt_s3d->globSclRot.m21 * s3d->locSclRot.m10 + prnt_s3d->globSclRot.m22 * s3d->locSclRot.m20;
        s3d->globSclRot.m21 = prnt_s3d->globSclRot.m20 * s3d->locSclRot.m01 + prnt_s3d->globSclRot.m21 * s3d->locSclRot.m11 + prnt_s3d->globSclRot.m22 * s3d->locSclRot.m21;
        s3d->globSclRot.m22 = prnt_s3d->globSclRot.m20 * s3d->locSclRot.m02 + prnt_s3d->globSclRot.m21 * s3d->locSclRot.m12 + prnt_s3d->globSclRot.m22 * s3d->locSclRot.m22;
    }
    else
    {
        s3d->globPos.sx = s3d->locPos.sx;
        s3d->globPos.sy = s3d->locPos.sy;
        s3d->globPos.sz = s3d->locPos.sz;

        s3d->globSclRot = s3d->locSclRot;
    }
}

size_t NC_STACK_base::base_func64(base_64arg *arg)
{
    __NC_STACK_base *base = &stack__base;

    TForm3D *glob_1c = sub_430A28();

    if ( glob_1c )
        sub_430A38(glob_1c);

    base_func73(arg);

    baseRender_msg base77;
    base77.frameTime = arg->field_4;
    base77.globTime = arg->field_0;
    base77.rndrStack = &renderStack;
    base77.adeCount = arg->field_C;
    base77.ownerID = base->ID;
    base77.minZ = 1.0;
    base77.maxZ = 1000.0;
    base77.flags = 0;

    base_func77(&base77);

    arg->field_C = base77.adeCount;

    arg->field_10 += renderStack.getSize();

    NC_STACK_win3d *win3d;
    win3d = GFXEngine::GFXe.getC3D();


    win3d->BeginFrame();
    win3d->setRSTR_BGpen(0);
    win3d->raster_func192(NULL);

    win3d->BeginScene();

    renderStack.render();

    win3d->EndScene();
    win3d->EndFrame();

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
        v6.parent_field_1c = &base->transform;
    else
        v6.parent_field_1c = NULL;

    (*kid)->base_func66(&v6);

    return 1;
}

// Add object to parent kids list
size_t NC_STACK_base::base_func66(base_66_arg_struct *prnt_info)
{
    __NC_STACK_base *base = &stack__base;

    if ( base->parent )
    {
        Remove(&base->kid_node);

        if ( base->flags & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = (NC_STACK_base *)-1;
            v7[1] = NULL;
            base->parent->base_func67(v7);
        }
    }

    base->parent = prnt_info->parent;
    if ( prnt_info->parent )
    {
        AddTail(prnt_info->KIDS, &base->kid_node);
        if ( base->flags & 4 )
        {
            NC_STACK_base *v7[2];
            v7[0] = this;
            v7[1] = this;

            base->parent->base_func67(v7);
        }
    }
    base->transform.parent_1c = prnt_info->parent_field_1c;

    return 1;
}

void sub_430A20(TForm3D *s3d)
{
    dword_546DC0 = s3d;
}

TForm3D *sub_430A28()
{
    return dword_546DC0;
}

size_t NC_STACK_base::base_func67(NC_STACK_base **arg)
{
    __NC_STACK_base *base = &stack__base;

    if (arg[0] == NULL)
    {
        base->mainChild = NULL;
        base->flags |= 0x44;
        base->mainObject = this;

        if ( base->parent )
        {
            arg[0] = this;
            arg[1] = this;

            base->parent->base_func67(arg);
        }
        sub_430A20(&base->transform);
    }
    else if ((int)(size_t)arg[0] == -1)
    {
        base->mainObject = 0;
        base->mainChild = NULL;
        base->flags &= 0xBB;

        if ( !base->parent )
            return 1;

        base->parent->base_func67(arg);
    }
    else if (arg[0])
    {
        base->mainChild = arg[0];
        base->mainObject = arg[1];

        base->flags |= 4;
        base->flags &= 0xBF;

        if ( !base->parent )
            return 1;

        arg[0] = this;

        base->parent->base_func67(arg);
    }

    return 1;
}

size_t NC_STACK_base::base_func68(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    TForm3D *s3d = &base->transform;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->locPos.sx += arg->x;
        if ( flg & 2 )
            s3d->locPos.sy += arg->y;
        if ( flg & 4 )
            s3d->locPos.sz += arg->z;
    }
    else
    {
        if ( flg & 1 )
            s3d->locPos.sx = arg->x;
        if ( flg & 2 )
            s3d->locPos.sy = arg->y;
        if ( flg & 4 )
            s3d->locPos.sz = arg->z;
    }
    return 1;
}

size_t NC_STACK_base::base_func69(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    TForm3D *s3d = &base->transform;

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
                s3d->vec.sx += arg->x;
            if ( flg & 2 )
                s3d->vec.sy += arg->y;
            if ( flg & 4 )
                s3d->vec.sz += arg->z;
        }
        else
        {
            if ( flg & 1 )
                s3d->vec.sx = arg->x;
            if ( flg & 2 )
                s3d->vec.sy = arg->y;
            if ( flg & 4 )
                s3d->vec.sz = arg->z;
        }
    }

    return 1;
}

size_t NC_STACK_base::base_func70(flag_xyz2 *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    TForm3D *s3d = &base->transform;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->ax = (s3d->ax + (arg->x << 16)) % (360 << 16);
        if ( flg & 2 )
            s3d->ay = (s3d->ay + (arg->y << 16)) % (360 << 16);
        if ( flg & 4 )
            s3d->az = (s3d->az + (arg->z << 16)) % (360 << 16);
    }
    else
    {
        if ( flg & 1 )
            s3d->ax = arg->x << 16;
        if ( flg & 2 )
            s3d->ay = arg->y << 16;
        if ( flg & 4 )
            s3d->az = arg->z << 16;
    }

    make_scale_rotation__matrix(s3d);

    return 1;
}

size_t NC_STACK_base::base_func71(flag_xyz2 *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    TForm3D *s3d = &base->transform;

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
                s3d->rx = (s3d->rx + (arg->x << 6)) % (360 << 16);
            if ( flg & 2 )
                s3d->ry = (s3d->ry + (arg->y << 6)) % (360 << 16);
            if ( flg & 4 )
                s3d->rz = (s3d->rz + (arg->z << 6)) % (360 << 16);
        }
        else
        {
            if ( flg & 1 )
                s3d->rx = arg->x << 6;
            if ( flg & 2 )
                s3d->ry = arg->y << 6;
            if ( flg & 4 )
                s3d->rz = arg->z << 6;
        }
    }

    return 1;
}

size_t NC_STACK_base::base_func72(flag_xyz *arg)
{
    __NC_STACK_base *base = &stack__base;

    int flg = arg->flag;

    TForm3D *s3d = &base->transform;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            s3d->scale.sx *= arg->x;
        if ( flg & 2 )
            s3d->scale.sy *= arg->y;
        if ( flg & 4 )
            s3d->scale.sz *= arg->z;
    }
    else
    {
        if ( flg & 1 )
            s3d->scale.sx = arg->x;
        if ( flg & 2 )
            s3d->scale.sy = arg->y;
        if ( flg & 4 )
            s3d->scale.sz = arg->z;
    }

    make_scale_rotation__matrix(s3d);

    return 1;
}

size_t NC_STACK_base::base_func73(base_64arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func73");
    __NC_STACK_base *base = &stack__base;

    if ( arg->field_0 != base->timeStamp )
    {
        base->timeStamp = arg->field_0;

        if ( base->OBJ_SKELETON )
        {
            if ( base->flags & 0x20 )
                base_func78(arg);

            if ( base->flags & 1 )
            {
                base->transform.locPos.sx += base->transform.vec.sx * arg->field_4;
                base->transform.locPos.sy += base->transform.vec.sy * arg->field_4;
                base->transform.locPos.sz += base->transform.vec.sz * arg->field_4;
            }

            if ( base->flags & 2 )
            {
                int a,b,c;
                a = (base->transform.rx * arg->field_4 + base->transform.ax) % (360 << 16);
                b = (base->transform.ry * arg->field_4 + base->transform.ay) % (360 << 16);
                c = (base->transform.rz * arg->field_4 + base->transform.az) % (360 << 16);

                base->transform.ax = a + (a < 0 ? (360 << 16) : 0);
                base->transform.ay = b + (b < 0 ? (360 << 16) : 0);
                base->transform.az = c + (c < 0 ? (360 << 16) : 0);

                make_scale_rotation__matrix(&base->transform);
            }
        }
    }

    for ( base_node *kid = (base_node *)base->KIDS.head; kid->next; kid = (base_node *)kid->next )
        kid->self_full->base_func73(arg);

    return 1;
}

size_t NC_STACK_base::base_func77(baseRender_msg *arg)
{
    __NC_STACK_base *base = &stack__base;

    int v12 = 0;

    if ( base->OBJ_SKELETON )
    {
        if ( !(base->flags & 0x40) )
        {
            sub_430A38(&base->transform);

            skeleton_arg_132 skel132;

            skel132.base_1c = &base->transform;

            skel132.glob_1c = sub_430A28();

            skel132.minZ = arg->minZ;
            skel132.maxZ = arg->maxZ;


            v12 = base->OBJ_SKELETON->skeleton_func132(&skel132);

            if ( v12 )
            {
                base->renderMsg.ownerID = arg->ownerID;
                base->renderMsg.timeStamp = arg->globTime;
                base->renderMsg.frameTime = arg->frameTime;
                base->renderMsg.minZ = arg->minZ;
                base->renderMsg.maxZ = arg->maxZ;
                base->renderMsg.rndrStack = arg->rndrStack;
                base->renderMsg.view = skel132.glob_1c;
                base->renderMsg.owner = skel132.base_1c;
                base->renderMsg.flags = arg->flags;

                base->renderMsg.OBJ_SKELETON = base->OBJ_SKELETON;
                base->renderMsg.adeCount = 0;

                __NC_STACK_ade *cur_ade = (__NC_STACK_ade *)base->ADES.head;

                while(cur_ade->next)
                {
                    cur_ade->self->ade_func65(&base->renderMsg);

                    cur_ade = (__NC_STACK_ade *)cur_ade->next;
                }

                arg->adeCount += base->renderMsg.adeCount;
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
        base->transform.ay = 0;
        base->transform.az = 0;
        base->transform.ax = 0;
    }

    base->transform.rx = -arg->field_8->sliders_vars[31] * 16384.0;
    base->transform.ry = -arg->field_8->sliders_vars[30] * 16384.0;
    base->transform.rz = -arg->field_8->sliders_vars[29] * 16384.0;

    base->transform.vec.sx = -arg->field_8->sliders_vars[28] * base->transform.locSclRot.m20;
    base->transform.vec.sy = 0;
    base->transform.vec.sz = -arg->field_8->sliders_vars[28] * base->transform.locSclRot.m22;

    base->transform.vec.sx += -arg->field_8->sliders_vars[27] * base->transform.locSclRot.m20;
    base->transform.vec.sy += -arg->field_8->sliders_vars[27] * base->transform.locSclRot.m21;
    base->transform.vec.sz += -arg->field_8->sliders_vars[27] * base->transform.locSclRot.m22;

    return 1;
}

size_t NC_STACK_base::base_func79(NC_STACK_base **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func79");

    __NC_STACK_base *base = &stack__base;
    NC_STACK_base *field_bc = base->mainObject;

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
        stack__base.renderMsg.sklt = skel->getSKEL_pSkelet();
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
        stack__base.transform.flags |= 1;
    else
        stack__base.transform.flags &= ~1;
}

void NC_STACK_base::setBASE_visLimit(int val)
{
    stack__base.visLimit = val;
    stack__base.renderMsg.fadeStart = val - stack__base.renderMsg.fadeLength;
}

void NC_STACK_base::setBASE_ambientLight(int val)
{
    stack__base.renderMsg.ambientLight = val;
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
    stack__base.renderMsg.fadeLength = len;
    stack__base.renderMsg.fadeStart = stack__base.visLimit - len;
}

void NC_STACK_base::setBASE_static(int stic)
{
    if (stic)
        stack__base.transform.flags |= 2;
    else
        stack__base.transform.flags &= ~2;
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
    if ( (stack__base.transform.flags & 1) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_visLimit()
{
    return stack__base.visLimit;
}

int NC_STACK_base::getBASE_ambientLight()
{
    return stack__base.renderMsg.ambientLight;
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
    return stack__base.transform.locPos.sx;
}

float NC_STACK_base::getBASE_y()
{
    return stack__base.transform.locPos.sy;
}

float NC_STACK_base::getBASE_z()
{
    return stack__base.transform.locPos.sz;
}

float NC_STACK_base::getBASE_vx()
{
    return stack__base.transform.vec.sx;
}

float NC_STACK_base::getBASE_vy()
{
    return stack__base.transform.vec.sy;
}

float NC_STACK_base::getBASE_vz()
{
    return stack__base.transform.vec.sz;
}

int NC_STACK_base::getBASE_ax()
{
    return stack__base.transform.ax >> 16;
}

int NC_STACK_base::getBASE_ay()
{
    return stack__base.transform.ay >> 16;
}

int NC_STACK_base::getBASE_az()
{
    return stack__base.transform.az >> 16;
}

int NC_STACK_base::getBASE_rx()
{
    return stack__base.transform.rx >> 6;
}

int NC_STACK_base::getBASE_ry()
{
    return stack__base.transform.ry >> 6;
}

int NC_STACK_base::getBASE_rz()
{
    return stack__base.transform.rz >> 6;
}

float NC_STACK_base::getBASE_sx()
{
    return stack__base.transform.scale.sx;
}

float NC_STACK_base::getBASE_sy()
{
    return stack__base.transform.scale.sy;
}

float NC_STACK_base::getBASE_sz()
{
    return stack__base.transform.scale.sz;
}

nlist *NC_STACK_base::getBASE_adeList()
{
    return &stack__base.ADES;
}

TForm3D *NC_STACK_base::getBASE_pTransform()
{
    return &stack__base.transform;
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
    return &stack__base.renderMsg;
}

int NC_STACK_base::getBASE_mainKid()
{
    return (stack__base.flags & BASE_FLAG_MAINKID) != 0;
}

int NC_STACK_base::getBASE_mainObjt()
{
    return (stack__base.flags & BASE_FLAG_MAINOBJT) != 0;
}

RenderStack *NC_STACK_base::getBASE_newRenderStack()
{
    return &renderStack;
}

//polys *NC_STACK_base::getBASE_renderStack()
//{
//    return renderStack;
//}
//
//void *NC_STACK_base::getBASE_argStack()
//{
//    return renderArgStack;
//}
//
//void *NC_STACK_base::getBASE_endArgStack()
//{
//    return renderArgStackEND;
//}

int NC_STACK_base::getBASE_fadeLength()
{
    return stack__base.renderMsg.fadeLength;
}

int NC_STACK_base::getBASE_static()
{
    if ( (stack__base.transform.flags & 2) )
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
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
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
        return (size_t)base_func77( (baseRender_msg *)data );
    case 78:
        return (size_t)base_func78( (base_64arg *)data );
    case 79:
        return (size_t)base_func79( (NC_STACK_base **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
