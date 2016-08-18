#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

#include "utils.h"
#include "nlist.h"

class NC_STACK_input;

struct ButtonBox
{
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};

struct ClickBox : public nnode
{
    int16_t xpos;
    int16_t ypos;
    int16_t btn_width;
    int16_t btn_height;
    int field_10;
    void *pobject;
    ButtonBox *buttons[48];
};

struct winp_131arg
{
    int flag;
    ClickBox *selected_btn;
    int selected_btnID;
    shortPoint move[3];
    shortPoint ldw_pos[3];
    shortPoint lup_pos[3];
};

struct struC5
{
    int period;
    uint8_t downed_key_2;
    uint8_t downed_key;
    uint8_t dword8;
    uint8_t chr;
    char field_8;
    int16_t field_9;
    int16_t field_B;
    int field_D;
    float sliders_vars[32];
    int but_flags;
    winp_131arg winp131arg;
};

class INPEngine
{
public:
    INPEngine(): input_class(NULL) {};
    int init();
    void deinit();

    void setWndMode(gfx_window *wnd);
    NC_STACK_input *getPInput();

    void sub_412D28(struC5 *a1);
    void AddClickBox(ClickBox *btn, int a2);
    void RemClickBox(ClickBox *btn);

private:
    NC_STACK_input *input_class;
};

extern INPEngine INPe;


#endif // ENGINE_INPUT_H_INCLUDED
