#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "includes.h"
#include "engine_input.h"
#include "windp.h"
#include "button.h"
#include "yw_net.h"


const NewClassDescr NC_STACK_windp::description("windp.class", &newinstance);

static const char *PROVNAME = "Enet UDP connection";

key_value_stru windp_keys[3] =
{
    {"net.gmode", KEY_TYPE_DIGIT, 0},               //0
    {"net.versioncheck", KEY_TYPE_BOOL, 1},
    {"game.debug", KEY_TYPE_BOOL, 0}
};

size_t NC_STACK_windp::func0(IDVList *stak)
{
    if ( !NC_STACK_network::func0(stak))
        return 0;

    if (!init())
    {
        delete_class_obj(this);
        return 0;
    }

    windp_func64();
    get_keyvalue_from_ini(NULL, windp_keys, 3);
    wdp_intern.guaranteed_md = windp_keys[0].value.val;
    wdp_intern.version_check = windp_keys[1].value.val;
    wdp_intern.debug = windp_keys[2].value.val;
    return 1;
}

void windp_intern::init()
{
    for(int i = 0; i < 64; i++)
        memset(&connections[i], 0, sizeof(connections[i]));

    version_check = 0;

    norm_block = NULL;

    norm_size = 0;

    guaranteed_md = 0;

    debug = 0;
}

bool NC_STACK_windp::init()
{
    wdp_intern.init();

    wdp_intern.norm_block = (char *)malloc(0x10000);
    if (!wdp_intern.norm_block)
    {
        deinit();
        return false;
    }

    wdp_intern.norm_size = 0x10000;

    return true;
}

void NC_STACK_windp::deinit()
{

}

size_t NC_STACK_windp::func1()
{
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_windp::func3(IDVList *stak)
{
    return NC_STACK_nucleus::func3(stak);
}

size_t NC_STACK_windp::windp_func64()
{
    return 0;
}

size_t NC_STACK_windp::windp_func65(windp_getNameMsg *arg)
{
    if (arg->id == 0)
    {
        arg->name = PROVNAME;
        return 1;
    }

    return 0;
}

size_t NC_STACK_windp::windp_func66(const char *provName)
{
    if (strcasecmp(provName, PROVNAME) == 0)
        return 1;
    return 0;
}

size_t NC_STACK_windp::windp_func67(windp_t1 **pconn)
{
    return 0;
}

size_t NC_STACK_windp::windp_func68(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func69(windp_getNameMsg *sessName)
{
    if (sessName->id == 0)
    {
        sessName->name = "80|Not a real session";
        return 1;
    }
    return 0;
}

size_t NC_STACK_windp::windp_func70(const char *sessName)
{
    return 0;
}

size_t NC_STACK_windp::windp_func71(windp_openSessionMsg *osm)
{
    return 0;
}

size_t NC_STACK_windp::windp_func72(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func73(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func74(IDVPair *stak)
{
    return 1;
}

size_t NC_STACK_windp::windp_func75(const char *sessName)
{
    return 0;
}

size_t NC_STACK_windp::windp_func76(windp_createPlayerMsg *cp)
{
    return 0;
}

size_t NC_STACK_windp::windp_func77(const char *playerName)
{
    return 0;
}

size_t NC_STACK_windp::windp_func78(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func79(windp_arg79 *arg)
{
    if (arg->ID == 0)
    {
        arg->name = (char *)"Test User NAME";
        arg->flags = 1;
        return 1;
    }

    return 0;
}

size_t NC_STACK_windp::windp_func80(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func81(windp_recvMsg *recv)
{
    return 0;
}

size_t NC_STACK_windp::windp_func82(windp_arg82 *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func83(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func84(int *arg)
{
    return 0;
}

size_t NC_STACK_windp::windp_func85(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func86(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func87(windp_arg87 *arg)
{
    printf("STUB %s\n","windp_func87");
    arg->field_41 = 0;

    return 1;
}

size_t NC_STACK_windp::windp_func88(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func89(const char **stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func90(IDVPair *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func91(int *stak)
{
    return 0;
}

int NC_STACK_windp::getNumPlayers()
{
    return 0;
}


size_t NC_STACK_windp::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 3:
        func3( (IDVList *)data );
        return 1;
    case 64:
        return (size_t)windp_func64();
    case 65:
        return (size_t)windp_func65( (windp_getNameMsg *)data );
    case 66:
        return (size_t)windp_func66( (const char *)data );
    case 67:
        return (size_t)windp_func67( (windp_t1 **)data );
    case 68:
        return (size_t)windp_func68( (IDVPair *)data );
    case 69:
        return (size_t)windp_func69( (windp_getNameMsg *)data );
    case 70:
        return (size_t)windp_func70( (const char *)data );
    case 71:
        return (size_t)windp_func71( (windp_openSessionMsg *)data );
    case 72:
        return (size_t)windp_func72( (IDVPair *)data );
    case 73:
        return (size_t)windp_func73( (IDVPair *)data );
    case 74:
        return (size_t)windp_func74( (IDVPair *)data );
    case 75:
        return (size_t)windp_func75( (const char *)data );
    case 76:
        return (size_t)windp_func76( (windp_createPlayerMsg *)data );
    case 77:
        return (size_t)windp_func77( (const char *)data );
    case 78:
        return (size_t)windp_func78( (IDVPair *)data );
    case 79:
        return (size_t)windp_func79( (windp_arg79 *)data );
    case 80:
        return (size_t)windp_func80( (IDVPair *)data );
    case 81:
        return (size_t)windp_func81( (windp_recvMsg *)data );
    case 82:
        return (size_t)windp_func82( (windp_arg82 *)data );
    case 83:
        return (size_t)windp_func83( (IDVPair *)data );
    case 84:
        return (size_t)windp_func84( (int *)data );
    case 85:
        return (size_t)windp_func85( (IDVPair *)data );
    case 86:
        return (size_t)windp_func86( (IDVPair *)data );
    case 87:
        return (size_t)windp_func87( (windp_arg87 *)data );
    case 88:
        return (size_t)windp_func88( (IDVPair *)data );
    case 89:
        return (size_t)windp_func89( (const char **)data );
    case 90:
        return (size_t)windp_func90( (IDVPair *)data );
    case 91:
        return (size_t)windp_func91( (int *)data );
    default:
        break;
    }
    return NC_STACK_network::compatcall(method_id, data);
}



















void yw_FractionInit(UserData *usr)
{
    mapINFO *lvl = &usr->p_ypaworld->LevelNet->mapInfos[ usr->netLevelID ];

    usr->FreeFraction = 0;

    if ( lvl->fractions_mask & 2 )
        usr->FreeFraction |= FREE_FRACTION_RESISTANCE;

    if ( lvl->fractions_mask & 0x40 )
        usr->FreeFraction |= FREE_FRACTION_GHORKOV;

    if ( lvl->fractions_mask & 8 )
        usr->FreeFraction |= FREE_FRACTION_MIKO;

    if ( lvl->fractions_mask & 0x10 )
        usr->FreeFraction |= FREE_FRACTION_TAER;

    uint32_t frkt = 0;
    uint32_t msk = usr->p_ypaworld->LevelNet->mapInfos[ usr->netLevelID ].fractions_mask;
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

    usr->SelectedFraction = frkt;
    usr->FreeFraction &= ~frkt;
}

void sub_46B328(UserData *usr)
{
    if ( usr->netSel < 0 )
        return;

    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( usr->p_ypaworld->windp->windp_func74(NULL) )
    {
        mapINFO *lvl = &yw->LevelNet->mapInfos[ usr->netLevelID ];

        int numpl = yw->windp->windp_func86(NULL);

        usr->netSelMode = 4;
        usr->netName[0] = 0;
        usr->netNameCurPos = 0;
        usr->network_listvw.firstShownEntries = 0;
        usr->network_listvw.selectedEntry = 0;
        usr->netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl->fractions_mask & 2 )
        {
            usr->SelectedFraction = FREE_FRACTION_RESISTANCE;
            usr->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
            usr->players2[ plid ].Fraction = FREE_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( lvl->fractions_mask & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_GHORKOV;
                usr->players2[ plid ].Fraction = FREE_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( lvl->fractions_mask & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_MIKO;
                usr->players2[ plid ].Fraction = FREE_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_MIKO;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( lvl->fractions_mask & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_TAER;
                usr->players2[ plid ].Fraction = FREE_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_TAER;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_TAER;
        }

        for(int i = 0; i < 4; i++)
            usr->players2[ i ].checksum = 0;

        uamessage_setLevel msg_slvl;
        msg_slvl.lvlID = usr->netLevelID;
        msg_slvl.msgID = UAMSG_SETLEVEL;
        msg_slvl.owner = 0;

        yw_arg181 ywmsg;
        ywmsg.data = &msg_slvl;
        ywmsg.dataSize = sizeof(msg_slvl);
        ywmsg.recvFlags = 2;
        ywmsg.recvID = 0;
        ywmsg.garant = 1;
        yw->self_full->ypaworld_func181(&ywmsg);

        yw->SendCRC(usr->netLevelID);

        windp_arg82 flushmsg;
        flushmsg.senderID = usr->callSIGN;
        flushmsg.senderFlags = 1;
        flushmsg.receiverID = NULL;
        flushmsg.receiverFlags = 2;
        flushmsg.guarant = 1;

        yw->windp->windp_func82(&flushmsg);

        char bff[300];
        sprintf(bff, "%d%s%s%s%s", usr->netLevelID, "|", usr->callSIGN, "|", usr->p_ypaworld->buildDate);

        yw->windp->windp_func75(bff);
    }
    else if ( usr->remoteMode )
    {
        mapINFO *lvl = &yw->LevelNet->mapInfos[ usr->netLevelID ];

        int numpl = yw->windp->windp_func86(NULL);

        usr->netSelMode = 4;
        usr->netName[0] = 0;
        usr->netNameCurPos = 0;
        usr->network_listvw.firstShownEntries = 0;
        usr->network_listvw.selectedEntry = 0;
        usr->netSel = -1;

        bool selected = false;
        int plid = 0;

        if ( lvl->fractions_mask & 2 )
        {
            usr->SelectedFraction = FREE_FRACTION_RESISTANCE;
            usr->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
            usr->players2[ plid ].Fraction = FREE_FRACTION_RESISTANCE;
            numpl--;
            plid++;
            selected = true;
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( lvl->fractions_mask & 0x40 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_GHORKOV;
                usr->players2[ plid ].Fraction = FREE_FRACTION_GHORKOV;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_GHORKOV;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( lvl->fractions_mask & 8 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_MIKO;
                usr->players2[ plid ].Fraction = FREE_FRACTION_MIKO;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_MIKO;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( lvl->fractions_mask & 0x10 )
        {
            if (numpl > 0)
            {
                numpl--;
                usr->FreeFraction &= ~FREE_FRACTION_TAER;
                usr->players2[ plid ].Fraction = FREE_FRACTION_TAER;
                plid++;
                if (!selected)
                {
                    selected = true;
                    usr->SelectedFraction = FREE_FRACTION_TAER;
                }
            }
        }
        else
        {
            usr->FreeFraction &= ~FREE_FRACTION_TAER;
        }

        uamessage_lobbyInit limsg;
        limsg.lvlID = usr->netLevelID;
        limsg.msgID = UAMSG_LOBBYINIT;
        limsg.owner = 0;

        yw_arg181 ywmsg;
        ywmsg.data = &limsg;
        ywmsg.dataSize = sizeof(limsg);
        ywmsg.recvFlags = 2;
        ywmsg.recvID = 0;
        ywmsg.garant = 1;
        yw->self_full->ypaworld_func181(&ywmsg);

        windp_arg82 flushmsg;
        flushmsg.senderID = usr->callSIGN;
        flushmsg.senderFlags = 1;
        flushmsg.receiverID = NULL;
        flushmsg.receiverFlags = 2;
        flushmsg.guarant = 1;

        yw->windp->windp_func82(&flushmsg);
    }
    else
    {
        if (usr->isHost)
        {
            char bff[300];
            sprintf(bff, "%d%s%s%s%s", usr->netLevelID, "|", usr->callSIGN, "|", usr->p_ypaworld->buildDate);

            if (yw->windp->windp_func90(NULL) == 4) //MODEM!!!!
                yw->win3d->windd_func320(NULL);

            windp_openSessionMsg os;
            os.name = bff;
            os.maxplayers = 4;

            size_t res = yw->windp->windp_func71(&os);

            if (yw->windp->windp_func90(NULL) == 4) //MODEM!!!!
                yw->win3d->windd_func321(NULL);

            if ( !res )
                return;
        }

        windp_createPlayerMsg cp;
        cp.name = usr->callSIGN;
        cp.flags = 1;

        if (yw->windp->windp_func76(&cp))
        {
            usr->p_ypaworld->isNetGame = 1;
            usr->netSel = -1;
            usr->netSelMode = 4;
            usr->netName[0] = 0;
            usr->netNameCurPos = 0;
            usr->network_listvw.firstShownEntries = 0;
            usr->network_listvw.selectedEntry = 0;

            int plid = yw->windp->getNumPlayers();

            strcpy(usr->players2[ plid - 1 ].name, cp.name);

            yw_FractionInit(usr);

            usr->rdyStart = 1;
            usr->players2[ plid - 1].rdyStart = 1;
            usr->players2[ plid - 1].cd = 1;
            usr->cd = 1;
            usr->last_cdchk = usr->glblTime;
        }
    }
}

void yw_NetOKProvider(UserData *usr)
{
    if (usr->netSel >= 0)
    {
        windp_getNameMsg prNameMsg;
        prNameMsg.id = usr->netSel;

        if ( usr->p_ypaworld->windp->windp_func65(&prNameMsg) )
        {
            usr->p_ypaworld->win3d->windd_func320(NULL);

            if ( usr->p_ypaworld->windp->windp_func66(prNameMsg.name) )
            {
                usr->netSelMode = 2;
                usr->netSel = -1;
                usr->network_listvw.firstShownEntries = 0;
                usr->network_listvw.selectedEntry = 0;

                usr->network_listvw.CloseDialog(usr->p_ypaworld);

                strcpy(usr->netName, usr->callSIGN);

                usr->netNameCurPos = strlen(usr->netName);
            }

            usr->p_ypaworld->win3d->windd_func321(NULL);

            int type = usr->p_ypaworld->windp->windp_func90(NULL);

            if ( type != 4 && type != 3 )
            {
                usr->update_time_norm = 200;
                usr->flush_time_norm = 200;
            }
            else
            {
                usr->update_time_norm = 400;
                usr->flush_time_norm = 400;
            }
        }
    }
}

void yw_JoinNetGame(UserData *usr)
{
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    if ( yw->windp->windp_func90(NULL) != 4 || usr->modemAskSession )
    {
        windp_getNameMsg gName;
        gName.id = usr->netSel;

        if ( yw->windp->windp_func69(&gName) )
        {
            if ( yw->windp->windp_func90(NULL) == 4 )
                yw->win3d->windd_func320(NULL);

            if ( yw->windp->windp_func70(gName.name) )
            {
                if ( yw->windp->windp_func90(NULL) == 4 )
                    yw->win3d->windd_func321(NULL);

                usr->netName[0] = 0;
                usr->netNameCurPos = 0;
                usr->netLevelID = 0;

                std::string buf = gName.name;

                for (size_t i = 0; i < buf.length(); i++)
                {
                    if (buf[i] == '"')
                    {
                        buf.resize(i);

                        usr->netLevelID = atoi(buf.c_str());
                        break;
                    }
                }
                usr->network_listvw.firstShownEntries = 0;
                usr->netLevelName = get_lang_string(yw->string_pointers_p2, 1800 + usr->netLevelID, yw->LevelNet->mapInfos[ usr->netLevelID ].map_name);

                windp_arg79 plData;
                plData.ID = 0;
                plData.mode = 0;
                while ( yw->windp->windp_func79(&plData) )
                {
                    buf = plData.name;

                    if ( !strcasecmp(plData.name, usr->callSIGN) )
                    {
                        buf += "X";
                    }

                    strncpy(yw->GameShell->players2[plData.ID].name, buf.c_str(), sizeof(yw->GameShell->players2[plData.ID].name));


                    plData.ID++;
                }

                windp_createPlayerMsg crPlayerMsg;
                crPlayerMsg.name = usr->callSIGN;
                crPlayerMsg.flags = 1;

                if ( yw->windp->windp_func76(&crPlayerMsg) )
                {
                    yw->isNetGame = 1;
                    usr->netSel = -1;
                    usr->netSelMode = 4;
                    usr->netName[0] = 0;
                    usr->netNameCurPos = 0;
                    usr->network_listvw.firstShownEntries = 0;
                    usr->network_listvw.selectedEntry = 0;
                    usr->isHost = 0;

                    int plid = yw->windp->getNumPlayers();

                    strncpy(usr->players2[plid - 1].name, usr->callSIGN, sizeof(usr->players2[plid - 1].name));

                    yw_FractionInit(usr);

                    usr->players2[plid - 1].cd = 1;
                    usr->cd = 1;
                    usr->last_cdchk = usr->glblTime;

                    uamessage_cd cdMsg;
                    cdMsg.cd = usr->cd;
                    cdMsg.msgID = UAMSG_CD;
                    cdMsg.rdy = -1;
                    cdMsg.owner = 0;

                    yw_arg181 ywMsg;
                    ywMsg.dataSize = sizeof(cdMsg);
                    ywMsg.recvFlags = 2;
                    ywMsg.recvID = 0;
                    ywMsg.data = &cdMsg;
                    ywMsg.garant = 1;

                    yw->self_full->ypaworld_func181(&ywMsg);
                }

                plData.mode = 0;
                plData.ID = 0;

                while ( yw->windp->windp_func79(&plData) && strcasecmp(plData.name, usr->callSIGN) )
                    plData.ID++;

                usr->rdyStart = 0;
                usr->players2[plData.ID].rdyStart = 0;

                yw->SendCRC(usr->netLevelID);
            }
            else
            {
                if ( yw->windp->windp_func90(NULL) == 4 )
                    yw->win3d->windd_func320(NULL);

                //sb_0x46bb54__sub0(usr->p_ypaworld, get_lang_string(yw->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(yw->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));
                printf("%s: %s\n", get_lang_string(yw->string_pointers_p2, 2400, "YPA ERROR MESSAGE"), get_lang_string(yw->string_pointers_p2, 2401, "SESSION NOT LONGER AVAILABLE"));

                if ( yw->windp->windp_func90(NULL) == 4 )
                {
                    usr->netSel = -1;
                    sub_46D698(usr);
                }
                else
                {
                    yw->windp->windp_func68(NULL);
                }
            }
        }
    }
    else
    {
        if ( yw->windp->windp_func90(NULL) == 4 )
            yw->win3d->windd_func320(NULL);

        if ( yw->windp->windp_func68(NULL) )
        {
            usr->modemAskSession = 1;
        }

        if ( yw->windp->windp_func90(NULL) == 4 )
            yw->win3d->windd_func321(NULL);
    }
}

int yw_DestroyPlayer(_NC_STACK_ypaworld *yw, const char *playerName)
{
    int plID = -1;
    UserData *usr = yw->GameShell;

    windp_arg79 getPlDat_msg;
    getPlDat_msg.mode = 0;
    getPlDat_msg.ID = 0;

    while ( yw->windp->windp_func79(&getPlDat_msg) )
    {
        if ( !strcasecmp(getPlDat_msg.name, playerName) )
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

    return yw->windp->windp_func77(playerName);
}

void yw_netcleanup(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    if (usr)
    {
        windp_arg82 flushmsg;
        flushmsg.senderID = usr->callSIGN;
        flushmsg.senderFlags = 1;
        flushmsg.receiverID = NULL;
        flushmsg.receiverFlags = 2;
        flushmsg.guarant = 1;

        yw->windp->windp_func82(&flushmsg);

        if( usr->netSelMode == 4 || yw->netGameStarted )
            yw_DestroyPlayer( yw, usr->callSIGN );

        if( ( usr->netSelMode == 4 && usr->isHost) || yw->netGameStarted )
            yw->windp->windp_func73(NULL);

        yw->windp->windp_func85(NULL);

        usr->netSelMode = 0;
        yw->isNetGame = 0;
        usr->network_listvw.firstShownEntries = 0;
        usr->netSel = -1;
        usr->nInputMode = 0;
        usr->isHost = 0;
        usr->blocked = 0;
        usr->netPlayerOwner = 0;
        usr->netLevelID = 0;
        usr->modemAskSession = 0;
        usr->sentAQ = 0;

        for (int i = 0; i < 4; i++)
        {
            usr->players2[ i ].msg[0] = 0;
            usr->players2[ i ].Fraction = 0;
            usr->players2[ i ].rdyStart = 0;
            usr->players2[ i ].name[0] = 0;
            usr->players2[ i ].welcmd = 0;
            usr->players2[ i ].w84upd = 0;
            usr->players2[ i ].checksum = 0;
        }

        for (int i = 0; i < 8; i++)
            usr->players[ i ].name[0] = 0;

        usr->rdyStart = 0;
        yw->netGameStarted = 0;
        usr->msgBuffLine = 0;
        usr->lastSender[0] = 0;
        usr->disconnected = 0;
        usr->problemCnt = 0;
        usr->netProblem = 0;
        usr->netProblemCount = 0;
        usr->takTime = 0;
        usr->noSent = 0;
        usr->FreeFraction = (FREE_FRACTION_GHORKOV | FREE_FRACTION_MIKO | FREE_FRACTION_TAER);
        usr->SelectedFraction = FREE_FRACTION_RESISTANCE;
        usr->remoteMode = 0;
    }

    log_netlog("netcleanup:      ende\n");
}

void sub_46D698(UserData *usr)
{
    usr->field_46 = 1;

    int v4 = 2;
    usr->network_button->button_func68(&v4);

    v4 = 1;
    usr->titel_button->button_func68(&v4);

    usr->network_listvw.CloseDialog(usr->p_ypaworld);

    button_66arg arg66;
    arg66.butID = 1016;
    arg66.field_4 = 2;

    usr->sub_bar_button->button_func73(&arg66);

    arg66.butID = 1219;
    arg66.field_4 = 2;
    usr->sub_bar_button->button_func73(&arg66);

    yw_netcleanup(usr->p_ypaworld);
}

bool yw_initNetLogFile()
{
    dprintf("MAKE ME %s (multiplayer)\n", "yw_initNetLogFile");
    return true;
}

void yw_NetPrintStartInfo(UserData *usr)
{
    extern char **ypaworld__string_pointers; //declared in yw.cpp

    if ( yw_initNetLogFile() )
    {
        log_netlog("-------------- Start YPA network session ------------------\n\n");

        windp_t1 *arg67 = NULL;
        usr->p_ypaworld->windp->windp_func67(&arg67);

        if ( arg67 )
            log_netlog("Provider: %s\n", arg67->name);
        else
            log_netlog("!!! Unknown provider\n");

        log_netlog("for this povider I send a dplay-msg after %d ms\n", usr->flush_time_norm);
        log_netlog("Following players startet with the game:\n");

        windp_arg79 arg79;
        arg79.mode = 0;
        arg79.ID = 0;

        while ( usr->p_ypaworld->windp->windp_func79(&arg79) )
        {
            int id;
            if ( arg79.flags & 1 )
            {
                id = usr->SelectedFraction;
            }
            else
            {
                id = usr->players2[arg79.ID].Fraction;
            }

            switch ( id )
            {
            case FREE_FRACTION_RESISTANCE:
                log_netlog("    %s and plays Resistance\n", arg79.name);
                break;

            case FREE_FRACTION_MIKO:
                log_netlog("    %s and plays Mykonier\n", arg79.name);
                break;

            case FREE_FRACTION_TAER:
                log_netlog("    %s and plays Taerkasten\n", arg79.name);
                break;

            case FREE_FRACTION_GHORKOV:
                log_netlog("    %s and plays Ghorkov\n", arg79.name);
                break;

            default:
                log_netlog("    %s and plays an unknown race\n", arg79.name);
                break;
            }

            arg79.ID++;
        }

        if ( usr->isHost )
            log_netlog("\nThe local player is %s and is HOST\n", usr->callSIGN);
        else
            log_netlog("\nThe local player is %s and is CLIENT\n", usr->callSIGN);

        const char *tmp = get_lang_string(ypaworld__string_pointers, usr->netLevelID + 1800, usr->p_ypaworld->LevelNet->mapInfos[ usr->netLevelID ].map_name);
        log_netlog("They play level %d, this is %s\n", usr->netLevelID, tmp);
        log_netlog("the session started at timeindex %d\n", usr->p_ypaworld->timeStamp / 1000);
        log_netlog("\n\n--------------------------------------------------------\n");
    }
    else
    {
        ypa_log_out("Unable to open Network log script\n");
    }
}

int ypaworld_func158__sub0__sub8(UserData *usr, const char **a2, const char **a3)
{
    extern char **ypaworld__string_pointers; //declared in yw.cpp

    int numPlayers = usr->p_ypaworld->windp->windp_func86(NULL);
    int hasCD = 0;

    for( int i = 0; i < numPlayers; i++ )
    {
        if ( usr->players2[i].cd )
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


void ypaworld_func151__sub7(UserData *usr)
{
//  usr->p_ypaworld->windp->windp_func91(v2);
    log_netlog("---------------- YPA Network Statistics -------------------\n\n");
    log_netlog("Sending:\n");
//  log_netlog("   bytes per second: %d\n", usr->field_5487);
//  log_netlog("   bps minimum:      %d\n", usr->field_547F);
//  log_netlog("   bps maximum:      %d\n", usr->field_5483);
//  log_netlog("   packet size:      %d\n", usr->field_549F);
//  log_netlog("   packet minimum:   %d\n", usr->field_5493);
//  log_netlog("   packet maximum:   %d\n", usr->field_5497);
    log_netlog("receiving:\n");
//  log_netlog("   bytes per second: %d\n", usr->field_547B);
//  log_netlog("   bps minimum:      %d\n", usr->field_5473);
//  log_netlog("   bps maximum:      %d\n", usr->field_5477);
    log_netlog("statistical sr-thread info\n");
//  log_netlog("   max. in send list %d\n", v2[3]);
//  log_netlog("   max. in recv list %d\n", v2[2]);
    log_netlog("\nthe session ended at timeindex %d\n", usr->p_ypaworld->timeStamp / 1000);
    log_netlog("-----------------------------------------------------------\n");

    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func151__sub7");
}




//////////////multiplayer wnd///////////
netgamelst netgame_wnd;
char b_stru_5C85C0__cmdbuf[1024];
ButtonBox b_stru_5C85C0__btn0;
ButtonBox b_stru_5C85C0__btn1;


void sb_0x451034__sub6(_NC_STACK_ypaworld *yw)
{
    netgame_wnd.flags = 0x138;
    netgame_wnd.dialogBox.xpos = 0;
    netgame_wnd.dialogBox.ypos = 2 * yw->screen_height / 3;
    netgame_wnd.dialogBox.buttons[0] = &b_stru_5C85C0__btn0;
    netgame_wnd.dialogBox.buttons[1] = &b_stru_5C85C0__btn1;
    netgame_wnd.dialogBox.btn_width = yw->screen_width - 1;
    netgame_wnd.dialogBox.btn_height = 2 * yw->font_default_h + 2;
    netgame_wnd.dialogBox.field_10 = 2;


    netgame_wnd.field_1CC = yw->tiles[0]->chars[97].width;
    netgame_wnd.field_1D0 = 2 * yw->font_default_h;
    netgame_wnd.field_1D4 = yw->screen_width;
    netgame_wnd.field_1D8 = netgame_wnd.field_1D0;
    netgame_wnd.field_1DC[0] = 0;
    netgame_wnd.field_21C = 0;

    netgame_wnd.cmdstrm.includ = NULL;
    netgame_wnd.cmdstrm.cmdbuf = b_stru_5C85C0__cmdbuf;

    b_stru_5C85C0__btn0.x = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.y = 0;
    b_stru_5C85C0__btn0.w = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.h = yw->font_default_h;

    b_stru_5C85C0__btn1.x = 0;
    b_stru_5C85C0__btn1.y = 0;
    b_stru_5C85C0__btn1.w = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn1.h = yw->font_default_h;
}

//netgui update
void ypaworld_func64__sub7__sub5(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub7__sub5");
}
