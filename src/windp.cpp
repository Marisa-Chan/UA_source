#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "includes.h"
#include "engine_input.h"
#include "windp.h"
#include "button.h"
#include "yw_net.h"


const Nucleus::ClassDescr NC_STACK_windp::description("windp.class", &newinstance);

const char *SERV_STR = "UA:SOURCE TEST NETWORK";

Common::Ini::KeyList windp_keys
{
    Common::Ini::Key("net.gmode",  Common::Ini::KT_DIGIT),               //0
    Common::Ini::Key("net.versioncheck", Common::Ini::KT_BOOL, true),
    Common::Ini::Key("game.debug", Common::Ini::KT_BOOL)
};

size_t NC_STACK_windp::func0(IDVList &stak)
{
    if ( !NC_STACK_network::func0(stak))
        return 0;

    if (!init())
    {
        delete_class_obj(this);
        return 0;
    }

    Common::Ini::ParseIniFile(DefaultIniFile, &windp_keys);
    guaranteed_md = windp_keys[0].Get<int>();
    version_check = windp_keys[1].Get<bool>();
    debug = windp_keys[2].Get<bool>();
    return 1;
}

void NC_STACK_windp::_clear()
{
    for(int i = 0; i < MAX_PROVIDERS; i++)
        providers[i].clear();

    connType = 0;
    
    _hasLobby = false;
    _myName = "";
    
    _nextUsersGet = 0;
    _nextSessionsGet = 0;
    
    _sesID = 0;
    _sesName = "";

    version_ident = "";
    version_check = 0;

    norm_block = NULL;

    norm_size = 0;

    sndBuff = NULL;
    sndBuff_size = 0;
    sndBuff_off = 0;
    guaranteed_md = 0;

    debug = 0;

    zcli = NULL;
    zhost = NULL;
    zcon = NULL;
}

bool NC_STACK_windp::init()
{
    _clear();

    norm_block = (char *)malloc(0x10000);
    if (!norm_block)
    {
        deinit();
        return false;
    }

    norm_size = 0x10000;

    sndBuff = (char *)malloc(0x400);

    if (!sndBuff)
    {
        deinit();
        return false;
    }

    sndBuff_size = 0x400;

    return true;
}

void NC_STACK_windp::deinit()
{

}

size_t NC_STACK_windp::func1()
{
    return NC_STACK_nucleus::func1();
}


size_t NC_STACK_windp::SetMode(bool hosting)
{
    if (hosting)
    {
        connType = 1;
        zhost = new ZNDNet::ZNDSingle(SERV_STR);
        zcon = zhost;
    }
    else
    {
        connType = 2;
        zcli = new ZNDNet::ZNDClient(SERV_STR);
        zcon = zcli;
    }
    return 1;
}

size_t NC_STACK_windp::GetMode()
{
    return connType;
}

void NC_STACK_windp::SetWantedName(const std::string &name)
{
    _myName = name;
}

std::string NC_STACK_windp::GetMyName()
{
    return _myName;
}

bool NC_STACK_windp::Connect(const std::string &connString)
{
    if (connType != 2)
        return false;
    
    IPaddress server;
    if ( SDLNet_ResolveHost(&server, connString.c_str(), 61234) != 0)
        return false;
    
    zcli->Start(_myName, server);
    
    uint32_t deadTime = SDL_GetTicks() + CONF_CONN_WAIT;
    bool waitConn = true;
    
    while (waitConn)
    {
        SDL_Delay(5);

        if (SDL_GetTicks() > deadTime)
        {
            zcli->Stop();
            return false;
        }
        
        for(ZNDNet::Event *evt = zcli->Events_PopSystem(); evt != NULL; evt = zcli->Events_PopSystem())
        {
            switch( evt->type )
            {
                case ZNDNet::EVENT_CONNECTED:
                    {
                        ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
                        _myName = dat->name;
                        _hasLobby = dat->value == 1 ? true : false;
                        _myID = dat->id;
                        
                        if (_hasLobby)
                            waitConn = false;
                    }
                    break;
                
                case ZNDNet::EVENT_SESSION_JOIN:
                {
                    ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
                    _sesID = dat->id;
                    _sesName = dat->name;
                    waitConn = false;
                }
                break;

                case ZNDNet::EVENT_DISCONNECT:
                case ZNDNet::EVENT_CONNERR:
                    {
                        zcli->Stop();
                        return false;
                    }
                    break;
                    
                default:
                    break;
            }
            delete evt;
        }
    }
    
    ReloadUsersList();
    ReloadSessionsList();
    
    return true;
}

bool NC_STACK_windp::IsConnected()
{
    if (connType != 2)
        return false;
    
    return zcli->GetStatus() == ZNDNet::NetUser::STATUS_CONNECTED;
}

bool NC_STACK_windp::HasLobby()
{
    if (!IsConnected())
        return false;
    
    return _hasLobby;
}

bool NC_STACK_windp::IsJoined()
{
    if (connType != 2)
        return false;
    
    return zcon->GetMySesID() != 0;
}


bool NC_STACK_windp::Host(const std::string &gameName, int playerNum)
{
    if (connType != 1)
        return false;
    
    zhost->Start(_myName, 61234, gameName, playerNum);
    _myID = zhost->GetMyID();
    
    ReloadUsersList();
    return true;
}



size_t NC_STACK_windp::EnumSessions(IDVPair *stak)
{
    //ask sessions
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::GetSessionName(windp_getNameMsg *sessName)
{
    if (sessName->id < _rawSessions.size())
    {
        sessName->name = _rawSessions.at(sessName->id).name;
        return 1;
    }
    return 0;
}

std::string NC_STACK_windp::GetCurrentSessionName()
{
    return _sesName;
}

size_t NC_STACK_windp::JoinSession(const std::string &sessName)
{
    //join session
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::CreateSession(windp_openSessionMsg *osm)
{
    //open session
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::GetSessionData(IDVPair *stak)
{
    //get session
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::CloseSession(IDVPair *stak)
{
    //close session
    if (connType == 1)
    {
        if (zhost)
            zhost->CloseSession(0);
    }
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::GetSessionStatus()
{
    if (!zcon)
        return SESSION_STATUS_NONE;

    return SESSION_STATUS_NONE;
}

size_t NC_STACK_windp::SetSessionName(const char *sessName)
{
    //set session name
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::CreatePlayer(windp_createPlayerMsg *cp)
{
    //create player
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::DeletePlayer(const char *playerName)
{
    //destroy player
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::EnumPlayers(IDVPair *stak)
{
    //enum players
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::GetPlayerData(windp_arg79 *arg)
{
    //get player data
    if (arg->ID >= _rawUsers.size())
        return 0;
    
    ZNDNet::UserInfo &usr = _rawUsers.at(arg->ID);
    arg->name = usr.name;
    arg->flags = usr.ID == _myID ? 1 : 0;
    return 1;
}

bool NC_STACK_windp::Send(yw_arg181 *arg)
{
    if (!zcon)
        return false;
    
    if (arg->recvFlags == 2)
        zcon->BroadcastData(arg->data, arg->dataSize, (arg->garant ? ZNDNet::PKT_FLAG_GARANT : 0));
    else
    {
        uint64_t id = GetUserID(arg->recvID);
        if (!id)
            return false;
        
        zcon->SendData(id, arg->data, arg->dataSize, (arg->garant ? ZNDNet::PKT_FLAG_GARANT : 0));
    }
    
    return true;
}

bool NC_STACK_windp::Recv(windp_recvMsg *recv)
{
    ZNDNet::Event *evt = zcon->Events_Pop(); 
    
    if (!evt)
        return false;
    
    recv->msgType = RECVMSG_NONE;
    
    bool continueRecv = true;
    
    switch( evt->type )
    {
        case ZNDNet::EVENT_SESSION_LIST:
        {
            if (zcli)
                zcli->GetSessions(_rawSessions);
        }
        break;
        
        case ZNDNet::EVENT_SESSION_JOIN:
        {
            ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
            _sesID = dat->id;
            _sesName = dat->name;
        }
        break;
        
        case ZNDNet::EVENT_USER_ADD:
        {
            ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
            zcon->GetUsers(_rawUsers);
            
            recv->msgType = RECVMSG_CREATEPLAYER;
            recv->senderName = dat->name;
        }
        break;

        case ZNDNet::EVENT_USER_LEAVE:
        {
            ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
            zcon->GetUsers(_rawUsers); 
            
            recv->msgType = RECVMSG_DESTROYPLAYER;
            recv->senderName = dat->name;
        }
        break;
        
        case ZNDNet::EVENT_DATA:
        {
            ZNDNet::EventData *dat = (ZNDNet::EventData *)evt;
            
            recv->msgType = RECVMSG_NORMAL;
            recv->_data.assign(dat->data, dat->data + dat->size);
            recv->data = recv->_data.data();
            recv->size = recv->_data.size();
            recv->senderName = GetUserName(dat->from);
            recv->cast = dat->cast;
        }
        break;
        
        case ZNDNet::EVENT_USER_LIST:
        {
            zcon->GetUsers(_rawUsers);                
        }
        break;

        case ZNDNet::EVENT_DISCONNECT:
        case ZNDNet::EVENT_CONNERR:
        {
            zcon->Stop();
            continueRecv = false;
        }
        break;

        default:
            break;
    }
    delete evt;
    
    return continueRecv;
}

size_t NC_STACK_windp::FlushBuffer(windp_arg82 &stak)
{
    // flush buffer
    if (sndBuff_off == 0)
        return 0;
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::GetCaps(IDVPair *stak)
{
    // get caps info
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::LockSession(int *arg)
{
    // lock session
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::Reset(IDVPair *stak)
{
    if (zcli)
    {
        zcli->SendDisconnect();
        zcli->Stop();
        delete zcli;
    }
    else if (zhost)
    {
        zhost->CloseSession(0);
        zhost->Stop();
        delete zhost;
    }
    
    zcon = NULL;
    zcli = NULL;
    zhost = NULL;
    // reset
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

size_t NC_STACK_windp::CountPlayers(IDVPair *stak)
{
    // get num players
    //printf("%s\n", __PRETTY_FUNCTION__);
    return _rawUsers.size();
}

bool NC_STACK_windp::GetRemoteStart(windp_arg87 *arg)
{
    //check remote start
    arg->isClient = 0;
    arg->isHoster = 0;

    if (!zcon)
        return true;

    if (zcon == zhost)
    {
        arg->isHoster = 1;
        return true;
    }

    if (zcon == zcli)
    {
        arg->isClient = 1;
        return true;
    }

    return 1;
}

size_t NC_STACK_windp::GetLocalName(IDVPair *stak)
{
    //ask local machine
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

void NC_STACK_windp::SetVersion(const std::string &ver)
{
    //set session ident
    version_ident = ver;
}

size_t NC_STACK_windp::GetProvType()
{
    // get provider type
    printf("%s\n", __PRETTY_FUNCTION__);
    return connType;
}

size_t NC_STACK_windp::GetStats(int *stak)
{
    // diagnos
    printf("%s\n", __PRETTY_FUNCTION__);
    return 0;
}

int NC_STACK_windp::getNumPlayers()
{
    return _rawUsers.size();
}

void NC_STACK_windp::ReloadUsersList()
{
    if (connType > 0 && zcon && SDL_GetTicks() > _nextUsersGet)
    {
        _nextUsersGet = SDL_GetTicks() + CONF_USERS_GET_TIME;
        zcon->GetUsers(_rawUsers);
    }
}

void NC_STACK_windp::ReloadSessionsList()
{
    if (connType > 0 && zcli && SDL_GetTicks() > _nextSessionsGet)
    {
        _nextSessionsGet = SDL_GetTicks() + CONF_SESS_GET_TIME;
        zcli->GetSessions(_rawSessions);
    }
}


uint64_t NC_STACK_windp::GetUserID(const std::string &name)
{
    for(const ZNDNet::UserInfo& inf : _rawUsers)
        if (name.compare(inf.name) == 0)
            return inf.ID;
    return 0;
}

std::string NC_STACK_windp::GetUserName(uint64_t id)
{
    for(const ZNDNet::UserInfo& inf : _rawUsers)
        if (inf.ID == id)
            return inf.name;
    return "";
}

















void UserData::yw_FractionInit()
{
    mapINFO *lvl = &p_ypaworld->LevelNet->mapInfos[ netLevelID ];

    FreeFraction = 0;

    if ( lvl->fractions_mask & 2 )
        FreeFraction |= FREE_FRACTION_RESISTANCE;

    if ( lvl->fractions_mask & 0x40 )
        FreeFraction |= FREE_FRACTION_GHORKOV;

    if ( lvl->fractions_mask & 8 )
        FreeFraction |= FREE_FRACTION_MIKO;

    if ( lvl->fractions_mask & 0x10 )
        FreeFraction |= FREE_FRACTION_TAER;

    uint32_t frkt = 0;
    uint32_t msk = p_ypaworld->LevelNet->mapInfos[ netLevelID ].fractions_mask;
    if ( msk & 2 )
    {
        frkt = FREE_FRACTION_RESISTANCE;
    }
    else if ( msk & 0x40 )
    {
        frkt = FREE_FRACTION_GHORKOV;
    }
    else if ( msk & 8 )
    {
        frkt = FREE_FRACTION_MIKO;
    }
    else if ( msk & 0x10 )
    {
        frkt = FREE_FRACTION_TAER;
    }

    SelectedFraction = frkt;
    FreeFraction &= ~frkt;
}

void UserData::AfterMapChoose()
{
    if ( netSel < 0 )
        return;
    
    switch ( p_YW->windp->GetMode() )
    {
        case 1:
        {
            std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", callSIGN, "|", p_ypaworld->buildDate);
            p_YW->windp->Host(bff, 4);
            
            p_YW->isNetGame = 1;
            netSel = -1;
            netSelMode = 4;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;


            std::string myName = p_YW->windp->GetMyName();

            strcpy(players2[ 0 ].name, myName.c_str());

            yw_FractionInit();

            rdyStart = 1;
            players2[0].rdyStart = 1;
            players2[0].cd = 1;
            cd = 1;
            last_cdchk = glblTime;
        }
        break;
            
        case 2:
            
            break;
        
        default:
            break;
    }
    return;

    if ( p_ypaworld->windp->GetSessionStatus() )
    {
        mapINFO *lvl = &p_ypaworld->LevelNet->mapInfos[ netLevelID ];

        int numpl = p_ypaworld->windp->CountPlayers(NULL);

        netSelMode = 4;
        netName = "";
        netNameCurPos = 0;
        network_listvw.firstShownEntries = 0;
        network_listvw.selectedEntry = 0;
        netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl->fractions_mask & 2 )
        {
            SelectedFraction = FREE_FRACTION_RESISTANCE;
            FreeFraction &= ~FREE_FRACTION_RESISTANCE;
            players2[ plid ].Fraction = FREE_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( lvl->fractions_mask & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_GHORKOV;
                players2[ plid ].Fraction = FREE_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( lvl->fractions_mask & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_MIKO;
                players2[ plid ].Fraction = FREE_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_MIKO;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( lvl->fractions_mask & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_TAER;
                players2[ plid ].Fraction = FREE_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_TAER;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_TAER;
        }

        for(int i = 0; i < 4; i++)
            players2[ i ].checksum = 0;

        uamessage_setLevel msg_slvl;
        msg_slvl.lvlID = netLevelID;
        msg_slvl.msgID = UAMSG_SETLEVEL;
        msg_slvl.owner = 0;

        yw_arg181 ywmsg;
        ywmsg.data = &msg_slvl;
        ywmsg.dataSize = sizeof(msg_slvl);
        ywmsg.recvFlags = 2;
        ywmsg.recvID = 0;
        ywmsg.garant = 1;
        p_YW->ypaworld_func181(&ywmsg);

        p_ypaworld->SendCRC(netLevelID);

        windp_arg82 flushmsg;
        flushmsg.senderID = callSIGN.c_str();
        flushmsg.senderFlags = 1;
        flushmsg.receiverID = NULL;
        flushmsg.receiverFlags = 2;
        flushmsg.guarant = 1;

        p_ypaworld->windp->FlushBuffer(flushmsg);

        std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", callSIGN, "|", p_ypaworld->buildDate);

        p_ypaworld->windp->SetSessionName(bff.c_str());
    }
    else if ( remoteMode )
    {
        mapINFO *lvl = &p_ypaworld->LevelNet->mapInfos[ netLevelID ];

        int numpl = p_ypaworld->windp->CountPlayers(NULL);

        netSelMode = 4;
        netName = "";
        netNameCurPos = 0;
        network_listvw.firstShownEntries = 0;
        network_listvw.selectedEntry = 0;
        netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl->fractions_mask & 2 )
        {
            SelectedFraction = FREE_FRACTION_RESISTANCE;
            FreeFraction &= ~FREE_FRACTION_RESISTANCE;
            players2[ plid ].Fraction = FREE_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( lvl->fractions_mask & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_GHORKOV;
                players2[ plid ].Fraction = FREE_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( lvl->fractions_mask & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_MIKO;
                players2[ plid ].Fraction = FREE_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_MIKO;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( lvl->fractions_mask & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~FREE_FRACTION_TAER;
                players2[ plid ].Fraction = FREE_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = FREE_FRACTION_TAER;
                }
            }
        }
        else
        {
            FreeFraction &= ~FREE_FRACTION_TAER;
        }

        uamessage_lobbyInit limsg;
        limsg.lvlID = netLevelID;
        limsg.msgID = UAMSG_LOBBYINIT;
        limsg.owner = 0;

        yw_arg181 ywmsg;
        ywmsg.data = &limsg;
        ywmsg.dataSize = sizeof(limsg);
        ywmsg.recvFlags = 2;
        ywmsg.recvID = 0;
        ywmsg.garant = 1;
        p_YW->ypaworld_func181(&ywmsg);

        windp_arg82 flushmsg;
        flushmsg.senderID = callSIGN.c_str();
        flushmsg.senderFlags = 1;
        flushmsg.receiverID = NULL;
        flushmsg.receiverFlags = 2;
        flushmsg.guarant = 1;

        p_ypaworld->windp->FlushBuffer(flushmsg);
    }
    else
    {
        if (isHost)
        {
            std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", callSIGN, "|", p_ypaworld->buildDate);

            if (p_ypaworld->windp->GetProvType() == 4) //MODEM!!!!
                p_ypaworld->_win3d->windd_func320(NULL);

            windp_openSessionMsg os;
            os.name = bff.c_str();
            os.maxplayers = 4;

            size_t res = p_ypaworld->windp->CreateSession(&os);

            if (p_ypaworld->windp->GetProvType() == 4) //MODEM!!!!
                p_ypaworld->_win3d->windd_func321(NULL);

            if ( !res )
                return;
        }

        windp_createPlayerMsg cp;
        cp.name = callSIGN.c_str();
        cp.flags = 1;

        if (p_ypaworld->windp->CreatePlayer(&cp))
        {
            p_ypaworld->isNetGame = 1;
            netSel = -1;
            netSelMode = 4;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;

            int plid = p_ypaworld->windp->getNumPlayers();

            strcpy(players2[ plid - 1 ].name, cp.name);

            yw_FractionInit();

            rdyStart = 1;
            players2[ plid - 1].rdyStart = 1;
            players2[ plid - 1].cd = 1;
            cd = 1;
            last_cdchk = glblTime;
        }
    }
}

void UserData::yw_NetOKProvider()
{
    if ( netSel == 0 || netSel == 1 )
    {
        p_ypaworld->_win3d->windd_func320(NULL);

        if ( p_ypaworld->windp->SetMode(netSel == 0) )
        {
            netSelMode = NETSCREEN_ENTER_NAME;
            netSel = -1;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;

            p_YW->GuiWinClose( &network_listvw );

            netName = callSIGN;

            netNameCurPos = netName.size();
        }

        p_ypaworld->_win3d->windd_func321(NULL);

        update_time_norm = 200;
        flush_time_norm = 200;
    }
}

void UserData::yw_JoinNetGame()
{
    if ( p_ypaworld->windp->GetProvType() != 4 || modemAskSession )
    {
        windp_getNameMsg gName;
        gName.id = netSel;

        if ( p_ypaworld->windp->GetSessionName(&gName) )
        {
            if ( p_ypaworld->windp->GetProvType() == 4 )
                p_ypaworld->_win3d->windd_func320(NULL);

            if ( p_ypaworld->windp->JoinSession(gName.name) )
            {
                if ( p_ypaworld->windp->GetProvType() == 4 )
                    p_ypaworld->_win3d->windd_func321(NULL);

                netName = "";
                netNameCurPos = 0;
                netLevelID = 0;

                std::string buf = gName.name;

                for (size_t i = 0; i < buf.length(); i++)
                {
                    if (buf[i] == '"')
                    {
                        buf.resize(i);

                        netLevelID = atoi(buf.c_str());
                        break;
                    }
                }
                network_listvw.firstShownEntries = 0;
                netLevelName = get_lang_string(p_ypaworld->string_pointers_p2, 1800 + netLevelID, p_ypaworld->LevelNet->mapInfos[ netLevelID ].map_name.c_str());

                windp_arg79 plData;
                plData.ID = 0;
                plData.mode = 0;
                while ( p_ypaworld->windp->GetPlayerData(&plData) )
                {
                    buf = plData.name;

                    if ( !StriCmp(plData.name, callSIGN) )
                    {
                        buf += "X";
                    }

                    strncpy(players2[plData.ID].name, buf.c_str(), sizeof(players2[plData.ID].name));


                    plData.ID++;
                }

                windp_createPlayerMsg crPlayerMsg;
                crPlayerMsg.name = callSIGN.c_str();
                crPlayerMsg.flags = 1;

                if ( p_ypaworld->windp->CreatePlayer(&crPlayerMsg) )
                {
                    p_ypaworld->isNetGame = 1;
                    netSel = -1;
                    netSelMode = 4;
                    netName = "";
                    netNameCurPos = 0;
                    network_listvw.firstShownEntries = 0;
                    network_listvw.selectedEntry = 0;
                    isHost = 0;

                    int plid = p_ypaworld->windp->getNumPlayers();

                    strncpy(players2[plid - 1].name, callSIGN.c_str(), sizeof(players2[plid - 1].name));

                    yw_FractionInit();

                    players2[plid - 1].cd = 1;
                    cd = 1;
                    last_cdchk = glblTime;

                    uamessage_cd cdMsg;
                    cdMsg.cd = cd;
                    cdMsg.msgID = UAMSG_CD;
                    cdMsg.rdy = -1;
                    cdMsg.owner = 0;

                    yw_arg181 ywMsg;
                    ywMsg.dataSize = sizeof(cdMsg);
                    ywMsg.recvFlags = 2;
                    ywMsg.recvID = 0;
                    ywMsg.data = &cdMsg;
                    ywMsg.garant = 1;

                    p_YW->ypaworld_func181(&ywMsg);
                }

                plData.mode = 0;
                plData.ID = 0;

                while ( p_ypaworld->windp->GetPlayerData(&plData) && StriCmp(plData.name, callSIGN) )
                    plData.ID++;

                rdyStart = 0;
                players2[plData.ID].rdyStart = 0;

                p_ypaworld->SendCRC(netLevelID);
            }
            else
            {
                if ( p_ypaworld->windp->GetProvType() == 4 )
                    p_ypaworld->_win3d->windd_func320(NULL);

                //sb_0x46bb54__sub0(p_ypaworld, get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));
                printf("%s: %s\n", get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));

                if ( p_ypaworld->windp->GetProvType() == 4 )
                {
                    netSel = -1;
                    sub_46D698();
                }
                else
                {
                    p_ypaworld->windp->EnumSessions(NULL);
                }
            }
        }
    }
    else
    {
        if ( p_ypaworld->windp->GetProvType() == 4 )
            p_ypaworld->_win3d->windd_func320(NULL);

        if ( p_ypaworld->windp->EnumSessions(NULL) )
        {
            modemAskSession = 1;
        }

        if ( p_ypaworld->windp->GetProvType() == 4 )
            p_ypaworld->_win3d->windd_func321(NULL);
    }
}

void UserData::JoinLobbyLessGame()
{
    if (p_YW->windp->IsConnected())
    {
        uint32_t waitForSession = SDL_GetTicks() + 1000;
        while (SDL_GetTicks() < waitForSession && !p_YW->windp->IsJoined())
            SDL_Delay(10);
        
        if (p_YW->windp->IsJoined())
        {
            netName = "";
            netNameCurPos = 0;
            netLevelID = 0;

            std::string buf = p_YW->windp->GetCurrentSessionName();
            printf("%s\n", buf.c_str());

            for (size_t i = 0; i < buf.length(); i++)
            {
                if (buf[i] == '|')
                {
                    buf.resize(i);

                    netLevelID = std::stoi(buf);
                    break;
                }
            }
            network_listvw.firstShownEntries = 0;
            netLevelName = get_lang_string(p_ypaworld->string_pointers_p2, 1800 + netLevelID, p_ypaworld->LevelNet->mapInfos[ netLevelID ].map_name.c_str());

            windp_arg79 plData;
            plData.ID = 0;
            plData.mode = 0;
            while ( p_ypaworld->windp->GetPlayerData(&plData) )
            {
                buf = plData.name;

                if ( !StriCmp(plData.name, callSIGN) )
                {
                    buf += "X";
                }

                strncpy(players2[plData.ID].name, buf.c_str(), sizeof(players2[plData.ID].name));


                plData.ID++;
            }
                
            p_ypaworld->isNetGame = 1;
            netSel = -1;
            netSelMode = 4;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;
            isHost = 0;
            
            int plid = p_ypaworld->windp->getNumPlayers();

            strncpy(players2[plid - 1].name, callSIGN.c_str(), sizeof(players2[plid - 1].name));

            yw_FractionInit();

            players2[plid - 1].cd = 1;
            cd = 1;
            last_cdchk = glblTime;

            uamessage_cd cdMsg;
            cdMsg.cd = cd;
            cdMsg.msgID = UAMSG_CD;
            cdMsg.rdy = -1;
            cdMsg.owner = 0;

            yw_arg181 ywMsg;
            ywMsg.dataSize = sizeof(cdMsg);
            ywMsg.recvFlags = 2;
            ywMsg.recvID = 0;
            ywMsg.data = &cdMsg;
            ywMsg.garant = 1;

            p_YW->ypaworld_func181(&ywMsg);
        }
    }
    return;
    
    if ( p_ypaworld->windp->GetProvType() != 4 || modemAskSession )
    {
        windp_getNameMsg gName;
        gName.id = netSel;

        if ( p_ypaworld->windp->GetSessionName(&gName) )
        {
            if ( p_ypaworld->windp->GetProvType() == 4 )
                p_ypaworld->_win3d->windd_func320(NULL);

            if ( p_ypaworld->windp->JoinSession(gName.name) )
            {
                if ( p_ypaworld->windp->GetProvType() == 4 )
                    p_ypaworld->_win3d->windd_func321(NULL);

                netName = "";
                netNameCurPos = 0;
                netLevelID = 0;

                std::string buf = gName.name;

                for (size_t i = 0; i < buf.length(); i++)
                {
                    if (buf[i] == '"')
                    {
                        buf.resize(i);

                        netLevelID = atoi(buf.c_str());
                        break;
                    }
                }
                network_listvw.firstShownEntries = 0;
                netLevelName = get_lang_string(p_ypaworld->string_pointers_p2, 1800 + netLevelID, p_ypaworld->LevelNet->mapInfos[ netLevelID ].map_name.c_str());

                windp_arg79 plData;
                plData.ID = 0;
                plData.mode = 0;
                while ( p_ypaworld->windp->GetPlayerData(&plData) )
                {
                    buf = plData.name;

                    if ( !StriCmp(plData.name, callSIGN) )
                    {
                        buf += "X";
                    }

                    strncpy(players2[plData.ID].name, buf.c_str(), sizeof(players2[plData.ID].name));


                    plData.ID++;
                }

                windp_createPlayerMsg crPlayerMsg;
                crPlayerMsg.name = callSIGN.c_str();
                crPlayerMsg.flags = 1;

                if ( p_ypaworld->windp->CreatePlayer(&crPlayerMsg) )
                {
                    p_ypaworld->isNetGame = 1;
                    netSel = -1;
                    netSelMode = 4;
                    netName = "";
                    netNameCurPos = 0;
                    network_listvw.firstShownEntries = 0;
                    network_listvw.selectedEntry = 0;
                    isHost = 0;

                    int plid = p_ypaworld->windp->getNumPlayers();

                    strncpy(players2[plid - 1].name, callSIGN.c_str(), sizeof(players2[plid - 1].name));

                    yw_FractionInit();

                    players2[plid - 1].cd = 1;
                    cd = 1;
                    last_cdchk = glblTime;

                    uamessage_cd cdMsg;
                    cdMsg.cd = cd;
                    cdMsg.msgID = UAMSG_CD;
                    cdMsg.rdy = -1;
                    cdMsg.owner = 0;

                    yw_arg181 ywMsg;
                    ywMsg.dataSize = sizeof(cdMsg);
                    ywMsg.recvFlags = 2;
                    ywMsg.recvID = 0;
                    ywMsg.data = &cdMsg;
                    ywMsg.garant = 1;

                    p_YW->ypaworld_func181(&ywMsg);
                }

                plData.mode = 0;
                plData.ID = 0;

                while ( p_ypaworld->windp->GetPlayerData(&plData) && StriCmp(plData.name, callSIGN.c_str()) )
                    plData.ID++;

                rdyStart = 0;
                players2[plData.ID].rdyStart = 0;

                p_ypaworld->SendCRC(netLevelID);
            }
            else
            {
                if ( p_ypaworld->windp->GetProvType() == 4 )
                    p_ypaworld->_win3d->windd_func320(NULL);

                //sb_0x46bb54__sub0(p_ypaworld, get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));
                printf("%s: %s\n", get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));

                if ( p_ypaworld->windp->GetProvType() == 4 )
                {
                    netSel = -1;
                    sub_46D698();
                }
                else
                {
                    p_ypaworld->windp->EnumSessions(NULL);
                }
            }
        }
    }
    else
    {
        if ( p_ypaworld->windp->GetProvType() == 4 )
            p_ypaworld->_win3d->windd_func320(NULL);

        if ( p_ypaworld->windp->EnumSessions(NULL) )
        {
            modemAskSession = 1;
        }

        if ( p_ypaworld->windp->GetProvType() == 4 )
            p_ypaworld->_win3d->windd_func321(NULL);
    }
}

int yw_DestroyPlayer(NC_STACK_ypaworld *yw, const char *playerName)
{
    int plID = -1;
    UserData *usr = yw->GameShell;

    windp_arg79 getPlDat_msg;
    getPlDat_msg.mode = 0;
    getPlDat_msg.ID = 0;

    while ( yw->windp->GetPlayerData(&getPlDat_msg) )
    {
        if ( !StriCmp(getPlDat_msg.name, playerName) )
        {
            plID = getPlDat_msg.ID;
            break;
        }

        getPlDat_msg.ID++;
    }

    if ( plID != -1 )
    {
        for (int i = plID; i < 3; i++ )
            usr->players2[ i ] = usr->players2[ i + 1 ];
    }

    return yw->windp->DeletePlayer(playerName);
}

void UserData::yw_netcleanup()
{
    windp_arg82 flushmsg;
    flushmsg.senderID = callSIGN.c_str();
    flushmsg.senderFlags = 1;
    flushmsg.receiverID = NULL;
    flushmsg.receiverFlags = 2;
    flushmsg.guarant = 1;

    p_ypaworld->windp->FlushBuffer(flushmsg);

    if( netSelMode == 4 || p_ypaworld->netGameStarted )
        yw_DestroyPlayer( p_ypaworld, callSIGN.c_str() );

    if( ( netSelMode == 4 && isHost) || p_ypaworld->netGameStarted )
        p_ypaworld->windp->CloseSession(NULL);

    p_ypaworld->windp->Reset(NULL);

    netSelMode = 0;

    p_ypaworld->isNetGame = 0;

    network_listvw.firstShownEntries = 0;
    netSel = -1;
    nInputMode = 0;
    isHost = 0;
    blocked = 0;
    netPlayerOwner = 0;
    netLevelID = 0;
    modemAskSession = 0;
    sentAQ = 0;

    for (int i = 0; i < 4; i++)
    {
        players2[ i ].msg[0] = 0;
        players2[ i ].Fraction = 0;
        players2[ i ].rdyStart = 0;
        players2[ i ].name[0] = 0;
        players2[ i ].welcmd = 0;
        players2[ i ].w84upd = 0;
        players2[ i ].checksum = 0;
    }

    for (int i = 0; i < 8; i++)
        players[ i ].name[0] = 0;

    rdyStart = 0;

    p_ypaworld->netGameStarted = 0;

    msgBuffLine = 0;
    lastSender[0] = 0;
    disconnected = 0;
    problemCnt = 0;
    netProblem = 0;
    netProblemCount = 0;
    takTime = 0;
    noSent = 0;
    FreeFraction = (FREE_FRACTION_GHORKOV | FREE_FRACTION_MIKO | FREE_FRACTION_TAER);
    SelectedFraction = FREE_FRACTION_RESISTANCE;
    remoteMode = 0;

    log_netlog("netcleanup:      ende\n");
}

void UserData::sub_46D698()
{
    envMode = ENVMODE_TITLE;

    network_button->Hide();

    titel_button->Show();

    p_YW->GuiWinClose(&network_listvw);

    NC_STACK_button::button_66arg arg66;
    arg66.butID = 1016;
    arg66.field_4 = 2;

    sub_bar_button->button_func73(&arg66);

    arg66.butID = 1219;
    arg66.field_4 = 2;
    sub_bar_button->button_func73(&arg66);

    yw_netcleanup();
}

bool yw_initNetLogFile()
{
    dprintf("MAKE ME %s (multiplayer)\n", "yw_initNetLogFile");
    return true;
}

void UserData::yw_NetPrintStartInfo()
{
    extern char **ypaworld__string_pointers; //declared in yw.cpp

    if ( yw_initNetLogFile() )
    {
        log_netlog("-------------- Start YPA network session ------------------\n\n");

        size_t pT = p_ypaworld->windp->GetProvType();

        if ( pT == 1 )
            log_netlog("Provider: Hosting\n");
        else if ( pT == 2 )
            log_netlog("Provider: Connect\n");
        else
            log_netlog("!!! Unknown provider\n");

        log_netlog("for this povider I send a dplay-msg after %d ms\n", flush_time_norm);
        log_netlog("Following players startet with the game:\n");

        windp_arg79 arg79;
        arg79.mode = 0;
        arg79.ID = 0;

        while ( p_ypaworld->windp->GetPlayerData(&arg79) )
        {
            int id;
            if ( arg79.flags & 1 )
            {
                id = SelectedFraction;
            }
            else
            {
                id = players2[arg79.ID].Fraction;
            }

            switch ( id )
            {
            case FREE_FRACTION_RESISTANCE:
                log_netlog("    %s and plays Resistance\n", arg79.name.c_str());
                break;

            case FREE_FRACTION_MIKO:
                log_netlog("    %s and plays Mykonier\n", arg79.name.c_str());
                break;

            case FREE_FRACTION_TAER:
                log_netlog("    %s and plays Taerkasten\n", arg79.name.c_str());
                break;

            case FREE_FRACTION_GHORKOV:
                log_netlog("    %s and plays Ghorkov\n", arg79.name.c_str());
                break;

            default:
                log_netlog("    %s and plays an unknown race\n", arg79.name.c_str());
                break;
            }

            arg79.ID++;
        }

        if ( isHost )
            log_netlog("\nThe local player is %s and is HOST\n", callSIGN.c_str());
        else
            log_netlog("\nThe local player is %s and is CLIENT\n", callSIGN.c_str());

        const char *tmp = get_lang_string(ypaworld__string_pointers, netLevelID + 1800, p_ypaworld->LevelNet->mapInfos[ netLevelID ].map_name.c_str());
        log_netlog("They play level %d, this is %s\n", netLevelID, tmp);
        log_netlog("the session started at timeindex %d\n", p_ypaworld->timeStamp / 1000);
        log_netlog("\n\n--------------------------------------------------------\n");
    }
    else
    {
        ypa_log_out("Unable to open Network log script\n");
    }
}

int UserData::ypaworld_func158__sub0__sub8(const char **a2, const char **a3)
{
    extern char **ypaworld__string_pointers; //declared in yw.cpp

    int numPlayers = p_ypaworld->windp->CountPlayers(NULL);
    int hasCD = 0;

    for( int i = 0; i < numPlayers; i++ )
    {
        if ( players2[i].cd )
            hasCD++;
    }

    if ( numPlayers > 3 && hasCD < 2 )
    {
        *a2 = get_lang_string(ypaworld__string_pointers, 2485, "YOU NEED 2 CD TO START 4 PLAYER GAME");
        *a3 = get_lang_string(ypaworld__string_pointers, 2444, "2444");
        return 1;
    }
    else if ( hasCD < 1 )
    {
        *a2 = get_lang_string(ypaworld__string_pointers, 2484, "YOU NEED A CD TO START THIS GAME");
        *a3 = get_lang_string(ypaworld__string_pointers, 2443, "2443");
        return 1;
    }

    return 0;
}


void UserData::ypaworld_func151__sub7()
{
//  p_ypaworld->windp->windp_func91(v2);
    log_netlog("---------------- YPA Network Statistics -------------------\n\n");
    log_netlog("Sending:\n");
//  log_netlog("   bytes per second: %d\n", field_5487);
//  log_netlog("   bps minimum:      %d\n", field_547F);
//  log_netlog("   bps maximum:      %d\n", field_5483);
//  log_netlog("   packet size:      %d\n", field_549F);
//  log_netlog("   packet minimum:   %d\n", field_5493);
//  log_netlog("   packet maximum:   %d\n", field_5497);
    log_netlog("receiving:\n");
//  log_netlog("   bytes per second: %d\n", field_547B);
//  log_netlog("   bps minimum:      %d\n", field_5473);
//  log_netlog("   bps maximum:      %d\n", field_5477);
    log_netlog("statistical sr-thread info\n");
//  log_netlog("   max. in send list %d\n", v2[3]);
//  log_netlog("   max. in recv list %d\n", v2[2]);
    log_netlog("\nthe session ended at timeindex %d\n", p_ypaworld->timeStamp / 1000);
    log_netlog("-----------------------------------------------------------\n");

    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func151__sub7");
}




//////////////multiplayer wnd///////////
netgamelst netgame_wnd;
char b_stru_5C85C0__cmdbuf[1024];

void sb_0x451034__sub6(NC_STACK_ypaworld *yw)
{
    netgame_wnd.flags = 0x138;
    netgame_wnd.x = 0;
    netgame_wnd.y = 2 * yw->screen_height / 3;
    netgame_wnd.w = yw->screen_width - 1;
    netgame_wnd.h = 2 * yw->font_default_h + 2;

    netgame_wnd.buttons.resize(2);
    netgame_wnd.buttons[0] = ButtonBox(yw->tiles[0]->map[97].w, 0,
                                       yw->tiles[0]->map[97].w, yw->font_default_h);
    netgame_wnd.buttons[1] = ButtonBox(0, 0,
                                       yw->tiles[0]->map[97].w, yw->font_default_h);

    netgame_wnd.field_1CC = yw->tiles[0]->map[97].w;
    netgame_wnd.field_1D0 = 2 * yw->font_default_h;
    netgame_wnd.field_1D4 = yw->screen_width;
    netgame_wnd.field_1D8 = netgame_wnd.field_1D0;
    netgame_wnd.field_1DC[0] = 0;
    netgame_wnd.field_21C = 0;

    netgame_wnd.cmdstrm.includ = NULL;
    netgame_wnd.cmdstrm.cmdbuf = b_stru_5C85C0__cmdbuf;

}

//netgui update
void ypaworld_func64__sub7__sub5(NC_STACK_ypaworld *yw, InputState *inpt)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub7__sub5");
}
