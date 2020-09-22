#include "zndNet.h"
#include "errcode.h"

namespace ZNDNet
{

ZNDServer::ZNDServer(const std::string &servstring)
: ZNDNet(servstring), users(ZNDNET_USERS_MAX)
{
}

Pkt *ZNDServer::Recv_PreparePacket(InRawPkt *pkt)
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

void ZNDServer::ProcessSystemPkt(Pkt* pkt)
{
    if (!pkt)
        return;

    if (pkt->datasz < 1 )
        return;

    MemReader rd(pkt->data + 1, pkt->datasz - 1);

    if (pkt->user == NULL)
    {
        if (pkt->data[0] == SYS_MSG_HANDSHAKE && rd.size() >= 4)
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

            if (users.FindByName(nm))
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
                while(users.FindByName(nm));
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
            SrvSendConnected(usr, true);
            UserToLobby(usr, 0);
        }
    }
    else
    {
        switch(pkt->data[0])
        {
            case SYS_MSG_LIST_GAMES:
                {
                    if (rd.size() != 8)
                        return;

                    if (pkt->user->ID != rd.readU64())
                        return;

                    RefData *msg = USRDataGenGamesList();
                    SendingData *dat = new SendingData(pkt->user->addr, pkt->user->GetSeq(), msg, PKT_FLAG_ASYNC);
                    dat->SetChannel(pkt->user->__idx, 0);
                    Send_PushData(dat);
                }
                break;

            case SYS_MSG_SES_CREATE:
                if (rd.size() > 5)
                {
                    std::string sesname;
                    std::string pswd;

                    sesname.clear();
                    rd.readSzStr(sesname);

                    if (!SessionCheckName(sesname) || SessionFind(sesname))
                    {
                        SessionErrSend(pkt->user, ERR_SES_CREATE);
                        return;
                    }

                    uint32_t mx_pl = rd.readU32();

                    if (mx_pl > ZNDNET_SES_USERS_MAX)
                        mx_pl = ZNDNET_SES_USERS_MAX;

                    pswd.clear();
                    rd.readSzStr(pswd);
                    if (pswd.size() > 0 && !SessionCheckPswd(pswd))
                    {
                        SessionErrSend(pkt->user, ERR_SES_CREATE);
                        return;
                    }

                    NetSession *ns = new NetSession();
                    ns->Init(GenerateID(), sesname, false);
                    ns->max_players = mx_pl;
                    ns->password = pswd;
                    ns->open = true;

                    sessions[ns->ID] = ns;

                    DoSessionUserJoin(pkt->user, ns);
                }
                else
                {
                    SessionErrSend(pkt->user, ERR_SES_CREATE);
                    return;
                }
                break;

            case SYS_MSG_SES_JOIN:
                if (rd.size() > 8 )
                {
                    uint64_t SID = rd.readU64();
                    std::string pss;
                    rd.readSzStr(pss);

                    NetSession *ses = SessionFind(SID);
                    if (!ses || !ses->open)
                    {
                        SessionErrSend(pkt->user, ERR_SES_JOIN);
                        return;
                    }

                    if (ses->max_players > 0 && ses->users.size() >= ses->max_players)
                    {
                        SessionErrSend(pkt->user, ERR_SES_JOIN);
                        return;
                    }

                    if (ses->password.size() > 0 && ses->password != pss)
                    {
                        SessionErrSend(pkt->user, ERR_SES_JOIN);
                        return;
                    }

                    DoSessionUserJoin(pkt->user, ses);
                }
                else
                {
                    SessionErrSend(pkt->user, ERR_SES_JOIN);
                    return;
                }
                break;

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

            case SYS_MSG_DISCONNECT:
                {
                    DisconnectUser(pkt->user);
                    users.Free(pkt->user);
                }
                break;

            case SYS_MSG_SES_SHOW:
                {
                    if (rd.size() == 1 )
                    {
                        uint8_t show = rd.readU8();
                        if (pkt->user->sesID != sLobby.ID)
                        {
                            NetSession *ses = SessionFind(pkt->user->sesID);
                            if (ses && ses->lead == pkt->user)
                                ses->open = (show == 1);
                        }
                    }
                }
                break;

            case SYS_MSG_SES_LEAVE:
                {
                    if (pkt->user->sesID != sLobby.ID)
                        UserToLobby(pkt->user, 0);
                }
                break;

            case SYS_MSG_SES_KICK:
                {
                    if (rd.size() == sizeof(uint64_t) )
                    {
                        uint64_t UID = rd.readU64();

                        if (pkt->user->sesID != sLobby.ID && UID != pkt->user->ID)
                        {
                            NetSession *ses = SessionFind(pkt->user->sesID);
                            if (ses && ses->lead == pkt->user)
                            {
                                for(NetUserList::iterator it = ses->users.begin(); it != ses->users.end(); it++)
                                {
                                    if ((*it)->ID == UID)
                                    {
                                        UserToLobby(*it, 2);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                break;

            case SYS_MSG_SES_CLOSE:
                {
                    if (rd.size() == sizeof(uint32_t) )
                    {
                        uint32_t timeout = rd.readU32();

                        if (timeout > 60000)
                            timeout = 60000;

                        if (pkt->user->sesID != sLobby.ID)
                        {
                            NetSession *ses = SessionFind(pkt->user->sesID);
                            if (ses && ses->lead == pkt->user && !ses->closeTimer)
                            {
                                ses->closeTimer = ttime.GetTicks() + timeout;

                                RefDataWStream *dat = RefDataWStream::create(8);
                                dat->writeU8(SYS_MSG_SES_CLOSE);
                                dat->writeU32(timeout);

                                SrvSessionBroadcast(ses, dat, PKT_FLAG_SYSTEM, 0, NULL);
                            }
                        }
                    }
                }
                break;

            default:
                break;
        }
    }
}

void ZNDServer::ProcessRegularPkt(Pkt* pkt)
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
                        NetUser *usr = users.FindByID(to);
                        if (usr)
                        {
                            RefData *dat = USRDataGenData(from, false, to, pkt->data + 1 + rd.tell(), sz);
                            SendingData *snd = new SendingData(usr->addr, usr->GetSeq(), dat, pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC));
                            snd->SetChannel(usr->__idx, pkt->uchnl);
                            Send_PushData(snd);
                        }
                    }
                    else if ( cast == 1 )
                    {
                        NetSession *ses = SessionFind(to);
                        if (ses)
                        {
                            RefData *dat = USRDataGenData(from, true, to, pkt->data + 1 + rd.tell(), sz);
                            SrvSessionBroadcast(ses, dat,  pkt->flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC),  pkt->uchnl, pkt->user);
                        }
                    }

                }
            }
            break;

        default:
            break;
    }
}

void ZNDServer::Start(uint16_t port)
{
    mode = MODE_SERVER;
    sock = SDLNet_UDP_Open(port);

    users.Init();
    sLobby.Init( GenerateID(), "", true );

    threadsEnd = false;

    recvThread = SDL_CreateThread(_RecvThread, "", this);
    sendThread = SDL_CreateThread(_SendThread, "", this);
    updateThread = SDL_CreateThread(_UpdateThread, "", this);
}








void ZNDServer::SendLeaderStatus(NetUser *usr, bool lead)
{
    if (!usr)
        return;

    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(SYS_MSG_SES_LEAD);
    dat->writeU8(lead ? 1 : 0);

    SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), dat, PKT_FLAG_SYSTEM); // Send, NOW YOU A LEADER!
    dta->SetChannel(usr->__idx, 0);
    Send_PushData(dta);
}



RefData *ZNDServer::USRDataGenGamesList()
{
    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(USR_MSG_LIST_GAMES);
    dat->writeU32(sessions.size());
    uint32_t opened = 0;
    for(NetSessionMap::iterator it = sessions.begin(); it != sessions.end(); it++)
    {
        if (it->second->open && !it->second->closeTimer)
        {
            dat->writeU64( it->second->ID );
            dat->writeU32( it->second->users.size() );
            dat->writeU32( it->second->max_players );
            dat->writeU8( (it->second->password.size() > 0 ? 1 : 0 ) );
            dat->writeSzStr( it->second->name );

            opened++;
        }
    }

    dat->seek(1, 0);
    dat->writeU32(opened); //rewrite sessions.size()

    dat->seek(0, 2);

    return dat;
}






void ZNDServer::DisconnectUser(NetUser *usr)
{
    if (!usr)
        return;

    Send_Clear(usr->addr);
    Confirm_Clear(usr->addr);
    Pending_Clear(usr->addr);

    SessionUserLeave(usr, 1);

    SrvSendDisconnect(usr); //Send to user disconnect msg

    usr->status = NetUser::STATUS_DISCONNECTED;
}


void ZNDServer::InterprocessUpdate()
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
                    DisconnectUser(usr);

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

    for(NetSessionMap::iterator it = sessions.begin() ; it != sessions.end();)
    {
        NetSession *ses = it->second;

        if (ses)
        {
            if (ses->users.size() == 0)
            {
                it = sessions.erase(it);
                delete ses;
                continue;
            }

            if (ses->closeTimer && curTime >= ses->closeTimer)
            {
                it = sessions.erase(it);
                SessionDisconnectAllUsers(ses, 2);
                delete ses;
                continue;
            }
        } // if (usr)

        it++;
    }

    PendingCheck(); // Delete timeout packets
    ConfirmQueueCheck();

}


RefData *ZNDServer::SYSDataGenSesLeave(int8_t type)
{
    RefDataWStream *strm = RefDataWStream::create(4);
    strm->writeU8(SYS_MSG_SES_LEAVE);
    strm->writeU8(type);
    return strm;
}


void ZNDServer::UserToLobby(NetUser *usr, uint8_t type)
{
    if (!usr)
        return;

    SessionUserLeave(usr, type);

    usr->sesID = sLobby.ID;
    sLobby.users.push_back(usr);
}

}
