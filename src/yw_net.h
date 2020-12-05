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
    UAMSG_CD = UAMSG_BASE + 42,
    UAMSG_SCORE = UAMSG_BASE + 43
};

struct uamessage_base
{
    uint32_t msgID;
    uint32_t tstamp;
    uint32_t msgCnt;
    uint8_t owner;
    uint8_t p[3];
};

struct uamessage_load : uamessage_base
{
    uint32_t level;
};

struct uamessage_newVhcl : uamessage_base
{
    vec3d pos;
    uint32_t parent;
    uint32_t id;
    uint32_t commandID;
    uint8_t kind;
    uint8_t type;
    uint8_t np[2];
};

struct uamessage_destroyVhcl : uamessage_base
{
    uint32_t id;
    uint8_t type;
    uint8_t dp[3];
};

struct uamessage_newLeader : uamessage_base
{
    uint32_t id;
    uint32_t newLeader;
};

struct uamessage_newWeapon : uamessage_base
{
    vec3d pos;
    uint32_t id;
    uint32_t launcher;
    vec3d dir;
    uint32_t target;
    uint8_t flags;
    uint8_t type;
    uint8_t targetType;
    uint8_t targetOwner;
    vec3d targetPos;
};

struct uamessage_setState: uamessage_base
{
    uint32_t id;
    uint32_t setFlags;
    uint32_t unsetFlags;
    uint8_t newStatus;
    uint8_t classID;
    uint8_t sp[2];
};


struct vhcldata
{
    int16_t pos_x;
    int16_t pos_y;
    int16_t pos_z;
    int16_t vp;
    int8_t roll;
    int8_t pitch;
    int8_t yaw;
    uint8_t specialinfo;
    uint32_t ident;
    int32_t energy;

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
    uint16_t number;
    uint16_t size;
    uint32_t diffTime;
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
    uint32_t id;
    uint32_t newParent;
    uint32_t killer;
    uint8_t landed;
    uint8_t classID;
    uint8_t killerOwner;
    uint8_t dp;
};


struct uamessage_vhclEnergy : uamessage_base
{
    uint32_t id;
    int32_t energy;
    uint32_t killer;
    uint8_t killerOwner;
    uint8_t ep[3];
};

struct uamessage_sectorEnergy : uamessage_base
{
    vec3d pos;
    int32_t energy;
    uint32_t whoHit;
    uint8_t sectOwner;
    uint8_t ep[3];
};

struct uamessage_startBuild : uamessage_base
{
    uint32_t bproto;
    int16_t sec_x;
    int16_t sec_y;
};

struct uamessage_bldVhcl : uamessage_base
{
    struct bldVhcl
    {
        uint32_t id;
        vec3d base;
        vec3d pos;
        int16_t protoID;
        uint8_t bldp[2];
    };

    bldVhcl vhcl[8];
};

struct uamessage_viewer : uamessage_base
{
    uint32_t id;
    uint32_t launcher;
    uint8_t classID;
    uint8_t view;
    uint8_t vp[2];
};

struct uamessage_syncgame : uamessage_base
{
    uint32_t hostID;
    uint32_t gun[ 8 ];
};

struct uamessage_hostDie : uamessage_base
{
    uint32_t killer;
    uint8_t killerOwner;
    uint8_t hdp[3];
};

struct uamessage_message : uamessage_base
{
    char message[64];
};

struct uamessage_kick : uamessage_base
{
    char text[64];
};

struct uamessage_upgrade : uamessage_base
{
    int16_t upgradeID;
    int16_t enable;
};

struct uamessage_fraction : uamessage_base
{
    int16_t freefrac;
    int16_t newfrac;
};

struct uamessage_welcome : uamessage_base
{
    int16_t fraction;
    uint8_t rdy;
    uint8_t cd;
};

struct uamessage_ready : uamessage_base
{
    uint8_t rdy;
    uint8_t rp[3];
};

typedef uamessage_base uamessage_requpdate;

struct vhclUpdData
{
    uint8_t type;
    uint8_t status;
    uint8_t vhclID;
    int8_t rot_x;
    int8_t rot_y;
    int8_t rot_z;
    uint8_t pp[2];
    uint32_t status_flg;
    uint32_t gid;
    int32_t engy;
    vec3d pos;
};

struct uamessage_update : uamessage_base
{
    uint32_t sz;
    uint32_t num;
    vhclUpdData data[1024];
};

struct uamessage_impulse : uamessage_base
{
    uint32_t id;
    vec3d pos;
    int32_t impulse;
    float mass;
    vec3d dir;
    float dir_len;
};

struct uamessage_logmsg : uamessage_base
{
    uint32_t sender;
    uint8_t senderOwner;
    uint8_t lgp[3];
    int32_t pri;
    int32_t id;
    int32_t pr1;
    int32_t pr2;
    int32_t pr3;
};

struct uamessage_reorder : uamessage_base
{
    uint32_t mode;
    uint32_t comm;
    uint32_t commID;
    uint32_t num;
    uint32_t sz;
    uint32_t units[500];
};

struct uamessage_startPlasma : uamessage_base
{
    uint32_t time;
    float scale;
    uint32_t id;
    vec3d pos;
    mat3x3 dir;
};

struct uamessage_endPlasma : uamessage_base
{
    uint32_t id;
};

struct uamessage_startBeam : uamessage_base
{
    vec3d pos;
    uint32_t id;
};

struct uamessage_endBeam : uamessage_base
{
    uint32_t id;
};

struct uamessage_exit : uamessage_base
{
    uint8_t norm;
    uint8_t qp[3];
};

struct uamessage_setLevel : uamessage_base
{
    char hostName[64];
    uint32_t lvlID;
    uint8_t fractions[4];
};

typedef uamessage_setLevel uamessage_lobbyInit;

struct uamessage_crc : uamessage_base
{
    uint32_t checksum;
};

struct uamessage_ping: uamessage_base
{
    uint32_t timestamp;
};

struct uamessage_startproblem: uamessage_base
{
    uint32_t problem;
};

struct uamessage_endproblem: uamessage_base
{
    uint32_t solved;
};

struct uamessage_cd : uamessage_base
{
    uint8_t cd;
    int8_t rdy;
    uint8_t cd_p[2];
};

struct uamessage_score : uamessage_base
{
    World::player_status status[8];
};



#endif
