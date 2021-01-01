#ifndef GUI_UAWINDOW_H_INCLUDED
#define GUI_UAWINDOW_H_INCLUDED

#include "uacommon.h"
#include "uabutton.h"
#include "uascroll.h"

namespace Gui
{
    
class UAWindow : public Widget, public UA
{
public:
    enum
    {
        FLAG_WND_CLOSE   = (1 << 0),
        FLAG_WND_HELP    = (1 << 1),
        FLAG_WND_MAXM    = (1 << 2),
        FLAG_WND_VSCROLL = (1 << 3),
        FLAG_WND_HSCROLL = (1 << 4),
        
        FLAG_WND_RESIZEABLE   = (1 << 8),
        FLAG_WND_UNMOVE       = (1 << 9),
    };
public:
    
    UAWindow(const std::string &title, Common::PointRect posRect, uint32_t flags = FLAG_WND_CLOSE | FLAG_WND_RESIZEABLE);
    UAWindow(Widget *parent, const std::string &title, Common::PointRect posRect, uint32_t flags = FLAG_WND_CLOSE | FLAG_WND_RESIZEABLE);
    ~UAWindow();
    
    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
    virtual void MouseDown(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseUp(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseMove(Common::Point pos, Common::Point scrPos, int button);
    virtual void Resize(Common::Point sz);    

protected:
    static void OnCloseClick(Widget *w, void *); // For button
    static void OnHelpClick(Widget *w, void *);  // For button
    
    static void OnBtnDownSound(Widget *w, void *, Common::Point, Common::Point, int);

    virtual void Update();
    
    
    void Init(const std::string &title, Common::PointRect posRect, uint32_t flags);
    
public:
    TFOnWidget   _fOnHelpClick          = NULL;
    void *       _fOnHelpClickData      = NULL;
    
public:
    std::string   _title;
    uint32_t      _flagsWindow  = 0;
    bool          _inResizing = false;
    Common::Point _rszPnt;
    
    
    UAOneTileButton *_helpBtn   = NULL;
    UAOneTileButton *_closeBtn  = NULL;
    UAOneTileButton *_maxmBtn   = NULL;
    
    UAScroll *_vScroll  = NULL;
    UAScroll *_hScroll  = NULL;
    
    Common::Rect _titleRect;
    
    Common::Rect _areaTitle;
    Common::Rect _areaResize;

    
protected:
    size_t  _TSET_U = TILESET_DEFAULT; // up
    size_t  _TSET_M = TILESET_DEFAULT; // mid
    size_t  _TSET_D = TILESET_DEFAULT; // down
    uint8_t _GLYPH_LU = 'u'; // Left-up
    uint8_t _GLYPH_MU = 'v'; // Mid-up
    uint8_t _GLYPH_RU = 'w'; // Right-up
    uint8_t _GLYPH_LM = '{'; // Left-mid
    uint8_t _GLYPH_MM = ' '; // Mid-mid
    uint8_t _GLYPH_RM = '}'; // Right-mid
    uint8_t _GLYPH_LD = 'x'; // Left-down
    uint8_t _GLYPH_MD = 'y'; // Mid-down
    uint8_t _GLYPH_RD = 'z'; // Right-down
};
}

#endif