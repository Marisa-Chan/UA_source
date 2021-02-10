#include "includes.h"
#include "MC.h"
#include "nucleas.h"

#include "system/gfx.h"
#include "system/sound.h"
#include "system/inpt.h"

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

#include "3ds.h"
#include "image.h"

int set_classes_list()
{
    Nucleus::ClassList::Instance.push_back(NC_STACK_nucleus::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_rsrc::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_bitmap::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_skeleton::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ilbm::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_sklt::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ade::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_area::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_base::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_bmpanim::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_amesh::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_particle::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_embed::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_idev::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_input::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_itimer::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_iwimp::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_sample::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_wav::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_display::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_button::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_network::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_win3d::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_winp::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_wintimer::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_windp::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypaworld::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypabact::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypatank::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_yparobo::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypamissile::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypaflyer::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypacar::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypaufo::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_ypagun::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_3ds::description);
    Nucleus::ClassList::Instance.push_back(NC_STACK_image::description);
    return 1;
}

void sb_0x411c08()
{
    engines.kvPairs.clear();


    ypa_log_out("Nucleus shutdown:\n");
}



