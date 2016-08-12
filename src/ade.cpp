#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"


const NewClassDescr NC_STACK_ade::description("ade.class", &newinstance);


void NC_STACK_ade::ade_func0__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case ADE_ATT_BKCHECK:
                setADE_bkCheck(stk->value.i_data);
                break;
            case ADE_ATT_DPTHFADE:
                setADE_depthFade(stk->value.i_data);
                break;
            case ADE_ATT_POINT:
                setADE_point(stk->value.i_data);
                break;
            case ADE_ATT_POLY:
                setADE_poly(stk->value.i_data);
                break;
            }
            stk++;
        }
    }
}


size_t NC_STACK_ade::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    __NC_STACK_ade *ade = &stack__ade;

    ade->self = this;
    ade_func0__sub0(stak);

    return 1;
}

size_t NC_STACK_ade::func1(stack_vals *stak)
{
    __NC_STACK_ade *ade = &stack__ade;

    if ( ade->flags & ADE_FLAG_INLIST )
        Remove(ade);

    return NC_STACK_nucleus::func1(stak);
}


void NC_STACK_ade::ade_func2__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case ADE_ATT_BKCHECK:
                setADE_bkCheck(stk->value.i_data);
                break;
            case ADE_ATT_DPTHFADE:
                setADE_depthFade(stk->value.i_data);
                break;
            case ADE_ATT_POINT:
                setADE_point(stk->value.i_data);
                break;
            case ADE_ATT_POLY:
                setADE_poly(stk->value.i_data);
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_ade::func2(stack_vals *stak)
{
    ade_func2__sub0(stak);

    return NC_STACK_nucleus::func2(stak);
}

void NC_STACK_ade::ade_func3__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case ADE_ATT_BKCHECK:
                *(int *)stk->value.p_data = getADE_bkCheck();
                break;
            case ADE_ATT_DPTHFADE:
                *(int *)stk->value.p_data = getADE_depthFade();
                break;
            case ADE_ATT_POINT:
                *(int *)stk->value.p_data = getADE_point();
                break;
            case ADE_ATT_POLY:
                *(int *)stk->value.p_data = getADE_poly();
                break;
            case ADE_ATT_PADE:
                *(__NC_STACK_ade **)stk->value.p_data = getADE_pAde();
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_ade::func3(stack_vals *stak)
{
    ade_func3__sub0(stak);

    return NC_STACK_nucleus::func3(stak);
}


size_t NC_STACK_ade::func5(MFILE **file)
{
    MFILE *mfile = *file;
    int obj_ok = 0;
    while ( 1 )
    {
        int iff_res = read_next_IFF(mfile, 2);

        if ( iff_res == -2 )
            break;

        if ( iff_res )
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
                break;

            this->stack__ade.self = this;

        }
        else if ( chunk->TAG == TAG_STRC )
        {
            if ( obj_ok )
            {
                ADE_STRC hdr;

                mfread(mfile, &hdr, sizeof(ADE_STRC));

                hdr.field_0 = SWAP16(hdr.field_0);
                hdr.field_4 = SWAP16(hdr.field_4);
                hdr.field_6 = SWAP16(hdr.field_6);

                if ( hdr.field_0 >= 1 )
                {
                    setADE_depthFade( (hdr.field_3 & ADE_FLAG_DPTHFADE) != 0 );
                    setADE_bkCheck( (hdr.field_3 & ADE_FLAG_BKCHECK) != 0 );
                    setADE_point( hdr.field_4 );
                    setADE_poly( hdr.field_6 );
                }
            }
            read_next_IFF(mfile, 2);
        }
        else
        {
            read_default(mfile);
        }
    }

    return obj_ok;
}

size_t NC_STACK_ade::func6(MFILE **file)
{
    MFILE *mfile = *file;
    __NC_STACK_ade *ade = &stack__ade;

    if ( sub_412FC0(mfile, TAG_ADE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    ADE_STRC hdr;
    hdr.field_0 = SWAP16(1);
    hdr.field_2 = 0;
    hdr.field_3 = ade->flags & (ADE_FLAG_BKCHECK | ADE_FLAG_DPTHFADE);
    hdr.field_4 = SWAP16(ade->strc_f4);
    hdr.field_6 = SWAP16(ade->strc_f6);
    hdr.field_8 = 0;

    sub_413564(mfile, sizeof(ADE_STRC), &hdr);
    sub_413290(mfile);
    return sub_413290(mfile) == 0;
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
    return stack__ade.strc_f4;
}

int NC_STACK_ade::getADE_poly()
{
    return stack__ade.strc_f6;
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
    stack__ade.strc_f4 = arg;
}

void NC_STACK_ade::setADE_poly(int arg)
{
    stack__ade.strc_f6 = arg;
}


size_t NC_STACK_ade::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return (size_t)func2( (stack_vals *)data );
    case 3:
        return (size_t)func3( (stack_vals *)data );
    case 5:
        return (size_t)func5( (MFILE **)data );
    case 6:
        return (size_t)func6( (MFILE **)data );
    case 64:
        return (size_t)ade_func64( (nlist **)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
