#ifndef RSRC_H_INCLUDED
#define RSRC_H_INCLUDED

#include <string>
#include <list>
#include "nucleas.h"

struct rsrc;

typedef std::list<rsrc *> RSRCList;

struct rsrc
{
    const std::string name;
    const bool shared;

    int32_t ref_cnt;
    void *data;

    rsrc(const std::string &_name, bool _shared):
        name(_name),
        shared(_shared)
    {
        ref_cnt = 0;
        data = NULL;
    }
};

struct rsrc_func66_arg
{
    const char *filename;
    IFFile *file;
    int OpenedStream;
};

class NC_STACK_rsrc: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *res);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg) {
        return 0;
    };

    NC_STACK_rsrc()
    {
        resource = NULL;
        _flags = 0;
    };

    virtual ~NC_STACK_rsrc() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_rsrc();
    };

    enum RSRC_ATT
    {
        RSRC_ATT_NAME = 0x80001000,
        RSRC_ATT_TRYSHARED = 0x80001001,
        RSRC_ATT_PDATA = 0x80001002,
        RSRC_ATT_DONTCOPY = 0x80001003,
        RSRC_ATT_PIFFFILE = 0x80001005,
        RSRC_ATT_SHAREDLIST = 0x80001006,
        RSRC_ATT_PRIVATELIST = 0x80001007,
        RSRC_ATT_LISTYPE = 0x80001008
    };

    //Set

    //Get
    virtual const char * getRsrc_name();
    virtual const std::string getRsrc_name() const;
    virtual int getRsrc_tryShared();
    virtual void *getRsrc_pData();
    virtual int getRsrc_dontCopy();

    static RSRCList *getRsrc_sharedList();
    static RSRCList *getRsrc_privateList();

    //Data
public:
    static const Nucleus::ClassDescr description;

protected:
    rsrc *resource;
    uint32_t _flags;

protected:
    static RSRCList privateList;
    static RSRCList publicList;
};

#endif // RSRC_H_INCLUDED
