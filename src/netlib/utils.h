#ifndef ZNDNET_UTILS_H_INCLUDED
#define ZNDNET_UTILS_H_INCLUDED


namespace ZNDNet
{

inline bool IPCMP(const IPaddress &a, const IPaddress &b)
{
    return a.host == b.host && a.port == b.port;
}


inline void writeU32(uint32_t u, void *dst)
{
    uint8_t *dst8 = (uint8_t *)dst;
    dst8[0] = u & 0xFF;
    dst8[1] = (u >> 8) & 0xFF;
    dst8[2] = (u >> 16) & 0xFF;
    dst8[3] = (u >> 24) & 0xFF;
}

inline uint32_t readU32(const void *src)
{
    uint8_t *src8 = (uint8_t *)src;
    return src8[0] | (src8[1] << 8) | (src8[2] << 16) | (src8[3] << 24);
}




class MemReader
{
public:
    uint8_t   readU8()
    {
        if (!_data || _pos >= _size)
            return 0;

        return _data[_pos++];
    }

    uint32_t  readU32()
    {
        if (!_data || _pos >= _size)
            return 0;

        uint32_t rt = 0;
        read(&rt, 4);

        return rt;
    }

    uint64_t  readU64()
    {
        if (!_data || _pos >= _size)
            return 0;

        uint64_t rt = 0;
        read(&rt, 8);

        return rt;
    }

    size_t    readStr(std::string &str, size_t n)
    {
        if (!_data || _pos >= _size)
            return 0;

        size_t toread = _size - _pos;
        if (toread > n)
            toread = n;

        str.assign((const char *)&_data[_pos], toread);

        _pos += toread;

        return toread;
    }

    size_t    readSzStr(std::string &str)
    {
        if (!_data || _pos >= _size)
            return 0;

        uint8_t n = _data[_pos];
        _pos++;

        int32_t toread = _size - _pos;
        if (toread > n)
            toread = n;

        if (toread > 0)
            str.assign((const char *)&_data[_pos], toread);

        _pos += toread;

        return toread;
    }

    size_t    read(void *dst, size_t n)
    {
        if (!_data || _pos >= _size)
            return 0;

        size_t toread = _size - _pos;
        if (toread > n)
            toread = n;

        memcpy(dst, &_data[_pos], toread);
        _pos += toread;
        return toread;
    }

    size_t    seek(size_t nbytes)
    {
        if (!_data)
            return 0;

        _pos = nbytes;

        if (_pos > _size)
            _pos = _size;

        return _pos;
    }

    size_t    skip(int32_t nbytes)
    {
        if (!_data)
            return 0;

        if ( _pos + nbytes < 0 )
            _pos = 0;
        else
            _pos += nbytes;

        if (_pos > _size)
            _pos = _size;

        return _pos;
    }

    size_t    tell() { return _pos; };
    size_t    size() { return _size; };

    MemReader(const void *data, size_t sz)
    : _data((const uint8_t *)data), _size(sz), _pos(0)
    {}

protected:
    const uint8_t * const _data;
    const size_t _size;
    size_t   _pos;
};

}

#endif
