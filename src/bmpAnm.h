#ifndef BMPANM_H_INCLUDED
#define BMPANM_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"
#include "bitmap.h"

struct texStru;

struct bmpAnim_t2
{
    tUtV *opls;
    bitmap_intern *bitm;
    int field_8;
    int16_t field_C;
    int16_t field_E;
};

struct bmpAnim_t1_objs
{
    NC_STACK_bitmap *clsObj;
    bitmap_intern *bitm_intern;
    char *title;
};

struct bmpAnim_t1
{
    char *titles;
    tUtV *field_4;
    bmpAnim_t1_objs *frames;
    char *className;
    bmpAnim_t2 *t2;
    bmpAnim_t2 *t2_end;
    int16_t t2_cnt;
    int16_t field_1A;
    int16_t field_1C;
    int16_t frame_count;
};

struct __NC_STACK_bmpanim
{
    bmpAnim_t1 *bmpanm_intern;
    bmpAnim_t2 *t2_;
    int field_8;
    int field_C;
    int16_t field_10;
    int16_t field_12;
};

struct __attribute__((packed)) bmpanm_loc
{
    int32_t a;
    int16_t frame_id;
    int16_t uv_id;
};

class NC_STACK_bmpanim: public NC_STACK_bitmap
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **file);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func65(rsrc **pres);
    virtual size_t rsrc_func66(rsrc_func66_arg *sv);
    virtual void bitmap_func130(bitmap_arg130 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_bmpanim() {
        memset(&stack__bmpanim, 0, sizeof(stack__bmpanim));
    };
    virtual ~NC_STACK_bmpanim() {};

    virtual const char * getClassName() {
        return "bmpanim.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_bmpanim();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_bmpanim stack__bmpanim;
};

#endif // BITMAP_H_INCLUDED
