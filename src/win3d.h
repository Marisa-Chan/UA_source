#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include <deque>
#include "system/system.h"
#include "system/gfx.h"
#include "common.h"
#include "base.h"
#include "ini.h"

struct ResBitmap;
struct polysDat;

#define MSK(X) (1 << (X))

enum W3D_STATES
{
    TEXTUREHANDLE,
    SHADEMODE,
    STIPPLEENABLE,
    SRCBLEND,
    DESTBLEND,
    TEXTUREMAPBLEND,
    ALPHABLENDENABLE,
    ZWRITEENABLE,
    TEXTUREMAG,
    TEXTUREMIN,
    W3D_STATES_MAX
};


struct ResBitmap;

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

struct windd_dlgBox
{
    const char *title;
    const char *ok;
    const char *cancel;
    const char *startText;
    int time;
    void (*timer_func)(int, int, int);
    void *timer_context;
    int replace;
    int maxLen;
    char *result;
};


struct wdd_func324arg
{
    const char *name;
    const char *guid;
    int currr;
};

struct ScreenText
{
    const char *string;
    int p1;
    int p2;
    int p3;
    int p4;
    int flag;
};

struct ScreenFont
{
    TTF_Font *ttfFont;
    int height;
    uint8_t r, g, b;

    std::list<ScreenText *> entries;

    ScreenFont();
};

class NC_STACK_win3d: public NC_STACK_nucleus
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
    virtual size_t raster_func198(const Common::FLine &arg);
    virtual size_t raster_func199(const Common::Line &arg);
    virtual size_t raster_func200(const Common::FLine &arg);
    virtual size_t raster_func201(const Common::Line &r);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDat *arg);
    virtual void raster_func207(int id, TileMap *tiles);
    virtual TileMap *raster_func208(int id);
    virtual int raster_func208(TileMap *tiles);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void raster_func210(const Common::FRect &arg);
    virtual void raster_func211(const Common::Rect &arg);
    virtual void BeginScene();
    virtual void EndScene();
    virtual size_t raster_func217(SDL_Color color);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual void raster_func221(const Common::Rect &arg);
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void display_func261(int ID, UA_PALETTE &pal, int from, int num);
    virtual void display_func261(int ID, UA_PALETTE &pal);
    virtual void display_func262(rstr_262_arg *arg);

    virtual bool AllocTexture(ResBitmap *arg);
    virtual void FreeTexture(ResBitmap *arg);
    virtual void display_func271(IDVPair *stak);
    virtual void display_func272(IDVPair *);
    virtual UA_PALETTE * display_func273(int paletteId);
    virtual void SaveScreenshot(const std::string & screenName);
    virtual void windd_func320(IDVPair *);
    virtual void windd_func321(IDVPair *);
    virtual void windd_func322(windd_dlgBox *dlgBox);
    virtual void windd_func323(const char **filename);
    virtual void windd_func324(wdd_func324arg *inout);
    virtual void windd_func325(wdd_func324arg *arg);

    NC_STACK_win3d();
    virtual ~NC_STACK_win3d() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_win3d();
    };

    enum WDD_ATT
    {
        WDD_ATT_CURSOR = 0x80002000,
        WDD_ATT_TEXFILT = 0x80002001,
        WDD_ATT_DIS_LOWRES = 0x80002002,
        WDD_ATT_16BIT_TEX = 0x80002003,
        WDD_ATT_DRAW_PRIM = 0x80002004
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

    virtual void setWDD_cursor(int arg);
    virtual void setWDD_disLowRes(int arg);
    virtual void setWDD_16bitTex(int arg);
    virtual void setWDD_drawPrim(int arg);

    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();
    
    
    
    virtual void SetPalette(UA_PALETTE &pal);
    virtual void SetPen(SDL_Color pen);
    virtual void setRSTR_BGpen(uint32_t pen) {};
    virtual void setRSTR_shdRmp(ResBitmap *rmp) {};
    virtual void setRSTR_trcRmp(ResBitmap *rmp) {};
    virtual void setRSTR_FGApen(uint32_t pen) {};

    virtual UA_PALETTE *GetPalette();

    virtual int16_t GetWidth();
    virtual int16_t GetHeight();

    Common::Point GetSize() const { return Common::Point(_width, _height); };
    
    
    
    
    // windd methods
    size_t windd_func0(IDVList &stak);

    virtual void setW3D_texFilt(int arg);

    void draw2DandFlush();

    int LoadFontByDescr(const std::string &fontname);
    void matrixAspectCorrection(mat3x3 &inout, bool invert);
    void getAspectCorrection(float &cW, float &cH, bool invert);

    void setFrustumClip(float near, float far);

    static bool compare(polysDat *a, polysDat *b);
    
    void windd_func323__sub0(const char *filename);
    void win3dInitialisation();

    virtual void ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp);
    virtual SDL_PixelFormat *GetScreenFormat();
    virtual SDL_Surface *CreateSurfaceScreenFormat(int width, int height);
    virtual SDL_Surface *ConvertToScreenFormat(SDL_Surface *src);
    
    static SDL_Surface *ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt);
    
    virtual bool ChangeResolution(Common::Point res, bool windowed = false);

protected:
    void initPolyEngine();
    void initPixelFormats();
    void SetRenderStates(int arg);
    void sb_0x43b518(polysDat *polysDat, int a5, int a6);
    void RenderTransparent();
    void DrawScreenText();
    void AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag);
    void DrawTextEntry(const ScreenText *txt);

    void win3d_func209__sub0(char *cmdline, char **arr);
    void win3d_func262__sub0(int a2, int *a3, int *a4);

    static SDL_Cursor *wrapLoadCursor(const char *name);

    void _setFrustumClip(float near, float far);
    
    void sub_420EDC(Common::Line line, uint8_t r, uint8_t g, uint8_t b);
    char * windd_func322__sub0(const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, uint32_t timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen);
    
    void ApplyResolution();

    bool SetResolution(Common::Point res);

public:
    //Data
    static const Nucleus::ClassDescr description;

    static Common::Ini::KeyList win3d_keys;

public:
    int _forcesoftcursor;
    int _movie_player;
    int _field_38;
    int _txt16bit;
    int _use_simple_d3d;
    int _disable_lowres;
    int _export_window_mode;
    int _flags;

    // From bigdata
    std::deque<polysDat *> _pending;
    float _greyColors[9][3];
    // \From bigdata

    int _dither;
    int _filter;
    int _antialias;
    int _alpha;
    int _zbuf_when_tracy;
    int _colorkey;

    SDL_PixelFormat *_pixfmt;
    SDL_DisplayMode _mode;
    GLint _glPixfmt, _glPixtype;

    uint32_t _rendStates[W3D_STATES_MAX];
    uint32_t _rendStates2[W3D_STATES_MAX];

    int _sceneBeginned;

    ScreenFont _font;

    float _corrW, _corrIW;
    float _corrH, _corrIH;
    bool _solidFont;
    
    // Display class
    SDL_Color _field_4 = {0}; // Color?
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
    // Display class
    int16_t _width  = 0;
    int16_t _height = 0;

    // Win3d
    GLdouble _frustum[16];
    float _frustumNear;
    float _frustumFar;
};


#endif //WIN3D_H_INCLUDED
