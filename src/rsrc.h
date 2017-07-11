#ifndef RSRC_H_INCLUDED
#define RSRC_H_INCLUDED

#include "nucleas.h"

struct rsrc : public nnode
{
    const char *name;
    char title[64];
    int32_t ref_cnt;
    int16_t what_list;
    void *data;
    const char *class_name;
};

struct __NC_STACK_rsrc
{
    rsrc *p_rsrc;
    void *p_data;
    uint32_t flags;
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
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *a3);
    virtual size_t func3(stack_vals *stak);
    virtual rsrc * rsrc_func64(stack_vals *stak);
    virtual size_t rsrc_func65(rsrc **res);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg) {
        return 0;
    };

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_rsrc() {
        memset(&stack__rsrc, 0, sizeof(stack__rsrc));
    };
    virtual ~NC_STACK_rsrc() {};

    virtual const char * getClassName() {
        return "rsrc.class";
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
    virtual int getRsrc_tryShared();
    virtual void *getRsrc_pData();
    virtual int getRsrc_dontCopy();
    virtual nlist *getRsrc_sharedList();
    virtual nlist *getRsrc_privateList();

    //Data
    static const NewClassDescr description;

    __NC_STACK_rsrc stack__rsrc;
};

#endif // RSRC_H_INCLUDED
