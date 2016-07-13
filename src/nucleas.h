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

    enum NC_ATT
    {
        NC_ATT_NAME = 0x80000000,
        NC_ATT_CLASSNAME = 0x80000002
    };

    //Set
    virtual void setName(const char *);

    //Get
    virtual const char * getName();

    //Non-virtual methods
    void nucleus_func0__sub0(stack_vals *a2);
    void nucleus_setter(stack_vals *a2);
    void nucleus_getter(stack_vals *a2);


    //Data
    static const NewClassDescr description;

    char *NAME;
};



#endif // NUCLEAS_H_INCLUDED
