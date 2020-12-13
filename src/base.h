#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <deque>
#include <list>
#include "nucleas.h"
#include "engine_tform.h"
#include "engine_input.h"
#include "skeleton.h"
#include "ade.h"


class NC_STACK_base;
class NC_STACK_skeleton;
class NC_STACK_ade;

typedef std::list<NC_STACK_base *> BaseList;



struct polysDat
{
    void (*render_func)(void *);
    int32_t range;
    polysDatSub datSub;
};

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
    TFEngine::TForm3D *view;
    TFEngine::TForm3D *owner;
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

struct STRC_base
{
    int16_t version;
    vec3d pos;
    vec3d vec;
    vec3d scale;
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t rx;
    int16_t ry;
    int16_t rz;
    int16_t attFlags;
    int16_t _un1;
    int32_t visLimit;
    int32_t ambientLight;
};

struct flag_xyz
{
    int flag;
    vec3d v;
};

struct flag_xyz2
{
    int flag;
    int x;
    int y;
    int z;
};

struct baseRender_msg
{
    int frameTime;
    int globTime;
    RenderStack *rndrStack;
    int adeCount;
    int ownerID;
    float minZ;
    float maxZ;
    uint32_t flags;

    baseRender_msg()
    {
    	clear();
    }

    void clear()
    {
    	frameTime = 0;
		globTime = 0;
		rndrStack = NULL;
		adeCount = 0;
		ownerID = 0;
		minZ = 0.;
		maxZ = 0.;
		flags = 0;
    }
};


struct base_64arg
{
    int TimeStamp;
    int DTime;
    InputState *field_8;
    int field_C;
    int field_10;
};

struct vhclBases
{
    NC_STACK_base *base;
    TFEngine::TForm3D *trigo;

    vhclBases()
    {
        base = NULL;
        trigo = NULL;
    }
};


class NC_STACK_base: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual size_t base_func64(base_64arg *arg);
    virtual size_t base_func65(NC_STACK_base *kid);
    virtual size_t base_func66(NC_STACK_base *parent, TFEngine::TForm3D *tform);
    virtual size_t base_func67(NC_STACK_base **arg);
    virtual size_t base_func68(flag_xyz *arg);
    virtual size_t base_func69(flag_xyz *arg);
    virtual size_t base_func70(flag_xyz2 *arg);
    virtual size_t base_func71(flag_xyz2 *arg);
    virtual size_t base_func72(flag_xyz *arg);
    virtual size_t base_func73(base_64arg *arg);
    virtual size_t base_func77(baseRender_msg *arg);
    virtual size_t base_func78(base_64arg *arg);
    virtual size_t base_func79(NC_STACK_base **arg);

    NC_STACK_base() {
        ID = 0;
        flags = 0;
        timeStamp = 0;
        OBJ_SKELETON = NULL;
        parent = NULL;
//        mainChild = NULL;
        mainObject = NULL;
        visLimit = 0;
        OBJT = NULL;
    };
    virtual ~NC_STACK_base() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_base();
    };

    enum //Flags
    {
        FLAG_MOVING = 1,
        FLAG_ROTATING = 2,
        FLAG_MAINKID = 4,
        FLAG_RENDERALL = 8,
        FLAG_TERMCOLL = 0x10,
        FLAG_INPUTHANDLE = 0x20,
        FLAG_MAINOBJT = 0x40,
        FLAG_EMBDRSRC = 0x80
    };

    enum //Attributes
    {
        ATT_SKELET = 0x80001000,
        ATT_ADE = 0x80001001,
        ATT_PARENTFOLLOW = 0x80001002,
        ATT_VISLIMIT = 0x80001004,
        ATT_AMBIENTLIGHT = 0x80001005,
        ATT_RENDERALL = 0x80001006,
        ATT_INPUTHANDLE = 0x80001008,
        ATT_X = 0x80001009,
        ATT_Y = 0x8000100A,
        ATT_Z = 0x8000100B,
        ATT_VX = 0x8000100C,
        ATT_VY = 0x8000100D,
        ATT_VZ = 0x8000100E,
        ATT_AX = 0x8000100F,
        ATT_AY = 0x80001010,
        ATT_AZ = 0x80001011,
        ATT_RX = 0x80001012,
        ATT_RY = 0x80001013,
        ATT_RZ = 0x80001014,
        ATT_SX = 0x80001015,
        ATT_SY = 0x80001016,
        ATT_SZ = 0x80001017,
        ATT_ADELIST = 0x80001018,
        ATT_PTRANSFORM = 0x80001019,
        ATT_KIDSLIST = 0x8000101A,
        ATT_KIDNODE = 0x8000101B,
        ATT_RENDERPARAMS = 0x8000101D,
        ATT_MAINKID = 0x8000101E,
        ATT_MAINOBJT = 0x8000101F,
        ATT_RENDERSTACK = 0x80001020,
        ATT_ARGSTACK = 0x80001021,
        ATT_ENDARGSTACK = 0x80001022,
        ATT_FADELEN = 0x80001023,
        ATT_STATIC = 0x80001024,
        ATT_EMBDRSRC = 0x80001025
    };

    virtual void setBASE_skeleton(NC_STACK_skeleton *);
    virtual void setBASE_ADE(NC_STACK_ade *);
    virtual void setBASE_parentFollow(int);
    virtual void setBASE_visLimit(int);
    virtual void setBASE_ambientLight(int);
    virtual void setBASE_renderAll(int);
    virtual void setBASE_inputHandle(int);
    virtual void setBASE_fadeLength(int);
    virtual void setBASE_static(int);
    virtual void setBASE_embdRsrc(int);

    virtual NC_STACK_skeleton *getBASE_skeleton();
    virtual int getBASE_parentFollow();
    virtual int getBASE_visLimit();
    virtual int getBASE_ambientLight();
    virtual int getBASE_renderAll();
    virtual int getBASE_inputHandle();
    virtual float getBASE_x();
    virtual float getBASE_y();
    virtual float getBASE_z();
    virtual float getBASE_vx();
    virtual float getBASE_vy();
    virtual float getBASE_vz();
    virtual int getBASE_ax();
    virtual int getBASE_ay();
    virtual int getBASE_az();
    virtual int getBASE_rx();
    virtual int getBASE_ry();
    virtual int getBASE_rz();
    virtual float getBASE_sx();
    virtual float getBASE_sy();
    virtual float getBASE_sz();
    virtual AdeList *getBASE_adeList();
    virtual TFEngine::TForm3D *getBASE_pTransform();
    virtual BaseList &getBASE_kidList();
//    virtual base_node *getBASE_kidNode();
    virtual area_arg_65 *getBASE_renderParams();
    virtual int getBASE_mainKid();
    virtual int getBASE_mainObjt();
//    virtual polys *getBASE_renderStack();
//    virtual void *getBASE_argStack();
//    virtual void *getBASE_endArgStack();
    virtual int getBASE_fadeLength();
    virtual int getBASE_static();
    virtual int getBASE_embdRsrc();

    RenderStack *getBASE_newRenderStack();

    void base_setter(IDVPair *stak);
    void base_getter(IDVPair *stak);

    vec3d getBASE_pos();
    vec3d getBASE_vec();

protected:
    int READ_STRC(IFFile *mfile);
    int READ_ADES(IFFile *mfile);
    int READ_KIDS(IFFile *mfile);

public:
    static NC_STACK_base *READ_BAS_FILE(const std::string &fname);


public:
    //Data
    static const Nucleus::ClassDescr description;

    static RenderStack renderStack;

public:

    int ID;
    uint32_t flags;
    int timeStamp;
    NC_STACK_skeleton *OBJ_SKELETON;
    AdeList ADES;
    TFEngine::TForm3D transform;
    NC_STACK_base *parent;
//    NC_STACK_base *mainChild;
    NC_STACK_base *mainObject;
    BaseList KIDS;
    BaseList *parentList;
//    base_node kid_node;
    int visLimit;
    area_arg_65 renderMsg;
    NC_STACK_nucleus *OBJT;
};

#endif // BASE_H_INCLUDED
