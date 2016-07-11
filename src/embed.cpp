#include "includes.h"
#include "nucleas.h"

#include "embed.h"
#include "utils.h"

#include "rsrc.h"

#include "math.h"
#include "inttypes.h"



const NewClassDescr NC_STACK_embed::description("embed.class", &newinstance);



int sub_41C418(nlist *list, NC_STACK_rsrc *obj)
{
    embd_node *v4 = (embd_node *)list->tailpred;
    embd_node *v5 = NULL;

    if ( v4->prev )
    {
        if ( v4->num < 32 )
            v5 = v4;
    }

    if ( !v5 )
    {
        v5 = (embd_node *)AllocVec(sizeof(embd_node), 65537);
        if ( !v5 )
            return 0;
        AddTail(list, v5);
    }

    v5->objects[v5->num] = obj;
    v5->num++;
    return 1;
}

// Create embed resource node and fill rsrc field data
size_t NC_STACK_embed::func0(stack_vals *)
{
    dprintf("MAKE ME %s\n","embed_func0");
    return 0;
}

size_t NC_STACK_embed::func1(stack_vals *stak)
{
    __NC_STACK_embed *embd = &this->stack__embed;

    while ( embd->embed_objects.head->next )
    {
        embd_node *nod = (embd_node *)embd->embed_objects.head;
        for(int i = 0; i < nod->num; i++)
        {
            NC_STACK_nucleus *embd_obj = nod->objects[i];
            if (embd_obj)
                delete_class_obj(embd_obj);
        }
        Remove(nod);
        nc_FreeMem(nod);
    }
    return NC_STACK_nucleus::func1(stak);
}

size_t NC_STACK_embed::func5(MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_embed *embd = NULL;
    int obj_ok = 0;

    while ( 1 )
    {
        int v5 = read_next_IFF(mfile, 2);

        if ( v5 == -2 )
            break;

        if ( v5 )
        {
            if ( obj_ok )
                func1(NULL);
            return 0;
        }

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj_ok = NC_STACK_nucleus::func5(file);

            if ( !obj_ok )
                return 0;

            embd = &this->stack__embed;
            init_list(&embd->embed_objects);
        }
        else if ( chunk->TAG == TAG_EMRS )
        {
            char classname[256];

            mfread(mfile, classname, 255);
            read_next_IFF(mfile, 2);

            NC_STACK_rsrc *embd_class = dynamic_cast<NC_STACK_rsrc *>( init_get_class(classname,
                                        0x80001000, &classname[strlen(classname) + 1],
                                        0x80001001, 1,
                                        0x80001005, mfile,
                                        0) );

            if ( embd_class && !sub_41C418(&embd->embed_objects, embd_class) )
            {
                delete_class_obj(embd_class);
                func1(NULL);
                return 0;
            }
        }
        else
        {
            read_default(mfile);
        }

    }
    return obj_ok;
}

size_t NC_STACK_embed::func6(MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_embed *embd = &this->stack__embed;

    if ( sub_412FC0(mfile, TAG_EMBD, TAG_FORM, -1) )
        return 0;

    if ( NC_STACK_nucleus::func6(file) )
    {
        embd_node *node = (embd_node *)embd->embed_objects.head;

        if ( node->next )
        {
            while ( node )
            {
                if ( node->num )
                {
                    for (int i = 0; i < node->num; i++)
                    {
                        NC_STACK_rsrc *embd_obj = node->objects[i];
                        if ( embd_obj )
                        {
                            char v23 = 0;
                            char *classname, *resname;
                            call_vtbl(embd_obj, 3, 0x80000002, &classname, 0);
                            call_vtbl(embd_obj, 3, 0x80001000, &resname, 0);

                            sub_412FC0(mfile, 0, TAG_EMRS, -1);
                            sub_413564(mfile, strlen(classname) + 1, classname);
                            sub_413564(mfile, strlen(resname) + 1, resname);
                            sub_413564(mfile, 1, &v23);
                            sub_413290(mfile);

                            rsrc_func66_arg arg66;
                            arg66.filename = NULL;
                            arg66.file = mfile;
                            arg66.OpenedStream = 2;

                            if ( !embd_obj->rsrc_func66(&arg66 ) )
                                return 0;
                        }
                    }
                }

                node = (embd_node *)node->next;
            }
        }
        return sub_413290(mfile) == 0;
    }
    else
        return 0;
    return 0;
}

size_t NC_STACK_embed::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 5:
        return (size_t)func5( (MFILE **)data );
    case 6:
        return (size_t)func6( (MFILE **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
