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


int area_func0__sub0(__NC_STACK_area *area, stack_vals *stak)
{
    stack_vals *stk = stak;

    area->field_c = 1;
    area->field_D = 0;
    area->field_E = 0;

    int16_t v4 = 0;

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

            case 0x80001004:
                if ( stk->value )
                    area->field_8 |= 1;
                else
                    area->field_8 &= 0xFFFE;
                break;
            case 0x80001008:
                area->field_14 = stk->value;
                break;
            case 0x80002000:
                if ( stk->value )
                {
                    NC_STACK_bitmap *ilbm = (NC_STACK_bitmap *)stk->value;
                    if ( area->ilbm1 != NULL )
                        delete_class_obj(area->ilbm1);
                    area->ilbm1 = ilbm;
                }
                break;
            case 0x80002001:
                area->field_c = stk->value;
                break;
            case 0x80002004:
                v4 &= 0xFFF9;

                if ( stk->value == 1 )
                {
                    v4 |= 2;
                }
                else if ( stk->value == 2 )
                {
                    v4 |= 6;
                }
                break;

            case 0x80002005:
                v4 &= 0xFFF7;

                if ( stk->value == 1 )
                    v4 |= 8;
                break;

            case 0x80002006:
                v4 &= 0xFFCF;
                switch ( stk->value )
                {
                default:
                    break;
                case 1:
                    v4 |= 0x10;
                    break;
                case 2:
                    v4 |= 0x20;
                    break;
                case 3:
                    v4 |= 0x30;
                    break;
                }
                break;

            case 0x80002007:
                v4 &= 0xFF3F;
                switch ( stk->value )
                {
                default:
                    break;
                case 1:
                    v4 |= 0x40;
                    break;
                case 2:
                    v4 |= 0x80;
                    break;
                case 3:
                    v4 |= 0xC0;
                    break;
                }
                break;

            case 0x80002008:
                v4 &= 0xFEFF;
                if ( stk->value == 1 )
                    v4 |= 0x100;
                break;

            case 0x80002009:
                if ( stk->value )
                {
                    NC_STACK_bitmap *ilbm = (NC_STACK_bitmap *)stk->value;
                    if ( area->ilbm2 != NULL )
                        delete_class_obj(area->ilbm2);
                    area->ilbm2 = ilbm;
                }
                break;

            case 0x8000200A:
                area->field_E = stk->value;
                break;

            case 0x8000200B:
                area->field_D = stk->value;
                break;
            }
            stk++;
        }
    }
    area->field_16 = v4;

    return 1;
}


size_t NC_STACK_area::func0(stack_vals *stak)
{
    if ( !NC_STACK_ade::func0(stak) )
        return 0;

    __NC_STACK_area *area = &this->stack__area;

    if ( !area_func0__sub0(area, stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_area::func1(stack_vals *stak)
{
    __NC_STACK_area *area = &this->stack__area;

    if ( area->ilbm1 )
        delete_class_obj(area->ilbm1);
    if ( area->ilbm2 )
        delete_class_obj(area->ilbm2);
    return NC_STACK_ade::func1(stak);
}


void area_func2__sub0(__NC_STACK_area *area, stack_vals *stak)
{

    stack_vals *stk = stak;

    int16_t v4 = area->field_16;

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

            case 0x80001004:
                if ( stk->value )
                    area->field_8 |= 1;
                else
                    area->field_8 &= 0xFFFE;
                break;
            case 0x80001008:
                area->field_14 = stk->value;
                break;
            case 0x80002000:
                if ( stk->value )
                {
                    NC_STACK_bitmap *ilbm = (NC_STACK_bitmap *)stk->value;
                    if ( area->ilbm1 != NULL )
                        delete_class_obj(area->ilbm1);
                    area->ilbm1 = ilbm;
                }
                break;
            case 0x80002001:
                area->field_c = stk->value;
                break;
            case 0x80002004:
                v4 &= 0xFFF9;

                if ( stk->value == 1 )
                {
                    v4 |= 2;
                }
                else if ( stk->value == 2 )
                {
                    v4 |= 6;
                }
                break;

            case 0x80002005:
                v4 &= 0xFFF7;

                if ( stk->value == 1 )
                    v4 |= 8;
                break;

            case 0x80002006:
                v4 &= 0xFFCF;
                switch ( stk->value )
                {
                default:
                    break;
                case 1:
                    v4 |= 0x10;
                    break;
                case 2:
                    v4 |= 0x20;
                    break;
                case 3:
                    v4 |= 0x30;
                    break;
                }
                break;

            case 0x80002007:
                v4 &= 0xFF3F;
                switch ( stk->value )
                {
                default:
                    break;
                case 1:
                    v4 |= 0x40;
                    break;
                case 2:
                    v4 |= 0x80;
                    break;
                case 3:
                    v4 |= 0xC0;
                    break;
                }
                break;

            case 0x80002008:
                v4 &= 0xFEFF;
                if ( stk->value == 1 )
                    v4 |= 0x100;
                break;

            case 0x80002009:
                if ( stk->value )
                {
                    NC_STACK_bitmap *ilbm = (NC_STACK_bitmap *)stk->value;
                    if ( area->ilbm2 != NULL )
                        delete_class_obj(area->ilbm2);
                    area->ilbm2 = ilbm;
                }
                break;

            case 0x8000200A:
                area->field_E = stk->value;
                break;

            case 0x8000200B:
                area->field_D = stk->value;
                break;
            case 0x8000200C:
                v4 = stk->value & 0xFFFF;
                area->field_8 = stk->value >> 16;
                break;
            case 0x8000200D:
                area->field_E = stk->value & 0xFF;
                area->field_c = (stk->value >> 16) & 0xFF;
                area->field_D = (stk->value >> 8) & 0xFF;
                break;
            }
            stk++;
        }
    }
    area->field_16 = v4;
}

size_t NC_STACK_area::func2(stack_vals *stak)
{
    __NC_STACK_area *area = &this->stack__area;

    area_func2__sub0(area, stak);

    return NC_STACK_ade::func2(stak);
}

void area_func3__sub0(__NC_STACK_area *area, stack_vals *stak)
{
    stack_vals *stk = stak;

    int16_t v3 = area->field_16;
    int16_t v4 = v3 & 0x30;
    int16_t v9 = v3 & 6;
    int16_t v5 = v3 & 0xC0;
    int16_t v6 = v3 & 8;
    int16_t v10 = v3 & 0x100;

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

            case 0x80002000:
                *(NC_STACK_bitmap **)stk->value = area->ilbm1;
                break;

            case 0x80002001:
                *(int *)stk->value = area->field_c;
                break;

            case 0x80002004:
                if ( v9 == 0 )
                {
                    *(int *)stk->value = 0;
                }
                else if ( v9 <= 2 )
                {
                    *(int *)stk->value = 1;
                }
                else if ( v9 == 6 )
                {
                    *(int *)stk->value = 2;
                }
                break;

            case 0x80002005:
                if ( v6 == 8 )
                {
                    *(int *)stk->value = 1;
                }
                else
                {
                    *(int *)stk->value = 0;
                }
                break;

            case 0x80002006:
                if ( v4 == 0 )
                {
                    *(int *)stk->value = 0;
                }
                else if ( v4 == 0x10 )
                {
                    *(int *)stk->value = 1;
                }
                else if ( v4 == 0x20u )
                {
                    *(int *)stk->value = 2;
                }
                else if ( v4 == 0x30 )
                {
                    *(int *)stk->value = 3;
                }
                break;

            case 0x80002007:

                if ( v5 == 0 )
                {
                    *(int *)stk->value = 0;
                }
                else if ( v5 == 0x40 )
                {
                    *(int *)stk->value = 1;
                }
                else if ( v5 == 0x80u )
                {
                    *(int *)stk->value = 2;
                }
                else if ( v5 == 0xC0 )
                {
                    *(int *)stk->value = 3;
                }
                break;

            case 0x80002008:
                if ( v10 == 0x100 )
                    *(int *)stk->value = 1;
                else if ( v10 == 0 )
                    *(int *)stk->value = 0;
                break;

            case 0x80002009:
                *(NC_STACK_bitmap **)stk->value = area->ilbm2;
                break;

            case 0x8000200A:
                *(int *)stk->value = area->field_E;
                break;

            case 0x8000200B:
                *(int *)stk->value = area->field_D;
                break;

            case 0x8000200E:
                *(int **)stk->value = &area->field_10;
                break;

            }
            stk++;
        }
    }
}

size_t NC_STACK_area::func3(stack_vals *stak)
{

    __NC_STACK_area *area = &this->stack__area;

    area_func3__sub0(area, stak);

    return NC_STACK_ade::func3(stak);
}

int area_func5__sub0(NC_STACK_area *obj, __NC_STACK_area *, MFILE *mfile)
{
    if ( obj )
    {
        AREA_STRC tmp;
        mfread(mfile, &tmp, sizeof(AREA_STRC));
        tmp.field_0 = SWAP16(tmp.field_0);
        tmp.field_2 = SWAP32(tmp.field_2);
        tmp.field_6 = SWAP32(tmp.field_6);

        stack_vals stk[3];

        stk[0].id = 0;

        if ( tmp.field_0 >= 1 )
        {
            stk[0].id = 0x8000200C;
            stk[0].value = tmp.field_2;
            stk[1].id = 0x8000200D;
            stk[1].value = tmp.field_6;
            stk[2].id = 0;
        }
        obj->func2(stk);

        return 1;
    }

    return 0;
}



int area_func5__sub1(NC_STACK_area *obj, __NC_STACK_area *area, MFILE *mfile)
{
    if ( obj )
    {
        int v8 = area->field_16 & 8;

        if ( (area->field_16 & 0xC0) == 0xC0 )
            v8 |= 0xC0;

        NC_STACK_nucleus *objt = (NC_STACK_nucleus *)READ_OBJT(mfile);
        if ( objt )
        {
            if (v8 == 8 )
            {
                call_vtbl(obj, 2, 0x80002000, objt, 0);
            }
            else if ( v8 == 0xC0 )
            {
                call_vtbl(obj, 2, 0x80002009, objt, 0);
            }
            else if ( v8 == 0xC8)
            {
                if ( area->ilbm1 )
                {
                    call_vtbl(obj, 2, 0x80002009, objt, 0);
                }
                else
                {
                    call_vtbl(obj, 2, 0x80002000, objt, 0);
                }
            }
            return 1;
        }
    }

    return 0;
}


size_t NC_STACK_area::func5(MFILE **file)
{
    MFILE *mfile = *file;
    int obj_ok = 0;
    __NC_STACK_area *area = NULL;

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

            area = &this->stack__area;

        }
        else if ( chunk->TAG == TAG_STRC )
        {
            if ( !area_func5__sub0(this, area, mfile) )
            {
                func1(NULL);
                return 0;
            }
            read_next_IFF(mfile, 2);
        }
        else if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
        {
            if ( !area_func5__sub1(this, area, mfile) )
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
    __NC_STACK_area *area = &this->stack__area;

    if ( sub_412FC0(mfile, TAG_AREA, TAG_FORM, -1) )
        return 0;


    if ( !NC_STACK_ade::func6(file) )
        return 0;

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    AREA_STRC tmp;
    tmp.field_0 = 1;
    tmp.field_2 = area->field_16 | (area->field_8 << 16);
    tmp.field_6 = area->field_E | (area->field_D << 8) | (area->field_c << 16);

    tmp.field_2 = SWAP32(tmp.field_2);
    tmp.field_6 = SWAP32(tmp.field_6);

    sub_413564(mfile, sizeof(AREA_STRC), &tmp);
    sub_413290(mfile);

    if ( (area->field_16 & 8) )
    {
        if ( !area->ilbm1 )
            return 0;
        if ( !sub_4117F8(area->ilbm1, mfile) )
            return 0;
    }
    if ( (area->field_16 & 0xC0) == 0xC0 )
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
    __NC_STACK_area *area = &this->stack__area;
    polysDatSub *datSub = &arg->polyDat->datSub;

    int v5 = area->field_16 & 0xFEF6;

    if (v5 == 0)
        v5 = 0;
    else if (v5 == 2)
        v5 = 1;
    else if (v5 == 6)
        v5 = 2;
    else if (v5 == 0x32)
        v5 = 9;
    else if (v5 == 0x36)
        v5 = 10;
    else if (v5 == 0x42)
        v5 = 17;
    else if (v5 == 0x46)
        v5 = 18;
    else if (v5 == 0x72)
        v5 = 25;
    else if (v5 == 0x76)
        v5 = 26;
    else if (v5 == 0x82)
        v5 = 33;
    else
        return 1;

    datSub->renderFlags = v5;

    datSub->vertexes = (xyz *)&datSub->field_18;

    skeleton_arg133 skel133;

    skel133.field_0 = area->field_14;
    skel133.field_4 = 0;

    if ( datSub->renderFlags & 3 )
        skel133.field_4 = 1;
    if ( datSub->renderFlags & 0xC )
        skel133.field_4 |= 2;
    if ( area->field_8 & 1 )
        skel133.field_4 |= 4;

    skel133.polysubDat = datSub;
    skel133.field_10 = arg->field_24;
    skel133.field_14 = arg->field_28;
    skel133.field_18 = area->field_E * 0.00390625;
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

        if ( datSub->renderFlags & 0xC )
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
                datSub->renderFlags &= 0xFFFFFFF3;
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


        arg->outPolys->range = maxz;
        arg->outPolys->data = arg->polyDat;
        arg->outPolys++;

        arg->polyDat->render_func = GFXEngine::defRenderFunc;
        arg->polyDat = v19;
    }
    return 1;
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
