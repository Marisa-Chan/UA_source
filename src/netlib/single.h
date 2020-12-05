#ifndef ZNDNET_SINGLE_H_INCLUDED
#define ZNDNET_SINGLE_H_INCLUDED


namespace ZNDNet
{

class ZNDSingle: public ZNDNet
{
public:
    ZNDSingle(const std::string &servstring);

    void Start(const std::string &name, uint16_t port, const std::string &sname, uint32_t max_players);

    void ShowSession(bool show);
    void CloseSession(uint32_t closeTime);

    void KickUser(uint64_t _ID);

    bool GetUsers(UserInfoVect &dst);
    bool GetUser(UserInfo &dst, const char *name);
    bool GetUser(UserInfo &dst, uint64_t _ID);

    void SendData(uint64_t to, void *data, uint32_t sz, uint8_t flags = 0, uint8_t channel = CHANNEL_USR);
    void BroadcastData(void *data, uint32_t sz, uint8_t flags = 0, uint8_t channel = CHANNEL_USR);
    
    uint64_t GetMyID();
    std::string GetMyName();
    
    uint64_t GetMySesID();

protected:
    static int _RecvThread(void *data);
    static int _SendThread(void *data);
    static int _UpdateThread(void *data);

    void DisconnectUser(NetUser *usr, uint8_t type);

    Pkt *Recv_PreparePacket(InRawPkt *pkt);
    void ProcessSystemPkt(Pkt *pkt);
    void ProcessRegularPkt(Pkt *pkt);

    void InterprocessUpdate();

    void UserToSession(NetUser *usr, NetSession *ses);

    void CloseSession();

// Data
public:

protected:
    NetUser     ME;
    NetUsersPack users;

    NetSession  session;

    std::atomic_uint_fast32_t eStatus;
};

}

#endif
