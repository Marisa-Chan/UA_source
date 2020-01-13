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

    ypabact.bact_type = BACT_TYPES_MISSLE;

    _mislEmitter = NULL;
    _mislLifeTime = 5000;
    _mislDelayTime = 0;
    _mislType = MISL_BOMB;

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case BACT_ATT_VIEWER:
                setBACT_viewer(val.value.i_data);
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher((NC_STACK_ypabact *)val.value.p_data);
                break;

            case MISS_ATT_TYPE:
                setMISS_type(val.value.i_data);
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(val.value.i_data);
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(val.value.i_data);
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(val.value.i_data);
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( val.value.i_data );
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

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case BACT_ATT_VIEWER:
                setBACT_viewer(val.value.i_data);
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher((NC_STACK_ypabact *)val.value.p_data);
                break;

            case MISS_ATT_TYPE:
                setMISS_type(val.value.i_data);
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(val.value.i_data);
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(val.value.i_data);
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(val.value.i_data);
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( val.value.i_data );
                break;

            case MISS_ATT_POW_HELI:
                setMISS_powHeli(val.value.i_data);
                break;

            case MISS_ATT_POW_TANK:
                setMISS_powTank(val.value.i_data);
                break;

            case MISS_ATT_POW_FLYER:
                setMISS_powFlyer(val.value.i_data);
                break;

            case MISS_ATT_POW_ROBO:
                setMISS_powRobo(val.value.i_data);
                break;

            case MISS_ATT_RAD_HELI:
                setMISS_radHeli(val.value.i_data);
                break;

            case MISS_ATT_RAD_TANK:
                setMISS_radTank(val.value.i_data);
                break;

            case MISS_ATT_RAD_FLYER:
                setMISS_radFlyer(val.value.i_data);
                break;

            case MISS_ATT_RAD_ROBO:
                setMISS_radRobo(val.value.i_data);
                break;

            case MISS_ATT_STHEIGHT:
                setMISS_startHeight(val.value.i_data);
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypamissile::func3(IDVList &stak)
{
    NC_STACK_ypabact::func3(stak);

    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case MISS_ATT_LAUNCHER:
                *(NC_STACK_ypabact **)val.value.p_data = getMISS_launcher();
                break;

            case MISS_ATT_TYPE:
                *(int *)val.value.p_data = getMISS_type();
                break;

            case MISS_ATT_LIFETIME:
                *(int *)val.value.p_data = getMISS_lifeTime();
                break;

            case MISS_ATT_DELAY:
                *(int *)val.value.p_data = getMISS_delay();
                break;

            case MISS_ATT_DRIVETIME:
                *(int *)val.value.p_data = getMISS_driveTime();
                break;

            case MISS_ATT_IGNOREBUILDS:
                *(int *)val.value.p_data = getMISS_ignoreBuilds();
                break;

            case MISS_ATT_POW_HELI:
                *(int *)val.value.p_data = getMISS_powHeli();
                break;

            case MISS_ATT_POW_TANK:
                *(int *)val.value.p_data = getMISS_powTank();
                break;

            case MISS_ATT_POW_FLYER:
                *(int *)val.value.p_data = getMISS_powFlyer();
                break;

            case MISS_ATT_POW_ROBO:
                *(int *)val.value.p_data = getMISS_powRobo();
                break;

            case MISS_ATT_RAD_HELI:
                *(int *)val.value.p_data = getMISS_radHeli();
                break;

            case MISS_ATT_RAD_TANK:
                *(int *)val.value.p_data = getMISS_radTank();
                break;

            case MISS_ATT_RAD_FLYER:
                *(int *)val.value.p_data = getMISS_radFlyer();
                break;

            case MISS_ATT_RAD_ROBO:
                *(int *)val.value.p_data = getMISS_radRobo();
                break;

            case MISS_ATT_STHEIGHT:
                *(int *)val.value.p_data = getMISS_startHeight();
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
    if ( ypabact.status == BACT_STATUS_DEAD )
        ypabact.yls_time -= arg->frameTime;


    if ( ypabact.primTtype )
    {
        if ( ypabact.primTtype == BACT_TGT_TYPE_UNIT )
            ypabact.target_vec = ypabact.primT.pbact->position - ypabact.position;
        else
            ypabact.target_vec = ypabact.primTpos - ypabact.position;
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
        a5 = ypabact.self->getBACT_viewer();

    yw_130arg arg130;
    arg130.pos_x = ypabact.old_pos.x;
    arg130.pos_z = ypabact.old_pos.z;
    _world->ypaworld_func130(&arg130);

    cellArea *v68[3];

    v68[0] = arg130.pcell;

    arg130.pos_x = ypabact.position.x;
    arg130.pos_z = ypabact.position.z;
    _world->ypaworld_func130(&arg130);

    v68[2] = arg130.pcell;

    if ( arg130.pcell == v68[0] )
    {
        v68[1] = v68[0];
    }
    else
    {
        arg130.pos_x = (ypabact.position.x - ypabact.old_pos.x) * 0.5 + ypabact.old_pos.x;
        arg130.pos_z = (ypabact.position.z - ypabact.old_pos.z) * 0.5 + ypabact.old_pos.z;
        _world->ypaworld_func130(&arg130);

        v68[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || v68[i] != v68[i - 1] )
        {
            if (v68[i] == NULL)
                ypa_log_out("ypamissile_func70__sub0 NULL sector i = %d, 621: %f %f 62D: %f %f \n", i, ypabact.position.x, ypabact.position.z, ypabact.old_pos.x, ypabact.old_pos.z);

            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v68[ i ]->units_list.head;
            for (; bct->next ; bct = (__NC_STACK_ypabact *)bct->next)
            {
                if ( bct->self == this || bct->self == _mislEmitter || bct->bact_type == BACT_TYPES_MISSLE || bct->status == BACT_STATUS_DEAD )
                    continue;

                if (bct->bact_type == BACT_TYPES_GUN && bct->shield >= 100)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );

                    if ( gun->IsRoboGun() )
                        continue;
                }

                if ( !a5 && bct->owner == _mislEmitter->ypabact.owner )
                {
                    continue;
                }

                if (_mislEmitter->ypabact.bact_type == BACT_TYPES_GUN)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( _mislEmitter );

                    if (bct->owner == ypabact.owner)
                    {
                        if (gun->IsRoboGun())
                        {
                            if (bct->bact_type == BACT_TYPES_ROBO)
                                continue;

                            if (bct->bact_type == BACT_TYPES_GUN )
                            {
                                NC_STACK_ypagun *bgun = dynamic_cast<NC_STACK_ypagun *>( bct->self );

                                if (bgun->IsRoboGun())
                                    continue;
                            }
                        }
                    }
                }

                if ( _mislType == MISL_BOMB && bct->position.y < _mislStartHeight )
                    continue;

                rbcolls *v82 = bct->self->getBACT_collNodes();

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

                        ttmp = bct->position + bct->rotation.Transpose().Transform(v8->coll_pos);
                    }
                    else
                    {
                        ttmp = bct->position;
                        radius = bct->radius;
                    }

                    if ( !v82 || radius >= 0.01 )
                    {
                        vec3d to_enemy = ttmp - ypabact.old_pos;
                        vec3d dist_vect = ypabact.position - ypabact.old_pos;

                        if ( to_enemy.dot( ypabact.rotation.AxisZ() )>= 0.3 )
                        {
                            float dist_vect_len = dist_vect.normalise();

                            vec3d vp = dist_vect * to_enemy;

                            float wpn_radius = 0.0;

                            switch ( bct->bact_type )
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
                                wpn_radius = ypabact.radius;
                                break;
                            }

                            if ( wpn_radius == 0.0)
                                wpn_radius = ypabact.radius;

                            float vp_len = vp.length();
                            float to_enemy_len = to_enemy.length();

                            if ( radius + wpn_radius > vp_len )
                            {
                                /*  Tube collision test, not cylinder!
                                    Will hit only when distance ~ wpn_radius */
                                if ( sqrt( POW2(dist_vect_len) + POW2(vp_len) ) > fabs(to_enemy_len - wpn_radius) )
                                {
                                    NC_STACK_ypabact *a1 = _world->getYW_userHostStation();

                                    __NC_STACK_ypabact *v85;
                                    v85 = a1->getBACT_pBact();

                                    collisionSumRadius += radius;
                                    collisionCount++;
                                    collisionSumPosition += bct->position;

                                    bct->status_flg &= ~BACT_STFLAG_LAND;

                                    

                                    int v83 = bct->self->getBACT_inputting();

                                    int v92 = 0;

                                    switch ( bct->bact_type )
                                    {
                                    case BACT_TYPES_BACT:
                                        v92 = ypabact.energy * _mislEnergyHeli;
                                        break;

                                    case BACT_TYPES_TANK:
                                    case BACT_TYPES_CAR:
                                        v92 = ypabact.energy * _mislEnergyTank;
                                        break;

                                    case BACT_TYPES_FLYER:
                                    case BACT_TYPES_UFO:
                                        v92 = ypabact.energy * _mislEnergyFlyer;
                                        break;

                                    case BACT_TYPES_ROBO:
                                        v92 = ypabact.energy * _mislEnergyRobo;
                                        break;

                                    default:
                                        v92 = ypabact.energy;
                                        break;
                                    }

                                    float v46;
                                    float v47;

                                    if ( v83 || bct->status_flg & BACT_STFLAG_ISVIEW )
                                    {
                                        v46 = v92 * (100 - bct->shield);
                                        v47 = 250;
                                    }
                                    else
                                    {
                                        v46 = v92 * (100 - bct->shield);
                                        v47 = 100.0;
                                    }

                                    v92 = ceil(v46 / v47); //Missile damage (ceil for less damage)
                                    if ( v92 )
                                    {
                                        bact_arg84 arg84;
                                        arg84.energy = -v92;
                                        arg84.unit = &_mislEmitter->ypabact;

                                        if ( v85->owner == ypabact.owner || !_world->ypaworld.isNetGame )
                                            bct->self->ModifyEnergy(&arg84);
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
        ypabact.position = collisionSumPosition / (float)collisionCount;

        collisionSumRadius /= (float)collisionCount;

        if ( collisionSumRadius >= 50.0 )
        {
            vec3d posDelta = ypabact.position - ypabact.old_pos;
            float deltaLen = posDelta.length();

            if ( deltaLen < 1.0 )
                deltaLen = 1.0;

            ypabact.position -= (posDelta / deltaLen) * collisionSumRadius;
        }
    }

    return collisionCount > 0;
}

vec3d NC_STACK_ypamissile::CalcForceVector()
{
    ypabact.thraction = ypabact.force;

    return vec3d::Normalise(  ypabact.fly_dir * ypabact.fly_dir_length * ypabact.airconst
                            + ypabact.target_dir * ypabact.thraction
                            - vec3d(0.0, ypabact.mass * 9.80665, 0.0));
}

void NC_STACK_ypamissile::AI_layer3(update_msg *arg)
{
    _world->ypaworld_func145(&ypabact);

    float v40 = ypabact.target_vec.length();

    if ( v40 > 0.1 )
    {
        if ( ypabact.primTtype != BACT_TGT_TYPE_DRCT )
            ypabact.target_dir = ypabact.target_vec / v40;
    }

    ypabact.AI_time1 = 0;

    ypabact.thraction = ypabact.force;

    float v38 = arg->frameTime * 0.001;

    if ( ypabact.status == BACT_STATUS_NORMAL )
    {
        if ( _mislFlags & FLAG_MISL_COUNTDELAY)
            _mislDelayTime -= arg->frameTime;

        if ( (_mislFlags & FLAG_MISL_COUNTDELAY)  &&  _mislDelayTime <= 0 )
        {
            ApplyImpulse();

            ypabact.status = BACT_STATUS_DEAD;

            setState_msg arg78;
            arg78.setFlags = BACT_STFLAG_DEATH2;
            arg78.unsetFlags = 0;
            arg78.newStatus = BACT_STATUS_NOPE;

            SetState(&arg78);

            if ( !(_mislFlags & FLAG_MISL_IGNOREBUILDS) || !ypabact.pSector->w_type )
            {
                if ( _world->ypaworld.URBact->owner == ypabact.owner || !_world->ypaworld.isNetGame )
                {
                    yw_arg129 v25;

                    v25.pos.x = ypabact.fly_dir.x * 5.0 + ypabact.position.x;
                    v25.pos.z = ypabact.fly_dir.z * 5.0 + ypabact.position.z;
                    v25.field_10 = ypabact.energy;
                    v25.unit = &_mislEmitter->ypabact;

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
                arg74.vec = ypabact.fly_dir;
                arg74.flag = 0;

                Move(&arg74);
                break;

            default:
                break;
            }

            if ( TubeCollisionTest() )
            {
                setState_msg arg78;
                ApplyImpulse();

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
            arg136.stPos = ypabact.old_pos;
            arg136.vect = ypabact.position - ypabact.old_pos;
            arg136.flags = 0;

            _world->ypaworld_func136(&arg136);

            if ( arg136.isect )
            {
                AlignMissileByNormal( arg136.skel->polygons[ arg136.polyID ].Normal() );

                ypabact.position = arg136.isectPos;

                ResetViewing();

                _mislType = MISL_INTERNAL;
                _mislFlags |= FLAG_MISL_COUNTDELAY;

                if ( !_mislDelayTime )
                {
                    ApplyImpulse();

                    ypabact.status = BACT_STATUS_DEAD;

                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_DEATH2;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);

                    if ( !(_mislFlags & FLAG_MISL_IGNOREBUILDS) || !ypabact.pSector->w_type )
                    {
                        if ( _world->ypaworld.URBact->owner == ypabact.owner || !_world->ypaworld.isNetGame )
                        {
                            yw_arg129 v25;

                            v25.pos.x = ypabact.fly_dir.x * 5.0 + ypabact.position.x;
                            v25.pos.z = ypabact.fly_dir.z * 5.0 + ypabact.position.z;
                            v25.field_10 = ypabact.energy;
                            v25.unit = &_mislEmitter->ypabact;

                            ChangeSectorEnergy(&v25);
                        }
                    }
                }

                int a4 = _mislEmitter->getBACT_inputting();

                if ( a4 )
                {
                    if ( _mislEmitter->ypabact.host_station == _mislEmitter->ypabact.parent_bacto )
                    {
                        if ( _mislEmitter->ypabact.host_station )
                        {
                            setTarget_msg arg67;
                            arg67.tgt_type = BACT_TGT_TYPE_CELL;
                            arg67.tgt_pos = ypabact.position;
                            arg67.priority = 0;

                            _mislEmitter->SetTarget(&arg67);
                        }
                    }
                }
            }
            else
            {
                _mislDriveTime -= arg->frameTime;

                if ( _mislDriveTime < 0 )
                {
                    _mislType = MISL_BOMB;

                    ypabact.airconst = 10.0;
                    ypabact.airconst_static = 10.0;
                }

                _mislLifeTime -= arg->frameTime;

                if ( _mislLifeTime >= 0 )
                {
                    AlignMissile( arg->frameTime * 0.001 );
                }
                else
                {
                    ApplyImpulse();

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
    ypabact.old_pos = ypabact.position;

    if (ypabact.status == BACT_STATUS_NORMAL)
        AI_layer1(arg);
    else
        ResetViewing();
}

void NC_STACK_ypamissile::Move(move_msg *arg)
{
    ypabact.old_pos = ypabact.position;

    float v8;

    if ( ypabact.status != BACT_STATUS_DEAD && _mislType != MISL_BOMB )
        v8 = ypabact.mass * 9.80665;
    else
        v8 = ypabact.mass * 39.2266;

    vec3d v26(0.0, 0.0, 0.0);

    if ( !(arg->flag & 1) )
        v26 = arg->vec * ypabact.thraction;

    vec3d vec1 = vec3d(0.0, v8, 0.0) + v26 - ypabact.fly_dir * (ypabact.fly_dir_length * ypabact.airconst);

    float v33 = vec1.normalise();

    if ( v33 > 0.0 )
    {
        vec3d v36 = ypabact.fly_dir * ypabact.fly_dir_length + vec1 * (v33 / ypabact.mass * arg->field_0);

        float v32 = v36.length();

        if ( v32 > 0.0 )
            v36 /= v32;

        ypabact.fly_dir = v36;

        ypabact.fly_dir_length = v32;
    }

    ypabact.position += ypabact.fly_dir * (ypabact.fly_dir_length * arg->field_0 * 6.0);

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
    SFXEngine::SFXe.sub_424000(&ypabact.soundcarrier, 2);
    SFXEngine::SFXe.sub_424000(&ypabact.soundcarrier, 0);
    SFXEngine::SFXe.sub_424000(&ypabact.soundcarrier, 1);

    if ( arg->newStatus )
        ypabact.status = arg->newStatus;

    if ( arg->setFlags )
        ypabact.status_flg |= arg->setFlags;

    if ( arg->unsetFlags )
        ypabact.status_flg &= ~arg->unsetFlags;

    if ( arg->newStatus == BACT_STATUS_DEAD )
    {
        setBACT_visProto(ypabact.vp_dead.base);
        setBACT_vpTransform(ypabact.vp_dead.trigo);

        SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 2);

        StartDestFX(1);

        ypabact.fly_dir_length = 0;
    }

    if ( arg->newStatus == BACT_STATUS_NORMAL )
    {
        setBACT_visProto(ypabact.vp_normal.base);
        setBACT_vpTransform(ypabact.vp_normal.trigo);

        SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 0);
    }

    if ( arg->unsetFlags == BACT_STFLAG_DEATH2 )
    {
        setBACT_visProto(ypabact.vp_normal.base);
        setBACT_vpTransform(ypabact.vp_normal.trigo);

        SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 0);
    }

    if ( arg->setFlags == BACT_STFLAG_DEATH2 )
    {
        ypabact.status = BACT_STATUS_DEAD;

        setBACT_visProto(ypabact.vp_megadeth.base);
        setBACT_vpTransform(ypabact.vp_megadeth.trigo);

        SFXEngine::SFXe.startSound(&ypabact.soundcarrier, 2);

        StartDestFX(2);

        ypabact.fly_dir_length = 0;
    }

    return 1;
}

void NC_STACK_ypamissile::ResetViewing()
{
    if ( getBACT_viewer() )
    {
        setBACT_viewer(0);
        setBACT_inputting(0);

        if ( _mislEmitter->ypabact.status != BACT_STATUS_DEAD || (size_t)_mislEmitter->ypabact.parent_bacto <= 3 )
        {
            _mislEmitter->setBACT_viewer(1);
            _mislEmitter->setBACT_inputting(1);
        }
        else
        {
            _mislEmitter->ypabact.parent_bacto->setBACT_viewer(1);
            _mislEmitter->ypabact.parent_bacto->setBACT_inputting(1);
        }

    }
}

void NC_STACK_ypamissile::ApplyImpulse()
{
    bact_arg83 arg83;
    arg83.energ = ypabact.energy;
    arg83.pos = ypabact.position;
    arg83.pos2 = ypabact.fly_dir;
    arg83.force = ypabact.fly_dir_length;
    arg83.mass = ypabact.mass;

    float v16 = ypabact.fly_dir_length * ypabact.mass;

    if ( v16 > _world->ypaworld.max_impulse && _world->ypaworld.max_impulse > 0.0 )
    {
        float v7 = _world->ypaworld.max_impulse / v16;
        arg83.force *= v7;
        arg83.mass *= v7;
    }

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)ypabact.pSector->units_list.head;

    while(bct->next)
    {
        if ( bct->bact_type != BACT_TYPES_MISSLE && bct->bact_type != BACT_TYPES_ROBO && bct->bact_type != BACT_TYPES_TANK && bct->bact_type != BACT_TYPES_CAR && bct->bact_type != BACT_TYPES_GUN && bct->bact_type != BACT_TYPES_HOVER && !(bct->status_flg & BACT_STFLAG_DEATH2) )
        {
            int v10 = 1;

            if ( _world->ypaworld.isNetGame )
            {
                if ( ypabact.owner != bct->owner )
                    v10 = 0;
            }

            if ( v10 )
                bct->self->ApplyImpulse(&arg83);
        }

        bct = (__NC_STACK_ypabact *)bct->next;
    }

    if ( _world->ypaworld.isNetGame )
    {
        uamessage_impulse impMsg;
        impMsg.msgID = UAMSG_IMPULSE;
        impMsg.owner = ypabact.owner;
        impMsg.id = ypabact.gid;
        impMsg.pos = ypabact.position;
        impMsg.impulse = ypabact.energy;
        impMsg.dir = ypabact.fly_dir;
        impMsg.dir_len = ypabact.fly_dir_length;
        impMsg.mass = ypabact.mass;

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
    if ( ypabact.fly_dir != vec3d(0.0, 0.0, 0.0) )
    {
        vec3d dir = ypabact.rotation.AxisZ(); // Get Z-axis, as dir
        vec3d u = vec3d::Normalise(dir * ypabact.fly_dir); // vector cross product

        // If length == 0 - no rotation
        if ( u.length() > 0.0 )
        {
            //scalar cross product
            float rotAngle = clp_acos( dir.dot(ypabact.fly_dir) );

            if ( _mislType == MISL_BOMB )
            {
                if ( dtime != 0.0 )
                {
                    float mxrot = ypabact.maxrot * dtime;

                    if ( rotAngle < -mxrot )
                        rotAngle = -mxrot;

                    if ( rotAngle > mxrot )
                        rotAngle = mxrot;
                }
            }

            if ( fabs(rotAngle) > BOMB_MIN_ANGLE )
                ypabact.rotation *= mat3x3::AxisAngle(u, rotAngle);
        }

        // Fix camera Z-axis rotation
        if ( _mislFlags & FLAG_MISL_VIEW )
        {
            float ZAngle = clp_acos( ypabact.rotation.AxisX().XZ().length() ); // Get degree of current Z-axis rotation

            if ( ypabact.rotation.m11 < 0.0 )
                ZAngle = C_PI - ZAngle;

            if ( ypabact.rotation.m01 < 0.0 )
                ZAngle = -ZAngle;

            ypabact.rotation = mat3x3::RotateZ(-ZAngle) * ypabact.rotation;
        }
    }
}

void NC_STACK_ypamissile::AlignMissileByNormal(const vec3d &normal)
{
    vec3d UpVector = ypabact.rotation.AxisY();

    vec3d vaxis = UpVector * normal;

    if ( vaxis.normalise() != 0.0 )
    {
        float angle = clp_acos( UpVector.dot(normal) );

        if ( fabs(angle) > BACT_MIN_ANGLE )
            ypabact.rotation *= mat3x3::AxisAngle(vaxis, angle);
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


size_t NC_STACK_ypamissile::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( *(IDVList *)data );
    case 3:
        return func3( *(IDVList *)data );
    case 68:
        AI_layer1( (update_msg *)data );
        return 1;
    case 69:
        AI_layer2( (update_msg *)data );
        return 1;
    case 70:
        AI_layer3( (update_msg *)data );
        return 1;
    case 71:
        User_layer( (update_msg *)data );
        return 1;
    case 74:
        Move( (move_msg *)data );
        return 1;
    case 78:
        SetState( (setState_msg *)data );
        return 1;
    case 96:
        Renew();
        return 1;
    case 119:
        return (size_t)SetStateInternal( (setState_msg *)data );
    case 128:
        ResetViewing();
        return 1;
    case 129:
        ApplyImpulse();
        return 1;
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
