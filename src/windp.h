#ifndef WINDP_H_INCLUDED
#define WINDP_H_INCLUDED

#include <string>
#include "nucleas.h"
#include "network.h"
#include "lstvw.h"
#include "netlib/zndNet.h"

enum FREE_FRACTION
{
    FREE_FRACTION_RESISTANCE = 1,
    FREE_FRACTION_GHORKOV = 2,
    FREE_FRACTION_MIKO = 4,
    FREE_FRACTION_TAER = 8
};


struct ProviderStruct
{
    std::string name;
    int type;
    //guid or other

    void clear()
    {
        name = "";
        type = 0;
    }
};

struct windp_getNameMsg
{
    std::string name;
    uint8_t id;
};

struct windp_openSessionMsg
{
    const char *name;
    int maxplayers;
};

struct windp_createPlayerMsg
{
    uint32_t flags;
    const char *name;
    uint32_t data1;
    uint32_t data2;
};

struct windp_arg79
{
    int mode;
    uint32_t ID;
    std::string name;
    uint32_t flags;
};

struct windp_recvMsg
{
    std::vector<uint8_t> _data;
    void *data;
    uint32_t size;
    uint32_t cast;
    uint32_t msgType;
    std::string senderName;
};

struct windp_arg82
{
    const char *senderID;
    int senderFlags;
    char *receiverID;
    int receiverFlags;
    int guarant;
};

struct windp_arg87
{
    char callSIGN[64];
    char isHoster;
    char isClient;
};




class NC_STACK_ypaworld;
class UserData;

void yw_HandleNetMsg(NC_STACK_ypaworld *yw);


void yw_CheckCRCs(NC_STACK_ypaworld *yw);


int yw_DestroyPlayer(NC_STACK_ypaworld *yw, const char *playerName);



class netgamelst: public GuiBase
{
public:
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    char field_1DC[64];
    int field_21C;
};

extern netgamelst netgame_wnd;

void sb_0x451034__sub6(NC_STACK_ypaworld *yw);
void ypaworld_func64__sub7__sub5(NC_STACK_ypaworld *yw, InputState *inpt);




class NC_STACK_windp: public NC_STACK_network
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();

    virtual size_t SetMode(bool hosting);
    size_t GetMode();
    
    void SetWantedName(const std::string &name);
    std::string GetMyName();
    std::string GetCurrentSessionName();
    
    // Only for client
    bool Connect(const std::string &connString);
    bool IsConnected();
    bool HasLobby();
    bool IsJoined();
    
    
    // Only for host
    bool Host(const std::string &gameName, int playerNum);
    

    virtual size_t EnumSessions(IDVPair *stak);
    virtual size_t GetSessionName(windp_getNameMsg *arg);
    virtual size_t JoinSession(const std::string &stak);
    virtual size_t CreateSession(windp_openSessionMsg *os);
    virtual size_t GetSessionData(IDVPair *stak);
    virtual size_t CloseSession(IDVPair *stak);
    virtual size_t GetSessionStatus();
    virtual size_t SetSessionName(const char *sessName);
    

    virtual size_t CreatePlayer(windp_createPlayerMsg *pl);
    virtual size_t DeletePlayer(const char *playerName);
    virtual size_t EnumPlayers(IDVPair *stak);
    virtual size_t GetPlayerData(windp_arg79 *stak);

    bool Send(yw_arg181 *arg);
    bool Recv(windp_recvMsg *recv);
    virtual size_t FlushBuffer(windp_arg82 &stak);

    virtual size_t GetCaps(IDVPair *stak);
    virtual size_t LockSession(int *);
    virtual size_t Reset(IDVPair *stak);
    virtual size_t CountPlayers(IDVPair *stak);

    virtual bool GetRemoteStart(windp_arg87 *arg);

    virtual size_t GetLocalName(IDVPair *stak);
    virtual void   SetVersion(const std::string &arg);
    virtual size_t GetProvType();

    virtual size_t GetStats(int *);

    NC_STACK_windp() {
        _clear();
        //memset(&wdp_intern, 0, sizeof(wdp_intern));
    };
    virtual ~NC_STACK_windp() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_windp();
    };

    enum
    {
        MSG_T_UNKNOWN = 0,
        MSG_T_PLAYER = 1,
        MSG_T_ALL = 2
    };

    enum
    {
        SESSION_STATUS_NONE = 0,
        SESSION_STATUS_JOINED = 1,
        SESSION_STATUS_HOSTING = 2
    };
    
    enum
    {
        CONF_CONN_WAIT = 5000,
        CONF_USERS_GET_TIME = 1000,
        CONF_SESS_GET_TIME = 1000,
    };

    bool init();
    void deinit();
    int getNumPlayers();

protected:
    enum
    {
        MAX_PROVIDERS = 2, //64,
    };

    void _clear();
    
    void ReloadUsersList();
    void ReloadSessionsList();
    uint64_t GetUserID(const std::string &name);
    std::string GetUserName(uint64_t id);


    //Data
public:
    static const Nucleus::ClassDescr description;

    ProviderStruct providers[MAX_PROVIDERS];

    int connType;

    ZNDNet::ZNDClient *zcli;
    ZNDNet::ZNDSingle *zhost;
    ZNDNet::ZNDNet    *zcon;
    bool _hasLobby;
    uint64_t    _myID;
    std::string _myName;
    ZNDNet::UserInfoVect _rawUsers;
    ZNDNet::SessionInfoVect _rawSessions;
    uint32_t _nextUsersGet;
    uint32_t _nextSessionsGet;
    uint64_t _sesID;
    std::string _sesName;
    /*  _DWORD dword1504;
      IDirectPlay3 *dplay3;
      int providerID;
      int connType;
      int num_sessions;
      windp_t2 sessions[64];
      int act_sess;
      windp_t2 own_sess;
      int sess_joined;
      int sess_created;*/
    std::string version_ident;
    int version_check;
    /*windp_t3 players[4];
    int num_players;*/
    char *norm_block;
    //char *big_block;
    int norm_size;
    /*int big_size;
    int guaranteed;*/
    char *sndBuff;
    int sndBuff_size;
    int sndBuff_off;
    int guaranteed_md;
    /*nlist recv_list;
    nlist send_list;
    HANDLE mevent;*/
    int debug;
};

#endif // WINDP_H_INCLUDED
