#include "includes.h"
#include "nucleas.h"
#include "sample.h"
#include "utils.h"


const NewClassDescr NC_STACK_sample::description("sample.class", &newinstance);


size_t NC_STACK_sample::func0(stack_vals *stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    __NC_STACK_sample *smpl = &this->stack__sample;

    call_vtbl(this, 3, 0x80001002, &smpl->p_sampl, 0);

    return 1;
}

size_t NC_STACK_sample::func3(stack_vals *stak)
{
    __NC_STACK_sample *sample = &this->stack__sample;

    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == 0)
            break;
        else if (stk->id == 2)
        {
            stk = (stack_vals *)stk->value;
        }
        else if ( stk->id == 3 )
        {
            stk += stk->value;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case 0x80002000:
                *(sampl **)stk->value = sample->p_sampl;
                break;
            case 0x80002001:
                if ( sample->p_sampl )
                    *(int *)stk->value = sample->p_sampl->field_8;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80002002:
                if ( sample->p_sampl )
                    *(int *)stk->value = sample->p_sampl->bufsz;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80002003:
                if ( sample->p_sampl )
                    *(void **)stk->value = sample->p_sampl->sample_buffer;
                else
                    *(void **)stk->value = NULL;
                break;
            }
            stk++;
        }
    }

    return NC_STACK_rsrc::func3(stak);
}


rsrc * NC_STACK_sample::rsrc_func64(stack_vals *stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);

    if ( !res )
        return NULL;

    int bufsz = find_id_in_stack_def_val(0x80002002, 0, stak);
    int type = find_id_in_stack_def_val(0x80002001, 0xFFFF, stak);

    if ( bufsz == 0 || type == 0xFFFF )
        return res;

    sampl *smpl = (sampl *)AllocVec(sizeof(sampl), 65537);

    if ( !smpl )
        return res;

    smpl->bufsz = bufsz;
    smpl->field_8 = type;

    void *buf = (void *)find_id_in_stack_def_val(0x80002003, 0, stak);

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

size_t NC_STACK_sample::rsrc_func65(rsrc **pres)
{
    rsrc *res = *pres;
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

    return NC_STACK_rsrc::rsrc_func65(pres);
}

void * NC_STACK_sample::sample_func128(void **arg)
{
    printf("%s - NOT RECOGINZED ARGUMENT\n","sample_func128");
    sampl *smpl = this->stack__sample.p_sampl;
    arg[2] = smpl;
    return smpl;
}

size_t NC_STACK_sample::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 65:
        return rsrc_func65( (rsrc **)data );
    case 128:
        return (size_t)sample_func128( (void **)data );
    default:
        break;
    }
    return NC_STACK_rsrc::compatcall(method_id, data);
}
