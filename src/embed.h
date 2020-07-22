#ifndef EMBED_H_INCLUDED
#define EMBED_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

struct __NC_STACK_embed
{
    nlist embed_objects;
};

struct embd_node: public nnode
{
    int num;
    NC_STACK_rsrc *objects[32];
};

class NC_STACK_embed: public NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList &);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_embed() {
        memset(&stack__embed, 0, sizeof(stack__embed));
    };
    virtual ~NC_STACK_embed() {};
    
    virtual const std::string &GetClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_embed();
    };

    //Data
    static const Nucleus::ClassDescr description;

    __NC_STACK_embed stack__embed;
};

#endif // EMBED_H_INCLUDED
