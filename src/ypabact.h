#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"
#include "engine_tform.h"
#include "base.h"
#include "listnode.h"

// !!!! if period is small, then this never happen
#define BACT_MIN_ANGLE 0.0002

class NC_STACK_ypabact;
struct __NC_STACK_ypabact;
class NC_STACK_ypamissile;

class NC_STACK_ypaworld;
struct _NC_STACK_ypaworld;

class NC_STACK_yparobo;

struct yw_arg129;

struct cellArea;

typedef RefList<NC_STACK_ypabact *> YpabactList;
typedef RefList<NC_STACK_ypamissile *> YpamissileList;

struct destFX
{
    char type_flag;
    int ModelID; // Model id. >= 0
    vec3d pos;

    destFX()
    {
        clear();
    }

    void clear()
    {
        type_flag = 0;
        ModelID = 0;
        pos = 0.0;
    }
};


struct bact_node : public nnode
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;

    bact_node()
    {
        bacto = NULL;
        bact = NULL;
    }
};

enum EVPROTO_FLAG
{
    EVPROTO_FLAG_ACTIVE = 1,
    EVPROTO_FLAG_SCALE  = 2
};

struct extra_vproto
{
    float scale;
    vec3d pos;
    mat3x3 rotate;
    int flags;
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

enum BACT_STATUS
{
    BACT_STATUS_NOPE = 0,
    BACT_STATUS_NORMAL = 1,
    BACT_STATUS_DEAD = 2,
    BACT_STATUS_IDLE = 3,
    BACT_STATUS_CREATE = 4,
    BACT_STATUS_BEAM = 5
};

enum BACT_STFLAG
{
    BACT_STFLAG_FIGHT_P     = 1, // Primary target fight
    BACT_STFLAG_FIGHT_S     = 2, // Secondary target fight
    BACT_STFLAG_FORMATION   = 4,
    //BACT_STFLAG_DODGE = 8,          //Unused flag
    BACT_STFLAG_DODGE_LEFT  = 0x10,
    BACT_STFLAG_DODGE_RIGHT = 0x20,
    BACT_STFLAG_MOVE        = 0x40,
    BACT_STFLAG_UPWRD       = 0x80,
    BACT_STFLAG_FIRE        = 0x100,
    BACT_STFLAG_LAND        = 0x200,
    BACT_STFLAG_DEATH1      = 0x400,
    BACT_STFLAG_DEATH2      = 0x800,
    //BACT_STFLAG_AKKU        = 0x1000,  //Unused flag
    BACT_STFLAG_APPROACH    = 0x2000,
    BACT_STFLAG_ESCAPE      = 0x4000,
    BACT_STFLAG_XLEFT       = 0x8000,
    BACT_STFLAG_YUP         = 0x10000,
    BACT_STFLAG_BCRASH      = 0x20000,
    BACT_STFLAG_LCRASH      = 0x40000,
    BACT_STFLAG_UNUSE       = 0x80000,
    BACT_STFLAG_SCALE       = 0x100000,
    BACT_STFLAG_SHAKE       = 0x200000,
    BACT_STFLAG_NORENDER    = 0x400000,
    BACT_STFLAG_ISVIEW      = 0x800000,
    BACT_STFLAG_SEFFECT     = 0x1000000,
    BACT_STFLAG_CLEAN       = 0x2000000,
    BACT_STFLAG_WAYPOINT    = 0x4000000,
    BACT_STFLAG_WAYPOINTCCL = 0x8000000,
    BACT_STFLAG_NOMSG       = 0x10000000,
    BACT_STFLAG_DSETTED     = 0x20000000,
    BACT_STFLAG_ATTACK      = 0x40000000
};

enum BACT_OFLAG
{
    BACT_OFLAG_VIEWER     = 1,
    BACT_OFLAG_USERINPT   = 2,
    BACT_OFLAG_EXACTCOLL  = 4,
    BACT_OFLAG_BACTCOLL   = 8,
    BACT_OFLAG_LANDONWAIT = 0x10,
    BACT_OFLAG_EXTRAVIEW  = 0x20,
    BACT_OFLAG_ALWAYSREND = 0x40
};

union BactTarget
{
    __NC_STACK_ypabact *pbact;
    cellArea *pcell;
};


struct roboColl
{
    float robo_coll_radius;
    vec3d coll_pos;
    vec3d field_10;

    roboColl()
    {
        clear();
    }

    void clear()
    {
        robo_coll_radius = 0.;
        coll_pos = vec3d();
        field_10 = vec3d();
    }
};

struct rbcolls
{
    int field_0;
    int8_t robo_coll_num;
    std::array<roboColl, 16> roboColls;

    rbcolls()
    {
        clear();
    }

    void clear()
    {
        field_0 = 0;
        robo_coll_num = 0;
        for ( auto &c : roboColls )
            c.clear();
    }
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
    uint32_t gid; // global bact id
    uint8_t vehicleID; // vehicle id, from scr files
    char bflags;
    int commandID;
    NC_STACK_yparobo *host_station; // parent robo?
    NC_STACK_ypabact *parent_bacto;
    __NC_STACK_ypabact *parent_bact;
    nlist subjects_list;
    bact_node subject_node;
    samples_collection1 soundcarrier;
    int soundFlags;
    int volume;
    int pitch;
    float pitch_max;
    int energy;
    int energy_max;
    int reload_const;
//    int16_t field_3CE;
    char shield;
//    char field_3D1;
    char radar; // num sectors view
    uint8_t owner;
    char aggr;
    char status;
    int status_flg; //Additional status flags
//    int field_3DA;
    char primTtype;
    char secndTtype;
    int primT_cmdID;
    int secndT_cmdID;
    BactTarget primT;
    vec3d primTpos;
    BactTarget secndT;
    vec3d sencdTpos;

    float adist_sector;
    float adist_bact;
    float sdist_sector;
    float sdist_bact;
    vec3d waypoints[32]; //waypoints
    int16_t current_waypoint;
    int16_t waypoints_count;
    int m_cmdID;
    uint8_t m_owner;
    int fe_cmdID; // found enemy group ID
    int fe_time; //
    float mass;
    float force;
    float airconst;
    float airconst_static;
    float maxrot;

    vec3d viewer_position;
    mat3x3 viewer_rotation;
    float viewer_horiz_angle;
    float viewer_vert_angle;
    float viewer_max_up;
    float viewer_max_down;
    float viewer_max_side;

    float thraction;
    vec3d fly_dir;
    float fly_dir_length;

    vec3d position; //Current pos
    vec3d old_pos; //Prev pos
    vec3d target_vec; //Vector to target
    vec3d target_dir; //Target 1-vector direction
    mat3x3 rotation;

    float height;
    float height_max_user;
    vec3d scale;
    vhclBases vp_normal;
    vhclBases vp_fire;
    vhclBases vp_wait;
    vhclBases vp_dead;
    vhclBases vp_megadeth;
    vhclBases vp_genesis;
    int vp_active;
    extra_vproto vp_extra[3];
    int vp_extra_mode;
    std::array<destFX, 16> destroyFX;
    float radius;
    float viewer_radius;
    float overeof;
    float viewer_overeof;
//    float pos_x_cntr;
//
//    float pos_y_cntr;

    TFEngine::TForm3D tForm;
    int clock;           // local time
    int AI_time1;
    int AI_time2;
//    int field_921;
//    int field_925;
    int search_time1;
    int search_time2;
    int scale_time;
    int brkfr_time;
    int brkfr_time2;
//    int field_93D;
    int newtarget_time;
    int assess_time;
    int waitCol_time; //Used in tank
    int slider_time;
//    int field_951;
    int dead_time;
    int beam_time;
    int energy_time;
    vec3d mpos;
    int weapon;
    char weapon_flags;
    int mgun;
    char num_weapons;

    YpamissileList missiles_list;
    int weapon_time;
    vec3d fire_pos;
    float gun_angle;
    float gun_angle_user;
    float gun_leftright;
    float gun_radius;
    float gun_power;
    int mgun_time;
    int salve_counter;
    int kill_after_shot;
    float heading_speed;
    __NC_STACK_ypabact *killer;
    int16_t killer_owner;
    int16_t reb_count;
    int atk_ret;
    uint32_t lastFrmStamp;
    mat3x3 netDRot;
    mat3x3 netRotation;
    vec3d netDSpeed;
    float scale_start;
    float scale_speed;
    float scale_accel;
    int scale_duration;
    int scale_pos;
    int scale_delay;
    NC_STACK_base *vp_fx_models[32];
    TFEngine::TForm3D *vp_fx_tform[32];

    int oflags;
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    vhclBases current_vp;
    nlist attackers_list;
    bact_node attack_node_prim;
    bact_node attack_node_scnd;
    int yls_time;

    __NC_STACK_ypabact();
};

struct newMaster_msg
{
    NC_STACK_ypabact *bacto;
    __NC_STACK_ypabact *bact;
    nlist *list;
};

struct bact_arg80
{
    vec3d pos;
    int field_C;
};

struct setState_msg
{
    int newStatus;
    int setFlags;
    int unsetFlags;
};

struct update_msg
{
    int gTime;
    int frameTime;
    struC5 *inpt;
    int units_count;
    int user_action;
    int protoID;
    __NC_STACK_ypabact *selectBact;
    cellArea *target_Sect;
    int target_sect_x;
    int target_sect_y;
    int target_point_flags;
    vec3d target_point;
    __NC_STACK_ypabact *target_Bact;
    int energy;

    update_msg()
    {
        gTime = 0;
        frameTime = 0;
        inpt = NULL;
        units_count = 0;
        user_action = 0;
        protoID = 0;
        selectBact = NULL;
        target_Sect = NULL;
        target_sect_x = 0;
        target_sect_y = 0;
        target_point_flags = 0;
        target_Bact = NULL;
        energy = 0;
    }
};

struct setTarget_msg
{
    char tgt_type;
    int priority;
    BactTarget tgt;
    vec3d tgt_pos;
};

struct bact_arg124
{
    vec2d from;
    vec2d to;
    int16_t steps_cnt;
    int16_t field_12;
    vec3d waypoints[32];
};

struct bact_arg84
{
    int energy;
    __NC_STACK_ypabact *unit;
};

struct bact_arg88
{
    vec3d pos1;
};

struct bact_arg94
{
    int field_0;
    vec3d pos1;
    vec3d pos2;
};

struct move_msg
{
    float field_0;
    vec3d vec;
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
    vec3d pos;
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
    vec3d field_0;
    float field_C;
    int field_10;
};

struct bact_arg106
{
    int field_0;
    vec3d field_4;
    __NC_STACK_ypabact *ret_bact;
};

struct bact_arg79
{
    vec3d direction;
    vec3d start_point;
    int tgType;
    BactTarget target;
    vec3d tgt_pos;
    int weapon;
    int g_time;
    int flags;
};

struct bact_arg75
{
    vec3d pos;
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
    vec3d pos;
    int unkn;
    float radius;
};

struct bact_arg83
{
    int energ;
    vec3d pos;
    vec3d pos2;
    float force;
    float mass;
};


class NC_STACK_ypabact: public NC_STACK_nucleus
{
public:
    enum TA
    {
        TA_CANCEL,
        TA_MOVE,
        TA_FIGHT,
        TA_IGNORE
    };
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual size_t func3(IDVList &stak);
    virtual void Update(update_msg *arg);
    virtual void Render(baseRender_msg *arg);
    virtual void SetTarget(setTarget_msg *arg);
    virtual void AI_layer1(update_msg *arg);
    virtual void AI_layer2(update_msg *arg);
    virtual void AI_layer3(update_msg *arg);
    virtual void User_layer(update_msg *arg);
    virtual void AddSubject(NC_STACK_ypabact *kid);
    virtual void SetNewMaster(newMaster_msg *arg);
    virtual void Move(move_msg *arg);
    virtual void FightWithBact(bact_arg75 *arg);
    virtual void FightWithSect(bact_arg75 *arg);
    virtual void Die();
    virtual void SetState(setState_msg *arg);
    virtual size_t LaunchMissile(bact_arg79 *arg);
    virtual size_t SetPosition(bact_arg80 *arg);
    virtual void GetSummary(bact_arg81 *arg);
    virtual void EnergyInteract(update_msg *arg);
    virtual void ApplyImpulse(bact_arg83 *arg);
    virtual void ModifyEnergy(bact_arg84 *arg);
    virtual bool ypabact_func85(vec3d *arg);
    virtual size_t CrashOrLand(bact_arg86 *arg);
    virtual size_t CollisionWithBact(int arg);
    virtual void Recoil(bact_arg88 *arg);
    virtual void ypabact_func89(IDVPair *arg);
    virtual void GetSectorTarget(bact_arg90 *arg);
    virtual void GetBestSectorPart(vec3d *arg);
    virtual void GetForcesRatio(bact_arg92 *arg);
    virtual void ypabact_func93(IDVPair *arg);
    virtual void GetFormationPosition(bact_arg94 *arg);
    virtual void ypabact_func95(IDVPair *arg);
    virtual void Renew();
    virtual void HandBrake(update_msg *arg);
    virtual void ypabact_func98(IDVPair *arg);
    virtual void CreationTimeUpdate(update_msg *arg);
    virtual size_t IsDestroyed();
    virtual size_t CheckFireAI(bact_arg101 *arg);
    virtual void MarkSectorsForView(IDVPair *arg);
    virtual void ypabact_func103(IDVPair *arg);
    virtual void StuckFree(update_msg *arg);
    virtual size_t FireMinigun(bact_arg105 *arg);
    virtual size_t UserTargeting(bact_arg106 *arg);
    virtual void HandleVisChildrens(int *arg);
    virtual size_t GetFightMotivation(float *arg);
    virtual void ReorganizeGroup(bact_arg109 *arg);
    virtual size_t TargetAssess(bact_arg110 *arg);
    virtual size_t TestTargetSector(__NC_STACK_ypabact *);
    virtual void BeamingTimeUpdate(update_msg *arg);
    virtual void StartDestFX(uint8_t arg);
    virtual void CorrectPositionOnLand();
    virtual void CorrectPositionInLevelBox(void *);
    virtual void NetUpdate(update_msg *arg);
    virtual void ypabact_func117(update_msg *arg);
    virtual void Release();
    virtual size_t SetStateInternal(setState_msg *arg);
    virtual void ChangeSectorEnergy(yw_arg129 *arg);
    virtual void DeadTimeUpdate(update_msg *arg);
    virtual void ypabact_func122(update_msg *arg);
    virtual void ypabact_func123(update_msg *arg);
    virtual size_t PathFinder(bact_arg124 *arg);
    virtual size_t SetPath(bact_arg124 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypabact()
    {
        _world = NULL;
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
    virtual void setBACT_vpTransform(TFEngine::TForm3D *);
    virtual void setBACT_extraViewer(int);
    virtual void setBACT_alwaysRender(int);


    virtual NC_STACK_ypaworld *getBACT_pWorld();
    virtual TFEngine::TForm3D *getBACT_pTransform();
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
    virtual TFEngine::TForm3D *getBACT_vpTransform();
    virtual int getBACT_extraViewer();
    virtual bact_node *getBACT_primAttackNode();
    virtual bact_node *getBACT_secnAttackNode();
    virtual int getBACT_alwaysRender();

    void DoTargetWaypoint();
    void FixSectorFall();
    void FixBeyondTheWorld();

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_ypabact ypabact;
    NC_STACK_ypaworld *_world;
};

void sub_493DB0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, NC_STACK_ypaworld *ywo);
void sb_0x4874c4(__NC_STACK_ypabact *bact, int a2, int a3, float a4);

#endif // YBACT_H_INCLUDED
