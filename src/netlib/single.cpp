#include "zndNet.h"

namespace ZNDNet
{
ZNDSingle::ZNDSingle(const std::string &servstring)
: ZNDNet(servstring), users(ZNDNET_SES_USERS_MAX)
{
    eStatus = 0;
}


void ZNDSingle::Start(const std::string &name, uint16_t port, const std::string &sname, uint32_t max_players)
{
    mode = MODE_SINGLE;
    sock = SDLNet_UDP_Open(port);

    users.Init();

    ME.net = true;
    ME.ID = GenerateID();
    ME.name = name;
    ME.status = NetUser::STATUS_CONNECTED;

    session.Init(GenerateID(), sname, false);
    session.max_players = max_players;
    session.open = true;

    session.users.push_back(&ME);
    session.lead = &ME;

    eStatus = 0;

    threadsEnd = false;

    recvThread = SDL_CreateThread(_RecvThread, "", this);
    sendThread = SDL_CreateThread(_SendThread, "", this);
    updateThread = SDL_CreateThread(_UpdateThread, "", this);
}

Pkt *ZNDSingle::Recv_PreparePacket(InRawPkt *pkt)
{
    if (!pkt->Parse())
    {
        // Incorrect packet
        delete pkt;
        return NULL;
    }

    NetUser *from = users.FindByIP(pkt->addr);

    if (pkt->hdr.flags & PKT_FLAG_SYSTEM) // System message
    {
        return new Pkt(pkt, from);
    }
    else if (from) // Not system msg and known user
    {
        if (pkt->hdr.flags & PKT_FLAG_PART) // incomplete data, do assembly
        {
            AddrSeq ipseq;
            ipseq.set(pkt->addr, pkt->hdr.seqid);

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
                return new Pkt(parted, from);
            }

            // if not complete -> return NULL
        }
        else // not parted packet -> make it
        {
            return new Pkt(pkt, from);
        }
    }
    else // Not system and unknown user -> delete packet
    {
        delete pkt;
    }

    return NULL;
}

void ZNDSingle::ProcessSystemPkt(Pkt* pkt)
{
    if (!pkt)
        return;

    if (pkt->datasz < 1 )
        return;

    MemReader rd(pkt->data + 1, pkt->datasz - 1);

    if (pkt->user == NULL)
    {
        if (pkt->data[0] == SYS_MSG_HANDSHAKE && rd.size() >= 4 &&
            session.open && session.HasSlot())
        {
            uint8_t servstrSz = rd.readU8();
            uint8_t namestrSz = rd.readU8();

            if (namestrSz == 0 || servstrSz == 0 ||
               ((size_t)namestrSz + (size_t)servstrSz + 2) != rd.size() ||
                    servstrSz != servString.size())
            {
                return;
            }

            std::string srvnm;
            rd.readStr(srvnm, servstrSz);

            if (servString.compare(srvnm) != 0)
                return;

            std::string nm;
            rd.readStr(nm,  namestrSz);

            CorrectName(nm);

            if (users.FindByName(nm) || strcmp(ME.name.c_str(), nm.c_str()) == 0)
            {
                if (nm.size() > ZNDNET_USER_NAME_MAX - 5)
                    nm.resize( ZNDNET_USER_NAME_MAX - 5 );

                nm += '.';
                std::string base = nm;

                int cnt = 0;
                do
                {
                    char bf[10];
                    sprintf(bf, "%04d", (int)(ttime.GetTicks() % 10000));
                    nm = base;
                    nm += bf;
                    cnt ++;

                    if (cnt == 100)
                    {
                        SrvSendConnErr(pkt->addr, ERR_CONN_NAME);
                        return;
                    }
                }
                while(users.FindByName(nm) || strcmp(ME.name.c_str(), nm.c_str()) == 0);
            }

            NetUser *usr = users.Alloc();

            if (usr == NULL) // No free space
            {
                SrvSendConnErr(pkt->addr, ERR_CONN_FULL);
                return;
            }
            usr->addr = pkt->addr;
            usr->pingTime = ttime.GetTicks();
            usr->pingSeq = 0;
            usr->pongTime = usr->pingTime;
            usr->pongSeq = 0;
            usr->status = NetUser::STATUS_CONNECTED;
            usr->ID = GenerateID();
            usr->name = nm;
            usr->latence = 0;
            usr->sesID = 0;
            usr->net = true;

            //sLobby.users.push_back(usr);
            //usr->sesID = sLobby.ID;
            SrvSendConnected(usr, false);
            UserToSession(usr, &session);

            eStatus |= FLAGS_USERS_LIST_UPD;
            Events_Push( new EventNameID(EVENT_USER_ADD, 0, usr->name, usr->ID) );
        }
    }
    else
    {
        switch(pkt->data[0])
        {
            case SYS_MSG_PING:
                if (rd.size() == 4)
                {
                    uint32_t seq = rd.readU32();

                    if (seq > pkt->user->pongSeq && seq <= pkt->user->pingSeq)
                    {
                        pkt->user->pongSeq = seq;
                        pkt->user->pongTime = ttime.GetTicks();

                        int32_t latence = (pkt->user->pingSeq - seq) * DELAY_PING + (ttime.GetTicks() - pkt->user->pingTime);

                        if (!pkt->user->latence)
                            pkt->user->latence  = latence;
                        else
                            pkt->user->latence = latence + (pkt->user->latence - latence) / (int32_t)LATENCE_OLD_PART;
                    }
                }
                break;

            case SYS_MSG_DELIVERED:
                if (rd.size() == 4)
                {
                    uint32_t seq = rd.readU32();
                    ConfirmReceive( AddrSeq(pkt->addr, seq) );
                }
                break;

            case SYS_MSG_RETRY:
                if (rd.size() == 12)
                {
                    uint32_t seq = rd.readU32();
                    uint32_t from = rd.readU32();
                    uint32_t upto = rd.readU32();
                    ConfirmRetry( AddrSeq(pkt->addr, seq), from, upto );
                }
                break;

            case SYS_MSG_SES_LEAVE:
                {
                    DisconnectUser(pkt->user, 0);
                    users.Free(pkt->user);
                }
                break;

            case SYS_MSG_DISCONNECT:
                {
                    DisconnectUser(pkt->user, 1);
                    users.Free(pkt->user);
                }
                break;

            default:
                break;
        }
    }
}

void ZNDSingle::ProcessRegularPkt(Pkt* pkt)
{
    if (!pkt || !pkt->user)
        return;

    if (pkt->datasz < 1 )
        return;

    MemReader rd(pkt->data + 1, pkt->datasz - 1);

    switch(pkt->data[0])
    {
        case USR_MSG_DATA:
            if (rd.size() > 21)
            {
                uint64_t from = rd.readU64();
                uint8_t  cast = rd.readU8();
                uint64_t to = rd.readU64();
                uint32_t sz = rd.readU32();

                if (rd.size() == sz + rd.tell() && from == pkt->user->ID)
                {
                    if ( cast == 0 )
                    {
                        if (ME.ID == to)
                        {
                            Events_Push( new EventData(EVENT_DATA, pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC),
                                                                   from, (cast == 1 ? true : false),  to,
                                                                   sz,
                                                                   pkt->data + 1 + rd.tell(),
                                                                   pkt->uchnl ) );
                        }
                        else
                        {
                            NetUser *usr = users.FindByID(to);
                            if (usr)
                            {
                                RefData *dat = USRDataGenData(from, false, to, pkt->data + 1 + rd.tell(), sz);
                                SendingData *snd = new SendingData(usr->addr, usr->GetSeq(), dat, pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC));
                                snd->SetChannel(usr->__idx, pkt->uchnl);
                                Send_PushData(snd);
                            }
                        }
                    }
                    else if ( cast == 1 )
                    {
                        Events_Push( new EventData(EVENT_DATA, pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC),
                                                               from, (cast == 1 ? true : false),  to,
                                                               sz,
                                                               pkt->data + 1 + rd.tell(),
                                                               pkt->uchnl ) );

                        RefData *dat = USRDataGenData(from, true, to, pkt->data + 1 + rd.tell(), sz);
                        SrvSessionBroadcast(&session, dat,  pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC),  pkt->uchnl, pkt->user);
                    }

                }
            }
            break;

        default:
            break;
    }
}

void ZNDSingle::InterprocessUpdate()
{
    uint64_t curTime = ttime.GetTicks();

    for(NetUserList::iterator it = users.begin() ; it != users.end();)
    {
        NetUser *usr = *it;

        if (usr)
        {
            if (usr->net && usr->IsOnline())
            {
                if (curTime > usr->pongTime + TIMEOUT_USER)
                {
                    DisconnectUser(usr, 1);

                    it = users.erase(it);

                    //Disconnect player
                    continue; //skip it++
                }
                else
                {
                    //Ping
                    if (curTime > usr->pingTime + DELAY_PING)
                    {
                        usr->pingTime = curTime;
                        usr->pingSeq++;
                        SrvSendPing(usr);
                    }
                }
            }
        } // if (usr)

        it++;
    }

    if (session.closeTimer && curTime >= session.closeTimer)
        CloseSession();
}


void ZNDSingle::UserToSession(NetUser *usr, NetSession *ses)
{
    if (!usr || !ses)
        return;

    if (!ses->lobby)
    {
        if (!ses->users.empty())
        {
            RefData *dat = SrvDataGenUserJoin(usr);
            SrvSessionBroadcast(ses, dat, 0, 0, usr); //Send it in sync System channel
        }

        SrvSendSessionJoin(usr, ses, false);
    }

    ses->users.push_back(usr);

    if (usr->net)
    {
        RefData *dat = SrvDataGenUserList(ses);
        if (dat)
            Send_PushData( MkSendingData(usr, dat, 0, 0) ); //Send it in sync System channel
    }
}

void ZNDSingle::DisconnectUser(NetUser *usr, uint8_t type)
{
    if (!usr || !usr->net)
        return;

    Send_Clear(usr->addr);
    Confirm_Clear(usr->addr);
    Pending_Clear(usr->addr);

    session.users.remove(usr); // Remove this luser

    if (!session.users.empty()) // If anybody is alive
    {
        // Make packets for another users about usr is leave
        RefData *datLeave = SrvDataGenUserLeave(usr, type);
        SrvSessionBroadcast(&session, datLeave, 0, 0, usr); //Send it in sync System channel
    }

    Events_Push( new EventNameID(EVENT_USER_LEAVE, type, usr->name, usr->ID) );
    eStatus |= FLAGS_USERS_LIST_UPD;

    SrvSendDisconnect(usr); //Send to user disconnect msg

    usr->status = NetUser::STATUS_DISCONNECTED;
}

void ZNDSingle::ShowSession(bool show)
{
    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        session.open = show;
        SDL_UnlockMutex(eSyncMutex);
    }
}

void ZNDSingle::CloseSession()
{
    Send_Clear();
    Confirm_Clear();
    Pending_Clear();

    for(NetUserList::iterator it = session.users.begin(); it != session.users.end(); it = session.users.erase(it))
    {
        NetUser *usr = *it;
        if (usr)
        {
            if (usr->net)
            {
                SrvSendDisconnect(usr); //Send to user disconnect msg

                users.Free(usr);
            }
        }
    }

    eStatus = 0;
    ME.status = NetUser::STATUS_DISCONNECTED;
    Events_Push( new Event(EVENT_DISCONNECT, ERR_DISCONNECT_SERVER) );

    threadsEnd = true;
}

void ZNDSingle::CloseSession(uint32_t closeTime)
{
    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        if (!session.closeTimer)
        {
            if (closeTime > 60000)
                closeTime = 60000;

            session.closeTimer = ttime.GetTicks() + closeTime;

            RefDataWStream *dat = RefDataWStream::create(8);
            dat->writeU8(SYS_MSG_SES_CLOSE);
            dat->writeU32(closeTime);

            SrvSessionBroadcast(&session, dat, PKT_FLAG_SYSTEM, 0, NULL);

            Events_Push( new Event(EVENT_SESSION_END, closeTime) );
        }
        SDL_UnlockMutex(eSyncMutex);
    }
}

void ZNDSingle::KickUser(uint64_t _ID)
{
    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        NetUser *usr = users.FindByID(_ID);
        if (usr && usr->net)
        {
            DisconnectUser(usr, 2);
            users.Free(usr);
        }

        SDL_UnlockMutex(eSyncMutex);
    }
}

bool ZNDSingle::GetUsers(UserInfoVect &dst)
{
    if (!ME.IsOnline())
        return false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        dst.resize(session.users.size());

        uint32_t i = 0;
        for(NetUserList::iterator it = session.users.begin(); it != session.users.end(); it++)
        {
            dst[i] = *(*it);

            if ((*it) == &ME)
                dst[i].lead = true;

            i++;
        }

        SDL_UnlockMutex(eSyncMutex);
    }
    return true;
}

bool ZNDSingle::GetUser(UserInfo &dst, const char *name)
{
    if (!ME.IsOnline())
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        for(NetUserList::iterator it = session.users.begin(); it != session.users.end(); it++)
        {
            if ( strcmp( (*it)->name.c_str(), name ) == 0 )
            {
                dst = **it;
                if ((*it) == &ME)
                    dst.lead = true;

                res = true;
                break;
            }
        }

        SDL_UnlockMutex(eSyncMutex);
    }
    return res;
}

bool ZNDSingle::GetUser(UserInfo &dst, uint64_t _ID)
{
    if (!ME.IsOnline())
        return false;

    bool res = false;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        for(NetUserList::iterator it = session.users.begin(); it != session.users.end(); it++)
        {
            if ( (*it)->ID == _ID )
            {
                dst = **it;
                if ((*it) == &ME)
                    dst.lead = true;

                res = true;
                break;
            }
        }

        SDL_UnlockMutex(eSyncMutex);
    }
    return res;
}

void ZNDSingle::SendData(uint64_t to, void *data, uint32_t sz, uint8_t flags, uint8_t channel)
{
    if (!data || !sz)
        return;

    flags &= (PKT_FLAG_GARANT | PKT_FLAG_ASYNC);

    if (channel >= ZNDNET_USER_SCHNLS)
        channel = ZNDNET_USER_SCHNLS - 1;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        NetUser *usr = users.FindByID(to);
        if (usr)
        {
            RefData *rfdat = USRDataGenData(ME.ID, false, to, data, sz);

            SendingData *snd = new SendingData(usr->addr, usr->GetSeq(), rfdat, flags);
            snd->SetChannel(usr->__idx, channel);
            Send_PushData(snd);
        }
        SDL_UnlockMutex(eSyncMutex);
    }
}

void ZNDSingle::BroadcastData(void *data, uint32_t sz, uint8_t flags, uint8_t channel)
{
    if (!data || !sz)
        return;

    flags &= (PKT_FLAG_GARANT | PKT_FLAG_ASYNC);

    if (channel >= ZNDNET_USER_SCHNLS)
        channel = ZNDNET_USER_SCHNLS - 1;

    if (SDL_LockMutex(eSyncMutex) == 0)
    {
        RefData *rfdat = USRDataGenData(ME.ID, true, session.ID, data, sz);

        SrvSessionBroadcast(&session, rfdat,  flags,  channel, &ME);

        SDL_UnlockMutex(eSyncMutex);
    }
}

}
