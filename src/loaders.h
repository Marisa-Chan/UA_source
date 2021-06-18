#ifndef LOADERS_H_INCLUDED
#define LOADERS_H_INCLUDED

#include <string>
#include "bitmap.h"
#include "base.h"

namespace Utils
{
    NC_STACK_bitmap *ProxyLoadImage(IDVList &stak);
    NC_STACK_bitmap *ProxyLoadImage(IDVList::TInitList lst);
    
    NC_STACK_base *ProxyLoadBase(const std::string &fname);
}

#endif // LOADERS_H_INCLUDED
