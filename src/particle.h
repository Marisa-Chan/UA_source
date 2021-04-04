#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "nucleas.h"
#include "area.h"
#include "ade.h"

class NC_STACK_particle;

class NC_STACK_particle: public NC_STACK_ade
{
public:
    
    class SpawnOpts : public InstanceOpts
    {
    public:
        SpawnOpts(NC_STACK_ade *ade) : InstanceOpts(ade) {};
        virtual ~SpawnOpts() {};
        
        int32_t Time = 0;
        int32_t TimeStamp = -1;
        int32_t Age = 0;
    };

    struct StartEnd
    {
        vec3d start;
        vec3d end;
    };
    
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **file);
    virtual size_t ade_func65(area_arg_65 *arg, InstanceOpts * opts = NULL);
    virtual void SetAccel(const vec3d &start, const vec3d &end);
    virtual void SetMagnify(const vec3d &start, const vec3d &end);
    virtual void AddLifeStage(NC_STACK_area *ade);
    virtual NC_STACK_ade * ExtractLifeStage(uint32_t id);

    NC_STACK_particle()
    {};
    
    virtual ~NC_STACK_particle() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance()
    {
        return new NC_STACK_particle();
    };

    enum PRTCL_ATT
    {
        PRTCL_ATT_STSPEED = 0x80002000,
        PRTCL_ATT_NUMCONT = 0x80002001,
        PRTCL_ATT_CONTLFTIME = 0x80002002,
        PRTCL_ATT_BIRTRATE = 0x80002003,
        PRTCL_ATT_LFTIME = 0x80002004,
        PRTCL_ATT_PADE = 0x80002005,
        PRTCL_ATT_STSIZE = 0x80002006,
        PRTCL_ATT_ENDSIZE = 0x80002007,
        PRTCL_ATT_STGEN = 0x80002008,
        PRTCL_ATT_ENDGEN = 0x80002009,
        PRTCL_ATT_NOISE = 0x8000200A,
        PRTCL_ATT_PPADE = 0x8000200B
    };

    virtual void setADE_depthFade(int);
    virtual void setADE_point(int);

    virtual void SetStartSpeed(int);
    virtual void SetNumContexts(int);
    virtual void SetContextLifeTime(int);
    virtual void SetGenRate(int);
    virtual void SetParticleLifeTime(int);
    virtual void SetParticlesOneStage(NC_STACK_ade *);
    virtual void SetSizeStart(int);
    virtual void SetSizeEnd(int);
    virtual void SetGenStartTime(int);
    virtual void SetGenEndTime(int);
    virtual void SetNoisePower(int);
    virtual void SetParticlesStages(const std::vector<NC_STACK_ade *> &);

    virtual int GetStartSpeed();
    virtual int GetNumContexts();
    virtual int GetContextLifeTime();
    virtual int GetGenRate();
    virtual int GetParticleLifeTime();
    virtual NC_STACK_ade *GetParticleOneStage();
    virtual int GetSizeStart();
    virtual int GetSizeEnd();
    virtual int GetGenStartTime();
    virtual int GetGenEndTime();
    virtual int GetNoisePower();
    virtual std::vector<NC_STACK_ade *> &GetParticlesStages();

    int ReadIFFAtts(IFFile *mfile);
    
    virtual InstanceOpts *GenRenderInstance() override;

private:
    void UpdateScaleDelta();
    void UpdateLifeStages();
    void UpdateAccelMagnify();
    void _SetLifeStages(const std::vector<NC_STACK_ade *> &);

public:
    static float Rand();
    static vec3d RandVec();
    
private:
    static std::array<float, 1024> _randomTable;
    static uint32_t _randomIndex;
    
    //Data
public:
    static const Nucleus::ClassDescr description;

public:
    bool _depthFade = false;
    size_t _onVerticeID = 0;
    int _lifePerAde = 1;
    std::vector<NC_STACK_ade *> _lifeStagesAdes;
    vec3d _accelStart;
    vec3d _accelEnd;
    vec3d _accelDelta;
    vec3d _magnifyStart;
    vec3d _magnifyEnd;
    vec3d _magnifyDelta;
    int32_t _lifeTime = 3000; // Particle
    int32_t _genRate = 10;
    int32_t _genPause = 0;
    float _scaleStart = 30.0;
    float _scaleEnd = 30.0;
    float _scaleDelta = 0;
    float _startSpeed = 50.0;
    float _noisePower = 0.0;
    int32_t _numContexts = 1; // Only for store
    int32_t _genLifeTime = 1000; // Generator cycle time until next
    int32_t _genStart = 0;
    int32_t _genEnd = 1000;
    
};

#endif // PARTICLE_H_INCLUDED
