#include "uabutton.h"

namespace Gui
{

void UABaseButton::MouseMove(Common::Point pos, Common::Point scrPos, int button)
{
    if ( !(_btnFlags & FLAG_LOCKED) )
    {
        _btnFlags &= ~FLAG_PRESSED;

        if ( IsLOn(pos) && button & MICE_LEFT)
            _btnFlags |= FLAG_PRESSED;
    }
    
    Widget::MouseMove(pos, scrPos, button);
}

void UABaseButton::MouseDown(Common::Point pos, Common::Point scrPos, int button)
{
    if ( !(_btnFlags & FLAG_LOCKED) )
    {
        _btnFlags &= ~FLAG_PRESSED;

        if ( IsLOn(pos) && button & MICE_LEFT)
            _btnFlags |= FLAG_PRESSED;
    }
    
    Widget::MouseDown(pos, scrPos, button);
}

void UABaseButton::MouseUp(Common::Point pos, Common::Point scrPos, int button)
{
    if ( !(_btnFlags & FLAG_LOCKED) )
    {
        if ( button & MICE_LEFT)
        {
            if ((_btnFlags & FLAG_PRESSED) && _fOnClick)
                _fOnClick(this, _fOnClickData);                
                
            _btnFlags &= ~FLAG_PRESSED;
        }
    }
    
    Widget::MouseUp(pos, scrPos, button);
}



UATextButton::UATextButton()
: UABaseButton()
{
    Init();
}

UATextButton::UATextButton(Widget *parent)
: UABaseButton(parent)
{
    Init();
}

UATextButton::UATextButton(Widget *parent, const std::string &txt, const Common::PointRect &xyw)
: UABaseButton(parent)
{
    Init();
    
    _flags |= FLAG_ENABLED;
    
    _text = txt;
    _rect = xyw;   
    _rect.SetSize( FixSizes(_rect.Size()) );
}

void UATextButton::Init()
{
    _color.a = 255;
    _color.r = 255;
    _color.g = 255;
    _color.b = 255;
}

void UATextButton::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{
    int setID = _tilesetUp;
    if ( _btnFlags & FLAG_LOCKED )
        setID = _tilesetLock;
    else if ( (_btnFlags & FLAG_PRESSED) && !(_btnFlags & FLAG_TOUCH) )
        setID = _tilesetDown;

    TileMap *tiles = _UATiles[setID];
    if (!tiles)
        return;

    Common::PointRect txtRect(0, 0, _rect.Width(), tiles->h);
    
    if ( !(_btnFlags & FLAG_NOBORDERS) ) // Draw borders
    {
        tiles->Draw(surface, Common::Point(), _lTile);
        tiles->Draw(surface, Common::Point(txtRect.w - tiles->map[_rTile].w, 0), _rTile);
            
        txtRect.w -= tiles->map[_lTile].w + tiles->map[_rTile].w;
        txtRect.x += tiles->map[_lTile].w;
    }
    
    tiles->Fill(surface, txtRect, _mTile); // Center part

    int tflags = 0;
    if ( (_btnFlags & FLAG_PRESSED) && !(_btnFlags & FLAG_TOUCH) )
        tflags = TXTFLAG_PRESSED;

    if ( _btnFlags & FLAG_CENTER )
        tflags |= TXTFLAG_CENTER;
    else if ( _btnFlags & FLAG_RALIGN )
        tflags |= TXTFLAG_RIGHTALIGN;

    DrawText(surface, _text, tflags, _color, txtRect);
}

Common::Point UATextButton::FixSizes(Common::Point sz)
{
    if (_UATiles[_tilesetUp])
    {
        TileMap *tiles = _UATiles[_tilesetUp];
        sz.y = tiles->h;
        
        int minW = tiles->map[_mTile].w;
        if (!(_btnFlags & FLAG_NOBORDERS))
            minW += tiles->map[_lTile].w + tiles->map[_rTile].w;
        
        if (sz.x < minW)
            sz.x = minW;
    }
    return sz;
}

void UATextButton::Resize(Common::Point sz)
{
    Widget::Resize( FixSizes(sz) );
}

void UATextButton::SetTileSets(int up, int down, int lock) 
{ 
    _tilesetUp = up; 
    _tilesetDown = down; 
    _tilesetLock = lock;
}



UATileButton::UATileButton()
: UABaseButton()
{}

UATileButton::UATileButton(Widget *parent)
: UABaseButton(parent)
{}

UATileButton::UATileButton(Widget *parent, const std::string &upTiles, const Common::PointRect &xyw, int up, int down)
: UABaseButton(parent)
{
    _flags |= FLAG_ENABLED;
    
    _upTiles = upTiles;
    _downTiles = upTiles;
    _rect = xyw;
    _upTilesSet = up; 
    _downTilesSet = down; 

    _rect.SetSize( FixSizes(_rect.Size()) );
}

UATileButton::UATileButton(Widget *parent, const std::string &upTiles, const std::string &downTiles, const Common::PointRect &xyw, int up, int down)
: UABaseButton(parent)
{
    _flags |= FLAG_ENABLED;
    
    _upTiles = upTiles;
    _downTiles = downTiles;
    _rect = xyw;
    _upTilesSet = up; 
    _downTilesSet = down; 
    
    _rect.SetSize( FixSizes(_rect.Size()) );
}

void UATileButton::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{
    int setID = _tilesetUp;
    if ( _btnFlags & FLAG_LOCKED )
        setID = _tilesetLock;
    else if ( (_btnFlags & FLAG_PRESSED) && !(_btnFlags & FLAG_TOUCH) )
        setID = _tilesetDown;

    TileMap *tiles = _UATiles[setID];
    if (!tiles)
        return;
    
    std::string *tileStr;
    TileMap *tilesStrSet;
    if ( (_btnFlags & FLAG_PRESSED) && !(_btnFlags & FLAG_TOUCH) )
    {
        tileStr = &_downTiles;
        tilesStrSet = _UATiles[_downTilesSet];
    }
    else
    {
        tileStr = &_upTiles;
        tilesStrSet = _UATiles[_upTilesSet];
    }
    
    int usedWidth = 0;
    for ( uint8_t t: *tileStr )
        usedWidth += tilesStrSet->map[ t ].w;
    
    int dx = 0;
    int maxW = _rect.Width();
    int freeSpace = maxW - usedWidth;
    if ( !(_btnFlags & FLAG_NOBORDERS) ) // Draw borders
    {
        freeSpace -= tiles->map[ _lTile ].w + tiles->map[ _rTile ].w;
        dx += tiles->map[ _lTile ].w;
    }
    
    if ( freeSpace > 0 )
    {
        if (_btnFlags & FLAG_CENTER)
        {
            tiles->Fill(surface, Common::PointRect(dx, 0, freeSpace / 2, tiles->h), _mTile);
            dx += freeSpace / 2;
            freeSpace /= 2;
        }
        else if (_btnFlags & FLAG_RALIGN)
        {
            tiles->Fill(surface, Common::PointRect(dx, 0, freeSpace, tiles->h), _mTile);
            dx += freeSpace;
            freeSpace = 0;
        }
    }
    
    for ( uint8_t t: *tileStr )
    {
        if (dx >= maxW)
            break;
        Common::Rect dstRect(dx, 0, maxW, tiles->h);
        
        tilesStrSet->Draw(surface, dstRect, t);
        dx += tilesStrSet->map[ t ].w;
    }
    
    if ( freeSpace > 0 )
       tiles->Fill(surface, Common::PointRect(dx, 0, freeSpace, tiles->h), _mTile);

    if ( !(_btnFlags & FLAG_NOBORDERS) ) // Draw borders
    {
        tiles->Draw(surface, Common::Point(), _lTile);
        tiles->Draw(surface, Common::Point(maxW - tiles->map[_rTile].w, 0), _rTile);
    }
}

void UATileButton::Resize(Common::Point sz)
{
    if (_UATiles[_tilesetUp])
        sz.y = _UATiles[_tilesetUp]->h;
    
    Widget::Resize(sz);
}

void UATileButton::SetTileSets(int up, int down, int lock) 
{ 
    _tilesetUp = up; 
    _tilesetDown = down; 
    _tilesetLock = lock;
}

Common::Point UATileButton::FixSizes(Common::Point sz)
{
    TileMap *tiles = _UATiles[_tilesetUp];
    TileMap *valtiles = _UATiles[_upTilesSet];
    sz.y = tiles->h;

    int minW = 0;
    for (uint8_t t: _upTiles)
        minW += valtiles->map[t].w;
        
    if (!(_btnFlags & FLAG_NOBORDERS))
        minW += tiles->map[_lTile].w + tiles->map[_rTile].w;

    if (sz.x < minW)
        sz.x = minW;
    return sz;
}


UAOneTileButton::UAOneTileButton(uint8_t upTile, const Common::PointRect &xyw, int upSet, int downSet)
: UABaseButton()
{
    Init(upTile, upTile, xyw, upSet, downSet);
}

UAOneTileButton::UAOneTileButton(uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet)
: UABaseButton()
{
    Init(upTile, downTile, xyw, upSet, downSet);
}

UAOneTileButton::UAOneTileButton(Widget *parent, uint8_t upTile, const Common::PointRect &xyw, int upSet, int downSet)
: UABaseButton(parent)
{
    Init(upTile, upTile, xyw, upSet, downSet);
}

UAOneTileButton::UAOneTileButton(Widget *parent, uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet)
: UABaseButton(parent)
{
    Init(upTile, downTile, xyw, upSet, downSet);
}

void UAOneTileButton::Init(uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet)
{
    _flags |= FLAG_ENABLED;
    
    _upTile = upTile;
    _downTile = downTile;
    _rect = xyw;
    _upTileSet = upSet; 
    _downTileSet = downSet; 
    
    _rect.SetSize( _UATiles[_upTileSet]->map[_upTile].Size() );
}
        
void UAOneTileButton::Resize(Common::Point sz)
{
    Widget::Resize(_UATiles[_upTileSet]->map[_upTile].Size());
}

void UAOneTileButton::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{
    if ( (_btnFlags & FLAG_PRESSED) && !(_btnFlags & FLAG_TOUCH) )
        _UATiles[_downTileSet]->Draw(surface, Common::Point(), _downTile);
    else
        _UATiles[_upTileSet]->Draw(surface, Common::Point(), _upTile);
}



}
