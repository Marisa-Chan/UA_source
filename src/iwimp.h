#ifndef IWIMP_H_INCLUDED
#define IWIMP_H_INCLUDED

#include "idev.h"
#include "engine_input.h"
#include "utils.h"

extern class_stored iwimp_class_off;

struct NC_STACK_iwimp;

struct __NC_STACK_iwimp
{
	int field_0;
	nlist list;
	inp_node *selected_btn;
	int selected_btnID;
};

struct NC_STACK_iwimp : public NC_STACK_idev
{
	__NC_STACK_iwimp stack__iwimp;
};

struct iwimp_arg129
{
	nnode *node;
	int field_4;
};


#endif // IWIMP_H_INCLUDED
