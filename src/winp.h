#ifndef WINP_H_INCLUDED
#define WINP_H_INCLUDED

#include "iwimp.h"
#include "utils.h"

struct __NC_STACK_winp
{
    int remapIndex;
    int mousePos;
    int mouseDelta;
    uint8_t hotKeys[48];
};

class NC_STACK_winp: public NC_STACK_iwimp
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stk);
    virtual void idev_func64(win_64arg *arg);
    virtual void idev_func65(win_64arg *arg);
    virtual void idev_func66(winp_66arg *arg);
    virtual int idev_func67(const char **arg);
    virtual int idev_func68(winp_68arg *arg);
    virtual void idev_func69(int arg);
    virtual void idev_func70(idev_query_arg *arg);
    virtual void idev_func71(winp_71arg *arg);
    virtual void CheckClick(ClickBoxInf *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_winp() {
        memset(&stack__winp, 0, sizeof(stack__winp));
    };
    virtual ~NC_STACK_winp() {};

    virtual const char * getClassName() {
        return "winp.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_winp();
    };
    static void initfirst();

    enum FF_TYPE
    {
        FF_TYPE_ALL = 0,
        FF_TYPE_TANKENGINE = 1,
        FF_TYPE_JETENGINE = 2,
        FF_TYPE_HELIENGINE = 3,
        FF_TYPE_ROTDAMPER = 4,
        FF_TYPE_MINIGUN = 5,
        FF_TYPE_MISSILEFIRE = 6,
        FF_TYPE_GRENADEFIRE = 7,
        FF_TYPE_BOMBFIRE = 8,
        FF_TYPE_COLLISION = 9,
        FF_TYPE_SHAKE = 10
    };

    enum FF_STATE
    {
        FF_STATE_START = 0,
        FF_STATE_STOP = 1,
        FF_STATE_UPDATE = 2
    };

protected:
    void FFstopAll();
    void FFDOTankEngine(int state, float p1, float p2);
    void FFDOJetEngine(int state, float p1, float p2);
    void FFDOHeliEngine(int state, float p1, float p2);
    void FFDORotDamper(int state, float p1);
    void FFDOMiniGun(int state);
    void FFDOMissileFire(int state);
    void FFDOGrenadeFire(int state);
    void FFDOBombFire(int state);
    void FFDOCollision(int state, float a2, float a3, float a4);
    void FFDOShake(int state, float a2, float a3, float a4, float a5);

public:
    //Data
    static const NewClassDescr description;

    __NC_STACK_winp stack__winp;
};

#endif // WINP_H_INCLUDED
