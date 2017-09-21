#ifndef IDVPAIR_H_INCLUDED
#define IDVPAIR_H_INCLUDED

#include <inttypes.h>
#include <map>

struct IDVPair
{
    unsigned int id;
    uint8_t flags;
    uint8_t type;

    union _value
    {
        void *p_data;
        int i_data;
        float f_data;
        size_t u_data;
    } value;

    enum TYPE
    {
        TYPE_UNK     = 0,
        TYPE_POINTER = 1,
        TYPE_INTEGER = 2,
        TYPE_FLOAT   = 3,
        TYPE_SZ      = 4
    };

    enum FLAGS
    {
        FLAGS_SKIP = 1
    };

    IDVPair()
    {
        id = 0;
        value.u_data = 0;
        flags = 0;
        type = TYPE_UNK;
    }

    IDVPair(unsigned int _id, size_t _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, int _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, float _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, void *_val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, const void *_val)
    {
        flags = 0;
        set(_id, _val);
    }

    void set(unsigned int _id, size_t _val)
    {
        id = _id;
        value.u_data = _val;
        type = TYPE_SZ;
    }

    void set(unsigned int _id, int _val)
    {
        id = _id;
        value.i_data = _val;
        type = TYPE_INTEGER;
    }

    void set(unsigned int _id, float _val)
    {
        id = _id;
        value.f_data = _val;
        type = TYPE_FLOAT;
    }

    void set(unsigned int _id, void *_val)
    {
        id = _id;
        value.p_data = _val;
        type = TYPE_POINTER;
    }

    void set(unsigned int _id, const void *_val)
    {
        id = _id;
        value.p_data = (void *)_val;
        type = TYPE_POINTER;
    }

    inline bool skip()
    {
        return (flags & FLAGS_SKIP) != 0;
    }
};


class IDVList: public std::map<unsigned int, IDVPair>
{
public:
    void Add(unsigned int _id, size_t _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, int _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, float _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, void *_val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, const void *_val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    int Get(unsigned int _id, int _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.i_data;
    }

    size_t Get(unsigned int _id, size_t _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.u_data;
    }

    float Get(unsigned int _id, float _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.f_data;
    }

    void *GetPointer(unsigned int _id, void *_def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.p_data;
    }

    const char * GetConstChar(unsigned int _id, const char * _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (const char *)(it->second).value.u_data;
    }
};

#endif // IDVPAIR_H_INCLUDED
