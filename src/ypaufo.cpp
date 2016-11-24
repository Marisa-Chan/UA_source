#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "yw.h"
#include "ypaufo.h"
#include "log.h"

#include <math.h>


const NewClassDescr NC_STACK_ypaufo::description("ypaufo.class", &newinstance);


int NC_STACK_ypaufo::ypaufo_func0__sub0(stack_vals *stak)
{
    stack__ypaufo.field_c = 200.0;

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
                stack__ypaufo.ywo = (NC_STACK_ypaworld *)stk->value.p_data;
                stack__ypaufo.yw = &stack__ypaufo.ywo->stack__ypaworld;
                break;

            case BACT_ATT_INPUTTING:
                setBACT_inputting(stk->value.i_data);
                break;

            case UFO_ATT_TOGO:
                setUFO_togo(stk->value.i_data);
                break;

            }
            stk++;
        }
    }

    return 1;
}

size_t NC_STACK_ypaufo::func0(stack_vals *stak)
{
    if ( !NC_STACK_ypabact::func0(stak) )
        return 0;

    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &stack__ypabact;

    ufo->bact_internal = bact;

    bact->bact_type = BACT_TYPES_UFO;

    if ( !ypaufo_func0__sub0(stak) )
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_ypaufo::func1(stack_vals *stak)
{
    return NC_STACK_ypabact::func1(stak);
}

int NC_STACK_ypaufo::ypaufo_func2__sub0(stack_vals *stak)
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

            case BACT_ATT_INPUTTING:
                setBACT_inputting(stk->value.i_data);
                break;

            case UFO_ATT_TOGO:
                setUFO_togo(stk->value.i_data);
                break;
            }
            stk++;
        }
    }

    return 1;
}


size_t NC_STACK_ypaufo::func2(stack_vals *stak)
{
    NC_STACK_ypabact::func2(stak);
    ypaufo_func2__sub0(stak);

    return 1;
}

void NC_STACK_ypaufo::ypaufo_func3__sub0(stack_vals *stak)
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

            case UFO_ATT_TOGO:
                *(int *)stk->value.p_data = getUFO_togo();
                break;

            }
            stk++;
        }
    }
}

size_t NC_STACK_ypaufo::func3(stack_vals *stak)
{
    NC_STACK_ypabact::func3(stak);
    ypaufo_func3__sub0(stak);

    return 1;
}

void NC_STACK_ypaufo::ypabact_func70(ypabact_arg65 *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &stack__ypabact;

    float v110 = arg->field_4 / 1000.0;

    int v94 = getBACT_bactCollisions();

    int v5 = ufo->ywo->ypaworld_func145(bact);

    float v108 = sqrt(  POW2(bact->target_vec.sx) + POW2(bact->target_vec.sy) + POW2(bact->target_vec.sz)  );

    if ( v108 != 0.0 )
    {
        bact->target_dir.sx = bact->target_vec.sx / v108;
        bact->target_dir.sy = bact->target_vec.sy / v108;
        bact->target_dir.sz = bact->target_vec.sz / v108;
    }

    int v8 = !bact->secndTtype && v108 < 1200.0;

    if ( v108 > ufo->field_c )
        v108 = ufo->field_c;

    switch ( bact->status )
    {
    case BACT_STATUS_NORMAL:
    {
        bact->thraction = bact->force;

        int v100 = arg->field_4;

        if ( !v94
                || (!v5 && !v8)
                || !ypabact_func87(&v100) )
        {
            if ( bact->primTtype || bact->secndTtype )
            {
                ufo->field_14 = bact->mass * 9.80665;
                bact->thraction = bact->force;


                if ( ufo->field_1c & 1 )
                {
                    bact->thraction = 0;

                    if ( fabs(bact->fly_dir_length * bact->fly_dir.sx) >= 0.1 || fabs(bact->fly_dir_length * bact->fly_dir.sz) >= 0.1 )
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

                                ufo->field_18 = ufo->field_18 - v17;

                                mat3x3 v81;
                                v81.m00 = cos(v17);
                                v81.m01 = 0;
                                v81.m02 = sin(v17);
                                v81.m10 = 0;
                                v81.m11 = 1.0;
                                v81.m12 = 0;
                                v81.m20 = -sin(v17);
                                v81.m21 = 0;
                                v81.m22 = cos(v17);

                                mat3x3 v83;
                                mat_mult(&v81, &bact->rotation, &v83);

                                bact->rotation = v83;
                            }
                            else
                            {
                                mat3x3 v81;
                                v81.m00 = cos(ufo->field_18);
                                v81.m01 = 0;
                                v81.m02 = sin(ufo->field_18);
                                v81.m10 = 0;
                                v81.m11 = 1.0;
                                v81.m12 = 0;
                                v81.m20 = -sin(ufo->field_18);
                                v81.m21 = 0;
                                v81.m22 = cos(ufo->field_18);

                                mat3x3 v83;
                                mat_mult(&v81, &bact->rotation, &v83);

                                bact->rotation = v83;

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

                bact_arg74 arg74;
                arg74.flag = 0;
                arg74.field_0 = v110;

                ypabact_func74(&arg74);

                ypaworld_arg136 arg136_2;
                arg136_2.pos_x = bact->old_pos.sx;
                arg136_2.pos_y = bact->old_pos.sy;
                arg136_2.pos_z = bact->old_pos.sz;

                float v29 = sqrt( POW2( bact->position.sz - bact->old_pos.sz ) + POW2( bact->position.sx - bact->old_pos.sx ) );

                if ( v29 >= 0.01 )
                {
                    arg136_2.field_14 = (bact->position.sx - bact->old_pos.sx) * 300.0 / v29;
                    arg136_2.field_18 = 0;
                    arg136_2.field_1C = (bact->position.sz - bact->old_pos.sz) * 300.0 / v29;
                }
                else
                {
                    arg136_2.field_14 = bact->rotation.m20 * 300.0;
                    arg136_2.field_18 = bact->rotation.m21 * 300.0;
                    arg136_2.field_1C = bact->rotation.m22 * 300.0;
                }

                ypaworld_arg136 arg136;
                arg136.pos_x = bact->position.sx;
                arg136.pos_y = bact->position.sy;
                arg136.pos_z = bact->position.sz;
                arg136.field_14 = 0;
                arg136.field_18 = bact->height;
                arg136.field_1C = 0;

                arg136_2.field_40 = 0;
                arg136.field_40 = 0;

                ufo->ywo->ypaworld_func136(&arg136);

                ufo->ywo->ypaworld_func136(&arg136_2);

                if ( (arg136_2.field_20 && arg136_2.field_24 * 300.0 < bact->radius) || (arg136.field_20 && arg136.field_24 * 300.0 < bact->radius) )
                {
                    bact_arg88 arg88;

                    if ( arg136_2.field_20 && arg136_2.field_24 * 300.0 < bact->radius )
                    {
                        arg88.pos1.sx = arg136_2.field_3C->triangles[arg136_2.field_38].field_0;
                        arg88.pos1.sy = arg136_2.field_3C->triangles[arg136_2.field_38].field_4;
                        arg88.pos1.sz = arg136_2.field_3C->triangles[arg136_2.field_38].field_8;
                    }
                    else
                    {
                        arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                        arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                        arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;
                    }

                    arg88.pos2.sy = 2.0;
                    arg88.pos2.sz = v110;

                    ypabact_func88(&arg88);
                }

                if ( ufo->field_1c & 1 )
                {
                    if ( ufo->field_1c & 6 )
                    {

                        if ( ufo->field_1c & 2 )
                        {
                            if ( !arg136.field_20 || (arg136.field_20 && arg136.field_24 > 0.8) )
                            {
                                ufo->field_1c &= 0xFFFFFFFC;
                                ufo->field_10 = v108;
                            }
                        }

                        if ( ufo->field_1c & 4 )
                        {
                            if ( arg136.field_20 && arg136.field_24 < 0.8 )
                            {
                                ufo->field_1c &= 0xFFFFFFFA;
                                ufo->field_10 = v108;
                            }
                        }

                    }
                }
                else if ( (arg136_2.field_20 && arg136_2.field_24 < 0.5) || (arg136.field_20 && arg136.field_24 < 0.5) )
                {
                    if ( arg136.field_20 && arg136.field_24 < 0.5 )
                    {
                        ufo->field_1c |= 3;
                    }
                    else
                    {
                        ufo->field_1c |= 9;

                        float v104 = bact->rotation.m22 * arg136_2.field_3C->triangles[arg136_2.field_38].field_8 + bact->rotation.m20 * arg136_2.field_3C->triangles[arg136_2.field_38].field_0;

                        float tmpsq = sqrt( POW2(bact->rotation.m20) + POW2(bact->rotation.m22) );

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        tmpsq = sqrt( POW2(arg136_2.field_3C->triangles[arg136_2.field_38].field_8) + POW2(arg136_2.field_3C->triangles[arg136_2.field_38].field_0) );

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        if ( v104 > 1.0 )
                            v104 = 1.0;

                        if ( v104 < -1.0 )
                            v104 = -1.0;

                        ufo->field_18 = 1.570796326794896 - acos(v104);
                    }
                }
                else if ( ufo->field_10 <= 0.0 )
                {
                    if ( arg136.field_20 )
                    {
                        float v104 = bact->rotation.m20 * bact->target_dir.sx + bact->rotation.m22 * bact->target_dir.sz;

                        float tmpsq = sqrt(POW2(bact->rotation.m20) + POW2(bact->rotation.m22));

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        tmpsq = sqrt(POW2(bact->target_dir.sx) + POW2(bact->target_dir.sz));

                        NDIV_CARRY(tmpsq);

                        v104 /= tmpsq;

                        if ( v104 > 1.0 )
                            v104 = 1.0;

                        if ( v104 < -1.0 )
                            v104 = -1.0;

                        float v121 = acos(v104);

                        if ( v121 > 0.2 )
                        {
                            if ( bact->rotation.m20 * bact->target_dir.sz - bact->rotation.m22 * bact->target_dir.sx < 0.0 )
                                v121 = -v121;

                            ufo->field_18 = v121;
                            ufo->field_1c |= 9;
                        }
                    }
                    else
                    {
                        NC_STACK_ypabact *v102 = ufo->ywo->getYW_userVehicle();;

                        if ( ( (bact->secndTtype != BACT_TGT_TYPE_UNIT || v102 != bact->secndT.pbact->self) && ( bact->primTtype != BACT_TGT_TYPE_UNIT || v102 != bact->primT.pbact->self) ) || bact->target_dir.sy >= 0.0 )
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

                    ypabact_func75(&arg75);
                }
                else if ( bact->secndTtype == BACT_TGT_TYPE_CELL )
                {
                    arg75.pos = bact->sencdTpos;
                    arg75.target.pcell = bact->secndT.pcell;
                    arg75.prio = 1;

                    ypabact_func76(&arg75);
                }
                else if ( bact->primTtype == BACT_TGT_TYPE_UNIT )
                {
                    arg75.target.pbact = bact->primT.pbact;
                    arg75.prio = 0;

                    ypabact_func75(&arg75);
                }
                else if ( bact->primTtype == BACT_TGT_TYPE_CELL )
                {
                    arg75.pos = bact->primTpos;
                    arg75.target.pcell = bact->primT.pcell;
                    arg75.prio = 0;

                    ypabact_func76(&arg75);
                }
                else
                {
                    bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

                    if ( bact->status_flg & BACT_STFLAG_FIRE )
                    {
                        bact_arg119 arg78;
                        arg78.field_8 = 256;
                        arg78.field_0 = 0;
                        arg78.field_4 = 0;

                        ypabact_func78(&arg78);
                    }
                }
            }
            else
            {
                bact->status_flg &= ~(BACT_STFLAG_FIGHT_P | BACT_STFLAG_FIGHT_S);

                bact_arg119 arg78;

                if ( bact->status_flg & BACT_STFLAG_FIRE )
                {
                    arg78.field_0 = 0;
                    arg78.field_4 = 0;
                    arg78.field_8 = 256;

                    ypabact_func78(&arg78);
                }

                arg78.field_0 = 3;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                ypabact_func78(&arg78);
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

            int v63 = ypabact_func110(&arg110);

            int v66 = ypabact_func110(&arg110_1);

            if ( v63 != 3 || v66 != 3 )
            {
                if ( !v63 )
                {
                    bact_arg67 arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_NONE;
                    arg67.priority = 1;

                    ypabact_func67(&arg67);
                }

                if ( !v66 )
                {
                    bact_arg67 arg67;
                    arg67.tgt_type = BACT_TGT_TYPE_CELL;
                    arg67.tgt_pos.sx = ufo->bact_internal->position.sx;
                    arg67.tgt_pos.sz = ufo->bact_internal->position.sz;
                    arg67.priority = 0;

                    ypabact_func67(&arg67);
                }

                if ( bact->primTtype || bact->secndTtype )
                {
                    bact_arg119 arg78;
                    arg78.field_8 = 512;
                    arg78.field_4 = 0;
                    arg78.field_0 = 1;

                    ypabact_func78(&arg78);
                    break;
                }
            }
        }

        int v93 = getBACT_landingOnWait();

        if ( v93 == 0 )
        {
            bact->thraction = 0.0;

            mat3x3 mat2;

            mat2.m00 = cos(bact->maxrot * v110);
            mat2.m01 = 0.0;
            mat2.m02 = sin(bact->maxrot * v110);

            mat2.m10 = 0.0;
            mat2.m11 = 1.0;
            mat2.m12 = 0.0;
            mat2.m20 = -sin(bact->maxrot * v110);
            mat2.m21 = 0.0;
            mat2.m22 = cos(bact->maxrot * v110);

            mat3x3 v82;
            mat_mult(&bact->rotation, &mat2, &v82);

            bact->rotation = v82;
        }
        else
        {
            bact->thraction = 0.0;

            ufo->field_14 = 0;

            if ( bact->status_flg & BACT_STFLAG_LAND )
            {
                bact_arg119 arg78;
                arg78.field_8 = 0;
                arg78.field_4 = 0;
                arg78.field_0 = 3;

                ypabact_func78(&arg78);

                ypaworld_arg136 arg136_1;

                arg136_1.pos_x = bact->position.sx;
                arg136_1.pos_y = bact->position.sy;
                arg136_1.pos_z = bact->position.sz;
                arg136_1.field_14 = 0;
                arg136_1.field_18 = bact->overeof + 50.0;
                arg136_1.field_1C = 0;
                arg136_1.field_40 = 0;

                ufo->ywo->ypaworld_func136(&arg136_1);

                if ( arg136_1.field_20 )
                    bact->position.sy = arg136_1.field_30 - bact->overeof;
            }
            else
            {
                bact_arg86 arg86;
                arg86.field_one = 0;
                arg86.field_two = arg->field_4;

                ypabact_func86(&arg86);
            }
        }
    }
    break;

    case BACT_STATUS_DEAD:
        ypabact_func121(arg);
        break;

    case BACT_STATUS_CREATE:
        ypabact_func99(arg);
        break;

    case BACT_STATUS_BEAM:
        ypabact_func112(arg);
        break;
    }
}

void NC_STACK_ypaufo::ypabact_func71(ypabact_arg65 *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &stack__ypabact;

    float v88 = arg->field_4 / 1000.0;

    int a4 = getBACT_bactCollisions();

    bact->old_pos = bact->position;

    if ( bact->status == BACT_STATUS_DEAD )
    {
        ypabact_func121(arg);
    }
    else if ( bact->status == BACT_STATUS_NORMAL || bact->status == BACT_STATUS_IDLE )
    {

        if ( bact->fly_dir_length != 0.0 )
        {
            if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
            {
                bact_arg119 arg78;
                arg78.field_0 = 1;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                ypabact_func78(&arg78);
            }

            bact->status_flg &= ~BACT_STFLAG_LAND;
        }
        else
        {
            ypaworld_arg136 arg136;
            arg136.pos_x = bact->position.sx;
            arg136.pos_y = bact->position.sy;
            arg136.pos_z = bact->position.sz;
            arg136.field_14 = 0;
            arg136.field_1C = 0;

            if ( bact->viewer_overeof <= bact->viewer_radius )
                arg136.field_18 = bact->viewer_radius * 1.5;
            else
                arg136.field_18 = bact->viewer_overeof * 1.5;

            arg136.field_40 = 0;

            ufo->ywo->ypaworld_func136(&arg136);

            if ( !arg136.field_20 || bact->thraction != 0.0 || ufo->field_14 > bact->mass * 9.80665 )
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
                    || sqrt( POW2(bact->primTpos.sx - bact->position.sx) + POW2(bact->primTpos.sz - bact->position.sz) ) <= 800.0 )
            {
                if ( bact->status_flg & BACT_STFLAG_LAND )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                    {
                        bact_arg119 arg78;
                        arg78.field_0 = 3;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;

                        ypabact_func78(&arg78);
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
                        bact_arg119 arg78;
                        arg78.field_0 = 3;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;

                        ypabact_func78(&arg78);
                    }
                }
            }
        }

        float v23 = -arg->inpt->sliders_vars[0] * bact->maxrot * v88;

        if ( fabs(v23) > 0.0 )
        {
            mat3x3 mat2;

            mat2.m00 = cos(v23);
            mat2.m01 = 0;
            mat2.m02 = sin(v23);
            mat2.m10 = 0;
            mat2.m11 = 1.0;
            mat2.m12 = 0;
            mat2.m20 = -sin(v23);
            mat2.m21 = 0;
            mat2.m22 = cos(v23);

            mat3x3 dst;
            mat_mult(&bact->rotation, &mat2, &dst);

            bact->rotation = dst;
        }

        float v25 = arg->inpt->sliders_vars[1] * bact->maxrot * v88;

        if ( fabs(v25) > 0.0 )
        {
            mat3x3 mat1;

            mat1.m00 = 1.0;
            mat1.m01 = 0;
            mat1.m02 = 0;
            mat1.m10 = 0;
            mat1.m11 = cos(v25);
            mat1.m12 = sin(v25);
            mat1.m20 = 0;
            mat1.m21 = -sin(v25);
            mat1.m22 = cos(v25);

            mat3x3 dst;
            mat_mult(&mat1, &bact->rotation, &dst);

            bact->rotation = dst;
        }

        if ( arg->inpt->sliders_vars[2] != 0.0 )
        {
            ufo->field_14 = (arg->inpt->sliders_vars[2] * 4.0 + 1.0) * bact->mass * 9.80665;

            float v85 = bact->pSector->height - bact->position.sy;
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
                int v83 = arg->field_4;

                ypabact_func87(&v83);
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
        arg79.tgt_pos.sx = bact->rotation.m20;
        arg79.tgt_pos.sy = bact->rotation.m21;
        arg79.tgt_pos.sz = bact->rotation.m22;

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4.sx = bact->rotation.m20;
        arg106.field_4.sy = bact->rotation.m21;
        arg106.field_4.sz = bact->rotation.m22;


        if ( ypabact_func106(&arg106) )
        {
            arg79.target.pbact = arg106.ret_bact;
            arg79.tgType = BACT_TGT_TYPE_UNIT;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.weapon = bact->weapon;
            arg79.direction.sx = bact->rotation.m20;
            arg79.direction.sy = bact->rotation.m21;
            arg79.direction.sz = bact->rotation.m22;
            arg79.tgType = BACT_TGT_TYPE_NONE;
            arg79.g_time = bact->clock;

            if ( bact->clock % 2 )
                arg79.start_point.sx = -bact->fire_pos.sx;
            else
                arg79.start_point.sx = bact->fire_pos.sx;

            arg79.start_point.sy = bact->fire_pos.sy;
            arg79.start_point.sz = bact->fire_pos.sz;
            arg79.flags = (arg->inpt->but_flags & 2) != 0;

            ypabact_func79(&arg79);
        }

        if ( bact->weapon == -1 )
        {
            if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
            {
                if ( bact->thraction < bact->force )
                {
                    bact->thraction += (float)arg->field_4 * bact->force * 0.0099999998;
                }
            }
            else if ( bact->thraction > 0.0 )
            {
                bact->thraction -= (float)arg->field_4 * bact->force * 0.001;
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

            if ( fabs(bact->fly_dir.sx * bact->fly_dir_length) > 0.1 || fabs(bact->fly_dir.sz * bact->fly_dir_length) > 0.1 )
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
                    bact_arg119 arg78;
                    arg78.field_4 = 0;
                    arg78.field_0 = 0;
                    arg78.field_8 = 256;

                    ypabact_func78(&arg78);
                }
            }

            if ( arg->inpt->but_flags & 4 )
            {
                if ( !(bact->status_flg & BACT_STFLAG_FIRE) )
                {
                    bact_arg119 arg78;
                    arg78.field_4 = 256;
                    arg78.field_8 = 0;
                    arg78.field_0 = 0;

                    ypabact_func78(&arg78);
                }

                bact_arg105 arg105;

                arg105.field_0.sx = bact->rotation.m20;
                arg105.field_0.sy = bact->rotation.m21;
                arg105.field_0.sz = bact->rotation.m22;
                arg105.field_C = v88;
                arg105.field_10 = bact->clock;

                ypabact_func105(&arg105);
            }
        }

        if ( bact->status_flg & BACT_STFLAG_LAND )
        {
            bact_arg74 arg74;
            arg74.flag = 0;
            arg74.field_0 = v88;

            ypabact_func74(&arg74);
        }
        else
        {
            yw_137col v60[10];

            for (int i = 3; i >= 0; i--)
            {
                bact_arg74 arg74;
                arg74.flag = 0;
                arg74.field_0 = v88;

                ypabact_func74(&arg74);

                ypaworld_arg137 arg137;
                arg137.pos = bact->position;
                arg137.pos2 = bact->fly_dir;
                arg137.radius = bact->viewer_radius;
                arg137.collisions = v60;
                arg137.coll_max = 10;
                arg137.field_30 = 0;

                ufo->ywo->ypaworld_func137(&arg137);

                int v49 = 0;

                float v94 = 0.0;
                float v93 = 0.0;
                float v92 = 0.0;

                if ( arg137.coll_count )
                {


                    for (int j = 0 ; j < arg137.coll_count; j++)
                    {
                        yw_137col *v50 = &arg137.collisions[j];

                        v93 += v50->pos2.sx;
                        v94 += v50->pos2.sz;
                        v92 += v50->pos2.sy;
                    }

                    float v89 = sqrt( POW2(v93) + POW2(v92) + POW2(v94) );

                    bact_arg88 arg88;

                    if ( v89 != 0.0 )
                    {
                        arg88.pos1.sx = v93 / v89;
                        arg88.pos1.sy = v92 / v89;
                        arg88.pos1.sz = v94 / v89;
                    }
                    else
                        arg88.pos1 = bact->fly_dir;

                    bact->thraction = 0;

                    arg88.pos2.sz = v88;
                    arg88.pos2.sx = 0.7;
                    arg88.pos2.sy = 2.0;

                    ypabact_func88(&arg88);

                    v49 = 1;
                }

                if ( v49 == 0 )
                {
                    ypaworld_arg136 arg136;
                    arg136.pos_x = bact->old_pos.sx;
                    arg136.pos_y = bact->old_pos.sy;
                    arg136.pos_z = bact->old_pos.sz;

                    arg136.field_14 = bact->position.sx - bact->old_pos.sx;
                    arg136.field_18 = bact->position.sy - bact->old_pos.sy;
                    arg136.field_1C = bact->position.sz - bact->old_pos.sz;

                    arg136.field_40 = 0;

                    ufo->ywo->ypaworld_func136(&arg136);

                    if ( arg136.field_20 )
                    {
                        bact->thraction = 0;

                        bact_arg88 arg88;

                        arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                        arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                        arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;

                        arg88.pos2.sx = 0.7;
                        arg88.pos2.sz = v88;
                        arg88.pos2.sy = 2.0;

                        ypabact_func88(&arg88);

                        v49 = 1;
                    }
                }

                if ( v49 == 0 )
                {
                    bact->status_flg &= ~BACT_STFLAG_LCRASH;
                    break;
                }

                if ( !(bact->soundcarrier.samples_data[5].field_12 & 2) )
                {
                    if ( !(bact->status_flg & BACT_STFLAG_LCRASH) )
                    {
                        bact->status_flg |= BACT_STFLAG_LCRASH;

                        startSound(&bact->soundcarrier, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v93 * 10.0 + bact->position.sx;
                        arg180.field_C = v94 * 10.0 + bact->position.sz;

                        ufo->ywo->ypaworld_func180(&arg180);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypaufo::ypabact_func74(bact_arg74 *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;
    __NC_STACK_ypabact *bact = &stack__ypabact;

    bact->old_pos = bact->position;

    float v55;

    if ( bact->status == BACT_STATUS_DEAD )
        v55 = bact->mass * 39.2266;
    else
        v55 = bact->mass * 9.80665;

    float v47, v48, v59;

    if ( arg->flag & 1 )
    {
        v47 = 0.0;
        v48 = 0.0;
        v59 = 0.0;
    }
    else
    {
        float v9 = sqrt( POW2(bact->rotation.m20) + POW2(bact->rotation.m22) );

        if ( v9 <= 0.001 )
        {
            v47 = -bact->rotation.m10;
            v48 = -bact->rotation.m12;

            v59 = ufo->field_14;
        }
        else
        {
            v47 = bact->rotation.m20 / v9;
            v48 = bact->rotation.m22 / v9;

            v59 = bact->thraction;
        }
    }

    float v44, v54;

    if ( arg->flag & 1 )
    {
        v44 = 0.0;
        v54 = 0.0;
    }
    else
    {
        v44 = -1.0;
        v54 = ufo->field_14;
    }

    float v18 = fabs(bact->fly_dir_length) * bact->airconst;

    float v45 = 0.0 * v55 + v47 * v59 + -bact->fly_dir.sx * v18 + 0.0 * v54;
    float v46 = 1.0 * v55 + 0.0 * v59 + -bact->fly_dir.sy * v18 + v44 * v54;
    float v49 = 0.0 * v55 + v48 * v59 + -bact->fly_dir.sz * v18 + 0.0 * v54;

    float v52 = sqrt( POW2(v45) + POW2(v46) + POW2(v49) );

    if ( v52 > 0.0 )
    {
        float v26 = v52 / bact->mass * arg->field_0;

        float v56 = bact->fly_dir.sy * bact->fly_dir_length + v46 * v26 / v52;
        float v42 = bact->fly_dir.sx * bact->fly_dir_length + v45 * v26 / v52;
        float v43 = bact->fly_dir.sz * bact->fly_dir_length + v49 * v26 / v52;

        float v51 = sqrt( POW2(v56) + POW2(v42) + POW2(v43) );

        if ( v51 > 0.0 )
        {
            v42 /= v51;
            v56 /= v51;
            v43 /= v51;
        }

        bact->fly_dir.sx = v42;
        bact->fly_dir.sy = v56;
        bact->fly_dir.sz = v43;

        bact->fly_dir_length = v51;
    }

    float a1x = bact->fly_dir.sx * bact->fly_dir_length * arg->field_0 * 6.0;
    float a1y = bact->fly_dir.sy * bact->fly_dir_length * arg->field_0 * 6.0;
    float a1z = bact->fly_dir.sz * bact->fly_dir_length * arg->field_0 * 6.0;

    if ( fabs(bact->fly_dir_length) > 0.1 )
    {
        bact->position.sx += a1x;
        bact->position.sy += a1y;
        bact->position.sz += a1z;
    }

    ufo->field_10 -= sqrt( POW2(a1x) + POW2(a1y) + POW2(a1z) );

    if ( ufo->field_10 < 0.0 )
        ufo->field_10 = 0;

    ypabact_func115(NULL);

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

size_t NC_STACK_ypaufo::ypabact_func80(bact_arg80 *arg)
{
    __NC_STACK_ypaufo *ufo = &stack__ypaufo;

    if ( !NC_STACK_ypabact::ypabact_func80(arg))
        return 0;

    ufo->field_14 = ufo->bact_internal->mass * 9.80665;
    return 1;
}

void NC_STACK_ypaufo::ypabact_func96(void *)
{
    NC_STACK_ypabact::ypabact_func96(NULL);

    __NC_STACK_ypaufo *ufo = &stack__ypaufo;

    ufo->field_14 = 0;

    setBACT_landingOnWait(0);
}


void NC_STACK_ypaufo::setBACT_inputting(int inpt)
{
    NC_STACK_ypabact::setBACT_inputting(inpt);

    if ( !inpt )
    {
        __NC_STACK_ypabact *bact = &stack__ypabact;
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
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 70:
        ypabact_func70( (ypabact_arg65 *)data );
        return 1;
    case 71:
        ypabact_func71( (ypabact_arg65 *)data );
        return 1;
    case 74:
        ypabact_func74( (bact_arg74 *)data );
        return 1;
    case 80:
        return (size_t)ypabact_func80( (bact_arg80 *)data );
    case 96:
        ypabact_func96( (void *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_ypabact::compatcall(method_id, data);
}
