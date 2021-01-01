#ifndef GUI_UABUTTON_H_INCLUDED
#define GUI_UABUTTON_H_INCLUDED

#include "uacommon.h"

namespace Gui
{
class UABaseButton : public Widget, public UA
{
public:
    enum FLAG
    {
        FLAG_PRESSED   =  (1 << 16),
        FLAG_LOCKED    =  (1 << 17),
        FLAG_NOBORDERS =  (1 << 18),
        FLAG_TOUCH     =  (1 << 19),
        FLAG_CENTER    =  (1 << 20),
        FLAG_RALIGN    =  (1 << 21),
    };
    
public:
    UABaseButton(): Widget() {}
    UABaseButton(Widget *parent): Widget(parent) {}
    
    void SetBtnFlags(int flags) { _btnFlags |= flags; }
    void UnsetBtnFlags(int flags) { _btnFlags &= ~flags; }
    void AsignBtnFlags(int flags) { _btnFlags = flags; }
    
    virtual void MouseDown(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseUp(Common::Point pos, Common::Point scrPos, int button);
    virtual void MouseMove(Common::Point pos, Common::Point scrPos, int button);
    
protected:
    virtual void OnBtnPress();
    virtual void OnBtnClick();
    
public:
    int32_t    Tag          = 0;
    
    
    TFOnWidget  _fOnPress     = NULL;
    void *      _fOnPressData = NULL;
    
    TFOnWidget  _fOnClick     = NULL;
    void *      _fOnClickData = NULL;
    
protected:
    uint32_t _btnFlags = 0;
};



class UATextButton : public UABaseButton
{
public:
    UATextButton();
    UATextButton(Widget *parent);
    UATextButton(Widget *parent, const std::string &txt, const Common::PointRect &xyw);
    
    void SetText(const std::string &txt) { _text = txt; }
    void SetTileSets(int up, int down, int lock);    
    void SetColor(SDL_Color clr);
    
    virtual void Resize(Common::Point sz);

    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
protected:
    void Init();
    Common::Point FixSizes(Common::Point sz);

protected:
    std::string _text;
    SDL_Color   _color;
    
    int _tilesetUp   = TILESET_46MAPC16;
    int _tilesetDown = TILESET_46MAPC32;
    int _tilesetLock = TILESET_46ENERGY;
    uint8_t _lTile = 'a';
    uint8_t _mTile = ' ';
    uint8_t _rTile = 'c';
};



class UATileButton : public UABaseButton
{
public:
    UATileButton();
    UATileButton(Widget *parent);
    UATileButton(Widget *parent, const std::string &upTiles, const Common::PointRect &xyw, int up, int down);
    UATileButton(Widget *parent, const std::string &upTiles, const std::string &downTiles, const Common::PointRect &xyw, int up, int down);
    
    void SetTilesUp(const std::string &tiles, int tileset) { _upTiles = tiles; _upTilesSet = tileset;}
    void SetTilesDown(const std::string &tiles, int tileset) { _downTiles = tiles; _downTilesSet = tileset;}
    void SetTileSets(int up, int down, int lock);   
    
    virtual void Resize(Common::Point sz);
    using Widget::Resize;

    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
protected:
    Common::Point FixSizes(Common::Point sz);
    
protected:
    std::string _upTiles;
    std::string _downTiles;
    int _upTilesSet   = 0;
    int _downTilesSet = 0;
    
    int _tilesetUp   = TILESET_46MAPC16;
    int _tilesetDown = TILESET_46MAPC32;
    int _tilesetLock = TILESET_46ENERGY;
    uint8_t _lTile = 'a';
    uint8_t _mTile = ' ';
    uint8_t _rTile = 'c';
};



class UAOneTileButton : public UABaseButton
{
public:
    UAOneTileButton(uint8_t upTile, const Common::PointRect &xyw, int upSet, int downSet);
    UAOneTileButton(uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet);
    UAOneTileButton(Widget *parent, uint8_t upTile, const Common::PointRect &xyw, int upSet, int downSet);
    UAOneTileButton(Widget *parent, uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet);
    
    void SetTileUp(uint8_t tile, int tileset) { _upTile = tile; _upTileSet = tileset;}
    void SetTileDown(uint8_t tile, int tileset) { _downTile = tile; _downTileSet = tileset;}
    
    virtual void Resize(Common::Point sz);

    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
protected:
    void Init(uint8_t upTile, uint8_t downTile, const Common::PointRect &xyw, int upSet, int downSet);
protected:
    uint8_t _upTile;
    uint8_t _downTile;
    int _upTileSet   = 0;
    int _downTileSet = 0;
};

}


#endif
