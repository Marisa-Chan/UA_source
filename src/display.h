#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include "raster.h"

extern class_stored display_class_off;


struct __attribute__((packed)) UA_PALENTRY
{
	BYTE r;
	BYTE g;
	BYTE b;
};

struct UA_PALETTE
{
	UA_PALENTRY pal_entries[256];
};

struct __NC_STACK_display
{
	UA_PALENTRY palette[256];
	UA_PALETTE field_300[8];
	bitmap_intern *pointer_bitm;
	int field_1b04;
};

struct NC_STACK_display : public NC_STACK_raster
{
	__NC_STACK_display stack__display;
};

struct rstr_261_arg
{
	int pal_id;
	int entrie_id;
	int pal_num;
	UA_PALENTRY *pal_entries;
};

struct rstr_262_arg
{
	int dword0;
	int *pdword4;
	int *pdword8;
};

struct displ_arg263
{
	bitmap_intern *bitm;
	int pointer_id;
};


#endif // DISPLAY_H_INCLUDED
