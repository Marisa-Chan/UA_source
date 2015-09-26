#ifndef RSRC_H_INCLUDED
#define RSRC_H_INCLUDED

extern class_stored rsrc_class_off;

struct rsrc : class_named_nnode
{
  char title[64];
  __int16 ref_cnt;
  __int16 what_list;
  void *data;
  int class_name;
};

struct __NC_STACK_rsrc
{
	rsrc *p_rsrc;
	void *p_data;
	char field_8[4];
};

struct NC_STACK_rsrc : public NC_STACK_nucleus
{
    __NC_STACK_rsrc stack__rsrc;
};

#endif // RSRC_H_INCLUDED
