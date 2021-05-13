#include "../fmtlib/printf.h"
#include <SDL2/SDL_opengl.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/glext.h>
#else
#include <GL/glext.h>
#endif

#include "gfx.h"
#include "../utils.h"
#include "../common.h"
#include "../ini.h"
#include "../gui/root.h"
#include "../bitmap.h"
#include "../ilbm.h"
#include "../log.h"
#include "../gui/oldcompat.h"
#include "../font.h"
#include "inivals.h"


namespace GFX
{   
GFXEngine GFXEngine::Instance;

int GFXEngine::can_srcblend;
int GFXEngine::can_destblend;
int GFXEngine::can_stippling;
uint32_t GFXEngine::FpsMaxTicks = 1000/60;

const std::array<vec3d, 8> GFXEngine::_clrEff 
{   vec3d(1.0,  1.0,  1.0)
,   vec3d(1.21, 0.0,  0.29)
,   vec3d(0.13, 0.43, 2.17)
,   vec3d(0.0,  1.60, 1.60)
,   vec3d(1.0,  1.0,  1.0)
,   vec3d(0.57, 0.59, 0.59)
,   vec3d(1.4,  1.08,  1.12)
,   vec3d(0.3,  0.60, 0.7)};

PFNGLGENFRAMEBUFFERSPROC GLGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC GLBindFramebuffer = NULL;
PFNGLGENRENDERBUFFERSPROC GLGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC GLBindRenderbuffer = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC GLFrameBufferTexture = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC GLFrameBufferTexture2D = NULL;
PFNGLRENDERBUFFERSTORAGEPROC GLRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC GLFramebufferRenderbuffer = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC GLCheckFramebufferStatus = NULL;
PFNGLGENERATEMIPMAPPROC GLGenerateMipmap = NULL;
PFNGLCREATESHADERPROC GLCreateShader = NULL;
PFNGLSHADERSOURCEPROC GLShaderSource = NULL;
PFNGLDELETESHADERPROC GLDeleteShader = NULL;
PFNGLCOMPILESHADERPROC GLCompileShader = NULL;
PFNGLCREATEPROGRAMPROC GLCreateProgram = NULL;
PFNGLATTACHSHADERPROC GLAttachShader = NULL;
PFNGLLINKPROGRAMPROC GLLinkProgram = NULL;
PFNGLUSEPROGRAMPROC GLUseProgram = NULL;
PFNGLGETUNIFORMLOCATIONPROC GLGetUniformLocation = NULL;
PFNGLUNIFORM1FPROC GLUniform1f = NULL;
PFNGLUNIFORM1IPROC GLUniform1i = NULL;
PFNGLUNIFORM2FPROC GLUniform2f = NULL;
PFNGLUNIFORM2IPROC GLUniform2i = NULL;
PFNGLUNIFORM3FPROC GLUniform3f = NULL;
PFNGLUNIFORM3IPROC GLUniform3i = NULL;
PFNGLUNIFORM4FPROC GLUniform4f = NULL;
PFNGLUNIFORM4IPROC GLUniform4i = NULL;

PFNGLGETSHADERIVPROC GLGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC GLGetShaderInfoLog = NULL;
    

GFXEngine::GFXEngine()
{
    for(TileMap *&t : _tiles)
        t = NULL;
    
    for(SDL_Color &c : _palette)
        c = {0, 0, 0, 0};
    
    _forcesoftcursor = 0;
    _field_38 = 0;
    _txt16bit = 0;
    _use_simple_d3d = 0;
    _disable_lowres = 0;
    _export_window_mode = 0;
    _flags = 0;

    _pending.clear();

    _dither = 0;
    _filter = 0;
    _antialias = 0;
    _alpha = 255;
    _zbuf_when_tracy = 0;
    _colorkey = 0;

    _pixfmt = NULL;
    _glPixfmt = 0;
    _glPixtype = 0;

    _sceneBeginned = 0;

    _corrIW = _corrW = 1.0;
    _corrIH = _corrH = 1.0;

    _solidFont = true;
    
    _setFrustumClip(1.0, 8192.0);
    
    _normClr = vec3d(1.0, 1.0, 1.0);
    _invClr = vec3d(0.0, 0.0, 0.0);
}


int out_guid_to_file(const char *filename, const char *name)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        fil->puts(name);
        delete fil;
        return 1;
    }
    return 0;
}

void out_yes_no_status(const char *filename, int val)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        if ( val )
            fil->puts("yes");
        else
            fil->puts("no");
        delete fil;
    }
}

void GFXEngine::DrawTextEntry(const ScreenText *txt)
{
    if ( _font.ttfFont )
    {
        if ( txt->flag & 0x20 )
        {
            _font.r = txt->p1;
            _font.g = txt->p2;
            _font.b = txt->p3;
        }
        else
        {
            int len = strlen(txt->string);

            if (len)
            {

                int cx = 0, cy = 0;

                if ( txt->flag & 0xE )
                {
                    TTF_SizeUTF8(_font.ttfFont, txt->string, &cx, &cy);
                }

                int p1 = txt->p1;
                int p2 = txt->p2;
                int p3 = txt->p3;
                int p4 = txt->p4;

                if ( txt->flag & 8 )
                    p3 = cx * p3 / 100;

                SDL_Rect clipRect;

                clipRect.x = p1;
                clipRect.w = p3 + 4;
                clipRect.h = p4 + 1;
                clipRect.y = p2;

                if ( txt->flag & 2 )
                {
                    if ( txt->flag & 8 )
                    {
                        p1 -= cx;
                        clipRect.x = p1;
                        clipRect.w = p3 + 4;
                    }
                    else
                    {
                        p1 += (p3 - cx);
                    }
                }
                else if ( txt->flag & 4 )
                {
                    if ( txt->flag & 8 )
                    {
                        p1 -= cx / 2;
                        clipRect.x = p1;
                        clipRect.w = p3 + 4;
                    }
                    else
                    {
                        p1 += (p3 - cx) / 2;
                    }
                }

                SDL_SetClipRect(Screen(), &clipRect);


                int v10 = ((p4 - _font.height) / 2) - 2 + p2;
                if ( txt->flag & 0x10 )
                {
                    v10++;
                    p1++;
                }

                SDL_Color clr;
                clr.a = 255;
                clr.r = 0;
                clr.g = 0;
                clr.b = 0;

                SDL_Surface *tmp;

                if (_solidFont)
                {
                    tmp = TTF_RenderUTF8_Solid(_font.ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_NONE);
                }
                else
                {
                    tmp = TTF_RenderUTF8_Blended(_font.ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
                }

                SDL_Rect want;
                want.w = tmp->w;
                want.h = tmp->h;
                want.x = p1 + 2;
                want.y = v10 + 1;

                SDL_BlitSurface(tmp, NULL, Screen(), &want);

                clr.a = 255;
                clr.r = _font.r;
                clr.g = _font.g;
                clr.b = _font.b;

                if (_solidFont)
                {
                    SDL_SetPaletteColors(tmp->format->palette, &clr, 1, 1);
                }
                else
                {
                    SDL_FreeSurface(tmp);
                    tmp = TTF_RenderUTF8_Blended(_font.ttfFont, txt->string, clr);
                    SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
                }

                want.w = tmp->w;
                want.h = tmp->h;
                want.x = p1 + 1;
                want.y = v10;

                SDL_BlitSurface(tmp, NULL, Screen(), &want);
                SDL_FreeSurface(tmp);


                SDL_SetClipRect(Screen(), NULL);
            }
        }
    }
}

void GFXEngine::AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag)
{
    ScreenText *v8 = new ScreenText;
    v8->string = string;
    v8->p1 = p1;
    v8->p2 = p2;
    v8->p3 = p3;
    v8->p4 = p4;
    v8->flag = flag;

    _font.entries.push_back(v8);
}

void GFXEngine::DrawScreenText()
{
    _font.r = 255;
    _font.g = 255;
    _font.b = 0;

    for ( std::list<ScreenText *>::iterator it = _font.entries.begin(); it != _font.entries.end(); it++ )
    {
        DrawTextEntry(*it);
        delete (*it);
    }

    _font.entries.clear();
}

void GFXEngine::initPixelFormats()
{
    if (_pixfmt)
        SDL_FreeFormat(_pixfmt);

    SDL_DisplayMode curr;
    SDL_GetCurrentDisplayMode(0, &curr);
    curr.format = CorrectSurfaceFormat(curr.format);

    _pixfmt = SDL_AllocFormat( curr.format );

    GLMapFormat(curr.format, &_glPixfmt, &_glPixtype);
}

void GFXEngine::initPolyEngine()
{
    _rendStates[TEXTUREHANDLE] = 0;
    _rendStates[SHADEMODE] = 1; //Smooth
    _rendStates[STIPPLEENABLE] = 0;
    _rendStates[SRCBLEND] = 1;//D3DBLEND_ONE;
    _rendStates[DESTBLEND] = 0;//D3DBLEND_ZERO;
    _rendStates[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
    _rendStates[ALPHABLENDENABLE] = 0; /* TRUE to enable alpha blending */
    _rendStates[ZWRITEENABLE] = 1; /* TRUE to enable z writes */
    _rendStates[TEXTUREMAG] = (_filter != 0); // D3DFILTER_NEAREST Or D3DFILTER_LINEAR
    _rendStates[TEXTUREMIN] = (_filter != 0); // D3DFILTER_NEAREST Or D3DFILTER_LINEAR

    for (int i = 0; i < W3D_STATES_MAX; i++)
        _rendStates2[i] = _rendStates[i];

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);

    glDisable(GL_CULL_FACE);

    if (_dither)
        glEnable(GL_DITHER);
    else
        glDisable(GL_DITHER);

    glDisable(GL_BLEND);
    glDisable(GL_FOG);

    //win3d__SetRenderState(w3d, D3DRENDERSTATE_TEXTUREMAG, (w3d->filter != 0) + D3DFILTER_NEAREST); // Or D3DFILTER_LINEAR

    //win3d__SetRenderState(w3d, D3DRENDERSTATE_SUBPIXEL, 1); /* TRUE to enable subpixel correction */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEDALPHA, can_stippling); /* TRUE to enable stippled alpha */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEENABLE, 0); /* TRUE to enable stippling */
    //win3d__SetRenderState(w3d, D3DRENDERSTATE_COLORKEYENABLE, 1); /* TRUE to enable source colorkeyed textures */

    _pending.clear();

    //glAlphaFunc ( GL_GREATER, 0.1 ) ;
    //glEnable ( GL_ALPHA_TEST );



//	for(int y = 0; y < 32; y++)
//    {
//        for(int x = 0; x < 4; x++)
//        {
//            if (y & 1)
//                stpl[x + y * 4] = 0x55;
//            else
//                stpl[x + y * 4] = 0xAA;
//        }
//    }

//	if (can_stippling)
//        glPolygonStipple(stpl);
}

int GFXEngine::LoadFontByDescr(const std::string &fontname)
{
    std::vector<std::string> splt = Stok::Split(fontname, ",");

    std::string facename;
    std::string s_height;
    
    if (splt.size() > 0)
        facename = splt[0];
    
    if (splt.size() > 1)
        s_height = splt[1];
    //const char *s_weight = strtok(0, ",");
    //const char *s_charset = strtok(0, ",");

    int height;//, weight, charset;
    if ( !facename.empty() && !s_height.empty() )//&& s_weight && s_charset )
    {
        height = std::stoi(s_height);
        //weight = atoi(s_weight);
        //charset = atoi(s_charset);
    }
    else
    {
        height = 12;
        //charset = 0;
        facename = "MS Sans Serif";
        //weight = 400;
    }

    if ( _font.ttfFont )
    {
        TTF_CloseFont(_font.ttfFont);
        _font.ttfFont = NULL;
    }

    _font.height = height;
    _font.ttfFont = System::LoadFont(facename, height);

    if ( _font.ttfFont )
    {
        if (!_solidFont)
            TTF_SetFontHinting(_font.ttfFont, TTF_HINTING_LIGHT);
        else
            TTF_SetFontHinting(_font.ttfFont, TTF_HINTING_MONO);

        return 1;
    }

    printf("Can't load font\n");

    return 0;
}


size_t GFXEngine::windd_func0(IDVList &stak)
{
    int txt16bit_def = read_yes_no_status("env/txt16bit.def", 1);
    int drawprim_def = read_yes_no_status("env/drawprim.def", 0);
    _export_window_mode = System::IniConf::GfxExportWindowMode.Get<bool>();     // gfx.export_window_mode

    switch(System::IniConf::GfxBlending.Get<int>())
    {
        case 0:
        {
            can_srcblend = 1;
            can_destblend = 0;
            can_stippling = 0;
        }
        break;
        
        default:
        case 1:
        {
            can_srcblend = 1;
            can_destblend = 1;
            can_stippling = 0;
        }
        break;
    
        case 2:
        {
            can_srcblend = 0;
            can_destblend = 0;
            can_stippling = 1;
        }
        break;
    }

    SetResolution( Common::Point(stak.Get<int32_t>(ATT_WIDTH, DEFAULT_WIDTH), stak.Get<int32_t>(ATT_HEIGHT, DEFAULT_HEIGHT)) );

    _forcesoftcursor = 0;
    _disable_lowres = System::IniConf::GfxDisableLowres.Get<bool>();
    _txt16bit = txt16bit_def;
    _use_simple_d3d = drawprim_def;

    _solidFont = System::IniConf::GfxSolidFont.Get<bool>();

    switch( System::IniConf::GfxVsync.Get<int>() )
    {
        case 0:
            SDL_GL_SetSwapInterval(0);
            break;
        
        default:
        case 1:
            SDL_GL_SetSwapInterval(1);
            break;
        
        case 2:
            {
                if ( SDL_GL_SetSwapInterval(-1) == -1)
                    SDL_GL_SetSwapInterval(1);
            }
            break;
    }

    fpsLimitter(System::IniConf::GfxMaxFps.Get<int>());

    LoadFontByDescr("MS Sans Serif,12,400,0");

    //win3d->field_54______rsrc_field4 = (bitmap_intern *)getRsrc_pData();
    return 1;
}


bool GFXEngine::SetResolution(Common::Point res)
{
    _width = res.x;   //stak.Get<int32_t>(ATT_WIDTH, 0);
    _height = res.y; //stak.Get<int32_t>(ATT_HEIGHT, 0);
    
    _clip = Common::Rect(_width - 1, _height - 1);

    _field_54c = _width / 2;
    _field_550 = _height / 2;

    _field_554 = _width / 2;
    _field_558 = _height / 2;
    
    return true;
}

size_t GFXEngine::func0(IDVList &stak)
{
    System::IniConf::ReadFromNucleusIni();

    if ( !windd_func0(stak) )
        return 0;

    _dither = System::IniConf::GfxDither.Get<bool>();
    _filter = System::IniConf::GfxFilter.Get<bool>();
    _antialias = System::IniConf::GfxAntialias.Get<bool>();
    _zbuf_when_tracy = System::IniConf::GfxZbufWhenTracy.Get<bool>();
    _colorkey = System::IniConf::GfxColorkey.Get<bool>();

    if ( can_srcblend )
        _alpha = 192;
    else
        _alpha = 128;

    _pending.clear();

    ApplyResolution();

    return 1;
}

void GFXEngine::ApplyResolution()
{        
    if ( (float)_width / (float)_height >= 1.4 )
    {
        int half = (_width + _height) / 2;
        _corrW = (float)half * 1.1429 / (float)_width;
        _corrH = (float)half * 0.85715 / (float)_height;
        _corrIW = 1.0 / _corrW;
        _corrIH = 1.0 / _corrH;
    }
    else //No correction
    {
        _corrIW = _corrW = 1.0;
        _corrIH = _corrH = 1.0;
    }
    
    initPixelFormats();
    
    initPolyEngine();
}

bool GFXEngine::ChangeResolution(Common::Point res, bool windowed)
{    
    SetResolution(res);        
    ApplyResolution();
    return true;
}


size_t GFXEngine::raster_func198(const Common::FLine &arg)
{
    float tX = _field_554 - 1.0;
    float tY = _field_558 - 1.0;

    int y1 = (arg.y1 + 1.0) * tY;
    int y2 = (arg.y2 + 1.0) * tY;
    int x1 = (arg.x1 + 1.0) * tX;
    int x2 = (arg.x2 + 1.0) * tX;

    DrawLine(Screen(),
                      Common::Line(x1, y1, x2, y2),
                      _field_4.r,
                      _field_4.g,
                      _field_4.b);
    return 1;
}


size_t GFXEngine::raster_func199(const Common::Line &arg)
{
    DrawLine(Screen(),
                      Common::Line(_field_54c + arg.x1, _field_550 + arg.y1,
                                   _field_54c + arg.x2, _field_550 + arg.y2),
                      _field_4.r,
                      _field_4.g,
                      _field_4.b );

    return 1;
}

void GFXEngine::sub_420EDC(Common::Line line, uint8_t r, uint8_t g, uint8_t b)
{
    if ( line.ClipBy(_clip) )
    {
        Common::Line tmp2 = line;

        if ( _inverseClip.IsEmpty() || !tmp2.ClipBy(_inverseClip) )
        {
            DrawLine(Screen(), line, r, g, b);
        }
        else
        {
            if ( tmp2.P2() != line.P2() )
            {
                if ( tmp2.P1() != line.P1() )
                {
                    DrawLine(Screen(), Common::Line(line.P1(), tmp2.P1()), r, g, b);
                    DrawLine(Screen(), Common::Line(tmp2.P2(), line.P2()), r, g, b);
                }
                else
                {
                    DrawLine(Screen(), Common::Line(tmp2.P2(), line.P2()), r, g, b);
                }
            }
            else
            {
                DrawLine(Screen(), Common::Line(line.P1(), tmp2.P1()), r, g, b);
            }
        }
    }
}

size_t GFXEngine::raster_func200(const Common::FLine &arg)
{
    float tX = _field_554 - 1.0;
    float tY = _field_558 - 1.0;

    sub_420EDC (Common::Line( (arg.x1 + 1.0) * tX
                            , (arg.y1 + 1.0) * tY
                            , (arg.x2 + 1.0) * tX
                            , (arg.y2 + 1.0) * tY),
                        _field_4.r,
                        _field_4.g,
                        _field_4.b);

    return 1;
}

size_t GFXEngine::raster_func201(const Common::Line &l)
{
    sub_420EDC( Common::Line( _field_54c + l.x1
                            , _field_550 + l.y1
                            , _field_54c + l.x2
                            , _field_550 + l.y2),
                        _field_4.r,
                        _field_4.g,
                        _field_4.b );

    return 1;
}

size_t GFXEngine::raster_func202(rstr_arg204 *arg)
{
    Common::Rect r1;
    r1.left   = (arg->float4.left   + 1.0) * (arg->pbitm->width / 2);
    r1.top    = (arg->float4.top    + 1.0) * (arg->pbitm->height / 2);
    r1.right  = (arg->float4.right  + 1.0) * (arg->pbitm->width / 2);
    r1.bottom = (arg->float4.bottom + 1.0) * (arg->pbitm->height / 2);

    Common::Rect r2;
    r2.left   = _field_554 * (arg->float14.left   + 1.0);
    r2.top    = _field_558 * (arg->float14.top    + 1.0);
    r2.right  = _field_554 * (arg->float14.right  + 1.0);
    r2.bottom = _field_558 * (arg->float14.bottom + 1.0);
    
    SDL_Rect src = r1;
    SDL_Rect dst = r2;
    
    SDL_BlitScaled(arg->pbitm->swTex, &src, Screen(), &dst);
    
    return 1;
}

size_t GFXEngine::raster_func204(rstr_arg204 *arg)
{   
    Common::Rect r1;
    r1.left   = (arg->float4.left   + 1.0) * (arg->pbitm->width / 2);
    r1.top    = (arg->float4.top    + 1.0) * (arg->pbitm->height / 2);
    r1.right  = (arg->float4.right  + 1.0) * (arg->pbitm->width / 2);
    r1.bottom = (arg->float4.bottom + 1.0) * (arg->pbitm->height / 2);

    Common::Rect r2;
    r2.left   = _field_554 * (arg->float14.left   + 1.0);
    r2.top    = _field_558 * (arg->float14.top    + 1.0);
    r2.right  = _field_554 * (arg->float14.right  + 1.0);
    r2.bottom = _field_558 * (arg->float14.bottom + 1.0);
    
    if ( _clip.IsIntersects(r2) )
    {
        if ( r2.left < _clip.left )
        {
            r1.left += (_clip.left - r2.left) * r1.Width() / r2.Width();
            r2.left = _clip.left;
        }

        if ( r2.right > _clip.right )
        {
            r1.right += (_clip.right - r2.right) * r1.Width() / r2.Width();
            r2.right = _clip.right;
        }

        if ( r2.top < _clip.top )
        {
            r1.top += (_clip.top - r2.top) * r1.Height() / r2.Height();
            r2.top = _clip.top;
        }

        if ( r2.bottom > _clip.bottom )
        {
            r1.bottom += (_clip.bottom - r2.bottom) * r1.Height() / r2.Height();
            r2.bottom = _clip.bottom;
        }
    
        SDL_Rect src = r1;
        SDL_Rect dst = r2;
        SDL_BlitScaled(arg->pbitm->swTex, &src, Screen(), &dst);
    }

    return 1;
}

void GFXEngine::SetRenderStates(int setAll)
{
    uint32_t changeStates = 0;

    if (setAll)
        changeStates = 0xFFFFFFFF;

    for (int i = 0; i < W3D_STATES_MAX; i++)
    {
        if (_rendStates2[i] != _rendStates[i])
            changeStates |= 1 << i;

        _rendStates[i] = _rendStates2[i];
    }

    if ( changeStates & MSK(TEXTUREHANDLE))
    {
        if (_rendStates[TEXTUREHANDLE])
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, _rendStates[TEXTUREHANDLE]);
        }
        else
        {
            glDisable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

    if ( changeStates & ( MSK(TEXTUREHANDLE) | MSK(TEXTUREMAG) ) )
    {
        if (_rendStates[TEXTUREMAG])
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if ( changeStates & ( MSK(TEXTUREHANDLE) | MSK(TEXTUREMIN) ) )
    {
        if (_rendStates[TEXTUREMIN])
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        else
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }


    if ( changeStates & MSK(SHADEMODE) )
    {
        if (_rendStates[SHADEMODE])
            glShadeModel(GL_SMOOTH);
        else
            glShadeModel(GL_FLAT);
    }

    if ( changeStates & MSK(STIPPLEENABLE) )
    {
    }

    if ( changeStates & (MSK(SRCBLEND) | MSK(DESTBLEND)) )
    {
        GLint src, dst;
        switch (_rendStates[SRCBLEND])
        {
        case 0:
            src = GL_ZERO;
            break;

        case 1:
            src = GL_ONE;
            break;

        default:
        case 2:
            src = GL_SRC_ALPHA;
            break;

        case 3:
            src = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }

        switch (_rendStates[DESTBLEND])
        {
        case 0:
            dst = GL_ZERO;
            break;

        case 1:
            dst = GL_ONE;
            break;

        case 2:
            dst = GL_SRC_ALPHA;
            break;

        default:
        case 3:
            dst = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }

        glBlendFunc(src, dst);
    }

    if ( changeStates & MSK(TEXTUREMAPBLEND) )
    {
        if (_rendStates[TEXTUREMAPBLEND] == 0)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else if (_rendStates[TEXTUREMAPBLEND] == 1)
        {
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
        }
        else if (_rendStates[TEXTUREMAPBLEND] == 2)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    if ( changeStates & MSK(ALPHABLENDENABLE) )
    {
        if (_rendStates[ALPHABLENDENABLE])
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    if ( changeStates & MSK(ZWRITEENABLE) )
    {
        if (_rendStates[ZWRITEENABLE])
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }
}


void GFXEngine::Rendering3DStuff(polysDat *in, bool renderTransparent)
{
    polysDatSub *polysDat = &in->datSub;

    struct vtxOut
    {
        float x, y, z;
        float tu, tv;
        float r, g, b, a;
    };

    if ( !_sceneBeginned )
        return;

    if ( polysDat->vertexCount < 3 || polysDat->vertexCount > 12 )
        return;

    //Store for rendering later ( from 214 method )
    if ( !(polysDat->renderFlags & RFLAGS_FALLOFF) )
        if ( (polysDat->renderFlags & (RFLAGS_ZEROTRACY | RFLAGS_LUMTRACY)) && !renderTransparent )
        {
            _pending.push_back( in );
            return;
        }

    vtxOut vtx[24];

    for (int i = 0; i < polysDat->vertexCount; i++)
    {
        vtx[i].x = polysDat->vertexes[i].x;
        vtx[i].y = polysDat->vertexes[i].y;
        vtx[i].z = polysDat->vertexes[i].z;
        vtx[i].tu = 0.0;
        vtx[i].tv = 0.0;
        vtx[i].r = polysDat->r;
        vtx[i].g = polysDat->g;
        vtx[i].b = polysDat->b;
        vtx[i].a = 1.0;
    }

    _rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
    _rendStates2[STIPPLEENABLE] = 0;
    _rendStates2[SRCBLEND] = 1;//D3DBLEND_ONE;
    _rendStates2[DESTBLEND] = 0;//D3DBLEND_ZERO;
    _rendStates2[TEXTUREMAPBLEND] = 0;//D3DTBLEND_COPY;
    _rendStates2[ALPHABLENDENABLE] = 0;
    _rendStates2[ZWRITEENABLE] = 1;
    _rendStates2[TEXTUREHANDLE] = 0;

    _rendStates2[TEXTUREMAG] = (_filter != 0);
    _rendStates2[TEXTUREMIN] = (_filter != 0);

    if ( polysDat->renderFlags & (RFLAGS_LINMAP | RFLAGS_PERSPMAP) )
    {
        if (in->datSub.pbitm)
            _rendStates2[TEXTUREHANDLE] = in->datSub.pbitm->hwTex;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vtx[i].tu = polysDat->tu_tv[i].tu;
            vtx[i].tv = polysDat->tu_tv[i].tv;
        }
    }
    else
    {
        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vtx[i].r = 0;
            vtx[i].g = 0;
            vtx[i].b = 0;
            vtx[i].a = 1.0;
        }

    }

    if ( polysDat->renderFlags & (RFLAGS_FLATSHD | RFLAGS_GRADSHD) )
    {
        _rendStates2[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
        _rendStates2[SHADEMODE] = 1;//D3DSHADE_GOURAUD;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            float comp = (1.0 - polysDat->color[i]);
            vtx[i].r *= comp;
            vtx[i].g *= comp;
            vtx[i].b *= comp;
            vtx[i].a = 1.0;
        }
    }

    if ( polysDat->renderFlags & RFLAGS_LUMTRACY )
    {
        if ( !_zbuf_when_tracy )
            _rendStates2[ZWRITEENABLE] = 0;

        if ( can_destblend )
        {
            _rendStates2[ALPHABLENDENABLE] = 1;
            _rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            _rendStates2[SRCBLEND] = 1;//D3DBLEND_ONE;
            _rendStates2[DESTBLEND] = 1;//D3DBLEND_ONE;
            _rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }
        else if ( can_srcblend )
        {
            _rendStates2[ALPHABLENDENABLE] = 1;
            _rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            _rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            _rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            _rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }
        else if ( can_stippling )
        {
            _rendStates2[ALPHABLENDENABLE] = 1;
            _rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            _rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            _rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            _rendStates2[STIPPLEENABLE] = 1;
            _rendStates2[SHADEMODE] = 0;//D3DSHADE_FLAT;
        }

        for (int i = 0; i < polysDat->vertexCount; i++)
            vtx[i].a = (float)_alpha / 255.0;
    }
    else if ( polysDat->renderFlags & RFLAGS_ZEROTRACY )
    {
        if ( !_zbuf_when_tracy )
            _rendStates2[ZWRITEENABLE] = 0;

        if ( _pixfmt->BytesPerPixel != 1 )
        {
            _rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            _rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
        }

        _rendStates2[ALPHABLENDENABLE] = 1;
        _rendStates2[TEXTUREMAG] = 0;//D3DFILTER_NEAREST;
        _rendStates2[TEXTUREMIN] = 0;//D3DFILTER_NEAREST;
        _rendStates2[TEXTUREMAPBLEND] = 2;//D3DTBLEND_MODULATE;
    }

    if (System::IniConf::GfxNewSky.Get<bool>())
    {
        if (polysDat->renderFlags & RFLAGS_SKY)
        {
            _rendStates2[ZWRITEENABLE] = 0;
        }
        else if (polysDat->renderFlags & RFLAGS_FALLOFF)
        {
            _rendStates2[ZWRITEENABLE] = 1;
            _rendStates2[ALPHABLENDENABLE] = 1;
            _rendStates2[TEXTUREMAPBLEND] = 2;
            _rendStates2[SRCBLEND] = 2;
            
            if (polysDat->renderFlags & RFLAGS_LUMTRACY)
                _rendStates2[DESTBLEND] = 1;
            else
                _rendStates2[DESTBLEND] = 3;
            
            _rendStates2[SHADEMODE] = 1;
            _rendStates2[STIPPLEENABLE] = 0;

            float transDist = System::IniConf::GfxSkyDistance.Get<int>();
            float transLen = System::IniConf::GfxSkyLength.Get<int>();

            for (int i = 0; i < polysDat->vertexCount; i++)
            {
                if (polysDat->distance[i] > transDist)
                {
                    float prc = (polysDat->distance[i] - transDist) / transLen;
                    if (prc > 1.0)
                        prc = 1.0;

                    vtx[i].a = (1.0 - prc);
                }
            }
        }
    }

    SetRenderStates(0);

    glBegin(GL_TRIANGLE_FAN);
    {
        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            glColor4f(vtx[i].r, vtx[i].g, vtx[i].b, vtx[i].a);
            glTexCoord2f(vtx[i].tu, vtx[i].tv);
            glVertex3d(vtx[i].x, -vtx[i].y, vtx[i].z);
        }
    }
    glEnd();
}

size_t GFXEngine::raster_func206(polysDat *arg)
{
    Rendering3DStuff(arg, false);

    return 1;
}

void GFXEngine::raster_func207(int id, TileMap *t)
{
    _tiles[id] = t;
}

TileMap *GFXEngine::raster_func208(int id)
{
    return _tiles[id];
}

int GFXEngine::raster_func208(TileMap *t)
{
    if ( t )
    {
        for (int i = 0; i < 256; i++)
        {
            if (_tiles[i] == t)
                return i;
        }
    }
    return -1;
}

void GFXEngine::win3d_func209__sub0(char *cmdline, char **arr)
{
    int v11;

    int bytesPerColor = Screen()->format->BytesPerPixel;

    char *curpos = cmdline;
    int w_pixels = Screen()->pitch / bytesPerColor;
    TileMap *tile = NULL;

    int x_out = 0;
    int y_out = 0;

    int x_out_txt = 0;
    int y_out_txt = 0;
    int txt_flag = 0;

    int y_pos_line = 0;
    int x_pos_line = 0;

    int halfWidth = _width / 2;
    int halfHeight = _height / 2;

    int line_width = 0;
    int line_height = 0;

    //if v11 = 0 - clone first column of tile  (count = line_width)
    //if v11 = 1 - normal copy of tile
    v11 = 1;


    int x_off = 0;
    int y_off = 0;


    char *positions[64];
    int position_idx = 0;

    positions[position_idx] = NULL;
    position_idx++;

    while ( 1 )
    {
        int v13 = FontUA::get_u8(&curpos);

        if ( v13 )
        {
            Common::PointRect &chrr = tile->map[v13];

            int cpy_width;

            if ( line_width )
                cpy_width = line_width - x_off;
            else
                cpy_width = chrr.w - x_off;

            int cpy_height = line_height - y_off;

            //SDL_Rect srcR, dstR;
            Common::Rect dstR(x_out, y_out, x_out + cpy_width, y_out + cpy_height);            
            /*dstR.x = x_out;
            dstR.y = y_out;
            dstR.w = cpy_width;
            dstR.h = cpy_height;*/
            
            Common::Rect srcR;
            srcR.left = chrr.x + x_off;
            srcR.top = chrr.y + y_off;
            srcR.bottom = chrr.y + y_off + cpy_height;
            
            if (v11)
                srcR.right = chrr.x + x_off + cpy_width;
            else
                srcR.right = chrr.x + x_off + 1;
            
            DrawFill(tile->img->GetSwTex(), srcR, Screen(), dstR);
                    
            /*srcR.h = cpy_height;
            
            if (v11)
                srcR.w = cpy_width;
            else
                srcR.w = 1;
            
            for(int i = 0; i < cpy_width; i += srcR.w)
            {
                SDL_BlitSurface(tile->img->GetSwTex(), &srcR, Screen(), &dstR);
                dstR.x += srcR.w;
            }*/     

            line_width = 0;
            x_off = 0;
            x_out += cpy_width;
            v11 = 1;
        }
        else // 0
        {
            int opcode = FontUA::get_u8(&curpos);

            switch ( opcode )
            {
            case 0: // End
                position_idx--;

                curpos = positions[position_idx];
                if ( curpos )
                    break;

                DrawScreenText();
                return;

            case 1: // x pos from center
                x_out = halfWidth + FontUA::get_s16(&curpos);
                x_pos_line = x_out;

                y_pos_line = y_out;
                y_off = 0;

                line_height = tile->h;
                break;

            case 2: // y pos from center
                y_out = halfHeight + FontUA::get_s16(&curpos);
                x_pos_line = x_out;

                y_pos_line = y_out;
                y_off = 0;

                line_height = tile->h;
                break;

            case 3: //xpos
                x_out = FontUA::get_s16(&curpos);
                if ( x_out < 0 )
                    x_out += w_pixels;

                x_pos_line = x_out;
                y_pos_line = y_out;

                line_height = tile->h;
                y_off = 0;
                break;

            case 4: //ypos
                y_out = FontUA::get_s16(&curpos);
                if ( y_out < 0 )
                    y_out += _height;

                x_pos_line = x_out;
                y_pos_line = y_out;

                line_height = tile->h;
                y_off = 0;
                break;

            case 5: //add to x pos
                x_out += FontUA::get_s16(&curpos);
                break;

            case 6: //add to y pos
                y_out += FontUA::get_s16(&curpos);
                break;

            case 7: //next line
                y_out = (line_height - y_off) + y_pos_line;
                y_pos_line = y_out;
                x_out = x_pos_line;

                y_off = 0;
                line_height = tile->h;
                break;

            case 8: // Select tileset
                tile = _tiles[FontUA::get_u8(&curpos)];
                break;

            case 9: // Include another cmdlist source
            {
                int azaza = FontUA::get_u8(&curpos);
                positions[position_idx] = curpos;
                position_idx++;
                curpos = arr[azaza];
            }
            break;

            case 10:
                line_width = FontUA::get_u8(&curpos);

                v11 = 0;
                x_off = 0;

                break;

            case 11:

                line_width = FontUA::get_u8(&curpos);

                v11 = 0;
                x_off = 0;

                line_width -= (x_out - x_pos_line);
                break;

            case 12: // Set x offset
                x_off = FontUA::get_u8(&curpos);
                break;

            case 13: // Set x width
                line_width = FontUA::get_u8(&curpos);
                break;

            case 14: // Set y offset
                y_off = FontUA::get_u8(&curpos);
                break;

            case 15: // Set y height
                line_height = FontUA::get_u8(&curpos);
                break;

            case 16: // Full reset tileset
                tile = _tiles[FontUA::get_u8(&curpos)];
                line_height = tile->h;
                y_off = 0;
                break;

            case 17:
                line_width = FontUA::get_s16(&curpos);
                v11 = 0;
                x_off = 0;
                line_width -= (x_out - x_pos_line);
                break;

            case 18: // Add text
            {
                int block_width = FontUA::get_s16(&curpos);
                int flag = txt_flag | FontUA::get_u16(&curpos);

                char *txtpos = (char *)curpos;

                curpos += strlen(txtpos) + 1;
                AddScreenText(txtpos, x_out_txt, y_out_txt, block_width, tile->h, flag);
            }
            break;

            case 19: // Copy current x/y pos for text output
                x_out_txt = x_out;
                y_out_txt = y_out;
                break;

            case 20: // Add txtout flag
                txt_flag |= FontUA::get_u16(&curpos);
                break;

            case 21: // Delete txtout flag
                txt_flag &= ~(FontUA::get_u16(&curpos));
                break;

            case 22: // set color for font
            {
                int r = FontUA::get_u16(&curpos);

                int g = FontUA::get_u16(&curpos);

                int b = FontUA::get_u16(&curpos);

                AddScreenText(0, r, g, b, 0, 0x20);
            }
            break;
            }
        }
    }
}

void GFXEngine::raster_func209(w3d_a209 *arg)
{
    win3d_func209__sub0(arg->cmdbuf, arg->includ);
}

void GFXEngine::raster_func210(const Common::FRect &arg)
{
    _clip = Common::Rect(   (arg.left + 1.0) * (_field_554 + -1.0),
                            (arg.top + 1.0) * (_field_558 + -1.0),
                            (arg.right + 1.0) * (_field_554 + -1.0),
                            (arg.bottom + 1.0) * (_field_558 + -1.0) );
    }

void GFXEngine::raster_func211(const Common::Rect &arg)
{
    _clip = Common::Rect(   _field_54c + arg.left,
                            _field_550 + arg.top,
                            _field_54c + arg.right,
                            _field_550 + arg.bottom );
}


void GFXEngine::BeginScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(_frustum);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _sceneBeginned = 1;
}

bool GFXEngine::compare(polysDat *a, polysDat *b)
{
    return a->range > b->range;
}

void GFXEngine::RenderTransparent()
{
    if ( _sceneBeginned )
    {
        if ( _pending.size() )
        {
            std::stable_sort(_pending.begin(), _pending.end(), compare);

            for (std::deque<polysDat *>::iterator it = _pending.begin(); it != _pending.end(); it++)
            {
                Rendering3DStuff((*it), true);
            }

            _pending.clear();
        }
    }
}

// Draw transparent
void GFXEngine::EndScene()
{
    RenderTransparent();

    _sceneBeginned = 0;
}

size_t GFXEngine::raster_func217(SDL_Color color)
{
    //if ( !ColorCmp(color, Color(255, 255, 255, 255) ) )
    _field_4 = color;

    return 0;
}

void GFXEngine::raster_func218(rstr_218_arg *arg)
{
    Common::Rect sRect( (arg->rect1.left + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.top + 1.0) * (arg->bitm_intern->height / 2),
                        (arg->rect1.right + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.bottom + 1.0) * (arg->bitm_intern->height / 2) );

    Common::Rect dRect( (arg->rect2.left + 1.0) * _field_554,
                        (arg->rect2.top + 1.0) * _field_558,
                        (arg->rect2.right + 1.0) * _field_554,
                        (arg->rect2.bottom + 1.0) * _field_558 );

    BlitScaleMasked(arg->bitm_intern->swTex, sRect, arg->bitm_intern2->swTex, arg->flg, Screen(), dRect);
}

void GFXEngine::raster_func221(const Common::Rect &arg)
{
    _inverseClip.left = _field_54c + arg.left;
    _inverseClip.top = _field_550 + arg.top;
    _inverseClip.right = _field_54c + arg.right;
    _inverseClip.bottom = _field_550 + arg.bottom;
}

void GFXEngine::BeginFrame()
{
    SDL_FillRect(Screen(), NULL, SDL_MapRGBA(Screen()->format, 0, 0, 0, 0) );
    
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);

    glPushAttrib(GL_DEPTH_WRITEMASK);
    glDepthMask(GL_TRUE);
    
    if (_colorEffects)
    {
        GLBindFramebuffer(GL_FRAMEBUFFER, 0);
    
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPopAttrib();
}

void GFXEngine::EndFrame()
{   
    if (_colorEffects > 1)
    {
        GLBindFramebuffer(GL_FRAMEBUFFER, 0);   
        DrawFBO();
    }    
    
    Gui::Root::Instance.Draw(RealScreen());
    DrawScreenSurface();
    Gui::Root::Instance.HwCompose();
    
    if (_colorEffects == 1)
    {
        GLBindFramebuffer(GL_FRAMEBUFFER, 0);   
        DrawFBO();
    }
    
    System::Flip();
}

void GFXEngine::SetColorEffectsPowers(const std::vector<ColorFx> &arg)
{    
    if (arg.empty())
    {
        _normClr = vec3d(1.0, 1.0, 1.0);
        _invClr = vec3d(0.0, 0.0, 0.0);
    }
    else
    {
        _normClr = vec3d(0.0, 0.0, 0.0);
        _invClr = vec3d(0.0, 0.0, 0.0);

        for (ColorFx fx : arg)
        {
            switch(fx.Id)
            {
                case 4:
                case 5:
                case 7:
                    _invClr += _clrEff.at( fx.Id ) * fx.Pwr;
                    break;
                default:
                    _normClr += _clrEff.at( fx.Id ) * fx.Pwr;
                    break;
            }
        }
    }
}

bool GFXEngine::AllocTexture(ResBitmap *bitm)
{
    if (bitm->swTex && !bitm->hwTex)
    {
        glPushAttrib(GL_TEXTURE_2D | GL_TEXTURE_BINDING_2D);

        glGenTextures(1, &bitm->hwTex);

        if (!bitm->hwTex)
        {
            glPopAttrib();
            return false;
        }

        glBindTexture(GL_TEXTURE_2D, bitm->hwTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (bitm->swTex->format->format == _pixfmt->format)
        {
            SDL_LockSurface(bitm->swTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitm->width, bitm->height, 0, _glPixfmt, _glPixtype, bitm->swTex->pixels);
            SDL_UnlockSurface(bitm->swTex);
        }
        else
        {
            SDL_Surface *conv = ConvertSDLSurface(bitm->swTex, _pixfmt);
            
            SDL_LockSurface(conv);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitm->width, bitm->height, 0, _glPixfmt, _glPixtype, conv->pixels);
            SDL_UnlockSurface(conv);
            
            SDL_FreeSurface(conv);
        }
        
        glPopAttrib();
    }

    return true;
}

void GFXEngine::FreeTexture(ResBitmap *bitm)
{
    if ( bitm->hwTex )
        glDeleteTextures(1, &bitm->hwTex);

    bitm->hwTex = 0;
}

void GFXEngine::display_func271(IDVPair *stak)
{

}

void GFXEngine::display_func272(IDVPair *)
{
}

void GFXEngine::win3d_func274__sub0(FSMgr::FileHandle *fil)
{
    int bf_w = 0, bf_h = 0;

    uint8_t *buf = MakeScreenCopy(&bf_w, &bf_h);

    if (buf && bf_w && bf_h)
    {
        fil->printf("P6\n#YPA screenshot\n%d %d\n255\n", bf_w, bf_h);

        int lwidth = 3 * bf_w;

        for (int j = 0; j < bf_h; j++)
        {
            uint8_t *ln = buf + lwidth * (bf_h - 1 - j);

            for (int i = 0; i < bf_w; i++)
            {
                uint8_t *px = ln + i * 3;

                uint8_t r = px[0];
                uint8_t g = px[1];
                uint8_t b = px[2];

                fil->writeU8(r);
                fil->writeU8(g);
                fil->writeU8(b);
            }
        }

        free(buf);
    }
}

void GFXEngine::SaveScreenshot(const std::string & screenName)
{
    FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("%s.ppm", screenName), "wb");
    if ( fil )
    {
        win3d_func274__sub0(fil);
        delete fil;
    }
}


void GFXEngine::windd_func320(IDVPair *)
{
}

void GFXEngine::windd_func321(IDVPair *)
{
}





char * GFXEngine::windd_func322__sub0(const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, uint32_t timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen)
{
    dprintf("MAKE ME %s\n","windd_func322__sub0");
    return NULL;
}

//Show DLGBox with edit field and get entered value
void GFXEngine::windd_func322(windd_dlgBox *dlgBox)
{
    windd_func320(NULL);

    dlgBox->result = windd_func322__sub0(
                         dlgBox->title,
                         dlgBox->ok,
                         dlgBox->cancel,
                         dlgBox->startText,
                         dlgBox->time,
                         dlgBox->timer_func,
                         dlgBox->timer_context,
                         dlgBox->replace,
                         dlgBox->maxLen);

    windd_func321(NULL);
}



void GFXEngine::windd_func324(wdd_func324arg *inout)
{
    if ( inout->guid )
    {
        inout->name = NULL;
        inout->guid = NULL;
        inout->currr = 0;
    }
    else
    {
        inout->name = "OpenGL";
        inout->guid = "<primary>";
        inout->currr = 1;
    }
}

void GFXEngine::windd_func325(wdd_func324arg *arg)
{
    const char *v4 = arg->guid;
    const char *v5 = "<error>";

    if ( v4 )
    {
        if ( !strcmp(v4, "<primary>") )
        {
            v5 = v4;
            v4 = NULL;
        }
        if ( !strcmp(v4, "<software>") )
        {
            v5 = v4;
            v4 = NULL;
        }
    }

    out_guid_to_file("env/guid3d.def", v5);
}



void GFXEngine::setWDD_cursor(int mode)
{
}

void GFXEngine::setWDD_disLowRes(int arg)
{
}

void GFXEngine::setWDD_16bitTex(int arg)
{
    out_yes_no_status("env/txt16bit.def", arg);
}

void GFXEngine::setWDD_drawPrim(int arg)
{
    out_yes_no_status("env/drawprim.def", arg);
}



int GFXEngine::getWDD_16bitTex()
{
    return _txt16bit;
}

int GFXEngine::getWDD_drawPrim()
{
    return _use_simple_d3d;
}

void GFXEngine::setW3D_texFilt(int arg)
{
    _filter = arg;
}

void GFXEngine::SetPalette(UA_PALETTE &newPal)
{
    _palette = newPal;
    
    _normClr = vec3d(1.0, 1.0, 1.0);
    _invClr = vec3d(0.0, 0.0, 0.0);
}

void GFXEngine::SetPen(SDL_Color pen)
{
    _field_4 = pen;
}

UA_PALETTE *GFXEngine::GetPalette()
{
    return &_palette;
}

int16_t GFXEngine::GetWidth()
{
    return _width;
}

int16_t GFXEngine::GetHeight()
{
    return _height;
}


void GFXEngine::draw2DandFlush()
{
    if (_colorEffects)
        GLBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    Gui::Root::Instance.Draw(RealScreen());
    DrawScreenSurface();
    Gui::Root::Instance.HwCompose();

    SDL_FillRect(Screen(), NULL, SDL_MapRGBA(Screen()->format, 0, 0, 0, 0) );
    
    if (_colorEffects)
        GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

void GFXEngine::matrixAspectCorrection(mat3x3 &inout, bool invert)
{
    if (invert)
    {
        inout.m00 *= _corrIW;
        inout.m01 *= _corrIW;
        inout.m02 *= _corrIW;
        inout.m10 *= _corrIH;
        inout.m11 *= _corrIH;
        inout.m12 *= _corrIH;
    }
    else
    {
        inout.m00 *= _corrW;
        inout.m01 *= _corrW;
        inout.m02 *= _corrW;
        inout.m10 *= _corrH;
        inout.m11 *= _corrH;
        inout.m12 *= _corrH;
    }
}

void GFXEngine::getAspectCorrection(float &cW, float &cH, bool invert)
{
    if (invert)
    {
        cW = _corrIW;
        cH = _corrIH;
    }
    else
    {
        cW = _corrW;
        cH = _corrH;
    }
}

void GFXEngine::setFrustumClip(float _near, float _far)
{
    if (_near != _frustumNear || _far != _frustumFar)
        _setFrustumClip(_near, _far);
}

void GFXEngine::_setFrustumClip(float _near, float _far)
{
    //-z * frustum
    _frustumNear = _near;
    _frustumFar = _far;
    _frustum[0] = 1.0;
    _frustum[1] = 0.0;
    _frustum[2] = 0.0;
    _frustum[3] = 0.0;

    _frustum[4] = 0.0;
    _frustum[5] = 1.0;
    _frustum[6] = 0.0;
    _frustum[7] = 0.0;

    _frustum[8] = 0.0;
    _frustum[9] = 0.0;
    _frustum[10] = (_far + _near) / (_far - _near);
    _frustum[11] = 1.0;

    _frustum[12] = 0.0;
    _frustum[13] = 0.0;
    _frustum[14] = -2.0 * (_far * _near) / (_far - _near);
    _frustum[15] = 0.0;
}

void GFXEngine::ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp)
{
    for (uint16_t i = 0; i < dst->size(); i++)
    {
        SDL_Color &c = (*dst)[i];
        c = src[i];
        c.a = 255;

        if (c.r == 255 && c.g == 255 && c.b == 0)
        {
            c.a = 0;
            c.r = 0;
            c.g = 0;
            c.b = 0;
        }
        else
        {
            if (!can_destblend && can_srcblend && transp)
            {
                int mx = (c.r >= c.g) ? (c.r > c.b ? c.r: c.b) : (c.g > c.b ? c.g : c.b);

                if (mx <= 8)
                {
                    c.r = 0;
                    c.g = 0;
                    c.b = 0;
                    c.a = 0;
                }
                else
                {
                    float prm = mx;
                    c.r = 255.0 * (c.r / prm);
                    c.g = 255.0 * (c.g / prm);
                    c.b = 255.0 * (c.b / prm);
                    c.a = mx;
                }
            }
            else
            {
                c.a = 255;
            }
        }
    }
}

SDL_PixelFormat *GFXEngine::GetScreenFormat()
{
    return Screen()->format;
}

SDL_Surface *GFXEngine::CreateSurfaceScreenFormat(int width, int height)
{
    SDL_PixelFormat *fmt = GetScreenFormat();
#if SDL_VERSION_ATLEAST(2,0,5)
    return SDL_CreateRGBSurfaceWithFormat(0, width, height, fmt->BitsPerPixel, fmt->format);
#else
    return SDL_CreateRGBSurface(0, width, height, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
}

SDL_Surface *GFXEngine::ConvertToScreenFormat(SDL_Surface *src)
{
    return ConvertSDLSurface(src, Screen()->format);
}

SDL_Surface * GFXEngine::ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt)
{
#if (SDL_COMPILEDVERSION == SDL_VERSIONNUM(2, 0, 12))
    /***
     * Workaround for bug with convertation of surface with palette introduced
     * in SDL2 2.0.12 and fixed soon but after release.
     ***/
    if (src->format->BytesPerPixel == 1)
    {
#if SDL_VERSION_ATLEAST(2,0,5)
        SDL_Surface *tmp = SDL_CreateRGBSurfaceWithFormat(0, src->w, src->h, fmt->BitsPerPixel, fmt->format);
#else
        SDL_Surface *tmp = SDL_CreateRGBSurface(0, src->w, src->h, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
        SDL_BlendMode blend = SDL_BLENDMODE_NONE;
        SDL_GetSurfaceBlendMode(src, &blend);
        SDL_SetSurfaceBlendMode(src, SDL_BLENDMODE_NONE);
        SDL_BlitSurface(src, NULL, tmp, NULL);
        SDL_SetSurfaceBlendMode(src, blend);
        return tmp;
    }
    else
        return SDL_ConvertSurface(src, fmt, 0);
#else
    return SDL_ConvertSurface(src, fmt, 0);
#endif
}

void GFXEngine::fpsLimitter(int value)
{
    if (value > 1000)
        FpsMaxTicks = 0;
    else if (value <= 0)
        FpsMaxTicks = 0;
    else
        FpsMaxTicks = 1000/value;
}


}


TileMap::TileMap()
{
    img = NULL;
    h = 0;
}
    
TileMap::~TileMap()
{
    if (img)
        Nucleus::Delete(img);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Point &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::PointRect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.w)
        src.w = pos.w;
    if (src.h > pos.h)
        src.h = pos.h;    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Rect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.Width())
        src.w = pos.Width();
    if (src.h > pos.Height())
        src.h = pos.Height();    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Fill(SDL_Surface *surface, const Common::Rect &rect, uint8_t c)
{
    GFX::Engine.DrawFill(img->GetSwTex(), map[c], surface, rect);
}

void TileMap::Fill(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c)
{
    GFX::Engine.DrawFill(img->GetSwTex(), map[c], surface, rect);
}

void TileMap::FillColumn(SDL_Surface *surface, const Common::Rect &rect, uint8_t c)
{
    Common::PointRect sPRect = map[c];
    sPRect.w = 1;
    GFX::Engine.DrawFill(img->GetSwTex(), sPRect, surface, rect);
}

void TileMap::FillColumn(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c)
{
    Common::PointRect sPRect = map[c];
    sPRect.w = 1;
    GFX::Engine.DrawFill(img->GetSwTex(), sPRect, surface, rect);
}

int TileMap::GetWidth(uint8_t c) const
{
    return map[c].w;
}

int TileMap::GetWidth(const std::string &str) const
{
    int wdth = 0;
    for (uint8_t c : str)
        wdth += map[c].w;
    return wdth;
}

Common::Point TileMap::GetSize(uint8_t c) const
{
    return Common::Point(map[c].w, h);
}












namespace GFX
{
    
GfxMode::GfxMode()
{
    w = 0;
    h = 0;
    bpp = 0;
    mode = {0};
    windowed = false;
    sortid = 0;
    name = "";
}

GfxMode::GfxMode(GfxMode &&g)
{
    w = std::move(g.w);
    h = std::move(g.h);
    bpp = std::move(g.bpp);
    mode = std::move(g.mode);
    windowed = std::move(g.windowed);
    sortid = std::move(g.sortid);
    name = std::move(g.name);
}

GfxMode::GfxMode(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    windowed = g.windowed;
    sortid = g.sortid;
    name = g.name;
}

GfxMode& GfxMode::operator=(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    windowed = g.windowed;
    sortid = g.sortid;
    name = g.name;
    return *this;
}

GfxMode::operator bool() const
{
    if (w == 0 || h == 0 || bpp == 0)
        return false;
    return true;
}

bool GfxMode::Compare(const GfxMode &a, const GfxMode &b)
{
    return (a.sortid > b.sortid);
}




void GFXEngine::AddGfxMode(const GfxMode &md)
{
    for (const GfxMode &m : graphicsModes)
    {
        if ( m.sortid == md.sortid )
            return;
    }

    graphicsModes.push_back(md);
    log_d3dlog("Add display mode: %s%dx%d\n", (md.windowed ? "Windowed ": ""), md.w, md.h );
}


uint32_t GFXEngine::CursPix(uint8_t *data, int ofs, int bpp)
{
    switch (bpp)
    {
    case 1:
        return (data[ofs / 8] >> (7 - ofs % 8)) & 1;
    case 2:
        return (data[ofs / 4] >> ((3 - ofs % 4) << 1)) & 3;
    case 4:
        return (data[ofs / 2] >> ((1 - ofs % 2) << 2)) & 15;
    case 8:
        return data[ofs];
    case 16:
        return data[2 * ofs] | data[2 * ofs + 1] << 8;
    case 24:
        return data[3 * ofs] | data[3 * ofs + 1] << 8 | data[ 3 * ofs + 2] << 16;
    case 32:
        return data[4 * ofs] | data[4 * ofs + 1] << 8 | data[4 * ofs + 2] << 16 | data[4 * ofs + 3] << 24;
    }

    return 0;
}

SDL_Cursor *GFXEngine::LoadCursor(const std::string &name)
{
    FSMgr::FileHandle *fil = uaOpenFile( fmt::sprintf("res/%s.cur", name) , "rb");

    UA_PALETTE pal;

    if (!fil)
        return NULL;

    fil->readU16L();
    if (fil->readU16L() != 2)
    {
        delete fil;
        return NULL;
    }

    fil->readU16L(); //count

    //Only first entry
    fil->readU8(); //w
    fil->readU8(); //h
    fil->readU8(); //color count
    fil->readU8(); //reserved
    int hotX = fil->readU16L();
    int hotY = fil->readU16L();
    fil->readU32L(); //size
    int off = fil->readU32L();

    //seek to cursor
    fil->seek(off, SEEK_SET);

    //read InfoHeader
    fil->readU32L(); //header size
    int bmpw = fil->readS32L();
    int bmph = fil->readS32L();
    fil->readU16L(); //planes
    int bitcount = fil->readU16L();
    fil->readU32L(); //compression
    fil->readU32L(); //imagesize
    fil->readU32L(); //XpixelsPerM
    fil->readU32L(); //YpixelsPerM
    int clrused = fil->readU32L(); //ColorsUsed
    fil->readU32L(); //ColorsImportant

    // read pallete
    int palcnt = 0;
    if (clrused == 0 || bitcount < 16)
    {
        palcnt = clrused != 0 ? clrused : 1 << bitcount;

        for (int i = 0; i < palcnt; i++)
        {
            pal[i].r = fil->readU8();
            pal[i].g = fil->readU8();
            pal[i].b = fil->readU8();
            fil->readU8(); //reserved
        }
    }

    int width = bmpw;
    int height = abs(bmph)/2;

    int imgsz = height * width * bitcount / 8;
    int mask_size = height * width / 8;

    uint8_t *data = (uint8_t *)malloc(imgsz);
    uint8_t *mask = (uint8_t *)malloc(mask_size);

    fil->read(data, imgsz);
    fil->read(mask, mask_size);

    delete fil;


    SDL_Surface *cursr = SDL_CreateRGBSurface(0, width, height, 32, 0xFF, 0xFF00, 0xFF0000, 0xFF000000);

    for (int y = 0; y < height; y++)
    {
        int invY = height - 1 - y;
        uint8_t *row = (uint8_t *)cursr->pixels + y * cursr->pitch;

        for (int x = 0; x < width; x++)
        {
            if (palcnt > 0)
            {
                int idx = CursPix(data, invY * width + x, bitcount);
                int alph = CursPix(mask, invY * width + x, 1);
                row[x * 4 + 0] = pal[ idx ].r;
                row[x * 4 + 1] = pal[ idx ].g;
                row[x * 4 + 2] = pal[ idx ].b;
                row[x * 4 + 3] = (1 - alph) * 255;
            }
            else
            {
                uint32_t clr = CursPix(data, invY * width + x, bitcount);
                int alph = CursPix(mask, invY * width + x, 1);
                row[x * 4 + 0] = clr & 0xFF;
                row[x * 4 + 1] = (clr >> 8) & 0xFF;
                row[x * 4 + 2] = (clr >> 16) & 0xFF;
                row[x * 4 + 3] = (1 - alph) * 255;
            }
        }
    }

    free(data);
    free(mask);

    SDL_Cursor* cursor = SDL_CreateColorCursor(cursr, hotX, hotY);

    SDL_FreeSurface(cursr);

    return cursor;
}

void GFXEngine::Init()
{
    RecreateScreenSurface();
    System::EventsAddHandler(EventsWatcher);
    
    System::IniConf::ReadFromNucleusIni();
    
    SDL_DisplayMode deskMode;
    SDL_GetDesktopDisplayMode(0, &deskMode);

    int checkmodes[][2] =
    {
        {640, 480}, {800, 600}, {1024, 768}, {1280, 1024}, {1440, 1050}, {1600, 1200},
        {720, 480}, {852, 480}, {1280, 720}, {1366, 768}, {1600, 900}, {1920, 1080}, {2560, 1440},
        {-1, -1}
    };

    uint32_t corrected = CorrectSurfaceFormat(deskMode.format);

    for(int i = 0; checkmodes[i][0] != -1; i++)
    {
        SDL_DisplayMode target, closest;

        target.w = checkmodes[i][0];
        target.h = checkmodes[i][1];
        target.format = corrected;
        target.refresh_rate = 0;
        target.driverdata   = 0;

        if (SDL_GetClosestDisplayMode(0, &target, &closest) )
        {
            GfxMode mode;
            mode.w = closest.w;
            mode.h = closest.h;
            mode.mode = closest;
            mode.bpp = SDL_BYTESPERPIXEL(closest.format) * 8;
            mode.windowed = false;
            mode.name = fmt::sprintf("%d x %d", mode.w, mode.h);

            mode.sortid = (closest.w & 0x7FFF) << 7 | (closest.h & 0x7FFF);

            AddGfxMode(mode);
        }

        if (checkmodes[i][0] <= deskMode.w && checkmodes[i][1] <= deskMode.h)
        {
            GfxMode mode;
            mode.w = checkmodes[i][0];
            mode.h = checkmodes[i][1];
            mode.mode = deskMode;
            mode.bpp = SDL_BYTESPERPIXEL(corrected) * 8;
            mode.windowed = true;
            mode.name = fmt::sprintf("Windowed %d x %d", mode.w, mode.h);

            mode.sortid = 0x40000000 | (checkmodes[i][0] & 0x7FFF) << 7 | (checkmodes[i][1] & 0x7FFF);

            AddGfxMode(mode);
        }
    }

    graphicsModes.sort(GfxMode::Compare);
    
    cursors[0] = LoadCursor("Pointer");
    cursors[1] = LoadCursor("Cancel");
    cursors[2] = LoadCursor("Select");
    cursors[3] = LoadCursor("Attack");
    cursors[4] = LoadCursor("Goto");
    cursors[5] = LoadCursor("Disk");
    cursors[6] = LoadCursor("New");
    cursors[7] = LoadCursor("Add");
    cursors[8] = LoadCursor("Control");
    cursors[9] = LoadCursor("Beam");
    cursors[10] = LoadCursor("Build");

    IDVList stak { {ATT_WIDTH,  (int32_t)640},
            {ATT_HEIGHT, (int32_t)480} };
    func0( stak );

    Gui::Instance.SetScreenSize(GetSize());
    portalID = Gui::Instance.AddPortal(Common::Point(640, 480), Common::Rect(0, 0, 640, 480));
    scrCompat = new Gui::OldCompat(Common::Point(640, 480));
    Gui::Instance.AddWidgetPortal(portalID, scrCompat);
    
    LoadPalette(System::IniConf::GfxPalette.Get<std::string>());
    
    _colorEffects = System::IniConf::GfxColorEffects.Get<int32_t>();
    
    if (_colorEffects > 0)
    {
        GLGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)SDL_GL_GetProcAddress("glGenFramebuffers");
        GLBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)SDL_GL_GetProcAddress("glBindFramebuffer");
        GLGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)SDL_GL_GetProcAddress("glGenRenderbuffers");
        GLBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)SDL_GL_GetProcAddress("glBindRenderbuffer");
        GLFrameBufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)SDL_GL_GetProcAddress("glFramebufferTexture");
        GLFrameBufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)SDL_GL_GetProcAddress("glFramebufferTexture2D");
        GLRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)SDL_GL_GetProcAddress("glRenderbufferStorage");
        GLFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)SDL_GL_GetProcAddress("glFramebufferRenderbuffer");
        GLCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)SDL_GL_GetProcAddress("glCheckFramebufferStatus");
        GLGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)SDL_GL_GetProcAddress("glGenerateMipmap");
        GLCreateShader = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress("glCreateShader");
        GLShaderSource = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress("glShaderSource");
        GLCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress("glCompileShader");
        GLDeleteShader = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress("glDeleteShader");
        GLCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress("glCreateProgram");
        GLAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress("glAttachShader");
        GLLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress("glLinkProgram");
        GLUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress("glUseProgram");
        GLGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress("glGetUniformLocation");
        
        GLUniform1f = (PFNGLUNIFORM1FPROC)SDL_GL_GetProcAddress("glUniform1f");
        GLUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress("glUniform1i");
        GLUniform2f = (PFNGLUNIFORM2FPROC)SDL_GL_GetProcAddress("glUniform2f");
        GLUniform2i = (PFNGLUNIFORM2IPROC)SDL_GL_GetProcAddress("glUniform2i");
        GLUniform3f = (PFNGLUNIFORM3FPROC)SDL_GL_GetProcAddress("glUniform3f");
        GLUniform3i = (PFNGLUNIFORM3IPROC)SDL_GL_GetProcAddress("glUniform3i");
        GLUniform4f = (PFNGLUNIFORM4FPROC)SDL_GL_GetProcAddress("glUniform4f");
        GLUniform4i = (PFNGLUNIFORM4IPROC)SDL_GL_GetProcAddress("glUniform4i");
        
        GLGetShaderiv = (PFNGLGETSHADERIVPROC)SDL_GL_GetProcAddress("glGetShaderiv");
        GLGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress("glGetShaderInfoLog");
        
        
        GLGenFramebuffers(1, &_fbo);
        GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        glGenTextures(1, &_fboTex);
        glBindTexture(GL_TEXTURE_2D, _fboTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);


        GLGenRenderbuffers(1, &_fbod);
        GLBindRenderbuffer(GL_RENDERBUFFER, _fbod);
        GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 480);

        
        GLFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _fbod);

        GLFrameBufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTex, 0);

        GLBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        GLBindFramebuffer(GL_FRAMEBUFFER, 0);


        _psShader = LoadShader(GL_FRAGMENT_SHADER, "res/clreff.ps");
        _vsShader = LoadShader(GL_VERTEX_SHADER, "res/clreff.vs");

        _shaderProg = GLCreateProgram();
        GLAttachShader(_shaderProg, _psShader);
        GLAttachShader(_shaderProg, _vsShader);
        GLLinkProgram(_shaderProg);
        
        _shdrIDNorm = GLGetUniformLocation(_shaderProg, "normclr");
        _shdrIDInv = GLGetUniformLocation(_shaderProg, "invclr");
        _shdrIDrand = GLGetUniformLocation(_shaderProg, "randval");
        _shdrIDscrsize = GLGetUniformLocation(_shaderProg, "screenSize");
        _shdrIDmillisecs = GLGetUniformLocation(_shaderProg, "millisecs");
    }
    
}

void GFXEngine::Deinit()
{
    if (_pixfmt)
    {
        SDL_FreeFormat(_pixfmt);
        _pixfmt = NULL;
    }

    if ( _font.ttfFont )
    {
        TTF_CloseFont(_font.ttfFont);
        _font.ttfFont = NULL;
    }
    
    if (ScreenSurface)
        SDL_FreeSurface(ScreenSurface);
    
    if (scrCompat)
        delete scrCompat;
    
    ScreenSurface = NULL;
}

int GFXEngine::EventsWatcher(void *, SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_WINDOWEVENT:
    {
        switch(event->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            Instance.RecreateScreenSurface();
            Instance.UpdateFBOSizes();
            Gui::Instance.SetScreenSize(System::GetResolution());
            Gui::Instance.SetPortal(Instance.portalID, Common::Rect(System::GetResolution()));
            break;
        default:
            break;
        }

    }
    break;
    }
    
    return 1; // This event can be passed to another event watcher
}

void GFXEngine::SetCursor(int curID, int force)
{
    int sett = 0;

    if ( force )
        sett = 1;
    else if ( curID != CurrentCursorID )
        sett = 1;

    if ( sett )
    {
        if ( curID == 0 )
            SDL_ShowCursor(SDL_DISABLE);
        else if ( curID <= 11 )
        {
            if ( cursors[curID - 1] )
                SDL_SetCursor( cursors[curID - 1] );

            if (!CurrentCursorID)
                SDL_ShowCursor(SDL_ENABLE);
        }

    }

    CurrentCursorID = curID;
}

GfxMode GFXEngine::sub_41F68C()
{
    for (const GfxMode &m : graphicsModes)
    {
        if (m.w == 640 && m.h == 480)
            return m;
    }

    return graphicsModes.front();
}

int GFXEngine::GetScreenH()
{
    return GetHeight();
}

int GFXEngine::GetScreenW()
{
    return GetWidth();
}


GfxMode GFXEngine::windd_func0__sub0(const std::string &file)
{
    FSMgr::FileHandle *fil = uaOpenFile(file, "r");

    if ( fil )
    {
        std::string line;
        if ( fil->ReadLine(&line) )
        {
            size_t pos = line.find_first_of("\n\r");
            
            if (pos != std::string::npos)
                line.erase(pos);

            for (const GfxMode &m : graphicsModes)
            {
                if ( StriCmp(m.name, line) == 0 )
                    return m;
            }
        }
        delete fil;
    }

    return sub_41F68C();
}

void GFXEngine::SetResolution(int res)
{
    UA_PALETTE *screen_palette = GetPalette();

    UA_PALETTE palette_copy;

    if ( screen_palette )
        palette_copy = *screen_palette; // Copy palette
    else
    {
        for(auto &x : palette_copy)
        {
            x.r = 0;
            x.g = 0;
            x.b = 0;
            x.a = 0;
        }
    }

    EndFrame();

    //delete_class_obj(cls3D);
    
    GfxMode picked;
    if ( res )
    {
        for (const GfxMode &m : graphicsModes)
        {
            if ( m.sortid == res )
            {
                picked = m;
                break;
            }
        }

        if ( !picked )
            picked = sub_41F68C();
    }
    else
    {
        picked = windd_func0__sub0("env/vid.def");
    }
    
    if (!picked.windowed)
        System::SetVideoMode(Common::Point(picked.w, picked.h), SDL_WINDOW_FULLSCREEN_DESKTOP, &picked.mode);
    else
        System::SetVideoMode(Common::Point(picked.w, picked.h), 0, NULL);
    
    RecreateScreenSurface();
    
    GfxSelectedMode = picked.sortid;

    if ( ChangeResolution(Common::Point(picked.w, picked.h), picked.windowed) )
    {
        BeginFrame();

        SetPalette(palette_copy);
    }
    
    FSMgr::FileHandle *fil = uaOpenFile("env/vid.def", "w");
    if ( fil )
    {
        fil->printf("%s\n", picked.name.c_str());
        delete fil;
    }
    
    Gui::Instance.ResizePortal(portalID, Common::Point(picked.w, picked.h));
    Gui::Instance.SetScreenSize(System::GetResolution());
    Gui::Instance.SetPortal(portalID, Common::Rect(Common::Point(picked.w, picked.h)));
    
    UpdateFBOSizes();
}



size_t GFXEngine::display_func256(windd_arg256 *inout)
{
    auto it = graphicsModes.begin();

    if ( inout->sort_id )
    {
        while (it != graphicsModes.end())
        {
            if ( it->sortid == inout->sort_id )
                break;
            
            it++;
        }

    }

    if ( it == graphicsModes.end() )
        return 0;

    inout->sort_id = it->sortid;
    inout->width = it->w;
    inout->height = it->h;

    strncpy(inout->name, it->name.c_str(), 32);

    it++;
    
    if (it != graphicsModes.end())
        return it->sortid;
    
    return 0;
}










void GFXEngine::SetTracyRmp(ResBitmap *rmp)
{
    setRSTR_trcRmp(rmp);
}

void GFXEngine::SetShadeRmp(ResBitmap *rmp)
{
    setRSTR_shdRmp(rmp);
}

int32_t GFXEngine::GetGfxMode()
{
    return GfxSelectedMode;
}

TileMap * GFXEngine::GetTileset(int id)
{
    return raster_func208(id);
}

void GFXEngine::DrawText(w3d_a209 *arg)
{
    w3d_a209 arg209;
    arg209 = *arg;
    raster_func209(&arg209);
}

void GFXEngine::SetTileset(TileMap *tileset, int id)
{
    raster_func207(id, tileset);
}


bool GFXEngine::LoadPalette(const std::string &palette_ilbm)
{
    NC_STACK_bitmap *ilbm = Nucleus::CInit<NC_STACK_ilbm>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(palette_ilbm)},
        {NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, (int32_t)1}} );

    if (!ilbm)
        return false;

    SetPalette( *ilbm->getBMD_palette() );

    delete_class_obj(ilbm);

    return true;
}
    
uint8_t *GFXEngine::MakeScreenCopy(int *ow, int *oh)
{
    Common::Point res = System::GetResolution();

    // power of 2
    res.x &= ~1;
    res.y &= ~1;

    uint8_t *buf = (uint8_t *)malloc(res.x * res.y * 3);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, res.x, res.y, GL_RGB, GL_UNSIGNED_BYTE, buf);

    *ow = res.x;
    *oh = res.y;
    return buf;
}

uint8_t *GFXEngine::MakeDepthScreenCopy(int *ow, int *oh)
{
    Common::Point res = System::GetResolution();

    // power of 2
    res.x &= ~1;
    res.y &= ~1;

    uint8_t *buf = (uint8_t *)malloc(res.x * res.y);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, res.x, res.y, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buf);

    *ow = res.x;
    *oh = res.y;
    return buf;
}






SDL_Surface *GFXEngine::RealScreen()
{
    return ScreenSurface;
}

SDL_Surface *GFXEngine::Screen()
{
    return scrCompat->GetSurface();
}



// Draw line Bresenham's algorithm
void GFXEngine::DrawLine(SDL_Surface *surface, const Common::Line &line, uint8_t cr, uint8_t cg, uint8_t cb )
{
    if ((line.Width() == 0 && line.Height() == 0) || 
         !Common::Rect(surface->w, surface->h).IsIn(line.P1()) ||
         !Common::Rect(surface->w, surface->h).IsIn(line.P2()) )
        return;
    
    int rilWidth = surface->pitch / surface->format->BytesPerPixel;

    int xCount = line.Width();
    int yCount = line.Height();

    uint32_t color = SDL_MapRGBA(surface->format, cr, cg, cb, 255);

    int stepAdd, stepOdd;
    int steps, subSteps;

    if ( xCount <= yCount )
    {
        if ( line.y2 <= line.y1 )
            stepAdd = -rilWidth;
        else
            stepAdd = rilWidth;

        if ( line.x2 <= line.x1 )
            stepOdd = -1;
        else
            stepOdd = 1;

        steps = yCount;
        subSteps = xCount;
    }
    else
    {
        if ( line.x2 <= line.x1 )
            stepAdd = -1;
        else
            stepAdd = 1;

        if ( line.y2 <= line.y1 )
            stepOdd = -rilWidth;
        else
            stepOdd = rilWidth;
        
        steps = xCount;
        subSteps = yCount;
    }

    int incr1 = 2 * subSteps;
    int t = 2 * subSteps - steps;
    int incr2 = 2 * (subSteps - steps);
    
    SDL_LockSurface(surface);
    
    void *surfPos = (void *) ((uint8_t *) surface->pixels 
                    + line.y1 * surface->pitch 
                    + line.x1 * surface->format->BytesPerPixel );

    switch(surface->format->BytesPerPixel)
    {
        case 1:
        {
            uint8_t *surf = (uint8_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        case 2:
        {
            uint16_t *surf = (uint16_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        case 4:
        {
            uint32_t *surf = (uint32_t *)surfPos;

            for (int i = 0; i <= steps; i++) // Verify i bound
            {
                *surf = color;
                if ( t > 0 )
                {
                    t += incr2;
                    surf += stepOdd;
                }
                else
                    t += incr1;

                surf += stepAdd;
            }
        }
        break;
        
        default:
        break;
    }
    
    SDL_UnlockSurface(surface);
}

void GFXEngine::BlitScaleMasked(SDL_Surface *src, Common::Rect sRect, SDL_Surface *mask, uint8_t index, SDL_Surface *dst, Common::Rect dRect)
{
    if (mask->format->BitsPerPixel != 8)
        return;
        
    if (src->w != mask->w || src->h != mask->h)
        return;
    
    if (sRect.IsEmpty() || !sRect.IsValid())
        sRect = Common::Rect(src->w, src->h);
    else if (!Common::Rect(src->w, src->h).IsIn(sRect))
        return;
    
    if (dRect.IsEmpty() || !dRect.IsValid())
        dRect = Common::Rect(dst->w, dst->h);
    else if (!Common::Rect(dst->w, dst->h).IsIn(dRect))
        return;
    
    // Try fast
    if (src->format->format == dst->format->format)
    {
        switch(src->format->BytesPerPixel)
        {
            case 2:
            {
                SDL_LockSurface(src);
                SDL_LockSurface(mask);
                SDL_LockSurface(dst);
                
                int32_t dY = (sRect.Height() << 16) / dRect.Height();
                int32_t dX = (sRect.Width()  << 16) / dRect.Width();
                
                int32_t srcY  = sRect.top << 16;
                for (int y = dRect.top; y < dRect.bottom; y++)
                {
                    uint16_t *dBuf = (uint16_t *)((uint8_t *)dst->pixels + y * dst->pitch) + dRect.left;
                    uint16_t *sBuf = (uint16_t *)((uint8_t *)src->pixels + (srcY >> 16) * src->pitch) + sRect.left;
                    uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;
                    
                    int32_t xx = 0;
                    for (int x = dRect.left; x < dRect.right; x++)
                    {
                        if (mBuf[xx >> 16] == index)
                            *dBuf = sBuf[xx >> 16];
                        dBuf++;
                        xx += dX;
                    }
                    srcY += dY;
                }
                
                SDL_UnlockSurface(dst);
                SDL_UnlockSurface(mask);
                SDL_UnlockSurface(src);
            }
            break;
            
            case 4:
            {
                SDL_LockSurface(src);
                SDL_LockSurface(mask);
                SDL_LockSurface(dst);
                
                int32_t dY = (sRect.Height() << 16) / dRect.Height();
                int32_t dX = (sRect.Width()  << 16) / dRect.Width();
                
                int32_t srcY  = sRect.top << 16;
                for (int y = dRect.top; y < dRect.bottom; y++)
                {
                    uint32_t *dBuf = (uint32_t *)((uint8_t *)dst->pixels + y * dst->pitch) + dRect.left;
                    uint32_t *sBuf = (uint32_t *)((uint8_t *)src->pixels + (srcY >> 16) * src->pitch) + sRect.left;
                    uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;
                    
                    int32_t xx = 0;
                    for (int x = dRect.left; x < dRect.right; x++)
                    {
                        if (mBuf[xx >> 16] == index)
                            *dBuf = sBuf[xx >> 16];
                        dBuf++;
                        xx += dX;
                    }
                    srcY += dY;
                }
                
                SDL_UnlockSurface(dst);
                SDL_UnlockSurface(mask);
                SDL_UnlockSurface(src);
            }
            break;
            
            default:
            break;
        }
    }
    else // Slow
    {
        SDL_LockSurface(src);
        SDL_LockSurface(mask);
        SDL_LockSurface(dst);

        uint8_t sbpp = src->format->BytesPerPixel;
        uint8_t dbpp = dst->format->BytesPerPixel;
        
        int32_t dY = (sRect.Height() << 16) / dRect.Height();
        int32_t dX = (sRect.Width()  << 16) / dRect.Width();

        int32_t srcY  = sRect.top << 16;
        for (int y = dRect.top; y < dRect.bottom; y++)
        {
            uint8_t *dBuf = (uint8_t *)dst->pixels + y * dst->pitch + dRect.left * dbpp;
            uint8_t *sBuf = (uint8_t *)src->pixels + (srcY >> 16) * src->pitch + sRect.left * sbpp;
            uint8_t  *mBuf = (uint8_t *)mask->pixels + (srcY >> 16) * mask->pitch + sRect.left;

            int32_t xx = 0;
            for (int x = dRect.left; x < dRect.right; x++)
            {
                if (mBuf[xx >> 16] == index)
                {
                    uint8_t r,g,b;
                    uint32_t clr = 0;
                    
                    uint8_t *spix = sBuf + (xx >> 16) * sbpp;
                    for(int i = 0; i < sbpp; i++)
                        clr |= spix[i] << (i * 8);
                    
                    SDL_GetRGB(clr, src->format, &r, &g, &b);
                    clr = SDL_MapRGB(dst->format, r, g, b);
                    
                    for(int i = 0; i < dbpp; i++)
                        dBuf[i] = (clr >> (i * 8)) & 0xFF;
                }
                dBuf += dbpp;
                xx += dX;
            }
            srcY += dY;
        }

        SDL_UnlockSurface(dst);
        SDL_UnlockSurface(mask);
        SDL_UnlockSurface(src);
    }
}

void GFXEngine::DrawFillIntCpy(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect)
{
    const int32_t sh = sRect.Height();
    const int32_t sw = sRect.Width();
    const int32_t dh = dRect.Height();
    const int32_t dw = dRect.Width();
        
    SDL_LockSurface(src);
    SDL_LockSurface(dst);
                    
    if (dw > sw)
    {
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int32_t nit = dw / sw;
        const int32_t nit2 = (dw % sw) * dst->format->BytesPerPixel;
        const int32_t cpsz = sw * dst->format->BytesPerPixel;
        const int32_t skipdst = dst->pitch - dw * dst->format->BytesPerPixel + nit2;
                
        int sptch = src->pitch;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {
            for(int n = nit; n > 0; --n)
            {
                memcpy(pdst, psrcl, cpsz);
                pdst += cpsz;
            }
            
            memcpy(pdst, psrcl, nit2);
            pdst += skipdst;
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
        }        
    }
    else
    {
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int32_t cpsz = dw * dst->format->BytesPerPixel;
        const int32_t skipdst = dst->pitch;
                
        int sptch = src->pitch;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {            
            memcpy(pdst, psrcl, cpsz);
            pdst += skipdst;
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
        }        
    }
                
    SDL_UnlockSurface(dst);
    SDL_UnlockSurface(src);
}
    
template <typename T>
void GFXEngine::DrawFillIntCKey(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect)
{
    const int32_t sh = sRect.Height();
    const int32_t sw = sRect.Width();
    const int32_t dh = dRect.Height();
    const int32_t dw = dRect.Width();
        
    SDL_LockSurface(src);
    SDL_LockSurface(dst);
    
    uint32_t clrKey = 0;
    SDL_GetColorKey(src, &clrKey);
                
    if (dw > sw)
    {
        const int32_t skipdst = dst->pitch - dw * dst->format->BytesPerPixel;
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int32_t nit = dw / sw;
        const int32_t nit2 = dw % sw;
        
        const int sz = dst->format->BytesPerPixel;
        
        int sptch = src->pitch;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {
            
            for(int n = nit; n > 0; --n)
            {
                const uint8_t *lsrc = psrcl;
                for (int iw = sw; iw > 0; --iw)
                {
                    if (*(T *)lsrc != clrKey)
                        *(T *)pdst = *(T *)lsrc;
                    
                    lsrc += sz;
                    pdst += sz;
                }
            }
            
            uint8_t *rsrc = psrcl;            
            for(int iw = nit2; iw > 0; --iw)
            {
                if (*(T *)rsrc != clrKey)
                    *(T *)pdst = *(T *)rsrc;

                rsrc += sz;
                pdst += sz;
            }
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
            
            pdst += skipdst;
        }        
    }
    else
    {
        const int32_t skipdst = dst->pitch - dw * dst->format->BytesPerPixel;
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int sz = dst->format->BytesPerPixel;
        
        int sptch = src->pitch;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {            
            uint8_t *rsrc = psrcl;            
            for(int iw = dw; iw > 0; --iw)
            {
                if (*(T *)rsrc != clrKey)
                    *(T *)pdst = *(T *)rsrc;

                rsrc += sz;
                pdst += sz;
            }
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
            
            pdst += skipdst;
        }        
    }
                
    SDL_UnlockSurface(dst);
    SDL_UnlockSurface(src);
}

template <typename T>
void GFXEngine::DrawFillIntACpy(SDL_Surface* src, const Common::Rect& sRect, SDL_Surface* dst, const Common::Rect& dRect)
{
    const int32_t sh = sRect.Height();
    const int32_t sw = sRect.Width();
    const int32_t dh = dRect.Height();
    const int32_t dw = dRect.Width();
        
    SDL_LockSurface(src);
    SDL_LockSurface(dst);
    
    if (dw > sw)
    {
        const int32_t skipdst = dst->pitch - dw * dst->format->BytesPerPixel;
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int32_t nit = dw / sw;
        const int32_t nit2 = dw % sw;
        uint32_t amask = src->format->Amask;
        
        const int sz = dst->format->BytesPerPixel;
        
        int sptch = src->pitch;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {
            
            for(int n = nit; n > 0; --n)
            {
                const uint8_t *lsrc = psrcl;
                for (int iw = sw; iw > 0; --iw)
                {
                    if (*(T *)lsrc & amask)
                        *(T *)pdst = *(T *)lsrc;
                    
                    lsrc += sz;
                    pdst += sz;
                }
            }
            
            uint8_t *rsrc = psrcl;            
            for(int iw = nit2; iw > 0; --iw)
            {
                if (*(T *)rsrc & amask)
                    *(T *)pdst = *(T *)rsrc;

                rsrc += sz;
                pdst += sz;
            }
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
            
            pdst += skipdst;
        }        
    }
    else
    {
        const int32_t skipdst = dst->pitch - dw * dst->format->BytesPerPixel;
        uint8_t *psrc = (uint8_t *)src->pixels + sRect.top * src->pitch + sRect.left * src->format->BytesPerPixel;
        uint8_t *pdst = (uint8_t *)dst->pixels + dRect.top * dst->pitch + dRect.left * dst->format->BytesPerPixel;
        
        const int sz = dst->format->BytesPerPixel;
        
        int sptch = src->pitch;
        uint32_t amask = src->format->Amask;
        
        int ln = 0;
        uint8_t *psrcl = psrc;
        for(int ih = dh; ih > 0; --ih)
        {            
            uint8_t *rsrc = psrcl;            
            for(int iw = dw; iw > 0; --iw)
            {
                if (*(T *)rsrc & amask)
                    *(T *)pdst = *(T *)rsrc;

                rsrc += sz;
                pdst += sz;
            }
            
            ++ln;
            if (ln >= sh)
            {
                ln = 0;
                psrcl = psrc;
            }
            else
                psrcl += sptch;
            
            pdst += skipdst;
        }        
    }
                
    SDL_UnlockSurface(dst);
    SDL_UnlockSurface(src);
}

void GFXEngine::DrawFill(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect)
{
    if (sRect.IsEmpty() || dRect.IsEmpty())
        return;
    
    if (src->format->format == dst->format->format)
    {     
        if (SDL_HasColorKey(src))
        {
            switch(src->format->BytesPerPixel)
            {

                case 2:
                    DrawFillIntCKey<uint16_t>(src, sRect, dst, dRect);
                    break;

                case 4:
                    DrawFillIntCKey<uint32_t>(src, sRect, dst, dRect);
                    break;

                default:
                break;
            }
        }
        else
        {
            if (src->format->Amask)
            {
                switch(src->format->BytesPerPixel)
                {

                    case 2:
                        DrawFillIntACpy<uint16_t>(src, sRect, dst, dRect);
                        break;

                    case 4:
                        DrawFillIntACpy<uint32_t>(src, sRect, dst, dRect);
                        break;

                    default:
                    break;
                }
            }
            else
                DrawFillIntCpy(src, sRect, dst, dRect);
        }  
    }
    else
    {
        SDL_Rect lsrc = sRect;
        SDL_Rect ldst;
        for(ldst.y = dRect.top; ldst.y < dRect.bottom; ldst.y += lsrc.h)
        {
            if (dRect.bottom - ldst.y < lsrc.h)
                lsrc.h = dRect.bottom - ldst.y;

            lsrc.w = sRect.Width();

            for(ldst.x = dRect.left; ldst.x < dRect.right; ldst.x += lsrc.w)
            {
                if (dRect.right - ldst.x < lsrc.w)
                    lsrc.w = dRect.right - ldst.x;

                SDL_BlitSurface(src, &lsrc, dst, &ldst);
            }
        }
    }
}

void GFXEngine::Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::Point dPoint)
{
    SDL_Rect Ssrc = sRect;
    SDL_Rect Sdst = dPoint;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

void GFXEngine::Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::PointRect dRect)
{
    SDL_Rect Ssrc = sRect;
    if (Ssrc.w > dRect.w)
        Ssrc.w = dRect.w;
    if (Ssrc.h > dRect.h)
        Ssrc.h = dRect.h;
    SDL_Rect Sdst = dRect;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

uint32_t GFXEngine::CorrectSurfaceFormat(uint32_t format)
{
    if (format == SDL_PIXELFORMAT_BGR888)
        return SDL_PIXELFORMAT_ABGR8888;
    else if (format == SDL_PIXELFORMAT_RGB888)
        return SDL_PIXELFORMAT_ARGB8888;
    return format;
}

void GFXEngine::GLMapFormat(uint32_t pixelFormat, GLint *format, GLint *type)
{
    switch(pixelFormat)
    {
    case SDL_PIXELFORMAT_ARGB8888:
        *format = GL_BGRA;
        *type = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;

    case SDL_PIXELFORMAT_ABGR8888:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_INT_8_8_8_8_REV;
        break;

    case SDL_PIXELFORMAT_BGRA8888:
        *format = GL_BGRA;
        *type = GL_UNSIGNED_BYTE;
        break;

    case SDL_PIXELFORMAT_RGBA8888:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_BYTE;
        break;

    default:
        *format = GL_RGBA;
        *type = GL_UNSIGNED_BYTE;
        break;
    }
}

void GFXEngine::RecreateScreenSurface()
{
    if ( ScreenSurface )
        SDL_FreeSurface(ScreenSurface);

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    mode.format = CorrectSurfaceFormat(mode.format);

    int bpp;
    uint32_t rm, gm, bm, am;
    SDL_PixelFormatEnumToMasks(mode.format, &bpp, &rm, &gm, &bm, &am);

    GLMapFormat(mode.format, &pixfmt, &pixtype);
    
    Common::Point res = System::GetResolution();
    ScreenSurface = SDL_CreateRGBSurface(0, res.x, res.y, bpp, rm, gm, bm, am);

    if ( !screenTex )
        glGenTextures(1, &screenTex);

    glPushAttrib(GL_TEXTURE_BIT);

    glBindTexture(GL_TEXTURE_2D, screenTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, ScreenSurface->w, ScreenSurface->h, 0, pixfmt, pixtype, NULL);

    glPopAttrib();
}

void GFXEngine::DrawScreenSurface()
{
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);
    
    glPushAttrib(GL_LIGHTING | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT | GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //GL_ONE_MINUS_SRC_ALPHA

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, screenTex);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenSurface->w, ScreenSurface->h, pixfmt, pixtype, ScreenSurface->pixels);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glColor3f(1,1,1);
    
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 0);
        glVertex3f(-1.0, 1.0, 0);
        glTexCoord2f(0, 1);
        glVertex3f(-1.0, -1.0, 0);
        glTexCoord2f(1, 1);
        glVertex3f(1.0, -1.0, 0);
        glTexCoord2f(1, 0);
        glVertex3f(1.0, 1.0, 0);
    }
    glEnd();

    glPopAttrib();
}

uint32_t GFXEngine::LoadShader(int32_t type, const std::string &fl)
{
    GLuint sh = GLCreateShader(type);
    if (!sh)
        return 0;
    
    FSMgr::FileHandle *f = FSMgr::iDir::openFile(fl, "rb");
    if (!f)
    {
        GLDeleteShader(sh);
        return 0;
    }

    f->seek(0, SEEK_END);
    size_t sz = f->tell();
    f->seek(0, SEEK_SET);

    char *tmp = new char[sz];
    f->read(tmp, sz);

    std::string b;
    b.assign(tmp, sz);

    delete[] tmp;

    delete f;
    
    const GLchar *source = (const GLchar *)b.c_str();
    
    GLShaderSource(sh, 1, &source, 0);
    GLCompileShader(sh);
    
    GLint tmpvar;
    GLGetShaderiv(sh, GL_COMPILE_STATUS, &tmpvar);
    
    if (tmpvar == GL_FALSE)
    {
        GLGetShaderiv(sh, GL_INFO_LOG_LENGTH, &tmpvar);
        if (tmpvar > 0)
        {
            char *logbuff = new char[tmpvar + 2];
            GLGetShaderInfoLog(sh, tmpvar, NULL, logbuff);
            printf("%s\n", logbuff);
            delete[] logbuff;
        }
        GLDeleteShader(sh);
        return 0;
    }
    
    return sh;
}

void GFXEngine::DrawFBO()
{
    GLUseProgram(_shaderProg);
    
    Common::Point scrSz = System::GetResolution();
    
    if (_shdrIDNorm >= 0)
        GLUniform3f(_shdrIDNorm, _normClr.x, _normClr.y, _normClr.z);
    
    if (_shdrIDInv >= 0)
        GLUniform3f(_shdrIDInv, _invClr.x, _invClr.y, _invClr.z);
    
    if (_shdrIDrand >= 0)
        GLUniform1i(_shdrIDrand, rand());
    
    if (_shdrIDscrsize >= 0)
        GLUniform2i(_shdrIDscrsize, scrSz.x, scrSz.y);
    
    if (_shdrIDmillisecs >= 0)
        GLUniform1i(_shdrIDmillisecs, SDL_GetTicks());
    
    
    glViewport(0, 0, scrSz.x, scrSz.y);
    
    glPushAttrib(GL_LIGHTING | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT | GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_LIGHTING);

    glEnable(GL_BLEND);
    if (_fboBlend == 0)
        glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    else if (_fboBlend == 1)
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, _fboTex);
    
    glColor4f(1,1,1, 1);
    
    glBegin(GL_QUADS);
    {
        glTexCoord2f(0, 1);
        glVertex3f(-1.0, 1.0, 0);
        glTexCoord2f(0, 0);
        glVertex3f(-1.0, -1.0, 0);
        glTexCoord2f(1, 0);
        glVertex3f(1.0, -1.0, 0);
        glTexCoord2f(1, 1);
        glVertex3f(1.0, 1.0, 0);
    }
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, 0);

    glPopAttrib();
    GLUseProgram(0);
}
 
void GFXEngine::SetFBOBlending(int mode)
{
    _fboBlend = mode;
}

void GFXEngine::UpdateFBOSizes()
{
    if (_colorEffects)
    {
        Common::Point scrSz = System::GetResolution();

        glBindTexture(GL_TEXTURE_2D, _fboTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, scrSz.x, scrSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);

        GLBindRenderbuffer(GL_RENDERBUFFER, _fbod);
        GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, scrSz.x, scrSz.y);
        GLBindRenderbuffer(GL_RENDERBUFFER, 0);
    }
}

SDL_Color GFXEngine::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_Color tmp;
    tmp.a = a;
    tmp.r = r;
    tmp.g = g;
    tmp.b = b;
    return tmp;
}

bool GFXEngine::ColorCmp(const SDL_Color &a, const SDL_Color &b)
{
    if (a.a != b.a || a.r != b.r || a.g != b.g || a.b != b.b)
        return false;
    return true;
}

float GFXEngine::GetColorEffectPower(int id)
{
    int32_t pwr = 0;
    switch(id)
    {
        default:
        case 0:
            return 1.0;
        
        case 1:
            pwr = System::IniConf::GfxColorEffPower1.Get<int32_t>();
            break;
        
        case 2:
            pwr = System::IniConf::GfxColorEffPower2.Get<int32_t>();
            break;
        
        case 3:
            pwr = System::IniConf::GfxColorEffPower3.Get<int32_t>();
            break;
        
        case 4:
            pwr = System::IniConf::GfxColorEffPower4.Get<int32_t>();
            break;
            
        case 5:
            pwr = System::IniConf::GfxColorEffPower5.Get<int32_t>();
            break;
            
        case 6:
            pwr = System::IniConf::GfxColorEffPower6.Get<int32_t>();
            break;
            
        case 7:
            pwr = System::IniConf::GfxColorEffPower7.Get<int32_t>();
            break;
    }
    
    if (pwr < 0)
        pwr = 0;
    
    if (pwr > 100)
        pwr = 100;
    
    return (float)pwr / 100.0;
}


}