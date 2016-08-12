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

    bact->field_24 = 4;

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

void NC_STACK_ypamissile::ypabact_func68(ypabact_arg65 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( miss->selfie->field_3D5 == 2 )
    {
        int a4 = getBACT_yourLastSeconds();

        a4 -= arg->field_4;

        setBACT_yourLastSeconds(a4);
    }

    if ( bact->primTtype )
    {
        if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
        {
            bact->field_639.sx = bact->primT.pbact->field_621.sx - bact->field_621.sx;
            bact->field_639.sy = bact->primT.pbact->field_621.sy - bact->field_621.sy;
            bact->field_639.sz = bact->primT.pbact->field_621.sz - bact->field_621.sz;
        }
        else
        {
            bact->field_639.sx = bact->primTpos.sx - bact->field_621.sx;
            bact->field_639.sy = bact->primTpos.sy - bact->field_621.sy;
            bact->field_639.sz = bact->primTpos.sz - bact->field_621.sz;
        }
    }

    ypabact_func69(arg);
}

void NC_STACK_ypamissile::ypabact_func69(ypabact_arg65 *arg)
{
    ypabact_func70(arg);
}

int ypamissile_func70__sub0(__NC_STACK_ypamissile *miss)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    int v90 = 0;

    xyz v78;
    v78.sx = 0;
    v78.sy = 0;
    v78.sz = 0;

    int v81 = 0;
    float v91 = 0.0;

    int a5 = miss->ejaculator_bact->self->getBACT_inputting();

    if ( !a5 )
        a5 = bact->self->getBACT_viewer();

    yw_130arg arg130;
    arg130.pos_x = bact->field_62D.sx;
    arg130.pos_z = bact->field_62D.sz;
    miss->ywo->ypaworld_func130(&arg130);

    cellArea *v68[3];

    v68[0] = arg130.pcell;

    arg130.pos_x = bact->field_621.sx;
    arg130.pos_z = bact->field_621.sz;
    miss->ywo->ypaworld_func130(&arg130);

    v68[2] = arg130.pcell;

    if ( arg130.pcell == v68[0] )
    {
        v68[1] = v68[0];
    }
    else
    {
        arg130.pos_x = (bact->field_621.sx - bact->field_62D.sx) * 0.5 + bact->field_62D.sx;
        arg130.pos_z = (bact->field_621.sz - bact->field_62D.sz) * 0.5 + bact->field_62D.sz;
        miss->ywo->ypaworld_func130(&arg130);

        v68[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || v68[i] != v68[i - 1] )
        {
            if (v68[i] == NULL)
                ypa_log_out("ypamissile_func70__sub0 NULL sector i = %d, 621: %f %f 62D: %f %f \n", i, bact->field_621.sx, bact->field_621.sz, bact->field_62D.sx, bact->field_62D.sz);

            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v68[ i ]->units_list.head;
            while (bct->next)
            {
                if ( bct != bact && bct != miss->ejaculator_bact )
                {
                    if ( bct->field_24 != 4 && bct->field_3D5 != 2 )
                    {
                        int a4 = 0;
                        if (bct->field_24 == 9)
                        {
                            NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct->self );
                            a4 = gun->getGUN_roboGun();
                        }

                        if ( bct->field_24 != 9 || bct->shield > 100 || !a4 )
                        {
                            if ( a5 || bct->owner != miss->ejaculator_bact->owner )
                            {
                                int v99 = 0;

                                if (miss->ejaculator_bact->field_24 == 9)
                                {
                                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( miss->ejaculator_bact->self );
                                    v99 = gun->getGUN_roboGun();
                                }

                                if ( miss->ejaculator_bact->field_24 != 9
                                        || bct->owner != bact->owner
                                        || !v99
                                        || (bct->field_24 != 3 && (bct->field_24 != 9 || !a4) ) )
                                {

                                    if ( miss->field_c != 1 || bct->field_621.sy >= miss->posy )
                                    {
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

                                                ttmp.sx = bct->field_651.m00 * v8->robo_coll_x + bct->field_651.m10 * v8->robo_coll_y + bct->field_651.m20 * v8->robo_coll_z + bct->field_621.sx;
                                                ttmp.sy = bct->field_651.m01 * v8->robo_coll_x + bct->field_651.m11 * v8->robo_coll_y + bct->field_651.m21 * v8->robo_coll_z + bct->field_621.sy;
                                                ttmp.sz = bct->field_651.m02 * v8->robo_coll_x + bct->field_651.m12 * v8->robo_coll_y + bct->field_651.m22 * v8->robo_coll_z + bct->field_621.sz;
                                            }
                                            else
                                            {
                                                ttmp = bct->field_621;
                                                radius = bct->radius;
                                            }

                                            if ( !v82 || radius >= 0.01 )
                                            {
                                                float v72 = ttmp.sx - bact->field_62D.sx;
                                                float v73 = ttmp.sy - bact->field_62D.sy;
                                                float v74 = ttmp.sz - bact->field_62D.sz;

                                                float v69 = bact->field_621.sx - bact->field_62D.sx;
                                                float v70 = bact->field_621.sy - bact->field_62D.sy;
                                                float v71 = bact->field_621.sz - bact->field_62D.sz;


                                                if ( v72 * bact->field_651.m20 + v73 * bact->field_651.m21 + v74 * bact->field_651.m22 >= 0.3 )
                                                {
                                                    float v95 = sqrt( POW2(v70) + POW2(v69) + POW2(v71) );

                                                    NDIV_CARRY(v95);

                                                    float v65 = v69 / v95;
                                                    float v66 = v70 / v95;
                                                    float v67 = v71 / v95;

                                                    float v64 = v65 * v73 - v72 * v66;
                                                    float v62 = v66 * v74 - v73 * v67;
                                                    float v63 = v67 * v72 - v74 * v65;

                                                    float v93 = sqrt( POW2(v73) + POW2(v72) + POW2(v74) );

                                                    float v29 = 0.0;

                                                    switch ( bct->field_24 )
                                                    {
                                                    case 1:
                                                        v29 = miss->radius_heli;
                                                        break;

                                                    case 2:
                                                    case 8:
                                                        v29 = miss->radius_tank;
                                                        break;

                                                    case 6:
                                                    case 7:
                                                        v29 = miss->radius_flyer;
                                                        break;

                                                    case 3:
                                                        v29 = miss->radius_robo;
                                                        break;

                                                    default:
                                                        v29 = bact->radius;
                                                        break;
                                                    }

                                                    if ( v29 == 0.0)
                                                        v29 = bact->radius;

                                                    float v96 = sqrt( POW2(v63) + POW2(v62) + POW2(v64) );

                                                    if ( radius + v29 > v96 )
                                                    {
                                                        if ( sqrt( POW2(v95) + POW2(v96) ) > fabs(v93 - v29) )
                                                        {
                                                            NC_STACK_ypabact *a1 = miss->ywo->getYW_userHostStation();

                                                            __NC_STACK_ypabact *v85;
                                                            v85 = a1->getBACT_pBact();

                                                            v90 = 1;

                                                            v91 += radius;
                                                            v81++;

                                                            bct->field_3D6 &= 0xFFFFFDFF;

                                                            v78.sx += bct->field_621.sx;
                                                            v78.sy += bct->field_621.sy;
                                                            v78.sz += bct->field_621.sz;

                                                            int v83 = bct->self->getBACT_inputting();

                                                            int v92 = 0;

                                                            switch ( bct->field_24 )
                                                            {
                                                            case 1:
                                                                v92 = bact->energy * miss->energy_heli;
                                                                break;

                                                            case 2:
                                                            case 8:
                                                                v92 = bact->energy * miss->energy_tank;
                                                                break;

                                                            case 6:
                                                            case 7:
                                                                v92 = bact->energy * miss->energy_flyer;
                                                                break;

                                                            case 3:
                                                                v92 = bact->energy * miss->energy_robo;
                                                                break;

                                                            default:
                                                                v92 = bact->energy;
                                                                break;
                                                            }

                                                            float v46;
                                                            float v47;

                                                            if ( v83 || bct->field_3D6 & 0x800000 )
                                                            {
                                                                v46 = v92 * (100 - bct->shield);
                                                                v47 = 250;
                                                            }
                                                            else
                                                            {
                                                                v46 = v92 * (100 - bct->shield);
                                                                v47 = 100.0;
                                                            }

                                                            v92 = ceil(v46 / v47); //Misslie damage (ceil for less damage)
                                                            if ( v92 )
                                                            {
                                                                bact_arg84 arg84;
                                                                arg84.energy = -v92;
                                                                arg84.unit = miss->ejaculator_bact;

                                                                if ( v85->owner == bact->owner || !miss->yw->field_757E )
                                                                    bct->self->ypabact_func84(&arg84);
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }

                                    }

                                }
                            }
                        }
                    }
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    if ( v90 )
    {
        float v48 = (float)v81;

        bact->field_621.sx = v78.sx / v48;
        bact->field_621.sy = v78.sy / v48;
        bact->field_621.sz = v78.sz / v48;

        v91 *= v48;

        if ( v91 >= 50.0 )
        {
            float v54 = bact->field_621.sx - bact->field_62D.sx;
            float v55 = bact->field_621.sy - bact->field_62D.sy;
            float v57 = bact->field_621.sz - bact->field_62D.sz;


            float v100 = sqrt( POW2(v54) + POW2(v55) + POW2(v57) );

            if ( v100 < 1.0 )
                v100 = 1.0;

            bact->field_621.sx -= (bact->field_621.sx - bact->field_62D.sx) * v91 / v100;
            bact->field_621.sy -= (bact->field_621.sy - bact->field_62D.sy) * v91 / v100;
            bact->field_621.sz -= (bact->field_621.sz - bact->field_62D.sz) * v91 / v100;
        }
    }

    return v90;
}

void ypamissile_func70__sub1(__NC_STACK_ypamissile *miss, bact_arg74 *arg74)
{
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->field_601 = bact->force;

    arg74->vec.sx = bact->field_605.sx * bact->field_611 * bact->airconst + bact->field_601 * bact->field_645.sx;
    arg74->vec.sy = bact->field_605.sy * bact->field_611 * bact->airconst + bact->field_601 * bact->field_645.sy - bact->mass * 9.80665;
    arg74->vec.sz = bact->field_605.sz * bact->field_611 * bact->airconst + bact->field_601 * bact->field_645.sz;

    float v13 = sqrt( POW2(arg74->vec.sx) + POW2(arg74->vec.sy) + POW2(arg74->vec.sz) );

    if ( v13 > 0.0 )
    {
        arg74->vec.sx /= v13;
        arg74->vec.sy /= v13;
        arg74->vec.sz /= v13;
    }
}

void NC_STACK_ypamissile::ypabact_func70(ypabact_arg65 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    miss->ywo->ypaworld_func145(bact);

    float v40 = sqrt( POW2(bact->field_639.sx) + POW2(bact->field_639.sy) + POW2(bact->field_639.sz) );

    if ( v40 > 0.1 )
    {
        if ( bact->primTtype != BACT_TGT_TYPE_DRCT )
        {
            bact->field_645.sx = bact->field_639.sx / v40;
            bact->field_645.sy = bact->field_639.sy / v40;
            bact->field_645.sz = bact->field_639.sz / v40;
        }
    }

    bact->field_919 = 0;

    bact->field_601 = bact->force;

    float v38 = arg->field_4 * 0.001;

    if ( bact->field_3D5 == 1 )
    {
        if ( miss->field_2D & 2)
            miss->delay_time -= arg->field_4;

        if ( miss->field_2D & 2  &&  miss->delay_time <= 0 )
        {
            ypamissile_func129(NULL);

            miss->selfie->field_3D5 = 2;

            bact_arg119 arg78;
            arg78.field_4 = 2048;
            arg78.field_8 = 0;
            arg78.field_0 = 0;

            ypabact_func78(&arg78);

            if ( !(miss->field_2D & 4) || !miss->selfie->p_cell_area->w_type )
            {
                if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                {
                    yw_arg129 v25;

                    v25.pos.sx = bact->field_605.sx * 5.0 + bact->field_621.sx;
                    v25.pos.sz = bact->field_605.sz * 5.0 + bact->field_621.sz;
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
                arg74.vec = bact->field_605;
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
                arg136.pos_x = bact->field_62D.sx;
                arg136.pos_y = bact->field_62D.sy;
                arg136.pos_z = bact->field_62D.sz;
                arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
                arg136.field_18 = bact->field_621.sy - bact->field_62D.sy;
                arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;
                arg136.field_40 = 0;

                miss->ywo->ypaworld_func136(&arg136);

                if ( arg136.field_20 )
                {
                    miss_arg130 arg131;
                    arg131.pos.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
                    arg131.pos.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
                    arg131.pos.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;

                    ypamissile_func131(&arg131);


                    bact->field_621.sx = arg136.field_2C;
                    bact->field_621.sy = arg136.field_30;
                    bact->field_621.sz = arg136.field_34;

                    ypamissile_func128(NULL);

                    miss->field_c = 6;
                    miss->field_2D |= 2;

                    if ( !miss->delay_time )
                    {
                        ypamissile_func129(NULL);

                        miss->selfie->field_3D5 = 2;

                        bact_arg119 arg78;
                        arg78.field_4 = 2048;
                        arg78.field_8 = 0;
                        arg78.field_0 = 0;

                        ypabact_func78(&arg78);

                        if ( !(miss->field_2D & 4) || !miss->selfie->p_cell_area->w_type )
                        {
                            if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                            {
                                yw_arg129 v25;

                                v25.pos.sx = bact->field_605.sx * 5.0 + bact->field_621.sx;
                                v25.pos.sz = bact->field_605.sz * 5.0 + bact->field_621.sz;
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
                                arg67.tgt_pos = bact->field_621;
                                arg67.priority = 0;

                                miss->ejaculator_bact->self->ypabact_func67(&arg67);
                            }
                        }
                    }
                }
                else
                {
                    miss->drive_time -= arg->field_4;

                    if ( miss->drive_time < 0 )
                    {
                        miss->field_c = 1;

                        bact->airconst = 10.0;
                        bact->airconst2 = 10.0;
                    }

                    miss->life_time -= arg->field_4;

                    if ( miss->life_time >= 0 )
                    {
                        miss_arg130 arg130;
                        arg130.period = arg->field_4 * 0.001;

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

void NC_STACK_ypamissile::ypabact_func71(ypabact_arg65 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->field_62D = bact->field_621;

    if (bact->field_3D5 == 1)
        ypabact_func68(arg);
    else
        ypamissile_func128(NULL);
}

void NC_STACK_ypamissile::ypabact_func74(bact_arg74 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->field_62D = bact->field_621;

    float v8;

    if ( bact->field_3D5 != 2 && miss->field_c != 1 )
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

        v35 = bact->field_601;
    }

    float v12 = bact->field_611 * bact->airconst;

    float v29 = 0.0 * v8 + v26.sx * v35 + -bact->field_605.sx * v12;
    float v30 = 1.0 * v8 + v26.sy * v35 + -bact->field_605.sy * v12;
    float v31 = 0.0 * v8 + v26.sz * v35 + -bact->field_605.sz * v12;

    float v33 = sqrt( POW2(v30) + POW2(v29) + POW2(v31) );

    if ( v33 > 0.0 )
    {
        float v20 = v33 / bact->mass * arg->field_0;

        xyz v36;
        v36.sx = bact->field_605.sx * bact->field_611 + v29 * v20 / v33;
        v36.sy = bact->field_605.sy * bact->field_611 + v30 * v20 / v33;
        v36.sz = bact->field_605.sz * bact->field_611 + v31 * v20 / v33;

        float v32 = sqrt(POW2(v36.sx) + POW2(v36.sy) + POW2(v36.sz));

        if ( v32 > 0.0 )
        {
            v36.sx /= v32;
            v36.sy /= v32;
            v36.sz /= v32;
        }

        bact->field_605 = v36;

        bact->field_611 = v32;
    }

    bact->field_621.sx += bact->field_605.sx * bact->field_611 * arg->field_0 * 6.0;
    bact->field_621.sy += bact->field_605.sy * bact->field_611 * arg->field_0 * 6.0;
    bact->field_621.sz += bact->field_605.sz * bact->field_611 * arg->field_0 * 6.0;

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

    sub_424000(&bact->field_5A, 2);
    sub_424000(&bact->field_5A, 0);
    sub_424000(&bact->field_5A, 1);

    if ( arg->field_0 )
        bact->field_3D5 = arg->field_0;

    if ( arg->field_4 )
        bact->field_3D6 |= arg->field_4;

    if ( arg->field_8 )
        bact->field_3D6 &= ~arg->field_8;

    if ( arg->field_0 == 2 )
    {
        setBACT_visProto(bact->vp_dead.base);
        setBACT_vpTransform(bact->vp_dead.trigo);

        sub_423F74(&bact->field_5A, 2);

        uint8_t v8 = 1;
        ypabact_func113(&v8);

        bact->field_611 = 0;
    }

    if ( arg->field_0 == 1 )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        sub_423F74(&bact->field_5A, 0);
    }

    if ( arg->field_8 == 2048 )
    {
        setBACT_visProto(bact->vp_normal.base);
        setBACT_vpTransform(bact->vp_normal.trigo);

        sub_423F74(&bact->field_5A, 0);
    }

    if ( arg->field_4 == 2048 )
    {
        bact->field_3D5 = 2;

        setBACT_visProto(bact->vp_megadeth.base);
        setBACT_vpTransform(bact->vp_megadeth.trigo);

        sub_423F74(&bact->field_5A, 2);

        uint8_t v9 = 2;
        ypabact_func113(&v9);

        bact->field_611 = 0;
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

        if ( miss->ejaculator_bact->field_3D5 != 2 || (size_t)miss->ejaculator_bact->parent_bacto <= 3 )
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
    arg83.pos = bact->field_621;
    arg83.pos2 = bact->field_605;
    arg83.force = bact->field_611;
    arg83.mass = bact->mass;

    float v16 = miss->selfie->field_611 * miss->selfie->mass;

    if ( v16 > miss->yw->max_impulse && miss->yw->max_impulse > 0.0 )
    {
        float v7 = miss->yw->max_impulse / v16;
        arg83.force *= v7;
        arg83.mass *= v7;
    }

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)bact->p_cell_area->units_list.head;

    while(bct->next)
    {
        if ( bct->field_24 != 4 && bct->field_24 != 3 && bct->field_24 != 2 && bct->field_24 != 8 && bct->field_24 != 9 && bct->field_24 != 10 && !(bct->field_3D6 & 0x800) )
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
//    *(_DWORD *)v13 = 1026;
//    v13[12] = miss->selfie->owner;
//    *(_DWORD *)&v13[16] = miss->selfie->ypabact__id;
//    v11 = (char *)&miss->selfie->field_621;
//    *(_DWORD *)&v13[20] = *(_DWORD *)v11;
//    v11 += 4;
//    *(_DWORD *)&v13[24] = *(_DWORD *)v11;
//    *(_DWORD *)&v13[28] = *((_DWORD *)v11 + 1);
//    *(_DWORD *)&v13[32] = miss->selfie->energy;
//    v12 = (char *)&miss->selfie->field_605;
//    *(_DWORD *)&v13[40] = *(_DWORD *)v12;
//    v12 += 4;
//    *(_DWORD *)&v13[44] = *(_DWORD *)v12;
//    v12 += 4;
//    *(_DWORD *)&v13[48] = *(_DWORD *)v12;
//    *(_DWORD *)&v13[52] = *((_DWORD *)v12 + 1);
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

    if ( bact->field_605.sx != 0.0 || bact->field_605.sy != 0.0 || bact->field_605.sz != 0.0 )
    {
        xyz u;
        u.sx = bact->field_651.m21 * bact->field_605.sz - bact->field_605.sy * bact->field_651.m22;
        u.sy = bact->field_651.m22 * bact->field_605.sx - bact->field_605.sz * bact->field_651.m20;
        u.sz = bact->field_651.m20 * bact->field_605.sy - bact->field_651.m21 * bact->field_605.sx;

        float v37 = sqrt(POW2(u.sx) + POW2(u.sy) + POW2(u.sz));

        if ( v37 > 0.0 )
        {
            float v51 = bact->field_651.m20 * bact->field_605.sx + bact->field_651.m21 * bact->field_605.sy + bact->field_651.m22 * bact->field_605.sz;

            u.sx /= v37;
            u.sy /= v37;
            u.sz /= v37;

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

                mat_mult(&bact->field_651, &mat2, &dst);

                bact->field_651 = dst;
            }
        }

        if ( miss->field_2D & 1 )
        {
            float v53 = sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m02) );

            if ( v53 > 1.0 )
                v53 = 1.0;

            if ( v53 < -1.0 )
                v53 = -1.0;

            float v44 = acos(v53);

            if ( miss->selfie->field_651.m11 < 0.0 )
                v44 = 3.1415926 - v44;

            if ( miss->selfie->field_651.m01 < 0.0 )
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

            mat_mult(&mat1, &bact->field_651, &v23);

            bact->field_651 = v23;
        }
    }
}

void NC_STACK_ypamissile::ypamissile_func131(miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    float v33 = bact->field_651.m10;
    float v5 = bact->field_651.m11;
    float v36 = bact->field_651.m12;

    float v35 = arg->pos.sx;
    float v34 = arg->pos.sy;
    float v6 = arg->pos.sz;

    xyz vaxis;
    vaxis.sx = v5 * v6 - v36 * v34;
    vaxis.sy = v35 * v36 - v33 * v6;
    vaxis.sz = v33 * v34 - v5 * v35;

    float v30 = sqrt( POW2(vaxis.sx) + POW2(vaxis.sy) + POW2(vaxis.sz) );

    if ( v30 != 0.0 )
    {
        float v43 = v33 * v35 + v5 * v34 + v36 * v6;

        vaxis.sx /= v30;
        vaxis.sy /= v30;
        vaxis.sz /= v30;

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

            mat_mult(&bact->field_651, &mat2, &v21);

            bact->field_651 = v21;
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
        ypabact_func68( (ypabact_arg65 *)data );
        return 1;
    case 69:
        ypabact_func69( (ypabact_arg65 *)data );
        return 1;
    case 70:
        ypabact_func70( (ypabact_arg65 *)data );
        return 1;
    case 71:
        ypabact_func71( (ypabact_arg65 *)data );
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
