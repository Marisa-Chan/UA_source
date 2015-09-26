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


#endif //WIN3D_H_INCLUDED
