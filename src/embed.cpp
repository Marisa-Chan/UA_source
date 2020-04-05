#include "includes.h"
#include "nucleas.h"

#include "embed.h"
#include "utils.h"

#include "rsrc.h"

#include "math.h"
#include "inttypes.h"



const Nucleus::ClassDescr NC_STACK_embed::description("embed.class", &newinstance);



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
size_t NC_STACK_embed::func0(IDVList &)
{
    dprintf("MAKE ME %s\n","embed_func0");
    return 0;
}

size_t NC_STACK_embed::func1()
{
    __NC_STACK_embed *embd = &stack__embed;

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
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_embed::func5(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_embed *embd = NULL;
    int obj_ok = 0;

    while ( 1 )
    {
        int v5 = mfile->parse();

        if ( v5 == -2 )
            break;

        if ( v5 )
        {
            if ( obj_ok )
                func1();
            return 0;
        }

        IFFile::Context *chunk = mfile->getCurrentChunk();

        if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_ROOT )
        {
            obj_ok = NC_STACK_nucleus::func5(file);

            if ( !obj_ok )
                return 0;

            embd = &stack__embed;
            init_list(&embd->embed_objects);
        }
        else if ( chunk->TAG == TAG_EMRS )
        {
            char classname[256];

            mfile->read(classname, 255);
            mfile->parse();

            IDVList init_atts;
            init_atts.Add(NC_STACK_rsrc::RSRC_ATT_NAME, &classname[strlen(classname) + 1]);
            init_atts.Add(NC_STACK_rsrc::RSRC_ATT_TRYSHARED, 1);
            init_atts.Add(NC_STACK_rsrc::RSRC_ATT_PIFFFILE, mfile);

            NC_STACK_rsrc *embd_class = Nucleus::CTFInit<NC_STACK_rsrc>(classname, init_atts);

            if ( embd_class && !sub_41C418(&embd->embed_objects, embd_class) )
            {
                delete_class_obj(embd_class);
                func1();
                return 0;
            }
        }
        else
        {
            mfile->skipChunk();
        }

    }
    return obj_ok;
}

size_t NC_STACK_embed::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_embed *embd = &stack__embed;

    if ( mfile->pushChunk(TAG_EMBD, TAG_FORM, -1) )
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
                            const char *classname, *resname;
                            classname = embd_obj->getClassName();
                            resname = embd_obj->getRsrc_name();

                            mfile->pushChunk(0, TAG_EMRS, -1);
                            mfile->write(classname, strlen(classname) + 1);
                            mfile->write(resname, strlen(resname) + 1);
                            mfile->writeU8(0);
                            mfile->popChunk();

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
        return mfile->popChunk() == IFFile::IFF_ERR_OK;
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
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 5:
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
