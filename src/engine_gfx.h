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
    int width;
    int x;
    int y;
};

typedef std::array<SDL_Color, 256> UA_PALETTE;

struct ResBitmap
{    
    int16_t _width;
    int16_t _height;
    UA_PALETTE *_pallete;
    SDL_Surface *_swTex;
    GLuint _hwTex;
    
    ResBitmap()
    {
        _width = 0;
        _height = 0;
        _pallete = NULL;
        _swTex = NULL;
        _hwTex = 0;
    }
};

struct tiles_stru
{
    NC_STACK_bitmap *font_image;
    ResBitmap *field_4;
    tile_xy *chars;
    int font_height;
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

    w3d_a209() : cmdbuf(NULL), includ(NULL) {};
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
    vec3d vertexes[GFX_MAX_VERTEX];
    tUtV tu_tv[GFX_MAX_VERTEX];
    float color[GFX_MAX_VERTEX];
    float r;
    float g;
    float b;
    float distance[GFX_MAX_VERTEX];
    ResBitmap *pbitm;
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
    void setTracyRmp(ResBitmap *rmp);
    void setShadeRmp(ResBitmap *rmp);

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
