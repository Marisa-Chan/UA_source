#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "nucleas.h"

extern class_stored network_class_off;

struct NC_STACK_network;

struct __NC_STACK_network
{
    int dummy;
};

struct NC_STACK_network : public NC_STACK_nucleus
{
    __NC_STACK_network stack__network;
};

#endif // NETWORK_H_INCLUDED
