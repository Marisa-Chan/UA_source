#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "includes.h"
#include "engine_input.h"
#include "windp.h"
#include "button.h"


const NewClassDescr NC_STACK_windp::description("windp.class", &newinstance);

static const char *PROVNAME = "Enet UDP connection";

size_t NC_STACK_windp::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak))
        return 0;

    return 1;
}

size_t NC_STACK_windp::func1(stack_vals *stak)
{
    return NC_STACK_nucleus::func1(stak);
}

size_t NC_STACK_windp::func3(stack_vals *stak)
{
    return NC_STACK_nucleus::func3(stak);
}

size_t NC_STACK_windp::windp_func64(stack_vals *stak)
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

size_t NC_STACK_windp::windp_func67(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func68(stack_vals *stak)
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

size_t NC_STACK_windp::windp_func70(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func71(windp_openSessionMsg *osm)
{
    return 0;
}

size_t NC_STACK_windp::windp_func72(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func73(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func74(stack_vals *stak)
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

size_t NC_STACK_windp::windp_func78(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func79(windp_arg79 *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func80(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func81(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func82(windp_arg82 *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func83(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func84(int *arg)
{
    return 0;
}

size_t NC_STACK_windp::windp_func85(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func86(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func87(windp_arg87 *arg)
{
    printf("STUB %s\n","windp_func87");
    arg->field_41 = 0;

    return 1;
}

size_t NC_STACK_windp::windp_func88(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func89(const char **stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func90(stack_vals *stak)
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
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 3:
        func3( (stack_vals *)data );
        return 1;
    case 64:
        return (size_t)windp_func64( (stack_vals *)data );
    case 65:
        return (size_t)windp_func65( (windp_getNameMsg *)data );
    case 66:
        return (size_t)windp_func66( (const char *)data );
    case 67:
        return (size_t)windp_func67( (stack_vals *)data );
    case 68:
        return (size_t)windp_func68( (stack_vals *)data );
    case 69:
        return (size_t)windp_func69( (windp_getNameMsg *)data );
    case 70:
        return (size_t)windp_func70( (stack_vals *)data );
    case 71:
        return (size_t)windp_func71( (windp_openSessionMsg *)data );
    case 72:
        return (size_t)windp_func72( (stack_vals *)data );
    case 73:
        return (size_t)windp_func73( (stack_vals *)data );
    case 74:
        return (size_t)windp_func74( (stack_vals *)data );
    case 75:
        return (size_t)windp_func75( (const char *)data );
    case 76:
        return (size_t)windp_func76( (windp_createPlayerMsg *)data );
    case 77:
        return (size_t)windp_func77( (const char *)data );
    case 78:
        return (size_t)windp_func78( (stack_vals *)data );
    case 79:
        return (size_t)windp_func79( (windp_arg79 *)data );
    case 80:
        return (size_t)windp_func80( (stack_vals *)data );
    case 81:
        return (size_t)windp_func81( (stack_vals *)data );
    case 82:
        return (size_t)windp_func82( (windp_arg82 *)data );
    case 83:
        return (size_t)windp_func83( (stack_vals *)data );
    case 84:
        return (size_t)windp_func84( (int *)data );
    case 85:
        return (size_t)windp_func85( (stack_vals *)data );
    case 86:
        return (size_t)windp_func86( (stack_vals *)data );
    case 87:
        return (size_t)windp_func87( (windp_arg87 *)data );
    case 88:
        return (size_t)windp_func88( (stack_vals *)data );
    case 89:
        return (size_t)windp_func89( (const char **)data );
    case 90:
        return (size_t)windp_func90( (stack_vals *)data );
    case 91:
        return (size_t)windp_func91( (int *)data );
    default:
        break;
    }
    return NC_STACK_network::compatcall(method_id, data);
}



















void yw_HandleNetMsg(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "yw_HandleNetMsg");
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
        ywmsg.value = &msg_slvl;
        ywmsg.val_size = sizeof(msg_slvl);
        ywmsg.field_14 = 2;
        ywmsg.field_10 = 0;
        ywmsg.field_18 = 1;
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
        ywmsg.value = &limsg;
        ywmsg.val_size = sizeof(limsg);
        ywmsg.field_14 = 2;
        ywmsg.field_10 = 0;
        ywmsg.field_18 = 1;
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
    dprintf("MAKE ME %s (multiplayer)\n", "sb_0x46bb54");
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

void ypaworld_func158__sub1(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub1");
}

void ypaworld_func158__sub2(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub2");
}

int ypaworld_func158__sub0__sub8(UserData *usr, const char**, const char**)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub0__sub8");
    return 0;
}

void sb_0x4deac0(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sb_0x4deac0");
}

void ypaworld_func64__sub18(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub18");
}

void ypaworld_func64__sub10(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub10");
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
    log_netlog("\nthe session ended at timeindex %d\n", usr->p_ypaworld->field_1614 / 1000);
    log_netlog("-----------------------------------------------------------\n");
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
