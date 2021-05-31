#include <string>
#include <list>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "../fmtlib/printf.h"

#include "system.h"
#include "fsmgr.h"
#include "../utils.h"


namespace System
{
    
struct FontNode
{
    std::string Path;
    std::string Name;
    std::string Style;
    
    FontNode() {};
    FontNode(const std::string &p, const std::string &n, const std::string &s)
    : Path(p)
    , Name(n)
    , Style(s)
    {};
    
    FontNode(const FontNode &&fnt)
    {
        Path = std::move(fnt.Path);
        Name = std::move(fnt.Name);
        Style = std::move(fnt.Style);
    };
};

struct TFontLookup
{
    const std::string Name; // Font name or part
    const std::string Alt1;
    const std::string Alt2;
};


ResRatio::ResRatio()
{}

ResRatio::ResRatio(const Common::Point &p)
: W(p.x), H(p.y)
{
    R = (float)W / (float)H;
}

ResRatio::ResRatio(int32_t w, int32_t h)
: W(w), H(h)
{
    R = (float)W / (float)H;
}

bool ResRatio::operator==(const ResRatio &b) const
{
    if (W != b.W || H != b.H)
        return false;
    return true;
}

bool ResRatio::operator!=(const ResRatio &b) const
{
    if (W == b.W || H == b.H)
        return false;
    return true;
}

ResRatio::operator Common::Point() const
{
    return Common::Point(W, H);
}

void ResRatio::Set(int32_t w, int32_t h)
{
    W = w;
    H = h;
    R = (float)W / (float)H;
}


static std::list< SDL_EventFilter > EventHandlers;
static std::list< FontNode > FontsList;

static TFontLookup FontsLookup[]
{
    {"arial", "liberation sans", ""},
    {"dungeon", "xolonium bold", "xolonium regular"},
    {"ms sans serif", "microsoft sans serif", "liberation serif"},
    {"courier", "liberation mono", ""},
    {"small font", "press start 2p", ""},
    {"ＭＳ Ｐゴシック", "MS PGothic", "Textar"},
    {"MS PGothic", "Textar", ""}
};
    
    
static SDL_Window *window = NULL;
static SDL_GLContext cont = NULL;
static ResRatio winRes(640, 480);
static bool curRelMouse = false;

void ScanFonts()
{
    FontsList.clear();

    FSMgr::DirIter dir = FSMgr::iDir::readDir("fonts/");
    if (dir)
    {
        FSMgr::iNode *nod = NULL;
        while(dir.getNext(&nod))
        {
            if (nod && nod->getType() == FSMgr::iNode::NTYPE_FILE)
            {
                std::string tmp = nod->getName();
                for (size_t i = 0; i < tmp.length(); i++)
                    tmp[i] = std::tolower(tmp[i]);

                if( tmp.find(".ttf") != std::string::npos || tmp.find(".otf") != std::string::npos || tmp.find(".fon") != std::string::npos )
                {
                    TTF_Font *fnt = TTF_OpenFont(nod->getPath().c_str(), 12);
                    if (fnt)
                    {
                        FontsList.push_back( FontNode(nod->getPath(), TTF_FontFaceFamilyName(fnt), TTF_FontFaceStyleName(fnt)) );

                        TTF_CloseFont(fnt);
                    }
                }
            }
        }
    }
}

std::string FindFont(const std::string &fontName)
{
    for(const FontNode &fnt : FontsList )
    {
        if (StriCmp(fontName, fnt.Name) == 0)
            return fnt.Path;
        else
        {
            std::string ttmp = fnt.Name;
            ttmp += " ";
            ttmp += fnt.Style;

            if (StriCmp(fontName, ttmp) == 0)
                return fnt.Path;
        }
    }

    return std::string();
}


TTF_Font *LoadFont(const std::string &fontname, int height)
{
    std::string tmp2 = fontname;
    for (size_t i = 0; i < tmp2.length(); i++)
        tmp2[i] = std::tolower(tmp2[i]);

    TTF_Font *fnt = NULL;

    std::string filename = FindFont(tmp2);

    if (filename.empty())
    {
        for( const TFontLookup &lookup : FontsLookup )
        {
            if ( !StriCmp(tmp2, lookup.Name) )
            {
                filename = FindFont(lookup.Alt1);

                if (!filename.empty())
                    break;

                filename = FindFont(lookup.Alt2);

                if (!filename.empty())
                    break;
            }
        }
    }

    if (filename.empty())
    {
        for( const TFontLookup &lookup : FontsLookup )
        {
            if ( tmp2.find(lookup.Name) != std::string::npos )
            {
                filename = FindFont(lookup.Alt1);

                if (!filename.empty())
                    break;

                filename = FindFont(lookup.Alt2);

                if (!filename.empty())
                    break;
            }
        }
    }

    if (!filename.empty())
        fnt = TTF_OpenFont(filename.c_str(), height);

    if (!fnt)
        fmt::printf("Can't load font %s %d\n", fontname, height);

    return fnt;
}


void Init()
{
    EventHandlers.clear();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
    {
        printf("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("OpenUA (Urban Assault)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winRes.W, winRes.H, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (!window)
    {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }

    cont = SDL_GL_CreateContext(window);

    TTF_Init();

    ScanFonts();

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    SDLNet_Init();
}

void Deinit()
{
    IMG_Quit();

    FontsList.clear();

    if (!cont)
        SDL_GL_DeleteContext(cont);

    if (!window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}


bool ProcessEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
        case SDL_WINDOWEVENT:
        {
            switch(event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:

                int w, h;
                SDL_GL_GetDrawableSize(window, &w, &h);
                winRes = ResRatio(w, h);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                return true;

            default:
                break;
            }

        }
        break;
        
        case SDL_QUIT:
            return true;

        default:
            break;
        }
        
        for(SDL_EventFilter &e : EventHandlers)
        {
            if ( (e)(NULL, &event) == 0 )
                break;
        }
    }

    return false;
}

ResRatio GetRResolution()
{
    return winRes;
}

Common::Point GetResolution()
{
    return Common::Point(winRes.W, winRes.H);
}

void Flip()
{
    SDL_GL_SwapWindow(window);
}

void SetVideoMode(const Common::Point &sz, uint32_t full, SDL_DisplayMode *mode)
{
    if (window)
    {
        SDL_SetWindowFullscreen(window, full);

        if (full != 0 && mode)
        {
            SDL_Delay(250);
            SDL_SetWindowDisplayMode(window, mode);
        }
        else
        {
            if (winRes != sz)
            {
                if (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED)
                {
                    SDL_RestoreWindow(window);
                    SDL_Delay(250);
                    
                    SDL_SetWindowSize(window, sz.x, sz.y);
                    
                    SDL_MaximizeWindow(window);
                    SDL_Delay(250);
                }
                else
                    SDL_SetWindowSize(window, sz.x, sz.y);
            }
        }
        
        int w, h;
        SDL_GL_GetDrawableSize(window, &w, &h);
        winRes = ResRatio(w, h);
    }
}

void RestoreWindow()
{
    SDL_RestoreWindow(window);
    SDL_Delay(250);
}

void SetReleativeMouse(bool mode)
{
    if (mode != curRelMouse)
    {
        if (mode)
            SDL_SetRelativeMouseMode(SDL_TRUE);
        else
        {
            SDL_SetRelativeMouseMode(SDL_FALSE);

            int WW = 0, WH = 0;
            SDL_GetWindowSize(window, &WW, &WH);
            SDL_WarpMouseInWindow(window, WW / 2, WH / 2);
        }

        curRelMouse = mode;
    }
}

void EventsAddHandler(SDL_EventFilter func, bool first)
{
    if (first)
        EventHandlers.push_front(func);
    else
        EventHandlers.push_back(func);
}

void EventsDeleteHandler(SDL_EventFilter func)
{
    EventHandlers.remove(func);
}

void PostQuitMessage()
{
    SDL_Event event;
    event.type = SDL_QUIT;
    SDL_PushEvent(&event);
}

}
