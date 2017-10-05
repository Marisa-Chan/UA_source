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


const NewClassDescr NC_STACK_ypatank::description("ypatank.class", &newinstance);

size_t NC_STACK_ypatank::func0(IDVList *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    stack__ypatank.bact_internal = &ypabact;
    ypabact.bact_type = BACT_TYPES_TANK;

    stack__ypatank.field_c = 3;

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
                    stack__ypatank.ywo = (NC_STACK_ypaworld *)val.value.p_data;
                    stack__ypatank.yw = &stack__ypatank.ywo->stack__ypaworld;
                    break;

                case TANK_ATT_TIP:
                    setTANK_tip( val.value.i_data );
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypatank::func1()
{
    return NC_STACK_ypabact::func1();
}


size_t NC_STACK_ypatank::func2(IDVList *stak)
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
                case TANK_ATT_TIP:
                    setTANK_tip( val.value.i_data );
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ypatank::func3(IDVList *stak)
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
                case TANK_ATT_TIP:
                    *(int *)val.value.p_data = getTANK_tip();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return 1;
}

void NC_STACK_ypatank::AI_layer3(update_msg *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;

    float v244 = arg->frameTime / 1000.0;

    int v194 = getBACT_bactCollisions();

    __NC_STACK_ypabact *bact = tank->bact_internal;

    tank_arg128 arg128;
    arg128.field_0 = arg->frameTime / 1000.0;
    arg128.field_4 = bact->rotation.AxisZ();

    float v224 = bact->radius + 300.0;

    int v212 = tank->ywo->ypaworld_func145(bact);

    float v220 = bact->target_vec.length();

    if ( v220 > 0.0 )
        bact->target_dir = bact->target_vec / v220;

    bool v215 = !bact->secndTtype && v220 < 1200.0;

    switch ( bact->status )
    {
    case BACT_STATUS_NORMAL:
    {
        if ( !(bact->status_flg & BACT_STFLAG_LAND) )
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->frameTime;

            CrashOrLand(&arg86);
            break;
        }

        if ( !bact->primTtype && !bact->secndTtype )
        {
            bact->status = BACT_STATUS_IDLE;
            bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

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

        if ( v220 == 0.0 )
            break;

        if ( bact->waitCol_time > 0 )
        {
            bact->waitCol_time -= arg->frameTime;
        }
        else
        {

            float v206 = bact->target_dir.XZ().dot( bact->rotation.AxisZ().XZ() );

            float tmpsq = bact->rotation.AxisZ().XZ().length();

            NDIV_CARRY(tmpsq);

            v206 /= tmpsq;

            tmpsq = bact->target_dir.XZ().length();

            NDIV_CARRY(tmpsq);

            v206 /= tmpsq;

            float v240 = clp_acos(v206);

            if ( tank->field_25 & 0xF )
            {
                if ( tank->field_11.XZ().dot( bact->target_dir.XZ() ) < 0.0
                        && tank->field_1D == 0.0
                        && tank->field_21 == 0.0 )
                {
                    tank->field_21 = 0;
                    tank->field_1D = 0;
                    tank->field_25 &= 0xF0;
                    break;
                }

                if ( tank->field_25 & 0xC )
                {
                    if ( tank->field_1D == 0.0 && tank->field_21 == 0.0 )
                    {
                        ypaworld_arg136 arg136;

                        if ( tank->field_25 & 4 )
                        {
                            arg136.stPos = bact->position - bact->rotation.AxisX() * (2.0 * bact->radius);
                            arg136.vect = (bact->rotation.AxisX() * 4.0 + bact->rotation.AxisY() * 3.0) * bact->radius;
                        }
                        else
                        {
                            arg136.stPos = bact->position + bact->rotation.AxisX() * (2.0 * bact->radius);
                            arg136.vect = (bact->rotation.AxisX() * -4.0 + bact->rotation.AxisY() * 3.0) * bact->radius;
                        }
                        arg136.flags = 1;

                        tank->ywo->ypaworld_func136(&arg136);

                        if ( !arg136.isect || arg136.skel->polygons[ arg136.polyID ].B > 0.6 )
                            tank->field_25 &= 0xF3;

                        if ( !(tank->field_25 & 0xC) )
                            tank->field_21 = 80.0;
                    }
                }

                if ( tank->field_25 & 3 )
                {
                    if ( tank->field_1D == 0.0 && tank->field_21 == 0.0 )
                    {
                        ypaworld_arg136 arg136;

                        if ( tank->field_25 & 1 )
                            arg136.vect = -bact->rotation.AxisX() * 150.0;
                        else
                            arg136.vect = bact->rotation.AxisX() * 150.0;

                        arg136.stPos = bact->position;
                        arg136.flags = 1;

                        tank->ywo->ypaworld_func136(&arg136);

                        if ( !arg136.isect || arg136.skel->polygons[ arg136.polyID ].B > 0.6 )
                            tank->field_25 &= 0xFC;

                        if ( !(tank->field_25 & 3) )
                            tank->field_21 = 80.0;
                    }
                }

                if ( tank->field_1D <= 0.001 )
                {
                    tank->field_1D = 0;
                }
                else
                {
                    float v236 = bact->maxrot * v244;

                    if ( v236 <= tank->field_1D )
                    {
                        tank->field_1D -= v236;
                    }
                    else
                    {
                        v236 = tank->field_1D;
                        tank->field_1D = 0;
                    }

                    if ( tank->field_25 & 5 )
                        bact->rotation = mat3x3::RotateY(-v236) * bact->rotation;
                    else
                        bact->rotation = mat3x3::RotateY(v236) * bact->rotation;
                }

                if ( tank->field_1D == 0.0 )
                    bact->status_flg |= BACT_STFLAG_MOVE;
            }
            else
            {
                bact->status_flg |= BACT_STFLAG_MOVE;

                if ( v240 > 1.1 && tank->field_21 <= 0.001 )
                    bact->thraction *= 0.1 / (v244 + 0.1);

                if ( bact->rotation.m20 * bact->target_dir.z - bact->rotation.m22 * bact->target_dir.x < 0.0 )
                    v240 = -v240;

                if ( v240 >= 0.0 )
                {
                    float v195 = bact->maxrot * v244;

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
                    float v218 = -bact->maxrot * v244;

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

                if ( tank->field_21 <= 0.0001 )
                    bact->rotation = mat3x3::RotateY(v240) * bact->rotation;
            }

            if ( bact->status_flg & BACT_STFLAG_MOVE )
            {
                bact->thraction += bact->force * v244 * 0.8;

                if ( bact->thraction > bact->force )
                    bact->thraction = bact->force;

                if ( !(tank->field_25 & 0xF) && tank->field_21 > 0.0 )
                    bact->thraction -= bact->force * v244 * 0.6;

                if ( bact->thraction < 0.0 )
                    bact->thraction = 0;

                if ( !(bact->status_flg & BACT_STFLAG_ATTACK) || !tank->field_26 || tank->field_25 )
                {
                    move_msg arg74;
                    arg74.flag = 0;
                    arg74.field_0 = v244;

                    Move(&arg74);
                }

                if ( tank->field_21 > 0.0 )
                {
                    tank->field_21 -= ( bact->position.XZ() - bact->old_pos.XZ() ).length();

                    if ( tank->field_21 <= 0.0 )
                        tank->field_21 = 0;
                }
            }
            else
            {
                bact->old_pos = bact->position;
                bact->thraction = 0;
            }

            ypaworld_arg136 arg136;
            arg136.stPos = bact->old_pos;
            arg136.vect = bact->position - bact->old_pos;

            float v217 = arg136.vect.length();

            if ( v217 > 5.0 && bact->status_flg & BACT_STFLAG_MOVE )
                arg136.vect *= v224 / v217;
            else
                arg136.vect = bact->rotation.AxisZ() * v224;

            arg136.flags = 1;

            tank->ywo->ypaworld_func136(&arg136);

            if ( arg136.isect )
            {
                vec3d v55 = arg136.skel->polygons[ arg136.polyID ].Normal();

                if ( v55.y < 0.6 && tank->field_1D == 0.0)
                {
                    if ( arg136.tVal * v224 > (bact->radius + 50.0) && fabs(v240) < 1.0 && fabs(0.0) < 0.7 && v220 >= 840.0 )
                    {
                        if ( !bact->secndTtype )
                        {
                            float v242 = (bact->fly_dir_length * 0.1) * arg->frameTime / 1000.0;

                            if ( bact->rotation.AxisZ().XZ().cross( v55.XZ() ) >= 0.0 )
                                bact->rotation = mat3x3::RotateY(-v242) * bact->rotation;
                            else
                                bact->rotation = mat3x3::RotateY(v242) * bact->rotation;

                            tank->field_21 = arg136.tVal * v224 + 10.0;
                        }
                    }

                    if ( arg136.tVal * v224 <= (bact->radius + 50.0) )
                    {
                        bact->status_flg &= ~BACT_STFLAG_MOVE;
                        bact->position = bact->old_pos;

                        ypaworld_arg136 arg136_2;
                        arg136_2.stPos = bact->position;
                        arg136_2.vect = bact->rotation.AxisX() * 300.0;
                        arg136_2.flags = 1;

                        tank->ywo->ypaworld_func136(&arg136_2);

                        vec3d v187;

                        if ( arg136_2.isect )
                            v187 = v55 * arg136_2.skel->polygons[ arg136_2.polyID ].Normal();
                        else
                            v187 = vec3d(-v55.z, 0.0, v55.x);

                        if ( v187 == vec3d(0.0, 0.0, 0.0) )
                            v187 = bact->rotation.AxisX();

                        tank->field_11 = bact->rotation.AxisY() * v187;

                        if ( bact->rotation.AxisZ().dot(tank->field_11) < 0.0 )
                            tank->field_11 = -tank->field_11;

                        float tmpsq = tank->field_11.XZ().length();

                        NDIV_CARRY(tmpsq);

                        ypaworld_arg136 arg136_1;
                        ypaworld_arg136 arg136_3;

                        arg136_1.vect.x = tank->field_11.z * 150.0 / tmpsq;
                        arg136_1.vect.y = 0;
                        arg136_1.vect.z = -tank->field_11.x * 150.0 / tmpsq;
                        arg136_1.stPos = bact->old_pos;
                        arg136_1.flags = 1;

                        arg136_3.vect.x = -tank->field_11.z * 150.0 / tmpsq;
                        arg136_3.vect.y = 0;
                        arg136_3.vect.z = tank->field_11.x * 150.0 / tmpsq;
                        arg136_3.stPos = bact->old_pos;
                        arg136_3.flags = 1;

                        tank->ywo->ypaworld_func136(&arg136_1);
                        tank->ywo->ypaworld_func136(&arg136_3);

                        bool v86 = arg136_1.isect && arg136_1.skel->polygons[ arg136_1.polyID ].B < 0.6;
                        bool v87 = arg136_3.isect && arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6;

                        int v93;

                        vec2d az2d = bact->rotation.AxisZ().XZ();

                        if ( (v87 && !v86) || (v86 && !v87) )
                        {
                            if ( v87 )
                            {
                                float v206 = arg136_1.vect.XZ().dot( az2d );

                                tmpsq = az2d.length();

                                NDIV_CARRY(tmpsq);

                                v206 = v206 / tmpsq / 150.0;

                                tank->field_1D = clp_acos(v206);
                                v93 = 1;
                            }
                            else
                            {
                                float v206 = az2d.dot( arg136_3.vect.XZ() );

                                tmpsq = az2d.length();

                                NDIV_CARRY(tmpsq);

                                v206 = v206 / tmpsq / 150.0;

                                tank->field_1D = clp_acos(v206);
                                v93 = 0;
                            }
                        }
                        else
                        {
                            float v206 = az2d.dot( tank->field_11.XZ() );

                            tmpsq = az2d.length();

                            NDIV_CARRY(tmpsq);

                            v206 = v206 / tmpsq;

                            tmpsq = tank->field_11.XZ().length();

                            NDIV_CARRY(tmpsq);

                            v206 = v206 / tmpsq;

                            tank->field_1D = C_PI_2 - clp_acos(v206) + 0.01;

                            if ( az2d.cross( tank->field_11.XZ() ) <= 0.0 )
                                v93 = 0;
                            else
                                v93 = 1;
                        }

                        if ( !(tank->field_25 & 1) || v93 )
                        {
                            if ( tank->field_25 & 2 && v93 )
                            {
                                tank->field_1D = C_PI - tank->field_1D;
                            }
                            else if ( v93 )
                            {
                                tank->field_25 |= 1;
                            }
                            else
                            {
                                tank->field_25 |= 2;
                            }
                        }
                        else
                        {
                            tank->field_1D = C_PI - tank->field_1D;
                        }

                        tank->field_21 = 100.0;
                    }
                }
            }

            if ( !ypatank_func128(&arg128) )
            {
                bact->fly_dir_length = 0;
                bact->thraction = 0;

                arg136.isect = 0;

                if ( !(arg128.field_10 & 1) )
                {
                    arg136.stPos = bact->position + bact->rotation.AxisZ() * bact->radius;

                    float v223 = bact->radius * 8.0;
                    arg136.vect = (bact->rotation.AxisY() - bact->rotation.AxisZ()) * v223;
                    arg136.flags = 1;

                    tank->ywo->ypaworld_func136(&arg136);

                    if ( !arg136.isect || ( arg136.isect && arg136.tVal * v223 * 0.7 > (bact->overeof + 30.0) ) )
                    {
                        bact->status_flg &= ~BACT_STFLAG_LAND;
                        break;
                    }
                }

                bact->position = bact->old_pos;

                if ( (arg136.isect && arg136.skel->polygons[ arg136.polyID ].B < 0.6) || arg128.field_10 & 1 )
                {
                    bact->status_flg &= ~BACT_STFLAG_MOVE;
                    tank->field_25 &= 0xFC;

                    vec3d v208(0.0, 0.0, 0.0);

                    if ( arg136.isect )
                        v208 = -arg136.skel->polygons[ arg136.polyID ].Normal();

                    ypaworld_arg136 arg136_4;

                    arg136_4.stPos = bact->position;
                    arg136_4.vect = bact->rotation.AxisY() * 300.0;
                    arg136_4.flags = 1;

                    tank->ywo->ypaworld_func136(&arg136_4);

                    if ( arg128.field_10 & 1 )
                        v208 = -arg128.field_14;

                    vec3d v184;

                    if ( arg136_4.isect )
                        v184 = v208 * arg136_4.skel->polygons[arg136_4.polyID].Normal();
                    else
                        v184 = vec3d(-v208.z, 0.0, v208.x);

                    tank->field_11 = bact->rotation.AxisY() * v184;

                    if ( tank->field_11.dot( bact->rotation.AxisZ() ) < 0.0 )
                        tank->field_11 = -tank->field_11;

                    vec2d axsZ = bact->rotation.AxisZ().XZ();

                    float v206 = axsZ.dot( tank->field_11.XZ() );

                    float tmpsq = axsZ.length();

                    NDIV_CARRY(tmpsq);

                    v206 = v206 / tmpsq;

                    tmpsq = tank->field_11.XZ().length();

                    NDIV_CARRY(tmpsq);

                    v206 = v206 / tmpsq;

                    tank->field_1D = C_PI_2 - clp_acos(v206);

                    if ( tank->field_1D < 0.1 )
                        tank->field_1D = 0.1;

                    float v145 = axsZ.cross( tank->field_11.XZ() );

                    if ( tank->field_25 & 4 && v145 <= 0.0 )
                    {
                        tank->field_1D = C_PI - tank->field_1D;
                    }
                    else if ( tank->field_25 & 8 && v145 > 0.0 )
                    {
                        tank->field_1D = C_PI - tank->field_1D;
                    }
                    else if ( v145 <= 0.0 )
                    {
                        tank->field_25 |= 8;
                    }
                    else
                    {
                        tank->field_25 |= 4;
                    }

                    tank->field_21 = 100.0;
                }
            }

            if ( v194 )
            {
                if ( v212 || v215 )
                {
                    if ( CollisionWithBact(arg->frameTime) )
                        break;
                }
            }
        }

        bact_arg75 arg75;
        arg75.fperiod = v244;
        arg75.g_time = bact->clock;

        if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
        {
            arg75.prio = 1;
            arg75.target.pbact = bact->secndT.pbact;

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
        if ( bact->clock - bact->newtarget_time > 500 )
        {
            bact_arg110 arg110;
            bact_arg110 arg110_1;

            arg110.tgType = bact->secndTtype;
            arg110.priority = 1;

            arg110_1.tgType = bact->primTtype;
            arg110_1.priority = 0;

            int v150 = TargetAssess(&arg110);
            int v153 = TargetAssess(&arg110_1);

            if ( v150 != 3 || v153 != 3 )
            {
                if ( !v150 )
                {
                    setTarget_msg arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    SetTarget(&arg67);
                }

                if ( !v153 )
                {
                    setTarget_msg arg67;
                    arg67.priority = 0;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.x = bact->position.x;
                    arg67.tgt_pos.z = bact->position.z;

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

        setState_msg arg78;
        arg78.unsetFlags = 0;
        arg78.setFlags = 0;
        arg78.newStatus = BACT_STATUS_IDLE;

        SetState(&arg78);

        bact->thraction = 0;

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            bact->fly_dir_length = 0;

            if ( tank->field_D )
            {
                if ( bact->status_flg & BACT_STFLAG_DODGE_LEFT )
                    bact->rotation = mat3x3::RotateY(v244 * 0.33333334) * bact->rotation;
                else if ( bact->status_flg & BACT_STFLAG_DODGE_RIGHT )
                    bact->rotation = mat3x3::RotateY(-v244 * 0.33333334) * bact->rotation;

                tank->field_D--;
            }
            else
            {
                bact->status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT);
                tank->field_D = arg->frameTime / 5;

                if ( tank->field_c & 2 )
                {
                    int v158 = ((size_t)this + bact->clock) % 7; //WAT!?

                    if ( v158 )
                    {
                        if ( v158 == 1 )
                            bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;
                    }
                    else
                    {
                        bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
                    }
                }
            }
            if ( !ypatank_func128(&arg128) )
                bact->status_flg &= ~BACT_STFLAG_LAND;
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
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    bact->airconst = bact->airconst_static;

    int a4 = getBACT_bactCollisions();

    float v90 = arg->frameTime * 0.001;

    tank_arg129 arg129;

    arg129.field_0 = v90;
    arg129.field_4 = bact->rotation.AxisZ();

    if (bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE)
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
        }
        else
        {

            if ( bact->primTtype != BACT_TGT_TYPE_CELL || (bact->primTpos.XZ() - bact->position.XZ()).length() <= 800.0 )
            {
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }

                bact->status = BACT_STATUS_NORMAL;
            }
            else
            {
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    setState_msg arg78;
                    arg78.newStatus = BACT_STATUS_IDLE;
                    arg78.unsetFlags = 0;
                    arg78.setFlags = 0;

                    SetState(&arg78);
                }
                else
                    bact->status = BACT_STATUS_IDLE;
            }
        }

        float v85 = -arg->inpt->sliders_vars[3] * bact->maxrot * v90 * 2.0;

        if ( fabs(v85) > 0.0 )
            bact->rotation = mat3x3::RotateY(v85) * bact->rotation;

        if ( (bact->fly_dir_length < 0.0 && bact->thraction > 0.0) || (bact->fly_dir_length > 0.0 && bact->thraction < 0.0) )
        {
            if ( fabs(bact->fly_dir.y) > 0.1 )
            {
                float v84 = 1.0 - v90 * 4.0;

                if ( v84 < 0.1 )
                    v84 = 0.1;

                bact->fly_dir_length *= v84;
            }
        }

        float v88 = arg->inpt->sliders_vars[4];
        float v75 = fabs(v88);

        if ( v88 > 1.0 )
            v88 = 1.0;
        else if ( v88 < -1.0 )
            v88 = -1.0;

        bact->thraction += bact->force * (v90 * 0.75) * v88;

        float v78;

        if ( arg->inpt->but_flags & 0x80000000 )
            v78 = bact->force * v75;
        else
            v78 = bact->force;

        if ( bact->thraction > v78 )
            bact->thraction = v78;
        if ( bact->thraction < -v78 )
            bact->thraction = -v78;

        if ( fabs(v88) > 0.001 )
            bact->status_flg |= BACT_STFLAG_MOVE;

        bact->gun_angle_user += v90 * arg->inpt->sliders_vars[5];

        if ( bact->gun_angle_user < -0.3 )
            bact->gun_angle_user = -0.3;

        if ( bact->gun_angle_user > 0.8 )
            bact->gun_angle_user = 0.8;


        bact->gun_leftright = bact->gun_leftright - v90 * arg->inpt->sliders_vars[15];

        if ( bact->gun_leftright < -0.8 )
            bact->gun_leftright = -0.8;

        if ( bact->gun_leftright > 0.8 )
            bact->gun_leftright = 0.8;

        vec3d v67;
        v67 = bact->rotation.AxisZ();

        float corW, corH;
        GFXEngine::GFXe.getC3D()->getAspectCorrection(corW, corH, true);

        v67 -= bact->rotation.AxisY() * (bact->gun_angle_user * corH);
        v67 -= bact->rotation.AxisX() * (bact->gun_leftright * corW);

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

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.direction = v67;
            arg79.weapon = bact->weapon;
            arg79.g_time = bact->clock;

            if ( bact->clock % 2 )
                arg79.start_point.x = -bact->fire_pos.x;
            else
                arg79.start_point.x = bact->fire_pos.x;

            arg79.start_point.y = bact->fire_pos.y;
            arg79.start_point.z = bact->fire_pos.z;
            arg79.flags = (arg->inpt->but_flags & 2) != 0;
            arg79.flags |= 2;

            LaunchMissile(&arg79);
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->status_flg & BACT_STFLAG_FIRE )
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
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
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
                arg105.field_10 = bact->clock;

                FireMinigun(&arg105);
            }

        }

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            move_msg arg74;

            if ( arg->inpt->but_flags & 8 )
            {
                bact->thraction = 0;

                if ( fabs(bact->fly_dir_length) >= 4.0 )
                {
                    float v46 = 1.0 - v90 * 4.0;

                    if ( v46 < 0.1 )
                        v46 = 0.1;

                    bact->fly_dir_length *= v46;
                }
                else
                {
                    bact->status_flg &= ~BACT_STFLAG_MOVE;
                    bact->fly_dir_length = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.field_0 = v90;

            if ( bact->status_flg & BACT_STFLAG_MOVE )
                Move(&arg74);

            ypaworld_arg136 arg136;

            arg136.stPos = bact->old_pos;
            arg136.vect = bact->position - bact->old_pos;
            arg136.flags = 0;

            tank->ywo->ypaworld_func136(&arg136);

            if ( (a4 && CollisionWithBact(arg->frameTime)) || !arg136.isect ) // Check it
            {
                ypatank_func129(&arg129);
            }
            else
            {
                bact->position = bact->old_pos;
                bact->fly_dir_length = 0;
                bact->thraction = 0;
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
    else if (bact->status == BACT_STATUS_DEAD)
    {
        DeadTimeUpdate(arg);
    }
}

void NC_STACK_ypatank::Move(move_msg *arg)
{
    __NC_STACK_ypabact *bact = &ypabact;

    bact->old_pos = bact->position;

    float v50;

    if ( arg->flag & 2 && bact->status_flg & BACT_STFLAG_LAND )
    {
        v50 = 0.0;
    }
    else
    {
        if ( bact->status == BACT_STATUS_DEAD )
            v50 = bact->mass * 39.2266;
        else
            v50 = bact->mass * 9.80665;
    }

    vec3d v47;

    if ( arg->flag & 1 )
        v47 = vec3d::OY(v50 * 2.0);
    else
        v47 = bact->rotation.AxisZ() * bact->thraction;

    vec3d v40 = vec3d::OY(v50) + v47 - bact->fly_dir * (bact->fly_dir_length * bact->airconst);

    float v45 = v40.length();

    if ( v45 > 0.0 )
    {
        vec3d v37 = bact->fly_dir * bact->fly_dir_length + v40 * ( arg->field_0 / bact->mass );

        float v42 = v37.length();

        if ( v42 > 0.0 )
            v37 /= v42;

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            bact->fly_dir = bact->rotation.AxisZ();

            bact->fly_dir_length = v37.dot( bact->fly_dir ) * v42;
        }
        else
        {
            bact->fly_dir = v37;
            bact->fly_dir_length = v42;
        }
    }

    bact->position += bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

    CorrectPositionInLevelBox(NULL);

    bact->soundcarrier.samples_data[0].pitch = bact->pitch;
    bact->soundcarrier.samples_data[0].volume = bact->volume;

    float v48 = fabs(bact->fly_dir_length) / (bact->force / bact->airconst_static);
    float v46;

    if ( bact->pitch_max <= -0.8 )
        v46 = 1.1;
    else
        v46 = bact->pitch_max;

    float v49 = v46 * v48;

    if ( v49 > v46 )
        v49 = v46;

    if ( bact->soundcarrier.samples_data[0].psampl )
        bact->soundcarrier.samples_data[0].pitch += (float)(bact->soundcarrier.samples_data[0].psampl->SampleRate + bact->soundcarrier.samples_data[0].pitch) * v49;
}

size_t NC_STACK_ypatank::SetPosition(bact_arg80 *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    if ( NC_STACK_ypabact::SetPosition(arg) )
    {
        if ( arg->field_C & 1 )
        {
            ypaworld_arg136 arg136;
            arg136.stPos = bact->position.X0Z() - vec3d::OY(30000.0);
            arg136.vect = vec3d::OY(50000.0);
            arg136.flags = 0;

            tank->ywo->ypaworld_func136(&arg136);

            if ( !arg136.isect )
                return 0;

            int a4 = getBACT_viewer();

            if ( a4 )
                bact->position.y = arg136.isectPos.y - bact->viewer_overeof;
            else
                bact->position.y = arg136.isectPos.y - bact->overeof;

            bact->status_flg |= BACT_STFLAG_LAND;
        }
        return 1;
    }
    return 0;
}

void NC_STACK_ypatank::ApplyImpulse(bact_arg83 *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    float v30 = 500.0 / bact->mass;

    vec3d v27 = bact->position - arg->pos;

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

    vec3d v24 = bact->position + vec3d(v27.x, -v27.y, v27.z);

    ypaworld_arg136 arg136;
    arg136.stPos = bact->position;
    arg136.vect = v24 - bact->position;
    arg136.flags = 0;

    tank->ywo->ypaworld_func136(&arg136);

    if ( !arg136.isect )
        bact->position = v24;
}

void ypatank_func87__sub0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2)
{
    int v23 = (float)bact2->energy_max * 0.7;

    if ( v23 < 10000 )
        v23 = 10000;

    if ( v23 > 25000 )
        v23 = 25000;

    float v3 = (float)bact2->scale_time * 0.2 / (float)v23 * (float)bact2->energy_max;

    if ( v3 + bact->energy > bact->energy_max )
    {
        __NC_STACK_yparobo *robo = &bact->host_station->stack__yparobo;
        __NC_STACK_ypabact *rbact = &bact->host_station->ypabact;

        bact->energy = bact->energy_max;
        rbact = robo->bact_internal;

        int v10 = (int)v3 - (bact->energy_max - bact->energy);

        if ( v10 + rbact->energy > rbact->energy_max )
        {
            int v13 = rbact->energy;

            rbact->energy = rbact->energy_max;
            int v14 = v10 - (rbact->energy_max - v13);
            int v16 = v14 + robo->field_4F5;

            if ( v16 >= rbact->energy_max )
            {
                int v17 = robo->field_4F5;
                robo->field_4F5 = rbact->energy_max;

                robo->field_4FD += v14 - (rbact->energy_max - v17);

                if ( robo->field_4FD > rbact->energy_max )
                    robo->field_4FD = rbact->energy_max;
            }
            else
            {
                robo->field_4F5 = v16;
            }
        }
        else
        {
            rbact->energy = v10 + rbact->energy;
        }
    }
    else
    {
        bact->energy = v3 + bact->energy;
    }
}

size_t NC_STACK_ypatank::CollisionWithBact(int arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    int v105 = 0;
    int v108 = 0;

    float v121 = 0.0;
    int v103 = 0;

    __NC_STACK_ypabact *v80[10];
    vec3d v78[10];

    if ( bact->status_flg & BACT_STFLAG_LAND )
    {
        int a4 = getBACT_inputting();

        float v104;

        if ( a4 )
            v104 = 0.6;
        else
            v104 = 0.82;

        yw_130arg arg130;
        arg130.pos_x = bact->position.x;
        arg130.pos_z = bact->position.z;

        int v113 = 0;
        int v106 = 0;

        __NC_STACK_ypabact *v117 = NULL;

        float v99 = 300.0;
        float v124 = arg * 0.001;

        if ( !tank->ywo->ypaworld_func130(&arg130) )
            return 0;

        __NC_STACK_ypabact *v12 = (__NC_STACK_ypabact *)arg130.pcell->units_list.head;

        float v109 = 1200.0;

        while ( v12->next )
        {
            int v114 = v12->status == BACT_STATUS_DEAD   &&   v12->vp_extra[0].flags & EVPROTO_FLAG_ACTIVE   &&   a4 &&   v12->scale_time > 0;

            if ( v12->bact_type != BACT_TYPES_MISSLE
                    && (!v12->self->IsDestroyed() || v114)
                    && v12 != bact )
            {

                rbcolls *v96 = v12->self->getBACT_collNodes();

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
                        v89 = v12->position;
                        v19 = v12->radius;
                    }
                    else
                    {
                        roboColl &v15 = v96->roboColls[j];

                        v89 = v12->position + v12->rotation.Transpose().Transform(v15.coll_pos);
                        v19 = v15.robo_coll_radius;
                    }

                    if (!v96 || v19 >= 0.01 )
                    {
                        vec3d v93 = v89 - bact->position;

                        float v126 = v93.length();

                        if ( v126 < 300.0 )
                        {
                            float v115 = bact->radius + v19;

                            if ( v114 && v126 < v115 )
                            {
                                ypatank_func87__sub0(bact, v12);

                                v12->scale_time = -1;

                                if ( tank->yw->GameShell )
                                    startSound(&tank->yw->GameShell->samples2_info, 4);

                                if ( tank->yw->isNetGame )
                                {
                                    uamessage_endPlasma eplMsg;
                                    eplMsg.msgID = UAMSG_ENDPLASMA;
                                    eplMsg.owner = v12->owner;
                                    eplMsg.id = v12->gid;

                                    yw_arg181 arg181;
                                    arg181.recvID = 0;
                                    arg181.dataSize = sizeof(eplMsg);
                                    arg181.recvFlags = 2;
                                    arg181.garant = 1;
                                    arg181.data = &eplMsg;

                                    tank->ywo->ypaworld_func181(&arg181);

                                    if ( bact->owner != v12->owner )
                                    {
                                        v12->vp_extra[0].flags = 0;
                                        v12->vp_extra[0].vp.trigo = NULL;
                                        v12->vp_extra[0].vp.base = NULL;
                                    }
                                }
                                break;
                            }
                            else
                            {

                                float v118 = v93.dot( bact->rotation.AxisZ() );

                                if ( v126 > 0.01 )
                                    v118 = v118 / v126;

                                if ( fabs(bact->fly_dir_length) >= 0.1 )
                                {
                                    if ( bact->fly_dir_length < 0.0 )
                                    {
                                        v118 = -v118;
                                    }
                                }
                                else if ( bact->thraction < 0.0 )
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

                                        float v30 = v93.XZ().cross( bact->rotation.AxisZ().XZ() );
                                        v103 = v30 > 0.0;

                                        if ( v126 < v115 )
                                            v106 = 1;

                                        if ( bact->rotation.AxisZ().dot( v117->rotation.AxisZ() ) < 0.0 )
                                            v108 = 1;

                                        if ( v117->status == BACT_STATUS_IDLE )
                                            v108 = 1;

                                        if ( v117->waitCol_time <= 0 )
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
            v12 = (__NC_STACK_ypabact *)v12->next;
        }


        if ( !v117 )
        {
            bact->status_flg &= ~BACT_STFLAG_BCRASH;
            return 0;
        }

        if ( a4 )
        {
            if ( v106 && v113 )
            {
                vec3d v88(0.0, 0.0, 0.0);

                for (int i = 0; i < v113; i++)
                {
                    if ( !tank->yw->isNetGame || bact->owner == v80[i]->owner )
                    {
                        float v40 = bact->mass * 8.0 * fabs(bact->fly_dir_length);
                        float v41 = bact->thraction * v124 * 100.0;

                        float v127 = v124 * ((v40 + v41) / v80[i]->mass);

                        if ( v127 > 0.05 )
                        {
                            vec3d v85 = v78[i] * v127 + v80[i]->position;

                            ypaworld_arg136 arg136;
                            arg136.stPos = v80[i]->position;
                            arg136.vect = v85 - v80[i]->position;
                            arg136.flags = 0;

                            tank->ywo->ypaworld_func136(&arg136);

                            if ( !arg136.isect )
                            {

                                v80[i]->old_pos = v80[i]->position;

                                v80[i]->position = v85;

                                v80[i]->status_flg &= ~BACT_STFLAG_LAND;

                                v80[i]->self->CorrectPositionInLevelBox(NULL);
                            }
                        }

                        v88 += v78[i];
                    }
                }

                v88 /= (float)v113;

                vec3d v90 = bact->rotation.AxisZ();

                if ( bact->fly_dir_length < 0.0 )
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
                    bact->position = bact->old_pos;
                    bact->fly_dir_length = 0;
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

                    float v62 = fabs(bact->fly_dir_length) * v124 * 6.0;

                    bact->position = v88 * v62 + bact->old_pos;
                }

                if ( !(bact->status_flg & BACT_STFLAG_BCRASH) )
                {
                    startSound(&bact->soundcarrier, 6);
                    bact->status_flg |= BACT_STFLAG_BCRASH;
                }

                yw_arg180 arg180;

                arg180.field_4 = 1.0;
                arg180.field_8 = v88.x;
                arg180.field_C = v88.z;
                arg180.effects_type = 5;

                tank->ywo->ypaworld_func180(&arg180);
            }
            else
            {
                bact->status_flg &= ~BACT_STFLAG_BCRASH;
            }
        }
        else
        {
            if ( v106 )
            {
                bact->fly_dir_length = 0;
                bact->thraction = 0;

                bact->position = bact->old_pos;
                bact->status_flg &= ~BACT_STFLAG_MOVE;

                if ( !(bact->status_flg & BACT_STFLAG_BCRASH) )
                    bact->status_flg |= BACT_STFLAG_BCRASH;

                if ( v108 || !v105 )
                {
                    if ( !(tank->field_25 & 0xC) )
                    {
                        if ( tank->field_25 & 3 )
                        {
                            tank->field_21 = 100.0;
                            tank->field_1D = 1.5;
                        }
                        else
                        {
                            if ( v103 )
                                tank->field_25 |= 2;
                            else
                                tank->field_25 |= 1;

                            tank->field_21 = 80.0;
                            tank->field_1D = 1.5 - v121;
                        }
                    }
                }
                else
                {
                    bact->waitCol_time = 1000;
                }
            }
            else
            {
                bact->fly_dir_length *= v109 / v99;
                bact->status_flg &= ~BACT_STFLAG_BCRASH;
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
    __NC_STACK_ypabact *bact = &ypabact;

    if ( !(bact->status_flg & BACT_STFLAG_LAND) )
    {
        NC_STACK_ypabact::Recoil(arg);

        bact->fly_dir = bact->fly_dir * bact->fly_dir_length - arg->pos1 * 5.0;

        bact->fly_dir_length = bact->fly_dir.length();

        if ( bact->fly_dir_length > 0.001 )
            bact->fly_dir /= bact->fly_dir_length;
    }
}

void NC_STACK_ypatank::Renew()
{
    __NC_STACK_ypatank *tank = &stack__ypatank;

    NC_STACK_ypabact::Renew();

    tank->field_D = 0;
}

size_t NC_STACK_ypatank::CheckFireAI(bact_arg101 *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    vec3d v34 = arg->pos - bact->position;

    float v37 = v34.length();

    if ( v37 == 0.0 )
        return 0;

    v34 /= v37;

    WeapProto *a4 = tank->ywo->getYW_weaponProtos();

    tank->field_26 = 1;

    ypaworld_arg136 arg149;

    arg149.stPos = bact->position;
    arg149.vect = arg->pos - arg149.stPos;
    arg149.flags = 0;

    tank->ywo->ypaworld_func149(&arg149);

    if ( arg149.isect )
    {
        if ( arg->unkn == 2 )
        {
            __NC_STACK_ypabact *v15;

            if ( bact->secndTtype == BACT_TGT_TYPE_UNIT )
                v15 = bact->secndT.pbact;
            else
                v15 = bact->primT.pbact;

            if ( v15 )
            {
                if ( v15->bact_type == BACT_TYPES_TANK || v15->bact_type == BACT_TYPES_CAR || v15->bact_type == BACT_TYPES_GUN )
                {
                    tank->field_26 = 0;
                }
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

            if ( tank->ywo->ypaworld_func130(&arg130) )
            {
                if ( tank->ywo->ypaworld_func130(&arg130_1) )
                {
                    if ( arg130.pcell != arg130_1.pcell )
                    {
                        tank->field_26 = 0;
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
                                tank->field_26 = 0;
                            }
                        }
                    }
                }
            }
        }
    }

    WeapProto *v22 = NULL;
    int v43 = 0;

    if ( bact->weapon == -1 )
    {
        v22 = NULL;
    }
    else
    {
        v22 = &a4[ bact->weapon ];

        if ( v22->model_id & 1 )
            v43 = v22->model_id & 0xFE;
        else
            v22 = NULL;
    }

    if ( !v22 )
    {
        if ( bact->mgun == -1 )
            return 0;
        v43 = 2;
    }

    float v38 = 0.0;

    if ( v34.XZ() != vec2d(0.0, 0.0) )
    {
        v38 = v34.XZ().dot( bact->rotation.AxisZ().XZ() );

        float tmpsq = v34.XZ().length();

        NDIV_CARRY(tmpsq);

        v38 = v38 / tmpsq;

        tmpsq = bact->rotation.AxisZ().XZ().length();

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
            if ( v43 == 16 )
            {
                if ( v37 < 1200.0 && v38 > 0.93 )
                {
                    if ( v34.y > -0.85 && v34.y < 0.2 )
                        return 1;
                }
            }
            else if ( v37 < 1200.0 && v38 > 0.93 )
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
            if ( v43 == 16 )
            {
                if ( v37 < 1200.0 && v38 > 0.91 )
                {
                    if ( v34.y > -0.4 && v34.y < 0.3 )
                        return 1;
                }
            }
            else if ( v37 < 1200.0 && v38 > 0.91 )
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

size_t NC_STACK_ypatank::TestTargetSector(__NC_STACK_ypabact *cel_unit)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    cellArea *bactPcell = bact->pSector;
    cellArea *cunitPcell = cel_unit->pSector;

    if ( cel_unit->sectX == bact->sectX || cel_unit->sectY == bact->sectY )
    {
        if ( fabs(bactPcell->height - cunitPcell->height) >= 500.0 )
            return 0;
    }
    else
    {
        yw_130arg arg130;
        arg130.pos_x = bact->position.x;
        arg130.pos_z = cel_unit->position.z;

        tank->ywo->ypaworld_func130(&arg130);

        cellArea *arg130_Pcell =arg130.pcell;
        arg130.pos_x = cel_unit->position.x;
        arg130.pos_z = bact->position.z;

        tank->ywo->ypaworld_func130(&arg130);

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

void NC_STACK_ypatank::CorrectPositionOnLand(void *)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    NC_STACK_ypabact::CorrectPositionOnLand(NULL);

    if ( bact->status_flg & BACT_STFLAG_LAND )
    {
        int a4 = getBACT_viewer();

        float v6;

        if ( a4 )
            v6 = bact->viewer_overeof;
        else
            v6 = bact->overeof;

        ypaworld_arg136 arg136;
        arg136.stPos = bact->position;
        arg136.vect = vec3d::OY( v6 * 2.0 );
        arg136.flags = 0;

        tank->ywo->ypaworld_func136(&arg136);

        if ( arg136.isect )
            bact->position.y = arg136.isectPos.y - v6;
        else
            bact->status_flg &= ~BACT_STFLAG_LAND;
    }
}

void NC_STACK_ypatank::sub_49DA3C(vec3d &v)
{
    float force = ypabact.thraction - ypabact.airconst * ypabact.fly_dir_length;

    if ( fabs(force) > 3.0 )
    {
        float angle = 0.2 * (force / ypabact.force);

        v = mat3x3::AxisAngle(ypabact.rotation.AxisX(), -angle).Transform(v);
    }
}

size_t NC_STACK_ypatank::ypatank_func128(tank_arg128 *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    arg->field_10 = 0;

    int a4 = getBACT_viewer();

    float v5;

    if ( a4 )
        v5 = bact->viewer_overeof;
    else
        v5 = bact->overeof;

    ypaworld_arg136 arg136;
    arg136.stPos = bact->rotation.AxisY() * v5 + bact->position - vec3d::OY(50.0);
    arg136.vect = vec3d::OY( bact->overeof + 110.0 );
    arg136.flags = 0;

    tank->ywo->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        vec3d v10 = arg136.skel->polygons[ arg136.polyID ].Normal();

        if ( v10.y < 0.6 )
        {
            arg->field_10 |= 1;
            arg->field_14 = v10;
        }
        else
        {
            if ( tank->field_c & 1 )
                sub_49DA3C(v10);

            vec3d vaxis = bact->rotation.AxisY() * v10;

            float v46 = vaxis.length();

            if ( v46 > 0.0 )
            {
                float v57 = clp_acos( v10.dot( bact->rotation.AxisY() ) );

                float v47 = bact->maxrot * 2.0 * arg->field_0;

                if ( v57 > v47 )
                    v57 = v47;

                if ( fabs(v57) < 0.01 )
                    v57 = 0.0;

                vaxis /= v46;

                bact->rotation *= mat3x3::AxisAngle(vaxis, v57);
            }

            bact->position = arg136.isectPos - bact->rotation.AxisY() * v5;

            return 1;
        }
    }

    return 0;
}

size_t NC_STACK_ypatank::ypatank_func129(tank_arg129 *arg)
{
    __NC_STACK_ypatank *tank = &stack__ypatank;
    __NC_STACK_ypabact *bact = &ypabact;

    float v152 = 1.73;
    float v155 = 1.7;
    int v149 = 0;
    int v151 = 0;
    int v148 = 0;

    int a4 = getBACT_viewer();

    int v143 = getBACT_inputting();

    float v5;
    if ( a4 )
        v5 = bact->viewer_overeof;
    else
        v5 = bact->overeof;

    vec3d v133;
    vec3d v123;
    vec3d v122;
    float v156;

    if ( bact->fly_dir_length < 0.0 )
    {
        v123 = bact->position + (-bact->rotation.AxisZ() - bact->rotation.AxisX()) * bact->viewer_radius * 0.7071;

        v133 = bact->position + (bact->rotation.AxisX() - bact->rotation.AxisZ()) * bact->viewer_radius * 0.7071;

        v122 = bact->position + bact->rotation.AxisZ() * bact->viewer_radius;

        v156 = -1.0;
    }
    else
    {
        v123 = bact->position + (bact->rotation.AxisZ() - bact->rotation.AxisX()) * bact->viewer_radius * 0.7071;

        v133 = bact->position + (bact->rotation.AxisZ() + bact->rotation.AxisX()) * bact->viewer_radius * 0.7071;

        v122 = bact->position - bact->rotation.AxisZ() * bact->viewer_radius;

        v156 = 1.0;
    }

    v122.y += bact->viewer_overeof;
    v123.y += bact->viewer_overeof;
    v133.y += bact->viewer_overeof;

    ypaworld_arg136 arg136;
    ypaworld_arg136 arg136_1;
    ypaworld_arg136 arg136_2;

    arg136.stPos = bact->position - vec3d::OY(v152 * bact->viewer_radius);
    arg136_1.stPos = bact->position - vec3d::OY(v152 * bact->viewer_radius);
    arg136_2.stPos = bact->position - vec3d::OY(v152 * bact->viewer_radius);

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

    tank->ywo->ypaworld_func136(&arg136);

    if ( arg136.isect )
    {
        vec3d v48 = arg136.skel->polygons[ arg136.polyID ].Normal();

        if ( fabs(v48.y) < 0.6 )
        {
            float v164 = v48.dot(bact->rotation.AxisZ()) * v156;

            if ( v164 <= 0.0 )
            {
                bact->thraction = 0;
            }
            else
            {

                if ( bact->fly_dir_length > 2.333333333333334 )
                {
                    if ( v143 )
                    {
                        startSound(&bact->soundcarrier, 5);

                        yw_arg180 arg180;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = arg136.isectPos.x;
                        arg180.field_C = arg136.isectPos.z;
                        arg180.effects_type = 5;

                        tank->ywo->ypaworld_func180(&arg180);
                    }
                }

                if ( v164 >= 0.82 )
                {
                    bact->thraction = 0;
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;
                    return 2;
                }

                bact->position.x = bact->old_pos.x - v48.x * 10.0;
                bact->position.z = bact->old_pos.z - v48.z * 10.0;
            }
            return 2;
        }
    }
    else
    {
        if ( !v143 )
            return 2;

        arg136.isectPos = bact->position + arg136.vect - vec3d::OY(v152 * bact->viewer_radius);

        v149 = 1;
    }

    tank->ywo->ypaworld_func136(&arg136_1);

    if ( arg136_1.isect )
    {
        vec3d v54 = arg136_1.skel->polygons[ arg136_1.polyID ].Normal();

        if ( fabs(v54.y) < 0.6 )
        {
            float v172 = v54.dot(bact->rotation.AxisZ()) * v156;

            if ( v172 <= 0.0 )
            {
                bact->thraction = 0;
            }
            else
            {
                if ( bact->fly_dir_length > 2.333333333333334 )
                {
                    if ( v143 )
                    {
                        startSound(&bact->soundcarrier, 5);

                        yw_arg180 arg180_3;
                        arg180_3.field_8 = arg136_1.isectPos.x;
                        arg180_3.field_C = arg136_1.isectPos.z;
                        arg180_3.effects_type = 5;
                        arg180_3.field_4 = 1.0;

                        tank->ywo->ypaworld_func180(&arg180_3);
                    }
                }
                if ( v172 >= 0.82 )
                {
                    bact->thraction = 0;
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;

                    return 1;
                }

                bact->position.x = bact->old_pos.x - v54.x * 10.0;
                bact->position.z = bact->old_pos.z - v54.z * 10.0;
            }
            return 1;
        }
    }
    else
    {
        if ( !v143 )
            return 1;

        arg136_1.isectPos = bact->position + arg136_1.vect - vec3d::OY(v152 * bact->viewer_radius);
        v151 = 1;
    }

    tank->ywo->ypaworld_func136(&arg136_2);

    if ( arg136_2.isect )
    {
        if ( fabs(arg136_2.skel->polygons[arg136_2.polyID].B) < 0.6 )
        {
            bact->thraction = 0;

            if ( bact->fly_dir_length < -2.333333333333334 )
            {
                if ( v143 )
                {
                    startSound(&bact->soundcarrier, 5);

                    yw_arg180 arg180_1;
                    arg180_1.field_4 = 1.0;
                    arg180_1.field_8 = arg136_2.isectPos.x;
                    arg180_1.field_C = arg136_2.isectPos.z;
                    arg180_1.effects_type = 5;

                    tank->ywo->ypaworld_func180(&arg180_1);
                }
            }

            bact->position += bact->fly_dir * (bact->fly_dir_length * arg->field_0 * 6.0);

            return 3;
        }
    }
    else
    {
        arg136_2.isectPos = bact->position + arg136_2.vect - vec3d::OY(v152 * bact->viewer_radius);

        v148 = 1;
    }

    if ( a4 )
    {
        if ( v156 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.stPos = bact->position;
            arg136_3.vect = bact->rotation.AxisZ() * bact->viewer_radius;
            arg136_3.flags = 0;

            tank->ywo->ypaworld_func136(&arg136_3);

            if ( arg136_3.isect )
            {
                if ( arg136_3.skel->polygons[ arg136_3.polyID ].B < 0.6 )
                {
                    if ( bact->fly_dir_length > 2.333333333333334 )
                    {
                        if ( v143 )
                        {
                            startSound(&bact->soundcarrier, 5);

                            yw_arg180 arg180_2;
                            arg180_2.effects_type = 5;
                            arg180_2.field_4 = 1.0;
                            arg180_2.field_8 = arg136_3.isectPos.x;
                            arg180_2.field_C = arg136_3.isectPos.z;

                            tank->ywo->ypaworld_func180(&arg180_2);
                        }
                    }

                    bact->thraction = 0;
                    bact->fly_dir_length = 0;
                    bact->position = bact->old_pos;
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

        if ( tank->field_c & 1 )
            sub_49DA3C(v170);

        float v74 = fabs(bact->fly_dir_length);

        vec3d vaxis = bact->rotation.AxisY() * v170;;

        float v163;
        if ( v74 >= 5.0 )
            v163 = 0.007;
        else
            v163 = 0.01;

        float v147 = vaxis.length();
        if ( v147 > 0.0 )
        {
            vaxis /= v147;

            float v166 = clp_acos( v170.dot( bact->rotation.AxisY() ) );

            if ( v166 > bact->maxrot * 2.0 * arg->field_0 )
                v166 = bact->maxrot * 2.0 * arg->field_0;

            if ( fabs(v166) > v163 )
                bact->rotation *= mat3x3::AxisAngle(vaxis, v166);
        }
    }

    if ( arg136.isect || arg136_1.isect || arg136_2.isect )
    {
        if ( v149 || v151 || v148 )
        {
            float v92 = bact->viewer_overeof * v155 * 0.8;

            ypaworld_arg136 arg136_4;
            arg136_4.stPos = bact->position - vec3d::OY(v92);
            arg136_4.vect = vec3d::OY( v92 * 2.0 );

            arg136_4.flags = a4 == 0;

            tank->ywo->ypaworld_func136(&arg136_4);

            if ( !arg136_4.isect || (arg136_4.isect && arg136_4.skel->polygons[ arg136_4.polyID ].B < 0.6) )
            {
                vec3d tmp(0.0, 0.0, 0.0);

                bact->status_flg &= ~BACT_STFLAG_LAND;

                if ( v149 )
                    tmp = bact->rotation.AxisZ() * v156 - bact->rotation.AxisX();

                if ( v151 )
                    tmp += bact->rotation.AxisZ() * v156 + bact->rotation.AxisX();

                if ( v148 )
                    tmp -= bact->rotation.AxisZ() * v156;

                bact->position += tmp * (arg->field_0 * 400.0);
                return 0;
            }
            bact->position.y = arg136_4.isectPos.y - v5;
        }
        else
        {
            bact->position.y = (arg136.isectPos.y + arg136_1.isectPos.y + arg136_2.isectPos.y) * 0.33333334 - v5;
        }
    }
    else
    {
        bact->status_flg &= ~BACT_STFLAG_LAND;
    }
    return 0;
}

void NC_STACK_ypatank::setTANK_tip(int tip)
{
    if ( tip )
        stack__ypatank.field_c |= 1;
    else
        stack__ypatank.field_c &= ~1;
}

int NC_STACK_ypatank::getTANK_tip()
{
    if (stack__ypatank.field_c & 1)
        return 1;
    return 0;
}



size_t NC_STACK_ypatank::compatcall(int method_id, void *data)
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
    case 83:
        ApplyImpulse( (bact_arg83 *)data );
        return 1;
    case 87:
        return (size_t)CollisionWithBact( (int)(size_t)data );
    case 88:
        Recoil( (bact_arg88 *)data );
        return 1;
    case 96:
        Renew();
        return 1;
    case 101:
        return (size_t)CheckFireAI( (bact_arg101 *)data );
    case 111:
        return (size_t)TestTargetSector( (__NC_STACK_ypabact *)data );
    case 114:
        CorrectPositionOnLand( (void *)data );
        return 1;
    case 128:
        return (size_t)ypatank_func128( (tank_arg128 *)data );
    case 129:
        return (size_t)ypatank_func129( (tank_arg129 *)data );
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
