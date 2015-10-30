#ifndef MC_H_INCLUDED
#define MC_H_INCLUDED

enum MC_TYPE
{
	MC_TYPE_RES = 0x0,
	MC_TYPE_CLASS = 0x1,
	MC_TYPE_ENGINE = 0x2,
	MC_TYPE_UNKNOWN = 0x3
};

enum MC2
{
  MC2_GFX_ENGINE = 0x80000101,
  MC2_AUDIO_ENGINE = 0x80000102,
  MC2_INPUT_ENGINE = 0x80000301,
  MC2_TFORM_ENGINE = 0x80000401,
};


void init_mc_res_class_engine_strings(MC_TYPE a1, const char *str);

int set_classes_list();
stored_functions_engine *init_engine(unsigned int engineID);
void deinit_engine(unsigned int engineID);

void sb_0x411c08();
const char * get_MC_str(MC_TYPE a1);

#endif // MC_H_INCLUDED
