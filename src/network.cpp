#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "network.h"


stored_functions *classvtbl_get_network();
class_return * class_set_network(int, ...);

stored_functions network_class_vtbl(class_set_network);


class_stored network_class_off (NULL, NULL, "MC2classes:network.class", classvtbl_get_network);


stored_functions *classvtbl_get_network()
{
    return &network_class_vtbl;
}

CLASSFUNC network_funcs[1024];




NC_STACK_network * network_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_network *obj = (NC_STACK_network *)call_parent(zis, clss, 0, stak);

    return obj;
}

size_t network_func1(NC_STACK_network *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void network_func2(NC_STACK_network *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void network_func3(NC_STACK_network *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}




class_return network_class_descr;

class_return * class_set_network(int , ...)
{

    memset(network_funcs, 0, sizeof(CLASSFUNC) * 1024);

    network_class_descr.parent = "nucleus.class";

    network_funcs[0] = (CLASSFUNC)network_func0;
    network_funcs[1] = (CLASSFUNC)network_func1;
    network_funcs[2] = (CLASSFUNC)network_func2;
    network_funcs[3] = (CLASSFUNC)network_func3;

    network_class_descr.vtbl = network_funcs;
    ////network_class_descr.varSize = sizeof(__NC_STACK_network);
    network_class_descr.varSize = sizeof(NC_STACK_network) - offsetof(NC_STACK_network, stack__network); //// HACK
    network_class_descr.field_A = 0;
    return &network_class_descr;
}
