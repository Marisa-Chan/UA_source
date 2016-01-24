#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "ypabact.h"


stored_functions *classvtbl_get_ypabact();
class_return * class_set_ypabact(int, ...);

stored_functions ypabact_class_vtbl(class_set_ypabact);


class_stored ypabact_class_off (NULL, NULL, "MC2classes:ypabact.class", classvtbl_get_ypabact);


stored_functions *classvtbl_get_ypabact()
{
    return &ypabact_class_vtbl;
}

CLASSFUNC ypabact_funcs[1024];




NC_STACK_ypabact * ypabact_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypabact *obj = (NC_STACK_ypabact *)call_parent(zis, clss, 0, stak);

    return obj;
}

size_t ypabact_func1(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypabact_func2(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void ypabact_func3(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}




class_return ypabact_class_descr;

class_return * class_set_ypabact(int , ...)
{

    memset(ypabact_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypabact_class_descr.parent = "nucleus.class";

    ypabact_funcs[0] = (CLASSFUNC)ypabact_func0;
    ypabact_funcs[1] = (CLASSFUNC)ypabact_func1;
    ypabact_funcs[2] = (CLASSFUNC)ypabact_func2;
    ypabact_funcs[3] = (CLASSFUNC)ypabact_func3;

    ypabact_class_descr.vtbl = ypabact_funcs;
    ////ypabact_class_descr.varSize = sizeof(__NC_STACK_ypabact);
    ypabact_class_descr.varSize = sizeof(NC_STACK_ypabact) - offsetof(NC_STACK_ypabact, stack__ypabact); //// HACK
    ypabact_class_descr.field_A = 0;
    return &ypabact_class_descr;
}
