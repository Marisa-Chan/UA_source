#include <SDL2/SDL_opengl.h>

#include "root.h"
#include "widget.h"

#include "../wrapSDL.h"


namespace Gui
{
Root Root::Instance;

Root::~Root()
{
    if (_hwTex)
        glDeleteTextures(1 , &_hwTex);

    if (_dirtSurface)
        SDL_FreeSurface(_dirtSurface);
}

uint32_t Root::GetNextId()
{
    auto tmID = _nextId;
    _nextId = (_nextId + 2) & ~(1 << ((sizeof(_nextId) << 3) - 1));
    return tmID;
}

void Root::AddWidget(Widget *w, bool top)
{
    _miceOn = NULL; // Find it

    w->Unparent();
    w->_rooted = LAYER_NORMAL;
    
    if (w->_modal)
    {
        top = true;
        _modals.push_front(w);
    }    
    
    if (top)
        _normal.push_front(w);
    else
        _normal.push_back(w);
    
    w->OnAttachDetach(true);
    
    if (w->IsEnabled() && w->_fOnHideShow) // If it's already enabled do call hide/show
        w->_fOnHideShow(w, w->_fOnHideShowData, true);
        
}

WidgetList& Root::GetLayerList(int l)
{
    switch(l)
    {
    case LAYER_BACK:
        return _background;
    case LAYER_FRONT:
        return _foreground;
    case LAYER_NORMAL:
    default:
        return _normal;
    }
}

void Root::RemoveWidget(Widget *w)
{
    if (!w->IsRooted())
        return;
    
    _modals.remove(w);
    
    GetLayerList(w->_rooted).remove(w);

    if (_miceOn == w)
        _miceOn = NULL;

    if (_focused == w)
        _focused = NULL;

    if (_dragging == w)
        _dragging = NULL;

    w->_rooted = LAYER_UNK;
    
    w->OnAttachDetach(false);
    
    if (w->IsEnabled() && w->_fOnHideShow) // If enabled do call hide/show
        w->_fOnHideShow(w, w->_fOnHideShowData, false);
}


void Root::Draw(SDL_Surface *screen)
{
    if (!_hwRender)
    {
        for(auto it = _normal.rbegin(); it != _normal.rend(); it++)
            DrawWidget(screen, Common::Rect(screen->w, screen->h), Common::Point(0, 0), *it);
    }
    else
    {
        for(Widget *w : _normal)
            HwPrepareWidget(w);
    }
    
    /*if (_miceOn)
    {
        Common::Rect rct = _miceOn->GetScreenVisibleRect();
        
        SDL_Surface *srf = SDL_CreateRGBSurfaceWithFormat(0, rct.Width(), rct.Height(), screen->format->BitsPerPixel, screen->format->format);
        SDL_FillRect(srf, NULL, SDL_MapRGBA(srf->format, 255, 0, 255, 128) );
        
        SDL_Rect pz = rct;
        
        SDL_BlitSurface(srf, NULL, screen, &pz);
        
        SDL_FreeSurface(srf);
    }*/
}

void Root::ModAlpha(SDL_Surface *surf, Common::Rect space, uint8_t alpha)
{
    if ( alpha != 255 && surf->format->BitsPerPixel == 32 && surf->format->Amask)
    {
        int bpp = surf->format->BytesPerPixel;
        uint32_t ashift = surf->format->Ashift;
        //uint32_t aloss = surf->format->Aloss;
        uint32_t amask = surf->format->Amask;
        uint32_t anmask = ~amask;
        
        if (space.IsEmpty())
            space = Common::Rect(surf->w, surf->h);
        
        space.ClipBy(surf->w, surf->h);
        
        if (!space.IsEmpty())
        {
            if ( SDL_LockSurface(surf) == 0 )
            {
                for (int y = space.top; y < space.bottom; y++)
                {
                    uint32_t *pixels = (uint32_t *)((uint8_t *)surf->pixels + y * surf->pitch + space.left * bpp);
                    
                    for (int x = space.left; x < space.right; x++)
                    {
                        uint32_t clr = *pixels;
                        uint32_t a = ((uint32_t)((clr & amask) >> ashift) * (uint32_t)alpha / 255) << ashift;
                        *pixels = (clr & anmask) | a;

                        ++pixels;
                    }
                }
                
                SDL_UnlockSurface(surf);
            }
        }
    }
}

void Root::DrawWidget(SDL_Surface *screen, Common::Rect space, Common::Point parentOffset, Widget *w, uint32_t alph)
{
    uint32_t effA = ((uint32_t)w->_alpha * alph) / 255;
    if (w && w->IsEnabled() && effA != 0 && !space.IsEmpty())
    {
        Common::Rect widgetSpace = w->_rect;
        widgetSpace.Translate(parentOffset);
        widgetSpace.ClipBy(space);
        if (!widgetSpace.IsEmpty())
        {
            bool drawWidget = (w->_flags & Widget::FLAG_NODRAW) == 0;
            if ( drawWidget &&
                 (!_dirtSurface || _dirtSurface->w < w->_rect.Width() || _dirtSurface->h < w->_rect.Height()))
            {
                int mW;
                int mH;
                if (_dirtSurface)
                {
                    mW = Common::MAX(_dirtSurface->w, w->_rect.Width());
                    mH = Common::MAX(_dirtSurface->h, w->_rect.Height());
                    SDL_FreeSurface(_dirtSurface);
                }
                else
                {
                    mW = w->_rect.Width();
                    mH = w->_rect.Height();
                }

                _dirtSurface = CreateScreenFmtSurface(mW, mH);
            }

            Common::Rect dirtRect = widgetSpace;
            dirtRect.Translate(-parentOffset);
            dirtRect.Translate(-w->GetPos());
            
            if (drawWidget)
                w->Draw(_dirtSurface, dirtRect);

            SDL_Rect dstRect;
            SDL_Rect srcRect;

            dstRect.x = widgetSpace.left;
            dstRect.y = widgetSpace.top;
            dstRect.w = widgetSpace.Width();
            dstRect.h = widgetSpace.Height();
            srcRect.x = dirtRect.left;
            srcRect.y = dirtRect.top;
            srcRect.w = dirtRect.Width();
            srcRect.h = dirtRect.Height();

            if (drawWidget)
            {
                if (effA != 255)
                    ModAlpha(_dirtSurface, srcRect, effA);
                
                SDL_BlitSurface(_dirtSurface, &srcRect, screen, &dstRect);
            }

            parentOffset += w->_rect.Pos();
            
            Common::Point clientOffset = parentOffset;
            Common::Rect clientSpace = widgetSpace;
            
            if (w->_flags & Widget::FLAG_CLIENT)
            {
                Common::Rect s = w->_client;
                s.Translate( parentOffset );
                clientSpace.ClipBy(s);

                clientOffset += w->_client.Pos();
            }

            for(auto it = w->_childs.rbegin(); it != w->_childs.rend(); it++)
            {
                if ( (*it)->_flags & Widget::FLAG_PRIVATE )
                    DrawWidget(screen, widgetSpace, parentOffset, *it, effA);
                else if ( !clientSpace.IsEmpty() )
                    DrawWidget(screen, clientSpace, clientOffset, *it, effA);
            }
                
        }
    }
}

void Root::HwPrepareWidget(Widget *w)
{
    if (w && w->IsEnabled() && w->_alpha != 0 && !w->_rect.IsEmpty())
    {
        bool drawWidget = (w->_flags & Widget::FLAG_NODRAW) == 0;
        
        if ( drawWidget || !w->_childs.empty() )
        {
            if (!w->_hwSurface || w->_hwSurface->w != w->_rect.Width() || w->_hwSurface->h != w->_rect.Height())
            {
                if (w->_hwSurface)
                    SDL_FreeSurface(w->_hwSurface);

                w->_hwSurface = CreateScreenFmtSurface(w->_rect.Width(), w->_rect.Height());
            }

            Common::Rect widgetSpace(w->_rect.Size());

            if (drawWidget)
                w->Draw( w->_hwSurface, widgetSpace );
            else
                SDL_FillRect( w->_hwSurface, NULL, SDL_MapRGBA(w->_hwSurface->format, 0, 0, 0, 0));

            Common::Point clientOffset(0, 0);
            Common::Rect clientSpace = widgetSpace;

            if (w->_flags & Widget::FLAG_CLIENT)
            {
                Common::Rect s = w->_client;
                clientSpace.ClipBy(s);

                clientOffset += w->_client.Pos();
            }

            for(auto it = w->_childs.rbegin(); it != w->_childs.rend(); it++)
            {
                if ( (*it)->_flags & Widget::FLAG_PRIVATE )
                    DrawWidget(w->_hwSurface, widgetSpace, Common::Point(0, 0), *it);
                else if ( !clientSpace.IsEmpty() )
                    DrawWidget(w->_hwSurface, clientSpace, clientOffset, *it);
            }
        }
    }
}

bool Root::MouseDown(Common::Point pos, int button)
{
    ValidateWidgets();
        
    _micePos = pos;

    int btn = 0;
    switch(button)
    {
    case SDL_BUTTON_LEFT    : btn = MICE_LEFT;    break;
    case SDL_BUTTON_RIGHT   : btn = MICE_RIGHT;   break;
    case SDL_BUTTON_MIDDLE  : btn = MICE_MIDDLE;  break;
    default: return false;
    }
    
    bool firstBtn = _buttons == 0;
    _buttons |= btn;

    if (_dragging)
        return true;
    else
    {       
        if (firstBtn)
            UpdateWidgetOnMice( FindByPos(pos, true) );

        if (_miceOn)
        {
            if (!_miceOn->IsFocused() && button == MICE_LEFT)
            {
                if (!(_miceOn->_flags & Widget::FLAG_NOBRING))
                    RootWidgetToFront(_miceOn);
                ChangeFocus(_miceOn);
            }
            
            _miceOn->MouseDown(_miceOn->ScreenCoordToWidget(pos), pos, btn);
            return true;
        }
    }
    return false;
}

bool Root::MouseUp(Common::Point pos, int button)
{
    ValidateWidgets();
    
    _micePos = pos;

    int btn = 0;
    switch(button)
    {
    case SDL_BUTTON_LEFT    : btn = MICE_LEFT;    break;
    case SDL_BUTTON_RIGHT   : btn = MICE_RIGHT;   break;
    case SDL_BUTTON_MIDDLE  : btn = MICE_MIDDLE;  break;
    default: return false;
    }

    _buttons &= ~btn;
    
    if (_dragging)
    {
        if (!_buttons)
        {
            _dragging->MoveTo(pos - _dragPos);
            _dragging = NULL;
            
            
            UpdateWidgetOnMice( FindByPos(pos, true) );
        }
        return true;
    }
    else
    {
        if (_miceOn)
        {
            if (!_miceOn->IsFocused())
            {
                if (!(_miceOn->_flags & Widget::FLAG_NOBRING))
                    RootWidgetToFront(_miceOn);
                ChangeFocus(_miceOn);
            }

            _miceOn->MouseUp(_miceOn->ScreenCoordToWidget(pos), pos, btn);
            
            if (!_buttons)
                UpdateWidgetOnMice( FindByPos(pos, true) );

            return true;
        }
    }
    return false;
}

bool Root::MouseMove(Common::Point pos)
{
    ValidateWidgets();
    
    _micePos = pos;

    if (_dragging)
    {
        _dragging->MoveTo(pos - _dragPos);
        return true;
    }
    else
    {
        if (!_buttons)
            UpdateWidgetOnMice( FindByPos(pos, true) );

        if (_miceOn)
        {
            _miceOn->MouseMove(_miceOn->ScreenCoordToWidget(pos), pos, _buttons);
            return true;
        }
    }
    return false;
}

void Root::RootWidgetToFront(Widget *w)
{
    Widget *root = w->GetRoot();

    if (_normal.MoveToFirst(root))
        return;

    if (_foreground.MoveToFirst(root))
        return;
}


Widget *Root::_FindByPos(WidgetList &lst, const Common::Point &pos, bool stopOnModal)
{
    for(auto w : lst)
    {
        if (w->IsEnabled() && w->GetScreenVisibleRect().IsIn(pos))
            return w->FindByPos(pos, stopOnModal);
    }
    return NULL;
}

Widget *Root::FindByPos(const Common::Point &pos, bool stopOnModal)
{
    if (stopOnModal && !_modals.empty())
    {
        for(auto w : _modals)
        {
            if (w->IsEnabled())
            {
                if (w->GetScreenVisibleRect().IsIn(pos))
                    return w->FindByPos(pos, stopOnModal);
                return NULL;
            }
        }
    }
            
    Widget *w = _FindByPos(_foreground, pos, stopOnModal);
    if (w)
        return w;

    w = _FindByPos(_normal, pos, stopOnModal);
    if (w)
        return w;

    return NULL;
}

Widget *Root::FindByID(uint32_t id, bool enabled)
{
    Widget *w = _FindByID(_foreground, id, enabled);
    if (w)
        return w;

    w = _FindByID(_normal, id, enabled);
    if (w)
        return w;

    return NULL;
}

Widget *Root::_FindByID(WidgetList &lst, uint32_t id, bool enabled)
{
    for(auto w : lst)
    {
        Widget *t = w->FindByID(id, enabled);
        if (t)
            return t;
    }
    return NULL;
}


void Root::ChangeFocus(Widget *w)
{
    if (_focused == w)
        return;

    if (_focused)
    {
        _focused->FocusOff();
        _focused->_flags &= ~Widget::FLAG_FOCUSED;
    }

    _focused = w;

    if (_focused)
    {
        _focused->FocusOn();
        _focused->_flags |= Widget::FLAG_FOCUSED;
    }
}

void Root::StartDragging(Widget *w)
{
    _dragging = w;
    _dragPos = w->GetScreenPos() - w->_rect.Pos() + (_micePos - w->GetScreenPos());
}


void Root::SetScreenSize(Common::Point sz)
{
    _screenSize = sz;
    
    for(Widget *& w : _foreground)
        w->OnParentResize(sz);
    
    for(Widget *& w : _normal)
        w->OnParentResize(sz);
    
    for(Widget *& w : _background)
        w->OnParentResize(sz);
}

Common::Point Root::GetScreenSize()
{
    return _screenSize;
}

bool Root::CheckEnable(Widget *w)
{
    while (w)
    {
        if (!w->IsEnabled())
            return false;
        w = w->_parent;
    }
    return true;
}

void Root::ValidateWidgets()
{
    if (_dragging && !CheckEnable(_dragging))
        _dragging = NULL;
    
    if (_miceOn && !CheckEnable(_miceOn))
        _miceOn = NULL;
    
    if (_focused && !CheckEnable(_focused))
        ChangeFocus(NULL);
}


void Root::UpdateWidgetOnMice(Widget *w)
{
    if ( _miceOn == w )
        return;
    
    if (_miceOn)
        _miceOn->MouseLeave();
    
    _miceOn = w;
    
    if (w)
        w->MouseEnter();
}


uint32_t Root::TimerAdd(uint32_t wID, uint32_t time, uint32_t code)
{
    if (_timersToNew)
        _timersNew.push_back( Timer(_nextTimerID, _timeStamp + time, wID, code) );
    else
        _timers.push_back( Timer(_nextTimerID, _timeStamp + time, wID, code) );
    
    auto tmID = _nextTimerID;
    _nextTimerID = (_nextTimerID + 2) & ~(1 << ((sizeof(_nextTimerID) << 3) - 1));
    return tmID;
}
    
int32_t Root::_TimerGet(std::list<Timer> &list, uint32_t id)
{
    for ( const Timer &t : list)
    {
        if (t.ID == id)
            return t.EndTime;
    }
    
    return -1;
}

int32_t Root::TimerGet(uint32_t id)
{
    int32_t t = _TimerGet(_timers, id);
    if (t != -1)
        return t - _timeStamp;
    
    t = _TimerGet(_timersNew, id);
    if (t != -1)
        return t - _timeStamp;
        
    return -1;
}
   
bool Root::_TimerDelete(std::list<Timer> &list, uint32_t id)
{
    for ( auto it = list.begin(); it != list.end(); it++ )
    {
        if (it->ID == id)
        {
            list.erase( it );                
            return true;
        }
    }
    
    return false;
}

bool Root::TimerDelete(uint32_t id)
{
    if (_TimerDelete(_timers, id))
        return true;
    
    if (_TimerDelete(_timersNew, id))
        return true;
    
    return false;
}

int Root::_TimerDeleteByWidget(std::list<Timer> &list, uint32_t wID, uint32_t code)
{
    int c = 0;
    
    for ( auto it = list.begin(); it != list.end(); )
    {
        if (it->WidgetID == wID && (code == Timer::CODE_ANY || it->Code == code))
        {
            it = list.erase(it);
            c++;
        }
        else
            it++;
    }

    return c;
}

int Root::TimerDeleteByWidget(uint32_t wID, uint32_t code)
{
    return _TimerDeleteByWidget(_timers, wID, code) + _TimerDeleteByWidget(_timersNew, wID, code);
}


void Root::TimersUpdate(uint32_t dtime)
{
    _timeStamp += dtime;
    
    _timersToNew = true;
    for ( auto it = _timers.begin(); it != _timers.end(); )
    {
        if (it->EndTime <= _timeStamp)
        {
            Timer t = *it;

            // Delete timer first
            it = _timers.erase(it);

            Widget *w = FindByID(t.WidgetID);
            if (w)
                w->TimerEvent(t.Code, t.ID);
        }
        else
            it++;
    }
    _timersToNew = false;
    
    for( auto it = _timersNew.begin(); it != _timersNew.end(); it = _timersNew.erase(it) )
        _timers.push_back( *it );    
}


SDL_Surface *Root::CreateScreenFmtSurface(uint32_t w, uint32_t h)
{
    if (w == 0 || h == 0)
        return NULL;
    
    SDL_DisplayMode curr;
    SDL_GetCurrentDisplayMode(0, &curr);
    curr.format = SDLWRAP_CorrectFormat(curr.format);
    
#if SDL_VERSION_ATLEAST(2,0,5)
    return SDL_CreateRGBSurfaceWithFormat(0, w, h, SDL_BITSPERPIXEL(curr.format), curr.format);
#else
    int bpp;
    Uint32 Rmask, Gmask, Bmask, Amask;
    SDL_PixelFormatEnumToMasks(curr.format, &bpp, &Rmask, &Gmask, &Bmask, &Amask);
        
    return SDL_CreateRGBSurface(0, w, h, bpp, Rmask, Gmask, Bmask, Amask );
#endif
    
}

void Root::HwCompose()
{
    if (_hwRender && _normal.size())
    {
        glPushAttrib(GL_LIGHTING | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TRANSFORM_BIT | GL_TEXTURE_BIT | GL_TEXTURE_2D);

        glMatrixMode(GL_PROJECTION);
        glOrtho(0, _screenSize.x, _screenSize.y, 0, -1, 1);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glDisable(GL_LIGHTING);

        glDepthMask(GL_FALSE);
        glDisable(GL_DEPTH_TEST);

        glDisable(GL_LIGHTING);

        glEnable(GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //GL_ONE_MINUS_SRC_ALPHA

        if ( !_hwTex )
            glGenTextures(1, &_hwTex);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _hwTex);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        for(auto it = _normal.rbegin(); it != _normal.rend(); it++)
            HwRenderWidget(*it);


        glPopAttrib();
    }
}

void Root::HwRenderWidget(Widget *w)
{
    if (w && w->IsEnabled() && w->_alpha != 0 /*&& Common::Rect(_screenSize).IsIntersects(w->_rect)*/ && w->_hwSurface)
    {
        GLint format, fmtype;
        SDLWRAP_GL_mapFormat(w->_hwSurface->format->format, &format, &fmtype);
        
        SDL_LockSurface(w->_hwSurface);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w->_hwSurface->w, w->_hwSurface->h, 0, format, fmtype, w->_hwSurface->pixels);
        SDL_UnlockSurface(w->_hwSurface);
        
        glColor4ub(255, 255, 255, w->_alpha);

        glBegin(GL_QUADS);
        {
            glTexCoord2i(0, 0);
            glVertex3i(w->_rect.left, w->_rect.top, 0);
            glTexCoord2i(0, 1);
            glVertex3i(w->_rect.left, w->_rect.bottom, 0);
            glTexCoord2i(1, 1);
            glVertex3i(w->_rect.right, w->_rect.bottom, 0);
            glTexCoord2i(1, 0);
            glVertex3i(w->_rect.right, w->_rect.top, 0);
        }
        glEnd();
    }
}


WidgetList::iterator WidgetList::find(Widget *w)
{
    for(auto it = begin(); it != end(); it++)
    {
        if (*it == w)
            return it;
    }
    return end();
}

bool WidgetList::MoveToFirst(Widget *w)
{
    for(auto it = begin(); it != end(); it++)
    {
        if (*it == w)
        {
            if (it != begin())
            {
                erase(it); // Do not use it
                push_front(w);
            }
            return true;
        }
    }

    return false;
}

}
