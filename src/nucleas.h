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
    NC_STACK_nucleus *(*_newinstance)() = NULL;

    ClassDescr(const std::string &clsname,  NC_STACK_nucleus *(*newinst)() )
    : _classname(clsname)
    , _newinstance(newinst)
    {}
};

template <class _T>
static NC_STACK_nucleus * __ClassInstanciator() { return new _T(); };

template <class _T>
static ClassDescr MakeClassDescr()
{ 
    return ClassDescr(_T::__ClassName, __ClassInstanciator<_T>); 
}

}

class NC_STACK_nucleus
{
public:
    virtual size_t Init(IDVList &stak);
    virtual size_t Deinit();
    virtual size_t LoadingFromIFF(IFFile **file);
    virtual size_t SavingIntoIFF(IFFile **file);


    NC_STACK_nucleus() {
        NAME.clear();
    };
    virtual ~NC_STACK_nucleus() {};

    virtual const std::string ClassName() const {
        return __ClassName;
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

    //Load/Save from/to IFF
    static NC_STACK_nucleus *LoadObjectFromIFF(IFFile *mfile);
    bool SaveObjectIntoIFF(IFFile *mfile);
    
    void Delete() {
        Deinit();
        delete this;
    }

public:
    //Data
    static constexpr const char * __ClassName = "nucleus.class";

public:
    std::string NAME;
};


namespace Nucleus
{

class TClassList: public std::list<ClassDescr>
{
public:
    iterator find(const std::string &clsname);

public:
    static TClassList Instance;
};

static constexpr TClassList &ClassList = TClassList::Instance;



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

    if (!tmp->Init(stak))
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
    if (!tmp->Init(stak))
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
    TClassList::iterator it = ClassList.find(classname);
    if (it == ClassList.end())
        return NULL;

    NC_STACK_nucleus *inst = it->_newinstance();
    T *tInst = dynamic_cast<T *>(inst);

    if (!tInst || !tInst->Init(stak) )
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

}



#endif // NUCLEAS_H_INCLUDED
