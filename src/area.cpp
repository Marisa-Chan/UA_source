#include "includes.h"
#include "nucleas.h"
#include "area.h"
#include "utils.h"
#include "system/inpt.h"

#include "skeleton.h"
#include "base.h"

#include "system/gfx.h"
#include "system/inivals.h"


const Nucleus::ClassDescr NC_STACK_area::description("area.class", &newinstance);

size_t NC_STACK_area::Init(IDVList &stak)
{
    if ( !NC_STACK_ade::Init(stak) )
        return 0;

    _colorVal = 1;
    _tracyVal = 0;
    _shadeVal = 0;
    _polflags = 0;

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

size_t NC_STACK_area::Deinit()
{
    if ( _texImg )
        Nucleus::Delete(_texImg);
    if ( _tracyImg )
        Nucleus::Delete(_tracyImg);
    return NC_STACK_ade::Deinit();
}


int NC_STACK_area::area_func5__sub0(IFFile *mfile)
{
    struct AREA_STRC
    {
        int16_t version;
        uint16_t flags;
        uint16_t polFlags;
        uint8_t _un1;
        uint8_t clrVal;
        uint8_t trcVal;
        uint8_t shdVal;
    } tmp;

    mfile->readS16B(tmp.version);
    mfile->readU16B(tmp.flags);
    mfile->readU16B(tmp.polFlags);
    mfile->readU8(tmp._un1);
    mfile->readU8(tmp.clrVal);
    mfile->readU8(tmp.trcVal);
    mfile->readU8(tmp.shdVal);

    if ( tmp.version >= 1 )
    {
        _polflags = tmp.polFlags;
        _flags = tmp.flags;
        _colorVal = tmp.clrVal;
        _tracyVal = tmp.trcVal;
        _shadeVal = tmp.shdVal;
    }

    return 1;
}



int NC_STACK_area::area_func5__sub1(IFFile *mfile)
{
    int v8 = _polflags & AREA_POL_FLAG_TEXUTRED;

    if ( (_polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
        v8 |= AREA_POL_FLAG_TRACYMAPPED;

    NC_STACK_bitmap *objt = dynamic_cast<NC_STACK_bitmap *>( LoadObjectFromIFF(mfile) );
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
            if ( _texImg )
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


size_t NC_STACK_area::LoadingFromIFF(IFFile **file)
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
                Deinit();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ADE )
        {
            obj_ok = NC_STACK_ade::LoadingFromIFF(file);

            if ( !obj_ok )
                return 0;
        }
        else if ( chunk->TAG == TAG_STRC )
        {
            if ( obj_ok && !area_func5__sub0(mfile) )
            {
                Deinit();
                return 0;
            }
            mfile->parse();
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( obj_ok && !area_func5__sub1(mfile) )
            {
                Deinit();
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

size_t NC_STACK_area::SaveIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_AREA, TAG_FORM, -1) )
        return 0;


    if ( !NC_STACK_ade::SavingIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1); // version
    mfile->writeU16B(_flags);
    mfile->writeU16B(_polflags);
    mfile->writeU8(0);
    mfile->writeU8(_colorVal);
    mfile->writeU8(_tracyVal);
    mfile->writeU8(_shadeVal);

    mfile->popChunk();

    if ( (_polflags & AREA_POL_FLAG_TEXUTRED) )
    {
        if ( !_texImg )
            return 0;
        if ( !_texImg->SaveObjectIntoIFF(mfile) )
            return 0;
    }
    if ( (_polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
    {
        if ( !_tracyImg )
            return 0;
        if ( !_tracyImg->SaveObjectIntoIFF(mfile) )
            return 0;
    }

    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

// Add area to list
size_t NC_STACK_area::ade_func65(area_arg_65 *arg, InstanceOpts * opts /* = NULL */)
{
    polysDat *data = arg->rndrStack->get();
    polysDatSub *datSub = &data->datSub;

    int renderFlags = _polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

    if (renderFlags == 0)
        renderFlags = 0;
    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_LINMAP;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_PERSPMAP;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_LINMAP | GFX::RFLAGS_GRADSHD;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        renderFlags = GFX::RFLAGS_PERSPMAP | GFX::RFLAGS_GRADSHD;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_LINMAP | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_PERSPMAP | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_LINMAP | GFX::RFLAGS_GRADSHD | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        renderFlags = GFX::RFLAGS_PERSPMAP | GFX::RFLAGS_GRADSHD | GFX::RFLAGS_ZEROTRACY;

    else if (renderFlags == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_FLATTRACY) )
        renderFlags = GFX::RFLAGS_LINMAP | GFX::RFLAGS_LUMTRACY;
    else
        return 1;

    datSub->renderFlags = renderFlags;
    datSub->r = 1.0;
    datSub->g = 1.0;
    datSub->b = 1.0;

    skeleton_arg133 skel133;

    skel133.polyID = _polID;
    skel133.field_4 = 0;

    if ( datSub->renderFlags & (GFX::RFLAGS_LINMAP | GFX::RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( datSub->renderFlags & (GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( _flags & AREA_FLAG_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.rndrArg = datSub;
    skel133.minZ = arg->minZ;
    skel133.maxZ = arg->maxZ;
    skel133.shadeVal = _shadeVal / 256.0;
    skel133.fadeStart = arg->fadeStart;
    skel133.fadeLength = arg->fadeLength;

    if ( _texImg )
    {
        bitmap_arg130 bitm130;

        bitm130.time_stmp = arg->timeStamp;
        bitm130.frame_time = arg->frameTime;

        _texImg->bitmap_func130(&bitm130);

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

        if ( datSub->renderFlags & ( GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD ) )
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
                datSub->renderFlags &= ~( GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD );
            }
            else if ( v8 == datSub->vertexCount && !System::IniConf::GfxNewSky.Get<bool>())
            {
                datSub->renderFlags = 0;
            }
        }

        datSub->renderFlags |= (arg->flags & GFX::RFLAGS_SKY);

        float maxz = 0.0;

        for (int i = 0; i < datSub->vertexCount; i++)
            if (datSub->vertexes[i].z > maxz)
                maxz = datSub->vertexes[i].z;

        if ( !(arg->flags & GFX::RFLAGS_IGNORE_FALLOFF) && System::IniConf::GfxNewSky.Get<bool>() )
        {
            float maxln = 0.0;

            for (int i = 0; i < datSub->vertexCount; i++)
            {
                datSub->distance[i] = datSub->vertexes[i].XZ().length();
                if (datSub->distance[i] > maxln)
                    maxln = datSub->distance[i];
            }

            if (maxln > System::IniConf::GfxSkyDistance.Get<int>())
                datSub->renderFlags |= GFX::RFLAGS_FALLOFF;
        }

        data->range = maxz;

        arg->rndrStack->commit();
    }
    return 1;
}

void NC_STACK_area::setADE_depthFade(int mode)
{
    if ( mode )
        _flags |= AREA_FLAG_DPTHFADE;
    else
        _flags &= ~AREA_FLAG_DPTHFADE;

    NC_STACK_ade::setADE_depthFade(mode);
}

void NC_STACK_area::setADE_poly(int num)
{
    _polID = num;

    NC_STACK_ade::setADE_poly(num);
}

void NC_STACK_area::setAREA_bitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( _texImg != NULL )
            Nucleus::Delete(_texImg);

        _texImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_colorVal(int val)
{
    _colorVal = val;
}

void NC_STACK_area::setAREA_map(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_LINEARMAPPED;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_DEPTHMAPPED;
}

void NC_STACK_area::setAREA_tex(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TEXBIT);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_TEXUTRED;
}

void NC_STACK_area::setAREA_shade(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_FLATSHADE;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_LINESHADE;
    else if ( mode == 3 )
        _polflags |= AREA_POL_FLAG_GRADIENTSHADE;
}

void NC_STACK_area::setAREA_tracy(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_CLEARTRACY;
    else if ( mode == 2 )
        _polflags |= AREA_POL_FLAG_FLATTRACY;
    else if ( mode == 3 )
        _polflags |= AREA_POL_FLAG_TRACYMAPPED;
}

void NC_STACK_area::setAREA_tracymode(int mode)
{
    _polflags &= ~(AREA_POL_FLAG_TRACYBIT3);

    if ( mode == 1 )
        _polflags |= AREA_POL_FLAG_LIGHT;
}

void NC_STACK_area::setAREA_tracybitm(NC_STACK_bitmap *bitm)
{
    if ( bitm )
    {
        if ( _tracyImg != NULL )
            Nucleus::Delete(_tracyImg);

        _tracyImg = bitm;
        bitm->PrepareTexture();
    }
}

void NC_STACK_area::setAREA_shadeVal(int val)
{
    _shadeVal = val;
}

void NC_STACK_area::setAREA_tracyVal(int val)
{
    _tracyVal = val;
}

void NC_STACK_area::setAREA_blob1(uint32_t val)
{
    _polflags = val & 0xFFFF;
    _flags = val >> 16;
}

void NC_STACK_area::setAREA_blob2(uint32_t val)
{
    _shadeVal = val & 0xFF;
    _colorVal = (val >> 16) & 0xFF;
    _tracyVal = (val >> 8) & 0xFF;
}


NC_STACK_bitmap *NC_STACK_area::getAREA_bitm()
{
    return _texImg;
}

int NC_STACK_area::getAREA_colorVal()
{
    return _colorVal;
}

int NC_STACK_area::getAREA_map()
{
    int v9 = _polflags & (AREA_POL_FLAG_MAPBIT1 | AREA_POL_FLAG_MAPBIT2);

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
    int v6 = _polflags & AREA_POL_FLAG_TEXBIT;

    if ( v6 == AREA_POL_FLAG_TEXUTRED )
        return 1;

    return 0;
}

int NC_STACK_area::getAREA_shade()
{
    int v4 = _polflags & (AREA_POL_FLAG_SHADEBIT1 | AREA_POL_FLAG_SHADEBIT2);

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
    int v5 = _polflags & (AREA_POL_FLAG_TRACYBIT1 | AREA_POL_FLAG_TRACYBIT2);

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
    int v10 = _polflags & AREA_POL_FLAG_TRACYBIT3;

    if ( v10 == AREA_POL_FLAG_DARK )
        return 0;
    else if ( v10 == AREA_POL_FLAG_LIGHT )
        return 1;

    return 0;
}

NC_STACK_bitmap *NC_STACK_area::getAREA_tracybitm()
{
    return _tracyImg;
}

int NC_STACK_area::getAREA_shadeVal()
{
    return _shadeVal;
}

int NC_STACK_area::getAREA_tracyVal()
{
    return _tracyVal;
}
