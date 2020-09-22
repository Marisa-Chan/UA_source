#ifndef ZNDNET_NETSESSION_H_INCLUDED
#define ZNDNET_NETSESSION_H_INCLUDED

#include <string>
#include <unordered_map>
#include <vector>

#include "netuser.h"

namespace ZNDNet
{

struct NetSession
{
    uint64_t    ID;
    bool        lobby;
    std::string name;
    NetUserList users;
    NetUser    *lead;
    std::string password;
    bool        open;

    uint32_t     max_players;

    uint64_t  orphanedTimer;
    uint64_t  closeTimer;

    NetSession()
    {
        clear();
    }

    void Init(uint64_t _ID, const std::string &_name, bool _lobby = false)
    {
        ID = _ID;
        name = _name;
        users.clear();
        password.clear();
        lead = NULL;
        lobby = _lobby;
        max_players = 0;
        closeTimer = 0;
        open = true;
    }

    void clear()
    {
        ID = 0;
        name.clear();
        users.clear();
        password.clear();
        lead = NULL;
        lobby = false;
        max_players = 0;
        orphanedTimer = 0;
        closeTimer = 0;
        open = true;
    }

    bool HasSlot()
    {
        if (!max_players || users.size() < max_players)
            return true;

        return false;
    }
};

typedef std::unordered_map<uint64_t, NetSession *> NetSessionMap;


struct SessionInfo
{
    uint64_t    ID;
    std::string name;
    bool        pass;
    uint32_t    players;
    uint32_t    max_players;

    SessionInfo& operator= (const SessionInfo& x)
    {
        ID = x.ID;
        name = x.name;
        pass = x.pass;
        players = x.players;
        max_players = x.max_players;
        return *this;
    };
};
typedef std::vector<SessionInfo> SessionInfoVect;

}

#endif
