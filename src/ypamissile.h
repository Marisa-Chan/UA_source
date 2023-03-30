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
    virtual size_t Init(IDVList &stak) override;
    virtual size_t Deinit() override;
    virtual size_t SetParameters(IDVList &stak) override;
    virtual void AI_layer1(update_msg *arg) override;
    virtual void AI_layer2(update_msg *arg) override;
    virtual void AI_layer3(update_msg *arg) override;
    virtual void User_layer(update_msg *arg) override;
    virtual void Move(move_msg *arg) override;
    virtual void SetState(setState_msg *arg) override;
    virtual void Renew() override;
    virtual size_t SetStateInternal(setState_msg *arg) override;
    
    virtual void ResetViewing(); // Detach camera
    virtual void Impact(); // Apply impulse to all in sector
    virtual void AlignMissile(float dtime = 0.0);
    virtual void AlignMissileByNormal(const vec3d &normal);

    NC_STACK_ypamissile();
    virtual ~NC_STACK_ypamissile() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
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

    virtual void setBACT_viewer(bool) override;
    
    virtual void SetLauncherBact(NC_STACK_ypabact *);
    virtual void SetMissileType(int);
    virtual void SetLifeTime(int);
    virtual void SetDelay(int);
    virtual void SetDriveTime(int);
    virtual void SetIgnoreBuilds(int);
    virtual void SetPowerHeli(int);
    virtual void SetPowerTank(int);
    virtual void SetPowerFlyer(int);
    virtual void SetPowerRobo(int);
    virtual void SetRadiusHeli(float);
    virtual void SetRadiusTank(float);
    virtual void SetRadiusFlyer(float);
    virtual void SetRadiusRobo(float);
    virtual void SetStartHeight(float);

    virtual NC_STACK_ypabact *GetLauncherBact();
    virtual int GetMissileType();
    virtual int GetLifeTime();
    virtual int GetDelay();
    virtual int GetDriveTime();
    virtual int GetIgnoreBuilds();
    virtual int GetPowerHeli();
    virtual int GetPowerTank();
    virtual int GetPowerFlyer();
    virtual int GetPowerRobo();
    virtual float GetRadiusHeli();
    virtual float GetRadiusTank();
    virtual float GetRadiusFlyer();
    virtual float GetRadiusRobo();
    virtual float GetStartHeight();
    
    vec3d CalcForceVector();
    bool TubeCollisionTest();

    //Data
public:
    static constexpr const char * __ClassName = "ypamissile.class";
    
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
