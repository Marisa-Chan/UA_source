#ifndef ENGINE_GFX_H_INCLUDED
#define ENGINE_GFX_H_INCLUDED

#include <array>
#include <math.h>
#include <inttypes.h>

#include "system.h"
#include "../vectors.h"
#include "../matrix.h"
#include "../IFFile.h"
#include "../nucleas.h"



class NC_STACK_win3d;
namespace Gui{ class OldCompat; };

typedef Tvec2d<double> vec2d;
typedef Tvec3d<double> vec3d;
typedef Tmat3x3<double, double> mat3x3;
typedef Tmat4x4<double, double> mat4x4;

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


class TFEngine
{
public:
    struct SinCos
    {
        float sin;
        float cos;
    };

    class TForm3D
    {
    public:
        enum
        {
            FLAG_FOLLOW_PARENT = 1, // Follow to parent transform
            FLAG_NO_WRLD_ROT   = 2  // Do not do rotation on world transformation stage, only translate. (Save performance)
        };
    public:
        void MakeScaleRotationMatrix();
        void CalcGlobal();

    public:
        TForm3D()
        {
            parent_1c = NULL;
            ax = 0;
            ay = 0;
            az = 0;
            rx = 0;
            ry = 0;
            rz = 0;
            flags = 0;
        };

        TForm3D *parent_1c;
        vec3d locPos;
        vec3d globPos;
        vec3d vec;
        vec3d scale;
        mat3x3 locSclRot;
        mat3x3 globSclRot;
        int32_t ax;
        int32_t ay;
        int32_t az;
        int32_t rx;
        int32_t ry;
        int32_t rz;
        uint32_t flags;
        mat4x4 tform;

    protected:
        void scale_rot_0();
        void scale_rot_1();
        void scale_rot_2();
        void scale_rot_3();
        void scale_rot_4();
        void scale_rot_5();
        void scale_rot_6();
        void scale_rot_7();
    };

public:
    int init();
    void deinit();

    inline SinCos& Angle(int angle) {
        return SinCos_table[angle];
    };
    inline TForm3D *GetViewPoint() {
        return viewPoint;
    };
    inline void SetViewPoint(TForm3D *pnt) {
        viewPoint = pnt;
    };

    inline static bool Vec3dReadIFF(vec3d &v, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(v.x);
            res &= iff->readFloatB(v.y);
            res &= iff->readFloatB(v.z);
        }
        else
        {
            res &= iff->readFloatL(v.x);
            res &= iff->readFloatL(v.y);
            res &= iff->readFloatL(v.z);
        }
        return res;
    }

    inline static bool Vec3dWriteIFF(const vec3d &v, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(v.x);
            res &= iff->writeFloatB(v.y);
            res &= iff->writeFloatB(v.z);
        }
        else
        {
            res &= iff->writeFloatL(v.x);
            res &= iff->writeFloatL(v.y);
            res &= iff->writeFloatL(v.z);
        }
        return res;
    }

    inline static bool Mat3x3ReadIFF(mat3x3 &m, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(m.m00);
            res &= iff->readFloatB(m.m01);
            res &= iff->readFloatB(m.m02);
            res &= iff->readFloatB(m.m10);
            res &= iff->readFloatB(m.m11);
            res &= iff->readFloatB(m.m12);
            res &= iff->readFloatB(m.m20);
            res &= iff->readFloatB(m.m21);
            res &= iff->readFloatB(m.m22);
        }
        else
        {
            res &= iff->readFloatL(m.m00);
            res &= iff->readFloatL(m.m01);
            res &= iff->readFloatL(m.m02);
            res &= iff->readFloatL(m.m10);
            res &= iff->readFloatL(m.m11);
            res &= iff->readFloatL(m.m12);
            res &= iff->readFloatL(m.m20);
            res &= iff->readFloatL(m.m21);
            res &= iff->readFloatL(m.m22);
        }
        return res;
    }

    inline static bool Mat3x3WriteIFF(const mat3x3 &m, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(m.m00);
            res &= iff->writeFloatB(m.m01);
            res &= iff->writeFloatB(m.m02);
            res &= iff->writeFloatB(m.m10);
            res &= iff->writeFloatB(m.m11);
            res &= iff->writeFloatB(m.m12);
            res &= iff->writeFloatB(m.m20);
            res &= iff->writeFloatB(m.m21);
            res &= iff->writeFloatB(m.m22);
        }
        else
        {
            res &= iff->writeFloatL(m.m00);
            res &= iff->writeFloatL(m.m01);
            res &= iff->writeFloatL(m.m02);
            res &= iff->writeFloatL(m.m10);
            res &= iff->writeFloatL(m.m11);
            res &= iff->writeFloatL(m.m12);
            res &= iff->writeFloatL(m.m20);
            res &= iff->writeFloatL(m.m21);
            res &= iff->writeFloatL(m.m22);
        }
        return res;
    }

public:
    static TFEngine Engine;

protected:
    SinCos *SinCos_table;
    TForm3D *viewPoint;
};




struct windd_arg256
{
    int sort_id;
    int width;
    int height;
    char name[32];
};



namespace GFX
{
    
struct GfxMode
{
    int w = 0;
    int h = 0;
    int bpp = 0;
    SDL_DisplayMode mode = {0};
    bool windowed = false;
    int32_t sortid = 0;
    std::string name;
    
    GfxMode();
    GfxMode(GfxMode &&g);
    GfxMode(const GfxMode &g);

    GfxMode& operator=(const GfxMode &g);    
    operator bool() const;    
    
    static bool Compare(const GfxMode &a, const GfxMode &b);
};
    
class GFXEngine
{
public:
    void Init();
    void Deinit();
    
    void RecreateScreenSurface();
    void DrawScreenSurface();
    
    SDL_Surface *Screen();
    SDL_Surface *RealScreen();
    
    // Get 3D class
    NC_STACK_win3d *C3D();

    uint8_t *MakeScreenCopy(int *ow, int *oh);
    uint8_t *MakeDepthScreenCopy(int *ow, int *oh);
    
    
    int GetScreenH();
    int GetScreenW();
    
    
    GfxMode windd_func0__sub0(const std::string &file);
    GfxMode sub_41F68C();

    void SetResolution(int res);
    void SetTracyRmp(ResBitmap *rmp);
    void SetShadeRmp(ResBitmap *rmp);

    void SetCursor(int curID, int force);
    int32_t GetGfxMode();

    size_t display_func256(windd_arg256 *inout);

    void SetTileset(TileMap *tileset, int id);
    TileMap * GetTileset(int id);

    void DrawText(w3d_a209 *arg);

    bool LoadPalette(const std::string &palette_ilbm);
    SDL_Cursor *LoadCursor(const std::string &name);
    
    
    static uint32_t CorrectSurfaceFormat(uint32_t format);
    static void GLMapFormat(uint32_t pixelFormat, GLint *format, GLint *type);
    
    static void DrawLine(SDL_Surface *surface, const Common::Line &line, uint8_t cr, uint8_t cg, uint8_t cb);
    static void BlitScaleMasked(SDL_Surface *src, Common::Rect sRect, SDL_Surface *mask, uint8_t index, SDL_Surface *dst, Common::Rect dRect);
    static void DrawFill(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect);
    
    static void Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::Point dPoint);
    static void Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::PointRect dRect);
    
    static SDL_Color Color(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255);
    static bool ColorCmp(const SDL_Color &a, const SDL_Color &b);
    
private:
    GFXEngine() {};
    
    void AddGfxMode(const GfxMode &md);
    uint32_t CursPix(uint8_t *data, int ofs, int bpp);
    
    static int EventsWatcher(void *, SDL_Event *event);
public:
    static GFXEngine Instance;
    
private:
    std::list<GfxMode> graphicsModes;
    std::array<SDL_Cursor *, 11> cursors;
    int CurrentCursorID = -1;
    int32_t GfxSelectedMode = 0;
    Gui::OldCompat *scrCompat = NULL;
    int32_t portalID = -1;
    
    SDL_Surface *ScreenSurface = NULL;
    GLuint screenTex = 0;
    GLint pixfmt, pixtype;
    NC_STACK_win3d *win3dObj = NULL;
};
    
static constexpr GFXEngine &Engine = GFXEngine::Instance;






    
}

#endif // ENGINE_GFX_H_INCLUDED
