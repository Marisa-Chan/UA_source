#include <string>
#include <list>
#include "wrapSDL.h"
#include "fsmgr.h"


static SDL_Window *window = NULL;
static SDL_GLContext cont = NULL;
static SDL_Surface *screen = NULL;
static GLuint screenTex = 0;
static GLint pixfmt, pixtype;
static bool curRelMouse = false;



static int sW = 640, sH = 480;
static int curW = 640, curH = 480;

static std::list< SDL_EventFilter > eventHandlers;

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
    const char *sign; // Font name or part
    const char *alt1;
    const char *alt2;
} fontsLookup[] =
{
    {"arial", "liberation sans", ""},
    {"dungeon", "xolonium bold", "xolonium regular"},
    {"ms sans serif", "microsoft sans serif", "liberation serif"},
    {"courier", "liberation mono", ""},
    {"small font", "press start 2p", ""},
    {NULL, NULL, NULL}
};

void SDLWRAP_ScanFonts()
{
    for(std::list< FontNode *>::iterator it = fontsList.begin(); it != fontsList.end(); it++)
    {
        if (*it)
            delete *it;
    }

    fontsList.clear();

    FSMgr::DirIter *dir = FSMgr::iDir::readDir("fonts/");
    if (dir)
    {
        FSMgr::iNode *nod = NULL;
        while(dir->getNext(nod))
        {
            if (nod && nod->getType() == FSMgr::iNode::NTYPE_FILE)
            {
                std::string tmp = nod->getName();
                for (size_t i = 0; i < tmp.length(); i++)
                    tmp[i] = tolower(tmp[i]);

                if( strstr(tmp.c_str(), ".ttf") || strstr(tmp.c_str(), ".otf") || strstr(tmp.c_str(), ".fon") )
                {
                    TTF_Font *fnt = TTF_OpenFont(nod->getPath(), 12);
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

        delete dir;
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
    eventHandlers.clear();

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
}

void SDLWRAP_DEINIT()
{
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
        for(std::list< SDL_EventFilter >::iterator it = eventHandlers.begin(); it != eventHandlers.end(); it++)
            (*it)(NULL, &event);

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




void SDLWRAP_addHandler(SDL_EventFilter func)
{
    eventHandlers.push_back(func);
}

void SDLWRAP_flipWindow()
{
    SDLWRAP_drawScreen();

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

SDL_Surface *SDLWRAP_getScreenSurface()
{
    return screen;
}

const char *SDLWRAP_FindFont(const char *fontName)
{
    for(std::list< FontNode *>::iterator it = fontsList.begin(); it != fontsList.end(); it++)
    {
        if (*it)
        {
            FontNode *fntn = *it;

            if (strcasecmp(fontName, fntn->name.c_str()) == 0)
                return fntn->filepath.c_str();
            else
            {
                std::string ttmp = fntn->name;
                ttmp += " ";
                ttmp += fntn->stylename;

                if (strcasecmp(fontName, ttmp.c_str()) == 0)
                    return fntn->filepath.c_str();
            }
        }
    }

    return NULL;
}


TTF_Font *SDLWRAP_loadFont(const char *fontname, int height)
{
    std::string tmp2 = fontname;
    for (size_t i = 0; i < tmp2.length(); i++)
        tmp2[i] = tolower(tmp2[i]);

    TTF_Font *fnt = NULL;

    const char *filename = SDLWRAP_FindFont(tmp2.c_str());

    if (!filename)
    {
        for(int i = 0; fontsLookup[i].sign != NULL; i++ )
        {
            if ( strcmp(tmp2.c_str(), fontsLookup[i].sign) == 0 )
            {
                filename = SDLWRAP_FindFont(fontsLookup[i].alt1);

                if (filename)
                    break;

                filename = SDLWRAP_FindFont(fontsLookup[i].alt2);

                if (filename)
                    break;
            }
        }
    }

    if (!filename)
    {
        for(int i = 0; fontsLookup[i].sign != NULL; i++ )
        {
            if ( strstr(tmp2.c_str(), fontsLookup[i].sign) != NULL )
            {
                filename = SDLWRAP_FindFont(fontsLookup[i].alt1);

                if (filename)
                    break;

                filename = SDLWRAP_FindFont(fontsLookup[i].alt2);

                if (filename)
                    break;
            }
        }
    }

    if (filename)
        fnt = TTF_OpenFont(filename, height);

    if (!fnt)
        printf("Can't load font %s %d\n", fontname, height);

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

void SDLWRAP_mousePosNorm(SDLWRAP_Point &in)
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
