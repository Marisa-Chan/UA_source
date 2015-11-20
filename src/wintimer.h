#ifndef WINTIMER_H_INCLUDED
#define WINTIMER_H_INCLUDED

#include "itimer.h"

extern class_stored wintimer_class_off;

struct NC_STACK_wintimer;

struct __NC_STACK_wintimer
{
    DWORD ticks;
};

struct NC_STACK_wintimer : public NC_STACK_itimer
{
	__NC_STACK_wintimer stack__wintimer;
};

#endif // WINTIMER_H_INCLUDED
