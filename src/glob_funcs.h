#ifndef GLOB_FUNCS_H_INCLUDED
#define GLOB_FUNCS_H_INCLUDED

#include <string>
#include <list>
#include "MFILE.h"
#include "MC.h"
#include "ini.h"

struct TKVPair
{
    std::string name;
    std::string value;
};

struct g_engines
{
    int __AllocCount;
    int __AllocSize;
    int __AllocMax;
    int file_handles;
    std::string MC_RES_CLASS_ENGINE[3];
    const char *some_params_pointers[32];
    int some_params_count;
    NC_STACK_display *display___win3d;
    std::list<TKVPair *> kvPairs;

    g_engines()
    {
        __AllocCount = 0;
        __AllocSize = 0;
        __AllocMax = 0;
        file_handles = 0;

        for (int i = 0; i < 32; i++)
            some_params_pointers[i] = NULL;

        some_params_count = 0;
        display___win3d = NULL;
    }
};


void nc_FreeMem(void *);
void *AllocVec(size_t size, int a2);



const char * get_prefix_replacement(const char *prefix);
void set_prefix_replacement(const char *str1, const char *str2);

char * file_path_copy_manipul(const char *src, char *dst, int size);


void sub_412810(const char *a1, char *a2, int num);

void fpsLimitter(int value);


#endif // GLOB_FUNCS_H_INCLUDED
