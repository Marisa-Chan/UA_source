#ifndef WINTIMER_H_INCLUDED
#define WINTIMER_H_INCLUDED

#include "itimer.h"

struct __NC_STACK_wintimer
{
    uint32_t ticks;
};

class NC_STACK_wintimer: public NC_STACK_itimer
{
public:
    virtual int itimer_func64();

    NC_STACK_wintimer() {
        memset(&stack__wintimer, 0, sizeof(stack__wintimer));
    };
    virtual ~NC_STACK_wintimer() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_wintimer();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_wintimer stack__wintimer;
};

#endif // WINTIMER_H_INCLUDED
