#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_ade::description("ade.class", &newinstance);


size_t NC_STACK_ade::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case ADE_ATT_BKCHECK:
                setADE_bkCheck(val.Get<int32_t>());
                break;
            case ADE_ATT_DPTHFADE:
                setADE_depthFade(val.Get<int32_t>());
                break;
            case ADE_ATT_POINT:
                setADE_point(val.Get<int32_t>());
                break;
            case ADE_ATT_POLY:
                setADE_poly(val.Get<int32_t>());
                break;
            default:
                break;
            }
        }
    }

    return 1;
}

size_t NC_STACK_ade::func1()
{
    if ( AttachedTo )
    {
        AttachedTo->remove(this);
        AttachedTo = NULL;
    }

    return NC_STACK_nucleus::func1();
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

    if ( mfile->pushChunk(TAG_ADE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1);
    mfile->writeS8(0);
    mfile->writeS8( flags & (ADE_FLAG_BKCHECK | ADE_FLAG_DPTHFADE) );
    mfile->writeS16B(point);
    mfile->writeS16B(poly);
    mfile->writeS16B(0);

    mfile->popChunk();
    return mfile->popChunk() == IFFile::IFF_ERR_OK;
}

// Add ade to list
size_t NC_STACK_ade::ade_func64(AdeList &lst)
{
    if ( AttachedTo )
    {
        AttachedTo->remove(this);
        AttachedTo = NULL;
    }

    lst.push_back(this);
    AttachedTo = &lst;

    return 1;
}

size_t NC_STACK_ade::ade_func65(area_arg_65 *arg)
{
    return 1;
}



int NC_STACK_ade::getADE_bkCheck()
{
    if ( (flags & ADE_FLAG_BKCHECK) )
        return 1;
    return 0;
}

int NC_STACK_ade::getADE_depthFade()
{
    if ( (flags & ADE_FLAG_DPTHFADE) )
        return 1;
    return 0;
}

int NC_STACK_ade::getADE_point()
{
    return point;
}

int NC_STACK_ade::getADE_poly()
{
    return poly;
}

//__NC_STACK_ade *NC_STACK_ade::getADE_pAde()
//{
//    return &stack__ade;
//}



void NC_STACK_ade::setADE_bkCheck(int arg)
{
    if ( arg )
        flags |= ADE_FLAG_BKCHECK;
    else
        flags &= ~ADE_FLAG_BKCHECK;
}

void NC_STACK_ade::setADE_depthFade(int arg)
{
    if ( arg )
        flags |= ADE_FLAG_DPTHFADE;
    else
        flags &= ~ADE_FLAG_DPTHFADE;
}

void NC_STACK_ade::setADE_point(int arg)
{
    point = arg;
}

void NC_STACK_ade::setADE_poly(int arg)
{
    poly = arg;
}

