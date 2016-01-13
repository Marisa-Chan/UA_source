#ifndef AREA_H_INCLUDED
#define AREA_H_INCLUDED

#include <inttypes.h>
#include "ade.h"
#include "bitmap.h"


extern class_stored area_class_off;

struct NC_STACK_area;

struct __NC_STACK_area
{
    NC_STACK_bitmap *ilbm1;
    NC_STACK_bitmap *ilbm2;
    int field_8;
    char field_c;
    char field_D;
    char field_E;
    char field_F;
    int field_10;
    __int16 field_14;
    __int16 field_16;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
};

struct NC_STACK_area : public NC_STACK_ade
{
    __NC_STACK_area stack__area;
};

struct __attribute__((packed)) AREA_STRC
{
    __int16 field_0;
    int field_2;
    int field_6;
};

#endif // AREA_H_INCLUDED
