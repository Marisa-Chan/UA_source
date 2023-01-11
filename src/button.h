#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "nucleas.h"
#include "inttypes.h"
#include "utils.h"
#include <vector>

class NC_STACK_button: public NC_STACK_nucleus, public ClickBox
{
//Structures
public:

    enum // button types
    {
        TYPE_BUTTON     = 1,
        TYPE_CHECKBX    = 2,
        TYPE_CAPTION    = 3,
        TYPE_RADIOBTN   = 4,
        TYPE_SLIDER     = 5
    };

    enum // button states
    {
        FLAG_PRESSED    = 1,
        FLAG_DISABLED   = 2,
        FLAG_DOWN       = 4,
        FLAG_DRAW       = 8,
        FLAG_BORDER     = 0x10,
        FLAG_CENTER     = 0x20,
        FLAG_TEXT       = 0x40,
        FLAG_NOPRESS    = 0x80,
        FLAG_RALIGN     = 0x100
    };


    struct Slider //slider
    {
        int16_t value; // current value
        int16_t max; // maximal value
        int16_t min; // minimal value

        int16_t field_6_;
        int16_t field_8_;

        int16_t pressPart;
        int16_t oldValue;
        int16_t scrDownX;
    };

    struct button_str2
    {
        int16_t xpos;
        int16_t ypos;
        int16_t width;
        int16_t height;
        std::string caption;
        std::string caption2;
        int16_t flags;
        int down_id;
        int up_id;
        int pressed_id;
        int16_t button_type;
        int16_t button_id;
        Slider *field_41C;
        int txt_r;
        int txt_g;
        int txt_b;
        uint8_t tileset_down;
        uint8_t tileset_up;
        uint8_t field_42E;
    };

    typedef std::vector<button_str2> WidgetArr;

    struct button_64_arg
    {
        int button_type;
        int xpos;
        int ypos;
        int width;
        int field_10;
        std::string caption;
        std::string caption2;
        int downCode;
        int upCode;
        int pressedCode;
        int button_id;
        int flags;
        Slider *field_34;
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

    struct button_arg76
    {
        int butID;
        int16_t xpos;
        int16_t ypos;
        int16_t width;
    };

    struct ResCode
    {
        uint16_t code;
        uint16_t btn;

        ResCode() : code(0), btn(0) {};
        ResCode(uint16_t _code) : code(_code), btn(0) {};
        ResCode(uint16_t _code, uint16_t _btn) : code(_code), btn(_btn) {};

        explicit operator bool() const
        {
            return code != 0 || btn != 0;
        }
    };

// Methods
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t Add(button_64_arg *arg);
    virtual size_t Remove(int butID);
    virtual size_t Enable(button_66arg *arg);
    virtual size_t Disable(button_66arg *arg);
    
    virtual void ShowScreen(); // Part of 68th method
    virtual void HideScreen(); // Part of 68th method
    
    virtual ResCode ProcessWidgetsEvents(TInputState *arg);
    virtual size_t Draw();
    bool SetText(int butID, const std::string &field_4, const std::string &field_8);
    bool SetText(int butID, const std::string &field_4);
    int GetIndexByID(int butid);
    virtual void SetState(button_66arg *arg);
    Slider * GetSliderData(int butid);
    size_t Refresh(int butid);
    virtual size_t setXYWidth(button_arg76 *arg);

    NC_STACK_button() {
        field_d8.reserve(ClickBox::RESERVED);
        buttons.reserve(ClickBox::RESERVED);
    };
    virtual ~NC_STACK_button() {};

    virtual const std::string ClassName() const {
        return __ClassName;
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
    virtual void setBTN_chars(const std::string &);

    virtual int getBTN_x();
    virtual int getBTN_y();
    virtual int getBTN_w();
    virtual int getBTN_h();

protected:
    void UpdateSlider(NC_STACK_button *btn, button_str2 *sbt);
    void button_func70__sub1(NC_STACK_button *btn, button_str2 *sbt, char **pbuf);
    void button_func70__sub0(NC_STACK_button *btn, button_str2 *sbt, char **pbuf);

    void UnsetRadioButtons();

    //Data
public:
    static constexpr const char * __ClassName = "button.class";

    WidgetArr field_d8;
    int16_t idd = 0;
    bool    visible = false;
    uint8_t field_19c = 0;
    uint8_t field_19D = 0;
    uint8_t field_19E = 0;
    uint8_t field_19F = 0;
    uint8_t field_1A0 = 0;
    uint8_t field_1A1 = 0;
    int16_t screen_width = 0;
    int16_t screen_height = 0;
};

#endif // BUTTON_H_INCLUDED
