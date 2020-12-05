#include "zndNet.h"

namespace ZNDNet
{

ZNDClient::ZNDClient(const std::string &servstring)
: ZNDNet(servstring)
{
    cSessionsReqTimeNext = 0;
    cME.status = NetUser::STATUS_DISCONNECTED;
    eStatus = 0;
}

Pkt *ZNDClient::Recv_PreparePacket(InRawPkt *pkt)
{
    if (!pkt->Parse())
    {
        // Incorrect packet
        delete pkt;
        return NULL;
    }


    if (pkt->hdr.flags & PKT_FLAG_SYSTEM) // System message
    {
        return new Pkt(pkt, NULL);
    }
    else
    {
        if (pkt->hdr.flags & PKT_FLAG_PART) // incomplete data, do assembly
        {
            AddrSeq ipseq(pkt->addr, pkt->hdr.seqid);

            InPartedPkt *parted = NULL;

            for (PartedList::iterator it = pendingPkt.begin(); it != pendingPkt.end(); it++)
            {
                if ((*it)->ipseq == ipseq)
                {
                    parted = *it;
                    break;
                }
            }

            if (!parted)
            {
                parted = new InPartedPkt(ipseq, pkt->hdr.fsize, pkt->hdr.flags, pkt->hdr.uchnl);
                pendingPkt.push_back(parted);
            }

            if ( parted->Feed(pkt, ttime.GetTicks()) ) // Is complete?
            {
                /*if (parted->retry != RETRY_GARANT)
                    printf("Recovered packet! %d\n", parted->ipseq.seq);*/

                pendingPkt.remove(parted);
                return new Pkt(parted, NULL);
            }

            // if not complete -> return NULL
        }
        else // not parted packet -> make it
        {
            return new Pkt(pkt, NULL);
        }
    }

    return NULL;
}

void ZNDClient::ProcessSystemPkt(Pkt* pkt)
{
    if (!pkt)
        return;

    if (pkt->datasz < 1 )
        return;

    MemReader rd(pkt->data + 1, pkt->datasz - 1);

    switch(pkt->data[0])
    {
        case SYS_MSG_CONNECTED:
            {
                if (cME.status == NetUser::STATUS_CONNECTING)
                {
                    cServHasLobby = rd.readU8() != 0;
                    uint64_t ID = rd.readU64();
                    std::string nm;
                    rd.readSzStr(nm);
                    cME.name = nm;
                    cME.ID = ID;
                    cME.status = NetUser::STATUS_CONNECTED;

                    Events_Push( new EventNameID(EVENT_CONNECTED, cServHasLobby, cME.name, cME.ID) );

                    cTimeOut = ttime.GetTicks() + TIMEOUT_USER;
                }
            }
            break;

        case SYS_MSG_DISCONNECT:
            {
                eStatus = 0;
                threadsEnd = true;
                Events_Push( new Event(EVENT_DISCONNECT, ERR_DISCONNECT_SERVER) );
            }
            break;

        case SYS_MSG_SES_JOIN:
            {
                cLeader = (rd.readU8() == 1);
                cME.sesID = rd.readU64();
                rd.readSzStr( cJoinedSessionName );
                eStatus &= ~(FLAGS_USERS_LIST_GET | FLAGS_USERS_LIST_UPD);
                eStatus |= FLAGS_SESSION_JOINED;

                Events_Push( new EventNameID(EVENT_SESSION_JOIN, 0, cJoinedSessionName, cME.sesID) );
            }
            break;

        case SYS_MSG_SES_LEAVE:
            {
                cLeader = false;
                cME.sesID = 0;
                eStatus &= ~(FLAGS_SESSION_JOINED | FLAGS_USERS_LIST_GET | FLAGS_USERS_LIST_UPD);

                Events_Push( new Event(EVENT_SESSION_LEAVE, rd.readU8()) );
            }
            break;

        case SYS_MSG_SES_LEAD:
            {
                if (cME.sesID)
                {
                    cLeader = (rd.readU8() == 1);
                    Events_Push( new Event(EVENT_SESSION_LEAD, cLeader ? 1 : 0) );
                }
            }
            break;

        case SYS_MSG_PING:
            if (rd.size() == 4)
            {
                uint32_t seq = rd.readU32();
                RefDataWStream *rfdat = RefDataWStream::create();
                rfdat->writeU8(SYS_MSG_PING);
                rfdat->writeU32(seq);

                Send_PushData( new SendingData(cServAddress, 0, rfdat, PKT_FLAG_SYSTEM) );

                cTimeOut = ttime.GetTicks() + TIMEOUT_USER;
            }
            break;

        case SYS_MSG_DELIVERED:
            if (rd.size() == 4)
            {
                uint32_t seq = rd.readU32();
                ConfirmReceive( AddrSeq(cServAddress, seq) );
            }
            break;

        case SYS_MSG_RETRY:
            if (rd.size() == 12)
            {
                uint32_t seq = rd.readU32();
                uint32_t from = rd.readU32();
                uint32_t upto = rd.readU32();
                ConfirmRetry( AddrSeq(cServAddress, seq), from, upto );
            }
            break;

        case SYS_MSG_SES_CLOSE:
            if (rd.size() == sizeof(uint32_t))
            {
                if (eStatus & FLAGS_SESSION_JOINED)
                    Events_Push( new Event(EVENT_SESSION_END, rd.readU32()) );
            }
            break;

        case SYS_MSG_CONNERR:
            if (cME.status == NetUser::STATUS_CONNECTING && rd.size() == 1)
            {
                uint8_t type = rd.readU8();
                if (type == ERR_CONN_FULL || type == ERR_CONN_NAME)
                {
                    threadsEnd = true; //Go to
                    Events_Push( new Event(EVENT_CONNERR, type) );
                }
            }
            break;

        default:
            break;
    }
}

void ZNDClient::ProcessRegularPkt(Pkt* pkt)
{
    if (!pkt)
        return;

    if (pkt->datasz < 1 )
        return;

    MemReader rd(pkt->data + 1, pkt->datasz - 1);

    switch(pkt->data[0])
    {
        case USR_MSG_LIST_GAMES: //New sessions list arrived -> mark it's
            {
                if (rd.size() < 4)
                    return;

                uint32_t numSessions = rd.readU32();

                eStatus &= (~FLAGS_SESSIONS_LIST_GET);

                cSessions.resize(numSessions);

                for(uint32_t i = 0; i < numSessions; i++)
                {
                    SessionInfo *inf = &cSessions[i];
                    inf->ID = rd.readU64();
                    inf->players = rd.readU32();
                    inf->max_players = rd.readU32();
                    inf->pass = (rd.readU8() > 0 ? true : false);
                    rd.readSzStr(inf->name);
                }

                eStatus |= FLAGS_SESSIONS_LIST_GET | FLAGS_SESSIONS_LIST_UPD;
                Events_Push( new Event(EVENT_SESSION_LIST, 0) );
            }
            break;

        case USR_MSG_SES_USERLIST:
            {
                if (rd.size() < 4)
                    return;

                uint32_t numUsers = rd.readU32();

                eStatus &= (~FLAGS_USERS_LIST_GET);

                cUsers.resize(numUsers);

                for(uint32_t i = 0; i < numUsers; i++)
                {
                    UserInfo *inf = &cUsers[i];
                    inf->ID = rd.readU64();
                    rd.readSzStr(inf->name);
                }

                eStatus |= FLAGS_USERS_LIST_GET | FLAGS_USERS_LIST_UPD;
                Events_Push( new Event(EVENT_USER_LIST, 0) );
            }
            break;

        case USR_MSG_SES_USERJOIN: //Somebody join to session where you
            {
                UserInfo inf;
                inf.lead = false;

                inf.ID = rd.readU64();
                rd.readSzStr(inf.name);

                cUsers.push_back(inf);

                eStatus |= FLAGS_USERS_LIST_UPD;
                Events_Push( new EventNameID(EVENT_USER_ADD, 0, inf.name, inf.ID) );
            }
            break;

        case USR_MSG_SES_USERLEAVE: //Somebody leav session where you
            {
                uint64_t UID = rd.readU64();
                uint8_t tp = rd.readU8();

                for(UserInfoVect::iterator it = cUsers.begin(); it != cUsers.end(); it++)
                {
                    if (it->ID == UID)
                    {
                        Events_Push( new EventNameID(EVENT_USER_LEAVE, tp, it->name, it->ID) );
                        cUsers.erase(it);
                        eStatus |= FLAGS_USERS_LIST_UPD;
                        break;
                    }
                }
            }
            break;

        case USR_MSG_DATA:
            if (rd.size() > 21)
            {
                uint64_t from = rd.readU64();
                uint8_t  cast = rd.readU8();
                uint64_t to = rd.readU64();
                uint32_t sz = rd.readU32();

                if (rd.size() == sz + rd.tell())
                {
                    if ( (cast == 0 && to == cME.ID) ||
                         (cast == 1 && to == cME.sesID) )
                    {
                        Events_Push( new EventData(EVENT_DATA, pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC),
                                                   from, (cast == 1 ? true : false),  to,
                                                   sz,
                                                   pkt->data + 1 + rd.tell(),
                                                   pkt->uchnl ) );
                    }
                }
            }
            break;

        default:
            break;
    }
}


void ZNDClient::Start(const std::string &name, const IPaddress &addr)
{
    if (cME.status != NetUser::STATUS_DISCONNECTED)
        return;

    mode = MODE_CLIENT;
    sock = SDLNet_UDP_Open(0);

    cServAddress = addr;
    cME.name = name;
    cME.status = NetUser::STATUS_CONNECTING;

    eStatus = 0;

    cSessionsReqTimeNext = 0;
    cSessionsMakeRequest = false;

    threadsEnd = false;

    cTimeOut = ttime.GetTicks() + TIMEOUT_CONNECT;

    recvThread = SDL_CreateThread(_RecvThread, "", this);
    sendThread = SDL_CreateThread(_SendThread, "", this);

    updateThread = SDL_CreateThread(_UpdateThread, "", this);

    SendConnect();
}

void ZNDClient::SendConnect()
{
    RefDataWStream *rfdata = RefDataWStream::create();
    rfdata->writeU8(SYS_MSG_HANDSHAKE);
    rfdata->writeU8(servString.size());
    rfdata->writeU8(cME.name.size());
    rfdata->writeStr(servString);
    rfdata->writeStr(cME.name);

    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}


void ZNDClient::RequestGamesList()
{
    RefDataWStream *rfdata = RefDataWStream::create();
    rfdata->writeU8(SYS_MSG_LIST_GAMES);
    rfdata->writeU64(cME.ID);

    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}

uint8_t ZNDClient::GetStatus()
{
    return cME.status;
}

bool ZNDClient::GetSessions(SessionInfoVect &dst)
{
    if ( !cME.IsOnline() )
        return false;

    if ( !(eStatus & FLAGS_SESSIONS_LIST_GET) )
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        if (eStatus & FLAGS_SESSIONS_LIST_GET)
        {
            eStatus &= (~FLAGS_SESSIONS_LIST_UPD);
            dst = cSessions;
            res = true;
        }
        SDL_UnlockMutex(eSyncMutex);
    }

    return res;
}

void ZNDClient::RequestSessions()
{
    if ( !cME.IsOnline() )
        return;

    cSessionsMakeRequest = true;
}



void ZNDClient::CreateSession(const std::string &name, const std::string &pass, uint32_t max_players)
{
    if ( !cME.IsOnline() )
        return;

    if ( !SessionCheckName(name) || ( pass.size() > 0 && !SessionCheckPswd(pass) ))
        return;

    RefDataWStream *rfdata = RefDataWStream::create();
    rfdata->writeU8(SYS_MSG_SES_CREATE);
    rfdata->writeSzStr(name);
    rfdata->writeU32(max_players);
    rfdata->writeSzStr(pass);

    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}

void ZNDClient::JoinSession(uint64_t SID, const std::string &pass)
{
    if ( !cME.IsOnline() )
        return;

    RefDataWStream *rfdata = RefDataWStream::create();
    rfdata->writeU8(SYS_MSG_SES_JOIN);
    rfdata->writeU64(SID);
    rfdata->writeSzStr(pass);

    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}


bool ZNDClient::GetUsers(UserInfoVect &dst)
{
    if ( !cME.IsOnline() )
        return false;

    if ( !(eStatus & FLAGS_USERS_LIST_GET) )
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        if (eStatus & FLAGS_USERS_LIST_GET)
        {
            eStatus &= (~FLAGS_USERS_LIST_UPD);
            dst = cUsers;
            res = true;
        }
        SDL_UnlockMutex(eSyncMutex);
    }

    return res;
}


bool ZNDClient::GetUser(UserInfo &dst, const char *name)
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_USERS_LIST_GET) )
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        if (eStatus & FLAGS_USERS_LIST_GET)
        {
            for(UserInfoVect::iterator it = cUsers.begin(); it != cUsers.end(); it++)
            {
                if (strcmp(it->name.c_str(), name) == 0)
                {
                    dst = *it;
                    res = true;
                    break;
                }
            }
        }
        SDL_UnlockMutex(eSyncMutex);
    }

    return res;
}

bool ZNDClient::GetUser(UserInfo &dst, uint64_t _ID)
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_USERS_LIST_GET) )
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        if (eStatus & FLAGS_USERS_LIST_GET)
        {
            for(UserInfoVect::iterator it = cUsers.begin(); it != cUsers.end(); it++)
            {
                if (it->ID == _ID)
                {
                    dst = *it;
                    res = true;
                    break;
                }
            }
        }
        SDL_UnlockMutex(eSyncMutex);
    }

    return res;
}


void ZNDClient::InterprocessUpdate()
{
    uint64_t curTime = ttime.GetTicks();

    if (curTime > cTimeOut)
    {
        threadsEnd = true;

        if (cME.status == NetUser::STATUS_CONNECTING)
            Events_Push( new Event(EVENT_CONNERR, ERR_CONN_TIMEOUT) );
        else
            Events_Push( new Event(EVENT_DISCONNECT, ERR_DISCONNECT_TIMEOUT) );

        return;
    }


    if (cSessionsMakeRequest && cSessionsReqTimeNext < curTime)
    {
        cSessionsMakeRequest = false;
        cSessionsReqTimeNext = curTime + DELAY_SESS_REQ;

        RequestGamesList();
    }

    PendingCheck(); // Delete timeout packets
    ConfirmQueueCheck();
}

void ZNDClient::SendDisconnect()
{
    if ( cME.IsOnline() )
    {
        Send_Clear();

        RefDataWStream *rfdata = RefDataWStream::create(4);
        rfdata->writeU8(SYS_MSG_DISCONNECT);
        Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
    }
}

void ZNDClient::ShowSession(bool show)
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_SESSION_JOINED) || !cLeader )
        return;

    RefDataWStream *rfdata = RefDataWStream::create(4);
    rfdata->writeU8(SYS_MSG_SES_SHOW);
    rfdata->writeU8(show ? 1 : 0);
    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}


void ZNDClient::LeaveSession()
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_SESSION_JOINED) )
        return;

    RefDataWStream *rfdata = RefDataWStream::create(4);
    rfdata->writeU8(SYS_MSG_SES_LEAVE);
    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}

void ZNDClient::CloseSession(uint32_t closeTime)
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_SESSION_JOINED) || !cLeader )
        return;

    if (closeTime > 60000)
        closeTime = 60000;

    RefDataWStream *rfdata = RefDataWStream::create(8);
    rfdata->writeU8(SYS_MSG_SES_CLOSE);
    rfdata->writeU32(closeTime);
    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}

void ZNDClient::KickUser(uint64_t _ID)
{
    if ( !cME.IsOnline() || !(eStatus & FLAGS_SESSION_JOINED) || !cLeader )
        return;

    RefDataWStream *rfdata = RefDataWStream::create(16);
    rfdata->writeU8(SYS_MSG_SES_KICK);
    rfdata->writeU64(_ID);
    Send_PushData( new SendingData(cServAddress, 0, rfdata, PKT_FLAG_SYSTEM));
}


void ZNDClient::SendData(uint64_t to, void *data, uint32_t sz, uint8_t flags, uint8_t channel)
{
    if (!data || !sz)
        return;

    RefData *rfdat = USRDataGenData(cME.ID, false, to, data, sz);

    flags &= (PKT_FLAG_GARANT | PKT_FLAG_ASYNC);

    if (channel >= ZNDNET_USER_SCHNLS)
        channel = ZNDNET_USER_SCHNLS - 1;

    SendingData *snd = new SendingData(cServAddress, cME.GetSeq(), rfdat, flags);
    snd->SetChannel(0, channel);

    Send_PushData(snd);
}

void ZNDClient::BroadcastData(void *data, uint32_t sz, uint8_t flags, uint8_t channel)
{
    if (!data || !sz)
        return;

    RefData *rfdat = USRDataGenData(cME.ID, true, cME.sesID, data, sz);

    flags &= (PKT_FLAG_GARANT | PKT_FLAG_ASYNC);

    if (channel >= ZNDNET_USER_SCHNLS)
        channel = ZNDNET_USER_SCHNLS - 1;

    SendingData *snd = new SendingData(cServAddress, cME.GetSeq(), rfdat, flags);
    snd->SetChannel(0, channel);

    Send_PushData(snd);
}

uint64_t ZNDClient::GetMyID()
{
    if ( !cME.IsOnline() )
        return 0;
    return cME.ID;
}

std::string ZNDClient::GetMyName()
{
    if ( !cME.IsOnline() )
        return std::string();
    return cME.name;
}

uint64_t ZNDClient::GetMySesID()
{
    if ( !cME.IsOnline() )
        return 0;
    return cME.sesID;
}


}
