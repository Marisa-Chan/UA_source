#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED

// Should be deleted


#ifdef GLOBAL

#define EXTR

#else

#define EXTR extern

#endif // GLOBAL

extern int dword_514EFC;
EXTR int dword_514F24;

EXTR TInputState input_states;
EXTR base_64arg world_update_arg;

EXTR NC_STACK_ypaworld *ypaworld;


#endif // GLOBAL_H_INCLUDED1
