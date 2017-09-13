#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"


const NewClassDescr NC_STACK_ade::description("ade.class", &newinstance);


size_t NC_STACK_ade::func0(IDVList *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    __NC_STACK_ade *ade = &stack__ade;

    ade->self = this;

    if (stak)
    {
        for(IDVList::iterator it = stak->begin(); it != stak->end(); it++)
        {
            IDVPair &val = it->second;

            if ( !val.skip() )
            {
                switch (val.id)
                {
                case ADE_ATT_BKCHECK:
                    setADE_bkCheck(val.value.i_data);
                    break;
                case ADE_ATT_DPTHFADE:
                    setADE_depthFade(val.value.i_data);
                    break;
                case ADE_ATT_POINT:
                    setADE_point(val.value.i_data);
                    break;
                case ADE_ATT_POLY:
                    setADE_poly(val.value.i_data);
                    break;
                default:
                    break;
                }
            }
        }
    }

    return 1;
}

size_t NC_STACK_ade::func1()
{
    if ( stack__ade.flags & ADE_FLAG_INLIST )
        Remove(&stack__ade);

    return NC_STACK_nucleus::func1();
}

size_t NC_STACK_ade::func2(IDVList *stak)
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
                case ADE_ATT_BKCHECK:
                    setADE_bkCheck(val.value.i_data);
                    break;
                case ADE_ATT_DPTHFADE:
                    setADE_depthFade(val.value.i_data);
                    break;
                case ADE_ATT_POINT:
                    setADE_point(val.value.i_data);
                    break;
                case ADE_ATT_POLY:
                    setADE_poly(val.value.i_data);
                    break;
                default:
                    break;
                }
            }
        }
    }

    return NC_STACK_nucleus::func2(stak);
}


size_t NC_STACK_ade::func3(IDVList *stak)
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
                case ADE_ATT_BKCHECK:
                    *(int *)val.value.p_data = getADE_bkCheck();
                    break;
                case ADE_ATT_DPTHFADE:
                    *(int *)val.value.p_data = getADE_depthFade();
                    break;
                case ADE_ATT_POINT:
                    *(int *)val.value.p_data = getADE_point();
                    break;
                case ADE_ATT_POLY:
                    *(int *)val.value.p_data = getADE_poly();
                    break;
                case ADE_ATT_PADE:
                    *(__NC_STACK_ade **)val.value.p_data = getADE_pAde();
                    break;
                default:
                    break;
                }
            }
        }
    }

    return NC_STACK_nucleus::func3(stak);
}


size_t NC_STACK_ade::func5(IFFile **file)
{
    IFFile *mfile = *file;
    int obj_ok = 0;
    while ( 1 )
    {
        int iff_res = mfile->parse();

        if ( iff_res == IFFile::IFF_ERR_EOC )
            break;

        if ( iff_res )
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
                break;

            this->stack__ade.self = this;

        }
        else if ( chunk->TAG == TAG_STRC )
        {
            if ( obj_ok )
            {
                ADE_STRC hdr;

                mfile->readS16B(hdr.version);
                mfile->readS8(hdr._nu1);
                mfile->readS8(hdr.flags);
                mfile->readS16B(hdr.point);
                mfile->readS16B(hdr.poly);
                mfile->readS16B(hdr._nu2);

                if ( hdr.version >= 1 )
                {
                    setADE_depthFade( (hdr.flags & ADE_FLAG_DPTHFADE) != 0 );
                    setADE_bkCheck( (hdr.flags & ADE_FLAG_BKCHECK) != 0 );
                    setADE_point( hdr.point );
                    setADE_poly( hdr.poly );
                }
            }
            mfile->parse();
        }
        else
        {
            mfile->skipChunk();
        }
    }

    return obj_ok;
}

size_t NC_STACK_ade::func6(IFFile **file)
{
    IFFile *mfile = *file;
    __NC_STACK_ade *ade = &stack__ade;

    if ( mfile->pushChunk(TAG_ADE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1);
    mfile->writeS8(0);
    mfile->writeS8( ade->flags & (ADE_FLAG_BKCHECK | ADE_FLAG_DPTHFADE) );
    mfile->writeS16B(ade->point);
    mfile->writeS16B(ade->poly);
    mfile->writeS16B(0);

    mfile->popChunk();
    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

// Add ade to list
size_t NC_STACK_ade::ade_func64(nlist **lst)
{
    __NC_STACK_ade *ade = &stack__ade;

    if ( ade->flags & 1 )
        Remove(ade);

    AddTail(*lst, ade);
    ade->flags |= 1;

    return 1;
}

size_t NC_STACK_ade::ade_func65(area_arg_65 *arg)
{
    return 1;
}



int NC_STACK_ade::getADE_bkCheck()
{
    if ( (stack__ade.flags & ADE_FLAG_BKCHECK) )
        return 1;
    return 0;
}

int NC_STACK_ade::getADE_depthFade()
{
    if ( (stack__ade.flags & ADE_FLAG_DPTHFADE) )
        return 1;
    return 0;
}

int NC_STACK_ade::getADE_point()
{
    return stack__ade.point;
}

int NC_STACK_ade::getADE_poly()
{
    return stack__ade.poly;
}

__NC_STACK_ade *NC_STACK_ade::getADE_pAde()
{
    return &stack__ade;
}



void NC_STACK_ade::setADE_bkCheck(int arg)
{
    if ( arg )
        stack__ade.flags |= ADE_FLAG_BKCHECK;
    else
        stack__ade.flags &= ~ADE_FLAG_BKCHECK;
}

void NC_STACK_ade::setADE_depthFade(int arg)
{
    if ( arg )
        stack__ade.flags |= ADE_FLAG_DPTHFADE;
    else
        stack__ade.flags &= ~ADE_FLAG_DPTHFADE;
}

void NC_STACK_ade::setADE_point(int arg)
{
    stack__ade.point = arg;
}

void NC_STACK_ade::setADE_poly(int arg)
{
    stack__ade.poly = arg;
}


size_t NC_STACK_ade::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return (size_t)func2( (IDVList *)data );
    case 3:
        return (size_t)func3( (IDVList *)data );
    case 5:
        return (size_t)func5( (IFFile **)data );
    case 6:
        return (size_t)func6( (IFFile **)data );
    case 64:
        return (size_t)ade_func64( (nlist **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
