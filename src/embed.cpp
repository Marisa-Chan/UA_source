#include "includes.h"
#include "nucleas.h"

#include "embed.h"
#include "utils.h"

#include "rsrc.h"

#include "math.h"
#include "inttypes.h"


stored_functions *classvtbl_get_embed();
class_return * class_set_embed(int, ...);

stored_functions embed_class_vtbl(class_set_embed);


class_stored embed_class_off (NULL, NULL, "MC2classes:embed.class", classvtbl_get_embed);


stored_functions *classvtbl_get_embed()
{
    return &embed_class_vtbl;
}

CLASSFUNC embed_funcs[1024];

int sub_41C418(nlist *list, NC_STACK_class *obj)
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
NC_STACK_embed * embed_func0(class_stru *, class_stru *, stack_vals *)
{
    dprintf("MAKE ME %s\n","embed_func0");
    return NULL;
}

size_t embed_func1(NC_STACK_embed *obj, class_stru *zis, stack_vals *stak)
{
    __NC_STACK_embed *embd = &obj->stack__embed;

    while ( embd->embed_objects.head->next )
    {
        embd_node *nod = (embd_node *)embd->embed_objects.head;
        for(int i = 0; i < nod->num; i++)
        {
            NC_STACK_class *embd_obj = nod->objects[i];
            if (embd_obj)
                delete_class_obj(embd_obj);
        }
        Remove(nod);
        nc_FreeMem(nod);
    }
    return call_parent(zis, obj, 1, stak);
}

NC_STACK_embed * embed_func5(class_stru *clss, class_stru *zis, MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_embed *embd = NULL;
    NC_STACK_embed *obj = NULL;

    while ( 1 )
    {
        int v5 = read_next_IFF(mfile, 2);

        if ( v5 == -2 )
            break;

        if ( v5 )
        {
            if ( obj )
                call_method(obj, 1);
            return NULL;
        }

        MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj = (NC_STACK_embed *)call_parent(zis, clss, 5, (stack_vals *)file);

            if ( !obj )
                return NULL;

            embd = &obj->stack__embed;
            init_list(&embd->embed_objects);
        }
        else if ( chunk->TAG == TAG_EMRS )
        {
            char classname[256];

            mfread(mfile, classname, 255);
            read_next_IFF(mfile, 2);

            NC_STACK_class *embd_class = init_get_class(classname,
                                         0x80001000, &classname[strlen(classname) + 1],
                                         0x80001001, 1,
                                         0x80001005, mfile,
                                         0);

            if ( embd_class && !sub_41C418(&embd->embed_objects, embd_class) )
            {
                delete_class_obj(embd_class);
                call_method(obj, 1);
                return NULL;
            }
        }
        else
        {
            read_default(mfile);
        }

    }
    return obj;
}

size_t embed_func6(NC_STACK_embed *obj, class_stru *zis, MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_embed *embd = &obj->stack__embed;

    if ( sub_412FC0(mfile, TAG_EMBD, TAG_FORM, -1) )
        return 0;

    if ( call_parent(zis, obj, 6, (stack_vals *)file) )
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
                        NC_STACK_class *embd_obj = node->objects[i];
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

                            if ( !call_method(embd_obj, 66, &arg66 ) )
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

class_return embed_class_descr;

class_return * class_set_embed(int, ...)
{
    memset(embed_funcs, 0, sizeof(CLASSFUNC) * 1024);

    embed_funcs[0] = (CLASSFUNC)embed_func0;
    embed_funcs[1] = (CLASSFUNC)embed_func1;
    embed_funcs[5] = (CLASSFUNC)embed_func5;
    embed_funcs[6] = (CLASSFUNC)embed_func6;

    embed_class_descr.parent = "nucleus.class";

    embed_class_descr.vtbl = embed_funcs;
    ////embed_class_descr.varSize = sizeof(__NC_STACK_embed);
    embed_class_descr.varSize = sizeof(NC_STACK_embed) - offsetof(NC_STACK_embed, stack__embed); //// HACK
    embed_class_descr.field_A = 0;
    return &embed_class_descr;
}
