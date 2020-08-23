#include "uacommon.h"


namespace Gui
{
    
std::array<TileMap *, UA::TILESET_MAX> UA::_UATiles;
SDL_Color UA::_UATextColor;
TTF_Font *UA::_UATTF = NULL;
int UA::_UAFontH = 0;
NC_STACK_ypaworld *UA::yw = NULL;


void UA::Init()
{
    for (auto &x: _UATiles)
        x = NULL;
}

void UA::Deinit()
{
    for (auto x: _UATiles)
    {
        if (x)
            delete x;
    }
}

void UA::LoadFont(const std::string &fontstr)
{
    auto res = Stok::Split(fontstr, ","); // Fontname, height

    std::string fontname;
    
    if ( res.size() >= 2 && !res[0].empty() && !res[1].empty() )
    {
        _UAFontH = stoi(res[1]);
        fontname = res[0];
    }
    else
    {
        _UAFontH = 12;
        fontname = "MS Sans Serif";
    }

    if ( _UATTF )
    {
        TTF_CloseFont(_UATTF);
        _UATTF = NULL;
    }

    //stack__win3d.font.height = height;
    _UATTF = SDLWRAP_loadFont(fontname, _UAFontH);

    if (!_UATTF)
        return;
   
    if (!NC_STACK_win3d::win3d_keys[15].Get<bool>())
        TTF_SetFontHinting(_UATTF, TTF_HINTING_LIGHT);
    else
        TTF_SetFontHinting(_UATTF, TTF_HINTING_MONO);
}

void UA::DrawText(SDL_Surface *srf, const std::string &text, int flag, const SDL_Color &color, const Common::PointRect& area)
{
    if (text.empty())
        return;

    int cx = 0, cy = 0;

    if ( flag & (TXTFLAG_RIGHTALIGN | TXTFLAG_CENTER | TXTFLAG_RELWIDTH) )
        TTF_SizeUTF8(_UATTF, text.c_str(), &cx, &cy);

    Common::PointRect pp = area;
    if ( flag & TXTFLAG_RELWIDTH )
        pp.w = cx * pp.w / 100;

    SDL_Rect clipRect = pp;
    clipRect.w += 4;
    clipRect.h += 1;

    if ( flag & TXTFLAG_RIGHTALIGN )
    {
        if ( flag & TXTFLAG_RELWIDTH )
        {
            pp.x -= cx;
            clipRect.x = pp.x;
            clipRect.w = pp.w + 4;
        }
        else
        {
            pp.x += (pp.w - cx);
        }
    }
    else if ( flag & TXTFLAG_CENTER )
    {
        if ( flag & TXTFLAG_RELWIDTH )
        {
            pp.x -= cx / 2;
            clipRect.x = pp.x;
            clipRect.w = pp.w + 4;
        }
        else
        {
            pp.x += (pp.w - cx) / 2;
        }
    }

    SDL_SetClipRect(srf, &clipRect);

    int dy = pp.y + ((pp.h - _UAFontH) / 2) - 2;
    if ( flag & TXTFLAG_PRESSED )
    {
        dy++;
        pp.x++;
    }

    SDL_Color clr;
    clr.a = 255;
    clr.r = 0;
    clr.g = 0;
    clr.b = 0;

    SDL_Surface *tmp;

    if (NC_STACK_win3d::win3d_keys[15].Get<bool>())
    {
        tmp = TTF_RenderUTF8_Solid(_UATTF, text.c_str(), clr);
        SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_NONE);
    }
    else
    {
        tmp = TTF_RenderUTF8_Blended(_UATTF, text.c_str(), clr);
        SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
    }

    SDL_Rect want;
    want.w = tmp->w;
    want.h = tmp->h;
    want.x = pp.x + 2;
    want.y = dy + 1;

    SDL_BlitSurface(tmp, NULL, srf, &want);

    clr = color;
    
    if (NC_STACK_win3d::win3d_keys[15].Get<bool>())
    {
        SDL_SetPaletteColors(tmp->format->palette, &clr, 1, 1);
    }
    else
    {
        SDL_FreeSurface(tmp);
        tmp = TTF_RenderUTF8_Blended(_UATTF, text.c_str(), clr);
        SDL_SetSurfaceBlendMode(tmp, SDL_BLENDMODE_BLEND);
    }

    want.w = tmp->w;
    want.h = tmp->h;
    want.x = pp.x + 1;
    want.y = dy;

    SDL_BlitSurface(tmp, NULL, srf, &want);
    SDL_FreeSurface(tmp);


    SDL_SetClipRect(srf, NULL);
}

}