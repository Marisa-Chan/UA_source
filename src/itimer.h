#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

#include "nucleas.h"


class NC_STACK_itimer: public NC_STACK_nucleus
{
public:
    virtual int itimer_func64() {
        return 1;
    };

    NC_STACK_itimer() {};
    virtual ~NC_STACK_itimer() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };


    //Data
    static constexpr const char * __ClassName = "itimer.class";
};

#endif // ITIMER_H_INCLUDED
