#ifndef ADE_H_INCLUDED
#define ADE_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

extern class_stored ade_class_off;

struct NC_STACK_ade;


struct __attribute__((packed)) ADE_STRC
{
  __int16 field_0;
  char field_2;
  char field_3;
  __int16 field_4;
  __int16 field_6;
  __int16 field_8;
};


struct __NC_STACK_ade : public nnode
{
	NC_STACK_ade *self;
	char flags;
	char field_D;
	__int16 strc_f4;
	__int16 strc_f6;
};

struct NC_STACK_ade : public NC_STACK_nucleus
{
	__NC_STACK_ade stack__ade;
};

#endif // ADE_H_INCLUDED
