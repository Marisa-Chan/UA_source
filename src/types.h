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
}
#endif // TYPES_H_INCLUDED
