#ifndef ILBM_H_INCLUDED
#define ILBM_H_INCLUDED

#include "bitmap.h"

struct __NC_STACK_ilbm
{
    char field_0;
    char field_1;
    char field_2;
    char field_3;
};


struct __attribute__((packed)) BMHD_type
{
    WORD width;
    WORD height;
    WORD x;
    WORD y;
    char nPlanes;
    char masking;
    char compression;
    char pad1;
    WORD transparentColor;
    char xAspect;
    char yAspect;
    WORD pageWidth;
    WORD pageHeight;
};

struct __attribute__((packed)) VBMP_type
{
    WORD width;
    WORD height;
    WORD pad1;
};

class NC_STACK_ilbm: public NC_STACK_bitmap
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **pmfile);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ilbm() {
        memset(&stack__ilbm, 0, sizeof(stack__ilbm));
    };
    virtual ~NC_STACK_ilbm() {};

    virtual const char * getClassName() {
        return "ilbm.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ilbm();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_ilbm stack__ilbm;
};

#endif // ILBM_H_INCLUDED
