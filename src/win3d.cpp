#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "utils.h"


stored_functions *classvtbl_get_win3d();
class_return * class_set_win3d(int, ...);

stored_functions win3d_class_vtbl(class_set_win3d);


class_stored win3d_class_off (NULL, NULL, "MC2classes:drivers/gfx/win3d.class", classvtbl_get_win3d);


stored_functions *classvtbl_get_win3d()
{
	return &win3d_class_vtbl;
}

CLASSFUNC win3d_funcs[1024];


NC_STACK_win3d * win3d_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
	NC_STACK_win3d *clss = (NC_STACK_win3d *)call_parent(zis, obj, 0, stak); // rsrc_func0

	if (clss)
	{
	}

	return clss;
}

size_t win3d_func1(NC_STACK_win3d *obj, class_stru *zis, stack_vals *stak)
{
	return call_parent(zis, obj, 1, stak);
}

void win3d_func2(NC_STACK_win3d *obj, class_stru *zis, stack_vals *stak)
{
	call_parent(zis, obj, 2, stak);
}

size_t win3d_func198(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func199(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func200(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func201(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func202(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func204(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func206(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func209(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func213(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func214(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func215(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func216(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func218(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func262(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func266(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func267(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func268(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func269(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func270(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func271(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func272(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t win3d_func274(void *, class_stru *, stack_vals *)
{
	return 0;
}


class_return win3d_class_descr;

void win3d_nullsub(void *, class_stru *, stack_vals *)
{
	;
}

class_return * class_set_win3d(int, ...)
{
	memset(win3d_funcs, 0, sizeof(CLASSFUNC) * 1024);

	win3d_funcs[0] = (CLASSFUNC)win3d_func0;
	win3d_funcs[1] = (CLASSFUNC)win3d_func1;
	win3d_funcs[2] = (CLASSFUNC)win3d_func2;

	win3d_funcs[198] = (CLASSFUNC)win3d_func198;
	win3d_funcs[199] = (CLASSFUNC)win3d_func199;
	win3d_funcs[200] = (CLASSFUNC)win3d_func200;
	win3d_funcs[201] = (CLASSFUNC)win3d_func201;
	win3d_funcs[202] = (CLASSFUNC)win3d_func202;

	win3d_funcs[204] = (CLASSFUNC)win3d_func204;

	win3d_funcs[206] = (CLASSFUNC)win3d_func206;

	win3d_funcs[209] = (CLASSFUNC)win3d_func209;

	win3d_funcs[213] = (CLASSFUNC)win3d_func213;
	win3d_funcs[214] = (CLASSFUNC)win3d_func214;
	win3d_funcs[215] = (CLASSFUNC)win3d_func215;
	win3d_funcs[216] = (CLASSFUNC)win3d_func216;

	win3d_funcs[218] = (CLASSFUNC)win3d_func218;

	win3d_funcs[262] = (CLASSFUNC)win3d_func262;

	win3d_funcs[266] = (CLASSFUNC)win3d_func266;
	win3d_funcs[267] = (CLASSFUNC)win3d_func267;
	win3d_funcs[268] = (CLASSFUNC)win3d_func268;
	win3d_funcs[269] = (CLASSFUNC)win3d_func269;
	win3d_funcs[270] = (CLASSFUNC)win3d_func270;
	win3d_funcs[271] = (CLASSFUNC)win3d_func271;
	win3d_funcs[272] = (CLASSFUNC)win3d_func272;

	win3d_funcs[274] = (CLASSFUNC)win3d_func274;

	win3d_class_descr.parent = "drivers/gfx/windd.class";

	win3d_class_descr.vtbl = win3d_funcs;
	////win3d_class_descr.varSize = sizeof(__NC_STACK_win3d);
	win3d_class_descr.varSize = sizeof(NC_STACK_win3d) - offsetof(NC_STACK_win3d, stack__win3d); //// HACK
	win3d_class_descr.field_A = 0;
	return &win3d_class_descr;
}
