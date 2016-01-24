#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"

extern class_stored ypabact_class_off;

struct NC_STACK_ypabact;

struct __NC_STACK_ypabact
{
    int energy;
    int energy_2;
};

struct NC_STACK_ypabact : public NC_STACK_nucleus
{
    __NC_STACK_ypabact stack__ypabact;
};

#endif // YBACT_H_INCLUDED
