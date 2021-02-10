#include "oldcompat.h"
#include "../system/inpt.h"
#include "../winp.h"

namespace Gui
{

OldCompat::OldCompat(const Common::Point &sz)
{
    _flags |= FLAG_ENABLED | FLAG_NOBRING;
    _rect.SetSize(sz);
    _hwSurface = Root::CreateScreenFmtSurface(sz.x, sz.y);
}

void OldCompat::Resize(const Common::Point &sz)
{
    if(_hwSurface)
        SDL_FreeSurface(_hwSurface);
    
    _hwSurface = Root::CreateScreenFmtSurface(sz.x, sz.y);
    
    _rect.SetSize(sz);
}

SDL_Surface *OldCompat::GetSurface()
{
    return _hwSurface;
}

void OldCompat::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{
    if (surface != _hwSurface)
        SDL_BlitSurface(_hwSurface, NULL, surface, NULL);
}

void OldCompat::OnParentResize(Common::Point newSz)
{
    Resize(newSz);
}
    
void OldCompat::MouseDown(Common::Point pos, Common::Point scrPos, int button, int clkNum)
{
    NC_STACK_winp::OnMouseDown(pos, button, clkNum);
}

void OldCompat::MouseUp(Common::Point pos, Common::Point scrPos, int button, int clkNum)
{
    NC_STACK_winp::OnMouseUp(pos, button, clkNum);
}

void OldCompat::MouseMove(Common::Point pos, Common::Point scrPos, Common::Point relMove, int buttons)
{
    NC_STACK_winp::OnMouseMove(pos, relMove);
}

}