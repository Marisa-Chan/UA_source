#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
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




class_return ypamissile_class_descr;

class_return * class_set_ypamissile(int , ...)
{

    memset(ypamissile_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypamissile_class_descr.parent = "ypabact.class";

    ypamissile_funcs[0] = (CLASSFUNC)ypamissile_func0;
    ypamissile_funcs[1] = (CLASSFUNC)ypamissile_func1;
    ypamissile_funcs[2] = (CLASSFUNC)ypamissile_func2;
    ypamissile_funcs[3] = (CLASSFUNC)ypamissile_func3;

    ypamissile_class_descr.vtbl = ypamissile_funcs;
    ////ypamissile_class_descr.varSize = sizeof(__NC_STACK_ypamissile);
    ypamissile_class_descr.varSize = sizeof(NC_STACK_ypamissile) - offsetof(NC_STACK_ypamissile, stack__ypamissile); //// HACK
    ypamissile_class_descr.field_A = 0;
    return &ypamissile_class_descr;
}
