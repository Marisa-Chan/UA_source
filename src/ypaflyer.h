#ifndef YFLYER_H_INCLUDED
#define YFLYER_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypaflyer_class_off;

struct NC_STACK_ypaflyer;

struct __NC_STACK_ypaflyer
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    float field_c;
    int field_10;
};

struct NC_STACK_ypaflyer : public NC_STACK_ypabact
{
    __NC_STACK_ypaflyer stack__ypaflyer;
};

#endif // YFLYER_H_INCLUDED
