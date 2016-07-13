#ifndef WINDD_H_INCLUDED
#define WINDD_H_INCLUDED

#include <ddraw.h>
#include <d3d.h>

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



void sub_42A640(__NC_STACK_windd *obj);

int dbcs_StartText();
void dbcs_DrawText(const char *string, int p1, int p2, int p3, int p4, char flag);
int dbcs_EndText(LPDDSURFACEDESC surfDesc);
void dbcs_AddText(const char *string, int p1, int p2, int p3, int p4, int flag);
int sb_0x4bf0a0(LPDDSURFACEDESC surf);
int load_font(const char *fontname);


class NC_STACK_windd: public NC_STACK_display
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t raster_func192(stack_vals *);
    virtual size_t raster_func193(bitmap_intern **stak);
    virtual void raster_func194(stack_vals *stak);
    virtual void raster_func195(stack_vals *stak);
    virtual void raster_func196(stack_vals *stak);
    virtual void raster_func197(stack_vals *stak);
    virtual size_t raster_func198(w3d_func198arg *stak);
    virtual size_t raster_func199(w3d_func199arg *stak);
    virtual size_t raster_func200(w3d_func198arg *stak);
    virtual size_t raster_func201(w3d_func199arg *stak);
    virtual size_t raster_func202(rstr_arg204 *stak);
    virtual size_t raster_func203(stack_vals *stak);
    virtual size_t raster_func204(rstr_arg204 *stak);
    virtual size_t raster_func205(stack_vals *stak);
    virtual size_t raster_func206(polysDatSub *stak);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void raster_func213(polysDatSub *stak);
    virtual size_t raster_func214(void *stak);
    virtual void raster_func215(void *stak);
    virtual void raster_func216(void *stak);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual size_t raster_func219(stack_vals *stak);
    virtual size_t display_func256(windd_arg256 *inout);
    virtual void display_func257(stack_vals *);
    virtual void display_func258(stack_vals *);
    virtual void display_func259(stack_vals *);
    virtual void display_func260(stack_vals *);
    virtual void display_func261(rstr_261_arg *arg);
    virtual void display_func262(rstr_262_arg *arg);
    virtual void display_func263(displ_arg263 *arg);
    virtual void windd_func320(stack_vals *);
    virtual void windd_func321(stack_vals *);
    virtual void windd_func322(windd_dlgBox *dlgBox);
    virtual void windd_func323(const char **filename);
    virtual void windd_func324(wdd_func324arg *inout);
    virtual void windd_func325(wdd_func324arg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_windd() {
        memset(&stack__windd, 0, sizeof(stack__windd));
    };
    virtual ~NC_STACK_windd() {};

    virtual const char * getClassName() {
        return "windd.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_windd();
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

    //Data
    static const NewClassDescr description;

    __NC_STACK_windd stack__windd;
};

#endif // RASTER_H_INCLUDED
