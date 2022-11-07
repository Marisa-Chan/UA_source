#ifndef COMMONLINE_H_INCLUDED
#define COMMONLINE_H_INCLUDED

#include "common.h"

namespace Common
{

template <typename T> struct TLine;

typedef TLine<int32_t> Line;
typedef TLine<float> FLine;

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

}

#endif