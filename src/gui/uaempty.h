#ifndef GUI_UAEMPTY_H_INCLUDED
#define GUI_UAEMPTY_H_INCLUDED

#include "widget.h"

namespace Gui
{


class UAEmpty : public Widget
{
public:
    UAEmpty();
    UAEmpty(Widget *parent);
    UAEmpty(Common::PointRect xyw);
    UAEmpty(Widget *parent, const Common::PointRect &xyw);
    
    virtual void OnAttachDetach(bool attach);
    virtual void OnParentResize(Common::Point newSz);
    
protected:
    void Update();
    
protected:
    bool _filling = false;
};


}



#endif