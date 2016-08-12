#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "utils.h"


const NewClassDescr NC_STACK_raster::description("raster.class", &newinstance);


size_t NC_STACK_raster::func0(stack_vals *stak)
{
    if ( !NC_STACK_bitmap::func0(stak) )
        return 0;

    dprintf("MAKE ME %s\n","raster_func0");

    __NC_STACK_raster *rstr = &stack__raster;
    rstr->bitm_intern = (bitmap_intern *)getRsrc_pData();

    rstr->field_24.x1 = 0;
    rstr->field_24.y1 = 0;
    rstr->field_24.x2 = rstr->bitm_intern->width - 1;
    rstr->field_24.y2 = rstr->bitm_intern->height - 1;

    rstr->field_54c = rstr->bitm_intern->width / 2;
    rstr->field_550 = rstr->bitm_intern->height / 2;

    rstr->field_554 = (rstr->bitm_intern->width / 2);
    rstr->field_558 = (rstr->bitm_intern->height / 2);

    return 1;
}

size_t NC_STACK_raster::func1(stack_vals *stak)
{
    dprintf("MAKE ME %s\n","raster_func1");
    return NC_STACK_bitmap::func1(stak);
}

size_t NC_STACK_raster::func2(stack_vals *stak)
{
    dprintf("MAKE ME %s\n","raster_func2");

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
            switch(stk->id)
            {
            case RSTR_ATT_FGPEN:
                setRSTR_FGpen(stk->value.u_data);
                break;

            case RSTR_ATT_BGPEN:
                setRSTR_BGpen(stk->value.u_data);
                break;

            case RSTR_ATT_SHADE_RMP:
                setRSTR_shdRmp((bitmap_intern *)stk->value.p_data);
                break;

            case RSTR_ATT_TRACY_RMP:
                setRSTR_trcRmp((bitmap_intern *)stk->value.p_data);
                break;

            case RSTR_ATT_FGAPEN:
                setRSTR_FGApen(stk->value.i_data);
                break;

            default:
                break;
            }
            stk++;
        }
    }

    return NC_STACK_bitmap::func2(stak);
}

size_t NC_STACK_raster::func3(stack_vals *stak)
{
    dprintf("MAKE ME %s\n","raster_func3");
    //__NC_STACK_raster *internal = &obj->stack__raster;

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
            stk++;
        }
    }
    return NC_STACK_bitmap::func3(stak);
}


size_t NC_STACK_raster::raster_func192(stack_vals *)
{
    __NC_STACK_raster *rstr = &stack__raster;
    bitmap_intern *bitm = rstr->bitm_intern;

    memset(bitm->buffer, rstr->field_c, bitm->width * bitm->height);
    return 0;
}

size_t NC_STACK_raster::raster_func193(bitmap_intern **out)
{
    __NC_STACK_raster *rstr = &stack__raster;
    bitmap_intern *bitm = rstr->bitm_intern;

    memcpy((*out)->buffer, bitm->buffer, bitm->width * bitm->height);
    return 0;
}

size_t NC_STACK_raster::raster_func198(w3d_func198arg *)
{
    dprintf("MAKE ME %s\n","raster_func198");
    return 0;
}

size_t NC_STACK_raster::raster_func199(w3d_func199arg *)
{
    dprintf("MAKE ME %s\n","raster_func199");
    return 0;
}

size_t NC_STACK_raster::raster_func200(w3d_func198arg *)
{
    dprintf("MAKE ME %s\n","raster_func200");
    return 0;
}

size_t NC_STACK_raster::raster_func201(w3d_func199arg *)
{
    dprintf("MAKE ME %s\n","raster_func201");
    return 0;
}

size_t NC_STACK_raster::raster_func202(rstr_arg204 *)
{
    dprintf("MAKE ME %s\n","raster_func202");
    return 0;
}

size_t NC_STACK_raster::raster_func203(stack_vals *)
{
    dprintf("MAKE ME %s\n","raster_func203");
    return 0;
}

size_t NC_STACK_raster::raster_func204(rstr_arg204 *)
{
    dprintf("MAKE ME %s\n","raster_func204");
    return 0;
}

size_t NC_STACK_raster::raster_func205(stack_vals *)
{
    dprintf("MAKE ME %s\n","raster_func205");
    return 0;
}

size_t NC_STACK_raster::raster_func206(polysDatSub *)
{
    dprintf("MAKE ME %s\n","raster_func206");
    return 0;
}

void NC_STACK_raster::raster_func207(rstr_207_arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    rstr->tiles[arg->id] = arg->tiles;
}

void NC_STACK_raster::raster_func208(rstr_207_arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;

    if ( arg->tiles )
    {
        arg->id = -1;

        for (int i = 0; i < 256; i++)
        {
            if (rstr->tiles[i] == arg->tiles)
            {
                arg->id = i;
                break;
            }
        }
    }
    else
    {
        arg->tiles = rstr->tiles[arg->id];
    }
}

void NC_STACK_raster::raster_func209(w3d_a209 *)
{
    dprintf("MAKE ME %s\n","raster_func209");
}

void NC_STACK_raster::raster_func210(ua_fRect *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;

    rstr->field_24.x1 = (arg->x1 + 1.0) * (rstr->field_554 + -1.0);
    rstr->field_24.y1 = (arg->y1 + 1.0) * (rstr->field_558 + -1.0);
    rstr->field_24.x2 = (arg->x2 + 1.0) * (rstr->field_554 + -1.0);
    rstr->field_24.y2 = (arg->y2 + 1.0) * (rstr->field_558 + -1.0);
}

void NC_STACK_raster::raster_func211(ua_dRect *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;

    rstr->field_24.x1 = rstr->field_54c + arg->x1;
    rstr->field_24.y1 = rstr->field_550 + arg->y1;
    rstr->field_24.x2 = rstr->field_54c + arg->x2;
    rstr->field_24.y2 = rstr->field_550 + arg->y2;
}

size_t NC_STACK_raster::raster_func212(stack_vals *)
{
    dprintf("MAKE ME %s\n","raster_func212");
    return 0;
}

void NC_STACK_raster::raster_func213(polysDatSub *)
{
}

size_t NC_STACK_raster::raster_func214(void *)
{
    dprintf("MAKE ME %s\n","raster_func214");
    return 0;
}

void NC_STACK_raster::raster_func215(void *)
{
}

void NC_STACK_raster::raster_func216(void *)
{
}

size_t NC_STACK_raster::raster_func217(rstr_arg217 *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;

    if ( arg->dword0 != 0xFFFFFFFF )
        rstr->field_4 = arg->dword0;

    if ( arg->dword4 != 0xFFFFFFFF )
        rstr->field_8 = arg->dword4;

    if ( arg->dword8 != 0xFFFFFFFF )
        rstr->field_c = arg->dword8;

    return 0;
}

void NC_STACK_raster::raster_func218(rstr_218_arg *)
{
    dprintf("MAKE ME %s\n","raster_func218");
}

size_t NC_STACK_raster::raster_func219(stack_vals *)
{
    dprintf("MAKE ME %s\n","raster_func219");
    return 0;
}

size_t NC_STACK_raster::raster_func220(stack_vals *)
{
    dprintf("MAKE ME %s\n","raster_func220");
    return 0;
}

void NC_STACK_raster::raster_func221(ua_dRect *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;

    rstr->field_38.x1 = rstr->field_54c + arg->x1;
    rstr->field_38.y1 = rstr->field_550 + arg->y1;
    rstr->field_38.x2 = rstr->field_54c + arg->x2;
    rstr->field_38.y2 = rstr->field_550 + arg->y2;
}



void NC_STACK_raster::setRSTR_FGpen(uint32_t pen)
{
    stack__raster.field_4 = pen;
}

void NC_STACK_raster::setRSTR_BGpen(uint32_t pen)
{
    stack__raster.field_c = pen;
}

void NC_STACK_raster::setRSTR_shdRmp(bitmap_intern *rmp)
{
//	rstr->field_10 = rmp;
//	if ( rmp )
//		rstr->field_14 = rmp->buffer;
}

void NC_STACK_raster::setRSTR_trcRmp(bitmap_intern *rmp)
{
//	rstr->field_18 = rmp;
//	if ( rmp )
//		rstr->field_1c = rmp->buffer;
}

void NC_STACK_raster::setRSTR_FGApen(uint32_t pen)
{
    stack__raster.field_8 = pen;
}


size_t NC_STACK_raster::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 192:
        return (size_t)raster_func192( (stack_vals *)data );
    case 193:
        return (size_t)raster_func193( (bitmap_intern **)data );
    case 198:
        return (size_t)raster_func198( (w3d_func198arg *)data );
    case 199:
        return (size_t)raster_func199( (w3d_func199arg *)data );
    case 200:
        return (size_t)raster_func200( (w3d_func198arg *)data );
    case 201:
        return (size_t)raster_func201( (w3d_func199arg *)data );
    case 202:
        return (size_t)raster_func202( (rstr_arg204 *)data );
    case 203:
        return (size_t)raster_func203( (stack_vals *)data );
    case 204:
        return (size_t)raster_func204( (rstr_arg204 *)data );
    case 205:
        return (size_t)raster_func205( (stack_vals *)data );
    case 206:
        return (size_t)raster_func206( (polysDatSub *)data );
    case 207:
        raster_func207( (rstr_207_arg *)data );
        return 1;
    case 208:
        raster_func208( (rstr_207_arg *)data );
        return 1;
    case 209:
        raster_func209( (w3d_a209 *)data );
        return 1;
    case 210:
        raster_func210( (ua_fRect *)data );
        return 1;
    case 211:
        raster_func211( (ua_dRect *)data );
        return 1;
    case 212:
        return (size_t)raster_func212( (stack_vals *)data );
    case 213:
        raster_func213( (polysDatSub *)data );
        return 1;
    case 214:
        return (size_t)raster_func214( (stack_vals *)data );
    case 215:
        raster_func215( (void *)data );
        return 1;
    case 216:
        raster_func216( (void *)data );
        return 1;
    case 217:
        return (size_t)raster_func217( (rstr_arg217 *)data );
    case 218:
        raster_func218( (rstr_218_arg *)data );
        return 1;
    case 219:
        return (size_t)raster_func219( (stack_vals *)data );
    case 220:
        return (size_t)raster_func220( (stack_vals *)data );
    case 221:
        raster_func221( (ua_dRect *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_bitmap::compatcall(method_id, data);
}
