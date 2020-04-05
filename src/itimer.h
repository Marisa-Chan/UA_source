#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

#include "nucleas.h"


struct __NC_STACK_itimer
{

};

class NC_STACK_itimer: public NC_STACK_nucleus
{
public:
    virtual int itimer_func64(void *) {
        return 1;
    };

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_itimer() {
        memset(&stack__itimer, 0, sizeof(stack__itimer));
    };
    virtual ~NC_STACK_itimer() {};

    virtual const char * getClassName() {
        return "itimer.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_itimer();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_itimer stack__itimer;
};

#endif // ITIMER_H_INCLUDED
