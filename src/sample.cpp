#include "includes.h"
#include "nucleas.h"
#include "sample.h"
#include "utils.h"


size_t NC_STACK_sample::Init(IDVList &stak)
{
    if ( !NC_STACK_rsrc::Init(stak) )
        return 0;

    p_sampl = (TSampleData *)getRsrc_pData();

    return 1;
}

rsrc * NC_STACK_sample::rsrc_func64(IDVList &stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);

    if ( !res )
        return NULL;

    int bufsz = stak.Get<int32_t>(SMPL_ATT_LEN, 0);
    int type = stak.Get<int32_t>(SMPL_ATT_TYPE, 0xFFFF);

    if ( bufsz == 0 || type == 0xFFFF )
        return res;

    TSampleData *smpl = new TSampleData();

    if ( !smpl )
        return res;

    smpl->bufsz = bufsz;
    smpl->field_8 = type;

    void *buf = stak.Get<void *>(SMPL_ATT_BUFFER, NULL);

    if ( !buf )
    {
        smpl->_Buffer.resize(bufsz);
        smpl->Data = smpl->_Buffer.data();
    }
    else
    {
        smpl->Data = buf;
        smpl->field_10 |= 1;
    }

    res->data = smpl;

    return res;
}

size_t NC_STACK_sample::rsrc_func65(rsrc *res)
{
    TSampleData *smpl = (TSampleData *)res->data;

    if ( smpl )
    {
        delete smpl;
        res->data = NULL;
    }

    return NC_STACK_rsrc::rsrc_func65(res);
}


TSampleData *NC_STACK_sample::GetSampleData()
{
    return p_sampl;
}

int NC_STACK_sample::getSMPL_type()
{
    if (p_sampl)
        return p_sampl->field_8;
    return 0;
}

int NC_STACK_sample::getSMPL_len()
{
    if (p_sampl)
        return p_sampl->bufsz;
    return 0;
}

void *NC_STACK_sample::getSMPL_buffer()
{
    if (p_sampl)
        return p_sampl->Data;
    return 0;
}



