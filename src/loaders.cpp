
#include "loaders.h"
#include "utils.h"
#include "image.h"
#include "ilbm.h"

namespace Utils
{
    
NC_STACK_bitmap *ProxyLoadImage(IDVList &stak)
{
    std::string name = stak.Get<std::string>(NC_STACK_rsrc::RSRC_ATT_NAME, "");
    size_t pos = name.rfind('.');
    if (pos != std::string::npos)
        name = name.substr(pos + 1);
    
    if ( !StriCmp(name, "jpg") ||
         !StriCmp(name, "png") ||
         !StriCmp(name, "tiff") ||
         !StriCmp(name, "webp") ||
         !StriCmp(name, "jpeg") )
        return Nucleus::CInit<NC_STACK_image>( stak );

    return Nucleus::CInit<NC_STACK_ilbm>( stak );
}

NC_STACK_bitmap *ProxyLoadImage(IDVList::TInitList lst)
{
    IDVList stak(lst);
    return ProxyLoadImage(stak);
}

}