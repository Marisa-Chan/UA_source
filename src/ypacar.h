#ifndef YCAR_H_INCLUDED
#define YCAR_H_INCLUDED

#include "nucleas.h"
#include "ypatank.h"

extern class_stored ypacar_class_off;

struct NC_STACK_ypacar;

struct __NC_STACK_ypacar
{
};

struct NC_STACK_ypacar : public NC_STACK_ypabact
{
    __NC_STACK_ypacar stack__ypacar;
};

#endif // YCAR_H_INCLUDED
