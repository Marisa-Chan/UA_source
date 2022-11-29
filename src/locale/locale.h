#ifndef LOCALE_H
#define LOCALE_H

#include <string>
#include <vector>

#include "strid.h"
#include "default.h"


namespace Locale {
  

class Text
{
public:
    static void SetLangDefault();
    static std::string Get(int32_t id, const std::string &def);
    
    static bool LngFileLoad(const std::string &filename);
    
    
    static std::string Tooltip(uint32_t id)
    {
        return Get(LBL_TIPS + id, DefaultStrings::Tooltips[ id ]);
    }
    
    static std::string Common(uint32_t id)
    {
        return Get(LBL_COMMON + id, DefaultStrings::Commons[ id ]);
    }
    
    static std::string Advice(uint32_t id)
    {
        return Get(LBL_ADVICE + id, DefaultStrings::Advices[ id ]);
    }
    
    static std::string KeyName(uint32_t id)
    {
        return Get(LBL_KEYNAME + id, DefaultStrings::Keynames[ id ]);
    }
    
    static std::string HUD(uint32_t id)
    {
        return Get(LBL_HUDTXT + id, DefaultStrings::HudStrings[ id ]);
    }
    
    static std::string WinName(uint32_t id)
    {
        return Get(LBL_WINNAMES + id, DefaultStrings::WinNames[ id ]);
    }
    
    static std::string Title(uint32_t id)
    {
        return Get(LBL_TITLES + id, DefaultStrings::Titles[ id ]);
    }
    
    static std::string Brief(uint32_t id)
    {
        return Get(LBL_BRIEF + id, DefaultStrings::BriefTexts[ id ]);
    }
    
    static std::string Feedback(uint32_t id)
    {
        return Get(LBL_FEEDBACK + id, DefaultStrings::Feedbacks[ id ]);
    }
    
    static std::string Dialogs(uint32_t id)
    {
        return Get(LBL_DIALOGS + id, DefaultStrings::Dialogs[ id ]);
    }
    
    static std::string Netdlg(uint32_t id)
    {
        return Get(LBL_NETDLG + id, DefaultStrings::NetDlgs[ id ]);
    }
    
    static std::string Inputs(uint32_t id)
    {
        return Get(LBL_INPUTS + id, DefaultStrings::Inputs[ id ]);
    }
    
    static std::string GlobMap(uint32_t id)
    {
        return Get(LBL_GLOBMAP + id, DefaultStrings::GlobMaps[ id ]);
    }
    
    static std::string Help(uint32_t id)
    {
        return Get(LBL_HELP + id, DefaultStrings::Helps[ id ]);
    }
    
    static std::string Advanced(uint32_t id)
    {
        return Get(LBL_ADVANCED + id, DefaultStrings::Advanced[ id ]);
    }
    
    static std::string VehicleName(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_VEHICLES + id, dflt);
    }
    
    static std::string BuildingName(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_BUILDINGS + id, dflt);
    }
    
    static std::string NetBuildingName(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_NETBUILDS + id, dflt);
    }
    
    static std::string LevelName(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_LEVELNAMES + id, dflt);
    }
    
    static std::string MissionText(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_MISSIONSTXT + id, dflt);
    }
    
    static std::string ToD(uint32_t id, const std::string &dflt)
    {
        return Get(LBL_TODS + id, dflt);
    }
    
    static std::string Font()
    {
        return Get(LBL_COMMON + CMN_FONT, DefaultStrings::Commons[ CMN_FONT ]);
    }
    
    static std::string SmallFont()
    {
        return Get(LBL_COMMON + CMN_SMALLFONT, DefaultStrings::Commons[ CMN_SMALLFONT ]);
    }
    
    static const std::string &GetLocaleName()
    {
        return _localeName;
    }
    
    static void SetLocaleName(const std::string &name)
    {
        _localeName = name;
    }

protected:
    static std::string _localeName; // locale name
    static std::vector<std::string> _localeStrings;
};

}

#endif