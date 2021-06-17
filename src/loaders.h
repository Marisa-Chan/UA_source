#ifndef LOADERS_H_INCLUDED
#define LOADERS_H_INCLUDED

#include <string>
#include "bitmap.h"

namespace Utils
{
    NC_STACK_bitmap *ProxyLoadImage(IDVList &stak);
    NC_STACK_bitmap *ProxyLoadImage(IDVList::TInitList lst);
}

#endif // LOADERS_H_INCLUDED
