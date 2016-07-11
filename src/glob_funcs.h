#ifndef GLOB_FUNCS_H_INCLUDED
#define GLOB_FUNCS_H_INCLUDED

#include "MFILE.h"
#include "MC.h"
#include "classes.h"
#include "ini.h"

struct g_engines
{
    int __AllocCount;
    int __AllocSize;
    int __AllocMax;
    int file_handles;
    stored_functions_engine *setted_gfx_engine;
    stored_functions_engine *setted_audio_engine;
    stored_functions_engine *setted_input_engine;
    stored_functions_engine *setted_tform_engine;
    char MC_RES_CLASS_ENGINE[3][256];
    const char *some_params_pointers[32];
    int some_params_count;
    NC_STACK_display *display___win3d;
    int field_3D4;
    nlist stru_525D68;
};


void nc_FreeMem(void *);
void *AllocVec(size_t size, int a2);

struct nnode_str: nnode
{
    char str[32];
    char str2[32];
};

const char * get_prefix_replacement(const char *prefix);
int set_prefix_replacement(const char *str1, const char *str2);

FILE * FOpen(const char *src_path, const char *mode);
int FClose(FILE *a1);

char * file_path_copy_manipul(const char *src, char *dst, int size);


struct ncDir
{
    HANDLE hndl;
    int attr;
    int field_8;
    int field_C;
    int field_10;
    char field_14;
    char field_15;
    WORD field_16;
    WORD field_18;
    int field_1A;
    char cFileName[256];
    int16_t field_11E;
    char field_120;
};

struct dirEntry
{
    int field_0;
    int field_4;
    char e_name[256];
};

ncDir *OpenDir(const char *dir);
dirEntry *ReadDir(ncDir *_dir, dirEntry *dir_entry);
void CloseDir(ncDir *dir);

void sub_412810(const char *a1, char *a2, int num);
int delete_file(const char *path);
int removeDirectory(const char *path);
int createDirectory(const char *a1);

#endif // GLOB_FUNCS_H_INCLUDED
