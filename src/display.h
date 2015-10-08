#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "raster.h"

extern class_stored display_class_off;


struct UA_PALENTRY
{
    BYTE r;
    BYTE g;
    BYTE b;
};

struct __NC_STACK_display
{
  UA_PALENTRY palette[256];

};

struct NC_STACK_display : public NC_STACK_raster
{
    __NC_STACK_display stack__display;
};


#endif // DISPLAY_H_INCLUDED
