#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"


stored_functions *classvtbl_get_ade();
class_return * class_set_ade(int, ...);

stored_functions ade_class_vtbl(class_set_ade);


class_stored ade_class_off (NULL, NULL, "MC2classes:ade.class", classvtbl_get_ade);


stored_functions *classvtbl_get_ade()
{
	return &ade_class_vtbl;
}

CLASSFUNC ade_funcs[1024];

void ade_func0__sub0(__NC_STACK_ade *ade, stack_vals *stak)
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

			case 0x80001003:
				if ( stk->value )
					ade->flags |= 4;
				else
					ade->flags &= 0xFB;
				break;
			case 0x80001004:
				if ( stk->value )
					ade->flags |= 2;
				else
					ade->flags &= 0xFD;
				break;
			case 0x80001007:
				ade->strc_f4 = stk->value;
				break;
			case 0x80001008:
				ade->strc_f6 = stk->value;
				break;
			}
			stk++;
		}
	}
}


NC_STACK_ade * ade_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
	NC_STACK_ade *clss = (NC_STACK_ade *)call_parent(zis, obj, 0, stak); // rsrc_func0
	if (clss)
	{
		__NC_STACK_ade *ade = &clss->stack__ade;

		ade->self = clss;
		ade_func0__sub0(ade, stak);
	}

	return clss;
}

size_t ade_func1(NC_STACK_ade *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_ade *ade = &obj->stack__ade;

	if ( ade->flags & 1 )
		Remove(ade);

	return call_parent(zis, obj, 1, stak);
}


void ade_func2__sub0(__NC_STACK_ade *ade, stack_vals *stak)
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

			case 0x80001003:
				if ( stk->value )
					ade->flags |= 4;
				else
					ade->flags &= 0xFB;
				break;
			case 0x80001004:
				if ( stk->value )
					ade->flags |= 2;
				else
					ade->flags &= 0xFD;
				break;
			case 0x80001007:
				ade->strc_f4 = stk->value;
				break;
			case 0x80001008:
				ade->strc_f6 = stk->value;
				break;
			}
			stk++;
		}
	}
}

void ade_func2(NC_STACK_ade *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_ade *ade = &obj->stack__ade;

	ade_func2__sub0(ade, stak);

	call_parent(zis, obj, 2, stak);
}

void ade_func3__sub0(__NC_STACK_ade *ade, stack_vals *stak)
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

			case 0x80001003:
				if ( !(ade->flags & 4) )
					*(int *)stk->value = 0;
				else
					*(int *)stk->value = 1;
				break;
			case 0x80001004:
				if ( ade->flags & 2 )
					*(int *)stk->value = 1;
				else
					*(int *)stk->value = 0;
				break;
			case 0x80001007:
				*(int *)stk->value = ade->strc_f4;
				break;
			case 0x80001008:
				*(int *)stk->value = ade->strc_f6;
				break;
			case 0x8000100B:
				*(__NC_STACK_ade **)stk->value = ade;
				break;
			}
			stk++;
		}
	}
}

void ade_func3(NC_STACK_ade *obj, class_stru *zis, stack_vals *stak)
{

	__NC_STACK_ade *ade = &obj->stack__ade;

	ade_func3__sub0(ade, stak);

	call_parent(zis, obj, 3, stak);
}


NC_STACK_ade *ade_func5(class_stru *obj, class_stru *zis, MFILE **file)
{
	MFILE *mfile = *file;
	NC_STACK_ade *clss = NULL;
	while ( 1 )
	{
		int iff_res = read_next_IFF(mfile, 2);

		if ( iff_res == -2 )
			break;

		if ( iff_res )
		{
			if ( clss )
				call_method(clss, 1);
			return NULL;
		}

		MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

		if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
		{
			clss = (NC_STACK_ade *)call_parent(zis, obj, 5, (stack_vals *)file);

			if ( !clss )
				break;

			clss->stack__ade.self = clss;

		}
		else if ( chunk->TAG == TAG_STRC )
		{
			if ( clss )
			{
				ADE_STRC hdr;

				mfread(mfile, &hdr, sizeof(ADE_STRC));

				hdr.field_0 = SWAP16(hdr.field_0);
				hdr.field_4 = SWAP16(hdr.field_4);
				hdr.field_6 = SWAP16(hdr.field_6);

				if ( hdr.field_0 >= 1 )
				{
					stack_vals stk[5];

					stk[0].id = 0x80001004;
					stk[0].value = (hdr.field_3 & 2) != 0;
					stk[1].id = 0x80001003;
					stk[1].value = (hdr.field_3 & 4) != 0;
					stk[2].id = 0x80001007;
					stk[2].value = hdr.field_4;
					stk[3].id = 0x80001008;
					stk[3].value = hdr.field_6;
					stk[4].id = 0;

					call_method(clss, 2, stk);
				}
			}
			read_next_IFF(mfile, 2);
		}
		else
		{
			read_default(mfile);
		}
	}

	return clss;
}

int ade_func6(NC_STACK_ade *obj, class_stru *zis, MFILE **file)
{
	MFILE *mfile = *file;
	__NC_STACK_ade *ade = &obj->stack__ade;

	if ( sub_412FC0(mfile, TAG_ADE, TAG_FORM, -1) )
		return 0;

	if ( !call_parent(zis, obj, 6, (stack_vals *)file) )
		return 0;

	sub_412FC0(mfile, 0, TAG_STRC, -1);

	ADE_STRC hdr;
	hdr.field_0 = SWAP16(1);
	hdr.field_2 = 0;
	hdr.field_3 = ade->flags & 6;
	hdr.field_4 = SWAP16(ade->strc_f4);
	hdr.field_6 = SWAP16(ade->strc_f6);
	hdr.field_8 = 0;

	sub_413564(mfile, sizeof(ADE_STRC), &hdr);
	sub_413290(mfile);
	return sub_413290(mfile) == 0;
}

// Add ade to list
void ade_func64(NC_STACK_ade *obj, class_stru *zis, nlist **lst)
{
	__NC_STACK_ade *ade = &obj->stack__ade;

	if ( ade->flags & 1 )
		Remove(ade);

	AddTail(*lst, ade);
	ade->flags |= 1;
}

class_return ade_class_descr;

class_return * class_set_ade(int a1, ...)
{

	memset(ade_funcs, 0, sizeof(CLASSFUNC) * 1024);

	ade_funcs[0] = (CLASSFUNC)ade_func0;
	ade_funcs[1] = (CLASSFUNC)ade_func1;
	ade_funcs[2] = (CLASSFUNC)ade_func2;
	ade_funcs[3] = (CLASSFUNC)ade_func3;
	ade_funcs[5] = (CLASSFUNC)ade_func5;
	ade_funcs[6] = (CLASSFUNC)ade_func6;
	ade_funcs[64] = (CLASSFUNC)ade_func64;

	ade_class_descr.parent = "nucleus.class";

	ade_class_descr.vtbl = ade_funcs;
	////ade_class_descr.varSize = sizeof(__NC_STACK_ade);
	ade_class_descr.varSize = sizeof(NC_STACK_ade) - offsetof(NC_STACK_ade, stack__ade); //// HACK
	ade_class_descr.field_A = 0;
	return &ade_class_descr;
}
