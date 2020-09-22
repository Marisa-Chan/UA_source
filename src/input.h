#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <string>
#include "nucleas.h"
#include "wintimer.h"
#include "winp.h"

namespace Input
{
    enum ITYPE
    {
        ITYPE_UNK       = 0,
        ITYPE_WIMP      = 1,
        ITYPE_TIMER     = 2,
        ITYPE_KBD       = 3,
        ITYPE_BUTTON    = 4,
        ITYPE_SLIDER    = 5
    };
    
    struct KeyInfo
    {
        int16_t KeyCode;
        const bool IsSlider;
        bool down;
        
        KeyInfo(int16_t k, bool s)
        : KeyCode(k)
        , IsSlider(s)
        , down(false)
        {}
    };
    
    enum KEYCODE
    {
        KC_NONE,
        KC_ESCAPE,
        KC_SPACE,
        KC_UP,
        KC_DOWN,
        KC_LEFT,
        KC_RIGHT,
        KC_F1,
        KC_F2,
        KC_F3,
        KC_F4,
        KC_F5,
        KC_F6,
        KC_F7,
        KC_F8,
        KC_F9,
        KC_F10,
        KC_F11,
        KC_F12,
        KC_BACKSPACE,
        KC_TAB,
        KC_CLEAR,
        KC_RETURN,
        KC_CTRL,
        KC_SHIFT,
        KC_ALT,
        KC_PAUSE,
        KC_PGUP,
        KC_PGDOWN,
        KC_END,
        KC_HOME,
        KC_SELECT,
        KC_EXECUTE,
        KC_SNAPSHOT,
        KC_INSERT,
        KC_DELETE,
        KC_HELP,
        KC_1,
        KC_2,
        KC_3,
        KC_4,
        KC_5,
        KC_6,
        KC_7,
        KC_8,
        KC_9,
        KC_0,
        KC_A,
        KC_B,
        KC_C,
        KC_D,
        KC_E,
        KC_F,
        KC_G,
        KC_H,
        KC_I,
        KC_J,
        KC_K,
        KC_L,
        KC_M,
        KC_N,
        KC_O,
        KC_P,
        KC_Q,
        KC_R,
        KC_S,
        KC_T,
        KC_U,
        KC_V,
        KC_W,
        KC_X,
        KC_Y,
        KC_Z,
        KC_NUM0,
        KC_NUM1,
        KC_NUM2,
        KC_NUM3,
        KC_NUM4,
        KC_NUM5,
        KC_NUM6,
        KC_NUM7,
        KC_NUM8,
        KC_NUM9,
        KC_NUMMUL,
        KC_NUMPLUS,
        KC_NUMDOT,
        KC_NUMMINUS,
        KC_NUMENTER,
        KC_NUMDIV,
        KC_EXTRA1,
        KC_EXTRA2,
        KC_EXTRA3,
        KC_EXTRA4,
        KC_EXTRA5,
        KC_EXTRA6,
        KC_EXTRA7,
        KC_EXTRA8,
        KC_EXTRA9,
        KC_EXTRA10,
        KC_EXTRA11,
        KC_EXTRA12,
        KC_EXTRA13,
        KC_EXTRA14,
        KC_EXTRA15,
        KC_EXTRA16,
        KC_EXTRA17,
        KC_EXTRA18,
        KC_LMB,
        KC_RMB,
        KC_MMB,
        KC_MOUSEX,
        KC_MOUSEY,
        KC_JOYB0,
        KC_JOYB1,
        KC_JOYB2,
        KC_JOYB3,
        KC_JOYB4,
        KC_JOYB5,
        KC_JOYB6,
        KC_JOYB7,
        KC_JOYX,
        KC_JOYY,
        KC_JOYTHROTTLE,
        KC_JOYHATX,
        KC_JOYHATY,
        KC_JOYRUDDER,
        
        
        KC_MAX
    };
    
    struct KeyMapName
    {
        const std::string Name;
        KEYCODE   ID;
    };
}

class NC_STACK_input;

struct InputNode
{
    enum FLAGS
    {
        FLAG_AND    = 1 << 0, // Not used?
        FLAG_OR     = 1 << 1,
        FLAG_INV    = 1 << 2,
        FLAG_FSLIDE = 1 << 3, // Force slider
    };
    
    NC_STACK_idev *DriverObj  = NULL;
    int Flags                 = 0;
    bool IsSlider             = false;
    std::string DriverName;
    std::string KeyName;
};

class NC_STACK_input: public NC_STACK_nucleus
{
public:
    typedef std::list<InputNode> InputNodeList;
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual bool SetInputExpression(bool slider, uint32_t index, const std::string &val);
    bool InitDriver(uint8_t type, const std::string &val);
    virtual void QueryInput(InputState *arg);

    virtual bool SetHotKey(uint16_t id, const std::string &keyname);

    virtual void wimp_addClickNodeFront(ClickBox *box);
    virtual void wimp_addClickNodeBack(ClickBox *box);
    virtual void wimp_remClickNode(ClickBox *box);
    virtual void ForceFeedback(uint8_t state, uint8_t effID, float p1 = 0.0, float p2 = 0.0, float p3 = 0.0, float p4 = 0.0);

    virtual void ResetSlider(int sldr);
    
    int16_t GetHotKeyID(int16_t keycode);
    int16_t GetHotKey(uint16_t id);

    NC_STACK_input() {};
    virtual ~NC_STACK_input() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_input();
    };

protected:
    static void FreeKNodes(InputNodeList *lst);
    static std::string ParseInputNodeNext(const std::string &inputStr, InputNodeList *lst);
    static void UpdateList(InputNodeList *lst, bool *btn = NULL, float *slider = NULL);
    
    int16_t CheckHotKey(int16_t key);

//Data
public:
    static const Nucleus::ClassDescr description;

    NC_STACK_itimer *_timer    = NULL;
    NC_STACK_iwimp * _wimp     = NULL;
    NC_STACK_idev *  _keyboard = NULL;
    std::array<InputNodeList, 32> _buttons;
    std::array<InputNodeList, 32> _sliders;
    std::array<int16_t, 48>       _hotKeys;
    
    static std::array<Input::KeyInfo, Input::KC_MAX>    KeyMatrix;
    static std::array<std::string, Input::KC_MAX>       KeyTitle;
    static std::array<Input::KeyMapName, Input::KC_MAX> KeyNamesTable;
    static std::vector<Input::KeyMapName>                KeyAltNamesTable;
    
    static int16_t GetKeyIDByName(const std::string &name);
};

#endif // INPUT_H_INCLUDED
