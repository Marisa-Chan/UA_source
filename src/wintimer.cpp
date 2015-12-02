#include <inttypes.h>
#include <windows.h>
#include "includes.h"
#include "nucleas.h"
#include "wintimer.h"
#include "utils.h"


stored_functions *classvtbl_get_wintimer();
class_return * class_set_wintimer(int, ...);

stored_functions wintimer_class_vtbl(class_set_wintimer);


class_stored wintimer_class_off (NULL, NULL, "MC2classes:drivers/input/wintimer.class", classvtbl_get_wintimer);


stored_functions *classvtbl_get_wintimer()
{
	return &wintimer_class_vtbl;
}

CLASSFUNC wintimer_funcs[1024];


DWORD wintimer_func64__sub0()
{
	LARGE_INTEGER v3;
	LARGE_INTEGER PerformanceCount;

	if ( QueryPerformanceFrequency(&v3) )
	{
		QueryPerformanceCounter(&PerformanceCount);
		return PerformanceCount.QuadPart / (v3.LowPart >> 10);
	}
	else
	{
		return GetTickCount();
	}
}

int wintimer_func64(NC_STACK_wintimer *obj, class_stru *zis, void *)
{
	__NC_STACK_wintimer *tmr = &obj->stack__wintimer;

	DWORD tik = wintimer_func64__sub0();

	int period = tik - tmr->ticks;

	tmr->ticks = tik;

	if ( period < 0 || period > 2000 )
		period = 1;

	return period;
}


class_return wintimer_class_descr;

class_return * class_set_wintimer(int a1, ...)
{

	memset(wintimer_funcs, 0, sizeof(CLASSFUNC) * 1024);

	wintimer_funcs[64] = (CLASSFUNC)wintimer_func64;

	wintimer_class_descr.parent = "itimer.class";

	wintimer_class_descr.vtbl = wintimer_funcs;
	////wintimer_class_descr.varSize = sizeof(__NC_STACK_wintimer);
	wintimer_class_descr.varSize = sizeof(NC_STACK_wintimer) - offsetof(NC_STACK_wintimer, stack__wintimer); //// HACK
	wintimer_class_descr.field_A = 0;
	return &wintimer_class_descr;
}
