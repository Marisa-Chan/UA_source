#ifndef WORLD_PROTOS_H_INCLUDED
#define WORLD_PROTOS_H_INCLUDED

#include "../system/sound.h"
#include "../nucleas.h"
#include "../sample.h"
#include "../skeleton.h"

class NC_STACK_ypabact;

namespace World
{
struct TRoboProto;

struct DestFX
{
    enum FXTYPES {
        FX_NONE = 0,
        FX_DEATH,    // "death"
        FX_MEGADETH, // "megadeth"
        FX_CREATE,   // "create"
        FX_BEAM      // "beam"
    };
    
    uint8_t Type = FX_NONE;
    int ModelID  = 0; // Model id. >= 0
    vec3d Pos;
    bool Accel   = false;

    static uint8_t ParseTypeName(const std::string &in);
};


struct TRoboColl
{
    float robo_coll_radius = 0.0;
    vec3d coll_pos;
    vec3d field_10;
};

struct rbcolls
{
    int field_0 = 0;
    std::vector<TRoboColl> roboColls;
    
    rbcolls() {};
    
    rbcolls(const rbcolls &b)
    {
        operator =(b);
    }
    
    rbcolls(rbcolls &&b)
    {
        field_0 = b.field_0;
        roboColls = std::move(b.roboColls);
    }
    
    rbcolls &operator=(const rbcolls &b)
    {
        field_0 = b.field_0;
        roboColls = b.roboColls;
        return *this;
    }
};
    
struct TVhclSound
{
    struct TSndSample
    {
        std::string Name;
        NC_STACK_sample *Sample = NULL;
        
        ~TSndSample()
        {
            if (Sample)
            {
                Sample->Delete();
                Sample = NULL;
            }
        }
    };
    
    TSndSample MainSample;
    
    std::vector<TSndSample> ExtSamples;
    int16_t volume = 0;
    int16_t pitch = 0;
    TSndFXParam sndPrm;
    TSndFxPosParam sndPrm_shk;
    std::vector<TSampleParams> extS;
    
    void LoadSamples();
    void ClearSounds();
};

struct TVhclProto
{
    enum
    {
        SND_NORMAL  = 0,
        SND_FIRE    = 1,
        SND_WAIT    = 2,
        SND_GENESIS = 3,
        SND_EXPLODE = 4,
        SND_CRSHLND = 5,
        SND_CRSHVHCL= 6,
        SND_GODOWN  = 7,
        SND_COCKPIT = 8,
        SND_BEAMIN  = 9,
        SND_BEAMOUT = 10,
        SND_BUILD   = 11,
        
        SND_MAX     = 12
    };
    
    inline static bool IsLoopingSnd(int i)
    {
        switch (i)
        {
            default:
                return false;
            
            case SND_NORMAL:    
            case SND_FIRE:
            case SND_WAIT:
            case SND_GENESIS:
            case SND_GODOWN:
            case SND_COCKPIT:
                return true;                      
        }
        return false;
    }
    
    int32_t Index = -1;
    int model_id = 0;
    uint8_t disable_enable_bitmask = 0;
    int8_t weapon = 0;
    int field_4 = 0;
    int8_t mgun = 0;
    uint8_t type_icon = 0;
    std::string name;
    int16_t vp_normal = 0;
    int16_t vp_fire = 0;
    int16_t vp_dead = 0;
    int16_t vp_wait = 0;
    int16_t vp_megadeth = 0;
    int16_t vp_genesis = 0;
    std::vector<DestFX> dest_fx;      // dest_fx
    std::vector<DestFX>    ExtDestroyFX; // ext_dest_fx
    std::array<TVhclSound, SND_MAX> sndFX;
    int vo_type = 0;
    float max_pitch = 0.0;
    int16_t field_1D6D = 0;
    int16_t field_1D6F = 0;
    int shield = 0;
    int energy = 0;
    int field_1D79 = 0;
    float adist_sector = 0.0;
    float adist_bact = 0.0;
    float sdist_sector = 0.0;
    float sdist_bact = 0.0;
    int8_t radar = 0;
    float mass = 0.0;
    float force = 0.0;
    float airconst = 0.0;
    float maxrot = 0.0;
    float height = 0.0;
    float radius = 0.0;
    float overeof = 0.0;
    float vwr_radius = 0.0;
    float vwr_overeof = 0.0;
    float gun_angle = 0.0;
    float fire_x = 0.0;
    float fire_y = 0.0;
    float fire_z = 0.0;
    int16_t num_weapons = 0;
    float gun_power = 0.0;
    float gun_radius = 0.0;
    int kill_after_shot = 0;
    float scale_fx_p0 = 0.0;
    float scale_fx_p1 = 0.0;
    float scale_fx_p2 = 0.0;
    int scale_fx_p3 = 0;
    std::array<int16_t, 32> scale_fx_pXX;
    int8_t job_fighttank = 0;
    int8_t job_fighthelicopter = 0;
    int8_t job_fightflyer = 0;
    int8_t job_fightrobo = 0;
    int8_t job_conquer = 0;
    int8_t job_reconnoitre = 0;
    NC_STACK_skeleton *wireframe = NULL;
    NC_STACK_skeleton *hud_wireframe = NULL;
    NC_STACK_skeleton *mg_wireframe = NULL;
    NC_STACK_skeleton *wpn_wireframe_1 = NULL;
    NC_STACK_skeleton *wpn_wireframe_2 = NULL;
    IDVList initParams;
    
    bool hidden = false;
    int8_t unhideRadar = 0;
    
    TRoboProto *RoboProto = NULL;

    ~TVhclProto();
};

struct TWeapProto
{
    enum
    {
        SND_NORMAL = 0,
        SND_LAUNCH = 1,
        SND_HIT    = 2,
        
        SND_MAX    = 3
    };
    
    int8_t unitID = 0;
    uint8_t enable_mask = 0;
    int16_t _weaponFlags = 0;
    uint8_t type_icon = 0;
    std::string name;
    int16_t vp_normal = 0;
    int16_t vp_fire = 0;
    int16_t vp_dead = 0;
    int16_t vp_wait = 0;
    int16_t vp_megadeth = 0;
    int16_t vp_genesis = 0;
    std::vector<DestFX> dfx;
    std::vector<DestFX> ExtDestroyFX; // ext_dest_fx
    std::array<TVhclSound, SND_MAX> sndFXes;
//    int field_870 = 0;
//    int field_874 = 0;
    int energy = 0;
//    int field_87C = 0;
    int life_time = 0;
    int life_time_nt = 0;
    int drive_time = 0;
    int delay_time = 0;
    float adistSector = 0;
    float adistBact = 0;
    int shot_time = 0;
    int shot_time_user = 0;
    int salve_shots = 0;
    int salve_delay = 0;
    float energy_heli = 0.0;
    float energy_tank = 0.0;
    float energy_flyer = 0.0;
    float energy_robo = 0.0;
    float radius_heli = 0.0;
    float radius_tank = 0.0;
    float radius_flyer = 0.0;
    float radius_robo = 0.0;
    float mass = 0.0;
    float force = 0.0;
    float airconst = 0.0;
    float maxrot = 0.0;
    float heightStd = 0;
    float radius = 0.0;
    float overeof = 0.0;
    float vwr_radius = 0.0;
    float vwr_overeof = 0.0;
    float start_speed = 0.0;
    NC_STACK_skeleton *wireframe = NULL;
    IDVList initParams;
    
    ~TWeapProto();
};



struct TBuildingProto
{
    struct TGun
    {
        int32_t VhclID = 0;
        vec3d Pos;
        vec3d Dir;
    };
    
    int32_t Index = -1;
    uint8_t SecType = 0;
    uint8_t EnableMask = 0;
    uint8_t ModelID = 0;
    uint8_t Power = 0;
    uint8_t TypeIcon = 0;
    std::string Name;
    int Energy = 0;
    TVhclSound SndFX;
    std::vector<TGun> Guns;
};

struct TRoboGun
{
    vec3d pos;
    vec3d dir;
    NC_STACK_ypabact *gun_obj = NULL;
    std::string robo_gun_name;
    uint8_t robo_gun_type = 0;

    TRoboGun()
    {}
    
    TRoboGun(const TRoboGun &b)
    {
        operator =(b);
    }
    
    TRoboGun(TRoboGun &&b)
    {
        pos = b.pos;
        dir = b.dir;
        gun_obj = b.gun_obj;
        robo_gun_name = std::move(b.robo_gun_name);
        robo_gun_type = b.robo_gun_type;
    }

    TRoboGun& operator=(const TRoboGun &b)
    {
        pos = b.pos;
        dir = b.dir;
        gun_obj = b.gun_obj;
        robo_gun_name = b.robo_gun_name;
        robo_gun_type = b.robo_gun_type;
        return *this;
    }
};

struct TRoboProto
{
    vec3d viewer;
    mat3x3 matrix;
    int field_30 = 0;
    int field_34 = 0;
    float robo_viewer_max_up = 0.0;
    float robo_viewer_max_down = 0.0;
    float robo_viewer_max_side = 0.0;
    std::vector<TRoboGun> guns;
    vec3d dock;
    rbcolls coll;

    TRoboProto()
    {}
    
    TRoboProto(const TRoboProto &b)
    {
        operator =(b);
    }
    
    TRoboProto(TRoboProto &&b)
    {
        viewer = b.viewer;
        matrix = b.matrix;
        field_30 = b.field_30;
        field_34 = b.field_34;
        robo_viewer_max_up = b.robo_viewer_max_up;
        robo_viewer_max_down = b.robo_viewer_max_down;
        robo_viewer_max_side = b.robo_viewer_max_side;
        guns = std::move(b.guns);
        dock = b.dock;
        coll = b.coll;
    }
    
    TRoboProto &operator=(const TRoboProto &b)
    {
        viewer = b.viewer;
        matrix = b.matrix;
        field_30 = b.field_30;
        field_34 = b.field_34;
        robo_viewer_max_up = b.robo_viewer_max_up;
        robo_viewer_max_down = b.robo_viewer_max_down;
        robo_viewer_max_side = b.robo_viewer_max_side;
        guns = b.guns;
        dock = b.dock;
        coll = b.coll;
        return *this;
    }
};

}

#endif