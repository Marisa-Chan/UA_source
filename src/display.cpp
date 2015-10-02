#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "utils.h"


stored_functions *classvtbl_get_display();
class_return * class_set_display(int, ...);

stored_functions display_class_vtbl(class_set_display);


class_stored display_class_off (NULL, NULL, "MC2classes:display.class", classvtbl_get_display);


stored_functions *classvtbl_get_display()
{
	return &display_class_vtbl;
}

CLASSFUNC display_funcs[1024];
g_engines *engines___display;


NC_STACK_display * display_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
	NC_STACK_display *clss = (NC_STACK_display *)call_parent(zis, obj, 0, stak); // raster_func0

	if (clss)
	{
	}

	return clss;
}

size_t display_func1(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
{
	return call_parent(zis, obj, 1, stak);
}

void display_func2(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
{
	call_parent(zis, obj, 2, stak);
}

void display_func3(NC_STACK_display *obj, class_stru *zis, stack_vals *stak)
{

	//__NC_STACK_display *internal = &obj->stack__display;

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


size_t display_func261(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func262(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func263(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func264(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func265(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func266(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func267(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func268(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func269(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func270(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func273(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t display_func274(void *, class_stru *, stack_vals *)
{
	return 0;
}


class_return display_class_descr;

void display_nullsub(void *, class_stru *, stack_vals *)
{
	;
}

class_return * class_set_display(int a1, ...)
{

	stack_vals vals[128];

	if (a1 != 0)
	{
		va_list va;
		va_start(va, a1);

		va_to_arr(vals, 128, a1, va);

		va_end(va);
	}

	engines___display = (g_engines *)find_id_in_stack_def_val(0x80000002, 0, vals);

	memset(display_funcs, 0, sizeof(CLASSFUNC) * 1024);

	display_funcs[0] = (CLASSFUNC)display_func0;
	display_funcs[1] = (CLASSFUNC)display_func1;
	display_funcs[2] = (CLASSFUNC)display_func2;
	display_funcs[3] = (CLASSFUNC)display_func3;
	display_funcs[261] = (CLASSFUNC)display_func261;
	display_funcs[262] = (CLASSFUNC)display_func262;
	display_funcs[263] = (CLASSFUNC)display_func263;
	display_funcs[264] = (CLASSFUNC)display_func264;
	display_funcs[265] = (CLASSFUNC)display_func265;
	display_funcs[266] = (CLASSFUNC)display_func266;
	display_funcs[267] = (CLASSFUNC)display_func267;
	display_funcs[268] = (CLASSFUNC)display_func268;
	display_funcs[269] = (CLASSFUNC)display_func269;
	display_funcs[270] = (CLASSFUNC)display_func270;
	display_funcs[273] = (CLASSFUNC)display_func273;
	display_funcs[274] = (CLASSFUNC)display_func274;

	display_class_descr.parent = "raster.class";

	display_class_descr.vtbl = display_funcs;
	////display_class_descr.varSize = sizeof(__NC_STACK_display);
	display_class_descr.varSize = sizeof(NC_STACK_display) - offsetof(NC_STACK_display, stack__display); //// HACK
	display_class_descr.field_A = 0;
	return &display_class_descr;
}
