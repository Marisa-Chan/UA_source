#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include <string>
#include <list>
#include "listnode.h"


struct cellArea;
struct NC_STACK_ypabact;

namespace Engine
{
typedef std::list< std::string > StringList;

};

namespace World
{
typedef RefList<NC_STACK_ypabact *> CellBactList;

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
