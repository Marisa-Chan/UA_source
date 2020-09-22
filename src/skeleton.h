#ifndef SKELETON_H_INCLUDED
#define SKELETON_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

namespace UAskeleton
{

struct Vertex: public vec3d
{
    int flags;

    enum CLIP
    {
        CLIP_NONE = 0,
        CLIP_LEFT = 1,
        CLIP_RIGHT = 2,
        CLIP_BOTTOM = 4,
        CLIP_TOP = 8,
        CLIP_NEAR = 0x10,
        CLIP_FAR = 0x20,
        CLIP_ANY = (CLIP_LEFT | CLIP_RIGHT | CLIP_BOTTOM | CLIP_TOP | CLIP_NEAR | CLIP_FAR)
    };

    Vertex &operator=(vec3d b)
    {
        x = b.x;
        y = b.y;
        z = b.z;

        return *this;
    }
};

struct Polygon
{
    int16_t num_vertices;
    int16_t v[GFX_MAX_VERTEX];

    // Geometry plane Ax + Bx + Cz + D = 0
    // A,B,C plane vector normal n
    float A;
    float B;
    float C;
    float D;

    const vec3d Normal() const
    {
        return vec3d(A, B, C);
    }
};

struct Data
{
    Vertex *POO;
    int16_t POO_NUM;

    Vertex *SEN;
    int16_t SEN_NUM;

    Polygon *polygons;
    int polygonsCount;

    Vertex *tformedVertex;


    Data()
    {
        POO = NULL;
        SEN = NULL;
        polygons = NULL;
        tformedVertex = NULL;
        POO_NUM = 0;
        SEN_NUM = 0;
        polygonsCount = 0;
    };

    ~Data()
    {
        if ( POO )
            delete[] POO;

        if ( polygons )
            delete[] polygons;

        if ( SEN )
            delete[] SEN;

        if ( tformedVertex )
            delete[] tformedVertex;
    };
};

}

struct __NC_STACK_skeleton
{
    UAskeleton::Data *data;
};

struct skeleton_129_arg
{
    UAskeleton::Data *skeleton;
    int sen_count;
};

struct skeleton_130_arg
{
    UAskeleton::Data *skeleton;
    int pol_count;
    int num_indexes;
};

struct skeleton_arg_132
{
    mat4x4 tform;
    float minZ;
    float maxZ;
};

struct skeleton_arg133
{
    int polyID;
    int field_4;
    polysDatSub *rndrArg;
    tUtV *texCoords;
    float minZ;
    float maxZ;
    float shadeVal;
    float fadeStart;
    float fadeLength;
};

class NC_STACK_skeleton: public NC_STACK_rsrc
{
public:
    virtual size_t func0(IDVList &stak);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual __NC_STACK_skeleton * skeleton_func128(IDVPair *);
    virtual size_t skeleton_func129(skeleton_129_arg *arg);
    virtual size_t skeleton_func130(skeleton_130_arg *arg);
    virtual size_t skeleton_func131(int *arg);
    virtual size_t skeleton_func132(skeleton_arg_132 *arg);
    virtual bool skeleton_func133(skeleton_arg133 *arg);

    NC_STACK_skeleton() {
        memset(&stack__skeleton, 0, sizeof(stack__skeleton));
    };
    virtual ~NC_STACK_skeleton() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_skeleton();
    };

    enum SKEL_ATT
    {
        SKEL_ATT_PSKELET = 0x80002000,
        SKEL_ATT_POINTSCNT = 0x80002001,
        SKEL_ATT_SENCNT = 0x80002002,
        SKEL_ATT_POLYCNT = 0x80002003,
        SKEL_ATT_POLYPNTCNT = 0x80002004
    };

    virtual UAskeleton::Data *GetSkelet();
    virtual int getSKEL_pntCount();
    virtual int getSKEL_senCount();
    virtual int getSKEL_polyCount();

protected:
    bool PolygonCheckInvisible(UAskeleton::Vertex *in, vec3d *out, UAskeleton::Polygon *pol);
    bool TransformVertexes(skeleton_arg_132 *arg, UAskeleton::Vertex *in, UAskeleton::Vertex *out, int num);


public:
    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_skeleton stack__skeleton;
};

#endif // SKELETON_H_INCLUDED
