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













struct bitmap_intern;

struct enum_gfx_modes
{
    int width;
    int height;
    int bits;
    int unk;
};

struct _devices
{
    D3DDEVICEDESC dev_descr;
    GUID driver_guid;
    GUID device_guid;
    int has_driver_guid;
    int has_device_guid;
    char driver_descript[256];
    char driver_name[256];
    char device_name[256];
    char device_descr[256];
    int unk0_def_1;
    int unk1_def_0;
    int zbuf_use;
    int can_srcblend;
    int can_stippling;
    int unk2_def_0;
    int textures_in_sysmem;
    int can_colorkey;
    int can_destblend;
    int zbuf_bit_depth;
};

struct windd_params
{
    int field_0;
    unsigned int gfx_modes_count;
    enum_gfx_modes gfx_modes[64];
    DDSURFACEDESC displ_mode_surface;
    DDSURFACEDESC ddSurfDescr__primary;
    int video_mem;
    _devices selected_device;
    int device_selected_id;
    int number_of_devices;
    _devices enum_devices_[32];
    int field_AD04;
};

struct mode_node : public nnode
{
    int sort_id;
    int width;
    int height;
    char name[128];
    int field_94;
    int rel_width;
    int rel_height;
    int bits;
    int field_A4;
    int field_A8;
    int field_AC;
    int field_B0;
};

struct windd_formats
{
    DDSURFACEDESC surf_descr;
    int use_clut;
    int rbits;
    int gbits;
    int bbits;
    int abits;
    int rgbbitcount;
    int vosem;
};

struct windd_intern
{
    IDirectDrawSurface *z_buf;
    IDirect3DDevice *d3d1Dev;
    IDirect3DViewport2 *d3d2Viewport;
    IDirect3DExecuteBuffer *executebuffer;
    IDirect3DMaterial2 *material;
    IDirect3DDevice2 *d3d2dev;
    int formats_count;
    int selected_format_id;
    windd_formats formats[32];
};

struct wddDevice: public nnode
{
    char name[256];
    char guid[256];
    int curr;
};



struct __attribute__((packed)) LOGPALETTE256
{
    LOGPALETTE pal;
    PALETTEENTRY entrs[255];
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



extern windd_params dd_params;


struct __NC_STACK_windd;
void sub_42A640(__NC_STACK_windd *obj);

int dbcs_StartText();
void dbcs_DrawText(const char *string, int p1, int p2, int p3, int p4, char flag);
int dbcs_EndText(LPDDSURFACEDESC surfDesc);
void dbcs_AddText(const char *string, int p1, int p2, int p3, int p4, int flag);
int sb_0x4bf0a0(LPDDSURFACEDESC surf);
int load_font(const char *fontname);














struct __NC_STACK_windd
{
    HWND hwnd;
    HINSTANCE cursor;
    IDirectDrawSurface *primary_surf;
    IDirectDrawSurface *back_surf;
    IDirectDrawSurface *field_10;
    IDirectDrawClipper *clipper;
    IDirectDrawPalette *ddrawPal;
    int width;
    int height;
    void *surface_locked_surfaceData;
    int field_28;
    int surface_locked_pitch;
    int field_30;
    int movie_player;
    int field_38;
    int txt16bit;
    int use_simple_d3d;
    int disable_lowres;
    int export_window_mode;
    int sort_id;
    char field_50;
    char field_51;
    char field_52;
    char field_53;
    bitmap_intern *field_54______rsrc_field4;
    windd_intern *intern;
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
    virtual void raster_func213(polysDatSub *);
    virtual size_t raster_func214(void *);
    virtual void raster_func215(void *);
    virtual void raster_func216(void *);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t display_func256(windd_arg256 *inout);
    virtual void display_func257(stack_vals *);
    virtual void display_func258(stack_vals *);
    virtual void display_func261(rstr_261_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
    virtual size_t display_func266(bitmap_intern **arg);
    virtual void display_func267(bitmap_intern **arg);
    virtual void display_func268(bitmap_intern **arg);
    virtual size_t display_func269(bitmap_intern **arg);
    virtual void display_func270(bitmap_intern **arg);
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
        memset(&stack__win3d, 0, sizeof(stack__win3d));
        memset(&stack__windd, 0, sizeof(stack__windd));
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
    virtual gfx_window *getDISP_displInf();
    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();

    // windd methods
    size_t windd_func0(stack_vals *stak);
    size_t windd_func1(stack_vals *stak);
    static void sub_42D37C(__NC_STACK_windd *wdd, UA_PALETTE *pal);



    virtual void setW3D_texFilt(int arg);

    //Data
    static const NewClassDescr description;

    __NC_STACK_win3d stack__win3d;
    __NC_STACK_windd stack__windd;
};


#endif //WIN3D_H_INCLUDED
