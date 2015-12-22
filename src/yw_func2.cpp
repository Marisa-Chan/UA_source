#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"


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
