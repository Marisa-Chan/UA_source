#include "includes.h"
#include "nucleas.h"
#include "utils.h"

const NewClassDescr NC_STACK_nucleus::description("nucleus.class", &newinstance);


void NC_STACK_nucleus::nucleus_func0__sub0(stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value.p_data;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == NC_ATT_NAME )
                setName((const char *)a2->value.p_data);

            a2++;
        }
    }
}



size_t NC_STACK_nucleus::func0(stack_vals *stack)
{
    nucleus_func0__sub0(stack);
    return 1;
}

size_t NC_STACK_nucleus::func1(stack_vals *)
{
    if (NAME)
        free(NAME);

    return 1;
}

void NC_STACK_nucleus::nucleus_setter(stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value.p_data;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == NC_ATT_NAME )
                setName( (const char *)a2->value.p_data );

            a2++;
        }
    }
}

size_t NC_STACK_nucleus::func2(stack_vals *a3)
{
    nucleus_setter(a3);

    return 1;
}


void NC_STACK_nucleus::nucleus_getter(stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == 0)
            break;

        if ( a2->id == 2 )
        {
            a2 = (stack_vals *)a2->value.p_data;
        }
        else if ( a2->id == 3 )
        {
            a2 += a2->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            if ( a2->id == NC_ATT_NAME )
                *(const char **)a2->value.p_data = getName();
            else if ( a2->id == NC_ATT_CLASSNAME )
                *(const char **)a2->value.p_data = getClassName();
            a2++;
        };
    }
}


size_t NC_STACK_nucleus::func3(stack_vals *stk)
{
    nucleus_getter(stk);

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

            setName(a4);
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

void NC_STACK_nucleus::setName(const char *newName)
{
    if ( newName )
    {
        if (NAME == NULL)
        {
            NAME = (char *)calloc(1, 33);

            if ( NAME )
                strncpy(NAME, newName, 32);
        }
        else
            strncpy(NAME, newName, 32);

    }
    else if (NAME != NULL )
    {
        free(NAME);
        NAME = NULL;
    }
}

const char *NC_STACK_nucleus::getName()
{
    return NAME;
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






NC_STACK_nucleus * init_get_class(const char *classname, stack_vals *stak)
{
    stack_vals tmp[1];
    tmp[0].end();

    if (stak == NULL)
        stak = tmp;

    const NewClassDescr *cls_descr = getClassAllocator(classname);

    if (!cls_descr)
        return NULL;

    NC_STACK_nucleus *class_examplar = cls_descr->newinstance();

    //class object constructor
    if ( !class_examplar->func0(stak) )
    {
        delete class_examplar;
        return NULL;
    }

    return class_examplar;
}


int delete_class_obj(NC_STACK_nucleus *cls)
{
    int ret = cls->func1(NULL);

    delete cls;

    return ret;
}


stack_vals * find_id_in_stack2(unsigned int id, stack_vals *a2)
{
    while ( 1 )
    {
        if ( a2->id == id )
            return a2;
        if ( a2->id == 0 )
            return NULL;

        if ( a2->id == 2 )
            a2 = (stack_vals *)a2->value.p_data;
        else if ( a2->id == 3 )
            a2 += a2->value.i_data;
        else
            a2++;
    }
}

size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, stack_vals *a3)
{
    if (!a3)
        return def_value;

    while ( 1 )
    {
        if ( a3->id == find_id )
            return a3->value.u_data;
        if ( a3->id == 0 )
            return def_value;

        if ( a3->id == 2 )
        {
            if (a3->value.p_data == NULL)
                printf("find_id_in_stack_def_val, stack NULL pointer\n");
            else
                a3 = (stack_vals *)a3->value.p_data;
        }
        else if ( a3->id == 3 )
            a3 += a3->value.i_data;
        else
            a3++;
    }
}


NC_STACK_nucleus * READ_OBJT(MFILE *mfile)
{
    const NewClassDescr *clss = NULL;
    NC_STACK_nucleus *obj = NULL;
    while ( 1 )
    {
        int v4 = read_next_IFF(mfile, 2);

        if ( v4 == -2 )
            break;

        if ( v4 )
            return NULL;

        int tag = GET_FORM_INFO_OR_NULL(mfile)->TAG;

        if ( tag == TAG_CLID )
        {
            char classname[300];
            memset(classname, 0 , 300);

            if ( mfread(mfile, classname, 256) < 0 )
                return NULL;

            clss = getClassAllocator(classname); // get_class(classname);

            if ( !clss )
            {
                return NULL;
            }


            read_next_IFF(mfile, 2);
        }
        else if ( tag == TAG_FORM )
        {
            MFILE *v11 = mfile;

            obj = clss->newinstance();

            if ( !obj->func5(&v11) )
            {
                delete obj;
                return NULL;
            }
        }
        else
        {
            if ( !read_default(mfile) )
            {
                return NULL;
            }

        }
    }
    return obj;
}

NC_STACK_base *READ_BAS_FILE(const char *fname)
{
    NC_STACK_base *result = NULL;

    MFILE *mfile = new_MFILE();
    if ( !mfile )
        return NULL;

    FILE *fil = FOpen(fname, "rb");
    mfile->file_handle = fil;
    if ( !fil )
    {
        del_MFILE(mfile);
        return NULL;
    }

    if ( sub_412F98(mfile, 0) )
    {
        FClose(mfile->file_handle);
        del_MFILE(mfile);
        return NULL;
    }

    if ( !read_next_IFF(mfile, 2) )
    {
        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);
        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_MC2 && !read_next_IFF(mfile, 2) )
        {
            chunk = GET_FORM_INFO_OR_NULL(mfile);
            if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_OBJT )
                result = (NC_STACK_base *)READ_OBJT(mfile);
        }
    }

    FClose(fil);
    del_MFILE(mfile);

    return result;
}

int sub_4117F8(NC_STACK_nucleus *obj, MFILE *mfile)
{
    if ( sub_412FC0(mfile, TAG_OBJT, TAG_FORM, -1) )
        return 0;

    const char *clsname = obj->getClassName();
    int namesz = strlen(clsname) + 1;

    if ( sub_412FC0(mfile, 0, TAG_CLID, namesz) )
        return 0;

    if ( sub_413564(mfile, namesz, clsname) < 0 )
        return 0;

    sub_413290(mfile);
    MFILE *tmp = mfile;
    int res = obj->func6(&tmp);
    sub_413290(mfile);

    return res;
}




void va_to_arr(stack_vals *out, int sz, va_list in)
{
    memset(out, 0, sizeof(stack_vals) * sz);

    int previd = -1;
    for (int i = 0; i < sz; i++)
    {
        if (previd == 2)
        {
            out[i].end();
            break;
        }

        unsigned int id = va_arg(in, unsigned int);
        if (id == 0 || i == sz - 1)
        {
            out[i].end();
            break;
        }

        size_t value = va_arg(in,size_t);

        out[i].set(id, value);

        previd = id;
    }
}

void va_to_arr(stack_vals *out, int sz, unsigned int _id, va_list in)
{
    memset(out, 0, sizeof(stack_vals) * sz);

    int previd = -1;
    if (_id != 0)
    {
        out[0].set(_id, va_arg(in, size_t) );

        for (int i = 1; i < sz; i++)
        {
            if (previd == 2)
            {
                out[i].end();
                break;
            }

            unsigned int id = va_arg(in, unsigned int);
            if (id == 0 || i == sz - 1)
            {
                out[i].end();
                break;
            }

            size_t value = va_arg(in,size_t);

            out[i].set(id, value);

            previd = id;
        }
    }
}

