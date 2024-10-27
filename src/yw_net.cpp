#include <stdlib.h>
#include <string.h>
#include "system/sound.h"
#include "yw.h"
#include "yw_internal.h"
#include "yw_net.h"
#include "log.h"
#include "ypabact.h"
#include "yparobo.h"
#include "ypagun.h"
#include "ypamissile.h"
#include "font.h"
#include "windp.h"
#include <fmt/printf.h>
#include "system/inivals.h"

#define C_2PI_127       0.04947390005653217698   // (2PI / 127)
#define C_127_2PI       20.21267777267070764265  // (127 / 2PI)

extern uint32_t bact_id;
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

void yw_netReportError(NC_STACK_ypabact *host, uint32_t id)
{
    NC_STACK_ypaworld *ywo = host->getBACT_pWorld();

    if ( ywo->_userRobo != host )
    {
        bool created = false;
        for (int i = 0; i < netDebug_CREATE_CNT; i++)
        {
            if ( netDebug_CREATED[host->_owner][i] == id )
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
            if ( netDebug_DESTROYED[host->_owner][i] == id )
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


void NC_STACK_ypaworld::SendCRC(int lvlid)
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
    
    crc = fileCrc32( fmt::sprintf("levels:multi/L%02d%02d.ldf", lvlid, lvlid) , crc);

    _GameShell->netCRC = crc;

    uamessage_crc crcmsg;
    crcmsg.msgID = UAMSG_CRC;
    crcmsg.owner = _GameShell->netPlayerOwner;
    crcmsg.checksum = crc;

    NetBroadcastMessage(&crcmsg, sizeof(crcmsg), true);
}

void yw_CheckCRCs(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->_GameShell;

    std::string strs[4];

    if ( usr->takTime > 0 )
    {
        usr->takTime -= usr->DTime;
    }
    else
    {
        usr->takTime = 15000;

        int plCnt = yw->_netDriver->GetPlayerCount();

        bool err = false;
        int errmsg = 0;

        for (int i = 0; i < plCnt; i++)
        {
            UserData::TNetPlayerLobbyData &pl = yw->_GameShell->lobbyPlayers[i];
            if ( StriCmp(yw->_GameShell->netPlayerName, pl.Name) &&
                    usr->netCRC != pl.DataChecksum &&
                    pl.DataChecksum != 0 &&
                    usr->netCRC != 0 )
            {
                err = true;
                strs[errmsg] = fmt::sprintf("%s %s", pl.Name, Locale::Text::Advanced(Locale::ADV_HASHMISS) );
                errmsg++;
            }
        }

        if ( err )
        {
            const std::string lngstr = Locale::Text::Advanced(Locale::ADV_COMPUTER);

            for (int i = 0; i < errmsg; i++)
            {
                sub_4D0C24(yw, lngstr, strs[i]);
            }
        }
    }
}


NC_STACK_ypabact * NC_STACK_ypaworld::yw_getHostByOwner(uint8_t owner)
{
    for( NC_STACK_ypabact* &bct : _unitsList )
    {
        if ( bct->_owner == owner && bct->_bact_type == BACT_TYPES_ROBO )
            return bct;
    }

    return NULL;
}

NC_STACK_ypabact * NetGetBactByID(NC_STACK_ypabact *host, uint32_t id)
{
    if ( id == host->_gid )
        return host;

    for( NC_STACK_ypabact* &comm : host->_kidList )
    {
        if ( id == comm->_gid )
            return comm;

        for( NC_STACK_ypabact* &unit : comm->_kidList )
        {
            if ( unit->_gid == id)
                return unit;
        }
    }

    yw_netReportError(host, id);

    return NULL;
}

void yw_netBakeVhcl(NC_STACK_ypabact *bact, uamessage_vhclData *dat, int id, int interpolate)
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

    common->pos_x = (int32_t)bact->_position.x / 2;
    common->pos_y = (int32_t)bact->_position.y / 2;
    common->pos_z = (int32_t)bact->_position.z / 2;

    if ( !interpolate && extended)
    {
        extended->speed = bact->_fly_dir * bact->_fly_dir_length;
    }

    if ( bact->getBACT_viewer() && (bact->_status_flg & BACT_STFLAG_LAND) )
    {
        float len = bact->_viewer_overeof - bact->_overeof;
        common->pos_y += (int32_t)len / 2;
    }

    if ( bact->_bact_type == BACT_TYPES_GUN )
    {
        common->pos_x = (int32_t)bact->_old_pos.x / 2;
        common->pos_y = (int32_t)bact->_old_pos.y / 2;
        common->pos_z = (int32_t)bact->_old_pos.z / 2;

        common->specialinfo |= vhcldata::SI_YPAGUN;
    }
    else
    {
        common->specialinfo &= ~vhcldata::SI_YPAGUN;
    }

    vec3d out = bact->_rotation.GetEuler();

    common->roll = out.x * C_127_2PI;
    common->pitch = out.y * C_127_2PI;
    common->yaw = out.z * C_127_2PI;

    common->specialinfo &= ~vhcldata::SI_UNK;
    common->ident = bact->_gid;

    if ( bact->_status_flg & BACT_STFLAG_NORENDER )
        common->specialinfo |= vhcldata::SI_NORENDER;
    else
        common->specialinfo &= ~vhcldata::SI_NORENDER;

    if ( bact->_status_flg & BACT_STFLAG_LAND )
        common->specialinfo |= vhcldata::SI_LAND;
    else
        common->specialinfo &= ~vhcldata::SI_LAND;

    if ( bact->_bact_type == BACT_TYPES_ROBO )
        common->specialinfo |= vhcldata::SI_ROBO;
    else
        common->specialinfo &= ~vhcldata::SI_ROBO;

    if ( bact->_status_flg & BACT_STFLAG_DSETTED )
        common->specialinfo |= vhcldata::SI_DSETTED;
    else
        common->specialinfo &= ~vhcldata::SI_DSETTED;

    bact->_status_flg &= ~BACT_STFLAG_DSETTED;

    common->energy = bact->_energy;
    common->vp = 0;
}

bool yw_prepareVHCLData(NC_STACK_ypaworld *yw, uamessage_vhclData *dat)
{
    NC_STACK_ypabact *fndRobo = NULL;

    for( NC_STACK_ypabact* &robo : yw->_unitsList)
    {
        if ( yw->_userRobo == robo )
        {
            fndRobo = robo;
            break;
        }
    }

    if ( !fndRobo )
    {
        log_netlog("CollectVehicleData: My Robo doesn't exist!\n");
        return false;
    }

    yw_netBakeVhcl(fndRobo, dat, 0, yw->_netInterpolate);
    dat->owner = fndRobo->_owner;

    int num = 1;

    for( NC_STACK_ypabact* &comm : fndRobo->_kidList )
    {
        if (num >= 1023)
            break;

        yw_netBakeVhcl(comm, dat, num, yw->_netInterpolate);
        num++;

        for( NC_STACK_ypabact* &unit : comm->_kidList )
        {
            if ( num >= 1023 )
                break;
            yw_netBakeVhcl(unit, dat, num, yw->_netInterpolate);

            num++;
        }
    }

    dat->hdr.number = num;

    if (yw->_netInterpolate )
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

void yw_cleanPlayer(NC_STACK_ypaworld *yw, const std::string &name, uint8_t owner, uint8_t mode)
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
                log_netlog("destroy player: robo of player %s not found!\n", name.c_str());
                log_netlog("In cases of trouble with other players this is no problem\n");
                return;
            }

            if ( !StriCmp(yw->_GameShell->netPlayers[own].Name, name) && yw->_GameShell->netPlayers[own].DestroyFlags == 0 )
                break;

            own++;
        }
    }

    NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(own);
    if ( bhost )
    {
        yw->NetRemove(bhost);
        yw->_GameShell->netPlayers[own].DestroyFlags = 3;
    }
}

void NC_STACK_ypaworld::yw_netApplyVhclDataI(NC_STACK_ypabact *bact, uamessage_vhclDataI *dat, int id, uint32_t timestamp)
{
    if ( id < dat->hdr.number )
    {
        vec3d v49;
        v49.x = 2 * dat->data[id].pos_x;
        v49.y = 2 * dat->data[id].pos_y;
        v49.z = 2 * dat->data[id].pos_z;

        if ( v49.x < 0.0 || v49.x > bact->_wrldSize.x || v49.z > 0.0 || v49.z < bact->_wrldSize.y )
            log_netlog(
                "\n+++ EVD: impossible position x %7.2f(%d) z %7.2f(%d) of object %d\n",
                v49.x,
                dat->data[id].pos_x,
                v49.z,
                dat->data[id].pos_z,
                bact->_gid);

        float dtime = dat->hdr.diffTime * 0.001;

        vec3d tmp = (v49 - bact->_old_pos) / (dtime * 6.0);

        bact->_fly_dir_length = tmp.normalise();

        if ( bact->_fly_dir_length > 0.0001 )
            bact->_fly_dir = tmp;

        bact->_position = bact->_old_pos;
        bact->_old_pos = v49;

        if ( dat->data[id].specialinfo & vhcldata::SI_DSETTED )
        {
            bact->_position = bact->_old_pos;
            bact->_fly_dir_length = 0;
        }

        vec3d v47 = vec3d(dat->data[id].roll, dat->data[id].pitch, dat->data[id].yaw)  *  C_2PI_127;

        mat3x3 out = mat3x3::Euler(v47);

        bact->_netDRot = (out - bact->_netRotation) / dtime;

        bact->_rotation = bact->_netRotation;
        bact->_netRotation = out;

        if ( dat->data[id].specialinfo & vhcldata::SI_NORENDER )
            bact->_status_flg |= BACT_STFLAG_NORENDER;

        if ( dat->data[id].specialinfo & vhcldata::SI_LAND )
            bact->_status_flg |= BACT_STFLAG_LAND;
        else
            bact->_status_flg &= ~BACT_STFLAG_LAND;

        bact->_lastFrmStamp = timestamp;
        bact->_energy = dat->data[id].energy;
    }
}

void NC_STACK_ypaworld::yw_netApplyVhclDataE(NC_STACK_ypabact *bact, uamessage_vhclDataE *dat, int id, uint32_t timestamp)
{
    if ( dat->hdr.number > id )
    {
        int32_t tmstmp = timestamp - bact->_lastFrmStamp;
        if ( tmstmp > 2 )
        {
            bact->_old_pos = bact->_position;

            bact->_position.x = 2 * dat->data[id].pos_x;
            bact->_position.y = 2 * dat->data[id].pos_y;
            bact->_position.z = 2 * dat->data[id].pos_z;

            vec3d v73 = dat->data[id].speed;

            float dtime = tmstmp * 0.001;

            bact->_netDSpeed = (v73 - bact->_fly_dir * bact->_fly_dir_length) / dtime;

            float spd = bact->_netDSpeed.length();

            if ( spd > maxSpeed )
            {
                if ( bact->_bact_type != BACT_TYPES_MISSLE )
                {
                    log_netlog("d_speed max: %10.5f / class %d dof.v old %10.5f with net_time %4.3f\n", spd, bact->_bact_type, bact->_fly_dir_length, dtime);
                    maxSpeed = spd;
                }
            }

            if ( spd > 400.0 )
            {
                bact->_netDSpeed *= 4.0 / spd;
            }

            if ( bact->_bact_type != BACT_TYPES_GUN && bact->_bact_type != BACT_TYPES_ROBO )
            {
                bact->_fly_dir_length = v73.length();

                if ( bact->_fly_dir_length > 0.001 )
                {
                    bact->_fly_dir = v73 / bact->_fly_dir_length;
                }
            }
            else
            {
                bact->_fly_dir = (bact->_position - bact->_old_pos) / (dtime * 6.0);

                bact->_fly_dir_length = bact->_fly_dir.length();
                if ( bact->_fly_dir_length > 0.001 )
                {
                    bact->_fly_dir = bact->_fly_dir / bact->_fly_dir_length;
                }

                if ( bact->_bact_type == BACT_TYPES_ROBO )
                {
                    bact->_fly_dir.z = 0;
                    bact->_fly_dir.x = 0;
                }

                bact->_netDSpeed = vec3d(0.0, 0.0, 0.0);
            }

            if ( dat->data[id].specialinfo & vhcldata::SI_DSETTED )
            {
                bact->_old_pos = bact->_position;
                bact->_fly_dir_length = 0;
            }

            vec3d rot;
            rot.x = dat->data[id].roll * C_2PI_127;
            rot.y = dat->data[id].pitch * C_2PI_127;
            rot.z = dat->data[id].yaw * C_2PI_127;

            mat3x3 out = mat3x3::Euler(rot);

            bact->_netDRot = (out - bact->_netRotation) / dtime;

            bact->_rotation = out;
            bact->_netRotation = out;

            if ( dat->data[id].specialinfo & vhcldata::SI_NORENDER )
                bact->_status_flg |= BACT_STFLAG_NORENDER;

            if ( dat->data[id].specialinfo & vhcldata::SI_LAND )
                bact->_status_flg |= BACT_STFLAG_LAND;
            else
                bact->_status_flg &= ~BACT_STFLAG_LAND;

            bact->_lastFrmStamp = timestamp;

            if ( bact->_status_flg & BACT_STFLAG_LAND )
            {
                ypaworld_arg136 v69;
                v69.stPos = bact->_position;
                v69.vect = bact->_rotation.AxisY() * 200.0;
                v69.flags = 0;

                ypaworld_func136(&v69);

                if ( v69.isect )
                    bact->_position = v69.isectPos - bact->_rotation.AxisY() * bact->_overeof;
            }

            bact->_energy = dat->data[id].energy;
        }
    }
}

void NC_STACK_ypaworld::yw_processVhclDataMsgs(uamessage_vhclData *msg, NC_STACK_ypabact *host_node)
{
    uamessage_vhclDataI *datI = NULL;
    uamessage_vhclDataE *datE = NULL;

    if (_netInterpolate)
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
        NC_STACK_ypabact *bact = NULL;

        uint32_t ident = 0;

        if (datI)
            ident = datI->data[i].ident;
        else
            ident = datE->data[i].ident;

        bact = NetGetBactByID(host_node, ident);

        if ( !bact )
            log_netlog("+++ EVD: Haven't found vehicle ident %d  from owner %d (%dsec)\n", ident, host_node->_owner, _timeStamp / 1000);
        else
        {
            if ( _netInterpolate )
                yw_netApplyVhclDataI(bact, datI, i, _timeStamp);
            else
                yw_netApplyVhclDataE(bact, datE, i, _timeStamp);
        }
    }
}

int yw_netGetUnitsCount(NC_STACK_ypabact *host)
{
    int count = 1 + host->_kidList.size();
    
    for ( NC_STACK_ypabact* &comm : host->_kidList )
        count += comm->_kidList.size();
    
    return count;
}



NC_STACK_ypabact * yw_netGetMissileByID(NC_STACK_ypabact *host, uint32_t id)
{
    for (NC_STACK_ypamissile* &miss : host->_missiles_list)
    {
        if ( miss->_gid == id )
            return miss;
    }

    for ( NC_STACK_ypabact* &comm : host->_kidList )
    {
        for (NC_STACK_ypamissile* &miss : comm->_missiles_list)
        {
            if ( miss->_gid == id )
                return miss;
        }

        for ( NC_STACK_ypabact* &unit : comm->_kidList )
        {
            for (NC_STACK_ypamissile* &miss : unit->_missiles_list)
            {
                if ( miss->_gid == id )
                    return miss;
            }
        }
    }

    yw_netReportError(host, id);

    return NULL;
}

NC_STACK_ypabact * yw_netGetMissileOfBact(NC_STACK_ypabact *host, uint32_t mslId, uint32_t bactId)
{
    if ( bactId == host->_gid )
    {
        for (NC_STACK_ypamissile* &miss : host->_missiles_list)
        {
            if ( mslId == miss->_gid )
                return miss;
        }
    }
    else
    {
        for (NC_STACK_ypabact* &comm : host->_kidList)
        {
            if ( bactId == comm->_gid )
            {
                for (NC_STACK_ypamissile* &miss : comm->_missiles_list)
                {
                    if ( mslId == miss->_gid )
                        return miss;
                }

                goto yw_netGetMissileOfBact_NOTFOUND;
            }

            for (NC_STACK_ypabact* &unit : comm->_kidList )
            {
                if ( unit->_gid == bactId)
                {
                    for (NC_STACK_ypamissile* &miss : unit->_missiles_list)
                    {
                        if ( mslId == miss->_gid )
                            return miss;
                    }

                    goto yw_netGetMissileOfBact_NOTFOUND;
                }
            }
        }
    }

yw_netGetMissileOfBact_NOTFOUND:

    yw_netReportError(host, mslId);

    return NULL;
}

void yw_netAddVhclUpdData(vhclUpdData *dat, uint8_t type, NC_STACK_ypabact *bact)
{
    dat->type = type;
    dat->pos = bact->_position;
    dat->status = bact->_status;
    dat->status_flg = bact->_status_flg;
    dat->gid = bact->_gid;
    dat->vhclID = bact->_vehicleID;
    dat->engy = bact->_energy;

    vec3d out = bact->_rotation.GetEuler();

    dat->rot_x = out.x * C_127_2PI;
    dat->rot_y = out.y * C_127_2PI;
    dat->rot_z = out.z * C_127_2PI;
}

void yw_netUpdDataVhcl(vhclUpdData *dat, NC_STACK_ypabact *bact, char owner, NC_STACK_yparobo *host_station)
{
    bact->_owner = owner;

    bact->_rotation = mat3x3::Euler( vec3d(dat->rot_x, dat->rot_y, dat->rot_z) * C_2PI_127 );

    bact->_host_station = host_station;
    bact->_gid = dat->gid;
    bact->_energy = dat->engy;
    bact->_status_flg = dat->status_flg;
    bact->_status = dat->status;

    setState_msg sstt;

    if ( bact->_status_flg & BACT_STFLAG_DEATH2 )
    {
        sstt.newStatus = BACT_STATUS_NOPE;
        sstt.unsetFlags = 0;
        sstt.setFlags = BACT_STFLAG_DEATH2;
    }
    else if ( bact->_status_flg & BACT_STFLAG_FIRE )
    {
        sstt.newStatus = BACT_STATUS_NOPE;
        sstt.unsetFlags = 0;
        sstt.setFlags = BACT_STFLAG_FIRE;
    }
    else
    {
        sstt.unsetFlags = 0;
        sstt.setFlags = 0;
        sstt.newStatus = dat->status;
    }

    bact->SetStateInternal(&sstt);
}


void yw_netSendUpdate(NC_STACK_ypaworld *yw, uint8_t owner, const char *recvID)
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

    NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(owner);

    if ( !bhost )
    {
        log_netlog("\n+++ UPD:No Robo found for owner %d/receiver %s\n", owner, recvID);
        return;
    }

    int numb = 1;

    yw_netAddVhclUpdData(updinf.data, 1, bhost);

    for(NC_STACK_ypamissile* &miss : bhost->_missiles_list )
    {
        yw_netAddVhclUpdData(&updinf.data[numb], 4, miss);

        numb++;

        if ( numb >= 1023 )
            break;
    }

    for(NC_STACK_ypabact* &comnd : bhost->_kidList)
    {
        bool isrobogun = false;

        if ( comnd->_bact_type == BACT_TYPES_GUN )
        {
            NC_STACK_ypagun *gn = dynamic_cast<NC_STACK_ypagun *>(comnd);
            if ( gn )
                isrobogun = gn->IsRoboGun();
        }

        if ( isrobogun )
            yw_netAddVhclUpdData(&updinf.data[numb], 5, comnd);
        else
            yw_netAddVhclUpdData(&updinf.data[numb], 2, comnd);

        numb++;

        if ( numb >= 1023 )
            break;

        for(NC_STACK_ypamissile* &miss : comnd->_missiles_list)
        {
            yw_netAddVhclUpdData(&updinf.data[numb], 4, miss);

            numb++;

            if ( numb >= 1023 )
                break;
        }

        for(NC_STACK_ypabact* &slv : comnd->_kidList)
        {
            yw_netAddVhclUpdData(&updinf.data[numb], 3, slv);

            numb++;

            if ( numb >= 1023 )
                break;

            for(NC_STACK_ypamissile* &miss : slv->_missiles_list)
            {
                yw_netAddVhclUpdData(&updinf.data[numb], 4, miss);

                numb++;

                if ( numb >= 1023 )
                    break;
            }
        }
    }

    updinf.data[numb].type = 0;
    numb++;

    updinf.owner = owner;
    updinf.sz = ((char *)&updinf.data[numb] - (char *)&updinf);

    updinf.msgID = UAMSG_UPDATE;
    updinf.num = numb;

    if ( yw->NetSendMessage(&updinf, updinf.sz, recvID, true) )
        log_netlog("\n+++ UPD: Ok, sent Update from owner %d to reciever %s\n", owner, recvID);
    else
        log_netlog("\n+++ UPD: Send-Error. Hmmmm...\n");
}

bool yw_netRecvUpdate(NC_STACK_ypaworld *yw, uamessage_update *msg, int owner)
{
    NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(owner);

    if ( bhost )
    {
        yw->NetRemove(bhost);

        int id = 0;

        NC_STACK_yparobo *currHost = NULL;
        NC_STACK_ypabact *currComm = NULL;
        World::RefBactList *hostList = NULL;
        World::RefBactList::iterator hostLstIt;
        NC_STACK_ypabact *lastBct = NULL;

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

                NC_STACK_ypabact *tmp = yw->ypaworld_func146(&newVhcl);

                currHost = dynamic_cast<NC_STACK_yparobo *>(tmp);

                if ( !currHost )
                {
                    log_netlog("RESTORE: Unable to create robo\n");
                    return false;
                }

                lastBct = currHost;

                hostList = &currHost->_kidList;
                
                hostLstIt = hostList->begin(); //CheckIT may be it must iterate to first robogun?
                
                if ( hostLstIt == hostList->end() )
                    hostList = NULL;

                yw_netUpdDataVhcl(dat, currHost, owner, NULL);

                yw->ypaworld_func134(currHost);
            }
            break;

            case 5:
            {
                if ( !hostList || hostLstIt == hostList->end())
                    break;

                yw_netUpdDataVhcl(dat, *hostLstIt, owner, currHost);

                //Find next host gun
                for (; hostLstIt != hostList->end(); hostLstIt++)
                {
                    NC_STACK_ypabact *bct = *hostLstIt;
                    if ( bct->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gn = dynamic_cast<NC_STACK_ypagun *>(bct);

                        if (gn && gn->IsRoboGun())
                            break;
                    }
                }

                if ( hostLstIt == hostList->end() )
                    hostList = NULL;
            }
            break;

            case 2:
            {
                newVhcl.pos = dat->pos;
                newVhcl.vehicle_id = dat->vhclID;
                currComm = yw->ypaworld_func146(&newVhcl);

                if ( !currComm )
                {
                    log_netlog("RESTORE: Unable to create cmdr\n");
                    return false;
                }

                lastBct = currComm;

                yw_netUpdDataVhcl(dat, currComm, owner, currHost);

                currHost->AddSubject(currComm);
            }
            break;

            case 3:
            {
                newVhcl.pos = dat->pos;
                newVhcl.vehicle_id = dat->vhclID;

                NC_STACK_ypabact *tmp = yw->ypaworld_func146(&newVhcl);
                if ( !tmp )
                {
                    log_netlog("RESTORE: Unable to create slave\n");
                    return false;
                }

                lastBct = tmp;

                yw_netUpdDataVhcl(dat, tmp, owner, currHost);

                currComm->AddSubject(tmp);
            }
            break;

            case 4:
            {
                newMiss.pos = dat->pos;
                newMiss.vehicle_id = dat->vhclID;
                NC_STACK_ypamissile *tmp = yw->ypaworld_func147(&newMiss);

                if ( !tmp )
                {
                    log_netlog("RESTORE: Unable to create weapon\n");
                    return false;
                }

                yw_netUpdDataVhcl(dat, tmp, owner, currHost);

                tmp->_kidRef.Detach();
                tmp->_parent = NULL;

                lastBct->_missiles_list.push_back(tmp);

                tmp->SetLauncherBact(lastBct);
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

NC_STACK_ypabact * yw_netFindReorderUnit(NC_STACK_ypabact *bact_host, uint32_t ID)
{
    if ( ID == bact_host->_gid )
        return bact_host;

    for ( NC_STACK_ypabact* &comm : bact_host->_kidList )
    {
        if ( ID == comm->_gid )
            return comm;

        for ( NC_STACK_ypabact* &unit : comm->_kidList )
        {
            if ( ID == unit->_gid )
                return unit;

            for ( NC_STACK_ypabact* &uniunit : unit->_kidList )
            {
                if ( ID == uniunit->_gid )
                    return uniunit;
            }
        }
    }

    yw_netReportError(bact_host, ID);
    return NULL;
}

size_t yw_handleNormMsg(NC_STACK_ypaworld *yw, windp_recvMsg *msg, std::string *err)
{
    uamessage_base *bMsg = (uamessage_base *)msg->data;
    uint32_t msgID = bMsg->msgID;
    uint8_t owner = bMsg->owner;
    uint32_t tstamp = bMsg->tstamp;
    uint32_t msgcnt = bMsg->msgCnt;

    int tv481 = 0;

    if (owner > 0 && owner < 8)
    {
        UserData::TNetPlayerData &pl = yw->_GameShell->netPlayers[owner];
        pl.LastMessageTime = yw->_updateMessage.gTime;

        if ( msgID != UAMSG_VHCLENERGY && msgID != UAMSG_ENDPLASMA && msg->cast == 2 )
        {
            if ( tstamp >= pl.TimeStamp )
            {
                if ( tstamp - pl.TimeStamp > 6000 )
                    log_netlog(" Message %d from owner %d at %d sec is too fast...\n", msgID, owner, tstamp / 1000);

                pl.TimeStamp = tstamp;
                tv481 = 0;
            }
            else
            {
                tv481 = 1;
                log_netlog("Warning, Msg %d from owner %d is too old!\n", msgID, owner);
                log_netlog("         old: at %d sec  new: at %d sec\n", pl.TimeStamp, tstamp);
            }

            if ( msgcnt == pl.MessageCount )
            {
                pl.MessageCount++;
            }
            else
            {
                if ( msgcnt <= pl.MessageCount )
                {
                    log_netlog("HMM !!!  Late message received with count %d\n", msgcnt);
                    log_netlog("         from owner %d, id %d at time %d\n", owner, msgID, tstamp / 1000);
                }
                else
                {
                    log_netlog("PANIC!!! Msg lost! Last msg has count %d\n", pl.MessageCount - 1);
                    log_netlog("         Msg now (%d) has count %d\n", msgID, msgcnt);
                    log_netlog("         From Owner %d at time %d\n\n", owner, tstamp / 1000);
                }
                pl.MessageCount = msgcnt + 1;
            }
        }
    }
    else
    {
        if ( msgID != UAMSG_FRACTION && msgID != UAMSG_MESSAGE && msgID != UAMSG_WELCOME && msgID != UAMSG_READY && msgID != UAMSG_LOAD )
            log_netlog("Warning, no or false owner (%d) specified for msg %d\n", owner, msgID);
        tv481 = 0;
    }
    
    UserData::TNetPlayerLobbyData *plr = NULL;

    for(UserData::TNetPlayerLobbyData &pl : yw->_GameShell->lobbyPlayers)
    {
        if (StriCmp(pl.Name, msg->senderName) == 0)
        {
            plr = &pl;
            
            if (pl.UpdateCountDown)
            {
                pl.UpdateCountDown -= yw->_updateMessage.frameTime;

                if (pl.UpdateCountDown)
                    pl.UpdateCountDown = 0;
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

        if (yw->_GameShell->netPlayers[owner].DestroyFlags)
            break;

        uamessage_load *ldMsg = (uamessage_load *)msg->data;
        yw->_GameShell->envAction.action = EnvAction::ACTION_NETPLAY;
        yw->_GameShell->envAction.params[0] = ldMsg->level;
        yw->_GameShell->envAction.params[1] = ldMsg->level;

        yw->_isNetGame = true;
        yw->_GameShell->yw_NetPrintStartInfo();
    }
    break;

    case UAMSG_NEWVHCL:
    {
        szmsg = sizeof(uamessage_newVhcl);
        uamessage_newVhcl *nvMsg = (uamessage_newVhcl *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        if ( plr && plr->UpdateCountDown )
            break;

        ypaworld_arg146 newVhcl;
        newVhcl.pos = nvMsg->pos;
        newVhcl.vehicle_id = nvMsg->type;

        NC_STACK_ypabact *bacto = yw->ypaworld_func146(&newVhcl);

        if ( !bacto )
            break;

        setState_msg sstate;
        sstate.newStatus = BACT_STATUS_CREATE;
        sstate.setFlags = 0;
        sstate.unsetFlags = 0;

        bacto->SetStateInternal(&sstate);

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(nvMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NV: Robo Owner %d not found (%ds)\n", nvMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        if ( nvMsg->kind == 0 )
        {
            yw->ypaworld_func134(bacto);
        }
        else if ( nvMsg->kind == 1 )
        {
            host_node->AddSubject(bacto);
        }
        else if ( nvMsg->kind == 2 )
        {
            NC_STACK_ypabact *lstobj = NULL;

            for( NC_STACK_ypabact* &ob : host_node->_kidList )
            {
                if ( nvMsg->parent == ob->_gid )
                {
                    lstobj = ob;
                    break;
                }
            }

            if ( !lstobj )
            {
                log_netlog("NV: No master for created shadow object!\n");
                yw->ypaworld_func144(bacto);
                *err = msg->senderName;

                break;
            }

            lstobj->AddSubject(bacto);
        }

        bacto->_owner = nvMsg->owner;
        bacto->_gid = nvMsg->id;
        bacto->_commandID = nvMsg->commandID;
        bacto->_lastFrmStamp = yw->_timeStamp;
        bacto->_host_station = dynamic_cast<NC_STACK_yparobo *>(host_node);

        yw->HistoryAktCreate( bacto );
        netDebug_AddCreated(host_node->_owner, nvMsg->id);
        break;
    }
    break;

    case UAMSG_DESTROYVHCL:
    {
        szmsg = sizeof(uamessage_destroyVhcl);
        uamessage_destroyVhcl *dvMsg = (uamessage_destroyVhcl *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(dvMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ DV:Havent found robo with owner %d (%ds)\n", dvMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *bctt = NetGetBactByID(host_node, dvMsg->id);

        if ( !bctt )
        {
            log_netlog("\n+++ DV: Havent found vehicle %d (%ds)\n", dvMsg->id, yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        if ( !(bctt->_status_flg & BACT_STFLAG_DEATH1) )
        {
            log_netlog("+++ DV: Release a non-logic-dead vehicle %d! (%ds)\n", dvMsg->id, yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        int vnumb = yw_netGetUnitsCount(host_node);

        while( !bctt->_kidList.empty() )
        {
            yw->ypaworld_func144(bctt->_kidList.front());
            log_netlog("+++ DV: Released vehicle with slave! (%ds)\n", yw->_timeStamp / 1000);
            *err = msg->senderName;
        }

        while(bctt->_missiles_list.begin() != bctt->_missiles_list.end())
        {
            NC_STACK_ypamissile * misl = bctt->_missiles_list.front();
            bctt->_missiles_list.pop_front(); //CHECKIT? why only first missile detach and others will be processed by NetReleaseMissiles

            misl->_parent = NULL;

            yw->NetReleaseMissiles(bctt);

            log_netlog("+++ DV: Released vehicle with weapons! (%ds)\n", yw->_timeStamp / 1000);
            *err = msg->senderName;
        }

        yw->ypaworld_func144(bctt);

        netDebug_addDestroyed(host_node->_owner, dvMsg->id);

        if ( dvMsg->type != BACT_TYPES_MISSLE && yw_netGetUnitsCount(host_node) != (vnumb - 1) )
        {
            log_netlog("\n+++ DV: Vehiclecount changed more than 1! (%ds)\n", yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }
    }
    break;

    case UAMSG_NEWLEADER:
    {
        szmsg = sizeof(uamessage_newLeader);
        uamessage_newLeader *nlMsg = (uamessage_newLeader *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(nlMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NC:Havent found robo with owner %d (%ds)\n", nlMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        int unitscnt = yw_netGetUnitsCount(host_node);

        NC_STACK_ypabact *oldLeader = NetGetBactByID(host_node, nlMsg->id);

        if ( !oldLeader )
        {
            log_netlog("\n+++ NC: Havent found vehicle %d (%ds)\n", nlMsg->id, yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        if ( nlMsg->newLeader )
        {
            NC_STACK_ypabact *newLeader = NetGetBactByID(host_node, nlMsg->newLeader);

            if ( newLeader )
            {
                host_node->AddSubject(newLeader);

                while (!oldLeader->_kidList.empty())
                    newLeader->AddSubject( oldLeader->_kidList.front() );

                newLeader->AddSubject( oldLeader );
            }
        }

        if ( yw_netGetUnitsCount(host_node) != unitscnt )
        {
            log_netlog("\n+++ NC: Vehiclecount changed! (%ds)\n", yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }
    }
    break;

    case UAMSG_NEWWEAPON:
    {
        szmsg = sizeof(uamessage_newWeapon);
        uamessage_newWeapon *nwMsg = (uamessage_newWeapon *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(nwMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NW: Havent found robo with owner %d (%ds)\n", nwMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        ypaworld_arg146 newWpn;
        newWpn.pos = nwMsg->pos;
        newWpn.vehicle_id = nwMsg->type;

        NC_STACK_ypamissile *weapo = yw->ypaworld_func147(&newWpn);

        if ( !weapo )
        {
            log_netlog("\n+++ NW: Was not able to create weapon %d for owner %d\n", nwMsg->type, nwMsg->owner);
            break;
        }

        NC_STACK_ypabact *weapLauncher = NetGetBactByID(host_node, nwMsg->launcher);
        if ( !weapLauncher )
        {
            log_netlog("\n+++ NW: Havent found vehicle %d (%ds)\n", nwMsg->id, yw->_timeStamp / 1000);
            yw->ypaworld_func144(weapo);
            *err = msg->senderName;
            break;
        }

        weapo->_kidRef.Detach();
        weapo->_parent = NULL;

        weapLauncher->_missiles_list.push_back(weapo);

        weapo->SetLauncherBact(weapLauncher);

        weapo->_owner = nwMsg->owner;
        weapo->_gid = nwMsg->id;
        weapo->_lastFrmStamp = yw->_timeStamp;

        netDebug_AddCreated(host_node->_owner, nwMsg->id);

        weapo->_fly_dir_length = nwMsg->dir.length();
        weapo->_fly_dir = nwMsg->dir;

        if ( weapo->_fly_dir_length > 0.001 )
        {
            weapo->_fly_dir /= weapo->_fly_dir_length;
        }

        weapo->AlignMissile();

        int wpnType = weapo->GetMissileType();

        if ( wpnType == 3 )
        {
            setTarget_msg stargt;
            stargt.tgt_pos = nwMsg->targetPos;
            stargt.priority = 0;
            stargt.tgt_type = nwMsg->targetType;

            if ( stargt.tgt_type == BACT_TGT_TYPE_UNIT )
            {
                NC_STACK_ypabact *ownerhost = yw->yw_getHostByOwner(nwMsg->targetOwner);

                if ( !ownerhost )
                {
                    log_netlog("\n+++ NW: false targetowner %d for weapon\n", nwMsg->targetOwner);
                    *err = msg->senderName;
                    break;
                }

                stargt.tgt.pbact = NetGetBactByID(ownerhost, nwMsg->target);

                if ( !stargt.tgt.pbact )
                    stargt.tgt_type = BACT_TGT_TYPE_NONE;
            }

            weapo->SetTarget(&stargt);

            if ( nwMsg->targetType == BACT_TGT_TYPE_CELL )
                weapo->_primTpos.y = nwMsg->targetPos.y;
        }

        if ( wpnType == 2 )
        {
            weapo->_primTtype = BACT_TGT_TYPE_DRCT;
            weapo->_target_dir = weapo->_fly_dir;
        }

        weapo->_host_station = dynamic_cast<NC_STACK_yparobo*>(host_node);

        if ( nwMsg->targetType == BACT_TGT_TYPE_UNIT )
            break;

        int weapLifeTime = yw->GetWeaponsProtos().at(nwMsg->type).life_time_nt;

        if ( weapLifeTime )
            weapo->SetLifeTime(weapLifeTime);

    }
    break;

    case UAMSG_SETSTATE:
    {
        szmsg = sizeof(uamessage_setState);
        uamessage_setState *ssMsg = (uamessage_setState *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(ssMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ SS: Havent found robo with owner %d (%ds)\n", ssMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *bct = NetGetBactByID(host_node, ssMsg->id);
        if ( !bct )
        {
            log_netlog("\n+++ SS: Havent found vehicle %d (%ds)\n", ssMsg->id, yw->_timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        setState_msg ststate;
        ststate.newStatus = ssMsg->newStatus;
        ststate.setFlags = ssMsg->setFlags;
        ststate.unsetFlags = ssMsg->unsetFlags;
        bct->SetStateInternal(&ststate);
    }
    break;

    case UAMSG_VHCLDATA_I:
    {
        uamessage_vhclData *vdMsg = (uamessage_vhclData *)msg->data;
        szmsg = msg->_data.size();

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) || tv481 )
            break;

        if ( yw->_netInterpolate )
        {
            NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(vdMsg->owner);

            if ( host_node )
                yw->yw_processVhclDataMsgs(vdMsg, host_node);
            else
                log_netlog("\n+++ VD: Havent found robo with owner %d (%ds)\n", vdMsg->owner, yw->_timeStamp / 1000);
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
        szmsg = msg->_data.size();

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) || tv481 )
            break;

        if ( yw->_netInterpolate )
        {
            log_netlog("\n+++ VD: An interpolate-Program gets extrapolate data!!!\n");
        }
        else
        {
            NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(vdMsg->owner);

            if ( host_node )
                yw->yw_processVhclDataMsgs(vdMsg, host_node);
            else
                log_netlog("\n+++ VD: Havent found robo with owner %d (%ds)\n", vdMsg->owner, yw->_timeStamp / 1000);
        }
    }
    break;

    case UAMSG_DEAD:
    {
        szmsg = sizeof(uamessage_dead);
        uamessage_dead *ddMsg = (uamessage_dead *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(ddMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ D: Havent found robo with owner %d (%ds)\n", ddMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *fndBact = NULL;

        if ( ddMsg->classID == BACT_TYPES_MISSLE )
        {
            fndBact = yw_netGetMissileByID(host_node, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found weapon %d (%ds)\n", ddMsg->id, yw->_timeStamp / 1000);
                *err = msg->senderName;
                break;
            }

        }
        else
        {
            fndBact = NetGetBactByID(host_node, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found vehicle %d (%ds)\n", ddMsg->id, yw->_timeStamp / 1000);
                *err = msg->senderName;
                break;
            }
        }

        for ( World::RefBactList::iterator it = fndBact->_kidList.begin(); it != fndBact->_kidList.end(); )
        {
            NC_STACK_ypabact *nd = *it;
            it++;
            
            if ( nd->_status == BACT_STATUS_DEAD )
            {
                if ( fndBact->_parent )
                    fndBact->_parent->AddSubject(nd);
                else
                    yw->ypaworld_func134(nd);
                    

                nd->_status_flg |= BACT_STFLAG_NOMSG;
            }
        }

        if ( ddMsg->newParent == 0 && fndBact->_kidList.empty() )
        {
            NC_STACK_ypabact *hst = yw->getYW_userHostStation();

            int sft = 0;
            if ( fndBact->_bact_type == BACT_TYPES_GUN )
                sft = fndBact->_host_station->getROBO_epSafety();

            if ( hst->_owner == ddMsg->killerOwner && fndBact->IsParentMyRobo() && !sft && !(fndBact->_status_flg & BACT_STFLAG_NOMSG) )
            {
                robo_arg134 rarg134;
                rarg134.unit = fndBact->_killer;
                rarg134.field_4 = 5;
                rarg134.field_8 = sft;
                rarg134.field_C = sft;
                rarg134.field_10 = sft;
                rarg134.field_14 = 36;

                NC_STACK_yparobo *hstt = dynamic_cast<NC_STACK_yparobo *>(host_node);
                if (hstt)
                    hstt->placeMessage(&rarg134);
            }
        }
        else
        {
            NC_STACK_ypabact *np = NULL;

            if ( ddMsg->newParent )
                np = NetGetBactByID(host_node, ddMsg->newParent);

            if ( np != NULL )
            {
                host_node->AddSubject(np);

                while (!fndBact->_kidList.empty())
                {
                    np->AddSubject(fndBact->_kidList.front());
                }
            }
            else
            {
                if ( ddMsg->newParent )
                    log_netlog("\n+++ D: No master (%d) found for my slaves (%ds)\n", ddMsg->newParent, yw->_timeStamp / 1000);
                else
                    log_netlog("\n+++ D: No master given for my slaves (%ds)\n", yw->_timeStamp / 1000);

                *err = msg->senderName;
            }
        }

        if ( fndBact->_parent )
        {
            while ( !fndBact->_missiles_list.empty() )
            {
                NC_STACK_ypamissile* miss = fndBact->_missiles_list.front();
                fndBact->_missiles_list.pop_front();
                
                fndBact->_parent->_missiles_list.push_back(miss);
                miss->SetLauncherBact(fndBact->_parent);
            }
        }
        else
        {
            yw->NetReleaseMissiles(fndBact);
        }
        

        fndBact->_status = BACT_STATUS_DEAD;

        if ( ddMsg->landed )
        {
            setState_msg sst;
            sst.unsetFlags = 0;
            sst.newStatus = BACT_STATUS_NOPE;
            sst.setFlags = BACT_STFLAG_DEATH2;
            fndBact->SetStateInternal(&sst);
        }

        fndBact->CleanAttackersTarget();

        for ( NC_STACK_ypabact* &k : fndBact->_kidList )
            log_netlog("+++ D: I am dead, but I have slave ident %d class %d with state %d (%ds)\n", k->_gid, k->_bact_type, k->_status, yw->_timeStamp / 1000);

        fndBact->_status_flg |= BACT_STFLAG_DEATH1;

        if ( ddMsg->killerOwner )
        {
            NC_STACK_ypabact *v134 = yw->yw_getHostByOwner(ddMsg->killerOwner);

            if ( v134 )
                fndBact->_killer = NetGetBactByID(v134, ddMsg->killer);
            else
                fndBact->_killer = NULL;
        }

        yw->HistoryAktKill(fndBact);

        fndBact->_status_flg |= BACT_STFLAG_DEATH1;
    }
    break;

    case UAMSG_VHCLENERGY:
    {
        szmsg = sizeof(uamessage_vhclEnergy);
        uamessage_vhclEnergy *veMsg = (uamessage_vhclEnergy *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(veMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", veMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        if ( host_node != yw->_userRobo )
            break;

        NC_STACK_ypabact *fbact = NetGetBactByID(host_node, veMsg->id);

        if ( !fbact )
            break;

        fbact->_energy += veMsg->energy;
        fbact->_killer_owner = veMsg->killerOwner;

        if ( fbact->_energy >= 0 )
            break;

        if ( veMsg->killer )
        {
            NC_STACK_ypabact *khost = yw->yw_getHostByOwner(veMsg->killerOwner);

            if ( !khost )
            {
                log_netlog("\n+++ VE: No robo with owner %d found\n", veMsg->killerOwner);
                break;
            }

            NC_STACK_ypabact *kbact = NetGetBactByID(khost, veMsg->killer);

            fbact->_killer = kbact;

            if ( !kbact )
                log_netlog("\n+++ VE: False killer %d (owner %d) given\n", veMsg->killer, veMsg->killerOwner);
        }
        else
        {
            fbact->_killer = NULL;
        }

        setState_msg v410;
        v410.newStatus = BACT_STATUS_DEAD;
        v410.setFlags = 0;
        v410.unsetFlags = 0;

        fbact->SetState(&v410);
    }
    break;

    case UAMSG_SECTORENERGY:
    {
        szmsg = sizeof(uamessage_sectorEnergy);
        uamessage_sectorEnergy *seMsg = (uamessage_sectorEnergy *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(seMsg->owner);

        yw_arg129 arg129;
        if ( host_node )
        {
            arg129.unit = NetGetBactByID(host_node, seMsg->whoHit);
        }
        else
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", seMsg->owner, yw->_timeStamp / 1000);
            arg129.unit = 0;
        }

        arg129.pos = seMsg->pos;
        arg129.field_10 = seMsg->energy;
        arg129.OwnerID = seMsg->sectOwner;

        yw->ypaworld_func129(&arg129);
    }
    break;

    case UAMSG_STARTBUILD:
    {
        szmsg = sizeof(uamessage_startBuild);
        uamessage_startBuild *sbMsg = (uamessage_startBuild *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        if ( yw->yw_getHostByOwner(sbMsg->owner) )
        {
            ypaworld_arg148 arg148;
            arg148.owner = sbMsg->owner;
            arg148.blg_ID = sbMsg->bproto;
            arg148.field_C = 0;
            arg148.CellId = {sbMsg->sec_x, sbMsg->sec_y};
            arg148.field_18 = 0;

            yw->ypaworld_func148(&arg148);
        }
        else
        {
            log_netlog("\n+++ SB: Havent found robo with owner %d (%ds)\n", sbMsg->owner, yw->_timeStamp / 1000);
        }
    }
    break;

    case UAMSG_BUILDINGVHCL:
    {
        uamessage_bldVhcl *bvMsg = (uamessage_bldVhcl *)msg->data;
        szmsg = sizeof(uamessage_bldVhcl);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) || !bvMsg->vhcl[0].protoID )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(bvMsg->owner);

        if ( bhost )
        {
            ypaworld_arg146 newVhcl;
            newVhcl.vehicle_id = bvMsg->vhcl[0].protoID;
            newVhcl.pos = bvMsg->vhcl[0].pos;

            NC_STACK_ypabact *kmnd = yw->ypaworld_func146(&newVhcl);
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

                kmnd->_gid = bvMsg->vhcl[0].id;
                kmnd->_owner = bvMsg->owner;
                kmnd->_host_station = dynamic_cast<NC_STACK_yparobo *>(bhost);

                bhost->AddSubject(kmnd);

                netDebug_AddCreated(bhost->_owner, kmnd->_gid);

                int idid = 0;
                while (bvMsg->vhcl[idid].protoID != 0)
                {
                    newVhcl.vehicle_id = bvMsg->vhcl[idid].protoID;
                    newVhcl.pos = bvMsg->vhcl[idid].pos;

                    NC_STACK_ypabact *slv = yw->ypaworld_func146(&newVhcl);

                    if ( slv )
                    {
                        NC_STACK_ypagun *slv_gn = dynamic_cast<NC_STACK_ypagun *>(slv);

                        if (slv_gn)
                            slv_gn->ypagun_func128(bvMsg->vhcl[idid].base, false);

                        ssms.newStatus = BACT_STATUS_CREATE;
                        ssms.setFlags = 0;
                        ssms.unsetFlags = 0;
                        slv->SetStateInternal(&ssms);

                        slv->_gid = bvMsg->vhcl[idid].id;
                        slv->_owner = bvMsg->owner;
                        slv->_host_station = dynamic_cast<NC_STACK_yparobo *>(bhost);

                        kmnd->AddSubject(slv);

                        netDebug_AddCreated(bhost->_owner, slv->_gid);
                    }
                    idid++;
                }
            }
        }
        else
            log_netlog("\n+++ BV: Havent found robo with owner %d (%ds)\n", bvMsg->owner, yw->_timeStamp / 1000);
    }
    break;

    case UAMSG_VIEWER:
    {
        szmsg = sizeof(uamessage_viewer);
        uamessage_viewer *vwMsg = (uamessage_viewer *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(vwMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ V: Havent found robo with owner %d (%ds)\n", vwMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *fndbct = NULL;

        if ( vwMsg->classID == BACT_TYPES_MISSLE )
        {
            fndbct = yw_netGetMissileOfBact(bhost, vwMsg->id, vwMsg->launcher);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found weapon %d of rifleman %d (%ds)\n", vwMsg->id, vwMsg->launcher, yw->_timeStamp / 1000);
                *err = msg->senderName;
                break;
            }
        }
        else
        {
            fndbct = NetGetBactByID(bhost, vwMsg->id);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found vehicle %d (%ds)\n", vwMsg->id, yw->_timeStamp / 1000);
                *err = msg->senderName;
                break;
            }
        }

        if ( vwMsg->view )
            fndbct->_status_flg |= BACT_STFLAG_ISVIEW;
        else
            fndbct->_status_flg &= ~BACT_STFLAG_ISVIEW;
    }
    break;

    case UAMSG_SYNCGM:
    {
        szmsg = sizeof(uamessage_syncgame);
        uamessage_syncgame *sgMsg = (uamessage_syncgame *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
        {
            log_netlog("\n+++SG: received sync of a dead player %s\n", msg->senderName.c_str());
            break;
        }

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(sgMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SG: Havent found robo with owner %d (%ds)\n", sgMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        bhost->_gid = sgMsg->hostID;
        int nmb = 0;

        for(NC_STACK_ypabact* &comm : bhost->_kidList)
        {
            bool rbgun = false;
            if ( comm->_bact_type == BACT_TYPES_GUN )
            {
                NC_STACK_ypagun *gno = dynamic_cast<NC_STACK_ypagun *>(comm);
                if (gno)
                    rbgun = gno->IsRoboGun();
            }

            if ( rbgun )
            {
                comm->_gid = sgMsg->gun[nmb];;
                nmb++;
            }
        }

        yw->_GameShell->netPlayers[sgMsg->owner].Ready = true;

        log_netlog("received READY TO PLAY from owner %d\n", sgMsg->owner);
    }
    break;

    case UAMSG_HOSTDIE:
    {
        uamessage_hostDie *hdMsg = (uamessage_hostDie *)msg->data;
        szmsg = sizeof(uamessage_hostDie);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(hdMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", hdMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        int last = bhost->getBACT_yourLastSeconds();
        setState_msg stm;

        for ( NC_STACK_ypabact* &cm : bhost->_kidList )
        {
            for ( NC_STACK_ypabact* &un : cm->_kidList )
            {
                un->CleanAttackersTarget();
                yw->NetReleaseMissiles(un);

                stm.newStatus = BACT_STATUS_CREATE;
                stm.unsetFlags = 0;
                stm.setFlags = 0;

                un->SetStateInternal(&stm);
                un->_status = BACT_STATUS_DEAD;

                SFXEngine::SFXe.sub_424000(&un->_soundcarrier, 3);
                SFXEngine::SFXe.startSound(&un->_soundcarrier, 7);

                un->_soundFlags &= 0xFFFFFFF7;
                un->_soundFlags |= 0x80;

                un->setBACT_yourLastSeconds(last);
                un->_status_flg |= BACT_STFLAG_DEATH1;
            }

            cm->CleanAttackersTarget();
            yw->NetReleaseMissiles(cm);

            stm.newStatus = BACT_STATUS_CREATE;
            stm.unsetFlags = 0;
            stm.setFlags = 0;

            cm->SetStateInternal(&stm);
            cm->_status = BACT_STATUS_DEAD;

            SFXEngine::SFXe.sub_424000(&cm->_soundcarrier, 3);
            SFXEngine::SFXe.startSound(&cm->_soundcarrier, 7);

            cm->_soundFlags &= 0xFFFFFFF7;
            cm->_soundFlags |= 0x80;

            cm->setBACT_yourLastSeconds(last);
            cm->_status_flg |= BACT_STFLAG_DEATH1;
        }

        bhost->CleanAttackersTarget();
        yw->NetReleaseMissiles(bhost);

        bhost->_status_flg |= BACT_STFLAG_DEATH1;

        if ( yw->_GameShell->netPlayerOwner != bhost->_owner )
        {
            int nwee = 0;
            int wee = 0;
            for ( NC_STACK_ypabact* &bnd : yw->_unitsList )
            {
                if ( bnd->_bact_type == BACT_TYPES_ROBO &&
                        yw->_GameShell->netPlayerOwner == bnd->_owner &&
                        bnd->_status != BACT_STATUS_DEAD )
                    wee = 1;

                if ( bnd->_bact_type == BACT_TYPES_ROBO &&
                        yw->_GameShell->netPlayerOwner != bnd->_owner &&
                        bnd->_status != BACT_STATUS_DEAD )
                    nwee = 1;
            }

            if ( wee == 1 && nwee == 0 )
            {
                yw_arg159 arg159;
                arg159.Priority = 10;
                arg159.MsgID = 91;
                arg159.unit = 0;
                yw->ypaworld_func159(&arg159);

                yw->_netEvent.EventType = 1;
                yw->_netEvent.TimeStamp = yw->_timeStamp;
            }
            else
            {
                if ( !hdMsg->killerOwner )
                {
                    yw->_netEvent.EventType = 4;
                    yw->_netEvent.TimeStamp = yw->_timeStamp;

                    yw->_netEvent.PlayerName = msg->senderName;
                }
                else if ( hdMsg->killerOwner == yw->_GameShell->netPlayerOwner )
                {
                    yw->_netEvent.EventType = 2;
                    yw->_netEvent.TimeStamp = yw->_timeStamp;
                    yw->_netEvent.PlayerName = msg->senderName;
                }
                else
                {
                    yw->_netEvent.EventType = 3;
                    yw->_netEvent.TimeStamp = yw->_timeStamp;
                    yw->_netEvent.PlayerName = msg->senderName;

                    yw->_netEvent.Player2Name = yw->_GameShell->netPlayers[hdMsg->killerOwner].Name;
                }

                yw_arg159 arg159;
                arg159.unit = 0;
                arg159.Priority = 50;
                switch ( hdMsg->owner)
                {
                case 3:
                    arg159.MsgID = 90;
                    break;
                case 4:
                    arg159.MsgID = 89;
                    break;
                case 6:
                    arg159.MsgID = 88;
                    break;
                case 1:
                    arg159.MsgID = 87;
                    break;
                case 2:
                case 5:
                    break;
                }
                yw->ypaworld_func159(&arg159);
            }
        }

        if ( hdMsg->killerOwner )
        {
            NC_STACK_ypabact *tmp = yw->yw_getHostByOwner(hdMsg->killerOwner);
            if ( tmp )
                bhost->_killer = NetGetBactByID(tmp, hdMsg->killer);
            else
                bhost->_killer = NULL;
        }
        
        yw->HistoryAktKill(bhost);

        if ( hdMsg->killer && hdMsg->killerOwner == yw->_userRobo->_owner )
        {
            NC_STACK_ypabact *nde = yw->yw_getHostByOwner(hdMsg->killerOwner);

            if ( nde )
            {
                //rarg134.unit = yw_netGetBactByID(nde->bact, hdMsg->killer);
                if ( !NetGetBactByID(nde, hdMsg->killer) )
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
        arg177.bact = bhost;
        yw->ypaworld_func177(&arg177);
    }
    break;

    case UAMSG_MESSAGE:
    {
        uamessage_message *msgMsg = (uamessage_message *)msg->data;
        szmsg = sizeof(uamessage_message);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;


        if ( yw->_netDriver->IsPlayer( msg->senderName ) )
        {
            int nmb = strtol(msgMsg->message, 0, 0);

            if (nmb > 0)
            {
                sub_4D9550(yw, nmb);
            }
            else
            {
                if ( yw->_isNetGameStarted )
                {
                    std::string bff = fmt::sprintf("%s: %s", msg->senderName, msgMsg->message);

                    yw_arg159 arg159;
                    arg159.txt = bff;
                    arg159.MsgID = 93;
                    arg159.Priority = 10;
                    arg159.unit = NULL;

                    yw->ypaworld_func159(&arg159);
                }
                else
                {
                    sub_4D0C24(yw, msg->senderName, msgMsg->message);
                }
            }

            break;
        }
    }
    break;

    case UAMSG_KICK:
    {
        uamessage_kick *kcMsg = (uamessage_kick *)msg->data;
        szmsg = sizeof(uamessage_kick);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->netProblemOwner = kcMsg->owner;

        yw->_GameShell->netProblemName = kcMsg->text;

        if ( !StriCmp(kcMsg->text, yw->_GameShell->netPlayerName) )
        {
            yw->_GameShell->netPlayers[owner].DestroyFlags = 1;
            yw->_GameShell->noSent = true;
            yw->_GameShell->netProblem = 3;
            yw->_GameShell->netProblemCountDown = 10000;
        }
        else
        {
            yw_cleanPlayer(yw, "", kcMsg->owner, 1);
            yw_DestroyPlayer(yw, kcMsg->text);

            if ( yw->_GameShell->netProblem != 3 )
            {
                yw->_GameShell->netProblem = 4;
                yw->_GameShell->netProblemCountDown = 15000;
            }
        }

        if ( !StriCmp(kcMsg->text, yw->_GameShell->netPlayerName) )
        {
            log_netlog(">>> I was kicked off by the host! (time %d)\n", yw->_timeStamp / 1000);
        }
        else
        {
            log_netlog(">>> Host told me he has kicked off %s (time %d)\n", kcMsg->text, yw->_timeStamp / 1000);
        }
    }
    break;

    case UAMSG_UPGRADE:
    {
        uamessage_upgrade *upMsg = (uamessage_upgrade *)msg->data;
        szmsg = sizeof(uamessage_upgrade);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        TMapGem &gemProt = yw->_techUpgrades[upMsg->upgradeID];
        cellArea &cl = yw->SectorAt(gemProt.CellId);

        int vhcl, bld;
        sub_47C1EC(yw, &gemProt, &vhcl, &bld);

        if ( vhcl )
            yw->_vhclProtos[vhcl].disable_enable_bitmask = 0;

        if ( bld )
            yw->_buildProtos[bld].EnableMask = 0;
      
        uint16_t lastVhcl = 0;
        uint16_t lastBuild = 0;

        switch ( yw->_GameShell->netPlayerOwner )
        {
        case 1:
            lastVhcl = gemProt.NwVprotoNum1;
            lastBuild = gemProt.NwBprotoNum1;
            break;
        case 6:
            lastVhcl = gemProt.NwVprotoNum2;
            lastBuild = gemProt.NwBprotoNum2;
            break;
        case 3:
            lastVhcl = gemProt.NwVprotoNum3;
            lastBuild = gemProt.NwBprotoNum3;
            break;
        case 4:
            lastVhcl = gemProt.NwVprotoNum4;
            lastBuild = gemProt.NwBprotoNum4;
            break;
        case 2:
        case 5:
            lastBuild = 0;
            lastVhcl = 0;
            break;
        }

        yw->HistoryEventAdd( World::History::Upgrade(gemProt.CellId.x, gemProt.CellId.y, owner, gemProt.Type, lastVhcl, 0, lastBuild) );

        if ( cl.PurposeType != cellArea::PT_TECHUPGRADE )
        {
            std::string bff = Locale::Text::Feedback(Locale::FEEDBACK_TECHDOWN);
            bff += gemProt.MsgDefault;

            yw_arg159 arg159;
            arg159.Priority = 80;
            arg159.txt = bff;
            arg159.MsgID = 29;
            arg159.unit = NULL;

            yw->ypaworld_func159(&arg159);

        }

        if ( upMsg->enable )
        {
            cl.PurposeType = cellArea::PT_TECHUPGRADE;
            cl.PurposeIndex = upMsg->upgradeID;
        }
        else
        {
            cl.PurposeIndex = 0;
            cl.PurposeType = cellArea::PT_NONE;
        }
    }
    break;

    case UAMSG_FRACTION:
    {
        szmsg = sizeof(uamessage_fraction);
        uamessage_fraction *frMsg = (uamessage_fraction *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->FreeFraction &= ~frMsg->newfrac;
        yw->_GameShell->FreeFraction |= frMsg->freefrac;

        int32_t indx = yw->_netDriver->GetPlayerIndex( msg->senderName );
        if ( indx != -1 )
        {
            yw->_GameShell->lobbyPlayers[indx].NetFraction = frMsg->newfrac;
            break;
        }
    }
    break;

    case UAMSG_WELCOME:
    {
        szmsg = sizeof(uamessage_welcome);
        uamessage_welcome *wmMsg = (uamessage_welcome *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        int32_t indx = yw->_netDriver->GetPlayerIndex( msg->senderName );
        if ( indx != -1 )
        {
            if ( yw->_GameShell->remoteMode )
                yw->_GameShell->FreeFraction |= yw->_GameShell->lobbyPlayers[indx].NetFraction;

            yw->_GameShell->lobbyPlayers[indx].NetFraction = wmMsg->fraction;
            yw->_GameShell->lobbyPlayers[indx].Ready = wmMsg->rdy;
            yw->_GameShell->lobbyPlayers[indx].Welcomed = true;
        }

        yw->_GameShell->FreeFraction &= ~wmMsg->fraction;

        if ( yw->_GameShell->SelectedFraction == wmMsg->fraction )
        {
            if ( yw->_GameShell->FreeFraction & World::OWNER_RESIST_BIT )
            {
                yw->_GameShell->SelectedFraction = World::OWNER_RESIST_BIT;
            }
            else if ( yw->_GameShell->FreeFraction & World::OWNER_GHOR_BIT )
            {
                yw->_GameShell->SelectedFraction = World::OWNER_GHOR_BIT;
            }
            else if ( yw->_GameShell->FreeFraction & World::OWNER_MYKO_BIT )
            {
                yw->_GameShell->SelectedFraction = World::OWNER_MYKO_BIT;
            }
            else if ( yw->_GameShell->FreeFraction & World::OWNER_TAER_BIT )
            {
                yw->_GameShell->SelectedFraction = World::OWNER_TAER_BIT;
            }
        }

        yw->_GameShell->FreeFraction &= ~yw->_GameShell->SelectedFraction;

        uamessage_fraction frMsg;
        frMsg.msgID = UAMSG_FRACTION;
        frMsg.freefrac = 0;
        frMsg.newfrac = yw->_GameShell->SelectedFraction;
        frMsg.owner = yw->_GameShell->netPlayerOwner;

        yw->NetBroadcastMessage(&frMsg, sizeof(frMsg), true);
    }
    break;

    case UAMSG_READY:
    {
        uamessage_ready *rdMsg = (uamessage_ready *)msg->data;
        szmsg = sizeof(uamessage_ready);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        int32_t index = yw->_netDriver->GetPlayerIndex( msg->senderName );
        if ( index != -1 )
        {
            yw->_GameShell->lobbyPlayers[index].Ready = rdMsg->rdy;
            break;
        }
    }
    break;

    case UAMSG_REQUPDATE:
    {
        szmsg = sizeof(uamessage_requpdate);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw_netSendUpdate(yw, yw->_GameShell->netPlayerOwner, msg->senderName.c_str());
    }
    break;

    case UAMSG_UPDATE:
    {
        uamessage_update *updMsg = (uamessage_update *)msg->data;
        szmsg = updMsg->sz;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
        {
            log_netlog("\n+++UPD: got update from DEAD %s (%d)", msg->senderName.c_str(), yw->_timeStamp / 1000);
            plr->UpdateCountDown = 0;
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

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        if ( plr && plr->UpdateCountDown )
            break;

        yw_130arg secInfo;
        secInfo.pos_x = impMsg->pos.x;
        secInfo.pos_z = impMsg->pos.z;

        if ( !yw->GetSectorInfo(&secInfo) )
            break;

        bact_arg83 impls;
        impls.pos = impMsg->pos;
        impls.energ = impMsg->impulse;
        impls.pos2 = impMsg->dir;
        impls.force = impMsg->dir_len;
        impls.mass = impMsg->mass;

        for( NC_STACK_ypabact* &curUnit : secInfo.pcell->unitsList )
        {
            if (curUnit->_bact_type != BACT_TYPES_MISSLE &&
                    curUnit->_bact_type != BACT_TYPES_ROBO &&
                    curUnit->_bact_type != BACT_TYPES_TANK &&
                    curUnit->_bact_type != BACT_TYPES_CAR &&
                    curUnit->_bact_type != BACT_TYPES_GUN &&
                    curUnit->_bact_type != BACT_TYPES_HOVER &&
                    !(curUnit->_status_flg & BACT_STFLAG_DEATH2) &&
                    curUnit->_owner == yw->_userRobo->_owner)
                curUnit->ApplyImpulse(&impls);
        }
    }
    break;

    case UAMSG_LOGMSG:
    {
        szmsg = sizeof(uamessage_logmsg);
        uamessage_logmsg *lgMsg = (uamessage_logmsg *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(lgMsg->senderOwner);
        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", lgMsg->senderOwner, yw->_timeStamp / 1000);
            break;
        }

        if ( bhost != yw->_userRobo )
            break;

        robo_arg134 rarg134;
        if ( lgMsg->sender )
        {
            rarg134.unit = NetGetBactByID(bhost, lgMsg->sender);

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

        NC_STACK_yparobo *rbo = dynamic_cast<NC_STACK_yparobo *>(bhost);
        if (rbo)
            rbo->placeMessage(&rarg134);
    }
    break;

    case UAMSG_REORDER:
    {
        uamessage_reorder *ordMsg = (uamessage_reorder *)msg->data;
        szmsg = ordMsg->sz;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(ordMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ NORG: Havent found robo with owner %d (%ds)\n", ordMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        //yw_netGetUnitsCount(bhost->bact); // Not used

        NC_STACK_ypabact *fnd = yw_netFindReorderUnit(bhost, ordMsg->comm);
        if ( !fnd )
        {
            log_netlog("\n+++ NORG: Commander %d of owner %d not found (%ds)\n", ordMsg->comm, owner, yw->_timeStamp / 1000);
            break;
        }

        fnd->_commandID = ordMsg->commID;
        bhost->AddSubject(fnd);

        for (uint32_t i = 0; i < ordMsg->num; i++)
        {
            NC_STACK_ypabact *fnds = yw_netFindReorderUnit(bhost, ordMsg->units[i]);
            if ( fnds )
            {
                fnd->AddSubject(fnds);
            }
            else
            {
                log_netlog("\n+++ NORG: Slave %d of Owner %d not found (%ds)\n", ordMsg->units[i], owner, yw->_timeStamp / 1000);
            }
        }
    }
    break;

    case UAMSG_LOBBYINIT:
    {
        uamessage_lobbyInit *lobbyMsg = (uamessage_lobbyInit *)msg->data;
        szmsg = sizeof(uamessage_lobbyInit);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->netLevelID = lobbyMsg->lvlID;
        yw->_GameShell->netLevelName = yw->GetLevelName(lobbyMsg->lvlID);

        int32_t index = yw->_netDriver->GetPlayerIndex( lobbyMsg->hostName );
        if ( index != -1 )
        {
            yw->_GameShell->lobbyPlayers[index].Ready = true;
            break;
        }

        TMapRegionInfo &minf = yw->_globalMapRegions.MapRegions[lobbyMsg->lvlID];
        int plCount = yw->_netDriver->GetPlayerCount();

        index = 0;
        
        if ( minf.IsFraction(World::OWNER_RESIST) )
        {
            if ( plCount > 0 )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_RESIST_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_RESIST_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_RESIST_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_RESIST_BIT;
        }

        if ( minf.IsFraction(World::OWNER_GHOR) )
        {
            if ( plCount > 0 )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_GHOR_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_GHOR_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_GHOR_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_GHOR_BIT;
        }

        if ( minf.IsFraction(World::OWNER_MYKO) )
        {
            if ( plCount > 0 )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_MYKO_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_MYKO_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_MYKO_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_MYKO_BIT;
        }

        if ( minf.IsFraction(World::OWNER_TAER) )
        {
            if ( plCount > 0 )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_TAER_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_TAER_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_TAER_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_TAER_BIT;
        }

        yw->SendCRC(yw->_GameShell->netLevelID);
    }
    break;

    case UAMSG_SETLEVEL:
    {
        uamessage_setLevel *stlvlMsg = (uamessage_setLevel *)msg->data;
        szmsg = sizeof(uamessage_setLevel);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->netLevelID = stlvlMsg->lvlID;
        yw->_GameShell->netLevelName = yw->GetLevelName(stlvlMsg->lvlID);

        for (UserData::TNetPlayerLobbyData &pl : yw->_GameShell->lobbyPlayers)
            pl.DataChecksum = 0;

        TMapRegionInfo &minf = yw->_globalMapRegions.MapRegions[stlvlMsg->lvlID];
        
        int plCount = yw->_netDriver->GetPlayerCount();
        int32_t index = 0;
        
        if ( minf.IsFraction(World::OWNER_RESIST) )
        {
            if ( plCount )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_RESIST_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_RESIST_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_RESIST_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_RESIST_BIT;
        }

        if ( minf.IsFraction(World::OWNER_GHOR) )
        {
            if ( plCount )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_GHOR_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_GHOR_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_GHOR_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_GHOR_BIT;
        }

        if ( minf.IsFraction(World::OWNER_MYKO) )
        {
            if ( plCount )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_MYKO_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_MYKO_BIT;

                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_MYKO_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_MYKO_BIT;
        }

        if ( minf.IsFraction(World::OWNER_TAER) )
        {
            if ( plCount )
            {
                yw->_GameShell->FreeFraction &= ~World::OWNER_TAER_BIT;
                yw->_GameShell->lobbyPlayers[index].NetFraction = World::OWNER_TAER_BIT;
                
                if ( index == yw->_netDriver->GetMyIndex() )
                    yw->_GameShell->SelectedFraction = World::OWNER_TAER_BIT;

                plCount--;
                index++;
            }
        }
        else
        {
            yw->_GameShell->FreeFraction &= ~World::OWNER_TAER_BIT;
        }

        yw->SendCRC(yw->_GameShell->netLevelID);
    }
    break;

    case UAMSG_STARTPLASMA:
    {
        szmsg = sizeof(uamessage_startPlasma);
        uamessage_startPlasma *plasmaMsg = (uamessage_startPlasma *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(plasmaMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SP: Havent found robo with owner %d (%ds)\n", plasmaMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *bbact = NetGetBactByID(bhost, plasmaMsg->id);
        if ( !bbact )
        {
            log_netlog("\n+++ SP: vehicle %d doesn't exist\n", plasmaMsg->id);
            break;
        }

        bbact->_scale_time = plasmaMsg->time;
        bbact->_vp_extra[0].scale = plasmaMsg->scale;
        bbact->_vp_extra[0].pos = plasmaMsg->pos;
        bbact->_vp_extra[0].rotate = plasmaMsg->dir;
        bbact->_vp_extra[0].pos = plasmaMsg->pos;

        bbact->_vp_extra[0].SetVP(bbact->_vp_genesis);
        bbact->_vp_extra[0].flags |= EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE;

        bbact->_vp_extra_mode = 1;
    }
    break;

    case UAMSG_ENDPLASMA:
    {
        szmsg = sizeof(uamessage_endPlasma);
        uamessage_endPlasma *endpMsg = (uamessage_endPlasma *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags || (plr && plr->UpdateCountDown) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(endpMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", endpMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        if ( bhost != yw->_userRobo )
            break;

        NC_STACK_ypabact *bbact = NetGetBactByID(bhost, endpMsg->id);
        if ( !bbact )
        {
            log_netlog("\n+++ EP: vehicle %d doesn't exist\n", endpMsg->id);
            break;
        }

        bbact->_vp_extra_mode = 0;
        bbact->_scale_time = -1;
    }
    break;

    case UAMSG_STARTBEAM:
    {
        szmsg = sizeof(uamessage_startBeam);
        uamessage_startBeam *stbmMsg = (uamessage_startBeam *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(stbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ STB: Havent found robo with owner %d (%ds)\n", stbmMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        NC_STACK_yparobo *orobo = dynamic_cast<NC_STACK_yparobo *>(bhost);
        if (orobo)
        {
            orobo->_roboBeamPos = stbmMsg->pos;
            orobo->_roboBeamTimePre = 1500;

            SFXEngine::SFXe.startSound(&orobo->_soundcarrier, 9);
            orobo->_vp_extra_mode = 2;
        }
    }
    break;

    case UAMSG_ENDBEAM:
    {
        uamessage_endBeam *enbmMsg = (uamessage_endBeam *)msg->data;
        szmsg = sizeof(uamessage_endBeam);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(enbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", enbmMsg->owner, yw->_timeStamp / 1000);
            break;
        }

        bhost->_vp_extra[0].flags = 0;
        bhost->_vp_extra[1].flags = 0;
        bhost->_vp_extra_mode = 0;
    }
    break;

    case UAMSG_EXIT:
    {
        szmsg = sizeof(uamessage_exit);
        uamessage_exit *exitMsg = (uamessage_exit *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags && exitMsg->norm )
            break;

        if ( exitMsg->norm )
        {
            log_netlog(">>> received ANNOUNCEQUIT from %s at %d\n", msg->senderName.c_str(), yw->_timeStamp / 1000);
            yw->_GameShell->netPlayers[owner].Status = 3;
        }

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(exitMsg->owner);

        if ( !bhost || bhost->_status == BACT_STATUS_DEAD )
        {
            if ( exitMsg->norm )
                break;
        }

        yw->_netEvent.EventType = 5;
        yw->_netEvent.TimeStamp = yw->_timeStamp;
        yw->_netEvent.PlayerName = msg->senderName;

        yw_arg159 arg159;
        arg159.Priority = 50;
        arg159.unit = NULL;

        switch ( exitMsg->owner )
        {
        case 3:
            arg159.MsgID = 86;
            break;

        case 4:
            arg159.MsgID = 85;
            break;

        case 6:
            arg159.MsgID = 84;
            break;

        case 1:
            arg159.MsgID = 83;
            break;

        default:
            break;
        }

        yw->ypaworld_func159(&arg159);
    }
    break;

    case UAMSG_CRC:
    {
        szmsg = sizeof(uamessage_crc);
        uamessage_crc *crcMsg = (uamessage_crc *)msg->data;

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        int32_t index = yw->_netDriver->GetPlayerIndex(msg->senderName);
        if ( index != -1 )
            yw->_GameShell->lobbyPlayers[index].DataChecksum = crcMsg->checksum;

        yw->_GameShell->takTime = 0;
    }
    break;

    case UAMSG_REQPING:
    {
        uamessage_ping *pingMsg = (uamessage_ping *)msg->data;
        szmsg = sizeof(uamessage_ping);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        uamessage_ping pongMsg;
        pongMsg.owner = yw->_GameShell->netPlayerOwner;
        pongMsg.msgID = UAMSG_PONG;
        pongMsg.timestamp = pingMsg->timestamp;

        yw->NetSendMessage(&pongMsg, sizeof(pongMsg), msg->senderName, true);

        if ( yw->_updateMessage.frameTime >= 3 )
            break;

        yw->_netDriver->FlushBroadcastBuffer();
    }
    break;

    case UAMSG_PONG:
    {
        uamessage_ping *pongMsg = (uamessage_ping *)msg->data;
        szmsg = sizeof(uamessage_ping);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        int latency = yw->_GameShell->netPlayers[owner].Latency;

        if (latency)
            yw->_GameShell->netPlayers[owner].Latency = (4 * latency + yw->_timeStamp - pongMsg->timestamp) / 5;
        else
            yw->_GameShell->netPlayers[owner].Latency = yw->_timeStamp - pongMsg->timestamp;

        if ( yw->_GameShell->isHost && yw->_GameShell->netProblem != 1 && yw->_GameShell->netPlayers[owner].Latency >= 7000 )
        {
            yw->_GameShell->netProblem = 1;
            yw->_GameShell->netProblemCountDown = 200;
            yw->_GameShell->latencyCheck = 5;

            uamessage_startproblem prblm;
            prblm.owner = yw->_GameShell->netPlayerOwner;
            prblm.msgID = UAMSG_STARTPROBLEM;
            prblm.problem = 1;

            yw->NetBroadcastMessage(&prblm, sizeof(prblm), true);
        }
    }
    break;

    case UAMSG_STARTPROBLEM:
    {
        uamessage_startproblem *prblmMsg = (uamessage_startproblem *)msg->data;
        szmsg = sizeof(uamessage_startproblem);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->netProblem = prblmMsg->problem;

        if ( prblmMsg->problem != 1 )
            break;

        yw->_GameShell->netProblemCountDown = 500;
    }
    break;

    case UAMSG_ENDPROBLEM:
    {
        uamessage_endproblem *eprblmMsg = (uamessage_endproblem *)msg->data;
        szmsg = sizeof(uamessage_endproblem);

        if ( yw->_GameShell->netPlayers[owner].DestroyFlags )
            break;

        yw->_GameShell->netAllOk = eprblmMsg->solved;
        yw->_GameShell->netAllOkCountDown = 3000;

        if ( eprblmMsg->solved == 3 )
            yw->_GameShell->netAllOkCountDown = 0;

        yw->_GameShell->netProblem = 0;
    }
    break;

    case UAMSG_SCORE:
    {
        uamessage_score *scoreMsg = (uamessage_score *)msg->data;
        szmsg = sizeof(uamessage_score);

        yw->_gameplayStats = scoreMsg->status;

    }
    break;

    default:
        printf("Make me yw_handleNormMsg :  UAMSG_BASE + %d\n", msgID - UAMSG_BASE);
        break;
    }

    return szmsg;
}

std::string yw_corruptionCheck(UserData *usr)
{
    NC_STACK_ypaworld *ywo = usr->p_YW;
    NC_STACK_ypabact *found = NULL;

    if ( ywo->_timeStamp - usr->deadCheckTime >= 100000 )
    {
        usr->deadCheckTime = ywo->_timeStamp;

        for ( NC_STACK_ypabact* &station : ywo->_unitsList )
        {
            if ( station->_owner != 0 && station->_owner != usr->netPlayerOwner )
            {
                for ( NC_STACK_ypabact* &comm : station->_kidList )
                {
                    for ( NC_STACK_ypabact* &unit : comm->_kidList )
                    {
                        if ( ywo->_timeStamp - unit->_lastFrmStamp > 180000 )
                        {
                            found = unit;
                            break;
                        }
                    }

                    if ( found )
                        break;

                    if ( ywo->_timeStamp - comm->_lastFrmStamp > 180000 )
                    {
                        found = comm;
                        break;
                    }
                }

                if ( found )
                    break;

                if ( ywo->_timeStamp - station->_lastFrmStamp > 180000 )
                {
                    found = station;
                    break;
                }
            }

        }

        if ( found )
        {
            log_netlog("\n+++ CC: found old vehicle id %d, class %d, owner %d at time %d. Request update\n", found->_gid, found->_bact_type, found->_owner, ywo->_timeStamp / 1000);
            return usr->netPlayers[found->_owner].Name;
        }
    }
    return "";
}

void yw_HandleNetMsg(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->_GameShell;

    uint32_t upd_time;

    if ( SPEED_DOWN_NET )
        upd_time = 1400;
    else
        upd_time = usr->update_time_norm;

    if ( yw->_timeStamp - yw->_netUpdateTimeStamp > upd_time && usr->netProblem != 1 )
    {
        uamessage_vhclData *vhcldata;
        if ( yw->_netInterpolate )
        {
            vhcldata = &vhcldata_interpolate;
            vhcldata_interpolate.msgID = UAMSG_VHCLDATA_I;
        }
        else
        {
            vhcldata = &vhcldata_exact;
            vhcldata_exact.msgID = UAMSG_VHCLDATA_E;
        }

        vhcldata->tstamp = yw->_timeStamp;

        if ( yw_prepareVHCLData(yw, vhcldata) )
        {
            if ( yw->_netInterpolate )
                vhcldata->hdr.size = ((char *)&vhcldata_interpolate.data[ vhcldata->hdr.number ]) - ((char *)vhcldata);
            else
                vhcldata->hdr.size = ((char *)&vhcldata_exact.data[ vhcldata->hdr.number ]) - ((char *)vhcldata);

            vhcldata->hdr.diffTime = yw->_timeStamp - yw->_netUpdateTimeStamp;

            yw->NetBroadcastMessage(vhcldata, vhcldata->hdr.size, false);
        }

        yw->_netUpdateTimeStamp = yw->_timeStamp;
    }

    if ( usr->isHost )
    {
        if ( yw->_isNetGameStarted )
        {
            if ( usr->sendScoreCountDown < 0 )
            {
                uamessage_score scoreMsg;
                scoreMsg.owner = usr->netPlayerOwner;
                scoreMsg.msgID = UAMSG_SCORE;

                for(int i = 0; i < 8; i++)
                    scoreMsg.status[i] = yw->_gameplayStats[i];

                yw->NetBroadcastMessage(&scoreMsg, sizeof(scoreMsg), false);

                usr->sendScoreCountDown = 1500;
            }
            else
            {
                usr->sendScoreCountDown -= usr->DTime;
            }

        }
    }

    uint32_t msgcount = 0;
    std::string err_sender;

    windp_recvMsg recvMsg;

    while ( yw->_netDriver->Recv(&recvMsg) )
    {
        if (recvMsg.msgType != RECVMSG_NONE)
        {
            usr->netrecv_count += recvMsg._data.size();

            msgcount++;

            if ( yw->_levelInfo.State != TLevelInfo::STATE_ABORTED || !yw->_isNetGameStarted )
            {
                if ( yw->_netDriver->GetPlayerCount() * 10 >= msgcount )
                {
                    yw->_netInfoOverkill = false;
                }
                else
                {
                    yw->_netInfoOverkill = true;
                    log_netlog("Info overkill !!! (msg-count %d at %ds)\n", msgcount, yw->_timeStamp / 1000);
                    usr->problemTimer = 4000;
                }

                err_sender[0] = 0;

                switch ( recvMsg.msgType )
                {
                case RECVMSG_CREATEPLAYER:
                {

                    if ( usr->isHost && usr->remoteMode && usr->netSelMode == UserData::NETSCREEN_INSESSION )
                    {
                        if ( usr->netLevelID > 0 && usr->netLevelID < 256 )
                        {
                            uamessage_lobbyInit lbyMsg;
                            strncpy(lbyMsg.hostName, usr->netPlayerName.c_str(), 64);
                            lbyMsg.msgID = UAMSG_LOBBYINIT;
                            lbyMsg.lvlID = usr->netLevelID;
                            lbyMsg.owner = 0;

                            yw->NetBroadcastMessage(&lbyMsg, sizeof(lbyMsg), true);
                        }
                    }

                    uamessage_welcome wlcmMsg;
                    wlcmMsg.owner = 0;
                    wlcmMsg.msgID = UAMSG_WELCOME;
                    wlcmMsg.fraction = usr->SelectedFraction;
                    wlcmMsg.rdy = usr->rdyStart;

                    yw->NetSendMessage(&wlcmMsg, sizeof(wlcmMsg), recvMsg.senderName, true);

                    if ( usr->netLevelID )
                        yw->SendCRC(usr->netLevelID);

                    int plid = yw->_netDriver->GetPlayerCount();

                    
                    usr->lobbyPlayers[plid - 1].Name = recvMsg.senderName;
                    usr->lobbyPlayers[plid - 1].Welcomed = true;
                }
                break;

                case RECVMSG_DESTROYPLAYER:
                {
                    bool itisI = false;
                    bool plFound = false;

                    if ( !StriCmp(usr->netPlayerName, recvMsg.senderName) )
                    {
                        itisI = true;
                        yw->_levelInfo.State = TLevelInfo::STATE_ABORTED;
                    }

                    for(size_t i = 0; i < usr->lobbyPlayers.size(); i++)
                    {
                        if ( !StriCmp(usr->lobbyPlayers[i].Name, recvMsg.senderName) )
                        {
                            for (size_t j = i; j < usr->lobbyPlayers.size() - 1; j++)
                                usr->lobbyPlayers[j] = usr->lobbyPlayers[ j + 1 ];

                            plFound = true;
                            break;
                        }
                    }

                    log_netlog(">>> Received a destroy player for %s at %d\n", recvMsg.senderName.c_str(), yw->_timeStamp / 1000);

                    if ( !usr->HasInited && plFound)
                    {
                        for ( UserData::TNetPlayerData &pl : usr->netPlayers )
                        {
                            if ( !StriCmp(pl.Name, recvMsg.senderName) )
                            {
                                if ( pl.Status != 3 )
                                    pl.Status = 4;
                                break;
                            }
                        }

                        if ( !yw->_isNetGameStarted && !itisI )
                        {
                            if ( usr->netProblem != 3 )
                            {
                                usr->netProblem = 4;
                                usr->netProblemCountDown = 15000;
                            }
                        }

                        yw_cleanPlayer(yw, recvMsg.senderName, 0, 0);
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
                    usr->isHost = true;
                    if ( !yw->_isNetGameStarted )
                    {
                        int v70 = 0;
                        yw->_netDriver->LockSession(&v70);

                        usr->rdyStart = true;
                        usr->blocked = false;

                        int32_t index = yw->_netDriver->GetMyIndex();
                        if ( index != -1 )
                            usr->lobbyPlayers[index].Ready = true;
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

                        size_t msg_size = yw_handleNormMsg(yw, &recvMsg, &err_sender);

                        if ( !msg_size )
                        {
                            log_netlog("    unknown message was number %d and message before was type %d\n", msgcnt, HNDL_MSG[ msgcnt - 1 ]);
                            log_netlog("    current offset is %d, size is %d\n", msgSizes, recvMsg._data.size());
                            break;
                        }

                        if ( msg_size < sizeof(uamessage_base) )
                        {
                            log_netlog("Error: Message %d has strange size %d!\n", msgID, msg_size);
                            break;
                        }


                        msgSizes += msg_size;

                        if ( msgSizes >= recvMsg._data.size() )
                            break;
                        else
                            recvMsg.data = (char *)recvMsg.data + msg_size;
                    }
                    if (msgcnt == 1)
                        printf("RECVMSG_NORMAL ID %d\n", HNDL_MSG[1]);
                    printf("RECVMSG_NORMAL msgcnt %d\n", msgcnt);

                    std::string crpt = yw_corruptionCheck(usr);
                    if ( !crpt.empty() )
                        err_sender = crpt;
                }
                break;

                default:
                    break;
                }

                if ( !err_sender.empty() )
                {
                    for (UserData::TNetPlayerLobbyData &pl : usr->lobbyPlayers)
                    {
                        if (StriCmp(pl.Name, err_sender) == 0)
                        {
                            if ( pl.UpdateCountDown == 0 )
                            {
                                log_netlog("Drastic Error: Request Update from %s\n", err_sender.c_str());

                                uamessage_requpdate updMsg;
                                updMsg.msgID = UAMSG_REQUPDATE;
                                updMsg.owner = usr->netPlayerOwner;

                                yw->NetSendMessage(&updMsg, sizeof(updMsg), err_sender, true);

                                pl.UpdateCountDown = 2000;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    usr->netrecv_time += yw->_updateMessage.frameTime;

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



bool NC_STACK_ypaworld::yw_NetSetHostStations(const std::vector<MapRobo> &Robos)
{
    _levelInfo.OwnerMask = 0;
    _levelInfo.UserMask = 0;

    for (size_t i = 0; i < _GameShell->netPlayers.size(); i++)
    {
        UserData::TNetPlayerData &pl = _GameShell->netPlayers[i];
        pl.Ready = false;
        pl.Owner = i;
    }

    const MapRobo *selHost = NULL;

    int owner = 0;

    for ( const TDPPlayerData &plData :  _netDriver->GetPlayersData() )
    {
        if ( (size_t)plData.Index >= Robos.size() )
        {
            log_netlog("No Robo declared for player %d\n", Robos.size());
            return false;
        }

        int selFraction;

        if ( plData.IsItMe() )
        {
            selFraction = _GameShell->SelectedFraction;
        }
        else
        {
            selFraction = _GameShell->lobbyPlayers[plData.Index].NetFraction;
        }

        if ( selFraction == 0 )
        {
            log_netlog("Error no race for robo %s found\n", plData.name.c_str());
            return false;
        }

        switch ( selFraction )
        {
        case World::OWNER_RESIST_BIT:
            owner = 1;
            break;
        case World::OWNER_GHOR_BIT:
            owner = 6;
            break;
        case World::OWNER_MYKO_BIT:
            owner = 3;
            break;
        case World::OWNER_TAER_BIT:
            owner = 4;
            break;
        default:
            break;
        }

        selHost = NULL;
        _GameShell->lobbyPlayers[plData.Index].Owner = owner;

        for ( const MapRobo &host: Robos )
        {
            if ( owner == host.Owner )
            {
                selHost = &host;
                break;
            }
        }

        if ( !selHost )
        {
            log_netlog("Oh! No owner %d specified in LDF, but somebody wanted to play it!\n", owner);
            return false;
        }

        vec3d place(selHost->Pos);

        _GameShell->netPlayers[owner].Name = plData.name;

        ypaworld_arg136 arg136;
        arg136.stPos = place.X0Z() - vec3d::OY(30000.0);
        arg136.vect = vec3d::OY(50000.0);
        arg136.flags = 0;

        ypaworld_func136(&arg136);

        if ( arg136.isect )
            place.y += arg136.isectPos.y;
        else
            log_netlog("Warning: Robo placed without y-correction\n");

        ypaworld_arg146 arg146;
        switch ( selFraction )
        {
        case World::OWNER_RESIST_BIT:
            arg146.vehicle_id = 56;
            break;
        case World::OWNER_GHOR_BIT:
            arg146.vehicle_id = 57;
            break;
        case World::OWNER_MYKO_BIT:
            arg146.vehicle_id = 58;
            break;
        case World::OWNER_TAER_BIT:
            arg146.vehicle_id = 60;
            break;
        default:
            break;
        }
        arg146.pos = place;

        NC_STACK_ypabact *hostObj = ypaworld_func146(&arg146);

        if ( !hostObj )
        {
            log_netlog("NetPlaceRobos: Unable to create robo (owner %d, type %d)\n", plData.Index + 1, arg146.vehicle_id);
            return false;
        }

        hostObj->_owner = owner;
        hostObj->_gid |= owner << 24;

        _levelInfo.OwnerMask |= 1 << owner;

        for (NC_STACK_ypabact* &bct :  hostObj->_kidList)
        {
            bct->_gid |= owner << 24;
            bct->_owner = owner;
        }

        if ( plData.IsItMe() )
        {
            hostObj->setBACT_viewer(true);
            hostObj->setBACT_inputting(true);
            _GameShell->netPlayerOwner = owner;
            _levelInfo.UserMask |= 1 << owner;
        }

        ypaworld_func134(hostObj);

        hostObj->setBACT_bactCollisions(1);

        NC_STACK_yparobo *robo = static_cast<NC_STACK_yparobo *>(hostObj);
        robo->setROBO_fillMode(15);

        int enrg = selHost->Energy / 4;

        robo->setROBO_battVehicle(enrg);
        robo->setROBO_battBeam(enrg);
        robo->setROBO_viewAngle(selHost->ViewAngle);

        hostObj->_energy = enrg;
        hostObj->_energy_max = enrg;

        if ( selHost->ReloadConst )
            hostObj->_reload_const = selHost->ReloadConst;
        else
            hostObj->_reload_const = hostObj->_energy_max;
    }
    return true;
}



size_t NC_STACK_ypaworld::ypaworld_func179(yw_arg161 *arg)
{
    bact_id = 0x10000;
    
    _particles.Clear();

    TLevelDescription proto;
    bool loadOK = false;

    if ( LevelCommonLoader(&proto, arg->lvlID, arg->field_4) )
    {
        if ( LoadTypeMap(proto.TypStr) )
        {
            if ( LoadOwnerMap(proto.OwnStr) )
            {
                if ( LoadHightMap(proto.HgtStr) )
                {
                    if ( yw_NetSetHostStations(proto.Robos) )
                    {
                        if ( LoadBlgMap(proto.BlgStr) )
                        {
                            for (int y = 0; y < _mapSize.y; y++)
                            {
                                for (int x = 0; x < _mapSize.x; x++)
                                {
                                    CellCheckHealth(&_cells(x, y), World::OWNER_RECALC, NULL);
                                }
                            }

                            yw_InitTechUpgradeBuildings();
                            InitGates();
                            InitSuperItems();
                            UpdatePowerEnergy();
                            
                            PrepareAllFillers();

                            if ( sb_0x451034(this) )
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
        DeleteLevel();
        return 0;
    }

    _lvlPrimevalTypeMap = _lvlTypeMap;
    _lvlPrimevalOwnMap = _lvlOwnMap;

    _GameShell->msgcount = 0;

    for (UserData::TNetPlayerLobbyData &pl : _GameShell->lobbyPlayers)
    {
        pl.Msg.clear();
        pl.IsTrouble = false;
        pl.UpdateCountDown = 0;
    }

    for (UserData::TNetPlayerData &pl : _GameShell->netPlayers)
    {
        pl.DestroyFlags = 0;
        pl.Ready = false;
        pl.LastMessageTime = _timeStamp + 1000;
        pl.TimeStamp = 0;
        pl.Status = 0;
        pl.ProblemCountDown = 0;
    }

    for ( NC_STACK_ypabact* &bct : _unitsList )
        _GameShell->netPlayers[ bct->_owner ].Status = 1;

    NC_STACK_ypabact *fndHost = NULL;
    for ( NC_STACK_ypabact* &host : _unitsList )
    {
        if (host->_owner == _GameShell->netPlayerOwner)
        {
            fndHost = host;
            break;
        }
    }

    _GameShell->netPlayers[ _GameShell->netPlayerOwner ].Ready = true;
    _isNetGameStarted = false;

    _netStartTimer = System::IniConf::NetWaitStart.Get<int>();
    _GameShell->kickTime = System::IniConf::NetKickoff.Get<int>();

    log_netlog("netstarttime was initialized with %d sec\n", _netStartTimer / 1000);
    log_netlog("kickoff was initialized with      %d sec\n", _GameShell->kickTime / 1000);

    if ( fndHost )
    {
        uamessage_syncgame syncMsg;
        syncMsg.owner = _GameShell->netPlayerOwner;
        syncMsg.msgID = UAMSG_SYNCGM;
        syncMsg.hostID = fndHost->_gid;

        int gn = 0;
        for(NC_STACK_ypabact* &bct : fndHost->_kidList)
        {
            if (gn >= 8)
                break;

            int rbgun = 0;
            if (bct->_bact_type == BACT_TYPES_GUN)
                rbgun = ((NC_STACK_ypagun *)bct)->IsRoboGun();

            if (rbgun)
            {
                syncMsg.gun[gn] = bct->_gid;
                gn++;
            }
        }

        NetBroadcastMessage(&syncMsg, sizeof(syncMsg), true);

        _netDriver->FlushBroadcastBuffer();

        log_netlog("Sent a READY TO PLAY to all for my Owner %d\n", _GameShell->netPlayerOwner);

        fndHost->setBACT_viewer(true);
    }

    _GameShell->netsend_count = 0;
    _GameShell->netrecv_count = 0;
    _GameShell->netrecv_time = 0;
    _GameShell->netsend_speed = 0;
    _GameShell->netrecv_speed = 0;
    _GameShell->netrecv_bpsMin = 0;
    _GameShell->netrecv_bpsMax = 0;
    _GameShell->netrecv_bpsAvr = 0;
    _GameShell->netsend_bpsMin = 0;
    _GameShell->netsend_bpsMax = 0;
    _GameShell->netsend_bpsAvr = 0;
    _GameShell->net_gcnt = 0;
    _GameShell->net_packet_size = 0;
    _GameShell->net_packet_min = 0;
    _GameShell->net_packet_max = 0;
    _GameShell->net_packet_cnt = 0;
    _GameShell->net_packet_avr = 0;
    _GameShell->deadCheckTime = _timeStamp + 300000;
    _GameShell->sendScoreCountDown = 3000;

    _playersStats.fill(World::TPlayerStatus());

    return 1;
}


void yw_NetHandleProblems(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->_GameShell;
    switch ( usr->netProblem )
    {
    case 1:
        usr->ResetInputPeriod = true;
        usr->netProblemCountDown--;

        if ( usr->netProblemCountDown <= 0 )
        {
            usr->netProblem = 0;
            usr->netAllOk = 3;
            usr->netAllOkCountDown = 0;

            for (UserData::TNetPlayerData &pl : usr->netPlayers)
                pl.Latency = 0;

            if ( usr->isHost )
            {
                uamessage_endproblem eProblm;
                eProblm.owner = usr->netPlayerOwner;
                eProblm.msgID = UAMSG_ENDPROBLEM;
                eProblm.solved = 3;

                yw->NetBroadcastMessage(&eProblm, sizeof(eProblm), true);
            }
        }
        break;

    case 3:
        usr->netProblemCountDown -= yw->_updateMessage.frameTime;
        if ( usr->netProblemCountDown < 0 )
        {
            yw->_levelInfo.State = TLevelInfo::STATE_ABORTED;
            usr->netProblem = 0;
        }
        break;

    case 4:
        usr->netProblemCountDown -= yw->_updateMessage.frameTime;

        if ( usr->netProblemCountDown < 0 )
        {
            usr->netProblem = 0;
            for (int i = 0; i < 8; i++)
                usr->netPlayers[i].DestroyFlags &= ~2;
        }
        break;

    default:
        break;
    }
}

int yw_NetCheckPlayers(NC_STACK_ypaworld *yw)
{
    int v2 = 0;
    UserData *usr = yw->_GameShell;

    for ( const TDPPlayerData &plData : yw->_netDriver->GetPlayersData() )
    {
        if ( !plData.IsItMe() )
        {
            UserData::TNetPlayerData *plr = NULL;

            for ( UserData::TNetPlayerData &pl : usr->netPlayers )
            {
                if ( StriCmp(pl.Name, plData.name) == 0 )
                {
                    plr = &pl;
                    break;
                }
            }

            if ( plr )
            {
                if ( plr->ProblemCountDown > 0 )
                {
                    plr->ProblemCountDown -= yw->_updateMessage.frameTime;
                    if ( plr->ProblemCountDown <= 0 )
                        plr->Status = 1;
                }

                int tm = yw->_updateMessage.gTime - plr->LastMessageTime;

                if ( yw->_isNetGameStarted && tm > 7000 )
                {
                    SPEED_DOWN_NET = 1;

                    if ( tm / 1000 != (tm - yw->_updateMessage.frameTime) / 1000 )
                    {
                        log_netlog("Waiting for player %s. (time %ds at %ds)\n", plData.name.c_str(), tm / 1000, yw->_timeStamp / 1000);
                        log_netlog("    Reduce data transfer rate\n");
                    }

                    usr->problemTimer = 4000;

                    plr->Status = 2;
                    plr->ProblemCountDown = 5000;

                    if ( (yw->_updateMessage.gTime - (int)plr->LastMessageTime) > usr->kickTime )
                    {
                        if ( usr->isHost )
                        {
                            uamessage_kick msgKk;
                            msgKk.owner = plr->Owner;
                            msgKk.msgID = UAMSG_KICK;
                            strcpy(msgKk.text, plr->Name.c_str());

                            yw_cleanPlayer(yw, plr->Name, 0, 0);

                            if ( usr->netProblem != 3 )
                            {
                                usr->netProblemName = msgKk.text;
                                usr->netProblem = 4;
                                usr->netProblemCountDown = 15000;
                            }

                            v2 = 1;

                            yw->NetBroadcastMessage(&msgKk, sizeof(msgKk), true);

                            yw_DestroyPlayer(yw, msgKk.text);

                            log_netlog(">>> I have kicked off %s because I haven't heard anything for %d sec (at time %d)\n", msgKk.text, usr->kickTime / 1000, yw->_timeStamp / 1000);
                        }
                    }

                    if ( (yw->_updateMessage.gTime - plr->LastMessageTime) > 20000 )
                    {
                        if ( !usr->isHost )
                        {
                            if ( usr->netProblem != 4 && usr->netProblem != 3 )
                            {
                                usr->netProblem = 5;
                                usr->netProblemCountDown = 10;
                                v2 = 1;
                            }
                        }
                    }

                    plr->ConnectionProblem = 1;
                }
                else
                {
                    plr->ConnectionProblem = 0;
                }
            }
            else
            {
                log_netlog("Warning: No Playerdata for player %s in PlayersOK() (%ds)\n", plData.name.c_str(), yw->_timeStamp / 1000);
            }
        }
    }

    if ( usr->netProblem == 5 && !v2 )
        usr->netProblem = 0;

    return 1;
}

int yw_NetCheckPlayersInGame(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->_GameShell;

    int numpl = yw->_netDriver->GetPlayerCount();

    if ( yw->_isNetGameStarted )
        return 1;

    int rdypl = 0;
    std::vector<UserData::TNetPlayerData*> wting;
    wting.reserve(World::CVMaxNetPlayers);

    for(UserData::TNetPlayerData &pl : usr->netPlayers)
    {
        if (!pl.Name.empty())
        {
            if ( pl.Ready )
                rdypl++;
            else if (pl.Owner != 0)
                wting.push_back(&pl);
        }
    }

    if ( rdypl == numpl )
    {
        yw->_isNetGameStarted = true;
        return 1;
    }

    SDL_Delay(50);

    TInputState inpt;
    Input::Engine.QueryInput(&inpt);

    CmdStream buf;
    buf.reserve(1024);

    FontUA::select_tileset(&buf, 0);
    FontUA::set_xpos(&buf, 0);
    FontUA::set_ypos(&buf, 0);
    FontUA::copy_position(&buf);
    FontUA::add_txt(&buf, 2 * yw->_screenSize.x / 3 - 1, 1,  Locale::Text::Get(Locale::LBL_WAITINGFOR, Locale::DefaultStrings::WaitingForPlayers) );
    FontUA::next_line(&buf);

    yw->_netStartTimer -= inpt.Period;
    if ( yw->_netStartTimer > 0 )
    {
        std::string timeStr = fmt::sprintf("(%d)", yw->_netStartTimer / 1000);

        FontUA::copy_position(&buf);
        FontUA::add_txt(&buf, yw->_screenSize.x / 3 - 1, 1, timeStr);
        FontUA::next_line(&buf);
    }

    for(UserData::TNetPlayerData *pl : wting)
    {
        if ( yw->_netStartTimer <= 0 && usr->isHost )
        {
            uamessage_kick msgKk;
            strcpy(msgKk.text, pl->Name.c_str());
            msgKk.msgID = UAMSG_KICK;
            msgKk.owner = pl->Owner;

            yw_cleanPlayer(yw, pl->Name, 0, 0);

            yw->NetBroadcastMessage(&msgKk, sizeof(msgKk), true);

            yw_DestroyPlayer(yw, msgKk.text);

            if ( usr->netProblem != 3 )
            {
                usr->netProblemName = msgKk.text;

                yw->_GameShell->netProblem = 4;
                yw->_GameShell->netProblemCountDown = 15000;
            }
            log_netlog(">>> I have kicked off %s because I didn't heard anything after loading (time %d)\n", msgKk.text, yw->_timeStamp / 1000);
            log_netlog("    netstarttime was %d\n", yw->_netStartTimer);
        }
        else
        {
            std::string tmpstr = "   ";
            tmpstr += pl->Name;

            FontUA::copy_position(&buf);
            FontUA::add_txt(&buf, yw->_screenSize.x - 1, 1, tmpstr);
            FontUA::next_line(&buf);
        }
    }

    if ( yw->_netStartTimer <= 0 )
    {
        if ( yw->_GameShell->isHost )
            return 1;

        FontUA::copy_position(&buf);
        FontUA::add_txt(&buf, yw->_screenSize.x - 1, 1, "WAITING FOR HOST");
        FontUA::next_line(&buf);
    }

    FontUA::set_end(&buf);

    GFX::Engine.BeginFrame();

    GFX::Engine.ProcessDrawSeq(buf);

    GFX::Engine.EndFrame();

    return 0;
}


void yw_NetCheckPing(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->_GameShell;

    if ( usr->latencyCheck <= 0 && usr->isHost && (usr->netProblem == 0 || usr->netProblem == 1) )
    {
        if ( usr->netProblem == 1 )
            usr->latencyCheck = 5;
        else
            usr->latencyCheck = 2000;

        uamessage_ping rPing;
        rPing.owner = usr->netPlayerOwner;
        rPing.msgID = UAMSG_REQPING;
        rPing.timestamp = yw->_timeStamp;

        yw->NetBroadcastMessage(&rPing, sizeof(rPing), true);

        if ( yw->_updateMessage.frameTime < 3 )
        {
            yw->_netDriver->FlushBroadcastBuffer();
        }
    }
    else
    {
        usr->latencyCheck -= yw->_updateMessage.frameTime;
    }
}


void yw_NetMsgHndlLoop(NC_STACK_ypaworld *yw)
{
    while ( 1 )
    {
        if ( yw->_GameShell->netProblem != 0 )
            yw_NetHandleProblems(yw);

        yw->_GameShell->netAllOkCountDown -= yw->_updateMessage.frameTime;

        yw_HandleNetMsg(yw);

        if ( yw->_levelInfo.State == TLevelInfo::STATE_ABORTED )
            break;

        SPEED_DOWN_NET = 0;

        if ( yw_NetCheckPlayers(yw) && yw_NetCheckPlayersInGame(yw) )
        {
            yw_NetCheckPing(yw);

            if ( yw->_GameShell->problemTimer > 0 )
                yw->_GameShell->problemTimer -= yw->_updateMessage.frameTime;
            break;
        }
    }
}


void yw_NetDrawStats(NC_STACK_ypaworld *yw)
{
    bool toDraw = false;

    std::string t[16];

    UserData *usr = yw->_GameShell;

    TileMap *font29 = yw->_guiTiles[29];
    CmdStream drawbuf;
    drawbuf.reserve(2000);

    FontUA::select_tileset(&drawbuf, 29);
    FontUA::set_center_xpos(&drawbuf, font29->h - (yw->_screenSize.x / 2) );
    FontUA::set_center_ypos(&drawbuf, font29->h - (yw->_screenSize.y / 2) );

    if ( usr->disconnected == false && usr->problemTimer > 0 )
    {
        if ( (yw->_updateMessage.gTime / 300) & 1 )
        {
            FontUA::store_u8(&drawbuf, 65);
        }
    }

    FontUA::set_end(&drawbuf);

    GFX::Engine.ProcessDrawSeq(drawbuf);

    int numelm = 0;

    if ( usr->netProblem )
    {
        toDraw = true;

        switch ( usr->netProblem )
        {
        case 1:
            if ( usr->isHost )
            {
                t[0] = Locale::Text::Advanced(Locale::ADV_HOSTLATENCY);
                t[1] = fmt::sprintf("%s %d", Locale::Text::Advanced(Locale::ADV_WAIT1), usr->netProblemCountDown);
                numelm = 2;
            }
            else
            {
                t[0] = Locale::Text::Advanced(Locale::ADV_CLILATENCY);
                t[1] = fmt::sprintf("%s %d", Locale::Text::Advanced(Locale::ADV_WAIT2), usr->netProblemCountDown);
                numelm = 2;
            }
            break;

        case 3:
            t[0] = Locale::Text::Advanced(Locale::ADV_KICKEDPROBLEM);
            t[1] = fmt::sprintf("%s %d", Locale::Text::Advanced(Locale::ADV_LVLAUTOFINISH), usr->netProblemCountDown / 1000);
            numelm = 2;
            break;

        case 4:
            t[0] = Locale::Text::Advanced(Locale::ADV_PLREMOVED);
            t[1] = fmt::sprintf(Locale::Text::Advanced(Locale::ADV_COZNETPROBLEM));
            numelm = 2;

            for(UserData::TNetPlayerData &pl : usr->netPlayers)
            {
                if (pl.DestroyFlags & 2)
                {
                    t[numelm] = pl.Name;
                    numelm++;
                }
            }
            break;

        case 5:
            t[0] = Locale::Text::Advanced(Locale::ADV_NOCONNTOPL);
            t[1] = fmt::sprintf(Locale::Text::Advanced(Locale::ADV_FINISHIFNOT));
            numelm = 2;

            for(UserData::TNetPlayerData &pl : usr->netPlayers)
            {
                if ( (yw->_updateMessage.gTime - pl.LastMessageTime) > 20000 )
                {
                    if ( pl.Ready )
                    {
                        if ( StriCmp(pl.Name, usr->netPlayerName) )
                        {
                            t[numelm] = pl.Name;
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
        if ( usr->netAllOkCountDown > 0 )
        {
            numelm = 1;
            toDraw = true;

            if ( usr->netAllOk == 1 )
            {
                t[0] = Locale::Text::Advanced(Locale::ADV_NETISOK);
            }
            else if ( usr->netAllOk == 2 )
            {
                t[0] = Locale::Text::Advanced(Locale::ADV_UNRESPROBLM);
            }
            else if ( usr->netAllOk == 3 )
            {
                t[0] = "   ";
                toDraw = false;
            }
            else
            {
                t[0] = "???";
            }
        }
    }

    if ( toDraw )
    {
        TileMap *font0 = yw->_guiTiles[0];
        drawbuf.clear();

        FontUA::select_tileset(&drawbuf, 0);
        FontUA::set_center_xpos(&drawbuf, yw->_screenSize.x / 4 - yw->_screenSize.x / 2);
        FontUA::set_center_ypos(&drawbuf, 12 - yw->_screenSize.y / 2);

        GuiBase::FormateTitle(yw, yw->_screenSize.x / 4 - yw->_screenSize.x / 2,
                                    12 - yw->_screenSize.y / 2,
                                    yw->_screenSize.x / 2,
                                    Locale::Text::Advanced(Locale::ADV_NETSTATUS),
                                    &drawbuf, 0, 0);

        FontUA::next_line(&drawbuf);

        FontUA::select_tileset(&drawbuf, 0);

        for(int i = 0; i < numelm; i++)
        {
            FontUA::store_u8(&drawbuf, '{');

            FontUA::copy_position(&drawbuf);
            FontUA::op17(&drawbuf, yw->_screenSize.x / 2 - font0->map['}'].w);

            FontUA::store_u8(&drawbuf, ' ');
            FontUA::store_u8(&drawbuf, '}');

            FontUA::add_txt(&drawbuf, yw->_screenSize.x / 2 - 2 * font0->map['W'].w, 4, t[i] );

            FontUA::next_line(&drawbuf);
        }

        FontUA::set_yoff(&drawbuf, font0->h - 1);

        FontUA::store_u8(&drawbuf, 'x');
        FontUA::op17(&drawbuf, yw->_screenSize.y / 2 - font0->map['z'].w);
        FontUA::store_u8(&drawbuf, 'y');
        FontUA::store_u8(&drawbuf, 'z');

        FontUA::set_end(&drawbuf);

        GFX::Engine.ProcessDrawSeq(drawbuf);
    }
}
