#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "utils.h"


const Nucleus::ClassDescr NC_STACK_bitmap::description("bitmap.class", &newinstance);

int NC_STACK_bitmap::sub_416704(pixel_2d *src)
{
    __NC_STACK_bitmap *a2 = &stack__bitmap;

    //// WHAT IT THIS !?
    int a4 = getRsrc_dontCopy();

    if ( a2->outline_coords )
    {
        if ( !a4 )
            nc_FreeMem(a2->outline_coords);
    }

    if ( a4 )
    {
        a2->outline_coords = (tUtV *)src;
        return 1;
    }

    int opl_count = 1; //Mandatory end (tu/tv = -1/-1)

    pixel_2d *opl_cur = src;

    while (opl_cur->flags >= 0)
    {
        opl_count++;
        opl_cur++;
    }

    tUtV *unk = (tUtV *)AllocVec(sizeof(tUtV) * opl_count, 1);

    if ( unk )
    {
        tUtV *tmp = unk;
        pixel_2d *opl = src;

        for (int i = 0; i < (opl_count - 1); i++)
        {
            tmp[i].tu = (float)opl[i].x * (1.0 / 256.0);
            tmp[i].tv = (float)opl[i].y * (1.0 / 256.0);
        }

        tmp[opl_count - 1].tu = -1;
        tmp[opl_count - 1].tv = -1;

        a2->outline_coords = unk;

        return 1;
    }
    return 0;
}


size_t NC_STACK_bitmap::func0(IDVList &stak)
{
    if ( !NC_STACK_rsrc::func0(stak) )
        return 0;

    __NC_STACK_bitmap *internal = &stack__bitmap;

    pixel_2d *v9 = NULL;

    v9 = (pixel_2d *)stak.GetPointer(BMD_ATT_OUTLINE, NULL);

    if ( v9 )
        sub_416704(v9);

    internal->bitm_intern = (ResBitmap *)getRsrc_pData();

    return 1;
}

size_t NC_STACK_bitmap::func1()
{
    __NC_STACK_bitmap *internal = &stack__bitmap;

    if ( internal->outline_coords )
    {
        size_t a4 = getRsrc_dontCopy();

        if ( !a4 )
            nc_FreeMem(internal->outline_coords);
    }

    return NC_STACK_rsrc::func1();
}

size_t NC_STACK_bitmap::func2(IDVList &stak)
{
    pixel_2d *v5 = (pixel_2d *)stak.GetPointer(BMD_ATT_OUTLINE, NULL);
    if ( v5 )
        setBMD_outline(v5);

    UA_PALETTE *ppal = (UA_PALETTE *)stak.GetPointer(BMD_ATT_PCOLORMAP, NULL);
    if ( ppal )
        setBMD_palette(ppal);

    return NC_STACK_rsrc::func2(stak);
}

size_t NC_STACK_bitmap::func3(IDVList &stak)
{
    for(IDVList::iterator it = stak.begin(); it != stak.end(); it++)
    {
        IDVPair &val = it->second;

        if ( !val.skip() )
        {
            switch (val.id)
            {
            case BMD_ATT_OUTLINE:
                *(void **)val.value.p_data = NULL;
                break;
            case BMD_ATT_WIDTH:
                *(int *)val.value.p_data = getBMD_width();
                break;
            case BMD_ATT_HEIGHT:
                *(int *)val.value.p_data = getBMD_height();
                break;
            case BMD_ATT_HAS_COLORMAP:
                *(int *)val.value.p_data = getBMD_hasPalette();
                break;

            default:
                break;
            }
        }
    }

    return NC_STACK_rsrc::func3(stak);
}

// Create bitmap resource node and fill rsrc field data
rsrc * NC_STACK_bitmap::rsrc_func64(IDVList &stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);// rsrc_func64
    if ( res )
    {
        int width = stak.Get(BMD_ATT_WIDTH, 0);
        int height = stak.Get(BMD_ATT_HEIGHT, 0);
        int colormap = stak.Get(BMD_ATT_HAS_COLORMAP, 0);
        //int create_texture = stak.Get(BMD_ATT_TEXTURE, 0);


        if ( width && height )
        {
            ResBitmap *intern = new ResBitmap;

            if ( intern )
            {
                if (colormap)
                    intern->palette = new UA_PALETTE;

                intern->width = width;
                intern->height = height;

                if ( !colormap )
                {
                    intern->width = width;
                    intern->height = height;

                    // allocate buffer, create palette, surface and texture
                    intern->swTex = CreateSurfaceScreenFormat(width, height);
                    if (!intern->swTex)
                    {
                        delete intern;
                        return res;
                    }
                }
                else
                {
                    intern->swTex = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);
                    if ( !intern->swTex )
                    {
                        delete intern;
                        return res;
                    }
                }
                res->data = intern;
            }
        }
    }
    return res;
}

size_t NC_STACK_bitmap::rsrc_func65(rsrc *res)
{
    ResBitmap *intern = (ResBitmap *)(res)->data;

    if ( intern )
    {
        if ( intern->hwTex )
            engines.display___win3d->FreeTexture(intern);

        if ( intern->swTex )
            SDL_FreeSurface(intern->swTex);

        if ( intern->palette )
            delete intern->palette;

        delete intern;
        res->data = NULL;
    }

    return NC_STACK_rsrc::rsrc_func65(res);
}

size_t NC_STACK_bitmap::bitmap_func128(IDVPair *)
{
    return 0;
}

size_t NC_STACK_bitmap::bitmap_func129(IDVPair *)
{
    return 0;
}

void NC_STACK_bitmap::bitmap_func130(bitmap_arg130 *out)
{
    __NC_STACK_bitmap *bitm = &stack__bitmap;
    out->pbitm = bitm->bitm_intern;
    out->outline = bitm->outline_coords;
}



void NC_STACK_bitmap::setBMD_outline(pixel_2d *otl)
{
    sub_416704(otl);
}

void NC_STACK_bitmap::setBMD_palette(UA_PALETTE *newPal)
{
    if ( stack__bitmap.bitm_intern->palette )
        *stack__bitmap.bitm_intern->palette = *newPal;
}



ResBitmap * NC_STACK_bitmap::GetResBmp()
{
    return stack__bitmap.bitm_intern;
}

int NC_STACK_bitmap::getBMD_width()
{
    if (stack__bitmap.bitm_intern)
        return stack__bitmap.bitm_intern->width;

    return 0;
}

int NC_STACK_bitmap::getBMD_height()
{
    if (stack__bitmap.bitm_intern)
        return stack__bitmap.bitm_intern->height;

    return 0;
}

int NC_STACK_bitmap::getBMD_hasPalette()
{
    if (stack__bitmap.bitm_intern)
        return stack__bitmap.bitm_intern->palette != NULL;

    return 0;
}

UA_PALETTE *NC_STACK_bitmap::getBMD_palette()
{
    if (stack__bitmap.bitm_intern)
        return stack__bitmap.bitm_intern->palette;

    return NULL;
}

SDL_Surface * NC_STACK_bitmap::GetSwTex()
{
    if (stack__bitmap.bitm_intern)
        return stack__bitmap.bitm_intern->swTex;

    return NULL;
}

void NC_STACK_bitmap::PrepareTexture( bool force )
{
    if (!stack__bitmap.bitm_intern)
        return;
    
    if (stack__bitmap.bitm_intern->hwTex && !force)
        return;
    
    if (stack__bitmap.bitm_intern->hwTex)
        engines.display___win3d->FreeTexture(stack__bitmap.bitm_intern);
        
    engines.display___win3d->AllocTexture(stack__bitmap.bitm_intern);
}

SDL_Surface * NC_STACK_bitmap::ConvertToScreen(SDL_Surface *src)
{
    return SDL_ConvertSurface(src, engines.display___win3d->GetScreenFormat(), 0);
}

SDL_Surface *NC_STACK_bitmap::CreateSurfaceScreenFormat(int width, int height)
{
    SDL_PixelFormat *fmt = engines.display___win3d->GetScreenFormat();
#if SDL_VERSION_ATLEAST(2,0,5)
    return SDL_CreateRGBSurfaceWithFormat(0, width, height, fmt->BitsPerPixel, fmt->format);
#else
    return SDL_CreateRGBSurface(0, width, height, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask );
#endif
}

size_t NC_STACK_bitmap::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( *(IDVList *)data );
    case 1:
        return (size_t)func1();
    case 2:
        return func2( *(IDVList *)data );
    case 3:
        return func3( *(IDVList *)data );
    case 64:
        return (size_t)rsrc_func64( *(IDVList *)data );
    case 65:
        return rsrc_func65( (rsrc *)data );
    case 128:
        return (size_t)bitmap_func128( (IDVPair *)data );
    case 129:
        return (size_t)bitmap_func129( (IDVPair *)data );
    case 130:
        bitmap_func130( (bitmap_arg130 *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_rsrc::compatcall(method_id, data);
}
