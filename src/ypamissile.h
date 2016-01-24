#ifndef YMISSILE_H_INCLUDED
#define YMISSILE_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypamissile_class_off;

struct NC_STACK_ypamissile;

struct __NC_STACK_ypamissile
{
};

struct NC_STACK_ypamissile : public NC_STACK_ypabact
{
    __NC_STACK_ypamissile stack__ypamissile;
};

#endif // YMISSILE_H_INCLUDED
