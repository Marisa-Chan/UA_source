#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "yw.h"

#include "def_parser.h"

#include "yw_internal.h"

#include "button.h"



stored_functions *classvtbl_get_ypaworld();
class_return * class_set_ypaworld(int, ...);

stored_functions ypaworld_class_vtbl(class_set_ypaworld);


class_stored ypaworld_class_off (NULL, NULL, "MC2classes:ypaworld.class", classvtbl_get_ypaworld);


stored_functions *classvtbl_get_ypaworld()
{
	return &ypaworld_class_vtbl;
}

CLASSFUNC ypaworld_funcs[1024];

polys *p_outPolys;
polysDat *p_polysdata;
polysDat *p_polysdata_end;

char **ypaworld__string_pointers;


int sub_4493B0(scrCallBack *arg)
{
	if ( arg->field_18 )
	{
		if ( !strcasecmp(arg->p1, "end") )
		{
			arg->field_18 = 0;
			return 2;
		}
		else if ( arg->p1 && arg->p2 )
		{
			set_prefix_replacement(arg->p1, arg->p2);
			ypa_log_out("parsing assign.txt: set assign %s to %s\n", arg->p1, arg->p2);
			return 0;
		}
		else
		{
			return 4;
		}
	}
	else if ( !strcasecmp(arg->p1, "begin_assign") )
	{
		arg->field_18 = 1;
		return 1;
	}

	return 3;
}


int ypaworld_func0__sub0(const char *file)
{
	scrCallBack v3;

	memset(&v3, 0, sizeof(scrCallBack));
	v3.func = sub_4493B0;
	return def_parseFile(file, 1, &v3, 0);
}

void ypaworld_func0__sub1()
{
	char replace[1024];

	for (int i = 0; i < 6; i++)
	{
		const char *prefix;
		switch (i)
		{
		case 0:
			prefix = "data";
			break;
		case 1:
			prefix = "save";
			break;
		case 2:
			prefix = "help";
			break;
		case 3:
			prefix = "mov";
			break;
		case 4:
			prefix = "levels";
			break;
		case 5:
			prefix = "mbpix";
			break;
		default:
			prefix = "nop";
			break;
		}

		if ( read_reg_key(prefix, replace, 1024) )
		{
			set_prefix_replacement(prefix, replace);
			ypa_log_out("parsing registry: set assign %s to %s\n", prefix, replace);
		}
	}
}

void sub_4711E0(_NC_STACK_ypaworld *yw)
{
	yw->very_big_array__p_begin = yw->lang_strings;
	yw->lang_strings__end = yw->lang_strings + 0x20000;

	memset(yw->lang_strings, 0, 0x20000);
	memset(yw->string_pointers, 0, 0x28A0);

	memset(yw->lang_name, 0, sizeof(yw->lang_name));
	strcpy(yw->lang_name, "default");
}

int yw_InitLocale(_NC_STACK_ypaworld *yw)
{
	int v3 = 0;
	yw->lang_strings = (char *)AllocVec(0x20000, 1);

	if ( yw->lang_strings )
	{
		yw->string_pointers = (char **)AllocVec(sizeof(char *) * 2600, 65537);

		if ( yw->string_pointers )
		{
			yw->string_pointers_p2 = yw->string_pointers;
			sub_4711E0(yw);
			v3 = 1;
		}
	}

	if ( !v3 )
	{
////	sub_4D99C4(); // free language dll

		if ( yw->lang_strings )
			nc_FreeMem(yw->lang_strings);

		if ( yw->string_pointers )
			nc_FreeMem(yw->string_pointers);

		yw->very_big_array__p_begin = NULL;
		yw->lang_strings__end = NULL;
		yw->string_pointers = NULL;
		yw->lang_strings = NULL;
	}
	return v3;
}

int sub_4DA354(_NC_STACK_ypaworld *yw, const char *filename)
{
	scrCallBack clbk[3];
	memset(clbk, 0, sizeof(clbk));

	char buf[256];

	strcpy(buf, get_prefix_replacement("rsrc"));
	set_prefix_replacement("rsrc", "data:");

	clbk[0].world = yw;
	clbk[0].func = VhclProtoParser;
	clbk[1].world = yw;
	clbk[1].func = WeaponProtoParser;
	clbk[2].world2 = yw;
	clbk[2].func = BuildProtoParser;

	int res = def_parseFile(filename, 3, clbk, 1);
	set_prefix_replacement("rsrc", buf);
	return res;
}

int init_prototypes(_NC_STACK_ypaworld *yw)
{
	yw->VhclProtos = (VhclProto *)AllocVec(sizeof(VhclProto) * 256, 65537);
	yw->WeaponProtos = (WeapProto *)AllocVec(sizeof(WeapProto) * 128, 65537);
	yw->BuildProtos = (BuildProto *)AllocVec(sizeof(BuildProto) * 128, 65537);
	yw->RoboProtos = (roboProto *)AllocVec(sizeof(roboProto) * 16, 65537);

	if ( yw->VhclProtos && yw->WeaponProtos && yw->BuildProtos && yw->RoboProtos )
	{
		if ( sub_4DA354(yw, "data:scripts/startup.scr") )
			return 1;
	}

	return 0;
}

int yw_initAttrs(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, stack_vals *stak)
{
	yw->fxnumber = 16;
	yw->field_1368 = find_id_in_stack_def_val(0x8000200E, 5, stak);
	yw->field_15e4 = find_id_in_stack_def_val(0x80002007, 1400, stak);
	yw->field_15e8 = find_id_in_stack_def_val(0x80002008, 600, stak);
	yw->field_15ec = find_id_in_stack_def_val(0x80002009, 4200, stak);
	yw->field_15f0 = find_id_in_stack_def_val(0x8000200A, 1100, stak);
	yw->sectors_maxX = find_id_in_stack_def_val(0x80002000, 64, stak);
	yw->sectors_maxY = find_id_in_stack_def_val(0x80002001, 64, stak);
	yw->sectors_maxX2 = yw->sectors_maxX;
	yw->sectors_maxY2 = yw->sectors_maxY;
	yw->field_15f4 = find_id_in_stack_def_val(0x8000200B, -550, stak);
	yw->field_15f8 = find_id_in_stack_def_val(0x8000200C, 1, stak);
	yw->field_15fc = find_id_in_stack_def_val(0x8000200D, 1, stak);

	char *v4 = (char *)find_id_in_stack_def_val(0x8000201D, 0, stak);
	yw->buildDate = v4;
	if ( v4 )
	{
		for (char *pchr = v4; *pchr; pchr++)
		{
			char chr = toupper(*pchr);
			if ( chr < ' ' || chr > 'Z' )
				chr = '*';
			*pchr = chr;
		}
	}
	return 1;
}


recorder *sub_48025C()
{
	printf("MAKE ME %s\n","sub_48025C");
	return (recorder *)AllocVec(sizeof(recorder), 65537);
}

int yw_InitSceneRecorder(_NC_STACK_ypaworld *yw)
{
	yw->sceneRecorder = sub_48025C();
	return yw->sceneRecorder != 0;
}

NC_STACK_ypaworld *ypaworld_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
	NC_STACK_ypaworld *obj = (NC_STACK_ypaworld *)call_parent(zis, clss, 0, stak);

	if (obj)
	{
		_NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)&obj->stack__ypaworld;

		yw->self_full = obj;

		init_mc_res_class_engine_strings(MC_TYPE_RES, "rsrc:");
		set_prefix_replacement("rsrc", "mc2res");
		set_prefix_replacement("data", "Data");
		set_prefix_replacement("save", "Save");
		set_prefix_replacement("help", "Help");
		set_prefix_replacement("mov", "Data:mov");
		set_prefix_replacement("levels", "Levels");
		set_prefix_replacement("mbpix", "levels:mbpix");

		if ( !ypaworld_func0__sub0("env/assign.txt") )
			ypa_log_out("Warning, no env/assign.txt script.\n");

		ypaworld_func0__sub1();

		if ( !yw_InitLocale(yw) )
		{
			ypa_log_out("yw_main.c/OM_NEW: yw_InitLocale() failed!\n");
			call_method(obj, 1);
			return NULL;
		}

		const char *langname = "language";
		call_method(obj, 166, &langname); // Load lang strings

//		if ( !make_CD_CHECK(1, 1, v10, v9) )
//		{
//			call_method(obj, 1);
//			return NULL;
//		}

		yw->vhcls_models = (vhclBases *)AllocVec(sizeof(vhclBases) * 512, 65537);
		yw->legos = (cityBases *)AllocVec(sizeof(cityBases) * 256, 65537);
		yw->subSectors = (subSec *)AllocVec(sizeof(subSec) * 256, 65537);
		yw->secTypes = (secType *)AllocVec(sizeof(secType) * 256, 65537);

		if ( !yw->vhcls_models || !yw->legos || !yw->subSectors || !yw->secTypes )
		{
			call_method(obj, 1);
			return NULL;
		}

		if ( !init_prototypes(yw) )
		{
			ypa_log_out("ERROR: couldn't initialize prototypes.\n");
			call_method(obj, 1);
			return NULL;
		}
		call_vtbl(obj, 3, 0x80001020, &p_outPolys, 0x80001021, &p_polysdata, 0x80001022, &p_polysdata_end, 0);

		int width, height;
		gfxEngine__getter(0x80003003, &width, 0x80003004, &height, 0);

		yw->screen_width = width;
		yw->screen_height = height;

		init_list(&yw->field_48);
		init_list(&yw->field_17a0);
		init_list(&yw->field_54);

		if ( !yw_initAttrs(obj, yw, stak) )
		{
			ypa_log_out("yw_main.c/OM_NEW: yw_initAttrs() failed!\n");
			call_method(obj, 1);
			return 0;
		}

		yw->field_130[0] = 3;
		for (int i = 1; i <= 100; i++ )
			yw->field_130[i] = 2;
		for (int i = 101; i <= 200; i++ )
			yw->field_130[i] = 1;
		for (int i = 201; i < 256; i++ )
			yw->field_130[i] = 0;

		for (int j = 0; j < 64; j++)
		{
			for (int i = 0; i < 64; i++)
			{
				yw->sqrt_table[j][i] = sqrt(j * j + i * i);
			}
		}

		yw->cells = (cellArea *)AllocVec(sizeof(cellArea) * yw->sectors_maxX * yw->sectors_maxY, 65537);

		if ( !yw->cells )
		{
			ypa_log_out("yw_main.c/OM_NEW: alloc of cell area failed!\n");
			call_method(obj, 1);
			return NULL;
		}
		if ( !yw_InitSceneRecorder(yw) )
		{
			ypa_log_out("yw_main.c/OM_NEW: init scene recorder failed!\n");
			call_method(obj, 1);
			return NULL;
		}
		if ( !yw_InitTooltips(yw) )
		{
			ypa_log_out("yw_main.c/OM_NEW: yw_InitTooltips() failed!\n");
			call_method(obj, 1);
			return NULL;
		}

		yw->one_game_res = 1;
		yw->shell_default_res = (640 << 12) | 480;
		yw->game_default_res = (640 << 12) | 480;

		if ( !yw_InitLevelNet(yw) )
		{
			ypa_log_out("yw_main.c/OM_NEW: yw_InitLevelNet() failed!\n");
			call_method(obj, 1);
			return NULL;
		}

		yw->field_73CE |= 0x10;

		if ( !yw_InitNetwork(yw) )
		{
			ypa_log_out("yw_main.c/OM_NEW: yw_InitNetwork() failed!\n");
			call_method(obj, 1);
			return NULL;
		}

		yw->str17_NOT_FALSE = 0;
		yw->field_138c = 0;

	}
	else
	{
		ypa_log_out("yw_main.c/OM_NEW: _supermethoda() failed!\n");
		obj = NULL;
	}

	return obj;
}


void ypaworld_func1(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func2(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func3__sub0(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw, stack_vals *stak)
{
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
			switch ( stk->id )
			{
			default:
				break;


			case 0x80002018:
				*(char ***)stk->value = yw->string_pointers;
				break;


			}
			stk++;
		}
	}
}

void ypaworld_func3(NC_STACK_ypaworld *obj, class_stru *zis, stack_vals *stak)
{
	_NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

	ypaworld_func3__sub0(obj, yw, stak);

	call_parent(zis, obj, 3, stak);
}


void ypaworld_func64(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func129(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func130(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func131(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func132(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func133(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func134(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func135(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func136(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func137(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func138(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func139(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func140(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func143(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func144(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func145(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func146(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func147(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func148(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func149(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func150(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func151(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func153(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


size_t ypaworld_func154(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
	_NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

	yw->GameShell = usr;

	usr->p_ypaworld = yw;
	usr->field_46 = 1;

	call_vtbl(obj, 3, 0x80002018, &ypaworld__string_pointers, 0);

	return 1;
}


void ypaworld_func155(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}



void sub_4E6FEC(_NC_STACK_ypaworld *yw)
{
	stru_LevelNet *lvlnet;

	lvlnet = yw->LevelNet;
	if ( lvlnet->ilbm_menu_map )
	{
		delete_class_obj(lvlnet->ilbm_menu_map);
		lvlnet->ilbm_menu_map = NULL;
	}
	if ( lvlnet->ilbm_rollover_map )
	{
		delete_class_obj(lvlnet->ilbm_rollover_map);
		lvlnet->ilbm_rollover_map = NULL;
	}
	if ( lvlnet->ilbm_finished_map )
	{
		delete_class_obj(lvlnet->ilbm_finished_map);
		lvlnet->ilbm_finished_map = NULL;
	}
	if ( lvlnet->ilbm_enabled_map )
	{
		delete_class_obj(lvlnet->ilbm_enabled_map);
		lvlnet->ilbm_enabled_map = NULL;
	}
	if ( lvlnet->ilbm_mask_map )
	{
		delete_class_obj(lvlnet->ilbm_mask_map);
		lvlnet->ilbm_mask_map = NULL;
	}
}


void sb_0x4e75e8__sub1(_NC_STACK_ypaworld *yw, int a2)
{
	int v37 = 1;

	if ( yw->LevelNet->bg_n )
	{
		char buf[256];
		strcpy(buf, get_prefix_replacement("rsrc"));

		set_prefix_replacement("rsrc", "levels:");

		int v38 = 0;
		int v39 = 65535;
		for (int i = 0; i < yw->LevelNet->bg_n; i++)
		{

			int xx = (yw->LevelNet->background_map[i].size_x - yw->screen_width);
			int yy = (yw->LevelNet->background_map[i].size_y - yw->screen_height);

			int sq = sqrt(xx * xx + yy * yy);

			if (sq < v39)
			{
				v38 = i;
				v39 = sq;
			}
		}

		char *menu_map = NULL;
		char *rollover_map = NULL;
		char *mask_map = NULL;
		char *finished_map = NULL;
		char *enabled_map = NULL;

		NC_STACK_ilbm *ilbm_menu_map  = NULL;
		NC_STACK_ilbm *ilbm_rollover_map = NULL;
		NC_STACK_ilbm *ilbm_mask_map = NULL;
		NC_STACK_ilbm *ilbm_finished_map = NULL;
		NC_STACK_ilbm *ilbm_enabled_map = NULL;

		switch ( a2 )
		{
		case 1:
		case 2:
		case 3:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
			menu_map  = yw->LevelNet->menu_map[v38].map_name;
			rollover_map = yw->LevelNet->settings_map[v38].map_name;
			break;
		case 4:
			menu_map  = yw->LevelNet->tut_background_map[v38].map_name;
			mask_map = yw->LevelNet->tut_mask_map[v38].map_name;
			rollover_map = yw->LevelNet->tut_rollover_map[v38].map_name;
			break;
		case 5:
			menu_map  = yw->LevelNet->background_map[v38].map_name;
			rollover_map = yw->LevelNet->rollover_map[v38].map_name;
			finished_map = yw->LevelNet->finished_map[v38].map_name;
			mask_map = yw->LevelNet->mask_map[v38].map_name;
			enabled_map = yw->LevelNet->enabled_map[v38].map_name;
			break;
		default:
			break;
		}

		if ( menu_map )
		{
			ilbm_menu_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, menu_map, 0x80002008, 1, 0x80002009, 1, 0);
			if ( !ilbm_menu_map )
			{
				ypa_log_out("world.ini: Could not load %s\n", menu_map);
				v37 = 0;
			}
		}

		if ( rollover_map )
		{
			ilbm_rollover_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, rollover_map, 0x80002008, 1, 0x80002009, 1, 0);
			if ( !ilbm_rollover_map )
			{
				ypa_log_out("world.ini: Could not load %s\n", rollover_map);
				v37 = 0;
			}
		}

		if ( finished_map )
		{
			ilbm_finished_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, finished_map, 0x80002008, 1, 0x80002009, 1, 0);
			if ( !ilbm_finished_map )
			{
				ypa_log_out("world.ini: Could not load %s\n", finished_map);
				v37 = 0;
			}
		}

		if ( enabled_map )
		{
			ilbm_enabled_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, enabled_map, 0x80002008, 1, 0x80002009, 1, 0);
			if ( !ilbm_enabled_map )
			{
				ypa_log_out("world.ini: Could not load %s\n", enabled_map);
				v37 = 0;
			}
		}
		if ( mask_map )
		{
			ilbm_mask_map = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, mask_map, 0);
			if ( !ilbm_mask_map )
			{
				ypa_log_out("world.ini: Could not load %s\n", mask_map);
				v37 = 0;
			}
		}

		set_prefix_replacement("rsrc", buf);

		if ( !v37 )
		{
			if ( ilbm_menu_map )
			{
				delete_class_obj(ilbm_menu_map);
				ilbm_menu_map = NULL;
			}
			if ( ilbm_rollover_map )
			{
				delete_class_obj(ilbm_rollover_map);
				ilbm_rollover_map = NULL;
			}
			if ( ilbm_finished_map )
			{
				delete_class_obj(ilbm_finished_map);
				ilbm_finished_map = NULL;
			}
			if ( ilbm_enabled_map )
			{
				delete_class_obj(ilbm_enabled_map);
				ilbm_enabled_map = NULL;
			}
			if ( ilbm_mask_map )
			{
				delete_class_obj(ilbm_mask_map);
				ilbm_mask_map = NULL;
			}
		}
		sub_4E6FEC(yw);
		yw->LevelNet->ilbm_menu_map = ilbm_menu_map;
		yw->LevelNet->ilbm_mask_map = ilbm_mask_map;
		yw->LevelNet->ilbm_rollover_map = ilbm_rollover_map;
		yw->LevelNet->ilbm_finished_map = ilbm_finished_map;
		yw->LevelNet->ilbm_enabled_map = ilbm_enabled_map;
	}
}

void sb_0x4e75e8__sub0(_NC_STACK_ypaworld *yw)
{
	lvlnet_t1 regions[256];

	if ( yw->LevelNet->ilbm_mask_map )
	{
		for (int i = 0; i < 256; i++)
		{
			regions[i].field_0 = 10000;
			regions[i].field_4 = 10000;
			regions[i].field_8 = -10000;
			regions[i].field_C = -10000;
		}

		bitmap_intern *bitm;
		call_vtbl(yw->LevelNet->ilbm_mask_map, 3, 0x80002000, &bitm, 0);

		for (int y = 0; y < bitm->height; y++ )
		{
			uint8_t *ln = ((uint8_t *)bitm->buffer + y * bitm->pitch);

			for (int x = 0; x < bitm->width; x++)
			{

				int v7 = ln[x];
				if ( v7 < 256 )
				{
					lvlnet_t1 *v8 = &regions[v7];

					if ( x < v8->field_0 )
						v8->field_0 = x;

					if ( x > v8->field_8 )
						v8->field_8 = x;

					if ( y < v8->field_4 )
						v8->field_4 = y;

					if ( y > v8->field_C )
						v8->field_C = y;
				}
			}
		}

		for (int i = 0; i < 256; i++)
		{
			mapINFO *minf = &yw->LevelNet->mapInfos[i];

			if ( minf->field_0 && minf->field_0 != 4 && regions[i].field_0 != 10000 )
			{
				minf->field_9C.x1 = 2.0 * ((float)(regions[i].field_0) / (float)bitm->width) + -1.0;
				minf->field_9C.x2 = 2.0 * ((float)(regions[i].field_8) / (float)bitm->width) + -1.0;
				minf->field_9C.y1 = 2.0 * ((float)(regions[i].field_4) / (float)bitm->height) + -1.0;
				minf->field_9C.y2 = 2.0 * ((float)(regions[i].field_C) / (float)bitm->height) + -1.0;
			}
			else
			{
				minf->field_9C.x2 = 0;
				minf->field_9C.y1 = 0;
				minf->field_9C.y2 = 0;
				minf->field_9C.x1 = 0;
			}
		}
	}
}

void sb_0x4e75e8(_NC_STACK_ypaworld *yw, int a2)
{
	sb_0x4e75e8__sub1(yw, a2);
	if ( a2 >= 4 && a2 <= 5 )
	{
		yw->field_81AB = 0;
		yw->brief.field_2E68 = 0;
		yw->LevelNet->field_BE38 = 0;

		sb_0x4e75e8__sub0(yw);

		yw->TOD_ID = loadTOD(yw, "tod.def");

		int v6 = yw->TOD_ID + 1;

		if ( (v6 + 2490) > 2510 )
			v6 = 0;
		writeTOD(yw, "tod.def", v6);
	}
}

int ypaworld_func156__sub2(_NC_STACK_ypaworld *yw)
{
	NC_STACK_win3d *win3d;
	gfxEngine__getter(0x8000300D, &win3d, 0);

	int v5[4];
	v5[0] = -(yw->screen_width >> 1);
	v5[2] = yw->screen_width >> 1;
	v5[1] = -(yw->screen_height >> 1);
	v5[3] = yw->screen_height >> 1;

	call_method(win3d, 211, v5);
	sb_0x4e75e8(yw, yw->GameShell->field_46);
	return 1;
}

size_t ypaworld_func156(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
	_NC_STACK_ypaworld *yw = &obj->stack__ypaworld;
	//_yw = usr->p_ypaworld;
	if ( !yw->one_game_res )
	{
//    v247 = yw->shell_default_res;
//    v248 = 0;
//    call_method(yw, 174, &247);
	}

	if ( !yw_LoadSet(yw, 46) )
	{
		ypa_log_out("Unable to load set for shell\n");
		return 0;
	}

	printf("%d\n", yw_LoadSet(yw, 1)); //// HACK TEST LOAD SET

	if ( !ypaworld_func156__sub2(yw) )
	{
		ypa_log_out("Could not init level select stuff!\n");
		return 0;
	}

	usr->titel_button = (NC_STACK_button *)init_get_class("button.class", 0x80001003, 0, 0x80001004, 0, 0x80001005, yw->screen_width, 0x80001006, yw->screen_height, 0);
	if ( !usr->titel_button )
	{
		ypa_log_out("Unable to create Titel-Button-Object\n");
		return 0;
	}

	int v70 = 0;

	button_64_arg btn_64arg;

	btn_64arg.tileset_down = 19;
	btn_64arg.tileset_up = 18;
	btn_64arg.button_type = 1;
	btn_64arg.field_3A = 30;
	btn_64arg.xpos = yw->screen_width * 0.3328125;
	btn_64arg.ypos = yw->screen_height * 0.2291666666666666;
	btn_64arg.width = yw->screen_width / 3;
	btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 80, "GAME");
	btn_64arg.caption2 = 0;
	btn_64arg.field_1C = 0;
	btn_64arg.down_id = 1251;
	btn_64arg.pressed_id = 0;
	btn_64arg.button_id = 1018;
	btn_64arg.up_id = 1024;
	btn_64arg.state = 112;
	btn_64arg.txt_r = yw->iniColors[68].r;
	btn_64arg.txt_g = yw->iniColors[68].g;
	btn_64arg.txt_b = yw->iniColors[68].b;

	if ( call_method(usr->titel_button, 64, &btn_64arg) )
	{
		btn_64arg.ypos = yw->screen_height * 0.3083333333333334;
		btn_64arg.field_1C = 0;
		btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 81, "NETWORK");
		btn_64arg.caption2 = 0;
		btn_64arg.up_id = 1022;
		btn_64arg.pressed_id = 0;
		btn_64arg.down_id = 1251;
		btn_64arg.button_id = 1016;

		if ( call_method(usr->titel_button, 64, &btn_64arg) )
		{
			btn_64arg.xpos = yw->screen_width * 0.3328125;
			btn_64arg.ypos = yw->screen_height * 0.4333333333333334;
			btn_64arg.width = yw->screen_width / 3;
			btn_64arg.field_1C = 0;
			btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 83, "INPUT");
			btn_64arg.caption2 = 0;
			btn_64arg.pressed_id = 0;
			btn_64arg.down_id = 1251;
			btn_64arg.button_id = 1003;
			btn_64arg.up_id = 1007;

			if ( call_method(usr->titel_button, 64, &btn_64arg) )
			{
				btn_64arg.ypos = yw->screen_height * 0.5125;
				btn_64arg.field_1C = 0;
				btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 84, "SETTINGS");
				btn_64arg.caption2 = 0;
				btn_64arg.up_id = 1005;
				btn_64arg.pressed_id = 0;
				btn_64arg.down_id = 1251;
				btn_64arg.button_id = 1004;

				if ( call_method(usr->titel_button, 64, &btn_64arg) )
				{
					btn_64arg.ypos = yw->screen_height * 0.5916666666666667;
					btn_64arg.field_1C = 0;
					btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 85, "PLAYER");
					btn_64arg.caption2 = 0;
					btn_64arg.pressed_id = 0;
					btn_64arg.down_id = 1251;
					btn_64arg.up_id = 1001;
					btn_64arg.button_id = 1001;

					if ( call_method(usr->titel_button, 64, &btn_64arg) )
					{
						btn_64arg.xpos = yw->screen_width * 0.890625;
						btn_64arg.ypos = yw->screen_height * 0.9583333333333334;
						btn_64arg.width = yw->screen_width * 0.1;
						btn_64arg.field_1C = 0;
						btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 86, "LOCALE");
						btn_64arg.caption2 = 0;
						btn_64arg.up_id = 1011;
						btn_64arg.pressed_id = 0;
						btn_64arg.down_id = 1251;
						btn_64arg.button_id = 1008;

						if ( call_method(usr->titel_button, 64, &btn_64arg) )
						{
							btn_64arg.xpos = yw->screen_width * 0.3328125;
							btn_64arg.ypos = yw->screen_height * 0.7166666666666667;
							btn_64arg.width = yw->screen_width / 3;
							btn_64arg.field_1C = 0;
							btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 87, "HELP");
							btn_64arg.caption2 = 0;
							btn_64arg.pressed_id = 0;
							btn_64arg.down_id = 1251;
							btn_64arg.button_id = 1017;
							btn_64arg.up_id = 1025;

							if ( call_method(usr->titel_button, 64, &btn_64arg) )
							{
								btn_64arg.ypos = yw->screen_height * 0.7958333333333333;
								btn_64arg.field_1C = 0;
								btn_64arg.caption = get_lang_string(ypaworld__string_pointers, 88, "QUIT");
								btn_64arg.caption2 = 0;
								btn_64arg.up_id = 1013;
								btn_64arg.pressed_id = 0;
								btn_64arg.down_id = 1251;
								btn_64arg.button_id = 1007;

								if ( call_method(usr->titel_button, 64, &btn_64arg) )
									v70 = 1;
							}
						}
					}
				}
			}
		}
	}

	if ( !v70 )
	{
		ypa_log_out("Unable to add button to Titel\n");
		return 0;
	}

	/*if ( usr->lang_dlls_count <= 1 )
	{
	  v240.butID = 1008;
	  usr->titel_button->baseClass_nucleus.baseClass.class_owner->clvtbl[67].cl_func(
	    usr->titel_button,
	    usr->titel_button->baseClass_nucleus.baseClass.class_owner->clvtbl[67].p_cl,
	    &v240);
	}*/

	int v250 = 1;
	call_method(usr->titel_button, 68, &v250);




	return 1;
}


void ypaworld_func157(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func158__sub4(_NC_STACK_ypaworld *yw, UserData *usr, struC5 *struc5)
{
	if ( usr->field_4A )
		sb_0x4e75e8(yw, usr->field_46);

	if ( yw->GameShell->field_46 == 0 )
	{
		ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_rollover_map);
	}
	else if ( yw->GameShell->field_46 == 1 )
	{
		ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_menu_map);
	}
	else if ( yw->GameShell->field_46 >= 4 && yw->GameShell->field_46 <= 5 )
	{
		//ypaworld_func158__sub4__sub1(yw, usr, struc5);
	}
	else
	{
		ypaworld_func158__sub4__sub0(yw, yw->LevelNet->ilbm_rollover_map);
	}
}


void ypaworld_func158(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
	_NC_STACK_ypaworld *yw = &obj->stack__ypaworld;
	usr->field_0x2fbc = 0;

	call_method(yw->win3d, 257);



	ypaworld_func158__sub4(yw, usr, usr->field_3A);

	call_method(yw->win3d, 215);

	call_method(usr->titel_button, 69, usr->field_3A);
	call_method(usr->titel_button, 70, 0);

	yw->brief.brf_objs.field_0 = 2;
	yw->brief.brf_objs.object_id = 24;

	yw->brief.brf_objs.field_C = 1000;
	yw->brief.field_2E7C += usr->field_3A->period;

	ypaworld_func158__DrawVehicle(yw, &yw->brief, usr->field_3A);

	call_method(yw->win3d, 216);

	call_method(yw->win3d, 258);
}


void ypaworld_func159(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func160(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func161(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func162(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func163(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func164(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func165(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


char * sb_0x471428__sub0(char *a1, const char *a2)
{
	strcpy(a1, a2);

	while ( *a1 )
	{
		if ( *a1 == '\\' )
			*a1 = '\n';
		a1 = CharNext(a1);
	}
	return a1 + 1;
}

int locale_parser(scrCallBack *arg)
{
	_NC_STACK_ypaworld *yw = (_NC_STACK_ypaworld *)arg->dataForStore;

	if ( arg->field_18 )
	{
		if ( !strcasecmp(arg->p1, "end") )
		{
			arg->field_18 = 0;
			return 2;
		}

		int id = atoi(arg->p1);
		int err = 0;

		if ( id < 2600 )
		{
			if ( yw->very_big_array__p_begin < yw->lang_strings__end )
			{
				if ( !yw->string_pointers[id] )
				{
					yw->string_pointers[id] = yw->very_big_array__p_begin;
					yw->very_big_array__p_begin = sb_0x471428__sub0(yw->very_big_array__p_begin, arg->p2);
					err = 0;
				}
				else
				{
					printf("%s %s\n",arg->p1, arg->p2);
					err = 3;
				}
			}
			else
			{
				err = 2;
			}
		}
		else
		{
			err = 1;
		}

		if ( err == 1 )
		{
			ypa_log_out("Locale parser: id [%d]аtoo big.\n", id);
			return 4;
		}
		else if ( err == 2 )
		{
			ypa_log_out("Locale parser: buffer overflow at id [%d].\n", id);
			return 4;
		}
		else if ( err == 3 )
		{
			ypa_log_out("Locale parser: id [%d] already defined.\n", id);
			return 4;
		}

		return 0;
	}
	else if ( !strcasecmp(arg->p1, "begin_locale") )
	{
		arg->field_18 = 1;
		return 1;

	}

	return 3;
}

int load_lang_lng(_NC_STACK_ypaworld *yw, const char *lang)
{
	char buf[128];
	sprintf(buf, "locale/%s.lng", lang);

	scrCallBack pars;
	memset(&pars, 0, sizeof(scrCallBack));

	pars.dataForStore = yw;
	pars.func = locale_parser;

	if ( def_parseFile(buf, 1, &pars, 1) )
		return 1;

	ypa_log_out("ERROR: Could not load language file '%s'!!!\n", buf);
	return 0;
}

size_t ypaworld_func166(NC_STACK_ypaworld *obj, class_stru *zis,const char **langname)
{
	_NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

	sub_4711E0(yw);
	strcpy(yw->lang_name, *langname);

	int v19 = 0;
//// We do not use this dll-garbage
//  char buf[256];
//  char LibFileName[256];
//  sprintf(buf, "locale/%s.dll", yw->lang_name);
//  sub_412810(buf, LibFileName, 256);
//  if ( load_lang_dll(LibFileName, yw->lang_strings, yw->lang_strings__end, yw->string_pointers, 2600) )
//    v19 = 1;

	if ( v19 || load_lang_lng(yw, yw->lang_name) )
	{
		const char *v11 = NULL;

		if ( yw->screen_width >= 512 )
			v11 = get_lang_string(yw->string_pointers_p2, 15, "MS Sans Serif,12,400,0");
		else
			v11 = get_lang_string(yw->string_pointers_p2, 16, "Arial,8,400,0");

		load_font(v11);

		NC_STACK_win3d *win3d = NULL;
		gfxEngine__getter(0x8000300D, &win3d, 0);

		if ( !strcasecmp( get_lang_string(yw->string_pointers_p2, 17, "FALSE") , "FALSE") )
		{
			if ( win3d )
				call_vtbl(win3d, 2, 0x80005002, 0, 0);
			yw->str17_NOT_FALSE = 0;
		}
		else
		{
			if ( win3d )
				call_vtbl(win3d, 2, 0x80005002, 1, 0);
			yw->str17_NOT_FALSE = 1;
		}

		return 1;
	}

	sub_4711E0(yw);
	return 0;
}


void ypaworld_func167(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func168(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func169(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func170(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func171(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func172(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func173(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func174(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func175(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func176(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func177(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func179(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func180(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func181(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func182(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func183(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func184(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func185(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}




class_return ypaworld_class_descr;

class_return * class_set_ypaworld(int, ...)
{
	memset(ypaworld_funcs, 0, sizeof(CLASSFUNC) * 1024);

	ypaworld_funcs[0] = (CLASSFUNC)ypaworld_func0;
	ypaworld_funcs[1] = (CLASSFUNC)ypaworld_func1;
	ypaworld_funcs[2] = (CLASSFUNC)ypaworld_func2;
	ypaworld_funcs[3] = (CLASSFUNC)ypaworld_func3;
	ypaworld_funcs[64] = (CLASSFUNC)ypaworld_func64;
	ypaworld_funcs[129] = (CLASSFUNC)ypaworld_func129;
	ypaworld_funcs[130] = (CLASSFUNC)ypaworld_func130;
	ypaworld_funcs[131] = (CLASSFUNC)ypaworld_func131;
	ypaworld_funcs[132] = (CLASSFUNC)ypaworld_func132;
	ypaworld_funcs[133] = (CLASSFUNC)ypaworld_func133;
	ypaworld_funcs[134] = (CLASSFUNC)ypaworld_func134;
	ypaworld_funcs[135] = (CLASSFUNC)ypaworld_func135;
	ypaworld_funcs[136] = (CLASSFUNC)ypaworld_func136;
	ypaworld_funcs[137] = (CLASSFUNC)ypaworld_func137;
	ypaworld_funcs[138] = (CLASSFUNC)ypaworld_func138;
	ypaworld_funcs[139] = (CLASSFUNC)ypaworld_func139;
	ypaworld_funcs[140] = (CLASSFUNC)ypaworld_func140;
	ypaworld_funcs[143] = (CLASSFUNC)ypaworld_func143;
	ypaworld_funcs[144] = (CLASSFUNC)ypaworld_func144;
	ypaworld_funcs[145] = (CLASSFUNC)ypaworld_func145;
	ypaworld_funcs[146] = (CLASSFUNC)ypaworld_func146;
	ypaworld_funcs[147] = (CLASSFUNC)ypaworld_func147;
	ypaworld_funcs[148] = (CLASSFUNC)ypaworld_func148;
	ypaworld_funcs[149] = (CLASSFUNC)ypaworld_func149;
	ypaworld_funcs[150] = (CLASSFUNC)ypaworld_func150;
	ypaworld_funcs[151] = (CLASSFUNC)ypaworld_func151;
	ypaworld_funcs[153] = (CLASSFUNC)ypaworld_func153;
	ypaworld_funcs[154] = (CLASSFUNC)ypaworld_func154;
	ypaworld_funcs[155] = (CLASSFUNC)ypaworld_func155;
	ypaworld_funcs[156] = (CLASSFUNC)ypaworld_func156;
	ypaworld_funcs[157] = (CLASSFUNC)ypaworld_func157;
	ypaworld_funcs[158] = (CLASSFUNC)ypaworld_func158;
	ypaworld_funcs[159] = (CLASSFUNC)ypaworld_func159;
	ypaworld_funcs[160] = NULL;
	ypaworld_funcs[161] = (CLASSFUNC)ypaworld_func161;
	ypaworld_funcs[162] = (CLASSFUNC)ypaworld_func162;
	ypaworld_funcs[163] = (CLASSFUNC)ypaworld_func163;
	ypaworld_funcs[164] = (CLASSFUNC)ypaworld_func164;
	ypaworld_funcs[165] = (CLASSFUNC)ypaworld_func165;
	ypaworld_funcs[166] = (CLASSFUNC)ypaworld_func166;
	ypaworld_funcs[167] = (CLASSFUNC)ypaworld_func167;
	ypaworld_funcs[168] = (CLASSFUNC)ypaworld_func168;
	ypaworld_funcs[169] = (CLASSFUNC)ypaworld_func169;
	ypaworld_funcs[170] = (CLASSFUNC)ypaworld_func170;
	ypaworld_funcs[171] = (CLASSFUNC)ypaworld_func171;
	ypaworld_funcs[172] = (CLASSFUNC)ypaworld_func172;
	ypaworld_funcs[173] = (CLASSFUNC)ypaworld_func173;
	ypaworld_funcs[174] = (CLASSFUNC)ypaworld_func174;
	ypaworld_funcs[175] = (CLASSFUNC)ypaworld_func175;
	ypaworld_funcs[176] = (CLASSFUNC)ypaworld_func176;
	ypaworld_funcs[177] = (CLASSFUNC)ypaworld_func177;
	ypaworld_funcs[179] = (CLASSFUNC)ypaworld_func179;
	ypaworld_funcs[180] = (CLASSFUNC)ypaworld_func180;
	ypaworld_funcs[181] = (CLASSFUNC)ypaworld_func181;
	ypaworld_funcs[182] = (CLASSFUNC)ypaworld_func182;
	ypaworld_funcs[183] = (CLASSFUNC)ypaworld_func183;
	ypaworld_funcs[184] = (CLASSFUNC)ypaworld_func184;
	ypaworld_funcs[185] = (CLASSFUNC)ypaworld_func185;

	ypaworld_class_descr.parent = "base.class";

	ypaworld_class_descr.vtbl = ypaworld_funcs;
	////ypaworld_class_descr.varSize = sizeof(__NC_STACK_ypaworld);
	ypaworld_class_descr.varSize = sizeof(NC_STACK_ypaworld) - offsetof(NC_STACK_ypaworld, stack__ypaworld); //// HACK
	ypaworld_class_descr.field_A = 0;
	return &ypaworld_class_descr;
}
