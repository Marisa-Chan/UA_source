#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "engine_input.h"
#include "winp.h"
#include "utils.h"

#define DIRECTINPUT_VERSION 0x0500

#include <dinput.h>

struct winp_effects
{
    IDirectInputEffect *effects[11];
    DIPERIODIC eff0_DIPERIODIC;
    DIPERIODIC eff1_DIPERIODIC;
    DIPERIODIC eff2_DIPERIODIC;
    DICONDITION eff3_DICONDITION[2];
    DIPERIODIC eff4_DIPERIODIC;
    DIPERIODIC eff5_DIPERIODIC;
    DIRAMPFORCE eff6_DIRAMPFORCE;
    DIRAMPFORCE eff7_DIRAMPFORCE;
    DIRAMPFORCE eff8_DIRAMPFORCE;
    DIPERIODIC eff9_DIPERIODIC;
    DIPERIODIC eff10_DIPERIODIC;
};

struct winp_loc
{
    HINSTANCE hinstance;
    HWND hwnd;
    LPDIRECTINPUT idinput;
    LPDIRECTINPUTDEVICE mouseDev;
    LPDIRECTINPUTDEVICE2 dinputDev2;
    LPDIRECTINPUTDEVICE2 dinputDev2_2;
    DIDEVCAPS dinput2Caps;
    int field_30;
    int field_34;
    int field_38;
    int field_3C;
    int field_40;
    winp_effects effects;
//	int field_128;
//	int field_12C;
//	int field_130;
//	int field_134;
//	int field_138;
//	int field_13C;
//	int field_140;
//	int field_144;
//	int field_148;
//	int field_14C;
//	int field_150;
//	int field_154;
//	int field_158;
//	int field_15C;
//	int field_160;
//	int field_164;
//	int field_168;
//	int field_16C;
//	int field_170;
//	int field_174;
//	int field_178;
//	int field_17C;
//	int field_180;
//	int field_184;
//	int field_188;
//	int field_18C;
//	int field_190;
//	int field_194;
//	int field_198;
//	int field_19C;
//	int field_1A0;
//	int field_1A4;
//	int field_1A8;
//	int field_1AC;
//	int field_1B0;
//	int field_1B4;
//	int field_1B8;
//	int field_1BC;
//	int field_1C0;
//	int field_1C4;
//	int field_1C8;
//	int field_1CC;
//	int field_1D0;
//	int field_1D4;
//	int field_1D8;
//	int field_1DC;
//	int field_1E0;
//	int field_1E4;
//	int field_1E8;
//	int field_1EC;
//	int field_1F0;
//	int field_1F4;
//	int field_1F8;
//	int field_1FC;
//	int field_200;
//	int field_204;
//	int field_208;
//	int field_20C;
//	int field_210;
//	int field_214;
//	int field_218;
//	int field_21C;
//	int field_220;
//	int field_224;
};


winp__func67__internal winp_keys[129] = {{"nop", 1, 0xFF, 0},       {"esc", 1, VK_ESCAPE, 0},       {"space", 1, VK_SPACE, 0},      {"up", 1, VK_UP, 0},        {"down", 1, VK_DOWN, 0},
    {"left", 1, VK_LEFT, 0},   {"right", 1, VK_RIGHT, 0},      {"f1", 1, VK_F1, 0},            {"f2", 1, VK_F2, 0},        {"f3", 1, VK_F3, 0},
    {"f4", 1, VK_F4, 0},       {"f5", 1, VK_F5, 0},            {"f6", 1, VK_F6, 0},            {"f7", 1, VK_F7, 0},        {"f8", 1, VK_F8, 0},
    {"f9", 1, VK_F9, 0},       {"f10", 1, VK_F10, 0},          {"f11", 1, VK_F11, 0},          {"f12", 1, VK_F12, 0},      {"bs", 1, VK_BACK, 0},
    {"tab", 1, VK_TAB, 0},     {"clear", 1, VK_CLEAR, 0},      {"return", 1, VK_RETURN, 0},    {"ctrl", 1, VK_CONTROL, 0}, {"rshift", 1, VK_SHIFT, 0},
    {"lshift", 1, VK_SHIFT, 0},{"shift", 1, VK_SHIFT, 0},      {"alt", 1, VK_MENU, 0},         {"pause", 1, VK_PAUSE, 0},  {"pageup", 1, VK_PRIOR, 0},
    {"pagedown", 1, VK_NEXT, 0},{"end", 1, VK_END, 0},         {"home", 1, VK_HOME, 0},        {"select", 1, VK_SELECT, 0},{"execute", 1, VK_EXECUTE, 0},
    {"snapshot", 1, VK_SNAPSHOT, 0},{"ins", 1, VK_INSERT, 0},  {"del", 1, VK_DELETE, 0},       {"help", 1, VK_HELP, 0},    {"1", 1, 0x31, 0},
    {"2", 1, 0x32, 0},         {"3", 1, 0x33, 0},              {"4", 1, 0x34, 0},              {"5", 1, 0x35, 0},          {"6", 1, 0x36, 0},
    {"7", 1, 0x37, 0},         {"8", 1, 0x38, 0},              {"9", 1, 0x39, 0},              {"0", 1, 0x30, 0},          {"a", 1, 0x41, 0},
    {"b", 1, 0x42, 0},         {"c", 1, 0x43, 0},              {"d", 1, 0x44, 0},              {"e", 1, 0x45, 0},          {"f", 1, 0x46, 0},
    {"g", 1, 0x47, 0},         {"h", 1, 0x48, 0},              {"i", 1, 0x49, 0},              {"j", 1, 0x4a, 0},          {"k", 1, 0x4b, 0},
    {"l", 1, 0x4c, 0},         {"m", 1, 0x4d, 0},              {"n", 1, 0x4e, 0},              {"o", 1, 0x4f, 0},          {"p", 1, 0x50, 0},
    {"q", 1, 0x51, 0},         {"r", 1, 0x52, 0},              {"s", 1, 0x53, 0},              {"t", 1, 0x54, 0},          {"u", 1, 0x55, 0},
    {"v", 1, 0x56, 0},         {"w", 1, 0x57, 0},              {"x", 1, 0x58, 0},              {"y", 1, 0x59, 0},          {"z", 1, 0x5a, 0},
    {"num0", 1, VK_NUMPAD0, 0},{"num1", 1, VK_NUMPAD1, 0},     {"num2", 1, VK_NUMPAD2, 0},     {"num3", 1, VK_NUMPAD3, 0}, {"num4", 1, VK_NUMPAD4, 0},
    {"num5", 1, VK_NUMPAD5, 0},{"num6", 1, VK_NUMPAD6, 0},     {"num7", 1, VK_NUMPAD7, 0},     {"num8", 1, VK_NUMPAD8, 0}, {"num9", 1, VK_NUMPAD9, 0},
    {"nummul", 1, VK_MULTIPLY, 0},{"numplus", 1, VK_ADD, 0},   {"numdot", 1, VK_DECIMAL, 0},   {"numminus", 1, VK_SUBTRACT, 0},{"enter", 1, VK_SEPARATOR, 0},
    {"numdiv", 1, VK_DIVIDE, 0},{"extra1", 1, VK_OEM_COMMA, 0},{"extra2", 1, VK_OEM_PERIOD, 0},{"extra3", 1, VK_OEM_MINUS, 0},{"extra4", 1, VK_OEM_102, 0},
    {"extra5", 1, VK_OEM_1, 0}, {"extra6", 1, VK_OEM_PLUS, 0}, {"extra7", 1, VK_OEM_3, 0},     {"extra8", 1, VK_OEM_7, 0}, {"extra9", 1, VK_OEM_2, 0},
    {"extra10", 1, VK_OEM_6, 0},{"extra11", 1, VK_OEM_5, 0},   {"extra12", 1, VK_OEM_4, 0},    {"extra13", 1, VK_OEM_8, 0},{"extra14", 1, VK_SCROLL, 0},
    {"extra15", 1, VK_NUMLOCK, 0},{"extra16", 1, VK_F13, 0},   {"extra17", 1, VK_F14, 0},      {"extra18", 1, VK_F15, 0},  {"lmb", 1, VK_F18, 0},
    {"rmb", 1, VK_F19, 0},     {"mmb", 1, VK_F20, 0},          {"mousex", 2, VK_F21, 0},       {"mousey", 2, VK_F22, 0},   {"joyb0", 1, 0x86, 0},
    {"joyb1", 1, 0x87, 0},     {"joyb2", 1, 0x88, 0},          {"joyb3", 1, 0x89, 0},          {"joyb4", 1, 0x8a, 0},      {"joyb5", 1, 0x8b, 0},
    {"joyb6", 1, 0x8c, 0},     {"joyb7", 1, 0x8d, 0},          {"joyx", 2, 0x8e, 0},           {"joyy", 2, 0x8f, 0},       {"joythrottle", 2, VK_NUMLOCK, 0},
    {"joyhatx", 2, VK_SCROLL, 0},{"joyhaty", 2, VK_OEM_FJ_JISHO, 0},{"joyrudder", 2, VK_OEM_FJ_MASSHOU, 0}, {NULL, 0, -1, 0}
};

WNDPROC oldWndProc;

winp_loc winp_local;
BYTE vk_map[256];

int winp_max_x, winp_max_y;
int downed_key_2, downed_key;

DWORD joyState_buttonsState;

int wmcharBuf[8];
int wmcharBuf_cnt, wmcharBuf_maxCnt;

int winp_lbut, winp_rbut, winp_mbut;
int winp_ldbl;
int winp_lbut_dwn_cnt, winp_lbut_up_cnt;
int winp_rbut_up_cnt, winp_rbut_dwn_cnt;
int winp_mbut_up_cnt, winp_mbut_dwn_cnt;

shortPoint mouseState_pos;
shortPoint winp_move;
shortPoint winp_ldw_pos;
shortPoint winp_rdw_pos;
shortPoint winp_mdwn_pos;
shortPoint winp_lup_pos;
shortPoint winp_rup_pos;
shortPoint winp_mup_pos;

shortPoint winp_pov;
shortPoint joyState_pos;
shortPoint joyState_ZRzpos;




void sb_0x43d976__sub0()
{
    if ( winp_local.effects.effects[0] )
    {
        winp_local.effects.effects[0]->Release();
        winp_local.effects.effects[0] = NULL;
    }
}

void sb_0x43d976__sub1()
{
    if ( winp_local.effects.effects[1] )
    {
        winp_local.effects.effects[1]->Release();
        winp_local.effects.effects[1] = NULL;
    }
}

void sb_0x43d976__sub2()
{
    if ( winp_local.effects.effects[2] )
    {
        winp_local.effects.effects[2]->Release();
        winp_local.effects.effects[2] = NULL;
    }
}

void sb_0x43d976__sub3()
{
    if ( winp_local.effects.effects[3] )
    {
        winp_local.effects.effects[3]->Release();
        winp_local.effects.effects[3] = NULL;
    }
}

void sb_0x43d976__sub4()
{
    if ( winp_local.effects.effects[4] )
    {
        winp_local.effects.effects[4]->Release();
        winp_local.effects.effects[4] = NULL;
    }
}

void sb_0x43d976__sub5()
{
    if ( winp_local.effects.effects[5] )
    {
        winp_local.effects.effects[5]->Release();
        winp_local.effects.effects[5] = NULL;
    }
}

void sb_0x43d976__sub6()
{
    if ( winp_local.effects.effects[6] )
    {
        winp_local.effects.effects[6]->Release();
        winp_local.effects.effects[6] = NULL;
    }
}

void sb_0x43d976__sub7()
{
    if ( winp_local.effects.effects[7] )
    {
        winp_local.effects.effects[7]->Release();
        winp_local.effects.effects[7] = NULL;
    }
}

void sb_0x43d976__sub8()
{
    if ( winp_local.effects.effects[8] )
    {
        winp_local.effects.effects[8]->Release();
        winp_local.effects.effects[8] = NULL;
    }
}

void sb_0x43d976__sub9()
{
    if ( winp_local.effects.effects[9] )
    {
        winp_local.effects.effects[9]->Release();
        winp_local.effects.effects[9] = NULL;
    }

    if ( winp_local.effects.effects[10] )
    {
        winp_local.effects.effects[10]->Release();
        winp_local.effects.effects[10] = NULL;
    }
}




void sb_0x43d976()
{
    if ( winp_local.mouseDev )
    {
        winp_local.mouseDev->Unacquire();
        winp_local.mouseDev->Release();
        winp_local.mouseDev = NULL;
    }

    if ( winp_local.dinputDev2 )
    {
        winp_local.dinputDev2->Unacquire();
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
    }

    if ( winp_local.dinputDev2_2 )
    {
        sb_0x43d976__sub0();
        sb_0x43d976__sub1();
        sb_0x43d976__sub2();
        sb_0x43d976__sub3();
        sb_0x43d976__sub4();
        sb_0x43d976__sub5();
        sb_0x43d976__sub6();
        sb_0x43d976__sub7();
        sb_0x43d976__sub8();
        sb_0x43d976__sub9();

        memset(&winp_local.effects, 0, sizeof(winp_effects));

        winp_local.dinputDev2_2->Release();
        winp_local.dinputDev2_2 = NULL;
    }
    if ( winp_local.idinput )
        winp_local.idinput->Release();
}

int class_set_winp__sub0__sub0()
{
    HRESULT hres = winp_local.idinput->CreateDevice(GUID_SysMouse, &winp_local.mouseDev, NULL);
    if ( hres )
    {
        log_dinput_error("CreateDevice()", "DirectInput", hres);
        sb_0x43d976();
        return 0;
    }

    hres = winp_local.mouseDev->SetDataFormat(&c_dfDIMouse);
    if ( hres )
    {
        log_dinput_error("SetDataFormat()", "DirectInputDevice", hres);
        sb_0x43d976();
        return 0;
    }

    hres = winp_local.mouseDev->SetCooperativeLevel(winp_local.hwnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    if ( hres )
    {
        log_dinput_error("SetCooperativeLevel()", "DirectInputDevice", hres);
        sb_0x43d976();
        return 0;
    }

    DIPROPDWORD diprop;
    diprop.diph.dwSize = sizeof(DIPROPDWORD);
    diprop.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diprop.diph.dwObj = 0;
    diprop.diph.dwHow = 0;
    diprop.diph.dwHow = 0;
    diprop.dwData = 16;

    hres = winp_local.mouseDev->SetProperty(DIPROP_BUFFERSIZE, (LPCDIPROPHEADER)&diprop);
    if ( hres )
    {
        log_dinput_error("SetProperty(BUFFERSIZE)", "DirectInputDevice", hres);
        sb_0x43d976();
        return 0;
    }

    diprop.dwData = 0;
    hres = winp_local.mouseDev->SetProperty(DIPROP_AXISMODE, (LPCDIPROPHEADER)&diprop);
    if ( hres )
    {
        log_dinput_error("SetProperty(AXISMODE)", "DirectInputDevice", hres);
        sb_0x43d976();
        return 0;
    }

    if ( ghWnd )
        ShowWindow(ghWnd, SW_SHOW);

    hres = winp_local.mouseDev->Acquire();
    if ( hres )
    {
        log_dinput_error("Acquire(Mouse)", "DirectInputDevice", hres);
        sb_0x43d976();
        return 0;
    }

    return 1;
}


BOOL __stdcall winp_enum(LPCDIDEVICEINSTANCE lpddi, LPVOID )
{
    LPDIRECTINPUTDEVICE tmpDev;

    winp_local.dinputDev2 = NULL;

    HRESULT hres = winp_local.idinput->CreateDevice(lpddi->guidInstance, &tmpDev, NULL);
    if ( hres )
    {
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
        return 1;
    }

    memset(&winp_local.dinput2Caps, 0, sizeof(DIDEVCAPS));

    hres = tmpDev->QueryInterface(IID_IDirectInputDevice2, (LPVOID *)&winp_local.dinputDev2);
    tmpDev->Release();

    if ( hres )
    {
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
        return 1;
    }

    winp_local.dinput2Caps.dwSize = sizeof(DIDEVCAPS);

    hres = winp_local.dinputDev2->GetCapabilities(&winp_local.dinput2Caps);
    if ( hres )
    {
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
        return 1;
    }

    if ( winp_local.dinput2Caps.dwFlags & DIDC_FORCEFEEDBACK )
    {
        hres = winp_local.dinputDev2->QueryInterface(IID_IDirectInputDevice2, (LPVOID *)&winp_local.dinputDev2_2);
        if ( hres )
            winp_local.dinputDev2_2 = NULL;
    }

    hres = winp_local.dinputDev2->SetDataFormat(&c_dfDIJoystick);
    if ( hres )
    {
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
        return 1;
    }

    hres = winp_local.dinputDev2->SetCooperativeLevel(winp_local.hwnd, 5);
    if ( hres )
    {
        winp_local.dinputDev2->Release();
        winp_local.dinputDev2 = NULL;
        return 1;
    }

    DIPROPRANGE diproprange;

    memset(&diproprange, 0, sizeof(DIPROPRANGE));


    diproprange.diph.dwSize = sizeof(DIPROPRANGE);
    diproprange.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    diproprange.diph.dwObj = 0;
    diproprange.diph.dwHow = 1;
    diproprange.lMin = -300;
    diproprange.lMax = 300;

    hres = winp_local.dinputDev2->SetProperty(DIPROP_RANGE, (LPCDIPROPHEADER)&diproprange);

    if ( hres )
        log_dinput_error("SetProperty() 1", "DirectInputDevice", hres);

    diproprange.diph.dwObj = 4;
    hres = winp_local.dinputDev2->SetProperty(DIPROP_RANGE, (LPCDIPROPHEADER)&diproprange);

    if ( hres )
        log_dinput_error("SetProperty() 2", "DirectInputDevice", hres);

    if ( winp_local.dinput2Caps.dwAxes > 2 )
    {
        diproprange.diph.dwObj = 8;
        hres = winp_local.dinputDev2->SetProperty(DIPROP_RANGE, (LPCDIPROPHEADER)&diproprange);

        if ( hres )
            log_dinput_error("SetProperty() 3", "DirectInputDevice", hres);
    }

    if ( winp_local.dinput2Caps.dwAxes > 3 )
    {
        diproprange.diph.dwObj = 20;
        hres = winp_local.dinputDev2->SetProperty(DIPROP_RANGE, (LPCDIPROPHEADER)&diproprange);

        if ( hres )
            log_dinput_error("SetProperty() 4", "DirectInputDevice", hres);
    }

    DIPROPDWORD dipropdword;
    memset(&dipropdword, 0, sizeof(DIPROPDWORD));

    dipropdword.diph.dwSize = sizeof(DIPROPDWORD);
    dipropdword.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipropdword.diph.dwObj = 0;
    dipropdword.diph.dwHow = 1;
    dipropdword.dwData = 2000;

    hres = winp_local.dinputDev2->SetProperty(DIPROP_DEADZONE, (LPCDIPROPHEADER)&dipropdword);
    if ( hres )
        log_dinput_error("SetProperty() 5", "DirectInputDevice", hres);


    dipropdword.diph.dwObj = 4;
    hres = winp_local.dinputDev2->SetProperty(DIPROP_DEADZONE, (LPCDIPROPHEADER)&dipropdword);

    if ( hres )
        log_dinput_error("SetProperty() 6", "DirectInputDevice", hres);


    if ( winp_local.dinput2Caps.dwAxes > 3 )
    {
        dipropdword.diph.dwObj = 20;
        hres = winp_local.dinputDev2->SetProperty(DIPROP_DEADZONE, (LPCDIPROPHEADER)&dipropdword);

        if ( hres )
            log_dinput_error("SetProperty() 7", "DirectInputDevice", hres);
    }

    if ( winp_local.dinput2Caps.dwFlags & DIDC_FORCEFEEDBACK )
    {
        dipropdword.diph.dwSize = sizeof(DIPROPDWORD);
        dipropdword.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipropdword.diph.dwObj = 0;
        dipropdword.diph.dwHow = 0;
        dipropdword.dwData = 0;

        hres = winp_local.dinputDev2->SetProperty(DIPROP_AUTOCENTER, (LPCDIPROPHEADER)&dipropdword);
        if ( hres )
            log_dinput_error("SetProperty() 8", "DirectInputDevice", hres);
    }
    return 0;
}


void createEffect0()
{
    printf("CLEAN ME %s\n","createEffect0");
    DIEFFECT v1; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v2; // [sp+54h] [bp-28h]@1
    LONG directions[2]; // [sp+68h] [bp-14h]@1
    DWORD rgdwAxes[2]; // [sp+70h] [bp-Ch]@1

    v2.dwSize = 20;
    v2.dwAttackTime = 50000;
    v2.dwAttackLevel = 0;
    v2.dwFadeTime = 50000;
    v2.dwFadeLevel = 0;
    rgdwAxes[0] = 0;
    rgdwAxes[1] = 4;
    directions[1] = 0;
    v1.dwSize = 52;
    v1.dwFlags = 34;
    v1.dwSamplePeriod = 10000;
    v1.dwGain = 10000;
    v1.dwTriggerButton = -1;
    v1.dwTriggerRepeatInterval = 0;
    v1.cAxes = 2;
    v1.rgdwAxes = rgdwAxes;
    v1.rglDirection = directions;
    v1.lpEnvelope = &v2;
    winp_local.effects.eff0_DIPERIODIC.dwMagnitude = 3000;
    winp_local.effects.eff0_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff0_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff0_DIPERIODIC.dwPeriod = 200000;
    v1.dwDuration = -1;
    directions[0] = 9000;
    v1.cbTypeSpecificParams = 16;
    v1.lpvTypeSpecificParams = &winp_local.effects.eff0_DIPERIODIC;
    winp_local.dinputDev2_2->CreateEffect(GUID_Sine, &v1, &winp_local.effects.effects[0], 0);
}

void createEffect1()
{
    printf("CLEAN ME %s\n","createEffect1");

    DIEFFECT v1; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v2; // [sp+54h] [bp-28h]@1
    LONG v3[2]; // [sp+68h] [bp-14h]@1
    DWORD v4[2]; // [sp+70h] [bp-Ch]@1

    v2.dwSize = 20;
    v2.dwAttackTime = 50000;
    v2.dwAttackLevel = 0;
    v2.dwFadeTime = 50000;
    v2.dwFadeLevel = 0;
    v4[0] = 0;
    v4[1] = 4;
    v3[1] = 0;
    v1.dwSize = 52;
    v1.dwFlags = 34;
    v1.dwSamplePeriod = 10000;
    v1.dwGain = 10000;
    v1.dwTriggerButton = -1;
    v1.dwTriggerRepeatInterval = 0;
    v1.cAxes = 2;
    v1.rgdwAxes = v4;
    v1.rglDirection = v3;
    v1.lpEnvelope = &v2;
    winp_local.effects.eff1_DIPERIODIC.dwMagnitude = 2300;
    winp_local.effects.eff1_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff1_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff1_DIPERIODIC.dwPeriod = 71500;
    v1.dwDuration = -1;
    v3[0] = 9000;
    v1.cbTypeSpecificParams = 16;
    v1.lpvTypeSpecificParams = &winp_local.effects.eff1_DIPERIODIC;
    winp_local.dinputDev2_2->CreateEffect(GUID_Sine, &v1, &winp_local.effects.effects[1], 0);
}

void createEffect2()
{
    printf("CLEAN ME %s\n","createEffect2");

    DIEFFECT v1; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v2; // [sp+54h] [bp-28h]@1
    LONG v3[2]; // [sp+68h] [bp-14h]@1
    DWORD v4[2]; // [sp+70h] [bp-Ch]@1

    v2.dwSize = 20;
    v2.dwAttackTime = 50000;
    v2.dwAttackLevel = 0;
    v2.dwFadeTime = 50000;
    v2.dwFadeLevel = 0;
    v4[0] = 0;
    v4[1] = 4;
    v3[1] = 0;
    v1.dwSize = 52;
    v1.dwFlags = 34;
    v1.dwSamplePeriod = 10000;
    v1.dwGain = 10000;
    v1.dwTriggerButton = -1;
    v1.dwTriggerRepeatInterval = 0;
    v1.cAxes = 2;
    v1.rgdwAxes = v4;
    v1.rglDirection = v3;
    v1.lpEnvelope = &v2;
    winp_local.effects.eff2_DIPERIODIC.dwMagnitude = 4500;
    winp_local.effects.eff2_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff2_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff2_DIPERIODIC.dwPeriod = 166666;
    v1.dwDuration = -1;
    v3[0] = 0;
    v1.cbTypeSpecificParams = 16;
    v1.lpvTypeSpecificParams = &winp_local.effects.eff2_DIPERIODIC;
    winp_local.dinputDev2_2->CreateEffect(GUID_SawtoothUp, &v1, &winp_local.effects.effects[2], 0);
}

void createEffect3()
{
    printf("CLEAN ME %s\n","createEffect3");

    DIEFFECT v0; // [sp+20h] [bp-5Ch]@1
//  signed int v1; // [sp+54h] [bp-28h]@1
//  int v2; // [sp+58h] [bp-24h]@1
//  signed int v3; // [sp+5Ch] [bp-20h]@1
//  int v4; // [sp+60h] [bp-1Ch]@1
//  signed int v5; // [sp+64h] [bp-18h]@1
    LONG v6[2]; // [sp+68h] [bp-14h]@1
    DWORD v7[2]; // [sp+70h] [bp-Ch]@1
    int msgid; // [sp+78h] [bp-4h]@1

//  v1 = 20;
//  v3 = 50000;
//  v2 = 0;
//  v5 = 50000;
//  v4 = 0;
    v7[0] = 0;
    v7[1] = 4;
    v6[0] = 0;
    v6[1] = 0;
    v0.dwSize = 52;
    v0.dwFlags = 34;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v7;
    v0.rglDirection = v6;
    winp_local.effects.eff3_DICONDITION[0].lOffset = 0;
    winp_local.effects.eff3_DICONDITION[0].lPositiveCoefficient = 10000;
    winp_local.effects.eff3_DICONDITION[0].lNegativeCoefficient = 10000;
    winp_local.effects.eff3_DICONDITION[0].dwPositiveSaturation = 0;
    winp_local.effects.eff3_DICONDITION[0].dwNegativeSaturation = 0;
    winp_local.effects.eff3_DICONDITION[0].lDeadBand = 0;
    winp_local.effects.eff3_DICONDITION[1].lOffset = 0;
    winp_local.effects.eff3_DICONDITION[1].lPositiveCoefficient = 10000;
    winp_local.effects.eff3_DICONDITION[1].lNegativeCoefficient = 10000;
    winp_local.effects.eff3_DICONDITION[1].dwPositiveSaturation = 0;
    winp_local.effects.eff3_DICONDITION[1].dwNegativeSaturation = 0;
    winp_local.effects.eff3_DICONDITION[1].lDeadBand = 0;
    v0.dwDuration = -1;
    v0.lpEnvelope = 0;
    v0.cbTypeSpecificParams = 48;
    v0.lpvTypeSpecificParams = winp_local.effects.eff3_DICONDITION;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_Damper, &v0, &winp_local.effects.effects[3], 0);
    if ( msgid )
        log_dinput_error("DirectInputEffect", "GetParameters()", msgid);
}

void createEffect4()
{
    printf("CLEAN ME %s\n","createEffect4");

    DIEFFECT v1; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v2; // [sp+54h] [bp-28h]@1
    LONG v3[2]; // [sp+68h] [bp-14h]@1
    DWORD v4[2]; // [sp+70h] [bp-Ch]@1

    v2.dwSize = 20;
    v2.dwAttackTime = 50000;
    v2.dwAttackLevel = 0;
    v2.dwFadeTime = 50000;
    v2.dwFadeLevel = 0;
    v4[0] = 0;
    v4[1] = 4;
    v3[1] = 0;
    v1.dwSize = 52;
    v1.dwFlags = 34;
    v1.dwSamplePeriod = 10000;
    v1.dwGain = 10000;
    v1.dwTriggerButton = -1;
    v1.dwTriggerRepeatInterval = 0;
    v1.cAxes = 2;
    v1.rgdwAxes = v4;
    v1.rglDirection = v3;
    v1.lpEnvelope = &v2;
    winp_local.effects.eff4_DIPERIODIC.dwMagnitude = 5000;
    winp_local.effects.eff4_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff4_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff4_DIPERIODIC.dwPeriod = 83333;
    v1.dwDuration = -1;
    v3[0] = 0;
    v1.cbTypeSpecificParams = 16;
    v1.lpvTypeSpecificParams = &winp_local.effects.eff4_DIPERIODIC;
    winp_local.dinputDev2_2->CreateEffect(GUID_Square, &v1, &winp_local.effects.effects[4], 0);
}

void createEffect5()
{
    printf("CLEAN ME %s\n","createEffect5");

    DIEFFECT v0; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v1; // [sp+54h] [bp-28h]@1
    LONG v2[2]; // [sp+68h] [bp-14h]@1
    DWORD v3[2]; // [sp+70h] [bp-Ch]@1
    int msgid; // [sp+78h] [bp-4h]@1

    v1.dwSize = 20;
    v3[0] = 0;
    v3[1] = 4;
    v2[0] = 0;
    v2[1] = 0;
    v0.dwSize = 52;
    v0.dwFlags = 34;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v3;
    v0.rglDirection = v2;
    v0.lpEnvelope = &v1;
    winp_local.effects.eff5_DIPERIODIC.dwMagnitude = 10000;
    winp_local.effects.eff5_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff5_DIPERIODIC.dwPhase = 9000;
    winp_local.effects.eff5_DIPERIODIC.dwPeriod = 1000000;
    v1.dwAttackTime = 0;
    v1.dwAttackLevel = 10000;
    v1.dwFadeTime = 264000;
    v1.dwFadeLevel = 0;
    v0.dwDuration = 600000;
    v0.cbTypeSpecificParams = 16;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff5_DIPERIODIC;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_Sine, &v0, &winp_local.effects.effects[5], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
}

void createEffect6()
{
    printf("CLEAN ME %s\n","createEffect6");

    DIEFFECT v0; // [sp+20h] [bp-5Ch]@1
    DIENVELOPE v1; // [sp+54h] [bp-28h]@1
    LONG v2[2]; // [sp+68h] [bp-14h]@1
    DWORD v3[2]; // [sp+70h] [bp-Ch]@1
    int msgid; // [sp+78h] [bp-4h]@1

    v1.dwSize = 20;
    v3[0] = 0;
    v3[1] = 4;
    v2[0] = 0;
    v2[1] = 0;
    v0.dwSize = 52;
    v0.dwFlags = 34;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v3;
    v0.rglDirection = v2;
    v0.lpEnvelope = &v1;
    winp_local.effects.eff6_DIRAMPFORCE.lStart = 10000;
    winp_local.effects.eff6_DIRAMPFORCE.lEnd = -10000;
    v1.dwAttackTime = 0;
    v1.dwAttackLevel = 10000;
    v1.dwFadeTime = 57000;
    v1.dwFadeLevel = 0;
    v0.dwDuration = 300000;
    v0.cbTypeSpecificParams = 8;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff6_DIRAMPFORCE;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_RampForce, &v0, &winp_local.effects.effects[6], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
}

void createEffect7()
{
    printf("CLEAN ME %s\n","createEffect7");

    DIEFFECT v0; // [sp+20h] [bp-5Ch]@1
//  signed int v1; // [sp+54h] [bp-28h]@1
//  int v2; // [sp+58h] [bp-24h]@1
//  signed int v3; // [sp+5Ch] [bp-20h]@1
//  int v4; // [sp+60h] [bp-1Ch]@1
//  signed int v5; // [sp+64h] [bp-18h]@1
    LONG v6[2]; // [sp+68h] [bp-14h]@1
    DWORD v7[2]; // [sp+70h] [bp-Ch]@1
    int msgid; // [sp+78h] [bp-4h]@1

//  v1 = 20;
//  v3 = 50000;
//  v2 = 0;
//  v5 = 50000;
//  v4 = 0;
    v7[0] = 0;
    v7[1] = 4;
    v6[0] = 0;
    v6[1] = 0;
    v0.dwSize = 52;
    v0.dwFlags = 34;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v7;
    v0.rglDirection = v6;
    winp_local.effects.eff7_DIRAMPFORCE.lStart = 8000;
    winp_local.effects.eff7_DIRAMPFORCE.lEnd = -8000;
    v0.dwDuration = 400000;
    v0.lpEnvelope = 0;
    v0.cbTypeSpecificParams = 8;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff7_DIRAMPFORCE;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_RampForce, &v0, &winp_local.effects.effects[7], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
}

void createEffect8()
{
    printf("CLEAN ME %s\n","createEffect8");

    DIEFFECT v0; // [sp+20h] [bp-5Ch]@1
//  DIENVELOPE v1; // [sp+54h] [bp-28h]@1
    LONG v2[2]; // [sp+68h] [bp-14h]@1
    DWORD v3[2]; // [sp+70h] [bp-Ch]@1
    int msgid; // [sp+78h] [bp-4h]@1

//  v1.dwSize = 20;
//  v1.dwAttackTime = 50000;
//  v1.dwAttackLevel = 0;
//  v1.dwFadeTime = 50000;
//  v1.dwFadeLevel = 0;
    v3[0] = 0;
    v3[1] = 4;
    v2[0] = 0;
    v2[1] = 0;
    v0.dwSize = 52;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v3;
    v0.rglDirection = v2;
    winp_local.effects.eff8_DIRAMPFORCE.lStart = 10000;
    winp_local.effects.eff8_DIRAMPFORCE.lEnd = -10000;
    v0.dwFlags = 18;
    v0.dwDuration = 95000;
    v0.lpEnvelope = 0;
    v0.cbTypeSpecificParams = 8;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff8_DIRAMPFORCE;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_RampForce, &v0, &winp_local.effects.effects[8], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
}

void createEffect9_10()
{
    printf("CLEAN ME %s\n","createEffect9_10");

    DIEFFECT v0; // [sp+40h] [bp-5Ch]@1
    DIENVELOPE v1; // [sp+74h] [bp-28h]@1
    LONG v2[2]; // [sp+88h] [bp-14h]@1
    DWORD v3[2]; // [sp+90h] [bp-Ch]@1
    int msgid; // [sp+98h] [bp-4h]@1

    v1.dwSize = 20;
    v3[0] = 0;
    v3[1] = 4;
    v2[0] = 0;
    v2[1] = 0;
    v0.dwSize = 52;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v3;
    v0.rglDirection = v2;
    v0.lpEnvelope = &v1;
    winp_local.effects.eff9_DIPERIODIC.dwMagnitude = 10000;
    winp_local.effects.eff9_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff9_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff9_DIPERIODIC.dwPeriod = 71428;
    v1.dwAttackTime = 0;
    v1.dwAttackLevel = 10000;
    v1.dwFadeTime = 370000;
    v1.dwFadeLevel = 0;
    v0.dwFlags = 18;
    v0.dwDuration = 1000000;
    v0.cbTypeSpecificParams = 16;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff9_DIPERIODIC;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_Square, &v0, &winp_local.effects.effects[9], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
    v1.dwSize = 20;
    v3[0] = 0;
    v3[1] = 4;
    v2[0] = 0;
    v2[1] = 0;
    v0.dwSize = 52;
    v0.dwSamplePeriod = 10000;
    v0.dwGain = 10000;
    v0.dwTriggerButton = -1;
    v0.dwTriggerRepeatInterval = 0;
    v0.cAxes = 2;
    v0.rgdwAxes = v3;
    v0.rglDirection = v2;
    v0.lpEnvelope = &v1;
    winp_local.effects.eff10_DIPERIODIC.dwMagnitude = 10000;
    winp_local.effects.eff10_DIPERIODIC.lOffset = 0;
    winp_local.effects.eff10_DIPERIODIC.dwPhase = 0;
    winp_local.effects.eff10_DIPERIODIC.dwPeriod = 166666;
    v1.dwAttackTime = 0;
    v1.dwAttackLevel = 10000;
    v1.dwFadeTime = 630000;
    v1.dwFadeLevel = 0;
    v0.dwFlags = 18;
    v0.dwDuration = 1000000;
    v0.cbTypeSpecificParams = 16;
    v0.lpvTypeSpecificParams = &winp_local.effects.eff10_DIPERIODIC;
    msgid = winp_local.dinputDev2_2->CreateEffect(GUID_Sine, &v0, &winp_local.effects.effects[10], 0);
    if ( msgid )
        log_dinput_error("DirectInputDevice2", "CreateEffect()", msgid);
}

int class_set_winp__sub0__sub1()
{
    HRESULT hres = winp_local.idinput->EnumDevices(DIDEVTYPE_JOYSTICK, winp_enum, NULL, DIEDFL_ATTACHEDONLY);
    if ( hres )
    {
        log_dinput_error("EnumDevices()", "DirectInput", hres);
        return 0;
    }

    if ( winp_local.dinputDev2 )
    {
        hres = winp_local.dinputDev2->Acquire();

        if ( hres )
        {
            log_dinput_error("Acquire(Joystick)", "DirectInputDevice", hres);
            sb_0x43d976();
            return 0;
        }

        if ( winp_local.dinputDev2_2 )
        {
            memset(&winp_local.effects, 0, sizeof(winp_effects));
            createEffect0();
            createEffect1();
            createEffect2();
            createEffect3();
            createEffect4();
            createEffect5();
            createEffect6();
            createEffect7();
            createEffect8();
            createEffect9_10();
        }
    }
    return 1;
}




int class_set_winp__sub0()
{
    memset(&winp_local, 0, sizeof(winp_loc));

    if ( ghWnd && ghInstance )
    {
        winp_local.hinstance = ghInstance;
        winp_local.hwnd = ghWnd;

        HRESULT errid = DirectInputCreate(ghInstance, DIRECTINPUT_VERSION, &winp_local.idinput, 0);
        if ( errid )
        {
            log_dinput_error("DirectInputCreate()", "DirectInput", errid);
            sb_0x43d976();
            return 0;
        }
        else if ( class_set_winp__sub0__sub0() )
            return class_set_winp__sub0__sub1() != 0;
        else
            return 0;
    }
    else
        return 0;
}








const NewClassDescr NC_STACK_winp::description("winp.class", &newinstance);

void winp_keyDown(int vk)
{
    if ( vk_map[vk] != 0xFF )
        winp_keys[vk_map[vk]].down = 1;

    downed_key = vk;
    downed_key_2 = vk;
}

void winp_keyUp(int vk)
{
    if ( vk_map[vk] != 0xFF )
        winp_keys[vk_map[vk]].down = 0;

    if ( vk == downed_key_2 )
        downed_key_2 = 0;
}

void winp_mousePos(HWND hwnd, shortPoint *in, shortPoint *out)
{
    RECT Rect;

    GetClientRect(hwnd, &Rect);

    int width = Rect.right - Rect.left;
    int height = Rect.bottom - Rect.top;

    out->x = winp_max_x * (in->x - Rect.left) / width;
    out->y = winp_max_y * (in->y - Rect.top) / height;


    if ( out->x >= winp_max_x )
        out->x = winp_max_x - 1;
    else if ( out->x < 0 )
        out->x = 0;

    if ( out->y >= winp_max_y )
        out->y = winp_max_y - 1;
    else if (out->y < 0)
        out->y = 0;
}


LRESULT __stdcall winp_winProc(HWND hWnd, UINT Msg, int wParam, LPARAM lParam)
{
    shortPoint a2 = lParam;

    switch (Msg)
    {
    case WM_KEYDOWN:
        winp_keyDown(wParam);
        break;

    case WM_KEYUP:
        winp_keyUp(wParam);
        break;

    case WM_CHAR:
        if ( wmcharBuf_cnt < wmcharBuf_maxCnt )
        {
            wmcharBuf[wmcharBuf_cnt] = wParam;
            wmcharBuf_cnt++;
        }
        break;

    case WM_MOUSEMOVE:
        winp_mousePos(hWnd, &a2, &winp_move);
        break;

    case WM_LBUTTONDOWN:
    {
        winp_mousePos(hWnd, &a2, &winp_ldw_pos);
        winp_lbut = 1;

        winp_lbut_dwn_cnt++;
        winp_move = winp_ldw_pos;
        winp_keyDown(VK_F18);
    }
    break;

    case WM_LBUTTONUP:
    {
        winp_mousePos(hWnd, &a2, &winp_lup_pos);
        winp_lbut = 0;

        winp_lbut_up_cnt++;
        winp_move = winp_lup_pos;
        winp_keyUp(VK_F18);
    }
    break;

    case WM_LBUTTONDBLCLK:
        winp_ldbl = 1;
        break;

    case WM_RBUTTONDOWN:
    {
        winp_mousePos(hWnd, &a2, &winp_rdw_pos);
        winp_rbut = 1;

        winp_rbut_dwn_cnt++;
        winp_move = winp_rdw_pos;
        winp_keyDown(VK_F19);
    }
    break;

    case WM_RBUTTONUP:
    {
        winp_mousePos(hWnd, &a2, &winp_rup_pos);
        winp_rbut = 0;

        winp_rbut_up_cnt++;
        winp_move = winp_rup_pos;
        winp_keyUp(VK_F19);
    }
    break;

    case WM_MBUTTONDOWN:
    {
        winp_mousePos(hWnd, &a2, &winp_mdwn_pos);
        winp_mbut = 1;

        winp_mbut_dwn_cnt++;
        winp_move = winp_mdwn_pos;
        winp_keyDown(VK_F20);
    }
    break;

    case WM_MBUTTONUP:
    {
        winp_mousePos(hWnd, &a2, &winp_mup_pos);
        winp_mbut = 0;

        winp_mbut_up_cnt++;
        winp_move = winp_mup_pos;
        winp_keyUp(VK_F20);
    }
    break;

    default:
        break;
    }

    return CallWindowProcA(oldWndProc, hWnd, Msg, wParam, lParam);
}

void sub_43E59A(HWND a1)
{
    oldWndProc = (WNDPROC)SetWindowLong(a1, GWL_WNDPROC, (LONG)winp_winProc);
}

void sub_43E5CB(HWND a1)
{
    if ( oldWndProc )
    {
        SetWindowLongA(a1, GWL_WNDPROC, (LONG)oldWndProc);
        oldWndProc = NULL;
    }
}

size_t NC_STACK_winp::func0(stack_vals *stak)
{
    if ( !NC_STACK_iwimp::func0(stak) )
        return 0;

    __NC_STACK_winp *winp = &this->stack__winp;
    winp->field_4 = -1;
    gfx_window *v8 = (gfx_window *)find_id_in_stack_def_val(0x80002000, 0, stak);
    if ( v8 )
    {
        winp->hWND = v8->hwnd;
        winp_max_y = v8->height;
        winp_max_x = v8->width;
        sub_43E59A(winp->hWND);
    }

    return 1;
}

size_t NC_STACK_winp::func1(stack_vals *stak)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    if ( winp->hWND )
        sub_43E5CB(winp->hWND);

    return NC_STACK_iwimp::func1(stak);
}

size_t NC_STACK_winp::func2(stack_vals *stak)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    gfx_window *v7 = (gfx_window *)find_id_in_stack_def_val(0x80002000, 0, stak);
    if ( v7 )
    {
        if ( winp->hWND )
            sub_43E5CB(winp->hWND);
        winp->hWND = v7->hwnd;
        winp_max_x = v7->width;
        winp_max_y = v7->height;

        sub_43E59A(winp->hWND);
    }
    return NC_STACK_iwimp::func2(stak);
}

size_t NC_STACK_winp::func3(stack_vals *stk)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    stack_vals *v6 = find_id_in_stack2(0x80002000, stk);

    if ( v6 )
        *(HWND *)v6->value = winp->hWND;

    return NC_STACK_iwimp::func3(stk);
}

void NC_STACK_winp::idev_func64(win_64arg *arg)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    if ( winp->field_4 < 0 )
    {
        arg->field_4 = 0;
    }
    else
    {
        winp__func67__internal *v4 = &winp_keys[winp->field_4];
        switch ( v4->keycode )
        {
        case 0x81:
            arg->field_4 = winp_lbut;
            break;
        case 0x83:
            arg->field_4 = winp_mbut;
            break;
        case 0x82:
            arg->field_4 = winp_rbut;
            break;
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
            if ( !(joyState_buttonsState & (1 << ((v4->keycode & 0xFF) - 0x86)) ) )
                arg->field_4 = 0;
            else
                arg->field_4 = 1;
            break;
        default:
            arg->field_4 = v4->down;
            break;
        }
    }
}

void NC_STACK_winp::idev_func65(win_64arg *arg)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    if ( winp->field_4 < 0 )
    {
        arg->field_8 = 0;
        return;
    }

    int v6 = 1;

    switch ( winp_keys[winp->field_4].keycode )
    {
    case 0x84:
    {
        int v9 = mouseState_pos.x - winp->field_8;
        winp->field_c += v9;

        winp->field_8 = mouseState_pos.x;

        if ( v9 )
            v6 = 0;
    }
    break;

    case 0x85:
    {
        int v10 = mouseState_pos.y - winp->field_8;
        winp->field_c += v10;

        winp->field_8 = mouseState_pos.y;

        if ( v10 )
            v6 = 0;
    }
    break;

    case 0x8E:
    {
        int v12 = winp->field_c;

        if ( v12 > joyState_pos.x )
        {
            v12 -= 37;

            if ( v12 < joyState_pos.x )
                v12 = joyState_pos.x;
        }
        else if ( v12 < joyState_pos.x )
        {
            v12 += 37;

            if ( v12 > joyState_pos.x )
                v12 = joyState_pos.x;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    case 0x8F:
    {
        int v12 = winp->field_c;

        if ( v12 > joyState_pos.y )
        {
            v12 -= 37;

            if ( v12 < joyState_pos.y )
                v12 = joyState_pos.y;
        }
        else if ( v12 < joyState_pos.y )
        {
            v12 += 37;

            if ( v12 > joyState_pos.y )
                v12 = joyState_pos.y;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    case 0x90:
    {
        int v12 = winp->field_c;

        if ( v12 > -joyState_ZRzpos.x )
        {
            v12 -= 37;

            if ( v12 < -joyState_ZRzpos.x )
                v12 = -joyState_ZRzpos.x;
        }
        else if ( v12 < -joyState_ZRzpos.x )
        {
            v12 += 37;

            if ( v12 > -joyState_ZRzpos.x )
                v12 = -joyState_ZRzpos.x;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    case 0x93:
    {
        int v12 = winp->field_c;

        if ( v12 > joyState_ZRzpos.y )
        {
            v12 -= 37;

            if ( v12 < joyState_ZRzpos.y )
                v12 = joyState_ZRzpos.y;
        }
        else if ( v12 < joyState_ZRzpos.y )
        {
            v12 += 37;

            if ( v12 > joyState_ZRzpos.y )
                v12 = joyState_ZRzpos.y;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    case 0x91:
    {
        int v12 = winp->field_c;

        if ( v12 > winp_pov.x )
        {
            v12 -= 37;

            if ( v12 < winp_pov.x )
                v12 = winp_pov.x;
        }
        else if ( v12 < winp_pov.x )
        {
            v12 += 37;

            if ( v12 > winp_pov.x )
                v12 = winp_pov.x;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    case 0x92:
    {
        int v12 = winp->field_c;

        if ( v12 > winp_pov.y )
        {
            v12 -= 37;

            if ( v12 < winp_pov.y )
                v12 = winp_pov.y;
        }
        else if ( v12 < winp_pov.y )
        {
            v12 += 37;

            if ( v12 > winp_pov.y )
                v12 = winp_pov.y;
        }
        winp->field_c = v12;
        v6 = 0;
    }
    break;

    default:
        if ( winp_keys[winp->field_4].down )
        {
            v6 = 0;
            winp->field_c += 37;
        }
        break;
    }

    if ( v6 )
        winp->field_c = 8 * winp->field_c / 10;

    if ( winp->field_c > 300 )
        winp->field_c = 300;
    else if ( winp->field_c < -300 )
        winp->field_c = -300;

    arg->field_8 = winp->field_c / 300.0;
}

void NC_STACK_winp::idev_func66(winp_66arg *arg)
{
    int cnt = wmcharBuf_cnt;
    __NC_STACK_winp *winp = &this->stack__winp;

    arg->dword8 = 0;
    arg->downed_key_2 = downed_key_2;
    arg->downed_key = downed_key;
    arg->chr = wmcharBuf[0];

    if ( cnt )
    {
        cnt--;
        for (int i = 0; i < cnt; i++)
            wmcharBuf[i] = wmcharBuf[i + 1];
    }

    wmcharBuf[cnt] = 0;

    downed_key = 0;
    if ( arg->downed_key )
    {
        for (int i = 0; i < 48; i++)
        {
            if (winp->field_10[i] == arg->downed_key)
            {
                arg->dword8 = i | 0x80;
                break;
            }
        }
    }
    wmcharBuf_cnt = cnt;
}

int NC_STACK_winp::idev_func67(const char **arg)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    for(int i = 0; i < 128; i++)
    {
        if ( !strcasecmp(winp_keys[i].keyname, *arg) )
        {
            winp->field_4 = i;
            return winp_keys[i].keytype;
        }
    }
    return 0;
}

int NC_STACK_winp::idev_func68(winp_68arg *arg)
{
    __NC_STACK_winp *winp = &this->stack__winp;

    if ( arg->id < 0x30 )
    {
        for ( int i = 0; i < 128; i++ )
        {
            if ( !strcasecmp(winp_keys[i].keyname, arg->keyname) && winp_keys[i].keytype == 1 )
            {
                winp->field_10[arg->id] = winp_keys[i].keycode;
                return 3;
            }
        }
    }
    return 0;
}

void NC_STACK_winp::idev_func69(int *arg)
{
    printf("CHECK ARG TYPE %s\n","winp_func69");

    __NC_STACK_winp *winp = &this->stack__winp;

    if ( *arg == 1 )
        winp->field_c = 0;
}

void NC_STACK_winp::idev_func70(int *arg)
{
    //printf("CHECK ARG TYPE %s\n","winp_func70");

    __NC_STACK_winp *winp = &this->stack__winp;

    if ( *arg )
    {
        for ( int i = 0; i < 48; i++ )
        {
            if ( winp->field_10[i] == *arg )
            {
                arg[1] = i | 0x80;
                break;
            }
        }
    }
    else if (arg[1])
    {
        int tmp = arg[1] & 0x7F;
        if (tmp < 48)
            *arg = winp->field_10[tmp];
    }
}


DIEFFECT stru_43D631 = {sizeof(DIEFFECT), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
DIPERIODIC stru_43D665 = {0x10, 0, 0, 0};
DIEFFECT stru_43D675 = {sizeof(DIEFFECT), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
DICONDITION stru_43D6A9[2] = {{0x30, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
DIEFFECT stru_43D6D9 = {sizeof(DIEFFECT), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
DIRAMPFORCE stru_43D70D = {8, 0};
DIEFFECT stru_43D715 = {sizeof(DIEFFECT), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
DIPERIODIC stru_43D749 = {0x10, 0, 0, 0};
DIENVELOPE stru_43D759 = {sizeof(DIENVELOPE), 0, 0, 0, 0};

HRESULT sub_4404C4(IDirectInputEffect *effct)
{
    HRESULT hres = 0;
    if ( effct )
    {
        hres = effct->Start(1, 0);
        if ( hres == (HRESULT)0x80040203 )
        {
            effct->Download();
            hres = effct->Start(1, 0);
        }
    }
    return hres;
}

void sub_440066(IDirectInputEffect *dinpEff, float a2, float a3)
{
    DIPERIODIC period;
    DIEFFECT dieff;

    if ( dinpEff )
    {
        memcpy(&dieff, &stru_43D631, sizeof(DIEFFECT));

        period.dwMagnitude = stru_43D665.dwMagnitude;
        period.lOffset = stru_43D665.lOffset;
        period.dwPhase = stru_43D665.dwPhase;
        period.dwPeriod = stru_43D665.dwPeriod;

        dieff.dwSize = sizeof(DIEFFECT);
        dieff.cbTypeSpecificParams = sizeof(DIPERIODIC);
        dieff.lpvTypeSpecificParams = &period;

        HRESULT hres = dinpEff->GetParameters(&dieff, DIEP_TYPESPECIFICPARAMS);
        if ( hres )
        {
            log_dinput_error("DirectInputEffect", "GetParameters()", hres);
        }
        else
        {
            period.dwMagnitude = a2;
            period.dwPeriod = a3;
            hres = dinpEff->SetParameters(&dieff, DIEP_TYPESPECIFICPARAMS);
            if ( hres )
                log_dinput_error("DirectInputEffect", "SetParameters()", hres);
        }
    }
}

void winp_func71__sub0(int state, float p1, float p2)
{
    if ( winp_local.effects.effects[0] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[0] )
                    winp_local.effects.effects[0]->Stop();
            }
            else
            {
                float v5 = p1 * 6000.0;
                float v6 = 1000000.0 / (p2 * 13.0 + 5.0);
                sub_440066(winp_local.effects.effects[0], v5, v6);
            }
        }
        else
        {
            if ( winp_local.effects.effects[0] )
                winp_local.effects.effects[0]->Stop();
            if ( winp_local.effects.effects[1] )
                winp_local.effects.effects[1]->Stop();
            if ( winp_local.effects.effects[2] )
                winp_local.effects.effects[2]->Stop();

            HRESULT hres = sub_4404C4(winp_local.effects.effects[0]);
            if ( hres )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", hres);

            float a2 = p1 * 5000.0;
            float a3 = 1000000.0 / (p2 * 13.0 + 5.0);
            sub_440066(winp_local.effects.effects[0], a2, a3);
        }
    }
}

void winp_func71__sub1(int state, float p1, float p2)
{
    if ( winp_local.effects.effects[1] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[1] )
                    winp_local.effects.effects[1]->Stop();
            }
            else
            {
                float v5 = p1 * 6000.0;
                float v6 = 1000000.0 / (p2 * 12.0 + 14.0);
                sub_440066(winp_local.effects.effects[1], v5, v6);
            }
        }
        else
        {
            if ( winp_local.effects.effects[0] )
                winp_local.effects.effects[0]->Stop();
            if ( winp_local.effects.effects[1] )
                winp_local.effects.effects[1]->Stop();
            if ( winp_local.effects.effects[2] )
                winp_local.effects.effects[2]->Stop();

            HRESULT errid = sub_4404C4(winp_local.effects.effects[1]);
            if ( errid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", errid);

            float a2 = p1 * 6000.0;
            float a3 = 1000000.0 / (p2 * 12.0 + 14.0);
            sub_440066(winp_local.effects.effects[1], a2, a3);
        }
    }
}

void winp_func71__sub2(int state, float p1, float p2)
{
    if ( winp_local.effects.effects[2] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[2] )
                    winp_local.effects.effects[2]->Stop();
            }
            else
            {
                float v5 = p1 * 8000.0;
                float v6 = 1000000.0 / (p2 * 12.0 + 6.0);
                sub_440066(winp_local.effects.effects[2], v5, v6);
            }
        }
        else
        {
            if ( winp_local.effects.effects[0] )
                winp_local.effects.effects[0]->Stop();
            if ( winp_local.effects.effects[1] )
                winp_local.effects.effects[1]->Stop();
            if ( winp_local.effects.effects[2] )
                winp_local.effects.effects[2]->Stop();

            HRESULT hres = sub_4404C4(winp_local.effects.effects[2]);
            if ( hres )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", hres);

            float a2 = p1 * 8000.0;
            float a3 = 1000000.0 / (p2 * 12.0 + 6.0);
            sub_440066(winp_local.effects.effects[2], a2, a3);
        }
    }
}

void winp_func71__sub3__sub0(IDirectInputEffect *effct, float a2)
{
    DICONDITION v6[2];
    DIEFFECT v7;

    if ( effct )
    {
        memcpy(&v7, &stru_43D675, sizeof(DIEFFECT));
        memcpy(v6, stru_43D6A9, sizeof(DICONDITION) * 2);
        v7.dwSize = sizeof(DIEFFECT);
        v7.cbTypeSpecificParams = sizeof(DICONDITION) * 2;
        v7.lpvTypeSpecificParams = v6;

        HRESULT msgid = effct->GetParameters(&v7, DIEP_TYPESPECIFICPARAMS);
        if ( msgid )
        {
            log_dinput_error("DirectInputEffect", "GetParameters()", msgid);
        }
        else
        {
            v6[0].lPositiveCoefficient = a2;
            v6[0].lNegativeCoefficient = a2;
            v6[1].lPositiveCoefficient = a2;
            v6[1].lNegativeCoefficient = a2;
            msgid = effct->SetParameters(&v7, DIEP_TYPESPECIFICPARAMS);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "SetParameters()", msgid);
        }
    }
}

void winp_func71__sub3(int state, float p1)
{
    if ( winp_local.effects.effects[3] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[3] )
                    winp_local.effects.effects[3]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[3] )
                winp_local.effects.effects[3]->Stop();

            float v2 = p1 * 10000.0;
            winp_func71__sub3__sub0(winp_local.effects.effects[3], v2);

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[3]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub4(int state)
{
    if ( winp_local.effects.effects[4] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[4] )
                    winp_local.effects.effects[4]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[4] )
                winp_local.effects.effects[4]->Stop();

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[4]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub5(int state)
{
    if ( winp_local.effects.effects[5] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[5] )
                    winp_local.effects.effects[5]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[5] )
                winp_local.effects.effects[5]->Stop();
            if ( winp_local.effects.effects[6] )
                winp_local.effects.effects[6]->Stop();
            if ( winp_local.effects.effects[7] )
                winp_local.effects.effects[7]->Stop();

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[5]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub6(int state)
{
    if ( winp_local.effects.effects[6] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[6] )
                    winp_local.effects.effects[6]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[5] )
                winp_local.effects.effects[5]->Stop();
            if ( winp_local.effects.effects[6] )
                winp_local.effects.effects[6]->Stop();
            if ( winp_local.effects.effects[7] )
                winp_local.effects.effects[7]->Stop();

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[6]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub7(int state)
{
    if ( winp_local.effects.effects[7] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[7] )
                    winp_local.effects.effects[7]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[5] )
                winp_local.effects.effects[5]->Stop();
            if ( winp_local.effects.effects[6] )
                winp_local.effects.effects[6]->Stop();
            if ( winp_local.effects.effects[7] )
                winp_local.effects.effects[7]->Stop();

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[7]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub8__sub0(IDirectInputEffect *effct, float a2, float a3, float a4)
{
    LONG v8[2];
    DIRAMPFORCE v9;
    DIEFFECT v10;

    if ( effct )
    {
        memcpy(&v10, &stru_43D6D9, sizeof(DIEFFECT));

        v9.lStart = stru_43D70D.lStart;
        v9.lEnd = stru_43D70D.lEnd;
        v10.cbTypeSpecificParams = sizeof(DIRAMPFORCE);
        v10.lpvTypeSpecificParams = &v9;

        HRESULT msgid = effct->GetParameters(&v10, DIEP_TYPESPECIFICPARAMS);
        if ( msgid )
        {
            log_dinput_error("DirectInputEffect", "GetParameters()", msgid);
        }
        else
        {
            v9.lStart = a2 * 10000.0;
            v9.lEnd = a2 * -10000.0;
            v10.dwFlags = 18;
            v10.cAxes = 2;
            v10.rgdwAxes = 0;
            v10.rglDirection = v8;
            v8[0] = a3 * 1000.0;
            v8[1] = a4 * 1000.0;

            msgid = effct->SetParameters(&v10, DIEP_TYPESPECIFICPARAMS | DIEP_DIRECTION);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "SetParameters()", msgid);
        }
    }
}

void winp_func71__sub8(int state, float a2, float a3, float a4)
{
    if ( winp_local.effects.effects[8] )
    {
        if ( state )
        {
            if ( state == 1 )
            {
                if ( winp_local.effects.effects[8] )
                    winp_local.effects.effects[8]->Stop();
            }
        }
        else
        {
            if ( winp_local.effects.effects[8] )
                winp_local.effects.effects[8]->Stop();

            winp_func71__sub8__sub0(winp_local.effects.effects[8], a2, a3, a4);

            HRESULT msgid = sub_4404C4(winp_local.effects.effects[8]);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "winp_FFStart()", msgid);
        }
    }
}

void winp_func71__sub9__sub0(float a1, float a2, float a3, float a4)
{
    LONG v10[2];
    DIENVELOPE v12;
    DIPERIODIC v13;
    DIEFFECT v14;
    IDirectInputEffect *effct9 = winp_local.effects.effects[9];
    IDirectInputEffect *effct10 = winp_local.effects.effects[10];

    if ( effct9 && effct10 )
    {
        memcpy(&v14, &stru_43D715, sizeof(DIEFFECT));
        v13.dwMagnitude = stru_43D749.dwMagnitude;
        v13.lOffset = stru_43D749.lOffset;
        v13.dwPhase = stru_43D749.dwPhase;
        v13.dwPeriod = stru_43D749.dwPeriod;

        v12.dwSize = stru_43D759.dwSize;
        v12.dwAttackLevel = stru_43D759.dwAttackLevel;
        v12.dwAttackTime = stru_43D759.dwAttackTime;
        v12.dwFadeLevel = stru_43D759.dwFadeLevel;
        v12.dwFadeTime = stru_43D759.dwFadeTime;

        v14.cbTypeSpecificParams = sizeof(DIPERIODIC);
        v14.lpvTypeSpecificParams = &v13;

        HRESULT msgid = effct9->GetParameters(&v14, DIEP_TYPESPECIFICPARAMS);
        if ( !msgid )
        {
            v13.dwMagnitude = a1 * 10000.0;
            v13.lOffset = 0;
            v13.dwPhase = 0;
            v13.dwPeriod = 71428;

            v12.dwAttackTime = 0;
            v12.dwAttackLevel = a1 * 10000.0;
            v12.dwFadeTime = a2 * 0.37 * 1000.0;
            v12.dwFadeLevel = 0;

            v14.dwFlags = 18;
            v14.dwDuration = a2 * 0.63 * 1000.0;
            v14.cAxes = 2;
            v14.rgdwAxes = 0;
            v14.lpEnvelope = &v12;
            v14.rglDirection = v10;

            v10[0] = a3 * 1000.0;
            v10[1] = a4 * 1000.0;

            msgid = effct9->SetParameters(&v14, DIEP_TYPESPECIFICPARAMS | DIEP_ENVELOPE | DIEP_DIRECTION | DIEP_DURATION);
            if ( msgid )
                log_dinput_error("DirectInputEffect", "SetParameters(Shake1)", msgid);
        }
    }
}


void winp_func71__sub9(int state, float a2, float a3, float a4, float a5)
{
    if ( winp_local.effects.effects[9] )
    {
        if ( winp_local.effects.effects[10] )
        {
            if ( state )
            {
                if ( state == 1 )
                {
                    if ( winp_local.effects.effects[9] )
                        winp_local.effects.effects[9]->Stop();
                }
            }
            else
            {
                if ( winp_local.effects.effects[9] )
                    winp_local.effects.effects[9]->Stop();
                if ( winp_local.effects.effects[10] )
                    winp_local.effects.effects[10]->Stop();

                winp_func71__sub9__sub0(a2, a3, a4, a5);

                HRESULT msgid = sub_4404C4(winp_local.effects.effects[9]);
                if ( msgid )
                    log_dinput_error("DirectInputEffect", "winp_FFStart(Shake1)", msgid);
            }
        }
    }
}

void winp_func71__sub10()
{
    if ( winp_local.effects.effects[0] )
        winp_local.effects.effects[0]->Stop();
    if ( winp_local.effects.effects[1] )
        winp_local.effects.effects[1]->Stop();
    if ( winp_local.effects.effects[2] )
        winp_local.effects.effects[2]->Stop();
    if ( winp_local.effects.effects[3] )
        winp_local.effects.effects[3]->Stop();
    if ( winp_local.effects.effects[4] )
        winp_local.effects.effects[4]->Stop();
    if ( winp_local.effects.effects[5] )
        winp_local.effects.effects[5]->Stop();
    if ( winp_local.effects.effects[6] )
        winp_local.effects.effects[6]->Stop();
    if ( winp_local.effects.effects[7] )
        winp_local.effects.effects[7]->Stop();
    if ( winp_local.effects.effects[8] )
        winp_local.effects.effects[8]->Stop();
    if ( winp_local.effects.effects[9] )
        winp_local.effects.effects[9]->Stop();
    if ( winp_local.effects.effects[10] )
        winp_local.effects.effects[10]->Stop();
}


void NC_STACK_winp::idev_func71(winp_71arg *arg)
{
    switch ( arg->effID )
    {
    case 1:
        winp_func71__sub0(arg->state, arg->p1, arg->p2);
        break;
    case 2:
        winp_func71__sub1(arg->state, arg->p1, arg->p2);
        break;
    case 3:
        winp_func71__sub2(arg->state, arg->p1, arg->p2);
        break;
    case 4:
        winp_func71__sub3(arg->state, arg->p1);
        break;
    case 5:
        winp_func71__sub4(arg->state);
        break;
    case 6:
        winp_func71__sub5(arg->state);
        break;
    case 7:
        winp_func71__sub6(arg->state);
        break;
    case 8:
        winp_func71__sub7(arg->state);
        break;
    case 9:
        winp_func71__sub8(arg->state, arg->p1, arg->p3, arg->p4);
        break;
    case 10:
        winp_func71__sub9(arg->state, arg->p1, arg->p2, arg->p3, arg->p4);
        break;
    case 0:
        winp_func71__sub10();
        break;
    default:
        return;
    }
}

size_t NC_STACK_winp::iwimp_func128(stack_vals *stak)
{
    return 1;
}

void winp_func131__sub1()
{
    if ( winp_local.dinputDev2 )
    {
        int v2 = joyState_buttonsState;
        joyState_buttonsState = 0;
        memset(&joyState_pos, 0, sizeof(shortPoint));
        memset(&joyState_ZRzpos, 0, sizeof(shortPoint));

        HRESULT hres = winp_local.dinputDev2->Poll();
        if ( hres == (HRESULT)0x8007001E || hres == (HRESULT)0x8007000C )
        {
            hres = winp_local.dinputDev2->Acquire();
            hres = winp_local.dinputDev2->Poll();
        }

        DIJOYSTATE joyState;
        hres = winp_local.dinputDev2->GetDeviceState(sizeof(DIJOYSTATE), &joyState);
        if ( !hres )
        {
            joyState_pos.x = joyState.lX;
            joyState_pos.y = joyState.lY;

            if ( winp_local.dinput2Caps.dwAxes <= 2 )
                joyState_ZRzpos.x = -300;
            else
                joyState_ZRzpos.x = joyState.lZ;

            if ( winp_local.dinput2Caps.dwAxes <= 3 )
                joyState_ZRzpos.y = 0;
            else
                joyState_ZRzpos.y = joyState.lRz;

            for (int i = 0; i < 8; i++ )
            {
                if ( joyState.rgbButtons[i] & 0x80 )
                {
                    joyState_buttonsState |= 1 << i;
                    if ( !((1 << i) & v2) )
                        winp_keyDown(i + 0x86);
                }
                else if ( (1 << i) & v2 )
                {
                    winp_keyUp(i + 0x86);
                }
            }
            if ( winp_local.dinput2Caps.dwPOVs )
            {
                if ( joyState.rgdwPOV[0] == 65535 )
                {
                    winp_pov.x = 0;
                    winp_pov.y = 0;
                }
                else if ( joyState.rgdwPOV[0] >= 4500 && joyState.rgdwPOV[0] < 13500 )
                {
                    winp_pov.x = 300;
                    winp_pov.y = 0;
                }
                else if ( joyState.rgdwPOV[0] >= 13500 && joyState.rgdwPOV[0] < 22500 )
                {
                    winp_pov.x = -0;
                    winp_pov.y = -300;
                }
                else if ( joyState.rgdwPOV[0] >= 22500 && joyState.rgdwPOV[0] < 31500 )
                {
                    winp_pov.x = -300;
                    winp_pov.y = 0;
                }
                else if (joyState.rgdwPOV[0] >= 31500 || joyState.rgdwPOV[0] < 4500)
                {
                    winp_pov.x = 0;
                    winp_pov.y = 300;
                }
            }
            else
            {
                winp_pov.x = 0;
                winp_pov.y = 0;
            }
        }
    }
}

void winp_getmouse_state()
{
    DIMOUSESTATE dimouse;

    if ( winp_local.mouseDev )
    {
        HRESULT v1 = winp_local.mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &dimouse);
        if ( v1 == (HRESULT)0x8007001E )
        {
            v1 = winp_local.mouseDev->Acquire();
            v1 = winp_local.mouseDev->GetDeviceState(sizeof(DIMOUSESTATE), &dimouse);
        }

        if ( !v1 )
        {
            mouseState_pos.x = 8 * dimouse.lX;
            mouseState_pos.y = 8 * dimouse.lY;
        }
    }
}

void NC_STACK_winp::iwimp_func131(winp_131arg *arg)
{
    __NC_STACK_winp *winp = &this->stack__winp;
    arg->flag = 0;
    if ( winp->hWND )
    {
        winp_getmouse_state();
        winp_func131__sub1();

        arg->move[0].x = winp_move.x;
        arg->move[0].y = winp_move.y;

        if ( winp_lbut )
            arg->flag |= 4;
        if ( winp_mbut )
            arg->flag |= 0x800;
        if ( winp_rbut )
            arg->flag |= 0x100;
        if ( winp_ldbl )
            arg->flag |= 0x2000;
        winp_ldbl = 0;
        if ( winp_lbut_dwn_cnt > 0 )
        {
            arg->ldw_pos[0].x = winp_ldw_pos.x;
            arg->ldw_pos[0].y = winp_ldw_pos.y;
            arg->flag |= 2;
            winp_lbut_dwn_cnt = 0;
        }
        if ( winp_lbut_up_cnt > 0 )
        {
            arg->lup_pos[0].x = winp_lup_pos.x;
            arg->lup_pos[0].y = winp_lup_pos.y;
            arg->flag |= 8;
            winp_lbut_up_cnt = 0;
        }
        if ( winp_rbut_dwn_cnt > 0 )
        {
            arg->flag |= 0x80;
            winp_rbut_dwn_cnt = 0;
        }
        if ( winp_rbut_up_cnt > 0 )
        {
            arg->flag |= 0x200;
            winp_rbut_up_cnt = 0;
        }
        if ( winp_mbut_dwn_cnt > 0 )
        {
            arg->flag |= 0x400;
            winp_mbut_dwn_cnt = 0;
        }
        if ( winp_mbut_up_cnt > 0 )
        {
            arg->flag |= 0x1000;
            winp_mbut_up_cnt = 0;
        }

        if ( arg->move[0].x >= winp_max_x )
            arg->move[0].x = winp_max_x - 1;
        else if ( arg->move[0].x < 0 )
            arg->move[0].x = 0;

        if ( arg->move[0].y >= winp_max_y )
            arg->move[0].y = winp_max_y - 1;
        else if ( arg->move[0].y < 0 )
            arg->move[0].y = 0;

        if ( arg->ldw_pos[0].x >= winp_max_x )
            arg->ldw_pos[0].x = winp_max_x - 1;
        else if ( arg->ldw_pos[0].x < 0 )
            arg->ldw_pos[0].x = 0;

        if ( arg->ldw_pos[0].y >= winp_max_y )
            arg->ldw_pos[0].y = winp_max_y - 1;
        else if ( arg->ldw_pos[0].y < 0 )
            arg->ldw_pos[0].y = 0;


        if ( arg->lup_pos[0].x >= winp_max_x )
            arg->lup_pos[0].x = winp_max_x - 1;
        else if ( arg->lup_pos[0].x < 0 )
            arg->lup_pos[0].x = 0;

        if ( arg->lup_pos[0].y >= winp_max_y )
            arg->lup_pos[0].y = winp_max_y - 1;
        else if ( arg->lup_pos[0].y < 0 )
            arg->lup_pos[0].y = 0;

        NC_STACK_iwimp::iwimp_func131(arg);
    }
}

void NC_STACK_winp::initfirst()
{
    if ( !class_set_winp__sub0() )
        return;

    memset(vk_map, 0xFF, 256);
    for (int i = 0; i < 256; i++)
    {
        int j = 0;
        while (winp_keys[j].keytype)
        {
            if (i == winp_keys[j].keycode && vk_map[i] == 0xFF)
                vk_map[i] = j;

            j++;
        }
    }

    winp_max_x = 0;
    winp_max_y = 0;
    downed_key_2 = 0;
    downed_key = 0;
    joyState_buttonsState = 0;

    memset(wmcharBuf, 0, sizeof(int) * 8);
    wmcharBuf_maxCnt = 8;
    wmcharBuf_cnt = 0;

    winp_lbut = 0;
    winp_rbut = 0;
    winp_mbut = 0;
    winp_ldbl = 0;
    winp_lbut_dwn_cnt = 0;
    winp_lbut_up_cnt = 0;
    winp_rbut_up_cnt = 0;
    winp_rbut_dwn_cnt = 0;
    winp_mbut_up_cnt = 0;
    winp_mbut_dwn_cnt = 0;

    memset(&mouseState_pos, 0, sizeof(shortPoint));
    memset(&winp_move, 0, sizeof(shortPoint));
    memset(&winp_ldw_pos, 0, sizeof(shortPoint));
    memset(&winp_rdw_pos, 0, sizeof(shortPoint));
    memset(&winp_mdwn_pos, 0, sizeof(shortPoint));
    memset(&winp_lup_pos, 0, sizeof(shortPoint));
    memset(&winp_rup_pos, 0, sizeof(shortPoint));
    memset(&winp_mup_pos, 0, sizeof(shortPoint));
}

size_t NC_STACK_winp::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        func2( (stack_vals *)data );
        return 1;
    case 3:
        func3( (stack_vals *)data );
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
        idev_func69( (int *)data );
        return 1;
    case 70:
        idev_func70( (int *)data );
        return 1;
    case 71:
        idev_func71( (winp_71arg *)data );
        return 1;
    case 128:
        return (size_t)iwimp_func128( (stack_vals *)data );
    case 131:
        iwimp_func131( (winp_131arg *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_iwimp::compatcall(method_id, data);
}
