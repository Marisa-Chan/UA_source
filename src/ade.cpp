#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"



size_t NC_STACK_ade::Init(IDVList &stak)
{
    if ( !NC_STACK_nucleus::Init(stak) )
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

size_t NC_STACK_ade::Deinit()
{
    if ( AttachedTo )
    {
        AttachedTo->remove(this);
        AttachedTo = NULL;
    }

    return NC_STACK_nucleus::Deinit();
}

size_t NC_STACK_ade::LoadingFromIFF(IFFile **file)
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
                Deinit();
            return 0;
        }

        const IFFile::Context &chunk = mfile->GetCurrentChunk();

        if ( chunk.Is(TAG_FORM, TAG_ROOT) )
        {
            obj_ok = NC_STACK_nucleus::LoadingFromIFF(file);

            if ( !obj_ok )
                break;
        }
        else if ( chunk.Is(TAG_STRC) )
        {
            if ( obj_ok )
            {
                ADE_STRC hdr;

                hdr.version = mfile->readS16B();
                hdr._nu1 = mfile->readS8();
                hdr.flags = mfile->readS8();
                hdr.point = mfile->readS16B();
                hdr.poly = mfile->readS16B();
                hdr._nu2 = mfile->readS16B();

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

size_t NC_STACK_ade::SavingIntoIFF(IFFile **file)
{
    IFFile *mfile = *file;

    if ( mfile->pushChunk(TAG_ADE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::SavingIntoIFF(file) )
        return 0;

    mfile->pushChunk(0, TAG_STRC, -1);

    mfile->writeS16B(1);
    mfile->writeS8(0);
    mfile->writeS8( flags & (ADE_FLAG_BKCHECK | ADE_FLAG_DPTHFADE) );
    mfile->writeS16B(_pointID);
    mfile->writeS16B(_polyID);
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
    return _pointID;
}

int NC_STACK_ade::getADE_poly()
{
    return _polyID;
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
    _pointID = arg;
}

void NC_STACK_ade::setADE_poly(int arg)
{
    _polyID = arg;
}

NC_STACK_ade::InstanceOpts *NC_STACK_ade::GenRenderInstance()
{
    return new InstanceOpts(this);
}

