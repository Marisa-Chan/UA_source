#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypatank.h"


stored_functions *classvtbl_get_ypatank();
class_return * class_set_ypatank(int, ...);

stored_functions ypatank_class_vtbl(class_set_ypatank);


class_stored ypatank_class_off (NULL, NULL, "MC2classes:ypatank.class", classvtbl_get_ypatank);


stored_functions *classvtbl_get_ypatank()
{
    return &ypatank_class_vtbl;
}

CLASSFUNC ypatank_funcs[1024];




NC_STACK_ypatank * ypatank_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypatank *obj = (NC_STACK_ypatank *)call_parent(zis, clss, 0, stak);

    //field_24 = 2;

    return obj;
}

size_t ypatank_func1(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypatank_func2(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypatank_func3(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void ypatank_func70(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func70");
    //call_parent(zis, obj, 70, arg);
}

void ypatank_func71(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func71");
    call_parent(zis, obj, 71, arg);
}

void ypatank_func74(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func74");
    call_parent(zis, obj, 74, arg);
}

void ypatank_func80(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func80");
    call_parent(zis, obj, 80, arg);
}

void ypatank_func83(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func83");
    call_parent(zis, obj, 83, arg);
}

void ypatank_func87(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func87");
    call_parent(zis, obj, 87, arg);
}

void ypatank_func88(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func88");
    call_parent(zis, obj, 88, arg);
}

void ypatank_func96(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func96");
    call_parent(zis, obj, 96, arg);
}

void ypatank_func101(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func101");
    call_parent(zis, obj, 101, arg);
}

void ypatank_func111(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func111");
    call_parent(zis, obj, 111, arg);
}

void ypatank_func114(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func114");
    call_parent(zis, obj, 114, arg);
}

void ypatank_func128(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func128");
    call_parent(zis, obj, 128, arg);
}

void ypatank_func129(NC_STACK_ypatank *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypatank_func129");
    call_parent(zis, obj, 129, arg);
}



class_return ypatank_class_descr;

class_return * class_set_ypatank(int , ...)
{

    memset(ypatank_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypatank_class_descr.parent = "ypabact.class";

    ypatank_funcs[0] = (CLASSFUNC)ypatank_func0;
    ypatank_funcs[1] = (CLASSFUNC)ypatank_func1;
    ypatank_funcs[2] = (CLASSFUNC)ypatank_func2;
    ypatank_funcs[3] = (CLASSFUNC)ypatank_func3;
    ypatank_funcs[70] = (CLASSFUNC)ypatank_func70;
    ypatank_funcs[71] = (CLASSFUNC)ypatank_func71;
    ypatank_funcs[74] = (CLASSFUNC)ypatank_func74;
    ypatank_funcs[80] = (CLASSFUNC)ypatank_func80;
    ypatank_funcs[83] = (CLASSFUNC)ypatank_func83;
    ypatank_funcs[87] = (CLASSFUNC)ypatank_func87;
    ypatank_funcs[88] = (CLASSFUNC)ypatank_func88;
    ypatank_funcs[96] = (CLASSFUNC)ypatank_func96;
    ypatank_funcs[101] = (CLASSFUNC)ypatank_func101;
    ypatank_funcs[111] = (CLASSFUNC)ypatank_func111;
    ypatank_funcs[114] = (CLASSFUNC)ypatank_func114;
    ypatank_funcs[128] = (CLASSFUNC)ypatank_func128;
    ypatank_funcs[129] = (CLASSFUNC)ypatank_func129;

    ypatank_class_descr.vtbl = ypatank_funcs;
    ////ypatank_class_descr.varSize = sizeof(__NC_STACK_ypatank);
    ypatank_class_descr.varSize = sizeof(NC_STACK_ypatank) - offsetof(NC_STACK_ypatank, stack__ypatank); //// HACK
    ypatank_class_descr.field_A = 0;
    return &ypatank_class_descr;
}
