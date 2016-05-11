#ifndef YGUN_H_INCLUDED
#define YGUN_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypagun_class_off;

struct NC_STACK_ypagun;

struct __NC_STACK_ypagun
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_intern;
    float field_c;
    float field_10;
    float field_14;
    xyz dir;
    xyz field_24;
    char field_30;
    int field_31;
    int field_35;
    char field_39;
    int field_3A;
};

struct NC_STACK_ypagun : public NC_STACK_ypabact
{
    __NC_STACK_ypagun stack__ypagun;
};

struct gun_arg128
{
    int field_0;
    xyz dir;
};

struct gun_arg129
{
    xyz vec;
    xyz dir;
    float angle;
};

#endif // YGUN_H_INCLUDED
