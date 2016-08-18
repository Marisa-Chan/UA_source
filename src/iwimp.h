#ifndef IWIMP_H_INCLUDED
#define IWIMP_H_INCLUDED

#include "idev.h"
#include "engine_input.h"
#include "utils.h"


class NC_STACK_iwimp;

struct __NC_STACK_iwimp
{
    int field_0;
    nlist list;
    ClickBox *selected_btn;
    int selected_btnID;
};


struct iwimp_arg129
{
    nnode *node;
    int field_4;
};


class NC_STACK_iwimp: public NC_STACK_idev
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t iwimp_func128(stack_vals *);
    virtual void iwimp_func129(iwimp_arg129 *arg);
    virtual void iwimp_func130(iwimp_arg129 *arg);
    virtual void iwimp_func131(winp_131arg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_iwimp() {
        memset(&stack__iwimp, 0, sizeof(stack__iwimp));
    };
    virtual ~NC_STACK_iwimp() {};

    virtual const char * getClassName() {
        return "iwimp.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_iwimp();
    };

    virtual void setWIMP_wndInfo(gfx_window *wnd) {};

    //Data
    static const NewClassDescr description;

    __NC_STACK_iwimp stack__iwimp;
};

#endif // IWIMP_H_INCLUDED
