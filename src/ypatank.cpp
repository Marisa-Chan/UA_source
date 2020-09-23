#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "yw.h"
#include "ypatank.h"
#include "yparobo.h"
#include "log.h"

#include "yw_net.h"


const Nucleus::ClassDescr NC_STACK_ypatank::description("ypatank.class", &newinstance);

NC_STACK_ypatank::NC_STACK_ypatank()
{
    _tankFlags = 0;
    _tankWaitCount = 0;
    _tankCollisionAngle = 0.;
    _tankCollisionWay = 0.;
    _tankCollisionFlag = 0;
    _tankExpectTgt = false;
}

size_t NC_STACK_ypatank::func0(IDVList &stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    _bact_type = BACT_TYPES_TANK;

    _tankFlags = (FLAG_TANK_TIP | FLAG_TANK_ROTWAIT);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case TANK_ATT_TIP:
                setTANK_tip( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypatank::func1()
{
    return NC_STACK_ypabact::func1();
}


size_t NC_STACK_ypatank::func2(IDVList &stak)
{
    NC_STACK_ypabact::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case TANK_ATT_TIP:
                setTANK_tip( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_ypatank::AI_layer3(update_msg *arg)
{
    float v244 = arg->frameTime / 1000.0;

    int v194 = getBACT_bactCollisions();

    float v224 = _radius + 300.0;

    int v212 = _world->ypaworld_func145(this);

    float v220 = _target_vec.length();

    if ( v220 > 0.0 )
        _target_dir = _target_vec / v220;

    bool v215 = !_secndTtype && v220 < 1200.0;

    switch ( _status )
    {
    case BACT_STATUS_NORMAL:
    {
        if ( !(_status_flg & BACT_STFLAG_LAND) )
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->frameTime;

            CrashOrLand(&arg86);
            break;
        }

        if ( !_primTtype && !_secndTtype )
        {
            _status = BACT_STATUS_IDLE;
            _status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

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

        if ( v220 == 0.0 )
            break;

        if ( _waitCol_time > 0 )
        {
            _waitCol_time -= arg->frameTime;
        }
        else
        {

            float v206 = _target_dir.XZ().dot( _rotation.AxisZ().XZ() );

            float tmpsq = _rotation.AxisZ().XZ().length();

            NDIV_CARRY(tmpsq);

            v206 /= tmpsq;

            tmpsq = _target_dir.XZ().length();

            NDIV_CARRY(tmpsq);

            v206 /= tmpsq;

            float v240 = clp_acos(v206);

            if ( _tankCollisionFlag & (COLL_WALL_L | COLL_WALL_R | COLL_HILL_L | COLL_HILL_R) )
            {
                if ( _tankCollisionVector.XZ().dot( _target_dir.XZ() ) < 0.0
                        && _tankCollisionAngle == 0.0
                        && _tankCollisionWay == 0.0 )
                {
                    _tankCollisionWay = 0;
                    _tankCollisionAngle = 0;
                    _tankCollisionFlag &= ~(COLL_WALL_L | COLL_WALL_R | COLL_HILL_L | COLL_HILL_R);
                    break;
                }

                if ( (_tankCollisionFlag & (COLL_HILL_L | COLL_HILL_R)) && 
                     _tankCollisionAngle == 0.0 && 
                     _tankCollisionWay == 0.0 )
                {
                    ypaworld_arg136 arg136;

                    if ( _tankCollisionFlag & COLL_HILL_L )
                    {
                        arg136.stPos = _position - _rotation.AxisX() * (2.0 * _radius);
                        arg136.vect = (_rotation.AxisX() * 4.0 + _rotation.AxisY() * 3.0) * _radius;
                    }
                    else
                    {
                        arg136.stPos = _position + _rotation.AxisX() * (2.0 * _radius);
                        arg136.vect = (_rotation.AxisX() * -4.0 + _rotation.AxisY() * 3.0) * _radius;
                    }
                    arg136.flags = 1;

                    _world->ypaworld_func136(&arg136);

                    if ( !arg136.isect || arg136.skel->polygons[ arg136.polyID ].B > 0.6 )
                        _tankCollisionFlag &= ~(COLL_HILL_L | COLL_HILL_R);

                    if ( !(_tankCollisionFlag & (COLL_HILL_L | COLL_HILL_R)) )
                        _tankCollisionWay = 80.0;
                }

                if ( _tankCollisionFlag & (COLL_WALL_L | COLL_WALL_R) )
                {
                    if ( _tankCollisionAngle == 0.0 && _tankCollisionWay == 0.0 )
                    {
                        ypaworld_arg136 arg136;

                        if ( _tankCollisionFlag & COLL_WALL_L )
                            arg136.vect = -_rotation.AxisX() * 150.0;
                        else
                            arg136.vect = _rotation.AxisX() * 150.0;

                        arg136.stPos = _position;
                        arg136.flags = 1;

                        _world->ypaworld_func136(&arg136);

                        if ( !arg136.isect || arg136.skel->polygons[ arg136.polyID ].B > 0.6 )
                            _tankCollisionFlag &= ~(COLL_WALL_L | COLL_WALL_R);

                        if ( !(_tankCollisionFlag & (COLL_WALL_L | COLL_WALL_R)) )
                            _tankCollisionWay = 80.0;
                    }
                }

                if ( _tankCollisionAngle <= 0.001 )
                {
                    _tankCollisionAngle = 0;
                }
                else
                {
                    float v236 = _maxrot * v244;

                    if ( v236 <= _tankCollisionAngle )
                    {
                        _tankCollisionAngle -= v236;
                    }
                    else
                    {
                        v236 = _tankCollisionAngle;
                        _tankCollisionAngle = 0;
                    }

                    if ( _tankCollisionFlag & (COLL_WALL_L | COLL_HILL_L) )
                        _rotation = mat3x3::RotateY(-v236) * _rotation;
                    else
                        _rotation = mat3x3::RotateY(v236) * _rotation;
                }

                if ( _tankCollisionAngle == 0.0 )
                    _status_flg |= BACT_STFLAG_MOVE;
            }
            else
            {
                _status_flg |= BACT_STFLAG_MOVE;

                if ( v240 > 1.1 && _tankCollisionWay <= 0.001 )
                    _thraction *= 0.1 / (v244 + 0.1);

                if ( _rotation.AxisZ().XZ().cross( _target_dir.XZ() ) < 0.0 )
                    v240 = -v240;

                if ( v240 >= 0.0 )
                {
                    float v195 = _maxrot * v244;

                    if ( v240 <= v195 )
                    {
                        if ( v240 < 0.05 )
                            v240 = 0.0;
                    }
                    else
                    {
                        v240 = v195;
                    }
                }
                else
                {
                    float v218 = -_maxrot * v244;

                    if ( v240 >= v218 )
                    {
                        if ( v240 > -0.05 )
                            v240 = 0.0;
                    }
                    else
                    {
                        v240 = v218;
                    }
                }

                if ( _tankCollisionWay <= 0.0001 )
                    _rotation = mat3x3::RotateY(v240) * _rotation;
            }

            if ( _status_flg & BACT_STFLAG_MOVE )
            {
                _thraction += _force * v244 * 0.8;

                if ( _thraction > _force )
                    _thraction = _force;

                if ( !(_tankCollisionFlag & (COLL_WALL_L | COLL_WALL_R | COLL_HILL_L | COLL_HILL_R)) && 
                      _tankCollisionWay > 0.0 )
                    _thraction -= _force * v244 * 0.6;

                if ( _thraction < 0.0 )
                    _thraction = 0;

                if ( !(_status_flg & BACT_STFLAG_ATTACK) || !_tankExpectTgt || _tankCollisionFlag )
                {
                    move_msg arg74;
                    arg74.flag = 0;
                    arg74.field_0 = v244;

                    Move(&arg74);
                }

                if ( _tankCollisionWay > 0.0 )
                {
                    _tankCollisionWay -= ( _position.XZ() - _old_pos.XZ() ).length();

                    if ( _tankCollisionWay <= 0.0 )
                        _tankCollisionWay = 0;
                }
            }
            else
            {
                _old_pos = _position;
                _thraction = 0;
            }

            ypaworld_arg136 arg136;
            arg136.stPos = _old_pos;
            arg136.vect = _position - _old_pos;

            float v217 = arg136.vect.length();

            if ( v217 > 5.0 && _status_flg & BACT_STFLAG_MOVE )
                arg136.vect *= v224 / v217;
            else
                arg136.vect = _rotation.AxisZ() * v224;

            arg136.flags = 1;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect )
            {
                vec3d v55 = arg136.skel->polygons[ arg136.polyID ].Normal();

                if ( v55.y < 0.6 && _tankCollisionAngle == 0.0)
                {
                    if ( arg136.tVal * v224 > (_radius + 50.0) && fabs(v240) < 1.0 && fabs(0.0) < 0.7 && v220 >= 840.0 )
                    {
                        if ( !_secndTtype )
                        {
                            float v242 = (_fly_dir_length * 0.1) * arg->frameTime / 1000.0;

                            if ( _rotation.AxisZ().XZ().cross( v55.XZ() ) >= 0.0 )
                                _rotation = mat3x3::RotateY(-v242) * _rotation;
                            else
                                _rotation = mat3x3::RotateY(v242) * _rotation;

                            _tankCollisionWay = arg136.tVal * v224 + 10.0;
                        }
                    }

                    if ( arg136.tVal * v224 <= (_radius + 50.0) )
                    {
                        _status_flg &= ~BACT_STFLAG_MOVE;
                        _position = _old_pos;

                        ypaworld_arg136 arg136_2;
                        arg136_2.stPos = _position;
                        arg136_2.vect = _rotation.AxisY() * 300.0;
                        arg136_2.flags = 1;

                        _world->ypaworld_func136(&arg136_2);

                        vec3d v187;

                        if ( arg136_2.isect )
                            v187 = v55 * arg136_2.skel->polygons[ arg136_2.polyID ].Normal();
                        else
                            v187 = vec3d(-v55.z, 0.0, v55.x);

                        if ( v187 == vec3d(0.0, 0.0, 0.0) )
                            v187 = _rotation.AxisX();

                        _tankCollisionVector = _rotation.AxisY() * v187;

                        if ( _rotation.AxisZ().dot(_tankCollisionVector) < 0.0 )
                            _tankCollisionVector = -_tankCollisionVector;

                        float tmpLn = _tankCollisionVector.XZ().length();

                        NDIV_CARRY(tmpLn);

                        ypaworld_arg136 arg136_1;
                        ypaworld_arg136 arg136_3;

                        arg136_1.vect.x = _tankCollisionVector.z * 150.0 / tmpLn;
                        arg136_1.vect.y = 0;
                        arg136_1.vect.z = -_tankCollisionVector.x * 150.0 / tmpLn;
                        arg136_1.stPos = _old_pos;
                        arg136_1.flags = 1;

                        arg136_3.vect.x = -_tankCollisionVector.z * 150.0 / tmpLn;
                        arg136_3.vect.y = 0;
                        arg136_3.vect.z = _tankCollisionVector.x * 150.0 / tmpLn;
                        arg136_3.stPos = _old_pos;
                        arg136_3.flags = 1;

                        _world->ypaworld_func136(&arg136_1);
                        _world->ypaworld_func136(&arg136_3);

                        bool v86 = arg136_1.isect && arg136_1.skel->polygons[ arg136_1.polyID ].B < 0.6;
                        bool v87 = arg136_3.isect && arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6;

                        bool wallLeft = false;

                        vec2d az2d = _rotation.AxisZ().XZ();

                        if ( (v87 && !v86) || (v86 && !v87) )
                        {
                            if ( v87 )
                            {
                                float dotLen = arg136_1.vect.XZ().dot( az2d );

                                tmpLn = az2d.length();

                                NDIV_CARRY(tmpLn);

                                dotLen = dotLen / tmpLn / 150.0;

                                _tankCollisionAngle = clp_acos(dotLen);
                                wallLeft = true;
                            }
                            else
                            {
                                float dotLen = az2d.dot( arg136_3.vect.XZ() );

                                tmpLn = az2d.length();

                                NDIV_CARRY(tmpLn);

                                dotLen = dotLen / tmpLn / 150.0;

                                _tankCollisionAngle = clp_acos(dotLen);
                                wallLeft = false;
                            }
                        }
                        else
                        {
                            float dotLen = az2d.dot( _tankCollisionVector.XZ() );

                            tmpLn = az2d.length();

                            NDIV_CARRY(tmpLn);

                            dotLen = dotLen / tmpLn;

                            tmpLn = _tankCollisionVector.XZ().length();

                            NDIV_CARRY(tmpLn);

                            dotLen = dotLen / tmpLn;

                            _tankCollisionAngle = C_PI_2 - clp_acos(dotLen) + 0.01;

                            if ( az2d.cross( _tankCollisionVector.XZ() ) <= 0.0 )
                                wallLeft = false;
                            else
                                wallLeft = true;
                        }

                        if ( (_tankCollisionFlag & COLL_WALL_L) && !wallLeft )
                        {
                            _tankCollisionAngle = C_PI - _tankCollisionAngle;
                        }
                        else if ( (_tankCollisionFlag & COLL_WALL_R) && wallLeft )
                        {
                            _tankCollisionAngle = C_PI - _tankCollisionAngle;
                        }
                        else if ( wallLeft )
                        {
                            _tankCollisionFlag |= COLL_WALL_L;
                        }
                        else
                        {
                            _tankCollisionFlag |= COLL_WALL_R;
                        }

                        _tankCollisionWay = 100.0;
                    }
                }
            }

            vec3d alignNorm;
            int alignRes = AlignVehicleAI(v244, &alignNorm);
            if ( alignRes != ALIGN_DONE )
            {
                _fly_dir_length = 0;
                _thraction = 0;

                arg136.isect = 0;

                if ( alignRes == ALIGN_NONE )
                {
                    arg136.stPos = _position + _rotation.AxisZ() * _radius;

                    float v223 = _radius * 8.0;
                    arg136.vect = (_rotation.AxisY() - _rotation.AxisZ()) * v223;
                    arg136.flags = 1;

                    _world->ypaworld_func136(&arg136);

                    if ( !arg136.isect || ( arg136.isect && arg136.tVal * v223 * 0.7 > (_overeof + 30.0) ) )
                    {
                        _status_flg &= ~BACT_STFLAG_LAND;
                        break;
                    }
                }

                _position = _old_pos;

                if ( (arg136.isect && arg136.skel->polygons[ arg136.polyID ].B < 0.6) || alignRes == ALIGN_NORMAL )
                {
                    _status_flg &= ~BACT_STFLAG_MOVE;
                    _tankCollisionFlag &= ~(COLL_WALL_L | COLL_WALL_R);

                    vec3d v208(0.0, 0.0, 0.0);

                    if ( arg136.isect )
                        v208 = -arg136.skel->polygons[ arg136.polyID ].Normal();

                    ypaworld_arg136 arg136_4;

                    arg136_4.stPos = _position;
                    arg136_4.vect = _rotation.AxisY() * 300.0;
                    arg136_4.flags = 1;

                    _world->ypaworld_func136(&arg136_4);

                    if ( alignRes == ALIGN_NORMAL )
                        v208 = -alignNorm;

                    vec3d v184;

                    if ( arg136_4.isect )
                        v184 = v208 * arg136_4.skel->polygons[arg136_4.polyID].Normal();
                    else
                        v184 = vec3d(-v208.z, 0.0, v208.x);

                    _tankCollisionVector = _rotation.AxisY() * v184;

                    if ( _tankCollisionVector.dot( _rotation.AxisZ() ) < 0.0 )
                        _tankCollisionVector = -_tankCollisionVector;

                    vec2d axsZ = _rotation.AxisZ().XZ();

                    float dotLen = axsZ.dot( _tankCollisionVector.XZ() );

                    float ln = axsZ.length();

                    NDIV_CARRY(ln);

                    dotLen = dotLen / ln;

                    ln = _tankCollisionVector.XZ().length();

                    NDIV_CARRY(ln);

                    dotLen = dotLen / ln;

                    _tankCollisionAngle = C_PI_2 - clp_acos(dotLen);

                    if ( _tankCollisionAngle < 0.1 )
                        _tankCollisionAngle = 0.1;

                    bool collHillLeft = axsZ.cross( _tankCollisionVector.XZ() ) > 0.0;

                    if ( (_tankCollisionFlag & COLL_HILL_L) && !collHillLeft )
                    {
                        _tankCollisionAngle = C_PI - _tankCollisionAngle;
                    }
                    else if ( (_tankCollisionFlag & COLL_HILL_R) && collHillLeft )
                    {
                        _tankCollisionAngle = C_PI - _tankCollisionAngle;
                    }
                    else if ( collHillLeft )
                    {
                        _tankCollisionFlag |= COLL_WALL_L;
                    }
                    else
                    {
                        _tankCollisionFlag |= COLL_WALL_R;
                    }

                    _tankCollisionWay = 100.0;
                }
            }

            if ( v194 && ( v212 || v215 ) )
            {
                if ( CollisionWithBact(arg->frameTime) )
                    break;
            }
        }

        bact_arg75 arg75;
        arg75.fperiod = v244;
        arg75.g_time = _clock;

        if ( _secndTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.prio = 1;
            arg75.target.pbact = _secndT.pbact;

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
                arg78.newStatus = BACT_STATUS_NOPE;
                arg78.setFlags = 0;
                arg78.unsetFlags = BACT_STFLAG_FIRE;

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

            int v150 = TargetAssess(&arg110);
            int v153 = TargetAssess(&arg110_1);

            if ( v150 != TA_IGNORE || v153 != TA_IGNORE )
            {
                if ( v150 == TA_CANCEL )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( v153 == TA_CANCEL)
                {
                    setTarget_msg arg67;
                    arg67.priority = 0;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = _position.x;
                    arg67.tgt_pos.z = _position.z;

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

        setState_msg arg78;
        arg78.unsetFlags = 0;
        arg78.setFlags = 0;
        arg78.newStatus = BACT_STATUS_IDLE;

        SetState(&arg78);

        _thraction = 0;

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            _fly_dir_length = 0;

            if ( _tankWaitCount )
            {
                if ( _status_flg & BACT_STFLAG_DODGE_LEFT )
                    _rotation = mat3x3::RotateY(v244 * 0.33333334) * _rotation;
                else if ( _status_flg & BACT_STFLAG_DODGE_RIGHT )
                    _rotation = mat3x3::RotateY(-v244 * 0.33333334) * _rotation;

                _tankWaitCount--;
            }
            else
            {
                _status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT);
                _tankWaitCount = arg->frameTime / 5;

                if ( _tankFlags & FLAG_TANK_ROTWAIT )
                {
                    switch( ((size_t)this + _clock) % 7 ) //WAT!?
                    {
                        case 0:
                            _status_flg |= BACT_STFLAG_DODGE_LEFT;
                            break;
                        case 1:
                            _status_flg |= BACT_STFLAG_DODGE_RIGHT;
                            break;
                        default:
                            break;
                    }
                }
            }
            if ( AlignVehicleAI(v244, NULL) != ALIGN_DONE )
                _status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->frameTime;

            CrashOrLand(&arg86);
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

void NC_STACK_ypatank::User_layer(update_msg *arg)
{
    _airconst = _airconst_static;

    int a4 = getBACT_bactCollisions();

    float v90 = arg->frameTime * 0.001;
    vec3d oldDir = _rotation.AxisZ();

    if (_status == BACT_STATUS_NORMAL || _status == BACT_STATUS_IDLE)
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
        }
        else
        {

            if ( _primTtype != BACT_TGT_TYPE_CELL || (_primTpos.XZ() - _position.XZ()).length() <= 800.0 )
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }

                _status = BACT_STATUS_NORMAL;
            }
            else
            {
                if ( !(_status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }
                else
                    _status = BACT_STATUS_IDLE;
            }
        }

        float v85 = -arg->inpt->Sliders[3] * _maxrot * v90 * 2.0;

        if ( fabs(v85) > 0.0 )
            _rotation = mat3x3::RotateY(v85) * _rotation;

        if ( (_fly_dir_length < 0.0 && _thraction > 0.0) || (_fly_dir_length > 0.0 && _thraction < 0.0) )
        {
            if ( fabs(_fly_dir.y) > 0.1 )
            {
                float v84 = 1.0 - v90 * 4.0;

                if ( v84 < 0.1 )
                    v84 = 0.1;

                _fly_dir_length *= v84;
            }
        }

        float v88 = arg->inpt->Sliders[4];
        float v75 = fabs(v88);

        if ( v88 > 1.0 )
            v88 = 1.0;
        else if ( v88 < -1.0 )
            v88 = -1.0;

        _thraction += _force * (v90 * 0.75) * v88;

        float v78;

        if ( arg->inpt->Buttons.Is(31) )
            v78 = _force * v75;
        else
            v78 = _force;

        if ( _thraction > v78 )
            _thraction = v78;
        if ( _thraction < -v78 )
            _thraction = -v78;

        if ( fabs(v88) > 0.001 )
            _status_flg |= BACT_STFLAG_MOVE;

        _gun_angle_user += v90 * arg->inpt->Sliders[5];

        if ( _gun_angle_user < -0.3 )
            _gun_angle_user = -0.3;

        if ( _gun_angle_user > 0.8 )
            _gun_angle_user = 0.8;


        _gun_leftright = _gun_leftright - v90 * arg->inpt->Sliders[15];

        if ( _gun_leftright < -0.8 )
            _gun_leftright = -0.8;

        if ( _gun_leftright > 0.8 )
            _gun_leftright = 0.8;

        vec3d v67;
        v67 = _rotation.AxisZ();

        float corW, corH;
        GFXEngine::GFXe.getC3D()->getAspectCorrection(corW, corH, true);

        v67 -= _rotation.AxisY() * (_gun_angle_user * corH);
        v67 -= _rotation.AxisX() * (_gun_leftright * corW);

        float v89 = v67.length();

        if ( v89 > 0.0001 )
            v67 /= v89;

        bact_arg79 arg79;
        bact_arg106 arg106;

        arg79.tgt_pos = v67;
        arg79.tgType = BACT_TGT_TYPE_DRCT;

        arg106.field_4 = v67;
        arg106.field_0 = 5;

        if ( UserTargeting(&arg106) )
        {
            arg79.target.pbact = arg106.ret_bact;
            arg79.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->Buttons.IsAny({0, 1}) )
        {
            arg79.direction = v67;
            arg79.weapon = _weapon;
            arg79.g_time = _clock;

            if ( _clock % 2 )
                arg79.start_point.x = -_fire_pos.x;
            else
                arg79.start_point.x = _fire_pos.x;

            arg79.start_point.y = _fire_pos.y;
            arg79.start_point.z = _fire_pos.z;
            arg79.flags = (arg->inpt->Buttons.Is(1) ? 1 : 0);
            arg79.flags |= 2;

            LaunchMissile(&arg79);
        }

        if ( _mgun != -1 )
        {
            if ( _status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->Buttons.Is(2)) )
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
                arg105.field_0 = v67;
                arg105.field_C = v90;
                arg105.field_10 = _clock;

                FireMinigun(&arg105);
            }

        }

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;

            if ( arg->inpt->Buttons.Is(3) )
            {
                _thraction = 0;

                if ( fabs(_fly_dir_length) >= 4.0 )
                {
                    float v46 = 1.0 - v90 * 4.0;

                    if ( v46 < 0.1 )
                        v46 = 0.1;

                    _fly_dir_length *= v46;
                }
                else
                {
                    _status_flg &= ~BACT_STFLAG_MOVE;
                    _fly_dir_length = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.field_0 = v90;

            if ( _status_flg & BACT_STFLAG_MOVE )
                Move(&arg74);

            ypaworld_arg136 arg136;

            arg136.stPos = _old_pos;
            arg136.vect = _position - _old_pos;
            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( (a4 && CollisionWithBact(arg->frameTime)) || !arg136.isect ) // Check it
            {
                AlignVehicleUser(v90, oldDir);
            }
            else
            {
                _position = _old_pos;
                _fly_dir_length = 0;
                _thraction = 0;
            }
        }
        else
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->frameTime;

            CrashOrLand(&arg86);
        }
    }
    else if (_status == BACT_STATUS_DEAD)
    {
        DeadTimeUpdate(arg);
    }
}

void NC_STACK_ypatank::Move(move_msg *arg)
{
    _old_pos = _position;

    float v50;

    if ( arg->flag & 2 && _status_flg & BACT_STFLAG_LAND )
    {
        v50 = 0.0;
    }
    else
    {
        if ( _status == BACT_STATUS_DEAD )
            v50 = _mass * 39.2266;
        else
            v50 = _mass * 9.80665;
    }

    vec3d v47;

    if ( arg->flag & 1 )
        v47 = vec3d::OY(v50 * 2.0);
    else
        v47 = _rotation.AxisZ() * _thraction;

    vec3d v40 = vec3d::OY(v50) + v47 - _fly_dir * (_fly_dir_length * _airconst);

    float v45 = v40.length();

    if ( v45 > 0.0 )
    {
        vec3d v37 = _fly_dir * _fly_dir_length + v40 * ( arg->field_0 / _mass );

        float v42 = v37.length();

        if ( v42 > 0.0 )
            v37 /= v42;

        if ( _status_flg & BACT_STFLAG_LAND )
        {
            _fly_dir = _rotation.AxisZ();

            _fly_dir_length = v37.dot( _fly_dir ) * v42;
        }
        else
        {
            _fly_dir = v37;
            _fly_dir_length = v42;
        }
    }

    _position += _fly_dir * (_fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);

    _soundcarrier.samples_data[0].pitch = _pitch;
    _soundcarrier.samples_data[0].volume = _volume;

    float v48 = fabs(_fly_dir_length) / (_force / _airconst_static);
    float v46;

    if ( _pitch_max <= -0.8 )
        v46 = 1.1;
    else
        v46 = _pitch_max;

    float v49 = v46 * v48;

    if ( v49 > v46 )
        v49 = v46;

    if ( _soundcarrier.samples_data[0].psampl )
        _soundcarrier.samples_data[0].pitch += (float)(_soundcarrier.samples_data[0].psampl->SampleRate + _soundcarrier.samples_data[0].pitch) * v49;
}

size_t NC_STACK_ypatank::SetPosition(bact_arg80 *arg)
{
    if ( NC_STACK_ypabact::SetPosition(arg) )
    {
        if ( arg->field_C & 1 )
        {
            ypaworld_arg136 arg136;
            arg136.stPos = _position.X0Z() - vec3d::OY(30000.0);
            arg136.vect = vec3d::OY(50000.0);
            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( !arg136.isect )
                return 0;

            int a4 = getBACT_viewer();

            if ( a4 )
                _position.y = arg136.isectPos.y - _viewer_overeof;
            else
                _position.y = arg136.isectPos.y - _overeof;

            _status_flg |= BACT_STFLAG_LAND;
        }
        return 1;
    }
    return 0;
}

void NC_STACK_ypatank::ApplyImpulse(bact_arg83 *arg)
{
    float v30 = 500.0 / _mass;

    vec3d v27 = _position - arg->pos;

    float v31 = arg->energ * 0.0004;

    float v32 = v27.length();

    if ( v32 <= 1.0 )
    {
        v27 = arg->pos2;
        v32 = 1.0;
    }
    else
        v27 /= v32;

    v27 *= v31 * 10.0 * v30 / v32;

    vec3d v24 = _position + vec3d(v27.x, -v27.y, v27.z);

    ypaworld_arg136 arg136;
    arg136.stPos = _position;
    arg136.vect = v24 - _position;
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( !arg136.isect )
        _position = v24;
}

void NC_STACK_ypatank::ypatank_func87__sub0(NC_STACK_ypabact *bact2)
{
    int v23 = (float)bact2->_energy_max * 0.7;

    if ( v23 < 10000 )
        v23 = 10000;

    if ( v23 > 25000 )
        v23 = 25000;

    float v3 = (float)bact2->_scale_time * 0.2 / (float)v23 * (float)bact2->_energy_max;

    if ( v3 + _energy > _energy_max )
    {
        NC_STACK_yparobo *robo = _host_station;

        _energy = _energy_max;

        int v10 = (int)v3 - (_energy_max - _energy);

        if ( v10 + robo->_energy > robo->_energy_max )
        {
            int v13 = robo->_energy;

            robo->_energy = robo->_energy_max;
            int v14 = v10 - (robo->_energy_max - v13);
            int v16 = v14 + robo->_roboEnergyLife;

            if ( v16 >= robo->_energy_max )
            {
                int v17 = robo->_roboEnergyLife;
                robo->_roboEnergyLife = robo->_energy_max;

                robo->_roboEnergyMove += v14 - (robo->_energy_max - v17);

                if ( robo->_roboEnergyMove > robo->_energy_max )
                    robo->_roboEnergyMove = robo->_energy_max;
            }
            else
            {
                robo->_roboEnergyLife = v16;
            }
        }
        else
        {
            robo->_energy = v10 + robo->_energy;
        }
    }
    else
    {
        _energy = v3 + _energy;
    }
}

size_t NC_STACK_ypatank::CollisionWithBact(int arg)
{
    int v105 = 0;
    int v108 = 0;

    float v121 = 0.0;
    int v103 = 0;

    NC_STACK_ypabact *v80[10];
    vec3d v78[10];

    if ( _status_flg & BACT_STFLAG_LAND )
    {
        int a4 = getBACT_inputting();

        float v104;

        if ( a4 )
            v104 = 0.6;
        else
            v104 = 0.82;

        yw_130arg arg130;
        arg130.pos_x = _position.x;
        arg130.pos_z = _position.z;

        int v113 = 0;
        int v106 = 0;

        NC_STACK_ypabact *v117 = NULL;

        float v99 = 300.0;
        float v124 = arg * 0.001;

        if ( !_world->ypaworld_func130(&arg130) )
            return 0;

        float v109 = 1200.0;

        for ( NC_STACK_ypabact* &v12 : arg130.pcell->unitsList )
        {
            int v114 = v12->_status == BACT_STATUS_DEAD   &&   v12->_vp_extra[0].flags & EVPROTO_FLAG_ACTIVE   &&  a4 &&   v12->_scale_time > 0;

            if ( v12->_bact_type != BACT_TYPES_MISSLE
                    && (!v12->IsDestroyed() || v114)
                    && v12 != this )
            {

                rbcolls *v96 = v12->getBACT_collNodes();

                int v110;

                if ( v96 )
                    v110 = v96->robo_coll_num;
                else
                    v110 = 1;

                for (int j = v110 - 1; j >= 0; j--)
                {
                    vec3d v89;
                    float v19;

                    if ( !v96 )
                    {
                        v89 = v12->_position;
                        v19 = v12->_radius;
                    }
                    else
                    {
                        roboColl &v15 = v96->roboColls[j];

                        v89 = v12->_position + v12->_rotation.Transpose().Transform(v15.coll_pos);
                        v19 = v15.robo_coll_radius;
                    }

                    if (!v96 || v19 >= 0.01 )
                    {
                        vec3d v93 = v89 - _position;

                        float v126 = v93.length();

                        if ( v126 < 300.0 )
                        {
                            float v115 = _radius + v19;

                            if ( v114 && v126 < v115 )
                            {
                                ypatank_func87__sub0(v12);

                                v12->_scale_time = -1;

                                if ( _world->GameShell )
                                    SFXEngine::SFXe.startSound(&_world->GameShell->samples2_info, 4);

                                if ( _world->isNetGame )
                                {
                                    uamessage_endPlasma eplMsg;
                                    eplMsg.msgID = UAMSG_ENDPLASMA;
                                    eplMsg.owner = v12->_owner;
                                    eplMsg.id = v12->_gid;

                                    yw_arg181 arg181;
                                    arg181.recvID = 0;
                                    arg181.dataSize = sizeof(eplMsg);
                                    arg181.recvFlags = 2;
                                    arg181.garant = 1;
                                    arg181.data = &eplMsg;

                                    _world->ypaworld_func181(&arg181);

                                    if ( _owner != v12->_owner )
                                    {
                                        v12->_vp_extra[0].flags = 0;
                                        v12->_vp_extra[0].vp.trigo = NULL;
                                        v12->_vp_extra[0].vp.base = NULL;
                                    }
                                }
                                break;
                            }
                            else
                            {

                                float v118 = v93.dot( _rotation.AxisZ() );

                                if ( v126 > 0.01 )
                                    v118 = v118 / v126;

                                if ( fabs(_fly_dir_length) >= 0.1 )
                                {
                                    if ( _fly_dir_length < 0.0 )
                                    {
                                        v118 = -v118;
                                    }
                                }
                                else if ( _thraction < 0.0 )
                                {
                                    v118 = -v118;
                                }

                                if ( v118 >= v104 && v126 < v115 )
                                {
                                    if ( !v117 || v126 < v109 )
                                    {
                                        v109 = v126;
                                        v117 = v12;

                                        v121 = clp_acos(v118);

                                        float v30 = v93.XZ().cross( _rotation.AxisZ().XZ() );
                                        v103 = v30 > 0.0;

                                        if ( v126 < v115 )
                                            v106 = 1;

                                        if ( _rotation.AxisZ().dot( v117->_rotation.AxisZ() ) < 0.0 )
                                            v108 = 1;

                                        if ( v117->_status == BACT_STATUS_IDLE )
                                            v108 = 1;

                                        if ( v117->_waitCol_time <= 0 )
                                            v105 = 1;
                                    }

                                    if ( a4 )
                                    {
                                        if ( !v96 && v113 < 10 )
                                        {
                                            v80[v113] = v12;

                                            if ( v126 <= 0.0001 )
                                                v78[v113] = v93;
                                            else
                                                v78[v113] = v93 / v126;
                                            v113++;
                                        }
                                    }
                                }

                            }
                        }
                    }



                }


            }
        }


        if ( !v117 )
        {
            _status_flg &= ~BACT_STFLAG_BCRASH;
            return 0;
        }

        if ( a4 )
        {
            if ( v106 && v113 )
            {
                vec3d v88(0.0, 0.0, 0.0);

                for (int i = 0; i < v113; i++)
                {
                    if ( !_world->isNetGame || _owner == v80[i]->_owner )
                    {
                        float v40 = _mass * 8.0 * fabs(_fly_dir_length);
                        float v41 = _thraction * v124 * 100.0;

                        float v127 = v124 * ((v40 + v41) / v80[i]->_mass);

                        if ( v127 > 0.05 )
                        {
                            vec3d v85 = v78[i] * v127 + v80[i]->_position;

                            ypaworld_arg136 arg136;
                            arg136.stPos = v80[i]->_position;
                            arg136.vect = v85 - v80[i]->_position;
                            arg136.flags = 0;

                            _world->ypaworld_func136(&arg136);

                            if ( !arg136.isect )
                            {

                                v80[i]->_old_pos = v80[i]->_position;

                                v80[i]->_position = v85;

                                v80[i]->_status_flg &= ~BACT_STFLAG_LAND;

                                v80[i]->CorrectPositionInLevelBox(NULL);
                            }
                        }

                        v88 += v78[i];
                    }
                }

                v88 /= (float)v113;

                vec3d v90 = _rotation.AxisZ();

                if ( _fly_dir_length < 0.0 )
                    v90 = -v90;

                float v125 = v90.XZ().dot( v88.XZ() );

                float v120 = v90.XZ().length();

                if ( v120 > 0.001 )
                    v125 = v125 / v120;

                float v122 = v88.XZ().length();

                if ( v122 > 0.001 )
                    v125 = v125 / v122;

                if ( v125 >= 0.95 )
                {
                    _position = _old_pos;
                    _fly_dir_length = 0;
                }
                else
                {
                    if ( v88.XZ().cross( v90.XZ() ) >= 0.0 )
                    {
                        int v61 = v88.x;
                        v88.x = -v88.z;
                        v88.z = v61;
                    }
                    else
                    {
                        int v60 = v88.x;
                        v88.x = v88.z;
                        v88.z = -v60;
                    }

                    float v62 = fabs(_fly_dir_length) * v124 * 6.0;

                    _position = v88 * v62 + _old_pos;
                }

                if ( !(_status_flg & BACT_STFLAG_BCRASH) )
                {
                    SFXEngine::SFXe.startSound(&_soundcarrier, 6);
                    _status_flg |= BACT_STFLAG_BCRASH;
                }

                yw_arg180 arg180;

                arg180.field_4 = 1.0;
                arg180.field_8 = v88.x;
                arg180.field_C = v88.z;
                arg180.effects_type = 5;

                _world->ypaworld_func180(&arg180);
            }
            else
            {
                _status_flg &= ~BACT_STFLAG_BCRASH;
            }
        }
        else
        {
            if ( v106 )
            {
                _fly_dir_length = 0;
                _thraction = 0;

                _position = _old_pos;
                _status_flg &= ~BACT_STFLAG_MOVE;

                if ( !(_status_flg & BACT_STFLAG_BCRASH) )
                    _status_flg |= BACT_STFLAG_BCRASH;

                if ( v108 || !v105 )
                {
                    if ( !(_tankCollisionFlag & (COLL_HILL_L | COLL_HILL_R)) )
                    {
                        if ( _tankCollisionFlag & (COLL_WALL_L | COLL_WALL_R) )
                        {
                            _tankCollisionWay = 100.0;
                            _tankCollisionAngle = 1.5;
                        }
                        else
                        {
                            if ( v103 )
                                _tankCollisionFlag |= COLL_WALL_R;
                            else
                                _tankCollisionFlag |= COLL_WALL_L;

                            _tankCollisionWay = 80.0;
                            _tankCollisionAngle = 1.5 - v121;
                        }
                    }
                }
                else
                {
                    _waitCol_time = 1000;
                }
            }
            else
            {
                _fly_dir_length *= v109 / v99;
                _status_flg &= ~BACT_STFLAG_BCRASH;
            }
        }

        if ( a4 && v106 )
            return 1;
        return 0;

    }

    return NC_STACK_ypabact::CollisionWithBact(arg);
}

void NC_STACK_ypatank::Recoil(bact_arg88 *arg)
{
    if ( !(_status_flg & BACT_STFLAG_LAND) )
    {
        NC_STACK_ypabact::Recoil(arg);

        _fly_dir = _fly_dir * _fly_dir_length - arg->pos1 * 5.0;

        _fly_dir_length = _fly_dir.length();

        if ( _fly_dir_length > 0.001 )
            _fly_dir /= _fly_dir_length;
    }
}

void NC_STACK_ypatank::Renew()
{
    NC_STACK_ypabact::Renew();

    _tankWaitCount = 0;
}

size_t NC_STACK_ypatank::CheckFireAI(bact_arg101 *arg)
{
    vec3d v34 = arg->pos - _position;

    float v37 = v34.length();

    if ( v37 == 0.0 )
        return 0;

    v34 /= v37;

    WeapProto *a4 = _world->getYW_weaponProtos();

    _tankExpectTgt = true;

    ypaworld_arg136 arg149;

    arg149.stPos = _position;
    arg149.vect = arg->pos - arg149.stPos;
    arg149.flags = 0;

    _world->ypaworld_func149(&arg149);

    if ( arg149.isect )
    {
        if ( arg->unkn == 2 )
        {
            NC_STACK_ypabact *v15;

            if ( _secndTtype == BACT_TGT_TYPE_UNIT )
                v15 = _secndT.pbact;
            else
                v15 = _primT.pbact;

            if ( v15 )
            {
                if ( v15->_bact_type == BACT_TYPES_TANK || v15->_bact_type == BACT_TYPES_CAR || v15->_bact_type == BACT_TYPES_GUN )
                    _tankExpectTgt = false;
            }
        }
        else
        {
            yw_130arg arg130;
            yw_130arg arg130_1;

            arg130.pos_x = arg->pos.x;
            arg130.pos_z = arg->pos.z;

            arg130_1.pos_x = arg149.isectPos.x;
            arg130_1.pos_z = arg149.isectPos.z;

            if ( _world->ypaworld_func130(&arg130) )
            {
                if ( _world->ypaworld_func130(&arg130_1) )
                {
                    if ( arg130.pcell != arg130_1.pcell )
                    {
                        _tankExpectTgt = false;
                    }
                    else
                    {
                        if ( arg130.pcell->comp_type != 1 )
                        {
                            int v17 = (arg130.pos_x + -150.0) / 300.0;
                            int v18 = (arg130.pos_z + 150.0) / 300.0;

                            int v19 = (arg130_1.pos_x + -150.0) / 300.0;
                            int v20 = (arg130_1.pos_z + 150.0) / 300.0;

                            if ( v17 != v19 || v20 != v18 )
                            {
                                _tankExpectTgt = false;
                            }
                        }
                    }
                }
            }
        }
    }

    WeapProto *v22 = NULL;
    int v43 = 0;

    if ( _weapon == -1 )
    {
        v22 = NULL;
    }
    else
    {
        v22 = &a4[ _weapon ];

        if ( v22->model_id & 1 )
            v43 = v22->model_id & 0xFE;
        else
            v22 = NULL;
    }

    if ( !v22 )
    {
        if ( _mgun == -1 )
            return 0;
        v43 = 2;
    }

    float v38 = 0.0;

    if ( v34.XZ() != vec2d(0.0, 0.0) )
    {
        v38 = v34.XZ().dot( _rotation.AxisZ().XZ() );

        float tmpsq = v34.XZ().length();

        NDIV_CARRY(tmpsq);

        v38 = v38 / tmpsq;

        tmpsq = _rotation.AxisZ().XZ().length();

        NDIV_CARRY(tmpsq);

        v38 = v38 / tmpsq;
    }
    else
    {
        v38 = 0.0;
    }

    if ( arg->unkn == 2 )
    {
        if ( v43 )
        {
            if ( v37 < 1200.0 && v38 > 0.93 )
            {
                if ( v34.y > -0.85 && v34.y < 0.2 )
                    return 1;
            }
        }
        else if ( v34.y > 0.98 )
        {
            return 1;
        }
    }
    else if ( v22 )
    {
        if ( v43 )
        {
            if ( v37 < 1200.0 && v38 > 0.91 )
            {
                if ( v34.y > -0.4 && v34.y < 0.3 )
                    return 1;
            }
        }
        else if ( v34.y > 0.92 )
        {
            return 1;
        }
    }
    return 0;
}

size_t NC_STACK_ypatank::TestTargetSector(NC_STACK_ypabact *cel_unit)
{
    cellArea *bactPcell = _pSector;
    cellArea *cunitPcell = cel_unit->_pSector;

    if ( cel_unit->_sectX == _sectX || cel_unit->_sectY == _sectY )
    {
        if ( fabs(bactPcell->height - cunitPcell->height) >= 500.0 )
            return 0;
    }
    else
    {
        yw_130arg arg130;
        arg130.pos_x = _position.x;
        arg130.pos_z = cel_unit->_position.z;

        _world->ypaworld_func130(&arg130);

        cellArea *arg130_Pcell =arg130.pcell;
        arg130.pos_x = cel_unit->_position.x;
        arg130.pos_z = _position.z;

        _world->ypaworld_func130(&arg130);

        if ( fabs(bactPcell->height    - arg130_Pcell->height) >= 500.0 && fabs(arg130.pcell->height -   cunitPcell->height) >= 500.0 )
            return 0;
        if ( fabs(bactPcell->height    - arg130.pcell->height) >= 500.0 && fabs(arg130_Pcell->height -   cunitPcell->height) >= 500.0 )
            return 0;
        if ( fabs(bactPcell->height    - arg130_Pcell->height) >= 500.0 && fabs(bactPcell->height    - arg130.pcell->height) >= 500.0 )
            return 0;
        if ( fabs(arg130_Pcell->height -   cunitPcell->height) >= 500.0 && fabs(arg130.pcell->height -   cunitPcell->height) >= 500.0 )
            return 0;
    }
    return 1;
}

void NC_STACK_ypatank::CorrectPositionOnLand()
{
    NC_STACK_ypabact::CorrectPositionOnLand();

    if ( _status_flg & BACT_STFLAG_LAND )
    {
        int a4 = getBACT_viewer();

        float v6;

        if ( a4 )
            v6 = _viewer_overeof;
        else
            v6 = _overeof;

        ypaworld_arg136 arg136;
        arg136.stPos = _position;
        arg136.vect = vec3d::OY( v6 * 2.0 );
        arg136.flags = 0;

        _world->ypaworld_func136(&arg136);

        if ( arg136.isect )
            _position.y = arg136.isectPos.y - v6;
        else
            _status_flg &= ~BACT_STFLAG_LAND;
    }
}

vec3d NC_STACK_ypatank::TankTip(vec3d v)
{
    float force = _thraction - _airconst * _fly_dir_length;

    if ( fabs(force) > 3.0 )
    {
        float angle = 0.2 * (force / _force);

        v = mat3x3::AxisAngle(_rotation.AxisX(), -angle).Transform(v);
    }
    return v;
}

int NC_STACK_ypatank::AlignVehicleAI(float dtime, vec3d *pNormal)
{
    int a4 = getBACT_viewer();

    float v5;

    if ( a4 )
        v5 = _viewer_overeof;
    else
        v5 = _overeof;

    ypaworld_arg136 arg136;
    arg136.stPos = _rotation.AxisY() * v5 + _position - vec3d::OY(50.0);
    arg136.vect = vec3d::OY( _overeof + 110.0 );
    arg136.flags = 0;

    _world->ypaworld_func136(&arg136);

    if ( !arg136.isect )
        return ALIGN_NONE;
    
    vec3d normal = arg136.skel->polygons[ arg136.polyID ].Normal();

    if ( normal.y < 0.6 )
    {
        if (pNormal)
            *pNormal = normal;
        return ALIGN_NORMAL;
    }

    if ( _tankFlags & FLAG_TANK_TIP )
        normal = TankTip(normal);

    vec3d vaxis = _rotation.AxisY() * normal;

    float v46 = vaxis.length();

    if ( v46 > 0.0 )
    {
        float v57 = clp_acos( normal.dot( _rotation.AxisY() ) );

        float v47 = _maxrot * 2.0 * dtime;

        if ( v57 > v47 )
            v57 = v47;

        if ( fabs(v57) < 0.01 )
            v57 = 0.0;

        vaxis /= v46;

        _rotation *= mat3x3::AxisAngle(vaxis, v57);
    }

    _position = arg136.isectPos - _rotation.AxisY() * v5;

    return ALIGN_DONE;
}

int NC_STACK_ypatank::AlignVehicleUser(float dtime, const vec3d &oldDir)
{
    float v152 = 1.73;
    float v155 = 1.7;
    int v149 = 0;
    int v151 = 0;
    int v148 = 0;

    int a4 = getBACT_viewer();

    int v143 = getBACT_inputting();

    float v5;
    if ( a4 )
        v5 = _viewer_overeof;
    else
        v5 = _overeof;

    vec3d v133;
    vec3d v123;
    vec3d v122;
    float v156;

    if ( _fly_dir_length < 0.0 )
    {
        v123 = _position + (-_rotation.AxisZ() - _rotation.AxisX()) * _viewer_radius * 0.7071;

        v133 = _position + (_rotation.AxisX() - _rotation.AxisZ()) * _viewer_radius * 0.7071;

        v122 = _position + _rotation.AxisZ() * _viewer_radius;

        v156 = -1.0;
    }
    else
    {
        v123 = _position + (_rotation.AxisZ() - _rotation.AxisX()) * _viewer_radius * 0.7071;

        v133 = _position + (_rotation.AxisZ() + _rotation.AxisX()) * _viewer_radius * 0.7071;

        v122 = _position - _rotation.AxisZ() * _viewer_radius;

        v156 = 1.0;
    }

    v122.y += _viewer_overeof;
    v123.y += _viewer_overeof;
    v133.y += _viewer_overeof;

    ypaworld_arg136 arg136;
    ypaworld_arg136 arg136_1;
    ypaworld_arg136 arg136_2;

    arg136.stPos = _position - vec3d::OY(v152 * _viewer_radius);
    arg136_1.stPos = _position - vec3d::OY(v152 * _viewer_radius);
    arg136_2.stPos = _position - vec3d::OY(v152 * _viewer_radius);

    arg136.vect = (v123 - arg136.stPos) * v155;

    arg136_1.vect = (v133 - arg136_1.stPos) * v155;

    arg136_2.vect = (v122 - arg136_2.stPos) * v155;

    if ( a4 )
    {
        arg136.flags = 0;
        arg136_1.flags = 0;
        arg136_2.flags = 0;
    }
    else
    {
        arg136.flags = 1;
        arg136_1.flags = 1;
        arg136_2.flags = 1;
    }

    _world->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        vec3d v48 = arg136.skel->polygons[ arg136.polyID ].Normal();

        if ( fabs(v48.y) < 0.6 )
        {
            float v164 = v48.dot(_rotation.AxisZ()) * v156;

            if ( v164 <= 0.0 )
            {
                _thraction = 0;
            }
            else
            {

                if ( _fly_dir_length > 2.333333333333334 )
                {
                    if ( v143 )
                    {
                        SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                        yw_arg180 arg180;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = arg136.isectPos.x;
                        arg180.field_C = arg136.isectPos.z;
                        arg180.effects_type = 5;

                        _world->ypaworld_func180(&arg180);
                    }
                }

                if ( v164 >= 0.82 )
                {
                    _thraction = 0;
                    _fly_dir_length = 0;
                    _position = _old_pos;
                    return 2;
                }

                _position.x = _old_pos.x - v48.x * 10.0;
                _position.z = _old_pos.z - v48.z * 10.0;
            }
            return 2;
        }
    }
    else
    {
        if ( !v143 )
            return 2;

        arg136.isectPos = _position + arg136.vect - vec3d::OY(v152 * _viewer_radius);

        v149 = 1;
    }

    _world->ypaworld_func136(&arg136_1);

    if ( arg136_1.isect )
    {
        vec3d v54 = arg136_1.skel->polygons[ arg136_1.polyID ].Normal();

        if ( fabs(v54.y) < 0.6 )
        {
            float v172 = v54.dot(_rotation.AxisZ()) * v156;

            if ( v172 <= 0.0 )
            {
                _thraction = 0;
            }
            else
            {
                if ( _fly_dir_length > 2.333333333333334 )
                {
                    if ( v143 )
                    {
                        SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                        yw_arg180 arg180_3;
                        arg180_3.field_8 = arg136_1.isectPos.x;
                        arg180_3.field_C = arg136_1.isectPos.z;
                        arg180_3.effects_type = 5;
                        arg180_3.field_4 = 1.0;

                        _world->ypaworld_func180(&arg180_3);
                    }
                }
                if ( v172 >= 0.82 )
                {
                    _thraction = 0;
                    _fly_dir_length = 0;
                    _position = _old_pos;

                    return 1;
                }

                _position.x = _old_pos.x - v54.x * 10.0;
                _position.z = _old_pos.z - v54.z * 10.0;
            }
            return 1;
        }
    }
    else
    {
        if ( !v143 )
            return 1;

        arg136_1.isectPos = _position + arg136_1.vect - vec3d::OY(v152 * _viewer_radius);
        v151 = 1;
    }

    _world->ypaworld_func136(&arg136_2);

    if ( arg136_2.isect )
    {
        if ( fabs(arg136_2.skel->polygons[arg136_2.polyID].B) < 0.6 )
        {
            _thraction = 0;

            if ( _fly_dir_length < -2.333333333333334 )
            {
                if ( v143 )
                {
                    SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                    yw_arg180 arg180_1;
                    arg180_1.field_4 = 1.0;
                    arg180_1.field_8 = arg136_2.isectPos.x;
                    arg180_1.field_C = arg136_2.isectPos.z;
                    arg180_1.effects_type = 5;

                    _world->ypaworld_func180(&arg180_1);
                }
            }

            _position += _fly_dir * (_fly_dir_length * dtime * 6.0);

            return 3;
        }
    }
    else
    {
        arg136_2.isectPos = _position + arg136_2.vect - vec3d::OY(v152 * _viewer_radius);

        v148 = 1;
    }

    if ( a4 )
    {
        if ( v156 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.stPos = _position;
            arg136_3.vect = _rotation.AxisZ() * _viewer_radius;
            arg136_3.flags = 0;

            _world->ypaworld_func136(&arg136_3);

            if ( arg136_3.isect )
            {
                if ( arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6 )
                {
                    if ( _fly_dir_length > 2.333333333333334 )
                    {
                        if ( v143 )
                        {
                            SFXEngine::SFXe.startSound(&_soundcarrier, 5);

                            yw_arg180 arg180_2;
                            arg180_2.effects_type = 5;
                            arg180_2.field_4 = 1.0;
                            arg180_2.field_8 = arg136_3.isectPos.x;
                            arg180_2.field_C = arg136_3.isectPos.z;

                            _world->ypaworld_func180(&arg180_2);
                        }
                    }

                    _thraction = 0;
                    _fly_dir_length = 0;
                    _position = _old_pos;
                }
            }
        }
    }

    if ( arg136_1.isect && arg136.isect && arg136_2.isect )
    {

        vec3d v124 = arg136_1.isectPos - arg136_2.isectPos;
        vec3d v127 = arg136.isectPos - arg136_2.isectPos;
        vec3d v170 = v127 * v124;

        float v171 = v170.length() * v156;

        if ( fabs(v171) <= 0.0001 )
            v170 = vec3d::OY(1.0);
        else
            v170 /= v171;

        if ( v170.y < -0.1 )
            v170 = -v170;

        if ( _tankFlags & FLAG_TANK_TIP )
            v170 = TankTip(v170);

        float v74 = fabs(_fly_dir_length);

        vec3d vaxis = _rotation.AxisY() * v170;;

        float v163;
        if ( v74 >= 5.0 )
            v163 = 0.007;
        else
            v163 = 0.01;

        float v147 = vaxis.length();
        if ( v147 > 0.0 )
        {
            vaxis /= v147;

            float v166 = clp_acos( v170.dot( _rotation.AxisY() ) );

            if ( v166 > _maxrot * 2.0 * dtime )
                v166 = _maxrot * 2.0 * dtime;

            if ( fabs(v166) > v163 )
                _rotation *= mat3x3::AxisAngle(vaxis, v166);
        }
    }

    if ( arg136.isect || arg136_1.isect || arg136_2.isect )
    {
        if ( v149 || v151 || v148 )
        {
            float v92 = _viewer_overeof * v155 * 0.8;

            ypaworld_arg136 arg136_4;
            arg136_4.stPos = _position - vec3d::OY(v92);
            arg136_4.vect = vec3d::OY( v92 * 2.0 );

            arg136_4.flags = a4 == 0;

            _world->ypaworld_func136(&arg136_4);

            if ( !arg136_4.isect || (arg136_4.isect && arg136_4.skel->polygons[ arg136_4.polyID ].B < 0.6) )
            {
                vec3d tmp(0.0, 0.0, 0.0);

                _status_flg &= ~BACT_STFLAG_LAND;

                if ( v149 )
                    tmp = _rotation.AxisZ() * v156 - _rotation.AxisX();

                if ( v151 )
                    tmp += _rotation.AxisZ() * v156 + _rotation.AxisX();

                if ( v148 )
                    tmp -= _rotation.AxisZ() * v156;

                _position += tmp * (dtime * 400.0);
                return 0;
            }
            _position.y = arg136_4.isectPos.y - v5;
        }
        else
        {
            _position.y = (arg136.isectPos.y + arg136_1.isectPos.y + arg136_2.isectPos.y) * 0.33333334 - v5;
        }
    }
    else
    {
        _status_flg &= ~BACT_STFLAG_LAND;
    }
    return 0;
}

void NC_STACK_ypatank::setTANK_tip(int tip)
{
    if ( tip )
        _tankFlags |= FLAG_TANK_TIP;
    else
        _tankFlags &= ~FLAG_TANK_TIP;
}

int NC_STACK_ypatank::getTANK_tip()
{
    if (_tankFlags & FLAG_TANK_TIP)
        return 1;
    return 0;
}


