#ifndef  YWINT_H_INCLUDED
#define YWINT_H_INCLUDED

struct squadProto
{
	int field_0;
	int useable;
	int owner;
	int vehicle;
	int num;
	float pos_x;
	int field_18;
	float pos_z;
	int mb_status;
};

struct mapRobo
{
	int16_t owner;
	int16_t vehicle;
	char gap_4[4];
	float pos_x;
	float pos_y;
	float pos_z;
	int energy;
	char con_budget;
	char rad_budget;
	char pow_budget;
	char def_budget;
	int reload_const;
	char saf_budget;
	char rec_budget;
	char cpl_budget;
	char rob_budget;
	int mb_status;
	int16_t viewangle;
	int saf_delay;
	int pow_delay;
	int rad_delay;
	int cpl_delay;
	int def_delay;
	int con_delay;
	int rec_delay;
	int rob_delay;
};

struct dbmapProto
{
	int16_t size_x;
	int16_t size_y;
	char name[32];
};

struct mapProto
{
	char flags;
	char gap_1[3];
	int setNumber;
	int event_loop;
	int secXsize;
	int secYsize;
	int slow_connection;
	char sky[64];
	char typ[64];
	char own[64];
	char hgt[64];
	char blg[64];
	int mapRobos_count;
	mapRobo mapRobos[8];
	int squad_count;
	squadProto squads[96];
	char palettes[8][64];
	int mbmaps_count;
	dbmapProto mbmaps[4];
	int dbmap_count;
	dbmapProto dbmaps[4];
};

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

#endif
