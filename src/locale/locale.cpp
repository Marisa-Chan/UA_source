#include "locale.h"

#include "world/consts.h"
#include "system/fsmgr.h"
#include "utils.h"

namespace Locale {

std::string Text::_localeName;
std::vector<std::string> Text::_localeStrings(World::CVLocaleStringsNumber);

void Text::SetLangDefault()
{
    for( std::string &s : _localeStrings )
        s.clear();

    _localeName = "default";
}

std::string Text::Get(int32_t id, const std::string &def)
{
    const std::string &tmp = _localeStrings.at(id);
    if ( tmp.empty() ) // || !StriCmp(tmp, "<>") )
        return def;
    return tmp;
}

bool Text::LngFileLoad(const std::string &filename)
{
    FSMgr::FileHandle fil = uaOpenFile(filename, "r");
    if ( !fil.OK() )
        return false;

    bool multiline = false;
    int32_t strid = -1;
    std::string buf;
    
    while (fil.ReadLine(&buf))
    {
        size_t lend = buf.find_first_of("\n\r");
        if (lend != std::string::npos)
            buf.erase(lend);
        
        if (multiline && strid != -1)
        {
            multiline = false;

            if ( buf.back() == '\\' ) // next line too
                multiline = true;
            
            std::replace(buf.begin(), buf.end(), '\\', '\n');
            _localeStrings.at(strid) += buf;
        }
        else
        {
            strid = -1;
            multiline = false;
            
            size_t pos1 = buf.find_first_not_of(" \t");
            if (pos1 != std::string::npos)
            {
                size_t pos2 = buf.find_first_of("= \t", pos1);
                if (pos2 != std::string::npos)
                {
                    strid = std::stol(buf.substr(pos1, pos2 - pos1), NULL, 0);
                    
                    if (strid < 0 || strid >= (int32_t)_localeStrings.size() )
                        strid = -1;
                }
                
                if (strid != -1)
                {
                    pos1 = buf.find_first_of("=", pos2);
                    if (pos1 != std::string::npos && pos1 + 1 < buf.size())
                    {
                        pos1 += 1; // Next after '='
                        pos2 = buf.find_first_not_of(" \t", pos1);
                        if (pos2 != std::string::npos)
                            pos1 = pos2;
                        
                        if ( buf.back() == '\\' ) // Multiline
                            multiline = true;
                        
                        std::string &s = _localeStrings.at(strid);
                        s = buf.substr(pos1);
                        std::replace(s.begin(), s.end(), '\\', '\n');
                        
                        if (!StriCmp(s, "<>"))
                            s.clear();
                    }
                }
            }
        }
    }
    return true;
}

}