#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wintimer.h"
#include "utils.h"



const Nucleus::ClassDescr NC_STACK_wintimer::description("wintimer.class", &newinstance);


uint32_t wintimer_func64__sub0()
{
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 cnt = SDL_GetPerformanceCounter();
    return cnt / (freq / 1024);
//        return SDL_GetTicks();
}

int NC_STACK_wintimer::itimer_func64()
{
    uint32_t tik = wintimer_func64__sub0();

    int period = tik - ticks;

    ticks = tik;

    if ( period < 0 || period > 2000 )
        period = 1;

    return period;
}

