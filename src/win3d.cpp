#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "display.h"
#include "win3d.h"
#include "utils.h"
#include "MS/d3dmacs.h"

#include "font.h"

windd_params dd_params;

IDirectDraw *ddraw;
IDirect3D2 *d3d2;


int txt16bit = 0;

GUID *driver_guid;
char *driver_descript;
char *driver_name;

wdd_font *font;

int dword_514EFC = 0;


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

key_value_stru windd_keys[7] =
{
    {"gfx.force_soft_cursor", KEY_TYPE_BOOL, 0},
    {"gfx.all_modes", KEY_TYPE_BOOL, 0},
    {"gfx.movie_player", KEY_TYPE_BOOL, 1},
    {"gfx.force_alpha_textures", KEY_TYPE_BOOL, 0},
    {"gfx.use_draw_primitive", KEY_TYPE_BOOL, 0},
    {"gfx.disable_lowres", KEY_TYPE_BOOL, 0},
    {"gfx.export_window_mode", KEY_TYPE_BOOL, 0}
};

void guid_to_string(char *out, GUID *guid, const char *name)
{
    if ( guid )
    {
        sprintf(out, "0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",
                guid->Data1,
                guid->Data2,
                guid->Data3,
                guid->Data4[0],
                guid->Data4[1],
                guid->Data4[2],
                guid->Data4[3],
                guid->Data4[4],
                guid->Data4[5],
                guid->Data4[6],
                guid->Data4[7]);

        return;
    }

    strcpy(out, name);
}

int out_guid_to_file(const char *filename, GUID *guid, const char *name)
{
    char buf[128];

    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        guid_to_string(buf, guid, name);
        fil->puts(buf);
        delete fil;
        return 1;
    }
    return 0;
}

int windd_func0__sub1__sub0(const char *filename, GUID *out)
{
    char buf[256];

    memset(out, 0, sizeof(GUID));

    FSMgr::FileHandle *fil = uaOpenFile(filename, "r");

    if ( fil )
    {
        fil->gets(buf, 256);
        delete fil;

        char *eol = strpbrk(buf, "\n\r;");
        if ( eol )
            *eol = 0;

        if ( !strcmp(buf, "<primary>") )
        {
            return 2;
        }
        else
        {
            if ( !strcmp(buf, "<software>") )
                return 3;

            char *token = strtok(buf, ", \t");
            if ( token )
                out->Data1 = strtoul(token, 0, 0);

            token = strtok(0, ", \t");
            if ( token )
                out->Data2 = strtoul(token, 0, 0);

            token = strtok(0, ", \t");
            if ( token )
                out->Data3 = strtoul(token, 0, 0);

            for (int i = 0; i < 8; i++)
            {
                token = strtok(0, ", \t");
                if ( token )
                    out->Data4[i] = strtoul(token, 0, 0);
            }
            return 1;
        }
    }
    return 0;
}

void sub_41F610(const char *name, const char *guid, int cur)
{
    wddDevice *dev = (wddDevice *)AllocVec(sizeof(wddDevice), 65537);
    if ( dev )
    {
        strcpy(dev->name, name);
        strcpy(dev->guid, guid);
        dev->curr = cur != 0;
        AddTail(&graph_drivers_list, dev);

        const char *v8;

        if ( cur )
            v8 = "is_current";
        else
            v8 = "not_current";
        log_d3dlog("wdd_AddDevice(%s,%s,%s)\n", name, guid, v8);
    }
}


void sub_41F490(unsigned int width, unsigned int height, int bits, int a4)
{
    int rel_width, rel_height;

    if ( a4 & 8 )
    {
        rel_height = height / 2;
        rel_width = width  / 2;
    }
    else
    {
        rel_height = height;
        rel_width = width;
    }

    int adding = a4 & 1;

    if (adding == 0)
    {
        if (!modes_list.head->next)
            adding = 1;
        else
        {
            mode_node *node = (mode_node *)modes_list.head;
            while ( node->next )
            {
                if ( rel_width == node->rel_width && rel_height == node->rel_height && bits == node->bits)
                    return;

                node = (mode_node *)node->next;
            }
            adding = 1;
        }
    }

    if ( adding )
    {
        mode_node *node = (mode_node *)AllocVec(sizeof(mode_node), 65537);

        if ( !node )
            return;

        node->sort_id = height | (width << 12);

        if (bits == 32) //HACK
            node->sort_id |= 0x20000000;

        if ( a4 & 1 )
            node->sort_id |= 0x8000;

        if ( a4 & 8 )
            node->sort_id |= 0x4000;

        node->height = height;
        node->width = width;

        node->name[0] = 0;
        node->rel_width = rel_width;
        node->rel_height = rel_height;
        node->field_94 = a4;
        node->bits = bits;

        int v12 = a4 & 0xEF;

        if (v12 == 1 || v12 == 5)
            sprintf(node->name, "WIN: %d x %d", node->width, node->height);
        else if (v12 == 8)
            sprintf(node->name, "%d x %d x %d", node->rel_width, node->rel_height, bits);
        else
            sprintf(node->name, "%d x %d x %d", node->width, node->height, bits);

        if ( a4 & 1 )
        {
            AddTail(&modes_list, node);
        }
        else
        {
            mode_node *v13 = (mode_node *)modes_list.tailpred;
            if ( modes_list.tailpred->prev )
            {
                do
                {
                    if ( !(v13->field_94 & 1) && rel_width >= v13->rel_width && rel_height >= v13->rel_height )
                        break;
                    v13 = (mode_node *)v13->prev;
                }
                while ( v13->prev );
            }
            v13->next->prev = node;
            node->prev = v13;
            node->next = v13->next;
            v13->next = node;
        }
    }
}



void out_yes_no_status(const char *filename, int val)
{
    FSMgr::FileHandle *fil = uaOpenFile(filename, "w");
    if ( fil )
    {
        if ( val )
            fil->puts("yes");
        else
            fil->puts("no");
        delete fil;
    }
}







int dbcs_StartText()
{
    if ( !font )
        return 0;

    font->ddsurf->Unlock(NULL);
    if ( font->ddsurf->GetDC(&font->hDC) )
        return 0;

    SelectObject(font->hDC, font->hFont);
    SetTextColor(font->hDC, 0xFFFFu);
    font->TextColor = 0xFFFF;
    SetBkMode(font->hDC, 1);

    return 1;
}

void dbcs_DrawText(const char *string, int p1, int p2, int p3, int p4, char flag)
{
    int newColor;

    if ( font )
    {
        if ( flag & 0x20 )
        {
            newColor = (p3 << 16) | p1 | (p2 << 8);
            if ( newColor != font->TextColor )
            {
                SetTextColor(font->hDC, newColor);
                font->TextColor = newColor;
            }
        }
        else
        {
            tagSIZE psizl;
            psizl.cx = 0;
            psizl.cy = 0;

            if ( flag & 0xE )
            {
                GetTextExtentPoint32A(font->hDC, string, strlen(string), &psizl);
            }

            if ( flag & 8 )
                p3 = psizl.cx * p3 / 100;

            RECT rect;

            rect.left = p1;
            rect.right = p1 + p3 + 4;
            rect.bottom = p2 + p4 + 1;
            rect.top = p2;

            if ( flag & 2 )
            {
                if ( flag & 8 )
                {
                    p1 -= psizl.cx;
                    rect.left = p1;
                    rect.right = p1 + p3 + 4;
                }
                else
                {
                    p1 += (p3 - psizl.cx);
                }
            }
            else if ( flag & 4 )
            {
                if ( flag & 8 )
                {
                    p1 -= psizl.cx / 2;
                    rect.left = p1;
                    rect.right = p1 + p3 + 4;
                }
                else
                {
                    p1 += (p3 - psizl.cx) / 2;
                }
            }

            int v10 = ((p4 - font->height) / 2) - 1 + p2;
            if ( flag & 0x10 )
            {
                v10++;
                p1++;
            }

            SetTextColor(font->hDC, 0);
            ExtTextOut(font->hDC, p1 + 2, v10 + 1, 4, &rect, string, strlen(string), 0);
            SetTextColor(font->hDC, font->TextColor);
            ExtTextOut(font->hDC, p1 + 1, v10, 4, &rect, string, strlen(string), 0);
        }
    }
}

int dbcs_EndText(LPDDSURFACEDESC surfDesc)
{
    if ( !font )
    {
        log_d3dlog("dbcs_EndText(): no DBCS pointer (back ptr invalid!)\n");
        return 0;
    }
    if ( !font->hDC )
    {
        log_d3dlog("dbcs_EndText(): no device context (back ptr invalid!)\n");
        return 0;
    }

    font->ddsurf->ReleaseDC(font->hDC);
    font->hDC = NULL;

    memset(surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc->dwSize = sizeof(DDSURFACEDESC);

    HRESULT err = font->ddsurf->Lock( NULL, surfDesc, DDLOCK_NOSYSLOCK | DDLOCK_WAIT, 0);
    if ( err )
        log_d3d_fail("dbcs_EndText()", "Lock on backsurface failed.", err);

    return 1;
}

void dbcs_AddText(const char *string, int p1, int p2, int p3, int p4, int flag)
{
    int cnt = font->strings_count;
    if ( cnt < 64 )
    {
        wdd_font_st1 *v8 = &font->field_18[cnt];
        font->strings_count = cnt + 1;

        v8->string = string;
        v8->p1 = p1;
        v8->p2 = p2;
        v8->p3 = p3;
        v8->p4 = p4;
        v8->flag = flag;
    }
}

int sb_0x4bf0a0(LPDDSURFACEDESC surf)
{
    if ( font->strings_count )
    {
        dbcs_StartText();
        for ( int i = 0; i < font->strings_count; i++ )
            dbcs_DrawText(font->field_18[i].string, font->field_18[i].p1, font->field_18[i].p2, font->field_18[i].p3, font->field_18[i].p4, font->field_18[i].flag);

        font->strings_count = 0;
        return dbcs_EndText(surf);
    }
    return 0;
}




HRESULT __stdcall enumdevice_callback(LPGUID lpGuid, LPSTR lpDeviceDescription, LPSTR lpDeviceName, LPD3DDEVICEDESC hw, LPD3DDEVICEDESC, LPVOID )
{
    _devices *device = &dd_params.enum_devices_[dd_params.number_of_devices];

    memset(&dd_params.enum_devices_[dd_params.number_of_devices], 0, sizeof(_devices));

    log_d3dlog("-> enum devices:\n");
    log_d3dlog("    name = %s\n", lpDeviceName);
    log_d3dlog("    desc = %s\n", lpDeviceDescription);

    if ( lpGuid )
    {
        memcpy(&device->device_guid, lpGuid, sizeof(GUID));
        device->has_device_guid = 1;

        log_d3dlog(
            "    guid = 0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",
            lpGuid->Data1,
            lpGuid->Data2,
            lpGuid->Data3,
            lpGuid->Data4[0],
            lpGuid->Data4[1],
            lpGuid->Data4[2],
            lpGuid->Data4[3],
            lpGuid->Data4[4],
            lpGuid->Data4[5],
            lpGuid->Data4[6],
            lpGuid->Data4[7]);
    }
    if ( driver_guid )
    {
        memcpy(&device->driver_guid, driver_guid, sizeof(GUID));
        device->has_driver_guid = 1;
    }

    strncpy(device->device_name, lpDeviceName, 255);
    strncpy(device->device_descr, lpDeviceDescription, 255);
    strncpy(device->driver_descript, driver_descript, 255);
    strncpy(device->driver_name, driver_name, 255);

    if ( !hw->dcmColorModel )
    {
        log_d3dlog("enum devices: skip, is not hardware\n");

        memset(device, 0, sizeof(_devices));

        return 1;
    }

    device->unk0_def_1 = 1;

    memcpy(&device->dev_descr, hw, hw->dwSize);

    log_d3dlog("enum devices: ok, is hardware\n");

    device->unk1_def_0 = 0;
    device->unk2_def_0 = 0;

    unsigned int dwRGB;


    switch(dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount)
    {
    case 1:
        dwRGB = DDBD_1;
        break;
    case 2:
        dwRGB = DDBD_2;
        break;
    case 4:
        dwRGB = DDBD_4;
        break;
    case 8:
        dwRGB = DDBD_8;
        break;
    case 16:
        dwRGB = DDBD_16;
        break;
    case 24:
        dwRGB = DDBD_24;
        break;
    case 32:
        dwRGB = DDBD_32;
        break;
    default:
        dwRGB = 0x0;
        break;
    }

    if ( dwRGB & device->dev_descr.dwDeviceRenderBitDepth )
    {
        device->unk1_def_0 = 1;
        log_d3dlog("enum devices: can render into desktop bit depth\n");
    }

    if ( device->dev_descr.dpcTriCaps.dwRasterCaps & D3DPRASTERCAPS_ZBUFFERLESSHSR )
    {
        device->zbuf_use = 0;
        log_d3dlog("enum devices: zbufferlesshsr set (no zbuf used)\n");
    }
    else
    {
        if ( !device->dev_descr.dwDeviceZBufferBitDepth )
        {
            log_d3dlog("enum devices: skip, no hidden surface removal...\n");
            return 1;
        }

        device->zbuf_use = 1;
        log_d3dlog("enum devices: use zbuffer\n");
    }

    if ( device->zbuf_use )
    {
        if ( device->dev_descr.dwDeviceZBufferBitDepth & DDBD_16 )
        {
            device->zbuf_bit_depth = 16;
        }
        else if ( device->dev_descr.dwDeviceZBufferBitDepth & DDBD_8 )
        {
            device->zbuf_bit_depth = 8;
        }
        else if ( device->dev_descr.dwDeviceZBufferBitDepth & DDBD_24 )
        {
            device->zbuf_bit_depth = 24;
        }
        else if ( device->dev_descr.dwDeviceZBufferBitDepth & DDBD_32 )
        {
            device->zbuf_bit_depth = 32;
        }

        log_d3dlog("enum devices: zbuf bit depth is %d\n", device->zbuf_bit_depth);
    }

    device->can_destblend = 0;
    device->can_stippling = 0;
    device->can_srcblend = 0;

    if ( (device->dev_descr.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_ONE) &&
            (device->dev_descr.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_ONE) &&
            !txt16bit )
    {
        device->can_srcblend = 1;
        device->can_destblend = 1;

        log_d3dlog("enum devices: can do srcblend = srcalpha; destblend = one\n");
    }
    else if ( (device->dev_descr.dpcTriCaps.dwShadeCaps & (D3DPSHADECAPS_ALPHAFLATSTIPPLED | D3DPSHADECAPS_ALPHAFLATSTIPPLED)) &&
              !(device->dev_descr.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND) )
    {
        device->can_stippling = 1;

        log_d3dlog("enum devices: can do alpha stippling\n");
    }
    else if ( (device->dev_descr.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) && (device->dev_descr.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) )
    {
        device->can_srcblend = 1;
        device->can_destblend = 0;

        log_d3dlog("enum devices: can do srcblend = srcalpha; destblend = invsrcalpha\n");
    }
    else
    {
        log_d3dlog("enum devices: skip, no alpha, no stipple...\n");
        return 1;
    }

    can_srcblend = device->can_srcblend;
    can_destblend = device->can_destblend;
    can_stippling = device->can_stippling;

    if ( device->dev_descr.dwDevCaps & D3DDEVCAPS_TEXTURESYSTEMMEMORY )
    {
        device->textures_in_sysmem = 1;
        log_d3dlog("enum devices: can do sysmem textures\n");
    }
    else if ( device->dev_descr.dwDevCaps & D3DDEVCAPS_TEXTUREVIDEOMEMORY )
    {
        device->textures_in_sysmem = 0;
        log_d3dlog("enum devices: textures in vmem\n");
    }
    else
    {
        log_d3dlog("enum devices: skip, does not support texture mapping\n");
        return 1;
    }


    if ( device->dev_descr.dpcTriCaps.dwTextureCaps & 8 )
    {
        device->can_colorkey = 1;
        log_d3dlog("enum devices: can do colorkey\n");
    }
    else
    {
        device->can_colorkey = 0;
        log_d3dlog("enum devices: no colorkey support\n");
    }

    dd_params.number_of_devices++;

    log_d3dlog("enum devices: device accepted\n");

    return 1;
}

signed int __stdcall DDRAW_ENUMERATE_CallBack(GUID *lpGUID, LPSTR DRIVER_NAME, LPSTR DRIVER_DESCR, void *)
{
    char buf[144];

    ddraw = NULL;
    d3d2 = NULL;

    if ( DirectDrawCreate(lpGUID, &ddraw, 0) )
        return 1;

    dd_params.displ_mode_surface.dwSize = sizeof(DDSURFACEDESC);

    ddraw->GetDisplayMode(&dd_params.displ_mode_surface);

    guid_to_string(buf, lpGUID, "<primary>");

    log_d3dlog("-> enumerate ddraw objects...\n");
    log_d3dlog("    -> name = %s\n", DRIVER_DESCR);
    log_d3dlog("    -> desc = %s\n", DRIVER_NAME);
    log_d3dlog("    -> guid = %s\n", buf);

    DDCAPS driver_caps;
    memset(&driver_caps, 0, sizeof(DDCAPS));
    driver_caps.dwSize = sizeof(DDCAPS);

    DDCAPS emul_caps;
    memset(&emul_caps, 0, sizeof(DDCAPS));
    emul_caps.dwSize = sizeof(DDCAPS);

    if ( ddraw->GetCaps(&driver_caps, &emul_caps) )
    {
        ddraw->Release();
        ddraw = NULL;
        return 1;
    }
    else
    {
        if ( driver_caps.dwCaps & DDCAPS_3D )
        {
            if ( !ddraw->QueryInterface(IID_IDirect3D2, (void **)&d3d2) )
            {
                driver_descript = DRIVER_DESCR;
                driver_name = DRIVER_NAME;

                driver_guid = lpGUID;

                d3d2->EnumDevices(enumdevice_callback, 0);


                driver_descript = NULL;
                driver_name = NULL;
                driver_guid = NULL;

                d3d2->Release();
                d3d2 = NULL;
            }
        }

        ddraw->Release();
        ddraw = NULL;

        return 1;
    }
    return 1;
}



HRESULT __stdcall gfx_modes_callback(LPDDSURFACEDESC a1, LPVOID )
{
////	if ( a1->dwWidth <= 1024 && a1->dwHeight <= 768 ) ////HACK
    {
        dd_params.gfx_modes[dd_params.gfx_modes_count].width = a1->dwWidth;
        dd_params.gfx_modes[dd_params.gfx_modes_count].height = a1->dwHeight;
        dd_params.gfx_modes[dd_params.gfx_modes_count].bits = a1->ddpfPixelFormat.dwRGBBitCount;
        dd_params.gfx_modes[dd_params.gfx_modes_count].unk = 0;

        log_d3dlog(
            "enum display mode: %dx%dx%d\n",
            a1->dwWidth,
            a1->dwHeight,
            a1->ddpfPixelFormat.dwRGBBitCount);

        dd_params.gfx_modes_count++;

        if ( dd_params.gfx_modes_count == 64 )
            return 0;
    }
    return 1;
}



int cmp_gfx_modes (const void * a, const void * b)
{
    enum_gfx_modes *a1 = (enum_gfx_modes *)a;
    enum_gfx_modes *a2 = (enum_gfx_modes *)b;

    if ( a1->bits < a2->bits )
        return -1;

    if ( a2->bits < a1->bits )
        return 1;

    if ( a1->width < a2->width )
        return -1;

    if ( a2->width < a1->width )
        return 1;

    if (a1->height < a2->height)
        return -1;

    if (a2->height < a1->height)
        return 1;

    return 0;
}




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


int win3dInitialisation(__NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = (win3d_bigdata *)AllocVec(sizeof(win3d_bigdata), 0);
    w3d->bigdata = bigdata;
    if ( bigdata )
    {
        memset(bigdata, 0, sizeof(win3d_bigdata));

        bigdata->_dx = w3d->width / 2;
        bigdata->_dy = w3d->height / 2;

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

int initPixelFormats(__NC_STACK_win3d *w3d)
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


    windd_formats *dd_fmt = &w3d->intern->formats[w3d->intern->selected_format_id];

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

void sub_439E30(__NC_STACK_win3d *w3d)
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

int initTextureCache(__NC_STACK_win3d *w3d)
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
        windd_formats *wdd_fmt = &w3d->intern->formats[w3d->intern->selected_format_id];

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
            sub_439E30(w3d);
            return 0;
        }

        if ( surfDescr.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 )
        {
            res = ddraw->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, tmpPal, &tex->ddrawPal, NULL);

            if ( res )
            {
                log_d3d_fail("win3d.class/w3d_txtcache.c/InitTxtCache()", "IDirectDraw::CreatePalette()", res);
                sub_439E30(w3d);
                return 0;
            }

            res = tex->ddDrawSurface->SetPalette(tex->ddrawPal);
            if ( res )
            {
                log_d3d_fail("win3d.class/w3d_txtcache.c", "IDirectDrawSurface::SetPalette()", res);
                sub_439E30(w3d);
                return 0;
            }
        }

        res = tex->ddDrawSurface->QueryInterface(IID_IDirect3DTexture2, (void **)&tex->d3dtex2);
        if ( res )
        {
            log_d3d_fail("win3d.class", "QueryInterface(IID_IDirect3DTextur2) failed.", res);
            sub_439E30(w3d);
            return 0;
        }

        res = tex->ddDrawSurface->QueryInterface(IID_IDirect3DTexture, (void **)&tex->d3dtex);
        if ( res )
        {
            log_d3d_fail("win3d.class", "QueryInterface(IID_IDIrect3DTexture) failed.", res);
            sub_439E30(w3d);
            return 0;
        }

        w3d->bigdata->texCh_count++;
    }
    return 1;
}

void sub_43BE88(__NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( w3d->bigdata->sceneBeginned )
    {
        OP_EXIT(bigdata->rendStates.lpInsPointer);

        void *lpInsStart = bigdata->rendStates.lpInsStart;

        OP_PROCESS_VERTICES(1, lpInsStart);
        PROCESSVERTICES_DATA( D3DPROCESSVERTICES_COPY, 0, bigdata->rendStates.vertexCount, lpInsStart);

        execBuf *rendStates = &bigdata->rendStates;

        HRESULT res = w3d->intern->executebuffer->Unlock();
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

        res = w3d->intern->executebuffer->SetExecuteData(&execData);
        if ( res )
        {
            log_d3d_fail("win3d.class", "IDirect3DExecuteBuffer::SetExecuteData() failed", res);
        }
        else
        {
            res = w3d->intern->d3d1Dev->Execute(w3d->intern->executebuffer, w3d->intern->d3d2Viewport, D3DEXECUTE_UNCLIPPED);
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

void sub_43BD50(__NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        D3DEXECUTEBUFFERDESC bufDescr;
        memset(&bufDescr, 0, sizeof(D3DEXECUTEBUFFERDESC));

        execBuf *execBuff = &w3d->bigdata->rendStates;
        bufDescr.dwSize = sizeof(D3DEXECUTEBUFFERDESC);

        while ( true )
        {
            HRESULT res = w3d->intern->executebuffer->Lock(&bufDescr);
            if ( res == D3DERR_EXECUTE_LOCKED ) //0x887602CA
                w3d->intern->executebuffer->Unlock();
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

void execBuff_initRenderStates(__NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( !w3d->use_simple_d3d )
        {
            execBuf *execBuff = &w3d->bigdata->rendStates;

            if ( w3d->bigdata->sceneBeginned )
            {
                if ( (char *)execBuff->lpInsPointer + 1000 > execBuff->lpBufEnd
                        || (char *)execBuff->lpBufStart3 + 1000 > execBuff->lpInsStart )
                {
                    sub_43BE88(w3d);
                    sub_43BD50(w3d);
                }
            }
            execBuff->lpStates = execBuff->lpInsPointer;
            execBuff->rendStatesCount = 0;

            OP_STATE_RENDER(execBuff->rendStatesCount, execBuff->lpInsPointer);
        }
    }
}

void win3d__beginScene(__NC_STACK_win3d *w3d)
{
    HRESULT res;

    if ( w3d->use_simple_d3d )
    {
        res = w3d->intern->d3d2dev->BeginScene();
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
        res = w3d->intern->d3d1Dev->BeginScene();
        if ( res )
        {
            w3d->bigdata->sceneBeginned = 0;
            log_d3d_fail("win3d.class", "D3DDevice::BeginScene() failed", res);
        }
        else
        {
            w3d->bigdata->sceneBeginned = 1;
        }
        sub_43BD50(w3d);
    }
}

void win3d__SetRenderState(__NC_STACK_win3d *w3d, D3DRENDERSTATETYPE type, DWORD state)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( w3d->use_simple_d3d )
        {
            HRESULT res = w3d->intern->d3d2dev->SetRenderState(type, state);

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

void execBuff_finishRenderStates( __NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( !w3d->use_simple_d3d )
        {
            execBuf *execBuff = &bigdata->rendStates;

            void *lpStates = bigdata->rendStates.lpStates;

            OP_STATE_RENDER(execBuff->rendStatesCount, lpStates);

            execBuff->lpStates = NULL;
            execBuff->rendStatesCount = 0;
        }
    }
}

void win3d__endScene(__NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( w3d->use_simple_d3d )
        {
            HRESULT res = w3d->intern->d3d2dev->EndScene();
            if ( res )
                log_d3d_fail("win3d.class", "D3DDevice2::EndScene() failed", res);
        }
        else
        {
            sub_43BE88(w3d);

            HRESULT res = w3d->intern->d3d1Dev->EndScene();
            if ( res )
                log_d3d_fail("win3d.class", "D3DDevice::EndScene() failed", res);
        }
    }
}

int initPolyEngine(__NC_STACK_win3d *w3d)
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

    win3d__beginScene(w3d);

    execBuff_initRenderStates(w3d);

    win3d__SetRenderState(w3d, D3DRENDERSTATE_TEXTUREADDRESS, D3DTADDRESS_CLAMP);
    win3d__SetRenderState(w3d, D3DRENDERSTATE_ZENABLE, 1); /* TRUE to enable z test */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_TEXTUREMAG, (w3d->filter != 0) + D3DFILTER_NEAREST); // Or D3DFILTER_LINEAR
    win3d__SetRenderState(w3d, D3DRENDERSTATE_TEXTUREMAPBLEND, D3DTBLEND_MODULATE);
    win3d__SetRenderState(w3d, D3DRENDERSTATE_CULLMODE, D3DCULL_NONE);
    win3d__SetRenderState(w3d, D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
    win3d__SetRenderState(w3d, D3DRENDERSTATE_DITHERENABLE, w3d->dither);
    win3d__SetRenderState(w3d, D3DRENDERSTATE_ALPHABLENDENABLE, 0); /* TRUE to enable alpha blending */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_FOGENABLE, 0); /* TRUE to enable fog */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_SUBPIXEL, 1); /* TRUE to enable subpixel correction */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEDALPHA, dd_params.selected_device.can_stippling); /* TRUE to enable stippled alpha */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_STIPPLEENABLE, 0); /* TRUE to enable stippling */
    win3d__SetRenderState(w3d, D3DRENDERSTATE_COLORKEYENABLE, 1); /* TRUE to enable source colorkeyed textures */

    execBuff_finishRenderStates(w3d);

    win3d__endScene(w3d);

    w3d->bigdata->subt1_count = 0;
    memset(w3d->bigdata->subt1, 0, sizeof(wind3d_sub1) * 512);

    w3d->bigdata->dat_1C14_count = 0;
    memset(w3d->bigdata->dat_1C14, 0, 4); //?

    return 1;
}



int windd_func0__sub1(int export_window_mode)
{
    DDCAPS emul_caps;
    DDCAPS driver_caps;


    ddraw = NULL;
    d3d2 = NULL;

    memset(&dd_params, 0, sizeof(dd_params));

    log_d3dlog("dd/d3d init: entered wdd_DDrawCreate()\n");

    HRESULT v1 = DirectDrawEnumerate(DDRAW_ENUMERATE_CallBack, 0);
    if ( v1 )
    {
        log_d3d_fail("DirectDraw", "DirectDrawEnumerate()", v1);
        log_d3dlog("common init failed: DirectDrawEnumerate()\n");
        return 0;
    }

    if (dword_514EFC)
    {
        GUID guid;
        char buf[128];

        int guid_ty = windd_func0__sub1__sub0("env/guid3d.def", &guid);

        switch ( guid_ty )
        {
        case 0:
            log_d3dlog("d3d init: guid3d.def invalid\n");
            break;
        case 1:
            guid_to_string(buf, &guid, "<error>");
            log_d3dlog("d3d init: guid3d.def is %s\n", buf);
            break;
        case 2:
            log_d3dlog("d3d init: guid3d.def is <primary>\n");
            break;
        case 3:
            log_d3dlog("d3d init: guid3d.def is <software>\n");
            break;
        default:
            break;
        }

        if ( dd_params.number_of_devices )
        {
            int guidFound = 0;

            if ( guid_ty == 3 )
            {
                log_d3dlog("d3d init: ddraw mode wanted, exit\n");
                if ( d3d2 )
                {
                    d3d2->Release();
                    d3d2 = NULL;
                }
                if ( ddraw )
                {
                    ddraw->Release();
                    ddraw = NULL;
                }

                return 0;
            }

            if ( guid_ty == 2 || guid_ty == 1 )
            {
                for (int i = 0; i < dd_params.number_of_devices; i++)
                {
                    _devices *devs = &dd_params.enum_devices_[i];

                    if ( guid_ty == 2 )
                    {
                        if ( !devs->has_driver_guid )
                        {
                            log_d3dlog("d3d init: found match for guid3d.def\n");

                            dd_params.device_selected_id = i;

                            guidFound = 1;

                            break;
                        }
                    }
                    else if ( guid_ty == 3 && devs->has_driver_guid && !memcmp(&guid, &devs->driver_guid, sizeof(GUID)) )
                    {
                        log_d3dlog("d3d init: found match for guid3d.def\n");

                        dd_params.device_selected_id = i;

                        guidFound = 1;

                        break;
                    }
                }
            }

            if ( !guidFound )
            {
                log_d3dlog("d3d init: no guid3d.def match found, using autodetect\n");
                dd_params.device_selected_id = dd_params.number_of_devices - 1;
            }

            memcpy(&dd_params.selected_device, &dd_params.enum_devices_[dd_params.device_selected_id], sizeof(_devices));

            GUID *v7;

            if ( dd_params.selected_device.has_driver_guid )
                v7 = &dd_params.selected_device.driver_guid;
            else
                v7 = 0;

            log_d3dlog("picked: %s, %s\n", dd_params.selected_device.device_name, dd_params.selected_device.device_descr);

            out_guid_to_file("env/guid3d.def", v7, "<primary>");

            if ( !DirectDrawCreate(v7, &ddraw, 0) )
                ddraw->QueryInterface(IID_IDirect3D2, (void**)&d3d2);
        }

        if ( !d3d2 )
        {
            log_d3dlog("d3d init failed: no suitable d3d device found.\n");

            if ( d3d2 )
            {
                d3d2->Release();
                d3d2 = NULL;
            }
            if ( ddraw )
            {
                ddraw->Release();
                ddraw = NULL;
            }
            return 0;
        }

        if ( dd_params.selected_device.has_driver_guid )
        {
            dd_params.selected_device.unk1_def_0 = 0;
            dd_params.selected_device.unk2_def_0 = 1;
            dd_params.field_0 &= (~1);
            log_d3dlog("d3d init: non-primary device picked, assuming fullscreen card\n");
        }
    }


    if ( !ddraw )
    {
        v1 = DirectDrawCreate(NULL, &ddraw, NULL);
        if ( v1 )
        {
            log_d3d_fail("DirectDraw", "DirectDrawCreate()", v1);
            log_d3dlog("common init failed: DirectDrawCreate()\n");
            return 0;
        }
    }

    memset(&driver_caps, 0, sizeof(DDCAPS));
    driver_caps.dwSize = sizeof(DDCAPS);

    memset(&emul_caps, 0, sizeof(DDCAPS));
    emul_caps.dwSize = sizeof(DDCAPS);

    if ( ddraw->GetCaps(&driver_caps, &emul_caps) )
        dd_params.video_mem = 0;
    else
        dd_params.video_mem = driver_caps.dwVidMemTotal;

    log_d3dlog("common init: vmem total is %d\n", dd_params.video_mem);

    v1 = ddraw->EnumDisplayModes(0, NULL, NULL, gfx_modes_callback);
    if ( v1 )
    {
        log_d3d_fail("DirectDraw", "DirectDraw::EnumDisplayModes()", v1);
        log_d3dlog("common init failed: EnumDisplayModes()\n");
        return 0;
    }

    sub_41F610("software", "<software>", dword_514EFC == 0);

    for (int v12 = 0; v12 < dd_params.number_of_devices; v12++)
    {
        _devices *devs = &dd_params.enum_devices_[v12];

        char guid[256];
        if ( devs->has_driver_guid )
            guid_to_string(guid, &devs->driver_guid, "<error>");
        else
            strcpy(guid, "<primary>");

        sub_41F610(devs->driver_name, guid, v12 == dd_params.device_selected_id && dword_514EFC);
    }

    qsort(dd_params.gfx_modes, dd_params.gfx_modes_count, sizeof(enum_gfx_modes), cmp_gfx_modes);

    for (unsigned int i = 0; i < dd_params.gfx_modes_count; i++)
    {
        enum_gfx_modes *devs = &dd_params.gfx_modes[i];

        if ( !dword_514EFC )
        {
            if ( devs->bits == 8 )
            {
                if ( devs->width >= 640 )
                    sub_41F490(devs->width, devs->height, devs->bits, devs->bits);

                sub_41F490(devs->width, devs->height, devs->bits, 0);

                log_d3dlog("dd init: export display mode %dx%dx%d\n", devs->width, devs->height, devs->bits);
            }
        }
        else
        {
            unsigned int dwRGB;

            switch(devs->bits)
            {
            case 1:
                dwRGB = DDBD_1;
                break;
            case 2:
                dwRGB = DDBD_2;
                break;
            case 4:
                dwRGB = DDBD_4;
                break;
            case 8:
                dwRGB = DDBD_8;
                break;
            case 16:
                dwRGB = DDBD_16;
                break;
            case 24:
                dwRGB = DDBD_24;
                break;
            case 32:
                dwRGB = DDBD_32;
                break;
            default:
                dwRGB = 0x0;
                break;
            }

            if ( dd_params.selected_device.dev_descr.dwDeviceRenderBitDepth & dwRGB )
            {
                if ( devs->bits == 16 || devs->bits == 32)
                {
                    devs->unk = 1;

                    sub_41F490(devs->width, devs->height, devs->bits, 16);

                    log_d3dlog("d3d init: export display mode %dx%dx%d\n", devs->width, devs->height, devs->bits);
                }
            }
        }
    }

    if ( export_window_mode )
    {
        if ( dword_514EFC )
        {
            if ( dd_params.selected_device.unk1_def_0 )
            {
                sub_41F490(320, 200, dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount, 17);
                log_d3dlog("dd init: export windowed mode %dx%dx%d\n", 320, 200, dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount);
            }
        }
        else if ( dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount == 8 )
        {
            sub_41F490(320, 200, dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount, 5);
            log_d3dlog("dd init: export windowed mode %dx%dx%d\n", 320, 200, dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount);
        }
    }

    log_d3dlog("dd/d3d init: wdd_DDrawCreate() left\n");
    return 1;
}



mode_node * sub_41F68C()
{
    mode_node *result = (mode_node *)modes_list.head;
    if ( modes_list.head->next )
    {
        while ( result->width != 640 || result->height != 480 )
        {
            result = (mode_node *)result->next;
            if ( !result->next )
                return (mode_node *)modes_list.head;
        }
    }
    else
    {
        result = (mode_node *)modes_list.head;
    }
    return result;
}

mode_node * windd_func0__sub0(const char *file)
{
    char buf[128];

    FSMgr::FileHandle *fil = uaOpenFile(file, "r");

    mode_node *node = NULL;

    if ( fil )
    {
        if ( fil->gets(buf, 128) )
        {
            char *eol = strpbrk(buf, "\n\r");
            if ( eol )
                *eol = 0;
            node = (mode_node *)modes_list.head;
            if ( modes_list.head->next )
            {
                while ( strcasecmp(buf, node->name) )
                {
                    node = (mode_node *)node->next;
                    if ( !node->next )
                    {
                        node = NULL;
                        break;
                    }
                }
            }
            else
            {
                node = NULL;
            }
        }
        delete fil;
    }

    if ( !node )
        node = sub_41F68C();

    return node;
}

int sub_42AC78(__NC_STACK_win3d *a1)
{
    return dd_params.selected_device.unk2_def_0 || a1->field_30;
}

void sub_42D410(__NC_STACK_win3d *obj, int a2, int a3)
{
    if ( obj->hwnd )
    {
        if ( !sub_42AC78(obj) )
        {
            int v5 = 0;

            if ( a3 )
            {
                obj->field_28 = a2;
                while ( ShowCursor(1) < 0 )
                    ;
                v5 = 1;
            }
            else
            {
                if ( a2 != obj->field_28 )
                {
                    if ( obj->field_28 )
                    {
                        if ( !a2 )
                        {
                            while ( ShowCursor(0) >= 0 )
                                ;
                        }
                    }
                    if ( !obj->field_28 )
                    {
                        if ( a2 )
                        {
                            while ( ShowCursor(1) < 0 )
                                ;
                        }
                    }
                    v5 = 1;
                    obj->field_28 = a2;
                }
            }


            if ( v5 )
            {
                HCURSOR cur = 0;
                switch ( a2 )
                {
                case 1:
                    cur = uaLoadCursor(obj->cursor, "Pointer");
                    break;
                case 2:
                    cur = uaLoadCursor(obj->cursor, "Cancel");
                    break;
                case 3:
                    cur = uaLoadCursor(obj->cursor, "Select");
                    break;
                case 4:
                    cur = uaLoadCursor(obj->cursor, "Attack");
                    break;
                case 5:
                    cur = uaLoadCursor(obj->cursor, "Goto");
                    break;
                case 6:
                    cur = uaLoadCursor(obj->cursor, "Disk");
                    break;
                case 7:
                    cur = uaLoadCursor(obj->cursor, "New");
                    break;
                case 8:
                    cur = uaLoadCursor(obj->cursor, "Add");
                    break;
                case 9:
                    cur = uaLoadCursor(obj->cursor, "Control");
                    break;
                case 10:
                    cur = uaLoadCursor(obj->cursor, "Beam");
                    break;
                case 11:
                    cur = uaLoadCursor(obj->cursor, "Build");
                    break;
                default:
                    break;
                }

                if ( cur )
                    SetCursor(cur);
            }
        }
    }
}


void sub_42A7BC(__NC_STACK_win3d *obj)
{
    if ( obj->hwnd )
    {
        if ( obj->field_10 )
        {
            HDC dc = GetDC(obj->hwnd);
            if ( dc )
            {
                HDC hdcSrc;
                if ( !obj->field_10->GetDC(&hdcSrc) )
                {
                    HPALETTE hpal = 0;

                    if ( !dword_514EFC )
                    {
                        LOGPALETTE256 pal;
                        memset(&pal, 0, sizeof(LOGPALETTE256));

                        PALETTEENTRY *entr = pal.pal.palPalEntry;
                        pal.pal.palVersion = 0x300;
                        pal.pal.palNumEntries = 256;

                        for (int i = 0 ; i < 10; i++)
                        {
                            entr[246 + i].peRed = 246 + i;
                            entr[246 + i].peFlags = 2;
                            entr[i].peRed = i;
                            entr[i].peFlags = 2;
                        }

                        if ( obj->ddrawPal )
                            obj->ddrawPal->GetEntries(0, 10, 236, entr + 10);

                        hpal = CreatePalette(&pal.pal);

                        if ( hpal )
                        {
                            SelectPalette(dc, hpal, 0);
                            RealizePalette(dc);
                        }
                    }

                    RECT Rect;

                    GetClientRect(obj->hwnd, &Rect);

                    int h,w;

                    if ( obj->field_50 & 8 )
                    {
                        h = obj->height / 2;
                        w = obj->width / 2;
                    }
                    else
                    {
                        h = obj->height;
                        w = obj->width;
                    }

                    if ( w != Rect.right || h != Rect.bottom )
                        StretchBlt(dc, 0, 0, Rect.right, Rect.bottom, hdcSrc, 0, 0, w, h, SRCCOPY);
                    else
                        BitBlt(dc, 0, 0, w, h, hdcSrc, 0, 0, SRCCOPY);

                    if ( hpal )
                        DeleteObject(hpal);

                    obj->field_10->ReleaseDC(hdcSrc);
                }
                ReleaseDC(obj->hwnd, dc);
            }
        }
    }
}

void sub_42A640(__NC_STACK_win3d *obj)
{
    if ( obj->primary_surf )
        if ( obj->primary_surf->IsLost() )
            obj->primary_surf->Restore();

    if ( obj->back_surf )
        if ( obj->back_surf->IsLost() )
        {
            obj->back_surf->Restore();
            obj->surface_locked_surfaceData = 0;
            obj->surface_locked_pitch = 0;
        }

    if ( obj->field_10 )
        if ( obj->field_10->IsLost() )
            obj->field_10->Restore();

    if ( obj->intern )
        if ( obj->intern->z_buf )
            if ( obj->intern->z_buf->IsLost() )
                obj->intern->z_buf->Restore();
}

void wdd_Kill3D(__NC_STACK_win3d *obj)
{
    log_d3dlog("-> Entering wdd_Kill3D()\n");

    if ( obj->intern )
    {
        if ( obj->intern->material )
        {
            obj->intern->material->Release();
            obj->intern->material = NULL;
        }

        if ( obj->intern->executebuffer )
        {
            obj->intern->executebuffer->Release();
            obj->intern->executebuffer = NULL;
        }

        if ( obj->intern->d3d2Viewport )
        {
            obj->intern->d3d2Viewport->Release();
            obj->intern->d3d2Viewport = NULL;
        }

        if ( obj->intern->d3d1Dev )
        {
            obj->intern->d3d1Dev->Release();
            obj->intern->d3d1Dev = NULL;
        }

        if ( obj->intern->d3d2dev )
        {
            obj->intern->d3d2dev->Release();
            obj->intern->d3d2dev = NULL;
        }

        if ( obj->intern->z_buf )
        {
            obj->intern->z_buf->Release();
            obj->intern->z_buf = NULL;
        }

        nc_FreeMem(obj->intern);
        obj->intern = NULL;
    }
    log_d3dlog("-> Leaving wdd_Kill3D()\n");
}

void sub_42BD38(__NC_STACK_win3d *obj)
{
    if ( obj->intern )
        wdd_Kill3D(obj);

    if ( obj->ddrawPal )
    {
        obj->ddrawPal->Release();
        obj->ddrawPal = NULL;
    }

    if ( obj->clipper )
    {
        obj->clipper->Release();
        obj->clipper = NULL;
    }

    if ( obj->back_surf )
    {
        obj->back_surf->Release();
        obj->back_surf = NULL;
    }

    if ( obj->primary_surf )
    {
        obj->primary_surf->Release();
        obj->primary_surf = NULL;
    }

    if ( obj->field_10 )
    {
        obj->field_10->Release();
        obj->field_10 = NULL;
    }
}


LRESULT WINAPI sub_42A978(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    __NC_STACK_win3d *obj = (__NC_STACK_win3d *)GetClassLongPtr(hWnd, 0);

    switch(Msg)
    {

    case WM_DESTROY:
    {
        if ( obj )
        {
            sub_42BD38(obj);
            obj->hwnd = 0;
            PostQuitMessage(0);
        }
        if ( dword_514F1C )
        {
            ImmAssociateContext(hWnd, dword_514F1C);
            dword_514F1C = 0;
        }
        return 0;
    }
    break;

    case WM_ACTIVATE:
    {
        if ( obj )
            sub_42D410(obj, 1, 1);
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ppnt;

        BeginPaint(hWnd, &ppnt);
        if ( obj && obj->hwnd && obj->primary_surf && obj->back_surf )
        {
            if ( dword_514F24 )
            {
                sub_42A7BC(obj);
            }
            else
            {
                sub_42A640(obj);
                if ( obj->field_50 & 1 )
                {
                    RECT rc;
                    POINT Point;
                    GetClientRect(obj->hwnd, &rc);
                    Point.x = 0;
                    Point.y = 0;
                    ClientToScreen(obj->hwnd, &Point);
                    OffsetRect(&rc, Point.x, Point.y);
                    obj->primary_surf->Blt(&rc, obj->back_surf, NULL, DDBLT_WAIT, NULL);
                }
                else if ( !dword_514EFC )
                {
                    obj->primary_surf->Blt(NULL, obj->back_surf, NULL, DDBLT_WAIT, NULL);
                }
            }
        }
        EndPaint(hWnd, &ppnt);
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    break;

    case WM_ERASEBKGND:
        return 1; //Avoid flickering
        break;


    case WM_QUERYNEWPALETTE:
    case WM_PALETTECHANGED:
    {
        if (Msg == WM_PALETTECHANGED && (HWND)wParam == obj->hwnd)
            return DefWindowProc(hWnd, Msg, wParam, lParam);

        if ( obj )
            if ( obj->ddrawPal )
                obj->primary_surf->SetPalette(obj->ddrawPal);

        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    break;

    case WM_SYSCOMMAND:
    {
        if ( wParam == SC_KEYMENU )
            return 0;
        if ( wParam == SC_SCREENSAVE )
            return 0;
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    break;

    case WM_QUIT:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
        break;

    case WM_ACTIVATEAPP:
    {
        if ( wParam == 1 )
        {
            RECT Rect;
            GetWindowRect(hWnd, &Rect);
            ClipCursor(&Rect);
            if ( obj )
                sub_42A640(obj);
        }
        else
        {
            ClipCursor(0);
            if ( obj )
            {
                if ( obj->surface_locked_surfaceData )
                {
                    obj->surface_locked_pitch = 0;
                    obj->surface_locked_surfaceData = 0;
                    obj->back_surf->Unlock(0);
                }
            }
        }
        if ( obj )
            sub_42D410(obj, 1, 1);
        return DefWindowProc(hWnd, Msg, wParam, lParam);
    }
    break;


    default:
        return DefWindowProc(hWnd, Msg, wParam, lParam);
        break;
    }

    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

int createWindow(__NC_STACK_win3d *obj, HINSTANCE hInstance, int cmdShow, int width, int height)
{
    HICON big256 = uaLoadIcon(hInstance, "Big256");
    HCURSOR hCursor = uaLoadCursor(hInstance, "Pointer");

    if ( ghWnd )
    {
        obj->hwnd = ghWnd;

        int w, h;
        if ( obj->field_50 & 1 )
        {
            SetWindowLongPtr(ghWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
            h = height;
            w = width;
        }
        else
        {
            SetWindowLongPtr(ghWnd, GWL_STYLE, WS_POPUP | WS_SYSMENU);
            SetWindowLongPtr(ghWnd, GWL_EXSTYLE, WS_EX_TOPMOST);
            h = GetSystemMetrics(SM_CYSCREEN);
            w = GetSystemMetrics(SM_CXSCREEN);
        }
        SetWindowPos(ghWnd, 0, 0, 0, w, h, SWP_SHOWWINDOW);
        SetClassLongPtr(ghWnd, 0, (LONG_PTR)obj);
        return 1;
    }
    else
    {
        WNDCLASS winClass;

        winClass.hInstance = hInstance;
        winClass.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
        winClass.lpfnWndProc = sub_42A978;
        winClass.cbClsExtra = sizeof(void *);
        winClass.cbWndExtra = 0;
        winClass.hIcon = big256;
        winClass.hCursor = 0;
        winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        winClass.lpszMenuName = 0;
        winClass.lpszClassName = "UA Window Class";

        RegisterClass(&winClass);

        int w, h;
        DWORD wndstyle;
        DWORD dwExStyle;
        const char *lpclassname = "UA Window Class";
        const char *windname = "Urban Assault";

        if ( obj->field_50 & 1 ) // WINDOWED
        {
            h = height;
            w = width;
            wndstyle = WS_OVERLAPPEDWINDOW;
            dwExStyle = 0;

            obj->hwnd = CreateWindowEx(dwExStyle, lpclassname, windname, wndstyle, CW_USEDEFAULT, CW_USEDEFAULT, w, h, 0, 0, hInstance, 0);
        }
        else
        {
            h = GetSystemMetrics(SM_CYSCREEN);
            w = GetSystemMetrics(SM_CXSCREEN);
            wndstyle = WS_POPUP | WS_SYSMENU;
            dwExStyle = WS_EX_TOPMOST;

            obj->hwnd = CreateWindowEx(dwExStyle, lpclassname, windname, wndstyle, 0, 0, w, h, 0, 0, hInstance, 0);
        }

        if ( obj->hwnd )
        {
            dword_514F1C = ImmAssociateContext(obj->hwnd, 0);
            ShowWindow(obj->hwnd, cmdShow);
            UpdateWindow(obj->hwnd);
            SetCursor(hCursor);

            ghWnd = obj->hwnd;
            SetClassLongPtr(obj->hwnd, 0, (LONG_PTR)obj);
            return 1;
        }
    }
    return 0;
}

int wdd_Create3DWinEnv(__NC_STACK_win3d *obj, int width, int height)
{
    log_d3dlog("-> Entering wdd_Create3DWinEnv()\n");

    ddraw->SetCooperativeLevel(obj->hwnd, DDSCL_NORMAL);

    DDSURFACEDESC surfDesc;
    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwFlags = DDSD_CAPS;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    HRESULT res = ddraw->CreateSurface(&surfDesc, &obj->primary_surf, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Primary)", res);
        return 0;
    }

    surfDesc.dwWidth = width;
    surfDesc.dwHeight = height;
    surfDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE | DDSCAPS_OFFSCREENPLAIN;

    res = ddraw->CreateSurface(&surfDesc, &obj->back_surf, NULL);

    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Back)", res);
        return 0;
    }

    res = ddraw->CreateClipper(0, &obj->clipper, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateClipper()", res);
        return 0;
    }

    obj->clipper->SetHWnd(0, obj->hwnd);
    obj->primary_surf->SetClipper(obj->clipper);

    DDBLTFX fx;

    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;

    obj->primary_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    obj->back_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

    log_d3dlog("-> Leaving wdd_Create3DWinEnv()\n");
    return 1;
}

int wdd_Create3DFullEnv(__NC_STACK_win3d *obj, int width, int height, int bits)
{
    log_d3dlog("-> Entering wdd_Create3DFullEnv()\n");
    ddraw->SetCooperativeLevel(obj->hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);

    HRESULT res = ddraw->SetDisplayMode(width, height, bits);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::SetDisplayMode()", res);
        return 0;
    }

    DDSURFACEDESC surfDesc;

    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);
    surfDesc.dwFlags = DDSD_BACKBUFFERCOUNT | DDSD_CAPS;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY | DDSCAPS_3DDEVICE | DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    surfDesc.dwBackBufferCount = 1;

    res = ddraw->CreateSurface(&surfDesc, &obj->primary_surf, 0);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Primary)", res);
        return 0;
    }

    DDSCAPS caps;

    caps.dwCaps = DDSCAPS_BACKBUFFER;
    res = obj->primary_surf->GetAttachedSurface(&caps, &obj->back_surf);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::GetAttachedSurface(Back)", res);
        return 0;
    }

    res = ddraw->CreateClipper(0, &obj->clipper, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateClipper()", res);
        return 0;
    }

    obj->clipper->SetHWnd(0, obj->hwnd);
    obj->primary_surf->SetClipper(obj->clipper);

    DDBLTFX fx;

    fx.dwSize = sizeof(DDBLTFX);
    fx.dwFillColor = 0;

    obj->primary_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    obj->back_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

    log_d3dlog("-> Leaving wdd_Create3DFullEnv()\n");

    return 1;
}



HRESULT __stdcall EnumTextureFormats__CallBack(DDSURFACEDESC *descr, void *lpContext)
{
    __NC_STACK_win3d *win3d = (__NC_STACK_win3d *)lpContext;

    windd_formats *frmt = &win3d->intern->formats[win3d->intern->formats_count];

    memset(frmt, 0, sizeof(windd_formats));
    memcpy(&frmt->surf_descr, descr, sizeof(DDSURFACEDESC));

    if ( descr->ddpfPixelFormat.dwFlags & (DDPF_PALETTEINDEXED2 | DDPF_PALETTEINDEXED1 | DDPF_PALETTEINDEXEDTO8 | DDPF_PALETTEINDEXED4) )
        return 1;

    if ( descr->ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8 )
    {
        frmt->use_clut = 1;
        frmt->vosem = 8;
        log_d3dlog("enum texture formats: 8bpp clut\n");
    }
    else
    {
        frmt->vosem = 0;
        frmt->use_clut = 0;

        int rbits = 0;
        if ( descr->ddpfPixelFormat.dwRBitMask )
        {
            DWORD bits = descr->ddpfPixelFormat.dwRBitMask;

            while( !(bits & 1) )
                bits >>= 1;

            for(rbits = 0; bits & 1; rbits++)
                bits >>= 1;
        }

        int gbits = 0;
        if ( descr->ddpfPixelFormat.dwGBitMask )
        {
            DWORD bits = descr->ddpfPixelFormat.dwGBitMask;

            while( !(bits & 1) )
                bits >>= 1;

            for(gbits = 0; bits & 1; gbits++)
                bits >>= 1;
        }

        int bbits = 0;
        if ( descr->ddpfPixelFormat.dwBBitMask )
        {
            DWORD bits = descr->ddpfPixelFormat.dwBBitMask;

            while( !(bits & 1) )
                bits >>= 1;

            for(bbits = 0; bits & 1; bbits++)
                bits >>= 1;
        }

        int abits = 0;
        if ( descr->ddpfPixelFormat.dwRGBAlphaBitMask )
        {
            DWORD bits = descr->ddpfPixelFormat.dwRGBAlphaBitMask;

            while( !(bits & 1) )
                bits >>= 1;

            for(abits = 0; bits & 1; abits++)
                bits >>= 1;
        }

        frmt->rbits = rbits;
        frmt->gbits = gbits;
        frmt->bbits = bbits;
        frmt->abits = abits;
        frmt->rgbbitcount = descr->ddpfPixelFormat.dwRGBBitCount;
        log_d3dlog("enum texture formats: %d%d%d%d\n", rbits, gbits, bbits, abits);
    }

    win3d->intern->formats_count++;

    if ( win3d->intern->formats_count != 32 )
        return 1;

    return 0;
}

int wdd_Create3D(__NC_STACK_win3d *obj, int width, int height)
{
    log_d3dlog("-> Entering wdd_Create3D()\n");

    windd_intern *internal = (windd_intern *)AllocVec(sizeof(windd_intern), 0);
    obj->intern = internal;

    if ( !internal )
    {
        wdd_Kill3D(obj);
        return 0;
    }

    memset(internal, 0, sizeof(windd_intern));

    DDSURFACEDESC surfDesc;
    HRESULT res;

    if ( dd_params.selected_device.zbuf_use )
    {
        memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
        surfDesc.dwWidth = width;
        surfDesc.dwHeight = height;
        surfDesc.ddsCaps.dwCaps = DDSCAPS_ZBUFFER | DDSCAPS_VIDEOMEMORY;
        surfDesc.dwSize = sizeof(DDSURFACEDESC);
        surfDesc.dwFlags = DDSD_ZBUFFERBITDEPTH | DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;

        surfDesc.dwMipMapCount = dd_params.selected_device.zbuf_bit_depth;

        log_d3dlog("d3d init: create zbuf (w=%d,h=%d,bpp=%d)\n", width, height, dd_params.selected_device.zbuf_bit_depth);

        res = ddraw->CreateSurface(&surfDesc, &internal->z_buf, NULL);
        if ( res )
        {
            log_d3d_fail("DirectDraw", "Could not create z buffer.", res);
            log_d3dlog("d3d init: zbuf creation failed\n");
            wdd_Kill3D(obj);
            return 0;
        }

        res = obj->back_surf->AddAttachedSurface(internal->z_buf);
        if ( res )
        {
            log_d3d_fail("DirectDraw", "Could not attach z buffer.", res);
            log_d3dlog("d3d init: could not attach zbuf\n");
            wdd_Kill3D(obj);
            return 0;
        }
    }

    res = d3d2->CreateDevice(dd_params.selected_device.device_guid, obj->back_surf, &internal->d3d2dev);
    if ( res )
    {
        log_d3d_fail("windd.class", "IDirect3D2::CreateDevice() failed.", res);
        return 0;
    }

    res = internal->d3d2dev->QueryInterface(IID_IDirect3DDevice, (void **)&internal->d3d1Dev);
    if ( res )
    {
        log_d3d_fail("windd.class", "IDirect3DDevice2::QueryInterface(IID_IDirect3DDevice) failed", res);
        return 0;
    }

    internal->formats_count = 0;

    res = internal->d3d2dev->EnumTextureFormats(EnumTextureFormats__CallBack, obj);
    if ( res )
    {
        log_d3d_fail("DirectDraw", "EnumTextureFormats failed.", res);
        log_d3dlog("d3d init: EnumTextureFormats() failed\n");
        wdd_Kill3D(obj);
        return 0;
    }

    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    res = obj->primary_surf->GetSurfaceDesc(&surfDesc);
    if ( res )
    {
        log_d3d_fail("DirectDraw", "GetSurfaceDesc(Primary)", res);
        log_d3dlog("d3d init: GetSurfaceDesc() for primary failed\n");
        wdd_Kill3D(obj);
        return 0;
    }

    int selected = -1;
    int v34 = -1;
    int a2 = -1;
    int id = 0;

    internal->selected_format_id = -1;

    while ( 1 )
    {
        if ( id >= internal->formats_count )
            break;

        windd_formats *frmt = &internal->formats[id];

        if ( dd_params.selected_device.can_srcblend
                && dd_params.selected_device.can_destblend
                && dd_params.selected_device.can_colorkey
                && frmt->vosem == 8
                && selected == -1 )
        {
            selected = id;
        }
        else if ( dd_params.selected_device.can_stippling
                  && dd_params.selected_device.can_colorkey
                  && frmt->vosem == 8
                  && selected == -1 )
        {
            selected = id;
        }
        else if ( frmt->rbits != 4 || frmt->gbits != 4 || frmt->bbits != 4 || 4 != frmt->abits )
        {
            if ( frmt->rbits == 8 && frmt->gbits == 8 && frmt->bbits == 8 && 8 == frmt->abits )
                a2 = id;
        }
        else
        {
            v34 = id;
        }
        id++;
    }

    if ( selected == -1 )
    {
        if ( v34 != -1 )
        {
            internal->selected_format_id = v34;
            log_d3dlog("d3d init: use 4444 texture format\n");
        }
        else
        {
            selected = a2;
            if ( a2 == v34 )
            {
                log_d3d_fail("Direct3D", "No suitable texture format.", 0);
                log_d3dlog("d3d init: no suitable texture format found\n");
                wdd_Kill3D(obj);
                return 0;
            }
            internal->selected_format_id = selected;
            log_d3dlog("d3d init: use 8888 texture format\n");
        }
    }
    else
    {
        internal->selected_format_id = selected;
        log_d3dlog("d3d init: use 8bpp clut texture format\n");
    }

    res = d3d2->CreateViewport(&internal->d3d2Viewport, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "IDirect3D::CreateViewport()", res);
        wdd_Kill3D(obj);
        return 0;
    }

    res = internal->d3d2dev->AddViewport(internal->d3d2Viewport);
    if ( res )
    {
        log_d3d_fail("windd.class", "IDirect3DDevice::AddViewport()", res);
        wdd_Kill3D(obj);
        return 0;
    }

    D3DVIEWPORT data;
    memset(&data, 0, sizeof(D3DVIEWPORT));

    data.dwX = 0;
    data.dwY = 0;
    data.dwWidth = width;
    data.dwHeight = height;

    data.dvScaleX = width * 0.5;
    data.dvScaleY = height * 0.5;
    data.dwSize = sizeof(D3DVIEWPORT);
    data.dvMaxX = width / (2.0 * data.dvScaleX);
    data.dvMaxY = height / (2.0 * data.dvScaleY);

    res = internal->d3d2Viewport->SetViewport(&data);
    if ( res )
    {
        log_d3d_fail("windd.class", "IDirect3DViewport::SetViewport()", res);
        wdd_Kill3D(obj);
        return 0;
    }

    res = internal->d3d2dev->SetCurrentViewport(internal->d3d2Viewport);
    if ( res )
    {
        log_d3d_fail("windd.class", "ID3DDevice2::SetCurrentViewport() failed", res);
        wdd_Kill3D(obj);
        return 0;
    }

    res = d3d2->CreateMaterial(&internal->material, NULL);
    if ( res )
    {
        log_d3d_fail("Direct3D", "IDirect3D::CreateMaterial(Background)", res);
        wdd_Kill3D(obj);
        return 0;
    }

    D3DMATERIAL mat;

    memset(&mat, 0, sizeof(D3DMATERIAL));
    mat.dwSize = sizeof(D3DMATERIAL);
    mat.diffuse.r = 0;
    mat.diffuse.g = 0;
    mat.diffuse.b = 0;
    mat.diffuse.a = 0;
    mat.dwRampSize = 1;

    internal->material->SetMaterial(&mat);

    D3DMATERIALHANDLE matHndl;

    internal->material->GetHandle(internal->d3d2dev, &matHndl);
    internal->d3d2Viewport->SetBackground(matHndl);

    int bufsize = 0x10000;
    if ( dd_params.selected_device.dev_descr.dwMaxBufferSize )
    {
        if ( dd_params.selected_device.dev_descr.dwMaxBufferSize < 0x10000 )
            bufsize = dd_params.selected_device.dev_descr.dwMaxBufferSize;
    }

    D3DEXECUTEBUFFERDESC execbuf;

    memset(&execbuf, 0, sizeof(D3DEXECUTEBUFFERDESC));
    execbuf.dwBufferSize = bufsize;
    execbuf.dwSize = sizeof(D3DEXECUTEBUFFERDESC);
    execbuf.dwFlags = D3DDEB_BUFSIZE;

    log_d3dlog("d3d init: create execbuf with size %d\n", bufsize);

    res = internal->d3d1Dev->CreateExecuteBuffer(&execbuf, &internal->executebuffer, NULL);
    if ( res )
    {
        log_d3d_fail("wdd_Create3D", "CreateExecuteBuffer() failed.", res);
        log_d3dlog("d3d init: CreateExecuteBuffer() failed.\n");
        wdd_Kill3D(obj);
        return 0;
    }

    if ( obj->use_simple_d3d )
        log_d3dlog("***> using DrawPrimitive <***\n");
    else
        log_d3dlog("***> using ExecuteBuffer <***\n");

    log_d3dlog("-> Leaving wdd_Create3D()\n");

    return 1;
}

int wdd_Create2DWinEnv(__NC_STACK_win3d *obj, LPPALETTEENTRY pal, int width, int height)
{
    log_d3dlog("-> Entering wdd_Create2DWinEnv()\n");

    obj->field_50 |= 2;

    ddraw->SetCooperativeLevel(obj->hwnd, DDSCL_NORMAL);

    DDSURFACEDESC surfDesc;
    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));

    surfDesc.dwFlags = DDSD_CAPS;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    HRESULT res = ddraw->CreateSurface(&surfDesc, &obj->primary_surf, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Primary)", res);
        return 0;
    }

    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

    surfDesc.dwHeight = height;
    surfDesc.dwWidth = width;
    surfDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    surfDesc.dwSize = sizeof(DDSURFACEDESC);


    res = ddraw->CreateSurface(&surfDesc, &obj->back_surf, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Back)", res);
        return 0;
    }

    res = ddraw->CreateClipper(0, &obj->clipper, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateClipper()", res);
        return 0;
    }

    obj->clipper->SetHWnd(0, obj->hwnd);
    obj->primary_surf->SetClipper(obj->clipper);

    res = ddraw->CreatePalette(DDPCAPS_8BIT, pal, &obj->ddrawPal, NULL);
    if ( res )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreatePalette()", res);
        return 0;
    }

    obj->primary_surf->SetPalette(obj->ddrawPal);
    obj->back_surf->SetPalette(obj->ddrawPal);

    log_d3dlog("-> Leaving wdd_Create2DWinEnv()\n");

    return 1;
}

int wdd_Create2DFullEnv(__NC_STACK_win3d *obj, LPPALETTEENTRY pal, int width, int height)
{
    int w = width;
    int h = height;

    log_d3dlog("-> Entering wdd_Create2DFullEnv()\n");

    obj->field_50 |= 2;

    ddraw->SetCooperativeLevel(obj->hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    HRESULT v8 = ddraw->SetDisplayMode(w, h, 8);

    if ( v8 )
    {
        log_d3d_fail("windd.class", "DirectDraw::SetDisplayMode()", v8);
        return 0;
    }
    DDSURFACEDESC surfDesrc;

    memset(&surfDesrc, 0, sizeof(DDSURFACEDESC));

    surfDesrc.dwFlags = DDSD_CAPS;
    surfDesrc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    surfDesrc.dwSize = sizeof(DDSURFACEDESC);

    v8 = ddraw->CreateSurface(&surfDesrc, &obj->primary_surf, NULL);
    if ( v8 )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Primary)", v8);
        return 0;
    }

    surfDesrc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
    surfDesrc.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;

    if ( obj->field_50 & 8 )
    {
        w /= 2;
        h /= 2;
    }

    surfDesrc.dwWidth = w;
    surfDesrc.dwHeight = h;

    v8 = ddraw->CreateSurface(&surfDesrc, &obj->back_surf, NULL);
    if ( v8 )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateSurface(Back)", v8);
        return 0;
    }

    v8 = ddraw->CreateClipper(0, &obj->clipper, NULL);
    if ( v8 )
    {
        log_d3d_fail("windd.class", "DirectDraw::CreateClipper()", v8);
        return 0;
    }

    obj->clipper->SetHWnd(0, obj->hwnd);

    obj->primary_surf->SetClipper(obj->clipper);

    v8 = ddraw->CreatePalette(DDPCAPS_ALLOW256 | DDPCAPS_8BIT, pal, &obj->ddrawPal, 0);
    if ( v8 )
    {
        log_d3d_fail("windd.class", "CreatePalette()", v8);
        return 0;
    }

    obj->primary_surf->SetPalette(obj->ddrawPal);

    obj->back_surf->SetPalette(obj->ddrawPal);

    log_d3dlog("-> Leaving wdd_Create2DFullEnv()\n");

    return 1;
}

int load_font(const char *fontname)
{
    if ( !font )
        return 0;

    char buf[128];

    strcpy(buf, fontname);

    const char *facename = strtok(buf, ",");
    const char *s_height = strtok(0, ",");
    const char *s_weight = strtok(0, ",");
    const char *s_charset = strtok(0, ",");

    int height, weight, charset;
    if ( facename && s_height && s_weight && s_charset )
    {
        height = atoi(s_height);
        weight = atoi(s_weight);
        charset = atoi(s_charset);
    }
    else
    {
        height = 12;
        charset = 0;
        facename = "MS Sans Serif";
        weight = 400;
    }

    if ( font )
    {
        if ( font->hFont )
        {
            DeleteObject(font->hFont);
            font->hFont = 0;
        }
    }

    font->height = height;

    font->hFont = CreateFont(-height, 0, 0, 0, weight, 0, 0, 0, charset, 0, 0, 1, 0, facename);
    if ( font->hFont )
        return 1;

    return 0;
}

void windd_func0__sub2__sub4__sub0()
{
    if ( font )
    {
        if ( font->hFont )
        {
            DeleteObject(font->hFont);
            font->hFont = NULL;
        }
    }
}

int windd_func0__sub2__sub4(IDirectDraw *ddraw, IDirectDrawSurface *ddsurf, const char *fontname)
{
    font = (wdd_font *)AllocVec(sizeof(wdd_font), 0);

    int v6 = 0;

    if ( font )
    {
        font->ddraw = ddraw;
        font->ddsurf = ddsurf;
        if ( load_font(fontname) )
            v6 = 1;
    }

    if ( !v6 )
    {
        if ( font )
        {
            windd_func0__sub2__sub4__sub0();
            nc_FreeMem(font);
            font = NULL;
        }
    }

    return v6;
}

int windd_func0__sub2(__NC_STACK_win3d *obj, UA_PALETTE *palette, int width, unsigned int height, int bits)
{
    PALETTEENTRY tmpPal[256];

    log_d3dlog("-> Entering wdd_InitDDrawStuff()\n");

    if ( ghInstance )
    {
        if ( !gCmdShow )
            return 0;

        if ( palette )
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i].peRed = palette->pal_entries[i].r;
                tmpPal[i].peGreen = palette->pal_entries[i].g;
                tmpPal[i].peBlue = palette->pal_entries[i].b;
                tmpPal[i].peFlags = 0;
            }
        }
        else
        {
            for (int i = 0; i < 256; i++)
            {
                tmpPal[i].peRed = 255 * ((i >> 5) & 7) / 7;
                tmpPal[i].peGreen = 255 * ((i >> 2) & 7) / 7;
                tmpPal[i].peBlue = 255 * (i & 3) / 7;
                tmpPal[i].peFlags = 0;
            }
        }

        obj->field_28 = -1;
        obj->width = width;
        obj->cursor = ghInstance;
        obj->height = height;

        if ( createWindow(obj, ghInstance, gCmdShow, width, height) )
        {
            log_d3dlog("->     after wdd_ValidateWindow()\n");

            if ( dword_514EFC )
            {
                if ( obj->field_50 & 1 )
                {
                    if ( !wdd_Create3DWinEnv(obj, width, height) )
                        return 0;
                }
                else if ( !wdd_Create3DFullEnv(obj, width, height, bits) )
                {
                    return 0;
                }

                log_d3dlog("->     after wdd_Create3DFull/WinEnv()\n");

                if ( !wdd_Create3D(obj, width, height) )
                    return 0;

                log_d3dlog("->     after wdd_Create3D()\n");
            }
            else
            {
                if ( obj->field_50 & 1 )
                {
                    if ( !wdd_Create2DWinEnv(obj, tmpPal, width, height) )
                        return 0;
                }
                else if ( !wdd_Create2DFullEnv(obj, tmpPal, width, height) )
                {
                    return 0;
                }

                log_d3dlog("->     after wdd_Create2DFull/WinEnv()\n");
            }

            dd_params.field_AD04 = windd_func0__sub2__sub4(ddraw, obj->back_surf, "MS Sans Serif,12,400,0");

            log_d3dlog("->     after dbcs_Init()\n");

            memset(&dd_params.ddSurfDescr__primary, 0, sizeof(DDSURFACEDESC));

            dd_params.ddSurfDescr__primary.dwSize = sizeof(DDSURFACEDESC);

            HRESULT v17 = obj->primary_surf->GetSurfaceDesc(&dd_params.ddSurfDescr__primary);
            if ( v17 )
            {
                log_d3d_fail("windd.class/wdd_winbox.c", "IDirectDrawSurface::GetSurfaceDesc(Primary)", v17);
                return 0;
            }

            log_d3dlog("->     after GetSurfaceDesc()\n");

            if ( dd_params.selected_device.unk2_def_0 || obj->field_30 )
                ShowCursor(0);
            log_d3dlog("-> Leaving wdd_InitDrawStuff()\n");
            return 1;
        }
        else
        {
            log_d3dlog("wdd_ValidateWindow() failed.\n");
            return 0;
        }
    }
    else
        return 0;
}

size_t NC_STACK_win3d::windd_func0(stack_vals *stak)
{
    stack_vals tmp[8];
    memset(tmp, 0, sizeof(stack_vals) * 8);

    int txt16bit_def = read_yes_no_status("env/txt16bit.def", 1);
    int drawprim_def = read_yes_no_status("env/drawprim.def", 0);
    txt16bit = txt16bit_def;

    get_keyvalue_from_ini(0, windd_keys, 7);

    init_list(&modes_list);
    init_list(&graph_drivers_list);
    int export_window_mode = windd_keys[6].value.val;     // gfx.export_window_mode

    if ( !windd_func0__sub1(windd_keys[6].value.val) )
        return 0;

    int v7 = find_id_in_stack_def_val(DISP_ATT_DISPLAY_ID, 0, stak);
    UA_PALETTE *pal = (UA_PALETTE *)find_id_in_stack_def_val(BMD_ATT_PCOLORMAP, 0, stak);

    mode_node *picked;

    if ( v7 )
    {
        picked = (mode_node *)modes_list.head;
        if ( modes_list.head->next )
        {
            while ( v7 != picked->sort_id )
            {
                picked = (mode_node *)picked->next;
                if ( !picked->next )
                {
                    picked = sub_41F68C();
                    break;
                }
            }
        }
        else
        {
            picked = sub_41F68C();
        }
    }
    else
    {
        picked = windd_func0__sub0("env/vid.def");
    }

    log_d3dlog("ddraw init: picked mode %s\n", picked->name);

    if ( picked->field_94 & 8 )
    {
        tmp[0].set(BMD_ATT_WIDTH, picked->width / 2);
        tmp[1].set(BMD_ATT_HEIGHT, picked->height / 2);
    }
    else
    {
        tmp[0].set(BMD_ATT_WIDTH, picked->width);
        tmp[1].set(BMD_ATT_HEIGHT, picked->height);
    }
    tmp[2].set(BMD_ATT_BUFFER, 1);
    tmp[3].set(BMD_ATT_HAS_COLORMAP, 1);
    tmp[4].nextStack(stak);

    if ( !NC_STACK_display::func0(tmp) )
        return 0;

    __NC_STACK_win3d *win3d = &stack__win3d;

    win3d->field_30 = 0;
    win3d->sort_id = picked->sort_id;
    win3d->movie_player = windd_keys[2].value.val;
    win3d->disable_lowres = windd_keys[5].value.val;
    win3d->txt16bit = txt16bit_def;
    win3d->use_simple_d3d = drawprim_def;
    win3d->export_window_mode = export_window_mode;

    win3d->field_50 |= 1; ////HACK

    if ( picked->field_94 & 1 )
        win3d->field_50 |= 1;
    if ( picked->field_94 & 4 )
        win3d->field_50 |= 4;
    if ( picked->field_94 & 8 )
        win3d->field_50 |= 8;
    if ( picked->field_94 & 0x10 )
        win3d->field_50 |= 0x10;

    if ( !windd_func0__sub2(win3d, pal, picked->width, picked->height, picked->bits) )
    {
        log_d3dlog("wdd_InitDDrawStuff() failed.\n");

        func1(NULL);
        return 0;
    }

    FSMgr::FileHandle *fil = uaOpenFile("env/vid.def", "w");
    if ( fil )
    {
        fil->printf("%s\n", picked->name);
        delete fil;
    }

    win3d->field_54______rsrc_field4 = (bitmap_intern *)getRsrc_pData();
    return 1;
}









size_t NC_STACK_win3d::func0(stack_vals *stak)
{
    get_keyvalue_from_ini(0, win3d_keys, 7);

    if ( win3d_keys[6].value.val ) // if force_emul
        return 0;

    dword_514EFC = 1;

    if ( !windd_func0(stak) )
        return 0;

    __NC_STACK_win3d *w3d = &stack__win3d;

    w3d->dither = win3d_keys[0].value.val;
    w3d->filter = win3d_keys[1].value.val;
    w3d->antialias = win3d_keys[2].value.val;
    w3d->zbuf_when_tracy = win3d_keys[4].value.val;
    w3d->colorkey = win3d_keys[5].value.val;

    if ( can_srcblend )
        w3d->alpha = 192;
    else
        w3d->alpha = 128;

    if ( !win3dInitialisation(w3d) )
    {
        ypa_log_out("win3d.class: Initialization failed.\n");
        func1(NULL);
        return 0;
    }

    if ( !initPixelFormats(w3d) )
    {
        ypa_log_out("win3d.class: Pixelformat problems.\n");
        func1(NULL);
        return 0;
    }

    if ( !initTextureCache(w3d) )
    {
        ypa_log_out("win3d.class: Failed to initialize texture cache.\n");
        func1(NULL);
        return 0;
    }

    if ( !initPolyEngine(w3d) )
    {
        ypa_log_out("win3d.class: Failed to initialize polygon engine.\n");
        func1(NULL);
        return 0;
    }

    return 1;
}






void windd_func1__sub1__sub0()
{
    if ( font )
    {
        if ( font->hFont )
        {
            DeleteObject(font->hFont);
            font->hFont = NULL;
        }
        nc_FreeMem(font);
        font = NULL;
    }
}

void wdd_KillDDrawStuff(__NC_STACK_win3d *win3d)
{
    log_d3dlog("-> Entering wdd_KillDDrawStuff()\n");

    windd_func1__sub1__sub0();

    dd_params.field_AD04 = 0;

    if ( !(win3d->field_50 & 1) )
        ddraw->RestoreDisplayMode();

    if ( win3d->hwnd )
    {
        DDBLTFX fx;
        fx.dwFillColor = 0;
        fx.dwSize = sizeof(DDBLTFX);

        if ( win3d->primary_surf )
            win3d->primary_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

        if ( win3d->back_surf )
            win3d->back_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

        SetClassLong(win3d->hwnd, 0, 0);

        sub_42BD38(win3d);

        win3d->hwnd = 0;
    }

    win3d->cursor = 0;

    log_d3dlog("-> Leaving wdd_KillDDrawStuff()\n");
}

void windd_func1__sub0()
{
    if ( d3d2 )
    {
        d3d2->Release();
        d3d2 = NULL;
    }

    if ( ddraw )
    {
        ddraw->Release();
        ddraw = NULL;
    }
}


size_t NC_STACK_win3d::windd_func1(stack_vals *stak)
{
    wdd_KillDDrawStuff(&stack__win3d);
    while ( 1 )
    {
        nnode *nod = RemHead((nlist *)&modes_list);

        if ( !nod )
            break;

        nc_FreeMem(nod);
    }
    while ( 1 )
    {
        nnode *nod = RemHead(&graph_drivers_list);

        if ( !nod )
            break;

        nc_FreeMem(nod);
    }
    windd_func1__sub0();
    return NC_STACK_display::func1(stak);
}

void win3d_func1__sub1(__NC_STACK_win3d *w3d)
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

void win3d_func1__sub0(__NC_STACK_win3d *w3d)
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

    sub_439E30(w3d);

    win3d_func1__sub1(w3d);
    win3d_func1__sub0(w3d);

    return windd_func1(stak);
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

            case WDD_ATT_CURSOR:
                setWDD_cursor(stk->value.i_data);
                break;

            case WDD_ATT_DIS_LOWRES:
                setWDD_disLowRes(stk->value.i_data);
                break;

            case WDD_ATT_16BIT_TEX:
                setWDD_16bitTex(stk->value.i_data);
                break;

            case WDD_ATT_DRAW_PRIM:
                setWDD_drawPrim(stk->value.i_data);
                break;

            case WDD_ATT_TEXFILT:
                setW3D_texFilt(stk->value.i_data);
                break;
            }
            stk++;
        }
    }
    return NC_STACK_display::func2(stak);
}

size_t NC_STACK_win3d::func3(stack_vals *stak)
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

            case DISP_ATT_DISPLAY_ID:
                *(int *)stk->value.p_data = getDISP_displID();
                break;
            case DISP_ATT_DISPLAY_INF:
                *(gfx_window **)stk->value.p_data = getDISP_displInf();
                break;
            case WDD_ATT_16BIT_TEX:
                *(int *)stk->value.p_data = getWDD_16bitTex();
                break;
            case WDD_ATT_DRAW_PRIM:
                *(int *)stk->value.p_data = getWDD_drawPrim();
                break;
            }
            stk++;
        }
    }
    return NC_STACK_display::func3(stak);
}


// Draw line
void sub_43CD40(__NC_STACK_win3d *w3d, int x1, int y1, int x2, int y2, unsigned int r, unsigned int g, unsigned int b, int, int, int )
{
    if ( w3d->surface_locked_surfaceData )
    {
        int BytesPerColor = w3d->bigdata->primary__pixelformat.BytesPerColor;

        int rilWidth = w3d->surface_locked_pitch / (unsigned int)BytesPerColor;

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
            WORD *surf = ((WORD *)w3d->surface_locked_surfaceData + rilWidth * y1 + x1);

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
            DWORD *surf = ((DWORD *)w3d->surface_locked_surfaceData + rilWidth * y1 + x1);

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

size_t NC_STACK_win3d::raster_func192(stack_vals *)
{
    return 1;
}

size_t NC_STACK_win3d::raster_func198(w3d_func198arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;
    __NC_STACK_win3d *w3d = &stack__win3d;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    sub_43CD40(w3d, x1, y1, x2, y2,
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
    __NC_STACK_display *rstr = &stack__display;
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_43CD40(w3d,
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

void sub_420EDC(__NC_STACK_display *rstr, __NC_STACK_win3d *w3d, int x1, int y1, int x2, int y2, int r, unsigned int g, unsigned int b, unsigned int a11, int a12, int a13)
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
            sub_43CD40(w3d, tmp2.x1, tmp2.y1, tmp2.x2, tmp2.y2, r, g, b, a11, a12, a13);
        }
        else if ( !v14 )
        {
            if ( tmp2.x2 != tmp1.x2 || tmp2.y2 != tmp1.y2 )
            {
                if ( tmp2.x1 != tmp1.x1 || tmp2.y1 != tmp1.y1 )
                {
                    sub_43CD40(w3d, tmp1.x1, tmp1.y1, tmp2.x1, tmp2.y1, r, g, b, a11, a12, a13);
                    sub_43CD40(w3d, tmp2.x2, tmp2.y2, tmp1.x2, tmp1.y2, r, g, b, a11, a12, a13);
                }
                else
                {
                    sub_43CD40(w3d, tmp2.x2, tmp2.y2, tmp1.x2, tmp1.y2, r, g, b, a11, a12, a13);
                }
            }
            else
            {
                sub_43CD40(w3d, tmp1.x1, tmp1.y1, tmp2.x1, tmp2.y1, r, g, b, a11, a12, a13);
            }
        }
    }
}

size_t NC_STACK_win3d::raster_func200(w3d_func198arg *arg)
{
    __NC_STACK_display *rstr = &stack__display;
    __NC_STACK_win3d *w3d = &stack__win3d;

    float tX = rstr->field_554 - 1.0;
    float tY = rstr->field_558 - 1.0;

    int y1 = (arg->y1 + 1.0) * tY;
    int y2 = (arg->y2 + 1.0) * tY;
    int x1 = (arg->x1 + 1.0) * tX;
    int x2 = (arg->x2 + 1.0) * tX;

    sub_420EDC(rstr, w3d, x1, y1, x2, y2,
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
    __NC_STACK_display *rstr = &stack__display;
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_420EDC(rstr, w3d,
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

void sub_43CEE0(__NC_STACK_win3d *w3d, void *srcBuf, int width, int a1, int a2, int a3, int a4, int x1, int y1, int x2, int y2)
{
    if ( w3d->surface_locked_surfaceData )
    {
        if (w3d->bigdata->primary__pixelformat.BytesPerColor == 2)
        {
            WORD *dstSurf = (WORD *)w3d->surface_locked_surfaceData;
            if ( x2 != x1 && y2 != y1 )
            {
                DWORD v21 = a2 << 16;
                for (int i = y1; i < y2; i++ )
                {
                    WORD *buf = &dstSurf[i * (w3d->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor)];

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
            DWORD *dstSurf = (DWORD *)w3d->surface_locked_surfaceData;
            if ( x2 != x1 && y2 != y1 )
            {
                DWORD v21 = a2 << 16;
                for (int i = y1; i < y2; i++ )
                {
                    DWORD *buf = &dstSurf[i * (w3d->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor)];

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
    __NC_STACK_display *rstr = &stack__display;
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

    sub_43CEE0(w3d, (WORD *)pbitm->buffer, pbitm->width, a1, a2, a3, a4, a5, a6, a7, a8);
    return 1;
}


int win3d_func204__sub0(__NC_STACK_display *rstr, rstr_loc204 *arg)
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

    __NC_STACK_display *rstr = &stack__display;
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
        sub_43CEE0(w3d, (WORD *)loc.pbitm->buffer, loc.pbitm->width, loc.dword4, loc.dword8, loc.dwordC, loc.dword10, loc.dword18, loc.dword1C, loc.dword20, loc.dword24);

    return 1;
}

void sb_0x43b518__sub0__sub0( __NC_STACK_win3d *w3d)
{
    if ( w3d->bigdata->sceneBeginned )
    {
        if ( w3d->use_simple_d3d )
        {
            win3d__SetRenderState(w3d, D3DRENDERSTATE_FLUSHBATCH, 1);
        }
        else
        {
            sub_43BE88(w3d);
            sub_43BD50(w3d);
        }
    }
}

int loadTextureToCache(__NC_STACK_win3d *w3d, texStru *tex, int id)
{
    texCache *ptx = &w3d->bigdata->texCh[id];
    if ( !w3d->use_simple_d3d && ptx->loadedFromTexture.dx1)
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

D3DTEXTUREHANDLE getTexHandle(__NC_STACK_win3d *w3d, texCache *tex)
{
    D3DTEXTUREHANDLE d3dtexHandle;
    HRESULT res;

    if ( w3d->use_simple_d3d )
        res = tex->d3dtex2->GetHandle(w3d->intern->d3d2dev, &d3dtexHandle);
    else
        res = tex->d3dtex->GetHandle(w3d->intern->d3d1Dev, &d3dtexHandle);
    if ( res )
        return 0;

    return d3dtexHandle;
}


D3DTEXTUREHANDLE sb_0x43b518__sub0(__NC_STACK_win3d *w3d, texStru *tex, int a4)
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
            return getTexHandle(w3d, ptx);
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

            sb_0x43b518__sub0__sub0(w3d);
            if ( loadTextureToCache(w3d, tex, idd) )
                return getTexHandle(w3d, &w3d->bigdata->texCh[idd]);
        }
    }
    else if ( loadTextureToCache(w3d, tex, id) )
    {
        return getTexHandle(w3d, &w3d->bigdata->texCh[id]);
    }
    return 0;
}

int SetRenderStates(__NC_STACK_win3d *w3d, int arg)
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
        execBuff_initRenderStates(w3d);

        for (int i = 0; i < 11; i++)
        {
            if (execBuff->rendStates2[i].value != execBuff->rendStates[i].value)
                win3d__SetRenderState(w3d, execBuff->rendStates2[i].d3dRenderState, execBuff->rendStates2[i].value);

            execBuff->rendStates[i].value = execBuff->rendStates2[i].value;
        }

        execBuff_finishRenderStates(w3d);
        return 1;
    }

    return 0;
}


void DrawPrimitive(__NC_STACK_win3d *w3d, D3DTLVERTEX *vtx, int vtxCount)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    if ( !w3d->bigdata->sceneBeginned )
        return;

    if ( w3d->use_simple_d3d )
    {
        HRESULT res = w3d->intern->d3d2dev->DrawPrimitive(D3DPT_TRIANGLEFAN, D3DVT_TLVERTEX, vtx, vtxCount, D3DDP_DONOTCLIP | D3DDP_DONOTUPDATEEXTENTS );
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
                sub_43BE88(w3d);
                sub_43BD50(w3d);
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

void sb_0x43b518(__NC_STACK_win3d *w3d, polysDatSub *polysDat, texStru *tex, int a5, int a6)
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
        texHndl = sb_0x43b518__sub0(w3d, tex, a5);
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
            vOut[i].color |= w3d->alpha << 24;
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

    SetRenderStates(w3d, 0);
    DrawPrimitive(w3d, vOut, polysDat->vertexCount);
    return;
}

size_t NC_STACK_win3d::raster_func206(polysDatSub *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    if ( arg->pbitm )
        sb_0x43b518(w3d, arg, arg->pbitm->ddrawSurfTex, 0, 0);
    else
        sb_0x43b518(w3d, arg, NULL, 0, 0);

    return 1;
}

void win3d_func209__sub0(__NC_STACK_win3d *w3d, tiles_stru **tiles, char *cmdline, char **arr)
{
    ////printf("CLEAN ME %s\n","win3d_func209__sub0");

    int v11;


    if ( w3d->surface_locked_surfaceData )
    {
        win3d_bigdata *bigdata = w3d->bigdata;
        int bytesPerColor = bigdata->primary__pixelformat.BytesPerColor;
        DWORD FFFF0000__color = w3d->bigdata->primary__pixelformat.FFFF0000__color;

        char *curpos = cmdline;
        int w_pixels = w3d->surface_locked_pitch / bytesPerColor;
        tiles_stru *tile = NULL;


        int x_out = 0;
        int y_out = 0;

        int x_out_txt = 0;
        int y_out_txt = 0;
        int txt_flag = 0;

        int y_pos_line = 0;
        int x_pos_line = 0;



        int rilHeight, rilWidth;

        if ( w3d->field_50 & 8 )
        {
            rilHeight = w3d->height / 2;
            rilWidth = w3d->width / 2;
        }
        else
        {
            rilHeight = w3d->height;
            rilWidth = w3d->width;
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
                    WORD *dstpixel = (WORD *)w3d->surface_locked_surfaceData + w_pixels * y_out + x_out;

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
                    DWORD *dstpixel = (DWORD *)w3d->surface_locked_surfaceData + w_pixels * y_out + x_out;

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
                        w3d->surface_locked_surfaceData = a1.lpSurface;
                        w3d->surface_locked_pitch = a1.lPitch;
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
    win3d_func209__sub0(&stack__win3d, stack__display.tiles, arg->cmdbuf, arg->includ);
}

void win3d_func213__sub0__sub0(__NC_STACK_win3d *w3d)
{
    for(int texid = 0; texid < w3d->bigdata->texCh_count; texid++)
    {
        texCache *texCh = &w3d->bigdata->texCh[texid];
        if ( texCh->ddDrawSurface->IsLost() )
        {
            texCh->ddDrawSurface->Restore();
            if ( texCh->loadedFromTexture.dx1 )
                loadTextureToCache(w3d, texCh->txStru, texid);
        }
        texCh->used &= 0xFFFFFFFDu;
    }
}

void win3d_func213__sub0(__NC_STACK_win3d *w3d)
{
    if ( w3d->hwnd )
    {
        win3d_func213__sub0__sub0(w3d);
        win3d__beginScene(w3d);
    }
}

void NC_STACK_win3d::raster_func213(polysDatSub *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func213__sub0(w3d);
}

int sub_43BB6C(const void * a, const void * b)
{
    wind3d_sub1 *a1 = (wind3d_sub1 *)a;
    wind3d_sub1 *a2 = (wind3d_sub1 *)b;

    return a2->tex->texture.dx1 - a1->tex->texture.dx1;
}

void win3d_func214__sub1(__NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;
    if ( bigdata->sceneBeginned )
    {
        if ( bigdata->subt1_count )
        {
            qsort(bigdata->subt1, bigdata->subt1_count, sizeof(wind3d_sub1), sub_43BB6C);

            for (int i = 0; i < bigdata->subt1_count; i++)
                sb_0x43b518(w3d, bigdata->subt1[i].polyData, bigdata->subt1[i].tex, 1, 0);

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

void win3d_func214__sub2(__NC_STACK_win3d *w3d)
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
                sb_0x43b518(w3d, bigdata->dat_1C14[i].polyData, bigdata->dat_1C14[i].tex, 1, 1);

            bigdata->dat_1C14_count = 0;
        }
    }
}

void win3d_func214__sub0(__NC_STACK_win3d *w3d)
{
    win3d_bigdata *bigdata = w3d->bigdata;

    for (int i = 0; i < bigdata->texCh_count; i++)
        if (bigdata->texCh[i].used & 2)
            bigdata->texCh[i].used |= 1;
}

// Draw transparent
size_t NC_STACK_win3d::raster_func214(void *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func214__sub1(w3d);
    win3d_func214__sub2(w3d);
    win3d__endScene(w3d);
    win3d_func214__sub0(w3d);
    return 1;
}


int win3d_func215__sub0(__NC_STACK_win3d *w3d)
{
    if ( !w3d->hwnd )
    {
        w3d->surface_locked_pitch = 0;
        w3d->surface_locked_surfaceData = NULL;
        return 0;
    }

    if ( w3d->surface_locked_surfaceData )
        return 1;

    sub_42A640(w3d);

    DDSURFACEDESC surfDesc;
    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);

    HRESULT res = w3d->back_surf->Lock(NULL, &surfDesc, DDLOCK_WAIT | DDLOCK_NOSYSLOCK, 0);

    if ( res )
    {
        log_d3d_fail("w3d_LockBackBuffer()", "Lock() failed.", res);
        w3d->surface_locked_pitch = 0;
        w3d->surface_locked_surfaceData = NULL;
        return 0;
    }

    w3d->surface_locked_surfaceData = surfDesc.lpSurface;
    w3d->surface_locked_pitch = surfDesc.lPitch;
    return 1;
}

void NC_STACK_win3d::raster_func215(void *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func215__sub0(w3d);
}


void win3d_func216__sub0(__NC_STACK_win3d *w3d)
{
    if ( w3d->hwnd )
    {
        if ( w3d->surface_locked_surfaceData )
        {
            w3d->back_surf->Unlock(NULL);
            w3d->surface_locked_surfaceData = NULL;
            w3d->surface_locked_pitch = 0;
        }
    }
}

void NC_STACK_win3d::raster_func216(void *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func216__sub0(w3d);
}




void win3d_func218__sub0(__NC_STACK_win3d *w3d, void *buf1, int width, BYTE *buf2, int elmnt, ua_dRect rect, ua_dRect rect2)
{
    if ( w3d->surface_locked_surfaceData )
    {
        if (w3d->bigdata->primary__pixelformat.BytesPerColor == 2)
        {
            WORD *locked = (WORD *)w3d->surface_locked_surfaceData;

            int wdth = w3d->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor;


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
            DWORD *locked = (DWORD *)w3d->surface_locked_surfaceData;

            int wdth = w3d->surface_locked_pitch / w3d->bigdata->primary__pixelformat.BytesPerColor;


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
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *rstr = &stack__display;

    ua_dRect rect1, rect2;

    rect1.x1 = (arg->rect1.x1 + 1.0) * (arg->bitm_intern->width / 2);
    rect1.y1 = (arg->rect1.y1 + 1.0) * (arg->bitm_intern->height / 2);
    rect1.x2 = (arg->rect1.x2 + 1.0) * (arg->bitm_intern->width / 2);
    rect1.y2 = (arg->rect1.y2 + 1.0) * (arg->bitm_intern->height / 2);

    rect2.x1 = (arg->rect2.x1 + 1.0) * rstr->field_554;
    rect2.y1 = (arg->rect2.y1 + 1.0) * rstr->field_558;
    rect2.x2 = (arg->rect2.x2 + 1.0) * rstr->field_554;
    rect2.y2 = (arg->rect2.y2 + 1.0) * rstr->field_558;

    win3d_func218__sub0(w3d, (WORD *)arg->bitm_intern->buffer, arg->bitm_intern->width, (BYTE *)arg->bitm_intern2->buffer, arg->flg, rect1, rect2);
}

size_t NC_STACK_win3d::display_func256(windd_arg256 *inout)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    mode_node *nod;

    if ( inout->sort_id )
    {
        mode_node *tmp = (mode_node *)modes_list.head;

        if ( modes_list.head->next )
        {
            while ( inout->sort_id != tmp->sort_id )
            {
                tmp = (mode_node *)tmp->next;

                if ( !tmp->next )
                {
                    tmp = sub_41F68C();
                    break;
                }
            }
        }
        else
        {
            tmp = sub_41F68C();
        }

        nod = tmp;
    }
    else if ( w3d->disable_lowres )
    {
        mode_node *tmp = (mode_node *)modes_list.head;

        while (tmp->next)
        {
            nod = tmp;
            if ( tmp->rel_width >= 512 )
                break;
            tmp = (mode_node *)tmp->next;
        }

        if ( !tmp->next )
            nod = NULL;
    }
    else
    {
        nod = (mode_node *)modes_list.head;
    }

    if ( !nod )
        return 0;

    inout->sort_id = nod->sort_id;
    inout->width = nod->width;
    inout->height = nod->height;

    strncpy(inout->name, nod->name, 32);

    if (nod->next->next)
        return ((mode_node*)nod->next)->sort_id;
    else
        return 0;
}


HRESULT clearViewport(__NC_STACK_win3d *wdd)
{
    if ( dword_514EFC )
    {
        DWORD flags = D3DCLEAR_TARGET;
        if ( wdd->intern->z_buf )
            flags = D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER;

        D3DRECT rect;
        rect.x1 = 0;
        rect.y1 = 0;
        rect.x2 = wdd->width;
        rect.y2 = wdd->height;

        return wdd->intern->d3d2Viewport->Clear(1, &rect, flags);
    }
    else
    {
        DDBLTFX fx;
        fx.dwFillColor = dword_514EFC;
        fx.dwSize = sizeof(DDBLTFX);

        return wdd->back_surf->Blt(NULL, NULL, NULL,  DDBLT_WAIT | DDBLT_COLORFILL, &fx);
    }
}

void clearAndLockBackBufferSurface(__NC_STACK_win3d *wdd)
{
    POINT Point;

    GetCursorPos(&Point);
    SetCursorPos(Point.x, Point.y);

    if ( wdd->hwnd )
    {
        sub_42A640(wdd);
        clearViewport(wdd);

        if ( dword_514EFC )
        {
            wdd->surface_locked_pitch = 0;
            wdd->surface_locked_surfaceData = NULL;
        }
        else if ( !wdd->surface_locked_surfaceData )
        {
            DDSURFACEDESC surfDesc;
            memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
            surfDesc.dwSize = sizeof(DDSURFACEDESC);

            HRESULT res = wdd->back_surf->Lock(NULL, &surfDesc, DDLOCK_NOSYSLOCK | DDLOCK_WAIT, NULL);

            if ( res )
            {
                wdd->surface_locked_surfaceData = NULL;
                wdd->surface_locked_pitch = 0;
                log_d3d_fail("wdd_winbox.c/wdd_Begin", "Lock on back surface failed", res);
            }
            else
            {
                wdd->surface_locked_surfaceData = surfDesc.lpSurface;
                wdd->surface_locked_pitch = surfDesc.lPitch;
            }
        }
    }
    else
    {
        wdd->surface_locked_pitch = 0;
        wdd->surface_locked_surfaceData = NULL;
    }
}

void NC_STACK_win3d::display_func257(stack_vals *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    clearAndLockBackBufferSurface(w3d);
    w3d->field_54______rsrc_field4->buffer = w3d->surface_locked_surfaceData;
    w3d->field_54______rsrc_field4->pitch = w3d->surface_locked_pitch;
}



void  windd_func258__sub1(__NC_STACK_win3d *wdd)
{
    if ( wdd->hwnd )
    {
        if ( !dword_514EFC )
        {
            if ( wdd->surface_locked_surfaceData )
            {
                wdd->back_surf->Unlock(NULL);
                wdd->surface_locked_surfaceData = NULL;
                wdd->surface_locked_pitch = 0;
            }
        }

        if ( wdd->field_50 & 1 )
        {
            RECT rc;
            POINT Point;

            GetClientRect(wdd->hwnd, &rc);

            Point.x = 0;
            Point.y = 0;
            ClientToScreen(wdd->hwnd, &Point);
            OffsetRect(&rc, Point.x, Point.y);

            wdd->primary_surf->Blt(&rc, wdd->back_surf, NULL, DDBLT_WAIT, NULL);
        }
        else
        {
            if ( dword_514EFC )
            {
                wdd->primary_surf->Flip(wdd->back_surf, DDFLIP_WAIT);
            }
            else
            {
                wdd->primary_surf->Blt(NULL, wdd->back_surf, NULL, DDBLT_WAIT, NULL);
            }
        }
    }
}

void windd_func258__sub2(__NC_STACK_win3d *wdd, int *x, int *y)
{
    POINT point;
    GetCursorPos(&point);

    *x = (wdd->width * point.x) / GetSystemMetrics(SM_CXSCREEN);
    *y = (wdd->height * point.y) / GetSystemMetrics(SM_CYSCREEN);
}

void windd_func258__sub0(NC_STACK_win3d *obj, __NC_STACK_display *dspl, __NC_STACK_win3d *wdd, int xx, signed int yy)
{
    if ( dspl->pointer_bitm )
    {
        int w = wdd->width;
        int h = wdd->height;

        if ( wdd->field_50 & 8 )
        {
            xx *= 2;
            yy *= 2;
            w *= 2;
            h *= 2;
        }

        if ( xx >= 0 && yy >= 0 && xx < w && yy < h )
        {
            tile_xy v15[2];

            v15[1].byteoff = 0;
            v15[1].width = dspl->pointer_bitm->width;

            tiles_stru v13;

            v13.font_image = NULL;
            v13.field_4 = dspl->pointer_bitm;
            v13.field_8 = v13.field_4->buffer;
            v13.chars = v15;
            v13.font_height = dspl->pointer_bitm->height;

            rstr_207_arg v16;
            v16.id = 127;
            v16.tiles = &v13;

            obj->raster_func207(&v16);

            int v21 = 0;
            int v20 = 0;

            if ( xx + dspl->pointer_bitm->width > w)
            {
                v21 = w - xx;
                if (w == xx)
                    return;
            }

            if ( yy + dspl->pointer_bitm->height > h)
            {
                v20 = h - yy;
                if (h == yy)
                    return;
            }

            char cmdBuff[64];
            char *cmdPoint = &cmdBuff[0];

            FontUA::select_tileset(&cmdPoint, 127);
            FontUA::set_center_xpos(&cmdPoint, xx - (w / 2));
            FontUA::set_center_ypos(&cmdPoint, yy - (h / 2));

            if (v21)
                FontUA::set_xwidth(&cmdPoint, v21);

            if (v20)
                FontUA::set_yheight(&cmdPoint, v20);

            FontUA::store_u8(&cmdPoint, 1);

            FontUA::set_end(&cmdPoint);

            w3d_a209 a209;
            a209.cmdbuf = cmdBuff;
            a209.includ = NULL;

            obj->raster_func209(&a209);
        }
    }
}

void NC_STACK_win3d::display_func258(stack_vals *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *dspl = &stack__display;

    if ( sub_42AC78(w3d) )
    {
        int yy;
        int xx;

        windd_func258__sub2(w3d, &xx, &yy);

        raster_func215(NULL);
        windd_func258__sub0(this, dspl, w3d, xx, yy);
        raster_func216(NULL);
    }
    windd_func258__sub1(w3d);
}


void NC_STACK_win3d::display_func261(rstr_261_arg *arg)
{
    if ( !dword_514EFC && !arg->pal_id && !arg->entrie_id )
    {
        arg->palette->pal_entries[0].r = 0;
        arg->palette->pal_entries[0].g = 0;
        arg->palette->pal_entries[0].b = 0;
    }
    NC_STACK_display::display_func261(arg);
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

void NC_STACK_win3d::sub_42D37C(__NC_STACK_win3d *wdd, UA_PALETTE *pal)
{
    if ( wdd->ddrawPal )
    {
        PALETTEENTRY tmp[256];

        for(int i = 0; i < 256; i++)
        {
            tmp[i].peRed = pal->pal_entries[i].r;
            tmp[i].peGreen = pal->pal_entries[i].g;
            tmp[i].peBlue = pal->pal_entries[i].b;
            tmp[i].peFlags = 0;
        }

        wdd->ddrawPal->SetEntries(0, 0, 256, tmp);
        wdd->primary_surf->SetPalette(wdd->ddrawPal);
    }
}

void NC_STACK_win3d::display_func262(rstr_262_arg *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func262__sub0(w3d, arg->dword0, arg->pdword4, arg->pdword8);
    NC_STACK_display::display_func262(arg);

    __NC_STACK_display *dspl  = &stack__display;

    sub_42D37C(w3d, &dspl->palette);
}


void NC_STACK_win3d::display_func263(displ_arg263 *arg)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    sub_42D410(w3d, arg->pointer_id, 0);
    NC_STACK_display::display_func263(arg);
}





int win3d__allocTexBuffer(__NC_STACK_win3d *w3d, int w, int h, bitmap_intern *arg)
{
    arg->buffer = NULL;

    void *buf = AllocVec(w * h * w3d->bigdata->primary__pixelformat.BytesPerColor, 0);
    if (!buf)
        return 0;

    arg->buffer = buf;
    return w * w3d->bigdata->primary__pixelformat.BytesPerColor;
}

signed int win3d__createSurfTexPal(__NC_STACK_win3d *w3d, int w, int h, texStru **_tex)
{
    *_tex = NULL;

    texStru *tex = (texStru *)AllocVec(sizeof(texStru), 0);

    if ( !tex )
    {
        log_d3d_fail("win3d.class/w3d_txtcache.c/ObtainTexture()", "Out Of Mem", 0);
        return 0;
    }
    *_tex = tex;

    windd_formats *format = &w3d->intern->formats[w3d->intern->selected_format_id];
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
    __NC_STACK_win3d *w3d = &stack__win3d;

    bitmap_intern *bitm_intern = *arg;

    if ( bitm_intern->flags & 8 )
    {
        bitm_intern->pitch = win3d__allocTexBuffer(w3d, bitm_intern->width, bitm_intern->height, bitm_intern);
        return bitm_intern->pitch != 0;
    }
    else
    {
        return win3d__createSurfTexPal(w3d, bitm_intern->width, bitm_intern->height, &bitm_intern->ddrawSurfTex);
    }
}

void win3d__tex_apply_palette_hw(__NC_STACK_win3d *w3d, UA_PALETTE *pal, __NC_STACK_display *dspl, texStru *tex, int a5)
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

void win3d__tex_apply_palette(__NC_STACK_win3d *w3d, UA_PALETTE *pal, __NC_STACK_display *dspl, int w, int h, void *buf)
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
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *dspl = &stack__display;

    bitmap_intern *bitm = *arg;

    if (bitm->flags & BITMAP_FLAG_SYSMEM)
        win3d__tex_apply_palette(w3d, bitm->pallete, dspl, bitm->width, bitm->height, bitm->buffer);
    else
        win3d__tex_apply_palette_hw(w3d, bitm->pallete, dspl, bitm->ddrawSurfTex, bitm->flags & 0x10);
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


void win3d_func271__sub0(__NC_STACK_win3d *w3d)
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
    __NC_STACK_win3d *w3d = &stack__win3d;

    win3d_func271__sub0(w3d);
    display_func272(stak);
}


void win3d_func274__sub0(__NC_STACK_win3d *w3d, FSMgr::FileHandle *fil)
{
    if ( w3d->surface_locked_surfaceData )
    {
        int BytesPerColor = w3d->bigdata->primary__pixelformat.BytesPerColor;
        int ril_width = w3d->surface_locked_pitch / BytesPerColor;

        int width = w3d->width;
        int height = w3d->height;

        char buf[128];

        sprintf(buf, "P6\n#YPA screenshot\n%d %d\n255\n", width, height);
        fil->write(buf, strlen(buf));

        wind3d_pixelformat *fmt = &w3d->bigdata->primary__pixelformat;
        if ( BytesPerColor == 2 )
        {
            int pos = 0;
            for (int j = 0; j < height; j++)
            {
                WORD *line = (WORD *)w3d->surface_locked_surfaceData + pos;

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

                    fil->writeU8(r);
                    fil->writeU8(g);
                    fil->writeU8(b);
                }

                pos += ril_width;
            }
        }
        else if ( BytesPerColor == 4 )
        {
            int pos = 0;
            for (int j = 0; j < height; j++)
            {
                DWORD *line = (DWORD *)w3d->surface_locked_surfaceData + pos;

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

                    fil->writeU8(r);
                    fil->writeU8(g);
                    fil->writeU8(b);
                }

                pos += ril_width;
            }
        }
    }
}

void NC_STACK_win3d::display_func274(const char **name)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    char filename[128];

    strcpy(filename, *name);
    strcat(filename, ".ppm");

    FSMgr::FileHandle *fil = uaOpenFile(filename, "wb");
    if ( fil )
    {
        raster_func215(NULL);
        win3d_func274__sub0(w3d, fil);
        raster_func216(NULL);
        delete fil;
    }
}




void sb_0x42d530__sub0(__NC_STACK_win3d *wdd)
{
    DDSURFACEDESC surfDesc;

    sub_42A640(wdd);

    memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
    surfDesc.dwSize = sizeof(DDSURFACEDESC);
    if ( !wdd->back_surf->GetSurfaceDesc(&surfDesc) )
    {
        int w = surfDesc.dwWidth;
        int h = surfDesc.dwHeight;

        memset(&surfDesc, 0, sizeof(DDSURFACEDESC));
        surfDesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
        surfDesc.dwHeight = h;
        surfDesc.dwWidth = w;
        surfDesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY | DDSCAPS_OFFSCREENPLAIN;
        surfDesc.dwSize = sizeof(DDSURFACEDESC);

        if ( !ddraw->CreateSurface(&surfDesc, &wdd->field_10, NULL) )
        {
            if ( wdd->ddrawPal )
                wdd->field_10->SetPalette(wdd->ddrawPal);

            wdd->field_10->Blt(NULL, wdd->primary_surf, NULL, DDBLT_WAIT, NULL);
        }
    }
}

void sb_0x42d530(__NC_STACK_win3d *wdd, int a2)
{
    if ( wdd->hwnd )
    {
        dword_514F24 = 1;
        if ( !dword_514EFC )
        {
            if ( wdd->surface_locked_surfaceData )
            {
                wdd->back_surf->Unlock(NULL);
                wdd->surface_locked_surfaceData = NULL;
                wdd->surface_locked_pitch = 0;
            }
        }

        sb_0x42d530__sub0(wdd);

        if ( !(wdd->field_50 & 1) )
        {
            if ( dword_514EFC )
                if ( !(dd_params.field_0 & 1) )
                {
                    ddraw->FlipToGDISurface();
                    sub_4BF181(100);
                }

            if ( a2 )
            {
                DDBLTFX fx;
                fx.dwSize = sizeof(DDBLTFX);
                fx.dwFillColor = 0;

                wdd->primary_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);
                wdd->back_surf->Blt(NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL, &fx);

                ddraw->SetDisplayMode(640, 480, 16);
                ddraw->SetCooperativeLevel(wdd->hwnd, DDSCL_NORMAL);

                SetWindowLongPtr(wdd->hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU);
                SetWindowLongPtr(wdd->hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                int h = GetSystemMetrics(SM_CYSCREEN);
                int w = GetSystemMetrics(SM_CXSCREEN);
                SetWindowPos(wdd->hwnd, 0, 0, 0, w, h, SWP_SHOWWINDOW);
            }
            else
            {
                if ( dd_params.ddSurfDescr__primary.dwWidth <= 400 || dd_params.ddSurfDescr__primary.dwHeight <= 300 )
                    ddraw->SetDisplayMode(dd_params.displ_mode_surface.dwWidth,	dd_params.displ_mode_surface.dwHeight, dd_params.displ_mode_surface.ddpfPixelFormat.dwRGBBitCount);

                ddraw->SetCooperativeLevel(wdd->hwnd, DDSCL_NORMAL);
                SetWindowLongPtr(wdd->hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU);
                SetWindowLongPtr(wdd->hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                int h = GetSystemMetrics(SM_CYSCREEN);
                int w = GetSystemMetrics(SM_CXSCREEN);
                SetWindowPos(wdd->hwnd, 0, 0, 0, w, h, SWP_SHOWWINDOW);

                sub_42A7BC(wdd);
            }
        }

        while ( ShowCursor(1) < 0 )
        {}

        HCURSOR Pointer = uaLoadCursor(wdd->cursor, "Pointer");

        if ( Pointer )
            SetCursor(Pointer);
    }
}

void NC_STACK_win3d::windd_func320(stack_vals *)
{
    sb_0x42d530(&stack__win3d, 0);
}





void sub_42D724(__NC_STACK_win3d *wdd, int a2)
{
    if ( wdd->hwnd )
    {
        dword_514F24 = 0;
        if ( wdd->field_10 )
        {
            wdd->field_10->Release();
            wdd->field_10 = NULL;
        }

        int v5 = wdd->field_30;

        if ( dd_params.selected_device.unk2_def_0 || wdd->field_30 != 0 )
            v5 = 1;

        if ( v5 )
            ShowCursor(0);

        if ( !(wdd->field_50 & 1) )
        {
            int v6 = 0;

            ddraw->SetCooperativeLevel(wdd->hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);

            if ( a2 || dd_params.ddSurfDescr__primary.dwWidth <= 400 || dd_params.ddSurfDescr__primary.dwHeight <= 300 )
                v6 = 1;

            if ( v6 )
            {
                ddraw->SetDisplayMode(dd_params.ddSurfDescr__primary.dwWidth, dd_params.ddSurfDescr__primary.dwHeight, dd_params.ddSurfDescr__primary.ddpfPixelFormat.dwRGBBitCount);

                SetWindowLongPtr(wdd->hwnd, GWL_STYLE, WS_POPUP | WS_SYSMENU);
                SetWindowLongPtr(wdd->hwnd, GWL_EXSTYLE, WS_EX_TOPMOST);

                int h = GetSystemMetrics(SM_CYSCREEN);
                int w = GetSystemMetrics(SM_CXSCREEN);

                SetWindowPos(wdd->hwnd, 0, 0, 0, w, h, SWP_SHOWWINDOW);
            }
        }
        sub_42D410(wdd, wdd->field_28, 1);
    }
}

void NC_STACK_win3d::windd_func321(stack_vals *)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *dspl = &stack__display;

    sub_42D724(w3d, 0);
    sub_42D37C(w3d, &dspl->palette);
}





char * windd_func322__sub0(__NC_STACK_win3d *wdd, const char *box_title, const char *box_ok, const char *box_cancel, const char *box_startText, UINT timer_time, void (*timer_func)(int, int, int), void *timer_context, int replace, int maxLen)
{
    dprintf("MAKE ME %s\n","windd_func322__sub0");
    return NULL;
}

//Show DLGBox with edit field and get entered value
void NC_STACK_win3d::windd_func322(windd_dlgBox *dlgBox)
{
    __NC_STACK_win3d *w3d = &stack__win3d;

    windd_func320(NULL);

    dlgBox->result = windd_func322__sub0(
                         w3d,
                         dlgBox->title,
                         dlgBox->ok,
                         dlgBox->cancel,
                         dlgBox->startText,
                         dlgBox->time,
                         dlgBox->timer_func,
                         dlgBox->timer_context,
                         dlgBox->replace,
                         dlgBox->maxLen);

    windd_func321(NULL);
}





void windd_func323__sub0__sub0(const char *filename, HWND hwnd)
{
    printf("MAKE MOVIE PLAYER %s, file %s, HWND %d\n","windd_func323__sub0__sub0", filename, (int)(size_t)hwnd);
}


void windd_func323__sub0(__NC_STACK_win3d *w3d, const char *filename)
{
    if ( w3d->hwnd )
    {
////    unk_514F20 = 1;
        sb_0x42d530(w3d, 1);
        if ( w3d->movie_player )
            windd_func323__sub0__sub0(filename, w3d->hwnd);
        sub_42D724(w3d, 1);
////    unk_514F20 = 0;
    }
}

//Play movie file
void NC_STACK_win3d::windd_func323(const char **filename)
{
    __NC_STACK_win3d *w3d = &stack__win3d;
    __NC_STACK_display *dspl = &stack__display;

    windd_func323__sub0(w3d, *filename);
    sub_42D37C(w3d, &dspl->palette);
}

void NC_STACK_win3d::windd_func324(wdd_func324arg *inout)
{
    wddDevice *findedNode;
    wddDevice *node;

    findedNode = NULL;
    if ( inout->guid )
    {
        node = (wddDevice *)graph_drivers_list.head;
        while (node->next)
        {
            if ( node->guid == inout->guid )
            {
                findedNode = (wddDevice *)node->next;

                if ( node->next->next )
                    break;
            }
            node = (wddDevice *)node->next;

            findedNode = NULL;
        }
    }
    else
    {
        findedNode = (wddDevice *)graph_drivers_list.head;
    }

    if ( findedNode )
    {
        inout->name = findedNode->name;
        inout->guid = findedNode->guid;
        inout->currr = findedNode->curr;
    }
    else
    {
        inout->guid = NULL;
        inout->currr = 0;
        inout->name = NULL;
    }
}

void NC_STACK_win3d::windd_func325(wdd_func324arg *arg)
{
    const char *v4 = arg->guid;
    const char *v5 = "<error>";

    if ( v4 )
    {
        if ( !strcmp(v4, "<primary>") )
        {
            v5 = v4;
            v4 = NULL;
        }
        if ( !strcmp(v4, "<software>") )
        {
            v5 = v4;
            v4 = NULL;
        }
    }

    out_guid_to_file("env/guid3d.def", (GUID *)v4, v5);
}



void NC_STACK_win3d::setWDD_cursor(int arg)
{
    if ( arg || !stack__win3d.field_30 )
    {
        if ( arg == 1 && !stack__win3d.field_30 )
        {
            stack__win3d.field_30 = 1;
            while ( ShowCursor(0) >= 0 )
            { }
        }
    }
    else
    {
        stack__win3d.field_30 = 0;
        while ( ShowCursor(1) < 0 )
        { }
        sub_42D410(&stack__win3d, 1, 1);
    }
}

void NC_STACK_win3d::setWDD_disLowRes(int arg)
{
    stack__win3d.disable_lowres = arg;
}

void NC_STACK_win3d::setWDD_16bitTex(int arg)
{
    out_yes_no_status("env/txt16bit.def", arg);
}

void NC_STACK_win3d::setWDD_drawPrim(int arg)
{
    out_yes_no_status("env/drawprim.def", arg);
}



int NC_STACK_win3d::getDISP_displID()
{
    return stack__win3d.sort_id;
}

gfx_window *NC_STACK_win3d::getDISP_displInf()
{
    static gfx_window window_params__return;
    window_params__return.hwnd = stack__win3d.hwnd;
    if ( stack__win3d.field_50 & 8 )
    {
        window_params__return.width = stack__win3d.width / 2 ;
        window_params__return.height = stack__win3d.height / 2;
    }
    else
    {
        window_params__return.width = stack__win3d.width;
        window_params__return.height = stack__win3d.height;
    }

    return &window_params__return;
}

int NC_STACK_win3d::getWDD_16bitTex()
{
    return stack__win3d.txt16bit;
}

int NC_STACK_win3d::getWDD_drawPrim()
{
    return stack__win3d.use_simple_d3d;
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
    case 3:
        func3( (stack_vals *)data );
        return 1;
    case 192:
        return (size_t)raster_func192( (stack_vals *)data );
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
    case 256:
        return (size_t)display_func256( (windd_arg256 *)data );
    case 257:
        display_func257( (stack_vals *)data );
        return 1;
    case 258:
        display_func258( (stack_vals *)data );
        return 1;
    case 262:
        display_func262( (rstr_262_arg *)data );
        return 1;
    case 263:
        display_func263( (displ_arg263 *)data );
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
    case 320:
        windd_func320( (stack_vals *)data );
        return 1;
    case 321:
        windd_func321( (stack_vals *)data );
        return 1;
    case 322:
        windd_func322( (windd_dlgBox *)data );
        return 1;
    case 323:
        windd_func323( (const char **)data );
        return 1;
    case 324:
        windd_func324( (wdd_func324arg *)data );
        return 1;
    case 325:
        windd_func325( (wdd_func324arg *)data );
        return 1;
    default:
        break;
    }
    return NC_STACK_display::compatcall(method_id, data);
}
