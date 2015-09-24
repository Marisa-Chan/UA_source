#include "includes.h"
#include "engine_gfx.h"

stored_functions *classvtbl_get_gfxEngine();

int gfxEngine__init(int, ...);
void gfxEngine__deinit();
void gfxEngine__setter(int, ...);
void gfxEngine__getter(int, ...);


stored_functions_engine gfx_engine_vtbl = {gfxEngine__init, gfxEngine__deinit, gfxEngine__setter, gfxEngine__getter};



class_stored gfx_engine_off (NULL, NULL, "MC2engines:gfx.engine", classvtbl_get_gfxEngine);

stored_functions *classvtbl_get_gfxEngine()
{
	return (stored_functions *)&gfx_engine_vtbl;
}

char gfx_palette[128];
char gfx_display[128];
char gfx_display2[128];

key_value_stru gfx_keys[6] = {{"gfx.mode", KEY_TYPE_DIGIT, 0},
	{"gfx.xres", KEY_TYPE_DIGIT, 0},
	{"gfx.yres", KEY_TYPE_DIGIT, 0},
	{"gfx.palette", KEY_TYPE_STRING1, gfx_palette},
	{"gfx.display", KEY_TYPE_STRING1, gfx_display},
	{"gfx.display2", KEY_TYPE_STRING1, gfx_display2}
};


NC_STACK_class *win3d_class_pointer;


int sub_422CE8(const char *display, const char *display2, void *a5)
{
	char buf[33];

	if ( *display )
	{
		strcpy(buf, "drivers/gfx/");
		strcat(buf, display);

////		win3d_class_pointer = (NC_STACK_win3d *)init_get_class(buf,  0x80001000, "display",  0x80001001,  2,  0x80004000,  a5,  0);
		win3d_class_pointer = init_get_class(buf,  0x80001000, "display",  0x80001001,  2,  0x80004000,  a5,  0);

		if ( !win3d_class_pointer )
		{
			if ( *display2 )
			{
				strcpy(buf, "drivers/gfx/");
				strcat(buf, display2);
////		win3d_class_pointer = (NC_STACK_win3d *)init_get_class(buf,  0x80001000, "display",  0x80001001,  2,  0x80004000,  a5,  0);
				win3d_class_pointer = init_get_class(buf,  0x80001000, "display",  0x80001001,  2,  0x80004000,  a5,  0);
			}
		}
		if ( !win3d_class_pointer )
			ypa_log_out("gfx.engine: display driver init failed!\n");
	}
	else
	{
		ypa_log_out("gfx.engine: no display driver name given!\n");
	}
	return win3d_class_pointer != NULL;
}

int win3d__load_palette_from_ilbm(const char *palette)
{
    NC_STACK_class *ilbm = init_get_class("ilbm.class", 0x80001000, palette, 0x80002006, 1, 0);

    if (!ilbm)
        return 0;

    void *bitmap_palette;
    call_vtbl(ilbm, 3, 0x80002007, &bitmap_palette, 0); //Getter
    call_vtbl(win3d_class_pointer, 2, 0x80002007, bitmap_palette, 0); //Setter

    delete_class_obj(ilbm);

    return 1;
}


int gfxEngine__init(int, ...)
{
	memset(gfx_palette, 0, 128);
	memset(gfx_display, 0, 128);
	memset(gfx_display2, 0, 128);
	get_keyvalue_from_ini(0, gfx_keys, 6);

	if ( sub_422CE8((const char *)gfx_keys[4].value.pval, (const char *)gfx_keys[5].value.pval, 0) )
	{
        win3d__load_palette_from_ilbm((const char *)gfx_keys[3].value.pval);
		return 1;
	}
	return 0;
}

void gfxEngine__deinit()
{
////TODO
}

void gfxEngine__setter(int, ...)
{
////TODO
}

void gfxEngine__getter(int, ...)
{
////TODO
}
