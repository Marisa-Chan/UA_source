#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "button.h"
#include "utils.h"


stored_functions *classvtbl_get_button();
class_return * class_set_button(int, ...);

stored_functions button_class_vtbl(class_set_button);


class_stored button_class_off (NULL, NULL, "MC2classes:button.class", classvtbl_get_button);


stored_functions *classvtbl_get_button()
{
	return &button_class_vtbl;
}

CLASSFUNC button_funcs[1024];

class_return button_class_descr;

class_return * class_set_button(int a1, ...)
{

	memset(button_funcs, 0, sizeof(CLASSFUNC) * 1024);

//    button_funcs[0] = (CLASSFUNC)button_func0;
//    button_funcs[1] = (CLASSFUNC)button_func1;
//    button_funcs[2] = (CLASSFUNC)button_func2;
//    button_funcs[3] = (CLASSFUNC)button_func3;
//	button_funcs[64] = (CLASSFUNC)button_func64;
//	button_funcs[65] = (CLASSFUNC)button_func65;
//	button_funcs[66] = (CLASSFUNC)button_func66;
//	button_funcs[67] = (CLASSFUNC)button_func67;
//	button_funcs[68] = (CLASSFUNC)button_func68;
//	button_funcs[69] = (CLASSFUNC)button_func69;
//	button_funcs[70] = (CLASSFUNC)button_func70;
//	button_funcs[71] = (CLASSFUNC)button_func71;
//	button_funcs[72] = (CLASSFUNC)button_func72;
//	button_funcs[73] = (CLASSFUNC)button_func73;
//	button_funcs[74] = (CLASSFUNC)button_func74;
//	button_funcs[75] = (CLASSFUNC)button_func75;
//	button_funcs[76] = (CLASSFUNC)button_func76;

	button_class_descr.parent = "nucleus.class";

	button_class_descr.vtbl = button_funcs;
	////button_class_descr.varSize = sizeof(__NC_STACK_button);
	button_class_descr.varSize = sizeof(NC_STACK_button) - offsetof(NC_STACK_button, stack__button); //// HACK
	button_class_descr.field_A = 0;
	return &button_class_descr;
}
