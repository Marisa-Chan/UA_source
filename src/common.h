#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <vector>

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

	Point Invert() { return Point( -x, -y ); }
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
		return (left <= x) && (x < right) && (top <= y) && (y < bottom);
	}
	bool IsIn(const Point &p) const {
		return IsIn(p.x, p.y);
	}
	bool IsIn(const Rect &r) const {
		return (left <= r.left) && (r.right <= right) && (top <= r.top) && (r.bottom <= bottom);
	}

	bool IsIntersects(const Rect &r) const {
		return (left < r.right) && (r.left < right) && (top < r.bottom) && (r.top < bottom);
	}

	Rect IntersectionRect(const Rect &r) const {
		if (!IsIntersects(r))
			return Rect();

		return Rect(MAX(r.left, left), MAX(r.top, top), MIN(r.right, right), MIN(r.bottom, bottom));
	}

	void Clip(const Rect &r) {
		if (top < r.top) top = r.top;
		else if (top > r.bottom) top = r.bottom;

		if (left < r.left) left = r.left;
		else if (left > r.right) left = r.right;

		if (bottom > r.bottom) bottom = r.bottom;
		else if (bottom < r.top) bottom = r.top;

		if (right > r.right) right = r.right;
		else if (right < r.left) right = r.left;
	}

	void Clip(int maxw, int maxh) {
		Clip(Rect(0, 0, maxw, maxh));
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

	operator bool()
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
    
    PlaneVector<T> *Copy()
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

typedef Common::PlaneVector<uint8_t> PlaneBytes;
     


}

#endif
