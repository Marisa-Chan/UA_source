#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <inttypes.h>
#include <vector>
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
template<typename T> inline T MIN(T a, T b)	{ return (a < b) ? a : b; };
template<typename T> inline T MAX(T a, T b)	{ return (a > b) ? a : b; };

struct Point
{
    int x;
    int y;

    Point() : x(0), y(0) {};
    Point(int x1, int y1) : x(x1), y(y1) {};

    bool  operator==(const Point &p)    const { return x == p.x && y == p.y; }
    bool  operator!=(const Point &p)    const { return x != p.x || y != p.y; }
    Point operator+(const Point &delta) const { return Point(x + delta.x, y + delta.y); }
    Point operator-(const Point &delta) const { return Point(x - delta.x, y - delta.y); }

    void operator+=(const Point &delta) {
            x += delta.x;
            y += delta.y;
    }

    void operator-=(const Point &delta) {
            x -= delta.x;
            y -= delta.y;
    }

    Point Invert() const { return Point( -x, -y ); }
    
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

struct Rect
{
    int left;
    int top;
    int right;
    int bottom;

    Rect() : left(0), top(0), right(0), bottom(0) {};
    Rect(int w, int h) : left(0), top(0), right(w), bottom(h) {};
    Rect(Point p) : left(0), top(0), right(p.x), bottom(p.y) {};
    Rect(int x1, int y1, int x2, int y2) : left(x1), top(y1), right(x2), bottom(y2) {};

    bool IsValid() const {
            return (left <= right && top <= bottom);
    }

    bool IsEmpty() const {
            return (left >= right || top >= bottom);
    }

    int Width() const { return right - left; }
    int Height() const { return bottom - top; }

    void SetWidth(int Width) {
            right = left + Width;
    }

    void SetHeight(int Height) {
            bottom = top + Height;
    }

    void SetSize(Point sz) {
            right = left + sz.x;
            bottom = top + sz.y;
    }

    bool IsIn(int x, int y) const {
            return (x >= left) && (x < right) && (y >= top) && (y < bottom);
    }
    bool IsIn(const Point &p) const {
            return IsIn(p.x, p.y);
    }
    bool IsIn(const Rect &r) const {
            return (left <= r.left) && (right >= r.right) && (top <= r.top) && (bottom >= r.bottom);
    }

    bool IsIntersects(const Rect &r) const {
            return (left < r.right) && (r.left < right) && (top < r.bottom) && (r.top < bottom);
    }

    Rect IntersectionRect(const Rect &r) const {
            if (!IsIntersects(r))
                    return Rect();

            return Rect(MAX(r.left, left), MAX(r.top, top), MIN(r.right, right), MIN(r.bottom, bottom));
    }

    void ClipBy(const Rect &r) {
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
            ClipBy(Rect(0, 0, maxw, maxh));
    }

    void MoveTo(int x, int y) {
            bottom += y - top;
            right += x - left;
            top = y;
            left = x;
    }

    void MoveTo(const Point &p) {
            MoveTo(p.x, p.y);
    }

    void Translate(int dx, int dy) {
            left += dx; right += dx;
            top += dy; bottom += dy;
    }

    void Translate(const Point &p) {
            left += p.x; right += p.x;
            top += p.y; bottom += p.y;
    }

    operator bool() const
    {
        return !IsEmpty();
    }

    Point Pos() const
    {
        return Point(left, top);
    }

    Point Size() const
    {
        return Point(Width(), Height());
    }
    
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

struct PointRect
{
    int x;
    int y;
    int w;
    int h;

    PointRect() : x(0), y(0), w(0), h(0) {};
    PointRect(int x1, int y1) : x(x1), y(y1), w(0), h(0) {};
    PointRect(Point p) : x(p.x), y(p.y), w(0), h(0) {};
    PointRect(Point p, int w_, int h_) : x(p.x), y(p.y), w(w_), h(h_) {};
    PointRect(int x_, int y_, int w_, int h_) : x(x_), y(y_), w(w_), h(h_) {};
    
    int Right() const { return x + w; }
    int Bottom() const { return y + h; }
    
    bool  operator==(const PointRect &p)    const { return x == p.x && y == p.y && w == p.w && h == p.h; }
    bool  operator!=(const PointRect &p)    const { return x != p.x || y != p.y || w != p.w || h != p.h; }
    PointRect operator+(const Point &delta) const { return PointRect(x + delta.x, y + delta.y, w, h); }
    PointRect operator-(const Point &delta) const { return PointRect(x - delta.x, y - delta.y, w, h); }

    void operator+=(const Point &delta) {
            x += delta.x;
            y += delta.y;
    }

    void operator-=(const Point &delta) {
            x -= delta.x;
            y -= delta.y;
    }
    
    void MoveTo(int px, int py) {
            x = px;
            y = py;
    }

    void MoveTo(const Point &p) {
            MoveTo(p.x, p.y);
    }

    void Translate(int dx, int dy) {
            x += dx;
            y += dy;
    }

    void Translate(const Point &p) {
            x += p.x;
            y += p.y;
    }
    
    bool IsIn(int px, int py) const {
            return (px >= x) && (px < Right()) && (py >= y) && (py < Bottom());
    }
    bool IsIn(const Point &p) const {
            return IsIn(p.x, p.y);
    }
    bool IsIn(const Rect &r) const {
            return (r.left >= x) && (r.right <= Right()) && (r.top >= y) && (r.bottom <= Bottom());
    }
    bool IsIn(const PointRect &r) const {
            return (r.x >= x) && (r.Right() <= Right()) && (r.y >= y) && (r.Bottom() <= Bottom());
    }

    bool IsIntersects(const Rect &r) const {
            return (x < r.right) && (Right() > r.left) && (y < r.bottom) && (Bottom() > r.top);
    }
    
    bool IsIntersects(const PointRect &r) const {
            return (x < r.Right()) && (Right() > r.x) && (y < r.Bottom()) && (Bottom() > r.y);
    }
   
    void ClipBy(const Rect &r) {
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
            ClipBy(Rect(0, 0, maxw, maxh));
    }
    
    void ClipBy(const PointRect &r) {
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
    
    Point Pos() const
    {
        return Point(x, y);
    }

    Point Size() const
    {
        return Point(w, h);
    }
    
    operator bool() const
    {
        return !IsEmpty();
    }

    operator Point() const
    {
        return Point(x, y);
    }
    
    operator Rect() const
    {
        return Rect(x, y, x + w, y + h);
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

template <class T>
class PlaneVector : public std::vector<T>
{
public:
    PlaneVector()
    {}
    
    PlaneVector(const PlaneVector &b)
    {
        operator=(b);
    }
    
    PlaneVector(uint32_t w, uint32_t h)
    {
        Resize(w, h);
    }
    
    PlaneVector<T>* Copy()
    {
        PlaneVector<T> *tmp = new PlaneVector<T>;
        *tmp = *this;
        return tmp;
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
    
    T& operator()(uint32_t x, uint32_t y)
    {
        return this->at(x + y * _w);
    }
    
    T* Line(uint32_t y)
    {
        return &(this->at(y * _w));
    }
    
    Point Size() const
    {
        return Point(_w, _h);
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
    
protected:
    uint32_t _w;
    uint32_t _h;
};

typedef PlaneVector<uint8_t> PlaneBytes;
     

/**
 * Clip line by clip rect
 * Result - is part of line in clip rect
 * line parameter will be modified
 **/
bool ClipLine(const Rect &clip, Rect *line);

}

#endif
