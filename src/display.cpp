#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "utils.h"


stored_functions *classvtbl_get_display();
class_return * class_set_display(int, ...);

stored_functions display_class_vtbl(class_set_display);


class_stored display_class_off (NULL, NULL, "MC2classes:display.class", classvtbl_get_display);


stored_functions *classvtbl_get_display()
{
    return &display_class_vtbl;
}

CLASSFUNC display_funcs[1024];
g_engines *engines___display;


NC_STACK_display * display_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
    NC_STACK_display *clss = (NC_STACK_display *)call_parent(zis, obj, 0, stak); // raster_func0

    engines.display___win3d = clss;

    return clss;
}

size_t display_func1(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
{
    engines.display___win3d = NULL;
    return call_parent(zis, obj, 1, stak);
}

void display_func2(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
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

            call_method(obj, 261, &arg_261);

            int v11 = 0;
            int v12 = 256;
            arg_262.pdword4 = &v11;
            arg_262.dword0 = 1;
            arg_262.pdword8 = &v12;

            call_method(obj, 262,  &arg_262);
        }
    }

    call_parent(zis, obj, 2, stak);
}

void display_func3(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
{
    __NC_STACK_display *displ = &obj->stack__display;

    stack_vals *val = find_id_in_stack2(0x80002007, stak);
    if ( val )
    {
        *(UA_PALENTRY **)val->value = displ->palette;
        val->id = 1;
    }

    call_parent(zis, obj, 3, stak);
}


void display_func261(NC_STACK_display *obj, class_stru *, rstr_261_arg *arg)
{
    __NC_STACK_display *displ = &obj->stack__display;

    memcpy(&displ->field_300[arg->pal_id].pal_entries[arg->entrie_id], arg->pal_entries, sizeof(UA_PALENTRY) * arg->pal_num);
}

void display_func262(NC_STACK_display *obj, class_stru *, rstr_262_arg *arg)
{
    __NC_STACK_display *displ = &obj->stack__display;

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

void display_func263(NC_STACK_display *obj, class_stru *, displ_arg263 *arg)
{
    __NC_STACK_display *displ = &obj->stack__display;

    call_method(obj, 265, 0);
    displ->pointer_bitm = arg->bitm;
    call_method(obj, 264, 0);
}

void display_func264(NC_STACK_display *obj, class_stru *, void *)
{
    __NC_STACK_display *displ = &obj->stack__display;
    displ->field_1b04 &= 0xFFFFFFFE;
}

void display_func265(NC_STACK_display *obj, class_stru *, void *)
{
    __NC_STACK_display *displ = &obj->stack__display;
    displ->field_1b04 |= 1;
}


size_t display_func266(NC_STACK_display *, class_stru *, bitmap_intern **pbitm)
{
    bitmap_intern *bitm = *pbitm;

    bitm->pitch = bitm->width;
    bitm->buffer = AllocVec(bitm->width * bitm->height, 65537);
    bitm->flags |= 2;
    return bitm->buffer != NULL;
}

void display_func267(NC_STACK_display *, class_stru *, void *)
{
}

void display_func268(NC_STACK_display *, class_stru *, bitmap_intern **pbitm)
{
    bitmap_intern *bitm = *pbitm;
    if (bitm->buffer)
    {
        nc_FreeMem(bitm->buffer);
        bitm->buffer = NULL;
    }
}

size_t display_func269(NC_STACK_display *, class_stru *, void *)
{
    return 1;
}

void display_func270(NC_STACK_display *, class_stru *, void *)
{
}

UA_PALENTRY *display_func273(NC_STACK_display *obj, class_stru *, rstr_261_arg *arg)
{
    __NC_STACK_display *displ = &obj->stack__display;

    arg->pal_entries = displ->field_300[arg->pal_id].pal_entries;
    return arg->pal_entries;
}

size_t display_func274(void *, class_stru *, stack_vals *)
{
    printf("MAKE ME %s(Save pcx screenshot)\n","display_func274");
    return 0;
}


class_return display_class_descr;

void display_nullsub(void *, class_stru *, stack_vals *)
{
    ;
}

class_return * class_set_display(int a1, ...)
{

    stack_vals vals[128];

    if (a1 != 0)
    {
        va_list va;
        va_start(va, a1);

        va_to_arr(vals, 128, a1, va);

        va_end(va);
    }

    engines___display = (g_engines *)find_id_in_stack_def_val(0x80000002, 0, vals);

    memset(display_funcs, 0, sizeof(CLASSFUNC) * 1024);

    display_funcs[0] = (CLASSFUNC)display_func0;
    display_funcs[1] = (CLASSFUNC)display_func1;
    display_funcs[2] = (CLASSFUNC)display_func2;
    display_funcs[3] = (CLASSFUNC)display_func3;
    display_funcs[261] = (CLASSFUNC)display_func261;
    display_funcs[262] = (CLASSFUNC)display_func262;
    display_funcs[263] = (CLASSFUNC)display_func263;
    display_funcs[264] = (CLASSFUNC)display_func264;
    display_funcs[265] = (CLASSFUNC)display_func265;
    display_funcs[266] = (CLASSFUNC)display_func266;
    display_funcs[267] = (CLASSFUNC)display_func267;
    display_funcs[268] = (CLASSFUNC)display_func268;
    display_funcs[269] = (CLASSFUNC)display_func269;
    display_funcs[270] = (CLASSFUNC)display_func270;
    display_funcs[273] = (CLASSFUNC)display_func273;
    display_funcs[274] = (CLASSFUNC)display_func274;

    display_class_descr.parent = "raster.class";

    display_class_descr.vtbl = display_funcs;
    ////display_class_descr.varSize = sizeof(__NC_STACK_display);
    display_class_descr.varSize = sizeof(NC_STACK_display) - offsetof(NC_STACK_display, stack__display); //// HACK
    display_class_descr.field_A = 0;
    return &display_class_descr;
}
