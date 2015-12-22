#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "utils.h"
#include "engine_input.h"
#include "input.h"



stored_functions *classvtbl_get_input();
class_return * class_set_input(int, ...);

stored_functions input_class_vtbl(class_set_input);


class_stored input_class_off (NULL, NULL, "MC2classes:input.class", classvtbl_get_input);


stored_functions *classvtbl_get_input()
{
	return &input_class_vtbl;
}

CLASSFUNC input_funcs[1024];

NC_STACK_input *input_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
	NC_STACK_input *obj = (NC_STACK_input *)call_parent(zis, clss, 0, stak);
	if ( obj )
	{
		__NC_STACK_input *inp = &obj->stack__input;

		for (int i = 0; i < 32; i++)
		{
			init_list(&inp->buttons_lists[i]);
			init_list(&inp->sliders_lists[i]);
		}
	}
	return obj;
}

size_t input_func1(NC_STACK_input *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_input *inp = &obj->stack__input;

	if ( inp->timer )
		delete_class_obj(inp->timer);

	if ( inp->wimp )
		delete_class_obj(inp->wimp);

	if ( inp->keyboard )
		delete_class_obj(inp->keyboard);

	for (int i = 0; i < 32; i++)
	{
		while( 1 )
		{
			inputkey_node *nod = (inputkey_node *)RemHead(&inp->buttons_lists[i]);

			if (!nod)
				break;

			if (nod->driver_obj)
				delete_class_obj(nod->driver_obj);

			nc_FreeMem(nod);
		}

		while( 1 )
		{
			inputkey_node *nod = (inputkey_node *)RemHead(&inp->sliders_lists[i]);

			if (!nod)
				break;

			if (nod->driver_obj)
				delete_class_obj(nod->driver_obj);

			nc_FreeMem(nod);
		}
	}
	return call_parent(zis, obj, 1, stak);
}

const char * input__parse__inputkey_node(const char *a1, nlist *lst)
{
	inputkey_node *node = (inputkey_node *)AllocVec(sizeof(inputkey_node), 65537);

	if ( node )
		AddTail(lst, node);

	if ( node )
	{
		char *out = node->driver_name;
		int v8 = 0;

		while ( 1 )
		{
			char v10 = *a1;

			if (!v10)
				return NULL;
			else if (v10 == '\t' || v10 == ' ')
			{
				if (v8)
					return a1;
			}
			else if (v10 == '#')
			{
				if (v8)
					return a1;

				node->field_C |= 8;
			}
			else if (v10 == '&')
			{
				if (v8)
					return a1;

				node->field_C |= 1;
			}
			else if (v10 == ':')
			{
				v8 = 1;
				out = node->keyname;
			}
			else if (v10 == '|')
			{
				if (v8)
					return a1;

				node->field_C |= 2;
			}
			else if (v10 == '~')
			{
				if (v8)
					return a1;

				node->field_C |= 4;
			}
			else
			{
				*out = v10;
				out++;
			}

			a1++;
		}
	}
	return NULL;
}

int input__set_buttons_and_sliders(__NC_STACK_input *inp, input__func64__params *arg)
{
	nlist *v6;

	if ( arg->type_id == 4 )
		v6 = &inp->buttons_lists[arg->item_number];
	else
		v6 = &inp->sliders_lists[arg->item_number];

	if ( arg->value )
	{
		while ( 1 )
		{
			inputkey_node *node = (inputkey_node *)RemHead(v6);

			if ( !node )
				break;

			if ( node->driver_obj )
				delete_class_obj(node->driver_obj);

			nc_FreeMem(node);
		}

		const char *v3 = input__parse__inputkey_node(arg->value, v6);

		while (v3)
			v3 = input__parse__inputkey_node(v3, v6);

		for ( inputkey_node *node = (inputkey_node *)v6->head; node->next; node = (inputkey_node *)node->next )
		{
			if ( node->driver_name[0] && node->keyname[0] )
			{
				char classname[256];

				strcpy(classname, "drivers/input/");
				strcat(classname, node->driver_name);
				strcat(classname, ".class");

				node->driver_obj = (NC_STACK_winp *)init_get_class(classname, 0);
				if ( node->driver_obj )
				{
					char *v26 = node->keyname;

					node->keyType = call_method(node->driver_obj, 67, &v26);

					if ( !node->keyType )
						ypa_log_out("input.class, WARN: Driver '%s' did not accept id '%s'.\n", node->driver_name, node->keyname);
				}
				else
				{
					ypa_log_out("input.class, WARN: Unknown driver '%s'.\n", node->driver_name);
				}
			}
		}
	}
	return 1;
}

size_t input_func64(NC_STACK_input *obj, class_stru *, input__func64__params *arg)
{
	char classname[256];

	__NC_STACK_input *inp = &obj->stack__input;

	switch ( arg->type_id )
	{
	case 1:                                     // input.wimp
		if ( inp->wimp )
			delete_class_obj(inp->wimp);

		strcpy(classname, "drivers/input/");
		strcat(classname, arg->value);
		strcat(classname, ".class");

		inp->wimp = (NC_STACK_winp *)init_get_class(classname, 0);
		if ( !inp->wimp )
			return 0;
		break;

	case 2:                                     // input.timer
		if ( inp->timer )
			delete_class_obj(inp->timer);

		strcpy(classname, "drivers/input/");
		strcat(classname, arg->value);
		strcat(classname, ".class");

		inp->timer = (NC_STACK_wintimer *)init_get_class(classname, 0);
		if ( !inp->timer )
			return 0;
		break;

	case 3:
		if ( inp->keyboard )
			delete_class_obj(inp->keyboard);

		strcpy(classname, "drivers/input/");
		strcat(classname, arg->value);
		strcat(classname, ".class");

		inp->keyboard = (NC_STACK_winp *)init_get_class(classname, 0);
		if ( !inp->keyboard )
			return 0;
		break;

	case 4:
		if ( !input__set_buttons_and_sliders(inp, arg) )
			return 0;
		break;

	case 5:
		if ( !input__set_buttons_and_sliders(inp, arg) )
			return 0;
		break;

	default:
		return 0;
		break;
	}
	return 1;
}

void sub_41CC94(nlist *lst, inp_l65 *loc, int arg)
{
	win_64arg warg64;

	inputkey_node *node = (inputkey_node *)lst->head;
	if ( lst->head->next )
	{
		while ( 1 )
		{
			if ( node->driver_obj )
			{
				warg64.field_0 = arg;

				if (node->keyType == 1)
				{
					if (node->field_C & 8)
					{
						call_method(node->driver_obj, 65, &warg64);

						if ( node->field_C & 4 )
							loc->field_4 -= warg64.field_8;
						else
							loc->field_4 += warg64.field_8;
					}
					else
					{
						call_method(node->driver_obj, 64, &warg64);

						int v9 = warg64.field_4;

						if ( node->field_C & 4 )
							v9 = warg64.field_4 == 0;

						if ( node->field_C & 2 )
							loc->field_0 |= v9;
						else
							loc->field_0 &= v9;
					}
				}
				else if (node->keyType == 2)
				{
					call_method(node->driver_obj, 65, &warg64);

					if ( node->field_C & 4 )
						loc->field_4 -= warg64.field_8;
					else
						loc->field_4 += warg64.field_8;
				}
			}

			node = (inputkey_node *)node->next;
			if ( !node->next )
				return;
		}
	}
}

void input_func65(NC_STACK_input *obj, class_stru *, struC5 *arg)
{
	__NC_STACK_input *inp = &obj->stack__input;
	memset(arg, 0, sizeof(struC5));

	if ( inp->timer )
		arg->period = call_method(inp->timer, 64);
	else
		arg->period = 20;

	inp->field_4 += arg->period;

	if ( inp->wimp &&
			call_method(inp->wimp, 128) )
	{
		if ( inp->keyboard )
		{
			winp_66arg v15;
			memset(&v15, 0, sizeof(winp_66arg));

			call_method(inp->keyboard, 66, &v15);

			arg->downed_key_2 = v15.downed_key_2;
			arg->downed_key = v15.downed_key;
			arg->dword8 = v15.dword8;
			arg->chr = v15.chr;
		}

		call_method(inp->wimp, 131, &arg->winp131arg);

		for (int i = 0; i < 32; i++)
		{
			inp_l65 loc;
			loc.field_0 = 1;
			loc.field_4 = 0;

			sub_41CC94(&inp->buttons_lists[i], &loc, inp->field_4);

			if (loc.field_0)
				arg->but_flags |= 1 << i;
		}

		for (int i = 0; i < 32; i++)
		{
			inp_l65 loc;
			loc.field_0 = 1;
			loc.field_4 = 0;

			sub_41CC94(&inp->sliders_lists[i], &loc, inp->field_4);

			if (loc.field_0)
				arg->sliders_vars[i] = loc.field_4;
		}

		arg->field_9 = arg->sliders_vars[0] * 32.0;
		arg->field_B = arg->sliders_vars[1] * 32.0;
	}
	else
	{
		for (int i = 0; i < 32; i++)
		{
			inp_l65 loc;
			sub_41CC94(&inp->sliders_lists[i], &loc, inp->field_4);
		}
	}
}

size_t input_func66(NC_STACK_input *obj, class_stru *, input__func66__params *arg)
{
	__NC_STACK_input *inpt = &obj->stack__input;

	NC_STACK_class *v7 = NULL;
	if ( arg->field_0 == 1 )
	{
		v7 = inpt->wimp;
	}
	else if ( arg->field_0 == 2 )
	{
		v7 = inpt->timer;
	}
	else if ( arg->field_0 == 3 )
	{
		v7 = inpt->keyboard;
	}

	if ( v7 )
	{
		return call_method(v7, arg->funcID, arg->vals);
	}
	else if ( arg->field_0 == 4 || arg->field_0 == 5 )
	{
		nlist *lst;

		if ( arg->field_0 == 4 )
			lst = &inpt->buttons_lists[arg->field_4];
		else
			lst = &inpt->sliders_lists[arg->field_4];

		inputkey_node *node = (inputkey_node *)lst->head;
		while (node->next)
		{
			if (node->driver_obj)
			{
				call_method(node->driver_obj, arg->funcID, arg->vals);
			}

			node = (inputkey_node *)node->next;
		}
		return 0;
	}
	return 0;
}


class_return input_class_descr;

class_return * class_set_input(int , ...)
{

	memset(input_funcs, 0, sizeof(CLASSFUNC) * 1024);

	input_funcs[0] = (CLASSFUNC)input_func0;
	input_funcs[1] = (CLASSFUNC)input_func1;
	input_funcs[64] = (CLASSFUNC)input_func64;
	input_funcs[65] = (CLASSFUNC)input_func65;
	input_funcs[66] = (CLASSFUNC)input_func66;

	input_class_descr.parent = "nucleus.class";

	input_class_descr.vtbl = input_funcs;
	////input_class_descr.varSize = sizeof(__NC_STACK_input);
	input_class_descr.varSize = sizeof(NC_STACK_input) - offsetof(NC_STACK_input, stack__input); //// HACK
	input_class_descr.field_A = 0;
	return &input_class_descr;
}
