#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"


void delete_tile_stuct(tiles_stru *tileset)
{
	if ( tileset )
	{
		if ( tileset->font_image )
			delete_class_obj(tileset->font_image);
		if ( tileset->chars )
			nc_FreeMem(tileset->chars);
		nc_FreeMem(tileset);
	}
}

tiles_stru * yw_LoadFont(_NC_STACK_ypaworld *yw, const char *fontname)
{
	char filename[256];

	strcpy(filename, "rsrc:");

	if ( yw->screen_width >= 512 )
		strcat(filename, "hfonts/");
	else
		strcat(filename, "fonts/");

	strcat(filename, fontname);

	FILE *fil = FOpen(filename, "r");
	if ( !fil )
		return NULL;

	tiles_stru *tileset = (tiles_stru *)AllocVec(sizeof(tiles_stru), 65537);

	if ( tileset )
	{
		tile_xy *v12 = (tile_xy *)AllocVec(sizeof(tile_xy) * 256, 65537);
		tileset->chars = v12;
		if ( !v12 )
		{
			delete_tile_stuct(tileset);
			tileset = NULL;
		}
	}

	if ( tileset )
	{
		char buf[128];

		if ( !fgets(buf, 128, fil) )
		{
			ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
			if ( tileset )
			{
				if ( tileset->font_image )
					delete_class_obj(tileset->font_image);
				if ( tileset->chars )
					nc_FreeMem(tileset->chars);
				nc_FreeMem(tileset);
			}
			return NULL;
		}

		char *chrpos = buf;

		while (*chrpos)
		{
			if (*chrpos == '\n')
			{
				*chrpos = 0;
				break;
			}
			chrpos++;
		}

		char *bitmap_name = strtok(buf, " \t");

		if ( !bitmap_name )
		{
			if ( tileset )
			{
				if ( tileset->font_image )
					delete_class_obj(tileset->font_image);
				if ( tileset->chars )
					nc_FreeMem(tileset->chars);
				nc_FreeMem(tileset);
			}
			ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
			return NULL;
		}

		tileset->font_image = init_get_class("ilbm.class", 0x80001000, bitmap_name, 0x80002008, 1, 0x80002009, 1, 0);
		if ( !tileset->font_image )
		{
			if ( tileset->chars )
				nc_FreeMem(tileset->chars);
			nc_FreeMem(tileset);
			ypa_log_out("yw_LoadFont(): font %s, couldn't load fontpage %s.\n", fontname, bitmap_name);
			return NULL;
		}

		call_vtbl(tileset->font_image, 3, 0x80002000, &tileset->field_4, 0);
		tileset->field_8 = tileset->field_4->buffer;

		char *fntHeight = strtok(0, " \t");
		if ( !fntHeight )
		{
			ypa_log_out("yw_LoadFont(): font %s, font definition file corrupt.\n", fontname);
			if ( tileset )
			{
				if ( tileset->font_image )
					delete_class_obj(tileset->font_image);
				if ( tileset->chars )
					nc_FreeMem(tileset->chars);
				nc_FreeMem(tileset);
			}
			return NULL;
		}

		tileset->font_height = atoi(fntHeight);

		while ( fgets(buf, 128, fil) )
		{
			chrpos = buf;

			while (*chrpos)
			{
				if (*chrpos == '\n')
				{
					*chrpos = 0;
					break;
				}
				chrpos++;
			}

			int go = 0;
			uint8_t chr;
			char *nxt;

			if ( buf[0] == ' ' )
			{
				chr = (uint8_t)' ';
				nxt = buf + 1;
				go = 1;
			}
			else
			{
				char *chrname = strtok(buf, " \t");
				if (chrname)
				{
					if ( *chrname == '#' && chrname[1] )
						chr = atoi(chrname + 1);
					else
						chr = *(uint8_t *)chrname;

					nxt = 0;
					go = 1;
				}
			}

			if ( go )
			{
				char *str_x = strtok(nxt, " \t");
				if ( str_x )
				{
					int xpos = atoi(str_x);

					char *str_y = strtok(0, " \t");
					if ( str_y )
					{
						int ypos = atoi(str_y);

						char *str_w = strtok(0, " \t");
						if ( str_w )
						{
							tileset->chars[chr].byteoff = tileset->field_4->width * ypos + xpos;
							tileset->chars[chr].width = atoi(str_w);
						}
					}
				}
			}
		}

		int v33 = 0;

		for (int i = 0; i < 256; i++)
		{
			if (tileset->chars[i].width)
			{
				v33 = i;
				break;
			}
		}

		for (int i = 0; i < 256; i++)
		{
			if (tileset->chars[i].width == 0)
			{
				tileset->chars[i] = tileset->chars[v33];
			}
		}
	}
	FClose(fil);
	return tileset;
}


tiles_stru * yw_LoadTileSet(const char *filename, int chr_width, int font_height, int tile_width, int tile_height, int columns, int rows, int x_off, int y_off)
{
	tiles_stru *tileset = (tiles_stru *)AllocVec(sizeof(tiles_stru), 65537);

	if ( tileset )
	{
		tile_xy *v13 = (tile_xy *)AllocVec(sizeof(tile_xy) * 256, 65537);
		tileset->chars = v13;
		if ( !v13 )
		{
			delete_tile_stuct(tileset);
			tileset = NULL;
		}
	}

	if ( tileset )
	{
		tileset->font_image = init_get_class("ilbm.class", 0x80001000, filename, 0x80002008, 1, 0x80002009, 1, 0);
		if ( tileset->font_image )
		{
			call_vtbl(tileset->font_image, 3, 0x80002000, &tileset->field_4, 0);

			tileset->field_8 = tileset->field_4->buffer;
			tileset->font_height = font_height;

			int id = 0;
			int y = y_off;
			for (int j = 0; j < rows; j++)
			{
				for (int i = 0; i < columns; i++ )
				{
					bitmap_intern *bitm = tileset->field_4;

					int x_pos = x_off + i * tile_width;
					int y_pos = y;

					if ( chr_width + (x_off + i * tile_width) > bitm->width )
						x_pos = bitm->width - chr_width;

					if ( font_height + y > bitm->height )
						y_pos = bitm->height - font_height;

					tileset->chars[id].byteoff = tileset->field_4->width * y_pos + x_pos;
					tileset->chars[id].width = chr_width;

					id++;
				}
				y += tile_height;
			}
		}
		else
		{
			if ( tileset->chars )
				nc_FreeMem(tileset->chars);
			nc_FreeMem(tileset);
			tileset = NULL;
		}
	}
	return tileset;
}

void load_fonts_and_icons(_NC_STACK_ypaworld *yw)
{
	const char *font_names[32] =
	{
		"default.font", //0
		"maprobo.font",
		"icondown.font",
		"defblue.font",
		"defwhite.font",
		"menublue.font", //5
		"menugray.font",
		"menuw.font",
		"gadget.font",
		"menudown.font",
		"mapbtns.font", //10
		"maphorz.font",
		"mapvert.font",
		"mapvert1.font",
		"gauge4.font",
		"tracy.font", //15
		"mapcur4.font",
		"mapcur8.font",
		"mapcur16.font",
		"mapcur32.font",
		"mapcur64.font", //20
		"icon_nb.font",
		"icon_pb.font",
		"icon_db.font",
		"icon_ns.font",
		"icon_ps.font", //25
		"type_nb.font",
		"type_pb.font",
		"type_ns.font",
		"type_ps.font",
		"energy.font", //30
		"ltracy.font"
	};

	for (int i = 0; i < 32; i++)
	{
		yw->tiles[i] = yw_LoadFont(yw, font_names[i]);
		if ( !yw->tiles[i] )
		{
			ypa_log_out("Could not load font (%s)", font_names[i]);
			return;
		}

		gfx_set_tileset(yw->tiles[i], i);
	}


	yw->tiles[40] = yw_LoadTileSet("lego16.ilbm", 16, 16, 16, 16, 16, 16, 0, 0);
	if ( !yw->tiles[40] )
		return;
	gfx_set_tileset(yw->tiles[40], 40);

	yw->tiles[41] = yw_LoadTileSet("lego8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
	if ( !yw->tiles[41] )
		return;
	gfx_set_tileset(yw->tiles[41], 41);

	yw->tiles[42] = yw_LoadTileSet("lego4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
	if ( !yw->tiles[42] )
		return;
	gfx_set_tileset(yw->tiles[42], 42);

	yw->tiles[43] = yw_LoadTileSet("sec4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
	if ( !yw->tiles[43] )
		return;
	gfx_set_tileset(yw->tiles[43], 43);

	yw->tiles[44] = yw_LoadTileSet("sec8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
	if ( !yw->tiles[44] )
		return;
	gfx_set_tileset(yw->tiles[44], 44);

	for (int i = 0; i < 5; i++)
	{
		int id = 50 + i;

		int v12 = 4 << i;
		int v13 = 4 << i;
		int v1, v2, a10;

		switch ( i )
		{
		case 0:
			v1 = 4;
			v2 = 504;
			a10 = 64;
			break;
		case 1:
			v1 = 8;
			v2 = 432;
			a10 = 64;
			break;
		case 2:
			v1 = 16;
			v2 = 288;
			a10 = 64;
			break;
		case 3:
			v1 = 32;
			v2 = 0;
			a10 = 64;
			break;
		case 4:
			v1 = 64;
			v2 = 0;
			a10 = 0;
			break;

		default:
			break;
		}

		tiles_stru *v14 = yw_LoadTileSet("mapmisc.ilbm",v12, v13, v1, 0, 9, 1, v2, a10);

		if ( !v14 )
			return;

		v14->chars[9].byteoff = v14->chars[8].byteoff;
		v14->chars[9].width = v14->chars[8].width;
		v14->chars[8].byteoff = v14->chars[0].byteoff;
		v14->chars[8].width = v14->chars[0].width;

		yw->tiles[id] = v14;
		gfx_set_tileset(v14, id);


	}

	yw->tiles[59] = yw_LoadFont(yw, "mapvhcl3.font");
	if ( !yw->tiles[59] )
		return;
	gfx_set_tileset(yw->tiles[59], 59);

	yw->tiles[60] = yw_LoadFont(yw, "mapvhcl5.font");
	if ( !yw->tiles[60] )
		return;
	gfx_set_tileset(yw->tiles[60], 60);

	yw->tiles[61] = yw_LoadFont(yw, "mapvhcl7.font");
	if ( !yw->tiles[61] )
		return;
	gfx_set_tileset(yw->tiles[61], 61);

	yw->tiles[62] = yw_LoadFont(yw, "mapvhcl9.font");
	if ( !yw->tiles[62] )
		return;
	gfx_set_tileset(yw->tiles[62], 62);

	yw->font_default_h = yw->tiles[0]->font_height;
	yw->font_default_w__a = yw->tiles[0]->chars[97].width; // a
	yw->font_default_w__b = yw->tiles[0]->chars[98].width; // b

	yw->font_yscrl_bkg_w = yw->tiles[12]->chars[66].width; // B (Y-Scroller, Background-Inners)
	yw->font_xscrl_h = yw->tiles[11]->font_height;

	yw->field_1a38 = 2;

	yw->font_yscrl_h = yw->tiles[12]->font_height;

	yw->icon_order__w = yw->tiles[21]->chars[65].width; // A (Mode:      ORDER)
	yw->icon_order__h = yw->tiles[21]->font_height;

	yw->icon_help__w = yw->tiles[24]->chars[65].width; // A (NEU: DER HILFEBUTTTON)
	yw->icon_help__h = yw->tiles[24]->font_height;

	yw->icon_energy__h = yw->tiles[30]->font_height; // ENERGIE height
	yw->icon0___h = yw->icon_order__h;
}



void yw_InitLevelNet__sub0(_NC_STACK_ypaworld *yw)
{
	for(int i = 0; i < 70; i++)
	{
		yw->iniColors[i].r = 255;
		yw->iniColors[i].g = 255;
		yw->iniColors[i].b = 0;
		yw->iniColors[i].i = 10;
		yw->iniColors[i].color = 0xFFFF00;
	}
}

int yw_ParseWorldIni(_NC_STACK_ypaworld *yw, const char *filename)
{
	scrCallBack parsers[5];

	memset(parsers, 0, sizeof(parsers));

	parsers[0].world = yw;
	parsers[0].func = BkgParser;
	parsers[1].world = yw;
	parsers[1].func = Colors_Parser;
	parsers[2].world = yw;
	parsers[2].func = MoviesParser;
	parsers[3].world = yw;
	parsers[3].func = SuperItem_Parse;
	parsers[4].world = yw;
	parsers[4].func = Misc_Parse;

	return def_parseFile(filename, 5, parsers, 2);
}

int yw_InitLevelNet(_NC_STACK_ypaworld *yw)
{
	yw->field_2d90 = (stru_2d90 *)AllocVec(sizeof(stru_2d90), 65537);
	if ( !yw->field_2d90 )
	{
		if ( yw->LevelNet )
			nc_FreeMem(yw->LevelNet);

		if ( yw->field_2d90 )
			nc_FreeMem(yw->field_2d90);

		yw->field_2d90 = NULL;
		yw->LevelNet = NULL;
		return 0;
	}

	yw->LevelNet = (stru_LevelNet *)AllocVec(sizeof(stru_LevelNet), 65537);
	if ( !yw->LevelNet )
	{
		if ( yw->LevelNet )
			nc_FreeMem(yw->LevelNet);

		if ( yw->field_2d90 )
			nc_FreeMem(yw->field_2d90);

		yw->field_2d90 = NULL;
		yw->LevelNet = NULL;
		return 0;
	}

	yw_InitLevelNet__sub0(yw);
	if ( !yw_ParseWorldIni(yw, "data:world.ini") )
	{
		ypa_log_out("yw_ParseWorldIni() failed.\n");
		if ( yw->LevelNet )
			nc_FreeMem(yw->LevelNet);

		if ( yw->field_2d90 )
			nc_FreeMem(yw->field_2d90);

		yw->field_2d90 = NULL;
		yw->LevelNet = NULL;
		return 0;
	}

	if ( !yw_ScanLevels(yw) )
	{
		ypa_log_out("yw_ScanLevels() failed.\n");
		if ( yw->LevelNet )
			nc_FreeMem(yw->LevelNet);

		if ( yw->field_2d90 )
			nc_FreeMem(yw->field_2d90);

		yw->field_2d90 = NULL;
		yw->LevelNet = NULL;
		return 0;
	}
	yw->field_2d90->field_74 = 128;
	memset(yw->field_2d90->jodiefoster, 0, 32);
	return 1;
}

int yw_InitNetwork(_NC_STACK_ypaworld *yw)
{
	printf("MAKE ME %s\n","yw_InitNetwork");
	return 1;
}



void yw_setIniColor(_NC_STACK_ypaworld *yw, int color_id, int r, int g, int b, int i)
{
	yw->iniColors[color_id].r = r;
	yw->iniColors[color_id].g = g;
	yw->iniColors[color_id].b = b;
	yw->iniColors[color_id].i = i;
	yw->iniColors[color_id].color = ((r << 16) & 0xFF0000) | ((g << 8) & 0xFF0000) | (b & 0xFF) ;
}

int yw_parse_color(_NC_STACK_ypaworld *yw, int color_number, char *color_string)
{
	char *pr = strtok(color_string, "_");
	char *pg = strtok(0, "_");
	char *pb = strtok(0, "_");
	char *pi = strtok(0, "_");

	if ( pr && pg && pb && pi )
	{
		int r = atoi(pr);
		int g = atoi(pg);
		int b = atoi(pb);
		int i = atoi(pi);
		yw_setIniColor(yw, color_number, r, g, b, i);
		return 1;
	}

	return 0;
}

int get_level_numb(const char *filename)
{
	char buf[260];
	strcpy(buf, filename);
	char *tmp = strpbrk(buf, ".");
	if (tmp)
		*tmp = 0;

	int v9 = strlen(buf);

	if ( v9 == 5 )
	{
		buf[3] = 0;
	}
	else if ( v9 == 7 )
	{
		buf[4] = 0;
	}

	int v10 = atoi(&buf[1]);
	if ( v10 < 1 || v10 >= 256 )
	{
		v10 = -1;
		ypa_log_out("Invalid level num [valid: 0..127] for %s.\n", filename);
	}
	return v10;
}





int sb_0x4e1a88__sub0__sub0(_NC_STACK_ypaworld *yw, mapProto *mapp, const char *fname)
{
	scrCallBack v7[3];

	memset(v7, 0, sizeof(v7));
	memset(mapp, 0, sizeof(mapProto));

	v7[0].dataForStore = mapp;
	v7[0].world = yw;
	v7[0].func = LevelDataParser;
	v7[1].dataForStore = mapp;
	v7[1].func = parse_map_robos;
	v7[2].world = yw;
	v7[2].dataForStore = mapp;
	v7[2].func = MapParseSizes;

	return def_parseFile(fname, 3, v7, 6);
}



int sb_0x4e1a88__sub0(_NC_STACK_ypaworld *yw, int multiplayer, const char *fname)
{
  mapProto mapp;
  char buf[256];

  int levelnmb = get_level_numb(fname);

  if ( levelnmb != -1 )
  {
    mapINFO *minf = &yw->LevelNet->mapInfos[levelnmb];

    if ( multiplayer )
    {
        strcpy(buf, "levels:multi/");
    }
    else
    {
        strcpy(buf, "levels:single/");
    }

    strcat(buf, fname);

    sb_0x4e1a88__sub0__sub0(yw, &mapp, buf);

    if ( multiplayer )
      minf->field_0 = 4;
    else
      minf->field_0 = 2;

    minf->field_10 = 0;
    minf->field_C = minf->field_10;
    minf->field_8 = minf->field_10;
    minf->field_4 = minf->field_10;

    printf("FIX ME (memset(minf->field_14, 0, 8)) %s #%d\n", __FILE__, __LINE__);
    memset(minf->field_14, 0, 8);

    strcpy(minf->mapPath, buf);

    strcpy(minf->map_name, yw->field_2d90->map_name);

    printf("FIX ME (memset(minf->field_9C, 0, 16)) %s #%d\n", __FILE__, __LINE__);
    memset(minf->field_9C, 0, 16);

    minf->fractions_mask = 0;
    minf->robos_count = mapp.mapRobos_count;

    for (int i = 0; i < mapp.mapRobos_count; i++)
        minf->fractions_mask |= 1 << mapp.mapRobos[i].owner;

    minf->secXsize = LOBYTE(mapp.secXsize);
    minf->secYsize = LOBYTE(mapp.secYsize);
    minf->slow_connection = mapp.slow_connection;
    return 1;
  }
  return 0;
}

int sb_0x4e1a88(_NC_STACK_ypaworld *yw, int multiplayer)
{
	int v10 = 1;

	const char *v4;

	if ( multiplayer )
		v4 = "levels:multi/";
	else
		v4 = "levels:single/";

	ncDir *v6 = OpenDir(v4);
	if ( v6 )
	{
		dirEntry v8;
		while ( ReadDir(v6, &v8) )
		{
			if ( strcmp(v8.e_name, ".") && strcmp(v8.e_name, "..") )
			{
				if ( sb_0x4e1a88__sub0(yw, multiplayer, v8.e_name) )
				{
					ypa_log_out("Scanning [%s%s] .. ok.\n", v4, v8.e_name);
				}
				else
				{
					ypa_log_out("Scanning [%s%s] .. FAILED.\n", v4, v8.e_name);
					v10 = 0;
				}
			}
		}
		CloseDir(v6);
	}

	return v10;
}

int yw_ScanLevels(_NC_STACK_ypaworld *yw)
{
	int v2 = 1;
	if ( !sb_0x4e1a88(yw, 0) )
		v2 = 0;
	if ( !sb_0x4e1a88(yw, 1) )
		v2 = 0;
	return v2;
}
