#ifndef YROBO_H_INCLUDED
#define YROBO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

extern class_stored yparobo_class_off;

struct NC_STACK_yparobo;

struct __NC_STACK_yparobo
{
    NC_STACK_ypaworld *wrld;
    _NC_STACK_ypaworld *wrld_yw;
    __NC_STACK_ypabact *bact_internal;

    char field_1DA;

    int field_1dc;

    char field_1E7;
    char field_1e8;
    char field_1E9;
    char field_1EA;
    char field_1EB;
    char field_1ec;
    char field_1ED;
    char field_1EE;

    int field_235; //??

    int field_24D; //??

    int field_265; //??

    int field_281; //??

    int field_299; //??

    int field_2B1; //??

    int field_2C9; //??

    int field_2E1; //??

    cellArea *pcell;

    int field_4F5; //??
    int field_4F9; //??
    int field_4FD; //??

    char field_501;
};

struct NC_STACK_yparobo : public NC_STACK_ypabact
{
    __NC_STACK_yparobo stack__yparobo;
};

#endif // YROBO_H_INCLUDED
