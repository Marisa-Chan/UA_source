#ifndef BITMAP_H_INCLUDED
#define BITMAP_H_INCLUDED

#include "rsrc.h"

extern class_stored bitmap_class_off;

struct texStru;

struct bitmap_intern
{
	void *buffer;
	__int16 width;
	__int16 height;
	int pitch;
	BYTE *pallete;
	char flags;
	char field_11;
	__int16 field_12;
	texStru *ddrawSurfTex;
	int field_18;
	int field_1C;
};

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

struct bitmap_opl2
{
	float field_0;
	float field_4;
};

union bitmap_uniopl
{
	bitmap__opl *opl1;
	bitmap_opl2 *opl2;
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
	bitmap_opl2 *opl2;
};

#endif // BITMAP_H_INCLUDED
