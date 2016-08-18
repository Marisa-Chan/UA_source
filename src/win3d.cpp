#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "utils.h"
#include "MS/d3dmacs.h"

#include "font.h"

extern IDirectDraw *ddraw;  //FROM windd.cpp
extern IDirect3D2 *d3d2;    //FROM windd.cpp


const NewClassDescr NC_STACK_win3d::description("win3d.class", &newinstance);

key_value_stru win3d_keys[7] =
{
    {"gfx.dither", KEY_TYPE_BOOL, 0},
    {"gfx.filter", KEY_TYPE_BOOL, 0},
    {"gfx.antialias", KEY_TYPE_BOOL, 0},
    {"gfx.alpha", KEY_TYPE_DIGIT, 0xC0},
    {"gfx.zbuf_when_tracy", KEY_TYPE_BOOL, 0},
    {"gfx.colorkey", KEY_TYPE_BOOL, 0},
    {"gfx.force_emul", KEY_TYPE_BOOL, 0}
};

DWORD RGBAToColor(unsigned int r, unsigned int g, unsigned int b, unsigned int a, int rshift, int gshift, int bshift, int ashift, int rmask, int gmask, int bmask, int amask)
{
    DWORD rbits;
    DWORD gbits;
    DWORD bbits;
    DWORD abits;

    if ( rshift < 0 )
        rbits = r >> -rshift;
    else
        rbits = r << rshift;
    if ( gshift < 0 )
        gbits = g >> -gshift;
    else
        gbits = g << gshift;
    if ( bshift < 0 )
        bbits = b >> -bshift;
    else
        bbits = b << bshift;
    if ( ashift < 0 )
        abits = a >> -ashift;
    else
        abits = a << ashift;

    return (bmask & bbits) | (gmask & gbits) | (amask & abits) | (rmask & rbits);
}


int win3dInitialisation(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = (win3d_bigdata *)AllocVec(sizeof(win3d_bigdata), 0);
    w3d->bigdata = bigdata;
    if ( bigdata )
    {
        memset(bigdata, 0, sizeof(win3d_bigdata));

        bigdata->_dx = wdd->width / 2;
        bigdata->_dy = wdd->height / 2;

        for (int i = 0; i < 8; i++)
        {
            DWORD color = 0;
            switch(i)
            {
            case 0:
            case 4:
            case 5:
            case 6:
                color = 0xFFFFFF;
                break;
            case 1:
                color = 0xFF0000;
                break;
            case 2:
                color = 0xFF;
                break;
            case 3:
                color = 0xFF80;
                break;
            case 7:
                color = 0xFFC0;
                break;
            default:
                break;
            }

            bigdata->gray_colors__[i][0] = ((color >> 16) & 0xFF) / 255.0;
            bigdata->gray_colors__[i][1] = ((color >> 8) & 0xFF) / 255.0;
            bigdata->gray_colors__[i][2] = (color & 0xFF) / 255.0;
        }

        bigdata->gray_colors__[8][0] = 1.0;
        bigdata->gray_colors__[8][1] = 1.0;
        bigdata->gray_colors__[8][2] = 1.0;

        return 1;
    }

    return 0;
}

int initPixelFormats(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    wind3d_pixelformat *fmt = &w3d->bigdata->primary__pixelformat;

    memset(fmt, 0, sizeof(wind3d_pixelformat));

    int bytes = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRGBBitCount / 8;
    fmt->BytesPerColor = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRGBBitCount / 8;

    if ( bytes == 2 )
        fmt->color_mode = 1;
    else if ( bytes == 4)
        fmt->color_mode = 2;
    else
        fmt->color_mode = 0;

    if ( !(dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) )
    {
        fmt->dwRBitMask = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRBitMask;
        fmt->dwGBitMask = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwGBitMask;
        fmt->dwBBitMask = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwBBitMask;
        fmt->dwAlphaBitMask = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRGBAlphaBitMask;

        DWORD bits = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRBitMask;
        int i;

        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            fmt->dwRShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            fmt->dwRShift -= 8 - i;
        }
        else
            fmt->dwRShift = 0;

        bits = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwGBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            fmt->dwGShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            fmt->dwGShift -= 8 - i;
        }
        else
            fmt->dwGShift = 0;

        bits = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwBBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            fmt->dwBShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            fmt->dwBShift -= 8 - i;
        }
        else
            fmt->dwBShift = 0;

        bits = dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRGBAlphaBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            fmt->dwAShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            fmt->dwAShift -= 8 - i;
        }
        else
            fmt->dwAShift = 0;

        fmt->FFFF0000__color = RGBAToColor(255, 255, 0, 0, fmt->dwRShift, fmt->dwGShift, fmt->dwBShift, fmt->dwAShift, fmt->dwRBitMask, fmt->dwGBitMask, fmt->dwBBitMask, fmt->dwAlphaBitMask);

        fmt->colors = (BYTE *)AllocVec(fmt->BytesPerColor * 256, 0);

        if ( !fmt->colors )
        {
            if ( w3d->bigdata )
            {
                if ( w3d->bigdata->primary__pixelformat.colors )
                {
                    nc_FreeMem(w3d->bigdata->primary__pixelformat.colors);
                    w3d->bigdata->primary__pixelformat.colors = NULL;
                }
                if ( w3d->bigdata->selected__pixelformat.colors )
                {
                    nc_FreeMem(w3d->bigdata->selected__pixelformat.colors);
                    w3d->bigdata->selected__pixelformat.colors = NULL;
                }
            }
            return 0;
        }
    }


    w3d->bigdata->field_3870 = 0;

    for (int i = 0; i < 255; i++)
        w3d->bigdata->grey_gradient[i] = RGBAToColor(i, i, i, 255, fmt->dwRShift, fmt->dwGShift, fmt->dwBShift, fmt->dwAShift, fmt->dwRBitMask, fmt->dwGBitMask, fmt->dwBBitMask, fmt->dwAlphaBitMask);

    wind3d_pixelformat *pixfmt = &w3d->bigdata->selected__pixelformat;
    memset(pixfmt, 0, sizeof(wind3d_pixelformat));


    windd_formats *dd_fmt = &wdd->intern->formats[wdd->intern->selected_format_id];

    pixfmt->BytesPerColor = dd_fmt->surf_descr.ddpfPixelFormat.dwRGBBitCount / 8;

    if (pixfmt->BytesPerColor == 2)
        pixfmt->color_mode = 1;
    else if (pixfmt->BytesPerColor == 4)
        pixfmt->color_mode = 2;
    else
        pixfmt->color_mode = 0;

    if ( !(dd_fmt->surf_descr.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 ))
    {
        pixfmt->dwRBitMask = dd_fmt->surf_descr.ddpfPixelFormat.dwRBitMask;
        pixfmt->dwGBitMask = dd_fmt->surf_descr.ddpfPixelFormat.dwGBitMask;
        pixfmt->dwBBitMask = dd_fmt->surf_descr.ddpfPixelFormat.dwBBitMask;
        pixfmt->dwAlphaBitMask = dd_fmt->surf_descr.ddpfPixelFormat.dwRGBAlphaBitMask;


        DWORD bits = dd_fmt->surf_descr.ddpfPixelFormat.dwRBitMask;
        int i;

        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            pixfmt->dwRShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            pixfmt->dwRShift -= 8 - i;
        }
        else
            pixfmt->dwRShift = 0;

        bits = dd_fmt->surf_descr.ddpfPixelFormat.dwGBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            pixfmt->dwGShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            pixfmt->dwGShift -= 8 - i;
        }
        else
            pixfmt->dwGShift = 0;

        bits = dd_fmt->surf_descr.ddpfPixelFormat.dwBBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            pixfmt->dwBShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            pixfmt->dwBShift -= 8 - i;
        }
        else
            pixfmt->dwBShift = 0;

        bits = dd_fmt->surf_descr.ddpfPixelFormat.dwRGBAlphaBitMask;
        if ( bits )
        {
            for ( i = 0; !(bits & 1); i++ )
                bits >>= 1;

            pixfmt->dwAShift = i;

            for ( i = 0; bits & 1; i++ )
                bits >>= 1;

            pixfmt->dwAShift -= 8 - i;
        }
        else
            pixfmt->dwAShift = 0;


        pixfmt->FFFF0000__color = RGBAToColor(255, 255, 0, 0, pixfmt->dwRShift, pixfmt->dwGShift, pixfmt->dwBShift, pixfmt->dwAShift, pixfmt->dwRBitMask, pixfmt->dwGBitMask, pixfmt->dwBBitMask, pixfmt->dwAlphaBitMask);

        pixfmt->colors = (BYTE *)AllocVec(pixfmt->BytesPerColor * 256, 0);

        if ( !pixfmt->colors )
        {
            if ( w3d->bigdata )
            {
                if ( w3d->bigdata->primary__pixelformat.colors )
                {
                    nc_FreeMem(w3d->bigdata->primary__pixelformat.colors);
                    w3d->bigdata->primary__pixelformat.colors = NULL;
                }
                if ( w3d->bigdata->selected__pixelformat.colors )
                {
                    nc_FreeMem(w3d->bigdata->selected__pixelformat.colors);
                    w3d->bigdata->selected__pixelformat.colors = NULL;
                }
            }
            return 0;
        }
    }

    return 1;
}

void sub_439E30(__NC_STACK_windd *, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata )
    {
        for (int i = 0; i < 32; i++)
        {
            texCache *tex = &w3d->bigdata->texCh[i];

            if ( tex->ddrawPal )
            {
                tex->ddrawPal->Release();
                tex->ddrawPal = NULL;
            }

            if ( tex->d3dtex )
            {
                tex->d3dtex->Release();
                tex->d3dtex = NULL;
            }

            if ( tex->d3dtex2 )
            {
                tex->d3dtex2->Release();
                tex->d3dtex2 = NULL;
            }

            if ( tex->ddDrawSurface )
            {
                tex->ddDrawSurface->Release();
                tex->ddDrawSurface = NULL;
            }
        }
    }
}

int initTextureCache(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    w3d->bigdata->texCh_count = 0;

    memset(w3d->bigdata->texCh, 0, sizeof(texCache) * 32);

    PALETTEENTRY tmpPal[256];
    for (int i = 0; i < 256; i++)
    {
        tmpPal[i].peGreen = i;
        tmpPal[i].peBlue = i;
        tmpPal[i].peRed = i;
    }

    while (w3d->bigdata->texCh_count < 32)
    {
        texCache *tex = &w3d->bigdata->texCh[w3d->bigdata->texCh_count];
        windd_formats *wdd_fmt = &wdd->intern->formats[wdd->intern->selected_format_id];

        tex->used = 1;

        DDSURFACEDESC surfDescr;
        memset(&surfDescr, 0, sizeof(DDSURFACEDESC));
        surfDescr.dwSize = sizeof(DDSURFACEDESC);
        surfDescr.dwFlags = DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        surfDescr.dwWidth = 256;
        surfDescr.dwHeight = 256;

        memcpy(&surfDescr.ddpfPixelFormat, &wdd_fmt->surf_descr.ddpfPixelFormat, sizeof(surfDescr.ddpfPixelFormat));
        surfDescr.ddsCaps.dwCaps = DDSCAPS_ALLOCONLOAD | DDSCAPS_VIDEOMEMORY | DDSCAPS_TEXTURE;

        HRESULT res = ddraw->CreateSurface(&surfDescr, &tex->ddDrawSurface, NULL);

        if ( res == DDERR_OUTOFVIDEOMEMORY )
            break;

        if ( res )
        {
            log_d3d_fail("win3d.class/w3d_txtcache.c/InitTxtCache()", "IDirectDraw::CreateSurface()", res);
            sub_439E30(wdd, w3d);
            return 0;
        }

        if ( surfDescr.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 )
        {
            res = ddraw->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, tmpPal, &tex->ddrawPal, NULL);

            if ( res )
            {
                log_d3d_fail("win3d.class/w3d_txtcache.c/InitTxtCache()", "IDirectDraw::CreatePalette()", res);
                sub_439E30(wdd, w3d);
                return 0;
            }

            res = tex->ddDrawSurface->SetPalette(tex->ddrawPal);
            if ( res )
            {
                log_d3d_fail("win3d.class/w3d_txtcache.c", "IDirectDrawSurface::SetPalette()", res);
                sub_439E30(wdd, w3d);
                return 0;
            }
        }

        res = tex->ddDrawSurface->QueryInterface(IID_IDirect3DTexture2, (void **)&tex->d3dtex2);
        if ( res )
        {
            log_d3d_fail("win3d.class", "QueryInterface(IID_IDirect3DTextur2) failed.", res);
            sub_439E30(wdd, w3d);
            return 0;
        }

        res = tex->ddDrawSurface->QueryInterface(IID_IDirect3DTexture, (void **)&tex->d3dtex);
        if ( res )
        {
            log_d3d_fail("win3d.class", "QueryInterface(IID_IDIrect3DTexture) failed.", res);
            sub_439E30(wdd, w3d);
            return 0;
        }

        w3d->bigdata->texCh_count++;
    }
    return 1;
}

void sub_43BE88(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( w3d->bigdata->sceneBeginned )
    {
        OP_EXIT(bigdata->rendStates.lpInsPointer);

        void *lpInsStart = bigdata->rendStates.lpInsStart;

        OP_PROCESS_VERTICES(1, lpInsStart);
        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, 0, bigdata->rendStates.vertexCount, lpInsStart);

        execBuf *rendStates = &bigdata->rendStates;

        HRESULT res = wdd->intern->executebuffer->Unlock();
        if ( res )
        {
            log_d3d_fail("win3d.class", "IDirect3DExecuteBuffer::Unlock() failed", res);
            return;
        }

        D3DEXECUTEDATA execData;
        memset(&execData, 0, sizeof(D3DEXECUTEDATA));

        execData.dwVertexOffset = 0;
        execData.dwSize = sizeof(D3DEXECUTEDATA);
        execData.dwVertexCount = rendStates->vertexCount;
        execData.dwInstructionOffset = (char *)rendStates->lpInsStart - (char *)rendStates->lpBufStart;
        execData.dwInstructionLength = (char *)rendStates->lpInsPointer - (char *)rendStates->lpInsStart;

        res = wdd->intern->executebuffer->SetExecuteData(&execData);
        if ( res )
        {
            log_d3d_fail("win3d.class", "IDirect3DExecuteBuffer::SetExecuteData() failed", res);
        }
        else
        {
            res = wdd->intern->d3d1Dev->Execute(wdd->intern->executebuffer, wdd->intern->d3d2Viewport, D3DEXECUTE_UNCLIPPED);
            if ( res )
            {
                log_d3d_fail("win3d.class", "IDirect3DDevice::Execute() failed", res);
            }
        }

        rendStates->lpBufStart = NULL;
        rendStates->lpInsStart = NULL;
        rendStates->lpBufStart2 = NULL;
        rendStates->lpBufEnd = NULL;
        rendStates->lpInsPointer = NULL;
        rendStates->lpBufStart3 = NULL;
        rendStates->field_CC = 0;
        rendStates->vertexCount = 0;
        rendStates->field_B0 = 0;
    }
}

void sub_43BD50(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        D3DEXECUTEBUFFERDESC bufDescr;
        memset(&bufDescr, 0, sizeof(D3DEXECUTEBUFFERDESC));

        execBuf *execBuff = &w3d->bigdata->rendStates;
        bufDescr.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

        while ( true )
        {
            HRESULT res = wdd->intern->executebuffer->Lock(&bufDescr);
            if ( res == D3DERR_EXECUTE_LOCKED ) //0x887602CA
                wdd->intern->executebuffer->Unlock();
            else if (res == D3D_OK)
                break;
        }

        execBuff->field_B0 = 1;
        execBuff->lpBufStart = bufDescr.lpData;
        execBuff->lpBufStart2 = execBuff->lpBufStart;
        execBuff->lpBufStart3 = execBuff->lpBufStart2;

        execBuff->lpBufEnd = (char *)execBuff->lpBufStart + bufDescr.dwBufferSize;

        execBuff->lpInsStart = (char *)execBuff->lpBufStart + (bufDescr.dwBufferSize / 2);

        execBuff->field_CC = 0;
        execBuff->vertexCount = 0;

        execBuff->lpInsPointer = execBuff->lpInsStart;

        OP_PROCESS_VERTICES(1, execBuff->lpInsPointer);
        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, 0, 0, execBuff->lpInsPointer);
    }
}

void execBuff_initRenderStates(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( !wdd->use_simple_d3d )
        {
            execBuf *execBuff = &w3d->bigdata->rendStates;

            if ( w3d->bigdata->sceneBeginned )
            {
                if ( (char *)execBuff->lpInsPointer + 1000 > execBuff->lpBufEnd
                        || (char *)execBuff->lpBufStart3 + 1000 > execBuff->lpInsStart )
                {
                    sub_43BE88(wdd, w3d);
                    sub_43BD50(wdd, w3d);
                }
            }
            execBuff->lpStates = execBuff->lpInsPointer;
            execBuff->rendStatesCount = 0;

            OP_STATE_RENDER(execBuff->rendStatesCount, execBuff->lpInsPointer);
        }
    }
}

void win3d__beginScene(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    HRESULT res;

    if ( wdd->use_simple_d3d )
    {
        res = wdd->intern->d3d2dev->BeginScene();
        if ( res )
        {
            w3d->bigdata->sceneBeginned = 0;
            log_d3d_fail("win3d.class", "D3DDevice2::BeginScene() failed", res);
        }
        else
        {
            w3d->bigdata->sceneBeginned = 1;
        }
    }
    else
    {
        res = wdd->intern->d3d1Dev->BeginScene();
        if ( res )
        {
            w3d->bigdata->sceneBeginned = 0;
            log_d3d_fail("win3d.class", "D3DDevice::BeginScene() failed", res);
        }
        else
        {
            w3d->bigdata->sceneBeginned = 1;
        }
        sub_43BD50(wdd, w3d);
    }
}

void win3d__SetRenderState(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, D3DRENDERSTATETYPE type, DWORD state)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( wdd->use_simple_d3d )
        {
            HRESULT res = wdd->intern->d3d2dev->SetRenderState(type, state);

            if ( res )
                log_d3d_fail("win3d.class", "SetRenderState() failed", res);
        }
        else
        {
            execBuf *execBuff = &bigdata->rendStates;
            execBuff->field_B0 &= 0xFD;
            execBuff->rendStatesCount++;

            STATE_DATA(type, state, execBuff->lpInsPointer);
        }
    }
}

void execBuff_finishRenderStates(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( !wdd->use_simple_d3d )
        {
            execBuf *execBuff = &bigdata->rendStates;

            void *lpStates = bigdata->rendStates.lpStates;

            OP_STATE_RENDER(execBuff->rendStatesCount, lpStates);

            execBuff->lpStates = NULL;
            execBuff->rendStatesCount = 0;
        }
    }
}

void win3d__endScene(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( wdd->use_simple_d3d )
        {
            HRESULT res = wdd->intern->d3d2dev->EndScene();
            if ( res )
                log_d3d_fail("win3d.class", "D3DDevice2::EndScene() failed", res);
        }
        else
        {
            sub_43BE88(wdd, w3d);

            HRESULT res = wdd->intern->d3d1Dev->EndScene();
            if ( res )
                log_d3d_fail("win3d.class", "D3DDevice::EndScene() failed", res);
        }
    }
}

int initPolyEngine(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    execBuf *execBuff = &w3d->bigdata->rendStates;

    execBuff->rendStates[TEXTUREHANDLE].d3dRenderState = D3DRENDERSTATE_TEXTUREHANDLE;
    execBuff->rendStates[TEXTUREHANDLE].value = 0;  /* Texture handle */

    execBuff->rendStates[TEXTUREPERSPECTIVE].d3dRenderState = D3DRENDERSTATE_TEXTUREPERSPECTIVE;
    execBuff->rendStates[TEXTUREPERSPECTIVE].value = 0;  /* TRUE for perspective correction */

    execBuff->rendStates[SHADEMODE].d3dRenderState = D3DRENDERSTATE_SHADEMODE;
    execBuff->rendStates[SHADEMODE].value = D3DSHADE_GOURAUD;

    execBuff->rendStates[STIPPLEENABLE].d3dRenderState = D3DRENDERSTATE_STIPPLEENABLE;
    execBuff->rendStates[STIPPLEENABLE].value = 0; /* TRUE to enable stippling */

    execBuff->rendStates[SRCBLEND].d3dRenderState = D3DRENDERSTATE_SRCBLEND;
    execBuff->rendStates[SRCBLEND].value = D3DBLEND_ONE;

    execBuff->rendStates[DESTBLEND].d3dRenderState = D3DRENDERSTATE_DESTBLEND;
    execBuff->rendStates[DESTBLEND].value = D3DBLEND_ZERO;

    execBuff->rendStates[TEXTUREMAPBLEND].d3dRenderState = D3DRENDERSTATE_TEXTUREMAPBLEND;
    execBuff->rendStates[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATE;

    execBuff->rendStates[ALPHABLENDENABLE].d3dRenderState = D3DRENDERSTATE_ALPHABLENDENABLE;
    execBuff->rendStates[ALPHABLENDENABLE].value = 0; /* TRUE to enable alpha blending */

    execBuff->rendStates[ZWRITEENABLE].d3dRenderState = D3DRENDERSTATE_ZWRITEENABLE;
    execBuff->rendStates[ZWRITEENABLE].value = 1; /* TRUE to enable z writes */

    execBuff->rendStates[TEXTUREMAG].d3dRenderState = D3DRENDERSTATE_TEXTUREMAG;
    execBuff->rendStates[TEXTUREMAG].value = (w3d->filter != 0) + D3DFILTER_NEAREST; // Or D3DFILTER_LINEAR

    execBuff->rendStates[TEXTUREMIN].d3dRenderState = D3DRENDERSTATE_TEXTUREMIN;
    execBuff->rendStates[TEXTUREMIN].value = (w3d->filter != 0) + D3DFILTER_NEAREST; // Or D3DFILTER_LINEAR

    memcpy(execBuff->rendStates2, execBuff->rendStates, sizeof(rendState) * 11);

    win3d__beginScene(wdd, w3d);

    execBuff_initRenderStates(wdd, w3d);

    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_CLAMP);
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_ZENABLE, 1); /* TRUE to enable z test */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_TEXTUREMAG, (w3d->filter != 0) + D3DFILTER_NEAREST); // Or D3DFILTER_LINEAR
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_DITHERENABLE, w3d->dither);
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_ALPHABLENDENABLE, 0); /* TRUE to enable alpha blending */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_FOGENABLE, 0); /* TRUE to enable fog */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_SUBPIXEL, 1); /* TRUE to enable subpixel correction */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_STIPPLEDALPHA, dd_params.selected_device.can_stippling); /* TRUE to enable stippled alpha */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_STIPPLEENABLE, 0); /* TRUE to enable stippling */
    win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_COLORKEYENABLE, 1); /* TRUE to enable source colorkeyed textures */

    execBuff_finishRenderStates(wdd, w3d);

    win3d__endScene(wdd, w3d);

    w3d->bigdata->subt1_count = 0;
    memset(w3d->bigdata->subt1, 0, sizeof(wind3d_sub1) * 512);

    w3d->bigdata->dat_1C14_count = 0;
    memset(w3d->bigdata->dat_1C14, 0, 4); //?

    return 1;
}


size_t NC_STACK_win3d::func0(stack_vals *stak)
{
    get_keyvalue_from_ini(0, win3d_keys, 7);

    if ( win3d_keys[6].value.val ) // if force_emul
        return 0;

    dword_514EFC = 1;

    if ( !NC_STACK_windd::func0(stak) )
        return 0;

    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_windd *wdd = &stack__windd;

    w3d->dither = win3d_keys[0].value.val;
    w3d->filter = win3d_keys[1].value.val;
    w3d->antialias = win3d_keys[2].value.val;
    w3d->zbuf_when_tracy = win3d_keys[4].value.val;
    w3d->colorkey = win3d_keys[5].value.val;

    if ( can_srcblend )
        w3d->field_10 = 192;
    else
        w3d->field_10 = 128;

    if ( !win3dInitialisation(wdd, w3d) )
    {
        ypa_log_out("win3d.class: Initialization failed.\n");
        func1(NULL);
        return 0;
    }

    if ( !initPixelFormats(wdd, w3d) )
    {
        ypa_log_out("win3d.class: Pixelformat problems.\n");
        func1(NULL);
        return 0;
    }

    if ( !initTextureCache(wdd, w3d) )
    {
        ypa_log_out("win3d.class: Failed to initialize texture cache.\n");
        func1(NULL);
        return 0;
    }

    if ( !initPolyEngine(wdd, w3d) )
    {
        ypa_log_out("win3d.class: Failed to initialize polygon engine.\n");
        func1(NULL);
        return 0;
    }

    return 1;
}

void win3d_func1__sub1(__NC_STACK_windd *, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata )
    {
        if ( w3d->bigdata->primary__pixelformat.colors )
        {
            nc_FreeMem(w3d->bigdata->primary__pixelformat.colors);
            w3d->bigdata->primary__pixelformat.colors = NULL;
        }
        if ( w3d->bigdata->selected__pixelformat.colors )
        {
            nc_FreeMem(w3d->bigdata->selected__pixelformat.colors);
            w3d->bigdata->selected__pixelformat.colors = NULL;
        }
    }
}

void win3d_func1__sub0(__NC_STACK_windd *, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata )
    {
        nc_FreeMem(w3d->bigdata);
        w3d->bigdata = NULL;
    }
}

size_t NC_STACK_win3d::func1(stack_vals *stak)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_windd *wdd = &stack__windd;

    sub_439E30(wdd, w3d);

    win3d_func1__sub1(wdd, w3d);
    win3d_func1__sub0(wdd, w3d);

    return NC_STACK_windd::func1(stak);
}

size_t NC_STACK_win3d::func2(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case WDD_ATT_TEXFILT:
                setW3D_texFilt(stk->value.i_data);
                break;
            }
            stk++;
        }
    }
    return NC_STACK_windd::func2(stak);
}


// Draw line
void sub_43CD40(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, int x1, int y1, int x2, int y2, unsigned int r, unsigned int g, unsigned int b, int, int, int )
{
    if ( wdd->surface_locked_surfaceData )
    {
        int BytesPerColor = w3d->bigdata->primary__pixelformat.BytesPerColor;

        int rilWidth = wdd->surface_locked_pitch / (unsigned int)BytesPerColor;

        int xCount = abs(x2 - x1);
        int yCount = abs(y2 - y1);

        DWORD color = RGBAToColor(
                          r,
                          g,
                          b,
                          0,
                          w3d->bigdata->primary__pixelformat.dwRShift,
                          w3d->bigdata->primary__pixelformat.dwGShift,
                          w3d->bigdata->primary__pixelformat.dwBShift,
                          w3d->bigdata->primary__pixelformat.dwAShift,
                          w3d->bigdata->primary__pixelformat.dwRBitMask,
                          w3d->bigdata->primary__pixelformat.dwGBitMask,
                          w3d->bigdata->primary__pixelformat.dwBBitMask,
                          w3d->bigdata->primary__pixelformat.dwAlphaBitMask);

        int dy, dx;

        if ( xCount <= yCount )
        {
            if ( y2 <= y1 )
                dy = -rilWidth;
            else
                dy = rilWidth;

            if ( x2 <= x1 )
                dx = -1;
            else
                dx = 1;

            int tmp = xCount;
            xCount = yCount;
            yCount = tmp;
        }
        else
        {
            if ( x2 <= x1 )
                dy = -1;
            else
                dy = 1;

            if ( y2 <= y1 )
                dx = -rilWidth;
            else
                dx = rilWidth;
        }

        int v27 = 2 * yCount;
        int v17 = 2 * yCount - xCount;
        int v18 = 2 * (yCount - xCount);

        if ( BytesPerColor == 2 )
        {
            WORD *surf = ((WORD *)wdd->surface_locked_surfaceData + rilWidth * y1 + x1);

            for (int i = 0; i <= xCount; i++) // Verify i bound
            {
                *surf = color;
                if ( v17 > 0 )
                {
                    v17 += v18;
                    surf += dx;
                }
                else
                {
                    v17 += v27;
                }
                surf += dy;
            }
        }
        else if ( BytesPerColor == 4 )
        {
            DWORD *surf = ((DWORD *)wdd->surface_locked_surfaceData + rilWidth * y1 + x1);

            for (int i = 0; i <= xCount; i++) // Verify i bound
            {
                *surf = color;
                if ( v17 > 0 )
                {
                    v17 += v18;
                    surf += dx;
                }
                else
                {
                    v17 += v27;
                }
                surf += dy;
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func198(w3d_func198arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    sub_43CD40(wdd, w3d, x1, y1, x2, y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF,
               (rstr->field_8 >> 16) & 0xFF,
               (rstr->field_8 >> 8) & 0xFF,
               rstr->field_8 & 0xFF);

    return 1;
}


size_t NC_STACK_win3d::raster_func199(w3d_func199arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_43CD40(wdd, w3d,
               rstr->field_54c + arg->x1,
               rstr->field_550 + arg->y1,
               rstr->field_54c + arg->x2,
               rstr->field_550 + arg->y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF,
               (rstr->field_8 >> 16) & 0xFF,
               (rstr->field_8 >> 8) & 0xFF,
               rstr->field_8 & 0xFF);

    return 1;
}

int sub_420C74(xyxyNNN *a1, xyxyNNN *inout)
{
    int flag1 = 0;

    if ( inout->y1 > a1->y2 )
        flag1 = 8;
    else if ( inout->y1 < a1->y1 )
        flag1 = 4;

    if ( inout->x1 > a1->x2 )
        flag1 |= 2;
    else if ( inout->x1 < a1->x1 )
        flag1 |= 1;

    int flag2 = 0;

    if ( inout->y2 > a1->y2 )
        flag2 = 8;
    else if (inout->y2 < a1->y1)
        flag2 = 4;

    if ( inout->x2 > a1->x2 )
        flag2 |= 2;
    else if ( inout->x2 < a1->x1 )
        flag2 |= 1;

    int loop = 1;
    int v18 = 1;

    int tmp_x1 = inout->x1;
    int tmp_x2 = inout->x2;
    int tmp_y1 = inout->y1;
    int tmp_y2 = inout->y2;

    while ( loop )
    {
        if ( (flag2 | flag1) == 0 )
        {
            loop = 0;
            break;
        }

        if ( (flag2 & flag1) != 0 )
        {
            loop = 0;
            v18 = -1;
            break;
        }

        v18 = 0;

        int flag;

        if ( flag1 )
            flag = flag1;
        else
            flag = flag2;

        int dy = tmp_y2 - tmp_y1;
        int dx = tmp_x2 - tmp_x1;

        int vX = 0, vY = 0;

        if ( flag & 4 )
        {
            vY = a1->y1;
            vX = dx * (a1->y1 - tmp_y1) / dy + tmp_x1;
        }
        else if ( flag & 8 )
        {
            vY = a1->y2;
            vX = dx * (a1->y2 - tmp_y1) / dy + tmp_x1;
        }
        else if ( flag & 1 )
        {
            vX = a1->x1;
            vY = dy * (a1->x1 - tmp_x1) / (tmp_x2 - tmp_x1) + tmp_y1;
        }
        else if ( flag & 2 )
        {
            vX = a1->x2;
            vY = dy * (a1->x2 - tmp_x1) / (tmp_x2 - tmp_x1) + tmp_y1;
        }

        if ( flag == flag1 )
        {
            tmp_y1 = vY;
            tmp_x1 = vX;
        }
        else
        {
            tmp_y2 = vY;
            tmp_x2 = vX;
        }

        int newFlag = 0;

        if ( vY > a1->y2 )
            newFlag = 8;
        else if ( vY < a1->y1 )
            newFlag = 4;

        if ( vX > a1->x2 )
            newFlag |= 2;
        else if ( vX < a1->x1 )
            newFlag |= 1;

        if ( flag == flag1 )
            flag1 = newFlag;
        else
            flag2 = newFlag;
    }

    if ( v18 == 0 )
    {
        inout->x1 = tmp_x1;
        inout->y1 = tmp_y1;
        inout->x2 = tmp_x2;
        inout->y2 = tmp_y2;
    }

    return v18;
}

void sub_420EDC(__NC_STACK_raster *rstr, __NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, int x1, int y1, int x2, int y2, int r, unsigned int g, unsigned int b, unsigned int a11, int a12, int a13)
{
    xyxyNNN tmp1;

    tmp1.x1 = x1;
    tmp1.x2 = x2;
    tmp1.y1 = y1;
    tmp1.y2 = y2;

    if ( sub_420C74(&rstr->field_24, &tmp1) != -1 )
    {
        xyxyNNN tmp2 = tmp1;

        int v14;

        if ( rstr->field_38.x1 == rstr->field_38.x2 )
            v14 = -1;
        else
            v14 = sub_420C74(&rstr->field_38, &tmp2);

        if ( v14 == -1 )
        {
            sub_43CD40(wdd, w3d, tmp2.x1, tmp2.y1, tmp2.x2, tmp2.y2, r, g, b, a11, a12, a13);
        }
        else if ( !v14 )
        {
            if ( tmp2.x2 != tmp1.x2 || tmp2.y2 != tmp1.y2 )
            {
                if ( tmp2.x1 != tmp1.x1 || tmp2.y1 != tmp1.y1 )
                {
                    sub_43CD40(wdd, w3d, tmp1.x1, tmp1.y1, tmp2.x1, tmp2.y1, r, g, b, a11, a12, a13);
                    sub_43CD40(wdd, w3d, tmp2.x2, tmp2.y2, tmp1.x2, tmp1.y2, r, g, b, a11, a12, a13);
                }
                else
                {
                    sub_43CD40(wdd, w3d, tmp2.x2, tmp2.y2, tmp1.x2, tmp1.y2, r, g, b, a11, a12, a13);
                }
            }
            else
            {
                sub_43CD40(wdd, w3d, tmp1.x1, tmp1.y1, tmp2.x1, tmp2.y1, r, g, b, a11, a12, a13);
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func200(w3d_func198arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    sub_420EDC(rstr, wdd, w3d, x1, y1, x2, y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF,
               (rstr->field_8 >> 16) & 0xFF,
               (rstr->field_8 >> 8) & 0xFF,
               rstr->field_8 & 0xFF);

    return 1;
}

size_t NC_STACK_win3d::raster_func201(w3d_func199arg *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_420EDC(rstr, wdd, w3d,
               rstr->field_54c + arg->x1,
               rstr->field_550 + arg->y1,
               rstr->field_54c + arg->x2,
               rstr->field_550 + arg->y2,
               (rstr->field_4 >> 16) & 0xFF,
               (rstr->field_4 >> 8) & 0xFF,
               rstr->field_4 & 0xFF,
               (rstr->field_8 >> 16) & 0xFF,
               (rstr->field_8 >> 8) & 0xFF,
               rstr->field_8 & 0xFF);

    return 1;
}

void sub_43CEE0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, void *srcBuf, int width, int a1, int a2, int a3, int a4, int x1, int y1, int x2, int y2)
{
    if ( wdd->surface_locked_surfaceData )
    {
        if (w3d->bigdata->primary__pixelformat.BytesPerColor == 2)
        {
            WORD *dstSurf = (WORD *)wdd->surface_locked_surfaceData;
            if ( x2 != x1 && y2 != y1 )
            {
                DWORD v21 = a2 << 16;
                for (int i = y1; i < y2; i++ )
                {
                    WORD *buf = &dstSurf[i * (wdd->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor)];

                    DWORD v13 = a1 << 16;
                    if ( x1 < x2 )
                    {
                        WORD *cur = buf + x1;
                        WORD *bend = buf + x2;
                        while(cur < bend)
                        {
                            *cur = ((WORD *)srcBuf)[width * (v21 >> 16) + (v13 >> 16)];
                            v13 += ((a3 - a1) << 16) / (x2 - x1);
                            cur++;
                        }
                    }
                    v21 += ((a4 - a2) << 16) / (y2 - y1);
                }
            }
        }
        else if (w3d->bigdata->primary__pixelformat.BytesPerColor == 4)
        {
            DWORD *dstSurf = (DWORD *)wdd->surface_locked_surfaceData;
            if ( x2 != x1 && y2 != y1 )
            {
                DWORD v21 = a2 << 16;
                for (int i = y1; i < y2; i++ )
                {
                    DWORD *buf = &dstSurf[i * (wdd->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor)];

                    DWORD v13 = a1 << 16;
                    if ( x1 < x2 )
                    {
                        DWORD *cur = buf + x1;
                        DWORD *bend = buf + x2;
                        while(cur < bend)
                        {
                            *cur =  ((DWORD*)srcBuf)[width * (v21 >> 16) + (v13 >> 16)];
                            v13 += ((a3 - a1) << 16) / (x2 - x1);
                            cur++;
                        }
                    }
                    v21 += ((a4 - a2) << 16) / (y2 - y1);
                }
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func202(rstr_arg204 *arg)
{
    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    bitmap_intern *pbitm = arg->pbitm;

    int a1 = (arg->float4 + 1.0) * (arg->pbitm->width / 2);
    int a2 = (arg->float8 + 1.0) * (arg->pbitm->height / 2);
    int a3 = (arg->floatC + 1.0) * (arg->pbitm->width / 2);
    int a4 = (arg->float10 + 1.0) * (arg->pbitm->height / 2);

    int a5 = rstr->field_554 * (arg->float14 + 1.0);
    int a6 = rstr->field_558 * (arg->float18 + 1.0);
    int a7 = rstr->field_554 * (arg->float1C + 1.0);
    int a8 = rstr->field_558 * (arg->float20 + 1.0);

    sub_43CEE0(wdd, w3d, (WORD *)pbitm->buffer, pbitm->width, a1, a2, a3, a4, a5, a6, a7, a8);
    return 1;
}


int win3d_func204__sub0(__NC_STACK_raster *rstr, rstr_loc204 *arg)
{
    if ( rstr->field_24.x1 > arg->dword20 || rstr->field_24.x2 < arg->dword18 || rstr->field_24.y1 > arg->dword24 || rstr->field_24.y2 < arg->dword1C )
        return 0;

    int d04 = arg->dword4;
    int d08 = arg->dword8;
    int d0C = arg->dwordC;
    int d10 = arg->dword10;

    int d18 = arg->dword18;
    int d1C = arg->dword1C;
    int d20 = arg->dword20;
    int d24 = arg->dword24;


    if ( d18 < rstr->field_24.x1 )
    {
        d04 += (rstr->field_24.x1 - d18) * (d0C - d04) / (d20 - d18);
        d18 = rstr->field_24.x1;
    }

    if ( d20 > rstr->field_24.x2 )
    {
        d0C += (d0C - d04) * (rstr->field_24.x2 - d20) / (d20 - d18);
        d20 = rstr->field_24.x2;
    }

    if ( d1C < rstr->field_24.y1 )
    {
        d08 += (d10 - d08) * (rstr->field_24.y1 - d1C) / (d24 - d1C);
        d1C = rstr->field_24.y1;
    }

    if ( d24 > rstr->field_24.y2 )
    {
        d10 += (rstr->field_24.y2 - d24) * (d10 - d08) / (d24 - d1C);
        d24 = rstr->field_24.y2;
    }

    arg->dword4 = d04;
    arg->dword8 = d08;
    arg->dwordC = d0C;
    arg->dword10 = d10;
    arg->dword18 = d18;
    arg->dword1C = d1C;
    arg->dword20 = d20;
    arg->dword24 = d24;

    return 1;
}


size_t NC_STACK_win3d::raster_func204(rstr_arg204 *arg)
{

    __NC_STACK_raster *rstr = &stack__raster;
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    rstr_loc204 loc;

    loc.pbitm = arg->pbitm;
    loc.dword4 = (arg->float4 + 1.0) * (arg->pbitm->width / 2);
    loc.dword8 = (arg->float8 + 1.0) * (arg->pbitm->height / 2);
    loc.dwordC = (arg->floatC + 1.0) * (arg->pbitm->width / 2);
    loc.dword10 = (arg->float10 + 1.0) * (arg->pbitm->height / 2);

    loc.dword18 = (arg->float14 + 1.0) * rstr->field_554;
    loc.dword1C = (arg->float18 + 1.0) * rstr->field_558;
    loc.dword20 = (arg->float1C + 1.0) * rstr->field_554;
    loc.dword24 = (arg->float20 + 1.0) * rstr->field_558;

    if ( win3d_func204__sub0(rstr, &loc) )
        sub_43CEE0(wdd, w3d, (WORD *)loc.pbitm->buffer, loc.pbitm->width, loc.dword4, loc.dword8, loc.dwordC, loc.dword10, loc.dword18, loc.dword1C, loc.dword20, loc.dword24);

    return 1;
}

void sb_0x43b518__sub0__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( wdd->use_simple_d3d )
        {
            win3d__SetRenderState(wdd, w3d, D3DRENDERSTATE_FLUSHBATCH, 1);
        }
        else
        {
            sub_43BE88(wdd, w3d);
            sub_43BD50(wdd, w3d);
        }
    }
}

int loadTextureToCache(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, texStru *tex, int id)
{
    texCache *ptx = &w3d->bigdata->texCh[id];
    if ( !wdd->use_simple_d3d && ptx->loadedFromTexture.dx1)
    {
        HRESULT res = ptx->d3dtex->Unload();
        if (res)
        {
            log_d3d_fail("win3d.class", "IDirect3DTexture::Unload()", res);
            return 0;
        }
    }

    ptx->field_4 = 1;
    ptx->used = (ptx->used & 0xFFFFFFFC) | 2;
    ptx->loadedFromTexture.dx2 = tex->texture.dx2;
    ptx->txStru = tex;

    if ( w3d->bigdata->selected__pixelformat.BytesPerColor == 1 )
    {
        DDCOLORKEY ckey;
        ckey.dwColorSpaceLowValue = 0;
        ckey.dwColorSpaceHighValue = 0;
        ptx->ddDrawSurface->SetColorKey(DDCKEY_SRCBLT, &ckey);
    }

    return ptx->d3dtex2->Load(ptx->loadedFromTexture.dx2) == D3D_OK;
}

D3DTEXTUREHANDLE getTexHandle(__NC_STACK_windd *wdd, __NC_STACK_win3d *, texCache *tex)
{
    D3DTEXTUREHANDLE d3dtexHandle;
    HRESULT res;

    if ( wdd->use_simple_d3d )
        res = tex->d3dtex2->GetHandle(wdd->intern->d3d2dev, &d3dtexHandle);
    else
        res = tex->d3dtex->GetHandle(wdd->intern->d3d1Dev, &d3dtexHandle);
    if ( res )
        return 0;

    return d3dtexHandle;
}


D3DTEXTUREHANDLE sb_0x43b518__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, texStru *tex, int a4)
{
    if ( !w3d->bigdata->sceneBeginned )
        return 0;

    win3d_bigdata *bigdata = w3d->bigdata;


    int id = -1;

    for (int i = 0; i < bigdata->texCh_count; i++)
    {
        texCache *ptx = &bigdata->texCh[i];
        if ( ptx->loadedFromTexture.dx2 == tex->texture.dx2 )
        {
            ptx->used = (ptx->used & 0xFFFFFFFC) | 2;
            ptx->field_4++;
            return getTexHandle(wdd, w3d, ptx);
        }

        if ( ptx->used & 1 )
            id = i;
    }

    if ( id == -1 )
    {
        if ( a4 )
        {
            int idd = 0;

            for (int i = 0; i < bigdata->texCh_count; i++)
            {
                if ( bigdata->texCh[i].field_4 < bigdata->texCh[idd].field_4 )
                    idd = i;
            }

            sb_0x43b518__sub0__sub0(wdd, w3d);
            if ( loadTextureToCache(wdd, w3d, tex, idd) )
                return getTexHandle(wdd, w3d, &w3d->bigdata->texCh[idd]);
        }
    }
    else if ( loadTextureToCache(wdd, w3d, tex, id) )
    {
        return getTexHandle(wdd, w3d, &w3d->bigdata->texCh[id]);
    }
    return 0;
}

int SetRenderStates(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, int arg)
{
    execBuf *execBuff = &w3d->bigdata->rendStates;

    int statesChanged = 0;

    if ( arg )
    {
        statesChanged = 11;
    }
    else
    {
        for (int i = 0; i < 11; i++)
            if (execBuff->rendStates2[i].value != execBuff->rendStates[i].value)
                statesChanged++;
    }

    if ( statesChanged )
    {
        execBuff_initRenderStates(wdd, w3d);

        for (int i = 0; i < 11; i++)
        {
            if (execBuff->rendStates2[i].value != execBuff->rendStates[i].value)
                win3d__SetRenderState(wdd, w3d, execBuff->rendStates2[i].d3dRenderState, execBuff->rendStates2[i].value);

            execBuff->rendStates[i].value = execBuff->rendStates2[i].value;
        }

        execBuff_finishRenderStates(wdd, w3d);
        return 1;
    }

    return 0;
}


void DrawPrimitive(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, D3DTLVERTEX *vtx, int vtxCount)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    if ( !w3d->bigdata->sceneBeginned )
        return;

    if ( wdd->use_simple_d3d )
    {
        HRESULT res = wdd->intern->d3d2dev->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DVT_TLVERTEX, vtx, vtxCount, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS );
        if ( res )
            log_d3d_fail("win3d.class", "DrawPrimitive() failed", res);
    }
    else
    {
        execBuf *execBuff = &bigdata->rendStates;
        if ( bigdata->sceneBeginned )
        {
            if ( ((char *)execBuff->lpInsPointer + 1000) > (char *)execBuff->lpBufEnd ||
                    ((char *)execBuff->lpBufStart3 + 1000) > (char *)execBuff->lpInsStart )
            {
                sub_43BE88(wdd, w3d);
                sub_43BD50(wdd, w3d);
            }
        }

        void *lp = execBuff->lpInsPointer;

        if ( execBuff->field_B0 & 2 )
        {
            void *pTriang = execBuff->field_DC;
            execBuff->field_E0 += vtxCount - 2;

            OP_TRIANGLE_LIST(execBuff->field_E0, pTriang);
        }
        else
        {
            execBuff->field_DC = lp;
            execBuff->field_E0 = vtxCount - 2;

            OP_TRIANGLE_LIST(execBuff->field_E0, lp);
        }
        for(int i = 1; i < vtxCount - 1; i++)
        {
            int tmp = execBuff->field_CC;

            ((LPD3DTRIANGLE)lp)->v1 = tmp;
            ((LPD3DTRIANGLE)lp)->v2 = tmp + i;
            ((LPD3DTRIANGLE)lp)->v3 = tmp + i + 1;

            if (i == 1)
                ((LPD3DTRIANGLE)lp)->wFlags = D3DTRIFLAG_STARTFLAT(vtxCount - 3);
            else
                ((LPD3DTRIANGLE)lp)->wFlags = D3DTRIFLAG_EVEN;

            lp = (char *)lp + sizeof(D3DTRIANGLE);
        }

        execBuff->lpInsPointer = lp;

        execBuff->field_CC += vtxCount;
        execBuff->vertexCount += vtxCount;

        VERTEX_DATA(vtx, vtxCount, execBuff->lpBufStart3);

        execBuff->field_B0 |= 2;
    }
}

void sb_0x43b518(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, polysDatSub *polysDat, texStru *tex, int a5, int a6)
{
    D3DTEXTUREHANDLE texHndl = 0;

    win3d_bigdata *bigdata = w3d->bigdata;
    if ( !bigdata->sceneBeginned )
        return;

    if ( polysDat->vertexCount < 3 || polysDat->vertexCount > 12 )
        return;

    //Store for rendering later ( from 214 method )
    if ( polysDat->renderFlags & 0x30 && !a6 )
    {
        if ( bigdata->dat_1C14_count < 512 )
        {
            bigdata->dat_1C14[bigdata->dat_1C14_count].polyData = polysDat;
            bigdata->dat_1C14[bigdata->dat_1C14_count].tex = tex;
            bigdata->dat_1C14_count++;
        }
        return;
    }

    //Store for rendering later ( from 214 method )
    if ( polysDat->renderFlags & 3 )
    {
        texHndl = sb_0x43b518__sub0(wdd, w3d, tex, a5);
        if ( !texHndl )
        {
            if ( bigdata->subt1_count < 512 )
            {
                bigdata->subt1[bigdata->subt1_count].polyData = polysDat;
                bigdata->subt1[bigdata->subt1_count].tex = tex;
                bigdata->subt1_count++;
            }
            return;
        }
    }


    D3DTLVERTEX vOut[24];

    int x_max_id = 0;
    int x_min_id = 0;
    int y_max_id = 0;
    int y_min_id = 0;

    for (int i = 0; i < polysDat->vertexCount; i++)
    {
        vOut[i].sx = (polysDat->vertexes[i].sx + 1.0) * w3d->bigdata->_dx;
        vOut[i].sy = (polysDat->vertexes[i].sy + 1.0) * w3d->bigdata->_dy;
        vOut[i].sz = polysDat->vertexes[i].sz / 8192.0;
        vOut[i].color = 0xFFFFFFFF;
        vOut[i].specular = 0;
        vOut[i].tu = 0;
        vOut[i].tv = 0;
        vOut[i].rhw = 1.0 / polysDat->vertexes[i].sz;

        if ( vOut[i].sx > vOut[y_max_id].sx )
            x_max_id = i;
        else if ( vOut[i].sx < vOut[y_min_id].sx )
            x_min_id = i;

        if ( vOut[i].sy > vOut[y_max_id].sy )
            y_max_id = i;
        else if ( vOut[i].sy < vOut[y_min_id].sy )
            y_min_id = i;
    }

    float xLen = vOut[x_max_id].sx - vOut[x_min_id].sx;
    if ( xLen <= 0.0 )
        return;

    float yLen = vOut[y_max_id].sy - vOut[y_min_id].sy;
    if ( yLen <= 0.0 )
        return;

    if ( polysDat->renderFlags & 2 )
    {
        if ( xLen < 32.0 && yLen < 32.0 )
        {
            polysDat->renderFlags = (polysDat->renderFlags & 0xFFFFFFFC) | 1;
        }
    }

    execBuf *execBuff = &bigdata->rendStates;

    execBuff->rendStates2[TEXTUREPERSPECTIVE].value = 0;
    execBuff->rendStates2[SHADEMODE].value = D3DSHADE_FLAT;
    execBuff->rendStates2[STIPPLEENABLE].value = 0;
    execBuff->rendStates2[SRCBLEND].value = D3DBLEND_ONE;
    execBuff->rendStates2[DESTBLEND].value = D3DBLEND_ZERO;
    execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_COPY;
    execBuff->rendStates2[ALPHABLENDENABLE].value = 0;
    execBuff->rendStates2[ZWRITEENABLE].value = 1;
    execBuff->rendStates2[TEXTUREHANDLE].value = 0;

    execBuff->rendStates2[TEXTUREMAG].value = (w3d->filter != 0) + 1;
    execBuff->rendStates2[TEXTUREMIN].value = (w3d->filter != 0) + 1;

    if ( polysDat->renderFlags & 1 )
    {
        execBuff->rendStates2[TEXTUREHANDLE].value = texHndl;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vOut[i].tu = polysDat->tu_tv[i].tu;
            vOut[i].tv = polysDat->tu_tv[i].tv;
        }
    }
    else if ( polysDat->renderFlags & 2 )
    {
        execBuff->rendStates2[TEXTUREPERSPECTIVE].value = 1;
        execBuff->rendStates2[TEXTUREHANDLE].value = texHndl;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vOut[i].tu = polysDat->tu_tv[i].tu;
            vOut[i].tv = polysDat->tu_tv[i].tv;
        }
    }
    else
    {
        for (int i = 0; i < polysDat->vertexCount; i++)
            vOut[i].color = 0xFF000000;
    }

    if ( polysDat->renderFlags & 0xC )
    {
        execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATE;
        execBuff->rendStates2[SHADEMODE].value = D3DSHADE_GOURAUD;

        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            int comp = (1.0 - polysDat->color[i]) * 255.0;
            vOut[i].color = 0xFF000000 | (comp << 16) | (comp << 8) | comp;
        }
    }

    if ( polysDat->renderFlags & 0x20 )
    {
        if ( !w3d->zbuf_when_tracy )
            execBuff->rendStates2[ZWRITEENABLE].value = 0;

        if ( dd_params.selected_device.can_destblend )
        {
            execBuff->rendStates2[ALPHABLENDENABLE].value = 1;
            execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATEALPHA;
            execBuff->rendStates2[SRCBLEND].value = D3DBLEND_ONE;
            execBuff->rendStates2[DESTBLEND].value = D3DBLEND_ONE;
            execBuff->rendStates2[SHADEMODE].value = D3DSHADE_FLAT;
        }
        else if ( dd_params.selected_device.can_srcblend )
        {
            execBuff->rendStates2[ALPHABLENDENABLE].value = 1;
            execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATEALPHA;
            execBuff->rendStates2[SRCBLEND].value = D3DBLEND_SRCALPHA;
            execBuff->rendStates2[DESTBLEND].value = D3DBLEND_INVSRCALPHA;
            execBuff->rendStates2[SHADEMODE].value = D3DSHADE_FLAT;
        }
        else if ( dd_params.selected_device.can_stippling )
        {
            execBuff->rendStates2[ALPHABLENDENABLE].value = 1;
            execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATEALPHA;
            execBuff->rendStates2[SRCBLEND].value = D3DBLEND_SRCALPHA;
            execBuff->rendStates2[DESTBLEND].value = D3DBLEND_INVSRCALPHA;
            execBuff->rendStates2[STIPPLEENABLE].value = 1;
            execBuff->rendStates2[SHADEMODE].value = D3DSHADE_FLAT;
        }
        for (int i = 0; i < polysDat->vertexCount; i++)
        {
            vOut[i].color &= 0x00FFFFFF;
            vOut[i].color |= w3d->field_10 << 24;
        }
    }
    else if ( polysDat->renderFlags & 0x10 )
    {
        if ( !w3d->zbuf_when_tracy )
            execBuff->rendStates2[ZWRITEENABLE].value = 0;

        if ( w3d->bigdata->selected__pixelformat.BytesPerColor != 1 )
        {
            execBuff->rendStates2[DESTBLEND].value = D3DBLEND_INVSRCALPHA;
            execBuff->rendStates2[SRCBLEND].value = D3DBLEND_SRCALPHA;
        }

        execBuff->rendStates2[ALPHABLENDENABLE].value = 1;
        execBuff->rendStates2[TEXTUREMAG].value = D3DFILTER_NEAREST;
        execBuff->rendStates2[TEXTUREMIN].value = D3DFILTER_NEAREST;
        execBuff->rendStates2[TEXTUREMAPBLEND].value = D3DTBLEND_MODULATE;
    }

    SetRenderStates(wdd, w3d, 0);
    DrawPrimitive(wdd, w3d, vOut, polysDat->vertexCount);
    return;
}

size_t NC_STACK_win3d::raster_func206(polysDatSub *arg)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    if ( arg->pbitm )
        sb_0x43b518(wdd, w3d, arg, arg->pbitm->ddrawSurfTex, 0, 0);
    else
        sb_0x43b518(wdd, w3d, arg, NULL, 0, 0);

    return 1;
}

void win3d_func209__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, tiles_stru **tiles, char *cmdline, char **arr)
{
    ////printf("CLEAN ME %s\n","win3d_func209__sub0");

    int v11;


    if ( wdd->surface_locked_surfaceData )
    {
        win3d_bigdata *bigdata = w3d->bigdata;
        int bytesPerColor = bigdata->primary__pixelformat.BytesPerColor;
        DWORD FFFF0000__color = w3d->bigdata->primary__pixelformat.FFFF0000__color;

        char *curpos = cmdline;
        int w_pixels = wdd->surface_locked_pitch / bytesPerColor;
        tiles_stru *tile = NULL;


        int x_out = 0;
        int y_out = 0;

        int x_out_txt = 0;
        int y_out_txt = 0;
        int txt_flag = 0;

        int y_pos_line = 0;
        int x_pos_line = 0;



        int rilHeight, rilWidth;

        if ( wdd->field_50 & 8 )
        {
            rilHeight = wdd->height / 2;
            rilWidth = wdd->width / 2;
        }
        else
        {
            rilHeight = wdd->height;
            rilWidth = wdd->width;
        }

        int halfWidth = rilWidth / 2;
        int halfHeight = rilHeight / 2;

        int line_width = 0;
        int line_height = 0;

        //if v11 = 0 - clone first column of tile  (count = line_width)
        //if v11 = 1 - normal copy of tile
        v11 = 1;


        int x_off = 0;
        int y_off = 0;


        char *positions[64];
        int position_idx = 0;

        positions[position_idx] = NULL;
        position_idx++;

        while ( 1 )
        {
            int v13 = FontUA::get_u8(&curpos);

            if ( v13 )
            {
                tile_xy *chrr = &tile->chars[v13];

                int cpy_width, src_width;

                if ( line_width )
                    cpy_width = line_width - x_off;
                else
                    cpy_width = chrr->width - x_off;

                int cpy_height = line_height - y_off;

                if ( v11 )
                    src_width = tile->field_4->width - cpy_width;
                else
                    src_width = tile->field_4->width;

                if (bytesPerColor == 2)
                {

                    WORD *srcpixel = (WORD *)tile->field_4->buffer + chrr->byteoff + x_off + y_off * tile->field_4->width;
                    WORD *dstpixel = (WORD *)wdd->surface_locked_surfaceData + w_pixels * y_out + x_out;

                    for (int j = cpy_height; j > 0; j--)
                    {
                        for (int i = cpy_width; i > 0; i--)
                        {
                            if (*srcpixel != FFFF0000__color)
                                *dstpixel = *srcpixel;

                            srcpixel += v11;
                            dstpixel++;
                        }

                        srcpixel += src_width;
                        dstpixel += (w_pixels - cpy_width);
                    }

                }
                else if (bytesPerColor == 4)
                {
                    DWORD *srcpixel = (DWORD *)tile->field_4->buffer + chrr->byteoff + x_off + y_off * tile->field_4->width;
                    DWORD *dstpixel = (DWORD *)wdd->surface_locked_surfaceData + w_pixels * y_out + x_out;

                    for (int j = cpy_height; j > 0; j--)
                    {
                        for (int i = cpy_width; i > 0; i--)
                        {
                            if (*srcpixel != FFFF0000__color)
                                *dstpixel = *srcpixel;

                            srcpixel += v11;
                            dstpixel++;
                        }

                        srcpixel += src_width;
                        dstpixel += (w_pixels - cpy_width);
                    }
                }
                else
                {
                    printf("win3d_func209__sub0, BytesPerPixel == %d\n", bytesPerColor);
                }

                line_width = 0;
                x_off = 0;
                x_out += cpy_width;
                v11 = 1;
            }
            else // 0
            {
                int opcode = FontUA::get_u8(&curpos);

                switch ( opcode )
                {
                case 0: // End
                    position_idx--;

                    curpos = positions[position_idx];
                    if ( curpos )
                        break;

                    DDSURFACEDESC a1;
                    if ( sb_0x4bf0a0(&a1) )
                    {
                        wdd->surface_locked_surfaceData = a1.lpSurface;
                        wdd->surface_locked_pitch = a1.lPitch;
                    }
                    return;

                case 1: // x pos from center
                    x_out = halfWidth + FontUA::get_s16(&curpos);
                    x_pos_line = x_out;

                    y_pos_line = y_out;
                    y_off = 0;

                    line_height = tile->font_height;
                    break;

                case 2: // y pos from center
                    y_out = halfHeight + FontUA::get_s16(&curpos);
                    x_pos_line = x_out;

                    y_pos_line = y_out;
                    y_off = 0;

                    line_height = tile->font_height;
                    break;

                case 3: //xpos
                    x_out = FontUA::get_s16(&curpos);
                    if ( x_out < 0 )
                        x_out += w_pixels;

                    x_pos_line = x_out;
                    y_pos_line = y_out;

                    line_height = tile->font_height;
                    y_off = 0;
                    break;

                case 4: //ypos
                    y_out = FontUA::get_s16(&curpos);
                    if ( y_out < 0 )
                        y_out += rilHeight;

                    x_pos_line = x_out;
                    y_pos_line = y_out;

                    line_height = tile->font_height;
                    y_off = 0;
                    break;

                case 5: //add to x pos
                    x_out += FontUA::get_s16(&curpos);
                    break;

                case 6: //add to y pos
                    y_out += FontUA::get_s16(&curpos);
                    break;

                case 7: //next line
                    y_out = (line_height - y_off) + y_pos_line;
                    y_pos_line = y_out;
                    x_out = x_pos_line;

                    y_off = 0;
                    line_height = tile->font_height;
                    break;

                case 8: // Select tileset
                    tile = tiles[FontUA::get_u8(&curpos)];
                    break;

                case 9: // Include another cmdlist source
                {
                    int azaza = FontUA::get_u8(&curpos);
                    positions[position_idx] = curpos;
                    position_idx++;
                    curpos = arr[azaza];
                }
                break;

                case 10:
                    line_width = FontUA::get_u8(&curpos);

                    v11 = 0;
                    x_off = 0;

                    break;

                case 11:

                    line_width = FontUA::get_u8(&curpos);

                    v11 = 0;
                    x_off = 0;

                    line_width -= (x_out - x_pos_line);
                    break;

                case 12: // Set x offset
                    x_off = FontUA::get_u8(&curpos);
                    break;

                case 13: // Set x width
                    line_width = FontUA::get_u8(&curpos);
                    break;

                case 14: // Set y offset
                    y_off = FontUA::get_u8(&curpos);
                    break;

                case 15: // Set y height
                    line_height = FontUA::get_u8(&curpos);
                    break;

                case 16: // Full reset tileset
                    tile = tiles[FontUA::get_u8(&curpos)];
                    line_height = tile->font_height;
                    y_off = 0;
                    break;

                case 17:
                    line_width = FontUA::get_s16(&curpos);
                    v11 = 0;
                    x_off = 0;
                    line_width -= (x_out - x_pos_line);
                    break;

                case 18: // Add text
                {
                    int block_width = FontUA::get_s16(&curpos);
                    int flag = txt_flag | FontUA::get_u16(&curpos);

                    char *txtpos = (char *)curpos;

                    curpos += strlen(txtpos) + 1;
                    dbcs_AddText(txtpos, x_out_txt, y_out_txt, block_width, tile->font_height, flag);
                }
                break;

                case 19: // Copy current x/y pos for text output
                    x_out_txt = x_out;
                    y_out_txt = y_out;
                    break;

                case 20: // Add txtout flag
                    txt_flag |= FontUA::get_u16(&curpos);
                    break;

                case 21: // Delete txtout flag
                    txt_flag &= ~(FontUA::get_u16(&curpos));
                    break;

                case 22: // set color for font
                {
                    int r = FontUA::get_u16(&curpos);

                    int g = FontUA::get_u16(&curpos);

                    int b = FontUA::get_u16(&curpos);

                    dbcs_AddText(0, r, g, b, 0, 0x20);
                }
                break;
                }
            }
        }
    }
}

void NC_STACK_win3d::raster_func209(w3d_a209 *arg)
{
    win3d_func209__sub0(&stack__windd, &stack__win3d, this->stack__raster.tiles, arg->cmdbuf, arg->includ);
}

void win3d_func213__sub0__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    for(int texid = 0; texid < w3d->bigdata->texCh_count; texid++)
    {
        texCache *texCh = &w3d->bigdata->texCh[texid];
        if ( texCh->ddDrawSurface->IsLost() )
        {
            texCh->ddDrawSurface->Restore();
            if ( texCh->loadedFromTexture.dx1 )
                loadTextureToCache(wdd, w3d, texCh->txStru, texid);
        }
        texCh->used &= 0xFFFFFFFDu;
    }
}

void win3d_func213__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    if ( wdd->hwnd )
    {
        win3d_func213__sub0__sub0(wdd, w3d);
        win3d__beginScene(wdd, w3d);
    }
}

void NC_STACK_win3d::raster_func213(polysDatSub *)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func213__sub0(wdd, w3d);
}

int sub_43BB6C(const void * a, const void * b)
{
    wind3d_sub1 *a1 = (wind3d_sub1 *)a;
    wind3d_sub1 *a2 = (wind3d_sub1 *)b;

    return a2->tex->texture.dx1 - a1->tex->texture.dx1;
}

void win3d_func214__sub1(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( bigdata->subt1_count )
        {
            qsort(bigdata->subt1, bigdata->subt1_count, sizeof(wind3d_sub1), sub_43BB6C);

            for (int i = 0; i < bigdata->subt1_count; i++)
                sb_0x43b518(wdd, w3d, bigdata->subt1[i].polyData, bigdata->subt1[i].tex, 1, 0);

            bigdata->subt1_count = 0;

        }
    }
}

int sub_43BB80(const void * a, const void * b)
{
    wind3d_sub1 *a1 = (wind3d_sub1 *)a;
    wind3d_sub1 *a2 = (wind3d_sub1 *)b;

    if (a1->tex->texture.dx1 == a2->tex->texture.dx1)
    {
        if (a1->field_8 == a2->field_8)
            return 0;
        else if (a1->field_8 < a2->field_8)
            return 1;
        else if (a1->field_8 > a2->field_8)
            return -1;
    }
    else if (a1->tex->texture.dx1 < a2->tex->texture.dx1)
        return -1;
    else if (a1->tex->texture.dx1 > a2->tex->texture.dx1)
        return 1;

    return 0;
}

void win3d_func214__sub2(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    if ( bigdata->sceneBeginned )
    {
        if ( bigdata->dat_1C14_count )
        {
            for (int i = 0; i < bigdata->dat_1C14_count; i++)
            {
                float tmp = 0.0;
                wind3d_sub1* subt = &bigdata->dat_1C14[i];

                for (int j = 0; j < subt->polyData->vertexCount; j++)
                {
                    xyz *vtx = &subt->polyData->vertexes[j];
                    if (tmp < vtx->sz)
                        tmp = vtx->sz;
                }

                subt->field_8 = tmp;
            }

            qsort(bigdata->dat_1C14, bigdata->dat_1C14_count, sizeof(wind3d_sub1), sub_43BB80);

            for (int i = 0; i < bigdata->dat_1C14_count; i++)
                sb_0x43b518(wdd, w3d, bigdata->dat_1C14[i].polyData, bigdata->dat_1C14[i].tex, 1, 1);

            bigdata->dat_1C14_count = 0;
        }
    }
}

void win3d_func214__sub0(__NC_STACK_windd *, __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    for (int i = 0; i < bigdata->texCh_count; i++)
        if (bigdata->texCh[i].used & 2)
            bigdata->texCh[i].used |= 1;
}

// Draw transparent
size_t NC_STACK_win3d::raster_func214(void *)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func214__sub1(wdd, w3d);
    win3d_func214__sub2(wdd, w3d);
    win3d__endScene(wdd, w3d);
    win3d_func214__sub0(wdd, w3d);
    return 1;
}


int win3d_func215__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *)
{
    if ( !wdd->hwnd )
    {
        wdd->surface_locked_pitch = 0;
        wdd->surface_locked_surfaceData = NULL;
        return 0;
    }

    if ( wdd->surface_locked_surfaceData )
        return 1;

    sub_42A640(wdd);

    DDSURFACEDESC surfDesc;
    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    HRESULT res = wdd->back_surf->Lock(NULL, &surfDesc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0);

    if ( res )
    {
        log_d3d_fail("w3d_LockBackBuffer()", "Lock() failed.", res);
        wdd->surface_locked_pitch = 0;
        wdd->surface_locked_surfaceData = NULL;
        return 0;
    }

    wdd->surface_locked_surfaceData = surfDesc.lpSurface;
    wdd->surface_locked_pitch = surfDesc.lPitch;
    return 1;
}

void NC_STACK_win3d::raster_func215(void *)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func215__sub0(wdd, w3d);
}


void win3d_func216__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *)
{
    if ( wdd->hwnd )
    {
        if ( wdd->surface_locked_surfaceData )
        {
            wdd->back_surf->Unlock(NULL);
            wdd->surface_locked_surfaceData = NULL;
            wdd->surface_locked_pitch = 0;
        }
    }
}

void NC_STACK_win3d::raster_func216(void *)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func216__sub0(wdd, w3d);
}




void win3d_func218__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, void *buf1, int width, BYTE *buf2, int elmnt, ua_dRect rect, ua_dRect rect2)
{
    if ( wdd->surface_locked_surfaceData )
    {
        if (w3d->bigdata->primary__pixelformat.BytesPerColor == 2)
        {
            WORD *locked = (WORD *)wdd->surface_locked_surfaceData;

            int wdth = wdd->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor;


            if ( rect2.x2 != rect2.x1 && rect2.y2 != rect2.y1 )
            {
                int v15 = rect.y1 << 16;

                WORD *lkdLine = &locked[wdth * rect2.y1];

                for (int i = rect2.y1; i < rect2.y2; i++)
                {
                    WORD *bf = (WORD *)buf1;
                    WORD *bf1line = &bf[width * (v15 >> 16)];
                    BYTE *bf2line = &buf2[width * (v15 >> 16)];

                    int v13 = rect.x1 << 16;

                    WORD *lkdPos = &lkdLine[rect2.x1];
                    for (int j = rect2.x1; j < rect2.x2; j++)
                    {
                        if (bf2line[v13 >> 16] == elmnt)
                            *lkdPos = bf1line[v13 >> 16];
                        lkdPos++;
                        v13 += ((rect.x2 - rect.x1) << 16) / (rect2.x2 - rect2.x1);
                    }


                    lkdLine += wdth;
                    v15 += ((rect.y2 - rect.y1) << 16) / (rect2.y2 - rect2.y1);
                }
            }
        }
        else if (w3d->bigdata->primary__pixelformat.BytesPerColor == 4)
        {
            DWORD *locked = (DWORD *)wdd->surface_locked_surfaceData;

            int wdth = wdd->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor;


            if ( rect2.x2 != rect2.x1 && rect2.y2 != rect2.y1 )
            {
                int v15 = rect.y1 << 16;

                DWORD *lkdLine = &locked[wdth * rect2.y1];

                for (int i = rect2.y1; i < rect2.y2; i++)
                {
                    DWORD *bf = (DWORD *)buf1;
                    DWORD *bf1line = &bf[width * (v15 >> 16)];
                    BYTE *bf2line = &buf2[width * (v15 >> 16)];

                    int v13 = rect.x1 << 16;

                    DWORD *lkdPos = &lkdLine[rect2.x1];
                    for (int j = rect2.x1; j < rect2.x2; j++)
                    {
                        if (bf2line[v13 >> 16] == elmnt)
                            *lkdPos = bf1line[v13 >> 16];
                        lkdPos++;
                        v13 += ((rect.x2 - rect.x1) << 16) / (rect2.x2 - rect2.x1);
                    }


                    lkdLine += wdth;
                    v15 += ((rect.y2 - rect.y1) << 16) / (rect2.y2 - rect2.y1);
                }
            }
        }
    }
}


void NC_STACK_win3d::raster_func218(rstr_218_arg *arg)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_raster *rstr = &stack__raster;

    ua_dRect rect1, rect2;

    rect1.x1 = (arg->rect1.x1 + 1.0) * (arg->bitm_intern->width / 2);
    rect1.y1 = (arg->rect1.y1 + 1.0) * (arg->bitm_intern->height / 2);
    rect1.x2 = (arg->rect1.x2 + 1.0) * (arg->bitm_intern->width / 2);
    rect1.y2 = (arg->rect1.y2 + 1.0) * (arg->bitm_intern->height / 2);

    rect2.x1 = (arg->rect2.x1 + 1.0) * rstr->field_554;
    rect2.y1 = (arg->rect2.y1 + 1.0) * rstr->field_558;
    rect2.x2 = (arg->rect2.x2 + 1.0) * rstr->field_554;
    rect2.y2 = (arg->rect2.y2 + 1.0) * rstr->field_558;

    win3d_func218__sub0(wdd, w3d, (WORD *)arg->bitm_intern->buffer, arg->bitm_intern->width, (BYTE *)arg->bitm_intern2->buffer, arg->flg, rect1, rect2);
}



void win3d_func262__sub0(__NC_STACK_win3d *w3d, int a2, int *a3, int *a4)
{
    float cl1 = 0.0;
    float cl2 = 0.0;
    float cl3 = 0.0;

    if ( a2 )
    {
        float tclr1 = 0.0;
        float tclr2 = 0.0;
        float tclr3 = 0.0;

        win3d_bigdata *bigdata = w3d->bigdata;

        for(int i = 0; i < a2; i++)
        {
            float v15 = a4[i] / 255.0;
            int v14 = a3[i];

            tclr1 += bigdata->gray_colors__[v14][0] * v15;
            tclr2 += bigdata->gray_colors__[v14][1] * v15;
            tclr3 += bigdata->gray_colors__[v14][2] * v15;
        }

        cl1 = tclr1;
        cl2 = tclr2;
        cl3 = tclr3;
    }
    if ( cl1 > 1.0 )
        cl1 = 1.0;
    if ( cl2 > 1.0 )
        cl2 = 1.0;
    if ( cl3 > 1.0 )
        cl3 = 1.0;

    w3d->bigdata->gray_colors__[8][0] = cl1;
    w3d->bigdata->gray_colors__[8][1] = cl2;
    w3d->bigdata->gray_colors__[8][2] = cl3;
}

void NC_STACK_win3d::display_func262(rstr_262_arg *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func262__sub0(w3d, arg->dword0, arg->pdword4, arg->pdword8);
    NC_STACK_windd::display_func262(arg);
}



int win3d__allocTexBuffer(__NC_STACK_windd *, __NC_STACK_win3d *w3d, int w, int h, bitmap_intern *arg)
{
    arg->buffer = NULL;

    void *buf = AllocVec(w * h * w3d->bigdata->primary__pixelformat.BytesPerColor, 0);
    if (!buf)
        return 0;

    arg->buffer = buf;
    return w * w3d->bigdata->primary__pixelformat.BytesPerColor;
}

signed int win3d__createSurfTexPal(__NC_STACK_windd *wdd, __NC_STACK_win3d *, int w, int h, texStru **_tex)
{
    *_tex = NULL;

    texStru *tex = (texStru *)AllocVec(sizeof(texStru), 0);

    if ( !tex )
    {
        log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "Out Of Mem", 0);
        return 0;
    }
    *_tex = tex;

    windd_formats *format = &wdd->intern->formats[wdd->intern->selected_format_id];
    memset(tex, 0, 12);

    DDSURFACEDESC surfDesc;
    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));

    surfDesc.dwWidth = w;
    surfDesc.dwHeight = h;
    surfDesc.dwSize = sizeof(DDSURFACEDESC);
    surfDesc.dwFlags = DDSD_PIXELFORMAT | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_TEXTURE | DDSCAPS_SYSTEMMEMORY;

    memcpy(&surfDesc.ddpfPixelFormat, &format->surf_descr.ddpfPixelFormat, sizeof(DDPIXELFORMAT));


    HRESULT res = ddraw->CreateSurface(&surfDesc, &tex->surface, NULL);
    if ( res )
    {
        log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "IDirect3DTexture::CreateSurface(SrcTxt)", res);
        return 0;
    }

    if ( (surfDesc.ddpfPixelFormat.dwFlags & 0x20) )
    {
        PALETTEENTRY pal[256];
        memset(pal, 0, 256 * sizeof(PALETTEENTRY));

        res = ddraw->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, pal, &tex->palette, NULL);
        if ( res )
        {
            log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "IDirectDraw::CreatePalette(SrcTxt)", res);
            return 0;
        }

        res = tex->surface->SetPalette(tex->palette);
        if ( res )
        {
            log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "IDirectDrawSurface::SetPalette(SrcTxt)", res);
            return 0;
        }
    }

    res = tex->surface->QueryInterface(IID_IDirect3DTexture2, (void **)&tex->texture.dx2);
    if ( res )
    {
        log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "IDirectDrawSurface::QueryInterface(SrcTxt)", res);
        return 0;
    }

    return 1;
}

size_t NC_STACK_win3d::display_func266(bitmap_intern **arg)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    bitmap_intern *bitm_intern = *arg;

    if ( bitm_intern->flags & 8 )
    {
        bitm_intern->pitch = win3d__allocTexBuffer(wdd, w3d, bitm_intern->width, bitm_intern->height, bitm_intern);
        return bitm_intern->pitch != 0;
    }
    else
    {
        return win3d__createSurfTexPal(wdd, w3d, bitm_intern->width, bitm_intern->height, &bitm_intern->ddrawSurfTex);
    }
}

void win3d__tex_apply_palette_hw(__NC_STACK_windd *, __NC_STACK_win3d *w3d, UA_PALETTE *pal, __NC_STACK_display *dspl, texStru *tex, int a5)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    DWORD dwRBitMask = bigdata->selected__pixelformat.dwRBitMask;
    DWORD dwRShift = bigdata->selected__pixelformat.dwRShift;

    DWORD dwGBitMask = bigdata->selected__pixelformat.dwGBitMask;
    DWORD dwGShift = bigdata->selected__pixelformat.dwGShift;

    DWORD dwBBitMask = bigdata->selected__pixelformat.dwBBitMask;
    DWORD dwBShift = bigdata->selected__pixelformat.dwBShift;

    DWORD dwAlphaBitMask = bigdata->selected__pixelformat.dwAlphaBitMask;
    DWORD dwAShift = bigdata->selected__pixelformat.dwAShift;

    int BytesPerColor = bigdata->selected__pixelformat.BytesPerColor;

    if ( BytesPerColor == 1 )
    {

        PALETTEENTRY tmpPal[256];
        if ( pal )
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i].peRed = pal->pal_entries[i].r;
                tmpPal[i].peGreen = pal->pal_entries[i].g;
                tmpPal[i].peBlue = pal->pal_entries[i].b;
                tmpPal[i].peFlags = 0;
            }
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i].peRed = dspl->palette.pal_entries[i].r;
                tmpPal[i].peGreen = dspl->palette.pal_entries[i].g;
                tmpPal[i].peBlue = dspl->palette.pal_entries[i].b;
                tmpPal[i].peFlags = 0;
            }
        }
        tex->palette->SetEntries(0, 0, 256, tmpPal);
    }
    else if (BytesPerColor == 2)
    {
        DWORD tmpPal[256];

        for (int i = 0; i < 256; i++)
        {
            int r,g,b,a;
            if ( pal )
            {
                r = pal->pal_entries[i].r;
                g = pal->pal_entries[i].g;
                b = pal->pal_entries[i].b;
            }
            else
            {
                r = dspl->palette.pal_entries[i].r;
                g = dspl->palette.pal_entries[i].g;
                b = dspl->palette.pal_entries[i].b;
            }

            if (r == 255 && g == 255 && b == 0)
            {
                a = 0;
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                if (!dd_params.selected_device.can_destblend && dd_params.selected_device.can_srcblend && a5)
                {
                    int mx = (r >= g) ? (r > b ? r: b) : (g > b ? g : b);

                    if (mx <= 8)
                    {
                        r = 0;
                        g = 0;
                        b = 0;
                        a = 0;
                    }
                    else
                    {
                        float prm = mx;
                        r = 255.0 * (r / prm);
                        g = 255.0 * (g / prm);
                        b = 255.0 * (b / prm);
                        a = mx;
                    }
                }
                else
                {
                    a = 255;
                }
            }

            tmpPal[i] = RGBAToColor(r, g, b, a,
                                    dwRShift, dwGShift, dwBShift, dwAShift,
                                    dwRBitMask, dwGBitMask, dwBBitMask, dwAlphaBitMask);
        }

        DDSURFACEDESC surfDesc;

        memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
        surfDesc.dwSize = sizeof(DDSURFACEDESC);

        if ( tex->surface->Lock(NULL, &surfDesc, 0, 0) == D3D_OK )
        {

            BYTE *indexes = (BYTE *)surfDesc.lpSurface + (surfDesc.dwHeight * surfDesc.dwWidth) - 1;
            WORD *words = (WORD *)surfDesc.lpSurface + (surfDesc.dwHeight * surfDesc.dwWidth) - 1;

            for (int i = surfDesc.dwHeight * surfDesc.dwWidth; i > 0; i--)
            {
                *words = tmpPal[*indexes];
                words--;
                indexes--;
            }

            tex->surface->Unlock(surfDesc.lpSurface);
        }
    }
    else if (BytesPerColor == 4)
    {
        DWORD tmpPal[256];

        for (int i = 0; i < 256; i++)
        {
            int r,g,b,a;
            if ( pal )
            {
                r = pal->pal_entries[i].r;
                g = pal->pal_entries[i].g;
                b = pal->pal_entries[i].b;
            }
            else
            {
                r = dspl->palette.pal_entries[i].r;
                g = dspl->palette.pal_entries[i].g;
                b = dspl->palette.pal_entries[i].b;
            }

            if (r == 255 && g == 255 && b == 0)
            {
                a = 0;
                r = 0;
                g = 0;
                b = 0;
            }
            else
            {
                if (!dd_params.selected_device.can_destblend && dd_params.selected_device.can_srcblend && a5)
                {
                    int mx = (r >= g) ? (r > b ? r: b) : (g > b ? g : b);

                    if (mx <= 8)
                    {
                        r = 0;
                        g = 0;
                        b = 0;
                        a = 0;
                    }
                    else
                    {
                        float prm = mx;
                        r = 255.0 * (r / prm);
                        g = 255.0 * (g / prm);
                        b = 255.0 * (b / prm);
                        a = mx;
                    }
                }
                else
                {
                    a = 255;
                }
            }

            tmpPal[i] = RGBAToColor(r, g, b, a,
                                    dwRShift, dwGShift, dwBShift, dwAShift,
                                    dwRBitMask, dwGBitMask, dwBBitMask, dwAlphaBitMask);
        }

        DDSURFACEDESC surfDesc;

        memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
        surfDesc.dwSize = sizeof(DDSURFACEDESC);

        if ( tex->surface->Lock(NULL, &surfDesc, 0, 0) == D3D_OK )
        {

            BYTE *indexes = (BYTE *)surfDesc.lpSurface + (surfDesc.dwHeight * surfDesc.dwWidth) - 1;
            DWORD *dwords = ((DWORD *)surfDesc.lpSurface) + (surfDesc.dwHeight * surfDesc.dwWidth) - 1;

            for (int i = surfDesc.dwHeight * surfDesc.dwWidth; i > 0; i--)
            {
                *dwords = tmpPal[*indexes];
                dwords--;
                indexes--;
            }

            tex->surface->Unlock(surfDesc.lpSurface);
        }
    }
}

void win3d__tex_apply_palette(__NC_STACK_windd *, __NC_STACK_win3d *w3d, UA_PALETTE *pal, __NC_STACK_display *dspl, int w, int h, void *buf)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    int dwRBitMask = w3d->bigdata->primary__pixelformat.dwRBitMask;
    int dwRShift = bigdata->primary__pixelformat.dwRShift;

    int dwGBitMask = bigdata->primary__pixelformat.dwGBitMask;
    int dwGShift = bigdata->primary__pixelformat.dwGShift;

    int dwBBitMask = bigdata->primary__pixelformat.dwBBitMask;
    int dwBShift = bigdata->primary__pixelformat.dwBShift;

    int BytesPerColor = bigdata->primary__pixelformat.BytesPerColor;

    DWORD tmpPal[256];

    if ( BytesPerColor == 2 )
    {
        if ( pal )
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i] = RGBAToColor(pal->pal_entries[i].r, pal->pal_entries[i].g, pal->pal_entries[i].b, 0,
                                        dwRShift, dwGShift, dwBShift, 0,
                                        dwRBitMask, dwGBitMask, dwBBitMask, 0);
            }
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i] = RGBAToColor(dspl->palette.pal_entries[i].r, dspl->palette.pal_entries[i].g, dspl->palette.pal_entries[i].b, 0,
                                        dwRShift, dwGShift, dwBShift, 0,
                                        dwRBitMask, dwGBitMask, dwBBitMask, 0);
            }
        }

        BYTE *indexes = (BYTE *)buf + (h * w) - 1;
        WORD *words = (WORD *)buf + (h * w) - 1;

        for (int i = h * w; i > 0; i--)
        {
            *words = tmpPal[*indexes];
            words--;
            indexes--;
        }
    }
    else if ( BytesPerColor == 4 )
    {
        if ( pal )
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i] = RGBAToColor(pal->pal_entries[i].r, pal->pal_entries[i].g, pal->pal_entries[i].b, 0,
                                        dwRShift, dwGShift, dwBShift, 0,
                                        dwRBitMask, dwGBitMask, dwBBitMask, 0);
            }
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i] = RGBAToColor(dspl->palette.pal_entries[i].r, dspl->palette.pal_entries[i].g, dspl->palette.pal_entries[i].b, 0,
                                        dwRShift, dwGShift, dwBShift, 0,
                                        dwRBitMask, dwGBitMask, dwBBitMask, 0);
            }
        }

        BYTE *indexes = (BYTE *)buf + (h * w) - 1;
        DWORD *dwords = (DWORD *)buf + (h * w) - 1;

        for (int i = h * w; i > 0; i--)
        {
            *dwords = tmpPal[*indexes];
            dwords--;
            indexes--;
        }
    }
    else
    {
        log_d3d_fail("win3d.class/w3d_txtcacje.c/MangleTxtBlt()", "Unsupported txt pixformat.", 0);
    }
}

void NC_STACK_win3d::display_func267(bitmap_intern **arg)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *dspl = &stack__display;

    bitmap_intern *bitm = *arg;

    if (bitm->flags & BITMAP_FLAG_SYSMEM)
        win3d__tex_apply_palette(wdd, w3d, bitm->pallete, dspl, bitm->width, bitm->height, bitm->buffer);
    else
        win3d__tex_apply_palette_hw(wdd, w3d, bitm->pallete, dspl, bitm->ddrawSurfTex, bitm->flags & 0x10);
}


void win3d_func268__sub0(void *bf)
{
    if ( bf )
        nc_FreeMem(bf);
}

void win3d_func268__sub1(texStru *tex)
{
    if ( tex->palette )
    {
        tex->palette->Release();
        tex->palette = NULL;
    }

    if ( tex->texture.dx2 )
    {
        tex->texture.dx2->Release();
        tex->texture.dx2 = NULL;
    }

    if ( tex->surface )
    {
        tex->surface->Release();
        tex->surface = NULL;
    }

    nc_FreeMem(tex);
}


void NC_STACK_win3d::display_func268(bitmap_intern **arg)
{
    bitmap_intern *bitm = *arg;

    if ( bitm->flags & BITMAP_FLAG_SYSMEM )
        win3d_func268__sub0(bitm->buffer);
    else
        win3d_func268__sub1(bitm->ddrawSurfTex);
}



int win3d__IDirectDrawSurface__Lock(bitmap_intern *bitm, texStru *tex)
{
    DDSURFACEDESC surfDesc;

    bitm->buffer = 0;

    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    HRESULT res = tex->surface->Lock(NULL, &surfDesc, 0, 0);
    if ( res )
    {
        log_d3d_fail("win3d.class/w3d_txtcache.c/LockTexture()", "IDirectDrawSurface::Lock()", res);
        return 0;
    }
    else
    {
        bitm->buffer = surfDesc.lpSurface;
        return 1;
    }
}

size_t NC_STACK_win3d::display_func269(bitmap_intern **arg)
{
    bitmap_intern *bitm = *arg;
    if ( bitm->flags & BITMAP_FLAG_SYSMEM )
        return 1;
    else
        return win3d__IDirectDrawSurface__Lock(bitm, bitm->ddrawSurfTex);
}



int win3d__IDirectDrawSurface__Unlock(bitmap_intern *bitm, texStru *tex)
{
    HRESULT res = tex->surface->Unlock(bitm->buffer);
    if ( res )
        log_d3d_fail("win3d.class/w3d_txtcache.c/UnlockTexture()", "IDirectDrawSurface::Unlock()", res);
    return res;
}

void NC_STACK_win3d::display_func270(bitmap_intern **arg)
{
    bitmap_intern *bitm = *arg;

    if ( !(bitm->flags & BITMAP_FLAG_SYSMEM) )
    {
        win3d__IDirectDrawSurface__Unlock(bitm, bitm->ddrawSurfTex);
        bitm->buffer = 0;
    }
}

void NC_STACK_win3d::display_func272(stack_vals *)
{
    return ;
}


void win3d_func271__sub0(__NC_STACK_windd *, __NC_STACK_win3d *w3d)
{
    for ( int i = 0; i < w3d->bigdata->texCh_count; i++ )
    {
        texCache *tex = &w3d->bigdata->texCh[i];
        tex->field_4 = 0;
        tex->loadedFromTexture.dx2 = NULL;
        tex->txStru = NULL;
        tex->used = 1;
    }
}

void NC_STACK_win3d::display_func271(stack_vals *stak)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func271__sub0(wdd, w3d);
    display_func272(stak);
}


void win3d_func274__sub0(__NC_STACK_windd *wdd, __NC_STACK_win3d *w3d, FILE *fil)
{
    if ( wdd->surface_locked_surfaceData )
    {
        int BytesPerColor = w3d->bigdata->primary__pixelformat.BytesPerColor;
        int ril_width = wdd->surface_locked_pitch / BytesPerColor;

        int width = wdd->width;
        int height = wdd->height;

        char buf[128];

        sprintf(buf, "P6\n#YPA screenshot\n%d %d\n255\n", width, height);
        fwrite(buf, strlen(buf), 1, fil);

        wind3d_pixelformat *fmt = &w3d->bigdata->primary__pixelformat;
        if ( BytesPerColor == 2 )
        {
            int pos = 0;
            for (int j = 0; j < height; j++)
            {
                WORD *line = (WORD *)wdd->surface_locked_surfaceData + pos;

                for (int i = 0; i < width; i++)
                {
                    WORD px = line[i];

                    DWORD r = px & fmt->dwRBitMask;
                    if (fmt->dwRShift <= 0)
                        r <<= -fmt->dwRShift;
                    else
                        r >>= fmt->dwRShift;

                    DWORD g = px & fmt->dwGBitMask;
                    if (fmt->dwGShift <= 0)
                        g <<= -fmt->dwGShift;
                    else
                        g >>= fmt->dwGShift;

                    DWORD b = px & fmt->dwBBitMask;
                    if (fmt->dwBShift <= 0)
                        b <<= -fmt->dwBShift;
                    else
                        b >>= fmt->dwBShift;

                    fputc(r, fil);
                    fputc(g, fil);
                    fputc(b, fil);
                }

                pos += ril_width;
            }
        }
        else if ( BytesPerColor == 4 )
        {
            int pos = 0;
            for (int j = 0; j < height; j++)
            {
                DWORD *line = (DWORD *)wdd->surface_locked_surfaceData + pos;

                for (int i = 0; i < width; i++)
                {
                    DWORD px = line[i];

                    DWORD r = px & fmt->dwRBitMask;
                    if (fmt->dwRShift <= 0)
                        r <<= -fmt->dwRShift;
                    else
                        r >>= fmt->dwRShift;

                    DWORD g = px & fmt->dwGBitMask;
                    if (fmt->dwGShift <= 0)
                        g <<= -fmt->dwGShift;
                    else
                        g >>= fmt->dwGShift;

                    DWORD b = px & fmt->dwBBitMask;
                    if (fmt->dwBShift <= 0)
                        b <<= -fmt->dwBShift;
                    else
                        b >>= fmt->dwBShift;

                    fputc(r, fil);
                    fputc(g, fil);
                    fputc(b, fil);
                }

                pos += ril_width;
            }
        }
    }
}

void NC_STACK_win3d::display_func274(const char **name)
{
    __NC_STACK_windd *wdd = &stack__windd;
    __NC_STACK_win3d *w3d = &stack__win3d;

    char filename[128];

    strcpy(filename, *name);
    strcat(filename, ".ppm");

    FILE *fil = FOpen(filename, "wb");
    if ( fil )
    {
        raster_func215(NULL);
        win3d_func274__sub0(wdd, w3d, fil);
        raster_func216(NULL);
        FClose(fil);
    }
}



void NC_STACK_win3d::setW3D_texFilt(int arg)
{
    stack__win3d.filter = arg;
}


size_t NC_STACK_win3d::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return func2( (stack_vals *)data );
    case 198:
        raster_func198( (w3d_func198arg *)data );
        return 1;
    case 199:
        raster_func199( (w3d_func199arg *)data );
        return 1;
    case 200:
        raster_func200( (w3d_func198arg *)data );
        return 1;
    case 201:
        raster_func201( (w3d_func199arg *)data );
        return 1;
    case 202:
        raster_func202( (rstr_arg204 *)data );
        return 1;
    case 204:
        raster_func204( (rstr_arg204 *)data );
        return 1;
    case 206:
        raster_func206( (polysDatSub *)data );
        return 1;
    case 209:
        raster_func209( (w3d_a209 *)data );
        return 1;
    case 213:
        raster_func213( (polysDatSub *)data );
        return 1;
    case 214:
        raster_func214( (void *)data );
        return 1;
    case 215:
        raster_func215( (void *)data );
        return 1;
    case 216:
        raster_func216( (void *)data );
        return 1;
    case 218:
        raster_func218( (rstr_218_arg *)data );
        return 1;
    case 262:
        display_func262( (rstr_262_arg *)data );
        return 1;
    case 266:
        return (size_t)display_func266( (bitmap_intern **)data );
    case 267:
        display_func267( (bitmap_intern **)data );
        return 1;
    case 268:
        display_func268( (bitmap_intern **)data );
        return 1;
    case 269:
        return (size_t)display_func269( (bitmap_intern **)data );
    case 270:
        display_func270( (bitmap_intern **)data );
        return 1;
    case 271:
        display_func271( (stack_vals *)data );
        return 1;
    case 272:
        display_func272( (stack_vals *)data );
        return 1;
    case 274:
        display_func274( (const char **)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_windd::compatcall(method_id, data);
}
