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


const NewClassDescr NC_STACK_ypamissile::description("ypamissile.class", &newinstance);

size_t NC_STACK_ypamissile::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    stack__ypamissile.selfie = &ypabact;

    stack__ypamissile.selfie_node.bact = &ypabact;
    stack__ypamissile.selfie_node.bacto = this;

    ypabact.bact_type = BACT_TYPES_MISSLE;

    stack__ypamissile.ejaculator_bact = 0;
    stack__ypamissile.life_time = 5000;
    stack__ypamissile.delay_time = 0;
    stack__ypamissile.field_c = 1;

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case BACT_ATT_WORLD:
                    stack__ypamissile.ywo = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__ypamissile.yw = &stack__ypamissile.ywo->stack__ypaworld;
                    break;

                case BACT_ATT_VIEWER:
                    setBACT_viewer(val.value.i_data);
                    break;

                case MISS_ATT_LAUNCHER:
                    setMISS_launcher((__NC_STACK_ypabact *)val.value.p_data);
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
    }

    return 1;
}

size_t NC_STACK_ypamissile::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypamissile::func2(IDVList *stak)
{
    NC_STACK_ypabact::func2(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
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
                    setMISS_launcher((__NC_STACK_ypabact *)val.value.p_data);
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
    }

    return 1;
}

size_t NC_STACK_ypamissile::func3(IDVList *stak)
{
    NC_STACK_ypabact::func3(stak);

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case MISS_ATT_LAUNCHER:
                    *(__NC_STACK_ypabact **)val.value.p_data = getMISS_launcher();
                    break;

                case MISS_ATT_TYPE:
                    *(int *)val.value.p_data = getMISS_type();
                    break;

                case MISS_ATT_PNODE:
                    *(bact_node **)val.value.p_data = getMISS_pNode();
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
    }

    return 1;
}

void NC_STACK_ypamissile::AI_layer1(update_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( miss->selfie->status == BACT_STATUS_DEAD )
    {
        int a4 = getBACT_yourLastSeconds();

        a4 -= arg->frameTime;

        setBACT_yourLastSeconds(a4);
    }

    if ( bact->primTtype )
    {
        if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
        {
            bact->target_vec.x = bact->primT.pbact->position.x - bact->position.x;
            bact->target_vec.y = bact->primT.pbact->position.y - bact->position.y;
            bact->target_vec.z = bact->primT.pbact->position.z - bact->position.z;
        }
        else
        {
            bact->target_vec.x = bact->primTpos.x - bact->position.x;
            bact->target_vec.y = bact->primTpos.y - bact->position.y;
            bact->target_vec.z = bact->primTpos.z - bact->position.z;
        }
    }

    AI_layer2(arg);
}

void NC_STACK_ypamissile::AI_layer2(update_msg *arg)
{
    AI_layer3(arg);
}

int ypamissile_func70__sub0(__NC_STACK_ypamissile *miss)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    int v90 = 0;

    vec3d v78(0.0, 0.0, 0.0);

    int v81 = 0;
    float v91 = 0.0;

    int a5 = miss->ejaculator_bact->self->getBACT_inputting();

    if ( !a5 )
        a5 = bact->self->getBACT_viewer();

    yw_130arg arg130;
    arg130.pos_x = bact->old_pos.x;
    arg130.pos_z = bact->old_pos.z;
    miss->ywo->ypaworld_func130(&arg130);

    cellArea *v68[3];

    v68[0] = arg130.pcell;

    arg130.pos_x = bact->position.x;
    arg130.pos_z = bact->position.z;
    miss->ywo->ypaworld_func130(&arg130);

    v68[2] = arg130.pcell;

    if ( arg130.pcell == v68[0] )
    {
        v68[1] = v68[0];
    }
    else
    {
        arg130.pos_x = (bact->position.x - bact->old_pos.x) * 0.5 + bact->old_pos.x;
        arg130.pos_z = (bact->position.z - bact->old_pos.z) * 0.5 + bact->old_pos.z;
        miss->ywo->ypaworld_func130(&arg130);

        v68[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || v68[i] != v68[i - 1] )
        {
            if (v68[i] == NULL)
                ypa_log_out("ypamissile_func70__sub0 NULL sector i = %d, 621: %f %f 62D: %f %f \n", i, bact->position.x, bact->position.z, bact->old_pos.x, bact->old_pos.z);

            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v68[ i ]->units_list.head;
            for (; bct->next ; bct = (__NC_STACK_ypabact *)bct->next)
            {
                if ( bct == bact || bct == miss->ejaculator_bact || bct->bact_type == BACT_TYPES_MISSLE || bct->status == BACT_STATUS_DEAD )
                    continue;

                if (bct->bact_type == BACT_TYPES_GUN && bct->shield >= 100)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );

                    if ( gun->getGUN_roboGun() )
                        continue;
                }

                if ( !a5 && bct->owner == miss->ejaculator_bact->owner )
                {
                    continue;
                }

                if (miss->ejaculator_bact->bact_type == BACT_TYPES_GUN)
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( miss->ejaculator_bact->self );

                    if (bct->owner == bact->owner)
                    {
                        if (gun->getGUN_roboGun())
                        {
                            if (bct->bact_type == BACT_TYPES_ROBO)
                                continue;

                            if (bct->bact_type == BACT_TYPES_GUN )
                            {
                                NC_STACK_ypagun *bgun = dynamic_cast<NC_STACK_ypagun *>( bct->self );

                                if (bgun->getGUN_roboGun())
                                    continue;
                            }
                        }
                    }
                }

                if ( miss->field_c == 1 && bct->position.y < miss->posy )
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

                        ttmp.x = bct->rotation.m00 * v8->coll_pos.x + bct->rotation.m10 * v8->coll_pos.y + bct->rotation.m20 * v8->coll_pos.z + bct->position.x;
                        ttmp.y = bct->rotation.m01 * v8->coll_pos.x + bct->rotation.m11 * v8->coll_pos.y + bct->rotation.m21 * v8->coll_pos.z + bct->position.y;
                        ttmp.z = bct->rotation.m02 * v8->coll_pos.x + bct->rotation.m12 * v8->coll_pos.y + bct->rotation.m22 * v8->coll_pos.z + bct->position.z;
                    }
                    else
                    {
                        ttmp = bct->position;
                        radius = bct->radius;
                    }

                    if ( !v82 || radius >= 0.01 )
                    {
                        vec3d to_enemy = ttmp - bact->old_pos;
                        vec3d dist_vect = bact->position - bact->old_pos;

                        if ( to_enemy.dot( bact->rotation.AxisZ() ) >= 0.3 )
                        {
                            float dist_vect_len;
                            vec3d dir_vect = dist_vect.normalise(&dist_vect_len);

                            vec3d vp = dir_vect * to_enemy;

                            float wpn_radius = 0.0;

                            switch ( bct->bact_type )
                            {
                            case BACT_TYPES_BACT:
                                wpn_radius = miss->radius_heli;
                                break;

                            case BACT_TYPES_TANK:
                            case BACT_TYPES_CAR:
                                wpn_radius = miss->radius_tank;
                                break;

                            case BACT_TYPES_FLYER:
                            case BACT_TYPES_UFO:
                                wpn_radius = miss->radius_flyer;
                                break;

                            case BACT_TYPES_ROBO:
                                wpn_radius = miss->radius_robo;
                                break;

                            default:
                                wpn_radius = bact->radius;
                                break;
                            }

                            if ( wpn_radius == 0.0)
                                wpn_radius = bact->radius;

                            float vp_len = vp.length();
                            float to_enemy_len = to_enemy.length();

                            if ( radius + wpn_radius > vp_len )
                            {
                                /*  Tube collision test, not cylinder!
                                    Will hit only when distance ~ wpn_radius */
                                if ( sqrt( POW2(dist_vect_len) + POW2(vp_len) ) > fabs(to_enemy_len - wpn_radius) )
                                {
                                    NC_STACK_ypabact *a1 = miss->ywo->getYW_userHostStation();

                                    __NC_STACK_ypabact *v85;
                                    v85 = a1->getBACT_pBact();

                                    v90 = 1;

                                    v91 += radius;
                                    v81++;

                                    bct->status_flg &= ~0x200;

                                    v78 += bct->position;

                                    int v83 = bct->self->getBACT_inputting();

                                    int v92 = 0;

                                    switch ( bct->bact_type )
                                    {
                                    case BACT_TYPES_BACT:
                                        v92 = bact->energy * miss->energy_heli;
                                        break;

                                    case BACT_TYPES_TANK:
                                    case BACT_TYPES_CAR:
                                        v92 = bact->energy * miss->energy_tank;
                                        break;

                                    case BACT_TYPES_FLYER:
                                    case BACT_TYPES_UFO:
                                        v92 = bact->energy * miss->energy_flyer;
                                        break;

                                    case BACT_TYPES_ROBO:
                                        v92 = bact->energy * miss->energy_robo;
                                        break;

                                    default:
                                        v92 = bact->energy;
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
                                        arg84.unit = miss->ejaculator_bact;

                                        if ( v85->owner == bact->owner || !miss->yw->isNetGame )
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

    if ( v90 )
    {
        float v48 = (float)v81;

        bact->position = v78 / v48;

        v91 *= v48;

        if ( v91 >= 50.0 )
        {
            vec3d v54 = bact->position - bact->old_pos;

            float v100 = v54.length();

            if ( v100 < 1.0 )
                v100 = 1.0;

            bact->position -= v54 * v91 / v100;
        }
    }

    return v90;
}

void ypamissile_func70__sub1(__NC_STACK_ypamissile *miss, move_msg *arg74)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->thraction = bact->force;

    arg74->vec = bact->fly_dir * bact->fly_dir_length * bact->airconst + bact->target_dir * bact->thraction - vec3d(0.0, bact->mass * 9.80665, 0.0);

    arg74->vec.normalise();
}

void NC_STACK_ypamissile::AI_layer3(update_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    miss->ywo->ypaworld_func145(bact);

    float v40 = bact->target_vec.length();

    if ( v40 > 0.1 )
    {
        if ( bact->primTtype != BACT_TGT_TYPE_DRCT )
            bact->target_dir = bact->target_vec / v40;
    }

    bact->AI_time1 = 0;

    bact->thraction = bact->force;

    float v38 = arg->frameTime * 0.001;

    if ( bact->status == BACT_STATUS_NORMAL )
    {
        if ( miss->field_2D & 2)
            miss->delay_time -= arg->frameTime;

        if ( miss->field_2D & 2  &&  miss->delay_time <= 0 )
        {
            ypamissile_func129(NULL);

            miss->selfie->status = BACT_STATUS_DEAD;

            setState_msg arg78;
            arg78.setFlags = BACT_STFLAG_DEATH2;
            arg78.unsetFlags = 0;
            arg78.newStatus = BACT_STATUS_NOPE;

            SetState(&arg78);

            if ( !(miss->field_2D & 4) || !miss->selfie->pSector->w_type )
            {
                if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->isNetGame )
                {
                    yw_arg129 v25;

                    v25.pos.x = bact->fly_dir.x * 5.0 + bact->position.x;
                    v25.pos.z = bact->fly_dir.z * 5.0 + bact->position.z;
                    v25.field_10 = bact->energy;
                    v25.unit = miss->ejaculator_bact;

                    ChangeSectorEnergy(&v25);
                }
            }
        }
        else
        {
            move_msg arg74;

            switch ( miss->field_c )
            {
            case 1:
                arg74.field_0 = v38;
                arg74.flag = 1;
                Move(&arg74);
                break;

            case 2:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                Move(&arg74);
                break;

            case 3:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                Move(&arg74);
                break;

            case 4:
                arg74.field_0 = v38;
                arg74.vec = bact->fly_dir;
                arg74.flag = 0;

                Move(&arg74);
                break;

            default:
                break;
            }

            if ( ypamissile_func70__sub0(miss) )
            {
                setState_msg arg78;
                ypamissile_func129(NULL);

                arg78.newStatus = BACT_STATUS_DEAD;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);

                ypamissile_func128(NULL);
            }
            else if ( miss->field_c != 6 )
            {
                ypaworld_arg136 arg136;
                arg136.stPos.x = bact->old_pos.x;
                arg136.stPos.y = bact->old_pos.y;
                arg136.stPos.z = bact->old_pos.z;
                arg136.vect.x = bact->position.x - bact->old_pos.x;
                arg136.vect.y = bact->position.y - bact->old_pos.y;
                arg136.vect.z = bact->position.z - bact->old_pos.z;
                arg136.flags = 0;

                miss->ywo->ypaworld_func136(&arg136);

                if ( arg136.isect )
                {
                    miss_arg130 arg131;
                    arg131.pos.x = arg136.skel->polygons[ arg136.polyID ].A;
                    arg131.pos.y = arg136.skel->polygons[ arg136.polyID ].B;
                    arg131.pos.z = arg136.skel->polygons[ arg136.polyID ].C;

                    ypamissile_func131(&arg131);


                    bact->position.x = arg136.isectPos.x;
                    bact->position.y = arg136.isectPos.y;
                    bact->position.z = arg136.isectPos.z;

                    ypamissile_func128(NULL);

                    miss->field_c = 6;
                    miss->field_2D |= 2;

                    if ( !miss->delay_time )
                    {
                        ypamissile_func129(NULL);

                        miss->selfie->status = BACT_STATUS_DEAD;

                        setState_msg arg78;
                        arg78.setFlags = BACT_STFLAG_DEATH2;
                        arg78.unsetFlags = 0;
                        arg78.newStatus = BACT_STATUS_NOPE;

                        SetState(&arg78);

                        if ( !(miss->field_2D & 4) || !miss->selfie->pSector->w_type )
                        {
                            if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->isNetGame )
                            {
                                yw_arg129 v25;

                                v25.pos.x = bact->fly_dir.x * 5.0 + bact->position.x;
                                v25.pos.z = bact->fly_dir.z * 5.0 + bact->position.z;
                                v25.field_10 = bact->energy;
                                v25.unit = miss->ejaculator_bact;

                                ChangeSectorEnergy(&v25);
                            }
                        }
                    }

                    int a4 = miss->ejaculator_bact->self->getBACT_inputting();

                    if ( a4 )
                    {
                        if ( miss->ejaculator_bact->host_station == miss->ejaculator_bact->parent_bacto )
                        {
                            if ( miss->ejaculator_bact->host_station )
                            {
                                setTarget_msg arg67;
                                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                                arg67.tgt_pos = bact->position;
                                arg67.priority = 0;

                                miss->ejaculator_bact->self->SetTarget(&arg67);
                            }
                        }
                    }
                }
                else
                {
                    miss->drive_time -= arg->frameTime;

                    if ( miss->drive_time < 0 )
                    {
                        miss->field_c = 1;

                        bact->airconst = 10.0;
                        bact->airconst_static = 10.0;
                    }

                    miss->life_time -= arg->frameTime;

                    if ( miss->life_time >= 0 )
                    {
                        miss_arg130 arg130;
                        arg130.period = arg->frameTime * 0.001;

                        ypamissile_func130(&arg130);
                    }
                    else
                    {
                        ypamissile_func129(NULL);

                        setState_msg arg78;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;
                        arg78.newStatus = BACT_STATUS_DEAD;

                        SetState(&arg78);

                        ypamissile_func128(NULL);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypamissile::User_layer(update_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->old_pos = bact->position;

    if (bact->status == BACT_STATUS_NORMAL)
        AI_layer1(arg);
    else
        ypamissile_func128(NULL);
}

void NC_STACK_ypamissile::Move(move_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->old_pos = bact->position;

    float v8;

    if ( bact->status != BACT_STATUS_DEAD && miss->field_c != 1 )
        v8 = bact->mass * 9.80665;
    else
        v8 = bact->mass * 39.2266;

    vec3d v26;
    float v35;

    if ( arg->flag & 1 )
    {
        v26.x = 0.0;
        v26.y = 0.0;
        v26.z = 0.0;

        v35 = 0.0;
    }
    else
    {
        v26 = arg->vec;

        v35 = bact->thraction;
    }

    vec3d vec1 = vec3d(0.0, v8, 0.0) + v26 * v35 - bact->fly_dir * (bact->fly_dir_length * bact->airconst);

    float v33 = vec1.normalise();

    if ( v33 > 0.0 )
    {
        vec3d v36 = bact->fly_dir * bact->fly_dir_length + vec1 * (v33 / bact->mass * arg->field_0);

        float v32 = v36.length();

        if ( v32 > 0.0 )
            v36 /= v32;

        bact->fly_dir = v36;

        bact->fly_dir_length = v32;
    }

    bact->position += bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);
}

void NC_STACK_ypamissile::SetState(setState_msg *arg)
{
    SetStateInternal(arg);
}

void NC_STACK_ypamissile::Renew()
{
    NC_STACK_ypabact::Renew();

    __NC_STACK_ypamissile *miss = &stack__ypamissile;

    miss->field_2D  = 0;
    miss->delay_time = 0;

    setBACT_yourLastSeconds(3000);
}

size_t NC_STACK_ypamissile::SetStateInternal(setState_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    sub_424000(&bact->soundcarrier, 2);
    sub_424000(&bact->soundcarrier, 0);
    sub_424000(&bact->soundcarrier, 1);

    if ( arg->newStatus )
        bact->status = arg->newStatus;

    if ( arg->setFlags )
        bact->status_flg |= arg->setFlags;

    if ( arg->unsetFlags )
        bact->status_flg &= ~arg->unsetFlags;

    if ( arg->newStatus == BACT_STATUS_DEAD )
    {
        setBACT_visProto(bact->vp_dead.base);
        setBACT_vpTransform(bact->vp_dead.trigo);

        startSound(&bact->soundcarrier, 2);

        StartDestFX(1);

        bact->fly_dir_length = 0;
    }

    if ( arg->newStatus == BACT_STATUS_NORMAL )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        startSound(&bact->soundcarrier, 0);
    }

    if ( arg->unsetFlags == BACT_STFLAG_DEATH2 )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        startSound(&bact->soundcarrier, 0);
    }

    if ( arg->setFlags == BACT_STFLAG_DEATH2 )
    {
        bact->status = BACT_STATUS_DEAD;

        setBACT_visProto(bact->vp_megadeth.base);
        setBACT_vpTransform(bact->vp_megadeth.trigo);

        startSound(&bact->soundcarrier, 2);

        StartDestFX(2);

        bact->fly_dir_length = 0;
    }

    return 1;
}

void NC_STACK_ypamissile::ypamissile_func128(void *)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;

    int a4 = getBACT_viewer();

    if ( a4 )
    {
        setBACT_viewer(0);
        setBACT_inputting(0);

        if ( miss->ejaculator_bact->status != BACT_STATUS_DEAD || (size_t)miss->ejaculator_bact->parent_bacto <= 3 )
        {
            miss->ejaculator_bact->self->setBACT_viewer(1);
            miss->ejaculator_bact->self->setBACT_inputting(1);
        }
        else
        {
            miss->ejaculator_bact->parent_bacto->setBACT_viewer(1);
            miss->ejaculator_bact->parent_bacto->setBACT_inputting(1);
        }

    }
}

void NC_STACK_ypamissile::ypamissile_func129(void *)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact_arg83 arg83;
    arg83.energ = bact->energy;
    arg83.pos = bact->position;
    arg83.pos2 = bact->fly_dir;
    arg83.force = bact->fly_dir_length;
    arg83.mass = bact->mass;

    float v16 = bact->fly_dir_length * bact->mass;

    if ( v16 > miss->yw->max_impulse && miss->yw->max_impulse > 0.0 )
    {
        float v7 = miss->yw->max_impulse / v16;
        arg83.force *= v7;
        arg83.mass *= v7;
    }

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)bact->pSector->units_list.head;

    while(bct->next)
    {
        if ( bct->bact_type != BACT_TYPES_MISSLE && bct->bact_type != BACT_TYPES_ROBO && bct->bact_type != BACT_TYPES_TANK && bct->bact_type != BACT_TYPES_CAR && bct->bact_type != BACT_TYPES_GUN && bct->bact_type != BACT_TYPES_HOVER && !(bct->status_flg & BACT_STFLAG_DEATH2) )
        {
            int v10 = 1;

            if ( miss->yw->isNetGame )
            {
                if ( bact->owner != bct->owner )
                    v10 = 0;
            }

            if ( v10 )
                bct->self->ApplyImpulse(&arg83);
        }

        bct = (__NC_STACK_ypabact *)bct->next;
    }

    if ( miss->yw->isNetGame )
    {
        uamessage_impulse impMsg;
        impMsg.msgID = UAMSG_IMPULSE;
        impMsg.owner = bact->owner;
        impMsg.id = bact->gid;
        impMsg.pos = bact->position;
        impMsg.impulse = bact->energy;
        impMsg.dir = bact->fly_dir;
        impMsg.dir_len = bact->fly_dir_length;
        impMsg.mass = bact->mass;

        yw_arg181 arg181;
        arg181.recvID = 0;
        arg181.dataSize = sizeof(impMsg);
        arg181.recvFlags = 2;
        arg181.garant = 1;
        arg181.data = &impMsg;

        miss->ywo->ypaworld_func181(&arg181);
    }
}

void NC_STACK_ypamissile::ypamissile_func130(miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( bact->fly_dir != vec3d(0.0, 0.0, 0.0) )
    {
        vec3d dir = bact->rotation.AxisZ(); // Get Z-axis, as dir
        vec3d u = dir * bact->fly_dir; // vector cross product

        float v37 = u.normalise(); // normalise and get length

        if ( v37 > 0.0 )
        {
            float v51 = dir.dot(bact->fly_dir); //scalar cross product

            if ( v51 > 1.0 )
                v51 = 1.0;

            if ( v51 < -1.0 )
                v51 = -1.0;

            float v52 = acos(v51);

            if ( miss->field_c == 1 )
            {
                if ( arg )
                {
                    float mxrot = bact->maxrot * arg->period;

                    if ( v52 < -mxrot )
                        v52 = -mxrot;

                    if ( v52 > mxrot )
                        v52 = mxrot;
                }
            }

            if ( fabs(v52) > MISSILE_BOMB_MIN_ANGLE )
                bact->rotation *= mat3x3::AxisAngle(u, v52);
        }

        if ( miss->field_2D & 1 )
        {
            float v53 = sqrt( POW2(bact->rotation.m00) + POW2(bact->rotation.m02) );

            if ( v53 > 1.0 )
                v53 = 1.0;

            if ( v53 < -1.0 )
                v53 = -1.0;

            float v44 = acos(v53);

            if ( miss->selfie->rotation.m11 < 0.0 )
                v44 = C_PI - v44;

            if ( miss->selfie->rotation.m01 < 0.0 )
                v44 = -v44;

            bact->rotation = mat3x3::RotateZ(-v44) * bact->rotation;
        }
    }
}

void NC_STACK_ypamissile::ypamissile_func131(miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    vec3d vec1 = bact->rotation.AxisY();
    vec3d vec2 = arg->pos;

    vec3d vaxis = vec1 * vec2;

    float v30 = vaxis.normalise();

    if ( v30 != 0.0 )
    {
        float v43 = vec1.dot(vec2);

        if ( v43 > 1.0 )
            v43 = 1.0;

        if ( v43 < -1.0 )
            v43 = -1.0;

        float v12 = acos(v43);

        if ( fabs(v12) > BACT_MIN_ANGLE )
            bact->rotation *= mat3x3::AxisAngle(vaxis, v12);
    }
}



void NC_STACK_ypamissile::setBACT_viewer(int vwr)
{
    NC_STACK_ypabact::setBACT_viewer(vwr);

    if ( vwr )
        stack__ypamissile.field_2D |= 1;
    else
        stack__ypamissile.field_2D &= ~1;
}

void NC_STACK_ypamissile::setMISS_launcher(__NC_STACK_ypabact *bact)
{
    stack__ypamissile.ejaculator_bact = bact;
}

void NC_STACK_ypamissile::setMISS_type(int tp)
{
    stack__ypamissile.field_c = tp;
}

void NC_STACK_ypamissile::setMISS_lifeTime(int time)
{
    stack__ypamissile.life_time = time;
}

void NC_STACK_ypamissile::setMISS_delay(int delay)
{
    stack__ypamissile.delay_time = delay;
}

void NC_STACK_ypamissile::setMISS_driveTime(int time)
{
    stack__ypamissile.drive_time = time;
}

void NC_STACK_ypamissile::setMISS_ignoreBuilds(int ign)
{
    if ( ign )
        stack__ypamissile.field_2D |= 4;
    else
        stack__ypamissile.field_2D &= ~4;
}

void NC_STACK_ypamissile::setMISS_powHeli(int po)
{
    stack__ypamissile.energy_heli = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powTank(int po)
{
    stack__ypamissile.energy_tank = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powFlyer(int po)
{
    stack__ypamissile.energy_flyer = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_powRobo(int po)
{
    stack__ypamissile.energy_robo = po * 0.001;
}

void NC_STACK_ypamissile::setMISS_radHeli(int rad)
{
    stack__ypamissile.radius_heli = rad;
}

void NC_STACK_ypamissile::setMISS_radTank(int rad)
{
    stack__ypamissile.radius_tank = rad;
}

void NC_STACK_ypamissile::setMISS_radFlyer(int rad)
{
    stack__ypamissile.radius_flyer = rad;
}

void NC_STACK_ypamissile::setMISS_radRobo(int rad)
{
    stack__ypamissile.radius_robo = rad;
}

void NC_STACK_ypamissile::setMISS_startHeight(int posy)
{
    stack__ypamissile.posy = posy;
}



__NC_STACK_ypabact *NC_STACK_ypamissile::getMISS_launcher()
{
    return stack__ypamissile.ejaculator_bact;
}

int NC_STACK_ypamissile::getMISS_type()
{
    return stack__ypamissile.field_c;
}

bact_node *NC_STACK_ypamissile::getMISS_pNode()
{
    return &stack__ypamissile.selfie_node;
}

int NC_STACK_ypamissile::getMISS_lifeTime()
{
    return stack__ypamissile.life_time;
}

int NC_STACK_ypamissile::getMISS_delay()
{
    return stack__ypamissile.delay_time;
}

int NC_STACK_ypamissile::getMISS_driveTime()
{
    return stack__ypamissile.drive_time;
}

int NC_STACK_ypamissile::getMISS_ignoreBuilds()
{
    return stack__ypamissile.field_2D & 4;
}

int NC_STACK_ypamissile::getMISS_powHeli()
{
    return stack__ypamissile.energy_heli * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powTank()
{
    return stack__ypamissile.energy_tank * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powFlyer()
{
    return stack__ypamissile.energy_flyer * 1000.0;
}

int NC_STACK_ypamissile::getMISS_powRobo()
{
    return stack__ypamissile.energy_robo * 1000.0;
}

int NC_STACK_ypamissile::getMISS_radHeli()
{
    return stack__ypamissile.radius_heli;
}

int NC_STACK_ypamissile::getMISS_radTank()
{
    return stack__ypamissile.radius_tank;
}

int NC_STACK_ypamissile::getMISS_radFlyer()
{
    return stack__ypamissile.radius_flyer;
}

int NC_STACK_ypamissile::getMISS_radRobo()
{
    return stack__ypamissile.radius_robo;
}

int NC_STACK_ypamissile::getMISS_startHeight()
{
    return stack__ypamissile.posy;
}


size_t NC_STACK_ypamissile::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( (IDVList *)data );
    case 3:
        return func3( (IDVList *)data );
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
        ypamissile_func128( (void *)data );
        return 1;
    case 129:
        ypamissile_func129( (void *)data );
        return 1;
    case 130:
        ypamissile_func130( (miss_arg130 *)data );
        return 1;
    case 131:
        ypamissile_func131( (miss_arg130 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
