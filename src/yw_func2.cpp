#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"
#include "font.h"
#include <math.h>

extern Key_stru keySS[256];
extern char **ypaworld__string_pointers;

void sb_0x4eb94c__sub0(_NC_STACK_ypaworld *yw, unsigned int obj_id, int a3, xyz *pos, base77Func *arg)
{
	brf_obj *brobj = &yw->brief.brf_objs + obj_id;

	NC_STACK_base *model_base = yw->vhcls_models[ yw->VhclProtos[ brobj->object_id ].vp_normal ].base;
	call_vtbl(model_base, 2, 0x80001004, 16000, 0);
	call_vtbl(model_base, 2, 0x80001023, 100, 0);

	flag_xyz tmp;
	tmp.flag = 7;
	tmp.x = pos->sx;
	tmp.y = pos->sy;
	tmp.z = pos->sz;

	call_method(model_base, 68, &tmp);

	if ( obj_id >= 3 )
	{
		brobj->field_8 -= (arg->field_0 / 5);
		if ( brobj->field_8 < 0 )
			brobj->field_8 += 360;
	}
	else
	{
		brobj->field_8 += (arg->field_0 / 5);
		if ( brobj->field_8 >= 360 )
			brobj->field_8 -= 360;
	}

	flag_xyz2 rot;
	rot.flag = 7;
	rot.x = a3 + 10;
	rot.y = brobj->field_8;
	rot.z = 0;

	call_method(model_base, 70, &rot);
	//printf("Try DRAW %d\n", (int)model_base);
	call_method(model_base, 77, arg); //Draw vehicle
}

void sb_0x4eb94c__sub1(_NC_STACK_ypaworld *yw, unsigned int obj_id, int rot, xyz *pos, base77Func *arg)
{
	brf_obj *brobj = &yw->brief.brf_objs + obj_id;

	secType *secType = &yw->secTypes[brobj->object_id];

	NC_STACK_base *v7 = yw->vhcls_models[0].base;

	if ( obj_id >= 3 )
	{
		brobj->field_8 -= (arg->field_0 / 5);
		if ( brobj->field_8 < 0 )
			brobj->field_8 += 360;
	}
	else
	{
		brobj->field_8 += (arg->field_0 / 5);
		if ( brobj->field_8 >= 360 )
			brobj->field_8 -= 360;
	}

	flag_xyz2 v17;
	v17.flag = 7;
	v17.x = rot + 10;
	v17.y = brobj->field_8;
	v17.z = 0;

	call_method(v7, 70, &v17);

	base_1c_struct *p3d;
	call_vtbl(v7, 3, 0x80001019, &p3d, 0);

	int first;
	int demens;

	if ( secType->field_0 == 1 )
	{
		first = 0;
		demens = 1;
	}
	else
	{
		first = -1;
		demens = 3;
	}

	int v22 = first;
	for (int i = 0; i < demens; i++)
	{
		int v30 = first;
		for (int j = 0; j < demens; j++)
		{
			float v13 = v30 * 300.0;
			float v14 = v22 * 300.0;

			flag_xyz v16;
			v16.flag = 7;
			v16.x = p3d->scale_rotation.m00 * v13 + pos->sx + 0.0 * p3d->scale_rotation.m01 + p3d->scale_rotation.m02 * v14;
			v16.y = p3d->scale_rotation.m10 * v13 + pos->sy + 0.0 * p3d->scale_rotation.m11 + p3d->scale_rotation.m12 * v14;
			v16.z = p3d->scale_rotation.m20 * v13 + pos->sz + 0.0 * p3d->scale_rotation.m21 + p3d->scale_rotation.m22 * v14;

			NC_STACK_base *lego = yw->legos[ secType->field_4[j][i]->field_4 ].base;
			call_vtbl(lego, 2, 0x80001024, 0, 0);
			call_vtbl(lego, 2, 0x80001004, 16000, 0);
			call_vtbl(lego, 2, 0x80001023, 100, 0);

			call_method(lego, 70, &v17);
			call_method(lego, 68, &v16);
			call_method(lego, 77, arg);

			v30++;
		}
		v22++;
	}
}

void sb_0x4eb94c(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc, int object_id, int a5)
{
	brf_obj *brobj = &brf->brf_objs + object_id;

	brf->field_4174.field_0 = struc->period;
	brf->field_4174.field_4 = brf->field_2E7C;
	brf->field_4174.field_1C = 1;

	base_1c_struct v14;
	memset(&v14, 0, sizeof(base_1c_struct));
	v14.scale_x = 1.0;
	v14.scale_y = 1.0;
	v14.scale_z = 1.0;
	v14.scale_rotation.m00 = 1.0;
	v14.scale_rotation.m01 = 0;
	v14.scale_rotation.m02 = 0;
	v14.scale_rotation.m10 = 0;
	v14.scale_rotation.m11 = 1.0;
	v14.scale_rotation.m12 = 0;
	v14.scale_rotation.m20 = 0;
	v14.scale_rotation.m21 = 0;
	v14.scale_rotation.m22 = 1.0;

	sub_430A20(&v14);
	sub_430A38(&v14);

	xyz pos;

	if ( brobj->field_0 )
	{
		pos.sx = (brobj->field_18 + brobj->field_10) * 0.5;
		pos.sy = (brobj->field_1C + brobj->field_14) * 0.5;

		float v16;
		float v17;
		float v18;
		int rot;

		if ( brobj->field_0 == 1 )
		{
			v16 = 9600.0;
			v17 = 3600.0;
		}
		else if ( brobj->field_0 == 2 )
		{
			float radius = yw->VhclProtos[brobj->object_id].radius;

			v17 = radius * 7.0;
			v16 = radius * 32.0;
		}

		if ( a5 >= 500 )
		{
			v18 = v17;
			rot = 0;
		}
		else
		{
			v18 = v16 + (v17 - v16) * a5 / 500.0;
			rot = -90 * a5 / 500 + 90;
		}

		pos.sz = v18;
		pos.sy = pos.sy * v18;
		pos.sx = pos.sx * v18;

		if ( brobj->field_0 == 1 )
		{
			sb_0x4eb94c__sub1(yw, object_id, rot, &pos, &brf->field_4174);
		}
		else if ( brobj->field_0 == 2 )
		{
			sb_0x4eb94c__sub0(yw, object_id, rot, &pos, &brf->field_4174);
		}
	}
}

int sub_4D7BFC(const void *a1, const void *a2)
{
	return ((const polys*)a1)->range - ((const polys*)a2)->range;
}


void ypaworld_func158__DrawVehicle(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc)
{
	call_method(yw->win3d, 213, 0);

	brf->field_4174.field_0 = 1;
	brf->field_4174.field_4 = 1;
	brf->field_4174.field_14 = 0;
	brf->field_4174.field_18 = 1200;
	brf->field_4174.field_1C = 1;
	brf->field_4174.field_20 = 17.0;
	brf->field_4174.field_24 = 32000.0;
	brf->field_4174.curOutPoly = p_outPolys;
	brf->field_4174.polysData = p_polysdata;
	brf->field_4174.polysData_end = p_polysdata_end;

	if ( brf->brf_objs.field_0 )
	{
		int v7 = brf->field_2E7C - brf->brf_objs.field_C;
		if ( v7 > 50 )
			sb_0x4eb94c(yw, brf, struc, 0, v7 - 50);
	}

	int v8 = brf->field_4174.curOutPoly - p_outPolys;

	if ( v8 > 1 )
		qsort(p_outPolys, brf->field_4174.curOutPoly - p_outPolys, sizeof(polys), sub_4D7BFC);

	for (int i = 0; i < v8; i++)
	{
		polysDat *pol = p_outPolys[i].data;
		pol->render_func(&pol->datSub);
	}

	call_method(yw->win3d, 214, 0);
}



int yw_draw_input_list(_NC_STACK_ypaworld *yw, UserData *usr)
{
	lstvw_updlimits(yw, &usr->input_listview, -2, -2);
	win3d_select_tileset(0);

	char *v4 = lstvw_up_border(yw, &usr->input_listview, usr->input_listview.data_cmdbuf, 0, "uvw");

	for (int i = 1; i <= usr->input_listview.element_count; i++ )
	{
		int v24 = i + usr->input_listview.scroll_pos;
		if ( usr->keyConfig[i - 1].slider_name )
		{
			listview_t1 a1a[2];

			//v5 = v24;
			memset(a1a, 0, sizeof(a1a));

			int v33;
			int v31;
			int v32;
			int v30;

			if ( v24 == usr->field_D36 )
			{
				v30 = 98;
				v31 = 100;
				v32 = 9;
				v33 = 99;
			}
			else
			{
				v30 = 102;
				v31 = 102;
				v32 = 0;
				v33 = 102;
			}

			int v34 = usr->input_listview.width - 2 * usr->p_ypaworld->font_default_w__b + 1;

			char v19[200];

			if ( usr->keyConfig[ v24 - 1 ].inp_type == 2 )
			{
				const char *nm1, *a4;
				if ( usr->keyConfig[ v24 - 1 ].slider_neg )
					nm1 = keySS[usr->keyConfig[ v24 - 1 ].slider_neg].title_by_language;
				else
					nm1 = "-";

				if ( usr->keyConfig[ v24 - 1 ].KeyCode )
					a4 = keySS[ usr->keyConfig[ v24 - 1 ].KeyCode ].title_by_language;
				else
					a4 = "-";

				if ( usr->keyConfig[ v24 - 1 ].field_10 & 1 )
					a4 = get_lang_string(ypaworld__string_pointers, 308, "?");

				if ( usr->keyConfig[ v24 - 1 ].field_10 & 2 )
					nm1 = get_lang_string(ypaworld__string_pointers, 308, "?");

				sprintf(v19, "%s/%s", nm1, a4);
			}
			else
			{
				const char *v8;

				if ( usr->keyConfig[ v24 - 1 ].KeyCode )
					v8 = keySS[ usr->keyConfig[ v24 - 1 ].KeyCode ].title_by_language;
				else
					v8 = "-";

				if ( usr->keyConfig[ v24 - 1 ].field_10 & 1 )
					v8 = get_lang_string(ypaworld__string_pointers, 308, "?");

				sprintf(v19, "%s", v8);
			}

			a1a[0].txt = usr->keyConfig[v24 - 1].slider_name;
			a1a[0].field_width = v34 * 0.68;
			a1a[0].tileset_id = v32;
			a1a[0].bkg_tile = v33;
			a1a[0].left_tile = v30;
			a1a[0].right_tile = 0;
			a1a[0].flags = 37;

			a1a[1].txt = v19;
			a1a[1].field_width = v34 - v34 * 0.68;
			a1a[1].tileset_id = v32;
			a1a[1].bkg_tile = v33;
			a1a[1].left_tile = 0;
			a1a[1].right_tile = v31;
			a1a[1].flags = 38;

			fntcmd_select_tileset(&v4, 0);
			fntcmd_store_s8(&v4, '{'); // Left wnd border

			if ( v24 == usr->field_D36 )
			{
				fntcmd_set_txtColor(&v4, usr->p_ypaworld->iniColors[62].r, usr->p_ypaworld->iniColors[62].g, usr->p_ypaworld->iniColors[62].b);
			}
			else
			{
				fntcmd_set_txtColor(&v4, usr->p_ypaworld->iniColors[61].r, usr->p_ypaworld->iniColors[61].g, usr->p_ypaworld->iniColors[61].b);
			}

			v4 = lstvw_txt_line(yw, v4, 2, a1a);

			fntcmd_select_tileset(&v4, 0);
			fntcmd_store_s8(&v4, '}'); // Right wnd border
			fntcmd_next_line(&v4);
		}
	}
	v4 = lstvw_down_border(yw, &usr->input_listview, v4, 0, "xyz");
	fntcmd_set_end(&v4);

	w3d_a209 v21;
	v21.cmdbuf = usr->input_listview.draw_cmd;
	v21.includ = usr->input_listview.field_1C4;

	return sub_423288(&v21);
}


void set_keys_vals(_NC_STACK_ypaworld *yw)
{
	memset(keySS, 0, sizeof(keySS));

	keySS[0].title_by_language = "*";
	keySS[0].KEYCODE = '*';
	keySS[0].short_name = "nop";

	keySS[27].title_by_language = get_lang_string(yw->string_pointers_p2, 1001, "ESC");
	keySS[27].KEYCODE = 0;
	keySS[27].short_name = "esc";

	keySS[32].title_by_language = get_lang_string(yw->string_pointers_p2, 1002, "SPACE");
	keySS[32].KEYCODE = ' ';
	keySS[32].short_name = "space";

	keySS[38].title_by_language = get_lang_string(yw->string_pointers_p2, 1003, "UP");
	keySS[38].KEYCODE = 0;
	keySS[38].short_name = "up";

	keySS[40].title_by_language = get_lang_string(yw->string_pointers_p2, 1004, "DOWN");
	keySS[40].KEYCODE = 0;
	keySS[40].short_name = "down";

	keySS[37].title_by_language = get_lang_string(yw->string_pointers_p2, 1005, "LEFT");
	keySS[37].short_name = "left";
	keySS[37].KEYCODE = 0;

	keySS[39].title_by_language = get_lang_string(yw->string_pointers_p2, 1006, "RIGHT");
	keySS[39].KEYCODE = 0;
	keySS[39].short_name = "right";

	keySS[112].title_by_language = get_lang_string(yw->string_pointers_p2, 1007, "F1");
	keySS[112].KEYCODE = 0;
	keySS[112].short_name = "f1";

	keySS[113].title_by_language = get_lang_string(yw->string_pointers_p2, 1008, "F2");
	keySS[113].KEYCODE = 0;
	keySS[113].short_name = "f2";

	keySS[114].title_by_language = get_lang_string(yw->string_pointers_p2, 1009, "F3");
	keySS[114].KEYCODE = 0;
	keySS[114].short_name = "f3";

	keySS[115].title_by_language = get_lang_string(yw->string_pointers_p2, 1010, "F4");
	keySS[115].KEYCODE = 0;
	keySS[115].short_name = "f4";

	keySS[116].title_by_language = get_lang_string(yw->string_pointers_p2, 1011, "F5");
	keySS[116].short_name = "f5";
	keySS[116].KEYCODE = 0;

	keySS[117].title_by_language = get_lang_string(yw->string_pointers_p2, 1012, "F6");
	keySS[117].short_name = "f6";
	keySS[117].KEYCODE = 0;

	keySS[118].title_by_language = get_lang_string(yw->string_pointers_p2, 1013, "F7");
	keySS[118].KEYCODE = 0;
	keySS[118].short_name = "f7";

	keySS[119].title_by_language = get_lang_string(yw->string_pointers_p2, 1014, "F8");
	keySS[119].KEYCODE = 0;
	keySS[119].short_name = "f8";

	keySS[120].title_by_language = get_lang_string(yw->string_pointers_p2, 1015, "F9");
	keySS[120].KEYCODE = 0;
	keySS[120].short_name = "f9";

	keySS[121].title_by_language = get_lang_string(yw->string_pointers_p2, 1016, "F10");
	keySS[121].KEYCODE = 0;
	keySS[121].short_name = "f10";

	keySS[122].title_by_language = get_lang_string(yw->string_pointers_p2, 1017, "F11");
	keySS[122].short_name = "f11";
	keySS[122].KEYCODE = 0;

	keySS[123].title_by_language = get_lang_string(yw->string_pointers_p2, 1018, "F12");
	keySS[123].short_name = "f12";
	keySS[123].KEYCODE = 0;

	keySS[8].title_by_language = get_lang_string(yw->string_pointers_p2, 1019, "BACK");
	keySS[8].KEYCODE = 0;
	keySS[8].short_name = "bs";

	keySS[9].title_by_language = get_lang_string(yw->string_pointers_p2, 1020, "TAB");
	keySS[9].KEYCODE = 0;
	keySS[9].short_name = "tab";

	keySS[12].title_by_language = get_lang_string(yw->string_pointers_p2, 1021, "CLEAR");
	keySS[12].KEYCODE = 0;
	keySS[12].short_name = "clear";

	keySS[13].title_by_language = get_lang_string(yw->string_pointers_p2, 1022, "RETURN");
	keySS[13].KEYCODE = 0;
	keySS[13].short_name = "return";

	keySS[17].title_by_language = get_lang_string(yw->string_pointers_p2, 1023, "CTRL");
	keySS[17].short_name = "ctrl";
	keySS[17].KEYCODE = 0;

	keySS[16].title_by_language = get_lang_string(yw->string_pointers_p2, 1024, "SHIFT");
	keySS[16].short_name = "rshift";
	keySS[16].KEYCODE = 0;

	keySS[18].title_by_language = get_lang_string(yw->string_pointers_p2, 1025, "ALT");
	keySS[18].KEYCODE = 0;
	keySS[18].short_name = "alt";

	keySS[19].title_by_language = get_lang_string(yw->string_pointers_p2, 1026, "PAUSE");
	keySS[19].KEYCODE = 0;
	keySS[19].short_name = "pause";

	keySS[33].title_by_language = get_lang_string(yw->string_pointers_p2, 1027, "PGUP");
	keySS[33].KEYCODE = 0;
	keySS[33].short_name = "pageup";

	keySS[34].title_by_language = get_lang_string(yw->string_pointers_p2, 1028, "PGDOWN");
	keySS[34].KEYCODE = 0;
	keySS[34].short_name = "pagedown";

	keySS[35].title_by_language = get_lang_string(yw->string_pointers_p2, 1029, "END");
	keySS[35].short_name = "end";
	keySS[35].KEYCODE = 0;

	keySS[36].title_by_language = get_lang_string(yw->string_pointers_p2, 1030, "HOME");
	keySS[36].short_name = "home";
	keySS[36].KEYCODE = 0;

	keySS[41].title_by_language = get_lang_string(yw->string_pointers_p2, 1031, "SELECT");
	keySS[41].KEYCODE = 0;
	keySS[41].short_name = "select";

	keySS[43].title_by_language = get_lang_string(yw->string_pointers_p2, 1032, "EXEC");
	keySS[43].KEYCODE = 0;
	keySS[43].short_name = "execute";

	keySS[44].title_by_language = get_lang_string(yw->string_pointers_p2, 1033, "PRINT");
	keySS[44].KEYCODE = 0;
	keySS[44].short_name = "snapshot";

	keySS[45].title_by_language = get_lang_string(yw->string_pointers_p2, 1034, "INS");
	keySS[45].KEYCODE = 0;
	keySS[45].short_name = "ins";

	keySS[46].title_by_language = get_lang_string(yw->string_pointers_p2, 1035, "DEL");
	keySS[46].short_name = "del";
	keySS[46].KEYCODE = 0;

	keySS[49].title_by_language = get_lang_string(yw->string_pointers_p2, 1037, "1");
	keySS[49].short_name = "1";
	keySS[49].KEYCODE = '1';

	keySS[50].title_by_language = get_lang_string(yw->string_pointers_p2, 1038, "2");
	keySS[50].KEYCODE = '2';
	keySS[50].short_name = "2";

	keySS[51].title_by_language = get_lang_string(yw->string_pointers_p2, 1039, "3");
	keySS[51].KEYCODE = '3';
	keySS[51].short_name = "3";

	keySS[52].title_by_language = get_lang_string(yw->string_pointers_p2, 1040, "4");
	keySS[52].KEYCODE = 0x34;
	keySS[52].short_name = "4";

	keySS[53].title_by_language = get_lang_string(yw->string_pointers_p2, 1041, "5");
	keySS[53].KEYCODE = '5';
	keySS[53].short_name = "5";

	keySS[54].title_by_language = get_lang_string(yw->string_pointers_p2, 1042, "6");
	keySS[54].KEYCODE = '6';
	keySS[54].short_name = "6";

	keySS[55].title_by_language = get_lang_string(yw->string_pointers_p2, 1043, "7");
	keySS[55].short_name = "7";
	keySS[55].KEYCODE = '7';

	keySS[56].title_by_language = get_lang_string(yw->string_pointers_p2, 1044, "8");
	keySS[56].KEYCODE = '8';
	keySS[56].short_name = "8";

	keySS[57].title_by_language = get_lang_string(yw->string_pointers_p2, 1045, "9");
	keySS[57].KEYCODE = '9';
	keySS[57].short_name = "9";

	keySS[48].title_by_language = get_lang_string(yw->string_pointers_p2, 1046, "0");
	keySS[48].KEYCODE = '0';
	keySS[48].short_name = "0";

	keySS[65].title_by_language = get_lang_string(yw->string_pointers_p2, 1047, "A");
	keySS[65].KEYCODE = 'A';
	keySS[65].short_name = "a";

	keySS[66].title_by_language = get_lang_string(yw->string_pointers_p2, 1048, "B");
	keySS[66].short_name = "b";
	keySS[66].KEYCODE = 'B';

	keySS[67].title_by_language = get_lang_string(yw->string_pointers_p2, 1049, "C");
	keySS[67].KEYCODE = 'C';
	keySS[67].short_name = "c";

	keySS[68].title_by_language = get_lang_string(yw->string_pointers_p2, 1050, "D");
	keySS[68].KEYCODE = 68;
	keySS[68].short_name = "d";

	keySS[69].title_by_language = get_lang_string(yw->string_pointers_p2, 1051, "E");
	keySS[69].KEYCODE = 69;
	keySS[69].short_name = "e";

	keySS[70].title_by_language = get_lang_string(yw->string_pointers_p2, 1052, "F");
	keySS[70].KEYCODE = 70;
	keySS[70].short_name = "f";

	keySS[71].title_by_language = get_lang_string(yw->string_pointers_p2, 1053, "G");
	keySS[71].KEYCODE = 71;
	keySS[71].short_name = "g";

	keySS[72].title_by_language = get_lang_string(yw->string_pointers_p2, 1054, "H");
	keySS[72].short_name = "h";
	keySS[72].KEYCODE = 72;

	keySS[73].title_by_language = get_lang_string(yw->string_pointers_p2, 1055, "I");
	keySS[73].short_name = "i";
	keySS[73].KEYCODE = 73;

	keySS[74].title_by_language = get_lang_string(yw->string_pointers_p2, 1056, "J");
	keySS[74].KEYCODE = 74;
	keySS[74].short_name = "j";

	keySS[75].title_by_language = get_lang_string(yw->string_pointers_p2, 1057, "K");
	keySS[75].KEYCODE = 75;
	keySS[75].short_name = "k";

	keySS[76].title_by_language = get_lang_string(yw->string_pointers_p2, 1058, "L");
	keySS[76].KEYCODE = 76;
	keySS[76].short_name = "l";

	keySS[77].title_by_language = get_lang_string(yw->string_pointers_p2, 1059, "M");
	keySS[77].KEYCODE = 77;
	keySS[77].short_name = "m";

	keySS[78].title_by_language = get_lang_string(yw->string_pointers_p2, 1060, "N");
	keySS[78].short_name = "n";
	keySS[78].KEYCODE = 78;

	keySS[79].title_by_language = get_lang_string(yw->string_pointers_p2, 1061, "O");
	keySS[79].short_name = "o";
	keySS[79].KEYCODE = 79;

	keySS[80].title_by_language = get_lang_string(yw->string_pointers_p2, 1062, "P");
	keySS[80].KEYCODE = 80;
	keySS[80].short_name = "p";

	keySS[81].title_by_language = get_lang_string(yw->string_pointers_p2, 1063, "Q");
	keySS[81].KEYCODE = 81;
	keySS[81].short_name = "q";

	keySS[82].title_by_language = get_lang_string(yw->string_pointers_p2, 1064, "R");
	keySS[82].KEYCODE = 82;
	keySS[82].short_name = "r";

	keySS[83].title_by_language = get_lang_string(yw->string_pointers_p2, 1065, "S");
	keySS[83].KEYCODE = 83;
	keySS[83].short_name = "s";

	keySS[84].title_by_language = get_lang_string(yw->string_pointers_p2, 1066, "T");
	keySS[84].short_name = "t";
	keySS[84].KEYCODE = 84;

	keySS[85].title_by_language = get_lang_string(yw->string_pointers_p2, 1067, "U");
	keySS[85].short_name = "u";
	keySS[85].KEYCODE = 85;

	keySS[86].title_by_language = get_lang_string(yw->string_pointers_p2, 1068, "V");
	keySS[86].KEYCODE = 86;
	keySS[86].short_name = "v";

	keySS[87].title_by_language = get_lang_string(yw->string_pointers_p2, 1069, "W");
	keySS[87].KEYCODE = 87;
	keySS[87].short_name = "w";

	keySS[88].title_by_language = get_lang_string(yw->string_pointers_p2, 1070, "X");
	keySS[88].KEYCODE = 88;
	keySS[88].short_name = "x";

	keySS[89].title_by_language = get_lang_string(yw->string_pointers_p2, 1071, "Y");
	keySS[89].KEYCODE = 89;
	keySS[89].short_name = "y";

	keySS[90].title_by_language = get_lang_string(yw->string_pointers_p2, 1072, "Z");
	keySS[90].short_name = "z";
	keySS[90].KEYCODE = 90;

	keySS[96].title_by_language = get_lang_string(yw->string_pointers_p2, 1073, "NUM 0");
	keySS[96].short_name = "num0";
	keySS[96].KEYCODE = 0;

	keySS[97].title_by_language = get_lang_string(yw->string_pointers_p2, 1074, "NUM 1");
	keySS[97].KEYCODE = 0;
	keySS[97].short_name = "num1";

	keySS[98].title_by_language = get_lang_string(yw->string_pointers_p2, 1075, "NUM 2");
	keySS[98].KEYCODE = 0;
	keySS[98].short_name = "num2";

	keySS[99].title_by_language = get_lang_string(yw->string_pointers_p2, 1076, "NUM 3");
	keySS[99].KEYCODE = 0;
	keySS[99].short_name = "num3";

	keySS[100].title_by_language = get_lang_string(yw->string_pointers_p2, 1077, "NUM 4");
	keySS[100].KEYCODE = 0;
	keySS[100].short_name = "num4";

	keySS[101].title_by_language = get_lang_string(yw->string_pointers_p2, 1078, "NUM 5");
	keySS[101].short_name = "num5";
	keySS[101].KEYCODE = 0;

	keySS[102].title_by_language = get_lang_string(yw->string_pointers_p2, 1079, "NUM 6");
	keySS[102].short_name = "num6";
	keySS[102].KEYCODE = 0;

	keySS[103].title_by_language = get_lang_string(yw->string_pointers_p2, 1080, "NUM 7");
	keySS[103].KEYCODE = 0;
	keySS[103].short_name = "num7";

	keySS[104].title_by_language = get_lang_string(yw->string_pointers_p2, 1081, "NUM 8");
	keySS[104].KEYCODE = 0;
	keySS[104].short_name = "num8";

	keySS[105].title_by_language = get_lang_string(yw->string_pointers_p2, 1082, "NUM 9");
	keySS[105].KEYCODE = 0;
	keySS[105].short_name = "num9";

	keySS[106].title_by_language = get_lang_string(yw->string_pointers_p2, 1083, "MUL");
	keySS[106].KEYCODE = 0;
	keySS[106].short_name = "nummul";

	keySS[107].title_by_language = get_lang_string(yw->string_pointers_p2, 1084, "ADD");
	keySS[107].KEYCODE = 0;
	keySS[107].short_name = "numplus";

	keySS[110].title_by_language = get_lang_string(yw->string_pointers_p2, 1085, "DOT");
	keySS[110].short_name = "numdot";
	keySS[110].KEYCODE = 0;

	keySS[109].title_by_language = get_lang_string(yw->string_pointers_p2, 1086, "SUB");
	keySS[109].KEYCODE = 0;
	keySS[109].short_name = "numminus";

	keySS[108].title_by_language = get_lang_string(yw->string_pointers_p2, 1087, "ENTER");
	keySS[108].KEYCODE = 0;
	keySS[108].short_name = "enter";

	keySS[111].title_by_language = get_lang_string(yw->string_pointers_p2, 1088, "DIV");
	keySS[111].KEYCODE = 0;
	keySS[111].short_name = "numdiv";

	keySS[188].title_by_language = get_lang_string(yw->string_pointers_p2, 1089, "EXTRA_1");
	keySS[188].KEYCODE = 44;
	keySS[188].short_name = "extra1";

	keySS[190].title_by_language = get_lang_string(yw->string_pointers_p2, 1090, "EXTRA_2");
	keySS[190].short_name = "extra2";
	keySS[190].KEYCODE = 46;

	keySS[189].title_by_language = get_lang_string(yw->string_pointers_p2, 1091, "EXTRA_3");
	keySS[189].KEYCODE = 45;
	keySS[189].short_name = "extra3";

	keySS[226].title_by_language = get_lang_string(yw->string_pointers_p2, 1092, "EXTRA_4");
	keySS[226].KEYCODE = 60;
	keySS[226].short_name = "extra4";

	keySS[186].title_by_language = get_lang_string(yw->string_pointers_p2, 1093, "EXTRA_5");
	keySS[186].KEYCODE = 252;
	keySS[186].short_name = "extra5";

	keySS[187].title_by_language = get_lang_string(yw->string_pointers_p2, 1094, "EXTRA_6");
	keySS[187].KEYCODE = 43;
	keySS[187].short_name = "extra6";

	keySS[192].title_by_language = get_lang_string(yw->string_pointers_p2, 1095, "EXTRA_7");
	keySS[192].KEYCODE = 246;
	keySS[192].short_name = "extra7";

	keySS[222].title_by_language = get_lang_string(yw->string_pointers_p2, 1096, "EXTRA_8");
	keySS[222].short_name = "extra8";
	keySS[222].KEYCODE = 228;

	keySS[191].title_by_language = get_lang_string(yw->string_pointers_p2, 1097, "EXTRA_9");
	keySS[191].short_name = "extra9";
	keySS[191].KEYCODE = 35;

	keySS[221].title_by_language = get_lang_string(yw->string_pointers_p2, 1098, "EXTRA_10");
	keySS[221].KEYCODE = 96;
	keySS[221].short_name = "extra10";

	keySS[220].title_by_language = get_lang_string(yw->string_pointers_p2, 1099, "EXTRA_11");
	keySS[220].KEYCODE = 94;
	keySS[220].short_name = "extra11";

	keySS[219].title_by_language = get_lang_string(yw->string_pointers_p2, 1100, "EXTRA_12");
	keySS[219].KEYCODE = 0;
	keySS[219].short_name = "extra12";

	keySS[223].title_by_language = get_lang_string(yw->string_pointers_p2, 1101, "EXTRA_13");
	keySS[223].KEYCODE = 0;
	keySS[223].short_name = "extra13";

	keySS[145].title_by_language = get_lang_string(yw->string_pointers_p2, 1102, "EXTRA_14");
	keySS[145].short_name = "extra14";
	keySS[145].KEYCODE = 0;

	keySS[144].title_by_language = get_lang_string(yw->string_pointers_p2, 1103, "EXTRA_15");
	keySS[144].short_name = "extra15";
	keySS[144].KEYCODE = 0;

	keySS[124].title_by_language = get_lang_string(yw->string_pointers_p2, 1104, "EXTRA_16");
	keySS[124].KEYCODE = 0;
	keySS[124].short_name = "extra16";

	keySS[125].title_by_language = get_lang_string(yw->string_pointers_p2, 1105, "EXTRA_17");
	keySS[125].KEYCODE = 0;
	keySS[125].short_name = "extra17";

	keySS[126].title_by_language = get_lang_string(yw->string_pointers_p2, 1106, "EXTRA_18");
	keySS[126].KEYCODE = 0;
	keySS[126].short_name = "extra18";

	keySS[131].title_by_language = get_lang_string(yw->string_pointers_p2, 1121, "MIDDLE MOUSE");
	keySS[131].KEYCODE = 0;
	keySS[131].short_name = "mmb";

	keySS[134].title_by_language = get_lang_string(yw->string_pointers_p2, 1123, "JOYB0");
	keySS[134].short_name = "joyb0";
	keySS[134].KEYCODE = 0;

	keySS[135].title_by_language = get_lang_string(yw->string_pointers_p2, 1124, "JOYB1");
	keySS[135].short_name = "joyb1";
	keySS[135].KEYCODE = 0;

	keySS[136].title_by_language = get_lang_string(yw->string_pointers_p2, 1125, "JOYB2");
	keySS[136].KEYCODE = 0;
	keySS[136].short_name = "joyb2";

	keySS[137].title_by_language = get_lang_string(yw->string_pointers_p2, 1126, "JOYB3");
	keySS[137].KEYCODE = 0;
	keySS[137].short_name = "joyb3";

	keySS[138].title_by_language = get_lang_string(yw->string_pointers_p2, 1127, "JOYB4");
	keySS[138].KEYCODE = 0;
	keySS[138].short_name = "joyb4";

	keySS[139].title_by_language = get_lang_string(yw->string_pointers_p2, 1128, "JOYB5");
	keySS[139].KEYCODE = 0;
	keySS[139].short_name = "joyb5";

	keySS[140].title_by_language = get_lang_string(yw->string_pointers_p2, 1129, "JOYB6");
	keySS[140].short_name = "joyb6";
	keySS[140].KEYCODE = 0;

	keySS[141].title_by_language = get_lang_string(yw->string_pointers_p2, 1130, "JOYB7");
	keySS[141].short_name = "joyb7";
	keySS[141].KEYCODE = 0;
}


int yw_loadSky(_NC_STACK_ypaworld *yw, const char *skyname)
{
	char buf[256];
	strcpy(buf, "data:");
	strcat(buf, skyname);

	NC_STACK_base *sky = READ_BAS_FILE(buf);
	yw->sky_loaded_base = sky;
	if ( !sky )
	{
		ypa_log_out("Couldn't create %s\n", buf);
		return 0;
	}

	call_vtbl(sky, 2, 0x80001024, 1, 0);
	call_vtbl(sky, 2, 0x80001004, yw->field_15ec, 0);
	call_vtbl(sky, 2, 0x80001023, yw->field_15f0, 0);
	return 1;
}

//void yw_renderSky_test(_NC_STACK_ypaworld *yw, base77Func *rndr_params)
//{
//	if ( yw->sky_loaded_base )
//	{
//		float v6 = rndr_params->field_24;
//		flag_xyz v5;
//		v5.x = 0;
//		v5.y = 0;
//		v5.z = 0;
//		v5.flag = 7;
//
//		call_method(yw->sky_loaded_base, 68, &v5);
//
//		rndr_params->field_24 = 32000.0;
//
//		call_method(yw->sky_loaded_base, 77, rndr_params);
//
//		rndr_params->field_24 = v6;
//	}
//}


void fill_videmodes_list(UserData *usr)
{
	while ( 1 )
	{
		nnode *nod = RemHead(&usr->video_mode_list);
		if ( !nod )
			break;
		nc_FreeMem(nod);
	}

	gfxEngine__getter(0x8000300D, &usr->p_ypaworld->win3d, 0);

	windd_arg256 warg_256;
	warg_256.sort_id = 0;

	int id = -1;

	while( id )
	{
		id = call_method(usr->p_ypaworld->win3d, 256, &warg_256);

		video_mode_node *vnode = (video_mode_node *)AllocVec(sizeof(video_mode_node), 65536);

		if ( vnode )
		{
			vnode->sort_id = warg_256.sort_id;
			vnode->width = warg_256.width;
			vnode->height = warg_256.height;

			memset(vnode->name, 0, 128);
			strncpy(vnode->name, warg_256.name, 127);

			AddTail(&usr->video_mode_list, vnode);
		}
		warg_256.sort_id = id;
	}
}


void listSaveDir(UserData *usr, const char *saveDir)
{
	player_status statuses[8];

	memcpy(statuses, usr->p_ypaworld->playerstatus, sizeof(player_status) * 8);

	char v21[300];

	strcpy(v21, usr->callSIGN);

	int v5 = usr->p_ypaworld->maxroboenergy;
	int v27 = usr->p_ypaworld->maxreloadconst;

	usr->opened_dir = OpenDir(saveDir);
	if ( usr->opened_dir )
	{
		dirEntry a2a;
		while ( ReadDir(usr->opened_dir, &a2a) )
		{
			if ( a2a.field_0 & 1 )
			{
				if ( strcmp(a2a.e_name, ".") && strcmp(a2a.e_name, "..") )
				{
					profilesNode *v10 = (profilesNode *)AllocVec(sizeof(profilesNode), 65537);
					if ( v10 )
					{
						strcpy(v10->profile_subdir, a2a.e_name);

						AddTail(&usr->files_list, v10);

						scrCallBack v25;

						v25.func = parseSaveUser;
						v25.world = (_NC_STACK_ypaworld *)usr->p_ypaworld->self_full;
						v25.world2 = usr->p_ypaworld;

						char buf[300];

						sprintf(buf, "%s/%s/user.txt\n", saveDir, a2a.e_name);

						if ( !def_parseFile(buf, 1, &v25, 2) )
							ypa_log_out("Warning, cannot parse %s for time scanning\n", buf);

						v10->field_C = 1;
						v10->pStatus_3 = usr->p_ypaworld->playerstatus[1].p3;
					}
				}
			}
		}
		CloseDir(usr->opened_dir);
	}
	else
	{
		ypa_log_out("Unknown Game-Directory %s\n", saveDir);
	}

	memcpy(usr->p_ypaworld->playerstatus, statuses, sizeof(player_status) * 8);

	usr->p_ypaworld->maxreloadconst = v27;
	usr->p_ypaworld->maxroboenergy = v5;

	strcpy(usr->callSIGN, v21);
}


void listLocaleDir(UserData *usr, const char *dirname)
{
	usr->lang_dlls_count = 0;

	langDll_node *deflng = NULL;
	ncDir *dir = OpenDir(dirname);
	if ( dir )
	{
		dirEntry v18;

		while ( ReadDir(dir, &v18) )
		{
			char *v3 = strstr(v18.e_name, ".LNG");
			if ( !v3 )
				v3 = strstr(v18.e_name, ".lng");
			/*if ( !v3 )
			  v3 = strstr(v18.e_name, ".dll");
			if ( !v3 )
			  v3 = strstr(v18.e_name, ".DLL");*/

			if ( !(v18.field_0 & 1) && v3 )
			{
				char v19[256];
				memset(v19, 0, 256);

				for (int i = 0; i < 31; i++)
				{
					char v5 = v18.e_name[i];
					if (v5 <= ' ')
						break;
					if (v5 == '.')
						break;

					v19[i] = toupper(v5);
				}

				langDll_node *v7 = (langDll_node *)usr->lang_dlls.head;

				int finded = 0;

				while( v7->next )
				{
					if ( !strcasecmp(v7->langDllName, v19) )
					{
						finded = 1;
						break;
					}
					v7 = (langDll_node *)v7->next;
				}

				if ( !finded )
				{
					usr->lang_dlls_count++;

					langDll_node *v9 = (langDll_node *)AllocVec(sizeof(langDll_node), 65537);
					if ( v9 )
					{
						strcpy(v9->langDllName, v19);
						AddTail(&usr->lang_dlls, v9);

						if ( !strcasecmp(v9->langDllName, "language") )
							deflng = v9;
					}
				}
			}
		}

		CloseDir(dir);
	}
	else
	{
		ypa_log_out("Unknown Locale-Directory %s\n", dirname);
	}

	if ( deflng )
		usr->default_lang_dll = deflng;
}



void ypaworld_func154__sub0(_NC_STACK_ypaworld *yw)
{
	if ( yw->movies.movies_names_present[0] )
	{
		gfxEngine__getter(0x8000300D, &yw->win3d, 0);

		char buf[256];

		sub_412810(yw->movies.game_intro, buf, 256);
		char *v5 = buf;

		call_method(yw->win3d, 323, &v5);

		sub_412D28(&input_states);
		input_states.downed_key = 0;
		input_states.downed_key_2 = 0;
		input_states.dword8 = 0;
	}
}
