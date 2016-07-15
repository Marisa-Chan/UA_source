#ifndef ENGINE_GFX_H_INCLUDED
#define ENGINE_GFX_H_INCLUDED

#include <inttypes.h>
#include <ddraw.h>
#include "engine_tform.h"

class NC_STACK_win3d;
class NC_STACK_bitmap;

struct tile_xy
{
    int byteoff;
    int width;
};

struct texStru;

struct __attribute__((packed)) UA_PALENTRY
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct __attribute__((packed)) UA_PALETTE
{
    UA_PALENTRY pal_entries[256];
};

struct bitmap_intern
{
    void *buffer;
    __int16 width;
    __int16 height;
    int pitch;
    UA_PALETTE *pallete;
    char flags;
    char field_11;
    __int16 field_12;
    texStru *ddrawSurfTex;
    int field_18;
    int field_1C;
};

struct tiles_stru
{
    NC_STACK_bitmap *font_image;
    bitmap_intern *field_4;
    void  *field_8;
    tile_xy *chars;
    int font_height;
    int field_12;
    int field_14;
};


struct rstr_207_arg
{
    tiles_stru *tiles;
    int id;
};

struct w3d_a209
{
    char *cmdbuf;
    char **includ;
};

struct tUtV
{
    float tu;
    float tv;
};

struct polysDatSub
{
    int renderFlags;
    int vertexCount;
    xyz *vertexes;
    tUtV *tu_tv;
    float *color;
    bitmap_intern *pbitm;
};



struct wdd_font_st1
{
    const char *string;
    int p1;
    int p2;
    int p3;
    int p4;
    int flag;
};

struct wdd_font
{
    IDirectDraw *ddraw;
    IDirectDrawSurface *ddsurf;
    HFONT hFont;
    int height;
    HDC hDC;
    int strings_count;
    wdd_font_st1 field_18[64];
    int TextColor;
};

struct gfx_window
{
    HWND hwnd;
    int width;
    int height;
};


class GFXEngine
{
public:
    GFXEngine(): cls3D(NULL) {};
    int init();
    void deinit();
    int getScreenH();
    int getScreenW();
    NC_STACK_win3d *getC3D();
    gfx_window *getWindow();

    void setResolution(int res);
    void setTracyRmp(bitmap_intern *rmp);
    void setShadeRmp(bitmap_intern *rmp);

    int loadPal(const char *palette_ilbm);

    void setTileset(tiles_stru *tileset, int id);
    tiles_stru * getTileset(int id);
    void drawText(w3d_a209 *arg);

    static void defRenderFunc(void *dat);

private:
    int sub_422CE8(const char *display, const char *display2, int gfxmode);

//Data

private:
    NC_STACK_win3d *cls3D;
};

extern GFXEngine GFXe;

#endif // ENGINE_GFX_H_INCLUDED
