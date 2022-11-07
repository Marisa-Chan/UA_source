#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

#include <vector>
#include "../utils.h"
#include "common/common.h"
#include "common/bitman.h"

class NC_STACK_input;

class ButtonBox
{
public:
    int16_t x = 0;
    int16_t y = 0;
    int16_t w = 0;
    int16_t h = 0;

    ButtonBox() = default;
    ButtonBox(int16_t _x, int16_t _y, int16_t _w, int16_t _h) 
    : x(_x), y(_y), w(_w), h(_h) {};

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

    void *pobject = NULL;
    std::vector<ButtonBox> buttons;
};

struct TMousePos
{
    Common::Point ScreenPos;
    Common::Point BoxPos;
    Common::Point BtnPos;
};

struct TClickBoxInf
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

    int flag = 0;
    ClickBox *selected_btn = NULL;
    uint32_t selected_btnID = 0;
    TMousePos move;
    TMousePos ldw_pos;
    TMousePos lup_pos;
};

struct TInputState
{
    uint32_t Period = 0;
    int16_t  KbdLastDown = 0;
    int16_t  KbdLastHit = 0;
    int16_t  HotKeyID = 0;
    uint8_t  chr = 0;

    std::array<float, 32> Sliders = {0.0};
    Common::BitMan<32> Buttons;
    TClickBoxInf ClickInf;
};

class INPEngine
{
public:
    INPEngine(): input_class(NULL) {};
    int init();
    void deinit();

    NC_STACK_input *GetInput();

    void QueryInput(TInputState *a1);
    void AddClickBoxFront(ClickBox *box);
    void AddClickBoxBack(ClickBox *box);

    void RemClickBox(ClickBox *box);

private:
    NC_STACK_input *input_class;
};

extern INPEngine INPe;


#endif // ENGINE_INPUT_H_INCLUDED
