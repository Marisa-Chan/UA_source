#ifndef YFLYER_H_INCLUDED
#define YFLYER_H_INCLUDED

#include "nucleas.h"
#include "ypabact.h"

struct __NC_STACK_ypaflyer
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    float field_c;
    int field_10;
};

class NC_STACK_ypaflyer: public NC_STACK_ypabact
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func70(ypabact_arg65 *arg);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual void ypabact_func74(bact_arg74 *arg);
    virtual size_t ypabact_func80(bact_arg80 *arg);
    virtual void ypabact_func96(void *);
    virtual void ypabact_func97(ypabact_arg65 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypaflyer() {
        memset(&stack__ypaflyer, 0, sizeof(stack__ypaflyer));
    };
    virtual ~NC_STACK_ypaflyer() {};

    virtual const char * getClassName() {
        return "ypaflyer.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypaflyer();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypaflyer stack__ypaflyer;
};

#endif // YFLYER_H_INCLUDED
