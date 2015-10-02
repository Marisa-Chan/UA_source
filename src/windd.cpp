#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "utils.h"

#include <wingdi.h>


stored_functions *classvtbl_get_windd();
class_return * class_set_windd(int, ...);

stored_functions windd_class_vtbl(class_set_windd);


class_stored windd_class_off (NULL, NULL, "MC2classes:drivers/gfx/windd.class", classvtbl_get_windd);


stored_functions *classvtbl_get_windd()
{
	return &windd_class_vtbl;
}

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



CLASSFUNC windd_funcs[1024];


windd_params dd_params;

IDirectDraw *ddraw;
IDirect3D2 *d3d2;


int txt16bit = 0;

GUID *driver_guid;
char *driver_descript;
char *driver_name;

int dword_514EFC = 0;


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

	FILE *fil = fopen(filename, "w");
	if ( fil )
	{
		guid_to_string(buf, guid, name);
		fputs(buf, fil);
		fclose(fil);
		return 1;
	}
	return 0;
}

int windd_func0__sub1__sub0(const char *filename, GUID *out)
{
	char buf[256];

	memset(out, 0, sizeof(GUID));

	FILE *fil = fopen(filename, "r");

	if ( fil )
	{
		fgets(buf, 256, fil);
		fclose(fil);

		char *eol = strpbrk(buf, "\n;");
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

		if ( a4 & 1 )
			node->sort_id |= 0x8000u;

		if ( a4 & 8 )
			node->sort_id |= 0x4000u;

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




HRESULT __stdcall enumdevice_callback(LPGUID lpGuid, LPSTR lpDeviceDescription, LPSTR lpDeviceName, LPD3DDEVICEDESC hw, LPD3DDEVICEDESC sw, LPVOID a6)
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
	else
	{
		if ( !(device->dev_descr.dpcTriCaps.dwShadeCaps & (D3DPSHADECAPS_ALPHAFLATSTIPPLED | D3DPSHADECAPS_ALPHAFLATSTIPPLED)) ||
				(device->dev_descr.dpcTriCaps.dwShadeCaps & D3DPSHADECAPS_ALPHAFLATBLEND) )
		{
			if ( !(device->dev_descr.dpcTriCaps.dwSrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) || !(device->dev_descr.dpcTriCaps.dwDestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) )
			{
				log_d3dlog("enum devices: skip, no alpha, no stipple...\n");
				return 1;
			}

			device->can_srcblend = 1;
			device->can_destblend = 0;

			log_d3dlog("enum devices: can do srcblend = srcalpha; destblend = invsrcalpha\n");
		}
		else
		{
			device->can_stippling = 1;

			log_d3dlog("enum devices: can do alpha stippling\n");
		}
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

signed int __stdcall DDRAW_ENUMERATE_CallBack(GUID *lpGUID, LPSTR DRIVER_NAME, LPSTR DRIVER_DESCR, void *a4)
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



int __stdcall gfx_modes_callback(LPDDSURFACEDESC a1, LPVOID lpContext)
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
				if ( devs->bits == 16 )
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

	FILE *fil = fopen(file, "r");

	mode_node *node = NULL;

	if ( fil )
	{
		if ( fgets(buf, 128, fil) )
		{
			char *eol = strpbrk(buf, "\n");
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
		fclose(fil);
	}

	if ( !node )
		node = sub_41F68C();

	return node;
}

int sub_42AC78(__NC_STACK_windd *a1)
{
	return dd_params.selected_device.unk2_def_0 || a1->field_30;
}

void sub_42D410(__NC_STACK_windd *obj, int a2, int a3)
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
					cur = LoadCursor(obj->cursor, "Pointer");
					break;
				case 2:
					cur = LoadCursor(obj->cursor, "Cancel");
					break;
				case 3:
					cur = LoadCursor(obj->cursor, "Select");
					break;
				case 4:
					cur = LoadCursor(obj->cursor, "Attack");
					break;
				case 5:
					cur = LoadCursor(obj->cursor, "Goto");
					break;
				case 6:
					cur = LoadCursor(obj->cursor, "Disk");
					break;
				case 7:
					cur = LoadCursor(obj->cursor, "New");
					break;
				case 8:
					cur = LoadCursor(obj->cursor, "Add");
					break;
				case 9:
					cur = LoadCursor(obj->cursor, "Control");
					break;
				case 10:
					cur = LoadCursor(obj->cursor, "Beam");
					break;
				case 11:
					cur = LoadCursor(obj->cursor, "Build");
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


void sub_42A7BC(__NC_STACK_windd *obj)
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
						h = obj->height >> 1;
						w = obj->width >> 1;
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

void sub_42A640(__NC_STACK_windd *obj)
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

void wdd_Kill3D(__NC_STACK_windd *obj)
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

void sub_42BD38(__NC_STACK_windd *obj)
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


long int __stdcall sub_42A978(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	printf("FIX ME %s\n","sub_42A978");
	__NC_STACK_windd *v4;
	__NC_STACK_windd *v7;
	__NC_STACK_windd *v8;
	IDirectDrawSurface *v9;
	int result;
	int v11;
	struct tagRECT rc;
	struct tagRECT Rect;
	struct tagPOINT Point;

	v4 = (__NC_STACK_windd *)GetClassLong(hWnd, 0);
	__NC_STACK_windd *obj = (__NC_STACK_windd *)GetClassLong(hWnd, 0);
	v7 = v4;
	v8 = v4;
	if ( Msg < WM_QUIT ) //0x12
	{
		if ( Msg >= WM_ACTIVATE ) //0x0006
		{
			if ( Msg <= WM_ACTIVATE ) //0x0006
			{
				if ( v4 )
					sub_42D410(v4, 1, 1);
				return DefWindowProcA(hWnd, Msg, wParam, lParam);
			}
			if ( Msg == WM_PAINT ) //0x000f
			{
				BeginPaint(hWnd, (LPPAINTSTRUCT)&v11);
				if ( v7 && v7->hwnd && v7->primary_surf && v7->back_surf )
				{
					if ( dword_514F24 )
					{
						sub_42A7BC(v7);
					}
					else
					{
						sub_42A640(v7);
						if ( v7->field_50 & 1 )
						{
							GetClientRect(v7->hwnd, &rc);
							Point.x = 0;
							Point.y = 0;
							ClientToScreen(v8->hwnd, &Point);
							OffsetRect(&rc, Point.x, Point.y);
							v8->primary_surf->Blt(&rc, v8->back_surf, 0, 0x1000000, 0);
						}
						else if ( !dword_514EFC )
						{
							v7->primary_surf->Blt(
								(LPRECT)dword_514EFC,
								v7->back_surf,
								(LPRECT)dword_514EFC,
								0x1000000,
								(LPDDBLTFX)dword_514EFC);
							EndPaint(hWnd, (const PAINTSTRUCT *)&v11);
							return DefWindowProcA(hWnd, Msg, wParam, lParam);
						}
					}
				}
				EndPaint(hWnd, (const PAINTSTRUCT *)&v11);
			}
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		}
		if ( Msg <= WM_CREATE || Msg != WM_DESTROY )   // 0x0001   0x0002
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		if ( v4 )
		{
			sub_42BD38(v4);
			v7->hwnd = 0;
			PostQuitMessage(0);
		}
		if ( dword_514F1C )
		{
			ImmAssociateContext(hWnd, dword_514F1C);
			dword_514F1C = 0;
		}
		result = 0;
	}
	else // Msg >= WM_QUIT  //////0x12
	{
		if ( Msg == WM_QUIT ) ////0x12
			return DefWindowProcA(hWnd, Msg, wParam, lParam);

		if ( Msg == WM_SYSCOMMAND ) //// 0x0112
		{
			if ( wParam == 61696 )
				return 0;
			if ( wParam == 61760 )
				return 0;
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		}

		if ( Msg == WM_QUERYNEWPALETTE || (Msg == WM_PALETTECHANGED && (HWND)wParam != obj->hwnd) ) ////0x030F   0x0311
		{
			if ( obj )
				if ( obj->ddrawPal )
					obj->primary_surf->SetPalette(v4->ddrawPal);

			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		}


		if ( Msg < WM_ERASEBKGND )  ////0x0014
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		if ( Msg > WM_ERASEBKGND )  //// 0x0014
		{
			if ( Msg == WM_ACTIVATEAPP )  ////0x001c
			{
				if ( wParam == 1 )
				{
					GetWindowRect(hWnd, &Rect);
					ClipCursor(&Rect);
					if ( v7 )
						sub_42A640(v7);
				}
				else
				{
					ClipCursor(0);
					if ( v7 )
					{
						if ( v7->surface_locked_surfaceData )
						{
							v7->surface_locked_pitch = 0;
							v9 = v7->back_surf;
							v7->surface_locked_surfaceData = 0;
							v9->Unlock(0);
						}
					}
				}
				if ( v8 )
					sub_42D410(v8, 1, 1);
			}
			return DefWindowProcA(hWnd, Msg, wParam, lParam);
		}
		result = 1;
	}
	return result;
}

int createWindow(__NC_STACK_windd *obj, HINSTANCE hInstance, int cmdShow, int width, int height)
{
	HICON big256 = LoadIconA(hInstance, "Big256");
	HCURSOR hCursor = LoadCursorA(hInstance, "Pointer");

	if ( ghWnd )
	{
		obj->hwnd = ghWnd;

		int w, h;
		if ( obj->field_50 & 1 )
		{
			SetWindowLong(ghWnd, GCL_HMODULE, WS_OVERLAPPEDWINDOW);
			h = height;
			w = width;
		}
		else
		{
			SetWindowLong(ghWnd, GCL_HMODULE, 0x80080000);
			SetWindowLong(ghWnd, GCL_CBCLSEXTRA, 8);
			h = GetSystemMetrics(SM_CYSCREEN);
			w = GetSystemMetrics(SM_CXSCREEN);
		}
		SetWindowPos(ghWnd, 0, 0, 0, w, h, SWP_SHOWWINDOW);
		SetClassLongA(ghWnd, 0, (LONG)obj);
		return 1;
	}
	else
	{
		WNDCLASS winClass;

		winClass.hInstance = hInstance;
		winClass.style = CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
		winClass.lpfnWndProc = sub_42A978;
		winClass.cbClsExtra = 4;
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

		if ( obj->field_50 & 1 )
		{
			h = height;
			w = width;
			wndstyle = WS_OVERLAPPEDWINDOW;
			dwExStyle = 0;
		}
		else
		{
			h = GetSystemMetrics(SM_CYSCREEN);
			w = GetSystemMetrics(SM_CXSCREEN);
			wndstyle = WS_POPUP | WS_SYSMENU;
			dwExStyle = WS_EX_TOPMOST;
		}

		obj->hwnd = CreateWindowEx(dwExStyle, lpclassname, windname, wndstyle, 0, 0, w, h, 0, 0, hInstance, 0);

		if ( obj->hwnd )
		{
			dword_514F1C = ImmAssociateContext(obj->hwnd, 0);
			ShowWindow(obj->hwnd, cmdShow);
			UpdateWindow(obj->hwnd);
			SetCursor(hCursor);

			ghWnd = obj->hwnd;
			SetClassLong(obj->hwnd, 0, (LONG)obj);
			return 1;
		}
	}
	return 0;
}

int initSurfacesAndClipper(__NC_STACK_windd *a1, int a2, int a3, int a4)
{
	printf("MAKE ME %s\n","initSurfacesAndClipper");
	return 0;
}

int windd_func0__sub2__sub2(__NC_STACK_windd *a1, int w, int h, int a4)
{
	printf("MAKE ME %s\n","windd_func0__sub2__sub2");
	return 0;
}

int windd_func0__sub2__sub3(__NC_STACK_windd *a1, int w, int h, int a4)
{
	printf("MAKE ME %s\n","windd_func0__sub2__sub3");
	return 0;
}

int windd_func0__sub2__sub0(__NC_STACK_windd *a1, LPPALETTEENTRY pal, int w, int h)
{
	printf("MAKE ME %s\n","windd_func0__sub2__sub0");
	return 0;
}

int wdd_Create2DFullEnv(__NC_STACK_windd *obj, LPPALETTEENTRY pal, int width, int height)
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

int windd_func0__sub2__sub4(IDirectDraw *a1, IDirectDrawSurface *a2, const char *a4)
{
	printf("MAKE ME %s\n","windd_func0__sub2__sub4");
	return 0;
}

int windd_func0__sub2(__NC_STACK_windd *obj, BYTE *palette, int width, unsigned int height, int bits)
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
				BYTE *cur = palette + i * 3;
				tmpPal[i].peRed = cur[0];
				tmpPal[i].peGreen = cur[1];
				tmpPal[i].peBlue = cur[2];
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
					if ( !initSurfacesAndClipper(obj, width, height, bits) )
						return 0;
				}
				else if ( !windd_func0__sub2__sub2(obj, width, height, bits) )
				{
					return 0;
				}

				log_d3dlog("->     after wdd_Create3DFull/WinEnv()\n");

				if ( !windd_func0__sub2__sub3(obj, width, height, bits) )
					return 0;

				log_d3dlog("->     after wdd_Create3D()\n");
			}
			else
			{
				if ( obj->field_50 & 1 )
				{
					if ( !windd_func0__sub2__sub0(obj, tmpPal, width, height) )
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

NC_STACK_windd * windd_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
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

	if ( windd_func0__sub1(windd_keys[6].value.val) )
	{
		int v7 = find_id_in_stack_def_val(0x80004000, 0, stak);
		BYTE *pal = (BYTE *)find_id_in_stack_def_val(0x80002007, 0, stak);

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
			tmp[0].id = 0x80002002;
			tmp[0].value = (unsigned int)picked->width / 2;
			tmp[1].id = 0x80002003;
			tmp[1].value = (unsigned int)picked->height / 2;
		}
		else
		{
			tmp[0].id = 0x80002002;
			tmp[0].value = (unsigned int)picked->width;
			tmp[1].id = 0x80002003;
			tmp[1].value = (unsigned int)picked->height;
		}
		tmp[2].id = 0x80002005;
		tmp[2].value = 1;
		tmp[3].id = 0x80002006;
		tmp[3].value = 1;
		tmp[4].id = 2;
		tmp[4].value = (size_t)stak;

		NC_STACK_windd *clss = (NC_STACK_windd *)call_parent(zis, obj, 0, tmp);

		if ( clss )
		{
			__NC_STACK_windd *windd = &clss->stack__windd;

			windd->field_30 = 0;
			windd->sort_id = picked->sort_id;
			windd->movie_player = windd_keys[2].value.val;
			windd->disable_lowres = windd_keys[5].value.val;
			windd->txt16bit = txt16bit_def;
			windd->use_simple_d3d = drawprim_def;
			windd->export_window_mode = export_window_mode;

			windd->field_50 |= 1; ////HACK

			if ( picked->field_94 & 1 )
				windd->field_50 |= 1;
			if ( picked->field_94 & 4 )
				windd->field_50 |= 4;
			if ( picked->field_94 & 8 )
				windd->field_50 |= 8;
			if ( picked->field_94 & 0x10 )
				windd->field_50 |= 0x10;

			if ( windd_func0__sub2(windd, pal, picked->width, picked->height, picked->bits) )
			{
				FILE *fil = fopen("env/vid.def", "w");
				if ( fil )
				{
					fprintf(fil, "%s\n", picked->name);
					fclose(fil);
				}

				call_vtbl(clss, 3, 0x80001002, &windd->field_54______rsrc_field4, 0);
				return clss;
			}
			else
			{
				log_d3dlog("wdd_InitDDrawStuff() failed.\n");

				call_method(clss, 1);
				return NULL;
			}
		}
	}
	else
		return NULL;
	return NULL;
}

size_t windd_func1(NC_STACK_windd *obj, class_stru *zis, stack_vals *stak)
{
	return call_parent(zis, obj, 1, stak);
}

void windd_func2(NC_STACK_windd *obj, class_stru *zis, stack_vals *stak)
{
	call_parent(zis, obj, 2, stak);
}

void windd_func3(NC_STACK_windd *obj, class_stru *zis, stack_vals *stak)
{

	//__NC_STACK_windd *internal = &obj->stack__windd;

	stack_vals *stk = stak;

	while ( 1 )
	{
		if (stk->id == 0)
			break;
		else if (stk->id == 2)
		{
			stk = (stack_vals *)stk->value;
		}
		else if ( stk->id == 3 )
		{
			stk += stk->value;
			////a2++; ////BUGFIX?
		}
		else
		{
			stk++;
		}
	}
	call_parent(zis, obj, 3, stak);
}


size_t windd_func192(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func193(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func194(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func195(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func196(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func197(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func198(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func199(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func200(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func201(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func202(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func203(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func204(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func205(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func206(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func209(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func213(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func214(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func215(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func216(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func218(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func219(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func256(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func257(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func258(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func259(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func260(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func261(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func262(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func263(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func320(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func321(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func322(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func323(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func324(void *, class_stru *, stack_vals *)
{
	return 0;
}

size_t windd_func325(void *, class_stru *, stack_vals *)
{
	return 0;
}


class_return windd_class_descr;

void windd_nullsub(void *, class_stru *, stack_vals *)
{
	;
}

class_return * class_set_windd(int, ...)
{
	memset(windd_funcs, 0, sizeof(CLASSFUNC) * 1024);

	windd_funcs[0] = (CLASSFUNC)windd_func0;
	windd_funcs[1] = (CLASSFUNC)windd_func1;
	windd_funcs[2] = (CLASSFUNC)windd_func2;
	windd_funcs[3] = (CLASSFUNC)windd_func3;
	windd_funcs[192] = (CLASSFUNC)windd_func192;
	windd_funcs[193] = (CLASSFUNC)windd_func193;
	windd_funcs[194] = (CLASSFUNC)windd_func194;
	windd_funcs[195] = (CLASSFUNC)windd_func195;
	windd_funcs[196] = (CLASSFUNC)windd_func196;
	windd_funcs[197] = (CLASSFUNC)windd_func197;
	windd_funcs[198] = (CLASSFUNC)windd_func198;
	windd_funcs[199] = (CLASSFUNC)windd_func199;
	windd_funcs[200] = (CLASSFUNC)windd_func200;
	windd_funcs[201] = (CLASSFUNC)windd_func201;
	windd_funcs[202] = (CLASSFUNC)windd_func202;
	windd_funcs[203] = (CLASSFUNC)windd_func203;
	windd_funcs[204] = (CLASSFUNC)windd_func204;
	windd_funcs[205] = (CLASSFUNC)windd_func205;
	windd_funcs[206] = (CLASSFUNC)windd_func206;

	windd_funcs[209] = (CLASSFUNC)windd_func209;

	windd_funcs[213] = (CLASSFUNC)windd_func213;
	windd_funcs[214] = (CLASSFUNC)windd_func214;
	windd_funcs[215] = (CLASSFUNC)windd_func215;
	windd_funcs[216] = (CLASSFUNC)windd_func216;

	windd_funcs[218] = (CLASSFUNC)windd_func218;
	windd_funcs[219] = (CLASSFUNC)windd_func219;


	windd_funcs[256] = (CLASSFUNC)windd_func256;
	windd_funcs[257] = (CLASSFUNC)windd_func257;
	windd_funcs[258] = (CLASSFUNC)windd_func258;
	windd_funcs[259] = (CLASSFUNC)windd_func259;
	windd_funcs[260] = (CLASSFUNC)windd_func260;
	windd_funcs[261] = (CLASSFUNC)windd_func261;
	windd_funcs[262] = (CLASSFUNC)windd_func262;
	windd_funcs[263] = (CLASSFUNC)windd_func263;

	windd_funcs[320] = (CLASSFUNC)windd_func320;
	windd_funcs[321] = (CLASSFUNC)windd_func321;
	windd_funcs[322] = (CLASSFUNC)windd_func322;
	windd_funcs[323] = (CLASSFUNC)windd_func323;
	windd_funcs[324] = (CLASSFUNC)windd_func324;
	windd_funcs[325] = (CLASSFUNC)windd_func325;

	windd_class_descr.parent = "display.class";

	windd_class_descr.vtbl = windd_funcs;
	////windd_class_descr.varSize = sizeof(__NC_STACK_windd);
	windd_class_descr.varSize = sizeof(NC_STACK_windd) - offsetof(NC_STACK_windd, stack__windd); //// HACK
	windd_class_descr.field_A = 0;
	return &windd_class_descr;
}
