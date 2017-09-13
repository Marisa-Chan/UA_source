#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "nucleas.h"

struct __NC_STACK_network
{
    int dummy;
};

class NC_STACK_network: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_network() {
        memset(&stack__network, 0, sizeof(stack__network));
    };
    virtual ~NC_STACK_network() {};

    virtual const char * getClassName() {
        return "network.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_network();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_network stack__network;
};

#endif // NETWORK_H_INCLUDED
