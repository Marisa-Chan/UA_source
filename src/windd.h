#ifndef WINDD_H_INCLUDED
#define WINDD_H_INCLUDED

#include <ddraw.h>
#include <d3d.h>

extern class_stored windd_class_off;

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
	int field_54______rsrc_field4;
	windd_intern *intern;
};

struct __attribute__((packed)) LOGPALETTE256
{
    LOGPALETTE pal;
    PALETTEENTRY entrs[255];
};

struct NC_STACK_windd : public NC_STACK_display
{
	__NC_STACK_windd stack__windd;
};


#endif // RASTER_H_INCLUDED
