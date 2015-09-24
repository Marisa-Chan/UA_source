#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

#ifdef GLOBAL

#define EXTR

#else

#define EXTR extern

#endif // GLOBAL

#include "glob_funcs.h"

EXTR HINSTANCE ghInstance;
EXTR int gCmdShow;
EXTR HWND ghWnd;
EXTR LPSTR gpcmdline;

EXTR char cmdline_copy[512];


EXTR g_engines engines;

EXTR glob_funcs *point_to_off_51363C;
EXTR glob_funcs off_51363C;


EXTR nlist classes_list;

EXTR glob_funcs stru_51363C;

EXTR stored_functions_engine* pgfx_engine;
EXTR stored_functions_engine* paudio_engine;
EXTR stored_functions_engine* pinput_engine;
EXTR stored_functions_engine* ptform_engine;


#endif // GLOBAL_H_INCLUDED1
