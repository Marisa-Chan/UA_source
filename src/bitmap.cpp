#include "includes.h"
#include "nucleas.h"
#include "rsrc.h"
#include "bitmap.h"
#include "utils.h"


stored_functions *classvtbl_get_bitmap();
class_return * class_set_bitmap(int, ...);

stored_functions bitmap_class_vtbl(class_set_bitmap);


class_stored bitmap_class_off (NULL, NULL, "MC2classes:bitmap.class", classvtbl_get_bitmap);


stored_functions *classvtbl_get_bitmap()
{
	return &bitmap_class_vtbl;
}

CLASSFUNC bitmap_funcs[1024];
g_engines *engines___bitmap;


unsigned int sub_416704(NC_STACK_bitmap *a1, __NC_STACK_bitmap *a2, bitmap__opl *a3)
{
	//// WHAT IT THIS !?
	size_t a4 = 0;

	call_vtbl(a1, 3, 0x80001003, &a4, 0);

	if ( a2->field_4.opl2 )
	{
		if ( !a4 )
			nc_FreeMem(a2->field_4.opl2);
	}

	if ( a4 )
	{
		a2->field_4.opl1 = a3;
		return 1;
	}

	signed int v6 = 1;

	bitmap__opl *v7 = a3;
	if ( v7->field_E >= 0 )
	{
		__int16 v8;
		do
		{
			++v6;
			v8 = v7[1].field_E;
			++v7;
		}
		while ( v8 >= 0 );
	}

	bitmap_opl2 *unk = (bitmap_opl2 *)AllocVec(sizeof(bitmap_opl2) * v6, 1);

	if ( unk )
	{
		bitmap_opl2 *tmp = unk;
		bitmap_opl2 *up = &unk[v6 - 1];
		bitmap__opl *v12 = a3;
		if ( tmp < up )
		{
			do
			{
				tmp->field_0 = (long double)v12->field_0 * (1.0 / 256.0);
				tmp->field_4 = (long double)v12->field_2 * (1.0 / 256.0);
				tmp += 2;
				++v12;
			}
			while ( tmp < up );
		}
		tmp->field_0 = -1;
		tmp->field_4 = -1;

		a2->field_4.opl2 = unk;

		return 1;
	}
	return 0;
}


NC_STACK_bitmap * bitmap_func0(class_stru *obj, class_stru *zis, stack_vals *stak)
{
	NC_STACK_bitmap *clss = (NC_STACK_bitmap *)call_parent(zis, obj, 0, stak); // rsrc_func0
	if (clss)
	{
		__NC_STACK_bitmap *internal = &clss->stack__bitmap;

		void *v9 = (void *)find_id_in_stack_def_val(0x80002001, 0, stak);
		if ( v9 )
			sub_416704(clss, internal, (bitmap__opl *)v9);

		call_vtbl(clss, 3, 0x80001002, &internal->bitm_intern, 0); // Copy rsrc->data to bitm_intern
	}

	return clss;
}

size_t bitmap_func1(NC_STACK_bitmap *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_bitmap *internal = &obj->stack__bitmap;
	if ( internal->field_4.opl2 )
	{
		size_t a4 = 0;

		call_vtbl(obj, 3, 0x80001003, &a4, 0);
		if ( !a4 )
			nc_FreeMem(internal->field_4.opl2);
	}
	return call_parent(zis, obj, 1, stak);
}

void bitmap_func2(NC_STACK_bitmap *obj, class_stru *zis, stack_vals *stak)
{
	__NC_STACK_bitmap *internal = &obj->stack__bitmap;

	stack_vals *v5 = find_id_in_stack2(0x80002001, stak);
	if ( v5 )
		sub_416704(obj, internal, (bitmap__opl *)v5->value);

	stack_vals *ppal = find_id_in_stack2(0x80002007, stak);
	if ( ppal )
	{
		if ( internal->bitm_intern->pallete )
			memcpy(internal->bitm_intern->pallete, (void *)ppal->value, 256 * 3);
	}

	call_parent(zis, obj, 2, stak);
}

void bitmap_func3(NC_STACK_bitmap *obj, class_stru *zis, stack_vals *stak)
{

	__NC_STACK_bitmap *internal = &obj->stack__bitmap;

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

			case 0x80002000:
				*(bitmap_intern **)stk->value = internal->bitm_intern;
				break;
			case 0x80002001:
				*(void **)stk->value = 0;
				break;
			case 0x80002002:
				if ( internal->bitm_intern )
					*(int *)stk->value = internal->bitm_intern->width;
				else
					*(int *)stk->value = 0;
				break;
			case 0x80002003:
				if ( internal->bitm_intern )
					*(int *)stk->value = internal->bitm_intern->height;
				else
					*(int *)stk->value = 0;
				break;
			case 0x80002005:
				if ( internal->bitm_intern )
					*(void **)stk->value = internal->bitm_intern->buffer;
				else
					*(void **)stk->value = 0;
				break;
			case 0x80002006:
				*(int *)stk = internal->bitm_intern->pallete != NULL;
				break;
			case 0x80002007:
				*(void **)stk = internal->bitm_intern->pallete;
				break;
			}
			stk++;
		}
	}
	call_parent(zis, obj, 3, stak);
}

// Create bitmap resource node and fill rsrc field data
rsrc * bitmap_func64(NC_STACK_bitmap *obj, class_stru *zis, stack_vals *stak)
{
	rsrc *res = (rsrc *)call_parent(zis, obj, 64, stak);// rsrc_func64
	if ( res )
	{
		int width = find_id_in_stack_def_val(0x80002002, 0, stak);
		int height = find_id_in_stack_def_val(0x80002003, 0, stak);
		int v22 = find_id_in_stack_def_val(0x80002006, 0, stak);
		int v20 = find_id_in_stack_def_val(0x80002008, 0, stak);
		int v21 = find_id_in_stack_def_val(0x80002009, 0, stak);// software surface


		if ( (width && height) || v22 )
		{
			bitmap_intern *intern = (bitmap_intern *)AllocVec(sizeof(bitmap_intern), 65537);

			if ( intern )
			{
				if (v22)
				{
					intern->flags |= BITMAP_FLAG_HAS_PALETTE;
					intern->pallete = (BYTE *)AllocVec(256 * 3, 65537);
				}

				if ( !v22 || intern->pallete != NULL )
				{
					if ( width && height )
					{
						intern->width = width;
						intern->height = height;
						if ( v20 && engines___bitmap->display___win3d )
						{
							intern->width = width;
							intern->flags = intern->flags | 2;
							intern->height = height;

							if ( v21 )
								intern->flags = intern->flags | BITMAP_FLAG_SOFTWARE;

							// allocate buffer, create palette, surface and texture
							if ( ! call_method( engines___bitmap->display___win3d, 266, &intern) )
							{
								nc_FreeMem(intern);
								return res;
							}
						}
						else
						{
							void *buf = (void *)find_id_in_stack_def_val(0x80002005, 0, stak);
							intern->pitch = width;
							if ( buf )
							{
								intern->buffer = buf;
								intern->flags = intern->flags | 1;
							}
							else
							{
								intern->buffer = AllocVec(height * width, 65537);
								if ( !intern->buffer )
								{
									nc_FreeMem(intern);
									return res;
								}
							}
						}
					}
					res->data = intern;
				}
				else
				{
					nc_FreeMem(intern);
				}
			}
		}
	}
	return res;
}

void bitmap_func65(NC_STACK_bitmap *obj, class_stru *zis, rsrc **pres)
{
	bitmap_intern *intern = (bitmap_intern *)(*pres)->data;
	rsrc *res = *pres;
	if ( intern )
	{
		if ( intern->flags & 2 && engines___bitmap->display___win3d )
		{
			call_method(engines___bitmap->display___win3d, 268, &intern);
		}
		else if ( !(intern->flags & 1) )
		{
			if ( intern->buffer )
				nc_FreeMem(intern->buffer);
		}

		if ( intern->pallete )
			nc_FreeMem(intern->pallete);

		nc_FreeMem(intern);
		res->data = NULL;
	}
	call_parent(zis, obj, 65, (stack_vals *)pres);
}

size_t bitmap_func128(NC_STACK_bitmap *, class_stru *, stack_vals *)
{
    return 0;
}

size_t bitmap_func129(NC_STACK_bitmap *, class_stru *, stack_vals *)
{
    return 0;
}

void bitmap_func130(NC_STACK_bitmap *, class_stru *, stack_vals *)
{
////TODO
}


class_return bitmap_class_descr;

class_return * class_set_bitmap(int a1, ...)
{

	stack_vals vals[128];

	if (a1 != 0)
	{
		va_list va;
		va_start(va, a1);

		va_to_arr(vals, 128, a1, va);

		va_end(va);
	}

	engines___bitmap = (g_engines *)find_id_in_stack_def_val(0x80000002, 0, vals);

	memset(bitmap_funcs, 0, sizeof(CLASSFUNC) * 1024);

	bitmap_funcs[0] = (CLASSFUNC)bitmap_func0;
	bitmap_funcs[1] = (CLASSFUNC)bitmap_func1;
	bitmap_funcs[2] = (CLASSFUNC)bitmap_func2;
	bitmap_funcs[3] = (CLASSFUNC)bitmap_func3;
	bitmap_funcs[64] = (CLASSFUNC)bitmap_func64;
	bitmap_funcs[65] = (CLASSFUNC)bitmap_func65;
	bitmap_funcs[128] = (CLASSFUNC)bitmap_func128;
	bitmap_funcs[129] = (CLASSFUNC)bitmap_func129;
	bitmap_funcs[130] = (CLASSFUNC)bitmap_func130;

	bitmap_class_descr.parent = "rsrc.class";

	bitmap_class_descr.vtbl = bitmap_funcs;
	////bitmap_class_descr.varSize = sizeof(__NC_STACK_bitmap);
	bitmap_class_descr.varSize = sizeof(NC_STACK_bitmap) - offsetof(NC_STACK_bitmap, stack__bitmap); //// HACK
	bitmap_class_descr.field_A = 0;
	return &bitmap_class_descr;
}
