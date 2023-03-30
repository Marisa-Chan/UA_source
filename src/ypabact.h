#ifndef YBACT_H_INCLUDED
#define YBACT_H_INCLUDED

#include "nucleas.h"
#include "system/gfx.h"
#include "base.h"
#include "listnode.h"
#include "types.h"
#include "world/protos.h"

// !!!! if period is small, then this never happen
#define BACT_MIN_ANGLE 0.0002

class NC_STACK_ypabact;
class NC_STACK_yparobo;
class NC_STACK_ypamissile;

class NC_STACK_ypaworld;

struct yw_arg129;

struct cellArea;





enum EVPROTO_FLAG
{
    EVPROTO_FLAG_ACTIVE = 1,
    EVPROTO_FLAG_SCALE  = 2
};

struct extra_vproto
{
    float scale = 0.0;
    vec3d pos;
    mat3x3 rotate;
    int flags = 0;
    NC_STACK_base::Instance *vp = NULL;
    
    ~extra_vproto()
    {
        if (vp)
            delete vp;
    }
    
    void SetVP(NC_STACK_base::Instance *newVP)
    {
        Common::DeleteAndNull(&vp);
        vp = newVP;
    }
    
    void SetVP(NC_STACK_base *bas)
    {
        Common::DeleteAndNull(&vp);
        
        if (bas)
            vp = bas->GenRenderInstance();
        else
            vp = NULL;
    }
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
    NC_STACK_ypabact *pbact;
    cellArea *pcell;
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

struct newMaster_msg
{
    NC_STACK_ypabact *bact;
    World::RefBactList *list;
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
    
    setState_msg()
    {
        newStatus = 0;
        setFlags = 0;
        unsetFlags = 0;
    }
};

struct update_msg
{
    int gTime = 0;
    int frameTime = 0;
    TInputState *inpt = NULL;
    int units_count = 0;
    int user_action = 0;
    int protoID = 0;
    NC_STACK_ypabact *selectBact = NULL;
    cellArea *target_Sect = NULL;
    vec3d target_point;
    NC_STACK_ypabact *target_Bact = NULL;
    int energy = 0;
};

struct setTarget_msg
{
    uint8_t tgt_type;
    uint32_t priority;
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
    NC_STACK_ypabact *unit;
};

struct bact_arg88
{
    vec3d pos1;
};

struct bact_arg94
{
    int field_0;
    vec3d pos1;
    //vec3d pos2;
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

struct bact_arg109
{
    int field_0;
    NC_STACK_ypabact *field_4;
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
    NC_STACK_ypabact *field_18;

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
    NC_STACK_ypabact *ret_bact;
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
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t SetParameters(IDVList &stak);
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
    virtual NC_STACK_ypabact *GetSectorTarget(Common::Point CellId) const;
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
    virtual void MarkSectorsForView();
    virtual void ypabact_func103(IDVPair *arg);
    virtual void StuckFree(update_msg *arg);
    virtual size_t FireMinigun(bact_arg105 *arg);
    virtual size_t UserTargeting(bact_arg106 *arg);
    virtual void HandleVisChildrens(int *arg);
    virtual bool GetFightMotivation(float *arg);
    virtual void ReorganizeGroup(bact_arg109 *arg);
    virtual size_t TargetAssess(bact_arg110 *arg);
    virtual bool TestTargetSector(const NC_STACK_ypabact *) const { return true; };
    virtual void BeamingTimeUpdate(update_msg *arg);
    virtual void StartDestFXByType(uint8_t arg);
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

    NC_STACK_ypabact();
    virtual ~NC_STACK_ypabact();
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum BACT_ATT
    {
        BACT_ATT_WORLD = 0x80001001,
        BACT_ATT_PTRANSFORM = 0x80001002,
        BACT_ATT_VIEWER = 0x80001004,       // bool
        BACT_ATT_INPUTTING = 0x80001005,    // bool
        BACT_ATT_EXACTCOLL = 0x80001006,    // bool
        BACT_ATT_BACTCOLL = 0x80001007,     // bool
        BACT_ATT_ATTACKLIST = 0x80001008,
        BACT_ATT_AIRCONST = 0x80001009,
        BACT_ATT_LANDINGONWAIT = 0x8000100A, // bool
        BACT_ATT_YOURLS = 0x8000100B,
        BACT_ATT_VISPROT = 0x8000100C,
        BACT_ATT_AGGRESSION = 0x8000100D,
        BACT_ATT_COLLNODES = 0x8000100E,
        //BACT_ATT_VPTRANSFORM = 0x8000100F,
        BACT_ATT_EXTRAVIEWER = 0x80001010,  // bool
        BACT_ATT_P_ATTACKNODE = 0x80001011,
        BACT_ATT_S_ATTACKNODE = 0x80001012,
        BACT_ATT_ALWAYSRENDER = 0x80001013  // bool
    };

    virtual void setBACT_viewer(bool);
    virtual void setBACT_inputting(bool);
    virtual void setBACT_exactCollisions(bool);
    virtual void setBACT_bactCollisions(bool);
    virtual void setBACT_airconst(int);
    virtual void setBACT_landingOnWait(bool);
    virtual void setBACT_yourLastSeconds(int);
    virtual void SetVP(NC_STACK_base *vp);
    virtual void setBACT_aggression(int);
    virtual void setBACT_extraViewer(bool);
    virtual void setBACT_alwaysRender(bool);

    NC_STACK_ypabact * GetEnemyCandidateInSector(const cellArea &cell, float *radius, char *job) const;

    virtual NC_STACK_ypaworld *getBACT_pWorld()
    { return _world; }
    
    virtual TF::TForm3D *getBACT_pTransform()
    { return &_tForm; }
    
    virtual bool getBACT_viewer() const 
    { return (_oflags & BACT_OFLAG_VIEWER) != 0; }
    
    virtual bool getBACT_inputting() const 
    { return (_oflags & BACT_OFLAG_USERINPT) != 0; }

    virtual bool getBACT_exactCollisions() const 
    { return (_oflags & BACT_OFLAG_EXACTCOLL) != 0; }
    
    virtual bool getBACT_bactCollisions() const 
    { return (_oflags & BACT_OFLAG_BACTCOLL) != 0; }
    
    virtual bool getBACT_landingOnWait() const 
    { return (_oflags & BACT_OFLAG_LANDONWAIT) != 0; }
    
    virtual int getBACT_yourLastSeconds() const 
    { return _yls_time; }
    
    virtual NC_STACK_base *GetVP()
    {
        if (_current_vp)
            return _current_vp->Bas;

        return NULL;
    }
    
    virtual int getBACT_aggression() const
    { return _aggr; }
    
    virtual World::rbcolls *getBACT_collNodes()
    { return NULL; }
    
    virtual bool getBACT_extraViewer() const
    { return (_oflags & BACT_OFLAG_EXTRAVIEW) != 0; }    
    
    virtual bool getBACT_alwaysRender() const
    { return (_oflags & BACT_OFLAG_ALWAYSREND) != 0; }
    
    
    void ChangeEscapeFlag(bool escape);
    
    virtual bool IsGroundUnit() const { return false; };
    
    bool IsNeedsWaypoints() const;
    bool IsAnyKidWithoutSecondUnitTarget() const;
    
    void sub_4843BC(NC_STACK_ypabact *bact2, int a3);
    void sub_493480(NC_STACK_ypabact *bact2, int mode);
    void StartDestFX(const World::DestFX &fx);

    void DoTargetWaypoint();
    void FixSectorFall();
    void FixBeyondTheWorld();
    void CleanAttackersTarget();
    
    void CopyTargetOf(NC_STACK_ypabact *commander);
    
    bool IsParentMyRobo() const;
    
    void CopyWaypointsStuff(NC_STACK_ypabact *bact);
    
    World::RefBactList &GetKidList() { return _kidList; }
    

    // static methods for return correspond for reflist  kid ref node
    static World::RefBactList::Node& GetCellRefNode(NC_STACK_ypabact *&bact)
    {
        return bact->_cellRef;
    }
    
    static World::RefBactList::Node& GetKidRefNode(NC_STACK_ypabact *&bact)
    {
        return bact->_kidRef;
    }
 
protected:
    void SetKidsPath(int beginWp);

    //Data
public:
    static constexpr const char * __ClassName = "ypabact.class";
public:    
    
    World::RefBactList::Node _cellRef;
    
    Common::Point _cellId;
    cellArea *_pSector = NULL;
    
    vec2d _wrldSize;
    
    Common::Point _wrldSectors;
 
    int _bact_type;
    uint32_t _gid = 0; // global bact id
    uint8_t _vehicleID; // vehicle id, from scr files
    uint8_t _bflags;
    uint32_t _commandID = 0;
    NC_STACK_yparobo *_host_station; // parent robo?
    NC_STACK_ypabact *_parent;
    World::RefBactList _kidList;
    World::RefBactList::Node _kidRef;
    TSndCarrier _soundcarrier;
    int _soundFlags;
    int _volume;
    int _pitch;
    float _pitch_max;
    int _energy;
    int _energy_max;
    int _reload_const;
//    int16_t field_3CE;
    uint8_t _shield;
//    char field_3D1;
    uint8_t _radar; // num sectors view
    uint8_t _owner;
    uint8_t _aggr;
    uint8_t _status;
//    uint64_t paddiong;
    int _status_flg; //Additional status flags
//    int field_3DA;
    uint8_t _primTtype;
    uint8_t _secndTtype;
    uint32_t _primT_cmdID;
    uint32_t _secndT_cmdID;
    BactTarget _primT;
    vec3d _primTpos;
    BactTarget _secndT;
    vec3d _sencdTpos;

    float _adist_sector;
    float _adist_bact;
    float _sdist_sector;
    float _sdist_bact;
    vec3d _waypoints[32]; //waypoints
    int16_t _current_waypoint;
    int16_t _waypoints_count;
    int _m_cmdID;
    uint8_t _m_owner;
    uint32_t _fe_cmdID; // found enemy group ID
    int _fe_time; //
    float _mass;
    float _force;
    float _airconst;
    float _airconst_static;
    float _maxrot;

    vec3d _viewer_position;
    mat3x3 _viewer_rotation;
    float _viewer_horiz_angle;
    float _viewer_vert_angle;
    float _viewer_max_up;
    float _viewer_max_down;
    float _viewer_max_side;

    float _thraction;
    vec3d _fly_dir;
    float _fly_dir_length;

    vec3d _position; //Current pos
    vec3d _old_pos; //Prev pos
    vec3d _target_vec; //Vector to target
    vec3d _target_dir; //Target 1-vector direction
    mat3x3 _rotation;

    float _height;
    float _height_max_user;
    vec3d _scale;
    NC_STACK_base *_vp_normal;
    NC_STACK_base *_vp_fire;
    NC_STACK_base *_vp_wait;
    NC_STACK_base *_vp_dead;
    NC_STACK_base *_vp_megadeth;
    NC_STACK_base *_vp_genesis;
    int _vp_active;
    extra_vproto _vp_extra[3];
    int _vp_extra_mode;
    std::vector<World::DestFX> _destroyFX;    // dest_fx
    std::vector<World::DestFX> _extDestroyFX; // ext_dest_fx
    float _radius;
    float _viewer_radius;
    float _overeof;
    float _viewer_overeof;
//    float pos_x_cntr;
//
//    float pos_y_cntr;

    TF::TForm3D _tForm;
    int _clock;           // local time
    int _AI_time1;
    int _AI_time2;
//    int field_921;
//    int field_925;
    int _search_time1;
    int _search_time2;
    int _scale_time;
    int _brkfr_time;
    int _brkfr_time2;
//    int field_93D;
    int _newtarget_time;
    int _assess_time;
    int _waitCol_time; //Used in tank
    int _slider_time;
//    int field_951;
    int _dead_time;
    int _beam_time;
    int _energy_time;
    vec3d _mpos;
    int _weapon;
    uint8_t _weapon_flags;
    int _mgun;
    uint8_t _num_weapons;

    World::MissileList _missiles_list;
    int _weapon_time;
    vec3d _fire_pos;
    float _gun_angle;
    float _gun_angle_user;
    float _gun_leftright;
    float _gun_radius;
    float _gun_power;
    int _mgun_time;
    int _salve_counter;
    int _kill_after_shot;
    float _heading_speed;
    NC_STACK_ypabact *_killer;
    int16_t _killer_owner;
    int16_t _reb_count;
    int _atk_ret;
    uint32_t _lastFrmStamp;
    mat3x3 _netDRot;
    mat3x3 _netRotation;
    vec3d _netDSpeed;
    float _scale_start;
    float _scale_speed;
    float _scale_accel;
    int _scale_duration;
    int _scale_pos;
    int _scale_delay;
    NC_STACK_base *_vp_fx_models[32];

    int _oflags;
    NC_STACK_ypaworld *_yw;
    NC_STACK_base::Instance *_current_vp = NULL;
    World::BactList _attackersList;
    int _yls_time;  
    
protected:
    NC_STACK_ypaworld *_world;
};

void sb_0x4874c4(NC_STACK_ypabact *bact, int a2, int a3, float a4);

#endif // YBACT_H_INCLUDED
