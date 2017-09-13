#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "network.h"



const NewClassDescr NC_STACK_network::description("network.class", &newinstance);



size_t NC_STACK_network::func0(IDVList *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_network::func1()
{
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_network::func2(IDVList *stak)
{
    return NC_STACK_nucleus::func2(stak);
}

size_t NC_STACK_network::func3(IDVList *stak)
{
    return NC_STACK_nucleus::func3(stak);
}



size_t NC_STACK_network::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        func2( (IDVList *)data );
        return 1;
    case 3:
        func3( (IDVList *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
