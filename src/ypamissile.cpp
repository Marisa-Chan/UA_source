#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypamissile.h"

#include <math.h>


stored_functions *classvtbl_get_ypamissile();
class_return * class_set_ypamissile(int, ...);

stored_functions ypamissile_class_vtbl(class_set_ypamissile);


class_stored ypamissile_class_off (NULL, NULL, "MC2classes:ypamissile.class", classvtbl_get_ypamissile);


stored_functions *classvtbl_get_ypamissile()
{
    return &ypamissile_class_vtbl;
}

CLASSFUNC ypamissile_funcs[1024];



int ypamissile_func0__sub0(NC_STACK_ypamissile *obj, __NC_STACK_ypamissile *miss, stack_vals *stak)
{
    miss->ejaculator_bact = 0;
    miss->life_time = 5000;
    miss->delay_time = 0;
    miss->field_c = 1;

    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80001001:
                miss->ywo = (NC_STACK_ypaworld *)stk->value;
                miss->yw = &miss->ywo->stack__ypaworld;
                break;

            case 0x80001004:
                if ( stk->value )
                    miss->field_2D |= 1;
                else
                    miss->field_2D &= 0xFFFFFFFE;

                break;

            case 0x80002000:
                miss->ejaculator_bact = (__NC_STACK_ypabact *)stk->value;
                break;

            case 0x80002002:
                miss->field_c = stk->value;
                break;

            case 0x80002004:
                miss->life_time = stk->value;
                break;

            case 0x80002005:
                miss->delay_time = stk->value;
                break;

            case 0x80002006:
                miss->drive_time = stk->value;
                break;

            case 0x80002007:
                if ( stk->value )
                    miss->field_2D |= 4;
                else
                    miss->field_2D &= 0xFFFFFFFB;
                break;

            }
            stk++;
        }
    }

    return 1;
}


NC_STACK_ypamissile * ypamissile_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypamissile *obj = (NC_STACK_ypamissile *)call_parent(zis, clss, 0, stak);

    if (obj)
    {
        __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;

        //call_vtbl(obj, 3, 0x80001003, &bact, 0);
        __NC_STACK_ypabact *bact = &obj->stack__ypabact;

        miss->selfie = bact;

        miss->selfie_node.bact = bact;
        miss->selfie_node.bacto = obj;

        bact->field_24 = 4;

        if ( !ypamissile_func0__sub0(obj, miss, stak) )
        {
            call_method(obj, 1);
            return NULL;
        }
    }

    return obj;
}

size_t ypamissile_func1(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypamissile_func2__sub0(NC_STACK_ypamissile *obj, __NC_STACK_ypamissile *miss, stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80001004:
                if ( stk->value )
                    miss->field_2D |= 1;
                else
                    miss->field_2D &= 0xFFFFFFFE;

                break;

            case 0x80002000:
                miss->ejaculator_bact = (__NC_STACK_ypabact *)stk->value;
                break;

            case 0x80002002:
                miss->field_c = stk->value;
                break;

            case 0x80002004:
                miss->life_time = stk->value;
                break;

            case 0x80002005:
                miss->delay_time = stk->value;
                break;

            case 0x80002006:
                miss->drive_time = stk->value;
                break;

            case 0x80002007:
                if ( stk->value )
                    miss->field_2D |= 4;
                else
                    miss->field_2D &= 0xFFFFFFFB;
                break;

            case 0x80002008:
                miss->energy_heli = stk->value * 0.001;
                break;

            case 0x80002009:
                miss->energy_tank = stk->value * 0.001;
                break;

            case 0x8000200A:
                miss->energy_flyer = stk->value * 0.001;
                break;

            case 0x8000200B:
                miss->energy_robo = stk->value * 0.001;
                break;

            case 0x8000200C:
                miss->radius_heli = stk->value;
                break;

            case 0x8000200D:
                miss->radius_tank = stk->value;
                break;

            case 0x8000200E:
                miss->radius_flyer = stk->value;
                break;

            case 0x8000200F:
                miss->radius_robo = stk->value;
                break;

            case 0x80002010:
                miss->posy = stk->value;
                break;

            }
            stk++;
        }
    }
}

void ypamissile_func2(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);

    ypamissile_func2__sub0(obj, &obj->stack__ypamissile, stak);
}

void ypamissile_func3__sub0(NC_STACK_ypamissile *obj, __NC_STACK_ypamissile *miss, stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80002000:
                *(__NC_STACK_ypabact **)stk->value = miss->ejaculator_bact;
                break;

            case 0x80002002:
                *(int *)stk->value = miss->field_c;
                break;

            case 0x80002003:
                *(bact_node **)stk->value = &miss->selfie_node;
                break;

            case 0x80002004:
                *(int *)stk->value = miss->life_time;
                break;

            case 0x80002005:
                *(int *)stk->value = miss->delay_time;
                break;

            case 0x80002006:
                *(int *)stk->value = miss->drive_time;
                break;

            case 0x80002007:
                *(int *)stk->value = miss->field_2D & 4;
                break;

            case 0x80002008:
                *(int *)stk->value = miss->energy_heli * 1000.0;
                break;

            case 0x80002009:
                *(int *)stk->value = miss->energy_tank * 1000.0;
                break;

            case 0x8000200A:
                *(int *)stk->value = miss->energy_flyer * 1000.0;
                break;

            case 0x8000200B:
                *(int *)stk->value = miss->energy_robo * 1000.0;
                break;

            case 0x8000200C:
                *(int *)stk->value = miss->radius_heli;
                break;

            case 0x8000200D:
                *(int *)stk->value = miss->radius_tank;
                break;

            case 0x8000200E:
                *(int *)stk->value = miss->radius_flyer;
                break;

            case 0x8000200F:
                *(int *)stk->value = miss->radius_robo;
                break;

            case 0x80002010:
                *(int *)stk->value = miss->posy;
                break;

            }
            stk++;
        }
    }
}

void ypamissile_func3(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);

    ypamissile_func3__sub0(obj, &obj->stack__ypamissile, stak);
}

void ypamissile_func68(NC_STACK_ypamissile *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( miss->selfie->field_3D5 == 2 )
    {
        int a4;

        call_vtbl(obj, 3, 0x8000100B, &a4, 0);

        a4 -= arg->field_4;

        call_vtbl(obj, 2, 0x8000100B, a4, 0);
    }

    if ( bact->field_3DE )
    {
        if ( bact->field_3DE == 2 )
        {
            bact->field_639.sx = bact->field_3e8->field_621.sx - bact->field_621.sx;
            bact->field_639.sy = bact->field_3e8->field_621.sy - bact->field_621.sy;
            bact->field_639.sz = bact->field_3e8->field_621.sz - bact->field_621.sz;
        }
        else
        {
            bact->field_639.sx = bact->field_3ec.sx - bact->field_621.sx;
            bact->field_639.sy = bact->field_3ec.sy - bact->field_621.sy;
            bact->field_639.sz = bact->field_3ec.sz - bact->field_621.sz;
        }
    }

    call_method(obj, 69, arg);
}

void ypamissile_func69(NC_STACK_ypamissile *obj, class_stru *zis, ypabact_arg65 *arg)
{
    call_method(obj, 70, arg);
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

    int a5;
    call_vtbl(miss->ejaculator_bact->self, 3, 0x80001005, &a5, 0);

    if ( !a5 )
        call_vtbl(bact->self, 3, 0x80001004, &a5, 0);

    yw_130arg arg130;
    arg130.pos_x = bact->field_62D.sx;
    arg130.pos_z = bact->field_62D.sz;
    call_method(miss->ywo, 130, &arg130);

    cellArea *v68[3];

    v68[0] = arg130.pcell;

    arg130.pos_x = bact->field_621.sx;
    arg130.pos_z = bact->field_621.sz;
    call_method(miss->ywo, 130, &arg130);

    v68[2] = arg130.pcell;

    if ( arg130.pcell == v68[0] )
    {
        v68[1] = v68[0];
    }
    else
    {
        arg130.pos_x = (bact->field_621.sx - bact->field_62D.sx) * 0.5 + bact->field_62D.sx;
        arg130.pos_z = (bact->field_621.sz - bact->field_62D.sz) * 0.5 + bact->field_62D.sz;
        call_method(miss->ywo, 130, &arg130);

        v68[1] = arg130.pcell;
    }

    for (int i = 0; i < 3; i++)
    {
        if ( i == 0 || v68[i] != v68[i - 1] )
        {
            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v68[ i ]->field_3C.head;
            while (bct->next)
            {
                if ( bct != bact && bct != miss->ejaculator_bact )
                {
                    if ( bct->field_24 != 4 && bct->field_3D5 != 2 )
                    {
                        int a4;
                        call_vtbl(bct->self, 3, 0x80002006, &a4, 0);

                        if ( bct->field_24 != 9 || bct->shield > 100 || !a4 )
                        {
                            if ( a5 || bct->owner != miss->ejaculator_bact->owner )
                            {
                                int v99;
                                int v99_1;

                                call_vtbl(miss->ejaculator_bact->self, 3, 0x80002006, &v99, 0);
                                call_vtbl(bct->self, 3, 0x80002006, &v99_1, 0);

                                if ( miss->ejaculator_bact->field_24 != 9
                                        || bct->owner != bact->owner
                                        || !v99
                                        || (bct->field_24 != 3 && (bct->field_24 != 9 || !v99_1) ) )
                                {

                                    if ( miss->field_c != 1 || bct->field_621.sy >= miss->posy )
                                    {
                                        rbcolls *v82;
                                        call_vtbl(bct->self, 3, 0x8000100E, &v82, 0);

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
                                                rbcolls *v8 = &v82[j];
                                                radius = v8->roboColls->robo_coll_radius;

                                                ttmp.sx = bct->field_651.m00 * v8->roboColls->robo_coll_x + bct->field_651.m10 * v8->roboColls->robo_coll_y + bct->field_651.m20 * v8->roboColls->robo_coll_z + bct->field_621.sx;
                                                ttmp.sy = bct->field_651.m01 * v8->roboColls->robo_coll_x + bct->field_651.m11 * v8->roboColls->robo_coll_y + bct->field_651.m21 * v8->roboColls->robo_coll_z + bct->field_621.sy;
                                                ttmp.sz = bct->field_651.m02 * v8->roboColls->robo_coll_x + bct->field_651.m12 * v8->roboColls->robo_coll_y + bct->field_651.m22 * v8->roboColls->robo_coll_z + bct->field_621.sz;
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
                                                    float v22 = 1.0 / v95;

                                                    float v65 = v69 * v22;
                                                    float v66 = v70 * v22;
                                                    float v67 = v71 * v22;

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
                                                            NC_STACK_ypabact *a1;

                                                            call_vtbl(miss->ywo, 3, 0x80002010, &a1, 0);

                                                            __NC_STACK_ypabact *v85;
                                                            call_vtbl(a1, 3, 0x80001003, &v85, 0);

                                                            v90 = 1;

                                                            v91 += radius;
                                                            v81++;

                                                            bct->field_3D6 &= 0xFFFFFDFF;

                                                            v78.sx += bct->field_621.sx;
                                                            v78.sy += bct->field_621.sy;
                                                            v78.sz += bct->field_621.sz;

                                                            int v83;

                                                            call_vtbl(bct->self, 3, 0x80001005, &v83, 0);

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
                                                                    call_method(bct->self, 84, &arg84);
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
        float v48 = 1.0 / (float)v81;

        bact->field_621.sx = v78.sx * v48;
        bact->field_621.sy = v78.sy * v48;
        bact->field_621.sz = v78.sz * v48;

        v91 *= v48;

        if ( v91 >= 50.0 )
        {
            float v54 = bact->field_621.sx - bact->field_62D.sx;
            float v55 = bact->field_621.sy - bact->field_62D.sy;
            float v57 = bact->field_621.sz - bact->field_62D.sz;


            float v100 = sqrt( POW2(v54) + POW2(v55) + POW2(v57) );

            if ( v100 < 1.0 )
                v100 = 1.0;

            float v59 = 1.0 / v100;
            bact->field_621.sx -= (bact->field_621.sx - bact->field_62D.sx) * v91 * v59;
            bact->field_621.sy -= (bact->field_621.sy - bact->field_62D.sy) * v91 * v59;
            bact->field_621.sz -= (bact->field_621.sz - bact->field_62D.sz) * v91 * v59;
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
        float v9 = 1.0 / v13;
        arg74->vec.sx *= v9;
        arg74->vec.sy *= v9;
        arg74->vec.sz *= v9;
    }
}

void ypamissile_func70(NC_STACK_ypamissile *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    call_method(miss->ywo, 145, bact);

    float v40 = sqrt( POW2(bact->field_639.sx) + POW2(bact->field_639.sy) + POW2(bact->field_639.sz) );

    if ( v40 > 0.1 )
    {
        if ( bact->field_3DE != 4 )
        {
            float v8 = 1.0 / v40;

            bact->field_645.sx = bact->field_639.sx * v8;
            bact->field_645.sy = bact->field_639.sy * v8;
            bact->field_645.sz = bact->field_639.sz * v8;
        }
    }

    bact->field_919 = 0;

    bact->field_601 = bact->force;

    float v38 = arg->field_4 * 0.001;

    if ( bact->field_3D5 == 1 )
    {
        miss->delay_time -= arg->field_4;

        if ( miss->field_2D & 2  &&  miss->delay_time <= 0 )
        {
            call_method(obj, 129, 0);

            miss->selfie->field_3D5 = 2;

            bact_arg119 arg78;
            arg78.field_4 = 2048;
            arg78.field_8 = 0;
            arg78.field_0 = 0;

            call_method(obj, 78,  &arg78);

            if ( !(miss->field_2D & 4) || !miss->selfie->p_cell_area->field_3A )
            {
                if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                {
                    yw_arg129 v25;

                    v25.pos.sx = bact->field_605.sx * 5.0 + bact->field_621.sx;
                    v25.pos.sz = bact->field_605.sz * 5.0 + bact->field_621.sz;
                    v25.field_10 = bact->energy;
                    v25.unit = miss->ejaculator_bact;

                    call_method(obj, 120, &v25);
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
                break;

            case 2:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                break;

            case 3:
                arg74.field_0 = v38;
                arg74.flag = 0;

                ypamissile_func70__sub1(miss, &arg74);
                break;

            case 4:
                arg74.field_0 = v38;
                arg74.vec = bact->field_605;
                arg74.flag = 0;
                break;

            default:
                break;
            }

            call_method(obj, 74, &arg74);

            if ( ypamissile_func70__sub0(miss) )
            {
                bact_arg119 arg78;
                call_method(obj, 129, 0);

                arg78.field_0 = 2;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                call_method(obj, 78, &arg78);

                call_method(obj, 128, 0);
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

                call_method(miss->ywo, 136, &arg136);

                if ( arg136.field_20 )
                {
                    miss_arg130 arg131;
                    arg131.pos.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
                    arg131.pos.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
                    arg131.pos.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;

                    call_method(obj, 131, &arg131);


                    bact->field_621.sx = arg136.field_2C;
                    bact->field_621.sy = arg136.field_30;
                    bact->field_621.sz = arg136.field_34;

                    call_method(obj, 128, 0);

                    miss->field_c = 6;
                    miss->field_2D |= 2;

                    if ( !miss->delay_time )
                    {
                        call_method(obj, 129, 0);

                        miss->selfie->field_3D5 = 2;

                        bact_arg119 arg78;
                        arg78.field_4 = 2048;
                        arg78.field_8 = 0;
                        arg78.field_0 = 0;

                        call_method(obj, 78, &arg78);

                        if ( !(miss->field_2D & 4) || !miss->selfie->p_cell_area->field_3A )
                        {
                            if ( miss->yw->field_1b80->owner == miss->selfie->owner || !miss->yw->field_757E )
                            {
                                yw_arg129 v25;

                                v25.pos.sx = bact->field_605.sx * 5.0 + bact->field_621.sx;
                                v25.pos.sz = bact->field_605.sz * 5.0 + bact->field_621.sz;
                                v25.field_10 = bact->energy;
                                v25.unit = miss->ejaculator_bact;

                                call_method(obj, 120, &v25);
                            }
                        }
                    }

                    int a4;
                    call_vtbl(miss->ejaculator_bact->self, 3, 0x80001005, &a4, 0);

                    if ( a4 )
                    {
                        if ( miss->ejaculator_bact->field_32 == miss->ejaculator_bact->parent_bacto )
                        {
                            if ( miss->ejaculator_bact->field_32 )
                            {
                                bact_arg67 arg67;
                                arg67.field_0 = 1;
                                arg67.targ = bact->field_621;
                                arg67.field_4 = 0;

                                call_method(miss->ejaculator_bact->self, 67, &arg67);
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

                        call_method(obj, 130, &arg130);
                    }
                    else
                    {
                        call_method(obj, 129, 0);

                        bact_arg119 arg78;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;
                        arg78.field_0 = 2;

                        call_method(obj, 78, &arg78);

                        call_method(obj, 128, 0);
                    }
                }
            }
        }
    }
}

void ypamissile_func71(NC_STACK_ypamissile *obj, class_stru *zis, stack_vals *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    bact->field_62D = bact->field_621;

    if (bact->field_3D5 == 1)
        call_method(obj, 68, arg);
    else
        call_method(obj, 128, 0);
}

void ypamissile_func74(NC_STACK_ypamissile *obj, class_stru *zis, bact_arg74 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
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
        float v19 = 1.0 / v33;
        float v20 = v33 / bact->mass * arg->field_0;

        xyz v36;
        v36.sx = bact->field_605.sx * bact->field_611 + v29 * v19 * v20;
        v36.sy = bact->field_605.sy * bact->field_611 + v30 * v19 * v20;
        v36.sz = bact->field_605.sz * bact->field_611 + v31 * v19 * v20;

        float v32 = sqrt(POW2(v36.sx) + POW2(v36.sy) + POW2(v36.sz));

        if ( v32 > 0.0 )
        {
            float v25 = 1.0 / v32;
            v36.sx *= v25;
            v36.sy *= v25;
            v36.sz *= v25;
        }

        bact->field_605 = v36;

        bact->field_611 = v32;
    }

    bact->field_621.sx += bact->field_605.sx * bact->field_611 * arg->field_0 * 6.0;
    bact->field_621.sy += bact->field_605.sy * bact->field_611 * arg->field_0 * 6.0;
    bact->field_621.sz += bact->field_605.sz * bact->field_611 * arg->field_0 * 6.0;

    call_method(obj, 115, 0);
}

void ypamissile_func78(NC_STACK_ypamissile *obj, class_stru *zis, bact_arg119 *arg)
{
    call_method(obj, 119, arg);
}

void ypamissile_func96(NC_STACK_ypamissile *obj, class_stru *zis, void *)
{
    call_parent(zis, obj, 96, 0);

    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;

    miss->field_2D  = 0;
    miss->delay_time = 0;

    call_vtbl(obj, 2, 0x8000100B, 3000, 0);
}

size_t ypamissile_func119(NC_STACK_ypamissile *obj, class_stru *zis, bact_arg119 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
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
        call_vtbl(obj, 2, 0x8000100C, bact->vp_dead.base, 0);
        call_vtbl(obj, 2, 0x8000100F, bact->vp_dead.trigo, 0);

        sub_423F74(&bact->field_5A, 2);

        uint8_t v8 = 1;
        call_method(obj, 113, &v8);

        bact->field_611 = 0;
    }

    if ( arg->field_0 == 1 )
    {
        call_vtbl(obj, 2, 0x8000100C, bact->vp_normal.base, 0);
        call_vtbl(obj, 2, 0x8000100F, bact->vp_normal.trigo, 0);

        sub_423F74(&bact->field_5A, 0);
    }

    if ( arg->field_8 == 2048 )
    {
        call_vtbl(obj, 2, 0x8000100C, bact->vp_normal.base, 0);
        call_vtbl(obj, 2, 0x8000100F, bact->vp_normal.trigo, 0);

        sub_423F74(&bact->field_5A, 0);
    }

    if ( arg->field_4 == 2048 )
    {
        bact->field_3D5 = 2;

        call_vtbl(obj, 2, 0x8000100C, bact->vp_megadeth.base, 0);
        call_vtbl(obj, 2, 0x8000100F, bact->vp_megadeth.trigo, 0);

        sub_423F74(&bact->field_5A, 2);

        uint8_t v9 = 2;
        call_method(obj, 113, &v9);

        bact->field_611 = 0;
    }

    return 1;
}

void ypamissile_func128(NC_STACK_ypamissile *obj, class_stru *zis, void *)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;

    int a4;
    call_vtbl(obj, 3, 0x80001004, &a4, 0);

    if ( a4 )
    {
        call_vtbl(obj, 2, 0x80001004, 0, 0);
        call_vtbl(obj, 2, 0x80001005, 0, 0);

        if ( miss->ejaculator_bact->field_3D5 != 2 || (size_t)miss->ejaculator_bact->parent_bacto <= 3 )
        {
            call_vtbl(miss->ejaculator_bact->self, 2, 0x80001004, 1, 0);
            call_vtbl(miss->ejaculator_bact->self, 2, 0x80001005, 1, 0);
        }
        else
        {
            call_vtbl(miss->ejaculator_bact->parent_bacto, 2, 0x80001004, 1, 0);
            call_vtbl(miss->ejaculator_bact->parent_bacto, 2, 0x80001005, 1, 0);
        }

    }
}

void ypamissile_func129(NC_STACK_ypamissile *obj, class_stru *zis, void *)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
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

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)bact->p_cell_area->field_3C.head;

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
                call_method(bct->self, 83, &arg83);
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

        call_method(miss->ywo, 181, &arg181);
    }
}

void ypamissile_func130(NC_STACK_ypamissile *obj, class_stru *zis, miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    if ( bact->field_605.sx != 0.0 || bact->field_605.sy != 0.0 || bact->field_605.sz != 0.0 )
    {
        float v46 = bact->field_651.m21 * bact->field_605.sz - bact->field_605.sy * bact->field_651.m22;
        float v47 = bact->field_651.m22 * bact->field_605.sx - bact->field_605.sz * bact->field_651.m20;
        float v45 = bact->field_651.m20 * bact->field_605.sy - bact->field_651.m21 * bact->field_605.sx;

        float v37 = sqrt(POW2(v46) + POW2(v47) + POW2(v45));

        if ( v37 > 0.0 )
        {
            float v8 = 1.0 / v37;

            float v51 = bact->field_651.m20 * bact->field_605.sx + bact->field_651.m21 * bact->field_605.sy + bact->field_651.m22 * bact->field_605.sz;

            v46 *= v8;
            v47 *= v8;
            v45 *= v8;

            if ( v51 > 1.0 )
                v51 = 1.0;

            if ( v51 < -1.0 )
                v51 = -1.0;

            float v52 = acos(v51);

            if ( miss->field_c == 1 )
            {
                if ( arg )
                {
                    float mxrot = -bact->maxrot * arg->period;

                    if ( v52 < -mxrot )
                        v52 = -mxrot;

                    if ( v52 > mxrot )
                        v52 = mxrot;
                }
            }

            if ( v52 > 0.01 || v52 < -0.01 )
            {
                float v9 = sin(-v52);
                float v27 = cos(v52);

                mat3x3 mat2;

                mat2.m00 = (1.0 - v27) * v46 * v46 + v27;
                mat2.m01 = (1.0 - v27) * v46 * v47 - v9 * v45;
                mat2.m02 = (1.0 - v27) * v45 * v46 + v9 * v47;
                mat2.m10 = (1.0 - v27) * v46 * v47 + v9 * v45;
                mat2.m11 = (1.0 - v27) * v47 * v47 + v27;
                mat2.m12 = (1.0 - v27) * v47 * v45 - v9 * v46;
                mat2.m20 = (1.0 - v27) * v45 * v46 - v9 * v47;
                mat2.m21 = (1.0 - v27) * v47 * v45 + v9 * v46;
                mat2.m22 = (1.0 - v27) * v45 + v27;

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

void ypamissile_func131(NC_STACK_ypamissile *obj, class_stru *zis, miss_arg130 *arg)
{
    __NC_STACK_ypamissile *miss = &obj->stack__ypamissile;
    __NC_STACK_ypabact *bact = miss->selfie;

    float v33 = bact->field_651.m10;
    float v5 = bact->field_651.m11;
    float v36 = bact->field_651.m12;

    float v35 = arg->pos.sx;
    float v34 = arg->pos.sy;
    float v6 = arg->pos.sz;

    float v37 = v5 * v6 - v36 * v34;
    float v38 = v35 * v36 - v33 * v6;
    float v39 = v33 * v34 - v5 * v35;

    float v30 = sqrt( POW2(v37) + POW2(v38) + POW2(v39) );

    if ( v30 != 0.0 )
    {
        float v11 = 1.0 / v30;

        float v43 = v33 * v35 + v5 * v34 + v36 * v6;

        v37 *= v11;
        v38 *= v11;
        v39 *= v11;

        if ( v43 > 1.0 )
            v43 = 1.0;

        if ( v43 < -1.0 )
            v43 = -1.0;

        float v12 = acos(v43);

        if ( fabs(v12) > BACT_MIN_ANGLE )
        {
            float v13 = sin(-v12);
            float v23 = cos(v12);

            mat3x3 mat2;

            mat2.m00 = (1.0 - v23) * v37 * v37 + v23;
            mat2.m01 = (1.0 - v23) * v37 * v38 - v13 * v39;
            mat2.m02 = (1.0 - v23) * v39 * v37 + v13 * v38;
            mat2.m10 = (1.0 - v23) * v37 * v38 + v13 * v39;
            mat2.m11 = (1.0 - v23) * v38 * v38 + v23;
            mat2.m12 = (1.0 - v23) * v38 * v39 - v13 * v37;
            mat2.m20 = (1.0 - v23) * v39 * v37 - v13 * v38;
            mat2.m21 = (1.0 - v23) * v38 * v39 + v13 * v37;
            mat2.m22 = (1.0 - v23) * v39 * v39 + v23;

            mat3x3 v21;

            mat_mult(&bact->field_651, &mat2, &v21);

            bact->field_651 = v21;
        }
    }
}


class_return ypamissile_class_descr;

class_return * class_set_ypamissile(int , ...)
{

    memset(ypamissile_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypamissile_class_descr.parent = "ypabact.class";

    ypamissile_funcs[0] = (CLASSFUNC)ypamissile_func0;
    ypamissile_funcs[1] = (CLASSFUNC)ypamissile_func1;
    ypamissile_funcs[2] = (CLASSFUNC)ypamissile_func2;
    ypamissile_funcs[3] = (CLASSFUNC)ypamissile_func3;
    ypamissile_funcs[68] = (CLASSFUNC)ypamissile_func68;
    ypamissile_funcs[69] = (CLASSFUNC)ypamissile_func69;
    ypamissile_funcs[70] = (CLASSFUNC)ypamissile_func70;
    ypamissile_funcs[71] = (CLASSFUNC)ypamissile_func71;
    ypamissile_funcs[74] = (CLASSFUNC)ypamissile_func74;
    ypamissile_funcs[78] = (CLASSFUNC)ypamissile_func78;
    ypamissile_funcs[96] = (CLASSFUNC)ypamissile_func96;
    ypamissile_funcs[119] = (CLASSFUNC)ypamissile_func119;
    ypamissile_funcs[128] = (CLASSFUNC)ypamissile_func128;
    ypamissile_funcs[129] = (CLASSFUNC)ypamissile_func129;
    ypamissile_funcs[130] = (CLASSFUNC)ypamissile_func130;
    ypamissile_funcs[131] = (CLASSFUNC)ypamissile_func131;

    ypamissile_class_descr.vtbl = ypamissile_funcs;
    ////ypamissile_class_descr.varSize = sizeof(__NC_STACK_ypamissile);
    ypamissile_class_descr.varSize = sizeof(NC_STACK_ypamissile) - offsetof(NC_STACK_ypamissile, stack__ypamissile); //// HACK
    ypamissile_class_descr.field_A = 0;
    return &ypamissile_class_descr;
}
