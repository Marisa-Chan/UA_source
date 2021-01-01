#ifndef ENGINE_GFX_H_INCLUDED
#define ENGINE_GFX_H_INCLUDED

#include <inttypes.h>
#include <array>
#include "wrapSDL.h"
#include "engine_tform.h"
#include "nucleas.h"

#define GFX_MAX_VERTEX 12

class NC_STACK_win3d;
class NC_STACK_bitmap;

typedef std::array<SDL_Color, 256> UA_PALETTE;

struct ResBitmap
{    
    int16_t width;
    int16_t height;
    UA_PALETTE *palette;
    SDL_Surface *swTex;
    GLuint hwTex;
    
    ResBitmap()
    {
        width = 0;
        height = 0;
        palette = NULL;
        swTex = NULL;
        hwTex = 0;
    }
};

struct TileMap
{
    NC_STACK_bitmap *img;
    std::array<Common::PointRect, 256> map;
    int h;
    
    TileMap();  
    ~TileMap();
    
    void Draw(SDL_Surface *surface, const Common::Point &pos, uint8_t c);
    void Draw(SDL_Surface *surface, const Common::PointRect &posRect, uint8_t c);
    void Draw(SDL_Surface *surface, const Common::Rect &posRect, uint8_t c);
    
    void Fill(SDL_Surface *surface, const Common::Rect &rect, uint8_t c);
    void Fill(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c);
    
    //Special fill use 1pix column
    void FillColumn(SDL_Surface *surface, const Common::Rect &rect, uint8_t c);
    void FillColumn(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c);
    
    int GetWidth(uint8_t c) const;
    int GetWidth(const std::string &str) const;
    Common::Point GetSize(uint8_t c) const;
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

    int loadPal(const std::string &palette_ilbm);

    void setTileset(TileMap *tileset, int id);
    TileMap * getTileset(int id);
    void drawText(w3d_a209 *arg);

    static void defRenderFunc(void *dat);

private:
    int sub_422CE8(const std::string &display, const std::string &display2, int gfxmode);

//Data
public:
    static GFXEngine GFXe;

private:
    NC_STACK_win3d *cls3D;
};

namespace GFX
{

SDL_Color Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);

}

#endif // ENGINE_GFX_H_INCLUDED
