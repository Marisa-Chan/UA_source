#ifndef WINP_H_INCLUDED
#define WINP_H_INCLUDED

#include <map>
#include <queue>
#include "iwimp.h"
#include "utils.h"
#include "inp_ff.h"

class NC_STACK_winp: public NC_STACK_iwimp
{
public:
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
    
    enum 
    {
        MAXCHARQUEUE = 8,
        MAXJOYMAP    = 32,
        MAXJOYHAT    = 4,
    };

public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual bool GetState();
    virtual float GetSlider();
    virtual void QueryKeyboard(InputState *arg);
    virtual bool BindKey(const std::string &keyName);
    virtual void ResetSlider();
    virtual void ForceFeedBack(uint8_t state, uint8_t effID, float p1 = 0.0, float p2 = 0.0, float p3 = 0.0, float p4 = 0.0);
    virtual void CheckClick(ClickBoxInf *arg);

    NC_STACK_winp() {};
    virtual ~NC_STACK_winp() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_winp();
    };
    static void initfirst();
    
protected:
    static void CheckJoy();
    static void KeyDown(int16_t vk);
    static void KeyUp(int16_t vk);
    static int InputWatch(void *, SDL_Event *event);
    static SDL_JoystickGUID sdlReadJoyGuid();
    static bool sdlGUIDcmp(SDL_JoystickGUID &gd1, SDL_JoystickGUID &gd2);
    static int sdlJoyAxis(SDL_Joystick* joystick, int axis);
    static void sdlJoyReadMapping(SDL_Joystick* joystick);
    
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
    //Data
    static const Nucleus::ClassDescr description;
    static std::map<int16_t, int16_t> KBDMapping;
    
    int16_t _bindedKey = -1;
    int32_t _sliderPos = 0;
    
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
    
    static Input::FF::TankEngine     _ffTankEngine;
    static Input::FF::JetEngine      _ffJetEngine;
    static Input::FF::CopterEngine   _ffCopterEngine;
    static Input::FF::RotationDamper _ffRotDamper;
    static Input::FF::MiniGun        _ffMGun;
    static Input::FF::MissileFire    _ffMissFire;
    static Input::FF::GrenadeFire    _ffGrenadeFire;
    static Input::FF::BombFire       _ffBombFire;
    static Input::FF::Collision      _ffCollide;
    static Input::FF::Shake          _ffShake;
};

#endif // WINP_H_INCLUDED
