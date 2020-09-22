#ifndef ZNDNET_H_INCLUDED
#define ZNDNET_H_INCLUDED

#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_net.h>
#include <list>
#include <string>
#include <deque>
#include <atomic>
#include <unordered_map>


#define ZNDNET_BUFF_SIZE     4096
#define ZNDNET_USERS_MAX      1024
#define ZNDNET_SES_USERS_MAX  (ZNDNET_USERS_MAX / 4)
#define ZNDNET_USER_NAME_MAX 20
#define ZNDNET_MAX_MTU       1400
#define ZNDNET_PKT_UDPSZ     8
#define ZNDNET_PKT_MAXSZ     (ZNDNET_MAX_MTU - ZNDNET_PKT_UDPSZ)
#define ZNDNET_USER_SCHNLS   2          // Minimum 2 (0 - always system synced, 1+ user channels)
#define ZNDNET_SYNC_CHANNELS (ZNDNET_USERS_MAX * ZNDNET_USER_SCHNLS)

#define ZNDNET_TUNE_MAXDELAY 10
#define ZNDNET_TUNE_MAXPKTS    16     // Maximum packets that will be polled at once
#define ZNDNET_TUNE_SEND_DELAY   1      // millisecond
#define ZNDNET_TUNE_SEND_MAXDATA 13107  // how many bytes can be sent before force delay (13107 ~bytes per 1ms on 100Mbit/s)
// High load
//#define ZNDNET_TUNE_RECV_PKTS    64
//#define ZNDNET_TUNE_SEND_DELAY   0
//#define ZNDNET_TUNE_SEND_MAXDATA 131070

#include "errcode.h"
#include "types.h"
#include "tick.h"
#include "events.h"
#include "addrseq.h"
#include "sendstorage.h"
#include "netuser.h"
#include "netsession.h"
#include "inpacket.h"
#include "utils.h"




namespace ZNDNet
{

enum MODE
{
    MODE_UNKNOWN,
    MODE_SERVER,
    MODE_CLIENT,
    MODE_SINGLE
};

// Client flags
enum
{
    FLAGS_SESSIONS_LIST_GET = 0x1,
    FLAGS_SESSIONS_LIST_UPD = 0x2,
    FLAGS_USERS_LIST_GET    = 0x4,
    FLAGS_USERS_LIST_UPD    = 0x8,
    FLAGS_SESSION_JOINED    = 0x10,
};

// PACKETS
enum
{
    // user<->server manipulations
    SYS_MSG_HANDSHAKE    = 1,
    SYS_MSG_CONNECTED    = 2,
    SYS_MSG_DISCONNECT   = 3,
    SYS_MSG_PING         = 5,
    SYS_MSG_DELIVERED    = 7,
    SYS_MSG_RETRY        = 8,
    SYS_MSG_LIST_GAMES   = 0x30,
    SYS_MSG_SES_JOIN     = 0x40, //Server->User if joined (or create). User->Server for request for join
    SYS_MSG_SES_LEAVE    = 0x41, //User->Server request. Server->User kick/disconnect
    SYS_MSG_SES_LEAD     = 0x42, //Server->User
    SYS_MSG_SES_CREATE   = 0x43, //User->Server
    SYS_MSG_SES_SHOW     = 0x44, //User->Server
    SYS_MSG_SES_KICK     = 0x45, //User->Server
    SYS_MSG_SES_CLOSE    = 0x46, //User->Server, Server->User
    SYS_MSG_SES_ERR      = 0x4F,
    SYS_MSG_CONNERR      = 0x81,

    // server->user big answers, user<->user info/manipulations
    USR_MSG_DATA          = 0x10,
    USR_MSG_LIST_GAMES    = 0x30, //Server->User
    USR_MSG_SES_USERJOIN  = 0x40, //Broadcasting Server->[users]
    USR_MSG_SES_USERLEAVE = 0x41, //Broadcasting Server->[users]
    USR_MSG_SES_USERLIST  = 0x42, //Server->User (list users in session)
};






class ZNDNet
{
// Methods
public:
    ~ZNDNet();

    Event *Events_Pop();
    void   Events_ClearByType(uint32_t type);
    void   Events_Clear();
    Event *Events_PeekByType(uint32_t type);
    Event *Events_WaitForMsg(uint32_t type, uint32_t time = 0);

    void   Stop(); // Don't call it from internal threads!

    virtual void ShowSession(bool show) = 0;
    virtual void CloseSession(uint32_t closeTime) = 0;

    virtual void KickUser(uint64_t _ID) = 0;

    virtual bool GetUsers(UserInfoVect &dst) = 0;
    virtual bool GetUser(UserInfo &dst, const char *name) = 0;
    virtual bool GetUser(UserInfo &dst, uint64_t _ID) = 0;

    virtual void SendData(uint64_t to, void *data, uint32_t sz, uint8_t flags = 0, uint8_t channel = CHANNEL_USR) = 0;
    virtual void BroadcastData(void *data, uint32_t sz, uint8_t flags = 0, uint8_t channel = CHANNEL_USR) = 0;


    bool SessionCheckName(const std::string &name);
    bool SessionCheckPswd(const std::string &pswd);

protected:
    ZNDNet(const std::string &servstring);

    //For receive thread
    void Recv_PushInRaw(InRawPkt *inpkt);
    InRawPkt *Recv_PopInRaw();
    void Recv_Clear();

    //For sending thread
    void Send_PushData(SendingData *data);
    void Send_RetryData(SendingData *data, size_t from = 0, size_t to = 0, bool decr = true);
    void Send_Clear(const IPaddress &addr);
    void Send_Clear();

    void ConfirmQueueCheck();
    void ConfirmReceive(AddrSeq _seq);
    void PendingCheck();
    void ConfirmRetry(AddrSeq _seq, uint32_t from, uint32_t to);

    void Confirm_Clear(const IPaddress &addr);
    void Confirm_Clear();
    void Pending_Clear(const IPaddress &addr);
    void Pending_Clear();

    RefData *USRDataGenData(uint64_t from, bool cast, uint64_t to, void *data, uint32_t sz);

    void SendDelivered(uint32_t _seqid, const IPaddress &addr);
    void SendRetry(uint32_t _seqid, const IPaddress &addr, uint32_t nextOff, uint32_t upto);

    static uint8_t PrepareOutPacket(SendingData &dat, UDPpacket &out);


    void SrvSendConnected(NetUser *usr, bool multisession);
    void SrvSendConnErr(const IPaddress &addr, uint8_t type);

    void SrvSendSessionJoin(NetUser *usr, NetSession *ses, bool leader);

    void SrvSessionBroadcast(NetSession *ses, RefData *dat, uint8_t flags, uint8_t chnl = 0, NetUser *from = NULL);

    RefData *SrvDataGenUserJoin(NetUser *usr);
    RefData *SrvDataGenUserList(NetSession *ses);
    RefData *SrvDataGenUserLeave(NetUser *usr, uint8_t type);

    void SrvSendPing(NetUser *usr);
    void SrvSendDisconnect(NetUser *usr);


    uint64_t GenerateID();
    void CorrectName(std::string &_name);
    SendingData *MkSendingData(NetUser *usr, RefData *data, uint8_t flags, uint32_t chnl = 0);

// Events
    void Events_Push(Event *evnt);

// Data
public:

protected:
    int         mode;
    std::string servString;
    UDPsocket   sock;


    volatile bool threadsEnd; //Run threads
    // In Raw packets
    SDL_Thread   *recvThread;

    InRawList   recvPktList;
    SDL_mutex  *recvPktListMutex;
    ////

    // Sending packets
    SDL_Thread   *sendThread;

    SendingList   sendPktList;
    SDL_mutex  *sendPktListMutex; //Only for pushing to the end

    SendingList   confirmQueue;
    SDL_mutex  *confirmQueueMutex;

    SDL_mutex  *sendModifyMutex; //Massive list changes, stop send iterations.

    ////

    SDL_Thread *updateThread;


    PartedList  pendingPkt;

    Tick64       ttime;




// External sync
    SDL_mutex  *eSyncMutex;

    EventList eEventList;
    int32_t eEventDataSize;
    SDL_mutex  *eEventMutex;
    std::atomic_uint_fast32_t eEventWaitLock;
    std::atomic_uint_fast32_t eEventNextID;

};

}

#include "client.h"
#include "server.h"
#include "single.h"

#endif // ZNDNET_H_INCLUDED
