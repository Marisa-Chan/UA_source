#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypaflyer.h"
#include "log.h"
#include <math.h>


const Nucleus::ClassDescr NC_STACK_ypaflyer::description("ypaflyer.class", &newinstance);

size_t NC_STACK_ypaflyer::func0(IDVList &stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case FLY_ATT_TYPE:
                setFLY_type(val.Get<int32_t>());
                break;
            default:
                break;
            }
        }
    }

    _bact_type = BACT_TYPES_FLYER;

    _flyerBoost = 0;
    _flyerType = 0;

    return 1;
}

size_t NC_STACK_ypaflyer::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypaflyer::func2(IDVList &stak)
{
    NC_STACK_ypabact::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case FLY_ATT_TYPE:
                setFLY_type(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_ypaflyer::sb_0x4b255c(float a2, vec3d v, int a4)
{
    float v38 = _force / _airconst;

    if ( _fly_dir.x != 0.0 || _fly_dir.y != 0.0 || _fly_dir.z != 0.0 )
    {
        if ( v.x == 0.0 && v.y == 0.0 && v.z == 0.0 )
            v = _rotation.AxisZ();

        vec2d tmp = _rotation.AxisZ().XZ();

        float v56 = clp_asin( _rotation.m01 );
        float v52 = clp_asin( _rotation.m21 );

        float v49 = 0.0;

        if ( _flyerType & 1 && _fly_dir_length >= 0.1 )
            v49 = clp_asin( _fly_dir.y ) * _fly_dir_length / v38;

        if ( v49 > 1.2 )
            v49 = 1.2;
        else if ( v49 < -0.8 )
            v49 = -0.8;

        float v45;

        if ( v52 >= 0.0 )
            v45 = (v52 + 0.6) * 0.8;
        else
            v45 = (v52 + -0.6) * -0.8;

        float v51 = -(v49 - v52);

        if ( v51 >= 0.0 )
        {
            float maxrot = _maxrot * a2 * v45;

            if ( v51 <= maxrot )
            {
                if ( v51 < 0.0005 )
                    v51 = 0.0;
            }
            else
            {
                v51 = maxrot;
            }
        }
        else
        {
            float maxrot = -_maxrot * a2 * v45;
            if ( v51 >= maxrot )
            {
                if ( v51 > -0.0005 )
                    v51 = 0.0;
            }
            else
            {
                v51 = maxrot;
            }
        }

        vec3d vaxis = _rotation.AxisX().X0Z();

        if ( vaxis.normalise() >= 0.001 )
            _rotation *= mat3x3::AxisAngle(vaxis, v51);

        if ( _flyerType & 2 )
        {
            if ( (tmp.x != 0.0 || tmp.y != 0.0) && (v.x != 0.0 || v.z != 0.0) )
            {
                float v57 = clp_acos( tmp.dot( v.XZ() ) / tmp.length() / v.XZ().length() );

                if ( v57 < 0.001 )
                    v57 = 0.0;

                if ( tmp.cross( v.XZ() ) < 0.0 )
                    v57 = -v57;

                float v58 = 0.0;

                if ( _fly_dir_length >= 0.1 )
                {
                    float aa2 = a2;

                    NDIV_CARRY(aa2);

                    v58 = fabs(_fly_dir_length) * _airconst / _force * (v57 * 1.45 / (aa2 * _maxrot));
                }


                float v44 = 1.0;
                if ( !a4 )
                {
                    if ( v56 >= 0.0 )
                        v44 = v56 + 0.4;
                    else
                        v44 = -(v56 - 0.4);
                }

                float v50 = v58 - v56;

                if ( v50 >= 0.0 )
                {
                    float v40 = _maxrot * a2 * v44;

                    if ( v50 <= v40 )
                    {
                        if ( v50 < 0.001 )
                            v50 = 0.0;
                    }
                    else
                    {
                        v50 = v40;
                    }
                }
                else
                {
                    float v43 = -_maxrot * a2 * v44;
                    if ( v50 >= v43 )
                    {
                        if ( v50 > -0.001 )
                            v50 = 0.0;
                    }
                    else
                    {
                        v50 = v43;
                    }
                }

                _rotation = mat3x3::RotateZ(v50) * _rotation;
            }
        }
        else
        {
            if ( v56 >= 0.0 )
            {
                float v41 = _maxrot * a2 * v45;

                if ( v56 <= v41 )
                {
                    if ( v56 < 0.001 )
                        v56 = 0.0;
                }
                else
                {
                    v56 = v41;
                }
            }
            else
            {
                float v39 = -_maxrot * a2 * v45;

                if ( v56 >= v39 )
                {
                    if ( v56 > -0.001 )
                        v56 = 0.0;
                }
                else
                {
                    v56 = v39;
                }
            }

            _rotation = mat3x3::RotateZ(-v56) * _rotation;
        }
    }
}


void NC_STACK_ypaflyer::ypaflyer_func70__sub0(float angl)
{
    if ( _target_dir.x != 0.0 || _target_dir.z != 0.0 )
    {
        float v21 = clp_acos( _target_dir.XZ().dot( _rotation.AxisZ().XZ() ) / _target_dir.XZ().length() / _rotation.AxisZ().XZ().length() );

        float v9 = _maxrot * angl;

        if ( fabs(v21) > v9 )
            v21 = v9;

        if ( _target_dir.XZ().cross( _rotation.AxisZ().XZ() ) > 0.0 )
            v21 = -v21;

        _rotation = mat3x3::RotateY(v21) * _rotation;
    }

    _thraction = _force;

    float v14;

    if ( fabs(_target_dir.y) >= 0.1 )
    {
        if ( _target_dir.y > -0.1 )
            v14 = _mass * 0.7;
        else
            v14 = _mass * 2.5;
    }
    else
        v14 = _mass;

    _flyerBoost = v14 * 9.80665;

    if ( _status_flg & BACT_STFLAG_UPWRD )
        _flyerBoost = _mass * 2.5 * 9.80665;
}



void NC_STACK_ypaflyer::AI_layer3(update_msg *arg)
{
    float a2a = arg->frameTime / 1000.0;

    float v88 = _target_vec.length();

    if ( v88 > 0.0 )
        _target_dir = _target_vec / v88;

    int a4 = getBACT_viewer();
    int v82 = getBACT_exactCollisions();
    int v79 = getBACT_bactCollisions();

    int v90 = _world->ypaworld_func145(this);

    float v91;

    if ( a4 )
        v91 = _viewer_radius;
    else
        v91 = _radius;

    switch ( _status )
    {
    case BACT_STATUS_NORMAL:
    {
        vec3d a3 = _rotation.AxisZ();

        if ( v79 )
        {
            if ( v90 || (!_secndTtype && v88 < 1200.0) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( !_primTtype && !_secndTtype )
        {
            _status = BACT_STATUS_IDLE;

            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
            break;
        }

        //printf(" %f %f %f\n", bact->field_639.x, bact->field_639.y, bact->field_639.z);

        ypaworld_arg136 arg136;
        arg136.isect = 0;
        arg136.flags = 0;
        arg136.stPos = _old_pos;
        arg136.vect = (_position - _old_pos).X0Z();

        float v89 = arg136.vect.XZ().length();

        if ( v89 <= 0.0 )
            arg136.vect = _rotation.AxisZ() * 300.0;
        else
            arg136.vect *= 300.0 / v89;

        ypaworld_arg136 arg136_1;

        arg136_1.isect = 0;
        arg136_1.flags = 0;

        if ( a4 || _status_flg & BACT_STFLAG_DODGE_RIGHT || (v90 && v82) )
        {
            arg136_1.stPos = _old_pos;

            arg136_1.vect.x = arg136.vect.x * 0.93969 - arg136.vect.z * 0.34202;
            arg136_1.vect.y = arg136.vect.y;
            arg136_1.vect.z = arg136.vect.z * 0.93969 + arg136.vect.x * 0.34202;

            _world->ypaworld_func136(&arg136_1);
        }

        ypaworld_arg136 arg136_2;

        arg136_2.isect = 0;
        arg136_2.flags = 0;

        if ( a4 || _status_flg & BACT_STFLAG_DODGE_LEFT || (v90 && v82) )
        {
            arg136_2.stPos = _old_pos;
            arg136_2.vect.x = arg136.vect.x * 0.93969 + arg136.vect.z * 0.34202;
            arg136_2.vect.y = arg136.vect.y;
            arg136_2.vect.z = arg136.vect.z * 0.93969 - arg136.vect.x * 0.34202;

            _world->ypaworld_func136(&arg136_2);
        }

        if ( a4 || !(_status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) || (v90 && v82) )
            _world->ypaworld_func136(&arg136);

        int v18 = 0;

        bact_arg88 arg88;
        arg88.pos1 = 0;

        if ( arg136.isect )
        {
            if ( v89 + v91 > arg136.tVal * 300.0 )
            {
                arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                v18++;
            }
        }

        if ( arg136_1.isect )
        {
            if ( v89 + v91 > arg136_1.tVal * 300.0 )
            {
                arg88.pos1 += arg136_1.skel->polygons[arg136_1.polyID].Normal();

                v18++;
            }
        }

        if ( arg136_2.isect )
        {
            if ( v89 + v91 > arg136_2.tVal * 300.0 )
            {
                arg88.pos1 += arg136_2.skel->polygons[arg136_2.polyID].Normal();

                v18++;
            }
        }

        if ( v18 )
        {
            arg88.pos1 /= (float)v18;

            Recoil(&arg88);
        }
        else
        {
            _status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
        {
            _status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT);
            _status_flg |= BACT_STFLAG_MOVE;
        }

        if ( !(_status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) )
        {
            if ( arg136_1.isect == 1 && arg136_2.isect == 1 )
            {
                if ( arg136_1.tVal >= arg136_2.tVal )
                    _status_flg |= BACT_STFLAG_DODGE_LEFT;
                else
                    _status_flg |= BACT_STFLAG_DODGE_RIGHT;
            }

            if ( arg136_1.isect == 1 && !arg136_2.isect )
                _status_flg |= BACT_STFLAG_DODGE_RIGHT;

            if ( !arg136_1.isect && arg136_2.isect == 1 )
                _status_flg |= BACT_STFLAG_DODGE_LEFT;

            if ( !arg136_1.isect && !arg136_2.isect && arg136.isect == 1 )
                _status_flg |= BACT_STFLAG_DODGE_LEFT;
        }

        ypaworld_arg136 arg136_3;

        arg136_3.stPos = _old_pos;

        float v22 = _mass * 9.80665;

        if ( v22 <= _force )
            v22 = _force;

        float v100 = _airconst;

        if ( v100 < 10.0 )
            v100 = 10.0;

        arg136_3.vect.x = (_fly_dir.x * 200.0 * _fly_dir_length) / (v22 / v100);

        if ( arg136_3.vect.x < -200.0 )
            arg136_3.vect.x = -200.0;

        if ( arg136_3.vect.x > 200.0 )
            arg136_3.vect.x = 200.0;

        arg136_3.vect.y = _height;

        arg136_3.vect.z = (_fly_dir.z * 200.0 * _fly_dir_length) / (v22 / v100);

        if ( arg136_3.vect.z < -200.0 )
            arg136_3.vect.z = -200.0;

        if ( arg136_3.vect.z > 200.0 )
            arg136_3.vect.z = 200.0;

        arg136_3.flags = 0;

        _world->ypaworld_func136(&arg136_3);

        if ( arg136_3.isect )
        {
            _target_dir.y = -(1.0 - arg136_3.tVal);
        }
        else
        {
            NC_STACK_ypabact *v81 = _world->getYW_userVehicle();

            if ( ( (_secndTtype != BACT_TGT_TYPE_UNIT || (_secndT.pbact->_bact_type != BACT_TYPES_ROBO && v81 != _secndT.pbact))
                    && (_primTtype != BACT_TGT_TYPE_UNIT || (_primT.pbact->_bact_type != BACT_TYPES_ROBO && v81 != _primT.pbact)) )
                    || _target_dir.y >= -0.01 )
            {
                if ( _target_dir.y < 0.15 )
                    _target_dir.y = 0.15;
            }
        }

        if ( _status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT) )
            _target_dir.y = -0.2;

        if ( arg136_3.isect && arg136_3.tVal * _height < _radius && _fly_dir.y > 0.0 )
        {
            arg88.pos1 = arg136_3.skel->polygons[arg136_3.polyID].Normal();

            Recoil(&arg88);
        }
        else
        {
            _status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( _target_dir.y != 0.0 )
            _target_dir.normalise();

        float v92 = arg136.vect.XZ().length();

        NDIV_CARRY(v92);

        if ( _status_flg & BACT_STFLAG_DODGE_LEFT )
        {
            _target_dir.x = -arg136.vect.z / v92;
            _target_dir.z = arg136.vect.x / v92;
        }
        else if ( _status_flg & BACT_STFLAG_DODGE_RIGHT )
        {
            _target_dir.x = arg136.vect.z  / v92;
            _target_dir.z = -arg136.vect.x  / v92;
        }

        ypaflyer_func70__sub0(a2a);

        move_msg arg74;
        arg74.flag = 0;
        arg74.field_0 = a2a;

        Move(&arg74);

        sb_0x4b255c(a2a, a3, 0);


        bact_arg75 arg75;
        arg75.fperiod = a2a;
        arg75.g_time = _clock;

        //printf("%d %d \n", bact->field_3DF, bact->field_3DE);

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
            arg75.prio = 0;
            arg75.target.pbact = _primT.pbact;

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
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

                SetState(&arg78);
            }
        }

    }
    break;

    case BACT_STATUS_DEAD:
        DeadTimeUpdate(arg);
        break;

    case BACT_STATUS_IDLE:
    {

        if ( _clock - _newtarget_time > 500 )
        {
            bact_arg110 arg110;
            arg110.tgType = _secndTtype;
            arg110.priority = 1;

            bact_arg110 arg110_1;
            arg110_1.tgType = _primTtype;
            arg110_1.priority = 0;

            int v52 = TargetAssess(&arg110);

            int v55 = TargetAssess(&arg110_1);

            if ( v52 != TA_IGNORE || v55 != TA_IGNORE )
            {

                if ( v52 == TA_CANCEL )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( v55 == TA_CANCEL )
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

        int v80 = getBACT_landingOnWait();

        if ( v80 )
        {
            _thraction = 0;
            _flyerBoost = 0;

            if ( _status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;

                SetState(&arg78);

                ypaworld_arg136 arg136_4;
                arg136_4.stPos = _position;
                arg136_4.vect = vec3d::OY(_overeof + 50.0);
                arg136_4.flags = 0;

                _world->ypaworld_func136(&arg136_4);

                if ( arg136_4.isect )
                    _position.y = arg136_4.isectPos.y - _overeof;
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

    case BACT_STATUS_CREATE:
        CreationTimeUpdate(arg);
        break;

    case BACT_STATUS_BEAM:
        BeamingTimeUpdate(arg);
        break;
    }
}


void NC_STACK_ypaflyer::User_layer(update_msg *arg)
{
    _airconst = _airconst_static;

    float a2 = (float)arg->frameTime / 1000.0;

    int a4 = getBACT_bactCollisions();

    if ( _status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE )
    {
        vec3d a3;
        a3 = _rotation.AxisZ();

        float v60 = -arg->inpt->Sliders[0] * _maxrot * a2;

        if ( a4 )
        {
            if ( !(_status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        float v61;

        if ( _status_flg & BACT_STFLAG_LAND )
            v61 = 0.1;
        else
            v61 = 1.0;

        if ( _fly_dir_length >= v61 )
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
            arg136.vect.z = 0;
            arg136.vect.x = 0;

            float v9;

            if ( _viewer_overeof <= _viewer_radius )
                v9 = _viewer_radius;
            else
                v9 = _viewer_overeof;

            arg136.flags = 0;
            arg136.vect.y = v9 * 1.5;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect && _thraction < _force * 0.001 && _flyerBoost <= _mass * 9.80665 )
            {
                _status_flg |= BACT_STFLAG_LAND;
                _fly_dir_length = 0;
                _thraction = 0;

                _flyerBoost = _mass * 9.80665;
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_LAND;
            }

            float tmp = 0.0;

            if ( _primTtype == BACT_TGT_TYPE_CELL)
                tmp = (_primTpos.XZ() - _position.XZ()).length();

            if ( _primTtype != BACT_TGT_TYPE_CELL || tmp <= 800.0 )
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

        _rotation *= mat3x3::RotateY(v60);

        _thraction += _force * (a2 * 0.3) * arg->inpt->Sliders[2];

        if ( _thraction > _force )
            _thraction = _force;

        if ( _thraction < 0.0 )
            _thraction = 0;


        _flyerBoost = (fabs(_fly_dir_length) / 111.0 + 1.0) * (arg->inpt->Sliders[1] * 20000.0) * 0.5 + _mass * 9.80665;

        float v22 = _pSector->height - _position.y;

        float v63 = 1.0 / _height_max_user;

        float v72 = _mass * 7.0 * 9.80665  * ( 1.0 - ( POW2(v22) * POW2(v63) ) );

        if ( _flyerBoost > v72 )
            _flyerBoost = v72;

        bact_arg79 arg79;
        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = _rotation.AxisZ();

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = _rotation.AxisZ();

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->Buttons.IsAny({0, 1}) )
        {
            arg79.direction = vec3d(0.0, 0.0, 0.0);
            arg79.weapon = _weapon;
            arg79.g_time = _clock;

            if ( _clock % 2 )
                arg79.start_point.x = _fire_pos.x;
            else
                arg79.start_point.x = -_fire_pos.x;

            arg79.start_point.y = _fire_pos.y;
            arg79.start_point.z = _fire_pos.z;
            arg79.flags = (arg->inpt->Buttons.Is(1) ? 1 : 0) | 2;

            LaunchMissile(&arg79);
        }

        if ( _mgun != -1 )
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                if ( !arg->inpt->Buttons.Is(2) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            if ( arg->inpt->Buttons.Is(2))
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = _rotation.AxisZ();
                arg105.field_C = a2;
                arg105.field_10 = _clock;

                FireMinigun(&arg105);
            }
        }

        if ( arg->inpt->Buttons.Is(3) )
            HandBrake(arg);

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;
            arg74.field_0 = a2;
            arg74.flag = 0;

            Move(&arg74);
        }
        else
        {
            vec3d tmp(0.0, 0.0, 0.0);

            yw_137col v43[10];

            for (int i = 3; i >= 0; i--)
            {
                int v67 = 0;

                move_msg arg74;
                arg74.field_0 = a2;
                arg74.flag = 0;

                Move(&arg74);

                ypaworld_arg137 arg137;

                arg137.pos = _position + _rotation.AxisZ() * 0.5 * _viewer_radius;
                arg137.pos2 = _fly_dir;
                arg137.radius = _viewer_radius;
                arg137.coll_max = 10;
                arg137.field_30 = 0;
                arg137.collisions = v43;

                _world->ypaworld_func137(&arg137);

                if ( arg137.coll_count )
                {
                    tmp = vec3d(0.0, 0.0, 0.0);

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                        tmp += arg137.collisions[ j ].pos2;

                    bact_arg88 arg88;
                    if ( tmp.normalise() != 0.0 )
                        arg88.pos1 = tmp;
                    else
                        arg88.pos1 = _fly_dir;

                    Recoil(&arg88);

                    _flyerBoost = _mass * 9.80665;

                    v67 = 1;
                }

                if ( !v67 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = _old_pos;
                    arg136.vect = _position - _old_pos;
                    arg136.flags = 0;

                    _world->ypaworld_func149(&arg136);

                    if ( arg136.isect )
                    {
                        bact_arg88 arg88;
                        arg88.pos1 = arg136.skel->polygons[ arg136.polyID ].Normal();

                        Recoil(&arg88);

                        v67 = 1;
                    }
                }

                if ( !v67 )
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
                        arg180.field_8 = 10.0 * tmp.x + _position.x;
                        arg180.field_C = 10.0 * tmp.z + _position.z;

                        _world->ypaworld_func180(&arg180);
                    }
                }

                _fly_dir.y -= 0.2;
            }

            sb_0x4b255c(a2, a3, 1);
        }
    }
    else if ( _status == BACT_STATUS_DEAD )
        DeadTimeUpdate(arg);
}

void NC_STACK_ypaflyer::Move(move_msg *arg)
{
    _old_pos = _position;

    float v46;

    if ( _status == BACT_STATUS_DEAD )
        v46 = _mass * 39.2266;
    else
        v46 = _mass * 9.80665;

    vec3d tmp(0.0, 0.0, 0.0);
    float v47 = 0.0;

    if ( !(arg->flag & 1) )
    {
        tmp = _rotation.AxisZ().X0Z();
        v47 = _thraction;
    }

    float v35 = 0.0;

    if ( !(arg->flag & 1) )
        v35 = -1.0 * _flyerBoost;

    float v12 = _fly_dir_length * _airconst;

    vec3d tmp2 = vec3d::OY(v46) + tmp * v47 - _fly_dir * v12 + vec3d::OY(v35);

    float v33 = tmp2.length();

    if ( v33 > 0.0 )
    {
        //vec3d tmp3 = bact->fly_dir * bact->fly_dir_length + tmp2 * (v33 / bact->mass * arg->field_0) / v33; //(arg->field_0 / bact->mass);
        vec3d tmp3 = _fly_dir * _fly_dir_length + tmp2 * (arg->field_0 / _mass);

        _fly_dir_length = tmp3.length();

        if ( _fly_dir_length > 0.0 )
            _fly_dir = tmp3 / _fly_dir_length;
    }

    if ( _fly_dir_length > 1.5 )
        _position += _fly_dir * _fly_dir_length * arg->field_0 * 6.0;

    CorrectPositionInLevelBox(NULL);

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    float v48 = fabs(_fly_dir_length) / (_force / _airconst_static);
    float v44;

    if ( _pitch_max <= -0.8 )
        v44 = 1.2;
    else
        v44 = _pitch_max;

    float v49 = v48 * v44;

    if ( v49 > v44 )
        v49 = v44;

    if ( _soundcarrier.samples_data[0].psampl )
        _soundcarrier.samples_data[0].pitch += (_soundcarrier.samples_data[0].psampl->SampleRate + _soundcarrier.samples_data[0].pitch) * v49;
}

size_t NC_STACK_ypaflyer::SetPosition(bact_arg80 *arg)
{
    if ( !NC_STACK_ypabact::SetPosition(arg) )
        return 0;

    _flyerBoost = _mass * 9.80665;
    return 1;
}

void NC_STACK_ypaflyer::Renew()
{
    NC_STACK_ypabact::Renew();

    _flyerType = 0;
    _flyerBoost = 0;
}

void NC_STACK_ypaflyer::HandBrake(update_msg *arg)
{
    NC_STACK_ypabact::HandBrake(arg);

    _thraction = 0;
    _flyerBoost = _mass * 9.80665;
}


void NC_STACK_ypaflyer::setFLY_type(int tp)
{
    _flyerType = tp;
}

int NC_STACK_ypaflyer::getFLY_type()
{
    return _flyerType;
}


