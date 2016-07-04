#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#include "classes.h"
#include "yw.h"
#include "ypacar.h"

#include "log.h"



stored_functions *classvtbl_get_ypacar();
class_return * class_set_ypacar(int, ...);

stored_functions ypacar_class_vtbl(class_set_ypacar);


class_stored ypacar_class_off (NULL, NULL, "MC2classes:ypacar.class", classvtbl_get_ypacar);


stored_functions *classvtbl_get_ypacar()
{
    return &ypacar_class_vtbl;
}

CLASSFUNC ypacar_funcs[1024];


int ypacar_func0__sub0(NC_STACK_ypacar *obj, __NC_STACK_ypacar *car, stack_vals *stak)
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

            case 0x80001001:
                car->ywo = (NC_STACK_ypaworld *)stk->value;
                car->yw = &car->ywo->stack__ypaworld;
                break;

            case 0x80003000:
                if (stk->value)
                    car->field_c = 1;
                else
                    car->field_c = 0;
                break;

            case 0x80003001:
                car->field_10 = stk->value;
                break;

            }
            stk++;
        }
    }

    return 1;
}

NC_STACK_ypacar * ypacar_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypacar *obj = (NC_STACK_ypacar *)call_parent(zis, clss, 0, stak);

    if (obj)
    {
        __NC_STACK_ypacar *car = &obj->stack__ypacar;
        __NC_STACK_ypabact *bact = &obj->stack__ypabact;

        car->bact_internal = bact;

        bact->field_24 = 8;

        if ( !ypacar_func0__sub0(obj, car, stak) )
        {
            call_method(obj, 1);
            return NULL;
        }
    }

    return obj;
}

size_t ypacar_func1(NC_STACK_ypacar *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void ypacar_func2__sub0(NC_STACK_ypacar *obj, __NC_STACK_ypacar *car, stack_vals *stak)
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

            case 0x80003000:
                if (stk->value)
                    car->field_c = 1;
                else
                    car->field_c = 0;
                break;

            case 0x80003001:
                car->field_10 = stk->value;
                break;

            }
            stk++;
        }
    }
}

void ypacar_func2(NC_STACK_ypacar *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
    ypacar_func2__sub0(obj, &obj->stack__ypacar, stak);
}

void ypacar_func3__sub0(NC_STACK_ypacar *obj, __NC_STACK_ypacar *car, stack_vals *stak)
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

            case 0x80003000:
                if (car->field_c)
                    *(int *)stk->value = 1;
                else
                    *(int *)stk->value = 0;
                break;

            case 0x80003001:
                *(int *)stk->value = car->field_10;
                break;

            }
            stk++;
        }
    }
}

void ypacar_func3(NC_STACK_ypacar *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
    ypacar_func3__sub0(obj, &obj->stack__ypacar, stak);
}

void ypacar_func71__sub1(__NC_STACK_ypabact *bact, float angle)
{
    float cs = cos(angle);
    float sn = sin(angle);

    mat3x3 mat;

    mat.m00 = cs;
    mat.m01 = 0;
    mat.m02 = sn;
    mat.m10 = 0;
    mat.m11 = 1.0;
    mat.m12 = 0;
    mat.m20 = -sn;
    mat.m21 = 0;
    mat.m22 = cs;

    mat3x3 dst;
    mat_mult(&mat, &bact->field_651, &dst);

    bact->field_651 = dst;
}

void ypacar_func71__sub0(NC_STACK_ypacar *caro)
{
    __NC_STACK_ypacar *car = &caro->stack__ypacar;
    __NC_STACK_ypabact *bact = &caro->stack__ypabact;

    float v53;

    if ( car->field_10 >= 10000 )
        v53 = log(10000.0 / car->field_10) * -428.5714285714286;
    else
        v53 = 1.0;

    int v3 = (v53 / 300.0);

    for (int i = 0; i < v3; i++)
    {
        float v4 = sqrt( POW2(i) + POW2(v3) );

        for (int j = 0; j <= v4; j++)
        {
            yw_arg129 arg120;
            arg120.field_10 = exp( sqrt( POW2(i) + POW2(j) ) * (1200.0 * -2.8) / 1200.0 ) * car->field_10;
            arg120.pos.sx = i * 300.0 + bact->field_621.sx;
            arg120.pos.sy = 0;
            arg120.pos.sz = j * 300.0 + bact->field_621.sz;
            arg120.unit = bact;

            if ( arg120.pos.sx > 1200.0  &&  arg120.pos.sx < bact->field_18 - 1200.0  &&  arg120.pos.sz < -1200.0  &&  arg120.pos.sz > bact->field_1c + 1200.0 )
                call_method(bact->self, 120, &arg120);

            if ( i )
            {
                arg120.pos.sx = bact->field_621.sx - i * 300.0;
                arg120.pos.sz = bact->field_621.sz + j * 300.0;

                if ( arg120.pos.sx > 1200.0 )
                {
                    if ( arg120.pos.sx < bact->field_18 - 1200.0 && arg120.pos.sz < -1200.0 && arg120.pos.sz > bact->field_1c + 1200.0 )
                        call_method(bact->self, 120, &arg120);
                }
            }

            if ( j )
            {
                arg120.pos.sx = bact->field_621.sx + i * 300.0;
                arg120.pos.sz = bact->field_621.sz - j * 300.0;

                if ( arg120.pos.sx > 1200.0 )
                {
                    if ( arg120.pos.sx < bact->field_18 - 1200.0  &&  arg120.pos.sz < -1200.0  &&  arg120.pos.sz > bact->field_1c + 1200.0 )
                        call_method(bact->self, 120, &arg120);
                }
            }

            if ( i )
            {
                if ( j )
                {
                    arg120.pos.sx = bact->field_621.sx - i * 300.0;
                    arg120.pos.sz = bact->field_621.sz - j * 300.0;

                    if ( arg120.pos.sx > 1200.0 )
                    {
                        if ( arg120.pos.sx < bact->field_18 - 1200.0  &&  arg120.pos.sz < -1200.0  &&  arg120.pos.sz > bact->field_1c + 1200.0 )
                            call_method(bact->self, 120, &arg120);
                    }
                }
            }
        }
    }

    int v13 = v53 / 1200.0;

    for (int i = 0; i < v13; i++)
    {
        int v59 = sqrt( POW2( i ) + POW2( v13 ) );

        for (int j = 0; j <= v59; j++ )
        {
            int v16 = i + bact->field_c;
            int v18 = j + bact->field_10;

            if ( v16 > 0 && v16 < bact->field_20 - 1  &&  v18 > 0 && v18 < bact->field_22 - 1 )
            {
                __NC_STACK_ypabact *v19 = (__NC_STACK_ypabact *)bact->p_cell_area[i + j * bact->field_20].units_list.head;

                while ( v19->next )
                {
                    int v63 = 0;

                    if ( v19 != bact )
                    {
                        if ( v19->field_24 != 4 )
                        {
                            xyz tmp;
                            tmp.sx = bact->field_621.sx - v19->field_621.sx;
                            tmp.sy = bact->field_621.sy - v19->field_621.sy;
                            tmp.sz = bact->field_621.sz - v19->field_621.sz;

                            int v26 = exp(sqrt( POW2(tmp.sx) + POW2(tmp.sy) + POW2(tmp.sz) ) * -2.8 / 1200.0) * car->field_10;
                            int v67 = ((1.0 - (float)v19->shield * 0.01) * (float)v26);

                            v19->energy -= v67;

                            if ( car->yw->field_757E )
                            {
                                v63 = 1;

                                char v41[32];
//                      *(_DWORD *)v41 = 1010;
//                      *(_DWORD *)&v41[4] = car->yw->field_1614;
//                      v41[12] = v19->owner;
//                      *(_DWORD *)&v41[16] = v19->ypabact__id;
//                      *(_DWORD *)&v41[20] = -v67;

                                yw_arg181 arg181;
                                arg181.field_10 = 0;
                                arg181.field_14 = 2;
                                arg181.field_C = 1;
                                arg181.val_size = 32;
                                arg181.field_8 = car->yw->GameShell->callSIGN;
                                arg181.field_18 = 1;
                                arg181.value = &v41;

                                call_method(car->ywo, 181, &arg181);
                            }
                        }

                        if ( v19->energy <= 0 || v19->field_24 == 4 )
                        {
                            bact_arg119 arg78;
                            arg78.field_4 = 0;
                            arg78.field_8 = 0;
                            arg78.field_0 = 2;

                            if ( v19->field_24 == 4 )
                                call_method(v19->self, 78, &arg78);
                            else
                                call_method(v19->self, 119, &arg78);

                            if ( v19->field_24 == 3 )
                            {
                                if ( !v63 )
                                {
                                    yw_arg177 arg177;
                                    arg177.bact = v19;
                                    arg177.field_4 = bact->owner;

                                    call_method(car->ywo, 177, &arg177);
                                }
                            }
                        }
                    }

                    v19 = (__NC_STACK_ypabact *)v19->next;
                }

            }
        }
    }

    bact->field_651.m21 = 1.9;

    float tmpsq = sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m21) + POW2(bact->field_651.m22) );

    NDIV_CARRY(tmpsq);

    bact->field_651.m20 /= tmpsq;
    bact->field_651.m21 /= tmpsq;
    bact->field_651.m22 /= tmpsq;

    if ( fabs(bact->field_651.m22) <= 0.1 )
    {
        float m20 = bact->field_651.m20;

        NDIV_CARRY(m20);

        bact->field_651.m02 = sqrt( 1.0 / (POW2(bact->field_651.m22) / (POW2(m20)) + 1.0) );
        bact->field_651.m00 = -bact->field_651.m22 * bact->field_651.m02 / m20;
    }
    else
    {
        float m22 = bact->field_651.m22;

        NDIV_CARRY(m22);

        bact->field_651.m00 = sqrt( 1.0 / (POW2(bact->field_651.m20) / (POW2(m22)) + 1.0) );
        bact->field_651.m02 = -bact->field_651.m20 * bact->field_651.m00 / m22;
    }

    bact->field_651.m01 = 0;

    if ( bact->field_651.m02 * bact->field_651.m20 - bact->field_651.m00 * bact->field_651.m22 > 0.0 )
    {
        bact->field_651.m00 = -bact->field_651.m00;
        bact->field_651.m02 = -bact->field_651.m02;
    }

    bact->field_651.m10 = bact->field_651.m21 * bact->field_651.m02 - bact->field_651.m22 * bact->field_651.m01;
    bact->field_651.m11 = bact->field_651.m22 * bact->field_651.m00 - bact->field_651.m20 * bact->field_651.m02;
    bact->field_651.m12 = bact->field_651.m20 * bact->field_651.m01 - bact->field_651.m21 * bact->field_651.m00;

    bact->field_605.sx = -bact->field_651.m20;
    bact->field_605.sy = -bact->field_651.m21;
    bact->field_605.sz = -bact->field_651.m22;

    bact->field_611 = 200.0;
    bact->field_3D6 &= 0xFFFFFDFF;
    bact->field_24 = 6;

    bact_arg119 arg78;
    arg78.field_0 = 2;
    arg78.field_4 = 0;
    arg78.field_8 = 0;

    call_method(bact->self, 78, &arg78);

    bact->field_24 = 8;
    bact->energy = -10;
}

void ypacar_func71(NC_STACK_ypacar *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypacar *car = (__NC_STACK_ypacar *)&obj->stack__ypacar;
    __NC_STACK_ypabact *bact = (__NC_STACK_ypabact *)&obj->stack__ypabact;

    bact->airconst = bact->airconst2;

    int a4 = 0;
    call_vtbl(obj, 3, 0x80001007, &a4, 0);

    bact->field_62D = bact->field_621;

    float v78 = arg->field_4 / 1000.0;

    tank_arg129 arg129;
    arg129.field_0 = v78;
    arg129.field_4.sx = bact->field_651.m20;
    arg129.field_4.sy = bact->field_651.m21;
    arg129.field_4.sz = bact->field_651.m22;

    if (bact->field_3D5 == 2)
        call_method(obj, 121, arg);
    else if (bact->field_3D5 == 1 || bact->field_3D5 == 3)
    {
        if ( bact->field_611 != 0.0 )
        {
            if ( ! (bact->field_3D6 & 0x100) )
            {
                bact_arg119 arg78;
                arg78.field_0 = 1;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                call_method(obj, 78, &arg78);
            }
        }
        else
        {
            if ( bact->field_3DE != 1 || sqrt( POW2(bact->field_3ec.sx - bact->field_621.sx) + POW2(bact->field_3ec.sz - bact->field_621.sz) ) <= 800.0 )
            {
                if ( !(bact->field_3D6 & 0x100) )
                {
                    bact_arg119 arg78;
                    arg78.field_0 = 3;
                    arg78.field_8 = 0;
                    arg78.field_4 = 0;

                    call_method(obj, 78, &arg78);
                }

                bact->field_3D5 = 1;
            }
            else
            {

                if ( bact->field_3D6 & 0x100 )
                {
                    bact->field_3D5 = 3;
                }
                else
                {
                    bact_arg119 arg78;
                    arg78.field_0 = 3;
                    arg78.field_8 = 0;
                    arg78.field_4 = 0;

                    call_method(obj, 78, &arg78);
                }
            }
        }

        if ( arg->inpt->sliders_vars[3] != 0.0 )
        {
            if ( bact->field_611 != 0.0 )
            {
                float v63 = fabs(bact->field_611);

                ypacar_func71__sub1(bact,  -arg->inpt->sliders_vars[3] * bact->maxrot * v78 * (sqrt(v63) * 0.2) );
            }
        }

        if ( (bact->field_611 < 0.0 && bact->field_601 > 0.0) || (bact->field_611 > 0.0 && bact->field_601 < 0.0) )
        {
            if ( fabs(bact->field_605.sy) > 0.1 )
            {
                float v74 = 1.0 - (v78 * 4.0);

                if ( v74 < 0.1 )
                    v74 = 0.1;

                bact->field_611 *= v74;
            }
        }

        float v76 = arg->inpt->sliders_vars[4];
        float v65 = fabs(arg->inpt->sliders_vars[4]);

        if ( v76 < -1.0 )
            v76 = -1.0;
        else if ( v76 > 1.0)
            v76 = 1.0;

        bact->field_601 += bact->force * (v78 * 0.75) * v76;

        float v69;
        if ( arg->inpt->but_flags & 0x80000000 )
            v69 = bact->force * v65;
        else
            v69 = bact->force;

        if ( bact->field_601 > v69 )
            bact->field_601 = v69;

        if ( bact->field_601 < -v69 )
            bact->field_601 = -v69;

        if ( fabs(v76) > 0.001 )
            bact->field_3D6 |= 0x40;

        bact->gun_angle2 += v78 * arg->inpt->sliders_vars[5];

        if ( bact->gun_angle2 < -0.3 )
            bact->gun_angle2 = -0.3;

        if ( bact->gun_angle2 > 0.8 )
            bact->gun_angle2 = 0.8;

        bact_arg79 arg79;
        arg79.field_18 = 4;
        arg79.field_20.sx = bact->field_651.m20;
        arg79.field_20.sy = bact->field_651.m21;
        arg79.field_20.sz = bact->field_651.m22;

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4.sx = bact->field_651.m20;
        arg106.field_4.sy = bact->field_651.m21 - bact->gun_angle2;
        arg106.field_4.sz = bact->field_651.m22;

        if ( call_method(obj, 106, &arg106) )
        {
            arg79.field_18 = 2;
            arg79.field_1C = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            if ( car->field_c )
            {
                ypacar_func71__sub0(obj);
            }
            else
            {
                arg79.field_2C = bact->weapon;
                arg79.field_0.sx = bact->field_651.m20 - bact->field_651.m10 * bact->gun_angle2;
                arg79.field_0.sy = bact->field_651.m21 - bact->field_651.m11 * bact->gun_angle2;
                arg79.field_0.sz = bact->field_651.m22 - bact->field_651.m12 * bact->gun_angle2;
                arg79.field_30 = bact->field_915;

                if ( bact->field_915 % 2 )
                    arg79.fire_point.sx = -bact->fire_x;
                else
                    arg79.fire_point.sx = bact->fire_x;

                arg79.fire_point.sy = bact->fire_y;
                arg79.fire_point.sz = bact->fire_z;

                arg79.field_34 = ((arg->inpt->but_flags & 2) != 0) | 2;

                call_method(obj, 79, &arg79);
            }
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->field_3D6 & 0x100 )
            {
                if ( !(arg->inpt->but_flags & 4) )
                {
                    bact_arg119 arg78;
                    arg78.field_4 = 0;
                    arg78.field_0 = 0;
                    arg78.field_8 = 256;

                    call_method(obj, 78, &arg78);
                }
            }

            if ( arg->inpt->but_flags & 4 )
            {
                if ( !(bact->field_3D6 & 0x100) )
                {
                    bact_arg119 arg78;
                    arg78.field_4 = 256;
                    arg78.field_8 = 0;
                    arg78.field_0 = 0;

                    call_method(obj, 78, &arg78);
                }

                bact_arg105 arg105;
                arg105.field_0.sx = bact->field_651.m20 - bact->field_651.m10 * bact->gun_angle2;
                arg105.field_0.sy = bact->field_651.m21 - bact->field_651.m11 * bact->gun_angle2;
                arg105.field_0.sz = bact->field_651.m22 - bact->field_651.m12 * bact->gun_angle2;
                arg105.field_C = v78;
                arg105.field_10 = bact->field_915;

                call_method(obj, 105, &arg105);
            }
        }

        if ( bact->field_3D6 & 0x200 )
        {
            bact_arg74 arg74;

            if ( arg->inpt->but_flags & 8 )
            {
                bact->field_601 = 0;

                if ( fabs(bact->field_611) >= 4.0 )
                {
                    float v75 = 1.0 - v78 * 4.0;

                    if ( v75 < 0.1 )
                        v75 = 0.1;

                    bact->field_611 *= v75 * 0.1;
                }
                else
                {
                    bact->field_3D6 &= 0xFFFFFFBF;
                    bact->field_611 = 0;
                }

                arg74.flag = 2;
            }
            else
            {
                arg74.flag = 0;
            }

            arg74.flag = 0;
            arg74.field_0 = v78;

            if ( bact->field_3D6 & 0x40 )
                call_method(obj, 74, &arg74);

            int v62 = arg->field_4;

            if ( a4 && call_method(obj, 87, &v62) )
            {
                call_method(obj, 129, &arg129);
            }
            else
            {
                ypaworld_arg136 arg136;

                arg136.pos_x = bact->field_62D.sx;
                arg136.pos_y = bact->field_62D.sy;
                arg136.pos_z = bact->field_62D.sz;
                arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
                arg136.field_18 = bact->field_621.sy - bact->field_62D.sy;
                arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;
                arg136.field_40 = 0;

                call_method(car->ywo, 136, &arg136);

                if ( arg136.field_20 )
                {

                    bact->field_621 = bact->field_62D;

                    bact->field_611 = 0;
                    bact->field_601 = 0;
                }
                else
                {
                    call_method(obj, 129, &arg129);
                }
            }
        }
        else
        {
            bact_arg86 arg86;
            arg86.field_one = 1;
            arg86.field_two = arg->field_4;

            call_method(obj, 86, &arg86);
        }
    }
}

size_t ypacar_func128(NC_STACK_ypacar *obj, class_stru *zis, tank_arg128 *arg)
{
    __NC_STACK_ypacar *car = (__NC_STACK_ypacar *)&obj->stack__ypacar;
    __NC_STACK_ypabact *bact = (__NC_STACK_ypabact *)&obj->stack__ypabact;

    arg->field_10 = 0;

    int a4 = 0;
    call_vtbl(obj, 3, 0x80001004, &a4, 0);

    float v5;

    if ( a4 )
        v5 = bact->vwr_overeof;
    else
        v5 = bact->overeof;

    ypaworld_arg136 arg136;
    arg136.pos_x = bact->field_651.m10 * v5 + bact->field_621.sx;
    arg136.pos_y = bact->field_651.m11 * v5 + bact->field_621.sy - 50.0;
    arg136.pos_z = bact->field_651.m12 * v5 + bact->field_621.sz;
    arg136.field_14 = 0;
    arg136.field_18 = bact->overeof + 110.0;
    arg136.field_1C = 0;
    arg136.field_40 = 0;

    call_method(car->ywo, 136, &arg136);

    if ( !arg136.field_20 )
        return 0;

    pol_entries2 *v8 = &arg136.field_3C->triangles[ arg136.field_38 ];

    if ( v8->field_4 < 0.6 )
    {
        arg->field_10 |= 1;
        arg->field_14.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
        arg->field_14.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
        arg->field_14.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;

        return 0;
    }

    xyz tmp;
    tmp.sx = v8->field_0;
    tmp.sy = v8->field_4;
    tmp.sz = v8->field_8;

// call_vtbl(obj, 3, 0x80002000, &v36, 0);

    xyz vaxis;
    vaxis.sx = bact->field_651.m11 * tmp.sz - bact->field_651.m12 * tmp.sy;
    vaxis.sy = bact->field_651.m12 * tmp.sx - bact->field_651.m10 * tmp.sz;
    vaxis.sz = bact->field_651.m10 * tmp.sy - bact->field_651.m11 * tmp.sx;

    float v44 = sqrt( POW2(vaxis.sx) + POW2(vaxis.sy) + POW2(vaxis.sz) );
    if ( v44 > 0.0 )
    {
        float v54 = tmp.sx * bact->field_651.m10 + tmp.sy * bact->field_651.m11 + tmp.sz * bact->field_651.m12;

        if ( v54 > 1.0 )
            v54 = 1.0;

        if ( v54 < -1.0 )
            v54 = -1.0;

        float v56 = acos(v54);
        float v45 = bact->maxrot * 2.0 * arg->field_0;

        if ( v56 > v45 )
            v56 = v45;

        if ( fabs(v56) < 0.01 )
            v56 = 0.0;

        vaxis.sx /= v44;
        vaxis.sy /= v44;
        vaxis.sz /= v44;

        mat3x3 mat2;

        mat_gen_axis_rotate(&vaxis, v56, &mat2, MAT_FLAG_INV_SIN);

        mat3x3 dst;
        mat_mult(&bact->field_651, &mat2, &dst);

        bact->field_651 = dst;
    }

    bact->field_621.sx = arg136.field_2C - bact->field_651.m10 * v5;
    bact->field_621.sy = arg136.field_30 - bact->field_651.m11 * v5;
    bact->field_621.sz = arg136.field_34 - bact->field_651.m12 * v5;

    return 1;
}

void ypacar_func129__sub0(NC_STACK_ypacar *caro, tank_arg129 *arg, xyz *darg)
{
    //__NC_STACK_ypacar *car = &caro->stack__ypacar;
    __NC_STACK_ypabact *bact = &caro->stack__ypabact;

    float v65 = bact->field_651.m20;
    //v5 = bact->field_651.m21;
    float v66 = bact->field_651.m22;

    int a4;
    call_vtbl(bact->self, 3, 0x80001004, &a4, 0);
    float v73;

    if ( a4 )
        v73 = -0.5;
    else
        v73 = -0.2;

    float v78 = bact->field_601 - bact->airconst * bact->field_611;

    if ( fabs(v78) > 0.1 )
    {
        xyz vaxis;
        vaxis.sx = bact->field_651.m00;
        vaxis.sy = bact->field_651.m01;
        vaxis.sz = bact->field_651.m02;

        float v8 = v73 * v78 / bact->force;

        mat3x3 mat2;

        mat_gen_axis_rotate(&vaxis, v8, &mat2, MAT_FLAG_INV_SIN);

        xyz tmp;
        tmp.sx = mat2.m00 * darg->sx + mat2.m01 * darg->sy + mat2.m02 * darg->sz;
        tmp.sy = mat2.m10 * darg->sx + mat2.m11 * darg->sy + mat2.m12 * darg->sz;
        tmp.sz = mat2.m20 * darg->sx + mat2.m21 * darg->sy + mat2.m22 * darg->sz;

        *darg = tmp;
    }

    float v76 = v65 * arg->field_4.sx + v66 * arg->field_4.sz;

    float tmpsq = sqrt( POW2(v65) + POW2(v66) );

    NDIV_CARRY(tmpsq);

    v76 = v76 / tmpsq;

    tmpsq = sqrt( POW2(arg->field_4.sx) + POW2(arg->field_4.sz) );

    v76 = v76 / tmpsq;

    if ( v76 > 1.0 )
        v76 = 1.0;

    if ( v76 < -1.0 )
        v76 = -1.0;

    if (arg->field_0 != 0.0)
    {
        float v77 = acos(v76) * (fabs(bact->field_611) * 0.002 / arg->field_0);

        if ( v77 > 0.001 )
        {
            if ( v65 * arg->field_4.sz - v66 * arg->field_4.sx < 0.0 )
                v77 = -v77;

            float v42 = bact->field_651.m20;
            float v43 = bact->field_651.m21;
            float v44 = bact->field_651.m22;

            float sn = sin(-v77);
            float cs = cos(v77);
            float ics = 1.0 - cs;

            xyz tmp;
            tmp.sx = (ics * v42 * v43 - sn * v44) * darg->sy + (ics * v42 * v42 + cs) * darg->sx       + (ics * v44 * v42 + sn * v43) * darg->sz;
            tmp.sy = (ics * v43 * v43 + cs) * darg->sy       + (ics * v42 * v43 + sn * v44) * darg->sx + (ics * v43 * v44 - sn * v42) * darg->sz;
            tmp.sz = (ics * v43 * v44 + sn * v42) * darg->sy + (ics * v44 * v42 - sn * v43) * darg->sx + (ics * v44 * v44 + cs) * darg->sz;

            *darg = tmp;
        }
    }
}

size_t ypacar_func129(NC_STACK_ypacar *obj, class_stru *zis, tank_arg129 *arg)
{
    __NC_STACK_ypacar *car = &obj->stack__ypacar;
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float v162 = 1.73;
    float v166 = 1.7;
    int v158 = 0;
    int v160 = 0;
    int v161 = 0;

    int a4, v149;
    call_vtbl(obj, 3, 0x80001004, &a4, 0);
    call_vtbl(obj, 3, 0x80001005, &v149, 0);

    float v5;

    if ( a4 )
        v5 = bact->vwr_overeof;
    else
        v5 = bact->overeof;

    float v126, v127, v128;
    float v129, v130, v131;
    float v132, v133, v134;
    float v167;

    if ( bact->field_611 < 0.0 )
    {
        v126 = (bact->field_651.m00 - bact->field_651.m20) * bact->vwr_radius * 0.7071 + bact->field_621.sx;
        v127 = (bact->field_651.m01 - bact->field_651.m21) * bact->vwr_radius * 0.7071 + bact->field_621.sy;
        v128 = (bact->field_651.m02 - bact->field_651.m22) * bact->vwr_radius * 0.7071 + bact->field_621.sz;

        v129 = bact->field_651.m20 * bact->vwr_radius  +  bact->field_621.sx;
        v130 = bact->field_651.m21 * bact->vwr_radius  +  bact->field_621.sy;
        v131 = bact->field_651.m22 * bact->vwr_radius  +  bact->field_621.sz;

        v132 = (-bact->field_651.m20 - bact->field_651.m00) * bact->vwr_radius * 0.7071 + bact->field_621.sx;
        v133 = (-bact->field_651.m21 - bact->field_651.m01) * bact->vwr_radius * 0.7071 + bact->field_621.sy;
        v134 = (-bact->field_651.m22 - bact->field_651.m02) * bact->vwr_radius * 0.7071 + bact->field_621.sz;

        v167 = -1.0;
    }
    else
    {
        v126 = (bact->field_651.m20 + bact->field_651.m00) * bact->vwr_radius * 0.7071 + bact->field_621.sx;
        v127 = (bact->field_651.m21 + bact->field_651.m01) * bact->vwr_radius * 0.7071 + bact->field_621.sy;
        v128 = (bact->field_651.m22 + bact->field_651.m02) * bact->vwr_radius * 0.7071 + bact->field_621.sz;

        v129 = bact->field_621.sx - bact->field_651.m20 * bact->vwr_radius;
        v130 = bact->field_621.sy - bact->field_651.m21 * bact->vwr_radius;
        v131 = bact->field_621.sz - bact->field_651.m22 * bact->vwr_radius;

        v132 = (bact->field_651.m20 - bact->field_651.m00) * bact->vwr_radius * 0.7071 + bact->field_621.sx;
        v133 = (bact->field_651.m21 - bact->field_651.m01) * bact->vwr_radius * 0.7071 + bact->field_621.sy;
        v134 = (bact->field_651.m22 - bact->field_651.m02) * bact->vwr_radius * 0.7071 + bact->field_621.sz;

        v167 = 1.0;
    }

    v127 += bact->vwr_overeof;
    v130 += bact->vwr_overeof;
    v133 += bact->vwr_overeof;

    ypaworld_arg136 arg136;
    ypaworld_arg136 arg136_1;
    ypaworld_arg136 arg136_2;

    arg136.pos_x = bact->field_621.sx;
    arg136_1.pos_x = bact->field_621.sx;
    arg136_2.pos_x = bact->field_621.sx;

    arg136.pos_y = bact->field_621.sy - v162 * bact->vwr_radius;
    arg136_1.pos_y = bact->field_621.sy - v162 * bact->vwr_radius;
    arg136_2.pos_y = bact->field_621.sy - v162 * bact->vwr_radius;

    arg136.pos_z = bact->field_621.sz;
    arg136_1.pos_z = bact->field_621.sz;
    arg136_2.pos_z = bact->field_621.sz;

    arg136.field_14 = (v132 - arg136.pos_x) * v166;
    arg136.field_18 = (v133 - arg136.pos_y) * v166;
    arg136.field_1C = (v134 - arg136.pos_z) * v166;

    arg136_1.field_14 = (v126 - arg136_1.pos_x) * v166;
    arg136_1.field_18 = (v127 - arg136_1.pos_y) * v166;
    arg136_1.field_1C = (v128 - arg136_1.pos_z) * v166;

    arg136_2.field_14 = (v129 - arg136_2.pos_x) * v166;
    arg136_2.field_18 = (v130 - arg136_2.pos_y) * v166;
    arg136_2.field_1C = (v131 - arg136_2.pos_z) * v166;

    arg136.field_40 = 0;
    arg136_1.field_40 = 0;
    arg136_2.field_40 = 0;

    call_method(car->ywo, 136, &arg136);

    if ( arg136.field_20 )
    {
        pol_entries2 *v48 = &arg136.field_3C->triangles[ arg136.field_38 ];

        if ( fabs(v48->field_4) < 0.6 )
        {
            float v176 = (bact->field_651.m21 * v48->field_4 + bact->field_651.m20 * v48->field_0 + bact->field_651.m22 * v48->field_8) * v167;

            if ( v176 <= 0.0 )
            {
                bact->field_601 = 0;
            }
            else
            {
                if ( v176 >= 0.82 )
                {
                    bact->field_601 = 0;
                    bact->field_611 = 0;
                    bact->field_621 = bact->field_62D;
                }
                else
                {
                    bact->field_621.sx = bact->field_62D.sx - v48->field_0 * 10.0;
                    bact->field_621.sz = bact->field_62D.sz - v48->field_8 * 10.0;
                }

                if ( bact->field_611 > 2.333333333333334 )
                {
                    sub_423F74(&bact->field_5A, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;

                        arg180.field_8 = arg136.field_2C;
                        arg180.field_C = arg136.field_34;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;

                        call_method(car->ywo, 180, &arg180);
                    }
                }
            }
            return 2;
        }
    }
    else
    {
        if ( !v149 )
            return 2;

        arg136.field_2C = bact->field_621.sx + arg136.field_14;
        arg136.field_30 = bact->field_621.sy + arg136.field_18 - v162 * bact->vwr_radius;
        arg136.field_34 = bact->field_621.sz + arg136.field_1C;

        v158 = 1;
    }

    call_method(car->ywo, 136, &arg136_1);

    if ( arg136_1.field_20 )
    {
        pol_entries2 *v54 = &arg136_1.field_3C->triangles[ arg136_1.field_38 ];

        if ( fabs(v54->field_4) < 0.6 )
        {
            float v182 = (bact->field_651.m21 * v54->field_4 + bact->field_651.m20 * v54->field_0 + bact->field_651.m22 * v54->field_8) * v167;

            if ( v182 <= 0.0 )
            {
                bact->field_601 = 0;
            }
            else
            {
                if ( v182 >= 0.82 )
                {
                    bact->field_611 = 0;
                    bact->field_621 = bact->field_62D;
                    bact->field_601 = 0;
                }
                else
                {
                    bact->field_621.sx = bact->field_62D.sx - v54->field_0 * 10.0;
                    bact->field_621.sz = bact->field_62D.sz - v54->field_8 * 10.0;
                }

                if ( bact->field_611 > 2.333333333333334 )
                {
                    sub_423F74(&bact->field_5A, 5);

                    if ( v149 )
                    {
                        yw_arg180 arg180;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = arg136_1.field_2C;
                        arg180.field_C = arg136_1.field_34;
                        arg180.effects_type = 5;

                        call_method(car->ywo, 180, &arg180);
                    }
                }
            }

            return 1;
        }
    }
    else
    {
        if ( !v149 )
            return 1;

        arg136_1.field_2C = bact->field_621.sx + arg136_1.field_14;
        arg136_1.field_30 = bact->field_621.sy + arg136_1.field_18 - v162 * bact->vwr_radius;
        arg136_1.field_34 = bact->field_621.sz + arg136_1.field_1C;

        v160 = 1;
    }

    call_method(car->ywo, 136, &arg136_2);

    if ( arg136_2.field_20 )
    {
        pol_entries2 *v54 = &arg136_2.field_3C->triangles[ arg136_2.field_38 ];

        if ( fabs(v54->field_4) < 0.6 )
        {
            bact->field_621.sx += bact->field_611 * bact->field_605.sx * arg->field_0 * 6.0;
            bact->field_621.sy += bact->field_611 * bact->field_605.sy * arg->field_0 * 6.0;
            bact->field_621.sz += bact->field_611 * bact->field_605.sz * arg->field_0 * 6.0;

            bact->field_601 = 0;

            if ( bact->field_611 < -2.333333333333334 )
            {
                sub_423F74(&bact->field_5A, 5);

                if ( v149 )
                {
                    yw_arg180 arg180;
                    arg180.field_4 = 1.0;
                    arg180.field_8 = arg136_2.field_2C;
                    arg180.field_C = arg136_2.field_34;
                    arg180.effects_type = 5;

                    call_method(car->ywo, 180, &arg180);
                }
            }
            return 3;
        }
    }
    else
    {
        arg136_2.field_2C = bact->field_621.sx + arg136_2.field_14;
        arg136_2.field_30 = bact->field_621.sy + arg136_2.field_18 - v162 * bact->vwr_radius;
        arg136_2.field_34 = bact->field_621.sz + arg136_2.field_1C;

        v161 = 1;
    }

    if ( a4 )
    {
        if ( v167 > 0.0 )
        {
            ypaworld_arg136 arg136_3;
            arg136_3.pos_x = bact->field_621.sx;
            arg136_3.pos_y = bact->field_621.sy;
            arg136_3.pos_z = bact->field_621.sz;

            arg136_3.field_14 = bact->field_651.m20 * bact->vwr_radius;
            arg136_3.field_18 = bact->field_651.m21 * bact->vwr_radius;
            arg136_3.field_1C = bact->field_651.m22 * bact->vwr_radius;

            arg136_3.field_40 = 0;

            call_method(car->ywo, 136, &arg136_3);

            if ( arg136_3.field_20 )
            {
                if ( arg136_3.field_3C->triangles[ arg136_3.field_38 ].field_4 < 0.6 )
                {
                    if ( bact->field_611 > 2.333333333333334 )
                    {
                        sub_423F74(&bact->field_5A, 5);

                        if ( v149 )
                        {
                            yw_arg180 arg180;
                            arg180.field_4 = 1.0;
                            arg180.field_8 = arg136_3.field_2C;
                            arg180.field_C = arg136_3.field_34;
                            arg180.effects_type = 5;

                            call_method(car->ywo, 180, &arg180);
                        }
                    }

                    bact->field_601 = 0;
                    bact->field_611 = 0;
                    bact->field_621 = bact->field_62D;
                }
            }
        }
    }

    if ( arg136_1.field_20 && arg136.field_20 && arg136_2.field_20 )
    {
        float v138 = arg136.field_2C - arg136_2.field_2C;
        float v139 = arg136.field_30 - arg136_2.field_30;
        float v140 = arg136.field_34 - arg136_2.field_34;

        float v135 = arg136_1.field_2C - arg136_2.field_2C;
        float v136 = arg136_1.field_30 - arg136_2.field_30;
        float v137 = arg136_1.field_34 - arg136_2.field_34;

        float v180 = v139 * v137 - v140 * v136;
        float v175 = v140 * v135 - v138 * v137;
        float v181 = v138 * v136 - v139 * v135;

        float v157 = sqrt( POW2(v180) + POW2(v175) + POW2(v181) ) * v167;
        if ( v157 != 0.0 )
        {
            v180 /= v157;
            v175 /= v157;
            v181 /= v157;
        }
        else
        {
            v175 = 1.0;
            v181 = 0.0;
            v180 = 0.0;
        }

        if ( v175 < -0.1 )
        {
            v175 *= -v175;
            v180 *= -v180;
            v181 *= -v181;
        }

        int v151;
        call_vtbl(obj, 3, 0x80002000, &v151, 0);

        if ( v151 )
        {
            xyz v141;
            v141.sx = v180;
            v141.sy = v175;
            v141.sz = v181;

            ypacar_func129__sub0(obj, arg, &v141);

            v180 = v141.sx;
            v175 = v141.sy;
            v181 = v141.sz;
        }

        float v72 = fabs(bact->field_611);

        xyz vaxis;
        vaxis.sx = bact->field_651.m11 * v181 - bact->field_651.m12 * v175;
        vaxis.sy = bact->field_651.m12 * v180 - bact->field_651.m10 * v181;
        vaxis.sz = bact->field_651.m10 * v175 - bact->field_651.m11 * v180;

        float v173;

        if ( v72 >= 0.5 )
            v173 = 0.007;
        else
            v173 = 0.01;

        float v156 = sqrt( POW2(vaxis.sx) + POW2(vaxis.sy) + POW2(vaxis.sz) );

        if ( v156 > 0.0 )
        {
            vaxis.sx /= v156;
            vaxis.sy /= v156;
            vaxis.sz /= v156;

            float v177 = v180 * bact->field_651.m10 + v175 * bact->field_651.m11 + v181 * bact->field_651.m12;

            if ( v177 > 1.0 )
                v177 = 1.0;

            if ( v177 < -1.0 )
                v177 = -1.0;

            float v174 = acos(v177);
            float v155 = bact->maxrot * 2.0 * arg->field_0;

            if ( v174 > v155 )
                v174 = v155;

            if ( fabs(v174) > v173 )
            {
                mat3x3 mat2;

                mat_gen_axis_rotate(&vaxis, v174, &mat2, MAT_FLAG_INV_SIN);

                mat3x3 dst;
                mat_mult(&bact->field_651, &mat2, &dst);

                bact->field_651 = dst;
            }
        }
    }

    if ( !arg136.field_20 && !arg136_1.field_20 && !arg136_2.field_20 )
    {
        bact->field_3D6 &= 0xFFFFFDFF;
        return 0;
    }

    if ( !v158 && !v160 && !v161 )
    {
        bact->field_621.sy = (arg136.field_30 + arg136_1.field_30 + arg136_2.field_30) * 0.33333334 - v5;
        return 0;
    }

    float v90 = bact->vwr_overeof * v166 * 0.8;

    ypaworld_arg136 arg136_4;
    arg136_4.pos_x = bact->field_621.sx;
    arg136_4.pos_y = bact->field_621.sy;
    arg136_4.pos_z = bact->field_621.sz;

    arg136_4.pos_y -= v90;

    arg136_4.field_18 = v90 * 2.0;
    arg136_4.field_14 = 0;
    arg136_4.field_1C = 0;

    arg136_4.field_40 = 0;

    call_method(car->ywo, 136, &arg136_4);

    if ( arg136_4.field_20 && (!arg136_4.field_20 || arg136_4.field_3C->triangles[ arg136_4.field_38 ].field_4 >= 0.6) )
    {
        bact->field_621.sy = arg136_4.field_30 - v5;
        return 0;
    }

    float v123 = 0.0;
    float v124 = 0.0;
    float v125 = 0.0;

    bact->field_3D6 = 0xFFFFFDFF;

    if ( v158 )
    {
        v123 = v167 * bact->field_651.m20 - bact->field_651.m00;
        v124 = v167 * bact->field_651.m21 - bact->field_651.m01;
        v125 = v167 * bact->field_651.m22 - bact->field_651.m02;
    }

    if ( v160 )
    {
        v123 += v167 * bact->field_651.m20 + bact->field_651.m00;
        v124 += v167 * bact->field_651.m21 + bact->field_651.m01;
        v125 += v167 * bact->field_651.m22 + bact->field_651.m02;
    }

    if ( v161 )
    {
        v123 -= v167 * bact->field_651.m20;
        v124 -= v167 * bact->field_651.m21;
        v125 -= v167 * bact->field_651.m22;
    }

    bact->field_621.sx += v123 * arg->field_0 * 400.0;
    bact->field_621.sy += v124 * arg->field_0 * 400.0;
    bact->field_621.sz += v125 * arg->field_0 * 400.0;

    return 0;
}



class_return ypacar_class_descr;

class_return * class_set_ypacar(int , ...)
{

    memset(ypacar_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypacar_class_descr.parent = "ypatank.class";

    ypacar_funcs[0] = (CLASSFUNC)ypacar_func0;
    ypacar_funcs[1] = (CLASSFUNC)ypacar_func1;
    ypacar_funcs[2] = (CLASSFUNC)ypacar_func2;
    ypacar_funcs[3] = (CLASSFUNC)ypacar_func3;
    ypacar_funcs[71] = (CLASSFUNC)ypacar_func71;
    ypacar_funcs[128] = (CLASSFUNC)ypacar_func128;
    ypacar_funcs[129] = (CLASSFUNC)ypacar_func129;

    ypacar_class_descr.vtbl = ypacar_funcs;
    ////ypacar_class_descr.varSize = sizeof(__NC_STACK_ypacar);
    ypacar_class_descr.varSize = sizeof(NC_STACK_ypacar) - offsetof(NC_STACK_ypacar, stack__ypacar); //// HACK
    ypacar_class_descr.field_A = 0;
    return &ypacar_class_descr;
}
