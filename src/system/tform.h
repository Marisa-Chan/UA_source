#ifndef ENGINE_TFORM_H_INCLUDED
#define ENGINE_TFORM_H_INCLUDED

#include <inttypes.h>
#include "../vectors.h"
#include "../matrix.h"
#include "../ini.h"
#include "../IFFile.h"


namespace TF
{
    
    
    
class TForm3D
{
public:
    struct IntAngle
    {
        int32_t RawAngle = 0;

        IntAngle &operator=(int32_t angle);
        IntAngle &operator+=(int32_t angle);
        operator int32_t();
        explicit operator double();
        
        int32_t ClampRawAngle(int32_t angle);
    };

public:
    enum
    {
        FLAG_FOLLOW_PARENT = 1, // Follow to parent transform
        FLAG_NO_WRLD_ROT   = 2  // Do not do rotation on world transformation stage, only translate. (Save performance)
    };
public:
    void MakeScaleRotationMatrix();
    void CalcGlobal();

public:
    TForm3D *Parent = NULL;
    vec3d Pos;
    vec3d CalcPos;
    vec3d Scale;
    mat3x3 SclRot;
    mat3x3 CalcSclRot;
    IntAngle ax;
    IntAngle ay;
    IntAngle az;
    uint32_t flags = 0;
    mat4x4 TForm;

protected:
    void scale_rot_0();
    void scale_rot_1();
    void scale_rot_2();
    void scale_rot_3();
    void scale_rot_4();
    void scale_rot_5();
    void scale_rot_6();
    void scale_rot_7();
};
    
    
    
class TFEngine
{
public:
    struct SinCos
    {
        double sin;
        double cos;
    };

public:
    int Init();
    void Deinit();

    inline SinCos& Angle(int angle) {
        return SinCos_table[angle];
    };
    
    inline TForm3D *GetViewPoint() {
        return viewPoint;
    };
    inline void SetViewPoint(TForm3D *pnt) {
        viewPoint = pnt;
    };

    inline static bool Vec3dReadIFF(vec3d &v, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(v.x);
            res &= iff->readFloatB(v.y);
            res &= iff->readFloatB(v.z);
        }
        else
        {
            res &= iff->readFloatL(v.x);
            res &= iff->readFloatL(v.y);
            res &= iff->readFloatL(v.z);
        }
        return res;
    }

    inline static bool Vec3dWriteIFF(const vec3d &v, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(v.x);
            res &= iff->writeFloatB(v.y);
            res &= iff->writeFloatB(v.z);
        }
        else
        {
            res &= iff->writeFloatL(v.x);
            res &= iff->writeFloatL(v.y);
            res &= iff->writeFloatL(v.z);
        }
        return res;
    }

    inline static bool Mat3x3ReadIFF(mat3x3 &m, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(m.m00);
            res &= iff->readFloatB(m.m01);
            res &= iff->readFloatB(m.m02);
            res &= iff->readFloatB(m.m10);
            res &= iff->readFloatB(m.m11);
            res &= iff->readFloatB(m.m12);
            res &= iff->readFloatB(m.m20);
            res &= iff->readFloatB(m.m21);
            res &= iff->readFloatB(m.m22);
        }
        else
        {
            res &= iff->readFloatL(m.m00);
            res &= iff->readFloatL(m.m01);
            res &= iff->readFloatL(m.m02);
            res &= iff->readFloatL(m.m10);
            res &= iff->readFloatL(m.m11);
            res &= iff->readFloatL(m.m12);
            res &= iff->readFloatL(m.m20);
            res &= iff->readFloatL(m.m21);
            res &= iff->readFloatL(m.m22);
        }
        return res;
    }

    inline static bool Mat3x3WriteIFF(const mat3x3 &m, IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(m.m00);
            res &= iff->writeFloatB(m.m01);
            res &= iff->writeFloatB(m.m02);
            res &= iff->writeFloatB(m.m10);
            res &= iff->writeFloatB(m.m11);
            res &= iff->writeFloatB(m.m12);
            res &= iff->writeFloatB(m.m20);
            res &= iff->writeFloatB(m.m21);
            res &= iff->writeFloatB(m.m22);
        }
        else
        {
            res &= iff->writeFloatL(m.m00);
            res &= iff->writeFloatL(m.m01);
            res &= iff->writeFloatL(m.m02);
            res &= iff->writeFloatL(m.m10);
            res &= iff->writeFloatL(m.m11);
            res &= iff->writeFloatL(m.m12);
            res &= iff->writeFloatL(m.m20);
            res &= iff->writeFloatL(m.m21);
            res &= iff->writeFloatL(m.m22);
        }
        return res;
    }

public:
    static TFEngine Instance;

protected:
    SinCos *SinCos_table = NULL;
    TForm3D *viewPoint = NULL;
};


static constexpr TFEngine &Engine = TFEngine::Instance;


}

#endif // ENGINE_TFORM_H_INCLUDED
