#ifndef YTANK_H_INCLUDED
#define YTANK_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

class NC_STACK_ypatank: public NC_STACK_ypabact
{
public:
    enum FLAG_TANK
    {
        FLAG_TANK_TIP     = (1 << 0),
        FLAG_TANK_ROTWAIT = (1 << 1)
    };
    
    enum ALIGN
    {
        ALIGN_NONE = 0,
        ALIGN_DONE = 1,
        ALIGN_NORMAL = 2,
    };
    
    enum
    {
        COLL_WALL_L = (1 << 0),
        COLL_WALL_R = (1 << 1),
        COLL_HILL_L = (1 << 2),
        COLL_HILL_R = (1 << 3),
    };
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void ApplyImpulse(bact_arg83 *arg);
    virtual size_t CollisionWithBact(int arg);
    virtual void Recoil(bact_arg88 *arg);
    virtual void Renew();
    virtual size_t CheckFireAI(bact_arg101 *arg);
    virtual size_t TestTargetSector(NC_STACK_ypabact *cel_unit);
    virtual void CorrectPositionOnLand();
    virtual int AlignVehicleAI(float dtime, vec3d *pNormal);
    virtual int AlignVehicleUser(float dtime, const vec3d &oldDir);

    NC_STACK_ypatank();
    virtual ~NC_STACK_ypatank() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypatank();
    };
    
    
    virtual bool IsGroundUnit() { return true; };

    enum TANK_ATT
    {
        TANK_ATT_TIP = 0x80002000
    };

    virtual void setTANK_tip(int);

    virtual int getTANK_tip();


    vec3d TankTip(vec3d v);
    void ypatank_func87__sub0(NC_STACK_ypabact *bact2);

    //Data
public:
    static const Nucleus::ClassDescr description;

protected:
    uint8_t _tankFlags;
    int     _tankWaitCount;
    vec3d   _tankCollisionVector;
    float   _tankCollisionAngle;
    float   _tankCollisionWay;
    uint8_t _tankCollisionFlag;
    bool    _tankExpectTgt;
};


#endif // YTANK_H_INCLUDED
