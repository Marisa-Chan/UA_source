#ifndef AREA_H_INCLUDED
#define AREA_H_INCLUDED

#include <inttypes.h>
#include "ade.h"
#include "bitmap.h"


class NC_STACK_area;

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
    int16_t field_14;
    int16_t field_16;
    int field_18;
    int field_1c;
    int field_20;
    int field_24;
};


struct __attribute__((packed)) AREA_STRC
{
    int16_t field_0;
    int field_2;
    int field_6;
};


class NC_STACK_area: public NC_STACK_ade
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **file);
    virtual size_t ade_func65(area_arg_65 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_area() {
        memset(&stack__area, 0, sizeof(stack__area));
    };
    virtual ~NC_STACK_area() {};

    virtual const char * getClassName() {
        return "area.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_area();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_area stack__area;
};

#endif // AREA_H_INCLUDED
