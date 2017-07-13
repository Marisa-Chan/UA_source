#ifndef ADE_H_INCLUDED
#define ADE_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

class NC_STACK_ade;

struct ADE_STRC
{
    int16_t version;
    int8_t _nu1; // Not used
    int8_t flags;
    int16_t point;
    int16_t poly;
    int16_t _nu2; // Not used
};


struct __NC_STACK_ade : public nnode
{
    NC_STACK_ade *self;
    char flags;
    int16_t point;
    int16_t poly;
};

class NC_STACK_ade: public NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stak);
    virtual size_t func1(stack_vals *stak);
    virtual size_t func2(stack_vals *stak);
    virtual size_t func3(stack_vals *stak);
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
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

    enum ADE_FLAG
    {
        ADE_FLAG_INLIST = 1,
        ADE_FLAG_DPTHFADE = 2,
        ADE_FLAG_BKCHECK = 4
    };

    enum ADE_ATT
    {
        ADE_ATT_BKCHECK = 0x80001003,
        ADE_ATT_DPTHFADE = 0x80001004,
        ADE_ATT_POINT = 0x80001007,
        ADE_ATT_POLY = 0x80001008,
        ADE_ATT_PADE = 0x8000100B
    };

    virtual int getADE_bkCheck();
    virtual int getADE_depthFade();
    virtual int getADE_point();
    virtual int getADE_poly();
    virtual __NC_STACK_ade *getADE_pAde();

    virtual void setADE_bkCheck(int);
    virtual void setADE_depthFade(int);
    virtual void setADE_point(int);
    virtual void setADE_poly(int);


    void ade_func0__sub0(stack_vals *stak);
    void ade_func2__sub0(stack_vals *stak);
    void ade_func3__sub0(stack_vals *stak);

    //Data
    static const NewClassDescr description;

    __NC_STACK_ade stack__ade;
};

#endif // ADE_H_INCLUDED
