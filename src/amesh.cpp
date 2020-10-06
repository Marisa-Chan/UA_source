#include "includes.h"
#include "inttypes.h"
#include "nucleas.h"
#include "amesh.h"
#include "utils.h"
#include "engine_input.h"

#include "display.h"
#include "win3d.h"

#include "skeleton.h"
#include "base.h"

#include "engine_gfx.h"


const Nucleus::ClassDescr NC_STACK_amesh::description("amesh.class", &newinstance);

int sub_419E6C(__NC_STACK_amesh *amesh, tUtV **olpl)
{
    if ( amesh->texCoords )
    {
        delete[] amesh->texCoords;
        amesh->texCoords = NULL;
    }

    if ( amesh->texCoordsData )
    {
        delete[] amesh->texCoordsData;
        amesh->texCoordsData = NULL;
    }

    int olpl_cnt = 0;

    for (int i = 0; i < amesh->polyCnt; i++)
    {
        tUtV * v6 = olpl[i];
        while( v6->tu >= 0.0)
        {
            olpl_cnt++;
            v6++;
        }
        olpl_cnt++;
    }

    amesh->texCoords = new tUtV *[amesh->polyCnt];
    amesh->texCoordsData = new tUtV[olpl_cnt];

    if ( !amesh->texCoords || !amesh->texCoordsData )
        return 0;

    tUtV *uv = amesh->texCoordsData;
    //void *ed = ((char *)amesh->olpl + sizeof(tUtV *) * amesh->cnt + sizeof(tUtV) * olpl_cnt);

    for (int i = 0; i < amesh->polyCnt; i++)
    {
        tUtV * inCoord = olpl[i];
        amesh->texCoords[i] = uv;
        while( 1 )
        {
            *uv = *inCoord;
            inCoord++;
            uv++;

            if (inCoord->tu < 0.0)
                break;
        }
        *uv = *inCoord;
        uv++;
    }
    return 1;
}

size_t NC_STACK_amesh::func0(IDVList &stak)
{
    if ( !NC_STACK_area::func0(stak) )
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
                    func1();
                    return 0;
                }
                break;

            case AMESH_ATT_OTLPOOL:
                if ( !setAMESH_otls(val.Get<tUtV **>()) )
                {
                    func1();
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

size_t NC_STACK_amesh::func1()
{
    __NC_STACK_amesh *amesh = &stack__amesh;

    if ( amesh->atts )
        delete[] amesh->atts;

    if ( amesh->texCoords )
        delete[] amesh->texCoords;

    if ( amesh->texCoordsData )
        delete[] amesh->texCoordsData;

    return NC_STACK_area::func1();
}


size_t NC_STACK_amesh::func5(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;
    __NC_STACK_amesh *amesh = NULL;

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

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_AREA )
        {
            obj_ok = NC_STACK_area::func5(file);

            if ( !obj_ok )
                return 0;

            amesh = &stack__amesh;
        }
        else if ( chunk->TAG == TAG_ATTS )
        {
            if ( obj_ok )
            {
                amesh->polyCnt = chunk->TAG_SIZE / 6;
                amesh->atts = new ATTS[amesh->polyCnt];;

                if ( !amesh->atts )
                {
                    func1();
                    return 0;
                }

                for (int i = 0; i < amesh->polyCnt; i++)
                {
                    mfile->readS16B(amesh->atts[i].polyID);
                    mfile->readU8(amesh->atts[i].colorVal);
                    mfile->readU8(amesh->atts[i].shadeVal);
                    mfile->readU8(amesh->atts[i].tracyVal);
                    mfile->readU8(amesh->atts[i].pad);
                }
            }
            mfile->parse();
        }
        else if ( chunk->TAG == TAG_OLPL )
        {
            if ( obj_ok )
            {
                amesh->texCoords = new tUtV *[amesh->polyCnt];
                amesh->texCoordsData = new tUtV[ chunk->TAG_SIZE / 2 ];

                if ( !amesh->texCoords || !amesh->texCoordsData)
                {
                    func1();
                    return 0;
                }

                tUtV *uv = amesh->texCoordsData;

                for (int i = 0; i < amesh->polyCnt; i++)
                {
                    amesh->texCoords[i] = uv;

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

size_t NC_STACK_amesh::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_amesh *amesh = &stack__amesh;

    if ( mfile->pushChunk(TAG_AMSH, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_area::func6(file) )
        return 0;

    mfile->pushChunk(0, TAG_ATTS, -1);

    for (int i = 0; i < amesh->polyCnt; i++)
    {
        mfile->writeS16B(amesh->atts[i].polyID);
        mfile->writeU8(amesh->atts[i].colorVal);
        mfile->writeU8(amesh->atts[i].shadeVal);
        mfile->writeU8(amesh->atts[i].tracyVal);
        mfile->writeU8(amesh->atts[i].pad);
    }

    mfile->popChunk();

    if ( amesh->texCoords )
    {
        mfile->pushChunk(0, TAG_OLPL, -1);

        for (int i = 0; i < amesh->polyCnt; i++)
        {

            tUtV *uv = amesh->texCoords[i];
            int16_t cnt = 0;
            while (uv->tu >= 0.0)
            {
                cnt++;
                uv++;
            }

            mfile->writeS16B(cnt);

            uv = amesh->texCoords[i];

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
size_t NC_STACK_amesh::ade_func65(area_arg_65 *arg)
{
    __NC_STACK_amesh *amesh = &stack__amesh;

    //v5 = *(_uint16_t *)(amesh->field_14 + 6) & 0xFEF6;
    int renderFlags = stack__area.polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

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


    skeleton_arg133 skel133;

    skel133.field_4 = 0;

    if ( renderFlags & (NC_STACK_display::RFLAGS_LINMAP | NC_STACK_display::RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( renderFlags & (NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( amesh->flags & AMESH_FLAG_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.minZ = arg->minZ;
    skel133.maxZ = arg->maxZ;
    skel133.fadeStart = arg->fadeStart;
    skel133.fadeLength = arg->fadeLength;


    ResBitmap *v21;

    if ( amesh->ilbm1 )
    {
        bitmap_arg130 bitm130;

        bitm130.time_stmp = arg->timeStamp;
        bitm130.frame_time = arg->frameTime;
        amesh->ilbm1->bitmap_func130(&bitm130);
        v21 = bitm130.pbitm;
    }
    else
    {
        v21 = NULL;
    }

    for (int i = 0; i < amesh->polyCnt; i++)
    {
        polysDat *data = arg->rndrStack->get();
        polysDatSub *datSub = &data->datSub;

        datSub->renderFlags = renderFlags;
        datSub->r = 1.0;
        datSub->g = 1.0;
        datSub->b = 1.0;

        datSub->pbitm = v21;

        skel133.polyID = amesh->atts[i].polyID;

        skel133.rndrArg = datSub;

        skel133.shadeVal = amesh->atts[i].shadeVal / 256.0;

        if ( amesh->texCoords )
            skel133.texCoords = amesh->texCoords[i];
        else
            skel133.texCoords = NULL;

        if (arg->OBJ_SKELETON->skeleton_func133(&skel133))
        {
            arg->adeCount++;

            if ( datSub->renderFlags & ( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD ) )
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
                    datSub->renderFlags &= ~( NC_STACK_display::RFLAGS_FLATSHD | NC_STACK_display::RFLAGS_GRADSHD );
                }
                else if ( v8 == datSub->vertexCount )
                {
                    datSub->renderFlags = 0;
                }
            }

            datSub->renderFlags |= (arg->flags & NC_STACK_display::RFLAGS_SKY);

            float maxz = 0.0;

            for (int j = 0; j < datSub->vertexCount; j++)
                if (datSub->vertexes[j].z > maxz)
                    maxz = datSub->vertexes[j].z;

            if ( !(arg->flags & NC_STACK_display::RFLAGS_IGNORE_FALLOFF) && NC_STACK_win3d::win3d_keys[18].Get<bool>() )
            {
                float maxln = 0.0;

                for (int j = 0; j < datSub->vertexCount; j++)
                {
                    datSub->distance[j] = datSub->vertexes[j].XZ().length();
                    if (datSub->distance[j] > maxln)
                        maxln = datSub->distance[j];
                }

                if (maxln > NC_STACK_win3d::win3d_keys[19].Get<int>())
                    datSub->renderFlags |= NC_STACK_display::RFLAGS_FALLOFF;
            }

            data->render_func = GFXEngine::defRenderFunc;
            data->range = maxz;

            arg->rndrStack->commit();
        }
    }

    return 1;
}



void NC_STACK_amesh::setADE_depthFade(int arg)
{
    if ( arg )
        stack__amesh.flags |= AMESH_FLAG_DPTHFADE;
    else
        stack__amesh.flags &= ~ADE_FLAG_DPTHFADE;

    NC_STACK_area::setADE_depthFade(arg);
}

void NC_STACK_amesh::setAREA_bitm(NC_STACK_bitmap *bitm)
{
    stack__amesh.ilbm1 = bitm;
    bitm->PrepareTexture();

    NC_STACK_area::setAREA_bitm(bitm);
}

void NC_STACK_amesh::setAREA_tracybitm(NC_STACK_bitmap *bitm)
{
    stack__amesh.ilbm2 = bitm;
    bitm->PrepareTexture();

    NC_STACK_area::setAREA_tracybitm(bitm);
}

void NC_STACK_amesh::setAMESH_numpoly(int num)
{
    stack__amesh.polyCnt = num;
}

int NC_STACK_amesh::setAMESH_polys(ATTS *atts)
{
    if ( stack__amesh.atts )
    {
        delete[] stack__amesh.atts;
        stack__amesh.atts = NULL;
    }
    stack__amesh.atts = new ATTS[stack__amesh.polyCnt];

    if ( !stack__amesh.atts )
        return 0;

    for (int i = 0; i < stack__amesh.polyCnt; i++)
        stack__amesh.atts[i] = atts[i];

    return 1;
}

int NC_STACK_amesh::setAMESH_otls(tUtV **uv)
{
    return sub_419E6C(&stack__amesh, uv);
}


int NC_STACK_amesh::getAMESH_numpoly()
{
    return stack__amesh.polyCnt;
}