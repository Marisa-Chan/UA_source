#include "includes.h"
#include "nucleas.h"
#include "utils.h"


stored_functions *classvtbl_get_nucleus();
class_return * class_set_nucleus(int, ...);

stored_functions nucleas_class_vtbl(class_set_nucleus);



class_stored nucleas_class_off (NULL, NULL, "MC2classes:nucleus.class", classvtbl_get_nucleus);


stored_functions *classvtbl_get_nucleus()
{
	return &nucleas_class_vtbl;
}

CLASSFUNC nucleas_funcs[1024];


size_t nucleus_func_default(void *, class_stru *, stack_vals *)
{
	return 0;
}

void nucleus_func0__sub0(NC_STACK_nucleus *a1, stack_vals *a2)
{
	while ( 1 )
	{
		if ( a2->id == 0)
			break;

		if ( a2->id == 2 )
		{
			a2 = (stack_vals *)a2->value;
		}
		else if ( a2->id == 3 )
		{
			a2 += a2->value;
			////a2++; ////BUGFIX?
		}
		else
		{
			if ( a2->id == 0x80000000 )
			{
				if ( a2->value )
				{
					a1->NAME = (char *)AllocVec(33, 1);
					if ( a1->NAME )
						strncpy(a1->NAME, (const char *)a2->value, 32);
				}
			}
			a2++;
		}
	}
}



NC_STACK_nucleus * nucleus_func0(class_stru *caller, class_stru *, stack_vals *stack)
{
	int clsSize;

	if ( caller->parent_class )
		clsSize = caller->stack_offset + caller->class_stack_size;
	else
		clsSize = caller->class_stack_size;

	NC_STACK_nucleus *clss = (NC_STACK_nucleus *)AllocVec(clsSize, 65537);
	if ( clss )
	{
		caller->class_inst_count++;

		clss->class_owner = caller;

		nucleus_func0__sub0(clss, stack);
	}
	return clss;
}

size_t nucleus_func1(NC_STACK_nucleus *a1, class_stru *, stack_vals *)
{
	a1->class_owner->class_inst_count--;
	if ( a1->NAME )
		nc_FreeMem(a1->NAME);
	nc_FreeMem(a1);
	return 1;
}

void nucleus_setter(NC_STACK_nucleus *a1, stack_vals *a2)
{
	while ( 1 )
	{
		if ( a2->id == 0)
			break;

		if ( a2->id == 2 )
		{
			a2 = (stack_vals *)a2->value;
		}
		else if ( a2->id == 3 )
		{
			a2 += a2->value;
			////a2++; ////BUGFIX?
		}
		else
		{
			if ( a2->id == 0x80000000 )
			{
				if ( a2->value )
				{
					if (a1->NAME == NULL)
					{
						a1->NAME = (char *)AllocVec(33, 1);
						if ( a1->NAME )
							strncpy(a1->NAME, (const char *)a2->value, 32);
					}
					else
						strncpy(a1->NAME, (const char *)a2->value, 32);

				}
				else if (a1->NAME != NULL )
				{
					nc_FreeMem(a1->NAME);
					a1->NAME = NULL;
				}
			}
			a2++;
		}
	}
}

size_t nucleus_func2(NC_STACK_nucleus *a1, class_stru *, stack_vals *a3)
{
	nucleus_setter(a1, a3);

	return 1;
}


void nucleus_getter(NC_STACK_nucleus *a1, stack_vals *a2)
{
	while ( 1 )
	{
		if ( a2->id == 0)
			break;

		if ( a2->id == 2 )
		{
			a2 = (stack_vals *)a2->value;
		}
		else if ( a2->id == 3 )
		{
			a2 += a2->value;
			////a2++; ////BUGFIX?
		}
		else
		{
			if ( a2->id == 0x80000000 )
				*(char **)a2->value = a1->NAME;
			else if ( a2->id == 0x80000001 )
				*(class_stru **)a2->value = a1->class_owner;
			else if ( a2->id == 0x80000002 )
				*(const char **)a2->value = a1->class_owner->name;
            a2++;
		};
	}
}


size_t nucleus_func3(NC_STACK_nucleus *caller, class_stru *, stack_vals *stk)
{
	nucleus_getter(caller, stk);

	return 1;
}

NC_STACK_nucleus *nucleus_func5(class_stru *caller, class_stru *, MFILE **file)
{
	MFILE *mfile = *file;
	NC_STACK_nucleus *new_class = (NC_STACK_nucleus *)AllocVec(caller->stack_offset + caller->class_stack_size, 65537);
	if ( new_class )
	{
		caller->class_inst_count++;
		new_class->class_owner = caller;
		while ( 1 )
		{
			signed int v6 = read_next_IFF(mfile, 2);
			if ( v6 == -2 )
				break;
			if ( v6 )
			{
				call_method(new_class, 1, 0);
				return 0;
			}
			if ( GET_FORM_INFO_OR_NULL(mfile)->TAG == TAG_NAME )
			{
				char a4[33];
				memset(a4, 0, 33);

				if ( !mfread(mfile, a4, 32) )
				{
					call_method(new_class, 1, 0);
					return 0;
				}

				call_vtbl(new_class, 2, 0x80000000, a4, 0,0);  // nucleus::SetName(new_name)
				read_next_IFF(mfile, 2);
			}
			else
			{
				read_default(mfile);
			}
		}
	}
	return new_class;
}


size_t nucleus_func6(NC_STACK_nucleus *caller, class_stru *, MFILE **val)
{
	MFILE *mfile = *val;
	if ( sub_412FC0(mfile, TAG_ROOT, TAG_FORM, -1) )
        return 0;
	else
	{
		if ( caller->NAME )
		{
			sub_412FC0(mfile, 0, TAG_NAME, -1);
			sub_413564(mfile, strlen(caller->NAME), caller->NAME);
			sub_413290(mfile);
		}
		sub_413290(mfile);
		return 1;
	}
	return 0;
}



class_return nucleas_class_descr;


class_return * class_set_nucleus(int, ...)
{
	for (int i = 0; i < 1024; i++)
		nucleas_funcs[i] = &nucleus_func_default;

	nucleas_funcs[0] = (CLASSFUNC)nucleus_func0;
	nucleas_funcs[1] = (CLASSFUNC)nucleus_func1;
	nucleas_funcs[2] = (CLASSFUNC)nucleus_func2;
	nucleas_funcs[3] = (CLASSFUNC)nucleus_func3;
	nucleas_funcs[5] = (CLASSFUNC)nucleus_func5;
	nucleas_funcs[6] = (CLASSFUNC)nucleus_func6;


	nucleas_class_descr.parent = 0;
	nucleas_class_descr.vtbl = nucleas_funcs;
	nucleas_class_descr.varSize = sizeof(NC_STACK_nucleus);
	nucleas_class_descr.field_A = 0;
	return &nucleas_class_descr;
}
