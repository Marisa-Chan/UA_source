#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypamissile.h"
#include "yparobo.h"

#include "log.h"

#include <math.h>


const NewClassDescr NC_STACK_ypamissile::description("ypamissile.class", &newinstance);


int NC_STACK_ypamissile::ypamissile_func0__sub0(stack_vals *stak)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    miss->ejaculator_bact = 0;
    miss->life_time = 5000;
    miss->delay_time = 0;
    miss->field_c = 1;

    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BACT_ATT_WORLD:
                miss->ywo = (NC_STACK_ypaworld *)stk->value.p_data;
                miss->yw = &miss->ywo->stack__ypaworld;
                break;

            case BACT_ATT_VIEWER:
                setBACT_viewer(stk->value.i_data);
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher((__NC_STACK_ypabact *)stk->value.p_data);
                break;

            case MISS_ATT_TYPE:
                setMISS_type(stk->value.i_data);
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(stk->value.i_data);
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(stk->value.i_data);
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(stk->value.i_data);
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( stk->value.i_data );
                break;

            }
            stk++;
        }
    }

    return 1;
}


size_t NC_STACK_ypamissile::func0(stack_vals *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    __NC_STACK_ypamissile *miss = &stack__ypamissile;

    //bact = this->getBACT_pBact();
    __NC_STACK_ypabact *bact = &stack__ypabact;

    miss->selfie = bact;

    miss->selfie_node.bact = bact;
    miss->selfie_node.bacto = this;

    bact->bact_type = BACT_TYPES_MISSLE;

    if ( !ypamissile_func0__sub0(stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_ypamissile::func1(stack_vals *stak)
{
    return NC_STACK_ypabact::func1(stak);
}

void NC_STACK_ypamissile::ypamissile_func2__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BACT_ATT_VIEWER:
                setBACT_viewer(stk->value.i_data);
                break;

            case MISS_ATT_LAUNCHER:
                setMISS_launcher((__NC_STACK_ypabact *)stk->value.p_data);
                break;

            case MISS_ATT_TYPE:
                setMISS_type(stk->value.i_data);
                break;

            case MISS_ATT_LIFETIME:
                setMISS_lifeTime(stk->value.i_data);
                break;

            case MISS_ATT_DELAY:
                setMISS_delay(stk->value.i_data);
                break;

            case MISS_ATT_DRIVETIME:
                setMISS_driveTime(stk->value.i_data);
                break;

            case MISS_ATT_IGNOREBUILDS:
                setMISS_ignoreBuilds ( stk->value.i_data );
                break;

            case MISS_ATT_POW_HELI:
                setMISS_powHeli(stk->value.i_data);
                break;

            case MISS_ATT_POW_TANK:
                setMISS_powTank(stk->value.i_data);
                break;

            case MISS_ATT_POW_FLYER:
                setMISS_powFlyer(stk->value.i_data);
                break;

            case MISS_ATT_POW_ROBO:
                setMISS_powRobo(stk->value.i_data);
                break;

            case MISS_ATT_RAD_HELI:
                setMISS_radHeli(stk->value.i_data);
                break;

            case MISS_ATT_RAD_TANK:
                setMISS_radTank(stk->value.i_data);
                break;

            case MISS_ATT_RAD_FLYER:
                setMISS_radFlyer(stk->value.i_data);
                break;

            case MISS_ATT_RAD_ROBO:
                setMISS_radRobo(stk->value.i_data);
                break;

            case MISS_ATT_STHEIGHT:
                setMISS_startHeight(stk->value.i_data);
                break;

            }
            stk++;
        }
    }
}

size_t NC_STACK_ypamissile::func2(stack_vals *stak)
{
    NC_STACK_ypabact::func2(stak);

    ypamissile_func2__sub0(stak);
    return 1;
}

void NC_STACK_ypamissile::ypamissile_func3__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case MISS_ATT_LAUNCHER:
                *(__NC_STACK_ypabact **)stk->value.p_data = getMISS_launcher();
                break;

            case MISS_ATT_TYPE:
                *(int *)stk->value.p_data = getMISS_type();
                break;

            case MISS_ATT_PNODE:
                *(bact_node **)stk->value.p_data = getMISS_pNode();
                break;

            case MISS_ATT_LIFETIME:
                *(int *)stk->value.p_data = getMISS_lifeTime();
                break;

            case MISS_ATT_DELAY:
                *(int *)stk->value.p_data = getMISS_delay();
                break;

            case MISS_ATT_DRIVETIME:
                *(int *)stk->value.p_data = getMISS_driveTime();
                break;

            case MISS_ATT_IGNOREBUILDS:
                *(int *)stk->value.p_data = getMISS_ignoreBuilds();
                break;

            case MISS_ATT_POW_HELI:
                *(int *)stk->value.p_data = getMISS_powHeli();
                break;

            case MISS_ATT_POW_TANK:
                *(int *)stk->value.p_data = getMISS_powTank();
                break;

            case MISS_ATT_POW_FLYER:
                *(int *)stk->value.p_data = getMISS_powFlyer();
                break;

            case MISS_ATT_POW_ROBO:
                *(int *)stk->value.p_data = getMISS_powRobo();
                break;

            case MISS_ATT_RAD_HELI:
                *(int *)stk->value.p_data = getMISS_radHeli();
                break;

            case MISS_ATT_RAD_TANK:
                *(int *)stk->value.p_data = getMISS_radTank();
                break;

            case MISS_ATT_RAD_FLYER:
                *(int *)stk->value.p_data = getMISS_radFlyer();
                break;

            case MISS_ATT_RAD_ROBO:
                *(int *)stk->value.p_data = getMISS_radRobo();
                break;

            case MISS_ATT_STHEIGHT:
                *(int *)stk->value.p_data = getMISS_startHeight();
                break;

            }
            stk++;
        }
    }
}

size_t NC_STACK_ypamissile::func3(stack_vals *stak)
{
    NC_STACK_ypabact::func3(stak);

    ypamissile_func3__sub0(stak);
    return 1;
}

void NC_STACK_ypamissile::ypabact_func68(update_msg *arg)
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
            bact->target_vec.sx = bact->primT.pbact->position.sx - bact->position.sx;
            bact->target_vec.sy = bact->primT.pbact->position.sy - bact->position.sy;
            bact->target_vec.sz = bact->primT.pbact->position.sz - bact->position.sz;
        }
        else
        {
            bact->target_vec.sx = bact->primTpos.sx - bact->position.sx;
            bact->target_vec.sy = bact->primTpos.sy - bact->position.sy;
            bact->target_vec.sz = bact->primTpos.sz - bact->position.sz;
        }
    }

    ypabact_func69(arg);
}

void NC_STACK_ypamissile::ypabact_func69(update_msg *arg)
{
    ypabact_func70(arg);
}

int ypamissile_func70__sub0(__NC_STACK_ypamissile *miss)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    int v90 = 0;

    xyz v78(0.0, 0.0, 0.0);

    int v81 = 0;
    float v91 = 0.0;

    int a5 = miss->ejaculator_bact->self->getBACT_inputting();

    if ( !a5 )
        a5 = bact->self->getBACT_viewer();

    yw_130arg arg130;
    arg130.pos_x = bact->old_pos.sx;
    arg130.pos_z = bact->old_pos.sz;
    miss->ywo->ypaworld_func130(&arg130);

    cellArea *v68[3];

    v68[0] = arg130.pcell;

    arg130.pos_x = bact->position.sx;
    arg130.pos_z = bact->position.sz;
    miss->ywo->ypaworld_func130(&arg130);

    v68[2] = arg130.pcell;

    if ( arg130.pcell == v68[0] )
    {
        v68[1] = v68[0];
    }
    else
    {
        arg130.pos_x = (bact->position.sx - bact->old_pos.sx) * 0.5 + bact->old_pos.sx;
        arg130.pos_z = (bact->position.sz - bact->old_pos.sz) * 0.5 + bact->old_pos.sz;
        miss->ywo->ypaworld_func130(&arg130);

        v68[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || v68[i] != v68[i - 1] )
        {
            if (v68[i] == NULL)
                ypa_log_out("ypamissile_func70__sub0 NULL sector i = %d, 621: %f %f 62D: %f %f \n", i, bact->position.sx, bact->position.sz, bact->old_pos.sx, bact->old_pos.sz);

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

                if ( miss->field_c == 1 && bct->position.sy < miss->posy )
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
                    xyz ttmp;

                    if ( v82 )
                    {
                        roboColl *v8 = &v82->roboColls[j];
                        radius = v8->robo_coll_radius;

                        ttmp.sx = bct->rotation.m00 * v8->coll_pos.sx + bct->rotation.m10 * v8->coll_pos.sy + bct->rotation.m20 * v8->coll_pos.sz + bct->position.sx;
                        ttmp.sy = bct->rotation.m01 * v8->coll_pos.sx + bct->rotation.m11 * v8->coll_pos.sy + bct->rotation.m21 * v8->coll_pos.sz + bct->position.sy;
                        ttmp.sz = bct->rotation.m02 * v8->coll_pos.sx + bct->rotation.m12 * v8->coll_pos.sy + bct->rotation.m22 * v8->coll_pos.sz + bct->position.sz;
                    }
                    else
                    {
                        ttmp = bct->position;
                        radius = bct->radius;
                    }

                    if ( !v82 || radius >= 0.01 )
                    {
                        xyz to_enemy = ttmp - bact->old_pos;
                        xyz dist_vect = bact->position - bact->old_pos;

                        if ( to_enemy.dot( bact->rotation.getVect(2) ) >= 0.3 )
                        {
                            float dist_vect_len;
                            xyz dir_vect = dist_vect.normolize(&dist_vect_len);

                            xyz vp = dir_vect * to_enemy;

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

                                        if ( v85->owner == bact->owner || !miss->yw->field_757E )
                                            bct->self->ypabact_func84(&arg84);
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
            xyz v54 = bact->position - bact->old_pos;

            float v100 = v54.length();

            if ( v100 < 1.0 )
                v100 = 1.0;

            bact->position -= v54 * v91 / v100;
        }
    }

    return v90;
}

void ypamissile_func70__sub1(__NC_STACK_ypamissile *miss, bact_arg74 *arg74)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->thraction = bact->force;

    arg74->vec = bact->fly_dir * bact->fly_dir_length * bact->airconst + bact->target_dir * bact->thraction - xyz(0.0, bact->mass * 9.80665, 0.0);

    arg74->vec.normolize();
}

void NC_STACK_ypamissile::ypabact_func70(update_msg *arg)
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

            bact_arg119 arg78;
            arg78.field_4 = 2048;
            arg78.field_8 = 0;
            arg78.field_0 = 0;

            ypabact_func78(&arg78);

            if ( !(miss->field_2D & 4) || !miss->selfie->pSector->w_type )
            {
                if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                {
                    yw_arg129 v25;

                    v25.pos.sx = bact->fly_dir.sx * 5.0 + bact->position.sx;
                    v25.pos.sz = bact->fly_dir.sz * 5.0 + bact->position.sz;
                    v25.field_10 = bact->energy;
                    v25.unit = miss->ejaculator_bact;

                    ypabact_func120(&v25);
                }
            }
        }
        else
        {
            bact_arg74 arg74;

            switch ( miss->field_c )
            {
            case 1:
                arg74.field_0 = v38;
                arg74.flag = 1;
                ypabact_func74(&arg74);
                break;

            case 2:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                ypabact_func74(&arg74);
                break;

            case 3:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                ypabact_func74(&arg74);
                break;

            case 4:
                arg74.field_0 = v38;
                arg74.vec = bact->fly_dir;
                arg74.flag = 0;

                ypabact_func74(&arg74);
                break;

            default:
                break;
            }

            if ( ypamissile_func70__sub0(miss) )
            {
                bact_arg119 arg78;
                ypamissile_func129(NULL);

                arg78.field_0 = 2;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                ypabact_func78(&arg78);

                ypamissile_func128(NULL);
            }
            else if ( miss->field_c != 6 )
            {
                ypaworld_arg136 arg136;
                arg136.pos_x = bact->old_pos.sx;
                arg136.pos_y = bact->old_pos.sy;
                arg136.pos_z = bact->old_pos.sz;
                arg136.field_14 = bact->position.sx - bact->old_pos.sx;
                arg136.field_18 = bact->position.sy - bact->old_pos.sy;
                arg136.field_1C = bact->position.sz - bact->old_pos.sz;
                arg136.field_40 = 0;

                miss->ywo->ypaworld_func136(&arg136);

                if ( arg136.field_20 )
                {
                    miss_arg130 arg131;
                    arg131.pos.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
                    arg131.pos.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
                    arg131.pos.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;

                    ypamissile_func131(&arg131);


                    bact->position.sx = arg136.field_2C;
                    bact->position.sy = arg136.field_30;
                    bact->position.sz = arg136.field_34;

                    ypamissile_func128(NULL);

                    miss->field_c = 6;
                    miss->field_2D |= 2;

                    if ( !miss->delay_time )
                    {
                        ypamissile_func129(NULL);

                        miss->selfie->status = BACT_STATUS_DEAD;

                        bact_arg119 arg78;
                        arg78.field_4 = 2048;
                        arg78.field_8 = 0;
                        arg78.field_0 = 0;

                        ypabact_func78(&arg78);

                        if ( !(miss->field_2D & 4) || !miss->selfie->pSector->w_type )
                        {
                            if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                            {
                                yw_arg129 v25;

                                v25.pos.sx = bact->fly_dir.sx * 5.0 + bact->position.sx;
                                v25.pos.sz = bact->fly_dir.sz * 5.0 + bact->position.sz;
                                v25.field_10 = bact->energy;
                                v25.unit = miss->ejaculator_bact;

                                ypabact_func120(&v25);
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
                                bact_arg67 arg67;
                                arg67.tgt_type = BACT_TGT_TYPE_CELL;
                                arg67.tgt_pos = bact->position;
                                arg67.priority = 0;

                                miss->ejaculator_bact->self->ypabact_func67(&arg67);
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

                        bact_arg119 arg78;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;
                        arg78.field_0 = 2;

                        ypabact_func78(&arg78);

                        ypamissile_func128(NULL);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypamissile::ypabact_func71(update_msg *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->old_pos = bact->position;

    if (bact->status == BACT_STATUS_NORMAL)
        ypabact_func68(arg);
    else
        ypamissile_func128(NULL);
}

void NC_STACK_ypamissile::ypabact_func74(bact_arg74 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->old_pos = bact->position;

    float v8;

    if ( bact->status != BACT_STATUS_DEAD && miss->field_c != 1 )
        v8 = bact->mass * 9.80665;
    else
        v8 = bact->mass * 39.2266;

    xyz v26;
    float v35;

    if ( arg->flag & 1 )
    {
        v26.sx = 0.0;
        v26.sy = 0.0;
        v26.sz = 0.0;

        v35 = 0.0;
    }
    else
    {
        v26 = arg->vec;

        v35 = bact->thraction;
    }

    xyz vec1 = xyz(0.0, v8, 0.0) + v26 * v35 - bact->fly_dir * (bact->fly_dir_length * bact->airconst);

    float v33 = vec1.normolize();

    if ( v33 > 0.0 )
    {
        xyz v36 = bact->fly_dir * bact->fly_dir_length + vec1 * (v33 / bact->mass * arg->field_0);

        float v32 = v36.length();

        if ( v32 > 0.0 )
            v36 /= v32;

        bact->fly_dir = v36;

        bact->fly_dir_length = v32;
    }

    bact->position += bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

    ypabact_func115(NULL);
}

void NC_STACK_ypamissile::ypabact_func78(bact_arg119 *arg)
{
    ypabact_func119(arg);
}

void NC_STACK_ypamissile::ypabact_func96(void *)
{
    NC_STACK_ypabact::ypabact_func96(NULL);

    __NC_STACK_ypamissile *miss = &stack__ypamissile;

    miss->field_2D  = 0;
    miss->delay_time = 0;

    setBACT_yourLastSeconds(3000);
}

size_t NC_STACK_ypamissile::ypabact_func119(bact_arg119 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    sub_424000(&bact->soundcarrier, 2);
    sub_424000(&bact->soundcarrier, 0);
    sub_424000(&bact->soundcarrier, 1);

    if ( arg->field_0 )
        bact->status = arg->field_0;

    if ( arg->field_4 )
        bact->status_flg |= arg->field_4;

    if ( arg->field_8 )
        bact->status_flg &= ~arg->field_8;

    if ( arg->field_0 == 2 )
    {
        setBACT_visProto(bact->vp_dead.base);
        setBACT_vpTransform(bact->vp_dead.trigo);

        startSound(&bact->soundcarrier, 2);

        uint8_t v8 = 1;
        ypabact_func113(&v8);

        bact->fly_dir_length = 0;
    }

    if ( arg->field_0 == 1 )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        startSound(&bact->soundcarrier, 0);
    }

    if ( arg->field_8 == 2048 )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        startSound(&bact->soundcarrier, 0);
    }

    if ( arg->field_4 == 2048 )
    {
        bact->status = BACT_STATUS_DEAD;

        setBACT_visProto(bact->vp_megadeth.base);
        setBACT_vpTransform(bact->vp_megadeth.trigo);

        startSound(&bact->soundcarrier, 2);

        uint8_t v9 = 2;
        ypabact_func113(&v9);

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

    float v16 = miss->selfie->fly_dir_length * miss->selfie->mass;

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

            if ( miss->yw->field_757E )
            {
                if ( miss->selfie->owner != bct->owner )
                    v10 = 0;
            }

            if ( v10 )
                bct->self->ypabact_func83(&arg83);
        }

        bct = (__NC_STACK_ypabact *)bct->next;
    }

    if ( miss->yw->field_757E )
    {
        char v13[56];
//    *(_uint32_t *)v13 = 1026;
//    v13[12] = miss->selfie->owner;
//    *(_uint32_t *)&v13[16] = miss->selfie->ypabact__id;
//    v11 = (char *)&miss->selfie->field_621;
//    *(_uint32_t *)&v13[20] = *(_uint32_t *)v11;
//    v11 += 4;
//    *(_uint32_t *)&v13[24] = *(_uint32_t *)v11;
//    *(_uint32_t *)&v13[28] = *((_uint32_t *)v11 + 1);
//    *(_uint32_t *)&v13[32] = miss->selfie->energy;
//    v12 = (char *)&miss->selfie->field_605;
//    *(_uint32_t *)&v13[40] = *(_uint32_t *)v12;
//    v12 += 4;
//    *(_uint32_t *)&v13[44] = *(_uint32_t *)v12;
//    v12 += 4;
//    *(_uint32_t *)&v13[48] = *(_uint32_t *)v12;
//    *(_uint32_t *)&v13[52] = *((_uint32_t *)v12 + 1);
//    *(float *)&v13[36] = miss->selfie->mass;

        yw_arg181 arg181;
        arg181.field_10 = 0;
        arg181.val_size = 56;
        arg181.field_14 = 2;
        arg181.field_18 = 1;
        arg181.value = v13;

        miss->ywo->ypaworld_func181(&arg181);
    }
}

void NC_STACK_ypamissile::ypamissile_func130(miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( bact->fly_dir != xyz(0.0, 0.0, 0.0) )
    {
        xyz dir = bact->rotation.getVect(2); // Get Z-axis, as dir
        xyz u = dir * bact->fly_dir; // vector cross product

        float v37 = u.normolize(); // Normolize and get length

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

            if ( v52 > MISSILE_BOMB_MIN_ANGLE || v52 < -MISSILE_BOMB_MIN_ANGLE )
            {
                mat3x3 mat2;

                mat_gen_axis_rotate(&u, v52, &mat2, MAT_FLAG_INV_SIN);

                mat3x3 dst;

                mat_mult(&bact->rotation, &mat2, &dst);

                bact->rotation = dst;
            }
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
                v44 = 3.14159265358979323846 - v44;

            if ( miss->selfie->rotation.m01 < 0.0 )
                v44 = -v44;

            float v32 = cos(-v44);
            float v33 = sin(-v44);

            mat3x3 mat1;

            mat1.m00 = v32;
            mat1.m01 = v33;
            mat1.m02 = 0;
            mat1.m10 = -v33;
            mat1.m11 = v32;
            mat1.m12 = 0;
            mat1.m20 = 0;
            mat1.m21 = 0;
            mat1.m22 = 1.0;

            mat3x3 v23;

            mat_mult(&mat1, &bact->rotation, &v23);

            bact->rotation = v23;
        }
    }
}

void NC_STACK_ypamissile::ypamissile_func131(miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    xyz vec1 = bact->rotation.getVect(1);
    xyz vec2 = arg->pos;

    xyz vaxis = vec1 * vec2;

    float v30 = vaxis.normolize();

    if ( v30 != 0.0 )
    {
        float v43 = vec1.dot(vec2);

        if ( v43 > 1.0 )
            v43 = 1.0;

        if ( v43 < -1.0 )
            v43 = -1.0;

        float v12 = acos(v43);

        if ( fabs(v12) > BACT_MIN_ANGLE )
        {
            mat3x3 mat2;

            mat_gen_axis_rotate(&vaxis, v12, &mat2, MAT_FLAG_INV_SIN);

            mat3x3 v21;

            mat_mult(&bact->rotation, &mat2, &v21);

            bact->rotation = v21;
        }
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
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 68:
        ypabact_func68( (update_msg *)data );
        return 1;
    case 69:
        ypabact_func69( (update_msg *)data );
        return 1;
    case 70:
        ypabact_func70( (update_msg *)data );
        return 1;
    case 71:
        ypabact_func71( (update_msg *)data );
        return 1;
    case 74:
        ypabact_func74( (bact_arg74 *)data );
        return 1;
    case 78:
        ypabact_func78( (bact_arg119 *)data );
        return 1;
    case 96:
        ypabact_func96( (void *)data );
        return 1;
    case 119:
        return (size_t)ypabact_func119( (bact_arg119 *)data );
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
