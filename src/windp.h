#ifndef WINDP_H_INCLUDED
#define WINDP_H_INCLUDED


#include "nucleas.h"

extern class_stored windp_class_off;

struct NC_STACK_windp;

struct __NC_STACK_windp
{
    int dummy;
};

struct NC_STACK_windp : public NC_STACK_nucleus
{
    __NC_STACK_windp stack__windp;
};



struct windp_arg79
{
    int field_0;
    int field_4;
    char *field_8;
    int field_C;
    int field_10;
    int field_14;
};

struct windp_arg82
{
    char *field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
};

struct windp_arg87
{
    char callSIGN[64];
    char field_40;
    char field_41;
};

#endif // WINDP_H_INCLUDED
