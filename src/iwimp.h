#ifndef IWIMP_H_INCLUDED
#define IWIMP_H_INCLUDED

#include "idev.h"
#include "button.h"
#include "utils.h"

extern class_stored iwimp_class_off;

struct NC_STACK_iwimp;

struct __NC_STACK_iwimp
{
	int field_0;
	nlist list;
	__NC_STACK_button *selected_btn;
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
	int field_8;
};

struct winp_131arg
{
	int flag;
	__NC_STACK_button *selected_btn;
	int selected_btnID;
	shortPoint move[3];
	shortPoint ldw_pos[3];
	shortPoint lup_pos[3];
};

#endif // IWIMP_H_INCLUDED
