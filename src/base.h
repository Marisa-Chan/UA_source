#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <deque>
#include <list>
#include "nucleas.h"
#include "system/tform.h"
#include "system/gfx.h"
#include "system/inpt.h"
#include "skeleton.h"
#include "ade.h"


class NC_STACK_base;
class NC_STACK_skeleton;
class NC_STACK_ade;

typedef std::list<NC_STACK_base *> BaseList;



struct area_arg_65
{
    //polys *rndrSTK_cur;
    //polysDat *argSTK_cur;
    int timeStamp;
    int frameTime;
    TF::TForm3D *ViewTForm;
    TF::TForm3D *OwnerTForm;
    NC_STACK_skeleton *OBJ_SKELETON;
    UAskeleton::Data *sklt;
    float minZ;
    float maxZ;
    float fadeStart;
    float fadeLength;
    int ambientLight;
    uint32_t flags;

    area_arg_65()
    {
        timeStamp = 0;
        frameTime = 0;
        ViewTForm = NULL;
        OwnerTForm = NULL;
        sklt = NULL;
        minZ = 0.0;
        maxZ = 0.0;
        fadeStart = 0.0;
        fadeLength = 0.0;
        ambientLight = 0;
        flags = 0;
    }
};

struct baseRender_msg
{
    int frameTime = 0;
    int globTime = 0;
    int adeCount = 0;
    float minZ = 0.;
    float maxZ = 0.;
    uint32_t flags = 0;
};


struct base_64arg
{
    int TimeStamp = 0;
    int DTime = 0;
    TInputState *field_8 = NULL;
    int field_C = 0;
    int field_10 = 0;
};


struct TObjectCache 
{
    std::list<GFX::TMesh> Meshes;
    TF::TForm3D Transform;
    float fadeStart = 0.;
    float fadeLength = 0.;
    
    void Render(baseRender_msg *arg);
};

class NC_STACK_base: public NC_STACK_nucleus
{
public:
    enum USEFLAG
    {
        UF_X = 1,
        UF_Y = 2,
        UF_Z = 4,
        UF_XYZ = (UF_X | UF_Y | UF_Z),
        UF_XY  = (UF_X | UF_Y),
        UF_XZ  = (UF_X | UF_Z),
        UF_YZ  = (UF_Y | UF_Z)
    };
    
    class Instance
    {
    public:
        Instance(NC_STACK_base *bas) : Bas(bas) {};
        virtual ~Instance();        
        
    public:
        NC_STACK_base *Bas = NULL;
        std::vector<Instance *> KidsOpts;
        std::vector<NC_STACK_ade::InstanceOpts *> Particles;
    };
    
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **file);
    
    virtual void AddKid(NC_STACK_base *kid);
    virtual void ChangeParentTo(NC_STACK_base *parent, TF::TForm3D *tform);
    
    virtual void SetPosition(const vec3d &v, int flag = UF_XYZ);
    virtual void ChangePosition(const vec3d &v, int flag = UF_XYZ);
    virtual void SetEulerRotation(int32_t x, int32_t y, int32_t z, int flag = UF_XYZ);
    virtual void ChangeEulerRotation(int32_t x, int32_t y, int32_t z, int flag = UF_XYZ);
    virtual void SetScale(const vec3d &v, int flag = UF_XYZ);
    virtual void ChangeScale(const vec3d &v, int flag = UF_XYZ);
    
    virtual size_t Render(baseRender_msg *arg, Instance * inst = NULL, bool doCopy = false);
    virtual size_t RenderImmediately(baseRender_msg *arg, Instance * inst = NULL);
    
    virtual void RecalcInternal(bool kids = false);
    
    TObjectCache *MakeCache(); 
    virtual void MakeCache(TObjectCache *);

    virtual ~NC_STACK_base();
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };

    enum //Flags
    {
        FLAG_MOVING = 1,        // Not used, only for save
        FLAG_ROTATING = 2,      // Not used, only for save
        FLAG_MAINOBJT = 4,      // Not used, only for save
        FLAG_RENDERALL = 8,     // Not used, only for save
        FLAG_TERMCOLL = 0x10,   // Not used, only for save
        FLAG_INPUTHANDLE = 0x20,// Not used, only for save
        FLAG_FOLLOWPARENT = 0x40
    };

    virtual void SetSkeleton(NC_STACK_skeleton *);
    virtual void SetAde(NC_STACK_ade *);
    virtual void SetParentFollow(bool follow);
    virtual void SetVizLimit(int32_t);
    virtual void SetAmbientLight(int32_t);
    virtual void SetFadeLength(int32_t);
    virtual void SetStatic(bool);

    virtual NC_STACK_skeleton *GetSkeleton();
    virtual bool IsParentFollow();
    virtual int32_t GetVisLimit();
    virtual int32_t GetAmbientLight();
    virtual AdeList *GetAdeList();
    
    TF::TForm3D &TForm();
    virtual BaseList &GetKidList();

    virtual area_arg_65 *GetRenderParams();

    virtual int32_t GetFadeLength();
    virtual bool IsStatic();
    
    vec3d GetPos();
    vec3d GetScale();
    vec3d GetEulerAngles();
    vec3i GetIntEulerAngles();
    
    virtual Instance *GenRenderInstance();
    static void CheckOpts(Instance **vpOpts, NC_STACK_base *bas);
    
    static GFX::TMesh *FindMeshByRenderParams(std::list<GFX::TMesh> *list, const GFX::TRenderParams &p);
    
    void ComputeStaticFog();
    
    static void GenerateMeshCoordsCache(GFX::TMesh *mesh);
    void MakeCoordsCache();
    
    void FreeVBO();
    void MakeVBO();

protected:
    int ReadIFFTagSTRC(IFFile *mfile);
    int ReadIFFTagADES(IFFile *mfile);
    int ReadIFFTagKIDS(IFFile *mfile);
    
    

public:
    static NC_STACK_base *LoadBaseFromFile(const std::string &fname);


public:
    //Data
    static constexpr const char * __ClassName = "base.class";

public:
    
    std::list<GFX::TMesh> Meshes;

    NC_STACK_skeleton *_skeleton = NULL;
    AdeList _ADES;
    TF::TForm3D _transform;
    NC_STACK_base *_parent = NULL;
    BaseList _KIDS;
//    base_node kid_node;
    int _visLimit = 0;
    area_arg_65 _renderMsg;
    NC_STACK_nucleus *_embed = NULL;
    
    // Not used. Save it.
    vec3d ___svdMove;
    int16_t ___svdRx = 0;
    int16_t ___svdRy = 0;
    int16_t ___svdRz = 0;
    int16_t ___svdFlags = 0;
};

#endif // BASE_H_INCLUDED
