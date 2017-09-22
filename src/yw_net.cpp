#include <stdlib.h>
#include <string.h>
#include "nlist.h"
#include "engine_miles.h"
#include "yw.h"
#include "yw_internal.h"
#include "yw_net.h"
#include "log.h"
#include "ypabact.h"
#include "yparobo.h"
#include "ypagun.h"
#include "ypamissile.h"
#include "font.h"

#define C_2PI_127       0.04947390005653217698   // (2PI / 127)
#define C_127_2PI       20.21267777267070764265  // (127 / 2PI)

extern uint32_t bact_id;
extern key_value_stru ypaworld_keys[4];
extern char **ypaworld__string_pointers;


bool SPEED_DOWN_NET = false;
static uamessage_vhclDataI vhcldata_interpolate;
static uamessage_vhclDataE vhcldata_exact;
uint32_t HNDL_MSG[108];

float maxSpeed = 0.0;

const int netDebug_NUM = 4000;
int netDebug_CREATE_CNT = 0;
uint32_t netDebug_CREATED[7][netDebug_NUM];
int netDebug_DESTROY_CNT = 0;
uint32_t netDebug_DESTROYED[7][netDebug_NUM];

void netDebug_AddCreated(uint8_t owner, uint32_t id)
{
    if ( netDebug_CREATE_CNT >= netDebug_NUM )
        netDebug_CREATE_CNT = 0;

    netDebug_CREATED[owner][ netDebug_CREATE_CNT ] = id;
    netDebug_CREATE_CNT++;
}

void netDebug_addDestroyed(uint8_t owner, uint32_t id)
{
    if ( netDebug_DESTROY_CNT >= netDebug_NUM )
        netDebug_DESTROY_CNT = 0;

    netDebug_DESTROYED[ owner ][ netDebug_DESTROY_CNT ] = id;
    netDebug_DESTROY_CNT++;
}

void yw_netReportError(__NC_STACK_ypabact *host, uint32_t id)
{
    NC_STACK_ypaworld *ywo = host->self->getBACT_pWorld();
    _NC_STACK_ypaworld *yw = &ywo->stack__ypaworld;

    if ( yw->field_1b78 != host->self )
    {
        bool created = false;
        for (int i = 0; i < netDebug_CREATE_CNT; i++)
        {
            if ( netDebug_CREATED[host->owner][i] == id )
            {
                created = true;
                break;
            }
        }

        if ( created )
            log_netlog("\n--- shadow vehicle %d was created\n", id);
        else
            log_netlog("\n--- shadow vehicle %d was NOT created\n", id);

        bool destroyed = false;
        for (int i = 0; i < netDebug_DESTROY_CNT; i++)
        {
            if ( netDebug_DESTROYED[host->owner][i] == id )
            {
                destroyed = true;
                break;
            }
        }

        if ( destroyed )
            log_netlog("\n--- shadow vehicle %d was just destroyed\n", id);
        else
            log_netlog("\n--- shadow vehicle %d was NOT destroyed\n", id);
    }
}


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

bact_node * yw_getHostByOwner(_NC_STACK_ypaworld *yw, uint8_t owner)
{
    bact_node *bct = (bact_node *)yw->bact_list.head;

    while(bct->next)
    {
        if ( bct->bact->owner == owner && bct->bact->bact_type == BACT_TYPES_ROBO )
            return bct;
        bct = (bact_node *)bct->next;
    }

    return NULL;
}

__NC_STACK_ypabact * yw_netGetBactByID(__NC_STACK_ypabact *host, uint32_t id)
{
    if ( id == host->gid )
        return host;

    bact_node *comm = (bact_node *)host->subjects_list.head;
    while ( comm->next )
    {
        if ( id == comm->bact->gid )
            return comm->bact;

        bact_node *unit = (bact_node *)comm->bact->subjects_list.head;
        while ( unit->next )
        {
            if ( unit->bact->gid == id)
                return unit->bact;

            unit = (bact_node *)unit->next;
        }

        comm = (bact_node *)comm->next;
    }

    yw_netReportError(host, id);

    return NULL;
}

void yw_netBakeVhcl(__NC_STACK_ypabact *bact, uamessage_vhclData *dat, int id, int interpolate)
{
    vhcldata *common = NULL;
    vhcldataE *extended = NULL;

    if ( id >= 1024 )
    {
        log_netlog("Vehicle Buffer overflow!!! more than 500 vehicles\n");
        return;
    }

    if ( interpolate )
    {
        uamessage_vhclDataI *datI = static_cast<uamessage_vhclDataI *>(dat);
        if (datI)
        {
            common = &datI->data[id];
        }
    }
    else
    {
        uamessage_vhclDataE *datE = static_cast<uamessage_vhclDataE *>(dat);
        if (datE)
        {
            common = &datE->data[id];
            extended = &datE->data[id];
        }
    }

    if (!common)
    {
        log_netlog("yw_netBakeVhcl ERROR #1\n");
        return;
    }

    common->pos_x = (int32_t)bact->position.x / 2;
    common->pos_y = (int32_t)bact->position.y / 2;
    common->pos_z = (int32_t)bact->position.z / 2;

    if ( !interpolate && extended)
    {
        extended->speed = bact->fly_dir * bact->fly_dir_length;
    }

    if ( bact->self->getBACT_viewer() && (bact->status_flg & BACT_STFLAG_LAND) )
    {
        float len = bact->viewer_overeof - bact->overeof;
        common->pos_y += (int32_t)len / 2;
    }

    if ( bact->bact_type == BACT_TYPES_GUN )
    {
        common->pos_x = (int32_t)bact->old_pos.x / 2;
        common->pos_y = (int32_t)bact->old_pos.y / 2;
        common->pos_z = (int32_t)bact->old_pos.z / 2;

        common->specialinfo |= vhcldata::SI_YPAGUN;
    }
    else
    {
        common->specialinfo &= ~vhcldata::SI_YPAGUN;
    }

    vec3d out = bact->rotation.GetEuler();

    common->roll = out.x * C_127_2PI;
    common->pitch = out.y * C_127_2PI;
    common->yaw = out.z * C_127_2PI;

    common->specialinfo &= ~vhcldata::SI_UNK;
    common->ident = bact->gid;

    if ( bact->status_flg & BACT_STFLAG_NORENDER )
        common->specialinfo |= vhcldata::SI_NORENDER;
    else
        common->specialinfo &= ~vhcldata::SI_NORENDER;

    if ( bact->status_flg & BACT_STFLAG_LAND )
        common->specialinfo |= vhcldata::SI_LAND;
    else
        common->specialinfo &= ~vhcldata::SI_LAND;

    if ( bact->bact_type == BACT_TYPES_ROBO )
        common->specialinfo |= vhcldata::SI_ROBO;
    else
        common->specialinfo &= ~vhcldata::SI_ROBO;

    if ( bact->status_flg & BACT_STFLAG_DSETTED )
        common->specialinfo |= vhcldata::SI_DSETTED;
    else
        common->specialinfo &= ~vhcldata::SI_DSETTED;

    bact->status_flg &= BACT_STFLAG_DSETTED;

    common->energy = bact->energy;
    common->vp = 0;
}

bool yw_prepareVHCLData(_NC_STACK_ypaworld *yw, uamessage_vhclData *dat)
{
    bact_node *hosts = (bact_node *)yw->bact_list.head;
    bool found = false;

    while(hosts)
    {
        if ( yw->field_1b78 == hosts->bacto )
        {
            found = true;
            break;
        }

        hosts = (bact_node *)hosts->next;
    }

    if ( !found )
    {
        log_netlog("CollectVehicleData: My Robo doesn't exist!\n");
        return false;
    }

    yw_netBakeVhcl(hosts->bact, dat, 0, yw->netInterpolate);
    dat->owner = hosts->bact->owner;

    int num = 1;

    bact_node *comms = (bact_node *)hosts->bact->subjects_list.head;
    while(comms->next)
    {
        if (num >= 1023)
            break;

        yw_netBakeVhcl(comms->bact, dat, num, yw->netInterpolate);
        num++;

        bact_node *unit = (bact_node *)comms->bact->subjects_list.head;
        while(unit->next)
        {
            if ( num >= 1023 )
                break;
            yw_netBakeVhcl(unit->bact, dat, num, yw->netInterpolate);

            num++;

            unit = (bact_node *)unit->next;
        }

        comms = (bact_node *)comms->next;
    }

    dat->hdr.number = num;

    if (yw->netInterpolate )
    {
        uamessage_vhclDataI *datI = static_cast<uamessage_vhclDataI *>(dat);
        for(int i = 0; i < datI->hdr.number; i++)
        {
            if ( datI->data[i].ident < 0xFFFF )
                log_netlog("\n+++ CVD: i send nonsens! (ident %d)\n", datI->data[i].ident);
        }
    }
    else
        log_netlog("\n+++ CVD: Extrapolating ????\n");

    return true;
}

void yw_cleanPlayer(_NC_STACK_ypaworld *yw, const char *name, uint8_t owner, uint8_t mode)
{
    uint8_t own;
    if ( mode )
    {
        own = owner;
    }
    else
    {
        own = 0;

        while (true)
        {
            if (own >= 8)
            {
                log_netlog("destroy player: robo of player %s not found!\n", name);
                log_netlog("In cases of trouble with other players this is no problem\n");
                return;
            }

            if ( !strcasecmp(yw->GameShell->players[own].name, name) && yw->GameShell->players[own].isKilled == 0 )
                break;

            own++;
        }
    }

    bact_node *bhost = yw_getHostByOwner(yw, own);
    if ( bhost )
    {
        sub_4C8EB4(yw, bhost);
        yw->GameShell->players[own].isKilled = 3;
    }
}

void yw_netApplyVhclDataI(__NC_STACK_ypabact *bact, _NC_STACK_ypaworld *yw, uamessage_vhclDataI *dat, int id, uint32_t timestamp)
{
    if ( id < dat->hdr.number )
    {
        vec3d v49;
        v49.x = 2 * dat->data[id].pos_x;
        v49.y = 2 * dat->data[id].pos_y;
        v49.z = 2 * dat->data[id].pos_z;

        if ( v49.x < 0.0 || v49.x > bact->wrldX || v49.z > 0.0 || v49.z < bact->wrldY )
            log_netlog(
                "\n+++ EVD: impossible position x %7.2f(%d) z %7.2f(%d) of object %d\n",
                v49.x,
                dat->data[id].pos_x,
                v49.z,
                dat->data[id].pos_z,
                bact->gid);

        float dtime = dat->hdr.diffTime * 0.001;

        vec3d tmp = (v49 - bact->old_pos) / (dtime * 6.0);

        bact->fly_dir_length = tmp.normalise();

        if ( bact->fly_dir_length > 0.0001 )
            bact->fly_dir = tmp;

        bact->position = bact->old_pos;
        bact->old_pos = v49;

        if ( dat->data[id].specialinfo & vhcldata::SI_DSETTED )
        {
            bact->position = bact->old_pos;
            bact->fly_dir_length = 0;
        }

        vec3d v47;
        v47.x = dat->data[id].roll * C_2PI_127;
        v47.y = dat->data[id].pitch * C_2PI_127;
        v47.z = dat->data[id].yaw * C_2PI_127;

        mat3x3 out = mat3x3::Euler(v47);

        bact->netDRot.m00 = (out.m00 - bact->netRotation.m00) / dtime;
        bact->netDRot.m01 = (out.m01 - bact->netRotation.m01) / dtime;
        bact->netDRot.m02 = (out.m02 - bact->netRotation.m02) / dtime;
        bact->netDRot.m10 = (out.m10 - bact->netRotation.m10) / dtime;
        bact->netDRot.m11 = (out.m11 - bact->netRotation.m11) / dtime;
        bact->netDRot.m12 = (out.m12 - bact->netRotation.m12) / dtime;
        bact->netDRot.m20 = (out.m20 - bact->netRotation.m20) / dtime;
        bact->netDRot.m21 = (out.m21 - bact->netRotation.m21) / dtime;
        bact->netDRot.m22 = (out.m22 - bact->netRotation.m22) / dtime;

        bact->rotation = bact->netRotation;
        bact->netRotation = out;

        if ( dat->data[id].specialinfo & vhcldata::SI_NORENDER )
            bact->status_flg |= BACT_STFLAG_NORENDER;

        if ( dat->data[id].specialinfo & vhcldata::SI_LAND )
            bact->status_flg |= BACT_STFLAG_LAND;
        else
            bact->status_flg &= ~BACT_STFLAG_LAND;

        bact->lastFrmStamp = timestamp;
        bact->energy = dat->data[id].energy;
    }
}

void yw_netApplyVhclDataE(__NC_STACK_ypabact *bact, _NC_STACK_ypaworld *yw, uamessage_vhclDataE *dat, int id, uint32_t timestamp)
{
    if ( dat->hdr.number > id )
    {
        int32_t tmstmp = timestamp - bact->lastFrmStamp;
        if ( tmstmp > 2 )
        {
            bact->old_pos = bact->position;

            bact->position.x = 2 * dat->data[id].pos_x;
            bact->position.y = 2 * dat->data[id].pos_y;
            bact->position.z = 2 * dat->data[id].pos_z;

            vec3d v73 = dat->data[id].speed;

            float dtime = tmstmp * 0.001;

            bact->netDSpeed.x = (v73.x - bact->fly_dir.x * bact->fly_dir_length) / dtime;
            bact->netDSpeed.y = (v73.y - bact->fly_dir.y * bact->fly_dir_length) / dtime;
            bact->netDSpeed.z = (v73.z - bact->fly_dir.z * bact->fly_dir_length) / dtime;

            float spd = bact->netDSpeed.length();

            if ( spd > maxSpeed )
            {
                if ( bact->bact_type != BACT_TYPES_MISSLE )
                {
                    log_netlog("d_speed max: %10.5f / class %d dof.v old %10.5f with net_time %4.3f\n", spd, bact->bact_type, bact->fly_dir_length, dtime);
                    maxSpeed = spd;
                }
            }

            if ( spd > 400.0 )
            {
                bact->netDSpeed *= 4.0 / spd;
            }

            if ( bact->bact_type != BACT_TYPES_GUN && bact->bact_type != BACT_TYPES_ROBO )
            {
                bact->fly_dir_length = v73.length();

                if ( bact->fly_dir_length > 0.001 )
                {
                    bact->fly_dir = v73 / bact->fly_dir_length;
                }
            }
            else
            {
                bact->fly_dir.x = (bact->position.x - bact->old_pos.x) / (dtime * 6.0);
                bact->fly_dir.y = (bact->position.y - bact->old_pos.y) / (dtime * 6.0);
                bact->fly_dir.z = (bact->position.z - bact->old_pos.z) / (dtime * 6.0);

                bact->fly_dir_length = bact->fly_dir.length();
                if ( bact->fly_dir_length > 0.001 )
                {
                    bact->fly_dir = bact->fly_dir / bact->fly_dir_length;
                }

                if ( bact->bact_type == BACT_TYPES_ROBO )
                {
                    bact->fly_dir.z = 0;
                    bact->fly_dir.x = 0;
                }

                bact->netDSpeed.x = 0;
                bact->netDSpeed.y = 0;
                bact->netDSpeed.z = 0;
            }

            if ( dat->data[id].specialinfo & vhcldata::SI_DSETTED )
            {
                bact->old_pos = bact->position;
                bact->fly_dir_length = 0;
            }

            vec3d rot;
            rot.x = dat->data[id].roll * C_2PI_127;
            rot.y = dat->data[id].pitch * C_2PI_127;
            rot.z = dat->data[id].yaw * C_2PI_127;

            mat3x3 out = mat3x3::Euler(rot);

            bact->netDRot.m00 = (out.m00 - bact->netRotation.m00) / dtime;
            bact->netDRot.m01 = (out.m01 - bact->netRotation.m01) / dtime;
            bact->netDRot.m02 = (out.m02 - bact->netRotation.m02) / dtime;
            bact->netDRot.m10 = (out.m10 - bact->netRotation.m10) / dtime;
            bact->netDRot.m11 = (out.m11 - bact->netRotation.m11) / dtime;
            bact->netDRot.m12 = (out.m12 - bact->netRotation.m12) / dtime;
            bact->netDRot.m20 = (out.m20 - bact->netRotation.m20) / dtime;
            bact->netDRot.m21 = (out.m21 - bact->netRotation.m21) / dtime;
            bact->netDRot.m22 = (out.m22 - bact->netRotation.m22) / dtime;

            bact->rotation = out;
            bact->netRotation = out;

            if ( dat->data[id].specialinfo & vhcldata::SI_NORENDER )
                bact->status_flg |= BACT_STFLAG_NORENDER;

            if ( dat->data[id].specialinfo & vhcldata::SI_LAND )
                bact->status_flg |= BACT_STFLAG_LAND;
            else
                bact->status_flg &= ~BACT_STFLAG_LAND;

            bact->lastFrmStamp = timestamp;

            if ( bact->status_flg & BACT_STFLAG_LAND )
            {
                ypaworld_arg136 v69;
                v69.pos_x = bact->position.x;
                v69.pos_y = bact->position.y;
                v69.pos_z = bact->position.z;
                v69.field_14 = bact->rotation.m10 * 200.0;
                v69.field_18 = bact->rotation.m11 * 200.0;
                v69.field_1C = bact->rotation.m12 * 200.0;
                v69.field_40 = 0;

                yw->self_full->ypaworld_func136(&v69);

                if ( v69.field_20 )
                {
                    bact->position.x = v69.field_2C - bact->rotation.m10 * bact->overeof;
                    bact->position.y = v69.field_30 - bact->rotation.m11 * bact->overeof;
                    bact->position.z = v69.field_34 - bact->rotation.m12 * bact->overeof;
                }
            }

            bact->energy = dat->data[id].energy;
        }
    }
}

void yw_processVhclDataMsgs(_NC_STACK_ypaworld *yw, uamessage_vhclData *msg, bact_node *host_node)
{
    uamessage_vhclDataI *datI = NULL;
    uamessage_vhclDataE *datE = NULL;

    if (yw->netInterpolate)
        datI = static_cast<uamessage_vhclDataI *>(msg);
    else
        datE = static_cast<uamessage_vhclDataE *>(msg);

    if (!datI && !datE)
    {
        log_netlog("yw_processVhclDataMsgs ERROR #1\n");
        return;
    }


    for(int i = msg->hdr.number - 1; i >= 0; i--)
    {
        __NC_STACK_ypabact *bact = NULL;

        uint32_t ident = 0;

        if (datI)
            ident = datI->data[i].ident;
        else
            ident = datE->data[i].ident;

        bact = yw_netGetBactByID(host_node->bact, ident);

        if ( !bact )
            log_netlog("+++ EVD: Haven't found vehicle ident %d  from owner %d (%dsec)\n", ident, host_node->bact->owner, yw->timeStamp / 1000);
        else
        {
            if ( yw->netInterpolate )
                yw_netApplyVhclDataI(bact, yw, datI, i, yw->timeStamp);
            else
                yw_netApplyVhclDataE(bact, yw, datE, i, yw->timeStamp);
        }
    }
}

int yw_netGetUnitsCount(__NC_STACK_ypabact *host)
{
    int count = 1;
    bact_node *comm = (bact_node *)host->subjects_list.head;
    while (comm->next)
    {
        count++;

        bact_node *unt = (bact_node *)comm->bact->subjects_list.head;

        while (unt->next)
        {
            count++;
            unt = (bact_node *)unt->next;
        }

        comm = (bact_node *)comm->next;
    }
    return count;
}



__NC_STACK_ypabact * yw_netGetMissileByID(__NC_STACK_ypabact *host, uint32_t id)
{
    for (bact_node *msl = (bact_node *)host->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
    {
        if ( id == msl->bact->gid )
            return msl->bact;
    }

    bact_node *comm = (bact_node *)host->subjects_list.head;
    while ( comm->next )
    {
        for (bact_node *msl = (bact_node *)comm->bact->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
        {
            if ( id == msl->bact->gid )
                return msl->bact;
        }

        bact_node *unit = (bact_node *)comm->bact->subjects_list.head;
        while ( unit->next )
        {
            for (bact_node *msl = (bact_node *)unit->bact->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
            {
                if ( id == msl->bact->gid )
                    return msl->bact;
            }

            unit = (bact_node *)unit->next;
        }

        comm = (bact_node *)comm->next;
    }

    yw_netReportError(host, id);

    return NULL;
}

__NC_STACK_ypabact * yw_netGetMissileOfBact(__NC_STACK_ypabact *host, uint32_t mslId, uint32_t bactId)
{
    if ( bactId == host->gid )
    {
        for (bact_node *msl = (bact_node *)host->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
        {
            if ( mslId == msl->bact->gid )
                return msl->bact;
        }
    }
    else
    {
        bact_node *comm = (bact_node *)host->subjects_list.head;
        while ( comm->next )
        {
            if ( bactId == comm->bact->gid )
            {
                for (bact_node *msl = (bact_node *)comm->bact->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
                {
                    if ( mslId == msl->bact->gid )
                        return msl->bact;
                }

                goto yw_netGetMissileOfBact_NOTFOUND;
            }


            bact_node *unit = (bact_node *)comm->bact->subjects_list.head;
            while ( unit->next )
            {
                if ( unit->bact->gid == bactId)
                {
                    for (bact_node *msl = (bact_node *)unit->bact->missiles_list.head; msl->next; msl = (bact_node *)msl->next)
                    {
                        if ( mslId == msl->bact->gid )
                            return msl->bact;
                    }

                    goto yw_netGetMissileOfBact_NOTFOUND;
                }

                unit = (bact_node *)unit->next;
            }

            comm = (bact_node *)comm->next;
        }
    }

yw_netGetMissileOfBact_NOTFOUND:

    yw_netReportError(host, mslId);

    return NULL;
}

void yw_netAddVhclUpdData(vhclUpdData *dat, uint8_t type, __NC_STACK_ypabact *bact)
{
    dat->type = type;
    dat->pos = bact->position;
    dat->status = bact->status;
    dat->status_flg = bact->status_flg;
    dat->gid = bact->gid;
    dat->vhclID = bact->vehicleID;
    dat->engy = bact->energy;

    vec3d out = bact->rotation.GetEuler();

    dat->rot_x = out.x * C_127_2PI;
    dat->rot_y = out.y * C_127_2PI;
    dat->rot_z = out.z * C_127_2PI;
}

void yw_netUpdDataVhcl(vhclUpdData *dat, __NC_STACK_ypabact *bact, char owner, NC_STACK_yparobo *host_station)
{
    bact->owner = owner;

    bact->rotation = mat3x3::Euler( vec3d(dat->rot_x, dat->rot_y, dat->rot_z) * C_2PI_127 );

    bact->host_station = host_station;
    bact->gid = dat->gid;
    bact->energy = dat->engy;
    bact->status_flg = dat->status_flg;
    bact->status = dat->status;

    setState_msg sstt;

    if ( bact->status_flg & BACT_STFLAG_DEATH2 )
    {
        sstt.newStatus = 0;
        sstt.unsetFlags = 0;
        sstt.setFlags = BACT_STFLAG_DEATH2;
    }
    else if ( bact->status_flg & BACT_STFLAG_FIRE )
    {
        sstt.newStatus = 0;
        sstt.unsetFlags = 0;
        sstt.setFlags = BACT_STFLAG_FIRE;
    }
    else
    {
        sstt.unsetFlags = 0;
        sstt.setFlags = 0;
        sstt.newStatus = dat->status;
    }

    bact->self->SetState(&sstt);
}


void yw_netSendUpdate(_NC_STACK_ypaworld *yw, uint8_t owner, char *recvID)
{
    if ( !owner )
    {
        log_netlog("\n+++ UPD: Owner 0 requested update???\n");
        return;
    }

    if ( !recvID )
    {
        log_netlog("\n+++ UPD: No receiver given for Update\n");
        return;
    }

    static uamessage_update updinf;

    bact_node *bhost = yw_getHostByOwner(yw, owner);

    if ( !bhost )
    {
        log_netlog("\n+++ UPD:No Robo found for owner %d/receiver %s\n", owner, recvID);
        return;
    }

    int numb = 1;

    yw_netAddVhclUpdData(updinf.data, 1, bhost->bact);

    bact_node *msl = (bact_node *)bhost->bact->missiles_list.head;
    while (msl->next)
    {
        yw_netAddVhclUpdData(&updinf.data[numb], 4, msl->bact);

        numb++;

        if ( numb >= 1023 )
            break;

        msl = (bact_node *)msl->next;
    }

    bact_node *comnd = (bact_node *)bhost->bact->subjects_list.head;
    while (comnd->next)
    {
        bool isrobogun = false;

        if ( comnd->bact->bact_type == BACT_TYPES_GUN )
        {
            NC_STACK_ypagun *gn = dynamic_cast<NC_STACK_ypagun *>(comnd->bacto);
            if ( gn )
                isrobogun = gn->getGUN_roboGun();
        }

        if ( isrobogun )
            yw_netAddVhclUpdData(&updinf.data[numb], 5, comnd->bact);
        else
            yw_netAddVhclUpdData(&updinf.data[numb], 2, comnd->bact);

        numb++;

        if ( numb >= 1023 )
            break;

        msl = (bact_node *)comnd->bact->missiles_list.head;
        while ( msl->next )
        {
            yw_netAddVhclUpdData(&updinf.data[numb], 4, msl->bact);

            numb++;

            if ( numb >= 1023 )
                break;

            msl = (bact_node *)msl->next;
        }

        bact_node *slv = (bact_node *)comnd->bact->subjects_list.head;

        while (slv->next)
        {
            yw_netAddVhclUpdData(&updinf.data[numb], 3, slv->bact);

            numb++;

            if ( numb >= 1023 )
                break;

            msl = (bact_node *)slv->bact->missiles_list.head;
            while ( msl->next )
            {
                yw_netAddVhclUpdData(&updinf.data[numb], 4, msl->bact);

                numb++;

                if ( numb >= 1023 )
                    break;

                msl = (bact_node *)msl->next;
            }
            slv = (bact_node *)slv->next;
        }

        comnd = (bact_node *)comnd->next;
    }

    updinf.data[numb].type = 0;
    numb++;

    updinf.owner = owner;
    updinf.sz = ((char *)&updinf.data[numb] - (char *)&updinf);

    updinf.msgID = UAMSG_UPDATE;
    updinf.num = numb;

    yw_arg181 arg181;
    arg181.recvID = recvID;
    arg181.recvFlags = 1;
    arg181.dataSize = updinf.sz;
    arg181.data = &updinf;
    arg181.garant = 1;

    if ( yw->self_full->ypaworld_func181(&arg181) )
        log_netlog("\n+++ UPD: Ok, sent Update from owner %d to reciever %s\n", owner, recvID);
    else
        log_netlog("\n+++ UPD: Send-Error. Hmmmm...\n");
}

bool yw_netRecvUpdate(_NC_STACK_ypaworld *yw, uamessage_update *msg, int owner)
{
    bact_node *bhost = yw_getHostByOwner(yw, owner);

    if ( bhost )
    {
        sub_4C8EB4(yw, bhost);

        int id = 0;

        NC_STACK_yparobo *currHost = NULL;
        NC_STACK_ypabact *currComm = NULL;
        bact_node *bctnd = NULL;
        __NC_STACK_ypabact *lastBct = NULL;

        while ( msg->data[id].type != 0 )
        {
            ypaworld_arg146 newVhcl;
            ypaworld_arg146 newMiss;

            vhclUpdData *dat = &msg->data[id];

            switch ( dat->type )
            {
            case 1:
            {
                newVhcl.pos = dat->pos;
                newVhcl.vehicle_id = dat->vhclID;

                NC_STACK_ypabact *tmp = yw->self_full->ypaworld_func146(&newVhcl);

                currHost = dynamic_cast<NC_STACK_yparobo *>(tmp);

                if ( !currHost )
                {
                    log_netlog("RESTORE: Unable to create robo\n");
                    return false;
                }

                lastBct = currHost->getBACT_pBact();

                bctnd = (bact_node *)lastBct->subjects_list.head;

                if ( !bctnd->next )
                    bctnd = NULL;

                yw_netUpdDataVhcl(dat, lastBct, owner, NULL);

                yw->self_full->ypaworld_func134(currHost);
            }
            break;

            case 5:
            {
                if ( !bctnd )
                    break;

                yw_netUpdDataVhcl(dat, bctnd->bact, owner, currHost);

                //Find next host gun
                while (bctnd->next)
                {
                    bctnd = (bact_node *)bctnd->next;

                    if ( bctnd->bact->bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gn = dynamic_cast<NC_STACK_ypagun *>(bctnd->bacto);

                        if (gn && gn->getGUN_roboGun())
                            break;
                    }
                }

                if ( !bctnd->next )
                    bctnd = NULL;
            }
            break;

            case 2:
            {
                newVhcl.pos = dat->pos;
                newVhcl.vehicle_id = dat->vhclID;
                currComm = yw->self_full->ypaworld_func146(&newVhcl);

                if ( !currComm )
                {
                    log_netlog("RESTORE: Unable to create cmdr\n");
                    return false;
                }

                lastBct = currComm->getBACT_pBact();

                yw_netUpdDataVhcl(dat, lastBct, owner, currHost);

                currHost->AddSubject(currComm);
            }
            break;

            case 3:
            {
                newVhcl.pos = dat->pos;
                newVhcl.vehicle_id = dat->vhclID;

                NC_STACK_ypabact *tmp = yw->self_full->ypaworld_func146(&newVhcl);
                if ( !tmp )
                {
                    log_netlog("RESTORE: Unable to create slave\n");
                    return false;
                }

                lastBct = tmp->getBACT_pBact();

                yw_netUpdDataVhcl(dat, lastBct, owner, currHost);

                currComm->AddSubject(tmp);
            }
            break;

            case 4:
            {
                newMiss.pos = dat->pos;
                newMiss.vehicle_id = dat->vhclID;
                NC_STACK_ypamissile *tmp = yw->self_full->ypaworld_func147(&newMiss);

                if ( !tmp )
                {
                    log_netlog("RESTORE: Unable to create weapon\n");
                    return false;
                }

                __NC_STACK_ypabact *tbact = tmp->getBACT_pBact();

                yw_netUpdDataVhcl(dat, tbact, owner, currHost);

                if ( tbact->parent_bacto )
                {
                    Remove(&tbact->subject_node);
                    tbact->parent_bacto = NULL;
                }

                AddTail(&lastBct->missiles_list, tmp->getMISS_pNode());

                tmp->setMISS_launcher(lastBct);
            }
            break;

            default:
                break;
            }

            id++;
        }
    }
    return true;
}

__NC_STACK_ypabact * yw_netFindReorderUnit(__NC_STACK_ypabact *bact_host, uint32_t ID)
{
    if ( ID == bact_host->gid )
        return bact_host;

    bact_node *comms = (bact_node *)bact_host->subjects_list.head;
    while ( !comms->next )
    {
        if ( ID == comms->bact->gid )
            return comms->bact;

        bact_node *units = (bact_node *)comms->bact->subjects_list.head;

        while ( !units->next )
        {
            if ( ID == units->bact->gid )
                return units->bact;

            bact_node *uniunits = (bact_node *)comms->bact->subjects_list.head;

            while ( !uniunits->next )
            {
                if ( ID == uniunits->bact->gid )
                    return uniunits->bact;

                uniunits = (bact_node *)uniunits->next;
            }

            units = (bact_node *)units->next;
        }

        comms = (bact_node *)comms->next;
    }

    yw_netReportError(bact_host, ID);
    return NULL;
}

size_t yw_handleNormMsg(_NC_STACK_ypaworld *yw, windp_recvMsg *msg, char *err)
{
    uamessage_base *bMsg = (uamessage_base *)msg->data;
    uint32_t msgID = bMsg->msgID;
    uint8_t owner = bMsg->owner;
    uint32_t tstamp = bMsg->tstamp;
    uint32_t msgcnt = bMsg->msgCnt;

    int tv481 = 0;

    if (owner > 0 && owner < 8)
    {
        yw->GameShell->players[owner].lastMsgTime = yw->field_1b24.gTime;

        if ( msgID != UAMSG_VHCLENERGY && msgID != UAMSG_ENDPLASMA && msg->recvFlags == 2 )
        {
            if ( tstamp >= yw->GameShell->players[owner].tstamp )
            {
                if ( tstamp - yw->GameShell->players[owner].tstamp > 6000 )
                    log_netlog(" Message %d from owner %d at %d sec is too fast...\n", msgID, owner, tstamp / 1000);

                yw->GameShell->players[owner].tstamp = tstamp;
                tv481 = 0;
            }
            else
            {
                tv481 = 1;
                log_netlog("Warning, Msg %d from owner %d is too old!\n", msgID, owner);
                log_netlog("         old: at %d sec  new: at %d sec\n", yw->GameShell->players[owner].tstamp, tstamp);
            }

            if ( msgcnt == yw->GameShell->players[owner].msgCnt )
            {
                yw->GameShell->players[owner].msgCnt++;
            }
            else
            {
                if ( msgcnt <= yw->GameShell->players[owner].msgCnt )
                {
                    log_netlog("HMM !!!  Late message received with count %d\n", msgcnt);
                    log_netlog("         from owner %d, id %d at time %d\n", owner, msgID, tstamp / 1000);
                }
                else
                {
                    log_netlog("PANIC!!! Msg lost! Last msg has count %d\n", yw->GameShell->players[owner].msgCnt - 1);
                    log_netlog("         Msg now (%d) has count %d\n", msgID, msgcnt);
                    log_netlog("         From Owner %d at time %d\n\n", owner, tstamp / 1000);
                }
                yw->GameShell->players[owner].msgCnt = msgcnt + 1;
            }
        }
    }
    else
    {
        if ( msgID != UAMSG_FRACTION && msgID != UAMSG_MESSAGE && msgID != UAMSG_WELCOME && msgID != UAMSG_READY && msgID != UAMSG_LOAD )
            log_netlog("Warning, no or false owner (%d) specified for msg %d\n", owner, msgID);
        tv481 = 0;
    }

    netType1 *plr = NULL;

    for(int i = 0; i < 4; i++)
    {
        if (strcasecmp(yw->GameShell->players2[i].name, msg->senderID) == 0)
        {
            plr = &yw->GameShell->players2[i];

            if (plr->w84upd)
            {
                plr->w84upd -= yw->field_1b24.frameTime;

                if (plr->w84upd)
                    plr->w84upd = 0;
            }

            break;
        }
    }

    size_t szmsg = 0;

    switch (msgID)
    {
    case UAMSG_LOAD:
    {
        szmsg = sizeof(uamessage_load);

        if (yw->GameShell->players[owner].isKilled)
            break;

        uamessage_load *ldMsg = (uamessage_load *)msg->data;
        yw->GameShell->field_0x2fbc = 4;
        yw->GameShell->field_0x2fc0 = ldMsg->level;
        yw->GameShell->field_0x2fc4 = ldMsg->level;

        yw->isNetGame = 1;
        yw_NetPrintStartInfo(yw->GameShell);
    }
    break;

    case UAMSG_NEWVHCL:
    {
        szmsg = sizeof(uamessage_newVhcl);
        uamessage_newVhcl *nvMsg = (uamessage_newVhcl *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        if ( plr && plr->w84upd )
            break;

        ypaworld_arg146 newVhcl;
        newVhcl.pos = nvMsg->pos;
        newVhcl.vehicle_id = nvMsg->type;

        NC_STACK_ypabact *bacto = yw->self_full->ypaworld_func146(&newVhcl);

        if ( !bacto )
            break;

        __NC_STACK_ypabact *bact = bacto->getBACT_pBact();

        setState_msg sstate;
        sstate.newStatus = BACT_STATUS_CREATE;
        sstate.setFlags = 0;
        sstate.unsetFlags = 0;

        bacto->SetState(&sstate);

        bact_node *host_node = yw_getHostByOwner(yw, nvMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NV: Robo Owner %d not found (%ds)\n", nvMsg->owner, yw->timeStamp / 1000);
            break;
        }

        if ( nvMsg->kind == 0 )
        {
            yw->self_full->ypaworld_func134(bacto);
        }
        else if ( nvMsg->kind == 1 )
        {
            host_node->bacto->AddSubject(bacto);
        }
        else if ( nvMsg->kind == 2 )
        {
            bact_node *lstobj = (bact_node *)host_node->bact->subjects_list.head;

            bool found = false;

            while( lstobj->next )
            {
                if ( nvMsg->parent == lstobj->bact->gid )
                {
                    found = true;
                    break;
                }

                lstobj = (bact_node *)lstobj->next;
            }

            if ( !found )
            {
                log_netlog("NV: No master for created shadow object!\n");
                yw->self_full->ypaworld_func144(bacto);
                strcpy(err, msg->senderID);

                break;
            }

            lstobj->bacto->AddSubject(bacto);
        }

        bact->owner = nvMsg->owner;
        bact->gid = nvMsg->id;
        bact->commandID = nvMsg->commandID;
        bact->lastFrmStamp = yw->timeStamp;
        bact->host_station = dynamic_cast<NC_STACK_yparobo *>(host_node->bacto);

        yw_arg184 v459;
        v459.type = 4;
        v459.t34.field_1 = bact->owner;
        v459.t34.field_2 = bact->vehicleID;
        v459.t34.field_4 = bact->position.x * 256.0 / bact->wrldX;
        v459.t34.field_5 = bact->position.y * 256.0 / bact->wrldY;

        yw->self_full->ypaworld_func184(&v459);
        netDebug_AddCreated(host_node->bact->owner, nvMsg->id);
        break;
    }
    break;

    case UAMSG_DESTROYVHCL:
    {
        szmsg = sizeof(uamessage_destroyVhcl);
        uamessage_destroyVhcl *dvMsg = (uamessage_destroyVhcl *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, dvMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ DV:Havent found robo with owner %d (%ds)\n", dvMsg->owner, yw->timeStamp / 1000);
            break;
        }

        __NC_STACK_ypabact *bctt = yw_netGetBactByID(host_node->bact, dvMsg->id);

        if ( !bctt )
        {
            log_netlog("\n+++ DV: Havent found vehicle %d (%ds)\n", dvMsg->id, yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }

        if ( !(bctt->status_flg & BACT_STFLAG_DEATH1) )
        {
            log_netlog("+++ DV: Release a non-logic-dead vehicle %d! (%ds)\n", dvMsg->id, yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }

        int vnumb = yw_netGetUnitsCount(host_node->bact);

        bact_node *sunit = (bact_node *)bctt->subjects_list.head;

        while( bctt->subjects_list.head->next )
        {
            yw->self_full->ypaworld_func144(sunit->bacto);
            log_netlog("+++ DV: Released vehicle with slave! (%ds)\n", yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
        }

        while ( bctt->missiles_list.head->next )
        {
            bact_node *misl = (bact_node *)RemHead(&bctt->missiles_list);

            misl->bact->parent_bacto = NULL;

            sub_4F1B34(yw, bctt);

            log_netlog("+++ DV: Released vehicle with weapons! (%ds)\n", yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
        }

        yw->self_full->ypaworld_func144(bctt->self);

        netDebug_addDestroyed(host_node->bact->owner, dvMsg->id);

        if ( dvMsg->type != BACT_TYPES_MISSLE && yw_netGetUnitsCount(host_node->bact) != (vnumb - 1) )
        {
            log_netlog("\n+++ DV: Vehiclecount changed more than 1! (%ds)\n", yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }
    }
    break;

    case UAMSG_NEWLEADER:
    {
        szmsg = sizeof(uamessage_newLeader);
        uamessage_newLeader *nlMsg = (uamessage_newLeader *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, nlMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NC:Havent found robo with owner %d (%ds)\n", nlMsg->owner, yw->timeStamp / 1000);
            break;
        }

        int unitscnt = yw_netGetUnitsCount(host_node->bact);

        __NC_STACK_ypabact *bct = yw_netGetBactByID(host_node->bact, nlMsg->id);

        if ( !bct )
        {
            log_netlog("\n+++ NC: Havent found vehicle %d (%ds)\n", nlMsg->id, yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }

        if ( nlMsg->newLeader )
        {
            __NC_STACK_ypabact *newleader = yw_netGetBactByID(host_node->bact, nlMsg->newLeader);

            if ( newleader )
            {
                host_node->bacto->AddSubject(newleader->self);

                while (bct->subjects_list.head->next)
                {
                    newleader->self->AddSubject( ((bact_node *)bct->subjects_list.head)->bacto );
                }

                newleader->self->AddSubject( bct->self );
            }
        }

        if ( yw_netGetUnitsCount(host_node->bact) != unitscnt )
        {
            log_netlog("\n+++ NC: Vehiclecount changed! (%ds)\n", yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }
    }
    break;

    case UAMSG_NEWWEAPON:
    {
        szmsg = sizeof(uamessage_newWeapon);
        uamessage_newWeapon *nwMsg = (uamessage_newWeapon *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, nwMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NW: Havent found robo with owner %d (%ds)\n", nwMsg->owner, yw->timeStamp / 1000);
            break;
        }

        ypaworld_arg146 newWpn;
        newWpn.pos = nwMsg->pos;
        newWpn.vehicle_id = nwMsg->type;

        NC_STACK_ypamissile *weapo = yw->self_full->ypaworld_func147(&newWpn);

        if ( !weapo )
        {
            log_netlog("\n+++ NW: Was not able to create weapon %d for owner %d\n", nwMsg->type, nwMsg->owner);
            break;
        }

        __NC_STACK_ypabact *weapbact = weapo->getBACT_pBact();

        __NC_STACK_ypabact *weapLauncher = yw_netGetBactByID(host_node->bact, nwMsg->launcher);
        if ( !weapLauncher )
        {
            log_netlog("\n+++ NW: Havent found vehicle %d (%ds)\n", nwMsg->id, yw->timeStamp / 1000);
            yw->self_full->ypaworld_func144(weapo);
            strcpy(err, msg->senderID);
            break;
        }

        if ( weapbact->parent_bacto )
        {
            Remove(&weapbact->subject_node);
            weapbact->parent_bacto = NULL;
        }

        bact_node *weapnode = weapo->getMISS_pNode();
        AddTail(&weapLauncher->missiles_list, weapnode);

        weapo->setMISS_launcher(weapLauncher);

        weapbact->owner = nwMsg->owner;
        weapbact->gid = nwMsg->id;
        weapbact->lastFrmStamp = yw->timeStamp;

        netDebug_AddCreated(host_node->bact->owner, nwMsg->id);

        weapbact->fly_dir_length = nwMsg->dir.length();
        weapbact->fly_dir = nwMsg->dir;

        if ( weapbact->fly_dir_length > 0.001 )
        {
            weapbact->fly_dir /= weapbact->fly_dir_length;
        }

        weapo->ypamissile_func130(NULL);

        int wpnType = weapo->getMISS_type();

        if ( wpnType == 3 )
        {
            setTarget_msg stargt;
            stargt.tgt_pos = nwMsg->targetPos;
            stargt.priority = 0;
            stargt.tgt_type = nwMsg->targetType;

            if ( stargt.tgt_type == BACT_TGT_TYPE_UNIT )
            {
                bact_node *ownerhost = yw_getHostByOwner(yw, nwMsg->targetOwner);

                if ( !ownerhost )
                {
                    log_netlog("\n+++ NW: false targetowner %d for weapon\n", nwMsg->targetOwner);
                    strcpy(err, msg->senderID);
                    break;
                }

                stargt.tgt.pbact = yw_netGetBactByID(ownerhost->bact, nwMsg->target);

                if ( !stargt.tgt.pbact )
                    stargt.tgt_type = 0;
            }

            weapo->SetTarget(&stargt);

            if ( nwMsg->targetType == BACT_TGT_TYPE_CELL )
                weapbact->primTpos.y = nwMsg->targetPos.y;
        }

        if ( wpnType == 2 )
        {
            weapbact->primTtype = BACT_TGT_TYPE_DRCT;
            weapbact->target_dir = weapbact->fly_dir;
        }

        weapbact->host_station = dynamic_cast<NC_STACK_yparobo*>(host_node->bacto);

        if ( nwMsg->targetType == BACT_TGT_TYPE_UNIT )
            break;

        WeapProto *protos = yw->self_full->getYW_weaponProtos();
        int weapLifeTime = protos[nwMsg->type].life_time_nt;

        if ( weapLifeTime )
            weapo->setMISS_lifeTime(weapLifeTime);

    }
    break;

    case UAMSG_SETSTATE:
    {
        szmsg = sizeof(uamessage_setState);
        uamessage_setState *ssMsg = (uamessage_setState *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, ssMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ SS: Havent found robo with owner %d (%ds)\n", ssMsg->owner, yw->timeStamp / 1000);
            break;
        }

        __NC_STACK_ypabact *bct = yw_netGetBactByID(host_node->bact, ssMsg->id);
        if ( !bct )
        {
            log_netlog("\n+++ SS: Havent found vehicle %d (%ds)\n", ssMsg->id, yw->timeStamp / 1000);
            strcpy(err, msg->senderID);
            break;
        }

        setState_msg ststate;
        ststate.newStatus = ssMsg->newStatus;
        ststate.setFlags = ssMsg->setFlags;
        ststate.unsetFlags = ssMsg->unsetFlags;
        bct->self->SetState(&ststate);
    }
    break;

    case UAMSG_VHCLDATA_I:
    {
        uamessage_vhclData *vdMsg = (uamessage_vhclData *)msg->data;
        szmsg = msg->size;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) || tv481 )
            break;

        if ( yw->netInterpolate )
        {
            bact_node *host_node = yw_getHostByOwner(yw, vdMsg->owner);

            if ( host_node )
                yw_processVhclDataMsgs(yw, vdMsg, host_node);
            else
                log_netlog("\n+++ VD: Havent found robo with owner %d (%ds)\n", vdMsg->owner, yw->timeStamp / 1000);
        }
        else
        {
            log_netlog("\n+++ VD: An extrapolate-Program gets interpolate data!!!\n");
        }
    }
    break;

    case UAMSG_VHCLDATA_E:
    {
        uamessage_vhclData *vdMsg = (uamessage_vhclData *)msg->data;
        szmsg = msg->size;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) || tv481 )
            break;

        if ( yw->netInterpolate )
        {
            log_netlog("\n+++ VD: An interpolate-Program gets extrapolate data!!!\n");
        }
        else
        {
            bact_node *host_node = yw_getHostByOwner(yw, vdMsg->owner);

            if ( host_node )
                yw_processVhclDataMsgs(yw, vdMsg, host_node);
            else
                log_netlog("\n+++ VD: Havent found robo with owner %d (%ds)\n", vdMsg->owner, yw->timeStamp / 1000);
        }
    }
    break;

    case UAMSG_DEAD:
    {
        szmsg = sizeof(uamessage_dead);
        uamessage_dead *ddMsg = (uamessage_dead *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, ddMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ D: Havent found robo with owner %d (%ds)\n", ddMsg->owner, yw->timeStamp / 1000);
            break;
        }

        __NC_STACK_ypabact *fndBact = NULL;

        if ( ddMsg->classID == BACT_TYPES_MISSLE )
        {
            fndBact = yw_netGetMissileByID(host_node->bact, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found weapon %d (%ds)\n", ddMsg->id, yw->timeStamp / 1000);
                strcpy(err, msg->senderID);
                break;
            }

        }
        else
        {
            fndBact = yw_netGetBactByID(host_node->bact, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found vehicle %d (%ds)\n", ddMsg->id, yw->timeStamp / 1000);
                strcpy(err, msg->senderID);
                break;
            }
        }

        bact_node *nd = (bact_node *)fndBact->subjects_list.head;
        while ( nd->next )
        {
            if ( nd->bact->status == BACT_STATUS_DEAD )
            {
                if ( (size_t)fndBact->parent_bacto <= 2 )
                    yw->self_full->ypaworld_func134(nd->bacto);
                else
                    fndBact->parent_bacto->AddSubject(nd->bacto);

                nd->bact->status_flg |= BACT_STFLAG_NOMSG;
            }
            nd = (bact_node *)nd->next;
        }

        if ( ddMsg->newParent == 0 && !fndBact->subjects_list.head->next)
        {
            NC_STACK_ypabact *hst = yw->self_full->getYW_userHostStation();
            __NC_STACK_ypabact *hstbct = &hst->ypabact;

            int sft = 0;
            if ( fndBact->bact_type == BACT_TYPES_GUN )
                sft = fndBact->host_station->getROBO_epSafety();

            if ( hstbct->owner == ddMsg->killerOwner && fndBact->host_station == fndBact->parent_bacto && !sft && !(fndBact->status_flg & BACT_STFLAG_NOMSG) )
            {
                robo_arg134 rarg134;
                rarg134.unit = fndBact->killer;
                rarg134.field_4 = 5;
                rarg134.field_8 = sft;
                rarg134.field_C = sft;
                rarg134.field_10 = sft;
                rarg134.field_14 = 36;

                NC_STACK_yparobo *hstt = dynamic_cast<NC_STACK_yparobo *>(host_node->bacto);
                if (hstt)
                    hstt->placeMessage(&rarg134);
            }
        }
        else
        {
            __NC_STACK_ypabact *np = NULL;

            if ( ddMsg->newParent )
                np = yw_netGetBactByID(host_node->bact, ddMsg->newParent);

            if ( np != NULL )
            {
                host_node->bacto->AddSubject(np->self);

                while (fndBact->subjects_list.head->next)
                {
                    bact_node *kd = (bact_node *)fndBact->subjects_list.head;

                    np->self->AddSubject(kd->bacto);
                }
            }
            else
            {
                if ( ddMsg->newParent )
                    log_netlog("\n+++ D: No master (%d) found for my slaves (%ds)\n", ddMsg->newParent, yw->timeStamp / 1000);
                else
                    log_netlog("\n+++ D: No master given for my slaves (%ds)\n", yw->timeStamp / 1000);

                strcpy(err, msg->senderID);
            }
        }

        if ( (size_t)fndBact->parent_bacto <= 2 )
        {
            sub_4F1B34(yw, fndBact);
        }
        else
        {
            while ( true )
            {
                bact_node *rmvd = (bact_node *)RemHead(&fndBact->missiles_list);

                if ( !rmvd )
                    break;

                AddTail(&fndBact->parent_bact->missiles_list, rmvd);

                NC_STACK_ypamissile *miss = dynamic_cast<NC_STACK_ypamissile *>(rmvd->bacto);

                if (miss)
                    miss->setMISS_launcher(fndBact->parent_bact);
            }
        }

        fndBact->status = BACT_STATUS_DEAD;

        if ( ddMsg->landed )
        {
            setState_msg sst;
            sst.unsetFlags = 0;
            sst.newStatus = 0;
            sst.setFlags = BACT_STFLAG_DEATH2;
            fndBact->self->SetStateInternal(&sst);
        }

        sub_4F1A60(fndBact);

        for ( bact_node *k = (bact_node *)fndBact->subjects_list.head; k->next; k = (bact_node *)k->next )
            log_netlog("+++ D: I am dead, but I have slave ident %d class %d with state %d (%ds)\n", k->bact->gid, k->bact->bact_type, k->bact->status, yw->timeStamp / 1000);

        fndBact->status_flg |= BACT_STFLAG_DEATH1;

        if ( ddMsg->killerOwner )
        {
            bact_node *v134 = yw_getHostByOwner(yw, ddMsg->killerOwner);

            if ( v134 )
                fndBact->killer = yw_netGetBactByID(v134->bact, ddMsg->killer);
            else
                fndBact->killer = NULL;
        }

        if ( fndBact->killer )
        {
            yw_arg184 v427;
            v427.type = 3;
            v427.t34.field_1 = 8 * fndBact->killer->owner | fndBact->owner;

            if ( fndBact->killer->self->getBACT_viewer() || (fndBact->killer->status_flg & BACT_STFLAG_ISVIEW) )
                v427.t34.field_1 |= 0x80;

            if ( fndBact->self->getBACT_viewer() || (fndBact->status_flg & BACT_STFLAG_ISVIEW) )
                v427.t34.field_1 |= 0x40;

            v427.t34.field_2 = fndBact->vehicleID;

            if ( fndBact->bact_type == 3 )
                v427.t34.field_2 |= 0x8000;

            v427.t34.field_4 = fndBact->position.x * 256.0 / fndBact->wrldX;
            v427.t34.field_5 = fndBact->position.z * 256.0 / fndBact->wrldY;

            yw->self_full->ypaworld_func184(&v427);
        }

        fndBact->status_flg |= BACT_STFLAG_DEATH1;
    }
    break;

    case UAMSG_VHCLENERGY:
    {
        szmsg = sizeof(uamessage_vhclEnergy);
        uamessage_vhclEnergy *veMsg = (uamessage_vhclEnergy *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, veMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", veMsg->owner, yw->timeStamp / 1000);
            break;
        }

        if ( host_node->bacto != yw->field_1b78 )
            break;

        __NC_STACK_ypabact *fbact = yw_netGetBactByID(host_node->bact, veMsg->id);

        if ( !fbact )
            break;

        fbact->energy += veMsg->energy;
        fbact->killer_owner = veMsg->killerOwner;

        if ( fbact->energy >= 0 )
            break;

        if ( veMsg->killer )
        {
            bact_node *khost = yw_getHostByOwner(yw, veMsg->killerOwner);

            if ( !khost )
            {
                log_netlog("\n+++ VE: No robo with owner %d found\n", veMsg->killerOwner);
                break;
            }

            __NC_STACK_ypabact *kbact = yw_netGetBactByID(khost->bact, veMsg->killer);

            fbact->killer = kbact;

            if ( !kbact )
                log_netlog("\n+++ VE: False killer %d (owner %d) given\n", veMsg->killer, veMsg->killerOwner);
        }
        else
        {
            fbact->killer = 0;
        }

        setState_msg v410;
        v410.newStatus = BACT_STATUS_DEAD;
        v410.setFlags = 0;
        v410.unsetFlags = 0;

        fbact->self->SetState(&v410);
    }
    break;

    case UAMSG_SECTORENERGY:
    {
        szmsg = sizeof(uamessage_sectorEnergy);
        uamessage_sectorEnergy *seMsg = (uamessage_sectorEnergy *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        bact_node *host_node = yw_getHostByOwner(yw, seMsg->owner);

        yw_arg129 arg129;
        if ( host_node )
        {
            arg129.unit = yw_netGetBactByID(host_node->bact, seMsg->whoHit);
        }
        else
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", seMsg->owner, yw->timeStamp / 1000);
            arg129.unit = 0;
        }

        arg129.pos.x = seMsg->pos.x;
        arg129.pos.y = seMsg->pos.y;
        arg129.pos.z = seMsg->pos.z;
        arg129.field_10 = seMsg->energy;
        arg129.field_14 = seMsg->sectOwner;

        yw->self_full->ypaworld_func129(&arg129);
    }
    break;

    case UAMSG_STARTBUILD:
    {
        szmsg = sizeof(uamessage_startBuild);
        uamessage_startBuild *sbMsg = (uamessage_startBuild *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        if ( yw_getHostByOwner(yw, sbMsg->owner) )
        {
            ypaworld_arg148 arg148;
            arg148.ownerID = sbMsg->owner;
            arg148.ownerID2 = sbMsg->owner;
            arg148.blg_ID = sbMsg->bproto;
            arg148.field_C = 0;
            arg148.x = sbMsg->sec_x;
            arg148.y = sbMsg->sec_y;
            arg148.field_18 = 0;

            yw->self_full->ypaworld_func148(&arg148);
        }
        else
        {
            log_netlog("\n+++ SB: Havent found robo with owner %d (%ds)\n", sbMsg->owner, yw->timeStamp / 1000);
        }
    }
    break;

    case UAMSG_BUILDINGVHCL:
    {
        uamessage_bldVhcl *bvMsg = (uamessage_bldVhcl *)msg->data;
        szmsg = sizeof(uamessage_bldVhcl);

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) || !bvMsg->vhcl[0].protoID )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, bvMsg->owner);

        if ( bhost )
        {
            ypaworld_arg146 newVhcl;
            newVhcl.vehicle_id = bvMsg->vhcl[0].protoID;
            newVhcl.pos = bvMsg->vhcl[0].pos;

            NC_STACK_ypabact *kmnd = yw->self_full->ypaworld_func146(&newVhcl);
            if ( kmnd )
            {
                NC_STACK_ypagun *kmnd_gn = dynamic_cast<NC_STACK_ypagun *>(kmnd);

                if (kmnd_gn)
                    kmnd_gn->ypagun_func128(bvMsg->vhcl[0].base, false);

                setState_msg ssms;
                ssms.newStatus = BACT_STATUS_CREATE;
                ssms.setFlags = 0;
                ssms.unsetFlags = 0;
                kmnd->SetStateInternal(&ssms);

                __NC_STACK_ypabact *kmnd_bct = kmnd->getBACT_pBact();

                kmnd_bct->gid = bvMsg->vhcl[0].id;
                kmnd_bct->owner = bvMsg->owner;
                kmnd_bct->host_station = dynamic_cast<NC_STACK_yparobo *>(bhost->bacto);

                bhost->bacto->AddSubject(kmnd);

                netDebug_AddCreated(bhost->bact->owner, kmnd_bct->gid);

                int idid = 0;
                while (bvMsg->vhcl[idid].protoID != 0)
                {
                    newVhcl.vehicle_id = bvMsg->vhcl[idid].protoID;
                    newVhcl.pos = bvMsg->vhcl[idid].pos;

                    NC_STACK_ypabact *slv = yw->self_full->ypaworld_func146(&newVhcl);

                    if ( slv )
                    {
                        NC_STACK_ypagun *slv_gn = dynamic_cast<NC_STACK_ypagun *>(slv);

                        if (slv_gn)
                            slv_gn->ypagun_func128(bvMsg->vhcl[idid].base, false);

                        ssms.newStatus = BACT_STATUS_CREATE;
                        ssms.setFlags = 0;
                        ssms.unsetFlags = 0;
                        slv->SetStateInternal(&ssms);

                        __NC_STACK_ypabact *slv_bct = slv->getBACT_pBact();

                        slv_bct->gid = bvMsg->vhcl[idid].id;
                        slv_bct->owner = bvMsg->owner;
                        slv_bct->host_station = dynamic_cast<NC_STACK_yparobo *>(bhost->bacto);

                        kmnd->AddSubject(slv);

                        netDebug_AddCreated(bhost->bact->owner, slv_bct->gid);
                    }
                    idid++;
                }
            }
        }
        else
            log_netlog("\n+++ BV: Havent found robo with owner %d (%ds)\n", bvMsg->owner, yw->timeStamp / 1000);
    }
    break;

    case UAMSG_VIEWER:
    {
        szmsg = sizeof(uamessage_viewer);
        uamessage_viewer *vwMsg = (uamessage_viewer *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, vwMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ V: Havent found robo with owner %d (%ds)\n", vwMsg->owner, yw->timeStamp / 1000);
            break;
        }

        __NC_STACK_ypabact *fndbct = NULL;

        if ( vwMsg->classID == BACT_TYPES_MISSLE )
        {
            fndbct = yw_netGetMissileOfBact(bhost->bact, vwMsg->id, vwMsg->launcher);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found weapon %d of rifleman %d (%ds)\n", vwMsg->id, vwMsg->launcher, yw->timeStamp / 1000);
                strcpy(err, msg->senderID);
                break;
            }
        }
        else
        {
            fndbct = yw_netGetBactByID(bhost->bact, vwMsg->id);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found vehicle %d (%ds)\n", vwMsg->id, yw->timeStamp / 1000);
                strcpy(err, msg->senderID);
                break;
            }
        }

        if ( vwMsg->view )
            fndbct->status_flg |= BACT_STFLAG_ISVIEW;
        else
            fndbct->status_flg &= ~BACT_STFLAG_ISVIEW;
    }
    break;

    case UAMSG_SYNCGM:
    {
        szmsg = sizeof(uamessage_syncgame);
        uamessage_syncgame *sgMsg = (uamessage_syncgame *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
        {
            log_netlog("\n+++SG: received sync of a dead player %s\n", msg->senderID);
            break;
        }

        bact_node *bhost = yw_getHostByOwner(yw, sgMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SG: Havent found robo with owner %d (%ds)\n", sgMsg->owner, yw->timeStamp / 1000);
            break;
        }

        bhost->bact->gid = sgMsg->hostID;
        bact_node *v166 = (bact_node *)bhost->bact->subjects_list.head;
        int nmb = 0;

        while(v166->next)
        {
            bool rbgun = false;
            if ( v166->bact->bact_type == BACT_TYPES_GUN )
            {
                NC_STACK_ypagun *gno = dynamic_cast<NC_STACK_ypagun *>(v166->bacto);
                if (gno)
                    rbgun = gno->getGUN_roboGun();
            }

            if ( rbgun )
            {
                v166->bact->gid = sgMsg->gun[nmb];;
                nmb++;
            }

            v166 = (bact_node *)v166->next;
        }

        yw->GameShell->players[sgMsg->owner].rdyStart = 1;

        log_netlog("received READY TO PLAY from owner %d\n", sgMsg->owner);
    }
    break;

    case UAMSG_HOSTDIE:
    {
        uamessage_hostDie *hdMsg = (uamessage_hostDie *)msg->data;
        szmsg = sizeof(uamessage_hostDie);

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, hdMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", hdMsg->owner, yw->timeStamp / 1000);
            break;
        }

        int last = bhost->bacto->getBACT_yourLastSeconds();
        setState_msg stm;

        for ( bact_node *cm = (bact_node *)bhost->bact->subjects_list.head; cm->next; cm = (bact_node *)cm->next )
        {
            for ( bact_node *un = (bact_node *)cm->bact->subjects_list.head; un->next; un = (bact_node *)un->next )
            {
                sub_4F1A60(un->bact);
                sub_4F1B34(yw, un->bact);

                stm.newStatus = BACT_STATUS_CREATE;
                stm.unsetFlags = 0;
                stm.setFlags = 0;

                un->bacto->SetStateInternal(&stm);
                un->bact->status = BACT_STATUS_DEAD;

                sub_424000(&un->bact->soundcarrier, 3);
                startSound(&un->bact->soundcarrier, 7);

                un->bact->soundFlags &= 0xFFFFFFF7;
                un->bact->soundFlags |= 0x80;

                un->bacto->setBACT_yourLastSeconds(last);
                un->bact->status_flg |= BACT_STFLAG_DEATH1;
            }

            sub_4F1A60(cm->bact);
            sub_4F1B34(yw, cm->bact);

            stm.newStatus = BACT_STATUS_CREATE;
            stm.unsetFlags = 0;
            stm.setFlags = 0;

            cm->bacto->SetState(&stm);
            cm->bact->status = BACT_STATUS_DEAD;

            sub_424000(&cm->bact->soundcarrier, 3);
            startSound(&cm->bact->soundcarrier, 7);

            cm->bact->soundFlags &= 0xFFFFFFF7;
            cm->bact->soundFlags |= 0x80;

            cm->bacto->setBACT_yourLastSeconds(last);
            cm->bact->status_flg |= BACT_STFLAG_DEATH1;
        }

        sub_4F1A60(bhost->bact);
        sub_4F1B34(yw, bhost->bact);

        bhost->bact->status_flg |= 0x400;

        if ( yw->GameShell->netPlayerOwner != bhost->bact->owner )
        {
            int nwee = 0;
            int wee = 0;
            for ( bact_node *bnd = (bact_node *)yw->bact_list.head; bnd->next; bnd = (bact_node *)bnd->next )
            {
                if ( bnd->bact->bact_type == BACT_TYPES_ROBO &&
                        yw->GameShell->netPlayerOwner == bnd->bact->owner &&
                        bnd->bact->status != BACT_STATUS_DEAD )
                    wee = 1;

                if ( bnd->bact->bact_type == BACT_TYPES_ROBO &&
                        yw->GameShell->netPlayerOwner != bnd->bact->owner &&
                        bnd->bact->status != BACT_STATUS_DEAD )
                    nwee = 1;
            }

            if ( wee == 1 && nwee == 0 )
            {
                yw_arg159 arg159;
                arg159.field_4 = 10;
                arg159.field_C = 91;
                arg159.txt = 0;
                arg159.unit = 0;
                yw->self_full->ypaworld_func159(&arg159);

                yw->field_81CB.field_0 = 1;
                yw->field_81CB.field_4 = yw->timeStamp;
            }
            else
            {
                if ( !hdMsg->killerOwner )
                {
                    yw->field_81CB.field_0 = 4;
                    yw->field_81CB.field_4 = yw->timeStamp;

                    strncpy(yw->field_81CB.field_8, msg->senderID, sizeof(yw->field_81CB.field_8) - 1);
                }
                else if ( hdMsg->killerOwner == yw->GameShell->netPlayerOwner )
                {
                    yw->field_81CB.field_0 = 2;
                    yw->field_81CB.field_4 = yw->timeStamp;
                    strncpy(yw->field_81CB.field_8, msg->senderID, sizeof(yw->field_81CB.field_8) - 1);
                }
                else
                {
                    yw->field_81CB.field_0 = 3;
                    yw->field_81CB.field_4 = yw->timeStamp;
                    strncpy(yw->field_81CB.field_8, msg->senderID, sizeof(yw->field_81CB.field_8) - 1);

                    strncpy(yw->field_81CB.field_C, yw->GameShell->players[hdMsg->killerOwner].name, sizeof(yw->field_81CB.field_C) - 1);
                }

                yw_arg159 arg159;
                arg159.txt = 0;
                arg159.unit = 0;
                arg159.field_4 = 50;
                switch ( hdMsg->owner)
                {
                case 3:
                    arg159.field_C = 90;
                    break;
                case 4:
                    arg159.field_C = 89;
                    break;
                case 6:
                    arg159.field_C = 88;
                    break;
                case 1:
                    arg159.field_C = 87;
                    break;
                case 2:
                case 5:
                    break;
                }
                yw->self_full->ypaworld_func159(&arg159);
            }
        }

        if ( hdMsg->killerOwner )
        {
            bact_node *tmp = yw_getHostByOwner(yw, hdMsg->killerOwner);
            if ( tmp )
                bhost->bact->killer = yw_netGetBactByID(tmp->bact, hdMsg->killer);
            else
                bhost->bact->killer = NULL;
        }

        if ( bhost->bact->killer )
        {
            yw_arg184 mmsg;
            mmsg.type = 3;
            mmsg.t34.field_1 = 8 * bhost->bact->killer->owner | bhost->bact->owner;

            if ( bhost->bact->killer->self->getBACT_viewer() || bhost->bact->status_flg & BACT_STFLAG_ISVIEW )
                mmsg.t34.field_1 |= 0x80;

            if ( bhost->bact->self->getBACT_viewer() || bhost->bact->status_flg & BACT_STFLAG_ISVIEW )
                mmsg.t34.field_1 |= 0x40;

            mmsg.t34.field_2 = bhost->bact->vehicleID;

            if ( bhost->bact->bact_type == BACT_TYPES_ROBO )
                mmsg.t34.field_2 |= 0x8000;

            mmsg.t34.field_4 = bhost->bact->position.x * 256.0 / bhost->bact->wrldX;
            mmsg.t34.field_5 = bhost->bact->position.z * 256.0 / bhost->bact->wrldY;

            yw->self_full->ypaworld_func184(&mmsg);
        }

        if ( hdMsg->killer && hdMsg->killerOwner == yw->field_1b80->owner )
        {
            bact_node *nde = yw_getHostByOwner(yw, hdMsg->killerOwner);

            if ( nde )
            {
                //rarg134.unit = yw_netGetBactByID(nde->bact, hdMsg->killer);
                if ( !yw_netGetBactByID(nde->bact, hdMsg->killer) )
                    log_netlog("\n+++ RD: unknown ID %d for owner %d\n", hdMsg->killer, hdMsg->killerOwner);
            }
            else
            {
                log_netlog("\n+++ RD: false owner %d for killer %d\n", hdMsg->killerOwner, hdMsg->killer);
                //rarg134.unit = NULL;
            }
        }
        else
        {
            //rarg134.unit = NULL;
        }

        yw_arg177 arg177;
        arg177.field_4 = hdMsg->killerOwner;
        arg177.bact = bhost->bact;
        yw->self_full->ypaworld_func177(&arg177);
    }
    break;

    case UAMSG_MESSAGE:
    {
        uamessage_message *msgMsg = (uamessage_message *)msg->data;
        szmsg = sizeof(uamessage_message);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
            {
                int nmb = strtol(msgMsg->message, 0, 0);

                if (nmb > 0)
                {
                    sub_4D9550(yw, nmb);
                }
                else
                {
                    if ( yw->netGameStarted )
                    {
                        char bff[256];
                        sprintf(bff, "%s: %s", msg->senderID, msgMsg->message);

                        yw_arg159 arg159;
                        arg159.txt = bff;
                        arg159.field_C = 93;
                        arg159.field_4 = 10;
                        arg159.unit = NULL;

                        yw->self_full->ypaworld_func159(&arg159);
                    }
                    else
                    {
                        sub_4D0C24(yw, msg->senderID, msgMsg->message);
                    }
                }

                break;
            }

            plDat.ID++;
        }
    }
    break;

    case UAMSG_KICK:
    {
        uamessage_kick *kcMsg = (uamessage_kick *)msg->data;
        szmsg = sizeof(uamessage_kick);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->netProblemOwner = kcMsg->owner;

        strcpy(yw->GameShell->netProblemName, kcMsg->text);

        if ( !strcasecmp(kcMsg->text, yw->GameShell->callSIGN) )
        {
            yw->GameShell->players[owner].isKilled = 1;
            yw->GameShell->noSent = 1;
            yw->GameShell->netProblem = 3;
            yw->GameShell->netProblemCount = 10000;
        }
        else
        {
            yw_cleanPlayer(yw, 0, kcMsg->owner, 1);
            yw_DestroyPlayer(yw, kcMsg->text);

            if ( yw->GameShell->netProblem != 3 )
            {
                yw->GameShell->netProblem = 4;
                yw->GameShell->netProblemCount = 15000;
            }
        }

        if ( !strcasecmp(kcMsg->text, yw->GameShell->callSIGN) )
        {
            log_netlog(">>> I was kicked off by the host! (time %d)\n", yw->timeStamp / 1000);
        }
        else
        {
            log_netlog(">>> Host told me he has kicked off %s (time %d)\n", kcMsg->text, yw->timeStamp / 1000);
        }
    }
    break;

    case UAMSG_UPGRADE:
    {
        uamessage_upgrade *upMsg = (uamessage_upgrade *)msg->data;
        szmsg = sizeof(uamessage_upgrade);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        gemProto *gemProt = &yw->gems[upMsg->upgradeID];
        cellArea *cl = &yw->cells[yw->sectors_maxX2 * gemProt->sec_y + gemProt->sec_x];

        int vhcl, bld;
        sub_47C1EC(yw, gemProt, &vhcl, &bld);

        if ( vhcl )
            yw->VhclProtos[vhcl].disable_enable_bitmask = 0;

        if ( bld )
            yw->BuildProtos[bld].enable_mask = 0;

        yw_arg184 arg184;
        arg184.type = 7;
        arg184.t7.secX = gemProt->sec_x;
        arg184.t7.secY = gemProt->sec_y;
        arg184.t7.owner = owner;

        switch ( yw->GameShell->netPlayerOwner )
        {
        case 1:
            arg184.t7.last_vhcl = gemProt->nw_vproto_num_1;
            arg184.t7.last_build = gemProt->nw_bproto_num_1;
            break;
        case 6:
            arg184.t7.last_vhcl = gemProt->nw_vproto_num_2;
            arg184.t7.last_build = gemProt->nw_bproto_num_2;
            break;
        case 3:
            arg184.t7.last_vhcl = gemProt->nw_vproto_num_3;
            arg184.t7.last_build = gemProt->nw_bproto_num_3;
            break;
        case 4:
            arg184.t7.last_vhcl = gemProt->nw_vproto_num_4;
            arg184.t7.last_build = gemProt->nw_bproto_num_4;
            break;
        case 2:
        case 5:
            arg184.t7.last_build = 0;
            arg184.t7.last_vhcl = 0;
            break;
        }

        arg184.t7.last_weapon = 0;

        switch(gemProt->type)
        {
        case 25:
            arg184.t7.field_4 = 1;
            break;

        case 26:
            arg184.t7.field_4 = 2;
            break;

        case 27:
            arg184.t7.field_4 = 3;
            break;

        case 28:
            arg184.t7.field_4 = 4;
            break;

        case 78:
            arg184.t7.field_4 = 5;
            break;

        case 79:
            arg184.t7.field_4 = 6;
            break;

        default:
            arg184.t7.field_4 = 7;
            break;
        }

        yw->self_full->ypaworld_func184(&arg184);

        if ( cl->w_type != 4 )
        {
            char bff[256];
            strcpy(bff, get_lang_string(yw->string_pointers_p2, 229, "TECH-UPGRADE LOST! "));
            strcat(bff, gemProt->msg_default);

            yw_arg159 arg159;
            arg159.field_4 = 80;
            arg159.txt = bff;
            arg159.field_C = 29;
            arg159.unit = NULL;

            yw->self_full->ypaworld_func159(&arg159);

        }

        if ( upMsg->enable )
        {
            cl->w_type = 4;
            cl->w_id = upMsg->upgradeID;
        }
        else
        {
            cl->w_id = 0;
            cl->w_type = 0;
        }
    }
    break;

    case UAMSG_FRACTION:
    {
        szmsg = sizeof(uamessage_fraction);
        uamessage_fraction *frMsg = (uamessage_fraction *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->FreeFraction &= ~frMsg->newfrac;
        yw->GameShell->FreeFraction |= frMsg->freefrac;

        windp_arg79 plDat;
        plDat.ID = 0;
        plDat.mode = 0;

        while ( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
            {
                yw->GameShell->players2[plDat.ID].Fraction = frMsg->newfrac;
                break;
            }

            plDat.ID++;
        }
    }
    break;

    case UAMSG_WELCOME:
    {
        szmsg = sizeof(uamessage_welcome);
        uamessage_welcome *wmMsg = (uamessage_welcome *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while ( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
            {
                if ( yw->GameShell->remoteMode )
                    yw->GameShell->FreeFraction |= yw->GameShell->players2[plDat.ID].Fraction;

                yw->GameShell->players2[plDat.ID].Fraction = wmMsg->fraction;
                yw->GameShell->players2[plDat.ID].rdyStart = wmMsg->rdy;
                yw->GameShell->players2[plDat.ID].welcmd = 1;
                yw->GameShell->players2[plDat.ID].cd = wmMsg->cd;
            }
            plDat.ID++;
        }

        yw->GameShell->FreeFraction &= ~wmMsg->fraction;

        if ( yw->GameShell->SelectedFraction == wmMsg->fraction )
        {
            if ( yw->GameShell->FreeFraction & 1 )
            {
                yw->GameShell->SelectedFraction = 1;
            }
            else if ( yw->GameShell->FreeFraction & 2 )
            {
                yw->GameShell->SelectedFraction = 2;
            }
            else if ( yw->GameShell->FreeFraction & 4 )
            {
                yw->GameShell->SelectedFraction = 4;
            }
            else
            {
                yw->GameShell->SelectedFraction = 8;
            }
        }

        yw->GameShell->FreeFraction &= ~yw->GameShell->SelectedFraction;

        uamessage_fraction frMsg;
        frMsg.msgID = UAMSG_FRACTION;
        frMsg.freefrac = 0;
        frMsg.newfrac = yw->GameShell->SelectedFraction;
        frMsg.owner = yw->GameShell->netPlayerOwner;

        yw_arg181 nMsg;
        nMsg.dataSize = sizeof(uamessage_fraction);
        nMsg.recvFlags = 2;
        nMsg.data = &frMsg;
        nMsg.recvID = 0;
        nMsg.garant = 1;

        yw->self_full->ypaworld_func181(&nMsg);
    }
    break;

    case UAMSG_READY:
    {
        uamessage_ready *rdMsg = (uamessage_ready *)msg->data;
        szmsg = sizeof(uamessage_ready);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while(yw->windp->windp_func79(&plDat))
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
            {
                yw->GameShell->players2[plDat.ID].rdyStart = rdMsg->rdy;
                break;
            }
            plDat.ID++;
        }
    }
    break;

    case UAMSG_REQUPDATE:
    {
        szmsg = sizeof(uamessage_requpdate);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw_netSendUpdate(yw, yw->GameShell->netPlayerOwner, msg->senderID);
    }
    break;

    case UAMSG_UPDATE:
    {
        uamessage_update *updMsg = (uamessage_update *)msg->data;
        szmsg = updMsg->sz;

        if ( yw->GameShell->players[owner].isKilled )
        {
            log_netlog("\n+++UPD: got update from DEAD %s (%d)", msg->senderID, yw->timeStamp / 1000);
            plr->w84upd = 0;
        }
        else
        {
            yw_netRecvUpdate(yw, updMsg, updMsg->owner);

            if ( yw )
            {
                log_netlog("Received Update, tried successfully to restore vehicle structure\n");
            }
            else
            {
                log_netlog("DRAMATIC RESTORE ERROR. Couldn't restore data from owner %d\n", updMsg->owner);
            }
        }
    }
    break;

    case UAMSG_IMPULSE:
    {
        szmsg = sizeof(uamessage_impulse);
        uamessage_impulse *impMsg = (uamessage_impulse *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        if ( plr && plr->w84upd )
            break;

        yw_130arg secInfo;
        secInfo.pos_x = impMsg->pos.x;
        secInfo.pos_z = impMsg->pos.z;

        if ( !yw->self_full->ypaworld_func130(&secInfo) )
            break;

        bact_arg83 impls;
        impls.pos = impMsg->pos;
        impls.energ = impMsg->impulse;
        impls.pos2 = impMsg->dir;
        impls.force = impMsg->dir_len;
        impls.mass = impMsg->mass;


        __NC_STACK_ypabact *curUnit = (__NC_STACK_ypabact *)secInfo.pcell->units_list.head;

        while(curUnit->next)
        {
            if (curUnit->bact_type != BACT_TYPES_MISSLE &&
                    curUnit->bact_type != BACT_TYPES_ROBO &&
                    curUnit->bact_type != BACT_TYPES_TANK &&
                    curUnit->bact_type != BACT_TYPES_CAR &&
                    curUnit->bact_type != BACT_TYPES_GUN &&
                    curUnit->bact_type != BACT_TYPES_HOVER &&
                    !(curUnit->status_flg & BACT_STFLAG_DEATH2) &&
                    curUnit->owner == yw->field_1b80->owner)
                curUnit->self->ApplyImpulse(&impls);

            curUnit = (__NC_STACK_ypabact *)curUnit->next;
        }
    }
    break;

    case UAMSG_LOGMSG:
    {
        szmsg = sizeof(uamessage_logmsg);
        uamessage_logmsg *lgMsg = (uamessage_logmsg *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, lgMsg->senderOwner);
        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", lgMsg->senderOwner, yw->timeStamp / 1000);
            break;
        }

        if ( bhost->bacto != yw->field_1b78 )
            break;

        robo_arg134 rarg134;
        if ( lgMsg->sender )
        {
            rarg134.unit = yw_netGetBactByID(bhost->bact, lgMsg->sender);

            if ( !rarg134.unit )
                log_netlog("\n+++ LM: sender %d of owner %d not found (message %d)\n", lgMsg->sender, lgMsg->senderOwner, lgMsg->id);
        }
        else
        {
            rarg134.unit = NULL;
        }

        rarg134.field_14 = lgMsg->pri;
        rarg134.field_4 = lgMsg->id;
        rarg134.field_8 = lgMsg->pr1;
        rarg134.field_C = lgMsg->pr2;
        rarg134.field_10 = lgMsg->pr3;

        NC_STACK_yparobo *rbo = dynamic_cast<NC_STACK_yparobo *>(bhost->bacto);
        if (rbo)
            rbo->placeMessage(&rarg134);
    }
    break;

    case UAMSG_REORDER:
    {
        uamessage_reorder *ordMsg = (uamessage_reorder *)msg->data;
        szmsg = ordMsg->sz;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, ordMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ NORG: Havent found robo with owner %d (%ds)\n", ordMsg->owner, yw->timeStamp / 1000);
            break;
        }

        //yw_netGetUnitsCount(bhost->bact); // Not used

        __NC_STACK_ypabact *fnd = yw_netFindReorderUnit(bhost->bact, ordMsg->comm);
        if ( !fnd )
        {
            log_netlog("\n+++ NORG: Commander %d of owner %d not found (%ds)\n", ordMsg->comm, owner, yw->timeStamp / 1000);
            break;
        }

        fnd->commandID = ordMsg->commID;
        bhost->bacto->AddSubject(fnd->self);

        for (uint32_t i = 0; i < ordMsg->num; i++)
        {
            __NC_STACK_ypabact *fnds = yw_netFindReorderUnit(bhost->bact, ordMsg->units[i]);
            if ( fnds )
            {
                fnd->self->AddSubject(fnds->self);
            }
            else
            {
                log_netlog("\n+++ NORG: Slave %d of Owner %d not found (%ds)\n", ordMsg->units[i], owner, yw->timeStamp / 1000);
            }
        }
    }
    break;

    case UAMSG_LOBBYINIT:
    {
        uamessage_lobbyInit *lobbyMsg = (uamessage_lobbyInit *)msg->data;
        szmsg = sizeof(uamessage_lobbyInit);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->netLevelID = lobbyMsg->lvlID;
        yw->GameShell->netLevelName = get_lang_string(yw->string_pointers_p2, 1800 + yw->GameShell->netLevelID, yw->LevelNet->mapInfos[yw->GameShell->netLevelID].map_name);

        windp_arg79 plDat;
        plDat.ID = 0;
        plDat.mode = 0;


        while ( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(plDat.name, lobbyMsg->hostName) )
            {
                yw->GameShell->players2[plDat.ID].rdyStart = 1;
                break;
            }
            plDat.ID++;
        }

        mapINFO *minf = &yw->LevelNet->mapInfos[lobbyMsg->lvlID];
        int tmp = yw->windp->windp_func86(NULL);

        plDat.ID = 0;
        plDat.mode = 0;
        if ( minf->fractions_mask & 2 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_RESISTANCE;

                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_RESISTANCE;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( minf->fractions_mask & 0x40 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_GHORKOV;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_GHORKOV;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_GHORKOV;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( minf->fractions_mask & 8 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_MIKO;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_MIKO;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_MIKO;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( minf->fractions_mask & 0x10 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_TAER;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_TAER;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_TAER;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_TAER;
        }

        yw->SendCRC(yw->GameShell->netLevelID);
    }
    break;

    case UAMSG_SETLEVEL:
    {
        uamessage_setLevel *stlvlMsg = (uamessage_setLevel *)msg->data;
        szmsg = sizeof(uamessage_setLevel);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->netLevelID = stlvlMsg->lvlID;
        yw->GameShell->netLevelName = get_lang_string(yw->string_pointers_p2, 1800 + yw->GameShell->netLevelID, yw->LevelNet->mapInfos[yw->GameShell->netLevelID].map_name);

        for (int i = 0; i < 4; i++)
        {
            yw->GameShell->players2[i].checksum = 0;
        }

        mapINFO *minf = &yw->LevelNet->mapInfos[stlvlMsg->lvlID];
        int tmp = yw->windp->windp_func86(NULL);

        windp_arg79 plDat;
        plDat.ID = 0;
        plDat.mode = 0;

        if ( minf->fractions_mask & 2 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_RESISTANCE;

                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_RESISTANCE;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
        }

        if ( minf->fractions_mask & 0x40 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_GHORKOV;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_GHORKOV;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_GHORKOV;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_GHORKOV;
        }

        if ( minf->fractions_mask & 8 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_MIKO;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_MIKO;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_MIKO;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_MIKO;
        }

        if ( minf->fractions_mask & 0x10 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_TAER;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_TAER;
                yw->windp->windp_func79(&plDat);

                if ( !strcasecmp(plDat.name, yw->GameShell->callSIGN) )
                    yw->GameShell->SelectedFraction = FREE_FRACTION_TAER;

                tmp--;
                plDat.ID++;
            }
        }
        else
        {
            yw->GameShell->FreeFraction &= ~FREE_FRACTION_TAER;
        }

        yw->SendCRC(yw->GameShell->netLevelID);
    }
    break;

    case UAMSG_STARTPLASMA:
    {
        szmsg = sizeof(uamessage_startPlasma);
        uamessage_startPlasma *plasmaMsg = (uamessage_startPlasma *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, plasmaMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SP: Havent found robo with owner %d (%ds)\n", plasmaMsg->owner, yw->timeStamp / 1000);
            break;
        }

        __NC_STACK_ypabact *bbact = yw_netGetBactByID(bhost->bact, plasmaMsg->id);
        if ( !bbact )
        {
            log_netlog("\n+++ SP: vehicle %d doesn't exist\n", plasmaMsg->id);
            break;
        }

        bbact->scale_time = plasmaMsg->time;
        bbact->vp_extra[0].scale = plasmaMsg->scale;
        bbact->vp_extra[0].pos = plasmaMsg->pos;
        bbact->vp_extra[0].rotate = plasmaMsg->dir;
        bbact->vp_extra[0].pos = plasmaMsg->pos;

        bbact->vp_extra[0].vp.base = bbact->vp_genesis.base;
        bbact->vp_extra[0].vp.trigo = bbact->vp_genesis.trigo;
        bbact->vp_extra[0].flags |= EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE;

        bbact->vp_extra_mode = 1;
    }
    break;

    case UAMSG_ENDPLASMA:
    {
        szmsg = sizeof(uamessage_endPlasma);
        uamessage_endPlasma *endpMsg = (uamessage_endPlasma *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, endpMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", endpMsg->owner, yw->timeStamp / 1000);
            break;
        }

        if ( bhost->bacto != yw->field_1b78 )
            break;

        __NC_STACK_ypabact *bbact = yw_netGetBactByID(bhost->bact, endpMsg->id);
        if ( !bbact )
        {
            log_netlog("\n+++ EP: vehicle %d doesn't exist\n", endpMsg->id);
            break;
        }

        bbact->vp_extra_mode = 0;
        bbact->scale_time = -1;
    }
    break;

    case UAMSG_STARTBEAM:
    {
        szmsg = sizeof(uamessage_startBeam);
        uamessage_startBeam *stbmMsg = (uamessage_startBeam *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, stbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ STB: Havent found robo with owner %d (%ds)\n", stbmMsg->owner, yw->timeStamp / 1000);
            break;
        }

        NC_STACK_yparobo *orobo = dynamic_cast<NC_STACK_yparobo *>(bhost->bacto);
        if (orobo)
        {
            __NC_STACK_yparobo *robo = &orobo->stack__yparobo;

            robo->field_515 = stbmMsg->pos;
            robo->field_511 = 1500;

            startSound(&robo->bact_internal->soundcarrier, 9);
            bhost->bact->vp_extra_mode = 2;
        }
    }
    break;

    case UAMSG_ENDBEAM:
    {
        uamessage_endBeam *enbmMsg = (uamessage_endBeam *)msg->data;
        szmsg = sizeof(uamessage_endBeam);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        bact_node *bhost = yw_getHostByOwner(yw, enbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", enbmMsg->owner, yw->timeStamp / 1000);
            break;
        }

        bhost->bact->vp_extra[0].flags = 0;
        bhost->bact->vp_extra[1].flags = 0;
        bhost->bact->vp_extra_mode = 0;
    }
    break;

    case UAMSG_EXIT:
    {
        szmsg = sizeof(uamessage_exit);
        uamessage_exit *exitMsg = (uamessage_exit *)msg->data;

        if ( yw->GameShell->players[owner].isKilled && exitMsg->norm )
            break;

        if ( exitMsg->norm )
        {
            log_netlog(">>> received ANNOUNCEQUIT from %s at %d\n", msg->senderID, yw->timeStamp / 1000);
            yw->GameShell->players[owner].status = 3;
        }

        bact_node *bhost = yw_getHostByOwner(yw, exitMsg->owner);

        if ( !bhost || bhost->bact->status == BACT_STATUS_DEAD )
        {
            if ( exitMsg->norm )
                break;
        }

        yw->field_81CB.field_0 = 5;
        yw->field_81CB.field_4 = yw->timeStamp;
        strcpy(yw->field_81CB.field_8, msg->senderID);

        yw_arg159 arg159;
        arg159.field_4 = 50;
        arg159.txt = NULL;
        arg159.unit = NULL;

        switch ( exitMsg->owner )
        {
        case 3:
            arg159.field_C = 86;
            break;

        case 4:
            arg159.field_C = 85;
            break;

        case 6:
            arg159.field_C = 84;
            break;

        case 1:
            arg159.field_C = 83;
            break;

        default:
            break;
        }

        yw->self_full->ypaworld_func159(&arg159);
    }
    break;

    case UAMSG_CRC:
    {
        szmsg = sizeof(uamessage_crc);
        uamessage_crc *crcMsg = (uamessage_crc *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while ( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
                yw->GameShell->players2[plDat.ID].checksum = crcMsg->checksum;

            plDat.ID++;
        }

        yw->GameShell->takTime = 0;
    }
    break;

    case UAMSG_REQPING:
    {
        uamessage_ping *pingMsg = (uamessage_ping *)msg->data;
        szmsg = sizeof(uamessage_ping);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        uamessage_ping pongMsg;
        pongMsg.owner = yw->GameShell->netPlayerOwner;
        pongMsg.msgID = UAMSG_PONG;
        pongMsg.timestamp = pingMsg->timestamp;

        yw_arg181 nMsg;
        nMsg.data = &pongMsg;
        nMsg.dataSize = sizeof(pongMsg);
        nMsg.recvFlags = 1;
        nMsg.recvID = msg->senderID;
        nMsg.garant = 1;

        yw->self_full->ypaworld_func181(&nMsg);

        if ( yw->field_1b24.frameTime >= 3 )
            break;

        windp_arg82 warg82;
        warg82.senderFlags = 1;
        warg82.senderID = yw->GameShell->callSIGN;
        warg82.receiverID = 0;
        warg82.guarant = 0;
        warg82.receiverFlags = 2;

        yw->windp->windp_func82(&warg82);
    }
    break;

    case UAMSG_PONG:
    {
        uamessage_ping *pongMsg = (uamessage_ping *)msg->data;
        szmsg = sizeof(uamessage_ping);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        int latency = yw->GameShell->players[owner].latency;

        if (latency)
            yw->GameShell->players[owner].latency = (4 * latency + yw->timeStamp - pongMsg->timestamp) / 5;
        else
            yw->GameShell->players[owner].latency = yw->timeStamp - pongMsg->timestamp;

        if ( yw->GameShell->isHost && yw->GameShell->netProblem != 1 && yw->GameShell->players[owner].latency >= 7000 )
        {
            yw->GameShell->netProblem = 1;
            yw->GameShell->netProblemCount = 200;
            yw->GameShell->latencyCheck = 5;

            uamessage_startproblem prblm;
            prblm.owner = yw->GameShell->netPlayerOwner;
            prblm.msgID = UAMSG_STARTPROBLEM;
            prblm.problem = 1;

            yw_arg181 nMsg;
            nMsg.data = &prblm;
            nMsg.dataSize = sizeof(prblm);
            nMsg.recvFlags = 2;
            nMsg.recvID = 0;
            nMsg.garant = 1;

            yw->self_full->ypaworld_func181(&nMsg);
        }
    }
    break;

    case UAMSG_STARTPROBLEM:
    {
        uamessage_startproblem *prblmMsg = (uamessage_startproblem *)msg->data;
        szmsg = sizeof(uamessage_startproblem);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->netProblem = prblmMsg->problem;

        if ( prblmMsg->problem != 1 )
            break;

        yw->GameShell->netProblemCount = 500;
    }
    break;

    case UAMSG_ENDPROBLEM:
    {
        uamessage_endproblem *eprblmMsg = (uamessage_endproblem *)msg->data;
        szmsg = sizeof(uamessage_endproblem);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        yw->GameShell->netAllOk = eprblmMsg->solved;
        yw->GameShell->netAllOkCount = 3000;

        if ( eprblmMsg->solved == 3 )
            yw->GameShell->netAllOkCount = 0;

        yw->GameShell->netProblem = 0;
    }
    break;

    case UAMSG_CD:
    {
        uamessage_cd *cdMsg = (uamessage_cd *)msg->data;
        szmsg = sizeof(uamessage_cd);

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while ( yw->windp->windp_func79(&plDat) )
        {
            if ( !strcasecmp(msg->senderID, plDat.name) )
            {
                yw->GameShell->players2[plDat.ID].cd = cdMsg->cd;

                if ( cdMsg->rdy != -1 )
                {
                    yw->GameShell->players2[plDat.ID].rdyStart = cdMsg->rdy;
                }
                break;
            }

            plDat.ID++;
        }

    }
    break;

    case UAMSG_SCORE:
    {
        uamessage_score *scoreMsg = (uamessage_score *)msg->data;
        szmsg = sizeof(uamessage_score);

        for(int i = 0; i < 8; i++)
        {
            yw->ingamePlayerStatus[i] = scoreMsg->status[i];
        }
    }
    break;

    default:
        printf("Make me yw_handleNormMsg :  UAMSG_BASE + %d\n", msgID - UAMSG_BASE);
        break;
    }

    return szmsg;
}

const char *yw_corruptionCheck(UserData *usr)
{
    _NC_STACK_ypaworld *ywo = usr->p_ypaworld;
    __NC_STACK_ypabact *found = NULL;

    if ( ywo->timeStamp - usr->deadCheck >= 100000 )
    {
        usr->deadCheck = ywo->timeStamp;

        bact_node *stations = (bact_node *)ywo->bact_list.head;
        while ( stations->next )
        {
            if ( stations->bact->owner != usr->netPlayerOwner )
            {
                bact_node *comm = (bact_node *)stations->bact->subjects_list.head;
                while ( comm->next )
                {
                    bact_node *unit = (bact_node *)comm->bact->subjects_list.head;
                    while ( unit->next )
                    {
                        if ( ywo->timeStamp - unit->bact->lastFrmStamp > 180000 )
                        {
                            found = unit->bact;
                            break;
                        }

                        unit = (bact_node *)unit->next;
                    }

                    if ( found )
                        break;

                    if ( ywo->timeStamp - comm->bact->lastFrmStamp > 180000 )
                    {
                        found = comm->bact;
                        break;
                    }

                    comm = (bact_node *)comm->next;
                }

                if ( found )
                    break;

                if ( ywo->timeStamp - stations->bact->lastFrmStamp > 180000 )
                {
                    found = stations->bact;
                    break;
                }
            }

            stations = (bact_node *)stations->next;
        }

        if ( found )
        {
            log_netlog("\n+++ CC: found old vehicle id %d, class %d, owner %d at time %d. Request update\n", found->gid, found->bact_type, found->owner, ywo->timeStamp / 1000);
            return usr->players[found->owner].name;
        }
    }
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
                ywMsg.dataSize = sizeof(wlcmMsg);
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

        vec3d place(selHost->pos_x, selHost->pos_y, selHost->pos_z);

        strncpy(usr->players[owner].name, plData.name, 64);

        ypaworld_arg136 arg136;
        arg136.pos_x = place.x;
        arg136.pos_y = -30000.0;
        arg136.pos_z = place.z;
        arg136.field_18 = 50000.0;
        arg136.field_14 = 0;
        arg136.field_1C = 0;
        arg136.field_40 = 0;

        yw->self_full->ypaworld_func136(&arg136);

        if ( arg136.field_20 )
            place.y += arg136.field_30;
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


void yw_NetHandleProblems(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;
    switch ( usr->netProblem )
    {
    case 1:
        usr->field_0x10 = 1;
        usr->netProblemCount--;

        if ( usr->netProblemCount <= 0 )
        {
            usr->netProblem = 0;
            usr->netAllOk = 3;
            usr->netAllOkCount = 0;

            for (int i = 0; i < 4; i++)
                usr->players[i].latency = 0;

            if ( usr->isHost )
            {
                uamessage_endproblem eProblm;
                eProblm.owner = usr->netPlayerOwner;
                eProblm.msgID = UAMSG_ENDPROBLEM;
                eProblm.solved = 3;

                yw_arg181 arg181;
                arg181.recvFlags = 2;
                arg181.recvID = 0;
                arg181.data = &eProblm;
                arg181.garant = 1;
                arg181.dataSize = sizeof(uamessage_endproblem);

                yw->self_full->ypaworld_func181(&arg181);
            }
        }
        break;

    case 3:
        usr->netProblemCount -= yw->field_1b24.frameTime;
        if ( usr->netProblemCount < 0 )
        {
            yw->field_2d90->field_40 = 2;
            usr->netProblem = 0;
        }
        break;

    case 4:
        usr->netProblemCount -= yw->field_1b24.frameTime;

        if ( usr->netProblemCount < 0 )
        {
            usr->netProblem = 0;
            for (int i = 0; i < 8; i++)
                usr->players[i].isKilled &= ~2;
        }
        break;

    default:
        break;
    }
}

int yw_NetCheckPlayers(_NC_STACK_ypaworld *yw)
{
    int v2 = 0;
    UserData *usr = yw->GameShell;

    windp_arg79 arg79;
    arg79.ID = 0;
    arg79.mode = 0;
    while ( yw->windp->windp_func79(&arg79) )
    {
        if ( arg79.flags & 1 )
        {
            arg79.ID++;
        }
        else
        {
            int owner = 0;
            netType2 *plr = NULL;

            for ( int i = 0; i < 8; i++ )
            {
                if ( strcasecmp(usr->players[i].name, arg79.name) == 0 )
                {
                    owner = i;
                    plr = &usr->players[i];
                    break;
                }
            }

            if ( plr )
            {
                if ( plr->problemCnt > 0 )
                {
                    plr->problemCnt -= yw->field_1b24.frameTime;
                    if ( plr->problemCnt <= 0 )
                        plr->status = 1;
                }

                int tm = yw->field_1b24.gTime - plr->lastMsgTime;

                if ( yw->netGameStarted && tm > 7000 )
                {
                    SPEED_DOWN_NET = 1;

                    if ( tm / 1000 != (tm - yw->field_1b24.frameTime) / 1000 )
                    {
                        log_netlog("Waiting for player %s. (time %ds at %ds)\n", arg79.name, tm / 1000, yw->timeStamp / 1000);
                        log_netlog("    Reduce data transfer rate\n");
                    }

                    usr->problemCnt = 4000;

                    plr->status = 2;
                    plr->problemCnt = 5000;

                    if ( (yw->field_1b24.gTime - (int)plr->lastMsgTime) > usr->kickTime )
                    {
                        if ( usr->isHost )
                        {
                            uamessage_kick msgKk;
                            msgKk.owner = owner;
                            msgKk.msgID = UAMSG_KICK;
                            strcpy(msgKk.text, plr->name);

                            yw_cleanPlayer(yw, plr->name, 0, 0);

                            if ( usr->netProblem != 3 )
                            {
                                strcpy(usr->netProblemName, msgKk.text);
                                usr->netProblem = 4;
                                usr->netProblemCount = 15000;
                            }

                            v2 = 1;

                            yw_arg181 yw181;
                            yw181.data = &msgKk;
                            yw181.dataSize = sizeof(uamessage_kick);
                            yw181.recvFlags = 2;
                            yw181.recvID = 0;
                            yw181.garant = 1;
                            yw->self_full->ypaworld_func181(&yw181);

                            yw_DestroyPlayer(yw, msgKk.text);

                            log_netlog(">>> I have kicked off %s because I haven't heard anything for %d sec (at time %d)\n", msgKk.text, usr->kickTime / 1000, yw->timeStamp / 1000);
                        }
                    }

                    if ( (yw->field_1b24.gTime - plr->lastMsgTime) > 20000 )
                    {
                        if ( !usr->isHost )
                        {
                            if ( usr->netProblem != 4 && usr->netProblem != 3 )
                            {
                                usr->netProblem = 5;
                                usr->netProblemCount = 10;
                                v2 = 1;
                            }
                        }
                    }

                    plr->connProblem = 1;
                    arg79.ID++;
                }
                else
                {
                    plr->connProblem = 0;
                    arg79.ID++;
                }
            }
            else
            {
                log_netlog("Warning: No Playerdata for player %s in PlayersOK() (%ds)\n", arg79.name, yw->timeStamp / 1000);
                arg79.ID++;
            }
        }
    }

    if ( usr->netProblem == 5 && !v2 )
        usr->netProblem = 0;

    return 1;
}

int yw_NetCheckPlayersInGame(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    int numpl = yw->windp->windp_func86(NULL);

    if ( yw->netGameStarted )
        return 1;

    int rdypl = 0;
    netType2 *wting[8];

    for(int i = 0; i < 8; i++)
    {
        if (usr->players[i].name[0])
        {
            if ( usr->players[i].rdyStart )
            {
                rdypl++;
                wting[i] = NULL;
            }
            else
            {
                wting[i] = &usr->players[i];
            }
        }
        else
        {
            wting[i] = NULL;
        }
    }

    if ( rdypl == numpl )
    {
        yw->netGameStarted = 1;
        return 1;
    }

    SDL_Delay(50);

    struC5 inpt;
    memset(&inpt, 0, sizeof(struC5));
    INPe.sub_412D28(&inpt);

    char buf[1024];
    char *cur = buf;

    FontUA::select_tileset(&cur, 0);
    FontUA::set_xpos(&cur, 0);
    FontUA::set_ypos(&cur, 0);
    FontUA::copy_position(&cur);
    FontUA::add_txt(&cur, 2 * yw->screen_width / 3 - 1, 1,  get_lang_string(ypaworld__string_pointers, 651, "WAITING FOR PLAYERS: ") );
    FontUA::next_line(&cur);

    yw->netStartTime -= inpt.period;
    if ( yw->netStartTime > 0 )
    {
        char tmpstr[128];
        sprintf(tmpstr, "(%d)", yw->netStartTime / 1000);

        FontUA::copy_position(&cur);
        FontUA::add_txt(&cur, yw->screen_width / 3 - 1, 1, tmpstr);
        FontUA::next_line(&cur);
    }

    for(int i = 1; i < 8; i++)
    {
        if (wting[i])
        {
            if ( yw->netStartTime <= 0 && usr->isHost )
            {
                uamessage_kick msgKk;
                strcpy(msgKk.text, wting[i]->name);
                msgKk.msgID = UAMSG_KICK;
                msgKk.owner = i;

                yw_cleanPlayer(yw, wting[i]->name, 0, 0);

                yw_arg181 yw181;
                yw181.recvFlags = 2;
                yw181.data = &msgKk;
                yw181.recvID = 0;
                yw181.garant = 1;
                yw181.dataSize = sizeof(uamessage_kick);

                yw->self_full->ypaworld_func181(&yw181);

                yw_DestroyPlayer(yw, msgKk.text);

                if ( usr->netProblem != 3 )
                {
                    strcpy(usr->netProblemName, msgKk.text);

                    yw->GameShell->netProblem = 4;
                    yw->GameShell->netProblemCount = 15000;
                }
                log_netlog(">>> I have kicked off %s because I didn't heard anything after loading (time %d)\n", msgKk.text, yw->timeStamp / 1000);
                log_netlog("    netstarttime was %d\n", yw->netStartTime);
            }
            else
            {
                char tmpstr[128];
                strcpy(tmpstr, "     ");
                strcpy( &tmpstr[3], wting[i]->name);

                FontUA::copy_position(&cur);
                FontUA::add_txt(&cur, yw->screen_width - 1, 1, tmpstr);
                FontUA::next_line(&cur);
            }
        }
    }

    if ( yw->netStartTime <= 0 )
    {
        if ( yw->GameShell->isHost )
            return 1;

        FontUA::copy_position(&cur);
        FontUA::add_txt(&cur, yw->screen_width - 1, 1, "WAITING FOR HOST");
        FontUA::next_line(&cur);
    }

    FontUA::set_end(&cur);

    yw->win3d->BeginFrame();
    yw->win3d->LockSurface();

    w3d_a209 arg209;
    arg209.cmdbuf = buf;
    arg209.includ = NULL;
    yw->win3d->raster_func209(&arg209);

    yw->win3d->UnlockSurface();
    yw->win3d->EndFrame();

    return 0;
}


void yw_NetCheckPing(_NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    if ( usr->latencyCheck <= 0 && usr->isHost && (usr->netProblem == 0 || usr->netProblem == 1) )
    {
        if ( usr->netProblem == 1 )
            usr->latencyCheck = 5;
        else
            usr->latencyCheck = 2000;

        uamessage_ping rPing;
        rPing.owner = usr->netPlayerOwner;
        rPing.msgID = UAMSG_REQPING;
        rPing.timestamp = yw->timeStamp;

        yw_arg181 yw181;
        yw181.recvFlags = 2;
        yw181.recvID = 0;
        yw181.data = &rPing;
        yw181.garant = 1;
        yw181.dataSize = sizeof(uamessage_ping);

        yw->self_full->ypaworld_func181(&yw181);

        if ( yw->field_1b24.frameTime < 3 )
        {
            windp_arg82 dp82;
            dp82.senderFlags = 1;
            dp82.receiverID = 0;
            dp82.senderID = usr->callSIGN;
            dp82.guarant = 0;
            dp82.receiverFlags = 2;
            yw->windp->windp_func82(&dp82);
        }
    }
    else
    {
        usr->latencyCheck -= yw->field_1b24.frameTime;
    }
}


void yw_NetMsgHndlLoop(_NC_STACK_ypaworld *yw)
{
    while ( 1 )
    {
        if ( yw->GameShell->netProblem != 0 )
            yw_NetHandleProblems(yw);

        yw->GameShell->netAllOkCount -= yw->field_1b24.frameTime;

        yw_HandleNetMsg(yw);

        if ( yw->field_2d90->field_40 == 2 )
            break;

        SPEED_DOWN_NET = 0;

        if ( yw_NetCheckPlayers(yw) && yw_NetCheckPlayersInGame(yw) )
        {
            yw_NetCheckPing(yw);

            if ( yw->GameShell->problemCnt > 0 )
                yw->GameShell->problemCnt -= yw->field_1b24.frameTime;
            break;
        }
    }
}


void yw_NetDrawStats(_NC_STACK_ypaworld *yw)
{
    bool toDraw = false;

    char t[16][300];

    UserData *usr = yw->GameShell;

    tiles_stru *font29 = yw->tiles[29];
    char drawbuf[2000];
    char *cur = drawbuf;

    FontUA::select_tileset(&cur, 29);
    FontUA::set_center_xpos(&cur, font29->font_height - (yw->screen_width / 2) );
    FontUA::set_center_ypos(&cur, font29->font_height - (yw->screen_height / 2) );

    if ( usr->disconnected == 0 && usr->problemCnt > 0 )
    {
        if ( (yw->field_1b24.gTime / 300) & 1 )
        {
            FontUA::store_u8(&cur, 65);
        }
    }

    FontUA::set_end(&cur);

    yw->win3d->LockSurface();

    w3d_a209 v77;
    v77.cmdbuf = drawbuf;
    v77.includ = NULL;

    yw->win3d->raster_func209(&v77);

    yw->win3d->UnlockSurface();

    int numelm = 0;

    if ( usr->netProblem )
    {
        toDraw = true;

        switch ( usr->netProblem )
        {
        case 1:
            if ( usr->isHost )
            {
                strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2405, "HOST: LATENCY PROBLEMS."));
                sprintf(t[1], "%s %d", get_lang_string(ypaworld__string_pointers, 2423, "PLEASE WAIT"), usr->netProblemCount);
                numelm = 2;
            }
            else
            {
                strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2406, "CLIENT: LATENCY PROBLEMS."));
                sprintf(t[1], "%s %d", get_lang_string(ypaworld__string_pointers, 2424, "PLEASE WAIT"), usr->netProblemCount);
                numelm = 2;
            }
            break;

        case 3:
            strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2425, "YOU ARE KICKED OFF BECAUSE NETTROUBLE"));
            sprintf(t[1], "%s %d", get_lang_string(ypaworld__string_pointers, 2426, "LEVEL FINISHES AUTOMATICALLY"), usr->netProblemCount / 1000);
            numelm = 2;
            break;

        case 4:
            strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2427, "FOLLOWING PLAYERES WERE REMOVED"));
            strcpy(t[1], get_lang_string(ypaworld__string_pointers, 2428, "BECAUSE THEY HAD NETWORK PROBLEMS"));
            numelm = 2;

            for(int i = 0; i < 8; i++)
            {
                if (usr->players[i].isKilled & 2)
                {
                    strcpy(t[numelm], usr->players[i].name);
                    numelm++;
                }
            }
            break;

        case 5:
            strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2429, "NO CONNECTION TO FOLLOWING PLAYERS"));
            strcpy(t[1], get_lang_string(ypaworld__string_pointers, 2430, "FINISH IF PROBLEM CANNOT NE SOLVED"));
            numelm = 2;

            for(int i = 0; i < 8; i++)
            {
                if ( (yw->field_1b24.gTime - usr->players[i].lastMsgTime) > 20000 )
                {
                    if ( usr->players[i].rdyStart )
                    {
                        if ( strcasecmp(usr->players[i].name, usr->callSIGN) )
                        {
                            strcpy(t[numelm], usr->players[i].name);
                            numelm++;
                        }
                    }
                }
            }
            break;

        default:
            break;
        }
    }
    else
    {
        if ( usr->netAllOkCount > 0 )
        {
            numelm = 1;
            toDraw = true;

            if ( usr->netAllOk == 1 )
            {
                strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2409, "NETWORK IS NOW OK"));
            }
            else if ( usr->netAllOk == 2 )
            {
                strcpy(t[0], get_lang_string(ypaworld__string_pointers, 2408, "THERE WAS NO CHANCE TO SOLVE THIS PROBLEM"));
            }
            else if ( usr->netAllOk == 3 )
            {
                strcpy(t[0], "   ");
                toDraw = false;
            }
            else
            {
                strcpy(t[0], "???");
            }
        }
    }

    if ( toDraw )
    {
        tiles_stru *font0 = yw->tiles[0];
        cur = drawbuf;

        FontUA::select_tileset(&cur, 0);
        FontUA::set_center_xpos(&cur, yw->screen_width / 4 - yw->screen_width / 2);
        FontUA::set_center_ypos(&cur, 12 - yw->screen_height / 2);

        cur = GuiBase::FormateTitle(yw, yw->screen_width / 4 - yw->screen_width / 2,
                                    12 - yw->screen_height / 2,
                                    yw->screen_width / 2,
                                    get_lang_string(ypaworld__string_pointers, 2407, "NETZWERKSTATUS"),
                                    cur, 0, 0);

        FontUA::next_line(&cur);

        FontUA::select_tileset(&cur, 0);

        for(int i = 0; i < numelm; i++)
        {
            FontUA::store_u8(&cur, '{');

            FontUA::copy_position(&cur);
            FontUA::op17(&cur, yw->screen_width / 2 - font0->chars[(int)'}'].width);

            FontUA::store_u8(&cur, ' ');
            FontUA::store_u8(&cur, '}');

            FontUA::add_txt(&cur, yw->screen_width / 2 - 2 * font0->chars[(int)'W'].width, 4, t[i] );

            FontUA::next_line(&cur);
        }

        FontUA::set_yoff(&cur, font0->font_height - 1);

        FontUA::store_u8(&cur, 'x');
        FontUA::op17(&cur, yw->screen_height / 2 - font0->chars[(int)'z'].width);
        FontUA::store_u8(&cur, 'y');
        FontUA::store_u8(&cur, 'z');

        FontUA::set_end(&cur);

        yw->win3d->LockSurface();

        w3d_a209 v77;
        v77.cmdbuf = drawbuf;
        v77.includ = NULL;

        yw->win3d->raster_func209(&v77);

        yw->win3d->UnlockSurface();
    }
}
