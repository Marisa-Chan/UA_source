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
    if ( !_o.GameShell->user_name.empty() )
        return false;

    std::string buf = fmt::sprintf("save:%s/%s", _o.GameShell->user_name, filename);
    FSMgr::FileHandle *signFile = uaOpenFile(buf, "r");

    if ( !signFile )
        return false;

    bool res = signFile->ReadLine(&_o.GameShell->callSIGN);

    delete signFile;
    return res;
}


bool UserParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (StriCmp(word, "new_user"))
        return false;

    if (!_o.GameShell->remoteMode)
    {
        if ( !ReadUserNameFile("callsign.def") )
            _o.GameShell->callSIGN =  get_lang_string(_o.string_pointers_p2, 366, "UNNAMED");
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
        _o.maxroboenergy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "maxreloadconst") )
    {
        _o.maxreloadconst = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "numbuddies") )
    {
    }
    else if ( !StriCmp(p1, "beamenergy") )
    {
        _o.beamenergy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "playerstatus") )
    {
        Stok stok(p2, "_ \t");
        std::string val;
        if ( stok.GetNext(&val) )
        {
            int plid = std::stol(val, NULL, 0);
            if ( stok.GetNext(&val) )
            {
                _o.playerstatus[plid].destroyed = std::stol(val, NULL, 0);
                if ( stok.GetNext(&val) )
                {
                    _o.playerstatus[plid].destroyedByUser = std::stol(val, NULL, 0);
                    if ( stok.GetNext(&val) )
                    {
                        _o.playerstatus[plid].elapsedTime = std::stol(val, NULL, 0);
                        if ( stok.GetNext(&val) )
                        {
                            _o.playerstatus[plid].sectorsTaked = std::stol(val, NULL, 0);
                            if ( stok.GetNext(&val) )
                            {
                                _o.playerstatus[plid].score = std::stol(val, NULL, 0);
                                if ( stok.GetNext(&val) )
                                {
                                    _o.playerstatus[plid].power = std::stol(val, NULL, 0);
                                    if ( stok.GetNext(&val) )
                                    {
                                        _o.playerstatus[plid].upgrades = std::stol(val, NULL, 0);
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
            _o.field_2d90->jodiefoster[0] = std::stol(val, NULL, 0);
            if ( stok.GetNext(&val) )
            {
                _o.field_2d90->jodiefoster[1] = std::stol(val, NULL, 0);
                if ( stok.GetNext(&val) )
                {
                    _o.field_2d90->jodiefoster[2] = std::stol(val, NULL, 0);
                    if ( stok.GetNext(&val) )
                    {
                        _o.field_2d90->jodiefoster[3] = std::stol(val, NULL, 0);
                        if ( stok.GetNext(&val) )
                        {
                            _o.field_2d90->jodiefoster[4] = std::stol(val, NULL, 0);
                            if ( stok.GetNext(&val) )
                            {
                                _o.field_2d90->jodiefoster[5] = std::stol(val, NULL, 0);
                                if ( stok.GetNext(&val) )
                                {
                                    _o.field_2d90->jodiefoster[6] = std::stol(val, NULL, 0);
                                    if ( stok.GetNext(&val) )
                                        _o.field_2d90->jodiefoster[7] = std::stol(val, NULL, 0);
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
        for( auto &k: _o.GameShell->keyConfig )
            k.KeyCode = 0;
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

    _o.GameShell->_saveDataFlags |= World::SDF_INPUT;

    if ( !StriCmp(p1, "qualmode") )
    {
    }
    else if ( !StriCmp(p1, "joystick") )
    {
        if ( StrGetBool(p2) )
        {
             _o.GameShell->inp_joystick = true;
             _o.field_73CE &= ~World::PREF_JOYDISABLE;
        }
        else
        {
            _o.GameShell->inp_joystick = false;
            _o.field_73CE |= World::PREF_JOYDISABLE;
        }
    }
    else if ( !StriCmp(p1, "altjoystick") )
    {
        if ( StrGetBool(p2) )
        {
            _o.GameShell->inp_altjoystick = true;
            _o.field_73CE |= World::PREF_ALTJOYSTICK;
        }
        else
        {
            _o.GameShell->inp_altjoystick = false;
            _o.field_73CE &= ~World::PREF_ALTJOYSTICK;
        }
    }
    else if ( !StriCmp(p1, "forcefeedback") )
    {
        if ( StrGetBool(p2) )
            _o.field_73CE &= ~World::PREF_FFDISABLE;
        else
            _o.field_73CE |= World::PREF_FFDISABLE;
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
            int cfgIdex = std::stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            if ( !INPe.getPInput()->input_func64(Input::ITYPE_SLIDER, cfgIdex, buf) )
            {
                ypa_log_out("WARNING: cannot set slider %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_BAD_DATA;
            }


            int gsIndex = UserData::KeyIndexFromConfig(World::KEYC_TYPE_SLIDER, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in slider-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
            _o.GameShell->keyConfig[ gsIndex ].inp_type = World::KEYC_TYPE_SLIDER;
            _o.GameShell->keyConfig[ gsIndex ].keyID = cfgIdex;

            Stok stok(buf, " :\t\n");
            std::string tmp;
            if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
            {
                _o.GameShell->keyConfig[ gsIndex ].slider_neg = Input::GetKeyIdByName(tmp);

                if ( _o.GameShell->keyConfig[ gsIndex ].slider_neg == -1 )
                {
                    ypa_log_out("Unknown keyword for slider %s\n", tmp.c_str());
                    return ScriptParser::RESULT_BAD_DATA;
                }

                if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
                {
                    _o.GameShell->keyConfig[ gsIndex ].KeyCode = Input::GetKeyIdByName(tmp);

                    if ( _o.GameShell->keyConfig[ gsIndex ].KeyCode == -1 )
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

            int cfgIdex = std::stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            if ( !INPe.getPInput()->input_func64(Input::ITYPE_BUTTON, cfgIdex, buf) )
            {
                ypa_log_out("WARNING: cannot set button %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_BAD_DATA;
            }

            int gsIndex = UserData::KeyIndexFromConfig(World::KEYC_TYPE_BUTTON, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in button-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }
            _o.GameShell->keyConfig[ gsIndex ].inp_type = World::KEYC_TYPE_BUTTON;
            _o.GameShell->keyConfig[ gsIndex ].keyID = cfgIdex;

            Stok stok(buf, " :\t\n");
            std::string tmp;
            if ( stok.GetNext(&tmp) && stok.GetNext(&tmp) ) // skip drivername before ':'
            {
                _o.GameShell->keyConfig[ gsIndex ].KeyCode = Input::GetKeyIdByName(tmp);

                if ( _o.GameShell->keyConfig[ gsIndex ].KeyCode == -1 )
                {
                    ypa_log_out("Unknown keyword for button %s\n", tmp);
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

            int cfgIdex = std::stoi( Stok::Fast(p1.substr(13), "] \t=\n") );

            winp_68arg zz;
            zz.keyname = buf;
            zz.id = cfgIdex;

            if ( !INPe.getPInput()->keyb_setHotkey(&zz) )
            {
                ypa_log_out("WARNING: cannot set hotkey %d with %s\n", cfgIdex, buf.c_str());
                return ScriptParser::RESULT_BAD_DATA;
            }

            int gsIndex = UserData::KeyIndexFromConfig(World::KEYC_TYPE_HOTKEY, cfgIdex);
            if ( gsIndex == -1 )
            {
                ypa_log_out("Unknown number in hotkey-declaration (%d)\n", cfgIdex);
                return ScriptParser::RESULT_BAD_DATA;
            }

            _o.GameShell->keyConfig[ gsIndex ].inp_type = World::KEYC_TYPE_HOTKEY;
            _o.GameShell->keyConfig[ gsIndex ].keyID = cfgIdex;

            std::string tmp = Stok::Fast(buf, " :\t\n");
            if ( !tmp.empty() )
            {
                _o.GameShell->keyConfig[ gsIndex ].KeyCode = Input::GetKeyIdByName(tmp);
                if ( _o.GameShell->keyConfig[ gsIndex ].KeyCode == -1 )
                {
                    ypa_log_out("Unknown keyword for hotkey: %s\n", tmp.c_str());
                    return ScriptParser::RESULT_BAD_DATA;
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


vhclSndFX *VhclProtoParser::GetSndFxByName(const std::string &sndname)
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

bool FxParser::ParseExtSampleDef(vhclSndFX *sndfx, const std::string &p2)
{
    Stok stok(p2, "_");
    std::string pp1, pp2, pp3, pp4, pp5, pname;

    if ( !stok.GetNext(&pp1) || !stok.GetNext(&pp2) || !stok.GetNext(&pp3) || !stok.GetNext(&pp4) || !stok.GetNext(&pp5) || !stok.GetNext(&pname) )
        return false;

    if ( sndfx->extS.cnt < (int)sndfx->extS.sndExts.size() )
    {
        int curid = sndfx->extS.cnt;

        sndfx->extS.cnt++;

        sndExt *sndEx = &sndfx->extS.sndExts[ curid ];
        sndEx->sample = NULL;
        sndEx->loop = std::stol(pp1, NULL, 0);
        sndEx->vol = std::stol(pp2, NULL, 0);
        sndEx->smplRate = std::stol(pp3, NULL, 0);
        sndEx->offset = std::stol(pp4, NULL, 0);
        sndEx->smplCnt = std::stol(pp5, NULL, 0);
        sndfx->extSampleNames[curid] = pname;
    }
    return true;
}

int FxParser::ParseSndFX(const std::string &p1, const std::string &p2)
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

    vhclSndFX *sndfx = GetSndFxByName(val);
    if (!sndfx)
        return ScriptParser::RESULT_UNKNOWN;

    stok.GetNext(&val);

    switch (sndTP)
    {
        case 0:
        {
            if ( !StriCmp(val, "sample") )
                sndfx->sample_name = p2;
            else if ( !StriCmp(val, "volume") )
                sndfx->volume = std::stol(p2, NULL, 0);
            else if ( !StriCmp(val, "pitch") )
                sndfx->pitch = std::stol(p2, NULL, 0);
            else if ( !StriCmp(val, "ext") )
            {
                if ( !ParseExtSampleDef(sndfx, p2) )
                    return ScriptParser::RESULT_BAD_DATA;
            }
            else
                return ScriptParser::RESULT_UNKNOWN;
        }
        break;

        case 1:
        {
            if ( !StriCmp(val, "slot") )
                sndfx->sndPrm.slot = std::stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mag0") )
                sndfx->sndPrm.mag0 = std::stof(p2, 0);
            else if ( !StriCmp(val, "mag1") )
                sndfx->sndPrm.mag1 = std::stof(p2, 0);
            else if ( !StriCmp(val, "time") )
                sndfx->sndPrm.time = std::stol(p2, NULL, 0);
            else
                return ScriptParser::RESULT_UNKNOWN;
        }
        break;

        case 2:
        {
            if ( !StriCmp(val, "slot") )
                sndfx->sndPrm_shk.slot = std::stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mag0") )
                sndfx->sndPrm_shk.mag0 = std::stof(p2, 0);
            else if ( !StriCmp(val, "mag1") )
                sndfx->sndPrm_shk.mag1 = std::stof(p2, 0);
            else if ( !StriCmp(val, "time") )
                sndfx->sndPrm_shk.time = std::stol(p2, NULL, 0);
            else if ( !StriCmp(val, "mute") )
                sndfx->sndPrm_shk.mute = std::stof(p2, 0);
            else if ( !StriCmp(val, "x") )
                sndfx->sndPrm_shk.pos.x = std::stof(p2, 0);
            else if ( !StriCmp(val, "y") )
                sndfx->sndPrm_shk.pos.y = std::stof(p2, 0);
            else if ( !StriCmp(val, "z") )
                sndfx->sndPrm_shk.pos.z = std::stof(p2, 0);
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
    roboProto &robo = _o.RoboProtos[_vhclID & 0xF];

    if ( !StriCmp(p1, "end") )
    {
        if ( _vhcl->model_id == BACT_TYPES_ROBO )
        {
            _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_PROTO, &robo);
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

            robo.clear();
            robo.matrix = mat3x3::Ident();
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

            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, 3);
        }
        else if ( !StriCmp(p2, "glider") )
        {
            _vhcl->model_id = BACT_TYPES_FLYER;
            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, 2);
        }
        else if ( !StriCmp(p2, "zeppelin") )
        {
            _vhcl->model_id = BACT_TYPES_FLYER;
            _vhcl->initParams.Add(NC_STACK_ypaflyer::FLY_ATT_TYPE, 0);
        }
        else
        {
            return ScriptParser::RESULT_BAD_DATA;
        }
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _vhcl->disable_enable_bitmask |= 1 << std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _vhcl->disable_enable_bitmask &= ~(1 << std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _vhcl->name = p2;
        std::replace(_vhcl->name.begin(), _vhcl->name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _vhcl->energy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shield") )
    {
        _vhcl->shield = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mass") )
    {
        _vhcl->mass = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "force") )
    {
        _vhcl->force = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "maxrot") )
    {
        _vhcl->maxrot = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "airconst") )
    {
        _vhcl->airconst = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "height") )
    {
        _vhcl->height = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _vhcl->radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "overeof") )
    {
        _vhcl->overeof = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_radius") )
    {
        _vhcl->vwr_radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_overeof") )
    {
        _vhcl->vwr_overeof = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "adist_sector") )
    {
        _vhcl->adist_sector = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "adist_bact") )
    {
        _vhcl->adist_bact = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "sdist_sector") )
    {
        _vhcl->sdist_sector = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "sdist_bact") )
    {
        _vhcl->sdist_bact = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radar") )
    {
        _vhcl->radar = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy") )
    {
        _vhcl->energy += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shield") )
    {
        _vhcl->shield += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_radar") )
    {
        _vhcl->radar += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_normal") )
    {
        _vhcl->vp_normal = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_fire") )
    {
        _vhcl->vp_fire = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_megadeth") )
    {
        _vhcl->vp_megadeth = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_wait") )
    {
        _vhcl->vp_wait = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_dead") )
    {
        _vhcl->vp_dead = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_genesis") )
    {
        _vhcl->vp_genesis = std::stol(p2, NULL, 0);
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
            destFX &dfx = _vhcl->dest_fx[ _vhcl->destFxCount ];
            dfx.type_flag = 0;

            if ( !StriCmp(fx_type, "death") )
                dfx.type_flag |= 1;
            else if ( !StriCmp(fx_type, "megadeth") )
                dfx.type_flag |= 2;
            else if ( !StriCmp(fx_type, "create") )
                dfx.type_flag |= 4;
            else if ( !StriCmp(fx_type, "beam") )
                dfx.type_flag |= 8;
            else
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = std::stol(pp1, NULL, 0);
            dfx.pos.x = std::stof(pp2, 0);
            dfx.pos.y = std::stof(pp3, 0);
            dfx.pos.z = std::stof(pp4, 0);

            _vhcl->destFxCount++;
            if ( _vhcl->destFxCount >= _vhcl->dest_fx.size() )
                _vhcl->destFxCount = _vhcl->dest_fx.size() - 1;
        }
        else
        {
            return ScriptParser::RESULT_BAD_DATA;
        }
    }
    else if ( !StriCmp(p1, "weapon") )
    {
        _vhcl->weapon = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mgun") )
    {
        _vhcl->mgun = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "fire_x") )
    {
        _vhcl->fire_x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "fire_y") )
    {
        _vhcl->fire_y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "fire_z") )
    {
        _vhcl->fire_z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_radius") )
    {
        _vhcl->gun_radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_power") )
    {
        _vhcl->gun_power = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "gun_angle") )
    {
        _vhcl->gun_angle = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "num_weapons") )
    {
        _vhcl->num_weapons = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "kill_after_shot") )
    {
        _vhcl->kill_after_shot = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "job_fighthelicopter") )
    {
        _vhcl->job_fighthelicopter = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fightflyer") )
    {
        _vhcl->job_fightflyer = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fighttank") )
    {
        _vhcl->job_fighttank = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "job_fightrobo") )
    {
        _vhcl->job_fightrobo = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "job_reconnoitre") )
    {
        _vhcl->job_reconnoitre = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "job_conquer") )
    {
        _vhcl->job_conquer = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "gun_side_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_SIDEANGLE, (int)std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "gun_up_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_UPANGLE, (int)std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "gun_down_angle") )
    {
        _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_DOWNANGLE, (int)std::stol(p2, NULL, 0));
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
            _vhcl->initParams.Add(NC_STACK_ypagun::GUN_ATT_FIRETYPE, gun_type);
    }
    else if ( !StriCmp(p1, "kamikaze") )
    {
        _vhcl->initParams.Add(NC_STACK_ypacar::CAR_ATT_KAMIKAZE, 1);

        _vhcl->initParams.Add(NC_STACK_ypacar::CAR_ATT_BLAST, (int)std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "wireframe") )
    {
        if ( _vhcl->wireframe )
            Nucleus::Delete( _vhcl->wireframe );

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str());

        _vhcl->wireframe = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "hud_wireframe") )
    {
        if ( _vhcl->hud_wireframe )
            Nucleus::Delete(_vhcl->hud_wireframe);

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str());

        _vhcl->hud_wireframe = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "mg_wireframe") )
    {
        if ( _vhcl->mg_wireframe )
            Nucleus::Delete(_vhcl->mg_wireframe);

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str());

        _vhcl->mg_wireframe = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "wpn_wireframe_1") )
    {
        if ( _vhcl->wpn_wireframe_1 )
            Nucleus::Delete(_vhcl->wpn_wireframe_1);

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str());

        _vhcl->wpn_wireframe_1 = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "wpn_wireframe_2") )
    {
        if ( _vhcl->wpn_wireframe_2 )
            Nucleus::Delete(_vhcl->wpn_wireframe_2);

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str());

        _vhcl->wpn_wireframe_2 = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "vo_type") )
    {
        _vhcl->vo_type = std::stol(p2, NULL, 16);
    }
    else if ( !StriCmp(p1, "max_pitch") )
    {
        _vhcl->max_pitch = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "scale_fx") )
    {
        Stok stok(p2, "_");
        std::string pp0, pp1, pp2, pp3;

        if ( stok.GetNext(&pp0) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) )
        {
            _vhcl->scale_fx_p0 = std::stof(pp0, 0);
            _vhcl->scale_fx_p1 = std::stof(pp1, 0);
            _vhcl->scale_fx_p2 = std::stof(pp2, 0);
            _vhcl->scale_fx_p3 = std::stol(pp3, NULL, 0);

            int tmp = 0;
            while ( stok.GetNext(&pp0) )
            {
                _vhcl->scale_fx_pXX[tmp] = std::stol(pp0, NULL, 0);
                tmp++;
            }
        }
    }
    else if ( !StriCmp(p1, "robo_data_slot") )
    {
    }
    else if ( !StriCmp(p1, "robo_num_guns") )
    {
        robo.robo_num_guns = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_act_gun") )
    {
        _gunID = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_x") )
    {
        robo.guns[ _gunID ].pos.x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_y") )
    {
        robo.guns[ _gunID ].pos.y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_pos_z") )
    {
        robo.guns[ _gunID ].pos.z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_x") )
    {
        robo.guns[ _gunID ].dir.x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_y") )
    {
        robo.guns[ _gunID ].dir.y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_dir_z") )
    {
        robo.guns[ _gunID ].dir.z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_type") )
    {
        robo.guns[ _gunID ].robo_gun_type = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_gun_name") )
    {
        robo.guns[ _gunID ].robo_gun_name = p2;
    }
    else if ( !StriCmp(p1, "robo_dock_x") )
    {
        robo.dock.x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_dock_y") )
    {
        robo.dock.y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_dock_z") )
    {
        robo.dock.z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_num") )
    {
        robo.coll.robo_coll_num = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_act") )
    {
        _collID = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_radius") )
    {
        robo.coll.roboColls[ _collID ].robo_coll_radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_x") )
    {
        robo.coll.roboColls[ _collID ].coll_pos.x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_y") )
    {
        robo.coll.roboColls[ _collID ].coll_pos.y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_coll_z") )
    {
        robo.coll.roboColls[ _collID ].coll_pos.z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_x") )
    {
        robo.viewer.x = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_y") )
    {
        robo.viewer.y = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_z") )
    {
        robo.viewer.z = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_up") )
    {
        robo.robo_viewer_max_up = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_down") )
    {
        robo.robo_viewer_max_down = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_viewer_max_side") )
    {
        robo.robo_viewer_max_side = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "robo_does_twist") )
    {
        _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_WAIT_ROTATE, 1);
    }
    else if ( !StriCmp(p1, "robo_does_flux") )
    {
        _vhcl->initParams.Add(NC_STACK_yparobo::ROBO_ATT_WAIT_SWAY, 1);
    }
    else
        return ParseSndFX(p1, p2);

    return ScriptParser::RESULT_OK;
}


bool VhclProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( !StriCmp(word, "new_vehicle") )
    {
        _vhclID = std::stol(opt, NULL, 0);
        _vhcl = &_o.VhclProtos[_vhclID];

        if ( _vhcl->wireframe )
        {
            Nucleus::Delete(_vhcl->wireframe);
            _vhcl->wireframe = NULL;
        }

        if ( _vhcl->hud_wireframe )
        {
            Nucleus::Delete(_vhcl->hud_wireframe);
            _vhcl->hud_wireframe = NULL;
        }

        if ( _vhcl->mg_wireframe )
        {
            Nucleus::Delete(_vhcl->mg_wireframe);
            _vhcl->mg_wireframe = NULL;
        }

        if ( _vhcl->wpn_wireframe_1 )
        {
            Nucleus::Delete(_vhcl->wpn_wireframe_1);
            _vhcl->wpn_wireframe_1 = NULL;
        }

        if ( _vhcl->wpn_wireframe_2 )
        {
            Nucleus::Delete(_vhcl->wpn_wireframe_2);
            _vhcl->wpn_wireframe_2 = NULL;
        }

        _vhcl->clear();

        _vhcl->model_id = 2;
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
        _vhclID = std::stol(opt, NULL, 0);
        _vhcl = &_o.VhclProtos[_vhclID];

        _o.last_modify_vhcl = _vhclID;
        return true;
    }

    return false;
}

vhclSndFX *WeaponProtoParser::GetSndFxByName(const std::string &sndname)
{
    if ( !StriCmp(sndname, "normal") )
        return &_wpn->sndFXes[0];
    else if ( !StriCmp(sndname, "launch") )
        return &_wpn->sndFXes[1];
    else if ( !StriCmp(sndname, "hit") )
        return &_wpn->sndFXes[2];

    return NULL;
}

bool WeaponProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (!StriCmp(word, "new_weapon"))
    {
        int wpnId = std::stol(opt, NULL, 0);
        _wpn = &_o.WeaponProtos[wpnId];

        if ( _wpn->wireframe )
        {
            Nucleus::Delete(_wpn->wireframe);
            _wpn->wireframe = NULL;
        }

        _wpn->clear();
        _wpn->field_0 = 4;
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

        for (int i = 0; i < 3; i++)
        {
            vhclSndFX &fx = _wpn->sndFXes[i];
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
        int wpnId = std::stol(opt, NULL, 0);
        _wpn = &_o.WeaponProtos[wpnId];

        _o.last_modify_weapon = wpnId;
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
            _wpn->model_id = 17;
        else if ( !StriCmp(p2, "rocket") )
            _wpn->model_id = 3;
        else if ( !StriCmp(p2, "missile") )
            _wpn->model_id = 7;
        else if ( !StriCmp(p2, "bomb") || !StriCmp(p2, "special") )
            _wpn->model_id = 1;
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _wpn->enable_mask |= 1 << std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _wpn->enable_mask &= ~(1 << std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _wpn->name = p2;
        std::replace(_wpn->name.begin(), _wpn->name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _wpn->energy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "energy_heli") )
    {
        _wpn->energy_heli = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_tank") )
    {
        _wpn->energy_tank = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_flyer") )
    {
        _wpn->energy_flyer = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "energy_robo") )
    {
        _wpn->energy_robo = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "mass") )
    {
        _wpn->mass = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "force") )
    {
        _wpn->force = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "maxrot") )
    {
        _wpn->maxrot = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "airconst") )
    {
        _wpn->airconst = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius") )
    {
        _wpn->radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_heli") )
    {
        _wpn->radius_heli = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_tank") )
    {
        _wpn->radius_tank = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_flyer") )
    {
        _wpn->radius_flyer = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "radius_robo") )
    {
        _wpn->radius_robo = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "overeof") )
    {
        _wpn->overeof = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_radius") )
    {
        _wpn->vwr_radius = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "vwr_overeof") )
    {
        _wpn->vwr_overeof = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "start_speed") )
    {
        _wpn->start_speed = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "life_time") )
    {
        _wpn->life_time = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "life_time_nt") )
    {
        _wpn->life_time_nt = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "drive_time") )
    {
        _wpn->drive_time = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "delay_time") )
    {
        _wpn->delay_time = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shot_time") )
    {
        _wpn->shot_time = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "shot_time_user") )
    {
        _wpn->shot_time_user = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "salve_shots") )
    {
        _wpn->salve_shots = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "salve_delay") )
    {
        _wpn->salve_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy") )
    {
        _wpn->energy += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_heli") )
    {
        _wpn->energy_heli += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_tank") )
    {
        _wpn->energy_tank += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_flyer") )
    {
        _wpn->energy_flyer += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_energy_Robo") )
    {
        _wpn->energy_robo += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shot_time") )
    {
        _wpn->shot_time += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "add_shot_time_user") )
    {
        _wpn->shot_time_user += std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_normal") )
    {
        _wpn->vp_normal = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_fire") )
    {
        _wpn->vp_fire = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_megadeth") )
    {
        _wpn->vp_megadeth = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_wait") )
    {
        _wpn->vp_wait = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_dead") )
    {
        _wpn->vp_dead = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vp_genesis") )
    {
        _wpn->vp_genesis = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type_icon") )
    {
        _wpn->type_icon = p2[0];
    }
    else if ( !StriCmp(p1, "wireframe") )
    {
        if ( _wpn->wireframe )
            Nucleus::Delete(_wpn->wireframe);

        IDVList init_vals;
        init_vals.Add( NC_STACK_rsrc::RSRC_ATT_NAME, p2.c_str() );

        _wpn->wireframe = Nucleus::CInit<NC_STACK_sklt>(init_vals);
    }
    else if ( !StriCmp(p1, "dest_fx") )
    {
        Stok stok(p2, " _");
        std::string fx_type, pp1, pp2, pp3, pp4;

        if ( stok.GetNext(&fx_type) && stok.GetNext(&pp1) && stok.GetNext(&pp2) && stok.GetNext(&pp3) && stok.GetNext(&pp4) )
        {
            destFX &dfx = _wpn->dfx[ _wpn->destFxCount ];
            dfx.type_flag = 0;

            if ( !StriCmp(fx_type, "death") )
                dfx.type_flag |= 1;
            else if ( !StriCmp(fx_type, "megadeth") )
                dfx.type_flag |= 2;
            else if ( !StriCmp(fx_type, "create") )
                dfx.type_flag |= 4;
            else if ( !StriCmp(fx_type, "beam") )
                dfx.type_flag |= 8;
            else
                return ScriptParser::RESULT_BAD_DATA;

            dfx.ModelID = std::stol(pp1, NULL, 0);
            dfx.pos.x = std::stof(pp2, 0);
            dfx.pos.y = std::stof(pp3, 0);
            dfx.pos.z = std::stof(pp4, 0);

            _wpn->destFxCount++;
            if ( _wpn->destFxCount >= (int)_wpn->dfx.size() )
                _wpn->destFxCount = _wpn->dfx.size() - 1;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else
        return ParseSndFX(p1, p2);

    return ScriptParser::RESULT_OK;
}

bool BuildProtoParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (!StriCmp(word, "new_building"))
    {
        int bldId = std::stol(opt, NULL, 0);

        _bld = &_o.BuildProtos[bldId];
        _bld->clear();
        _bld->energy = 50000;
        _bld->type_icon = 65;
        _bld->sndfx.volume = 120;
        return true;
    }
    else if (!StriCmp(word, "modify_building"))
    {
        int bldId = std::stol(opt, NULL, 0);

        _bld = &_o.BuildProtos[bldId];
        _o.last_modify_build = bldId;
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
            _bld->model_id = 0;
        }
        else if ( !StriCmp(p2, "kraftwerk") )
        {
            _bld->model_id = 1;
        }
        else if ( !StriCmp(p2, "radar") )
        {
            _bld->model_id = 2;
        }
        else if ( !StriCmp(p2, "defcenter") )
        {
            _bld->model_id = 3;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "enable") )
    {
        _bld->enable_mask |= 1 << std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "disable") )
    {
        _bld->enable_mask &= ~(1 << std::stol(p2, NULL, 0));
    }
    else if ( !StriCmp(p1, "name") )
    {
        _bld->name = p2;
        std::replace(_bld->name.begin(), _bld->name.end(), '_', ' ');
    }
    else if ( !StriCmp(p1, "power") )
    {
        _bld->power = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _bld->energy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_type") )
    {
        _bld->sec_type = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type_icon") )
    {
        _bld->type_icon = p2[0];
    }
    else if ( !StriCmp(p1, "snd_normal_sample") )
    {
        _bld->sndfx.sample_name = p2;
    }
    else if ( !StriCmp(p1, "snd_normal_volume") )
    {
        _bld->sndfx.volume = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "snd_normal_pitch") )
    {
        _bld->sndfx.pitch = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sbact_act") )
    {
        _sbact = std::stol(p2, NULL, 0);
    }
    else
    {
        buildSbact *v19 = &_bld->sbacts[_sbact];
        if ( !StriCmp(p1, "sbact_vehicle") )
        {
            v19->sbact_vehicle = std::stol(p2, NULL, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_x") )
        {
            v19->sbact_pos_x = std::stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_y") )
        {
            v19->sbact_pos_y = std::stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_pos_z") )
        {
            v19->sbact_pos_z = std::stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_x") )
        {
            v19->sbact_dir_x = std::stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_y") )
        {
            v19->sbact_dir_y = std::stof(p2, 0);
        }
        else if ( !StriCmp(p1, "sbact_dir_z") )
        {
            v19->sbact_dir_z = std::stof(p2, 0);
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

    for (yw_movie &movie : _o.movies)
        movie.clear();
    return true;
}

int MovieParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
	if ( !StriCmp(p1, "end") )
		return ScriptParser::RESULT_SCOPE_END;
	else if ( !StriCmp(p1, "game_intro") )
	{
		_o.movies[World::MOVIE_INTRO] = p2;
		_o.movies[World::MOVIE_INTRO] = true;
	}
	else if ( !StriCmp(p1, "win_extro") )
	{
		_o.movies[World::MOVIE_WIN] = p2;
		_o.movies[World::MOVIE_WIN] = true;
	}
	else if ( !StriCmp(p1, "lose_extro") )
	{
		_o.movies[World::MOVIE_LOSE] = p2;
		_o.movies[World::MOVIE_LOSE] = true;
	}
	else if ( !StriCmp(p1, "user_intro") )
	{
		_o.movies[World::MOVIE_USER] = p2;
		_o.movies[World::MOVIE_USER] = true;
	}
	else if ( !StriCmp(p1, "kyt_intro") )
	{
		_o.movies[World::MOVIE_KYT] = p2;
		_o.movies[World::MOVIE_KYT] = true;
	}
	else if ( !StriCmp(p1, "taer_intro") )
	{
		_o.movies[World::MOVIE_TAER] = p2;
		_o.movies[World::MOVIE_TAER] = true;
	}
	else if ( !StriCmp(p1, "myk_intro") )
	{
		_o.movies[World::MOVIE_MYK] = p2;
		_o.movies[World::MOVIE_MYK] = true;
	}
	else if ( !StriCmp(p1, "sulg_intro") )
	{
		_o.movies[World::MOVIE_SULG] = p2;
		_o.movies[World::MOVIE_SULG] = true;
	}
	else if ( !StriCmp(p1, "black_intro") )
	{
		_o.movies[World::MOVIE_BLACK] = p2;
		_o.movies[World::MOVIE_BLACK] = true;
	}
	else
		return ScriptParser::RESULT_UNKNOWN;
	return ScriptParser::RESULT_OK;
}


BkgParser::BkgParser(NC_STACK_ypaworld *o)
: _o(*(o->LevelNet))
{}

int BkgParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _o.bg_n++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "background_map") )
    {
        _o.background_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "rollover_map") )
    {
        _o.rollover_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "finished_map") )
    {
        _o.finished_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "enabled_map") )
    {
        _o.enabled_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "mask_map") )
    {
        _o.mask_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "tut_background_map") )
    {
        _o.tut_background_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "tut_rollover_map") )
    {
        _o.tut_rollover_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "tut_mask_map") )
    {
        _o.tut_mask_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "menu_map") )
    {
        _o.menu_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "input_map") )
    {
        _o.input_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "settings_map") )
    {
        _o.settings_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "network_map") )
    {
        _o.network_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "locale_map") )
    {
        _o.locale_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "save_map") )
    {
        _o.save_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "about_map") )
    {
        _o.about_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "help_map") )
    {
        _o.help_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "brief_map") )
    {
        _o.brief_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "debrief_map") )
    {
        _o.debrief_map[_o.bg_n].map_name = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _o.background_map[_o.bg_n].size_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _o.background_map[_o.bg_n].size_y = std::stol(p2, NULL, 0);
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

    _o.beam_energy_start = 500;
	_o.beam_energy_add = 100;
	_o.unit_limit = 512;
	_o.unit_limit_type = 0;
	_o.unit_limit_arg = 0;
	_o.easy_cheat_keys = 0;
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
        _o.one_game_res = StrGetBool(p2);
    }
    else if ( !StriCmp(p1, "shell_default_res") )
    {
    	Stok stok(p2, "_ \t");
    	std::string pp1, pp2;
        if ( stok.GetNext(&pp1) && stok.GetNext(&pp2) )
        {
            _o.shell_default_res = std::stol(pp2, NULL, 0) | (std::stol(pp1, NULL, 0) << 12);
        }
    }
    else if ( !StriCmp(p1, "game_default_res") )
    {
    	Stok stok(p2, "_ \t");
        std::string pp1, pp2;
        if ( stok.GetNext(&pp1) && stok.GetNext(&pp2) )
        {
            _o.game_default_res = std::stol(pp2, NULL, 0) | (std::stol(pp1, NULL, 0) << 12);
        }
    }
    else if ( !StriCmp(p1, "max_impulse") )
    {
        _o.max_impulse = std::stof(p2);
    }
    else if ( !StriCmp(p1, "unit_limit") )
    {
        _o.unit_limit = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_type") )
    {
        _o.unit_limit_type = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_arg") )
    {
        _o.unit_limit_arg = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "beam_energy_start") )
    {
        _o.beam_energy_start = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "beam_energy_add") )
    {
        _o.beam_energy_add = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "easy_cheat_keys") )
    {
        _o.easy_cheat_keys = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool SuperItemParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_superitem") )
        return false;

    _o.superbomb_wall_vproto = 0;
    _o.superbomb_center_vproto = 0;
    return true;
};

int SuperItemParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        return ScriptParser::RESULT_SCOPE_END;
    }
	else if ( !StriCmp(p1, "superbomb_center_vproto") )
    {
        _o.superbomb_center_vproto = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "superbomb_wall_vproto") )
    {
        _o.superbomb_wall_vproto = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;
    return ScriptParser::RESULT_OK;
}


void MapSizesParser::ParseSizes(ScriptParser::Parser &parser, int *outx, int *outy)
{
    std::string tmp;
    parser.ReadLine(&tmp);

    Stok stok(tmp, " \r\n");

    std::string sX, sY;
    stok.GetNext(&sX);
    stok.GetNext(&sY);

    int x = std::stol(sX, NULL, 0);
    int y = std::stol(sY, NULL, 0);

    for(int i = 0; i < y; i++)
        parser.ReadLine(&tmp);

    if (outx)
        (*outx) = x;
    if (outy)
        (*outy) = y;
}


int MapSizesParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "typ_map") )
    {
        int y_size;
        int x_size;

        ParseSizes(parser, &x_size, &y_size);
        _m.secXsize = x_size;
        _m.secYsize = y_size;
    }
    else if ( !StriCmp(p1, "own_map") || !StriCmp(p1, "hgt_map") || !StriCmp(p1, "blg_map") )
    {
        ParseSizes(parser, NULL, NULL);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool LevelDataParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_level") )
        return false;

    _o.field_2d90->map_name = "<NO NAME>";
    _o.field_2d90->movie.clear();
    _o.field_2d90->win_movie.clear();
    _o.field_2d90->lose_movie.clear();
    _o.vehicle_sector_ratio_1 = 0;
    _o.unit_limit_1 = _o.unit_limit;
    _o.unit_limit_type_1 = _o.unit_limit_type;
    _o.unit_limit_arg_1 = _o.unit_limit_arg;
    return true;
}

int LevelDataParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( p1.find("title_") != std::string::npos )
    {
        std::string title_lang = "title_";
        title_lang += _o.lang_name;

        if ( !StriCmp(p1, "title_default") || !StriCmp(p1, title_lang) )
                _o.field_2d90->map_name = p2;
    }
    else if ( !StriCmp(p1, "set") )
    {
        _m.setNumber = std::stol(p2, NULL, 0);
        _m.flags |= 1;
    }
    else if ( !StriCmp(p1, "sky") )
    {
        _m.sky = p2;
        _m.flags |= 2;
    }
    else if ( !StriCmp(p1, "typ") )
    {
        _m.typ = p2;
        _m.flags |= 4;
    }
    else if ( !StriCmp(p1, "own") )
    {
        _m.own = p2;
        _m.flags |= 8;
    }
    else if ( !StriCmp(p1, "hgt") )
    {
        _m.hgt = p2;
        _m.flags |= 0x10;
    }
    else if ( !StriCmp(p1, "blg") )
    {
        _m.blg = p2;
        _m.flags |= 0x20;
    }
    else if ( !StriCmp(p1, "palette") )
    {
        _m.palettes[0] = p2;
    }
    else if ( !StriCmp(p1, "slot0") )
    {
        _m.palettes[0] = p2;
    }
    else if ( !StriCmp(p1, "slot1") )
    {
        _m.palettes[1] = p2;
    }
    else if ( !StriCmp(p1, "slot2") )
    {
        _m.palettes[2] = p2;
    }
    else if ( !StriCmp(p1, "slot3") )
    {
        _m.palettes[3] = p2;
    }
    else if ( !StriCmp(p1, "slot4") )
    {
        _m.palettes[4] = p2;
    }
    else if ( !StriCmp(p1, "slot5") )
    {
        _m.palettes[5] = p2;
    }
    else if ( !StriCmp(p1, "slot6") )
    {
        _m.palettes[6] = p2;
    }
    else if ( !StriCmp(p1, "slot7") )
    {
        _m.palettes[7] = p2;
    }
    else if ( !StriCmp(p1, "script") )
    {
        if ( !_o.sub_4DA354(p2) )
            return ScriptParser::RESULT_BAD_DATA;
        return ScriptParser::RESULT_OK;
    }
    else if ( !StriCmp(p1, "ambiencetrack") )
    {
        _o.field_2d90->amb_track_p1 = 0;
        _o.field_2d90->amb_track_p2 = 0;

        Stok stok(p2, " \t_\n");
        std::string tmp;
        stok.GetNext(&tmp);
        _o.field_2d90->amb_track_p0 = std::stol(tmp, NULL, 0);

        if ( stok.GetNext(&tmp) )
        {
            _o.field_2d90->amb_track_p1 = std::stol(tmp, NULL, 0);

            if ( stok.GetNext(&tmp) )
                _o.field_2d90->amb_track_p2 = std::stol(tmp, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "movie") )
    {
        _o.field_2d90->movie = p2;
    }
    else if ( !StriCmp(p1, "win_movie") )
    {
        _o.field_2d90->win_movie = p2;
    }
    else if ( !StriCmp(p1, "lose_movie") )
    {
        _o.field_2d90->lose_movie = p2;
    }
    else if ( !StriCmp(p1, "event_loop") )
    {
        _m.event_loop = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "slow_connection") )
    {
        if ( StrGetBool(p2) )
        {
            _m.slow_connection = 1;
        }
        else
        {
            _m.slow_connection = 0;
        }
    }
    else if ( !StriCmp(p1, "vehicle_sector_ratio") )
    {
        _o.vehicle_sector_ratio_1 = std::stof(p2, 0);
    }
    else if ( !StriCmp(p1, "unit_limit") )
    {
        _o.unit_limit_1 = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_type") )
    {
        _o.unit_limit_type_1 = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "unit_limit_arg") )
    {
        _o.unit_limit_arg_1 = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

bool MapRobosParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if (StriCmp(word, "begin_robo"))
        return false;

    _r = &_m.mapRobos[_m.mapRobos_count];
    _r->mb_status = 0;
    return true;
}


int MapRobosParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if (_m.mapRobos_count == 0) //If it's first host station - save owner for brief
            _m.playerOwner = _r->owner;

        _m.mapRobos_count++;
        _m.flags |= 0x40;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "owner") )
    {
        _r->owner = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "vehicle") )
    {
        _r->vehicle = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_x") )
    {
        int tmp = std::stol(p2, NULL, 0);
        _r->pos_y = -300;
        _r->pos_x = tmp * 1200.0 + 600.0;
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        int tmp = std::stol(p2, NULL, 0);
        _r->pos_y = -300;
        _r->pos_z = -(tmp * 1200.0 + 600.0);
    }
    else if ( !StriCmp(p1, "pos_x") )
    {
        _r->pos_x = std::stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_y") )
    {
        _r->pos_y = std::stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_z") )
    {
        _r->pos_z = std::stof(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _r->energy = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "con_budget") )
    {
        _r->con_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rad_budget") )
    {
        _r->rad_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pow_budget") )
    {
        _r->pow_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "def_budget") )
    {
        _r->def_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "saf_budget") )
    {
        _r->saf_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rec_budget") )
    {
        _r->rec_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "cpl_budget") )
    {
        _r->cpl_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rob_budget") )
    {
        _r->rob_budget = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "viewangle") )
    {
        _r->viewangle = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "saf_delay") )
    {
        _r->saf_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pow_delay") )
    {
        _r->pow_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rad_delay") )
    {
        _r->rad_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "cpl_delay") )
    {
        _r->cpl_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "def_delay") )
    {
        _r->def_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "con_delay") )
    {
        _r->con_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rec_delay") )
    {
        _r->rec_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "rob_delay") )
    {
        _r->rob_delay = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _r->mb_status = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _r->mb_status = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _r->mb_status = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "reload_const") )
    {
        _r->reload_const = std::stol(p2, NULL, 0);
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
        {"timercount", World::SOUND_ID_TIMERCOUNT}
    };

    static const ShellSoundNames block2[] =
    {
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
                        return ( _o.LoadSample(0, t.id, p2) ? ScriptParser::RESULT_OK : ScriptParser::RESULT_BAD_DATA );
                    else if (!StriCmp("volume", tp))
                        _o.samples1_info.samples_data[t.id].volume = std::stoi(p2);
                    else if (!StriCmp("pitch", tp))
                        _o.samples1_info.samples_data[t.id].pitch = std::stoi(p2);
                    else
                        return ScriptParser::RESULT_UNKNOWN;

                    return ScriptParser::RESULT_OK;
                }
            }

            for (auto &t: block2)
            {
                if ( !StriCmp(t.name, sm) )
                {
                    if (!StriCmp("sample", tp))
                        return ( _o.LoadSample(1, t.id, p2) ? ScriptParser::RESULT_OK : ScriptParser::RESULT_BAD_DATA );
                    else if (!StriCmp("volume", tp))
                        _o.samples2_info.samples_data[t.id].volume = std::stoi(p2);
                    else if (!StriCmp("pitch", tp))
                        _o.samples2_info.samples_data[t.id].pitch = std::stoi(p2);
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

        _o.shelltrack = std::stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.shelltrack__adv.min_delay = std::stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.shelltrack__adv.max_delay = std::stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "missiontrack") )
    {
        _o.missiontrack__adv.min_delay = 0;
        _o.missiontrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.missiontrack = std::stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.missiontrack__adv.min_delay = std::stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.missiontrack__adv.max_delay = std::stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "debriefingtrack") )
    {
        _o.debriefingtrack__adv.min_delay = 0;
        _o.debriefingtrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.debriefingtrack = std::stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.debriefingtrack__adv.min_delay = std::stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.debriefingtrack__adv.max_delay = std::stol(val, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "loadingtrack") )
    {
        _o.loadingtrack__adv.min_delay = 0;
        _o.loadingtrack__adv.max_delay = 0;

        std::string val;
        stok.GetNext(&val);

        _o.loadingtrack = std::stol(val, NULL, 0);
        if ( stok.GetNext(&val) )
        {
            _o.loadingtrack__adv.min_delay = std::stol(val, NULL, 0);

            if ( stok.GetNext(&val) )
                _o.loadingtrack__adv.max_delay = std::stol(val, NULL, 0);
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

    _s = &_m.squads[ _m.squad_count ];
    return true;
}

int LevelSquadParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_s->vehicle )
        {
            ypa_log_out("Squad init: squad[%d]аno vehicle defined!\n", _m.squad_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->num )
        {
            ypa_log_out("Squad init: squad[%d] num of vehicles is 0!\n", _m.squad_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _s->pos_x == 0.0 || _s->pos_z == 0.0 )
        {
            ypa_log_out("Squad init: squad[%d] no pos given!\n", _m.squad_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        _s->field_0 = 1;
        _m.squad_count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "owner") )
    {
        _s->owner = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "useable") )
    {
        _s->useable = 1;
    }
    else if ( !StriCmp(p1, "vehicle") )
    {
        _s->vehicle = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "num") )
    {
        _s->num = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "pos_x") )
    {
        _s->pos_x = std::stod(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "pos_z") )
    {
        _s->pos_z = std::stod(p2, 0) + 0.3;
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _s->mb_status = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _s->mb_status = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _s->mb_status = 2;
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

    _g = &_o.field_2d90->gates[ _o.field_2d90->gate_count ];
    _g->mb_status = 0;
    return true;
}

int LevelGatesParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_g->closed_bp )
        {
            ypa_log_out("Gate init: gate[%d] no closed building defined!\n", _o.field_2d90->gate_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_g->opened_bp )
        {
            ypa_log_out("Gate init: gate[%d] no opened building defined!\n", _o.field_2d90->gate_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _g->sec_x == 0 || _g->sec_y == 0)
        {
            ypa_log_out("Gate init: gate[%d] no sector coords!\n", _o.field_2d90->gate_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_g->target_level_count )
        {
            ypa_log_out("Gate init: gate[%d] no target levels defined!\n", _o.field_2d90->gate_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        _o.field_2d90->gate_count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "sec_x") )
    {
        _g->sec_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _g->sec_y = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "closed_bp") )
    {
        _g->closed_bp = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "opened_bp") )
    {
        _g->opened_bp = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "target_level") )
    {
        if ( _g->target_level_count < 8 )
        {
            _g->target_levels[ _g->target_level_count ] = std::stol(p2, NULL, 0);
            _g->target_level_count++;
        }
    }
    else if ( !StriCmp(p1, "keysec_x") )
    {
        if ( _g->keySectors_count < 16 )
        {
            _g->keySectors[_g->keySectors_count].x = std::stol(p2, NULL, 0);
        }
    }
    else if ( !StriCmp(p1, "keysec_y") )
    {
        if ( _g->keySectors_count < 16 )
        {
            _g->keySectors[ _g->keySectors_count ].y = std::stol(p2, NULL, 0);
            _g->keySectors_count++;
        }
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _g->mb_status = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _g->mb_status = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _g->mb_status = 2;
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
    _d = &_m.mbmaps[ _m.mbmaps_count ];
    return true;
}

int LevelMbMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _m.mbmaps_count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "name") )
    {
        _d->name = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _d->size_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _d->size_y = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




bool LevelGemParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_gem") )
        return false;

    _g = &_o.gems[_count];
    _g->mb_status = 0;
    return true;
}

int LevelGemParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( !_g->building )
        {
            ypa_log_out("WStein init: gem[%d] no building defined!\n", _count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _g->sec_x == 0 || _g->sec_y == 0 )
        {
            ypa_log_out("WStein init: gem[%d] sector pos wonky tonk!\n", _count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        _g->field_0 = 1;
        _count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( p1.find("msg_") != std::string::npos )
    {
        std::string tmp = fmt::sprintf("msg_%s", _o.lang_name);

        if ( !StriCmp(p1, "msg_default") || !StriCmp(p1, tmp) )
            _g->msg_default = p2;
    }
    else if ( !StriCmp(p1, "sec_x") )
    {
        _g->sec_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _g->sec_y = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "building") )
    {
        _g->building = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "type") )
    {
        switch ( std::stol(p2, NULL, 0) )
        {
        case 1:
            _g->type = 25;
            break;
        case 2:
            _g->type = 26;
            break;
        case 3:
            _g->type = 27;
            break;
        case 4:
            _g->type = 28;
            break;
        case 5:
            _g->type = 78;
            break;
        case 6:
            _g->type = 79;
            break;
        default:
            _g->type = 80;
            break;
        }
    }
    else if ( !StriCmp(p1, "script") )
    {
        _g->script = p2;

        FSMgr::FileHandle *tmp = uaOpenFile(_g->script, "r");

        if ( !tmp )
            return ScriptParser::RESULT_BAD_DATA;

        delete tmp;
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _g->mb_status = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _g->mb_status = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _g->mb_status = 2;
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
            _g->nw_vproto_num_1 = std::stol(tmp, NULL, 0);
            if ( stok.GetNext(&tmp) )
            {
                _g->nw_vproto_num_2 = std::stol(tmp, NULL, 0);
                if ( stok.GetNext(&tmp) )
                {
                    _g->nw_vproto_num_3 = std::stol(tmp, NULL, 0);
                    if ( stok.GetNext(&tmp) )
                        _g->nw_vproto_num_4 = std::stol(tmp, NULL, 0);
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
            _g->nw_bproto_num_1 = std::stol(tmp, NULL, 0);
            if ( stok.GetNext(&tmp) )
            {
                _g->nw_bproto_num_2 = std::stol(tmp, NULL, 0);
                if ( stok.GetNext(&tmp) )
                {
                    _g->nw_bproto_num_3 = std::stol(tmp, NULL, 0);
                    if ( stok.GetNext(&tmp) )
                        _g->nw_bproto_num_4 = std::stol(tmp, NULL, 0);
                }
            }
        }
    }
    else if ( !StriCmp(p1, "begin_action") )
    {
        std::string tmp;

        while( parser.ReadLine(&tmp) && (tmp.find("end_action") == std::string::npos) )
            _g->actions.push_back(tmp);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




bool LevelEnableParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_enable") )
        return false;

    _fraction = std::stol(opt, NULL, 0);

    for (int i = 0; i < 256; i++)
        _o.VhclProtos[i].disable_enable_bitmask &= ~(1 << _fraction);

    for (int i = 0; i < 128; i++)
        _o.BuildProtos[i].enable_mask &= ~(1 << _fraction);

    return true;
}

int LevelEnableParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "vehicle") )
    {
        int id = std::stol(p2, NULL, 0);
        if ( id >= 0 && id < 256 ) //_o.ypaworld.VhclProtos.size() )
            _o.VhclProtos[id].disable_enable_bitmask |= (1 << _fraction);
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "building") )
    {
        int id = std::stol(p2, NULL, 0);
        if ( id >= 0 && id < 128 )
            _o.BuildProtos[id].enable_mask |= (1 << _fraction);
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

    _d = &_m.dbmaps[ _m.dbmap_count ];
    return true;
}

int LevelDebMapParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _m.mbmaps_count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "name") )
    {
        _d->name = p2;
    }
    else if ( !StriCmp(p1, "size_x") )
    {
        _d->size_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "size_y") )
    {
        _d->size_y = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}


bool LevelSuperItemsParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_item") )
        return false;

    _s = &_o.field_2d90->supetItems[_o.field_2d90->supetItems_count];
    _s->type = 0;
    _s->countdown = 60000; //1hour
    _s->field_4 = 0;
    _s->mb_status = 0;
    return true;
}

int LevelSuperItemsParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        if ( _s->sec_x == 0 || _s->sec_y == 0)
        {
            ypa_log_out("Super item #%d: invalid sector coordinates!\n", _o.field_2d90->supetItems_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->inactive_bp )
        {
            ypa_log_out("Super item #%d: no <inactive_bp> defined!\n", _o.field_2d90->supetItems_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->active_bp )
        {
            ypa_log_out("Super item #%d: no <active_bp> defined!\n", _o.field_2d90->supetItems_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( !_s->trigger_bp )
        {
            ypa_log_out("Super item #%d: no <trigger_bp> defined!\n", _o.field_2d90->supetItems_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        if ( _s->type != 1 && _s->type != 2 )
        {
            ypa_log_out("Super item #%d: no valid <type> defined!\n", _o.field_2d90->supetItems_count);
            return ScriptParser::RESULT_BAD_DATA;
        }

        _o.field_2d90->supetItems_count++;
        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "sec_x") )
    {
        _s->sec_x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "sec_y") )
    {
        _s->sec_y = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "inactive_bp") )
    {
        _s->inactive_bp = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "active_bp") )
    {
        _s->active_bp = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "trigger_bp") )
    {
        _s->trigger_bp = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "keysec_x") )
    {
        _s->keySectors[_s->keySectors_count].x = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "keysec_y") )
    {
        _s->keySectors[_s->keySectors_count].y = std::stol(p2, NULL, 0);
        _s->keySectors_count++;
    }
    else if ( !StriCmp(p1, "mb_status") )
    {
        if ( !StriCmp(p2, "known") )
        {
            _s->mb_status = 0;
        }
        else if ( !StriCmp(p2, "unknown") )
        {
            _s->mb_status = 1;
        }
        else if ( !StriCmp(p2, "hidden") )
        {
            _s->mb_status = 2;
        }
        else
            return ScriptParser::RESULT_BAD_DATA;
    }
    else if ( !StriCmp(p1, "type") )
    {
        _s->type = std::stol(p2, NULL, 0);
    }
    else if ( !StriCmp(p1, "countdown") )
    {
        _s->countdown = std::stol(p2, NULL, 0);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

Common::PlaneBytes *MapAsPlaneBytes::ReadMapAsPlaneBytes(ScriptParser::Parser &parser)
{
    std::string buf;
    parser.ReadLine(&buf);

    std::string tmp;
    Stok stok(buf, " \r\n");
    stok.GetNext(&tmp);
    int w = std::stol(tmp, NULL, 0);
    stok.GetNext(&tmp);
    int h = std::stol(tmp, NULL, 0);
    
    Common::PlaneBytes *bmp = new Common::PlaneBytes;
    bmp->Resize(w, h);

    for (int j = 0; j < h; j++)
    {
        parser.ReadLine(&buf);
        stok = buf;

        uint8_t *ln = bmp->Line(j);

        for (int i = 0; i < w; i++)
        {
            stok.GetNext(&tmp);
            ln[i] = std::stol(tmp, NULL, 16);
        }
    }

    return bmp;
}

bool LevelMapsParser::IsScope(ScriptParser::Parser &parser, const std::string &word, const std::string &opt)
{
    if ( StriCmp(word, "begin_maps") )
        return false;

    if ( _o.typ_map )
    {
        delete _o.typ_map;
        _o.typ_map = NULL;
    }

    if ( _o.own_map )
    {
        delete _o.own_map;
        _o.own_map = NULL;
    }

    if ( _o.hgt_map )
    {
        delete _o.hgt_map;
        _o.hgt_map = NULL;
    }

    if ( _o.blg_map )
    {
        delete _o.blg_map;
        _o.blg_map = NULL;
    }

    return true;
}

int LevelMapsParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( !StriCmp(p1, "typ_map") )
    {
        _o.typ_map = ReadMapAsPlaneBytes(parser);

        if ( !_o.typ_map )
            return ScriptParser::RESULT_BAD_DATA;

        _m.secXsize = _o.typ_map->Width();
        _m.secYsize = _o.typ_map->Height();

        _m.flags |= 4;
    }
    else if ( !StriCmp(p1, "own_map") )
    {
        _o.own_map = ReadMapAsPlaneBytes(parser);
        if ( !_o.own_map )
            return ScriptParser::RESULT_BAD_DATA;

        _m.flags |= 8;
    }
    else if ( !StriCmp(p1, "hgt_map") )
    {
        _o.hgt_map = ReadMapAsPlaneBytes(parser);
        if ( !_o.hgt_map )
            return ScriptParser::RESULT_BAD_DATA;

        _m.flags |= 0x10;
    }
    else if ( !StriCmp(p1, "blg_map") )
    {
        _o.blg_map = ReadMapAsPlaneBytes(parser);
        if ( !_o.blg_map )
            return ScriptParser::RESULT_BAD_DATA;

        _m.flags |= 0x20;
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}



int VideoParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

        if ( win3d )
        {
            int txt16bit = win3d->getWDD_16bitTex();
            int simple_d3d = win3d->getWDD_drawPrim();

            if ( simple_d3d )
                _o.GameShell->GFX_flags |= World::GFX_FLAG_DRAWPRIMITIVES;
            else
                _o.GameShell->GFX_flags &= ~World::GFX_FLAG_DRAWPRIMITIVES;

            if ( txt16bit )
                _o.GameShell->GFX_flags |= World::GFX_FLAG_16BITTEXTURE;
            else
                _o.GameShell->GFX_flags &= ~World::GFX_FLAG_16BITTEXTURE;
        }
        return ScriptParser::RESULT_SCOPE_END;
    }

    _o.GameShell->_saveDataFlags |= World::SDF_VIDEO;

    if ( !StriCmp(p1, "videomode") )
    {
        int modeid = std::stoi(p2);
        const video_mode_node *vdmd = NULL;
        int index = 0;

        for (const auto &nod : _o.GameShell->video_mode_list)
        {
            if (modeid == nod.sort_id)
            {
                vdmd = &nod;
                break;
            }
            index++;
        }

        if ( !vdmd )
        {
            ypa_log_out("Warning: This machine doesn't support mode %d\n", modeid);

            index = 0;
            for (const auto &nod : _o.GameShell->video_mode_list)
            {
                if (nod.width == 640 && nod.height == 480)
                {
                    vdmd = &nod;
                    break;
                }
                index++;
            }
        }

        if ( !vdmd )
            printf("vdmd is NULL %s:%d\n", __FILE__, __LINE__);

        _o.GameShell->field_FBE = index;
        _o.game_default_res = vdmd->sort_id;
        _o.GameShell->game_default_res = vdmd->sort_id;
    }
    else if ( !StriCmp(p1, "farview") )
    {
        if ( StrGetBool(p2) )
        {
            _o.GameShell->GFX_flags |= World::GFX_FLAG_FARVIEW;
            _o.SetFarView(true);
        }
        else
        {
            _o.GameShell->GFX_flags &= ~World::GFX_FLAG_FARVIEW;
            _o.SetFarView(false);
        }
    }
    else if ( !StriCmp(p1, "filtering") )
    {
    }
    else if ( !StriCmp(p1, "drawprimitive") )
    {
        if ( StrGetBool(p2) )
            _o.GameShell->GFX_flags |= World::GFX_FLAG_DRAWPRIMITIVES;
        else
            _o.GameShell->GFX_flags &= ~World::GFX_FLAG_DRAWPRIMITIVES;
    }
    else if ( !StriCmp(p1, "16bittexture") )
    {
        if ( StrGetBool(p2) )
            _o.GameShell->GFX_flags |= World::GFX_FLAG_16BITTEXTURE;
        else
            _o.GameShell->GFX_flags &= ~World::GFX_FLAG_16BITTEXTURE;
    }
    else if ( !StriCmp(p1, "softmouse") )
    {
        if ( StrGetBool(p2) )
        {
            _o.GameShell->GFX_flags |= World::GFX_FLAG_SOFTMOUSE;
            _o.field_73CE |= World::PREF_SOFTMOUSE;

            _o._win3d->setWDD_cursor(1);
        }
        else
        {
            _o.GameShell->GFX_flags &= ~World::GFX_FLAG_SOFTMOUSE;
            _o.field_73CE &= ~World::PREF_SOFTMOUSE;

            _o._win3d->setWDD_cursor(0);
        }
    }
    else if ( !StriCmp(p1, "palettefx") )
    {
    }
    else if ( !StriCmp(p1, "heaven") )
    {
        if ( StrGetBool(p2) )
        {
            _o.GameShell->GFX_flags |= World::GFX_FLAG_SKYRENDER;
            _o.setYW_skyRender(true);
        }
        else
        {
            _o.GameShell->GFX_flags &= ~World::GFX_FLAG_SKYRENDER;
            _o.setYW_skyRender(false);
        }
    }
    else if ( !StriCmp(p1, "fxnumber") )
    {
        _o.fxnumber = std::stoi(p2);
        _o.GameShell->fxnumber = _o.fxnumber;
    }
    else if ( !StriCmp(p1, "enemyindicator") )
    {
        if ( StrGetBool(p2) )
        {
            _o.field_73CE |= World::PREF_ENEMYINDICATOR;
            _o.GameShell->enemyindicator = true;
        }
        else
        {
            _o.field_73CE &= ~World::PREF_ENEMYINDICATOR;
            _o.GameShell->enemyindicator = false;
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

    _o.GameShell->_saveDataFlags |= World::SDF_SOUND;

    if ( !StriCmp(p1, "channels") )
    {
    }
    else if ( !StriCmp(p1, "volume") )
    {
        _o.GameShell->snd__volume = std::stoi(p2);
        SFXEngine::SFXe.setMasterVolume(_o.GameShell->snd__volume);
    }
    else if ( !StriCmp(p1, "cdvolume") )
    {
        _o.GameShell->snd__cdvolume = std::stoi(p2);
        SFXEngine::SFXe.SetMusicVolume(_o.GameShell->snd__cdvolume);
    }
    else if ( !StriCmp(p1, "invertlr") )
    {
        if ( !StriCmp(p2, "yes") )
        {
            _o.GameShell->snd__flags2 |= World::SF_INVERTLR;
            SFXEngine::SFXe.setReverseStereo(true);
        }
        else
        {
            _o.GameShell->snd__flags2 &= ~World::SF_INVERTLR;
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
            _o.GameShell->snd__flags2 |= World::SF_CDSOUND;
            _o.field_73CE |= World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(true);
        }
        else
        {
            _o.GameShell->snd__flags2 &= ~World::SF_CDSOUND;
            _o.field_73CE &= ~World::PREF_CDMUSICDISABLE;

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

    _levelId = std::stoi(opt);
    return true;
};

int LevelStatusParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
        return ScriptParser::RESULT_SCOPE_END;

    if ( _setFlag )
        _o.GameShell->_saveDataFlags |= World::SDF_SCORE;

    if ( !StriCmp(p1, "status") )
    {
        if ( _o.LevelNet->mapInfos[_levelId].field_0 )
            _o.LevelNet->mapInfos[_levelId].field_0 = std::stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

int BuddyParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _o.field_2d90->buddies_count++;

        return ScriptParser::RESULT_SCOPE_END;
    }

    if ( !StriCmp(p1, "commandid") )
    {
        _o.field_2d90->buddies[ _o.field_2d90->buddies_count ].commandid = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "type") )
    {
        _o.field_2d90->buddies[ _o.field_2d90->buddies_count ].type = std::stoi(p2);
    }
    else if ( !StriCmp(p1, "energy") )
    {
        _o.field_2d90->buddies[ _o.field_2d90->buddies_count ].energy = std::stoi(p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}

void ShellParser::ParseStatus(save_status *status, const std::string &p2)
{
    Stok stok(p2, " _");
    std::string val;

    if ( stok.GetNext(&val) )
        status->p1 = std::stoi(val);

    if ( stok.GetNext(&val) )
        status->p2 = std::stoi(val);

    if ( stok.GetNext(&val) )
        status->p3 = std::stoi(val);

    if ( stok.GetNext(&val) )
        status->p4 = std::stoi(val);

    if ( stok.GetNext(&val) )
        status->p5 = std::stoi(val);

    if ( stok.GetNext(&val) )
        status->p6 = std::stoi(val);

    for (auto &x : status->pX)
    {
        if ( !stok.GetNext(&val) )
            break;

        x = std::stoi(val);
    }
}


int ShellParser::Handle(ScriptParser::Parser &parser, const std::string &p1, const std::string &p2)
{
    if ( !StriCmp(p1, "end") )
    {
        _o.field_739A = 1;
        return ScriptParser::RESULT_SCOPE_END;
    }

    _o.GameShell->_saveDataFlags |= World::SDF_SHELL;

    if ( !StriCmp(p1, "LANGUAGE") )
    {
        std::string * deflt = NULL;
        std::string * slct = NULL;

        for(std::string &s : _o.GameShell->lang_dlls)
        {
            if ( !StriCmp(s, p2) )
                slct = &s;
            if ( !StriCmp(s, "language") )
                deflt = &s;
        }

        if ( slct )
            _o.GameShell->default_lang_dll = slct;
        else
            _o.GameShell->default_lang_dll = deflt;

        _o.GameShell->prev_lang = _o.GameShell->default_lang_dll;

        if ( !_o.ypaworld_func175(_o.GameShell) )
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
        ParseStatus(&_o.robo_map_status, p2);
    }
    else if ( !StriCmp(p1, "robo_finder_status") )
    {
        ParseStatus(&_o.robo_finder_status, p2);
    }
    else if ( !StriCmp(p1, "vhcl_map_status") )
    {
        ParseStatus(&_o.vhcl_map_status, p2);
    }
    else if ( !StriCmp(p1, "vhcl_finder_status") )
    {
        ParseStatus(&_o.vhcl_finder_status, p2);
    }
    else
        return ScriptParser::RESULT_UNKNOWN;

    return ScriptParser::RESULT_OK;
}




}
}
