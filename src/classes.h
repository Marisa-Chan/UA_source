#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include "nlist.h"

#include "MFILE.h"

struct stack_vals
{
    unsigned int id;
    size_t value;
};

struct stored_functions_engine
{
    int (*init)();
    void (*deinit)();
    void (*setter)(unsigned int, ...);
    void (*getter)(unsigned int, ...);
};

#include "nucleas.h"


size_t call_method(NC_STACK_nucleus *a1, int idx, void *a3);
size_t call_method(NC_STACK_nucleus *a1, int idx);
size_t call_vtbl(NC_STACK_nucleus *a1, int idx, ...);

NC_STACK_nucleus * init_get_class(const char *classname, ...);
int delete_class_obj(NC_STACK_nucleus *cls);
stack_vals * find_id_in_stack2(unsigned int id, stack_vals *a2);
size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, stack_vals *a3);

NC_STACK_nucleus * READ_OBJT(MFILE *mfile);
int sub_4117F8(NC_STACK_nucleus *obj, MFILE *mfile);

struct NC_STACK_base;

NC_STACK_base *READ_BAS_FILE(const char *fname);

#endif // CLASSES_H_INCLUDED
