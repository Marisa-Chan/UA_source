#ifndef YFLYER_H_INCLUDED
#define YFLYER_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

struct __NC_STACK_ypaflyer
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    float field_c;
    int field_10;
};

class NC_STACK_ypaflyer: public NC_STACK_ypabact
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void Renew();
    virtual void HandBrake(update_msg *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypaflyer() {
        memset(&stack__ypaflyer, 0, sizeof(stack__ypaflyer));
    };
    virtual ~NC_STACK_ypaflyer() {};

    virtual const char * getClassName() {
        return "ypaflyer.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypaflyer();
    };

    enum FLY_ATT
    {
        FLY_ATT_TYPE = 0x80002000
    };

    virtual void setFLY_type(int);

    virtual int getFLY_type();

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypaflyer stack__ypaflyer;
};

#endif // YFLYER_H_INCLUDED
