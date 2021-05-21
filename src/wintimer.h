#ifndef WINTIMER_H_INCLUDED
#define WINTIMER_H_INCLUDED

#include "itimer.h"

class NC_STACK_wintimer: public NC_STACK_itimer
{
public:
    virtual int itimer_func64();

    NC_STACK_wintimer() {};
    virtual ~NC_STACK_wintimer() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_wintimer();
    };

    //Data
    static const Nucleus::ClassDescr description;

public:
    uint32_t ticks = 0;
};

#endif // WINTIMER_H_INCLUDED
