#ifndef ILBM_H_INCLUDED
#define ILBM_H_INCLUDED

#include "bitmap.h"

extern class_stored ilbm_class_off;

struct __NC_STACK_ilbm
{
	char field_0;
	char field_1;
	char field_2;
	char field_3;
};

struct NC_STACK_ilbm : public NC_STACK_bitmap
{
	__NC_STACK_ilbm stack__ilbm;
};

struct __attribute__((packed)) BMHD_type
{
	WORD width;
	WORD height;
	WORD x;
	WORD y;
	char nPlanes;
	char masking;
	char compression;
	char pad1;
	WORD transparentColor;
	char xAspect;
	char yAspect;
	WORD pageWidth;
	WORD pageHeight;
};

struct __attribute__((packed)) VBMP_type
{
	WORD width;
	WORD height;
	WORD pad1;
};

struct ilbm_func66_arg
{
	const char *filename;
	MFILE *file;
	int OpenedStream;
};

#endif // ILBM_H_INCLUDED
