#ifndef RASTER_H_INCLUDED
#define RASTER_H_INCLUDED

extern class_stored raster_class_off;

struct xyxyNNN
{
  int x1;
  int y1;
  int x2;
  int y2;
  int NNN;
};

struct tile_xy
{
  int chr;
  int width;
};

struct tiles_stru
{
  void *font_image; //FFFFFIX ME
  int field_4;
  int field_8;
  tile_xy *chars;
  int font_height;
  int field_12;
  int field_14;
};

struct ua_dRect
{
    int x1;
    int y1;
    int x2;
    int y2;
};

struct ua_fRect
{
    float x1;
    float y1;
    float x2;
    float y2;
};

struct __NC_STACK_raster
{
	bitmap_intern *bitm_intern;
	DWORD field_4; // Color?
	DWORD field_8; // and Color2 ?
//  int field_c;
//  int field_10;
//  int field_14;
//  int field_18;
//  int field_1c;
//  int field_20;
    xyxyNNN field_24;
    xyxyNNN field_38;
//  int field_4c;
//  int field_50;
//  int field_54;
//  int field_58;
//  int field_5c;
//  int field_60;
//  int field_64;
//  int field_68;
//  int field_6c;
//  int field_70;
//  int field_74;
//  int field_78;
//  int field_7c;
//  int field_80;
//  int field_84;
//  int field_88;
//  int field_8c;
//  int field_90;
//  int field_94;
//  int field_98;
//  int field_9c;
//  int field_a0;
//  int field_a4;
//  int field_a8;
//  int field_ac;
//  int field_b0;
//  int field_b4;
//  int field_b8;
//  int field_bc;
//  int field_c0;
//  int field_c4;
//  int field_c8;
//  int field_cc;
//  int field_d0;
//  int field_d4;
//  int field_d8;
//  int field_dc;
//  int field_e0;
//  int field_e4;
//  int field_e8;
//  int field_ec;
//  int field_f0;
//  int field_f4;
//  int field_f8;
//  int field_fc;
//  int field_100;
//  int field_104;
//  int field_108;
//  int field_10c;
//  int field_110;
//  int field_114;
//  int field_118;
//  int field_11c;
//  int field_120;
//  int field_124;
//  int field_128;
//  int field_12c;
//  int field_130;
//  int field_134;
//  int field_138;
//  int field_13c;
//  int field_140;
//  int field_144;
//  int field_148;
	tiles_stru *tiles[172];
//  int field_3fc;
//  int field_400;
//  int field_404;
//  int field_408;
//  int field_40c;
//  int field_410;
//  int field_414;
//  int field_418;
//  int field_41c;
//  int field_420;
//  int field_424;
//  int field_428;
//  int field_42c;
//  int field_430;
//  int field_434;
//  int field_438;
//  int field_43c;
//  int field_440;
//  int field_444;
//  int field_448;
//  int field_44c;
//  int field_450;
//  int field_454;
//  int field_458;
//  int field_45c;
//  int field_460;
//  int field_464;
//  int field_468;
//  int field_46c;
//  int field_470;
//  int field_474;
//  int field_478;
//  int field_47c;
//  int field_480;
//  int field_484;
//  int field_488;
//  int field_48c;
//  int field_490;
//  int field_494;
//  int field_498;
//  int field_49c;
//  int field_4a0;
//  int field_4a4;
//  int field_4a8;
//  int field_4ac;
//  int field_4b0;
//  int field_4b4;
//  int field_4b8;
//  int field_4bc;
//  int field_4c0;
//  int field_4c4;
//  int field_4c8;
//  int field_4cc;
//  int field_4d0;
//  int field_4d4;
//  int field_4d8;
//  int field_4dc;
//  int field_4e0;
//  int field_4e4;
//  int field_4e8;
//  int field_4ec;
//  int field_4f0;
//  int field_4f4;
//  int field_4f8;
//  int field_4fc;
//  int field_500;
//  int field_504;
//  int field_508;
//  int field_50c;
//  int field_510;
//  int field_514;
//  int field_518;
//  int field_51c;
//  int field_520;
//  int field_524;
//  int field_528;
//  int field_52c;
//  int field_530;
//  int field_534;
//  int field_538;
//  int field_53c;
//  int field_540;
//  int field_544;
//  int field_548;
	int field_54c;
	int field_550;
	float field_554;
	float field_558;
//  int field_55c;
//  int field_560;
//  int field_564;
//  int field_568;
//  int field_56c;
//  int field_570;
//  int field_574;
//  int field_578;
//  int field_57c;
//  int field_580;
//  int field_584;
//  int field_588;
//  int field_58c;
//  int field_590;
//  int field_594;
//  int field_598;
//  int field_59c;
};

struct NC_STACK_raster : public NC_STACK_bitmap
{
	__NC_STACK_raster stack__raster;
};


struct w3d_func198arg
{
	float x1;
	float y1;
	float x2;
	float y2;
};

struct w3d_func199arg
{
	int x1;
	int y1;
	int x2;
	int y2;
};

struct rstr_arg204
{
  bitmap_intern *pbitm;
  float float4;
  float float8;
  float floatC;
  float float10;
  float float14;
  float float18;
  float float1C;
  float float20;
};

struct rstr_loc204
{
  bitmap_intern *pbitm;
  int dword4;
  int dword8;
  int dwordC;
  int dword10;
  int dword14;
  int dword18;
  int dword1C;
  int dword20;
  int dword24;
};

struct rstr_218_arg
{
  bitmap_intern *bitm_intern;
  bitmap_intern *bitm_intern2;
  int flg;
  ua_fRect rect1;
  ua_fRect rect2;
};

struct rstr_262_arg
{
  int dword0;
  int *pdword4;
  int *pdword8;
};

#endif // RASTER_H_INCLUDED
