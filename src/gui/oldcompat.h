#ifndef GUI_OLDCOMPAT_H_INCLUDED
#define GUI_OLDCOMPAT_H_INCLUDED

#include "widget.h"

namespace Gui
{


class OldCompat : public Widget
{
public:
    OldCompat(const Common::Point &sz);
    
    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
    virtual void MouseDown(Common::Point pos, Common::Point scrPos, int button, int clkNum = 1);
    virtual void MouseUp(Common::Point pos, Common::Point scrPos, int button, int clkNum = 1);
    virtual void MouseMove(Common::Point pos, Common::Point scrPos, Common::Point relMove, int buttons);
    
    virtual void OnParentResize(Common::Point newSz);

    void Resize(const Common::Point &sz);
    
    SDL_Surface *GetSurface();
};


}


#endif