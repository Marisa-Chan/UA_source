#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yparobo.h"


stored_functions *classvtbl_get_yparobo();
class_return * class_set_yparobo(int, ...);

stored_functions yparobo_class_vtbl(class_set_yparobo);


class_stored yparobo_class_off (NULL, NULL, "MC2classes:yparobo.class", classvtbl_get_yparobo);


stored_functions *classvtbl_get_yparobo()
{
    return &yparobo_class_vtbl;
}

CLASSFUNC yparobo_funcs[1024];




NC_STACK_yparobo * yparobo_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_yparobo *obj = (NC_STACK_yparobo *)call_parent(zis, clss, 0, stak);

    return obj;
}

size_t yparobo_func1(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void yparobo_func2(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void yparobo_func3(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}




class_return yparobo_class_descr;

class_return * class_set_yparobo(int , ...)
{

    memset(yparobo_funcs, 0, sizeof(CLASSFUNC) * 1024);

    yparobo_class_descr.parent = "ypabact.class";

    yparobo_funcs[0] = (CLASSFUNC)yparobo_func0;
    yparobo_funcs[1] = (CLASSFUNC)yparobo_func1;
    yparobo_funcs[2] = (CLASSFUNC)yparobo_func2;
    yparobo_funcs[3] = (CLASSFUNC)yparobo_func3;

    yparobo_class_descr.vtbl = yparobo_funcs;
    ////yparobo_class_descr.varSize = sizeof(__NC_STACK_yparobo);
    yparobo_class_descr.varSize = sizeof(NC_STACK_yparobo) - offsetof(NC_STACK_yparobo, stack__yparobo); //// HACK
    yparobo_class_descr.field_A = 0;
    return &yparobo_class_descr;
}
