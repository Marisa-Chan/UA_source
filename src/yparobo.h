#ifndef YROBO_H_INCLUDED
#define YROBO_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"


class NC_STACK_yparobo;

struct robo_t1
{
    int field_0;
    int field_4;
};

struct __NC_STACK_yparobo
{
    NC_STACK_ypaworld *wrld;
    _NC_STACK_ypaworld *wrld_yw;
    __NC_STACK_ypabact *bact_internal;
    NC_STACK_yparobo *roboo;

    float field_c;
    rbcolls coll;
    float field_1D5;

    char field_1DA;
    int field_1DB;
    int field_1DF;
    int field_1E3;
    char field_1E7;
    char field_1e8;
    char field_1E9;
    char field_1EA;
    char field_1EB;
    char field_1ec;
    char field_1ED;
    char field_1EE;
    int dock_energ;
    int dock_cnt;
    int dock_user;
    int dock_time;
    xyz dock_tgt_pos;
    __NC_STACK_ypabact *dock_tgt_bact;
    char dock_tgType;
    char dock_aggr;
    int dock_tgt_comm_id;
    xyz dock_pos;
    int field_221;
    cellArea *field_225;  //For AI
    int field_229;  //For AI
    int field_22D;
    int field_231;
    int field_235; //??
    int field_239;
    cellArea *field_23D;  //For AI
    int field_241;  //For AI
    int field_245;
    int field_249;
    int field_24D; //??
    int field_251;
    cellArea *field_255;  //For AI
    int field_259;  //For AI
    int field_25D;
    int field_261;
    int field_265; //??
    int field_269;
    cellArea *field_26D;  //For AI
    int field_271;  //For AI
    int field_275;  //For AI
    int field_279;
    int field_27D;
    int field_281; //??
    int field_285;
    cellArea *field_289;  //For AI
    int field_28D;  //For AI
    int field_291;
    int field_295;
    int field_299; //??
    int field_29D;
    cellArea *field_2A1;  //For AI
    int field_2A5;  //For AI
    int field_2A9;  //For AI
    int field_2AD;
    int field_2B1; //??
    int field_2B5;
    cellArea *field_2B9;  //For AI
    int field_2BD;  //For AI
    int field_2C1;  //For AI
    int field_2C5;
    int field_2C9; //??
    int field_2CD;
    cellArea *field_2D1;  //For AI
    int field_2D5;  //For AI
    int field_2D9;  //For AI
    int field_2DD;
    int field_2E1; //??
    int field_2E5;  //For AI
    cellArea *field_2E9;  //For AI

    int field_2ED;  //For AI
    int field_2F1;  //For AI
    int field_2F5;  //For AI
    cellArea *field_2F9;  //For AI
    int field_2FD;  //For AI
    cellArea *pcell;

    int field_309;
    roboGun guns[8];

    int field_4F5; //??
    int field_4F9; //??
    int field_4FD; //??

    char field_501;
    char field_502;
    char field_503;

    int field_505;
    int field_509;
    int field_50D;
    int field_511;
    xyz field_515;
    int field_521;
    robo_t1 field_525[16];
    int field_5A5;
    int field_5A9;
};

struct robo_arg128
{
    int flags;
    BactTarget tgt;
    char tgType;
    xyz tgt_pos;
    int prim_comm_id;
    int comm_id;
    float distance;
    __NC_STACK_ypabact *comm_bact;
    NC_STACK_ypabact *comm_bacto;
};

struct robo_arg133
{
    int type;
    int num;
    xyz pos;
    int field_14;
    int *hetero_vehicles;
};

struct robo_arg134
{
    __NC_STACK_ypabact *unit;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
};

struct robo_t2
{
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
};

struct robo_loct1
{
    int energ;
    float distance;
    int aggr;
    char tgType;
    xyz tgt_pos;
    __NC_STACK_ypabact *tgt_bact;
    int command_id;
    int forbidden;
    int want;
    int good;
    int bad;
    int job;
};

class NC_STACK_yparobo: public NC_STACK_ypabact
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func68(ypabact_arg65 *arg);
    virtual void ypabact_func70(ypabact_arg65 *arg);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual void ypabact_func74(bact_arg74 *arg);
    virtual void ypabact_func77(void *);
    virtual size_t ypabact_func80(bact_arg80 *arg);
    virtual void ypabact_func82(ypabact_arg65 *arg);
    virtual void ypabact_func96(void *);
    virtual void ypabact_func97(ypabact_arg65 *);
    virtual void ypabact_func114(void *arg);
    virtual void ypabact_func121(ypabact_arg65 *arg);
    virtual void yparobo_func128(robo_arg128 *arg);
    virtual void yparobo_func129(stack_vals *arg);
    virtual void yparobo_func130(stack_vals *arg);
    virtual void yparobo_func131(stack_vals *arg);
    virtual size_t yparobo_func132(bact_arg67 *arg);
    virtual int yparobo_func133(robo_arg133 *arg);
    virtual int yparobo_func134(robo_arg134 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_yparobo() {
        memset(&stack__yparobo, 0, sizeof(stack__yparobo));
    };
    virtual ~NC_STACK_yparobo() {};

    virtual const char * getClassName() {
        return "yparobo.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_yparobo();
    };

    enum ROBO_ATT
    {
        ROBO_ATT_PROTO = 0x80002001,
        ROBO_ATT_EP_CONQUER = 0x80002002,
        ROBO_ATT_EP_DEFENSE = 0x80002003,
        ROBO_ATT_EP_RADAR = 0x80002004,
        ROBO_ATT_EP_POWER = 0x80002005,
        ROBO_ATT_EP_SAFETY = 0x80002006,
        ROBO_ATT_COMM_COUNT = 0x80002007,
        ROBO_ATT_BATT_VEHICLE = 0x80002008,
        ROBO_ATT_BATT_BUILDING = 0x80002009,
        ROBO_ATT_BATT_BEAM = 0x8000200A,
        ROBO_ATT_FILL_MODE = 0x8000200B,
        ROBO_ATT_WAIT_SWAY = 0x8000200C,
        ROBO_ATT_WAIT_ROTATE = 0x8000200D,
        ROBO_ATT_GUNS = 0x8000200E,
        ROBO_ATT_EP_CHANGEPLACE = 0x8000200F,
        ROBO_ATT_EP_RECONNOITRE = 0x80002010,
        ROBO_ATT_EP_ROBO = 0x80002011,
        ROBO_ATT_ROBOSTATE = 0x80002012,
        ROBO_ATT_VIEWANGLE = 0x80002013,
        ROBO_ATT_SAFDELAY = 0x80002014,
        ROBO_ATT_POWDELAY = 0x80002015,
        ROBO_ATT_RADDELAY = 0x80002016,
        ROBO_ATT_CPLDELAY = 0x80002017,
        ROBO_ATT_DEFDELAY = 0x80002018,
        ROBO_ATT_CONDELAY = 0x80002019,
        ROBO_ATT_ROBDELAY = 0x8000201A,
        ROBO_ATT_RECDELAY = 0x8000201B,
        ROBO_ATT_LOADFLAGS = 0x8000201C,
        ROBO_ATT_LOSSFLAGS = 0x8000201D,
        ROBO_ATT_ABSRELOAD = 0x8000201E
    };


    virtual void setBACT_inputting(int);
    virtual void setROBO_proto(roboProto *);
    virtual void setROBO_epConquer(int);
    virtual void setROBO_epDefense(int);
    virtual void setROBO_epRadar(int);
    virtual void setROBO_epPower(int);
    virtual void setROBO_epSafety(int);
    virtual void setROBO_commCount(int);
    virtual void setROBO_battVehicle(int);
    virtual void setROBO_battBuilding(int);
    virtual void setROBO_battBeam(int);
    virtual void setROBO_fillMode(int);
    virtual void setROBO_waitSway(int);
    virtual void setROBO_waitRotate(int);
    virtual void setROBO_epChangeplace(int);
    virtual void setROBO_epReconnoitre(int);
    virtual void setROBO_epRobo(int);
    virtual void setROBO_viewAngle(int);
    virtual void setROBO_safDelay(int);
    virtual void setROBO_powDelay(int);
    virtual void setROBO_radDelay(int);
    virtual void setROBO_cplDelay(int);
    virtual void setROBO_defDelay(int);
    virtual void setROBO_conDelay(int);
    virtual void setROBO_robDelay(int);
    virtual void setROBO_recDelay(int);

    virtual rbcolls *getBACT_collNodes();
    virtual int getROBO_epConquer();
    virtual int getROBO_epDefense();
    virtual int getROBO_epRadar();
    virtual int getROBO_epPower();
    virtual int getROBO_epSafety();
    virtual int getROBO_commCount();
    virtual int getROBO_battVehicle();
    virtual int getROBO_battBuilding();
    virtual int getROBO_battBeam();
    virtual int getROBO_fillMode();
    virtual roboGun *getROBO_guns();
    virtual int getROBO_epChangeplace();
    virtual int getROBO_epReconnoitre();
    virtual int getROBO_epRobo();
    virtual int getROBO_roboState();
    virtual int getROBO_safDelay();
    virtual int getROBO_powDelay();
    virtual int getROBO_radDelay();
    virtual int getROBO_cplDelay();
    virtual int getROBO_defDelay();
    virtual int getROBO_conDelay();
    virtual int getROBO_robDelay();
    virtual int getROBO_recDelay();
    virtual int getROBO_loadFlags();
    virtual int getROBO_lossFlags();
    virtual int getROBO_absReload();


    void yparobo_func0__sub1(stack_vals *stak);
    void yparobo_func2__sub0(stack_vals *stak);
    void yparobo_func3__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    __NC_STACK_yparobo stack__yparobo;
};

void ypabact_func65__sub0(__NC_STACK_ypabact *bact);

#endif // YROBO_H_INCLUDED
