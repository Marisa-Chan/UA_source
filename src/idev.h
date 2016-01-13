#ifndef IDEV_H_INCLUDED
#define IDEV_H_INCLUDED

#include "nucleas.h"

extern class_stored idev_class_off;

struct NC_STACK_idev;

struct __NC_STACK_idev
{

};

struct NC_STACK_idev : public NC_STACK_nucleus
{
    __NC_STACK_idev stack__idev;
};

#endif // IDEV_H_INCLUDED
