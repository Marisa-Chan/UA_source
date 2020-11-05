#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "engine_gfx.h"
#include "common.h"
#include "base.h"

struct ua_dRect
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct ua_fRect
{
    float x1;
    float y1;
    float x2;
    float y2;
    
    ua_fRect(float sx1, float sy1, float sx2, float sy2)
    : x1(sx1), y1(sy1), x2(sx2), y2(sy2)
    {}

    ua_fRect()
    {
    	clear();
    }
    
    void clear()
    {
    	x1 = 0.;
    	y1 = 0.;
    	x2 = 0.;
    	y2 = 0.;
    }
};


struct __NC_STACK_display
{
    uint32_t field_4; // Color?
    Common::Rect _clip;
    Common::Rect _inverseClip;
    std::array<TileMap *, 256> tiles;
    int field_54c;
    int field_550;
    float field_554;
    float field_558;
    
    UA_PALETTE palette;
    UA_PALETTE field_300[8];
};




struct w3d_func198arg
{
    float x1;
    float y1;
    float x2;
    float y2;
};

struct w3d_func199arg
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct rstr_arg204
{
    ResBitmap *pbitm;
    float float4;
    float float8;
    float floatC;
    float float10;
    float float14;
    float float18;
    float float1C;
    float float20;

    rstr_arg204()
    {
    	clear();
    }

    void clear()
    {
    	pbitm = NULL;
		float4 = 0.;
		float8 = 0.;
		floatC = 0.;
		float10 = 0.;
		float14 = 0.;
		float18 = 0.;
		float1C = 0.;
		float20 = 0.;
    }
};

struct rstr_loc204
{
    ResBitmap *pbitm;
    int dword4;
    int dword8;
    int dwordC;
    int dword10;
    int dword14;
    int dword18;
    int dword1C;
    int dword20;
    int dword24;
};

struct rstr_218_arg
{
    ResBitmap *bitm_intern;
    ResBitmap *bitm_intern2;
    int flg;
    ua_fRect rect1;
    ua_fRect rect2;
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

struct windd_arg256
{
    int sort_id;
    int width;
    int height;
    char name[32];
};

class NC_STACK_display: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t raster_func192(IDVPair *);
//    virtual size_t raster_func193(bitmap_intern **out);
    virtual size_t raster_func198(w3d_func198arg *);
    virtual size_t raster_func199(w3d_func199arg *);
    virtual size_t raster_func200(w3d_func198arg *);
    virtual size_t raster_func201(w3d_func199arg *);
    virtual size_t raster_func202(rstr_arg204 *);
    virtual size_t raster_func203(IDVPair *);
    virtual size_t raster_func204(rstr_arg204 *);
    virtual size_t raster_func205(IDVPair *);
    virtual size_t raster_func206(polysDat *);
    virtual void raster_func207(int id, TileMap *tiles);
    virtual TileMap *raster_func208(int id);
    virtual int raster_func208(TileMap *tiles);
    virtual void raster_func209(w3d_a209 *);
    virtual void raster_func210(ua_fRect *arg);
    virtual void raster_func211(ua_dRect *arg);
    virtual size_t raster_func212(IDVPair *);
    virtual void BeginScene();
    virtual void EndScene();
    virtual size_t raster_func217(uint32_t color);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t raster_func219(IDVPair *);
    virtual size_t raster_func220(IDVPair *);
    virtual void raster_func221(ua_dRect *arg);
    virtual size_t display_func256(windd_arg256 *inout) {
        return 0;
    };
    virtual void BeginFrame() {};
    virtual void EndFrame() {};

    virtual void display_func261(int ID, UA_PALETTE &pal, int from, int num);
    virtual void display_func261(int ID, UA_PALETTE &pal);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
//    virtual void display_func264(void *);
//    virtual void display_func265(void *);
    virtual bool AllocTexture(ResBitmap *pbitm);
    virtual void FreeTexture(ResBitmap *pbitm);
    virtual void display_func271(IDVPair *stak) {};
    virtual void display_func272(IDVPair *) {};
    virtual UA_PALETTE * display_func273(int paletteId);
    virtual void SaveScreenshot(const std::string & screenName);

    NC_STACK_display() {
        memset(&stack__display, 0, sizeof(stack__display));
    };
    virtual ~NC_STACK_display() {};
    
    virtual const std::string &GetClassName() const {
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
        ATT_DISPLAY_INF = 0x80001009
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
    
    
    virtual void ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp) = 0;
    virtual SDL_PixelFormat *GetScreenFormat() = 0;
    virtual SDL_Surface *CreateSurfaceScreenFormat(int width, int height) = 0;
    virtual SDL_Surface *ConvertToScreenFormat(SDL_Surface *src) = 0;

    //Data
public:
    static const Nucleus::ClassDescr description;

    __NC_STACK_display stack__display;

protected:
    int16_t _width;
    int16_t _height;
};

#endif // DISPLAY_H_INCLUDED
