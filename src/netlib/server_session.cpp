#include "zndNet.h"

namespace ZNDNet
{

NetSession *ZNDServer::SessionFind(uint64_t _ID)
{
    if (_ID == 0)
        return NULL;

    if (_ID == sLobby.ID)
        return &sLobby;

    NetSessionMap::iterator fnd = sessions.find(_ID);
    if (fnd != sessions.end())
        return fnd->second;

    return NULL;
}

NetSession *ZNDServer::SessionFind(const std::string &name)
{
    for(NetSessionMap::iterator it = sessions.begin(); it != sessions.end(); it++)
    {
        if (it->second->name == name)
            return it->second;
    }
    return NULL;
}

void ZNDServer::SessionDelete(uint64_t ID)
{
    NetSessionMap::iterator fnd = sessions.find(ID);
    if (fnd != sessions.end())
    {
        NetSession *ses = fnd->second;
        sessions.erase(fnd);

        if (ses)
        {
            SessionDisconnectAllUsers(ses, 1);
            delete ses;
        }
    }
}



void ZNDServer::SessionListUsers(NetUser *usr)
{
    if (!usr)
        return;

    if (!usr->IsOnline() || usr->sesID == 0 || usr->sesID == sLobby.ID)
        return;

    NetSession *ses = SessionFind(usr->sesID);
    if (ses)
    {
        RefData *dat = SrvDataGenUserList(ses);
        if (dat)
            Send_PushData( MkSendingData(usr, dat, 0, 0) ); //Send it in sync System channel
    }
}


void ZNDServer::SessionDisconnectAllUsers(NetSession *ses, uint8_t type)
{
    if (ses && !ses->lobby)
    {
        if (SDL_LockMutex(sendPktListMutex) == 0)
        {
            RefData *dat = SYSDataGenSesLeave(type);

            for(NetUserList::iterator it = ses->users.begin(); it != ses->users.end(); it = ses->users.erase(it))
            {
                NetUser *usr = *it;
                if (usr)
                {
                    if (usr->IsOnline())
                    {
                        SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), dat, PKT_FLAG_SYSTEM);
                        dta->SetChannel(usr->__idx, 0);
                        sendPktList.push_back(dta);
                    }

                    usr->sesID = sLobby.ID;
                    sLobby.users.push_back(usr);
                }
            }
            SDL_UnlockMutex(sendPktListMutex);
        }
    }
}


void ZNDServer::SessionUserLeave(NetUser *usr, uint8_t type)
{
    if (!usr)
        return;

    NetSession *sold = SessionFind(usr->sesID);

    if (sold)
    {
        if (sold->lobby)
            sold->users.remove(usr);
        else
        {
            SendingData *dta = new SendingData(usr->addr, usr->GetSeq(), SYSDataGenSesLeave(type), PKT_FLAG_SYSTEM);
            dta->SetChannel(usr->__idx, 0);
            sendPktList.push_back(dta);

            sold->users.remove(usr); // Remove this luser

            if (!sold->users.empty()) // If anybody is alive
            {
                // Make packets for another users about usr is leave
                RefData *datLeave = SrvDataGenUserLeave(usr, type);
                SrvSessionBroadcast(sold, datLeave, 0, 0, usr); //Send it in sync System channel
            }

            if (sold->lead == usr) // Oh my... this luser was a leader
            {
                if (!sold->users.empty())
                {
                    sold->lead = sold->users.front();

                    // Make packet for usr with leader flag
                    SendLeaderStatus(sold->lead, true);
                }
                else
                {
                    sold->lead = NULL;
                    //sold->orphanedTimer = ttime.GetTicks() + TIMEOUT_SESSION;
                    SessionDelete(sold->ID);
                }
            }
        }

        usr->sesID = 0;
    }
}

void ZNDServer::DoSessionUserJoin(NetUser *usr, NetSession *ses)
{
    if (!usr || !ses)
        return;

    SessionUserLeave(usr, 0);

    if (ses->ID != sLobby.ID)
    {
        if (!ses->users.empty())
        {
            RefData *dat = SrvDataGenUserJoin(usr);
            SrvSessionBroadcast(ses, dat, 0, 0, usr); //Send it in sync System channel
        }

        if (!ses->lead)
        {
            ses->lead = usr;
            SrvSendSessionJoin(usr, ses, true);
        }
        else
            SrvSendSessionJoin(usr, ses, false);
    }

    usr->sesID = ses->ID;
    ses->users.push_back(usr);

    SessionListUsers(usr);
}

RefData *ZNDServer::SessionErr(uint8_t code)
{
    RefDataStatic *dt = RefDataStatic::create(2);
    uint8_t *data = dt->get();
    data[0] = SYS_MSG_SES_ERR;
    data[1] = code;
    return dt;
}

void ZNDServer::SessionErrSend(NetUser *usr, uint8_t code)
{
    if (!usr)
        return;

    RefData *msg = SessionErr(code);
    SendingData *snd = new SendingData(usr->addr, usr->GetSeq(), msg, PKT_FLAG_SYSTEM);
    snd->SetChannel(usr->__idx, 0);
    Send_PushData(snd);
}


void ZNDServer::SessionClear()
{
    for(NetSessionMap::iterator it = sessions.begin() ; it != sessions.end(); it++)
        delete it->second;

    sessions.clear();
}


}
