#ifndef  YWINT_H_INCLUDED
#define YWINT_H_INCLUDED

extern polys *p_outPolys;
extern polysDat *p_polysdata;
extern polysDat *p_polysdata_end;

struct lvlnet_t1
{
	int field_0;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
};


int VhclProtoParser(scrCallBack *);
int WeaponProtoParser(scrCallBack *);
int BuildProtoParser(scrCallBack *);

int MoviesParser(scrCallBack *arg);
int BkgParser(scrCallBack *arg);
int Colors_Parser(scrCallBack *arg);
int Misc_Parse(scrCallBack *arg);
int SuperItem_Parse(scrCallBack *arg);

int LevelDataParser(scrCallBack *arg);
int MapParseSizes(scrCallBack *arg);
int parse_map_robos(scrCallBack *arg);

int yw_InitLevelNet(_NC_STACK_ypaworld *yw);
int yw_InitNetwork(_NC_STACK_ypaworld *yw);

void yw_setIniColor(_NC_STACK_ypaworld *yw, int color_id, int r, int g, int b, int i);
int yw_parse_color(_NC_STACK_ypaworld *yw, int color_number, char *color_string);
int yw_ScanLevels(_NC_STACK_ypaworld *yw);


int sub_4DA354(_NC_STACK_ypaworld *yw, const char *filename);
int yw_InitTooltips(_NC_STACK_ypaworld *yw);

int loadTOD(_NC_STACK_ypaworld *yw, const char *fname);
int writeTOD(_NC_STACK_ypaworld *yw, const char *fname, int tod);

void ypaworld_func158__sub4__sub0(_NC_STACK_ypaworld *yw, NC_STACK_ilbm *bitm);

void sb_0x4eb94c(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc, int object_id, int a5);
void ypaworld_func158__DrawVehicle(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc);

#endif
