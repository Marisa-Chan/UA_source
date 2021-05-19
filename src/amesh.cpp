#include "includes.h"
#include "inttypes.h"
#include "nucleas.h"
#include "amesh.h"
#include "utils.h"
#include "system/inpt.h"

#include "skeleton.h"
#include "base.h"

#include "system/gfx.h"
#include "system/inivals.h"


const Nucleus::ClassDescr NC_STACK_amesh::description("amesh.class", &newinstance);

size_t NC_STACK_amesh::Init(IDVList &stak)
{
    if ( !NC_STACK_area::Init(stak) )
        return 0;

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
            case AREA_ATT_TEXBITM:
                setAREA_bitm(val.Get<NC_STACK_bitmap *>());
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm(val.Get<NC_STACK_bitmap *>());
                break;

            case AMESH_ATT_NUMPOLY:
                setAMESH_numpoly(val.Get<int32_t>());
                break;

            case AMESH_ATT_ATTPOLYS:
                if ( !setAMESH_polys( val.Get<ATTS *>() ) )
                {
                    Deinit();
                    return 0;
                }
                break;

            case AMESH_ATT_OTLPOOL:
                if ( !setAMESH_otls(val.Get<tUtV **>()) )
                {
                    Deinit();
                    return 0;
                }
                break;

            default:
                break;
            }
        }
    }


    return 1;
}


size_t NC_STACK_amesh::LoadingFromIFF(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;

    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == IFFile::IFF_ERR_EOC )
            break;

        if ( iff_res )
        {
            if ( obj_ok )
                Deinit();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_AREA )
        {
            obj_ok = NC_STACK_area::LoadingFromIFF(file);

            if ( !obj_ok )
                return 0;
        }
        else if ( chunk->TAG == TAG_ATTS )
        {
            if ( obj_ok )
            {
                polyCnt = chunk->TAG_SIZE / 6;
                
                atts.resize(polyCnt);
                
                for (int i = 0; i < polyCnt; i++)
                {
                    ATTS &att = atts.at(i);
                    mfile->readS16B(att.polyID);
                    mfile->readU8(att.colorVal);
                    mfile->readU8(att.shadeVal);
                    mfile->readU8(att.tracyVal);
                    mfile->readU8(att.pad);
                }
            }
            mfile->parse();
        }
        else if ( chunk->TAG == TAG_OLPL )
        {
            if ( obj_ok )
            {
                texCoords.resize(polyCnt);
                texCoordsData.resize( chunk->TAG_SIZE / 2 );

                tUtV *uv = texCoordsData.data();

                for (int i = 0; i < polyCnt; i++)
                {
                    texCoords[i] = uv;

                    int16_t cnt;
                    mfile->readS16B(cnt);

                    for (int j = 0; j < cnt; j++)
                    {
                        uint8_t x; //If you cross refernce the VBMP these are the X,Y coordinates
                        uint8_t y; // as it is stored on disk
                        mfile->readU8(x);
                        mfile->readU8(y);

                        uv->tu = x / 256.0;
                        uv->tv = y / 256.0;
                        uv++;
                    }

                    uv->tu = -1;
                    uv->tv = -1;
                    uv++;
                }
            }
            mfile->parse();
        }
        else
        {
            mfile->skipChunk();
        }
    }

    return obj_ok;
}

size_t NC_STACK_amesh::SaveIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_AMSH, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_area::SaveIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_ATTS, -1);

    for (int i = 0; i < polyCnt; i++)
    {
        ATTS &att = atts.at(i);
        mfile->writeS16B(att.polyID);
        mfile->writeU8(att.colorVal);
        mfile->writeU8(att.shadeVal);
        mfile->writeU8(att.tracyVal);
        mfile->writeU8(att.pad);
    }

    mfile->popChunk();

    if ( !texCoords.empty() )
    {
        mfile->pushChunk(0, TAG_OLPL, -1);

        for (int i = 0; i < polyCnt; i++)
        {

            tUtV *uv = texCoords[i];
            int16_t cnt = 0;
            while (uv->tu >= 0.0)
            {
                cnt++;
                uv++;
            }

            mfile->writeS16B(cnt);

            uv = texCoords[i];

            for (int j = 0; j < cnt; j++)
            {
                uint8_t x;
                uint8_t y;
                x = uv[j].tu * 256.0;
                y = uv[j].tv * 256.0;

                mfile->writeU8(x);
                mfile->writeU8(y);
            }
        }
        mfile->popChunk();
    }
    mfile->popChunk();
    return 1;
}

// Add amesh to list
size_t NC_STACK_amesh::ade_func65(area_arg_65 *arg, InstanceOpts * opts /* = NULL */)
{
    //v5 = *(_uint16_t *)(amesh->field_14 + 6) & 0xFEF6;
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


    skeleton_arg133 skel133;

    skel133.field_4 = 0;

    if ( renderFlags & (GFX::RFLAGS_LINMAP | GFX::RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( renderFlags & (GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( flags & ADE_ATT_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.minZ = arg->minZ;
    skel133.maxZ = arg->maxZ;
    skel133.fadeStart = arg->fadeStart;
    skel133.fadeLength = arg->fadeLength;


    ResBitmap *v21;

    if ( _texImg )
    {
        bitmap_arg130 bitm130;

        bitm130.time_stmp = arg->timeStamp;
        bitm130.frame_time = arg->frameTime;
        _texImg->bitmap_func130(&bitm130);
        v21 = bitm130.pbitm;
    }
    else
    {
        v21 = NULL;
    }

    for (int i = 0; i < polyCnt; i++)
    {
        polysDat *data = arg->rndrStack->get();
        polysDatSub *datSub = &data->datSub;

        datSub->renderFlags = renderFlags;
        datSub->r = 1.0;
        datSub->g = 1.0;
        datSub->b = 1.0;

        datSub->pbitm = v21;

        skel133.polyID = atts.at(i).polyID;

        skel133.rndrArg = datSub;

        skel133.shadeVal = atts.at(i).shadeVal / 256.0;

        if ( !texCoords.empty() )
            skel133.texCoords = texCoords[i];
        else
            skel133.texCoords = NULL;

        if (arg->OBJ_SKELETON->skeleton_func133(&skel133))
        {
            arg->adeCount++;

            if ( datSub->renderFlags & ( GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD ) )
            {
                int v6 = 0;
                int v8 = 0;

                for (int j = 0; j < datSub->vertexCount; j++)
                {
                    float clr = datSub->color[j];
                    if (clr < 0.01)
                        v6++;
                    else if (clr > 0.99)
                        v8++;
                }

                if ( v6 == datSub->vertexCount )
                {
                    datSub->renderFlags &= ~( GFX::RFLAGS_FLATSHD | GFX::RFLAGS_GRADSHD );
                }
                else if ( v8 == datSub->vertexCount && !System::IniConf::GfxNewSky.Get<bool>() )
                {
                    datSub->renderFlags = 0;
                }
            }

            datSub->renderFlags |= (arg->flags & GFX::RFLAGS_SKY);

            float maxz = 0.0;

            for (int j = 0; j < datSub->vertexCount; j++)
                if (datSub->vertexes[j].z > maxz)
                    maxz = datSub->vertexes[j].z;

            if ( !(arg->flags & GFX::RFLAGS_IGNORE_FALLOFF) && System::IniConf::GfxNewSky.Get<bool>() )
            {
                float maxln = 0.0;

                for (int j = 0; j < datSub->vertexCount; j++)
                {
                    datSub->distance[j] = datSub->vertexes[j].XZ().length();
                    if (datSub->distance[j] > maxln)
                        maxln = datSub->distance[j];
                }

                if (maxln > System::IniConf::GfxSkyDistance.Get<int>())
                    datSub->renderFlags |= GFX::RFLAGS_FALLOFF;
            }

            data->range = maxz;

            arg->rndrStack->commit();
        }
    }

    return 1;
}

void NC_STACK_amesh::setAMESH_numpoly(int num)
{
    polyCnt = num;
}

int NC_STACK_amesh::setAMESH_polys(ATTS *patts)
{
    atts.resize(polyCnt);

    for (int i = 0; i < polyCnt; i++)
        atts.at(i) = patts[i];

    return 1;
}

int NC_STACK_amesh::setAMESH_otls(tUtV **uv)
{
    texCoords.clear();
    texCoordsData.clear();

    int olpl_cnt = 0;

    for (int i = 0; i < polyCnt; i++)
    {
        tUtV * v6 = uv[i];
        while( v6->tu >= 0.0)
        {
            olpl_cnt++;
            v6++;
        }
        olpl_cnt++;
    }

    texCoords.resize(polyCnt);
    texCoordsData.resize( olpl_cnt );

    tUtV *uvv = texCoordsData.data();
    //void *ed = ((char *)amesh->olpl + sizeof(tUtV *) * amesh->cnt + sizeof(tUtV) * olpl_cnt);

    for (int i = 0; i < polyCnt; i++)
    {
        tUtV * inCoord = uv[i];
        texCoords[i] = uvv;
        while( 1 )
        {
            *uvv = *inCoord;
            inCoord++;
            uvv++;

            if (inCoord->tu < 0.0)
                break;
        }
        *uvv = *inCoord;
        uvv++;
    }
    return 1;
}


int NC_STACK_amesh::getAMESH_numpoly()
{
    return polyCnt;
}