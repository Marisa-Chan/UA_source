#include <string>
#include <list>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_net.h>
#include "wrapSDL.h"
#include "fsmgr.h"
#include "utils.h"
#include "fmtlib/printf.h"


static SDL_Window *window = NULL;
static SDL_GLContext cont = NULL;
static SDL_Surface *screen = NULL;
static GLuint screenTex = 0;
static GLint pixfmt, pixtype;
static bool curRelMouse = false;



static int sW = 640, sH = 480;
static int curW = 640, curH = 480;

namespace SDLWRAP
{
static std::list< SDL_EventFilter > EventHandlers;
}


struct FontNode
{
    std::string filepath;
    std::string name;
    std::string stylename;
};

static std::list< FontNode * > fontsList;


/*SDLWRAP_mouseState mouseWork;
SDLWRAP_mouseState mousePubl;
SDLWRAP_keybState  keybWork;
SDLWRAP_keybState  keybPubl;
int VK_Map[SDL_NUM_SCANCODES];*/

static struct FontLookup
{
    const std::string sign; // Font name or part
    const std::string alt1;
    const std::string alt2;
} fontsLookup[] =
{
    {"arial", "liberation sans", ""},
    {"dungeon", "xolonium bold", "xolonium regular"},
    {"ms sans serif", "microsoft sans serif", "liberation serif"},
    {"courier", "liberation mono", ""},
    {"small font", "press start 2p", ""}
};

void SDLWRAP_ScanFonts()
{
    for(std::list< FontNode *>::iterator it = fontsList.begin(); it != fontsList.end(); it++)
    {
        if (*it)
            delete *it;
    }

    fontsList.clear();

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
                    tmp[i] = tolower(tmp[i]);

                if( strstr(tmp.c_str(), ".ttf") || strstr(tmp.c_str(), ".otf") || strstr(tmp.c_str(), ".fon") )
                {
                    TTF_Font *fnt = TTF_OpenFont(nod->getPath().c_str(), 12);
                    if (fnt)
                    {
                        FontNode *nd = new FontNode;
                        nd->filepath = nod->getPath();
                        nd->name = TTF_FontFaceFamilyName(fnt);
                        nd->stylename = TTF_FontFaceStyleName(fnt);

                        fontsList.push_back(nd);

                        TTF_CloseFont(fnt);
                    }
                }
            }
        }
    }
}

uint32_t SDLWRAP_CorrectFormat(uint32_t format)
{
    if (format == SDL_PIXELFORMAT_BGR888)
        return SDL_PIXELFORMAT_ABGR8888;
    else if (format == SDL_PIXELFORMAT_RGB888)
        return SDL_PIXELFORMAT_ARGB8888;
    return format;
}

void SDLWRAP_GL_mapFormat(uint32_t pixelFormat, GLint *format, GLint *type)
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

void SDLWRAP_recreateSurface()
{
    glViewport(0, 0, sW, sH);

    if ( screen )
        SDL_FreeSurface(screen);

    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    mode.format = SDLWRAP_CorrectFormat(mode.format);

    int bpp;
    uint32_t rm, gm, bm, am;
    SDL_PixelFormatEnumToMasks(mode.format, &bpp, &rm, &gm, &bm, &am);

    SDLWRAP_GL_mapFormat(mode.format, &pixfmt, &pixtype);

    screen = SDL_CreateRGBSurface(0, sW, sH, bpp, rm, gm, bm, am);

    if ( !screenTex )
        glGenTextures(1, &screenTex);

    glPushAttrib(GL_TEXTURE_BIT);

    glBindTexture(GL_TEXTURE_2D, screenTex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screen->w, screen->h, 0, pixfmt, pixtype, NULL);

    glPopAttrib();
}

void SDLWRAP_drawScreen()
{
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
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, screen->w, screen->h, pixfmt, pixtype, screen->pixels);

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



void SDLWRAP_INIT()
{
    SDLWRAP::EventHandlers.clear();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0)
    {
        printf("Couldn't initialize SDL: %s", SDL_GetError());
        return;
    }

    window = SDL_CreateWindow("OpenUA (Urban Assault)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, sW, sH, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

    if (!window)
    {
        printf("Couldn't create window and renderer: %s", SDL_GetError());
        return;
    }

    cont = SDL_GL_CreateContext(window);

    SDLWRAP_recreateSurface();

    TTF_Init();

    SDLWRAP_ScanFonts();

    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

    SDLNet_Init();
}

void SDLWRAP_DEINIT()
{
    IMG_Quit();

    for(std::list< FontNode *>::iterator it = fontsList.begin(); it != fontsList.end(); it++)
    {
        if (*it)
            delete *it;
    }

    fontsList.clear();

    if ( screen )
        SDL_FreeSurface(screen);

    if (screenTex)
        glDeleteTextures(1, &screenTex);

    if (!cont)
        SDL_GL_DeleteContext(cont);

    if (!window)
        SDL_DestroyWindow(window);

    SDL_Quit();
}


int SDLWRAP_UPDATE()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        for(SDL_EventFilter &e : SDLWRAP::EventHandlers)
            (e)(NULL, &event);

        switch(event.type)
        {

        case SDL_WINDOWEVENT:
        {
            switch(event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:

                SDL_GL_GetDrawableSize(window, &curW, &curH);
                glViewport(0, 0, curW, curH);
                break;

            case SDL_WINDOWEVENT_CLOSE:
                return 1;

            default:
                break;
            }

        }
        break;
        }
    }

    return 0;
}

void SDLWRAP_flipWindow()
{
    SDL_GL_SwapWindow(window);
}

void SDLWRAP_resizeWindow(int w, int h)
{
    if (w != sW || h != sH)
    {
        sW = w;
        sH = h;

        if (window)
            SDL_SetWindowSize(window, w, h);

        SDLWRAP_recreateSurface();
    }
}

void SDLWRAP_setFullscreen(uint32_t full, SDL_DisplayMode *mode)
{
    if (window)
    {
        SDL_SetWindowFullscreen(window, full);

        if (full != 0 && mode)
        {
            SDL_Delay(250);
            SDL_SetWindowDisplayMode(window, mode);
        }

    }
}

std::string SDLWRAP_FindFont(const std::string &fontName)
{
    for(FontNode *fnt : fontsList )
    {
        if (fnt)
        {
            if (StriCmp(fontName, fnt->name) == 0)
                return fnt->filepath;
            else
            {
                std::string ttmp = fnt->name;
                ttmp += " ";
                ttmp += fnt->stylename;

                if (StriCmp(fontName, ttmp) == 0)
                    return fnt->filepath;
            }
        }
    }

    return std::string();
}


TTF_Font *SDLWRAP_loadFont(const std::string &fontname, int height)
{
    std::string tmp2 = fontname;
    for (size_t i = 0; i < tmp2.length(); i++)
        tmp2[i] = tolower(tmp2[i]);

    TTF_Font *fnt = NULL;

    std::string filename = SDLWRAP_FindFont(tmp2);

    if (filename.empty())
    {
        for( const FontLookup &lookup : fontsLookup )
        {
            if ( !StriCmp(tmp2, lookup.sign) )
            {
                filename = SDLWRAP_FindFont(lookup.alt1);

                if (!filename.empty())
                    break;

                filename = SDLWRAP_FindFont(lookup.alt2);

                if (!filename.empty())
                    break;
            }
        }
    }

    if (filename.empty())
    {
        for( const FontLookup &lookup : fontsLookup )
        {
            if ( tmp2.find(lookup.sign) != std::string::npos )
            {
                filename = SDLWRAP_FindFont(lookup.alt1);

                if (!filename.empty())
                    break;

                filename = SDLWRAP_FindFont(lookup.alt2);

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

uint8_t *SDLWRAP_makeScreenCopy(int &ow, int &oh)
{
    int ww,wh;
    SDL_GL_GetDrawableSize(window, &ww, &wh);

    // power of 2
    ww &= ~1;
    wh &= ~1;

    uint8_t *buf = (uint8_t *)malloc(ww * wh * 3);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, ww, wh, GL_RGB, GL_UNSIGNED_BYTE, buf);

    ow = ww;
    oh = wh;
    return buf;
}

uint8_t *SDLWRAP_makeDepthScreenCopy(int &ow, int &oh)
{
    int ww,wh;
    SDL_GL_GetDrawableSize(window, &ww, &wh);

    // power of 2
    ww &= ~1;
    wh &= ~1;

    uint8_t *buf = (uint8_t *)malloc(ww * wh);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glReadBuffer(GL_FRONT);
    glReadPixels(0, 0, ww, wh, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, buf);

    ow = ww;
    oh = wh;
    return buf;
}



void SDLWRAP_releativeMouse(bool mode)
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

void SDLWRAP_restoreWindow()
{
    SDL_RestoreWindow(window);
    SDL_Delay(250);
}



namespace SDLWRAP
{    
SDL_Surface *Screen()
{
    return screen;
}

void EventsAddHandler(SDL_EventFilter func)
{
    EventHandlers.push_back(func);
}

// Draw line Bresenham's algorithm
void DrawLine(SDL_Surface *surface, const Common::Rect &line, uint8_t cr, uint8_t cg, uint8_t cb )
{
    if ((!line.Width() && !line.Height()) || 
         !Common::Rect(surface->w, surface->h).IsIn(Common::Point(line.left, line.top)) ||
         !Common::Rect(surface->w, surface->h).IsIn(Common::Point(line.right, line.bottom)) )
        return;
    
    int rilWidth = surface->pitch / surface->format->BytesPerPixel;

    int xCount = Common::ABS(line.Width());
    int yCount = Common::ABS(line.Height());

    uint32_t color = SDL_MapRGBA(surface->format, cr, cg, cb, 255);

    int stepAdd, stepOdd;
    int steps, subSteps;

    if ( xCount <= yCount )
    {
        if ( line.bottom <= line.top )
            stepAdd = -rilWidth;
        else
            stepAdd = rilWidth;

        if ( line.right <= line.left )
            stepOdd = -1;
        else
            stepOdd = 1;

        steps = yCount;
        subSteps = xCount;
    }
    else
    {
        if ( line.right <= line.left )
            stepAdd = -1;
        else
            stepAdd = 1;

        if ( line.bottom <= line.top )
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
                    + line.top * surface->pitch 
                    + line.left * surface->format->BytesPerPixel );

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

void BlitScaleMasked(SDL_Surface *src, Common::Rect sRect, SDL_Surface *mask, uint8_t index, SDL_Surface *dst, Common::Rect dRect)
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

void DrawFill(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect)
{
    if (sRect.IsEmpty() || dRect.IsEmpty())
        return;
    
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

void Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::Point dPoint)
{
    SDL_Rect Ssrc = sRect;
    SDL_Rect Sdst = dPoint;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

void Draw(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, Common::PointRect dRect)
{
    SDL_Rect Ssrc = sRect;
    if (Ssrc.w > dRect.w)
        Ssrc.w = dRect.w;
    if (Ssrc.h > dRect.h)
        Ssrc.h = dRect.h;
    SDL_Rect Sdst = dRect;
    SDL_BlitSurface(src, &Ssrc, dst, &Sdst);
}

Common::Point MousePosNorm(Common::Point in)
{
    if (sW != curW || sH != curH)
    {
        in.x = ((float)in.x / (float)curW) * (float)sW;
        in.y = ((float)in.y / (float)curH) * (float)sH;
    }

    if (in.x < 0)
        in.x = 0;
    if (in.x >= sW)
        in.x = sW - 1;

    if (in.y < 0)
        in.y = 0;
    if (in.y >= sH)
        in.y = sH - 1;
    
    return in;
}

}