#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "log.h"
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

key_value_stru yparobo_keys[2] =
{
    {"game.newai", KEY_TYPE_BOOL, 1},
    {"game.timeline", KEY_TYPE_DIGIT, 600000}
};

robo_t2 stru_5B0628[100];
int dword_515138[8];

struct rbo_xy
{
    int x;
    int y;
};

rbo_xy word_5182AE[8] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0},
    { 1,  0}, {-1, 1}, {0,  1}, { 1, 1}
};


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

                    robo->field_1DB |= 0x4000;
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


void  yparobo_func2__sub0(NC_STACK_yparobo *obj, __NC_STACK_yparobo *robo, stack_vals *stak)
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

            case 0x80001005:
                if ( stk->value )
                {
                    call_vtbl(robo->wrld, 2, 0x80002010, robo->bact_internal->self, 0);

                    robo->field_1DB |= 0x4000;
                }
                break;

            case 0x80002001:
            {
                roboProto *roboproto = (roboProto *)stk->value;
                for (int i = 0; i < 8; i++)
                {
                    if ( robo->guns[i].gun_obj )
                    {
                        __NC_STACK_ypabact *gun_bact;

                        call_vtbl(robo->guns[i].gun_obj, 3, 0x80001003, &gun_bact, 0);

                        if ( !( gun_bact->field_3D6 & 0x400 ) )
                            call_method(robo->guns[i].gun_obj, 77, 0);

                        call_method(obj, 118, robo->guns[i].gun_obj);

                        robo->guns[i].gun_obj = NULL;
                    }
                }

                __NC_STACK_ypabact *bact = robo->bact_internal;

                for (int i = 0; i < roboproto->robo_num_guns; i++)
                {
                    robo->guns[i] = roboproto->guns[i];

                    ypaworld_arg146 gun_req;

                    gun_req.pos.sx = bact->field_621.sx
                                     + bact->field_651.m00 * robo->guns[i].pos.sx
                                     + bact->field_651.m10 * robo->guns[i].pos.sy
                                     + bact->field_651.m20 * robo->guns[i].pos.sz;

                    gun_req.pos.sy = bact->field_621.sy
                                     + bact->field_651.m01 * robo->guns[i].pos.sx
                                     + bact->field_651.m11 * robo->guns[i].pos.sy
                                     + bact->field_651.m21 * robo->guns[i].pos.sz;

                    gun_req.pos.sz = bact->field_621.sz
                                     + bact->field_651.m02 * robo->guns[i].pos.sx
                                     + bact->field_651.m12 * robo->guns[i].pos.sy
                                     + bact->field_651.m22 * robo->guns[i].pos.sz;

                    gun_req.vehicle_id = robo->guns[i].robo_gun_type;

                    NC_STACK_ypagun *gun_obj = (NC_STACK_ypagun *)call_method(robo->wrld, 146, &gun_req);

                    robo->guns[i].gun_obj = gun_obj;

                    if ( gun_obj )
                    {
                        gun_arg128 v34;
                        v34.dir = robo->guns[i].dir;
                        v34.field_0 = 0;
                        call_method(gun_obj, 128, &v34);

                        call_vtbl(gun_obj, 2, 0x80002006, 1, 0);

                        __NC_STACK_ypabact *gun_bact;
                        call_vtbl(gun_obj, 3, 0x80001003, &gun_bact, 0);

                        gun_bact->owner = bact->owner;
                        gun_bact->field_2E = dword_5B1128;
                        gun_bact->field_32 = bact->self;

                        dword_5B1128++;

                        if ( robo->wrld->stack__ypaworld.field_757E )
                        {
                            gun_bact->ypabact__id |= gun_bact->owner << 24;
                            gun_bact->field_2E |= gun_bact->owner << 24;
                        }

                        gun_bact->field_3D4 = 60;

                        call_method(bact->self, 72, gun_obj);
                    }
                    else
                    {
                        ypa_log_out("Unable to create Robo-Gun\n");
                    }
                }

                robo->dock = roboproto->dock;
                bact->viewer = roboproto->viewer;
                bact->viewer_max_up = roboproto->robo_viewer_max_up;
                bact->viewer_max_down = roboproto->robo_viewer_max_down;
                bact->viewer_max_side = roboproto->robo_viewer_max_side;

                call_vtbl(obj, 2, 0x80001010, 1, 0);
                call_vtbl(obj, 2, 0x80001013, 1, 0);

                robo->coll = roboproto->coll;
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

            case 0x80002007:
                dword_5B1128 = stk->value;
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

void yparobo_func2(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);
    yparobo_func2__sub0(obj, &obj->stack__yparobo, stak);
}

void yparobo_func3__sub0(NC_STACK_yparobo *obj, __NC_STACK_yparobo *robo, stack_vals *stak)
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
            case 0x8000100E:
                *(rbcolls **)stk->value = &robo->coll;
                break;

            case 0x80002002:
                *(int *)stk->value = robo->field_1E7;
                break;

            case 0x80002003:
                *(int *)stk->value = robo->field_1EA;
                break;

            case 0x80002004:
                *(int *)stk->value = robo->field_1e8;
                break;

            case 0x80002005:
                *(int *)stk->value = robo->field_1E9;
                break;

            case 0x80002006:
                *(int *)stk->value = robo->field_1EB;
                break;

            case 0x80002007:
                *(int *)stk->value = dword_5B1128;
                break;

            case 0x80002008:
                *(int *)stk->value = robo->field_4F5;
                break;

            case 0x80002009:
                *(int *)stk->value = robo->field_4F9;
                break;

            case 0x8000200A:
                *(int *)stk->value = robo->field_4FD;
                break;

            case 0x8000200B:
                *(int *)stk->value = robo->field_501;
                break;

            case 0x8000200E:
                *(roboGun **)stk->value = robo->guns;
                break;

            case 0x8000200F:
                *(int *)stk->value = robo->field_1ED;
                break;

            case 0x80002010:
                *(int *)stk->value = robo->field_1ec;
                break;

            case 0x80002011:
                *(int *)stk->value = robo->field_1EE;
                break;

            case 0x80002012:
                *(int *)stk->value = robo->field_1DB;
                break;

            case 0x80002014:
                *(int *)stk->value = robo->field_24D;
                break;

            case 0x80002015:
                *(int *)stk->value = robo->field_265;
                break;

            case 0x80002016:
                *(int *)stk->value = robo->field_235;
                break;

            case 0x80002017:
                *(int *)stk->value = robo->field_2B1;
                break;

            case 0x80002018:
                *(int *)stk->value = robo->field_281;
                break;

            case 0x80002019:
                *(int *)stk->value = robo->field_299;
                break;

            case 0x8000201A:
                *(int *)stk->value = robo->field_2E1;
                break;

            case 0x8000201B:
                *(int *)stk->value = robo->field_2C9;
                break;

            case 0x8000201C:
                *(int *)stk->value = robo->field_502;
                break;

            case 0x8000201D:
                *(int *)stk->value = robo->field_503;
                break;

            case 0x8000201E:
                *(int *)stk->value = robo->field_505;
                break;
            }

            stk++;
        }
    }
}

void yparobo_func3(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
    yparobo_func3__sub0(obj, &obj->stack__yparobo, stak);
}

void yparobo_func68(NC_STACK_yparobo *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    if ( robo->bact_internal->field_3D5 == 2 )
    {
        int a4;
        call_vtbl(obj, 3, 0x8000100B, &a4, 0);
        a4 -= arg->field_4;
        call_vtbl(obj, 2, 0x8000100B, a4, 0);
    }

    robo->bact_internal->airconst = robo->bact_internal->airconst2;

    robo->bact_internal->field_5A.samples_data[0].pitch = robo->bact_internal->field_3BA;
    robo->bact_internal->field_5A.samples_data[0].volume = robo->bact_internal->field_3B6;

    for (int i = 0; i < 8; i++)
    {
        __NC_STACK_ypabact *gun_bact;

        if (robo->guns[i].gun_obj)
        {
            call_vtbl(robo->guns[i].gun_obj, 3, 0x80001003, &gun_bact, 0);

            gun_bact->owner = robo->bact_internal->owner;
        }
    }

    call_method(obj, 82, arg);

    if ( robo->bact_internal->field_3DE == 1 )
    {
        robo->bact_internal->field_639.sx = robo->bact_internal->field_3ec.sx - robo->bact_internal->field_621.sx;
        robo->bact_internal->field_639.sy = robo->bact_internal->field_3ec.sy - robo->bact_internal->field_621.sy;
        robo->bact_internal->field_639.sz = robo->bact_internal->field_3ec.sz - robo->bact_internal->field_621.sz;
    }
    else if ( robo->bact_internal->field_3DE != 2 )
    {
        robo->bact_internal->field_639.sx = 0;
        robo->bact_internal->field_639.sy = 0;
        robo->bact_internal->field_639.sz = 0;
    }

    int v9;
    call_vtbl(obj, 3, 0x80001005, &v9, 0);

    if ( v9 )
        call_method(obj, 71, arg);
    else
        call_method(obj, 70, arg);
}




void sub_4A6720(NC_STACK_ypaworld *ywo, __NC_STACK_ypabact *bact)
{
    yw_arg184_t4 arg184;
    arg184.type = 4;
    arg184.field_1 = bact->owner;
    arg184.field_2 = bact->id;
    arg184.field_4 = bact->field_621.sx * 256.0 / bact->field_18;
    arg184.field_5 = bact->field_621.sz * 256.0 / bact->field_1c;

    call_method(ywo, 184, &arg184);
}

int sub_4A587C(__NC_STACK_ypabact *bact)
{
    if ( bact->field_24 == 2 || bact->field_24 == 8 )
        return 1;

    bact_node *node = (bact_node *)bact->list2.head;

    while (node->next)
    {
        if (node->bact->field_24 == 2 || node->bact->field_24 == 8)
            return 1;

        node = (bact_node *)node->next;
    }

    return 0;
}

void sub_4A9F24(__NC_STACK_yparobo *robo, bact_node *node)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( robo->field_1DB & 8 )
    {
        bact_arg67 arg67;
        arg67.field_0 = robo->field_20F;
        arg67.field_4 = 0;
        arg67.field_8 = robo->field_20B;
        arg67.targ.sx = robo->field_1FF.sx;
        arg67.targ.sz = robo->field_1FF.sz;

        if ( arg67.field_0 == 2 )
        {
            arg67.field_4 = robo->field_211;

            if ( call_method(bact->self, 132, &arg67) )
            {
                if ( !((1 << robo->bact_internal->owner) & arg67.field_8->p_cell_area->field_39) )
                {
                    arg67.field_0 = 1;
                    arg67.targ.sx = robo->field_1FF.sx;
                    arg67.targ.sz = robo->field_1FF.sz;

                    robo->field_210 = 50;
                }
            }
            else
            {
                arg67.field_0 = 1;
                arg67.targ.sx = node->bact->field_621.sx;
                arg67.targ.sz = node->bact->field_621.sz;
            }
            arg67.field_4 = 0;
        }

        if ( node->bact->field_3D5 != 2 )
        {
            bact_arg124 arg124;
            arg124.field_0 = node->bact->field_621.sx;
            arg124.field_4 = node->bact->field_621.sz;

            if ( arg67.field_0 == 2 )
            {
                arg124.field_8 = arg67.field_8->field_621.sx;
                arg124.field_C = arg67.field_8->field_621.sz;
            }
            else
            {
                arg124.field_8 = arg67.targ.sx;
                arg124.field_C = arg67.targ.sz;
            }

            arg124.field_12 = 1;
            arg124.field_10 = 32;

            if ( sub_4A587C(node->bact) && call_method(node->bacto, 124, &arg124) )
            {
                arg124.field_10 = 32;

                call_method(node->bacto, 125, &arg124);

                if ( arg67.field_0 == 2 )
                {
                    node->bact->field_59c = arg67.field_8->field_2E;
                    node->bact->field_5a0 = arg67.field_8->owner;
                }
            }
            else
            {
                call_method(node->bacto, 67, &arg67);
            }
        }

        robo->field_1DB &= 0xFFFFFFF7;
    }

    call_vtbl(node->bacto, 2, 0x8000100D, (int)robo->field_210, 0);
    robo->field_1F3 = 0;
    robo->field_1F7 = 0;
    robo->field_1FB = 2000;
    robo->field_1DB &= 0xFF7FFFFF;
}

void sb_0x4a7010__sub0(__NC_STACK_yparobo *robo, bact_node *unit)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( robo->field_1FB <= 0 )
    {
        if ( robo->field_1EF > 0 )
        {
            NC_STACK_ypabact *unt = NULL;

            int v6 = bact->energy - bact->energy_2 * 0.2;

            if ( robo->field_1DB & 0x800000 )
                v6 += robo->field_50D;

            bact_arg81 arg81;
            arg81.enrg_type = 3;
            arg81.enrg_sum = 0;

            call_method(bact->self, 81, &arg81);


            int v23 = (arg81.enrg_sum * 0.05 + 1.0) * (float)unit->bact->energy_2;

            if ( v6 > (signed int)v23 )
            {
                ypaworld_arg146 arg146;
                arg146.vehicle_id = unit->bact->id;
                arg146.pos.sx = bact->field_621.sx + robo->dock.sx;
                arg146.pos.sy = bact->field_621.sy + robo->dock.sy;
                arg146.pos.sz = bact->field_621.sz + robo->dock.sz;

                unt = (NC_STACK_ypabact *)call_method(robo->wrld, 146, &arg146);
            }

            if ( unt )
            {
                __NC_STACK_ypabact *untbct;
                call_vtbl(unt, 3, 0x80001003, &untbct, 0);

                untbct->field_3D5 = 4;
                untbct->owner = bact->owner;
                untbct->field_2E = unit->bact->field_2E;
                untbct->field_32 = bact->self;

                int v20;
                call_vtbl(bact->self, 3, 0x80001007, &v20, 0);
                call_vtbl(unt, 2, 0x80001007, v20, 0);

                call_method(unit->bacto, 72, unt);

                bact_arg67 arg67;
                arg67.field_0 = 1;
                arg67.field_4 = 0;
                arg67.targ.sx = bact->field_621.sx + bact->field_651.m20 * 1200.0 * 0.5;
                arg67.targ.sz = bact->field_621.sz + bact->field_651.m22 * 1200.0 * 0.5;
                call_method(unt, 67, &arg67);

                if ( robo->wrld->stack__ypaworld.field_757E )
                {
                    untbct->ypabact__id |= untbct->owner << 24;

                    char v14[44];
                    /**(_DWORD *)v14 = 1001;
                    v14[12] = untbct->owner;
                    *(float *)&v14[16] = untbct->field_621.sx;
                    *(float *)&v14[20] = untbct->field_621.sy;
                    *(float *)&v14[24] = untbct->field_621.sz;
                    v14[41] = untbct->id;
                    *(_DWORD *)&v14[28] = untbct->parent_bact->ypabact__id;
                    *(_DWORD *)&v14[32] = untbct->ypabact__id;
                    v14[40] = 2;
                    *(_DWORD *)&v14[36] = unit->bact->field_2E;*/

                    yw_arg181 arg181;
                    arg181.field_14 = 2;
                    arg181.val_size = 44;
                    arg181.field_10 = 0;
                    arg181.field_18 = 1;
                    arg181.value = v14;

                    call_method(robo->wrld, 181, &arg181);
                }

                if ( robo->field_1DB & 0x800000 )
                {
                    if ( v23 > robo->field_50D )
                    {
                        bact->energy -= v23 - robo->field_50D;
                        robo->field_50D = 0;
                    }
                    else
                    {
                        robo->field_50D -= v23;
                    }
                }
                else
                {
                    bact->energy -= v23;
                }

                robo->field_1EF -= untbct->energy;

                bact_arg119 arg78;
                arg78.field_4 = 0;
                arg78.field_8 = 0;
                arg78.field_0 = 4;
                call_method(unt, 78, &arg78);

                untbct->field_931 = untbct->energy_2 * 0.2;
                robo->field_1FB = untbct->field_931 + 2000;
                robo->field_1F3++;

                sub_4A6720(robo->wrld, untbct);
            }
            else
            {
                sub_4A9F24(robo, unit);
            }
        }
        else
        {
            sub_4A9F24(robo, unit);
        }
    }
}


int sub_4A5A08(__NC_STACK_ypabact *bact, float a2, float a3)
{
    int v5 = 0;
    if ( bact->field_24 == 2 || bact->field_24 == 8 )
    {
        v5 = 1;
    }
    else
    {
        bact_node *node = (bact_node *)bact->list2.head;
        while (node->next)
        {
            if (node->bact->field_24 == 2 || node->bact->field_24 == 8)
            {
                v5 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }
    }

    if ( v5 )
    {
        bact_arg124 arg125;
        arg125.field_10 = 32;
        arg125.field_0 = bact->field_621.sx;
        arg125.field_4 = bact->field_621.sz;
        arg125.field_8 = a2;
        arg125.field_12 = 1;
        arg125.field_C = a3;
        if ( !call_method(bact->self, 125, &arg125) )
            return 0;
    }
    else
    {
        bact_arg67 arg67;
        arg67.targ.sx = a2;
        arg67.field_0 = 1;
        arg67.field_4 = 0;
        arg67.targ.sz = a3;
        call_method(bact->self, 67, &arg67);
    }
    return 1;
}

int sub_4A58C0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2)
{
    int v5 = 0;
    if ( bact->field_24 == 2 || bact->field_24 == 8 )
    {
        v5 = 1;
    }
    else
    {
        bact_node *node = (bact_node *)bact->list2.head;
        while (node->next)
        {
            if (node->bact->field_24 == 2 || node->bact->field_24 == 8)
            {
                v5 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }
    }

    if ( v5 )
    {
        bact_arg124 arg124;
        arg124.field_10 = 32;
        arg124.field_0 = bact->field_621.sx;
        arg124.field_4 = bact->field_621.sz;
        arg124.field_8 = bact2->field_621.sx;
        arg124.field_C = bact2->field_621.sz;
        arg124.field_12 = 1;

        if ( !call_method(bact->self, 124, &arg124) )
            return 0;

        if ( arg124.field_10 > 1 )
        {
            arg124.field_10 = 32;
            call_method(bact->self, 125, &arg124);

            bact->field_59c = bact2->field_2E;
            bact->field_5a0 = bact2->owner;
            return 1;
        }
    }

    bact_arg67 arg67;
    arg67.field_8 = bact2;
    arg67.field_4 = 0;
    arg67.field_0 = 2;
    call_method(bact->self, 67, &arg67);
    return 1;
}

void sb_0x4a7010__sub1__sub0(bact_node *nod1, bact_node *nod2)
{
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            __NC_STACK_ypabact *bact1 = nod1->bact;

            int v20 = i + bact1->field_c;

            if ( v20 > 0 && v20 < bact1->field_20 - 1 )
            {
                int v21 = j + bact1->field_10;

                if ( v21 > 0 && v21 < bact1->field_22 - 1 )
                {
                    cellArea *pcell = &bact1->p_cell_area[i + j * bact1->field_20];

                    if ( pcell->owner == bact1->owner && pcell->field_3A == 2 )
                    {
                        float yy = (v21 + 0.5) * -1200.0;
                        float xx = (v20 + 0.5) * 1200.0;
                        sub_4A5A08(nod2->bact, xx, yy);

                        call_vtbl(nod2->bacto, 2, 0x8000100D, 25, 0);
                        return;
                    }

                    if ( (1 << nod2->bact->owner) & pcell->field_39 )
                    {
                        __NC_STACK_ypabact *v10 = (__NC_STACK_ypabact *)pcell->field_3C.head;

                        while( v10->next )
                        {
                            if ( v10->field_24 == 9 && nod1->bact->owner == v10->owner && v10->field_3D5 != 2 )
                            {
                                int a4;
                                call_vtbl(v10->self, 3, 0x80002006, &a4, 0);

                                if ( !a4 )
                                {
                                    if ( v10->parent_bacto != v10->field_32 )
                                        v10 = v10->parent_bact;

                                    sub_4A58C0(nod2->bact, v10);

                                    call_vtbl(nod2->bacto, 2, 0x8000100D, 25, 0);
                                    return;
                                }
                            }
                            v10 = (__NC_STACK_ypabact *)v10->next;
                        }
                    }
                }
            }
        }
    }

    if ( (1 << nod2->bact->owner) & nod1->bact->p_cell_area->field_39 )
    {
        sub_4A58C0(nod2->bact, nod1->bact);

        call_vtbl(nod2->bacto, 2, 0x8000100D, 25, 0);
    }
    else
    {
        float yy = (nod1->bact->field_10 + 0.5) * -1200.0;
        float xx = (nod1->bact->field_c + 0.5) * 1200.0;

        sub_4A5A08(nod2->bact, xx, yy);

        call_vtbl(nod2->bacto, 2, 0x8000100D, 50, 0);
    }
}

void sb_0x4a7010__sub1(bact_node *unit, robo_t1 *rbt)
{
    __NC_STACK_ypabact *prnt = unit->bact->parent_bact;

    bact_node *v8 = NULL;

    bact_node *v6 = &prnt->list_node;

    while (v6->prev) // Go to head
        v6 = (bact_node *)v6->prev;

    v6 = (bact_node *)v6->next; // Go to first

    while (v6->next)
    {
        if ( v6->bact->field_2E == rbt->field_4 )
        {
            v8 = v6;
            break;
        }

        v6 = (bact_node *)v6->next;
    }

    if ( v8 )
    {
        if ( unit->bact->field_3DE == 2 )
        {
            if ( unit->bact->field_3e8->field_24 == 3 )
                sb_0x4a7010__sub1__sub0(v8, unit);
        }
        else if ( unit->bact->field_3DE == 1 )
        {
            cellArea *cell = (cellArea *)unit->bact->field_3e8;

            if (cell->owner == unit->bact->owner || v8->bact->p_cell_area == cell)
                sb_0x4a7010__sub1__sub0(v8, unit);
        }
    }
    else
    {
        rbt->field_4 = 0;
        rbt->field_0 = 0;
    }
}

void sub_4A5580(__NC_STACK_yparobo *robo, int a2)
{
    _NC_STACK_ypaworld *yw = &robo->wrld->stack__ypaworld;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    robo->field_511 -= a2;

    if ( robo->field_511 > 0 )
    {
        if ( robo->field_521 <= 0 )
        {
            int v23 = (1500 - robo->field_511) / 10;
            int v10 = robo->field_511 / 10;

            if ( bact->field_6BD[0].field_34 & 1 )
            {
                robo->field_521 = v10;
                bact->field_6BD[0].field_34 &= 0xFFFFFFFE;
            }
            else
            {
                robo->field_521 = v23;

                bact->field_6BD[0].field_4 = bact->field_621;
                bact->field_6BD[0].matrx = bact->field_651;
                bact->field_6BD[0].field_34 = 3;
                bact->field_6BD[0].field_0 = 1.0;
                bact->field_6BD[0].vp = bact->vp_genesis;
            }

            if ( robo->bact_internal->field_6BD[1].field_34 & 1 )
            {
                robo->field_521 = v10;
                bact->field_6BD[1].field_34 &= 0xFFFFFFFE;
            }
            else
            {
                robo->field_521 = v23;

                bact->field_6BD[1].field_4 = robo->field_515;
                bact->field_6BD[1].matrx = bact->field_651;
                bact->field_6BD[1].field_34 = 1;
                bact->field_6BD[1].vp = bact->vp_genesis;
            }
        }

        robo->field_521 -= a2;
    }
    else
    {
        robo->field_511 = 0;

        xyz tmp = bact->field_621;

        tmp.sy = robo->field_1D5;

        bact_arg80 v18;
        v18.pos = robo->field_515;
        v18.field_C = 0;

        call_method(bact->self, 80, &v18);

        if ( yw->field_757E )
        {
            bact->field_3D6 |= 0x20000000;

            bact_node *node = (bact_node *)bact->list2.head;

            while ( node->next )
            {
                if ( node->bact->field_24 == 9 )
                {
                    int a4;
                    call_vtbl(node->bacto, 3, 0x80002006, &a4, 0);

                    if ( a4 )
                        node->bact->field_3D6 |= 0x20000000;
                }
                node = (bact_node *)node->next;
            }
        }

        bact->field_62D = tmp;
        bact->field_62D.sy = tmp.sy;

        sub_423F74(&bact->field_5A, 10);

        robo->field_1DB &= 0xFFBFFFFF;
        bact->field_6BD[0].field_34 = 0;
        bact->field_6BD[1].field_34 = 0;

        if ( yw->field_757E )
        {
            char v17[20];
//      *(_DWORD *)v17 = 1034;
//      v17[12] = robo->bact_internal->owner;
//      *(_DWORD *)&v17[16] = robo->bact_internal->ypabact__id;

            yw_arg181 v16;
            v16.field_10 = 0;
            v16.field_14 = 2;
            v16.val_size = 20;
            v16.field_18 = 1;
            v16.value = v17;

            call_method(yw->self_full, 181, &v16);
        }
    }
}

void sub_4A1270(__NC_STACK_yparobo *robo, xyz *a2, float angle)
{
    for (int i = 0; i < 8; i++)
    {
        if ( robo->guns[i].gun_obj )
        {
            gun_arg129 v5;
            v5.vec.sx = a2->sx;
            v5.vec.sy = a2->sy;
            v5.vec.sz = a2->sz;
            v5.angle = -angle;

            call_method(robo->guns[i].gun_obj, 129, &v5);

            robo->guns[i].dir = v5.dir;
        }
    }
}

void sub_4A1014(__NC_STACK_yparobo *robo, float angle)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    mat3x3 tmp1;

    tmp1.m00 = cos(angle);
    tmp1.m01 = 0;
    tmp1.m02 = sin(angle);
    tmp1.m10 = 0;
    tmp1.m11 = 1.0;
    tmp1.m12 = 0;
    tmp1.m20 = -sin(angle);
    tmp1.m21 = 0;
    tmp1.m22 = cos(angle);

    mat3x3 tmp2;

    mat_mult(&tmp1, &bact->field_651, &tmp2);

    bact->field_651 = tmp2;

    xyz v6;
    v6.sx = bact->field_651.m10;
    v6.sy = bact->field_651.m11;
    v6.sz = bact->field_651.m12;

    sub_4A1270(robo, &v6, angle);

    robo->coll.field_0 = angle;
}

void sub_4A10E8(__NC_STACK_yparobo *robo, float angle)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    mat3x3 tmp1;

    tmp1.m00 = cos(angle);
    tmp1.m01 = 0;
    tmp1.m02 = sin(angle);
    tmp1.m10 = 0;
    tmp1.m11 = 1.0;
    tmp1.m12 = 0;
    tmp1.m20 = -sin(angle);
    tmp1.m21 = 0;
    tmp1.m22 = cos(angle);

    mat3x3 tmp2;

    mat_mult(&tmp1, &bact->field_651, &tmp2);

    bact->field_651 = tmp2;

    xyz v6;
    v6.sx = 0;
    v6.sy = 1.0;
    v6.sz = 0;

    sub_4A1270(robo, &v6, angle);

    robo->coll.field_0 = angle;
}

size_t yparobo_func70__sub1(__NC_STACK_yparobo *robo, float a2)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int a4, v81, v79;

    call_vtbl(bact->self, 3, 0x80001004, &a4, 0);
    call_vtbl(bact->self, 3, 0x80001005, &v81, 0);
    call_vtbl(bact->self, 3, 0x80001006, &v79, 0);

    xyz *v3 = &bact->field_621;
    xyz *v5 = &bact->field_62D;

    if ( v3->sx == v5->sx && v3->sy == v5->sy && v3->sz == v5->sz )
        return 0;

    bact_arg88 arg88_2;
    arg88_2.pos1.sz = 0;
    arg88_2.pos1.sy = 0;
    arg88_2.pos1.sx = 0;

    int v86 = 0;

    ypaworld_arg136 arg136;

    for (int i = 0; i < robo->coll.robo_coll_num; i++)
    {
        if ( !v81 || bact->field_3DE )
        {
            xyz tmp;
            tmp = robo->coll.roboColls[i].field_10;


            float tx = bact->field_621.sx +
                       bact->field_651.m00 * robo->coll.roboColls[i].robo_coll_x +
                       bact->field_651.m01 * robo->coll.roboColls[i].robo_coll_y +
                       bact->field_651.m02 * robo->coll.roboColls[i].robo_coll_z;

//            float ty = bact->field_621.sy +
//			      bact->field_651.m10 * robo->coll.roboColls[i].robo_coll_x +
//			      bact->field_651.m11 * robo->coll.roboColls[i].robo_coll_y +
//			      bact->field_651.m12 * robo->coll.roboColls[i].robo_coll_z;

            float tz = bact->field_621.sz +
                       bact->field_651.m20 * robo->coll.roboColls[i].robo_coll_x +
                       bact->field_651.m21 * robo->coll.roboColls[i].robo_coll_y +
                       bact->field_651.m22 * robo->coll.roboColls[i].robo_coll_z;


            arg136.pos_x = tmp.sx;
            arg136.pos_y = tmp.sy;
            arg136.pos_z = tmp.sz;
            arg136.field_40 = 0;

            float v82 = sqrt( (tmp.sz - tz) * (tmp.sz - tz) + (tmp.sx - tx) * (tmp.sx - tx) );

            if ( v82 <= 1.0 )
            {
                arg136.field_14 = bact->field_605.sx * 300.0;
                arg136.field_18 = bact->field_605.sy * 300.0;
                arg136.field_1C = bact->field_605.sz * 300.0;
            }
            else
            {
                float v17 = 1.0 / v82;

                arg136.field_14 = (tx - tmp.sx) * 300.0 * v17;
                arg136.field_18 = 0;
                arg136.field_1C = (tz - tmp.sz) * 300.0 * v17;
            }

            call_method(robo->wrld, 136, &arg136);


            if ( arg136.field_20 )
            {
                if ( arg136.field_24 * 300.0 >= robo->coll.roboColls[i].robo_coll_radius + v82)
                {
                    bact->field_3D6 &= 0xFFFBFFFF;
                    bact->field_3D6 |= 0x80;
                    return 0;
                }
                else if ( robo->coll.roboColls[i].robo_coll_radius > 0.01 )
                {
                    bact_arg88 arg88;
                    arg88.pos1.sx = arg136.field_3C->triangles[ arg136.field_38 ].field_0;
                    arg88.pos1.sy = arg136.field_3C->triangles[ arg136.field_38 ].field_4;
                    arg88.pos1.sz = arg136.field_3C->triangles[ arg136.field_38 ].field_8;

                    arg88.pos2.sx = 0.2;
                    arg88.pos2.sy = 2.0;
                    arg88.pos2.sz = a2;

                    call_method(bact->self, 88, &arg88);

                    if ( fabs(robo->coll.field_0) > 0.0 )
                    {
                        sub_4A1014(robo, -robo->coll.field_0);

                        robo->coll.field_0 = 0;
                    }

                    yw_130arg arg130;
                    arg130.pos_x = arg136.field_2C;
                    arg130.pos_z = arg136.field_34;

                    call_method(robo->wrld, 130, &arg130);

                    if ( v81 || !arg130.pcell->field_3A )
                    {
                        yw_arg129 v60;
                        v60.pos.sx = arg136.field_2C;
                        v60.pos.sy = arg136.field_30;
                        v60.pos.sz = arg136.field_34;
                        v60.unit = NULL;
                        v60.field_14 = 255;
                        v60.field_10 = v82 * 15000.0 / a2;

                        call_method(bact->self, 120, &v60);
                    }

                    bact->field_611 *= 0.4;
                    bact->field_3D6 |= 0x80;

                    return 1;
                }
            }
        }


        int v26 = call_method(robo->wrld, 145, bact);

        if ( (a4 || (v26 && v79)) && robo->coll.roboColls[i].robo_coll_radius > 0.01 )
        {
            xyz tmp;
            tmp = robo->coll.roboColls[i].field_10;


            float tx = bact->field_621.sx +
                       bact->field_651.m00 * robo->coll.roboColls[i].robo_coll_x +
                       bact->field_651.m01 * robo->coll.roboColls[i].robo_coll_y +
                       bact->field_651.m02 * robo->coll.roboColls[i].robo_coll_z;

//            float ty = bact->field_621.sy +
//			      bact->field_651.m10 * robo->coll.roboColls[i].robo_coll_x +
//			      bact->field_651.m11 * robo->coll.roboColls[i].robo_coll_y +
//			      bact->field_651.m12 * robo->coll.roboColls[i].robo_coll_z;

            float tz = bact->field_621.sz +
                       bact->field_651.m20 * robo->coll.roboColls[i].robo_coll_x +
                       bact->field_651.m21 * robo->coll.roboColls[i].robo_coll_y +
                       bact->field_651.m22 * robo->coll.roboColls[i].robo_coll_z;

            ypaworld_arg137 arg137;

            arg137.pos = tmp;

            float v83 = sqrt( (tmp.sz - tz) * (tmp.sz - tz) + (tmp.sx - tx) * (tmp.sx - tx) );

            if ( v83 <= 1.0 )
            {
                arg137.pos2.sx = bact->field_605.sx;
                arg137.pos2.sy = bact->field_605.sy;
                arg137.pos2.sz = bact->field_605.sz;
            }
            else
            {
                arg137.pos2.sx = 1.0 / v83 * (tx - tmp.sx);
                arg137.pos2.sz = arg137.pos2.sx;
                arg137.pos2.sy = 0;
            }

            yw_137col v57[32];

            arg137.radius = robo->coll.roboColls[i].robo_coll_radius;
            arg137.collisions = v57;
            arg137.field_30 = 0;
            arg137.coll_max = 32;

            call_method(robo->wrld, 137, &arg137);

            if ( arg137.coll_count )
            {
                v86++;

                float v89 = v83 * 15000.0;

                xyz ttmp;
                ttmp.sx = 0;
                ttmp.sy = 0;
                ttmp.sz = 0;

                for (int i = arg137.coll_count - 1; i >= 0; i--)
                {
                    yw_137col *clzn = &arg137.collisions[i];

                    ttmp.sx += clzn->pos2.sx;
                    ttmp.sy += clzn->pos2.sy;
                    ttmp.sz += clzn->pos2.sz;

                    yw_130arg arg130;
                    arg130.pos_x = clzn->pos1.sx;
                    arg130.pos_z = clzn->pos1.sz;

                    call_method(robo->wrld, 130, &arg130);

                    if ( v81 || !arg130.pcell->field_3A )
                    {
                        yw_arg129 v60;
                        v60.pos.sx = clzn->pos1.sx;
                        v60.pos.sy = clzn->pos1.sy;
                        v60.pos.sz = clzn->pos1.sz;
                        v60.unit = NULL;
                        v60.field_14 = 255;
                        v60.field_10 = v89 / a2;

                        call_method(bact->self, 120, &v60);
                    }
                }

                float v84 = sqrt(ttmp.sx * ttmp.sx + ttmp.sy * ttmp.sy + ttmp.sz * ttmp.sz);

                if ( v84 != 0.0 && !isnan(v84) )
                {
                    float v49 = 1.0 / v84;
                    arg88_2.pos1.sx += ttmp.sx * v49;
                    arg88_2.pos1.sy += ttmp.sy * v49;
                    arg88_2.pos1.sz += ttmp.sz * v49;
                }
                else
                {
                    arg88_2.pos1.sx += bact->field_605.sx;
                    arg88_2.pos1.sy += bact->field_605.sy;
                    arg88_2.pos1.sz += bact->field_605.sz;
                }
            }

            if ( v86 )
            {
                float v50 = 1.0 / (float)v86;
                arg88_2.pos1.sx = arg88_2.pos1.sx * v50;
                arg88_2.pos1.sy = arg88_2.pos1.sy * v50;
                arg88_2.pos1.sz = arg88_2.pos1.sz * v50;

                bact->field_621 = bact->field_62D;

                arg88_2.pos2.sz = a2;
                arg88_2.pos2.sy = 2.0;
                arg88_2.pos2.sx = 0.4;

                call_method(bact->self, 88, &arg88_2);

                if ( fabs(robo->coll.field_0) > 0.0 )
                {
                    sub_4A1014(robo, -robo->coll.field_0);
                    robo->coll.field_0 = 0;
                }

                bact->field_611 *= 0.4;
                bact->field_3D6 |= 0x80;

                return 1;
            }
        }
    }

    bact->field_3D6 &= 0xFFFBFFFF;

    arg136.pos_x = bact->field_621.sx;
    arg136.pos_y = bact->field_621.sy;
    arg136.pos_z = bact->field_621.sz;
    arg136.field_14 = bact->field_605.sx * 100.0;
    arg136.field_18 = bact->height * 1.5;
    arg136.field_1C = bact->field_605.sz * 100.0;
    arg136.field_40 = 0;

    call_method(robo->wrld, 136, &arg136);

    if ( arg136.field_20 )
    {
        if ( arg136.field_24 < 0.66 )
        {
            bact->field_645.sy = -1.0;
            bact->field_3D6 |= 0x80;
        }
    }
    else
    {
        bact->field_645.sy = 1.0;
        bact->field_3D6 &= 0xFFFFFF7F;
    }

    robo->coll.field_0 = 0;

    return 0;
}

void sub_4A4088(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact->field_621.sy = sin(arg->field_0 * 3.141592653589793 / 3000.0) * 25.0 + robo->field_1D5;

    for (int i = 0; i < 8; i++)
    {
        roboGun *gun = &robo->guns[i];

        if (gun->gun_obj)
        {
            bact_arg80 v11;
            v11.pos.sx = bact->field_621.sx + bact->field_651.m00 * gun->pos.sx + bact->field_651.m10 * gun->pos.sy + bact->field_651.m20 * gun->pos.sz;
            v11.pos.sy = bact->field_621.sy + bact->field_651.m01 * gun->pos.sx + bact->field_651.m11 * gun->pos.sy + bact->field_651.m21 * gun->pos.sz;
            v11.pos.sz = bact->field_621.sz + bact->field_651.m02 * gun->pos.sx + bact->field_651.m12 * gun->pos.sy + bact->field_651.m22 * gun->pos.sz;
            v11.field_C = 4;

            call_method(gun->gun_obj, 80, &v11);
        }
    }
}

void yparobo_func70__sub2__sub0(__NC_STACK_yparobo *robo)
{

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < robo->coll.robo_coll_num; i++)
    {
        robo->coll.roboColls[i].field_10.sx = bact->field_621.sx
                                              + bact->field_651.m00 * robo->coll.roboColls[i].robo_coll_x
                                              + bact->field_651.m01 * robo->coll.roboColls[i].robo_coll_y
                                              + bact->field_651.m02 * robo->coll.roboColls[i].robo_coll_z;

        robo->coll.roboColls[i].field_10.sy = bact->field_621.sy
                                              + bact->field_651.m10 * robo->coll.roboColls[i].robo_coll_x
                                              + bact->field_651.m11 * robo->coll.roboColls[i].robo_coll_y
                                              + bact->field_651.m12 * robo->coll.roboColls[i].robo_coll_z;

        robo->coll.roboColls[i].field_10.sz = bact->field_621.sz
                                              + bact->field_651.m20 * robo->coll.roboColls[i].robo_coll_x
                                              + bact->field_651.m21 * robo->coll.roboColls[i].robo_coll_y
                                              + bact->field_651.m22 * robo->coll.roboColls[i].robo_coll_z;
    }
}

int yparobo_func70__sub2__sub1(__NC_STACK_yparobo *robo)
{
    NC_STACK_ypabact *yw_1b7c;
    call_vtbl(robo->wrld, 3, 0x80002011, &yw_1b7c, 0);

    if ( yw_1b7c == robo->bact_internal->self )
        return 0;

    for (int i = 0; i < 8; i++)
        if (robo->guns[i].gun_obj && robo->guns[i].gun_obj == yw_1b7c)
            return 0;

    return 1;
}

void yparobo_func70__sub2(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_arg74 arg74;

    yparobo_func70__sub2__sub0(robo);

    mat3x3 *v5 = &bact->field_651;
    xyz *v6 = &bact->field_645;

    float v49 = arg->field_4 / 1000.0;

    robo->field_c = bact->mass * 9.80665;

    if ( bact->field_645.sy < -0.7 )
        robo->field_c = bact->mass * 3.0 * 9.80665;

    if ( bact->field_645.sy > 0.7 )
        robo->field_c = bact->mass * 0.91 * 9.80665;

    float v39;
    if ( bact->field_3DE )
        v39 = sqrt(bact->field_639.sx * bact->field_639.sx + bact->field_639.sz * bact->field_639.sz);
    else
        v39 = 0.0;

    if ( v39 >= 100.0 )
    {
        if ( bact->field_3D6 & 0x80 )
        {
            bact->airconst *= 3.0;

            robo->field_c = bact->mass * 2.5 * 9.80665;
            bact->field_601 = 0;

            float v38 = 1.0 - v49 * 4.0;
            if ( v38 < 0.0 )
                v38 = 0.1;

            bact->field_611 *= v38;
        }
        else if ( bact->field_3DE )
        {
            bact->field_601 = bact->force;
        }
        else
        {
            bact->field_601 = 0;
        }

        if ( !(bact->field_3D6 & 0x80) )
        {
            float v52 = sqrt(v5->m20 * v5->m20 + v5->m22 * v5->m22);
            float v51 = sqrt(v6->sz * v6->sz + v6->sx * v6->sx);

            if ( v52 <= 0.0001 || v51 <= 0.0001 )
            {
                bact->field_94D = 0;
                bact->field_3D6 &= 0xFFFF7FFF;
            }
            else
            {
                float v47 = (v5->m22 * v6->sz + v5->m20 * v6->sx) / (v51 * v52);

                if ( v47 > 1.0 )
                    v47 = 1.0;

                if ( v47 < -1.0 )
                    v47 = -1.0;

                float v23 = acos(v47);

                float a2 = v23;

                float v24 = fabs(v23);

                if ( v24 >= 0.001 )
                {
                    float mxx = v49 * bact->maxrot;

                    if ( v24 > mxx )
                        a2 = mxx;

                    if ( v5->m20 * v6->sz - v5->m22 * v6->sx < 0.0 )
                        a2 = -a2;

                    float v28;

                    if ( a2 <= 0.0 )
                    {
                        if ( !(bact->field_3D6 & 0x8000) || !bact->field_94D )
                            bact->field_94D = arg->field_0;

                        v28 = (arg->field_0 - bact->field_94D) * 0.9 * 0.001 + 0.1;
                        bact->field_3D6 |= 0x8000;
                    }
                    else
                    {
                        if ( bact->field_3D6 & 0x8000 || !bact->field_94D )
                            bact->field_94D = arg->field_0;

                        v28 = (arg->field_0 - bact->field_94D) * 0.9 * 0.001 + 0.1;
                        bact->field_3D6 &= 0xFFFF7FFF;
                    }

                    if ( v28 > 1.0 )
                        v28 = 1.0;

                    a2 *= v28;

                    sub_4A10E8(robo, a2);
                }
            }
        }

        float v31 = 1.0 / v39;

        arg74.field_0 = v49;
        arg74.flag = 0;
        arg74.vec.sx = bact->field_639.sx * v31;
        arg74.vec.sy = 0;
        arg74.vec.sz = bact->field_639.sz * v31;
    }
    else
    {
        int a4;
        call_vtbl(bact->self, 3, 0x80001004, &a4, 0);

        bact->field_601 = 0;

        if ( fabs(bact->field_605.sx * bact->field_611) <= 0.1 && fabs(bact->field_605.sz * bact->field_611) <= 0.1 )
        {
            if ( !(robo->field_1DB & 0x10) && !(robo->field_1DB & 0x20) && !(robo->field_1DB & 0x100) )
            {
                int v14 = yparobo_func70__sub2__sub1(robo);

                if ( v14 && robo->field_1DA & 1 )
                {
                    float v18;
                    if ( bact->field_3D6 & 0x80 )
                        v18 = bact->mass * 1.5;
                    else
                        v18 = bact->mass * 0.82;

                    robo->field_c = v18 * 9.80665;
                }
                else
                {
                    if ( fabs(bact->field_645.sy) < 0.7 )
                        bact->field_611 *= 0.35;
                }

                if ( v14 && robo->field_1DA & 2 )
                {
                    sub_4A1014(robo, bact->maxrot * 0.3 * v49);
                }
            }
        }
        else
        {
            bact->airconst *= 3.0;
            bact->field_611 *= 0.8;

            if ( fabs(bact->field_605.sx * bact->field_611) < 0.1 && fabs(bact->field_605.sz * bact->field_611) < 0.1 )
            {
                bact->field_605.sz = 0;
                bact->field_605.sx = bact->field_605.sz;

                bact_arg67 v34;
                v34.field_0 = 0;
                v34.field_4 = 0;
                call_method(bact->self, 67, &v34);
            }
        }
        arg74.vec.sx = 0;
        arg74.vec.sy = 0;
        arg74.vec.sz = 0;
        arg74.flag = 0;
        arg74.field_0 = v49;
    }

    call_method(bact->self, 74, &arg74);
}

void sub_4A4538(__NC_STACK_ypabact *bact)
{
    bact_arg67 arg67;
    arg67.field_0 = 0;
    arg67.field_4 = 1;

    call_method(bact->self, 67, &arg67);

    bact_node *node = (bact_node *)bact->list2.head;

    while( node->next )
    {
        call_method(node->bacto, 67, &arg67);

        node = (bact_node *)node->next;
    }
}

void sub_4A448C(__NC_STACK_ypabact *bact)
{

    bact_node *node = (bact_node *)bact->list2.head;

    while( node->next )
    {
        node->bact->field_59A = bact->field_59A;

        if ( node->bact->field_59A == 1 )
            node->bact->field_598 = 0;

        node->bact->field_3D6 |= 0x4000000;

        if ( bact->field_3D6 & 0x8000000 )
            node->bact->field_3D6 |= 0x8000000;
        else
            node->bact->field_3D6 &= 0xF7FFFFFF;

        for (int i = 0; i < 32; i++)
            node->bact->field_418[i] = bact->field_418[i];

        node = (bact_node *)node->next;
    }
}

void sub_4AB69C(__NC_STACK_ypabact *bact, char a2)
{
    if ( a2 )
        bact->field_3D6 |= 0x4000;
    else
        bact->field_3D6 &= 0xFFFFBFFF;

    bact_node *node = (bact_node *)bact->list2.head;

    while( node->next )
    {
        if ( a2 )
            node->bact->field_3D6 |= 0x4000;
        else
            node->bact->field_3D6 &= 0xFFFFBFFF;

        node = (bact_node *)node->next;
    }
}

int sb_0x4a45cc__sub0(__NC_STACK_ypabact *bact)
{
    xyz tmp;
    if ( bact->field_3DE == 2 )
    {
        tmp = bact->field_3e8->field_621;
    }
    else if ( bact->field_3DE == 1 )
    {
        tmp = bact->field_3ec;
    }
    else
        return 1;

    xyz tmp2;
    tmp2.sy = bact->field_621.sy - tmp.sy;
    tmp2.sx = bact->field_621.sx - tmp.sx;
    tmp2.sz = bact->field_621.sz - tmp.sz;

    float v19 = sqrt(tmp2.sy * tmp2.sy + tmp2.sx * tmp2.sx + tmp2.sz * tmp2.sz);

    bact_node *nod = (bact_node *)bact->list2.head;

    while( nod->next)
    {
        __NC_STACK_ypabact *bct = nod->bact;

        xyz tmp3;
        tmp3.sy = bct->field_621.sy - tmp.sy;
        tmp3.sx = bct->field_621.sx - tmp.sx;
        tmp3.sz = bct->field_621.sz - tmp.sz;

        float v20 = sqrt(tmp3.sy * tmp3.sy + tmp3.sx * tmp3.sx + tmp3.sz * tmp3.sz);

        if ( v20 < v19 )
            return 0;

        nod = (bact_node *)nod->next;
    }
    return 1;
}

void sb_0x4a45cc(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    bact_arg67 arg67;
    ypaworld_arg146 arg146;
    bact_arg119 arg78;
    bact_arg109 arg109;
    ypaworld_arg148 arg148;
    yw_arg181 arg181;
    robo_arg134 arg134;
    bact_arg124 arg124;

    __NC_STACK_ypabact *bact = robo->bact_internal;
    _NC_STACK_ypaworld *yw = robo->wrld_yw;
    NC_STACK_ypaworld *ywo = robo->wrld;


    int v6;

    switch ( arg->field_14 )
    {
    case 2: //Move squad
        arg->field_1C->field_3D6 &= 0xF3FFFFFF;

        if ( arg->field_20 )
        {
            arg67.targ.sx = arg->field_30.sx;
            arg67.targ.sz = arg->field_30.sz;
            arg67.field_0 = 1;
        }
        else
        {
            arg67.field_0 = 2;
            arg67.field_8 = arg->field_3C;
        }

        arg67.field_4 = 0;
        call_method(arg->field_1C->self, 67, &arg67);

        if ( !sb_0x4a45cc__sub0(arg->field_1C) )
        {
            arg109.field_4 = NULL;
            arg109.field_0 = 6;

            call_method(arg->field_1C->self, 109, &arg109);
        }

        if ( arg->field_1C->field_32 != arg->field_1C->parent_bacto )
            arg->field_1C = arg->field_1C->parent_bact;

        if ( arg->field_20 )
            v6 = sub_4A5A08(arg->field_1C, arg->field_30.sx, arg->field_30.sz);
        else
            v6 = sub_4A58C0(arg->field_1C, arg->field_3C);

        if ( !v6 )
        {
            if ( yw->GameShell )
                sub_423F74(&yw->GameShell->samples2_info, 1);
        }
        else
        {

            __NC_STACK_ypabact *v8 = arg->field_1C;

            if ( v8->parent_bacto != v8->field_32 )
                v8 = v8->parent_bact;

            arg134.unit = v8;
            arg134.field_C = 0;
            arg134.field_8 = 0;
            arg134.field_10 = 0;

            if ( v8->field_3DE == 1 )
            {
                cellArea *cell = (cellArea *)v8->field_3e8;
                if ( cell->owner == bact->owner )
                {
                    arg134.field_4 = 15;
                }
            }
            else if ( v8->field_3e8->owner == bact->owner )
            {
                arg134.field_4 = 15;
            }
            else
            {
                arg134.field_4 = 16;
            }

            arg134.field_14 = 38;
            call_method(bact->self, 134, &arg134);
        }
        sub_4A4538(arg->field_1C);
        sub_4AB69C(arg->field_1C, 0);
        break;

    case 3:
        //call_vtbl(robo->wrld, 3, 0x80002014, &a4, 0); //vhclproto
        if ( arg->field_40 <= robo->field_4F5 )
        {
            arg146.pos.sx = arg->field_30.sx;
            arg146.pos.sy = arg->field_30.sy;
            arg146.pos.sz = arg->field_30.sz;
            arg146.vehicle_id = arg->field_18;

            NC_STACK_ypabact *newbact = (NC_STACK_ypabact *)call_method(ywo, 146, &arg146);

            if ( newbact )
            {
                arg67.field_0 = 1;
                arg67.field_4 = 0;
                arg67.targ.sx = (arg146.pos.sx - bact->field_621.sx) * 4.0 + bact->field_621.sx;
                arg67.targ.sy = arg146.pos.sy;
                arg67.targ.sz = (arg146.pos.sz - bact->field_621.sz) * 4.0 + bact->field_621.sz;

                call_method(newbact, 67, &arg67);

                arg78.field_0 = 4;
                arg78.field_4 = 0;
                arg78.field_8 = 0;

                call_method(newbact, 78, &arg78);

                __NC_STACK_ypabact *bct;
                call_vtbl(newbact, 3, 0x80001003, &bct, 0);

                bct->field_931 = bct->energy_2 * 0.2;
                bct->field_2E = dword_5B1128;
                dword_5B1128++;

                bct->owner = bact->owner;
                call_method(bact->self, 72, newbact);

                if ( yw->field_757E )
                {
                    bct->ypabact__id |= bct->owner << 24;
                    bct->field_2E |= bct->owner << 24;

                    char v56[44];
                    /* *(_DWORD *)v56 = 1001;
                    v56[12] = bct->owner;
                    *(float *)&v56[16] = bct->field_621.sx;
                    *(float *)&v56[20] = bct->field_621.sy;
                    *(float *)&v56[24] = bct->field_621.sz;
                    v56[41] = bct->id;
                    *(_DWORD *)&v56[28] = bct->parent_bact->ypabact__id;
                    *(_DWORD *)&v56[32] = bct->ypabact__id;
                    v56[40] = 1;
                    *(_DWORD *)&v56[36] = bct->field_2E;*/

                    arg181.field_14 = 2;
                    arg181.val_size = 44;
                    arg181.field_10 = 0;
                    arg181.field_18 = 1;
                    arg181.value = v56;
                    call_method(ywo, 181, &arg181);
                }

                if ( bct->field_24 == 7 )
                    call_vtbl(newbact, 2, 0x8000100D, 25, 0);
                else
                    call_vtbl(newbact, 2, 0x8000100D, 60, 0);

                int v67;
                call_vtbl(bact->self, 3, 0x80001007, &v67, 0);
                call_vtbl(newbact, 2, 0x80001007, v67, 0);

                bct->field_32 = bact->self;
                robo->field_4F5 -= arg->field_40;

                sub_4A6720(ywo, bct);
            }
        }
        break;

    case 4:
        //call_vtbl(robo->wrld, 3, 0x80002014, &a4, 0); //vhclproto
        if ( arg->field_40 <= robo->field_4F5 )
        {
            arg146.pos = arg->field_30;
            arg146.vehicle_id = arg->field_18;
            NC_STACK_ypabact *newbact2 = (NC_STACK_ypabact *)call_method(ywo, 146, &arg146);

            if ( newbact2 )
            {
                arg78.field_0 = 4;
                arg78.field_4 = 0;
                arg78.field_8 = 0;
                call_method(newbact2, 78, &arg78);

                __NC_STACK_ypabact *bct;
                call_vtbl(newbact2, 3, 0x80001003, &bct, 0);

                bct->field_931 = bct->energy_2 * 0.2;
                bct->owner = bact->owner;
                bct->field_2E = arg->field_1C->field_2E;

                call_method(arg->field_1C->self, 72, newbact2);

                bct->field_3D4 = arg->field_1C->field_3D4;

                if ( yw->field_757E )
                {
                    bct->ypabact__id |= bct->owner << 24;

                    char v56[44];
                    /**(_DWORD *)v56 = 1001;
                    v56[12] = bct->owner;
                    *(float *)&v56[16] = bct->field_621.sx;
                    *(float *)&v56[20] = bct->field_621.sy;
                    *(float *)&v56[24] = bct->field_621.sz;
                    v56[41] = bct->id;
                    *(_DWORD *)&v56[28] = bct->parent_bact->ypabact__id;
                    *(_DWORD *)&v56[32] = bct->ypabact__id;
                    v56[40] = 2;
                    *(_DWORD *)&v56[36] = arg->field_1C->field_2E;*/
                    arg181.field_10 = 0;
                    arg181.field_14 = 2;
                    arg181.value = v56;
                    arg181.val_size = 44;
                    arg181.field_18 = 1;

                    call_method(ywo, 181, &arg181);
                }

                int v67;
                call_vtbl(bact->self, 3, 0x80001007, &v67, 0);
                call_vtbl(newbact2, 2, 0x80001007, v67, 0);

                bct->field_32 = bact->self;
                robo->field_4F5 -= arg->field_40;

                sub_4A6720(ywo, bct);
                sub_493DB0(bct, arg->field_1C, ywo);
            }
        }
        break;

    case 6:
        //call_vtbl(robo->wrld, 3, 0x80002013, &v70, 0); //buildproto

        if ( arg->field_40 <= robo->field_4F5 )
        {
            arg148.ownerID = bact->owner;
            arg148.ownerID2 = bact->owner;
            arg148.blg_ID = arg->field_18;
            arg148.field_C = 0;
            arg148.x = arg->field_24;
            arg148.y = arg->field_28;
            arg148.field_18 = 0;
            if ( call_method(ywo, 148, &arg148) )
            {
                robo->field_4F5 -= arg->field_40;

                if ( yw->field_757E )
                {
                    char v63[24];
                    /**(_DWORD *)v63 = 1012;
                    v63[12] = bact->owner;
                    *(_DWORD *)&v63[16] = *(_DWORD *)(v51 + 24);
                    *(_WORD *)&v63[20] = *(_WORD *)(v51 + 36);
                    *(_WORD *)&v63[22] = *(_WORD *)(v51 + 40);*/
                    arg181.field_10 = 0;
                    arg181.field_14 = 2;
                    arg181.value = v63;
                    arg181.field_18 = 1;
                    arg181.val_size = 24;

                    call_method(ywo, 181, &arg181);
                }
            }
        }
        break;

    case 10:
        if ( !(robo->field_1DB & 0x400000) )
        {
            sub_423F74(&bact->field_5A, 9);

            robo->field_1DB |= 0x400000;
            robo->field_4FD -= arg->field_40;
            robo->field_515 = arg->field_30;
            robo->field_511 = 1500;
            if ( yw->field_757E )
            {
                char v57[32];
                /**(_DWORD *)v57 = 1033;
                v57[12] = bact->owner;
                *(_DWORD *)&v57[28] = bact->ypabact__id;
                *(float *)&v57[16] = robo->field_515.sx;
                *(float *)&v57[20] = robo->field_515.sy;
                *(float *)&v57[24] = robo->field_515.sz;*/
                arg181.field_10 = 0;
                arg181.field_14 = 2;
                arg181.value = v57;
                arg181.val_size = 32;
                arg181.field_18 = 1;

                call_method(ywo, 181, &arg181);
            }
        }
        break;

    case 11:
    {
        arg->field_1C->field_3D6 |= 0x4000000;
        arg->field_1C->field_59c = 0;

        int v11 = 0;

        if ( arg->field_1C->field_24 != 2 && arg->field_1C->field_24 != 8 )
        {
            bact_node *nod = (bact_node *)arg->field_1C->list2.head;
            while (nod->next)
            {
                if ( nod->bact->field_24 == 2 || nod->bact->field_24 == 8 )
                {
                    v11 = 1;
                    break;
                }

                nod = (bact_node *)nod->next;
            }
        }
        else
            v11 = 1;

        if ( v11 )
        {
            arg124.field_0 = arg->field_1C->field_621.sx;
            arg124.field_4 = arg->field_1C->field_621.sz;
            arg124.field_8 = arg->field_30.sx;
            arg124.field_C = arg->field_30.sz;
            arg124.field_10 = 32;
            arg124.field_12 = 1;

            int v14 = call_method(bact->self, 124, &arg124);

            if ( v14 && arg124.field_10 > 0)
            {
                for (int i = 0; i < arg124.field_10; i++)
                {
                    arg->field_1C->field_418[i].sx = arg124.field_14[i].sx;
                    arg->field_1C->field_418[i].sz = arg124.field_14[i].sz;
                }
                arg->field_1C->field_59A = arg124.field_10;
            }
            else
            {
                arg->field_1C->field_59A = 1;
                arg->field_1C->field_418[0].sx = arg->field_30.sx;
                arg->field_1C->field_418[0].sz = arg->field_30.sz;

                if ( yw->GameShell )
                    sub_423F74(&yw->GameShell->samples2_info, 1);
            }
        }
        else
        {
            arg->field_1C->field_59A = 1;
            arg->field_1C->field_418[0].sx = arg->field_30.sx;
            arg->field_1C->field_418[0].sz = arg->field_30.sz;
        }

        arg->field_1C->field_598 = 0;

        arg67.targ.sx = arg->field_1C->field_418[0].sx;
        arg67.targ.sz = arg->field_1C->field_418[0].sz;
        arg67.field_4 = 0;
        arg67.field_0 = 1;
        call_method(arg->field_1C->self, 67, &arg67);

        sub_4A448C(arg->field_1C);
        sub_4A4538(arg->field_1C);
        sub_4AB69C(arg->field_1C, 0);
    }
    break;

    case 12:
        if ( arg->field_1C->field_59A < 32 && (arg->field_1C->field_3D6 & 0x4000000) )
        {
            int v72 = arg->field_1C->field_59A;
            int v20 = 0;

            if ( arg->field_1C->field_24 != 2 && arg->field_1C->field_24 != 8 )
            {
                bact_node *nod = (bact_node *)arg->field_1C->list2.head;
                while (nod->next)
                {
                    if ( nod->bact->field_24 == 2 || nod->bact->field_24 == 8 )
                    {
                        v20 = 1;
                        break;
                    }

                    nod = (bact_node *)nod->next;
                }
            }
            else
                v20 = 1;

            if ( v20 )
            {
                arg124.field_0 = arg->field_1C->field_418[v72 - 1].sx;
                arg124.field_4 = arg->field_1C->field_418[v72 - 1].sz;
                arg124.field_8 = arg->field_30.sx;
                arg124.field_C = arg->field_30.sz;
                arg124.field_10 = 32 - v72;
                arg124.field_12 = 1;

                if ( call_method(bact->self, 124, &arg124) && arg124.field_10 > 0 )
                {
                    for (int i = 0; i < arg124.field_10; i++)
                    {
                        arg->field_1C->field_418[i + v72].sx = arg124.field_14[i].sx;
                        arg->field_1C->field_418[i + v72].sz = arg124.field_14[i].sz;
                    }
                    arg->field_1C->field_59A += arg124.field_10;

                }
                else
                {
                    arg->field_1C->field_418[v72].sx = arg->field_30.sx;
                    arg->field_1C->field_418[v72].sz = arg->field_30.sz;
                    arg->field_1C->field_59A++;

                    if ( yw->GameShell )
                        sub_423F74(&yw->GameShell->samples2_info, 1);
                }
            }
            else
            {
                arg->field_1C->field_418[v72].sx = arg->field_30.sx;
                arg->field_1C->field_418[v72].sz = arg->field_30.sz;
                arg->field_1C->field_59A++;
            }
            if ( !(arg->field_1C->field_3D6 & 0x4000000) )
            {
                arg->field_1C->field_3D6 |= 0x4000000;

                arg67.targ.sx = arg->field_1C->field_418[v72].sx;
                arg67.targ.sz = arg->field_1C->field_418[v72].sz;
                arg67.field_4 = 0;
                arg67.field_0 = 1;
                call_method(arg->field_1C->self, 67, &arg67);
            }
            sub_4A448C(arg->field_1C);
        }
        break;

    case 13:
        arg->field_1C->field_3D6 |= 0x8000000;
        sub_4A448C(arg->field_1C);
        break;

    default:
        break;
    }
}

int yparobo_func70__sub4__sub0__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 2 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_2 * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}

void sub_4F4FF4(__NC_STACK_yparobo *robo, int a2, bact_arg67 *parg67)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v6 = a2 % bact->field_20;
    int v5_2 = a2 / bact->field_20;

    if ( v6 == bact->field_c && v5_2 == bact->field_10 )
    {
        if ( v6 >= 2 )
            v6--;
        else
            v6++;
    }
    else
    {
        if ( v6 > bact->field_c )
            v6--;
        if ( v6 < bact->field_c )
            v6++;

        if ( v5_2 > bact->field_10 )
            v5_2--;
        if ( v5_2 < bact->field_10 )
            v5_2++;
    }

    parg67->field_0 = 1;
    parg67->field_4 = 0;
    parg67->targ.sx = (v6 + 0.5) * 1200.0;
    parg67->targ.sz = -(v5_2 + 0.5) * 1200.0;
}

void yparobo_func70__sub4__sub0(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2F5 % bact->field_20;
    int yy = robo->field_2F5 / bact->field_20;

    float v35 = arg->field_4 * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].field_3A )
    {
        bact_arg67 arg67;
        arg67.field_0 = 0;
        arg67.field_4 = 0;

        call_method(bact->self, 67, &arg67);
        robo->field_2FD = 0;
    }
    else
    {
        BuildProto *buildprotos;
        call_vtbl(robo->wrld, 3, 0x80002013, &buildprotos, 0);

        int build_id = yparobo_func70__sub4__sub0__sub0(robo, buildprotos);

        if ( build_id == -1 )
        {
            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 0;

            call_method(bact->self, 67, &arg67);
            robo->field_2FD = 0;
        }
        else
        {

            if ( abs(bact->field_c - xx) > 1 || abs(bact->field_10 - yy) > 1 )
            {
                bact_arg67 arg67;
                sub_4F4FF4(robo, robo->field_2F5, &arg67);

                arg67.field_0 = 5;

                call_method(bact->self, 67, &arg67);
            }
            else
            {

                if ( bact->field_c != xx || bact->field_10 != yy )
                {
                    bact_arg67 arg67;
                    arg67.field_0 = 0;
                    arg67.field_4 = 0;
                    call_method(bact->self, 67, &arg67);

                    float v30 = -(yy + 0.5) * 1200.0 - bact->field_621.sz;
                    float v29 = (xx + 0.5) * 1200.0 - bact->field_621.sx;

                    float v16 = 1.0 / sqrt(v30 * v30 + v29 * v29);

                    v29 *= v16;
                    v30 *= v16;

                    float v41 = v29 * bact->field_651.m20 + v30 * bact->field_651.m22;

                    if ( v41 <= 0.9 )
                    {
                        if ( v41 > 1.0 )
                            v41 = 1.0;
                        if ( v41 < -1.0 )
                            v41 = -1.0;

                        float a2 = acos(v41);

                        if ( v29 * bact->field_651.m22 - v30 * bact->field_651.m20 > 0.0 )
                            a2 = -a2;

                        if ( a2 < -bact->maxrot * v35 )
                            a2 = -bact->maxrot * v35;

                        if ( a2 > bact->maxrot * v35 )
                            a2 = bact->maxrot * v35;

                        sub_4A10E8(robo, a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = bact->owner;
                        arg148.ownerID = bact->owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( call_method(robo->wrld, 148, &arg148) )
                        {
                            BuildProto *proto = &buildprotos[build_id];

                            if ( robo->field_509 >= proto->energy )
                            {
                                robo->field_509 -= proto->energy;
                            }
                            else
                            {
                                bact->energy -= proto->energy - robo->field_509;
                                robo->field_509 = 0;
                            }

                            robo->field_2FD = 0;

                            if ( robo->field_1DF )
                                robo->field_235 = robo->field_1E3 * (100 - robo->field_1e8) / 100;
                        }
                        else
                        {
                            bact_arg67 arg67;
                            arg67.field_0 = 0;
                            arg67.field_4 = 0;

                            call_method(bact->self, 67, &arg67);

                            robo->field_2FD = 0;
                        }
                    }
                }
                else
                {
                    bact_arg67 arg67;

                    sub_4F4FF4(robo, robo->field_2F5, &arg67);

                    arg67.field_0 = 5;

                    call_method(bact->self, 67, &arg67);
                }

            }

        }
    }
}

int yparobo_func70__sub4__sub2__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 1 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_2 * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}

void yparobo_func70__sub4__sub2(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2F5 % bact->field_20;
    int yy = robo->field_2F5 / bact->field_20;

    float v38 = arg->field_4 * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].field_3A )
    {
        bact_arg67 arg67;
        arg67.field_0 = 0;
        arg67.field_4 = 0;
        call_method(bact->self, 67, &arg67);

        robo->field_2FD = 0;
    }
    else
    {
        yw_arg176 arg176;
        arg176.owner = bact->owner;
        call_method(robo->wrld, 176, &arg176);

        if ( robo->pcell[ robo->field_2F5 ].field_2F != -1 && ( arg176.field_4 >= 0.9 || arg176.field_4 <= 0.001) )
        {
            BuildProto *buildprotos;
            call_vtbl(robo->wrld, 3, 0x80002013, &buildprotos, 0);

            int bldid = yparobo_func70__sub4__sub2__sub0(robo, buildprotos);

            if ( bldid == -1 )
            {
                bact_arg67 arg67;
                arg67.field_0 = 0;
                arg67.field_4 = 0;
                call_method(bact->self, 67, &arg67);
                robo->field_2FD = 0;
            }
            else
            {
                if ( abs(bact->field_c - xx) > 1 || abs(bact->field_10 - yy) > 1 )
                {
                    bact_arg67 arg67;
                    sub_4F4FF4(robo, robo->field_2F5, &arg67);
                    arg67.field_0 = 5;
                    call_method(bact->self, 67, &arg67);
                }
                else
                {

                    if ( bact->field_c != xx || bact->field_10 != yy )
                    {
                        bact_arg67 arg67;
                        arg67.field_0 = 0;
                        arg67.field_4 = 0;
                        call_method(bact->self, 67, &arg67);

                        float v31 = -(yy + 0.5) * 1200.0 - bact->field_621.sz;
                        float v30 = (xx + 0.5) * 1200.0 - bact->field_621.sx;

                        float v17 = 1.0 / sqrt(v31 * v31 + v30 * v30);

                        v30 *= v17;
                        v31 *= v17;

                        float v44 = v30 * bact->field_651.m20 + v31 * bact->field_651.m22;

                        if ( v44 <= 0.9 )
                        {
                            if ( v44 > 1.0 )
                                v44 = 1.0;

                            if ( v44 < -1.0 )
                                v44 = -1.0;

                            float a2 = acos(v44);

                            if ( v30 * bact->field_651.m22 - v31 * bact->field_651.m20 > 0.0 )
                                a2 = -a2;

                            if ( a2 < -bact->maxrot * v38 )
                                a2 = -bact->maxrot * v38;

                            if ( a2 > bact->maxrot * v38 )
                                a2 = bact->maxrot * v38;

                            sub_4A10E8(robo, a2);
                        }
                        else
                        {
                            ypaworld_arg148 arg148;
                            arg148.ownerID2 = bact->owner;
                            arg148.ownerID = bact->owner;
                            arg148.blg_ID = bldid;
                            arg148.x = xx;
                            arg148.y = yy;
                            arg148.field_C = 0;
                            arg148.field_18 = 0;

                            if ( call_method(robo->wrld, 148, &arg148) )
                            {
                                BuildProto *proto = &buildprotos[bldid];

                                if ( robo->field_509 >= proto->energy )
                                {
                                    robo->field_509 -= proto->energy;
                                }
                                else
                                {
                                    bact->energy -= proto->energy - robo->field_509;
                                    robo->field_509 = 0;
                                }

                                robo->field_2FD = 0;

                                if ( robo->field_1DF )
                                    robo->field_265 = robo->field_1E3 * (100 - robo->field_1E9) / 100;
                            }
                            else
                            {
                                bact_arg67 arg67;
                                arg67.field_0 = 0;
                                arg67.field_4 = 0;
                                call_method(bact->self, 67, &arg67);
                                robo->field_2FD = 0;
                            }
                        }
                    }
                    else
                    {
                        bact_arg67 arg67;
                        sub_4F4FF4(robo, robo->field_2F5, &arg67);
                        arg67.field_0 = 5;
                        call_method(bact->self, 67, &arg67);
                    }
                }
            }
        }
        else
        {
            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 0;
            call_method(bact->self, 67, &arg67);
            robo->field_2FD = 0;
        }
    }
}

int yparobo_func70__sub4__sub1__sub0(__NC_STACK_yparobo *robo, BuildProto *protos)
{
    int id = -1;
    int power = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = 0; i < 128; i++)
    {
        BuildProto *proto = &protos[i];

        if ( proto->enable_mask & (1 << bact->owner) )
        {
            if ( proto->model_id == 3 )
            {
                if ( (robo->field_509 + bact->energy - bact->energy_2 * 0.2) > proto->energy )
                {
                    int protoPow = proto->power;

                    for (int j = 0; j < 8; j++)
                    {
                        if ( proto->sbacts[j].sbact_vehicle )
                            protoPow += 10;
                    }

                    if ( protoPow > power )
                    {
                        power = protoPow;
                        id = i;
                    }
                }
            }
        }
    }

    return id;
}


void yparobo_func70__sub4__sub1(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    // Copy of sub4_sub0 with minor changes
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2F5 % bact->field_20;
    int yy = robo->field_2F5 / bact->field_20;

    float v35 = arg->field_4 * 0.001;

    if ( robo->pcell[ robo->field_2F5 ].field_3A )
    {
        bact_arg67 arg67;
        arg67.field_0 = 0;
        arg67.field_4 = 0;

        call_method(bact->self, 67, &arg67);
        robo->field_2FD = 0;
    }
    else
    {
        BuildProto *buildprotos;
        call_vtbl(robo->wrld, 3, 0x80002013, &buildprotos, 0);

        int build_id = yparobo_func70__sub4__sub1__sub0(robo, buildprotos);

        if ( build_id == -1 )
        {
            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 0;

            call_method(bact->self, 67, &arg67);
            robo->field_2FD = 0;
        }
        else
        {

            if ( abs(bact->field_c - xx) > 1 || abs(bact->field_10 - yy) > 1 )
            {
                bact_arg67 arg67;
                sub_4F4FF4(robo, robo->field_2F5, &arg67);

                arg67.field_0 = 5;

                call_method(bact->self, 67, &arg67);
            }
            else
            {

                if ( bact->field_c != xx || bact->field_10 != yy )
                {
                    bact_arg67 arg67;
                    arg67.field_0 = 0;
                    arg67.field_4 = 0;
                    call_method(bact->self, 67, &arg67);

                    float v30 = -(yy + 0.5) * 1200.0 - bact->field_621.sz;
                    float v29 = (xx + 0.5) * 1200.0 - bact->field_621.sx;

                    float v16 = 1.0 / sqrt(v30 * v30 + v29 * v29);

                    v29 *= v16;
                    v30 *= v16;

                    float v41 = v29 * bact->field_651.m20 + v30 * bact->field_651.m22;

                    if ( v41 <= 0.9 )
                    {
                        if ( v41 > 1.0 )
                            v41 = 1.0;
                        if ( v41 < -1.0 )
                            v41 = -1.0;

                        float a2 = acos(v41);

                        if ( v29 * bact->field_651.m22 - v30 * bact->field_651.m20 > 0.0 )
                            a2 = -a2;

                        if ( a2 < -bact->maxrot * v35 )
                            a2 = -bact->maxrot * v35;

                        if ( a2 > bact->maxrot * v35 )
                            a2 = bact->maxrot * v35;

                        sub_4A10E8(robo, a2);
                    }
                    else
                    {
                        ypaworld_arg148 arg148;

                        arg148.ownerID2 = bact->owner;
                        arg148.ownerID = bact->owner;
                        arg148.blg_ID = build_id;
                        arg148.x = xx;
                        arg148.y = yy;
                        arg148.field_C = 0;
                        arg148.field_18 = 0;

                        if ( call_method(robo->wrld, 148, &arg148) )
                        {
                            BuildProto *proto = &buildprotos[build_id];

                            if ( robo->field_509 >= proto->energy )
                            {
                                robo->field_509 -= proto->energy;
                            }
                            else
                            {
                                bact->energy -= proto->energy - robo->field_509;
                                robo->field_509 = 0;
                            }

                            robo->field_2FD = 0;

                            if ( robo->field_1DF )
                                robo->field_24D = robo->field_1E3 * (100 - robo->field_1EB) / 100;
                        }
                        else
                        {
                            bact_arg67 arg67;
                            arg67.field_0 = 0;
                            arg67.field_4 = 0;

                            call_method(bact->self, 67, &arg67);

                            robo->field_2FD = 0;
                        }
                    }
                }
                else
                {
                    bact_arg67 arg67;

                    sub_4F4FF4(robo, robo->field_2F5, &arg67);

                    arg67.field_0 = 5;

                    call_method(bact->self, 67, &arg67);
                }

            }

        }
    }
}

void yparobo_func70__sub4__sub3(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2F5 % bact->field_20;
    int yy = robo->field_2F5 / bact->field_20;

    if ( xx >= 1 && xx <= (bact->field_20 - 2) && yy >= 1 && yy <= (bact->field_22 - 2) )
    {
        int tmp1 = bact->field_10 - yy;
        int tmp2 = bact->field_c - xx;

        float v12 = sqrt(tmp1 * tmp1 + tmp2 * tmp2);

        if ( v12 >= 0.1 )
        {
            bact_arg67 arg67;
            arg67.field_0 = 5;
            arg67.field_4 = 0;
            arg67.targ.sx = (xx + 0.5) * 1200.0;
            arg67.targ.sz = -(yy + 0.5) * 1200.0;

            call_method(bact->self, 67, &arg67);
        }
        else
        {
            robo->field_241 = 0;
            robo->field_239 = 0;
            robo->field_245 = 0;
            robo->field_2A5 = robo->field_241;
            robo->field_229 = robo->field_241;
            robo->field_259 = robo->field_241;
            robo->field_29D = robo->field_239;
            robo->field_221 = robo->field_239;
            robo->field_251 = robo->field_239;
            robo->field_2A9 = robo->field_245;
            robo->field_22D = robo->field_245;
            robo->field_25D = robo->field_245;
            robo->field_2FD = 0;
            robo->field_1DB &= 0xFFFEC7FF;

            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 0;
            call_method(bact->self, 67, &arg67);

            if ( robo->field_1DF )
                robo->field_2B1 = robo->field_1E3 * (100 - robo->field_1ED) / 100;
        }
    }
    else
    {
        robo->field_2FD = 0;
    }
}


float sub_4AA480(__NC_STACK_yparobo *robo, void *src, int bact_ref, int job)
{
    VhclProto *vhcl_protos;
    WeapProto *weapon_protos;

    call_vtbl(robo->wrld, 3, 0x80002014, &vhcl_protos, 0);
    call_vtbl(robo->wrld, 3, 0x80002012, &weapon_protos, 0);

    VhclProto *proto;

    if ( bact_ref )
        proto = &vhcl_protos[ ((__NC_STACK_ypabact *)src)->id ];
    else
        proto = (VhclProto *)src;


    WeapProto *wpn;

    if ( proto->weapon == -1 )
        wpn = NULL;
    else
        wpn = &weapon_protos[ proto->weapon ];

    switch ( job )
    {
    case 1:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fightrobo;
        break;

    case 2:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fighttank;
        break;

    case 4:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fighthelicopter;
        break;

    case 3:
        if ( (proto->mgun == -1 && !wpn) || proto->model_id == 7 )
            return -1.0;

        return proto->job_fightflyer;
        break;

    case 5:
        return proto->job_reconnoitre;
        break;

    case 6:
        if ( !wpn || proto->model_id == 7 )
            return -1.0;

        return proto->job_conquer;
        break;

    default:
        break;
    }
    return -1.0;
}

bact_node *sub_4AA640(__NC_STACK_yparobo *robo, robo_loct1 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v72 = 0;
    float v73 = 0.0;

    int v85 = -1;
    int v86 = -1;
    int v87 = -1;

    int v75 = 10000000;
    int v2 = 0;
    int v77 = 0;
    float v82 = -2.0;
    float v76 = 153600.0;
    bact_node *v74 = NULL;

    bact_node *node = (bact_node *)bact->list2.head;

    while ( node->next )
    {
        if ( node->bact->field_3D5 != 3 && !( node->bact->field_3D6 & 0x1000 ) && !( node->bact->field_3D6 & 0x80000 ) && !( node->bact->field_3D6 & 0x4000 ) &&  node->bact->field_24 != 9 )
        {
            v2++;

            float v8 = sub_4AA480(robo, node->bact, 1, arg->field_34);

            if ( abs(node->bact->field_c - bact->field_c) < 2 && abs(node->bact->field_10 - bact->field_10) < 2 )
                v77 = 1;

            if ( v8 > -1.0 && v8 >= v82 )
            {

                bact_arg81 arg81;
                arg81.enrg_type = 1;
                arg81.enrg_sum = 0;
                call_method(node->bacto, 81, &arg81);

                int v11 = abs(arg81.enrg_sum - arg->field_0);
                int v15 = 0;

                int v16;

                if ( v11 )
                    v16 = arg->field_0 / v11;
                else
                    v16 = 6;

                float v13 = node->bact->field_621.sx - arg->field_10.sx;
                float v14 = node->bact->field_621.sz - arg->field_10.sz;
                float v78 = v13 * v13 + v14 * v14;

                if ( v16 <= 5 )
                {
                    if ( v11 < v75 )
                    {
                        v15 = 1;
                    }
                }
                else if ( v78 < v76 )
                {
                    v15 = 1;
                }

                if ( v8 > v82 || (v8 == v82 && v15) )
                {
                    v75 = v11;
                    v76 = v78;
                    v74 = node;
                    v82 = v8;
                }

            }
        }
        node = (bact_node *)node->next;
    }

    if ( !(robo->field_1DB & 4) )
    {
        bact_arg81 arg81_2;
        arg81_2.enrg_sum = 0;
        arg81_2.enrg_type = 3;

        call_method(bact->self, 81, &arg81_2);

        v85 = -1;
        v86 = -1;
        v87 = -1;

        v73 = arg81_2.enrg_sum * 0.05 + 1.0;

        VhclProto *vhcl_protos;
        call_vtbl(robo->wrld, 3, 0x80002014, &vhcl_protos, 0);

        for (int i = 0; i < 256; i++)
        {
            if ( vhcl_protos[i].model_id )
            {
                float v20 = bact->energy_2 * 0.2;
                int v67 = bact->energy - v20;

                if ( robo->field_1DB & 0x800000 )
                    v67 += robo->field_50D;

                VhclProto *vhclprt = &vhcl_protos[i];
                if ( (1 << bact->owner) & vhclprt->disable_enable_bitmask )
                {
                    if ( vhclprt->model_id != 3 && vhclprt->model_id != 9 && vhclprt->model_id != 4 )
                    {
                        if ( v67 > ((float)vhclprt->energy * v73) )
                        {
                            float v80 = sub_4AA480(robo, vhclprt, 0, arg->field_34);

                            int v27 = 0;
                            if ( v2 < (robo->bact_internal->field_20 + robo->bact_internal->field_22) / 5 )
                                v27 = 1;

                            if ( bact->energy < ((float)bact->energy_2 * 0.9) )
                                v27 = 0;

                            if ( robo->field_1DB & 4 )
                                v27 = 0;

                            if ( v77 )
                                v27 = 0;

                            if ( v80 > -1.0 && (v80 > v82 || (v80 == v82 && v27)) )
                            {
                                v82 = v80;
                                v87 = v86;
                                v86 = v85;
                                v85 = i;
                            }
                        }
                    }
                }
            }
        }
    }

    //there must be switch, but it's originally deleted
    //arg->field_34;

    if ( v85 == -1 && !v74 )
        return NULL;

    if ( v85 == -1 )
    {
        if ( !(robo->field_1DB & 4) )
        {
            bact_arg81 arg81_3;
            arg81_3.enrg_type = 1;
            arg81_3.enrg_sum = 0;

            call_method(v74->bacto, 81, &arg81_3);

            if ( arg81_3.enrg_sum < arg->field_0 )
            {
                robo->field_1F7 = v74->bact->field_2E;
                robo->field_1EF = arg->field_0 - arg81_3.enrg_sum;
                v72 = 1;
            }
        }
    }
    else
    {
        int v32 = robo->bact_internal->field_915 % 10;
        int v33;

        if ( v32 <= 8 || v87 == -1 )
        {
            if ( v32 <= 5 || v86 == -1 )
                v33 = v85;
            else
                v33 = v86;
        }
        else
        {
            v33 = v87;
        }

        ypaworld_arg146 arg146;
        arg146.vehicle_id = v33;
        arg146.pos.sx = robo->bact_internal->field_621.sx + robo->dock.sx;
        arg146.pos.sy = robo->bact_internal->field_621.sy + robo->dock.sy;
        arg146.pos.sz = robo->bact_internal->field_621.sz + robo->dock.sz;

        NC_STACK_ypabact *new_unit = (NC_STACK_ypabact *)call_method(robo->wrld, 146, &arg146);

        if ( !new_unit )
            return NULL;

        __NC_STACK_ypabact *unt_bct;

        call_vtbl(new_unit, 3, 0x80001003, &unt_bct, 0);

        unt_bct->field_2E = dword_5B1128;
        dword_5B1128++;

        unt_bct->owner = robo->bact_internal->owner;
        unt_bct->field_32 = robo->bact_internal->self;

        int v69;
        call_vtbl(bact->self, 3, 0x80001007, &v69, 0);
        call_vtbl(new_unit, 2, 0x80001007, v69, 0);

        call_method(bact->self, 72, new_unit);
        if ( robo->wrld_yw->field_757E )
        {
            unt_bct->ypabact__id |= unt_bct->owner << 24;
            unt_bct->field_2E |= unt_bct->owner << 24;

            char a1a[44];
            /*
            *(_DWORD *)a1a = 1001;
            a1a[12] = unt_bct->owner;
            *(float *)&a1a[16] = unt_bct->field_621.sx;
            *(float *)&a1a[20] = unt_bct->field_621.sy;
            *(float *)&a1a[24] = unt_bct->field_621.sz;
            a1a[41] = unt_bct->id;
            *(_DWORD *)&a1a[28] = unt_bct->parent_bact->ypabact__id;
            *(_DWORD *)&a1a[32] = unt_bct->ypabact__id;
            a1a[40] = 1;
            *(_DWORD *)&a1a[36] = unt_bct->field_2E;*/

            yw_arg181 arg181;
            arg181.field_10 = 0;
            arg181.field_14 = 2;
            arg181.value = a1a;
            arg181.val_size = 44;
            arg181.field_18 = 1;

            call_method(robo->wrld, 181, &arg181);
        }

        robo->field_1DB |= 4;

        call_vtbl(new_unit, 2, 0x8000100D, 60, 0);

        robo->field_1F7 = unt_bct->field_2E;
        robo->field_1EF = arg->field_0 - unt_bct->energy;

        unt_bct->field_931 = (float)unt_bct->energy_2 * 0.2;

        robo->field_1FB = unt_bct->field_931 + 2000;

        unt_bct->field_3D6 &= 0xFFFFBFFF;

        bact_node *nnode = (bact_node *)unt_bct->list2.head;
        while ( nnode->next )
        {
            nnode->bact->field_3D6 &= 0xFFFFBFFF;

            nnode = (bact_node *)nnode->next;
        }

        bact_arg119 arg78;
        arg78.field_0 = 4;
        arg78.field_4 = 0;
        arg78.field_8 = 0;

        call_method(new_unit, 78, &arg78);


        bact_arg67 arg67;
        arg67.field_0 = 1;
        arg67.field_4 = 0;
        arg67.targ.sx = bact->field_651.m20 * 1200.0 * 0.5 + bact->field_621.sx;
        arg67.targ.sz = bact->field_651.m22 * 1200.0 * 0.5 + bact->field_621.sz;

        call_method(new_unit, 67, &arg67);

        int v70 = (float)unt_bct->energy_2 * v73;

        if ( robo->field_1DB & 0x800000 )
        {
            if ( v70 > robo->field_50D )
            {
                bact->energy -= v70 - robo->field_50D;
                robo->field_50D = 0;
            }
            else
            {
                robo->field_50D -= v70;
            }
        }
        else
        {
            bact->energy -= v70;
        }

        v74 = &unt_bct->list_node;
        v72 = 1;
        sub_4A6720(robo->wrld, unt_bct);
    }

    if ( v72 )
    {
        robo->field_1DB |= (8 | 4);
        robo->field_20B = arg->field_1C;
        robo->field_20F = arg->field_C;
        robo->field_211 = arg->field_20;

        robo->field_1FF = arg->field_10;
        robo->field_210 = arg->field_8;

        if ( arg->field_C == 2 )
            robo->field_1FF = arg->field_1C->field_621;
    }
    else
    {
        bact_arg67 arg67_2;
        arg67_2.targ = arg->field_10;
        arg67_2.field_4 = 0;
        arg67_2.field_8 = arg->field_1C;
        arg67_2.field_0 = arg->field_C;

        call_method(v74->bacto, 67, &arg67_2);

        call_vtbl(v74->bacto, 2, 0x8000100D, arg->field_8, 0);
    }

    return v74;
}



void yparobo_func70__sub4__sub4(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_arg92 arg92;
    arg92.field_14 = 2;
    arg92.pos.sx = ((robo->field_2E5 % bact->field_20) + 0.5) * 1200.0;
    arg92.pos.sz = -( (robo->field_2E5 / bact->field_20 ) + 0.5) * 1200.0;

    call_method(bact->self, 92, &arg92);

    int v20 = (arg92.energ2 - arg92.energ1) * 0.5;

    if ( v20 < 15000 )
        v20 = 15000;

    yw_130arg arg130;
    arg130.pos_x = arg92.pos.sx;
    arg130.pos_z = arg92.pos.sz;

    call_method(robo->wrld, 130,  &arg130);

    int enrg = 0;

    __NC_STACK_ypabact *cell_unit = (__NC_STACK_ypabact *)arg130.pcell->field_3C.head;

    while (cell_unit->next)
    {
        if ( bact->owner != cell_unit->owner && cell_unit->field_24 == 9 && (cell_unit->weapon != -1 || cell_unit->mgun != -1) )
        {
            int a4;
            call_vtbl(cell_unit->self, 3, 0x80002006, &a4, 0);

            if ( !a4 )
            {
                bact_node *bctt = (bact_node *)robo->wrld_yw->bact_list.head;

                while (bctt->next)
                {
                    if ( bctt->bact->field_24 == 3 && bctt->bact->field_3D5 != 2 && cell_unit->owner == bctt->bact->owner )
                    {
                        int v6 = bctt->bact->energy_2 / 2;

                        if ( v6 < 120000 )
                            v6 = 120000;

                        enrg = v6;
                        break;
                    }

                    bctt = (bact_node *)bctt->next;
                }
            }
        }

        if ( enrg )
            break;

        cell_unit = (__NC_STACK_ypabact *)cell_unit->next;
    }

    if ( enrg > v20 )
        v20 = enrg;

    ypaworld_arg136 arg136;
    arg136.pos_x = bact->field_621.sx + robo->dock.sx;
    arg136.pos_y = bact->field_621.sy + robo->dock.sy - 100.0;
    arg136.pos_z = bact->field_621.sz + robo->dock.sz;
    arg136.field_1C = 0;
    arg136.field_14 = 0;
    arg136.field_18 = 20000.0;
    arg136.field_40 = 0;

    call_method(robo->wrld, 136, &arg136);

    if ( bact->p_cell_area->sector_height_meters - arg136.field_30 >= 50.0 )
    {
        bact_arg67 arg67;
        arg67.targ.sx = bact->field_621.sx + 200.0;
        arg67.targ.sz = bact->field_621.sz + 300.0;
        arg67.field_4 = 0;
        arg67.field_0 = 5;

        call_method(bact->self, 67, &arg67);
    }
    else
    {
        float v9  = arg92.pos.sx - bact->field_621.sx;
        float v10 = arg92.pos.sz - bact->field_621.sz;

        robo_loct1 loct;

        loct.field_4 = sqrt(v9 * v9 + v10 * v10);
        loct.field_10.sx = arg92.pos.sx;
        loct.field_10.sz = arg92.pos.sz;
        loct.field_10.sy = 0;

        loct.field_1C = NULL;

        if ( v20 <= 20000 )
            loct.field_0 = 20000;
        else
            loct.field_0 = v20;

        loct.field_C = 1;

        if ( robo->field_2E9->field_3A == 2 )
            loct.field_8 = 49;
        else
            loct.field_8 = 60;

        loct.field_24 = 1;
        loct.field_28 = 32;
        loct.field_2C = 130;
        loct.field_30 = 0;
        loct.field_34 = 6;

        sub_4AA640(robo, &loct);

        if ( robo->field_1DF )
            robo->field_299 = robo->field_1E3 * (100 - robo->field_1E7) / 100;

        robo->field_2ED = 0;
    }
}

void yparobo_func70__sub4__sub5(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_arg67 arg132;
    arg132.field_4 = robo->field_2F1;

    if ( call_method(bact->self, 132, &arg132) )
    {
        bact_arg81 arg81;
        arg81.enrg_sum = 0;
        arg81.enrg_type = 1;

        call_method(arg132.field_8->self, 81, &arg81);

        int v18 = arg81.enrg_sum / 2;

        if ( v18 < 15000 )
            v18 = 15000;

        if ( arg132.field_8->field_24 == 9 && (arg132.field_8->weapon != -1 || arg132.field_8->mgun != -1) )
        {
            int a4;
            call_vtbl(arg132.field_8->self, 3, 0x80002006, &a4, 0);
            if ( !a4 )
            {
                bact_node *node = (bact_node *)robo->wrld_yw->bact_list.head;
                while( node->next )
                {
                    if ( node->bact->field_24 == 3 && node->bact->field_3D5 != 2 && arg132.field_8->owner == node->bact->owner )
                    {
                        int v4 = node->bact->energy_2 / 2;

                        if ( v4 < 120000 )
                            v4 = 120000;

                        v18 = v4;
                        break;
                    }

                    node = (bact_node *)node->next;
                }
            }
        }

        ypaworld_arg136 arg136;
        arg136.pos_x = bact->field_621.sx + robo->dock.sx;
        arg136.pos_y = bact->field_621.sy + robo->dock.sy - 100.0;
        arg136.pos_z = bact->field_621.sz + robo->dock.sz;
        arg136.field_1C = 0;
        arg136.field_14 = 0;
        arg136.field_18 = 20000.0;
        arg136.field_40 = 0;

        call_method(robo->wrld, 136, &arg136);

        if ( bact->p_cell_area->sector_height_meters - arg136.field_30 >= 50.0 )
        {
            arg132.targ.sx = bact->field_621.sx + 200.0;
            arg132.targ.sz = bact->field_621.sz + 300.0;
            arg132.field_4 = 0;
            arg132.field_0 = 5;

            call_method(bact->self, 67, &arg132);
        }
        else
        {
            float v7 = arg132.field_8->field_621.sx - bact->field_621.sx;
            float v8 = arg132.field_8->field_621.sz - bact->field_621.sz;

            robo_loct1 loct;
            loct.field_4 = sqrt(v7 * v7 + v8 * v8);
            loct.field_10.sx = arg132.field_8->field_621.sx;
            loct.field_10.sy = arg132.field_8->field_621.sy;
            loct.field_10.sz = arg132.field_8->field_621.sz;

            int v9 = v18;
            if ( v9 >= 40000 )
                v9 = 40000;

            loct.field_0 = v9;
            loct.field_C = 2;
            loct.field_1C = arg132.field_8;
            loct.field_20 = robo->field_2F1;
            loct.field_8 = 60;

            if ( arg132.field_8->field_24 != 2 && arg132.field_8->field_24 != 8 && arg132.field_8->field_24 != 10 )
            {
                loct.field_30 = 2;
                loct.field_28 = 32;
                loct.field_2C = 852;
                loct.field_24 = 129;
            }
            else
            {
                loct.field_28 = 32;
                loct.field_2C = 578;
                loct.field_30 = 256;
                loct.field_24 = 1;
            }

            switch ( arg132.field_8->field_24 )
            {
            case 2:
            case 8:
            case 10:
                loct.field_34 = 2;
                break;

            case 6:
            case 7:
                loct.field_34 = 3;
                break;

            default:
                loct.field_34 = 4;
                break;
            }

            sub_4AA640(robo, &loct);

            if ( robo->field_1DF )
                robo->field_281 = robo->field_1E3 * (100 - robo->field_1EA) / 100;

            robo->field_2ED = 0;
        }
    }
    else
    {
        robo->field_2ED = 0;
    }
}

void yparobo_func70__sub4__sub6(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_arg67 arg132;
    arg132.field_4 = robo->field_2F1;
    if ( call_method(bact->self, 132, &arg132) )
    {
        ypaworld_arg136 arg136;
        arg136.pos_x = bact->field_621.sx + robo->dock.sx;
        arg136.pos_y = bact->field_621.sy + robo->dock.sy - 100.0;
        arg136.pos_z = bact->field_621.sz + robo->dock.sz;
        arg136.field_1C = 0;
        arg136.field_14 = 0;
        arg136.field_18 = 20000.0;
        arg136.field_40 = 0;

        call_method(robo->wrld, 136, &arg136);

        if ( bact->p_cell_area->sector_height_meters - arg136.field_30 >= 50.0 )
        {
            arg132.targ.sx = bact->field_621.sx + 200.0;
            arg132.targ.sz = bact->field_621.sz + 300.0;
            arg132.field_0 = 5;
            arg132.field_4 = 0;
            call_method(bact->self, 67, &arg132);
        }
        else
        {
            float v4 = arg132.field_8->field_621.sx - bact->field_621.sx;
            float v5 = arg132.field_8->field_621.sz - bact->field_621.sz;

            robo_loct1 loct;
            loct.field_4 = sqrt(v4 * v4 + v5 * v5);
            loct.field_10.sx = arg132.field_8->field_621.sx;
            loct.field_10.sy = arg132.field_8->field_621.sy;
            loct.field_10.sz = arg132.field_8->field_621.sz;

            int v6 = arg132.field_8->energy_2 / 2;
            if ( v6 < 120000 )
                v6 = 120000;

            loct.field_0 = v6;
            loct.field_C = 2;
            loct.field_1C = arg132.field_8;
            loct.field_20 = robo->field_2F1;
            loct.field_8 = 60;
            loct.field_28 = 32;
            loct.field_2C = 852;
            loct.field_30 = 2;
            loct.field_24 = 1;
            loct.field_34 = 1;
            robo->field_1DB |= 0x800000;

            bact_node *nod = sub_4AA640(robo, &loct);
            if ( nod )
            {
                for (int i = 0; i < 16; i++)
                {
                    if ( !robo->field_525[i].field_0 )
                    {
                        robo->field_525[i].field_0 = nod->bact->field_2E;
                        robo->field_525[i].field_4 = arg132.field_8->field_2E;
                        break;
                    }
                }
            }

            if ( robo->field_1DF )
                robo->field_2E1 = robo->field_1E3 * (100 - robo->field_1EE) / 100;

            robo->field_2ED = 0;
        }
    }
    else
    {
        robo->field_2ED = 0;
    }
}

void yparobo_func70__sub4__sub7(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    ypaworld_arg136 arg136;
    arg136.pos_x = bact->field_621.sx + robo->dock.sx;
    arg136.pos_y = bact->field_621.sy + robo->dock.sy + -100.0;
    arg136.pos_z = bact->field_621.sz + robo->dock.sz;
    arg136.field_1C = 0;
    arg136.field_14 = 0;
    arg136.field_18 = 20000.0;
    arg136.field_40 = 0;

    call_method(robo->wrld, 136, &arg136);

    if ( bact->p_cell_area->sector_height_meters - arg136.field_30 >= 50.0 )
    {
        bact_arg67 arg67;
        arg67.targ.sx = bact->field_621.sx + 200.0;
        arg67.targ.sz = bact->field_621.sz + 300.0;
        arg67.field_0 = 5;
        arg67.field_4 = 0;

        call_method(bact->self, 67, &arg67);
    }
    else
    {
        robo_loct1 loct;
        loct.field_10.sx = ((robo->field_2E5 % bact->field_20) + 0.5) * 1200.0;
        loct.field_10.sy = 0;
        loct.field_10.sz = -((robo->field_2E5 / bact->field_20) + 0.5) * 1200.0;

        float v5 = loct.field_10.sx - bact->field_621.sx;
        float v6 = loct.field_10.sz - bact->field_621.sz;

        loct.field_0 = 10;
        loct.field_8 = 25;
        loct.field_24 = 0;
        loct.field_28 = 1024;
        loct.field_2C = 9;
        loct.field_34 = 5;
        loct.field_C = 1;
        loct.field_30 = 32;
        loct.field_4 = sqrt(v5 * v5 + v6 * v6);
        loct.field_1C = NULL;

        bact_node *nod = sub_4AA640(robo, &loct);

        if ( nod )
            nod->bact->field_3D4 = 25;

        if ( robo->field_1DF )
            robo->field_2C9 = robo->field_1E3 * (100 - robo->field_1ec) / 100;

        robo->field_2ED = 0;
    }
}


void yparobo_func70__sub4(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    if ( !robo->field_2FD )
    {
        int v7 = 0;
        int v2 = 0;

        if ( robo->field_1DB & 0x1000 )
        {
            if ( robo->field_251 )
            {
                if ( -robo->field_265 >= 0 && robo->field_265 != 0 )
                {
                    v2 = 0x20;
                    v7 = -robo->field_265;
                }
            }
        }
        if ( robo->field_1DB & 0x800 )
        {
            if ( robo->field_221 )
            {
                if ( -robo->field_235 > v7 )
                {
                    v2 = 0x10;
                    v7 = -robo->field_235;
                }
            }
        }
        if ( robo->field_1DB & 0x2000 )
        {
            if ( robo->field_239 )
            {
                if ( -robo->field_24D > v7 )
                {
                    v2 = 0x100;
                    v7 = -robo->field_24D;
                }
            }
        }
        if ( robo->field_1DB & 0x10000 )
        {
            if ( robo->field_29D )
            {
                if ( -robo->field_2B1 > v7 )
                {
                    v2 = 0x20000;
                    v7 = -robo->field_2B1;
                }
            }
        }

        if ( v7 > 0 )
        {
            if ( v2 == 0x10 )
            {
                robo->field_221 = 0;
                robo->field_22D = 0;
                robo->field_2FD = 0x10;
                robo->field_2F5 = robo->field_229;
                robo->field_2F9 = robo->field_225;
                robo->field_1DB &= 0xFFFFF7FF;
            }
            else if ( v2 == 0x20 )
            {
                robo->field_251 = 0;
                robo->field_25D = 0;
                robo->field_2FD = 0x20;
                robo->field_2F5 = robo->field_259;
                robo->field_1DB &= 0xFFFFEFFF;
                robo->field_2F9 = robo->field_255;
            }
            else if ( v2 == 0x100 )
            {
                robo->field_239 = 0;
                robo->field_245 = 0;
                robo->field_2F5 = robo->field_241;
                robo->field_2F9 = robo->field_23D;
                robo->field_2FD = 0x100;
                robo->field_1DB &= 0xFFFFDFFF;
            }
            else if ( v2 == 0x20000 )
            {
                robo->field_29D = 0;
                robo->field_2A9 = 0;
                robo->field_2F5 = robo->field_2A5;
                robo->field_2F9 = robo->field_2A1;
                robo->field_2FD = 0x20000;
                robo->field_1DB &= 0xFFFEFFFF;
            }
        }
    }

    int v14 = robo->field_2ED;
    int v3;

    if ( !robo->field_2ED && !(robo->field_1DB & 4) )
    {
        if ( robo->field_1DB & 0x200 )
        {
            if ( robo->field_269 )
            {
                if ( -robo->field_281 >= 0 && robo->field_281 != 0 )
                {
                    v3 = 0x80;
                    v14 = -robo->field_281;
                }
            }
        }

        if ( robo->field_1DB & 0x400 )
        {
            if ( robo->field_285 )
            {
                if ( -robo->field_299 > v14 )
                {
                    v3 = 0x40;
                    v14 = -robo->field_299;
                }
            }
        }

        if ( robo->field_1DB & 0x40000 )
        {
            if ( robo->field_2B5 )
            {
                if ( -robo->field_2C9 > v14 )
                {
                    v3 = 0x80000;
                    v14 = -robo->field_2C9;
                }
            }
        }
        if ( robo->field_1DB & 0x100000 )
        {
            if ( robo->field_2CD )
            {
                if ( -robo->field_2E1 > v14 )
                {
                    v3 = 0x200000;
                    v14 = -robo->field_2E1;
                }
            }
        }

        if ( v14 > 0 )
        {
            if ( v3 == 0x40 )
            {
                robo->field_285 = 0;
                robo->field_291 = 0;
                robo->field_2E5 = robo->field_28D;
                robo->field_2E9 = robo->field_289;
                robo->field_2ED = 0x40;
                robo->field_1DB &= 0xFFFFFBFF;
            }
            else if ( v3 == 0x80 )
            {
                robo->field_2ED = 0x80;
                robo->field_269 = 0;
                robo->field_2F1 = robo->field_275;
                robo->field_1DB &= 0xFFFFFDFF;
                robo->field_2E5 = robo->field_271;
                robo->field_2E9 = robo->field_26D;
            }
            else if ( v3 == 0x80000 )
            {
                robo->field_2ED = 0x80000;
                robo->field_2B5 = 0;
                robo->field_2C1 = 0;
                robo->field_2E5 = robo->field_2BD;
                robo->field_2E9 = robo->field_2B9;
                robo->field_1DB &= 0xFFFBFFFF;
            }
            else if ( v3 == 0x200000 )
            {
                robo->field_2CD = 0;
                robo->field_2F1 = robo->field_2D9;
                robo->field_2E5 = robo->field_2D5;
                robo->field_2E9 = robo->field_2D1;
                robo->field_2ED = 0x200000;
                robo->field_1DB &= 0xFFEFFFFF;
            }
        }
    }

    switch (robo->field_2FD)
    {
    case 0x10:
        yparobo_func70__sub4__sub0(robo, arg);
        break;

    case 0x20:
        yparobo_func70__sub4__sub2(robo, arg);
        break;

    case 0x100:
        yparobo_func70__sub4__sub1(robo, arg);
        break;

    case 0x20000:
        yparobo_func70__sub4__sub3(robo);
        break;

    default:
        break;
    }

    switch (robo->field_2ED)
    {
    case 0x40:
        yparobo_func70__sub4__sub4(robo);
        break;

    case 0x80:
        yparobo_func70__sub4__sub5(robo);
        break;

    case 0x80000:
        yparobo_func70__sub4__sub7(robo);
        break;

    case 0x200000:
        yparobo_func70__sub4__sub6(robo);
        break;

    default:
        break;
    }
}

void yparobo_func70__sub3(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    if ( robo->field_1F7 )
    {
        int v4 = 0;
        bact_node *node = (bact_node *)robo->bact_internal->list2.head;

        while( node->next )
        {
            if (robo->field_1E7 == node->bact->field_2E)
            {
                v4 = 1;
                break;
            }

            node = (bact_node *)node->next;
        }

        if ( !v4 )
        {
            robo->field_1FB = 0;
            robo->field_1F7 = 0;
            robo->field_1DB &= 0xFFFFFFFB;
        }
    }

    if ( robo->field_1FB > 0 )
    {
        robo->field_1FB -= arg->field_4;
        if ( robo->field_1FB <= 0 )
        {
            robo->field_1FB = 0;

            if ( !robo->field_1F7 )
                robo->field_1DB &= 0xFFFFFFFB;
        }
    }

    if ( robo->field_1FB <= 0 && !robo->field_1F7 )
        robo->field_1DB &= 0xFFFFFFFB;
}

void yparobo_func70__sub0(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( (float)bact->energy / (float)bact->energy_2 < 0.2 && (arg->field_0 - robo->field_309) > 20000 )
    {
        robo->field_309 = arg->field_0;

        int v3 = (float)bact->energy / (float)bact->energy_2 < 0.1;

        bact_node *unit = (bact_node *)bact->list2.head;

        while ( unit->next )
        {
            if ( unit->bact->field_3D5 == 1 || unit->bact->field_3D5 == 3 )
            {
                if ( unit->bact->field_24 != 9 )
                {
                    int v7;

                    if ( v3 )
                    {
                        v7 = 1;
                        unit->bact->field_3D6 &= 0xFFF7FFFF;
                    }
                    else
                    {
                        v7 = !(unit->bact->field_3D6 & 0x80000) && !unit->bact->field_3DF;
                    }

                    float v10 = unit->bact->field_621.sx - bact->field_621.sx;
                    float v11 = unit->bact->field_621.sz - bact->field_621.sz;

                    if ( (v10 * v10 + v11 * v11) < 4320000.0 && unit->bact->field_3DE == 2 && bact == unit->bact->field_3e8 )
                    {
                        if ( v3 )
                        {
                            call_vtbl(unit->bacto, 2, 0x8000100D, 99, 0);
                            unit->bact->field_3D6 &= 0xFFFFBFFF;
                        }
                        else
                        {
                            call_vtbl(unit->bacto, 2, 0x8000100D, 60, 0);
                        }
                    }

                    if ( v7 )
                    {
                        bact_arg67 arg67;

                        arg67.field_0 = 2;
                        arg67.field_4 = 0;
                        arg67.field_8 = robo->bact_internal;

                        call_method(unit->bacto, 67, &arg67);
                    }
                }
            }

            unit = (bact_node *)unit->next;
        }
    }
}

void yparobo_func70__sub5(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( bact->field_915 - robo->field_5A9 > 4000 )
    {
        for (int i = 0; i < 16; i++)
        {
            if ( robo->field_525[i].field_0 )
            {
                int v6 = 0;
                bact_node *node = (bact_node *)bact->list2.head;

                while (node->next)
                {
                    if (node->bact->field_2E == robo->field_525[i].field_0)
                    {
                        v6 = 1;
                        break;
                    }

                    node = (bact_node *)node->next;
                }

                if ( !v6 )
                {
                    robo->field_525[i].field_0 = 0;
                    robo->field_525[i].field_4 = 0;
                }
            }
        }
    }
}

void sub_4A42B8(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_node *node = (bact_node *)robo->wrld->stack__ypaworld.bact_list.head;

    while ( node->next )
    {
        __NC_STACK_ypabact *unit = node->bact;

        if ( unit->field_3D5 != 2 )
        {
            if ( unit != bact && unit->field_24 == 3 )
            {
                if ( (1 << bact->owner) & unit->p_cell_area->field_39 )
                {
                    bact_node *kids = (bact_node *)robo->bact_internal->list2.head;

                    __NC_STACK_ypabact *sel = 0;
                    float selfl = 0.0;

                    while ( kids->next )
                    {
                        __NC_STACK_ypabact *kbct = kids->bact;

                        if (kbct->field_3D5 != 2)
                        {
                            float xx = kbct->field_621.sx - unit->field_621.sx;
                            float zz = kbct->field_621.sz - unit->field_621.sz;
                            float v14 = xx * xx + zz * zz;

                            if ( !sel || v14 < selfl )
                            {
                                sel = kids->bact;
                                selfl = v14;
                            }
                        }
                        kids = (bact_node *)kids->next;
                    }

                    if ( sel )
                    {
                        robo_arg134 v12;

                        v12.field_4 = 6;
                        v12.field_8 = node->bact->owner;
                        v12.field_14 = 48;
                        v12.unit = sel;
                        v12.field_C = 0;
                        v12.field_10 = 0;

                        call_method(bact->self, 134, &v12);
                    }
                }
            }
        }

        node = (bact_node *)node->next;
    }
}

void sub_4A43E8(__NC_STACK_yparobo *robo)
{
    float v1 = (float)robo->bact_internal->energy / (float)robo->bact_internal->energy_2;

    if ( v1 < 0.2 )
    {
        if ( v1 < 0.07 )
            v1 = 0.01;

        robo_arg134 v2;
        v2.field_4 = 40;
        v2.field_C = 0;
        v2.field_10 = 0;
        v2.field_14 = 98;
        v2.field_8 = (v1 * 60000.0 * 5.0);
        v2.unit = robo->bact_internal;

        call_method(v2.unit->self, 134, &v2);
    }
}

void sb_0x4a7010(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    _NC_STACK_ypaworld *yw = &robo->wrld->stack__ypaworld;

    NC_STACK_ypabact *wee_bact;
    call_vtbl(robo->wrld, 3, 0x80002010, &wee_bact, 0);

    int its_me =  (wee_bact == bact->self);

    float v31;

    bact_node *units = (bact_node *)bact->list2.head;
    while (units->next)
    {
        __NC_STACK_ypabact * unit_bact = units->bact;

        if ( unit_bact->field_3D5 != 2 )
        {
            if ( robo->field_1F7 == unit_bact->field_2E && robo->field_1F7 )
            {
                sb_0x4a7010__sub0(robo, units);
            }
            else
            {
                if ( unit_bact->field_24 != 9 )
                {
                    if ( !(unit_bact->field_3D6 & 0x4000) )
                    {
                        if ( call_method(units->bacto, 108, &v31) )
                        {
                            if ( v31 < 0.5 && its_me )
                            {
                                bact_arg81 arg81;
                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 3;

                                call_method(units->bacto, 81, &arg81);

                                int v18 = arg81.enrg_sum;

                                arg81.enrg_sum = 0;
                                arg81.enrg_type = 5;
                                call_method(units->bacto, 81, &arg81);

                                if ( v18 < arg81.enrg_sum )
                                {
                                    robo_arg134 arg134;
                                    arg134.field_8 = 20000;
                                    arg134.field_10 = 0;
                                    arg134.field_C = 0;
                                    arg134.field_4 = 18;
                                    arg134.unit = unit_bact;
                                    arg134.field_14 = 42;

                                    call_method(bact->self, 134, &arg134);
                                }
                            }

                            if ( !its_me )
                            {
                                if ( bact->field_915 - robo->field_5A5 > 1000 )
                                {
                                    robo->field_5A5 = bact->field_915;

                                    for (int i = 0; i < 16; i++)
                                    {
                                        if ( robo->field_525[i].field_0 == unit_bact->field_2E)
                                        {
                                            sb_0x4a7010__sub1(units, &robo->field_525[i]);
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            unit_bact->field_3D6 |= 0x4000;

                            bact_node *nod = (bact_node *)unit_bact->list2.head;

                            while (nod->next)
                            {
                                nod->bact->field_3D6 |= 0x4000;

                                nod = (bact_node *)nod->next;
                            }

                            unit_bact->field_3D6 &= 0xF3FFFFFF;
                            unit_bact->field_59A = 0;
                            unit_bact->field_3D6 &= 0xFFF7FFFF;

                            nod = (bact_node *)unit_bact->list2.head;

                            while (nod->next)
                            {
                                nod->bact->field_3D6 &= 0xFFF7FFFF;

                                nod = (bact_node *)nod->next;
                            }

                            if ( sub_4A587C(unit_bact) )
                            {
                                bact_arg124 arg124;
                                arg124.field_10 = 32;
                                arg124.field_0 = unit_bact->field_621.sx;
                                arg124.field_4 = unit_bact->field_621.sz;
                                arg124.field_8 = bact->field_621.sx;
                                arg124.field_C = bact->field_621.sz;
                                arg124.field_12 = 1;

                                if ( ! call_method(units->bacto, 124, &arg124) )
                                    break;

                                if ( arg124.field_10 > 1 )
                                {
                                    arg124.field_10 = 32;
                                    call_method(units->bacto, 125, &arg124);

                                    unit_bact->field_59c = bact->field_2E;
                                    unit_bact->field_5a0 = bact->owner;
                                }
                                else
                                {
                                    bact_arg67 arg67;
                                    arg67.field_8 = bact;
                                    arg67.field_4 = 0;
                                    arg67.field_0 = 2;
                                    call_method(units->bacto, 67, &arg67);
                                }
                            }
                            else
                            {
                                bact_arg67 arg67;
                                arg67.field_8 = bact;
                                arg67.field_0 = 2;
                                arg67.field_4 = 0;
                                call_method(units->bacto, 67, &arg67);
                            }

                            if ( its_me )
                            {
                                robo_arg134 arg134;
                                arg134.field_8 = unit_bact->field_2E;
                                arg134.field_10 = 0;
                                arg134.field_C = 0;
                                arg134.field_4 = 9;
                                arg134.unit = unit_bact;
                                arg134.field_14 = 40;

                                call_method(bact->self, 134, &arg134);

                                if ( robo->wrld->stack__ypaworld.field_757E  )
                                {
                                    bact_arg90 arg90;
                                    arg90.unit = unit_bact;
                                    arg90.field_8 = 1;
                                    call_method(bact->self, 90, &arg90);

                                    if ( arg90.ret_unit )
                                    {
                                        char v24[44];
//										*(_DWORD *)v24 = 1027;
//										v24[12] = robo->bact_internal->owner;
//										*(_DWORD *)&v24[16] = arg90.ret_unit->ypabact__id;
//										v24[20] = arg90.ret_unit->owner;
//										*(_DWORD *)&v24[32] = 0;
//										*(_DWORD *)&v24[36] = 0;
//										*(_DWORD *)&v24[40] = 0;
//										*(_DWORD *)&v24[24] = 34;
//										*(_DWORD *)&v24[28] = 19;
                                        yw_arg181 arg181;
                                        arg181.field_10 = 0;
                                        arg181.field_14 = 2;
                                        arg181.value = v24;
                                        arg181.val_size = 44;
                                        arg181.field_18 = 1;
                                        call_method(robo->wrld, 181, &arg181);
                                    }
                                }
                            }
                            else
                            {
                                __NC_STACK_ypabact *v33 = NULL;

                                nlist *v30;
                                call_vtbl(units->bacto, 3, 0x80001008, &v30, 0);

                                bact_node *v17 = (bact_node *)v30->head;
                                while (v17->next)
                                {
                                    if (yw->field_1b80->owner == v17->bact->owner)
                                    {
                                        v33 = v17->bact;
                                        break;
                                    }

                                    v17 = (bact_node *)v17->next;
                                }

                                if ( v33 )
                                {
                                    if ( v33->field_24 == 4 )
                                        call_vtbl(v33->self, 3, 0x80002000, &v33, 0);

                                    if ( v33->field_32 != v33->parent_bacto )
                                        v33 = v33->parent_bact;

                                    robo_arg134 arg134;
                                    arg134.field_8 = 0;
                                    arg134.field_10 = 0;
                                    arg134.field_C = 0;
                                    arg134.field_4 = 19;
                                    arg134.unit = v33;
                                    arg134.field_14 = 34;
                                    call_method(bact->self, 134, &arg134);
                                }
                            }
                        }
                    }
                    else if ( call_method(units->bacto, 108, &v31) )
                    {
                        if ( unit_bact->owner == unit_bact->p_cell_area->owner && unit_bact->p_cell_area->field_2F )
                        {
                            bact_arg67 arg67;
                            arg67.field_0 = 1;
                            arg67.field_4 = 0;
                            arg67.targ = unit_bact->field_621;

                            call_method(units->bacto, 67, &arg67);
                        }

                        unit_bact->field_3D6 &= 0xFFFFBFFF;

                        bact_node *nod = (bact_node *)unit_bact->list2.head;

                        while (nod->next)
                        {
                            nod->bact->field_3D6 &= 0xFFFFBFFF;

                            nod = (bact_node *)nod->next;
                        }

                        if ( its_me )
                        {
                            robo_arg134 arg134;
                            arg134.field_8 = unit_bact->field_2E;
                            arg134.field_4 = 10;
                            arg134.field_10 = 0;
                            arg134.field_C = 0;
                            arg134.unit = unit_bact;
                            arg134.field_14 = 28;

                            call_method(bact->self, 134, &arg134);
                        }
                    }
                }
            }
        }

        units = (bact_node *)units->next;
    }
}

void sub_4A0260(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v11 = 0;
    int v12;

    for(int i = -1; i < 2; i++)
    {
        for(int j = -1; j < 2; j++)
        {
            int v4 = bact->field_10 + j;
            int v5 = bact->field_c + i;

            if ( v5 > 0 && v5 < bact->field_20 - 2 && v4 > 0 && v4 < bact->field_22 - 2 )
            {

                __NC_STACK_ypabact *v7 = (__NC_STACK_ypabact *)&bact->p_cell_area[i + j * bact->field_20].field_3C.head;

                while (v7->next)
                {
                    if (v7->owner != robo->bact_internal->owner && v7->field_3D5 != 2 && v7->weapon != -1 && v7->mgun != -1)
                    {
                        v11 = 1;
                        v12 = v7->field_2E;
                        break;
                    }

                    v7 = (__NC_STACK_ypabact *)v7->next;
                }
            }

        }
    }

    if ( v11 )
    {
        if ( !(robo->field_1DB & 0x8000) && robo->field_1DB & 0x4000 )
        {
            robo_arg134 arg134;
            arg134.field_4 = 13;
            arg134.field_8 = v12;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = bact;
            arg134.field_14 = 99;

            call_method(bact->self, 134, &arg134);
        }

        robo->field_1DB |= 0x8000;
    }
    else
    {
        robo->field_1DB &= 0xFFFF7FFF;
    }
}

int yparobo_func70__sub6__sub0(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_22D % bact->field_20;
    int yy = robo->field_22D / bact->field_20;

    if ( !(cell->field_39 & (1 << bact->owner)) )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != robo->bact_internal->owner )
        return -1;

    if ( cell->field_3A )
        return -1;

    float v8 = sqrt( POW2(bact->field_10 - yy) + POW2(bact->field_c - xx) );

    float v14 = ( v8 <= 0.01 ? 500.0 : 1000.0 / v8 );

    int v9 = 0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << bact->owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * bact->field_20 ].field_39 ) )
        {
            v9 = 1;
            v14 += 10.0;
        }
    }

    if ( !v9 || xx <= 1 || xx >= bact->field_20 - 2 || yy <= 1 || yy >= bact->field_22 - 2 )
        return -1;

    return v14;
}

int yparobo_func70__sub6__sub1(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2C1 % bact->field_20;
    int yy = robo->field_2C1 / bact->field_20;

    if ( cell->field_39 & (1 << bact->owner) )
        return -1;

    float v13 = sqrt(POW2(bact->field_10 - yy) + POW2(bact->field_c - xx));

    if ( v13 <= 0.01 )
        v13 = 0.0;

    for (int i = 0; i < 8; i++)
    {
        if ( !((1 << bact->owner) & cell[ word_5182AE[i].x + word_5182AE[i].y * bact->field_20 ].field_39 ) )
        {
            v13 += 5.0;
        }
    }

    return v13;
}

int yparobo_func70__sub6__sub2(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_245 % bact->field_20;
    int yy = robo->field_245 / bact->field_20;

    if ( !(cell->field_39 & (1 << bact->owner)) )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != robo->bact_internal->owner || cell->field_3A )
        return -1;

    float v9 = sqrt( POW2(bact->field_10 - yy) + POW2(bact->field_c - xx) );

    float v14;

    if ( v9 <= 0.01 )
        v14 = 500.0;
    else
        v14 = 1000.0 / v9;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * bact->field_20 ];

        if ( cll->field_3A && bact->owner == cll->owner)
            v14 += 5.0;

    }

    return v14;
}

int yparobo_func70__sub6__sub3(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_25D % bact->field_20;
    int yy = robo->field_25D / bact->field_20;

    if ( !(cell->field_39 & (1 << bact->owner)) )
        return -1;

    if ( cell->field_2F >= 255 )
        return -1;

    if ( cell->field_3A )
        return -1;

//  if ( cell->field_2E != 1 )
//  {
//      for (int i = 0; i < 3; i++)
//      {
//          for (int j = 0; j < 3; j++)
//          {
//
//          }
//      }
//  }

    if ( cell->owner != bact->owner )
        return -1;

    float v9 = sqrt( POW2(bact->field_10 - yy) + POW2(bact->field_c - xx) );
    float v26 = v9 <= 0.01 ? 500.0 : 1000.0 / v9;

    if ( v9 > 8.0 )
        return -1;

    float v27 = v26 + (255 - cell->field_2F) / 3.0;

    for (int i = 0; i < 8; i++)
    {
        cellArea *cll = &cell[ word_5182AE[i].x + word_5182AE[i].y * bact->field_20 ];

        if ( cll->owner == bact->owner )
        {
            int v11 = 0;

            if ( cll->field_2E == 1 )
            {
                v11 = cll->buildings_health[0][0];
            }
            else
            {
                for (int j = 0; j < 3; j++)
                {
                    for (int k = 0; k < 3; k++)
                    {
                        v11 += cll->buildings_health[j][k];
                    }
                }
            }
            v27 += (255 - v11) * 0.05 + 10.0;
        }

        if ( cll->field_3A == 2 )
            v27 *= 0.7;
    }

    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cell->field_3C.head;

    while ( bct->next )
    {
        v27 -= 5.0;
        bct = (__NC_STACK_ypabact *)bct->next;
    }

    return v27;
}

int yparobo_func70__sub6__sub4(__NC_STACK_yparobo *robo, cellArea *cell)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_291 % bact->field_20;
    int yy = robo->field_291 / bact->field_20;

    if ( cell->owner == bact->owner )
        return -1;

    float v7 = sqrt( POW2(bact->field_10 - yy) + POW2(bact->field_c - xx) );
    float v25;

    if ( v7 <= 0.01 )
        v25 = 1500.0;
    else
        v25 = 1000.0 / v7;

    if ( (1 << bact->owner) & cell->field_39 )
    {
        if ( cell->field_3A )
            v25 = v25 + 20.0;
        if ( cell->field_3A == 6 )
            v25 = v25 + 40.0;
        if ( cell->field_3A == 2 )
            v25 = v25 + 50.0;
    }

    for (int i = 0; i < 8; i++)
    {
        int tx = xx + word_5182AE[i].x;
        int ty = yy + word_5182AE[i].y;

        if ( tx >= 1 && ty >= 1 && tx <= bact->field_20 - 2 && ty <= bact->field_22 - 2)
        {
            if ( cell[ word_5182AE[i].x + word_5182AE[i].y * bact->field_20 ].owner == bact->owner )
                v25 = v25 + 3.0;
        }
    }

    if ( (1 << bact->owner) & cell->field_39 )
    {
        __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cell->field_3C.head;

        while ( bct->next )
        {
            if ( bct->owner != bact->owner && bct->owner)
            {
                if ( bct->field_24 != 4)
                {
                    if ( bct->field_24 == 3)
                        v25 += 100.0;
                    else
                        v25 += 5.0;
                }
            }

            bct = (__NC_STACK_ypabact *)bct->next;
        }
    }

    return v25;
}

int yparobo_func70__sub6__sub8(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->field_20 * bact->field_22;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &robo->pcell[i];

        if ( cll->field_3A == 3 )
        {
            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->field_3C.head;
            while (bct->next)
            {
                if ( bct->field_24 == 9 && bct->field_3D5 != 2 && bact->owner == bct->owner )
                {
                    int a4;
                    call_vtbl(bct->self, 3, 0x80002006, &a4, 0);

                    if ( a4 == 0 && ( bct->weapon != -1 || bct->mgun != -1 ) )
                    {
                        v4++;
                        break;
                    }
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v4;
}

int yparobo_func70__sub6__sub9(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->field_20 * bact->field_22;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        cellArea *cll = &robo->pcell[i];

        if ( cll->field_3A == 3 )
        {
            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->field_3C.head;
            while (bct->next)
            {
                if ( bct->field_24 == 9 && bct->field_3D5 != 2 && bact->owner == bct->owner )
                {
                    int a4;
                    call_vtbl(bct->self, 3, 0x80002006, &a4, 0);

                    if ( a4 == 0 && bct->weapon == -1 && bct->mgun == -1 )
                    {
                        v4++;
                        break;
                    }
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v4;
}

int yparobo_func70__sub6__sub10(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->field_20 * bact->field_22;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( robo->pcell[i].owner == bact->owner )
            v4++;
    }

    return v4;
}

int yparobo_func70__sub6__sub11(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int num = bact->field_20 * bact->field_22;

    int v4 = 0;

    for (int i = 0; i < num; i++)
    {
        if ( !((1 << bact->owner) & robo->pcell[i].field_39) )
            v4++;
    }

    return v4;
}

int yparobo_func70__sub6__sub12(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_node *node = &bact->list_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if ( node->bact->field_24 == 3)
        {
            if ( bact->owner != node->bact->owner )
            {
                int v9 = bact->field_c - node->bact->field_c;
                int v10 = bact->field_10 - node->bact->field_10;

                if ( abs(v9) < 3 && abs(v10) < 3 )
                    return 1;
            }
        }

        node = (bact_node *)node->next;
    }

    return 0;
}

int yparobo_func70__sub6__sub13(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int xx = bact->field_c + i;
            int yy = bact->field_10 + j;

            if ( xx > 0 && xx < bact->field_20 - 1 && yy > 0 && yy < bact->field_22 - 1 )
            {
                __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *) bact->p_cell_area[ i + j * bact->field_20 ].field_3C.head;

                while ( bct->next )
                {
                    if ( bct->owner )
                    {
                        if ( bct->owner != bact->owner )
                        {
                            if ( bct->field_24 != 4 && bct->field_24 != 3 && bct->field_3D5 != 2 )
                                return 1;
                        }
                    }
                    bct = (__NC_STACK_ypabact *)bct->next;
                }
            }
        }
    }

    return 0;
}


float sub_4F4C6C(bact_node *bctnd, __NC_STACK_yparobo *robo, int a4, int a3)
{
    __NC_STACK_ypabact *bct = bctnd->bact;

    if ( bct->field_3D6 & 0x400 )
        return -1.0;

    float v8 = sqrt( POW2(a3 - bct->field_10) + POW2(a4 - bct->field_c) );

    if (v8 != 0.0)
        v8 = 100.0 / v8;
    else
        v8 = 200.0;

    if ( bct->p_cell_area->owner == robo->bact_internal->owner )
        v8 += 100.0;

    return v8;
}

int yparobo_func70__sub6__sub5(__NC_STACK_yparobo *robo, int *a2, int *px, int *py)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact_node *node = &bact->list_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    float v32 = -0.5;

    int v29 = 0;

    while (node->next)
    {

        if ( node->bact->owner != bact->owner  &&  node->bact->owner  &&  node->bact->field_3D5 != 2 )
        {
            if ( abs(node->bact->field_c - bact->field_c) <= 2 &&
                    abs(node->bact->field_10 - bact->field_10) <= 2 )
            {
                *a2 = node->bact->field_2E;
                *px = node->bact->field_c;
                *py = node->bact->field_10;

                return 500;
            }

            bact_node *ndbct = (bact_node *)node->bact->list2.head;

            while (ndbct->next)
            {

                if ( (ndbct->bact->ypabact__id & 0xF) == robo->field_27D )
                {
                    int v26 = 0;

                    if ( ndbct->bact->field_24 == 9 )
                        call_vtbl(ndbct->bacto, 3, 0x80002006, &v26, 0);

                    if ( (1 << bact->owner) & ndbct->bact->p_cell_area->field_39 )
                    {
                        if ( ndbct->bact->field_3D5 != 2 && !v26 )
                        {
                            float tmp = sub_4F4C6C(ndbct, robo, bact->field_c, bact->field_10);
                            if ( tmp > v32 )
                            {
                                v32 = tmp;
                                v29 = ndbct->bact->field_2E;
                                *px = ndbct->bact->field_c;
                                *py = ndbct->bact->field_10;
                            }
                        }
                    }
                }
                ndbct = (bact_node *)ndbct->next;
            }
        }


        node = (bact_node *)node->next;
    }

    robo->field_27D++;

    if ( robo->field_27D > 15 )
        robo->field_27D = 0;

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v29;
    return v32;
}

int yparobo_func70__sub6__sub6(__NC_STACK_yparobo *robo, int *a2, int *px, int *py)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v32 = -0.5;
    int v21 = 0;

    bact_node *node = &bact->list_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if ( node->bact->owner != robo->bact_internal->owner && node->bact->owner && node->bact->field_3D5 != 2 )
        {
            if ( (1 << bact->owner) & node->bact->p_cell_area->field_39 )
            {
                float v13 = sub_4F4C6C(node, robo, bact->field_c, bact->field_10);
                if ( v13 > v32 )
                {
                    v21 = node->bact->field_2E;
                    v32 = v13;
                    *px = node->bact->field_c;
                    *py = node->bact->field_10;
                }
            }
        }
        node = (bact_node *)node->next;
    }

    if ( v32 < 0.0 )
    {
        *a2 = 0;
        return -1;
    }

    *a2 = v21;
    return v32;
}


int sub_4F4E48(__NC_STACK_yparobo *robo, int x, int y)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v14 = 0.0;

    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            cellArea *cll = &robo->pcell[(y + j) * bact->field_20 + x + i];

            yw_arg176 arg176;
            arg176.owner = cll->owner;

            call_method(robo->wrld, 176, &arg176);

            float v15;

            if ( i || j )
                v15 = 1.0;
            else
                v15 = 3.0;

            if ( bact->owner == cll->owner )
                v14 += cll->field_2F * arg176.field_4 + v15;
            else
                v14 -= cll->field_2F * arg176.field_4 + v15;

            __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->field_3C.head;

            while ( bct->next )
            {
                float v16 = 0;

                if ( bct->field_24 == 3 && bct != bact )
                {
                    v16 = 5.0;
                }
                else
                {
                    if ( bct->field_24 == 9 )
                    {
                        int a4;
                        call_vtbl(bct->self, 3, 0x80002006, &a4, 0);

                        if ( !a4 )
                            v16 = 1.0;
                    }
                }

                if ( v16 > 0.0 )
                {
                    if ( bct->owner == bact->owner )
                        v14 += v16;
                    else
                        v14 -= v16;
                }

                bct = (__NC_STACK_ypabact *)bct->next;
            }
        }
    }

    return v14;
}

int yparobo_func70__sub6__sub7(__NC_STACK_yparobo *robo)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int xx = robo->field_2A9 % bact->field_20;
    int yy = robo->field_2A9 / bact->field_20;

    int a1_4 = sub_4F4E48(robo, bact->field_c, bact->field_10);
    int v14 = sub_4F4E48(robo, xx, yy);

    float v12 = sqrt( POW2(bact->field_10 - yy) + POW2(bact->field_c - xx) );

    if ( v12 > 0.0 && a1_4 < v14 )
        v14 = ((float)v14 * (1.0 - v12 * 0.8 / 91.0));

    if ( v14 <= a1_4 )
        return -1;

    return v14;
}

void yparobo_func70__sub6(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_299 -= arg->field_4;
        if ( robo->field_299 > 0 )
            v6 = 0;
    }

    robo_arg128 arg128;

    if ( robo->field_1E7 && v6 )
    {
        int v89 = (bact->field_22 + bact->field_20) / 20 + 1;

        for (int i = v89; i > 0; i--)
        {
            if ( bact->field_20 * bact->field_22 == robo->field_291 )
            {
                robo->field_291 = 0;
                robo->field_1DB |= 0x400;
                return;
            }

            cellArea *pcell = &robo->pcell[ robo->field_291 ];

            int xx = robo->field_291 % bact->field_20;
            int yy = robo->field_291 / bact->field_20;

            if ( xx && xx != bact->field_20 - 1 && yy && yy != bact->field_22 - 1 )
            {
                int v12 = yparobo_func70__sub6__sub4(robo, pcell);
                if ( v12 > robo->field_285 )
                {
                    arg128.field_0 = 2;
                    arg128.field_8 = 1;
                    arg128.field_C = (xx + 0.5) * 1200.0;
                    arg128.field_14 = -(yy + 0.5) * 1200.0;

                    call_method(bact->self, 128, &arg128);

                    if ( !arg128.bacto )
                    {
                        robo->field_285 = v12;
                        robo->field_28D = robo->field_291;
                        robo->field_289 = pcell;
                        robo->field_295 = arg->field_0;
                    }
                }
            }

            robo->field_291++;
        }
    }
    else
    {
        robo->field_291 = 0;
        robo->field_285 = 0;
        robo->field_1DB &= 0xFFFFFBFF;
    }

    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_281 -= arg->field_4;

        if ( robo->field_281 > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub13(robo) )
        v6 = 1;

    if ( robo->field_1EA && v6 )
    {
        int v91, xx, yy;

        int v17 = yparobo_func70__sub6__sub5(robo, &v91, &xx, &yy);
        if ( v17 > robo->field_269 )
        {
            arg128.field_0 = 2;
            arg128.field_8 = 2;
            arg128.field_18 = v91;
            call_method(bact->self, 128, &arg128);

            if ( !arg128.bacto )
            {
                robo->field_269 = v17;
                robo->field_275 = v91;
                robo->field_279 = arg->field_0;
                robo->field_271 = yy * bact->field_20 + xx;
            }
        }
        robo->field_1DB |= 0x200;
    }
    else
    {
        robo->field_269 = 0;
        robo->field_1DB &= 0xFFFFFDFF;
    }

    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_2E1 -= arg->field_4;
        if ( robo->field_2E1 > 0 )
            v6 = 0;
    }

    if ( yparobo_func70__sub6__sub12(robo) )
        v6 = 1;

    if ( robo->field_1EE && v6 )
    {
        int v91, xx, yy;
        int v22 = yparobo_func70__sub6__sub6(robo, &v91, &xx, &yy);

        if ( v22 > robo->field_2CD )
        {
            robo_arg128 arg128;
            arg128.field_0 = 2;
            arg128.field_8 = 2;
            arg128.field_18 = v91;

            call_method(bact->self, 128, &arg128);

            if ( !arg128.bacto )
            {
                robo->field_2CD = v22;
                robo->field_2D9 = v91;
                robo->field_2DD = arg->field_0;
                robo->field_2D5 = bact->field_20 * yy + xx;
            }
        }
        robo->field_1DB |= 0x100000;
    }
    else
    {
        robo->field_2CD = 0;
        robo->field_1DB &= 0xFFEFFFFF;
    }

    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_2B1 -= arg->field_4;
        if ( robo->field_2B1 > 0 )
            v6 = 0;
    }

    if ( robo->field_1ED && v6 )
    {
        int v25 = (bact->field_20 + bact->field_22) / 10 + 1;

        for (int i = v25; i > 0; i--)
        {
            if ( bact->field_20 * bact->field_22 == robo->field_2A9 )
            {
                robo->field_2A9 = 0;
                robo->field_1DB |= 0x10000;
                return;
            }

            int xx = robo->field_2A9 % bact->field_20;
            int yy = robo->field_2A9 / bact->field_20;

            if ( xx && xx != bact->field_20 - 1 && yy && yy != bact->field_22 - 1 )
            {
                int v34 = yparobo_func70__sub6__sub7(robo);
                if ( v34 > robo->field_29D )
                {
                    robo->field_29D = v34;
                    robo->field_2A5 = robo->field_2A9;
                    robo->field_2AD = arg->field_0;
                }
            }
            robo->field_2A9++;
        }
    }
    else
    {
        robo->field_29D = 0;
        robo->field_2A9 = 0;
        robo->field_1DB &= 0xFFFEFFFF;
    }

    yw_arg176 v85;
    v85.owner = bact->owner;
    call_method(robo->wrld, 176, &v85);

    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_265 -= arg->field_4;
        if ( robo->field_265 > 0 )
            v6 = 0;
    }

    if ( v85.field_4 == 0.0 )
    {
        if ( robo->field_1E9 )
            v6 = 1;
    }

    float v93;
    if ( robo->field_1E9 )
        v93 = 100.0 / (float)robo->field_1E9 * 0.9;
    else
        v93 = 0.89999998;

    if ( robo->field_1E9 && (v85.field_8 > v93 || v85.field_4 < 0.0001) && v6 )
    {
        int v99 = (bact->field_20 + bact->field_22) / 20 + 1;
        for (int i = v99; i > 0; i--)
        {
            if ( bact->field_20 * bact->field_22 == robo->field_25D )
            {
                robo->field_25D = 0;
                robo->field_1DB |= 0x1000;
                return;
            }
            int xx = robo->field_25D % bact->field_20;
            int yy = robo->field_25D / bact->field_20;

            if ( xx && xx != bact->field_20 - 1 && yy && yy != bact->field_22 - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->field_25D];
                int v46 = yparobo_func70__sub6__sub3(robo, pcell);
                if ( v46 > robo->field_251 )
                {
                    robo->field_251 = v46;
                    robo->field_255 = pcell;
                    robo->field_259 = robo->field_25D;
                    robo->field_261 = arg->field_0;
                }
            }
            robo->field_25D++;
        }
    }
    else
    {
        robo->field_251 = 0;
        robo->field_25D = 0;
        robo->field_1DB &= 0xFFFFEFFF;
    }

    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_235 -= arg->field_4;
        if ( robo->field_235 > 0 )
            v6 = 0;
    }

    if ( robo->field_1e8 && (yparobo_func70__sub6__sub11(robo) / 64 > yparobo_func70__sub6__sub9(robo)) && v6 )
    {
        int v48 = (bact->field_22 + bact->field_20) / 20 + 1;

        for(int i = v48; i > 0; i--)
        {
            if ( bact->field_20 * bact->field_22 == robo->field_22D )
            {
                robo->field_22D = 0;
                robo->field_1DB |= 0x800;
                return;
            }

            int xx = robo->field_22D % bact->field_20;
            int yy = robo->field_22D / bact->field_20;

            if ( xx && xx != bact->field_20 - 1 && yy && yy != bact->field_22 - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->field_22D];

                int v58 = yparobo_func70__sub6__sub0(robo, pcell);

                if ( v58 > robo->field_221 )
                {
                    robo->field_221 = v58;
                    robo->field_225 = pcell;
                    robo->field_229 = robo->field_22D;
                    robo->field_231 = arg->field_0;
                }
            }
            robo->field_22D++;
        }
    }
    else
    {
        robo->field_22D = 0;
        robo->field_221 = 0;
        robo->field_1DB &= 0xFFFFF7FF;
    }


    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_24D -= arg->field_4;
        if ( robo->field_24D > 0 )
            v6 = 0;
    }

    int v92;
    if ( robo->field_1EB )
        v92 = 10 * (100 / robo->field_1EB);
    else
        v92 = 10;

    if ( robo->field_1EB && (yparobo_func70__sub6__sub10(robo) / v92) >= yparobo_func70__sub6__sub8(robo) && v6 )
    {
        int v60 = (bact->field_22 + bact->field_20) / 20 + 1;

        for(int i = v60; i > 0; i--)
        {
            if ( bact->field_20 * bact->field_22 == robo->field_245 )
            {
                robo->field_245 = 0;
                robo->field_1DB |= 0x2000;
                return;
            }

            int xx = robo->field_245 % bact->field_20;
            int yy = robo->field_245 / bact->field_20;

            if ( xx && xx != bact->field_20 - 1 && yy && yy != bact->field_22 - 1 )
            {
                cellArea *pcell = &robo->pcell[robo->field_245];
                int v71 = yparobo_func70__sub6__sub2(robo, pcell);

                if ( v71 > robo->field_239 )
                {
                    robo->field_239 = v71;
                    robo->field_23D = pcell;
                    robo->field_241 = robo->field_245;
                    robo->field_249 = arg->field_0;
                }
            }
            robo->field_245++;
        }
    }
    else
    {
        robo->field_239 = 0;
        robo->field_245 = 0;
        robo->field_1DB &= 0xFFFFDFFF;
    }


    v6 = 1;
    if ( robo->field_1DF )
    {
        robo->field_2C9 -= arg->field_4;
        if ( robo->field_2C9 > 0 )
            v6 = 0;
    }

    if ( robo->field_1ec && v6 )
    {
        int v90 = (bact->field_22 + bact->field_20) / 20 + 1;

        for (int i = v90; i > 0; i-- )
        {
            if ( bact->field_20 * bact->field_22 == robo->field_2C1 )
            {
                robo->field_2C1 = 0;
                robo->field_1DB |= 0x40000;
                return;
            }

            int xx = robo->field_2C1 % bact->field_20;
            int yy = robo->field_2C1 / bact->field_20;

            if ( xx > 1 && xx < bact->field_20 - 2 && yy > 1 && yy < bact->field_22 - 2 )
            {
                cellArea *pcell = &robo->pcell[robo->field_2C1];
                int v81 = yparobo_func70__sub6__sub1(robo, pcell);

                if ( v81 > robo->field_2B5 )
                {
                    robo_arg128 arg128;
                    arg128.field_8 = 1;
                    arg128.field_0 = 2;
                    arg128.field_C = (xx + 0.5) * 1200.0;
                    arg128.field_14 = -(yy + 0.5) * 1200.0;

                    call_method(bact->self, 128, &arg128);

                    if ( !arg128.bacto )
                    {
                        robo->field_2B5 = v81;
                        robo->field_2BD = robo->field_2C1;
                        robo->field_2B9 = pcell;
                        robo->field_2C5 = arg->field_0;
                    }
                }
            }
            robo->field_2C1++;
        }
    }
    else
    {
        robo->field_2B5 = 0;
        robo->field_2C1 = 0;
        robo->field_1DB &= 0xFFFBFFFF;
    }
}

void yparobo_func70(NC_STACK_yparobo *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int a4;
    call_vtbl(obj, 3, 0x80001007, &a4, 0);

    float tmp = sqrt(bact->field_639.sx * bact->field_639.sx + bact->field_639.sy * bact->field_639.sy + bact->field_639.sz * bact->field_639.sz);

    if ( tmp <= 0.0 )
    {
        bact->field_645.sy = 0;
        bact->field_645.sx = bact->field_645.sy;
        bact->field_645.sz = 0;
    }
    else
    {
        float v7 = 1.0 / tmp;
        bact->field_645.sx = bact->field_639.sx * v7;
        bact->field_645.sy = bact->field_639.sy * v7;
        bact->field_645.sz = bact->field_639.sz * v7;
    }

    if ( bact->field_3D5 == 1 )
    {
        if ( robo->field_1DB & 0x400000 )
            sub_4A5580(robo, arg->field_4);

        int v11 = arg->field_4;

        if ( !a4 || ! call_method(obj, 87, &v11) )
        {
            yparobo_func70__sub1(robo, arg->field_4 * 0.001);

            if ( robo->field_1DB & 0x4000 )
                sub_4A4088(robo, arg);
            else
                yparobo_func70__sub2(robo, arg);

            if ( robo->field_1DB & 0x4000 )
            {
                sub_4A42B8(robo);
                sub_4A43E8(robo);
                sb_0x4a45cc(robo, arg);
            }

            if ( !(robo->field_1DB & 0x4000) )
            {
                yparobo_func70__sub6(robo, arg);
                yparobo_func70__sub4(robo, arg);
                yparobo_func70__sub3(robo, arg);
                yparobo_func70__sub5(robo);
            }

            sb_0x4a7010(robo);

            sub_4A0260(robo);

            if ( robo->field_1DB & 0x8000 && !(robo->field_1DB & 0x4000) )
                yparobo_func70__sub0(robo, arg);
        }
    }
    else if ( bact->field_3D5 == 2 )
    {
        call_method(obj, 121, arg);
    }
}

void yparobo_func71__sub0(__NC_STACK_yparobo *robo, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = robo->bact_internal;

    struC5 *inpt = arg->inpt;
    float v18 = arg->field_4 / 1000.0;


    if ( inpt->sliders_vars[1] < -0.001 )
    {
        bact->field_5F1 += inpt->sliders_vars[1] * bact->maxrot * v18 * 2.0;

        if ( bact->field_5F1 > bact->viewer_max_up )
            bact->field_5F1 = bact->viewer_max_up;

        if ( bact->field_5F1 < -bact->viewer_max_down )
            bact->field_5F1 = -bact->viewer_max_down;
    }
    else if (inpt->sliders_vars[1] > 0.001)
    {
        bact->field_5F1 += inpt->sliders_vars[1] * bact->maxrot * v18 * 2.0;

        if ( bact->field_5F1 > bact->viewer_max_up )
            bact->field_5F1 = bact->viewer_max_up;

        if ( bact->field_5F1 < -bact->viewer_max_down )
            bact->field_5F1 = -bact->viewer_max_down;
    }

    if ( inpt->sliders_vars[0] < -0.001 )
    {
        bact->field_5ED -= inpt->sliders_vars[0] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_max_side < 3.15 )
        {
            if ( bact->field_5ED > bact->viewer_max_side )
                bact->field_5ED = bact->viewer_max_side;

            if ( bact->field_5ED < -bact->viewer_max_side )
                bact->field_5ED = -bact->viewer_max_side;
        }
    }
    else if (inpt->sliders_vars[0] > 0.001)
    {
        bact->field_5ED -= inpt->sliders_vars[0] * bact->maxrot * v18 * 2.0;

        if ( bact->viewer_max_side < 3.15 )
        {
            if ( bact->field_5ED > bact->viewer_max_side )
                bact->field_5ED = bact->viewer_max_side;

            if ( bact->field_5ED < -bact->viewer_max_side )
                bact->field_5ED = -bact->viewer_max_side;
        }
    }
}

void yparobo_func71(NC_STACK_yparobo *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    int a4;
    call_vtbl(obj, 3, 0x80001007, &a4, 0);

    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v36 = sqrt(bact->field_639.sx * bact->field_639.sx + bact->field_639.sy * bact->field_639.sy + bact->field_639.sz * bact->field_639.sz);

    if ( v36 <= 0.0 )
    {
        bact->field_645.sy = 0;
        bact->field_645.sx = bact->field_645.sy;
        bact->field_645.sz = 0;
    }
    else
    {
        float v9 = 1.0 / v36;
        bact->field_645.sx = bact->field_639.sx * v9;
        bact->field_645.sy = bact->field_639.sy * v9;
        bact->field_645.sz = bact->field_639.sz * v9;
    }

    if ( bact->field_3D5 == 1 )
    {
        if ( robo->field_1DB & 0x400000 )
            sub_4A5580(robo, arg->field_4);

        int v28 = arg->field_4;
        if ( !a4 || !call_method(obj, 87, &v28) )
        {
            sb_0x4a7010(robo);
            sub_4A0260(robo);
            sub_4A42B8(robo);
            sub_4A43E8(robo);

            if ( arg->inpt->but_flags & 8 )
                call_method(obj, 97, arg);

            sb_0x4a45cc(robo, arg);
            yparobo_func71__sub0(robo, arg);
            sub_4A4088(robo, arg);

            bact->field_5C9 = bact->field_651;

            mat3x3 v21;
            v21.m00 = cos(bact->field_5ED);
            v21.m01 = 0;
            v21.m02 = sin(bact->field_5ED);
            v21.m10 = 0;
            v21.m11 = 1.0;
            v21.m12 = 0;
            v21.m20 = -sin(bact->field_5ED);
            v21.m21 = 0;
            v21.m22 = cos(bact->field_5ED);

            mat3x3 dst;
            mat_mult(&v21, &bact->field_5C9, &dst);
            bact->field_5C9 = dst;

            v21.m00 = 1.0;
            v21.m01 = 0;
            v21.m02 = 0;
            v21.m10 = 0;
            v21.m11 = cos(bact->field_5F1);
            v21.m12 = sin(bact->field_5F1);
            v21.m20 = 0;
            v21.m21 = -sin(bact->field_5F1);
            v21.m22 = cos(bact->field_5F1);

            mat_mult(&v21, &bact->field_5C9, &dst);

            bact->field_5C9 = dst;
        }
    }
    else if ( bact->field_3D5 == 2 )
    {
        call_method(obj, 121, arg);
    }
    else if ( bact->field_3D5 == 3 )
    {
        bact_arg119 arg78;
        arg78.field_0 = 1;
        arg78.field_4 = 0;
        arg78.field_8 = 0;

        call_method(obj, 78, &arg78);

        bact->field_3D6 &= 0xFFFFFDFF;
    }
}

void yparobo_func74(NC_STACK_yparobo *obj, class_stru *zis, bact_arg74 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    bact->field_62D = bact->field_621;

    float v46 = 0.0;
    float v48 = 1.0;
    float v50 = 0.0;


    float v63;
    if ( bact->field_3D5 == 2 )
        v63 = bact->mass * 39.2266;
    else
        v63 = bact->mass * 9.80665;


    float v52, v54, v56, v65;
    if ( arg->flag & 1 )
    {
        v52 = 0.0;
        v54 = 0.0;
        v56 = 0.0;
        v65 = 0.0;
    }
    else
    {
        v52 = 0.0;
        v54 = -1.0;
        v56 = 0.0;
        v65 = robo->field_c;
    }

    float v51, v53, v55, v64;
    if ( arg->flag & 1 )
    {
        v55 = 0.0;
        v53 = 0.0;
        v64 = 0.0;
        v51 = 0.0;
    }
    else
    {
        v51 = arg->vec.sx;
        v55 = arg->vec.sy;
        v53 = arg->vec.sz;
        v64 = bact->field_601;
    }

    float v61 = bact->field_611 * bact->airconst;

    float v57 = v46 * v63 + v51 * v64 + -bact->field_605.sx * v61 + v52 * v65;
    float v58 = v48 * v63 + v55 * v64 + -bact->field_605.sy * v61 + v54 * v65;
    float v44 = v50 * v63 + v53 * v64 + -bact->field_605.sz * v61 + v56 * v65;

    float v62 = sqrt(v58 * v58 + v57 * v57 + v44 * v44);

    if ( v62 > 0.0 )
    {
        float v43 = v62 / bact->mass * arg->field_0;
        float v42 = 1.0 / v62;

        float v66 = bact->field_605.sy * bact->field_611 + v58 * v42 * v43;
        float v67 = bact->field_605.sx * bact->field_611 + v57 * v42 * v43;
        float v68 = bact->field_605.sz * bact->field_611 + v44 * v42 * v43;

        float v59 = sqrt(v66 * v66 + v67 * v67 + v68 * v68);

        if ( v59 > 0.0 )
        {
            float v26 = 1.0 / v59;
            v67 *= v26;
            v66 *= v26;
            v68 *= v26;
        }

        robo->bact_internal->field_605.sx = v67;
        robo->bact_internal->field_605.sy = v66;
        robo->bact_internal->field_605.sz = v68;
        robo->bact_internal->field_611 = v59;
    }

    if ( fabs(bact->field_611) > 0.1 )
    {
        bact->field_621.sx += bact->field_605.sx * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sy += bact->field_605.sy * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sz += bact->field_605.sz * bact->field_611 * arg->field_0 * 6.0;
    }

    call_method(obj, 115, 0);

    for (int i = 0; i < 8; i++)
    {
        if (robo->guns[i].gun_obj)
        {
            bact_arg80 arg80;
            arg80.pos.sx = bact->field_621.sx + bact->field_651.m00 * robo->guns[i].pos.sx + bact->field_651.m10 * robo->guns[i].pos.sy + bact->field_651.m20 * robo->guns[i].pos.sz;
            arg80.pos.sy = bact->field_621.sy + bact->field_651.m01 * robo->guns[i].pos.sx + bact->field_651.m11 * robo->guns[i].pos.sy + bact->field_651.m21 * robo->guns[i].pos.sz;
            arg80.pos.sz = bact->field_621.sz + bact->field_651.m02 * robo->guns[i].pos.sx + bact->field_651.m12 * robo->guns[i].pos.sy + bact->field_651.m22 * robo->guns[i].pos.sz;
            arg80.field_C = 4;

            call_method(robo->guns[i].gun_obj, 80, &arg80);
        }
    }

    robo->bact_internal->field_5A.samples_data[0].pitch = robo->bact_internal->field_3BA;
    robo->bact_internal->field_5A.samples_data[0].volume = robo->bact_internal->field_3B6;

    float v60 = fabs(bact->field_611) / (bact->force / bact->airconst2) * 1.4;

    if ( bact->field_5A.samples_data[0].psampl )
        bact->field_5A.samples_data[0].pitch = (bact->field_5A.samples_data[0].psampl->SampleRate + bact->field_5A.samples_data[0].pitch) * v60;
}

void yparobo_func77(NC_STACK_yparobo *obj, class_stru *zis, void *)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    _NC_STACK_ypaworld *yw = robo->wrld_yw;

    int a4;
    call_vtbl(obj, 3, 0x8000100B, &a4, 0);

    char v16[24];

    if ( !(bact->field_3D6 & 0x400) )
    {
        /*(_DWORD *)v16 = 1016;
        v16[12] = bact->owner;
        if ( bact->field_9B1 )
          *(_DWORD *)&v16[16] = bact->field_9B1->ypabact__id;
        else
          *(_DWORD *)&v16[16] = 0;
        v16[20] = bact->field_9B5;*/

        if ( bact->field_9B5 )
        {
            if ( !(bact->field_3D6 & 0x20000) )
            {
                yw_arg177 arg177;
                arg177.field_4 = bact->field_9B5;
                arg177.bact = bact;

                call_method(robo->wrld, 177, &arg177); //Reown sectors for new owner
            }
        }

        bact_node *node = (bact_node *)bact->list2.head;

        while (node->next)
        {
            bact_node *subnode = (bact_node *)node->bact->list2.head;

            while(subnode->next)
            {
                bact_node *next2 = (bact_node *)subnode->next;

                subnode->bact->field_9B1 = bact->field_9B1;

                if ( bact->field_3D6 & 0x2000000 )
                    subnode->bact->field_3D6 |= 0x2000000;

                call_method(subnode->bacto, 77, 0);

                subnode->bact->field_3D6 &= 0xFFFFFDFF;

                bact_arg119 arg119;
                arg119.field_8 = 0;
                arg119.field_4 = 0;
                arg119.field_0 = 4;
                call_method(subnode->bacto, 119, &arg119);

                subnode->bact->field_3D5 = 2;

                sub_424000(&subnode->bact->field_5A, 3);
                sub_423F74(&subnode->bact->field_5A, 7);

                subnode->bact->field_3B2 &= 0xFFFFFFF7;
                subnode->bact->field_3B2 |= 0x80;

                call_vtbl(subnode->bacto, 2, 0x8000100B, a4, 0);

                subnode = next2;
            }


            bact_node *next1 = (bact_node *)node->next;

            node->bact->field_9B1 = bact->field_9B1;

            if ( bact->field_3D6 & 0x2000000 )
                node->bact->field_3D6 |= 0x2000000;

            call_method(node->bacto, 77, 0);

            node->bact->field_3D6 &= 0xFFFFFDFF;

            bact_arg119 arg119;
            arg119.field_8 = 0;
            arg119.field_4 = 0;
            arg119.field_0 = 4;
            call_method(node->bacto, 119, &arg119);

            node->bact->field_3D5 = 2;

            sub_424000(&node->bact->field_5A, 3);
            sub_423F74(&node->bact->field_5A, 7);

            node->bact->field_3B2 &= 0xFFFFFFF7;
            node->bact->field_3B2 |= 0x80;

            call_vtbl(node->bacto, 2, 0x8000100B, a4, 0);

            node = next1;
        }

        NC_STACK_ypabact *v20;
        call_vtbl(robo->wrld, 3, 0x80002010, &v20, 0);

        if ( obj == v20 )
        {
            if ( !(bact->field_3D6 & 0x2000000) )
            {
                robo_arg134 arg134;
                arg134.field_4 = 12;
                arg134.field_8 = 0;
                arg134.field_10 = 0;
                arg134.field_C = 0;
                arg134.unit = bact;
                arg134.field_14 = 100;
                call_method(bact->self, 134, &arg134);
            }
        }
        else
        {
            robo_arg134 arg134;
            arg134.field_4 = 11;
            arg134.field_8 = bact->owner;
            arg134.field_10 = 0;
            arg134.field_C = 0;
            arg134.unit = bact->field_9B1;
            arg134.field_14 = 50;
            call_method(bact->self, 134, &arg134);
        }

        call_parent(zis, obj, 77, 0);

        bact->field_6BD[1].field_34 = 0;

        if ( yw->field_757E )
        {
            if ( bact->owner )
            {
                yw_arg181 arg181;
                arg181.value = v16;
                arg181.val_size = 24;
                arg181.field_14 = 2;
                arg181.field_10 = 0;
                arg181.field_18 = 1;
                call_method(yw->self_full, 181, &arg181);

                if ( obj == yw->field_1b78 )
                {
                    call_vtbl(yw->field_1b7c, 2, 0x80001005, 0, 0);
                    call_vtbl(yw->field_1b7c, 2, 0x80001004, 0, 0);

                    call_vtbl(obj, 2, 0x80001005, 1, 0);
                    call_vtbl(obj, 2, 0x80001004, 1, 0);
                }
            }
        }
    }
}

size_t yparobo_func80(NC_STACK_yparobo *obj, class_stru *zis, bact_arg80 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    int v5 = call_parent(zis, obj, 80, (stack_vals *)arg);
    if ( !v5 )
        return 0;

    robo->field_c = bact->mass * 9.80665;

    for (int i = 0; i < 8; i++)
    {
        roboGun *gun = &robo->guns[i];

        if (gun->gun_obj)
        {
            bact_arg80 v11;
            v11.pos.sx = bact->field_621.sx + bact->field_651.m00 * gun->pos.sx + bact->field_651.m10 * gun->pos.sy + bact->field_651.m20 * gun->pos.sz;
            v11.pos.sy = bact->field_621.sy + bact->field_651.m01 * gun->pos.sx + bact->field_651.m11 * gun->pos.sy + bact->field_651.m21 * gun->pos.sz;
            v11.pos.sz = bact->field_621.sz + bact->field_651.m02 * gun->pos.sx + bact->field_651.m12 * gun->pos.sy + bact->field_651.m22 * gun->pos.sz;
            v11.field_C = 4;

            call_method(gun->gun_obj, 80, &v11);
        }
    }

    robo->field_1D5 = bact->field_621.sy;

    return 1;
}

// Update robo energys
void yparobo_func82(NC_STACK_yparobo *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    float v65 = bact->reload_const_or_energy2;
    float v63 = (bact->field_915 - bact->field_95D) / 1000.0;

    if ( bact->field_3D5 != 2 && v63 >= 0.25 )
    {
        int v62 = bact->energy;

        int v61 = robo->field_4F5;
        int v59 = robo->field_4FD;

        bact->field_95D = bact->field_915;

        yw_arg176 arg176;
        arg176.owner = bact->p_cell_area->owner;

        call_method(robo->wrld, 176, &arg176);

        float v64 = bact->p_cell_area->field_2F;

        float v70 = v65 * v63 * v64 * arg176.field_4;

        if ( robo->field_1DB & 0x4000 )
        {
            int v67 = 0;

            float v68 = v70 / 6000.0;

            if ( bact->p_cell_area->owner )
            {
                if ( bact->p_cell_area->owner != bact->owner )
                    v68 = -v68;
            }
            else
            {
                v68 = 0.0;
            }

            if ( robo->field_501 & 1 || v68 < 0.0 )
                v67++;

            if ( robo->field_501 & 4 )
                v67++;

            if ( robo->field_501 & 8 )
                v67++;

            robo->field_505 = v65 * v64 / 6000.0;

            if ( v67 > 0 )
            {
                int v25 = 0;
                int v26 = 0;
                int v66 = 0;

                v68 = v68 / (float)v67;

                if ( robo->field_501 & 1 || v68 < 0.0 )
                    bact->energy += v68;

                if ( robo->field_501 & 4 )
                    robo->field_4F5 += v68;

                if ( robo->field_501 & 8 )
                    robo->field_4FD += v68;

                float v60 = v63 * v65 / 30.0;

                if ( robo->field_501 & 1 )
                    v25 = bact->energy;

                if ( robo->field_501 & 4 )
                    v25 += robo->field_4F5;

                if ( robo->field_501 & 8 )
                    v25 += robo->field_4FD;

                int v34 = v25 / v67;

                if ( robo->field_501 & 1 )
                {
                    if ( v34 > bact->energy )
                        v26++;
                    else if ( v34 < bact->energy )
                        v66++;
                }

                if ( robo->field_501 & 4 )
                {
                    if ( v34 > robo->field_4F5 )
                        v26++;
                    else if ( v34 < robo->field_4F5 )
                        v66++;
                }

                if ( robo->field_501 & 8 )
                {
                    if ( v34 > robo->field_4FD )
                        v26++;
                    else if ( v34 < robo->field_4FD )
                        v66++;
                }

                if ( v26 )
                    v26 = (int)v60 / v26;

                if ( v66 )
                    v66 = (int)v60 / v66;

                if ( robo->field_501 & 1 )
                {
                    if ( bact->energy <= v34 )
                    {
                        bact->energy += v26;

                        if ( bact->energy >= v34 )
                            bact->energy = v34;
                    }
                    else
                    {
                        bact->energy -= v66;
                        if ( bact->energy <= v34 )
                            bact->energy = v34;
                    }
                }

                if ( robo->field_501 & 4 )
                {
                    if ( robo->field_4F5 <= v34 )
                    {
                        robo->field_4F5 += v26;
                        if ( robo->field_4F5 >= v34)
                            robo->field_4F5 = v34;
                    }
                    else
                    {
                        robo->field_4F5 -= v66;
                        if ( robo->field_4F5 <= v34 )
                            robo->field_4F5 = v34;
                    }
                }

                if ( robo->field_501 & 8 )
                {
                    if ( v34 >= robo->field_4FD )
                    {
                        robo->field_4FD += v26;
                        if ( robo->field_4FD >= v34)
                            robo->field_4FD = v34;
                    }
                    else
                    {
                        robo->field_4FD -= v66;
                        if ( robo->field_4FD <= v34 )
                            robo->field_4FD = v34;
                    }
                }

            }
        }
        else
        {
            robo->field_505 = v65 * v64 / 7000.0;
            float v71 = v70 / 7000.0;
            if ( bact->owner == bact->p_cell_area->owner )
            {
                robo->field_509 += v71 * 0.15;
                robo->field_50D += v71 * 0.7;
                bact->energy += v71 * 0.7;
            }
            else
            {
                bact->energy -= v71;
            }
        }

        if ( bact->energy < 0 )
            bact->energy = 0;

        if ( robo->field_4F5 < 0 )
            robo->field_4F5 = 0;

        if ( robo->field_4FD < 0 )
            robo->field_4FD = 0;

        if ( robo->field_509 < 0 )
            robo->field_509 = 0;

        if ( robo->field_50D < 0 )
            robo->field_50D = 0;

        if ( bact->energy_2 < bact->energy )
            bact->energy = bact->energy_2;

        if ( bact->energy_2 < robo->field_4F5 )
            robo->field_4F5 = bact->energy_2;

        if ( bact->energy_2 < robo->field_4FD )
            robo->field_4FD = bact->energy_2;

        if ( bact->energy_2 < robo->field_509 )
            robo->field_509 = bact->energy_2;

        if ( bact->energy_2 < robo->field_50D )
            robo->field_50D = bact->energy_2;

        robo->field_503 = 0;
        robo->field_502 = 0;

        if ( v62 < bact->energy )
            robo->field_502 |= 1;
        else if ( v62 > bact->energy )
            robo->field_503 |= 1;

        if ( v61 < robo->field_4F5 )
            robo->field_502 |= 4;
        else if ( v61 > robo->field_4F5 )
            robo->field_503 |= 4;

        if ( v59 < robo->field_4FD )
            robo->field_502 |= 8;
        else if ( v59 > robo->field_4FD )
            robo->field_503 |= 8;
    }
}

void yparobo_func96(NC_STACK_yparobo *obj, class_stru *zis, void *)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    call_parent(zis, obj, 96, 0);

    robo->field_221 = 0;
    robo->field_269 = 0;
    robo->field_251 = 0;
    robo->field_239 = 0;
    robo->field_285 = 0;
    robo->field_29D = 0;
    robo->field_2CD = 0;
    robo->field_2B5 = 0;
    robo->field_25D = 0;
    robo->field_22D = 0;
    robo->field_245 = 0;
    robo->field_291 = 0;
    robo->field_235 = 0;
    robo->field_281 = 0;
    robo->field_265 = 0;
    robo->field_24D = 0;
    robo->field_299 = 0;
    robo->field_2E1 = 0;
    robo->field_2C9 = 0;
    robo->field_2B1 = 0;
    robo->field_509 = 0;

    yw_130arg arg130;
    arg130.pos_x = 600.0;
    arg130.pos_z = -600.0;

    call_method(robo->wrld, 130, &arg130);

    robo->field_1EF = 0;
    robo->field_1F3 = 0;
    robo->field_1FB = 0;
    robo->field_20F = 0;
    robo->field_210 = 0;
    robo->field_211 = 0;
    robo->field_1FF.sx = 0;
    robo->field_1FF.sy = 0;
    robo->field_1FF.sz = 0;
    robo->field_20B = NULL;
    robo->field_309 = 0;
    robo->field_511 = 0;
    robo->field_5A9 = 0;
    robo->field_5A5 = 0;
    robo->field_1DB = 0;

    robo->pcell = arg130.pcell;

    robo->field_1FB = 0;

    memset(robo->guns, 0, sizeof(robo->guns));
    memset(robo->field_525, 0, sizeof(robo->field_525));

    robo->bact_internal->field_2E = dword_5B1128;

    dword_5B1128++;

    call_vtbl(obj, 2, 0x8000100B, 3000, 0);

    get_keyvalue_from_ini(NULL, yparobo_keys, 2);

    robo->field_1DF = yparobo_keys[0].value.val;
    robo->field_1E3 = yparobo_keys[1].value.val;
}

void yparobo_func97(NC_STACK_yparobo *obj, class_stru *zis, void *)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    robo->bact_internal->field_611 *= 0.8;
    robo->bact_internal->field_601 = 0;
    robo->field_c = robo->bact_internal->mass * 9.80665;
}

void yparobo_func114(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
}

void yparobo_func121(NC_STACK_yparobo *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;
    _NC_STACK_ypaworld *yw = robo->wrld_yw;

    int a4;
    call_vtbl(obj, 3, 0x8000100B, &a4, 0);

    if ( !(bact->field_3D6 & 0x800) )
    {
        bact_arg119 arg78;
        arg78.field_0 = 0;
        arg78.field_8 = 0;
        arg78.field_4 = 2048;
        call_method(obj, 78, &arg78);
    }

    bact->field_3D6 |= 0x200;

    if ( !bact->owner || !bact->vp_genesis.base )
    {
        if ( a4 > 0 )
            return;

        int v31;
        call_vtbl(obj, 3, 0x80001005, &v31, 0);

        if ( v31 )
            bact->field_3D6 |= 0x400000;
        else
            call_method(obj, 118, obj);

        return;
    }

    int v33 = (float)bact->energy_2 * 0.7;
    if ( v33 < 10000 )
        v33 = 10000;
    if ( v33 > 25000 )
        v33 = 25000;

    if ( bact->field_6BD[0].field_34 & 1 )
    {
        bact->field_931 -= arg->field_4;

        if ( bact->field_931 <= 0 )
        {
            bact->field_6BD[0].vp.base = NULL;
            bact->field_6BD[0].vp.trigo = NULL;

            if ( a4 > 0 )
                return;

            int v30;
            call_vtbl(obj, 3, 0x80001005, &v30, 0);

            if ( v30 )
                bact->field_3D6 |= 0x400000;
            else
                call_method(obj, 118, obj);

            return;
        }

        bact->field_6BD[0].field_0 = sqrt((float)bact->field_931 / (float)v33) * 0.75;

        if ( bact->field_6BD[0].field_0 < 0.0 )
            bact->field_6BD[0].field_0 = 0;

        float v16 = bact->maxrot * 2.0 * ((float)arg->field_4 / 1000.0);

        mat3x3 mat1, dst;

        mat1.m00 = cos(v16);
        mat1.m01 = 0;
        mat1.m02 = sin(v16);
        mat1.m10 = 0;
        mat1.m11 = 1.0;
        mat1.m12 = 0;
        mat1.m20 = -sin(v16);
        mat1.m21 = 0;
        mat1.m22 = cos(v16);

        mat_mult(&mat1, &bact->field_6BD[0].matrx, &dst);

        bact->field_6BD[0].matrx = dst;

        if ( a4 <= 0 )
            bact->field_3D6 |= 0x400000;
    }
    else
    {
        bact->field_931 = v33;
        bact->field_6BD[0].field_0 = 0.75;
        bact->field_6BD[0].field_4 = bact->field_621;
        bact->field_6BD[0].matrx = bact->field_651;
        bact->field_6BD[0].vp = bact->vp_genesis;
        bact->field_6BD[0].field_34 |= 3;

        if ( yw->field_757E )
        {
            char v21[76];
            /*
            *(_DWORD *)v21 = 1030;
            v21[12] = bact->owner;
            *(float *)&v21[20] = 0.75;
            *(_DWORD *)&v21[16] = v33;
            *(_DWORD *)&v21[24] = bact->ypabact__id;
            *(xyz *)&v21[28] = bact->field_621;
            v21[40] = bact->field_651; //matrix copy
            */
            yw_arg181 arg181;
            arg181.value = v21;
            arg181.field_18 = 1;
            arg181.field_10 = 0;
            arg181.field_14 = 2;
            arg181.val_size = 76;
            call_method(yw->self_full, 181, &arg181);
        }
    }
}

void yparobo_func128(NC_STACK_yparobo *obj, class_stru *zis, robo_arg128 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    arg->bacto = NULL;
    arg->bact = NULL;

    bact_node *node = (bact_node *)bact->list2.head;

    while ( node->next )
    {
        if ( !(node->bact->field_3D6 & 0x400) )
        {
            int v7;

            if ( arg->field_0 & 1 )
            {
                if ( arg->field_1C == node->bact->field_2E )
                {
                    arg->bact = node->bact;
                    arg->bacto = node->bacto;
                    v7 = 1;
                }
                else
                {
                    v7 = 0;
                }
            }
            else
            {
                v7 = 1;
            }

            int v21;
            if ( arg->field_0 & 2 )
            {
                if ( node->bact->field_3DE == arg->field_8 || robo->field_20F == arg->field_8 )
                {
                    int v28 = arg->field_C / 1200.0;
                    int v29 = (-arg->field_14) / 1200.0;

                    int v30, v31, v16;
                    if ( node->bact->field_2E == robo->field_1F7 && robo->field_1F7 )
                    {
                        v30 = robo->field_1FF.sx / 1200.0;
                        v31 = (-robo->field_1FF.sz) / 1200.0;
                        v16 = robo->field_211;
                    }
                    else
                    {
                        v30 = node->bact->field_3ec.sx / 1200.0;
                        v31 = (-node->bact->field_3ec.sz) / 1200.0;
                        v16 = node->bact->field_3e0;
                    }

                    if ( (arg->field_8 == 2 && !arg->field_18 && arg->field_4 == node->bact->field_3e8)
                            || (arg->field_8 == 2 && arg->field_18 != 0 && v16 == arg->field_18)
                            || (arg->field_8 == 1 && v30 == v28 && v31 == v29) )
                    {
                        arg->bact = node->bact;
                        arg->bacto = node->bacto;
                    }
                }
                v21 = 0;
            }
            else
            {
                v21 = 1;
            }

            int v26;
            if ( !(arg->field_0 & 4) )
            {
                v26 = 1;
            }
            else
            {
                if ( node->bact->field_3DE == 1 )
                {
                    float tx, ty;

                    if ( node->bact->field_2E == robo->field_1F7 && robo->field_1F7 )
                    {
                        tx = robo->field_1FF.sx;
                        ty = robo->field_1FF.sz;
                    }
                    else
                    {
                        tx = node->bact->field_3ec.sx;
                        ty = node->bact->field_3ec.sz;
                    }
                    if ( POW2(arg->field_20) > POW2(tx - arg->field_C) + POW2(ty - arg->field_14) )
                    {
                        arg->bact = node->bact;
                        arg->bacto = node->bacto;

                        v26 = 1;
                    }
                    else
                    {
                        v26 = 0;
                    }
                }
                else
                {
                    v26 = 0;
                }
            }

            if ( v7 && v21 && v26 )
                break;
        }

        node = (bact_node *)node->next;
    }
}

void yparobo_func129(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func129");
    call_parent(zis, obj, 129, arg);
}

void yparobo_func130(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func130");
    call_parent(zis, obj, 130, arg);
}

void yparobo_func131(NC_STACK_yparobo *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","yparobo_func131");
    call_parent(zis, obj, 131, arg);
}

size_t yparobo_func132(NC_STACK_yparobo *obj, class_stru *zis, bact_arg67 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;
    __NC_STACK_ypabact *bact = robo->bact_internal;

    if ( !arg->field_4 )
    {
        arg->field_0 = 0;
        return 0;
    }


    bact_node *node = &bact->list_node;

    while (node->prev)
        node = (bact_node *)node->prev;

    node = (bact_node *)node->next;

    while (node->next)
    {
        if (arg->field_4 == node->bact->field_2E)
        {
            if ( node->bact->field_3D5 != 2 )
            {
                arg->field_4 = 0;
                arg->field_0 = 2;
                arg->field_8 = node->bact;
                return 1;
            }
            else
            {
                arg->field_4 = 0;
                arg->field_0 = 0;
                return 0;
            }
        }

        bact_node *subnode = (bact_node *)node->bact->list2.head;

        while (subnode->next)
        {
            if (arg->field_4 == subnode->bact->field_2E)
            {
                if ( subnode->bact->field_3D5 != 2 )
                {
                    arg->field_4 = 0;
                    arg->field_0 = 2;
                    arg->field_8 = subnode->bact;
                    return 1;
                }
                else
                {
                    arg->field_4 = 0;
                    arg->field_0 = 0;
                    return 0;
                }
            }

            subnode = (bact_node *)subnode->next;
        }

        node = (bact_node *)node->next;
    }

    arg->field_0 = 0;
    arg->field_4 = 0;
    return 0;
}

// Create squad for robo
int yparobo_func133(NC_STACK_yparobo *obj, class_stru *zis, robo_arg133 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    int curid = 0;
    int col = sqrt(arg->num) + 2;

    //call_vtbl(robo->wrld, 3, 0x80002014, &vhclprotos, 0);

    ypaworld_arg146 arg146;
    arg146.pos.sy = arg->pos.sy;
    arg146.pos.sx = 100 * (curid % col - col / 2) + arg->pos.sx;
    arg146.pos.sz = 100 * (curid / col) + arg->pos.sz;

    if ( arg->hetero_vehicles )
        arg146.vehicle_id = arg->hetero_vehicles[0];
    else
        arg146.vehicle_id = arg->type;

    NC_STACK_ypabact *squad_commander = (NC_STACK_ypabact *)call_method(robo->wrld, 146, &arg146); //Create first bact

    if ( !squad_commander )
        return 0;

    __NC_STACK_ypabact *bact_intern;

    call_vtbl(squad_commander, 3, 0x80001003, &bact_intern, 0);

    int v32;
    call_vtbl(obj, 3, 0x80001007, &v32, 0); //getflag
    call_vtbl(squad_commander, 2, 0x80001007, v32, 0); //setflag

    if ( arg->field_14 & 1 )
        bact_intern->field_3D6 |= 0x80000;

    bact_intern->owner = robo->bact_internal->owner;
    bact_intern->field_2E = dword_5B1128;
    bact_intern->field_32 = obj;
    bact_intern->field_3D4 = 60;

    bact_arg119 arg78;
    arg78.field_0 = 1;
    arg78.field_4 = 0;
    arg78.field_8 = 0;

    call_method(squad_commander, 78, &arg78);

    bact_arg67 arg67;
    arg67.field_0 = 1;
    arg67.field_4 = 0;
    arg67.targ.sx = arg146.pos.sx;
    arg67.targ.sz = arg146.pos.sz;

    call_method(squad_commander, 67, &arg67); //Set target

    for ( curid = 1; curid < arg->num; curid++)
    {
        if ( arg->hetero_vehicles ) // there must be a joke about gay and straight
            arg146.vehicle_id = arg->hetero_vehicles[arg->num - curid];
        else
            arg146.vehicle_id = arg->type;

        arg146.pos.sx = 100 * (curid % col - col / 2) + arg->pos.sx;
        arg146.pos.sz = 100 * (curid / col) + arg->pos.sz;

        NC_STACK_ypabact *next_bact = (NC_STACK_ypabact *)call_method(robo->wrld, 146, &arg146);

        if ( !next_bact )
            break;

        call_method(squad_commander, 72, next_bact); // Add to squad commander list

        arg67.targ.sx = arg146.pos.sx;
        arg67.targ.sz = arg146.pos.sz;
        call_method(next_bact, 67, &arg67); //Set target

        call_vtbl(next_bact, 3, 0x80001003, &bact_intern, 0);

        call_vtbl(next_bact, 2, 0x80001007, v32, 0);

        if ( arg->field_14 & 1 )
            bact_intern->field_3D6 |= 0x80000;

        bact_intern->owner = robo->bact_internal->owner;
        bact_intern->field_2E = dword_5B1128;
        bact_intern->field_32 = obj;
        bact_intern->field_3D4 = 60;

        call_method(next_bact, 78, &arg78);
    }

    dword_5B1128++;

    call_method(obj, 72, squad_commander);  //Add squad commander into robo list

    return 1;
}


const char * yparobo_func134__sub0(char **strings, int a2)
{
    const char *result;

    switch ( a2 )
    {
    case 2:
        result = get_lang_string(strings, 202, "SECTOR CONQUERED");
        break;

    case 6:
        result = get_lang_string(strings, 206, "FOUND ENEMY STATION");
        break;

    case 11:
        result = get_lang_string(strings, 211, "STATION IS DEAD ");
        break;

    case 12:
        result = get_lang_string(strings, 212, "YOUR STATION IS DEAD!");
        break;

    case 13:
        result = get_lang_string(strings, 213, "ENEMY NEAR STATION!");
        break;

    case 18:
        result = get_lang_string(strings, 218, "REQUEST SUPPORT");
        break;

    case 31:
        result = get_lang_string(strings, 231, "FLAK DESTROYED");
        break;

    case 32:
        result = get_lang_string(strings, 232, "RADAR STATION DESTROYED");
        break;

    case 33:
        result = get_lang_string(strings, 233, "ATTACK POWERSTATION");
        break;

    case 34:
        result = get_lang_string(strings, 234, "ATTACK FLAK");
        break;

    case 35:
        result = get_lang_string(strings, 235, "ATTACK RADAR STATION");
        break;

    case 40:
        result = get_lang_string(strings, 240, "HOST ENERGY CRITICAL");
        break;

    default:
        result = NULL;
        break;
    }
    return result;
}

int yparobo_func134__sub1(__NC_STACK_yparobo *robo, robo_arg134 *arg)
{
    int v2 = 0;

    __NC_STACK_ypabact *bact = robo->bact_internal;

    switch(arg->field_4)
    {
    case 1:
    {
        NC_STACK_ypabact *bacto;
        __NC_STACK_ypabact *v11;
        call_vtbl(robo->wrld, 3, 0x80002011, &bacto, 0);
        call_vtbl(bacto, 3, 0x80001003, &v11, 0);

        if ( v11->field_24 != 4 )
            v2 = 1;
        else
            v2 = 0;
    }
    break;

    case 2:
    case 3:
    case 4:
        if ( arg->field_8 == stru_5B0628[ arg->field_4 ].field_0 && arg->field_C == stru_5B0628[ arg->field_4 ].field_4 )
            v2 = 0;
        else
            v2 = 1;
        break;

    case 6:
        v2 = (robo->bact_internal->field_915 - dword_515138[ arg->field_8 ] > 45000) || dword_515138[ arg->field_8 ] == 0;
        dword_515138[ arg->field_8 ] = robo->bact_internal->field_915;
        break;

    case 7:
        v2 = 1;
        if ( arg->unit && arg->unit->field_24 == 9 )
        {
            int a4 = 0;
            call_vtbl(arg->unit->self, 3, 0x80002006, &a4, 0);

            if ( a4 )
                v2 = 0;
        }

        break;

    case 9:
    case 10:
    case 13:
        if ( arg->field_8 == stru_5B0628[arg->field_4].field_0 )
            v2 = 0;
        else
            v2 = 1;
        break;

    case 5:
    case 8:
    case 11:
    case 12:
    case 14:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
        v2 = 1;
        break;

    case 18:
    case 40:
        if ( (bact->field_915 - stru_5B0628[ arg->field_4 ].field_10) > arg->field_8 )
        {
            stru_5B0628[ arg->field_4 ].field_10 = bact->field_915;
            v2 = 1;
        }
        else
            v2 = 0;

        break;

    default:
        break;
    }

    if ( v2 )
    {
        stru_5B0628[arg->field_4].field_0 = arg->field_8;
        stru_5B0628[arg->field_4].field_4 = arg->field_C;
        stru_5B0628[arg->field_4].field_8 = arg->field_10;
    }

    return v2;
}

int yparobo_func134(NC_STACK_yparobo *obj, class_stru *zis, robo_arg134 *arg)
{
    __NC_STACK_yparobo *robo = &obj->stack__yparobo;

    if ( arg->unit && arg->unit->field_24 == 9 && arg->field_4 != 7 && arg->field_4 != 19 && arg->field_4 != 6 )
        return 0;

    char **strngs;
    call_vtbl(robo->wrld, 3, 0x80002018, &strngs, 0);

    if ( !yparobo_func134__sub1(robo, arg) )
        return 0;

    yw_arg159 v8;
    v8.field_4 = arg->field_14;
    v8.txt = yparobo_func134__sub0(strngs, arg->field_4);
    v8.unit = arg->unit;
    v8.field_C = arg->field_4;
    call_method(robo->wrld, 159, &v8);

    return 1;
}




class_return yparobo_class_descr;

class_return * class_set_yparobo(int, ...)
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

















void ypabact_func65__sub0(__NC_STACK_ypabact *bact) // This bact is robo!
{
    _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

    if ( bact->field_959 )
    {
        if ( bact->field_915 - bact->field_959 > 2000 )
        {
            int i = 0;

            __NC_STACK_ypabact *v1 = (__NC_STACK_ypabact *)bact->p_cell_area->field_3C.head;

            while ( v1->next )
            {
                int a4;
                call_vtbl(v1->self, 3, 0x80001004, &a4, 0);

                if ( v1->field_3D5 == 5 && v1->owner == bact->owner && !a4 )
                    i = 1;

                v1 = (__NC_STACK_ypabact *)v1->next;
            }

            if ( !i )
            {
                NC_STACK_yparobo *robj = (NC_STACK_yparobo *)&bact->self;
                __NC_STACK_yparobo *robo = &robj->stack__yparobo;

                __NC_STACK_ypabact *tmp = bact;

                call_method(bact->wrld, 168, &tmp);

                xyz tt = bact->field_621;

                bact->field_621 = bact->field_62D;
                robo->field_1D5 = bact->field_62D.sy;

                char a1a[200];
                sprintf(a1a, "save:%s/%d.fin", yw->GameShell->user_name, yw->field_2d90->levelID);

                yw_arg170 v23;
                v23.usr = yw->GameShell;
                v23.pbuf = a1a;

                if ( !call_method(bact->wrld, 170, &v23) )
                    ypa_log_out("Warning, final sgm save error\n");

                bact->field_621 = tt;

                if ( yw->GameShell )
                {
                    char v18[300];
                    sprintf(v18, "save:%s/%d.rst", yw->GameShell->user_name, yw->field_2d90->levelID);

                    delete_file(v18);
                }
                bact->field_3D6 |= 0x2000000;

                call_method(bact->self, 77, 0);
            }

        }
    }
    else
    {

        bact->field_621.sx = (bact->field_c + 0.5) * 1200.0;
        bact->field_621.sz = -(bact->field_10 + 0.5) * 1200.0;

        bact_node *unit = (bact_node *)bact->list2.head;

        while ( unit->next )
        {
            unit->bact->field_3D6 |= 0x2000000;

            bact_node *sub_unit = (bact_node *)unit->bact->list2.head;

            while ( sub_unit->next )
            {
                sub_unit->bact->field_3D6 |= 0x2000000;

                sub_unit = (bact_node *)sub_unit->next;
            }

            unit = (bact_node *)unit->next;
        }

        __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)bact->p_cell_area->field_3C.head;


        yw->field_2d90->buddies_count = 0;
        memset(yw->field_2d90->buddies, 0, sizeof(yw->field_2d90->buddies));

        int v15 = 0;

        while ( bct->next )
        {
            if  ( bct->field_3D5 != 2 )
            {
                if ( bct->field_24 != 3 && bct->field_24 != 4 && bct->field_24 != 9 && bct->owner == bact->owner )
                {
                    float v17 = fabs(bct->field_621.sx - (bact->field_c + 0.5) * 1200.0);
                    float v29 = fabs(bct->field_621.sz + (bact->field_10 + 0.5) * 1200.0);

                    if ( v17 <= 450.0 || v29 <= 450.0 )
                    {
                        bct->field_3D5 = 5;

                        bct->field_931 = 0;

                        bct->field_a28 = v15 + 1000;
                        v15 += 400;
                    }
                }
            }

            bct = (__NC_STACK_ypabact *)bct->next;
        }

        bact->field_959 = bact->field_915;
    }
}
