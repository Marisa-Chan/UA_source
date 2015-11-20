#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "iwimp.h"
#include "utils.h"


stored_functions *classvtbl_get_iwimp();
class_return * class_set_iwimp(int, ...);

stored_functions iwimp_class_vtbl(class_set_iwimp);


class_stored iwimp_class_off (NULL, NULL, "MC2classes:iwimp.class", classvtbl_get_iwimp);


stored_functions *classvtbl_get_iwimp()
{
	return &iwimp_class_vtbl;
}

CLASSFUNC iwimp_funcs[1024];

NC_STACK_iwimp * iwimp_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
	NC_STACK_iwimp *obj = (NC_STACK_iwimp *)call_parent(zis, clss, 0, stak);

	if (obj)
	{
		__NC_STACK_iwimp *wimp = &obj->stack__iwimp;

		init_list(&wimp->list);
	}

	return obj;
}

size_t iwimp_func128(NC_STACK_iwimp *obj, class_stru *zis, stack_vals *stak)
{
	return 1;
}

void iwimp_func129(NC_STACK_iwimp *obj, class_stru *zis, iwimp_arg129 *arg)
{
	nlist *lst = &obj->stack__iwimp.list;

	if ( arg->field_4 & 1 )
		AddHead(lst, arg->node);
	else
		AddTail(lst, arg->node);
}

void iwimp_func130(NC_STACK_iwimp *obj, class_stru *zis, iwimp_arg129 *arg)
{
	Remove(arg->node);
}

void sub_41D538(__NC_STACK_iwimp *wimp, winp_131arg *arg, shortPoint *points)
{
	__NC_STACK_button *btn = (__NC_STACK_button *)wimp->list.head;

	if ( btn->next )
	{
		while ( 1 )
		{
			points[1].x = points[0].x - btn->btn_xpos;
			points[1].y = points[0].y - btn->btn_ypos;

			if ( points[1].x >= 0 && points[1].x < btn->btn_width &&
					points[1].y >= 0 && points[1].y < btn->btn_height )
				break;

			btn = (__NC_STACK_button *)btn->next;
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

void iwimp_func131(NC_STACK_iwimp *obj, class_stru *zis, winp_131arg *arg)
{
	__NC_STACK_iwimp *wimp = &obj->stack__iwimp;

	arg->flag |= 1;
	arg->selected_btn = NULL;
	arg->selected_btnID = -1;

	sub_41D538(wimp, arg, arg->move);

	if ( arg->flag & 4 )
	{
		if ( arg->selected_btnID >= 0 &&
				wimp->selected_btn == arg->selected_btn &&
				arg->selected_btnID == wimp->selected_btnID )
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
					wimp->selected_btnID == arg->selected_btnID )
				arg->flag |= 0x40;
		}
		wimp->selected_btnID = -1;
		wimp->selected_btn = 0;
	}
}


class_return iwimp_class_descr;

class_return * class_set_iwimp(int a1, ...)
{

	memset(iwimp_funcs, 0, sizeof(CLASSFUNC) * 1024);

	iwimp_funcs[0] = (CLASSFUNC)iwimp_func0;
	iwimp_funcs[128] = (CLASSFUNC)iwimp_func128;
	iwimp_funcs[129] = (CLASSFUNC)iwimp_func129;
	iwimp_funcs[130] = (CLASSFUNC)iwimp_func130;
	iwimp_funcs[131] = (CLASSFUNC)iwimp_func131;

	iwimp_class_descr.parent = "idev.class";

	iwimp_class_descr.vtbl = iwimp_funcs;
	////iwimp_class_descr.varSize = sizeof(__NC_STACK_iwimp);
	iwimp_class_descr.varSize = sizeof(NC_STACK_iwimp) - offsetof(NC_STACK_iwimp, stack__iwimp); //// HACK
	iwimp_class_descr.field_A = 0;
	return &iwimp_class_descr;
}
