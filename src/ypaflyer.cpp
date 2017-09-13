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


void sb_0x4b255c__sub0(__NC_STACK_ypabact *bact, float a2)
{
    float v27 = sqrt( POW2(bact->rotation.m00) + POW2(bact->rotation.m02) );

    if ( v27 >= 0.001 )
    {
        vec3d vaxis;
        vaxis.x = bact->rotation.m00 / v27;
        vaxis.y = 0.0;
        vaxis.z = bact->rotation.m02 / v27;

        mat3x3 mat2;

        mat_gen_axis_rotate(&vaxis, a2, &mat2, MAT_FLAG_INV_SIN);

        mat3x3 v16;
        mat_mult(&bact->rotation, &mat2, &v16);

        bact->rotation = v16;
    }
}

void sb_0x4b255c(__NC_STACK_ypaflyer *fly, float a2, vec3d *a3, int a4)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    float v38 = bact->force / bact->airconst;

    if ( bact->fly_dir.x != 0.0 || bact->fly_dir.y != 0.0 || bact->fly_dir.z != 0.0 )
    {
        if ( a3->x == 0.0 && a3->y == 0.0 && a3->z == 0.0 )
        {
            a3->x = bact->rotation.m20;
            a3->y = bact->rotation.m21;
            a3->z = bact->rotation.m22;
        }

        float v30 = bact->rotation.m20;
        //float v31 = bact->field_651.m21;
        float v32 = bact->rotation.m22;

        float v53 = bact->rotation.m01;

        if ( v53 > 1.0 )
            v53 = 1.0;

        if ( v53 < -1.0 )
            v53 = -1.0;

        float v56 = asin(v53);

        float v54 = bact->rotation.m21;

        if ( v54 > 1.0 )
            v54 = 1.0;

        if ( v54 < -1.0 )
            v54 = -1.0;

        float v52 = asin(v54);

        float v49;

        if ( fly->field_10 & 1 && bact->fly_dir_length >= 0.1 )
        {
            float v55 = bact->fly_dir.y;

            if ( v55 > 1.0 )
                v55 = 1.0;

            if ( v55 < -1.0 )
                v55 = -1.0;

            v49 = asin(v55) * bact->fly_dir_length / v38;
        }
        else
        {
            v49 = 0.0;
        }

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
            float v42 = bact->maxrot * a2 * v45;

            if ( v51 <= v42 )
            {
                if ( v51 < 0.0005 )
                    v51 = 0.0;
            }
            else
            {
                v51 = v42;
            }
        }
        else
        {
            float v46 = -bact->maxrot * a2 * v45;
            if ( v51 >= v46 )
            {
                if ( v51 > -0.0005 )
                    v51 = 0.0;
            }
            else
            {
                v51 = v46;
            }
        }

        sb_0x4b255c__sub0(bact, v51);

        if ( fly->field_10 & 2 )
        {
            if ( (v30 != 0.0 || v32 != 0.0) && (a3->x != 0.0 || a3->z != 0.0) )
            {
                float v37 = (v32 * a3->z + v30 * a3->x) / sqrt( POW2(v32) + POW2(v30) );
                v37 = v37 / sqrt( POW2(a3->z) + POW2(a3->x) );

                float v57 = v37;

                if ( v57 > 1.0 )
                    v57 = 1.0;

                if ( v57 < -1.0 )
                    v57 = -1.0;

                v57 = acos(v57);

                if ( v57 < 0.001 )
                    v57 = 0.0;

                if ( v30 * a3->z - v32 * a3->x < 0.0 )
                    v57 = -v57;

                float v58;

                if ( bact->fly_dir_length < 0.1 )
                    v58 = 0.0;
                else
                {
                    float aa2 = a2;

                    NDIV_CARRY(aa2);

                    v58 = fabs(bact->fly_dir_length) * bact->airconst / bact->force * (v57 * 1.45 / (aa2 * bact->maxrot));
                }


                float v44;
                if ( a4 )
                {
                    v44 = 1.0;
                }
                else
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

                mat3x3 mat1;

                mat1.m00 = cos(v50);
                mat1.m01 = sin(v50);
                mat1.m02 = 0;

                mat1.m10 = -sin(v50);
                mat1.m11 = cos(v50);
                mat1.m12 = 0;

                mat1.m20 = 0;
                mat1.m21 = 0;
                mat1.m22 = 1.0;

                mat3x3 v26;

                mat_mult(&mat1, &bact->rotation, &v26);

                bact->rotation = v26;
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


            mat3x3 mat1;

            mat1.m00 = cos(-v56);
            mat1.m01 = sin(-v56);
            mat1.m02 = 0;

            mat1.m10 = -sin(-v56);
            mat1.m11 = cos(-v56);
            mat1.m12 = 0;

            mat1.m20 = 0;
            mat1.m21 = 0;
            mat1.m22 = 1.0;

            mat3x3 v26;

            mat_mult(&mat1, &bact->rotation, &v26);

            bact->rotation = v26;
        }
    }
}


void ypaflyer_func70__sub0(__NC_STACK_ypaflyer *fly, float angl)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    if ( bact->target_dir.x != 0.0 || bact->target_dir.z != 0.0 )
    {

        float v6 = (bact->target_dir.x * bact->rotation.m20 + bact->target_dir.z * bact->rotation.m22);

        float tmpsq = sqrt( POW2(bact->target_dir.x) + POW2(bact->target_dir.z) );

        NDIV_CARRY(tmpsq);

        v6 /= tmpsq;

        tmpsq = sqrt(POW2(bact->rotation.m20) + POW2(bact->rotation.m22));

        NDIV_CARRY(tmpsq);

        v6 /= tmpsq;

        if ( v6 > 1.0 )
            v6 = 1.0;

        if ( v6 < -1.0 )
            v6 = -1.0;

        float v21 = acos(v6);

        float v9 = bact->maxrot * angl;

        if ( fabs(v21) > v9 )
            v21 = v9;

        if ( bact->rotation.m22 * bact->target_dir.x - bact->rotation.m20 * bact->target_dir.z > 0.0 )
            v21 = -v21;

        float v19 = cos(v21);
        float v18 = sin(v21);

        mat3x3 v16;

        v16.m00 = v19;
        v16.m01 = 0;
        v16.m02 = v18;
        v16.m10 = 0;
        v16.m11 = 1.0;
        v16.m12 = 0;
        v16.m20 = -v18;
        v16.m21 = 0;
        v16.m22 = v19;

        mat3x3 dst;

        mat_mult(&v16, &bact->rotation, &dst);

        bact->rotation = dst;
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

    float v88 = sqrt( POW2(bact->target_vec.x) + POW2(bact->target_vec.y) + POW2(bact->target_vec.z) );

    if ( v88 > 0.0 )
    {
        bact->target_dir.x = bact->target_vec.x / v88;
        bact->target_dir.y = bact->target_vec.y / v88;
        bact->target_dir.z = bact->target_vec.z / v88;
    }

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
        vec3d a3;
        a3.x = bact->rotation.m20;
        a3.y = bact->rotation.m21;
        a3.z = bact->rotation.m22;

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
        arg136.field_20 = 0;
        arg136.field_40 = 0;
        arg136.pos_x = bact->old_pos.x;
        arg136.pos_y = bact->old_pos.y;
        arg136.pos_z = bact->old_pos.z;
        arg136.field_14 = bact->position.x - bact->old_pos.x;
        arg136.field_18 = 0;
        arg136.field_1C = bact->position.z - bact->old_pos.z;

        float v89 = sqrt( POW2(arg136.field_14) + POW2(arg136.field_1C) );

        if ( v89 <= 0.0 )
        {
            arg136.field_14 = bact->rotation.m20 * 300.0;
            arg136.field_18 = bact->rotation.m21 * 300.0;
            arg136.field_1C = bact->rotation.m22 * 300.0;
        }
        else
        {
            float v14 = 300.0 / v89;
            arg136.field_14 *= v14;
            arg136.field_18 *= v14;
            arg136.field_1C *= v14;
        }

        ypaworld_arg136 arg136_1;

        arg136_1.field_20 = 0;
        arg136_1.field_40 = 0;

        if ( a4 || bact->status_flg & BACT_STFLAG_DODGE_RIGHT || (v90 && v82) )
        {
            arg136_1.pos_x = bact->old_pos.x;
            arg136_1.pos_y = bact->old_pos.y;
            arg136_1.pos_z = bact->old_pos.z;

            arg136_1.field_14 = arg136.field_14 * 0.93969 - arg136.field_1C * 0.34202;
            arg136_1.field_18 = arg136.field_18;
            arg136_1.field_1C = arg136.field_1C * 0.93969 + arg136.field_14 * 0.34202;

            fly->ywo->ypaworld_func136(&arg136_1);
        }

        ypaworld_arg136 arg136_2;

        arg136_2.field_20 = 0;
        arg136_2.field_40 = 0;

        if ( a4 || bact->status_flg & BACT_STFLAG_DODGE_LEFT || (v90 && v82) )
        {
            arg136_2.pos_x = bact->old_pos.x;
            arg136_2.pos_y = bact->old_pos.y;
            arg136_2.pos_z = bact->old_pos.z;
            arg136_2.field_14 = arg136.field_14 * 0.93969 + arg136.field_1C * 0.34202;
            arg136_2.field_18 = arg136.field_18;
            arg136_2.field_1C = arg136.field_1C * 0.93969 - arg136.field_14 * 0.34202;

            fly->ywo->ypaworld_func136(&arg136_2);
        }

        if ( a4 || !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) || (v90 && v82) )
            fly->ywo->ypaworld_func136(&arg136);

        int v18 = 0;

        bact_arg88 arg88;
        arg88.pos1.x = 0;
        arg88.pos1.y = 0;
        arg88.pos1.z = 0;

        if ( arg136.field_20 )
        {
            if ( v89 + v91 > arg136.field_24 * 300.0 )
            {
                arg88.pos1.x = arg136.field_3C->polygons[arg136.field_38].A;
                arg88.pos1.y = arg136.field_3C->polygons[arg136.field_38].B;
                arg88.pos1.z = arg136.field_3C->polygons[arg136.field_38].C;

                v18++;
            }
        }

        if ( arg136_1.field_20 )
        {
            if ( v89 + v91 > arg136_1.field_24 * 300.0 )
            {
                arg88.pos1.x += arg136_1.field_3C->polygons[arg136_1.field_38].A;
                arg88.pos1.y += arg136_1.field_3C->polygons[arg136_1.field_38].B;
                arg88.pos1.z += arg136_1.field_3C->polygons[arg136_1.field_38].C;

                v18++;
            }
        }

        if ( arg136_2.field_20 )
        {
            if ( v89 + v91 > arg136_2.field_24 * 300.0 )
            {
                arg88.pos1.x += arg136_2.field_3C->polygons[arg136_2.field_38].A;
                arg88.pos1.y += arg136_2.field_3C->polygons[arg136_2.field_38].B;
                arg88.pos1.z += arg136_2.field_3C->polygons[arg136_2.field_38].C;

                v18++;
            }
        }

        if ( v18 )
        {
            float v19 = (float)v18;

            arg88.pos2.x = 0.7;
            arg88.pos2.y = 2.0;
            arg88.pos2.z = a2a;
            arg88.pos1.x /= v19;
            arg88.pos1.y /= v19;
            arg88.pos1.z /= v19;

            Recoil(&arg88);
        }
        else
        {
            bact->status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( !arg136.field_20 && !arg136_1.field_20 && !arg136_2.field_20 )
        {
            bact->status_flg &= ~(BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT);
            bact->status_flg |= BACT_STFLAG_MOVE;
        }

        if ( !(bact->status_flg & (BACT_STFLAG_DODGE_LEFT | BACT_STFLAG_DODGE_RIGHT)) )
        {
            if ( arg136_1.field_20 == 1 && arg136_2.field_20 == 1 )
            {
                if ( arg136_1.field_24 >= arg136_2.field_24 )
                    bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
                else
                    bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;
            }

            if ( arg136_1.field_20 == 1 && !arg136_2.field_20 )
                bact->status_flg |= BACT_STFLAG_DODGE_RIGHT;

            if ( !arg136_1.field_20 && arg136_2.field_20 == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;

            if ( !arg136_1.field_20 && !arg136_2.field_20 && arg136.field_20 == 1 )
                bact->status_flg |= BACT_STFLAG_DODGE_LEFT;
        }

        ypaworld_arg136 arg136_3;

        arg136_3.pos_x = bact->old_pos.x;
        arg136_3.pos_y = bact->old_pos.y;
        arg136_3.pos_z = bact->old_pos.z;

        float v22 = bact->mass * 9.80665;

        if ( v22 <= bact->force )
            v22 = bact->force;

        float v100 = bact->airconst;

        if ( v100 < 10.0 )
            v100 = 10.0;

        arg136_3.field_14 = (bact->fly_dir.x * 200.0 * bact->fly_dir_length) / (v22 / v100);

        if ( arg136_3.field_14 < -200.0 )
            arg136_3.field_14 = -200.0;

        if ( arg136_3.field_14 > 200.0 )
            arg136_3.field_14 = 200.0;

        arg136_3.field_18 = bact->height;

        arg136_3.field_1C = (bact->fly_dir.z * 200.0 * bact->fly_dir_length) / (v22 / v100);

        if ( arg136_3.field_1C < -200.0 )
            arg136_3.field_1C = -200.0;

        if ( arg136_3.field_1C > 200.0 )
            arg136_3.field_1C = 200.0;

        arg136_3.field_40 = 0;

        fly->ywo->ypaworld_func136(&arg136_3);

        if ( arg136_3.field_20 )
        {
            bact->target_dir.y = -(1.0 - arg136_3.field_24);
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

        if ( arg136_3.field_20 && arg136_3.field_24 * bact->height < bact->radius && bact->fly_dir.y > 0.0 )
        {
            arg88.pos1.x = arg136_3.field_3C->polygons[arg136_3.field_38].A;
            arg88.pos1.y = arg136_3.field_3C->polygons[arg136_3.field_38].B;
            arg88.pos1.z = arg136_3.field_3C->polygons[arg136_3.field_38].C;

            arg88.pos2.x = 0.7;
            arg88.pos2.y = 2.0;
            arg88.pos2.z = a2a;

            Recoil(&arg88);
        }
        else
        {
            bact->status_flg &= ~BACT_STFLAG_LCRASH;
        }

        if ( bact->target_dir.y != 0.0 )
        {
            float v39 = sqrt( POW2(bact->target_dir.y) + POW2(bact->target_dir.x) + POW2(bact->target_dir.z) );

            bact->target_dir.x /= v39;
            bact->target_dir.y /= v39;
            bact->target_dir.z /= v39;
        }

        float v92 = sqrt(POW2(arg136.field_14) + POW2(arg136.field_1C));

        NDIV_CARRY(v92);

        if ( bact->status_flg & BACT_STFLAG_DODGE_LEFT )
        {
            bact->target_dir.x = -arg136.field_1C / v92;
            bact->target_dir.z  = arg136.field_14 / v92;
        }
        else if ( bact->status_flg & BACT_STFLAG_DODGE_RIGHT )
        {
            bact->target_dir.x = arg136.field_1C  / v92;
            bact->target_dir.z  = -arg136.field_14  / v92;
        }

        ypaflyer_func70__sub0(fly, a2a);

        move_msg arg74;
        arg74.flag = 0;
        arg74.field_0 = a2a;

        Move(&arg74);

        sb_0x4b255c(fly, a2a, &a3, 0);


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
                arg136_4.pos_x = bact->position.x;
                arg136_4.pos_y = bact->position.y;
                arg136_4.pos_z = bact->position.z;
                arg136_4.field_14 = 0;
                arg136_4.field_18 = bact->overeof + 50.0;
                arg136_4.field_1C = 0;
                arg136_4.field_40 = 0;

                fly->ywo->ypaworld_func136(&arg136_4);

                if ( arg136_4.field_20 )
                    bact->position.y = arg136_4.field_30 - bact->overeof;
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

void ypaflyer_func71__sub1(__NC_STACK_ypabact *bact, float a4)
{
    mat3x3 mat2;
    mat2.m00 = cos(a4);
    mat2.m01 = 0;
    mat2.m02 = sin(a4);
    mat2.m10 = 0;
    mat2.m11 = 1.0;
    mat2.m12 = 0;
    mat2.m20 = -sin(a4);
    mat2.m21 = 0;
    mat2.m22 = cos(a4);

    mat3x3 v6;

    mat_mult(&bact->rotation, &mat2, &v6);

    bact->rotation = v6;
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
        a3.x = fly->bact_internal->rotation.m20;
        a3.y = fly->bact_internal->rotation.m21;
        a3.z = fly->bact_internal->rotation.m22;

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

            arg136.pos_x = fly->bact_internal->position.x;
            arg136.pos_y = fly->bact_internal->position.y;
            arg136.pos_z = fly->bact_internal->position.z;
            arg136.field_1C = 0;
            arg136.field_14 = 0;

            float v9;

            if ( fly->bact_internal->viewer_overeof <= fly->bact_internal->viewer_radius )
                v9 = fly->bact_internal->viewer_radius;
            else
                v9 = fly->bact_internal->viewer_overeof;

            arg136.field_40 = 0;
            arg136.field_18 = v9 * 1.5;

            fly->ywo->ypaworld_func136(&arg136);

            if ( arg136.field_20 && fly->bact_internal->thraction < fly->bact_internal->force * 0.001 && fly->field_c <= fly->bact_internal->mass * 9.80665 )
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

            float tmp;

            if ( fly->bact_internal->primTtype == BACT_TGT_TYPE_CELL)
            {
                float v13 = fly->bact_internal->primTpos.x - fly->bact_internal->position.x;
                float v14 = fly->bact_internal->primTpos.z - fly->bact_internal->position.z;

                tmp = sqrt(POW2(v13) + POW2(v14));
            }

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

        ypaflyer_func71__sub1(fly->bact_internal, v60);

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
        arg79.tgt_pos.x = fly->bact_internal->rotation.m20;
        arg79.tgt_pos.y = fly->bact_internal->rotation.m21;
        arg79.tgt_pos.z = fly->bact_internal->rotation.m22;

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4.x = fly->bact_internal->rotation.m20;
        arg106.field_4.y = fly->bact_internal->rotation.m21;
        arg106.field_4.z = fly->bact_internal->rotation.m22;

        if ( UserTargeting(&arg106) )
        {
            arg79.tgType = BACT_TGT_TYPE_UNIT;
            arg79.target.pbact = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.direction.y = 0;
            arg79.direction.x = 0;
            arg79.direction.z = 0;
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

                arg105.field_0.x = fly->bact_internal->rotation.m20;
                arg105.field_0.y = fly->bact_internal->rotation.m21;
                arg105.field_0.z = fly->bact_internal->rotation.m22;
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
            float v68 = 0.0;
            float v69 = 0.0;
            float v70 = 0.0;

            yw_137col v43[10];

            for (int i = 3; i >= 0; i--)
            {
                int v67 = 0;

                move_msg arg74;
                arg74.field_0 = a2;
                arg74.flag = 0;

                Move(&arg74);

                ypaworld_arg137 arg137;

                arg137.pos.x = fly->bact_internal->rotation.m20 * 0.5 * fly->bact_internal->viewer_radius + fly->bact_internal->position.x;
                arg137.pos.y = fly->bact_internal->rotation.m21 * 0.5 * fly->bact_internal->viewer_radius + fly->bact_internal->position.y;
                arg137.pos.z = fly->bact_internal->rotation.m22 * 0.5 * fly->bact_internal->viewer_radius + fly->bact_internal->position.z;
                arg137.pos2.x = fly->bact_internal->fly_dir.x;
                arg137.pos2.y = fly->bact_internal->fly_dir.y;
                arg137.pos2.z = fly->bact_internal->fly_dir.z;
                arg137.radius = fly->bact_internal->viewer_radius;
                arg137.coll_max = 10;
                arg137.field_30 = 0;
                arg137.collisions = v43;

                fly->ywo->ypaworld_func137(&arg137);

                if ( arg137.coll_count )
                {
                    v68 = 0.0;
                    v69 = 0.0;
                    v70 = 0.0;

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                    {
                        yw_137col *v31 = &arg137.collisions[ j ];

                        v69 += v31->pos2.x;
                        v68 += v31->pos2.y;
                        v70 += v31->pos2.z;
                    }

                    float v65 = sqrt( POW2(v69) + POW2(v68) + POW2(v70) );

                    vec3d v52;

                    if ( v65 != 0.0 )
                    {
                        v52.x = v69 / v65;
                        v52.y = v68 / v65;
                        v52.z = v70 / v65;
                    }
                    else
                    {
                        v52 = fly->bact_internal->fly_dir;
                    }

                    bact_arg88 arg88;
                    arg88.pos1 = v52;
                    arg88.pos2.x = 0.7;
                    arg88.pos2.y = 2.0;
                    arg88.pos2.z = a2;

                    Recoil(&arg88);

                    fly->field_c = fly->bact_internal->mass * 9.80665;

                    v67 = 1;
                }

                if ( !v67 )
                {
                    ypaworld_arg136 arg136;
                    arg136.pos_x = fly->bact_internal->old_pos.x;
                    arg136.pos_y = fly->bact_internal->old_pos.y;
                    arg136.pos_z = fly->bact_internal->old_pos.z;
                    arg136.field_14 = fly->bact_internal->position.x - fly->bact_internal->old_pos.x;
                    arg136.field_18 = fly->bact_internal->position.y - fly->bact_internal->old_pos.y;
                    arg136.field_1C = fly->bact_internal->position.z - fly->bact_internal->old_pos.z;
                    arg136.field_40 = 0;

                    fly->ywo->ypaworld_func149(&arg136);

                    if ( arg136.field_20 )
                    {
                        bact_arg88 arg88;
                        arg88.pos1.x = arg136.field_3C->polygons[ arg136.field_38 ].A;
                        arg88.pos1.y = arg136.field_3C->polygons[ arg136.field_38 ].B;
                        arg88.pos1.z = arg136.field_3C->polygons[ arg136.field_38 ].C;
                        arg88.pos2.x = 0.7;
                        arg88.pos2.y = 2.0;
                        arg88.pos2.z = a2;

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

                        startSound(&fly->bact_internal->soundcarrier, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = 10.0 * v69 + fly->bact_internal->position.x;
                        arg180.field_C = 10.0 * v70 + fly->bact_internal->position.z;

                        fly->ywo->ypaworld_func180(&arg180);
                    }
                }

                fly->bact_internal->fly_dir.y -= 0.2;
            }

            sb_0x4b255c(fly, a2, &a3, 1);
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

    float v34;
    float v36;
    float v47;

    if ( arg->flag & 1 )
    {
        v36 = 0.0;
        v34 = 0.0;
        v47 = 0.0;
    }
    else
    {
        v34 = bact->rotation.m20;
        v36 = bact->rotation.m22;
        v47 = bact->thraction;
    }

    float v35, v45;

    if ( arg->flag & 1 )
    {
        v35 = 0.0;
        v45 = 0.0;
    }
    else
    {
        v35 = -1.0;
        v45 = fly->field_c;
    }

    float v12 = bact->fly_dir_length * bact->airconst;

    float v39 = 1.0 * v46 + 0.0 * v47 + -bact->fly_dir.y * v12 + v35 * v45;
    float v38 = 0.0 * v46 + v34 * v47 + -bact->fly_dir.x * v12 + 0.0 * v45;
    float v42 = 0.0 * v46 + v36 * v47 + -bact->fly_dir.z * v12 + 0.0 * v45;

    float v33 = sqrt( POW2(v39) + POW2(v38) + POW2(v42) );

    if ( v33 > 0.0 )
    {
        float v20 = v33 / bact->mass * arg->field_0;

        float v41 = bact->fly_dir.y * bact->fly_dir_length + v39 * v20 / v33;
        float v40 = bact->fly_dir.x * bact->fly_dir_length + v38 * v20 / v33;
        float v43 = bact->fly_dir.z * bact->fly_dir_length + v42 * v20 / v33;

        bact->fly_dir_length = sqrt( POW2(v41) + POW2(v40) + POW2(v43) );

        if ( bact->fly_dir_length > 0.0 )
        {
            bact->fly_dir.x = v40 / bact->fly_dir_length;
            bact->fly_dir.y = v41 / bact->fly_dir_length;
            bact->fly_dir.z = v43 / bact->fly_dir_length;
        }
    }

    if ( bact->fly_dir_length > 1.5 )
    {
        bact->position.x += bact->fly_dir.x * bact->fly_dir_length * arg->field_0 * 6.0;
        bact->position.y += bact->fly_dir.y * bact->fly_dir_length * arg->field_0 * 6.0;
        bact->position.z += bact->fly_dir.z * bact->fly_dir_length * arg->field_0 * 6.0;
    }

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
