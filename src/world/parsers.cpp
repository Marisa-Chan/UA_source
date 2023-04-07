#include "parsers.h"
#include "../fmtlib/core.h"
#include "../fmtlib/printf.h"
#include "../yw.h"
#include "../yparobo.h"
#include "../ypaflyer.h"
#include "../ypacar.h"
#include "../log.h"

namespace World
{
namespace Parsers
{



bool UserParser::ReadUserNameFile(const std::string &filename)
{
    if ( !_o._GameShell->UserName.empty() )
        return false;

    std::string buf = fmt::sprintf("save:%s/%s", _o._GameShell->UserName, filename);
    FSMgr::FileHandle *signFile = uaOpenFileAlloc(buf, "r");

    if ( !signFile )
        return false;

    bool res = signFile->ReadLine(&_o._GameShell->netPlayerName);

    delete signFile;
    return res;
}


bool UserParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (StriCmp(word, "new_user"))
        return false;

    if (!_o._GameShell->remoteMode)
    {
        if ( !ReadUserNameFile("callsign.def") )
            _o._GameShell->netPlayerName =  Locale::Text::Dialogs(Locale::DLG_P_UNNAMED);
    }
    return true;
}

int UserParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "username") )
    {
    }
    else if ( !StriCmp(p1, "netname") )
    {
    }
    else if ( !StriCmp(p1, "maxroboenergy") )
    {
        _o._maxRoboEnergy = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "maxreloadconst") )
    {
        _o._maxReloadConst = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "numbuddies") )
    {
    }
    else if ( !StriCmp(p1, "beamenergy") )
    {
        _o._beamEnergyCapacity = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "playerstatus") )
    {
        Stok stok(p2, "_ \t");
        std::string val;
        if ( stok.GetNext(&val) )
        {
            int plid = parser.stol(val, NULL, 0);
            if ( stok.GetNext(&val) )
            {
                _o._playersStats[plid].DestroyedUnits = parser.stol(val, NULL, 0);
                if ( stok.GetNext(&val) )
                {
                    _o._playersStats[plid].DestroyedByUser = parser.stol(val, NULL, 0);
                    if ( stok.GetNext(&val) )
                    {
                        _o._playersStats[plid].ElapsedTime = parser.stol(val, NULL, 0);
                        if ( stok.GetNext(&val) )
                        {
                            _o._playersStats[plid].SectorsTaked = parser.stol(val, NULL, 0);
                            if ( stok.GetNext(&val) )
                            {
                                _o._playersStats[plid].Score = parser.stol(val, NULL, 0);
                                if ( stok.GetNext(&val) )
                                {
                                    _o._playersStats[plid].Power = parser.stol(val, NULL, 0);
                                    if ( stok.GetNext(&val) )
                                    {
                                        _o._playersStats[plid].Upgrades = parser.stol(val, NULL, 0);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else if ( !StriCmp(p1, "jodiefoster") )
    {
        Stok stok(p2, "_ \t");
        std::string val;
        if ( stok.GetNext(&val) )
        {
            _o._levelInfo.JodieFoster[0] = parser.stol(val, NULL, 0);
            if ( stok.GetNext(&val) )
            {
                _o._levelInfo.JodieFoster[1] = parser.stol(val, NULL, 0);
                if ( stok.GetNext(&val) )
                {
                    _o._levelInfo.JodieFoster[2] = parser.stol(val, NULL, 0);
                    if ( stok.GetNext(&val) )
                    {
                        _o._levelInfo.JodieFoster[3] = parser.stol(val, NULL, 0);
                        if ( stok.GetNext(&val) )
                        {
                            _o._levelInfo.JodieFoster[4] = parser.stol(val, NULL, 0);
                            if ( stok.GetNext(&val) )
                            {
                                _o._levelInfo.JodieFoster[5] = parser.stol(val, NULL, 0);
                                if ( stok.GetNext(&val) )
                                {
                                    _o._levelInfo.JodieFoster[6] = parser.stol(val, NULL, 0);
                                    if ( stok.GetNext(&val) )
                                        _o._levelInfo.JodieFoster[7] = parser.stol(val, NULL, 0);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}



bool InputParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( !StriCmp(word, "new_input") )
    {
        for( UserData::TInputConf &k: _o._GameShell->InputConfig )
        {
            k.PKeyCode = 0;
            k.NKeyCode = 0;
        }
        return true;
    }
    else if ( !StriCmp(word, "modify_input") )
        return true;

    return false;
}

int InputParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    _o._GameShell->savedDataFlags |= World::SDF_INPUT;

    if ( !StriCmp(p1, "qualmode") )
    {
    }
    else if ( !StriCmp(p1, "joystick") )
    {
        if ( StrGetBool(p2) )
        {
             _o._GameShell->joystickEnabled = true;
             _o._preferences &= ~World::PREF_JOYDISABLE;
        }
        else
        {
            _o._GameShell->joystickEnabled = false;
            _o._preferences |= World::PREF_JOYDISABLE;
        }
    }
    else if ( !StriCmp(p1, "altjoystick") )
    {
        if ( StrGetBool(p2) )
        {
            _o._GameShell->altJoystickEnabled = true;
            _o._preferences |= World::PREF_ALTJOYSTICK;
        }
        else
        {
            _o._GameShell->altJoystickEnabled = false;
            _o._preferences &= ~World::PREF_ALTJOYSTICK;
        }
    }
    else if ( !StriCmp(p1, "forcefeedback") )
    {
        if ( StrGetBool(p2) )
            _o._preferences &= ~World::PREF_FFDISABLE;
        else
            _o._preferences |= World::PREF_FFDISABLE;
    }
    else
    {
        std::string buf;
        for (std::string::const_iterator it = p2.cbegin(); it != p2.cend(); it++)
        {
            if (*it == '_')
                buf += ' ';
            else if (*it == '$')
                buf += "winp:";
            else
                buf += *it;
        }

        if ( !StriCmp(p1.substr(0,13), "input.slider[") )
        {
            bool ok = false;
            int cfgIdex = parser.stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            if ( !Input::Engine.SetInputExpression(true, cfgIdex, buf) )
            {
                ypa_log_out("WARNING: cannot set slider %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_BAD_DATA;
            }


            int gsIndex = UserData::InputIndexFromConfig(World::INPUT_BIND_TYPE_SLIDER, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in slider-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
            _o._GameShell->InputConfig[ gsIndex ].Type = World::INPUT_BIND_TYPE_SLIDER;
            _o._GameShell->InputConfig[ gsIndex ].KeyID = cfgIdex;

            Stok stok(buf, " :\t\n");
            std::string tmp;
            if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
            {
                _o._GameShell->InputConfig[ gsIndex ].NKeyCode = Input::Engine.GetKeyIDByName(tmp);

                if ( _o._GameShell->InputConfig[ gsIndex ].NKeyCode == -1 )
                {
                    ypa_log_out("Unknown keyword for slider %s\n", tmp.c_str());
                    return ScriptParser::RESULT_BAD_DATA;
                }

                if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
                {
                    _o._GameShell->InputConfig[ gsIndex ].PKeyCode = Input::Engine.GetKeyIDByName(tmp);

                    if ( _o._GameShell->InputConfig[ gsIndex ].PKeyCode == -1 )
                    {
                        ypa_log_out("Unknown keyword for slider %s\n", tmp.c_str());
                        return ScriptParser::RESULT_BAD_DATA;
                    }
                    ok = 1;
                }
            }

            if ( !ok )
            {
                ypa_log_out("Wrong input expression for slider %d\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
        }
        else if ( !StriCmp(p1.substr(0,13), "input.button[") )
        {
            bool ok = false;

            int cfgIdex = parser.stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            if ( !Input::Engine.SetInputExpression(false, cfgIdex, buf) )
            {
                ypa_log_out("WARNING: cannot set button %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_BAD_DATA;
            }

            int gsIndex = UserData::InputIndexFromConfig(World::INPUT_BIND_TYPE_BUTTON, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in button-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
            _o._GameShell->InputConfig[ gsIndex ].Type = World::INPUT_BIND_TYPE_BUTTON;
            _o._GameShell->InputConfig[ gsIndex ].KeyID = cfgIdex;

            Stok stok(buf, " :\t\n");
            std::string tmp;
            if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
            {
                _o._GameShell->InputConfig[ gsIndex ].PKeyCode = Input::Engine.GetKeyIDByName(tmp);

                if ( _o._GameShell->InputConfig[ gsIndex ].PKeyCode == -1 )
                {
                    ypa_log_out("Unknown keyword for button %s\n", tmp.c_str());
                    return ScriptParser::RESULT_BAD_DATA;
                }
                ok = true;
            }

            if ( !ok )
            {
                ypa_log_out("Wrong input expression for button %d\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
        }
        else if ( !StriCmp(p1.substr(0,13), "input.hotkey[") )
        {
            bool ok = false;

            int cfgIdex = parser.stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            if ( !Input::Engine.SetHotKey(cfgIdex, buf) )
            {
                ypa_log_out("WARNING: cannot set hotkey %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_OK;
            }

            int gsIndex = UserData::InputIndexFromConfig(World::INPUT_BIND_TYPE_HOTKEY, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in hotkey-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_OK;
            }

            _o._GameShell->InputConfig[ gsIndex ].Type = World::INPUT_BIND_TYPE_HOTKEY;
            _o._GameShell->InputConfig[ gsIndex ].KeyID = cfgIdex;

            std::string tmp = Stok::Fast(buf, " :\t\n");
            if ( !tmp.empty() )
            {
                _o._GameShell->InputConfig[ gsIndex ].PKeyCode = Input::Engine.GetKeyIDByName(tmp);
                if ( _o._GameShell->InputConfig[ gsIndex ].PKeyCode == -1 )
                {
                    ypa_log_out("Unknown keyword for hotkey: %s\n", tmp.c_str());
                    return ScriptParser::RESULT_OK;
                }
                ok = true;
            }

            if ( !ok )
            {
                ypa_log_out("Wrong input expression for hotkey %d\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
        }
        else
        {
            ypa_log_out("Unknown keyword %s in InputExpression\n", p1.c_str());
            return ScriptParser::RESULT_UNKNOWN;
        }
    }
    return ScriptParser::RESULT_OK;
}


TVhclSound *VhclProtoParser::GetSndFxByName(const std::string &sndname)
{
    struct SoundType
    {
        const std::string name;
        int id;
    };

    static const SoundType CmpVals[] = {
        {"normal",      0},
        {"fire",        1},
        {"wait",        2},
        {"genesis",     3},
        {"explode",     4},
        {"crashland",   5},
        {"crashvhcl",   6},
        {"goingdown",   7},
        {"cockpit",     8},
        {"beamin",      9},
        {"beamout",    10},
        {"build",      11},
    };

    for (const SoundType &t : CmpVals)
    {
        if ( !StriCmp(sndname, t.name) )
            return &_vhcl->sndFX[t.id];
    }

    return NULL;
}

bool FxParser::ParseExtSampleDef(ScriptParser::Parser &parser, TVhclSound *sndfx, const std::string &p2)
{
    Stok stok(p2, "_");
    std::string pp1, pp2, pp3, pp4, pp5, pname;

    if ( !stok.GetNext(&pp1) || !stok.GetNext(&pp2) || !stok.GetNext(&pp3) || !stok.GetNext(&pp4) || !stok.GetNext(&pp5) || !stok.GetNext(&pname) )
        return false;

    sndfx->extS.emplace_back();
    sndfx->ExtSamples.emplace_back();
    
    sndfx->ExtSamples.back().Name = pname;

    TSampleParams &sndEx = sndfx->extS.back();
    sndEx.Sample = NULL;
    sndEx.Loop = parser.stol(pp1, NULL, 0);
    sndEx.Vol = parser.stol(pp2, NULL, 0);
    sndEx.SampleRate = parser.stol(pp3, NULL, 0);
    sndEx.Offset = parser.stol(pp4, NULL, 0);
    sndEx.SampleCnt = parser.stol(pp5, NULL, 0);
    
    return true;
}

int FxParser::ParseSndFX(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    std::string val;
    Stok stok(p1, "_");
    stok.GetNext(&val);

    int sndTP;
    if ( !StriCmp(val, "snd") )
        sndTP = 0;
    else if ( !StriCmp(val, "pal") )
        sndTP = 1;
    else if ( !StriCmp(val, "shk") )
        sndTP = 2;
    else
        return ScriptParser::RESULT_UNKNOWN;

    stok.GetNext(&val);

    TVhclSound *sndfx = GetSndFxByName(val);
    if (!sndfx)
        return ScriptParser::RESULT_UNKNOWN;

    stok.GetNext(&val);

    switch (sndTP)
    {
        case 0:
        {
            if ( !StriCmp(val, "sample") )
                sndfx->MainSample.Name = p2;
            else if ( !StriCmp(val, "volume") )
                sndfx->volume = parser.stol(p2, NULL, 0);
            else if ( !StriCmp(val, "pitch") )
                sndfx->pitch = parser.stol(p2, NULL, 0);
            else if ( !StriCmp(val, "ext") )
            {
                if ( !ParseExtSampleDef(parser, sndfx, p2) )
                    return ScriptParser::RESULT_BAD_DATA;
            }
            else
                return ScriptParser::RESULT_UNKNOWN;
        }
        break;

        case 1:
        {
            if ( !StriCmp(val, "slot") )
                sndfx->sndPrm.slot = parser.stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mag0") )
                sndfx->sndPrm.mag0 = parser.stof(p2, 0);
            else if ( !StriCmp(val, "mag1") )
                sndfx->sndPrm.mag1 = parser.stof(p2, 0);
            else if ( !StriCmp(val, "time") )
                sndfx->sndPrm.time = parser.stol(p2, NULL, 0);
            else
                return ScriptParser::RESULT_UNKNOWN;
        }
        break;

        case 2:
        {
            if ( !StriCmp(val, "slot") )
                sndfx->sndPrm_shk.slot = parser.stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mag0") )
                sndfx->sndPrm_shk.mag0 = parser.stof(p2, 0);
            else if ( !StriCmp(val, "mag1") )
                sndfx->sndPrm_shk.mag1 = parser.stof(p2, 0);
            else if ( !StriCmp(val, "time") )
                sndfx->sndPrm_shk.time = parser.stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mute") )
                sndfx->sndPrm_shk.mute = parser.stof(p2, 0);
            else if ( !StriCmp(val, "x") )
                sndfx->sndPrm_shk.pos.x = parser.stof(p2, 0);
            else if ( !StriCmp(val, "y") )
                sndfx->sndPrm_shk.pos.y = parser.stof(p2, 0);
            else if ( !StriCmp(val, "z") )
                sndfx->sndPrm_shk.pos.z = parser.stof(p2, 0);
            else
                return ScriptParser::RESULT_UNKNOWN;
        }
        break;

        default:
            return ScriptParser::RESULT_UNKNOWN;
    }

    return ScriptParser::RESULT_OK;
}

int VhclProtoParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    TRoboProto *robo = _vhcl->RoboProto;
    
    if (!robo)
        robo = &_roboTmp;    

    if ( !StriCmp(p1, "end") )
    {
        if ( _vhcl->model_id == BACT_TYPES_ROBO )
        {
            if (!_vhcl->RoboProto)
                _vhcl->RoboProto = new TRoboProto(_roboTmp);
            
            _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_PROTO, _vhcl->RoboProto);
        }

        if ( _vhcl->model_id == BACT_TYPES_BACT )
            _vhcl->field_1D6F = (_vhcl->force * 0.6) / _vhcl->airconst;
        else
            _vhcl->field_1D6F = (_vhcl->force) / _vhcl->airconst;

        _vhcl->field_1D6D = (_vhcl->field_1D6F / 10) * 1200.0;

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "model") )
    {
        if ( !StriCmp(p2, "heli") )
        {
            _vhcl->model_id = BACT_TYPES_BACT;
        }
        else if ( !StriCmp(p2, "tank") )
        {
            _vhcl->model_id = BACT_TYPES_TANK;
        }
        else if ( !StriCmp(p2, "robo") )
        {
            _vhcl->model_id = BACT_TYPES_ROBO;

            *robo = TRoboProto();
            robo->matrix = mat3x3::Ident();
        }
        else if ( !StriCmp(p2, "ufo") )
        {
            _vhcl->model_id = BACT_TYPES_UFO;
        }
        else if ( !StriCmp(p2, "car") )
        {
            _vhcl->model_id = BACT_TYPES_CAR;
        }
        else if ( !StriCmp(p2, "gun") )
        {
            _vhcl->model_id = BACT_TYPES_GUN;
        }
        else if ( !StriCmp(p2, "hover") )
        {
            _vhcl->model_id = BACT_TYPES_HOVER;
        }
        else if ( !StriCmp(p2, "plane") )
        {
            _vhcl->model_id = BACT_TYPES_FLYER;

            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, (int32_t)3);
        }
        else if ( !StriCmp(p2, "glider") )
        {
            _vhcl->model_id = BACT_TYPES_FLYER;
            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, (int32_t)2);
        }
        else if ( !StriCmp(p2, "zeppelin") )
        {
            _vhcl->model_id = BACT_TYPES_FLYER;
            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, (int32_t)0);
        }
        else
        {
            return ScriptParser::RESULT_BAD_DATA;
        }
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _vhcl->disable_enable_bitmask |= 1 << parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _vhcl->disable_enable_bitmask &= ~(1 << parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _vhcl->name = p2;
        std::replace(_vhcl->name.begin(), _vhcl->name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _vhcl->energy = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shield") )
    {
        _vhcl->shield = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mass") )
    {
        _vhcl->mass = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "force") )
    {
        _vhcl->force = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "maxrot") )
    {
        _vhcl->maxrot = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "airconst") )
    {
        _vhcl->airconst = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "height") )
    {
        _vhcl->height = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _vhcl->radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "overeof") )
    {
        _vhcl->overeof = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_radius") )
    {
        _vhcl->vwr_radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_overeof") )
    {
        _vhcl->vwr_overeof = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "adist_sector") )
    {
        _vhcl->adist_sector = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "adist_bact") )
    {
        _vhcl->adist_bact = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "sdist_sector") )
    {
        _vhcl->sdist_sector = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "sdist_bact") )
    {
        _vhcl->sdist_bact = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radar") )
    {
        _vhcl->radar = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy") )
    {
        _vhcl->energy += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shield") )
    {
        _vhcl->shield += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_radar") )
    {
        _vhcl->radar += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_normal") )
    {
        _vhcl->vp_normal = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_fire") )
    {
        _vhcl->vp_fire = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_megadeth") )
    {
        _vhcl->vp_megadeth = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_wait") )
    {
        _vhcl->vp_wait = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_dead") )
    {
        _vhcl->vp_dead = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_genesis") )
    {
        _vhcl->vp_genesis = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type_icon") )
    {
        _vhcl->type_icon = p2[0];
    }
    else if ( !StriCmp(p1, "dest_fx") )
    {
        Stok stok(p2, " _");
        std::string fx_type, pp1, pp2, pp3, pp4;

        if ( stok.GetNext(&fx_type) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) && stok.GetNext(&pp4) )
        {
            _vhcl->dest_fx.emplace_back();
            DestFX &dfx = _vhcl->dest_fx.back();
            dfx.Type = DestFX::ParseTypeName(fx_type);

            if (dfx.Type == DestFX::FX_NONE)
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = parser.stol(pp1, NULL, 0);
            dfx.Pos.x = parser.stof(pp2, 0);
            dfx.Pos.y = parser.stof(pp3, 0);
            dfx.Pos.z = parser.stof(pp4, 0);
            
            std::string pp5;
            if ( stok.GetNext(&pp5) )
            {
                if (parser.stol(pp5, NULL, 0) != 0 )
                    dfx.Accel = true;
                else
                    dfx.Accel = false;
            }
        }
        else
        {
            return ScriptParser::RESULT_BAD_DATA;
        }
    }
    else if ( !StriCmp(p1, "ext_dest_fx") || !StriCmp(p1, "extended_dest_fx") )
    {
        Stok stok(p2, " _");
        std::string fx_type, pp1, pp2, pp3, pp4;

        if ( stok.GetNext(&fx_type) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) && stok.GetNext(&pp4) )
        {
            _vhcl->ExtDestroyFX.emplace_back();
            
            DestFX &dfx = _vhcl->ExtDestroyFX.back();
            
            dfx.Type = DestFX::ParseTypeName(fx_type);

            if (dfx.Type == DestFX::FX_NONE)
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = parser.stol(pp1, NULL, 0);
            dfx.Pos.x = parser.stof(pp2, 0);
            dfx.Pos.y = parser.stof(pp3, 0);
            dfx.Pos.z = parser.stof(pp4, 0);
            
            std::string pp5;
            if ( stok.GetNext(&pp5) )
            {
                if (parser.stol(pp5, NULL, 0) != 0 )
                    dfx.Accel = true;
                else
                    dfx.Accel = false;
            }
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "weapon") )
    {
        _vhcl->weapon = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mgun") )
    {
        _vhcl->mgun = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "fire_x") )
    {
        _vhcl->fire_x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "fire_y") )
    {
        _vhcl->fire_y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "fire_z") )
    {
        _vhcl->fire_z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_radius") )
    {
        _vhcl->gun_radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_power") )
    {
        _vhcl->gun_power = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_angle") )
    {
        _vhcl->gun_angle = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "num_weapons") )
    {
        _vhcl->num_weapons = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "kill_after_shot") )
    {
        _vhcl->kill_after_shot = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "job_fighthelicopter") )
    {
        _vhcl->job_fighthelicopter = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fightflyer") )
    {
        _vhcl->job_fightflyer = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fighttank") )
    {
        _vhcl->job_fighttank = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fightrobo") )
    {
        _vhcl->job_fightrobo = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "job_reconnoitre") )
    {
        _vhcl->job_reconnoitre = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "job_conquer") )
    {
        _vhcl->job_conquer = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "gun_side_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_SIDEANGLE, (int32_t)parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "gun_up_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_UPANGLE, (int32_t)parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "gun_down_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_DOWNANGLE, (int32_t)parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "gun_type") )
    {
        int gun_type = 0;
        if ( !StriCmp(p2, "flak") )
        {
            gun_type = 1;
        }
        else if ( !StriCmp(p2, "mg") )
        {
            gun_type = 2;
        }
        else
        {
            //StriCmp(p2, "dummy");
        }

        if ( gun_type )
            _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_FIRETYPE, (int32_t)gun_type);
    }
    else if ( !StriCmp(p1, "kamikaze") )
    {
        _vhcl->initParams.Add(NC_STACK_ypacar::CAR_ATT_KAMIKAZE, (int32_t)1);

        _vhcl->initParams.Add(NC_STACK_ypacar::CAR_ATT_BLAST, (int32_t)parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "wireframe") )
    {
        if ( _vhcl->wireframe )
            _vhcl->wireframe->Delete();
        
        _vhcl->wireframe = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "hud_wireframe") )
    {
        if ( _vhcl->hud_wireframe )
            _vhcl->hud_wireframe->Delete();

        _vhcl->hud_wireframe = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "mg_wireframe") )
    {
        if ( _vhcl->mg_wireframe )
            _vhcl->mg_wireframe->Delete();

        _vhcl->mg_wireframe = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "wpn_wireframe_1") )
    {
        if ( _vhcl->wpn_wireframe_1 )
            _vhcl->wpn_wireframe_1->Delete();

        _vhcl->wpn_wireframe_1 = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "wpn_wireframe_2") )
    {
        if ( _vhcl->wpn_wireframe_2 )
            _vhcl->wpn_wireframe_2->Delete();

        _vhcl->wpn_wireframe_2 = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "vo_type") )
    {
        _vhcl->vo_type = parser.stol(p2, NULL, 16);
    }
    else if ( !StriCmp(p1, "max_pitch") )
    {
        _vhcl->max_pitch = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "scale_fx") )
    {
        Stok stok(p2, "_");
        std::string pp0, pp1, pp2, pp3;

        if ( stok.GetNext(&pp0) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) )
        {
            _vhcl->scale_fx_p0 = parser.stof(pp0, 0);
            _vhcl->scale_fx_p1 = parser.stof(pp1, 0);
            _vhcl->scale_fx_p2 = parser.stof(pp2, 0);
            _vhcl->scale_fx_p3 = parser.stol(pp3, NULL, 0);

            int tmp = 0;
            while ( stok.GetNext(&pp0) )
            {
                _vhcl->scale_fx_pXX[tmp] = parser.stol(pp0, NULL, 0);
                tmp++;
            }
        }
    }
    else if ( !StriCmp(p1, "robo_data_slot") )
    {
    }
    else if ( !StriCmp(p1, "robo_num_guns") )
    {
        robo->guns.resize( parser.stol(p2, NULL, 0) );
    }
    else if ( !StriCmp(p1, "robo_act_gun") )
    {
        _gunID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_x") )
    {
        robo->guns[ _gunID ].pos.x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_y") )
    {
        robo->guns[ _gunID ].pos.y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_z") )
    {
        robo->guns[ _gunID ].pos.z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_x") )
    {
        robo->guns[ _gunID ].dir.x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_y") )
    {
        robo->guns[ _gunID ].dir.y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_z") )
    {
        robo->guns[ _gunID ].dir.z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_type") )
    {
        robo->guns[ _gunID ].robo_gun_type = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_name") )
    {
        robo->guns[ _gunID ].robo_gun_name = p2;
    }
    else if ( !StriCmp(p1, "robo_dock_x") )
    {
        robo->dock.x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_dock_y") )
    {
        robo->dock.y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_dock_z") )
    {
        robo->dock.z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_num") )
    {
        robo->coll.roboColls.resize( parser.stol(p2, NULL, 0) );
    }
    else if ( !StriCmp(p1, "robo_coll_act") )
    {
        _collID = parser.stol(p2, NULL, 0);         
    }
    else if ( !StriCmp(p1, "robo_coll_radius") )
    {
        robo->coll.roboColls[ _collID ].robo_coll_radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_x") )
    {
        robo->coll.roboColls[ _collID ].coll_pos.x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_y") )
    {
        robo->coll.roboColls[ _collID ].coll_pos.y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_z") )
    {
        robo->coll.roboColls[ _collID ].coll_pos.z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_x") )
    {
        robo->viewer.x = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_y") )
    {
        robo->viewer.y = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_z") )
    {
        robo->viewer.z = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_up") )
    {
        robo->robo_viewer_max_up = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_down") )
    {
        robo->robo_viewer_max_down = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_side") )
    {
        robo->robo_viewer_max_side = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_does_twist") )
    {
        _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_WAIT_ROTATE, (int32_t)1);
    }
    else if ( !StriCmp(p1, "robo_does_flux") )
    {
        _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_WAIT_SWAY, (int32_t)1);
    }
    else if ( !StriCmp(p1, "hidden") )
    {
        _vhcl->hidden = StrGetBool(p2);
    }
    else if ( !StriCmp(p1, "unhide_radar") )
    {
        _vhcl->unhideRadar = parser.stol(p2, NULL, 0);
        
        if (_vhcl->unhideRadar < 0)
            _vhcl->unhideRadar = 0;
        else if (_vhcl->unhideRadar > _vhcl->radar)
            _vhcl->unhideRadar = _vhcl->radar + 1;
    }
    else if ( !StriCmp(p1, "add_unhide_radar") )
    {
        _vhcl->unhideRadar += parser.stol(p2, NULL, 0);
        
        if (_vhcl->unhideRadar < 0)
            _vhcl->unhideRadar = 0;
        else if (_vhcl->unhideRadar > _vhcl->radar)
            _vhcl->unhideRadar = _vhcl->radar + 1;
    }
    else
        return ParseSndFX(parser, p1, p2);

    return ScriptParser::RESULT_OK;
}


bool VhclProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( !StriCmp(word, "new_vehicle") )
    {
        _roboTmp = TRoboProto();
        _vhclID = parser.stol(opt, NULL, 0);
        _vhcl = &_o._vhclProtos.at(_vhclID);
        
        *_vhcl = TVhclProto();
        
        _vhcl->Index = _vhclID;

        _vhcl->model_id = BACT_TYPES_TANK;
        _vhcl->weapon = -1;
        _vhcl->mgun = -1;
        _vhcl->type_icon = 65;
        _vhcl->vp_normal = 0;
        _vhcl->vp_fire = 1;
        _vhcl->vp_megadeth = 2;
        _vhcl->vp_wait = 3;
        _vhcl->vp_dead = 4;
        _vhcl->vp_genesis = 5;
        _vhcl->shield = 50;
        _vhcl->energy = 10000;
        _vhcl->adist_sector = 800.0;
        _vhcl->adist_bact = 650.0;
        _vhcl->sdist_sector = 200.0;
        _vhcl->sdist_bact = 100.0;
        _vhcl->radar = 1;
        _vhcl->kill_after_shot = 0;
        _vhcl->mass = 400.0;
        _vhcl->force = 5000.0;
        _vhcl->airconst = 80.0;
        _vhcl->maxrot = 0.8;
        _vhcl->height = 150.0;
        _vhcl->radius = 25.0;
        _vhcl->overeof = 25.0;
        _vhcl->vwr_radius = 30.0;
        _vhcl->vwr_overeof = 30.0;
        _vhcl->gun_power = 4000.0;
        _vhcl->gun_radius = 5.0;
        _vhcl->max_pitch = -1.0;
        _vhcl->job_fightflyer = 0;
        _vhcl->job_fighthelicopter = 0;
        _vhcl->job_fightrobo = 0;
        _vhcl->job_fighttank = 0;
        _vhcl->job_reconnoitre = 0;
        _vhcl->job_conquer = 0;

        for (auto &x : _vhcl->sndFX)
        {
            x.sndPrm.mag0 = 1.0;
            x.sndPrm_shk.mag0 = 1.0;
            x.sndPrm_shk.mute = 0.02;
            x.sndPrm_shk.pos.x = 0.2;
            x.sndPrm_shk.pos.y = 0.2;
            x.sndPrm_shk.pos.z = 0.2;
            x.volume = 120;
            x.sndPrm.time = 1000;
            x.sndPrm_shk.time = 1000;
        }

        _vhcl->initParams.clear();
        return true;
    }
    else if ( !StriCmp(word, "modify_vehicle") )
    {
        _vhclID = parser.stol(opt, NULL, 0);
        _vhcl = &_o._vhclProtos.at(_vhclID);
        
        _vhcl->Index = _vhclID;
        
        _o._upgradeVehicleId = _vhclID;
        return true;
    }

    return false;
}

TVhclSound *WeaponProtoParser::GetSndFxByName(const std::string &sndname)
{
    if ( !StriCmp(sndname, "normal") )
        return &_wpn->sndFXes[TWeapProto::SND_NORMAL];
    else if ( !StriCmp(sndname, "launch") )
        return &_wpn->sndFXes[TWeapProto::SND_LAUNCH];
    else if ( !StriCmp(sndname, "hit") )
        return &_wpn->sndFXes[TWeapProto::SND_HIT];

    return NULL;
}

bool WeaponProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (!StriCmp(word, "new_weapon"))
    {
        int wpnId = parser.stol(opt, NULL, 0);
        _wpn = &_o._weaponProtos[wpnId];

        *_wpn = TWeapProto();
        
        _wpn->unitID = 4;
        _wpn->name.clear();
        _wpn->energy = 10000;
        _wpn->mass = 50.0;
        _wpn->force = 5000.0;
        _wpn->airconst = 50.0;
        _wpn->maxrot = 2.0;
        _wpn->radius = 20.0;
        _wpn->overeof = 10.0;
        _wpn->vwr_radius = 20.0;
        _wpn->vwr_overeof = 20.0;
        _wpn->energy_heli = 1.0;
        _wpn->energy_tank = 1.0;
        _wpn->energy_flyer = 1.0;
        _wpn->energy_robo = 1.0;
        _wpn->radius_heli = 0;
        _wpn->radius_tank = 0;
        _wpn->radius_flyer = 0;
        _wpn->radius_robo = 0;
        _wpn->start_speed = 70.0;
        _wpn->life_time = 20000;
        _wpn->life_time_nt = 0;
        _wpn->drive_time = 7000;
        _wpn->shot_time = 3000;
        _wpn->shot_time_user = 1000;
        _wpn->salve_delay = 0;
        _wpn->salve_shots = 0;
        _wpn->vp_normal = 0;
        _wpn->vp_fire = 1;
        _wpn->vp_megadeth = 2;
        _wpn->vp_wait = 3;
        _wpn->vp_dead = 4;
        _wpn->vp_genesis = 5;
        _wpn->type_icon = 65;

        for (TVhclSound &fx : _wpn->sndFXes)
        {
            fx.sndPrm.mag0 = 1.0;
            fx.sndPrm_shk.mag0 = 1.0;
            fx.sndPrm_shk.mute = 0.02;
            fx.sndPrm_shk.pos.x = 0.2;
            fx.sndPrm_shk.pos.y = 0.2;
            fx.sndPrm_shk.pos.z = 0.2;
            fx.volume = 120;
            fx.sndPrm.time = 1000;
            fx.sndPrm_shk.time = 1000;
        }

        _wpn->initParams.clear();
        return true;
    }
    else if (!StriCmp(word, "modify_weapon"))
    {
        int wpnId = parser.stol(opt, NULL, 0);
        _wpn = &_o._weaponProtos[wpnId];

        _o._upgradeWeaponId = wpnId;
        return true;
    }

    return false;
}

int WeaponProtoParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "model") )
    {
        if ( !StriCmp(p2, "grenade") )
            _wpn->_weaponFlags = 17;
        else if ( !StriCmp(p2, "rocket") )
            _wpn->_weaponFlags = 3;
        else if ( !StriCmp(p2, "missile") )
            _wpn->_weaponFlags = 7;
        else if ( !StriCmp(p2, "bomb") || !StriCmp(p2, "special") )
            _wpn->_weaponFlags = 1;
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _wpn->enable_mask |= 1 << parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _wpn->enable_mask &= ~(1 << parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _wpn->name = p2;
        std::replace(_wpn->name.begin(), _wpn->name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _wpn->energy = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "energy_heli") )
    {
        _wpn->energy_heli = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_tank") )
    {
        _wpn->energy_tank = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_flyer") )
    {
        _wpn->energy_flyer = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_robo") )
    {
        _wpn->energy_robo = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "mass") )
    {
        _wpn->mass = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "force") )
    {
        _wpn->force = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "maxrot") )
    {
        _wpn->maxrot = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "airconst") )
    {
        _wpn->airconst = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _wpn->radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_heli") )
    {
        _wpn->radius_heli = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_tank") )
    {
        _wpn->radius_tank = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_flyer") )
    {
        _wpn->radius_flyer = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_robo") )
    {
        _wpn->radius_robo = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "overeof") )
    {
        _wpn->overeof = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_radius") )
    {
        _wpn->vwr_radius = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_overeof") )
    {
        _wpn->vwr_overeof = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "start_speed") )
    {
        _wpn->start_speed = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "life_time") )
    {
        _wpn->life_time = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "life_time_nt") )
    {
        _wpn->life_time_nt = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "drive_time") )
    {
        _wpn->drive_time = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "delay_time") )
    {
        _wpn->delay_time = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shot_time") )
    {
        _wpn->shot_time = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shot_time_user") )
    {
        _wpn->shot_time_user = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "salve_shots") )
    {
        _wpn->salve_shots = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "salve_delay") )
    {
        _wpn->salve_delay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy") )
    {
        _wpn->energy += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_heli") )
    {
        _wpn->energy_heli += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_tank") )
    {
        _wpn->energy_tank += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_flyer") )
    {
        _wpn->energy_flyer += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_Robo") )
    {
        _wpn->energy_robo += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shot_time") )
    {
        _wpn->shot_time += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shot_time_user") )
    {
        _wpn->shot_time_user += parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_normal") )
    {
        _wpn->vp_normal = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_fire") )
    {
        _wpn->vp_fire = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_megadeth") )
    {
        _wpn->vp_megadeth = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_wait") )
    {
        _wpn->vp_wait = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_dead") )
    {
        _wpn->vp_dead = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_genesis") )
    {
        _wpn->vp_genesis = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type_icon") )
    {
        _wpn->type_icon = p2[0];
    }
    else if ( !StriCmp(p1, "wireframe") )
    {
        if ( _wpn->wireframe )
            _wpn->wireframe->Delete();

        _wpn->wireframe = Nucleus::CInit<NC_STACK_sklt>( {{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(p2)}} );
    }
    else if ( !StriCmp(p1, "dest_fx") )
    {
        Stok stok(p2, " _");
        std::string fx_type, pp1, pp2, pp3, pp4;

        if ( stok.GetNext(&fx_type) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) && stok.GetNext(&pp4) )
        {
            _wpn->dfx.emplace_back();
            DestFX &dfx = _wpn->dfx.back();
            dfx.Type = DestFX::ParseTypeName(fx_type);

            if (dfx.Type == DestFX::FX_NONE)
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = parser.stol(pp1, NULL, 0);
            dfx.Pos.x = parser.stof(pp2, 0);
            dfx.Pos.y = parser.stof(pp3, 0);
            dfx.Pos.z = parser.stof(pp4, 0);
            
            std::string pp5;
            if ( stok.GetNext(&pp5) )
            {
                if (parser.stol(pp5, NULL, 0) != 0 )
                    dfx.Accel = true;
                else
                    dfx.Accel = false;
            }
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "ext_dest_fx") || !StriCmp(p1, "extended_dest_fx") )
    {
        Stok stok(p2, " _");
        std::string fx_type, pp1, pp2, pp3, pp4;

        if ( stok.GetNext(&fx_type) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) && stok.GetNext(&pp4) )
        {
            _wpn->ExtDestroyFX.emplace_back();
            
            DestFX &dfx = _wpn->ExtDestroyFX.back();
            
            dfx.Type = DestFX::ParseTypeName(fx_type);

            if (dfx.Type == DestFX::FX_NONE)
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = parser.stol(pp1, NULL, 0);
            dfx.Pos.x = parser.stof(pp2, 0);
            dfx.Pos.y = parser.stof(pp3, 0);
            dfx.Pos.z = parser.stof(pp4, 0);
            
            std::string pp5;
            if ( stok.GetNext(&pp5) )
            {
                if (parser.stol(pp5, NULL, 0) != 0 )
                    dfx.Accel = true;
                else
                    dfx.Accel = false;
            }
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else
        return ParseSndFX(parser, p1, p2);

    return ScriptParser::RESULT_OK;
}

bool BuildProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (!StriCmp(word, "new_building"))
    {
        int bldId = parser.stol(opt, NULL, 0);

        _o._buildProtos[bldId] = TBuildingProto();
        _bld = &_o._buildProtos[bldId];
        _bld->Index = bldId;
        _bld->Energy = 50000;
        _bld->TypeIcon = 65;
        _bld->SndFX.volume = 120;
        _bld->Guns.clear();
        return true;
    }
    else if (!StriCmp(word, "modify_building"))
    {
        int bldId = parser.stol(opt, NULL, 0);

        _bld = &_o._buildProtos[bldId];
        _bld->Index = bldId;
        _o._upgradeBuildId = bldId;
        return true;
    }

    return false;
}


int BuildProtoParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "model") )
    {
        if ( !StriCmp(p2, "building") )
        {
            _bld->ModelID = 0;
        }
        else if ( !StriCmp(p2, "kraftwerk") )
        {
            _bld->ModelID = 1;
        }
        else if ( !StriCmp(p2, "radar") )
        {
            _bld->ModelID = 2;
        }
        else if ( !StriCmp(p2, "defcenter") )
        {
            _bld->ModelID = 3;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _bld->EnableMask |= 1 << parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _bld->EnableMask &= ~(1 << parser.stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _bld->Name = p2;
        std::replace(_bld->Name.begin(), _bld->Name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "power") )
    {
        _bld->Power = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _bld->Energy = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_type") )
    {
        _bld->SecType = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type_icon") )
    {
        _bld->TypeIcon = p2[0];
    }
    else if ( !StriCmp(p1, "snd_normal_sample") )
    {
        _bld->SndFX.MainSample.Name = p2;
    }
    else if ( !StriCmp(p1, "snd_normal_volume") )
    {
        _bld->SndFX.volume = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "snd_normal_pitch") )
    {
        _bld->SndFX.pitch = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sbact_act") )
    {
        _gunID = parser.stol(p2, NULL, 0);
        if (_gunID >= _bld->Guns.size())
            _bld->Guns.resize(_gunID + 1);
    }
    else
    {
        TBuildingProto::TGun &pGun = _bld->Guns.at(_gunID);
        if ( !StriCmp(p1, "sbact_vehicle") )
        {
            pGun.VhclID = parser.stol(p2, NULL, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_x") )
        {
            pGun.Pos.x = parser.stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_y") )
        {
            pGun.Pos.y = parser.stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_z") )
        {
            pGun.Pos.z = parser.stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_x") )
        {
            pGun.Dir.x = parser.stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_y") )
        {
            pGun.Dir.y = parser.stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_z") )
        {
            pGun.Dir.z = parser.stof(p2, 0);
        }
        else
            return ScriptParser::RESULT_UNKNOWN;
    }

    return ScriptParser::RESULT_OK;
}

bool MovieParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_movies") )
        return false;

    for (std::string &movie : _o._movies)
        movie.clear();
    return true;
}

int MovieParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
	if ( !StriCmp(p1, "end") )
		return ScriptParser::RESULT_SCOPE_END;
	else if ( !StriCmp(p1, "game_intro") )
		_o._movies[World::MOVIE_INTRO] = p2;
	else if ( !StriCmp(p1, "win_extro") )
		_o._movies[World::MOVIE_WIN] = p2;
	else if ( !StriCmp(p1, "lose_extro") )
		_o._movies[World::MOVIE_LOSE] = p2;
	else if ( !StriCmp(p1, "user_intro") )
		_o._movies[World::MOVIE_USER] = p2;
	else if ( !StriCmp(p1, "kyt_intro") )
		_o._movies[World::MOVIE_KYT] = p2;
	else if ( !StriCmp(p1, "taer_intro") )
		_o._movies[World::MOVIE_TAER] = p2;
	else if ( !StriCmp(p1, "myk_intro") )
		_o._movies[World::MOVIE_MYK] = p2;
	else if ( !StriCmp(p1, "sulg_intro") )
		_o._movies[World::MOVIE_SULG] = p2;
	else if ( !StriCmp(p1, "black_intro") )
		_o._movies[World::MOVIE_BLACK] = p2;
	else
		return ScriptParser::RESULT_UNKNOWN;
	return ScriptParser::RESULT_OK;
}


BkgParser::BkgParser(NC_STACK_ypaworld *o)
: _o(o->_globalMapRegions)
{}

int BkgParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _o.NumSets++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "background_map") )
    {
        _o.background_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "rollover_map") )
    {
        _o.rollover_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "finished_map") )
    {
        _o.finished_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "enabled_map") )
    {
        _o.enabled_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "mask_map") )
    {
        _o.mask_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "tut_background_map") )
    {
        _o.tut_background_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "tut_rollover_map") )
    {
        _o.tut_rollover_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "tut_mask_map") )
    {
        _o.tut_mask_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "menu_map") )
    {
        _o.menu_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "input_map") )
    {
        _o.input_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "settings_map") )
    {
        _o.settings_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "network_map") )
    {
        _o.network_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "locale_map") )
    {
        _o.locale_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "save_map") )
    {
        _o.save_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "about_map") )
    {
        _o.about_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "help_map") )
    {
        _o.help_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "brief_map") )
    {
        _o.brief_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "debrief_map") )
    {
        _o.debrief_map[_o.NumSets].PicName = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _o.background_map[_o.NumSets].Size.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _o.background_map[_o.NumSets].Size.y = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


int ColorParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "owner_0") )
    {
        _o.ParseColorString(World::COLOR_OWNER_0, p2);
    }
    else if ( !StriCmp(p1, "owner_1") )
    {
        _o.ParseColorString(World::COLOR_OWNER_1, p2);
    }
    else if ( !StriCmp(p1, "owner_2") )
    {
        _o.ParseColorString(World::COLOR_OWNER_2, p2);
    }
    else if ( !StriCmp(p1, "owner_3") )
    {
        _o.ParseColorString(World::COLOR_OWNER_3, p2);
    }
    else if ( !StriCmp(p1, "owner_4") )
    {
        _o.ParseColorString(World::COLOR_OWNER_4, p2);
    }
    else if ( !StriCmp(p1, "owner_5") )
    {
        _o.ParseColorString(World::COLOR_OWNER_5, p2);
    }
    else if ( !StriCmp(p1, "owner_6") )
    {
        _o.ParseColorString(World::COLOR_OWNER_6, p2);
    }
    else if ( !StriCmp(p1, "owner_7") )
    {
        _o.ParseColorString(World::COLOR_OWNER_7, p2);
    }
    else if ( !StriCmp(p1, "map_direction") )
    {
        _o.ParseColorString(World::COLOR_MAP_DIRECTION, p2);
    }
    else if ( !StriCmp(p1, "map_primtarget") )
    {
        _o.ParseColorString(World::COLOR_MAP_PRIMTARGET, p2);
    }
    else if ( !StriCmp(p1, "map_sectarget") )
    {
        _o.ParseColorString(World::COLOR_MAP_SECTARGET, p2);
    }
    else if ( !StriCmp(p1, "map_commander") )
    {
        _o.ParseColorString(World::COLOR_MAP_COMMANDER, p2);
    }
    else if ( !StriCmp(p1, "map_dragbox") )
    {
        _o.ParseColorString(World::COLOR_MAP_DRAGBOX, p2);
    }
    else if ( !StriCmp(p1, "map_viewer") )
    {
        _o.ParseColorString(World::COLOR_MAP_VIEWER, p2);
    }
    else if ( !StriCmp(p1, "hud_weapon") )
    {
        _o.ParseColorString(World::COLOR_HUD_WEAPON_0, p2);
    }
    else if ( !StriCmp(p1, "hud_weapon_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_WEAPON_1, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_commandvec") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_CMDVEC_0, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_commandvec_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_CMDVEC_1, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_primtarget") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_PRIMTGT_0, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_primtarget_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_PRIMTGT_1, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_locktarget") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_LOCKTGT_0, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_locktarget_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_LOCKTGT_1, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_compass") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_0, p2);
    }
    else if ( !StriCmp(p1, "hud_compass_compass_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_COMPASS_1, p2);
    }
    else if ( !StriCmp(p1, "hud_vehicle") )
    {
        _o.ParseColorString(World::COLOR_HUD_VEHICLE_0, p2);
    }
    else if ( !StriCmp(p1, "hud_vehicle_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_VEHICLE_1, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_mg") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_MG_0, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_mg_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_MG_1, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_locked") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_LOCKED_0, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_locked_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_LOCKED_1, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_autonom") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_AUTONOM_0, p2);
    }
    else if ( !StriCmp(p1, "hud_visor_autonom_1") )
    {
        _o.ParseColorString(World::COLOR_HUD_VISOR_AUTONOM_1, p2);
    }
    else if ( !StriCmp(p1, "brief_lines") )
    {
        _o.ParseColorString(World::COLOR_BRIEF_LINES, p2);
    }
    else if ( !StriCmp(p1, "text_default") )
    {
        _o.ParseColorString(World::COLOR_TEXT_DEFAULT, p2);
    }
    else if ( !StriCmp(p1, "text_list") )
    {
        _o.ParseColorString(World::COLOR_TEXT_LIST, p2);
    }
    else if ( !StriCmp(p1, "text_list_sel") )
    {
        _o.ParseColorString(World::COLOR_TEXT_LIST_SEL, p2);
    }
    else if ( !StriCmp(p1, "text_tooltip") )
    {
        _o.ParseColorString(World::COLOR_TEXT_TOOLTIP, p2);
    }
    else if ( !StriCmp(p1, "text_message") )
    {
        _o.ParseColorString(World::COLOR_TEXT_MESSAGE, p2);
    }
    else if ( !StriCmp(p1, "text_hud") )
    {
        _o.ParseColorString(World::COLOR_TEXT_HUD, p2);
    }
    else if ( !StriCmp(p1, "text_briefing") )
    {
        _o.ParseColorString(World::COLOR_TEXT_BRIEFING, p2);
    }
    else if ( !StriCmp(p1, "text_debriefing") )
    {
        _o.ParseColorString(World::COLOR_TEXT_DEBRIEFING, p2);
    }
    else if ( !StriCmp(p1, "text_button") )
    {
        _o.ParseColorString(World::COLOR_TEXT_BUTTON, p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool MiscParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_misc") )
        return false;

    _o._beamEnergyStart = 500;
    _o._beamEnergyAdd = 100;
    _o._defaultUnitLimit = 512;
    _o._defaultUnitLimitType = 0;
    _o._defaultUnitLimitArg = 0;
    _o._easyCheatKeys = false;

    return true;
}

int MiscParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }
    else if ( !StriCmp(p1, "one_game_res") )
    {
        _o._oneGameRes = StrGetBool(p2);
    }
    else if ( !StriCmp(p1, "shell_default_res") )
    {
    	Stok stok(p2, "_ \t");
    	std::string pp1, pp2;
        if ( stok.GetNext(&pp1) && stok.GetNext(&pp2) )
        {
            _o._shellDefaultRes = Common::Point(parser.stol(pp1, NULL, 0), parser.stol(pp2, NULL, 0));
            _o._shellGfxMode = Common::Point(parser.stol(pp1, NULL, 0), parser.stol(pp2, NULL, 0));
        }
    }
    else if ( !StriCmp(p1, "game_default_res") )
    {
    	Stok stok(p2, "_ \t");
        std::string pp1, pp2;
        if ( stok.GetNext(&pp1) && stok.GetNext(&pp2) )
        {
            _o._gameDefaultRes = Common::Point(parser.stol(pp1, NULL, 0), parser.stol(pp2, NULL, 0));
            _o._gfxMode = Common::Point(parser.stol(pp1, NULL, 0), parser.stol(pp2, NULL, 0));
        }
    }
    else if ( !StriCmp(p1, "max_impulse") )
    {
        _o._maxImpulse = parser.stof(p2);
    }
    else if ( !StriCmp(p1, "unit_limit") )
    {
        _o._defaultUnitLimit = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_type") )
    {
        _o._defaultUnitLimitType = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_arg") )
    {
        _o._defaultUnitLimitArg = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "beam_energy_start") )
    {
        _o._beamEnergyStart = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "beam_energy_add") )
    {
        _o._beamEnergyAdd = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "easy_cheat_keys") )
    {
        _o._easyCheatKeys = parser.stol(p2, NULL, 0) != 0;
    }
    else if ( !StriCmp(p1, "multi_building") )
    {
        _o._allowMultiBuildWorld = StrGetBool(p2);
    }
    else if ( !StriCmp(p1, "hidden_fractions") )
    {
        _o._worldHiddenFractions = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool SuperItemParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_superitem") )
        return false;

    _o._stoudsonWaveVehicleId = 0;
    _o._stoudsonCenterVehicleId = 0;
    return true;
}

int SuperItemParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }
	else if ( !StriCmp(p1, "superbomb_center_vproto") )
    {
        _o._stoudsonCenterVehicleId = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "superbomb_wall_vproto") )
    {
        _o._stoudsonWaveVehicleId = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}


Common::Point MapSizesParser::ParseSizes(ScriptParser::Parser &parser)
{
    std::string tmp;
    parser.ReadLine(&tmp);

    Stok stok(tmp, " \r\n");

    std::string sX, sY;
    stok.GetNext(&sX);
    stok.GetNext(&sY);

    int32_t x = parser.stol(sX, NULL, 0);
    int32_t y = parser.stol(sY, NULL, 0);

    for(int i = 0; i < y; i++)
        parser.ReadLine(&tmp);

    return Common::Point(x, y);
}


int MapSizesParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "typ_map") )
    {
        _m.MapSize = ParseSizes(parser);
    }
    else if ( !StriCmp(p1, "own_map") || !StriCmp(p1, "hgt_map") || !StriCmp(p1, "blg_map") )
    {
        ParseSizes(parser);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool LevelDataParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_level") )
        return false;

    _o._levelInfo.MapName = "<NO NAME>";
    _o._levelInfo.MovieStr.clear();
    _o._levelInfo.MovieWinStr.clear();
    _o._levelInfo.MovieLoseStr.clear();
    _o._vehicleSectorRatio = 0;
    _o._levelUnitLimit = _o._defaultUnitLimit;
    _o._allowMultiBuildLevel = _o._allowMultiBuildWorld;
    _o._levelUnitLimitType = _o._defaultUnitLimitType;
    _o._levelUnitLimitArg = _o._defaultUnitLimitArg;
    _o._luaScriptName = "";
    return true;
}

int LevelDataParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( p1.find("title_") != std::string::npos )
    {
        std::string title_lang = std::string("title_") + Locale::Text::GetLocaleName();

        if ( !StriCmp(p1, "title_default") || !StriCmp(p1, title_lang) )
                _o._levelInfo.MapName = p2;
    }
    else if ( !StriCmp(p1, "set") )
    {
        _m.SetID = parser.stol(p2, NULL, 0);
        _m.ReadedPartsBits |= TLevelDescription::BIT_SET;
    }
    else if ( !StriCmp(p1, "sky") )
    {
        _m.SkyStr = p2;
        _m.ReadedPartsBits |= TLevelDescription::BIT_SKY;
    }
    else if ( !StriCmp(p1, "typ") )
    {
        _m.TypStr = p2;
        _m.ReadedPartsBits |= TLevelDescription::BIT_TYP;
    }
    else if ( !StriCmp(p1, "own") )
    {
        _m.OwnStr = p2;
        _m.ReadedPartsBits |= TLevelDescription::BIT_OWN;
    }
    else if ( !StriCmp(p1, "hgt") )
    {
        _m.HgtStr = p2;
        _m.ReadedPartsBits |= TLevelDescription::BIT_HGT;
    }
    else if ( !StriCmp(p1, "blg") )
    {
        _m.BlgStr = p2;
        _m.ReadedPartsBits |= TLevelDescription::BIT_BLG;
    }
    else if ( !StriCmp(p1, "palette") )
    {
        _m.Palettes[0] = p2;
    }
    else if ( !StriCmp(p1, "slot0") )
    {
        _m.Palettes[0] = p2;
    }
    else if ( !StriCmp(p1, "slot1") )
    {
        _m.Palettes[1] = p2;
    }
    else if ( !StriCmp(p1, "slot2") )
    {
        _m.Palettes[2] = p2;
    }
    else if ( !StriCmp(p1, "slot3") )
    {
        _m.Palettes[3] = p2;
    }
    else if ( !StriCmp(p1, "slot4") )
    {
        _m.Palettes[4] = p2;
    }
    else if ( !StriCmp(p1, "slot5") )
    {
        _m.Palettes[5] = p2;
    }
    else if ( !StriCmp(p1, "slot6") )
    {
        _m.Palettes[6] = p2;
    }
    else if ( !StriCmp(p1, "slot7") )
    {
        _m.Palettes[7] = p2;
    }
    else if ( !StriCmp(p1, "script") )
    {
        if ( !_o.LoadProtosScript(p2) )
            return ScriptParser::RESULT_BAD_DATA;
        return ScriptParser::RESULT_OK;
    }
    else if ( !StriCmp(p1, "ambiencetrack") )
    {
        _o._levelInfo.MusicTrackMinDelay = 0;
        _o._levelInfo.MusicTrackMaxDelay = 0;

        Stok stok(p2, " \t_\n");
        std::string tmp;
        stok.GetNext(&tmp);
        _o._levelInfo.MusicTrack = parser.stol(tmp, NULL, 0);

        if ( stok.GetNext(&tmp) )
        {
            _o._levelInfo.MusicTrackMinDelay = parser.stol(tmp, NULL, 0);

            if ( stok.GetNext(&tmp) )
                _o._levelInfo.MusicTrackMaxDelay = parser.stol(tmp, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "movie") )
    {
        _o._levelInfo.MovieStr = p2;
    }
    else if ( !StriCmp(p1, "win_movie") )
    {
        _o._levelInfo.MovieWinStr = p2;
    }
    else if ( !StriCmp(p1, "lose_movie") )
    {
        _o._levelInfo.MovieLoseStr = p2;
    }
    else if ( !StriCmp(p1, "event_loop") )
    {
        _m.EventLoopID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "slow_connection") )
    {
        if ( StrGetBool(p2) )
        {
            _m.SlowConnection = true;
        }
        else
        {
            _m.SlowConnection = false;
        }
    }
    else if ( !StriCmp(p1, "vehicle_sector_ratio") )
    {
        _o._vehicleSectorRatio = parser.stof(p2, 0);
    }
    else if ( !StriCmp(p1, "unit_limit") )
    {
        _o._levelUnitLimit = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_type") )
    {
        _o._levelUnitLimitType = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_arg") )
    {
        _o._levelUnitLimitArg = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "lua_script") )
    {
        _o._luaScriptName = p2;
    }
    else if ( !StriCmp(p1, "multi_building") )
    {
        _o._allowMultiBuildLevel = StrGetBool(p2);
    }
    else if ( !StriCmp(p1, "hidden_fractions") )
    {
        _o._hiddenFractions = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool MapRobosParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (StriCmp(word, "begin_robo"))
        return false;

    _m.Robos.emplace_back(); // Construct new element
    _r = &_m.Robos.back();
    _r->MbStatus = 0;
    return true;
}


int MapRobosParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if (_m.Robos.size() == 1) //If it's first host station - save owner for brief
            _m.PlayerOwner = _r->Owner;

        _m.ReadedPartsBits |= TLevelDescription::BIT_END;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "owner") )
    {
        _r->Owner = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vehicle") )
    {
        _r->VhclID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_x") )
    {
        int tmp = parser.stol(p2, NULL, 0);
        _r->Pos.y = -300;
        _r->Pos.x = tmp * CVSectorLength + CVSectorHalfLength;
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        int tmp = parser.stol(p2, NULL, 0);
        _r->Pos.y = -300;
        _r->Pos.z = -(tmp * CVSectorLength + CVSectorHalfLength);
    }
    else if ( !StriCmp(p1, "pos_x") )
    {
        _r->Pos.x = parser.stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_y") )
    {
        _r->Pos.y = parser.stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_z") )
    {
        _r->Pos.z = parser.stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _r->Energy = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "con_budget") )
    {
        _r->ConBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rad_budget") )
    {
        _r->RadBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pow_budget") )
    {
        _r->PowBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "def_budget") )
    {
        _r->DefBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "saf_budget") )
    {
        _r->SafBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rec_budget") )
    {
        _r->RecBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "cpl_budget") )
    {
        _r->CplBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rob_budget") )
    {
        _r->RobBudget = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "viewangle") )
    {
        _r->ViewAngle = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "saf_delay") )
    {
        _r->SafDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pow_delay") )
    {
        _r->PowDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rad_delay") )
    {
        _r->RadDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "cpl_delay") )
    {
        _r->CplDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "def_delay") )
    {
        _r->DefDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "con_delay") )
    {
        _r->ConDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rec_delay") )
    {
        _r->RecDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rob_delay") )
    {
        _r->RobDelay = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _r->MbStatus = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _r->MbStatus = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _r->MbStatus = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "reload_const") )
    {
        _r->ReloadConst = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




int ShellSoundParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    struct ShellSoundNames
    {
        const std::string name;
        const int id;
    };

    static const ShellSoundNames block1[] =
    {
        {"volume", World::SOUND_ID_VOLUME},
        {"right", World::SOUND_ID_RIGHT},
        {"left", World::SOUND_ID_LEFT},
        {"button", World::SOUND_ID_BUTTON},
        {"quit", World::SOUND_ID_QUIT},
        {"slider", World::SOUND_ID_SLIDER},
        {"welcome", World::SOUND_ID_WELCOME},
        {"menuopen", World::SOUND_ID_MENUOPEN},
        {"overlevel", World::SOUND_ID_OVERLEVEL},
        {"levelselect", World::SOUND_ID_LEVELSEL},
        {"textappear", World::SOUND_ID_TEXTAPPEAR},
        {"objectappear", World::SOUND_ID_OBJAPPEAR},
        {"sectorconquered", World::SOUND_ID_SECTCONQ},
        {"vhcldestroyed", World::SOUND_ID_VHCLDESTR},
        {"bldgconquered", World::SOUND_ID_BLDGCONQ},
        {"timercount", World::SOUND_ID_TIMERCOUNT},
        {"select", World::SOUND_ID_SELECT},
        {"error", World::SOUND_ID_ERROR},
        {"attention", World::SOUND_ID_ATTEN},
        {"secret", World::SOUND_ID_SECRET},
        {"plasma", World::SOUND_ID_PLASMA}
    };

    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }
    else
    {
        std::string sm, tp;
        Stok stok(p1, "_");

        if (stok.GetNext(&sm) && stok.GetNext(&tp))
        {
            for (auto &t: block1)
            {
                if ( !StriCmp(t.name, sm) )
                {
                    if (!StriCmp("sample", tp))
                        return ( _o.LoadSample(t.id, p2) ? ScriptParser::RESULT_OK : ScriptParser::RESULT_BAD_DATA );
                    else if (!StriCmp("volume", tp))
                        _o.samples1_info.Sounds[t.id].Volume = parser.stoi(p2);
                    else if (!StriCmp("pitch", tp))
                        _o.samples1_info.Sounds[t.id].Pitch = parser.stoi(p2);
                    else
                        return ScriptParser::RESULT_UNKNOWN;

                    return ScriptParser::RESULT_OK;
                }
            }
        }

        return ScriptParser::RESULT_UNKNOWN;
    }

    return ScriptParser::RESULT_OK;
}

int ShellTracksParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    Stok stok(p2, " \t_\n");

    if ( !StriCmp(p1, "shelltrack") )
    {
        _o.shelltrack__adv.min_delay = 0;
        _o.shelltrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.shelltrack = parser.stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.shelltrack__adv.min_delay = parser.stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.shelltrack__adv.max_delay = parser.stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "missiontrack") )
    {
        _o.missiontrack__adv.min_delay = 0;
        _o.missiontrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.missiontrack = parser.stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.missiontrack__adv.min_delay = parser.stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.missiontrack__adv.max_delay = parser.stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "debriefingtrack") )
    {
        _o.debriefingtrack__adv.min_delay = 0;
        _o.debriefingtrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.debriefingtrack = parser.stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.debriefingtrack__adv.min_delay = parser.stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.debriefingtrack__adv.max_delay = parser.stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "loadingtrack") )
    {
        _o.loadingtrack__adv.min_delay = 0;
        _o.loadingtrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.loadingtrack = parser.stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.loadingtrack__adv.min_delay = parser.stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.loadingtrack__adv.max_delay = parser.stol(val, NULL, 0);
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}



bool LevelSquadParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (StriCmp(word, "begin_squad"))
        return false;

    _m.Squads.emplace_back();
    _s = &_m.Squads.back();
    return true;
}

int LevelSquadParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_s->VhclID )
        {
            ypa_log_out("Squad init: squad[%d]аno vehicle defined!\n", _m.Squads.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->Count )
        {
            ypa_log_out("Squad init: squad[%d] num of vehicles is 0!\n", _m.Squads.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _s->X == 0.0 || _s->Z == 0.0 )
        {
            ypa_log_out("Squad init: squad[%d] no pos given!\n", _m.Squads.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }
        
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "owner") )
    {
        _s->Owner = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "useable") )
    {
        _s->Useable = true;
    }
    else if ( !StriCmp(p1, "vehicle") )
    {
        _s->VhclID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "num") )
    {
        _s->Count = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pos_x") )
    {
        _s->X = parser.stod(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_z") )
    {
        _s->Z = parser.stod(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _s->MbStatus = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _s->MbStatus = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _s->MbStatus = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool LevelGatesParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_gate") )
        return false;

    _o._levelInfo.Gates.emplace_back();
    _g = &_o._levelInfo.Gates.back();
    _g->MbStatus = 0;
    return true;
}

int LevelGatesParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_g->ClosedBldID )
        {
            ypa_log_out("Gate init: gate[%d] no closed building defined!\n", _o._levelInfo.Gates.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_g->OpenBldID )
        {
            ypa_log_out("Gate init: gate[%d] no opened building defined!\n", _o._levelInfo.Gates.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _g->CellId.x == 0 || _g->CellId.y == 0)
        {
            ypa_log_out("Gate init: gate[%d] no sector coords!\n", _o._levelInfo.Gates.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_g->PassToLevels.size() )
        {
            ypa_log_out("Gate init: gate[%d] no target levels defined!\n", _o._levelInfo.Gates.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "sec_x") )
    {
        _g->CellId.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _g->CellId.y = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "closed_bp") )
    {
        _g->ClosedBldID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "opened_bp") )
    {
        _g->OpenBldID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "target_level") )
    {
        _g->PassToLevels.push_back( parser.stol(p2, NULL, 0) );
    }
    else if ( !StriCmp(p1, "keysec_x") )
    {
        _g->KeySectors.emplace_back();
        _g->KeySectors.back().CellId.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "keysec_y") )
    {
        _g->KeySectors.back().CellId.y = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _g->MbStatus = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _g->MbStatus = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _g->MbStatus = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}





bool LevelMbMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_mbmap") )
        return false;
    
    _m.Mbmaps.emplace_back();
    _d = &_m.Mbmaps.back();
    return true;
}

int LevelMbMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "name") )
    {
        _d->PicName = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _d->Size.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _d->Size.y = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




bool LevelGemParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_gem") )
        return false;

    _o._techUpgrades.emplace_back();
    _g = &_o._techUpgrades.back();
    _g->MbStatus = 0;
    return true;
}

int LevelGemParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_g->BuildingID )
        {
            ypa_log_out("WStein init: gem[%d] no building defined!\n", _o._techUpgrades.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _g->CellId.x == 0 || _g->CellId.y == 0 )
        {
            ypa_log_out("WStein init: gem[%d] sector pos wonky tonk!\n", _o._techUpgrades.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( p1.find("msg_") != std::string::npos )
    {
        std::string tmp = fmt::sprintf("msg_%s", Locale::Text::GetLocaleName());

        if ( !StriCmp(p1, "msg_default") || !StriCmp(p1, tmp) )
            _g->MsgDefault = p2;
    }
    else if ( !StriCmp(p1, "sec_x") )
    {
        _g->CellId.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _g->CellId.y = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "building") )
    {
        _g->BuildingID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type") )
    {
        _g->Type = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "script") )
    {
        _g->ScriptFile = p2;

        FSMgr::FileHandle *tmp = uaOpenFileAlloc(_g->ScriptFile, "r");

        if ( !tmp )
            return ScriptParser::RESULT_BAD_DATA;

        delete tmp;
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _g->MbStatus = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _g->MbStatus = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _g->MbStatus = 2;
        }
        else
        {
            return ScriptParser::RESULT_BAD_DATA;
        }
    }
    else if ( !StriCmp(p1, "nw_vproto_num") )
    {
        Stok stok(p2, "_ \t");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            _g->NwVprotoNum1 = parser.stol(tmp, NULL, 0);
            if ( stok.GetNext(&tmp) )
            {
                _g->NwVprotoNum2 = parser.stol(tmp, NULL, 0);
                if ( stok.GetNext(&tmp) )
                {
                    _g->NwVprotoNum3 = parser.stol(tmp, NULL, 0);
                    if ( stok.GetNext(&tmp) )
                        _g->NwVprotoNum4 = parser.stol(tmp, NULL, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "nw_bproto_num") )
    {
        Stok stok(p2, "_ \t");
        std::string tmp;
        if ( stok.GetNext(&tmp) )
        {
            _g->NwBprotoNum1 = parser.stol(tmp, NULL, 0);
            if ( stok.GetNext(&tmp) )
            {
                _g->NwBprotoNum2 = parser.stol(tmp, NULL, 0);
                if ( stok.GetNext(&tmp) )
                {
                    _g->NwBprotoNum3 = parser.stol(tmp, NULL, 0);
                    if ( stok.GetNext(&tmp) )
                        _g->NwBprotoNum4 = parser.stol(tmp, NULL, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "begin_action") )
    {
        std::string tmp;

        while( parser.ReadLine(&tmp) && (tmp.find("end_action") == std::string::npos) )
            _g->ActionsList.push_back(tmp);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




bool LevelEnableParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_enable") )
        return false;

    _fraction = parser.stol(opt, NULL, 0);

    for (TVhclProto &vhcl : _o._vhclProtos)
        vhcl.disable_enable_bitmask &= ~(1 << _fraction);

    for (TBuildingProto &bld : _o._buildProtos)
        bld.EnableMask &= ~(1 << _fraction);

    return true;
}

int LevelEnableParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "vehicle") )
    {
        size_t id = parser.stol(p2, NULL, 0);
        if ( id >= 0 && id < _o._vhclProtos.size() ) //_o.ypaworld.VhclProtos.size() )
            _o._vhclProtos[id].disable_enable_bitmask |= (1 << _fraction);
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "building") )
    {
        size_t id = parser.stol(p2, NULL, 0);
        if ( id >= 0 && id < _o._buildProtos.size() )
            _o._buildProtos[id].EnableMask |= (1 << _fraction);
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}



bool LevelDebMapParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_dbmap") )
        return false;

    _m.Dbmaps.emplace_back();
    _d = &_m.Dbmaps.back();
    return true;
}

int LevelDebMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "name") )
    {
        _d->PicName = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _d->Size.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _d->Size.y = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool LevelSuperItemsParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_item") )
        return false;

    _o._levelInfo.SuperItems.emplace_back();
    
    _s = &_o._levelInfo.SuperItems.back();
    _s->Type = 0;
    _s->TimerValue = 60000; //1hour
    _s->State = TMapSuperItem::STATE_INACTIVE;
    _s->MbStatus = 0;
    return true;
}

int LevelSuperItemsParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( _s->CellId.x == 0 || _s->CellId.y == 0)
        {
            ypa_log_out("Super item #%d: invalid sector coordinates!\n", _o._levelInfo.SuperItems.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->InactiveBldID )
        {
            ypa_log_out("Super item #%d: no <inactive_bp> defined!\n", _o._levelInfo.SuperItems.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->ActiveBldID )
        {
            ypa_log_out("Super item #%d: no <active_bp> defined!\n", _o._levelInfo.SuperItems.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->TriggerBldID )
        {
            ypa_log_out("Super item #%d: no <trigger_bp> defined!\n", _o._levelInfo.SuperItems.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _s->Type != TMapSuperItem::TYPE_BOMB && _s->Type != TMapSuperItem::TYPE_WAVE )
        {
            ypa_log_out("Super item #%d: no valid <type> defined!\n", _o._levelInfo.SuperItems.size() - 1);
            return ScriptParser::RESULT_BAD_DATA;
        }

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "sec_x") )
    {
        _s->CellId.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _s->CellId.y = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "inactive_bp") )
    {
        _s->InactiveBldID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "active_bp") )
    {
        _s->ActiveBldID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "trigger_bp") )
    {
        _s->TriggerBldID = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "keysec_x") )
    {
        _s->KeySectors.emplace_back();
        _s->KeySectors.back().CellId.x = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "keysec_y") )
    {
        _s->KeySectors.back().CellId.y = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _s->MbStatus = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _s->MbStatus = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _s->MbStatus = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "type") )
    {
        _s->Type = parser.stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "countdown") )
    {
        _s->TimerValue = parser.stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

Common::PlaneBytes MapAsPlaneBytes::ReadMapAsPlaneBytes(ScriptParser::Parser &parser)
{
    std::string buf;
    parser.ReadLine(&buf);

    std::string tmp;
    Stok stok(buf, " \t\r\n");
    stok.GetNext(&tmp);
    int w = parser.stol(tmp, NULL, 0);
    stok.GetNext(&tmp);
    int h = parser.stol(tmp, NULL, 0);
    
    if (w <= 0 || h <= 0)
        return Common::PlaneBytes();
    
    Common::PlaneBytes bmp(w, h);

    for (int j = 0; j < h; j++)
    {
        parser.ReadLine(&buf);
        stok = buf;

        uint8_t *ln = bmp.Line(j);

        for (int i = 0; i < w; i++)
        {
            stok.GetNext(&tmp);
            ln[i] = parser.stol(tmp, NULL, 16);
        }
    }

    return bmp;
}

bool LevelMapsParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_maps") )
        return false;

    _o._lvlTypeMap.Clear();
    _o._lvlOwnMap.Clear();
    _o._lvlHeightMap.Clear();
    _o._lvlBuildingsMap.Clear();

    return true;
}

int LevelMapsParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "typ_map") )
    {
        _o._lvlTypeMap = ReadMapAsPlaneBytes(parser);

        if ( !_o._lvlTypeMap.IsOk() )
            return ScriptParser::RESULT_BAD_DATA;

        _m.MapSize = _o._lvlTypeMap.Size();

        _m.ReadedPartsBits |= TLevelDescription::BIT_TYP;
    }
    else if ( !StriCmp(p1, "own_map") )
    {
        _o._lvlOwnMap = ReadMapAsPlaneBytes(parser);
        if ( !_o._lvlOwnMap.IsOk() )
            return ScriptParser::RESULT_BAD_DATA;

        _m.ReadedPartsBits |= TLevelDescription::BIT_OWN;
    }
    else if ( !StriCmp(p1, "hgt_map") )
    {
        _o._lvlHeightMap = ReadMapAsPlaneBytes(parser);
        if ( !_o._lvlHeightMap.IsOk() )
            return ScriptParser::RESULT_BAD_DATA;

        _m.ReadedPartsBits |= TLevelDescription::BIT_HGT;
    }
    else if ( !StriCmp(p1, "blg_map") )
    {
        _o._lvlBuildingsMap = ReadMapAsPlaneBytes(parser);
        if ( !_o._lvlBuildingsMap.IsOk() )
            return ScriptParser::RESULT_BAD_DATA;

        _m.ReadedPartsBits |= TLevelDescription::BIT_BLG;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}



int VideoParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( GFX::Engine.getWDD_drawPrim() )
            _o._GameShell->GFXFlags |= World::GFX_FLAG_DRAWPRIMITIVES;
        else
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_DRAWPRIMITIVES;

        if ( GFX::Engine.getWDD_16bitTex() )
            _o._GameShell->GFXFlags |= World::GFX_FLAG_16BITTEXTURE;
        else
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_16BITTEXTURE;
        
        return ScriptParser::RESULT_SCOPE_END;
    }

    _o._GameShell->savedDataFlags |= World::SDF_VIDEO;

    if ( !StriCmp(p1, "videomode") )
    {
        int modeid = parser.stoi(p2);
        _o._gameDefaultRes = Common::Point((modeid >> 12 & 0xFFF), (modeid & 0xFFF));
        _o._GameShell->game_default_res = modeid;
    }
    else if ( !StriCmp(p1, "farview") )
    {
        if ( StrGetBool(p2) )
        {
            _o._GameShell->GFXFlags |= World::GFX_FLAG_FARVIEW;
            _o.SetFarView(true);
        }
        else
        {
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_FARVIEW;
            _o.SetFarView(false);
        }
    }
    else if ( !StriCmp(p1, "filtering") )
    {
    }
    else if ( !StriCmp(p1, "drawprimitive") )
    {
        if ( StrGetBool(p2) )
            _o._GameShell->GFXFlags |= World::GFX_FLAG_DRAWPRIMITIVES;
        else
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_DRAWPRIMITIVES;
    }
    else if ( !StriCmp(p1, "16bittexture") )
    {
        if ( StrGetBool(p2) )
            _o._GameShell->GFXFlags |= World::GFX_FLAG_16BITTEXTURE;
        else
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_16BITTEXTURE;
    }
    else if ( !StriCmp(p1, "softmouse") )
    {
        if ( StrGetBool(p2) )
        {
            _o._GameShell->GFXFlags |= World::GFX_FLAG_SOFTMOUSE;
            _o._preferences |= World::PREF_SOFTMOUSE;

            GFX::Engine.setWDD_cursor(1);
        }
        else
        {
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_SOFTMOUSE;
            _o._preferences &= ~World::PREF_SOFTMOUSE;

            GFX::Engine.setWDD_cursor(0);
        }
    }
    else if ( !StriCmp(p1, "palettefx") )
    {
    }
    else if ( !StriCmp(p1, "heaven") )
    {
        if ( StrGetBool(p2) )
        {
            _o._GameShell->GFXFlags |= World::GFX_FLAG_SKYRENDER;
            _o.setYW_skyRender(true);
        }
        else
        {
            _o._GameShell->GFXFlags &= ~World::GFX_FLAG_SKYRENDER;
            _o.setYW_skyRender(false);
        }
    }
    else if ( !StriCmp(p1, "fxnumber") )
    {
        _o._fxLimit = parser.stoi(p2);
        _o._GameShell->fxnumber = _o._fxLimit;
    }
    else if ( !StriCmp(p1, "enemyindicator") )
    {
        if ( StrGetBool(p2) )
        {
            _o._preferences |= World::PREF_ENEMYINDICATOR;
            _o._GameShell->enemyIndicator = true;
        }
        else
        {
            _o._preferences &= ~World::PREF_ENEMYINDICATOR;
            _o._GameShell->enemyIndicator = false;
        }
    }
    else if ( !StriCmp(p1, "gfxmode") )
    {
        Stok stok(p2, " _");
        std::string resW, resH, resWin;
        
        if ( stok.GetNext(&resW) && stok.GetNext(&resH) && stok.GetNext(&resWin))
        {
            int w = parser.stoi(resW);
            int h = parser.stoi(resH);
            int win = parser.stoi(resWin);
            
            const std::vector<GFX::GfxMode> &pModes = GFX::GFXEngine::Instance.GetAvailableModes();
            for (size_t i = 0; i < pModes.size(); ++i)
            {
                const GFX::GfxMode &mode = pModes.at(i);
                if (mode.w == w && mode.h == h)
                {
                    _o._GameShell->_gfxModeIndex = i;
                    _o._gfxMode = mode;

                    if (win)
                        _o._GameShell->GFXFlags |= World::GFX_FLAG_WINDOWED;
                    else
                        _o._GameShell->GFXFlags &= ~World::GFX_FLAG_WINDOWED;

                    _o._GameShell->_gfxMode = mode;
                    break;
                }
            }
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}

int SoundParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    _o._GameShell->savedDataFlags |= World::SDF_SOUND;

    if ( !StriCmp(p1, "channels") )
    {
    }
    else if ( !StriCmp(p1, "volume") )
    {
        _o._GameShell->soundVolume = parser.stoi(p2);
        SFXEngine::SFXe.setMasterVolume(_o._GameShell->soundVolume);
    }
    else if ( !StriCmp(p1, "cdvolume") )
    {
        _o._GameShell->musicVolume = parser.stoi(p2);
        SFXEngine::SFXe.SetMusicVolume(_o._GameShell->musicVolume);
    }
    else if ( !StriCmp(p1, "invertlr") )
    {
        if ( !StriCmp(p2, "yes") )
        {
            _o._GameShell->soundFlags |= World::SF_INVERTLR;
            SFXEngine::SFXe.setReverseStereo(true);
        }
        else
        {
            _o._GameShell->soundFlags &= ~World::SF_INVERTLR;
            SFXEngine::SFXe.setReverseStereo(false);
        }
    }
    else if ( !StriCmp(p1, "sound") )
    {
    }
    else if ( !StriCmp(p1, "cdsound") )
    {
        if ( !StriCmp(p2, "yes") )
        {
            _o._GameShell->soundFlags |= World::SF_CDSOUND;
            _o._preferences |= World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(true);
        }
        else
        {
            _o._GameShell->soundFlags &= ~World::SF_CDSOUND;
            _o._preferences &= ~World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(false);
        }
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool LevelStatusParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_levelstatus") )
        return false;

    _levelId = parser.stoi(opt);
    return true;
}

int LevelStatusParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( _setFlag )
        _o._GameShell->savedDataFlags |= World::SDF_SCORE;

    if ( !StriCmp(p1, "status") )
    {
        if ( _o._globalMapRegions.MapRegions[_levelId].Status != TMapRegionInfo::STATUS_NONE )
            _o._globalMapRegions.MapRegions[_levelId].Status = parser.stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool BuddyParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt) 
{ 
    if (!StriCmp(word, "begin_buddy"))
    {
        _o._levelInfo.Buddies.emplace_back();
        return true;
    }
    return false;
}

int BuddyParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "commandid") )
    {
        _o._levelInfo.Buddies.back().CommandID = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "type") )
    {
        _o._levelInfo.Buddies.back().Type = parser.stoi(p2);
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _o._levelInfo.Buddies.back().Energy = parser.stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

void ShellParser::ParseStatus(ScriptParser::Parser &parser, TMFWinStatus *status, const std::string &p2)
{
    Stok stok(p2, " _");
    std::string val;

    if ( stok.GetNext(&val) )
        status->Valid = parser.stoi(val) != 0;

    if ( stok.GetNext(&val) )
        status->IsOpen = parser.stoi(val) != 0;

    if ( stok.GetNext(&val) )
        status->Rect.x = parser.stoi(val);

    if ( stok.GetNext(&val) )
        status->Rect.y = parser.stoi(val);

    if ( stok.GetNext(&val) )
        status->Rect.w = parser.stoi(val);

    if ( stok.GetNext(&val) )
        status->Rect.h = parser.stoi(val);

    for (auto &x : status->Data)
    {
        if ( !stok.GetNext(&val) )
            break;

        x = parser.stoi(val);
    }
}


int ShellParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _o._shellConfIsParsed = true;
        return ScriptParser::RESULT_SCOPE_END;
    }

    _o._GameShell->savedDataFlags |= World::SDF_SHELL;

    if ( !StriCmp(p1, "LANGUAGE") )
    {
        std::string * deflt = NULL;
        std::string * slct = NULL;

        for(std::string &s : _o._GameShell->lang_dlls)
        {
            if ( !StriCmp(s, p2) )
                slct = &s;
            if ( !StriCmp(s, "language") )
                deflt = &s;
        }

        if ( slct )
            _o._GameShell->default_lang_dll = slct;
        else
            _o._GameShell->default_lang_dll = deflt;

        _o._GameShell->prev_lang = _o._GameShell->default_lang_dll;

        if ( !_o.ReloadLanguage() )
        {
            ypa_log_out("Unable to set new language\n");
        }
    }
    else if ( !StriCmp(p1, "SOUND") || !StriCmp(p1, "VIDEO") ||
              !StriCmp(p1, "INPUT") || !StriCmp(p1, "DISK") ||
              !StriCmp(p1, "LOCALE") || !StriCmp(p1, "NET") ||
              !StriCmp(p1, "FINDER") || !StriCmp(p1, "LOG") ||
              !StriCmp(p1, "ENERGY") || !StriCmp(p1, "MESSAGE") ||
              !StriCmp(p1, "MAP") )
    {

    }
    else if ( !StriCmp(p1, "robo_map_status") )
    {
        ParseStatus(parser, &_o._roboMapStatus, p2);
    }
    else if ( !StriCmp(p1, "robo_finder_status") )
    {
        ParseStatus(parser, &_o._roboFinderStatus, p2);
    }
    else if ( !StriCmp(p1, "vhcl_map_status") )
    {
        ParseStatus(parser, &_o._vhclMapStatus, p2);
    }
    else if ( !StriCmp(p1, "vhcl_finder_status") )
    {
        ParseStatus(parser, &_o._vhclFinderStatus, p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




}
}
