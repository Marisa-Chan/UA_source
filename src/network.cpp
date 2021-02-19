#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "network.h"



const Nucleus::ClassDescr NC_STACK_network::description("network.class", &newinstance);



size_t NC_STACK_network::Init(IDVList &stak)
{
    if ( !NC_STACK_nucleus::Init(stak) )
        return 0;

    return 1;
}

size_t NC_STACK_network::Deinit()
{
    return NC_STACK_nucleus::Deinit();
}

