#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "utils.h"


const NewClassDescr NC_STACK_display::description("display.class", &newinstance);


size_t NC_STACK_display::func0(stack_vals *stak)
{
    if ( !NC_STACK_raster::func0(stak) )
        return 0;

    engines.display___win3d = this;

    return 1;
}

size_t NC_STACK_display::func1(stack_vals *stak)
{
    engines.display___win3d = NULL;
    return NC_STACK_raster::func1(stak);
}

size_t NC_STACK_display::func2(stack_vals *stak)
{
    stack_vals *val = find_id_in_stack2(0x80002007, stak);
    if ( val )
    {
        if ( val->value )
        {
            rstr_261_arg arg_261;
            rstr_262_arg arg_262;

            arg_261.pal_entries = (UA_PALENTRY *)val->value;
            arg_261.pal_id = 0;
            arg_261.entrie_id = 0;
            arg_261.pal_num = 256;

            display_func261(&arg_261);

            int v11 = 0;
            int v12 = 256;
            arg_262.pdword4 = &v11;
            arg_262.dword0 = 1;
            arg_262.pdword8 = &v12;

            display_func262(&arg_262);
        }
    }

    return NC_STACK_raster::func2(stak);
}

size_t NC_STACK_display::func3(stack_vals *stak)
{
    __NC_STACK_display *displ = &this->stack__display;

    stack_vals *val = find_id_in_stack2(0x80002007, stak);
    if ( val )
    {
        *(UA_PALENTRY **)val->value = displ->palette;
        val->id = 1;
    }

    return NC_STACK_raster::func3(stak);
}


void NC_STACK_display::display_func261(rstr_261_arg *arg)
{
    __NC_STACK_display *displ = &this->stack__display;

    memcpy(&displ->field_300[arg->pal_id].pal_entries[arg->entrie_id], arg->pal_entries, sizeof(UA_PALENTRY) * arg->pal_num);
}

void NC_STACK_display::display_func262(rstr_262_arg *arg)
{
    __NC_STACK_display *displ = &this->stack__display;

    for (int i = 0; i < 256; i++)
    {
        int tmpr = 0;
        int tmpg = 0;
        int tmpb = 0;

        for (int j = 0; j < arg->dword0; j++)
        {
            UA_PALETTE *pal = &displ->field_300[ arg->pdword4[j] ];
            tmpr += arg->pdword8[j] * pal->pal_entries[i].r;
            tmpg += arg->pdword8[j] * pal->pal_entries[i].g;
            tmpb += arg->pdword8[j] * pal->pal_entries[i].b;
        }

        tmpr >>= 8;
        tmpg >>= 8;
        tmpb >>= 8;

        if (tmpr > 255)
            tmpr = 255;
        if (tmpg > 255)
            tmpg = 255;
        if (tmpb > 255)
            tmpb = 255;

        displ->palette[i].r = tmpr;
        displ->palette[i].g = tmpg;
        displ->palette[i].b = tmpb;
    }
}

void NC_STACK_display::display_func263(displ_arg263 *arg)
{
    __NC_STACK_display *displ = &this->stack__display;

    display_func265(NULL);
    displ->pointer_bitm = arg->bitm;
    display_func264(NULL);
}

void NC_STACK_display::display_func264(void *)
{
    __NC_STACK_display *displ = &this->stack__display;
    displ->field_1b04 &= 0xFFFFFFFE;
}

void NC_STACK_display::display_func265(void *)
{
    __NC_STACK_display *displ = &this->stack__display;
    displ->field_1b04 |= 1;
}


size_t NC_STACK_display::display_func266(bitmap_intern **pbitm)
{
    bitmap_intern *bitm = *pbitm;

    bitm->pitch = bitm->width;
    bitm->buffer = AllocVec(bitm->width * bitm->height, 65537);
    bitm->flags |= 2;
    return bitm->buffer != NULL;
}

void NC_STACK_display::display_func267(bitmap_intern **)
{
}

void NC_STACK_display::display_func268(bitmap_intern **pbitm)
{
    bitmap_intern *bitm = *pbitm;
    if (bitm->buffer)
    {
        nc_FreeMem(bitm->buffer);
        bitm->buffer = NULL;
    }
}

size_t NC_STACK_display::display_func269(bitmap_intern **)
{
    return 1;
}

void NC_STACK_display::display_func270(bitmap_intern **)
{
}

UA_PALENTRY * NC_STACK_display::display_func273(rstr_261_arg *arg)
{
    __NC_STACK_display *displ = &this->stack__display;

    arg->pal_entries = displ->field_300[arg->pal_id].pal_entries;
    return arg->pal_entries;
}

void NC_STACK_display::display_func274(const char **)
{
    dprintf("MAKE ME %s(Save pcx screenshot)\n","display_func274");
}


size_t NC_STACK_display::compatcall(int method_id, void *data)
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
    case 261:
        display_func261( (rstr_261_arg *)data );
        return 1;
    case 262:
        display_func262( (rstr_262_arg *)data );
        return 1;
    case 263:
        display_func263( (displ_arg263 *)data );
        return 1;
    case 264:
        display_func264( (void *)data );
        return 1;
    case 265:
        display_func265( (void *)data );
        return 1;
    case 266:
        return (size_t)display_func266( (bitmap_intern **)data );
    case 267:
        display_func267( (bitmap_intern **)data );
        return 1;
    case 268:
        display_func268( (bitmap_intern **)data );
        return 1;
    case 269:
        return (size_t)display_func269( (bitmap_intern **)data );
    case 270:
        display_func270( (bitmap_intern **)data );
        return 1;
    case 273:
        return (size_t)display_func273( (rstr_261_arg *)data );
    case 274:
        display_func274( (const char **)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_raster::compatcall(method_id, data);
}
