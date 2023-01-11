#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "utils.h"



size_t NC_STACK_bitmap::Init(IDVList &stak)
{
    if ( !NC_STACK_rsrc::Init(stak) )
        return 0;

    std::vector<tUtV> *v9 = stak.Get<std::vector<tUtV> *>(BMD_ATT_OUTLINE, NULL);

    if ( v9 )
        outline_coords = *v9;

    bitm_intern = (ResBitmap *)getRsrc_pData();

    return 1;
}

size_t NC_STACK_bitmap::Deinit()
{
    return NC_STACK_rsrc::Deinit();
}

// Create bitmap resource node and fill rsrc field data
rsrc * NC_STACK_bitmap::rsrc_func64(IDVList &stak)
{
    rsrc *res = NC_STACK_rsrc::rsrc_func64(stak);// rsrc_func64
    if ( res )
    {
        int width = stak.Get<int32_t>(BMD_ATT_WIDTH, 0);
        int height = stak.Get<int32_t>(BMD_ATT_HEIGHT, 0);
        int colormap = stak.Get<int32_t>(BMD_ATT_HAS_COLORMAP, 0);
        //int create_texture = stak.Get<int32_t>(BMD_ATT_TEXTURE, 0);


        if ( width && height )
        {
            ResBitmap *intern = new ResBitmap();

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
                    intern->swTex = GFX::Engine.CreateSurfaceScreenFormat(width, height);
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
            GFX::Engine.FreeTexture(intern);

        if ( intern->swTex )
            SDL_FreeSurface(intern->swTex);

        if ( intern->palette )
            delete intern->palette;

        delete intern;
        res->data = NULL;
    }

    return NC_STACK_rsrc::rsrc_func65(res);
}

void NC_STACK_bitmap::setBMD_palette(UA_PALETTE *newPal)
{
    if ( bitm_intern->palette )
        *bitm_intern->palette = *newPal;
}



ResBitmap * NC_STACK_bitmap::GetBitmap(int frameid)
{
    return bitm_intern;
}

std::vector<tUtV> &NC_STACK_bitmap::GetOutline(int frameid /* = -1 */)
{
    return outline_coords;
}

int NC_STACK_bitmap::getBMD_width()
{
    if (bitm_intern)
        return bitm_intern->width;

    return 0;
}

int NC_STACK_bitmap::getBMD_height()
{
    if (bitm_intern)
        return bitm_intern->height;

    return 0;
}

int NC_STACK_bitmap::getBMD_hasPalette()
{
    if (bitm_intern)
        return bitm_intern->palette != NULL;

    return 0;
}

UA_PALETTE *NC_STACK_bitmap::getBMD_palette()
{
    if (bitm_intern)
        return bitm_intern->palette;

    return NULL;
}

SDL_Surface * NC_STACK_bitmap::GetSwTex()
{
    if (bitm_intern)
        return bitm_intern->swTex;

    return NULL;
}

void NC_STACK_bitmap::PrepareTexture( bool force )
{
    if (!bitm_intern)
        return;
    
    if (bitm_intern->hwTex && !force)
        return;
    
    if (bitm_intern->hwTex)
        GFX::Engine.FreeTexture(bitm_intern);
        
    GFX::Engine.AllocTexture(bitm_intern);
}

