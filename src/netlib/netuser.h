#ifndef ZNDNET_NETUSER_H_INCLUDED
#define ZNDNET_NETUSER_H_INCLUDED

#include <deque>
#include <list>
#include <string>
#include <vector>

#include "utils.h"

namespace ZNDNet
{

struct NetUser
{
    enum
    {
        STATUS_DISCONNECTED     = 0,
        STATUS_CONNECTING       = 1,
        STATUS_CONNECTED        = 2
    };

    uint64_t ID;
    std::string name;
    IPaddress addr;
    uint64_t sesID;
    uint8_t status;

    uint64_t pingTime;
    uint32_t pingSeq;
    uint64_t pongTime;
    uint32_t pongSeq;

    int32_t latence;
    uint32_t seqid;

    int32_t __idx;

    bool net;
    void *__master;

    NetUser()
    {
        ID = 0;
        name = "";
        addr.host = 0;
        addr.port = 0;
        sesID = 0;
        status = STATUS_DISCONNECTED;

        pingTime = 0;
        pingSeq = 0;
        pongTime = 0;
        pongSeq = 0;

        latence = 0;
        seqid = 0;

        __idx = -1;
        __master = NULL;

        net = false;
    };

    inline bool IsOnline()
    {
        return status == NetUser::STATUS_CONNECTED;
    };

    inline uint32_t GetSeq()
    {
        return seqid++;
    }
};

typedef std::list<NetUser *> NetUserList;
typedef std::deque<NetUser *> NetUserQueue;

class NetUsersPack
{
public:
    NetUsersPack(uint32_t allocUsers)
    : maxUsers(allocUsers)
    {
        users = new NetUser[maxUsers];

        Init();
    }

    ~NetUsersPack()
    {
        delete[] users;
    }

    NetUser *Alloc()
    {
        if (freeUsers.empty())
            return NULL;

        NetUser *usr = freeUsers.front();
        freeUsers.pop_front();

        activeUsers.push_back(usr);

        return usr;
    }

    void Free(NetUser *usr)
    {
        if (!usr || usr->__master != this)
            return;

        activeUsers.remove(usr);
        freeUsers.push_back(usr);

        usr->status = NetUser::STATUS_DISCONNECTED;
    }

    NetUser *FindByIP(const IPaddress &addr)
    {
        for(NetUserList::iterator it = activeUsers.begin() ; it != activeUsers.end(); it++)
        {
            if ( IPCMP((*it)->addr, addr) )
                return (*it);
        }

        return NULL;
    }

    NetUser *FindByID(uint64_t ID)
    {
        for(NetUserList::iterator it = activeUsers.begin() ; it != activeUsers.end(); it++)
        {
            if ( (*it)->ID == ID )
                return (*it);
        }

        return NULL;
    }

    NetUser *FindByName(const std::string &name)
    {
        for(NetUserList::iterator it = activeUsers.begin() ; it != activeUsers.end(); it++)
        {
            if ( (*it)->name.size() == name.size() )
            {
                if ( strcmp((*it)->name.c_str(), name.c_str()) == 0 )
                    return (*it);
            }
        }

        return NULL;
    }

    void Init()
    {
        activeUsers.clear();
        freeUsers.clear();

        for(uint32_t i = 0; i < maxUsers; i++)
        {
            NetUser *usr = &users[i];
            usr->__idx = i;
            usr->__master = this;

            freeUsers.push_back(usr);
        }
    }

    inline NetUserList::iterator begin()
    {
        return activeUsers.begin();
    }

    inline NetUserList::iterator end()
    {
        return activeUsers.end();
    }

    inline NetUserList::iterator erase(NetUserList::iterator &it)
    {
        NetUser *usr = *it;
        if (usr->__master == this)
        {
            freeUsers.push_back(usr);
            return activeUsers.erase(it);
        }

        return ++it;
    }

    inline uint32_t size()
    {
        return activeUsers.size();
    }

protected:
    const uint32_t maxUsers;

    NetUser     *users;
    NetUserList  activeUsers;
    NetUserQueue freeUsers;
};


struct UserInfo
{
    uint64_t    ID;
    std::string name;
    bool        lead;

    UserInfo& operator= (const UserInfo& x)
    {
        ID = x.ID;
        name = x.name;
        lead = x.lead;
        return *this;
    };

    UserInfo& operator= (const NetUser& x)
    {
        ID = x.ID;
        name = x.name;
        lead = false;
        return *this;
    };
};
typedef std::vector<UserInfo> UserInfoVect;

}

#endif
