#ifndef WIN3D_H_INCLUDED
#define WIN3D_H_INCLUDED

#include <ddraw.h>
#include <d3d.h>

struct texStru
{
  IDirectDrawSurface *surface;
  IDirect3DTexture2 *texture;
  IDirectDrawPalette *palette;
};

extern class_stored win3d_class_off;

struct __NC_STACK_win3d
{
  int field_0;
};

struct NC_STACK_win3d : public NC_STACK_windd
{
    __NC_STACK_windd stack__win3d;
};


#endif //WIN3D_H_INCLUDED
