#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include <math.h>
#include "vectors.h"

#define MAT_FLAG_INV_SIN   1
#define MAT_FLAG_INV_COS   2

template <typename T, typename K> struct Tmat3x3;
template <typename T, typename K> struct Tmat4x4;

typedef Tmat3x3<double, double> mat3x3;
typedef Tmat4x4<double, double> mat4x4;


template <typename T, typename K>
struct Tmat3x3
{
    T m00;
    T m01;
    T m02;
    T m10;
    T m11;
    T m12;
    T m20;
    T m21;
    T m22;

    Tmat3x3()
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

    Tmat3x3(T _m00, T _m01, T _m02,
            T _m10, T _m11, T _m12,
            T _m20, T _m21, T _m22)
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

    Tmat3x3 (const Tvec3d<K> &axis, T angle, int flags = 0)
    {
        T cs = cos(angle);
        T sn = sin(angle);

        if (flags & MAT_FLAG_INV_COS)
            cs = -cs;

        if (flags & MAT_FLAG_INV_SIN)
            sn = -sn;

        T ics = 1.0 - cs;

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

    const Tvec3d<K> AxisX() const
    {
        return Tvec3d<K>(m00, m01, m02);
    }

    const Tvec3d<K> AxisY() const
    {
        return Tvec3d<K>(m10, m11, m12);
    }

    const Tvec3d<K> AxisZ() const
    {
        return Tvec3d<K>(m20, m21, m22);
    }

    void SetX(const Tvec3d<K> &v)
    {
        m00 = v.x;
        m01 = v.y;
        m02 = v.z;
    }

    void SetY(const Tvec3d<K> &v)
    {
        m10 = v.x;
        m11 = v.y;
        m12 = v.z;
    }

    void SetZ(const Tvec3d<K> &v)
    {
        m20 = v.x;
        m21 = v.y;
        m22 = v.z;
    }

    Tvec3d<K> Transform(const Tvec3d<K> &b) const
    {
        Tvec3d<K> tmp;
        tmp.x = m00 * b.x + m01 * b.y + m02 * b.z;
        tmp.y = m10 * b.x + m11 * b.y + m12 * b.z;
        tmp.z = m20 * b.x + m21 * b.y + m22 * b.z;
        return tmp;
    }

    Tmat3x3 &operator*=(const Tmat3x3 &b)
    {
        Tmat3x3 a = *this;
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

    Tmat3x3 &operator*=(T b)
    {
        m00 *= b;
        m01 *= b;
        m02 *= b;
        m10 *= b;
        m11 *= b;
        m12 *= b;
        m20 *= b;
        m21 *= b;
        m22 *= b;
        return *this;
    }

    Tmat3x3 &operator/=(T b)
    {
        m00 /= b;
        m01 /= b;
        m02 /= b;
        m10 /= b;
        m11 /= b;
        m12 /= b;
        m20 /= b;
        m21 /= b;
        m22 /= b;
        return *this;
    }

    Tmat3x3 &operator+=(const Tmat3x3 &b)
    {
        m00 += b.m00;
        m01 += b.m01;
        m02 += b.m02;
        m10 += b.m10;
        m11 += b.m11;
        m12 += b.m12;
        m20 += b.m20;
        m21 += b.m21;
        m22 += b.m22;
        return *this;
    }

    Tmat3x3 &operator-=(const Tmat3x3 &b)
    {
        m00 -= b.m00;
        m01 -= b.m01;
        m02 -= b.m02;
        m10 -= b.m10;
        m11 -= b.m11;
        m12 -= b.m12;
        m20 -= b.m20;
        m21 -= b.m21;
        m22 -= b.m22;
        return *this;
    }

    const Tmat3x3 operator*(const Tmat3x3 &b) const
    {
        Tmat3x3 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tmat3x3 operator+(const Tmat3x3 &b) const
    {
        Tmat3x3 tmp = *this;
        tmp += b;
        return tmp;
    }

    const Tmat3x3 operator-(const Tmat3x3 &b) const
    {
        Tmat3x3 tmp = *this;
        tmp -= b;
        return tmp;
    }

    const Tmat3x3 operator*(T b) const
    {
        Tmat3x3 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tmat3x3 operator/(T b) const
    {
        Tmat3x3 tmp = *this;
        tmp /= b;
        return tmp;
    }

    const Tmat3x3 Transpose() const
    {
        return Tmat3x3(m00, m10, m20,
                       m01, m11, m21,
                       m02, m12, m22);
    }

    static const Tmat3x3 RotateX(T angle)
    {
        T sn = sin(-angle);
        T cs = cos(angle);

        return Tmat3x3(1.0, 0.0, 0.0,
                       0.0,  cs, -sn,
                       0.0,  sn,  cs);
    }

    static const Tmat3x3 RotateY(T angle)
    {
        T sn = sin(angle);
        T cs = cos(angle);

        return Tmat3x3( cs, 0.0,  sn,
                        0.0, 1.0, 0.0,
                        -sn, 0.0,  cs);
    }

    static const Tmat3x3 RotateZ(T angle)
    {
        T sn = sin(-angle);
        T cs = cos(angle);

        return Tmat3x3( cs, -sn, 0.0,
                        sn,  cs, 0.0,
                        0.0, 0.0, 1.0);
    }

    static const Tmat3x3 AxisAngle(const Tvec3d<K> &v, T angle)
    {
        return Tmat3x3( v, angle, MAT_FLAG_INV_SIN );
    }

    static const Tmat3x3 Ident()
    {
        return Tmat3x3(1.0, 0.0, 0.0,
                       0.0, 1.0, 0.0,
                       0.0, 0.0, 1.0);
    }

    static const Tmat3x3 Basis(const Tvec3d<K> &x, const Tvec3d<K> &y, const Tvec3d<K> &z)
    {
        return Tmat3x3( x.x, x.y, x.z,
                        y.x, y.y, y.z,
                        z.x, z.y, z.z );
    }

    static const Tmat3x3 Scale(const Tvec3d<K> &sc)
    {
        return Tmat3x3( sc.x,  0.0,  0.0,
                        0.0, sc.y,  0.0,
                        0.0,  0.0, sc.z );
    }

    const Tvec3d<K> GetEuler() const
    {
        float sy = sqrt(m00 * m00 + m10 * m10);

        bool singular = sy < 1e-6;

        if ( !singular )
        {
            return Tvec3d<K>( atan2(m21, m22),
                              atan2(-m20, sy),
                              atan2(m10, m00) );
        }
        else
        {
            return Tvec3d<K>( atan2(-m12, m11),
                              atan2(-m20, sy),
                              0.0 );
        }
    }

    static const Tmat3x3 Euler(const Tvec3d<K> &euler)
    {
        float _cx = cos(euler.x);
        float _sx = sin(euler.x);
        float _cy = cos(euler.y);
        float _sy = sin(euler.y);
        float _cz = cos(euler.z);
        float _sz = sin(euler.z);

        return Tmat3x3( _cy * _cz,   _sy * _sx * _cz - _cx * _sz,   _sy * _cx * _cz + _sx * _sz,
                        _cy * _sz,   _sy * _sx * _sz + _cx * _cz,   _sy * _cx * _sz - _sx * _cz,
                        -_sy,                     _cy * _sx,                     _cy * _cx );
    }

    static const Tmat3x3 Euler(T x, T y, T z)
    {
        float _cx = cos(x);
        float _sx = sin(x);
        float _cy = cos(y);
        float _sy = sin(y);
        float _cz = cos(z);
        float _sz = sin(z);

        return Tmat3x3( _cy * _cz,   _sy * _sx * _cz - _cx * _sz,   _sy * _cx * _cz + _sx * _sz,
                        _cy * _sz,   _sy * _sx * _sz + _cx * _cz,   _sy * _cx * _sz - _sx * _cz,
                        -_sy,                     _cy * _sx,                     _cy * _cx );
    }

    static const Tmat3x3 Euler_ZXY(const Tvec3d<K> &euler)
    {
        float c1 = cos(euler.z);
        float s1 = sin(euler.z);
        float c2 = cos(euler.x);
        float s2 = sin(euler.x);
        float c3 = cos(euler.y);
        float s3 = sin(euler.y);

        return Tmat3x3( c1*c3 - s1*s2*s3,   -c2*s1,   c1*s3 + c3*s1*s2,
                        c3*s1 + c1*s2*s3,    c1*c2,   s1*s3 - c1*c3*s2,
                        -c2*s3,       s2,              c2*c3);
    }
};

template <typename T, typename K>
struct Tmat4x4
{
    T m00;
    T m01;
    T m02;
    T m03;
    T m10;
    T m11;
    T m12;
    T m13;
    T m20;
    T m21;
    T m22;
    T m23;
    T m30;
    T m31;
    T m32;
    T m33;

    Tmat4x4()
    {
        identity();
    }
    
    Tmat4x4(Tmat4x4 &&) = default;
    Tmat4x4(const Tmat4x4 &) = default;

    Tmat4x4(const Tvec3d<K> &t)
    {
        identity();
        setTranslate(t);
    }

    Tmat4x4(const Tmat3x3<T, K> &m)
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

    Tvec3d<K> Transform(const Tvec3d<K> &b) const
    {
        Tvec3d<K> tmp;
        tmp.x = m00 * b.x + m01 * b.y + m02 * b.z + m03;
        tmp.y = m10 * b.x + m11 * b.y + m12 * b.z + m13;
        tmp.z = m20 * b.x + m21 * b.y + m22 * b.z + m23;
        return tmp;
    }

    Tmat4x4 &operator*=(const Tmat3x3<T, K> &b)
    {
        Tmat4x4 a = *this;
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

//    Tmat4x4 &operator*=(const Tmat4x4 &b)
//    {
//        Tmat4x4 a = *this;
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

    Tmat4x4 &operator*=(const Tmat4x4 &b)
    {
        Tmat4x4 a = *this;
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

    Tmat4x4 &operator+=(const Tvec3d<K> &t)
    {
        m03 += t.x;
        m13 += t.y;
        m23 += t.z;
        return *this;
    }

    Tmat4x4 &operator-=(const Tvec3d<K> &t)
    {
        m03 -= t.x;
        m13 -= t.y;
        m23 -= t.z;
        return *this;
    }

    Tmat4x4 &operator=(const Tmat3x3<T, K> &m)
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

    const Tmat4x4 operator*(const Tmat3x3<T, K> &b) const
    {
        Tmat4x4 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tmat4x4 operator*(const Tmat4x4 &b) const
    {
        Tmat4x4 tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tmat4x4 operator+(const Tvec3d<K> &t) const
    {
        Tmat4x4 tmp = *this;
        tmp += t;
        return tmp;
    }

    const Tmat4x4 operator-(const Tvec3d<K> &t) const
    {
        Tmat4x4 tmp = *this;
        tmp -= t;
        return tmp;
    }
    
    Tmat4x4 &operator=(const Tmat4x4 &b) = default;

    Tvec3d<K> operator*(const Tvec3d<K> &v) const
    {
        return Transform(v);
    }

    void setTranslate(const Tvec3d<K> &t)
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

    const Tvec3d<K> getTranslate() const
    {
        return Tvec3d<K>(m03, m13, m23);
    }

};

#endif // MATRIX_H_INCLUDED
