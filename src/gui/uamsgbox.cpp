#include "uamsgbox.h"
#include "../engine_miles.h"
#include "../yw.h"


namespace Gui
{   
UAMessageBox::UAMessageBox(uint8_t type): UAWindow("", Common::PointRect(), 0) 
{
    Init(type);
}

UAMessageBox::UAMessageBox(Widget *parent, uint8_t type): UAWindow(parent, "", Common::PointRect(), 0) 
{
    Init(type);    
} 

void UAMessageBox::Init(uint8_t type)
{
    _flags |= FLAG_ENABLED;
    
    std::string tmp = get_lang_string(yw->getYW_localeStrings(), 2, "OK");
    
    _okBtn = new UATextButton(this, tmp, Common::PointRect());
    _okBtn->SetBtnFlags(UABaseButton::FLAG_CENTER);
    _okBtn->SetColor(_UAButtonTextColor);
    _okBtn->Tag = 1;
    
    tmp = get_lang_string(yw->getYW_localeStrings(), 3, "CANCEL");
    
    _cancelBtn = new UATextButton(this, tmp, Common::PointRect());
    _cancelBtn->SetBtnFlags(UABaseButton::FLAG_CENTER);
    _cancelBtn->SetColor(_UAButtonTextColor);
    _cancelBtn->Tag = 2;
    
    _okBtn->_fOnPress = OnBtnPressSound;
    _cancelBtn->_fOnPress = OnBtnPressSound;
    
    _okBtn->_fOnClick = OnBtnClick;
    _okBtn->_fOnClickData = this;
    _cancelBtn->_fOnClick = OnBtnClick;
    _cancelBtn->_fOnClickData = this;
    
    switch (type)
    {
        case TYPE_INGAME:
        default:
            type = TYPE_INGAME;
            break;
            
        case TYPE_INMENU:
            _TSET_U = TILESET_46DEFAULT;
            _TSET_M = TILESET_46DEFAULT;
            _TSET_D = TILESET_46DEFAULT;
            _GLYPH_MM = '{';
            break;
    }
    
    _type = type;
    
    Update();
}

void UAMessageBox::SetInform(bool onlyInform)
{
    _inform = onlyInform;
}

void UAMessageBox::Update()
{
    switch (_type)
    {
        default:
        case TYPE_INGAME:
        {
            Common::Point spc = GetSpace();
            MoveTo(Common::Point(spc.x * 0.234375, spc.y * 0.385417));
            Resize(Common::Point(spc.x * 0.53125, spc.y * 0.21875));
            
        }
        break;
        
        case TYPE_INMENU:
        {
            Common::Point spc = GetSpace();
            _rect.MoveTo(spc.x * 0.234375, spc.y * 0.385417);
            
            int hSize = spc.y * 0.21875;
            hSize -= (hSize % _UATiles[_TSET_D]->h) - 1;
            
            _rect.SetSize(spc.x * 0.53125 + _UATiles[_TSET_M]->GetWidth('}'), hSize);
            
            if (_inform)
            {
                _okBtn->ResizeWH(spc.x * 0.125, 0);
                _okBtn->MoveTo(ScreenCoordToClient(Common::Point(spc.x * 0.4375, spc.y * 0.53125)));
                
                _cancelBtn->SetEnable(false);
            }
            else
            {
                _okBtn->ResizeWH(spc.x * 0.125, 0);
                _okBtn->MoveTo(ScreenCoordToClient(Common::Point(spc.x * 0.25, spc.y * 0.53125)));
                
                _cancelBtn->SetEnable(true);
                _cancelBtn->ResizeWH(spc.x * 0.125, 0);
                _cancelBtn->MoveTo(ScreenCoordToClient(Common::Point(spc.x * 0.625, spc.y * 0.53125)));
            }
            
            _txtRect1 = Common::PointRect(ScreenCoordToWidget(Common::Point(spc.x * 0.25, spc.y * 0.4375)), spc.x * 0.5, _UATiles[_TSET_U]->h);
            _txtRect2 = Common::PointRect(ScreenCoordToWidget(Common::Point(spc.x * 0.25, spc.y * 0.46875)), spc.x * 0.5, _UATiles[_TSET_U]->h);
        }
        break;
    }

    UAWindow::Update();
}

void UAMessageBox::OnAttachDetach(bool attach)
{
    Update();
    
    UAWindow::OnAttachDetach(attach);
}

void UAMessageBox::Draw(SDL_Surface *surface, const Common::Rect &dirt)
{   
    switch (_type)
    {
        default:
        case TYPE_INGAME:
            UAWindow::Draw(surface, dirt);
            break;
        
        case TYPE_INMENU:
        {
            TileMap *tiles;
    
            // Draw window box
            // Top
            tiles   = _UATiles[_TSET_U];    

            tiles->Draw(surface, Common::Point(0, 0), _GLYPH_LU );

            Common::Rect out(tiles->GetWidth(_GLYPH_LU), 0, GetWidth() - tiles->GetWidth(_GLYPH_RU), tiles->h);
            tiles->FillColumn(surface, out, _GLYPH_MU);

            tiles->Draw(surface, Common::Point(out.right, 0), _GLYPH_RU );


            // Mid
            tiles   = _UATiles[_TSET_M];

            out = Common::Rect(0, tiles->h, tiles->GetWidth(_GLYPH_LM), GetHeight() - 1);
            tiles->Fill(surface, out, _GLYPH_LM);
            out.left = out.right;
            out.right = GetWidth() - tiles->GetWidth(_GLYPH_RM);
            tiles->FillColumn(surface, out, _GLYPH_MM);
            out.left = out.right;
            out.right = GetWidth();
            tiles->Fill(surface, out, _GLYPH_RM);

            // Bot
            tiles   = _UATiles[_TSET_D];

            Common::PointRect src = tiles->map[_GLYPH_LD];
            src.y += src.h - 1;
            src.h = 1;

            out.top = out.bottom;
            out.bottom += 1;
            out.left = 0;

            SDLWRAP::Draw(tiles->img->GetSwTex(), src, surface, out.Pos());

            src = tiles->map[_GLYPH_MD];
            src.y += src.h - 1;
            src.h = 1;
            src.w = 1;

            out = Common::Rect(tiles->GetWidth(_GLYPH_LD), out.top, GetWidth() - tiles->GetWidth(_GLYPH_RD), out.bottom);

            SDLWRAP::DrawFill(tiles->img->GetSwTex(), src, surface, out);

            src = tiles->map[_GLYPH_RD];
            src.y += src.h - 1;
            src.h = 1;

            SDLWRAP::Draw(tiles->img->GetSwTex(), src, surface, Common::Point(out.right, out.top));
            
            DrawText(surface, _txt1, TXTFLAG_CENTER, _UATextColor, _txtRect1);
            DrawText(surface, _txt2, TXTFLAG_CENTER, _UATextColor, _txtRect2);
        }
        break;
        
        
    }
}

void UAMessageBox::OnParentResize(Common::Point newSz)
{
    Update();
    UAWindow::OnParentResize(newSz);
}

void UAMessageBox::OnBtnPressSound(Widget *w, void *d)
{
    SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 3);
}

void UAMessageBox::OnBtnClick(Widget* w, void *d)
{
    UAMessageBox *mbox = (UAMessageBox *)d;
    UABaseButton *btn = (UABaseButton *)w;
    if (mbox && btn)
    {
        mbox->Result = btn->Tag;
        mbox->SetEnable(false);
    }
}

void UAMessageBox::SetTexts(const std::string &str1, const std::string &str2)
{
    _txt1 = str1;
    _txt2 = str2;
}



UABlockMsgBox::UABlockMsgBox(uint8_t type)
: UAEmpty()
{
    Init(type);
}

UABlockMsgBox::UABlockMsgBox(Widget *parent, uint8_t type)
: UAEmpty(parent)
{
    Init(type);
}

UABlockMsgBox::UABlockMsgBox(Common::PointRect xyw, uint8_t type)
: UAEmpty(xyw)
{
    Init(type);
}

UABlockMsgBox::UABlockMsgBox(Widget *parent, const Common::PointRect &xyw, uint8_t type)
: UAEmpty(parent, xyw)
{
    Init(type);
}

void UABlockMsgBox::Init(uint8_t type)
{
    _flags = Widget::FLAG_NODRAW;
    
    _msgBox = new UAMessageBox(this, type);
    _msgBox->_flagsWindow |= UAWindow::FLAG_WND_UNMOVE;
    _msgBox->SetEnable(true);
    
    _msgBox->_okBtn->_fOnClick = OnBtnClick;
    _msgBox->_okBtn->_fOnClickData = this;
    
    _msgBox->_cancelBtn->_fOnClick = OnBtnClick;
    _msgBox->_cancelBtn->_fOnClickData = this;
}

UAMessageBox *UABlockMsgBox::GetMsgBox()
{
    return _msgBox;
}

void UABlockMsgBox::OnBtnClick(Widget* w, void *d)
{
    UABlockMsgBox *mbox = (UABlockMsgBox *)d;
    UABaseButton *btn = (UABaseButton *)w;
    if (mbox && btn)
    {
        mbox->_msgBox->Result = btn->Tag;
        mbox->SetEnable(false);
    }
}

}