#include "includes.h"
#include "MC.h"
#include "classes.h"
#include "nucleas.h"

#include "engine_gfx.h"
#include "engine_miles.h"
#include "engine_tform.h"
#include "engine_input.h"

#include "rsrc.h"
#include "bitmap.h"
#include "ilbm.h"
#include "bmpAnm.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"

#include "skeleton.h"
#include "sklt.h"
#include "ade.h"
#include "area.h"
#include "amesh.h"
#include "sample.h"
#include "wav.h"
#include "base.h"

#include "particle.h"

#include "button.h"

#include "itimer.h"
#include "wintimer.h"

#include "idev.h"
#include "iwimp.h"
#include "winp.h"
#include "input.h"

#include "embed.h"

#include "network.h"
#include "windp.h"

#include "yw.h"

#include "ypabact.h"
#include "ypatank.h"
#include "ypacar.h"
#include "ypaufo.h"
#include "ypaflyer.h"
#include "ypagun.h"
#include "ypamissile.h"
#include "yparobo.h"
#include "ypaufo.h"

const char MC2RES[] = "MC2RES\\";
const char MC2CLASSES[] = "MC2classes:";
const char MC2ENGINES[] = "MC2engines:";

ClassList newclasses;

void init_mc_res_class_engine_strings(MC_TYPE a1, const char *str)
{
    if ( a1 < MC_TYPE_UNKNOWN )
    {
        if ( !str )
        {
            if (a1 == MC_TYPE_RES)
                str = MC2RES;
            else if (a1 == MC_TYPE_CLASS)
                str = MC2CLASSES;
            else if (a1 == MC_TYPE_ENGINE)
                str = MC2ENGINES;
            else
                return;
        }

        strcpy(engines.MC_RES_CLASS_ENGINE[a1], str);
    }
}


int set_classes_list()
{
    engines.__AllocCount = 0;
    engines.__AllocSize = 0;
    engines.__AllocMax = 0;
    engines.file_handles = 0;

    newclasses.push_back(&NC_STACK_nucleus::description);
    newclasses.push_back(&NC_STACK_rsrc::description);
    newclasses.push_back(&NC_STACK_bitmap::description);
    newclasses.push_back(&NC_STACK_skeleton::description);
    newclasses.push_back(&NC_STACK_ilbm::description);
    newclasses.push_back(&NC_STACK_sklt::description);
    newclasses.push_back(&NC_STACK_ade::description);
    newclasses.push_back(&NC_STACK_area::description);
    newclasses.push_back(&NC_STACK_base::description);
    newclasses.push_back(&NC_STACK_bmpanim::description);
    newclasses.push_back(&NC_STACK_amesh::description);
    newclasses.push_back(&NC_STACK_particle::description);
    newclasses.push_back(&NC_STACK_embed::description);
    newclasses.push_back(&NC_STACK_idev::description);
    newclasses.push_back(&NC_STACK_input::description);
    newclasses.push_back(&NC_STACK_itimer::description);
    newclasses.push_back(&NC_STACK_iwimp::description);
    newclasses.push_back(&NC_STACK_sample::description);
    newclasses.push_back(&NC_STACK_wav::description);
    newclasses.push_back(&NC_STACK_raster::description);
    newclasses.push_back(&NC_STACK_display::description);
    newclasses.push_back(&NC_STACK_button::description);
    newclasses.push_back(&NC_STACK_network::description);
    newclasses.push_back(&NC_STACK_windd::description);
    newclasses.push_back(&NC_STACK_win3d::description);
    newclasses.push_back(&NC_STACK_winp::description);
    newclasses.push_back(&NC_STACK_wintimer::description);
    newclasses.push_back(&NC_STACK_windp::description);
    newclasses.push_back(&NC_STACK_ypaworld::description);
    newclasses.push_back(&NC_STACK_ypabact::description);
    newclasses.push_back(&NC_STACK_ypatank::description);
    newclasses.push_back(&NC_STACK_yparobo::description);
    newclasses.push_back(&NC_STACK_ypamissile::description);
    newclasses.push_back(&NC_STACK_ypaflyer::description);
    newclasses.push_back(&NC_STACK_ypacar::description);
    newclasses.push_back(&NC_STACK_ypaufo::description);
    newclasses.push_back(&NC_STACK_ypagun::description);
    return 1;
}



stored_functions_engine *init_engine(unsigned int engineID)
{
    stored_functions_engine **engineFuncs = NULL;

    char v10[256];
    char v11[128];

    memset(v11, 0, 128);
    strcpy(v11, "dummy.engine");

    key_value_stru engine;
    engine.key = "dummy.engine";
    engine.key_type = KEY_TYPE_STRING1;
    engine.value.pval = v11;
    stored_functions_engine *engine_vtbl = NULL;

    if (engineID == MC2_GFX_ENGINE)
    {
        engineFuncs = &engines.setted_gfx_engine;
        engine_vtbl = &gfx_engine_vtbl;
        engine.key = "gfx.engine";
    }
    else if (engineID == MC2_AUDIO_ENGINE)
    {
        engineFuncs = &engines.setted_audio_engine;
        engine_vtbl = &miles_engine_vtbl;
        engine.key = "audio.engine";
    }
    else if (engineID == MC2_INPUT_ENGINE)
    {
        engineFuncs = &engines.setted_input_engine;
        engine_vtbl = &input_engine_vtbl;
        engine.key = "input.engine";
    }
    else if (engineID == MC2_TFORM_ENGINE)
    {
        engineFuncs = &engines.setted_tform_engine;
        engine_vtbl = &tform_engine_vtbl;
        engine.key = "tform.engine";
    }

    if ( get_keyvalue_from_ini(0, &engine, 1) )
    {
        strcpy(v10, (char *)engine.value.pval);

        if ( engine_vtbl )
        {
            *engineFuncs = engine_vtbl;
            if ( engine_vtbl->init() )
                return engine_vtbl;
            else
                return NULL;
        }
        else
            ypa_log_out("Can't find %s\n", v10);
    }
    return NULL;
}


void deinit_engine(unsigned int engineID)
{
////	unk_class *klass = NULL;
    stored_functions_engine *engine = NULL;

    switch(engineID)
    {
    case MC2_GFX_ENGINE:
////		klass = &engines.gfx_engine__unk_class;
        engine = engines.setted_gfx_engine;
        break;
    case MC2_AUDIO_ENGINE:
////		klass = &engines.audio_engine__unk_class;
        engine = engines.setted_audio_engine;
        break;
    case MC2_INPUT_ENGINE:
////		klass = &engines.input_engine__unk_class;
        engine = engines.setted_input_engine;
        break;
    case MC2_TFORM_ENGINE:
////		klass = &engines.tform_engine__unk_class;
        engine = engines.setted_tform_engine;
        break;
    default:
        break;
    }

    if ( engine )
    {
////		engine->deinit(klass);
        engine->deinit();
    }
}


void sb_0x411c08__sub0()
{
    ypa_log_out("Nucleus shutdown:\n");
    ypa_log_out("    __AllocCount = %d\n", engines.__AllocCount);
    ypa_log_out("    __AllocSize  = %d\n", engines.__AllocSize);
    ypa_log_out("    __AllocMax   = %d\n", engines.__AllocMax);
    ypa_log_out("    __FOpenCount = %d\n", engines.file_handles);
}

void sb_0x411c08()
{
    while ( 1 )
    {
        void *tmp = RemHead(&engines.stru_525D68);
        if ( !tmp )
            break;
        nc_FreeMem(tmp);
    }
    sb_0x411c08__sub0();
}

const char * get_MC_str(MC_TYPE a1)
{
    if ( a1 < MC_TYPE_UNKNOWN )
        return engines.MC_RES_CLASS_ENGINE[a1];
    else
        return NULL;
}
