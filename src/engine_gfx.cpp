#include "includes.h"
#include "engine_gfx.h"
#include "utils.h"
#include "common.h"

GFXEngine GFXEngine::GFXe;

char gfx_palette[128];
char gfx_display[128];
char gfx_display2[128];

key_value_stru gfx_keys[6] = {{"gfx.mode", KEY_TYPE_DIGIT, 0},
    {"gfx.xres", KEY_TYPE_DIGIT, 0},
    {"gfx.yres", KEY_TYPE_DIGIT, 0},
    {"gfx.palette", KEY_TYPE_STRING1, gfx_palette},
    {"gfx.display", KEY_TYPE_STRING1, gfx_display},
    {"gfx.display2", KEY_TYPE_STRING1, gfx_display2}
};

int GFXEngine::init()
{
    NC_STACK_win3d::initfirst();

    memset(gfx_palette, 0, 128);
    memset(gfx_display, 0, 128);
    memset(gfx_display2, 0, 128);
    get_keyvalue_from_ini(0, gfx_keys, 6);

    if ( sub_422CE8((const char *)gfx_keys[4].value.pval, (const char *)gfx_keys[5].value.pval, 0) )
    {
        loadPal((const char *)gfx_keys[3].value.pval);
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

int GFXEngine::sub_422CE8(const char *display, const char *display2, int gfxmode)
{
    if ( *display )
    {
        IDVList vals;
        vals.Add(NC_STACK_display::ATT_DISPLAY_ID, gfxmode);

        cls3D = Nucleus::CTFInit<NC_STACK_win3d>(display, vals);

        if ( !cls3D )
        {
            if ( *display2 )
                cls3D = Nucleus::CTFInit<NC_STACK_win3d>(display2, vals);
        }
        if ( !cls3D )
            ypa_log_out("gfx.engine: display driver init failed!\n");
    }
    else
    {
        ypa_log_out("gfx.engine: no display driver name given!\n");
    }
    return cls3D != NULL;
}

int GFXEngine::loadPal(const char *palette_ilbm)
{
    IDVList vals;
    vals.Add(NC_STACK_rsrc::RSRC_ATT_NAME, palette_ilbm);
    vals.Add(NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, 1);

    NC_STACK_bitmap *ilbm = Nucleus::CInit<NC_STACK_ilbm>(vals);

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

    if ( sub_422CE8(gfx_display, gfx_display2, res) )
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