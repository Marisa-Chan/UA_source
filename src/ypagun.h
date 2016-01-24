#ifndef YGUN_H_INCLUDED
#define YGUN_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypagun_class_off;

struct NC_STACK_ypagun;

struct __NC_STACK_ypagun
{
};

struct NC_STACK_ypagun : public NC_STACK_ypabact
{
    __NC_STACK_ypagun stack__ypagun;
};

#endif // YGUN_H_INCLUDED
