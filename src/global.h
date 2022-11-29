#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifdef GLOBAL

#define EXTR

#else

#define EXTR extern

#endif // GLOBAL

#include "glob_funcs.h"

extern int dword_514EFC;
EXTR int dword_514F24;

EXTR UserData userdata;
EXTR TInputState input_states;
EXTR base_64arg world_update_arg;

EXTR NC_STACK_ypaworld *ypaworld;

enum GameScreenMode {
    GAME_SCREEN_MODE_UNKNOWN = 0,
    GAME_SCREEN_MODE_MENU = 1,
    GAME_SCREEN_MODE_GAME = 2,
    GAME_SCREEN_MODE_UNKNOWN3 = 3
};

EXTR GameScreenMode gameScreen;


#endif // GLOBAL_H_INCLUDED1
