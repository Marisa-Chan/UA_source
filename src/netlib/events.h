#ifndef ZNDNET_EVENT_H_INCLUDED
#define ZNDNET_EVENT_H_INCLUDED

#include <list>

namespace ZNDNet
{

enum
{
    EVENT_DISCONNECT,   //On disconnect
    EVENT_CONNECTED,    //
    EVENT_CONNERR,
    EVENT_SESSION_LIST, //Sessions list receieved
    EVENT_SESSION_JOIN, //Success join
    EVENT_SESSION_FAIL, //Error join or create
    EVENT_SESSION_LEAVE,
    EVENT_SESSION_END,
    EVENT_SESSION_LEAD,  //
    EVENT_USER_LIST,
    EVENT_USER_ADD,
    EVENT_USER_LEAVE,
    EVENT_DATA,
};


class Event
{
public:
    const uint32_t type;
    const uint32_t value;
    uint32_t size;
    uint32_t __id;

    Event(uint32_t _type, uint32_t _value)
    : type(_type), value(_value)
    {
        size = 0;
        __id = 0;
    };

    virtual ~Event() {};
};

class EventNameID: public Event
{
public:
    std::string name;
    uint64_t id;

    EventNameID(uint32_t _type, uint32_t _value, const std::string &_name, uint64_t _id)
    : Event(_type, _value)
    {
        name = _name;
        id = _id;
    };

    virtual ~EventNameID() {};
};

class EventData: public Event
{
public:
    uint64_t from;
    bool     cast;
    uint64_t to;
    uint8_t *data;
    uint8_t  channel;

    EventData(uint32_t _type, uint32_t _value, uint64_t _from, bool _cast, uint64_t _to, uint32_t _sz, uint8_t *_data, uint8_t _channel)
    : Event(_type, _value)
    {
        from = _from;
        cast = _cast;
        to = _to;
        channel = _channel;

        if (_sz && _data)
        {
            data = new uint8_t[_sz];
            memcpy(data, _data, _sz);
            size = _sz;
        }
        else
        {
            size = 0;
            data = NULL;
        }

    };

    virtual ~EventData()
    {
        if (data)
            delete[] data;
    };
};

typedef std::list<Event *> EventList;

}

#endif
