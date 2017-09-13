#ifndef NUCLEAS_H_INCLUDED
#define NUCLEAS_H_INCLUDED

#undef max
#undef min

#include <inttypes.h>
#include <list>
#include <map>
#include "IFFile.h"

class NC_STACK_nucleus;

struct IDVPair
{
    unsigned int id;
    uint8_t flags;
    uint8_t type;

    union _value
    {
        void *p_data;
        int i_data;
        float f_data;
        size_t u_data;
    } value;

    enum TYPE
    {
        TYPE_UNK     = 0,
        TYPE_POINTER = 1,
        TYPE_INTEGER = 2,
        TYPE_FLOAT   = 3,
        TYPE_SZ      = 4
    };

    enum TAG
    {
        TAG_END = 0,
        TAG_SKIP = 1,
        TAG_PTAGS = 2,
        TAG_SKIP_N = 3,
        TAG_UNKNOWN = 4
    };

    enum FLAGS
    {
        FLAGS_SKIP = 1
    };

    IDVPair()
    {
        id = TAG_END;
        value.u_data = 0;
        flags = 0;
        type = TYPE_UNK;
    }

    IDVPair(unsigned int _id, size_t _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, int _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, float _val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, void *_val)
    {
        flags = 0;
        set(_id, _val);
    }

    IDVPair(unsigned int _id, const void *_val)
    {
        flags = 0;
        set(_id, _val);
    }

    void set(unsigned int _id, size_t _val)
    {
        id = _id;
        value.u_data = _val;
        type = TYPE_SZ;
    }

    void set(unsigned int _id, int _val)
    {
        id = _id;
        value.i_data = _val;
        type = TYPE_INTEGER;
    }

    void set(unsigned int _id, float _val)
    {
        id = _id;
        value.f_data = _val;
        type = TYPE_FLOAT;
    }

    void set(unsigned int _id, void *_val)
    {
        id = _id;
        value.p_data = _val;
        type = TYPE_POINTER;
    }

    void set(unsigned int _id, const void *_val)
    {
        id = _id;
        value.p_data = (void *)_val;
        type = TYPE_POINTER;
    }

    void end()
    {
        id = TAG_END;
        value.u_data = 0;
    }

    void nextStack(IDVPair *lst)
    {
        id = TAG_PTAGS;
        value.p_data = lst;
    }

    inline bool skip()
    {
        return (flags & FLAGS_SKIP) != 0;
    }
};

//typedef std::list<IDVPair> IDVList;
class IDVList: public std::map<unsigned int, IDVPair>
{
public:
    void Add(unsigned int _id, size_t _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, int _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, float _val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, void *_val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    void Add(unsigned int _id, const void *_val)
    {
        (*this)[_id] = IDVPair(_id, _val);
    }

    int Get(unsigned int _id, int _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.i_data;
    }

    size_t Get(unsigned int _id, size_t _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.u_data;
    }

    float Get(unsigned int _id, float _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.f_data;
    }

    void *GetPointer(unsigned int _id, void *_def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (it->second).value.p_data;
    }

    const char * GetConstChar(unsigned int _id, const char * _def)
    {
        iterator it = find(_id);

        if (it == end())
            return _def;
        else
            return (const char *)(it->second).value.u_data;
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
    virtual size_t func0(IDVList *stak);
    virtual size_t func1();
    virtual size_t func2(IDVList *stak);
    virtual size_t func3(IDVList *stak);
    virtual size_t func5(IFFile **file);
    virtual size_t func6(IFFile **file);

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


    //Data
    static const NewClassDescr description;

    char *NAME;
};


NC_STACK_nucleus * init_get_class(const char *classname, IDVList *stak);

int delete_class_obj(NC_STACK_nucleus *cls);
IDVPair * find_id_in_stack2(unsigned int id, IDVPair *a2);
size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, IDVPair *a3);
void *find_id_pval(unsigned int find_id, IDVPair *a3);

NC_STACK_nucleus * READ_OBJT(IFFile *mfile);
int sub_4117F8(NC_STACK_nucleus *obj, IFFile *mfile);

struct NC_STACK_base;

NC_STACK_base *READ_BAS_FILE(const char *fname);


#endif // NUCLEAS_H_INCLUDED
