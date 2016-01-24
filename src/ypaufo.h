#ifndef YUFO_H_INCLUDED
#define YUFO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored ypaufo_class_off;

struct NC_STACK_ypaufo;

struct __NC_STACK_ypaufo
{
};

struct NC_STACK_ypaufo : public NC_STACK_ypabact
{
    __NC_STACK_ypaufo stack__ypaufo;
};

#endif // YUFO_H_INCLUDED
