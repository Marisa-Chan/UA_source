#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "network.h"



const Nucleus::ClassDescr NC_STACK_network::description("network.class", &newinstance);



size_t NC_STACK_network::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_network::func1()
{
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_network::func2(IDVList &stak)
{
    return NC_STACK_nucleus::func2(stak);
}

size_t NC_STACK_network::func3(IDVList &stak)
{
    return NC_STACK_nucleus::func3(stak);
}
