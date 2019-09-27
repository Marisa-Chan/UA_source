#ifndef ENGINE_TFORM_H_INCLUDED
#define ENGINE_TFORM_H_INCLUDED

#include <math.h>
#include "vectors.h"
#include "matrix.h"
#include "IFFile.h"

typedef Tvec2d<double> vec2d;
typedef Tvec3d<double> vec3d;
typedef Tmat3x3<double, double> mat3x3;
typedef Tmat4x4<double, double> mat4x4;

class TFEngine
{
public:
    struct SinCos
    {
        float sin;
        float cos;
    };

    class TForm3D
    {
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
        TForm3D()
        {
            parent_1c = NULL;
            ax = 0;
            ay = 0;
            az = 0;
            rx = 0;
            ry = 0;
            rz = 0;
            flags = 0;
        };

        TForm3D *parent_1c;
        vec3d locPos;
        vec3d globPos;
        vec3d vec;
        vec3d scale;
        mat3x3 locSclRot;
        mat3x3 globSclRot;
        int32_t ax;
        int32_t ay;
        int32_t az;
        int32_t rx;
        int32_t ry;
        int32_t rz;
        uint32_t flags;
        mat4x4 tform;

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

public:
    int init();
    void deinit();

    inline SinCos& Angle(int angle) {return SinCos_table[angle];};
    inline TForm3D *GetViewPoint() {return viewPoint;};
    inline void SetViewPoint(TForm3D *pnt) {viewPoint = pnt;};

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
    static TFEngine Engine;

protected:
    SinCos *SinCos_table;
    TForm3D *viewPoint;
};



#endif // ENGINE_TFORM_H_INCLUDED
