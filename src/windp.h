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




struct _NC_STACK_ypaworld;
struct UserData;

void sb_0x4c9f14(_NC_STACK_ypaworld *yw);
void sub_46B328(UserData *usr);
void sub_46B1B8(UserData *usr);
void sb_0x46bb54(UserData *usr);
void sub_46D698(UserData *usr);
void ypaworld_func158__sub1(UserData *usr);
void ypaworld_func158__sub2(_NC_STACK_ypaworld *yw);
int ypaworld_func158__sub0__sub8(UserData *usr, const char**, const char**);
void sb_0x4deac0(UserData *usr);

#endif // WINDP_H_INCLUDED
