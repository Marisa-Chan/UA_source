#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"
#include "engine_tform.h"
#include "base.h"


// !!!! if period is small, then this never happen
#define BACT_MIN_ANGLE 0.0002

class NC_STACK_ypabact;
struct __NC_STACK_ypabact;

class NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;

class NC_STACK_yparobo;

struct yw_arg129;

struct cellArea;


struct bact_node : public nnode
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;
};

struct bact_6bd
{
    float field_0;
    xyz field_4;
    mat3x3 matrx;
    int field_34;
    vhclBases vp;
};

enum BACT_TGT_TYPE
{
    BACT_TGT_TYPE_NONE = 0,
    BACT_TGT_TYPE_CELL = 1,
    BACT_TGT_TYPE_UNIT = 2,
    BACT_TGT_TYPE_FRMT = 3,
    BACT_TGT_TYPE_DRCT = 4,
    BACT_TGT_TYPE_CELL_IND = 5,
    BACT_TGT_TYPE_UNIT_IND = 6
};

union BactTarget
{
    __NC_STACK_ypabact *pbact;
    cellArea *pcell;
};


struct roboColl
{
    float robo_coll_radius;
    xyz coll_pos;
    xyz field_10;
};

struct rbcolls
{
    int field_0;
    char robo_coll_num;
    roboColl roboColls[16];
};

enum BACT_TYPES
{
    BACT_TYPES_NOPE = 0,
    BACT_TYPES_BACT = 1,
    BACT_TYPES_TANK = 2,
    BACT_TYPES_ROBO = 3,
    BACT_TYPES_MISSLE = 4,
    BACT_TYPES_ZEPP = 5,  //No real class
    BACT_TYPES_FLYER = 6,
    BACT_TYPES_UFO = 7,
    BACT_TYPES_CAR = 8,
    BACT_TYPES_GUN = 9,
    BACT_TYPES_HOVER = 10 //No real class
};

struct __NC_STACK_ypabact : public nnode
{
    NC_STACK_ypabact *self;
    int sectX;
    int sectY;
    cellArea *pSector;
    float wrldX;
    float wrldY;
    int16_t secMaxX;
    int16_t secMaxY;
    int bact_type;
    int ypabact__id;
    uint8_t id;
    char field_2D;
    int field_2E;
    NC_STACK_yparobo *host_station; // parent robo?
    NC_STACK_ypabact *parent_bacto;
    __NC_STACK_ypabact *parent_bact;
    nlist list2;
    bact_node list_node;
    samples_collection1 field_5A;
    int field_3B2;
    int field_3B6;
    int field_3BA;
    float max_pitch;
    int energy;
    int energy_2;
    int reload_const_or_energy2;
    int16_t field_3CE;
    char shield;
    char field_3D1;
    char radar;
    uint8_t owner;
    char field_3D4;
    char field_3D5;
    int field_3D6;
    int field_3DA;
    char primTtype;
    char secndTtype;
    int primT_cmd_id;
    int secndT_cmd_id;
    BactTarget primT;
    xyz primTpos;
    BactTarget secndT;
    xyz sencdTpos;

    float adist_sector;
    float adist_bact;
    float sdist_sector;
    float sdist_bact;
    xyz field_418[32]; //waypoints
    int16_t field_598;
    int16_t field_59A;
    int field_59c;
    uint8_t field_5a0;
    int field_5A1;
    int field_5A5;
    float mass;
    float force;
    float airconst;
    float airconst_static;
    float maxrot;

    xyz viewer_position;
    mat3x3 viewer_rotation;
    float viewer_horiz_angle;
    float viewer_vert_angle;
    float viewer_max_up;
    float viewer_max_down;
    float viewer_max_side;

    float thraction;
    xyz fly_dir;
    float fly_dir_length;

    xyz position; //Current pos
    xyz old_pos; //Prev pos
    xyz target_vec; //Vector to target
    xyz target_dir; //Target 1-vector direction
    mat3x3 rotation;

    float height;
    float height_max_user;
    xyz scale;
    vhclBases vp_normal;
    vhclBases vp_fire;
    vhclBases vp_wait;
    vhclBases vp_dead;
    vhclBases vp_megadeth;
    vhclBases vp_genesis;
    int field_6B9;
    bact_6bd field_6BD[3];

    destFX dest_fx[16];
    float radius;
    float vwr_radius;
    float overeof;
    float vwr_overeof;
    float pos_x_cntr;

    float pos_y_cntr;

    base_1c_struct field_87D;
    int field_915;
    int field_919;
    int field_91D;
    int field_921;
    int field_925;
    int field_929;
    int field_92D;
    int field_931;
    int field_935;
    int field_939;
    int field_93D;
    int field_941;
    int field_945;
    int field_949; //Used in tank
    int field_94D;
    int field_951;
    int field_955;
    int field_959;
    int field_95D;
    xyz field_961;
    int weapon;
    char field_96E;
    int mgun;
    char num_weapons;
    nlist list3;
    int field_97D;
    float fire_x;
    float fire_y;
    float fire_z;
    float gun_angle;
    float gun_angle2;
    float field_995;
    float gun_radius;
    float gun_power;
    int field_9A1;
    int field_9A5;
    int kill_after_shot;
    float field_9AD;
    __NC_STACK_ypabact *field_9B1;
    int16_t field_9B5;
    int16_t field_9B6;
    int field_9b8;

    float field_a14;
    float field_a18;
    float field_a1c;
    int field_a20;
    int field_a24;
    int field_a28;
    NC_STACK_base *models_bases[32];
    base_1c_struct *models_trigo[32];

    int field_B34;
    NC_STACK_ypaworld *wrld;
    _NC_STACK_ypaworld * field_B3C;
    vhclBases current_vp;
    nlist field_B48;

    bact_node field_B54;
    bact_node field_B64;
    int field_B74;
};

struct bact_arg73
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;
    nlist *list;
};

struct bact_arg80
{
    xyz pos;
    int field_C;
};

struct bact_arg119
{
    int field_0;
    int field_4;
    int field_8;
};

struct ypabact_arg65
{
    int field_0;
    int field_4;
    struC5 *inpt;
    int numid;
    int field_10;
    int field_14;
    int field_18;
    __NC_STACK_ypabact *field_1C;
    cellArea *field_20;
    int field_24;
    int field_28;
    int field_2C;
    xyz field_30;
    __NC_STACK_ypabact *field_3C;
    int field_40;

    ypabact_arg65()
    {
        field_0 = 0;
        field_4 = 0;
        inpt = NULL;
        numid = 0;
        field_10 = 0;
        field_14 = 0;
        field_18 = 0;
        field_1C = NULL;
        field_20 = NULL;
        field_24 = 0;
        field_28 = 0;
        field_2C = 0;
        field_3C = NULL;
        field_40 = 0;
    }
};

struct bact_arg67
{
    char tgt_type;
    int priority;
    BactTarget tgt;
    xyz tgt_pos;
};

struct bact_arg124
{
    float from_x;
    float from_z;
    float to_x;
    float to_z;
    int16_t steps_cnt;
    int16_t field_12;
    xyz waypoints[32];
};

struct bact_arg84
{
    int energy;
    __NC_STACK_ypabact *unit;
};

struct bact_arg88
{
    xyz pos1;
    xyz pos2;
};

struct bact_arg94
{
    int field_0;
    xyz pos1;
    xyz pos2;
};

struct bact_arg74
{
    float field_0;
    xyz vec;
    int flag;
};

struct bact_arg81
{
    int enrg_type;
    int enrg_sum;
};

struct bact_arg90
{
    __NC_STACK_ypabact *unit;
    __NC_STACK_ypabact *ret_unit;
    int field_8;
    float pos_x;
    float pos_z;
};

struct bact_arg109
{
    int field_0;
    __NC_STACK_ypabact *field_4;
};

struct bact_arg92
{
    xyz pos;
    int energ1;
    int energ2;
    int field_14;
};

struct bact_hudi
{
    int field_0;
    int field_4;
    float field_8;
    float field_C;
    float field_10;
    float field_14;
    __NC_STACK_ypabact *field_18;

    bact_hudi()
    {
        field_0 = 0;
        field_4 = 0;
        field_8 = 0.0;
        field_C = 0.0;
        field_10 = 0.0;
        field_14 = 0.0;
        field_18 = NULL;
    }
};

struct bact_arg105
{
    xyz field_0;
    float field_C;
    int field_10;
};

struct bact_arg106
{
    int field_0;
    xyz field_4;
    __NC_STACK_ypabact *ret_bact;
};

struct bact_arg79
{
    xyz direction;
    xyz start_point;
    int tgType;
    BactTarget target;
    xyz tgt_pos;
    int weapon;
    int g_time;
    int flags;
};

struct bact_arg75
{
    xyz pos;
    BactTarget target;
    int prio;
    float fperiod;
    int g_time;
};

struct bact_arg110
{
    int tgType;
    int priority;
};

struct bact_arg86
{
    int field_one;
    int field_two;
};

struct bact_arg101
{
    xyz pos;
    int unkn;
    float radius;
};

struct bact_arg83
{
    int energ;
    xyz pos;
    xyz pos2;
    float force;
    float mass;
};


class NC_STACK_ypabact: public NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func65(ypabact_arg65 *arg);
    virtual void ypabact_func66(base77Func *arg);
    virtual void ypabact_func67(bact_arg67 *arg);
    virtual void ypabact_func68(ypabact_arg65 *arg);
    virtual void ypabact_func69(ypabact_arg65 *arg);
    virtual void ypabact_func70(ypabact_arg65 *arg);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual void ypabact_func72(NC_STACK_ypabact *kid);
    virtual void ypabact_func73(bact_arg73 *arg);
    virtual void ypabact_func74(bact_arg74 *arg);
    virtual void ypabact_func75(bact_arg75 *arg);
    virtual void ypabact_func76(bact_arg75 *arg);
    virtual void ypabact_func77(void *);
    virtual void ypabact_func78(bact_arg119 *arg);
    virtual size_t ypabact_func79(bact_arg79 *arg);
    virtual size_t ypabact_func80(bact_arg80 *arg);
    virtual void ypabact_func81(bact_arg81 *arg);
    virtual void ypabact_func82(ypabact_arg65 *arg);
    virtual void ypabact_func83(bact_arg83 *arg);
    virtual void ypabact_func84(bact_arg84 *arg);
    virtual void ypabact_func85(stack_vals *arg);
    virtual size_t ypabact_func86(bact_arg86 *arg);
    virtual size_t ypabact_func87(int *arg);
    virtual void ypabact_func88(bact_arg88 *arg);
    virtual void ypabact_func89(stack_vals *arg);
    virtual void ypabact_func90(bact_arg90 *arg);
    virtual void ypabact_func91(xyz *arg);
    virtual void ypabact_func92(bact_arg92 *arg);
    virtual void ypabact_func93(stack_vals *arg);
    virtual void ypabact_func94(bact_arg94 *arg);
    virtual void ypabact_func95(stack_vals *arg);
    virtual void ypabact_func96(void *);
    virtual void ypabact_func97(ypabact_arg65 *arg);
    virtual void ypabact_func98(stack_vals *arg);
    virtual void ypabact_func99(ypabact_arg65 *arg);
    virtual size_t ypabact_func100(void *);
    virtual size_t ypabact_func101(bact_arg101 *arg);
    virtual void ypabact_func102(stack_vals *arg);
    virtual void ypabact_func103(stack_vals *arg);
    virtual void ypabact_func104(ypabact_arg65 *arg);
    virtual size_t ypabact_func105(bact_arg105 *arg);
    virtual size_t ypabact_func106(bact_arg106 *arg);
    virtual void ypabact_func107(int *arg);
    virtual size_t ypabact_func108(float *arg);
    virtual void ypabact_func109(bact_arg109 *arg);
    virtual size_t ypabact_func110(bact_arg110 *arg);
    virtual size_t ypabact_func111(__NC_STACK_ypabact *);
    virtual void ypabact_func112(ypabact_arg65 *arg);
    virtual void ypabact_func113(uint8_t *arg);
    virtual void ypabact_func114(void *);
    virtual void ypabact_func115(void *);
    virtual void ypabact_func116(ypabact_arg65 *arg);
    virtual void ypabact_func117(stack_vals *arg);
    virtual void ypabact_func118(NC_STACK_ypabact *b_bacto);
    virtual size_t ypabact_func119(bact_arg119 *arg);
    virtual void ypabact_func120(yw_arg129 *arg);
    virtual void ypabact_func121(ypabact_arg65 *arg);
    virtual void ypabact_func122(stack_vals *arg);
    virtual void ypabact_func123(stack_vals *arg);
    virtual size_t ypabact_func124(bact_arg124 *arg);
    virtual size_t ypabact_func125(bact_arg124 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypabact()
    {
        memset(&stack__ypabact, 0, sizeof(stack__ypabact) );
    };
    virtual ~NC_STACK_ypabact() {};

    virtual const char * getClassName()
    {
        return "ypabact.class";
    };

    static NC_STACK_nucleus * newinstance()
    {
        return new NC_STACK_ypabact();
    };

    enum BACT_ATT
    {
        BACT_ATT_WORLD = 0x80001001,
        BACT_ATT_PTRANSFORM = 0x80001002,
        BACT_ATT_PBACT = 0x80001003,
        BACT_ATT_VIEWER = 0x80001004,
        BACT_ATT_INPUTTING = 0x80001005,
        BACT_ATT_EXACTCOLL = 0x80001006,
        BACT_ATT_BACTCOLL = 0x80001007,
        BACT_ATT_ATTACKLIST = 0x80001008,
        BACT_ATT_AIRCONST = 0x80001009,
        BACT_ATT_LANDINGONWAIT = 0x8000100A,
        BACT_ATT_YOURLS = 0x8000100B,
        BACT_ATT_VISPROT = 0x8000100C,
        BACT_ATT_AGGRESSION = 0x8000100D,
        BACT_ATT_COLLNODES = 0x8000100E,
        BACT_ATT_VPTRANSFORM = 0x8000100F,
        BACT_ATT_EXTRAVIEWER = 0x80001010,
        BACT_ATT_P_ATTACKNODE = 0x80001011,
        BACT_ATT_S_ATTACKNODE = 0x80001012,
        BACT_ATT_ALWAYSRENDER = 0x80001013
    };

    virtual void setBACT_viewer(int);
    virtual void setBACT_inputting(int);
    virtual void setBACT_exactCollisions(int);
    virtual void setBACT_bactCollisions(int);
    virtual void setBACT_airconst(int);
    virtual void setBACT_landingOnWait(int);
    virtual void setBACT_yourLastSeconds(int);
    virtual void setBACT_visProto(NC_STACK_base *);
    virtual void setBACT_aggression(int);
    virtual void setBACT_vpTransform(base_1c_struct *);
    virtual void setBACT_extraViewer(int);
    virtual void setBACT_alwaysRender(int);


    virtual NC_STACK_ypaworld *getBACT_pWorld();
    virtual base_1c_struct *getBACT_pTransform();
    virtual __NC_STACK_ypabact *getBACT_pBact();
    virtual int getBACT_viewer();
    virtual int getBACT_inputting();
    virtual int getBACT_exactCollisions();
    virtual int getBACT_bactCollisions();
    virtual nlist *getBACT_attackList();
    virtual int getBACT_landingOnWait();
    virtual int getBACT_yourLastSeconds();
    virtual NC_STACK_base *getBACT_visProto();
    virtual int getBACT_aggression();
    virtual rbcolls *getBACT_collNodes();
    virtual base_1c_struct *getBACT_vpTransform();
    virtual int getBACT_extraViewer();
    virtual bact_node *getBACT_primAttackNode();
    virtual bact_node *getBACT_secnAttackNode();
    virtual int getBACT_alwaysRender();


    int ypabact_func0__sub0(stack_vals *stak);
    int ypabact_func2__sub0(stack_vals *stak);
    void ypabact_func3__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypabact stack__ypabact;
};


void sub_493DB0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, NC_STACK_ypaworld *ywo);

#endif // YBACT_H_INCLUDED
