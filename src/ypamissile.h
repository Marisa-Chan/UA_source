#ifndef YMISSILE_H_INCLUDED
#define YMISSILE_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

class NC_STACK_ypamissile: public NC_STACK_ypabact
{
protected:
    // Bomb rotation speed
    const double BOMB_MIN_ANGLE = 0.001;
        
public:
    
    enum FLAG_MISL
    {
        FLAG_MISL_VIEW          = (1 << 0),
        FLAG_MISL_COUNTDELAY    = (1 << 1),
        FLAG_MISL_IGNOREBUILDS  = (1 << 2),
    };
    
    enum MISL_TYPE
    {
        MISL_BOMB       = 1, // Drop down
        MISL_DIRECT     = 2, // Simple missile
        MISL_TARGETED   = 3, // Targeted missile
        MISL_GRENADE    = 4, // Gravity affected
        MISL_OBSAVOID   = 5, // Obstacle avoiding
        MISL_INTERNAL   = 6, // Only for internal use
    };
    
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer1(update_msg *arg);
    virtual void AI_layer2(update_msg *arg);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual void SetState(setState_msg *arg);
    virtual void Renew();
    virtual size_t SetStateInternal(setState_msg *arg);
    virtual void ResetViewing();
    virtual void Impact(); // Apply impulse to all in sector
    virtual void AlignMissile(float dtime = 0.0);
    virtual void AlignMissileByNormal(const vec3d &normal);

    NC_STACK_ypamissile();
    virtual ~NC_STACK_ypamissile() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypamissile();
    };

    enum MISS_ATT
    {
        MISS_ATT_LAUNCHER = 0x80002000,
        MISS_ATT_TYPE = 0x80002002,
        MISS_ATT_LIFETIME = 0x80002004,
        MISS_ATT_DELAY = 0x80002005,
        MISS_ATT_DRIVETIME = 0x80002006,
        MISS_ATT_IGNOREBUILDS = 0x80002007,
        MISS_ATT_POW_HELI = 0x80002008,
        MISS_ATT_POW_TANK = 0x80002009,
        MISS_ATT_POW_FLYER = 0x8000200A,
        MISS_ATT_POW_ROBO = 0x8000200B,
        MISS_ATT_RAD_HELI = 0x8000200C,
        MISS_ATT_RAD_TANK = 0x8000200D,
        MISS_ATT_RAD_FLYER = 0x8000200E,
        MISS_ATT_RAD_ROBO = 0x8000200F,
        MISS_ATT_STHEIGHT = 0x80002010
    };

    virtual void setBACT_viewer(int);
    virtual void setMISS_launcher(NC_STACK_ypabact *);
    virtual void setMISS_type(int);
    virtual void setMISS_lifeTime(int);
    virtual void setMISS_delay(int);
    virtual void setMISS_driveTime(int);
    virtual void setMISS_ignoreBuilds(int);
    virtual void setMISS_powHeli(int);
    virtual void setMISS_powTank(int);
    virtual void setMISS_powFlyer(int);
    virtual void setMISS_powRobo(int);
    virtual void setMISS_radHeli(int);
    virtual void setMISS_radTank(int);
    virtual void setMISS_radFlyer(int);
    virtual void setMISS_radRobo(int);
    virtual void setMISS_startHeight(int);

    virtual NC_STACK_ypabact *getMISS_launcher();
    virtual int getMISS_type();
    virtual int getMISS_lifeTime();
    virtual int getMISS_delay();
    virtual int getMISS_driveTime();
    virtual int getMISS_ignoreBuilds();
    virtual int getMISS_powHeli();
    virtual int getMISS_powTank();
    virtual int getMISS_powFlyer();
    virtual int getMISS_powRobo();
    virtual int getMISS_radHeli();
    virtual int getMISS_radTank();
    virtual int getMISS_radFlyer();
    virtual int getMISS_radRobo();
    virtual int getMISS_startHeight();
    
    vec3d CalcForceVector();
    bool TubeCollisionTest();

    //Data
public:
    static const Nucleus::ClassDescr description;
    
protected:
    int _mislType = 0;
    NC_STACK_ypabact *_mislEmitter = NULL;
    int _mislLifeTime   = 0;
    int _mislDriveTime  = 0;
    int _mislDelayTime  = 0;
    int _mislFlags      = 0;
    float _mislStartHeight  = 0.0;
    float _mislEnergyHeli   = 0.0;
    float _mislEnergyTank   = 0.0;
    float _mislEnergyFlyer  = 0.0;
    float _mislEnergyRobo   = 0.0;
    float _mislRadiusHeli   = 0.0;
    float _mislRadiusTank   = 0.0;
    float _mislRadiusFlyer  = 0.0;
    float _mislRadiusRobo   = 0.0;
};

#endif // YMISSILE_H_INCLUDED
