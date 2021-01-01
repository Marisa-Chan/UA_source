#include "widget.h"


namespace Gui
{

Widget::Widget()
: _id(Root::Instance.GetNextId())
{
}

Widget::Widget(Widget *parent)
: _id(Root::Instance.GetNextId())
{
    parent->AddChild(this);
}

Widget::~Widget()
{
    Unparent();
    
    if (_hwSurface)
        SDL_FreeSurface(_hwSurface);
    
    for ( auto x : _childs )
    {
        x->_parent = NULL;
        delete x;
    }
}

void Widget::AddChild(Widget *child, bool top)
{
    child->Unparent();
    child->_parent = this;
    
    if (child->_modal)
    {
        top = true;
        _modals.push_front(child);
    }

    if (top)
        _childs.push_front(child);
    else
        _childs.push_back(child);
    
    child->OnAttachDetach(true);
}

void Widget::RemoveChild(Widget *child)
{
    _modals.remove(child);
    _childs.remove(child);
    child->_parent = NULL;
    
    child->OnAttachDetach(false);
}

void Widget::Unparent()
{
    if (_rooted != Root::LAYER_UNK)
    {
        Root::Instance.RemoveWidget(this);
        OnAttachDetach(false);
    }
    else if (_parent)
    {
        _parent->RemoveChild(this); //Will clean this field
        OnAttachDetach(false);
    }
}

bool Widget::IsOn(const Common::Point &pos) const
{
    return GetScreenVisibleRect().IsIn( pos );
}

bool Widget::IsOn(int x, int y) const
{
    return GetScreenVisibleRect().IsIn( Common::Point(x, y) );
}

bool Widget::IsLOn(const Common::Point &pos) const
{
    return GetWidgetVisibleRect().IsIn( pos );
}

bool Widget::IsLOn(int x, int y) const
{
    return GetWidgetVisibleRect().IsIn( Common::Point(x, y) );
}

Common::Point Widget::ScreenCoordToClient(Common::Point pos) const
{
    Common::Point p = pos - GetOnParentPos();
    
    if (_flags & FLAG_CLIENT)
        p -= _client.Pos();
    
    for (Widget *par = _parent; par; par = par->_parent)
        p -= par->GetOnParentPos();
    return p;
}

Common::Point Widget::ScreenCoordToWidget(Common::Point pos) const
{
    Common::Point p = pos - GetOnParentPos();
    
    for (Widget *par = _parent; par; par = par->_parent)
        p -= par->GetOnParentPos();
    return p;
}

Common::Point Widget::GetOnParentPos() const
{
    if (_parent)
    {
        if ( !(_flags & FLAG_PRIVATE) && (_parent->_flags & FLAG_CLIENT) )
            return _parent->_client.Pos() + _rect.Pos();
    }
    return _rect.Pos();
}

Common::Rect Widget::GetScreenVisibleRect() const
{
    Common::Rect tmp = _rect;
    bool primer = (_flags & FLAG_PRIVATE) != 0;
    
    for (Widget *par = _parent; par; par = par->_parent)
    {
        if ( !primer && (par->_flags & FLAG_CLIENT) )
        {
            tmp.Translate(par->_client.Pos());
            tmp.ClipBy(par->_client);
        }
        
        tmp.Translate(par->_rect.Pos());
        tmp.ClipBy(par->_rect);
        
        if (tmp.IsEmpty())
            return Common::Rect();
        
        primer = (par->_flags & FLAG_PRIVATE) != 0;
    }
    return tmp;
}

Common::Rect Widget::GetWidgetVisibleRect() const
{
    Common::Rect tmp = _rect;
    bool primer = (_flags & FLAG_PRIVATE) != 0;
    
    Common::Point offset = _rect.Pos();
    
    for (Widget *par = _parent; par; par = par->_parent)
    {
        if ( !primer && (par->_flags & FLAG_CLIENT) )
        {
            offset += par->_client.Pos();
            tmp.Translate(par->_client.Pos());
            tmp.ClipBy(par->_client);
        }
        
        offset += par->_rect.Pos();
        tmp.Translate(par->_rect.Pos());
        tmp.ClipBy(par->_rect);
        
        if (tmp.IsEmpty())
            return Common::Rect();
        
        primer = (par->_flags & FLAG_PRIVATE) != 0;
    }
    tmp.Translate(-offset);
    return tmp;
}

Widget *Widget::FindChildLPos(const Common::Point &pos, bool stopOnModal)
{
    if ( !IsEnabled() || !GetSizeRect().IsIn(pos) )
        return NULL;

    Common::Point cliPos = pos;
    bool inCli = true;
    
    if (_flags & FLAG_CLIENT)
    {
        if (!_client.IsEmpty() && _client.IsIn(pos))
            cliPos -= _client.Pos();
        else
            inCli = false;
    }
    
    if (stopOnModal && !_modals.empty())
    {
        for(auto w : _modals)
        {
            if (w->IsEnabled())
            {
                if (w->_flags & FLAG_PRIVATE)
                { 
                    if (w->_rect.IsIn(pos))
                        return w->FindChildLPos(pos - w->_rect.Pos(), stopOnModal);
                }
                else if ( inCli && w->_rect.IsIn(cliPos) )
                    return w->FindChildLPos(cliPos - w->_rect.Pos(), stopOnModal);
                
                return this;
            }
        }
    }

    for(auto w : _childs)
    {
        if (w->IsEnabled())
        {
            if (w->_flags & FLAG_PRIVATE)
            { 
                if (w->_rect.IsIn(pos))
                    return w->FindChildLPos(pos - w->_rect.Pos(), stopOnModal);
            }
            else if ( inCli && w->_rect.IsIn(cliPos) )
                return w->FindChildLPos(cliPos - w->_rect.Pos(), stopOnModal);
        }
    }
    return this;
}

Widget *Widget::FindByPos(const Common::Point &pos, bool stopOnModal)
{
    return FindChildLPos(ScreenCoordToWidget(pos), stopOnModal);
}


Widget *Widget::FindByID(uint32_t id, bool enabled)
{
    if (enabled && !IsEnabled())
        return NULL;
    
    if (_id == id)
        return this;
    
    for(auto w : _childs)
    {
        Widget *t = w->FindByID(id, enabled);
        if (t)
            return t;
    }
    return NULL;
}


Widget *Widget::GetRoot()
{
    Widget *cur = this;
    while (cur->_parent)
        cur = cur->_parent;
    return cur;
}

Common::Point Widget::GetPos() const
{
    return _rect.Pos();
}

Common::Point Widget::GetSize() const
{
    return _rect.Size();
}

Common::Rect  Widget::GetSizeRect() const
{
    return Common::Rect( _rect.Size() );
}

Common::Point Widget::GetScreenPos() const
{
    Common::Point point = GetOnParentPos();
    
    for (Widget *par = _parent; par; par = par->_parent)
        point += par->GetOnParentPos();

    return point;
}

void Widget::SetEnable(bool enable)
{
    bool top = IsRooted() && _parent == NULL;
    bool chg = enable != ((_flags & FLAG_ENABLED) != 0);
    
    if (enable)
        _flags |= FLAG_ENABLED;
    else
        _flags &= ~FLAG_ENABLED;
    
    if (_fOnHideShow && top && chg)
        _fOnHideShow(this, _fOnHideShowData, enable);
}

void Widget::SetClientEnable(bool enable)
{
    if (enable)
        _flags |= FLAG_CLIENT;
    else
        _flags &= ~FLAG_CLIENT;
}

void Widget::MoveTo(Common::Point pos)
{
    _rect.MoveTo(pos);
    if (_fOnMove)
        _fOnMove(this, _fOnMoveData, pos);
}

void Widget::SetPrivate(bool mode)
{
    if (mode)
        _flags |= FLAG_PRIVATE;
    else
        _flags &= ~FLAG_PRIVATE;
}

bool Widget::GetPrivate() const
{
    return (_flags & FLAG_PRIVATE) != 0;
}

void Widget::ToFront()
{
    if (IsRooted())
        Root::Instance.RootWidgetToFront(this);
    else if (_parent)
        _parent->_childs.MoveToFirst(this);
}

void Widget::SetClientRect(Common::Rect r)
{
    r.ClipBy( Common::Rect(_rect.Size()) );
    _client = r;
}

void Widget::Resize(Common::Point sz)
{
    if (sz.x < 0)
        sz.x = 0;
    
    if (sz.y < 0)
        sz.y = 0;
    
    _rect.SetSize( sz );
    
    if (_fOnResize)
        _fOnResize(this, _fOnResizeData, sz);
    
    for(Widget *w : _childs)
        w->OnParentResize(sz);
}

void Widget::MouseDown(Common::Point pos, Common::Point scrPos, int button)
{
    if (_fOnMouseDown)
        _fOnMouseDown(this, _fOnMouseDownData, pos, scrPos, button);
}

void Widget::MouseUp(Common::Point pos, Common::Point scrPos, int button)
{
    if (_fOnMouseUp)
        _fOnMouseUp(this, _fOnMouseUpData, pos, scrPos, button);
}

void Widget::MouseMove(Common::Point pos, Common::Point scrPos, int buttons)
{
    if (_fOnMouseMove)
        _fOnMouseMove(this, _fOnMouseMoveData, pos, scrPos, buttons);
}

void Widget::MouseEnter()
{
    _mOver = true;
    
    if (_fOnMouseEnter)
        _fOnMouseEnter(this, _fOnMouseEnterData);
}
void Widget::MouseLeave()
{
    _mOver = false;
    
    if (_fOnMouseLeave)
        _fOnMouseLeave(this, _fOnMouseLeaveData);
}

int Widget::GetWidth() const
{
    return _rect.Width();
}

int Widget::GetHeight() const
{
    return _rect.Height();
}

Common::Point Widget::GetSpace() const
{
    if (IsRooted())
        return Root::Instance.GetScreenSize();
    else if (_parent)
        return _parent->GetSize();
    return Common::Point();        
}

void Widget::OnAttachDetach(bool attach)
{
    if (_fOnAttachDetach)
        _fOnAttachDetach(this, _fOnAttachDetachData, attach);
}

void Widget::OnParentResize(Common::Point newSz)
{
    if (_fOnParentResize)
        _fOnParentResize(this, _fOnParentResizeData, newSz);
}

}
