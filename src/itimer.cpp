#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "itimer.h"
#include "utils.h"


stored_functions *classvtbl_get_itimer();
class_return * class_set_itimer(int, ...);

stored_functions itimer_class_vtbl(class_set_itimer);


class_stored itimer_class_off (NULL, NULL, "MC2classes:itimer.class", classvtbl_get_itimer);


stored_functions *classvtbl_get_itimer()
{
	return &itimer_class_vtbl;
}

CLASSFUNC itimer_funcs[1024];

class_return itimer_class_descr;

class_return * class_set_itimer(int a1, ...)
{

	memset(itimer_funcs, 0, sizeof(CLASSFUNC) * 1024);

	itimer_class_descr.parent = "nucleus.class";

	itimer_class_descr.vtbl = itimer_funcs;
	////itimer_class_descr.varSize = sizeof(__NC_STACK_itimer);
	itimer_class_descr.varSize = sizeof(NC_STACK_itimer) - offsetof(NC_STACK_itimer, stack__itimer); //// HACK
	itimer_class_descr.field_A = 0;
	return &itimer_class_descr;
}
