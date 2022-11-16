#ifndef WORLD_DEFSTR_H
#define WORLD_DEFSTR_H

#include <array>
#include <string>
#include "stringids.h"

namespace World { 

class DefaultStrings
{
public:
    static std::array<std::string, TIP_MAX> Tooltips;
};

}



#endif