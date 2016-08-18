#ifndef MC_H_INCLUDED
#define MC_H_INCLUDED

enum MC_TYPE
{
    MC_TYPE_RES = 0x0,
    MC_TYPE_CLASS = 0x1,
    MC_TYPE_ENGINE = 0x2,
    MC_TYPE_UNKNOWN = 0x3
};

void init_mc_res_class_engine_strings(MC_TYPE a1, const char *str);

int set_classes_list();

void sb_0x411c08();
const char * get_MC_str(MC_TYPE a1);

const NewClassDescr * getClassAllocator(const char *name);

#endif // MC_H_INCLUDED
