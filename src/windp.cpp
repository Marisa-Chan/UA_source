#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "includes.h"
#include "system/inpt.h"
#include "windp.h"
#include "button.h"
#include "yw_net.h"
#include "system/inivals.h"
#include "locale/locale.h"

const char *SERV_STR = "UA:SOURCE TEST NETWORK";

size_t NC_STACK_windp::Init(IDVList &stak)
{
    if ( !NC_STACK_network::Init(stak))
        return 0;

    if (!init())
    {
        Delete();
        return 0;
    }

    System::IniConf::ReadFromNucleusIni();
    guaranteed_md = System::IniConf::NetGmode.Get<int>();
    version_check = System::IniConf::NetVersionCheck.Get<bool>();
    debug = System::IniConf::GameDebug.Get<bool>();
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

    broadcastBuff.resize(CONF_SEND_SIZE);

    if (broadcastBuff.empty())
    {
        deinit();
        return false;
    }

    return true;
}

void NC_STACK_windp::deinit()
{

}

size_t NC_STACK_windp::Deinit()
{
    return NC_STACK_nucleus::Deinit();
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


bool NC_STACK_windp::GetPlayerData(uint32_t index, TDPPlayerData *out) const
{
    //get player data
    if (index >= _rawUsers.size())
        return 0;
    
    const ZNDNet::UserInfo &usr = _rawUsers.at(index);
    out->name = usr.name;
    out->flags = usr.ID == _myID ? TDPPlayerData::NPD_ITSME : 0;
    out->Index = index;
    return 1;
}

std::vector<TDPPlayerData> NC_STACK_windp::GetPlayersData() const
{
    std::vector<TDPPlayerData> tmp;
    tmp.resize( _rawUsers.size() );
    
    for(uint32_t i = 0; i < _rawUsers.size(); i++)
    {
        TDPPlayerData &p = tmp[i];
        const ZNDNet::UserInfo &usr = _rawUsers.at(i);
        p.Index = i;
        p.name = usr.name;
        p.flags = usr.ID == _myID ? TDPPlayerData::NPD_ITSME : 0;
    }
    return tmp;
}

int32_t NC_STACK_windp::GetMyIndex() const
{
    return _myIndex;
}

std::string NC_STACK_windp::GetPlayerName(uint32_t index) const
{
    if (index >= _rawUsers.size())
        return std::string();
    return _rawUsers.at(index).name;
}

int32_t NC_STACK_windp::GetPlayerIndex(const std::string &name) const
{
    for(uint32_t i = 0; i < _rawUsers.size(); i++)
    {
        if ( !StriCmp(_rawUsers[i].name, name) )
            return i;
    }
    return -1;
}

bool NC_STACK_windp::IsPlayer(const std::string &name) const
{
    for(const ZNDNet::UserInfo &p : _rawUsers)
    {
        if ( !StriCmp(p.name, name) )
            return true;
    }
    return false;
}


bool NC_STACK_windp::Broadcast(void *data, size_t dataSize, bool garantee)
{
    return QueueBroadcast(data, dataSize, garantee);
}

bool NC_STACK_windp::Send(void *data, size_t dataSize, const std::string &recvID, bool garantee)
{
    FlushBroadcastBuffer();
    
    return SendLowLevel(data, dataSize, recvID, garantee);
}

size_t NC_STACK_windp::QueueBroadcast(void *data, size_t dataSize, bool garantee)
{
    if (!zcon)
        return 0;
    
    if ( broadcastBuff_off + dataSize > broadcastBuff.size() )
        FlushBroadcastBuffer();

    if ( dataSize > broadcastBuff.size() )
        return BroadcastLowLevel(data, dataSize, garantee);

    std::memcpy( broadcastBuff.data() + broadcastBuff_off, data, dataSize );
    broadcastBuff_off += dataSize;

    return dataSize;
}

size_t NC_STACK_windp::BroadcastLowLevel(void *data, size_t dataSize, bool garantee)
{
    if (!zcon)
        return 0;
    
    zcon->BroadcastData(data, dataSize, (garantee ? ZNDNet::PKT_FLAG_GARANT : 0));
    
    return dataSize;
}

size_t NC_STACK_windp::SendLowLevel(void *data, size_t dataSize, const std::string &recvID, bool garantee)
{
    if (!zcon)
        return 0;

    uint64_t id = GetUserID(recvID);
    if (!id)
        return 0;

    zcon->SendData(id, data, dataSize, (garantee ? ZNDNet::PKT_FLAG_GARANT : 0));
 
    return dataSize;
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
            UpdateOwnPlayerIndex();
            
            recv->msgType = RECVMSG_CREATEPLAYER;
            recv->senderName = dat->name;
        }
        break;

        case ZNDNet::EVENT_USER_LEAVE:
        {
            ZNDNet::EventNameID *dat = (ZNDNet::EventNameID *)evt;
            zcon->GetUsers(_rawUsers); 
            UpdateOwnPlayerIndex();
            
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
            UpdateOwnPlayerIndex();
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

size_t NC_STACK_windp::FlushBroadcastBuffer()
{
    // flush buffer
    if (broadcastBuff_off == 0)
        return 0;
    
    size_t ret = BroadcastLowLevel(broadcastBuff.data(), broadcastBuff_off, true);

    if ( ret )
        broadcastBuff_off = 0;

    return ret;
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

uint32_t NC_STACK_windp::GetPlayerCount() const
{
    return _rawUsers.size();
}

bool NC_STACK_windp::GetRemoteStart(windp_arg87 *arg)
{
    //check remote start
    arg->isClient = false;
    arg->isHoster = false;

    if (!zcon)
        return true;

    if (zcon == zhost)
    {
        arg->isHoster = true;
        return true;
    }

    if (zcon == zcli)
    {
        arg->isClient = true;
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


void NC_STACK_windp::ReloadUsersList()
{
    if (connType > 0 && zcon && SDL_GetTicks() > _nextUsersGet)
    {
        _nextUsersGet = SDL_GetTicks() + CONF_USERS_GET_TIME;
        zcon->GetUsers(_rawUsers);
        UpdateOwnPlayerIndex();
    }
}

void NC_STACK_windp::UpdateOwnPlayerIndex()
{
    for(_myIndex = 0; _myIndex < (int32_t)_rawUsers.size(); _myIndex++)
    {
        if (_rawUsers[_myIndex].ID == _myID)
            return;
    }
    
    _myIndex = -1;
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
    TMapRegionInfo &lvl = p_YW->_globalMapRegions.MapRegions[ netLevelID ];

    FreeFraction = 0;

    if ( lvl.FractionsBits & 2 )
        FreeFraction |= NET_FRACTION_RESISTANCE;

    if ( lvl.FractionsBits & 0x40 )
        FreeFraction |= NET_FRACTION_GHORKOV;

    if ( lvl.FractionsBits & 8 )
        FreeFraction |= NET_FRACTION_MIKO;

    if ( lvl.FractionsBits & 0x10 )
        FreeFraction |= NET_FRACTION_TAER;

    uint32_t frkt = 0;
    uint32_t msk = p_YW->_globalMapRegions.MapRegions[ netLevelID ].FractionsBits;
    if ( msk & 2 )
    {
        frkt = NET_FRACTION_RESISTANCE;
    }
    else if ( msk & 0x40 )
    {
        frkt = NET_FRACTION_GHORKOV;
    }
    else if ( msk & 8 )
    {
        frkt = NET_FRACTION_MIKO;
    }
    else if ( msk & 0x10 )
    {
        frkt = NET_FRACTION_TAER;
    }

    SelectedFraction = frkt;
    FreeFraction &= ~frkt;
}

void UserData::AfterMapChoose()
{
    if ( netSel < 0 )
        return;
    
    switch ( p_YW->_netDriver->GetMode() )
    {
        case 1:
        {
            std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", p_YW->_netDriver->GetMyName(), "|", p_YW->_buildDate);
            p_YW->_netDriver->Host(bff, 4);
            
            p_YW->_isNetGame = true;
            netSel = -1;
            netSelMode = NETSCREEN_INSESSION;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;

            lobbyPlayers[ 0 ].Name = p_YW->_netDriver->GetMyName();

            yw_FractionInit();

            rdyStart = true;
            lobbyPlayers[0].Ready = true;
        }
        break;
            
        case 2:
            
            break;
        
        default:
            break;
    }
    return;

    if ( p_YW->_netDriver->GetSessionStatus() )
    {
        TMapRegionInfo &lvl = p_YW->_globalMapRegions.MapRegions[ netLevelID ];

        int numpl = p_YW->_netDriver->GetPlayerCount();

        netSelMode = NETSCREEN_INSESSION;
        netName = "";
        netNameCurPos = 0;
        network_listvw.firstShownEntries = 0;
        network_listvw.selectedEntry = 0;
        netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl.FractionsBits & 2 )
        {
            SelectedFraction = NET_FRACTION_RESISTANCE;
            FreeFraction &= ~NET_FRACTION_RESISTANCE;
            lobbyPlayers[ plid ].NetFraction = NET_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_RESISTANCE;
        }

        if ( lvl.FractionsBits & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_GHORKOV;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_GHORKOV;
        }

        if ( lvl.FractionsBits & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_MIKO;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_MIKO;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_MIKO;
        }

        if ( lvl.FractionsBits & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_TAER;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_TAER;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_TAER;
        }

        for(TNetPlayerLobbyData &pl : lobbyPlayers)
            pl.DataChecksum = 0;

        uamessage_setLevel msg_slvl;
        msg_slvl.lvlID = netLevelID;
        msg_slvl.msgID = UAMSG_SETLEVEL;
        msg_slvl.owner = 0;

        p_YW->NetBroadcastMessage(&msg_slvl, sizeof(msg_slvl), true);

        p_YW->SendCRC(netLevelID);

        p_YW->_netDriver->FlushBroadcastBuffer();

        std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", netPlayerName, "|", p_YW->_buildDate);

        p_YW->_netDriver->SetSessionName(bff.c_str());
    }
    else if ( remoteMode )
    {
        TMapRegionInfo &lvl = p_YW->_globalMapRegions.MapRegions[ netLevelID ];

        int numpl = p_YW->_netDriver->GetPlayerCount();

        netSelMode = NETSCREEN_INSESSION;
        netName = "";
        netNameCurPos = 0;
        network_listvw.firstShownEntries = 0;
        network_listvw.selectedEntry = 0;
        netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl.FractionsBits & 2 )
        {
            SelectedFraction = NET_FRACTION_RESISTANCE;
            FreeFraction &= ~NET_FRACTION_RESISTANCE;
            lobbyPlayers[ plid ].NetFraction = NET_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_RESISTANCE;
        }

        if ( lvl.FractionsBits & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_GHORKOV;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_GHORKOV;
        }

        if ( lvl.FractionsBits & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_MIKO;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_MIKO;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_MIKO;
        }

        if ( lvl.FractionsBits & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                FreeFraction &= ~NET_FRACTION_TAER;
                lobbyPlayers[ plid ].NetFraction = NET_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    SelectedFraction = NET_FRACTION_TAER;
                }
            }
        }
        else
        {
            FreeFraction &= ~NET_FRACTION_TAER;
        }

        uamessage_lobbyInit limsg;
        limsg.lvlID = netLevelID;
        limsg.msgID = UAMSG_LOBBYINIT;
        limsg.owner = 0;

        p_YW->NetBroadcastMessage(&limsg, sizeof(limsg), true);

        p_YW->_netDriver->FlushBroadcastBuffer();
    }
    else
    {
        if (isHost)
        {
            std::string bff = fmt::sprintf("%d%s%s%s%s", netLevelID, "|", netPlayerName, "|", p_YW->_buildDate);

            if (p_YW->_netDriver->GetProvType() == 4) //MODEM!!!!
                GFX::Engine.windd_func320(NULL);

            windp_openSessionMsg os;
            os.name = bff.c_str();
            os.maxplayers = 4;

            size_t res = p_YW->_netDriver->CreateSession(&os);

            if (p_YW->_netDriver->GetProvType() == 4) //MODEM!!!!
                GFX::Engine.windd_func321(NULL);

            if ( !res )
                return;
        }

        windp_createPlayerMsg cp;
        cp.name = netPlayerName.c_str();
        cp.flags = 1;

        if (p_YW->_netDriver->CreatePlayer(&cp))
        {
            p_YW->_isNetGame = true;
            netSel = -1;
            netSelMode = NETSCREEN_INSESSION;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;

            int plid = p_YW->_netDriver->GetPlayerCount();

            lobbyPlayers[ plid - 1 ].Name = cp.name;

            yw_FractionInit();

            rdyStart = true;
            lobbyPlayers[ plid - 1].Ready = true;
        }
    }
}

void UserData::yw_NetOKProvider()
{
    if ( netSel == 0 || netSel == 1 )
    {
        GFX::Engine.windd_func320(NULL);

        if ( p_YW->_netDriver->SetMode(netSel == 0) )
        {
            netSelMode = NETSCREEN_ENTER_NAME;
            netSel = -1;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;

            p_YW->GuiWinClose( &network_listvw );

            netName = netPlayerName;

            netNameCurPos = netName.size();
        }

        GFX::Engine.windd_func321(NULL);

        update_time_norm = 200;
        flush_time_norm = 200;
    }
}

void UserData::yw_JoinNetGame()
{
    if ( p_YW->_netDriver->GetProvType() != 4 || modemAskSession )
    {
        windp_getNameMsg gName;
        gName.id = netSel;

        if ( p_YW->_netDriver->GetSessionName(&gName) )
        {
            if ( p_YW->_netDriver->GetProvType() == 4 )
                GFX::Engine.windd_func320(NULL);

            if ( p_YW->_netDriver->JoinSession(gName.name) )
            {
                if ( p_YW->_netDriver->GetProvType() == 4 )
                    GFX::Engine.windd_func321(NULL);

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
                netLevelName = p_YW->GetLevelName(netLevelID);

                for(TDPPlayerData &p : p_YW->_netDriver->GetPlayersData())
                {
                    lobbyPlayers[p.Index].Name = p.name;
                    
                    if (p.IsItMe())
                    {
                        lobbyPlayers[p.Index].Name += "X";
                    }
                }

                windp_createPlayerMsg crPlayerMsg;
                crPlayerMsg.name = netPlayerName.c_str();
                crPlayerMsg.flags = 1;

                if ( p_YW->_netDriver->CreatePlayer(&crPlayerMsg) )
                {
                    p_YW->_isNetGame = true;
                    netSel = -1;
                    netSelMode = NETSCREEN_INSESSION;
                    netName = "";
                    netNameCurPos = 0;
                    network_listvw.firstShownEntries = 0;
                    network_listvw.selectedEntry = 0;
                    isHost = false;

                    yw_FractionInit();
                }

                rdyStart = false;
                int32_t myIndex = p_YW->_netDriver->GetMyIndex();
                if (myIndex != -1)
                    lobbyPlayers[myIndex].Ready = false;

                p_YW->SendCRC(netLevelID);
            }
            else
            {
                if ( p_YW->_netDriver->GetProvType() == 4 )
                    GFX::Engine.windd_func320(NULL);

                //sb_0x46bb54__sub0(p_ypaworld, get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));
                fmt::printf("%s: %s\n", Locale::Text::Advanced(Locale::ADV_ERRHEAD), Locale::Text::Advanced(Locale::ADV_ERRNOSESS));

                if ( p_YW->_netDriver->GetProvType() == 4 )
                {
                    netSel = -1;
                    ExitFromNetworkToMain();
                }
                else
                {
                    p_YW->_netDriver->EnumSessions(NULL);
                }
            }
        }
    }
    else
    {
        if ( p_YW->_netDriver->GetProvType() == 4 )
            GFX::Engine.windd_func320(NULL);

        if ( p_YW->_netDriver->EnumSessions(NULL) )
        {
            modemAskSession = true;
        }

        if ( p_YW->_netDriver->GetProvType() == 4 )
            GFX::Engine.windd_func321(NULL);
    }
}

void UserData::JoinLobbyLessGame()
{
    if (p_YW->_netDriver->IsConnected())
    {
        uint32_t waitForSession = SDL_GetTicks() + 1000;
        while (SDL_GetTicks() < waitForSession && !p_YW->_netDriver->IsJoined())
            SDL_Delay(10);
        
        if (p_YW->_netDriver->IsJoined())
        {
            netName = "";
            netNameCurPos = 0;
            netLevelID = 0;

            std::string buf = p_YW->_netDriver->GetCurrentSessionName();
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
            netLevelName = p_YW->GetLevelName(netLevelID);

            for(TDPPlayerData &p : p_YW->_netDriver->GetPlayersData())
            {
                lobbyPlayers[p.Index].Name = p.name;

                if (p.IsItMe())
                {
                    lobbyPlayers[p.Index].Name += "X";
                }
            }
                
            p_YW->_isNetGame = true;
            netSel = -1;
            netSelMode = NETSCREEN_INSESSION;
            netName = "";
            netNameCurPos = 0;
            network_listvw.firstShownEntries = 0;
            network_listvw.selectedEntry = 0;
            p_YW->GuiWinOpen( &network_listvw );
            isHost = false;

            yw_FractionInit();
        }
    }
    return;
    
    if ( p_YW->_netDriver->GetProvType() != 4 || modemAskSession )
    {
        windp_getNameMsg gName;
        gName.id = netSel;

        if ( p_YW->_netDriver->GetSessionName(&gName) )
        {
            if ( p_YW->_netDriver->GetProvType() == 4 )
                GFX::Engine.windd_func320(NULL);

            if ( p_YW->_netDriver->JoinSession(gName.name) )
            {
                if ( p_YW->_netDriver->GetProvType() == 4 )
                    GFX::Engine.windd_func321(NULL);

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
                netLevelName = p_YW->GetLevelName(netLevelID);

                for(TDPPlayerData &p : p_YW->_netDriver->GetPlayersData())
                {
                    lobbyPlayers[p.Index].Name = p.name;
                    
                    if (p.IsItMe())
                    {
                        lobbyPlayers[p.Index].Name += "X";
                    }
                }

                windp_createPlayerMsg crPlayerMsg;
                crPlayerMsg.name = netPlayerName.c_str();
                crPlayerMsg.flags = 1;

                if ( p_YW->_netDriver->CreatePlayer(&crPlayerMsg) )
                {
                    p_YW->_isNetGame = true;
                    netSel = -1;
                    netSelMode = NETSCREEN_INSESSION;
                    netName = "";
                    netNameCurPos = 0;
                    network_listvw.firstShownEntries = 0;
                    network_listvw.selectedEntry = 0;
                    isHost = false;

                    yw_FractionInit();
                }

                rdyStart = false;
                int32_t myIndex = p_YW->_netDriver->GetMyIndex();
                if (myIndex != -1)
                    lobbyPlayers[myIndex].Ready = false;

                p_YW->SendCRC(netLevelID);
            }
            else
            {
                if ( p_YW->_netDriver->GetProvType() == 4 )
                    GFX::Engine.windd_func320(NULL);

                //sb_0x46bb54__sub0(p_ypaworld, get_lang_string(p_ypaworld->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(p_ypaworld->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));
                fmt::printf("%s: %s\n", Locale::Text::Advanced(Locale::ADV_ERRHEAD), Locale::Text::Advanced(Locale::ADV_ERRNOSESS));

                if ( p_YW->_netDriver->GetProvType() == 4 )
                {
                    netSel = -1;
                    ExitFromNetworkToMain();
                }
                else
                {
                    p_YW->_netDriver->EnumSessions(NULL);
                }
            }
        }
    }
    else
    {
        if ( p_YW->_netDriver->GetProvType() == 4 )
            GFX::Engine.windd_func320(NULL);

        if ( p_YW->_netDriver->EnumSessions(NULL) )
        {
            modemAskSession = true;
        }

        if ( p_YW->_netDriver->GetProvType() == 4 )
            GFX::Engine.windd_func321(NULL);
    }
}

int yw_DestroyPlayer(NC_STACK_ypaworld *yw, const char *playerName)
{
    int32_t plID = yw->_netDriver->GetPlayerIndex(playerName);
    UserData *usr = yw->_GameShell;

    if ( plID != -1 )
    {
        for (size_t i = plID; i < usr->lobbyPlayers.size() - 1; i++ )
            usr->lobbyPlayers[ i ] = usr->lobbyPlayers[ i + 1 ];
    }

    return yw->_netDriver->DeletePlayer(playerName);
}

void UserData::yw_netcleanup()
{
    p_YW->_netDriver->FlushBroadcastBuffer();

    if( netSelMode ==NETSCREEN_INSESSION || p_YW->_isNetGameStarted )
        yw_DestroyPlayer( p_YW, netPlayerName.c_str() );

    if( ( netSelMode == NETSCREEN_INSESSION && isHost) || p_YW->_isNetGameStarted )
        p_YW->_netDriver->CloseSession(NULL);

    p_YW->_netDriver->Reset(NULL);

    netSelMode = NETSCREEN_MODE_SELECT;

    p_YW->_isNetGame = false;

    network_listvw.firstShownEntries = 0;
    netSel = -1;
    nInputMode = 0;
    isHost = false;
    blocked = false;
    netPlayerOwner = 0;
    netLevelID = 0;
    modemAskSession = false;
    sentAQ = false;

    for (TNetPlayerLobbyData &pl : lobbyPlayers)
        pl = UserData::TNetPlayerLobbyData();

    for (TNetPlayerData &pl : netPlayers)
        pl.Name.clear();

    rdyStart = false;

    p_YW->_isNetGameStarted = false;

    msgBuffers.clear();
    lastSender.clear();
    disconnected = false;
    problemTimer = 0;
    netProblem = 0;
    netProblemCountDown = 0;
    takTime = 0;
    noSent = false;
    FreeFraction = (NET_FRACTION_GHORKOV | NET_FRACTION_MIKO | NET_FRACTION_TAER);
    SelectedFraction = NET_FRACTION_RESISTANCE;
    remoteMode = false;

    log_netlog("netcleanup:      ende\n");
}

void UserData::ExitFromNetworkToMain()
{
    EnvMode = ENVMODE_TITLE;

    network_button->HideScreen();

    titel_button->ShowScreen();

    p_YW->GuiWinClose(&network_listvw);

    NC_STACK_button::button_66arg arg66;
    arg66.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_MULTIPLAYER;
    arg66.field_4 = 2;

    sub_bar_button->SetState(&arg66);

    arg66.butID = 1219;
    arg66.field_4 = 2;
    sub_bar_button->SetState(&arg66);

    yw_netcleanup();
}

bool yw_initNetLogFile()
{
    dprintf("MAKE ME %s (multiplayer)\n", "yw_initNetLogFile");
    return true;
}

void UserData::yw_NetPrintStartInfo()
{
    if ( yw_initNetLogFile() )
    {
        log_netlog("-------------- Start YPA network session ------------------\n\n");

        size_t pT = p_YW->_netDriver->GetProvType();

        if ( pT == 1 )
            log_netlog("Provider: Hosting\n");
        else if ( pT == 2 )
            log_netlog("Provider: Connect\n");
        else
            log_netlog("!!! Unknown provider\n");

        log_netlog("for this povider I send a dplay-msg after %d ms\n", flush_time_norm);
        log_netlog("Following players startet with the game:\n");

        for(uint32_t i = 0; i < p_YW->_netDriver->GetPlayerCount(); i++)
        {
            int id;
            if ( (int32_t)i == p_YW->_netDriver->GetMyIndex() )
            {
                id = SelectedFraction;
            }
            else
            {
                id = lobbyPlayers[i].NetFraction;
            }
            
            std::string name = p_YW->_netDriver->GetPlayerName(i);

            switch ( id )
            {
            case NET_FRACTION_RESISTANCE:
                log_netlog("    %s and plays Resistance\n", name.c_str());
                break;

            case NET_FRACTION_MIKO:
                log_netlog("    %s and plays Mykonier\n", name.c_str());
                break;

            case NET_FRACTION_TAER:
                log_netlog("    %s and plays Taerkasten\n", name.c_str());
                break;

            case NET_FRACTION_GHORKOV:
                log_netlog("    %s and plays Ghorkov\n", name.c_str());
                break;

            default:
                log_netlog("    %s and plays an unknown race\n", name.c_str());
                break;
            }
        }

        if ( isHost )
            log_netlog("\nThe local player is %s and is HOST\n", netPlayerName.c_str());
        else
            log_netlog("\nThe local player is %s and is CLIENT\n", netPlayerName.c_str());

        std::string tmp = p_YW->GetLevelName(netLevelID);
        log_netlog("They play level %d, this is %s\n", netLevelID, tmp.c_str());
        log_netlog("the session started at timeindex %d\n", p_YW->_timeStamp / 1000);
        log_netlog("\n\n--------------------------------------------------------\n");
    }
    else
    {
        ypa_log_out("Unable to open Network log script\n");
    }
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
    log_netlog("\nthe session ended at timeindex %d\n", p_YW->_timeStamp / 1000);
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
    netgame_wnd.y = 2 * yw->_screenSize.y / 3;
    netgame_wnd.w = yw->_screenSize.x - 1;
    netgame_wnd.h = 2 * yw->_fontH + 2;

    netgame_wnd.buttons.resize(2);
    netgame_wnd.buttons[0] = ButtonBox(yw->_guiTiles[0]->map[97].w, 0,
                                       yw->_guiTiles[0]->map[97].w, yw->_fontH);
    netgame_wnd.buttons[1] = ButtonBox(0, 0,
                                       yw->_guiTiles[0]->map[97].w, yw->_fontH);

    netgame_wnd.field_1CC = yw->_guiTiles[0]->map[97].w;
    netgame_wnd.field_1D0 = 2 * yw->_fontH;
    netgame_wnd.field_1D4 = yw->_screenSize.x;
    netgame_wnd.field_1D8 = netgame_wnd.field_1D0;
    netgame_wnd.field_1DC[0] = 0;
    netgame_wnd.field_21C = 0;

    netgame_wnd.cmdstrm.includ = NULL;
    netgame_wnd.cmdstrm.cmdbuf = b_stru_5C85C0__cmdbuf;

}

//netgui update
void ypaworld_func64__sub7__sub5(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub7__sub5");
}
