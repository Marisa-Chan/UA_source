#include <stdlib.h>
#include <string.h>
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
#include "windp.h"
#include "fmtlib/printf.h"

#define C_2PI_127       0.04947390005653217698   // (2PI / 127)
#define C_127_2PI       20.21267777267070764265  // (127 / 2PI)

extern uint32_t bact_id;
extern Common::Ini::KeyList ypaworld_keys;
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

    if ( ywo->UserRobo != host )
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

    ypaworld_func181(&ywmsg);
}

void yw_CheckCRCs(NC_STACK_ypaworld *yw)
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

        int plCnt = yw->windp->CountPlayers(NULL);

        bool err = false;
        int errmsg = 0;

        for (int i = 0; i < plCnt; i++)
        {
            if ( StriCmp(yw->GameShell->callSIGN, yw->GameShell->players2[i].name) &&
                    usr->netCRC != yw->GameShell->players2[i].checksum &&
                    yw->GameShell->players2[i].checksum != 0 &&
                    usr->netCRC != 0 )
            {
                err = true;
                strs[errmsg] = fmt::sprintf("%s %s", yw->GameShell->players2[i].name, get_lang_string(yw->string_pointers_p2, 2404, "HAS OTHER FILES THAN YOU") );
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

void UserData::yw_CheckCDs()
{
    if ( glblTime - last_cdchk >= 1500 )
    {
        last_cdchk = glblTime;

        windp_arg79 plData;
        plData.ID = 0;
        plData.mode = 0;

        while ( p_ypaworld->windp->GetPlayerData(&plData) )
        {
            if ( !StriCmp(plData.name, callSIGN) )
            {
                players2[plData.ID].cd = 1;
                break;
            }
            plData.ID++;
        }

        cd = 1;

        uamessage_cd cdMsg;
        cdMsg.cd = 1;
        cdMsg.rdy = rdyStart;
        cdMsg.msgID = UAMSG_CD;
        cdMsg.owner = 0;

        yw_arg181 ywMsg;
        ywMsg.dataSize = sizeof(cdMsg);
        ywMsg.recvFlags = 2;
        ywMsg.recvID = 0;
        ywMsg.garant = 1;
        ywMsg.data = &cdMsg;

        p_YW->ypaworld_func181(&ywMsg);
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
        if ( yw->UserRobo == robo )
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

    yw_netBakeVhcl(fndRobo, dat, 0, yw->netInterpolate);
    dat->owner = fndRobo->_owner;

    int num = 1;

    for( NC_STACK_ypabact* &comm : fndRobo->_kidList )
    {
        if (num >= 1023)
            break;

        yw_netBakeVhcl(comm, dat, num, yw->netInterpolate);
        num++;

        for( NC_STACK_ypabact* &unit : comm->_kidList )
        {
            if ( num >= 1023 )
                break;
            yw_netBakeVhcl(unit, dat, num, yw->netInterpolate);

            num++;
        }
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

void yw_cleanPlayer(NC_STACK_ypaworld *yw, const char *name, uint8_t owner, uint8_t mode)
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

    NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(own);
    if ( bhost )
    {
        yw->NetRemove(bhost);
        yw->GameShell->players[own].isKilled = 3;
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

        if ( v49.x < 0.0 || v49.x > bact->_wrldX || v49.z > 0.0 || v49.z < bact->_wrldY )
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

    if (netInterpolate)
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
            log_netlog("+++ EVD: Haven't found vehicle ident %d  from owner %d (%dsec)\n", ident, host_node->_owner, timeStamp / 1000);
        else
        {
            if ( netInterpolate )
                yw_netApplyVhclDataI(bact, datI, i, timeStamp);
            else
                yw_netApplyVhclDataE(bact, datE, i, timeStamp);
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

    yw_arg181 arg181;
    arg181.recvID = recvID;
    arg181.recvFlags = 1;
    arg181.dataSize = updinf.sz;
    arg181.data = &updinf;
    arg181.garant = 1;

    if ( yw->ypaworld_func181(&arg181) )
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
        yw->GameShell->players[owner].lastMsgTime = yw->field_1b24.gTime;

        if ( msgID != UAMSG_VHCLENERGY && msgID != UAMSG_ENDPLASMA && msg->cast == 2 )
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
        if (StriCmp(yw->GameShell->players2[i].name, msg->senderName) == 0)
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
        yw->GameShell->envAction.action = EnvAction::ACTION_NETPLAY;
        yw->GameShell->envAction.params[0] = ldMsg->level;
        yw->GameShell->envAction.params[1] = ldMsg->level;

        yw->isNetGame = 1;
        yw->GameShell->yw_NetPrintStartInfo();
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
            log_netlog("\n+++ NV: Robo Owner %d not found (%ds)\n", nvMsg->owner, yw->timeStamp / 1000);
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
        bacto->_lastFrmStamp = yw->timeStamp;
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

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(dvMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ DV:Havent found robo with owner %d (%ds)\n", dvMsg->owner, yw->timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *bctt = NetGetBactByID(host_node, dvMsg->id);

        if ( !bctt )
        {
            log_netlog("\n+++ DV: Havent found vehicle %d (%ds)\n", dvMsg->id, yw->timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        if ( !(bctt->_status_flg & BACT_STFLAG_DEATH1) )
        {
            log_netlog("+++ DV: Release a non-logic-dead vehicle %d! (%ds)\n", dvMsg->id, yw->timeStamp / 1000);
            *err = msg->senderName;
            break;
        }

        int vnumb = yw_netGetUnitsCount(host_node);

        while( !bctt->_kidList.empty() )
        {
            yw->ypaworld_func144(bctt->_kidList.front());
            log_netlog("+++ DV: Released vehicle with slave! (%ds)\n", yw->timeStamp / 1000);
            *err = msg->senderName;
        }

        while(bctt->_missiles_list.begin() != bctt->_missiles_list.end())
        {
            NC_STACK_ypamissile * misl = bctt->_missiles_list.front();
            bctt->_missiles_list.pop_front(); //CHECKIT? why only first missile detach and others will be processed by NetReleaseMissiles

            misl->_parent = NULL;

            yw->NetReleaseMissiles(bctt);

            log_netlog("+++ DV: Released vehicle with weapons! (%ds)\n", yw->timeStamp / 1000);
            *err = msg->senderName;
        }

        yw->ypaworld_func144(bctt);

        netDebug_addDestroyed(host_node->_owner, dvMsg->id);

        if ( dvMsg->type != BACT_TYPES_MISSLE && yw_netGetUnitsCount(host_node) != (vnumb - 1) )
        {
            log_netlog("\n+++ DV: Vehiclecount changed more than 1! (%ds)\n", yw->timeStamp / 1000);
            *err = msg->senderName;
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

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(nlMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NC:Havent found robo with owner %d (%ds)\n", nlMsg->owner, yw->timeStamp / 1000);
            break;
        }

        int unitscnt = yw_netGetUnitsCount(host_node);

        NC_STACK_ypabact *oldLeader = NetGetBactByID(host_node, nlMsg->id);

        if ( !oldLeader )
        {
            log_netlog("\n+++ NC: Havent found vehicle %d (%ds)\n", nlMsg->id, yw->timeStamp / 1000);
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
            log_netlog("\n+++ NC: Vehiclecount changed! (%ds)\n", yw->timeStamp / 1000);
            *err = msg->senderName;
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

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(nwMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ NW: Havent found robo with owner %d (%ds)\n", nwMsg->owner, yw->timeStamp / 1000);
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
            log_netlog("\n+++ NW: Havent found vehicle %d (%ds)\n", nwMsg->id, yw->timeStamp / 1000);
            yw->ypaworld_func144(weapo);
            *err = msg->senderName;
            break;
        }

        weapo->_kidRef.Detach();
        weapo->_parent = NULL;

        weapLauncher->_missiles_list.push_back(weapo);

        weapo->setMISS_launcher(weapLauncher);

        weapo->_owner = nwMsg->owner;
        weapo->_gid = nwMsg->id;
        weapo->_lastFrmStamp = yw->timeStamp;

        netDebug_AddCreated(host_node->_owner, nwMsg->id);

        weapo->_fly_dir_length = nwMsg->dir.length();
        weapo->_fly_dir = nwMsg->dir;

        if ( weapo->_fly_dir_length > 0.001 )
        {
            weapo->_fly_dir /= weapo->_fly_dir_length;
        }

        weapo->AlignMissile();

        int wpnType = weapo->getMISS_type();

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

        WeapProto *protos = yw->getYW_weaponProtos();
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

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(ssMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ SS: Havent found robo with owner %d (%ds)\n", ssMsg->owner, yw->timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *bct = NetGetBactByID(host_node, ssMsg->id);
        if ( !bct )
        {
            log_netlog("\n+++ SS: Havent found vehicle %d (%ds)\n", ssMsg->id, yw->timeStamp / 1000);
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

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) || tv481 )
            break;

        if ( yw->netInterpolate )
        {
            NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(vdMsg->owner);

            if ( host_node )
                yw->yw_processVhclDataMsgs(vdMsg, host_node);
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
        szmsg = msg->_data.size();

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) || tv481 )
            break;

        if ( yw->netInterpolate )
        {
            log_netlog("\n+++ VD: An interpolate-Program gets extrapolate data!!!\n");
        }
        else
        {
            NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(vdMsg->owner);

            if ( host_node )
                yw->yw_processVhclDataMsgs(vdMsg, host_node);
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

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(ddMsg->owner);

        if ( !host_node )
        {
            log_netlog("\n+++ D: Havent found robo with owner %d (%ds)\n", ddMsg->owner, yw->timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *fndBact = NULL;

        if ( ddMsg->classID == BACT_TYPES_MISSLE )
        {
            fndBact = yw_netGetMissileByID(host_node, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found weapon %d (%ds)\n", ddMsg->id, yw->timeStamp / 1000);
                *err = msg->senderName;
                break;
            }

        }
        else
        {
            fndBact = NetGetBactByID(host_node, ddMsg->id);

            if ( !fndBact )
            {
                log_netlog("\n+++ D: Havent found vehicle %d (%ds)\n", ddMsg->id, yw->timeStamp / 1000);
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
                    log_netlog("\n+++ D: No master (%d) found for my slaves (%ds)\n", ddMsg->newParent, yw->timeStamp / 1000);
                else
                    log_netlog("\n+++ D: No master given for my slaves (%ds)\n", yw->timeStamp / 1000);

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
                miss->setMISS_launcher(fndBact->_parent);
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
            log_netlog("+++ D: I am dead, but I have slave ident %d class %d with state %d (%ds)\n", k->_gid, k->_bact_type, k->_status, yw->timeStamp / 1000);

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

        if ( yw->GameShell->players[owner].isKilled )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(veMsg->owner);
        if ( !host_node )
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", veMsg->owner, yw->timeStamp / 1000);
            break;
        }

        if ( host_node != yw->UserRobo )
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

        if ( yw->GameShell->players[owner].isKilled )
            break;

        NC_STACK_ypabact *host_node = yw->yw_getHostByOwner(seMsg->owner);

        yw_arg129 arg129;
        if ( host_node )
        {
            arg129.unit = NetGetBactByID(host_node, seMsg->whoHit);
        }
        else
        {
            log_netlog("\n+++ VE: Havent found robo with owner %d (%ds)\n", seMsg->owner, yw->timeStamp / 1000);
            arg129.unit = 0;
        }

        arg129.pos = seMsg->pos;
        arg129.field_10 = seMsg->energy;
        arg129.field_14 = seMsg->sectOwner;

        yw->ypaworld_func129(&arg129);
    }
    break;

    case UAMSG_STARTBUILD:
    {
        szmsg = sizeof(uamessage_startBuild);
        uamessage_startBuild *sbMsg = (uamessage_startBuild *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        if ( yw->yw_getHostByOwner(sbMsg->owner) )
        {
            ypaworld_arg148 arg148;
            arg148.ownerID = sbMsg->owner;
            arg148.ownerID2 = sbMsg->owner;
            arg148.blg_ID = sbMsg->bproto;
            arg148.field_C = 0;
            arg148.x = sbMsg->sec_x;
            arg148.y = sbMsg->sec_y;
            arg148.field_18 = 0;

            yw->ypaworld_func148(&arg148);
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
            log_netlog("\n+++ BV: Havent found robo with owner %d (%ds)\n", bvMsg->owner, yw->timeStamp / 1000);
    }
    break;

    case UAMSG_VIEWER:
    {
        szmsg = sizeof(uamessage_viewer);
        uamessage_viewer *vwMsg = (uamessage_viewer *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(vwMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ V: Havent found robo with owner %d (%ds)\n", vwMsg->owner, yw->timeStamp / 1000);
            break;
        }

        NC_STACK_ypabact *fndbct = NULL;

        if ( vwMsg->classID == BACT_TYPES_MISSLE )
        {
            fndbct = yw_netGetMissileOfBact(bhost, vwMsg->id, vwMsg->launcher);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found weapon %d of rifleman %d (%ds)\n", vwMsg->id, vwMsg->launcher, yw->timeStamp / 1000);
                *err = msg->senderName;
                break;
            }
        }
        else
        {
            fndbct = NetGetBactByID(bhost, vwMsg->id);
            if ( !fndbct )
            {
                log_netlog("\n+++ V: Havent found vehicle %d (%ds)\n", vwMsg->id, yw->timeStamp / 1000);
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

        if ( yw->GameShell->players[owner].isKilled )
        {
            log_netlog("\n+++SG: received sync of a dead player %s\n", msg->senderName.c_str());
            break;
        }

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(sgMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SG: Havent found robo with owner %d (%ds)\n", sgMsg->owner, yw->timeStamp / 1000);
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

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(hdMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", hdMsg->owner, yw->timeStamp / 1000);
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

        if ( yw->GameShell->netPlayerOwner != bhost->_owner )
        {
            int nwee = 0;
            int wee = 0;
            for ( NC_STACK_ypabact* &bnd : yw->_unitsList )
            {
                if ( bnd->_bact_type == BACT_TYPES_ROBO &&
                        yw->GameShell->netPlayerOwner == bnd->_owner &&
                        bnd->_status != BACT_STATUS_DEAD )
                    wee = 1;

                if ( bnd->_bact_type == BACT_TYPES_ROBO &&
                        yw->GameShell->netPlayerOwner != bnd->_owner &&
                        bnd->_status != BACT_STATUS_DEAD )
                    nwee = 1;
            }

            if ( wee == 1 && nwee == 0 )
            {
                yw_arg159 arg159;
                arg159.field_4 = 10;
                arg159.field_C = 91;
                arg159.txt = 0;
                arg159.unit = 0;
                yw->ypaworld_func159(&arg159);

                yw->field_81CB.field_0 = 1;
                yw->field_81CB.field_4 = yw->timeStamp;
            }
            else
            {
                if ( !hdMsg->killerOwner )
                {
                    yw->field_81CB.field_0 = 4;
                    yw->field_81CB.field_4 = yw->timeStamp;

                    strncpy(yw->field_81CB.field_8, msg->senderName.c_str(), sizeof(yw->field_81CB.field_8) - 1);
                }
                else if ( hdMsg->killerOwner == yw->GameShell->netPlayerOwner )
                {
                    yw->field_81CB.field_0 = 2;
                    yw->field_81CB.field_4 = yw->timeStamp;
                    strncpy(yw->field_81CB.field_8, msg->senderName.c_str(), sizeof(yw->field_81CB.field_8) - 1);
                }
                else
                {
                    yw->field_81CB.field_0 = 3;
                    yw->field_81CB.field_4 = yw->timeStamp;
                    strncpy(yw->field_81CB.field_8, msg->senderName.c_str(), sizeof(yw->field_81CB.field_8) - 1);

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

        if ( hdMsg->killer && hdMsg->killerOwner == yw->UserRobo->_owner )
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

        if ( yw->GameShell->players[owner].isKilled )
            break;

        windp_arg79 plDat;
        plDat.mode = 0;
        plDat.ID = 0;

        while( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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
                        std::string bff = fmt::sprintf("%s: %s", msg->senderName.c_str(), msgMsg->message);

                        yw_arg159 arg159;
                        arg159.txt = bff.c_str();
                        arg159.field_C = 93;
                        arg159.field_4 = 10;
                        arg159.unit = NULL;

                        yw->ypaworld_func159(&arg159);
                    }
                    else
                    {
                        sub_4D0C24(yw, msg->senderName.c_str(), msgMsg->message);
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

        if ( !StriCmp(kcMsg->text, yw->GameShell->callSIGN) )
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

        if ( !StriCmp(kcMsg->text, yw->GameShell->callSIGN) )
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

        MapGem *gemProt = &yw->_Gems[upMsg->upgradeID];
        cellArea *cl = &yw->_cells[yw->_mapWidth * gemProt->SecY + gemProt->SecX];

        int vhcl, bld;
        sub_47C1EC(yw, gemProt, &vhcl, &bld);

        if ( vhcl )
            yw->VhclProtos[vhcl].disable_enable_bitmask = 0;

        if ( bld )
            yw->BuildProtos[bld].EnableMask = 0;
      
        uint16_t lastVhcl = 0;
        uint16_t lastBuild = 0;

        switch ( yw->GameShell->netPlayerOwner )
        {
        case 1:
            lastVhcl = gemProt->NwVprotoNum1;
            lastBuild = gemProt->NwBprotoNum1;
            break;
        case 6:
            lastVhcl = gemProt->NwVprotoNum2;
            lastBuild = gemProt->NwBprotoNum2;
            break;
        case 3:
            lastVhcl = gemProt->NwVprotoNum3;
            lastBuild = gemProt->NwBprotoNum3;
            break;
        case 4:
            lastVhcl = gemProt->NwVprotoNum4;
            lastBuild = gemProt->NwBprotoNum4;
            break;
        case 2:
        case 5:
            lastBuild = 0;
            lastVhcl = 0;
            break;
        }

        yw->HistoryEventAdd( World::History::Upgrade(gemProt->SecX, gemProt->SecY, owner, gemProt->Type, lastVhcl, 0, lastBuild) );

        if ( cl->w_type != 4 )
        {
            std::string bff = get_lang_string(yw->string_pointers_p2, 229, "TECH-UPGRADE LOST! ");
            bff += gemProt->MsgDefault;

            yw_arg159 arg159;
            arg159.field_4 = 80;
            arg159.txt = bff.c_str();
            arg159.field_C = 29;
            arg159.unit = NULL;

            yw->ypaworld_func159(&arg159);

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

        while ( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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

        while ( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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

        yw->ypaworld_func181(&nMsg);
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

        while(yw->windp->GetPlayerData(&plDat))
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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

        yw_netSendUpdate(yw, yw->GameShell->netPlayerOwner, msg->senderName.c_str());
    }
    break;

    case UAMSG_UPDATE:
    {
        uamessage_update *updMsg = (uamessage_update *)msg->data;
        szmsg = updMsg->sz;

        if ( yw->GameShell->players[owner].isKilled )
        {
            log_netlog("\n+++UPD: got update from DEAD %s (%d)", msg->senderName.c_str(), yw->timeStamp / 1000);
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

        if ( !yw->ypaworld_func130(&secInfo) )
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
                    curUnit->_owner == yw->UserRobo->_owner)
                curUnit->ApplyImpulse(&impls);
        }
    }
    break;

    case UAMSG_LOGMSG:
    {
        szmsg = sizeof(uamessage_logmsg);
        uamessage_logmsg *lgMsg = (uamessage_logmsg *)msg->data;

        if ( yw->GameShell->players[owner].isKilled )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(lgMsg->senderOwner);
        if ( !bhost )
        {
            log_netlog("\n+++ RD: Havent found robo with owner %d (%ds)\n", lgMsg->senderOwner, yw->timeStamp / 1000);
            break;
        }

        if ( bhost != yw->UserRobo )
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

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(ordMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ NORG: Havent found robo with owner %d (%ds)\n", ordMsg->owner, yw->timeStamp / 1000);
            break;
        }

        //yw_netGetUnitsCount(bhost->bact); // Not used

        NC_STACK_ypabact *fnd = yw_netFindReorderUnit(bhost, ordMsg->comm);
        if ( !fnd )
        {
            log_netlog("\n+++ NORG: Commander %d of owner %d not found (%ds)\n", ordMsg->comm, owner, yw->timeStamp / 1000);
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
        yw->GameShell->netLevelName = get_lang_string(yw->string_pointers_p2, 1800 + yw->GameShell->netLevelID, yw->LevelNet->mapInfos[yw->GameShell->netLevelID].map_name.c_str());

        windp_arg79 plDat;
        plDat.ID = 0;
        plDat.mode = 0;


        while ( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(plDat.name, lobbyMsg->hostName) )
            {
                yw->GameShell->players2[plDat.ID].rdyStart = 1;
                break;
            }
            plDat.ID++;
        }

        mapINFO *minf = &yw->LevelNet->mapInfos[lobbyMsg->lvlID];
        int tmp = yw->windp->CountPlayers(NULL);

        plDat.ID = 0;
        plDat.mode = 0;
        if ( minf->fractions_mask & 2 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_RESISTANCE;

                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
        yw->GameShell->netLevelName = get_lang_string(yw->string_pointers_p2, 1800 + yw->GameShell->netLevelID, yw->LevelNet->mapInfos[yw->GameShell->netLevelID].map_name.c_str());

        for (int i = 0; i < 4; i++)
        {
            yw->GameShell->players2[i].checksum = 0;
        }

        mapINFO *minf = &yw->LevelNet->mapInfos[stlvlMsg->lvlID];
        int tmp = yw->windp->CountPlayers(NULL);

        windp_arg79 plDat;
        plDat.ID = 0;
        plDat.mode = 0;

        if ( minf->fractions_mask & 2 )
        {
            if ( tmp )
            {
                yw->GameShell->FreeFraction &= ~FREE_FRACTION_RESISTANCE;
                yw->GameShell->players2[plDat.ID].Fraction = FREE_FRACTION_RESISTANCE;

                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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
                yw->windp->GetPlayerData(&plDat);

                if ( !StriCmp(plDat.name, yw->GameShell->callSIGN) )
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

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(plasmaMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ SP: Havent found robo with owner %d (%ds)\n", plasmaMsg->owner, yw->timeStamp / 1000);
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

        bbact->_vp_extra[0].vp.base = bbact->_vp_genesis.base;
        bbact->_vp_extra[0].vp.trigo = bbact->_vp_genesis.trigo;
        bbact->_vp_extra[0].flags |= EVPROTO_FLAG_ACTIVE | EVPROTO_FLAG_SCALE;

        bbact->_vp_extra_mode = 1;
    }
    break;

    case UAMSG_ENDPLASMA:
    {
        szmsg = sizeof(uamessage_endPlasma);
        uamessage_endPlasma *endpMsg = (uamessage_endPlasma *)msg->data;

        if ( yw->GameShell->players[owner].isKilled || (plr && plr->w84upd) )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(endpMsg->owner);

        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", endpMsg->owner, yw->timeStamp / 1000);
            break;
        }

        if ( bhost != yw->UserRobo )
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

        if ( yw->GameShell->players[owner].isKilled )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(stbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ STB: Havent found robo with owner %d (%ds)\n", stbmMsg->owner, yw->timeStamp / 1000);
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

        if ( yw->GameShell->players[owner].isKilled )
            break;

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(enbmMsg->owner);
        if ( !bhost )
        {
            log_netlog("\n+++ EP: Havent found robo with owner %d (%ds)\n", enbmMsg->owner, yw->timeStamp / 1000);
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

        if ( yw->GameShell->players[owner].isKilled && exitMsg->norm )
            break;

        if ( exitMsg->norm )
        {
            log_netlog(">>> received ANNOUNCEQUIT from %s at %d\n", msg->senderName.c_str(), yw->timeStamp / 1000);
            yw->GameShell->players[owner].status = 3;
        }

        NC_STACK_ypabact *bhost = yw->yw_getHostByOwner(exitMsg->owner);

        if ( !bhost || bhost->_status == BACT_STATUS_DEAD )
        {
            if ( exitMsg->norm )
                break;
        }

        yw->field_81CB.field_0 = 5;
        yw->field_81CB.field_4 = yw->timeStamp;
        strcpy(yw->field_81CB.field_8, msg->senderName.c_str());

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

        yw->ypaworld_func159(&arg159);
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

        while ( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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
        nMsg.recvID = msg->senderName.c_str();
        nMsg.garant = 1;

        yw->ypaworld_func181(&nMsg);

        if ( yw->field_1b24.frameTime >= 3 )
            break;

        windp_arg82 warg82;
        warg82.senderFlags = 1;
        warg82.senderID = yw->GameShell->callSIGN.c_str();
        warg82.receiverID = 0;
        warg82.guarant = 0;
        warg82.receiverFlags = 2;

        yw->windp->FlushBuffer(warg82);
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

            yw->ypaworld_func181(&nMsg);
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

        while ( yw->windp->GetPlayerData(&plDat) )
        {
            if ( !StriCmp(msg->senderName, plDat.name) )
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
    NC_STACK_ypaworld *ywo = usr->p_ypaworld;
    NC_STACK_ypabact *found = NULL;

    if ( ywo->timeStamp - usr->deadCheck >= 100000 )
    {
        usr->deadCheck = ywo->timeStamp;

        for ( NC_STACK_ypabact* &station : ywo->_unitsList )
        {
            if ( station->_owner != 0 && station->_owner != usr->netPlayerOwner )
            {
                for ( NC_STACK_ypabact* &comm : station->_kidList )
                {
                    for ( NC_STACK_ypabact* &unit : comm->_kidList )
                    {
                        if ( ywo->timeStamp - unit->_lastFrmStamp > 180000 )
                        {
                            found = unit;
                            break;
                        }
                    }

                    if ( found )
                        break;

                    if ( ywo->timeStamp - comm->_lastFrmStamp > 180000 )
                    {
                        found = comm;
                        break;
                    }
                }

                if ( found )
                    break;

                if ( ywo->timeStamp - station->_lastFrmStamp > 180000 )
                {
                    found = station;
                    break;
                }
            }

        }

        if ( found )
        {
            log_netlog("\n+++ CC: found old vehicle id %d, class %d, owner %d at time %d. Request update\n", found->_gid, found->_bact_type, found->_owner, ywo->timeStamp / 1000);
            return usr->players[found->_owner].name;
        }
    }
    return NULL;
}

void yw_HandleNetMsg(NC_STACK_ypaworld *yw)
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

            yw->ypaworld_func181(&ywMsg);
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
                yw->ypaworld_func181(&ywMsg);

                usr->sendScore = 1500;
            }
            else
            {
                usr->sendScore -= usr->frameTime;
            }

        }
    }

    uint32_t msgcount = 0;
    std::string err_sender;

    windp_recvMsg recvMsg;

    while ( yw->windp->Recv(&recvMsg) )
    {
        if (recvMsg.msgType != RECVMSG_NONE)
        {
            usr->netrecv_count += recvMsg._data.size();

            msgcount++;

            if ( yw->_levelInfo->State != 2 || !yw->netGameStarted )
            {
                if ( yw->windp->CountPlayers(NULL) * 5 >= msgcount )
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
                            strncpy(lbyMsg.hostName, usr->callSIGN.c_str(), 64);
                            lbyMsg.msgID = UAMSG_LOBBYINIT;
                            lbyMsg.lvlID = usr->netLevelID;
                            lbyMsg.owner = 0;

                            yw_arg181 ywMsg;
                            ywMsg.data = &lbyMsg;
                            ywMsg.dataSize = sizeof(lbyMsg);
                            ywMsg.garant = 1;
                            ywMsg.recvFlags = 2;
                            ywMsg.recvID = 0;
                            yw->ypaworld_func181(&ywMsg);
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
                    ywMsg.recvID = (char *)recvMsg.senderName.c_str();
                    yw->ypaworld_func181(&ywMsg);

                    if ( usr->netLevelID )
                        yw->SendCRC(usr->netLevelID);

                    int plid = yw->windp->getNumPlayers();

                    
                    strncpy(usr->players2[plid - 1].name, recvMsg.senderName.c_str(), 64);
                    usr->players2[plid - 1].welcmd = 1;
                }
                break;

                case RECVMSG_DESTROYPLAYER:
                {
                    bool itisI = false;
                    bool plFound = false;

                    if ( !StriCmp(usr->callSIGN, recvMsg.senderName) )
                    {
                        itisI = true;
                        yw->_levelInfo->State = 2;
                    }

                    for(int i = 0; i < 4; i++)
                    {
                        if ( !StriCmp(usr->players2[i].name, recvMsg.senderName) )
                        {
                            for (int j = i; j < 3; j++)
                            {
                                usr->players2[j] = usr->players2[ j + 1 ];
                            }

                            plFound = true;
                            break;
                        }
                    }

                    log_netlog(">>> Received a destroy player for %s at %d\n", recvMsg.senderName.c_str(), yw->timeStamp / 1000);

                    if ( usr->field_0x0 == 0 && plFound)
                    {
                        for (int i = 0; i < 8; i++)
                        {
                            if ( !StriCmp(usr->players[i].name, recvMsg.senderName) )
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

                        yw_cleanPlayer(yw, recvMsg.senderName.c_str(), 0, 0);
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
                        yw->windp->LockSession(&v70);

                        usr->rdyStart = 1;
                        usr->blocked = 0;

                        windp_arg79 plData;
                        plData.ID = 0;
                        plData.mode = 0;
                        while ( yw->windp->GetPlayerData(&plData) )
                        {
                            if ( !StriCmp(usr->callSIGN, plData.name) )
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

                    const char *crpt = yw_corruptionCheck(usr);
                    if ( crpt )
                        err_sender = crpt;
                }
                break;

                default:
                    break;
                }

                if ( !err_sender.empty() )
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (StriCmp(usr->players2[i].name, err_sender) == 0)
                        {
                            if ( !usr->players2[i].w84upd )
                            {
                                log_netlog("Drastic Error: Request Update from %s\n", err_sender.c_str());

                                uamessage_requpdate updMsg;
                                updMsg.msgID = UAMSG_REQUPDATE;
                                updMsg.owner = usr->netPlayerOwner;

                                yw_arg181 ywMsg;
                                ywMsg.data = &updMsg;
                                ywMsg.dataSize = sizeof(updMsg);
                                ywMsg.recvFlags = 1;
                                ywMsg.garant = 1;
                                ywMsg.recvID = err_sender.c_str();
                                yw->ypaworld_func181(&ywMsg);

                                usr->players2[i].w84upd = 2000;
                            }
                            break;
                        }
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



bool NC_STACK_ypaworld::yw_NetSetHostStations(const std::vector<MapRobo> &Robos)
{
    _levelInfo->OwnerMask = 0;
    _levelInfo->UserMask = 0;

    for (int i = 0; i < 8; i++)
        GameShell->players[i].rdyStart = 0;

    const MapRobo *selHost = NULL;

    int owner = 0;

    windp_arg79 plData;
    plData.ID = 0;
    plData.mode = 0;

    while ( windp->GetPlayerData(&plData) )
    {
        if ( (size_t)plData.ID == Robos.size() )
        {
            log_netlog("No Robo declared for player %d\n", Robos.size());
            return false;
        }

        int selFraction;

        if ( plData.flags & 1 )
        {
            selFraction = GameShell->SelectedFraction;
        }
        else
        {
            selFraction = GameShell->players2[plData.ID].Fraction;
        }

        if ( !selFraction )
        {
            log_netlog("Error no race for robo %s found\n", plData.name.c_str());
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
        GameShell->players2[plData.ID].owner = owner;

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

        strncpy(GameShell->players[owner].name, plData.name.c_str(), 64);

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

        NC_STACK_ypabact *hostObj = ypaworld_func146(&arg146);

        if ( !hostObj )
        {
            log_netlog("NetPlaceRobos: Unable to create robo (owner %d, type %d)\n", plData.ID + 1, arg146.vehicle_id);
            return false;
        }

        hostObj->_owner = owner;
        hostObj->_gid |= owner << 24;

        _levelInfo->OwnerMask |= 1 << owner;

        for (NC_STACK_ypabact* &bct :  hostObj->_kidList)
        {
            bct->_gid |= owner << 24;
            bct->_owner = owner;
        }

        if ( plData.flags & 1 )
        {
            hostObj->setBACT_viewer(1);
            hostObj->setBACT_inputting(1);
            GameShell->netPlayerOwner = owner;
            _levelInfo->UserMask |= 1 << owner;
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

        plData.ID++;
    }
    return true;
}



size_t NC_STACK_ypaworld::ypaworld_func179(yw_arg161 *arg)
{
    bact_id = 0x10000;

    LevelDesc proto;
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
                            for (int y = 0; y < _mapHeight; y++)
                            {
                                for (int x = 0; x < _mapWidth; x++)
                                {
                                    cellArea *cell = &_cells[x + y * _mapWidth];
                                    CellCheckHealth(cell, x, y, 255, NULL);
                                }
                            }

                            yw_InitTechUpgradeBuildings();
                            InitGates();
                            InitSuperItems();
                            UpdatePowerEnergy();

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

    if ( copyof_typemap )
    {
        delete copyof_typemap;
        copyof_typemap = 0;
    }

    if ( copyof_ownermap )
    {
        delete copyof_ownermap;
        copyof_ownermap = 0;
    }

    if ( typ_map )
        copyof_typemap = typ_map->Copy();

    if ( own_map )
        copyof_ownermap = own_map->Copy();

    GameShell->msgcount = 0;

    for (int i = 0; i < 4; i++)
    {
        GameShell->players2[i].msg[0] = 0;
        GameShell->players2[i].trbl = 0;
        GameShell->players2[i].w84upd = 0;
    }

    for (int i = 0; i < 8; i++)
    {
        GameShell->players[i].isKilled = 0;
        GameShell->players[i].rdyStart = 0;
        GameShell->players[i].lastMsgTime = timeStamp + 1000;
        GameShell->players[i].tstamp = 0;
        GameShell->players[i].status = 0;
        GameShell->players[i].problemCnt = 0;
    }

    for ( NC_STACK_ypabact* &bct : _unitsList )
        GameShell->players[ bct->_owner ].status = 1;

    NC_STACK_ypabact *fndHost = NULL;
    for ( NC_STACK_ypabact* &host : _unitsList )
    {
        if (host->_owner == GameShell->netPlayerOwner)
        {
            fndHost = host;
            break;
        }
    }

    GameShell->players[ GameShell->netPlayerOwner ].rdyStart = 1;
    netGameStarted = 0;

    netStartTime = ypaworld_keys[1].Get<int>();
    GameShell->kickTime = ypaworld_keys[2].Get<int>();

    log_netlog("netstarttime was initialized with %d sec\n", netStartTime / 1000);
    log_netlog("kickoff was initialized with      %d sec\n", GameShell->kickTime / 1000);

    if ( fndHost )
    {
        uamessage_syncgame syncMsg;
        syncMsg.owner = GameShell->netPlayerOwner;
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

        yw_arg181 ywMsg;
        ywMsg.data = &syncMsg;
        ywMsg.dataSize = sizeof(syncMsg);
        ywMsg.recvFlags = 2;
        ywMsg.recvID = 0;
        ywMsg.garant = 1;
        ypaworld_func181(&ywMsg);

        windp_arg82 windp82;
        windp82.senderFlags = 1;
        windp82.receiverFlags = 2;
        windp82.receiverID = 0;
        windp82.senderID = GameShell->callSIGN.c_str();
        windp82.guarant = 1;

        windp->FlushBuffer(windp82);

        log_netlog("Sent a READY TO PLAY to all for my Owner %d\n", GameShell->netPlayerOwner);

        fndHost->setBACT_viewer(1);
    }

    GameShell->netsend_count = 0;
    GameShell->netrecv_count = 0;
    GameShell->netrecv_time = 0;
    GameShell->netsend_speed = 0;
    GameShell->netrecv_speed = 0;
    GameShell->netrecv_bpsMin = 0;
    GameShell->netrecv_bpsMax = 0;
    GameShell->netrecv_bpsAvr = 0;
    GameShell->netsend_bpsMin = 0;
    GameShell->netsend_bpsMax = 0;
    GameShell->netsend_bpsAvr = 0;
    GameShell->net_gcnt = 0;
    GameShell->net_packet_size = 0;
    GameShell->net_packet_min = 0;
    GameShell->net_packet_max = 0;
    GameShell->net_packet_cnt = 0;
    GameShell->net_packet_avr = 0;
    GameShell->deadCheck = timeStamp + 300000;
    GameShell->sendScore = 3000;

    for (auto &x : playerstatus)
        x.clear();

    return 1;
}


void yw_NetHandleProblems(NC_STACK_ypaworld *yw)
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

                yw->ypaworld_func181(&arg181);
            }
        }
        break;

    case 3:
        usr->netProblemCount -= yw->field_1b24.frameTime;
        if ( usr->netProblemCount < 0 )
        {
            yw->_levelInfo->State = 2;
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

int yw_NetCheckPlayers(NC_STACK_ypaworld *yw)
{
    int v2 = 0;
    UserData *usr = yw->GameShell;

    windp_arg79 arg79;
    arg79.ID = 0;
    arg79.mode = 0;
    while ( yw->windp->GetPlayerData(&arg79) )
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
                if ( StriCmp(usr->players[i].name, arg79.name.c_str()) == 0 )
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
                        log_netlog("Waiting for player %s. (time %ds at %ds)\n", arg79.name.c_str(), tm / 1000, yw->timeStamp / 1000);
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
                            yw->ypaworld_func181(&yw181);

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
                log_netlog("Warning: No Playerdata for player %s in PlayersOK() (%ds)\n", arg79.name.c_str(), yw->timeStamp / 1000);
                arg79.ID++;
            }
        }
    }

    if ( usr->netProblem == 5 && !v2 )
        usr->netProblem = 0;

    return 1;
}

int yw_NetCheckPlayersInGame(NC_STACK_ypaworld *yw)
{
    UserData *usr = yw->GameShell;

    int numpl = yw->windp->CountPlayers(NULL);

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

    InputState inpt;
    inpt.Clear();
    INPe.QueryInput(&inpt);

    char buf[1024];
    char *cur = buf;

    FontUA::select_tileset(&cur, 0);
    FontUA::set_xpos(&cur, 0);
    FontUA::set_ypos(&cur, 0);
    FontUA::copy_position(&cur);
    FontUA::add_txt(&cur, 2 * yw->screen_width / 3 - 1, 1,  get_lang_string(ypaworld__string_pointers, 651, "WAITING FOR PLAYERS: ") );
    FontUA::next_line(&cur);

    yw->netStartTime -= inpt.Period;
    if ( yw->netStartTime > 0 )
    {
        std::string timeStr = fmt::sprintf("(%d)", yw->netStartTime / 1000);

        FontUA::copy_position(&cur);
        FontUA::add_txt(&cur, yw->screen_width / 3 - 1, 1, timeStr);
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

                yw->ypaworld_func181(&yw181);

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
                std::string tmpstr = "   ";
                tmpstr += wting[i]->name;

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

    yw->_win3d->BeginFrame();

    w3d_a209 arg209;
    arg209.cmdbuf = buf;
    arg209.includ = NULL;
    yw->_win3d->raster_func209(&arg209);

    yw->_win3d->EndFrame();

    return 0;
}


void yw_NetCheckPing(NC_STACK_ypaworld *yw)
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

        yw->ypaworld_func181(&yw181);

        if ( yw->field_1b24.frameTime < 3 )
        {
            windp_arg82 dp82;
            dp82.senderFlags = 1;
            dp82.receiverID = 0;
            dp82.senderID = usr->callSIGN.c_str();
            dp82.guarant = 0;
            dp82.receiverFlags = 2;
            yw->windp->FlushBuffer(dp82);
        }
    }
    else
    {
        usr->latencyCheck -= yw->field_1b24.frameTime;
    }
}


void yw_NetMsgHndlLoop(NC_STACK_ypaworld *yw)
{
    while ( 1 )
    {
        if ( yw->GameShell->netProblem != 0 )
            yw_NetHandleProblems(yw);

        yw->GameShell->netAllOkCount -= yw->field_1b24.frameTime;

        yw_HandleNetMsg(yw);

        if ( yw->_levelInfo->State == 2 )
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


void yw_NetDrawStats(NC_STACK_ypaworld *yw)
{
    bool toDraw = false;

    std::string t[16];

    UserData *usr = yw->GameShell;

    TileMap *font29 = yw->tiles[29];
    char drawbuf[2000];
    char *cur = drawbuf;

    FontUA::select_tileset(&cur, 29);
    FontUA::set_center_xpos(&cur, font29->h - (yw->screen_width / 2) );
    FontUA::set_center_ypos(&cur, font29->h - (yw->screen_height / 2) );

    if ( usr->disconnected == 0 && usr->problemCnt > 0 )
    {
        if ( (yw->field_1b24.gTime / 300) & 1 )
        {
            FontUA::store_u8(&cur, 65);
        }
    }

    FontUA::set_end(&cur);

    w3d_a209 v77;
    v77.cmdbuf = drawbuf;
    v77.includ = NULL;

    yw->_win3d->raster_func209(&v77);

    int numelm = 0;

    if ( usr->netProblem )
    {
        toDraw = true;

        switch ( usr->netProblem )
        {
        case 1:
            if ( usr->isHost )
            {
                t[0] = get_lang_string(ypaworld__string_pointers, 2405, "HOST: LATENCY PROBLEMS.");
                t[1] = fmt::sprintf("%s %d", get_lang_string(ypaworld__string_pointers, 2423, "PLEASE WAIT"), usr->netProblemCount);
                numelm = 2;
            }
            else
            {
                t[0] = get_lang_string(ypaworld__string_pointers, 2406, "CLIENT: LATENCY PROBLEMS.");
                t[1] = fmt::sprintf("%s %d", get_lang_string(ypaworld__string_pointers, 2424, "PLEASE WAIT"), usr->netProblemCount);
                numelm = 2;
            }
            break;

        case 3:
            t[0] = get_lang_string(ypaworld__string_pointers, 2425, "YOU ARE KICKED OFF BECAUSE NETTROUBLE");
            t[1] = fmt::sprintf("%s %d", get_lang_string(ypaworld__string_pointers, 2426, "LEVEL FINISHES AUTOMATICALLY"), usr->netProblemCount / 1000);
            numelm = 2;
            break;

        case 4:
            t[0] = get_lang_string(ypaworld__string_pointers, 2427, "FOLLOWING PLAYERES WERE REMOVED");
            t[1] = fmt::sprintf(get_lang_string(ypaworld__string_pointers, 2428, "BECAUSE THEY HAD NETWORK PROBLEMS"));
            numelm = 2;

            for(int i = 0; i < 8; i++)
            {
                if (usr->players[i].isKilled & 2)
                {
                    t[numelm] = usr->players[i].name;
                    numelm++;
                }
            }
            break;

        case 5:
            t[0] = get_lang_string(ypaworld__string_pointers, 2429, "NO CONNECTION TO FOLLOWING PLAYERS");
            t[1] = fmt::sprintf(get_lang_string(ypaworld__string_pointers, 2430, "FINISH IF PROBLEM CANNOT NE SOLVED"));
            numelm = 2;

            for(int i = 0; i < 8; i++)
            {
                if ( (yw->field_1b24.gTime - usr->players[i].lastMsgTime) > 20000 )
                {
                    if ( usr->players[i].rdyStart )
                    {
                        if ( StriCmp(usr->players[i].name, usr->callSIGN) )
                        {
                            t[numelm] = usr->players[i].name;
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
                t[0] = get_lang_string(ypaworld__string_pointers, 2409, "NETWORK IS NOW OK");
            }
            else if ( usr->netAllOk == 2 )
            {
                t[0] = get_lang_string(ypaworld__string_pointers, 2408, "THERE WAS NO CHANCE TO SOLVE THIS PROBLEM");
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
        TileMap *font0 = yw->tiles[0];
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
            FontUA::op17(&cur, yw->screen_width / 2 - font0->map['}'].w);

            FontUA::store_u8(&cur, ' ');
            FontUA::store_u8(&cur, '}');

            FontUA::add_txt(&cur, yw->screen_width / 2 - 2 * font0->map['W'].w, 4, t[i] );

            FontUA::next_line(&cur);
        }

        FontUA::set_yoff(&cur, font0->h - 1);

        FontUA::store_u8(&cur, 'x');
        FontUA::op17(&cur, yw->screen_height / 2 - font0->map['z'].w);
        FontUA::store_u8(&cur, 'y');
        FontUA::store_u8(&cur, 'z');

        FontUA::set_end(&cur);

        w3d_a209 v77_1;
        v77_1.cmdbuf = drawbuf;
        v77_1.includ = NULL;

        yw->_win3d->raster_func209(&v77_1);
    }
}
