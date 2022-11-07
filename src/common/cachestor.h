#ifndef CACHESTOR_H_INCLUDED
#define CACHESTOR_H_INCLUDED

#include "common.h"


namespace Common
{

template <typename T, std::size_t BLKSZ = 10000>
struct CacheStorage
{
    typedef std::array<T, BLKSZ> _TBLOCK;
    
    std::deque<_TBLOCK> Blocks;
    size_t NextPos = 0;
    
    T& GetNext()
    {
        while(NextPos >= Blocks.size() * BLKSZ)
            Blocks.emplace_back();
        
        T& tmp = Blocks.at( NextPos / BLKSZ ).at( NextPos % BLKSZ );
        NextPos++;
        return tmp;
    }
    
    void Rewind() { NextPos = 0; }
};

}

#endif