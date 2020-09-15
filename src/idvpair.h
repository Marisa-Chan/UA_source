#ifndef IDVPAIR_H_INCLUDED
#define IDVPAIR_H_INCLUDED

#include <inttypes.h>
#include <map>
#include <initializer_list>
#include "any-lite/any.hpp"

struct IDVPair
{
    uint32_t ID  = 0;
    bool    Skip = false;
    nonstd::any Value = (int32_t)0;

    IDVPair()
    {
        ID = 0;
        Skip = false;
        Value = (int32_t)0;
    }

    IDVPair(uint32_t _id, nonstd::any _val)
    : ID(_id), Skip(false), Value(_val)
    {}
    
    template <typename T>
    T Get()
    {
        T t;
        try
        {
            t = nonstd::any_cast<T>(Value);
        }
        catch (const nonstd::bad_any_cast& e)
        {
            printf("%X %s\n", ID, e.what());
        }
        return t;
    }
};


class IDVList: public std::map<uint32_t, IDVPair>
{
public:
    typedef std::pair<uint32_t, nonstd::any> TInitPair;
    typedef std::initializer_list< TInitPair > TInitList;
    
    using std::map<uint32_t, IDVPair>::map;
    
    IDVList()
    {}
    
    IDVList(TInitList pairs)
    {
        for ( const TInitPair &p : pairs )
            (*this)[p.first] = IDVPair(p.first, p.second);
    }
    
    void Add(uint32_t _id, nonstd::any _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }
    
    template <typename T>
    T Get(uint32_t _id, T _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        
        return it->second.Get<T>();
    }
};

#endif // IDVPAIR_H_INCLUDED
