#ifndef SKELETON_H_INCLUDED
#define SKELETON_H_INCLUDED

#include "rsrc.h"
#include "system/gfx.h"

namespace UAskeleton
{

struct Vertex: public vec3d
{
    uint8_t flags = 0;

    enum CLIP
    {
        CLIP_NONE = 0,
        CLIP_LEFT = (1 << 0),
        CLIP_RIGHT = (1 << 1),
        CLIP_BOTTOM = (1 << 2),
        CLIP_TOP = (1 << 3),
        CLIP_NEAR = (1 << 4),
        CLIP_FAR = (1 << 5),
        CLIP_ANY = (CLIP_LEFT | CLIP_RIGHT | CLIP_BOTTOM | CLIP_TOP | CLIP_NEAR | CLIP_FAR)
    };
    
    Vertex() {};
    Vertex(const vec3d& v) : vec3d(v) {};
    Vertex(const vec3d& v, uint8_t f) : vec3d(v), flags(f) {};

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
    int16_t num_vertices = 0;
    std::array<int16_t, GFX_MAX_VERTEX> v {{0}};

    // Geometry plane Ax + Bx + Cz + D = 0
    // A,B,C plane vector normal n
    float A = 0.0;
    float B = 0.0;
    float C = 0.0;
    float D = 0.0;

    const vec3d Normal() const
    {
        return vec3d(A, B, C);
    }
};

struct Data
{
    std::vector<Vertex> POO;
    std::vector<Vertex> SEN;
    std::vector<Polygon> polygons;
    std::vector<Vertex> tformedVertex;
};

}



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
    virtual size_t Init(IDVList &stak);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func65(rsrc *pres);
    virtual size_t skeleton_func131(int *arg);
    virtual size_t skeleton_func132(skeleton_arg_132 *arg);
    virtual bool skeleton_func133(skeleton_arg133 *arg);

    NC_STACK_skeleton() {};
    virtual ~NC_STACK_skeleton() {};
    
    virtual const std::string &ClassName() const {
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
    bool PolygonCheckInvisible(const std::vector<UAskeleton::Vertex> &in, const UAskeleton::Polygon &pol, std::vector<vec3d> *out);
    bool TransformVertexes(skeleton_arg_132 *arg, const std::vector<UAskeleton::Vertex> &in, std::vector<UAskeleton::Vertex> *out);


public:
    //Data
    static const Nucleus::ClassDescr description;

    UAskeleton::Data *_resData = NULL;
};

#endif // SKELETON_H_INCLUDED
