#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "yparobo.h"

#include "math.h"


stored_functions *classvtbl_get_yparobo();
class_return * class_set_yparobo(int, ...);

stored_functions yparobo_class_vtbl(class_set_yparobo);


class_stored yparobo_class_off (NULL, NULL, "MC2classes:yparobo.class", classvtbl_get_yparobo);


stored_functions *classvtbl_get_yparobo()
{
    return &yparobo_class_vtbl;
}

CLASSFUNC yparobo_funcs[1024];

char **dword_54B0E0; // ypaworld strings
int dword_5B1128;


void  yparobo_func0__sub1(NC_STACK_yparobo *obj, __NC_STACK_yparobo *robo, stack_vals *stak)
{
    robo->field_1E7 = 30;
    robo->field_1EA = 90;
    robo->field_1e8 = 30;
    robo->field_1E9 = 50;
    robo->field_1EB = 50;
    robo->field_1ec = 80;
    robo->field_1ED = 100;
    robo->field_1DA = 0;
    robo->field_501 = 15;

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
                robo->wrld = (NC_STACK_ypaworld *)stk->value;
                robo->wrld_yw = &robo->wrld->stack__ypaworld;
                break;

            case 0x80001005:
                if ( stk->value )
                {
                    call_vtbl(robo->wrld, 2, 0x80002010, robo->bact_internal->self, 0);

                    robo->field_1dc |= 0x40;
                }
                break;

            case 0x80002002:
                robo->field_1E7 = stk->value;
                break;

            case 0x80002003:
                robo->field_1EA = stk->value;
                break;

            case 0x80002004:
                robo->field_1e8 = stk->value;
                break;

            case 0x80002005:
                robo->field_1E9 = stk->value;
                break;

            case 0x80002006:
                robo->field_1EB = stk->value;
                break;

            case 0x80002008:
                robo->field_4F5 = stk->value;
                break;

            case 0x80002009:
                robo->field_4F9 = stk->value;
                break;

            case 0x8000200A:
                robo->field_4FD = stk->value;
                break;

            case 0x8000200B:
                robo->field_501 = stk->value;
                break;

            case 0x8000200C:
                if ( stk->value )
                    robo->field_1DA |= 1;
                else
                    robo->field_1DA &= 0xFE;
                break;

            case 0x8000200D:
                if ( stk->value )
                    robo->field_1DA |= 2;
                else
                    robo->field_1DA &= 0xFD;
                break;

            case 0x8000200F:
                robo->field_1ED = stk->value;
                break;

            case 0x80002010:
                robo->field_1ec = stk->value;
                break;

            case 0x80002011:
                robo->field_1EE = stk->value;
                break;

            case 0x80002013:
            {
                __NC_STACK_ypabact *bact = robo->bact_internal;

                bact->field_5C9.m00 = 1.0;
                bact->field_5C9.m01 = 0;
                bact->field_5C9.m02 = 0;
                bact->field_5C9.m10 = 0;
                bact->field_5C9.m11 = 1.0;
                bact->field_5C9.m12 = 0;
                bact->field_5C9.m20 = 0;
                bact->field_5C9.m21 = 0;
                bact->field_5C9.m22 = 1.0;

                robo->bact_internal->field_5ED = stk->value * 3.141592653589793 / 180.0;

                mat_rotate_y(&robo->bact_internal->field_5C9, robo->bact_internal->field_5ED);
            }
            break;

            case 0x80002014:
                robo->field_24D = stk->value;
                break;

            case 0x80002015:
                robo->field_265 = stk->value;
                break;

            case 0x80002016:
                robo->field_235 = stk->value;
                break;

            case 0x80002017:
                robo->field_2B1 = stk->value;
                break;

            case 0x80002018:
                robo->field_281 = stk->value;
                break;

            case 0x80002019:
                robo->field_299 = stk->value;
                break;

            case 0x8000201A:
                robo->field_2E1 = stk->value;
                break;

            case 0x8000201B:
                robo->field_2C9 = stk->value;
                break;
            }

            stk++;
        }
    }
}

cellArea * yparobo_func0__sub0(__NC_STACK_yparobo *robo)
{

    yw_130arg sect_info;

    sect_info.pos_x = 600.0;
    sect_info.pos_z = -600.0;

    call_method(robo->wrld, 130, &sect_info);

    return sect_info.pcell;
}

NC_STACK_yparobo * yparobo_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_yparobo *obj = (NC_STACK_yparobo *)call_parent(zis, clss, 0, stak);

    if ( obj )
    {
        __NC_STACK_yparobo *robo = &obj->stack__yparobo;

        __NC_STACK_ypabact *bact_int;

        call_vtbl(obj, 3, 0x80001003, &bact_int, 0);

        robo->bact_internal = bact_int;

        yparobo_func0__sub1(obj, robo, stak);

        robo->bact_internal->field_24 = 3;

        robo->pcell = yparobo_func0__sub0(robo);

        call_vtbl(robo->wrld, 3, 0x80002018, &dword_54B0E0, 0);
    }
    return obj;
}

size_t yparobo_func1(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void yparobo_func2(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
}

void yparobo_func3(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

void yparobo_func68(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func68");
    call_parent(zis, obj, 68, stak);
}

void yparobo_func70(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func70");
    call_parent(zis, obj, 70, stak);
}

void yparobo_func71(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func71");
    call_parent(zis, obj, 71, stak);
}

void yparobo_func74(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func74");
    call_parent(zis, obj, 74, stak);
}

void yparobo_func77(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func77");
    call_parent(zis, obj, 77, stak);
}

void yparobo_func80(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func80");
    call_parent(zis, obj, 80, stak);
}

void yparobo_func82(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    printf("MAKE ME %s\n", "yparobo_func82");
    call_parent(zis, obj, 82, stak);
}

void yparobo_func96(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func96");
    call_parent(zis, obj, 96, arg);
}

void yparobo_func97(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func97");
    call_parent(zis, obj, 97, arg);
}

void yparobo_func114(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func114");
    call_parent(zis, obj, 114, arg);
}

void yparobo_func121(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func121");
    call_parent(zis, obj, 121, arg);
}

void yparobo_func128(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func128");
    call_parent(zis, obj, 128, arg);
}

void yparobo_func129(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func129");
    call_parent(zis, obj, 129, arg);
}

void yparobo_func130(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func130");
    call_parent(zis, obj, 130, arg);
}

void yparobo_func131(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func131");
    call_parent(zis, obj, 131, arg);
}

void yparobo_func132(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func132");
    call_parent(zis, obj, 132, arg);
}

void yparobo_func133(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func133");
    call_parent(zis, obj, 133, arg);
}

void yparobo_func134(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","yparobo_func134");
    call_parent(zis, obj, 134, arg);
}




class_return yparobo_class_descr;

class_return * class_set_yparobo(int , ...)
{

    memset(yparobo_funcs, 0, sizeof(CLASSFUNC) * 1024);

    yparobo_class_descr.parent = "ypabact.class";

    yparobo_funcs[0] = (CLASSFUNC)yparobo_func0;
    yparobo_funcs[1] = (CLASSFUNC)yparobo_func1;
    yparobo_funcs[2] = (CLASSFUNC)yparobo_func2;
    yparobo_funcs[3] = (CLASSFUNC)yparobo_func3;
    yparobo_funcs[68] = (CLASSFUNC)yparobo_func68;
    yparobo_funcs[70] = (CLASSFUNC)yparobo_func70;
    yparobo_funcs[71] = (CLASSFUNC)yparobo_func71;
    yparobo_funcs[74] = (CLASSFUNC)yparobo_func74;
    yparobo_funcs[77] = (CLASSFUNC)yparobo_func77;
    yparobo_funcs[80] = (CLASSFUNC)yparobo_func80;
    yparobo_funcs[82] = (CLASSFUNC)yparobo_func82;
    yparobo_funcs[96] = (CLASSFUNC)yparobo_func96;
    yparobo_funcs[97] = (CLASSFUNC)yparobo_func97;
    yparobo_funcs[114] = (CLASSFUNC)yparobo_func114;
    yparobo_funcs[121] = (CLASSFUNC)yparobo_func121;
    yparobo_funcs[128] = (CLASSFUNC)yparobo_func128;
    yparobo_funcs[129] = (CLASSFUNC)yparobo_func129;
    yparobo_funcs[130] = (CLASSFUNC)yparobo_func130;
    yparobo_funcs[131] = (CLASSFUNC)yparobo_func131;
    yparobo_funcs[132] = (CLASSFUNC)yparobo_func132;
    yparobo_funcs[133] = (CLASSFUNC)yparobo_func133;
    yparobo_funcs[134] = (CLASSFUNC)yparobo_func134;

    dword_5B1128 = 1;

    yparobo_class_descr.vtbl = yparobo_funcs;
    ////yparobo_class_descr.varSize = sizeof(__NC_STACK_yparobo);
    yparobo_class_descr.varSize = sizeof(NC_STACK_yparobo) - offsetof(NC_STACK_yparobo, stack__yparobo); //// HACK
    yparobo_class_descr.field_A = 0;
    return &yparobo_class_descr;
}
