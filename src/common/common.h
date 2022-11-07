#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <inttypes.h>
#include <vector>
#include <array>
#include <deque>
#include <SDL2/SDL.h>

#ifdef ABS
#undef ABS
#endif

#ifdef MIN
#undef MIN
#endif

#ifdef MAX
#undef MAX
#endif

namespace Common
{

template<typename T> inline T ABS(T x)		{ return (x >= 0) ? x : -x; }
template<typename T> inline T MIN(T a, T b)	{ return (a < b) ? a : b; }
template<typename T> inline T MAX(T a, T b)	{ return (a > b) ? a : b; }
template<typename T> inline void DeleteAndNull(T *a) { if (*a) {delete *a; *a = NULL;} }


template <typename T> struct TPoint;
template <typename T> struct TRect;
template <typename T> struct TPointRect;

typedef TPoint<int32_t> Point;
typedef TRect<int32_t> Rect;
typedef TPointRect<int32_t> PointRect;
typedef TPoint<float> FPoint;
typedef TRect<float> FRect;

template <typename T>
struct TPoint
{
    typedef TPoint<T> __Point;    
    T x;
    T y;

    TPoint() : x(0), y(0) {};
    TPoint(T x1, T y1) : x(x1), y(y1) {};

    bool  operator==(const __Point &p)    const { return x == p.x && y == p.y; }
    bool  operator!=(const __Point &p)    const { return x != p.x || y != p.y; }
    __Point operator+(const __Point &delta) const { return TPoint(x + delta.x, y + delta.y); }
    __Point operator-(const __Point &delta) const { return TPoint(x - delta.x, y - delta.y); }
    
    __Point operator-() const { return __Point(-x, -y); } // -Point

    void operator+=(const __Point &delta) {
            x += delta.x;
            y += delta.y;
    }

    void operator-=(const __Point &delta) {
            x -= delta.x;
            y -= delta.y;
    }
    
    template <typename R>
    R Length() const {
        return sqrt( (x * x) + (y * y) );
    }
    
    template <typename R>
    R LengthTo(const __Point &b) const {
        return sqrt( (x - b.x) * (x - b.x) + ((y - b.y)  * (y - b.y)) );
    }

    __Point Invert() const { return TPoint( -x, -y ); }
    
    __Point AbsDistance(const __Point &b) const { return TPoint(ABS(x - b.x), ABS(y - b.y)); }
    
    bool IsNull() const { return !( operator bool() ); };
    
    operator bool() const
    {
        if (x != 0 || y != 0)
            return true;
        return false;
    }
    
    operator SDL_Rect() const
    {
        SDL_Rect tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.w = 0;
        tmp.h = 0;
        return tmp;
    }
};



template <typename T>
struct TRect
{
    T left;
    T top;
    T right;
    T bottom;

    TRect() : left(0), top(0), right(0), bottom(0) {};
    TRect(T w, T h) : left(0), top(0), right(w), bottom(h) {};
    TRect(TPoint<T> p) : left(0), top(0), right(p.x), bottom(p.y) {};
    TRect(TPoint<T> lt, TPoint<T> rb) : left(lt.x), top(lt.y), right(rb.x), bottom(rb.y) {};
    TRect(T x1, T y1, T x2, T y2) : left(x1), top(y1), right(x2), bottom(y2) {};
    TRect(const SDL_Rect &r) : left(r.x), top(r.y), right(r.x + r.w), bottom(r.y + r.h) {};

    bool IsValid() const {
            return (left <= right && top <= bottom);
    }

    bool IsEmpty() const {
            return (left >= right || top >= bottom);
    }

    inline T Width() const { return right - left; }
    inline T Height() const { return bottom - top; }

    void SetWidth(T Width) {
        right = left + Width;
    }

    void SetHeight(T Height) {
        bottom = top + Height;
    }

    void SetSize(TPoint<T> sz) {
        right = left + sz.x;
        bottom = top + sz.y;
    }
    
    void SetSize(T x, T y) {
        right = left + x;
        bottom = top + y;
    }

    bool IsIn(T x, T y) const {
        if( IsEmpty() )
            return false;
        
        return (x >= left) && (x < right) && (y >= top) && (y < bottom);
    }
    bool IsIn(const Point &p) const {
            return IsIn(p.x, p.y);
    }
    bool IsIn(const TRect &r) const {
        if( IsEmpty() )
            return false;
        
        return (left <= r.left) && (right >= r.right) && (top <= r.top) && (bottom >= r.bottom);
    }

    bool IsIntersects(const TRect &r) const {
        return (left < r.right) && (r.left < right) && (top < r.bottom) && (r.top < bottom);
    }

    TRect IntersectionRect(const TRect &r) const {
        if (!IsIntersects(r))
                return TRect();

        return TRect(MAX(r.left, left), MAX(r.top, top), MIN(r.right, right), MIN(r.bottom, bottom));
    }

    void ClipBy(const TRect &r) {
        if (top < r.top) top = r.top;
        else if (top > r.bottom) top = r.bottom;

        if (left < r.left) left = r.left;
        else if (left > r.right) left = r.right;

        if (bottom > r.bottom) bottom = r.bottom;
        else if (bottom < r.top) bottom = r.top;

        if (right > r.right) right = r.right;
        else if (right < r.left) right = r.left;
    }

    void ClipBy(int maxw, int maxh) {
        ClipBy(TRect(0, 0, maxw, maxh));
    }

    void MoveTo(T x, T y) {
        bottom += y - top;
        right += x - left;
        top = y;
        left = x;
    }

    void MoveTo(const TPoint<T> &p) {
        MoveTo(p.x, p.y);
    }

    void Translate(T dx, T dy) {
        left += dx; right += dx;
        top += dy; bottom += dy;
    }

    void Translate(const TPoint<T> &p) {
        left += p.x; right += p.x;
        top += p.y; bottom += p.y;
    }

    operator bool() const
    {
        return !IsEmpty();
    }

    TPoint<T> Pos() const
    {
        return TPoint<T>(left, top);
    }

    TPoint<T> Size() const
    {
        return TPoint<T>(Width(), Height());
    }
    
    operator TPointRect<T>() const;
    
    operator SDL_Rect() const
    {
        SDL_Rect tmp;
        tmp.x = left;
        tmp.y = top;
        tmp.w = Width();
        tmp.h = Height();
        return tmp;
    }

};



template <typename T>
struct TPointRect
{
    T x;
    T y;
    T w;
    T h;

    TPointRect() : x(0), y(0), w(0), h(0) {};
    TPointRect(int x1, int y1) : x(x1), y(y1), w(0), h(0) {};
    TPointRect(TPoint<T> p) : x(p.x), y(p.y), w(0), h(0) {};
    TPointRect(TPoint<T> p, T w_, T h_) : x(p.x), y(p.y), w(w_), h(h_) {};
    TPointRect(T x_, T y_, T w_, T h_) : x(x_), y(y_), w(w_), h(h_) {};
    TPointRect(const SDL_Rect &r) : x(r.x), y(r.y), w(r.w), h(r.h) {};
    
    T Right() const { return x + w; }
    T Bottom() const { return y + h; }
    
    bool  operator==(const PointRect &p)    const { return x == p.x && y == p.y && w == p.w && h == p.h; }
    bool  operator!=(const PointRect &p)    const { return x != p.x || y != p.y || w != p.w || h != p.h; }
    TPointRect operator+(const TPoint<T> &delta) const { return TPointRect(x + delta.x, y + delta.y, w, h); }
    TPointRect operator-(const TPoint<T> &delta) const { return TPointRect(x - delta.x, y - delta.y, w, h); }

    void operator+=(const TPoint<T> &delta) {
            x += delta.x;
            y += delta.y;
    }

    void operator-=(const TPoint<T> &delta) {
            x -= delta.x;
            y -= delta.y;
    }
    
    void MoveTo(T px, T py) {
            x = px;
            y = py;
    }

    void MoveTo(const TPoint<T> &p) {
            MoveTo(p.x, p.y);
    }

    void Translate(T dx, T dy) {
            x += dx;
            y += dy;
    }

    void Translate(const TPoint<T> &p) {
            x += p.x;
            y += p.y;
    }
    
    bool IsIn(T px, T py) const {
            return (px >= x) && (px < Right()) && (py >= y) && (py < Bottom());
    }
    bool IsIn(const TPoint<T> &p) const {
            return IsIn(p.x, p.y);
    }
    bool IsIn(const TRect<T> &r) const {
            return (r.left >= x) && (r.right <= Right()) && (r.top >= y) && (r.bottom <= Bottom());
    }
    bool IsIn(const TPointRect<T> &r) const {
            return (r.x >= x) && (r.Right() <= Right()) && (r.y >= y) && (r.Bottom() <= Bottom());
    }

    bool IsIntersects(const TRect<T> &r) const {
            return (x < r.right) && (Right() > r.left) && (y < r.bottom) && (Bottom() > r.top);
    }
    
    bool IsIntersects(const TPointRect<T> &r) const {
            return (x < r.Right()) && (Right() > r.x) && (y < r.Bottom()) && (Bottom() > r.y);
    }
   
    void ClipBy(const TRect<T> &r) {
            if (y < r.top) y = r.top;
            else if (y > r.bottom) y = r.bottom;

            if (x < r.left) x = r.left;
            else if (x > r.right) x = r.right;

            if (Bottom() > r.bottom) h = r.bottom - y;
            else if (Bottom() < r.top) h = r.top - y;

            if (Right() > r.right) w = r.right - x;
            else if (Right() < r.left) w = r.left - x;
    }

    void ClipBy(int maxw, int maxh) {
            ClipBy(TRect<T>(0, 0, maxw, maxh));
    }
    
    void ClipBy(const TPointRect<T> &r) {
            if (y < r.y) y = r.y;
            else if (y > r.Bottom()) y = r.Bottom();

            if (x < r.x) x = r.x;
            else if (x > r.Right()) x = r.Right();

            if (Bottom() > r.Bottom()) h = r.Bottom() - y;
            else if (Bottom() < r.y) h = r.y - y;

            if (Right() > r.Right()) w = r.Right() - x;
            else if (Right() < r.x) w = r.x - x;
    }
    

    bool IsEmpty() const {
            return (w == 0 || h == 0);
    }
    
    bool IsValid() const {
            return (w >= 0 && h >= 0);
    }
    
    TPoint<T> Pos() const
    {
        return TPoint<T>(x, y);
    }

    TPoint<T> Size() const
    {
        return TPoint<T>(w, h);
    }
    
    operator bool() const
    {
        return !IsEmpty();
    }

    operator TPoint<T>() const
    {
        return TPoint<T>(x, y);
    }
    
    operator TRect<T>() const
    {
        return TRect<T>(x, y, x + w, y + h);
    }
    
    operator SDL_Rect() const
    {
        SDL_Rect tmp;
        tmp.x = x;
        tmp.y = y;
        tmp.w = w;
        tmp.h = h;
        return tmp;
    }

};

template<typename T>
TRect<T>::operator TPointRect<T>() const
{
    return TPointRect<T>(left, top, Width(), Height());
}


}

#endif
