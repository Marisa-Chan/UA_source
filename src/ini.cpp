#include "ini.h"
#include "env.h"
#include "utils.h"

namespace Common {
namespace Ini {
    
Key::Key(const std::string &k, KEYTYPE t, nonstd::any v)
: Name(k), Type(t), Value(v)
{}


Key::Key(const std::string &k, KEYTYPE t)
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

inline Key& Deref(Key &a)
{
    return a;
}

inline Key& Deref(Key *a)
{
    return *a;
}

template <typename T>
void ParseLine(std::string line, std::vector<T> *lst)
{    
    size_t endp = line.find_first_of(";\r\n");
    
    if (endp != std::string::npos)
        line.erase(endp);
    
    Stok splt(line, "= \t");
    std::string token;

    if ( splt.GetNext(&token) )
    {
        for ( T &v : *lst )
        {
            if ( !StriCmp(Deref(v).Name, token) )
            {
                std::string tmp;
                switch ( Deref(v).Type )
                {
                case KT_DIGIT:
                    if ( splt.GetNext(&tmp) )
                        Deref(v).Value = (int32_t)std::stol(tmp, NULL, 0);
                    break;

                case KT_BOOL:
                    if ( splt.GetNext(&tmp) )
                    {
                        if ( StrGetBool(tmp) )
                            Deref(v).Value = true;
                        else
                            Deref(v).Value = false;
                    }
                    break;

                case KT_WORD:
                    if ( splt.GetNext(&tmp) )
                        Deref(v).Value = std::string(tmp);
                    break;

                case KT_STRING:
                    if ( splt.GetNext(&tmp, "=") )
                        Deref(v).Value = std::string(tmp);
                    break;
                    
                default:
                    break;
                }
            }
        }
    }
}

bool ParseIniFile(std::string iniFile, KeyList *lst)
{
    if ( iniFile.empty() )
        return false;

    FSMgr::FileHandle *fil = FSMgr::iDir::openFile(iniFile, "r");

    if ( !fil )
        return false;

    std::string buf;
    while ( fil->ReadLine(&buf) )
        ParseLine(buf, lst);
    
    delete fil;
    
    for( const std::string &str : Env._predefinedIniKeys )
        ParseLine<Key>(str, lst);

    return true;
}

bool ParseIniFile(std::string iniFile, PKeyList *lst)
{
    if ( iniFile.empty() )
        return false;

    FSMgr::FileHandle *fil = FSMgr::iDir::openFile(iniFile, "r");

    if ( !fil )
        return false;

    std::string buf;
    while ( fil->ReadLine(&buf) )
        ParseLine(buf, lst);
    
    delete fil;
    
    for( const std::string &str : Env._predefinedIniKeys )
        ParseLine<Key *>(str, lst);

    return true;
}

}
}