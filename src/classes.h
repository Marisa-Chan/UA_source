#ifndef CLASSES_H_INCLUDED
#define CLASSES_H_INCLUDED

#include "nlist.h"

#include "MFILE.h"

int class_def_deinit();

struct stack_vals
{
	unsigned int id;
	size_t value;
};


struct class_stru;

typedef  size_t ( *CLASSFUNC)(void *, class_stru *, stack_vals *);

typedef void * (*class_return_vtbl)();

struct class_return
{
	const char *parent;
	CLASSFUNC *vtbl;
	__int16 varSize;
	int field_A;
};


struct clvt
{
	CLASSFUNC cl_func;
	class_stru *p_cl;
};

struct stored_functions
{
	class_return *(*init)(int, ...);
	int (*deinit)();

	stored_functions(class_return *(*_init)(int, ...))
	{
		init = _init;
		deinit = class_def_deinit;
	}
};

struct stored_functions_engine
{
	int (*init)(unsigned int, ...);
	void (*deinit)();
	void (*setter)(unsigned int, ...);
	void (*getter)(unsigned int, ...);
};

struct class_named_nnode : public nnode
{
    char field_8;
	char field_9;
	char *name;
};

struct class_stored : public class_named_nnode
{
	stored_functions *( *get_stored_functions)();

	class_stored(nnode *_next, nnode *_prev, const char *_name,  stored_functions *( *_func)() )
	{
		next = _next;
		prev = _prev;
		name = (char *)_name;
		get_stored_functions = _func;
	}
};

struct unk_class
{
	class_stored *node;
	stored_functions *func;
};

struct class_stru : public class_named_nnode
{
	class_stru *parent_class;
	__int16 stack_offset;
	__int16 class_stack_size;
	__int16 field_16;
	__int16 field_18;
	int ref_count;
	int ref_cnt;
	int class_inst_count;
	int clret_fieldA;
	clvt *clvtbl;
	unk_class class_descriptor_in_list;
};

struct NC_STACK_class
{
	class_stru *class_owner;
	char *NAME;
};

class_stored * FIND(nlist *list, const char *name);
stored_functions * find_class(const char *name, unk_class *out);
size_t call_method(NC_STACK_class *a1, int idx, void *a3);
size_t call_method(NC_STACK_class *a1, int idx);
size_t call_vtbl(NC_STACK_class *a1, int idx, ...);

NC_STACK_class * init_get_class(const char *classname, ...);
int delete_class_obj(NC_STACK_class *cls);
stack_vals * find_id_in_stack2(unsigned int id, stack_vals *a2);
size_t find_id_in_stack_def_val(unsigned int find_id, size_t def_value, stack_vals *a3);
size_t call_parent(class_stru *zis, void *caller, int idx, stack_vals *stk);

NC_STACK_class * READ_OBJT(MFILE *mfile);
int sub_4117F8(NC_STACK_class *obj, MFILE *mfile);

#endif // CLASSES_H_INCLUDED
