#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include "wrapSDL.h"

struct bitmap_intern;

struct texStru
{
    SDL_Surface *sdlSurface;
    GLuint oTexture;
};

struct wind3d_sub1
{
    polysDatSub *polyData;
    texStru *tex;
    float field_8;
};

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

struct win3d_bigdata
{
    int dat_1C14_count;
    wind3d_sub1 dat_1C14[512];

    float gray_colors__[9][3];
};















struct bitmap_intern;

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
    void *surface_locked_surfaceData;
    SDL_Surface *screenSurface;
    int currentCursor;
    int surface_locked_pitch;
    int forcesoftcursor;
    int movie_player;
    int field_38;
    int txt16bit;
    int use_simple_d3d;
    int disable_lowres;
    int export_window_mode;
    int sort_id;
    int flags;

    win3d_bigdata *bigdata;
    int dither;
    int filter;
    int antialias;
    int alpha;
    int zbuf_when_tracy;
    int colorkey;

    SDL_PixelFormat *pixfmt;
    uint32_t colorKey;
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
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t raster_func192(stack_vals *);
    virtual size_t raster_func198(w3d_func198arg *arg);
    virtual size_t raster_func199(w3d_func199arg *arg);
    virtual size_t raster_func200(w3d_func198arg *arg);
    virtual size_t raster_func201(w3d_func199arg *arg);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDatSub *arg);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void BeginScene();
    virtual void EndScene();
    virtual void LockSurface();
    virtual void UnlockSurface();
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t display_func256(windd_arg256 *inout);
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void display_func261(rstr_261_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
    virtual size_t AllocTexture(bitmap_intern *arg);
    virtual void TextureApplyPalette(bitmap_intern *arg);
    virtual void FreeTexture(bitmap_intern *arg);
    virtual size_t LockTexture(bitmap_intern *arg);
    virtual void UnlockTexture(bitmap_intern *arg);
    virtual void display_func271(stack_vals *stak);
    virtual void display_func272(stack_vals *);
    virtual void display_func274(const char **name);
    virtual void windd_func320(stack_vals *);
    virtual void windd_func321(stack_vals *);
    virtual void windd_func322(windd_dlgBox *dlgBox);
    virtual void windd_func323(const char **filename);
    virtual void windd_func324(wdd_func324arg *inout);
    virtual void windd_func325(wdd_func324arg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_win3d() {
        _setFrustumClip(1.0, 10.0);
        //memset(&stack__win3d, 0, sizeof(stack__win3d));
    };
    virtual ~NC_STACK_win3d() {};

    virtual const char * getClassName() {
        return "win3d.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_win3d();
    };

    enum WDD_ATT
    {
        WDD_ATT_CURSOR = 0x80005000,
        WDD_ATT_TEXFILT = 0x80005001,
        WDD_ATT_DIS_LOWRES = 0x80005002,
        WDD_ATT_16BIT_TEX = 0x80005003,
        WDD_ATT_DRAW_PRIM = 0x80005004
    };

    virtual void setWDD_cursor(int arg);
    virtual void setWDD_disLowRes(int arg);
    virtual void setWDD_16bitTex(int arg);
    virtual void setWDD_drawPrim(int arg);

    virtual int getDISP_displID();
    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();

    // windd methods
    size_t windd_func0(stack_vals *stak);

    virtual void setW3D_texFilt(int arg);

    void draw2DandFlush();


    static void initfirst();
    int load_font(const char *fontname);
    void matrixAspectCorrection(mat3x3 *inout, bool invert);
    void getAspectCorrection(float &cW, float &cH, bool invert);

    void setFrustumClip(float near, float far);

protected:
    int initPolyEngine();
    int initPixelFormats();
    void SetRenderStates(int arg);
    void sb_0x43b518(polysDatSub *polysDat, texStru *tex, int a5, int a6);
    void RenderTransparent();
    void DrawScreenText();
    void AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag);
    void DrawTextEntry(const ScreenText *txt);

    void win3d_func209__sub0(tiles_stru **tiles, char *cmdline, char **arr);

    static SDL_Cursor *wrapLoadCursor(const char *name);

    void _setFrustumClip(float near, float far);

public:
    //Data
    static const NewClassDescr description;

    __NC_STACK_win3d stack__win3d;

protected:
    GLdouble frustum[16];
    float frustumNear;
    float frustumFar;
};


#endif //WIN3D_H_INCLUDED
