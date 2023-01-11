#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "nucleas.h"



class NC_STACK_network: public NC_STACK_nucleus
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();

    NC_STACK_network() {

    };
    virtual ~NC_STACK_network() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };
    //Data
    static constexpr const char * __ClassName = "network.class";
};

#endif // NETWORK_H_INCLUDED
