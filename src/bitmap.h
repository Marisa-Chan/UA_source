#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"
#include "system/gfx.h"


class NC_STACK_bitmap: public NC_STACK_rsrc
{
    
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual void SetTime(int32_t timeStamp, int32_t frameTime) {};

    NC_STACK_bitmap() {};
    virtual ~NC_STACK_bitmap() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
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
    virtual void setBMD_palette(UA_PALETTE *);
    
    //Get
    virtual ResBitmap  * GetBitmap(int frameid = -1);
    virtual std::vector<tUtV> &GetOutline(int frameid = -1);
    virtual int getBMD_width();
    virtual int getBMD_height();
    virtual int getBMD_hasPalette();
    virtual UA_PALETTE *getBMD_palette();
    
    virtual SDL_Surface * GetSwTex();
    
    virtual bool IsDynamic() const { return false; };
    virtual uint32_t GetFramesCount() const { return 1; };
    virtual uint32_t GetCurrentFrameID() const { return 0; };

    
    virtual void PrepareTexture( bool force = false );

public:
    //Data
    static constexpr const char * __ClassName = "bitmap.class";

public:
    ResBitmap *bitm_intern = NULL;
    std::vector<tUtV> outline_coords;
};

#endif // BITMAP_H_INCLUDED
