#ifndef ENGINE_INPUT_H_INCLUDED
#define ENGINE_INPUT_H_INCLUDED

extern class_stored input_engine_off;

#include "utils.h"
#include "button.h"

struct winp_131arg
{
	int flag;
	__NC_STACK_button *selected_btn;
	int selected_btnID;
	shortPoint move[3];
	shortPoint ldw_pos[3];
	shortPoint lup_pos[3];
};

struct struC5
{
	int period;
	char downed_key_2;
	char downed_key;
	char dword8;
	char chr;
	char field_8;
	int16_t field_9;
	int16_t field_B;
	int field_D;
	float sliders_vars[32];
	int but_flags;
	winp_131arg winp131arg;
};

void sub_412D28(struC5 *a1);
void sub_412D48(__NC_STACK_button *btn, int a2);
void sub_412D9C(__NC_STACK_button *btn);


#endif // ENGINE_INPUT_H_INCLUDED
