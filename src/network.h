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
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();

    NC_STACK_network() {
        memset(&stack__network, 0, sizeof(stack__network));
    };
    virtual ~NC_STACK_network() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_network();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_network stack__network;
};

#endif // NETWORK_H_INCLUDED
