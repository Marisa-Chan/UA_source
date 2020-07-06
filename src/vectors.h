#ifndef VECTORS_H_INCLUDED
#define VECTORS_H_INCLUDED

#include <math.h>

template <typename T>
struct Tvec2d
{
    T x;
    T y;

    Tvec2d()
    {
        x = 0.0;
        y = 0.0;
    }
    
    Tvec2d( const Tvec2d<T> &b)
    {
        x = b.x;
        y = b.y;
    }

    Tvec2d(T _x, T _y)
    {
        x = _x;
        y = _y;
    }

    Tvec2d(T _b)
    {
        x = _b;
        y = _b;
    }

    T length() const
    {
        return sqrt( x * x + y * y);
    }
    
    T square() const
    {
        return x * x + y * y;
    }

    T norm() const
    {
        return length();
    }

    T normalise()
    {
        T ln = length();

        if (ln == 0.0)
            return 0.0;

        *this /= ln;

        return ln;
    }

    const Tvec2d normalise(T *pln) const
    {
        Tvec2d tmp = *this;

        T ln = tmp.length();

        tmp /= ln;

        if (pln)
            *pln = ln;

        return tmp;
    }

    Tvec2d &operator=(const Tvec2d &b)
    {
        x = b.x;
        y = b.y;
        return *this;
    }

    Tvec2d &operator=(T b)
    {
        x = b;
        y = b;
        return *this;
    }

    Tvec2d &operator+=(const Tvec2d &b)
    {
        x += b.x;
        y += b.y;
        return *this;
    }

    Tvec2d &operator-=(const Tvec2d &b)
    {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    Tvec2d &operator+=(T b)
    {
        x += b;
        y += b;
        return *this;
    }

    Tvec2d &operator-=(T b)
    {
        x -= b;
        y -= b;
        return *this;
    }

    Tvec2d &operator*=(T b)
    {
        x *= b;
        y *= b;
        return *this;
    }

    Tvec2d &operator/=(T b)
    {
        x /= b;
        y /= b;
        return *this;
    }

    const Tvec2d operator-() const
    {
        return Tvec2d(-x, -y);
    }

    const Tvec2d operator+(const Tvec2d &b) const
    {
        Tvec2d tmp = *this;
        tmp += b;
        return tmp;
    }

    const Tvec2d operator-(const Tvec2d &b) const
    {
        Tvec2d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const Tvec2d operator+(T b) const
    {
        Tvec2d tmp = *this;
        tmp += b;
        return tmp;
    }

    const Tvec2d operator-(T b) const
    {
        Tvec2d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const Tvec2d operator*(T b) const
    {
        Tvec2d tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tvec2d operator/(T b) const
    {
        Tvec2d tmp = *this;
        tmp /= b;
        return tmp;
    }

    bool operator==(const Tvec2d &b) const
    {
        return x == b.x && y == b.y;
    }

    bool operator!=(const Tvec2d &b) const
    {
        return x != b.x || y != b.y;
    }

    T dot(const Tvec2d &b) const
    {
        return x * b.x + y * b.y;
    }

    T cross(const Tvec2d &b) const
    {
        return x * b.y - y * b.x;
    }

    T xyAngle()
    {
        if (x == 0.0 && y == 0.0)
            return 0.0;
        return atan2(y, x);
    }

    T yxAngle()
    {
        if (x == 0.0 && y == 0.0)
            return 0.0;
        return atan2(x, y);
    }
};


template <typename T>
struct Tvec3d
{
    T x;
    T y;
    T z;

    Tvec3d()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    }
    
    Tvec3d(const Tvec3d<T> &b)
    {
        x = b.x;
        y = b.y;
        z = b.z;
    }

    Tvec3d(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Tvec3d(T _b)
    {
        x = _b;
        y = _b;
        z = _b;
    }

    T length() const
    {
        return sqrt( x * x + y * y + z * z );
    }
    
    T square() const
    {
        return x * x + y * y + z * z;
    }

    T normalise()
    {
        T ln = length();

        if (ln == 0.0)
            return 0.0;

        *this /= ln;

        return ln;
    }

    const Tvec3d normalise(float *pln) const
    {
        Tvec3d tmp = *this;

        T ln = tmp.length();

        tmp /= ln;

        if (pln)
            *pln = ln;

        return tmp;
    }

    // xyz = another xyz
    Tvec3d &operator=(const Tvec3d &b)
    {
        x = b.x;
        y = b.y;
        z = b.z;

        return *this;
    }

    // xyz = float
    Tvec3d &operator=(T b)
    {
        x = b;
        y = b;
        z = b;

        return *this;
    }

    Tvec3d &operator+=(const Tvec3d &b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }

    Tvec3d &operator-=(const Tvec3d &b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }

    Tvec3d &operator*=(const Tvec3d &b)
    {
        Tvec3d tmp = *this;
        x = tmp.y * b.z - tmp.z * b.y;
        y = tmp.z * b.x - tmp.x * b.z;
        z = tmp.x * b.y - tmp.y * b.x;
        return *this;
    }

    Tvec3d &operator+=(T b)
    {
        x += b;
        y += b;
        z += b;
        return *this;
    }

    Tvec3d &operator-=(T b)
    {
        x -= b;
        y -= b;
        z -= b;
        return *this;
    }

    Tvec3d &operator*=(T b)
    {
        x *= b;
        y *= b;
        z *= b;
        return *this;
    }

    Tvec3d &operator/=(T b)
    {
        x /= b;
        y /= b;
        z /= b;
        return *this;
    }

    const Tvec3d operator-() const
    {
        return Tvec3d(-x, -y, -z);
    }

    const Tvec3d operator+(const Tvec3d &b) const
    {
        Tvec3d tmp = *this;
        tmp += b;
        return tmp;
    }

    const Tvec3d operator-(const Tvec3d &b) const
    {
        Tvec3d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const Tvec3d operator*(const Tvec3d &b) const
    {
        Tvec3d tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tvec3d operator+(T b) const
    {
        Tvec3d tmp = *this;
        tmp += b;
        return tmp;
    }

    const Tvec3d operator-(T b) const
    {
        Tvec3d tmp = *this;
        tmp -= b;
        return tmp;
    }

    const Tvec3d operator*(T b) const
    {
        Tvec3d tmp = *this;
        tmp *= b;
        return tmp;
    }

    const Tvec3d operator/(T b) const
    {
        Tvec3d tmp = *this;
        tmp /= b;
        return tmp;
    }

    bool operator==(const Tvec3d &b) const
    {
        return x == b.x && y == b.y && z == b.z;
    }

    bool operator!=(const Tvec3d &b) const
    {
        return x != b.x || y != b.y || z != b.z;
    }

    T dot(const Tvec3d &b) const
    {
        return x * b.x + y * b.y + z * b.z;
    }

    const Tvec2d<T> XZ() const
    {
        return Tvec2d<T>(x, z);
    }

    const Tvec3d X0Z() const
    {
        return Tvec3d(x, 0.0, z);
    }

    static const Tvec3d X0Z(const Tvec2d<T> &in)
    {
        return Tvec3d(in.x, 0.0, in.y);
    }
    
    static const Tvec3d X0Z(const Tvec3d<T> &in)
    {
        return Tvec3d(in.x, 0.0, in.z);
    }

    static const Tvec3d OX(T _x)
    {
        return Tvec3d(_x, 0.0, 0.0);
    }

    static const Tvec3d OY(T _y)
    {
        return Tvec3d(0.0, _y, 0.0);
    }

    static const Tvec3d OZ(T _z)
    {
        return Tvec3d(0.0, 0.0, _z);
    }
    
    static const Tvec3d Normalise(const Tvec3d &b)
    {
        Tvec3d tmp = b;
        T l = tmp.length();
        if (l != 0.0)
            tmp /= l;
        return tmp;
    }
};

#endif // VECTORS_H_INCLUDED
