#ifndef RSRC_H_INCLUDED
#define RSRC_H_INCLUDED

#include "nucleas.h"

struct rsrc : public nnode
{
    const char *name;
    char title[64];
    __int16 ref_cnt;
    __int16 what_list;
    void *data;
    char *class_name;
};

struct __NC_STACK_rsrc
{
    rsrc *p_rsrc;
    void *p_data;
    char field_8[4];
};

struct rsrc_func66_arg
{
    const char *filename;
    MFILE *file;
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

    //Data
    static const NewClassDescr description;

    __NC_STACK_rsrc stack__rsrc;
};

#endif // RSRC_H_INCLUDED
