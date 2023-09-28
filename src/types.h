#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
#include <list>
#include "listnode.h"
#include <cstdint>


struct cellArea;
class NC_STACK_ypabact;
class NC_STACK_ypamissile;

namespace Engine
{
typedef std::list< std::string > StringList;

}

namespace World
{
typedef RefList<NC_STACK_ypabact *> RefBactList;
typedef std::list<NC_STACK_ypabact *> BactList;
typedef std::list<NC_STACK_ypamissile *> MissileList;

struct TPlayerStatus
{
    int32_t DestroyedUnits = 0;
    int32_t DestroyedByUser = 0;
    int32_t ElapsedTime = 0;
    int32_t SectorsTaked = 0;
    int32_t Score = 0;
    int32_t Power = 0;
    int32_t Upgrades = 0;
};

}
#endif // TYPES_H_INCLUDED
