#include "env.h"
#include "utils.h"

namespace Common
{
    
TEnv TEnv::Instance;

void TEnv::AddGlobalIniKey(const std::string &str)
{
    _predefinedIniKeys.push_back(str);
}

std::string TEnv::GetPrefix(const std::string &var)
{
    for( auto &x : _kvPairs )
    {
        if ( !StriCmp(x.Name, var ) )
            return x.Value;
    }

    return std::string();
}

std::string TEnv::SetPrefix(const std::string &name, const std::string &value)
{
    for ( TKVPair &x : _kvPairs)
    {
        if ( !StriCmp(x.Name, name) )
        {
            std::string tmp = x.Value;
            x.Value = value;
            return tmp;
        }
    }

    _kvPairs.push_front( {name, value} );
    
    return std::string();
}

std::string TEnv::ApplyPrefix(const std::string &src)
{
    std::string buf = src;

    int stpos = 0;
    if ( buf.length() > 2 && buf[1] == ':' ) //disk name, e.g. C:/D:/Y:/...
        stpos = 2;

    size_t fndPos = buf.find(':', stpos);

    if ( fndPos == std::string::npos )
        return buf;

    std::string kword = buf.substr(0, fndPos);

    for (const TKVPair &p : _kvPairs)
    {
        if ( !StriCmp(p.Name, kword) )
        {
            if (p.Value.back() == ':') // if value has colon - do not add slash
                return ApplyPrefix( p.Value + buf.substr(fndPos + 1) );
            else
                return ApplyPrefix( p.Value + '/' + buf.substr(fndPos + 1) );
        }
    }

    buf[fndPos] = '/';

    return buf;
}
    
}