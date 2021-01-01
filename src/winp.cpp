#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "engine_input.h"
#include "winp.h"
#include "utils.h"
#include "log.h"
#include "gui/widget.h"

const Nucleus::ClassDescr NC_STACK_winp::description("winp.class", &newinstance);

std::map<int16_t, int16_t> NC_STACK_winp::KBDMapping;

int NC_STACK_winp::_kbdLastDown;
int NC_STACK_winp::_kbdLastHit;
std::queue<int32_t> NC_STACK_winp::_inputText;

bool NC_STACK_winp::_mLstate;
bool NC_STACK_winp::_mRstate;
bool NC_STACK_winp::_mMstate;
bool NC_STACK_winp::_mDBLstate;

int NC_STACK_winp::_mLUcnt, NC_STACK_winp::_mLDcnt;
int NC_STACK_winp::_mRUcnt, NC_STACK_winp::_mRDcnt;
int NC_STACK_winp::_mMUcnt, NC_STACK_winp::_mMDcnt;

Common::Point NC_STACK_winp::_mPos;
Common::Point NC_STACK_winp::_mMove;
Common::Point NC_STACK_winp::_mMoveQuery;

Common::Point NC_STACK_winp::_mLDpos;
Common::Point NC_STACK_winp::_mRDpos;
Common::Point NC_STACK_winp::_mMDpos;

Common::Point NC_STACK_winp::_mLUpos;
Common::Point NC_STACK_winp::_mRUpos;
Common::Point NC_STACK_winp::_mMUpos;


bool                 NC_STACK_winp::_joyEnable = false;
SDL_JoystickGUID     NC_STACK_winp::_joyWantGuid;

SDL_Joystick        *NC_STACK_winp::_joyHandle = NULL;
SDL_Haptic          *NC_STACK_winp::_joyHaptic = NULL;

std::array<int, 32>  NC_STACK_winp::_joyAxisMap;
std::array<bool, 32> NC_STACK_winp::_joyAxisMapInv;
std::array<int, 32>  NC_STACK_winp::_joyButtonMap;
std::array<int, 4>   NC_STACK_winp::_joyHatMap;

uint32_t             NC_STACK_winp::_joyButtonStates;
Common::Point        NC_STACK_winp::_joyPov;
Common::Point        NC_STACK_winp::_joyXYpos;
Common::Point        NC_STACK_winp::_joyZRZpos;

Input::FF::TankEngine     NC_STACK_winp::_ffTankEngine;
Input::FF::JetEngine      NC_STACK_winp::_ffJetEngine;
Input::FF::CopterEngine   NC_STACK_winp::_ffCopterEngine;
Input::FF::RotationDamper NC_STACK_winp::_ffRotDamper;
Input::FF::MiniGun        NC_STACK_winp::_ffMGun;
Input::FF::MissileFire    NC_STACK_winp::_ffMissFire;
Input::FF::GrenadeFire    NC_STACK_winp::_ffGrenadeFire;
Input::FF::BombFire       NC_STACK_winp::_ffBombFire;
Input::FF::Collision      NC_STACK_winp::_ffCollide;
Input::FF::Shake          NC_STACK_winp::_ffShake;





void sdlInputResetLog()
{
    FSMgr::FileHandle *v0 = uaOpenFile("env/sdlInputLog.txt", "w");
    if ( v0 )
    {
        v0->printf("YPA SDL Input log\n---------------------\n");
        delete v0;
    }
}

void sdlInputLog(const char *format, ...)
{
    va_list va;
    va_start(va, format);
    FSMgr::FileHandle *f = uaOpenFile("env/sdlInputLog.txt", "a");
    if ( f )
    {
        f->vprintf(format, va);
        delete f;
    }
    va_end(va);
}

SDL_JoystickGUID NC_STACK_winp::sdlReadJoyGuid()
{
    SDL_JoystickGUID guid;

    FSMgr::FileHandle *f = uaOpenFile("env/sdlJoy.txt", "r");
    if ( f )
    {
        std::string buf;
        f->ReadLine(&buf);
        delete f;

        size_t endp = buf.find_first_of("\r\n");

        if (endp != std::string::npos)
            buf.erase(endp);

        guid = SDL_JoystickGetGUIDFromString(buf.c_str());
    }
    else
        std::memset(&guid, 0, sizeof(SDL_JoystickGUID));

    return guid;
}

bool NC_STACK_winp::sdlGUIDcmp(SDL_JoystickGUID &gd1, SDL_JoystickGUID &gd2)
{
    return std::memcmp(&gd1, &gd2, sizeof(SDL_JoystickGUID)) == 0;
}

int NC_STACK_winp::sdlJoyAxis(SDL_Joystick* joystick, int axis)
{
    int tmp = SDL_JoystickGetAxis(joystick, _joyAxisMap[axis]);

    if ( _joyAxisMapInv[axis] )
        tmp = -tmp;

    if (abs(tmp) <= 6553) // 20% deadzone
        return 0;

    tmp /= 109; //-32768/32767  to -300/300

    if (tmp > 300)
        tmp = 300;
    else if (tmp < -300)
        tmp = -300;

    return tmp;
}

void NC_STACK_winp::sdlJoyReadMapping(SDL_Joystick* joystick)
{
    char bf[64];
    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), bf, 64);
    std::string guid(bf);

    for (size_t i = 0; i < MAXJOYHAT; i++)
        _joyHatMap[i] = i;

    for (size_t i = 0; i < MAXJOYMAP; i++)
    {
        _joyAxisMap[i] = i;
        _joyAxisMapInv[i] = false;
        _joyButtonMap[i] = i;
    }

    FSMgr::FileHandle *f = uaOpenFile("joyconf.txt", "r");

    if ( f )
    {
        std::string buf;
        while ( f->ReadLine(&buf) )
        {
            std::vector<std::string> splt = Stok::Split(buf, " :\t\n\r");
            if (splt.size() >= 3 && !StriCmp(splt[0], bf))
            {
                for(size_t i = 0; i < ((splt.size() - 1) / 2); i++)
                {
                    std::string &tp = splt.at(1 + i * 2);
                    std::string &val = splt.at(1 + i * 2 + 1);
                    
                    if(tolower(tp[0]) == 'a')
                    {
                        int axis = std::stoi( tp.substr(1) );
                        int realaxis = std::stoi( val );

                        if ( axis < MAXJOYMAP && axis >= 0 && abs(realaxis) < MAXJOYMAP)
                        {
                            if ( realaxis < 0 )
                                _joyAxisMapInv[ axis ] = true;
                            
                            realaxis = abs(realaxis);
                            
                            _joyAxisMap[ axis ] = realaxis;
                            sdlInputLog("\tAxis map %c%d -> %d\n", (_joyAxisMapInv[ axis ] ? '-' : ' '), realaxis, axis);
                        }

                    }
                    else if (tolower(tp[0]) == 'b')
                    {
                        int btn = std::stoi( tp.substr(1) );
                        int realbtn = abs(  std::stoi( val )  );

                        if ( btn < MAXJOYMAP && btn >= 0 && realbtn < MAXJOYMAP)
                        {
                            _joyButtonMap[ btn ] = realbtn;
                            sdlInputLog("\tButton map %d -> %d\n", realbtn, btn);
                        }
                    }
                    else if (tolower(tp[0]) == 'h')
                    {
                        int hat = std::stoi( tp.substr(1) );
                        int realhat = abs(  std::stoi( val )  );

                        if ( hat < MAXJOYHAT && hat >= 0 && realhat < MAXJOYHAT)
                        {
                            _joyHatMap[ hat ] = realhat;
                            sdlInputLog("\tHat map %d -> %d\n", realhat, hat);
                        }
                    }
                }
                break;
            }
        }

        delete f;
    }
}

    
void NC_STACK_winp::KeyDown(int16_t vk)
{
    if ( vk != Input::KC_NONE )
        NC_STACK_input::KeyMatrix.at(vk).down = true;

    _kbdLastDown = vk;
    _kbdLastHit = vk;
}

void NC_STACK_winp::KeyUp(int16_t vk)
{
    if ( vk != Input::KC_NONE )
        NC_STACK_input::KeyMatrix.at(vk).down = false;

    if (_kbdLastDown == vk)
        _kbdLastDown = Input::KC_NONE;
}

int NC_STACK_winp::InputWatch(void *, SDL_Event *event)
{
    switch(event->type)
    {
    case SDL_KEYDOWN:
        {
            auto it = KBDMapping.find(event->key.keysym.scancode);
            if (it != KBDMapping.end())
                KeyDown(it->second);
        }
        break;

    case SDL_KEYUP:
        {
            auto it = KBDMapping.find(event->key.keysym.scancode);
            if (it != KBDMapping.end())
                KeyUp(it->second);
        }
        break;

    case SDL_TEXTINPUT:
    {
        for( char c : event->text.text)
        {
            if (!c || _inputText.size() >= MAXCHARQUEUE)
                break;
            
            _inputText.push(c);
        }
    }
    break;

    case SDL_MOUSEBUTTONDOWN:
    {
        Common::Point corrPos = SDLWRAP::MousePosNorm(Common::Point(event->button.x, event->button.y));
        if ( !Gui::Root::Instance.MouseDown(corrPos, event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                _mLstate = true;
                _mLDpos = corrPos;

                _mLDcnt++;

                KeyDown(Input::KC_LMB);
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                _mRstate = true;
                _mRDpos = corrPos;

                _mRDcnt++;

                KeyDown(Input::KC_RMB);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                _mMstate = true;
                _mMDpos = corrPos;

                _mMDcnt++;

                KeyDown(Input::KC_MMB);
            }
        }
    }
    break;

    case SDL_MOUSEBUTTONUP:
    {
        Common::Point corrPos = SDLWRAP::MousePosNorm(Common::Point(event->button.x, event->button.y));
        if ( !Gui::Root::Instance.MouseUp(corrPos, event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                _mLstate = false;
                _mLUpos = corrPos;

                _mLUcnt++;

                KeyUp(Input::KC_LMB);

                if ((event->button.clicks & 1) == 0)
                    _mDBLstate = true;
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                _mRstate = false;
                _mRUpos = corrPos;

                _mRUcnt++;

                KeyUp(Input::KC_RMB);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                _mMstate = false;
                _mMUpos = corrPos;

                _mMUcnt++;

                KeyUp(Input::KC_MMB);
            }
        }
    }
    break;

    case SDL_MOUSEMOTION:
    {
        Common::Point corrPos = SDLWRAP::MousePosNorm(Common::Point(event->button.x, event->button.y));
        if ( !Gui::Root::Instance.MouseMove(corrPos) )
        {
            _mPos = corrPos;
            _mMoveQuery = Common::Point(event->motion.xrel, event->motion.yrel);
            
        }
    }
    break;

    default:
        break;
    }

    return 1;
}




size_t NC_STACK_winp::func0(IDVList &stak)
{
    if ( !NC_STACK_iwimp::func0(stak) )
        return 0;

    _bindedKey = -1;

    return 1;
}

size_t NC_STACK_winp::func1()
{
    return NC_STACK_iwimp::func1();
}


bool NC_STACK_winp::GetState()
{
    if ( _bindedKey < 0 )
        return false;

    switch ( _bindedKey )
    {
    case Input::KC_LMB:
        return _mLstate;
        break;
    case Input::KC_RMB:
        return _mRstate;
        break;
    case Input::KC_MMB:
        return _mMstate;
        break;
    case Input::KC_JOYB0:
    case Input::KC_JOYB1:
    case Input::KC_JOYB2:
    case Input::KC_JOYB3:
    case Input::KC_JOYB4:
    case Input::KC_JOYB5:
    case Input::KC_JOYB6:
    case Input::KC_JOYB7:
        if ( _joyButtonStates & (1 << (_bindedKey - Input::KC_JOYB0)) )
            return true;
        else
            return false;
        break;
    default:
        return NC_STACK_input::KeyMatrix.at(_bindedKey).down;
    }
    
    return false;
}

float NC_STACK_winp::GetSlider()
{
    if ( _bindedKey < 0 )
        return 0.0;

    int v6 = 1;

    switch ( _bindedKey )
    {
        case Input::KC_MOUSEX:
        {
            _sliderPos += _mMove.x * 8;

            if ( _mMove.x )
                v6 = 0;
        }
        break;

        case Input::KC_MOUSEY:
        {
            _sliderPos += _mMove.y * 8;

            if ( _mMove.y )
                v6 = 0;
        }
        break;

        case Input::KC_JOYX:
        {
            int v12 = _sliderPos;

            if ( v12 > _joyXYpos.x )
            {
                v12 -= 37;

                if ( v12 < _joyXYpos.x )
                    v12 = _joyXYpos.x;
            }
            else if ( v12 < _joyXYpos.x )
            {
                v12 += 37;

                if ( v12 > _joyXYpos.x )
                    v12 = _joyXYpos.x;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;

        case Input::KC_JOYY:
        {
            int v12 = _sliderPos;

            if ( v12 > _joyXYpos.y )
            {
                v12 -= 37;

                if ( v12 < _joyXYpos.y )
                    v12 = _joyXYpos.y;
            }
            else if ( v12 < _joyXYpos.y )
            {
                v12 += 37;

                if ( v12 > _joyXYpos.y )
                    v12 = _joyXYpos.y;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;

        case Input::KC_JOYTHROTTLE:
        {
            int v12 = _sliderPos;

            if ( v12 > -_joyZRZpos.x )
            {
                v12 -= 37;

                if ( v12 < -_joyZRZpos.x )
                    v12 = -_joyZRZpos.x;
            }
            else if ( v12 < -_joyZRZpos.x )
            {
                v12 += 37;

                if ( v12 > -_joyZRZpos.x )
                    v12 = -_joyZRZpos.x;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;

        case Input::KC_JOYHATX:
        {
            int v12 = _sliderPos;

            if ( v12 > _joyPov.x )
            {
                v12 -= 37;

                if ( v12 < _joyPov.x )
                    v12 = _joyPov.x;
            }
            else if ( v12 < _joyPov.x )
            {
                v12 += 37;

                if ( v12 > _joyPov.x )
                    v12 = _joyPov.x;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;

        case Input::KC_JOYHATY:
        {
            int v12 = _sliderPos;

            if ( v12 > _joyPov.y )
            {
                v12 -= 37;

                if ( v12 < _joyPov.y )
                    v12 = _joyPov.y;
            }
            else if ( v12 < _joyPov.y )
            {
                v12 += 37;

                if ( v12 > _joyPov.y )
                    v12 = _joyPov.y;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;
        
        case Input::KC_JOYRUDDER:
        {
            int v12 = _sliderPos;

            if ( v12 > _joyZRZpos.y )
            {
                v12 -= 37;

                if ( v12 < _joyZRZpos.y )
                    v12 = _joyZRZpos.y;
            }
            else if ( v12 < _joyZRZpos.y )
            {
                v12 += 37;

                if ( v12 > _joyZRZpos.y )
                    v12 = _joyZRZpos.y;
            }
            _sliderPos = v12;
            v6 = 0;
        }
        break;

        default:
            if (NC_STACK_input::KeyMatrix.at( _bindedKey ).down )
            {
                v6 = 0;
                _sliderPos += 37;
            }
            break;
    }

    if ( v6 )
        _sliderPos = 8 * _sliderPos / 10;

    if ( _sliderPos > 300 )
        _sliderPos = 300;
    else if ( _sliderPos < -300 )
        _sliderPos = -300;

    return _sliderPos / 300.0;
}

void NC_STACK_winp::QueryKeyboard(InputState *arg)
{
    arg->HotKeyID = -1;
    arg->KbdLastDown = _kbdLastDown;
    arg->KbdLastHit = _kbdLastHit;
    
    _kbdLastHit = Input::KC_NONE;

    arg->chr = 0;

    if ( !_inputText.empty() )
    {
       arg->chr = _inputText.front();
       _inputText.pop();
    }
}

bool NC_STACK_winp::BindKey(const std::string &keyName)
{
    int16_t id = NC_STACK_input::GetKeyIDByName(keyName);
    if (id == -1)
        return false;
    
    _bindedKey = id;
    return NC_STACK_input::KeyMatrix.at(id).IsSlider;
}

void NC_STACK_winp::ResetSlider()
{
    _sliderPos = 0;
}

void NC_STACK_winp::FFDOTankEngine(int state, float p1, float p2)
{
    if ( _ffTankEngine.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffTankEngine.Stop();
            _ffJetEngine.Stop();
            _ffCopterEngine.Stop();

            _ffTankEngine.Run();

            float a2 = p1 * 16383.0; // 5000
            float a3 = 1000.0 / (p2 * 13.0 + 5.0);
            _ffTankEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffTankEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 19660.0; // 6000
            float v6 = 1000.0 / (p2 * 13.0 + 5.0);
            _ffTankEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDOJetEngine(int state, float p1, float p2)
{
    if ( _ffJetEngine.OK() )
    {
        if ( state == FF_STATE_START)
        {
            _ffTankEngine.Stop();
            _ffJetEngine.Stop();
            _ffCopterEngine.Stop();

            _ffJetEngine.Run();

            float a2 = p1 * 19660.0; // 6000
            float a3 = 1000.0 / (p2 * 12.0 + 14.0);
            _ffJetEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffJetEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 19660.0; // 6000
            float v6 = 1000.0 / (p2 * 12.0 + 14.0);
            _ffJetEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDOHeliEngine(int state, float p1, float p2)
{
    if ( _ffCopterEngine.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffTankEngine.Stop();
            _ffJetEngine.Stop();
            _ffCopterEngine.Stop();

            _ffCopterEngine.Run();

            float a2 = p1 * 26213.6; //8000.0;
            float a3 = 1000.0 / (p2 * 12.0 + 6.0);
            _ffCopterEngine.Update(a2, a3);
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffCopterEngine.Stop();
        }
        else if ( state == FF_STATE_UPDATE )
        {
            float v5 = p1 * 26213.6; //8000.0;
            float v6 = 1000.0 / (p2 * 12.0 + 6.0);
            _ffCopterEngine.Update(v5, v6);
        }
    }
}

void NC_STACK_winp::FFDORotDamper(int state, float p1)
{
    if ( _ffRotDamper.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffRotDamper.Stop();

            _ffRotDamper.Update(p1 * 32767.0);

            _ffRotDamper.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffRotDamper.Stop();
        }
    }
}

void NC_STACK_winp::FFDOMiniGun(int state)
{
    if ( _ffMGun.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffMGun.Stop();

            _ffMGun.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffMGun.Stop();
        }
    }
}

void NC_STACK_winp::FFDOMissileFire(int state)
{
    if ( _ffMissFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffMissFire.Stop();
            _ffGrenadeFire.Stop();
            _ffBombFire.Stop();

            _ffMissFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffMissFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOGrenadeFire(int state)
{
    if ( _ffGrenadeFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffMissFire.Stop();
            _ffGrenadeFire.Stop();
            _ffBombFire.Stop();

            _ffGrenadeFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffGrenadeFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOBombFire(int state)
{
    if ( _ffBombFire.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffMissFire.Stop();
            _ffGrenadeFire.Stop();
            _ffBombFire.Stop();

            _ffBombFire.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffBombFire.Stop();
        }
    }
}

void NC_STACK_winp::FFDOCollision(int state, float a2, float a3, float a4)
{
    if ( _ffCollide.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffCollide.Stop();

            _ffCollide.Update(a2, a3, a4);

            _ffCollide.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffCollide.Stop();
        }
    }
}

void NC_STACK_winp::FFDOShake(int state, float a2, float a3, float a4, float a5)
{
    if ( _ffShake.OK() )
    {
        if ( state == FF_STATE_START )
        {
            _ffShake.Stop();

            _ffShake.Update(a2, a3, a4, a5);

            _ffShake.Run();
        }
        else if ( state == FF_STATE_STOP )
        {
            _ffShake.Stop();
        }
    }
}

void NC_STACK_winp::FFstopAll()
{
    _ffTankEngine.Stop();
    _ffJetEngine.Stop();
    _ffCopterEngine.Stop();
    _ffRotDamper.Stop();
    _ffMGun.Stop();
    _ffMissFire.Stop();
    _ffGrenadeFire.Stop();
    _ffBombFire.Stop();
    _ffCollide.Stop();
    _ffShake.Stop();
}


void NC_STACK_winp::ForceFeedBack(uint8_t state, uint8_t effID, float p1, float p2, float p3, float p4)
{
    switch ( effID )
    {
    case FF_TYPE_TANKENGINE:
        FFDOTankEngine(state, p1, p2);
        break;
    case FF_TYPE_JETENGINE:
        FFDOJetEngine(state, p1, p2);
        break;
    case FF_TYPE_HELIENGINE:
        FFDOHeliEngine(state, p1, p2);
        break;
    case FF_TYPE_ROTDAMPER:
        FFDORotDamper(state, p1);
        break;
    case FF_TYPE_MINIGUN:
        FFDOMiniGun(state);
        break;
    case FF_TYPE_MISSILEFIRE:
        FFDOMissileFire(state);
        break;
    case FF_TYPE_GRENADEFIRE:
        FFDOGrenadeFire(state);
        break;
    case FF_TYPE_BOMBFIRE:
        FFDOBombFire(state);
        break;
    case FF_TYPE_COLLISION:
        FFDOCollision(state, p1, p3, p4);
        break;
    case FF_TYPE_SHAKE:
        FFDOShake(state, p1, p2, p3, p4);
        break;
    case FF_TYPE_ALL:
        FFstopAll();
        break;
    default:
        return;
    }
}

void NC_STACK_winp::CheckJoy()
{
    if ( _joyEnable )
    {
        uint32_t prevBtnState = _joyButtonStates;
        _joyButtonStates = 0;
        _joyXYpos = Common::Point();
        _joyZRZpos = Common::Point();

        _joyXYpos.x = sdlJoyAxis(_joyHandle, 0);
        _joyXYpos.y = sdlJoyAxis(_joyHandle, 1);

        int joyaxs = SDL_JoystickNumAxes(_joyHandle);

        if ( joyaxs > 2 )
            _joyZRZpos.x = sdlJoyAxis(_joyHandle, 2);
        else
            _joyZRZpos.x = -300;

        if ( joyaxs > 3 )
            _joyZRZpos.y = sdlJoyAxis(_joyHandle, 3);
        else
            _joyZRZpos.y = 0;

        for (int i = 0; i < 8; i++ )
        {
            if ( SDL_JoystickGetButton(_joyHandle, _joyButtonMap[i]) )
            {
                _joyButtonStates |= 1 << i;

                if ( !((1 << i) & prevBtnState) )
                    KeyDown(i + Input::KC_JOYB0);
            }
            else if ( (1 << i) & prevBtnState )
            {
                KeyUp(i + Input::KC_JOYB0);
            }
        }

        if ( SDL_JoystickNumHats(_joyHandle) > 0)
        {
            switch( SDL_JoystickGetHat(_joyHandle, _joyHatMap[0]) )
            {
            case SDL_HAT_UP:
                _joyPov.x = 0;
                _joyPov.y = 300;
                break;

            case SDL_HAT_RIGHTUP:
                _joyPov.x = 212;
                _joyPov.y = 212;
                break;

            case SDL_HAT_RIGHT:
                _joyPov.x = 300;
                _joyPov.y = 0;
                break;

            case SDL_HAT_RIGHTDOWN:
                _joyPov.x = 212;
                _joyPov.y = -212;
                break;

            case SDL_HAT_DOWN:
                _joyPov.x = 0;
                _joyPov.y = -300;
                break;

            case SDL_HAT_LEFTDOWN:
                _joyPov.x = -212;
                _joyPov.y = -212;
                break;

            case SDL_HAT_LEFT:
                _joyPov.x = -300;
                _joyPov.y = 0;
                break;

            case SDL_HAT_LEFTUP:
                _joyPov.x = -212;
                _joyPov.y = 212;
                break;

            case SDL_HAT_CENTERED:
            default:
                _joyPov.x = 0;
                _joyPov.y = 0;
                break;
            }
        }
        else
        {
            _joyPov.x = 0;
            _joyPov.y = 0;
        }
    }
}

void NC_STACK_winp::CheckClick(ClickBoxInf *arg)
{
    arg->flag = 0;

    CheckJoy();

    arg->move.ScreenPos = _mPos;
    _mMove = _mMoveQuery;
    
    _mMoveQuery = Common::Point();

    if ( _mLstate )
        arg->flag |= ClickBoxInf::FLAG_LM_HOLD;
    if ( _mMstate )
        arg->flag |= ClickBoxInf::FLAG_MM_HOLD;
    if ( _mRstate )
        arg->flag |= ClickBoxInf::FLAG_RM_HOLD;
    if ( _mDBLstate )
        arg->flag |= ClickBoxInf::FLAG_DBL_CLICK;

    if ( _mLDcnt > 0 )
    {
        arg->ldw_pos.ScreenPos.x = _mLDpos.x;
        arg->ldw_pos.ScreenPos.y = _mLDpos.y;
        arg->flag |= ClickBoxInf::FLAG_LM_DOWN;
    }
    if ( _mLUcnt > 0 )
    {
        arg->lup_pos.ScreenPos.x = _mLUpos.x;
        arg->lup_pos.ScreenPos.y = _mLUpos.y;
        arg->flag |= ClickBoxInf::FLAG_LM_UP;
    }
    if ( _mRDcnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_RM_DOWN;
    }
    if ( _mRUcnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_RM_UP;
    }
    if ( _mMDcnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_MM_DOWN;
    }
    if ( _mMUcnt > 0 )
    {
        arg->flag |= ClickBoxInf::FLAG_MM_UP;
    }

    _mDBLstate = false;
    _mLDcnt = 0;
    _mLUcnt = 0;
    _mRDcnt = 0;
    _mRUcnt = 0;
    _mMDcnt = 0;
    _mMUcnt = 0;

    NC_STACK_iwimp::CheckClick(arg);
}

void NC_STACK_winp::initfirst()
{
    KBDMapping.clear();
    
    KBDMapping[SDL_SCANCODE_ESCAPE]      = Input::KC_ESCAPE;
    KBDMapping[SDL_SCANCODE_SPACE]       = Input::KC_SPACE;
    KBDMapping[SDL_SCANCODE_UP]          = Input::KC_UP;
    KBDMapping[SDL_SCANCODE_DOWN]        = Input::KC_DOWN;
    KBDMapping[SDL_SCANCODE_LEFT]        = Input::KC_LEFT;
    KBDMapping[SDL_SCANCODE_RIGHT]       = Input::KC_RIGHT;
    KBDMapping[SDL_SCANCODE_F1]          = Input::KC_F1;
    KBDMapping[SDL_SCANCODE_F2]          = Input::KC_F2;
    KBDMapping[SDL_SCANCODE_F3]          = Input::KC_F3;
    KBDMapping[SDL_SCANCODE_F4]          = Input::KC_F4;
    KBDMapping[SDL_SCANCODE_F5]          = Input::KC_F5;
    KBDMapping[SDL_SCANCODE_F6]          = Input::KC_F6;
    KBDMapping[SDL_SCANCODE_F7]          = Input::KC_F7;
    KBDMapping[SDL_SCANCODE_F8]          = Input::KC_F8;
    KBDMapping[SDL_SCANCODE_F9]          = Input::KC_F9;
    KBDMapping[SDL_SCANCODE_F10]         = Input::KC_F10;
    KBDMapping[SDL_SCANCODE_F11]         = Input::KC_F11;
    KBDMapping[SDL_SCANCODE_F12]         = Input::KC_F12;
    KBDMapping[SDL_SCANCODE_BACKSPACE]   = Input::KC_BACKSPACE;
    KBDMapping[SDL_SCANCODE_TAB]         = Input::KC_TAB;
    KBDMapping[SDL_SCANCODE_CLEAR]       = Input::KC_CLEAR;
    KBDMapping[SDL_SCANCODE_RETURN]      = Input::KC_RETURN;
    KBDMapping[SDL_SCANCODE_LCTRL]       = Input::KC_CTRL;
    KBDMapping[SDL_SCANCODE_RCTRL]       = Input::KC_CTRL;
    KBDMapping[SDL_SCANCODE_RSHIFT]      = Input::KC_SHIFT;
    KBDMapping[SDL_SCANCODE_LSHIFT]      = Input::KC_SHIFT;
    KBDMapping[SDL_SCANCODE_LALT]        = Input::KC_ALT;
    KBDMapping[SDL_SCANCODE_RALT]        = Input::KC_ALT;
    KBDMapping[SDL_SCANCODE_PAUSE]       = Input::KC_PAUSE;
    KBDMapping[SDL_SCANCODE_PAGEUP]      = Input::KC_PGUP;
    KBDMapping[SDL_SCANCODE_PAGEDOWN]    = Input::KC_PGDOWN;
    KBDMapping[SDL_SCANCODE_END]         = Input::KC_END;
    KBDMapping[SDL_SCANCODE_HOME]        = Input::KC_HOME;
    KBDMapping[SDL_SCANCODE_SELECT]      = Input::KC_SELECT;
    KBDMapping[SDL_SCANCODE_EXECUTE]     = Input::KC_EXECUTE;
    KBDMapping[SDL_SCANCODE_PRINTSCREEN] = Input::KC_SNAPSHOT;
    KBDMapping[SDL_SCANCODE_INSERT]      = Input::KC_INSERT;
    KBDMapping[SDL_SCANCODE_DELETE]      = Input::KC_DELETE;
    KBDMapping[SDL_SCANCODE_HELP]        = Input::KC_HELP;
    KBDMapping[SDL_SCANCODE_1]           = Input::KC_1;
    KBDMapping[SDL_SCANCODE_2]           = Input::KC_2;
    KBDMapping[SDL_SCANCODE_3]           = Input::KC_3;
    KBDMapping[SDL_SCANCODE_4]           = Input::KC_4;
    KBDMapping[SDL_SCANCODE_5]           = Input::KC_5;
    KBDMapping[SDL_SCANCODE_6]           = Input::KC_6;
    KBDMapping[SDL_SCANCODE_7]           = Input::KC_7;
    KBDMapping[SDL_SCANCODE_8]           = Input::KC_8;
    KBDMapping[SDL_SCANCODE_9]           = Input::KC_9;
    KBDMapping[SDL_SCANCODE_0]           = Input::KC_0;
    KBDMapping[SDL_SCANCODE_A]           = Input::KC_A;
    KBDMapping[SDL_SCANCODE_B]           = Input::KC_B;
    KBDMapping[SDL_SCANCODE_C]           = Input::KC_C;
    KBDMapping[SDL_SCANCODE_D]           = Input::KC_D;
    KBDMapping[SDL_SCANCODE_E]           = Input::KC_E;
    KBDMapping[SDL_SCANCODE_F]           = Input::KC_F;
    KBDMapping[SDL_SCANCODE_G]           = Input::KC_G;
    KBDMapping[SDL_SCANCODE_H]           = Input::KC_H;
    KBDMapping[SDL_SCANCODE_I]           = Input::KC_I;
    KBDMapping[SDL_SCANCODE_J]           = Input::KC_J;
    KBDMapping[SDL_SCANCODE_K]           = Input::KC_K;
    KBDMapping[SDL_SCANCODE_L]           = Input::KC_L;
    KBDMapping[SDL_SCANCODE_M]           = Input::KC_M;
    KBDMapping[SDL_SCANCODE_N]           = Input::KC_N;
    KBDMapping[SDL_SCANCODE_O]           = Input::KC_O;
    KBDMapping[SDL_SCANCODE_P]           = Input::KC_P;
    KBDMapping[SDL_SCANCODE_Q]           = Input::KC_Q;
    KBDMapping[SDL_SCANCODE_R]           = Input::KC_R;
    KBDMapping[SDL_SCANCODE_S]           = Input::KC_S;
    KBDMapping[SDL_SCANCODE_T]           = Input::KC_T;
    KBDMapping[SDL_SCANCODE_U]           = Input::KC_U;
    KBDMapping[SDL_SCANCODE_V]           = Input::KC_V;
    KBDMapping[SDL_SCANCODE_W]           = Input::KC_W;
    KBDMapping[SDL_SCANCODE_X]           = Input::KC_X;
    KBDMapping[SDL_SCANCODE_Y]           = Input::KC_Y;
    KBDMapping[SDL_SCANCODE_Z]           = Input::KC_Z;
    KBDMapping[SDL_SCANCODE_KP_0]        = Input::KC_NUM0;
    KBDMapping[SDL_SCANCODE_KP_1]        = Input::KC_NUM1;
    KBDMapping[SDL_SCANCODE_KP_2]        = Input::KC_NUM2;
    KBDMapping[SDL_SCANCODE_KP_3]        = Input::KC_NUM3;
    KBDMapping[SDL_SCANCODE_KP_4]        = Input::KC_NUM4;
    KBDMapping[SDL_SCANCODE_KP_5]        = Input::KC_NUM5;
    KBDMapping[SDL_SCANCODE_KP_6]        = Input::KC_NUM6;
    KBDMapping[SDL_SCANCODE_KP_7]        = Input::KC_NUM7;
    KBDMapping[SDL_SCANCODE_KP_8]        = Input::KC_NUM8;
    KBDMapping[SDL_SCANCODE_KP_9]        = Input::KC_NUM9;
    KBDMapping[SDL_SCANCODE_KP_MULTIPLY] = Input::KC_NUMMUL;
    KBDMapping[SDL_SCANCODE_KP_PLUS]     = Input::KC_NUMPLUS;
    KBDMapping[SDL_SCANCODE_KP_PERIOD]   = Input::KC_NUMDOT;
    KBDMapping[SDL_SCANCODE_KP_MINUS]    = Input::KC_NUMMINUS;
    KBDMapping[SDL_SCANCODE_KP_ENTER]    = Input::KC_NUMENTER;
    KBDMapping[SDL_SCANCODE_KP_DIVIDE]   = Input::KC_NUMDIV;
    KBDMapping[SDL_SCANCODE_COMMA]       = Input::KC_EXTRA1;
    KBDMapping[SDL_SCANCODE_PERIOD]      = Input::KC_EXTRA2;
    KBDMapping[SDL_SCANCODE_MINUS]       = Input::KC_EXTRA3;
    KBDMapping[SDL_SCANCODE_BACKSLASH]   = Input::KC_EXTRA4;
    KBDMapping[SDL_SCANCODE_SEMICOLON]   = Input::KC_EXTRA5;
    KBDMapping[SDL_SCANCODE_EQUALS]      = Input::KC_EXTRA6;
    KBDMapping[SDL_SCANCODE_GRAVE]       = Input::KC_EXTRA7;
    KBDMapping[SDL_SCANCODE_APOSTROPHE]  = Input::KC_EXTRA8;
    KBDMapping[SDL_SCANCODE_SLASH]       = Input::KC_EXTRA9;
    KBDMapping[SDL_SCANCODE_RIGHTBRACKET]= Input::KC_EXTRA10;
    KBDMapping[SDL_SCANCODE_BACKSLASH]   = Input::KC_EXTRA11;
    KBDMapping[SDL_SCANCODE_LEFTBRACKET] = Input::KC_EXTRA12;
    //KBDMapping[???]       = Input::KC_EXTRA13; // 	OEM_8 (§ !)
    KBDMapping[SDL_SCANCODE_SCROLLLOCK]  = Input::KC_EXTRA14;
    KBDMapping[SDL_SCANCODE_NUMLOCKCLEAR]= Input::KC_EXTRA15;
    KBDMapping[SDL_SCANCODE_F13]         = Input::KC_EXTRA16;
    KBDMapping[SDL_SCANCODE_F14]         = Input::KC_EXTRA17;
    KBDMapping[SDL_SCANCODE_F15]         = Input::KC_EXTRA18;

    //Joy staff

    _joyEnable = false;

    sdlInputResetLog();
    _joyWantGuid = sdlReadJoyGuid();

    int numJoy = SDL_NumJoysticks();

    if (numJoy)
    {
        for (int i = 0; i < numJoy; i++)
        {
            SDL_JoystickGUID jGUID = SDL_JoystickGetDeviceGUID(i);

            char guidBuff[64];
            SDL_JoystickGetGUIDString(jGUID, guidBuff, sizeof(guidBuff));

            sdlInputLog("Found joystick #%d: %s\n\tGUID:%s\n", i, SDL_JoystickNameForIndex(i), guidBuff);

            if (!_joyHandle && sdlGUIDcmp(jGUID, _joyWantGuid) )
                _joyHandle = SDL_JoystickOpen(i);
        }

        if ( !_joyHandle )
            _joyHandle = SDL_JoystickOpen(0);

        _joyHaptic = SDL_HapticOpenFromJoystick(_joyHandle);
    }
    else
    {
        sdlInputLog("No joysticks found\n");
    }

    if (_joyHandle)
    {
        _joyEnable = true;
        sdlInputLog("\nSelected joystick: %s\n", SDL_JoystickName(_joyHandle));
        sdlInputLog("\tAxes:%d\tHats:%d\tButtons:%d\tBalls:%d\n", SDL_JoystickNumAxes(_joyHandle), SDL_JoystickNumHats(_joyHandle), SDL_JoystickNumButtons(_joyHandle), SDL_JoystickNumBalls(_joyHandle));
        sdlJoyReadMapping(_joyHandle);
        sdlInputLog("\n");
    }

    _mLstate = false;
    _mRstate = false;
    _mMstate = false;
    _mDBLstate = false;

    _mLUcnt = 0;
    _mLDcnt = 0;
    _mRUcnt = 0;
    _mRDcnt = 0;
    _mMUcnt = 0;
    _mMDcnt = 0;
    
    _kbdLastDown = 0;
    _kbdLastHit = 0;

    _joyButtonStates = 0;

    SDLWRAP::EventsAddHandler(InputWatch);

    if (_joyHaptic)
    {
        _ffTankEngine.Bind(_joyHaptic);
        _ffJetEngine.Bind(_joyHaptic);
        _ffCopterEngine.Bind(_joyHaptic);
        _ffRotDamper.Bind(_joyHaptic);
        _ffMGun.Bind(_joyHaptic);
        _ffMissFire.Bind(_joyHaptic);
        _ffGrenadeFire.Bind(_joyHaptic);
        _ffBombFire.Bind(_joyHaptic);
        _ffCollide.Bind(_joyHaptic);
        _ffShake.Bind(_joyHaptic);
    }
}

