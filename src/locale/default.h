#ifndef WORLD_DEFSTR_H
#define WORLD_DEFSTR_H

#include <array>
#include <string>
#include "strid.h"

namespace Locale { 

class DefaultStrings
{
public:
    // LBL_COMMON 0
    static std::array<std::string, CMN_MAX> Commons;
    // LBL_HUDSTR 30
    static std::array<std::string, HUDSTR_MAX> HudStrings;
    // LBL_WINNAMES 50
    static std::array<std::string, WINNAME_MAX> WinNames;
    // LBL_TITLES 80
    static std::array<std::string, TITLE_MAX> Titles;
    // LBL_BRIEF 150
    static std::array<std::string, BRIEF_MAX> BriefTexts;
    // LBL_FEEDBACK 200
    static std::array<std::string, FEEDBACK_MAX> Feedbacks;
    // LBL_DIALOGS 300
    static std::array<std::string, DLG_MAX> Dialogs;
    // LBL_NETDLGS 400
    static std::array<std::string, NETDLG_MAX> NetDlgs;
    // LBL_INPUTS 500
    static std::array<std::string, INPUTS_MAX> Inputs;
    // 600
    static std::string YouPlay;
    // LBL_GLOBMAP 640
    static std::array<std::string, GLOBMAP_MAX> GlobMaps;
    // 651
    static std::string WaitingForPlayers;
    // LBL_ADVICE 700
    static std::array<std::string, ADVICE_MAX> Advices;
    // LBL_ADVICE 750
    static std::array<std::string, HELP_MAX> Helps;
    // LBL_TIPS 800
    static std::array<std::string, TIP_MAX> Tooltips;
    // LBL_KEYNAME 1000
    static std::array<std::string, KEYNAME_MAX> Keynames;
    // LBL_ADV 2400
    static std::array<std::string, ADV_MAX> Advanced;
};

}



#endif