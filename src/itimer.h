#ifndef ITIMER_H_INCLUDED
#define ITIMER_H_INCLUDED

#include "nucleas.h"

extern class_stored itimer_class_off;

struct NC_STACK_itimer;

struct __NC_STACK_itimer
{

};

struct NC_STACK_itimer : public NC_STACK_nucleus
{
    __NC_STACK_itimer stack__itimer;
};

#endif // ITIMER_H_INCLUDED
