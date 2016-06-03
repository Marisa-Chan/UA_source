#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "classes.h"
#include "includes.h"
#include "engine_input.h"
#include "windp.h"




stored_functions *classvtbl_get_windp();
class_return * class_set_windp(int, ...);

stored_functions windp_class_vtbl(class_set_windp);


class_stored windp_class_off (NULL, NULL, "MC2classes:windp.class", classvtbl_get_windp);


stored_functions *classvtbl_get_windp()
{
    return &windp_class_vtbl;
}

CLASSFUNC windp_funcs[1024];




NC_STACK_windp * windp_func0(class_stru *clss, class_stru *zis, stack_vals *stak)
{
    NC_STACK_windp *obj = (NC_STACK_windp *)call_parent(zis, clss, 0, stak);

    return obj;
}

size_t windp_func1(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return call_parent(zis, obj, 1, stak);
}

void windp_func3(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    call_parent(zis, obj, 3, stak);
}

size_t windp_func64(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func65(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func66(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func67(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func68(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func69(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func70(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func71(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func72(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func73(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func74(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func75(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func76(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func77(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func78(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func79(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func80(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func81(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func82(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func83(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func84(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func85(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func86(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func87(NC_STACK_windp *obj, class_stru *zis, windp_arg87 *arg)
{
    printf("STUB %s\n","windp_func87");
    arg->field_41 = 0;

    return 1;
}

size_t windp_func88(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func89(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func90(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}

size_t windp_func91(NC_STACK_windp *obj, class_stru *zis, stack_vals *stak)
{
    return 0;
}


class_return windp_class_descr;

class_return * class_set_windp(int , ...)
{

    memset(windp_funcs, 0, sizeof(CLASSFUNC) * 1024);

    windp_funcs[0] = (CLASSFUNC)windp_func0;
    windp_funcs[1] = (CLASSFUNC)windp_func1;
    windp_funcs[3] = (CLASSFUNC)windp_func3;
    windp_funcs[64] = (CLASSFUNC)windp_func64;
    windp_funcs[65] = (CLASSFUNC)windp_func65;
    windp_funcs[66] = (CLASSFUNC)windp_func66;
    windp_funcs[67] = (CLASSFUNC)windp_func67;
    windp_funcs[68] = (CLASSFUNC)windp_func68;
    windp_funcs[69] = (CLASSFUNC)windp_func69;
    windp_funcs[70] = (CLASSFUNC)windp_func70;
    windp_funcs[71] = (CLASSFUNC)windp_func71;
    windp_funcs[72] = (CLASSFUNC)windp_func72;
    windp_funcs[73] = (CLASSFUNC)windp_func73;
    windp_funcs[74] = (CLASSFUNC)windp_func74;
    windp_funcs[75] = (CLASSFUNC)windp_func75;
    windp_funcs[76] = (CLASSFUNC)windp_func76;
    windp_funcs[77] = (CLASSFUNC)windp_func77;
    windp_funcs[78] = (CLASSFUNC)windp_func78;
    windp_funcs[79] = (CLASSFUNC)windp_func79;
    windp_funcs[80] = (CLASSFUNC)windp_func80;
    windp_funcs[81] = (CLASSFUNC)windp_func81;
    windp_funcs[82] = (CLASSFUNC)windp_func82;
    windp_funcs[83] = (CLASSFUNC)windp_func83;
    windp_funcs[84] = (CLASSFUNC)windp_func84;
    windp_funcs[85] = (CLASSFUNC)windp_func85;
    windp_funcs[86] = (CLASSFUNC)windp_func86;
    windp_funcs[87] = (CLASSFUNC)windp_func87;
    windp_funcs[88] = (CLASSFUNC)windp_func88;
    windp_funcs[89] = (CLASSFUNC)windp_func89;
    windp_funcs[90] = (CLASSFUNC)windp_func90;
    windp_funcs[91] = (CLASSFUNC)windp_func91;

    windp_class_descr.parent = "network.class";

    windp_class_descr.vtbl = windp_funcs;
    ////windp_class_descr.varSize = sizeof(__NC_STACK_windp);
    windp_class_descr.varSize = sizeof(NC_STACK_windp) - offsetof(NC_STACK_windp, stack__windp); //// HACK
    windp_class_descr.field_A = 0;
    return &windp_class_descr;
}






















void sb_0x4c9f14(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sb_0x4c9f14");
}

void sub_46B328(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sub_46B328");
}

void sub_46B1B8(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sub_46B1B8");
}

void sb_0x46bb54(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sb_0x46bb54");
}

void sub_46D698(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sub_46D698");
}

void ypaworld_func158__sub1(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub1");
}

void ypaworld_func158__sub2(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub2");
}

int ypaworld_func158__sub0__sub8(UserData *usr, const char**, const char**)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func158__sub0__sub8");
    return 0;
}

void sb_0x4deac0(UserData *usr)
{
    dprintf("MAKE ME %s (multiplayer)\n", "sb_0x4deac0");
}

void yw_netcleanup(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "yw_netcleanup");
}

void ypaworld_func64__sub18(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub18");
}

void ypaworld_func64__sub10(_NC_STACK_ypaworld *yw)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub10");
}




//////////////multiplayer wnd///////////
netgamelst netgame_wnd;
char b_stru_5C85C0__cmdbuf[1024];
button_str1 b_stru_5C85C0__btn0;
button_str1 b_stru_5C85C0__btn1;


void sb_0x451034__sub6(_NC_STACK_ypaworld *yw)
{
    netgame_wnd.cmd_flag = 0x138;
    netgame_wnd.frm_1.btn_xpos = 0;
    netgame_wnd.frm_1.btn_ypos = 2 * yw->screen_height / 3;
    netgame_wnd.frm_1.field_18[0] = &b_stru_5C85C0__btn0;
    netgame_wnd.frm_1.field_18[1] = &b_stru_5C85C0__btn1;
    netgame_wnd.frm_1.btn_width = yw->screen_width - 1;
    netgame_wnd.frm_1.btn_height = 2 * yw->font_default_h + 2;
    netgame_wnd.frm_1.field_10 = 2;


    netgame_wnd.field_1CC = yw->tiles[0]->chars[97].width;
    netgame_wnd.field_1D0 = 2 * yw->font_default_h;
    netgame_wnd.field_1D4 = yw->screen_width;
    netgame_wnd.field_1D8 = netgame_wnd.field_1D0;
    netgame_wnd.field_1DC[0] = 0;
    netgame_wnd.field_21C = 0;

    netgame_wnd.cmdstrm.includ = NULL;
    netgame_wnd.cmdstrm.cmdbuf = b_stru_5C85C0__cmdbuf;

    b_stru_5C85C0__btn0.xpos = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.ypos = 0;
    b_stru_5C85C0__btn0.width = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.fnt_height = yw->font_default_h;

    b_stru_5C85C0__btn1.xpos = 0;
    b_stru_5C85C0__btn1.ypos = 0;
    b_stru_5C85C0__btn1.width = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn1.fnt_height = yw->font_default_h;
}

//netgui update
void ypaworld_func64__sub7__sub5(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub7__sub5");
}
