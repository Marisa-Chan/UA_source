#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "utils.h"


stored_functions *classvtbl_get_raster();
class_return * class_set_raster(int, ...);

stored_functions raster_class_vtbl(class_set_raster);


class_stored raster_class_off (NULL, NULL, "MC2classes:raster.class", classvtbl_get_raster);


stored_functions *classvtbl_get_raster()
{
	return &raster_class_vtbl;
}

CLASSFUNC raster_funcs[1024];


NC_STACK_raster * raster_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
	NC_STACK_raster *clss = (NC_STACK_raster *)call_parent(zis, obj, 0, stak); // bitmap_func0
	printf("MAKE ME %s\n","raster_func0");
	if (clss)
	{
		__NC_STACK_raster *rstr = &clss->stack__raster;
		call_vtbl(clss, 3, 0x80001002, &rstr->bitm_intern, 0);

		rstr->field_24.x1 = 0;
		rstr->field_24.y1 = 0;
		rstr->field_24.x2 = rstr->bitm_intern->width - 1;
		rstr->field_24.y2 = rstr->bitm_intern->height - 1;

		rstr->field_54c = rstr->bitm_intern->width / 2;
		rstr->field_550 = rstr->bitm_intern->height / 2;

		rstr->field_554 = (rstr->bitm_intern->width / 2);
		rstr->field_558 = (rstr->bitm_intern->height / 2);
	}

	return clss;
}

size_t raster_func1(NC_STACK_raster *obj, class_stru *zis, stack_vals *stak)
{
	printf("MAKE ME %s\n","raster_func1");
	return call_parent(zis, obj, 1, stak);
}

void raster_func2(NC_STACK_raster *obj, class_stru *zis, stack_vals *stak)
{
	printf("MAKE ME %s\n","raster_func2");
	call_parent(zis, obj, 2, stak);
}

void raster_func3(NC_STACK_raster *obj, class_stru *zis, stack_vals *stak)
{
	printf("MAKE ME %s\n","raster_func3");
	//__NC_STACK_raster *internal = &obj->stack__raster;

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
			stk++;
		}
	}
	call_parent(zis, obj, 3, stak);
}


size_t raster_func192(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func192");
	return 0;
}

size_t raster_func193(NC_STACK_raster *obj, class_stru *, bitmap_intern **out)
{
	__NC_STACK_raster *rstr = &obj->stack__raster;
	bitmap_intern *bitm = rstr->bitm_intern;

	memcpy((*out)->buffer, bitm->buffer, bitm->width * bitm->height);
	return 0;
}

size_t raster_func198(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func198");
	return 0;
}

size_t raster_func199(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func199");
	return 0;
}

size_t raster_func200(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func200");
	return 0;
}

size_t raster_func201(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func201");
	return 0;
}

size_t raster_func202(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func202");
	return 0;
}

size_t raster_func203(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func203");
	return 0;
}

size_t raster_func204(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func204");
	return 0;
}

size_t raster_func205(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func205");
	return 0;
}

size_t raster_func206(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func206");
	return 0;
}

void raster_func207(NC_STACK_raster *obj, class_stru *, rstr_207_arg *arg)
{
	__NC_STACK_raster *rstr = &obj->stack__raster;
	rstr->tiles[arg->id] = arg->tiles;
}

void raster_func208(NC_STACK_raster *obj, class_stru *, rstr_207_arg *arg)
{
	__NC_STACK_raster *rstr = &obj->stack__raster;

	if ( arg->tiles )
	{
		arg->id = -1;

		for (int i = 0; i < 256; i++)
		{
			if (rstr->tiles[i] == arg->tiles)
			{
				arg->id = i;
				break;
			}
		}
	}
	else
	{
		arg->tiles = rstr->tiles[arg->id];
	}
}

size_t raster_func209(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func209");
	return 0;
}

size_t raster_func210(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func210");
	return 0;
}

size_t raster_func211(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func211");
	return 0;
}

size_t raster_func212(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func212");
	return 0;
}

size_t raster_func214(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func214");
	return 0;
}

size_t raster_func217(NC_STACK_raster *obj, class_stru *, rstr_arg217 *arg)
{
	__NC_STACK_raster *rstr = &obj->stack__raster;

	if ( arg->dword0 != 0xFFFFFFFF )
		rstr->field_4 = arg->dword0;

	if ( arg->dword4 != 0xFFFFFFFF )
		rstr->field_8 = arg->dword4;

	if ( arg->dword8 != 0xFFFFFFFF )
		rstr->field_c = arg->dword8;

	return 0;
}

size_t raster_func218(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func218");
	return 0;
}

size_t raster_func219(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func219");
	return 0;
}

size_t raster_func220(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func220");
	return 0;
}

size_t raster_func221(void *, class_stru *, stack_vals *)
{
	printf("MAKE ME %s\n","raster_func221");
	return 0;
}



class_return raster_class_descr;

void raster_nullsub(void *, class_stru *, stack_vals *)
{
	;
}

class_return * class_set_raster(int, ...)
{
	memset(raster_funcs, 0, sizeof(CLASSFUNC) * 1024);

	raster_funcs[0] = (CLASSFUNC)raster_func0;
	raster_funcs[1] = (CLASSFUNC)raster_func1;
	raster_funcs[2] = (CLASSFUNC)raster_func2;
	raster_funcs[3] = (CLASSFUNC)raster_func3;
	raster_funcs[192] = (CLASSFUNC)raster_func192;
	raster_funcs[193] = (CLASSFUNC)raster_func193;
	raster_funcs[198] = (CLASSFUNC)raster_func198;
	raster_funcs[199] = (CLASSFUNC)raster_func199;
	raster_funcs[200] = (CLASSFUNC)raster_func200;
	raster_funcs[201] = (CLASSFUNC)raster_func201;
	raster_funcs[202] = (CLASSFUNC)raster_func202;
	raster_funcs[203] = (CLASSFUNC)raster_func203;
	raster_funcs[204] = (CLASSFUNC)raster_func204;
	raster_funcs[205] = (CLASSFUNC)raster_func205;
	raster_funcs[206] = (CLASSFUNC)raster_func206;
	raster_funcs[207] = (CLASSFUNC)raster_func207;
	raster_funcs[208] = (CLASSFUNC)raster_func208;
	raster_funcs[209] = (CLASSFUNC)raster_func209;
	raster_funcs[210] = (CLASSFUNC)raster_func210;
	raster_funcs[211] = (CLASSFUNC)raster_func211;
	raster_funcs[212] = (CLASSFUNC)raster_func212;
	raster_funcs[213] = (CLASSFUNC)raster_nullsub;
	raster_funcs[214] = (CLASSFUNC)raster_func214;
	raster_funcs[215] = (CLASSFUNC)raster_nullsub;
	raster_funcs[216] = (CLASSFUNC)raster_nullsub;
	raster_funcs[217] = (CLASSFUNC)raster_func217;
	raster_funcs[218] = (CLASSFUNC)raster_func218;
	raster_funcs[219] = (CLASSFUNC)raster_func219;
	raster_funcs[220] = (CLASSFUNC)raster_func220;
	raster_funcs[221] = (CLASSFUNC)raster_func221;

	raster_class_descr.parent = "bitmap.class";

	raster_class_descr.vtbl = raster_funcs;
	////raster_class_descr.varSize = sizeof(__NC_STACK_raster);
	raster_class_descr.varSize = sizeof(NC_STACK_raster) - offsetof(NC_STACK_raster, stack__raster); //// HACK
	raster_class_descr.field_A = 0;
	return &raster_class_descr;
}
