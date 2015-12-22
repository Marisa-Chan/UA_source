#ifndef BMPANM_H_INCLUDED
#define BMPANM_H_INCLUDED

#include "rsrc.h"
#include "engine_gfx.h"
#include "bitmap.h"

extern class_stored bmpanim_class_off;

struct texStru;

struct bmpAnim_t2
{
	tUtV *opls;
	bitmap_intern *bitm;
	int field_8;
	int16_t field_C;
	int16_t field_E;
};

struct bmpAnim_t1_objs
{
	NC_STACK_bitmap *clsObj;
	bitmap_intern *bitm_intern;
	char *title;
};

struct bmpAnim_t1
{
	char *titles;
	tUtV *field_4;
	bmpAnim_t1_objs *frames;
	char *className;
	bmpAnim_t2 *t2;
	bmpAnim_t2 *t2_end;
	int16_t t2_cnt;
	int16_t field_1A;
	int16_t field_1C;
	int16_t frame_count;
};

struct __NC_STACK_bmpanim
{
	bmpAnim_t1 *bmpanm_intern;
	bmpAnim_t2 *t2_;
	int field_8;
	int field_C;
	int16_t field_10;
	int16_t field_12;
};

struct NC_STACK_bmpanim : public NC_STACK_bitmap
{
	__NC_STACK_bmpanim stack__bmpanim;
};

struct __attribute__((packed)) bmpanm_loc
{
	int32_t a;
	int16_t frame_id;
	int16_t uv_id;
};

struct bmpanm_sv
{
	const char *resName;
	MFILE *mfile;
	int has_filename;
};

#endif // BITMAP_H_INCLUDED
