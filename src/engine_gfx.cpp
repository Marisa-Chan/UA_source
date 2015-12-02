#include "includes.h"
#include "engine_gfx.h"
#include "utils.h"

stored_functions *classvtbl_get_gfxEngine();

int gfxEngine__init(unsigned int, ...);
void gfxEngine__deinit();
void gfxEngine__setter(unsigned int, ...);
void gfxEngine__getter(unsigned int, ...);


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

void sub_4231FC(void *dat)
{
	call_method(win3d_class_pointer, 206, dat);
}


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


int gfxEngine__init(unsigned int, ...)
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
	if ( win3d_class_pointer )
	{
		delete_class_obj(win3d_class_pointer);
		win3d_class_pointer = 0;
	}
}

void gfxEngine__setter(unsigned int a1, ...)
{
	stack_vals vals[128];

	if (a1 != 0)
	{
		va_list va;
		va_start(va, a1);

		va_to_arr(vals, 128, a1, va);

		va_end(va);
	}

	if ( find_id_in_stack2(0x80003005, vals) )
		call_method(win3d_class_pointer, 259);

	if ( find_id_in_stack2(0x80003006, vals) )
		call_method(win3d_class_pointer, 260);

	size_t tmp = find_id_in_stack_def_val(0x80003009, 0, vals);
	if ( tmp )
		call_vtbl(win3d_class_pointer, 2, 0x80003003, tmp, 0);

	tmp = find_id_in_stack_def_val(0x8000300A, 0, vals);
	if ( tmp )
		call_vtbl(win3d_class_pointer, 2, 0x80003002, tmp, 0);

	stack_vals *v4 = find_id_in_stack2(0x80003007, vals);
	if ( v4 )
	{
		void *screen_palette;
		call_vtbl( win3d_class_pointer, 3, 0x80002007, &screen_palette, 0); // Get palette

		char palette_copy[256 * 3];
		if ( screen_palette )
			memcpy(palette_copy, screen_palette, 256 * 3); // Copy palette

		call_method(win3d_class_pointer, 258);

		delete_class_obj(win3d_class_pointer);

		if ( sub_422CE8(gfx_display, gfx_display2, (void *)v4->value) )
		{
			call_method(win3d_class_pointer, 257);

			////call_vtbl(win3d_class_pointer, 2,  0x80002007,	screen_palette,	0); //// BUG?
			call_vtbl(win3d_class_pointer, 2,  0x80002007,	palette_copy,	0); //// FIX?
		}
	}
}

void gfxEngine__getter(unsigned int a1, ...)
{
	stack_vals vals[128];

	if (a1 != 0)
	{
		va_list va;
		va_start(va, a1);
		va_to_arr(vals, 128, a1, va);
		va_end(va);
	}

	void *tmp = (void *)find_id_in_stack_def_val(0x80003003, 0, vals); // get screen width
	if ( tmp )
		call_vtbl(win3d_class_pointer, 3, 0x80002002, tmp, 0); // bitmap_func3

	tmp = (void *)find_id_in_stack_def_val(0x80003004, 0, vals); // get screen height
	if ( tmp )
		call_vtbl(win3d_class_pointer, 3, 0x80002003, tmp, 0); // bitmap_func3

	tmp = (void *)find_id_in_stack_def_val(0x80003007, 0, vals); // get window params
	if ( tmp )
		call_vtbl(win3d_class_pointer, 3, 0x80004001, tmp, 0);

	tmp = (void *)find_id_in_stack_def_val(0x8000300D, 0, vals); // get win3d pointer
	if ( tmp )
		*(NC_STACK_class **)tmp = win3d_class_pointer;

	tmp = (void *)find_id_in_stack_def_val(0x8000300B, 0, vals); // get display stack internal or palette?
	if ( tmp )
		call_vtbl(win3d_class_pointer, 3, 0x80002007, tmp, 0);

	tmp = (void *)find_id_in_stack_def_val(0x8000300C, 0, vals); // get bitmap buffer?
	if ( tmp )
	{
		void **bitmap = NULL;
		call_vtbl(win3d_class_pointer, 3, 0x80002000, &bitmap, 0);
		tmp = *bitmap;
	}
}


tiles_stru * sub_42324C(int id)
{
	rstr_207_arg arg207;

	arg207.tiles = 0;
	arg207.id = id;

	call_method(win3d_class_pointer, 208, &arg207);
	return arg207.tiles;
}

size_t sub_423288(w3d_a209 *arg)
{
	w3d_a209 arg209;
	arg209 = *arg;
	return call_method(win3d_class_pointer, 209, &arg209);
}

void gfx_set_tileset(tiles_stru *a1, int id)
{
	rstr_207_arg arg;

	arg.tiles = a1;
	arg.id = id;

	call_method(win3d_class_pointer, 207, &arg);
}
