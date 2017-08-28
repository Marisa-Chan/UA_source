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

size_t NC_STACK_rsrc::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    const char *res_name = (const char *)find_id_in_stack_def_val(RSRC_ATT_NAME, 0, stak);
    int reuse_loaded = find_id_in_stack_def_val(RSRC_ATT_TRYSHARED, 1, stak);

    if ( !res_name )
    {
        func1(NULL);
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

        if ( find_id_in_stack_def_val(RSRC_ATT_DONTCOPY, 0, stak) )
            flags |= 1;

        return 1;
    }
    else
    {
        func1(NULL);
        return 0;
    }
}

size_t NC_STACK_rsrc::func1(stack_vals *stak)
{
    if ( resource )
    {
        resource->ref_cnt--;

        if ( !resource->ref_cnt )
            rsrc_func65(resource);
    }

    return NC_STACK_nucleus::func1(stak);
}

size_t NC_STACK_rsrc::func3(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case RSRC_ATT_NAME:
                *(const char **)stk->value.p_data = getRsrc_name();
                break;

            case RSRC_ATT_PDATA:
                *(void **)stk->value.p_data = getRsrc_pData();
                break;

            case RSRC_ATT_TRYSHARED:
                *(int *)stk->value.p_data = getRsrc_tryShared();
                break;

            case RSRC_ATT_DONTCOPY:
                *(int *)stk->value.p_data = getRsrc_dontCopy();
                break;

            case RSRC_ATT_SHAREDLIST:
                *(RSRCList **)stk->value.p_data = getRsrc_sharedList();
                break;

            case RSRC_ATT_PRIVATELIST:
                *(RSRCList **)stk->value.p_data = getRsrc_privateList();
                break;
            }
            stk++;
        }
    }
    return NC_STACK_nucleus::func3(stak);
}

// Allocate resource node
rsrc * NC_STACK_rsrc::rsrc_func64(stack_vals *stak)
{
    char *resname = (char *)find_id_pval(RSRC_ATT_NAME, stak);

    int shared = find_id_in_stack_def_val(RSRC_ATT_TRYSHARED, 1, stak);

    int toTail = find_id_in_stack_def_val(RSRC_ATT_LISTYPE, 0, stak);

    rsrc *res = NULL;

    if ( resname )
    {
        res = new rsrc(resname, shared);

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
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 65:
        return (size_t)rsrc_func65( (rsrc *)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
