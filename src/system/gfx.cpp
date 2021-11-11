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
#include "../font.h"
#include "inivals.h"
#include "glfuncs.h"

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


bool TRenderNode::CompareSolid(TRenderNode *a, TRenderNode *b)
{
    if ( !a->Mesh )
        return true;
    
    if ( !b->Mesh )
        return false;
    
    return a->Tex < b->Tex;
}

bool TRenderNode::CompareTransparent(TRenderNode* a, TRenderNode* b)
{
    return a->Distance > b->Distance;
}

bool TRenderNode::CompareDistance(TRenderNode* a, TRenderNode* b)
{
    return a->Distance < b->Distance;
}

bool TRenderParams::operator==(const TRenderParams &b)
{
    if (Flags != b.Flags)
        return false;

    if (Flags & RFLAGS_DYNAMIC_TEXTURE)
        return TexSource->IsSameRes( b.TexSource );

    return Tex == b.Tex;
}



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
    FSMgr::FileHandle *fil = uaOpenFileAlloc(filename, "w");
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
    FSMgr::FileHandle *fil = uaOpenFileAlloc(filename, "w");
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
    _states = GfxStates();
    _states.LinearFilter = (_filter != 0);

    SetRenderStates(1);

    glEnable(GL_CULL_FACE);

    if (_dither)
        glEnable(GL_DITHER);
    else
        glDisable(GL_DITHER);
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

    SetResVariables( Common::Point(stak.Get<int32_t>(ATT_WIDTH, DEFAULT_WIDTH), stak.Get<int32_t>(ATT_HEIGHT, DEFAULT_HEIGHT)) );

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


bool GFXEngine::SetResVariables(Common::Point res)
{
    _resolution = res;   //stak.Get<int32_t>(ATT_WIDTH, 0);
    
    _clip = _resolution - Common::Point(1, 1);

    _field_54c = _resolution.x / 2;
    _field_550 = _resolution.y / 2;

    _field_554 = _resolution.x / 2;
    _field_558 = _resolution.y / 2;
    
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

    ApplyResolution();

    return 1;
}

void GFXEngine::ApplyResolution()
{        
    if ( (float)_resolution.x / (float)_resolution.y >= 1.4 )
    {
        int half = (_resolution.x + _resolution.y) / 2;
        _corrW = (float)half * 1.1429 / (float)_resolution.x;
        _corrH = (float)half * 0.85715 / (float)_resolution.y;
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
//    static const std::array<int, 4> blends = {GL_ZERO, GL_ONE, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    GfxStates *newStates;
    
    if (setAll < 2)
        newStates = &_states;
    else
        newStates = &_lastStates;
    
    bool forceSetShader = false;
    if (setAll)
        forceSetShader = true;
    
    if (_glext)
    {
        if (setAll || (newStates->Prog.ID != _lastStates.Prog.ID))
        {
            if (_vbo && _lastStates.Prog.ID)
            {
                if (_lastStates.Prog.PosLoc != -1)
                    Glext::GLDisableVertexAttribArray(_lastStates.Prog.PosLoc);
                
                if (_lastStates.Prog.ColorLoc != -1)
                    Glext::GLDisableVertexAttribArray(_lastStates.Prog.ColorLoc);
                
                if (_lastStates.Prog.UVLoc != -1)
                    Glext::GLDisableVertexAttribArray(_lastStates.Prog.UVLoc);
            }
            
            Glext::GLUseProgram(newStates->Prog.ID);
            
            forceSetShader = true;
            
            if (_vbo && newStates->Prog.ID)
            {
                if (newStates->Prog.PosLoc != -1)
                    Glext::GLEnableVertexAttribArray(newStates->Prog.PosLoc);
                
                if (newStates->Prog.ColorLoc != -1)
                    Glext::GLEnableVertexAttribArray(newStates->Prog.ColorLoc);
            }
        }
    }
    
    if (_vbo)
    {
        int32_t curBind = -1;
        glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &curBind);
        if (curBind != _vboParams)
            Glext::GLBindBuffer(GL_UNIFORM_BUFFER, _vboParams);
        
        if (setAll || (newStates->DataBuf != _lastStates.DataBuf))
        {
            Glext::GLBindBuffer(GL_ARRAY_BUFFER, newStates->DataBuf);
        }
        
        if (setAll || (newStates->IndexBuf != _lastStates.IndexBuf))
        {
            Glext::GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newStates->IndexBuf);
        }
        
        if (forceSetShader || (newStates->Tex != _lastStates.Tex))
        {
            if (newStates->Tex)
            {
                if (newStates->Prog.UVLoc != -1)
                    Glext::GLEnableVertexAttribArray(newStates->Prog.UVLoc);
                
                glBindTexture(GL_TEXTURE_2D, newStates->Tex);

                int32_t tmp = 1;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboTextured, 4, &tmp);
            }
            else
            {
                if (newStates->Prog.UVLoc != -1)
                    Glext::GLDisableVertexAttribArray(newStates->Prog.UVLoc);
                
                glBindTexture(GL_TEXTURE_2D, 0);

                int32_t tmp = 0;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboTextured, 4, &tmp);
            }
        }
        
        if ((forceSetShader || (newStates->Fog != _lastStates.Fog) ||
            (newStates->FogLength != _lastStates.FogLength) ||
            (newStates->FogStart != _lastStates.FogStart)) )
        {
            if (newStates->Fog)
            {
                float tmp[3] = { 1.0, newStates->FogStart, newStates->FogLength };
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboFogOff, 4 * 3, &tmp);
            }
            else
            {
                float tmp[3] = {0.0, 0.0, 0.0};
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboFogOff, 4 * 3, &tmp);
            }
        }
        
        if ((forceSetShader || (newStates->AFog != _lastStates.AFog) ||
            (newStates->AFogLength != _lastStates.AFogLength) ||
            (newStates->AFogStart != _lastStates.AFogStart)) )
        {
            if (newStates->AFog)
            {
                float tmp[3] = { 1.0, newStates->AFogStart, newStates->AFogLength };
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboAFogOff, 4 * 3, &tmp);
            }
            else
            {
                float tmp[3] = {0.0, 0.0, 0.0};
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboAFogOff, 4 * 3, &tmp);
            }
        }
        
        if (forceSetShader || (newStates->Shaded != _lastStates.Shaded))
        {
            if (newStates->Shaded)
            {
                int32_t tmp = 0;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboFlat, 4, &tmp);
            }
            else
            {
                int32_t tmp = 1;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboFlat, 4, &tmp);
            }
        }
        
        if (setAll || (newStates->AlphaTest != _lastStates.AlphaTest))
        {
            if (newStates->AlphaTest == false)
            {
                int32_t tmp = 0;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboATest, 4, &tmp);
            }
            else
            {
                int32_t tmp = 1;
                Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboATest, 4, &tmp);
            }
        }
    }
    else
    {
        if (setAll || (newStates->Stipple != _lastStates.Stipple))
        {
        }

        if (setAll || (newStates->Fog != _lastStates.Fog) ||
            (newStates->FogLength != _lastStates.FogLength) ||
            (newStates->FogStart != _lastStates.FogStart) )
        {
            if (newStates->Fog)
            {
                glEnable(GL_FOG);
                glFogf(GL_FOG_DENSITY, 1.0);
                glFogi(GL_FOG_MODE, GL_LINEAR);
                glHint(GL_FOG_HINT, GL_DONT_CARE);

                float fcolors[4] = {0.0, 0.0, 0.0, 1.0};

                glFogfv(GL_FOG_COLOR, fcolors);
                
                glFogf(GL_FOG_START, newStates->FogStart);
                glFogf(GL_FOG_END, newStates->FogStart + newStates->FogLength);
            }
            else
            {
                glDisable(GL_FOG);
            }
        }
        
        if (setAll || (newStates->Shaded != _lastStates.Shaded))
        {
            if (newStates->Shaded)
                glShadeModel(GL_SMOOTH);
            else
                glShadeModel(GL_FLAT);
        }
        
        if (setAll || (newStates->Tex != _lastStates.Tex))
        {
            if (newStates->Tex)
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, newStates->Tex);
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            }
            else
            {
                glDisable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            }
        }
        
        if (setAll || (newStates->AlphaTest != _lastStates.AlphaTest))
        {
            if (newStates->AlphaTest == false)
            {
                glDisable(GL_ALPHA_TEST);
            }
            else
            {
                glEnable(GL_ALPHA_TEST);
                glAlphaFunc(GL_GREATER, 0.0);
            }
        }
    }
    
    if (setAll || (newStates->DepthTest != _lastStates.DepthTest))
    {
        if (newStates->DepthTest)
        {
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    if (setAll || (newStates->Tex != _lastStates.Tex)
               || (newStates->LinearFilter != _lastStates.LinearFilter) )
    {
        if (newStates->Tex)
        {
            if (newStates->LinearFilter)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            }
            else
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            }
        }
    }
    
    if (setAll || (newStates->SrcBlend != _lastStates.SrcBlend)
                   || (newStates->DstBlend != _lastStates.DstBlend))
    {
        glBlendFunc(newStates->SrcBlend, newStates->DstBlend);
    }

    if (setAll || (newStates->TexBlend != _lastStates.TexBlend))
    {
        if (newStates->TexBlend == 0)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else if (newStates->TexBlend == 1)
        {
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
            glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
        }
        else if (newStates->TexBlend == 2)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    if (setAll || (newStates->AlphaBlend != _lastStates.AlphaBlend))
    {
        if (newStates->AlphaBlend)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
    }

    if (setAll || (newStates->Zwrite != _lastStates.Zwrite))
    {
        if (newStates->Zwrite)
            glDepthMask(GL_TRUE);
        else
            glDepthMask(GL_FALSE);
    }

    if (setAll < 2)
        _lastStates = _states;
}

void GFXEngine::RenderingMeshOld(TRenderNode *nod)
{
    if ( !_sceneBeginned )
        return;
    
    if (!nod)
        return;
    
    TMesh *mesh = nod->Mesh;
    
    if (!mesh)
        return;
    
    uint32_t flags = nod->Flags;

    _states.Shaded = false;
    _states.Stipple = false;
    _states.SrcBlend = GL_ONE;
    _states.DstBlend = GL_ZERO;
    _states.TexBlend = 0; //REPLACE
    _states.AlphaBlend = false;
    _states.Zwrite = true;
    _states.Tex = 0;
    _states.LinearFilter = (_filter != 0);
    _states.Fog = false;
    _states.AlphaTest = false;
    _states.AFog = false;
    _states.Prog = TShaderProg();
    
    bool useComputedColor = false;

    if ( flags & RFLAGS_TEXTURED )
    {
        if (nod->Tex)
            _states.Tex = nod->Tex->hwTex;
    }

    if ( flags & RFLAGS_SHADED )
    {
        _states.TexBlend = 2; //MODULATE
        _states.Shaded = true;
    }
    
    if ( flags & RFLAGS_FOG )
    {
        _states.Fog = true;
        _states.FogStart = nod->FogStart;
        _states.FogLength = nod->FogLength;
    }
    
    if ( flags & RFLAGS_LUMTRACY )
    {
        if ( !_zbuf_when_tracy )
            _states.Zwrite = false;

        if ( can_destblend )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_ONE;
            _states.DstBlend = GL_ONE;
            _states.Shaded = false;
        }
        else if ( can_srcblend )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_SRC_ALPHA;
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
            _states.Shaded = false;
        }
        else if ( can_stippling )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_SRC_ALPHA;//D3DBLEND_SRCALPHA;, 
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;//D3DBLEND_INVSRCALPHA;
            _states.Stipple = true;
            _states.Shaded = false;
        }
    }
    else if ( flags & RFLAGS_ZEROTRACY )
    {
        _states.AlphaTest = true;        

        if ( _pixfmt->BytesPerPixel != 1 )
        {
            _states.SrcBlend = GL_SRC_ALPHA;//D3DBLEND_SRCALPHA;, 
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;//D3DBLEND_INVSRCALPHA;
        }

        _states.AlphaBlend = true;
        _states.LinearFilter = false;
        _states.TexBlend = 2; //MODULATE
    }

    if (flags & RFLAGS_SKY)
    {
        _states.Fog = false;
        _states.Shaded = true;
    }
    else if (flags & RFLAGS_FALLOFF)
    {
        float transDist = System::IniConf::GfxSkyDistance.Get<int>();
        float transLen = System::IniConf::GfxSkyLength.Get<int>();

        for(TVertex &v : mesh->Vertexes)
        {
            v.ComputedColor = v.Color;

            float distance = nod->TForm.Transform( v.Pos ).XZ().length();

            if (distance > transDist)
            {
                float prc = (distance - transDist) / transLen;
                if (prc > 1.0)
                    prc = 1.0;
                if (prc < 0.0)
                    prc = 0.0;

                prc = (1.0 - prc);

                v.ComputedColor.a *= prc;

                useComputedColor = true;
            }
        }

        if (useComputedColor)
        {
            _states.Zwrite = true;
            _states.AlphaBlend = true;
            _states.TexBlend = 2; // MODULATE
            _states.SrcBlend = GL_SRC_ALPHA;

            if (flags & RFLAGS_LUMTRACY)
                _states.DstBlend = GL_ONE;
            else
                _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;

            _states.Shaded = true;
            _states.Stipple = false;
        }
    }
    
    if (flags & RFLAGS_COMPUTED_COLOR)
        useComputedColor = true;
    
    if (flags & RFLAGS_DISABLE_ZWRITE)
        _states.Zwrite = false;

    SetRenderStates(0);

    SetModelViewMatrix( nod->TForm );
  
    glVertexPointer(3, GL_FLOAT, sizeof(TVertex), &mesh->Vertexes[0].Pos);
    
    if (useComputedColor)
        glColorPointer(4, GL_FLOAT, sizeof(TVertex), &mesh->Vertexes[0].ComputedColor);
    else
        glColorPointer(4, GL_FLOAT, sizeof(TVertex), &mesh->Vertexes[0].Color);
    
    if (flags & RFLAGS_TEXTURED)
    {
        if ( (flags & RFLAGS_DYNAMIC_TEXTURE) && nod->coordsID >= 0 )
            glTexCoordPointer(2, GL_FLOAT, sizeof(tUtV), nod->Mesh->CoordsCache.at( nod->coordsID ).Coords.data());
        else
            glTexCoordPointer(2, GL_FLOAT, sizeof(TVertex), &mesh->Vertexes[0].TexCoord);
    }
    
    glDrawElements(GL_TRIANGLES, mesh->Indixes.size(), GLINDEXTYPE, mesh->Indixes.data());
}

void GFXEngine::RenderingMesh(TRenderNode *nod)
{
    if ( !_sceneBeginned )
        return;
    
    if (!nod)
        return;
    
    TMesh *mesh = nod->Mesh;
    
    if (!mesh)
        return;
    
    uint32_t flags = nod->Flags;

    _states.Shaded = false;
    _states.Stipple = false;
    _states.SrcBlend = GL_ONE;
    _states.DstBlend = GL_ZERO;
    _states.TexBlend = 0; //REPLACE
    _states.AlphaBlend = false;
    _states.Zwrite = true;
    _states.Tex = 0;
    _states.LinearFilter = (_filter != 0);
    _states.Fog = false;
    _states.AlphaTest = false;
    _states.AFog = false;
    _states.Prog = _stdShaderProg;
    
    if ( flags & RFLAGS_TEXTURED )
    {
        if (nod->Tex)
            _states.Tex = nod->Tex->hwTex;
    }

    if ( flags & RFLAGS_SHADED )
    {
        _states.TexBlend = 2; //MODULATE
        _states.Shaded = true;
    }
    
    if ( flags & RFLAGS_FOG )
    {
        _states.Fog = true;
        _states.FogStart = nod->FogStart;
        _states.FogLength = nod->FogLength;
    }
    
    if ( flags & RFLAGS_LUMTRACY )
    {
        if ( !_zbuf_when_tracy )
            _states.Zwrite = false;

        if ( can_destblend )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_ONE;
            _states.DstBlend = GL_ONE;
            _states.Shaded = false;
        }
        else if ( can_srcblend )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_SRC_ALPHA;
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
            _states.Shaded = false;
        }
        else if ( can_stippling )
        {
            _states.AlphaBlend = true;
            _states.TexBlend = 1; //MODULATEALPHA;
            _states.SrcBlend = GL_SRC_ALPHA;//D3DBLEND_SRCALPHA;, 
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;//D3DBLEND_INVSRCALPHA;
            _states.Stipple = true;
            _states.Shaded = false;
        }
    }
    else if ( flags & RFLAGS_ZEROTRACY )
    {
        _states.AlphaTest = true;        

        if ( _pixfmt->BytesPerPixel != 1 )
        {
            _states.SrcBlend = GL_SRC_ALPHA;//D3DBLEND_SRCALPHA;, 
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;//D3DBLEND_INVSRCALPHA;
        }

        _states.AlphaBlend = true;
        _states.LinearFilter = false;
        _states.TexBlend = 2; //MODULATE
    }

    if (flags & RFLAGS_SKY)
    {
        _states.Fog = false;
        _states.Shaded = true;
    }
    else if (flags & RFLAGS_FALLOFF)
    {
        _states.Zwrite = true;
        _states.AlphaBlend = true;
        _states.TexBlend = 2; // MODULATE
        _states.SrcBlend = GL_SRC_ALPHA;

        if (flags & RFLAGS_LUMTRACY)
            _states.DstBlend = GL_ONE;
        else
            _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;

        _states.Shaded = true;
        _states.Stipple = false;
        
        _states.AFog = true; 
        _states.AFogStart = System::IniConf::GfxSkyDistance.Get<int>();
        _states.AFogLength = System::IniConf::GfxSkyLength.Get<int>();
    }
    
    if (flags & RFLAGS_DISABLE_ZWRITE)
        _states.Zwrite = false;

    _states.DataBuf = nod->Mesh->glDataBuf;
    _states.IndexBuf = nod->Mesh->glIndexBuf;
    
    SetRenderStates(0);
    
    SetModelViewMatrix(nod->TForm);

    Glext::GLVertexAttribPointer(_lastStates.Prog.PosLoc, 3, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, Pos));
    
    if (flags & RFLAGS_COMPUTED_COLOR)
        Glext::GLVertexAttribPointer(_lastStates.Prog.ColorLoc, 4, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, ComputedColor));
    else
        Glext::GLVertexAttribPointer(_lastStates.Prog.ColorLoc, 4, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, Color));
    
    if (flags & RFLAGS_TEXTURED)
    {
        if ( (flags & RFLAGS_DYNAMIC_TEXTURE) && nod->coordsID >= 0 )
            Glext::GLVertexAttribPointer(_lastStates.Prog.UVLoc, 2, GL_FLOAT, GL_FALSE,  sizeof(tUtV), (void *)nod->Mesh->CoordsCache.at( nod->coordsID ).BufferPos);
        else
            Glext::GLVertexAttribPointer(_lastStates.Prog.UVLoc, 2, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, TexCoord));
    }
    
    glDrawElements(GL_TRIANGLES, mesh->Indixes.size(), GLINDEXTYPE, NULL);
}

void GFXEngine::RenderNode(TRenderNode *node)
{
    if (!node)
        return;
    
    switch(node->Type)
    {
        case TRenderNode::TYPE_MESH:
            if (_vbo)
                RenderingMesh(node);
            else
                RenderingMeshOld(node);
            break;
            
        case TRenderNode::TYPE_PARTICLE:
        {
            PrepareParticle(node);
            if (_vbo)
                RenderingMesh(node);
            else
                RenderingMeshOld(node);
        }
            break;
            
        default:
            break;
    }
}

void GFXEngine::PrepareParticle(TRenderNode *node)
{
    if (!node->Mesh)
        return;
    
    vec3d pos = node->TForm.getTranslate();
    
    node->TForm = mat4x4( mat3x3(node->ParticleSize, 0, 0,
                                 0, node->ParticleSize, 0,
                                 0, 0, 1.0) );
    node->TForm.setTranslate( pos );
}

void GFXEngine::QueueRenderMesh(TRenderNode *nod)
{
    if (!nod)
        return;
    
    TMesh *mesh = nod->Mesh;    
    if (!mesh)
        return;
    
    uint32_t flags = nod->Flags;
    
    if (flags & RFLAGS_SKY)
        _renderSkyBoxList.push_back(nod);
    else if (flags & RFLAGS_ZEROTRACY)
        _renderZeroTracyList.push_back(nod);
    else if (flags & RFLAGS_LUMTRACY)
        _renderLumaTracyList.push_back(nod);
    else
        _renderSolidList.push_back(nod);
}

void GFXEngine::Rasterize(uint32_t RasterEtapes)
{
    if (RasterEtapes & RASTER_SKY)
    {
        _renderSkyBoxList.sort(TRenderNode::CompareSolid);
        
        while(!_renderSkyBoxList.empty())
        {
            RenderNode( _renderSkyBoxList.front() );
            _renderSkyBoxList.pop_front();
        }
    }
    
    if (RasterEtapes & RASTER_SOLID)
    {
        _renderSolidList.sort(TRenderNode::CompareSolid);
        
        while(!_renderSolidList.empty())
        {
            RenderNode( _renderSolidList.front() );
            _renderSolidList.pop_front();
        }
    }
    
    if (RasterEtapes & RASTER_ZEROTR)
    {
        _renderZeroTracyList.sort(TRenderNode::CompareSolid);
        
        while(!_renderZeroTracyList.empty())
        {
            RenderNode( _renderZeroTracyList.front() );
            _renderZeroTracyList.pop_front();
        }
    }
    
    if (RasterEtapes & RASTER_LUMATR)
    {
        _renderLumaTracyList.sort(TRenderNode::CompareTransparent);
        
        while(!_renderLumaTracyList.empty())
        {
            RenderNode( _renderLumaTracyList.front() );
            _renderLumaTracyList.pop_front();
        }
    }
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

    int halfWidth = _resolution.x / 2;
    int halfHeight = _resolution.y / 2;

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
                    y_out += _resolution.y;

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
    SetRenderStates(2);
    
    SetProjectionMatrix( _frustum );
    SetModelViewMatrix( mat4x4f() );
    
    _sceneBeginned = 1;
}

// Draw transparent
void GFXEngine::EndScene()
{
    _sceneBeginned = 0;
    
    _renderSkyBoxList.clear();
    _renderSolidList.clear();
    _renderZeroTracyList.clear();
    _renderLumaTracyList.clear();
    
    _renderNodesCache.Rewind();
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
    
    bool saved = _states.Zwrite;
    
    _states.Zwrite = true;
    SetRenderStates(0);
    
    if (_colorEffects)
    {
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);
    
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);
    }

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    _states.Zwrite = saved;
    SetRenderStates(0);
    
    if (!_vbo)
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
    }
    
    _states.Prog = _stdShaderProg;
}

void GFXEngine::EndFrame()
{       
    if (_colorEffects > 1)
    {
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);   
        DrawFBO();
    }    

    Gui::Root::Instance.Draw(Screen());
    DrawScreenSurface();
    Gui::Root::Instance.HwCompose();
    
    if (_colorEffects == 1)
    {
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);   
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
        glGenTextures(1, &bitm->hwTex);

        if (!bitm->hwTex)
        {
            return false;
        }

        _states.Tex = bitm->hwTex;
        SetRenderStates(0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (bitm->swTex->format->format == _pixfmt->format)
        {
            SDL_LockSurface(bitm->swTex);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitm->width, bitm->height, 0, _glPixfmt, _glPixtype, bitm->swTex->pixels);
            SDL_UnlockSurface(bitm->swTex);
        }
        else
        {
            SDL_Surface *conv = ConvertSDLSurface(bitm->swTex, _pixfmt);
            
            SDL_LockSurface(conv);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitm->width, bitm->height, 0, _glPixfmt, _glPixtype, conv->pixels);
            SDL_UnlockSurface(conv);
            
            SDL_FreeSurface(conv);
        }
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
    FSMgr::FileHandle *fil = uaOpenFileAlloc(fmt::sprintf("%s.ppm", screenName), "wb");
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
    return _resolution.x;
}

int16_t GFXEngine::GetHeight()
{
    return _resolution.y;
}


void GFXEngine::draw2DandFlush()
{
    if (_colorEffects)
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    Gui::Root::Instance.Draw(Screen());
    DrawScreenSurface();
    Gui::Root::Instance.HwCompose();

    SDL_FillRect(Screen(), NULL, SDL_MapRGBA(Screen()->format, 0, 0, 0, 0) );
    
    if (_colorEffects)
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);
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
    
    _frustum = mat4x4f::UAFrustum(_near, _far);
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

GfxMode::GfxMode(GfxMode &&g)
{
    w = std::move(g.w);
    h = std::move(g.h);
    bpp = std::move(g.bpp);
    mode = std::move(g.mode);
    name = std::move(g.name);
    windowed = std::move(g.windowed);
}

GfxMode::GfxMode(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    name = g.name;
    windowed = g.windowed;
}

GfxMode::GfxMode(const Common::Point &sz)
: w(sz.x), h(sz.y)
{
    name = GenName(w, h);
}

GfxMode& GfxMode::operator=(const GfxMode &g)
{
    w = g.w;
    h = g.h;
    bpp = g.bpp;
    mode = g.mode;
    name = g.name;
    windowed = g.windowed;
    return *this;
}

GfxMode::operator bool() const
{
    if (w == 0 || h == 0)
        return false;
    return true;
}

bool GfxMode::operator==(const GfxMode &g) const
{
    return (w == g.w && h == g.h);
}

bool GfxMode::operator==(const Common::Point &g) const
{
    return (w == g.x && h == g.y);
}

bool GfxMode::operator!=(const GfxMode &g) const
{
    return w != g.w || h != g.h;
}

bool GfxMode::operator!=(const Common::Point &g) const
{
    return w != g.x || h != g.y;
}

bool GfxMode::SortCompare(const GfxMode &a, const GfxMode &b)
{
    if (a.w > b.w)
        return true;
    else if (a.w == b.w && a.h > b.h)
        return true;
    return false;
}

std::string GfxMode::GenName(int w, int h)
{
    return fmt::sprintf("%d x %d", w, h);
}




void GFXEngine::AddGfxMode(const GfxMode &md)
{
    for (const GfxMode &m : graphicsModes)
    {
        if ( m.w == md.w && m.h == md.h )
            return;
    }

    graphicsModes.push_back(md);
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
    FSMgr::FileHandle *fil = uaOpenFileAlloc( fmt::sprintf("res/%s.cur", name) , "rb");

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
    _glext = Glext::init();
    
    System::EventsAddHandler(EventsWatcher);
    
    System::IniConf::ReadFromNucleusIni();
    
    _vbo = System::IniConf::GfxVBO.Get<bool>();
    _colorEffects = System::IniConf::GfxColorEffects.Get<int32_t>();
    
    if (!_glext)
    {
        _colorEffects = 0;
        _vbo = false;
    }
    
    if (_vbo)
    {
        Glext::GLGenBuffers(1, &_vboParams);
        Glext::GLBindBuffer(GL_UNIFORM_BUFFER, _vboParams);
        Glext::GLBufferData(GL_UNIFORM_BUFFER, _vboParamsSize+64, NULL, GL_DYNAMIC_DRAW); 
        
        Glext::GLBindBufferBase(GL_UNIFORM_BUFFER, _vboParamsBlockBinding, _vboParams);
    }
    
    SDL_DisplayMode deskMode;
    SDL_GetDesktopDisplayMode(0, &deskMode);

    std::array<Common::Point, 17> checkModes
    {{
        {640, 480},     {800, 600},     {1024, 768},    {1280, 1024}, 
        {1440, 1050},   {1600, 1200},   {720, 480},     {852, 480}, 
        {1280, 720},    {1366, 768},    {1600, 900},    {1920, 1080}, 
        {1920, 1200},   {2560, 1080},   {2560, 1440},   {3440, 1440},
        {3840, 2160}
     }};
    
    graphicsModes.reserve(checkModes.size());

    uint32_t corrected = CorrectSurfaceFormat(deskMode.format);

    for(Common::Point m : checkModes)
    {
        SDL_DisplayMode target, closest;

        target.w = m.x;
        target.h = m.y;
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
            mode.name = GfxMode::GenName(mode.w, mode.h);

            AddGfxMode(mode);
        }
    }
    
    // Force to add custom resolutions
    std::vector<std::string> customModes = Stok::Split(System::IniConf::GfxAdditionalModes.Get<std::string>(), ",");
    for (std::string mod : customModes)
    {
        std::vector<std::string> vals = Stok::Split(mod, ":-x \t");
        if (vals.size() >= 2)
        {
            SDL_DisplayMode target, closest;
            
            target.w = std::stoi(vals[0]);
            target.h = std::stoi(vals[1]);
            target.format = corrected;
            target.refresh_rate = 0;
            target.driverdata   = 0;

            GfxMode mode;
            mode.w = target.w;
            mode.h = target.h;
            mode.mode = target;
            mode.bpp = SDL_BYTESPERPIXEL(corrected) * 8;
            mode.name = GfxMode::GenName(mode.w, mode.h);
                
            if (SDL_GetClosestDisplayMode(0, &target, &closest) )
            {
                mode.mode = closest;
                mode.bpp = SDL_BYTESPERPIXEL(closest.format) * 8;
                mode.name = GfxMode::GenName(mode.w, mode.h);
            }
            
            AddGfxMode(mode);
        }
    }
    
    

    std::sort(graphicsModes.begin(), graphicsModes.end(), GfxMode::SortCompare);
    
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

    RecreateScreenSurface();
    Gui::Instance.SetScreenSize(GetSize());
    
    LoadPalette(System::IniConf::GfxPalette.Get<std::string>());

    if (_vbo)
    {
        _stdPsShader = CompileShader(GL_FRAGMENT_SHADER, _stdPShaderText);
        _stdVsShader = CompileShader(GL_VERTEX_SHADER,   _stdVShaderText);
        uint32_t progID = Glext::GLCreateProgram();
        
        Glext::GLAttachShader(progID, _stdPsShader);
        Glext::GLAttachShader(progID, _stdVsShader);
        Glext::GLLinkProgram(progID);
        
        _stdShaderProg = TShaderProg( progID );
        
        BindVBOParameters(_stdShaderProg);
    }

    if (_colorEffects > 0)
    {
        Glext::GLGenFramebuffers(1, &_fbo);
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, _fbo);

        glGenTextures(1, &_fboTex);
        glBindTexture(GL_TEXTURE_2D, _fboTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, FBOTEXTYPE, 640, 480, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);


        Glext::GLGenRenderbuffers(1, &_fbod);
        Glext::GLBindRenderbuffer(GL_RENDERBUFFER, _fbod);
        Glext::GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 640, 480);

        
        Glext::GLFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _fbod);

        Glext::GLFrameBufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _fboTex, 0);

        Glext::GLBindRenderbuffer(GL_RENDERBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
        Glext::GLBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (_vbo)
        {
            _psShader = LoadShader(GL_FRAGMENT_SHADER, "res/clreff_vbo.ps");
            _vsShader = LoadShader(GL_VERTEX_SHADER, "res/clreff_vbo.vs");
        }
        else
        {
            _psShader = LoadShader(GL_FRAGMENT_SHADER, "res/clreff.ps");
            _vsShader = LoadShader(GL_VERTEX_SHADER, "res/clreff.vs");
        }

        uint32_t progID = Glext::GLCreateProgram();
        
        Glext::GLAttachShader(progID, _psShader);
        Glext::GLAttachShader(progID, _vsShader);
        Glext::GLLinkProgram(progID);
        
        _colorEffectsShaderProg = TColorEffectsProg(progID);
        
        if (_vbo)
            BindVBOParameters(_colorEffectsShaderProg);
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
    
    ScreenSurface = NULL;
    
    if (_stdQuadDataBuf)
        Glext::GLDeleteBuffers(1, &_stdQuadDataBuf);
    
    _stdQuadDataBuf = 0;
    
    if (_stdQuadIndexBuf)
        Glext::GLDeleteBuffers(1, &_stdQuadIndexBuf);
    
    _stdQuadIndexBuf = 0;
    
    if (_stdShaderProg.ID)
        Glext::GLDeleteProgram(_stdShaderProg.ID);
    
    if (_stdVsShader)
        Glext::GLDeleteShader(_stdVsShader);
    
    if (_stdPsShader)
        Glext::GLDeleteShader(_stdPsShader);
    
    _stdShaderProg.ID = 0;
    _stdVsShader = 0;
    _stdPsShader = 0;   
    
    if (_colorEffectsShaderProg.ID)
        Glext::GLDeleteProgram(_colorEffectsShaderProg.ID); 
    
    if (_psShader)
        Glext::GLDeleteShader(_psShader);
    
    if (_vsShader)
        Glext::GLDeleteShader(_vsShader);
    
    _colorEffectsShaderProg.ID = 0;
    _vsShader = 0;
    _psShader = 0;
}

GFXEngine::~GFXEngine()
{
    Deinit();
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
            Instance.UpdateFBOSizes();
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
    FSMgr::FileHandle *fil = uaOpenFileAlloc(file, "r");

    if ( fil )
    {
        std::string line;
        if ( fil->ReadLine(&line) )
        {
            size_t pos = line.find_first_of("\n\r");
            
            if (pos != std::string::npos)
                line.erase(pos);
            
            bool windowed = false;
            pos = line.find("Windowed");
            
            if (pos != std::string::npos && pos >= 1)
            {
                windowed = true;
                line.erase(pos - 1);
            }           

            for (const GfxMode &m : graphicsModes)
            {
                if ( StriCmp(m.name, line) == 0 )
                {
                    GfxMode tmp = m;
                    tmp.windowed = windowed;
                    return tmp;
                }
            }
        }
        delete fil;
    }

    return sub_41F68C();
}

int GFXEngine::GetGfxModeIndex(const Common::Point &res)
{
    int i = 0;
    for (const GfxMode &m : graphicsModes)
    {
        if (m == res)
            return i;
        ++i;
    }
    return -1;
}

void GFXEngine::SetResolution(const Common::Point &res, bool windowed)
{
    if (GfxSelectedMode == res && GfxSelectedMode.windowed == windowed)
        return;
    
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
            if ( m.w == res.x && m.h == res.y )
            {
                picked = m;
                picked.windowed = windowed;
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
    
    SetResVariables(picked);        
    ApplyResolution();
    
    RecreateScreenSurface();
    
    BeginFrame();
    SetPalette(palette_copy);
    
    GfxSelectedMode = picked;
    
    FSMgr::FileHandle *fil = uaOpenFileAlloc("env/vid.def", "w");
    if ( fil )
    {
        if (picked.windowed)
            fil->printf("%s Windowed\n", picked.name.c_str());
        else
            fil->printf("%s\n", picked.name.c_str());
            
        delete fil;
    }
    
    Gui::Instance.SetScreenSize(GetSize());
    
    UpdateFBOSizes();
}

const std::vector<GfxMode> &GFXEngine::GetAvailableModes()
{
    return graphicsModes;
}




void GFXEngine::SetTracyRmp(ResBitmap *rmp)
{
    setRSTR_trcRmp(rmp);
}

void GFXEngine::SetShadeRmp(ResBitmap *rmp)
{
    setRSTR_shdRmp(rmp);
}

GfxMode GFXEngine::GetGfxMode()
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



SDL_Surface *GFXEngine::Screen()
{
    return ScreenSurface;
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
#if SDL_VERSION_ATLEAST(2, 0, 9)
        if (SDL_HasColorKey(src))
#else
        uint32_t tmp;
        if (SDL_GetColorKey(src, &tmp) == 0)
#endif
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
    
    ScreenSurface = SDL_CreateRGBSurface(0, _resolution.x, _resolution.y, bpp, rm, gm, bm, am);

    if ( !screenTex )
        glGenTextures(1, &screenTex);

    _states.Tex = screenTex;
    SetRenderStates(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ScreenSurface->w, ScreenSurface->h, 0, pixfmt, pixtype, NULL);
}

void GFXEngine::DrawVtxQuad(const std::array<GFX::TVertex, 4> &vtx)
{
    static const uint16_t indexes[6] = {0, 1, 2, 0, 2, 3};
    
    if (_vbo)
    {
        if (!_stdQuadDataBuf)
        {
            Glext::GLGenBuffers(1, &_stdQuadDataBuf);
            Glext::GLBindBuffer(GL_ARRAY_BUFFER, _stdQuadDataBuf);
            Glext::GLBufferData(GL_ARRAY_BUFFER, sizeof(TVertex) * vtx.size(), NULL, GL_DYNAMIC_DRAW);
        }
        
        if (!_stdQuadIndexBuf)
        {
            Glext::GLGenBuffers(1, &_stdQuadIndexBuf);
            Glext::GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _stdQuadIndexBuf);
            Glext::GLBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), &indexes, GL_STATIC_DRAW);
        }
        
        _states.DataBuf = _stdQuadDataBuf;
        _states.IndexBuf = _stdQuadIndexBuf;
        
        SetRenderStates(0);
        
        Glext::GLBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(TVertex) * vtx.size(), vtx.data()); 
        
        if (_lastStates.Prog.PosLoc != -1)
            Glext::GLVertexAttribPointer(_lastStates.Prog.PosLoc, 3, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, Pos));  
        if (_lastStates.Prog.ColorLoc != -1)
            Glext::GLVertexAttribPointer(_lastStates.Prog.ColorLoc, 4, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, Color));
        if (_lastStates.Prog.UVLoc != -1)
            Glext::GLVertexAttribPointer(_lastStates.Prog.UVLoc, 2, GL_FLOAT, GL_FALSE,  sizeof(TVertex), (void *)offsetof(TVertex, TexCoord));
        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        
    }
    else
    {
        glVertexPointer(3, GL_FLOAT, sizeof(TVertex), &vtx[0].Pos);
        glColorPointer(4, GL_FLOAT, sizeof(TVertex), &vtx[0].Color);
        glTexCoordPointer(2, GL_FLOAT, sizeof(TVertex), &vtx[0].TexCoord);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indexes);
    }
}

void GFXEngine::DrawScreenSurface()
{
    GfxStates save = _states;
    
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);
    
    SetProjectionMatrix( mat4x4f() );
    SetModelViewMatrix( mat4x4f() );
    
    _states.DepthTest = false;
    _states.Zwrite = false;
    _states.AlphaBlend = true;
    _states.SrcBlend = GL_SRC_ALPHA;
    _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
    _states.Tex = screenTex;
    _states.TexBlend = 2;
    _states.Prog = _stdShaderProg;

    SetRenderStates(0);
    
    // Will be binded with SetRenderStates
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, ScreenSurface->w, ScreenSurface->h, pixfmt, pixtype, ScreenSurface->pixels);

    static std::array<TVertex, 4> vtx = {
        GFX::TVertex( vec3f(-1.0,  1.0, 0.0), tUtV(0.0, 0.0) ),
        GFX::TVertex( vec3f(-1.0, -1.0, 0.0), tUtV(0.0, 1.0) ),
        GFX::TVertex( vec3f( 1.0, -1.0, 0.0), tUtV(1.0, 1.0) ),
        GFX::TVertex( vec3f( 1.0,  1.0, 0.0), tUtV(1.0, 0.0) )
    };
    
    DrawVtxQuad(vtx);
    
    _states = save;
}

uint32_t GFXEngine::CompileShader(int32_t type, const std::string &string)
{
    GLuint sh = Glext::GLCreateShader(type);
    if (!sh)
        return 0;
    
    const GLchar *source = (const GLchar *)string.c_str();
    
    Glext::GLShaderSource(sh, 1, &source, 0);
    Glext::GLCompileShader(sh);
    
    GLint tmpvar;
    Glext::GLGetShaderiv(sh, GL_COMPILE_STATUS, &tmpvar);
    
    if (tmpvar == GL_FALSE)
    {
        Glext::GLGetShaderiv(sh, GL_INFO_LOG_LENGTH, &tmpvar);
        if (tmpvar > 0)
        {
            char *logbuff = new char[tmpvar + 2];
            Glext::GLGetShaderInfoLog(sh, tmpvar, NULL, logbuff);
            printf("Shader error: %s\n", logbuff);
            log_d3dlog("Shader error: %s\n", logbuff);
            delete[] logbuff;
        }
        Glext::GLDeleteShader(sh);
        return 0;
    }
    
    return sh;
}


uint32_t GFXEngine::LoadShader(int32_t type, const std::string &fl)
{
    FSMgr::FileHandle *f = FSMgr::iDir::openFileAlloc(fl, "rb");
    if (!f)
        return 0;

    f->seek(0, SEEK_END);
    size_t sz = f->tell();
    f->seek(0, SEEK_SET);

    char *tmp = new char[sz];
    f->read(tmp, sz);

    std::string b;
    b.assign(tmp, sz);

    delete[] tmp;

    delete f;
    
    return CompileShader(type, b);
}

void GFXEngine::DrawFBO()
{
    GfxStates save = _states;
    
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);
        
    SetProjectionMatrix( mat4x4f() );
    SetModelViewMatrix( mat4x4f() );
    
    _states.DepthTest = false;
    _states.Zwrite = false;
    _states.AlphaBlend = true;
    
    if (_fboBlend == 0)
    {
        _states.SrcBlend = GL_ONE;
        _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
    }
    else if (_fboBlend == 1)
    {
        _states.SrcBlend = GL_SRC_ALPHA;
        _states.DstBlend = GL_ONE_MINUS_SRC_ALPHA;
    }
    
    _states.Tex = _fboTex;
    _states.TexBlend = 2;
    _states.Prog = _colorEffectsShaderProg;

    // Apply texture and program
    SetRenderStates(0);
    
    if (_colorEffectsShaderProg.NormLoc >= 0)
        Glext::GLUniform3f(_colorEffectsShaderProg.NormLoc, _normClr.x, _normClr.y, _normClr.z);
    
    if (_colorEffectsShaderProg.InvLoc >= 0)
        Glext::GLUniform3f(_colorEffectsShaderProg.InvLoc, _invClr.x, _invClr.y, _invClr.z);
    
    if (_colorEffectsShaderProg.RandLoc >= 0)
        Glext::GLUniform1i(_colorEffectsShaderProg.RandLoc, rand());
    
    if (_colorEffectsShaderProg.ScrSizeLoc >= 0)
        Glext::GLUniform2i(_colorEffectsShaderProg.ScrSizeLoc, scrSz.x, scrSz.y);
    
    if (_colorEffectsShaderProg.MillisecsLoc >= 0)
        Glext::GLUniform1i(_colorEffectsShaderProg.MillisecsLoc, SDL_GetTicks());
    
    static std::array<TVertex, 4> vtx = {
        GFX::TVertex( vec3f(-1.0,  1.0, 0.0), tUtV(0.0, 1.0) ),
        GFX::TVertex( vec3f(-1.0, -1.0, 0.0), tUtV(0.0, 0.0) ),
        GFX::TVertex( vec3f( 1.0, -1.0, 0.0), tUtV(1.0, 0.0) ),
        GFX::TVertex( vec3f( 1.0,  1.0, 0.0), tUtV(1.0, 1.0) )
    };
    
    DrawVtxQuad(vtx);
    
    _states = save;
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

        _states.Tex = _fboTex;
        SetRenderStates(0);
        
        glTexImage2D(GL_TEXTURE_2D, 0, FBOTEXTYPE, scrSz.x, scrSz.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        Glext::GLBindRenderbuffer(GL_RENDERBUFFER, _fbod);
        Glext::GLRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, scrSz.x, scrSz.y);
        Glext::GLBindRenderbuffer(GL_RENDERBUFFER, 0);
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

Common::Point GFXEngine::ConvertPosTo2DStuff(const Common::Point &pos)
{
    Common::Point real = System::GetResolution();

    if (real == _resolution)
        return pos;
    
    Common::Point t( pos.x * _resolution.x / real.x,
                     pos.y * _resolution.y / real.y );

    if (t.x < 0)
        t.x = 0;
    if (t.x >= _resolution.x)
        t.x = _resolution.x - 1;

    if (t.y < 0)
        t.y = 0;
    if (t.y >= _resolution.y)
        t.y = _resolution.y - 1;

    return t;
}

TMesh::TMesh(const TMesh &b)
{
    Mat = b.Mat;
    Vertexes = b.Vertexes;
    Indixes = b.Indixes;
    CoordsCache = b.CoordsCache;
    BoundBox = b.BoundBox;
    
    glDataBuf = 0;
    glIndexBuf = 0;
}

TMesh &TMesh::operator=(const TMesh& b)
{
    Mat = b.Mat;
    Vertexes = b.Vertexes;
    Indixes = b.Indixes;
    CoordsCache = b.CoordsCache;
    BoundBox = b.BoundBox;
    
    glDataBuf = 0;
    glIndexBuf = 0;
    
    return *this;
}

TMesh::~TMesh()
{
    GFX::Engine.MeshFreeVBO(this);
    
    // Just notify about still existing buffers
    if (glDataBuf || glIndexBuf)
        printf("TMesh still has buffers!\n");
}

void GFXEngine::MeshMakeVBO(TMesh *mesh)
{
    if (_vbo)
    {        
        if (!mesh->glDataBuf)
            Glext::GLGenBuffers(1, &mesh->glDataBuf);
        
        if (!mesh->glIndexBuf)
            Glext::GLGenBuffers(1, &mesh->glIndexBuf);
                
        Glext::GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->glIndexBuf);
        Glext::GLBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->Indixes.size() * sizeof(IndexType), mesh->Indixes.data(), GL_STATIC_DRAW);
        
        Glext::GLBindBuffer(GL_ARRAY_BUFFER, mesh->glDataBuf);
        int32_t vtxDataSz = mesh->Vertexes.size() * sizeof(TVertex);
        int32_t coordDataSz = mesh->Vertexes.size() * sizeof(tUtV);
        Glext::GLBufferData(GL_ARRAY_BUFFER, vtxDataSz + mesh->CoordsCache.size() * coordDataSz, NULL, GL_STATIC_DRAW);
        
        int32_t off = 0;
        Glext::GLBufferSubData(GL_ARRAY_BUFFER, off, vtxDataSz, mesh->Vertexes.data());
        
        off += vtxDataSz;
        for (TCoordsCache &cch : mesh->CoordsCache)
        {
            Glext::GLBufferSubData(GL_ARRAY_BUFFER, off, coordDataSz, cch.Coords.data());
            cch.BufferPos = off;
            
            off += coordDataSz;
        }
        
        Glext::GLBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _lastStates.IndexBuf);
        Glext::GLBindBuffer(GL_ARRAY_BUFFER, _lastStates.DataBuf);  
    }
}

void GFXEngine::MeshFreeVBO(TMesh *mesh)
{
    if (_vbo)
    {
        if (mesh->glDataBuf)
        {
            Glext::GLDeleteBuffers(1, &mesh->glDataBuf);
            mesh->glDataBuf = 0;
        }
        
        if (mesh->glIndexBuf)
        {
            Glext::GLDeleteBuffers(1, &mesh->glIndexBuf);
            mesh->glIndexBuf = 0;
        }
        
//        if (mesh->glVao)
//        {
//            Glext::GLDeleteVertexArrays(1, &mesh->glVao);
//            mesh->glVao = 0;
//        }
    }
}

void GFXEngine::BindVBOParameters(TShaderProg &shader)
{
    if (_vbo)
    {
        uint32_t blockIndex = Glext::GLGetUniformBlockIndex(shader.ID, "Parameters");   
        if (blockIndex != GL_INVALID_INDEX)
            Glext::GLUniformBlockBinding(shader.ID, blockIndex, _vboParamsBlockBinding);
    }
}

void GFXEngine::SetProjectionMatrix(const mat4x4f &mat)
{
    mat4x4f tmp = mat.Transpose();
    if (_vbo)
    {
        Glext::GLBindBuffer(GL_UNIFORM_BUFFER, _vboParams);
        Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboMProjOff, 4 * 4 * 4, &tmp.m00);
    }
    else
    {
        glMatrixMode(GL_PROJECTION);
        glLoadMatrixf(&tmp.m00);
    }
}

void GFXEngine::SetModelViewMatrix(const mat4x4f &mat)
{
    mat4x4f tmp = mat.Transpose();
    if (_vbo)
    {
        Glext::GLBindBuffer(GL_UNIFORM_BUFFER, _vboParams);
        Glext::GLBufferSubData(GL_UNIFORM_BUFFER, _vboMViewOff, 4 * 4 * 4, &tmp.m00);
    }
    else
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadMatrixf(&tmp.m00);
    }
}

TShaderProg::TShaderProg(uint32_t id)
: ID(id)
{
    PosLoc = Glext::GLGetAttribLocation(ID, "vPos");
    UVLoc  = Glext::GLGetAttribLocation(ID, "vUV");
    ColorLoc = Glext::GLGetAttribLocation(ID, "vColor");
}

TColorEffectsProg::TColorEffectsProg(uint32_t id)
: TShaderProg(id)
{
    NormLoc = Glext::GLGetUniformLocation(ID, "normclr");
    InvLoc = Glext::GLGetUniformLocation(ID, "invclr");
    RandLoc = Glext::GLGetUniformLocation(ID, "randval");
    ScrSizeLoc = Glext::GLGetUniformLocation(ID, "screenSize");
    MillisecsLoc = Glext::GLGetUniformLocation(ID, "millisecs");
}

}