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
    bact_node selfie_node;
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
    xyz pos;
    float period;
};

class NC_STACK_ypamissile: public NC_STACK_ypabact
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func68(ypabact_arg65 *arg);
    virtual void ypabact_func69(ypabact_arg65 *arg);
    virtual void ypabact_func70(ypabact_arg65 *arg);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual void ypabact_func74(bact_arg74 *arg);
    virtual void ypabact_func78(bact_arg119 *arg);
    virtual void ypabact_func96(void *);
    virtual size_t ypabact_func119(bact_arg119 *arg);
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

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypamissile stack__ypamissile;
};

#endif // YMISSILE_H_INCLUDED
