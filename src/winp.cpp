#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "engine_input.h"
#include "winp.h"
#include "utils.h"
#include "log.h"
#include "inp_ff.h"
#include "gui/widget.h"

const Nucleus::ClassDescr NC_STACK_winp::description("winp.class", &newinstance);

struct winp__func67__internal
{
    const char *keyname;
    int keytype;
    int keycode;
    int down;
};

struct SDLWRAP_mouseState
{
    int l_state;
    int r_state;
    int m_state;
    int dbl_state;

    int lu_cnt, ld_cnt;
    int ru_cnt, rd_cnt;
    int mu_cnt, md_cnt;

    SDLWRAP_Point pos;
    SDLWRAP_Point move;
    SDLWRAP_Point __xrel;

    SDLWRAP_Point ld_pos;
    SDLWRAP_Point rd_pos;
    SDLWRAP_Point md_pos;

    SDLWRAP_Point lu_pos;
    SDLWRAP_Point ru_pos;
    SDLWRAP_Point mu_pos;
};

#define KEYBCHARBUFF 8

struct SDLWRAP_keybState
{
    int keyb_cont;
    int keyb_norm;

    int charBuff[KEYBCHARBUFF];
    int charBuffCnt;
};


winp__func67__internal winp_keys[129] = {{"nop", 1, 0xFF, 0},       {"esc", 1, UAVK_ESCAPE, 0},       {"space", 1, UAVK_SPACE, 0},      {"up", 1, UAVK_UP, 0},        {"down", 1, UAVK_DOWN, 0},
    {"left", 1, UAVK_LEFT, 0},   {"right", 1, UAVK_RIGHT, 0},      {"f1", 1, UAVK_F1, 0},            {"f2", 1, UAVK_F2, 0},        {"f3", 1, UAVK_F3, 0},
    {"f4", 1, UAVK_F4, 0},       {"f5", 1, UAVK_F5, 0},            {"f6", 1, UAVK_F6, 0},            {"f7", 1, UAVK_F7, 0},        {"f8", 1, UAVK_F8, 0},
    {"f9", 1, UAVK_F9, 0},       {"f10", 1, UAVK_F10, 0},          {"f11", 1, UAVK_F11, 0},          {"f12", 1, UAVK_F12, 0},      {"bs", 1, UAVK_BACK, 0},
    {"tab", 1, UAVK_TAB, 0},     {"clear", 1, UAVK_CLEAR, 0},      {"return", 1, UAVK_RETURN, 0},    {"ctrl", 1, UAVK_CONTROL, 0}, {"rshift", 1, UAVK_SHIFT, 0},
    {"lshift", 1, UAVK_SHIFT, 0},{"shift", 1, UAVK_SHIFT, 0},      {"alt", 1, UAVK_MENU, 0},         {"pause", 1, UAVK_PAUSE, 0},  {"pageup", 1, UAVK_PRIOR, 0},
    {"pagedown", 1, UAVK_NEXT, 0},{"end", 1, UAVK_END, 0},         {"home", 1, UAVK_HOME, 0},        {"select", 1, UAVK_SELECT, 0},{"execute", 1, UAVK_EXECUTE, 0},
    {"snapshot", 1, UAVK_SNAPSHOT, 0},{"ins", 1, UAVK_INSERT, 0},  {"del", 1, UAVK_DELETE, 0},       {"help", 1, UAVK_HELP, 0},    {"1", 1, 0x31, 0},
    {"2", 1, 0x32, 0},         {"3", 1, 0x33, 0},              {"4", 1, 0x34, 0},              {"5", 1, 0x35, 0},          {"6", 1, 0x36, 0},
    {"7", 1, 0x37, 0},         {"8", 1, 0x38, 0},              {"9", 1, 0x39, 0},              {"0", 1, 0x30, 0},          {"a", 1, 0x41, 0},
    {"b", 1, 0x42, 0},         {"c", 1, 0x43, 0},              {"d", 1, 0x44, 0},              {"e", 1, 0x45, 0},          {"f", 1, 0x46, 0},
    {"g", 1, 0x47, 0},         {"h", 1, 0x48, 0},              {"i", 1, 0x49, 0},              {"j", 1, 0x4a, 0},          {"k", 1, 0x4b, 0},
    {"l", 1, 0x4c, 0},         {"m", 1, 0x4d, 0},              {"n", 1, 0x4e, 0},              {"o", 1, 0x4f, 0},          {"p", 1, 0x50, 0},
    {"q", 1, 0x51, 0},         {"r", 1, 0x52, 0},              {"s", 1, 0x53, 0},              {"t", 1, 0x54, 0},          {"u", 1, 0x55, 0},
    {"v", 1, 0x56, 0},         {"w", 1, 0x57, 0},              {"x", 1, 0x58, 0},              {"y", 1, 0x59, 0},          {"z", 1, 0x5a, 0},
    {"num0", 1, UAVK_NUMPAD0, 0},{"num1", 1, UAVK_NUMPAD1, 0},     {"num2", 1, UAVK_NUMPAD2, 0},     {"num3", 1, UAVK_NUMPAD3, 0}, {"num4", 1, UAVK_NUMPAD4, 0},
    {"num5", 1, UAVK_NUMPAD5, 0},{"num6", 1, UAVK_NUMPAD6, 0},     {"num7", 1, UAVK_NUMPAD7, 0},     {"num8", 1, UAVK_NUMPAD8, 0}, {"num9", 1, UAVK_NUMPAD9, 0},
    {"nummul", 1, UAVK_MULTIPLY, 0},{"numplus", 1, UAVK_ADD, 0},   {"numdot", 1, UAVK_DECIMAL, 0},   {"numminus", 1, UAVK_SUBTRACT, 0},{"enter", 1, UAVK_SEPARATOR, 0},
    {"numdiv", 1, UAVK_DIVIDE, 0},{"extra1", 1, UAVK_OEM_COMMA, 0},{"extra2", 1, UAVK_OEM_PERIOD, 0},{"extra3", 1, UAVK_OEM_MINUS, 0},{"extra4", 1, UAVK_OEM_102, 0},
    {"extra5", 1, UAVK_OEM_1, 0}, {"extra6", 1, UAVK_OEM_PLUS, 0}, {"extra7", 1, UAVK_OEM_3, 0},     {"extra8", 1, UAVK_OEM_7, 0}, {"extra9", 1, UAVK_OEM_2, 0},
    {"extra10", 1, UAVK_OEM_6, 0},{"extra11", 1, UAVK_OEM_5, 0},   {"extra12", 1, UAVK_OEM_4, 0},    {"extra13", 1, UAVK_OEM_8, 0},{"extra14", 1, UAVK_SCROLL, 0},
    {"extra15", 1, UAVK_NUMLOCK, 0},{"extra16", 1, UAVK_F13, 0},   {"extra17", 1, UAVK_F14, 0},      {"extra18", 1, UAVK_F15, 0},  {"lmb", 1, 0x81, 0},
    {"rmb", 1, 0x82, 0},       {"mmb", 1, 0x83, 0},            {"mousex", 2, 0x84, 0},         {"mousey", 2, 0x85, 0},     {"joyb0", 1, 0x86, 0},
    {"joyb1", 1, 0x87, 0},     {"joyb2", 1, 0x88, 0},          {"joyb3", 1, 0x89, 0},          {"joyb4", 1, 0x8a, 0},      {"joyb5", 1, 0x8b, 0},
    {"joyb6", 1, 0x8c, 0},     {"joyb7", 1, 0x8d, 0},          {"joyx", 2, 0x8e, 0},           {"joyy", 2, 0x8f, 0},       {"joythrottle", 2, 0x90, 0},
    {"joyhatx", 2, 0x91, 0},{"joyhaty", 2, 0x92, 0},{"joyrudder", 2, 0x93, 0}, {NULL, 0, -1, 0}
};

SDLWRAP_mouseState mouseState;
SDLWRAP_keybState keybState;
int SDLtoVK[SDL_NUM_SCANCODES];
uint8_t vk_map[256];

int joydevid = -1;
bool joyEnable = false;
SDL_JoystickGUID wantGuid;

SDL_Joystick *sdljoy = NULL;
SDL_Haptic* sdlHaptic = NULL;

int joyAxisMap[32];
bool joyAxisMapInv[32];
int joyButtonMap[32];
int joyHatMap[4];

uint32_t joyButtonStates;
SDLWRAP_Point joyPov;
SDLWRAP_Point joyXYpos;
SDLWRAP_Point joyZRZpos;

static FF_TankEngine ffTankEngine;
static FF_JetEngine ffJetEngine;
static FF_CopterEngine ffCopterEngine;
static FF_RotationDamper ffRotDamper;
static FF_MiniGun ffMGun;
static FF_MissileFire ffMissFire;
static FF_GrenadeFire ffGrenadeFire;
static FF_BombFire ffBombFire;
static FF_Collision ffCollide;
static FF_Shake ffShake;

void sdlInputResetLog()
{
    FSMgr::FileHandle *v0 = uaOpenFile("env/sdlInputLog.txt", "w");
    if ( v0 )
    {
        v0->printf("YPA SDL Input log\n---------------------\n");
        delete v0;
    }
}

void sdlInputLog(const char *format, ...)
{
    va_list va;
    va_start(va, format);
    FSMgr::FileHandle *f = uaOpenFile("env/sdlInputLog.txt", "a");
    if ( f )
    {
        f->vprintf(format, va);
        delete f;
    }
    va_end(va);
}

SDL_JoystickGUID sdlReadJoyGuid()
{
    SDL_JoystickGUID guid;

    FSMgr::FileHandle *f = uaOpenFile("env/sdlJoy.txt", "r");
    if ( f )
    {
        char buf[64] = "";
        f->gets(buf, 64);
        delete f;

        char *eo = strpbrk(buf, "\r\n");

        if (eo)
            *eo = 0;

        guid = SDL_JoystickGetGUIDFromString(buf);
    }
    else
    {
        for (size_t i = 0; i < sizeof(guid.data); i++)
            guid.data[i] = 0;
    }

    return guid;
}

bool sdlGUIDcmp(SDL_JoystickGUID &gd1, SDL_JoystickGUID &gd2)
{
    for (size_t i = 0; i < sizeof(gd1.data); i++)
    {
        if ( gd1.data[i] != gd2.data[i] )
            return false;
    }

    return true;
}

int sdlJoyAxis(SDL_Joystick* joystick, int axis)
{
    int tmp = SDL_JoystickGetAxis(joystick, joyAxisMap[axis]);

    if ( joyAxisMapInv[axis] )
        tmp = -tmp;

    if (abs(tmp) <= 6553) // 20% deadzone
        return 0;

    tmp /= 109; //-32768/32767  to -300/300

    if (tmp > 300)
        tmp = 300;
    else if (tmp < -300)
        tmp = -300;

    return tmp;
}

void sdlJoyReadMapping(SDL_Joystick* joystick)
{
    const char *ignoreChars = " \t\n\r";

    char guid[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), guid, 64);

    for (size_t i = 0; i < 4; i++)
        joyHatMap[i] = i;

    for (size_t i = 0; i < 32; i++)
    {
        joyAxisMap[i] = i;
        joyAxisMapInv[i] = false;
        joyButtonMap[i] = i;
    }

    FSMgr::FileHandle *f = uaOpenFile("joyconf.txt", "r");

    if ( f )
    {
        char buf[4096];

        while ( f->gets(buf, 4096) )
        {
            int i = 0;
            std::string tmp = "";

            while(buf[i])
            {
                if ( !strchr(ignoreChars, buf[i]) )
                {
                    if (buf[i] == ':')
                        break;
                    else
                        tmp += buf[i];
                }

                i++;
            }

            if (tmp.length() > 0 && !strcasecmp(tmp.c_str(), guid))
            {
                while(buf[i])
                {
                    tmp = "";
                    std::string val = "";

                    bool valread = false;

                    while(buf[i])
                    {
                        if ( !strchr(ignoreChars, buf[i]) )
                        {
                            if (buf[i] == ':')
                            {
                                valread = true;
                            }
                            else
                            {
                                if (valread)
                                    val += tolower(buf[i]);
                                else
                                    tmp += tolower(buf[i]);
                            }

                        }
                        else
                        {
                            if (valread)
                                break;
                        }

                        i++;
                    }

                    if (tmp.length() > 0 && val.length() > 0)
                    {
                        if(tmp[0] == 'a')
                        {
                            int axis = atoi( tmp.c_str() + 1 );
                            int realaxis = abs(  atoi( val.c_str() )  );

                            if ( axis < 32 && axis >= 0 && realaxis < 32)
                            {
                                if ( strpbrk(val.c_str(), "-") )
                                    joyAxisMapInv[ axis ] = true;

                                joyAxisMap[ axis ] = realaxis;
                                sdlInputLog("\tAxis map %c%d -> %d\n", (joyAxisMapInv[ axis ] ? '-' : ' '), realaxis, axis);
                            }

                        }
                        else if (tmp[0] == 'b')
                        {
                            int btn = atoi( tmp.c_str() + 1 );
                            int realbtn = abs(  atoi( val.c_str() )  );

                            if ( btn < 32 && btn >= 0 && realbtn < 32)
                            {
                                joyButtonMap[ btn ] = realbtn;
                                sdlInputLog("\tButton map %d -> %d\n", realbtn, btn);
                            }
                        }
                        else if (tmp[0] == 'h')
                        {
                            int hat = atoi( tmp.c_str() + 1 );
                            int realhat = abs(  atoi( val.c_str() )  );

                            if ( hat < 4 && hat >= 0 && realhat < 4)
                            {
                                joyHatMap[ hat ] = realhat;
                                sdlInputLog("\tHat map %d -> %d\n", realhat, hat);
                            }
                        }
                    }
                }

                break;
            }
        }

        delete f;
    }
}


void vkkeydown(int vk)
{
    if ( vk_map[vk] != 0xFF )
        winp_keys[ vk_map[vk] ].down = 1;

    keybState.keyb_cont = vk;
    keybState.keyb_norm = vk;
}


void keydown(int sdl_vk)
{
    int vk = SDLtoVK[sdl_vk];

    if (vk)
        vkkeydown(vk);
}

void vkkeyup(int vk)
{
    if ( vk_map[vk] != 0xFF )
        winp_keys[ vk_map[vk] ].down = 0;

    if (keybState.keyb_cont == vk)
        keybState.keyb_cont = 0;
}

void keyup(int sdl_vk)
{
    int vk = SDLtoVK[sdl_vk];

    if (vk)
        vkkeyup(vk);
}

int InputWatch(void *, SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        //printf("%d %d %d %d\n", SDL_SCANCODE_SPACE, event->key.keysym.scancode, SDLtoVK[event->key.keysym.scancode], vk_map[ SDLtoVK[event->key.keysym.scancode] ]);
        keydown(event->key.keysym.scancode);
        break;

    case SDL_KEYUP:
        keyup(event->key.keysym.scancode);
        break;

    case SDL_TEXTINPUT:
    {
        const char *chrs = event->text.text;

        while(*chrs)
        {
            if ( keybState.charBuffCnt < KEYBCHARBUFF )
            {
                keybState.charBuff[ keybState.charBuffCnt ] = *chrs;
                keybState.charBuffCnt++;
            }
            else
                break;

            chrs++;
        }
    }
    break;

    case SDL_MOUSEBUTTONDOWN:
        if ( !Gui::Root::Instance.MouseDown(Common::Point(event->button.x, event->button.y), event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                mouseState.l_state = 1;
                mouseState.ld_pos.x = event->button.x;
                mouseState.ld_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.ld_pos);

                mouseState.ld_cnt++;

                vkkeydown(0x81);
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                mouseState.r_state = 1;
                mouseState.rd_pos.x = event->button.x;
                mouseState.rd_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.rd_pos);

                mouseState.rd_cnt++;

                vkkeydown(0x82);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                mouseState.m_state = 1;
                mouseState.md_pos.x = event->button.x;
                mouseState.md_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.md_pos);

                mouseState.md_cnt++;

                vkkeydown(0x83);
            }
        }

        break;

    case SDL_MOUSEBUTTONUP:
        if ( !Gui::Root::Instance.MouseUp(Common::Point(event->button.x, event->button.y), event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                mouseState.l_state = 0;
                mouseState.lu_pos.x = event->button.x;
                mouseState.lu_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.lu_pos);

                mouseState.lu_cnt++;

                vkkeyup(0x81);

                if ((event->button.clicks & 1) == 0)
                    mouseState.dbl_state = 1;
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                mouseState.r_state = 0;
                mouseState.ru_pos.x = event->button.x;
                mouseState.ru_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.ru_pos);

                mouseState.ru_cnt++;

                vkkeyup(0x82);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                mouseState.m_state = 0;
                mouseState.mu_pos.x = event->button.x;
                mouseState.mu_pos.y = event->button.y;

                SDLWRAP_mousePosNorm(mouseState.mu_pos);

                mouseState.mu_cnt++;

                vkkeyup(0x83);
            }
        }

        break;

    case SDL_MOUSEMOTION:
        if ( !Gui::Root::Instance.MouseMove(Common::Point(event->button.x, event->button.y)) )
        {
            mouseState.pos.x = event->motion.x;
            mouseState.pos.y = event->motion.y;
            mouseState.__xrel.x = event->motion.xrel;
            mouseState.__xrel.y = event->motion.yrel;

            SDLWRAP_mousePosNorm(mouseState.pos);
        }
        break;

    default:
        break;
    }

    return 1;
}







size_t NC_STACK_winp::func0(IDVList &stak)
{
    if ( !NC_STACK_iwimp::func0(stak) )
        return 0;

    stack__winp.remapIndex = -1;

    return 1;
}

size_t NC_STACK_winp::func1()
{
    return NC_STACK_iwimp::func1();
}

size_t NC_STACK_winp::func2(IDVList &stak)
{
    return NC_STACK_iwimp::func2(stak);
}

size_t NC_STACK_winp::func3(IDVList &stk)
{
    return NC_STACK_iwimp::func3(stk);
}

void NC_STACK_winp::idev_func64(win_64arg *arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    if ( winp->remapIndex < 0 )
    {
        arg->keyState = 0;
    }
    else
    {
        winp__func67__internal *v4 = &winp_keys[winp->remapIndex];
        switch ( v4->keycode )
        {
        case 0x81:
            arg->keyState = mouseState.l_state;
            break;
        case 0x83:
            arg->keyState = mouseState.m_state;
            break;
        case 0x82:
            arg->keyState = mouseState.r_state;
            break;
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
            if ( !(joyButtonStates & (1 << ((v4->keycode & 0xFF) - 0x86)) ) )
                arg->keyState = 0;
            else
                arg->keyState = 1;
            break;
        default:
            arg->keyState = v4->down;

            break;
        }
    }
}

void NC_STACK_winp::idev_func65(win_64arg *arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    if ( winp->remapIndex < 0 )
    {
        arg->field_8 = 0;
        return;
    }

    int v6 = 1;

    switch ( winp_keys[winp->remapIndex].keycode )
    {
    case 0x84:
    {
        winp->mouseDelta += mouseState.move.x * 8;

        if ( mouseState.move.x )
            v6 = 0;
    }
    break;

    case 0x85:
    {
        winp->mouseDelta += mouseState.move.y * 8;

        if ( mouseState.move.y )
            v6 = 0;
    }
    break;

    case 0x8E:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > joyXYpos.x )
        {
            v12 -= 37;

            if ( v12 < joyXYpos.x )
                v12 = joyXYpos.x;
        }
        else if ( v12 < joyXYpos.x )
        {
            v12 += 37;

            if ( v12 > joyXYpos.x )
                v12 = joyXYpos.x;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    case 0x8F:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > joyXYpos.y )
        {
            v12 -= 37;

            if ( v12 < joyXYpos.y )
                v12 = joyXYpos.y;
        }
        else if ( v12 < joyXYpos.y )
        {
            v12 += 37;

            if ( v12 > joyXYpos.y )
                v12 = joyXYpos.y;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    case 0x90:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > -joyZRZpos.x )
        {
            v12 -= 37;

            if ( v12 < -joyZRZpos.x )
                v12 = -joyZRZpos.x;
        }
        else if ( v12 < -joyZRZpos.x )
        {
            v12 += 37;

            if ( v12 > -joyZRZpos.x )
                v12 = -joyZRZpos.x;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    case 0x93:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > joyZRZpos.y )
        {
            v12 -= 37;

            if ( v12 < joyZRZpos.y )
                v12 = joyZRZpos.y;
        }
        else if ( v12 < joyZRZpos.y )
        {
            v12 += 37;

            if ( v12 > joyZRZpos.y )
                v12 = joyZRZpos.y;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    case 0x91:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > joyPov.x )
        {
            v12 -= 37;

            if ( v12 < joyPov.x )
                v12 = joyPov.x;
        }
        else if ( v12 < joyPov.x )
        {
            v12 += 37;

            if ( v12 > joyPov.x )
                v12 = joyPov.x;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    case 0x92:
    {
        int v12 = winp->mouseDelta;

        if ( v12 > joyPov.y )
        {
            v12 -= 37;

            if ( v12 < joyPov.y )
                v12 = joyPov.y;
        }
        else if ( v12 < joyPov.y )
        {
            v12 += 37;

            if ( v12 > joyPov.y )
                v12 = joyPov.y;
        }
        winp->mouseDelta = v12;
        v6 = 0;
    }
    break;

    default:
        if (winp_keys[ winp->remapIndex ].down )
        {
            v6 = 0;
            winp->mouseDelta += 37;
        }
        break;
    }

    if ( v6 )
        winp->mouseDelta = 8 * winp->mouseDelta / 10;

    if ( winp->mouseDelta > 300 )
        winp->mouseDelta = 300;
    else if ( winp->mouseDelta < -300 )
        winp->mouseDelta = -300;

    arg->field_8 = winp->mouseDelta / 300.0;
}

void NC_STACK_winp::idev_func66(winp_66arg *arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    arg->dword8 = 0;
    arg->downed_key_2 = keybState.keyb_cont;
    arg->downed_key = keybState.keyb_norm;

    keybState.keyb_norm = 0;

    arg->chr = keybState.charBuff[0];

    if ( keybState.charBuffCnt )
    {
        int tmp = keybState.charBuffCnt - 1;
        keybState.charBuffCnt--;

        for (int i = 0; i < tmp; i++)
            keybState.charBuff[i] = keybState.charBuff[i + 1];

        keybState.charBuffCnt = tmp;
    }

    keybState.charBuff[keybState.charBuffCnt] = 0;

    if ( arg->downed_key )
    {
        for (int i = 0; i < 48; i++)
        {
            if (winp->hotKeys[i] == arg->downed_key)
            {
                arg->dword8 = i | 0x80;
                break;
            }
        }
    }
}

int NC_STACK_winp::idev_func67(const char **arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    for(int i = 0; i < 128; i++)
    {
        if ( !strcasecmp(winp_keys[i].keyname, *arg) )
        {
            winp->remapIndex = i;
            return winp_keys[i].keytype;
        }
    }
    return 0;
}

int NC_STACK_winp::idev_func68(winp_68arg *arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    if ( arg->id < 48 )
    {
        for ( int i = 0; i < 128; i++ )
        {
            if ( !StriCmp(winp_keys[i].keyname, arg->keyname) && winp_keys[i].keytype == 1 )
            {
                winp->hotKeys[ arg->id ] = winp_keys[i].keycode;
                return 3;
            }
        }
    }
    return 0;
}

void NC_STACK_winp::idev_func69(int arg)
{
    __NC_STACK_winp *winp = &stack__winp;

    if ( arg == 1 )
        winp->mouseDelta = 0;
}

void NC_STACK_winp::idev_func70(idev_query_arg *arg)
{
    //printf("CHECK ARG TYPE %s\n","winp_func70");

    __NC_STACK_winp *winp = &stack__winp;

    if ( arg->keycode )
    {
        for ( int i = 0; i < 48; i++ )
        {
            if ( winp->hotKeys[i] == arg->keycode )
            {
                arg->hotkey = i | 0x80;
                break;
            }
        }
    }
    else if ( arg->hotkey )
    {
        int tmp = arg->hotkey & 0x7F;
        if (tmp < 48)
            arg->keycode = winp->hotKeys[tmp];
    }
}

void NC_STACK_winp::FFDOTankEngine(int state, float p1, float p2)
{
    if ( ffTankEngine.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffTankEngine.Stop();
            ffJetEngine.Stop();
            ffCopterEngine.Stop();

            ffTankEngine.Run();

            float a2 = p1 * 16383.0; // 5000
            float a3 = 1000.0 / (p2 * 13.0 + 5.0);
            ffTankEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            ffTankEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 19660.0; // 6000
            float v6 = 1000.0 / (p2 * 13.0 + 5.0);
            ffTankEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDOJetEngine(int state, float p1, float p2)
{
    if ( ffJetEngine.OK() )
    {
        if ( state == FF_STATE_START)
        {
            ffTankEngine.Stop();
            ffJetEngine.Stop();
            ffCopterEngine.Stop();

            ffJetEngine.Run();

            float a2 = p1 * 19660.0; // 6000
            float a3 = 1000.0 / (p2 * 12.0 + 14.0);
            ffJetEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            ffJetEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 19660.0; // 6000
            float v6 = 1000.0 / (p2 * 12.0 + 14.0);
            ffJetEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDOHeliEngine(int state, float p1, float p2)
{
    if ( ffCopterEngine.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffTankEngine.Stop();
            ffJetEngine.Stop();
            ffCopterEngine.Stop();

            ffCopterEngine.Run();

            float a2 = p1 * 26213.6; //8000.0;
            float a3 = 1000.0 / (p2 * 12.0 + 6.0);
            ffCopterEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            ffCopterEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 26213.6; //8000.0;
            float v6 = 1000.0 / (p2 * 12.0 + 6.0);
            ffCopterEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDORotDamper(int state, float p1)
{
    if ( ffRotDamper.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffRotDamper.Stop();

            ffRotDamper.Update(p1 * 32767.0);

            ffRotDamper.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffRotDamper.Stop();
        }
    }
}

void NC_STACK_winp::FFDOMiniGun(int state)
{
    if ( ffMGun.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffMGun.Stop();

            ffMGun.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffMGun.Stop();
        }
    }
}

void NC_STACK_winp::FFDOMissileFire(int state)
{
    if ( ffMissFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffMissFire.Stop();
            ffGrenadeFire.Stop();
            ffBombFire.Stop();

            ffMissFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffMissFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOGrenadeFire(int state)
{
    if ( ffGrenadeFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffMissFire.Stop();
            ffGrenadeFire.Stop();
            ffBombFire.Stop();

            ffGrenadeFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffGrenadeFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOBombFire(int state)
{
    if ( ffBombFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffMissFire.Stop();
            ffGrenadeFire.Stop();
            ffBombFire.Stop();

            ffBombFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffBombFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOCollision(int state, float a2, float a3, float a4)
{
    if ( ffCollide.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffCollide.Stop();

            ffCollide.Update(a2, a3, a4);

            ffCollide.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffCollide.Stop();
        }
    }
}

void NC_STACK_winp::FFDOShake(int state, float a2, float a3, float a4, float a5)
{
    if ( ffShake.OK() )
    {
        if ( state == FF_STATE_START )
        {
            ffShake.Stop();

            ffShake.Update(a2, a3, a4, a5);

            ffShake.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            ffShake.Stop();
        }
    }
}

void NC_STACK_winp::FFstopAll()
{
    ffTankEngine.Stop();
    ffJetEngine.Stop();
    ffCopterEngine.Stop();
    ffRotDamper.Stop();
    ffMGun.Stop();
    ffMissFire.Stop();
    ffGrenadeFire.Stop();
    ffBombFire.Stop();
    ffCollide.Stop();
    ffShake.Stop();
}


void NC_STACK_winp::idev_func71(winp_71arg *arg)
{
    switch ( arg->effID )
    {
    case FF_TYPE_TANKENGINE:
        FFDOTankEngine(arg->state, arg->p1, arg->p2);
        break;
    case FF_TYPE_JETENGINE:
        FFDOJetEngine(arg->state, arg->p1, arg->p2);
        break;
    case FF_TYPE_HELIENGINE:
        FFDOHeliEngine(arg->state, arg->p1, arg->p2);
        break;
    case FF_TYPE_ROTDAMPER:
        FFDORotDamper(arg->state, arg->p1);
        break;
    case FF_TYPE_MINIGUN:
        FFDOMiniGun(arg->state);
        break;
    case FF_TYPE_MISSILEFIRE:
        FFDOMissileFire(arg->state);
        break;
    case FF_TYPE_GRENADEFIRE:
        FFDOGrenadeFire(arg->state);
        break;
    case FF_TYPE_BOMBFIRE:
        FFDOBombFire(arg->state);
        break;
    case FF_TYPE_COLLISION:
        FFDOCollision(arg->state, arg->p1, arg->p3, arg->p4);
        break;
    case FF_TYPE_SHAKE:
        FFDOShake(arg->state, arg->p1, arg->p2, arg->p3, arg->p4);
        break;
    case FF_TYPE_ALL:
        FFstopAll();
        break;
    default:
        return;
    }
}

void winp_func131__sub1()
{
    if ( joyEnable )
    {
        uint32_t prevBtnState = joyButtonStates;
        joyButtonStates = 0;
        memset(&joyXYpos, 0, sizeof(joyXYpos));
        memset(&joyZRZpos, 0, sizeof(joyZRZpos));

        joyXYpos.x = sdlJoyAxis(sdljoy, 0);
        joyXYpos.y = sdlJoyAxis(sdljoy, 1);

        int joyaxs = SDL_JoystickNumAxes(sdljoy);

        if ( joyaxs > 2 )
            joyZRZpos.x = sdlJoyAxis(sdljoy, 2);
        else
            joyZRZpos.x = -300;

        if ( joyaxs > 3 )
            joyZRZpos.y = sdlJoyAxis(sdljoy, 3);
        else
            joyZRZpos.y = 0;

        for (int i = 0; i < 8; i++ )
        {
            if ( SDL_JoystickGetButton(sdljoy, joyButtonMap[i]) )
            {
                joyButtonStates |= 1 << i;

                if ( !((1 << i) & prevBtnState) )
                    vkkeydown(i + 0x86);
            }
            else if ( (1 << i) & prevBtnState )
            {
                vkkeyup(i + 0x86);
            }
        }

        if ( SDL_JoystickNumHats(sdljoy) > 0)
        {
            switch( SDL_JoystickGetHat(sdljoy, joyHatMap[0]) )
            {
            case SDL_HAT_UP:
                joyPov.x = 0;
                joyPov.y = 300;
                break;

            case SDL_HAT_RIGHTUP:
                joyPov.x = 212;
                joyPov.y = 212;
                break;

            case SDL_HAT_RIGHT:
                joyPov.x = 300;
                joyPov.y = 0;
                break;

            case SDL_HAT_RIGHTDOWN:
                joyPov.x = 212;
                joyPov.y = -212;
                break;

            case SDL_HAT_DOWN:
                joyPov.x = 0;
                joyPov.y = -300;
                break;

            case SDL_HAT_LEFTDOWN:
                joyPov.x = -212;
                joyPov.y = -212;
                break;

            case SDL_HAT_LEFT:
                joyPov.x = -300;
                joyPov.y = 0;
                break;

            case SDL_HAT_LEFTUP:
                joyPov.x = -212;
                joyPov.y = 212;
                break;

            case SDL_HAT_CENTERED:
            default:
                joyPov.x = 0;
                joyPov.y = 0;
                break;
            }
        }
        else
        {
            joyPov.x = 0;
            joyPov.y = 0;
        }
    }
}

void NC_STACK_winp::CheckClick(ClickBoxInf *arg)
{
    arg->flag = 0;

    winp_func131__sub1();

    arg->move.screenPos.x = mouseState.pos.x;
    arg->move.screenPos.y = mouseState.pos.y;

    mouseState.move.x = mouseState.__xrel.x;
    mouseState.move.y = mouseState.__xrel.y;
    mouseState.__xrel.x = 0;
    mouseState.__xrel.y = 0;

    if ( mouseState.l_state )
        arg->flag |= ClickBoxInf::FLAG_LM_HOLD;
    if ( mouseState.m_state )
        arg->flag |= ClickBoxInf::FLAG_MM_HOLD;
    if ( mouseState.r_state )
        arg->flag |= ClickBoxInf::FLAG_RM_HOLD;
    if ( mouseState.dbl_state )
        arg->flag |= ClickBoxInf::FLAG_DBL_CLICK;

    if ( mouseState.ld_cnt > 0 )
    {
        arg->ldw_pos.screenPos.x = mouseState.ld_pos.x;
        arg->ldw_pos.screenPos.y = mouseState.ld_pos.y;
        arg->flag |= ClickBoxInf::FLAG_LM_DOWN;
    }
    if ( mouseState.lu_cnt > 0 )
    {
        arg->lup_pos.screenPos.x = mouseState.lu_pos.x;
        arg->lup_pos.screenPos.y = mouseState.lu_pos.y;
        arg->flag |= ClickBoxInf::FLAG_LM_UP;
    }
    if ( mouseState.rd_cnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_RM_DOWN;
    }
    if ( mouseState.ru_cnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_RM_UP;
    }
    if ( mouseState.md_cnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_MM_DOWN;
    }
    if ( mouseState.mu_cnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_MM_UP;
    }

    mouseState.dbl_state = 0;
    mouseState.ld_cnt = 0;
    mouseState.lu_cnt = 0;
    mouseState.rd_cnt = 0;
    mouseState.ru_cnt = 0;
    mouseState.md_cnt = 0;
    mouseState.mu_cnt = 0;

    NC_STACK_iwimp::CheckClick(arg);
}

void NC_STACK_winp::initfirst()
{
    memset(SDLtoVK, 0, sizeof(SDLtoVK));

    SDLtoVK[SDL_SCANCODE_BACKSPACE]   = UAVK_BACK;
    SDLtoVK[SDL_SCANCODE_TAB]         = UAVK_TAB;
    SDLtoVK[SDL_SCANCODE_CLEAR]       = UAVK_CLEAR;
    SDLtoVK[SDL_SCANCODE_RETURN]      = UAVK_RETURN;
    SDLtoVK[SDL_SCANCODE_MENU]        = UAVK_MENU;
    SDLtoVK[SDL_SCANCODE_CAPSLOCK]    = UAVK_CAPITAL;
    SDLtoVK[SDL_SCANCODE_ESCAPE]      = UAVK_ESCAPE;
    SDLtoVK[SDL_SCANCODE_SPACE]       = UAVK_SPACE;
    SDLtoVK[SDL_SCANCODE_PAGEUP]      = UAVK_PRIOR;
    SDLtoVK[SDL_SCANCODE_PAGEDOWN]    = UAVK_NEXT;
    SDLtoVK[SDL_SCANCODE_END]         = UAVK_END;
    SDLtoVK[SDL_SCANCODE_HOME]        = UAVK_HOME;
    SDLtoVK[SDL_SCANCODE_LEFT]        = UAVK_LEFT;
    SDLtoVK[SDL_SCANCODE_UP]          = UAVK_UP;
    SDLtoVK[SDL_SCANCODE_RIGHT]       = UAVK_RIGHT;
    SDLtoVK[SDL_SCANCODE_DOWN]        = UAVK_DOWN;
    SDLtoVK[SDL_SCANCODE_PRINTSCREEN]       = UAVK_PRINT;
    SDLtoVK[SDL_SCANCODE_INSERT]      = UAVK_INSERT;
    SDLtoVK[SDL_SCANCODE_DELETE]      = UAVK_DELETE;
    SDLtoVK[SDL_SCANCODE_HELP]        = UAVK_HELP;

    for (int i=0; i< 9; i++)
        SDLtoVK[SDL_SCANCODE_1 + i]   = UAVK_0 + 1 + i;

    SDLtoVK[SDL_SCANCODE_0]   = UAVK_0;

    for (int i=0; i<= 25; i++)
        SDLtoVK[SDL_SCANCODE_A + i]   = UAVK_A + i;

    SDLtoVK[SDL_SCANCODE_KP_0]         = UAVK_NUMPAD0;
    SDLtoVK[SDL_SCANCODE_KP_1]         = UAVK_NUMPAD1;
    SDLtoVK[SDL_SCANCODE_KP_2]         = UAVK_NUMPAD2;
    SDLtoVK[SDL_SCANCODE_KP_3]         = UAVK_NUMPAD3;
    SDLtoVK[SDL_SCANCODE_KP_4]         = UAVK_NUMPAD4;
    SDLtoVK[SDL_SCANCODE_KP_5]         = UAVK_NUMPAD5;
    SDLtoVK[SDL_SCANCODE_KP_6]         = UAVK_NUMPAD6;
    SDLtoVK[SDL_SCANCODE_KP_7]         = UAVK_NUMPAD7;
    SDLtoVK[SDL_SCANCODE_KP_8]         = UAVK_NUMPAD8;
    SDLtoVK[SDL_SCANCODE_KP_9]         = UAVK_NUMPAD9;
    SDLtoVK[SDL_SCANCODE_KP_MULTIPLY] = UAVK_MULTIPLY;
    SDLtoVK[SDL_SCANCODE_KP_PLUS]     = UAVK_ADD;
    SDLtoVK[SDL_SCANCODE_KP_MINUS]    = UAVK_SUBTRACT;
    SDLtoVK[SDL_SCANCODE_KP_PERIOD]   = UAVK_DECIMAL;
    SDLtoVK[SDL_SCANCODE_KP_DIVIDE]   = UAVK_DIVIDE;

    for (int i=0; i< 15; i++)
        SDLtoVK[SDL_SCANCODE_F1 + i]  = UAVK_F1+i;

    SDLtoVK[SDL_SCANCODE_NUMLOCKCLEAR]     = UAVK_NUMLOCK;
    SDLtoVK[SDL_SCANCODE_SCROLLLOCK]   = UAVK_SCROLL;
    SDLtoVK[SDL_SCANCODE_LSHIFT]      = UAVK_SHIFT;
    SDLtoVK[SDL_SCANCODE_RSHIFT]      = UAVK_SHIFT;
    SDLtoVK[SDL_SCANCODE_LCTRL]       = UAVK_CONTROL;
    SDLtoVK[SDL_SCANCODE_RCTRL]       = UAVK_CONTROL;
    SDLtoVK[SDL_SCANCODE_MENU]        = UAVK_RMENU;
    SDLtoVK[SDL_SCANCODE_LEFTBRACKET] = UAVK_OEM_4;
    SDLtoVK[SDL_SCANCODE_RIGHTBRACKET] = UAVK_OEM_6;
    SDLtoVK[SDL_SCANCODE_SEMICOLON]   = UAVK_OEM_1;
    SDLtoVK[SDL_SCANCODE_BACKSLASH]   = UAVK_OEM_5;
    SDLtoVK[SDL_SCANCODE_APOSTROPHE]   = UAVK_OEM_7;
    SDLtoVK[SDL_SCANCODE_SLASH]   = UAVK_OEM_2;
    SDLtoVK[SDL_SCANCODE_BACKSLASH]   = UAVK_OEM_3;
    SDLtoVK[SDL_SCANCODE_COMMA]   = UAVK_OEM_COMMA;
    SDLtoVK[SDL_SCANCODE_PERIOD]  = UAVK_OEM_PERIOD;
    SDLtoVK[SDL_SCANCODE_MINUS]  = UAVK_OEM_MINUS;
    SDLtoVK[SDL_SCANCODE_EQUALS]   = UAVK_OEM_PLUS;

    memset(vk_map, 0xFF, sizeof(vk_map));
    for (int i = 0; i < 256; i++)
    {
        int j = 0;
        while (winp_keys[j].keytype)
        {
            if (i == winp_keys[j].keycode && vk_map[i] == 0xFF)
            {
                vk_map[i] = j;
                break;
            }

            j++;
        }
    }

    //Joy staff

    joyEnable = false;

    sdlInputResetLog();
    wantGuid = sdlReadJoyGuid();

    int numJoy = SDL_NumJoysticks();

    if (numJoy)
    {
        for (int i = 0; i < numJoy; i++)
        {
            SDL_JoystickGUID jGUID = SDL_JoystickGetDeviceGUID(i);

            char guidBuff[64];
            SDL_JoystickGetGUIDString(jGUID, guidBuff, sizeof(guidBuff));

            sdlInputLog("Found joystick #%d: %s\n\tGUID:%s\n", i, SDL_JoystickNameForIndex(i), guidBuff);

            if (!sdljoy && sdlGUIDcmp(jGUID, wantGuid) )
            {
                sdljoy = SDL_JoystickOpen(i);
                joydevid = i;
            }
        }

        if ( !sdljoy )
            sdljoy = SDL_JoystickOpen(0);

        sdlHaptic = SDL_HapticOpenFromJoystick(sdljoy);
    }
    else
    {
        sdlInputLog("No joysticks found\n");
    }

    if (sdljoy)
    {
        joyEnable = true;
        sdlInputLog("\nSelected joystick: %s\n", SDL_JoystickName(sdljoy));
        sdlInputLog("\tAxes:%d\tHats:%d\tButtons:%d\tBalls:%d\n", SDL_JoystickNumAxes(sdljoy), SDL_JoystickNumHats(sdljoy), SDL_JoystickNumButtons(sdljoy), SDL_JoystickNumBalls(sdljoy));
        sdlJoyReadMapping(sdljoy);
        sdlInputLog("\n");
    }


    memset(&keybState, 0, sizeof(keybState));
    memset(&mouseState, 0, sizeof(mouseState));

    joyButtonStates = 0;

    SDLWRAP_addHandler(InputWatch);

    keybState.charBuffCnt = 0;

    if (sdlHaptic)
    {
        ffTankEngine.Bind(sdlHaptic);
        ffJetEngine.Bind(sdlHaptic);
        ffCopterEngine.Bind(sdlHaptic);
        ffRotDamper.Bind(sdlHaptic);
        ffMGun.Bind(sdlHaptic);
        ffMissFire.Bind(sdlHaptic);
        ffGrenadeFire.Bind(sdlHaptic);
        ffBombFire.Bind(sdlHaptic);
        ffCollide.Bind(sdlHaptic);
        ffShake.Bind(sdlHaptic);
    }
}

size_t NC_STACK_winp::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        func2( *(IDVList *)data );
        return 1;
    case 3:
        func3( *(IDVList *)data );
        return 1;
    case 64:
        idev_func64( (win_64arg *)data );
        return 1;
    case 65:
        idev_func65( (win_64arg *)data );
        return 1;
    case 66:
        idev_func66( (winp_66arg *)data );
        return 1;
    case 67:
        return (size_t)idev_func67( (const char **)data );
    case 68:
        return (size_t)idev_func68( (winp_68arg *)data );
    case 69:
        idev_func69( (int)(size_t)data );
        return 1;
    case 70:
        idev_func70( (idev_query_arg *)data );
        return 1;
    case 71:
        idev_func71( (winp_71arg *)data );
        return 1;
    case 131:
        CheckClick( (ClickBoxInf *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_iwimp::compatcall(method_id, data);
}
