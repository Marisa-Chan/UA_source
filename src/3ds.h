#ifndef DS3_H_INCLUDED
#define DS3_H_INCLUDED

#include <list>
#include <string>
#include "system/fsmgr.h"
#include "nucleas.h"
#include "base.h"
#include "bitmap.h"

struct d3dsTextureMap
{
    std::string name;
    unsigned    flags = 0;
    float       percent = 0.0;
    float       blur = 0.0;
    float       scale[2] = {0.0};
    float       offset[2] = {0.0};
    float       rotation = 0.0;
    float       tint_1[3] = {0.0};
    float       tint_2[3] = {0.0};
    float       tint_r[3] = {0.0};
    float       tint_g[3] = {0.0};
    float       tint_b[3] = {0.0};

    NC_STACK_bitmap *tex = NULL;

    ~d3dsTextureMap()
    {
        if (tex)
            Nucleus::Delete(tex);
    }
};

struct d3dsMaterial
{
    std::string         name;               /* Material name */
    float               ambient[3] = {0.0};         /* Material ambient reflectivity */
    float               diffuse[3] = {0.0};         /* Material diffuse reflectivity */
    float               specular[3] = {0.0};        /* Material specular reflectivity */
    float               shininess = 0.0;          /* Material specular exponent */
    float               shin_strength = 0.0;
    int                 use_blur = 0;
    float               blur = 0.0;
    float               transparency = 0.0;
    float               falloff = 0.0;
    int                 is_additive = 0;
    int                 self_illum_flag = 0; /* bool */
    float               self_illum = 0.0;
    int                 use_falloff = 0;
    int                 shading = 0;
    int                 soften = 0;         /* bool */
    int                 face_map = 0;       /* bool */
    int                 two_sided = 0;      /* Material visible from back */
    int                 map_decal = 0;      /* bool */
    int                 use_wire = 0;
    int                 use_wire_abs = 0;
    float               wire_size  = 0.0;
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
    unsigned            autorefl_map_flags = 0;
    int                 autorefl_map_anti_alias = 0;  /* 0=None, 1=Low, 2=Medium, 3=High */
    int                 autorefl_map_size = 0;
    int                 autorefl_map_frame_step = 0;
};

class NC_STACK_3ds: public NC_STACK_base
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();

    NC_STACK_3ds();
    virtual ~NC_STACK_3ds();

    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_3ds();
    };

    bool LoadFromFile(const std::string &filename);
    bool LoadFromFile(FSMgr::FileHandle *file);
    
    static NC_STACK_3ds *Load3DS(const std::string &filename);
    
    virtual void RecalcInternal(bool kids = false) override;

private:
    size_t readChunkEditor(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkObject(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkTrimesh(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkVertex(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkFaces(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkMaterial(FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkTexMap(d3dsTextureMap &texmap, FSMgr::FileHandle *fil, size_t sz);
    size_t readChunkColor(float colors[3], FSMgr::FileHandle *fil, size_t sz);

    size_t readName(FSMgr::FileHandle *fil, std::string *dst, size_t maxn);

    d3dsMaterial *findMaterial(const std::string &matName);
    GFX::TRenderParams GenRenderParams(d3dsMaterial *);

public:
    //Data
    static const Nucleus::ClassDescr description;

private:
    std::vector<tUtV> texCoords;
    std::vector<d3dsMaterial *>faceMaterial;
    std::list<d3dsMaterial > materials;

    int32_t faceNum = 0;
};

#endif // BASE_H_INCLUDED
