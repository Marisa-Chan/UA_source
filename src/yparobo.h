#ifndef YROBO_H_INCLUDED
#define YROBO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored yparobo_class_off;

struct NC_STACK_yparobo;

struct __NC_STACK_yparobo
{
};

struct NC_STACK_yparobo : public NC_STACK_ypabact
{
    __NC_STACK_yparobo stack__yparobo;
};

#endif // YROBO_H_INCLUDED
