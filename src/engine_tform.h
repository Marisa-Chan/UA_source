#ifndef ENGINE_TFORM_H_INCLUDED
#define ENGINE_TFORM_H_INCLUDED

#include <math.h>
#include "IFFile.h"

struct SinCos
{
    float sin;
    float cos;
};

#define MAT_FLAG_INV_SIN   1
#define MAT_FLAG_INV_COS   2

struct vec2d
{
    float x;
    float y;

    vec2d()
    {
        x = 0.0;
        y = 0.0;
    }

    vec2d(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    vec2d(float _b)
    {
        x = _b;
        y = _b;
    }

    float length() const
    {
        return sqrt( x * x + y * y);
    }

    float norm() const
    {
        return length();
    }

    float normalise()
    {
        float ln = length();

        if (ln == 0.0)
            return 0.0;

        *this /= ln;

        return ln;
    }

    const vec2d normalise(float *pln) const
    {
        vec2d tmp = *this;

        float ln = tmp.length();

        tmp /= ln;

        if (pln)
            *pln = ln;

        return tmp;
    }

    vec2d &operator=(const vec2d &b)
    {
        x = b.x;
        y = b.y;
        return *this;
    }

    vec2d &operator=(float b)
    {
        x = b;
        y = b;
        return *this;
    }

    vec2d &operator+=(const vec2d &b)
    {
        x += b.x;
        y += b.y;
        return *this;
    }

    vec2d &operator-=(const vec2d &b)
    {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    vec2d &operator+=(float b)
    {
        x += b;
        y += b;
        return *this;
    }

    vec2d &operator-=(float b)
    {
        x -= b;
        y -= b;
        return *this;
    }

    vec2d &operator*=(float b)
    {
        x *= b;
        y *= b;
        return *this;
    }

    vec2d &operator/=(float b)
    {
        if (b != 0.0)
        {
            double tmp = 1.0 / b;
            *this *= tmp;
        }

        return *this;
    }

    const vec2d operator-() const
    {
        return vec2d(-x, -y);
    }

    const vec2d operator+(const vec2d &b) const
    {
        vec2d tmp = *this;
        tmp += b;
        return tmp;
    }

    const vec2d operator-(const vec2d &b) const
    {
        vec2d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const vec2d operator+(float b) const
    {
        vec2d tmp = *this;
        tmp += b;
        return tmp;
    }

    const vec2d operator-(float b) const
    {
        vec2d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const vec2d operator/(float b) const
    {
        vec2d tmp = *this;
        tmp /= b;
        return tmp;
    }

    bool operator==(const vec2d &b) const
    {
        return x == b.x && y == b.y;
    }

    bool operator!=(const vec2d &b) const
    {
        return x != b.x || y != b.y;
    }

    float dot(const vec2d &b) const
    {
        return x * b.x + y * b.y;
    }

    float cross(const vec2d &b) const
    {
        return x * b.y - y * b.x;
    }

    float xyAngle()
    {
        if (x == 0.0 && y == 0.0)
            return 0.0;
        return atan2(y, x);
    }

    float yxAngle()
    {
        if (x == 0.0 && y == 0.0)
            return 0.0;
        return atan2(x, y);
    }
};

struct vec3d
{
    typedef float xyzFLT;
    xyzFLT x;
    xyzFLT y;
    xyzFLT z;

    vec3d()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }

    vec3d(xyzFLT _x, xyzFLT _y, xyzFLT _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    vec3d(xyzFLT _b)
    {
        x = _b;
        y = _b;
        z = _b;
    }

    xyzFLT length() const
    {
        return sqrt( x * x + y * y + z * z );
    }

    xyzFLT normalise()
    {
        xyzFLT ln = length();

        if (ln == 0.0)
            return 0.0;

        *this /= ln;

        return ln;
    }

    const vec3d normalise(float *pln) const
    {
        vec3d tmp = *this;

        xyzFLT ln = tmp.length();

        tmp /= ln;

        if (pln)
            *pln = ln;

        return tmp;
    }

    // xyz = another xyz
    vec3d &operator=(const vec3d &b)
    {
        x = b.x;
        y = b.y;
        z = b.z;

        return *this;
    }

    // xyz = float
    vec3d &operator=(xyzFLT b)
    {
        x = b;
        y = b;
        z = b;

        return *this;
    }

    vec3d &operator+=(const vec3d &b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    vec3d &operator-=(const vec3d &b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }

    vec3d &operator*=(const vec3d &b)
    {
        vec3d tmp = *this;
        x = tmp.y * b.z - tmp.z * b.y;
        y = tmp.z * b.x - tmp.x * b.z;
        z = tmp.x * b.y - tmp.y * b.x;
        return *this;
    }

    vec3d &operator+=(xyzFLT b)
    {
        x += b;
        y += b;
        z += b;
        return *this;
    }

    vec3d &operator-=(xyzFLT b)
    {
        x -= b;
        y -= b;
        z -= b;
        return *this;
    }

    vec3d &operator*=(xyzFLT b)
    {
        x *= b;
        y *= b;
        z *= b;
        return *this;
    }

    vec3d &operator/=(xyzFLT b)
    {
        if (b != 0.0)
        {
            double tmp = 1.0 / b;
            *this *= tmp;
        }

        return *this;
    }

    const vec3d operator-() const
    {
        return vec3d(-x, -y, -z);
    }

    const vec3d operator+(const vec3d &b) const
    {
        vec3d tmp = *this;
        tmp += b;
        return tmp;
    }

    const vec3d operator-(const vec3d &b) const
    {
        vec3d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const vec3d operator*(const vec3d &b) const
    {
        vec3d tmp = *this;
        tmp *= b;
        return tmp;
    }

    const vec3d operator+(xyzFLT b) const
    {
        vec3d tmp = *this;
        tmp += b;
        return tmp;
    }

    const vec3d operator-(xyzFLT b) const
    {
        vec3d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const vec3d operator*(xyzFLT b) const
    {
        vec3d tmp = *this;
        tmp *= b;
        return tmp;
    }

    const vec3d operator/(xyzFLT b) const
    {
        vec3d tmp = *this;
        tmp /= b;
        return tmp;
    }

    bool operator==(const vec3d &b) const
    {
        return x == b.x && y == b.y && z == b.z;
    }

    bool operator!=(const vec3d &b) const
    {
        return x != b.x || y != b.y || z != b.z;
    }

    xyzFLT dot(const vec3d &b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }

    bool readIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(x);
            res &= iff->readFloatB(y);
            res &= iff->readFloatB(z);
        }
        else
        {
            res &= iff->readFloatL(x);
            res &= iff->readFloatL(y);
            res &= iff->readFloatL(z);
        }
        return res;
    }

    bool writeIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(x);
            res &= iff->writeFloatB(y);
            res &= iff->writeFloatB(z);
        }
        else
        {
            res &= iff->writeFloatL(x);
            res &= iff->writeFloatL(y);
            res &= iff->writeFloatL(z);
        }
        return res;
    }
};

struct mat3x3
{
    float m00;
    float m01;
    float m02;
    float m10;
    float m11;
    float m12;
    float m20;
    float m21;
    float m22;

    mat3x3()
    {
        m00 = 0.0;
        m01 = 0.0;
        m02 = 0.0;
        m10 = 0.0;
        m11 = 0.0;
        m12 = 0.0;
        m20 = 0.0;
        m21 = 0.0;
        m22 = 0.0;
    }

    mat3x3(float _m00, float _m01, float _m02,
           float _m10, float _m11, float _m12,
           float _m20, float _m21, float _m22)
    {
        m00 = _m00;
        m01 = _m01;
        m02 = _m02;
        m10 = _m10;
        m11 = _m11;
        m12 = _m12;
        m20 = _m20;
        m21 = _m21;
        m22 = _m22;
    }

    mat3x3 (const vec3d &axis, float angle, int flags = 0)
    {
        float cs = cos(angle);
        float sn = sin(angle);

        if (flags & MAT_FLAG_INV_COS)
            cs = -cs;

        if (flags & MAT_FLAG_INV_SIN)
            sn = -sn;

        float ics = 1.0 - cs;

        m00 = ics * axis.x * axis.x + cs;
        m01 = ics * axis.x * axis.y - sn * axis.z;
        m02 = ics * axis.z * axis.x + sn * axis.y;
        m10 = ics * axis.x * axis.y + sn * axis.z;
        m11 = ics * axis.y * axis.y + cs;
        m12 = ics * axis.y * axis.z - sn * axis.x;
        m20 = ics * axis.z * axis.x - sn * axis.y;
        m21 = ics * axis.y * axis.z + sn * axis.x;
        m22 = ics * axis.z * axis.z + cs;
    }

    const vec3d getVect(int id) const
    {
        switch (id)
        {
        default:
        case 0:
            return vec3d(m00, m01, m02);

        case 1:
            return vec3d(m10, m11, m12);

        case 2:
            return vec3d(m20, m21, m22);

        case 10:
            return vec3d(m00, m10, m20);

        case 11:
            return vec3d(m01, m11, m21);

        case 12:
            return vec3d(m02, m12, m22);
        }
    }

    const vec3d AxisX() const
    {
        return vec3d(m00, m01, m02);
    }

    const vec3d AxisY() const
    {
        return vec3d(m10, m11, m12);
    }

    const vec3d AxisZ() const
    {
        return vec3d(m20, m21, m22);
    }

    void SetX(const vec3d &v)
    {
        m00 = v.x;
        m01 = v.y;
        m02 = v.z;
    }

    void SetY(const vec3d &v)
    {
        m10 = v.x;
        m11 = v.y;
        m12 = v.z;
    }

    void SetZ(const vec3d &v)
    {
        m20 = v.x;
        m21 = v.y;
        m22 = v.z;
    }

    bool readIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(m00);
            res &= iff->readFloatB(m01);
            res &= iff->readFloatB(m02);
            res &= iff->readFloatB(m10);
            res &= iff->readFloatB(m11);
            res &= iff->readFloatB(m12);
            res &= iff->readFloatB(m20);
            res &= iff->readFloatB(m21);
            res &= iff->readFloatB(m22);
        }
        else
        {
            res &= iff->readFloatL(m00);
            res &= iff->readFloatL(m01);
            res &= iff->readFloatL(m02);
            res &= iff->readFloatL(m10);
            res &= iff->readFloatL(m11);
            res &= iff->readFloatL(m12);
            res &= iff->readFloatL(m20);
            res &= iff->readFloatL(m21);
            res &= iff->readFloatL(m22);
        }
        return res;
    }

    bool writeIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(m00);
            res &= iff->writeFloatB(m01);
            res &= iff->writeFloatB(m02);
            res &= iff->writeFloatB(m10);
            res &= iff->writeFloatB(m11);
            res &= iff->writeFloatB(m12);
            res &= iff->writeFloatB(m20);
            res &= iff->writeFloatB(m21);
            res &= iff->writeFloatB(m22);
        }
        else
        {
            res &= iff->writeFloatL(m00);
            res &= iff->writeFloatL(m01);
            res &= iff->writeFloatL(m02);
            res &= iff->writeFloatL(m10);
            res &= iff->writeFloatL(m11);
            res &= iff->writeFloatL(m12);
            res &= iff->writeFloatL(m20);
            res &= iff->writeFloatL(m21);
            res &= iff->writeFloatL(m22);
        }
        return res;
    }

    vec3d Transform(const vec3d &b) const
    {
        vec3d tmp;
        tmp.x = m00 * b.x + m01 * b.y + m02 * b.z;
        tmp.y = m10 * b.x + m11 * b.y + m12 * b.z;
        tmp.z = m20 * b.x + m21 * b.y + m22 * b.z;
        return tmp;
    }

    mat3x3 &operator*=(const mat3x3 &b)
    {
        mat3x3 a = *this;
        m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
        m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
        m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
        m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
        m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
        m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
        m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
        m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
        m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
        return *this;
    }

    const mat3x3 operator*(const mat3x3 &b) const
    {
        mat3x3 tmp = *this;
        tmp *= b;
        return tmp;
    }

    static const mat3x3 RotateX(float angle)
    {
        float sn = sin(angle);
        float cs = cos(angle);

        return mat3x3(1.0, 0.0, 0.0,
                      0.0,  cs,  sn,
                      0.0, -sn,  cs);
    }
};

struct mat4x4
{
    float m00;
    float m01;
    float m02;
    float m03;
    float m10;
    float m11;
    float m12;
    float m13;
    float m20;
    float m21;
    float m22;
    float m23;
    float m30;
    float m31;
    float m32;
    float m33;

    mat4x4()
    {
        identity();
    }

    mat4x4(const vec3d &t)
    {
        identity();
        setTranslate(t);
    }

    mat4x4(const mat3x3 &m)
    {
        *this = m;
    }

    inline void identity()
    {
        m00 = 1.0;
        m01 = 0.0;
        m02 = 0.0;
        m03 = 0.0;
        m10 = 0.0;
        m11 = 1.0;
        m12 = 0.0;
        m13 = 0.0;
        m20 = 0.0;
        m21 = 0.0;
        m22 = 1.0;
        m23 = 0.0;
        m30 = 0.0;
        m31 = 0.0;
        m32 = 0.0;
        m33 = 1.0;
    }

    vec3d Transform(const vec3d &b) const
    {
        vec3d tmp;
        tmp.x = m00 * b.x + m01 * b.y + m02 * b.z + m03;
        tmp.y = m10 * b.x + m11 * b.y + m12 * b.z + m13;
        tmp.z = m20 * b.x + m21 * b.y + m22 * b.z + m23;
        return tmp;
    }

    mat4x4 &operator*=(const mat3x3 &b)
    {
        mat4x4 a = *this;
        m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
        m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
        m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
        //m03 = m03;
        m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
        m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
        m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
        //m13 = m13;
        m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
        m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
        m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
        //m23 = m23;
        //m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20;
        //m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21;
        //m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22;
        //m33 = m33;
        return *this;
    }

//    mat4x4 &operator*=(const mat4x4 &b)
//    {
//        mat4x4 a = *this;
//        m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20 + a.m03 * b.m30;
//        m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21 + a.m03 * b.m31;
//        m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22 + a.m03 * b.m32;
//        m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03 * b.m33;
//        m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20 + a.m13 * b.m30;
//        m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31;
//        m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32;
//        m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33;
//        m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20 + a.m23 * b.m30;
//        m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31;
//        m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32;
//        m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33;
//        m30 = a.m30 * b.m00 + a.m31 * b.m10 + a.m32 * b.m20 + a.m33 * b.m30;
//        m31 = a.m30 * b.m01 + a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31;
//        m32 = a.m30 * b.m02 + a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32;
//        m33 = a.m30 * b.m03 + a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33;
//        return *this;
//    }

    mat4x4 &operator*=(const mat4x4 &b)
    {
        mat4x4 a = *this;
        m00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
        m01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
        m02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
        m03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03;
        m10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
        m11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
        m12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
        m13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13;
        m20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
        m21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
        m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
        m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23;
        m30 = 0.0;
        m31 = 0.0;
        m32 = 0.0;
        m33 = 1.0;
        return *this;
    }

    mat4x4 &operator+=(const vec3d &t)
    {
        m03 += t.x;
        m13 += t.y;
        m23 += t.z;
        return *this;
    }

    mat4x4 &operator-=(const vec3d &t)
    {
        m03 -= t.x;
        m13 -= t.y;
        m23 -= t.z;
        return *this;
    }

    mat4x4 &operator=(const mat3x3 &m)
    {
        m00 = m.m00;
        m01 = m.m01;
        m02 = m.m02;
        m03 = 0.0;
        m10 = m.m10;
        m11 = m.m11;
        m12 = m.m12;
        m13 = 0.0;
        m20 = m.m20;
        m21 = m.m21;
        m22 = m.m22;
        m23 = 0.0;
        m30 = 0.0;
        m31 = 0.0;
        m32 = 0.0;
        m33 = 1.0;
        return *this;
    }

    const mat4x4 operator*(const mat3x3 &b) const
    {
        mat4x4 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const mat4x4 operator*(const mat4x4 &b) const
    {
        mat4x4 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const mat4x4 operator+(const vec3d &t) const
    {
        mat4x4 tmp = *this;
        tmp += t;
        return tmp;
    }

    const mat4x4 operator-(const vec3d &t) const
    {
        mat4x4 tmp = *this;
        tmp -= t;
        return tmp;
    }

    vec3d operator*(const vec3d &v) const
    {
        return Transform(v);
    }

    void setTranslate(const vec3d &t)
    {
        m03 = t.x;
        m13 = t.y;
        m23 = t.z;
    }

    void invertTranslate()
    {
        m03 = -m03;
        m13 = -m13;
        m23 = -m23;
    }

    const vec3d getTranslate() const
    {
        return vec3d(m03, m13, m23);
    }

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


void mat_mult(mat3x3 *mat1, mat3x3 *mat2, mat3x3 *dst);
void mat_rotate_y(mat3x3 *mat, float a2);

void mat_gen_axis_rotate(vec3d *u, float angle, mat3x3 *out, int flags = 0);

#endif // ENGINE_TFORM_H_INCLUDED
