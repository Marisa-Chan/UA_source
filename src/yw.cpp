#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "yw.h"

#include "def_parser.h"

#include "yw_internal.h"



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
	yw->field_7492 = 16;
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


void ypaworld_func3(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
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


void ypaworld_func154(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func155(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func156(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func157(NC_STACK_ypaworld *obj, class_stru *zis, void *arg)
{
}


void ypaworld_func158(NC_STACK_ypaworld *obj, class_stru *zis, UserData *usr)
{
	usr->field_0x2fbc = 0;
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
		const char *v11;

		if ( yw->screen_width >= 512 )
			v11 = get_lang_string(yw->string_pointers_p2, 15, "MS Sans Serif,12,400,0");
		else
			v11 = get_lang_string(yw->string_pointers_p2, 16, "Arial,8,400,0");

		load_font(v11);

		NC_STACK_win3d *win3d;
		gfxEngine__getter(0x8000300D, &win3d);

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
