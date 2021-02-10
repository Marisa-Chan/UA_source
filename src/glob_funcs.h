#ifndef GLOB_FUNCS_H_INCLUDED
#define GLOB_FUNCS_H_INCLUDED

#include <string>
#include <list>
#include "IFFile.h"
#include "MC.h"
#include "ini.h"


struct TKVPair
{
    std::string name;
    std::string value;
};

struct g_engines
{
    const char *some_params_pointers[32];
    int some_params_count;
    NC_STACK_display *display___win3d;
    std::list<TKVPair> kvPairs;

    g_engines()
    {
        for (int i = 0; i < 32; i++)
            some_params_pointers[i] = NULL;

        some_params_count = 0;
        display___win3d = NULL;
    }
};


void nc_FreeMem(void *);
void *AllocVec(size_t size, int a2);



const char * get_prefix_replacement(const char *prefix);
std::string get_prefix_replacement(const std::string &prefix);
void set_prefix_replacement(const std::string &str1, const std::string &str2);
std::string SetPathKeys(const std::string &name, const std::string &value);

std::string file_path_copy_manipul(const std::string &src);


void sub_412810(const std::string &src, std::string &dst);

void fpsLimitter(int value);
bool tuneGetWeaponRadiusFix();


#endif // GLOB_FUNCS_H_INCLUDED
