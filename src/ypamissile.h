#ifndef YMISSILE_H_INCLUDED
#define YMISSILE_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

// Bomb rotation speed
#define MISSILE_BOMB_MIN_ANGLE 0.001


extern class_stored ypamissile_class_off;

struct NC_STACK_ypamissile;

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

struct NC_STACK_ypamissile : public NC_STACK_ypabact
{
    __NC_STACK_ypamissile stack__ypamissile;
};

struct miss_arg130
{
    xyz pos;
    float period;
};

#endif // YMISSILE_H_INCLUDED
