#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

#include <vector>
#include "utils.h"
#include "common.h"

class NC_STACK_input;

class ButtonBox
{
public:
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;

    ButtonBox() : x(0), y(0), w(0), h(0) {};
    ButtonBox(int16_t _x, int16_t _y, int16_t _w, int16_t _h) : x(_x), y(_y), w(_w), h(_h) {};

    explicit operator bool() const
    {
        return w != 0 && h != 0;
    }
};

class ClickBox : public ButtonBox
{
public:
    enum
    {
        RESERVED    = 48
    };

    void *pobject;
    std::vector<ButtonBox> buttons;

    ClickBox() : ButtonBox(), pobject(NULL) {};
};

struct MousePos
{
    Common::Point ScreenPos;
    Common::Point BoxPos;
    Common::Point BtnPos;
    
    void clear()
    {
        ScreenPos = Common::Point();
        BoxPos = Common::Point();
        BtnPos = Common::Point();
    }
};

struct ClickBoxInf
{
    enum
    {
        FLAG_OK        = 1,
        FLAG_LM_DOWN   = 2,
        FLAG_LM_HOLD   = 4,
        FLAG_LM_UP     = 8,
        FLAG_BTN_DOWN  = 0x10,
        FLAG_BTN_HOLD  = 0x20,
        FLAG_BTN_UP    = 0x40,
        FLAG_RM_DOWN   = 0x80,
        FLAG_RM_HOLD   = 0x100,
        FLAG_RM_UP     = 0x200,
        FLAG_MM_DOWN   = 0x400,
        FLAG_MM_HOLD   = 0x800,
        FLAG_MM_UP     = 0x1000,
        FLAG_DBL_CLICK = 0x2000
    };

    int flag;
    ClickBox *selected_btn;
    int selected_btnID;
    MousePos move;
    MousePos ldw_pos;
    MousePos lup_pos;
    
    void clear()
    {
        flag = 0;
        selected_btn = NULL;
        selected_btnID = 0;
        move.clear();
        ldw_pos.clear();
        lup_pos.clear();
    }
};

struct InputState
{
    uint32_t Period;
    int16_t  KbdLastDown;
    int16_t  KbdLastHit;
    int16_t  HotKeyID;
    uint8_t  chr;

    float Sliders[32];
    Common::BitMan Buttons;
    ClickBoxInf ClickInf;
    
    
    void Clear();
};

class INPEngine
{
public:
    INPEngine(): input_class(NULL) {};
    int init();
    void deinit();

    NC_STACK_input *getPInput();

    void QueryInput(InputState *a1);
    void AddClickBoxFront(ClickBox *box);
    void AddClickBoxBack(ClickBox *box);

    void RemClickBox(ClickBox *box);

private:
    NC_STACK_input *input_class;
};

extern INPEngine INPe;


#endif // ENGINE_INPUT_H_INCLUDED
