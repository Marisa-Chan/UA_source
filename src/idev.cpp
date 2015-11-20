#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "idev.h"
#include "utils.h"


stored_functions *classvtbl_get_idev();
class_return * class_set_idev(int, ...);

stored_functions idev_class_vtbl(class_set_idev);


class_stored idev_class_off (NULL, NULL, "MC2classes:idev.class", classvtbl_get_idev);


stored_functions *classvtbl_get_idev()
{
	return &idev_class_vtbl;
}

CLASSFUNC idev_funcs[1024];

void idev_func69(void *, void *, void *)
{

}

class_return idev_class_descr;

class_return * class_set_idev(int a1, ...)
{

	memset(idev_funcs, 0, sizeof(CLASSFUNC) * 1024);

	idev_funcs[69] = (CLASSFUNC)idev_func69;

	idev_class_descr.parent = "nucleus.class";

	idev_class_descr.vtbl = idev_funcs;
	////idev_class_descr.varSize = sizeof(__NC_STACK_idev);
	idev_class_descr.varSize = sizeof(NC_STACK_idev) - offsetof(NC_STACK_idev, stack__idev); //// HACK
	idev_class_descr.field_A = 0;
	return &idev_class_descr;
}
