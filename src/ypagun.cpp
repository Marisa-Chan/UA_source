#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypagun.h"


stored_functions *classvtbl_get_ypagun();
class_return * class_set_ypagun(int, ...);

stored_functions ypagun_class_vtbl(class_set_ypagun);


class_stored ypagun_class_off (NULL, NULL, "MC2classes:ypagun.class", classvtbl_get_ypagun);


stored_functions *classvtbl_get_ypagun()
{
    return &ypagun_class_vtbl;
}

CLASSFUNC ypagun_funcs[1024];




NC_STACK_ypagun * ypagun_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypagun *obj = (NC_STACK_ypagun *)call_parent(zis, clss, 0, stak);

    //field_24 = 9;

    return obj;
}

size_t ypagun_func1(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypagun_func2(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypagun_func3(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void ypagun_func70(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func70");
    call_parent(zis, obj, 70, arg);
}

void ypagun_func71(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func71");
    call_parent(zis, obj, 71, arg);
}

void ypagun_func75(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func75");
    call_parent(zis, obj, 75, arg);
}

void ypagun_func77(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func77");
    call_parent(zis, obj, 77, arg);
}

void ypagun_func80(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func80");
    call_parent(zis, obj, 80, arg);
}

void ypagun_func82(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func82");
    call_parent(zis, obj, 82, arg);
}

void ypagun_func96(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func96");
    call_parent(zis, obj, 96, arg);
}

void ypagun_func111(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func111");
    call_parent(zis, obj, 111, arg);
}

void ypagun_func128(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func128");
    call_parent(zis, obj, 128, arg);
}

void ypagun_func129(NC_STACK_ypagun *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypagun_func129");
    call_parent(zis, obj, 129, arg);
}


class_return ypagun_class_descr;

class_return * class_set_ypagun(int , ...)
{

    memset(ypagun_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypagun_class_descr.parent = "ypabact.class";

    ypagun_funcs[0] = (CLASSFUNC)ypagun_func0;
    ypagun_funcs[1] = (CLASSFUNC)ypagun_func1;
    ypagun_funcs[2] = (CLASSFUNC)ypagun_func2;
    ypagun_funcs[3] = (CLASSFUNC)ypagun_func3;
    ypagun_funcs[70] = (CLASSFUNC)ypagun_func70;
    ypagun_funcs[71] = (CLASSFUNC)ypagun_func71;
    ypagun_funcs[75] = (CLASSFUNC)ypagun_func75;
    ypagun_funcs[77] = (CLASSFUNC)ypagun_func77;
    ypagun_funcs[80] = (CLASSFUNC)ypagun_func80;
    ypagun_funcs[82] = (CLASSFUNC)ypagun_func82;
    ypagun_funcs[96] = (CLASSFUNC)ypagun_func96;
    ypagun_funcs[111] = (CLASSFUNC)ypagun_func111;
    ypagun_funcs[128] = (CLASSFUNC)ypagun_func128;
    ypagun_funcs[129] = (CLASSFUNC)ypagun_func129;

    ypagun_class_descr.vtbl = ypagun_funcs;
    ////ypagun_class_descr.varSize = sizeof(__NC_STACK_ypagun);
    ypagun_class_descr.varSize = sizeof(NC_STACK_ypagun) - offsetof(NC_STACK_ypagun, stack__ypagun); //// HACK
    ypagun_class_descr.field_A = 0;
    return &ypagun_class_descr;
}
