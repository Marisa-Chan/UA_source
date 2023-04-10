#include "tools.h"
#include "../loaders.h"


namespace World {
    
static bool bAllowMods = false;
static bool bAllowModsWarning = true;
    
Common::PlaneBytes GetPlaneBytesFromBitmap(NC_STACK_bitmap *bitmap)
{
    Common::PlaneBytes plane;
    
    if (!bitmap)
        return plane;
    
    ResBitmap * ibitm = bitmap->GetBitmap();
    if (!ibitm || !ibitm->swTex)
        return plane;
    
    if (ibitm->swTex->w == 0 || ibitm->swTex->h == 0)
        return plane;
    
    // Only 8 bits per pixel support
    if (ibitm->swTex->format->BytesPerPixel != 1)
        return plane;
    
    if ( SDL_LockSurface(ibitm->swTex) != 0 )
        return plane;
    
    plane.Resize(ibitm->swTex->w, ibitm->swTex->h);
    for(int32_t y = 0; y < ibitm->swTex->h; ++y)
    {
        uint8_t *pl = plane.Line(y);
        uint8_t *bl = ((uint8_t *)ibitm->swTex->pixels) + ibitm->swTex->pitch * y;
        for(int32_t x = 0; x < ibitm->swTex->w; ++x)
            pl[x] = bl[x];
    }
    
    SDL_UnlockSurface(ibitm->swTex);
            
    return plane;    
}

Common::PlaneBytes LoadMapDataFromImage(const std::string &fileName)
{
    Common::PlaneBytes plane;
    
    NC_STACK_bitmap *img = Utils::ProxyLoadImage({{NC_STACK_rsrc::RSRC_ATT_NAME, fileName}});
    if (!img)
        return plane;
    
    plane = GetPlaneBytesFromBitmap(img);
    
    img->Delete();
    
    return plane;
}


bool IsModsAllow(bool warning)
{
    if (!bAllowMods && warning && bAllowModsWarning)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Modify detection", "Custom modify detected and not allowed in \"vanilla\" game.\nModifications allowed only in MODs", NULL);
        bAllowModsWarning = false;
    }
    
    return bAllowMods;
}

bool AllowMods(bool allow)
{
    bool tmp = bAllowMods;
    bAllowMods = allow;
    return tmp;
}

}