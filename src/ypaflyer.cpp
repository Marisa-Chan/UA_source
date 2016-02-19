#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypaflyer.h"


stored_functions *classvtbl_get_ypaflyer();
class_return * class_set_ypaflyer(int, ...);

stored_functions ypaflyer_class_vtbl(class_set_ypaflyer);


class_stored ypaflyer_class_off (NULL, NULL, "MC2classes:ypaflyer.class", classvtbl_get_ypaflyer);


stored_functions *classvtbl_get_ypaflyer()
{
    return &ypaflyer_class_vtbl;
}

CLASSFUNC ypaflyer_funcs[1024];




NC_STACK_ypaflyer * ypaflyer_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypaflyer *obj = (NC_STACK_ypaflyer *)call_parent(zis, clss, 0, stak);

    //field_24 = 6;

    return obj;
}

size_t ypaflyer_func1(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypaflyer_func2(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypaflyer_func3(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void ypaflyer_func70(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func70");
    call_parent(zis, obj, 70, arg);
}

void ypaflyer_func71(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func71");
    call_parent(zis, obj, 71, arg);
}

void ypaflyer_func74(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func74");
    call_parent(zis, obj, 74, arg);
}

void ypaflyer_func80(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func80");
    call_parent(zis, obj, 80, arg);
}

void ypaflyer_func96(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func96");
    call_parent(zis, obj, 96, arg);
}

void ypaflyer_func97(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypaflyer_func97");
    call_parent(zis, obj, 97, arg);
}


class_return ypaflyer_class_descr;

class_return * class_set_ypaflyer(int , ...)
{

    memset(ypaflyer_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypaflyer_class_descr.parent = "ypabact.class";

    ypaflyer_funcs[0] = (CLASSFUNC)ypaflyer_func0;
    ypaflyer_funcs[1] = (CLASSFUNC)ypaflyer_func1;
    ypaflyer_funcs[2] = (CLASSFUNC)ypaflyer_func2;
    ypaflyer_funcs[3] = (CLASSFUNC)ypaflyer_func3;
    ypaflyer_funcs[70] = (CLASSFUNC)ypaflyer_func70;
    ypaflyer_funcs[71] = (CLASSFUNC)ypaflyer_func71;
    ypaflyer_funcs[74] = (CLASSFUNC)ypaflyer_func74;
    ypaflyer_funcs[80] = (CLASSFUNC)ypaflyer_func80;
    ypaflyer_funcs[96] = (CLASSFUNC)ypaflyer_func96;
    ypaflyer_funcs[97] = (CLASSFUNC)ypaflyer_func97;

    ypaflyer_class_descr.vtbl = ypaflyer_funcs;
    ////ypaflyer_class_descr.varSize = sizeof(__NC_STACK_ypaflyer);
    ypaflyer_class_descr.varSize = sizeof(NC_STACK_ypaflyer) - offsetof(NC_STACK_ypaflyer, stack__ypaflyer); //// HACK
    ypaflyer_class_descr.field_A = 0;
    return &ypaflyer_class_descr;
}
