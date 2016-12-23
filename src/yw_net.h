#ifndef  YWNET_H_INCLUDED
#define YWNET_H_INCLUDED

#include "yw.h"


enum RECVMSG
{
    RECVMSG_NORMAL = 0,
    RECVMSG_CREATEPLAYER = 1,
    RECVMSG_DESTROYPLAYER = 2,
    RECVMSG_HOST = 3,
    RECVMSG_SYSUNK = 4,
    RECVMSG_SETSESSIONDESC = 5
};

enum UAMSG
{
    UAMSG_BASE = 1000,
    UAMSG_VHCLDATA_I = UAMSG_BASE + 6,
    UAMSG_VHCLDATA_E = UAMSG_BASE + 7,
    UAMSG_SYNCGM = UAMSG_BASE + 15,
    UAMSG_WELCOME = UAMSG_BASE + 22,
    UAMSG_REQUPDATE = UAMSG_BASE + 24,
    UAMSG_LOBBYINIT = UAMSG_BASE + 29,
    UAMSG_SETLEVEL = UAMSG_BASE + 36,
    UAMSG_CRC = UAMSG_BASE + 37,
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


struct vhcldata
{
    int16_t pos_x;
    int16_t pos_y;
    int16_t pos_z;
    int16_t vp;
    uint8_t roll;
    uint8_t pitch;
    uint8_t yaw;
    uint8_t specialinfo;
    uint32_t ident;
    int32_t energy;
};

struct vhcldataE : vhcldata
{
    xyz speed;
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

struct uamessage_syncgame : uamessage_base
{
    uint32_t hostID;
    uint32_t gun[ 8 ];
};

struct uamessage_welcome : uamessage_base
{
    int16_t fraction;
    uint8_t rdy;
    uint8_t cd;
};

typedef uamessage_base uamessage_requpdate;

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

struct uamessage_cd : uamessage_base
{
    uint8_t cd;
    int8_t rdy;
    uint8_t cd_p[2];
};

struct uamessage_score : uamessage_base
{
    player_status status[8];
};



#endif
