#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypamissile.h"
#include "yparobo.h"
#include "yw_net.h"

#include "log.h"

#include <math.h>

const Nucleus::ClassDescr NC_STACK_ypamissile::description("ypamissile.class", &newinstance);

size_t NC_STACK_ypamissile::func0(IDVList &stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    _bact_type = BACT_TYPES_MISSLE;

    _mislEmitter = NULL;
    _mislLifeTime = 5000;
    _mislDelayTime = 0;
    _mislType = MISL_BOMB;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_VIEWER:
                setBACT_viewer(val.Get<int32_t>());
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher(val.Get<NC_STACK_ypabact *>());
                break;

            case MISS_ATT_TYPE:
                setMISS_type(val.Get<int32_t>());
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(val.Get<int32_t>());
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(val.Get<int32_t>());
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(val.Get<int32_t>());
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( val.Get<int32_t>() );
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypamissile::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypamissile::func2(IDVList &stak)
{
    NC_STACK_ypabact::func2(stak);

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BACT_ATT_VIEWER:
                setBACT_viewer(val.Get<int32_t>());
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher(val.Get<NC_STACK_ypabact *>());
                break;

            case MISS_ATT_TYPE:
                setMISS_type(val.Get<int32_t>());
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(val.Get<int32_t>());
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(val.Get<int32_t>());
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(val.Get<int32_t>());
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( val.Get<int32_t>() );
                break;

            case MISS_ATT_POW_HELI:
                setMISS_powHeli(val.Get<int32_t>());
                break;

            case MISS_ATT_POW_TANK:
                setMISS_powTank(val.Get<int32_t>());
                break;

            case MISS_ATT_POW_FLYER:
                setMISS_powFlyer(val.Get<int32_t>());
                break;

            case MISS_ATT_POW_ROBO:
                setMISS_powRobo(val.Get<int32_t>());
                break;

            case MISS_ATT_RAD_HELI:
                setMISS_radHeli(val.Get<int32_t>());
                break;

            case MISS_ATT_RAD_TANK:
                setMISS_radTank(val.Get<int32_t>());
                break;

            case MISS_ATT_RAD_FLYER:
                setMISS_radFlyer(val.Get<int32_t>());
                break;

            case MISS_ATT_RAD_ROBO:
                setMISS_radRobo(val.Get<int32_t>());
                break;

            case MISS_ATT_STHEIGHT:
                setMISS_startHeight(val.Get<int32_t>());
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

void NC_STACK_ypamissile::AI_layer1(update_msg *arg)
{
    if ( _status == BACT_STATUS_DEAD )
        _yls_time -= arg->frameTime;


    if ( _primTtype )
    {
        if ( _primTtype == BACT_TGT_TYPE_UNIT )
            _target_vec = _primT.pbact->_position - _position;
        else
            _target_vec = _primTpos - _position;
    }

    AI_layer2(arg);
}

void NC_STACK_ypamissile::AI_layer2(update_msg *arg)
{
    AI_layer3(arg);
}

bool NC_STACK_ypamissile::TubeCollisionTest()
{
    vec3d collisionSumPosition(0.0, 0.0, 0.0);
    int collisionCount = 0;
    float collisionSumRadius = 0.0;

    int a5 = _mislEmitter->getBACT_inputting();

    if ( !a5 )
        a5 = getBACT_viewer();

    yw_130arg arg130;
    arg130.pos_x = _old_pos.x;
    arg130.pos_z = _old_pos.z;
    _world->ypaworld_func130(&arg130);

    cellArea *pCells[3];

    pCells[0] = arg130.pcell;

    arg130.pos_x = _position.x;
    arg130.pos_z = _position.z;
    _world->ypaworld_func130(&arg130);

    pCells[2] = arg130.pcell;

    if ( arg130.pcell == pCells[0] )
    {
        pCells[1] = pCells[0];
    }
    else
    {
        arg130.pos_x = (_position.x - _old_pos.x) * 0.5 + _old_pos.x;
        arg130.pos_z = (_position.z - _old_pos.z) * 0.5 + _old_pos.z;
        _world->ypaworld_func130(&arg130);

        pCells[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || pCells[i] != pCells[i - 1] )
        {
            if (pCells[i] == NULL)
                ypa_log_out("ypamissile_func70__sub0 NULL sector i = %d, 621: %f %f 62D: %f %f \n", i, _position.x, _position.z, _old_pos.x, _old_pos.z);

            for ( NC_STACK_ypabact* &bct : pCells[i]->unitsList )
            {
                if ( bct == this || bct == _mislEmitter || bct->_bact_type == BACT_TYPES_MISSLE || bct->_status == BACT_STATUS_DEAD )
                    continue;

                if (bct->_bact_type == BACT_TYPES_GUN && bct->_shield >= 100)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );

                    if ( gun->IsRoboGun() )
                        continue;
                }

                if ( !a5 && bct->_owner == _mislEmitter->_owner )
                {
                    continue;
                }

                if (_mislEmitter->_bact_type == BACT_TYPES_GUN)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( _mislEmitter );

                    if (bct->_owner == _owner)
                    {
                        if (gun->IsRoboGun())
                        {
                            if (bct->_bact_type == BACT_TYPES_ROBO)
                                continue;

                            if (bct->_bact_type == BACT_TYPES_GUN )
                            {
                                NC_STACK_ypagun *bgun = dynamic_cast<NC_STACK_ypagun *>( bct );

                                if (bgun->IsRoboGun())
                                    continue;
                            }
                        }
                    }
                }

                if ( _mislType == MISL_BOMB && bct->_position.y < _mislStartHeight )
                    continue;

                rbcolls *v82 = bct->getBACT_collNodes();

                int v7;
                if ( v82 )
                    v7 = v82->robo_coll_num;
                else
                    v7 = 1;

                for (int j = v7 - 1; j >= 0; j--)
                {
                    float radius;
                    vec3d ttmp;

                    if ( v82 )
                    {
                        roboColl *v8 = &v82->roboColls[j];
                        radius = v8->robo_coll_radius;

                        ttmp = bct->_position + bct->_rotation.Transpose().Transform(v8->coll_pos);
                    }
                    else
                    {
                        ttmp = bct->_position;
                        radius = bct->_radius;
                    }

                    if ( !v82 || radius >= 0.01 )
                    {
                        vec3d to_enemy = ttmp - _old_pos;
                        vec3d dist_vect = _position - _old_pos;

                        if ( to_enemy.dot( _rotation.AxisZ() )>= 0.3 )
                        {
                            float dist_vect_len = dist_vect.normalise();

                            vec3d vp = dist_vect * to_enemy;

                            float wpn_radius = 0.0;

                            switch ( bct->_bact_type )
                            {
                            case BACT_TYPES_BACT:
                                wpn_radius = _mislRadiusHeli;
                                break;

                            case BACT_TYPES_TANK:
                            case BACT_TYPES_CAR:
                                wpn_radius = _mislRadiusTank;
                                break;

                            case BACT_TYPES_FLYER:
                            case BACT_TYPES_UFO:
                                wpn_radius = _mislRadiusFlyer;
                                break;

                            case BACT_TYPES_ROBO:
                                wpn_radius = _mislRadiusRobo;
                                break;

                            default:
                                wpn_radius = _radius;
                                break;
                            }

                            if ( wpn_radius == 0.0)
                                wpn_radius = _radius;

                            float vp_len = vp.length();
                            float to_enemy_len = to_enemy.length();

                            if ( radius + wpn_radius > vp_len )
                            {
                                /*  Tube collision test, not cylinder!
                                    Will hit only when distance ~ wpn_radius */
                                if ( sqrt( POW2(dist_vect_len) + POW2(vp_len) ) > fabs(to_enemy_len - wpn_radius) )
                                {
                                    NC_STACK_ypabact *a1 = _world->getYW_userHostStation();

                                    collisionSumRadius += radius;
                                    collisionCount++;
                                    collisionSumPosition += bct->_position;

                                    bct->_status_flg &= ~BACT_STFLAG_LAND;

                                    

                                    int v83 = bct->getBACT_inputting();

                                    int v92 = 0;

                                    switch ( bct->_bact_type )
                                    {
                                    case BACT_TYPES_BACT:
                                        v92 = _energy * _mislEnergyHeli;
                                        break;

                                    case BACT_TYPES_TANK:
                                    case BACT_TYPES_CAR:
                                        v92 = _energy * _mislEnergyTank;
                                        break;

                                    case BACT_TYPES_FLYER:
                                    case BACT_TYPES_UFO:
                                        v92 = _energy * _mislEnergyFlyer;
                                        break;

                                    case BACT_TYPES_ROBO:
                                        v92 = _energy * _mislEnergyRobo;
                                        break;

                                    default:
                                        v92 = _energy;
                                        break;
                                    }

                                    float v46;
                                    float v47;

                                    if ( v83 || bct->_status_flg & BACT_STFLAG_ISVIEW )
                                    {
                                        v46 = v92 * (100 - bct->_shield);
                                        v47 = 250;
                                    }
                                    else
                                    {
                                        v46 = v92 * (100 - bct->_shield);
                                        v47 = 100.0;
                                    }

                                    v92 = ceil(v46 / v47); //Missile damage (ceil for less damage)
                                    if ( v92 )
                                    {
                                        bact_arg84 arg84;
                                        arg84.energy = -v92;
                                        arg84.unit = _mislEmitter;

                                        if ( a1->_owner == _owner || !_world->isNetGame )
                                            bct->ModifyEnergy(&arg84);
                                    }

                                    break;
                                }
                            }
                        }
                    }
                }

            }
        }
    }

    if ( collisionCount > 0 )
    {
        // Set new position between collided objects
        _position = collisionSumPosition / (float)collisionCount;

        collisionSumRadius /= (float)collisionCount;

        if ( collisionSumRadius >= 50.0 )
        {
            vec3d posDelta = _position - _old_pos;
            float deltaLen = posDelta.length();

            if ( deltaLen < 1.0 )
                deltaLen = 1.0;

            _position -= (posDelta / deltaLen) * collisionSumRadius;
        }
    }

    return collisionCount > 0;
}

vec3d NC_STACK_ypamissile::CalcForceVector()
{
    _thraction = _force;

    return vec3d::Normalise(  _fly_dir * _fly_dir_length * _airconst
                            + _target_dir * _thraction
                            - vec3d(0.0, _mass * 9.80665, 0.0));
}

void NC_STACK_ypamissile::AI_layer3(update_msg *arg)
{
    _world->ypaworld_func145(this);

    float v40 = _target_vec.length();

    if ( v40 > 0.1 )
    {
        if ( _primTtype != BACT_TGT_TYPE_DRCT )
            _target_dir = _target_vec / v40;
    }

    _AI_time1 = 0;

    _thraction = _force;

    float v38 = arg->frameTime * 0.001;

    if ( _status == BACT_STATUS_NORMAL )
    {
        if ( _mislFlags & FLAG_MISL_COUNTDELAY)
            _mislDelayTime -= arg->frameTime;

        if ( (_mislFlags & FLAG_MISL_COUNTDELAY)  &&  _mislDelayTime <= 0 )
        {
            Impact();

            _status = BACT_STATUS_DEAD;

            setState_msg arg78;
            arg78.setFlags = BACT_STFLAG_DEATH2;
            arg78.unsetFlags = 0;
            arg78.newStatus = BACT_STATUS_NOPE;

            SetState(&arg78);

            if ( !(_mislFlags & FLAG_MISL_IGNOREBUILDS) || !_pSector->w_type )
            {
                if ( _world->UserRobo->_owner == _owner || !_world->isNetGame )
                {
                    yw_arg129 v25;

                    v25.pos.x = _fly_dir.x * 5.0 + _position.x;
                    v25.pos.z = _fly_dir.z * 5.0 + _position.z;
                    v25.field_10 = _energy;
                    v25.unit = _mislEmitter;

                    ChangeSectorEnergy(&v25);
                }
            }
        }
        else
        {
            move_msg arg74;

            switch ( _mislType )
            {
            case MISL_BOMB:
                arg74.field_0 = v38;
                arg74.flag = 1;
                Move(&arg74);
                break;

            case MISL_DIRECT:
                arg74.field_0 = v38;
                arg74.flag = 0;
                arg74.vec = CalcForceVector();
                Move(&arg74);
                break;

            case MISL_TARGETED:
                arg74.field_0 = v38;
                arg74.flag = 0;
                arg74.vec = CalcForceVector();
                Move(&arg74);
                break;

            case MISL_GRENADE:
                arg74.field_0 = v38;
                arg74.vec = _fly_dir;
                arg74.flag = 0;

                Move(&arg74);
                break;

            default:
                break;
            }

            if ( TubeCollisionTest() )
            {
                setState_msg arg78;
                Impact();

                arg78.newStatus = BACT_STATUS_DEAD;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);

                ResetViewing();
                return;
            }
            
            if ( _mislType == MISL_INTERNAL )
                return;
            
            ypaworld_arg136 arg136;
            arg136.stPos = _old_pos;
            arg136.vect = _position - _old_pos;
            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect )
            {
                AlignMissileByNormal( arg136.skel->polygons[ arg136.polyID ].Normal() );

                _position = arg136.isectPos;

                ResetViewing();

                _mislType = MISL_INTERNAL;
                _mislFlags |= FLAG_MISL_COUNTDELAY;

                if ( !_mislDelayTime )
                {
                    Impact();

                    _status = BACT_STATUS_DEAD;

                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_DEATH2;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);

                    if ( !(_mislFlags & FLAG_MISL_IGNOREBUILDS) || !_pSector->w_type )
                    {
                        if ( _world->UserRobo->_owner == _owner || !_world->isNetGame )
                        {
                            yw_arg129 v25;

                            v25.pos.x = _fly_dir.x * 5.0 + _position.x;
                            v25.pos.z = _fly_dir.z * 5.0 + _position.z;
                            v25.field_10 = _energy;
                            v25.unit = _mislEmitter;

                            ChangeSectorEnergy(&v25);
                        }
                    }
                }

                int a4 = _mislEmitter->getBACT_inputting();

                if ( a4 )
                {
                    if ( _mislEmitter->IsParentMyRobo() )
                    {
                        setTarget_msg arg67;
                        arg67.tgt_type = BACT_TGT_TYPE_CELL;
                        arg67.tgt_pos = _position;
                        arg67.priority = 0;

                        _mislEmitter->SetTarget(&arg67);
                    }
                }
            }
            else
            {
                _mislDriveTime -= arg->frameTime;

                if ( _mislDriveTime < 0 )
                {
                    _mislType = MISL_BOMB;

                    _airconst = 10.0;
                    _airconst_static = 10.0;
                }

                _mislLifeTime -= arg->frameTime;

                if ( _mislLifeTime >= 0 )
                {
                    AlignMissile( arg->frameTime * 0.001 );
                }
                else
                {
                    Impact();

                    setState_msg arg78;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_DEAD;

                    SetState(&arg78);

                    ResetViewing();
                }
            }
        }
    }
}

void NC_STACK_ypamissile::User_layer(update_msg *arg)
{
    _old_pos = _position;

    if (_status == BACT_STATUS_NORMAL)
        AI_layer1(arg);
    else
        ResetViewing();
}

void NC_STACK_ypamissile::Move(move_msg *arg)
{
    _old_pos = _position;

    float v8;

    if ( _status != BACT_STATUS_DEAD && _mislType != MISL_BOMB )
        v8 = _mass * 9.80665;
    else
        v8 = _mass * 39.2266;

    vec3d v26(0.0, 0.0, 0.0);

    if ( !(arg->flag & 1) )
        v26 = arg->vec * _thraction;

    vec3d vec1 = vec3d(0.0, v8, 0.0) + v26 - _fly_dir * (_fly_dir_length * _airconst);

    float v33 = vec1.normalise();

    if ( v33 > 0.0 )
    {
        vec3d v36 = _fly_dir * _fly_dir_length + vec1 * (v33 / _mass * arg->field_0);

        float v32 = v36.length();

        if ( v32 > 0.0 )
            v36 /= v32;

        _fly_dir = v36;

        _fly_dir_length = v32;
    }

    _position += _fly_dir * (_fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);
}

void NC_STACK_ypamissile::SetState(setState_msg *arg)
{
    SetStateInternal(arg);
}

void NC_STACK_ypamissile::Renew()
{
    NC_STACK_ypabact::Renew();

    _mislFlags  = 0;
    _mislDelayTime = 0;

    setBACT_yourLastSeconds(3000);
}

size_t NC_STACK_ypamissile::SetStateInternal(setState_msg *arg)
{
    SFXEngine::SFXe.sub_424000(&_soundcarrier, 2);
    SFXEngine::SFXe.sub_424000(&_soundcarrier, 0);
    SFXEngine::SFXe.sub_424000(&_soundcarrier, 1);

    if ( arg->newStatus )
        _status = arg->newStatus;

    if ( arg->setFlags )
        _status_flg |= arg->setFlags;

    if ( arg->unsetFlags )
        _status_flg &= ~arg->unsetFlags;

    if ( arg->newStatus == BACT_STATUS_DEAD )
    {
        setBACT_visProto(_vp_dead.base);
        setBACT_vpTransform(_vp_dead.trigo);

        SFXEngine::SFXe.startSound(&_soundcarrier, 2);

        StartDestFX(1);

        _fly_dir_length = 0;
    }

    if ( arg->newStatus == BACT_STATUS_NORMAL )
    {
        setBACT_visProto(_vp_normal.base);
        setBACT_vpTransform(_vp_normal.trigo);

        SFXEngine::SFXe.startSound(&_soundcarrier, 0);
    }

    if ( arg->unsetFlags == BACT_STFLAG_DEATH2 )
    {
        setBACT_visProto(_vp_normal.base);
        setBACT_vpTransform(_vp_normal.trigo);

        SFXEngine::SFXe.startSound(&_soundcarrier, 0);
    }

    if ( arg->setFlags == BACT_STFLAG_DEATH2 )
    {
        _status = BACT_STATUS_DEAD;

        setBACT_visProto(_vp_megadeth.base);
        setBACT_vpTransform(_vp_megadeth.trigo);

        SFXEngine::SFXe.startSound(&_soundcarrier, 2);

        StartDestFX(2);

        _fly_dir_length = 0;
    }

    return 1;
}

void NC_STACK_ypamissile::ResetViewing()
{
    if ( getBACT_viewer() )
    {
        setBACT_viewer(0);
        setBACT_inputting(0);

        if ( _mislEmitter->_status != BACT_STATUS_DEAD || _mislEmitter->_parent == NULL )
        {
            _mislEmitter->setBACT_viewer(1);
            _mislEmitter->setBACT_inputting(1);
        }
        else
        {
            _mislEmitter->_parent->setBACT_viewer(1);
            _mislEmitter->_parent->setBACT_inputting(1);
        }

    }
}

void NC_STACK_ypamissile::Impact()
{
    bact_arg83 arg83;
    arg83.energ = _energy;
    arg83.pos = _position;
    arg83.pos2 = _fly_dir;
    arg83.force = _fly_dir_length;
    arg83.mass = _mass;

    float v16 = _fly_dir_length * _mass;

    if ( v16 > _world->max_impulse && _world->max_impulse > 0.0 )
    {
        float v7 = _world->max_impulse / v16;
        arg83.force *= v7;
        arg83.mass *= v7;
    }

    for( NC_STACK_ypabact* &bct : _pSector->unitsList )
    {
        if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_ROBO && bct->_bact_type != BACT_TYPES_TANK && bct->_bact_type != BACT_TYPES_CAR && bct->_bact_type != BACT_TYPES_GUN && bct->_bact_type != BACT_TYPES_HOVER && !(bct->_status_flg & BACT_STFLAG_DEATH2) )
        {
            int v10 = 1;

            if ( _world->isNetGame )
            {
                if ( _owner != bct->_owner )
                    v10 = 0;
            }

            if ( v10 )
                bct->ApplyImpulse(&arg83);
        }
    }

    if ( _world->isNetGame )
    {
        uamessage_impulse impMsg;
        impMsg.msgID = UAMSG_IMPULSE;
        impMsg.owner = _owner;
        impMsg.id = _gid;
        impMsg.pos = _position;
        impMsg.impulse = _energy;
        impMsg.dir = _fly_dir;
        impMsg.dir_len = _fly_dir_length;
        impMsg.mass = _mass;

        yw_arg181 arg181;
        arg181.recvID = 0;
        arg181.dataSize = sizeof(impMsg);
        arg181.recvFlags = 2;
        arg181.garant = 1;
        arg181.data = &impMsg;

        _world->ypaworld_func181(&arg181);
    }
}

void NC_STACK_ypamissile::AlignMissile(float dtime)
{
    if ( _fly_dir != vec3d(0.0, 0.0, 0.0) )
    {
        vec3d dir = _rotation.AxisZ(); // Get Z-axis, as dir
        vec3d u = vec3d::Normalise(dir * _fly_dir); // vector cross product

        // If length == 0 - no rotation
        if ( u.length() > 0.0 )
        {
            //scalar cross product
            float rotAngle = clp_acos( dir.dot(_fly_dir) );

            if ( _mislType == MISL_BOMB )
            {
                if ( dtime != 0.0 )
                {
                    float mxrot = _maxrot * dtime;

                    if ( rotAngle < -mxrot )
                        rotAngle = -mxrot;

                    if ( rotAngle > mxrot )
                        rotAngle = mxrot;
                }
            }

            if ( fabs(rotAngle) > BOMB_MIN_ANGLE )
                _rotation *= mat3x3::AxisAngle(u, rotAngle);
        }

        // Fix camera Z-axis rotation
        if ( _mislFlags & FLAG_MISL_VIEW )
        {
            float ZAngle = clp_acos( _rotation.AxisX().XZ().length() ); // Get degree of current Z-axis rotation

            if ( _rotation.m11 < 0.0 )
                ZAngle = C_PI - ZAngle;

            if ( _rotation.m01 < 0.0 )
                ZAngle = -ZAngle;

            _rotation = mat3x3::RotateZ(-ZAngle) * _rotation;
        }
    }
}

void NC_STACK_ypamissile::AlignMissileByNormal(const vec3d &normal)
{
    vec3d UpVector = _rotation.AxisY();

    vec3d vaxis = UpVector * normal;

    if ( vaxis.normalise() != 0.0 )
    {
        float angle = clp_acos( UpVector.dot(normal) );

        if ( fabs(angle) > BACT_MIN_ANGLE )
            _rotation *= mat3x3::AxisAngle(vaxis, angle);
    }
}


NC_STACK_ypamissile::NC_STACK_ypamissile() 
{
    _mislType = 0;
    _mislEmitter = NULL;
    _mislLifeTime = 0;
    _mislDriveTime = 0;
    _mislDelayTime = 0;
    _mislFlags = 0;
    _mislStartHeight = 0.;
    _mislEnergyHeli = 0.;
    _mislEnergyTank = 0.;
    _mislEnergyFlyer = 0.;
    _mislEnergyRobo = 0.;
    _mislRadiusHeli = 0.;
    _mislRadiusTank = 0.;
    _mislRadiusFlyer = 0.;
    _mislRadiusRobo = 0.;
}


void NC_STACK_ypamissile::setBACT_viewer(int vwr)
{
    NC_STACK_ypabact::setBACT_viewer(vwr);

    if ( vwr )
        _mislFlags |= FLAG_MISL_VIEW;
    else
        _mislFlags &= ~FLAG_MISL_VIEW;
}

void NC_STACK_ypamissile::setMISS_launcher(NC_STACK_ypabact *bact)
{
    _mislEmitter = bact;
}

void NC_STACK_ypamissile::setMISS_type(int tp)
{
    _mislType = tp;
}

void NC_STACK_ypamissile::setMISS_lifeTime(int time)
{
    _mislLifeTime = time;
}

void NC_STACK_ypamissile::setMISS_delay(int delay)
{
    _mislDelayTime = delay;
}

void NC_STACK_ypamissile::setMISS_driveTime(int time)
{
    _mislDriveTime = time;
}

void NC_STACK_ypamissile::setMISS_ignoreBuilds(int ign)
{
    if ( ign )
        _mislFlags |= FLAG_MISL_IGNOREBUILDS;
    else
        _mislFlags &= ~FLAG_MISL_IGNOREBUILDS;
}

void NC_STACK_ypamissile::setMISS_powHeli(int po)
{
    _mislEnergyHeli = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powTank(int po)
{
    _mislEnergyTank = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powFlyer(int po)
{
    _mislEnergyFlyer = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powRobo(int po)
{
    _mislEnergyRobo = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_radHeli(int rad)
{
    _mislRadiusHeli = rad;
}

void NC_STACK_ypamissile::setMISS_radTank(int rad)
{
    _mislRadiusTank = rad;
}

void NC_STACK_ypamissile::setMISS_radFlyer(int rad)
{
    _mislRadiusFlyer = rad;
}

void NC_STACK_ypamissile::setMISS_radRobo(int rad)
{
    _mislRadiusRobo = rad;
}

void NC_STACK_ypamissile::setMISS_startHeight(int posy)
{
    _mislStartHeight = posy;
}



NC_STACK_ypabact *NC_STACK_ypamissile::getMISS_launcher()
{
    return _mislEmitter;
}

int NC_STACK_ypamissile::getMISS_type()
{
    return _mislType;
}

int NC_STACK_ypamissile::getMISS_lifeTime()
{
    return _mislLifeTime;
}

int NC_STACK_ypamissile::getMISS_delay()
{
    return _mislDelayTime;
}

int NC_STACK_ypamissile::getMISS_driveTime()
{
    return _mislDriveTime;
}

int NC_STACK_ypamissile::getMISS_ignoreBuilds()
{
    return (_mislFlags & FLAG_MISL_IGNOREBUILDS) != 0;
}

int NC_STACK_ypamissile::getMISS_powHeli()
{
    return _mislEnergyHeli * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powTank()
{
    return _mislEnergyTank * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powFlyer()
{
    return _mislEnergyFlyer * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powRobo()
{
    return _mislEnergyRobo * 1000.0;
}

int NC_STACK_ypamissile::getMISS_radHeli()
{
    return _mislRadiusHeli;
}

int NC_STACK_ypamissile::getMISS_radTank()
{
    return _mislRadiusTank;
}

int NC_STACK_ypamissile::getMISS_radFlyer()
{
    return _mislRadiusFlyer;
}

int NC_STACK_ypamissile::getMISS_radRobo()
{
    return _mislRadiusRobo;
}

int NC_STACK_ypamissile::getMISS_startHeight()
{
    return _mislStartHeight;
}


