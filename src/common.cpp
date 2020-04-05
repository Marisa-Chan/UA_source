#include "common.h"

namespace Common
{
    
enum CLIP_FLAGS
{
    CLIP_LEFT   = 1,
    CLIP_RIGHT  = 2,
    CLIP_TOP    = 4,
    CLIP_BOTTOM = 8
};

int ClipFlags(int x, int y, const Rect &clip)
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

bool ClipLine(const Rect &clip, Rect *line)
{
    int flag1 = ClipFlags(line->left,   line->top, clip);
    int flag2 = ClipFlags(line->right,  line->bottom, clip);

    Rect tmp = *line;

    while ( (flag2 | flag1) != 0 )
    {
        if ( (flag2 & flag1) != 0 )
            return false;

        int flag;

        if ( flag1 )
            flag = flag1;
        else
            flag = flag2;

        int dy = tmp.Height();
        int dx = tmp.Width();

        int vX = 0, vY = 0;

        if ( flag & CLIP_TOP )
        {
            vY = clip.top;
            vX = dx * (clip.top - tmp.top) / dy + tmp.left;
        }
        else if ( flag & CLIP_BOTTOM )
        {
            vY = clip.bottom;
            vX = dx * (clip.bottom - tmp.top) / dy + tmp.left;
        }
        else if ( flag & CLIP_LEFT )
        {
            vX = clip.left;
            vY = dy * (clip.left - tmp.left) / (tmp.right - tmp.left) + tmp.top;
        }
        else if ( flag & CLIP_RIGHT )
        {
            vX = clip.right;
            vY = dy * (clip.right - tmp.left) / (tmp.right - tmp.left) + tmp.top;
        }

        if ( flag == flag1 )
        {
            tmp.top     = vY;
            tmp.left    = vX;
        }
        else
        {
            tmp.bottom  = vY;
            tmp.right   = vX;
        }

        int newFlag = 0;

        if ( vY > clip.bottom )
            newFlag = 8;
        else if ( vY < clip.top )
            newFlag = 4;

        if ( vX > clip.right )
            newFlag |= 2;
        else if ( vX < clip.left )
            newFlag |= 1;

        if ( flag == flag1 )
            flag1 = newFlag;
        else
            flag2 = newFlag;
    }
    
    *line = tmp;
    
    return true;
}


Rect::operator PointRect() const
{
    return PointRect(left, top, Width(), Height());
}

}
