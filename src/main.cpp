#define GLOBAL
#include "includes.h"
#include "engine_gfx.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "ilbm.h"

#include "font.h"
#include "yw.h"

#include "button.h"

#include "engine_gfx.h"
#include "engine_input.h"


char tttast[50];
NC_STACK_button *btn;
NC_STACK_ilbm *disk_img;


//// TEST FUNCS

NC_STACK_ilbm * loadDisk_screen(int  wdth)
{
	const char *diskImage = "disk320.ilbm";

	if (wdth > 360)
	{
		if (wdth > 600)
			diskImage = "disk640.ilbm";
		else
			diskImage = "disk512.ilbm";
	}

	char rsrc_def[128];

	const char *pref = get_prefix_replacement("rsrc");
	strcpy(rsrc_def, pref);

	set_prefix_replacement("rsrc", "data:mc2res");

	NC_STACK_ilbm *ilbm = (NC_STACK_ilbm *)init_get_class("ilbm.class", 0x80001000, diskImage, 0x80002008, 1, 0x80002009, 1, 0);
	set_prefix_replacement("rsrc", rsrc_def);

	return ilbm;
}

char * sub_4515D8(char *curpos, char *str, int width_space, char )
{
	if ( width_space > 0 )
	{
		fntcmd_copy_position(&curpos);

		fntcmd_add_txt(&curpos, width_space, 1, str);
	}
	return curpos;
}

void splashScreen_OutTextTMP(NC_STACK_win3d *w3d, const char *txt, int x, int y)
{
	char cmdbuf[2048];
	char txtbuf[256];

	if ( txt )
	{
		char *v7 = (char *)cmdbuf;
		fntcmd_select_tileset(&v7, 15);
		fntcmd_set_center_xpos(&v7, x);
		fntcmd_set_ypos(&v7, y);
		fntcmd_set_txtColor(&v7, 255, 255, 255);

		strcpy(txtbuf, txt);

		v7 = sub_4515D8(v7, txtbuf, 640 - x, ' ');

		fntcmd_next_line(&v7);

		v7 = sub_4515D8(v7, txtbuf, 640 - x, ' ');

		fntcmd_set_end(&v7);

		w3d_a209 v16;
		v16.field_0 = cmdbuf;
		v16.field_4 = 0;
		call_method(w3d, 209, &v16);
	}
}

void draw_splashScreen( NC_STACK_ilbm *splashScreen, NC_STACK_button *btn)
{
	if ( splashScreen )
	{
		rstr_arg204 a4;
		call_vtbl(splashScreen, 3, 0x80002000, &a4.pbitm, 0); // bitmap_func3

		a4.float4 = -1.0;
		a4.float8 = -1.0;
		a4.floatC = 1.0;
		a4.float10 = 1.0;

		a4.float14 = -1.0;
		a4.float18 = -1.0;
		a4.float1C = 1.0;
		a4.float20 = 1.0;

		NC_STACK_win3d *win3d = NULL;

		pgfx_engine->getter(0x8000300D, &win3d, 0);

		if ( win3d )
		{
			call_method(win3d, 257);
			call_method(win3d, 215);
			call_method(win3d, 202, &a4);
			splashScreen_OutTextTMP(win3d, "AZAZAZA", 640 / 7, 480 / 5);
			call_method(btn, 70);
			call_method(win3d, 216);
			call_method(win3d, 258);
		}
	}
}

int sb_0x411324__sub0()
{
	printf("MAKE ME %s\n","sb_0x411324__sub0");
	return 0;
}

int sb_0x411324__sub2()
{
	printf("MAKE ME %s\n","sb_0x411324__sub0");
	return 0;
}

int sb_0x411324__sub1()
{
	userdata.field_42 = world_update_arg.field_0;
	userdata.field_3E = world_update_arg.field_4;
	userdata.field_3A = &input_states;

	call_method(ypaworld, 158, &userdata);

	if ( userdata.field_0x2fbc == 1 )
		return 0;
	else if ( userdata.field_0x2fbc == 2 )
	{

	}
	else if ( userdata.field_0x2fbc == 3 )
	{

	}
	else if ( userdata.field_0x2fbc == 4 )
	{

	}
	else if ( userdata.field_0x2fbc == 5 )
	{

	}

	return 1;
}


int sb_0x411324()
{
	memset(&input_states, 0, sizeof(struC5));
	sub_412D28(&input_states);

	if ( userdata.field_0x10 )
	{
		input_states.period = 1;
		userdata.field_0x10 = 0;
	}

	world_update_arg.field_4 = input_states.period + 1;
	world_update_arg.field_8 = &input_states;

	world_update_arg.field_0 += input_states.period + 1;
	input_states.period++;

	if ( dword_520400 == 1 )
	{
		return sb_0x411324__sub1();
	}
	else if ( dword_520400 == 2 )
	{
		return sb_0x411324__sub0();
	}
	else if ( dword_520400 == 3 )
	{
		return sb_0x411324__sub2();
	}
	return 1;
}

glob_funcs *init_classesLists_and_variables()
{
	memset(&engines, 0, sizeof(engines));
	init_list(&engines.allocated_class_list);
	init_list(&engines.stru_525D68);
	init_mc_res_class_engine_strings(MC_TYPE_RES, NULL);
	init_mc_res_class_engine_strings(MC_TYPE_CLASS, NULL);
	init_mc_res_class_engine_strings(MC_TYPE_ENGINE, NULL);
	ypa_log__ypa_general_log();
	init_d3dlog();
	init_dinputlog();
	if ( set_classes_list() )
	{
		return &off_51363C;
	}
	else
	{
		return NULL;
	}
}

int add_to_params_list(const char *a1)
{
	if ( engines.some_params_count >= 31 )
		return 0;

	engines.some_params_pointers[engines.some_params_count] = a1;
	engines.some_params_count++;
	return 1;
}



void deinit_globl_engines()
{
	if ( ptform_engine )
		deinit_engine(MC2_TFORM_ENGINE);
	if ( pinput_engine )
		deinit_engine(MC2_INPUT_ENGINE);
	if ( paudio_engine )
		deinit_engine(MC2_AUDIO_ENGINE);
	if ( pgfx_engine )
		deinit_engine(MC2_GFX_ENGINE);
	if ( point_to_off_51363C )
		sb_0x411c08();
}

int WinMain__sub0__sub0()
{
	ypaworld = 0;
	dword_520400 = 0;
	memset(&userdata, 0, sizeof(UserData));
	memset(&input_states, 0, sizeof(struC5));
	memset(&world_update_arg, 0, sizeof(base_64arg));

	point_to_off_51363C = init_classesLists_and_variables();
	if ( !point_to_off_51363C )
	{
		sub_412038("Couldn't open Nucleus!");
		deinit_globl_engines();
		return 0;
	}

	add_to_params_list("gfx.display  = win3d.class");
	add_to_params_list("gfx.display2 = windd.class");
	add_to_params_list("gfx.engine     = gfx.engine");
	add_to_params_list("tform.engine   = tform_ng.engine");
	add_to_params_list("input.engine   = input.engine");
	add_to_params_list("input.wimp     = winp");
	add_to_params_list("input.keyboard = winp");
	add_to_params_list("input.slider[10] = winp:mousex");
	add_to_params_list("input.slider[11] = winp:mousey");
	add_to_params_list("input.slider[12] = winp:joyx winp:joyrudder");
	add_to_params_list("input.slider[13] = winp:joyy");
	add_to_params_list("input.slider[14] = winp:joythrottle");
	add_to_params_list("input.slider[15] = winp:joyhatx");
	add_to_params_list("input.slider[16] = winp:joyhaty");
	add_to_params_list("input.button[16] = winp:joyb0");
	add_to_params_list("input.button[17] = winp:joyb1");
	add_to_params_list("input.button[18] = winp:joyb2");
	add_to_params_list("input.button[19] = winp:joyb3");
	add_to_params_list("input.button[20] = winp:joyb4");
	add_to_params_list("input.button[21] = winp:joyb5");
	add_to_params_list("input.button[22] = winp:joyb6");
	add_to_params_list("input.button[23] = winp:joyb7");
	pgfx_engine = init_engine(MC2_GFX_ENGINE);
	paudio_engine = init_engine(MC2_AUDIO_ENGINE);
	pinput_engine = init_engine(MC2_INPUT_ENGINE);
	ptform_engine = init_engine(MC2_TFORM_ENGINE);

	if ( !paudio_engine )
	{
		sub_412038("Couldn't open audio engine!");
		deinit_globl_engines();
		return 0;
	}

	if ( !pgfx_engine )
	{
		sub_412038("Couldn't open gfx engine!");
		deinit_globl_engines();
		return 0;
	}
	if ( !ptform_engine )
	{
		sub_412038("Couldn't open tform engine!");
		deinit_globl_engines();
		return 0;
	}
	if ( !pinput_engine )
	{
		sub_412038("Couldn't open input engine!");
		deinit_globl_engines();
		return 0;
	}

////HACK, don't use not used variables
//	int width, height;
//    windd__window_params *window_p;
//    size_t v3;
//
//	pgfx_engine->getter(0x80003003, &width, 0x80003004, &height, 0x80003002, &v3, 0x80003007, &window_p, 0);
//	pinput_engine->setter(0x80001004, width, 0x80001005, height, 0x80001006, v3, 0x80001007, window_p, 0);

	windd__window_params *window_p;
	pgfx_engine->getter(0x80003007, &window_p, 0);
	pinput_engine->setter(0x80001007, window_p, 0);

	return 1;
}

int yw_initGameWithSettings()
{
	printf("MAKE ME %s\n","yw_initGameWithSettings");
	return 1;
}

void ReadSnapsDir()
{
	printf("MAKE ME %s\n","ReadSnapsDir");
}

int WinMain__sub0__sub1()
{
	char buildDate[256];
	strcpy(buildDate, "Jul 09 1998  23:52:47");
//    strcpy(buildDate, __DATE__);
//    strcat(buildDate, " ");
//    strcat(buildDate, __TIME__);

	ypaworld = (NC_STACK_ypaworld *)init_get_class("ypaworld.class", 0x8000201D, buildDate, 0);
	if ( !ypaworld )
	{
		ypa_log_out("Unable to init ypaworld.class\n");
		return 0;
	}

	if ( !call_method(ypaworld, 154, &userdata) )
	{
		ypa_log_out("Unable to init shell structure\n");
		return 0;
	}

	if ( !yw_initGameWithSettings() )
	{
		ypa_log_out("Unable to init game with default settings\n");
		return 0;
	}

	if ( !userdata.field_0x0 && !call_method(ypaworld, 156, &userdata))
	{
		ypa_log_out("Error: Unable to open Gameshell\n");
		return 0;
	}

	dword_520400 = 1;
	ReadSnapsDir();

	return 1;
}

int WinMain__sub0()
{
	if ( WinMain__sub0__sub0() )
	{
		if ( WinMain__sub0__sub1() )
			return 1;
		deinit_globl_engines();
	}


////// TEST PLACE
//	init_mc_res_class_engine_strings(MC_TYPE_RES, "rsrc:");
//	set_prefix_replacement("rsrc", "mc2res");
//	set_prefix_replacement("data", "Data");
//	set_prefix_replacement("save", "Save");
//	set_prefix_replacement("help", "Help");
//	set_prefix_replacement("mov", "Data:mov");
//	set_prefix_replacement("levels", "Levels");
//	set_prefix_replacement("mbpix", "levels:mbpix");
//
//	disk_img = loadDisk_screen(640);
//
//	set_prefix_replacement("rsrc", "data:set46");
//	_NC_STACK_ypaworld yw;
//	yw.screen_width = 640;
//	yw.screen_height = 480;
//	load_fonts_and_icons(&yw);
//
//	btn = (NC_STACK_button *)init_get_class("button.class", 0x80001003, 0, 0x80001004, 0, 0x80001005, yw.screen_width, 0x80001006, yw.screen_height, 0);
//
//	button_64_arg v250;
//	v250.tileset_down = 16;
//	v250.tileset_up = 16;
//	v250.button_type = 3;
//	v250.field_3A = 16;
//	v250.xpos = 0;
//	v250.ypos = 8;
//	v250.width = yw.screen_width;
//	v250.caption = "Fuer den Kauf dieses erzgebirgischen Qualitaetsspielzeuges bedanken sich";
//	v250.caption2 = 0;
//	v250.field_1C = 0;
//	v250.down_id = 0; //downed
//	v250.pressed_id = 0;
//	v250.button_id = 2; //tip
//	v250.up_id = 0; //upped
//	v250.state = 96;
//	v250.txt_r = 180;
//	v250.txt_g = 221;
//	v250.txt_b = 221;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Bernd Beyreuther,";
//	v250.ypos = 12 * 2;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Andre 'Floh' Weissflog, Andreas Flemming,";
//	v250.ypos = 12 * 3;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Stefan 'Metzel Hetzel' Karau, Sylvius Lack,";
//	v250.ypos = 12 * 4;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Dietmar 'Didi' Koebelin, Nico Nitsch, Steffen Priebus, ";
//	v250.ypos = 12 * 5;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Stefan Warias, Henrik Volkening und";
//	v250.ypos = 12 * 6;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Uta Kapp";
//	v250.ypos = 12 * 7;
//	call_method(btn, 64, &v250);
//
//	v250.caption = " ";
//	v250.ypos = 12* 8;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Unser Dank gilt:";
//	v250.ypos = 12 * 9;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "dem gesamten Microsoft Team, besonders";
//	v250.ypos = 12 * 10;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Michael Lyons, Jonathan Sposato und Earnest Yuen";
//	v250.ypos = 12 * 11;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "weiterhin";
//	v250.ypos = 12 * 12;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Robert Birker, Andre 'Goetz' Blechschmidt, Jan Blechschmidt, Stephan Bludau,";
//	v250.ypos = 12 * 13;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "Andre Kunth, Markus Lorenz, Dirk Mansbart";
//	v250.ypos = 12 * 14;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "und natuerlich";
//	v250.ypos = 12 * 15;
//	call_method(btn, 64, &v250);
//
//	v250.caption = "        GoldEd - dPaint - SAS/C";
//	v250.ypos = 12 * 16;
//	call_method(btn, 64, &v250);
//
//	int v270 = 1;
//	call_method(btn, 68, &v270);
//
//	draw_splashScreen(disk_img, btn);
//
//	return 1;
////// TEST PLACE

	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE , LPSTR lpCmdLine, int nCmdShow)
{
	struct tagMSG Msg; // [sp+0h] [bp-28h]@8

	ghInstance = hInstance;
	gCmdShow = nCmdShow;
	ghWnd = 0;
	gpcmdline = lpCmdLine;

	HANDLE UAMUTEX = CreateMutex(0, 0, "UA Running Test Mutex");

	if ( UAMUTEX && GetLastError() == ERROR_ALREADY_EXISTS )
	{
		CloseHandle(UAMUTEX);
		return 0;
	}


	CoInitialize(NULL);

	strcpy(cmdline_copy, gpcmdline);

	if ( !WinMain__sub0() )
	{
		CoUninitialize();
		if ( UAMUTEX )
			CloseHandle(UAMUTEX);
		return 0;
	}

	do
	{
		while ( 1 )
		{
			while ( PeekMessage(&Msg, 0, 0, 0, 0) )
			{
				if ( !GetMessage(&Msg, 0, 0, 0) )
				{
////					sub_4113E8();
					CoUninitialize();
					return Msg.wParam;
				}
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			if ( !IsIconic(ghWnd) )
				break;
			WaitMessage();
		}
	}
	while ( sb_0x411324());

	if ( ghWnd )
		DestroyWindow(ghWnd);
////	sub_4113E8();

	CoUninitialize();
	return Msg.wParam;

}
