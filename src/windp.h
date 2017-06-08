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


struct windp_t1
{
    char name[64];
    //int guid[4];
    //void *connection;
};

struct windp_intern
{
    void init();

//  int connect_numb;
    windp_t1 connections[64];
    /*  _DWORD dword1504;
      IDirectPlay3 *dplay3;
      int providerID;
      int connType;
      int num_sessions;
      windp_t2 sessions[64];
      int act_sess;
      windp_t2 own_sess;
      int sess_joined;
      int sess_created;
      char version_ident[128];*/
    int version_check;
    /*windp_t3 players[4];
    int num_players;*/
    char *norm_block;
    //char *big_block;
    int norm_size;
    /*int big_size;
    int guaranteed;
    char *sndBuff;
    int sndBuff_size;
    int sndBuff_off;*/
    int guaranteed_md;
    /*nlist recv_list;
    nlist send_list;
    HANDLE mevent;*/
    int debug;
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

struct windp_recvMsg
{
    void *data;
    uint32_t size;
    char *recvID;
    uint32_t recvFlags;
    char *senderID;
    uint32_t senderFlags;
    uint32_t msgType;
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
void yw_CheckCDs(UserData *usr);
void yw_CheckCRCs(_NC_STACK_ypaworld *yw);
int ypaworld_func158__sub0__sub8(UserData *usr, const char**, const char**);
void yw_NetPrintStartInfo(UserData *usr);
int yw_DestroyPlayer(_NC_STACK_ypaworld *yw, const char *playerName);



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
void ypaworld_func151__sub7(UserData *usr);



class NC_STACK_windp: public NC_STACK_network
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t windp_func64();
    virtual size_t windp_func65(windp_getNameMsg *arg);
    virtual size_t windp_func66(const char *provName);
    virtual size_t windp_func67(windp_t1 **pconn);
    virtual size_t windp_func68(stack_vals *stak);
    virtual size_t windp_func69(windp_getNameMsg *arg);
    virtual size_t windp_func70(const char *stak);
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
    virtual size_t windp_func81(windp_recvMsg *recv);
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
        memset(&wdp_intern, 0, sizeof(wdp_intern));
    };
    virtual ~NC_STACK_windp() {};

    virtual const char * getClassName() {
        return "windp.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_windp();
    };


    bool init();
    void deinit();
    int getNumPlayers();



    //Data
    static const NewClassDescr description;

    windp_intern wdp_intern;
};

#endif // WINDP_H_INCLUDED
