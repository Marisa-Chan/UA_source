#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

extern class_stored input_engine_off;

#include "utils.h"

struct button_str1
{
    int16_t xpos;
    int16_t ypos;
    int16_t width;
    int16_t fnt_height;
};

struct inp_node : public nnode
{
    int16_t btn_xpos;
    int16_t btn_ypos;
    int16_t btn_width;
    int16_t btn_height;
    int field_10;
    void *pobject;
    button_str1 *field_18[48];
};

struct winp_131arg
{
    int flag;
    inp_node *selected_btn;
    int selected_btnID;
    shortPoint move[3];
    shortPoint ldw_pos[3];
    shortPoint lup_pos[3];
};

struct struC5
{
    int period;
    char downed_key_2;
    char downed_key;
    char dword8;
    char chr;
    char field_8;
    int16_t field_9;
    int16_t field_B;
    int field_D;
    float sliders_vars[32];
    int but_flags;
    winp_131arg winp131arg;
};

void sub_412D28(struC5 *a1);
void sub_412D48(inp_node *btn, int a2);
void sub_412D9C(inp_node *btn);

void inputEngine__getter(unsigned int, ...);
void inputEngine__setter(unsigned int a1, ...);


#endif // ENGINE_INPUT_H_INCLUDED
