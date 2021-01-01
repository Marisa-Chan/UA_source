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

    bool MouseDown(Common::Point pos, int button);
    bool MouseMove(Common::Point pos);
    bool MouseUp(Common::Point pos, int button);

    void RootWidgetToFront(Widget *w);
    void ChangeFocus(Widget *w);
    void StartDragging(Widget *w);
    void StopHoldingMouse();

    void SetScreenSize(Common::Point sz);
    Common::Point GetScreenSize();

    Widget *FindByPos(const Common::Point &pos, bool stopOnModal = false);
    Widget *FindByID(uint32_t id, bool enabled = true);
    
    uint32_t TimerAdd(uint32_t wID, uint32_t time, uint32_t code = Timer::CODE_ANY);
    
    int32_t TimerGet(uint32_t id);
    bool TimerDelete(uint32_t id); // By timer ID
    int TimerDeleteByWidget(uint32_t wID, uint32_t code = Timer::CODE_ANY); // By widget ID
    
    void TimersUpdate(uint32_t dtime);
    
    void HwCompose();
    
protected:
    Root() {};
    ~Root();
    
    Widget *_FindByPos(WidgetList &lst, const Common::Point& pos, bool stopOnModal = false);
    Widget *_FindByID(WidgetList &lst, uint32_t id, bool enabled = true);
    void DrawWidget(SDL_Surface *screen, Common::Rect space, Common::Point parentOffset, Widget *w, uint32_t alph = 255);
        
    WidgetList& GetLayerList(int l);
    
    bool CheckEnable(Widget *w);
    void ValidateWidgets();
    
    void UpdateWidgetOnMice(Widget *w);
    
    int32_t _TimerGet(std::list<Timer> &list, uint32_t id);
    bool _TimerDelete(std::list<Timer> &list, uint32_t id); // By timer ID
    int _TimerDeleteByWidget(std::list<Timer> &list, uint32_t wID, uint32_t code); // By widget ID
    
    void ModAlpha(SDL_Surface *surf, Common::Rect space, uint8_t alpha);
    
    void HwPrepareWidget(Widget *w);
    void HwRenderWidget(Widget *w);
    
    SDL_Surface *CreateScreenFmtSurface(uint32_t w, uint32_t h);

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
    
    /** Timer things **/
    uint32_t       _nextTimerID = 1;
    uint64_t         _timeStamp = 0;
    std::list<Timer>    _timers;
    std::list<Timer> _timersNew;
    bool           _timersToNew = false;
    
    bool              _hwRender = false;
    GLuint               _hwTex = 0;

    Common::Point      _screenSize;

    SDL_Surface       *_dirtSurface = NULL;
};

}

#endif
