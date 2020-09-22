#ifndef GUI_WIDGET_H_INCLUDED
#define GUI_WIDGET_H_INCLUDED

#include <list>
#include <inttypes.h>
#include <SDL2/SDL.h>
#include "../common.h"
#include "root.h"

namespace Gui
{
class Root;



class Widget
{
friend class Root;

public:
enum
{
    FLAG_ENABLED    = (1 << 0),
    FLAG_FOCUSED    = (1 << 1),
    FLAG_CLIENT     = (1 << 2), // Client space enabled
    FLAG_PRIVATE    = (1 << 3), // Do not place it in client rect
};

typedef void (*TFOnWidget)(Widget *w, void *);
typedef void (*TFOnPoint)(Widget *w, void *, Common::Point);
typedef void (*TFOnInt)(Widget *w, void *, int);
typedef void (*TFOnBool)(Widget *w, void *, bool);
typedef void (*TFOnMouse)(Widget *w, void *, Common::Point, Common::Point, int);

public:
    Widget();
    Widget(Widget *parent);
    virtual ~Widget();

public:
    bool IsOn(const Common::Point &pos) const; // Screen coords
    bool IsOn(int x, int y) const; // Screen coords
    bool IsLOn(const Common::Point &pos) const; // Local coords
    bool IsLOn(int x, int y) const; // Local coords

    bool IsEnabled() const { return (_flags & FLAG_ENABLED) != 0; };
    bool IsFocused() const { return (_flags & FLAG_FOCUSED) != 0; };

    Widget *FindByPos(const Common::Point &pos);  //Screen coords
    Widget *FindChildLPos(const Common::Point &pos); //Widget local coords
    
    Widget *FindByID(uint32_t id, bool enabled = true);
    
    Widget *GetRoot();
    Common::Point GetPos() const; // In parent coordinates
    Common::Point GetSize() const;
    int GetWidth() const;
    int GetHeight() const;
    Common::Point GetScreenPos() const; // In screen coordinates
    
    Common::Rect  GetSizeRect() const;

    Common::Point GetOnParentPos() const;

    Common::Rect GetScreenVisibleRect() const;
    Common::Rect GetWidgetVisibleRect() const;

    Common::Point ScreenCoordToClient(Common::Point pos) const;
    Common::Point ScreenCoordToWidget(Common::Point pos) const;

    void AddChild(Widget *child, bool top = true);
    void RemoveChild(Widget *child);
    void Unparent();
    
    void SetPrivate(bool mode);
    bool GetPrivate() const;

    virtual void MouseMove(Common::Point pos, Common::Point scrPos, int buttons);
    virtual void MouseDown(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseUp(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseEnter();
    virtual void MouseLeave();
    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt) {};
    virtual void FocusOff() {};
    virtual void FocusOn() {};
    
    virtual void TimerEvent(uint32_t code, uint32_t timerID) {};

    virtual void MoveTo(Common::Point pos);
    virtual void Resize(Common::Point sz);
    
    
    
    void SetAlpha(uint8_t a) 
    { 
        _alpha = a; 
    };
    
    uint8_t GetAlpha() const 
    {
        return _alpha;
    }
    
    
        
    void MoveTo(int x, int y)
    {
        MoveTo(Common::Point(x, y));
    };
    
    void ResizeWH(int w, int h)
    {
        Resize(Common::Point(w, h));
    };

    virtual void SetClientRect(Common::Rect r);
    virtual void SetClientEnable(bool enable);
    
    virtual void SetEnable(bool enable);

    inline bool IsRooted() const { return _rooted != Root::LAYER_UNK; };

public:
    TFOnPoint    _fOnResize        = NULL;
    void *       _fOnResizeData    = NULL;
    TFOnPoint    _fOnMove          = NULL;
    void *       _fOnMoveData      = NULL;
    TFOnMouse    _fOnMouseMove     = NULL;
    void *       _fOnMouseMoveData = NULL;
    TFOnMouse    _fOnMouseDown     = NULL;
    void *       _fOnMouseDownData = NULL;
    TFOnMouse    _fOnMouseUp       = NULL;
    void *       _fOnMouseUpData   = NULL;
    TFOnWidget   _fOnMouseEnter    = NULL;
    void *       _fOnMouseEnterData= NULL;
    TFOnWidget   _fOnMouseLeave    = NULL;
    void *       _fOnMouseLeaveData= NULL;
    
    TFOnBool     _fOnHideShow      = NULL; // Only for rooted widgets
    void *       _fOnHideShowData  = NULL; 
    
    TFOnPoint    _fOnParentResize  = NULL;
    void *       _fOnParentResizeData = NULL;
    

protected:
    const uint32_t         _id;
    Common::Rect         _rect;
    Common::Rect       _client;
    WidgetList         _childs;

    uint32_t              _tag = 0;
    bool                _mOver = false;
    uint32_t            _flags = 0;
    Widget            *_parent = NULL;
    uint8_t            _alpha  = 255;
    
    // Used only with HW compositing and if it's "paranted" to root
    SDL_Surface    *_hwSurface = NULL;

private:
    int    _rooted = Root::LAYER_UNK;
};

}

#endif

