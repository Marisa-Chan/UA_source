#include "includes.h"
#include "inttypes.h"
#include "nucleas.h"
#include "amesh.h"
#include "utils.h"
#include "engine_input.h"

#include "raster.h"
#include "display.h"
#include "windd.h"
#include "win3d.h"

#include "skeleton.h"
#include "base.h"

#include "engine_gfx.h"



stored_functions *classvtbl_get_amesh();
class_return * class_set_amesh(int, ...);

stored_functions amesh_class_vtbl(class_set_amesh);


class_stored amesh_class_off (NULL, NULL, "MC2classes:amesh.class", classvtbl_get_amesh);


stored_functions *classvtbl_get_amesh()
{
	return &amesh_class_vtbl;
}

CLASSFUNC amesh_funcs[1024];

int sub_419E6C(__NC_STACK_amesh *amesh, tUtV **olpl)
{
	if ( amesh->olpl )
	{
		nc_FreeMem(amesh->olpl);
		amesh->olpl = NULL;
	}

	int olpl_cnt = 0;

	for (int i = 0; i < amesh->cnt; i++)
	{
		tUtV * v6 = olpl[i];
		while( v6->tu >= 0.0)
		{
			olpl_cnt++;
			v6++;
		}
		olpl_cnt++;
	}

	amesh->olpl = (tUtV **)AllocVec(sizeof(tUtV *) * amesh->cnt + sizeof(tUtV) * olpl_cnt, 1);;
	if ( !amesh->olpl )
		return 0;

	tUtV *uv = (tUtV *)&amesh->olpl[amesh->cnt];
	//void *ed = ((char *)amesh->olpl + sizeof(tUtV *) * amesh->cnt + sizeof(tUtV) * olpl_cnt);

	for (int i = 0; i < amesh->cnt; i++)
	{
		tUtV * v6 = olpl[i];
		amesh->olpl[i] = uv;
		while( 1 )
		{
			*uv = *v6;
			v6++;
			uv++;

			if (v6->tu < 0.0)
				break;
		}
		*uv = *v6;
		uv++;
	}
	return 1;
}


int amesh_func0__sub0(__NC_STACK_amesh *amesh, stack_vals *stak)
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

			case 0x80001004:
				if ( stk->value )
					amesh->field_A |= 1;
				else
					amesh->field_A &= 0xFFFE;
				break;
			case 0x80002000:
				amesh->ilbm1 = (NC_STACK_bitmap *)stk->value;
				break;

			case 0x80002009:
				amesh->ilbm2 = (NC_STACK_bitmap *)stk->value;
				break;

			case 0x80003000:
				amesh->cnt = stk->value;
				break;

			case 0x80003001:
				if ( amesh->atts )
				{
					nc_FreeMem(amesh->atts);
					amesh->atts = NULL;
				}
				amesh->atts = (ATTS *)AllocVec(amesh->cnt * sizeof(ATTS), 1);

				if ( amesh->atts )
				{
					memcpy(amesh->atts, (void *)stk->value, amesh->cnt * sizeof(ATTS));
				}
				else
					return 0;
				break;

			case 0x80003002:
				if ( !sub_419E6C(amesh, (tUtV **)stk->value) )
					return 0;
				break;

			}
			stk++;
		}
	}

	return 1;
}


NC_STACK_amesh * amesh_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
	NC_STACK_amesh *obj = (NC_STACK_amesh *)call_parent(zis, clss, 0, stak); // rsrc_func0
	if (obj)
	{
		__NC_STACK_amesh *amesh = &obj->stack__amesh;

		if ( amesh_func0__sub0(amesh, stak) )
		{
			call_vtbl(obj, 3, 0x8000200E, &amesh->field_14, 0);
		}
		else
		{
			call_method(obj, 1);
			obj = NULL;
		}
	}

	return obj;
}

size_t amesh_func1(NC_STACK_amesh *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_amesh *amesh = &obj->stack__amesh;

	if ( amesh->atts )
		nc_FreeMem(amesh->atts);

	if ( amesh->olpl )
		nc_FreeMem(amesh->olpl);

	return call_parent(zis, obj, 1, stak);
}


void amesh_func2__sub0(__NC_STACK_amesh *amesh, stack_vals *stak)
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

			case 0x80001004:
				if ( stk->value )
					amesh->field_A |= 1;
				else
					amesh->field_A &= 0xFFFE;
				break;
			case 0x80002000:
				amesh->ilbm1 = (NC_STACK_bitmap *)stk->value;
				break;

			case 0x80002009:
				amesh->ilbm2 = (NC_STACK_bitmap *)stk->value;
				break;

			case 0x80003001:
				if ( amesh->atts )
				{
					nc_FreeMem(amesh->atts);
					amesh->atts = NULL;
				}
				amesh->atts = (ATTS *)AllocVec(amesh->cnt * sizeof(ATTS), 1);

				if ( amesh->atts )
				{
					memcpy(amesh->atts, (void *)stk->value, amesh->cnt * sizeof(ATTS));
				}

				break;

			case 0x80003002:
				sub_419E6C(amesh, (tUtV **)stk->value);
				break;
			}
			stk++;
		}
	}

}

void amesh_func2(NC_STACK_amesh *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_amesh *amesh = &obj->stack__amesh;

	amesh_func2__sub0(amesh, stak);

	call_parent(zis, obj, 2, stak);
}

void amesh_func3(NC_STACK_amesh *obj, class_stru *zis, stack_vals *stak)
{

	__NC_STACK_amesh *amesh = &obj->stack__amesh;

	stack_vals *val = find_id_in_stack2(0x80003000, stak);

	if ( val )
		*(int *)val->value = amesh->cnt;

	call_parent(zis, obj, 3, stak);
}

NC_STACK_amesh *amesh_func5(class_stru *clss, class_stru *zis, MFILE **file)
{
	MFILE *mfile = *file;
	NC_STACK_amesh *obj = NULL;
	__NC_STACK_amesh *amesh = NULL;

	while ( 1 )
	{
		int iff_res = read_next_IFF(mfile, 2);

		if ( iff_res == -2 )
			break;

		if ( iff_res )
		{
			if ( obj )
				call_method(obj, 1);
			return NULL;
		}

		MFILE_S1 *chunk = GET_FORM_INFO_OR_NULL(mfile);

		if ( chunk->TAG == TAG_FORM && chunk->TAG_EXTENSION == TAG_AREA )
		{
			obj = (NC_STACK_amesh *)call_parent(zis, clss, 5, (stack_vals *)file);

			if ( !obj )
				return NULL;

			amesh = &obj->stack__amesh;
			call_vtbl(obj, 3, 0x8000200E, &amesh->field_14, 0);

		}
		else if ( chunk->TAG == TAG_ATTS )
		{
			if ( obj )
			{
				amesh->cnt = chunk->TAG_SIZE / sizeof(ATTS);
				amesh->atts = (ATTS *)AllocVec(chunk->TAG_SIZE, 1);

				if ( !amesh->atts )
				{
					call_method(obj, 1);
					return NULL;
				}

				mfread(mfile, amesh->atts, chunk->TAG_SIZE);

				for (int i = 0; i < amesh->cnt; i++)
					amesh->atts[i].field_0 = SWAP16(amesh->atts[i].field_0);
			}
			read_next_IFF(mfile, 2);
		}
		else if ( chunk->TAG == TAG_OLPL )
		{
			if ( obj )
			{
				amesh->olpl = (tUtV **)AllocVec(sizeof(tUtV) * chunk->TAG_SIZE / 2 + sizeof(tUtV *) * amesh->cnt, 1);
				if ( !amesh->olpl )
				{
					call_method(obj, 1);
					return NULL;
				}

				tUtV *uv = (tUtV *)&amesh->olpl[amesh->cnt];

				for (int i = 0; i < amesh->cnt; i++)
				{
					amesh->olpl[i] = uv;

					int16_t cnt;
					mfread(mfile, &cnt, 2);

					cnt = SWAP16(cnt);

					for (int j = 0; j < cnt; j++)
					{
						uint8_t tmp[2];
						mfread(mfile, tmp, 2);

						uv->tu = tmp[0] / 256.0;
						uv->tv = tmp[1] / 256.0;
						uv++;
					}

					uv->tu = -1;
					uv->tv = -1;
					uv++;
				}
			}
			read_next_IFF(mfile, 2);
		}
		else
		{
			read_default(mfile);
		}
	}

	return obj;
}

int amesh_func6(NC_STACK_amesh *obj, class_stru *zis, MFILE **file)
{
	MFILE *mfile = *file;
	__NC_STACK_amesh *amesh = &obj->stack__amesh;

	if ( sub_412FC0(mfile, TAG_AMSH, TAG_FORM, -1) )
		return 0;

	if ( !call_parent(zis, obj, 6, (stack_vals *)file) )
		return 0;

	sub_412FC0(mfile, 0, TAG_ATTS, -1);

	for (int i = 0; i < amesh->cnt; i++)
		amesh->atts[i].field_0 = SWAP16(amesh->atts[i].field_0);

	sub_413564(mfile, (sizeof(ATTS) * amesh->cnt), amesh->atts);
	sub_413290(mfile);

	for (int i = 0; i < amesh->cnt; i++)
		amesh->atts[i].field_0 = SWAP16(amesh->atts[i].field_0);

	if ( amesh->olpl )
	{
		sub_412FC0(mfile, 0, TAG_OLPL, -1);

		for (int i = 0; i < amesh->cnt; i++)
		{

			tUtV *uv = amesh->olpl[i];
			int16_t cnt = 0;
			while (uv->tu >= 0.0)
			{
				cnt++;
				uv++;
			}

			cnt = SWAP16(cnt);

			sub_413564(mfile, 2, &cnt);

			cnt = SWAP16(cnt);

			uv = amesh->olpl[i];

			for (int j = 0; j < cnt; j++)
			{
				uint8_t tmp[2];
				tmp[0] = uv[j].tu * 256.0;
				tmp[1] = uv[j].tv * 256.0;

				sub_413564(mfile, 2, tmp);
			}
		}
		sub_413290(mfile);
	}
	sub_413290(mfile);
	return 1;
}

// Add amesh to list
void amesh_func65(NC_STACK_amesh *obj, class_stru *, area_arg_65 *arg)
{
	__NC_STACK_amesh *amesh = &obj->stack__amesh;

	//v5 = *(_WORD *)(amesh->field_14 + 6) & 0xFEF6;
	int v5 = obj->stack__area.field_16 & 0xFEF6; //HACK

	if (v5 == 0)
		v5 = 0;
	else if (v5 == 2)
		v5 = 1;
	else if (v5 == 6)
		v5 = 2;
	else if (v5 == 0x32)
		v5 = 9;
	else if (v5 == 0x36)
		v5 = 10;
	else if (v5 == 0x42)
		v5 = 17;
	else if (v5 == 0x46)
		v5 = 18;
	else if (v5 == 0x72)
		v5 = 25;
	else if (v5 == 0x76)
		v5 = 26;
	else if (v5 == 0x82)
		v5 = 33;
	else
		return;

	skeleton_arg133 skel133;

	skel133.field_4 = 0;

	if ( v5 & 3 )
		skel133.field_4 = 1;
	if ( v5 & 0xC )
		skel133.field_4 |= 2;
	if ( amesh->field_A & 1 )
		skel133.field_4 |= 4;

	skel133.field_10 = arg->field_24;
	skel133.field_14 = arg->field_28;
	skel133.field_1C = arg->field_2C;
	skel133.field_20 = arg->field_30;


	bitmap_intern *v21;

	if ( amesh->ilbm1 )
	{
		bitmap_arg130 bitm130;

		bitm130.field_0 = arg->field_C;
		bitm130.field_4 = arg->field_10;
		call_method(amesh->ilbm1, 130, &bitm130);
		v21 = bitm130.pbitm;
	}
	else
	{
		v21 = NULL;
	}


	for (int i = 0; i < amesh->cnt; i++)
	{
		polysDatSub *datSub = &arg->polyDat->datSub;

		datSub->renderFlags = v5;

		datSub->vertexes = (xyz *)&datSub->field_18;
		datSub->pbitm = v21;

		skel133.field_0 = amesh->atts[i].field_0;

		skel133.polysubDat = datSub;

		skel133.field_18 = amesh->atts[i].field_3 / 256.0;

		if ( amesh->olpl )
			skel133.field_C = amesh->olpl[i];
		else
			skel133.field_C = NULL;

		polysDat *v23 = (polysDat *)call_method(arg->OBJ_SKELETON, 133, &skel133);
		if (v23)
		{
			arg->field_38++;

			if ( datSub->renderFlags & 0xC )
			{
				int v6 = 0;
				int v8 = 0;

				for (int i = 0; i < datSub->vertexCount; i++)
				{
					float clr = datSub->color[i];
					if (clr < 0.01)
						v6++;
					else if (clr > 0.99)
						v8++;
				}

				if ( v6 == datSub->vertexCount )
				{
					datSub->renderFlags &= 0xFFFFFFF3;
				}
				else if ( v8 == datSub->vertexCount )
				{
					datSub->renderFlags = 0;
				}
			}

			float maxz = 0.0;

			for (int i = 0; i < datSub->vertexCount; i++)
				if (datSub->vertexes[i].sz > maxz)
					maxz = datSub->vertexes[i].sz;


			arg->outPolys->range = maxz;
			arg->outPolys->data = arg->polyDat;
			arg->outPolys++;

			arg->polyDat->render_func = sub_4231FC;
			arg->polyDat = v23;
		}
	}
}

class_return amesh_class_descr;

class_return * class_set_amesh(int , ...)
{

	memset(amesh_funcs, 0, sizeof(CLASSFUNC) * 1024);

	amesh_funcs[0] = (CLASSFUNC)amesh_func0;
	amesh_funcs[1] = (CLASSFUNC)amesh_func1;
	amesh_funcs[2] = (CLASSFUNC)amesh_func2;
	amesh_funcs[3] = (CLASSFUNC)amesh_func3;
	amesh_funcs[5] = (CLASSFUNC)amesh_func5;
	amesh_funcs[6] = (CLASSFUNC)amesh_func6;
	amesh_funcs[65] = (CLASSFUNC)amesh_func65;

	amesh_class_descr.parent = "area.class";

	amesh_class_descr.vtbl = amesh_funcs;
	////amesh_class_descr.varSize = sizeof(__NC_STACK_amesh);
	amesh_class_descr.varSize = sizeof(NC_STACK_amesh) - offsetof(NC_STACK_amesh, stack__amesh); //// HACK
	amesh_class_descr.field_A = 0;
	return &amesh_class_descr;
}
