#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "nucleas.h"

extern class_stored button_class_off;

struct NC_STACK_button;

struct button_str1
{
  int16_t xpos;
  int16_t ypos;
  int16_t width;
  int16_t fnt_height;
};

struct __NC_STACK_button : public nnode
{
    int16_t btn_xpos;
  int16_t btn_ypos;
  int16_t btn_width;
  int16_t btn_height;
  int field_10;
  int field_14;
  button_str1 *field_18[48];
//  button_str2 *field_d8[48];
//  __int16 idd;
//  __int16 field_19A;
//  char field_19c;
//  char field_19D;
//  char field_19E;
//  char field_19F;
//  char field_1A0;
//  char field_1A1;
//  __int16 screen_width;
//  __int16 screen_height;
};

struct NC_STACK_button : public NC_STACK_nucleus
{
	__NC_STACK_button stack__button;
};

#endif // BUTTON_H_INCLUDED
