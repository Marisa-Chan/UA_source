#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"

extern class_stored bitmap_class_off;

struct texStru;

struct bitmap__opl
{
	__int16 field_0;
	__int16 field_2;
	__int16 field_4;
	__int16 field_6;
	__int16 field_8;
	__int16 field_A;
	__int16 field_C;
	__int16 field_E;
};

struct tUtV
{
	float tu;
	float tv;
};

union bitmap_uniopl
{
	bitmap__opl *opl1;
	tUtV *opl2;
};

struct __NC_STACK_bitmap
{
	bitmap_intern *bitm_intern;
	bitmap_uniopl field_4;
	int field_8;
};

struct NC_STACK_bitmap : public NC_STACK_rsrc
{
	__NC_STACK_bitmap stack__bitmap;
};

enum BITMAP_FLAG
{
	BITMAP_FLAG_HAS_PALETTE  = 0x4,
	BITMAP_FLAG_SOFTWARE  = 0x8
};

struct bitmap_arg130
{
	int field_0;
	int field_4;
	bitmap_intern *pbitm;
	tUtV *opl2;
};

#endif // BITMAP_H_INCLUDED
