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
