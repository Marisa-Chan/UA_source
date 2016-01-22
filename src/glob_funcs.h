#ifndef GLOB_FUNCS_H_INCLUDED
#define GLOB_FUNCS_H_INCLUDED

#include "MFILE.h"
#include "MC.h"
#include "classes.h"
#include "ini.h"


struct glob_funcs
{
    void *(*AllocVec)(int size);
    void (*nc_FreeMem)(void *);
    void (*init_list)(nlist *);
    void (*AddHead)(nlist *, nnode *);
    void (*AddTail)(nlist *, nnode *);
    void (*RemHead)(nlist *);
    void (*RemTail)(nlist *);
    void (*Remove)(nnode *);
    class_stored *(*FIND)(nlist *, const char *);
    void *(*_fopen)(const char *, const char *);
    void (*_fclose)(void *);
    void (*_fseek)(void *, int off, int orig);
    void (*_fread)(void *, int, int, void *FILE);
    void (*_fwrite)(void *, int, int, void *FILE);
    stack_vals *(*find_id_in_stack2)(unsigned int id, stack_vals *);
    int (*find_id_in_stack_def_val)(unsigned int id, int def_value, stack_vals *);
    MFILE *(*new_MFILE)();
    void (*del_MFILE)(MFILE *);
    void (*locret_412FBC)();
    void (*sub_412F98)(MFILE *, MFILE_flags);
    /*dd offset nullsub_1*/
    int (*sub_412FC0)(MFILE *, int a2, int a3, int a4);
    int (*sub_413290)(MFILE *);
    int (*mfread)(MFILE *, void *dst, int size);
    size_t (*sub_413564)(int a1, int a2, const void *a3);
    int (*read_next_IFF)(MFILE *, int a2);
    MFILE_S1 *(*GET_FORM_INFO_OR_NULL)(MFILE *);
    int (*read_default)(MFILE *);
    stored_functions *(*init_engine)(unsigned int);
    void (*deinit_engine)(unsigned int);
    int (*init_get_class)(const char *classname, ...);
    int (*delete_class_obj)(NC_STACK_class *);
    int (*call_vtbl)(NC_STACK_class *, int idx, ...);
    int (*call_method)(class_stru **a1, int a2, stack_vals *a3);
    int (*call_parent)(class_stru *a1, class_stru *caller, int idx, stack_vals *stk);
    int (*sub_4118A0)(NC_STACK_class *a5, const char *a1);
    void *(*READ_BAS_FILE)(const char *a1);
    int (*sub_4117F8)(NC_STACK_class *a1, MFILE *a2);
    void *(*READ_OBJT)(MFILE *a1);
    signed int (*get_keyvalue_from_ini)(const char *ini_filename, key_value_stru *key, unsigned int key_count);
    int (*_fgets)(char *a1, int buflen, FILE *a3);
    /*null*/
    int (*ypa_log_out)(const char *a3, ...);
    void (*init_mc_res_class_engine_strings)(MC_TYPE a1, const char *str);
    const char *(*get_MC_str)(MC_TYPE a1);
    bool (*delete_file)(const char *a1);
    int (*OpenDir)(const char *dir);
    int (*CloseDir)(int hndl);
    int (*ReadDir)(int a1, void *dir_entry);
    bool (*CreateDirectory)(const char *a1);
    bool (*RemoveDirectory)(const char *a1);

};

struct g_engines
{
    nlist allocated_class_list;
    int __AllocCount;
    int __AllocSize;
    int __AllocMax;
    int file_handles;
    stored_functions_engine *setted_gfx_engine;
    stored_functions_engine *setted_audio_engine;
    stored_functions_engine *setted_input_engine;
    stored_functions_engine *setted_tform_engine;
    unk_class gfx_engine__unk_class;
    unk_class audio_engine__unk_class;
    unk_class input_engine__unk_class;
    unk_class tform_engine__unk_class;
    char MC_RES_CLASS_ENGINE[3][256];
    const char *some_params_pointers[32];
    int some_params_count;
    NC_STACK_class *display___win3d;
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
