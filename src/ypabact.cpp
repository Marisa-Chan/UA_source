#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "classes.h"
#include "yw.h"
#include "ypabact.h"
#include "yparobo.h"

#include "log.h"



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
                {
                    char v14[28];

                    if ( stk->value )
                    {
                        call_method(ywo, 131, bact); //Set current bact

                        bact->field_B34 |= 1;

                        if ( yw->field_757E )
                            v14[25] = 1;

                        sub_423F74(&bact->field_5A, 8);
                    }
                    else
                    {
                        bact->field_B34 &= 0xFFFFFFFE;

                        if ( yw->field_757E )
                            v14[25] = 0;

                        sub_424000(&bact->field_5A, 8);
                    }

                    if ( yw->field_757E ) // Network message send routine?
                    {
                        *(int *)(&v14[0]) = 1014;
                        v14[12] = bact->owner;
                        v14[24] = bact->field_24;
                        *(int *)(&v14[16]) = bact->ypabact__id;

                        if ( v14[24] == 4 )
                        {
                            int *a4;
                            call_vtbl(obj, 3, 0x80002000, &a4, 0);
                            *(int *)&v14[20] = a4[10];
                            printf("UNKNOWN !!! %s\n", "ypabact_func0__sub0");
                        }

                        yw_arg181 v13;
                        v13.field_10 = 0;
                        v13.field_14 = 2;
                        v13.value = v14;
                        v13.field_18 = 1;
                        v13.val_size = 28;

                        call_method(ywo, 181, &v13);
                    }
                }
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
                    bact->airconst = stk->value;
                    bact->airconst2 = stk->value;
                    break;

                case 0x8000100B:
                    bact->field_B74 = stk->value;
                    break;

                case 0x8000100D:
                {
                    bact->field_3D4 = stk->value;

                    bact_node *nod = (bact_node *)bact->list2.head;

                    while ( nod->next )
                    {
                        nod->bact->field_3D4 = stk->value;

                        nod = (bact_node *)nod->next;
                    }
                }
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
    bact_int->field_32 = NULL;
    bact_int->field_5C9.m00 = 1.0;
    bact_int->field_5C9.m01 = 0;
    bact_int->field_5C9.m02 = 0;
    bact_int->field_5C9.m10 = 0;
    bact_int->field_5C9.m11 = 1.0;
    bact_int->field_5C9.m12 = 0;
    bact_int->field_5C9.m20 = 0;
    bact_int->field_5C9.m21 = 0;
    bact_int->field_5C9.m22 = 1.0;
    bact_int->field_605.sx = 0;
    bact_int->field_605.sy = 0;
    bact_int->field_605.sz = 0;
    bact_int->field_611 = 0;
    bact_int->field_639.sx = 0;
    bact_int->field_639.sy = 0;
    bact_int->field_639.sz = 0;
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


void sub_493DB0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *bact2, NC_STACK_ypaworld *ywo)
{
    __NC_STACK_ypabact *v6 = NULL;

    bact->field_59A = 0;
    bact->field_59c = 0;
    bact->field_3D6 &= 0xF3FFFFFF;

    int v8;
    float v17;
    float v18;

    if ( bact2->field_3D6 & 0x4000000 )
    {
        if ( !bact2->field_59c )
        {
            int v9 = bact2->field_59A - 1;

            v17 = bact2->field_418[v9].sx;
            v18 = bact2->field_418[v9].sz;

            v8 = 1;
        }
        else if ( sub_48C244(ywo, bact2->field_59c, bact2->field_5a0) )
        {
            v8 = 2;
        }
        else
            v8 = 0;
    }
    else
    {
        if ( bact2->field_3DE == 2 )
        {
            v6 = bact2->field_3e8;
            v8 = 2;
        }
        else if ( bact2->field_3DE == 1 )
        {
            v17 = bact2->field_3ec.sx;
            v18 = bact2->field_3ec.sz;
            v8 = 1;
        }
        else
            v8 = 0;
    }

    if ( !v8 )
    {
        v8 = 2;
        v6 = bact2;
    }

    if ( bact->field_24 != 2 && bact->field_24 != 8 )
    {
        bact_arg67 arg67;
        arg67.field_0 = v8;
        arg67.field_4 = 0;

        if ( v8 == 2 )
        {
            arg67.field_8 = v6;
        }
        else
        {
            arg67.targ.sx = v17;
            arg67.targ.sz = v18;
        }

        call_method(bact->self, 67, &arg67);
    }
    else
    {
        bact_arg124 arg125;

        if ( v8 == 2 )
        {
            arg125.field_8 = v6->field_621.sx;
            arg125.field_C = v6->field_621.sz;
        }
        else
        {
            arg125.field_8 = v17;
            arg125.field_C = v18;
        }

        arg125.field_10 = 32;
        arg125.field_0 = bact->field_621.sx;
        arg125.field_4 = bact->field_621.sz;
        arg125.field_12 = 1;

        call_method(bact->self, 125, &arg125);

        if ( v8 == 2 )
        {
            bact->field_59c = v6->field_2E;
            bact->field_5a0 = v6->owner;
        }
    }
}



int ypabact_func2__sub0(NC_STACK_ypabact *obj, __NC_STACK_ypabact *bact, stack_vals *stak)
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
            {
                char v14[28];

                if ( stk->value )
                {
                    if (bact->field_B3C->current_bact)
                    {
                        if ( bact->field_B3C->current_bact->field_24 != 4 )
                            bact->field_9A5 = 0;
                    }

                    call_method(bact->wrld, 131, bact); //Set current bact

                    bact->field_B34 |= 1;

                    if ( bact->field_B3C->field_757E )
                        v14[25] = 1;

                    if ( bact->field_24 == 1 && !(bact->field_3D6 & 0x200) && bact->field_3D5 == 1 )
                        bact->field_601 = bact->force;

                    sub_423F74(&bact->field_5A, 8);
                }
                else
                {
                    bact->field_B34 &= 0xFFFFFFFE;

                    if ( bact->field_B3C->field_757E )
                        v14[25] = 0;

                    sub_424000(&bact->field_5A, 8);

                    if ( bact->field_24 != 4 && bact->field_24 != 3 && bact->field_3D5 != 2 )
                    {
                        if ( bact->field_32 == bact->parent_bacto )
                        {
                            if ( !(bact->field_3D6 & 0x4000000) || !(bact->field_3D6 & 0x8000000) )
                            {
                                bact_node *node = (bact_node *)bact->list2.head;

                                while(node->next)
                                {
                                    sub_493DB0(node->bact, bact, bact->wrld);

                                    node = (bact_node *)node->next;
                                }
                            }
                        }
                        else
                        {
                            if ( !(bact->field_3D6 & 0x4000000) || !(bact->field_3D6 & 0x8000000) )
                                sub_493DB0(bact, bact->parent_bact, bact->wrld);
                        }
                    }
                }

                if ( bact->field_B3C->field_757E ) // Network message send routine?
                {
                    *(int *)(&v14[0]) = 1014;
                    v14[12] = bact->owner;
                    v14[24] = bact->field_24;
                    *(int *)(&v14[16]) = bact->ypabact__id;

                    if ( v14[24] == 4 )
                    {
                        int *a4;
                        call_vtbl(obj, 3, 0x80002000, &a4, 0);
                        *(int *)&v14[20] = a4[10];
                        printf("UNKNOWN !!! %s\n", "ypabact_func0__sub0");
                    }

                    yw_arg181 v13;
                    v13.field_10 = 0;
                    v13.field_14 = 2;
                    v13.value = v14;
                    v13.field_18 = 1;
                    v13.val_size = 28;

                    call_method(bact->wrld, 181, &v13);
                }
            }
            break;

            case 0x80001005:
                if ( stk->value )
                {
                    bact->field_B34 |= 2;
                    call_vtbl(bact->wrld, 2, 0x80002011, obj, 0);

                    if ( bact->field_24 != 9 )
                        call_method(obj, 114, 0);
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
                bact->airconst = stk->value;
                bact->airconst2 = stk->value;
                break;

            case 0x8000100D:
            {
                bact->field_3D4 = stk->value;

                bact_node *nod = (bact_node *)bact->list2.head;

                while ( nod->next )
                {
                    nod->bact->field_3D4 = stk->value;

                    nod = (bact_node *)nod->next;
                }
            }
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
    bact_node *node = (bact_node *)bact->list3.head;

    while (node->next)
    {
        int a4;
        call_vtbl(node->bacto, 3, 0x8000100B, &a4, 0);

        if ( a4 <= 0 )
        {
            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 0;

            call_method(node->bacto, 67, &arg67);

            Remove(node);

            node->bact->parent_bacto = NULL;

            call_method(bact->self, 118, node->bacto);
        }

        node = (bact_node *)node->next;
    }
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

    if ( bact->p_cell_area->field_3A == 6 && bact->field_24 == 3 && retbact == bact->self ) // if bact class == robo
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
            bact_cam.grp_1.sx = bact->field_621.sx + bact->field_651.m00 * bact->viewer.sx + bact->field_651.m10 * bact->viewer.sy + bact->field_651.m20 * bact->viewer.sz;
            bact_cam.grp_1.sy = bact->field_621.sy + bact->field_651.m01 * bact->viewer.sx + bact->field_651.m11 * bact->viewer.sy + bact->field_651.m21 * bact->viewer.sz;
            bact_cam.grp_1.sz = bact->field_621.sz + bact->field_651.m02 * bact->viewer.sx + bact->field_651.m12 * bact->viewer.sy + bact->field_651.m22 * bact->viewer.sz;
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

    bact->field_5A.field_C  = bact->field_605.sx * bact->field_611;
    bact->field_5A.field_10 = bact->field_605.sy * bact->field_611;
    bact->field_5A.field_14 = bact->field_605.sz * bact->field_611;

    sb_0x4242e0(&bact->field_5A);
}

void ypabact_func66(NC_STACK_ypabact *obj, class_stru *zis, base77Func *arg)
{
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

    for (int i = 0; i < 3; i++)
    {
        bact_6bd *bd = &bact->field_6BD[i];

        if ( bd->vp.base )
        {
            if ( bd->field_34 & 1 )
            {
                bd->vp.trigo->grp_1 = bd->field_4;

                if ( bd->field_34 & 2 )
                {
                    bd->vp.trigo->scale_rotation.m00 = bd->matrx.m00 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m01 = bd->matrx.m10 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m02 = bd->matrx.m20 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m10 = bd->matrx.m01 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m11 = bd->matrx.m11 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m12 = bd->matrx.m21 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m20 = bd->matrx.m02 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m21 = bd->matrx.m12 * bd->field_0;
                    bd->vp.trigo->scale_rotation.m22 = bd->matrx.m22 * bd->field_0;
                }
                else
                {
                    bd->vp.trigo->scale_rotation.m00 = bd->matrx.m00;
                    bd->vp.trigo->scale_rotation.m01 = bd->matrx.m10;
                    bd->vp.trigo->scale_rotation.m02 = bd->matrx.m20;
                    bd->vp.trigo->scale_rotation.m10 = bd->matrx.m01;
                    bd->vp.trigo->scale_rotation.m11 = bd->matrx.m11;
                    bd->vp.trigo->scale_rotation.m12 = bd->matrx.m21;
                    bd->vp.trigo->scale_rotation.m20 = bd->matrx.m02;
                    bd->vp.trigo->scale_rotation.m21 = bd->matrx.m12;
                    bd->vp.trigo->scale_rotation.m22 = bd->matrx.m22;
                }

                arg->field_1C = bact->ypabact__id;

                call_method(bd->vp.base, 77, arg);
            }
        }
    }
}

void ypabact_func67(NC_STACK_ypabact *obj, class_stru *zis, bact_arg67 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact->field_945 = 0;
    yw_130arg arg130;

    if ( bact->field_3D6 & 0x400 && arg->field_0 == 2 )
    {
        ypa_log_out("ALARM!!! bact-settarget auf logische Leiche owner %d, class %d, prio %d\n", bact->owner, bact->field_24, arg->field_4);
    }
    else if ( arg->field_4 )
    {
        if ( bact->field_3DF == 2 )
            Remove(&bact->field_B64);

        switch ( arg->field_0 )
        {
        case 1:
        case 5:
            bact->field_3DF = 1;

            arg130.pos_x = arg->targ.sx;
            arg130.pos_z = arg->targ.sz;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > bact->field_18 - 1210.0 )
                arg130.pos_x = bact->field_18 - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < bact->field_1c + 1210.0 )
                arg130.pos_z = bact->field_1c + 1210.0;

            if ( call_method(bact->wrld, 130, &arg130) )
            {
                bact->field_3f8 = (__NC_STACK_ypabact *)arg130.pcell;
                bact->field_3fc.sx = arg130.pos_x;
                bact->field_3fc.sz = arg130.pos_z;
                bact->field_3fc.sy = arg130.pcell->sector_height_meters;
            }
            else
            {
                bact->field_3DF = 0;
                bact->field_3f8 = 0;
            }
            break;

        case 2:
        case 6:
            bact->field_3f8 = arg->field_8;
            bact->field_3DF = 2;

            if ( bact->field_3f8 )
            {
                if ( bact->field_3f8->field_3D6 & 0x400 )
                {
                    ypa_log_out("totes vehicle als nebenziel, owner %d, class %d\n", arg->field_8->owner, arg->field_8->field_24);
                    bact->field_3DF = 0;
                }
                else
                {
                    bact->field_3fc = bact->field_3f8->field_621;

                    nlist *lst;
                    call_vtbl(bact->field_3f8->self, 3, 0x80001008, &lst, 0);

                    if ( lst )
                        AddTail(lst, &bact->field_B64);
                    else
                        ypa_log_out("Net eigeklink\n");
                }
            }
            else
            {
                ypa_log_out("Yppsn\n");
                bact->field_3DF = 0;
            }
            break;

        case 3:
            bact->field_3DF = 3;
            bact->field_3fc = arg->targ;
            break;

        case 0:
            bact->field_3f8 = NULL;
            bact->field_3DF = 0;
            break;

        default:
            bact->field_3DF = arg->field_0;
            break;
        }
    }
    else
    {
        if ( bact->field_3DE == 2 )
            Remove(&bact->field_B54);

        switch ( arg->field_0 )
        {
        case 1:
        case 5:
            bact->field_3e0 = 0;
            bact->field_3DE = 1;

            arg130.pos_x = arg->targ.sx;
            arg130.pos_z = arg->targ.sz;

            if ( arg130.pos_x < 1210.0 )
                arg130.pos_x = 1210.0;

            if ( arg130.pos_x > bact->field_18 - 1210.0 )
                arg130.pos_x = bact->field_18 - 1210.0;

            if ( arg130.pos_z > -1210.0 )
                arg130.pos_z = -1210.0;

            if ( arg130.pos_z < bact->field_1c + 1210.0 )
                arg130.pos_z = bact->field_1c + 1210.0;

            if ( call_method(bact->wrld, 130, &arg130) )
            {
                bact->field_3e8 = (__NC_STACK_ypabact *)arg130.pcell;
                bact->field_3ec.sx = arg130.pos_x;
                bact->field_3ec.sz = arg130.pos_z;
                bact->field_3ec.sy = arg130.pcell->sector_height_meters;
            }
            else
            {
                bact->field_3DE = 0;
                bact->field_3e8 = 0;
            }
            break;

        case 2:
        case 6:
            bact->field_3e8 = arg->field_8;
            bact->field_3DE = 2;

            if ( bact->field_3e8 )
            {
                if ( bact->field_3e8->field_3D6 & 0x400 )
                {
                    ypa_log_out("totes vehicle als hauptziel, owner %d, class %d - ich bin class %d\n", arg->field_8->owner, arg->field_8->field_24, bact->field_24);
                    bact->field_3DE = 0;
                    return;
                }

                bact->field_3ec = bact->field_3e8->field_621;

                nlist *lst;
                call_vtbl(bact->field_3e8->self, 3, 0x80001008, &lst, 0);

                if ( lst )
                    AddTail(lst, &bact->field_B54);

                bact->field_3e0 = arg->field_8->field_2E;
            }
            else
            {
                ypa_log_out("PrimT. without a pointer\n");
                bact->field_3DE = 0;
            }
            break;

        case 3:
            bact->field_3DE = 3;
            bact->field_3e0 = 0;
            bact->field_3ec = arg->targ;
            break;

        case 0:
            bact->field_3e8 = 0;
            bact->field_59A = 0;
            bact->field_3DE = 0;
            bact->field_3D6 &= 0xFBFFFFFF;
            break;

        case 4:
            bact->field_645 = arg->targ;
            bact->field_3DE = 4;
            bact->field_3e8 = 0;
            bact->field_3e0 = 0;
            break;

        default:
            bact->field_3DE = arg->field_0;
            break;
        }

        if ( arg->field_0 == 1 || arg->field_0 == 2 )
        {
            bact_node *node = (bact_node *)bact->list2.head;

            while ( node->next )
            {
                if ( node->bact->field_3D5 != 2)
                {
                    call_method(node->bacto, 67, arg);
                    if ( !(bact->field_3D6 & 0x4000000)  )
                        bact->field_3D6 &= 0xF3FFFFFF;
                }

                node = (bact_node *)node->next;
            }
        }
    }
}

void ypabact_func68(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    bact_arg67 v36;

    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    //NC_STACK_ypabact *a4;
    //call_vtbl(bact->wrld, 3, 0x80002010, &a4, 0);

    if ( bact->mass == 1.0 )
    {
        if ( bact->field_3D6 & 0x800 )
        {
            bact->field_B74 -= arg->field_4;

            if ( bact->field_B74 < 0 )
                call_method(bact->wrld, 144, obj);
        }
        else
        {
            bact_arg119 v37;
            v37.field_0 = 0;
            v37.field_8 = 0;
            v37.field_4 = 2048;

            call_method(obj, 78, &v37);

            bact->field_B74 = 6000;
        }
    }
    else
    {
        if ( bact->field_24 != 4 )
            call_method(obj, 82, arg);

        if ( bact->field_3D5 == 2 )
        {
            if ( bact->field_3D6 & 0x200 )
                bact->field_B74 -= arg->field_4;
        }

        bact->airconst = bact->airconst2;

        bact->field_5A.samples_data[0].pitch = bact->field_3BA;
        bact->field_5A.samples_data[0].volume = bact->field_3B6;

        if ( bact->field_915 - bact->field_919 >= 250 && bact->field_3DE != 4 && bact->field_24 != 9 && bact->field_3D5 != 2 && bact->field_3D5 != 5 && bact->field_3D5 != 4 )
        {

            bact->field_919 = bact->field_915;
            bact->field_639.sx = 0;
            bact->field_639.sy = 0;
            bact->field_639.sz = 0;

            if ( bact->field_915 - bact->field_935 > 5000 )
            {
                bact->field_935 = bact->field_915;

                call_method(obj, 104, arg);
            }

            if ( bact->field_3D5 == 1 && bact->field_3DE )
            {
                if ( bact->field_3DE == 2 )
                {
                    bact->field_639.sx = bact->field_3e8->field_621.sx - bact->field_621.sx;
                    bact->field_639.sy = bact->field_3e8->field_621.sy - bact->field_621.sy;
                    bact->field_639.sz = bact->field_3e8->field_621.sz - bact->field_621.sz;

                    if ( bact->field_3e8->field_3D5 != 2)
                    {
                        bact->field_3ec = bact->field_3e8->field_621;
                    }
                }
                else
                {
                    bact->field_639.sx = bact->field_3ec.sx - bact->field_621.sx;
                    bact->field_639.sz = bact->field_3ec.sz - bact->field_621.sz;
                    bact->field_639.sy = bact->field_3ec.sy - bact->field_621.sy;
                }

                float tmp = bact->field_639.sx * bact->field_639.sx + bact->field_639.sy * bact->field_639.sy + bact->field_639.sz * bact->field_639.sz;

                if ( sqrt(tmp) > 2000.0 )
                    bact->field_639.sy = 0;

                if ( bact->field_32 == bact->parent_bacto )
                {
                    if ( bact->field_B34 & 1 )
                    {
                        tmp = bact->field_639.sx * bact->field_639.sx + bact->field_639.sy * bact->field_639.sy + bact->field_639.sz * bact->field_639.sz;

                        bact_node *node = (bact_node *)bact->list2.head;

                        int v44 = 0;

                        while (node->next)
                        {
                            if ( node->bact->field_3D5 != 2 )
                            {
                                if ( sqrt(tmp) < 800.0 )
                                {
                                    v36.field_0 = bact->field_3DE;
                                    v36.field_4 = 0;
                                    v36.field_8 = bact->field_3e8;
                                    v36.targ.sx = bact->field_3ec.sx;
                                    v36.targ.sy = bact->field_3ec.sy;
                                    v36.targ.sz = bact->field_3ec.sz;
                                }
                                else
                                {
                                    bact_arg94 v35;
                                    v35.field_0 = v44;
                                    call_method(obj, 94, &v35);

                                    v36.field_0 = 3;
                                    v36.field_4 = 0;
                                    v36.targ.sx = v35.pos1.sx;
                                    v36.targ.sy = v35.pos1.sy;
                                    v36.targ.sz = v35.pos1.sz;
                                }

                                call_method(node->bacto, 67, &v36);
                            }

                            node = (bact_node *)node->next;
                            v44++;
                        }
                    }
                }
            }

            if ( bact->field_3DE == 0)
            {
                if ( bact->field_32 && bact->field_3e0 )
                {
                    v36.field_4 = bact->field_3e0;

                    if ( call_method(bact->field_32, 132, &v36) )
                    {
                        v36.field_4 = 0;
                    }
                    else
                    {
                        bact->field_3e0 = 0;

                        v36.field_0 = 1;
                        v36.field_4 = 0;
                        v36.targ = bact->field_3ec;
                    }

                    call_method(obj, 67, &v36);
                }
            }

            if ( bact->field_B34 & 2 )
            {
                if ( bact->field_3DE == 2 )
                {
                    if ( bact->field_3e8 )
                    {
                        if ( !((1 << bact->owner) & bact->field_3e8->p_cell_area->field_39) )
                        {
                            v36.field_0 = 0;
                            v36.field_4 = 0;

                            call_method(obj, 67, &v36);
                        }
                    }
                }
            }
            else if ( bact->field_6B9 == 6 && bact->field_3D5 == 1 )
            {
                bact_arg119 v38;
                v38.field_0 = 1;
                v38.field_4 = 0;
                v38.field_8 = 0;
                call_method(obj, 78, &v38);
            }
        }

        call_method(obj, 69, arg);
    }
}

void ypabact_func69(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( (bact->field_915 - bact->field_91D) >= 250 && bact->owner != 0 && bact->field_3DF != 4 && bact->field_3D5 != 4 && bact->field_3D5 != 2 && bact->field_3D5 != 5 )
    {
        bact->field_91D = bact->field_915;

        if ( bact->field_3D6 & 0x4000 )
        {
            float a1 = sqrt(bact->field_639.sx * bact->field_639.sx + bact->field_639.sz * bact->field_639.sz);

            if ( a1 > 3600.0 )
            {
                bact_arg67 arg67;
                arg67.field_0 = 0;
                arg67.field_4 = 1;

                bact_node *nod = (bact_node *)bact->list2.head;

                while( nod->next )
                {
                    call_method(nod->bacto, 67, &arg67);
                    nod = (bact_node *)nod->next;
                }

                call_method(obj, 67, &arg67);

                if ( bact->field_B34 & 2 )
                    call_method(obj, 71, arg);
                else
                    call_method(obj, 70, arg);
                return;
            }
        }

        NC_STACK_ypabact *wee;
        call_vtbl(bact->wrld, 3, 0x80002010, &wee, 0);

        if ( bact->field_3D5 == 1 || bact->field_3D5 == 3 )
        {
            if ( bact->field_915 - bact->field_929 > 500 )
            {
                bact->field_929 = bact->field_915;

                bact_arg90 arg90;
                arg90.field_8 = 1;
                arg90.unit = bact;

                call_method(obj, 90, &arg90);

                if ( arg90.ret_unit )
                {
                    if ( arg90.ret_unit->field_24 != 3 && bact->parent_bacto == bact->field_32 && bact->field_32 == wee && arg90.ret_unit->field_2E != bact->field_5A1 && bact->field_915 - bact->field_5A5 > 45000 )
                    {
                        int v32 = 0;
                        if ( arg90.ret_unit->field_24 == 9 )
                            call_vtbl(arg90.ret_unit->self, 3, 0x80002006, &v32, 0);

                        if ( !v32 )
                        {
                            bact->field_5A1 = arg90.ret_unit->field_2E;
                            bact->field_5A5 = bact->field_915;

                            robo_arg134 arg134;
                            arg134.field_4 = 7;
                            arg134.field_8 = arg90.ret_unit->field_2E;
                            arg134.field_C = v32;
                            arg134.field_10 = v32;
                            arg134.unit = bact;
                            arg134.field_14 = 46;

                            call_method(bact->field_32, 134, &arg134);
                        }
                    }
                }

                if ( bact->field_3D5 == 3 || ( bact->field_3D4 >= 50 && !(bact->field_3D6 & 0x4000) && (bact->field_3DF == 0 || bact->field_3DF == 1 || bact->field_3DF == 3) ) )
                {
                    if ( arg90.ret_unit )
                    {
                        bact->field_3e4 = arg90.ret_unit->field_2E;

                        bact_arg67 arg67;
                        arg67.field_0 = 2;
                        arg67.field_4 = 1;
                        arg67.field_8 = arg90.ret_unit;
                        call_method(obj, 67, &arg67);
                    }

                    if ( bact->field_915 - bact->field_92D > 2000 && bact->field_3D4 == 75 && !(bact->field_B34 & 1) && bact->parent_bacto == bact->field_32 )
                    {
                        if ( (bact->field_3DF == 3 || bact->field_3DF == 0)
                                && bact->field_621.sx > 1260.0
                                && bact->field_621.sx < bact->field_18 + -1260.0
                                && bact->field_621.sz < -1260.0
                                && bact->field_621.sz > bact->field_1c + 1260.0 )
                        {
                            bact->field_92D = bact->field_915;

                            if ( bact->owner != bact->p_cell_area->owner )
                            {
                                bact_arg67 arg67;
                                arg67.field_4 = 1;
                                arg67.field_0 = 1;
                                arg67.targ.sx = bact->field_621.sx;
                                arg67.targ.sz = bact->field_621.sz;
                                call_method(obj, 67, &arg67);
                            }
                        }
                    }

                    if ( bact->field_32 == bact->parent_bacto && bact->field_3DF == 1 )
                    {
                        bact_node *nod = (bact_node *)bact->list2.head;

                        while( nod->next )
                        {
                            if ( nod->bact->field_3DF == 0 || nod->bact->field_3DF == 3 )
                            {
                                bact_arg67 arg67;
                                arg67.field_0 = 1;
                                arg67.targ.sx = bact->field_3fc.sx;
                                arg67.targ.sy = bact->field_3fc.sy;
                                arg67.targ.sz = bact->field_3fc.sz;
                                arg67.field_4 = 1;
                                call_method(nod->bacto, 67, &arg67);
                            }

                            nod = (bact_node *)nod->next;
                        }
                    }
                }
            }

            if ( bact->field_3DF == 2 )
            {
                bact->field_639.sx = bact->field_3f8->field_621.sx - bact->field_621.sx;
                bact->field_639.sy = bact->field_3f8->field_621.sy - bact->field_621.sy;
                bact->field_639.sz = bact->field_3f8->field_621.sz - bact->field_621.sz;
            }
            else if ( bact->field_3DF == 1)
            {
                bact->field_639.sx = bact->field_3fc.sx - bact->field_621.sx;
                bact->field_639.sy = bact->field_3fc.sy - bact->field_621.sy;
                bact->field_639.sz = bact->field_3fc.sz - bact->field_621.sz;
            }

            float v40 = sqrt(bact->field_639.sy * bact->field_639.sy + bact->field_639.sx * bact->field_639.sx + bact->field_639.sz * bact->field_639.sz);
            if ( v40 > 2000.0 )
                bact->field_639.sy = 0;
        }

        if ( bact->field_B34 & 2 )
        {
            if ( bact->field_3DF == 2 )
            {
                if ( bact->field_3f8 )
                {
                    float v25 = bact->field_621.sx - bact->field_3f8->field_621.sx;
                    float v26 = bact->field_621.sz - bact->field_3f8->field_621.sz;
                    float v33 = sqrt(v25 * v25 + v26 * v26);

                    if ( !((1 << bact->owner) & bact->field_3f8->p_cell_area->field_39) || v33 > 2160.0 )
                    {
                        bact_arg67 arg67;
                        arg67.field_4 = 1;
                        arg67.field_0 = 0;
                        call_method(obj, 67, &arg67);
                    }
                }
            }
        }
    }

    if ( bact->field_B34 & 2 )
        call_method(obj, 71, arg);
    else
        call_method(obj, 70, arg);
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

void ypabact_func78(NC_STACK_ypabact *obj, class_stru *zis, bact_arg119 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( (bact->field_24 == 2 || bact->field_24 == 8) && arg->field_0 == 2 )
    {
        bact_arg119 newarg;
        newarg.field_8 = 0;
        newarg.field_0 = 0;
        newarg.field_4 = 2048;

        call_method(obj, 78, &newarg);
    }
    else
    {
        int v6 = call_method(obj, 119, arg);

        _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

        if ( yw->field_757E )
        {
            if ( v6 && bact->owner && bact->field_24 != 4 )
            {
                char buf[32];
                /**(_DWORD *)buf = 1005;
                buf[12] = bact->owner;
                *(_DWORD *)&buf[16] = bact->ypabact__id;
                buf[28] = arg->field_0;
                *(_DWORD *)&buf[20] = arg->field_4;
                *(_DWORD *)&buf[24] = arg->field_8;
                buf[29] = bact->field_24;*/

                yw_arg181 v9;
                v9.field_14 = 2;
                v9.val_size = 32;
                v9.field_18 = 1;
                v9.field_10 = 0;
                v9.value = buf;

                call_method(bact->wrld, 181, &v9);
            }
        }
    }
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

void ypabact_func81(NC_STACK_ypabact *obj, class_stru *zis, bact_arg81 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact_node *node = (bact_node *)bact->list2.head;

    while ( node->next )
    {
        call_method(node->bacto, 81, arg);

        node = (bact_node *)node->next;
    }

    if ( bact->field_3D5 != 2 )
    {
        switch ( arg->enrg_type )
        {
        case 1:
            arg->enrg_sum += bact->energy;
            break;

        case 3:
            arg->enrg_sum++;
            break;

        case 2:
            arg->enrg_sum += bact->shield;
            break;

        case 4:
            arg->enrg_sum += bact->energy_2;
            break;

        case 5:
        {
            bact_node *nd = (bact_node *)bact->field_B48.head;

            while (nd->next)
            {
                arg->enrg_sum++;
                nd = (bact_node *)nd->next;
            }
        }
        break;

        default:
            break;
        }
    }
}

// Update bact energy
void ypabact_func82(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    cellArea *cell = bact->p_cell_area;

    if ( bact->field_3D5 != 2 )
    {
        int v16 = bact->field_915 - bact->field_95D;

        if ( v16 >= 1500 )
        {
            bact->field_95D = bact->field_915;

            yw_arg176 arg176;
            arg176.owner = cell->owner;

            call_method(bact->wrld, 176, &arg176);

            float v14 = v16 / 1000.0;

            float denerg = bact->energy_2 * v14 * cell->field_2F * arg176.field_4 / 7000.0;

            if ( bact->owner == cell->owner )
                bact->energy += denerg;
            else
                bact->energy -= denerg;

            if ( bact->energy < 0 )
                bact->energy = 0;

            if ( bact->energy > bact->energy_2 )
                bact->energy = bact->energy_2;
        }
    }
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


void ypabact_func87__sub0(__NC_STACK_ypabact *bact, __NC_STACK_ypabact *a2)
{
    int v2 = (int)((float)a2->energy_2 * 0.7);

    if ( v2 < 10000 )
        v2 = 10000;

    if ( v2 > 25000 )
        v2 = 25000;

    int v3 = (float)a2->field_931 * 0.2 / (float)v2 * (float)a2->energy_2;

    if ( bact->energy + v3 > bact->energy_2 )
    {
        NC_STACK_yparobo *robj = (NC_STACK_yparobo *)bact->field_32;
        __NC_STACK_yparobo *robo = &robj->stack__yparobo;
        __NC_STACK_ypabact *rbact = robo->bact_internal;

        int v10 = v3 - (bact->energy_2 - bact->energy);

        bact->energy = bact->energy_2;

        if ( rbact->energy + v10 > rbact->energy_2 )
        {
            int v14 = v10 - (rbact->energy_2 - rbact->energy);

            rbact->energy = rbact->energy_2;

            if ( robo->field_4F5 + v14 >= rbact->energy_2 )
            {
                robo->field_4FD += v14 - (rbact->energy_2 - robo->field_4F5);

                robo->field_4F5 = rbact->energy_2;

                if ( robo->field_4FD > rbact->energy_2 )
                    robo->field_4FD = rbact->energy_2;
            }
            else
            {
                robo->field_4F5 += v14;
            }
        }
        else
        {
            rbact->energy += v10;
        }
    }
    else
    {
        bact->energy += v3;
    }
}


xyz stru_5150F4 = {0, 0, 0};

size_t ypabact_func87(NC_STACK_ypabact *obj, class_stru *zis, int *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

    int a4;
    call_vtbl(bact->self, 3, 0x80001004, &a4, 0);

    float trad;
    if ( a4 )
        trad = bact->vwr_radius;
    else
        trad = bact->radius;


    int v49 = 0;

    rbcolls *v46 = NULL;

    call_vtbl(obj, 3, 0x8000100E, &v46, 0);
    if ( bact->field_611 == 0.0 )
        return 0;

    cellArea *cell = bact->p_cell_area;

    xyz stru_5150E8;
    stru_5150E8.sx = 0;
    stru_5150E8.sy = 0;
    stru_5150E8.sz = 0;

    int v45 = 0;

    rbcolls *v55;

    __NC_STACK_ypabact *bnode = (__NC_STACK_ypabact *)cell->field_3C.head;

    while ( bnode->next )
    {
        int v53 = bnode->field_3D5 == 2 && (bnode->field_6BD[0].field_34 & 1) && (bact->field_B34 & 2) && bnode->field_931 > 0 ;

        if ( bnode->self != obj
                && bnode->field_24 != 4
                && (!call_method(bnode->self, 100, 0) || v53) )
        {

            call_vtbl(bnode->self, 3, 0x8000100E, &v55, 0);

            int v9;

            if ( v55 )
            {
                v9 = v55->robo_coll_num;
                v49 = 1;
            }
            else
            {
                v9 = 1;
            }

            for (int i = v9 - 1; i >= 0; i--)
            {
                float ttrad;
                float v41, v42, v43;

                if (!v55)
                {
                    ttrad = trad;
                    v41 = bnode->field_621.sx;
                    v42 = bnode->field_621.sy;
                    v43 = bnode->field_621.sz;
                }
                else
                {
                    roboColl *v10 = &v55->roboColls[i];
                    ttrad = v10->robo_coll_radius;

                    v41 = bnode->field_621.sx + bnode->field_651.m00 * v10->robo_coll_x + bnode->field_651.m10 * v10->robo_coll_y + bnode->field_651.m20 * v10->robo_coll_z;
                    v42 = bnode->field_621.sy + bnode->field_651.m01 * v10->robo_coll_x + bnode->field_651.m11 * v10->robo_coll_y + bnode->field_651.m21 * v10->robo_coll_z;
                    v43 = bnode->field_621.sz + bnode->field_651.m02 * v10->robo_coll_x + bnode->field_651.m12 * v10->robo_coll_y + bnode->field_651.m22 * v10->robo_coll_z;

                    if ( ttrad < 0.01 )
                        continue;
                }

                stru_5150F4.sx = bact->field_621.sx - v41;
                stru_5150F4.sy = bact->field_621.sy - v42;
                stru_5150F4.sz = bact->field_621.sz - v43;

                float v56 = sqrt(stru_5150F4.sx * stru_5150F4.sx + stru_5150F4.sy * stru_5150F4.sy + stru_5150F4.sz * stru_5150F4.sz);

                if ( trad + ttrad >= v56 )
                {
                    if ( !v53 )
                    {
                        stru_5150E8.sy += v42;
                        stru_5150E8.sz += v43;
                        stru_5150E8.sx += v41;

                        v45++;
                    }
                    else
                    {
                        ypabact_func87__sub0(bact, bnode);

                        bnode->field_931 = -1;

                        if ( yw->GameShell )
                            sub_423F74(&yw->GameShell->samples2_info, 4);

                        if ( yw->field_757E )
                        {
                            char v39[20];
//              *(_DWORD *)v39 = 1031;
//              v39[12] = bnode->owner;
//              *(_DWORD *)&v39[16] = bnode->ypabact__id;

                            yw_arg181 v32;
                            v32.field_14 = 2;
                            v32.val_size = 20;
                            v32.field_10 = 0;
                            v32.field_18 = 1;
                            v32.value = v39;

                            call_method(yw->self_full, 181, &v32);

                            if ( bnode->owner != bact->owner )
                            {
                                bnode->field_6BD[0].field_34 = 0;
                                bnode->field_6BD[0].vp.trigo = NULL;
                                bnode->field_6BD[0].vp.base = NULL;
                            }
                        }
                        break;
                    }
                }
            }
        }

        bnode = (__NC_STACK_ypabact *)bnode->next;
    }

    if ( !v45 || (v46 && !v49) )
    {
        bact->field_3D6 &= 0xFFFDFFFF;
        return 0;
    }


    float v20 = 1.0 / (float)v45;

    stru_5150E8.sx *= v20;
    stru_5150E8.sy *= v20;
    stru_5150E8.sz *= v20;

    stru_5150F4.sx = stru_5150E8.sx - bact->field_621.sx;
    stru_5150F4.sy = stru_5150E8.sy - bact->field_621.sy;
    stru_5150F4.sz = stru_5150E8.sz - bact->field_621.sz;

    float v26 = sqrt(stru_5150F4.sy * stru_5150F4.sy + stru_5150F4.sx * stru_5150F4.sx + stru_5150F4.sz * stru_5150F4.sz);

    if ( v26 < 0.0001)
        return 0;

    float v27 = 1.0 / v26;

    bact_arg88 v33;
    v33.pos1.sx = stru_5150F4.sx * v27;
    v33.pos1.sy = stru_5150F4.sy * v27;
    v33.pos1.sz = stru_5150F4.sz * v27;

    float v61 = stru_5150F4.sy * bact->field_605.sy + stru_5150F4.sx * bact->field_605.sx + stru_5150F4.sz * bact->field_605.sz;

    if ( v61 > 1.0 )
        v61 = 1.0;

    if ( v61 < -1.0 )
        v61 = -1.0;

    if ( acos(v61) > 1.5708 )
        return 0;

    v33.pos2.sy = 2.0;
    v33.pos2.sz = *arg / 1000.0;

    if ( v55 )
        v33.pos2.sx = 1.2;
    else
        v33.pos2.sx = 0.8;

    if ( !(bact->field_3D6 & 0x20000) )
    {
        if ( a4 )
        {
            sub_423F74(&bact->field_5A, 6);

            bact->field_3D6 |= 0x20000;

            yw_arg180 v40;
            v40.field_4 = 1.0;
            v40.field_8 = stru_5150E8.sx;
            v40.field_C = stru_5150E8.sz;
            v40.effects_type = 5;

            call_method(bact->wrld, 180, &v40);
        }
    }

    if ( fabs(bact->field_611) < 0.1 )
        bact->field_611 = 1.0;

    call_method(obj, 88, &v33);

    bact->field_639 = bact->field_605;

    bact->field_919 = bact->field_915;
    bact->field_91D = bact->field_915;

    return 1;
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


int ypabact_func90__sub0__sub0(__NC_STACK_ypabact *unit)
{
    bact_node *node = (bact_node *)unit->list2.head;
    while ( node->next )
    {
        if ( node->bact->field_3DF != 2 )
            return 1;

        node = (bact_node *)node->next;
    }
    return 0;
}

__NC_STACK_ypabact * ypabact_func90__sub0(cellArea *cell, __NC_STACK_ypabact *unit, float *radius, char *job)
{
    NC_STACK_ypaworld *wrld;
    call_vtbl(unit->self, 3, 0x80001001, &wrld, 0);

    VhclProto *vhcl_protos;
    call_vtbl(wrld, 3, 0x80002014, &vhcl_protos, 0);

    __NC_STACK_ypabact *v40 = NULL;

    __NC_STACK_ypabact *cel_unit = (__NC_STACK_ypabact *)cell->field_3C.head;

    VhclProto *proto = &vhcl_protos[unit->id];

    while (cel_unit->next)
    {
        if ( cel_unit->field_24 != 4 && cel_unit->field_3D5 != 2 )
        {
            if ( cel_unit->owner != unit->owner && cel_unit->owner )
            {
                int job_id;

                switch ( cel_unit->field_24 )
                {
                case 1:
                    job_id = proto->job_fighthelicopter;
                    break;

                case 2:
                case 8:
                    job_id = proto->job_fighttank;
                    break;

                case 6:
                case 7:
                    job_id = proto->job_fightflyer;
                    break;

                case 3:
                    job_id = proto->job_fightrobo;
                    break;

                default:
                    job_id = 5;
                    break;
                }

                if ( *job <= job_id )
                {
                    float xx = unit->field_621.sx - cel_unit->field_621.sx;
                    float yy = unit->field_621.sy - cel_unit->field_621.sy;
                    float zz = unit->field_621.sz - cel_unit->field_621.sz;

                    float radivs = sqrt(xx * xx + yy * yy + zz * zz);

                    int v33;
                    call_vtbl(cel_unit->self, 3, 0x80001004, &v33, 0);

                    if ( *radius >= radivs || v33 )
                    {
                        if ( unit->field_24 == 9 || unit->field_24 == 3 )
                        {
                            if ( call_method(unit->self, 111, cel_unit) )
                            {
                                *radius = radivs;
                                *job = job_id;
                                v40 = cel_unit;
                            }
                        }
                        else
                        {
                            xyz tmp;

                            int v20;

                            if ( unit->parent_bacto == unit->field_32 )
                            {
                                if ( unit->field_3DE == 1 )
                                {
                                    tmp = unit->field_3ec;

                                }
                                else if ( unit->field_3DE == 2 )
                                {
                                    tmp = unit->field_3e8->field_621;
                                }
                                else
                                {
                                    tmp = unit->field_621;
                                }

                                v20 = 1;
                            }
                            else
                            {
                                __NC_STACK_ypabact *prnt_bct;
                                call_vtbl(unit->parent_bacto, 3, 0x80001003, &prnt_bct, 0);

                                if ( prnt_bct->field_3DE == 1 )
                                {
                                    tmp = prnt_bct->field_3ec;
                                }
                                else if ( prnt_bct->field_3DE == 2 )
                                {
                                    tmp = prnt_bct->field_3e8->field_621;
                                }
                                else
                                {
                                    tmp = unit->field_621;
                                }

                                v20 = 0;
                            }

                            zz = tmp.sz - unit->field_621.sz;
                            xx = tmp.sx - unit->field_621.sx;
                            float v48 = sqrt(zz * zz + xx * xx);

                            if ( v48 <= 3600.0 )
                            {
                                if ( call_method(unit->self, 111, cel_unit) )
                                {
                                    *radius = radivs;
                                    *job = job_id;
                                    v40 = cel_unit;
                                }
                            }
                            else
                            {
                                int v29 = 0;

                                nlist *lst;
                                call_vtbl(cel_unit->self, 3, 0x80001008, &lst, 0);

                                bact_node *bct_nd = (bact_node *)lst->head;
                                while ( bct_nd->next )
                                {
                                    bact_node *int_bct_nd;
                                    call_vtbl(bct_nd->bacto, 3, 0x80001012, &int_bct_nd, 0);

                                    if ( bct_nd == int_bct_nd && int_bct_nd->bact->owner == unit->owner )
                                        v29++;

                                    if ( v29 > 1 ) // Looks like hack
                                        break;

                                    bct_nd = (bact_node *)bct_nd->next;
                                }

                                if ( v29 <= 1 && (!v20 || !ypabact_func90__sub0__sub0(unit)) )
                                {
                                    if ( call_method(unit->self, 111, cel_unit) )
                                    {
                                        *radius = radivs;
                                        *job = job_id;
                                        v40 = cel_unit;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        cel_unit = (__NC_STACK_ypabact *)cel_unit->next;
    }

    return v40;
}

void ypabact_func90(NC_STACK_ypabact *obj, class_stru *zis, bact_arg90 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    yw_130arg arg130;
    if ( arg->field_8 & 1 )
    {
        arg130.pos_x = arg->unit->field_621.sx;
        arg130.pos_z = arg->unit->field_621.sz;
    }
    else
    {
        arg130.pos_x = arg->pos_x;
        arg130.pos_z = arg->pos_z;
    }

    if ( call_method(bact->wrld, 130, &arg130) )
    {
        float rad = 1800.0;
        char job = 0;

        arg->ret_unit = NULL;

        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {
                __NC_STACK_ypabact *v7 = ypabact_func90__sub0( &arg130.pcell[x + y * bact->field_20], arg->unit, &rad, &job);

                if ( v7 )
                    arg->ret_unit = v7;
            }
        }
    }
}

void ypabact_func91(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func91");
    call_parent(zis, obj, 91, arg);
}

void ypabact_func92(NC_STACK_ypabact *obj, class_stru *zis, bact_arg92 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    yw_130arg arg130;

    int v4 = bact->field_20;
    int v5 = bact->field_22;

    arg->energ1 = 0;
    arg->energ2 = 0;

    if ( arg->field_14 & 1 )
    {
        arg130.pos_x = bact->field_621.sx;
        arg130.pos_z = bact->field_621.sz;
    }
    else
    {
        arg130.pos_x = arg->pos.sx;
        arg130.pos_z = arg->pos.sz;
    }

    if ( call_method(bact->wrld, 130, &arg130) )
    {
        cellArea *cell = arg130.pcell;

        if ( arg130.sec_x != 0 &&  arg130.sec_z != 0 )
        {
            // left-up
            cellArea *tcell = &cell[-v4 - 1];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_z )
        {
            // up
            cellArea *tcell = &cell[-v4];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z )
        {
            // right-up
            cellArea *tcell = &cell[-v4 + 1];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x )
        {
            // left
            cellArea *tcell = &cell[-1];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        // center
        if ( (1 << bact->owner) & cell->field_39 )
        {
            __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)cell->field_3C.head;

            while (cl_unit->next)
            {
                if ( cl_unit->owner )
                {
                    if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                    {
                        if ( cl_unit->owner == bact->owner )
                            arg->energ1 += cl_unit->energy;
                        else
                            arg->energ2 += cl_unit->energy;
                    }
                }
                cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
            }
        }

        if ( arg130.sec_x < v4 - 1 )
        {
            // right
            cellArea *tcell = &cell[1];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x != 0 && arg130.sec_z < v5 - 1 )
        {
            // left-down
            cellArea *tcell = &cell[ v4 - 1 ];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_z < v5 - 1  )
        {
            // down
            cellArea *tcell = &cell[ v4 ];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( arg130.sec_x < v4 - 1 && arg130.sec_z < v5 - 1 )
        {
            // down-right
            cellArea *tcell = &cell[ v4 + 1 ];

            if ( (1 << bact->owner) & tcell->field_39 )
            {
                __NC_STACK_ypabact *cl_unit = (__NC_STACK_ypabact *)tcell->field_3C.head;

                while (cl_unit->next)
                {
                    if ( cl_unit->owner )
                    {
                        if ( cl_unit->field_3D5 != 2 && (cl_unit->field_24 != 3 || cl_unit->owner != bact->owner) && cl_unit->field_24 != 4 )
                        {
                            if ( cl_unit->owner == bact->owner )
                                arg->energ1 += cl_unit->energy;
                            else
                                arg->energ2 += cl_unit->energy;
                        }
                    }
                    cl_unit = (__NC_STACK_ypabact *)cl_unit->next;
                }
            }
        }

        if ( !(arg->field_14 & 2) )
        {
            int v33 = 0;

            if ( cell->field_2E == 1 )
            {
                v33 = cell->buildings_health[0][0];
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        if ( cell->buildings_health[i][j] ) //LOL
                            v33 += cell->buildings_health[i][j];
                    }
                }

                v33 /= 9;
            }

            if ( cell->owner == bact->owner )
            {
                if ( arg->field_14 & 4 )
                    arg->energ1 += v33 * 120;
            }
            else
            {
                arg->energ2 += v33 * 120;
            }
        }
    }
}

void ypabact_func93(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func93");
    call_parent(zis, obj, 93, arg);
}

void ypabact_func94(NC_STACK_ypabact *obj, class_stru *zis, bact_arg94 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float tmp = sqrt(bact->field_651.m20 * bact->field_651.m20 + bact->field_651.m22 * bact->field_651.m22);

    float v16 = bact->field_651.m20 / tmp;
    float v17 = bact->field_651.m22 / tmp;

    arg->pos2.sy = 0;

    arg->pos1.sy = bact->field_621.sy;
    arg->pos1.sx = bact->field_621.sx - (arg->field_0 / 3 + 1) * v16 * 150.0;
    arg->pos1.sz = bact->field_621.sz - (arg->field_0 / 3 + 1) * v17 * 150.0;

    int v6 = arg->field_0 % 3;

    if ( v6 == 0 )
    {
        arg->pos1.sx += 100.0 * v17;
        arg->pos1.sz += -100.0 * v16;
    }
    else if ( v6 == 2 )
    {
        arg->pos1.sx += -100.0 * v17;
        arg->pos1.sz += 100.0 * v16;
    }

    arg->pos2.sx = arg->pos1.sx - bact->field_621.sx;
    arg->pos2.sz = arg->pos1.sz - bact->field_621.sz;
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
    bact->field_32 = NULL;
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
    bact->field_9B1 = NULL;
    bact->field_935 = 0;
    bact->field_939 = 0;
    bact->field_961.sx = 0;
    bact->field_961.sy = 0;
    bact->field_961.sz = 0;
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

void sub_48A0E0(__NC_STACK_ypabact *bact, float angle)
{
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
}

void ypabact_func99(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    printf("MAKE ME %s\n","ypabact_func99");
    call_parent(zis, obj, 99, arg);
}

size_t ypabact_func100(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    return (bact->current_vp.base == bact->vp_dead.base || bact->current_vp.base == bact->vp_genesis.base || bact->current_vp.base == bact->vp_megadeth.base) && bact->field_3D5 == 2;
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

void ypabact_func104(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->field_93D > 0 )
        bact->field_93D -= arg->field_4;

    if ( bact->field_93D < 0 )
        bact->field_93D = 0;

    if ( bact->field_2D & 8 )
    {
        if ( !bact->field_93D )
            bact->field_B34 |= 8;
    }

    if ( bact->field_3D5 != 1 || bact->field_B34 & 2 )
    {
        bact->field_961 = bact->field_621;
        bact->field_939 = bact->field_915;
    }
    else
    {
        float xx = bact->field_961.sx - bact->field_621.sx;
        float yy = bact->field_961.sy - bact->field_621.sy;
        float zz = bact->field_961.sz - bact->field_621.sz;

        if (sqrt(xx * xx + yy * yy + zz * zz) >= 12.0)
        {
            bact->field_961 = bact->field_621;
            bact->field_939 = bact->field_915;
        }
        else
        {
            if ( bact->field_B34 & 8 )
                bact->field_2D |= 8;

            if ( bact->field_915 - bact->field_939 > 10000 )
            {
                if ( (bact->field_24 == 2 || bact->field_24 == 8) && !(bact->field_3D6 & 0x40000000) )
                {
                    bact->field_62D = bact->field_621;

                    bact->field_621.sx += -bact->field_651.m20 * 10.0;
                    bact->field_621.sy += -bact->field_651.m21 * 10.0;
                    bact->field_621.sz += -bact->field_651.m22 * 10.0;

                    call_method(obj, 115, 0);

                    sub_48A0E0(bact, 0.1);

                    ypaworld_arg136 arg136;
                    arg136.pos_x = bact->field_62D.sx;
                    arg136.pos_y = bact->field_62D.sy;
                    arg136.pos_z = bact->field_62D.sz;
                    arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
                    arg136.field_18 = bact->field_621.sy - bact->field_62D.sy;
                    arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;
                    arg136.field_40 = 1;

                    call_method(bact->wrld, 136, &arg136);

                    if ( arg136.field_20 )
                    {
                        bact->field_621.sx = arg136.field_2C;
                        bact->field_621.sy = arg136.field_30 + -5.0;
                        bact->field_621.sz = arg136.field_34;
                    }
                }
            }
        }
    }
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

size_t ypabact_func108(NC_STACK_ypabact *obj, class_stru *zis, float *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->field_3D4 == 100 )
        return 1;

    if ( bact->field_3D4 )
    {
        bact_arg81 arg81;
        arg81.enrg_sum = 0;
        arg81.enrg_type = 1;

        call_method(obj, 81, &arg81);

        float v11 = arg81.enrg_sum;

        arg81.enrg_sum = 0;
        arg81.enrg_type = 4;

        call_method(obj, 81, &arg81);

        v11 = v11 / (float)arg81.enrg_sum;

        if ( arg )
            *arg = v11;

        if ( bact->field_3D6 & 0x4000 )
        {
            if ( v11 > 0.5 )
                return 1;
        }
        else if ( v11 > 0.2 )
        {
            return 1;
        }
    }
    return 0;
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

size_t ypabact_func111(NC_STACK_ypabact *obj, class_stru *zis, __NC_STACK_ypabact *)
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

void ypabact_func114(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float radius;
    if ( bact->vwr_radius >= 32.0 )
        radius = bact->vwr_radius;
    else
        radius = 32.0;

    yw_137col coltmp[10];

    ypaworld_arg137 arg137;
    arg137.pos.sx = bact->field_621.sx;
    arg137.pos.sy = bact->field_621.sy;
    arg137.pos.sz = bact->field_621.sz;
    arg137.pos2.sx = bact->field_651.m00;
    arg137.pos2.sy = bact->field_651.m01;
    arg137.pos2.sz = bact->field_651.m02;
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    call_method(bact->wrld, 137, &arg137);

    xyz tmp;
    tmp.sx = 0.0;
    tmp.sy = 0.0;
    tmp.sz = 0.0;

    float trad = 0.0;

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.sy < 0.6 )
        {
            xyz tmp2;
            tmp2.sx = bact->field_621.sx - clsn->pos1.sx;
            tmp2.sy = bact->field_621.sy - clsn->pos1.sy;
            tmp2.sz = bact->field_621.sz - clsn->pos1.sz;

            tmp.sx += clsn->pos2.sx;
            tmp.sy += clsn->pos2.sy;
            tmp.sz += clsn->pos2.sz;

            float v36 = radius - sqrt(tmp2.sx * tmp2.sx + tmp2.sy * tmp2.sy + tmp2.sz * tmp2.sz);

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    if ( bact->vwr_radius >= 32.0 )
        radius = bact->vwr_radius;
    else
        radius = 32.0;

    arg137.pos.sx = bact->field_621.sx;
    arg137.pos.sy = bact->field_621.sy;
    arg137.pos.sz = bact->field_621.sz;
    arg137.pos2.sx = -bact->field_651.m00;
    arg137.pos2.sy = -bact->field_651.m01;
    arg137.pos2.sz = -bact->field_651.m02;
    arg137.coll_max = 10;
    arg137.radius = radius;
    arg137.field_30 = 0;
    arg137.collisions = coltmp;

    call_method(bact->wrld, 137, &arg137);

    for (int i = arg137.coll_count - 1; i >= 0; i-- )
    {
        yw_137col *clsn = &arg137.collisions[i];

        if ( clsn->pos2.sy < 0.6 )
        {
            xyz tmp2;
            tmp2.sx = bact->field_621.sx - clsn->pos1.sx;
            tmp2.sy = bact->field_621.sy - clsn->pos1.sy;
            tmp2.sz = bact->field_621.sz - clsn->pos1.sz;

            tmp.sx += clsn->pos2.sx;
            tmp.sy += clsn->pos2.sy;
            tmp.sz += clsn->pos2.sz;

            float v36 = radius - sqrt(tmp2.sx * tmp2.sx + tmp2.sy * tmp2.sy + tmp2.sz * tmp2.sz);

            if ( trad == 0.0 || trad < v36 )
                trad = v36;
        }
    }

    float v25 = sqrt(tmp.sx * tmp.sx + tmp.sy * tmp.sy + tmp.sz * tmp.sz);

    if ( v25 > 0.0001 )
    {
        float v26 = 1.0 / v25;
        tmp.sx *= v26;
        tmp.sy *= v26;
        tmp.sz *= v26;
    }

    bact->field_621.sx -= tmp.sx * trad;
    bact->field_621.sy -= tmp.sy * trad;
    bact->field_621.sz -= tmp.sz * trad;
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

class_return * class_set_ypabact(int, ...)
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
