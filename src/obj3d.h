#ifndef OBJ3D_H_INCLUDED
#define OBJ3D_H_INCLUDED

#include <list>
#include <string>
#include "system/fsmgr.h"
#include "nucleas.h"
#include "base.h"
#include "bitmap.h"


class NC_STACK_Obj3D: public NC_STACK_base
{        
protected:
    struct FaceVT
    {
        int32_t V = 0;
        int32_t T = 0;
        int32_t N = 0;
        
        FaceVT() = default;
        FaceVT(int32_t v, int32_t t, int32_t n)
        : V(v), T(t), N(n) {};
    };
    
    struct Mtl
    {
        std::string name;
        float diffuse[3];
        NC_STACK_bitmap *diffuseMap = NULL;
        float d = 1.0;
        
        ~Mtl()
        {
            if (diffuseMap)
                diffuseMap->Delete();
        }
        
        GFX::TRenderParams GenParams() const;
    };
        
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();

    NC_STACK_Obj3D();
    virtual ~NC_STACK_Obj3D();

    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_Obj3D();
    };
    
    virtual void RecalcInternal(bool kids = false) override {};

    bool LoadFromFile(const std::string &filename);
    bool LoadFromFile(FSMgr::FileHandle *file);
    
    static NC_STACK_Obj3D *LoadObj3D(const std::string &filename);
    
    static bool ExportObj(NC_STACK_base *base, const std::string &fname);

private:
    bool ParseObj(FSMgr::FileHandle *fil);
    FaceVT ParseFaceVT(const std::string &l);
    bool ParseMtl(const std::string &filename);
    Mtl *FindMtl(const std::string &filename);

public:
    //Data
    static const Nucleus::ClassDescr description;

private:
    std::vector<vec3d> _v;
    std::vector<vec3d> _vt;
    std::vector<vec3d> _vn;
    std::vector< std::array<FaceVT, GFX_MAX_VERTEX> > _f;
    std::list<Mtl> _mtls;
    
    std::string fname;
    
    Mtl *_currentMtl = NULL;
    
    int32_t faceNum = 0;
};

#endif // OBJ3D_H_INCLUDED
