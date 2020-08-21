#ifndef WRAP_SDL_H_INCLUDED
#define WRAP_SDL_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "common.h"


void SDLWRAP_INIT();
void SDLWRAP_DEINIT();
int SDLWRAP_UPDATE();

struct SDLWRAP_Point
{
    int x;
    int y;
};

void SDLWRAP_resizeWindow(int w, int h);

namespace SDLWRAP 
{
    SDL_Surface *Screen();
    
    void DrawLine(SDL_Surface *surface, const Common::Rect &line, uint8_t cr, uint8_t cg, uint8_t cb);
    void BlitScaleMasked(SDL_Surface *src, Common::Rect sRect, SDL_Surface *mask, uint8_t index, SDL_Surface *dst, Common::Rect dRect);
    void DrawFill(SDL_Surface *src, const Common::Rect &sRect, SDL_Surface *dst, const Common::Rect &dRect);
    
    Common::Point MousePosNorm(Common::Point in);
    
    void EventsAddHandler(SDL_EventFilter);
}

TTF_Font *SDLWRAP_loadFont(const std::string &fontname, int height);

uint8_t *SDLWRAP_makeScreenCopy(int &ow, int &oh);
uint8_t *SDLWRAP_makeDepthScreenCopy(int &ow, int &oh);

void SDLWRAP_GL_mapFormat(uint32_t pixelFormat, GLint *format, GLint *type);
void SDLWRAP_setFullscreen(uint32_t full, SDL_DisplayMode *mode);
uint32_t SDLWRAP_CorrectFormat(uint32_t format);
void SDLWRAP_drawScreen();
void SDLWRAP_flipWindow();

void SDLWRAP_releativeMouse(bool mode);



void SDLWRAP_restoreWindow();


#endif
