#include "includes.h"
#include "nucleas.h"
#include "sample.h"
#include "utils.h"


stored_functions *classvtbl_get_sample();
class_return * class_set_sample(int, ...);

stored_functions sample_class_vtbl(class_set_sample);


class_stored sample_class_off (NULL, NULL, "MC2classes:sample.class", classvtbl_get_sample);


stored_functions *classvtbl_get_sample()
{
	return &sample_class_vtbl;
}

CLASSFUNC sample_funcs[1024];

NC_STACK_sample * sample_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
	NC_STACK_sample *obj = (NC_STACK_sample *)call_parent(zis, clss, 0, stak);
	__NC_STACK_sample *smpl = &obj->stack__sample;

	if ( obj )
		call_vtbl(obj, 3, 0x80001002, &smpl->p_sampl, 0);

	return obj;
}

void sample_func3(NC_STACK_sample *obj, class_stru *zis, stack_vals *stak)
{

	__NC_STACK_sample *sample = &obj->stack__sample;

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

			case 0x80002000:
				*(sampl **)stk->value = sample->p_sampl;
				break;
			case 0x80002001:
				if ( sample->p_sampl )
					*(int *)stk->value = sample->p_sampl->field_8;
				else
					*(int *)stk->value = 0;
				break;
			case 0x80002002:
				if ( sample->p_sampl )
					*(int *)stk->value = sample->p_sampl->bufsz;
				else
					*(int *)stk->value = 0;
				break;
			case 0x80002003:
				if ( sample->p_sampl )
					*(void **)stk->value = sample->p_sampl->sample_buffer;
				else
					*(void **)stk->value = NULL;
				break;
			}
			stk++;
		}
	}

	call_parent(zis, obj, 3, stak);
}


rsrc * sample_func64(NC_STACK_sample *obj, class_stru *zis, stack_vals *stak)
{
	rsrc *res = (rsrc *)call_parent(zis, obj, 64, stak);

	if ( !res )
		return NULL;

	int bufsz = find_id_in_stack_def_val(0x80002002, 0, stak);
	int type = find_id_in_stack_def_val(0x80002001, 0xFFFF, stak);

	if ( bufsz == 0 || type == 0xFFFF )
		return res;

	sampl *smpl = (sampl *)AllocVec(sizeof(sampl), 65537);

	if ( !smpl )
		return res;

	smpl->bufsz = bufsz;
	smpl->field_8 = type;

	void *buf = (void *)find_id_in_stack_def_val(0x80002003, 0, stak);

	if ( !buf )
	{
		buf = AllocVec(bufsz, 65537);
		smpl->sample_buffer = buf;

		if ( !buf )
		{
			nc_FreeMem(smpl);
			return res;
		}
	}
	else
	{
		smpl->sample_buffer = buf;
		smpl->field_10 |= 1;
	}

	res->data = smpl;

	return res;
}

void sample_func65(NC_STACK_sample *obj, class_stru *zis, rsrc **pres)
{
	rsrc *res = *pres;
	sampl *smpl = (sampl *)res->data;

	if ( smpl )
	{
		if ( !(smpl->field_10 & 1) )
		{
			if ( smpl->sample_buffer )
				nc_FreeMem(smpl->sample_buffer);
		}
		nc_FreeMem(smpl);
		res->data = NULL;
	}

	call_parent(zis, obj, 65, (stack_vals *)pres);
}

void *sample_func128(NC_STACK_sample *obj, class_stru *, void **arg)
{
	printf("%s - NOT RECOGINZED ARGUMENT\n","sample_func128");
	sampl *smpl = obj->stack__sample.p_sampl;
	arg[2] = smpl;
	return smpl;
}

class_return sample_class_descr;

class_return * class_set_sample(int , ...)
{

	memset(sample_funcs, 0, sizeof(CLASSFUNC) * 1024);

	sample_funcs[0] = (CLASSFUNC)sample_func0;
	sample_funcs[3] = (CLASSFUNC)sample_func3;
	sample_funcs[64] = (CLASSFUNC)sample_func64;
	sample_funcs[65] = (CLASSFUNC)sample_func65;
	sample_funcs[128] = (CLASSFUNC)sample_func128;

	sample_class_descr.parent = "rsrc.class";

	sample_class_descr.vtbl = sample_funcs;
	////sample_class_descr.varSize = sizeof(__NC_STACK_sample);
	sample_class_descr.varSize = sizeof(NC_STACK_sample) - offsetof(NC_STACK_sample, stack__sample); //// HACK
	sample_class_descr.field_A = 0;
	return &sample_class_descr;
}
