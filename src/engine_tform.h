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

struct SinCos
{
    float sin;
    float cos;
};

struct destFX
{
    char type_flag;
    int p1; // Model id. >= 0
    float p2;
    float p3;
    float p4;

    destFX()
    {
        clear();
    }

    void clear()
    {
        type_flag = 0;
        p1 = 0;
        p2 = 0.0;
        p3 = 0.0;
        p4 = 0.0;
    }
};

class TFEngine
{
public:
    int init();
    void deinit();

    static SinCos *SinCos_table;
};

extern TFEngine TFe;


inline bool Vec3dReadIFF(vec3d &v, IFFile *iff, bool BigEndian)
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

inline bool Vec3dWriteIFF(const vec3d &v, IFFile *iff, bool BigEndian)
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

inline bool Mat3x3ReadIFF(mat3x3 &m, IFFile *iff, bool BigEndian)
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

inline bool Mat3x3WriteIFF(const mat3x3 &m, IFFile *iff, bool BigEndian)
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

#endif // ENGINE_TFORM_H_INCLUDED
