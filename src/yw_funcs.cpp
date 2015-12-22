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

int load_fonts_and_icons(_NC_STACK_ypaworld *yw)
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
			return 0;
		}

		gfx_set_tileset(yw->tiles[i], i);
	}


	yw->tiles[40] = yw_LoadTileSet("lego16.ilbm", 16, 16, 16, 16, 16, 16, 0, 0);
	if ( !yw->tiles[40] )
		return 0;
	gfx_set_tileset(yw->tiles[40], 40);

	yw->tiles[41] = yw_LoadTileSet("lego8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
	if ( !yw->tiles[41] )
		return 0;
	gfx_set_tileset(yw->tiles[41], 41);

	yw->tiles[42] = yw_LoadTileSet("lego4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
	if ( !yw->tiles[42] )
		return 0;
	gfx_set_tileset(yw->tiles[42], 42);

	yw->tiles[43] = yw_LoadTileSet("sec4.ilbm", 4, 4, 4, 4, 16, 16, 0, 0);
	if ( !yw->tiles[43] )
		return 0;
	gfx_set_tileset(yw->tiles[43], 43);

	yw->tiles[44] = yw_LoadTileSet("sec8.ilbm", 8, 8, 8, 8, 16, 16, 0, 0);
	if ( !yw->tiles[44] )
		return 0;
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
			return 0;

		v14->chars[9].byteoff = v14->chars[8].byteoff;
		v14->chars[9].width = v14->chars[8].width;
		v14->chars[8].byteoff = v14->chars[0].byteoff;
		v14->chars[8].width = v14->chars[0].width;

		yw->tiles[id] = v14;
		gfx_set_tileset(v14, id);


	}

	yw->tiles[59] = yw_LoadFont(yw, "mapvhcl3.font");
	if ( !yw->tiles[59] )
		return 0;
	gfx_set_tileset(yw->tiles[59], 59);

	yw->tiles[60] = yw_LoadFont(yw, "mapvhcl5.font");
	if ( !yw->tiles[60] )
		return 0;
	gfx_set_tileset(yw->tiles[60], 60);

	yw->tiles[61] = yw_LoadFont(yw, "mapvhcl7.font");
	if ( !yw->tiles[61] )
		return 0;
	gfx_set_tileset(yw->tiles[61], 61);

	yw->tiles[62] = yw_LoadFont(yw, "mapvhcl9.font");
	if ( !yw->tiles[62] )
		return 0;
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

	return 1;
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

		memset(&minf->field_9C, 0, sizeof(ua_fRect));

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

int yw_InitTooltips(_NC_STACK_ypaworld *yw)
{
	const char **tips = (const char **)AllocVec(sizeof(char *) * 199, 65537);
	yw->tooltips = tips;
	if ( tips )
	{
		tips[0] = NULL;
		tips[1] = "GIVE ORDERS TO SELECTED SQUADRON";
		tips[2] = "SWITCH INTO DEFENSE GUNS";
		tips[3] = "CREATE A NEW SQUADRON LEADER";
		tips[4] = "ADD A DRONE TO SELECTED SQUADRON";
		tips[5] = "TAKE OVER CONTROL OF A DRONE";
		tips[6] = "CREATE A NEW BUILDING";
		tips[7] = "BEAM TO SOME OTHER PLACE";
		tips[8] = "OPEN/CLOSE THE MAP WINDOW";
		tips[9] = "OPEN/CLOSE THE FINDER WINDOW";
		tips[10] = "OPEN/CLOSE THE MESSAGE LOG WINDOW";
		tips[11] = "OPEN/CLOSE THE ENERGY WINDOW";
		tips[12] = "AGGR SETTING: ESCAPE NOW";
		tips[13] = "AGGR SETTING: ATTACK PRIMARY TARGET ONLY";
		tips[14] = "AGGR SETTING: SEARCH AND DESTROY NEARBY ENEMY DRONES";
		tips[15] = "AGGR SETTING: ATTACK ALL ENEMY TARGETS ON YOUR WAY";
		tips[16] = "AGGR SETTING: GO AMOK";
		tips[17] = "SWITCH BACK INTO HOST STATION";
		tips[18] = "SWITCH INTO COMMANDER OF SQUAD";
		tips[19] = "SWITCH INTO NEXT UNIT OF SQUAD";
		tips[20] = "SWITCH INTO NEXT SQUADRON COMMANDER";
		tips[21] = "PAUSE OR CANCEL CURRENT MISSION";
		tips[22] = "SHOW/HIDE SECTOR DETAIL LAYER";
		tips[23] = "SHOW/HIDE SECTOR OWNER LAYER";
		tips[24] = "SHOW/HIDE EXTENDED INFO LAYER";
		tips[25] = "FREE MAP SCROLLING";
		tips[26] = "MAP SCROLLING LOCKED ON CURRENT VIEWER";
		tips[27] = "MAP SCROLLING LOCKED ON SELECTED SQUADRON";
		tips[28] = "ZOOM OUT";
		tips[29] = "ZOOM IN";
		tips[30] = "TOGGLE MAP SIZE";
		tips[31] = "SQUADRON ACTION: AWAITING ORDER";
		tips[32] = "SQUADRON ACTION: FIGHT ENEMY";
		tips[33] = "SQUADRON ACTION: ON THE WAY";
		tips[34] = "SQUADRON ACTION: ESCAPING";
		tips[35] = "ENERGY RELOAD LEVEL";
		tips[36] = "MAIN SYSTEM BATTERY LEVEL";
		tips[37] = "DRONE CREATION BATTERY LEVEL";
		tips[38] = "BUILDING CREATION BATTERY LEVEL";
		tips[39] = "BEAM TRANSPORTER BATTERY LEVEL";
		tips[40] = "SELECT ANOTHER ACTION MODE";
		tips[41] = "SELECT A DEFENSE GUN";
		tips[42] = "SELECT A DRONE TYPE";
		tips[43] = "SELECT A BUILDING TYPE";
		tips[44] = "SHOW/HIDE HEAD UP DISPLAY";
		tips[45] = "CONTROL TO LAST MESSAGE SENDER";
		tips[46] = "NUMBER OF VEHICLES IN SELECTED SQUAD";
		tips[47] = "SITUATION ANALYZER";
		tips[48] = "ONLINE HELP";
		tips[64] = "MAKE SQUADRON THE SELECTED ONE";
		tips[65] = "ADVANCE TO THIS LOCATION";
		tips[66] = "ATTACK THIS SECTOR";
		tips[67] = "ATTACK THIS DRONE";
		tips[68] = "CREATE A NEW SQUADRON LEADER";
		tips[69] = "ADD A NEW DRONE TO SELECTED SQUAD";
		tips[70] = "TAKE OVER HAND CONTROL OF THIS DRONE";
		tips[71] = "CREATE A NEW BUILDING HERE";
		tips[72] = "BEAM HOST STATION TO THIS LOCATION";
		tips[73] = "EXIT DEBRIEFING";
		tips[74] = "GOTO LOAD/SAVE SCREEN";
		tips[75] = "SELECT 3D DEVICE";
		tips[76] = "SEND THIS MESSAGE";
		tips[77] = "USE ALTERNATE JOYSTICK MODEL";
		tips[78] = "USE 16 BIT TEXTURES";
		tips[79] = "USE DRAW PRIMITIVE D3D MODEL";
		tips[80] = "880 == MOUSE CONTROL MODE ACTIVE (PRESS RMB TO DEACTIVATE)";
		tips[81] = "881 == UNIT LIMIT REACHED";
		tips[96] = "ERROR: NO VALID TARGET GIVEN!";
		tips[97] = "ERROR: NOT ENOUGH ROOM HERE!";
		tips[98] = "ERROR: CURRENTLY NOT ENOUGH ENERGY!";
		tips[99] = "ERROR: SECTOR MUST BE CONQUERED!";
		tips[100] = "ERROR: LOCATION TOO FAR AWAY!";
		tips[101] = "ERROR: LOCATION TOO CLOSE TO HOST STATION!";
		tips[102] = "ERROR: BUILDING CREATION SYSTEM BUSY!";
		tips[103] = "ERROR: MUST BE NO DRONES IN TARGET SECTOR!";
		tips[104] = "ERROR: TARGET LOCATION TOO JAGGY!";
		tips[105] = "ERROR: LOCATION NOT IN SENSOR AREA!";
		tips[106] = "GO TO LEVEL SELECTION MAP";
		tips[107] = "GO TO TRAINING AREA";
		tips[108] = "USE SELECTED LANGUAGE";
		tips[109] = "CLOSE LOCALE REQUESTER";
		tips[110] = "CHOOSE A NEW LEVEL";
		tips[111] = "SEND MESSAGE MTO ALL PLAYERS";
		tips[112] = "SELECT RESISTANCE AS RACE";
		tips[113] = "SELECT GHORKOV AS RACE";
		tips[114] = "SELECT MYKONIANS AS RACE";
		tips[115] = "SELECT TAERKASTEN AS RACE";
		tips[116] = "MARK GAME AS STARTABLE";
		tips[117] = "MARK GAME AS NOT STARTBALE";
		tips[118] = "LOAD SELECTED PLAYER";
		tips[119] = "SAVE ACTUAL PLAYER";
		tips[120] = "OPEN/CLOSE INPUTREQUESTER";
		tips[121] = "OPEN/CLOSE VIDEOREQUESTER";
		tips[122] = "OPEN/CLOSE SOUNDREQUESTER";
		tips[123] = "OPEN/CLOSE IN-OUT-REQUESTER";
		tips[124] = "OPEN/CLOSE LANGUAGEREQUESTER";
		tips[125] = "PAUSE/CONTINUE MISSION BRIEFING";
		tips[126] = "STOP MISSION BRIEFING";
		tips[127] = "LOAD LAST SAVEGAME";
		tips[128] = "WIND FORWARD MISSION BRIEFING";
		tips[129] = "RESET MISSION BRIEFING";
		tips[130] = "PLAY THE LEVEL";
		tips[131] = "UHHHMMM... QUIT? YEAH RIGHT, DUDE, KILL IT!";
		tips[132] = "PRESS FOR ADDITIONAL SELECTION";
		tips[133] = "ENTER NEW KEY IF BUTTON IS PRESSED";
		tips[134] = "YOU HEAR THE SOUND AT LEFT";
		tips[135] = "YOU HEAR THE SOUND AT RIGHT";
		tips[136] = "OPEN/CLOSE NETWORKREQUESTER";
		tips[137] = "GIVE ME MORE INFORMATION";
		tips[138] = "GO BACK TO START PAGE";
		tips[139] = "GO BACK TO LEVELSELECTION";
		tips[140] = "CLOSE NETWORK REQUESTER";
		tips[141] = "???";
		tips[142] = "APPLY THIS PROVIDER";
		tips[143] = "APPLY THIS PLAYERNAME";
		tips[144] = "CREATE GAME WITH THIS LEVEL";
		tips[145] = "JOIN THIS SESSION";
		tips[146] = "START GAME";
		tips[147] = "GO BACK TO PROVIDER SELECTION";
		tips[148] = "GO BACK TO ENTER A NEW NAME";
		tips[149] = "GO BACK TO SESSION SELECTION";
		tips[151] = "EXIT DEBRIEFING";
		tips[152] = "REWIND DEBRIEFING";
		tips[153] = "PLAYING TIME - THIS LEVEL";
		tips[154] = "PLAYING TIME - OVERALL";
		tips[155] = "KILLS - THIS LEVEL";
		tips[156] = "KILLS - OVERALL";
		tips[157] = "LOSSES - THIS LEVEL";
		tips[158] = "LOSSES - OVERALL";
		tips[159] = "SECTORS CONQUERED - THIS LEVEL";
		tips[160] = "SECTORS CONQUERED - OVERALL";
		tips[161] = "SCORE - THIS LEVEL";
		tips[162] = "SCORE - OVERALL";
		tips[163] = "POWERSTATIONS CAPTURED - THIS LEVEL";
		tips[164] = "POWERSTATIONS CAPTURED - OVERALL";
		tips[165] = "TECH UPGRADES CAPTURED - THIS LEVEL";
		tips[166] = "TECH UPGRADES CAPTURED - OVERALL";
		tips[167] = "SHOW STATISTICS FOR THIS RACE";
		tips[168] = "DELETE SELECTED PLAYER";
		tips[169] = "CREATE A NEW PLAYER";
		tips[170] = "LOAD PLAYER WITH THIS NAME";
		tips[171] = "SAVE ACTUAL PLAYER UNDER THIS NAME";
		tips[172] = "DELETE PLAYER WITH THIS NAME";
		tips[173] = "CREATE PLAYER WITH THIS NAME";
		tips[174] = "CLOSE PLAYER REQUESTER";
		tips[175] = "STOP LOADING PLAYER";
		tips[176] = "STOP SAVING PLAYER";
		tips[177] = "STOP DELETING PLAYER";
		tips[178] = "STOP CREATING PLAYER";
		tips[179] = "CLOSE SETTINGS REQUESTER";
		tips[180] = "APPLY NEW SETTINGS";
		tips[181] = "PRESS TO CHANGE SCREEN RESOLUTION";
		tips[182] = "SWITCH FX-SOUND";
		tips[183] = "TOGGLE CHANNEL ASSIGNMENT";
		tips[184] = "CHANGE HORIZON DEPTH";
		tips[185] = "SWITCH ON/OFF SKY";
		tips[186] = "SWITCH ON/OFF FILTERING";
		tips[187] = "ENABLE/DISABLE CD SOUND";
		tips[188] = "USE SOFTWARE OR HARDWARE MOUSEPOINTER";
		tips[189] = "ALLOW/FORBID ENEMYINDICATORS";
		tips[190] = "CHANGE FX VOLUME";
		tips[191] = "CHANGE CD VOLUME";
		tips[192] = "CHANGE NUMBER OF EXPLODE EFFECTS";
		tips[193] = "APPLY INPUT CHANGES";
		tips[194] = "IGNORE INPUT CHANGES";
		tips[195] = "RESET TO DEFAULT KEYS";
		tips[196] = "USE JOYSTICK";
		tips[197] = "USE FORCEFEEDBACK FOR JOSTICK";
		tips[198] = "REMOVE KEY FROM ACTION";
	}
	else
		return 0;
	return 1;
}

NC_STACK_base * sub_44AD8C(const char *fname)
{
	NC_STACK_base *obj = (NC_STACK_base *)init_get_class("base.class", 0);
	if ( obj )
	{
		FILE *fil = FOpen(fname, "r");
		if ( !fil )
		{
			delete_class_obj(obj);
			return NULL;
		}

		char buf[512];
		char basfl[300];

		while ( fgets(buf, 512, fil) )
		{
			char *v4 = strtok(buf, " #;\t\n");

			if ( !v4 || *v4 == '>' )
				break;

			strcpy(basfl, "rsrc:objects/");
			strcat(basfl, v4);

			NC_STACK_base *kid = READ_BAS_FILE(basfl);

			if ( !kid )
			{
				ypa_log_out("init: Could not load %s.\n", basfl);
				FClose(fil);
				delete_class_obj(obj);
				return 0;
			}

            //call_vtbl(obj, 65, kid);
			call_method(obj, 65, &kid); //Add to kid list
		}
		FClose(fil);
	}
	return obj;
}

NC_STACK_base *load_set_base()
{
	NC_STACK_base *base = READ_BAS_FILE("rsrc:objects/set.base");
	if ( !base )
	{
		ypa_log_out("init: no set.base, trying fragment load.\n");

		base = (NC_STACK_base *)init_get_class("base.class", 0);
		if ( base )
		{
			NC_STACK_base *visproto = READ_BAS_FILE("rsrc:objects/visproto.base");
			if ( !visproto )
			{
				ypa_log_out("init: no visproto.base, trying single load.\n");
				visproto = sub_44AD8C("rsrc:scripts/visproto.lst");
			}
			if ( !visproto )
			{
				delete_class_obj(base);
				return NULL;
			}
			//call_vtbl(base, 65, visproto);
			call_method(base, 65, &visproto);

			NC_STACK_base *lego = READ_BAS_FILE("rsrc:objects/lego.base");
			if ( !lego )
			{
				ypa_log_out("init: no lego.base, trying single load.\n");
				lego = sub_44AD8C("rsrc:scripts/set.sdf");
			}
			if ( !lego )
			{
				delete_class_obj(base);
				return NULL;
			}
			//call_vtbl(base, 65, lego);
			call_method(base, 65, &lego);

			NC_STACK_base *slurp = READ_BAS_FILE("rsrc:objects/slurp.base");
			if ( !slurp )
			{
				ypa_log_out("init: no slurp.base, trying single load.\n");
				slurp = sub_44AD8C("rsrc:scripts/slurps.lst");
			}
			if ( !slurp )
			{
				delete_class_obj(base);
				return NULL;
			}
			//call_vtbl(base, 65, slurp);
			call_method(base, 65, &slurp);
		}
	}
	return base;
}

int sub_44A12C(_NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
	nlist *kid_list;

	call_vtbl(base, 3, 0x8000101A, &kid_list, 0); // get kids list

	int id = 0;
	for ( base_node *bkid = (base_node *)kid_list->head; bkid->next; bkid = (base_node *)bkid->next)
	{
		NC_STACK_base *kid_obj = bkid->self_full;

		yw->vhcls_models[id].base = kid_obj;
		call_vtbl(kid_obj, 2, 0x80001004, yw->field_15e4, 0);
		call_vtbl(kid_obj, 2, 0x80001023, yw->field_15e8, 0);
		call_vtbl(kid_obj, 3, 0x80001019, &yw->vhcls_models[id].trigo, 0);
		id++;
	}
	return 1;
}

int yw_parse_lego(_NC_STACK_ypaworld *yw, FILE *fil, NC_STACK_base *base)
{
	nlist *kid_list;
	call_vtbl(base, 3, 0x8000101A, &kid_list, 0);

	int id = 0;
	for ( base_node *bkid = (base_node *)kid_list->head; bkid->next; bkid = (base_node *)bkid->next)
	{
		NC_STACK_base *kid_obj = bkid->self_full;

		yw->legos[id].base = kid_obj;
		call_vtbl(kid_obj, 2, 0x80001004, yw->field_15e4, 0);
		call_vtbl(kid_obj, 2, 0x80001023, yw->field_15e8, 0);
		call_vtbl(kid_obj, 2, 0x80001024, 1, 0);
		id++;
	}

	id = 0;
	char line_buf[1024];

	while ( fgets(line_buf, 1024, fil) && line_buf[0] != '>' )
	{
		cityBases *lego = &yw->legos[id];

		if ( id >= 256 )
		{
			ypa_log_out("Too many legos!\n");
			return 0;
		}

		char *v10 = strpbrk(line_buf, ";#\n");
		if ( v10 )
			*v10 = 0;

		if ( strtok(line_buf, " \t") ) // ignore base file name
		{
			char *v11 = strtok(0, " \t");
			if ( !v11 )
				return 0;

			NC_STACK_sklt *skelet = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, v11, 0);
			if ( !skelet )
			{
				ypa_log_out("Couldn't load sklt (%s)\n", v11);
				return 0;
			}

			skeleton_64_stru *v26;
			call_vtbl(skelet, 3, 0x80002000, &v26, 0);

			lego->sklt_obj = skelet;
			lego->sklt_obj_intern = v26;

			v11 = strtok(0, " \t");
			if ( !v11 )
				return 0;

			lego->field_11 = atoi(v11);

			v11 = strtok(0, " \t");
			if ( !v11 )
				return 0;

			lego->field_12 = atoi(v11);

			v11 = strtok(0, " \t");
			if ( !v11 )
				return 0;

			lego->field_10 = atoi(v11);

			v11 = strtok(0, " \t");
			if ( !v11 )
				return 0;

			switch(atoi(v11))
			{
			default:
			case 0:
				lego->selected_sklt_intern = lego->sklt_obj_intern;
				break;

			case 1:
				lego->selected_sklt_intern = yw->colsub_sklt_intrn;
				break;

			case 2:
				lego->selected_sklt_intern = yw->colcomp_sklt_intrn;
				break;
			}

			int fxnumb = 0;
			while ( 1 )
			{
				v11 = strtok(0, " \t");
				if ( !v11 || fxnumb >= yw->fxnumber )
					break;

				float x = 0.0;
				float y = 0.0;
				float z = 0.0;

				lego->field_14[fxnumb] = atoi(v11);

				v11 = strtok(0, " \t");
				if ( v11 )
					x = strtof(v11, 0);

				v11 = strtok(0, " \t");
				if ( v11 )
					z = strtof(v11, 0);

				v11 = strtok(0, " \t");
				if ( v11 )
					y = strtof(v11, 0);

				lego->field_24[fxnumb].pos_x = x;
				lego->field_24[fxnumb].pos_y = -y;
				lego->field_24[fxnumb].pos_z = -z;

				fxnumb++;
			}

			id++;
		}
	}
	return 1;
}

int yw_parse_subSect(_NC_STACK_ypaworld *yw, FILE *fil)
{
	char buf[256];

	int id = 0;
	while ( fgets(buf, 255, fil) && buf[0] != '>' )
	{
		if ( id >= 256 )
		{
			ypa_log_out("Too many sub sector defs!\n");
			return 0;
		}

		char *endln = strpbrk(buf, ";#\n");
		if ( endln )
			*endln = 0;

		char *pp = strtok(buf, " \t");
		if ( pp )
		{
			subSec *ssec = &yw->subSectors[id];

			ssec->field_4 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
				return 0;

			ssec->field_5 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
				return 0;

			ssec->field_6 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
				return 0;

			ssec->field_7 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
				return 0;

			if ( *pp > '0' && *pp == '1' )
				ssec->field_0 = 255;
			else
				ssec->field_0 = 0;

			pp = strtok(0, " \t");
			if ( pp )
				ssec->field_8 = atoi(pp);

			id++;
		}
	}
	return 1;
}

int yw_parse_sektor(_NC_STACK_ypaworld *yw, FILE *fil)
{
	char line_buf[512];
	char buf[512];

	while ( fgets(buf, 512, fil) && buf[0] != '>' )
	{
		strcpy(line_buf, buf);

		char *lnend = strpbrk(buf, ";#\n");
		if ( lnend )
			*lnend = 0;

		char *pp = strtok(buf, " \t");
		if ( pp )
		{
			secType *sektp = &yw->secTypes[atoi(pp)];

			pp = strtok(0, " \t");
			if ( !pp )
			{
				ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
				return 0;
			}

			sektp->field_0 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
			{
				ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
				return 0;
			}

			sektp->field_1 = atoi(pp);

			pp = strtok(0, " \t");
			if ( !pp )
			{
				ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
				return 0;
			}

			sektp->field_3 = atoi(pp);

			memset(sektp->field_4, 0, sizeof(sektp->field_4));

			if ( sektp->field_0 == 1 )
			{
				pp = strtok(0, " \t");
				if ( !pp )
				{
					ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
					return 0;
				}
				sektp->field_4[0][0] = &yw->subSectors[atoi(pp)];
			}
			else
			{
				for (int j = 0; j < 3; j++)
				{
					for (int i = 0; i < 3; i++)
					{
						pp = strtok(0, " \t");
						if ( !pp )
						{
							ypa_log_out("Error reading '%s', line '%s'.\n", "set.sdf", line_buf);
							return 0;
						}

						sektp->field_4[i][2 - j] = &yw->subSectors[atoi(pp)];
					}
				}
			}
		}
	}
	return 1;
}

int sub_44A97C(_NC_STACK_ypaworld *yw, NC_STACK_base *base)
{
	nlist *kid_list;
	call_vtbl(base, 3, 0x8000101A, &kid_list, 0);

	base_node *kid = (base_node *)kid_list->head;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				if ( !kid->next)
				{
					ypa_log_out("Too few slurps in slurp child.\n");
					return 0;
				}

				NC_STACK_base *full_kid = kid->self_full;

				call_vtbl(full_kid, 2, 0x80001004, yw->field_15e4, 0);
				call_vtbl(full_kid, 2, 0x80001023, yw->field_15e8, 0);
				call_vtbl(full_kid, 2, 0x80001024, 1, 0);

				NC_STACK_skeleton *skeleton;
				call_vtbl(full_kid, 3, 0x80001000, &skeleton, 0);

				skeleton_64_stru *skeleton_internal;
				call_vtbl(skeleton, 3, 0x80002000, &skeleton_internal, 0);

				if (i == 0)
				{
					yw->slurps1[j][k].skeletons_bas = full_kid;
					yw->slurps1[j][k].skeleton_internal = skeleton_internal;
				}
				else if (i == 1)
				{
					yw->slurps2[j][k].skeletons_bas = full_kid;
					yw->slurps2[j][k].skeleton_internal = skeleton_internal;
				}
				kid = (base_node *)kid->next;
			}
		}
	}

	char rsr[256];
	strcpy(rsr, get_prefix_replacement("rsrc"));

	set_prefix_replacement("rsrc", "data:mc2res");

	NC_STACK_sklt *tmp_skel = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, "Skeleton/ColSide.sklt", 0);
	if ( !tmp_skel )
	{
		ypa_log_out("Couldn't create side collision sklt.\n");
		return 0;
	}

	yw->ColSide = tmp_skel;
	call_vtbl(yw->ColSide, 3, 0x80002000, &yw->ColSide__skeletonIntern, 0);

	tmp_skel = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, "Skeleton/ColCross.sklt", 0);
	if ( !tmp_skel )
	{
		ypa_log_out("Couldn't create cross collision sklt.\n");
		return 0;
	}

	yw->ColCross = tmp_skel;
	call_vtbl(yw->ColCross, 3, 0x80002000, &yw->ColCross__skeletonIntern, 0);

	set_prefix_replacement("rsrc", rsr);

	return 1;
}

int yw_InitMouseStuff(_NC_STACK_ypaworld *yw)
{
	char rsr[256];

	strcpy(rsr, get_prefix_replacement("rsrc"));
	set_prefix_replacement("rsrc", "data:mc2res");

	for (int i = 0; i < 11; i++)
	{
		const char *v5;
		switch ( i )
		{
		case 1:
			v5 = (const char *)"pointers/cancel.ilbm";
			break;
		case 2:
			v5 = "pointers/select.ilbm";
			break;
		case 3:
			v5 = "pointers/attack.ilbm";
			break;
		case 4:
			v5 = "pointers/goto.ilbm";
			break;
		case 5:
			v5 = "pointers/disk.ilbm";
			break;
		case 6:
			v5 = "pointers/new.ilbm";
			break;
		case 7:
			v5 = "pointers/add.ilbm";
			break;
		case 8:
			v5 = "pointers/control.ilbm";
			break;
		case 9:
			v5 = "pointers/beam.ilbm";
			break;
		case 10:
			v5 = "pointers/build.ilbm";
			break;
		default:
			v5 = "pointers/pointer.ilbm";
			break;
		}

		yw->pointers[i] = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, v5, 0x80002008, 1, 0x80002009, 1, 0);
		if ( !yw->pointers[i] )
		{
			ypa_log_out("yw_select.c/yw_InitMouseStuff()\n");
			set_prefix_replacement("rsrc", rsr);
			return 0;
		}

		call_vtbl(yw->pointers[i], 3, 0x80002000, &yw->pointers__bitm[i], 0);
	}

	displ_arg263 arg_263;

	arg_263.bitm = 0;
	arg_263.pointer_id = 0;
	call_method(yw->win3d, 263, &arg_263);

	arg_263.bitm = yw->pointers__bitm[0];
	arg_263.pointer_id = 1;
	call_method(yw->win3d, 263, &arg_263);

	set_prefix_replacement("rsrc", rsr);
	return 1;
}

int yw_LoadSet(_NC_STACK_ypaworld *yw, int setID)
{
	gfxEngine__getter(0x8000300D, &yw->win3d, 0);
	yw->field_17c0 = 0;

	char buf[1024];
	sprintf(buf, "data:set%d", setID);

	char rsr[256];
	strcpy(rsr,  get_prefix_replacement("rsrc"));

	call_method(yw->win3d, 271);


	set_prefix_replacement("rsrc", "data:mc2res");

	yw->colsub_sklt = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, "skeleton/colsub.sklt", 0);
	if ( !yw->colsub_sklt )
	{
		ypa_log_out("Couldn't load <skeleton/colsub.sklt>, set %d.\n", setID);
		return 0;
	}
	call_vtbl(yw->colsub_sklt, 3, 0x80002000, &yw->colsub_sklt_intrn, 0);

	yw->colcomp_sklt = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, "skeleton/colcomp.sklt", 0);
	if ( !yw->colcomp_sklt )
	{
		ypa_log_out("Couldn't load <skeleton/colcomp.sklt>, set %d.\n", setID);
		return 0;
	}
	call_vtbl(yw->colcomp_sklt, 3, 0x80002000, &yw->colcomp_sklt_intrn, 0);


	set_prefix_replacement("rsrc", buf);

	if ( !win3d__load_palette_from_ilbm("palette/standard.pal") )
		ypa_log_out("WARNING: Could not load set default palette!\n");

	if ( setID != yw->set_number && setID != 46 )
	{
		if ( yw->additionalSet )
		{
			ypa_log_out("yw_LoadSet(): killing set object %d\n", yw->set_number);
			delete_class_obj(yw->additionalSet);
			yw->additionalSet = NULL;
			yw->set_number = 0;
		}

		yw->additionalSet = load_set_base();
		if ( !yw->additionalSet )
		{
			yw->set_number = 0;
			ypa_log_out("yw_LoadSet(): loading set object %d failed\n", setID);
			set_prefix_replacement("rsrc", rsr);
			return 0;
		}
		yw->set_number = setID;
		ypa_log_out("yw_LoadSet(): loaded set object %d ok\n", setID);
	}


	if ( setID != 46 )
	{
		FILE* fil = FOpen("rsrc:scripts/set.sdf", "r");
		if ( !fil )
		{
			ypa_log_out("Couldn't open set description file, set %d!\n", setID);
			return 0;
		}

		nlist *kid_list;

		call_vtbl(yw->additionalSet, 3, 0x8000101A, &kid_list, 0);// Get KIDS

		int kid_id = 0;

		for (base_node *bnode = (base_node *)kid_list->head; bnode->next; bnode = (base_node *)bnode->next)
		{
			if ( kid_id == 0 )
			{
				if ( !sub_44A12C(yw, bnode->self_full) )
					return 0;
			}
			else if ( kid_id == 1 )
			{
				if ( !yw_parse_lego(yw, fil, bnode->self_full) )
					return 0;

				if ( !yw_parse_subSect(yw, fil) )
					return 0;

				if ( !yw_parse_sektor(yw, fil) )
					return 0;
			}
			else if ( kid_id == 2 )
			{
				if ( !sub_44A97C(yw, bnode->self_full) )
					return 0;
			}

			kid_id++;
		}

		FClose(fil);
	}

	yw->tracyrmp_ilbm = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, "remap/tracyrmp.ilbm", 0);
	if ( !yw->tracyrmp_ilbm )
	{
		ypa_log_out("Couldn't load tracy remap table, set %d.\n", setID);
		return 0;
	}

	yw->shadermp_ilbm = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, "remap/shadermp.ilbm", 0);
	if ( !yw->shadermp_ilbm )
	{
		ypa_log_out("Couldn't load shade remap table, set %d.\n", setID);
		return 0;
	}

	bitmap_intern *shadermp;
	bitmap_intern *tracyrmp;

	call_vtbl(yw->tracyrmp_ilbm, 3, 0x80002000, &tracyrmp, 0);

	call_vtbl(yw->shadermp_ilbm, 3, 0x80002000, &shadermp, 0);

	gfxEngine__setter(0x80003009, tracyrmp, 0x8000300A, shadermp, 0);

	yw->additionalBeeBox = READ_BAS_FILE("rsrc:objects/beebox.base");
	if ( !yw->additionalBeeBox )
	{
		ypa_log_out("Couldn't load bbox object, set %d.\n", setID);
		return 0;
	}

	call_vtbl(yw->additionalBeeBox, 2, 0x80001024, 1, 0);

	if ( setID == 46 || setID == 42 )
	{
		if ( load_fonts_and_icons(yw) == 0 )
			return 0;
	}

	yw_InitMouseStuff(yw);
	return 1;
}


int loadTOD(_NC_STACK_ypaworld *yw, const char *fname)
{
	int tod = 0;
	if ( yw->GameShell )
	{
		char buf[256];
		sprintf(buf, "save:%s/%s", yw->GameShell->user_name, fname);
		FILE *fil = FOpen(buf, "r");

		if ( fil )
		{
			char lnbuf[128];
			if ( fgets(lnbuf, 128, fil) )
			{
				char *lnbrk = strpbrk(lnbuf, "\n;");
				if ( lnbrk )
					*lnbrk = 0;

				tod = atoi(lnbuf);
			}
			FClose(fil);
		}
	}
	return tod;
}

int writeTOD(_NC_STACK_ypaworld *yw, const char *fname, int tod)
{
	int v6 = 0;
	if ( yw->GameShell )
	{
		char buf[256];
		sprintf(buf, "save:%s/%s", yw->GameShell->user_name, fname);
		FILE *fil = FOpen(buf, "w");

		if ( fil )
		{
			fprintf(fil, "%d", tod);
			FClose(fil);
			v6 = 1;
		}
	}
	return v6;
}


void ypaworld_func158__sub4__sub0(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *bitm)
{
	if ( yw )
	{
	    call_method(yw->win3d, 215, 0);

	    rstr_arg204 a4;
		call_vtbl(bitm, 3, 0x80002000, &a4.pbitm, 0);

		a4.float14 = -1.0;
		a4.float4 = -1.0;
		a4.float18 = -1.0;
		a4.float8 = -1.0;
		a4.float1C = 1.0;
		a4.floatC = 1.0;
		a4.float20 = 1.0;
		a4.float10 = 1.0;

		call_method(yw->win3d, 202, &a4);

		call_method(yw->win3d, 216, 0);
	}
}
