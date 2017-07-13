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


const NewClassDescr NC_STACK_amesh::description("amesh.class", &newinstance);


int sub_419E6C(__NC_STACK_amesh *amesh, tUtV **olpl)
{
    if ( amesh->texCoords )
    {
        delete amesh->texCoords;
        amesh->texCoords = NULL;
    }

    if ( amesh->texCoordsData )
    {
        delete amesh->texCoordsData;
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


int NC_STACK_amesh::amesh_func0__sub0(stack_vals *stak)
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
            stk += stk->value.u_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case ADE_ATT_DPTHFADE:
                setADE_depthFade ( stk->value.i_data );
                break;
            case AREA_ATT_TEXBITM:
                setAREA_bitm((NC_STACK_bitmap *)stk->value.p_data);
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm((NC_STACK_bitmap *)stk->value.p_data);
                break;

            case AMESH_ATT_NUMPOLY:
                setAMESH_numpoly(stk->value.i_data);
                break;

            case AMESH_ATT_ATTPOLYS:
                if ( !setAMESH_polys( (ATTS *)stk->value.p_data ) )
                    return 0;
                break;

            case AMESH_ATT_OTLPOOL:
                if ( !setAMESH_otls((tUtV **)stk->value.p_data) )
                    return 0;
                break;

            }
            stk++;
        }
    }

    return 1;
}


size_t NC_STACK_amesh::func0(stack_vals *stak)
{
    if ( !NC_STACK_area::func0(stak) )
        return 0;

    if ( !amesh_func0__sub0(stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_amesh::func1(stack_vals *stak)
{
    __NC_STACK_amesh *amesh = &stack__amesh;

    if ( amesh->atts )
        delete amesh->atts;

    if ( amesh->texCoords )
        delete amesh->texCoords;

    if ( amesh->texCoordsData )
        delete amesh->texCoordsData;

    return NC_STACK_area::func1(stak);
}


void NC_STACK_amesh::amesh_func2__sub0(stack_vals *stak)
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

            case ADE_ATT_DPTHFADE:
                setADE_depthFade ( stk->value.i_data );
                break;
            case AREA_ATT_TEXBITM:
                setAREA_bitm((NC_STACK_bitmap *)stk->value.p_data);
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm((NC_STACK_bitmap *)stk->value.p_data);
                break;

            case AMESH_ATT_ATTPOLYS:
                setAMESH_polys((ATTS *)stk->value.p_data);
                break;

            case AMESH_ATT_OTLPOOL:
                setAMESH_otls((tUtV **)stk->value.p_data);
                break;
            }
            stk++;
        }
    }

}

size_t NC_STACK_amesh::func2(stack_vals *stak)
{
    amesh_func2__sub0(stak);

    return NC_STACK_area::func2(stak);
}

size_t NC_STACK_amesh::func3(stack_vals *stak)
{
    stack_vals *val = find_id_in_stack2(AMESH_ATT_NUMPOLY, stak);

    if ( val )
        *(int *)val->value.p_data = getAMESH_numpoly();

    return NC_STACK_area::func3(stak);
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
                func1(NULL);
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
                    func1(NULL);
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
                    func1(NULL);
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
                        uint8_t tmp[2];
                        mfile->readU8(tmp[0]);
                        mfile->readU8(tmp[1]);

                        uv->tu = tmp[0] / 256.0;
                        uv->tv = tmp[1] / 256.0;
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
                uint8_t tmp[2];
                tmp[0] = uv[j].tu * 256.0;
                tmp[1] = uv[j].tv * 256.0;

                mfile->writeU8(tmp[0]);
                mfile->writeU8(tmp[1]);
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
    int v5 = stack__area.polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

    if (v5 == 0)
        v5 = 0;
    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_LINMAP;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_PERSPMAP;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_LINMAP | NC_STACK_display::RSTR_RFLAGS_GRADSHD;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_PERSPMAP | NC_STACK_display::RSTR_RFLAGS_GRADSHD;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_LINMAP | NC_STACK_display::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_PERSPMAP | NC_STACK_display::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_LINMAP | NC_STACK_display::RSTR_RFLAGS_GRADSHD | NC_STACK_display::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_PERSPMAP | NC_STACK_display::RSTR_RFLAGS_GRADSHD | NC_STACK_display::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_FLATTRACY) )
        v5 = NC_STACK_display::RSTR_RFLAGS_LINMAP | NC_STACK_display::RSTR_RFLAGS_LUMTRACY;
    else
        return 1;

    skeleton_arg133 skel133;

    skel133.field_4 = 0;

    if ( v5 & (NC_STACK_display::RSTR_RFLAGS_LINMAP | NC_STACK_display::RSTR_RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( v5 & (NC_STACK_display::RSTR_RFLAGS_FLATSHD | NC_STACK_display::RSTR_RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( amesh->flags & AMESH_FLAG_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.field_10 = arg->minZ;
    skel133.field_14 = arg->maxZ;
    skel133.field_1C = arg->fadeStart;
    skel133.field_20 = arg->fadeLength;


    bitmap_intern *v21;

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
        polysDatSub *datSub = &arg->argSTK_cur->datSub;

        datSub->renderFlags = v5;

        datSub->vertexes = (xyz *)(datSub + 1);
        datSub->pbitm = v21;

        skel133.polyID = amesh->atts[i].polyID;

        skel133.rndrArg = datSub;

        skel133.field_18 = amesh->atts[i].shadeVal / 256.0;

        if ( amesh->texCoords )
            skel133.texCoords = amesh->texCoords[i];
        else
            skel133.texCoords = NULL;

        polysDat *v23 = (polysDat *)arg->OBJ_SKELETON->skeleton_func133(&skel133);
        if (v23)
        {
            arg->adeCount++;

            if ( datSub->renderFlags & ( NC_STACK_display::RSTR_RFLAGS_FLATSHD | NC_STACK_display::RSTR_RFLAGS_GRADSHD ) )
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
                    datSub->renderFlags &= ~( NC_STACK_display::RSTR_RFLAGS_FLATSHD | NC_STACK_display::RSTR_RFLAGS_GRADSHD );
                }
                else if ( v8 == datSub->vertexCount )
                {
                    datSub->renderFlags = 0;
                }
            }

            float maxz = 0.0;

            for (int i = 0; i < datSub->vertexCount; i++)
                if (datSub->vertexes[i].sz > maxz)
                    maxz = datSub->vertexes[i].sz;


            arg->rndrSTK_cur->range = maxz;
            arg->rndrSTK_cur->data = arg->argSTK_cur;
            arg->rndrSTK_cur++;

            arg->argSTK_cur->render_func = GFXEngine::defRenderFunc;
            arg->argSTK_cur = v23;
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

    NC_STACK_area::setAREA_bitm(bitm);
}

void NC_STACK_amesh::setAREA_tracybitm(NC_STACK_bitmap *bitm)
{
    stack__amesh.ilbm2 = bitm;

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
        delete stack__amesh.atts;
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



size_t NC_STACK_amesh::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
        return 1;
    case 3:
        return func3( (stack_vals *)data );
        return 1;
    case 5:
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
    case 65:
        ade_func65( (area_arg_65 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_area::compatcall(method_id, data);
}
