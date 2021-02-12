#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "system/gfx.h"
#include "common.h"
#include "base.h"

struct rstr_arg204
{
    ResBitmap *pbitm = NULL;
    Common::FRect float4;
    Common::FRect float14;
};

struct rstr_218_arg
{
    ResBitmap *bitm_intern;
    ResBitmap *bitm_intern2;
    int flg;
    Common::FRect rect1;
    Common::FRect rect2;
};

struct rstr_262_arg
{
    int cnt;
    int *slot;
    int *weight;
};

struct displ_arg263
{
    ResBitmap *bitm;
    int pointer_id;
};

class NC_STACK_display: public NC_STACK_nucleus
{
protected:
    enum
    {
        DEFAULT_WIDTH = 640,
        DEFAULT_HEIGHT = 480,
    };
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t raster_func192(IDVPair *);
//    virtual size_t raster_func193(bitmap_intern **out);
    virtual size_t raster_func198(const Common::FLine &);
    virtual size_t raster_func199(const Common::Line &l);
    virtual size_t raster_func200(const Common::FLine &);
    virtual size_t raster_func201(const Common::Line &r);
    virtual size_t raster_func202(rstr_arg204 *);
    virtual size_t raster_func203(IDVPair *);
    virtual size_t raster_func204(rstr_arg204 *);
    virtual size_t raster_func205(IDVPair *);
    virtual size_t raster_func206(polysDat *);
    virtual void raster_func207(int id, TileMap *tiles);
    virtual TileMap *raster_func208(int id);
    virtual int raster_func208(TileMap *tiles);
    virtual void raster_func209(w3d_a209 *);
    virtual void raster_func210(const Common::FRect &arg);
    virtual void raster_func211(const Common::Rect &arg);
    virtual size_t raster_func212(IDVPair *);
    virtual void BeginScene();
    virtual void EndScene();
    virtual size_t raster_func217(uint32_t color);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t raster_func219(IDVPair *);
    virtual size_t raster_func220(IDVPair *);
    virtual void raster_func221(const Common::Rect &arg);

    virtual void BeginFrame() {};
    virtual void EndFrame() {};

    virtual void display_func261(int ID, UA_PALETTE &pal, int from, int num);
    virtual void display_func261(int ID, UA_PALETTE &pal);
    virtual void display_func262(rstr_262_arg *arg);
//    virtual void display_func264(void *);
//    virtual void display_func265(void *);
    virtual bool AllocTexture(ResBitmap *pbitm);
    virtual void FreeTexture(ResBitmap *pbitm);
    virtual void display_func271(IDVPair *stak) {};
    virtual void display_func272(IDVPair *) {};
    virtual UA_PALETTE * display_func273(int paletteId);
    virtual void SaveScreenshot(const std::string & screenName);

    NC_STACK_display();
    virtual ~NC_STACK_display() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return NULL;//new NC_STACK_display();
    };

    enum DISP_ATT
    {
        ATT_FGPEN       = 0x80001000,
        ATT_BGPEN       = 0x80001001,
        ATT_SHADE_RMP   = 0x80001002,
        ATT_TRACY_RMP   = 0x80001003,
        ATT_FGAPEN      = 0x80001004,
        ATT_WIDTH       = 0x80001005,
        ATT_HEIGHT      = 0x80001006,
        ATT_PALETTE     = 0x80001007,
        ATT_DISPLAY_ID  = 0x80001008,
        ATT_DISPLAY_INF = 0x80001009,
        ATT_DISPLAY_WIN = 0x8000100A,
    };

    enum RFLAGS
    {
        RFLAGS_LINMAP = 0x1,
        RFLAGS_PERSPMAP = 0x2,
        RFLAGS_FLATSHD = 0x4,
        RFLAGS_GRADSHD = 0x8,
        RFLAGS_ZEROTRACY = 0x10,
        RFLAGS_LUMTRACY = 0x20,
        RFLAGS_SKY = 0x40,
        RFLAGS_FALLOFF = 0x80,
        RFLAGS_IGNORE_FALLOFF = 0x1000,
    };

    //Set
    virtual void SetPalette(UA_PALETTE &pal);

    virtual void SetPen(uint32_t pen);
    virtual void setRSTR_BGpen(uint32_t pen);
    virtual void setRSTR_shdRmp(ResBitmap *rmp);
    virtual void setRSTR_trcRmp(ResBitmap *rmp);
    virtual void setRSTR_FGApen(uint32_t pen);

    //Get
    virtual UA_PALETTE *GetPalette();

    virtual int16_t GetWidth();
    virtual int16_t GetHeight();

    Common::Point GetSize() const { return Common::Point(_width, _height); };
    
    virtual bool ChangeResolution(Common::Point res, bool windowed = false);
    
    
    virtual void ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp) = 0;
    virtual SDL_PixelFormat *GetScreenFormat() = 0;
    virtual SDL_Surface *CreateSurfaceScreenFormat(int width, int height) = 0;
    virtual SDL_Surface *ConvertToScreenFormat(SDL_Surface *src) = 0;

protected:
    bool SetResolution(Common::Point res);

    //Data
public:
    static const Nucleus::ClassDescr description;
    
    uint32_t _field_4 = 0; // Color?
    Common::Rect _clip;
    Common::Rect _inverseClip;
    std::array<TileMap *, 256> _tiles;
    int _field_54c   = 0;
    int _field_550   = 0;
    float _field_554 = 0;
    float _field_558 = 0;
    
    UA_PALETTE _palette;
    std::array<UA_PALETTE, 8> _field_300;
    
protected:
    int16_t _width  = 0;
    int16_t _height = 0;
};

#endif // DISPLAY_H_INCLUDED
