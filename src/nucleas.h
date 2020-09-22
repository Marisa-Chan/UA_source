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

namespace Nucleus
{
    
struct ClassDescr
{
    const std::string _classname;
    NC_STACK_nucleus *(*_newinstance)();

    ClassDescr(const std::string &clsname,  NC_STACK_nucleus *(*newinst)() );
};

}

class NC_STACK_nucleus
{
public:
    static constexpr const char * DefaultIniFile {"nucleus.ini"};
public:
    virtual size_t func0(IDVList &stak);
    virtual size_t func1();
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);

    NC_STACK_nucleus() {
        NAME.clear();
    };
    virtual ~NC_STACK_nucleus() {};

    virtual const std::string &GetClassName() const {
        return description._classname;
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
    static const Nucleus::ClassDescr description;

public:
    std::string NAME;
};


namespace Nucleus
{

class ClassList: public std::list<ClassDescr>
{
public:
    iterator find(const std::string &clsname);

static ClassList Instance;
};



/***
    Default class constructor when you exactly
    know what class you are want

    With values list.
***/
template<class T>
T* CInit(IDVList &stak)
{
    T *tmp = new T();
    if (!tmp)
        return NULL;

    if (!tmp->func0(stak))
    {
        delete tmp;
        return NULL;
    }

    return tmp;
}

template<class T>
T* CInit(IDVList::TInitList lst)
{
    IDVList stak(lst);
    return CInit<T>(stak);
}

/***
    Default class constructor when you exactly
    know what class you are want

    With empty values list.
***/
template<class T>
T* CInit()
{
    T *tmp = new T();
    if (!tmp)
        return NULL;

    IDVList stak;
    if (!tmp->func0(stak))
    {
        delete tmp;
        return NULL;
    }

    return tmp;
}


/***
    Class constructor when you get class name as string
    Will return pointer to default base class

    Null on unknown
***/
NC_STACK_nucleus *CFInit(const std::string &classname, IDVList &stak);
NC_STACK_nucleus *CFInit(const std::string &classname);
NC_STACK_nucleus *CFInit(const std::string &classname, IDVList::TInitList lst);


/***
    Class constructor when you get class name as string
    Will try to cast created class to specified class

    Null on unknown or if can't cast
***/
template <class T>
T *CTFInit(const std::string &classname, IDVList &stak)
{
    ClassList::iterator it = ClassList::Instance.find(classname);
    if (it == ClassList::Instance.end())
        return NULL;

    NC_STACK_nucleus *inst = it->_newinstance();
    T *tInst = dynamic_cast<T *>(inst);

    if (!tInst || !tInst->func0(stak) )
    {
        delete inst;
        return NULL;
    }

    return tInst;
}

template <class T>
T *CTFInit(const std::string &classname, IDVList::TInitList lst)
{
    IDVList stak(lst);
    return CTFInit<T>(classname, stak);
}

template <class T>
T *CTFInit(const std::string &classname)
{
    IDVList empty;
    return CTFInit<T>(classname, empty);
}


void Delete(NC_STACK_nucleus *clas);
}





int delete_class_obj(NC_STACK_nucleus *cls);

int sub_4117F8(NC_STACK_nucleus *obj, IFFile *mfile);

#endif // NUCLEAS_H_INCLUDED
