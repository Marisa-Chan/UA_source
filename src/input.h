#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"
#include "wintimer.h"
#include "winp.h"

class NC_STACK_input;

struct __NC_STACK_input
{
    int field_0;
    int field_4;
    NC_STACK_itimer *timer;
    NC_STACK_iwimp *wimp;
    NC_STACK_idev *keyboard;
    nlist buttons_lists[32];
    nlist sliders_lists[32];
};

struct inputkey_node : public nnode
{
    NC_STACK_idev *driver_obj;
    int field_C;
    int keyType;
    char driver_name[32];
    char keyname[32];
};

struct input__func64__params
{
    int type_id;
    int item_number;
    const char *value;
};

struct inp_l65
{
    int field_0;
    float field_4;
};

struct input__func66__params
{
    int field_0;
    int field_4;
    int funcID;
    void *vals;
};


class NC_STACK_input: public NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t input_func64(input__func64__params *arg);
    virtual void input_func65(struC5 *arg);
    virtual size_t input_func66(input__func66__params *arg);

    virtual int keyb_setHotkey(winp_68arg *arg);
    virtual void keyb_queryHotkey(idev_query_arg *arg);

    virtual void wimp_addClickNode(iwimp_arg129 *arg);
    virtual void wimp_remClickNode(iwimp_arg129 *arg);
    virtual void wimp_ForceFeedback(winp_71arg *ctrl);

    virtual void slider_reset(int sldr, int rtp);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_input() {
        memset(&stack__input, 0, sizeof(stack__input));
    };
    virtual ~NC_STACK_input() {};

    virtual const char * getClassName() {
        return "input.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_input();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_input stack__input;
};

#endif // INPUT_H_INCLUDED
