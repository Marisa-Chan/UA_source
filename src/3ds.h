#ifndef DS3_H_INCLUDED
#define DS3_H_INCLUDED

#include <list>
#include <string>
#include "fsmgr.h"
#include "nucleas.h"
#include "base.h"
#include "bitmap.h"

struct d3dsTextureMap
{
    std::string name;
    unsigned    flags;
    float       percent;
    float       blur;
    float       scale[2];
    float       offset[2];
    float       rotation;
    float       tint_1[3];
    float       tint_2[3];
    float       tint_r[3];
    float       tint_g[3];
    float       tint_b[3];

    NC_STACK_bitmap *tex;

    d3dsTextureMap()
    {
        tex = NULL;
    }

    ~d3dsTextureMap()
    {
        if (tex)
            delete_class_obj(tex);
    }
};

struct d3dsMaterial
{
    std::string         name;               /* Material name */
    float               ambient[3];         /* Material ambient reflectivity */
    float               diffuse[3];         /* Material diffuse reflectivity */
    float               specular[3];        /* Material specular reflectivity */
    float               shininess;          /* Material specular exponent */
    float               shin_strength;
    int                 use_blur;
    float               blur;
    float               transparency;
    float               falloff;
    int                 is_additive;
    int                 self_illum_flag; /* bool */
    float               self_illum;
    int                 use_falloff;
    int                 shading;
    int                 soften;         /* bool */
    int                 face_map;       /* bool */
    int                 two_sided;      /* Material visible from back */
    int                 map_decal;      /* bool */
    int                 use_wire;
    int                 use_wire_abs;
    float               wire_size;
    d3dsTextureMap      texture1_map;
    d3dsTextureMap      texture1_mask;
    d3dsTextureMap      texture2_map;
    d3dsTextureMap      texture2_mask;
    d3dsTextureMap      opacity_map;
    d3dsTextureMap      opacity_mask;
    d3dsTextureMap      bump_map;
    d3dsTextureMap      bump_mask;
    d3dsTextureMap      specular_map;
    d3dsTextureMap      specular_mask;
    d3dsTextureMap      shininess_map;
    d3dsTextureMap      shininess_mask;
    d3dsTextureMap      self_illum_map;
    d3dsTextureMap      self_illum_mask;
    d3dsTextureMap      reflection_map;
    d3dsTextureMap      reflection_mask;
    unsigned            autorefl_map_flags;
    int                 autorefl_map_anti_alias;  /* 0=None, 1=Low, 2=Medium, 3=High */
    int                 autorefl_map_size;
    int                 autorefl_map_frame_step;
};

class NC_STACK_3ds: public NC_STACK_base
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t base_func77(baseRender_msg *arg);

    NC_STACK_3ds();
    virtual ~NC_STACK_3ds();

    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_3ds();
    };

    bool readFile(const char *filename);

private:
    size_t readChunkEditor(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkObject(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkTrimesh(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkVertex(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkFaces(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkMaterial(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkTexMap(d3dsTextureMap &texmap, FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkColor(float colors[3], FSMgr::FileHandle *fil, size_t sz);

    size_t readName(FSMgr::FileHandle *fil, char *dst, size_t maxn);

    d3dsMaterial *findMaterial(const char *matName);

public:
    //Data
    static const Nucleus::ClassDescr description;

private:
    tUtV *texCoords;
    d3dsMaterial **faceMaterial;
    std::list<d3dsMaterial *> materials;

    int32_t faceNum;
};

#endif // BASE_H_INCLUDED
