#ifndef WINDP_H_INCLUDED
#define WINDP_H_INCLUDED


#include "nucleas.h"
#include "network.h"
#include "lstvw.h"

struct __NC_STACK_windp
{
    int dummy;
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



struct netgamelst: public listbase
{
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    char field_1DC[64];
    int field_21C;
};

extern netgamelst netgame_wnd;

void sb_0x451034__sub6(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub7__sub5(_NC_STACK_ypaworld *yw, struC5 *inpt);
void ypaworld_func64__sub18(_NC_STACK_ypaworld *yw);
void ypaworld_func64__sub10(_NC_STACK_ypaworld *yw);
void ypaworld_func151__sub7(UserData *usr);




class NC_STACK_windp: public NC_STACK_network
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t windp_func64(stack_vals *stak);
    virtual size_t windp_func65(stack_vals *stak);
    virtual size_t windp_func66(stack_vals *stak);
    virtual size_t windp_func67(stack_vals *stak);
    virtual size_t windp_func68(stack_vals *stak);
    virtual size_t windp_func69(int *);
    virtual size_t windp_func70(stack_vals *stak);
    virtual size_t windp_func71(stack_vals *stak);
    virtual size_t windp_func72(stack_vals *stak);
    virtual size_t windp_func73(stack_vals *stak);
    virtual size_t windp_func74(stack_vals *stak);
    virtual size_t windp_func75(stack_vals *stak);
    virtual size_t windp_func76(stack_vals *stak);
    virtual size_t windp_func77(stack_vals *stak);
    virtual size_t windp_func78(stack_vals *stak);
    virtual size_t windp_func79(windp_arg79 *stak);
    virtual size_t windp_func80(stack_vals *stak);
    virtual size_t windp_func81(stack_vals *stak);
    virtual size_t windp_func82(windp_arg82 *stak);
    virtual size_t windp_func83(stack_vals *stak);
    virtual size_t windp_func84(int *);
    virtual size_t windp_func85(stack_vals *stak);
    virtual size_t windp_func86(stack_vals *stak);
    virtual size_t windp_func87(windp_arg87 *arg);
    virtual size_t windp_func88(stack_vals *stak);
    virtual size_t windp_func89(const char **arg);
    virtual size_t windp_func90(stack_vals *stak);
    virtual size_t windp_func91(int *);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_windp() {
        memset(&stack__windp, 0, sizeof(stack__windp));
    };
    virtual ~NC_STACK_windp() {};

    virtual const char * getClassName() {
        return "windp.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_windp();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_windp stack__windp;
};

#endif // WINDP_H_INCLUDED
