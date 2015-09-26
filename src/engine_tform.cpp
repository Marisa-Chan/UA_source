#include "includes.h"
#include "engine_tform.h"

stored_functions *classvtbl_get_tformEngine();

int tformEngine__init(unsigned int, ...);
void tformEngine__deinit();
void tformEngine__setter(unsigned int, ...);
void tformEngine__getter(unsigned int, ...);


stored_functions_engine tform_engine_vtbl = {tformEngine__init, tformEngine__deinit, tformEngine__setter, tformEngine__getter};



class_stored tform_engine_off (NULL, NULL, "MC2engines:tform_NG.engine", classvtbl_get_tformEngine);




stored_functions *classvtbl_get_tformEngine()
{
	return (stored_functions *)&tform_engine_vtbl;
}

int tformEngine__init(unsigned int, ...)
{
////TODO
    return 1;
}

void tformEngine__deinit()
{
////TODO
}

void tformEngine__setter(unsigned int, ...)
{
////TODO
}

void tformEngine__getter(unsigned int, ...)
{
////TODO
}
