#include "includes.h"
#include "MC.h"
#include "nucleas.h"

#include "engine_gfx.h"
#include "engine_miles.h"
#include "engine_tform.h"
#include "engine_input.h"

#include "rsrc.h"
#include "bitmap.h"
#include "ilbm.h"
#include "bmpAnm.h"
#include "display.h"
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

#include "image.h"

ClassList newclasses;

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
    newclasses.push_back(&NC_STACK_display::description);
    newclasses.push_back(&NC_STACK_button::description);
    newclasses.push_back(&NC_STACK_network::description);
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
    newclasses.push_back(&NC_STACK_image::description);
    return 1;
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
    for(std::list<TKVPair *>::iterator it = engines.kvPairs.begin(); it != engines.kvPairs.end(); it++)
        delete *it;

    engines.kvPairs.clear();


    sb_0x411c08__sub0();
}

const NewClassDescr * getClassAllocator(const char *name)
{
    for(ClassList_iter it = newclasses.begin(); it != newclasses.end(); it++)
    {
        if (!strcasecmp(name, (*it)->classname) )
            return *it;
    }

    return NULL;
}

