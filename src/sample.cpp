#include "includes.h"
#include "nucleas.h"
#include "sample.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_sample::description("sample.class", &newinstance);


size_t NC_STACK_sample::func0(IDVList &stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    stack__sample.p_sampl = (sampl *)getRsrc_pData();

    return 1;
}

size_t NC_STACK_sample::func3(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case SMPL_ATT_PSAMPLE:
                *(sampl **)val.value.p_data = getSMPL_pSample();
                break;
            case SMPL_ATT_TYPE:
                *(int *)val.value.p_data = getSMPL_type();
                break;
            case SMPL_ATT_LEN:
                *(int *)val.value.p_data = getSMPL_len();
                break;
            case SMPL_ATT_BUFFER:
                *(void **)val.value.p_data = getSMPL_buffer();
                break;

            default:
                break;
            }
        }
    }

    return NC_STACK_rsrc::func3(stak);
}


rsrc * NC_STACK_sample::rsrc_func64(IDVList &stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);

    if ( !res )
        return NULL;

    int bufsz = stak.Get(SMPL_ATT_LEN, 0);
    int type = stak.Get(SMPL_ATT_TYPE, 0xFFFF);

    if ( bufsz == 0 || type == 0xFFFF )
        return res;

    sampl *smpl = (sampl *)AllocVec(sizeof(sampl), 65537);

    if ( !smpl )
        return res;

    smpl->bufsz = bufsz;
    smpl->field_8 = type;

    void *buf = (void *)stak.GetPointer(SMPL_ATT_BUFFER, NULL);

    if ( !buf )
    {
        buf = AllocVec(bufsz, 65537);
        smpl->sample_buffer = buf;

        if ( !buf )
        {
            nc_FreeMem(smpl);
            return res;
        }
    }
    else
    {
        smpl->sample_buffer = buf;
        smpl->field_10 |= 1;
    }

    res->data = smpl;

    return res;
}

size_t NC_STACK_sample::rsrc_func65(rsrc *res)
{
    sampl *smpl = (sampl *)res->data;

    if ( smpl )
    {
        if ( !(smpl->field_10 & 1) )
        {
            if ( smpl->sample_buffer )
                nc_FreeMem(smpl->sample_buffer);
        }
        nc_FreeMem(smpl);
        res->data = NULL;
    }

    return NC_STACK_rsrc::rsrc_func65(res);
}

void * NC_STACK_sample::sample_func128(void **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","sample_func128");
    sampl *smpl = stack__sample.p_sampl;
    arg[2] = smpl;
    return smpl;
}



sampl *NC_STACK_sample::getSMPL_pSample()
{
    return stack__sample.p_sampl;
}

int NC_STACK_sample::getSMPL_type()
{
    if (stack__sample.p_sampl)
        return stack__sample.p_sampl->field_8;
    return 0;
}

int NC_STACK_sample::getSMPL_len()
{
    if (stack__sample.p_sampl)
        return stack__sample.p_sampl->bufsz;
    return 0;
}

void *NC_STACK_sample::getSMPL_buffer()
{
    if (stack__sample.p_sampl)
        return stack__sample.p_sampl->sample_buffer;
    return 0;
}



