#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
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




class_return ypagun_class_descr;

class_return * class_set_ypagun(int , ...)
{

    memset(ypagun_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypagun_class_descr.parent = "ypabact.class";

    ypagun_funcs[0] = (CLASSFUNC)ypagun_func0;
    ypagun_funcs[1] = (CLASSFUNC)ypagun_func1;
    ypagun_funcs[2] = (CLASSFUNC)ypagun_func2;
    ypagun_funcs[3] = (CLASSFUNC)ypagun_func3;

    ypagun_class_descr.vtbl = ypagun_funcs;
    ////ypagun_class_descr.varSize = sizeof(__NC_STACK_ypagun);
    ypagun_class_descr.varSize = sizeof(NC_STACK_ypagun) - offsetof(NC_STACK_ypagun, stack__ypagun); //// HACK
    ypagun_class_descr.field_A = 0;
    return &ypagun_class_descr;
}
