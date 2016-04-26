#ifndef YTANK_H_INCLUDED
#define YTANK_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypatank_class_off;

struct NC_STACK_ypatank;

struct __NC_STACK_ypatank
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    uint8_t field_c;
    int     field_D;
    xyz     field_11;
    float   field_1D;
    float   field_21;
    uint8_t field_25;
    int     field_26;
};

struct NC_STACK_ypatank : public NC_STACK_ypabact
{
    __NC_STACK_ypatank stack__ypatank;
};

struct tank_arg128
{
    float field_0;
    xyz field_4;
    int field_10;
    xyz field_14;
};

struct tank_arg129
{
    float field_0;
    xyz field_4;
};


#endif // YTANK_H_INCLUDED
