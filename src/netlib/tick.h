#ifndef ZNDNET_TICK_H_INCLUDED
#define ZNDNET_TICK_H_INCLUDED

namespace ZNDNet
{

struct Tick64
{
    uint32_t lastTick;
    uint32_t lap;

    Tick64()
    {
        lap = 0;
        lastTick = 0;
    }

    uint64_t GetTicks()
    {
        uint32_t tick = SDL_GetTicks();

        if (lastTick > tick)
            lap++;

        lastTick = tick;

        return ((uint64_t)lap << 32) | (uint64_t)tick;
    }

    uint32_t GetSec()
    {
        return GetTicks()/1000;
    }
};

}

#endif
