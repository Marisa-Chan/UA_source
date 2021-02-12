#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include <deque>
#include "system/system.h"
#include "display.h"
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

class NC_STACK_win3d: public NC_STACK_display
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t raster_func192(IDVPair *);
    virtual size_t raster_func198(const Common::FLine &arg);
    virtual size_t raster_func199(const Common::Line &arg);
    virtual size_t raster_func200(const Common::FLine &arg);
    virtual size_t raster_func201(const Common::Line &r);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDat *arg);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void BeginScene();
    virtual void EndScene();
    virtual void raster_func218(rstr_218_arg *arg);
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void display_func262(rstr_262_arg *arg);

    virtual bool AllocTexture(ResBitmap *arg);
    virtual void FreeTexture(ResBitmap *arg);
    virtual void display_func271(IDVPair *stak);
    virtual void display_func272(IDVPair *);
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

    virtual void setWDD_cursor(int arg);
    virtual void setWDD_disLowRes(int arg);
    virtual void setWDD_16bitTex(int arg);
    virtual void setWDD_drawPrim(int arg);

    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();

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

protected:
    GLdouble _frustum[16];
    float _frustumNear;
    float _frustumFar;
};


#endif //WIN3D_H_INCLUDED
