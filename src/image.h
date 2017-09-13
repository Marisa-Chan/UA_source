#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include "bitmap.h"

class NC_STACK_image: public NC_STACK_bitmap
{
public:
    virtual size_t func0(IDVList *stak);
    virtual rsrc * rsrc_func64(IDVList *stak);

    NC_STACK_image();
    virtual ~NC_STACK_image();

    virtual const char * getClassName() {
        return "image.class";
    };

    static NC_STACK_nucleus * newinstance() {
        return new NC_STACK_image();
    };

    static NC_STACK_image *CInit(IDVList *stak);

    //Data
public:
    static const NewClassDescr description;
};

#endif // ILBM_H_INCLUDED
