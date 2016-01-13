#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"
#include "wintimer.h"
#include "winp.h"

extern class_stored input_class_off;

struct NC_STACK_input;

struct __NC_STACK_input
{
    int field_0;
    int field_4;
    NC_STACK_wintimer *timer;
    NC_STACK_winp *wimp;
    NC_STACK_winp *keyboard;
    nlist buttons_lists[32];
    nlist sliders_lists[32];
};

struct NC_STACK_input : public NC_STACK_nucleus
{
    __NC_STACK_input stack__input;
};

struct inputkey_node : public nnode
{
    NC_STACK_winp *driver_obj;
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

#endif // INPUT_H_INCLUDED
