#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
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




class_return ypatank_class_descr;

class_return * class_set_ypatank(int , ...)
{

    memset(ypatank_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypatank_class_descr.parent = "ypabact.class";

    ypatank_funcs[0] = (CLASSFUNC)ypatank_func0;
    ypatank_funcs[1] = (CLASSFUNC)ypatank_func1;
    ypatank_funcs[2] = (CLASSFUNC)ypatank_func2;
    ypatank_funcs[3] = (CLASSFUNC)ypatank_func3;

    ypatank_class_descr.vtbl = ypatank_funcs;
    ////ypatank_class_descr.varSize = sizeof(__NC_STACK_ypatank);
    ypatank_class_descr.varSize = sizeof(NC_STACK_ypatank) - offsetof(NC_STACK_ypatank, stack__ypatank); //// HACK
    ypatank_class_descr.field_A = 0;
    return &ypatank_class_descr;
}
