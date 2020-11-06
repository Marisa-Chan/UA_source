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

const Nucleus::ClassDescr NC_STACK_base::description("base.class", &newinstance);
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




int baseIDcounter = 1;




size_t NC_STACK_base::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    ID = baseIDcounter;
    baseIDcounter++;

    ADES.clear();
    KIDS.clear();

    transform.scale = vec3d(1.0, 1.0, 1.0);

    parentList = NULL;

    transform.MakeScaleRotationMatrix();

    visLimit = 4096;

    renderMsg.fadeStart = 3496.0;
    renderMsg.fadeLength = 600.0;
    renderMsg.ambientLight = 255;

    flags |= FLAG_RENDERALL;
    transform.flags |= TFEngine::TForm3D::FLAG_FOLLOW_PARENT;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case ATT_SKELET:
                setBASE_skeleton(val.Get<NC_STACK_skeleton *>());
                break;

            case ATT_ADE:
                setBASE_ADE(val.Get<NC_STACK_ade *>());
                break;

            case ATT_PARENTFOLLOW:
                setBASE_parentFollow(val.Get<int32_t>());
                break;

            case ATT_VISLIMIT:
                setBASE_visLimit(val.Get<int32_t>());
                break;

            case ATT_AMBIENTLIGHT:
                setBASE_ambientLight(val.Get<int32_t>());
                break;

            case ATT_RENDERALL:
                setBASE_renderAll(val.Get<int32_t>());
                break;

            case ATT_INPUTHANDLE:
                setBASE_inputHandle (val.Get<int32_t>());
                break;

            case ATT_FADELEN:
                setBASE_fadeLength(val.Get<int32_t>());
                break;

            case ATT_STATIC:
                setBASE_static (val.Get<int32_t>());
                break;

            case ATT_EMBDRSRC:
                setBASE_embdRsrc(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_base::func1()
{
    if (OBJ_SKELETON)
        delete_class_obj(OBJ_SKELETON);

    for (AdeList::iterator it = ADES.begin(); it != ADES.end(); it = ADES.erase(it))
    {
        (*it)->AttachedTo = NULL; //Clear ade ield, because we do erase in this list
        delete_class_obj(*it);
    }

    if ( parent )
        parent->KIDS.remove(this);

    for (BaseList::iterator it = KIDS.begin(); it != KIDS.end(); it = KIDS.erase(it))
    {
        (*it)->parent = NULL; //Clear kid parent field, because we do erase in this list
        delete_class_obj(*it);
    }


    if ( OBJT )
        delete_class_obj(OBJT);

    return NC_STACK_nucleus::func1();
}

int NC_STACK_base::READ_STRC(IFFile *mfile)
{
    bool readOK = true;

    STRC_base dst;

    readOK &= mfile->readS16B(dst.version);

    readOK &= mfile->readFloatB(dst.pos.x);
    readOK &= mfile->readFloatB(dst.pos.y);
    readOK &= mfile->readFloatB(dst.pos.z);

    readOK &= mfile->readFloatB(dst.vec.x);
    readOK &= mfile->readFloatB(dst.vec.y);
    readOK &= mfile->readFloatB(dst.vec.z);

    readOK &= mfile->readFloatB(dst.scale.x);
    readOK &= mfile->readFloatB(dst.scale.y);
    readOK &= mfile->readFloatB(dst.scale.z);

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
            base_func67(zzz);

        flag_xyz v38;

        v38.flag = 7;
        v38.v = dst.pos;

        base_func68(&v38);

        v38.v = dst.scale;

        base_func72(&v38); // SET_SCALE ?

        if ( dst.attFlags & 1 )
        {
            v38.v = dst.vec;
        }
        else
        {
            v38.flag = 8;
        }

        base_func69(&v38);

        flag_xyz2 v39;

        v39.flag = 7;

        v39.x = dst.ax;
        v39.y = dst.ay;
        v39.z = dst.az;

        base_func70(&v39);

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

        base_func71(&v39);

        setBASE_renderAll( (dst.attFlags & 8) != 0 );

//				stk[1].id = 0x80001007;
//				stk[1].value = (dst.p17 & 0x10) != 0;

        setBASE_inputHandle( (dst.attFlags & 0x20) != 0 );
        setBASE_parentFollow( (dst.attFlags & 0x40) != 0 );

//				stk[4].id = 0x80001003;
//				stk[4].value = dst.p18;

        setBASE_visLimit( dst.visLimit );
        setBASE_ambientLight( dst.ambientLight );

    }
    return 1;
}

int NC_STACK_base::READ_ADES(IFFile *mfile)
{
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

            setBASE_ADE(ade);
        }
        else
        {
            mfile->skipChunk();
        }
    }
    return 1;
}

int NC_STACK_base::READ_KIDS(IFFile *mfile)
{
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

            base_func65(objt);
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
    int STRC_readed = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == IFFile::IFF_ERR_EOC )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                func1();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();



        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj_ok = NC_STACK_nucleus::func5(file);

            if ( !obj_ok )
                return 0;

            ADES.clear();
            KIDS.clear();

            transform.scale = vec3d(1.0, 1.0, 1.0);

            ID = baseIDcounter;

            baseIDcounter++;

//            kid_node.self_full = this;

            transform.MakeScaleRotationMatrix();

            visLimit = 4096;
            renderMsg.fadeStart = 3496.0;
            renderMsg.fadeLength = 600.0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            STRC_readed = 1;

            if ( !READ_STRC(mfile) )
            {
                func1();
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
                        func1();
                        return 0;
                    }
                    setBASE_skeleton(skel);
                }
                else
                {
                    OBJT = READ_OBJT(mfile);
                }
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADES )
        {
            if ( !READ_ADES(mfile) )
            {
                func1();
                return 0;
            }
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_KIDS )
        {
            if ( !READ_KIDS(mfile) )
            {
                func1();
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

    if ( mfile->pushChunk(TAG_BASE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    if ( flags & FLAG_EMBDRSRC )
    {
        NC_STACK_embed *embd = Nucleus::CInit<NC_STACK_embed>();
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
    mfile->writeFloatB(transform.locPos.x);
    mfile->writeFloatB(transform.locPos.y);
    mfile->writeFloatB(transform.locPos.z);

    mfile->writeFloatB(transform.vec.x);
    mfile->writeFloatB(transform.vec.y);
    mfile->writeFloatB(transform.vec.z);

    mfile->writeFloatB(transform.scale.x);
    mfile->writeFloatB(transform.scale.y);
    mfile->writeFloatB(transform.scale.z);

    mfile->writeS16B(transform.ax >> 16);
    mfile->writeS16B(transform.ay >> 16);
    mfile->writeS16B(transform.az >> 16);

    mfile->writeS16B(transform.rx >> 6);
    mfile->writeS16B(transform.ry >> 6);
    mfile->writeS16B(transform.rz >> 6);

    int16_t attFlags = 0;
    if ( flags & FLAG_MOVING )
        attFlags = 1;

    if ( flags & FLAG_ROTATING )
        attFlags |= 2;

    if ( flags & FLAG_MAINOBJT )
        attFlags |= 4;

    if ( flags & FLAG_RENDERALL )
        attFlags |= 8;

    if ( flags & FLAG_TERMCOLL )
        attFlags |= 0x10;

    if ( flags & FLAG_INPUTHANDLE )
        attFlags |= 0x20;

    if ( transform.flags & TFEngine::TForm3D::FLAG_FOLLOW_PARENT )
        attFlags |= 0x40;

    mfile->writeS16B(attFlags);

    mfile->writeS16B(0); // _un1

    mfile->writeS32B(renderMsg.fadeStart + renderMsg.fadeLength); // visLimit
    mfile->writeS32B(renderMsg.ambientLight); // ambientLight

    mfile->popChunk();

    if ( OBJ_SKELETON )
    {
        if ( !sub_4117F8(OBJ_SKELETON, mfile) )
            return 0;

        if ( !ADES.empty() )
        {
            mfile->pushChunk(TAG_ADES, TAG_FORM, -1);
            for (AdeList::iterator it = ADES.begin(); it != ADES.end(); it++)
            {
                if ( !sub_4117F8(*it, mfile) )
                    return 0;
            }
            mfile->popChunk();
        }
    }

    if ( !KIDS.empty() )
    {
        mfile->pushChunk(TAG_KIDS, TAG_FORM, -1);

        for (BaseList::iterator it = KIDS.begin(); it != KIDS.end(); it++)
        {
            if ( !sub_4117F8(*it, mfile) )
                return 0;
        }
        mfile->popChunk();
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

size_t NC_STACK_base::base_func64(base_64arg *arg)
{
    TFEngine::TForm3D *glob_1c = TFEngine::Engine.GetViewPoint();

    if ( glob_1c )
        glob_1c->CalcGlobal();

    base_func73(arg);

    baseRender_msg base77;
    base77.frameTime = arg->DTime;
    base77.globTime = arg->TimeStamp;
    base77.rndrStack = &renderStack;
    base77.adeCount = arg->field_C;
    base77.ownerID = ID;
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

// Push parent info to kid
size_t NC_STACK_base::base_func65(NC_STACK_base *kid)
{
    if ( OBJ_SKELETON )
        kid->base_func66(this, &transform);
    else
        kid->base_func66(this, NULL);
    return 1;
}

// Add object to parent kids list
size_t NC_STACK_base::base_func66(NC_STACK_base *_parent, TFEngine::TForm3D *_tform)
{
    if ( parent )
    {
        parent->KIDS.remove(this);

        if ( flags & FLAG_MAINKID )
        {
            NC_STACK_base *v7[2];
            v7[0] = (NC_STACK_base *)-1;
            v7[1] = NULL;
            parent->base_func67(v7);
        }
    }

    parent = _parent;
    if ( parent )
    {
        parent->KIDS.push_back(this);

        if ( flags & FLAG_MAINKID )
        {
            NC_STACK_base *v7[2];
            v7[0] = this;
            v7[1] = this;

            parent->base_func67(v7);
        }
    }

    transform.parent_1c = _tform;

    return 1;
}

size_t NC_STACK_base::base_func67(NC_STACK_base **arg)
{
    if (arg[0] == NULL)
    {
//        mainChild = NULL;
        flags |= (FLAG_MAINOBJT | FLAG_MAINKID);
        mainObject = this;

        if ( parent )
        {
            arg[0] = this;
            arg[1] = this;

            parent->base_func67(arg);
        }
        TFEngine::Engine.SetViewPoint(&transform);
    }
    else if ((int)(size_t)arg[0] == -1)
    {
        mainObject = 0;
//        mainChild = NULL;
        flags &= ~(FLAG_MAINOBJT | FLAG_MAINKID);

        if ( !parent )
            return 1;

        parent->base_func67(arg);
    }
    else if (arg[0])
    {
//        mainChild = arg[0];
        mainObject = arg[1];

        flags |= FLAG_MAINKID;
        flags &= ~FLAG_MAINOBJT;

        if ( !parent )
            return 1;

        arg[0] = this;

        parent->base_func67(arg);
    }

    return 1;
}

size_t NC_STACK_base::base_func68(flag_xyz *arg)
{
    int flg = arg->flag;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            transform.locPos.x += arg->v.x;
        if ( flg & 2 )
            transform.locPos.y += arg->v.y;
        if ( flg & 4 )
            transform.locPos.z += arg->v.z;
    }
    else
    {
        if ( flg & 1 )
            transform.locPos.x = arg->v.x;
        if ( flg & 2 )
            transform.locPos.y = arg->v.y;
        if ( flg & 4 )
            transform.locPos.z = arg->v.z;
    }
    return 1;
}

size_t NC_STACK_base::base_func69(flag_xyz *arg)
{
    int flg = arg->flag;

    if ( arg->flag & 8 )
    {
        flags &= ~FLAG_MOVING;
    }
    else
    {
        flags |= FLAG_MOVING;

        if ( flg & 0x10 )
        {
            if ( flg & 1 )
                transform.vec.x += arg->v.x;
            if ( flg & 2 )
                transform.vec.y += arg->v.y;
            if ( flg & 4 )
                transform.vec.z += arg->v.z;
        }
        else
        {
            if ( flg & 1 )
                transform.vec.x = arg->v.x;
            if ( flg & 2 )
                transform.vec.y = arg->v.y;
            if ( flg & 4 )
                transform.vec.z = arg->v.z;
        }
    }

    return 1;
}

size_t NC_STACK_base::base_func70(flag_xyz2 *arg)
{
    int flg = arg->flag;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            transform.ax = (transform.ax + (arg->x << 16)) % (360 << 16);
        if ( flg & 2 )
            transform.ay = (transform.ay + (arg->y << 16)) % (360 << 16);
        if ( flg & 4 )
            transform.az = (transform.az + (arg->z << 16)) % (360 << 16);
    }
    else
    {
        if ( flg & 1 )
            transform.ax = arg->x << 16;
        if ( flg & 2 )
            transform.ay = arg->y << 16;
        if ( flg & 4 )
            transform.az = arg->z << 16;
    }

    transform.MakeScaleRotationMatrix();

    return 1;
}

size_t NC_STACK_base::base_func71(flag_xyz2 *arg)
{
    int flg = arg->flag;

    if ( arg->flag & 8 )
    {
        flags &= ~FLAG_ROTATING;
    }
    else
    {
        flags |= FLAG_ROTATING;

        if ( flg & 0x10 )
        {
            if ( flg & 1 )
                transform.rx = (transform.rx + (arg->x << 6)) % (360 << 16);
            if ( flg & 2 )
                transform.ry = (transform.ry + (arg->y << 6)) % (360 << 16);
            if ( flg & 4 )
                transform.rz = (transform.rz + (arg->z << 6)) % (360 << 16);
        }
        else
        {
            if ( flg & 1 )
                transform.rx = arg->x << 6;
            if ( flg & 2 )
                transform.ry = arg->y << 6;
            if ( flg & 4 )
                transform.rz = arg->z << 6;
        }
    }

    return 1;
}

size_t NC_STACK_base::base_func72(flag_xyz *arg)
{
    int flg = arg->flag;

    if ( arg->flag & 0x10 )
    {
        if ( flg & 1 )
            transform.scale.x *= arg->v.x;
        if ( flg & 2 )
            transform.scale.y *= arg->v.y;
        if ( flg & 4 )
            transform.scale.z *= arg->v.z;
    }
    else
    {
        if ( flg & 1 )
            transform.scale.x = arg->v.x;
        if ( flg & 2 )
            transform.scale.y = arg->v.y;
        if ( flg & 4 )
            transform.scale.z = arg->v.z;
    }

    transform.MakeScaleRotationMatrix();

    return 1;
}

size_t NC_STACK_base::base_func73(base_64arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func73");

    if ( arg->TimeStamp != timeStamp )
    {
        timeStamp = arg->TimeStamp;

        if ( OBJ_SKELETON )
        {
            if ( flags & FLAG_INPUTHANDLE )
                base_func78(arg);

            if ( flags & FLAG_MOVING )
                transform.locPos += transform.vec * arg->DTime;

            if ( flags & FLAG_ROTATING )
            {
                int a,b,c;
                a = (transform.rx * arg->DTime + transform.ax) % (360 << 16);
                b = (transform.ry * arg->DTime + transform.ay) % (360 << 16);
                c = (transform.rz * arg->DTime + transform.az) % (360 << 16);

                transform.ax = a + (a < 0 ? (360 << 16) : 0);
                transform.ay = b + (b < 0 ? (360 << 16) : 0);
                transform.az = c + (c < 0 ? (360 << 16) : 0);

                transform.MakeScaleRotationMatrix();
            }
        }
    }

    for (BaseList::iterator it = KIDS.begin(); it != KIDS.end(); it++)
        (*it)->base_func73(arg);

    return 1;
}

size_t NC_STACK_base::base_func77(baseRender_msg *arg)
{
    int v12 = 0;

    if ( OBJ_SKELETON )
    {
        if ( !(flags & FLAG_MAINOBJT) )
        {
            transform.CalcGlobal();

            TFEngine::TForm3D *view = TFEngine::Engine.GetViewPoint();

            skeleton_arg_132 skel132;
            skel132.minZ = arg->minZ;
            skel132.maxZ = arg->maxZ;
            skel132.tform = view->globSclRot;

            if ( !(transform.flags & TFEngine::TForm3D::FLAG_NO_WRLD_ROT) )
                skel132.tform *= (transform.tform - view->globPos);
            else
                skel132.tform *= mat4x4(transform.globPos - view->globPos);

            v12 = OBJ_SKELETON->skeleton_func132(&skel132);

            if ( v12 )
            {
                renderMsg.ownerID = arg->ownerID;
                renderMsg.timeStamp = arg->globTime;
                renderMsg.frameTime = arg->frameTime;
                renderMsg.minZ = arg->minZ;
                renderMsg.maxZ = arg->maxZ;
                renderMsg.rndrStack = arg->rndrStack;
                renderMsg.view = view;
                renderMsg.owner = &transform;
                renderMsg.flags = arg->flags;

                renderMsg.OBJ_SKELETON = OBJ_SKELETON;
                renderMsg.adeCount = 0;

                for (AdeList::iterator it = ADES.begin(); it != ADES.end(); it++)
                    (*it)->ade_func65(&renderMsg);

                arg->adeCount += renderMsg.adeCount;
            }
        }
    }

    for (BaseList::iterator it = KIDS.begin(); it != KIDS.end(); it++)
    {
        if ( (*it)->base_func77(arg) )
            v12 = 1;
    }

    return v12;
}

size_t NC_STACK_base::base_func78(base_64arg *arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func78");

    flags |= (FLAG_MOVING | FLAG_ROTATING);

    if ( arg->field_8->Buttons.Is(31) )
    {
        transform.ay = 0;
        transform.az = 0;
        transform.ax = 0;
    }

    transform.rx = -arg->field_8->Sliders[31] * 16384.0;
    transform.ry = -arg->field_8->Sliders[30] * 16384.0;
    transform.rz = -arg->field_8->Sliders[29] * 16384.0;

    transform.vec.x = -arg->field_8->Sliders[28] * transform.locSclRot.m20;
    transform.vec.y = 0;
    transform.vec.z = -arg->field_8->Sliders[28] * transform.locSclRot.m22;

    transform.vec.x += -arg->field_8->Sliders[27] * transform.locSclRot.m20;
    transform.vec.y += -arg->field_8->Sliders[27] * transform.locSclRot.m21;
    transform.vec.z += -arg->field_8->Sliders[27] * transform.locSclRot.m22;

    return 1;
}

size_t NC_STACK_base::base_func79(NC_STACK_base **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","base_func79");

    NC_STACK_base *field_bc = mainObject;

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
        if ( OBJ_SKELETON )
            delete_class_obj(OBJ_SKELETON);

        OBJ_SKELETON = skel;
        renderMsg.sklt = skel->GetSkelet();
    }
}

void NC_STACK_base::setBASE_ADE(NC_STACK_ade *ade)
{
    if (ade)
        ade->ade_func64(ADES);
}

void NC_STACK_base::setBASE_parentFollow(int follow)
{
    if ( follow )
        transform.flags |= TFEngine::TForm3D::FLAG_FOLLOW_PARENT;
    else
        transform.flags &= ~TFEngine::TForm3D::FLAG_FOLLOW_PARENT;
}

void NC_STACK_base::setBASE_visLimit(int val)
{
    visLimit = val;
    renderMsg.fadeStart = val - renderMsg.fadeLength;
}

void NC_STACK_base::setBASE_ambientLight(int val)
{
    renderMsg.ambientLight = val;
}

void NC_STACK_base::setBASE_renderAll(int rndr)
{
    if (rndr)
        flags |= FLAG_RENDERALL;
    else
        flags &= ~FLAG_RENDERALL;
}

void NC_STACK_base::setBASE_inputHandle(int hndl)
{
    if (hndl)
        flags |= FLAG_INPUTHANDLE;
    else
        flags &= ~FLAG_INPUTHANDLE;
}

void NC_STACK_base::setBASE_fadeLength(int len)
{
    renderMsg.fadeLength = len;
    renderMsg.fadeStart = visLimit - len;
}

void NC_STACK_base::setBASE_static(int stic)
{
    if (stic)
        transform.flags |= TFEngine::TForm3D::FLAG_NO_WRLD_ROT;
    else
        transform.flags &= ~TFEngine::TForm3D::FLAG_NO_WRLD_ROT;
}

void NC_STACK_base::setBASE_embdRsrc(int embd)
{
    if (embd)
        flags |= FLAG_EMBDRSRC;
    else
        flags &= ~FLAG_EMBDRSRC;
}

NC_STACK_skeleton *NC_STACK_base::getBASE_skeleton()
{
    return OBJ_SKELETON;
}

int NC_STACK_base::getBASE_parentFollow()
{
    if ( (transform.flags & TFEngine::TForm3D::FLAG_FOLLOW_PARENT) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_visLimit()
{
    return visLimit;
}

int NC_STACK_base::getBASE_ambientLight()
{
    return renderMsg.ambientLight;
}

int NC_STACK_base::getBASE_renderAll()
{
    if ( (flags & FLAG_RENDERALL) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_inputHandle()
{
    if ( (flags & FLAG_INPUTHANDLE) )
        return 1;
    return 0;
}

float NC_STACK_base::getBASE_x()
{
    return transform.locPos.x;
}

float NC_STACK_base::getBASE_y()
{
    return transform.locPos.y;
}

float NC_STACK_base::getBASE_z()
{
    return transform.locPos.z;
}

float NC_STACK_base::getBASE_vx()
{
    return transform.vec.x;
}

float NC_STACK_base::getBASE_vy()
{
    return transform.vec.y;
}

float NC_STACK_base::getBASE_vz()
{
    return transform.vec.z;
}

vec3d NC_STACK_base::getBASE_pos()
{
    return transform.locPos.x;
}

vec3d NC_STACK_base::getBASE_vec()
{
    return transform.vec;
}

int NC_STACK_base::getBASE_ax()
{
    return transform.ax >> 16;
}

int NC_STACK_base::getBASE_ay()
{
    return transform.ay >> 16;
}

int NC_STACK_base::getBASE_az()
{
    return transform.az >> 16;
}

int NC_STACK_base::getBASE_rx()
{
    return transform.rx >> 6;
}

int NC_STACK_base::getBASE_ry()
{
    return transform.ry >> 6;
}

int NC_STACK_base::getBASE_rz()
{
    return transform.rz >> 6;
}

float NC_STACK_base::getBASE_sx()
{
    return transform.scale.x;
}

float NC_STACK_base::getBASE_sy()
{
    return transform.scale.y;
}

float NC_STACK_base::getBASE_sz()
{
    return transform.scale.z;
}

AdeList *NC_STACK_base::getBASE_adeList()
{
    return &ADES;
}

TFEngine::TForm3D *NC_STACK_base::getBASE_pTransform()
{
    return &transform;
}

BaseList &NC_STACK_base::getBASE_kidList()
{
    return KIDS;
}

//base_node *NC_STACK_base::getBASE_kidNode()
//{
//    return &kid_node;
//}

area_arg_65 *NC_STACK_base::getBASE_renderParams()
{
    return &renderMsg;
}

int NC_STACK_base::getBASE_mainKid()
{
    return (flags & FLAG_MAINKID) != 0;
}

int NC_STACK_base::getBASE_mainObjt()
{
    return (flags & FLAG_MAINOBJT) != 0;
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
    return renderMsg.fadeLength;
}

int NC_STACK_base::getBASE_static()
{
    if ( (transform.flags & TFEngine::TForm3D::FLAG_NO_WRLD_ROT) )
        return 1;
    return 0;
}

int NC_STACK_base::getBASE_embdRsrc()
{
    if ( (flags & FLAG_EMBDRSRC) )
        return 1;
    return 0;
}



NC_STACK_base *NC_STACK_base::READ_BAS_FILE(const std::string &fname)
{
    NC_STACK_base *result = NULL;

    FSMgr::FileHandle *fil = uaOpenFile(fname, "rb");
    if ( !fil )
        return NULL;

    IFFile *mfile = new IFFile(fil, false, true);
    if ( !mfile )
    {
        delete fil;
        return NULL;
    }

    if ( !mfile->parse() )
    {
        IFFile::Context *chunk = mfile->getCurrentChunk();
        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_MC2 && !mfile->parse() )
        {
            chunk = mfile->getCurrentChunk();
            if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
                result = dynamic_cast<NC_STACK_base *>(READ_OBJT(mfile));
        }
    }

    delete mfile;

    return result;
}
