#ifndef GUI_ROOT_H_INCLUDED
#define GUI_ROOT_H_INCLUDED

#include <list>
#include <inttypes.h>
#include <SDL2/SDL.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "../common.h"

namespace Gui
{
class Widget;

enum MICE_BUTTONS
{
    MICE_LEFT   = (1 << 0),
    MICE_RIGHT  = (1 << 1),
    MICE_MIDDLE = (1 << 2),
};

class WidgetList: public std::list<Widget *>
{
public:
    iterator find(Widget *w);
    bool MoveToFirst(Widget *w);
};

struct ViewPortal
{
    bool          Used = false;
    Common::Point Size; //Internal resolution
    Common::Rect  Portal;
    WidgetList    Widgets;
    
    // Only used with software rendering
    SDL_Surface  *SoftSurface = NULL;
    
    Common::Point GetPortalPos(Common::Point pos);
    
    ~ViewPortal();
};

struct StreamTex
{
    Common::Point Size;
    std::array<uint32_t, 2> Texs {{0}};
    int32_t nextTex = 0;
    
    StreamTex();
    ~StreamTex();
    void Stream(Common::Point sz, int32_t fmt, int32_t type, const void *data);
};

struct Timer
{
    enum
    {
        CODE_ANY = 0,
    };
    
    uint32_t ID;
    uint64_t  EndTime;
    uint32_t WidgetID;
    uint32_t Code;
    
    Timer()
    : ID(0)
    , EndTime(0)
    , WidgetID(0)
    , Code(CODE_ANY)
    {}
    
    Timer(uint32_t id, uint32_t time, uint32_t wid, uint32_t cod = CODE_ANY)
    : ID(id)
    , EndTime(time)
    , WidgetID(wid)
    , Code(cod)
    {}
};


class Root
{
public:
    enum
    {
        LAYER_UNK    = 0,
        LAYER_NORMAL = 1,
        LAYER_FRONT  = 2,
        LAYER_BACK   = 3,
        LAYER_PORTAL = 4,
    };
public:
    uint32_t GetNextId();
    
    void SetHwCompose(bool hw)
    {
        _hwRender = hw;
    }

    void AddWidget(Widget *w, bool top = true);
    void RemoveWidget(Widget *w);
    void Draw(SDL_Surface *screen);

    bool MouseDown(Common::Point pos, int button, int clkNum = 1);
    bool MouseMove(Common::Point pos, Common::Point relMove);
    bool MouseUp(Common::Point pos, int button, int clkNum = 1);

    void RootWidgetToFront(Widget *w);
    void ChangeFocus(Widget *w);
    void StartDragging(Widget *w);
    void StopHoldingMouse();

    void SetScreenSize(Common::Point sz);
    Common::Point GetScreenSize(int32_t portalID = -1);

    Widget *FindByPos(const Common::Point &pos);
    
    Widget *FindByMouse(const Common::Point &pos);
    
    Widget *FindByID(uint32_t id, bool enabled = true);
    
    uint32_t TimerAdd(uint32_t wID, uint32_t time, uint32_t code = Timer::CODE_ANY);
    
    int32_t TimerGet(uint32_t id);
    bool TimerDelete(uint32_t id); // By timer ID
    int TimerDeleteByWidget(uint32_t wID, uint32_t code = Timer::CODE_ANY); // By widget ID
    
    void TimersUpdate(uint32_t dtime);
    
    void HwCompose();
    
    static SDL_Surface *CreateScreenFmtSurface(uint32_t w, uint32_t h);
    static void ModAlpha(SDL_Surface *surf, Common::Rect space, uint8_t alpha);

    // Portals
    int32_t AddPortal(Common::Point size, Common::Rect portal);
    bool DeletePortal(int32_t id);
    bool ResizePortal(int32_t id, Common::Point size);
    bool SetPortal(int32_t id, Common::Rect portal);
    void AddWidgetPortal(int32_t id, Widget *w, bool top = true);
    
protected:
    Root() {};
    ~Root();
    
    Widget *_FindByPos(WidgetList &lst, const Common::Point& pos);
    Widget *_FindByMouse(WidgetList &lst, const Common::Point& pos);
    Widget *_FindByID(WidgetList &lst, uint32_t id, bool enabled = true);
    void DrawWidget(SDL_Surface *screen, Common::Rect space, Common::Point parentOffset, Widget *w, uint32_t alph = 255);
    
    void DrawPortal(SDL_Surface *screen, ViewPortal &p);
        
    WidgetList& GetLayerList(Widget *w);
    
    bool CheckEnable(Widget *w);
    void ValidateWidgets();
    
    void UpdateWidgetOnMice(Widget *w);
    
    int32_t _TimerGet(std::list<Timer> &list, uint32_t id);
    bool _TimerDelete(std::list<Timer> &list, uint32_t id); // By timer ID
    int _TimerDeleteByWidget(std::list<Timer> &list, uint32_t wID, uint32_t code); // By widget ID

    void HwPrepareWidget(Widget *w);
    void HwRenderWidget(Widget *w);
    
    /***
     * Check if this widget is in portal
     * When convert position into portal coordinate
     ***/
    Common::Point CorrectPosForWidget(Widget *w, Common::Point pos);

public:
    static Root Instance;
protected:
            
    uint32_t            _nextId = 1;
    WidgetList          _normal;
    WidgetList      _foreground;
    WidgetList      _background;
    WidgetList          _modals;
    Widget*            _focused = NULL;
    Widget*             _miceOn = NULL;
    Widget*           _dragging = NULL;
    Common::Point      _dragPos;
    Common::Point      _micePos;
    int                _buttons = 0;
    
    std::vector<ViewPortal> _portals;
    
    /** Timer things **/
    uint32_t       _nextTimerID = 1;
    uint64_t         _timeStamp = 0;
    std::list<Timer>    _timers;
    std::list<Timer> _timersNew;
    bool           _timersToNew = false;
    
    bool              _hwRender = false;

    Common::Point      _screenSize;

    SDL_Surface       *_dirtSurface = NULL;
};

static constexpr Root &Instance = Root::Instance;

}

#endif
