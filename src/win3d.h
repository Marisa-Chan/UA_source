#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include <deque>
#include "wrapSDL.h"
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

struct __NC_STACK_win3d
{
    int width;
    int height;
    SDL_Surface *screenSurface;
    int currentCursor;
    int forcesoftcursor;
    int movie_player;
    int field_38;
    int txt16bit;
    int use_simple_d3d;
    int disable_lowres;
    int export_window_mode;
    int sort_id;
    int flags;

    // From bigdata
    std::deque<polysDat *> pending;
    float gray_colors__[9][3];
    // \From bigdata

    int dither;
    int filter;
    int antialias;
    int alpha;
    int zbuf_when_tracy;
    int colorkey;

    SDL_PixelFormat *pixfmt;
    SDL_DisplayMode mode;
    GLint glPixfmt, glPixtype;
    bool windowed;

    uint32_t rendStates[W3D_STATES_MAX];
    uint32_t rendStates2[W3D_STATES_MAX];

    int sceneBeginned;

    ScreenFont font;

    float corrW, corrIW;
    float corrH, corrIH;
    bool solidFont;

    __NC_STACK_win3d();
};

class NC_STACK_win3d: public NC_STACK_display
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t raster_func192(IDVPair *);
    virtual size_t raster_func198(w3d_func198arg *arg);
    virtual size_t raster_func199(w3d_func199arg *arg);
    virtual size_t raster_func200(w3d_func198arg *arg);
    virtual size_t raster_func201(w3d_func199arg *arg);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDat *arg);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void BeginScene();
    virtual void EndScene();
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t display_func256(windd_arg256 *inout);
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
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

    NC_STACK_win3d() {
        _setFrustumClip(1.0, 8192.0);
        //memset(&stack__win3d, 0, sizeof(stack__win3d));
    };
    virtual ~NC_STACK_win3d() {};
    
    virtual const std::string &GetClassName() const {
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

    virtual int getDISP_displID();
    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();

    // windd methods
    size_t windd_func0(IDVList &stak);

    virtual void setW3D_texFilt(int arg);

    void draw2DandFlush();


    static void initfirst();
    int load_font(const char *fontname);
    void matrixAspectCorrection(mat3x3 &inout, bool invert);
    void getAspectCorrection(float &cW, float &cH, bool invert);

    void setFrustumClip(float near, float far);

    static bool compare(polysDat *a, polysDat *b);

    virtual void ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp);
    virtual SDL_PixelFormat *GetScreenFormat();
    virtual SDL_Surface *CreateSurfaceScreenFormat(int width, int height);
    virtual SDL_Surface *ConvertToScreenFormat(SDL_Surface *src);

protected:
    int initPolyEngine();
    int initPixelFormats();
    void SetRenderStates(int arg);
    void sb_0x43b518(polysDat *polysDat, int a5, int a6);
    void RenderTransparent();
    void DrawScreenText();
    void AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag);
    void DrawTextEntry(const ScreenText *txt);

    void win3d_func209__sub0(TileMap **tiles, char *cmdline, char **arr);

    static SDL_Cursor *wrapLoadCursor(const char *name);

    void _setFrustumClip(float near, float far);
    
    void sub_420EDC(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b);

public:
    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_win3d stack__win3d;

    static Common::Ini::KeyList win3d_keys;

protected:
    GLdouble frustum[16];
    float frustumNear;
    float frustumFar;
};


#endif //WIN3D_H_INCLUDED
