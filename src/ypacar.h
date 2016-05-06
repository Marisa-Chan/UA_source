#ifndef YCAR_H_INCLUDED
#define YCAR_H_INCLUDED

#include "nucleas.h"
#include "ypatank.h"

extern class_stored ypacar_class_off;

struct NC_STACK_ypacar;

struct __NC_STACK_ypacar
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    int field_c;
    int field_10;
};

struct NC_STACK_ypacar : public NC_STACK_ypatank
{
    __NC_STACK_ypacar stack__ypacar;
};

#endif // YCAR_H_INCLUDED
