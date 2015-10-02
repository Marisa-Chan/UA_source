#include "includes.h"
#include "classes.h"
#include "utils.h"

int class_def_deinit()
{
	return 1;
}

class_stored * FIND(nlist *list, const char *name)
{
	class_stored *current = (class_stored *)list->head;
	if ( current->next )
	{
		while ( strcasecmp(name, current->name) )
		{
			current = (class_stored *)current->next;
			if ( !current->next )
				return NULL;
		}
	}
	else
		return NULL;

	return current;
}

stored_functions * find_class(const char *name, unk_class *out)
{
	class_stored *cls = FIND(&classes_list, name);
	if ( cls )
	{
		stored_functions *funcs = cls->get_stored_functions();
		if ( funcs )
		{
			out->func = funcs;
			out->node = cls;
			return funcs;
		}
	}
	return NULL;
}

size_t call_method(NC_STACK_class *a1, int a2, void *a3)
{
	if ( a1 )
		return a1->class_owner->clvtbl[a2].cl_func(a1, a1->class_owner->clvtbl[a2].p_cl, (stack_vals *)a3);

	ypa_log_out("ERROR: Method invocation on NULL Object!\n");
	return 0;
}

size_t call_method(NC_STACK_class *a1, int a2)
{
	if ( a1 )
		return a1->class_owner->clvtbl[a2].cl_func(a1, a1->class_owner->clvtbl[a2].p_cl, NULL);

	ypa_log_out("ERROR: Method invocation on NULL Object!\n");
	return 0;
}

size_t call_vtbl(NC_STACK_class *a1, int idx, ...)
{
	stack_vals vals[128];

	va_list va;

	va_start(va, idx);
	va_to_arr(vals, 128, va);
	va_end(va);

	if ( a1 )
	{
		clvt *v3 = &a1->class_owner->clvtbl[idx];
		return v3->cl_func(a1, v3->p_cl, vals);
	}
	else
	{
		ypa_log_out("ERROR: Method invocation on NULL Object!\n");
		return 0;
	}
	return 0;
}


int sub_411E90(class_stru *clss)
{
	if ( clss->ref_count )
	{
		clss->ref_count--;
	}
	else
	{
		if ( clss->ref_cnt || clss->class_inst_count )
			return 0;

		if ( clss->parent_class )
		{
			clss->parent_class->ref_cnt--;
			sub_411E90(clss->parent_class);
		}

		clss->class_descriptor_in_list.func->deinit();

		Remove(clss);

		if ( clss->name )
			nc_FreeMem(clss->name);

		if ( clss->clvtbl )
			nc_FreeMem(clss->clvtbl);

		nc_FreeMem(clss);
	}
	return 1;
}


class_stru * get_class(const char *a1)
{
	class_stru *class_descr = (class_stru *)FIND(&engines.allocated_class_list, a1);

	if ( class_descr )
	{
		class_descr->ref_count++;
	}
	else
	{
		class_descr = (class_stru *)AllocVec(sizeof(class_stru), 65537);

		if ( !class_descr )
			return NULL;

		clvt *clvtbl = (clvt *)AllocVec(sizeof(clvt) * 1024, 65537);

		if ( !clvtbl )
		{
			nc_FreeMem(class_descr);
			return NULL;
		}

		class_descr->clvtbl = clvtbl;

		char *classname = (char *)AllocVec(strlen(a1) + 1, 65537);

		if ( !classname )
		{
			nc_FreeMem(clvtbl);
			nc_FreeMem(class_descr);
			return NULL;
		}

		strcpy(classname, a1);


		class_descr->name = classname;


		char buf[256];

		strcpy(buf, "MC2classes:");
		strcat(buf, classname);

		stored_functions *class_initiator = find_class(buf, &class_descr->class_descriptor_in_list);

		if ( !class_initiator )
		{
			nc_FreeMem(classname);
			nc_FreeMem(clvtbl);
			nc_FreeMem(class_descr);
			return NULL;
		}

		class_return *class_ret = class_initiator->init(0x80000101, engines.setted_gfx_engine, 0x80000102, engines.setted_audio_engine, 0x80000301, engines.setted_input_engine, 0x80000401, engines.setted_tform_engine, 0x80000001, &stru_51363C, 0x80000002, &engines, 0);

		if ( !class_ret )
		{
			nc_FreeMem(classname);
			nc_FreeMem(clvtbl);
			nc_FreeMem(class_descr);
			return NULL;
		}

		class_descr->class_stack_size = class_ret->varSize;
		class_descr->clret_fieldA = class_ret->field_A;

		for (int i = 0; i < 1024; i++)
		{
			clvtbl[i].cl_func = class_ret->vtbl[i];
			clvtbl[i].p_cl = class_descr;
		}

		AddTail(&engines.allocated_class_list, class_descr);


		if ( class_ret->parent )
		{
			class_stru * parent_class = get_class(class_ret->parent);

			class_descr->parent_class = parent_class;

			if ( !parent_class )
			{
				sub_411E90(class_descr);
				return 0;
			}

			parent_class->ref_cnt++;

			class_descr->stack_offset = parent_class->class_stack_size + parent_class->stack_offset;

			for (int i = 0; i < 1024; i++)
			{
				if (clvtbl[i].cl_func == NULL)
				{
					clvtbl[i].cl_func = parent_class->clvtbl[i].cl_func;
					clvtbl[i].p_cl = parent_class->clvtbl[i].p_cl;
				}

			}
		}
	}
	return class_descr;
}


NC_STACK_class * init_get_class(const char *classname, ...)
{

	stack_vals vals[128];

	va_list va;

	va_start(va, classname);
	va_to_arr(vals, 128, va);
	va_end(va);

	class_stru * cls_descr = get_class(classname);

	if (!cls_descr)
		return NULL;

	//class object constructor
	NC_STACK_class *class_examplar = (NC_STACK_class *)cls_descr->clvtbl[0].cl_func(cls_descr, cls_descr->clvtbl[0].p_cl, vals);
	if (!class_examplar)
		sub_411E90(cls_descr);

	return class_examplar;
}


int delete_class_obj(NC_STACK_class *cls)
{
	class_stru *owner = cls->class_owner;

	int ret = call_method(cls, 1, NULL);

	if ( ret )
		sub_411E90(owner);

	return ret;
}


stack_vals * find_id_in_stack2(unsigned int id, stack_vals *a2)
{
	while ( 1 )
	{
		if ( a2->id == id )
			return a2;
		if ( a2->id == 0 )
			return NULL;

		if ( a2->id == 2 )
			a2 = (stack_vals *)a2->value;
		else if ( a2->id == 3 )
			a2 += a2->value;
		else
			a2++;
	}
}

size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, stack_vals *a3)
{
	while ( 1 )
	{
		if ( a3->id == find_id )
			return a3->value;
		if ( a3->id == 0 )
			return def_value;

		if ( a3->id == 2 )
			a3 = (stack_vals *)a3->value;
		else if ( a3->id == 3 )
			a3 += a3->value;
		else
			a3++;
	}
}

size_t call_parent(class_stru *zis, void *caller, int idx, stack_vals *stk)
{
	clvt *v4 = &zis->parent_class->clvtbl[idx];
	return v4->cl_func((NC_STACK_class *)caller, v4->p_cl, stk);
}
