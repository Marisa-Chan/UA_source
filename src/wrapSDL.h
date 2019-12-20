#ifndef WRAP_SDL_H_INCLUDED
#define WRAP_SDL_H_INCLUDED

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_ttf.h>
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
}

TTF_Font *SDLWRAP_loadFont(const char *fontname, int height);

uint8_t *SDLWRAP_makeScreenCopy(int &ow, int &oh);
uint8_t *SDLWRAP_makeDepthScreenCopy(int &ow, int &oh);

void SDLWRAP_addHandler(SDL_EventFilter);
void SDLWRAP_GL_mapFormat(uint32_t pixelFormat, GLint *format, GLint *type);
void SDLWRAP_setFullscreen(uint32_t full, SDL_DisplayMode *mode);
uint32_t SDLWRAP_CorrectFormat(uint32_t format);
void SDLWRAP_drawScreen();
void SDLWRAP_flipWindow();

void SDLWRAP_releativeMouse(bool mode);

void SDLWRAP_mousePosNorm(SDLWRAP_Point &in);

void SDLWRAP_restoreWindow();

enum VKK
{
    UAVK_BACK = 8,
    UAVK_TAB = 9,
    UAVK_CLEAR = 12,
    UAVK_RETURN = 13,
    UAVK_SHIFT = 16,
    UAVK_CONTROL = 17,
    UAVK_MENU = 18,
    UAVK_PAUSE = 19,
    UAVK_CAPITAL = 20,
    UAVK_ESCAPE = 0x1B,
    UAVK_SPACE = 32,
    UAVK_PRIOR = 33,
    UAVK_NEXT = 34,
    UAVK_END = 35,
    UAVK_HOME = 36,
    UAVK_LEFT = 37,
    UAVK_UP = 38,
    UAVK_RIGHT = 39,
    UAVK_DOWN = 40,
    UAVK_SELECT = 41,
    UAVK_PRINT = 42,
    UAVK_EXECUTE = 43,
    UAVK_SNAPSHOT = 44,
    UAVK_INSERT = 45,
    UAVK_DELETE = 46,
    UAVK_HELP = 47,
    UAVK_0    = 0x30,
    UAVK_A    = 0x41,
    UAVK_B    = 0x42,
    UAVK_C    = 0x43,
    UAVK_D    = 0x44,
    UAVK_E    = 0x45,
    UAVK_F    = 0x46,
    UAVK_G    = 0x47,
    UAVK_H    = 0x48,
    UAVK_I    = 0x49,
    UAVK_J    = 0x4A,
    UAVK_K    = 0x4B,
    UAVK_L    = 0x4C,
    UAVK_M    = 0x4D,
    UAVK_N    = 0x4E,
    UAVK_O    = 0x4F,
    UAVK_P    = 0x50,
    UAVK_Q    = 0x51,
    UAVK_R    = 0x52,
    UAVK_S    = 0x53,
    UAVK_T    = 0x54,
    UAVK_U    = 0x55,
    UAVK_V    = 0x56,
    UAVK_W    = 0x57,
    UAVK_X    = 0x58,
    UAVK_Y    = 0x59,
    UAVK_Z    = 0x5A,
    UAVK_NUMPAD0 = 0x60,
    UAVK_NUMPAD1 = 0x61,
    UAVK_NUMPAD2 = 0x62,
    UAVK_NUMPAD3 = 0x63,
    UAVK_NUMPAD4 = 0x64,
    UAVK_NUMPAD5 = 0x65,
    UAVK_NUMPAD6 = 0x66,
    UAVK_NUMPAD7 = 0x67,
    UAVK_NUMPAD8 = 0x68,
    UAVK_NUMPAD9 = 0x69,
    UAVK_MULTIPLY = 0x6A,
    UAVK_ADD = 0x6B,
    UAVK_SEPARATOR = 0x6C,
    UAVK_SUBTRACT = 0x6D,
    UAVK_DECIMAL = 0x6E,
    UAVK_DIVIDE = 0x6F,
    UAVK_F1 = 0x70,
    UAVK_F2 = 0x71,
    UAVK_F3 = 0x72,
    UAVK_F4 = 0x73,
    UAVK_F5 = 0x74,
    UAVK_F6 = 0x75,
    UAVK_F7 = 0x76,
    UAVK_F8 = 0x77,
    UAVK_F9 = 0x78,
    UAVK_F10 = 0x79,
    UAVK_F11 = 0x7A,
    UAVK_F12 = 0x7B,
    UAVK_F13 = 0x7C,
    UAVK_F14 = 0x7D,
    UAVK_F15 = 0x7E,
    UAVK_F16 = 0x7F,
    UAVK_F17 = 0x80,
    UAVK_F18 = 0x81,
    UAVK_F19 = 0x82,
    UAVK_F20 = 0x83,
    UAVK_F21 = 0x84,
    UAVK_F22 = 0x85,
    UAVK_F23 = 0x86,
    UAVK_F24 = 0x87,
    UAVK_NUMLOCK = 0x90,
    UAVK_SCROLL = 0x91,
    UAVK_LSHIFT = 0xA0,
    UAVK_RSHIFT = 0xA1,
    UAVK_LCONTROL = 0xA2,
    UAVK_RCONTROL = 0xA3,
    UAVK_LMENU = 0xA4,
    UAVK_RMENU = 0xA5,
    UAVK_OEM_1    = 0xBA,
    UAVK_OEM_2    = 0xBF,
    UAVK_OEM_3    = 0xC0,
    UAVK_OEM_4    = 0xDB,
    UAVK_OEM_5    = 0xDC,
    UAVK_OEM_6    = 0xDD,
    UAVK_OEM_7    = 0xDE,
    UAVK_OEM_8    = 0xDF,
    UAVK_OEM_PLUS   = 0xBB,
    UAVK_OEM_COMMA  = 0xBC,
    UAVK_OEM_MINUS  = 0xBD,
    UAVK_OEM_PERIOD = 0xBE,
    UAVK_OEM_102 = 0xE2,

};





#endif
