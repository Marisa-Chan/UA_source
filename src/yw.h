#ifndef YW_H_INCLUDED
#define YW_H_INCLUDED

#include "engine_gfx.h"


struct _NC_STACK_ypaworld
{
	tiles_stru *tiles[64];

	int16_t screen_width;
	int16_t screen_height;

	int font_default_h;
	int font_default_w__a;
	int font_yscrl_bkg_w;
	int font_xscrl_h;
	int font_default_w__b;
	int field_1a38;
	int font_yscrl_h;
	int icon_order__w;
	int icon_order__h;
	int icon_help__w;
	int icon_help__h;
	int icon_energy__h;
	int icon0___h;
};

void load_fonts_and_icons(_NC_STACK_ypaworld *yw);

#endif
