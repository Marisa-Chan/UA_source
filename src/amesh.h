#ifndef AMESH_H_INCLUDED
#define AMESH_H_INCLUDED

#include <inttypes.h>
#include "area.h"


struct __attribute__((packed)) ATTS
{
    int16_t field_0;
    uint8_t field_2;
    uint8_t field_3;
    uint8_t field_4;
    uint8_t field_5;
};

struct __NC_STACK_amesh
{
    NC_STACK_bitmap *ilbm1;
    NC_STACK_bitmap *ilbm2;
    int16_t cnt;
    int16_t field_A;
    ATTS *atts;
    tUtV **olpl;
//    int field_14;
};

class NC_STACK_amesh: public NC_STACK_area
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
    NC_STACK_amesh() {
        memset(&stack__amesh, 0, sizeof(stack__amesh));
    };
    virtual ~NC_STACK_amesh() {};

    virtual const char * getClassName() {
        return "amesh.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_amesh();
    };

    enum AMESH_FLAG
    {
        AMESH_FLAG_DPTHFADE = 1
    };

    enum AMESH_ATT
    {
        AMESH_ATT_NUMPOLY = 0x80003000,
        AMESH_ATT_ATTPOLYS = 0x80003001,
        AMESH_ATT_OTLPOOL = 0x80003002
    };


    virtual void setADE_depthFade(int);
    virtual void setAREA_bitm(NC_STACK_bitmap *);
    virtual void setAREA_tracybitm(NC_STACK_bitmap *);

    virtual void setAMESH_numpoly(int);
    virtual int setAMESH_polys(ATTS *);
    virtual int setAMESH_otls(tUtV **);

    virtual int getAMESH_numpoly();


    int amesh_func0__sub0(stack_vals *stak);
    void amesh_func2__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    __NC_STACK_amesh stack__amesh;
};

#endif // AMESH_H_INCLUDED
