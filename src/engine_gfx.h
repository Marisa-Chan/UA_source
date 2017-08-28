#ifndef ENGINE_GFX_H_INCLUDED
#define ENGINE_GFX_H_INCLUDED

#include <inttypes.h>
#include "wrapSDL.h"
#include "engine_tform.h"

#define GFX_MAX_VERTEX 12

class NC_STACK_win3d;
class NC_STACK_bitmap;

struct tile_xy
{
    int byteoff;
    int width;
};

struct UA_PALETTE
{
    SDL_Color pal_entries[256];
};

struct bitmap_intern
{
    void *buffer;
    int16_t width;
    int16_t height;
    int16_t pitch;
    UA_PALETTE *pallete;
    uint32_t flags;
    SDL_Surface *swTex;
    GLuint hwTex;
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
    xyz vertexes[GFX_MAX_VERTEX];
    tUtV tu_tv[GFX_MAX_VERTEX];
    float color[GFX_MAX_VERTEX];
    float r;
    float g;
    float b;
    float distance[GFX_MAX_VERTEX];
    bitmap_intern *pbitm;
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
public:
    static GFXEngine GFXe;

private:
    NC_STACK_win3d *cls3D;
};

#endif // ENGINE_GFX_H_INCLUDED
