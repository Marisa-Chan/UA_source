#ifndef EMBED_H_INCLUDED
#define EMBED_H_INCLUDED

#include "nucleas.h"
#include "nlist.h"

extern class_stored embed_class_off;

struct __NC_STACK_embed
{
	nlist embed_objects;
};

struct NC_STACK_embed : public NC_STACK_nucleus
{
	__NC_STACK_embed stack__embed;
};

struct embd_node: public nnode
{
	int num;
	NC_STACK_class *objects[32];
};

#endif // EMBED_H_INCLUDED
