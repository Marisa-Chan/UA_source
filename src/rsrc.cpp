#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "utils.h"

const NewClassDescr NC_STACK_rsrc::description("rsrc.class", &newinstance);

nlist g_rsrc_list1;
nlist g_rsrc_list2;

rsrc *rsrc_find(nlist *list, const char *name)
{
    rsrc *current = (rsrc *)list->head;
    if ( current->next )
    {
        while ( strcasecmp(name, current->name) )
        {
            current = (rsrc *)current->next;
            if ( !current->next )
                return NULL;
        }
    }
    else
        return NULL;

    return current;
}

size_t NC_STACK_rsrc::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    __NC_STACK_rsrc *internal = &this->stack__rsrc;

    const char *res_name = (const char *)find_id_in_stack_def_val(0x80001000, 0, stak);
    int reuse_loaded = find_id_in_stack_def_val(0x80001001, 1, stak);

    if ( !res_name )
    {
        func1(NULL);
        return 0;
    }

    rsrc *res = NULL;

    if ( reuse_loaded == 1 )
        res = rsrc_find(&g_rsrc_list1, res_name);

    if (!res)
        res = rsrc_func64(stak);

    if ( res )
    {
        res->ref_cnt++;
        internal->p_rsrc = res;
        internal->p_data = res->data;

        if ( find_id_in_stack_def_val(0x80001003, 0, stak) )
            internal->field_8[0] |= 1;

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
    __NC_STACK_rsrc *internal = &this->stack__rsrc;

    if ( internal->p_rsrc )
    {
        rsrc *res = internal->p_rsrc;

        res->ref_cnt--;

        if ( !res->ref_cnt )
            rsrc_func65(&res);
    }

    return NC_STACK_nucleus::func1(stak);
}

size_t NC_STACK_rsrc::func3(stack_vals *stak)
{

    __NC_STACK_rsrc *internal = &this->stack__rsrc;

    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80001000:
            {
                rsrc *res = internal->p_rsrc;
                if ( res )
                    *(const char **)stk->value = res->name;
                else
                    *(const char **)stk->value = NULL;
                break;
            }

            case 0x80001002:
                *(void **)stk->value = internal->p_data;
                break;

            case 0x80001001:
                if ( internal->p_rsrc == NULL)
                    *(int *)stk->value = 2;
                else
                    *(int *)stk->value = internal->p_rsrc->what_list;
                break;

            case 0x80001003:
                if ( internal->field_8[0] & 1 )
                    *(int *)stk->value = 1;
                else
                    *(int *)stk->value = 0;
                break;

            case 0x80001006:
                *(nlist **)stk->value = &g_rsrc_list1;
                break;

            case 0x80001007:
                *(nlist **)stk->value = &g_rsrc_list2;
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
    char *title = (char *)find_id_in_stack_def_val(0x80001000, 0, stak);

    int what_list = find_id_in_stack_def_val(0x80001001, 1, stak);

    int toTail = find_id_in_stack_def_val(0x80001008, 0, stak);

    rsrc *res = NULL;

    if ( title )
    {
        res = (rsrc *)AllocVec(sizeof(rsrc), 1);

        if ( res )
        {
            strncpy(res->title, title, 64);

            res->ref_cnt = 0;
            res->data = NULL;
            res->name = res->title;
            res->what_list = what_list;

            call_vtbl(this, 3, 0x80000002, &res->class_name, 0);

            nlist *lst;

            if (what_list == 1)
                lst = &g_rsrc_list1;
            else
                lst = &g_rsrc_list2;

            if ( toTail )
                AddTail(lst, res);
            else
                AddHead(lst, res);
        }
    }
    return res;
}

size_t NC_STACK_rsrc::rsrc_func65(rsrc **res)
{
    Remove(*res);

    if ( *res )
        nc_FreeMem(*res);

    return 1;
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
        return (size_t)rsrc_func65( (rsrc **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
