#define GLOBAL
#include "includes.h"
#include "engine_gfx.h"
#include "bitmap.h"
#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"
#include "ilbm.h"





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

void draw_splashScreen( NC_STACK_ilbm *splashScreen)
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
			call_method(win3d, 216);
			call_method(win3d, 258);
			call_method(win3d, 257);
			call_method(win3d, 215);
			call_method(win3d, 202, &a4);
			call_method(win3d, 216);
			call_method(win3d, 258);
		}
	}
}


//// TEST FUNCS ////


int sb_0x411324()
{
	return 0;
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
	signed int result; // eax@2
////	windd__window_params *v2; // [sp+0h] [bp-24h]@11
	void *v2;
	int v3; // [sp+4h] [bp-20h]@11
	int v4; // [sp+8h] [bp-1Ch]@11
	int v5; // [sp+Ch] [bp-18h]@11

////	ypaworld = 0;
////	dword_520400 = 0;
////	memset(&userdata, 0, 0x54A3);
////	memset(&stru_5258C0, 0, 197);
////	memset(&dword_520404, 0, 20);
	point_to_off_51363C = init_classesLists_and_variables();
	if ( point_to_off_51363C )
	{
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
		if ( paudio_engine )
		{
			if ( pgfx_engine )
			{
				if ( ptform_engine )
				{
					if ( pinput_engine )
					{
						pgfx_engine->getter(0x80003003, &v5, 0x80003004, &v4, 0x80003002, &v3, 0x80003007, &v2, 0);
						pinput_engine->setter(0x80001004, v5, 0x80001005, v4, 0x80001006, v3, 0x80001007, v2, 0);
						result = 1;
					}
					else
					{
						sub_412038("Couldn't open input engine!");
						deinit_globl_engines();
						result = 0;
					}
				}
				else
				{
					sub_412038("Couldn't open tform engine!");
					deinit_globl_engines();
					result = 0;
				}
			}
			else
			{
				sub_412038("Couldn't open gfx engine!");
				deinit_globl_engines();
				result = 0;
			}
		}
		else
		{
			sub_412038("Couldn't open audio engine!");
			deinit_globl_engines();
			result = 0;
		}
	}
	else
	{
		sub_412038("Couldn't open Nucleus!");
		deinit_globl_engines();
		result = 0;
	}
	return result;
}

int WinMain__sub0()
{
	if ( WinMain__sub0__sub0() )
	{
////		if ( WinMain::sub0::sub1() )
////			return 1;
////		deinit_globl_engines();
	}


//// TEST PLACE
    init_mc_res_class_engine_strings(MC_TYPE_RES, "rsrc:");
	set_prefix_replacement("rsrc", "mc2res");
	set_prefix_replacement("data", "Data");
	set_prefix_replacement("save", "Save");
	set_prefix_replacement("help", "Help");
	set_prefix_replacement("mov", "Data:mov");
	set_prefix_replacement("levels", "Levels");
	set_prefix_replacement("mbpix", "levels:mbpix");

	NC_STACK_ilbm *disk_img = loadDisk_screen(640);
	if (disk_img)
	{
		draw_splashScreen(disk_img);
		delete_class_obj(disk_img);
	}
	return 1;
//// TEST PLACE

	return 0;
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
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
	while ( sb_0x411324() || true);

	if ( ghWnd )
		DestroyWindow(ghWnd);
////	sub_4113E8();

	CoUninitialize();
	return Msg.wParam;

}
