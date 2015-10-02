#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "raster.h"

extern class_stored display_class_off;

struct __NC_STACK_display
{
  int field_0;

};

struct NC_STACK_display : public NC_STACK_raster
{
    __NC_STACK_display stack__display;
};


#endif // DISPLAY_H_INCLUDED
