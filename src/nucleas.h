#ifndef NUCLEAS_H_INCLUDED
#define NUCLEAS_H_INCLUDED

#undef max
#undef min

#include <inttypes.h>
#include <list>
#include <string>
#include "idvpair.h"
#include "IFFile.h"


class NC_STACK_nucleus;

struct NewClassDescr
{
    const std::string classname;
    NC_STACK_nucleus *(*newinstance)();

    NewClassDescr(const char *_clsname, NC_STACK_nucleus *(*_newinst)() ): classname(_clsname), newinstance(_newinst) {}
};


typedef std::list<const NewClassDescr *> ClassList;


class NC_STACK_nucleus
{
public:
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);

    virtual size_t compatcall(int method_id, void *data);
    NC_STACK_nucleus() {
        NAME.clear();
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
    virtual void setName(const std::string&);

    //Get
    virtual const std::string& getName() const;

    //Non-virtual methods
    static NC_STACK_nucleus *READ_OBJT(IFFile *mfile);

public:
    //Data
    static const NewClassDescr description;

public:
    std::string NAME;
};


NC_STACK_nucleus * init_get_class(const char *classname, IDVList *stak);

int delete_class_obj(NC_STACK_nucleus *cls);

int sub_4117F8(NC_STACK_nucleus *obj, IFFile *mfile);

#endif // NUCLEAS_H_INCLUDED
