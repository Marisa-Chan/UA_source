#ifndef ZNDNET_SENDSTORAGE_H_INCLUDED
#define ZNDNET_SENDSTORAGE_H_INCLUDED

#include <deque>
#include <list>
#include <string.h>

#include "addrseq.h"

namespace ZNDNet
{

class RefData
{
public:
    inline size_t size() {return _datasz;};
    inline int32_t unlink() {return --_refcnt;};
    inline void link() {++_refcnt;};
    inline int32_t refs() {return _refcnt;};

    void copy(void *dst) {copy(dst, 0, _datasz);};
    void copy(void *dst, size_t nbytes) {copy(dst, 0, nbytes);};

    virtual void copy(void *dst, size_t pos, size_t nbytes) = 0;
    virtual ~RefData() {};

protected:
    RefData(): _datasz(0), _refcnt(0) {};

protected:
    size_t    _datasz;
    int32_t   _refcnt;
};


class RefDataStatic: public RefData
{
public:
    inline uint8_t * get()
    {
        return _data;
    };

    virtual void copy(void *dst, size_t pos, size_t nbytes)
    {
        if (!dst || pos + nbytes > _datasz)
            return;

        memcpy(dst, &_data[pos], nbytes);
    };

    ~RefDataStatic()
    {
        if (_data)
            delete[] _data;
    };

    static RefDataStatic *create(size_t sz)
    {
        return new RefDataStatic(sz);
    };
    static RefDataStatic *create(uint8_t *data, size_t sz)
    {
        return new RefDataStatic(data, sz);
    };
protected:

    RefDataStatic(uint8_t *data, size_t sz)
    {
        _data = new uint8_t[sz];
        memcpy(_data, data, sz);

        _datasz = sz;
    };

    RefDataStatic(size_t sz)
    {
        _data = new uint8_t[sz];
        _datasz = sz;
    };

protected:
    uint8_t * _data;
};



class RefDataWStream: public RefData
{
public:
    void write(const void *_src, size_t nbytes)
    {
        if (!_src)
            return;

        checkfree(nbytes);

        const uint8_t *src = (const uint8_t *)_src;

        while(nbytes > 0)
        {
            size_t blkid  = _pos / _blksize;
            size_t blkpos = _pos % _blksize;

            size_t blkfree = _blksize - blkpos;
            size_t tocopy = nbytes;

            if (tocopy > blkfree)
                tocopy = blkfree;

            uint8_t *dst = _blocks[blkid];
            memcpy(&dst[blkpos], src, tocopy);

            src += tocopy;
            _pos += tocopy;
            if (_pos > _datasz)
                _datasz = _pos;
            nbytes -= tocopy;
        }
    }

    void writeU8(uint8_t bt)
    {
        checkfree(1);

        size_t blkid  = _pos / _blksize;
        size_t blkpos = _pos % _blksize;

        _blocks[blkid][blkpos] = bt;

        _pos++;

        if (_pos > _datasz)
            _datasz = _pos;
    }

    void writeU32(uint32_t dw)
    {
        /*uint8_t bytes[4];
        bytes[0] = dw & 0xFF;
        bytes[1] = (dw >> 8) & 0xFF;
        bytes[2] = (dw >> 16) & 0xFF;
        bytes[3] = (dw >> 24) & 0xFF;

        write(bytes, 4);*/
        write(&dw, 4);
    }

    void writeU64(uint64_t qw)
    {
        write(&qw, 8);
    }

    void writeStr(const std::string &str)
    {
        if (str.size())
            write(str.c_str(), str.size());
    }

    void writeSzStr(const std::string &str)
    {
        writeU8(str.size());
        if (str.size())
            write(str.c_str(), str.size());
    }

    bool seek(int32_t pos, uint8_t mode)
    {
        switch(mode)
        {
            case 0:
                break;
            case 1:
                pos += _pos;
                break;
            case 2:
                pos += _datasz;
                break;
            default:
                return false;
                break;
        }

        if (pos < 0 || pos > (int32_t)_datasz)
            return false;

        _pos = pos;
        return true;
    }

    size_t tell()
    {
        return _pos;
    }

    virtual void copy(void *_dst, size_t pos, size_t nbytes)
    {
        if (!_dst || pos + nbytes > _datasz)
            return;

        uint8_t *dst = (uint8_t *)_dst;

        while(nbytes > 0)
        {
            size_t blkid  = pos / _blksize;
            size_t blkpos = pos % _blksize;

            size_t blkafter = _blksize - blkpos;
            size_t tocopy = nbytes;

            if (tocopy > blkafter)
                tocopy = blkafter;

            uint8_t *src = _blocks[blkid];
            memcpy(dst, &src[blkpos], tocopy);

            dst += tocopy;
            nbytes -= tocopy;
            pos += tocopy;
        }
    }

    ~RefDataWStream()
    {
        for(_tBlockList::iterator it = _blocks.begin(); it != _blocks.end(); it++)
            delete [] *it;
    }

    static RefDataWStream *create(uint32_t blocksize = 0x4000)
    {
        return new RefDataWStream(blocksize);
    };
    static RefDataWStream *create(uint8_t *data, size_t sz, uint32_t blocksize = 0x4000)
    {
        return new RefDataWStream(data, sz, blocksize);
    };
protected:
    RefDataWStream(uint32_t blocksize)
    : _blksize(blocksize)
    {
        _pos = 0;
    }

    RefDataWStream(uint8_t *data, size_t sz, uint32_t blocksize)
    : _blksize(blocksize)
    {
        _pos = 0;
        write(data, sz);
    }

    void checkfree(size_t nbytes)
    {
        int32_t needed = nbytes - (_datasz - _pos);
        if (needed > 0)
        {
            int32_t free = _blksize * _blocks.size() - _datasz;
            while (free < needed)
            {
                _blocks.push_back(new uint8_t[_blksize]);
                free += _blksize;
            }
        }
    }


protected:
    typedef std::deque<uint8_t *> _tBlockList;

    _tBlockList    _blocks;
    size_t         _pos;

    const uint32_t _blksize;
};




struct SendingData
{
    AddrSeq  addr;
    RefData *pdata;
    size_t   sended;
    size_t   retryUpTo;
    uint8_t  flags;

    uint16_t tr_cnt;
    uint64_t timeout;

    uint32_t  schnl;  //For sync sending
    uint8_t   uchnl;

    SendingData(const AddrSeq &_addr, RefData *_data, uint8_t _flags)
    {
        addr = _addr;
        pdata = _data;
        flags = _flags;

        sended = 0;
        retryUpTo = 0;
        tr_cnt = RETRY_GARANT;
        timeout = 0;

        schnl = PKT_NO_CHANNEL;
        uchnl = PKT_CHNL_NOT_SET;

        if (pdata)
            pdata->link();
    }

    SendingData(const IPaddress &_addr, uint32_t _seq, RefData *_data, uint8_t _flags)
    {
        addr.set(_addr, _seq);
        pdata = _data;
        flags = _flags;

        sended = 0;
        retryUpTo = 0;
        tr_cnt = RETRY_GARANT;
        timeout = 0;

        schnl = PKT_NO_CHANNEL;
        uchnl = PKT_CHNL_NOT_SET;

        if (pdata)
            pdata->link();
    }

    ~SendingData()
    {
        if (pdata)
        {
            if (pdata->unlink() == 0)
                delete pdata;
        }
    }

    void SetChannel(uint32_t userIDX, uint32_t userChnl = 0)
    {
        if (userChnl != PKT_CHNL_NOT_SET && userIDX < ZNDNET_USERS_MAX && userChnl < ZNDNET_USER_SCHNLS)
        {
            schnl = userIDX * ZNDNET_USER_SCHNLS + userChnl;
            uchnl = userChnl;
        }
        else
        {
            schnl = PKT_NO_CHANNEL;
            uchnl = PKT_CHNL_NOT_SET;
        }
    }
};

typedef std::list<SendingData *> SendingList;

}

#endif
