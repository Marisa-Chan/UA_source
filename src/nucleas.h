#ifndef NUCLEAS_H_INCLUDED
#define NUCLEAS_H_INCLUDED

#undef max
#undef min

#include <list>
#include "MFILE.h"

class NC_STACK_nucleus;

struct stack_vals
{
    unsigned int id;
    union _value
    {
        void *p_data;
        int i_data;
        float f_data;
        size_t u_data;
    } value;


    enum TAG
    {
        TAG_END = 0,
        TAG_SKIP = 1,
        TAG_PTAGS = 2,
        TAG_SKIP_N = 3
    };



    void set(unsigned int _id, size_t _val)
    {
        id = _id;
        value.u_data = _val;
    }

    void set(unsigned int _id, int _val)
    {
        id = _id;
        value.i_data = _val;
    }

    void set(unsigned int _id, float _val)
    {
        id = _id;
        value.f_data = _val;
    }

    void set(unsigned int _id, void *_val)
    {
        id = _id;
        value.p_data = _val;
    }

    void set(unsigned int _id, const void *_val)
    {
        id = _id;
        value.p_data = (void *)_val;
    }

    void end()
    {
        id = TAG_END;
        value.u_data = 0;
    }

    void nextStack(stack_vals *lst)
    {
        id = TAG_PTAGS;
        value.p_data = lst;
    }


};

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


NC_STACK_nucleus * init_get_class(const char *classname, stack_vals *stak);

void va_to_arr(stack_vals *out, int sz, va_list in);
void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list in);
int delete_class_obj(NC_STACK_nucleus *cls);
stack_vals * find_id_in_stack2(unsigned int id, stack_vals *a2);
size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, stack_vals *a3);

NC_STACK_nucleus * READ_OBJT(MFILE *mfile);
int sub_4117F8(NC_STACK_nucleus *obj, MFILE *mfile);

struct NC_STACK_base;

NC_STACK_base *READ_BAS_FILE(const char *fname);


#endif // NUCLEAS_H_INCLUDED
