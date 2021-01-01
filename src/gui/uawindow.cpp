#include "uawindow.h"
#include "../engine_miles.h"
#include "../yw.h"

namespace Gui
{

UAWindow::UAWindow(const std::string &title, Common::PointRect posRect, uint32_t flags)
: Widget()
{
    Init(title, posRect, flags);
}

UAWindow::UAWindow(Widget *parent, const std::string &title, Common::PointRect posRect, uint32_t flags)
: Widget(parent)
{
    Init(title, posRect, flags);
}

void UAWindow::Init(const std::string &title, Common::PointRect posRect, uint32_t flags)
{
    _rect = posRect;
    _flagsWindow = flags;
    _title = title;
    
    SetClientEnable(true);
    
    _helpBtn = new UAOneTileButton(this, 'A', Common::PointRect(), TILESET_ICONNS, TILESET_ICONPS);
    _helpBtn->SetPrivate(true);
    _helpBtn->_fOnMouseDown = OnBtnDownSound;
    
    _closeBtn = new UAOneTileButton(this, 'U', Common::PointRect(), TILESET_ICONNS, TILESET_ICONPS);
    _closeBtn->SetPrivate(true);
    _closeBtn->_fOnClick = OnCloseClick;
    _closeBtn->_fOnClickData = this;
    _closeBtn->_fOnMouseDown = OnBtnDownSound;
    
    _maxmBtn = new UAOneTileButton(this, 'r', 't', Common::PointRect(), TILESET_DEFAULT, TILESET_DEFAULT);
    _maxmBtn->SetPrivate(true);
    
    _vScroll = new UAScroll(this);
    _vScroll->SetPrivate(true);
    _vScroll->SetScrollCount(10);
    
    _hScroll = new UAScroll(this, false);
    _hScroll->SetPrivate(true);
    _hScroll->SetScrollCount(10);
    
    Update();
}

UAWindow::~UAWindow()
{
    delete _helpBtn;
    delete _closeBtn;
    delete _maxmBtn;
    delete _vScroll;
    delete _hScroll;
}


void UAWindow::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{   
    TileMap *tiles;
    
    // Draw window box
    // Top
    tiles   = _UATiles[_TSET_U];    
    
    tiles->Draw(surface, Common::Point(0, 0), _GLYPH_LU );
    
    Common::Rect out(tiles->GetWidth(_GLYPH_LU), 0, GetWidth() - tiles->GetWidth(_GLYPH_RU), tiles->h);
    tiles->Fill(surface, out, _GLYPH_MU);
    
    tiles->Draw(surface, Common::Point(out.right, 0), _GLYPH_RU );
    
    
    // Mid
    tiles   = _UATiles[_TSET_M];
    
    out = Common::Rect(0, tiles->h, tiles->GetWidth(_GLYPH_LM), GetHeight() - tiles->h);
    tiles->Fill(surface, out, _GLYPH_LM);
    out.left = out.right;
    out.right = GetWidth() - tiles->GetWidth(_GLYPH_RM);
    tiles->Fill(surface, out, _GLYPH_MM);
    out.left = out.right;
    out.right = GetWidth();
    tiles->Fill(surface, out, _GLYPH_RM);
    
    // Bot
    tiles   = _UATiles[_TSET_D];
    
    tiles->Draw(surface, Common::Point(0, out.bottom), _GLYPH_LD);
    
    out = Common::Rect(tiles->GetWidth(_GLYPH_LD), out.bottom, GetWidth() - tiles->GetWidth(_GLYPH_RD), GetHeight());
    tiles->Fill(surface, out, _GLYPH_MD);
    
    tiles->Draw(surface, Common::Point(out.right, out.top), _GLYPH_RD);
    
    // Title text
    DrawText(surface, std::string(" ") + _title, 0, _UATextColor, _titleRect);
    
    if (_flagsWindow & FLAG_WND_RESIZEABLE)
        _UATiles[TILESET_MAPHORZ]->Draw(surface, _areaResize, 'G');
    
    /*SDL_Rect tmp = _client; 
    SDL_FillRect(surface, &tmp, SDL_MapRGB(surface->format, 255, 0, 0));*/
}

void UAWindow::MouseDown(Common::Point pos, Common::Point scrPos, int button)
{   
    if (button == MICE_LEFT)
    {
        if ((_flagsWindow & FLAG_WND_RESIZEABLE) && _areaResize.IsIn(pos))
        {
            _inResizing = true;
            _rszPnt = _rect.Size() - pos;
        }
        else if (_areaTitle.IsIn(pos) && (_flagsWindow & FLAG_WND_UNMOVE) == 0)
        {
            Root::Instance.StartDragging(this);
        }
    }
        
}

void UAWindow::MouseUp(Common::Point pos, Common::Point scrPos, int button)
{
    if (button == MICE_LEFT)
    {
        _inResizing = false;
    }
}

void UAWindow::MouseMove(Common::Point pos, Common::Point scrPos, int button)
{
    if (_inResizing && (button & MICE_LEFT))
        Resize(pos + _rszPnt);
}

void UAWindow::Resize(Common::Point sz)
{
    TileMap *def = _UATiles[_TSET_M];
    sz.x = Common::MAX(sz.x, def->GetWidth(_GLYPH_LM) + def->GetWidth(_GLYPH_RM) + _closeBtn->GetWidth() * 3);
    sz.y = Common::MAX(sz.y, def->h * 2);
    
    Widget::Resize(sz);
    
    Update();
}

void UAWindow::Update()
{
    _areaTitle = Common::Rect(0, 0, GetWidth(), _UATiles[_TSET_U]->h);
    _areaResize = Common::Rect(GetWidth() - _UATiles[TILESET_MAPHORZ]->GetWidth('G'), GetHeight() - _UATiles[TILESET_MAPHORZ]->h, GetWidth(), GetHeight());

    _client.left = _UATiles[_TSET_M]->GetWidth(_GLYPH_LM);
    _client.top = _areaTitle.bottom;
    _client.right = GetWidth() - _UATiles[_TSET_M]->GetWidth(_GLYPH_RM);
    _client.bottom = GetHeight() - _UATiles[_TSET_M]->GetWidth(_GLYPH_RM); // Yes, we use width
    
    int rpos = GetWidth();
    
    if ((_flagsWindow & (FLAG_WND_CLOSE | FLAG_WND_HELP | FLAG_WND_MAXM)) == 0)
        rpos -= _UATiles[_TSET_U]->GetWidth(_GLYPH_RU);
    
    if (_flagsWindow & FLAG_WND_CLOSE)
    {
        if (!_closeBtn->IsEnabled())
            _closeBtn->SetEnable(true);
        
        rpos -= _closeBtn->GetWidth();
        _closeBtn->MoveTo( rpos, 0);
    }
    else if (_closeBtn->IsEnabled())
        _closeBtn->SetEnable(false);
    
    if (_flagsWindow & FLAG_WND_HELP)
    {
        if (!_helpBtn->IsEnabled())
            _helpBtn->SetEnable(true);
        
        rpos -= _helpBtn->GetWidth();
        _helpBtn->MoveTo( rpos, 0);
    }
    else if (_helpBtn->IsEnabled())
        _helpBtn->SetEnable(false);
    
    if (_flagsWindow & FLAG_WND_MAXM)
    {
        if (!_maxmBtn->IsEnabled())
            _maxmBtn->SetEnable(true);
        
        rpos -= _maxmBtn->GetWidth();
        _maxmBtn->MoveTo( rpos, 0);
    }
    else if (_maxmBtn->IsEnabled())
        _maxmBtn->SetEnable(false);
    
    _titleRect = Common::Rect(_UATiles[_TSET_U]->GetWidth(_GLYPH_LU), 0, rpos, _areaTitle.bottom);
        
    if (_flagsWindow & FLAG_WND_VSCROLL)
    {
        if (!_vScroll->IsEnabled())
            _vScroll->SetEnable(true);
        
        _vScroll->MoveTo(GetWidth() - _vScroll->GetWidth(), _areaTitle.bottom);
        _vScroll->ResizeWH(_vScroll->GetWidth(), GetHeight() - _vScroll->GetPos().y - _areaResize.Height());
    }
    else if (_vScroll->IsEnabled())
        _vScroll->SetEnable(false);
    
    if (_flagsWindow & FLAG_WND_HSCROLL)
    {
        if (!_hScroll->IsEnabled())
            _hScroll->SetEnable(true);
        
        _hScroll->MoveTo(0, GetHeight() - _hScroll->GetHeight());
        _hScroll->ResizeWH(GetWidth() - _areaResize.Width(), _hScroll->GetHeight());
    }
    else if (_hScroll->IsEnabled())
        _hScroll->SetEnable(false);
    
    if (_flagsWindow & (FLAG_WND_HSCROLL | FLAG_WND_RESIZEABLE))
        _client.bottom = _areaResize.top;
    
    if (_flagsWindow & FLAG_WND_VSCROLL)
        _client.right = _areaResize.left;
}

void UAWindow::OnCloseClick(Widget *w, void *d)
{
    Widget *form = (Widget *)d;
    form->SetEnable(false);
}

void UAWindow::OnHelpClick(Widget *w, void *d)
{
    UAWindow *form = (UAWindow *)d;
    if (form->_fOnHelpClick)
        form->_fOnHelpClick(form, form->_fOnHelpClickData);
}

void UAWindow::OnBtnDownSound(Widget *w, void *d, Common::Point, Common::Point, int)
{
    SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 3);
}



}