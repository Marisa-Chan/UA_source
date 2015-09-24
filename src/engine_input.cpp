#include "includes.h"
#include "engine_input.h"

stored_functions *classvtbl_get_inputEngine();

int inputEngine__init(int, ...);
void inputEngine__deinit();
void inputEngine__setter(int, ...);
void inputEngine__getter(int, ...);


stored_functions_engine input_engine_vtbl = {inputEngine__init, inputEngine__deinit, inputEngine__setter, inputEngine__getter};



class_stored input_engine_off (NULL, NULL, "MC2engines:input.engine", classvtbl_get_inputEngine);




stored_functions *classvtbl_get_inputEngine()
{
	return (stored_functions *)&input_engine_vtbl;
}

int inputEngine__init(int, ...)
{
    ////TODO
    return 1;
}

void inputEngine__deinit()
{
////TODO
}

void inputEngine__setter(int, ...)
{
////TODO
}

void inputEngine__getter(int, ...)
{
////TODO
}
