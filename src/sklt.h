#ifndef SKLT_H_INCLUDED
#define SKLT_H_INCLUDED

#include "skeleton.h"

struct __NC_STACK_sklt
{

};

class NC_STACK_sklt: public NC_STACK_skeleton
{
public:
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg);

    NC_STACK_sklt() {
        memset(&stack__sklt, 0, sizeof(stack__sklt));
    };
    virtual ~NC_STACK_sklt() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_sklt();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_sklt stack__sklt;
};

#endif // SKLT_H_INCLUDED
