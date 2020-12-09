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

struct robo_arg128
{
    int flags;
    BactTarget tgt;
    char tgType;
    vec3d tgt_pos;
    int prim_comm_id;
    int comm_id;
    float distance;
    NC_STACK_ypabact *comm_bact;
    NC_STACK_ypabact *comm_bacto;
};

struct robo_arg134
{
    NC_STACK_ypabact *unit;
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
    vec3d tgt_pos;
    NC_STACK_ypabact *tgt_bact;
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
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual void AI_layer1(update_msg *arg);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void Move(move_msg *arg);
    virtual void Die();
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void EnergyInteract(update_msg *arg);
    virtual void Renew();
    virtual void HandBrake(update_msg *);
    virtual void CorrectPositionOnLand();
    virtual void DeadTimeUpdate(update_msg *arg);
    virtual void yparobo_func128(robo_arg128 *arg);
    virtual void yparobo_func129(IDVPair *arg);
    virtual void yparobo_func130(IDVPair *arg);
    virtual void yparobo_func131(IDVPair *arg);
    virtual size_t yparobo_func132(setTarget_msg *arg);
    virtual bool MakeSquad(const std::vector<int> &VhclIDS, vec3d pos, bool usable);
    virtual int placeMessage(robo_arg134 *arg);

    NC_STACK_yparobo();
    virtual ~NC_STACK_yparobo() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_yparobo();
    };

    enum ROBOSTATE
    {
        ROBOSTATE_SAFEZONE = 1,
        ROBOSTATE_ROBOFOUND = 2,
        ROBOSTATE_DOCKINUSE = 4,
        ROBOSTATE_SETDOCKTARGET = 8,

        ROBOSTATE_BUILDRADAR = 0x10,
        ROBOSTATE_BUILDPOWER = 0x20,
        ROBOSTATE_BUILDCONQUER = 0x40,
        ROBOSTATE_BUILDDEFENSE = 0x80,
        ROBOSTATE_BUILDSAFE = 0x100,

        ROBOSTATE_READYDEFENSE = 0x200,
        ROBOSTATE_READYCONQUER = 0x400,
        ROBOSTATE_READYRADAR = 0x800,
        ROBOSTATE_READYPOWER = 0x1000,
        ROBOSTATE_READYSAFE = 0x2000,

        ROBOSTATE_PLAYERROBO = 0x4000,
        ROBOSTATE_DANGER = 0x8000,

        ROBOSTATE_FOUNDPLACE = 0x10000,
        ROBOSTATE_CHANGEPLACE = 0x20000,
        ROBOSTATE_READYRECON = 0x40000,
        ROBOSTATE_BUILDRECON = 0x80000,
        ROBOSTATE_READYROBO = 0x100000,
        ROBOSTATE_BUILDROBO = 0x200000,

        ROBOSTATE_MOVE = 0x400000,
        ROBOSTATE_USEVHCLPARTS = 0x800000
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
    
    void ypabact_func65__sub0();

protected:

    void checkCommander();
    void InitForce(NC_STACK_ypabact *unit);
    void sub_4A9F24(NC_STACK_ypabact *unit);

    NC_STACK_ypabact *AllocForce(robo_loct1 *arg);

    void buildConquer();
    void buildDefense();
    void buildRobo();
    void buildReconnoitre();

    void buildRadar(update_msg *arg);
    void buildPower(update_msg *arg);
    void buildSafe(update_msg *arg);
    void changePlace();


    void AI_doWish(update_msg *arg);
    void AI_doMove(update_msg *arg);
    void AI_service(update_msg *arg);
    void AI_clearAttackerSlots();
    void AI_solveTrouble(update_msg *arg);
    void AI_checkWorld(update_msg *arg);

    void doBeamUpdate(int a2);
    void doUserCommands(update_msg *arg);
    void wallow(update_msg *arg);
    void searchEnemyRobo();
    void usersRoboEnergyCheck();

    void checkDanger();
    size_t checkCollisions(float a2);
    
    bool yparobo_func70__sub2__sub1();
    void yparobo_func70__sub2__sub0();
    int yparobo_func70__sub6__sub0(cellArea *cell);
    int yparobo_func70__sub6__sub2(cellArea *cell);
    int yparobo_func70__sub6__sub1(cellArea *cell);
    int yparobo_func70__sub6__sub7();
    int yparobo_func70__sub6__sub6(int *a2, int *px, int *py);
    int yparobo_func70__sub6__sub5(int *a2, int *px, int *py);
    int yparobo_func70__sub6__sub13();
    int yparobo_func70__sub6__sub12();
    int yparobo_func70__sub6__sub3(cellArea *cell);
    int yparobo_func70__sub6__sub4(cellArea *cell);
    int yparobo_func70__sub6__sub8();
    int yparobo_func70__sub6__sub9();
    int yparobo_func70__sub6__sub10();
    int yparobo_func70__sub6__sub11();
    int sub_4F4E48(int x, int y);
    float sub_4F4C6C(NC_STACK_ypabact *bctnd, int a4, int a3);
    
    int yparobo_func134__sub1(robo_arg134 *arg);
    const char * yparobo_func134__sub0(char **strings, int a2);
    
    void sub_4A4538(NC_STACK_ypabact *bact);
    void sub_4A448C(NC_STACK_ypabact *bact);
    void sub_4AB69C(NC_STACK_ypabact *bact, char a2);
    int sb_0x4a45cc__sub0(NC_STACK_ypabact *bact);
    void sb_0x4a7010__sub1__sub0(NC_STACK_ypabact *unit1, NC_STACK_ypabact *unit2);
    void sb_0x4a7010__sub1(NC_STACK_ypabact *unit, robo_t1 *rbt);
    int sub_4A5A08(NC_STACK_ypabact *bact, float a2, float a3);
    int sub_4A58C0(NC_STACK_ypabact *bact, NC_STACK_ypabact *bact2);
    
    void sub_4A1270(const vec3d &vaxis, float angle);
    void sub_4A1014(float angle);
    void sub_4A10E8(float angle);
    
    cellArea * yparobo_func0__sub0();
    void sub_4F4FF4(int a2, setTarget_msg *parg67);
    
    void yparobo_func71__sub0(update_msg *arg);
    int yparobo_func70__sub4__sub2__sub0(TBuildingProto *protos);
    int yparobo_func70__sub4__sub1__sub0(TBuildingProto *protos);
    int yparobo_func70__sub4__sub0__sub0(TBuildingProto *protos);


public:

    //Data
    static const Nucleus::ClassDescr description;

    
    float _roboFlotage;
    rbcolls _roboColls;
    float _roboYPos;

    char _roboWFlags;
    int _roboState;
    int _roboNewAI;
    int _roboTimeScale;

    char _roboEpConquer;
    char _roboEpRadar;
    char _roboEpPower;
    char _roboEpDefense;
    char _roboEpSafety;
    char _roboEpRecon;
    char _roboEpChangePlace;
    char _roboEpRobo;

    int _roboDockEnerg;
    int _roboDockCnt;
    int _roboDockUser;
    int _roboDockTime;
    vec3d _roboDockTargetPos;
    NC_STACK_ypabact *_roboDockTargetBact;
    char _roboDockTargetType;
    char _roboDockAggr;
    int _roboDockTargetCommandID;
    vec3d _roboDockPos;

    int _roboRadarValue;
    cellArea *_roboRadarCell;  //For AI
    int _roboRadarCellID;  //For AI
    int _roboRadarCellIndex;
    int _roboRadarTime;
    int _roboRadarDelay; //??

    int _roboSafetyValue;
    cellArea *_roboSafetyCell;  //For AI
    int _roboSafetyCellID;  //For AI
    int _roboSafetyCellIndex;
    int _roboSafetyTime;
    int _roboSafetyDelay; //??

    //Power
    int _roboPowerValue;
    cellArea *_roboPowerCell;  //For AI
    int _roboPowerCellID;  //For AI
    int _roboPowerCellIndex;
    int _roboPowerTime;
    int _roboPowerDelay; //??

    //Enemy
    int _roboEnemyValue;
    cellArea *_roboEnemyCell;  //For AI
    int _roboEnemyCellID;  //For AI
    int _roboEnemyCommandID;  //For AI
    int _roboEnemyTime;
    uint32_t _roboEnemyDiv;
    int _roboEnemyDelay; //??

    int _roboConqValue;
    cellArea *_roboConqCell;  //For AI
    int _roboConqCellID;  //For AI
    int _roboConqCellIndex;
    int _roboConqTime;
    int _roboConqDelay; //??

    int _roboPositionValue;
    cellArea *_roboPositionCell;  //For AI
    int _roboPositionCellID;  //For AI
    int _roboPositionCellIndex;  //For AI
    int _roboPositionTime;
    int _roboPositionDelay; //??
    
    int _roboExploreValue;
    cellArea *_roboExploreCell;  //For AI
    int _roboExploreCellID;  //For AI
    int _roboExploreCellIndex;  //For AI
    int _roboExploreTime;
    int _roboExploreDelay; //??
    
    int _roboDangerValue;
    cellArea *_roboDangerCell;  //For AI
    int _roboDangerCellID;  //For AI
    int _roboDangerCommandID;  //For AI
    int _roboDangerTime;
    int _roboDangerDelay; //??
    
    int _roboVehicleCellID;  //For AI
    cellArea *_roboVehicleCell;  //For AI
    int _roboVehicleDuty;  //For AI
    int _roboVehicleCommandID;  //For AI
    
    
    int _roboBuildingCellID;  //For AI
    cellArea *_roboBuildingCell;  //For AI
    int _roboBuildingDuty;  //For AI
    
    cellArea *_roboPCells; //Is it needed??? May be use first cell from _world?

    int _roboTestEnemyTime;
    
    std::array<roboGun, 8> _roboGuns;

    int _roboEnergyLife; //??
    int _roboEnergyBuilding; //??
    int _roboEnergyMove; //??

    char _roboFillMode;
    char _roboEnergyLoadFlags;
    char _roboEnergyLossFlags;

    int _roboEnergyReloadPS; // Reload per second
    
    int _roboBuildSpare;
    int _roboVehicleSpare;
    
    int _roboBeamTimePre;
    vec3d _roboBeamPos;
    int _roboBeamFXTime;
    
    robo_t1 _roboAttackers[16];
    int _roboAttackersTime;
    int _roboAttackersClearTime; //FIX IT
};

#endif // YROBO_H_INCLUDED
