#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "nucleas.h"
#include "inttypes.h"
#include "utils.h"
#include "input.h"

struct button_str2_t2 //slider
{
    int16_t field_0; // current value
    int16_t field_2; // maximal value
    int16_t field_4; // minimal value
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

struct __NC_STACK_button : public ClickBox
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


class NC_STACK_button: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);
    virtual size_t button_func64(button_64_arg *arg);
    virtual size_t button_func65(int *butID);
    virtual size_t button_func66(button_66arg *arg);
    virtual size_t button_func67(button_66arg *arg);
    virtual size_t button_func68(int *arg);
    virtual size_t button_func69(struC5 *arg);
    virtual size_t button_func70(void *);
    virtual size_t button_func71(button_71arg *arg);
    virtual int button_func72(int *butid);
    virtual size_t button_func73(button_66arg *arg);
    virtual button_str2_t2 * button_func74(int *butid);
    virtual size_t button_func75(int *butid);
    virtual size_t button_func76(button_arg76 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_button() {
        memset(&stack__button, 0, sizeof(stack__button));
    };
    virtual ~NC_STACK_button() {};

    virtual const char * getClassName() {
        return "button.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_button();
    };

    enum BTN_ATT
    {
        BTN_ATT_X = 0x80001003,
        BTN_ATT_Y = 0x80001004,
        BTN_ATT_W = 0x80001005,
        BTN_ATT_H = 0x80001006,
        BTN_ATT_PBTN = 0x80001007,
        BTN_ATT_CHARS = 0x80001008
    };

    virtual void setBTN_x(int);
    virtual void setBTN_y(int);
    virtual void setBTN_w(int);
    virtual void setBTN_h(int);
    virtual void setBTN_chars(const char *);

    virtual int getBTN_x();
    virtual int getBTN_y();
    virtual int getBTN_w();
    virtual int getBTN_h();
    virtual __NC_STACK_button *getBTN_pButton();

    //Data
    static const NewClassDescr description;

    __NC_STACK_button stack__button;
};

#endif // BUTTON_H_INCLUDED
