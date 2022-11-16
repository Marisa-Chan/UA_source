#ifndef PLANE_H_INCLUDED
#define PLANE_H_INCLUDED

#include "common.h"


namespace Common
{

template <class T> class PlaneVector;

typedef PlaneVector<uint8_t> PlaneBytes;

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
    using std::vector<T>::empty;
    
    PlaneVector() = default;
    PlaneVector( PlaneVector && ) = default;
    PlaneVector( const PlaneVector & ) = default;
    
    PlaneVector& operator=( const PlaneVector & ) = default;
    PlaneVector& operator=( PlaneVector && ) = default;
    
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
    
    const T* Line(uint32_t y) const
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

    void Clear()
    {
        _w = 0;
        _h = 0;
        std::vector<T>::clear();
    }
    
    bool IsNotNull() const
    {
        return _w > 0 && _h > 0;
    }
    
    bool IsNull() const
    {
        return _w == 0 || _h == 0;
    }
    
    bool IsOk() const
    {
        return _w > 0 && _h > 0 && (size() == _w * _h);
    }
    
protected:
    uint32_t _w = 0;
    uint32_t _h = 0;
};


template <class T, std::size_t XW, std::size_t XH>
struct PlaneArray : protected std::array<T, (XW * XH)>
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
    using _BaseType::fill;

    PlaneArray() = default;
    
    PlaneArray(T val)
    {
        fill(val);
    }

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
    
    const T* Line(uint32_t y) const
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
    
    static constexpr PlaneArray ArrayInit(const T& val)
    {
            PlaneArray t;
            t.fill(val);
            return t;
    }
};

}

#endif