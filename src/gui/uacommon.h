#ifndef GUI_UACOMMON_H_INCLUDED
#define GUI_UACOMMON_H_INCLUDED

#include <array>
#include <inttypes.h>
#include <SDL2/SDL.h>
#include "../common.h"
#include "../engine_gfx.h"
#include "../yw.h"

#include "widget.h"

namespace Gui
{
    
class UA
{
public:
    enum TILESETS
    {
        TILESET_46MAPC16, //18
        TILESET_46MAPC32, //19
        TILESET_46ENERGY, //30
        
        TILESET_DEFAULT,  // 0
        TILESET_MENUGRAY, // 6
        TILESET_ICONNS,   // 24
        TILESET_ICONPS,   // 25
        
        TILESET_MAPHORZ,  // 11
        TILESET_MAPVERT,  // 12
        TILESET_MAPVERT1, // 13
        
        TILESET_MAX
    };
    static std::array<TileMap *, TILESET_MAX> _UATiles;
    static SDL_Color _UATextColor;
    static TTF_Font *_UATTF;
    
    static NC_STACK_ypaworld *yw;
    
    static void LoadFont(const std::string &fontstr);
    
    
    enum TXTFLAG
    {
        TXTFLAG_RIGHTALIGN  =   (1 << 0),
        TXTFLAG_CENTER      =   (1 << 1),
        TXTFLAG_RELWIDTH    =   (1 << 2),
        TXTFLAG_PRESSED     =   (1 << 3),
    };
    static void DrawText(SDL_Surface *srf, const std::string &text, int flag, const SDL_Color &color, const Common::PointRect& area);
    static void Init();
    static void Deinit();
    
protected:
    static int _UAFontH;
};

}

#endif