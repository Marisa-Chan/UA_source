#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypamissile.h"


stored_functions *classvtbl_get_ypamissile();
class_return * class_set_ypamissile(int, ...);

stored_functions ypamissile_class_vtbl(class_set_ypamissile);


class_stored ypamissile_class_off (NULL, NULL, "MC2classes:ypamissile.class", classvtbl_get_ypamissile);


stored_functions *classvtbl_get_ypamissile()
{
    return &ypamissile_class_vtbl;
}

CLASSFUNC ypamissile_funcs[1024];




NC_STACK_ypamissile * ypamissile_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypamissile *obj = (NC_STACK_ypamissile *)call_parent(zis, clss, 0, stak);

    //field_24 = 4;

    return obj;
}

size_t ypamissile_func1(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypamissile_func2(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypamissile_func3(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void ypamissile_func68(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func68");
    call_parent(zis, obj, 68, arg);
}

void ypamissile_func69(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func69");
    call_parent(zis, obj, 69, arg);
}

void ypamissile_func70(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func70");
    call_parent(zis, obj, 70, arg);
}

void ypamissile_func71(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func71");
    call_parent(zis, obj, 71, arg);
}

void ypamissile_func74(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func74");
    call_parent(zis, obj, 74, arg);
}

void ypamissile_func78(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func78");
    call_parent(zis, obj, 78, arg);
}

void ypamissile_func96(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func96");
    call_parent(zis, obj, 96, arg);
}

void ypamissile_func119(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func119");
    call_parent(zis, obj, 119, arg);
}

void ypamissile_func128(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func128");
    call_parent(zis, obj, 128, arg);
}

void ypamissile_func129(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func129");
    call_parent(zis, obj, 129, arg);
}

void ypamissile_func130(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func130");
    call_parent(zis, obj, 130, arg);
}

void ypamissile_func131(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypamissile_func131");
    call_parent(zis, obj, 131, arg);
}


class_return ypamissile_class_descr;

class_return * class_set_ypamissile(int , ...)
{

    memset(ypamissile_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypamissile_class_descr.parent = "ypabact.class";

    ypamissile_funcs[0] = (CLASSFUNC)ypamissile_func0;
    ypamissile_funcs[1] = (CLASSFUNC)ypamissile_func1;
    ypamissile_funcs[2] = (CLASSFUNC)ypamissile_func2;
    ypamissile_funcs[3] = (CLASSFUNC)ypamissile_func3;
    ypamissile_funcs[68] = (CLASSFUNC)ypamissile_func68;
    ypamissile_funcs[69] = (CLASSFUNC)ypamissile_func69;
    ypamissile_funcs[70] = (CLASSFUNC)ypamissile_func70;
    ypamissile_funcs[71] = (CLASSFUNC)ypamissile_func71;
    ypamissile_funcs[74] = (CLASSFUNC)ypamissile_func74;
    ypamissile_funcs[78] = (CLASSFUNC)ypamissile_func78;
    ypamissile_funcs[96] = (CLASSFUNC)ypamissile_func96;
    ypamissile_funcs[119] = (CLASSFUNC)ypamissile_func119;
    ypamissile_funcs[128] = (CLASSFUNC)ypamissile_func128;
    ypamissile_funcs[129] = (CLASSFUNC)ypamissile_func129;
    ypamissile_funcs[130] = (CLASSFUNC)ypamissile_func130;
    ypamissile_funcs[131] = (CLASSFUNC)ypamissile_func131;

    ypamissile_class_descr.vtbl = ypamissile_funcs;
    ////ypamissile_class_descr.varSize = sizeof(__NC_STACK_ypamissile);
    ypamissile_class_descr.varSize = sizeof(NC_STACK_ypamissile) - offsetof(NC_STACK_ypamissile, stack__ypamissile); //// HACK
    ypamissile_class_descr.field_A = 0;
    return &ypamissile_class_descr;
}
