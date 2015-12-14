#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "utils.h"


stored_functions *classvtbl_get_rsrc();
class_return * class_set_rsrc(int, ...);

stored_functions rsrc_class_vtbl(class_set_rsrc);


class_stored rsrc_class_off (NULL, NULL, "MC2classes:rsrc.class", classvtbl_get_rsrc);


stored_functions *classvtbl_get_rsrc()
{
	return &rsrc_class_vtbl;
}

CLASSFUNC rsrc_funcs[1024];

nlist g_rsrc_list1;
nlist g_rsrc_list2;


NC_STACK_rsrc * rsrc_func0(class_stru *caller, class_stru *zis, stack_vals *stak)
{
	NC_STACK_rsrc *clss = (NC_STACK_rsrc *)call_parent(zis, caller, 0, stak); // nucleus_func0

	if (clss)
	{
		__NC_STACK_rsrc *internal = &clss->stack__rsrc;

		const char *res_name = (const char *)find_id_in_stack_def_val(0x80001000, 0, stak);
		int reuse_loaded = find_id_in_stack_def_val(0x80001001, 1, stak);

		if ( res_name )
		{
			rsrc *res = 0;
			if ( reuse_loaded == 1 )
				res = (rsrc *)FIND(&g_rsrc_list1, res_name);

			if (!res)
				res = (rsrc *)call_method(clss, 64, stak);

			if ( res )
			{
				res->ref_cnt++;
				internal->p_rsrc = res;
				internal->p_data = res->data;

				if ( find_id_in_stack_def_val(0x80001003, 0, stak) )
					internal->field_8[0] |= 1;

				return clss;
			}
			else
			{
				call_method(clss, 1);
				return NULL;
			}
		}
		else
		{
			call_method(clss, 1);
			return NULL;
		}
	}
	return NULL;
}

size_t rsrc_func1(NC_STACK_rsrc *a1, class_stru *a2, stack_vals *a3)
{
	__NC_STACK_rsrc *internal = &a1->stack__rsrc;

	if ( internal->p_rsrc )
	{
		rsrc *res = internal->p_rsrc;
		res->ref_cnt--;
		if ( !res->ref_cnt )
			call_method(a1, 65, &res);
	}

	return call_parent(a2, a1, 1, a3);
}

void rsrc_func3(NC_STACK_rsrc *caller, class_stru *zis, stack_vals *stak)
{

	__NC_STACK_rsrc *internal = &caller->stack__rsrc;

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

			case 0x80001000:
			{
				rsrc *res = internal->p_rsrc;
				if ( res )
					*(const char **)stk->value = res->name;
				else
					*(const char **)stk->value = NULL;
				break;
			}

			case 0x80001002:
				*(void **)stk->value = internal->p_data;
				break;

			case 0x80001001:
				if ( internal->p_rsrc == NULL)
					*(int *)stk->value = 2;
				else
					*(int *)stk->value = internal->p_rsrc->what_list;
				break;

			case 0x80001003:
				if ( internal->field_8[0] & 1 )
					*(int *)stk->value = 1;
				else
					*(int *)stk->value = 0;
				break;

			case 0x80001006:
				*(nlist **)stk->value = &g_rsrc_list1;
				break;

			case 0x80001007:
				*(nlist **)stk->value = &g_rsrc_list2;
				break;
			}
			stk++;
		}
	}
	call_parent(zis, caller, 3, stak);
}

// Allocate resource node
rsrc *rsrc_func64(NC_STACK_rsrc *obj, class_stru *zis, stack_vals *stak)
{
	char *title = (char *)find_id_in_stack_def_val(0x80001000, 0, stak);

	int what_list = find_id_in_stack_def_val(0x80001001, 1, stak);

	int toTail = find_id_in_stack_def_val(0x80001008, 0, stak);

	rsrc *res = NULL;

	if ( title )
	{
		res = (rsrc *)AllocVec(sizeof(rsrc), 1);

		if ( res )
		{
			strncpy(res->title, title, 64);

			res->ref_cnt = 0;
			res->data = NULL;
			res->name = res->title;
			res->what_list = what_list;

			call_vtbl(obj, 3, 0x80000002, &res->class_name, 0);

			nlist *lst;

			if (what_list == 1)
				lst = &g_rsrc_list1;
			else
				lst = &g_rsrc_list2;

			if ( toTail )
				AddTail(lst, res);
			else
				AddHead(lst, res);
		}
	}
	return res;
}

void rsrc_func65(NC_STACK_rsrc *obj, class_stru *zis, rsrc **res)
{
	Remove(*res);
	if ( *res )
		nc_FreeMem(*res);
}


class_return rsrc_class_descr;

class_return * class_set_rsrc(int, ...)
{
	init_list(&g_rsrc_list1);
	init_list(&g_rsrc_list2);
	memset(rsrc_funcs, 0, sizeof(CLASSFUNC) * 1024);

	rsrc_funcs[0] = (CLASSFUNC)rsrc_func0;
	rsrc_funcs[1] = (CLASSFUNC)rsrc_func1;
	rsrc_funcs[3] = (CLASSFUNC)rsrc_func3;
	rsrc_funcs[64] = (CLASSFUNC)rsrc_func64;
	rsrc_funcs[65] = (CLASSFUNC)rsrc_func65;

	rsrc_class_descr.parent = "nucleus.class";

	rsrc_class_descr.vtbl = rsrc_funcs;
	////rsrc_class_descr.varSize = sizeof(__NC_STACK_rsrc);
	rsrc_class_descr.varSize = sizeof(NC_STACK_rsrc) - offsetof(NC_STACK_rsrc, stack__rsrc); //// HACK
	rsrc_class_descr.field_A = 0;
	return &rsrc_class_descr;
}
