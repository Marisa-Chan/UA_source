#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "itimer.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_itimer::description("itimer.class", &newinstance);

size_t NC_STACK_itimer::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}

