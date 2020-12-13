#ifndef EMBED_H_INCLUDED
#define EMBED_H_INCLUDED

#include "nucleas.h"
#include <deque>

class NC_STACK_embed: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);

    NC_STACK_embed() {};
    virtual ~NC_STACK_embed() {};
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_embed();
    };

    //Data
    static const Nucleus::ClassDescr description;

    std::deque<NC_STACK_rsrc *> _resources;
};

#endif // EMBED_H_INCLUDED
