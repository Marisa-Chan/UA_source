#ifndef WINP_H_INCLUDED
#define WINP_H_INCLUDED

#include <map>
#include <queue>
#include "idev.h"
#include "utils.h"
#include "system/inpt.h"

class NC_STACK_winp: public NC_STACK_idev
{
public:
    enum 
    {
        MAXCHARQUEUE = 8,
        MAXJOYMAP    = 32,
        MAXJOYHAT    = 4,
    };

public:
    static constexpr const char * __ClassName = "winp.class";

public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    
    virtual bool GetState();
    virtual float GetSlider();
    
    virtual bool BindKey(const std::string &keyName);
    virtual void ResetSlider();

    NC_STACK_winp() {};
    virtual ~NC_STACK_winp() {};
    
    virtual const std::string ClassName() const {
        return __ClassName;
    };
  
public:
    static void InitFirst();
    
    static void OnMouseDown(Common::Point pos, int btn, int clkNum);
    static void OnMouseUp(Common::Point pos, int btn, int clkNum);
    static void OnMouseMove(Common::Point pos, Common::Point rel);
    
    static SDL_Haptic * GetJoyHaptic() { return _joyHaptic; };
    
    static void QueryKeyboard(TInputState *arg);
    static void QueryPointer(TClickBoxInf *arg);
    static bool HasFocus() { return true; };
    
    static Input::TQueryState GetQueryInterface()
    { return Input::TQueryState(__ClassName, QueryKeyboard, QueryPointer, HasFocus); };
    
protected:
    static void CheckJoy();
    static void KeyDown(int16_t vk);
    static void KeyUp(int16_t vk);
    static int InputWatch(void *, SDL_Event *event);
    
    static SDL_JoystickGUID sdlReadJoyGuid();
    static bool sdlGUIDcmp(SDL_JoystickGUID &gd1, SDL_JoystickGUID &gd2);
    static int sdlJoyAxis(SDL_Joystick* joystick, int axis);
    static void sdlJoyReadMapping(SDL_Joystick* joystick);
   

public:
    int16_t _bindedKey = -1;
    int32_t _sliderPos = 0;
    
    static std::map<int16_t, int16_t> KBDMapping;
    
protected:
    static int _kbdLastDown;
    static int _kbdLastHit;
    static std::queue<int32_t> _inputText;
    
    
    static bool _mLstate;
    static bool _mRstate;
    static bool _mMstate;
    static bool _mDBLstate;

    static int _mLUcnt, _mLDcnt;
    static int _mRUcnt, _mRDcnt;
    static int _mMUcnt, _mMDcnt;

    static Common::Point _mPos;
    static Common::Point _mMove;
    static Common::Point _mMoveQuery; // Read on event and null it on query

    static Common::Point _mLDpos;
    static Common::Point _mRDpos;
    static Common::Point _mMDpos;

    static Common::Point _mLUpos;
    static Common::Point _mRUpos;
    static Common::Point _mMUpos;
    
    
    static bool                 _joyEnable;
    static SDL_JoystickGUID     _joyWantGuid;

    static SDL_Joystick        *_joyHandle;
    static SDL_Haptic          *_joyHaptic;

    static std::array<int, MAXJOYMAP>  _joyAxisMap;
    static std::array<bool, MAXJOYMAP> _joyAxisMapInv;
    static std::array<int, MAXJOYMAP>  _joyButtonMap;
    static std::array<int, MAXJOYHAT>  _joyHatMap;

    static uint32_t             _joyButtonStates;
    static Common::Point        _joyPov;
    static Common::Point        _joyXYpos;
    static Common::Point        _joyZRZpos;
};

#endif // WINP_H_INCLUDED
