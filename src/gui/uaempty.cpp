#include "uaempty.h"

namespace Gui
{
    
enum {
    UAEMPTY_FLAGS = Widget::FLAG_NODRAW | Widget::FLAG_ENABLED | Widget::FLAG_NOBRING,
};
    
UAEmpty::UAEmpty()
: Widget()
{
    _filling = true;
    _flags = UAEMPTY_FLAGS;
    Update();
}

UAEmpty::UAEmpty(Widget *parent)
: Widget(parent)
{
    _filling = true;
    _flags = UAEMPTY_FLAGS;
    Update();
}

UAEmpty::UAEmpty(Common::PointRect xyw)
: Widget()
{
    _filling = false;
    _rect = xyw;
    _flags = UAEMPTY_FLAGS;
}

UAEmpty::UAEmpty(Widget *parent, const Common::PointRect &xyw)
: Widget(parent)
{
    _filling = false;
    _rect = xyw;
    _flags = UAEMPTY_FLAGS;
}

void UAEmpty::OnAttachDetach(bool attach)
{
    Update();
    Widget::OnAttachDetach(attach);
}

void UAEmpty::OnParentResize(Common::Point newSz)
{
    Update();
    Widget::OnParentResize(newSz);
}


void UAEmpty::Update()
{
    if (_filling)
    {
        Common::Point parentSz = GetSpace();
        if (!parentSz)
            Resize(parentSz);
    }
}
}