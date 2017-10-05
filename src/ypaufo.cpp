#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypaufo.h"
#include "log.h"

#include <math.h>


const NewClassDescr NC_STACK_ypaufo::description("ypaufo.class", &newinstance);


size_t NC_STACK_ypaufo::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &ypabact;

    ufo->bact_internal = bact;

    bact->bact_type = BACT_TYPES_UFO;

    stack__ypaufo.field_c = 200.0;

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
                    stack__ypaufo.ywo = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__ypaufo.yw = &stack__ypaufo.ywo->stack__ypaworld;
                    break;

                case BACT_ATT_INPUTTING:
                    setBACT_inputting(val.value.i_data);
                    break;

                case UFO_ATT_TOGO:
                    setUFO_togo(val.value.i_data);
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypaufo::func1()
{
    return NC_STACK_ypabact::func1();
}

size_t NC_STACK_ypaufo::func2(IDVList *stak)
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
                case BACT_ATT_INPUTTING:
                    setBACT_inputting(val.value.i_data);
                    break;

                case UFO_ATT_TOGO:
                    setUFO_togo(val.value.i_data);
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypaufo::func3(IDVList *stak)
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
                case UFO_ATT_TOGO:
                    *(int *)val.value.p_data = getUFO_togo();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

void NC_STACK_ypaufo::AI_layer3(update_msg *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &ypabact;

    float v110 = arg->frameTime / 1000.0;

    int v94 = getBACT_bactCollisions();

    int v5 = ufo->ywo->ypaworld_func145(bact);

    float v108 = bact->target_vec.length();

    if ( v108 != 0.0 )
        bact->target_dir = bact->target_vec / v108;

    int v8 = !bact->secndTtype && v108 < 1200.0;

    if ( v108 > ufo->field_c )
        v108 = ufo->field_c;

    switch ( bact->status )
    {
    case BACT_STATUS_NORMAL:
    {
        bact->thraction = bact->force;

        if ( !v94
                || (!v5 && !v8)
                || !CollisionWithBact(arg->frameTime) )
        {
            if ( bact->primTtype || bact->secndTtype )
            {
                ufo->field_14 = bact->mass * 9.80665;
                bact->thraction = bact->force;


                if ( ufo->field_1c & 1 )
                {
                    bact->thraction = 0;

                    if ( fabs(bact->fly_dir_length * bact->fly_dir.x) >= 0.1 || fabs(bact->fly_dir_length * bact->fly_dir.z) >= 0.1 )
                    {
                        bact->thraction = 0;
                        bact->fly_dir_length *= 0.8;
                    }
                    else
                    {
                        if ( ufo->field_1c & 8 )
                        {
                            float v17 = bact->maxrot * v110;

                            if ( fabs(ufo->field_18) >= v17 )
                            {
                                if ( ufo->field_18 < 0.0 )
                                    v17 = -v17;

                                ufo->field_18 -= v17;

                                bact->rotation = mat3x3::RotateY(v17) * bact->rotation;
                            }
                            else
                            {
                                bact->rotation = mat3x3::RotateY(ufo->field_18) * bact->rotation;

                                ufo->field_1c &= 0xFFFFFFF6;
                                ufo->field_18 = 0;
                                ufo->field_10 = v108;
                            }
                        }
                        else
                        {
                            if ( ufo->field_1c & 2 )
                                ufo->field_14 = bact->mass * 4.0 * 9.80665;

                            if ( ufo->field_1c & 4 )
                                ufo->field_14 = bact->mass * 0.3 * 9.80665;
                        }
                    }
                }

                move_msg arg74;
                arg74.flag = 0;
                arg74.field_0 = v110;

                Move(&arg74);

                ypaworld_arg136 arg136_2;
                arg136_2.stPos = bact->old_pos;

                vec2d ttmp = bact->position.XZ() - bact->old_pos.XZ();

                float v29 = ttmp.length();

                if ( v29 >= 0.01 )
                    arg136_2.vect = vec3d::X0Z(ttmp * 300.0 / v29);
                else
                    arg136_2.vect = bact->rotation.AxisZ() * 300.0;

                ypaworld_arg136 arg136;
                arg136.stPos = bact->position;
                arg136.vect = vec3d::OY( bact->height );

                arg136_2.flags = 0;
                arg136.flags = 0;

                ufo->ywo->ypaworld_func136(&arg136);

                ufo->ywo->ypaworld_func136(&arg136_2);

                if ( (arg136_2.isect && arg136_2.tVal * 300.0 < bact->radius) || (arg136.isect && arg136.tVal * 300.0 < bact->radius) )
                {
                    bact_arg88 arg88;

                    if ( arg136_2.isect && arg136_2.tVal * 300.0 < bact->radius )
                        arg88.pos1 = arg136_2.skel->polygons[arg136_2.polyID].Normal();
                    else
                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                    Recoil(&arg88);
                }

                if ( ufo->field_1c & 1 )
                {
                    if ( ufo->field_1c & 6 )
                    {

                        if ( ufo->field_1c & 2 )
                        {
                            if ( !arg136.isect || (arg136.isect && arg136.tVal > 0.8) )
                            {
                                ufo->field_1c &= 0xFFFFFFFC;
                                ufo->field_10 = v108;
                            }
                        }

                        if ( ufo->field_1c & 4 )
                        {
                            if ( arg136.isect && arg136.tVal < 0.8 )
                            {
                                ufo->field_1c &= 0xFFFFFFFA;
                                ufo->field_10 = v108;
                            }
                        }

                    }
                }
                else if ( (arg136_2.isect && arg136_2.tVal < 0.5) || (arg136.isect && arg136.tVal < 0.5) )
                {
                    if ( arg136.isect && arg136.tVal < 0.5 )
                    {
                        ufo->field_1c |= 3;
                    }
                    else
                    {
                        ufo->field_1c |= 9;

                        vec2d polv = arg136_2.skel->polygons[arg136_2.polyID].Normal().XZ();
                        vec2d axsz = bact->rotation.AxisZ().XZ();

                        float v104 = axsz.dot( polv );

                        float tmpsq = axsz.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        tmpsq = polv.length();

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        ufo->field_18 = C_PI_2 - clp_acos(v104);
                    }
                }
                else if ( ufo->field_10 <= 0.0 )
                {
                    if ( arg136.isect )
                    {
                        vec2d tgt2d = bact->target_dir.XZ();
                        vec2d axsz = bact->rotation.AxisZ().XZ();

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

                            ufo->field_18 = v121;
                            ufo->field_1c |= 9;
                        }
                    }
                    else
                    {
                        NC_STACK_ypabact *v102 = ufo->ywo->getYW_userVehicle();;

                        if ( ( (bact->secndTtype != BACT_TGT_TYPE_UNIT || v102 != bact->secndT.pbact->self) && ( bact->primTtype != BACT_TGT_TYPE_UNIT || v102 != bact->primT.pbact->self) ) || bact->target_dir.y >= 0.0 )
                        {
                            ufo->field_1c |= 5;
                        }
                    }
                }

                bact_arg75 arg75;
                arg75.fperiod = v110;
                arg75.g_time = bact->clock;

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
                    arg75.target.pbact = bact->primT.pbact;
                    arg75.prio = 0;

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
                        arg78.unsetFlags = BACT_STFLAG_FIRE;
                        arg78.newStatus = BACT_STATUS_NOPE;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }
            }
            else
            {
                bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

                setState_msg arg78;

                if ( bact->status_flg & BACT_STFLAG_FIRE )
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
        if ( bact->clock - bact->newtarget_time > 500 )
        {
            bact_arg110 arg110;
            bact_arg110 arg110_1;

            arg110.tgType = bact->secndTtype;
            arg110.priority = 1;

            arg110_1.tgType = bact->primTtype;
            arg110_1.priority = 0;

            int v63 = TargetAssess(&arg110);

            int v66 = TargetAssess(&arg110_1);

            if ( v63 != 3 || v66 != 3 )
            {
                if ( !v63 )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( !v66 )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = ufo->bact_internal->position.x;
                    arg67.tgt_pos.z = ufo->bact_internal->position.z;
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

        int v93 = getBACT_landingOnWait();

        if ( v93 == 0 )
        {
            bact->thraction = 0.0;
            bact->rotation *= mat3x3::RotateY(bact->maxrot * v110);
        }
        else
        {
            bact->thraction = 0.0;

            ufo->field_14 = 0;

            if ( bact->status_flg & BACT_STFLAG_LAND )
            {
                setState_msg arg78;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;
                arg78.newStatus = BACT_STATUS_IDLE;

                SetState(&arg78);

                ypaworld_arg136 arg136_1;

                arg136_1.stPos = bact->position;
                arg136_1.vect = vec3d::OY(bact->overeof + 50.0);
                arg136_1.flags = 0;

                ufo->ywo->ypaworld_func136(&arg136_1);

                if ( arg136_1.isect )
                    bact->position.y = arg136_1.isectPos.y - bact->overeof;
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
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &ypabact;

    float v88 = arg->frameTime / 1000.0;

    int a4 = getBACT_bactCollisions();

    bact->old_pos = bact->position;

    if ( bact->status == BACT_STATUS_DEAD )
    {
        DeadTimeUpdate(arg);
    }
    else if ( bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE )
    {

        if ( bact->fly_dir_length != 0.0 )
        {
            if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
            {
                setState_msg arg78;
                arg78.newStatus = BACT_STATUS_NORMAL;
                arg78.unsetFlags = 0;
                arg78.setFlags = 0;

                SetState(&arg78);
            }

            bact->status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;
            arg136.stPos = bact->position;
            arg136.vect = vec3d(0.0, 0.0, 0.0);

            if ( bact->viewer_overeof <= bact->viewer_radius )
                arg136.vect.y = bact->viewer_radius * 1.5;
            else
                arg136.vect.y = bact->viewer_overeof * 1.5;

            arg136.flags = 0;

            ufo->ywo->ypaworld_func136(&arg136);

            if ( !arg136.isect || bact->thraction != 0.0 || ufo->field_14 > bact->mass * 9.80665 )
            {
                bact->status_flg &= ~BACT_STFLAG_LAND;
            }
            else
            {
                bact->status_flg |= BACT_STFLAG_LAND;
                bact->fly_dir_length = 0;
                bact->thraction = 0;

                ufo->field_14 = bact->mass * 9.80665;
            }



            if ( bact->primTtype != BACT_TGT_TYPE_CELL
                    || (bact->primTpos.XZ() - bact->position.XZ()).length() <= 800.0 )
            {
                if ( bact->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                    {
                        setState_msg arg78;
                        arg78.newStatus = BACT_STATUS_IDLE;
                        arg78.unsetFlags = 0;
                        arg78.setFlags = 0;

                        SetState(&arg78);
                    }
                }

                bact->status = BACT_STATUS_NORMAL;
            }
            else
            {
                bact->status = BACT_STATUS_IDLE;

                if ( bact->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
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

        float v23 = -arg->inpt->sliders_vars[0] * bact->maxrot * v88;

        if ( fabs(v23) > 0.0 )
            bact->rotation *= mat3x3::RotateY(v23);

        float v25 = arg->inpt->sliders_vars[1] * bact->maxrot * v88;

        if ( fabs(v25) > 0.0 )
            bact->rotation = mat3x3::RotateX(v25) * bact->rotation;

        if ( arg->inpt->sliders_vars[2] != 0.0 )
        {
            ufo->field_14 = (arg->inpt->sliders_vars[2] * 4.0 + 1.0) * bact->mass * 9.80665;

            float v85 = bact->pSector->height - bact->position.y;
            float v96 = bact->height_max_user - v85;
            float v101 = 1.0 / bact->height_max_user;

            float v100 = 9.80665 * bact->mass * 5.0 * v96 * v101;

            if ( ufo->field_14 > v100 )
                ufo->field_14 = v100;
        }
        else
        {
            ufo->field_14 = bact->mass * 9.80665;
        }

        if ( a4 )
        {
            if ( !(bact->status_flg & BACT_STFLAG_LAND) )
            {
                CollisionWithBact(arg->frameTime);
            }
        }

        if ( arg->inpt->but_flags & 8 )
        {
            bact->thraction = 0;

            ufo->field_14 = bact->mass * 9.80665;

            bact->fly_dir_length *= 0.7;

            if ( bact->fly_dir_length < 0.1 )
                bact->fly_dir_length = 0;
        }

        bact_arg79 arg79;

        arg79.tgType = BACT_TGT_TYPE_DRCT;
        arg79.tgt_pos = bact->rotation.AxisZ();

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4 = bact->rotation.AxisZ();

        if ( UserTargeting(&arg106) )
        {
            arg79.target.pbact = arg106.ret_bact;
            arg79.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.weapon = bact->weapon;
            arg79.direction = bact->rotation.AxisZ();
            arg79.tgType = BACT_TGT_TYPE_NONE;
            arg79.g_time = bact->clock;

            if ( bact->clock % 2 )
                arg79.start_point.x = -bact->fire_pos.x;
            else
                arg79.start_point.x = bact->fire_pos.x;

            arg79.start_point.y = bact->fire_pos.y;
            arg79.start_point.z = bact->fire_pos.z;
            arg79.flags = (arg->inpt->but_flags & 2) != 0;

            LaunchMissile(&arg79);
        }

        if ( bact->weapon == -1 )
        {
            if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
            {
                if ( bact->thraction < bact->force )
                {
                    bact->thraction += (float)arg->frameTime * bact->force * 0.0099999998;
                }
            }
            else if ( bact->thraction > 0.0 )
            {
                bact->thraction -= (float)arg->frameTime * bact->force * 0.001;
            }
            else
            {
                bact->thraction = 0;

                if ( arg->inpt->sliders_vars[2] == 0.0 )
                    bact->fly_dir_length *= 0.6;

                if ( bact->fly_dir_length < 0.1 )
                    bact->fly_dir_length = 0;
            }
        }
        else
        {
            bact->thraction = 0;

            if ( fabs(bact->fly_dir.x * bact->fly_dir_length) > 0.1 || fabs(bact->fly_dir.z * bact->fly_dir_length) > 0.1 )
                bact->fly_dir_length *= 0.6;

            if ( bact->fly_dir_length < 0.1 )
                bact->fly_dir_length = 0;
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
            {
                if ( arg->inpt->but_flags & 4 )
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
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.setFlags = BACT_STFLAG_FIRE;
                    arg78.unsetFlags = 0;
                    arg78.newStatus = BACT_STATUS_NOPE;

                    SetState(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0 = bact->rotation.AxisZ();
                arg105.field_C = v88;
                arg105.field_10 = bact->clock;

                FireMinigun(&arg105);
            }
        }

        if ( bact->status_flg & BACT_STFLAG_LAND )
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
                arg137.pos = bact->position;
                arg137.pos2 = bact->fly_dir;
                arg137.radius = bact->viewer_radius;
                arg137.collisions = v60;
                arg137.coll_max = 10;
                arg137.field_30 = 0;

                ufo->ywo->ypaworld_func137(&arg137);

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
                        arg88.pos1 = bact->fly_dir;

                    bact->thraction = 0;

                    Recoil(&arg88);

                    v49 = 1;
                }

                if ( v49 == 0 )
                {
                    ypaworld_arg136 arg136;
                    arg136.stPos = bact->old_pos;
                    arg136.vect = bact->position - bact->old_pos;

                    arg136.flags = 0;

                    ufo->ywo->ypaworld_func136(&arg136);

                    if ( arg136.isect )
                    {
                        bact->thraction = 0;

                        bact_arg88 arg88;

                        arg88.pos1 = arg136.skel->polygons[arg136.polyID].Normal();

                        Recoil(&arg88);

                        v49 = 1;
                    }
                }

                if ( v49 == 0 )
                {
                    bact->status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(bact->soundcarrier.samples_data[5].flags & 2) )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_LCRASH) )
                    {
                        bact->status_flg |= BACT_STFLAG_LCRASH;

                        startSound(&bact->soundcarrier, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v93.x * 10.0 + bact->position.x;
                        arg180.field_C = v93.z * 10.0 + bact->position.z;

                        ufo->ywo->ypaworld_func180(&arg180);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypaufo::Move(move_msg *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &ypabact;

    bact->old_pos = bact->position;

    float v55;

    if ( bact->status == BACT_STATUS_DEAD )
        v55 = bact->mass * 39.2266;
    else
        v55 = bact->mass * 9.80665;

    vec3d mv(0.0, 0.0, 0.0);

    if ( !(arg->flag & 1) )
    {
        vec3d az = bact->rotation.AxisZ().X0Z();
        float v9 = az.length();

        if ( v9 <= 0.001 )
            mv = -bact->rotation.AxisY().X0Z() * ufo->field_14;
        else
            mv = az * (bact->thraction / v9);
    }

    float v44 = 0.0;

    if ( !(arg->flag & 1) )
        v44 = -1.0 * ufo->field_14;

    vec3d v45 = vec3d::OY(v55) + mv - bact->fly_dir * (fabs(bact->fly_dir_length) * bact->airconst) + vec3d::OY(v44);

    float v52 = v45.length();
    if ( v52 > 0.0 )
    {
        vec3d v42 = bact->fly_dir * bact->fly_dir_length + v45 * (arg->field_0 / bact->mass);

        float v51 = v42.length();
        if ( v51 > 0.0 )
            v42 /= v51;

        bact->fly_dir = v42;

        bact->fly_dir_length = v51;
    }

    vec3d a1v = bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

    if ( fabs(bact->fly_dir_length) > 0.1 )
        bact->position += a1v;

    ufo->field_10 -= a1v.length();

    if ( ufo->field_10 < 0.0 )
        ufo->field_10 = 0;

    CorrectPositionInLevelBox(NULL);

    bact->soundcarrier.samples_data[0].pitch = bact->pitch;
    bact->soundcarrier.samples_data[0].volume = bact->volume;

    float v53;

    if ( bact->pitch_max <= -0.8 )
        v53 = 1.2;
    else
        v53 = bact->pitch_max;

    float v58 = fabs(bact->fly_dir_length) / (bact->force / bact->airconst_static) * v53;

    if ( v58 > v53 )
        v58 = v53;

    if ( bact->soundcarrier.samples_data[0].psampl )
        bact->soundcarrier.samples_data[0].pitch = (bact->soundcarrier.samples_data[0].psampl->SampleRate + bact->soundcarrier.samples_data[0].pitch) * v58;

}

size_t NC_STACK_ypaufo::SetPosition(bact_arg80 *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;

    if ( !NC_STACK_ypabact::SetPosition(arg))
        return 0;

    ufo->field_14 = ufo->bact_internal->mass * 9.80665;
    return 1;
}

void NC_STACK_ypaufo::Renew()
{
    NC_STACK_ypabact::Renew();

    __NC_STACK_ypaufo *ufo = &stack__ypaufo;

    ufo->field_14 = 0;

    setBACT_landingOnWait(0);
}


void NC_STACK_ypaufo::setBACT_inputting(int inpt)
{
    NC_STACK_ypabact::setBACT_inputting(inpt);

    if ( !inpt )
    {
        __NC_STACK_ypabact *bact = &ypabact;
        bact->rotation.m00 = 1.0;
        bact->rotation.m01 = 0.0;
        bact->rotation.m02 = 0.0;
        bact->rotation.m10 = 0.0;
        bact->rotation.m11 = 1.0;
        bact->rotation.m12 = 0.0;
        bact->rotation.m20 = 0.0;
        bact->rotation.m21 = 0.0;
        bact->rotation.m22 = 1.0;
    }
}

void NC_STACK_ypaufo::setUFO_togo(int tog)
{
    stack__ypaufo.field_c = tog;
}

int NC_STACK_ypaufo::getUFO_togo()
{
    return stack__ypaufo.field_c;
}


size_t NC_STACK_ypaufo::compatcall(int method_id, void *data)
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
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
