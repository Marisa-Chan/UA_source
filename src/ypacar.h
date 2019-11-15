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
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func2(IDVList &stak);
    virtual size_t func3(IDVList &stak);
    virtual void User_layer(update_msg *arg);
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

    enum CAR_ATT
    {
        CAR_ATT_KAMIKAZE = 0x80003000,
        CAR_ATT_BLAST = 0x80003001
    };

    virtual void setCAR_kamikaze(int);
    virtual void setCAR_blast(int);

    virtual int getCAR_kamikaze();
    virtual int getCAR_blast();

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_ypacar stack__ypacar;
};

#endif // YCAR_H_INCLUDED
