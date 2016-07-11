#ifndef ADE_H_INCLUDED
#define ADE_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

class NC_STACK_ade;

struct __attribute__((packed)) ADE_STRC
{
    __int16 field_0;
    char field_2;
    char field_3;
    __int16 field_4;
    __int16 field_6;
    __int16 field_8;
};


struct __NC_STACK_ade : public nnode
{
    NC_STACK_ade *self;
    char flags;
    char field_D;
    __int16 strc_f4;
    __int16 strc_f6;
};

class NC_STACK_ade: public NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **file);
    virtual size_t ade_func64(nlist **lst);
    virtual size_t ade_func65(area_arg_65 *arg);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_ade() {
        memset(&stack__ade, 0, sizeof(stack__ade));
    };
    virtual ~NC_STACK_ade() {};

    virtual const char * getClassName() {
        return "ade.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ade();
    };

    //Data
    static const NewClassDescr description;

    __NC_STACK_ade stack__ade;
};

#endif // ADE_H_INCLUDED
