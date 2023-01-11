#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

#include <string>
#include <functional>
#include <vector>
#include "utils.h"
#include "common/common.h"
#include "common/bitman.h"
#include "inp_ff.h"
#include "itimer.h"
#include "idev.h"


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
    int32_t selected_btnID = 0;
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

    std::array<float, 32> Sliders = Common::ArrayInit<float, 32>(0.0);
    Common::BitMan<32> Buttons;
    TClickBoxInf ClickInf;
};


namespace Input
{
    enum ITYPE
    {
        ITYPE_UNK       = 0,
        ITYPE_WIMP      = 1,
        ITYPE_TIMER     = 2,
        ITYPE_KBD       = 3
    };
    
    struct KeyInfo
    {
        int16_t KeyCode;
        const bool IsSlider = false;
        bool down = false;
        
        KeyInfo(int16_t k, bool s = false)
        : KeyCode(k)
        , IsSlider(s)
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
    
    enum FF_TYPE
    {
        FF_TYPE_ALL = 0,
        FF_TYPE_TANKENGINE = 1,
        FF_TYPE_JETENGINE = 2,
        FF_TYPE_HELIENGINE = 3,
        FF_TYPE_ROTDAMPER = 4,
        FF_TYPE_MINIGUN = 5,
        FF_TYPE_MISSILEFIRE = 6,
        FF_TYPE_GRENADEFIRE = 7,
        FF_TYPE_BOMBFIRE = 8,
        FF_TYPE_COLLISION = 9,
        FF_TYPE_SHAKE = 10
    };

    enum FF_STATE
    {
        FF_STATE_START = 0,
        FF_STATE_STOP = 1,
        FF_STATE_UPDATE = 2
    };
    
    struct TQueryState
    {
        std::string ClassName;
        std::function<void(TInputState *arg)> KeyboardQuery;
        std::function<void(TClickBoxInf *arg)> PointerQuery;
        std::function<bool()> HasFocus;
        
        TQueryState() = default;
        TQueryState(TQueryState&&) = default;
        TQueryState(const TQueryState&) = default;
        TQueryState & operator=(const TQueryState&) = default;
        
        TQueryState(const std::string &clsName, 
                    std::function<void(TInputState *arg)> kbd = NULL, 
                    std::function<void(TClickBoxInf *arg)> pnt = NULL,
                    std::function<bool()> fcs = NULL)
        : ClassName(clsName)
        , KeyboardQuery(kbd)
        , PointerQuery(pnt)
        , HasFocus(fcs)
        {}
    };
}

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


namespace Input {
class INPEngine
{
public:
    typedef std::list<InputNode> InputNodeList;
    
public:
    int Init();
    void Deinit();

    void QueryInput(TInputState *state);
    
    void AddClickBoxFront(ClickBox *box);
    void AddClickBoxBack(ClickBox *box);
    void RemClickBox(ClickBox *box);
    
    virtual bool SetInputExpression(bool slider, uint32_t index, const std::string &val);
    bool InitDriver(uint8_t type, const std::string &val);

    virtual bool SetHotKey(uint16_t id, const std::string &keyname);

    virtual void ForceFeedback(uint8_t state, uint8_t effID, float p1 = 0.0, float p2 = 0.0, float p3 = 0.0, float p4 = 0.0);

    virtual void ResetSlider(int sldr);
    
    int16_t GetHotKeyID(int16_t keycode);
    int16_t GetHotKey(uint16_t id);
    
    void RegisterInterface(const TQueryState& iface);
    
    static bool GetKeyState(uint16_t id)
    {
        return KeyMatrix.at(id).down;
    }
    
protected:
    static void FreeKNodes(InputNodeList *lst);
    static std::string ParseInputNodeNext(const std::string &inputStr, InputNodeList *lst);
    static void UpdateList(InputNodeList *lst, bool *btn = NULL, float *slider = NULL);
    
    int16_t CheckHotKey(int16_t key);
    
    TQueryState FindInterface(const std::string &name);    
    
    void FFstopAll();
    void FFDOTankEngine(int state, float p1, float p2);
    void FFDOJetEngine(int state, float p1, float p2);
    void FFDOHeliEngine(int state, float p1, float p2);
    void FFDORotDamper(int state, float p1);
    void FFDOMiniGun(int state);
    void FFDOMissileFire(int state);
    void FFDOGrenadeFire(int state);
    void FFDOBombFire(int state);
    void FFDOCollision(int state, float a2, float a3, float a4);
    void FFDOShake(int state, float a2, float a3, float a4, float a5);
    
public:
    static INPEngine Instance;
    
protected:
    std::vector<TQueryState> Interfaces;
    
public:
    NC_STACK_itimer *_timer    = NULL;
    TQueryState _wimp;
    TQueryState _keyboard;
    std::array<InputNodeList, 32> _buttons;
    std::array<InputNodeList, 32> _sliders;
    std::array<int16_t, 48>       _hotKeys;
    
    static std::array<KeyInfo, KC_MAX>      KeyMatrix;
    static std::array<std::string, KC_MAX>  KeyTitle;
    static std::array<KeyMapName, KC_MAX>   KeyNamesTable;
    static std::vector<KeyMapName>          KeyAltNamesTable;
    
    static int16_t GetKeyIDByName(const std::string &name);
    
    FF::TankEngine     _ffTankEngine;
    FF::JetEngine      _ffJetEngine;
    FF::CopterEngine   _ffCopterEngine;
    FF::RotationDamper _ffRotDamper;
    FF::MiniGun        _ffMGun;
    FF::MissileFire    _ffMissFire;
    FF::GrenadeFire    _ffGrenadeFire;
    FF::BombFire       _ffBombFire;
    FF::Collision      _ffCollide;
    FF::Shake          _ffShake;
    
    struct TClickCheck
    {
        void CheckClick(TClickBoxInf *arg);

        void GetClick(TClickBoxInf *click, TMousePos &mouse);

        std::list<ClickBox *> CBoxList;
        ClickBox *SelectedCbox = NULL;
        int32_t SelectedButton = -1;
        
    } ClickCheck;
};


static constexpr INPEngine &Engine = INPEngine::Instance;
}


#endif // ENGINE_INPUT_H_INCLUDED
