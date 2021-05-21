#ifndef SKLT_H_INCLUDED
#define SKLT_H_INCLUDED

#include "skeleton.h"

class NC_STACK_sklt: public NC_STACK_skeleton
{
public:
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t func6(IFFile **file);
    virtual rsrc * rsrc_func64(IDVList &stak);
    virtual size_t rsrc_func66(rsrc_func66_arg *arg);

    NC_STACK_sklt() {};
    virtual ~NC_STACK_sklt() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_sklt();
    };

    //Data
    static const Nucleus::ClassDescr description;
};

#endif // SKLT_H_INCLUDED
