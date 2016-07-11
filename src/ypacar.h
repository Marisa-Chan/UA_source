#ifndef YCAR_H_INCLUDED
#define YCAR_H_INCLUDED

#include "nucleas.h"
#include "ypatank.h"

struct __NC_STACK_ypacar
{
    NC_STACK_ypaworld *ywo;
    _NC_STACK_ypaworld *yw;
    __NC_STACK_ypabact *bact_internal;
    int field_c;
    int field_10;
};

class NC_STACK_ypacar: public NC_STACK_ypatank
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual void ypabact_func71(ypabact_arg65 *arg);
    virtual size_t ypatank_func128(tank_arg128 *arg);
    virtual size_t ypatank_func129(tank_arg129 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ypacar() {
        memset(&stack__ypacar, 0, sizeof(stack__ypacar));
    };
    virtual ~NC_STACK_ypacar() {};

    virtual const char * getClassName() {
        return "ypacar.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ypacar();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_ypacar stack__ypacar;
};

#endif // YCAR_H_INCLUDED
