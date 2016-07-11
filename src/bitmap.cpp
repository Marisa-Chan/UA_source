#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "utils.h"


const NewClassDescr NC_STACK_bitmap::description("bitmap.class", &newinstance);

unsigned int sub_416704(NC_STACK_bitmap *a1, __NC_STACK_bitmap *a2, bitmap__opl *a3)
{
    //// WHAT IT THIS !?
    size_t a4 = 0;

    call_vtbl(a1, 3, 0x80001003, &a4, 0);

    if ( a2->field_4.opl2 )
    {
        if ( !a4 )
            nc_FreeMem(a2->field_4.opl2);
    }

    if ( a4 )
    {
        a2->field_4.opl1 = a3;
        return 1;
    }

    int opl_count = 1; //Mandatory end (tu/tv = -1/-1)

    bitmap__opl *opl_cur = a3;

    while (opl_cur->field_E >= 0)
    {
        opl_count++;
        opl_cur++;
    }

    tUtV *unk = (tUtV *)AllocVec(sizeof(tUtV) * opl_count, 1);

    if ( unk )
    {
        tUtV *tmp = unk;
        bitmap__opl *opl = a3;

        for (int i = 0; i < (opl_count - 1); i++)
        {
            tmp[i].tu = (float)opl[i].field_0 * (1.0 / 256.0);
            tmp[i].tv = (float)opl[i].field_2 * (1.0 / 256.0);
        }

        tmp[opl_count - 1].tu = -1;
        tmp[opl_count - 1].tv = -1;

        a2->field_4.opl2 = unk;

        return 1;
    }
    return 0;
}


size_t NC_STACK_bitmap::func0(stack_vals *stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    __NC_STACK_bitmap *internal = &this->stack__bitmap;

    void *v9 = (void *)find_id_in_stack_def_val(0x80002001, 0, stak);
    if ( v9 )
        sub_416704(this, internal, (bitmap__opl *)v9);

    call_vtbl(this, 3, 0x80001002, &internal->bitm_intern, 0); // Copy rsrc->data to bitm_intern

    return 1;
}

size_t NC_STACK_bitmap::func1(stack_vals *stak)
{
    __NC_STACK_bitmap *internal = &this->stack__bitmap;

    if ( internal->field_4.opl2 )
    {
        size_t a4 = 0;

        call_vtbl(this, 3, 0x80001003, &a4, 0);

        if ( !a4 )
            nc_FreeMem(internal->field_4.opl2);
    }

    return NC_STACK_rsrc::func1(stak);
}

size_t NC_STACK_bitmap::func2(stack_vals *stak)
{
    __NC_STACK_bitmap *internal = &this->stack__bitmap;

    stack_vals *v5 = find_id_in_stack2(0x80002001, stak);
    if ( v5 )
        sub_416704(this, internal, (bitmap__opl *)v5->value);

    stack_vals *ppal = find_id_in_stack2(0x80002007, stak);
    if ( ppal )
    {
        if ( internal->bitm_intern->pallete )
            memcpy(internal->bitm_intern->pallete, (void *)ppal->value, 256 * 3);
    }

    return NC_STACK_rsrc::func2(stak);
}

size_t NC_STACK_bitmap::func3(stack_vals *stak)
{
    __NC_STACK_bitmap *internal = &this->stack__bitmap;

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
                *(bitmap_intern **)stk->value = internal->bitm_intern;
                break;
            case 0x80002001:
                *(void **)stk->value = 0;
                break;
            case 0x80002002:
                if ( internal->bitm_intern )
                    *(int *)stk->value = internal->bitm_intern->width;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80002003:
                if ( internal->bitm_intern )
                    *(int *)stk->value = internal->bitm_intern->height;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80002005:
                if ( internal->bitm_intern )
                    *(void **)stk->value = internal->bitm_intern->buffer;
                else
                    *(void **)stk->value = 0;
                break;
            case 0x80002006:
                if ( internal->bitm_intern )
                    *(int *)stk->value = internal->bitm_intern->pallete != NULL;
                else
                    *(int *)stk->value = 0;
                break;
            case 0x80002007:
                if ( internal->bitm_intern )
                    *(BYTE **)stk->value = internal->bitm_intern->pallete;
                else
                    *(BYTE **)stk->value = NULL;
                break;
            }
            stk++;
        }
    }
    return NC_STACK_rsrc::func3(stak);
}

// Create bitmap resource node and fill rsrc field data
rsrc * NC_STACK_bitmap::rsrc_func64(stack_vals *stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);// rsrc_func64
    if ( res )
    {
        int width = find_id_in_stack_def_val(0x80002002, 0, stak);
        int height = find_id_in_stack_def_val(0x80002003, 0, stak);
        int v22 = find_id_in_stack_def_val(0x80002006, 0, stak);
        int v20 = find_id_in_stack_def_val(0x80002008, 0, stak);
        int v21 = find_id_in_stack_def_val(0x80002009, 0, stak);// software surface


        if ( (width && height) || v22 )
        {
            bitmap_intern *intern = (bitmap_intern *)AllocVec(sizeof(bitmap_intern), 65537);

            if ( intern )
            {
                if (v22)
                {
                    intern->flags |= BITMAP_FLAG_HAS_PALETTE;
                    intern->pallete = (BYTE *)AllocVec(256 * 3, 65537);
                }

                if ( !v22 || intern->pallete != NULL )
                {
                    if ( width && height )
                    {
                        intern->width = width;
                        intern->height = height;
                        if ( v20 && engines.display___win3d )
                        {
                            intern->width = width;
                            intern->flags = intern->flags | 2;
                            intern->height = height;

                            if ( v21 )
                                intern->flags = intern->flags | BITMAP_FLAG_SOFTWARE;

                            // allocate buffer, create palette, surface and texture
                            if ( !engines.display___win3d->display_func266(&intern) )
                            {
                                nc_FreeMem(intern);
                                return res;
                            }
                        }
                        else
                        {
                            void *buf = (void *)find_id_in_stack_def_val(0x80002005, 0, stak);
                            intern->pitch = width;
                            if ( buf )
                            {
                                intern->buffer = buf;
                                intern->flags = intern->flags | 1;
                            }
                            else
                            {
                                intern->buffer = AllocVec(height * width, 65537);
                                if ( !intern->buffer )
                                {
                                    nc_FreeMem(intern);
                                    return res;
                                }
                            }
                        }
                    }
                    res->data = intern;
                }
                else
                {
                    nc_FreeMem(intern);
                }
            }
        }
    }
    return res;
}

size_t NC_STACK_bitmap::rsrc_func65(rsrc **pres)
{
    bitmap_intern *intern = (bitmap_intern *)(*pres)->data;
    rsrc *res = *pres;
    if ( intern )
    {
        if ( intern->flags & 2 && engines.display___win3d )
        {
            engines.display___win3d->display_func268(&intern);
        }
        else if ( !(intern->flags & 1) )
        {
            if ( intern->buffer )
                nc_FreeMem(intern->buffer);
        }

        if ( intern->pallete )
            nc_FreeMem(intern->pallete);

        nc_FreeMem(intern);
        res->data = NULL;
    }

    return NC_STACK_rsrc::rsrc_func65(pres);
}

size_t NC_STACK_bitmap::bitmap_func128(stack_vals *)
{
    return 0;
}

size_t NC_STACK_bitmap::bitmap_func129(stack_vals *)
{
    return 0;
}

void NC_STACK_bitmap::bitmap_func130(bitmap_arg130 *out)
{
    __NC_STACK_bitmap *bitm = &this->stack__bitmap;
    out->pbitm = bitm->bitm_intern;
    out->opl2 = bitm->field_4.opl2;
}


size_t NC_STACK_bitmap::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 3:
        return func3( (stack_vals *)data );
    case 64:
        return (size_t)rsrc_func64( (stack_vals *)data );
    case 65:
        return rsrc_func65( (rsrc **)data );
    case 128:
        return (size_t)bitmap_func128( (stack_vals *)data );
    case 129:
        return (size_t)bitmap_func129( (stack_vals *)data );
    case 130:
        bitmap_func130( (bitmap_arg130 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_rsrc::compatcall(method_id, data);
}
