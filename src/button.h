#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "nucleas.h"
#include "inttypes.h"
#include "utils.h"
#include "input.h"

extern class_stored button_class_off;

struct NC_STACK_button;


struct button_str2_t2
{
    int16_t field_0;
    int16_t field_2;
    int16_t field_4;
    int16_t field_6;
    int16_t field_8;
    int16_t field_A;
    int16_t field_C;
    int16_t field_E;
};

struct button_str2
{
    int16_t xpos;
    int16_t ypos;
    int16_t width;
    int16_t field_6;
    char caption[512];
    char caption2[512];
    int16_t state;
    int16_t field_40A;
    int down_id;
    int up_id;
    int pressed_id;
    int16_t button_type;
    int16_t button_id;
    button_str2_t2 *field_41C;
    int txt_r;
    int txt_g;
    int txt_b;
    uint8_t tileset_down;
    uint8_t tileset_up;
    uint8_t field_42E;
};

struct __NC_STACK_button : public inp_node
{
    button_str2 *field_d8[48];
    int16_t idd;
    int16_t field_19A;
    uint8_t field_19c;
    uint8_t field_19D;
    uint8_t field_19E;
    uint8_t field_19F;
    uint8_t field_1A0;
    uint8_t field_1A1;
    int16_t screen_width;
    int16_t screen_height;
};

struct NC_STACK_button : public NC_STACK_nucleus
{
    __NC_STACK_button stack__button;
};

struct button_64_arg
{
    int button_type;
    int xpos;
    int ypos;
    int width;
    int field_10;
    const char *caption;
    const char *caption2;
    int field_1C;
    int down_id;
    int up_id;
    int pressed_id;
    int button_id;
    int state;
    button_str2_t2 *field_34;
    uint8_t tileset_down;
    uint8_t tileset_up;
    uint8_t field_3A;
    int txt_r;
    int txt_g;
    int txt_b;
};

struct button_66arg
{
    int butID;
    int field_4;
};

struct button_71arg
{
    int butID;
    const char *field_4;
    char *field_8;
};

struct button_arg76
{
    int butID;
    int16_t xpos;
    int16_t ypos;
    int16_t width;
};

#endif // BUTTON_H_INCLUDED
