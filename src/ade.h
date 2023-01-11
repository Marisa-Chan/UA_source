#ifndef ADE_H_INCLUDED
#define ADE_H_INCLUDED

#include "nucleas.h"
#include "system/gfx.h"

class NC_STACK_ade;
struct area_arg_65;

typedef std::list<NC_STACK_ade *> AdeList;

struct ADE_STRC
{
    int16_t version;
    int8_t _nu1; // Not used
    int8_t flags;
    int16_t point;
    int16_t poly;
    int16_t _nu2; // Not used
};

class NC_STACK_ade: public NC_STACK_nucleus
{
public:
    class InstanceOpts
    {
    public:
        InstanceOpts(NC_STACK_ade *ade) : Ade(ade) {};
        virtual ~InstanceOpts() {};
        
    public:
        NC_STACK_ade *Ade = NULL;
    };
    
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **file);
    virtual size_t ade_func64(AdeList &lst);

    NC_STACK_ade() {};
    virtual ~NC_STACK_ade() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum ADE_FLAG
    {
//        ADE_FLAG_INLIST = 1,
        ADE_FLAG_DPTHFADE = 2,
        ADE_FLAG_BKCHECK = 4
    };

    enum ADE_ATT
    {
        ADE_ATT_BKCHECK = 0x80001003,
        ADE_ATT_DPTHFADE = 0x80001004,
        ADE_ATT_POINT = 0x80001007,
        ADE_ATT_POLY = 0x80001008,
        ADE_ATT_PADE = 0x8000100B
    };

    virtual int getADE_bkCheck();
    virtual int getADE_depthFade();
    virtual int getADE_point();
    virtual int getADE_poly();
//    virtual __NC_STACK_ade *getADE_pAde();

    virtual void setADE_bkCheck(int);
    virtual void setADE_depthFade(int);
    virtual void setADE_point(int);
    virtual void setADE_poly(int);
    
    virtual InstanceOpts *GenRenderInstance();
    virtual bool IsParticle() const { return false; };
    
    virtual void GenMesh(std::list<GFX::TMesh> *meshList, NC_STACK_skeleton * skelet) {};
    
    virtual GFX::TRenderParams GetRenderParams( size_t polyID = 0 ) { return GFX::TRenderParams(); };
    virtual std::vector<tUtV> *GetOutline( size_t polyID = 0 ) { return NULL; };

    //Data
    static constexpr const char * __ClassName = "ade.class";


public:
    uint8_t flags = 0;
    int16_t _pointID = 0;
    int16_t _polyID = 0;

    AdeList *AttachedTo = NULL;
};

#endif // ADE_H_INCLUDED
