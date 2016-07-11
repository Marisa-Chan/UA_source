#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include <ddraw.h>
#include <d3d.h>

struct bitmap_intern;

union unitex
{
    IDirect3DTexture *dx1;
    IDirect3DTexture2 *dx2;
};

struct texStru
{
    IDirectDrawSurface *surface;
    unitex texture;
    IDirectDrawPalette *palette;
};

struct texCache
{
    int used;
    int field_4;
    IDirectDrawSurface *ddDrawSurface;
    IDirect3DTexture *d3dtex;
    IDirect3DTexture2 *d3dtex2;
    IDirectDrawPalette *ddrawPal;
    unitex loadedFromTexture;
    texStru *txStru;
};


struct wind3d_sub1
{
    polysDatSub *polyData;
    texStru *tex;
    float field_8;
};

struct wind3d_pixelformat
{
    int BytesPerColor;
    int color_mode;
    DWORD FFFF0000__color;
    int dwRBitMask;
    int dwRShift;
    int dwGBitMask;
    int dwGShift;
    int dwBBitMask;
    int dwBShift;
    int dwAlphaBitMask;
    int dwAShift;
    BYTE *colors;
};

struct rendState
{
    D3DRENDERSTATETYPE d3dRenderState;
    int value;
};

struct execBuf
{
    rendState rendStates[11];
    rendState rendStates2[11];
    int field_B0;
    void *lpInsStart;
    void *lpBufStart2;
    void *lpBufStart;
    void *lpBufEnd;
    void *lpInsPointer;
    void *lpBufStart3;
    int field_CC;
    int vertexCount;
    void *lpStates;
    DWORD rendStatesCount;
    void *field_DC;
    int field_E0;
};

struct win3d_bigdata
{
    float _dx;
    float _dy;
    int texCh_count;
    texCache texCh[32];

    int subt1_count;
    wind3d_sub1 subt1[512];

    int dat_1C14_count;
    wind3d_sub1 dat_1C14[512];

    wind3d_pixelformat primary__pixelformat;
    wind3d_pixelformat selected__pixelformat;
    int grey_gradient[255];
    int field_3870;

    execBuf rendStates;
    int sceneBeginned;

    float gray_colors__[9][3];
};

enum W3D_STATES
{
    TEXTUREHANDLE = 0x0,
    TEXTUREPERSPECTIVE = 0x1,
    SHADEMODE = 0x2,
    STIPPLEENABLE = 0x3,
    SRCBLEND = 0x4,
    DESTBLEND = 0x5,
    TEXTUREMAPBLEND = 0x6,
    ALPHABLENDENABLE = 0x7,
    ZWRITEENABLE = 0x8,
    TEXTUREMAG = 0x9,
    TEXTUREMIN = 0xA,
};


struct __NC_STACK_win3d
{
    win3d_bigdata *bigdata;
    int dither;
    int filter;
    int antialias;
    int field_10;
    int zbuf_when_tracy;
    int colorkey;
};

class NC_STACK_win3d: public NC_STACK_windd
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t raster_func198(w3d_func198arg *arg);
    virtual size_t raster_func199(w3d_func199arg *arg);
    virtual size_t raster_func200(w3d_func198arg *arg);
    virtual size_t raster_func201(w3d_func199arg *arg);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDatSub *arg);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void raster_func213(polysDatSub *);
    virtual size_t raster_func214(void *);
    virtual void raster_func215(void *);
    virtual void raster_func216(void *);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual size_t display_func266(bitmap_intern **arg);
    virtual void display_func267(bitmap_intern **arg);
    virtual void display_func268(bitmap_intern **arg);
    virtual size_t display_func269(bitmap_intern **arg);
    virtual void display_func270(bitmap_intern **arg);
    virtual void display_func271(stack_vals *stak);
    virtual void display_func272(stack_vals *);
    virtual void display_func274(const char **name);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_win3d() {
        memset(&stack__win3d, 0, sizeof(stack__win3d));
    };
    virtual ~NC_STACK_win3d() {};

    virtual const char * getClassName() {
        return "win3d.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_win3d();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_win3d stack__win3d;
};


#endif //WIN3D_H_INCLUDED
