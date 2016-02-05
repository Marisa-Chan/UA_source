#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "yw.h"
#include "ypabact.h"


stored_functions *classvtbl_get_ypabact();
class_return * class_set_ypabact(int, ...);

stored_functions ypabact_class_vtbl(class_set_ypabact);


class_stored ypabact_class_off (NULL, NULL, "MC2classes:ypabact.class", classvtbl_get_ypabact);


stored_functions *classvtbl_get_ypabact()
{
    return &ypabact_class_vtbl;
}

CLASSFUNC ypabact_funcs[1024];

int ypabact_id;
char **dword_5490B0; // ypaworld strings


int ypabact_func0__sub0(NC_STACK_ypabact *obj, __NC_STACK_ypabact *bact, stack_vals *stak)
{
    bact->mass = 400.0;
    bact->force = 5000.0;
    bact->airconst = 500.0;
    bact->maxrot = 0.5;
    bact->height = 150.0;
    bact->radius = 20.0;
    bact->vwr_radius = 40.0;
    bact->overeof = 10.0;
    bact->vwr_overeof = 40.0;
    bact->energy = 10000;
    bact->shield = 0;
    bact->field_9AD = 0.7;
    bact->field_B74 = 3000;
    bact->field_3D4 = 50;
    bact->energy_2 = 10000;
    bact->field_3CE = 0;
    bact->field_679 = 1600.0;
    bact->gun_radius = 5.0;
    bact->gun_power = 4000.0;
    bact->adist_sector = 800.0;
    bact->adist_bact = 650.0;
    bact->sdist_sector = 200.0;
    bact->sdist_bact = 100.0;
    bact->field_B34 = 4;

    NC_STACK_ypaworld *ywo = (NC_STACK_ypaworld *)find_id_in_stack_def_val(0x80001001, 0, stak);// get ypaworld
    bact->wrld = ywo;

    if ( ywo )
    {
        _NC_STACK_ypaworld *yw = &ywo->stack__ypaworld;
        bact->field_B3C = yw;

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
                    printf("MAKE ME %s\n", "ypabact_func0__sub0  case 0x80001004");

                    if ( stk->value )
                    {
                        call_method(ywo, 131, bact);

                        bact->field_B34 |= 1;

                        /*if ( yw->field_757E )
                          v23 = 1;

                        sub_423F74(&bact->field_5A, 8);*/
                    }
                    else
                    {
                        bact->field_B34 &= 0xFFFFFFFE;

                        /*if ( yw->field_757E )
                          v23 = 0;

                        sub_424000(&bact->field_5A, 8);*/
                    }
//
//            if ( yw->field_757E )
//            {
//                yw_arg181_a v14;
//
//                v14.fld_0 = 1014;
//                v14.fld_3 = bact->owner;
//                v14.fld_4_1 = bact->ypabact__id;
//                v14.fld_4_2 = 0;
//
//              if ( bact->field_24 == 4 )
//              {
//                call_vtbl(obj, 3, 0x80002000, &a4, 0);
//                v21 = *(_DWORD *)(a4 + 40);
//              }
//
//              yw_arg181 v13;
//
//              v13.field_10 = 0;
//              v13.field_14 = 2;
//              v13.field_0 = &v14;
//              v13.field_18 = 1;
//              v13.field_4 = 28;
//              call_method(ywo, 181, &v13);
//            }
                    break;

                case 0x80001005:
                    if ( stk->value )
                    {
                        bact->field_B34 |= 2;
                        call_vtbl(ywo, 2, 0x80002011, obj, 0);
                    }
                    else
                    {
                        bact->field_B34 &= 0xFFFFFFFD;
                    }
                    break;

                case 0x80001007:
                    if ( stk->value )
                        bact->field_B34 |= 8;
                    else
                        bact->field_B34 &= 0xFFFFFFF7;
                    break;

                case 0x80001006:
                    if ( stk->value )
                        bact->field_B34 |= 4;
                    else
                        bact->field_B34 &= 0xFFFFFFFB;
                    break;

                case 0x8000100A:
                    if ( stk->value )
                        bact->field_B34 |= 0x10;
                    else
                        bact->field_B34 &= 0xFFFFFFEF;
                    break;

                case 0x80001009:
                    bact->airconst = stk->value; // CHECK int or float
                    bact->airconst2 = stk->value;
                    break;

                case 0x8000100B:
                    bact->field_B74 = stk->value;
                    break;

                case 0x8000100D:
//            v12 = bact->list2.head;
//            for ( bact->field_3D4 = (char)stk->value; v12->next; v12 = v12->next )
//              LOBYTE(v12[1].prev[122].prev) = (_BYTE)stk->value;
                    break;

                case 0x8000100F:
                    bact->current_vp.trigo = (base_1c_struct *)stk->value;
                    break;

                case 0x8000100C:
                    bact->current_vp.base = (NC_STACK_base *)stk->value;
                    break;

                case 0x80001010:
                    if ( stk->value )
                        bact->field_B34 |= 0x20;
                    else
                        bact->field_B34 &= 0xFFFFFFDF;
                    break;

                case 0x80001013:
                    if ( stk->value )
                        bact->field_B34 |= 0x40;
                    else
                        bact->field_B34 &= 0xFFFFFFBF;
                    break;

                }
                stk++;
            }
        }
    }

    return 1;
}


NC_STACK_ypabact * ypabact_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_ypabact *obj = (NC_STACK_ypabact *)call_parent(zis, clss, 0, stak);

    if ( !obj )
        return NULL;

    __NC_STACK_ypabact *bact_int = &obj->stack__ypabact;

    init_list(&bact_int->field_B48);
    init_list(&bact_int->list2);
    init_list(&bact_int->list3);

    bact_int->ypabact__id = ypabact_id;
    bact_int->field_24 = 1;
    bact_int->field_3DA = 0;
    bact_int->field_32 = 0;
    bact_int->field_5C9.m00 = 1.0;
    bact_int->field_5C9.m01 = 0;
    bact_int->field_5C9.m02 = 0;
    bact_int->field_5C9.m10 = 0;
    bact_int->field_5C9.m11 = 1.0;
    bact_int->field_5C9.m12 = 0;
    bact_int->field_5C9.m20 = 0;
    bact_int->field_5C9.m21 = 0;
    bact_int->field_5C9.m22 = 1.0;
    bact_int->field_605 = 0;
    bact_int->field_609 = 0;
    bact_int->field_60D = 0;
    bact_int->field_611 = 0;
    bact_int->field_639 = 0;
    bact_int->field_63D = 0;
    bact_int->field_641 = 0;
    bact_int->field_B54.bacto = obj;
    bact_int->field_B64.bacto = obj;

    bact_int->list_node.bacto = obj;

    bact_int->field_B54.bact = bact_int;

    bact_int->field_B64.bact = bact_int;

    ypabact_id++;

    bact_int->field_651 = bact_int->field_5C9;

    bact_int->list_node.bact = bact_int;

    bact_int->self = obj;

    if ( !ypabact_func0__sub0(obj, bact_int, stak) )
    {
        call_method(obj, 1);
        return NULL;
    }

    bact_int->field_87D.grp_1 = bact_int->field_621;

    bact_int->field_87D.scale_rotation = bact_int->field_651;

    bact_int->field_3D5 = 1;

    int secMaxX, secMaxY;

    call_vtbl(bact_int->wrld, 3, 0x80002002, &secMaxX, 0);
    call_vtbl(bact_int->wrld, 3, 0x80002003, &secMaxY, 0);

    bact_int->field_18 =  secMaxX * 1200.0;
    bact_int->field_1c = -secMaxY * 1200.0;
    bact_int->field_20 = secMaxX;
    bact_int->field_22 = secMaxY;

    call_vtbl(bact_int->wrld, 3, 0x80002018, &dword_5490B0, 0);

    return obj;
}

size_t ypabact_func1(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}



int ypabact_func2__sub0(NC_STACK_ypabact *obj, __NC_STACK_ypabact *bact, stack_vals *stak)
{
    printf("MAKE ME %s\n", "ypabact_func2__sub0");



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
                {
                    call_method(bact->wrld, 131, bact);

                    bact->field_B34 |= 1;

                }
                else
                {
                    bact->field_B34 &= 0xFFFFFFFE;

                }
                break;

            }
            stk++;
        }
    }

    return 1;
}

void ypabact_func2(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 2, stak);

    ypabact_func2__sub0(obj, &obj->stack__ypabact, stak);
}

void ypabact_func3__sub0(NC_STACK_ypabact *obj, __NC_STACK_ypabact *bact, stack_vals *stak)
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

            case 0x80001002:
                *(base_1c_struct **)stk->value = &bact->field_87D;
                break;

            case 0x80001003:
                *(__NC_STACK_ypabact **)stk->value = bact;
                break;

            case 0x80001004:
                *(int *)stk->value = (bact->field_B34 & 1) != 0;
                break;

            case 0x80001005:
                *(int *)stk->value = (bact->field_B34 & 2) != 0;
                break;

            case 0x80001006:
                *(int *)stk->value = (bact->field_B34 & 4) != 0;
                break;

            case 0x80001007:
                *(int *)stk->value = (bact->field_B34 & 8) != 0;
                break;

            case 0x8000100A:
                *(int *)stk->value = (bact->field_B34 & 0x10) != 0;
                break;

            case 0x80001001:
                *(NC_STACK_ypaworld **)stk->value = bact->wrld;
                break;

            case 0x80001008:
                *(nlist **)stk->value = &bact->field_B48;
                break;

            case 0x8000100B:
                *(int *)stk->value = bact->field_B74;
                break;

            case 0x8000100C:
                *(NC_STACK_base **)stk->value = bact->current_vp.base;
                break;

            case 0x8000100D:
                *(int *)stk->value = bact->field_3D4;
                break;

            case 0x8000100E:
                *(int *)stk->value = 0;
                break;

            case 0x8000100F:
                *(base_1c_struct **)stk->value = bact->current_vp.trigo;
                break;

            case 0x80001010:
                *(int *)stk->value = (bact->field_B34 & 0x20) != 0;
                break;

            case 0x80001011:
                *(bact_node **)stk->value = &bact->field_B54;
                break;

            case 0x80001012:
                *(bact_node **)stk->value = &bact->field_B64;
                break;

            case 0x80001013:
                *(int *)stk->value = (bact->field_B34 & 0x40) != 0;
                break;

            }
            stk++;
        }
    }
}

void ypabact_func3(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);

    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    ypabact_func3__sub0(obj, bact, stak);
}






void sub_481F14(__NC_STACK_ypabact *bact)
{
    ypaworld_arg136 arg136;
    arg136.pos_x = bact->field_621.sx;
    arg136.pos_y = -30000.0;
    arg136.pos_z = bact->field_621.sz;
    arg136.field_14 = 0;
    arg136.field_1C = 0;
    arg136.field_18 = 50000.0;
    arg136.field_40 = 0;

    call_method(bact->wrld, 136, &arg136);

    if ( arg136.field_20 )
        bact->field_621.sy = arg136.field_30 - 50.0;
    else
        bact->field_621.sy = bact->p_cell_area->sector_height_meters  - 50.0;
}


void sub_481E0C(__NC_STACK_ypabact *bact)
{
    int maxX, maxY;

    call_vtbl(bact->wrld, 3, 0x80002002, &maxX, 0);
    call_vtbl(bact->wrld, 3, 0x80002003, &maxY, 0);

    float mx = maxX * 1200.0;
    float my = -(maxY * 1200.0);

    if ( bact->field_621.sx > mx )
        bact->field_621.sx = mx - 600.0;

    if ( bact->field_621.sx < 0.0 )
        bact->field_621.sx = 600.0;

    if ( bact->field_621.sz < my )
        bact->field_621.sz = my + 600.0;

    if ( bact->field_621.sz > 0.0 )
        bact->field_621.sz = -600.0;

    sub_481F14(bact);
}

void sub_481F94(__NC_STACK_ypabact *bact)
{
    printf("MAKE ME %s\n", "sub_481F94");
}

void ypabact_func65__sub0(__NC_STACK_ypabact *bact)
{
    printf("MAKE ME %s\n", "ypabact_func65__sub0");
}



base_1c_struct bact_cam;

void ypabact_func65(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    sub_430A20(&bact_cam);

    yw_130arg sect_info;
    sect_info.pos_x = bact->field_621.sx;
    sect_info.pos_z = bact->field_621.sz;

    if ( !call_method(bact->wrld, 130, &sect_info) )
    {
        sub_481E0C(bact);

        sect_info.pos_x = bact->field_621.sx;
        sect_info.pos_z = bact->field_621.sz;

        call_method(bact->wrld, 130, &sect_info);
    }

    cellArea *oldcell = bact->p_cell_area;

    bact->field_c = sect_info.sec_x;
    bact->field_10 = sect_info.sec_z;

    bact->pos_x_cntr = sect_info.pos_x_cntr;
    bact->pos_y_cntr = sect_info.pos_y_cntr;

    bact->p_cell_area = sect_info.pcell;

    if ( oldcell != sect_info.pcell ) // If cell changed
    {
        Remove(bact);  //Remove unit from old cell
        AddTail(&bact->p_cell_area->field_3C, bact);  // Add unit to new cell
    }

    if ( bact->p_cell_area->sector_height_meters + 1000.0 < bact->field_621.sy )
        sub_481F14(bact);

    NC_STACK_ypabact *retbact;
    call_vtbl(bact->wrld, 3, 0x80002010, &retbact, 0);

    if ( bact->p_cell_area->field_3A == 6 && bact->field_24 == 3 && retbact == bact->self )
        ypabact_func65__sub0(bact);

    if ( !(bact->field_3D6 & 0x400) && bact->energy <= 0 && bact->field_24 != 4 )
    {
        call_method(obj, 77, 0);

        if ( !call_method(obj, 100, 0) )
        {
            bact_arg119 v38;
            v38.field_4 = 0;
            v38.field_8 = 0;
            v38.field_0 = 3;

            call_method(obj, 78, &v38);
        }

        bact->field_3D5 = 2;
        bact->field_3D6 &= 0xFFFFFDFF;
    }

    bact->field_915 += arg->field_4;

    call_method(obj, 68, arg);

    bact_node *vnod = (bact_node *)bact->list3.head;
    while(vnod->next)
    {
        call_method(vnod->bacto, 65, arg);
        vnod = (bact_node *)vnod->next;
    }

    sub_481F94(bact);

    if ( bact->field_B34 & 1 )
    {
        if ( bact->field_B34 & 0x20 )
        {
            bact_cam.grp_1.sx = bact->field_621.sx + bact->field_651.m00 * bact->field_5BD + bact->field_651.m10 * bact->field_5C1 + bact->field_651.m20 * bact->field_5C5;
            bact_cam.grp_1.sy = bact->field_621.sy + bact->field_651.m01 * bact->field_5BD + bact->field_651.m11 * bact->field_5C1 + bact->field_651.m21 * bact->field_5C5;
            bact_cam.grp_1.sz = bact->field_621.sz + bact->field_651.m02 * bact->field_5BD + bact->field_651.m12 * bact->field_5C1 + bact->field_651.m22 * bact->field_5C5;

        }
        else
        {
            bact_cam.grp_1.sx = bact->field_621.sx;
            bact_cam.grp_1.sy = bact->field_621.sy;
            bact_cam.grp_1.sz = bact->field_621.sz;
        }

        if ( bact->field_B34 & 0x20 )
            bact_cam.scale_rotation = bact->field_5C9;
        else
            bact_cam.scale_rotation = bact->field_651;
    }

    bact->field_87D.grp_1 = bact->field_621;

    if ( bact->field_3D6 & 0x100000 )
    {
        bact->field_87D.scale_rotation.m00 = bact->field_651.m00 * bact->field_67D;
        bact->field_87D.scale_rotation.m01 = bact->field_651.m10 * bact->field_681;
        bact->field_87D.scale_rotation.m02 = bact->field_651.m20 * bact->field_685;
        bact->field_87D.scale_rotation.m10 = bact->field_651.m01 * bact->field_67D;
        bact->field_87D.scale_rotation.m11 = bact->field_651.m11 * bact->field_681;
        bact->field_87D.scale_rotation.m12 = bact->field_651.m21 * bact->field_685;
        bact->field_87D.scale_rotation.m20 = bact->field_651.m02 * bact->field_67D;
        bact->field_87D.scale_rotation.m21 = bact->field_651.m12 * bact->field_681;
        bact->field_87D.scale_rotation.m22 = bact->field_651.m22 * bact->field_685;
    }
    else
    {
        bact->field_87D.scale_rotation.m00 = bact->field_651.m00;
        bact->field_87D.scale_rotation.m01 = bact->field_651.m10;
        bact->field_87D.scale_rotation.m02 = bact->field_651.m20;
        bact->field_87D.scale_rotation.m10 = bact->field_651.m01;
        bact->field_87D.scale_rotation.m11 = bact->field_651.m11;
        bact->field_87D.scale_rotation.m12 = bact->field_651.m21;
        bact->field_87D.scale_rotation.m20 = bact->field_651.m02;
        bact->field_87D.scale_rotation.m21 = bact->field_651.m12;
        bact->field_87D.scale_rotation.m22 = bact->field_651.m22;
    }

    int numbid = arg->numid;

    arg->numid = 0;

    bact_node *bnod = (bact_node *)bact->list2.head;

    while (bnod->next)
    {
        call_method(bnod->bacto, 65, arg);

        arg->numid++;

        bnod = (bact_node *)bnod->next;
    }

    arg->numid = numbid;

    bact->field_5A.field_0 = bact->field_621;

    if ( bact->field_B34 & 1 )
    {
        bact->field_5A.field_0.sy = bact->field_651.m11 * 400.0 + bact->field_5A.field_0.sy;
        bact->field_5A.field_0.sz = bact->field_651.m12 * 400.0 + bact->field_5A.field_0.sz;
        bact->field_5A.field_0.sx = bact->field_651.m10 * 400.0 + bact->field_5A.field_0.sx;
    }

    bact->field_5A.field_C  = bact->field_605 * bact->field_611;
    bact->field_5A.field_10 = bact->field_609 * bact->field_611;
    bact->field_5A.field_14 = bact->field_60D * bact->field_611;

    sb_0x4242e0(&bact->field_5A);
}

void ypabact_func66(NC_STACK_ypabact *obj, class_stru *zis, base77Func *arg)
{
    printf("MAKE ME %s\n","ypabact_func66");

    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->current_vp.base )
    {
        if ( !(bact->field_3D6 & 0x400000) )
        {
            if ( !(bact->field_B34 & 1) || bact->field_B34 & 0x40 )
            {
                bact->current_vp.trigo->grp_1 = bact->field_87D.grp_1;
                bact->current_vp.trigo->scale_rotation = bact->field_87D.scale_rotation;

                arg->field_1C = bact->ypabact__id;
                call_method(bact->current_vp.base, 77, arg);
            }
        }
    }

}

void ypabact_func67(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func67");
    call_parent(zis, obj, 67, arg);
}

void ypabact_func68(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func68");
    call_parent(zis, obj, 68, arg);
}

void ypabact_func69(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func69");
    call_parent(zis, obj, 69, arg);
}

void ypabact_func70(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func70");
    call_parent(zis, obj, 70, arg);
}

void ypabact_func71(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func71");
    call_parent(zis, obj, 71, arg);
}

void ypabact_func72(NC_STACK_ypabact *obj, class_stru *zis, NC_STACK_ypabact *kid)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact_arg73 arg73;

    arg73.bacto = obj;
    arg73.bact = bact;
    arg73.list = &bact->list2;

    call_method(kid, 73, &arg73);
}

void ypabact_func73(NC_STACK_ypabact *obj, class_stru *zis, bact_arg73 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->parent_bacto )
        Remove(&bact->list_node);

    AddHead(arg->list, &bact->list_node);

    bact->parent_bacto = arg->bacto;
    bact->parent_bact = arg->bact;
}

void ypabact_func74(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func74");
    call_parent(zis, obj, 74, arg);
}

void ypabact_func75(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func75");
    call_parent(zis, obj, 75, arg);
}

void ypabact_func76(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func76");
    call_parent(zis, obj, 76, arg);
}

void ypabact_func77(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func77");
    call_parent(zis, obj, 77, arg);
}

void ypabact_func78(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func78");
    call_parent(zis, obj, 78, arg);
}

void ypabact_func79(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func79");
    call_parent(zis, obj, 79, arg);
}

size_t ypabact_func80(NC_STACK_ypabact *obj, class_stru *zis, bact_arg80 *arg)
{
    __NC_STACK_ypabact *bct = &obj->stack__ypabact;

    yw_130arg sect_info;

    sect_info.pos_x = arg->pos.sx;
    sect_info.pos_z = arg->pos.sz;
    if (!call_method(bct->wrld, 130, &sect_info))
        return 0;

    if ( bct->p_cell_area )
        Remove(bct);

    AddTail(&sect_info.pcell->field_3C, bct);

    bct->p_cell_area = sect_info.pcell;
    bct->field_62D.sx = arg->pos.sx;
    bct->field_621.sx = arg->pos.sx;
    bct->field_62D.sy = arg->pos.sy;
    bct->field_621.sy = arg->pos.sy;
    bct->field_62D.sz = arg->pos.sz;
    bct->field_621.sz = arg->pos.sz;
    bct->field_c = sect_info.sec_x;
    bct->field_10 = sect_info.sec_z;

    if ( !(arg->field_C & 2) )
        call_method(obj, 115);

    return 1;
}

void ypabact_func81(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func81");
    call_parent(zis, obj, 81, arg);
}

void ypabact_func82(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func82");
    call_parent(zis, obj, 82, arg);
}

void ypabact_func83(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func83");
    call_parent(zis, obj, 83, arg);
}

void ypabact_func84(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func84");
    call_parent(zis, obj, 84, arg);
}

void ypabact_func85(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func85");
    call_parent(zis, obj, 85, arg);
}

void ypabact_func86(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func86");
    call_parent(zis, obj, 86, arg);
}

void ypabact_func87(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func87");
    call_parent(zis, obj, 87, arg);
}

void ypabact_func88(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func88");
    call_parent(zis, obj, 88, arg);
}

void ypabact_func89(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func89");
    call_parent(zis, obj, 89, arg);
}

void ypabact_func90(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func90");
    call_parent(zis, obj, 90, arg);
}

void ypabact_func91(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func91");
    call_parent(zis, obj, 91, arg);
}

void ypabact_func92(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func92");
    call_parent(zis, obj, 92, arg);
}

void ypabact_func93(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func93");
    call_parent(zis, obj, 93, arg);
}

void ypabact_func94(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func94");
    call_parent(zis, obj, 94, arg);
}

void ypabact_func95(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func95");
    call_parent(zis, obj, 95, arg);
}

// Reset
void ypabact_func96(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact * bact = &obj->stack__ypabact;

    bact->field_B34 = 4;
    bact->field_3D6 = 0;
    bact->field_32 = 0;
    bact->field_B74 = 3000;
    bact->field_3DE = 0;

    bact->field_3DF = 0;
    bact->field_3e0 = 0;

    int maxX, maxY;

    call_vtbl(bact->wrld, 3, 0x80002002, &maxX, 0);
    call_vtbl(bact->wrld, 3, 0x80002003, &maxY, 0);
    bact->field_18 = maxX * 1200.0;
    bact->field_1c = -maxY * 1200.0;
    bact->field_20 = maxX;
    bact->field_22 = maxY;

    bact->field_2E = 0;
    bact->field_3D1 = 1;
    bact->field_9B1 = 0;
    bact->field_935 = 0;
    bact->field_939 = 0;
    bact->field_961 = 0;
    bact->field_965 = 0;
    bact->field_969 = 0;
    bact->field_995 = 0;
    bact->field_931 = 0;
    bact->field_915 = 0;
    bact->field_919 = 0;
    bact->field_91D = 0;
    bact->field_921 = 0;
    bact->field_925 = 0;
    bact->field_929 = 0;
    bact->field_92D = 0;
    bact->field_94D = 0;
    bact->field_951 = 0;
    bact->field_9A1 = 0;
    bact->field_97D = 0;
    bact->field_941 = 0;
    bact->field_945 = 0;
    bact->field_a24 = 0;
    bact->field_a28 = 0;
    bact->field_959 = 0;
    bact->field_95D = 0;
    bact->field_5A5 = -45000;
    bact->field_9A5 = 0;
    bact->kill_after_shot = 0;

    bact->field_6B9 = 0;
    bact->field_3B6 = bact->field_5A.samples_data[0].volume;
    bact->field_3BA = bact->field_5A.samples_data[0].pitch;

    bact->field_59c = 0;
    bact->gun_angle2 = bact->gun_angle;
    bact->field_B34 |= 0x10;

    for (int i = 0; i < 16; i++)
        bact->dest_fx[i].p1 = 0;

    memset(&bact->field_6BD, 0, sizeof(bact_6bd) * 3);

    init_list(&bact->field_B48);
    init_list(&bact->list2);
    init_list(&bact->list3);

    bact->field_B64.next = NULL;
    bact->field_B54.next = NULL;
    bact->field_B64.prev = NULL;
    bact->field_B54.prev = NULL;
}

void ypabact_func97(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func97");
    call_parent(zis, obj, 97, arg);
}

void ypabact_func98(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func98");
    call_parent(zis, obj, 98, arg);
}

void ypabact_func99(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func99");
    call_parent(zis, obj, 99, arg);
}

void ypabact_func100(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func100");
    call_parent(zis, obj, 100, arg);
}

void ypabact_func101(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func101");
    call_parent(zis, obj, 101, arg);
}

void ypabact_func102(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func102");
    call_parent(zis, obj, 102, arg);
}

void ypabact_func103(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func103");
    call_parent(zis, obj, 103, arg);
}

void ypabact_func104(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func104");
    call_parent(zis, obj, 104, arg);
}

void ypabact_func105(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func105");
    call_parent(zis, obj, 105, arg);
}

void ypabact_func106(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func106");
    call_parent(zis, obj, 106, arg);
}

void ypabact_func107(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func107");
    call_parent(zis, obj, 107, arg);
}

void ypabact_func108(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func108");
    call_parent(zis, obj, 108, arg);
}

void ypabact_func109(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func109");
    call_parent(zis, obj, 109, arg);
}

void ypabact_func110(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func110");
    call_parent(zis, obj, 110, arg);
}

size_t ypabact_func111(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    return 1;
}

void ypabact_func112(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func112");
    call_parent(zis, obj, 112, arg);
}

void ypabact_func113(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func113");
    call_parent(zis, obj, 113, arg);
}

void ypabact_func114(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func114");
    call_parent(zis, obj, 114, arg);
}


void ypabact_func115(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    int v4 = 0;

    if ( bact->field_621.sx > bact->field_18 - 1210.0 )
    {
        v4 = 1;
        bact->field_621.sx = bact->field_18 - 1210.0;
    }

    if ( bact->field_621.sx < 1210.0 )
    {
        v4 = 1;
        bact->field_621.sx = 1210.0;
    }

    if ( bact->field_621.sz > -1210.0 )
    {
        v4 = 1;
        bact->field_621.sz = -1210.0;
    }

    if ( bact->field_621.sz < bact->field_1c + 1210.0 )
    {
        v4 = 1;
        bact->field_621.sz = bact->field_1c + 1210.0;
    }

    if ( bact->field_B34 & 1 )
    {
        if ( v4 )
        {
            if ( bact->field_24 != 2 && bact->field_24 != 8 )
            {
                ypaworld_arg136 arg136;

                arg136.pos_x = bact->field_621.sx;
                arg136.pos_y = bact->field_621.sy + -100.0;
                arg136.pos_z = bact->field_621.sz;

                arg136.field_14 = 0;
                arg136.field_18 = bact->vwr_overeof + 100.0;
                arg136.field_1C = 0;
                arg136.field_40 = 0;

                call_method(bact->wrld, 136, &arg136);

                if ( arg136.field_20 )
                    bact->field_621.sy = arg136.field_30 - bact->vwr_overeof;
            }
        }
    }
}

void ypabact_func116(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func116");
    call_parent(zis, obj, 116, arg);
}

void ypabact_func117(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func117");
    call_parent(zis, obj, 117, arg);
}

void ypabact_func118(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func118");
    call_parent(zis, obj, 118, arg);
}

size_t ypabact_func119(NC_STACK_ypabact *obj, class_stru *zis, bact_arg119 *arg)
{
    int result = 0;

    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( arg->field_0 )
        bact->field_3D5 = arg->field_0;

    if ( arg->field_4 )
        bact->field_3D6 |= arg->field_4;

    if ( arg->field_8 )
        bact->field_3D6 &= ~arg->field_8;

    if ( arg->field_0 == 2 && (bact->field_6B9 != 2 && bact->field_6B9 != 3) )
    {
        bact->energy = -10000;

        bact->current_vp.base = bact->vp_dead.base;
        bact->current_vp.trigo = bact->vp_dead.trigo;

        bact->field_6B9 = 2;

        if ( bact->field_3B2 & 2 )
        {
            if ( bact->field_B34 & 2 )
            {
                yw_arg180 v43;
                v43.effects_type = 4;

                call_method(bact->wrld, 180, &v43);
            }

            sub_424000(&bact->field_5A, 1);
            bact->field_3B2 &= 0xFFFFFFFD;
        }

        if ( bact->field_B34 & 2 )
            sub_424000(&bact->field_5A, 8);

        if ( bact->field_3B2 & 1 )
        {
            bact->field_3B2 &= 0xFFFFFFFE;
            sub_424000(&bact->field_5A, 0);
        }

        if ( bact->field_3B2 & 8 )
        {
            bact->field_3B2 &= 0xFFFFFFF7;
            sub_424000(&bact->field_5A, 3);
        }

        if ( bact->field_3B2 & 4 )
        {
            bact->field_3B2 &= 0xFFFFFFFB;
            sub_424000(&bact->field_5A, 2);
        }

        sub_423F74(&bact->field_5A, 7);

        bact->field_3B2 |= 0x80;

        uint8_t v47 = 1;
        call_method(obj, 113, &v47);

        result = 1;
    }

    if ( arg->field_0 == 1 && 1 != bact->field_6B9 )
    {
        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;

        bact->field_6B9 = 1;

        if ( bact->field_3B2 & 8 )
        {
            bact->field_3B2 &= 0xFFFFFFF7;
            sub_424000(&bact->field_5A, 3);
        }

        if ( bact->field_3B2 & 4 )
        {
            bact->field_3B2 &= 0xFFFFFFFB;
            sub_424000(&bact->field_5A, 2);
        }

        if ( bact->field_3B2 & 0x80 )
        {
            bact->field_3B2 &= 0xFFFFFF7F;
            sub_424000(&bact->field_5A, 7);
        }

        if ( !(bact->field_3B2 & 1) )
        {
            bact->field_3B2 |= 1;
            sub_423F74(&bact->field_5A, 0);
        }

        result = 1;
    }

    if ( arg->field_0 == 5 && 5 != bact->field_6B9 )
    {
        bact->field_6B9 = 5;
        bact->current_vp.base = bact->vp_genesis.base;
        bact->current_vp.trigo = bact->vp_genesis.trigo;

        if ( bact->field_3B2 & 8 )
        {
            bact->field_3B2 &= 0xFFFFFFF7;
            sub_424000(&bact->field_5A, 3);
        }

        if ( bact->field_3B2 & 4 )
        {
            bact->field_3B2 &= 0xFFFFFFFB;
            sub_424000(&bact->field_5A, 2);
        }

        if ( bact->field_3B2 & 0x80 )
        {
            bact->field_3B2 &= 0xFFFFFF7F;
            sub_424000(&bact->field_5A, 7);
        }

        if ( !(bact->field_3B2 & 0x200) )
        {
            bact->field_3B2 |= 0x200;
            sub_423F74(&bact->field_5A, 9);
        }

        uint8_t v47 = 8;
        call_method(obj, 113, &v47);

        result = 1;
    }

    if ( arg->field_0 == 3 && bact->field_6B9 != 6 )
    {
        bact->current_vp.base = bact->vp_wait.base;
        bact->current_vp.trigo = bact->vp_wait.trigo;
        bact->field_6B9 = 6;

        if ( bact->field_3B2 & 1 )
        {
            bact->field_3B2 &= 0xFFFFFFFE;
            sub_424000(&bact->field_5A, 0);
        }

        if ( bact->field_3B2 & 8 )
        {
            bact->field_3B2 &= 0xFFFFFFF7;
            sub_424000(&bact->field_5A, 3);
        }

        if ( bact->field_3B2 & 0x80 )
        {
            bact->field_3B2 &= 0xFFFFFF7F;
            sub_424000(&bact->field_5A, 7);
        }

        if ( !(bact->field_3B2 & 4) )
        {
            bact->field_3B2 |= 4;
            sub_423F74(&bact->field_5A, 2);
        }

        result = 1;
    }

    if ( arg->field_0 == 4 && 4 != bact->field_6B9 )
    {
        bact->field_6B9 = arg->field_0;
        bact->current_vp.base = bact->vp_genesis.base;
        bact->current_vp.trigo = bact->vp_genesis.trigo;

        if ( bact->field_3B2 & 2 )
        {
            if ( bact->field_B34 & 2 )
            {
                yw_arg180 v46;
                v46.effects_type = 4;

                call_method(bact->wrld, 180, &v46);
            }

            sub_424000(&bact->field_5A, 1);
            bact->field_3B2 &= 0xFFFFFFFD;
        }

        if ( bact->field_3B2 & 1 )
        {
            bact->field_3B2 &= 0xFFFFFFFE;
            sub_424000(&bact->field_5A, 0);
        }

        if ( bact->field_3B2 & 4 )
        {
            bact->field_3B2 &= 0xFFFFFFFB;
            sub_424000(&bact->field_5A, 2);
        }

        if ( bact->field_3B2 & 0x80 )
        {
            bact->field_3B2 &= 0xFFFFFF7F;
            sub_424000(&bact->field_5A, 7);
        }

        if ( !(bact->field_3B2 & 8) )
        {
            bact->field_3B2 |= 8;
            sub_423F74(&bact->field_5A, 3);
        }

        uint8_t v47 = 4;
        call_method(obj, 113, &v47);

        result = 1;
    }

    if ( arg->field_8 == 256 && bact->field_6B9 == 7 )
    {
        if ( bact->field_B34 & 2 )
        {
            yw_arg180 v45;
            v45.effects_type = 4;
            call_method(bact->wrld, 180, &v45);
        }

        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;
        bact->field_6B9 = 1;

        sub_424000(&bact->field_5A, 1);

        bact->field_3B2 &= 0xFFFFFFFD;

        result = 1;
    }

    if ( arg->field_8 == 2048 && bact->field_6B9 == 3 )
    {
        bact->field_6B9 = 1;
        bact->current_vp.base = bact->vp_normal.base;
        bact->current_vp.trigo = bact->vp_normal.trigo;

        result = 1;
    }

    if ( arg->field_4 == 256 && bact->field_6B9 != 7 )
    {
        bact->field_6B9 = 7;
        bact->current_vp.base = bact->vp_fire.base;
        bact->current_vp.trigo = bact->vp_fire.trigo;

        if ( !(bact->field_3B2 & 2) )
        {
            if ( bact->field_B34 & 2 )
            {
                yw_arg180 v42;
                v42.effects_type = 3;
                call_method(bact->wrld, 180, &v42);
            }

            bact->field_3B2 |= 2;
            sub_423F74(&bact->field_5A, 1);
        }
        result = 1;
    }

    if ( arg->field_4 == 2048 )
    {
        bact->field_3D5 = 2;

        if ( bact->field_6B9 != 3 )
        {
            bact->current_vp.base = bact->vp_megadeth.base;
            bact->current_vp.trigo = bact->vp_megadeth.trigo;
            bact->field_6B9 = 3;

            if ( bact->field_3B2 & 2 )
            {
                if ( bact->field_B34 & 2 )
                {
                    yw_arg180 v44;
                    v44.effects_type = 4;
                    call_method(bact->wrld, 180, &v44);
                }

                sub_424000(&bact->field_5A, 1);
                bact->field_3B2 &= 0xFFFFFFFD;
            }

            if ( bact->field_B34 & 2 )
                sub_424000(&bact->field_5A, 8);

            if ( bact->field_3B2 & 1 )
            {
                bact->field_3B2 &= 0xFFFFFFFE;
                sub_424000(&bact->field_5A, 0);
            }

            if ( bact->field_3B2 & 8 )
            {
                bact->field_3B2 &= 0xFFFFFFF7;
                sub_424000(&bact->field_5A, 3);
            }

            if ( bact->field_3B2 & 4 )
            {
                bact->field_3B2 &= 0xFFFFFFFB;
                sub_424000(&bact->field_5A, 2);
            }

            if ( bact->field_3B2 & 0x80 )
            {
                bact->field_3B2 &= 0xFFFFFF7F;
                sub_424000(&bact->field_5A, 7);
            }

            sub_423F74(&bact->field_5A, 4);

            uint8_t v47 = 2;
            call_method(obj, 113, &v47);

            bact->field_611 = 0;

            result = 1;
        }
    }
    return result;
}

void ypabact_func120(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func120");
    call_parent(zis, obj, 120, arg);
}

void ypabact_func121(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func121");
    call_parent(zis, obj, 121, arg);
}

void ypabact_func122(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func122");
    call_parent(zis, obj, 122, arg);
}

void ypabact_func123(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func123");
    call_parent(zis, obj, 123, arg);
}

void ypabact_func124(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func124");
    call_parent(zis, obj, 124, arg);
}

void ypabact_func125(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func125");
    call_parent(zis, obj, 125, arg);
}


class_return ypabact_class_descr;

class_return * class_set_ypabact(int , ...)
{

    memset(ypabact_funcs, 0, sizeof(CLASSFUNC) * 1024);

    ypabact_class_descr.parent = "nucleus.class";

    ypabact_funcs[0] = (CLASSFUNC)ypabact_func0;
    ypabact_funcs[1] = (CLASSFUNC)ypabact_func1;
    ypabact_funcs[2] = (CLASSFUNC)ypabact_func2;
    ypabact_funcs[3] = (CLASSFUNC)ypabact_func3;
    ypabact_funcs[65] = (CLASSFUNC)ypabact_func65;
    ypabact_funcs[66] = (CLASSFUNC)ypabact_func66;
    ypabact_funcs[67] = (CLASSFUNC)ypabact_func67;
    ypabact_funcs[68] = (CLASSFUNC)ypabact_func68;
    ypabact_funcs[69] = (CLASSFUNC)ypabact_func69;
    ypabact_funcs[70] = (CLASSFUNC)ypabact_func70;
    ypabact_funcs[71] = (CLASSFUNC)ypabact_func71;
    ypabact_funcs[72] = (CLASSFUNC)ypabact_func72;
    ypabact_funcs[73] = (CLASSFUNC)ypabact_func73;
    ypabact_funcs[74] = (CLASSFUNC)ypabact_func74;
    ypabact_funcs[75] = (CLASSFUNC)ypabact_func75;
    ypabact_funcs[76] = (CLASSFUNC)ypabact_func76;
    ypabact_funcs[77] = (CLASSFUNC)ypabact_func77;
    ypabact_funcs[78] = (CLASSFUNC)ypabact_func78;
    ypabact_funcs[79] = (CLASSFUNC)ypabact_func79;
    ypabact_funcs[80] = (CLASSFUNC)ypabact_func80;
    ypabact_funcs[81] = (CLASSFUNC)ypabact_func81;
    ypabact_funcs[82] = (CLASSFUNC)ypabact_func82;
    ypabact_funcs[83] = (CLASSFUNC)ypabact_func83;
    ypabact_funcs[84] = (CLASSFUNC)ypabact_func84;
    ypabact_funcs[85] = (CLASSFUNC)ypabact_func85;
    ypabact_funcs[86] = (CLASSFUNC)ypabact_func86;
    ypabact_funcs[87] = (CLASSFUNC)ypabact_func87;
    ypabact_funcs[88] = (CLASSFUNC)ypabact_func88;
    ypabact_funcs[89] = (CLASSFUNC)ypabact_func89;
    ypabact_funcs[90] = (CLASSFUNC)ypabact_func90;
    ypabact_funcs[91] = (CLASSFUNC)ypabact_func91;
    ypabact_funcs[92] = (CLASSFUNC)ypabact_func92;
    ypabact_funcs[93] = (CLASSFUNC)ypabact_func93;
    ypabact_funcs[94] = (CLASSFUNC)ypabact_func94;
    ypabact_funcs[95] = (CLASSFUNC)ypabact_func95;
    ypabact_funcs[96] = (CLASSFUNC)ypabact_func96;
    ypabact_funcs[97] = (CLASSFUNC)ypabact_func97;
    ypabact_funcs[98] = (CLASSFUNC)ypabact_func98;
    ypabact_funcs[99] = (CLASSFUNC)ypabact_func99;
    ypabact_funcs[100] = (CLASSFUNC)ypabact_func100;
    ypabact_funcs[101] = (CLASSFUNC)ypabact_func101;
    ypabact_funcs[102] = (CLASSFUNC)ypabact_func102;
    ypabact_funcs[103] = (CLASSFUNC)ypabact_func103;
    ypabact_funcs[104] = (CLASSFUNC)ypabact_func104;
    ypabact_funcs[105] = (CLASSFUNC)ypabact_func105;
    ypabact_funcs[106] = (CLASSFUNC)ypabact_func106;
    ypabact_funcs[107] = (CLASSFUNC)ypabact_func107;
    ypabact_funcs[108] = (CLASSFUNC)ypabact_func108;
    ypabact_funcs[109] = (CLASSFUNC)ypabact_func109;
    ypabact_funcs[110] = (CLASSFUNC)ypabact_func110;
    ypabact_funcs[111] = (CLASSFUNC)ypabact_func111;
    ypabact_funcs[112] = (CLASSFUNC)ypabact_func112;
    ypabact_funcs[113] = (CLASSFUNC)ypabact_func113;
    ypabact_funcs[114] = (CLASSFUNC)ypabact_func114;
    ypabact_funcs[115] = (CLASSFUNC)ypabact_func115;
    ypabact_funcs[116] = (CLASSFUNC)ypabact_func116;
    ypabact_funcs[117] = (CLASSFUNC)ypabact_func117;
    ypabact_funcs[118] = (CLASSFUNC)ypabact_func118;
    ypabact_funcs[119] = (CLASSFUNC)ypabact_func119;
    ypabact_funcs[120] = (CLASSFUNC)ypabact_func120;
    ypabact_funcs[121] = (CLASSFUNC)ypabact_func121;
    ypabact_funcs[122] = (CLASSFUNC)ypabact_func122;
    ypabact_funcs[123] = (CLASSFUNC)ypabact_func123;
    ypabact_funcs[124] = (CLASSFUNC)ypabact_func124;
    ypabact_funcs[125] = (CLASSFUNC)ypabact_func125;

    ypabact_class_descr.vtbl = ypabact_funcs;
    ////ypabact_class_descr.varSize = sizeof(__NC_STACK_ypabact);
    ypabact_class_descr.varSize = sizeof(NC_STACK_ypabact) - offsetof(NC_STACK_ypabact, stack__ypabact); //// HACK
    ypabact_class_descr.field_A = 0;

    ypabact_id = 1;

    return &ypabact_class_descr;
}
