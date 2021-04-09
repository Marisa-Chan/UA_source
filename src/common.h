#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <inttypes.h>
#include <vector>
#include <array>
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
template <typename T> struct TLine;

typedef TPoint<int> Point;
typedef TRect<int> Rect;
typedef TPointRect<int> PointRect;
typedef TPoint<float> FPoint;
typedef TRect<float> FRect;

typedef TLine<int> Line;
typedef TLine<float> FLine;

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

    __Point Invert() const { return TPoint( -x, -y ); }
    
    operator bool() const
    {
        return x == 0 && y == 0;
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

    T Width() const { return right - left; }
    T Height() const { return bottom - top; }

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


template<typename T>
struct TLine
{
    enum CLIP_FLAGS
    {
        CLIP_LEFT   = 1,
        CLIP_RIGHT  = 2,
        CLIP_TOP    = 4,
        CLIP_BOTTOM = 8
    };

    T x1;
    T y1;
    T x2;
    T y2;
    
    TLine() : x1(0), y1(0), x2(0), y2(0) {};
    TLine(T x, T y) : x1(0), y1(0), x2(x), y2(y) {};
    TLine(TPoint<T> p) : x1(0), y1(0), x2(p.x), y2(p.y) {};
    TLine(TPoint<T> p, T x, T y) : x1(p.x), y1(p.y), x2(x), y2(y) {};
    TLine(T x, T y, TPoint<T> p) : x1(x), y1(y), x2(p.x), y2(p.y) {};
    TLine(TPoint<T> p1, TPoint<T> p2) : x1(p1.x), y1(p1.y), x2(p2.x), y2(p2.y) {};
    TLine(T ax, T ay, T bx, T by) : x1(ax), y1(ay), x2(bx), y2(by) {};
    
    double Length() const
    {
        T w = x2 - x1;
        T h = y2 - y1;
        return sqrt( w * w + h * h );
    }
    
    T Width() const
    {
        return ABS(x2 - x1);
    }
    
    T Height() const
    {
        return ABS(y2 - y1);
    }
    
    TPoint<T> P1() const
    {
        return TPoint<T>(x1, y1);
    }
    
    TPoint<T> P2() const
    {
        return TPoint<T>(x2, y2);
    }
    
    int ClipFlags(T x, T y, const TRect<T> &clip) const
    {
        int flag = 0;

        if ( y > clip.bottom )
            flag |= CLIP_BOTTOM;
        else if ( y < clip.top )
            flag |= CLIP_TOP;

        if ( x > clip.right )
            flag |= CLIP_RIGHT;
        else if ( x < clip.left )
            flag |= CLIP_LEFT;

        return flag;
    }
    
    /*
     * Clip line by rectangle.
     * Return true if line intersects with rect.
     */
    bool ClipBy(const TRect<T> &clip)
    {
        int flag1 = ClipFlags(x1,  y1, clip);
        int flag2 = ClipFlags(x2,  y2, clip);

        while ( (flag2 | flag1) != 0 )
        {
            /***
             * If flags for p1 and p2 intersects - it's mean
             * that line is not intersects with rect
             * and two points lay outside one side of clip rect.
             ***/                
            if ( (flag2 & flag1) != 0 )
                return false;

            int flag;

            if ( flag1 )
                flag = flag1;
            else
                flag = flag2;

            T dy = y2 - y1;
            T dx = x2 - x1;

            T vX = 0, vY = 0;

            if ( flag & CLIP_TOP )
            {
                vY = clip.top;
                vX = dx * (clip.top - y1) / dy + x1;
            }
            else if ( flag & CLIP_BOTTOM )
            {
                vY = clip.bottom;
                vX = dx * (clip.bottom - y1) / dy + x1;
            }
            else if ( flag & CLIP_LEFT )
            {
                vX = clip.left;
                vY = dy * (clip.left - x1) / (x2 - x1) + y1;
            }
            else if ( flag & CLIP_RIGHT )
            {
                vX = clip.right;
                vY = dy * (clip.right - x1) / (x2 - x1) + y1;
            }

            if ( flag == flag1 )
            {
                y1 = vY;
                x1 = vX;
            }
            else
            {
                y2 = vY;
                x2 = vX;
            }

            int newFlag = ClipFlags(vX, vY, clip);

            if ( flag == flag1 )
                flag1 = newFlag;
            else
                flag2 = newFlag;
        }

        return true;
    }
};



template <class T>
class PlaneVector : protected std::vector<T>
{
public:
    using std::vector<T>::data;
    using std::vector<T>::operator[];
    using std::vector<T>::size;
    using std::vector<T>::begin;
    using std::vector<T>::end;
    using std::vector<T>::rbegin;
    using std::vector<T>::rend;
    
    PlaneVector()
    {}
    
    PlaneVector(const PlaneVector &b)
    {
        operator=(b);
    }
    
    PlaneVector(PlaneVector &&b)
    : std::vector<T>(std::move(b))
    {
        _w = b._w;
        _h = b._h;
    }
    
    PlaneVector(uint32_t w, uint32_t h)
    {
        Resize(w, h);
    }
    
    PlaneVector(const Point &sz)
    {
        Resize(sz);
    }
    
    PlaneVector<T>* Copy()
    {
        PlaneVector<T> *tmp = new PlaneVector<T>;
        *tmp = *this;
        return tmp;
    }
    
    void Resize(const Point &sz)
    {
        _w = sz.x;
        _h = sz.y;
        this->resize(_w * _h);
    }
    
    void Resize(uint32_t w, uint32_t h)
    {
        _w = w;
        _h = h;
        this->resize(w * h);
    }
    
    T& At(uint32_t x, uint32_t y)
    {
        return this->at(x + y * _w);
    }
    
    T& At(const Point &p)
    {
        return this->at(p.x + p.y * _w);
    }
    
    T& At(size_t n)
    {
        return this->at(n);
    }
    
    T& operator()(uint32_t x, uint32_t y)
    {
        return this->at(x + y * _w);
    }
    
    T& operator()(const Point &p)
    {
        return this->at(p.x + p.y * _w);
    }
    
    T& operator()(size_t n)
    {
        return this->at(n);
    }
    
    T* Line(uint32_t y)
    {
        return &(this->at(y * _w));
    }
    
    Point Size() const
    {
        return Point(_w, _h);
    }
    
    size_t GetID(uint32_t x, uint32_t y) const
    {
        size_t n = x + y * _w;
        if (n < size())
            return n;
        return -1;
    }
    
    Point GetPos(size_t id) const
    {
        if (id >= 0 && id < size())
            return Point(id % _w, id / _w);
        return Point(-1, -1);
    }
    
    uint32_t Width() const
    {
        return _w;
    }
    
    uint32_t Height() const
    {
        return _h;
    }
    
    void operator=(const PlaneVector<T> &b)
    {
        std::vector<T>::operator=(b);
        _w = b._w;
        _h = b._h;
    }
    
    void Clear()
    {
        _w = 0;
        _h = 0;
        std::vector<T>::clear();
    }
    
protected:
    uint32_t _w;
    uint32_t _h;
};

typedef PlaneVector<uint8_t> PlaneBytes;

template <class T, std::size_t XW, std::size_t XH>
class PlaneArray : protected std::array<T, (XW * XH)>
{
protected:
    typedef std::array<T, (XW * XH)> _BaseType;
public:
    using _BaseType::data;
    using _BaseType::operator[];
    using _BaseType::size;
    using _BaseType::begin;
    using _BaseType::end;
    using _BaseType::rbegin;
    using _BaseType::rend;
    
    PlaneArray()
    {}
    
    PlaneArray(T val)
    {
        Clear(val);
    }
    
    PlaneArray(const PlaneArray &b)
    {
        operator=(b);
    }
    
    PlaneArray(PlaneArray &&b)
    : _BaseType(std::move(b))
    {}
    
    PlaneArray<T, XW, XH>* Copy()
    {
        PlaneArray<T, XW, XH> *tmp = new PlaneArray<T, XW, XH>;
        *tmp = *this;
        return tmp;
    }
    
    T& At(uint32_t x, uint32_t y)
    {
        return this->at(x + y * XW);
    }
    
    T& At(const Point &p)
    {
        return this->at(p.x + p.y * XW);
    }
    
    T& At(size_t n)
    {
        return this->at(n);
    }
    
    T& operator()(uint32_t x, uint32_t y)
    {
        return this->at(x + y * XW);
    }
    
    T& operator()(const Point &p)
    {
        return this->at(p.x + p.y * XW);
    }
    
    T& operator()(size_t n)
    {
        return this->at(n);
    }
    
    T* Line(uint32_t y)
    {
        return &(this->at(y * XW));
    }
    
    T Get(uint32_t x, uint32_t y) const
    {
        return this->at(x + y * XW);
    }
    
    T At(const Point &p) const
    {
        return this->at(p.x + p.y * XW);
    }
    
    T At(size_t n) const
    {
        return this->at(n);
    }
    
    Point Size() const
    {
        return Point(XW, XH);
    }
    
    size_t GetID(uint32_t x, uint32_t y) const
    {
        size_t n = x + y * XW;
        if (n < size())
            return n;
        return -1;
    }
    
    Point GetPos(size_t id) const
    {
        if (id >= 0 && id < size())
            return Point(id % XW, id / XW);
        return Point(-1, -1);
    }
    
    uint32_t Width() const
    {
        return XW;
    }
    
    uint32_t Height() const
    {
        return XH;
    }
    
    void operator=(const PlaneArray<T, XW, XH> &b)
    {
        _BaseType::operator=(b);
    }
    
    void Clear(T val)
    {
        for (T &a : *this)
            a = val;
    }
};

class BitMan : protected std::vector<uint32_t>
{
protected:
    typedef std::vector<uint32_t> BASE;
public:
    BitMan() {};
    BitMan(uint32_t sz) : BASE::vector((sz + 31) / 32) {};
    BitMan(const BitMan & b) : BASE::vector(b) {};
    BitMan(BitMan && b) : BASE::vector(b) {};
    
    BitMan(std::initializer_list<uint32_t> bits);
    
    using BASE::operator=;
    using BASE::at;
    using BASE::clear;
    
    uint32_t GetSize() const;
    void Resize(uint32_t sz);
    
    operator bool() const;
    
    bool Is(uint32_t bit) const;
    bool Is(std::initializer_list<uint32_t> bits) const;
    bool Is(const BitMan &bits) const;
    
    bool IsAny(std::initializer_list<uint32_t> bits) const;
    bool IsAny(const BitMan &bits) const;
    
    void Set(uint32_t bit);    
    void Set(std::initializer_list<uint32_t> bits);
    void Set(const BitMan &bits);
    
    void UnSet(uint32_t bit);
    void UnSet(std::initializer_list<uint32_t> bits);
    void UnSet(const BitMan &bits);
    
    void XSet(uint32_t bit);
    void XSet(std::initializer_list<uint32_t> bits);
    void XSet(const BitMan &bits);
    
    BitMan Not() const;
    
    BitMan And(uint32_t bit) const;
    BitMan And(std::initializer_list<uint32_t> bits) const;
    BitMan And(const BitMan &bits) const;
    
    BitMan Or(uint32_t bit) const;
    BitMan Or(std::initializer_list<uint32_t> bits) const;
    BitMan Or(const BitMan &bits) const;
    
    BitMan Xor(uint32_t bit) const;    
    BitMan Xor(std::initializer_list<uint32_t> bits) const;
    BitMan Xor(const BitMan &bits) const;
};

}

#endif
