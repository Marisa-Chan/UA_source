#include "includes.h"
#include "nucleas.h"
#include "area.h"
#include "utils.h"
#include "engine_input.h"

#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"

#include "skeleton.h"
#include "base.h"

#include "engine_gfx.h"


const NewClassDescr NC_STACK_area::description("area.class", &newinstance);


int NC_STACK_area::area_func0__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    stack__area.field_c = 1;
    stack__area.field_D = 0;
    stack__area.field_E = 0;
    stack__area.polflags = 0;

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
            case ADE_ATT_POLY:
                setADE_bkCheck( stk->value.i_data );
                break;
            case AREA_ATT_TEXBITM:
                setAREA_bitm ( (NC_STACK_bitmap *)stk->value.p_data );
                break;
            case AREA_ATT_COLORVAL:
                setAREA_colorVal( stk->value.i_data );
                break;
            case AREA_ATT_MAP:
                setAREA_map( stk->value.i_data );
                break;

            case AREA_ATT_TEX:
                setAREA_tex( stk->value.i_data );
                break;

            case AREA_ATT_SHADE:
                setAREA_shade ( stk->value.i_data );
                break;

            case AREA_ATT_TRACY:
                setAREA_tracy ( stk->value.i_data );
                break;

            case AREA_ATT_TRACYMODE:
                setAREA_tracymode( stk->value.i_data );
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm ( (NC_STACK_bitmap *)stk->value.p_data );
                break;

            case AREA_ATT_SHADEVAL:
                setAREA_shadeVal ( stk->value.i_data );
                break;

            case AREA_ATT_TRACYVAL:
                setAREA_tracyVal ( stk->value.i_data );
                break;
            }
            stk++;
        }
    }

    return 1;
}


size_t NC_STACK_area::func0(stack_vals *stak)
{
    if ( !NC_STACK_ade::func0(stak) )
        return 0;

    if ( !area_func0__sub0(stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_area::func1(stack_vals *stak)
{
    __NC_STACK_area *area = &stack__area;

    if ( area->ilbm1 )
        delete_class_obj(area->ilbm1);
    if ( area->ilbm2 )
        delete_class_obj(area->ilbm2);
    return NC_STACK_ade::func1(stak);
}


void NC_STACK_area::area_func2__sub0(stack_vals *stak)
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
            case ADE_ATT_POLY:
                setADE_bkCheck( stk->value.i_data );
                break;
            case AREA_ATT_TEXBITM:
                setAREA_bitm ( (NC_STACK_bitmap *)stk->value.p_data );
                break;
            case AREA_ATT_COLORVAL:
                setAREA_colorVal( stk->value.i_data );
                break;
            case AREA_ATT_MAP:
                setAREA_map( stk->value.i_data );
                break;

            case AREA_ATT_TEX:
                setAREA_tex( stk->value.i_data );
                break;

            case AREA_ATT_SHADE:
                setAREA_shade ( stk->value.i_data );
                break;

            case AREA_ATT_TRACY:
                setAREA_tracy ( stk->value.i_data );
                break;

            case AREA_ATT_TRACYMODE:
                setAREA_tracymode( stk->value.i_data );
                break;

            case AREA_ATT_TRACYBITM:
                setAREA_tracybitm ( (NC_STACK_bitmap *)stk->value.p_data );
                break;

            case AREA_ATT_SHADEVAL:
                setAREA_shadeVal ( stk->value.i_data );
                break;

            case AREA_ATT_TRACYVAL:
                setAREA_tracyVal ( stk->value.i_data );
                break;
            case AREA_ATT_BLOB1:
                setAREA_blob1( stk->value.i_data );
                break;
            case AREA_ATT_BLOB2:
                setAREA_blob2( stk->value.i_data );
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_area::func2(stack_vals *stak)
{
    area_func2__sub0(stak);

    return NC_STACK_ade::func2(stak);
}

void NC_STACK_area::area_func3__sub0(stack_vals *stak)
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

            case AREA_ATT_TEXBITM:
                *(NC_STACK_bitmap **)stk->value.p_data = getAREA_bitm();
                break;

            case AREA_ATT_COLORVAL:
                *(int *)stk->value.p_data = getAREA_colorVal();
                break;

            case AREA_ATT_MAP:
                *(int *)stk->value.p_data = getAREA_map();
                break;

            case AREA_ATT_TEX:
                *(int *)stk->value.p_data = getAREA_tex();
                break;

            case AREA_ATT_SHADE:
                *(int *)stk->value.p_data = getAREA_shade();
                break;

            case AREA_ATT_TRACY:
                *(int *)stk->value.p_data = getAREA_tracy();
                break;

            case AREA_ATT_TRACYMODE:
                *(int *)stk->value.p_data = getAREA_tracymode();
                break;

            case AREA_ATT_TRACYBITM:
                *(NC_STACK_bitmap **)stk->value.p_data = getAREA_tracybitm();
                break;

            case AREA_ATT_SHADEVAL:
                *(int *)stk->value.p_data = getAREA_shadeVal();
                break;

            case AREA_ATT_TRACYVAL:
                *(int *)stk->value.p_data = getAREA_tracyVal();
                break;

//            case AREA_ATT_PolInfo:
//                *(int **)stk->value = &area->field_10;
//                break;

            }
            stk++;
        }
    }
}

size_t NC_STACK_area::func3(stack_vals *stak)
{
    area_func3__sub0(stak);

    return NC_STACK_ade::func3(stak);
}

int NC_STACK_area::area_func5__sub0(MFILE *mfile)
{
    AREA_STRC tmp;
    mfread(mfile, &tmp, sizeof(AREA_STRC));
    tmp.field_0 = SWAP16(tmp.field_0);
    tmp.field_2 = SWAP32(tmp.field_2);
    tmp.field_6 = SWAP32(tmp.field_6);

    if ( tmp.field_0 >= 1 )
    {
        setAREA_blob1(tmp.field_2);
        setAREA_blob2(tmp.field_6);
    }

    return 1;
}



int NC_STACK_area::area_func5__sub1(MFILE *mfile)
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
            if ( stack__area.ilbm1 )
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


size_t NC_STACK_area::func5(MFILE **file)
{
    MFILE *mfile = *file;
    int obj_ok = 0;

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
                func1(NULL);
                return 0;
            }
            read_next_IFF(mfile, 2);
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( obj_ok && !area_func5__sub1(mfile) )
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

size_t NC_STACK_area::func6(MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_area *area = &stack__area;

    if ( sub_412FC0(mfile, TAG_AREA, TAG_FORM, -1) )
        return 0;


    if ( !NC_STACK_ade::func6(file) )
        return 0;

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    AREA_STRC tmp;
    tmp.field_0 = 1;
    tmp.field_2 = area->polflags | (area->field_8 << 16);
    tmp.field_6 = area->field_E | (area->field_D << 8) | (area->field_c << 16);

    tmp.field_2 = SWAP32(tmp.field_2);
    tmp.field_6 = SWAP32(tmp.field_6);

    sub_413564(mfile, sizeof(AREA_STRC), &tmp);
    sub_413290(mfile);

    if ( (area->polflags & AREA_POL_FLAG_TEXUTRED) )
    {
        if ( !area->ilbm1 )
            return 0;
        if ( !sub_4117F8(area->ilbm1, mfile) )
            return 0;
    }
    if ( (area->polflags & AREA_POL_FLAG_TRACYMAPPED) == AREA_POL_FLAG_TRACYMAPPED )
    {
        if ( !area->ilbm2 )
            return 0;
        if ( !sub_4117F8(area->ilbm2, mfile) )
            return 0;
    }

    return sub_413290(mfile) == 0;
}

// Add area to list
size_t NC_STACK_area::ade_func65(area_arg_65 *arg)
{
    __NC_STACK_area *area = &stack__area;
    polysDatSub *datSub = &arg->argSTK_cur->datSub;

    int v5 = area->polflags & ~(AREA_POL_FLAG_SCANLN | AREA_POL_FLAG_TEXBIT | AREA_POL_FLAG_TRACYBIT3);

    if (v5 == 0)
        v5 = 0;
    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_LINMAP;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_PERSPMAP;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_LINMAP | NC_STACK_raster::RSTR_RFLAGS_GRADSHD;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_NOTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_PERSPMAP | NC_STACK_raster::RSTR_RFLAGS_GRADSHD;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_LINMAP | NC_STACK_raster::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_PERSPMAP | NC_STACK_raster::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_LINMAP | NC_STACK_raster::RSTR_RFLAGS_GRADSHD | NC_STACK_raster::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_DEPTHMAPPED | AREA_POL_FLAG_GRADIENTSHADE | AREA_POL_FLAG_CLEARTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_PERSPMAP | NC_STACK_raster::RSTR_RFLAGS_GRADSHD | NC_STACK_raster::RSTR_RFLAGS_ZEROTRACY;

    else if (v5 == (AREA_POL_FLAG_LINEARMAPPED | AREA_POL_FLAG_NOSHADE | AREA_POL_FLAG_FLATTRACY) )
        v5 = NC_STACK_raster::RSTR_RFLAGS_LINMAP | NC_STACK_raster::RSTR_RFLAGS_LUMTRACY;
    else
        return 1;

    datSub->renderFlags = v5;

    datSub->vertexes = (xyz *)(datSub + 1); // Vertex data goes after arg data

    skeleton_arg133 skel133;

    skel133.field_0 = area->polnum;
    skel133.field_4 = 0;

    if ( datSub->renderFlags & (NC_STACK_raster::RSTR_RFLAGS_LINMAP | NC_STACK_raster::RSTR_RFLAGS_PERSPMAP ) )
        skel133.field_4 |= 1;
    if ( datSub->renderFlags & (NC_STACK_raster::RSTR_RFLAGS_FLATSHD | NC_STACK_raster::RSTR_RFLAGS_GRADSHD) )
        skel133.field_4 |= 2;
    if ( area->field_8 & AREA_FLAG_DPTHFADE )
        skel133.field_4 |= 4;

    skel133.rndrArg = datSub;
    skel133.field_10 = arg->field_24;
    skel133.field_14 = arg->field_28;
    skel133.field_18 = area->field_E / 256.0;
    skel133.field_1C = arg->field_2C;
    skel133.field_20 = arg->field_30;

    if ( area->ilbm1 )
    {
        bitmap_arg130 bitm130;

        bitm130.time_stmp = arg->field_C;
        bitm130.frame_time = arg->field_10;

        area->ilbm1->bitmap_func130(&bitm130);

        datSub->pbitm = bitm130.pbitm;
        skel133.field_C = bitm130.outline;
    }
    else
    {
        datSub->pbitm = NULL;
        skel133.field_C = NULL;
    }

    polysDat *v19 = (polysDat *)arg->OBJ_SKELETON->skeleton_func133(&skel133);

    if ( v19 )
    {
        arg->field_38++;

        if ( datSub->renderFlags & ( NC_STACK_raster::RSTR_RFLAGS_FLATSHD | NC_STACK_raster::RSTR_RFLAGS_GRADSHD ) )
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
                datSub->renderFlags &= ~( NC_STACK_raster::RSTR_RFLAGS_FLATSHD | NC_STACK_raster::RSTR_RFLAGS_GRADSHD );
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
        arg->argSTK_cur = v19;
    }
    return 1;
}

void NC_STACK_area::setADE_depthFade(int mode)
{
    if ( mode )
        stack__area.field_8 |= AREA_FLAG_DPTHFADE;
    else
        stack__area.field_8 &= ~AREA_FLAG_DPTHFADE;

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
        if ( stack__area.ilbm1 != NULL )
            delete_class_obj(stack__area.ilbm1);

        stack__area.ilbm1 = bitm;
    }
}

void NC_STACK_area::setAREA_colorVal(int val)
{
    stack__area.field_c = val;
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
        if ( stack__area.ilbm2 != NULL )
            delete_class_obj(stack__area.ilbm2);

        stack__area.ilbm2 = bitm;
    }
}

void NC_STACK_area::setAREA_shadeVal(int val)
{
    stack__area.field_E = val;
}

void NC_STACK_area::setAREA_tracyVal(int val)
{
    stack__area.field_D = val;
}

void NC_STACK_area::setAREA_blob1(uint32_t val)
{
    stack__area.polflags = val & 0xFFFF;
    stack__area.field_8 = val >> 16;
}

void NC_STACK_area::setAREA_blob2(uint32_t val)
{
    stack__area.field_E = val & 0xFF;
    stack__area.field_c = (val >> 16) & 0xFF;
    stack__area.field_D = (val >> 8) & 0xFF;
}


NC_STACK_bitmap *NC_STACK_area::getAREA_bitm()
{
    return stack__area.ilbm1;
}

int NC_STACK_area::getAREA_colorVal()
{
    return stack__area.field_c;
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
    return stack__area.ilbm2;
}

int NC_STACK_area::getAREA_shadeVal()
{
    return stack__area.field_E;
}

int NC_STACK_area::getAREA_tracyVal()
{
    return stack__area.field_D;
}



size_t NC_STACK_area::compatcall(int method_id, void *data)
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
    case 65:
        return (size_t)ade_func65( (area_arg_65 *)data );
    default:
        break;
    }
    return NC_STACK_ade::compatcall(method_id, data);
}
