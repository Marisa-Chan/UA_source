#ifndef  YWNET_H_INCLUDED
#define YWNET_H_INCLUDED

#include "yw.h"


enum RECVMSG
{
    RECVMSG_NONE   = 0,
    RECVMSG_NORMAL = 1,
    RECVMSG_CREATEPLAYER = 2,
    RECVMSG_DESTROYPLAYER = 3,
    RECVMSG_HOST = 4,
    RECVMSG_SYSUNK = 5,
    RECVMSG_SETSESSIONDESC = 6
};

enum UAMSG
{
    UAMSG_BASE = 1000,
    UAMSG_LOAD = UAMSG_BASE,
    UAMSG_NEWVHCL = UAMSG_BASE + 1,
    UAMSG_DESTROYVHCL = UAMSG_BASE + 2,
    UAMSG_NEWLEADER = UAMSG_BASE + 3,
    UAMSG_NEWWEAPON = UAMSG_BASE + 4,
    UAMSG_SETSTATE = UAMSG_BASE + 5,
    UAMSG_VHCLDATA_I = UAMSG_BASE + 6,
    UAMSG_VHCLDATA_E = UAMSG_BASE + 7,

    UAMSG_DEAD = UAMSG_BASE + 9,
    UAMSG_VHCLENERGY = UAMSG_BASE + 10,
    UAMSG_SECTORENERGY = UAMSG_BASE + 11,
    UAMSG_STARTBUILD = UAMSG_BASE + 12,
    UAMSG_BUILDINGVHCL = UAMSG_BASE + 13,
    UAMSG_VIEWER = UAMSG_BASE + 14,
    UAMSG_SYNCGM = UAMSG_BASE + 15,
    UAMSG_HOSTDIE = UAMSG_BASE + 16,
    // +17 DEBUG
    UAMSG_MESSAGE = UAMSG_BASE + 18,
    UAMSG_KICK = UAMSG_BASE + 19,
    UAMSG_UPGRADE = UAMSG_BASE + 20,
    UAMSG_FRACTION = UAMSG_BASE + 21,
    UAMSG_WELCOME = UAMSG_BASE + 22,
    UAMSG_READY = UAMSG_BASE + 23,
    UAMSG_REQUPDATE = UAMSG_BASE + 24,
    UAMSG_UPDATE = UAMSG_BASE + 25,
    UAMSG_IMPULSE = UAMSG_BASE + 26,
    UAMSG_LOGMSG = UAMSG_BASE + 27,
    UAMSG_REORDER = UAMSG_BASE + 28,
    UAMSG_LOBBYINIT = UAMSG_BASE + 29,
    UAMSG_STARTPLASMA = UAMSG_BASE + 30,
    UAMSG_ENDPLASMA = UAMSG_BASE + 31,
    // +32 SET VP
    UAMSG_STARTBEAM = UAMSG_BASE + 33,
    UAMSG_ENDBEAM = UAMSG_BASE + 34,
    UAMSG_EXIT = UAMSG_BASE + 35,
    UAMSG_SETLEVEL = UAMSG_BASE + 36,
    UAMSG_CRC = UAMSG_BASE + 37,
    UAMSG_REQPING = UAMSG_BASE + 38,
    UAMSG_PONG = UAMSG_BASE + 39,
    UAMSG_STARTPROBLEM = UAMSG_BASE + 40,
    UAMSG_ENDPROBLEM = UAMSG_BASE + 41,
    //UAMSG_CD = UAMSG_BASE + 42,
    UAMSG_SCORE = UAMSG_BASE + 43
};

struct uamessage_base
{
    uint32_t msgID = 0;
    uint32_t tstamp = 0;
    uint32_t msgCnt = 0;
    uint8_t owner = 0;
    uint8_t p[3] = {0};
};

struct uamessage_load : uamessage_base
{
    uint32_t level = 0;
};

struct uamessage_newVhcl : uamessage_base
{
    vec3d pos;
    uint32_t parent = 0;
    uint32_t id = 0;
    uint32_t commandID = 0;
    uint8_t kind = 0;
    uint8_t type = 0;
    uint8_t np[2] = {0};
};

struct uamessage_destroyVhcl : uamessage_base
{
    uint32_t id = 0;
    uint8_t type = 0;
    uint8_t dp[3] = {0};
};

struct uamessage_newLeader : uamessage_base
{
    uint32_t id = 0;
    uint32_t newLeader = 0;
};

struct uamessage_newWeapon : uamessage_base
{
    vec3d pos;
    uint32_t id = 0;
    uint32_t launcher = 0;
    vec3d dir;
    uint32_t target = 0;
    uint8_t flags = 0;
    uint8_t type = 0;
    uint8_t targetType = 0;
    uint8_t targetOwner = 0;
    vec3d targetPos;
};

struct uamessage_setState: uamessage_base
{
    uint32_t id = 0;
    uint32_t setFlags = 0;
    uint32_t unsetFlags = 0;
    uint8_t newStatus = 0;
    uint8_t classID = 0;
    uint8_t sp[2] = {0};
};


struct vhcldata
{
    int16_t pos_x = 0;
    int16_t pos_y = 0;
    int16_t pos_z = 0;
    int16_t vp = 0;
    int8_t roll = 0;
    int8_t pitch = 0;
    int8_t yaw = 0;
    uint8_t specialinfo = 0;
    uint32_t ident = 0;
    int32_t energy = 0;

    enum SI_FLAGS
    {
        SI_NORENDER = 1,
        SI_LAND = 2,
        SI_YPAGUN = 4,
        SI_UNK = 8,
        SI_ROBO = 0x10,
        SI_DSETTED = 0x20
    };
};

struct vhcldataE : vhcldata
{
    vec3d speed;
};

struct vhcldatahdr
{
    uint16_t number = 0;
    uint16_t size = 0;
    uint32_t diffTime = 0;
};

struct uamessage_vhclData : uamessage_base
{
    vhcldatahdr hdr;
};

struct uamessage_vhclDataI : uamessage_vhclData
{
    vhcldata data[1024];
};

struct uamessage_vhclDataE : uamessage_vhclData
{
    vhcldataE data[1024];
};

struct uamessage_dead : uamessage_base
{
    uint32_t id = 0;
    uint32_t newParent = 0;
    uint32_t killer = 0;
    uint8_t landed = 0;
    uint8_t classID = 0;
    uint8_t killerOwner = 0;
    uint8_t dp = 0;
};


struct uamessage_vhclEnergy : uamessage_base
{
    uint32_t id = 0;
    int32_t energy = 0;
    uint32_t killer = 0;
    uint8_t killerOwner = 0;
    uint8_t ep[3] = {0};
};

struct uamessage_sectorEnergy : uamessage_base
{
    vec3d pos;
    int32_t energy = 0;
    uint32_t whoHit = 0;
    uint8_t sectOwner = 0;
    uint8_t ep[3] = {0};
};

struct uamessage_startBuild : uamessage_base
{
    uint32_t bproto = 0;
    int16_t sec_x = 0;
    int16_t sec_y = 0;
};

struct uamessage_bldVhcl : uamessage_base
{
    struct bldVhcl
    {
        uint32_t id = 0;
        vec3d base;
        vec3d pos;
        int16_t protoID = 0;
        uint8_t bldp[2] = {0};
    };

    bldVhcl vhcl[8];
};

struct uamessage_viewer : uamessage_base
{
    uint32_t id = 0;
    uint32_t launcher = 0;
    uint8_t classID = 0;
    uint8_t view = 0;
    uint8_t vp[2] = {0};
};

struct uamessage_syncgame : uamessage_base
{
    uint32_t hostID = 0;
    uint32_t gun[ 8 ] = {0};
};

struct uamessage_hostDie : uamessage_base
{
    uint32_t killer = 0;
    uint8_t killerOwner = 0;
    uint8_t hdp[3] = {0};
};

struct uamessage_message : uamessage_base
{
    char message[64] = {0};
};

struct uamessage_kick : uamessage_base
{
    char text[64] = {0};
};

struct uamessage_upgrade : uamessage_base
{
    int16_t upgradeID = 0;
    int16_t enable = 0;
};

struct uamessage_fraction : uamessage_base
{
    int16_t freefrac = 0;
    int16_t newfrac = 0;
};

struct uamessage_welcome : uamessage_base
{
    int16_t fraction = 0;
    uint8_t rdy = 0;
    //uint8_t cd;
};

struct uamessage_ready : uamessage_base
{
    uint8_t rdy = 0;
    uint8_t rp[3] = {0};
};

typedef uamessage_base uamessage_requpdate;

struct vhclUpdData
{
    uint8_t type = 0;
    uint8_t status = 0;
    uint8_t vhclID = 0;
    int8_t rot_x = 0;
    int8_t rot_y = 0;
    int8_t rot_z = 0;
    uint8_t pp[2] = {0};
    uint32_t status_flg = 0;
    uint32_t gid = 0;
    int32_t engy = 0;
    vec3d pos;
};

struct uamessage_update : uamessage_base
{
    uint32_t sz = 0;
    uint32_t num = 0;
    vhclUpdData data[1024];
};

struct uamessage_impulse : uamessage_base
{
    uint32_t id = 0;
    vec3d pos;
    int32_t impulse = 0;
    float mass = 0.0;
    vec3d dir;
    float dir_len = 0.0;
};

struct uamessage_logmsg : uamessage_base
{
    uint32_t sender = 0;
    uint8_t senderOwner = 0;
    uint8_t lgp[3] = {0};
    int32_t pri = 0;
    int32_t id = 0;
    int32_t pr1 = 0;
    int32_t pr2 = 0;
    int32_t pr3 = 0;
};

struct uamessage_reorder : uamessage_base
{
    uint32_t mode = 0;
    uint32_t comm = 0;
    uint32_t commID = 0;
    uint32_t num = 0;
    uint32_t sz = 0;
    uint32_t units[500] = {0};
};

struct uamessage_startPlasma : uamessage_base
{
    uint32_t time = 0;
    float scale = 0.0;
    uint32_t id = 0;
    vec3d pos;
    mat3x3 dir;
};

struct uamessage_endPlasma : uamessage_base
{
    uint32_t id = 0;
};

struct uamessage_startBeam : uamessage_base
{
    vec3d pos;
    uint32_t id = 0;
};

struct uamessage_endBeam : uamessage_base
{
    uint32_t id = 0;
};

struct uamessage_exit : uamessage_base
{
    uint8_t norm = 0;
    uint8_t qp[3] = {0};
};

struct uamessage_setLevel : uamessage_base
{
    char hostName[64] = {0};
    uint32_t lvlID = 0;
    uint8_t fractions[4] = {0};
};

typedef uamessage_setLevel uamessage_lobbyInit;

struct uamessage_crc : uamessage_base
{
    uint32_t checksum = 0;
};

struct uamessage_ping: uamessage_base
{
    uint32_t timestamp = 0;
};

struct uamessage_startproblem: uamessage_base
{
    uint32_t problem = 0;
};

struct uamessage_endproblem: uamessage_base
{
    uint32_t solved = 0;
};

struct uamessage_cd : uamessage_base
{
    uint8_t cd = 0;
    int8_t rdy = 0;
    uint8_t cd_p[2] = {0};
};

struct uamessage_score : uamessage_base
{
    std::array<World::TPlayerStatus, World::CVFractionsCount> status;
};



#endif
