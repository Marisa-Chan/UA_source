#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "bitmap.h"

class NC_STACK_image: public NC_STACK_bitmap
{
public:
    virtual size_t Init(IDVList &stak);
    virtual rsrc * rsrc_func64(IDVList &stak);
    
    static bool SavePng(ResBitmap *bmp, const std::string &fname);

    NC_STACK_image();
    virtual ~NC_STACK_image();
    
    virtual const std::string &ClassName() const {
        return description._classname;
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_image();
    };

    //Data
public:
    static const Nucleus::ClassDescr description;
};

#endif // ILBM_H_INCLUDED
