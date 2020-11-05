#include "includes.h"
#include "engine_gfx.h"
#include "utils.h"
#include "common.h"
#include "gui/root.h"

GFXEngine GFXEngine::GFXe;

Common::Ini::KeyList gfx_keys
{
    Common::Ini::Key("gfx.mode",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.xres",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.yres",     Common::Ini::KT_DIGIT),
    Common::Ini::Key("gfx.palette",  Common::Ini::KT_WORD),
    Common::Ini::Key("gfx.display",  Common::Ini::KT_WORD),
    Common::Ini::Key("gfx.display2", Common::Ini::KT_WORD)
};

int GFXEngine::init()
{
    NC_STACK_win3d::initfirst();

    Common::Ini::ParseIniFile(NC_STACK_nucleus::DefaultIniFile, &gfx_keys);

    if ( sub_422CE8(gfx_keys[4].Get<std::string>(), 
                    gfx_keys[5].Get<std::string>(), 
                    0) )
    {
        loadPal(gfx_keys[3].Get<std::string>());
        return 1;
    }
    return 0;
}

void GFXEngine::deinit()
{
    if ( cls3D )
    {
        delete_class_obj(cls3D);
        cls3D = NULL;
    }
}

int GFXEngine::sub_422CE8(const std::string &display, const std::string &display2, int gfxmode)
{
    if ( !display.empty() )
    {
        cls3D = Nucleus::CTFInit<NC_STACK_win3d>(display, { {NC_STACK_display::ATT_DISPLAY_ID, (int32_t)gfxmode} } );

        if ( !cls3D )
        {
            if ( !display2.empty() )
                cls3D = Nucleus::CTFInit<NC_STACK_win3d>(display2, { {NC_STACK_display::ATT_DISPLAY_ID, (int32_t)gfxmode} } );
        }
        if ( !cls3D )
            ypa_log_out("gfx.engine: display driver init failed!\n");
    }
    else
    {
        ypa_log_out("gfx.engine: no display driver name given!\n");
    }

    Gui::Root::Instance.SetScreenSize(cls3D->GetSize());

    return cls3D != NULL;
}

int GFXEngine::loadPal(const std::string &palette_ilbm)
{
    NC_STACK_bitmap *ilbm = Nucleus::CInit<NC_STACK_ilbm>( {
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(palette_ilbm)},
        {NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, (int32_t)1}} );

    if (!ilbm)
        return 0;

    cls3D->SetPalette( *ilbm->getBMD_palette() );

    delete_class_obj(ilbm);

    return 1;
}

void GFXEngine::defRenderFunc(void *dat)
{
    GFXe.cls3D->raster_func206((polysDat *)dat);
}

int GFXEngine::getScreenH()
{
    return cls3D->GetHeight();
}

int GFXEngine::getScreenW()
{
    return cls3D->GetWidth();
}

NC_STACK_win3d *GFXEngine::getC3D()
{
    return cls3D;
}

void GFXEngine::setResolution(int res)
{
    UA_PALETTE *screen_palette = cls3D->GetPalette();

    UA_PALETTE palette_copy;

    if ( screen_palette )
        palette_copy = *screen_palette; // Copy palette
    else
    {
        for(auto &x : palette_copy)
        {
            x.r = 0;
            x.g = 0;
            x.b = 0;
            x.a = 0;
        }
    }

    cls3D->EndFrame();

    delete_class_obj(cls3D);

    if ( sub_422CE8(gfx_keys[4].Get<std::string>(), 
                    gfx_keys[5].Get<std::string>(), 
                    res) )
    {
        cls3D->BeginFrame();

        cls3D->SetPalette(palette_copy);
    }
}

void GFXEngine::setTracyRmp(ResBitmap *rmp)
{
    cls3D->setRSTR_trcRmp(rmp);
}

void GFXEngine::setShadeRmp(ResBitmap *rmp)
{
    cls3D->setRSTR_shdRmp(rmp);
}



TileMap * GFXEngine::getTileset(int id)
{
    return cls3D->raster_func208(id);
}

void GFXEngine::drawText(w3d_a209 *arg)
{
    w3d_a209 arg209;
    arg209 = *arg;
    cls3D->raster_func209(&arg209);
}

void GFXEngine::setTileset(TileMap *tileset, int id)
{
    cls3D->raster_func207(id, tileset);
}


TileMap::TileMap()
{
    img = NULL;
    h = 0;
}
    
TileMap::~TileMap()
{
    if (img)
        Nucleus::Delete(img);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Point &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::PointRect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.w)
        src.w = pos.w;
    if (src.h > pos.h)
        src.h = pos.h;    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Draw(SDL_Surface *surface, const Common::Rect &pos, uint8_t c)
{
    SDL_Rect src = map[c];
    if (src.w > pos.Width())
        src.w = pos.Width();
    if (src.h > pos.Height())
        src.h = pos.Height();    
    SDL_Rect dst = pos;
    SDL_BlitSurface(img->GetSwTex(), &src, surface, &dst);
}

void TileMap::Fill(SDL_Surface *surface, const Common::Rect &rect, uint8_t c)
{
    SDLWRAP::DrawFill(img->GetSwTex(), map[c], surface, rect);
}

void TileMap::Fill(SDL_Surface *surface, const Common::PointRect &rect, uint8_t c)
{
    SDLWRAP::DrawFill(img->GetSwTex(), map[c], surface, rect);
}

int TileMap::GetWidth(uint8_t c) const
{
    return map[c].w;
}

int TileMap::GetWidth(const std::string &str) const
{
    int wdth = 0;
    for (uint8_t c : str)
        wdth += map[c].w;
    return wdth;
}

Common::Point TileMap::GetSize(uint8_t c) const
{
    return Common::Point(map[c].w, h);
}

