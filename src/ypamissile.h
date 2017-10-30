#ifndef YMISSILE_H_INCLUDED
#define YMISSILE_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

// Bomb rotation speed
#define MISSILE_BOMB_MIN_ANGLE 0.001

struct __NC_STACK_ypamissile
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *selfie;
    char field_c;
    __NC_STACK_ypabact *ejaculator_bact;
    int life_time;
    int drive_time;
    int delay_time;
    int field_2D;
    float posy;
    float energy_heli;
    float energy_tank;
    float energy_flyer;
    float energy_robo;
    float radius_heli;
    float radius_tank;
    float radius_flyer;
    float radius_robo;
};

struct miss_arg130
{
    vec3d pos;
    float period;
};

class NC_STACK_ypamissile: public NC_STACK_ypabact
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);
    virtual void AI_layer1(update_msg *arg);
    virtual void AI_layer2(update_msg *arg);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual void SetState(setState_msg *arg);
    virtual void Renew();
    virtual size_t SetStateInternal(setState_msg *arg);
    virtual void ypamissile_func128(void *);
    virtual void ypamissile_func129(void *);
    virtual void ypamissile_func130(miss_arg130 *arg);
    virtual void ypamissile_func131(miss_arg130 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypamissile() {
        memset(&stack__ypamissile, 0, sizeof(stack__ypamissile));
    };
    virtual ~NC_STACK_ypamissile() {};

    virtual const char * getClassName() {
        return "ypamissile.class";
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
    virtual void setMISS_launcher(__NC_STACK_ypabact *);
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

    virtual __NC_STACK_ypabact *getMISS_launcher();
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

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypamissile stack__ypamissile;
};

#endif // YMISSILE_H_INCLUDED
