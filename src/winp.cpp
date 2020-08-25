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
    if ( vk != Input::KEY_NONE )
        NC_STACK_input::KeyMatrix.at(vk).down = true;

    _kbdLastDown = vk;
    _kbdLastHit = vk;
}

void NC_STACK_winp::KeyUp(int16_t vk)
{
    if ( vk != Input::KEY_NONE )
        NC_STACK_input::KeyMatrix.at(vk).down = false;

    if (_kbdLastDown == vk)
        _kbdLastDown = Input::KEY_NONE;
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
        if ( !Gui::Root::Instance.MouseDown(Common::Point(event->button.x, event->button.y), event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                _mLstate = true;
                _mLDpos.x = event->button.x;
                _mLDpos.y = event->button.y;

                _mLDpos = SDLWRAP::MousePosNorm(_mLDpos);

                _mLDcnt++;

                KeyDown(Input::KEY_LMB);
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                _mRstate = true;
                _mRDpos.x = event->button.x;
                _mRDpos.y = event->button.y;

                _mRDpos = SDLWRAP::MousePosNorm(_mRDpos);

                _mRDcnt++;

                KeyDown(Input::KEY_RMB);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                _mMstate = true;
                _mMDpos.x = event->button.x;
                _mMDpos.y = event->button.y;

                _mMDpos = SDLWRAP::MousePosNorm(_mMDpos);

                _mMDcnt++;

                KeyDown(Input::KEY_MMB);
            }
        }

        break;

    case SDL_MOUSEBUTTONUP:
        if ( !Gui::Root::Instance.MouseUp(Common::Point(event->button.x, event->button.y), event->button.button) )
        {
            if (event->button.button == SDL_BUTTON_LEFT)
            {
                _mLstate = false;
                _mLUpos.x = event->button.x;
                _mLUpos.y = event->button.y;

                _mLUpos = SDLWRAP::MousePosNorm(_mLUpos);

                _mLUcnt++;

                KeyUp(Input::KEY_LMB);

                if ((event->button.clicks & 1) == 0)
                    _mDBLstate = true;
            }
            else if (event->button.button == SDL_BUTTON_RIGHT)
            {
                _mRstate = false;
                _mRUpos.x = event->button.x;
                _mRUpos.y = event->button.y;

                _mRUpos = SDLWRAP::MousePosNorm(_mRUpos);

                _mRUcnt++;

                KeyUp(Input::KEY_RMB);
            }
            else if (event->button.button == SDL_BUTTON_MIDDLE)
            {
                _mMstate = false;
                _mMUpos.x = event->button.x;
                _mMUpos.y = event->button.y;

                _mMUpos = SDLWRAP::MousePosNorm(_mMUpos);

                _mMUcnt++;

                KeyUp(Input::KEY_MMB);
            }
        }

        break;

    case SDL_MOUSEMOTION:
        if ( !Gui::Root::Instance.MouseMove(Common::Point(event->button.x, event->button.y)) )
        {
            _mPos = SDLWRAP::MousePosNorm( Common::Point(event->motion.x, event->motion.y) );
            _mMoveQuery = Common::Point(event->motion.xrel, event->motion.yrel);
            
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

size_t NC_STACK_winp::func2(IDVList &stak)
{
    return NC_STACK_iwimp::func2(stak);
}

size_t NC_STACK_winp::func3(IDVList &stk)
{
    return NC_STACK_iwimp::func3(stk);
}

bool NC_STACK_winp::GetState()
{
    if ( _bindedKey < 0 )
        return false;

    switch ( _bindedKey )
    {
    case Input::KEY_LMB:
        return _mLstate;
        break;
    case Input::KEY_RMB:
        return _mRstate;
        break;
    case Input::KEY_MMB:
        return _mMstate;
        break;
    case Input::KEY_JOYB0:
    case Input::KEY_JOYB1:
    case Input::KEY_JOYB2:
    case Input::KEY_JOYB3:
    case Input::KEY_JOYB4:
    case Input::KEY_JOYB5:
    case Input::KEY_JOYB6:
    case Input::KEY_JOYB7:
        if ( _joyButtonStates & (1 << (_bindedKey - Input::KEY_JOYB0)) )
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
        case Input::KEY_MOUSEX:
        {
            _sliderPos += _mMove.x * 8;

            if ( _mMove.x )
                v6 = 0;
        }
        break;

        case Input::KEY_MOUSEY:
        {
            _sliderPos += _mMove.y * 8;

            if ( _mMove.y )
                v6 = 0;
        }
        break;

        case Input::KEY_JOYX:
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

        case Input::KEY_JOYY:
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

        case Input::KEY_JOYTHROTTLE:
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

        case Input::KEY_JOYHATX:
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

        case Input::KEY_JOYHATY:
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
        
        case Input::KEY_JOYRUDDER:
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
    
    _kbdLastHit = Input::KEY_NONE;

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
                    KeyDown(i + Input::KEY_JOYB0);
            }
            else if ( (1 << i) & prevBtnState )
            {
                KeyUp(i + Input::KEY_JOYB0);
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
    
    KBDMapping[SDL_SCANCODE_ESCAPE]      = Input::KEY_ESCAPE;
    KBDMapping[SDL_SCANCODE_SPACE]       = Input::KEY_SPACE;
    KBDMapping[SDL_SCANCODE_UP]          = Input::KEY_UP;
    KBDMapping[SDL_SCANCODE_DOWN]        = Input::KEY_DOWN;
    KBDMapping[SDL_SCANCODE_LEFT]        = Input::KEY_LEFT;
    KBDMapping[SDL_SCANCODE_RIGHT]       = Input::KEY_RIGHT;
    KBDMapping[SDL_SCANCODE_F1]          = Input::KEY_F1;
    KBDMapping[SDL_SCANCODE_F2]          = Input::KEY_F2;
    KBDMapping[SDL_SCANCODE_F3]          = Input::KEY_F3;
    KBDMapping[SDL_SCANCODE_F4]          = Input::KEY_F4;
    KBDMapping[SDL_SCANCODE_F5]          = Input::KEY_F5;
    KBDMapping[SDL_SCANCODE_F6]          = Input::KEY_F6;
    KBDMapping[SDL_SCANCODE_F7]          = Input::KEY_F7;
    KBDMapping[SDL_SCANCODE_F8]          = Input::KEY_F8;
    KBDMapping[SDL_SCANCODE_F9]          = Input::KEY_F9;
    KBDMapping[SDL_SCANCODE_F10]         = Input::KEY_F10;
    KBDMapping[SDL_SCANCODE_F11]         = Input::KEY_F11;
    KBDMapping[SDL_SCANCODE_F12]         = Input::KEY_F12;
    KBDMapping[SDL_SCANCODE_BACKSPACE]   = Input::KEY_BACKSPACE;
    KBDMapping[SDL_SCANCODE_TAB]         = Input::KEY_TAB;
    KBDMapping[SDL_SCANCODE_CLEAR]       = Input::KEY_CLEAR;
    KBDMapping[SDL_SCANCODE_RETURN]      = Input::KEY_RETURN;
    KBDMapping[SDL_SCANCODE_LCTRL]       = Input::KEY_CTRL;
    KBDMapping[SDL_SCANCODE_RCTRL]       = Input::KEY_CTRL;
    KBDMapping[SDL_SCANCODE_RSHIFT]      = Input::KEY_SHIFT;
    KBDMapping[SDL_SCANCODE_LSHIFT]      = Input::KEY_SHIFT;
    KBDMapping[SDL_SCANCODE_LALT]        = Input::KEY_ALT;
    KBDMapping[SDL_SCANCODE_RALT]        = Input::KEY_ALT;
    KBDMapping[SDL_SCANCODE_PAUSE]       = Input::KEY_PAUSE;
    KBDMapping[SDL_SCANCODE_PAGEUP]      = Input::KEY_PGUP;
    KBDMapping[SDL_SCANCODE_PAGEDOWN]    = Input::KEY_PGDOWN;
    KBDMapping[SDL_SCANCODE_END]         = Input::KEY_END;
    KBDMapping[SDL_SCANCODE_HOME]        = Input::KEY_HOME;
    KBDMapping[SDL_SCANCODE_SELECT]      = Input::KEY_SELECT;
    KBDMapping[SDL_SCANCODE_EXECUTE]     = Input::KEY_EXECUTE;
    KBDMapping[SDL_SCANCODE_PRINTSCREEN] = Input::KEY_SNAPSHOT;
    KBDMapping[SDL_SCANCODE_INSERT]      = Input::KEY_INSERT;
    KBDMapping[SDL_SCANCODE_DELETE]      = Input::KEY_DELETE;
    KBDMapping[SDL_SCANCODE_HELP]        = Input::KEY_HELP;
    KBDMapping[SDL_SCANCODE_1]           = Input::KEY_1;
    KBDMapping[SDL_SCANCODE_2]           = Input::KEY_2;
    KBDMapping[SDL_SCANCODE_3]           = Input::KEY_3;
    KBDMapping[SDL_SCANCODE_4]           = Input::KEY_4;
    KBDMapping[SDL_SCANCODE_5]           = Input::KEY_5;
    KBDMapping[SDL_SCANCODE_6]           = Input::KEY_6;
    KBDMapping[SDL_SCANCODE_7]           = Input::KEY_7;
    KBDMapping[SDL_SCANCODE_8]           = Input::KEY_8;
    KBDMapping[SDL_SCANCODE_9]           = Input::KEY_9;
    KBDMapping[SDL_SCANCODE_0]           = Input::KEY_0;
    KBDMapping[SDL_SCANCODE_A]           = Input::KEY_A;
    KBDMapping[SDL_SCANCODE_B]           = Input::KEY_B;
    KBDMapping[SDL_SCANCODE_C]           = Input::KEY_C;
    KBDMapping[SDL_SCANCODE_D]           = Input::KEY_D;
    KBDMapping[SDL_SCANCODE_E]           = Input::KEY_E;
    KBDMapping[SDL_SCANCODE_F]           = Input::KEY_F;
    KBDMapping[SDL_SCANCODE_G]           = Input::KEY_G;
    KBDMapping[SDL_SCANCODE_H]           = Input::KEY_H;
    KBDMapping[SDL_SCANCODE_I]           = Input::KEY_I;
    KBDMapping[SDL_SCANCODE_J]           = Input::KEY_J;
    KBDMapping[SDL_SCANCODE_K]           = Input::KEY_K;
    KBDMapping[SDL_SCANCODE_L]           = Input::KEY_L;
    KBDMapping[SDL_SCANCODE_M]           = Input::KEY_M;
    KBDMapping[SDL_SCANCODE_N]           = Input::KEY_N;
    KBDMapping[SDL_SCANCODE_O]           = Input::KEY_O;
    KBDMapping[SDL_SCANCODE_P]           = Input::KEY_P;
    KBDMapping[SDL_SCANCODE_Q]           = Input::KEY_Q;
    KBDMapping[SDL_SCANCODE_R]           = Input::KEY_R;
    KBDMapping[SDL_SCANCODE_S]           = Input::KEY_S;
    KBDMapping[SDL_SCANCODE_T]           = Input::KEY_T;
    KBDMapping[SDL_SCANCODE_U]           = Input::KEY_U;
    KBDMapping[SDL_SCANCODE_V]           = Input::KEY_V;
    KBDMapping[SDL_SCANCODE_W]           = Input::KEY_W;
    KBDMapping[SDL_SCANCODE_X]           = Input::KEY_X;
    KBDMapping[SDL_SCANCODE_Y]           = Input::KEY_Y;
    KBDMapping[SDL_SCANCODE_Z]           = Input::KEY_Z;
    KBDMapping[SDL_SCANCODE_KP_0]        = Input::KEY_NUM0;
    KBDMapping[SDL_SCANCODE_KP_1]        = Input::KEY_NUM1;
    KBDMapping[SDL_SCANCODE_KP_2]        = Input::KEY_NUM2;
    KBDMapping[SDL_SCANCODE_KP_3]        = Input::KEY_NUM3;
    KBDMapping[SDL_SCANCODE_KP_4]        = Input::KEY_NUM4;
    KBDMapping[SDL_SCANCODE_KP_5]        = Input::KEY_NUM5;
    KBDMapping[SDL_SCANCODE_KP_6]        = Input::KEY_NUM6;
    KBDMapping[SDL_SCANCODE_KP_7]        = Input::KEY_NUM7;
    KBDMapping[SDL_SCANCODE_KP_8]        = Input::KEY_NUM8;
    KBDMapping[SDL_SCANCODE_KP_9]        = Input::KEY_NUM9;
    KBDMapping[SDL_SCANCODE_KP_MULTIPLY] = Input::KEY_NUMMUL;
    KBDMapping[SDL_SCANCODE_KP_PLUS]     = Input::KEY_NUMPLUS;
    KBDMapping[SDL_SCANCODE_KP_PERIOD]   = Input::KEY_NUMDOT;
    KBDMapping[SDL_SCANCODE_KP_MINUS]    = Input::KEY_NUMMINUS;
    KBDMapping[SDL_SCANCODE_KP_ENTER]    = Input::KEY_NUMENTER;
    KBDMapping[SDL_SCANCODE_KP_DIVIDE]   = Input::KEY_NUMDIV;
    KBDMapping[SDL_SCANCODE_COMMA]       = Input::KEY_EXTRA1;
    KBDMapping[SDL_SCANCODE_PERIOD]      = Input::KEY_EXTRA2;
    KBDMapping[SDL_SCANCODE_MINUS]       = Input::KEY_EXTRA3;
    KBDMapping[SDL_SCANCODE_BACKSLASH]   = Input::KEY_EXTRA4;
    KBDMapping[SDL_SCANCODE_SEMICOLON]   = Input::KEY_EXTRA5;
    KBDMapping[SDL_SCANCODE_EQUALS]      = Input::KEY_EXTRA6;
    KBDMapping[SDL_SCANCODE_GRAVE]       = Input::KEY_EXTRA7;
    KBDMapping[SDL_SCANCODE_APOSTROPHE]  = Input::KEY_EXTRA8;
    KBDMapping[SDL_SCANCODE_SLASH]       = Input::KEY_EXTRA9;
    KBDMapping[SDL_SCANCODE_RIGHTBRACKET]= Input::KEY_EXTRA10;
    KBDMapping[SDL_SCANCODE_BACKSLASH]   = Input::KEY_EXTRA11;
    KBDMapping[SDL_SCANCODE_LEFTBRACKET] = Input::KEY_EXTRA12;
    //KBDMapping[???]       = Input::KEY_EXTRA13; // 	OEM_8 (§ !)
    KBDMapping[SDL_SCANCODE_SCROLLLOCK]  = Input::KEY_EXTRA14;
    KBDMapping[SDL_SCANCODE_NUMLOCKCLEAR]= Input::KEY_EXTRA15;
    KBDMapping[SDL_SCANCODE_F13]         = Input::KEY_EXTRA16;
    KBDMapping[SDL_SCANCODE_F14]         = Input::KEY_EXTRA17;
    KBDMapping[SDL_SCANCODE_F15]         = Input::KEY_EXTRA18;

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

