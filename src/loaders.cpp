
#include "loaders.h"
#include "utils.h"
#include "image.h"
#include "ilbm.h"
#include "3ds.h"
#include "obj3d.h"

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
         !StriCmp(name, "jpeg") ||
         !StriCmp(name, "bmp") ||
         !StriCmp(name, "gif") ||
         !StriCmp(name, "pcx") ||
         !StriCmp(name, "svg") )
        return Nucleus::CInit<NC_STACK_image>( stak );

    return Nucleus::CInit<NC_STACK_ilbm>( stak );
}

NC_STACK_bitmap *ProxyLoadImage(IDVList::TInitList lst)
{
    IDVList stak(lst);
    return ProxyLoadImage(stak);
}

NC_STACK_base *ProxyLoadBase(const std::string &fname)
{
    std::string name = fname;
    size_t pos = name.rfind('.');
    if (pos != std::string::npos)
        name = name.substr(pos + 1);
    
    if ( !StriCmp(name, "3ds") )
        return NC_STACK_3ds::Load3DS(fname);
    else if ( !StriCmp(name, "obj") )
        return NC_STACK_Obj3D::LoadObj3D(fname);
    
    return NC_STACK_base::LoadBaseFromFile(fname);
}

}