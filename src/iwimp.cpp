#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "engine_input.h"
#include "iwimp.h"
#include "utils.h"



const NewClassDescr NC_STACK_iwimp::description("iwimp.class", &newinstance);



size_t NC_STACK_iwimp::func0(stack_vals *stak)
{
    if ( !NC_STACK_idev::func0(stak) )
        return 0;

    __NC_STACK_iwimp *wimp = &stack__iwimp;

    init_list(&wimp->list);

    return 1;
}

size_t NC_STACK_iwimp::iwimp_func128(stack_vals *)
{
    return 1;
}

void NC_STACK_iwimp::iwimp_func129(iwimp_arg129 *arg)
{
    nlist *lst = &stack__iwimp.list;

    if ( arg->field_4 & 1 )
        AddHead(lst, arg->node);
    else
        AddTail(lst, arg->node);
}

void NC_STACK_iwimp::iwimp_func130(iwimp_arg129 *arg)
{
    Remove(arg->node);
}

void sub_41D538(__NC_STACK_iwimp *wimp, winp_131arg *arg, shortPoint *points)
{
    inp_node *btn = (inp_node *)wimp->list.head;

    if ( btn->next )
    {
        while ( 1 )
        {
            points[1].x = points[0].x - btn->btn_xpos;
            points[1].y = points[0].y - btn->btn_ypos;

            if ( points[1].x >= 0 && points[1].x < btn->btn_width &&
                    points[1].y >= 0 && points[1].y < btn->btn_height )
                break;

            btn = (inp_node *)btn->next;
            if ( !btn->next )
                return;
        }

        arg->selected_btn = btn;

        if ( btn->field_10 )
        {
            for (int i = 0; i < btn->field_10; i++)
            {
                button_str1 *v11 = btn->field_18[i];

                points[2].x = points[1].x - v11->xpos;
                points[2].y = points[1].y - v11->ypos;

                if ( points[2].x >= 0 && points[2].x < v11->width &&
                        points[2].y >= 0 && points[2].y < v11->fnt_height )
                {
                    arg->selected_btnID = i;
                    break;
                }
            }
        }
    }
}

void NC_STACK_iwimp::iwimp_func131(winp_131arg *arg)
{
    __NC_STACK_iwimp *wimp = &stack__iwimp;

    arg->flag |= 1;
    arg->selected_btn = NULL;
    arg->selected_btnID = -1;

    sub_41D538(wimp, arg, arg->move);

    if ( arg->flag & 4 )
    {
        if ( arg->selected_btnID >= 0 &&
                wimp->selected_btn == arg->selected_btn &&
                wimp->selected_btnID == arg->selected_btnID )
            arg->flag |= 0x20;
    }

    if ( arg->flag & 2 )
    {
        sub_41D538(wimp, arg, arg->ldw_pos);

        if ( arg->selected_btnID == -1 )
        {
            wimp->selected_btn = 0;
            wimp->selected_btnID = -1;
        }
        else
        {
            wimp->selected_btn = arg->selected_btn;
            wimp->selected_btnID = arg->selected_btnID;
            arg->flag |= 0x10;
        }
    }

    if ( arg->flag & 8 )
    {
        sub_41D538(wimp, arg, arg->lup_pos);

        if ( arg->selected_btn )
        {
            if ( arg->selected_btn == wimp->selected_btn &&
                    arg->selected_btnID == wimp->selected_btnID )
                arg->flag |= 0x40;
        }
        wimp->selected_btnID = -1;
        wimp->selected_btn = 0;
    }
}


size_t NC_STACK_iwimp::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 128:
        return (size_t)iwimp_func128( (stack_vals *)data );
    case 129:
        iwimp_func129( (iwimp_arg129 *)data );
        return 1;
    case 130:
        iwimp_func130( (iwimp_arg129 *)data );
        return 1;
    case 131:
        iwimp_func131( (winp_131arg *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_idev::compatcall(method_id, data);
}

