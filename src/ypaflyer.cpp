#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypaflyer.h"
#include "log.h"
#include <math.h>


const NewClassDescr NC_STACK_ypaflyer::description("ypaflyer.class", &newinstance);

size_t NC_STACK_ypaflyer::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

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
                    stack__ypaflyer.ywo = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__ypaflyer.yw = &stack__ypaflyer.ywo->stack__ypaworld;
                    break;

                case FLY_ATT_TYPE:
                    setFLY_type(val.value.i_data);
                    break;
                default:
                    break;
                }
            }
        }
    }

    __NC_STACK_ypabact *bact = &ypabact;

    stack__ypaflyer.bact_internal = bact;

    bact->bact_type = BACT_TYPES_FLYER;

    stack__ypaflyer.field_c = 0;
    stack__ypaflyer.field_10 = 0;

    return 1;
}

size_t NC_STACK_ypaflyer::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypaflyer::func2(IDVList *stak)
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
                case FLY_ATT_TYPE:
                    setFLY_type(val.value.i_data);
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypaflyer::func3(IDVList *stak)
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
                case FLY_ATT_TYPE:
                    *(int *)val.value.p_data = getFLY_type();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

void sb_0x4b255c(__NC_STACK_ypaflyer *fly, float a2, vec3d v, int a4)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    float v38 = bact->force / bact->airconst;

    if ( bact->fly_dir.x != 0.0 || bact->fly_dir.y != 0.0 || bact->fly_dir.z != 0.0 )
    {
        if ( v.x == 0.0 && v.y == 0.0 && v.z == 0.0 )
            v = bact->rotation.AxisZ();

        vec2d tmp = bact->rotation.AxisZ().XZ();

        float v56 = clp_asin( bact->rotation.m01 );
        float v52 = clp_asin( bact->rotation.m21 );

        float v49 = 0.0;

        if ( fly->field_10 & 1 && bact->fly_dir_length >= 0.1 )
            v49 = clp_asin( bact->fly_dir.y ) * bact->fly_dir_length / v38;

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
            float maxrot = bact->maxrot * a2 * v45;

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
            float maxrot = -bact->maxrot * a2 * v45;
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

        vec3d vaxis = bact->rotation.AxisX().X0Z();

        if ( vaxis.normalise() >= 0.001 )
            bact->rotation *= mat3x3::AxisAngle(vaxis, v51);

        if ( fly->field_10 & 2 )
        {
            if ( (tmp.x != 0.0 || tmp.y != 0.0) && (v.x != 0.0 || v.z != 0.0) )
            {
                float v57 = clp_acos( tmp.dot( v.XZ() ) / tmp.length() / v.XZ().length() );

                if ( v57 < 0.001 )
                    v57 = 0.0;

                if ( tmp.cross( v.XZ() ) < 0.0 )
                    v57 = -v57;

                float v58 = 0.0;

                if ( bact->fly_dir_length >= 0.1 )
                {
                    float aa2 = a2;

                    NDIV_CARRY(aa2);

                    v58 = fabs(bact->fly_dir_length) * bact->airconst / bact->force * (v57 * 1.45 / (aa2 * bact->maxrot));
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
                    float v40 = bact->maxrot * a2 * v44;

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
                    float v43 = -bact->maxrot * a2 * v44;
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

                bact->rotation = mat3x3::RotateZ(v50) * bact->rotation;
            }
        }
        else
        {
            if ( v56 >= 0.0 )
            {
                float v41 = bact->maxrot * a2 * v45;

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
                float v39 = -bact->maxrot * a2 * v45;

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

            bact->rotation = mat3x3::RotateZ(-v56) * bact->rotation;
        }
    }
}


void ypaflyer_func70__sub0(__NC_STACK_ypaflyer *fly, float angl)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    if ( bact->target_dir.x != 0.0 || bact->target_dir.z != 0.0 )
    {
        float v21 = clp_acos( bact->target_dir.XZ().dot( bact->rotation.AxisZ().XZ() ) / bact->target_dir.XZ().length() / bact->rotation.AxisZ().XZ().length() );

        float v9 = bact->maxrot * angl;

        if ( fabs(v21) > v9 )
            v21 = v9;

        if ( bact->target_dir.XZ().cross( bact->rotation.AxisZ().XZ() ) > 0.0 )
            v21 = -v21;

        bact->rotation = mat3x3::RotateY(v21) * bact->rotation;
    }

    bact->thraction = bact->force;

    float v14;

    if ( fabs(bact->target_dir.y) >= 0.1 )
    {
        if ( bact->target_dir.y > -0.1 )
            v14 = bact->mass * 0.7;
        else
            v14 = bact->mass * 2.5;
    }
    else
        v14 = bact->mass;

    fly->field_c = v14 * 9.80665;

    if ( bact->status_flg & BACT_STFLAG_UPWRD )
        fly->field_c = bact->mass * 2.5 * 9.80665;
}



void NC_STACK_ypaflyer::AI_layer3(update_msg *arg)
{
    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;
    __NC_STACK_ypabact *bact = fly->bact_internal;

    float a2a = arg->frameTime / 1000.0;

    float v88 = bact->target_vec.length();

    if ( v88 > 0.0 )
        bact->target_dir = bact->target_vec / v88;

    int a4 = getBACT_viewer();
    int v82 = getBACT_exactCollisions();
    int v79 = getBACT_bactCollisions();

    int v90 = fly->ywo->ypaworld_func145(bact);

    float v91;

    if ( a4 )
        v91 = bact->viewer_radius;
    else
        v91 = bact->radius;

    switch ( bact->status )
    {
    case BACT_STATUS_NORMAL:
    {
        vec3d a3 = bact->rotation.AxisZ();

        if ( v79 )
        {
            if ( v90 || (!bact->secndTtype && v88 < 1200.0) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( !bact->primTtype && !bact->secndTtype )
        {
            bact->status = BACT_STATUS_IDLE;

            if ( bact->status_flg & BACT_STFLAG_FIRE )
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
        arg136.stPos = bact->old_pos;
        arg136.vect = (bact->position - bact->old_pos).X0Z();

        float v89 = arg136.vect.XZ().length();

        if ( v89 <= 0.0 )
            arg136.vect = bact->rotation.AxisZ() * 300.0;
        else
            arg136.vect *= 300.0 / v89;

        ypaworld_arg136 arg136_1;

        arg136_1.isect = 0;
        arg136_1.flags = 0;

        if ( a4 || bact->status_flg & BACT_STFLAG_DODGE_RIGHT || (v90 && v82) )
        {
            arg136_1.stPos = bact->old_pos;

            arg136_1.vect.x = arg136.vect.x * 0.93969 - arg136.vect.z * 0.34202;
            arg136_1.vect.y = arg136.vect.y;
            arg136_1.vect.z = arg136.vect.z * 0.93969 + arg136.vect.x * 0.34202;

            fly->ywo->ypaworld_func136(&arg136_1);
        }

        ypaworld_arg136 arg136_2;

        arg136_2.isect = 0;
        arg136_2.flags = 0;

        if ( a4 || bact->status_flg & BACT_STFLAG_DODGE_LEFT || (v90 && v82) )
        {
            arg136_2.stPos = bact->old_pos;
            arg136_2.vect.x = arg136.vect.x * 0.93969 + arg136.vect.z * 0.34202;
            arg136_2.vect.y = arg136.vect.y;
            arg136_2.vect.z = arg136.vect.z * 0.93969 - arg136.vect.x * 0.34202;

            fly->ywo->ypaworld_func136(&arg136_2);
        }

        if ( a4 || !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) || (v90 && v82) )
            fly->ywo->ypaworld_func136(&arg136);

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
            bact->status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( !arg136.isect && !arg136_1.isect && !arg136_2.isect )
        {
            bact->status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT);
            bact->status_flg |= BACT_STFLAG_MOVE;
        }

        if ( !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) )
        {
            if ( arg136_1.isect == 1 && arg136_2.isect == 1 )
            {
                if ( arg136_1.tVal >= arg136_2.tVal )
                    bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
                else
                    bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;
            }

            if ( arg136_1.isect == 1 && !arg136_2.isect )
                bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;

            if ( !arg136_1.isect && arg136_2.isect == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;

            if ( !arg136_1.isect && !arg136_2.isect && arg136.isect == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
        }

        ypaworld_arg136 arg136_3;

        arg136_3.stPos = bact->old_pos;

        float v22 = bact->mass * 9.80665;

        if ( v22 <= bact->force )
            v22 = bact->force;

        float v100 = bact->airconst;

        if ( v100 < 10.0 )
            v100 = 10.0;

        arg136_3.vect.x = (bact->fly_dir.x * 200.0 * bact->fly_dir_length) / (v22 / v100);

        if ( arg136_3.vect.x < -200.0 )
            arg136_3.vect.x = -200.0;

        if ( arg136_3.vect.x > 200.0 )
            arg136_3.vect.x = 200.0;

        arg136_3.vect.y = bact->height;

        arg136_3.vect.z = (bact->fly_dir.z * 200.0 * bact->fly_dir_length) / (v22 / v100);

        if ( arg136_3.vect.z < -200.0 )
            arg136_3.vect.z = -200.0;

        if ( arg136_3.vect.z > 200.0 )
            arg136_3.vect.z = 200.0;

        arg136_3.flags = 0;

        fly->ywo->ypaworld_func136(&arg136_3);

        if ( arg136_3.isect )
        {
            bact->target_dir.y = -(1.0 - arg136_3.tVal);
        }
        else
        {
            NC_STACK_ypabact *v81 = fly->ywo->getYW_userVehicle();

            if ( ( (bact->secndTtype != BACT_TGT_TYPE_UNIT || (bact->secndT.pbact->bact_type != BACT_TYPES_ROBO && v81 != bact->secndT.pbact->self))
                    && (bact->primTtype != BACT_TGT_TYPE_UNIT || (bact->primT.pbact->bact_type != BACT_TYPES_ROBO && v81 != bact->primT.pbact->self)) )
                    || bact->target_dir.y >= -0.01 )
            {
                if ( bact->target_dir.y < 0.15 )
                    bact->target_dir.y = 0.15;
            }
        }

        if ( bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT) )
            bact->target_dir.y = -0.2;

        if ( arg136_3.isect && arg136_3.tVal * bact->height < bact->radius && bact->fly_dir.y > 0.0 )
        {
            arg88.pos1 = arg136_3.skel->polygons[arg136_3.polyID].Normal();

            Recoil(&arg88);
        }
        else
        {
            bact->status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( bact->target_dir.y != 0.0 )
            bact->target_dir.normalise();

        float v92 = arg136.vect.XZ().length();

        NDIV_CARRY(v92);

        if ( bact->status_flg & BACT_STFLAG_DODGE_LEFT )
        {
            bact->target_dir.x = -arg136.vect.z / v92;
            bact->target_dir.z = arg136.vect.x / v92;
        }
        else if ( bact->status_flg & BACT_STFLAG_DODGE_RIGHT )
        {
            bact->target_dir.x = arg136.vect.z  / v92;
            bact->target_dir.z = -arg136.vect.x  / v92;
        }

        ypaflyer_func70__sub0(fly, a2a);

        move_msg arg74;
        arg74.flag = 0;
        arg74.field_0 = a2a;

        Move(&arg74);

        sb_0x4b255c(fly, a2a, a3, 0);


        bact_arg75 arg75;
        arg75.fperiod = a2a;
        arg75.g_time = bact->clock;

        //printf("%d %d \n", bact->field_3DF, bact->field_3DE);

        if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.target.pbact = bact->secndT.pbact;
            arg75.prio = 1;

            FightWithBact(&arg75);
        }
        else if ( bact->secndTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = bact->sencdTpos;
            arg75.target.pcell = bact->secndT.pcell;
            arg75.prio = 1;

            FightWithSect(&arg75);
        }
        else if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.prio = 0;
            arg75.target.pbact = bact->primT.pbact;

            FightWithBact(&arg75);
        }
        else if ( bact->primTtype == BACT_TGT_TYPE_CELL )
        {
            arg75.pos = bact->primTpos;
            arg75.target.pcell = bact->primT.pcell;
            arg75.prio = 0;

            FightWithSect(&arg75);
        }
        else
        {
            bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

            if ( bact->status_flg & BACT_STFLAG_FIRE )
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

        if ( bact->clock - bact->newtarget_time > 500 )
        {
            bact_arg110 arg110;
            arg110.tgType = bact->secndTtype;
            arg110.priority = 1;

            bact_arg110 arg110_1;
            arg110_1.tgType = bact->primTtype;
            arg110_1.priority = 0;

            int v52 = TargetAssess(&arg110);

            int v55 = TargetAssess(&arg110_1);

            if ( v52 != 3 || v55 != 3 )
            {

                if ( !v52 )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( !v55 )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = bact->position.x;
                    arg67.tgt_pos.z = bact->position.z;
                    arg67.priority = 0;

                    SetTarget(&arg67);
                }

                if ( bact->primTtype || bact->secndTtype )
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
            bact->thraction = 0;
            fly->field_c = 0;

            if ( bact->status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;

                SetState(&arg78);

                ypaworld_arg136 arg136_4;
                arg136_4.stPos = bact->position;
                arg136_4.vect = vec3d::OY(bact->overeof + 50.0);
                arg136_4.flags = 0;

                fly->ywo->ypaworld_func136(&arg136_4);

                if ( arg136_4.isect )
                    bact->position.y = arg136_4.isectPos.y - bact->overeof;
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
    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;

    fly->bact_internal->airconst = fly->bact_internal->airconst_static;

    float a2 = (float)arg->frameTime / 1000.0;

    int a4 = getBACT_bactCollisions();

    if ( fly->bact_internal->status == BACT_STATUS_NORMAL || fly->bact_internal->status == BACT_STATUS_IDLE )
    {
        vec3d a3;
        a3 = fly->bact_internal->rotation.AxisZ();

        float v60 = -arg->inpt->sliders_vars[0] * fly->bact_internal->maxrot * a2;

        if ( a4 )
        {
            if ( !(fly->bact_internal->status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        float v61;

        if ( fly->bact_internal->status_flg & BACT_STFLAG_LAND )
            v61 = 0.1;
        else
            v61 = 1.0;

        if ( fly->bact_internal->fly_dir_length >= v61 )
        {
            if ( !(fly->bact_internal->status_flg & BACT_STFLAG_FIRE) )
            {
                setState_msg arg78;

                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);
            }

            fly->bact_internal->status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;

            arg136.stPos = fly->bact_internal->position;
            arg136.vect.z = 0;
            arg136.vect.x = 0;

            float v9;

            if ( fly->bact_internal->viewer_overeof <= fly->bact_internal->viewer_radius )
                v9 = fly->bact_internal->viewer_radius;
            else
                v9 = fly->bact_internal->viewer_overeof;

            arg136.flags = 0;
            arg136.vect.y = v9 * 1.5;

            fly->ywo->ypaworld_func136(&arg136);

            if ( arg136.isect && fly->bact_internal->thraction < fly->bact_internal->force * 0.001 && fly->field_c <= fly->bact_internal->mass * 9.80665 )
            {
                fly->bact_internal->status_flg |= BACT_STFLAG_LAND;
                fly->bact_internal->fly_dir_length = 0;
                fly->bact_internal->thraction = 0;

                fly->field_c = fly->bact_internal->mass * 9.80665;
            }
            else
            {
                fly->bact_internal->status_flg &= ~BACT_STFLAG_LAND;
            }

            float tmp = 0.0;

            if ( fly->bact_internal->primTtype == BACT_TGT_TYPE_CELL)
                tmp = (fly->bact_internal->primTpos.XZ() - fly->bact_internal->position.XZ()).length();

            if ( fly->bact_internal->primTtype != BACT_TGT_TYPE_CELL || tmp <= 800.0 )
            {
                if ( fly->bact_internal->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(fly->bact_internal->status_flg & BACT_STFLAG_FIRE) )
                    {
                        setState_msg arg78;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }

                fly->bact_internal->status = BACT_STATUS_NORMAL;
            }
            else
            {
                fly->bact_internal->status = BACT_STATUS_IDLE;

                if ( fly->bact_internal->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(fly->bact_internal->status_flg & BACT_STFLAG_FIRE) )
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

        ypabact.rotation *= mat3x3::RotateY(v60);

        fly->bact_internal->thraction += fly->bact_internal->force * (a2 * 0.3) * arg->inpt->sliders_vars[2];

        if ( fly->bact_internal->thraction > fly->bact_internal->force )
            fly->bact_internal->thraction = fly->bact_internal->force;

        if ( fly->bact_internal->thraction < 0.0 )
            fly->bact_internal->thraction = 0;


        fly->field_c = (fabs(fly->bact_internal->fly_dir_length) / 111.0 + 1.0) * (arg->inpt->sliders_vars[1] * 20000.0) * 0.5 + fly->bact_internal->mass * 9.80665;

        float v22 = fly->bact_internal->pSector->height - fly->bact_internal->position.y;

        float v63 = 1.0 / fly->bact_internal->height_max_user;

        float v72 = fly->bact_internal->mass * 7.0 * 9.80665  * ( 1.0 - ( POW2(v22) * POW2(v63) ) );

        if ( fly->field_c > v72 )
            fly->field_c = v72;

        bact_arg79 arg79;
        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = fly->bact_internal->rotation.AxisZ();

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = fly->bact_internal->rotation.AxisZ();

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.direction = vec3d(0.0, 0.0, 0.0);
            arg79.weapon = fly->bact_internal->weapon;
            arg79.g_time = fly->bact_internal->clock;

            if ( fly->bact_internal->clock % 2 )
                arg79.start_point.x = fly->bact_internal->fire_pos.x;
            else
                arg79.start_point.x = -fly->bact_internal->fire_pos.x;

            arg79.start_point.y = fly->bact_internal->fire_pos.y;
            arg79.start_point.z = fly->bact_internal->fire_pos.z;
            arg79.flags = ((arg->inpt->but_flags & 2) != 0) | 2;

            LaunchMissile(&arg79);
        }

        if ( fly->bact_internal->mgun != -1 )
        {
            if ( fly->bact_internal->status_flg & BACT_STFLAG_FIRE )
            {
                if ( !(arg->inpt->but_flags & 4) )
                {
                    setState_msg arg78;
                    arg78.setFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.unsetFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }
            }

            if ( arg->inpt->but_flags & 4 )
            {
                if ( !(fly->bact_internal->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;
                    arg78.setFlags = BACT_STFLAG_FIRE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = fly->bact_internal->rotation.AxisZ();
                arg105.field_C = a2;
                arg105.field_10 = fly->bact_internal->clock;

                FireMinigun(&arg105);
            }
        }

        if ( arg->inpt->but_flags & 8 )
            HandBrake(arg);

        if ( fly->bact_internal->status_flg & BACT_STFLAG_LAND )
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

                arg137.pos = fly->bact_internal->position + fly->bact_internal->rotation.AxisZ() * 0.5 * fly->bact_internal->viewer_radius;
                arg137.pos2 = fly->bact_internal->fly_dir;
                arg137.radius = fly->bact_internal->viewer_radius;
                arg137.coll_max = 10;
                arg137.field_30 = 0;
                arg137.collisions = v43;

                fly->ywo->ypaworld_func137(&arg137);

                if ( arg137.coll_count )
                {
                    tmp = vec3d(0.0, 0.0, 0.0);

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                        tmp += arg137.collisions[ j ].pos2;

                    bact_arg88 arg88;
                    if ( tmp.normalise() != 0.0 )
                        arg88.pos1 = tmp;
                    else
                        arg88.pos1 = fly->bact_internal->fly_dir;

                    Recoil(&arg88);

                    fly->field_c = fly->bact_internal->mass * 9.80665;

                    v67 = 1;
                }

                if ( !v67 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = fly->bact_internal->old_pos;
                    arg136.vect = fly->bact_internal->position - fly->bact_internal->old_pos;
                    arg136.flags = 0;

                    fly->ywo->ypaworld_func149(&arg136);

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
                    fly->bact_internal->status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(fly->bact_internal->soundcarrier.samples_data[5].flags & 2) )
                {
                    if ( !(fly->bact_internal->status_flg & BACT_STFLAG_LCRASH) )
                    {
                        fly->bact_internal->status_flg |= BACT_STFLAG_LCRASH;

                        SFXEngine::SFXe.startSound(&fly->bact_internal->soundcarrier, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = 10.0 * tmp.x + fly->bact_internal->position.x;
                        arg180.field_C = 10.0 * tmp.z + fly->bact_internal->position.z;

                        fly->ywo->ypaworld_func180(&arg180);
                    }
                }

                fly->bact_internal->fly_dir.y -= 0.2;
            }

            sb_0x4b255c(fly, a2, a3, 1);
        }
    }
    else if ( fly->bact_internal->status == BACT_STATUS_DEAD )
        DeadTimeUpdate(arg);
}

void NC_STACK_ypaflyer::Move(move_msg *arg)
{
    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;
    __NC_STACK_ypabact *bact = fly->bact_internal;

    bact->old_pos = bact->position;

    float v46;

    if ( bact->status == BACT_STATUS_DEAD )
        v46 = bact->mass * 39.2266;
    else
        v46 = bact->mass * 9.80665;

    vec3d tmp(0.0, 0.0, 0.0);
    float v47 = 0.0;

    if ( !(arg->flag & 1) )
    {
        tmp = bact->rotation.AxisZ().X0Z();
        v47 = bact->thraction;
    }

    float v35 = 0.0;

    if ( !(arg->flag & 1) )
        v35 = -1.0 * fly->field_c;

    float v12 = bact->fly_dir_length * bact->airconst;

    vec3d tmp2 = vec3d::OY(v46) + tmp * v47 - bact->fly_dir * v12 + vec3d::OY(v35);

    float v33 = tmp2.length();

    if ( v33 > 0.0 )
    {
        //vec3d tmp3 = bact->fly_dir * bact->fly_dir_length + tmp2 * (v33 / bact->mass * arg->field_0) / v33; //(arg->field_0 / bact->mass);
        vec3d tmp3 = bact->fly_dir * bact->fly_dir_length + tmp2 * (arg->field_0 / bact->mass);

        bact->fly_dir_length = tmp3.length();

        if ( bact->fly_dir_length > 0.0 )
            bact->fly_dir = tmp3 / bact->fly_dir_length;
    }

    if ( bact->fly_dir_length > 1.5 )
        bact->position += bact->fly_dir * bact->fly_dir_length * arg->field_0 * 6.0;

    CorrectPositionInLevelBox(NULL);

    bact->soundcarrier.samples_data[0].pitch = bact->pitch;
    bact->soundcarrier.samples_data[0].volume = bact->volume;

    float v48 = fabs(bact->fly_dir_length) / (bact->force / bact->airconst_static);
    float v44;

    if ( bact->pitch_max <= -0.8 )
        v44 = 1.2;
    else
        v44 = bact->pitch_max;

    float v49 = v48 * v44;

    if ( v49 > v44 )
        v49 = v44;

    if ( bact->soundcarrier.samples_data[0].psampl )
        bact->soundcarrier.samples_data[0].pitch += (bact->soundcarrier.samples_data[0].psampl->SampleRate + bact->soundcarrier.samples_data[0].pitch) * v49;
}

size_t NC_STACK_ypaflyer::SetPosition(bact_arg80 *arg)
{
    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;

    if ( !NC_STACK_ypabact::SetPosition(arg) )
        return 0;

    fly->field_c = fly->bact_internal->mass * 9.80665;
    return 1;
}

void NC_STACK_ypaflyer::Renew()
{
    NC_STACK_ypabact::Renew();

    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;

    fly->field_10 = 0;
    fly->field_c = 0;
}

void NC_STACK_ypaflyer::HandBrake(update_msg *arg)
{
    NC_STACK_ypabact::HandBrake(arg);

    __NC_STACK_ypaflyer *fly = &stack__ypaflyer;

    fly->bact_internal->thraction = 0;
    fly->field_c = fly->bact_internal->mass * 9.80665;
}


void NC_STACK_ypaflyer::setFLY_type(int tp)
{
    stack__ypaflyer.field_10 = tp;
}

int NC_STACK_ypaflyer::getFLY_type()
{
    return stack__ypaflyer.field_10;
}


size_t NC_STACK_ypaflyer::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        func2( (IDVList *)data );
        return 1;
    case 3:
        func3( (IDVList *)data );
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
    case 80:
        return (size_t)SetPosition( (bact_arg80 *)data );
    case 96:
        Renew();
        return 1;
    case 97:
        HandBrake( (update_msg *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
