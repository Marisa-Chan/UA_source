#ifndef ZNDNET_TYPES_H_INCLUDED
#define ZNDNET_TYPES_H_INCLUDED

namespace ZNDNet
{

enum HDR_OFF
{
    HDR_OFF_FLAGS       = 0,

    HDR_OFF_SYS_DATA    = 1, //For system short msgs

    HDR_OFF_SEQID       = 1, //SeqID
    HDR_OFF_CHANNEL     = 5,

    HDR_OFF_DATA        = 6, //Data position if not multipart

    HDR_OFF_PART_FSIZE  = 6, //Full data size
    HDR_OFF_PART_OFFSET = 10, //Offset of this chunk
    HDR_OFF_PART_DATA   = 14, //Offset of data in multipart


    HDR_OFF_SYS_MINSZ   = 2,
    HDR_OFF_MINSZ       = 7,
    HDR_OFF_PART_MINSZ  = 15,
};

enum PKT_FLAG
{
    PKT_FLAG_PART   = 0x1,
    PKT_FLAG_GARANT = 0x2,
    PKT_FLAG_ASYNC  = 0x4,
    PKT_FLAG_SYSTEM = 0x80,

    PKT_FLAG_MASK_SYSTEM = PKT_FLAG_SYSTEM,
    PKT_FLAG_MASK_NORMAL = (PKT_FLAG_PART | PKT_FLAG_GARANT | PKT_FLAG_ASYNC)
};


enum
{
    CHANNEL_SYS = 0,
    CHANNEL_USR = 1,
};


enum
{
    PKT_CHNL_NOT_SET = 0xFF,
    PKT_NO_CHANNEL = 0xFFFFFFFF,
    SES_NAME_MAX = 32,

    TIMEOUT_SRV_RECV_MAX = 10,
    TIMEOUT_CLI_RECV_MAX = 3,

    TIMEOUT_PENDING = 10000,
    TIMEOUT_PENDING_GARANT = 3300,
    TIMEOUT_GARANT = 15000,
    TIMEOUT_GARANT_MULTIPART = 3300,

    TIMEOUT_CONNECT = 10000,
    TIMEOUT_USER = 15000,
    TIMEOUT_SESSION = 60000,

    RETRY_GARANT = 2,

    DELAY_SESS_REQ = 5000,

    DELAY_PING = 5000,

    LATENCE_OLD_PART = 20,

    EVENTS_MAX = 1000,
    EVENTS_DATA_MAX = 104857600, //100 Mb for maximum data stored in events
};


}

#endif
