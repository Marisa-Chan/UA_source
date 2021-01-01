#ifndef GUI_UAMSGBOX_H_INCLUDED
#define GUI_UAMSGBOX_H_INCLUDED

#include "uacommon.h"
#include "uabutton.h"
#include "uawindow.h"
#include "uaempty.h"

namespace Gui
{
    
class UABlockMsgBox;
    
class UAMessageBox : public UAWindow
{
friend class UABlockMsgBox;

public:
    enum 
    {
        TYPE_INGAME = 0,
        TYPE_INMENU = 1,
    };

public:
    UAMessageBox(uint8_t type = TYPE_INGAME);
    UAMessageBox(Widget *parent, uint8_t type = TYPE_INGAME);
    
    virtual void SetInform(bool onlyInform);
    
    virtual void OnAttachDetach(bool attach);
    virtual void OnParentResize(Common::Point newSz);
    
    virtual void Draw(SDL_Surface *surface, const Common::Rect &dirt);
    
    virtual void SetTexts(const std::string &str1, const std::string &str2);

protected:
    Common::Point FixSizes(Common::Point sz);
    
    static void OnBtnPressSound(Widget *w, void *);
    static void OnBtnClick(Widget *w, void *);
    
    void Init(uint8_t type);
    
    virtual void Update();
    
public:
    
    uint8_t Result = 0;
    
protected:
    
    uint8_t _type = 0;
    bool    _inform = false;
    
    std::string   _txt1;
    std::string   _txt2;
    
    Common::PointRect _txtRect1;
    Common::PointRect _txtRect2;
    
    UATextButton *_okBtn   = NULL;
    UATextButton *_cancelBtn  = NULL;
};


class UABlockMsgBox : public UAEmpty
{
public:
    UABlockMsgBox(uint8_t type = UAMessageBox::TYPE_INGAME);
    UABlockMsgBox(Widget *parent, uint8_t type = UAMessageBox::TYPE_INGAME);
    UABlockMsgBox(Common::PointRect xyw, uint8_t type = UAMessageBox::TYPE_INGAME);
    UABlockMsgBox(Widget *parent, const Common::PointRect &xyw, uint8_t type = UAMessageBox::TYPE_INGAME);
    
    UAMessageBox *GetMsgBox();
    
protected:
    
    void Init(uint8_t type);
    
    static void OnBtnClick(Widget *w, void *);
    
protected:
    UAMessageBox *_msgBox;
};

}


#endif