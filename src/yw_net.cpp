#include "yw.h"
#include "yw_internal.h"
#include "yw_net.h"
#include "log.h"
#include "ypabact.h"
#include "yparobo.h"

bool SPEED_DOWN_NET = false;
static uamessage_vhclDataI vhcldata_interpolate;
static uamessage_vhclDataE vhcldata_exact;
uint32_t HNDL_MSG[108];

extern int bact_id;
extern key_value_stru ypaworld_keys[4];


void _NC_STACK_ypaworld::SendCRC(int lvlid)
{
    uint32_t crc = fileCrc32("data:scripts/feinde.scr");
    crc = fileCrc32("data:scripts/feinde.scr", crc);
    crc = fileCrc32("data:scripts/user.scr", crc);
    crc = fileCrc32("data:scripts/weap_f.scr", crc);
    crc = fileCrc32("data:scripts/weap_u.scr", crc);
    crc = fileCrc32("data:scripts/net_robo.scr", crc);
    crc = fileCrc32("data:scripts/net_bldg.scr", crc);
    crc = fileCrc32("data:scripts/flaks.scr", crc);
    crc = fileCrc32("data:scripts/net_ypa.scr", crc);
    crc = fileCrc32("data:scripts/inetrobo.scr", crc);

    char buf[300];
    sprintf(buf, "levels:multi/L%02d%02d.ldf", lvlid, lvlid);

    crc = fileCrc32(buf, crc);

    GameShell->netCRC = crc;

    uamessage_crc crcmsg;
    crcmsg.msgID = UAMSG_CRC;
    crcmsg.owner = GameShell->netPlayerOwner;
    crcmsg.checksum = crc;

    yw_arg181 ywmsg;
    ywmsg.data = &crcmsg;
    ywmsg.dataSize = sizeof(crcmsg);
    ywmsg.garant = 1;
    ywmsg.recvID = 0;
    ywmsg.recvFlags = 2;

    self_full->ypaworld_func181(&ywmsg);
}

void yw_CheckCRCs(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    std::string strs[4];

    if ( usr->takTime > 0 )
    {
        usr->takTime -= usr->frameTime;
    }
    else
    {
        usr->takTime = 15000;

        int plCnt = yw->windp->windp_func86(NULL);

        bool err = false;
        int errmsg = 0;

        for (int i = 0; i < plCnt; i++)
        {
            if ( strcasecmp(yw->GameShell->callSIGN, yw->GameShell->players2[i].name) &&
                    usr->netCRC != yw->GameShell->players2[i].checksum &&
                    yw->GameShell->players2[i].checksum != 0 &&
                    usr->netCRC != 0 )
            {
                err = true;

                char bf[300];
                sprintf(bf, "%s %s", yw->GameShell->players2[i].name, get_lang_string(yw->string_pointers_p2, 2404, "HAS OTHER FILES THAN YOU") );

                strs[errmsg] = bf;
                errmsg++;
            }
        }

        if ( err )
        {
            const char *lngstr = get_lang_string(yw->string_pointers_p2, 2403, "COMPUTER");

            for (int i = 0; i < errmsg; i++)
            {
                sub_4D0C24(yw, lngstr, strs[i].c_str());
            }
        }
    }
}

void yw_CheckCDs(UserData *usr)
{
    if ( usr->glblTime - usr->last_cdchk >= 1500 )
    {
        usr->last_cdchk = usr->glblTime;

        windp_arg79 plData;
        plData.ID = 0;
        plData.mode = 0;

        while ( usr->p_ypaworld->windp->windp_func79(&plData) )
        {
            if ( !strcasecmp(plData.name, usr->callSIGN) )
            {
                usr->players2[plData.ID].cd = 1;
                break;
            }
            plData.ID++;
        }

        usr->cd = 1;

        uamessage_cd cdMsg;
        cdMsg.cd = 1;
        cdMsg.rdy = usr->rdyStart;
        cdMsg.msgID = UAMSG_CD;
        cdMsg.owner = 0;

        yw_arg181 ywMsg;
        ywMsg.dataSize = sizeof(cdMsg);
        ywMsg.recvFlags = 2;
        ywMsg.recvID = 0;
        ywMsg.garant = 1;
        ywMsg.data = &cdMsg;

        usr->p_ypaworld->self_full->ypaworld_func181(&ywMsg);
    }
}

bool yw_prepareVHCLData(_NC_STACK_ypaworld *yw, uamessage_vhclData *dat)
{
    printf("Make me %s\n", "yw_prepareVHCLData");
    return true;
}

void yw_cleanPlayer(_NC_STACK_ypaworld *yw, const char *name, char owner, char mode)
{
    printf("Make me %s\n", "yw_cleanPlayer");
}

size_t yw_handleNormMsg(_NC_STACK_ypaworld *yw, windp_recvMsg *msg, char *err)
{
    printf("Make me %s\n", "yw_handleNormMsg");
    return 0;
}

const char *yw_corruptionCheck(UserData *usr)
{
    printf("Make me %s\n", "yw_corruptionCheck");
    return NULL;
}

void yw_HandleNetMsg(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    uint32_t upd_time;

    if ( SPEED_DOWN_NET )
        upd_time = 1400;
    else
        upd_time = usr->update_time_norm;

    if ( yw->timeStamp - yw->netUpdateTime > upd_time && usr->netProblem != 1 )
    {
        uamessage_vhclData *vhcldata;
        if ( yw->netInterpolate )
        {
            vhcldata = &vhcldata_interpolate;
            vhcldata_interpolate.msgID = UAMSG_VHCLDATA_I;
        }
        else
        {
            vhcldata = &vhcldata_exact;
            vhcldata_exact.msgID = UAMSG_VHCLDATA_E;
        }

        vhcldata->tstamp = yw->timeStamp;

        if ( yw_prepareVHCLData(yw, vhcldata) )
        {
            yw_arg181 ywMsg;
            ywMsg.data = vhcldata;

            if ( yw->netInterpolate )
                vhcldata->hdr.size = ((char *)&vhcldata_interpolate.data[ vhcldata->hdr.number ]) - ((char *)vhcldata);
            else
                vhcldata->hdr.size = ((char *)&vhcldata_exact.data[ vhcldata->hdr.number ]) - ((char *)vhcldata);

            vhcldata->hdr.diffTime = yw->timeStamp - yw->netUpdateTime;

            ywMsg.dataSize = vhcldata->hdr.size;
            ywMsg.recvID = 0;
            ywMsg.garant = 0;
            ywMsg.recvFlags = 2;

            yw->self_full->ypaworld_func181(&ywMsg);
        }

        yw->netUpdateTime = yw->timeStamp;
    }

    if ( usr->isHost )
    {
        if ( yw->netGameStarted )
        {
            if ( usr->sendScore < 0 )
            {
                uamessage_score scoreMsg;
                scoreMsg.owner = usr->netPlayerOwner;
                scoreMsg.msgID = UAMSG_SCORE;

                for(int i = 0; i < 8; i++)
                    scoreMsg.status[i] = yw->ingamePlayerStatus[i];

                yw_arg181 ywMsg;
                ywMsg.data = &scoreMsg;
                ywMsg.dataSize = sizeof(scoreMsg);
                ywMsg.recvID = 0;
                ywMsg.garant = 0;
                ywMsg.recvFlags = 2;
                yw->self_full->ypaworld_func181(&ywMsg);

                usr->sendScore = 1500;
            }
            else
            {
                usr->sendScore -= usr->frameTime;
            }

        }
    }

    uint32_t msgcount = 0;
    char err_sender[200];

    windp_recvMsg recvMsg;

    while ( yw->windp->windp_func81(&recvMsg) )
    {
        usr->netrecv_count += recvMsg.size;

        msgcount++;

        if ( yw->field_2d90->field_40 != 2 || !yw->netGameStarted )
        {
            if ( yw->windp->windp_func86(NULL) * 5 >= msgcount )
            {
                yw->netInfoOverkill = 0;
            }
            else
            {
                yw->netInfoOverkill = 1;
                log_netlog("Info overkill !!! (msg-count %d at %ds)\n", msgcount, yw->timeStamp / 1000);
                usr->problemCnt = 4000;
            }

            err_sender[0] = 0;

            switch ( recvMsg.msgType )
            {
            case RECVMSG_CREATEPLAYER:
            {

                if ( usr->isHost && usr->remoteMode && usr->netSelMode == 4 )
                {
                    if ( usr->netLevelID > 0 && usr->netLevelID < 256 )
                    {
                        uamessage_lobbyInit lbyMsg;
                        strncpy(lbyMsg.hostName, usr->callSIGN, 64);
                        lbyMsg.msgID = UAMSG_LOBBYINIT;
                        lbyMsg.lvlID = usr->netLevelID;
                        lbyMsg.owner = 0;

                        yw_arg181 ywMsg;
                        ywMsg.data = &lbyMsg;
                        ywMsg.dataSize = sizeof(lbyMsg);
                        ywMsg.garant = 1;
                        ywMsg.recvFlags = 2;
                        ywMsg.recvID = 0;
                        yw->self_full->ypaworld_func181(&ywMsg);
                    }
                }

                uamessage_welcome wlcmMsg;
                wlcmMsg.owner = 0;
                wlcmMsg.msgID = UAMSG_WELCOME;
                wlcmMsg.fraction = usr->SelectedFraction;
                wlcmMsg.cd = usr->cd;
                wlcmMsg.rdy = usr->rdyStart;

                yw_arg181 ywMsg;
                ywMsg.data = &wlcmMsg;
                ywMsg.dataSize = sizeof(&wlcmMsg);
                ywMsg.recvFlags = 1;
                ywMsg.garant = 1;
                ywMsg.recvID = (char *)recvMsg.data;
                yw->self_full->ypaworld_func181(&ywMsg);

                if ( usr->netLevelID )
                    yw->SendCRC(usr->netLevelID);

                int plid = yw->windp->getNumPlayers();

                strncpy(usr->players2[plid - 1].name, (const char *)recvMsg.data, 64);
                usr->players2[plid - 1].welcmd = 1;
            }
            break;

            case RECVMSG_DESTROYPLAYER:
            {
                bool itisI = false;
                bool plFound = false;

                if ( !strcasecmp(usr->callSIGN, (const char *)recvMsg.data) )
                {
                    itisI = true;
                    yw->field_2d90->field_40 = 2;
                }

                for(int i = 0; i < 4; i++)
                {
                    if ( strcasecmp(usr->players2[i].name, (const char *)recvMsg.data) == 0 )
                    {
                        for (int j = i; j < 3; j++)
                        {
                            usr->players2[j] = usr->players2[ j + 1 ];
                        }

                        plFound = true;
                        break;
                    }
                }

                log_netlog(">>> Received a destroy player for %s at %d\n", recvMsg.data, yw->timeStamp / 1000);

                if ( usr->field_0x0 == 0 && plFound)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if ( strcasecmp(usr->players[i].name, (const char *)recvMsg.data) == 0 )
                        {
                            if ( usr->players[i].status != 3 )
                                usr->players[i].status = 4;
                            break;
                        }
                    }

                    if ( !yw->netGameStarted && !itisI )
                    {
                        if ( usr->netProblem != 3 )
                        {
                            usr->netProblem = 4;
                            usr->netProblemCount = 15000;
                        }
                    }

                    yw_cleanPlayer(yw, (const char *)recvMsg.data, 0, 0);
                }
                else
                {
                    if ( plFound )
                        log_netlog("    but netgame was not startet!\n");
                    else
                        log_netlog("    but the player doesn't exist anymore!\n");
                }
            }
            break;

            case RECVMSG_HOST:
                usr->isHost = 1;
                if ( !yw->netGameStarted )
                {
                    int v70 = 0;
                    yw->windp->windp_func84(&v70);

                    usr->rdyStart = 1;
                    usr->blocked = 0;

                    windp_arg79 plData;
                    plData.ID = 0;
                    plData.mode = 0;
                    while ( yw->windp->windp_func79(&plData) )
                    {
                        if ( !strcasecmp(usr->callSIGN, plData.name) )
                        {
                            usr->players2[plData.ID].rdyStart = 1;
                            break;
                        }

                        plData.ID++;
                    }
                }
                break;

            case RECVMSG_NORMAL:
            {
                size_t msgSizes = 0;
                uint32_t msgcnt = 0;

                while ( 1 )
                {
                    msgcnt++;

                    if ( msgcnt > 100 )
                    {
                        log_netlog("Strange Number of Messages (>100)\n");
                        break;
                    }

                    uint32_t msgID = ((uamessage_base *)recvMsg.data)->msgID;
                    HNDL_MSG[ msgcnt ] = msgID;

                    size_t msg_size = yw_handleNormMsg(yw, &recvMsg, err_sender);

                    if ( !msg_size )
                    {
                        log_netlog("    unknown message was number %d and message before was type %d\n", msgcnt, HNDL_MSG[ msgcnt - 1 ]);
                        log_netlog("    current offset is %d, size is %d\n", msgSizes, recvMsg.size);
                        break;
                    }

                    if ( msg_size < sizeof(uamessage_base) )
                    {
                        log_netlog("Error: Message %d has strange size %d!\n", msgID, msg_size);
                        break;
                    }


                    msgSizes += msg_size;

                    if ( msgSizes >= recvMsg.size )
                        break;
                    else
                        recvMsg.data = (char *)recvMsg.data + msg_size;
                }

                const char *crpt = yw_corruptionCheck(usr);
                if ( crpt )
                    strncpy(err_sender, crpt, 200);
            }
            break;

            default:
                break;
            }

            if ( err_sender[0] )
            {
                for (int i = 0; i < 4; i++)
                {
                    if (strcasecmp(usr->players2[i].name, err_sender) == 0)
                    {
                        if ( !usr->players2[i].w84upd )
                        {
                            log_netlog("Drastic Error: Request Update from %s\n", err_sender);

                            uamessage_requpdate updMsg;
                            updMsg.msgID = UAMSG_REQUPDATE;
                            updMsg.owner = usr->netPlayerOwner;

                            yw_arg181 ywMsg;
                            ywMsg.data = &updMsg;
                            ywMsg.dataSize = sizeof(updMsg);
                            ywMsg.recvFlags = 1;
                            ywMsg.garant = 1;
                            ywMsg.recvID = err_sender;
                            yw->self_full->ypaworld_func181(&ywMsg);

                            usr->players2[i].w84upd = 2000;
                        }
                        break;
                    }
                }
            }
        }
    }

    usr->netrecv_time += yw->field_1b24.frameTime;

    if ( usr->netrecv_time > 3000 )
    {
        if ( usr->netrecv_time )
        {
            usr->netrecv_speed = 1000 * usr->netrecv_count / usr->netrecv_time;
            usr->netsend_speed = 1000 * usr->netsend_count / usr->netrecv_time;
        }

        if ( usr->netrecv_bpsMin )
        {
            if ( usr->netrecv_bpsMin > usr->netrecv_speed )
                usr->netrecv_bpsMin = usr->netrecv_speed;
        }
        else
        {
            usr->netrecv_bpsMin = usr->netrecv_speed;
        }

        if ( usr->netsend_bpsMin )
        {
            if ( usr->netsend_bpsMin > usr->netsend_speed )
                usr->netsend_bpsMin = usr->netsend_speed;
        }
        else
        {
            usr->netsend_bpsMin = usr->netsend_speed;
        }

        if ( usr->netrecv_bpsMax < usr->netrecv_speed )
            usr->netrecv_bpsMax = usr->netrecv_speed;

        if ( usr->netsend_bpsMax < usr->netsend_speed )
            usr->netsend_bpsMax = usr->netsend_speed;

        usr->netsend_bpsAvr = (usr->netsend_bpsAvr * usr->net_gcnt + usr->netsend_speed) / (usr->net_gcnt + 1);
        usr->netrecv_bpsAvr = (usr->netrecv_bpsAvr * usr->net_gcnt + usr->netrecv_speed) / (usr->net_gcnt + 1);

        if ( usr->net_packet_cnt )
            usr->net_packet_size = usr->netsend_count / usr->net_packet_cnt;

        usr->net_packet_avr = (usr->net_packet_avr * usr->net_gcnt + usr->net_packet_size) / (usr->net_gcnt + 1);

        usr->net_gcnt++;

        usr->netrecv_count = 0;
        usr->netsend_count = 0;
        usr->net_packet_cnt = 0;
        usr->netrecv_time = 0;
    }
}



bool yw_NetSetHostStations(_NC_STACK_ypaworld *yw, mapRobo *mapHosts, int hosts_cnt)
{
    UserData *usr = yw->GameShell;

    yw->field_2d90->ownerMap__has_vehicles = 0;
    yw->field_2d90->field_60 = 0;

    for (int i = 0; i < 8; i++)
        usr->players[i].rdyStart = 0;

    mapRobo *selHost = NULL;

    int owner = 0;

    windp_arg79 plData;
    plData.ID = 0;
    plData.mode = 0;

    while ( yw->windp->windp_func79(&plData) )
    {
        if ( plData.ID == hosts_cnt )
        {
            log_netlog("No Robo declared for player %d\n", hosts_cnt);
            return false;
        }

        int selFraction;

        if ( plData.flags & 1 )
        {
            selFraction = usr->SelectedFraction;
        }
        else
        {
            selFraction = usr->players2[plData.ID].Fraction;
        }

        if ( !selFraction )
        {
            log_netlog("Error no race for robo %s found\n", plData.name);
            return false;
        }

        switch ( selFraction - 1 )
        {
        case 0:
            owner = 1;
            break;
        case 1:
            owner = 6;
            break;
        case 3:
            owner = 3;
            break;
        case 7:
            owner = 4;
            break;
        case 2:
        case 4:
        case 5:
        case 6:
            break;
        }

        selHost = NULL;
        usr->players2[plData.ID].owner = owner;

        for (int i = 0; i < hosts_cnt; i++)
        {
            if ( owner == mapHosts[i].owner )
            {
                selHost = &mapHosts[i];
                break;
            }
        }

        if ( !selHost )
        {
            log_netlog("Oh! No owner %d specified in LDF, but somebody wanted to play it!\n", owner);
            return false;
        }

        xyz place(selHost->pos_x, selHost->pos_y, selHost->pos_z);

        strncpy(usr->players[owner].name, plData.name, 64);

        ypaworld_arg136 arg136;
        arg136.pos_x = place.sx;
        arg136.pos_y = -30000.0;
        arg136.pos_z = place.sz;
        arg136.field_18 = 50000.0;
        arg136.field_14 = 0;
        arg136.field_1C = 0;
        arg136.field_40 = 0;

        yw->self_full->ypaworld_func136(&arg136);

        if ( arg136.field_20 )
            place.sy += arg136.field_30;
        else
            log_netlog("Warning: Robo placed without y-correction\n");

        ypaworld_arg146 arg146;
        switch ( selFraction - 1 )
        {
        case 0:
            arg146.vehicle_id = 56;
            break;
        case 1:
            arg146.vehicle_id = 57;
            break;
        case 3:
            arg146.vehicle_id = 58;
            break;
        case 7:
            arg146.vehicle_id = 60;
            break;
        case 2:
        case 4:
        case 5:
        case 6:
            break;
        }
        arg146.pos = place;

        NC_STACK_ypabact *hostObj = yw->self_full->ypaworld_func146(&arg146);

        if ( !hostObj )
        {
            log_netlog("NetPlaceRobos: Unable to create robo (owner %d, type %d)\n", plData.ID + 1, arg146.vehicle_id);
            return false;
        }

        __NC_STACK_ypabact *hostBact = hostObj->getBACT_pBact();

        hostBact->owner = owner;
        hostBact->gid |= owner << 24;

        yw->field_2d90->ownerMap__has_vehicles |= 1 << owner;

        for (bact_node *bct = (bact_node *)hostBact->subjects_list.head; bct->next; bct = (bact_node *)bct->next )
        {
            bct->bact->gid |= owner << 24;
            bct->bact->owner = owner;
        }

        if ( plData.flags & 1 )
        {
            hostObj->setBACT_viewer(1);
            hostObj->setBACT_inputting(1);
            yw->GameShell->netPlayerOwner = owner;
            yw->field_2d90->field_60 |= 1 << owner;
        }

        yw->self_full->ypaworld_func134(hostObj);

        hostObj->setBACT_bactCollisions(1);

        NC_STACK_yparobo *robo = static_cast<NC_STACK_yparobo *>(hostObj);
        robo->setROBO_fillMode(15);

        int enrg = selHost->energy / 4;

        robo->setROBO_battVehicle(enrg);
        robo->setROBO_battBeam(enrg);
        robo->setROBO_viewAngle(selHost->viewangle);

        hostBact->energy = enrg;
        hostBact->energy_max = enrg;

        if ( selHost->reload_const )
            hostBact->reload_const = selHost->reload_const;
        else
            hostBact->reload_const = hostBact->energy_max;

        plData.ID++;
    }
    return true;
}



size_t NC_STACK_ypaworld::ypaworld_func179(yw_arg161 *arg)
{
//  _NC_STACK_ypaworld *yw; // edi@1
//  int v4; // ecx@1
//  int v5; // ebx@1
//  unsigned int v6; // ebp@7
//  unsigned int v7; // esi@8
//  int v8; // ebx@9
//  cellArea *v9; // edx@9
//  int result; // eax@14
//  int v11; // eax@23
//  int v12; // eax@25
//  bact_node *i; // eax@27
//  bact_node *v14; // esi@29
//  UserData *v15; // edx@33
//  bact_node *v16; // edx@34
//  unsigned int v17; // ebp@34
//  int v18; // ebx@35
//  UserData *v19; // eax@42
//  int v20; // ecx@42
//  int v21; // ecx@42
//   // [sp+0h] [bp-14DCh]@1
//  uamessage_syncgame syncMsg; // [sp+1458h] [bp-84h]@34
//  yw_arg181 ywMsg; // [sp+148Ch] [bp-50h]@42
//  windp_arg82 windp82; // [sp+14A8h] [bp-34h]@42
//  int a4; // [sp+14BCh] [bp-20h]@37
//  int v27; // [sp+14C0h] [bp-1Ch]@1
//  int v28; // [sp+14C4h] [bp-18h]@1

    _NC_STACK_ypaworld *yw = &stack__ypaworld;
    bact_id = 0x10000;

    mapProto proto;
    bool loadOK = false;

    if ( sb_0x44ca90(yw, &proto, arg->lvlID, arg->field_4) )
    {
        if ( cells_mark_type(yw, proto.typ) )
        {
            if ( cells_mark_owner(yw, proto.own) )
            {
                if ( cells_mark_hight(yw, proto.hgt) )
                {
                    if ( yw_NetSetHostStations(yw, proto.mapRobos, proto.mapRobos_count) )
                    {
                        if ( sub_44B9B8(this, yw, proto.blg) )
                        {
                            for (int y = 0; y < yw->sectors_maxY2; y++)
                            {
                                for (int x = 0; x < yw->sectors_maxX2; x++)
                                {
                                    cellArea *cell = &yw->cells[x + y * yw->sectors_maxX2];
                                    sb_0x44fc60(yw, cell, x, y, 255, 0);
                                }
                            }

                            yw_InitTechUpgradeBuildings(this, yw);
                            yw_InitGates(yw);
                            yw_InitSuperItems(yw);
                            sub_44F748(yw);

                            if ( sb_0x451034(this, yw) )
                                loadOK = true;
                        }
                    }
                }
            }
        }
    }

    if ( !loadOK )
    {
        log_netlog("Unable to init network level (1)\n");
        ypaworld_func151(NULL);
        return 0;
    }

    if ( yw->copyof_typemap )
    {
        delete_class_obj(yw->copyof_typemap);
        yw->copyof_typemap = 0;
    }

    if ( yw->copyof_ownermap )
    {
        delete_class_obj(yw->copyof_ownermap);
        yw->copyof_ownermap = 0;
    }

    if ( yw->typ_map )
        yw->copyof_typemap = sub_44816C(yw->typ_map, "copyof_typemap");

    if ( yw->own_map )
        yw->copyof_ownermap = sub_44816C(yw->own_map, "copyof_ownermap");

    yw->GameShell->msgcount = 0;

    for (int i = 0; i < 4; i++)
    {
        yw->GameShell->players2[i].msg[0] = 0;
        yw->GameShell->players2[i].trbl = 0;
        yw->GameShell->players2[i].w84upd = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        yw->GameShell->players[i].isKilled = 0;
        yw->GameShell->players[i].rdyStart = 0;
        yw->GameShell->players[i].lastMsgTime = yw->timeStamp + 1000;
        yw->GameShell->players[i].tstamp = 0;
        yw->GameShell->players[i].status = 0;
        yw->GameShell->players[i].problemCnt = 0;
    }

    for (bact_node *bct = (bact_node *)yw->bact_list.head; bct->next; bct = (bact_node *)bct->next )
        yw->GameShell->players[ bct->bact->owner ].status = 1;

    bool fnd = false;
    bact_node *fndHost = (bact_node *)yw->bact_list.head;

    while (fndHost->next)
    {
        if (fndHost->bact->owner == yw->GameShell->netPlayerOwner)
        {
            fnd = true;
            break;
        }

        fndHost = (bact_node *)fndHost->next;
    }

    yw->GameShell->players[ yw->GameShell->netPlayerOwner ].rdyStart = 1;
    yw->netGameStarted = 0;

    yw->netStartTime = ypaworld_keys[1].value.val;
    yw->GameShell->kickTime = ypaworld_keys[2].value.val;

    log_netlog("netstarttime was initialized with %d sec\n", yw->netStartTime / 1000);
    log_netlog("kickoff was initialized with      %d sec\n", yw->GameShell->kickTime / 1000);

    if ( fnd )
    {
        uamessage_syncgame syncMsg;
        syncMsg.owner = yw->GameShell->netPlayerOwner;
        syncMsg.msgID = UAMSG_SYNCGM;
        syncMsg.hostID = fndHost->bact->gid;

        int gn = 0;
        for(bact_node *bct = (bact_node *)fndHost->bact->subjects_list.head; bct->next; bct = (bact_node *)bct->next)
        {
            if (gn >= 8)
                break;

            int rbgun = 0;
            if (bct->bact->bact_type == BACT_TYPES_GUN)
                rbgun = ((NC_STACK_ypagun *)bct->bacto)->getGUN_roboGun();

            if (rbgun)
            {
                syncMsg.gun[gn] = bct->bact->gid;
                gn++;
            }
        }

        yw_arg181 ywMsg;
        ywMsg.data = &syncMsg;
        ywMsg.dataSize = sizeof(syncMsg);
        ywMsg.recvFlags = 2;
        ywMsg.recvID = 0;
        ywMsg.garant = 1;
        yw->self_full->ypaworld_func181(&ywMsg);

        windp_arg82 windp82;
        windp82.senderFlags = 1;
        windp82.receiverFlags = 2;
        windp82.receiverID = 0;
        windp82.senderID = yw->GameShell->callSIGN;
        windp82.guarant = 1;

        yw->windp->windp_func82(&windp82);

        log_netlog("Sent a READY TO PLAY to all for my Owner %d\n", yw->GameShell->netPlayerOwner);

        fndHost->bacto->setBACT_viewer(1);
    }

    yw->GameShell->netsend_count = 0;
    yw->GameShell->netrecv_count = 0;
    yw->GameShell->netrecv_time = 0;
    yw->GameShell->netsend_speed = 0;
    yw->GameShell->netrecv_speed = 0;
    yw->GameShell->netrecv_bpsMin = 0;
    yw->GameShell->netrecv_bpsMax = 0;
    yw->GameShell->netrecv_bpsAvr = 0;
    yw->GameShell->netsend_bpsMin = 0;
    yw->GameShell->netsend_bpsMax = 0;
    yw->GameShell->netsend_bpsAvr = 0;
    yw->GameShell->net_gcnt = 0;
    yw->GameShell->net_packet_size = 0;
    yw->GameShell->net_packet_min = 0;
    yw->GameShell->net_packet_max = 0;
    yw->GameShell->net_packet_cnt = 0;
    yw->GameShell->net_packet_avr = 0;
    yw->GameShell->deadCheck = yw->timeStamp + 300000;
    yw->GameShell->sendScore = 3000;

    memset(yw->playerstatus, 0, sizeof(yw->playerstatus));

    return 1;
}
