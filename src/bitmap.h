#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

struct texStru;

struct pixel_2d
{
    int x;
    int y;
    int32_t zbuff;
    uint32_t color;
    int mapx;
    int mapy;
    int16_t flags;
};

struct __NC_STACK_bitmap
{
    bitmap_intern *bitm_intern;
    tUtV * outline_coords;
    int flags;
};

enum BITMAP_FLAG
{
    BITMAP_FLAG_EXTDATA  = 0x1,
    BITMAP_FLAG_TEXTURE  = 0x2,
    BITMAP_FLAG_HAS_PALETTE  = 0x4,
    BITMAP_FLAG_SYSMEM  = 0x8,
    BITMAP_FLAG_TRANSP  = 0x10
};

struct bitmap_arg130
{
    int time_stmp;
    int frame_time;
    bitmap_intern *pbitm;
    tUtV *outline;
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

    enum BMD_ATT
    {
        BMD_ATT_PBITMAP = 0x80002000,
        BMD_ATT_OUTLINE = 0x80002001,
        BMD_ATT_WIDTH = 0x80002002,
        BMD_ATT_HEIGHT = 0x80002003,
        BMD_ATT_BUFFER = 0x80002005,
        BMD_ATT_HAS_COLORMAP = 0x80002006,
        BMD_ATT_PCOLORMAP = 0x80002007,
        BMD_ATT_TEXTURE = 0x80002008,
        BMD_ATT_TEXTURE_SYS = 0x80002009 // In system memory
    };

    //Set
    virtual void setBMD_outline(pixel_2d *);
    virtual void setBMD_palette(UA_PALETTE *);

    //Get
    virtual bitmap_intern * getBMD_pBitmap();
    virtual int getBMD_width();
    virtual int getBMD_height();
    virtual void *getBMD_buffer();
    virtual int getBMD_hasPalette();
    virtual UA_PALETTE *getBMD_palette();


    int sub_416704(pixel_2d *a3);

    //Data
    static const NewClassDescr description;

    __NC_STACK_bitmap stack__bitmap;
};

#endif // BITMAP_H_INCLUDED
