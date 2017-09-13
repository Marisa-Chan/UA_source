#include <string.h>

#include "rsrc.h"

const NewClassDescr NC_STACK_rsrc::description("rsrc.class", &newinstance);

RSRCList NC_STACK_rsrc::privateList;
RSRCList NC_STACK_rsrc::publicList;

rsrc *rsrc_find(RSRCList *list, const char *name)
{
    for(RSRCList::iterator it = list->begin(); it != list->end(); it++)
    {
        if (strcasecmp(name, (*it)->name.c_str()) == 0)
            return (*it);
    }

    return NULL;
}

NC_STACK_rsrc::NC_STACK_rsrc()
{
    resource = NULL;
    flags = 0;
}

size_t NC_STACK_rsrc::func0(IDVList *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    const char *res_name = NULL;
    int reuse_loaded = 1;

    if (stak)
    {
        res_name = stak->GetConstChar(RSRC_ATT_NAME, NULL);
        reuse_loaded = stak->Get(RSRC_ATT_TRYSHARED, 1);
    }

    if ( !res_name )
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

        if ( stak && stak->Get(RSRC_ATT_DONTCOPY, 0) )
            flags |= 1;

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

        if ( !resource->ref_cnt )
            rsrc_func65(resource);
    }

    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_rsrc::func3(IDVList *stak)
{
    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case RSRC_ATT_NAME:
                    *(const char **)val.value.p_data = getRsrc_name();
                    break;

                case RSRC_ATT_PDATA:
                    *(void **)val.value.p_data = getRsrc_pData();
                    break;

                case RSRC_ATT_TRYSHARED:
                    *(int *)val.value.p_data = getRsrc_tryShared();
                    break;

                case RSRC_ATT_DONTCOPY:
                    *(int *)val.value.p_data = getRsrc_dontCopy();
                    break;

                case RSRC_ATT_SHAREDLIST:
                    *(RSRCList **)val.value.p_data = getRsrc_sharedList();
                    break;

                case RSRC_ATT_PRIVATELIST:
                    *(RSRCList **)val.value.p_data = getRsrc_privateList();
                    break;

                default:
                    break;
                }
            }
        }
    }

    return NC_STACK_nucleus::func3(stak);
}

// Allocate resource node
rsrc * NC_STACK_rsrc::rsrc_func64(IDVList *stak)
{
    if (!stak)
        return NULL;

    const char *resname = stak->GetConstChar(RSRC_ATT_NAME, NULL);
    int shared = stak->Get(RSRC_ATT_TRYSHARED, 1);
    int toTail = stak->Get(RSRC_ATT_LISTYPE, 0);

    if ( !resname )
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
    if (flags & 1)
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


size_t NC_STACK_rsrc::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 3:
        return func3( (IDVList *)data );
    case 64:
        return (size_t)rsrc_func64( (IDVList *)data );
    case 65:
        return (size_t)rsrc_func65( (rsrc *)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
