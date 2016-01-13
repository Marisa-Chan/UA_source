#ifndef AMESH_H_INCLUDED
#define AMESH_H_INCLUDED

#include <inttypes.h>
#include "area.h"



extern class_stored amesh_class_off;

struct __attribute__((packed)) ATTS
{
    __int16 field_0;
    uint8_t field_2;
    uint8_t field_3;
    uint8_t field_4;
    uint8_t field_5;
};

struct __NC_STACK_amesh
{
    NC_STACK_bitmap *ilbm1;
    NC_STACK_bitmap *ilbm2;
    __int16 cnt;
    __int16 field_A;
    ATTS *atts;
    tUtV **olpl;
    int field_14;
};

struct NC_STACK_amesh : public NC_STACK_area
{
    __NC_STACK_amesh stack__amesh;
};

#endif // AMESH_H_INCLUDED
