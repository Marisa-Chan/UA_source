#include "includes.h"
#include "nucleas.h"
#include "ade.h"
#include "utils.h"


const NewClassDescr NC_STACK_ade::description("ade.class", &newinstance);


void ade_func0__sub0(__NC_STACK_ade *ade, stack_vals *stak)
{
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

            case 0x80001003:
                if ( stk->value )
                    ade->flags |= 4;
                else
                    ade->flags &= 0xFB;
                break;
            case 0x80001004:
                if ( stk->value )
                    ade->flags |= 2;
                else
                    ade->flags &= 0xFD;
                break;
            case 0x80001007:
                ade->strc_f4 = stk->value;
                break;
            case 0x80001008:
                ade->strc_f6 = stk->value;
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

    __NC_STACK_ade *ade = &this->stack__ade;

    ade->self = this;
    ade_func0__sub0(ade, stak);

    return 1;
}

size_t NC_STACK_ade::func1(stack_vals *stak)
{
    __NC_STACK_ade *ade = &this->stack__ade;

    if ( ade->flags & 1 )
        Remove(ade);

    return NC_STACK_nucleus::func1(stak);
}


void ade_func2__sub0(__NC_STACK_ade *ade, stack_vals *stak)
{

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

            case 0x80001003:
                if ( stk->value )
                    ade->flags |= 4;
                else
                    ade->flags &= 0xFB;
                break;
            case 0x80001004:
                if ( stk->value )
                    ade->flags |= 2;
                else
                    ade->flags &= 0xFD;
                break;
            case 0x80001007:
                ade->strc_f4 = stk->value;
                break;
            case 0x80001008:
                ade->strc_f6 = stk->value;
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_ade::func2(stack_vals *stak)
{
    __NC_STACK_ade *ade = &this->stack__ade;

    ade_func2__sub0(ade, stak);

    return NC_STACK_nucleus::func2(stak);
}

void ade_func3__sub0(__NC_STACK_ade *ade, stack_vals *stak)
{
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

            case 0x80001003:
                if ( !(ade->flags & 4) )
                    *(int *)stk->value = 0;
                else
                    *(int *)stk->value = 1;
                break;
            case 0x80001004:
                if ( ade->flags & 2 )
                    *(int *)stk->value = 1;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80001007:
                *(int *)stk->value = ade->strc_f4;
                break;
            case 0x80001008:
                *(int *)stk->value = ade->strc_f6;
                break;
            case 0x8000100B:
                *(__NC_STACK_ade **)stk->value = ade;
                break;
            }
            stk++;
        }
    }
}

size_t NC_STACK_ade::func3(stack_vals *stak)
{

    __NC_STACK_ade *ade = &this->stack__ade;

    ade_func3__sub0(ade, stak);

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
                    stack_vals stk[5];

                    stk[0].id = 0x80001004;
                    stk[0].value = (hdr.field_3 & 2) != 0;
                    stk[1].id = 0x80001003;
                    stk[1].value = (hdr.field_3 & 4) != 0;
                    stk[2].id = 0x80001007;
                    stk[2].value = hdr.field_4;
                    stk[3].id = 0x80001008;
                    stk[3].value = hdr.field_6;
                    stk[4].id = 0;

                    func2(stk);
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
    __NC_STACK_ade *ade = &this->stack__ade;

    if ( sub_412FC0(mfile, TAG_ADE, TAG_FORM, -1) )
        return 0;

    if ( !NC_STACK_nucleus::func6(file) )
        return 0;

    sub_412FC0(mfile, 0, TAG_STRC, -1);

    ADE_STRC hdr;
    hdr.field_0 = SWAP16(1);
    hdr.field_2 = 0;
    hdr.field_3 = ade->flags & 6;
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
    __NC_STACK_ade *ade = &this->stack__ade;

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
