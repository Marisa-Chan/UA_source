#include "includes.h"
#include "engine_gfx.h"
#include "utils.h"

GFXEngine GFXe;

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
    char buf[33];

    if ( *display )
    {
        stack_vals vals[4];
        vals[0].set(NC_STACK_rsrc::RSRC_ATT_NAME, "display");
        vals[1].set(NC_STACK_rsrc::RSRC_ATT_TRYSHARED, 2);
        vals[2].set(NC_STACK_display::DISP_ATT_DISPLAY_ID, gfxmode);
        vals[3].end();

        strcpy(buf, display);

        cls3D = dynamic_cast<NC_STACK_win3d *>( init_get_class(buf, vals) );

        if ( !cls3D )
        {
            if ( *display2 )
            {
                strcpy(buf, display2);
                cls3D = dynamic_cast<NC_STACK_win3d *>( init_get_class(buf, vals) );
            }
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
    stack_vals vals[3];
    vals[0].set(NC_STACK_rsrc::RSRC_ATT_NAME, palette_ilbm);
    vals[1].set(NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, 1);
    vals[2].end();

    NC_STACK_ilbm *ilbm = dynamic_cast<NC_STACK_ilbm *>(init_get_class("ilbm.class", vals) );

    if (!ilbm)
        return 0;

    cls3D->setBMD_palette( ilbm->getBMD_palette() );

    delete_class_obj(ilbm);

    return 1;
}

void GFXEngine::defRenderFunc(void *dat)
{
    GFXe.cls3D->raster_func206((polysDatSub *)dat);
}

int GFXEngine::getScreenH()
{
    return cls3D->getBMD_height();
}

int GFXEngine::getScreenW()
{
    return cls3D->getBMD_width();
}

NC_STACK_win3d *GFXEngine::getC3D()
{
    return cls3D;
}

gfx_window *GFXEngine::getWindow()
{
    return cls3D->getDISP_displInf();
}

void GFXEngine::setResolution(int res)
{
    UA_PALETTE *screen_palette = cls3D->getBMD_palette();

    UA_PALETTE palette_copy;
    memset(&palette_copy, 0, sizeof(palette_copy));

    if ( screen_palette )
        palette_copy = *screen_palette; // Copy palette

    cls3D->display_func258(NULL);

    delete_class_obj(cls3D);

    if ( sub_422CE8(gfx_display, gfx_display2, res) )
    {
        cls3D->display_func257(NULL);

        cls3D->setBMD_palette(&palette_copy);
    }
}

void GFXEngine::setTracyRmp(bitmap_intern *rmp)
{
    cls3D->setRSTR_trcRmp(rmp);
}

void GFXEngine::setShadeRmp(bitmap_intern *rmp)
{
    cls3D->setRSTR_shdRmp(rmp);
}



tiles_stru * GFXEngine::getTileset(int id)
{
    rstr_207_arg arg207;

    arg207.tiles = 0;
    arg207.id = id;

    cls3D->raster_func208(&arg207);
    return arg207.tiles;
}

void GFXEngine::drawText(w3d_a209 *arg)
{
    w3d_a209 arg209;
    arg209 = *arg;
    cls3D->raster_func209(&arg209);
}

void GFXEngine::setTileset(tiles_stru *tileset, int id)
{
    rstr_207_arg arg;

    arg.tiles = tileset;
    arg.id = id;

    cls3D->raster_func207(&arg);
}
