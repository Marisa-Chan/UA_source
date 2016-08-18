#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "includes.h"
#include "engine_input.h"
#include "windp.h"


const NewClassDescr NC_STACK_windp::description("windp.class", &newinstance);



size_t NC_STACK_windp::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak))
        return 0;

    return 1;
}

size_t NC_STACK_windp::func1(stack_vals *stak)
{
    return NC_STACK_nucleus::func1(stak);
}

size_t NC_STACK_windp::func3(stack_vals *stak)
{
    return NC_STACK_nucleus::func3(stak);
}

size_t NC_STACK_windp::windp_func64(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func65(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func66(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func67(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func68(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func69(int *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func70(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func71(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func72(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func73(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func74(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func75(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func76(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func77(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func78(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func79(windp_arg79 *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func80(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func81(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func82(windp_arg82 *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func83(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func84(int *arg)
{
    return 0;
}

size_t NC_STACK_windp::windp_func85(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func86(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func87(windp_arg87 *arg)
{
    printf("STUB %s\n","windp_func87");
    arg->field_41 = 0;

    return 1;
}

size_t NC_STACK_windp::windp_func88(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func89(const char **stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func90(stack_vals *stak)
{
    return 0;
}

size_t NC_STACK_windp::windp_func91(int *stak)
{
    return 0;
}


size_t NC_STACK_windp::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 3:
        func3( (stack_vals *)data );
        return 1;
    case 64:
        return (size_t)windp_func64( (stack_vals *)data );
    case 65:
        return (size_t)windp_func65( (stack_vals *)data );
    case 66:
        return (size_t)windp_func66( (stack_vals *)data );
    case 67:
        return (size_t)windp_func67( (stack_vals *)data );
    case 68:
        return (size_t)windp_func68( (stack_vals *)data );
    case 69:
        return (size_t)windp_func69( (int *)data );
    case 70:
        return (size_t)windp_func70( (stack_vals *)data );
    case 71:
        return (size_t)windp_func71( (stack_vals *)data );
    case 72:
        return (size_t)windp_func72( (stack_vals *)data );
    case 73:
        return (size_t)windp_func73( (stack_vals *)data );
    case 74:
        return (size_t)windp_func74( (stack_vals *)data );
    case 75:
        return (size_t)windp_func75( (stack_vals *)data );
    case 76:
        return (size_t)windp_func76( (stack_vals *)data );
    case 77:
        return (size_t)windp_func77( (stack_vals *)data );
    case 78:
        return (size_t)windp_func78( (stack_vals *)data );
    case 79:
        return (size_t)windp_func79( (windp_arg79 *)data );
    case 80:
        return (size_t)windp_func80( (stack_vals *)data );
    case 81:
        return (size_t)windp_func81( (stack_vals *)data );
    case 82:
        return (size_t)windp_func82( (windp_arg82 *)data );
    case 83:
        return (size_t)windp_func83( (stack_vals *)data );
    case 84:
        return (size_t)windp_func84( (int *)data );
    case 85:
        return (size_t)windp_func85( (stack_vals *)data );
    case 86:
        return (size_t)windp_func86( (stack_vals *)data );
    case 87:
        return (size_t)windp_func87( (windp_arg87 *)data );
    case 88:
        return (size_t)windp_func88( (stack_vals *)data );
    case 89:
        return (size_t)windp_func89( (const char **)data );
    case 90:
        return (size_t)windp_func90( (stack_vals *)data );
    case 91:
        return (size_t)windp_func91( (int *)data );
    default:
        break;
    }
    return NC_STACK_network::compatcall(method_id, data);
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

void ypaworld_func151__sub7(UserData *usr)
{
//  usr->p_ypaworld->windp->windp_func91(v2);
    log_netlog("---------------- YPA Network Statistics -------------------\n\n");
    log_netlog("Sending:\n");
//  log_netlog("   bytes per second: %d\n", usr->field_5487);
//  log_netlog("   bps minimum:      %d\n", usr->field_547F);
//  log_netlog("   bps maximum:      %d\n", usr->field_5483);
//  log_netlog("   packet size:      %d\n", usr->field_549F);
//  log_netlog("   packet minimum:   %d\n", usr->field_5493);
//  log_netlog("   packet maximum:   %d\n", usr->field_5497);
    log_netlog("receiving:\n");
//  log_netlog("   bytes per second: %d\n", usr->field_547B);
//  log_netlog("   bps minimum:      %d\n", usr->field_5473);
//  log_netlog("   bps maximum:      %d\n", usr->field_5477);
    log_netlog("statistical sr-thread info\n");
//  log_netlog("   max. in send list %d\n", v2[3]);
//  log_netlog("   max. in recv list %d\n", v2[2]);
    log_netlog("\nthe session ended at timeindex %d\n", usr->p_ypaworld->field_1614 / 1000);
    log_netlog("-----------------------------------------------------------\n");
}




//////////////multiplayer wnd///////////
netgamelst netgame_wnd;
char b_stru_5C85C0__cmdbuf[1024];
ButtonBox b_stru_5C85C0__btn0;
ButtonBox b_stru_5C85C0__btn1;


void sb_0x451034__sub6(_NC_STACK_ypaworld *yw)
{
    netgame_wnd.flags = 0x138;
    netgame_wnd.dialogBox.xpos = 0;
    netgame_wnd.dialogBox.ypos = 2 * yw->screen_height / 3;
    netgame_wnd.dialogBox.buttons[0] = &b_stru_5C85C0__btn0;
    netgame_wnd.dialogBox.buttons[1] = &b_stru_5C85C0__btn1;
    netgame_wnd.dialogBox.btn_width = yw->screen_width - 1;
    netgame_wnd.dialogBox.btn_height = 2 * yw->font_default_h + 2;
    netgame_wnd.dialogBox.field_10 = 2;


    netgame_wnd.field_1CC = yw->tiles[0]->chars[97].width;
    netgame_wnd.field_1D0 = 2 * yw->font_default_h;
    netgame_wnd.field_1D4 = yw->screen_width;
    netgame_wnd.field_1D8 = netgame_wnd.field_1D0;
    netgame_wnd.field_1DC[0] = 0;
    netgame_wnd.field_21C = 0;

    netgame_wnd.cmdstrm.includ = NULL;
    netgame_wnd.cmdstrm.cmdbuf = b_stru_5C85C0__cmdbuf;

    b_stru_5C85C0__btn0.x = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.y = 0;
    b_stru_5C85C0__btn0.w = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn0.h = yw->font_default_h;

    b_stru_5C85C0__btn1.x = 0;
    b_stru_5C85C0__btn1.y = 0;
    b_stru_5C85C0__btn1.w = yw->tiles[0]->chars[97].width;
    b_stru_5C85C0__btn1.h = yw->font_default_h;
}

//netgui update
void ypaworld_func64__sub7__sub5(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    dprintf("MAKE ME %s (multiplayer)\n", "ypaworld_func64__sub7__sub5");
}
