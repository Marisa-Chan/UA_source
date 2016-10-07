#include <string>
#include <map>
#include <list>
#include "wrapSDL.h"


static SDL_Window *window = NULL;
static SDL_GLContext cont = NULL;
static SDL_Surface *screen = NULL;
static GLuint screenTex = 0;
static GLint pixfmt, pixtype;



static int sW = 640, sH = 480;
static int curW = 640, curH = 480;

static std::list< SDL_EventFilter > eventHandlers;


/*SDLWRAP_mouseState mouseWork;
SDLWRAP_mouseState mousePubl;
SDLWRAP_keybState  keybWork;
SDLWRAP_keybState  keybPubl;
int VK_Map[SDL_NUM_SCANCODES];*/

static struct FontReplace
{
    const char *sign;
    const char *fontfile;
    const char *alt1;
    const char *alt2;
} fontsList[] =
{
    {"arial", "arial.ttf", "", ""},
    {"dungeon", "dungeon.ttf", "", ""},
    {"sans", "mssans.ttf", "", ""},
    {"small font", "small_font.ttf", "", ""},
    {"", "", "", ""}
};

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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
}

void SDLWRAP_DEINIT()
{
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
            SDL_SetWindowDisplayMode(window, mode);
    }
}

SDL_Surface *SDLWRAP_getScreenSurface()
{
    return screen;
}


TTF_Font *SDLWRAP_loadFont(const char *fontname, int height)
{
    std::string tmp2 = fontname;
    for (size_t i = 0; i < tmp2.length(); i++)
        tmp2[i] = tolower(tmp2[i]);

    TTF_Font *fnt = NULL;

    for(int i = 0; strcmp(fontsList[i].sign, "") != 0; i++ )
    {
        if ( strstr(tmp2.c_str(), fontsList[i].sign) != NULL)
        {
            std::string tmp = "fonts/";
            tmp += fontsList[i].fontfile;

            fnt = TTF_OpenFont(tmp.c_str(), height);

            if (!fnt)
            {
                tmp = "fonts/";
                tmp += fontsList[i].alt1;

                fnt = TTF_OpenFont(tmp.c_str(), height);
            }

            if (!fnt)
            {
                tmp = "fonts/";
                tmp += fontsList[i].alt2;

                fnt = TTF_OpenFont(tmp.c_str(), height);
            }

            if (fnt)
                break;
        }
    }

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

void SDLWRAP_wrapMouse()
{
    int ww = 0, wh = 0;
    SDL_GetWindowSize(window, &ww, &wh);
    SDL_WarpMouseInWindow(window, ww / 2, wh / 2);
}

void SDLWRAP_restoreWindow()
{
    SDL_RestoreWindow(window);
}
