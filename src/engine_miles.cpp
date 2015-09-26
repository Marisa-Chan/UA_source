#include "includes.h"
#include "engine_miles.h"

stored_functions *classvtbl_get_milesEngine();

int milesEngine__init(unsigned int, ...);
void milesEngine__deinit();
void milesEngine__setter(unsigned int, ...);
void milesEngine__getter(unsigned int, ...);


stored_functions_engine miles_engine_vtbl = {milesEngine__init, milesEngine__deinit, milesEngine__setter, milesEngine__getter};



class_stored miles_engine_off (NULL, NULL, "MC2engines:miles.engine", classvtbl_get_milesEngine);




stored_functions *classvtbl_get_milesEngine()
{
	return (stored_functions *)&miles_engine_vtbl;
}

int milesEngine__init(unsigned int, ...)
{
////TODO
    return 1;
}

void milesEngine__deinit()
{
////TODO
}

void milesEngine__setter(unsigned int, ...)
{
////TODO
}

void milesEngine__getter(unsigned int, ...)
{
////TODO
}
