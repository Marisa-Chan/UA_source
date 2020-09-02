#ifndef YGUN_H_INCLUDED
#define YGUN_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

class NC_STACK_ypagun: public NC_STACK_ypabact
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void FightWithBact(bact_arg75 *arg);
    virtual void Die();
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void EnergyInteract(update_msg *arg);
    virtual void Renew();
    virtual size_t TestTargetSector(NC_STACK_ypabact *cel_unit);
    virtual void ypagun_func128(const vec3d &basis, bool directDown);
    virtual vec3d ypagun_func129(const vec3d &axis, float angle);

    NC_STACK_ypagun()
    {
        _gunMaxUp    = 0.0;
        _gunMaxDown  = 0.0;
        _gunMaxSide  = 0.0;
        _gunBasis    = vec3d(0.0, 0.0, 0.0);
        _gunRott     = vec3d(0.0, 0.0, 0.0);
        _gunType  = 0;
        _gunFireTime = 0;
        _gunFireCount = 0;
        _gunFlags = 0;
        _gunDownTime = 0;
    };
    virtual ~NC_STACK_ypagun() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance()
    {
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

    enum GUN_TYPE
    {
        GUN_TYPE_DUMMY  = 0, // For radar
        GUN_TYPE_REAL   = 1,
        GUN_TYPE_PROTO  = 2
    };

    enum GUN_FLAGS
    {
        GUN_FLAGS_GROUND   = 1,
        GUN_FLAGS_ROBO     = 2,
        GUN_FLAGS_FALLDOWN = 4,
        GUN_FLAGS_SHOT     = 8
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
    virtual bool getGUN_setGround();
    virtual bool IsRoboGun();


    bool CheckPedestal();

    //Data
    static const Nucleus::ClassDescr description;

    float _gunMaxUp;
    float _gunMaxDown;
    float _gunMaxSide;
    vec3d _gunBasis; // Basis Z
    vec3d _gunRott; // Basis Y
    char _gunType;
    int _gunFireTime;
    int _gunFireCount;
    char _gunFlags;
    int _gunDownTime;
};

#endif // YGUN_H_INCLUDED
