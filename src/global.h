#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifdef GLOBAL

#define EXTR

#else

#define EXTR extern

#endif // GLOBAL

#include "glob_funcs.h"

EXTR char cmdline_copy[512];


EXTR g_engines engines;


EXTR int can_srcblend;
EXTR int can_destblend;
EXTR int can_stippling;

extern int dword_514EFC;
EXTR int dword_514F24;

EXTR UserData userdata;
EXTR InputState input_states;
EXTR base_64arg world_update_arg;

EXTR NC_STACK_ypaworld *ypaworld;
EXTR int dword_520400;


#endif // GLOBAL_H_INCLUDED1
