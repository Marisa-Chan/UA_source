#ifndef YTANK_H_INCLUDED
#define YTANK_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypatank_class_off;

struct NC_STACK_ypatank;

struct __NC_STACK_ypatank
{
};

struct NC_STACK_ypatank : public NC_STACK_ypabact
{
    __NC_STACK_ypatank stack__ypatank;
};

#endif // YTANK_H_INCLUDED
