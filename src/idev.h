#ifndef IDEV_H_INCLUDED
#define IDEV_H_INCLUDED

#include "nucleas.h"

class NC_STACK_idev;

struct __NC_STACK_idev
{

};

struct win_64arg
{
    int field_0;
    int field_4;
    float field_8;
};

struct winp_66arg
{
    DWORD downed_key_2;
    DWORD downed_key;
    DWORD dword8;
    DWORD chr;
};

struct winp_68arg
{
    const char *keyname;
    DWORD id;
};

struct idev_query_arg
{
    int keycode;
    int hotkey;
};

struct winp_71arg
{
    DWORD effID;
    DWORD state;
    float p1;
    float p2;
    float p3;
    float p4;
};


class NC_STACK_idev: public NC_STACK_nucleus
{
public:
    virtual void idev_func64(win_64arg *arg) {};
    virtual void idev_func65(win_64arg *arg) {};
    virtual void idev_func66(winp_66arg *arg) {};
    virtual int idev_func67(const char **arg) {
        return 0;
    };
    virtual int idev_func68(winp_68arg *arg) {
        return 0;
    };
    virtual void idev_func69(int arg) {};
    virtual void idev_func70(idev_query_arg *arg) {};
    virtual void idev_func71(winp_71arg *arg) {};


    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_idev() {
        memset(&stack__idev, 0, sizeof(stack__idev));
    };
    virtual ~NC_STACK_idev() {};

    virtual const char * getClassName() {
        return "idev.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_idev();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_idev stack__idev;
};

#endif // IDEV_H_INCLUDED
