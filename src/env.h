#ifndef ENV_ENGINE_H_INCLUDED
#define ENV_ENGINE_H_INCLUDED

#include <string>
#include <list>
#include "ini.h"

namespace Common
{
    
struct TKVPair
{
    std::string Name;
    std::string Value;
};

class TEnv
{
friend bool Ini::ParseIniFile(std::string iniFile, Ini::KeyList *lst);
friend bool Ini::ParseIniFile(std::string iniFile, Ini::PKeyList *lst);

public:
    void AddGlobalIniKey(const std::string &str);
    
    std::string GetPrefix(const std::string &name);
    std::string SetPrefix(const std::string &name, const std::string &value);
    
    std::string ApplyPrefix(const std::string &src);
    
private:
    TEnv() {};
    
public:
    static TEnv Instance;
 
private:
    std::list<TKVPair> _kvPairs;
    std::list<std::string> _predefinedIniKeys;
};

static constexpr TEnv &Env = TEnv::Instance;
        
        
}


#endif