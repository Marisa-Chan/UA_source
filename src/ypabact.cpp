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

                case 0x80001009:
                    bact->airconst = stk->value;
                    bact->airconst2 = stk->value;
                    break;

                case 0x8000100A:
                    if ( stk->value )
                        bact->field_B34 |= 0x10;
                    else
                        bact->field_B34 &= 0xFFFFFFEF;
                    break;

                case 0x8000100B:
                    bact->field_B74 = stk->value;
                    break;

                case 0x8000100C:
                    bact->current_vp.base = (NC_STACK_base *)stk->value;
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
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    sub_423DD8(&bact->field_5A);

    bact->field_3D6 |= 0x2000000;

    if ( !(bact->field_3D6 & 0x400) )
        call_method(obj, 77, 0);

    if ( bact->p_cell_area )
        Remove(bact);

    if ( bact->parent_bacto )
        Remove(&bact->list_node);

    while (1)
    {
        bact_node *nd = (bact_node *)bact->list2.head;

        if (!nd->next)
            break;

        delete_class_obj(nd->bacto);
    }

    while ( 1 )
    {
        bact_node *nd = (bact_node *)bact->list3.head;

        if (!nd->next)
            break;

        Remove(bact->list3.head);

        delete_class_obj(nd->bacto);
    }

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

            case 0x80001006:
                if ( stk->value )
                    bact->field_B34 |= 4;
                else
                    bact->field_B34 &= 0xFFFFFFFB;
                break;

            case 0x80001007:
                if ( stk->value )
                    bact->field_B34 |= 8;
                else
                    bact->field_B34 &= 0xFFFFFFF7;
                break;

            case 0x80001009:
                bact->airconst = stk->value;
                bact->airconst2 = stk->value;
                break;

            case 0x8000100A:
                if ( stk->value )
                    bact->field_B34 |= 0x10;
                else
                    bact->field_B34 &= 0xFFFFFFEF;
                break;

            case 0x8000100B:
                bact->field_B74 = stk->value;
                break;

            case 0x8000100C:
                bact->current_vp.base = (NC_STACK_base *)stk->value;
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
        bact_node * next_node = (bact_node *)node->next; // Save next node before remove

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

        node = next_node;
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
        bact_node *next_vnod = (bact_node *)vnod->next;
        call_method(vnod->bacto, 65, arg);
        vnod = next_vnod;
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
        bact->field_87D.scale_rotation.m00 = bact->field_651.m00 * bact->field_67D.sx;
        bact->field_87D.scale_rotation.m01 = bact->field_651.m10 * bact->field_67D.sy;
        bact->field_87D.scale_rotation.m02 = bact->field_651.m20 * bact->field_67D.sz;
        bact->field_87D.scale_rotation.m10 = bact->field_651.m01 * bact->field_67D.sx;
        bact->field_87D.scale_rotation.m11 = bact->field_651.m11 * bact->field_67D.sy;
        bact->field_87D.scale_rotation.m12 = bact->field_651.m21 * bact->field_67D.sz;
        bact->field_87D.scale_rotation.m20 = bact->field_651.m02 * bact->field_67D.sx;
        bact->field_87D.scale_rotation.m21 = bact->field_651.m12 * bact->field_67D.sy;
        bact->field_87D.scale_rotation.m22 = bact->field_651.m22 * bact->field_67D.sz;
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
        bact_node *next_node = (bact_node *)bnod->next;

        call_method(bnod->bacto, 65, arg);

        arg->numid++;

        bnod = next_node;
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

void ypabact_func70__sub1(__NC_STACK_ypabact *bact, ypabact_arg65 *arg)
{
    bact->field_601 = bact->force;

    float v39 = arg->field_4 / 1000.0;

    float v55 = -bact->field_645.sy;

    if ( v55 == 1.0 )
        v55 = 0.99998999;

    if ( v55 == -1.0 )
        v55 = -0.99998999;

    float v54 = bact->mass * 9.80665;
    float v60 = bact->field_601;

    if ( v60 == 0.0 )
        v60 = 0.1;

    float v5 = sqrt( (1.0 - POW2(v55)) ) * (v55 * -0.5);

    float v6 = (1.0 - 0.25 * POW2(v55) + 0.25 * POW2(v55) * POW2(v55)) * (POW2(v54) / POW2(v60));

    float v58 = sqrt( (1.0 - v6) ) + (v54 * v5 / v60);

    if ( v58 > 1.0 )
        v58 = 1.0;

    if ( v58 < -1.0 )
        v58 = -1.0;

    float v49 = -cos( asin(v58) );
    float v47;

    if ( bact->field_645.sz != 0.0 )
    {
        float v62 = (1.0 - POW2(v49)) / (POW2(bact->field_645.sx) / POW2(bact->field_645.sz) + 1.0);

        if ( v62 < 0.0 )
            v62 = 0.0;

        v47 = sqrt(v62);

        if ( bact->field_645.sz < 0.0 )
            v47 = -v47;
    }
    else
    {
        v47 = 0.0;
    }

    float v48;

    if ( bact->field_645.sx != 0.0 )
    {
        float v57 = (1.0 - POW2(v49)) / (POW2(bact->field_645.sz) / POW2(bact->field_645.sx) + 1.0);

        if ( v57 < 0.0 )
            v57 = 0.0;

        v48 = sqrt(v57);

        if ( bact->field_645.sx < 0.0 )
            v48 = -v48;
    }
    else
    {
        v48 = 0.0;
    }

    float v51 = -bact->field_651.m11 * v47 - v49 * -bact->field_651.m12;
    float v52 = -bact->field_651.m12 * v48 - v47 * -bact->field_651.m10;
    float v50 = -bact->field_651.m10 * v49 - v48 * -bact->field_651.m11;

    float v43 = sqrt( POW2(v51) + POW2(v52) + POW2(v50) );

    if ( v43 != 0.0 )
    {
        float v16 = 1.0 / v43;
        float v59 = -bact->field_651.m10 * v48 + -bact->field_651.m11 * v49 + -bact->field_651.m12 * v47;

        v50 *= v16;
        v51 *= v16;
        v52 *= v16;

        if ( v59 > 1.0 )
            v59 = 1.0;

        if ( v59 < -1.0 )
            v59 = -1.0;

        float v42 = bact->maxrot * v39;

        float v56 = acos(v59);

        if ( v56 > v42 )
            v56 = v42;

        if ( fabs(v56) > BACT_MIN_ANGLE )
        {
            float v18 = sin(-v56);
            float v31 = cos(v56);


            mat3x3 mat2;

            mat2.m00 = (1.0 - v31) * v51 * v51 + v31;
            mat2.m01 = (1.0 - v31) * v51 * v52 - v18 * v50;
            mat2.m02 = (1.0 - v31) * v50 * v51 + v18 * v52;

            mat2.m10 = (1.0 - v31) * v51 * v52 + v18 * v50;
            mat2.m11 = (1.0 - v31) * v52 * v52 + v31;
            mat2.m12 = (1.0 - v31) * v52 * v50 - v18 * v51;

            mat2.m20 = (1.0 - v31) * v50 * v51 - v18 * v52;
            mat2.m21 = (1.0 - v31) * v52 * v50 + v18 * v51;
            mat2.m22 = (1.0 - v31) * v50 * v50 + v31;

            mat3x3 v26;

            mat_mult(&bact->field_651, &mat2, &v26);

            bact->field_651 = v26;
        }
    }
}

void ypabact_func70__sub0(__NC_STACK_ypabact *bact, int a2)
{
    float v21 = bact->field_651.m11;

    if ( v21 > 1.0 )
        v21 = 1.0;

    if ( v21 < -1.0 )
        v21 = -1.0;

    if ( acos(v21) > 0.003 && (bact->field_605.sz != 0.0 || bact->field_605.sx != 0.0) && bact->field_611 > 0.0 )
    {
        float v11 = a2 / 1000.0;

        float v18 = bact->field_605.sz * bact->field_651.m22 + bact->field_605.sx * bact->field_651.m20;

        v18 = v18 / sqrt( POW2(bact->field_605.sx) + POW2(bact->field_605.sz) );

        float v19 = v18 / sqrt( POW2(bact->field_651.m22) + POW2(bact->field_651.m20) );

        if ( v19 > 1.0 )
            v19 = 1.0;

        if ( v19 < -1.0 )
            v19 = -1.0;

        float v20 = acos(v19);

        float v13 = bact->maxrot * v11 * (fabs(v20) * 0.8 + 0.2);

        if ( v20 > v13 )
            v20 = v13;

        if ( bact->field_605.sx * bact->field_651.m22 - bact->field_651.m20 * bact->field_605.sz < 0.0 )
            v20 = -v20;

        mat3x3 mat2;
        mat2.m00 = cos(v20);
        mat2.m01 = 0;
        mat2.m02 = -sin(v20);

        mat2.m10 = 0;
        mat2.m11 = 1.0;
        mat2.m12 = 0;

        mat2.m20 = sin(v20);
        mat2.m21 = 0;
        mat2.m22 = cos(v20);

        mat3x3 v6;

        mat_mult(&bact->field_651, &mat2, &v6);

        bact->field_651 = v6;
    }
}

void ypabact_func70(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float v75 = arg->field_4 / 1000.0;

    float v77 = sqrt(POW2(bact->field_639.sx) + POW2(bact->field_639.sy) + POW2(bact->field_639.sz));

    if ( v77 > 0.0 )
    {
        float v6 = 1.0 / v77;

        bact->field_645.sx = bact->field_639.sx * v6;
        bact->field_645.sy = bact->field_639.sy * v6;
        bact->field_645.sz = bact->field_639.sz * v6;
    }

    int v82 = bact->field_B34 & 1;
    int v70 = bact->field_B34 & 4;

    int v80 = call_method(bact->wrld, 145, bact);

    int v79;

    if ( v82 )
        v79 = bact->vwr_radius;
    else
        v79 = bact->radius;

    switch ( bact->field_3D5 )
    {
    case 1:
    {
        if ( bact->field_B34 & 8 )
        {
            if ( (v80 || (bact->field_3DF == 0 && v77 < 1200.0)) && !(bact->field_3D6 & 0x200) )
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

        ypaworld_arg136 arg136;

        arg136.field_20 = 0;
        arg136.pos_x = bact->field_62D.sx;
        arg136.pos_y = bact->field_62D.sy;
        arg136.pos_z = bact->field_62D.sz;
        arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
        arg136.field_18 = 0;
        arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;

        float v78 = sqrt(POW2(arg136.field_14) + POW2(arg136.field_1C));

        if ( v78 <= 0.0 )
        {
            arg136.field_14 = bact->field_651.m20 * 300.0;
            arg136.field_18 = bact->field_651.m21 * 300.0;
            arg136.field_1C = bact->field_651.m22 * 300.0;
        }
        else
        {
            float v14 = 300.0 / v78;

            arg136.field_14 *= v14;
            arg136.field_18 *= v14;
            arg136.field_1C *= v14;
        }

        arg136.field_20 = 0;
        arg136.field_40 = 0;

        ypaworld_arg136 arg136_1;
        arg136_1.field_20 = 0;
        arg136_1.field_40 = 0;

        if ( v82 || (bact->field_3D6 & 0x20) || (v80 && v70) )
        {
            arg136_1.pos_x = bact->field_62D.sx;
            arg136_1.pos_y = bact->field_62D.sy;
            arg136_1.pos_z = bact->field_62D.sz;
            arg136_1.field_14 = arg136.field_14 * 0.93969 - arg136.field_1C * 0.34202;
            arg136_1.field_18 = arg136.field_18;
            arg136_1.field_1C = arg136.field_1C * 0.93969 + arg136.field_14 * 0.34202;

            call_method(bact->wrld, 136, &arg136_1);
        }

        ypaworld_arg136 arg136_2;
        arg136_2.field_20 = 0;
        arg136_2.field_40 = 0;

        if ( v82 || (bact->field_3D6 & 0x10) || (v80 && v70) )
        {
            arg136_2.pos_x = bact->field_62D.sx;
            arg136_2.pos_y = bact->field_62D.sy;
            arg136_2.pos_z = bact->field_62D.sz;
            arg136_2.field_14 = arg136.field_14 * 0.93969 + arg136.field_1C * 0.34202;
            arg136_2.field_18 = arg136.field_18;
            arg136_2.field_1C = arg136.field_1C * 0.93969 - arg136.field_14 * 0.34202;

            call_method(bact->wrld, 136, &arg136_2);
        }

        if ( v82 || !(bact->field_3D6 & 0x30) || (v80 && v70) )
            call_method(bact->wrld, 136, &arg136);

        int v18 = 0;

        bact_arg88 arg88;
        arg88.pos1.sy = 0;
        arg88.pos1.sx = 0;
        arg88.pos1.sz = 0;

        if ( arg136.field_20 )
        {
            if ( v78 + v79 > arg136.field_24 * 300.0 )
            {
                arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;
                v18++;
            }
        }

        if ( arg136_1.field_20 )
        {
            if ( v78 + v79 > arg136_1.field_24 * 300.0 )
            {
                arg88.pos1.sx += arg136_1.field_3C->triangles[arg136_1.field_38].field_0;
                arg88.pos1.sy += arg136_1.field_3C->triangles[arg136_1.field_38].field_4;
                arg88.pos1.sz += arg136_1.field_3C->triangles[arg136_1.field_38].field_8;
                v18++;
            }
        }

        if ( arg136_2.field_20 )
        {
            if ( v78 + v79 > arg136_2.field_24 * 300.0 )
            {
                arg88.pos1.sx += arg136_2.field_3C->triangles[arg136_2.field_38].field_0;
                arg88.pos1.sy += arg136_2.field_3C->triangles[arg136_2.field_38].field_4;
                arg88.pos1.sz += arg136_2.field_3C->triangles[arg136_2.field_38].field_8;
                v18++;
            }
        }

        if ( !arg136.field_20 && !arg136_1.field_20 && !arg136_2.field_20 )
        {
            bact->field_3D6 &= 0xFFFFFF8F;
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

        float v21 = bact->mass * 9.80665;

        if ( v21 <= bact->force )
            v21 = bact->force;

        float v88;

        if ( bact->airconst >= 10.0 )
            v88 = bact->airconst;
        else
            v88 = 10.0;


        ypaworld_arg136 arg136_3;

        arg136_3.field_14 = (bact->field_605.sx * 200.0 * bact->field_611) / (v21 / v88);

        if ( arg136_3.field_14 < -200.0 )
            arg136_3.field_14 = -200.0;

        if ( arg136_3.field_14 > 200.0 )
            arg136_3.field_14 = 200.0;

        arg136_3.field_18 = bact->height;

        arg136_3.field_1C = (bact->field_605.sz * 200.0 * bact->field_611) / (v21 / v88);

        if ( arg136_3.field_1C < -200.0 )
            arg136_3.field_1C = -200.0;

        if ( arg136_3.field_1C > 200.0 )
            arg136_3.field_1C = 200.0;

        arg136_3.pos_x = bact->field_62D.sx;
        arg136_3.pos_y = bact->field_62D.sy;
        arg136_3.pos_z = bact->field_62D.sz;
        arg136_3.field_40 = 0;

        call_method(bact->wrld, 136, &arg136_3);

        if ( arg136_3.field_20 )
        {
            bact->field_645.sy = -(1.0 - arg136_3.field_24);
        }
        else
        {
            NC_STACK_ypabact *a4;
            call_vtbl(bact->wrld, 3, 0x80002011, &a4, 0);

            if ( ((bact->field_3DF != 2 || (a4 != bact->field_3f8->self && bact->field_3f8->field_24 != 3)) &&
                    (bact->field_3DE != 2 || (a4 != bact->field_3e8->self && bact->field_3e8->field_24 != 3)))
                    || bact->field_645.sy >= -0.01 )
            {
                if ( bact->field_645.sy < 0.15 )
                    bact->field_645.sy = 0.15;
            }
        }

        if ( bact->field_3D6 & 0x30 )
            bact->field_645.sy = -0.2;

        if ( arg136_3.field_20 )
        {
            if ( arg136_3.field_24 * bact->height < bact->radius && bact->field_605.sy > 0.0 )
            {
                arg88.pos1.sx = arg88.pos1.sx + arg136_3.field_3C->triangles[arg136_3.field_38].field_0;
                arg88.pos1.sy = arg88.pos1.sy + arg136_3.field_3C->triangles[arg136_3.field_38].field_4;
                arg88.pos1.sz = arg88.pos1.sz + arg136_3.field_3C->triangles[arg136_3.field_38].field_8;
                v18++;
            }
        }

        if ( v18 )
        {
            float v29 = 1.0 / (float)v18;

            arg88.pos2.sy = 2.0;
            arg88.pos2.sx = 0.7;
            arg88.pos2.sz = v75;
            arg88.pos1.sx *= v29;
            arg88.pos1.sy *= v29;
            arg88.pos1.sz *= v29;
            call_method(obj, 88, &arg88);
        }
        else
        {
            bact->field_3D6 &= 0xFFFBFFFF;
        }

        if ( bact->field_645.sy != 0.0 )
        {
            float v31 = 1.0 / sqrt(POW2(bact->field_645.sx) + POW2(bact->field_645.sy) + POW2(bact->field_645.sz));

            bact->field_645.sx *= v31;
            bact->field_645.sy *= v31;
            bact->field_645.sz *= v31;
        }

        float v81 = 1.0 / sqrt(POW2(arg136.field_14) + POW2(arg136.field_1C));

        if ( bact->field_3D6 & 0x10 )
        {
            bact->field_645.sx = -arg136.field_1C * v81;
            bact->field_645.sz = arg136.field_14 * v81;
        }
        else if ( bact->field_3D6 & 0x20 )
        {
            bact->field_645.sx = arg136.field_1C * v81;
            bact->field_645.sz = -arg136.field_14 * v81;
        }

        ypabact_func70__sub1(bact, arg);

        if ( bact->field_3D6 & 8 )
            bact->field_611 *= 0.95;

        bact->field_601 = (0.85 - bact->field_645.sy) * bact->force;

        bact_arg74 arg74;
        arg74.flag = 0;
        arg74.field_0 = arg->field_4 / 1000.0;

        call_method(obj, 74, &arg74);

        ypabact_func70__sub0(bact, arg->field_4);

        bact_arg75 arg75;

        arg75.field_14 = v75;
        arg75.field_18 = bact->field_915;

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
            arg75.bct = bact->field_3e8;
            arg75.field_x = 0;

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
            if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_8 = 256;
                arg78.field_0 = 0;
                arg78.field_4 = 0;

                call_method(obj, 78, &arg78);
            }

            bact->field_3D6 &= 0xFFFFFFFC;
        }
    }
    break;

    case 2:
        call_method(obj, 121, arg);
        break;

    case 3:

        if ( bact->field_915 - bact->field_941 > 500 )
        {
            bact->field_941 = bact->field_915;

            bact_arg110 arg110;
            arg110.field_one = bact->field_3DF;
            arg110.field_two = 1;

            int v46 = call_method(obj, 110, &arg110);

            arg110.field_two = 0;
            arg110.field_one = bact->field_3DE;
            int v48 = call_method(obj, 110, &arg110);

            if ( v46 != 3 || v48 != 3 )
            {
                bact_arg67 arg67;

                if ( !v46 )
                {
                    arg67.field_0 = 0;
                    arg67.field_4 = 1;
                    call_method(obj, 67, &arg67);
                }

                if ( !v48 )
                {
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

        if ( bact->field_B34 & 0x10 )
        {
            bact->field_601 = 0;

            if ( bact->field_3D6 & 0x200 )
            {
                bact_arg119 arg78;
                arg78.field_8 = 0;
                arg78.field_4 = 0;
                arg78.field_0 = 3;
                call_method(obj, 78, &arg78);

                ypaworld_arg136 v52;
                v52.pos_x = bact->field_621.sx;
                v52.pos_y = bact->field_621.sy;
                v52.pos_z = bact->field_621.sz;
                v52.field_14 = 0;
                v52.field_1C = 0;
                v52.field_40 = 0;
                v52.field_18 = bact->overeof + 50.0;

                call_method(bact->wrld, 136, &v52);

                if ( v52.field_20 )
                    bact->field_621.sy = v52.field_30 - bact->overeof;
            }
            else
            {
                bact_arg86 v65;
                v65.field_one = 0;
                v65.field_two = arg->field_4;

                call_method(obj, 86, &v65);
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

void ypabact_func71__sub0(__NC_STACK_ypabact *bact, float a4)
{
    mat3x3 mat2;
    mat2.m00 = cos(a4);
    mat2.m01 = sin(a4);
    mat2.m02 = 0;
    mat2.m10 = -sin(a4);
    mat2.m11 = cos(a4);
    mat2.m12 = 0;
    mat2.m20 = 0;
    mat2.m21 = 0;
    mat2.m22 = 1.0;

    mat3x3 v3;

    mat_mult(&mat2, &bact->field_651, &v3);

    bact->field_651 = v3;
}

void ypabact_func71__sub1(__NC_STACK_ypabact *bact, float a4)
{
    mat3x3 mat2;
    mat2.m00 = 1.0;
    mat2.m01 = 0;
    mat2.m02 = 0;
    mat2.m10 = 0;
    mat2.m11 = cos(a4);
    mat2.m12 = sin(a4);
    mat2.m20 = 0;
    mat2.m21 = -sin(a4);
    mat2.m22 = cos(a4);

    mat3x3 v3;

    mat_mult(&mat2, &bact->field_651, &v3);

    bact->field_651 = v3;
}

void ypabact_func71__sub2(__NC_STACK_ypabact *bact, float a4)
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

    mat3x3 v3;

    mat_mult(&mat2, &bact->field_651, &v3);

    bact->field_651 = v3;
}



void ypabact_func71(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact->airconst = bact->airconst2;

    float v106 = arg->field_4 / 1000.0;

    if ( bact->field_3D5 == 1 || bact->field_3D5 == 3 )
    {

        bact->field_62D = bact->field_621;

        if ( bact->field_B34 & 8 )
        {
            if ( !(bact->field_3D6 & 0x200) )
            {
                int arg87 = arg->field_4;
                call_method(obj, 87, &arg87);
            }
        }

        float v98;

        if ( bact->field_3D6 & 0x200 )
            v98 = 0.1;
        else
            v98 = 1.0;

        bact_arg119 arg78;

        if ( v98 <= fabs(bact->field_611) )
        {
            if ( !(bact->field_3D6 & 0x100) )
            {
                arg78.field_0 = 1;
                arg78.field_8 = 0;
                arg78.field_4 = 0;
                call_method(obj, 78, &arg78);
            }

            bact->field_3D6 &= 0xFFFFFDFF;
        }
        else
        {
            ypaworld_arg136 arg136;

            arg136.pos_x = bact->field_621.sx;
            arg136.pos_y = bact->field_621.sy;
            arg136.pos_z = bact->field_621.sz;
            arg136.field_1C = 0;
            arg136.field_14 = 0;

            float v8;

            if ( bact->vwr_overeof <= bact->vwr_radius )
                v8 = bact->vwr_radius;
            else
                v8 = bact->vwr_overeof;

            arg136.field_40 = 0;
            arg136.field_18 = v8 * 1.5;

            call_method(bact->wrld, 136, &arg136);

            if ( arg136.field_20 && bact->field_601 <= bact->mass * 9.80665 )
            {
                bact->field_611 = 0;
                bact->field_3D6 |= 0x200;
                bact->field_621.sy = arg136.field_30 - bact->vwr_overeof;
                bact->field_601 = bact->mass * 9.80665;
            }
            else
            {
                bact->field_3D6 &= 0xFFFFFDFF;
            }

            float a4 = POW2(bact->field_3ec.sx - bact->field_621.sx) + POW2(bact->field_3ec.sz - bact->field_621.sz);

            if ( bact->field_3DE != 1 || sqrt(a4) <= 800.0 )
            {
                if ( bact->field_3D6 & 0x200 )
                {
                    if ( !(bact->field_3D6 & 0x100) )
                    {
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;
                        arg78.field_0 = 3;
                        call_method(obj, 78, &arg78);
                    }
                }

                bact->field_3D5 = 1;
            }
            else
            {
                bact->field_3D5 = 3;

                if ( bact->field_3D6 & 0x200 )
                {
                    if ( !(bact->field_3D6 & 0x100) )
                    {
                        arg78.field_0 = 3;
                        arg78.field_8 = 0;
                        arg78.field_4 = 0;
                        call_method(obj, 78, &arg78);
                    }
                }
            }
        }

        float v110 = arg->inpt->sliders_vars[1] * bact->maxrot * v106;
        float v103 = -arg->inpt->sliders_vars[0] * bact->maxrot * v106;

        if ( (fabs(bact->field_605.sy) > 0.98 || bact->field_611 == 0.0) && bact->field_651.m11 > 0.996 && arg->inpt->sliders_vars[1] == 0.0 )
        {
            float v23 = 1.0 / sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m02) );

            float v26 = 1.0 / sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m22) );

            bact->field_651.m00 *= v23;
            bact->field_651.m01 = 0;
            bact->field_651.m02 *= v23;

            bact->field_651.m10 = 0;
            bact->field_651.m11 = 1.0;
            bact->field_651.m12 = 0;

            bact->field_651.m20 *= v26;
            bact->field_651.m21 = 0;
            bact->field_651.m22 *= v26;
        }

//    float v84 = sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m22) );
//    v84 /= sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m21) + POW2(bact->field_651.m22) );
//
//    float v75 = v84;
//
//    if ( v84 > 1.0 )
//      v75 = 1.0;

        float v109 = sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m02) );
        v109 /= sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m01) + POW2(bact->field_651.m02) );

        if ( v109 > 1.0 )
            v109 = 1.0;

        if ( v109 < -1.0 )
            v109 = -1.0;

        float v111 = acos(v109);

        if ( bact->field_651.m01 < 0.0 )
            v111 = -v111;

        if ( fabs(v111) < 0.01 )
            v111 = 0.0;

        float v36 = fabs(v111);

        float v101 = bact->field_9AD * v36 + bact->field_9AD * 0.25;

        float v102 = bact->maxrot * v106   *  v101;

        if ( v102 > v36 )
            v102 = v36;

        if ( v111 > 0.0 )
            v102 = -v102;

        float v104 = -v103 + v102;

        if ( fabs(v104 + v111) > 1.0 )
        {
            if ( v104 >= 0.0 )
                v104 = 1.0 - fabs(v111);
            else
                v104 = fabs(v111) - 1.0;
        }


        if ( fabs(v104) > bact->maxrot * 2.0 * v106 * v101 )
        {
            if ( v104 < 0.0 )
                v104 = bact->maxrot * -2.0 * v106 * v101;

            if ( v104 >= 0.0 )
                v104 = bact->maxrot * 2.0 * v106 * v101;
        }

        if ( fabs(v104) < 0.001 )
            v104 = 0.0;

        ypabact_func71__sub1(bact, v110 * 0.5);
        ypabact_func71__sub0(bact, v104 * 0.5);
        ypabact_func71__sub2(bact, v103 * 0.5);

        bact->field_601 += bact->force * v106 * 0.5 * arg->inpt->sliders_vars[2];

        if ( bact->field_601 < 0.0 )
            bact->field_601 = 0;

        if ( bact->field_601 > bact->force )
            bact->field_601 = bact->force;

        float v99 = bact->field_601;

        float v47 = bact->p_cell_area->sector_height_meters - bact->field_621.sy;
        float v94 = bact->field_679 * 0.8;

        if ( v47 > v94 )
        {
            float v91 = bact->mass * 9.80665 - bact->force;
            float v89 = 1.0 / (bact->field_679 * 0.2);
            float v86 = (v47 - v94) * v91 * v89;

            if ( bact->field_601 > v86 )
                bact->field_601 = v86;

            if ( bact->field_601 < 0.0 )
                bact->field_601 = 0;
        }

        bact_arg79 v61;

        v61.field_18 = 4;
        v61.field_20.sx = bact->field_651.m20;
        v61.field_20.sy = bact->field_651.m21;
        v61.field_20.sz = bact->field_651.m22;

        bact_arg106 v64;
        v64.field_0 = 5;
        v64.field_4.sx = bact->field_651.m20;
        v64.field_4.sy = bact->field_651.m21;
        v64.field_4.sz = bact->field_651.m22;

        if ( call_method(obj, 106, &v64) )
        {
            v61.field_1C = v64.ret_bact;
            v61.field_18 = 2;
        }

        if ( arg->inpt->but_flags & 1 || arg->inpt->but_flags & 2 )
        {
            v61.field_0.sy = 0;
            v61.field_0.sx = 0;
            v61.field_0.sz = 0;
            v61.field_2C = bact->weapon;
            v61.field_30 = bact->field_915;

            if ( v61.field_30 % 2 )
                v61.fire_point.sx = bact->fire_x;
            else
                v61.fire_point.sx = -bact->fire_x;

            v61.fire_point.sy = bact->fire_y;
            v61.fire_point.sz = bact->fire_z;
            v61.field_34 = ((arg->inpt->but_flags & 2) != 0) | 2;

            call_method(obj, 79, &v61);
        }

        if ( bact->mgun != -1 )
        {
            if ( bact->field_3D6 & 0x100 )
            {
                if ( !(arg->inpt->but_flags & 4) )
                {
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
                    arg78.field_8 = 0;
                    arg78.field_0 = 0;
                    arg78.field_4 = 256;

                    call_method(obj, 78, &arg78);
                }

                bact_arg105 arg105;

                arg105.field_0.sx = bact->field_651.m20;
                arg105.field_0.sy = bact->field_651.m21;
                arg105.field_0.sz = bact->field_651.m22;
                arg105.field_C = v106;
                arg105.field_10 = bact->field_915;

                call_method(obj, 105, &arg105);
            }
        }

        if ( arg->inpt->but_flags & 8 )
        {
            call_method(obj, 97, arg);

            v99 = bact->field_601;
        }

        if ( bact->field_3D6 & 0x200 )
        {
            bact_arg74 arg74;
            arg74.flag = 0;
            arg74.field_0 = v106;

            call_method(obj, 74, &arg74);
        }
        else
        {
            float v81;
            float v83;
            float v82;

            yw_137col v43[10];

            for (int i = 3; i >= 0; i--)
            {
                bact_arg74 arg74;
                arg74.flag = 0;
                arg74.field_0 = v106;

                call_method(obj, 74, &arg74);

                int v50 = 0;

                ypaworld_arg137 arg137;
                arg137.pos.sx = bact->field_621.sx;
                arg137.pos.sy = bact->field_621.sy;
                arg137.pos.sz = bact->field_621.sz;
                arg137.pos2.sx = bact->field_605.sx;
                arg137.pos2.sy = bact->field_605.sy;
                arg137.pos2.sz = bact->field_605.sz;
                arg137.radius = 32.0;
                arg137.collisions = v43;
                arg137.field_30 = 0;
                arg137.coll_max = 10;

                call_method(bact->wrld, 137, &arg137);

                if ( arg137.coll_count )
                {
                    v81 = 0.0;
                    v83 = 0.0;
                    v82 = 0.0;

                    for (int j = arg137.coll_count - 1; j >= 0; j--)
                    {
                        yw_137col *v31 = &arg137.collisions[ j ];

                        v82 += v31->pos2.sx;
                        v81 += v31->pos2.sy;
                        v83 += v31->pos2.sz;
                    }

                    float v78 = sqrt( POW2(v82) + POW2(v81) + POW2(v83) );

                    xyz v52;

                    if ( v78 != 0.0 )
                    {
                        float v55 = 1.0 / v78;
                        v52.sx = v82 * v55;
                        v52.sy = v81 * v55;
                        v52.sz = v83 * v55;
                    }
                    else
                    {
                        v52 = bact->field_605;
                    }

                    bact_arg88 arg88;

                    arg88.pos1 = v52;
                    arg88.pos2.sy = 2.0;
                    arg88.pos2.sx = 0.7;
                    arg88.pos2.sz = v106;

                    call_method(obj, 88,  &arg88);

                    v50 = 1;
                }

                if ( !v50 )
                {
                    ypaworld_arg136 arg136;
                    arg136.pos_x = bact->field_62D.sx;
                    arg136.pos_y = bact->field_62D.sy;
                    arg136.pos_z = bact->field_62D.sz;
                    arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
                    arg136.field_18 = bact->field_621.sy - bact->field_62D.sy;
                    arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;
                    arg136.field_40 = 0;

                    call_method(bact->wrld, 136, &arg136);

                    if ( arg136.field_20 )
                    {
                        bact_arg88 arg88;
                        arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                        arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                        arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;
                        arg88.pos2.sx = 0.7;
                        arg88.pos2.sy = 2.0;
                        arg88.pos2.sz = v106;

                        call_method(obj, 88,  &arg88);

                        v50 = 1;
                    }
                }

                if ( !v50 )
                {
                    bact->field_3D6 &= 0xFFFBFFFF;
                    break;
                }

                if ( !(bact->field_5A.samples_data[5].field_12 & 2) )
                {
                    if ( !(bact->field_3D6 & 0x40000) )
                    {
                        bact->field_3D6 |= 0x40000;

                        sub_423F74(&bact->field_5A, 5);

                        yw_arg180 arg180;
                        arg180.effects_type = 5;
                        arg180.field_4 = 1.0;
                        arg180.field_8 = v82 * 10.0 + bact->field_621.sx;
                        arg180.field_C = 10.0 * v83 + bact->field_621.sz;

                        call_method(bact->wrld, 180, &arg180);
                    }
                }
            }
        }

        bact->field_601 = v99;
    }
    else if ( bact->field_3D5 == 2 )
    {
        call_method(obj, 121, arg);
    }
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

void ypabact_func74(NC_STACK_ypabact *obj, class_stru *zis, bact_arg74 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact->field_62D = bact->field_621;

    float v48;

    if ( bact->field_3D5 == 2 )
        v48 = bact->mass * 39.2266;
    else
        v48 = bact->mass * 9.80665;

    float v39;

    float v47;
    float v49;
    float v54;

    if ( arg->flag & 1 )
    {
        v47 = 0.0;
        v49 = 0.0;
        v54 = 0.0;

        v39 = 0.0;
    }
    else
    {
        v47 = -bact->field_651.m11;
        v49 = -bact->field_651.m12;
        v54 = -bact->field_651.m10;

        v39 = bact->field_601;

        if ( bact->field_B34 & 2 )
        {
            float v5;

            if ( v54 >= 0.0 )
                v5 = 1;
            else
                v5 = -1;

            v54 = sqrt( fabs(v54) ) * v5;

            float v7;

            if ( v49 >= 0.0 )
                v7 = 1;
            else
                v7 = -1;

            v49 = sqrt( fabs(v49) ) * v7;

            float v38;

            if ( v47 >= 0.0 )
                v38 = 1;
            else
                v38 = -1;

            v47 = POW2(v47) * v38;
        }
    }

    float v11 = bact->field_611 * bact->airconst;

    float v51 = 1.0 * v48 + v47 * v39 + -bact->field_605.sy * v11;
    float v41 = 0.0 * v48 + v54 * v39 + -bact->field_605.sx * v11;
    float v44 = 0.0 * v48 + v49 * v39 + -bact->field_605.sz * v11;

    float v45 = sqrt( POW2(v51) + POW2(v41) + POW2(v44) );

    if ( bact->field_B34 & 2 )
    {
        if ( v51 >= 0.0 )
            v51 *= 3.0;
        else
            v51 *= 5.0;
    }

    if ( v45 > 0.0 )
    {
        float v18 = 1.0 / v45;
        float v19 = v45 / bact->mass * arg->field_0;

        float v42 = bact->field_605.sx * bact->field_611 + v19 * (v18 * v41);
        float v46 = bact->field_605.sy * bact->field_611 + v19 * (v18 * v51);
        float v40 = bact->field_605.sz * bact->field_611 + v19 * (v18 * v44);

        bact->field_611 = sqrt( POW2(v42) + POW2(v46) + POW2(v40) );

        if ( bact->field_611 > 0.0 )
        {
            bact->field_605.sx = v42 / bact->field_611;
            bact->field_605.sy = v46 / bact->field_611;
            bact->field_605.sz = v40 / bact->field_611;
        }
    }

    if ( fabs(bact->field_611) > 0.1 )
    {
        bact->field_621.sx += bact->field_605.sx * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sy += bact->field_605.sy * bact->field_611 * arg->field_0 * 6.0;
        bact->field_621.sz += bact->field_605.sz * bact->field_611 * arg->field_0 * 6.0;
    }

    call_method(obj, 115, 0);

    bact->field_5A.samples_data[0].pitch = bact->field_3BA;
    bact->field_5A.samples_data[0].volume = bact->field_3B6;

//  float v50;
//  if ( bact->max_pitch <= -0.8 )
//    v50 = 1.2;
//  else
//    v50 = bact->max_pitch;
//
//  float v30 = fabs(bact->field_611) * v50;
//  float v31 = bact->force * bact->force - bact->mass * 100.0 * bact->mass;
//
//  float v43 = v30 / (sqrt(v31) / bact->airconst2);
//
//  if ( v43 > v50 )
//    v43 = v50;
//
//  if ( bact->field_5A.samples_data[0].field_0 )
//  {
//    v34 = ((long double)*(_DWORD *)(bact->field_5A.samples_data[0].field_0 + 12) + (long double)bact->field_5A.samples_data[0].pitch) * v43;
//    bact->field_5A.samples_data[0].pitch += v34;
//  }
    dprintf ("COMPLETE ME %s\n", "ypabact_func74");
}

void ypabact_func75(NC_STACK_ypabact *obj, class_stru *zis, bact_arg75 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    arg->field_0 = arg->bct->field_621;

    xyz v40;

    v40.sx = arg->bct->field_621.sx - bact->field_621.sx;
    v40.sy = arg->bct->field_621.sy - bact->field_621.sy;
    v40.sz = arg->bct->field_621.sz - bact->field_621.sz;

    float v45 = sqrt( POW2(v40.sx) + POW2(v40.sy) + POW2(v40.sz) );

    float v10 = 1.0 / v45;

    v40.sx *= v10;
    v40.sy *= v10;
    v40.sz *= v10;

    bact_arg110 arg110;

    xyz *fui;
    int v54 = 0;
    int v51 = 0;

    if ( bact->field_3f8 == arg->bct )
    {
        fui = &bact->field_3f8->field_621;
        arg110.field_two = 1;
        v54 = 1;
    }
    else
    {
        fui = &bact->field_3e8->field_621;
        arg110.field_two = 0;
        v51 = 1;
    }

    NC_STACK_ypabact *a4;

    call_vtbl(bact->wrld, 3, 0x80002010, &a4, 0);

    int v16 = 0;

    if ( bact->field_915 - bact->field_945 <= 500 && bact->field_915 >= 500 )
    {
        v16 = bact->field_9b8;
    }
    else
    {
        bact->field_945 = bact->field_915;

        arg110.field_one = 2;
        v16 = call_method(obj, 110, &arg110);

        bact->field_9b8 = v16;
    }

    if ( v16 == 2 )
    {
        float v19 = fui->sx - bact->field_621.sx;
        float v18 = fui->sz - bact->field_621.sz;

        float v48 = sqrt( POW2(v19) + POW2(v18) );

        if ( bact->field_3D6 & 0x2000 )
        {
            bact->field_3D6 &= 0xBFFFFFFF;

            if ( (bact->field_621.sx < 1320.0 || bact->field_621.sz > -1320.0 || bact->field_621.sx > bact->field_18 - 1320.0 || bact->field_621.sz < bact->field_1c + 1320.0) || bact->adist_bact < v48 )
            {
                bact->field_3D6 &= 0xFFFFDFFF;
            }
            else
            {
                bact->field_91D = bact->field_915;
                bact->field_919 = bact->field_915;
            }
        }
        else
        {
            float v22 = fui->sx - bact->field_621.sx;
            float v21 = fui->sz - bact->field_621.sz;

            if ( sqrt( POW2(v21) + POW2(v22) ) >= bact->sdist_bact )
            {
                if ( bact->adist_sector <= v48 )
                    bact->field_3D6 &= 0xBFFFFFFF;
                else
                    bact->field_3D6 |= 0x40000000;
            }
            else
            {
                bact->field_3D6 &= 0xBFFFFFFF;

                if ( bact->field_3D1 == 2 || (arg->field_18 & 1 && bact->field_3D1 == 3) )
                {
                    bact->field_639.sx = bact->field_605.sx;
                    bact->field_639.sz = bact->field_605.sz;
                }
                else
                {
                    bact->field_639.sx = -bact->field_605.sx;
                    bact->field_639.sz = -bact->field_605.sz;
                }

                bact->field_91D = bact->field_915;
                bact->field_919 = bact->field_915;
                bact->field_3D6 |= 0x2000;
            }
        }
    }
    else
    {
        bact->field_3D6 &= 0xBFFFDFFF;
    }

    if ( v16 )
    {
        if ( v16 == 3 )
        {
            bact->field_3D6 &= 0xFFFFDFFF;

            if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_4 = 0;
                arg78.field_0 = 0;
                arg78.field_8 = 256;

                call_method(obj, 78, &arg78);
            }

            if ( bact->field_3f8 == arg->bct )
            {
                bact->field_3D6 &= 0xFFFFFFFD;

                bact_arg67 arg67;
                arg67.field_0 = 0;
                arg67.field_4 = 1;

                call_method(obj, 67, &arg67);

                v54 = 0;
            }
            else
            {
                bact->field_3D6 &= 0xFFFFFFFE;

                if ( (bact->parent_bacto == bact->field_32 && bact->field_32 && bact->field_32 == a4) && bact->field_3D5 != 3 && !(bact->field_3D6 & 0x4000) )
                {
                    robo_arg134 arg134;
                    arg134.unit = bact;
                    arg134.field_4 = 1;
                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.field_14 = 32;

                    call_method(bact->field_32, 134, &arg134);
                }

                bact_arg119 arg78;
                arg78.field_8 = 0;
                arg78.field_4 = 0;
                arg78.field_0 = 1;

                call_method(obj, 78, &arg78);

                bact->field_3D5 = 3;
            }
        }

        if ( v16 == 1 )
        {
            if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_4 = 0;
                arg78.field_0 = 0;
                arg78.field_8 = 256;

                call_method(obj, 78, &arg78);
            }
        }

        if ( v16 == 2 )
        {
            bact_arg101 arg101;
            arg101.pos = arg->bct->field_621;
            arg101.unkn = 2;
            arg101.radius = arg->bct->radius;

            if ( call_method(obj, 101, &arg101) )
            {
                if ( v54 )
                    bact->field_3D6 |= 2;
                else
                    bact->field_3D6 |= 1;

                bact_arg79 arg79;

                arg79.field_0.sx = bact->field_651.m20;

                if ( bact->field_24 == 2 )
                    arg79.field_0.sy = v40.sy;
                else
                    arg79.field_0.sy = bact->field_651.m21 - bact->gun_angle;

                arg79.field_0.sz = bact->field_651.m22;
                arg79.field_18 = 2;
                arg79.field_1C = arg->bct;
                arg79.field_20 = arg->field_0;
                arg79.field_2C = bact->weapon;
                arg79.field_30 = bact->field_915;

                if ( arg->field_18 & 1 )
                    arg79.fire_point.sx = bact->fire_x;
                else
                    arg79.fire_point.sx = -bact->fire_x;

                arg79.fire_point.sy = bact->fire_y;
                arg79.fire_point.sz = bact->fire_z;
                arg79.field_34 = 0;

                call_method(obj, 79, &arg79);
            }
            else
            {
                bact->field_3D6 &= 0xBFFFFFFF;
            }

            if ( v45 < 1000.0 &&   bact->mgun != -1 &&   v40.sx * bact->field_651.m20 + v40.sy * bact->field_651.m21 + v40.sz * bact->field_651.m22 > 0.85 )
            {
                if ( v54 )
                    bact->field_3D6 |= 2;
                else
                    bact->field_3D6 |= 1;

                if ( !(bact->field_3D6 & 0x100) )
                {
                    bact_arg119 arg78;
                    arg78.field_8 = 0;
                    arg78.field_0 = 0;
                    arg78.field_4 = 256;

                    call_method(obj, 78, &arg78);
                }

                bact_arg105 arg105;

                arg105.field_C = arg->field_14;
                arg105.field_10 = bact->field_915;
                arg105.field_0.sx = bact->field_651.m20;
                arg105.field_0.sy = bact->field_651.m21;
                arg105.field_0.sz = bact->field_651.m22;

                call_method(obj, 105, &arg105);
            }
            else if ( bact->field_3D6 & 0x100 )
            {
                bact_arg119 arg78;
                arg78.field_4 = 0;
                arg78.field_0 = 0;
                arg78.field_8 = 256;

                call_method(obj, 78, &arg78);
            }
        }
    }
    else
    {
        if ( v51 )
        {
            bact->field_3D6 &= 0xFFFFFFFE;

            bact_arg67 arg67;
            arg67.field_4 = 0;
            arg67.field_0 = 1;
            arg67.targ = bact->field_3ec;

            call_method(obj, 67, &arg67);
        }

        if ( v54 )
        {
            bact->field_3D6 &= 0xFFFFFFFD;

            bact_arg67 arg67;
            arg67.field_4 = 1;
            arg67.field_0 = 0;

            call_method(obj, 67, &arg67);
        }

        bact->field_3D6 &= 0xFFFFDFFF;

        if ( bact->field_3D6 & 0x100 )
        {
            bact_arg119 arg78;
            arg78.field_4 = 0;
            arg78.field_0 = 0;
            arg78.field_8 = 256;

            call_method(obj, 78, &arg78);
        }
    }
}

void ypabact_func76(NC_STACK_ypabact *obj, class_stru *zis, bact_arg75 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    int v64 = 0;
    int v68 = 0;

    xyz *fui;

    bact_arg110 arg110;

    if ( bact->field_3f8 == arg->bct )
    {
        fui = &bact->field_3fc;
        v64 = 1;

        arg110.field_two = 1;
    }
    else
    {
        fui = &bact->field_3ec;
        v68 = 1;

        arg110.field_two = 0;
    }

    NC_STACK_ypabact *a4;
    call_vtbl(bact->wrld, 3, 0x80002010, &a4, 0);

    int v65 = bact->parent_bacto == bact->field_32 && bact->field_32 && bact->field_32 == a4;

    float v12 = bact->field_621.sz - fui->sz;
    float v13 = bact->field_621.sx - fui->sx;

    float v62 = sqrt(POW2(v12) + POW2(v13));

    int v15 = 0;

    if ( bact->field_915 - bact->field_945 <= 500 && bact->field_915 >= 500 )
    {
        v15 = bact->field_9b8;
    }
    else
    {
        bact->field_945 = bact->field_915;

        arg110.field_one = 1;
        v15 = call_method(obj, 110, &arg110);

        bact->field_9b8 = v15;
    }

    if ( v15 == 2 )
    {
        float v17 = fui->sz - bact->field_621.sz;
        float v18 = fui->sx - bact->field_621.sx;

        float v63 = sqrt( POW2(v17) + POW2(v18) );

        if ( bact->field_3D6 & 0x2000 )
        {
            bact->field_3D6 &= 0xBFFFFFFF;

            if ( (bact->field_621.sx < 1320 || bact->field_621.sz > -1320.0 || bact->field_621.sx > bact->field_18 - 1320.0 || bact->field_621.sz < bact->field_1c + 1320.0) || bact->adist_sector < v63 )
            {
                bact->field_3D6 &= 0xFFFFDFFF;
            }
            else
            {
                bact->field_91D = bact->field_915;
                bact->field_919 = bact->field_915;
            }
        }
        else if ( bact->sdist_sector <= v63 )
        {
            if ( bact->adist_sector <= v63 )
                bact->field_3D6 &= 0xBFFFFFFF;
            else
                bact->field_3D6 |= 0x40000000;
        }
        else
        {
            bact->field_3D6 &= 0xBFFFFFFF;

            if ( bact->field_3D1 == 2 || (arg->field_18 & 1 && bact->field_3D1 == 3) )
            {
                bact->field_639.sx = bact->field_605.sx;
                bact->field_639.sz = bact->field_605.sz;
                bact->field_639.sy = bact->field_605.sy;
            }
            else
            {
                bact->field_639.sx = -bact->field_605.sx;
                bact->field_639.sz = -bact->field_605.sz;
                bact->field_639.sy = -bact->field_605.sy;
            }

            bact->field_91D = bact->field_915;
            bact->field_919 = bact->field_915;

            bact->field_3D6 |= 0x2000;
        }
    }
    else
    {
        bact->field_3D6 &= 0xBFFFDFFF;
    }

    if ( bact->field_3D6 & 0x100 )
    {
        bact_arg119 arg78;
        arg78.field_8 = 256;
        arg78.field_4 = 0;
        arg78.field_0 = 0;

        call_method(obj, 78, &arg78);
    }

    if ( v15 )
    {
        if ( v15 == 3 )
        {
            bact->field_3D6 &= 0xFFFFDFFF;

            if ( v64 )
            {
                bact->field_3D6 &= 0xFFFFFFFD;

                if ( v65 && bact->field_3f8 != bact->field_3e8 )
                {
                    robo_arg134 arg134;

                    arg134.field_4 = 2;
                    arg134.field_8 = bact->field_3fc.sx / 1200.0;
                    arg134.field_C = -bact->field_3fc.sz / 1200.0;
                    arg134.field_10 = 0;
                    arg134.field_14 = 22;
                    arg134.unit = bact;

                    call_method(bact->field_32, 134, &arg134);
                }

                bact_arg67 arg67;
                arg67.field_4 = 1;
                arg67.field_0 = 0;

                call_method(obj, 67, &arg67);

                v64 = 0;
            }
            else
            {
                bact->field_3D6 &= 0xFFFFFFFE;

                if ( v65 && bact->field_3D5 != 3 )
                {
                    robo_arg134 arg134;

                    arg134.field_10 = 0;
                    arg134.field_C = 0;
                    arg134.field_8 = 0;
                    arg134.unit = bact;
                    arg134.field_4 = 1;
                    arg134.field_14 = 32;

                    call_method(bact->field_32, 134, &arg134);
                }

                bact->field_3D5 = 3;
            }
        }

        if ( v15 == 2 )
        {
            if ( v68 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( !(bact->field_3D6 & 1) && v65 && bact->field_3f8 != bact->field_3e8 )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = bact->field_3ec.sx / 1200.0;
                        arg134.field_C = -bact->field_3ec.sz / 1200.0;
                        arg134.unit = bact;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        call_method(bact->field_32, 134, &arg134);
                    }

                    bact->field_3D6 |= 1;
                }

                call_method(obj, 91, &bact->field_3ec);

                arg->field_0 = bact->field_3ec;
            }

            if ( v64 )
            {
                if ( v62 < 1200.0 )
                {
                    if ( v65 && !(bact->field_3D6 & 2) )
                    {
                        robo_arg134 arg134;

                        arg134.field_4 = 3;
                        arg134.field_8 = bact->field_3fc.sx / 1200.0;
                        arg134.field_C = -bact->field_3fc.sz / 1200.0;
                        arg134.unit = bact;
                        arg134.field_10 = 0;
                        arg134.field_14 = 20;

                        call_method(bact->field_32, 134, &arg134);
                    }

                    bact->field_3D6 |= 2;
                }

                call_method(obj, 91, &bact->field_3fc);

                arg->field_0 = bact->field_3fc;
            }

            bact_arg101 arg101;
            arg101.unkn = 1;
            arg101.pos.sx = arg->field_0.sx;
            arg101.pos.sy = arg->field_0.sy;
            arg101.pos.sz = arg->field_0.sz;

            if ( call_method(obj, 101, &arg101) )
            {
                float v40 = bact->field_621.sy + bact->fire_y - arg->field_0.sy;
                float v41 = bact->field_621.sx + bact->fire_x - arg->field_0.sx;
                float v43 = bact->field_621.sz + bact->fire_z - arg->field_0.sz;

                float v60 = sqrt( POW2(v40) + POW2(v41) + POW2(v43) );

                if ( v60 < 0.01 )
                    v60 = 0.01;

                if ( v64 )
                    bact->field_3D6 |= 2;
                else
                    bact->field_3D6 |= 1;

                float v44 = 1.0 / v60;

                bact_arg79 arg79;

                arg79.field_0.sx = -(bact->field_621.sx + bact->fire_x - arg->field_0.sx) * v44;
                arg79.field_0.sy = -(bact->field_621.sy + bact->fire_y - arg->field_0.sy) * v44;
                arg79.field_0.sz = -(bact->field_621.sz + bact->fire_z - arg->field_0.sz) * v44;
                arg79.field_18 = 1;
                arg79.field_1C = arg->bct;
                arg79.field_20 = arg->field_0;
                arg79.field_2C = bact->weapon;
                arg79.field_30 = bact->field_915;

                if ( arg->field_18 & 1 )
                    arg79.fire_point.sx = bact->fire_x;
                else
                    arg79.fire_point.sx = -bact->fire_x;

                arg79.fire_point.sy = bact->fire_y;
                arg79.fire_point.sz = bact->fire_z;
                arg79.field_34 = 0;

                call_method(obj, 79, &arg79);
            }
            else
            {
                bact->field_3D6 &= 0xBFFFFFFF;
            }
        }
    }
    else
    {
        bact->field_3D6 &= 0xFFFFDFFF;

        if ( v68 )
        {
            if ( v65 )
            {
                robo_arg134 arg134;

                arg134.unit = bact;
                arg134.field_4 = 4;
                arg134.field_8 = bact->field_3ec.sx / 1200.0;
                arg134.field_C = -bact->field_3ec.sz / 1200.0;
                arg134.field_14 = 18;
                arg134.field_10 = 0;

                call_method(bact->field_32, 134, &arg134);
            }

            bact->field_3D6 &= 0xFFFFFFFE;

            bact_arg67 arg67;
            arg67.field_0 = 1;
            arg67.targ.sx = bact->field_621.sx;
            arg67.targ.sz = bact->field_621.sz;
            arg67.field_4 = 0;

            call_method(obj, 67, &arg67);
        }

        if ( v64 )
        {
            if ( v65 )
            {
                robo_arg134 arg134;

                arg134.unit = bact;
                arg134.field_4 = 4;
                arg134.field_8 = bact->field_3fc.sx / 1200.0;
                arg134.field_10 = 0;
                arg134.field_14 = 18;
                arg134.field_C = -bact->field_3fc.sz / 1200.0;

                call_method(bact->field_32, 134, &arg134);
            }

            bact->field_3D6 &= 0xFFFFFFFD;

            bact_arg67 arg67;
            arg67.field_0 = 0;
            arg67.field_4 = 1;
            call_method(obj, 67, &arg67);
        }
    }
}

void ypabact_func77__sub0(__NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2)
{
    if ( bact1->field_3D6 & 0x4000000 )
    {
        for (int i = 0; i < 32; i++)
            bact2->field_418[i] = bact1->field_418[i];

        bact2->field_3D6 |= 0x4000000;

        if ( bact1->field_3D6 & 0x8000000 )
            bact2->field_3D6 |= 0x8000000;
        else
            bact2->field_3D6 &= 0xF7FFFFFF;

        bact2->field_59A = bact1->field_59A;
        bact2->field_598 = bact1->field_598;
    }
}

void ypabact_func77(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    char v51[32];

    if ( !(bact->field_3D6 & 0x400) )
    {
        int maxy;
        int maxx;

        call_vtbl(bact->wrld, 3, 0x80002000, &maxx, 0);
        call_vtbl(bact->wrld, 3, 0x80002001, &maxy, 0);


//    *(_DWORD *)v51 = 1009;
//    v51[12] = bact->owner;
//    *(_DWORD *)&v51[16] = bact->ypabact__id;
//    *(_DWORD *)&v51[20] = 0;
//    v51[28] = 0;
//    v51[29] = bact->field_24;
//    int v3;
//
//    if ( bact->field_9B1 )
//      v3 = bact->field_9B1->ypabact__id;
//    else
//      v3 = 0;
//
//    *(_DWORD *)&v51[24] = v3;
//    v51[30] = bact->field_9B5;

        _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

        bact_node *v74 = NULL;

        bact_node *v4 = (bact_node *)bact->list2.head;

        if (v4->next)
        {

            while (v4->next)
            {
                bact_node *next_node = (bact_node *)v4->next;

                if ( v4->bact->field_3D5 == 2 )
                {
                    if ( (size_t)bact->parent_bacto <= 2 )
                        call_method(bact->wrld, 134, v4->bacto);
                    else
                        call_method(bact->parent_bacto, 72, v4->bacto);

                    v4->bact->field_3D6 |= 0x10000000;
                }
                else
                {
                    float v9 = v4->bact->field_621.sx - bact->field_621.sx;
                    float v10 = v4->bact->field_621.sz - bact->field_621.sz;

                    float v73 = POW2(v9) + POW2(v10);

                    float v14;

                    if ( v74 )
                        v14 = POW2(v74->bact->field_621.sx - bact->field_621.sx) + POW2(v74->bact->field_621.sz - bact->field_621.sz);
                    else
                        v14 = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0;

                    if ( v4->bact->field_24 == 7 )
                        v73 = (POW2(maxx) + POW2(maxy)) * 1200.0 * 1200.0 - 1000.0;

                    if ( v73 <= v14 )
                        v74 = v4;
                }

                v4 = next_node;
            }

            if ( v74 )
            {
                if ( (size_t)bact->parent_bacto != 1 )
                    call_method(bact->parent_bacto, 72, v74->bacto);
                else
                    call_method(bact->wrld, 134, v74->bacto);

                while ( 1 )
                {
                    bact_node *v16 = (bact_node *)bact->list2.head;

                    if ( !v16->next )
                        break;

                    call_method(v74->bacto, 72, v16->bacto);
                }

                bact_arg67 arg67;
                arg67.targ = bact->field_3ec;
                arg67.field_8 = bact->field_3e8;
                arg67.field_0 = bact->field_3DE;
                arg67.field_4 = 0;

                call_method(v74->bacto, 67, &arg67);

                ypabact_func77__sub0(bact, v74->bact);

                v74->bact->field_2E = bact->field_2E;
                v74->bact->field_3D4 = bact->field_3D4;

                if ( yw->field_757E )
                {
//          if ( bact->owner )
//            *(_DWORD *)&v51[20] = v74->bact->ypabact__id;
                }
            }
            else
            {
                bact_node *v64 = (bact_node *)bact->list2.head;

                while(v64->next)
                {
                    bact_node *v21 = (bact_node *)v64->bact->list2.head;

                    while (v21->next)
                    {
                        bact_node *next2 = (bact_node *)v21->next;

                        call_method(bact->wrld, 134, v21->bacto);

                        if ( v21->bact->field_3D5 != 2 )
                            ypa_log_out("Scheisse, da hфngt noch ein Lebendiger unter der Leiche! owner %d, state %d, class %d\n", v21->bact->owner, v21->bact->field_3D5, bact->field_24);

                        v21 = next2;
                    }

                    bact_node *next1 = (bact_node *)v64->next;

                    call_method(bact->wrld, 134, v64->bacto);

                    v64 = next1;
                }
            }
        }

        NC_STACK_ypabact *v76;
        call_vtbl(bact->wrld, 3, 0x80002010, &v76, 0);

        if ( !v74 && bact->field_32 == bact->parent_bacto && !(bact->field_3D6 & 0x10000000) )
        {
            robo_arg134 v53;

            if ( v76 == bact->field_32 )
            {
                if ( bact->field_24 == 9 )
                {
                    if ( bact->weapon != -1 || -1 != bact->mgun )
                    {
                        v53.field_14 = 80;
                        v53.field_4 = 31;
                    }
                    else
                    {
                        v53.field_14 = 80;
                        v53.field_4 = 32;
                    }

                    v53.field_10 = 0;
                    v53.field_C = 0;
                    v53.field_8 = 0;
                    v53.unit = bact;

                    call_method(bact->field_32, 134, &v53);
                }
                else
                {
                    if ( !(bact->field_3D6 & 0x2000000) )
                    {
                        v53.field_8 = bact->field_2E;
                        v53.unit = bact;
                        v53.field_10 = 0;
                        v53.field_14 = 44;
                        v53.field_C = 0;
                        v53.field_4 = 8;

                        call_method(bact->field_32, 134, &v53);
                    }
                }
            }
            else
            {
                if ( bact->field_9B1 && v76 == bact->field_9B1->field_32 )
                {
                    v53.field_4 = 5;
                    v53.unit = bact->field_9B1;
                    v53.field_8 = bact->field_3e0;
                    v53.field_10 = 0;
                    v53.field_C = 0;
                    v53.field_14 = 36;

                    call_method(bact->field_32, 134, &v53);
                }
            }

        }

        while ( 1 )
        {
            bact_node *v30 = (bact_node *)RemHead(&bact->field_B48);

            if ( !v30 )
                break;

            bact_node *v68, *v69;

            call_vtbl(v30->bacto, 3, 0x80001011, &v68, 0);
            call_vtbl(v30->bacto, 3, 0x80001012, &v69, 0);

            if ( v30 == v68 )
            {
                v30->bact->field_3e8 = NULL;
                v30->bact->field_3DE = 0;
                v30->bact->field_945 = 0;
            }
            else if ( v30 == v69 )
            {
                v30->bact->field_3f8 = NULL;
                v30->bact->field_3DF = 0;
                v30->bact->field_945 = 0;
            }
            else
            {
                ypa_log_out("Hein BlЎd\n");
            }
        }

        if ( (size_t)bact->parent_bacto <= 2 )
        {
            while ( 1 )
            {
                bact_node *v36 = (bact_node *)RemHead(&bact->list3);

                if ( !v36 )
                    break;

                call_method(v36->bacto, 128, 0);

                bact_arg119 arg119;
                arg119.field_0 = 2;
                arg119.field_8 = 0;
                arg119.field_4 = 0;
                call_method(v36->bacto, 119, &arg119);

                bact_arg67 arg67;
                arg67.field_0 = 0;
                arg67.field_4 = 0;
                call_method(v36->bacto, 67, &arg67);

                v36->bact->parent_bacto = NULL;

                call_method(bact->wrld, 144, v36->bacto);
            }
        }
        else
        {
            while ( 1 )
            {
                bact_node *v33 = (bact_node *)RemHead(&bact->list3);

                if ( !v33 )
                    break;

                AddTail(&bact->parent_bact->list3, v33);

                call_vtbl(v33->bacto, 2, 0x80002000, bact->parent_bact, 0);
            }
        }

        if ( bact->field_3DF == 2 )
            Remove(&bact->field_B64);

        if ( bact->field_3DE == 2 )
            Remove(&bact->field_B54);


        bact->field_3DF = 0;
        bact->field_3DE = 0;

        if ( bact->field_B64.next && bact->field_B64.prev )
        {
            ypa_log_out("ALARM!!! st-Node noch in liste!!! owner %d, class %d\n", bact->owner, bact->field_24);
            Remove(&bact->field_B64);
        }

        if ( bact->field_B54.next && bact->field_B54.prev )
        {
            ypa_log_out("ALARM!!! pt-Node noch in liste!!! owner %d, class %d\n", bact->owner, bact->field_24);
            Remove(&bact->field_B54);
        }

        bact->field_3D5 = 2;
        bact->field_2E = 0;
        bact->field_3D6 |= 0x400;
        bact->field_955 = bact->field_915;

        if ( bact->field_3D6 & 0x200 )
        {
            if ( bact->field_6B9 == 1 || bact->field_6B9 == 6 )
            {
                bact_arg119 arg119;
                arg119.field_8 = 0;
                arg119.field_0 = 0;
                arg119.field_4 = 2048;

                call_method(obj, 119, &arg119);

                if ( yw->field_757E )
                {
//          if ( bact->owner )
//            v51[28] = 1;
                }
            }
        }

        if ( bact->field_B34 & 2 )
        {
            if ( !(bact->field_B34 & 1) )
            {
                if ( bact->parent_bact )
                    call_vtbl(obj, 2, 0x80001005, 0, 0);
            }
        }

        if ( yw->field_757E )
        {
            if ( bact->owner )
            {
                if ( bact->field_24 != 3 )
                {
                    yw_arg181 arg181;
                    arg181.value = v51;
                    arg181.field_14 = 2;
                    arg181.val_size = 32;
                    arg181.field_10 = 0;
                    arg181.field_18 = 1;

                    call_method(yw->self_full, 181, &arg181);
                }
            }
        }

        if ( bact->owner )
        {
            if ( !(bact->field_3D6 & 0x2000000) )
            {
                if ( bact->field_9B1 )
                {
                    yw_arg184_t4 arg184;
                    arg184.type = 3;
                    arg184.field_1 = 8 * bact->field_9B1->owner | bact->owner;

                    int v70;
                    call_vtbl(bact->field_9B1->self, 3, 0x80001004, &v70, 0);

                    if ( v70 || bact->field_9B1->field_3D6 & 0x800000 )
                        arg184.field_1 |= 0x80;

                    call_vtbl(bact->self, 3, 0x80001004, &v70, 0);

                    if ( v70 || bact->field_3D6 & 0x800000 )
                        arg184.field_1 |= 0x40;

                    arg184.field_2 = bact->id;

                    if ( bact->field_24 == 3 )
                        arg184.field_2 |= 0x8000;

                    arg184.field_4 = bact->field_621.sx * 256.0 / bact->field_18;
                    arg184.field_5 = bact->field_621.sz * 256.0 / bact->field_1c;

                    call_method(bact->wrld, 184, &arg184);
                }
            }
        }
    }
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

size_t ypabact_func79(NC_STACK_ypabact *obj, class_stru *zis, bact_arg79 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    NC_STACK_ypabact *wobj = NULL;

    WeapProto *wprotos;
    call_vtbl(bact->wrld, 3, 0x80002012, &wprotos, 0);

    if ( arg->field_2C == 255 )
        return 0;

    if ( bact->field_97D )
    {
        int v4;

        if ( bact->field_B34 & 2 )
            v4 = wprotos[arg->field_2C].shot_time_user;
        else
            v4 = wprotos[arg->field_2C].shot_time;

        if ( wprotos[arg->field_2C].salve_shots )
        {
            if ( wprotos[arg->field_2C].salve_shots <= bact->field_9A5 )
                v4 = wprotos[arg->field_2C].salve_delay;
        }

        if ( arg->field_30 - bact->field_97D < v4 )
            return 0;
    }

    if ( bact->field_9A5 < wprotos[arg->field_2C].salve_shots )
        bact->field_9A5 += 1;
    else
        bact->field_9A5 = 1;

    if ( bact->field_B34 & 2 )
    {
        yw_arg180 v26;

        if ( wprotos[arg->field_2C].model_id & 2 )
            v26.effects_type = 0;
        else if ( wprotos[arg->field_2C].model_id & 0x10 )
            v26.effects_type = 1;
        else
            v26.effects_type = 2;

        call_method(bact->wrld, 180, &v26);
    }

    int v13;

    if ( bact->num_weapons <= 1 )
        v13 = 1;
    else
        v13 = bact->num_weapons;

    for (int i = 0; i < v13; i++)
    {
        float v37;

        if ( v13 == 1 )
            v37 = arg->fire_point.sx;
        else
        {
            float v14 = fabs(arg->fire_point.sx);
            v37 = (i * 2) * v14 / (v13 - 1) - v14;
        }

        ypaworld_arg146 arg147;
        arg147.vehicle_id = arg->field_2C;
        arg147.pos.sx = bact->field_621.sx + bact->field_651.m00 * v37 + bact->field_651.m10 * arg->fire_point.sy + bact->field_651.m20 * arg->fire_point.sz;
        arg147.pos.sy = bact->field_621.sy + bact->field_651.m01 * v37 + bact->field_651.m11 * arg->fire_point.sy + bact->field_651.m21 * arg->fire_point.sz;
        arg147.pos.sz = bact->field_621.sz + bact->field_651.m02 * v37 + bact->field_651.m12 * arg->fire_point.sy + bact->field_651.m22 * arg->fire_point.sz;

        wobj = (NC_STACK_ypabact *)call_method(bact->wrld, 147, &arg147);

        if ( !wobj )
            return 0;

        __NC_STACK_ypabact *wbact;
        call_vtbl(wobj, 3, 0x80001003, &wbact, 0);

        call_vtbl(wobj, 2, 0x80002000, bact, 0);

        call_vtbl(wobj, 2, 0x80002010, (int)arg147.pos.sy, 0);

        wbact->owner = bact->owner;

        if ( bact->field_24 != 9 )
            bact->energy -= wbact->energy / 300;

        if ( arg->field_0.sx != 0.0 || arg->field_0.sy != 0.0 || arg->field_0.sz != 0.0 )
        {
            wbact->field_605 = arg->field_0;
        }
        else
        {
            wbact->field_605.sx = bact->field_651.m20;
            wbact->field_605.sy = bact->field_651.m21;
            wbact->field_605.sz = bact->field_651.m22;
        }

        wbact->field_611 = bact->field_611 + wprotos[ arg->field_2C ].start_speed;

        if ( !(wprotos[arg->field_2C].model_id & 0x12) )
            wbact->field_611 *= 0.2;

        wbact->field_651.m20 = wbact->field_605.sx;
        wbact->field_651.m21 = wbact->field_605.sy;
        wbact->field_651.m22 = wbact->field_605.sz;

        wbact->field_651.m00 = bact->field_651.m00;
        wbact->field_651.m01 = bact->field_651.m01;
        wbact->field_651.m02 = bact->field_651.m02;

        wbact->field_651.m10 = wbact->field_651.m21 * wbact->field_651.m02 - wbact->field_651.m22 * wbact->field_651.m01;
        wbact->field_651.m11 = wbact->field_651.m22 * wbact->field_651.m00 - wbact->field_651.m20 * wbact->field_651.m02;
        wbact->field_651.m12 = wbact->field_651.m20 * wbact->field_651.m01 - wbact->field_651.m00 * wbact->field_651.m21;

        if ( i == 0 )
        {
            if ( arg->field_34 & 1 )
            {
                wbact->field_621.sx = wbact->field_621.sx - wbact->field_651.m20 * 30.0;
                wbact->field_621.sy = wbact->field_621.sy - wbact->field_651.m21 * 30.0;
                wbact->field_621.sz = wbact->field_621.sz - wbact->field_651.m22 * 30.0;
            }
        }

        if ( wbact->parent_bacto )
        {
            Remove(&wbact->list_node);
            wbact->parent_bacto = NULL;
        }

        bact_node *a2a = NULL;

        call_vtbl(wobj, 3, 0x80002003, &a2a, 0);
        AddTail(&bact->list3, a2a);

        int v42 = 0;
        call_vtbl(wobj, 3, 0x80002002, &v42, 0);

        if ( v42 == 3 )
        {
            bact_arg67 arg67;

            arg67.field_8 = arg->field_1C;
            arg67.field_0 = arg->field_18;
            arg67.field_4 = 0;
            arg67.targ = arg->field_20;

            call_method(wobj, 67, &arg67);

            if ( arg->field_34 & 2 )
            {
                if ( arg->field_18 == 1 )
                    wbact->field_3ec.sy = arg->field_20.sy;
            }
        }

        char v23[68];

//          *(float *)&v23[56] = arg->field_20.sx;
//          *(float *)&v23[60] = arg->field_20.sy;
//          *(float *)&v23[64] = arg->field_20.sz;

        if ( v42 == 2 )
        {
            wbact->field_3DE = 4;
            wbact->field_645.sx = wbact->field_605.sx;
            wbact->field_645.sy = wbact->field_605.sy;
            wbact->field_645.sz = wbact->field_605.sz;
        }

        wbact->field_32 = bact->field_32;
        bact->field_97D = arg->field_30;

        sub_423F74(&wbact->field_5A, 1);

        _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

        if ( yw->field_757E )
        {
            wbact->ypabact__id |= bact->owner << 24;
//            *(_DWORD *)v23 = 1004;
//            v23[12] = bact->owner;
//            *(_DWORD *)&v23[28] = wbact->ypabact__id;
//            *(_DWORD *)&v23[32] = bact->ypabact__id;
//            v23[53] = arg->field_2C;
//            *(float *)&v23[16] = arg147.pos.sx;
//            *(float *)&v23[20] = arg147.pos.sy;
//            *(float *)&v23[24] = arg147.pos.sz;
//            v23[52] = 0;
//            *(float *)&v23[36] = wbact->field_605.sx * wbact->field_611;
//            *(float *)&v23[40] = wbact->field_605.sy * wbact->field_611;
//            *(float *)&v23[44] = wbact->field_605.sz * wbact->field_611;
//            v23[54] = wbact->field_3DE;

            if ( wbact->field_3DE == 2 )
            {
//              *(_DWORD *)&v23[48] = wbact->field_3e8->ypabact__id;
//              v23[55] = wbact->field_3e8->owner;
            }

            yw_arg181 arg181;
            arg181.value = v23;
            arg181.val_size = 68;
            arg181.field_14 = 2;
            arg181.field_10 = 0;
            arg181.field_18 = 1;

            call_method(yw->self_full, 181, &arg181);
        }

        if ( arg->field_34 & 1 )
        {
            if ( i == 0 )
            {
                if ( bact->field_B34 & 1 )
                {
                    call_vtbl(obj, 2, 0x80001004, 0, 0);
                    call_vtbl(wobj, 2, 0x80001004, 1, 0);
                }
            }
        }

        if ( arg->field_18 != 2 )
        {
            int life_time_nt = wprotos[ arg->field_2C ].life_time_nt;

            if ( life_time_nt )
                call_vtbl(wobj, 2, 0x80002004, life_time_nt, 0);
        }
    }

    if ( bact->kill_after_shot )
    {
        if ( bact->field_B34 & 2 )
        {
            call_vtbl(obj, 2, 0x80001004, 0, 0);
            call_vtbl(wobj, 2, 0x80001004, 1, 0);
        }

        bact_arg84 arg84;
        arg84.unit = bact->parent_bact;
        arg84.energy = -2 * bact->energy_2;

        call_method(obj, 84, &arg84);
    }
    return 1;
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

void ypabact_func83(NC_STACK_ypabact *obj, class_stru *zis, bact_arg83 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float v81 = 50.0 / bact->mass;
    float v79 = arg->energ * 0.0004;

    float v60 = bact->field_621.sx - arg->pos.sx;
    float v61 = bact->field_621.sy - arg->pos.sy;
    float v62 = bact->field_621.sz - arg->pos.sz;

    float v84 = sqrt(POW2(v60) + POW2(v61) + POW2(v62));

    if ( v84 <= bact->radius )
    {
        float v28 = 2.5 * arg->mass;
        float v30 = 1.0 / (bact->mass + arg->mass);

        float v63 = (v28 * arg->pos2.sx * arg->force + bact->mass * bact->field_605.sx * bact->field_611) * v30;
        float v64 = (v28 * arg->pos2.sy * arg->force + bact->mass * bact->field_605.sy * bact->field_611) * v30;
        float v65 = (v28 * arg->pos2.sz * arg->force + bact->mass * bact->field_605.sz * bact->field_611) * v30;

        bact->field_611 = sqrt(POW2(v63) + POW2(v64) + POW2(v65));

        if ( bact->field_611 > 0.0 )
        {
            bact->field_605.sx = v63 / bact->field_611;
            bact->field_605.sy = v64 / bact->field_611;
            bact->field_605.sz = v65 / bact->field_611;
        }

        v60 = arg->pos2.sx;
        v61 = arg->pos2.sy;
        v62 = arg->pos2.sz;

        v84 = 1.0;
    }
    else
    {
        float v14 = 1.0 / v84;

        v60 *= v14;
        v61 *= v14;
        v62 *= v14;

        float v63 = bact->field_605.sx * bact->field_611;
        v63 += (v60 * v81 * v79) * v14;

        float v64 = bact->field_605.sy * bact->field_611;
        v64 += (v61 * v81 * v79) * v14;

        float v65 = bact->field_605.sz * bact->field_611;
        v65 += (v62 * v81 * v79) * v14;

        bact->field_611 = sqrt( POW2(v64) + POW2(v63) + POW2(v65) );

        if ( bact->field_611 > 0.0 )
        {
            bact->field_605.sx = v63 / bact->field_611;
            bact->field_605.sy = v64 / bact->field_611;
            bact->field_605.sz = v65 / bact->field_611;
        }
    }

    call_method(obj, 115, 0);

    float v40 = bact->field_651.m21 * v61 + bact->field_651.m20 * v60 + bact->field_651.m22 * v62;

    bact->field_3D6 &= 0xFFFFFDFF;

    float v80 = v81 * 0.01 * v79 / v84;

    if ( fabs(v40) <= 0.7071 )
    {
        if ( v60 * bact->field_651.m22 - v62 * bact->field_651.m20 >= 0.0 )
        {
            mat3x3 v56;
            v56.m00 = cos(v80);
            v56.m01 = sin(v80);
            v56.m02 = 0;
            v56.m10 = -sin(v80);
            v56.m11 = cos(v80);
            v56.m12 = 0;
            v56.m20 = 0;
            v56.m21 = 0;
            v56.m22 = 1.0;

            mat3x3 v59;
            mat_mult(&v56, &bact->field_651, &v59);

            bact->field_651 = v59;
        }
        else
        {
            mat3x3 v44;
            v44.m00 = cos(-v80);
            v44.m01 = sin(-v80);
            v44.m02 = 0;
            v44.m10 = -sin(-v80);
            v44.m11 = cos(-v80);
            v44.m12 = 0;
            v44.m20 = 0;
            v44.m21 = 0;
            v44.m22 = 1.0;

            mat3x3 v45;
            mat_mult(&v44, &bact->field_651, &v45);

            bact->field_651 = v45;
        }
    }
    else if ( v40 <= 0.7071 )
    {
        mat3x3 v58;
        v58.m00 = 1.0;
        v58.m01 = 0;
        v58.m02 = 0;
        v58.m10 = 0;
        v58.m11 = cos(v80);
        v58.m12 = sin(v80);
        v58.m20 = 0;
        v58.m21 = -sin(v80);
        v58.m22 = cos(v80);

        mat3x3 v57;
        mat_mult(&v58, &bact->field_651, &v57);

        bact->field_651 = v57;
    }
    else
    {
        mat3x3 mat1;
        mat1.m00 = 1.0;
        mat1.m01 = 0;
        mat1.m02 = 0;
        mat1.m10 = 0;
        mat1.m11 = cos(-v80);
        mat1.m12 = sin(-v80);
        mat1.m20 = 0;
        mat1.m21 = -sin(-v80);
        mat1.m22 = cos(-v80);

        mat3x3 dst;
        mat_mult(&mat1, &bact->field_651, &dst);

        bact->field_651 = dst;
    }
}

void ypabact_func84(NC_STACK_ypabact *obj, class_stru *zis, bact_arg84 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    int v6 = 0;

    int a4;
    call_vtbl(bact->wrld, 3, 0x8000201F, &a4, 0);

    if ( !bact->wrld || !(bact->field_B34 & 1) || !a4 || arg->energy <= -1000000 )
    {
        _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

        if ( yw->field_757E )
            v6 = 1;

        if ( !v6 || !arg->unit || bact->owner == arg->unit->owner )
        {
            bact->energy += arg->energy;

            if ( arg->unit )
                bact->field_9B5 = arg->unit->owner;
            else
                bact->field_9B5 = 0;

            if ( bact->energy <= 0 )
            {
                bact->field_9B1 = arg->unit;
                bact->field_3D6 &= 0xFFFFFDFF;

                bact_arg119 v16;
                v16.field_0 = 2;
                v16.field_8 = 0;
                v16.field_4 = 0;

                call_method(obj, 78, &v16);

                call_method(obj, 77, 0);
            }
        }
        else
        {
            char v14[32];
//      *(_DWORD *)v14 = 1010;
//      v14[12] = bact->owner;
//      *(_DWORD *)&v14[16] = bact->ypabact__id;
//      *(_DWORD *)&v14[20] = arg->energy;
//
//      if ( arg->unit )
//      {
//        *(_DWORD *)&v14[24] = arg->unit->ypabact__id;
//        v14[28] = arg->unit->owner;
//      }
//      else
//      {
//        *(_DWORD *)&v14[24] = 0;
//        v14[28] = 0;
//      }

            yw_arg181 arg181;

            arg181.field_10 = 0;
            arg181.field_14 = 2;
            arg181.value = v14;
            arg181.val_size = 32;
            arg181.field_18 = 1;

            call_method(yw->self_full, 181, &arg181);
        }
    }
}

void ypabact_func85(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func85");
    call_parent(zis, obj, 85, arg);
}


void ypabact_func86__sub1(__NC_STACK_ypabact *bact)
{
    if ( bact->field_605.sx < 0.0 )
        bact->field_605.sx -= 7.0;

    if ( bact->field_605.sz < 0.0 )
        bact->field_605.sz -= 7.0;

    if ( bact->field_605.sx >= 0.0 )
        bact->field_605.sx += 7.0;

    if ( bact->field_605.sz >= 0.0 )
        bact->field_605.sz += 7.0;

    if ( bact->field_611 < 15.0 )
        bact->field_611 = 15.0;

    float v4 = sqrt( POW2(bact->field_605.sx) + POW2(bact->field_605.sy) + POW2(bact->field_605.sz) );

    if ( v4 <= 0.001 )
    {
        bact->field_605.sx = 0;
        bact->field_605.sy = 1.0;
        bact->field_605.sz = 0;
    }
    else
    {
        float v6 = 1.0 / v4;

        bact->field_605.sx *= v6;
        bact->field_605.sy *= v6;
        bact->field_605.sz *= v6;
    }
}

void sub_48AB14(__NC_STACK_ypabact *bact, xyz *a2)
{
    float v36 = bact->field_651.m11 * a2->sz - bact->field_651.m12 * a2->sy;
    float v38 = bact->field_651.m12 * a2->sx - bact->field_651.m10 * a2->sz;
    float v37 = bact->field_651.m10 * a2->sy - bact->field_651.m11 * a2->sx;

    float v29 = sqrt(POW2(v36) + POW2(v38) + POW2(v37));

    if ( v29 != 0.0 )
    {
        float v10 = 1.0 / v29;

        float v42 = bact->field_651.m10 * a2->sx + bact->field_651.m11 * a2->sy + bact->field_651.m12 * a2->sz;

        v36 *= v10;
        v38 *= v10;
        v37 *= v10;

        if ( v42 > 1.0 )
            v42 = 1.0;

        if ( v42 < -1.0 )
            v42 = -1.0;

        float v11 = acos(v42);

        if ( v11 > 0.001 )
        {
            float v12 = sin(-v11);
            float v22 = cos(v11);

            mat3x3 mat2;
            mat2.m00 = (1.0 - v22) * v36 * v36 + v22;
            mat2.m01 = (1.0 - v22) * v38 - v12 * v37;
            mat2.m02 = (1.0 - v22) * v37 * v36 + v12 * v38;

            mat2.m10 = (1.0 - v22) * v38 + v12 * v37;
            mat2.m11 = (1.0 - v22) * v38 * v38 + v22;
            mat2.m12 = (1.0 - v22) * v38 * v37 - v12 * v36;

            mat2.m20 = (1.0 - v22) * v37 * v36 - v12 * v38;
            mat2.m21 = (1.0 - v22) * v38 * v37 + v12 * v36;
            mat2.m22 = (1.0 - v22) * v37 * v37 + v22;

            mat3x3 v20;

            mat_mult(&bact->field_651, &mat2, &v20);

            bact->field_651 = v20;
        }
    }
}

void ypabact_func86__sub0(__NC_STACK_ypabact *bact, int a2)
{
    bact->field_3D6 |= 0x100000;

    if ( bact->field_a20 > bact->field_a24 )
    {
        float v5 = bact->maxrot * a2 / 1000.0;


        bact->field_a18 += bact->field_a1c * a2 / 1000.0;
        bact->field_a14 += bact->field_a18 * (a2 / 1000.0);

        bact->field_67D.sx = bact->field_a14;
        bact->field_67D.sy = bact->field_a14;
        bact->field_67D.sz = bact->field_a14;

        float v6 = sin(v5);
        float v20 = cos(v5);

        mat3x3 mat1;

        mat1.m00 = v20;
        mat1.m01 = 0;
        mat1.m02 = v6;

        mat1.m10 = 0;
        mat1.m11 = 1.0;
        mat1.m12 = 0;

        mat1.m20 = -v6;
        mat1.m21 = 0;
        mat1.m22 = v20;

        mat3x3 v16;

        mat_mult(&mat1, &bact->field_651, &v16);

        bact->field_651 = v16;

        int v14 = 0;
        for (int i = 0; i < 32; i++)
        {
            if ( bact->models_bases[i] )
                v14++;
        }

        if ( v14 )
        {
            int v15 = bact->field_a24 * v14 / bact->field_a20;

            bact->current_vp.base = bact->models_bases[v15];
            bact->current_vp.trigo = bact->models_trigo[v15];
        }

        bact->field_a24 += a2;
    }
    else
    {
        bact->field_B74 = -1;
        call_method(bact->self, 118, bact->self);
    }
}

size_t ypabact_func86(NC_STACK_ypabact *obj, class_stru *zis, bact_arg86 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    yw_137col v58[10];

    int v85 = 0;

    if ( bact->field_3D6 & 0x1000000 )
    {
        ypabact_func86__sub0(bact, arg->field_two);
    }
    else
    {
        float v84;
        float v90;

        if ( bact->field_B34 & 1 )
        {
            v84 = bact->vwr_radius;
            v90 = bact->vwr_overeof;
        }
        else
        {
            v84 = bact->radius;
            v90 = bact->overeof;
        }

        if ( bact->field_24 == 3 )
            v90 = 60.0;

        float v83 = bact->field_651.m10;
        float v82 = -bact->field_651.m12;

        float v94 = arg->field_two / 1000.0;

        float v105 = sqrt( POW2(v82) + POW2(v83) );

        if ( v105 > 0.001 && !(arg->field_one & 1) )
        {
            float v6 = 1.0 / v105;

            float v101 = bact->field_651.m11;

            v82 *= v6;
            v83 *= v6;

            if ( v101 > 1.0 )
                v101 = 1.0;

            if ( v101 < -1.0 )
                v101 = -1.0;

            float v108 = acos(v101);
            float v81 = bact->maxrot * v94;

            if ( v108 > v81 )
                v108 = v81;

            if ( fabs(v108) > BACT_MIN_ANGLE )
            {
                float v7 = sin(-v108);
                float v74 = cos(v108);

                float v79 = 0.0;

                mat3x3 mat2;

                mat2.m00 = (1.0 - v74) * v82 * v82 + v74;
                mat2.m01 = (1.0 - v74) * v82 * v79 - v7 * v83;
                mat2.m02 = (1.0 - v74) * v83 * v82 + v7 * v79;

                mat2.m10 = (1.0 - v74) * v82 * v79 + v7 * v83;
                mat2.m11 = (1.0 - v74) * v79 * v79 + v74;
                mat2.m12 = (1.0 - v74) * v79 * v83 - v7 * v82;

                mat2.m20 = (1.0 - v74) * v83 * v82 - v7 * v79;
                mat2.m21 = (1.0 - v74) * v79 * v83 + v7 * v82;
                mat2.m22 = (1.0 - v74) * v83 + v74;

                mat3x3 dst;
                mat_mult(&bact->field_651, &mat2, &dst);

                bact->field_651 = dst;
            }
        }

        if ( arg->field_one & 2 )
        {
            float v18 = fabs(bact->field_611) * v94 * 0.08;

            float v76 = cos(v18);
            float v77 = sin(v18);

            mat3x3 mat1;
            mat1.m00 = v76;
            mat1.m01 = v77;
            mat1.m02 = 0;

            mat1.m10 = -v77;
            mat1.m11 = v76;
            mat1.m12 = 0;

            mat1.m20 = 0;
            mat1.m21 = 0;
            mat1.m22 = 1.0;

            mat3x3 v63;
            mat_mult(&mat1, &bact->field_651, &v63);

            bact->field_651 = v63;
        }

        if ( !(bact->field_3D6 & 0x200) )
        {
            if ( arg->field_one & 1 )
                bact->airconst = 0;
            else
                bact->airconst = 500.0;

            for (int i = 0; i < 3; i++)
            {

                bact_arg74 v66;

                v66.field_0 = v94;
                v66.flag = 1;

                call_method(obj, 74, &v66);

                int v20 = 0;

                if ( bact->field_B34 & 8 )
                {
                    int arg87 = arg->field_two;
                    if ( call_method(obj, 87, &arg87) )
                    {
                        if ( bact->field_24 == 2 || bact->field_24 == 8 )
                        {
                            ypabact_func86__sub1(bact);
                            return 0;
                        }

                        return 0;
                    }
                }

                if ( bact->field_B34 & 1 )
                {
                    ypaworld_arg137 arg137;
                    arg137.pos.sx = bact->field_605.sx * bact->field_611 * v94 * 6.0 + bact->field_621.sx;
                    arg137.pos.sy = bact->field_605.sy * bact->field_611 * v94 * 6.0 + bact->field_621.sy;
                    arg137.pos.sz = bact->field_605.sz * bact->field_611 * v94 * 6.0 + bact->field_621.sz;
                    arg137.pos2.sx = bact->field_605.sx;
                    arg137.pos2.sy = bact->field_605.sy;
                    arg137.pos2.sz = bact->field_605.sz;
                    arg137.radius = v84;
                    arg137.collisions = v58;
                    arg137.field_30 = 0;
                    arg137.coll_max = 10;

                    call_method(bact->wrld, 137, &arg137);

                    if ( arg137.coll_count )
                    {
                        int v24 = 0;
                        v85 = 1;

                        float v98 = 0.0;
                        float v104 = 0.0;
                        float v97 = 0.0;

                        for (int j = arg137.coll_count - 1; j >= 0; j--)
                        {
                            yw_137col *v25 = &arg137.collisions[ j ];

                            v98 += v25->pos2.sx;
                            v104 += v25->pos2.sy;
                            v97 += v25->pos2.sz;

                            if ( v104 > 0.6 )
                                v24 = 1;
                        }

                        float a1a = sqrt( POW2(v98) + POW2(v104) + POW2(v97) );

                        bact_arg88 arg88;
                        xyz a2a;

                        if ( a1a != 0.0 )
                        {
                            float v32 = 1.0 / a1a;

                            arg88.pos1.sx = v98 * v32;
                            arg88.pos1.sy = v104 * v32;
                            arg88.pos1.sz = v97 * v32;

                            a2a = arg88.pos1;
                        }
                        else
                        {
                            a2a = bact->field_605;
                            arg88.pos1 = bact->field_605;
                        }

                        if ( arg->field_one & 1 )
                        {
                            bact->energy -= fabs(bact->field_611) * 10.0;

                            if ( bact->energy <= 0 || (bact->current_vp.base == bact->vp_dead.base && bact->field_3D5 == 2) )
                            {
                                bact_arg119 arg78;
                                arg78.field_4 = 2048;
                                arg78.field_8 = 0;
                                arg78.field_0 = 0;

                                call_method(obj, 78, &arg78);
                            }

                            if ( bact->field_B34 & 2 )
                            {
                                if ( fabs(bact->field_611) > 7.0 )
                                    sub_423F74(&bact->field_5A, 5);

                                yw_arg180 arg180_1;

                                arg180_1.effects_type = 5;
                                arg180_1.field_4 = 1.0;
                                arg180_1.field_8 = v98 * 10.0 + bact->field_621.sx;
                                arg180_1.field_C = v97 * 10.0 + bact->field_621.sz;

                                call_method(bact->wrld, 180, &arg180_1);
                            }

                            if ( v104 >= 0.6 && v24 )
                            {
                                bact->field_621.sy = bact->field_62D.sy;

                                bact->field_3D6 |= 0x200;

                                bact->field_611 *= sqrt( POW2(bact->field_605.sx) + POW2(bact->field_605.sz) );

                                sub_48AB14(bact, &a2a);

                                bact->field_9B6 = 0;
                            }
                            else
                            {
                                arg88.pos2.sy = 0.7;
                                arg88.pos2.sx = 0.7;
                                arg88.pos2.sz = v94;

                                call_method(obj, 88, &arg88);

                                bact->field_9B6++;

                                v20 = 1;

                                if ( bact->field_9B6 > 50 )
                                {
                                    bact->energy = -10000;
                                    bact->field_3D6 |= 0x200;
                                }
                            }
                        }
                        else if ( v104 < 0.6 )
                        {
                            arg88.pos2.sy = 2.0;
                            arg88.pos2.sx = 0.7;
                            arg88.pos2.sz = v94;

                            call_method(obj, 88, &arg88);

                            v20 = 1;
                        }
                        else
                        {
                            bact->field_621.sy = bact->field_62D.sy;
                            bact->field_611 = 0;
                            bact->field_9B6 = 0;
                            bact->field_3D6 |= 0x200;
                        }
                    }
                }

                if ( !v85 )
                {
                    ypaworld_arg136 arg136;
                    arg136.pos_x = bact->field_62D.sx;
                    arg136.pos_y = bact->field_62D.sy;
                    arg136.pos_z = bact->field_62D.sz;
                    arg136.field_14 = bact->field_621.sx - bact->field_62D.sx;
                    arg136.field_18 = bact->field_621.sy - bact->field_62D.sy + v90;
                    arg136.field_1C = bact->field_621.sz - bact->field_62D.sz;
                    arg136.field_40 = 0;

                    call_method(bact->wrld, 136, &arg136);

                    if ( arg136.field_20 )
                    {
                        bact_arg88 arg88;

                        arg88.pos1.sx = arg136.field_3C->triangles[arg136.field_38].field_0;
                        arg88.pos1.sy = arg136.field_3C->triangles[arg136.field_38].field_4;
                        arg88.pos1.sz = arg136.field_3C->triangles[arg136.field_38].field_8;

                        xyz a2a = arg88.pos1;

                        if ( arg->field_one & 1 )
                        {
                            bact->energy -= fabs(bact->field_611) * 10.0;

                            if ( bact->energy <= 0 || (bact->current_vp.base == bact->vp_dead.base && bact->field_3D5 == 2) )
                            {
                                bact_arg119 arg78;
                                arg78.field_4 = 2048;
                                arg78.field_8 = 0;
                                arg78.field_0 = 0;

                                call_method(bact->self, 78, &arg78);
                            }

                            if ( bact->field_B34 & 2 )
                            {
                                if ( fabs(bact->field_611) > 7.0 )
                                    sub_423F74(&bact->field_5A, 5);

                                yw_arg180 arg180;

                                arg180.effects_type = 5;
                                arg180.field_4 = 1.0;
                                arg180.field_8 = a2a.sx * 10.0 + bact->field_621.sx;
                                arg180.field_C = a2a.sz * 10.0 + bact->field_621.sz;

                                call_method(bact->wrld, 180, &arg180);
                            }

                            if ( arg136.field_3C->triangles[arg136.field_38].field_4 < 0.6 )
                            {
                                arg88.pos2.sy = 0.7;
                                arg88.pos2.sx = 0.7;
                                arg88.pos2.sz = v94;

                                call_method(obj, 88, &arg88);

                                bact->field_9B6++;

                                v20 = 1;

                                if ( bact->field_9B6 > 50 )
                                {
                                    bact->energy = -10000;
                                    bact->field_3D6 |= 0x200;
                                }
                            }
                            else
                            {
                                bact->field_621.sx = arg136.field_2C;
                                bact->field_621.sy = arg136.field_30 - v90;
                                bact->field_621.sz = arg136.field_34;


                                bact->field_3D6 |= 0x200;

                                bact->field_611 *= sqrt( POW2(bact->field_605.sx) + POW2(bact->field_605.sz) );

                                sub_48AB14(bact, &a2a);

                                bact->field_9B6 = 0;
                            }
                        }
                        else if ( arg136.field_3C->triangles[arg136.field_38].field_4 < 0.6 )
                        {
                            arg88.pos2.sy = 2.0;
                            arg88.pos2.sx = 0.7;
                            arg88.pos2.sz = v94;

                            call_method(obj, 88, &arg88);

                            v20 = 1;
                        }
                        else
                        {
                            bact->field_621.sy = arg136.field_30 - v90;

                            bact->field_611 = 0;
                            bact->field_9B6 = 0;
                            bact->field_3D6 |= 0x200;
                        }
                    }
                }

                if ( !v20 ) // Alternative exit from loop
                    break;
            }

        }
        if ( bact->field_3D6 & 0x200 )
            return 1;
    }
    return 0;
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

        if ( bnode->self != obj && bnode->field_24 != 4 && (!call_method(bnode->self, 100, 0) || v53) )
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

void ypabact_func88(NC_STACK_ypabact *obj, class_stru *zis, bact_arg88 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( !(bact->field_3D6 & 0x200) )
    {
        if ( bact->field_605.sx * arg->pos1.sx + bact->field_605.sy * arg->pos1.sy + bact->field_605.sz * arg->pos1.sz >= 0.0 )
        {
            if ( bact->field_611 != 0.0 )
            {
                bact->field_621 = bact->field_62D;

                float v4 = (arg->pos1.sx * bact->field_605.sx + arg->pos1.sy * bact->field_605.sy + arg->pos1.sz * bact->field_605.sz) * 2.0;

                bact->field_605.sx -= arg->pos1.sx * v4;
                bact->field_605.sy -= arg->pos1.sy * v4;
                bact->field_605.sz -= arg->pos1.sz * v4;

                bact->field_611 *= 25.0 / (fabs(bact->field_611) + 10.0);
            }
        }
    }
}

void ypabact_func89(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func89");
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

void ypabact_func91(NC_STACK_ypabact *obj, class_stru *zis, xyz *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    yw_130arg arg130;
    arg130.pos_x = arg->sx;
    arg130.pos_z = arg->sz;

    call_method(bact->wrld, 130, &arg130);

    float v15 = arg130.sec_x * 1200.0 + 600.0;
    float v13 = -(arg130.sec_z * 1200.0 + 600.0);

    arg->sx = v15;
    arg->sz = v13;

    if ( arg130.pcell->field_2E != 1 )
    {
        int v7 = 0;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                if ( arg130.pcell->buildings_health[j][i] > v7 )
                {
                    arg->sz = 300.0 * (-1 + i) + v13;
                    arg->sx = 300.0 * (-1 + j) + v15;

                    v7 = arg130.pcell->buildings_health[j][i];
                }
            }
        }
    }
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
    dprintf("MAKE ME %s\n","ypabact_func93");
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
    dprintf("MAKE ME %s\n","ypabact_func95");
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
    bact->field_995 = 0.0;
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

void ypabact_func97(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact * bact = &obj->stack__ypabact;

    bact->field_601 = bact->mass * 9.77665;

    float v53 = arg->field_4 * 0.001;

    float v55 = 0.0 * bact->field_651.m11 - bact->field_651.m12;
    float v57 = 0.0 * bact->field_651.m12 - 0.0 * bact->field_651.m10;
    float v56 = bact->field_651.m10 - 0.0 * bact->field_651.m11;

    float v63 = sqrt( POW2(v55) + POW2(v56) + POW2(v57) );

    if ( v63 > 0.001 )
    {
        float v10 = 1.0 / v63;

        float v65 = bact->field_651.m10 * 0.0 + bact->field_651.m11 * 1.0 + bact->field_651.m12 * 0.0;

        v55 *= v10;
        v57 *= v10;
        v56 *= v10;

        if ( v65 > 1.0 )
            v65 = 1.0;

        if ( v65 < -1.0 )
            v65 = -1.0;

        float v62 = acos(v65);
        float v11 = bact->maxrot * v53;

        if ( v11 < v62 )
            v62 = (v62 * 1.5) * bact->maxrot * v53;

        if ( fabs(v62) <= 0.0015 )
        {
            bact->field_651.m10 = 0;
            bact->field_651.m11 = 1.0;
            bact->field_651.m12 = 0;

            float v27 = 1.0 / sqrt( POW2(bact->field_651.m00) + POW2(bact->field_651.m02) );

            bact->field_651.m00 *= v27;
            bact->field_651.m01 = 0;
            bact->field_651.m02 *= v27;

            float v32 = 1.0 / sqrt( POW2(bact->field_651.m20) + POW2(bact->field_651.m22) );

            bact->field_651.m20 *= v32;
            bact->field_651.m21 = 0;
            bact->field_651.m22 *= v32;

            if ( fabs(bact->field_611) < 0.1 )
            {
                bact->field_605.sx = 0;
                bact->field_605.sy = 1.0;
                bact->field_605.sz = 0;

                bact->field_611 = 0;
            }
        }
        else
        {
            float v12 = sin(-v62);
            float v44 = cos(v62);

            mat3x3 mat2;

            mat2.m00 = (1.0 - v44) * v55 * v55 + v44;
            mat2.m01 = (1.0 - v44) * v55 * v57 - v12 * v56;
            mat2.m02 = (1.0 - v44) * v56 * v55 + v12 * v57;

            mat2.m10 = (1.0 - v44) * v55 * v57 + v12 * v56;
            mat2.m11 = (1.0 - v44) * v57 * v57 + v44;
            mat2.m12 = (1.0 - v44) * v57 * v56 - v12 * v55;

            mat2.m20 = (1.0 - v44) * v56 * v55 - v12 * v57;
            mat2.m21 = (1.0 - v44) * v57 * v56 + v12 * v55;
            mat2.m22 = (1.0 - v44) * v56 + v44;

            mat3x3 v41;
            mat_mult(&bact->field_651, &mat2, &v41);

            bact->field_651 = v41;
        }
    }

    bact->field_611 *= 0.8;
}

void ypabact_func98(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func98");
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

void ypabact_func99(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    bact->field_931 -= arg->field_4;

    float v30 = arg->field_4 / 1000.0;

    if ( bact->field_931 > 0 )
    {
        bact->field_3D6 |= 0x100000;

        if ( bact->field_931 < 0 )
        {
            bact->field_67D.sx = 1.0;
            bact->field_67D.sy = 1.0;
            bact->field_67D.sz = 1.0;
        }
        else
        {
            float v21 = 0.9 / ((float)bact->field_931 / 1000.0 + 0.9) + 0.1;
            bact->field_67D.sx = v21;
            bact->field_67D.sy = v21;
            bact->field_67D.sz = v21;
        }

        sub_48A0E0(bact, 2.5 / bact->field_67D.sx * v30);
    }
    else
    {
        bact_arg119 v25;
        v25.field_0 = 1;
        v25.field_4 = 0;
        v25.field_8 = 0;

        call_method(obj, 78, &v25);

        bact->field_3D6 &= 0xFFEFFFFF;

        bact_arg80 v24;

        v24.pos = bact->field_621;
        v24.field_C = 0;

        call_method(obj, 80, &v24);

        NC_STACK_ypabact *a4;
        call_vtbl(bact->wrld, 3, 0x80002010, &a4, 0);

        if ( bact->field_32 == a4 )
        {

            if ( bact->field_32 == bact->parent_bacto )
            {
                robo_arg134 v23;
                v23.unit = bact;
                v23.field_4 = 14;
                v23.field_8 = 0;
                v23.field_C = 0;
                v23.field_10 = 0;
                v23.field_14 = 26;

                call_method(bact->field_32, 134, &v23);
            }
        }

        if ( bact->field_32 )
        {
            if ( bact->field_24 != 9 )
            {
                __NC_STACK_ypabact *v27;

                call_vtbl(bact->field_32, 3, 0x80001003, &v27, 0);

                bact->field_605.sx = v24.pos.sx - v27->field_621.sx;
                bact->field_605.sy = v24.pos.sy - v27->field_621.sy;
                bact->field_605.sz = v24.pos.sz - v27->field_621.sz;

                float v31 = sqrt( POW2(bact->field_605.sx) + POW2(bact->field_605.sy) + POW2(bact->field_605.sz) );

                if ( v31 > 0.001 )
                {
                    float v17 = 1.0 / v31;

                    bact->field_605.sx *= v17;
                    bact->field_605.sy *= v17;
                    bact->field_605.sz *= v17;
                }

                bact->field_611 = 20.0;
            }
        }
    }
}

size_t ypabact_func100(NC_STACK_ypabact *obj, class_stru *zis, void *)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    return (bact->current_vp.base == bact->vp_dead.base || bact->current_vp.base == bact->vp_genesis.base || bact->current_vp.base == bact->vp_megadeth.base) && bact->field_3D5 == 2;
}

size_t ypabact_func101(NC_STACK_ypabact *obj, class_stru *zis, bact_arg101 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    xyz tmp;

    if ( arg->unkn == 2 )
    {
        tmp.sx = arg->pos.sx - bact->field_621.sx;
        tmp.sy = arg->pos.sy - bact->field_621.sy;
        tmp.sz = arg->pos.sz - bact->field_621.sz;
    }
    else
    {
        tmp.sy = bact->height;
        tmp.sz = arg->pos.sz - bact->field_621.sz;
        tmp.sx = arg->pos.sx - bact->field_621.sx;
    }

    float v33 = sqrt(  POW2(tmp.sy) + POW2(tmp.sx) + POW2(tmp.sz)  );

    if ( v33 == 0.0 )
        return 0;

    float v6 = 1.0 / v33;

    float v25 = tmp.sx * v6;
    float v27 = tmp.sy * v6;
    float v29 = tmp.sz * v6;

    WeapProto *a4;
    call_vtbl(bact->wrld, 3, 0x80002012, &a4, 0);

    WeapProto *v8 = NULL;

    int v36;

    if ( bact->weapon != -1 )
    {
        v8 = &a4[ bact->weapon ];


        if ( v8->model_id & 1 )
            v36 = v8->model_id & 0xFE;
        else
            v8 = NULL;
    }

    if ( !v8 )
    {
        if ( bact->mgun == -1 )
            return 0;

        v36 = 2;
    }

    if ( arg->unkn == 2 )
    {
        float v32;

        if ( v8 )
        {

            float v38 = arg->radius * 0.8 + v8->radius;

            if ( v38 >= 40.0 )
            {
                v32 = v38;
            }
            else
            {
                v32 = 3.0625;
            }
        }
        else
        {
            float v41 = arg->radius * 0.8;

            if ( v41 >= 40.0 )
                v32 = v41;
            else
                v32 = 40.0;
        }

        if ( v36 )
        {
            if ( v36 == 16 )
            {
                if ( v33 < 1200.0 && v29 * bact->field_651.m22 + v25 * bact->field_651.m20 > 0.93 )
                    return 1;
            }
            else
            {
                float v19 = v27 * bact->field_651.m22 - v29 * bact->field_651.m21;
                float v18 = v25 * bact->field_651.m21 - v27 * bact->field_651.m20;
                float v21 = v29 * bact->field_651.m20 - v25 * bact->field_651.m22;

                float v31 = sqrt( POW2(v18) + POW2(v19) + POW2(v21) );

                if ( v33 < 1200.0 && (v27 * bact->field_651.m21 + v25 * bact->field_651.m20 + v29 * bact->field_651.m22 > 0.0) && v32 / v33 > v31 )
                    return 1;
            }
        }
        else
        {
            float v13 = arg->pos.sz - bact->field_621.sz;
            float v14 = arg->pos.sx - bact->field_621.sx;

            if ( sqrt( POW2(v13) + POW2(v14) ) < v32 && arg->pos.sy > bact->field_621.sy )
                return 1;
        }
    }
    else if ( v8 )
    {
        if ( v36 )
        {
            if ( v36 == 16 )
            {
                if ( v33 < 1200.0 && v29 * bact->field_651.m22 + v25 * bact->field_651.m20 > 0.91 )
                    return 1;
            }
            else if ( v33 < 1200.0 && v27 * bact->field_651.m21 + v25 * bact->field_651.m20 + v29 * bact->field_651.m22 > 0.91 )
            {
                return 1;
            }
        }
        else
        {
            float v22 = arg->pos.sz - bact->field_621.sz;
            float v23 = arg->pos.sx - bact->field_621.sx;

            if ( sqrt(POW2(v22) + POW2(v23)) < v8->radius )
                return 1;
        }
    }
    return 0;
}

void ypabact_func102(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->field_24 != 4 )
    {
        if ( bact->field_3D5 != 2 && bact->field_3D5 != 4 )
        {
            int v8 = 0;

            if (!bact->parent_bact)
                v8 = 1;
            else
            {
                if (bact->radar >= bact->parent_bact->radar)
                    v8 = 1;
                else if (bact->field_c != bact->parent_bact->field_c)
                    v8 = 1;
                else if (bact->field_10 != bact->parent_bact->field_10)
                    v8 = 1;
            }

            if ( v8 )
            {
                if ( bact->owner < 8 )
                {
                    cellArea *v19 = bact->p_cell_area;

                    for (int i = -bact->radar; i <= bact->radar; i++)
                    {
                        int yy = bact->field_10 + i;

                        int v20 = bact->field_20 * i;

                        if ( bact->radar == 1 )
                        {
                            if ( yy > 0 && yy < bact->field_22 - 1 )
                            {
                                if ( bact->field_c - 1 > 0 )
                                    v19[v20 - 1].field_39 |= 1 << bact->owner;

                                v19[v20].field_39 |= 1 << bact->owner;

                                if ( bact->field_c + 1 < bact->field_20 - 1 )
                                    v19[v20 + 1].field_39 |= 1 << bact->owner;
                            }
                        }
                        else
                        {
                            float vtmp = POW2((float)bact->radar) - POW2((float)i);

                            if (vtmp < 0.0)
                                vtmp = 0.0;

                            int tmp = dround( sqrt(vtmp) );

                            for (int j = -tmp; j <= tmp; j++)
                            {
                                int xx = bact->field_c + j;

                                if ( xx > 0 && xx < bact->field_20 - 1 && yy > 0 && yy < bact->field_22 - 1 )
                                    v19[v20 + j].field_39 |= 1 << bact->owner;
                            }
                        }
                    }
                }
            }

            bact_node *v15 = (bact_node *)bact->list2.head;
            while( v15->next )
            {
                call_method(v15->bacto, 102, 0);

                v15 = (bact_node *)v15->next;
            }
        }
    }
}

void ypabact_func103(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func103");
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

size_t ypabact_func105(NC_STACK_ypabact *obj, class_stru *zis, bact_arg105 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    int a5 = 0;

    if ( bact->wrld->stack__ypaworld.field_757E )
        a5 = 1;

    if ( bact->mgun == -1 )
        return 0;

    __NC_STACK_ypabact *v108 = NULL;
    float v123 = 0.0;
    float v121 = 0.0;
    float v66, v67, v68;

    yw_130arg arg130;
    arg130.pos_x = bact->field_621.sx;
    arg130.pos_z = bact->field_621.sz;

    float v74 = bact->field_621.sx + arg->field_0.sx * 1000.0;
    //float v75 = arg->field_0.sy * 1000.0 + bact->field_621.sy;
    float v76 = bact->field_621.sz + arg->field_0.sz * 1000.0;

    call_method(bact->wrld, 130, &arg130);

    cellArea *v70[3];
    v70[0] = arg130.pcell;

    arg130.pos_x = v74;
    arg130.pos_z = v76;
    call_method(bact->wrld, 130, &arg130);

    v70[2] = arg130.pcell;

    if ( arg130.pcell == v70[0] )
    {
        v70[1] = v70[0];
    }
    else
    {
        arg130.pos_x = (v74 - bact->field_621.sx) * 0.5 + bact->field_621.sx;
        arg130.pos_z = (v76 - bact->field_621.sz) * 0.5 + bact->field_621.sz;

        call_method(bact->wrld, 130, &arg130);

        v70[1] = arg130.pcell;
    }

    int v107 = 0;
    if ( bact->field_24 == 9 )
    {
        int a4;
        call_vtbl(obj, 3, 0x80002006, &a4, 0);

        if ( a4 )
            v107 = 1;
    }
    else
    {
        bact->energy -= bact->gun_power * arg->field_C / 300.0;
    }

    for(int i = 0; i < 3; i++)
    {
        if ( i <= 0 || v70[ i ] != v70[ i - 1 ] )
        {
            __NC_STACK_ypabact *v21 = (__NC_STACK_ypabact *)v70[ i ]->field_3C.head;

            while ( v21->next )
            {
                if ( v21 != bact && v21->field_24 != 4 && v21->field_3D5 != 2 )
                {
                    int v89 = 0;
                    if (v21->field_24 == 9)
                        call_vtbl(v21->self, 3, 0x80002006, &v89, 0);

                    if ( v21->field_24 != 9 || v21->shield > 100 || !v89 )
                    {
                        if ( (bact->field_B34 & 2 || v21->owner != bact->owner) && (!v107 || v21->self != bact->field_32) )
                        {

                            rbcolls *v93;
                            call_vtbl(v21->self, 3, 0x8000100E, &v93, 0);

                            int v109;
                            if ( v93 )
                                v109 = v93->robo_coll_num;
                            else
                                v109 = 1;

                            int v22 = 0;

                            for (int j = v109 - 1; j >= 0; j-- )
                            {
                                float v77, v78, v79, v27;

                                if ( v93 )
                                {
                                    v77 = v21->field_621.sx +
                                          v21->field_651.m00 * v93->roboColls[j].robo_coll_x +
                                          v21->field_651.m10 * v93->roboColls[j].robo_coll_y +
                                          v21->field_651.m20 * v93->roboColls[j].robo_coll_z;

                                    v78 = v21->field_621.sy +
                                          v21->field_651.m01 * v93->roboColls[j].robo_coll_x +
                                          v21->field_651.m11 * v93->roboColls[j].robo_coll_y +
                                          v21->field_651.m21 * v93->roboColls[j].robo_coll_z;

                                    v79 = v21->field_621.sz +
                                          v21->field_651.m02 * v93->roboColls[j].robo_coll_x +
                                          v21->field_651.m12 * v93->roboColls[j].robo_coll_y +
                                          v21->field_651.m22 * v93->roboColls[j].robo_coll_z;

                                    v27 = v93->roboColls[j].robo_coll_radius;
                                }
                                else
                                {
                                    v77 = v21->field_621.sx;
                                    v78 = v21->field_621.sy;
                                    v79 = v21->field_621.sz;

                                    v27 = v21->radius;
                                }

                                if ( !v93 || v27 >= 0.01 )
                                {
                                    v121 = v27;

                                    float v63 = v77 - bact->field_62D.sx;
                                    float v64 = v78 - bact->field_62D.sy;
                                    float v65 = v79 - bact->field_62D.sz;

                                    if ( v63 * bact->field_651.m20 + v64 * bact->field_651.m21 + v65 * bact->field_651.m22 >= 0.3 )
                                    {
                                        float v36 = arg->field_0.sx * v64 - v63 * arg->field_0.sy;
                                        float v33 = arg->field_0.sy * v65 - v64 * arg->field_0.sz;
                                        float v34 = arg->field_0.sz * v63 - v65 * arg->field_0.sx;

                                        float v111 = sqrt(POW2(v63) + POW2(v64) + POW2(v65));
                                        float v110 = sqrt(POW2(v36) + POW2(v33) + POW2(v34));

                                        float v37 = v27 + bact->gun_radius;

                                        if ( v37 > v110 )
                                        {
                                            if ( sqrt( POW2(v110) + 1000000.0 ) > v111 )
                                            {
                                                if ( !v22 )
                                                {
                                                    int v88;
                                                    call_vtbl(v21->self, 3, 0x80001005, &v88, 0);

                                                    int energ;

                                                    if ( v88 || v21->field_3D6 & 0x800000 )
                                                    {
                                                        float v39 = (bact->gun_power * arg->field_C) * (100.0 - (float)v21->shield);
                                                        energ = (v39 * 0.004);
                                                    }
                                                    else
                                                    {

                                                        float v41 = (bact->gun_power * arg->field_C) * (100.0 - (float)v21->shield);
                                                        energ = v41 / 100;
                                                    }

                                                    bact_arg84 v86;
                                                    v86.unit = bact;
                                                    v86.energy = -energ;

                                                    if ( energ )
                                                        call_method(v21->self, 84, &v86);
                                                }

                                                v22 = 1;

                                                if ( !v108 || v123 > v111 )
                                                {
                                                    v108 = v21;
                                                    v123 = v111;

                                                    v66 = v21->field_621.sx;
                                                    v67 = v21->field_621.sy;
                                                    v68 = v21->field_621.sz;
                                                }
                                            }
                                        }
                                    }

                                }

                            }


                        }
                    }
                }

                v21 = (__NC_STACK_ypabact *)v21->next;
            }
        }
    }

    int v88;
    call_vtbl(obj, 3, 0x80001005, &v88, 0);

    if ( (v88 || call_method(bact->wrld, 145, bact)) && !a5 )
    {
        WeapProto *v90;
        call_vtbl(bact->wrld, 3, 0x80002012, &v90, 0);

        int v45;

        if ( v88 )
        {
            int v43 = v90[bact->mgun].shot_time_user;
            float v42 = arg->field_C * 1000.0;

            if ( v43 <= v42 )
                v45 = v42;
            else
                v45 = v43;
        }
        else
        {
            int v47 = v90[bact->mgun].shot_time;
            float v46 = arg->field_C * 1000.0;

            if ( v47 <= v46 )
                v45 = v46;
            else
                v45 = v47;
        }

        if ( arg->field_10 - bact->field_9A1 > v45 )
        {
            bact->field_9A1 = arg->field_10;

            int v55 = 0;
            int v96 = 0;

            ypaworld_arg136 v59;

            float v80, v81, v82;

            if ( v108 )
            {
                float v50 = v121 * 0.7;
                float v51 = 1.0 / v123;
                v55 = 1;
                v96 = 0;

                v80 = v66 - (v66 - bact->field_621.sx) * v50 * v51;
                v81 = v67 - (v67 - bact->field_621.sy) * v50 * v51;
                v82 = v68 - (v68 - bact->field_621.sz) * v50 * v51;
            }
            else
            {
                v59.pos_x = bact->field_621.sx;
                v59.pos_y = bact->field_621.sy;
                v59.pos_z = bact->field_621.sz;
                v59.field_14 = arg->field_0.sx * 1000.0;
                v59.field_18 = arg->field_0.sy * 1000.0;
                v59.field_1C = arg->field_0.sz * 1000.0;
                v59.field_40 = 0;

                call_method(bact->wrld, 149, &v59);

                if ( v59.field_20 )
                {
                    v80 = v59.field_2C;
                    v81 = v59.field_30;
                    v82 = v59.field_34;

                    v96 = 1;
                    v55 = 1;
                }
                else
                {
                    v55 = 0;
                }
            }

            if ( v55 )
            {
                ypaworld_arg146 arg147;
                arg147.pos.sx = v80;
                arg147.pos.sy = v81;
                arg147.pos.sz = v82;
                arg147.vehicle_id = bact->mgun;

                NC_STACK_ypabact *v57 = (NC_STACK_ypabact *)call_method(bact->wrld, 147, &arg147);

                if ( v57 )
                {
                    __NC_STACK_ypabact *v103;
                    call_vtbl(v57, 3, 0x80001003, &v103, 0);

                    v103->owner = bact->owner;

                    if ( v103->parent_bacto )
                    {
                        Remove(&v103->list_node);
                        v103->parent_bacto = NULL;
                    }

                    bact_node *a2a;
                    call_vtbl(v57, 3, 0x80002003, &a2a, 0);

                    AddTail(&bact->list3, a2a);

                    bact_arg119 v69;
                    v69.field_0 = 2;
                    v69.field_4 = 0;
                    v69.field_8 = 0;

                    call_method(v57, 119, &v69);

                    if ( v96 )
                    {
                        v69.field_4 = 2048;
                        v69.field_0 = 0;
                        v69.field_8 = 0;
                        call_method(v57, 119, &v69);

                        xyz v61;
                        v61.sx = v59.field_3C->triangles[ v59.field_38 ].field_0;
                        v61.sy = v59.field_3C->triangles[ v59.field_38 ].field_4;
                        v61.sz = v59.field_3C->triangles[ v59.field_38 ].field_8;

                        call_method(v57, 131, &v61);
                    }
                }
            }
        }
    }
    return 1;
}


void sub_4843BC(__NC_STACK_ypabact *bact1, __NC_STACK_ypabact *bact2, int a3)
{
    bact_hudi hudi;

    float v23;
    float v24;

    if ( bact2 )
    {
        float v17 = bact2->field_621.sx - bact1->field_621.sx;
        float v18 = bact2->field_621.sy - bact1->field_621.sy;
        float v19 = bact2->field_621.sz - bact1->field_621.sz;

        float v20 = bact1->field_651.m00 * v17 + bact1->field_651.m01 * v18 + bact1->field_651.m02 * v19;
        float v21 = bact1->field_651.m10 * v17 + bact1->field_651.m11 * v18 + bact1->field_651.m12 * v19;
        float v22 = bact1->field_651.m20 * v17 + bact1->field_651.m21 * v18 + bact1->field_651.m22 * v19;

        if ( v22 != 0.0 )
        {
            float v12 = 1.0 / v22;
            v23 = v20 * v12;
            v24 = v21 * v12;
        }
        else
        {
            v24 = 0.0;
            v23 = 0.0;
        }

        hudi.field_18 = bact2;
    }
    else
    {
        v23 = -bact1->field_995;
        v24 = -bact1->gun_angle2;

        hudi.field_18 = NULL;
    }

    if ( bact1->mgun == -1 )
    {
        hudi.field_0 = 0;
    }
    else
    {
        hudi.field_0 = 1;
        hudi.field_8 = -bact1->field_995;
        hudi.field_C = -bact1->gun_angle2;
    }

    if ( bact1->weapon == -1 || a3 )
    {
        hudi.field_4 = 0;
    }
    else
    {
        if ( bact1->field_96E & 4 )
        {
            hudi.field_10 = v23;
            hudi.field_4 = 4;
            hudi.field_14 = v24;
        }
        else
        {
            if ( (bact1->field_96E & 4) || !(bact1->field_96E & 2) )
                hudi.field_4 = 2;
            else
                hudi.field_4 = 3;

            hudi.field_10 = -bact1->field_995;
            hudi.field_14 = -bact1->gun_angle2;
        }
    }

    call_method(bact1->wrld, 153, &hudi);
}

size_t ypabact_func106(NC_STACK_ypabact *obj, class_stru *zis, bact_arg106 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    WeapProto *weaps;
    call_vtbl(bact->wrld, 3, 0x80002012, &weaps, 0);

    __NC_STACK_ypabact *targeto = 0;
    float v56;

    float v55;

    if ( bact->weapon == -1 )
        v55 = 0.0;
    else
        v55 = weaps[bact->weapon].radius;

    int a3a = !(bact->field_96E & 2) && !(bact->field_96E & 0x10);

    if ( bact->weapon != -1 && !a3a )
    {
        yw_130arg arg130;
        arg130.pos_x = bact->field_621.sx;
        arg130.pos_z = bact->field_621.sz;

        call_method(bact->wrld, 130, &arg130);


        float v38 = bact->field_651.m20 * 1200.0 + bact->field_621.sx;
        float v40 = bact->field_651.m22 * 1200.0 + bact->field_621.sz;

        cellArea *v41[3];

        v41[0] = arg130.pcell;

        arg130.pos_x = v38;
        arg130.pos_z = v40;

        call_method(bact->wrld, 130, &arg130);

        v41[2] = arg130.pcell;

        if ( arg130.pcell == v41[0] )
        {
            v41[1] = v41[0];
        }
        else
        {
            arg130.pos_x = (v38 - bact->field_621.sx) * 0.5 + bact->field_621.sx;
            arg130.pos_z = (v40 - bact->field_621.sz) * 0.5 + bact->field_621.sz;

            call_method(bact->wrld, 130, &arg130);

            v41[1] = arg130.pcell;
        }

        for (int i = 0; i < 3; i++)
        {
            if ( i <= 0 || v41[i] != v41[i - 1] )
            {
                if ( v41[i] )
                {
                    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v41[i]->field_3C.head;

                    while (bct->next)
                    {
                        if ( bct != bact )
                        {

                            if ( bct->field_24 != 4 && bct->field_3D5 != 2 )
                            {
                                int v53 = 0;
                                if (bct->field_24 == 9)
                                    call_vtbl(bct->self, 3, 0x80002006, &v53, 0);

                                if ( !v53 )
                                {
                                    if ( arg->field_0 & 2 || bct->owner != bact->owner )
                                    {
                                        if ( arg->field_0 & 1 || bct->owner == bact->owner || !bct->owner )
                                        {
                                            if ( arg->field_0 & 4 || bct->owner )
                                            {
                                                float v46 = bct->field_621.sy - bact->field_62D.sy;
                                                float v45 = bct->field_621.sx - bact->field_62D.sx;
                                                float v47 = bct->field_621.sz - bact->field_62D.sz;

                                                if ( v45 * bact->field_651.m20 + v46 * bact->field_651.m21 + v47 * bact->field_651.m22 >= 0.0 )
                                                {
                                                    float v57 = sqrt(POW2(v45) + POW2(v46) + POW2(v47));

                                                    float v50 = arg->field_4.sx * v46 - arg->field_4.sy * v45;
                                                    float v48 = arg->field_4.sy * v47 - arg->field_4.sz * v46;
                                                    float v49 = arg->field_4.sz * v45 - arg->field_4.sx * v47;

                                                    float v59 = v57 * 1000.0 * 0.0005 + 20.0;
                                                    float v60 = sqrt(POW2(v49) + POW2(v48) + POW2(v50));

                                                    if ( ((v60 < v59 && (bact->field_96E & 4)) || (bct->radius + v55 > v60 && !(bact->field_96E & 4)) )
                                                            && v57 < 2000.0
                                                            && (v56 > v60 || !targeto) )
                                                    {
                                                        targeto = bct;
                                                        v56 = v60;
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
        }
    }

    if ( targeto )
    {
        sub_4843BC(bact, targeto, a3a);

        bact_arg67 arg67;
        arg67.field_0 = 2;
        arg67.field_4 = 1;
        arg67.field_8 = targeto;

        call_method(obj, 67, &arg67);

        arg->ret_bact = targeto;
        return 1;
    }

    sub_4843BC(bact, NULL, a3a);
    arg->ret_bact = NULL;

    return 0;
}

void ypabact_func107(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func107");
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

        if (arg81.enrg_sum == 0) // Possible devision by zero
            arg81.enrg_sum = 1;

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
    dprintf("MAKE ME %s\n","ypabact_func109");
    call_parent(zis, obj, 109, arg);
}

int ypabact_func110__sub0(__NC_STACK_ypabact *bact)
{
    float v2 = bact->field_621.sx - bact->field_3ec.sx;
    float v3 = bact->field_621.sz - bact->field_3ec.sz;


    if ( sqrt( POW2(v2) + POW2(v3) ) >= 300.0 )
        return 1;

    if ( !(bact->field_3D6 & 0x8000000) )
    {
        bact->field_598++;

        bact_arg67 arg67;

        if ( bact->field_59A > 1 )
        {
            arg67.field_0 = 5;
            arg67.field_4 = 0;
            arg67.targ.sx = bact->field_418[ bact->field_598 ].sx;
            arg67.targ.sz = bact->field_418[ bact->field_598 ].sz;

            call_method(bact->self, 67, &arg67);
        }

        if ( bact->field_598 >= bact->field_59A - 1 )
        {
            if ( bact->field_59c )
            {
                __NC_STACK_ypabact *v9 = sub_48C244(bact->wrld, bact->field_59c, bact->field_5a0);

                if ( v9 )
                {
                    if ( (1 << bact->owner) & v9->p_cell_area->field_39 )
                    {
                        arg67.field_8 = v9;
                        arg67.field_0 = 2;
                        arg67.field_4 = 0;

                        call_method(bact->self, 67, &arg67);
                    }
                }
            }

            bact->field_5a0 = 0;
            bact->field_59c = 0;
            bact->field_3D6 &= 0xF3FFFFFF;
        }
    }
    else
    {

        bact->field_598++;

        int v5 = bact->field_598;

        if ( bact->field_598 >= bact->field_59A )
        {
            bact->field_598 = 0;
            v5 = 0;
        }

        bact_arg67 arg67;

        arg67.field_0 = 5;
        arg67.field_4 = 0;
        arg67.targ.sx = bact->field_418[ v5 ].sx;
        arg67.targ.sz = bact->field_418[ v5 ].sz;

        call_method(bact->self, 67, &arg67);
    }
    return 1;
}

size_t ypabact_func110(NC_STACK_ypabact *obj, class_stru *zis, bact_arg110 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    int v6 = 0;
    int v9 = 0;

    if ( arg->field_one == 3 && (bact->field_3DE == 3 || bact->field_3DF == 3) )
        return 1;

    if ( !arg->field_one )
        return 3;


    if ( bact->field_3DE == 1 )
    {
        float v10 = bact->field_621.sx - bact->field_3ec.sx;
        float v11 = bact->field_621.sz - bact->field_3ec.sz;

        if ( sqrt( POW2(v10) + POW2(v11) ) < 1800.0 )
            v9 = 1;

        if ( bact->owner == ((cellArea *)bact->field_3e8)->owner )
            v6 = 1;
    }

    if ( bact->field_3DE == 2 )
    {
        float v13 = bact->field_621.sx - bact->field_3e8->field_621.sx;
        float v14 = bact->field_621.sz - bact->field_3e8->field_621.sz;

        if ( sqrt( POW2(v13) + POW2(v14) ) < 1800.0 )
            v9 = 1;

        if ( bact->owner == bact->field_3e8->owner )
            v6 = 1;
    }

    if ( arg->field_one == 2 )
    {
        __NC_STACK_ypabact *v16 = NULL;
        int v51, v54;

        if ( arg->field_two == 1 )
        {
            v16 = bact->field_3f8;
            v51 = 1;
            v54 = 50;
        }
        else if ( arg->field_two == 0)
        {
            v16 = bact->field_3e8;
            v51 = 0;
            v54 = 25;
        }

        if ( v16 )
        {
            float v17 = v16->field_621.sx - bact->field_621.sx;
            float v18 = v16->field_621.sz - bact->field_621.sz;

            float v50 = sqrt( POW2(v17) + POW2(v18) );

            if ( !( (1 << bact->owner) & v16->p_cell_area->field_39) )
                return 0;

            if ( bact->field_3D4 >= 100 )
            {
                if ( v51 && v50 > 2160.0 )
                    return 0;

                return 2;
            }

            if ( !v16->owner || v16->owner == bact->owner )
            {
                if ( v50 < 300.0 )
                    return 3;

                return 1;
            }

            if ( bact->field_3D6 & 0x4000 )
            {
                if ( !v9 )
                    return 0;

                return 2;
            }

            if ( bact->field_3D4 >= v54 )
            {
                if ( !v51 || bact->field_24 == 9 )
                    return 2;

                if ( v50 > 2160.0 )
                    return 0;

                xyz *v21;

                if ( bact->field_32 == bact->parent_bacto )
                {

                    if ( bact->field_3DE == 1 )
                        v21 = &bact->field_3ec;
                    else if ( bact->field_3DE == 2 )
                        v21 = &bact->field_3e8->field_621;
                    else
                        v21 = &bact->field_621;
                }
                else
                {
                    __NC_STACK_ypabact *a4;
                    call_vtbl(bact->parent_bacto, 3, 0x80001003, &a4, 0);

                    if ( a4->field_3DE == 1 )
                        v21 = &a4->field_3ec;
                    else if ( a4->field_3DE == 2 )
                        v21 = &a4->field_3e8->field_621;
                    else
                        v21 = &bact->field_621;
                }

                float v26 = v21->sz - bact->field_621.sz;
                float v27 = v21->sx - bact->field_621.sx;

                if ( sqrt(POW2(v26) + POW2(v27)) > 3600.0 )
                {
                    nlist *lst;
                    call_vtbl(bact->field_3f8->self, 3, 0x80001008, &lst, 0);

                    bact_node *v43 = (bact_node *)lst->head;

                    int v28 = 0;

                    while(v43->next)
                    {
                        bact_node *v44;

                        call_vtbl(v43->bacto, 3, 0x80001012, &v44, 0);

                        if ( v43 == v44 && v44->bact->owner == bact->owner )
                            v28++;

                        if ( v28 > 2 )
                            break;

                        v43 = (bact_node *)v43->next;
                    }

                    if ( v28 > 2 )
                        return 0;
                }
                return 2;
            }

            if ( !v9 || !v6 )
                return 0;

            return 2;
        }
    }
    else if ( arg->field_one == 1 )
    {

        cellArea *v52 = NULL;
        int v51 = 0;
        int v54 = 0;

        float v48 = 0;
        float v47 = 0;

        if ( bact->field_3DF == 1 )
        {
            v52 = (cellArea *)bact->field_3f8;

            v48 = bact->field_3fc.sx;
            v47 = bact->field_3fc.sz;

            v54 = 75;
            v51 = 1;
        }
        else if ( bact->field_3DE == 1 )
        {
            v52 = (cellArea *)bact->field_3e8;

            v48 = bact->field_3ec.sx;
            v47 = bact->field_3ec.sz;

            v54 = 25;
            v51 = 0;
        }

        if ( bact->field_3D6 & 0x4000000 && !v51 )
            return ypabact_func110__sub0(bact);

        if ( !v52 )
            return 3;

        int v32;

        if ( v52->field_2E == 1 )
        {
            v32 = v52->buildings_health[0][0];
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    v32 += v52->buildings_health[i][j];
                }
            }
        }

        float v37 = bact->field_621.sx - v48;
        float v38 = bact->field_621.sz - v47;

        float v49 = sqrt( POW2(v37) + POW2(v38) );

        if ( bact->field_3D4 >= 100 )
        {
            if ( v32 <= 0 && v52->owner == bact->owner )
            {
                if ( v49 < 300.0 )
                    return 3;

                return 1;
            }

            return 2;
        }

        if ( v49 >= 300.0 )
        {
            if ( bact->owner != v52->owner )
            {
                if ( bact->field_3D6 & 0x4000 || bact->field_3D4 < v54 )
                    return 0;

                return 2;
            }

            if ( v51 )
                return 0;

            return 1;
        }

        if ( bact->owner == v52->owner )
        {
            if ( v51 )
                return 0;

            return 3;
        }

        if ( bact->field_3D6 & 0x4000 || bact->field_3D4 < v54 )
            return 0;
        else
            return 2;
    }
    else
        return 3;

    return 3;
}

size_t ypabact_func111(NC_STACK_ypabact *obj, class_stru *zis, __NC_STACK_ypabact *)
{
    return 1;
}

void ypabact_func112(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    float v14 = 0.66;

    if ( bact->field_a28 <= 0 )
    {
        if ( bact->field_931 >= 1980.0 )
        {
            if ( bact->field_931 >= 3000 )
            {
                call_method(bact->wrld, 168, &bact);

                bact->field_3D6 |= 0x2000000;

                call_method(obj, 77);

                if ( bact->field_B34 & 2 )
                    bact->field_3D6 |= 0x400000;
                else
                    call_method(obj, 118, obj);

                bact->field_3D6 &= 0xFFEFFFFF;
            }
            else
            {
                bact->field_3D6 |= 0x100000;

                bact->field_67D.sx = 1.0;
                bact->field_67D.sy = 30.0 - ((bact->field_931 - 1980.0) * 30.0 / 1020.0);
                bact->field_67D.sz = 1.0;
            }
        }
        else
        {
            if ( bact->vp_genesis.base != bact->current_vp.base )
            {
                bact_arg119 arg78;
                arg78.field_0 = 5;
                arg78.field_4 = 0;
                arg78.field_8 = 0;

                call_method(obj, 78, &arg78);
            }

            bact->field_3D6 |= 0x100000;

            bact->field_67D.sx = 1.0;
            bact->field_67D.sy = (30 * bact->field_931)/ (v14 * 3000.0);
            bact->field_67D.sz = 1.0;
        }

        bact->field_931 += arg->field_4;
    }
    else
    {
        bact->field_a28 -= arg->field_4;
    }
}

void __fastcall ypabact_func113__sub0(__NC_STACK_ypabact *main, destFX *fx)
{
    ypaworld_arg146 arg146;

    arg146.pos = main->field_621;
    arg146.vehicle_id = fx->p1;

    if ( main->radius > 31.0 )    // 31.0
    {
        float v23 = sqrt( POW2(fx->p2) + POW2(fx->p3) + POW2(fx->p4) );

        if ( v23 > 0.1 )
        {
            float v5 = 1.0 / v23;

            float v15 = fx->p2 * v5 * main->radius;
            float v16 = fx->p3 * v5 * main->radius;
            float v17 = fx->p4 * v5 * main->radius;

            arg146.pos.sx += v15 * main->field_651.m00 + v16 * main->field_651.m01 + v17 * main->field_651.m02;
            arg146.pos.sy += v15 * main->field_651.m10 + v16 * main->field_651.m11 + v17 * main->field_651.m12;
            arg146.pos.sz += v15 * main->field_651.m20 + v16 * main->field_651.m21 + v17 * main->field_651.m22;
        }
    }

    NC_STACK_ypabact *bah = (NC_STACK_ypabact *)call_method(main->wrld, 146, &arg146);

    if ( bah )
    {
        call_method(main->wrld, 134, bah);

        bact_arg119 v18;
        v18.field_0 = 2;
        v18.field_4 = 0;
        v18.field_8 = 0;

        call_method(bah, 119, &v18);

        __NC_STACK_ypabact *a4;
        call_vtbl(bah, 3, 0x80001003, &a4, 0);

        a4->field_605.sx = main->field_651.m00 * fx->p2 + main->field_651.m01 * fx->p3 + main->field_651.m02 * fx->p4;
        a4->field_605.sy = main->field_651.m10 * fx->p2 + main->field_651.m11 * fx->p3 + main->field_651.m12 * fx->p4;
        a4->field_605.sz = main->field_651.m20 * fx->p2 + main->field_651.m21 * fx->p3 + main->field_651.m22 * fx->p4;

        if ( fx->type_flag & 0x10 )
        {
            a4->field_605.sx += main->field_605.sx * main->field_611;
            a4->field_605.sy += main->field_605.sy * main->field_611;
            a4->field_605.sz += main->field_605.sz * main->field_611;
        }

        float v24 = sqrt( POW2(a4->field_605.sx) + POW2(a4->field_605.sy) + POW2(a4->field_605.sz) );

        if ( v24 > 0.001 )
        {
            a4->field_611 = v24;

            float v13 = 1.0 / v24;

            a4->field_605.sx *= v13;
            a4->field_605.sy *= v13;
            a4->field_605.sz *= v13;
        }
    }
}

void ypabact_func113(NC_STACK_ypabact *obj, class_stru *zis, uint8_t *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( call_method(bact->wrld, 145, bact) )
    {
        int a4;
        call_vtbl(bact->wrld, 3, 0x8000201B, &a4, 0);

        if (a4 > 16)
            a4 = 16;

        for (int i = 0; i < a4; i++)
        {
            if ( bact->dest_fx[i].p1 )
            {
                destFX *v8 = &bact->dest_fx[i];

                if ( (v8->type_flag & 2 && *arg == 2) || (v8->type_flag & 1 && *arg == 1) || (v8->type_flag & 4 && *arg == 4) || (v8->type_flag & 8 && *arg == 8) )
                    ypabact_func113__sub0(bact, v8);
            }
        }
    }
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
    dprintf("MAKE ME %s\n","ypabact_func116");
    call_parent(zis, obj, 116, arg);
}

void ypabact_func117(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func117");
    call_parent(zis, obj, 117, arg);
}

void ypabact_func118(NC_STACK_ypabact *obj, class_stru *zis, NC_STACK_ypabact *b_bacto)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    _NC_STACK_ypaworld *yw = &bact->wrld->stack__ypaworld;

    __NC_STACK_ypabact *b_bact;
    call_vtbl(b_bacto, 3, 0x80001003, &b_bact, 0);

    if ( b_bact->owner )
    {
        if ( yw->field_757E )
        {
            if ( b_bact->field_24 != 4 )
            {
                char v7[24];
//        *(_DWORD *)v7 = 1002;
//        v7[12] = b_bact->owner;
//        *(_DWORD *)&v7[16] = b_bact->ypabact__id;
//        v7[20] = b_bact->field_24;
                yw_arg181 v6;
                v6.field_14 = 2;
                v6.field_10 = 0;
                v6.value = v7;
                v6.field_18 = 1;
                v6.val_size = 24;

                call_method(yw->self_full, 181, &v6);
            }
        }
    }

    call_method(bact->wrld, 144, b_bacto);
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

        if ( bact->field_3B2 & 0x200 )
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

void ypabact_func120(NC_STACK_ypabact *obj, class_stru *zis, yw_arg129 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    arg->field_14 = 255;

    call_method(bact->wrld, 129, arg);

    yw_130arg arg130;
    arg130.pos_x = arg->pos.sx;
    arg130.pos_z = arg->pos.sz;

    int v5;

    if ( call_method(bact->wrld, 130, &arg130) )
        v5 = arg130.pcell->owner;
    else
        v5 = 0;

    if ( bact->wrld->stack__ypaworld.field_757E )
    {
        char v8[40];
//    *(_DWORD *)v8 = 1011;
//    v8[12] = bact->owner;
//    *(float *)&v8[16] = arg->pos.sx;
//    *(float *)&v8[20] = arg->pos.sy;
//    *(float *)&v8[24] = arg->pos.sz;
//    *(_DWORD *)&v8[28] = arg->field_10;
        v8[36] = v5;
//    v7 = arg->unit;
//    if ( v7 )
//      v7 = (__NC_STACK_ypabact *)v7->ypabact__id;
//    *(_DWORD *)&v8[32] = v7;

        yw_arg181 arg181;
        arg181.field_10 = 0;
        arg181.field_14 = 2;
        arg181.value = v8;
        arg181.val_size = 40;
        arg181.field_18 = 1;

        call_method(bact->wrld, 181, &arg181);
    }
}

void sb_0x4874c4(__NC_STACK_ypabact *bact, int a2, int a3, float a4)
{
    bact->field_6BD[0].field_0 = sqrt( (float)bact->field_931 / (float)a2 ) * a4;

    if ( bact->field_6BD[0].field_0 < 0.0 )
        bact->field_6BD[0].field_0 = 0;

    mat_rotate_y(&bact->field_6BD[0].matrx, bact->maxrot * 2.0 * (float)a3 * 0.001);
}

void ypabact_func121(NC_STACK_ypabact *obj, class_stru *zis, ypabact_arg65 *arg)
{
    __NC_STACK_ypabact *bact = &obj->stack__ypabact;

    if ( bact->field_3D6 & 0x200 || (bact->field_915 - bact->field_955 > 5000 && bact->field_3D6 & 0x400 ) )
    {
        if ( !(bact->field_3D6 & 0x800) )
        {
            bact_arg119 arg78;
            arg78.field_0 = 0;
            arg78.field_8 = 0;
            arg78.field_4 = 2048;

            call_method(obj, 78, &arg78);
        }

        bact->field_3D6 |= 0x200;

        if ( bact->owner && bact->field_24 != 4 && bact->vp_genesis.base )
        {
            int a2 = bact->energy_2 * 0.7;

            if ( a2 < 10000 )
                a2 = 10000;

            if ( a2 > 25000 )
                a2 = 25000;

            if ( bact->field_6BD[0].field_34 & 1 )
            {
                bact->field_931 -= arg->field_4;

                if ( bact->field_931 <= 0 )
                {
                    bact->field_6BD[0].vp.trigo = NULL;
                    bact->field_6BD[0].vp.base = NULL;

                    if ( bact->field_B74 <= 0 )
                    {

                        if ( bact->field_B34 & 2 )
                            bact->field_3D6 |= 0x400000;
                        else
                            call_method(obj, 118, obj);

                    }
                }
                else
                {
                    sb_0x4874c4(bact, a2, arg->field_4, 0.75);

                    if ( bact->field_B74 <= 0 )
                        bact->field_3D6 |= 0x400000;
                }
            }
            else
            {
                bact->field_931 = a2;
                bact->field_6BD[0].field_0 = 0.75;
                bact->field_6BD[0].field_4 = bact->field_621;
                bact->field_6BD[0].matrx = bact->field_651;
                bact->field_6BD[0].vp = bact->vp_genesis;
                bact->field_6BD[0].field_34 |= 3;

                if ( bact->wrld->stack__ypaworld.field_757E )
                {
                    char v9[76];
//          *(_DWORD *)v9 = 1030;
//          v9[12] = bact->owner;
//          *(float *)&v9[20] = 0.75;
//          *(_DWORD *)&v9[16] = a2;
//          *(_DWORD *)&v9[24] = bact->ypabact__id;
//          *(float *)&v9[28] = bact->field_621.sx;
//          *(float *)&v9[32] = bact->field_621.sy;
//          *(float *)&v9[36] = bact->field_621.sz;
//          memcpy(&v9[40], &bact->field_651, 0x24);

                    yw_arg181 arg181;
                    arg181.value = v9;
                    arg181.val_size = 76;
                    arg181.field_10 = 0;
                    arg181.field_14 = 2;
                    arg181.field_18 = 1;

                    call_method(bact->wrld, 181, &arg181);
                }
            }
        }
        else if ( bact->field_B74 <= 0 )
        {
            if ( bact->field_B34 & 2 )
                bact->field_3D6 |= 0x400000;
            else
                call_method(obj, 118, obj);
        }
    }
    else
    {
        bact_arg86 arg86;
        arg86.field_one = 3;
        arg86.field_two = arg->field_4;

        call_method(obj, 86, &arg86);
    }
}

void ypabact_func122(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func122");
    call_parent(zis, obj, 122, arg);
}

void ypabact_func123(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func123");
    call_parent(zis, obj, 123, arg);
}

void ypabact_func124(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func124");
    call_parent(zis, obj, 124, arg);
}

void ypabact_func125(NC_STACK_ypabact *obj, class_stru *zis, stack_vals *arg)
{
    dprintf("MAKE ME %s\n","ypabact_func125");
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
