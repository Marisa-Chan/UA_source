#include "includes.h"
#include "nucleas.h"
#include "utils.h"

const NewClassDescr NC_STACK_nucleus::description("nucleus.class", &newinstance);


void nucleus_func0__sub0(NC_STACK_nucleus *a1, stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == 0x80000000 )
            {
                if ( a2->value )
                {
                    a1->NAME = (char *)AllocVec(33, 1);
                    if ( a1->NAME )
                        strncpy(a1->NAME, (const char *)a2->value, 32);
                }
            }
            a2++;
        }
    }
}



size_t NC_STACK_nucleus::func0(stack_vals *stack)
{
    nucleus_func0__sub0(this, stack);
    return 1;
}

size_t NC_STACK_nucleus::func1(stack_vals *)
{
    return 1;
}

void nucleus_setter(NC_STACK_nucleus *a1, stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == 0x80000000 )
            {
                if ( a2->value )
                {
                    if (a1->NAME == NULL)
                    {
                        a1->NAME = (char *)AllocVec(33, 1);
                        if ( a1->NAME )
                            strncpy(a1->NAME, (const char *)a2->value, 32);
                    }
                    else
                        strncpy(a1->NAME, (const char *)a2->value, 32);

                }
                else if (a1->NAME != NULL )
                {
                    nc_FreeMem(a1->NAME);
                    a1->NAME = NULL;
                }
            }
            a2++;
        }
    }
}

size_t NC_STACK_nucleus::func2(stack_vals *a3)
{
    nucleus_setter(this, a3);

    return 1;
}


void nucleus_getter(NC_STACK_nucleus *a1, stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == 0x80000000 )
                *(char **)a2->value = a1->NAME;
            else if ( a2->id == 0x80000001 )
                *(void **)a2->value = NULL;
            else if ( a2->id == 0x80000002 )
                *(const char **)a2->value = a1->getClassName();
            a2++;
        };
    }
}


size_t NC_STACK_nucleus::func3(stack_vals *stk)
{
    nucleus_getter(this, stk);

    return 1;
}

size_t NC_STACK_nucleus::func5(MFILE **file)
{
    MFILE *mfile = *file;

    while ( 1 )
    {
        signed int v6 = read_next_IFF(mfile, 2);
        if ( v6 == -2 )
            break;
        if ( v6 )
        {
            func1(NULL);
            return 0;
        }
        if ( GET_FORM_INFO_OR_NULL(mfile)->TAG == TAG_NAME )
        {
            char a4[33];
            memset(a4, 0, 33);

            if ( !mfread(mfile, a4, 32) )
            {
                func1(NULL);
                return 0;
            }

            call_vtbl(this, 2, 0x80000000, a4, 0,0);  // nucleus::SetName(new_name)
            read_next_IFF(mfile, 2);
        }
        else
        {
            read_default(mfile);
        }
    }

    return 1;
}


size_t NC_STACK_nucleus::func6(MFILE **val)
{
    MFILE *mfile = *val;
    if ( sub_412FC0(mfile, TAG_ROOT, TAG_FORM, -1) )
        return 0;
    else
    {
        if ( this->NAME )
        {
            sub_412FC0(mfile, 0, TAG_NAME, -1);
            sub_413564(mfile, strlen(this->NAME), this->NAME);
            sub_413290(mfile);
        }
        sub_413290(mfile);
        return 1;
    }
    return 0;
}


size_t NC_STACK_nucleus::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return (size_t)func2( (stack_vals *)data );
    case 3:
        return (size_t)func3( (stack_vals *)data );
    case 5:
        return (size_t)func5( (MFILE **)data );
    case 6:
        return (size_t)func6( (MFILE **)data );
    default:
        break;
    }
    return 0;
}

