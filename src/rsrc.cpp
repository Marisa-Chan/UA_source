#include <string.h>

#include "rsrc.h"
#include "utils.h"

const Nucleus::ClassDescr NC_STACK_rsrc::description("rsrc.class", &newinstance);

RSRCList NC_STACK_rsrc::privateList;
RSRCList NC_STACK_rsrc::publicList;

rsrc *rsrc_find(RSRCList *list, const std::string &name)
{
    for( rsrc* res : *list )
    {
        if (!StriCmp(name, res->name))
            return res;
    }

    return NULL;
}

size_t NC_STACK_rsrc::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    int reuse_loaded = 1;

    const std::string res_name = stak.Get<std::string>(RSRC_ATT_NAME, "");
    reuse_loaded = stak.Get<int32_t>(RSRC_ATT_TRYSHARED, 1);

    if ( res_name.empty() )
    {
        func1();
        return 0;
    }

    rsrc *res = NULL;

    if ( reuse_loaded == 1 )
        res = rsrc_find(&publicList, res_name);

    if (!res)
        res = rsrc_func64(stak);

    if ( res )
    {
        res->ref_cnt++;
        resource = res;

        if ( stak.Get<int32_t>(RSRC_ATT_DONTCOPY, 0) )
            _flags |= 1;

        return 1;
    }
    else
    {
        func1();
        return 0;
    }
}

size_t NC_STACK_rsrc::func1()
{
    if ( resource )
    {
        resource->ref_cnt--;

        if ( resource->ref_cnt == 0 )
            rsrc_func65(resource);

        resource = NULL;
    }

    return NC_STACK_nucleus::func1();
}

// Allocate resource node
rsrc * NC_STACK_rsrc::rsrc_func64(IDVList &stak)
{
    const std::string resname = stak.Get<std::string>(RSRC_ATT_NAME, "");
    int shared = stak.Get<int32_t>(RSRC_ATT_TRYSHARED, 1);
    int toTail = stak.Get<int32_t>(RSRC_ATT_LISTYPE, 0);

    if ( resname.empty() )
        return NULL;

    rsrc *res = new rsrc(resname, shared);

    if ( res )
    {
        RSRCList *lst;

        if (shared == 1)
            lst = &publicList;
        else
            lst = &privateList;

        if ( toTail )
            lst->push_back(res);
        else
            lst->push_front(res);
    }
    return res;
}

size_t NC_STACK_rsrc::rsrc_func65(rsrc *res)
{
    if (res->shared)
        publicList.remove(res);
    else
        privateList.remove(res);

    if ( res )
        delete res;

    return 1;
}

const char * NC_STACK_rsrc::getRsrc_name()
{
    if ( resource )
        return resource->name.c_str();

    return NULL;
}

const std::string NC_STACK_rsrc::getRsrc_name() const
{
    if ( resource )
        return resource->name;

    return std::string();
}

int NC_STACK_rsrc::getRsrc_tryShared()
{
    if ( resource )
        return resource->shared;

    return 2;
}

void *NC_STACK_rsrc::getRsrc_pData()
{
    if (resource)
        return resource->data;

    else return NULL;
}

int NC_STACK_rsrc::getRsrc_dontCopy()
{
    if (_flags & 1)
        return 1;

    return 0;
}

RSRCList *NC_STACK_rsrc::getRsrc_sharedList()
{
    return &publicList;
}

RSRCList *NC_STACK_rsrc::getRsrc_privateList()
{
    return &privateList;
}

