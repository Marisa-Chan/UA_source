#include "includes.h"
#include "nucleas.h"
#include "utils.h"

namespace Nucleus
{
ClassList ClassList::Instance;

ClassList::iterator ClassList::find(const std::string &clsname)
{
    for(iterator it = begin(); it != end(); it++)
    {
        if (!StriCmp(clsname, it->_classname) )
            return it;
    }

    return end();
}

ClassDescr::ClassDescr(const std::string &clsname,  NC_STACK_nucleus *(*newinst)() )
: _classname(clsname)
, _newinstance(newinst)
{};



NC_STACK_nucleus *CFInit(const std::string &classname, IDVList &stak)
{
    ClassList::iterator it = ClassList::Instance.find(classname);
    if (it == ClassList::Instance.end())
        return NULL;

    NC_STACK_nucleus *inst = it->_newinstance();

    if ( !inst->func0(stak) )
    {
        delete inst;
        return NULL;
    }

    return inst;
}

NC_STACK_nucleus *CFInit(const std::string &classname)
{
    IDVList empty;
    return CFInit(classname, empty);
}

void Delete(NC_STACK_nucleus *clas)
{
    clas->func1();
    delete clas;
}

}




const Nucleus::ClassDescr NC_STACK_nucleus::description("nucleus.class", &newinstance);

size_t NC_STACK_nucleus::func0(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case NC_ATT_NAME:
                setName((const char *)val.value.p_data);
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_nucleus::func1()
{
    NAME.clear();

    return 1;
}

size_t NC_STACK_nucleus::func2(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case NC_ATT_NAME:
                setName((const char *)val.value.p_data);
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_nucleus::func3(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case NC_ATT_NAME:
                *(const char **)val.value.p_data = NAME.c_str();
                break;

            case NC_ATT_CLASSNAME:
                *(const char **)val.value.p_data = getClassName();
                break;

            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_nucleus::func5(IFFile **file)
{
    IFFile *mfile = *file;

    while ( 1 )
    {
        signed int v6 = mfile->parse();
        if ( v6 == IFFile::IFF_ERR_EOC )
            break;
        if ( v6 )
        {
            func1();
            return 0;
        }
        if ( mfile->getCurrentChunk()->TAG == TAG_NAME )
        {
            char a4[33];
            memset(a4, 0, 33);

            if ( !mfile->read(a4, 32) )
            {
                func1();
                return 0;
            }

            setName(a4);
            mfile->parse();
        }
        else
        {
            mfile->skipChunk();
        }
    }

    return 1;
}


size_t NC_STACK_nucleus::func6(IFFile **val)
{
    IFFile *mfile = *val;
    if ( mfile->pushChunk(TAG_ROOT, TAG_FORM, -1) )
        return 0;
    else
    {
        if ( !NAME.empty() && NAME.length() <= 32 )
        {
            mfile->pushChunk(0, TAG_NAME, -1);
            mfile->write(NAME.c_str(), NAME.length());
            mfile->popChunk();
        }
        mfile->popChunk();
        return 1;
    }
    return 0;
}

void NC_STACK_nucleus::setName(const std::string &newName)
{
    if ( !newName.empty() )
        NAME = newName;
    else
        NAME.clear();
}

const std::string &NC_STACK_nucleus::getName() const
{
    return NAME;
}


size_t NC_STACK_nucleus::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return (size_t)func2( *(IDVList *)data );
    case 3:
        return (size_t)func3( *(IDVList *)data );
    case 5:
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
    default:
        break;
    }
    return 0;
}






int delete_class_obj(NC_STACK_nucleus *cls)
{
    int ret = cls->func1();

    delete cls;

    return ret;
}

NC_STACK_nucleus *NC_STACK_nucleus::READ_OBJT(IFFile *mfile)
{
    Nucleus::ClassList::iterator clss = Nucleus::ClassList::Instance.end();
    NC_STACK_nucleus *obj = NULL;
    while ( 1 )
    {
        int v4 = mfile->parse();

        if ( v4 == IFFile::IFF_ERR_EOC )
            break;

        if ( v4 )
            return NULL;

        int tag = mfile->getCurrentChunk()->TAG;

        if ( tag == TAG_CLID )
        {
            char classname[300];
            memset(classname, 0, 300);

            if ( mfile->read(classname, 256) < 0 )
                return NULL;

            clss = Nucleus::ClassList::Instance.find(classname);
            if ( clss == Nucleus::ClassList::Instance.end() )
            {
                return NULL;
            }


            mfile->parse();
        }
        else if ( tag == TAG_FORM )
        {
            IFFile *v11 = mfile;

            obj = clss->_newinstance();

            if ( !obj->func5(&v11) )
            {
                delete obj;
                return NULL;
            }
        }
        else
        {
            if ( !mfile->skipChunk() )
            {
                return NULL;
            }

        }
    }
    return obj;
}

int sub_4117F8(NC_STACK_nucleus *obj, IFFile *mfile)
{
    if ( mfile->pushChunk(TAG_OBJT, TAG_FORM, -1) )
        return 0;

    const char *clsname = obj->getClassName();
    int namesz = strlen(clsname) + 1;

    if ( mfile->pushChunk(0, TAG_CLID, namesz) )
        return 0;

    if ( mfile->write(clsname, namesz) < 0 )
        return 0;

    mfile->popChunk();

    IFFile *tmp = mfile;
    int res = obj->func6(&tmp);
    mfile->popChunk();

    return res;
}


