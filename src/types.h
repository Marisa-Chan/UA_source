#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
#include <list>
#include "listnode.h"


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

struct player_status
{
    int destroyed;
    int destroyedByUser;
    int elapsedTime;
    int sectorsTaked;
    int score;
    int power;
    int upgrades;

    player_status()
    {
    	clear();
    }

    void clear()
    {
    	destroyed = 0;
        destroyedByUser = 0;
        elapsedTime = 0;
        sectorsTaked = 0;
        score = 0;
        power = 0;
        upgrades = 0;
    }
};

}
#endif // TYPES_H_INCLUDED
