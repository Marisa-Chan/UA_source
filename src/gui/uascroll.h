#ifndef GUI_UASCROLL_H_INCLUDED
#define GUI_UASCROLL_H_INCLUDED

#include "uacommon.h"

namespace Gui
{
    
class UAScroll : public Widget, public UA
{
protected:
    enum
    {
        AREA_NONE  = 0,
        AREA_MINUS = 1,
        AREA_BTN   = 2,
        AREA_PLUS  = 3,
    };
    
    enum
    {
        TIME_FIRST = 500,
        TIME_NEXT  = 100,
    };
public:
    
    UAScroll(bool verical = true);
    UAScroll(Widget *parent, bool vertical = true);
    ~UAScroll();
    
    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
    virtual void MouseDown(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseUp(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseMove(Common::Point pos, Common::Point scrPos, int button);
    virtual void Resize(Common::Point sz);
    
    virtual void TimerEvent(uint32_t code, uint32_t timerID);
    
    int GetScrollPos()      const { return _scrlPos; };
    int GetScrollCount()    const { return _scrlCount; };
    int GetScrollPageSize() const { return _scrlPageSz; };
    
    void SetScrollPos(int p);
    void SetScrollCount(int c);
    void SetScrollPageSize(int ps);
    
protected:
    
    void StepMinus();
    void StepPlus();
    
    virtual void Check();
    virtual void UpdateAreas();
    
    virtual int GetArea(Common::Point pos);
    
protected:
    const bool _vertical;
    
    int   _scrlCount    = 1;
    int   _scrlPos      = 1;
    int   _scrlPageSz   = 1;
    
    Common::Rect _areaMinus;
    Common::Rect _areaBtn;
    Common::Rect _areaPlus;
    
    Common::Point _btnOff;
    Common::Point _lastPos;
    
    int   _svdPxPos       = 0;
    int   _curPxPos       = 0;
    
    int   _downedArea    = AREA_NONE;
    int   _mouseArea     = AREA_NONE;
};

}


#endif