#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

struct texStru;

struct bitmap__opl
{
    __int16 field_0;
    __int16 field_2;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
    __int16 field_A;
    __int16 field_C;
    __int16 field_E;
};

union bitmap_uniopl
{
    bitmap__opl *opl1;
    tUtV *opl2;
};

struct __NC_STACK_bitmap
{
    bitmap_intern *bitm_intern;
    bitmap_uniopl field_4;
    int field_8;
};

enum BITMAP_FLAG
{
    BITMAP_FLAG_HAS_PALETTE  = 0x4,
    BITMAP_FLAG_SOFTWARE  = 0x8
};

struct bitmap_arg130
{
    int field_0;
    int field_4;
    bitmap_intern *pbitm;
    tUtV *opl2;
};

class NC_STACK_bitmap: public NC_STACK_rsrc
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func65(rsrc **pres);
    virtual size_t bitmap_func128(stack_vals *);
    virtual size_t bitmap_func129(stack_vals *);
    virtual void bitmap_func130(bitmap_arg130 *out);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_bitmap() {
        memset(&stack__bitmap, 0, sizeof(stack__bitmap));
    };
    virtual ~NC_STACK_bitmap() {};

    virtual const char * getClassName() {
        return "bitmap.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_bitmap();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_bitmap stack__bitmap;
};

#endif // BITMAP_H_INCLUDED
