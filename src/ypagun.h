#ifndef YGUN_H_INCLUDED
#define YGUN_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

struct __NC_STACK_ypagun
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_intern;
    float field_c;
    float field_10;
    float field_14;
    xyz dir;
    xyz field_24;
    char field_30;
    int field_31;
    int field_35;
    char field_39;
    int field_3A;
};

struct gun_arg128
{
    int field_0;
    xyz dir;
};

struct gun_arg129
{
    xyz vec;
    xyz dir;
    float angle;
};

class NC_STACK_ypagun: public NC_STACK_ypabact
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func70(ypabact_arg65 *arg);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual void ypabact_func75(bact_arg75 *arg);
    virtual void ypabact_func77(void *);
    virtual size_t ypabact_func80(bact_arg80 *arg);
    virtual void ypabact_func82(ypabact_arg65 *arg);
    virtual void ypabact_func96(void *);
    virtual size_t ypabact_func111(__NC_STACK_ypabact *cel_unit);
    virtual void ypagun_func128(gun_arg128 *arg);
    virtual void ypagun_func129(gun_arg129 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypagun() {
        memset(&stack__ypagun, 0, sizeof(stack__ypagun));
    };
    virtual ~NC_STACK_ypagun() {};

    virtual const char * getClassName() {
        return "ypagun.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypagun();
    };

    enum GUN_ATT
    {
        GUN_ATT_SIDEANGLE = 0x80002000,
        GUN_ATT_UPANGLE = 0x80002001,
        GUN_ATT_DOWNANGLE = 0x80002002,
        GUN_ATT_FIRETYPE = 0x80002003,
        GUN_ATT_FIRETIME = 0x80002004,
        GUN_ATT_SETGROUND = 0x80002005,
        GUN_ATT_ROBOGUN = 0x80002006
    };

    virtual void setGUN_sideAngle(int);
    virtual void setGUN_upAngle(int);
    virtual void setGUN_downAngle(int);
    virtual void setGUN_fireType(int);
    virtual void setGUN_fireTime(int);
    virtual void setGUN_setGround(int);
    virtual void setGUN_roboGun(int);

    virtual int getGUN_sideAngle();
    virtual int getGUN_upAngle();
    virtual int getGUN_downAngle();
    virtual int getGUN_fireType();
    virtual int getGUN_fireTime();
    virtual int getGUN_setGround();
    virtual int getGUN_roboGun();


    int ypagun_func0__sub0(stack_vals *stak);
    int ypagun_func2__sub0(stack_vals *stak);
    int ypagun_func3__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypagun stack__ypagun;
};

#endif // YGUN_H_INCLUDED
