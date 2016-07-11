#ifndef NUCLEAS_H_INCLUDED
#define NUCLEAS_H_INCLUDED

#include <list>

class NC_STACK_nucleus;

struct NewClassDescr
{
    const char *classname;
    NC_STACK_nucleus *(*newinstance)();

    NewClassDescr(const char *_clsname, NC_STACK_nucleus *(*_newinst)() ): classname(_clsname), newinstance(_newinst) {}
};


typedef std::list<const NewClassDescr *> ClassList;
typedef std::list<const NewClassDescr *>::iterator ClassList_iter;



class NC_STACK_nucleus
{
public:
    virtual size_t func0(stack_vals *stack);
    virtual size_t func1(stack_vals *);
    virtual size_t func2(stack_vals *a3);
    virtual size_t func3(stack_vals *stk);
    virtual size_t func5(MFILE **file);
    virtual size_t func6(MFILE **val);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_nucleus() {
        NAME = NULL;
    };
    virtual ~NC_STACK_nucleus() {};

    virtual const char * getClassName() {
        return "nucleus.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_nucleus();
    };

    //Data
    static const NewClassDescr description;

    char *NAME;
};



#endif // NUCLEAS_H_INCLUDED
