#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wintimer.h"
#include "utils.h"

int NC_STACK_wintimer::itimer_func64()
{
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 cnt = SDL_GetPerformanceCounter();
    uint32_t tik = cnt / (freq / 1024);

    int period = tik - ticks;

    ticks = tik;

    if ( period < 0 || period > 2000 )
        period = 1;

    return period;
}

