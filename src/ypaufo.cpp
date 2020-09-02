#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypaufo.h"
#include "log.h"

#include <math.h>


const Nucleus::ClassDescr NC_STACK_ypaufo::description("ypaufo.class", &newinstance);


size_t NC_STACK_ypaufo::func0(IDVList &stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    _bact_type = BACT_TYPES_UFO;

    _ufoTogo = 200.0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_INPUTTING:
                setBACT_inputting(val.Get<int32_t>());
                break;

            case UFO_ATT_TOGO:
                setUFO_togo(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypaufo::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypaufo::func2(IDVList &stak)
{
    NC_STACK_ypabact::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_INPUTTING:
                setBACT_inputting(val.Get<int32_t>());
                break;

            case UFO_ATT_TOGO:
                setUFO_togo(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_ypaufo::AI_layer3(update_msg *arg)
{
    float v110 = arg->frameTime / 1000.0;

    int v94 = getBACT_bactCollisions();

    int v5 = _world->ypaworld_func145(this);

    float v108 = _target_vec.length();

    if ( v108 != 0.0 )
        _target_dir = _target_vec / v108;

    int v8 = !_secndTtype && v108 < 1200.0;

    if ( v108 > _ufoTogo )
        v108 = _ufoTogo;

    switch ( _status )
    {
    case BACT_STATUS_NORMAL:
    {
        _thraction = _force;

        if ( !v94
                || (!v5 && !v8)
                || !CollisionWithBact(arg->frameTime) )
        {
            if ( _primTtype || _secndTtype )
            {
                _ufoBoost = _mass * 9.80665;
                _thraction = _force;


                if ( _ufoFlags & 1 )
                {
                    _thraction = 0;

                    if ( fabs(_fly_dir_length * _fly_dir.x) >= 0.1 || fabs(_fly_dir_length * _fly_dir.z) >= 0.1 )
                    {
                        _thraction = 0;
                        _fly_dir_length *= 0.8;
                    }
                    else
                    {
                        if ( _ufoFlags & 8 )
                        {
                            float v17 = _maxrot * v110;

                            if ( fabs(_ufoProcAngle) >= v17 )
                            {
                                if ( _ufoProcAngle < 0.0 )
                                    v17 = -v17;

                                _ufoProcAngle -= v17;

                                _rotation = mat3x3::RotateY(v17) * _rotation;
                            }
                            else
                            {
                                _rotation = mat3x3::RotateY(_ufoProcAngle) * _rotation;

                                _ufoFlags &= 0xFFFFFFF6;
                                _ufoProcAngle = 0;
                                _ufoTogoRisidue = v108;
                            }
                        }
                        else
                        {
                            if ( _ufoFlags & 2 )
                                _ufoBoost = _mass * 4.0 * 9.80665;

                            if ( _ufoFlags & 4 )
                                _ufoBoost = _mass * 0.3 * 9.80665;
                        }
                    }
                }

                move_msg arg74;
                arg74.flag = 0;
                arg74.field_0 = v110;

                Move(&arg74);

                ypaworld_arg136 arg136_2;
                arg136_2.stPos = _old_pos;

                vec2d ttmp = _position.XZ() - _old_pos.XZ();

                float v29 = ttmp.length();

                if ( v29 >= 0.01 )
                    arg136_2.vect = vec3d::X0Z(ttmp * 300.0 / v29);
                else
                    arg136_2.vect = _rotation.AxisZ() * 300.0;

                ypaworld_arg136 arg136;
                arg136.stPos = _position;
                arg136.vect = vec3d::OY( _height );

                arg136_2.flags = 0;
                arg136.flags = 0;

                _world->ypaworld_func136(&arg136);

                _world->ypaworld_func136(&arg136_2);

                if ( (arg136_2.isect && arg136_2.tVal * 300.0 < _radius) || (arg136.isect && arg136.tVal * 300.0 < _radius) )
                {
                    bact_arg88 arg88;

                    if ( arg136_2.isect && arg136_2.tVal * 300.0 < _radius )
                        arg88.pos1 = arg136_2.skel->polygons[arg136_2.polyID].Normal();
                    else
                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                    Recoil(&arg88);
                }

                if ( _ufoFlags & 1 )
                {
                    if ( _ufoFlags & 6 )
                    {

                        if ( _ufoFlags & 2 )
                        {
                            if ( !arg136.isect || (arg136.isect && arg136.tVal > 0.8) )
                            {
                                _ufoFlags &= 0xFFFFFFFC;
                                _ufoTogoRisidue = v108;
                            }
                        }

                        if ( _ufoFlags & 4 )
                        {
                            if ( arg136.isect && arg136.tVal < 0.8 )
                            {
                                _ufoFlags &= 0xFFFFFFFA;
                                _ufoTogoRisidue = v108;
                            }
                        }

                    }
                }
                else if ( (arg136_2.isect && arg136_2.tVal < 0.5) || (arg136.isect && arg136.tVal < 0.5) )
                {
                    if ( arg136.isect && arg136.tVal < 0.5 )
                    {
                        _ufoFlags |= 3;
                    }
                    else
                    {
                        _ufoFlags |= 9;

                        vec2d polv = arg136_2.skel->polygons[arg136_2.polyID].Normal().XZ();
                        vec2d axsz = _rotation.AxisZ().XZ();

                        float v104 = axsz.dot( polv );

                        float tmpsq = axsz.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        tmpsq = polv.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        _ufoProcAngle = C_PI_2 - clp_acos(v104);
                    }
                }
                else if ( _ufoTogoRisidue <= 0.0 )
                {
                    if ( arg136.isect )
                    {
                        vec2d tgt2d = _target_dir.XZ();
                        vec2d axsz = _rotation.AxisZ().XZ();

                        float v104 = tgt2d.dot( axsz );

                        float tmpsq = axsz.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        tmpsq = tgt2d.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        float v121 = clp_acos(v104);

                        if ( v121 > 0.2 )
                        {
                            if ( axsz.cross( tgt2d ) < 0.0 )
                                v121 = -v121;

                            _ufoProcAngle = v121;
                            _ufoFlags |= 9;
                        }
                    }
                    else
                    {
                        NC_STACK_ypabact *v102 = _world->getYW_userVehicle();;

                        if ( ( (_secndTtype != BACT_TGT_TYPE_UNIT || v102 != _secndT.pbact) && ( _primTtype != BACT_TGT_TYPE_UNIT || v102 != _primT.pbact) ) || _target_dir.y >= 0.0 )
                        {
                            _ufoFlags |= 5;
                        }
                    }
                }

                bact_arg75 arg75;
                arg75.fperiod = v110;
                arg75.g_time = _clock;

                if ( _secndTtype == BACT_TGT_TYPE_UNIT )
                {
                    arg75.target.pbact = _secndT.pbact;
                    arg75.prio = 1;

                    FightWithBact(&arg75);
                }
                else if ( _secndTtype == BACT_TGT_TYPE_CELL )
                {
                    arg75.pos = _sencdTpos;
                    arg75.target.pcell = _secndT.pcell;
                    arg75.prio = 1;

                    FightWithSect(&arg75);
                }
                else if ( _primTtype == BACT_TGT_TYPE_UNIT )
                {
                    arg75.target.pbact = _primT.pbact;
                    arg75.prio = 0;

                    FightWithBact(&arg75);
                }
                else if ( _primTtype == BACT_TGT_TYPE_CELL )
                {
                    arg75.pos = _primTpos;
                    arg75.target.pcell = _primT.pcell;
                    arg75.prio = 0;

                    FightWithSect(&arg75);
                }
                else
                {
                    _status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

                    if ( _status_flg & BACT_STFLAG_FIRE )
                    {
                        setState_msg arg78;
                        arg78.unsetFlags = BACT_STFLAG_FIRE;
                        arg78.newStatus = BACT_STATUS_NOPE;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }
            }
            else
            {
                _status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

                setState_msg arg78;

                if ( _status_flg & BACT_STFLAG_FIRE )
                {
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = 0;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                arg78.newStatus = BACT_STATUS_IDLE;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);
            }
        }
    }
    break;

    case BACT_STATUS_IDLE:
    {
        if ( _clock - _newtarget_time > 500 )
        {
            bact_arg110 arg110;
            bact_arg110 arg110_1;

            arg110.tgType = _secndTtype;
            arg110.priority = 1;

            arg110_1.tgType = _primTtype;
            arg110_1.priority = 0;

            int v63 = TargetAssess(&arg110);

            int v66 = TargetAssess(&arg110_1);

            if ( v63 != TA_IGNORE || v66 != TA_IGNORE )
            {
                if ( v63 == TA_CANCEL )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( v66 == TA_CANCEL )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = _position.x;
                    arg67.tgt_pos.z = _position.z;
                    arg67.priority = 0;

                    SetTarget(&arg67);
                }

                if ( _primTtype || _secndTtype )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = BACT_STFLAG_LAND;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NORMAL;

                    SetState(&arg78);
                    break;
                }
            }
        }

        int v93 = getBACT_landingOnWait();

        if ( v93 == 0 )
        {
            _thraction = 0.0;
            _rotation *= mat3x3::RotateY(_maxrot * v110);
        }
        else
        {
            _thraction = 0.0;

            _ufoBoost = 0;

            if ( _status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;

                SetState(&arg78);

                ypaworld_arg136 arg136_1;

                arg136_1.stPos = _position;
                arg136_1.vect = vec3d::OY(_overeof + 50.0);
                arg136_1.flags = 0;

                _world->ypaworld_func136(&arg136_1);

                if ( arg136_1.isect )
                    _position.y = arg136_1.isectPos.y - _overeof;
            }
            else
            {
                bact_arg86 arg86;
                arg86.field_one = 0;
                arg86.field_two = arg->frameTime;

                CrashOrLand(&arg86);
            }
        }
    }
    break;

    case BACT_STATUS_DEAD:
        DeadTimeUpdate(arg);
        break;

    case BACT_STATUS_CREATE:
        CreationTimeUpdate(arg);
        break;

    case BACT_STATUS_BEAM:
        BeamingTimeUpdate(arg);
        break;
    }
}

void NC_STACK_ypaufo::User_layer(update_msg *arg)
{
    float v88 = arg->frameTime / 1000.0;

    int a4 = getBACT_bactCollisions();

    _old_pos = _position;

    if ( _status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
    else if ( _status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE )
    {

        if ( _fly_dir_length != 0.0 )
        {
            if ( !(_status_flg & BACT_STFLAG_FIRE) )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);
            }

            _status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;
            arg136.stPos = _position;
            arg136.vect = vec3d(0.0, 0.0, 0.0);

            if ( _viewer_overeof <= _viewer_radius )
                arg136.vect.y = _viewer_radius * 1.5;
            else
                arg136.vect.y = _viewer_overeof * 1.5;

            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( !arg136.isect || _thraction != 0.0 || _ufoBoost > _mass * 9.80665 )
            {
                _status_flg &= ~BACT_STFLAG_LAND;
            }
            else
            {
                _status_flg |= BACT_STFLAG_LAND;
                _fly_dir_length = 0;
                _thraction = 0;

                _ufoBoost = _mass * 9.80665;
            }



            if ( _primTtype != BACT_TGT_TYPE_CELL
                    || (_primTpos.XZ() - _position.XZ()).length() <= 800.0 )
            {
                if ( _status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(_status_flg & BACT_STFLAG_FIRE) )
                    {
                        setState_msg arg78;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }

                _status = BACT_STATUS_NORMAL;
            }
            else
            {
                _status = BACT_STATUS_IDLE;

                if ( _status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(_status_flg & BACT_STFLAG_FIRE) )
                    {
                        setState_msg arg78;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }
            }
        }

        float v23 = -arg->inpt->Sliders[0] * _maxrot * v88;

        if ( fabs(v23) > 0.0 )
            _rotation *= mat3x3::RotateY(v23);

        float v25 = arg->inpt->Sliders[1] * _maxrot * v88;

        if ( fabs(v25) > 0.0 )
            _rotation = mat3x3::RotateX(v25) * _rotation;

        if ( arg->inpt->Sliders[2] != 0.0 )
        {
            _ufoBoost = (arg->inpt->Sliders[2] * 4.0 + 1.0) * _mass * 9.80665;

            float v85 = _pSector->height - _position.y;
            float v96 = _height_max_user - v85;
            float v101 = 1.0 / _height_max_user;

            float v100 = 9.80665 * _mass * 5.0 * v96 * v101;

            if ( _ufoBoost > v100 )
                _ufoBoost = v100;
        }
        else
        {
            _ufoBoost = _mass * 9.80665;
        }

        if ( a4 )
        {
            if ( !(_status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( arg->inpt->Buttons.Is(3) )
        {
            _thraction = 0;

            _ufoBoost = _mass * 9.80665;

            _fly_dir_length *= 0.7;

            if ( _fly_dir_length < 0.1 )
                _fly_dir_length = 0;
        }

        bact_arg79 arg79;

        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = _rotation.AxisZ();

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = _rotation.AxisZ();

        if ( UserTargeting(&arg106) )
        {
            arg79.target.pbact = arg106.ret_bact;
            arg79.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->Buttons.IsAny({0, 1}) )
        {
            arg79.weapon = _weapon;
            arg79.direction = _rotation.AxisZ();
            arg79.tgType = BACT_TGT_TYPE_NONE;
            arg79.g_time = _clock;

            if ( _clock % 2 )
                arg79.start_point.x = -_fire_pos.x;
            else
                arg79.start_point.x = _fire_pos.x;

            arg79.start_point.y = _fire_pos.y;
            arg79.start_point.z = _fire_pos.z;
            arg79.flags = (arg->inpt->Buttons.Is(1) ? 1 : 0);

            LaunchMissile(&arg79);
        }

        if ( _weapon == -1 )
        {
            if ( arg->inpt->Buttons.IsAny({0, 1}) )
            {
                if ( _thraction < _force )
                {
                    _thraction += (float)arg->frameTime * _force * 0.0099999998;
                }
            }
            else if ( _thraction > 0.0 )
            {
                _thraction -= (float)arg->frameTime * _force * 0.001;
            }
            else
            {
                _thraction = 0;

                if ( arg->inpt->Sliders[2] == 0.0 )
                    _fly_dir_length *= 0.6;

                if ( _fly_dir_length < 0.1 )
                    _fly_dir_length = 0;
            }
        }
        else
        {
            _thraction = 0;

            if ( fabs(_fly_dir.x * _fly_dir_length) > 0.1 || fabs(_fly_dir.z * _fly_dir_length) > 0.1 )
                _fly_dir_length *= 0.6;

            if ( _fly_dir_length < 0.1 )
                _fly_dir_length = 0;
        }

        if ( _mgun != -1 )
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                if ( arg->inpt->Buttons.Is(2) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            if ( arg->inpt->Buttons.Is(2) )
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_FIRE;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = _rotation.AxisZ();
                arg105.field_C = v88;
                arg105.field_10 = _clock;

                FireMinigun(&arg105);
            }
        }

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;
            arg74.flag = 0;
            arg74.field_0 = v88;

            Move(&arg74);
        }
        else
        {
            yw_137col v60[10];

            for (int i = 3; i >= 0; i--)
            {
                move_msg arg74;
                arg74.flag = 0;
                arg74.field_0 = v88;

                Move(&arg74);

                ypaworld_arg137 arg137;
                arg137.pos = _position;
                arg137.pos2 = _fly_dir;
                arg137.radius = _viewer_radius;
                arg137.collisions = v60;
                arg137.coll_max = 10;
                arg137.field_30 = 0;

                _world->ypaworld_func137(&arg137);

                int v49 = 0;

                vec3d v93(0.0, 0.0, 0.0);

                if ( arg137.coll_count )
                {


                    for (int j = 0 ; j < arg137.coll_count; j++)
                        v93 += arg137.collisions[j].pos2;

                    float v89 = v93.length();

                    bact_arg88 arg88;

                    if ( v89 != 0.0 )
                        arg88.pos1 = v93 / v89;
                    else
                        arg88.pos1 = _fly_dir;

                    _thraction = 0;

                    Recoil(&arg88);

                    v49 = 1;
                }

                if ( v49 == 0 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = _old_pos;
                    arg136.vect = _position - _old_pos;

                    arg136.flags = 0;

                    _world->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        _thraction = 0;

                        bact_arg88 arg88;

                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                        Recoil(&arg88);

                        v49 = 1;
                    }
                }

                if ( v49 == 0 )
                {
                    _status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(_soundcarrier.samples_data[5].flags & 2) )
                {
                    if ( !(_status_flg & BACT_STFLAG_LCRASH) )
                    {
                        _status_flg |= BACT_STFLAG_LCRASH;

                        SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v93.x * 10.0 + _position.x;
                        arg180.field_C = v93.z * 10.0 + _position.z;

                        _world->ypaworld_func180(&arg180);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypaufo::Move(move_msg *arg)
{
    _old_pos = _position;

    float v55;

    if ( _status == BACT_STATUS_DEAD )
        v55 = _mass * 39.2266;
    else
        v55 = _mass * 9.80665;

    vec3d mv(0.0, 0.0, 0.0);

    if ( !(arg->flag & 1) )
    {
        vec3d az = _rotation.AxisZ().X0Z();
        float v9 = az.length();

        if ( v9 <= 0.001 )
            mv = -_rotation.AxisY().X0Z() * _ufoBoost;
        else
            mv = az * (_thraction / v9);
    }

    float v44 = 0.0;

    if ( !(arg->flag & 1) )
        v44 = -1.0 * _ufoBoost;

    vec3d v45 = vec3d::OY(v55) + mv - _fly_dir * (fabs(_fly_dir_length) * _airconst) + vec3d::OY(v44);

    float v52 = v45.length();
    if ( v52 > 0.0 )
    {
        vec3d v42 = _fly_dir * _fly_dir_length + v45 * (arg->field_0 / _mass);

        float v51 = v42.length();
        if ( v51 > 0.0 )
            v42 /= v51;

        _fly_dir = v42;

        _fly_dir_length = v51;
    }

    vec3d a1v = _fly_dir * (_fly_dir_length * arg->field_0 * 6.0);

    if ( fabs(_fly_dir_length) > 0.1 )
        _position += a1v;

    _ufoTogoRisidue -= a1v.length();

    if ( _ufoTogoRisidue < 0.0 )
        _ufoTogoRisidue = 0;

    CorrectPositionInLevelBox(NULL);

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    float v53;

    if ( _pitch_max <= -0.8 )
        v53 = 1.2;
    else
        v53 = _pitch_max;

    float v58 = fabs(_fly_dir_length) / (_force / _airconst_static) * v53;

    if ( v58 > v53 )
        v58 = v53;

    if ( _soundcarrier.samples_data[0].psampl )
        _soundcarrier.samples_data[0].pitch = (_soundcarrier.samples_data[0].psampl->SampleRate + _soundcarrier.samples_data[0].pitch) * v58;

}

size_t NC_STACK_ypaufo::SetPosition(bact_arg80 *arg)
{
    if ( !NC_STACK_ypabact::SetPosition(arg))
        return 0;

    _ufoBoost = _mass * 9.80665;
    return 1;
}

void NC_STACK_ypaufo::Renew()
{
    NC_STACK_ypabact::Renew();

    _ufoBoost = 0;

    setBACT_landingOnWait(0);
}


void NC_STACK_ypaufo::setBACT_inputting(int inpt)
{
    NC_STACK_ypabact::setBACT_inputting(inpt);

    if ( !inpt )
    {
        _rotation.m00 = 1.0;
        _rotation.m01 = 0.0;
        _rotation.m02 = 0.0;
        _rotation.m10 = 0.0;
        _rotation.m11 = 1.0;
        _rotation.m12 = 0.0;
        _rotation.m20 = 0.0;
        _rotation.m21 = 0.0;
        _rotation.m22 = 1.0;
    }
}

void NC_STACK_ypaufo::setUFO_togo(int tog)
{
    _ufoTogo = tog;
}

int NC_STACK_ypaufo::getUFO_togo()
{
    return _ufoTogo;
}
