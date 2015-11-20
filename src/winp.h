#ifndef WINP_H_INCLUDED
#define WINP_H_INCLUDED

#include "iwimp.h"
#include "utils.h"

extern class_stored winp_class_off;

struct NC_STACK_winp;

struct __NC_STACK_winp
{
	HWND hWND;
	int field_4;
	int field_8;
	int field_c;
	BYTE field_10[48];
};

struct NC_STACK_winp : public NC_STACK_iwimp
{
	__NC_STACK_winp stack__winp;
};



struct winp_66arg
{
	DWORD downed_key_2;
	DWORD downed_key;
	DWORD dword8;
	DWORD chr;
};

struct winp__func67__internal
{
	const char *keyname;
	int keytype;
	int keycode;
	int down;
};

struct winp_68arg
{
	const char *keyname;
	DWORD id;
};

struct winp_s2000
{
	HWND hwnd;
	POINT pos;
};

struct win_64arg
{
	int field_0;
	int field_4;
	float field_8;
};

struct winp_71arg
{
  DWORD effID;
  DWORD state;
  float p1;
  float p2;
  float p3;
  float p4;
};

#endif // WINP_H_INCLUDED
