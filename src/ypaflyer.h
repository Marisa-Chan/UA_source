#ifndef YFLYER_H_INCLUDED
#define YFLYER_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypaflyer_class_off;

struct NC_STACK_ypaflyer;

struct __NC_STACK_ypaflyer
{
};

struct NC_STACK_ypaflyer : public NC_STACK_ypabact
{
    __NC_STACK_ypaflyer stack__ypaflyer;
};

#endif // YFLYER_H_INCLUDED
