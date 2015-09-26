#include "includes.h"
#include "MC.h"
#include "classes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"

#include "engine_gfx.h"
#include "engine_miles.h"
#include "engine_tform.h"
#include "engine_input.h"

const char MC2RES[] = "MC2RES\\";
const char MC2CLASSES[] = "MC2classes:";
const char MC2ENGINES[] = "MC2engines:";

void init_mc_res_class_engine_strings(MC_TYPE a1, const char *str)
{
	if ( a1 < MC_TYPE_UNKNOWN )
	{
		if ( !str )
		{
			if (a1 == MC_TYPE_RES)
				str = MC2RES;
			else if (a1 == MC_TYPE_CLASS)
				str = MC2CLASSES;
			else if (a1 == MC_TYPE_ENGINE)
				str = MC2ENGINES;
			else
				return;
		}

		strcpy(engines.MC_RES_CLASS_ENGINE[a1], str);
	}
}






void add_to_classes_list(nnode *a1)
{
	AddTail(&classes_list, a1);
}


int set_classes_list()
{
	init_list(&classes_list);

	engines.__AllocCount = 0;
	engines.__AllocSize = 0;
	engines.__AllocMax = 0;
	engines.file_handles = 0;

	add_to_classes_list(&nucleas_class_off);
	add_to_classes_list(&rsrc_class_off);
	add_to_classes_list(&bitmap_class_off);
////	add_to_classes_list(&skeleton_class_off.nodeBase);
////	add_to_classes_list(&ilbm_class_off.nodeBase);
////	add_to_classes_list(&sklt_class_off.nodeBase);
////	add_to_classes_list(&ade_class_off.nodeBase);
////	add_to_classes_list(&area_class_off.nodeBase);
////	add_to_classes_list(&base_class_off.nodeBase);
////	add_to_classes_list(&bmpanim_class_off.nodeBase);
////	add_to_classes_list(&amesh_class_off.nodeBase);
////	add_to_classes_list(&particle_class_off.nodeBase);
////	add_to_classes_list(&embed_class_off.nodeBase);
////	add_to_classes_list(&idev_class_off.nodeBase);
////	add_to_classes_list(&input_class_off.nodeBase);
////	add_to_classes_list(&itimer_class_off.nodeBase);
////	add_to_classes_list(&iwimp_class_off.nodeBase);
////	add_to_classes_list(&sample_class_off.nodeBase);
////	add_to_classes_list(&wav_class_off.nodeBase);
////	add_to_classes_list(&raster_class_off.nodeBase);
////	add_to_classes_list(&display_class_off.nodeBase);
////	add_to_classes_list(&button_class_off.nodeBase);
////	add_to_classes_list(&requester_class_off.nodeBase);
////	add_to_classes_list(&network_class_off.nodeBase);
////	add_to_classes_list(&windd_class_off.nodeBase);
////	add_to_classes_list(&win3d_class_off.nodeBase);
////	add_to_classes_list(&winp_class_off.nodeBase);
////	add_to_classes_list(&wintimer_class_off.nodeBase);
////	add_to_classes_list(&windp_class_off.nodeBase);
	add_to_classes_list(&gfx_engine_off);
	add_to_classes_list(&tform_engine_off);
	add_to_classes_list(&input_engine_off);
	add_to_classes_list(&miles_engine_off);
////	add_to_classes_list(&ypaworld_class_off.nodeBase);
////	add_to_classes_list(&ypabact_class_off.nodeBase);
////	add_to_classes_list(&ypatank_class_off.nodeBase);
////	add_to_classes_list(&yparobo_class_off.nodeBase);
////	add_to_classes_list(&ypamissile_class_off.nodeBase);
////	add_to_classes_list(&ypaflyer_class_off.nodeBase);
////	add_to_classes_list(&ypacar_class_off.nodeBase);
////	add_to_classes_list(&ypaufo_class_off.nodeBase);
////	add_to_classes_list(&ypagun_class_off.nodeBase);
	return 1;
}



stored_functions_engine *init_engine(unsigned int engineID)
{
	unk_class *unkClss = NULL;
	stored_functions_engine **engineFuncs = NULL; // ecx@8

	char v10[256]; // [sp+0h] [bp-1A8h]@14
	char v11[128]; // [sp+100h] [bp-A8h]@1

	memset(v11, 0, 128);
	strcpy(v11, "dummy.engine");

	key_value_stru engine;
	engine.key = "dummy.engine";
	engine.key_type = KEY_TYPE_STRING1;
	engine.value.pval = v11;

	if (engineID == MC2_GFX_ENGINE)
	{
		unkClss = &engines.gfx_engine__unk_class;
		engineFuncs = &engines.setted_gfx_engine;
		engine.key = "gfx.engine";
	}
	else if (engineID == MC2_AUDIO_ENGINE)
	{
		unkClss = &engines.audio_engine__unk_class;
		engineFuncs = &engines.setted_audio_engine;
		engine.key = "audio.engine";
	}
	else if (engineID == MC2_INPUT_ENGINE)
	{
		unkClss = &engines.input_engine__unk_class;
		engineFuncs = &engines.setted_input_engine;
		engine.key = "input.engine";
	}
	else if (engineID == MC2_TFORM_ENGINE)
	{
		unkClss = &engines.tform_engine__unk_class;
		engineFuncs = &engines.setted_tform_engine;
		engine.key = "tform.engine";
	}

	if ( get_keyvalue_from_ini(0, &engine, 1u) )
	{
		strcpy(v10, "MC2engines:");
		strcat(v10, (char *)engine.value.pval);

		stored_functions_engine *engClass = (stored_functions_engine *)find_class(v10, unkClss);
		if ( engClass )
		{
			*engineFuncs = engClass;
			if ( engClass->init(0x80000001, &stru_51363C, 0x80000002, &engines, 0) )
				return engClass;
			else
				return NULL;
		}
		else
            ypa_log_out("Can't find %s\n", v10);
	}
	return NULL;
}


void deinit_engine(unsigned int engineID)
{
////	unk_class *klass = NULL;
	stored_functions_engine *engine = NULL;

	switch(engineID)
	{
	case MC2_GFX_ENGINE:
////		klass = &engines.gfx_engine__unk_class;
		engine = engines.setted_gfx_engine;
		break;
	case MC2_AUDIO_ENGINE:
////		klass = &engines.audio_engine__unk_class;
		engine = engines.setted_audio_engine;
		break;
	case MC2_INPUT_ENGINE:
////		klass = &engines.input_engine__unk_class;
		engine = engines.setted_input_engine;
		break;
	case MC2_TFORM_ENGINE:
////		klass = &engines.tform_engine__unk_class;
		engine = engines.setted_tform_engine;
		break;
	default:
		break;
	}

	if ( engine )
	{
////		engine->deinit(klass);
		engine->deinit();
	}
}


void sb_0x411c08__sub0()
{
	ypa_log_out("Nucleus shutdown:\n");
	ypa_log_out("    __AllocCount = %d\n", engines.__AllocCount);
	ypa_log_out("    __AllocSize  = %d\n", engines.__AllocSize);
	ypa_log_out("    __AllocMax   = %d\n", engines.__AllocMax);
	ypa_log_out("    __FOpenCount = %d\n", engines.file_handles);
}

void sb_0x411c08()
{
	while ( 1 )
	{
		void *tmp = RemHead(&engines.stru_525D68);
		if ( !tmp )
			break;
		nc_FreeMem(tmp);
	}
	sb_0x411c08__sub0();
}
