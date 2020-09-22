#include "zndNet.h"

namespace ZNDNet
{

int ZNDSingle::_RecvThread(void *data)
{
    ZNDSingle *_this = (ZNDSingle *)data;

    UDPpacket *inpkt[ZNDNET_TUNE_MAXPKTS + 1];

    if (_this)
    {
        for (int i = 0; i < ZNDNET_TUNE_MAXPKTS; i++)
        {
            inpkt[i] = new UDPpacket;
            inpkt[i]->data = new uint8_t[ZNDNET_BUFF_SIZE];
            inpkt[i]->maxlen = ZNDNET_BUFF_SIZE;
            inpkt[i]->channel = -1;
        }

        inpkt[ZNDNET_TUNE_MAXPKTS] = NULL;

        while (!_this->threadsEnd)
        {
            int numrecv = SDLNet_UDP_RecvV(_this->sock, inpkt);

            if (numrecv > 0)
            {
                for(int i = 0; i < numrecv; i++)
                {
                    InRawPkt *pkt = new InRawPkt(inpkt[i]);
                    _this->Recv_PushInRaw(pkt);
                }

                SDL_Delay( ZNDNET_TUNE_MAXDELAY - ( ZNDNET_TUNE_MAXDELAY * numrecv / ZNDNET_TUNE_MAXPKTS ) );
            }
            else
                SDL_Delay(ZNDNET_TUNE_MAXDELAY);
        }

        for (int i = 0; i < ZNDNET_TUNE_MAXPKTS; i++)
        {
            delete[] inpkt[i]->data;
            delete inpkt[i];
        }
    }

    return 0;
}


int ZNDSingle::_SendThread(void *data)
{
    ZNDSingle *_this = (ZNDSingle *)data;

    uint8_t *sendBuffer;
    uint32_t *syncThings;
    uint32_t loop = 1;
    bool lastloop = false;
    UDPpacket pkt;
    const uint32_t SES_CHANNELS = ZNDNET_SES_USERS_MAX * ZNDNET_USER_SCHNLS;

    if (_this)
    {
        sendBuffer = new uint8_t[ZNDNET_BUFF_SIZE];
        syncThings = new uint32_t[SES_CHANNELS + 1]; // +extra channel for incorrect channels
        memset(syncThings, 0, (SES_CHANNELS + 1) * sizeof(uint32_t));

        pkt.data = sendBuffer;
        pkt.maxlen = ZNDNET_BUFF_SIZE;
        pkt.channel = -1;

        while ( !_this->threadsEnd || !lastloop)
        {
            if (_this->threadsEnd)
                lastloop = true;

            if (SDL_LockMutex(_this->sendModifyMutex) == 0)
            {
                SendingList::iterator it = _this->sendPktList.begin();
                size_t sendedBytes = 0;

                while(it != _this->sendPktList.end() && (!_this->threadsEnd || lastloop))
                {
                    if (sendedBytes >= ZNDNET_TUNE_SEND_MAXDATA)
                    {
                        sendedBytes = 0;
                        SDL_Delay(ZNDNET_TUNE_SEND_DELAY);
                    }

                    SendingData* dta = (*it);

                    if (lastloop) //Skip not system packets
                    {
                        if (!(dta->flags & PKT_FLAG_SYSTEM))
                        {
                            it++;
                            continue;
                        }
                    }

                    if (dta->flags & PKT_FLAG_SYSTEM)
                    {
                        if ( PrepareOutPacket(*dta, pkt) == 1 )
                        {
                            sendedBytes += pkt.len;
                            SDLNet_UDP_Send(_this->sock, -1, &pkt);
                        }

                        if ( SDL_LockMutex(_this->sendPktListMutex) == 0 )
                        {
                            delete dta;
                            it = _this->sendPktList.erase(it);
                            SDL_UnlockMutex(_this->sendPktListMutex);
                        }
                    }
                    else
                    {
                        uint32_t syncID = dta->schnl;

                        if (syncID == PKT_NO_CHANNEL || syncID > SES_CHANNELS)
                            syncID = SES_CHANNELS;

                        bool async = (dta->schnl == PKT_NO_CHANNEL) || (dta->flags & PKT_FLAG_ASYNC);

                        if ( async || syncThings[ syncID ] != loop )
                        {
                            if (!async)
                                syncThings[ syncID ] = loop; // Mark this channel has sent data on this loop

                            uint8_t res = PrepareOutPacket(*dta, pkt);

                            if ( res != 0 )
                            {
                                sendedBytes += pkt.len;
                                SDLNet_UDP_Send(_this->sock, -1, &pkt);

                                if (res == 1) //All data send
                                {
                                    if ( SDL_LockMutex(_this->sendPktListMutex) == 0 )
                                    {
                                        it = _this->sendPktList.erase(it);
                                        SDL_UnlockMutex(_this->sendPktListMutex);
                                    }

                                    if (dta->flags & PKT_FLAG_GARANT)
                                    {
                                        if (dta->tr_cnt)
                                        {
                                            dta->timeout = TIMEOUT_GARANT + _this->ttime.GetTicks();

                                            if ( SDL_LockMutex(_this->confirmQueueMutex) == 0 )
                                            {
                                                _this->confirmQueue.push_back(dta);
                                                SDL_UnlockMutex(_this->confirmQueueMutex);
                                            }
                                        }
                                        else
                                            delete dta;
                                    }
                                    else
                                        delete dta;
                                }
                                else // Not all data was send, keep it for next loop
                                {
                                    it++;
                                }
                            }
                            else
                            {
                                if ( SDL_LockMutex(_this->sendPktListMutex)  == 0 )
                                {
                                    it = _this->sendPktList.erase(it);
                                    SDL_UnlockMutex(_this->sendPktListMutex);

                                    delete dta;
                                }
                            }
                        }
                        else
                            it++;

                    }
                }

                SDL_UnlockMutex(_this->sendModifyMutex);
            }

            loop++;

            if (_this->sendPktList.size())
            {
                if ( _this->sendPktList.size() > ZNDNET_TUNE_MAXPKTS)
                    SDL_Delay( 0 );
                else
                    SDL_Delay( ZNDNET_TUNE_MAXDELAY - (ZNDNET_TUNE_MAXDELAY * _this->sendPktList.size() / ZNDNET_TUNE_MAXPKTS));
            }
            else
                SDL_Delay(ZNDNET_TUNE_MAXDELAY);
        }

        delete[] sendBuffer;
        delete[] syncThings;
    }

    return 0;
}


int ZNDSingle::_UpdateThread(void *data)
{
    ZNDSingle *_this = (ZNDSingle *)data;

    while (!_this->threadsEnd)
    {
        uint32_t pktsRecv = 0;

        uint64_t forceBrake = _this->ttime.GetTicks() + TIMEOUT_CLI_RECV_MAX;

        //This type is needed often work with client-side things -> do sync locks for every packet.
        while (_this->ttime.GetTicks() < forceBrake)
        {
            InRawPkt *ipkt = _this->Recv_PopInRaw();
            if (!ipkt)
                break; // If no more packets -> do another things

            pktsRecv++;

            if (SDL_LockMutex(_this->eSyncMutex) == 0)
            {
                Pkt * pkt = _this->Recv_PreparePacket(ipkt);
                if (pkt)
                {

                    if (pkt->flags & PKT_FLAG_SYSTEM)
                        _this->ProcessSystemPkt(pkt);
                    else
                    {
                        if (pkt->flags & PKT_FLAG_GARANT)
                            _this->SendDelivered(pkt->seqid, pkt->addr);

                        _this->ProcessRegularPkt(pkt);
                    }

                    delete pkt;
                }
                SDL_UnlockMutex(_this->eSyncMutex);
            }
        }

        if (SDL_LockMutex(_this->eSyncMutex) == 0)
        {
            _this->InterprocessUpdate();
            SDL_UnlockMutex(_this->eSyncMutex);
        }

        if (pktsRecv > ZNDNET_TUNE_MAXPKTS)
            SDL_Delay(1);
        else
            SDL_Delay(1 + ZNDNET_TUNE_MAXDELAY - ZNDNET_TUNE_MAXDELAY * pktsRecv / ZNDNET_TUNE_MAXPKTS);
    }

    SDL_WaitThread(_this->recvThread, NULL);
    SDL_WaitThread(_this->sendThread, NULL);

    _this->recvThread = NULL;
    _this->sendThread = NULL;

    _this->ME.status = NetUser::STATUS_DISCONNECTED;

    _this->Send_Clear();
    _this->Confirm_Clear();
    _this->Pending_Clear();
    _this->Recv_Clear();

    SDLNet_UDP_Close(_this->sock);

    return 0;
}


}
