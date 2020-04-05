#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "idev.h"
#include "utils.h"



const Nucleus::ClassDescr NC_STACK_idev::description("idev.class", &newinstance);


size_t NC_STACK_idev::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 69:
        idev_func69( (int )(size_t)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
