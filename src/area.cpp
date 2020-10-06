#include "includes.h"
#include "nucleas.h"
#include "area.h"
#include "utils.h"
#include "engine_input.h"

#include "display.h"
#include "win3d.h"

#include "skeleton.h"
#include "base.h"

#include "engine_gfx.h"


const Nucleus::ClassDescr NC_STACK_area::description("area.class", &newinstance);

size_t NC_STACK_area::func0(IDVList &stak)
{
    if ( !NC_STACK_ade::func0(stak) )
        return 0;

    stack__area.colorVal = 1;
    stack__area.tracyVal = 0;
    stack__area.shadeVal = 0;
    stack__area.polflags = 0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case ADE_ATT_DPTHFADE:
                setADE_depthFade ( val.Get<int32_t>() );
                break;
            case ADE_ATT_POLY:
                setADE_bkCheck( val.Get<int32_t>() );
                break;
            case AREA_ATT_TEXBITM:
                setAREA_bitm ( val.Get<NC_STACK_bitmap *>() );
                break;
            case AREA_ATT_COLORVAL:
                setAREA_colorVal( val.Get<int32_t>() );
                break;
            case AREA_ATT_MAP:
                setAREA_map( val.Get<int32_t>() );
                break;

            case AREA_ATT_TEX:
                setAREA_tex( val.Get<int32_t>() );
                break;

            case AREA_ATT_SHADE:
                setAREA_shade ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACY:
                setAREA_tracy ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYMODE:
                setAREA_tracymode( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm ( val.Get<NC_STACK_bitmap *>() );
                break;

            case AREA_ATT_SHADEVAL:
                setAREA_shadeVal ( val.Get<int32_t>() );
                break;

            case AREA_ATT_TRACYVAL:
                setAREA_tracyVal ( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_area::func1()
{
    __NC_STACK_area *area = &stack__area;

    if ( area->texImg )
        delete_class_obj(area->texImg);
    if ( area->tracyImg )
        delete_class_obj(area->tracyImg);
    return NC_STACK_ade::func1();
}


int NC_STACK_area::area_func5__sub0(IFFile *mfile)
{
    AREA_STRC tmp;

    mfile->readS16B(tmp.version);
    mfile->readU16B(tmp.flags);
    mfile->readU16B(tmp.polFlags);
    mfile->readU8(tmp._un1);
    mfile->readU8(tmp.clrVal);
    mfile->readU8(tmp.trcVal);
    mfile->readU8(tmp.shdVal);

    if ( tmp.version >= 1 )
    {
        stack__area.polflags = tmp.polFlags;
        stack__area.flags = tmp.flags;
        stack__area.colorVal = tmp.clrVal;
        stack__area.tracyVal = tmp.trcVal;
        stack__area.shadeVal = tmp.shdVal;
    }

    return 1;
}



int NC_STACK_area::area_func5__sub1(IFFile *mfile)
{
    int v8 = stack__area.polflags & AREA_POL_FLAG_TEXUTRED;

    if ( (stack__area.polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
        v8 |= AREA_POL_FLAG_TRACYMAPPED;

    NC_STACK_bitmap *objt = dynamic_cast<NC_STACK_bitmap *>( READ_OBJT(mfile) );
    if ( objt )
    {
        if (v8 == AREA_POL_FLAG_TEXUTRED )
        {
            setAREA_bitm(objt);
        }
        else if ( v8 == AREA_POL_FLAG_TRACYMAPPED )
        {
            setAREA_tracybitm(objt);
        }
        else if ( v8 == (AREA_POL_FLAG_TRACYMAPPED | AREA_POL_FLAG_TEXUTRED) )
        {
            if ( stack__area.texImg )
            {
                setAREA_tracybitm(objt);
            }
            else
            {
                setAREA_bitm(objt);
            }
        }
        return 1;
    }

    return 0;
}


size_t NC_STACK_area::func5(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == -2 )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                func1();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADE )
        {
            obj_ok = NC_STACK_ade::func5(file);

            if ( !obj_ok )
                return 0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            if ( obj_ok && !area_func5__sub0(mfile) )
            {
                func1();
                return 0;
            }
            mfile->parse();
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( obj_ok && !area_func5__sub1(mfile) )
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

size_t NC_STACK_area::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_area *area = &stack__area;

    if ( mfile->pushChunk(TAG_AREA, TAG_FORM, -1) )
        return 0;


    if ( !NC_STACK_ade::func6(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1); // version
    mfile->writeU16B(area->flags);
    mfile->writeU16B(area->polflags);
    mfile->writeU8(0);
    mfile->writeU8(area->colorVal);
    mfile->writeU8(area->tracyVal);
    mfile->writeU8(area->shadeVal);

    mfile->popChunk();

    if ( (area->polflags & AREA_POL_FLAG_TEXUTRED) )
    {
        if ( !area->texImg )
            return 0;
        if ( !sub_4117F8(area->texImg, mfile) )
            return 0;
    }
    if ( (area->polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
    {
        if ( !area->tracyImg )
            return 0;
        if ( !sub_4117F8(area->tracyImg, mfile) )
            return 0;
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

// Add area to list
size_t NC_STACK_area::ade_func65(area_arg_65 *arg)
{
    __NC_STACK_area *area = &stack__area;
//    polysDatSub *datSub = &arg->argSTK_cur->datSub;
    polysDat *data = arg->rndrStack->get();
    polysDatSub *datSub = &data->datSub;

    int renderFlags = area->polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

    if (renderFlags == 0)
        renderFlags = 0;
    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_LINMAP;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_PERSPMAP;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_GRADSHD;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_PERSPMAP | NC_STACK_display::RFLAGS_GRADSHD;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_PERSPMAP | NC_STACK_display::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_GRADSHD | NC_STACK_display::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_PERSPMAP | NC_STACK_display::RFLAGS_GRADSHD | NC_STACK_display::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_FLATTRACY) )
        renderFlags = NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_LUMTRACY;
    else
        return 1;

    datSub->renderFlags = renderFlags;
    datSub->r = 1.0;
    datSub->g = 1.0;
    datSub->b = 1.0;

    skeleton_arg133 skel133;

    skel133.polyID = area->polnum;
    skel133.field_4 = 0;

    if ( datSub->renderFlags & (NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( datSub->renderFlags & (NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( area->flags & AREA_FLAG_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.rndrArg = datSub;
    skel133.minZ = arg->minZ;
    skel133.maxZ = arg->maxZ;
    skel133.shadeVal = area->shadeVal / 256.0;
    skel133.fadeStart = arg->fadeStart;
    skel133.fadeLength = arg->fadeLength;

    if ( area->texImg )
    {
        bitmap_arg130 bitm130;

        bitm130.time_stmp = arg->timeStamp;
        bitm130.frame_time = arg->frameTime;

        area->texImg->bitmap_func130(&bitm130);

        datSub->pbitm = bitm130.pbitm;
        skel133.texCoords = bitm130.outline;
    }
    else
    {
        datSub->pbitm = NULL;
        skel133.texCoords = NULL;
    }

    if ( arg->OBJ_SKELETON->skeleton_func133(&skel133) )
    {
        arg->adeCount++;

        if ( datSub->renderFlags & ( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD ) )
        {
            int v6 = 0;
            int v8 = 0;

            for (int i = 0; i < datSub->vertexCount; i++)
            {
                float clr = datSub->color[i];
                if (clr < 0.01)
                    v6++;
                else if (clr > 0.99)
                    v8++;
            }

            if ( v6 == datSub->vertexCount )
            {
                datSub->renderFlags &= ~( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD );
            }
            else if ( v8 == datSub->vertexCount )
            {
                datSub->renderFlags = 0;
            }
        }

        datSub->renderFlags |= (arg->flags & NC_STACK_display::RFLAGS_SKY);

        float maxz = 0.0;

        for (int i = 0; i < datSub->vertexCount; i++)
            if (datSub->vertexes[i].z > maxz)
                maxz = datSub->vertexes[i].z;

        if ( !(arg->flags & NC_STACK_display::RFLAGS_IGNORE_FALLOFF) && NC_STACK_win3d::win3d_keys[18].Get<bool>() )
        {
            float maxln = 0.0;

            for (int i = 0; i < datSub->vertexCount; i++)
            {
                datSub->distance[i] = datSub->vertexes[i].XZ().length();
                if (datSub->distance[i] > maxln)
                    maxln = datSub->distance[i];
            }

            if (maxln > NC_STACK_win3d::win3d_keys[19].Get<int>())
                datSub->renderFlags |= NC_STACK_display::RFLAGS_FALLOFF;
        }

        data->render_func = GFXEngine::defRenderFunc;
        data->range = maxz;

        arg->rndrStack->commit();
    }
    return 1;
}

void NC_STACK_area::setADE_depthFade(int mode)
{
    if ( mode )
        stack__area.flags |= AREA_FLAG_DPTHFADE;
    else
        stack__area.flags &= ~AREA_FLAG_DPTHFADE;

    NC_STACK_ade::setADE_depthFade(mode);
}

void NC_STACK_area::setADE_poly(int num)
{
    stack__area.polnum = num;

    NC_STACK_ade::setADE_poly(num);
}

void NC_STACK_area::setAREA_bitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( stack__area.texImg != NULL )
            delete_class_obj(stack__area.texImg);

        stack__area.texImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_colorVal(int val)
{
    stack__area.colorVal = val;
}

void NC_STACK_area::setAREA_map(int mode)
{
    stack__area.polflags &= ~(AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

    if ( mode == 1 )
        stack__area.polflags |= AREA_POL_FLAG_LINEARMAPPED;
    else if ( mode == 2 )
        stack__area.polflags |= AREA_POL_FLAG_DEPTHMAPPED;
}

void NC_STACK_area::setAREA_tex(int mode)
{
    stack__area.polflags &= ~(AREA_POL_FLAG_TEXBIT);

    if ( mode == 1 )
        stack__area.polflags |= AREA_POL_FLAG_TEXUTRED;
}

void NC_STACK_area::setAREA_shade(int mode)
{
    stack__area.polflags &= ~(AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

    if ( mode == 1 )
        stack__area.polflags |= AREA_POL_FLAG_FLATSHADE;
    else if ( mode == 2 )
        stack__area.polflags |= AREA_POL_FLAG_LINESHADE;
    else if ( mode == 3 )
        stack__area.polflags |= AREA_POL_FLAG_GRADIENTSHADE;
}

void NC_STACK_area::setAREA_tracy(int mode)
{
    stack__area.polflags &= ~(AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

    if ( mode == 1 )
        stack__area.polflags |= AREA_POL_FLAG_CLEARTRACY;
    else if ( mode == 2 )
        stack__area.polflags |= AREA_POL_FLAG_FLATTRACY;
    else if ( mode == 3 )
        stack__area.polflags |= AREA_POL_FLAG_TRACYMAPPED;
}

void NC_STACK_area::setAREA_tracymode(int mode)
{
    stack__area.polflags &= ~(AREA_POL_FLAG_TRACYBIT3);

    if ( mode == 1 )
        stack__area.polflags |= AREA_POL_FLAG_LIGHT;
}

void NC_STACK_area::setAREA_tracybitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( stack__area.tracyImg != NULL )
            delete_class_obj(stack__area.tracyImg);

        stack__area.tracyImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_shadeVal(int val)
{
    stack__area.shadeVal = val;
}

void NC_STACK_area::setAREA_tracyVal(int val)
{
    stack__area.tracyVal = val;
}

void NC_STACK_area::setAREA_blob1(uint32_t val)
{
    stack__area.polflags = val & 0xFFFF;
    stack__area.flags = val >> 16;
}

void NC_STACK_area::setAREA_blob2(uint32_t val)
{
    stack__area.shadeVal = val & 0xFF;
    stack__area.colorVal = (val >> 16) & 0xFF;
    stack__area.tracyVal = (val >> 8) & 0xFF;
}


NC_STACK_bitmap *NC_STACK_area::getAREA_bitm()
{
    return stack__area.texImg;
}

int NC_STACK_area::getAREA_colorVal()
{
    return stack__area.colorVal;
}

int NC_STACK_area::getAREA_map()
{
    int v9 = stack__area.polflags & (AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

    if ( v9 == AREA_POL_FLAG_NONMAPPED )
        return 0;
    else if ( v9 == AREA_POL_FLAG_LINEARMAPPED )
        return 1;
    else if ( v9 == AREA_POL_FLAG_DEPTHMAPPED )
        return 2;

    return 0;
}

int NC_STACK_area::getAREA_tex()
{
    int v6 = stack__area.polflags & AREA_POL_FLAG_TEXBIT;

    if ( v6 == AREA_POL_FLAG_TEXUTRED )
        return 1;

    return 0;
}

int NC_STACK_area::getAREA_shade()
{
    int v4 = stack__area.polflags & (AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

    if ( v4 == AREA_POL_FLAG_NOSHADE )
        return 0;
    else if ( v4 == AREA_POL_FLAG_FLATSHADE )
        return 1;
    else if ( v4 == AREA_POL_FLAG_LINESHADE )
        return 2;
    else if ( v4 == AREA_POL_FLAG_GRADIENTSHADE )
        return 3;
    return 0;
}

int NC_STACK_area::getAREA_tracy()
{
    int v5 = stack__area.polflags & (AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

    if ( v5 == AREA_POL_FLAG_NOTRACY )
        return 0;
    else if ( v5 == AREA_POL_FLAG_CLEARTRACY )
        return 1;
    else if ( v5 == AREA_POL_FLAG_FLATTRACY )
        return 2;
    else if ( v5 == AREA_POL_FLAG_TRACYMAPPED )
        return 3;
    return 0;
}

int NC_STACK_area::getAREA_tracymode()
{
    int v10 = stack__area.polflags & AREA_POL_FLAG_TRACYBIT3;

    if ( v10 == AREA_POL_FLAG_DARK )
        return 0;
    else if ( v10 == AREA_POL_FLAG_LIGHT )
        return 1;

    return 0;
}

NC_STACK_bitmap *NC_STACK_area::getAREA_tracybitm()
{
    return stack__area.tracyImg;
}

int NC_STACK_area::getAREA_shadeVal()
{
    return stack__area.shadeVal;
}

int NC_STACK_area::getAREA_tracyVal()
{
    return stack__area.tracyVal;
}
