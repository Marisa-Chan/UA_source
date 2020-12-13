#ifndef YFLYER_H_INCLUDED
#define YFLYER_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

class NC_STACK_ypaflyer: public NC_STACK_ypabact
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void Renew();
    virtual void HandBrake(update_msg *arg);

    NC_STACK_ypaflyer() 
    {
        _flyerBoost = 0.0;
        _flyerType    = 0;
    };
    virtual ~NC_STACK_ypaflyer() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
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
    
    
    void sb_0x4b255c(float a2, vec3d v, int a4);
    void ypaflyer_func70__sub0(float angl);

    //Data
    static const Nucleus::ClassDescr description;
    
    float _flyerBoost = 0.0;
    int _flyerType    = 0;
};

#endif // YFLYER_H_INCLUDED
