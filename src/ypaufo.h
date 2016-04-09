#ifndef YUFO_H_INCLUDED
#define YUFO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypaufo_class_off;

struct NC_STACK_ypaufo;

struct __NC_STACK_ypaufo
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    float field_c;
    float field_10;
    float field_14;
    float field_18;
    int field_1c;
};

struct NC_STACK_ypaufo : public NC_STACK_ypabact
{
    __NC_STACK_ypaufo stack__ypaufo;
};

#endif // YUFO_H_INCLUDED
