#include "uascroll.h"

namespace Gui
{

UAScroll::UAScroll(bool vertical)
: Widget()
, _vertical(vertical)
{
    _flags |= FLAG_ENABLED;
    
    _rect.SetSize(16, 16);

    Check();
    UpdateAreas();
}

UAScroll::UAScroll(Widget *parent, bool vertical)
: Widget(parent)
, _vertical(vertical)
{
    _flags |= FLAG_ENABLED;
    
    _rect.SetSize(16, 16);

    Check();
    UpdateAreas();
}

UAScroll::~UAScroll()
{
}

void UAScroll::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{
    if (_vertical)
    {
        TileMap *commonGfx  = _UATiles[TILESET_MAPVERT];
        TileMap *borders = _UATiles[TILESET_MAPVERT1]; // Borders 1px

        if ( !_areaMinus.IsEmpty() )
        {
            Common::Rect dUp = _areaMinus;
            borders->Draw(surface, dUp, 'C');
            dUp.top += borders->h;   

            if (!dUp.IsEmpty())
                commonGfx->Fill(surface, dUp, 'B');
        }

        if ( !_areaBtn.IsEmpty() )
        {
            Common::Rect btn = _areaBtn;
            borders->Draw(surface, btn, 'E');

            btn.top += borders->h;
            btn.bottom -= borders->h;

            if (!btn.IsEmpty())
                commonGfx->Fill(surface, btn, 'C');

            btn.top = btn.bottom;
            btn.bottom += borders->h;

            borders->Draw(surface, btn, 'F');
        }

        if ( !_areaPlus.IsEmpty() )
        {
            Common::Rect dwn = _areaPlus;
            dwn.bottom -= borders->h;

            if (!dwn.IsEmpty())
                commonGfx->Fill(surface, dwn, 'B');

            dwn.top = dwn.bottom;
            dwn.bottom += borders->h;

            borders->Draw(surface, dwn, 'D');
        }
    }
    else
    {
        TileMap *gfx  = _UATiles[TILESET_MAPHORZ];
        
        if ( !_areaMinus.IsEmpty() )
        {
            Common::Rect dLeft = _areaMinus;
            gfx->Draw(surface, dLeft.Pos(), 'A');
            
            dLeft.left += gfx->GetWidth('A');
            if (!dLeft.IsEmpty())
                gfx->Fill(surface, dLeft, 'B');
        }
        
        if ( !_areaBtn.IsEmpty() )
        {
            Common::Rect btn = _areaBtn;
            gfx->Draw(surface, btn.Pos(), 'D');

            btn.left += gfx->GetWidth('D');
            btn.right -= gfx->GetWidth('F');

            if (!btn.IsEmpty())
                gfx->Fill(surface, btn, 'E');

            btn.left = btn.right;
            btn.right += gfx->GetWidth('F');

            gfx->Draw(surface, btn.Pos(), 'F');
        }
        
        if ( !_areaPlus.IsEmpty() )
        {
            Common::Rect dRght = _areaPlus;
            dRght.right -= gfx->GetWidth('C');

            if (!dRght.IsEmpty())
                gfx->Fill(surface, dRght, 'B');

            dRght.left = dRght.right;
            dRght.right += gfx->GetWidth('C');

            gfx->Draw(surface, dRght.Pos(), 'C');
        }
    }
}
    
void UAScroll::MouseDown(Common::Point pos, Common::Point scrPos, int button)
{
    _lastPos = pos;
    
    if (button == MICE_LEFT)
    {
        _downedArea = GetArea(pos);
        _mouseArea = _downedArea;

        if (_downedArea == AREA_BTN)
        {
            _btnOff = pos - _areaBtn.Pos();
            
            _svdPxPos = _vertical ? _areaBtn.top : _areaBtn.left;
            _curPxPos = _svdPxPos;
        }
        else if (_downedArea == AREA_MINUS)
        {
            StepMinus();
            Root::Instance.TimerAdd(_id, TIME_FIRST);
        }
        else if (_downedArea == AREA_PLUS)
        {
            StepPlus();
            Root::Instance.TimerAdd(_id, TIME_FIRST);
        }
    }
}

void UAScroll::MouseUp(Common::Point pos, Common::Point scrPos, int button)
{
    _lastPos = pos;
    
    if (button == MICE_LEFT)
    {
        if (_downedArea == AREA_BTN)
        {
            if ( GetSizeRect().IsIn(pos) )
            {
                if (_vertical)
                    _scrlPos = (pos.y - _btnOff.y + _areaBtn.Height() / 2) * _scrlCount / _rect.Height();
                else
                    _scrlPos = (pos.x - _btnOff.x + _areaBtn.Width() / 2) * _scrlCount / _rect.Width();
            }
        }
        else if (_downedArea == AREA_MINUS || _downedArea == AREA_PLUS)
            Root::Instance.TimerDeleteByWidget(_id);

        _downedArea = AREA_NONE;
    
        Check();
        UpdateAreas();
    }
}

void UAScroll::MouseMove(Common::Point pos, Common::Point scrPos, int button)
{
    _lastPos = pos;
    
    if (button & MICE_LEFT)
    {
        auto oldArea = _mouseArea;
        _mouseArea = GetArea(pos);

        if (_downedArea == AREA_BTN)
        {
            if ( GetSizeRect().IsIn(pos) ) // is in scrollbar
            {
                _curPxPos = _vertical ? (pos.y - _btnOff.y) : (pos.x - _btnOff.x);

                Check();
                UpdateAreas();
            }
            else if (_curPxPos != _svdPxPos)
            {
                _curPxPos = _svdPxPos;

                Check();
                UpdateAreas();
            }
        }
        else if ((_downedArea == AREA_MINUS || _downedArea == AREA_PLUS) && oldArea != _mouseArea) // changed area
        {
            if (oldArea == _downedArea)
                Root::Instance.TimerDeleteByWidget(_id);
            else if (_mouseArea == _downedArea)
                Root::Instance.TimerAdd(_id, TIME_NEXT);
        }
    }
}

void UAScroll::Resize(Common::Point sz)
{
    int oldPx = _vertical ? GetHeight() : GetWidth();
    
    Widget::Resize(sz);
    
    Check();
    UpdateAreas();
    
    if (_downedArea == AREA_BTN)
        _svdPxPos = _svdPxPos * (_vertical ? GetHeight() : GetWidth()) / oldPx;
}

void UAScroll::TimerEvent(uint32_t code, uint32_t timerID)
{
    if (_downedArea != AREA_NONE && _mouseArea == _downedArea)
    {
        if (_downedArea == AREA_MINUS)
            StepMinus();
        else if (_downedArea == AREA_PLUS)
            StepPlus();
        
        Root::Instance.TimerAdd(_id, TIME_NEXT);
    }
}

void UAScroll::StepMinus()
{
    _scrlPos--;
    
    Check();
    UpdateAreas();
}

void UAScroll::StepPlus()
{
    _scrlPos++;

    Check();
    UpdateAreas();
}

void UAScroll::Check()
{
    if (_vertical)
    {
        if (_rect.Width() > _UATiles[TILESET_MAPVERT]->map['B'].w)
            _rect.SetWidth(_UATiles[TILESET_MAPVERT]->map['B'].w);
    }
    else
    {
        if (_rect.Height() > _UATiles[TILESET_MAPHORZ]->h)
            _rect.SetHeight(_UATiles[TILESET_MAPHORZ]->h);
    }
    
    if (_scrlPageSz < 1)
        _scrlPageSz = 1;
    
    if (_scrlPos > _scrlCount - _scrlPageSz)
        _scrlPos = _scrlCount - _scrlPageSz;
            
    if (_scrlPos < 0)
        _scrlPos = 0;
}

void UAScroll::UpdateAreas()
{
    int pxLen;
    int pxW;
    
    if (_vertical)
    {
        pxLen = _rect.Height();
        pxW = _rect.Width();
    }
    else
    {
        pxLen = _rect.Width();
        pxW = _rect.Height();
    }

    int btnSz = pxLen;
    int btnStart = 0;

    if ( _scrlCount > _scrlPageSz )
    {
        btnSz    = pxLen * _scrlPageSz / _scrlCount;
        btnStart = pxLen * _scrlPos    / _scrlCount;

        if (btnSz < 2)
        {
            btnStart -= (2 - btnSz) / 2;
            btnSz = 2;
        }

        if (_downedArea == AREA_BTN)
        {
            btnStart = _curPxPos;
            if (btnStart + btnSz > pxLen)
                btnStart = pxLen - btnSz;
            if (btnStart < 0)
                btnStart = 0;
        }
    }

    if (_vertical)
    {
        _areaMinus = Common::PointRect(0, 0, pxW, btnStart);
        _areaBtn   = Common::PointRect(0, btnStart, pxW, btnSz);
        _areaPlus  = Common::PointRect(0, btnStart + btnSz, pxW, pxLen - (btnStart + btnSz));
    }
    else
    {
        _areaMinus = Common::PointRect(0, 0, btnStart, pxW);
        _areaBtn   = Common::PointRect(btnStart, 0, btnSz, pxW);
        _areaPlus  = Common::PointRect(btnStart + btnSz, 0, pxLen - (btnStart + btnSz), pxW);
    }

    _mouseArea = GetArea(_lastPos);
}

int UAScroll::GetArea(Common::Point pos)
{
    if (_areaBtn.IsIn(pos))
        return AREA_BTN;
    else if (_areaMinus.IsIn(pos))
        return AREA_MINUS;
    else if (_areaPlus.IsIn(pos))
        return AREA_PLUS;
    return AREA_NONE;
}

void UAScroll::SetScrollPos(int p)
{
    _downedArea = AREA_NONE;
    _scrlPos = p;
    
    Check();
    UpdateAreas();
}

void UAScroll::SetScrollCount(int c)
{
    _scrlCount = c;
    
    Check();
    UpdateAreas();
}

void UAScroll::SetScrollPageSize(int ps)
{
    _scrlPageSz = ps;
    
    Check();
    UpdateAreas();
}



}