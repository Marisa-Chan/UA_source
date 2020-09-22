#include "zndNet.h"
#include <regex>


namespace ZNDNet
{

ZNDNet::ZNDNet(const std::string &servstring)
{
    mode = MODE_UNKNOWN;
    servString = servstring;

    threadsEnd = true;

    updateThread = NULL;

    recvThread = NULL;
    recvPktListMutex = SDL_CreateMutex();

    sendThread = NULL;
    sendPktListMutex = SDL_CreateMutex();

    //confirmPktList.clear();
    confirmQueueMutex = SDL_CreateMutex();

    eSyncMutex = SDL_CreateMutex();

    eEventMutex = SDL_CreateMutex();
    eEventDataSize = 0;
    eEventNextID = 0;
    eEventWaitLock = 0;

    sendModifyMutex = SDL_CreateMutex();
}

ZNDNet::~ZNDNet()
{
    Stop();

    Events_Clear();

    SDL_DestroyMutex(recvPktListMutex);
    SDL_DestroyMutex(sendPktListMutex);
    SDL_DestroyMutex(confirmQueueMutex);
    SDL_DestroyMutex(eSyncMutex);
    SDL_DestroyMutex(eEventMutex);
    SDL_DestroyMutex(sendModifyMutex);
}



uint64_t ZNDNet::GenerateID()
{
    return 1 + SDL_GetPerformanceCounter();
}



void ZNDNet::CorrectName(std::string &name)
{
    if (name.size() > ZNDNET_USER_NAME_MAX)
        name.resize(ZNDNET_USER_NAME_MAX);

    for(size_t i = 0; i < name.size(); i++)
    {
        char chr = name[i];
        chr &= 0x7F;
        if (chr < ' ')
            chr = ' ';
        name[i] = chr;
    }
}


SendingData *ZNDNet::MkSendingData(NetUser *usr, RefData *data, uint8_t flags, uint32_t chnl)
{
    SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), data, flags);
    dta->SetChannel(usr->__idx, chnl);
    return dta;
}


void ZNDNet::ConfirmQueueCheck()
{
    uint64_t tcur = ttime.GetTicks();

    for (SendingList::iterator it = confirmQueue.begin(); it != confirmQueue.end(); )
    {
        SendingData *dta = *it;

        if (tcur >= dta->timeout)
        {
            if ( SDL_LockMutex(confirmQueueMutex) == 0 )
            {
                it = confirmQueue.erase(it);
                SDL_UnlockMutex(confirmQueueMutex);

                Send_RetryData(dta);
            }
        }
        else
            it++;
    }
}

void ZNDNet::PendingCheck()
{
    uint64_t tcur = ttime.GetTicks();

    for (PartedList::iterator it = pendingPkt.begin(); it != pendingPkt.end();)
    {
        if (tcur >= (*it)->timeout)
        {
            InPartedPkt *pkt = *it;
            if ( (pkt->flags & PKT_FLAG_GARANT) && pkt->retry > 0 )
            {
                size_t upto = pkt->RetryUpTo();
                if (!upto)
                {
                    it = pendingPkt.erase(it);
                    delete pkt;
                }
                else
                {
                    pkt->retry--;
                    pkt->timeout = tcur + TIMEOUT_PENDING_GARANT;
                    SendRetry(pkt->ipseq.seq, pkt->ipseq.addr, pkt->nextOff, upto);
                }
            }
            else
            {
                it = pendingPkt.erase(it);
                delete pkt;
            }
        }
        else
            it++;
    }
}

void ZNDNet::ConfirmReceive(AddrSeq _seq)
{
    for (SendingList::iterator it = confirmQueue.begin(); it != confirmQueue.end(); )
    {
        SendingData *dta = *it;
        if (dta->addr == _seq)
        {
            if ( SDL_LockMutex(confirmQueueMutex) == 0 )
            {
                it = confirmQueue.erase(it);
                SDL_UnlockMutex(confirmQueueMutex);

                delete dta;
            }
            break;
        }
        else
            it++;
    }
}

void ZNDNet::ConfirmRetry(AddrSeq _seq, uint32_t from, uint32_t to)
{
    if (to <= from)
        return;

    for (SendingList::iterator it = confirmQueue.begin(); it != confirmQueue.end(); )
    {
        SendingData *dta = *it;
        if (dta->addr == _seq)
        {
            if (dta->pdata->size() > from && dta->pdata->size() >= to )
            {
                if ( SDL_LockMutex(confirmQueueMutex) == 0 )
                {
                    it = confirmQueue.erase(it);
                    SDL_UnlockMutex(confirmQueueMutex);

                    Send_RetryData(dta, from, to, false);
                }
            }
            break;
        }
        else
            it++;
    }
}

void ZNDNet::SendDelivered(uint32_t _seqid, const IPaddress &addr)
{
    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(SYS_MSG_DELIVERED);
    dat->writeU32(_seqid);

    Send_PushData( new SendingData(addr, 0, dat, PKT_FLAG_SYSTEM) );
}

void ZNDNet::SendRetry(uint32_t _seqid, const IPaddress &addr, uint32_t nextOff, uint32_t upto)
{
    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(SYS_MSG_RETRY);
    dat->writeU32(_seqid);
    dat->writeU32(nextOff);
    dat->writeU32(upto);

    Send_PushData( new SendingData(addr, 0, dat, PKT_FLAG_SYSTEM) );
}




void ZNDNet::Events_Push(Event *evnt)
{
    if (!evnt)
        return;

    if (evnt->size >= EVENTS_DATA_MAX)
    {
        delete evnt;
        return;
    }

    if (SDL_LockMutex(eEventMutex) == 0)
    {
        while (eEventList.size() >= EVENTS_MAX || (eEventDataSize + evnt->size) >= EVENTS_DATA_MAX)
        {
            Event *tmp = eEventList.front();
            eEventList.pop_front();

            eEventDataSize -= tmp->size;
            delete tmp;
        }

        if (eEventWaitLock == 0 && eEventList.empty()) //Check for no more events in list and no wait functions
            eEventNextID = 0; //Clear ID for avoid overflow

        eEventDataSize += evnt->size;
        evnt->__id = eEventNextID;
        eEventList.push_back(evnt);

        eEventNextID++;

        SDL_UnlockMutex(eEventMutex);
    }
}

Event *ZNDNet::Events_Pop()
{
    if ( eEventList.empty() )
        return NULL;

    if (SDL_LockMutex(eEventMutex) == 0)
    {
        Event *tmp = eEventList.front();
        eEventList.pop_front();

        eEventDataSize -= tmp->size;

        SDL_UnlockMutex(eEventMutex);

        return tmp;
    }

    return NULL;
}

void ZNDNet::Events_ClearByType(uint32_t type)
{
    if (SDL_LockMutex(eEventMutex) == 0)
    {
        for(EventList::iterator it = eEventList.begin(); it != eEventList.end();)
        {
            Event *evt = *it;
            if (evt->type == type)
            {
                eEventDataSize -= evt->size;
                it = eEventList.erase(it);
                delete evt;
            }
            else
                it++;
        }
        SDL_UnlockMutex(eEventMutex);
    }
}

void ZNDNet::Events_Clear()
{
    if (SDL_LockMutex(eEventMutex) == 0)
    {
        for(EventList::iterator it = eEventList.begin(); it != eEventList.end(); it = eEventList.erase(it))
            delete (*it);

        eEventDataSize = 0;

        if (eEventWaitLock == 0) //Check No wait functions
            eEventNextID = 0; //Clear ID for avoid overflow

        SDL_UnlockMutex(eEventMutex);
    }
}

Event *ZNDNet::Events_PeekByType(uint32_t type)
{
    if (SDL_LockMutex(eEventMutex) == 0)
    {
        for(EventList::iterator it = eEventList.begin(); it != eEventList.end(); it++)
        {
            Event *evt = *it;
            if (evt->type == type)
            {
                eEventDataSize -= evt->size;

                eEventList.erase(it);
                SDL_UnlockMutex(eEventMutex);

                return evt;
            }
        }
        SDL_UnlockMutex(eEventMutex);
    }
    return NULL;
}

Event *ZNDNet::Events_WaitForMsg(uint32_t type, uint32_t time)
{
    uint32_t nextID = 0;

    if (SDL_LockMutex(eEventMutex) == 0)
    {
        eEventWaitLock++;

        for(EventList::iterator it = eEventList.begin(); it != eEventList.end(); it++)
        {
            Event *evt = *it;
            if (evt->type == type)
            {
                eEventDataSize -= evt->size;
                eEventList.erase(it);
                eEventWaitLock--;
                SDL_UnlockMutex(eEventMutex);
                return evt;
            }
        }

        nextID = eEventNextID;
        SDL_UnlockMutex(eEventMutex);
    }

    //Wait for new events

    uint64_t tmax = ttime.GetTicks() + time;

    while(true)
    {
        SDL_Delay(1);

        if (nextID < eEventNextID) // If new packets arrived
        {
            if (SDL_LockMutex(eEventMutex) == 0) //lock it for iterate
            {
                for(EventList::iterator it = eEventList.begin(); it != eEventList.end(); it++)
                {
                    Event *evt = *it;
                    if (evt->type == type)
                    {
                        eEventDataSize -= evt->size;
                        eEventList.erase(it);
                        eEventWaitLock--;
                        SDL_UnlockMutex(eEventMutex);
                        return evt;
                    }
                }

                nextID = eEventNextID;
                SDL_UnlockMutex(eEventMutex);
            }
        }

        if (time && ttime.GetTicks() >= tmax)
            break;
    }

    eEventWaitLock--;
    return NULL;
}

void ZNDNet::Confirm_Clear(const IPaddress &addr)
{
    for (SendingList::iterator it = confirmQueue.begin(); it != confirmQueue.end(); )
    {
        SendingData *dta = *it;
        if ( IPCMP(dta->addr.addr, addr) )
        {
            if ( SDL_LockMutex(confirmQueueMutex) == 0 )
            {
                it = confirmQueue.erase(it);
                SDL_UnlockMutex(confirmQueueMutex);

                delete dta;
            }
        }
        else
            it++;
    }
}

void ZNDNet::Confirm_Clear()
{
    for (SendingList::iterator it = confirmQueue.begin(); it != confirmQueue.end(); it = confirmQueue.erase(it))
        delete *it;
}

void ZNDNet::Pending_Clear(const IPaddress &addr)
{
    for (PartedList::iterator it = pendingPkt.begin(); it != pendingPkt.end();)
    {
        InPartedPkt *pkt = *it;
        if ( IPCMP(pkt->ipseq.addr, addr) )
        {
            it = pendingPkt.erase(it);
            delete pkt;
        }
        else
            it++;
    }
}

void ZNDNet::Pending_Clear()
{
    for (PartedList::iterator it = pendingPkt.begin(); it != pendingPkt.end(); it = pendingPkt.erase(it))
        delete *it;
}




RefData *ZNDNet::USRDataGenData(uint64_t from, bool cast, uint64_t to, void *data, uint32_t sz)
{
    if (!data || !sz)
        return NULL;

    RefDataWStream *rfdat = RefDataWStream::create(32 + sz);
    rfdat->writeU8(USR_MSG_DATA);
    rfdat->writeU64(from);
    rfdat->writeU8(cast ? 1 : 0);
    rfdat->writeU64(to);
    rfdat->writeU32(sz);
    rfdat->write(data, sz);
    return rfdat;
}

void ZNDNet::Stop()
{
    if (updateThread)
    {
        threadsEnd = true;
        SDL_WaitThread(updateThread, NULL);
        updateThread = NULL;
    }
}


uint8_t ZNDNet::PrepareOutPacket(SendingData &data, UDPpacket &out )
{
    if (data.flags & PKT_FLAG_SYSTEM)
    {
        if ( data.pdata->size() <= (ZNDNET_PKT_MAXSZ - HDR_OFF_SYS_DATA) )
        {
            out.address = data.addr.addr;
            out.len = data.pdata->size() + HDR_OFF_SYS_DATA;
            out.maxlen = out.len;
            out.data[HDR_OFF_FLAGS] = PKT_FLAG_SYSTEM;
            data.pdata->copy(&out.data[HDR_OFF_SYS_DATA]);
            return 1;
        }
    }
    else
    {
        if ( data.pdata->size() <= (ZNDNET_PKT_MAXSZ - HDR_OFF_DATA) ) //Normal MSG by one piece
        {
            out.address = data.addr.addr;
            out.len = data.pdata->size() + HDR_OFF_DATA;
            out.maxlen = out.len;

            out.data[HDR_OFF_FLAGS] = data.flags & (PKT_FLAG_GARANT | PKT_FLAG_ASYNC);
            writeU32(data.addr.seq, &out.data[HDR_OFF_SEQID]);
            out.data[HDR_OFF_CHANNEL] = data.uchnl;

            data.pdata->copy(&out.data[HDR_OFF_DATA]);
            return 1;
        }
        else if ( data.sended < data.pdata->size() ) // Multipart
        {
            uint32_t UpTo = data.pdata->size();

            if (data.retryUpTo)
                UpTo = data.retryUpTo; //Set upper border, if some part not receieved

            uint32_t pktdatalen = UpTo - data.sended;  //How many bytes we needed to send

            if ( pktdatalen > (ZNDNET_PKT_MAXSZ - HDR_OFF_PART_DATA) )
                pktdatalen = (ZNDNET_PKT_MAXSZ - HDR_OFF_PART_DATA); //Maximum size we can handle by 1 packet

            out.address = data.addr.addr;
            out.len = HDR_OFF_PART_DATA + pktdatalen;
            out.maxlen = out.len;

            out.data[HDR_OFF_FLAGS] = PKT_FLAG_PART | (data.flags & PKT_FLAG_MASK_NORMAL);
            writeU32(data.addr.seq, &out.data[HDR_OFF_SEQID]);
            out.data[HDR_OFF_CHANNEL] = data.uchnl;

            writeU32(data.pdata->size(), &out.data[HDR_OFF_PART_FSIZE]);
            writeU32(data.sended, &out.data[HDR_OFF_PART_OFFSET]);

            data.pdata->copy(&out.data[HDR_OFF_PART_DATA], data.sended, pktdatalen);

            data.sended += pktdatalen;

            if (data.sended >= UpTo)
                return 1;
            else
                return 2;
        }
    }
    return 0;
}


void ZNDNet::SrvSendConnected(NetUser *usr, bool multisession)
{
    if (!usr)
        return;

    RefDataWStream *strm = RefDataWStream::create();

    strm->writeU8(SYS_MSG_CONNECTED);
    strm->writeU8( multisession ? 1 : 0 ); //has lobby
    strm->writeU64(usr->ID);
    strm->writeSzStr(usr->name);

    Send_PushData( new SendingData(usr->addr, 0, strm, PKT_FLAG_SYSTEM) );
}

void ZNDNet::SrvSendConnErr(const IPaddress &addr, uint8_t type)
{
    RefDataWStream *strm = RefDataWStream::create(4);
    strm->writeU8(SYS_MSG_CONNERR);
    strm->writeU8(type);

    Send_PushData( new SendingData(addr, 0, strm, PKT_FLAG_SYSTEM) );
}

RefData *ZNDNet::SrvDataGenUserJoin(NetUser *usr)
{
    if (!usr)
        return NULL;

    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(USR_MSG_SES_USERJOIN);
    dat->writeU64(usr->ID);
    dat->writeSzStr(usr->name);
    return dat;
}

void ZNDNet::SrvSendSessionJoin(NetUser *usr, NetSession *ses, bool leader)
{
    if (!usr || !ses)
        return;

    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(SYS_MSG_SES_JOIN);
    dat->writeU8(leader ? 1 : 0);
    dat->writeU64(ses->ID);
    dat->writeSzStr(ses->name);

    SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), dat, PKT_FLAG_SYSTEM); // Send, NOW YOU A LEADER!
    dta->SetChannel(usr->__idx, 0);
    Send_PushData(dta);
}

RefData *ZNDNet::SrvDataGenUserList(NetSession *ses)
{
    if (!ses || ses->lobby)
        return NULL;

    RefDataWStream *dat = RefDataWStream::create();
    dat->writeU8(USR_MSG_SES_USERLIST);
    dat->writeU32(ses->users.size());

    for(NetUserList::iterator it = ses->users.begin(); it != ses->users.end(); it++)
    {
        dat->writeU64( (*it)->ID );
        dat->writeSzStr( (*it)->name );
    }

    return dat;
}

void ZNDNet::SrvSendPing(NetUser *usr)
{
    if (!usr)
        return;

    RefDataWStream *strm = RefDataWStream::create();

    strm->writeU8(SYS_MSG_PING);
    strm->writeU32(usr->pingSeq);

    Send_PushData( new SendingData(usr->addr, 0, strm, PKT_FLAG_SYSTEM) );
}

void ZNDNet::SrvSendDisconnect(NetUser *usr)
{
    if (!usr)
        return;

    RefDataWStream *strm = RefDataWStream::create(4);

    strm->writeU8(SYS_MSG_DISCONNECT);

    Send_PushData( new SendingData(usr->addr, 0, strm, PKT_FLAG_SYSTEM) );
}

RefData *ZNDNet::SrvDataGenUserLeave(NetUser *usr, uint8_t type)
{
    if (!usr)
        return NULL;

    RefDataWStream *dat = RefDataWStream::create(16);
    dat->writeU8(USR_MSG_SES_USERLEAVE);
    dat->writeU64(usr->ID);
    dat->writeU8(type);
    return dat;
}

void ZNDNet::SrvSessionBroadcast(NetSession *ses, RefData *dat, uint8_t flags, uint8_t chnl, NetUser *from)
{
    if (!ses || !dat)
        return;

    if (SDL_LockMutex(sendPktListMutex) == 0)
    {
        for(NetUserList::iterator it = ses->users.begin(); it != ses->users.end(); it++)
        {
            NetUser *usr = *it;
            if (usr && usr != from && usr->net && usr->IsOnline())
            {
                SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), dat, flags);
                dta->SetChannel(usr->__idx, chnl);
                sendPktList.push_back(dta);
            }
        }
        SDL_UnlockMutex(sendPktListMutex);
    }
}

bool ZNDNet::SessionCheckName(const std::string &name)
{
    if (name.size() > SES_NAME_MAX)
        return false;

    std::regex re("[[:alnum:]][[:print:]]+[[:graph:]]" );

    if ( std::regex_match (name,  re) )
        return true;

    return false;
}

bool ZNDNet::SessionCheckPswd(const std::string &pswd)
{
    std::regex re("[[:graph:]]{4,16}");

    if ( std::regex_match (pswd,  re) )
        return true;

    return false;
}




void ZNDNet::Recv_PushInRaw(InRawPkt *inpkt)
{
    if (SDL_LockMutex(recvPktListMutex) == 0)
    {
        recvPktList.push_back(inpkt);
        SDL_UnlockMutex(recvPktListMutex);
    }
}


InRawPkt *ZNDNet::Recv_PopInRaw()
{
    InRawPkt *ret = NULL;

    if (SDL_LockMutex(recvPktListMutex) == 0)
    {
        if (!recvPktList.empty())
        {
            ret = recvPktList.front();
            recvPktList.pop_front();
        }

        SDL_UnlockMutex(recvPktListMutex);
    }

    return ret;
}

void ZNDNet::Send_PushData(SendingData *data)
{
    if (!data)
        return;

    if (SDL_LockMutex(sendPktListMutex) == 0)
    {
        sendPktList.push_back(data);
        SDL_UnlockMutex(sendPktListMutex);
    }
}

void ZNDNet::Send_RetryData(SendingData *data, size_t from, size_t to, bool decr)
{
    if (!data)
        return;

    if (decr)
    {
        if (data->tr_cnt > 0)
            data->tr_cnt--;
    }


    if (from >= data->pdata->size() || to > data->pdata->size() || (to != 0 && to < from) )
    {
        delete data;
        return;
    }

    data->sended = from;
    data->retryUpTo = to;

    Send_PushData(data);
}




void ZNDNet::Send_Clear(const IPaddress &addr)
{
    if (SDL_LockMutex(sendModifyMutex) == 0)
    {
        for (SendingList::iterator it = sendPktList.begin(); it != sendPktList.end(); )
        {
            SendingData* dta = (*it);
            if ( IPCMP(dta->addr.addr, addr) )
            {
                delete dta;
                it = sendPktList.erase(it);
            }
            else
                it++;
        }

        SDL_UnlockMutex(sendModifyMutex);
    }
}

void ZNDNet::Send_Clear()
{
    if (SDL_LockMutex(sendModifyMutex) == 0)
    {
        for (SendingList::iterator it = sendPktList.begin(); it != sendPktList.end(); it = sendPktList.erase(it))
            delete *it;

        SDL_UnlockMutex(sendModifyMutex);
    }
}

void ZNDNet::Recv_Clear()
{
    if (SDL_LockMutex(recvPktListMutex) == 0)
    {
        if (!recvPktList.empty())
        {
            delete recvPktList.front();
            recvPktList.pop_front();
        }

        SDL_UnlockMutex(recvPktListMutex);
    }
}


}
