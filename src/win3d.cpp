#include <algorithm>

#include <inttypes.h>
#include <list>
#include "system/system.h"
#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "display.h"
#include "win3d.h"
#include "utils.h"

#include "font.h"

#include "gui/root.h"



const Nucleus::ClassDescr NC_STACK_win3d::description("win3d.class", &newinstance);

Common::Ini::KeyList NC_STACK_win3d::win3d_keys
{
    Common::Ini::Key("gfx.dither",       Common::Ini::KT_BOOL),               //0
    Common::Ini::Key("gfx.filter",       Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.antialias",    Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.alpha",        Common::Ini::KT_DIGIT, (int32_t)192),
    Common::Ini::Key("gfx.zbuf_when_tracy", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.colorkey",     Common::Ini::KT_BOOL),             //5
    Common::Ini::Key("gfx.force_emul",   Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.force_soft_cursor", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.all_modes",    Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.movie_player", Common::Ini::KT_BOOL, true),
    Common::Ini::Key("gfx.force_alpha_textures", Common::Ini::KT_BOOL), //10
    Common::Ini::Key("gfx.use_draw_primitive", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.disable_lowres", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.export_window_mode", Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.blending",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.solidfont",    Common::Ini::KT_BOOL),          //15
    Common::Ini::Key("gfx.vsync",        Common::Ini::KT_DIGIT, (int32_t)1),
    Common::Ini::Key("gfx.maxfps",       Common::Ini::KT_DIGIT, (int32_t)60),
    Common::Ini::Key("gfx.newsky",       Common::Ini::KT_BOOL),
    Common::Ini::Key("gfx.skydistance",  Common::Ini::KT_DIGIT, (int32_t)3000),
    Common::Ini::Key("gfx.skylength",    Common::Ini::KT_DIGIT, (int32_t)500)               //20
};


ScreenFont::ScreenFont()
{
    ttfFont = NULL;
    height = 0;
    r = 0;
    g = 0;
    b = 0;
}

NC_STACK_win3d::NC_STACK_win3d()
{
    _forcesoftcursor = 0;
    _movie_player = 0;
    _field_38 = 0;
    _txt16bit = 0;
    _use_simple_d3d = 0;
    _disable_lowres = 0;
    _export_window_mode = 0;
    _flags = 0;

    _pending.clear();

    for(int i = 0; i < 9; i++)
    {
        _greyColors[i][0] = 0.0;
        _greyColors[i][1] = 0.0;
        _greyColors[i][2] = 0.0;
    }

    _dither = 0;
    _filter = 0;
    _antialias = 0;
    _alpha = 0;
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


void NC_STACK_win3d::DrawTextEntry(const ScreenText *txt)
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

                SDL_SetClipRect(GFX::Engine.Screen(), &clipRect);


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

                SDL_BlitSurface(tmp, NULL, GFX::Engine.Screen(), &want);

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

                SDL_BlitSurface(tmp, NULL, GFX::Engine.Screen(), &want);
                SDL_FreeSurface(tmp);


                SDL_SetClipRect(GFX::Engine.Screen(), NULL);
            }
        }
    }
}

void NC_STACK_win3d::AddScreenText(const char *string, int p1, int p2, int p3, int p4, int flag)
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

void NC_STACK_win3d::DrawScreenText()
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

void NC_STACK_win3d::win3dInitialisation()
{
    _pending.clear();

    for (int i = 0; i < 8; i++)
    {
        uint32_t color = 0;
        switch(i)
        {
        case 0:
        case 4:
        case 5:
        case 6:
            color = 0xFFFFFF;
            break;
        case 1:
            color = 0xFF0000;
            break;
        case 2:
            color = 0x0000FF;
            break;
        case 3:
            color = 0x00FF80;
            break;
        case 7:
            color = 0x00FFC0;
            break;
        default:
            break;
        }

        _greyColors[i][0] = ((color >> 16) & 0xFF) / 255.0;
        _greyColors[i][1] = ((color >> 8) & 0xFF) / 255.0;
        _greyColors[i][2] = (color & 0xFF) / 255.0;
    }

    _greyColors[8][0] = 1.0;
    _greyColors[8][1] = 1.0;
    _greyColors[8][2] = 1.0;
}

void NC_STACK_win3d::initPixelFormats()
{
    if (_pixfmt)
        SDL_FreeFormat(_pixfmt);

    SDL_DisplayMode curr;
    SDL_GetCurrentDisplayMode(0, &curr);
    curr.format = GFX::Engine.CorrectSurfaceFormat(curr.format);

    _pixfmt = SDL_AllocFormat( curr.format );

    GFX::Engine.GLMapFormat(curr.format, &_glPixfmt, &_glPixtype);
}

void NC_STACK_win3d::initPolyEngine()
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









int NC_STACK_win3d::LoadFontByDescr(const std::string &fontname)
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

size_t NC_STACK_win3d::windd_func0(IDVList &stak)
{
    int txt16bit_def = read_yes_no_status("env/txt16bit.def", 1);
    int drawprim_def = read_yes_no_status("env/drawprim.def", 0);
    _export_window_mode = win3d_keys[13].Get<bool>();     // gfx.export_window_mode

    switch(win3d_keys[14].Get<int>())
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

    //log_d3dlog(" picked mode %s\n", picked.name.c_str());

    if ( !NC_STACK_display::func0(stak) )
        return 0;

    _forcesoftcursor = 0;
    _movie_player = win3d_keys[9].Get<bool>();
    _disable_lowres = win3d_keys[12].Get<bool>();
    _txt16bit = txt16bit_def;
    _use_simple_d3d = drawprim_def;

    _solidFont = win3d_keys[15].Get<bool>();

    switch( win3d_keys[16].Get<int>() )
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

    fpsLimitter(win3d_keys[17].Get<int>());

    LoadFontByDescr("MS Sans Serif,12,400,0");

    //win3d->field_54______rsrc_field4 = (bitmap_intern *)getRsrc_pData();
    return 1;
}









size_t NC_STACK_win3d::func0(IDVList &stak)
{
    Common::Ini::ParseIniFile(DefaultIniFile, &win3d_keys);

    if ( !windd_func0(stak) )
        return 0;

    _dither = win3d_keys[0].Get<bool>();
    _filter = win3d_keys[1].Get<bool>();
    _antialias = win3d_keys[2].Get<bool>();
    _zbuf_when_tracy = win3d_keys[4].Get<bool>();
    _colorkey = win3d_keys[5].Get<bool>();

    if ( can_srcblend )
        _alpha = 192;
    else
        _alpha = 128;

    win3dInitialisation();

    ApplyResolution();

    return 1;
}

void NC_STACK_win3d::ApplyResolution()
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

bool NC_STACK_win3d::ChangeResolution(Common::Point res, bool windowed)
{    
    NC_STACK_display::ChangeResolution(res, windowed);
        
    ApplyResolution();
    
    return true;
}

/*bool NC_STACK_win3d::ChangeResolution(int32_t mode)
{
    GfxMode picked;
    if ( mode )
    {
        for (const GfxMode &m : graphicsModes)
        {
            if ( m.sortid == mode )
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
    
    _mode = picked.mode;
    _sort_id = picked.sortid;
    
    ChangeResolution(Common::Point(picked.w, picked.h), picked.windowed);
    return true;
}*/

size_t NC_STACK_win3d::func1()
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

    return NC_STACK_display::func1();
}


size_t NC_STACK_win3d::raster_func192(IDVPair *)
{
    return 1;
}

size_t NC_STACK_win3d::raster_func198(const Common::FLine &arg)
{
    float tX = _field_554 - 1.0;
    float tY = _field_558 - 1.0;

    int y1 = (arg.y1 + 1.0) * tY;
    int y2 = (arg.y2 + 1.0) * tY;
    int x1 = (arg.x1 + 1.0) * tX;
    int x2 = (arg.x2 + 1.0) * tX;

    GFX::Engine.DrawLine(GFX::Engine.Screen(),
                      Common::Line(x1, y1, x2, y2),
                      _field_4.r,
                      _field_4.g,
                      _field_4.b);
    return 1;
}


size_t NC_STACK_win3d::raster_func199(const Common::Line &arg)
{
    GFX::Engine.DrawLine(GFX::Engine.Screen(),
                      Common::Line(_field_54c + arg.x1, _field_550 + arg.y1,
                                   _field_54c + arg.x2, _field_550 + arg.y2),
                      _field_4.r,
                      _field_4.g,
                      _field_4.b );

    return 1;
}

void NC_STACK_win3d::sub_420EDC(Common::Line line, uint8_t r, uint8_t g, uint8_t b)
{
    if ( line.ClipBy(_clip) )
    {
        Common::Line tmp2 = line;

        if ( _inverseClip.IsEmpty() || !tmp2.ClipBy(_inverseClip) )
        {
            GFX::Engine.DrawLine(GFX::Engine.Screen(), line, r, g, b);
        }
        else
        {
            if ( tmp2.P2() != line.P2() )
            {
                if ( tmp2.P1() != line.P1() )
                {
                    GFX::Engine.DrawLine(GFX::Engine.Screen(), Common::Line(line.P1(), tmp2.P1()), r, g, b);
                    GFX::Engine.DrawLine(GFX::Engine.Screen(), Common::Line(tmp2.P2(), line.P2()), r, g, b);
                }
                else
                {
                    GFX::Engine.DrawLine(GFX::Engine.Screen(), Common::Line(tmp2.P2(), line.P2()), r, g, b);
                }
            }
            else
            {
                GFX::Engine.DrawLine(GFX::Engine.Screen(), Common::Line(line.P1(), tmp2.P1()), r, g, b);
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func200(const Common::FLine &arg)
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

size_t NC_STACK_win3d::raster_func201(const Common::Line &l)
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

size_t NC_STACK_win3d::raster_func202(rstr_arg204 *arg)
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
    
    SDL_BlitScaled(arg->pbitm->swTex, &src, GFX::Engine.Screen(), &dst);
    
    return 1;
}

size_t NC_STACK_win3d::raster_func204(rstr_arg204 *arg)
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
        SDL_BlitScaled(arg->pbitm->swTex, &src, GFX::Engine.Screen(), &dst);
    }

    return 1;
}

void NC_STACK_win3d::SetRenderStates(int setAll)
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


void NC_STACK_win3d::sb_0x43b518(polysDat *in, int a5, int a6)
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
        if ( polysDat->renderFlags & 0x30 && !a6 )
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
            vtx[i].a = _alpha;
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

    if (win3d_keys[18].Get<bool>())
    {
        if (polysDat->renderFlags & RFLAGS_SKY)
        {
            _rendStates2[ZWRITEENABLE] = 0;
        }
        else if (polysDat->renderFlags & RFLAGS_FALLOFF)
        {
            _rendStates2[ZWRITEENABLE] = 1;
            _rendStates2[ALPHABLENDENABLE] = 1;
            _rendStates2[TEXTUREMAPBLEND] = 1;//D3DTBLEND_MODULATEALPHA;
            _rendStates2[SRCBLEND] = 2;//D3DBLEND_SRCALPHA;
            _rendStates2[DESTBLEND] = 3;//D3DBLEND_INVSRCALPHA;
            _rendStates2[SHADEMODE] = 1;//D3DSHADE_FLAT;
            _rendStates2[STIPPLEENABLE] = 0;

            float transDist = win3d_keys[19].Get<int>();
            float transLen = win3d_keys[20].Get<int>();

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

size_t NC_STACK_win3d::raster_func206(polysDat *arg)
{
    sb_0x43b518(arg, 0, 0);

    return 1;
}

void NC_STACK_win3d::win3d_func209__sub0(char *cmdline, char **arr)
{
    int v11;

    int bytesPerColor = GFX::Engine.Screen()->format->BytesPerPixel;

    char *curpos = cmdline;
    int w_pixels = GFX::Engine.Screen()->pitch / bytesPerColor;
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

            SDL_Rect srcR, dstR;
            dstR.x = x_out;
            dstR.y = y_out;
            dstR.w = cpy_width;
            dstR.h = cpy_height;
            
            srcR.x = chrr.x + x_off;
            srcR.y = chrr.y + y_off;
            srcR.h = cpy_height;
            
            if (v11)
                srcR.w = cpy_width;
            else
                srcR.w = 1;
            
            for(int i = 0; i < cpy_width; i += srcR.w)
            {
                SDL_BlitSurface(tile->img->GetSwTex(), &srcR, GFX::Engine.Screen(), &dstR);
                dstR.x += srcR.w;
            }                

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

void NC_STACK_win3d::raster_func209(w3d_a209 *arg)
{
    win3d_func209__sub0(arg->cmdbuf, arg->includ);
}


void NC_STACK_win3d::BeginScene()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glLoadMatrixd(_frustum);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    _sceneBeginned = 1;
}

bool NC_STACK_win3d::compare(polysDat *a, polysDat *b)
{
    return a->range > b->range;
}

void NC_STACK_win3d::RenderTransparent()
{
    if ( _sceneBeginned )
    {
        if ( _pending.size() )
        {
            std::stable_sort(_pending.begin(), _pending.end(), compare);

            for (std::deque<polysDat *>::iterator it = _pending.begin(); it != _pending.end(); it++)
            {
                sb_0x43b518((*it), 1, 1);
            }

            _pending.clear();
        }
    }
}

// Draw transparent
void NC_STACK_win3d::EndScene()
{
    RenderTransparent();

    _sceneBeginned = 0;
}

void NC_STACK_win3d::raster_func218(rstr_218_arg *arg)
{
    Common::Rect sRect( (arg->rect1.left + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.top + 1.0) * (arg->bitm_intern->height / 2),
                        (arg->rect1.right + 1.0) * (arg->bitm_intern->width / 2),
                        (arg->rect1.bottom + 1.0) * (arg->bitm_intern->height / 2) );

    Common::Rect dRect( (arg->rect2.left + 1.0) * _field_554,
                        (arg->rect2.top + 1.0) * _field_558,
                        (arg->rect2.right + 1.0) * _field_554,
                        (arg->rect2.bottom + 1.0) * _field_558 );

    GFX::Engine.BlitScaleMasked(arg->bitm_intern->swTex, sRect, arg->bitm_intern2->swTex, arg->flg, GFX::Engine.Screen(), dRect);
}



void NC_STACK_win3d::BeginFrame()
{
    SDL_FillRect(GFX::Engine.Screen(), NULL, SDL_MapRGBA(GFX::Engine.Screen()->format, 0, 0, 0, 0) );
    
    Common::Point scrSz = System::GetResolution();
    glViewport(0, 0, scrSz.x, scrSz.y);
    
    glPushAttrib(GL_DEPTH_WRITEMASK);
    glDepthMask(GL_TRUE);

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPopAttrib();
}

void NC_STACK_win3d::EndFrame()
{
    Gui::Root::Instance.Draw(GFX::Engine.RealScreen());
    GFX::Engine.DrawScreenSurface();
    Gui::Root::Instance.HwCompose();
    
    System::Flip();
}




void NC_STACK_win3d::win3d_func262__sub0(int a2, int *a3, int *a4)
{
    float cl1 = 0.0;
    float cl2 = 0.0;
    float cl3 = 0.0;

    if ( a2 )
    {
        float tclr1 = 0.0;
        float tclr2 = 0.0;
        float tclr3 = 0.0;

        for(int i = 0; i < a2; i++)
        {
            float v15 = a4[i] / 255.0;
            int v14 = a3[i];

            tclr1 += _greyColors[v14][0] * v15;
            tclr2 += _greyColors[v14][1] * v15;
            tclr3 += _greyColors[v14][2] * v15;
        }

        cl1 = tclr1;
        cl2 = tclr2;
        cl3 = tclr3;
    }
    if ( cl1 > 1.0 )
        cl1 = 1.0;
    if ( cl2 > 1.0 )
        cl2 = 1.0;
    if ( cl3 > 1.0 )
        cl3 = 1.0;

    _greyColors[8][0] = cl1;
    _greyColors[8][1] = cl2;
    _greyColors[8][2] = cl3;
}

void NC_STACK_win3d::display_func262(rstr_262_arg *arg)
{
    win3d_func262__sub0(arg->cnt, arg->slot, arg->weight);
    NC_STACK_display::display_func262(arg);
}

bool NC_STACK_win3d::AllocTexture(ResBitmap *bitm)
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
        
        glPopAttrib();
    }

    return true;
}

void NC_STACK_win3d::FreeTexture(ResBitmap *bitm)
{
    if ( bitm->hwTex )
        glDeleteTextures(1, &bitm->hwTex);

    bitm->hwTex = 0;
}

void NC_STACK_win3d::display_func271(IDVPair *stak)
{

}

void NC_STACK_win3d::display_func272(IDVPair *)
{
}




void win3d_func274__sub0(FSMgr::FileHandle *fil)
{
    int bf_w = 0, bf_h = 0;

    uint8_t *buf = GFX::Engine.MakeScreenCopy(&bf_w, &bf_h);

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

void NC_STACK_win3d::SaveScreenshot(const std::string & screenName)
{
    FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("%s.ppm", screenName), "wb");
    if ( fil )
    {
        win3d_func274__sub0(fil);
        delete fil;
    }
}


void NC_STACK_win3d::windd_func320(IDVPair *)
{
}

void NC_STACK_win3d::windd_func321(IDVPair *)
{
}





char * NC_STACK_win3d::windd_func322__sub0(const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, uint32_t timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen)
{
    dprintf("MAKE ME %s\n","windd_func322__sub0");
    return NULL;
}

//Show DLGBox with edit field and get entered value
void NC_STACK_win3d::windd_func322(windd_dlgBox *dlgBox)
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





void windd_func323__sub0__sub0(const char *filename)
{
    printf("MAKE MOVIE PLAYER %s, file %s\n","windd_func323__sub0__sub0", filename);
}


void NC_STACK_win3d::windd_func323__sub0(const char *filename)
{
////    unk_514F20 = 1;
    if ( _movie_player )
        windd_func323__sub0__sub0(filename);
////    unk_514F20 = 0;
}

//Play movie file
void NC_STACK_win3d::windd_func323(const char **filename)
{
    windd_func323__sub0(*filename);
}

void NC_STACK_win3d::windd_func324(wdd_func324arg *inout)
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

void NC_STACK_win3d::windd_func325(wdd_func324arg *arg)
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



void NC_STACK_win3d::setWDD_cursor(int mode)
{
}

void NC_STACK_win3d::setWDD_disLowRes(int arg)
{
}

void NC_STACK_win3d::setWDD_16bitTex(int arg)
{
    out_yes_no_status("env/txt16bit.def", arg);
}

void NC_STACK_win3d::setWDD_drawPrim(int arg)
{
    out_yes_no_status("env/drawprim.def", arg);
}



int NC_STACK_win3d::getWDD_16bitTex()
{
    return _txt16bit;
}

int NC_STACK_win3d::getWDD_drawPrim()
{
    return _use_simple_d3d;
}

void NC_STACK_win3d::setW3D_texFilt(int arg)
{
    _filter = arg;
}



void NC_STACK_win3d::draw2DandFlush()
{
    Gui::Root::Instance.Draw(GFX::Engine.RealScreen());
    GFX::Engine.DrawScreenSurface();
    Gui::Root::Instance.HwCompose();

    SDL_FillRect(GFX::Engine.Screen(), NULL, SDL_MapRGBA(GFX::Engine.Screen()->format, 0, 0, 0, 0) );
}

void NC_STACK_win3d::matrixAspectCorrection(mat3x3 &inout, bool invert)
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

void NC_STACK_win3d::getAspectCorrection(float &cW, float &cH, bool invert)
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

void NC_STACK_win3d::setFrustumClip(float _near, float _far)
{
    if (_near != _frustumNear || _far != _frustumFar)
        _setFrustumClip(_near, _far);
}

void NC_STACK_win3d::_setFrustumClip(float _near, float _far)
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

void NC_STACK_win3d::ConvAlphaPalette(UA_PALETTE *dst, const UA_PALETTE &src, bool transp)
{
    for (uint16_t i = 0; i < dst->size(); i++)
    {
        SDL_Color &c = (*dst)[i];
        c = src[i];
        c.a = 255;

        if (c.a == 255 && c.g == 255 && c.b == 0)
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

SDL_PixelFormat *NC_STACK_win3d::GetScreenFormat()
{
    return GFX::Engine.Screen()->format;
}

SDL_Surface *NC_STACK_win3d::CreateSurfaceScreenFormat(int width, int height)
{
    SDL_PixelFormat *fmt = GFX::Engine.C3D()->GetScreenFormat();
#if SDL_VERSION_ATLEAST(2,0,5)
    return SDL_CreateRGBSurfaceWithFormat(0, width, height, fmt->BitsPerPixel, fmt->format);
#else
    return SDL_CreateRGBSurface(0, width, height, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
}

SDL_Surface *NC_STACK_win3d::ConvertToScreenFormat(SDL_Surface *src)
{
    return ConvertSDLSurface(src, GFX::Engine.Screen()->format);
}

SDL_Surface * NC_STACK_win3d::ConvertSDLSurface(SDL_Surface *src, const SDL_PixelFormat * fmt)
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

