#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "wintimer.h"
#include "utils.h"



const NewClassDescr NC_STACK_wintimer::description("wintimer.class", &newinstance);


uint32_t wintimer_func64__sub0()
{
    Uint64 freq = SDL_GetPerformanceFrequency();
    Uint64 cnt = SDL_GetPerformanceCounter();
    return cnt / (freq / 1024);
//        return SDL_GetTicks();
}

int NC_STACK_wintimer::itimer_func64(void *)
{
    __NC_STACK_wintimer *tmr = &stack__wintimer;

    uint32_t tik = wintimer_func64__sub0();

    int period = tik - tmr->ticks;

    tmr->ticks = tik;

    if ( period < 0 || period > 2000 )
        period = 1;

    return period;
}


size_t NC_STACK_wintimer::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 64:
        return (size_t)itimer_func64( (void *)data );
    default:
        break;
    }
    return NC_STACK_itimer::compatcall(method_id, data);
}
