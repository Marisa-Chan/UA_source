#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

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
    ResBitmap *bitm_intern;
    tUtV * outline_coords;
    int flags;
};



struct bitmap_arg130
{
    int time_stmp;
    int frame_time;
    ResBitmap *pbitm;
    tUtV *outline;
};

class NC_STACK_bitmap: public NC_STACK_rsrc
{
    
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual size_t bitmap_func128(IDVPair *);
    virtual size_t bitmap_func129(IDVPair *);
    virtual void bitmap_func130(bitmap_arg130 *out);

    NC_STACK_bitmap() {
        memset(&stack__bitmap, 0, sizeof(stack__bitmap));
    };
    virtual ~NC_STACK_bitmap() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_bitmap();
    };

    enum BMD_ATT
    {
        BMD_ATT_OUTLINE = 0x80002001,
        BMD_ATT_WIDTH = 0x80002002,
        BMD_ATT_HEIGHT = 0x80002003,
        BMD_ATT_HAS_COLORMAP = 0x80002006,
        BMD_ATT_PCOLORMAP = 0x80002007,
        BMD_ATT_CONVCOLOR = 0x80002008,
    };

    //Set
    virtual void setBMD_outline(pixel_2d *);
    virtual void setBMD_palette(UA_PALETTE *);

    //Get
    virtual ResBitmap  * GetResBmp();
    virtual int getBMD_width();
    virtual int getBMD_height();
    virtual int getBMD_hasPalette();
    virtual UA_PALETTE *getBMD_palette();
    
    virtual SDL_Surface * GetSwTex();


    int sub_416704(pixel_2d *a3);
    
    virtual void PrepareTexture( bool force = false );

public:
    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_bitmap stack__bitmap;
};

#endif // BITMAP_H_INCLUDED
