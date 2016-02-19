#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypaufo.h"


stored_functions *classvtbl_get_ypaufo();
class_return * class_set_ypaufo(int, ...);

stored_functions ypaufo_class_vtbl(class_set_ypaufo);


class_stored ypaufo_class_off (NULL, NULL, "MC2classes:ypaufo.class", classvtbl_get_ypaufo);


stored_functions *classvtbl_get_ypaufo()
{
    return &ypaufo_class_vtbl;
}

CLASSFUNC ypaufo_funcs[1024];




NC_STACK_ypaufo * ypaufo_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypaufo *obj = (NC_STACK_ypaufo *)call_parent(zis, clss, 0, stak);

    //field_24 = 7;

    return obj;
}

size_t ypaufo_func1(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypaufo_func2(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypaufo_func3(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void ypaufo_func70(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaufo_func70");
    call_parent(zis, obj, 70, arg);
}

void ypaufo_func71(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaufo_func71");
    call_parent(zis, obj, 71, arg);
}

void ypaufo_func74(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaufo_func74");
    call_parent(zis, obj, 74, arg);
}

void ypaufo_func80(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaufo_func80");
    call_parent(zis, obj, 80, arg);
}

void ypaufo_func96(NC_STACK_ypaufo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaufo_func96");
    call_parent(zis, obj, 96, arg);
}


class_return ypaufo_class_descr;

class_return * class_set_ypaufo(int , ...)
{

    memset(ypaufo_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypaufo_class_descr.parent = "ypabact.class";

    ypaufo_funcs[0] = (CLASSFUNC)ypaufo_func0;
    ypaufo_funcs[1] = (CLASSFUNC)ypaufo_func1;
    ypaufo_funcs[2] = (CLASSFUNC)ypaufo_func2;
    ypaufo_funcs[3] = (CLASSFUNC)ypaufo_func3;
    ypaufo_funcs[70] = (CLASSFUNC)ypaufo_func70;
    ypaufo_funcs[71] = (CLASSFUNC)ypaufo_func71;
    ypaufo_funcs[74] = (CLASSFUNC)ypaufo_func74;
    ypaufo_funcs[80] = (CLASSFUNC)ypaufo_func80;
    ypaufo_funcs[96] = (CLASSFUNC)ypaufo_func96;

    ypaufo_class_descr.vtbl = ypaufo_funcs;
    ////ypaufo_class_descr.varSize = sizeof(__NC_STACK_ypaufo);
    ypaufo_class_descr.varSize = sizeof(NC_STACK_ypaufo) - offsetof(NC_STACK_ypaufo, stack__ypaufo); //// HACK
    ypaufo_class_descr.field_A = 0;
    return &ypaufo_class_descr;
}
