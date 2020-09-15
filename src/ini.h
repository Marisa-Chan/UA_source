#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

#include <string>
#include <vector>
#include "any-lite/any.hpp"

namespace Common {
namespace Ini {

enum KEYTYPE
{
    KT_DIGIT = 0x0,
    KT_WORD  = 0x1,
    KT_BOOL  = 0x2,
    KT_STRING = 0x3,
};

struct Key
{
    const std::string Name;
    KEYTYPE Type;
    nonstd::any Value;
    
    Key(const std::string &k, KEYTYPE t, nonstd::any v)
    : Name(k), Type(t), Value(v)
    {}
    
    Key(const std::string &k, KEYTYPE t)
    : Name(k), Type(t)
    {
        switch(t)
        {
            default:
            case KT_DIGIT:
                Value = (int32_t)0;
                break;
            
            case KT_WORD:
            case KT_STRING:
                Value = std::string();
                break;
                
            case KT_BOOL:
                Value = false;
                break;
        }
    }
    
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
            printf("%s %s\n", Name.c_str(), e.what());
        }
        return t;
    }
};

typedef std::vector<Key> KeyList;

bool ParseIniFile(std::string iniFile, KeyList *lst);


}
}

#endif // INI_H_INCLUDED
