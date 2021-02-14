#ifndef ENGINE_GFX_H_INCLUDED
#define ENGINE_GFX_H_INCLUDED

#include <array>
#include <deque>
#include <math.h>
#include <inttypes.h>

#include "system.h"
#include "../ini.h"
#include "../vectors.h"
#include "../matrix.h"
#include "../IFFile.h"
#include "../nucleas.h"



namespace Gui{ class OldCompat; };

typedef Tvec2d<double> vec2d;
typedef Tvec3d<double> vec3d;
typedef Tmat3x3<double, double> mat3x3;
typedef Tmat4x4<double, double> mat4x4;

#define GFX_MAX_VERTEX 12

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

struct polysDat
{
    int32_t range;
    polysDatSub datSub;
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


struct rstr_arg204
{
    ResBitmap *pbitm = NULL;
    Common::FRect float4;
    Common::FRect float14;
};

struct rstr_218_arg
{
    ResBitmap *bitm_intern;
    ResBitmap *bitm_intern2;
    int flg;
    Common::FRect rect1;
    Common::FRect rect2;
};

struct rstr_262_arg
{
    int cnt;
    int *slot;
    int *weight;
};

struct displ_arg263
{
    ResBitmap *bitm;
    int pointer_id;
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
    TTF_Font *ttfFont = NULL;
    int height = 0;
    uint8_t r = 0, g = 0, b = 0;

    std::list<ScreenText *> entries;
};
    
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

enum WDD_ATT
{
    WDD_ATT_CURSOR = 0x80002000,
    WDD_ATT_TEXFILT = 0x80002001,
    WDD_ATT_DIS_LOWRES = 0x80002002,
    WDD_ATT_16BIT_TEX = 0x80002003,
    WDD_ATT_DRAW_PRIM = 0x80002004
};

enum DISP_ATT
{
    ATT_FGPEN       = 0x80001000,
    ATT_BGPEN       = 0x80001001,
    ATT_SHADE_RMP   = 0x80001002,
    ATT_TRACY_RMP   = 0x80001003,
    ATT_FGAPEN      = 0x80001004,
    ATT_WIDTH       = 0x80001005,
    ATT_HEIGHT      = 0x80001006,
    ATT_PALETTE     = 0x80001007,
    ATT_DISPLAY_ID  = 0x80001008,
    ATT_DISPLAY_INF = 0x80001009,
    ATT_DISPLAY_WIN = 0x8000100A,
};

enum RFLAGS
{
    RFLAGS_LINMAP = 0x1,
    RFLAGS_PERSPMAP = 0x2,
    RFLAGS_FLATSHD = 0x4,
    RFLAGS_GRADSHD = 0x8,
    RFLAGS_ZEROTRACY = 0x10,
    RFLAGS_LUMTRACY = 0x20,
    RFLAGS_SKY = 0x40,
    RFLAGS_FALLOFF = 0x80,
    RFLAGS_IGNORE_FALLOFF = 0x1000,
};
    
class GFXEngine
{
protected:
    enum
    {
        DEFAULT_WIDTH = 640,
        DEFAULT_HEIGHT = 480,
    };
    
public:
    static uint32_t _fbo;
    static uint32_t _fboTex;
    static int can_srcblend;
    static int can_destblend;
    static int can_stippling;
    static uint32_t FpsMaxTicks;
    
public:
    void Init();
    void Deinit();
    
    void RecreateScreenSurface();
    void DrawScreenSurface();
    
    SDL_Surface *Screen();
    SDL_Surface *RealScreen();
    

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
    GFXEngine();
    
    void AddGfxMode(const GfxMode &md);
    uint32_t CursPix(uint8_t *data, int ofs, int bpp);
    
    static int EventsWatcher(void *, SDL_Event *event);

public:
    virtual size_t func0(IDVList &stak);
    virtual size_t raster_func198(const Common::FLine &arg);
    virtual size_t raster_func199(const Common::Line &arg);
    virtual size_t raster_func200(const Common::FLine &arg);
    virtual size_t raster_func201(const Common::Line &r);
    virtual size_t raster_func202(rstr_arg204 *arg);
    virtual size_t raster_func204(rstr_arg204 *arg);
    virtual size_t raster_func206(polysDat *arg);
    virtual void raster_func207(int id, TileMap *tiles);
    virtual TileMap *raster_func208(int id);
    virtual int raster_func208(TileMap *tiles);
    virtual void raster_func209(w3d_a209 *arg);
    virtual void raster_func210(const Common::FRect &arg);
    virtual void raster_func211(const Common::Rect &arg);
    virtual void BeginScene();
    virtual void EndScene();
    virtual size_t raster_func217(SDL_Color color);
    virtual void raster_func218(rstr_218_arg *arg);
    virtual void raster_func221(const Common::Rect &arg);
    virtual void BeginFrame();
    virtual void EndFrame();
    virtual void display_func261(int ID, UA_PALETTE &pal, int from, int num);
    virtual void display_func261(int ID, UA_PALETTE &pal);
    virtual void display_func262(rstr_262_arg *arg);

    virtual bool AllocTexture(ResBitmap *arg);
    virtual void FreeTexture(ResBitmap *arg);
    virtual void display_func271(IDVPair *stak);
    virtual void display_func272(IDVPair *);
    virtual UA_PALETTE * display_func273(int paletteId);
    virtual void SaveScreenshot(const std::string & screenName);
    virtual void windd_func320(IDVPair *);
    virtual void windd_func321(IDVPair *);
    virtual void windd_func322(windd_dlgBox *dlgBox);
    virtual void windd_func323(const char **filename);
    virtual void windd_func324(wdd_func324arg *inout);
    virtual void windd_func325(wdd_func324arg *arg);

    
    

    virtual void setWDD_cursor(int arg);
    virtual void setWDD_disLowRes(int arg);
    virtual void setWDD_16bitTex(int arg);
    virtual void setWDD_drawPrim(int arg);

    virtual int getWDD_16bitTex();
    virtual int getWDD_drawPrim();
    
    
    
    virtual void SetPalette(UA_PALETTE &pal);
    virtual void SetPen(SDL_Color pen);
    virtual void setRSTR_BGpen(uint32_t pen) {};
    virtual void setRSTR_shdRmp(ResBitmap *rmp) {};
    virtual void setRSTR_trcRmp(ResBitmap *rmp) {};
    virtual void setRSTR_FGApen(uint32_t pen) {};

    virtual UA_PALETTE *GetPalette();

    virtual int16_t GetWidth();
    virtual int16_t GetHeight();

    Common::Point GetSize() const { return Common::Point(_width, _height); };
    
    
    
    
    // windd methods
    size_t windd_func0(IDVList &stak);

    virtual void setW3D_texFilt(int arg);

    void draw2DandFlush();

    int LoadFontByDescr(const std::string &fontname);
    void matrixAspectCorrection(mat3x3 &inout, bool invert);
    void getAspectCorrection(float &cW, float &cH, bool invert);

    void setFrustumClip(float near, float far);

    static bool compare(polysDat *a, polysDat *b);
    
    void windd_func323__sub0(const char *filename);
    void win3dInitialisation();

    virtual void ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp);
    virtual SDL_PixelFormat *GetScreenFormat();
    virtual SDL_Surface *CreateSurfaceScreenFormat(int width, int height);
    virtual SDL_Surface *ConvertToScreenFormat(SDL_Surface *src);
    
    static SDL_Surface *ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt);
    
    virtual bool ChangeResolution(Common::Point res, bool windowed = false);
    
    void fpsLimitter(int value);

protected:
    void initPolyEngine();
    void initPixelFormats();
    void SetRenderStates(int arg);
    void sb_0x43b518(polysDat *polysDat, int a5, int a6);
    void RenderTransparent();
    void DrawScreenText();
    void AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag);
    void DrawTextEntry(const ScreenText *txt);

    void win3d_func209__sub0(char *cmdline, char **arr);
    void win3d_func262__sub0(int a2, int *a3, int *a4);
    void win3d_func274__sub0(FSMgr::FileHandle *fil);

    static SDL_Cursor *wrapLoadCursor(const char *name);

    void _setFrustumClip(float near, float far);
    
    void sub_420EDC(Common::Line line, uint8_t r, uint8_t g, uint8_t b);
    char * windd_func322__sub0(const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, uint32_t timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen);
    
    void ApplyResolution();

    bool SetResolution(Common::Point res);

public:
    //Data
    static const Nucleus::ClassDescr description;

    static Common::Ini::KeyList win3d_keys;
    
    
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

public:
    int _forcesoftcursor;
    int _movie_player;
    int _field_38;
    int _txt16bit;
    int _use_simple_d3d;
    int _disable_lowres;
    int _export_window_mode;
    int _flags;

    // From bigdata
    std::deque<polysDat *> _pending;
    float _greyColors[9][3];
    // \From bigdata

    int _dither;
    int _filter;
    int _antialias;
    int _alpha;
    int _zbuf_when_tracy;
    int _colorkey;

    SDL_PixelFormat *_pixfmt;
    SDL_DisplayMode _mode;
    GLint _glPixfmt, _glPixtype;

    uint32_t _rendStates[W3D_STATES_MAX];
    uint32_t _rendStates2[W3D_STATES_MAX];

    int _sceneBeginned;

    ScreenFont _font;

    float _corrW, _corrIW;
    float _corrH, _corrIH;
    bool _solidFont;
    
    // Display class
    SDL_Color _field_4 = {0}; // Color?
    Common::Rect _clip;
    Common::Rect _inverseClip;
    std::array<TileMap *, 256> _tiles;
    int _field_54c   = 0;
    int _field_550   = 0;
    float _field_554 = 0;
    float _field_558 = 0;
    
    UA_PALETTE _palette;
    std::array<UA_PALETTE, 8> _field_300;
    
protected:
    // Display class
    int16_t _width  = 0;
    int16_t _height = 0;

    // Win3d
    GLdouble _frustum[16];
    float _frustumNear;
    float _frustumFar;
};
    
static constexpr GFXEngine &Engine = GFXEngine::Instance;






    
}

#endif // ENGINE_GFX_H_INCLUDED
