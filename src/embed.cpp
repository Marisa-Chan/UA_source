#include "includes.h"
#include "nucleas.h"

#include "embed.h"
#include "utils.h"

#include "rsrc.h"

#include "math.h"
#include "inttypes.h"



const Nucleus::ClassDescr NC_STACK_embed::description("embed.class", &newinstance);


// Create embed resource node and fill rsrc field data
size_t NC_STACK_embed::func0(IDVList &)
{
    dprintf("MAKE ME %s\n","embed_func0");
    return 0;
}

size_t NC_STACK_embed::func1()
{
    for ( NC_STACK_rsrc *res : _resources )
        Nucleus::Delete(res);
    
    _resources.clear();
    
    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_embed::func5(IFFile **file)
{
    IFFile *mfile = *file;
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

            _resources.clear();
        }
        else if ( chunk->TAG == TAG_EMRS )
        {
            std::string classname = mfile->readStr(255);
            mfile->parse();
            
            std::string resname;
            size_t fnd = classname.find('\0');
            if (fnd != std::string::npos)
            {
                resname = classname.substr(fnd + 1);
                classname.resize(fnd);
            }
            if (classname.back() == '\0')
                classname.pop_back();
            if (resname.back() == '\0')
                resname.pop_back();

            NC_STACK_rsrc *embd_class = Nucleus::CTFInit<NC_STACK_rsrc>(classname,
               {{NC_STACK_rsrc::RSRC_ATT_NAME, resname},
                {NC_STACK_rsrc::RSRC_ATT_TRYSHARED, (int32_t)1},
                {NC_STACK_rsrc::RSRC_ATT_PIFFFILE, mfile}});

            if ( !embd_class )
            {
                func1();
                return 0;
            }
            
            _resources.push_back(embd_class);
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

    if ( mfile->pushChunk(TAG_EMBD, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;
    
    for ( NC_STACK_rsrc *embd_obj : _resources )
    {
        if ( embd_obj )
        {
            std::string classname = embd_obj->GetClassName();
            std::string resname = embd_obj->getRsrc_name();

            mfile->pushChunk(0, TAG_EMRS, -1);
            mfile->write(classname.c_str(), classname.length() + 1);
            mfile->write(resname.c_str(), resname.length() + 1);
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
    
    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

