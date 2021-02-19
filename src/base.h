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





class RenderStack
{
public:
    RenderStack();
    ~RenderStack();

    typedef  bool (*tCompare) (polysDat *, polysDat *);

    polysDat *get();
    void commit();
    size_t getSize();
    void clear(bool dealloc = false);
    void render(bool sorting = true, tCompare _func = NULL, bool Clear = true);

    static bool comparePrio(polysDat *a, polysDat *b);

private:
    static bool compare(polysDat *a, polysDat *b);

private:
    static const size_t heapSize = 10000;
    size_t currentElement;
    std::deque<polysDat *> heaps;

    std::deque<polysDat *> que;
};

struct area_arg_65
{
    int ownerID;
    //polys *rndrSTK_cur;
    //polysDat *argSTK_cur;
    RenderStack *rndrStack;
    int timeStamp;
    int frameTime;
    TF::TForm3D *view;
    TF::TForm3D *owner;
    NC_STACK_skeleton *OBJ_SKELETON;
    UAskeleton::Data *sklt;
    float minZ;
    float maxZ;
    float fadeStart;
    float fadeLength;
    int ambientLight;
    int adeCount;
    uint32_t flags;

    area_arg_65()
    {
        ownerID = 0;
        rndrStack = NULL;
        timeStamp = 0;
        frameTime = 0;
        view = NULL;
        owner = NULL;
        OBJ_SKELETON = NULL;
        sklt = NULL;
        minZ = 0.0;
        maxZ = 0.0;
        fadeStart = 0.0;
        fadeLength = 0.0;
        ambientLight = 0;
        adeCount = 0;
        flags = 0;
    }
};

struct baseRender_msg
{
    int frameTime = 0;
    int globTime = 0;
    RenderStack *rndrStack = NULL;
    int adeCount = 0;
    int ownerID = 0;
    float minZ = 0.;
    float maxZ = 0.;
    uint32_t flags = 0;
};


struct base_64arg
{
    int TimeStamp;
    int DTime;
    InputState *field_8;
    int field_C;
    int field_10;
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
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    
    virtual size_t InitFromIFF(IFFile **file);
    virtual size_t DeinitFromIFF(IFFile **file);
    
    virtual size_t AddKid(NC_STACK_base *kid);
    virtual size_t ChangeParentTo(NC_STACK_base *parent, TF::TForm3D *tform);
    
    virtual void SetPosition(const vec3d &v, int flag = UF_XYZ);
    virtual void ChangePosition(const vec3d &v, int flag = UF_XYZ);
    virtual void SetEulerRotation(int32_t x, int32_t y, int32_t z, int flag = UF_XYZ);
    virtual void ChangeEulerRotation(int32_t x, int32_t y, int32_t z, int flag = UF_XYZ);
    virtual void SetScale(const vec3d &v, int flag = UF_XYZ);
    virtual void ChangeScale(const vec3d &v, int flag = UF_XYZ);
    
    virtual size_t Render(baseRender_msg *arg);

    virtual ~NC_STACK_base() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * NewInstance() {
        return new NC_STACK_base();
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

    RenderStack *GetRenderStack();
    
    vec3d GetPos();
    vec3d GetScale();
    vec3d GetEulerAngles();
    vec3i GetIntEulerAngles();

protected:
    int ReadIFFTagSTRC(IFFile *mfile);
    int ReadIFFTagADES(IFFile *mfile);
    int ReadIFFTagKIDS(IFFile *mfile);

public:
    static NC_STACK_base *LoadBaseFromFile(const std::string &fname);


public:
    //Data
    static const Nucleus::ClassDescr description;

    static RenderStack renderStack;

public:

    int _ID = 0;
    int _timeStamp = 0;
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
