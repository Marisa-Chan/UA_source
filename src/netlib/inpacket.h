#ifndef ZNDNET_INPKT_H_INCLUDED
#define ZNDNET_INPKT_H_INCLUDED

#include <list>

#include "types.h"
#include "addrseq.h"
#include "utils.h"

namespace ZNDNet
{

struct InRawPktHdr
{
    uint8_t   flags;
    uint32_t  fsize;    //Full size
    uint32_t  offset;   //Offset
    uint32_t  seqid;
    uint8_t  *data;
    size_t    datasz;
    uint8_t   uchnl;

    InRawPktHdr()
    : flags(0), fsize(0), offset(0), seqid(0)
    , data(NULL), datasz(0), uchnl(PKT_CHNL_NOT_SET)
    {}

    bool Parse(uint8_t *_data, size_t len)
    {
        if (!_data)
            return false; // No data

        flags = _data[HDR_OFF_FLAGS];

        if (flags & PKT_FLAG_SYSTEM)
        {
            if (flags & (~PKT_FLAG_MASK_SYSTEM)) // Only system flags must be setted
                return false;

            if (len < HDR_OFF_SYS_MINSZ)
                return false;

            data = &_data[HDR_OFF_SYS_DATA];
            datasz = len - HDR_OFF_SYS_DATA;
        }
        else
        {
            // Incorrect flags
            if ( (flags & (~PKT_FLAG_MASK_NORMAL)) )
                return false;

            if (len < HDR_OFF_SEQID + 4)
                return false;

            seqid  = readU32( &_data[HDR_OFF_SEQID] );
            uchnl  = _data[HDR_OFF_CHANNEL];

            if (uchnl >= ZNDNET_USER_SCHNLS)
                uchnl = PKT_CHNL_NOT_SET;

            if (flags & PKT_FLAG_PART)
            {
                if (len < HDR_OFF_PART_MINSZ)
                    return false;

                fsize  = readU32( &_data[HDR_OFF_PART_FSIZE] );
                offset = readU32( &_data[HDR_OFF_PART_OFFSET] );
                data   = &_data[HDR_OFF_PART_DATA];
                datasz = len - HDR_OFF_PART_DATA;
            }
            else
            {
                if (len < HDR_OFF_MINSZ)
                    return false;

                offset = 0;
                fsize  = len - HDR_OFF_DATA;
                data   = &_data[HDR_OFF_DATA];
                datasz = fsize;
            }

            if ( offset + datasz > fsize )
                return false;
        }

        return true;
    }
};





// Raw input packets
struct InRawPkt
{
    IPaddress addr;
    uint8_t  *data;
    size_t    len;

    // Fill by parser
    InRawPktHdr hdr;

    InRawPkt(const UDPpacket *pkt)
    {
        if (pkt->len && pkt->data)
        {
            len = pkt->len;
            data = new uint8_t[len];
            memcpy(data, pkt->data, len);
            addr = pkt->address;
        }
        else
        {
            data = NULL;
            len = 0;
            addr.host = 0;
            addr.port = 0;
        }
    }

    ~InRawPkt()
    {
        if (data)
            delete[] data;
    }

    bool Parse()
    {
        if (!data)
            return false; // No data

        return hdr.Parse(data, len);
    }
};

typedef std::list<InRawPkt *> InRawList;





// Used for packets assembly
struct InPartedPkt
{
    AddrSeq   ipseq;
    uint64_t  timeout;
    size_t    nextOff;
    uint8_t  *data;
    size_t    len;
    uint8_t   flags;
    uint8_t   uchnl;
    uint8_t   retry;
    InRawList parts;

    InPartedPkt(const AddrSeq& _ipseq, size_t _len, uint8_t _flags, uint8_t _channel)
    {
        ipseq = _ipseq;
        timeout = 0;
        nextOff = 0;
        data = NULL;
        len = 0;
        flags = _flags;
        uchnl = _channel;
        retry = RETRY_GARANT;

        if (_len)
        {
            data = new uint8_t[_len];
            len = _len;
        }
    }

    ~InPartedPkt()
    {
        if (data)
            delete[] data;

        InRawList::iterator it = parts.begin();
        while(it != parts.end())
        {
            delete *it;
            it = parts.erase(it);
        }
    }

    bool Feed(InRawPkt *pkt, uint64_t timestamp)
    {
        if (!pkt || !pkt->hdr.data || pkt->hdr.datasz <= 0 ||
            pkt->hdr.seqid != ipseq.seq ||
            pkt->hdr.offset < nextOff ||
            len < pkt->hdr.offset + pkt->hdr.datasz)
        {
            delete pkt;
            return false;
        }

        if (pkt->hdr.offset == nextOff)
        {
            if (pkt->hdr.flags & PKT_FLAG_GARANT)
                timeout = timestamp + TIMEOUT_PENDING_GARANT; // For retry
            else
                timeout = timestamp + TIMEOUT_PENDING;

            _Insert(pkt);
            delete pkt;

            if (!parts.empty()) // Check applicability of other parts
            {
                InRawList::iterator it = parts.begin();
                while(it != parts.end())
                {
                    if ( (*it)->hdr.offset < nextOff )
                    {   //Wrong packet
                        delete *it;
                        it = parts.erase(it);
                    }
                    else if ( (*it)->hdr.offset == nextOff )
                    {
                        _Insert(*it);
                        delete *it;
                        parts.erase(it);
                        it = parts.begin();
                    }
                    else
                        it++;

                }
            }
        }
        else
        {
            parts.push_back(pkt);
        }

        return nextOff >= len;
    }

    size_t RetryUpTo()
    {
        size_t tmp = len;
        for(InRawList::iterator it = parts.begin(); it != parts.end(); it++)
        {
            InRawPkt *pkt = *it;
            tmp = std::min((size_t)pkt->hdr.offset, tmp);
        }

        if (tmp > nextOff)
            return tmp;

        return 0;
    }

    void _Insert(InRawPkt *pkt)
    {
        memcpy(&data[nextOff], pkt->hdr.data, pkt->hdr.datasz);
        nextOff += pkt->hdr.datasz;
    }
};

typedef std::list<InPartedPkt *> PartedList;






struct Pkt
{
    IPaddress addr;
    uint8_t  *_raw_data;
    size_t    _raw_len;

    uint8_t   flags;
    uint32_t  seqid;
    uint8_t  *data;
    size_t    datasz;

    uint8_t   uchnl;

    NetUser  *user;

    Pkt(InRawPkt *pk, NetUser *usr)
    {
        addr = pk->addr;
        _raw_data = pk->data;
        _raw_len = pk->len;
        flags = pk->hdr.flags;
        seqid = pk->hdr.seqid;
        data = pk->hdr.data;
        datasz = pk->hdr.datasz;
        user = usr;
        uchnl = pk->hdr.uchnl;

        //Detach data and delete
        pk->data = NULL;
        pk->len = 0;

        delete pk;
    }

    Pkt(InPartedPkt *pk, NetUser *usr)
    {
        addr = pk->ipseq.addr;
        _raw_data = pk->data;
        _raw_len = pk->len;
        flags = pk->flags;
        seqid = pk->ipseq.seq;
        data = pk->data;
        datasz = pk->len;
        user = usr;
        uchnl = pk->uchnl;

        //Detach data and delete
        pk->data = NULL;
        pk->len = 0;

        delete pk;
    }

    ~Pkt()
    {
        if (_raw_data)
            delete[] _raw_data;
    }
};


}

#endif
