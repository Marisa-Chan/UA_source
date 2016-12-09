#ifndef WINDP_H_INCLUDED
#define WINDP_H_INCLUDED


#include "nucleas.h"
#include "network.h"
#include "lstvw.h"

enum FREE_FRACTION
{
    FREE_FRACTION_RESISTANCE = 1,
    FREE_FRACTION_GHORKOV = 2,
    FREE_FRACTION_MIKO = 4,
    FREE_FRACTION_TAER = 8
};

struct __NC_STACK_windp
{
    int dummy;
};



enum UAMSG
{
    UAMSG_BASE = 1000,
    UAMSG_LOBBYINIT = UAMSG_BASE + 29,
    UAMSG_SETLEVEL = UAMSG_BASE + 36,
    UAMSG_CRC = UAMSG_BASE + 37,
};



struct uamessage_base
{
    uint32_t msgID;
    uint32_t tstamp;
    uint32_t msgCnt;
    uint8_t owner;
    uint8_t p[3];
};

struct uamessage_setLevel : uamessage_base
{
    char hostName[64];
    uint32_t lvlID;
    uint8_t fractions[4];
};

typedef uamessage_setLevel uamessage_lobbyInit;

struct uamessage_crc : uamessage_base
{
    uint32_t checksum;
};





struct windp_getNameMsg
{
    const char *name;
    int id;
};

struct windp_openSessionMsg
{
    const char *name;
    int maxplayers;
};

struct windp_createPlayerMsg
{
    uint32_t flags;
    const char *name;
    uint32_t data1;
    uint32_t data2;
};

struct windp_arg79
{
    int mode;
    int ID;
    char *name;
    uint32_t flags;
    uint32_t dat1;
    uint32_t dat2;
};

struct windp_arg82
{
    char *senderID;
    int senderFlags;
    char *receiverID;
    int receiverFlags;
    int guarant;
};

struct windp_arg87
{
    char callSIGN[64];
    char field_40;
    char field_41;
};




struct _NC_STACK_ypaworld;
struct UserData;

void yw_HandleNetMsg(_NC_STACK_ypaworld *yw);
void sub_46B328(UserData *usr);
void yw_NetOKProvider(UserData *usr);
void yw_JoinNetGame(UserData *usr);
void sub_46D698(UserData *usr);
void ypaworld_func158__sub1(UserData *usr);
void ypaworld_func158__sub2(_NC_STACK_ypaworld *yw);
int ypaworld_func158__sub0__sub8(UserData *usr, const char**, const char**);
void sb_0x4deac0(UserData *usr);



struct netgamelst: public GuiBase
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
    virtual size_t windp_func65(windp_getNameMsg *arg);
    virtual size_t windp_func66(const char *provName);
    virtual size_t windp_func67(stack_vals *stak);
    virtual size_t windp_func68(stack_vals *stak);
    virtual size_t windp_func69(windp_getNameMsg *arg);
    virtual size_t windp_func70(stack_vals *stak);
    virtual size_t windp_func71(windp_openSessionMsg *os);
    virtual size_t windp_func72(stack_vals *stak);
    virtual size_t windp_func73(stack_vals *stak);
    virtual size_t windp_func74(stack_vals *stak);
    virtual size_t windp_func75(const char *sessName);
    virtual size_t windp_func76(windp_createPlayerMsg *pl);
    virtual size_t windp_func77(const char *playerName);
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

    int getNumPlayers();

    //Data
    static const NewClassDescr description;

    __NC_STACK_windp stack__windp;
};

#endif // WINDP_H_INCLUDED
