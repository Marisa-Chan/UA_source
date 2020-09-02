#ifndef ADE_H_INCLUDED
#define ADE_H_INCLUDED

#include "nucleas.h"

class NC_STACK_ade;
struct area_arg_65;

typedef std::list<NC_STACK_ade *> AdeList;

struct ADE_STRC
{
    int16_t version;
    int8_t _nu1; // Not used
    int8_t flags;
    int16_t point;
    int16_t poly;
    int16_t _nu2; // Not used
};

class NC_STACK_ade: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual size_t ade_func64(AdeList &lst);
    virtual size_t ade_func65(area_arg_65 *arg);

    NC_STACK_ade() {
        flags = 0;
        point = 0;
        poly = 0;
        AttachedTo = NULL;
    };
    virtual ~NC_STACK_ade() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_ade();
    };

    enum ADE_FLAG
    {
//        ADE_FLAG_INLIST = 1,
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
//    virtual __NC_STACK_ade *getADE_pAde();

    virtual void setADE_bkCheck(int);
    virtual void setADE_depthFade(int);
    virtual void setADE_point(int);
    virtual void setADE_poly(int);

    //Data
    static const Nucleus::ClassDescr description;


public:
    char flags;
    int16_t point;
    int16_t poly;

    AdeList *AttachedTo;
};

#endif // ADE_H_INCLUDED
