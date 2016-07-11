#include "includes.h"
#include "classes.h"
#include "utils.h"

extern ClassList newclasses;

const NewClassDescr * FIND2(ClassList *list, const char *name)
{
    for(ClassList_iter it = newclasses.begin(); it != newclasses.end(); it++)
    {
        if (!strcasecmp(name, (*it)->classname) )
            return *it;
    }

    return NULL;
}

size_t call_method(NC_STACK_nucleus *a1, int a2, void *a3)
{
    if ( a1 )
    {
        //printf("call_method %s (%s) %d\n", a1->getClassName(), a1->NAME, a2);
        return a1->compatcall(a2, a3);
    }

    ypa_log_out("ERROR: Method (%d) invocation on NULL Object!\n", a2);
    return 0;
}

size_t call_method(NC_STACK_nucleus *a1, int a2)
{
    if ( a1 )
    {
        //printf("call_method %s (%s) %d\n", a1->getClassName(), a1->NAME, a2);
        return a1->compatcall(a2, NULL);
    }

    ypa_log_out("ERROR: Method (%d) invocation on NULL Object!\n", a2);
    return 0;
}

size_t call_vtbl(NC_STACK_nucleus *a1, int idx, ...)
{
    stack_vals vals[128];

    va_list va;

    va_start(va, idx);
    va_to_arr(vals, 128, va);
    va_end(va);

    if ( a1 )
    {
        return a1->compatcall(idx, vals);
    }
    else
    {
        ypa_log_out("ERROR: Method invocation on NULL Object!\n");
        return 0;
    }
    return 0;
}

NC_STACK_nucleus * init_get_class(const char *classname, ...)
{
    stack_vals vals[128];

    va_list va;

    va_start(va, classname);
    va_to_arr(vals, 128, va);
    va_end(va);

    const NewClassDescr *cls_descr = FIND2(&newclasses, classname);

    if (!cls_descr)
        return NULL;

    NC_STACK_nucleus *class_examplar = cls_descr->newinstance();

    //class object constructor
    if ( !class_examplar->func0(vals) )
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
            a2 = (stack_vals *)a2->value;
        else if ( a2->id == 3 )
            a2 += a2->value;
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
            return a3->value;
        if ( a3->id == 0 )
            return def_value;

        if ( a3->id == 2 )
        {
            if (a3->value == 0)
                printf("find_id_in_stack_def_val, stack NULL pointer\n");
            else
                a3 = (stack_vals *)a3->value;
        }
        else if ( a3->id == 3 )
            a3 += a3->value;
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

            clss = FIND2(&newclasses, classname); // get_class(classname);

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
