#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypaflyer.h"
#include "log.h"
#include <math.h>


stored_functions *classvtbl_get_ypaflyer();
class_return * class_set_ypaflyer(int, ...);

stored_functions ypaflyer_class_vtbl(class_set_ypaflyer);


class_stored ypaflyer_class_off (NULL, NULL, "MC2classes:ypaflyer.class", classvtbl_get_ypaflyer);


stored_functions *classvtbl_get_ypaflyer()
{
    return &ypaflyer_class_vtbl;
}

CLASSFUNC ypaflyer_funcs[1024];


int ypaflyer_func0__sub0(NC_STACK_ypaflyer *obj, __NC_STACK_ypaflyer *fly, stack_vals *stak)
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
                fly->ywo = (NC_STACK_ypaworld *)stk->value;
                fly->yw = &fly->ywo->stack__ypaworld;
                break;

            case 0x80002000:
                fly->field_10 = stk->value;
                break;

            }
            stk++;
        }
    }

    return 1;
}


NC_STACK_ypaflyer * ypaflyer_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypaflyer *obj = (NC_STACK_ypaflyer *)call_parent(zis, clss, 0, stak);

    if (obj)
    {
        __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;

        if ( ypaflyer_func0__sub0(obj, fly, stak) )
        {
            //call_vtbl(result, 3, 0x80001003, &bact, 0);

            __NC_STACK_ypabact *bact = &obj->stack__ypabact;

            fly->bact_internal = bact;

            bact->field_24 = 6;

            fly->field_c = 0;
            fly->field_10 = 0;
        }
        else
        {
            call_method(obj, 1);
            return NULL;
        }
    }

    return obj;
}

size_t ypaflyer_func1(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

int ypaflyer_func2__sub0(NC_STACK_ypaflyer *obj, __NC_STACK_ypaflyer *fly, stack_vals *stak)
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
                fly->field_10 = stk->value;
                break;

            }
            stk++;
        }
    }

    return 1;
}

void ypaflyer_func2(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);

    ypaflyer_func2__sub0(obj, &obj->stack__ypaflyer, stak);
}

void ypaflyer_func3__sub0(NC_STACK_ypaflyer *obj, __NC_STACK_ypaflyer *fly, stack_vals *stak)
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
                *(int *)stk->value = fly->field_10;
                break;

            }
            stk++;
        }
    }
}

void ypaflyer_func3(NC_STACK_ypaflyer *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);

    ypaflyer_func3__sub0(obj, &obj->stack__ypaflyer, stak);
}


void sb_0x4b255c__sub0(__NC_STACK_ypabact *bact, float a2)
{
    float v27 = sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m02) );

    if ( v27 >= 0.001 )
    {
        float v4 = sin(-a2);
        float v18 = cos(a2);

        float v5 = 1.0 / v27;

        mat3x3 mat2;

        mat2.m00 = (1.0 - v18) * bact->field_651.m00 * v5 + v18;
        mat2.m01 = 0.0 * (1.0 - v18) * bact->field_651.m00 * v5 - v4 * v5 * bact->field_651.m02;
        mat2.m02 = (1.0 - v18) * v5 * bact->field_651.m02 * bact->field_651.m00 * v5 + 0.0 * v4;

        mat2.m10 = 0.0 * (1.0 - v18) * bact->field_651.m00 * v5 + v4 * v5 * bact->field_651.m02;
        mat2.m11 = 0.0 * 0.0 * (1.0 - v18) + v18;
        mat2.m12 = 0.0 * (1.0 - v18) * v5 * bact->field_651.m02 - bact->field_651.m00 * v5 * v4;

        mat2.m20 = (1.0 - v18) * v5 * bact->field_651.m02 * bact->field_651.m00 * v5 - 0.0 * v4;
        mat2.m21 = bact->field_651.m00 * v5 * v4 + 0.0 * (1.0 - v18) * v5 * bact->field_651.m02;
        mat2.m22 = (1.0 - v18) * v5 * bact->field_651.m02 * v5 * bact->field_651.m02 + v18;

        mat3x3 v16;
        mat_mult(&bact->field_651, &mat2, &v16);

        bact->field_651 = v16;
    }
}

void sb_0x4b255c(__NC_STACK_ypaflyer *fly, float a2, xyz *a3, int a4)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    float v38 = bact->force / bact->airconst;

    if ( bact->field_605.sx != 0.0 || bact->field_605.sy != 0.0 || bact->field_605.sz != 0.0 )
    {
        if ( a3->sx == 0.0 && a3->sy == 0.0 && a3->sz == 0.0 )
        {
            a3->sx = bact->field_651.m20;
            a3->sy = bact->field_651.m21;
            a3->sz = bact->field_651.m22;
        }

        float v30 = bact->field_651.m20;
        //float v31 = bact->field_651.m21;
        float v32 = bact->field_651.m22;

        float v53 = bact->field_651.m01;

        if ( v53 > 1.0 )
            v53 = 1.0;

        if ( v53 < -1.0 )
            v53 = -1.0;

        float v56 = asin(v53);

        float v54 = bact->field_651.m21;

        if ( v54 > 1.0 )
            v54 = 1.0;

        if ( v54 < -1.0 )
            v54 = -1.0;

        float v52 = asin(v54);

        float v49;

        if ( fly->field_10 & 1 && bact->field_611 >= 0.1 )
        {
            float v55 = bact->field_605.sy;

            if ( v55 > 1.0 )
                v55 = 1.0;

            if ( v55 < -1.0 )
                v55 = -1.0;

            v49 = asin(v55) * bact->field_611 / v38;
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
            if ( (v30 != 0.0 || v32 != 0.0) && (a3->sx != 0.0 || a3->sz != 0.0) )
            {
                float v37 = (v32 * a3->sz + v30 * a3->sx) / sqrt( POW2(v32) + POW2(v30) );
                v37 = v37 / sqrt( POW2(a3->sz) + POW2(a3->sx) );

                float v57 = v37;

                if ( v57 > 1.0 )
                    v57 = 1.0;

                if ( v57 < -1.0 )
                    v57 = -1.0;

                v57 = acos(v57);

                if ( v57 < 0.001 )
                    v57 = 0.0;

                if ( v30 * a3->sz - v32 * a3->sx < 0.0 )
                    v57 = -v57;

                float v58;

                if ( bact->field_611 < 0.1 )
                    v58 = 0.0;
                else
                {
                    float aa2 = a2;

                    NDIV_CARRY(aa2);

                    v58 = fabs(bact->field_611) * bact->airconst / bact->force * (v57 * 1.45 / (aa2 * bact->maxrot));
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

                mat_mult(&mat1, &bact->field_651, &v26);

                bact->field_651 = v26;
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

            mat_mult(&mat1, &bact->field_651, &v26);

            bact->field_651 = v26;
        }
    }
}


void ypaflyer_func70__sub0(__NC_STACK_ypaflyer *fly, float angl)
{
    __NC_STACK_ypabact *bact = fly->bact_internal;

    if ( bact->field_645.sx != 0.0 || bact->field_645.sz != 0.0 )
    {

        float v6 = (bact->field_645.sx * bact->field_651.m20 + bact->field_645.sz * bact->field_651.m22);

        float tmpsq = sqrt( POW2(bact->field_645.sx) + POW2(bact->field_645.sz) );

        NDIV_CARRY(tmpsq);

        v6 /= tmpsq;

        tmpsq = sqrt(POW2(bact->field_651.m20) + POW2(bact->field_651.m22));

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

        if ( bact->field_651.m22 * bact->field_645.sx - bact->field_651.m20 * bact->field_645.sz > 0.0 )
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

        mat_mult(&v16, &bact->field_651, &dst);

        bact->field_651 = dst;
    }

    bact->field_601 = bact->force;

    float v14;

    if ( fabs(bact->field_645.sy) >= 0.1 )
    {
        if ( bact->field_645.sy > -0.1 )
            v14 = bact->mass * 0.7;
        else
            v14 = bact->mass * 2.5;
    }
    else
        v14 = bact->mass;

    fly->field_c = v14 * 9.80665;

    if ( bact->field_3D6 & 0x80 )
        fly->field_c = bact->mass * 2.5 * 9.80665;
}



void ypaflyer_func70(NC_STACK_ypaflyer *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;
    __NC_STACK_ypabact *bact = fly->bact_internal;

    float a2a = arg->field_4 / 1000.0;

    float v88 = sqrt( POW2(bact->field_639.sx) + POW2(bact->field_639.sy) + POW2(bact->field_639.sz) );

    if ( v88 > 0.0 )
    {
        float v5 = 1.0 / v88;
        bact->field_645.sx = bact->field_639.sx * v5;
        bact->field_645.sy = bact->field_639.sy * v5;
        bact->field_645.sz = bact->field_639.sz * v5;
    }

    int a4, v82, v79;

    call_vtbl(obj, 3, 0x80001004, &a4, 0);
    call_vtbl(obj, 3, 0x80001006, &v82, 0);
    call_vtbl(obj, 3, 0x80001007, &v79, 0);

    int v90 = call_method(fly->ywo, 145, bact);

    float v91;

    if ( a4 )
        v91 = bact->vwr_radius;
    else
        v91 = bact->radius;

    switch ( bact->field_3D5 )
    {
    case 1:
    {
        xyz a3;
        a3.sx = bact->field_651.m20;
        a3.sy = bact->field_651.m21;
        a3.sz = bact->field_651.m22;

        if ( v79 )
        {
            if ( v90 || (!bact->field_3DF && v88 < 1200.0) )
            {
                int arg87 = arg->field_4;

                call_method(obj, 87, &arg87);
            }
        }

        if ( !bact->field_3DE && !bact->field_3DF )
        {
            bact->field_3D5 = 3;

            if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_0 = 0;
                arg78.field_4 = 0;
                arg78.field_8 = 256;

                call_method(obj, 78, &arg78);
            }
            break;
        }

        //printf(" %f %f %f\n", bact->field_639.sx, bact->field_639.sy, bact->field_639.sz);

        ypaworld_arg136 arg136;
        arg136.field_20 = 0;
        arg136.field_40 = 0;
        arg136.pos_x = bact->field_62D.sx;
        arg136.pos_y = bact->field_62D.sy;
        arg136.pos_z = bact->field_62D.sz;
        arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
        arg136.field_18 = 0;
        arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;

        float v89 = sqrt( POW2(arg136.field_14) + POW2(arg136.field_1C) );

        if ( v89 <= 0.0 )
        {
            arg136.field_14 = bact->field_651.m20 * 300.0;
            arg136.field_18 = bact->field_651.m21 * 300.0;
            arg136.field_1C = bact->field_651.m22 * 300.0;
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

        if ( a4 || bact->field_3D6 & 0x20 || (v90 && v82) )
        {
            arg136_1.pos_x = bact->field_62D.sx;
            arg136_1.pos_y = bact->field_62D.sy;
            arg136_1.pos_z = bact->field_62D.sz;

            arg136_1.field_14 = arg136.field_14 * 0.93969 - arg136.field_1C * 0.34202;
            arg136_1.field_18 = arg136.field_18;
            arg136_1.field_1C = arg136.field_1C * 0.93969 + arg136.field_14 * 0.34202;

            call_method(fly->ywo, 136, &arg136_1);
        }

        ypaworld_arg136 arg136_2;

        arg136_2.field_20 = 0;
        arg136_2.field_40 = 0;

        if ( a4 || bact->field_3D6 & 0x10 || (v90 && v82) )
        {
            arg136_2.pos_x = bact->field_62D.sx;
            arg136_2.pos_y = bact->field_62D.sy;
            arg136_2.pos_z = bact->field_62D.sz;
            arg136_2.field_14 = arg136.field_14 * 0.93969 + arg136.field_1C * 0.34202;
            arg136_2.field_18 = arg136.field_18;
            arg136_2.field_1C = arg136.field_1C * 0.93969 - arg136.field_14 * 0.34202;

            call_method(fly->ywo, 136, &arg136_2);
        }

        if ( a4 || !(bact->field_3D6 & 0x30) || (v90 && v82) )
            call_method(fly->ywo, 136, &arg136);

        int v18 = 0;

        bact_arg88 arg88;
        arg88.pos1.sx = 0;
        arg88.pos1.sy = 0;
        arg88.pos1.sz = 0;

        if ( arg136.field_20 )
        {
            if ( v89 + v91 > arg136.field_24 * 300.0 )
            {
                arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;

                v18++;
            }
        }

        if ( arg136_1.field_20 )
        {
            if ( v89 + v91 > arg136_1.field_24 * 300.0 )
            {
                arg88.pos1.sx += arg136_1.field_3C->triangles[arg136_1.field_38].field_0;
                arg88.pos1.sy += arg136_1.field_3C->triangles[arg136_1.field_38].field_4;
                arg88.pos1.sz += arg136_1.field_3C->triangles[arg136_1.field_38].field_8;

                v18++;
            }
        }

        if ( arg136_2.field_20 )
        {
            if ( v89 + v91 > arg136_2.field_24 * 300.0 )
            {
                arg88.pos1.sx += arg136_2.field_3C->triangles[arg136_2.field_38].field_0;
                arg88.pos1.sy += arg136_2.field_3C->triangles[arg136_2.field_38].field_4;
                arg88.pos1.sz += arg136_2.field_3C->triangles[arg136_2.field_38].field_8;

                v18++;
            }
        }

        if ( v18 )
        {
            float v19 = 1.0 / (float)v18;

            arg88.pos2.sx = 0.7;
            arg88.pos2.sy = 2.0;
            arg88.pos2.sz = a2a;
            arg88.pos1.sx *= v19;
            arg88.pos1.sy *= v19;
            arg88.pos1.sz *= v19;

            call_method(obj, 88, &arg88);
        }
        else
        {
            bact->field_3D6 &= 0xFFFBFFFF;
        }

        if ( !arg136.field_20 && !arg136_1.field_20 && !arg136_2.field_20 )
        {
            bact->field_3D6 &= 0xFFFFFFCF;
            bact->field_3D6 |= 0x40;
        }

        if ( !(bact->field_3D6 & 0x30) )
        {
            if ( arg136_1.field_20 == 1 && arg136_2.field_20 == 1 )
            {
                if ( arg136_1.field_24 >= arg136_2.field_24 )
                    bact->field_3D6 |= 0x10;
                else
                    bact->field_3D6 |= 0x20;
            }

            if ( arg136_1.field_20 == 1 && !arg136_2.field_20 )
                bact->field_3D6 |= 0x20;

            if ( !arg136_1.field_20 && arg136_2.field_20 == 1 )
                bact->field_3D6 |= 0x10;

            if ( !arg136_1.field_20 && !arg136_2.field_20 && arg136.field_20 == 1 )
                bact->field_3D6 |= 0x10;
        }

        ypaworld_arg136 arg136_3;

        arg136_3.pos_x = bact->field_62D.sx;
        arg136_3.pos_y = bact->field_62D.sy;
        arg136_3.pos_z = bact->field_62D.sz;

        float v22 = bact->mass * 9.80665;

        if ( v22 <= bact->force )
            v22 = bact->force;

        float v100 = bact->airconst;

        if ( v100 < 10.0 )
            v100 = 10.0;

        arg136_3.field_14 = (bact->field_605.sx * 200.0 * bact->field_611) / (v22 / v100);

        if ( arg136_3.field_14 < -200.0 )
            arg136_3.field_14 = -200.0;

        if ( arg136_3.field_14 > 200.0 )
            arg136_3.field_14 = 200.0;

        arg136_3.field_18 = bact->height;

        arg136_3.field_1C = (bact->field_605.sz * 200.0 * bact->field_611) / (v22 / v100);

        if ( arg136_3.field_1C < -200.0 )
            arg136_3.field_1C = -200.0;

        if ( arg136_3.field_1C > 200.0 )
            arg136_3.field_1C = 200.0;

        arg136_3.field_40 = 0;

        call_method(fly->ywo, 136, &arg136_3);

        if ( arg136_3.field_20 )
        {
            bact->field_645.sy = -(1.0 - arg136_3.field_24);
        }
        else
        {
            NC_STACK_ypabact *v81;
            call_vtbl(fly->ywo, 3, 0x80002011, &v81, 0);

            if ( ( (bact->field_3DF != 2 || (bact->field_3f8->field_24 != 3 && v81 != bact->field_3f8->self))
                    && (bact->field_3DE != 2 || (bact->field_3e8->field_24 != 3 && v81 != bact->field_3e8->self)) )
                    || bact->field_645.sy >= -0.01 )
            {
                if ( bact->field_645.sy < 0.15 )
                    bact->field_645.sy = 0.15;
            }
        }

        if ( bact->field_3D6 & 0x30 )
            bact->field_645.sy = -0.2;

        if ( arg136_3.field_20 && arg136_3.field_24 * bact->height < bact->radius && bact->field_605.sy > 0.0 )
        {
            arg88.pos1.sx = arg136_3.field_3C->triangles[arg136_3.field_38].field_0;
            arg88.pos1.sy = arg136_3.field_3C->triangles[arg136_3.field_38].field_4;
            arg88.pos1.sz = arg136_3.field_3C->triangles[arg136_3.field_38].field_8;

            arg88.pos2.sx = 0.7;
            arg88.pos2.sy = 2.0;
            arg88.pos2.sz = a2a;

            call_method(obj, 88, &arg88);
        }
        else
        {
            bact->field_3D6 &= 0xFFFBFFFF;
        }

        if ( bact->field_645.sy != 0.0 )
        {
            float v39 = 1.0 / sqrt( POW2(bact->field_645.sy) + POW2(bact->field_645.sx) + POW2(bact->field_645.sz) );

            bact->field_645.sx *= v39;
            bact->field_645.sy *= v39;
            bact->field_645.sz *= v39;
        }

        float v92 = POW2(arg136.field_14) + POW2(arg136.field_1C);

        NDIV_CARRY(v92);

        if ( bact->field_3D6 & 0x10 )
        {
            float v44 = 1.0 / sqrt(v92);

            bact->field_645.sx = -arg136.field_1C * v44;
            bact->field_645.sz  = arg136.field_14 * v44;
        }
        else if ( bact->field_3D6 & 0x20 )
        {
            float v46 = 1.0 / sqrt(v92);

            bact->field_645.sx = arg136.field_1C * v46;
            bact->field_645.sz  = -arg136.field_14 * v46;
        }

        ypaflyer_func70__sub0(fly, a2a);

        bact_arg74 arg74;
        arg74.flag = 0;
        arg74.field_0 = a2a;

        call_method(obj, 74, &arg74);

        sb_0x4b255c(fly, a2a, &a3, 0);


        bact_arg75 arg75;
        arg75.field_14 = a2a;
        arg75.field_18 = bact->field_915;

        //printf("%d %d \n", bact->field_3DF, bact->field_3DE);

        if ( bact->field_3DF == 2 )
        {
            arg75.bct = bact->field_3f8;
            arg75.field_x = 1;

            call_method(obj, 75, &arg75);
        }
        else if ( bact->field_3DF == 1 )
        {
            arg75.field_0 = bact->field_3fc;
            arg75.bct = bact->field_3f8;
            arg75.field_x = 1;

            call_method(obj, 76, &arg75);
        }
        else if ( bact->field_3DE == 2 )
        {
            arg75.field_x = 0;
            arg75.bct = bact->field_3e8;

            call_method(obj, 75, &arg75);
        }
        else if ( bact->field_3DE == 1 )
        {
            arg75.field_0 = bact->field_3ec;
            arg75.bct = bact->field_3e8;
            arg75.field_x = 0;

            call_method(obj, 76, &arg75);
        }
        else
        {
            bact->field_3D6 &= 0xFFFFFFFC;

            if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_0 = 0;
                arg78.field_4 = 0;
                arg78.field_8 = 256;

                call_method(obj, 78, &arg78);
            }
        }

    }
    break;

    case 2:
        call_method(obj, 121, arg);
        break;

    case 3:
    {

        if ( bact->field_915 - bact->field_941 > 500 )
        {
            bact_arg110 arg110;
            arg110.field_one = bact->field_3DF;
            arg110.field_two = 1;

            bact_arg110 arg110_1;
            arg110_1.field_one = bact->field_3DE;
            arg110_1.field_two = 0;

            int v52 = call_method(obj, 110, &arg110);

            int v55 = call_method(obj, 110, &arg110_1);

            if ( v52 != 3 || v55 != 3 )
            {

                if ( !v52 )
                {
                    bact_arg67 arg67;
                    arg67.field_0 = 0;
                    arg67.field_4 = 1;

                    call_method(obj, 67, &arg67);
                }

                if ( !v55 )
                {
                    bact_arg67 arg67;
                    arg67.field_0 = 1;
                    arg67.targ.sx = bact->field_621.sx;
                    arg67.targ.sz = bact->field_621.sz;
                    arg67.field_4 = 0;

                    call_method(obj, 67, &arg67);
                }

                if ( bact->field_3DE || bact->field_3DF )
                {
                    bact_arg119 arg78;
                    arg78.field_8 = 512;
                    arg78.field_4 = 0;
                    arg78.field_0 = 1;

                    call_method(obj, 78, &arg78);
                    break;
                }
            }
        }

        int v80;
        call_vtbl(obj, 3, 0x8000100A, &v80, 0);

        if ( v80 )
        {
            bact->field_601 = 0;
            fly->field_c = 0;

            if ( bact->field_3D6 & 0x200 )
            {
                bact_arg119 arg78;
                arg78.field_8 = 0;
                arg78.field_4 = 0;
                arg78.field_0 = 3;

                call_method(obj, 78, &arg78);

                ypaworld_arg136 arg136_4;
                arg136_4.pos_x = bact->field_621.sx;
                arg136_4.pos_y = bact->field_621.sy;
                arg136_4.pos_z = bact->field_621.sz;
                arg136_4.field_14 = 0;
                arg136_4.field_18 = bact->overeof + 50.0;
                arg136_4.field_1C = 0;
                arg136_4.field_40 = 0;

                call_method(fly->ywo, 136, &arg136_4);

                if ( arg136_4.field_20 )
                    bact->field_621.sy = arg136_4.field_30 - bact->overeof;
            }
            else
            {
                bact_arg86 arg86;
                arg86.field_one = 0;
                arg86.field_two = arg->field_4;

                call_method(obj, 86, &arg86);
            }
        }
    }
    break;

    case 4:
        call_method(obj, 99, arg);
        break;

    case 5:
        call_method(obj, 112, arg);
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

    mat_mult(&bact->field_651, &mat2, &v6);

    bact->field_651 = v6;
}



void ypaflyer_func71(NC_STACK_ypaflyer *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;

    fly->bact_internal->airconst = fly->bact_internal->airconst2;

    float a2 = (float)arg->field_4 / 1000.0;

    int a4;
    call_vtbl(obj, 3, 0x80001007, &a4, 0);

    if ( fly->bact_internal->field_3D5 == 1 || fly->bact_internal->field_3D5 == 3 )
    {
        xyz a3;
        a3.sx = fly->bact_internal->field_651.m20;
        a3.sy = fly->bact_internal->field_651.m21;
        a3.sz = fly->bact_internal->field_651.m22;

        float v60 = -arg->inpt->sliders_vars[0] * fly->bact_internal->maxrot * a2;

        if ( a4 )
        {
            if ( !(fly->bact_internal->field_3D6 & 0x200) )
            {
                int arg87 = arg->field_4;

                call_method(obj, 87, &arg87);
            }
        }

        float v61;

        if ( fly->bact_internal->field_3D6 & 0x200 )
            v61 = 0.1;
        else
            v61 = 1.0;

        if ( fly->bact_internal->field_611 >= v61 )
        {
            if ( !(fly->bact_internal->field_3D6 & 0x100) )
            {
                bact_arg119 arg78;

                arg78.field_0 = 1;
                arg78.field_8 = 0;
                arg78.field_4 = 0;

                call_method(obj, 78, &arg78);
            }

            fly->bact_internal->field_3D6 &= 0xFFFFFDFF;
        }
        else
        {
            ypaworld_arg136 arg136;

            arg136.pos_x = fly->bact_internal->field_621.sx;
            arg136.pos_y = fly->bact_internal->field_621.sy;
            arg136.pos_z = fly->bact_internal->field_621.sz;
            arg136.field_1C = 0;
            arg136.field_14 = 0;

            float v9;

            if ( fly->bact_internal->vwr_overeof <= fly->bact_internal->vwr_radius )
                v9 = fly->bact_internal->vwr_radius;
            else
                v9 = fly->bact_internal->vwr_overeof;

            arg136.field_40 = 0;
            arg136.field_18 = v9 * 1.5;

            call_method(fly->ywo, 136, &arg136);

            if ( arg136.field_20 && fly->bact_internal->field_601 < fly->bact_internal->force * 0.001 && fly->field_c <= fly->bact_internal->mass * 9.80665 )
            {
                fly->bact_internal->field_3D6 |= 0x200;
                fly->bact_internal->field_611 = 0;
                fly->bact_internal->field_601 = 0;

                fly->field_c = fly->bact_internal->mass * 9.80665;
            }
            else
            {
                fly->bact_internal->field_3D6 &= 0xFFFFFDFF;
            }

            float tmp;

            if ( fly->bact_internal->field_3DE == 1)
            {
                float v13 = fly->bact_internal->field_3ec.sx - fly->bact_internal->field_621.sx;
                float v14 = fly->bact_internal->field_3ec.sz - fly->bact_internal->field_621.sz;

                tmp = sqrt(POW2(v13) + POW2(v14));
            }

            if ( fly->bact_internal->field_3DE != 1 || tmp <= 800.0 )
            {
                if ( fly->bact_internal->field_3D6 & 0x200 )
                {
                    if ( !(fly->bact_internal->field_3D6 & 0x100) )
                    {
                        bact_arg119 arg78;
                        arg78.field_0 = 3;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;

                        call_method(obj, 78, &arg78);
                    }
                }

                fly->bact_internal->field_3D5 = 1;
            }
            else
            {
                fly->bact_internal->field_3D5 = 3;

                if ( fly->bact_internal->field_3D6 & 0x200 )
                {
                    if ( !(fly->bact_internal->field_3D6 & 0x100) )
                    {
                        bact_arg119 arg78;
                        arg78.field_0 = 3;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;

                        call_method(obj, 78, &arg78);
                    }
                }
            }
        }

        ypaflyer_func71__sub1(fly->bact_internal, v60);

        fly->bact_internal->field_601 += fly->bact_internal->force * (a2 * 0.3) * arg->inpt->sliders_vars[2];

        if ( fly->bact_internal->field_601 > fly->bact_internal->force )
            fly->bact_internal->field_601 = fly->bact_internal->force;

        if ( fly->bact_internal->field_601 < 0.0 )
            fly->bact_internal->field_601 = 0;


        fly->field_c = (fabs(fly->bact_internal->field_611) / 111.0 + 1.0) * (arg->inpt->sliders_vars[1] * 20000.0) * 0.5 + fly->bact_internal->mass * 9.80665;

        float v22 = fly->bact_internal->p_cell_area->sector_height_meters - fly->bact_internal->field_621.sy;

        float v63 = 1.0 / fly->bact_internal->field_679;

        float v72 = fly->bact_internal->mass * 7.0 * 9.80665  * ( 1.0 - ( POW2(v22) * POW2(v63) ) );

        if ( fly->field_c > v72 )
            fly->field_c = v72;

        bact_arg79 arg79;
        arg79.field_18 = 4;
        arg79.field_20.sx = fly->bact_internal->field_651.m20;
        arg79.field_20.sy = fly->bact_internal->field_651.m21;
        arg79.field_20.sz = fly->bact_internal->field_651.m22;

        bact_arg106 arg106;
        arg106.field_0 = 5;
        arg106.field_4.sx = fly->bact_internal->field_651.m20;
        arg106.field_4.sy = fly->bact_internal->field_651.m21;
        arg106.field_4.sz = fly->bact_internal->field_651.m22;

        if ( call_method(obj, 106, &arg106) )
        {
            arg79.field_18 = 2;
            arg79.field_1C = arg106.ret_bact;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            arg79.field_0.sy = 0;
            arg79.field_0.sx = 0;
            arg79.field_0.sz = 0;
            arg79.field_2C = fly->bact_internal->weapon;
            arg79.field_30 = fly->bact_internal->field_915;

            if ( fly->bact_internal->field_915 % 2 )
                arg79.fire_point.sx = fly->bact_internal->fire_x;
            else
                arg79.fire_point.sx = -fly->bact_internal->fire_x;

            arg79.fire_point.sy = fly->bact_internal->fire_y;
            arg79.fire_point.sz = fly->bact_internal->fire_z;
            arg79.field_34 = ((arg->inpt->but_flags & 2) != 0) | 2;

            call_method(obj, 79, &arg79);
        }

        if ( fly->bact_internal->mgun != -1 )
        {
            if ( fly->bact_internal->field_3D6 & 0x100 )
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
                if ( !(fly->bact_internal->field_3D6 & 0x100) )
                {
                    bact_arg119 arg78;
                    arg78.field_8 = 0;
                    arg78.field_0 = 0;
                    arg78.field_4 = 256;

                    call_method(obj, 78, &arg78);
                }

                bact_arg105 arg105;

                arg105.field_0.sx = fly->bact_internal->field_651.m20;
                arg105.field_0.sy = fly->bact_internal->field_651.m21;
                arg105.field_0.sz = fly->bact_internal->field_651.m22;
                arg105.field_C = a2;
                arg105.field_10 = fly->bact_internal->field_915;

                call_method(obj, 105, &arg105);
            }
        }

        if ( arg->inpt->but_flags & 8 )
            call_method(obj, 97, arg);

        if ( fly->bact_internal->field_3D6 & 0x200 )
        {
            bact_arg74 arg74;
            arg74.field_0 = a2;
            arg74.flag = 0;

            call_method(obj, 74, &arg74);
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

                bact_arg74 arg74;
                arg74.field_0 = a2;
                arg74.flag = 0;

                call_method(obj, 74, &arg74);

                ypaworld_arg137 arg137;

                arg137.pos.sx = fly->bact_internal->field_651.m20 * 0.5 * fly->bact_internal->vwr_radius + fly->bact_internal->field_621.sx;
                arg137.pos.sy = fly->bact_internal->field_651.m21 * 0.5 * fly->bact_internal->vwr_radius + fly->bact_internal->field_621.sy;
                arg137.pos.sz = fly->bact_internal->field_651.m22 * 0.5 * fly->bact_internal->vwr_radius + fly->bact_internal->field_621.sz;
                arg137.pos2.sx = fly->bact_internal->field_605.sx;
                arg137.pos2.sy = fly->bact_internal->field_605.sy;
                arg137.pos2.sz = fly->bact_internal->field_605.sz;
                arg137.radius = fly->bact_internal->vwr_radius;
                arg137.coll_max = 10;
                arg137.field_30 = 0;
                arg137.collisions = v43;

                call_method(fly->ywo, 137, &arg137);

                if ( arg137.coll_count )
                {
                    v68 = 0.0;
                    v69 = 0.0;
                    v70 = 0.0;

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                    {
                        yw_137col *v31 = &arg137.collisions[ j ];

                        v69 += v31->pos2.sx;
                        v68 += v31->pos2.sy;
                        v70 += v31->pos2.sz;
                    }

                    float v65 = sqrt( POW2(v69) + POW2(v68) + POW2(v70) );

                    xyz v52;

                    if ( v65 != 0.0 )
                    {
                        float v38 = 1.0 / v65;
                        v52.sx = v69 * v38;
                        v52.sy = v68 * v38;
                        v52.sz = v70 * v38;
                    }
                    else
                    {
                        v52 = fly->bact_internal->field_605;
                    }

                    bact_arg88 arg88;
                    arg88.pos1 = v52;
                    arg88.pos2.sx = 0.7;
                    arg88.pos2.sy = 2.0;
                    arg88.pos2.sz = a2;

                    call_method(obj, 88, &arg88);

                    fly->field_c = fly->bact_internal->mass * 9.80665;

                    v67 = 1;
                }

                if ( !v67 )
                {
                    ypaworld_arg136 arg136;
                    arg136.pos_x = fly->bact_internal->field_62D.sx;
                    arg136.pos_y = fly->bact_internal->field_62D.sy;
                    arg136.pos_z = fly->bact_internal->field_62D.sz;
                    arg136.field_14 = fly->bact_internal->field_621.sx - fly->bact_internal->field_62D.sx;
                    arg136.field_18 = fly->bact_internal->field_621.sy - fly->bact_internal->field_62D.sy;
                    arg136.field_1C = fly->bact_internal->field_621.sz - fly->bact_internal->field_62D.sz;
                    arg136.field_40 = 0;

                    call_method(fly->ywo, 149, &arg136);

                    if ( arg136.field_20 )
                    {
                        bact_arg88 arg88;
                        arg88.pos1.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
                        arg88.pos1.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
                        arg88.pos1.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;
                        arg88.pos2.sx = 0.7;
                        arg88.pos2.sy = 2.0;
                        arg88.pos2.sz = a2;

                        call_method(obj, 88, &arg88);

                        v67 = 1;
                    }
                }

                if ( !v67 )
                {
                    fly->bact_internal->field_3D6 &= 0xFFFBFFFF;
                    break;
                }

                if ( !(fly->bact_internal->field_5A.samples_data[5].field_12 & 2) )
                {
                    if ( !(fly->bact_internal->field_3D6 & 0x40000) )
                    {
                        fly->bact_internal->field_3D6 |= 0x40000;

                        sub_423F74(&fly->bact_internal->field_5A, 5);

                        yw_arg180 arg180;

                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = 10.0 * v69 + fly->bact_internal->field_621.sx;
                        arg180.field_C = 10.0 * v70 + fly->bact_internal->field_621.sz;

                        call_method(fly->ywo, 180, &arg180);
                    }
                }

                fly->bact_internal->field_605.sy -= 0.2;
            }

            sb_0x4b255c(fly, a2, &a3, 1);
        }
    }
    else if ( fly->bact_internal->field_3D5 == 2 )
        call_method(obj, 121, arg);
}

void ypaflyer_func74(NC_STACK_ypaflyer *obj, class_stru *zis, bact_arg74 *arg)
{
    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;
    __NC_STACK_ypabact *bact = fly->bact_internal;

    bact->field_62D = bact->field_621;

    float v46;

    if ( bact->field_3D5 == 2 )
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
        v34 = bact->field_651.m20;
        v36 = bact->field_651.m22;
        v47 = bact->field_601;
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

    float v12 = bact->field_611 * bact->airconst;

    float v39 = 1.0 * v46 + 0.0 * v47 + -bact->field_605.sy * v12 + v35 * v45;
    float v38 = 0.0 * v46 + v34 * v47 + -bact->field_605.sx * v12 + 0.0 * v45;
    float v42 = 0.0 * v46 + v36 * v47 + -bact->field_605.sz * v12 + 0.0 * v45;

    float v33 = sqrt( POW2(v39) + POW2(v38) + POW2(v42) );

    if ( v33 > 0.0 )
    {
        float v19 = 1.0 / v33;
        float v20 = v33 / bact->mass * arg->field_0;

        float v41 = bact->field_605.sy * bact->field_611 + v39 * v19 * v20;
        float v40 = bact->field_605.sx * bact->field_611 + v38 * v19 * v20;
        float v43 = bact->field_605.sz * bact->field_611 + v42 * v19 * v20;

        bact->field_611 = sqrt( POW2(v41) + POW2(v40) + POW2(v43) );

        if ( bact->field_611 > 0.0 )
        {
            bact->field_605.sx = v40 / bact->field_611;
            bact->field_605.sy = v41 / bact->field_611;
            bact->field_605.sz = v43 / bact->field_611;
        }
    }

    if ( bact->field_611 > 1.5 )
    {
        bact->field_621.sx += bact->field_605.sx * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sy += bact->field_605.sy * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sz += bact->field_605.sz * bact->field_611 * arg->field_0 * 6.0;
    }

    call_method(obj, 115, 0);

    bact->field_5A.samples_data[0].pitch = bact->field_3BA;
    bact->field_5A.samples_data[0].volume = bact->field_3B6;

    float v48 = fabs(bact->field_611) / (bact->force / bact->airconst2);
    float v44;

    if ( bact->max_pitch <= -0.8 )
        v44 = 1.2;
    else
        v44 = bact->max_pitch;

    float v49 = v48 * v44;

    if ( v49 > v44 )
        v49 = v44;

    if ( bact->field_5A.samples_data[0].psampl )
        bact->field_5A.samples_data[0].pitch += (bact->field_5A.samples_data[0].psampl->SampleRate + bact->field_5A.samples_data[0].pitch) * v49;
}

size_t ypaflyer_func80(NC_STACK_ypaflyer *obj, class_stru *zis, bact_arg80 *arg)
{
    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;

    if ( !call_parent(zis, obj, 80, (stack_vals *)arg))
        return 0;

    fly->field_c = fly->bact_internal->mass * 9.80665;
    return 1;
}

void ypaflyer_func96(NC_STACK_ypaflyer *obj, class_stru *zis, void *)
{
    call_parent(zis, obj, 96, 0);

    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;

    fly->field_10 = 0;
    fly->field_c = 0;
}

void ypaflyer_func97(NC_STACK_ypaflyer *obj, class_stru *zis, void *)
{
    //call_parent(zis, obj, 97, arg); //Arg?
    call_parent(zis, obj, 97, 0);

    __NC_STACK_ypaflyer *fly = &obj->stack__ypaflyer;

    fly->bact_internal->field_601 = 0;
    fly->field_c = fly->bact_internal->mass * 9.80665;
}


class_return ypaflyer_class_descr;

class_return * class_set_ypaflyer(int, ...)
{

    memset(ypaflyer_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypaflyer_class_descr.parent = "ypabact.class";

    ypaflyer_funcs[0] = (CLASSFUNC)ypaflyer_func0;
    ypaflyer_funcs[1] = (CLASSFUNC)ypaflyer_func1;
    ypaflyer_funcs[2] = (CLASSFUNC)ypaflyer_func2;
    ypaflyer_funcs[3] = (CLASSFUNC)ypaflyer_func3;
    ypaflyer_funcs[70] = (CLASSFUNC)ypaflyer_func70;
    ypaflyer_funcs[71] = (CLASSFUNC)ypaflyer_func71;
    ypaflyer_funcs[74] = (CLASSFUNC)ypaflyer_func74;
    ypaflyer_funcs[80] = (CLASSFUNC)ypaflyer_func80;
    ypaflyer_funcs[96] = (CLASSFUNC)ypaflyer_func96;
    ypaflyer_funcs[97] = (CLASSFUNC)ypaflyer_func97;

    ypaflyer_class_descr.vtbl = ypaflyer_funcs;
    ////ypaflyer_class_descr.varSize = sizeof(__NC_STACK_ypaflyer);
    ypaflyer_class_descr.varSize = sizeof(NC_STACK_ypaflyer) - offsetof(NC_STACK_ypaflyer, stack__ypaflyer); //// HACK
    ypaflyer_class_descr.field_A = 0;
    return &ypaflyer_class_descr;
}
