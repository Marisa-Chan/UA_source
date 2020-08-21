#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#include <string>
#include "nucleas.h"
#include "nlist.h"
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
    
    enum KEY
    {
        KEY_NONE,
        KEY_ESCAPE,
        KEY_SPACE,
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        KEY_F1,
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5,
        KEY_F6,
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_F11,
        KEY_F12,
        KEY_BACKSPACE,
        KEY_TAB,
        KEY_CLEAR,
        KEY_RETURN,
        KEY_CTRL,
        KEY_SHIFT,
        KEY_ALT,
        KEY_PAUSE,
        KEY_PGUP,
        KEY_PGDOWN,
        KEY_END,
        KEY_HOME,
        KEY_SELECT,
        KEY_EXECUTE,
        KEY_SNAPSHOT,
        KEY_INSERT,
        KEY_DELETE,
        KEY_HELP,
        KEY_1,
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,
        KEY_A,
        KEY_B,
        KEY_C,
        KEY_D,
        KEY_E,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_I,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_M,
        KEY_N,
        KEY_O,
        KEY_P,
        KEY_Q,
        KEY_R,
        KEY_S,
        KEY_T,
        KEY_U,
        KEY_V,
        KEY_W,
        KEY_X,
        KEY_Y,
        KEY_Z,
        KEY_NUM0,
        KEY_NUM1,
        KEY_NUM2,
        KEY_NUM3,
        KEY_NUM4,
        KEY_NUM5,
        KEY_NUM6,
        KEY_NUM7,
        KEY_NUM8,
        KEY_NUM9,
        KEY_NUMMUL,
        KEY_NUMPLUS,
        KEY_NUMDOT,
        KEY_NUMMINUS,
        KEY_NUMENTER,
        KEY_NUMDIV,
        KEY_EXTRA1,
        KEY_EXTRA2,
        KEY_EXTRA3,
        KEY_EXTRA4,
        KEY_EXTRA5,
        KEY_EXTRA6,
        KEY_EXTRA7,
        KEY_EXTRA8,
        KEY_EXTRA9,
        KEY_EXTRA10,
        KEY_EXTRA11,
        KEY_EXTRA12,
        KEY_EXTRA13,
        KEY_EXTRA14,
        KEY_EXTRA15,
        KEY_EXTRA16,
        KEY_EXTRA17,
        KEY_EXTRA18,
        KEY_LMB,
        KEY_RMB,
        KEY_MMB,
        KEY_MOUSEX,
        KEY_MOUSEY,
        KEY_JOYB0,
        KEY_JOYB1,
        KEY_JOYB2,
        KEY_JOYB3,
        KEY_JOYB4,
        KEY_JOYB5,
        KEY_JOYB6,
        KEY_JOYB7,
        KEY_JOYX,
        KEY_JOYY,
        KEY_JOYTHROTTLE,
        KEY_JOYHATX,
        KEY_JOYHATY,
        KEY_JOYRUDDER,
        
        
        KEY_MAX
    };
    
    struct KeyMapName
    {
        const std::string Name;
        KEY   ID;
    };
};

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

    virtual size_t compatcall(int method_id, void *data);
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
    
    static std::array<Input::KeyInfo, Input::KEY_MAX>    KeyMatrix;
    static std::array<std::string, Input::KEY_MAX>       KeyTitle;
    static std::array<Input::KeyMapName, Input::KEY_MAX> KeyNamesTable;
    static std::vector<Input::KeyMapName>                KeyAltNamesTable;
    
    static int16_t GetKeyIDByName(const std::string &name);
};

#endif // INPUT_H_INCLUDED
