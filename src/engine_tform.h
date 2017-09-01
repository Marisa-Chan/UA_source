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

struct xyz
{
    typedef float xyzFLT;
    xyzFLT sx;
    xyzFLT sy;
    xyzFLT sz;

    xyz()
    {
        sx = 0.0;
        sy = 0.0;
        sz = 0.0;
    }

    xyz(xyzFLT _x, xyzFLT _y, xyzFLT _z)
    {
        sx = _x;
        sy = _y;
        sz = _z;
    }

    xyz(xyzFLT _b)
    {
        sx = _b;
        sy = _b;
        sz = _b;
    }

    xyzFLT length()
    {
        return sqrt( sx * sx + sy * sy + sz * sz );
    }

    xyzFLT normolize()
    {
        xyzFLT ln = length();

        if (ln == 0.0)
            return 0.0;

        *this /= ln;

        return ln;
    }

    const xyz normolize(float *pln) const
    {
        xyz tmp = *this;

        xyzFLT ln = tmp.length();

        tmp /= ln;

        if (pln)
            *pln = ln;

        return tmp;
    }

    // xyz = another xyz
    xyz &operator=(const xyz &b)
    {
        sx = b.sx;
        sy = b.sy;
        sz = b.sz;

        return *this;
    }

    // xyz = float
    xyz &operator=(xyzFLT b)
    {
        sx = b;
        sy = b;
        sz = b;

        return *this;
    }

    xyz &operator+=(const xyz &b)
    {
        sx += b.sx;
        sy += b.sy;
        sz += b.sz;
        return *this;
    }

    xyz &operator-=(const xyz &b)
    {
        sx -= b.sx;
        sy -= b.sy;
        sz -= b.sz;
        return *this;
    }

    xyz &operator*=(const xyz &b)
    {
        xyz tmp = *this;
        sx = tmp.sy * b.sz - tmp.sz * b.sy;
        sy = tmp.sz * b.sx - tmp.sx * b.sz;
        sz = tmp.sx * b.sy - tmp.sy * b.sx;
        return *this;
    }

    xyz &operator+=(xyzFLT b)
    {
        sx += b;
        sy += b;
        sz += b;
        return *this;
    }

    xyz &operator-=(xyzFLT b)
    {
        sx -= b;
        sy -= b;
        sz -= b;
        return *this;
    }

    xyz &operator*=(xyzFLT b)
    {
        sx *= b;
        sy *= b;
        sz *= b;
        return *this;
    }

    xyz &operator/=(xyzFLT b)
    {
        if (b != 0.0)
        {
            double tmp = 1.0 / b;
            *this *= tmp;
        }

        return *this;
    }

    const xyz operator-() const
    {
        return xyz(-sx, -sy, -sz);
    }

    const xyz operator+(const xyz &b) const
    {
        xyz tmp = *this;
        tmp += b;
        return tmp;
    }

    const xyz operator-(const xyz &b) const
    {
        xyz tmp = *this;
        tmp -= b;
        return tmp;
    }

    const xyz operator*(const xyz &b) const
    {
        xyz tmp = *this;
        tmp *= b;
        return tmp;
    }

    const xyz operator+(xyzFLT b) const
    {
        xyz tmp = *this;
        tmp += b;
        return tmp;
    }

    const xyz operator-(xyzFLT b) const
    {
        xyz tmp = *this;
        tmp -= b;
        return tmp;
    }

    const xyz operator*(xyzFLT b) const
    {
        xyz tmp = *this;
        tmp *= b;
        return tmp;
    }

    const xyz operator/(xyzFLT b) const
    {
        xyz tmp = *this;
        tmp /= b;
        return tmp;
    }

    bool operator==(const xyz &b) const
    {
        return sx == b.sx && sy == b.sy && sz == b.sz;
    }

    bool operator!=(const xyz &b) const
    {
        return sx != b.sx || sy != b.sy || sz != b.sz;
    }

    xyzFLT dot(const xyz &b)
    {
        return sx * b.sx + sy * b.sy + sz * b.sz;
    }

    bool readIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->readFloatB(sx);
            res &= iff->readFloatB(sy);
            res &= iff->readFloatB(sz);
        }
        else
        {
            res &= iff->readFloatL(sx);
            res &= iff->readFloatL(sy);
            res &= iff->readFloatL(sz);
        }
        return res;
    }

    bool writeIFF(IFFile *iff, bool BigEndian)
    {
        bool res = true;
        if (BigEndian)
        {
            res &= iff->writeFloatB(sx);
            res &= iff->writeFloatB(sy);
            res &= iff->writeFloatB(sz);
        }
        else
        {
            res &= iff->writeFloatL(sx);
            res &= iff->writeFloatL(sy);
            res &= iff->writeFloatL(sz);
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

    const xyz getVect(int id) const
    {
        switch (id)
        {
        default:
        case 0:
            return xyz(m00, m01, m02);

        case 1:
            return xyz(m10, m11, m12);

        case 2:
            return xyz(m20, m21, m22);

        case 10:
            return xyz(m00, m10, m20);

        case 11:
            return xyz(m01, m11, m21);

        case 12:
            return xyz(m02, m12, m22);
        }
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

    xyz Transform(const xyz &b) const
    {
        xyz tmp;
        tmp.sx = m00 * b.sx + m01 * b.sy + m02 * b.sz;
        tmp.sy = m10 * b.sx + m11 * b.sy + m12 * b.sz;
        tmp.sz = m20 * b.sx + m21 * b.sy + m22 * b.sz;
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

    mat4x4(const xyz &t)
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

    xyz Transform(const xyz &b) const
    {
        xyz tmp;
        tmp.sx = m00 * b.sx + m01 * b.sy + m02 * b.sz + m03;
        tmp.sy = m10 * b.sx + m11 * b.sy + m12 * b.sz + m13;
        tmp.sz = m20 * b.sx + m21 * b.sy + m22 * b.sz + m23;
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

    mat4x4 &operator+=(const xyz &t)
    {
        m03 += t.sx;
        m13 += t.sy;
        m23 += t.sz;
        return *this;
    }

    mat4x4 &operator-=(const xyz &t)
    {
        m03 -= t.sx;
        m13 -= t.sy;
        m23 -= t.sz;
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

    const mat4x4 operator+(const xyz &t) const
    {
        mat4x4 tmp = *this;
        tmp += t;
        return tmp;
    }

    const mat4x4 operator-(const xyz &t) const
    {
        mat4x4 tmp = *this;
        tmp -= t;
        return tmp;
    }

    xyz operator*(const xyz &v) const
    {
        return Transform(v);
    }

    void setTranslate(const xyz &t)
    {
        m03 = t.sx;
        m13 = t.sy;
        m23 = t.sz;
    }

    void invertTranslate()
    {
        m03 = -m03;
        m13 = -m13;
        m23 = -m23;
    }

    const xyz getTranslate() const
    {
        return xyz(m03, m13, m23);
    }

};

struct destFX
{
    char type_flag;
    int p1; // Model id. >= 0
    float p2;
    float p3;
    float p4;
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

void mat_gen_axis_rotate(xyz *u, float angle, mat3x3 *out, int flags = 0);

#endif // ENGINE_TFORM_H_INCLUDED
