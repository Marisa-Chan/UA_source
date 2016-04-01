#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "def_parser.h"
#include "yw_internal.h"
#include "yw.h"
#include "input.h"

#include "font.h"

#include "yparobo.h"
#include "ypagun.h"

extern int bact_id;


////////////// Draw wireframe
char byte_5C8DB0[1024];

////////////// Draw
char byte_5FFF80[8192];


////////////////////////////////////////
listbase *dword_5BAFAC = NULL;
const char *dword_5BAF98;

////////////////////////////////////////

int dword_5BAF9C;




/////////////Asking//////////////////
listview lstvw2;

int dword_5BAFA0;
int dword_5BAFA4;
int dword_5BAFA8;



////////// exit menu/////////
listview exit_menu;

int dword_5C8B78;
int dword_5C8B7C;
int dword_5C8B80;
int dword_5C8B84;
int dword_5C8B88;



////////////////////////////////////////


squadMan squadron_manager;


/////////// INFO LOG //////////////

yw_infolog info_log;

char byte_5A7650[1024];




//////////// Map //////////////

int dword_516510;
int dword_516514;
int dword_516518;
int dword_51651C;

float flt_5164F4 = 0.0;
float flt_516504 = 0.0;
float flt_51650C = 0.0;

float flt_516524;
float flt_51652C;
float flt_516528;
float flt_516530;

tehMap robo_map;

char byte_5BA6E8[512];
char t1_cmdbuf_1[256];
char t1_cmdbuf_2[256];
char t1_cmdbuf_3[32768];
char *t1_cmdbufs[3] = {t1_cmdbuf_1, t1_cmdbuf_2, t1_cmdbuf_3};

button_str1 stru_5B2560;
button_str1 stru_5B2550;
button_str1 stru_5B2568;
button_str1 stru_5B25E0;
button_str1 stru_5B2580;
button_str1 stru_5B2598;
button_str1 stru_5B25C0;
button_str1 stru_5B2590;
button_str1 stru_5B2570;
button_str1 stru_5B2588;
button_str1 stru_5B2558;
button_str1 stru_5B2578;
button_str1 stru_5B25A8;
button_str1 stru_5B25D0;
button_str1 stru_5B25B0;
button_str1 stru_5B25A0;
button_str1 stru_5B25C8;
button_str1 stru_5B25D8;

typedef int (*mapFunc)(_NC_STACK_ypaworld *yw, int x, int y);

_NC_STACK_ypaworld *dword_5BAA60; // For sort func


///////// up panel ///////////
energPanel up_panel;

button_str1 stru_5C8BA0;
button_str1 stru_5C8D90;
button_str1 stru_5C8D98;
button_str1 stru_5C8DA0;
char byte_51805C[512];




///////// down panel /////////
listview gui_lstvw;

button_str1 analyzer_btn;
button_str1 menu_btn;
button_str1 create_btn;
button_str1 turrets_btn;
button_str1 next_squad_btn;
button_str1 to_host_btn;
button_str1 help_btn;
button_str1 map_btn;
button_str1 to_leader_btn;
button_str1 squad_btn;
button_str1 into_vhcl_btn;
char byte_516534[1088];

bzd bzda;

////////////////////////////////////////
////////////////////////////////////////


void sub_481204(_NC_STACK_ypaworld *yw, int a2, int a3)
{
    if ( a2 > yw->field_17c4 )
    {
        yw->field_17c8 = a3;
        yw->field_17c4 = a2;
    }
}

DWORD yw_GetColor(_NC_STACK_ypaworld *yw, int color_id)
{
    if ( dword_514EFC )
        return yw->iniColors[color_id].color;
    else
        return yw->iniColors[color_id].i;

}

void sb_0x4c87fc__sub0(_NC_STACK_ypaworld *yw, listbase *lstvw)
{
    if ( !(lstvw->cmd_flag & 0x20) )
    {
        Remove(lstvw);
        AddHead(&yw->field_17a0, lstvw);

        sub_412D9C(&lstvw->frm_1);
        sub_412D48(&lstvw->frm_1, 0);
    }
}

void sb_0x4c87fc(_NC_STACK_ypaworld *yw, const char *a2, listbase *lstvw)
{
    sub_4C31EC(yw, &lstvw2);

    sb_0x4c87fc__sub0(yw, &lstvw2);

    dword_5BAF98 = a2;
    dword_5BAFAC = lstvw;
    dword_5BAF9C = 3;
}

void sub_449DE8(_NC_STACK_ypaworld *yw, const char *a2)
{
    if ( !yw->field_757E )
    {
        const char *v4 = get_lang_string(yw->string_pointers_p2, 2486, "REALLY LAUNCH ONLINE HELP ?");

        sb_0x4c87fc(yw, v4, &exit_menu);

        dword_5C8B78 = 13;

        yw->field_81AF = NULL;
        yw->field_81B3 = a2;
    }
}


void create_squad_man(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    tile_xy *fnt0 = yw->tiles[0]->chars;
    tile_xy *fnt24 = yw->tiles[24]->chars;
    tile_xy *fnt28 = yw->tiles[28]->chars;

    int f0c32_w = fnt0[32].width;
    int v6 = f0c32_w + fnt0[123].width;

    int v9 = yw->field_1a38 + yw->font_default_h;
    int v10 = yw->field_1a38;

    int v8 = f0c32_w + v6 + 5 * fnt24[49].width;
    int v11 = 4 * fnt0[65].width + v8 + fnt28[97].width + f0c32_w + 8;

    const char *v14 = get_lang_string(yw->string_pointers_p2, 51, "FINDER");

    if ( lstvw_init(
                yw,
                &squadron_manager.lstvw,
                0x80000001,
                v14,
                0x80000002,
                1,
                0x80000003,
                0,
                0x80000004,
                12,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                24,
                0x80000012,
                3,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v11,
                0x8000000E,
                32000,
                0x8000000D,
                v11,
                0x8000000F,
                1,
                0x80000015,
                v10,
                0x80000016,
                v9,
                0x80000014,
                73,
                0) )
    {
        squadron_manager.field_2CC = fnt28[65].width;
        squadron_manager.field_2D0 = fnt28[64].width + fnt28[97].width + yw->font_default_w__b;
        squadron_manager.field_2D8 = v8;
        squadron_manager.field_2DC = v6;
        squadron_manager.field_2D4 = fnt28[64].width + squadron_manager.field_2CC + squadron_manager.field_2D0;
    }
}



void sub_4C3A54(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_1b7c )
    {
        if ( yw->field_1b7c == yw->field_1b78 || yw->field_1b70 )
        {
            bzda.field_1CC = 0x29;

            if ( bzda.field_8E0 )
            {
                bzda.field_1CC = 0x2B;

                if ( yw->field_2410 != -1 )
                    bzda.field_1CC = 0x2F;
            }

            if ( bzda.field_8E4 )
                bzda.field_1CC |= 0x10;

            bzda.field_1D4 = 1;
        }
        else
        {
            bzda.field_1D4 = 1;
            bzda.field_1CC = 41;
        }

        if ( yw->field_1b80->field_3D5 == 2 )
            bzda.field_1CC = 0;
    }
    else
    {
        bzda.field_1D4 = 0;
        bzda.field_1CC = 0;
    }

    bzda.field_1D0 &= bzda.field_1CC;
    bzda.field_1D8 &= bzda.field_1D4;

    if ( !bzda.field_1D0 )
        bzda.field_1D0 = bzda.field_1CC & 1;
}

void sb_0x451034__sub8(_NC_STACK_ypaworld *yw)
{
    memset(&up_panel, 0, sizeof(up_panel));

    tiles_stru *f30 = yw->tiles[30];

    up_panel.field_1CC = f30->font_height;
    up_panel.field_1DC = f30->chars[65].width;
    up_panel.cmd_flag = 0;
    up_panel.field_1D0 = up_panel.field_1DC + f30->chars[84].width;
    up_panel.frm_1.field_10 = 4;


    int v8 = yw->screen_width - 4 * (up_panel.field_1DC + f30->chars[84].width);

    up_panel.field_1D8 = v8 / 4;
    up_panel.field_1D4 = v8 / 8;
    up_panel.field_1E0 = 2;
    up_panel.field_1E4 = up_panel.field_1D0 - up_panel.field_1DC - 4;

    up_panel.frm_1.btn_xpos = 0;
    up_panel.frm_1.btn_ypos = 0;
    up_panel.frm_1.btn_width = yw->screen_width;
    up_panel.frm_1.btn_height = up_panel.field_1CC;

    up_panel.frm_1.field_18[0] = &stru_5C8D98;
    up_panel.frm_1.field_18[1] = &stru_5C8DA0;
    up_panel.frm_1.field_18[2] = &stru_5C8D90;
    up_panel.frm_1.field_18[3] = &stru_5C8BA0;

    for (int i = 0; i < 4; i++)
    {
        up_panel.frm_1.field_18[i]->ypos = 0;
        up_panel.frm_1.field_18[i]->xpos = up_panel.field_1D4 + (up_panel.field_1D0 + up_panel.field_1D8) * i;
        up_panel.frm_1.field_18[i]->width = up_panel.field_1D0;
        up_panel.frm_1.field_18[i]->fnt_height = up_panel.field_1CC;
    }

    up_panel.cmdstrm.cmdbuf = byte_51805C;

    sub_412D48(&up_panel.frm_1, 0);
}


void sub_4F681C(float a1, float a2, int *a3, int *a4)
{
    if ( robo_map.field_1E8 & 0x100 )
    {
        float a1a = a1 - robo_map.field_1D8;
        float a2a = a2 - robo_map.field_1DC;

        a1 = robo_map.field_1D8 + robo_map.field_264 * a1a + flt_5164F4 * a2a;
        a2 = robo_map.field_1DC + flt_516504 * a1a + flt_51650C * a2a;
    }

    *a3 = robo_map.field_200 + dround(a1 / robo_map.field_1E0) - robo_map.field_1F0;
    *a4 = robo_map.field_204 + dround(-a2 / robo_map.field_1E4) - robo_map.field_1F4;
}

void sub_4F68FC(NC_STACK_win3d *win3d, float a3, float a4, float a5, float a6, DWORD a7)
{
    w3d_func199arg a3a;
    sub_4F681C(a3, a4, &a3a.x1, &a3a.y1);
    sub_4F681C(a5, a6, &a3a.x2, &a3a.y2);

    rstr_arg217 v10;
    v10.dword0 = a7;
    v10.dword4 = a7;
    v10.dword8 = -1;

    call_method(win3d, 217, &v10);
    call_method(win3d, 201, &a3a);
}


char * sub_4F6980(char *cur, float a1, float a2, char a3, int a4, int a5)
{
    char *pcur = cur;

    int a3a, v30;
    sub_4F681C(a1, a2, &a3a, &v30);

    int v7 = a3a - a4 / 2 - robo_map.field_200;
    int v8 = v30 - a5 / 2 - robo_map.field_204;
    int v9 = v7 + a4;
    int v10 = v8 + a5;

    if ( v7 < robo_map.field_1F8 && v8 < robo_map.field_1FC && v9 > 0 && v10 > 0 )
    {
        int v11;

        if ( v7 >= 0 )
        {
            v11 = 0;
        }
        else
        {
            v11 = -v7;
            v7 = 0;
        }

        int v33;

        if ( v8 >= 0 )
        {
            v33 = 0;
        }
        else
        {
            v33 = -v8;
            v8 = 0;
        }

        int v34;

        if ( v9 <= robo_map.field_1F8 )
            v34 = 0;
        else
            v34 = robo_map.field_1F8 - v7;

        int v35;

        if ( v10 <= robo_map.field_1FC )
            v35 = 0;
        else
            v35 = robo_map.field_1FC - v8;

        fntcmd_set_center_xpos(&pcur, robo_map.field_200 + v7);
        fntcmd_set_center_ypos(&pcur, robo_map.field_204 + v8);

        if ( v33 )
        {
            fntcmd_set_yoff(&pcur, v33);
        }
        else if ( v35 )
        {
            fntcmd_set_yheight(&pcur, v35);
        }

        if ( v11 )
        {
            fntcmd_set_xoff(&pcur, v11);
        }
        else if ( v34 )
        {
            fntcmd_set_xwidth(&pcur, v34);
        }

        fntcmd_store_u8(&pcur, a3);
    }

    return pcur;
}

void sub_4F72E8(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact)
{
    if ( bact != yw->field_1b80 && bact->field_32 != bact->parent_bacto )
    {
        uint32_t clr;
        __NC_STACK_ypabact *bct;

        if ( bact->field_3DF == 2 )
        {
            clr = yw_GetColor(yw, 10);
            bct = bact->field_3f8;
        }
        else
        {
            if ( !bact->parent_bact )
                return;

            clr = yw_GetColor(yw, 11);
            bct = bact->parent_bact;
        }

        sub_4F68FC(yw->win3d, bact->field_621.sx, bact->field_621.sz, bct->field_621.sx, bct->field_621.sz, clr);
        return;
    }

    if ( bact->field_3DF == 2 )
    {
        __NC_STACK_ypabact *bct = bact->field_3f8;

        sub_4F68FC(yw->win3d, bact->field_621.sx, bact->field_621.sz, bct->field_621.sx, bct->field_621.sz,  yw_GetColor(yw, 10));
        return;
    }

    if ( bact->field_3DF == 1 )
    {
        sub_4F68FC(yw->win3d, bact->field_621.sx, bact->field_621.sz, bact->field_3fc.sx, bact->field_3fc.sz, yw_GetColor(yw, 10));
    }
    else
    {
        if ( bact->field_3DE == 1 )
        {
            sub_4F68FC(yw->win3d, bact->field_621.sx, bact->field_621.sz, bact->field_3ec.sx, bact->field_3ec.sz, yw_GetColor(yw, 9));
        }
        else if ( bact->field_3DE == 2 )
        {
            sub_4F68FC(yw->win3d, bact->field_621.sx, bact->field_621.sz, bact->field_3e8->field_621.sx, bact->field_3e8->field_621.sz, yw_GetColor(yw, 9));
        }

        if ( bact->field_3D6 & 0x4000000 )
        {
            if ( bact->field_3D6 & 0x8000000 )
            {
                uint32_t clr = yw_GetColor(yw, 9);

                for (int i = 1; i < bact->field_59A; i++)
                    sub_4F68FC(yw->win3d, bact->field_418[i].sx, bact->field_418[i].sy, bact->field_418[i - 1].sx, bact->field_418[i - 1].sy, clr);

                sub_4F68FC(yw->win3d, bact->field_418[bact->field_59A - 1].sx, bact->field_418[bact->field_59A - 1].sy, bact->field_418[0].sx, bact->field_418[0].sy, clr);
            }
            else if ( bact->field_59A > 0 )
            {
                uint32_t clr = yw_GetColor(yw, 9);

                for (int i = bact->field_598; i < bact->field_59A - 1; i++)
                    sub_4F68FC(yw->win3d, bact->field_418[i].sx, bact->field_418[i].sy, bact->field_418[i + 1].sx, bact->field_418[i + 1].sy, clr);
            }
        }
    }
}

void  sb_0x4f8f64__sub1__sub0(_NC_STACK_ypaworld *yw)
{
    bact_node *v2 = (bact_node *)yw->bact_list.head;

    while( v2->next )
    {
        if ( v2->bact->field_24 == 3 && v2->bact != yw->field_1b80 )
        {
            NC_STACK_yparobo * obrob = (NC_STACK_yparobo *)v2->bacto;

            //call_vtbl(v2->bacto, 3, 0x80000001, &a4, 0);

            __NC_STACK_yparobo *robo = &obrob->stack__yparobo;

            for (int i = 0; i < 8; i++)
            {
                int v10 = 0;

                switch ( i )
                {
                case 0:
                    if ( robo->field_221 > 0 )
                        v10 = robo->field_229;
                    break;

                case 1:
                    if ( robo->field_239 > 0 )
                        v10 = robo->field_241;
                    break;

                case 2:
                    if ( robo->field_251 > 0 )
                        v10 = robo->field_259;
                    break;

                case 3:
                    if ( robo->field_269 > 0 )
                        v10 = robo->field_271;
                    break;

                case 4:
                    if ( robo->field_285 > 0 )
                        v10 = robo->field_28D;
                    break;

                case 5:
                    if ( robo->field_29D > 0 )
                        v10 = robo->field_2A5;
                    break;

                case 6:
                    if ( robo->field_2B5 > 0 )
                        v10 = robo->field_2BD;
                    break;

                case 7:
                    if ( robo->field_2CD > 0 )
                        v10 = robo->field_2D5;
                    break;

                default:
                    break;
                }

                if ( v10 )
                {
                    float a5 = (v10 % yw->sectors_maxX2) * 1200.0 + 600.0;
                    float a6 = -((v10 / yw->sectors_maxX2) * 1200.0 + 600.0);

                    sub_4F68FC(yw->win3d, v2->bact->field_621.sx, v2->bact->field_621.sz, a5, a6, yw_GetColor(yw, i));
                }
            }


            if ( robo->field_2FD )
            {
                float v26 = -((robo->field_2F5 / yw->sectors_maxX2) * 1200.0 + 600.0);
                float v28 = (robo->field_2F5 % yw->sectors_maxX2) * 1200.0 + 600.0;

                if ( (yw->field_1614 / 300) & 1 )
                    sub_4F68FC(yw->win3d, v2->bact->field_621.sx, v2->bact->field_621.sz, v28, v26, yw_GetColor(yw, 0));
            }

            if ( robo->field_2ED )
            {
                float v22 = (robo->field_2E5 % yw->sectors_maxX2) * 1200.0 + 600.0;
                float v27 = -(robo->field_2E5 / yw->sectors_maxX2 * 1200.0 + 600.0);

                if ( (yw->field_1614 / 300) & 1 )
                {
                    sub_4F68FC(yw->win3d, v2->bact->field_621.sx, v2->bact->field_621.sz, v22, v27, yw_GetColor(yw, 7));
                }
            }
        }
        v2 = (bact_node *)v2->next;
    }
}

void sb_0x4f8f64__sub1(_NC_STACK_ypaworld *yw)
{
    if ( robo_map.field_1E8 & 0x100 )
    {
        if ( yw->field_1b80 != yw->field_1b84 )
        {
            sub_4F68FC(yw->win3d, yw->field_1b84->field_621.sx, yw->field_1b84->field_621.sz, yw->field_1b84->parent_bact->field_621.sx, yw->field_1b84->parent_bact->field_621.sz, yw_GetColor(yw, 11));

            if ( yw->field_1b84->parent_bacto == yw->field_1b84->field_32 )
            {
                int v7 = 0;
                float a5, a6;

                if ( yw->field_1b84->field_3DE == 1 )
                {
                    a5 = yw->field_1b84->field_3ec.sx;
                    a6 = yw->field_1b84->field_3ec.sz;
                    v7 = 1;
                }
                else if ( yw->field_1b84->field_3DE == 2 )
                {
                    a5 = yw->field_1b84->field_3e8->field_621.sx;
                    a6 = yw->field_1b84->field_3e8->field_621.sz;
                    v7 = 1;
                }

                if ( v7 )
                    sub_4F68FC(yw->win3d, yw->field_1b84->field_621.sx, yw->field_1b84->field_621.sz, a5, a6, yw_GetColor(yw, 9));
            }
        }
    }
    else
    {
        if ( robo_map.field_1EC & 4 )
        {
            for (int i = 0; i < yw->field_2414; i++)
            {
                __NC_STACK_ypabact *tmp = yw->field_1c0c[i];

                if (tmp->field_3D5 != 2)
                    sub_4F72E8(yw, tmp);

                bact_node *bct = (bact_node *)tmp->list2.head;
                while(bct->next)
                {
                    if ( bct->bact->field_3D5 != 2 )
                        sub_4F72E8(yw, bct->bact);

                    bct = (bact_node *)bct->next;
                }
            }
        }
        else
        {
            if ( yw->field_2410 != -1 )
            {
                __NC_STACK_ypabact *tmp = yw->field_1c0c[yw->field_2410];

                if ( tmp->field_3D5 != 2 )
                    sub_4F72E8(yw, tmp);
            }
        }

        if ( yw->field_1b84 )
        {
            sub_4F68FC(yw->win3d, yw->field_1b84->field_621.sx, 0.0, yw->field_1b84->field_621.sx, -yw->map_Height_meters, yw_GetColor(yw, 13));
            sub_4F68FC(yw->win3d, 0.0, yw->field_1b84->field_621.sz, yw->map_Width_meters, yw->field_1b84->field_621.sz, yw_GetColor(yw, 13));
        }

        if ( robo_map.field_1E8 & 0x200 )
        {

            w3d_func199arg v23;
            w3d_func199arg v24;
            w3d_func199arg v25;
            w3d_func199arg v26;
            rstr_arg217 v27;

            int v21 = yw->screen_width / 2;
            int v22 = yw->screen_height / 2;

            v24.x1 = dword_516510 - v21;
            v24.x2 = dword_516518 - v21;
            v24.y1 = dword_516514 - v22;
            v24.y2 = dword_516514 - v22;

            v23.x1 = dword_516518 - v21;
            v23.x2 = dword_516510 - v21;
            v23.y1 = dword_51651C - v22;
            v23.y2 = dword_51651C - v22;

            v25.x1 = dword_516510 - v21;
            v25.x2 = dword_516510 - v21;
            v25.y1 = dword_51651C - v22;
            v25.y2 = dword_516514 - v22;

            v26.x1 = dword_516518 - v21;
            v26.x2 = dword_516518 - v21;
            v26.y1 = dword_516514 - v22;
            v26.y2 = dword_51651C - v22;



            v27.dword0 = yw_GetColor(yw, 12);
            v27.dword4 = yw_GetColor(yw, 12);
            v27.dword8 = -1;

            call_method(yw->win3d, 217, &v27);
            call_method(yw->win3d, 201, &v24);
            call_method(yw->win3d, 201, &v26);
            call_method(yw->win3d, 201, &v23);
            call_method(yw->win3d, 201, &v25);
        }
    }

    if ( yw->field_1b68 )
        sb_0x4f8f64__sub1__sub0(yw);
}


char * sb_0x4f8f64__sub2__sub0(char *cur, float a1, float a2, char a3, int a4, int a5)
{
    char *pcur = cur;

    int a3a, v27;
    sub_4F681C(a1, a2, &a3a, &v27);

    int v8 = a3a - a4 / 2 - robo_map.field_200;
    int v9 = v27 - a4 / 2 - robo_map.field_204;
    int v10 = v8 + a4;
    int v11 = v9 + a5;

    if ( v8 < robo_map.field_1F8 && v9 < robo_map.field_1FC && v10 > 0 && v11 > 0 )
    {
        int v31;
        if ( v8 >= 0 )
        {
            v31 = 0;
        }
        else
        {
            v31 = -v8;
            v8 = 0;
        }

        int v12;
        if ( v9 >= 0 )
        {
            v12 = 0;
        }
        else
        {
            v12 = -v9;
            v9 = 0;
        }

        int v30;
        if ( v10 <= robo_map.field_1F8 )
            v30 = 0;
        else
            v30 = robo_map.field_1F8 - v8;

        int v32;
        if ( v11 <= robo_map.field_1FC )
            v32 = a5;
        else
            v32 = robo_map.field_1FC - v9;

        fntcmd_set_center_xpos(&pcur, robo_map.field_200 + v8);
        fntcmd_set_center_ypos(&pcur, robo_map.field_204 + v9);


        if ( v12 )
            fntcmd_set_yoff(&pcur, v12);

        if ( v32 )
            fntcmd_set_yheight(&pcur, v32);

        if ( v31 )
        {
            fntcmd_set_xoff(&pcur, v31);
        }
        else if ( v30 )
        {
            fntcmd_set_xwidth(&pcur, v30);
        }

        fntcmd_store_u8(&pcur, a3);
    }
    return pcur;
}


char * sb_0x4f8f64__sub2(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int a4, v4;

    switch ( robo_map.field_1EE )
    {
    case 0:
        v4 = 16;
        a4 = 4;
        break;

    case 1:
        v4 = 17;
        a4 = 8;
        break;

    case 2:
        v4 = 18;
        a4 = 16;
        break;

    case 3:
        v4 = 19;
        a4 = 32;
        break;

    case 4:
        v4 = 20;
        a4 = 64;
        break;

    default:
        break;
    }

    if ( a4 >= 8 )
    {
        if ( robo_map.field_1EC & 1 )
        {
            fntcmd_select_tileset(&pcur, v4);

            for (int i = 0; i < yw->field_38; i++)
            {
                yw_field34 *v7 = &yw->field_34[i];

                if ( v7->p_cell )
                {
                    if ( (1 << yw->field_1b80->owner) & v7->p_cell->field_39 )
                    {
                        if ( v7->power_2 > 0 )
                        {
                            int v9 = 0;
                            float a1 = v7->x * 1200.0 + 600.0;
                            float a2 = -(v7->y * 1200.0 + 600.0);

                            if ( v7->power_2 <= 32)
                                v9 = 0x80;
                            else if ( v7->power_2 <= 64)
                                v9 = 0x81;
                            else if ( v7->power_2 <= 96)
                                v9 = 0x82;
                            else if ( v7->power_2 <= 128)
                                v9 = 0x83;
                            else if ( v7->power_2 <= 160)
                                v9 = 0x84;
                            else if ( v7->power_2 <= 192)
                                v9 = 0x85;
                            else if ( v7->power_2 <= 224)
                                v9 = 0x86;
                            else if ( v7->power_2 <= 256)
                                v9 = 0x87;

                            pcur = sub_4F6980(pcur, a1, a2, 0x89, a4, a4);
                            pcur = sb_0x4f8f64__sub2__sub0(pcur, a1, a2, v9, a4, a4 / 8);
                        }
                    }
                }
            }

            for (int i = 0; i < 8; i++)
            {
                gemProto *v11 = &yw->gems[i];

                cellArea *v12 = &yw->cells[v11->sec_y * yw->sectors_maxX2 + v11->sec_x];
                if ( v11->field_0 )
                {
                    if ( v12->field_39 & (1 << yw->field_1b80->owner) )
                    {
                        int v13 = 1;

                        if ( v12->field_3A == 4 )
                        {
                            if ( yw->field_1614 / 300 & 1 )
                                v13 = 0;
                        }

                        if ( v13 )
                        {
                            float v14 = v11->sec_x * 1200.0 + 600.0;
                            float v15 = -(v11->sec_y * 1200.0 + 600.0);
                            pcur = sub_4F6980(pcur, v14, v15, 0x88, a4, a4);
                        }
                    }
                }
            }

            for (int i = 0; i < yw->field_2d90->gate_count; i++ )
            {
                gateProto *v16 = &yw->field_2d90->gates[i];

                if ( v16->pcell->field_3A == 5 )
                {
                    if ( (1 << yw->field_1b80->owner) & v16->pcell->field_39 )
                    {
                        float v19 = v16->sec_x * 1200.0 + 600.0;
                        float v20 = -(v16->sec_y * 1200.0 + 600.0);
                        pcur = sub_4F6980(pcur, v19, v20, 0x93, a4, a4);
                    }

                    for (int j = 0; j < v16->keySectors_count; j++)
                    {
                        cellArea *v22 = v16->keySectors[j].cell;
                        int v52 = 0;

                        if ( v22 )
                        {
                            if ( (1 << yw->field_1b80->owner) & v22->field_39 )
                            {
                                if ( v22->owner == yw->field_1b80->owner || yw->field_1614 / 300 & 1 )
                                    v52 = 1;
                            }
                        }

                        if ( v52 )
                        {
                            float v23 = v16->keySectors[j].x * 1200.0 + 600.0;
                            float v24 = -(v16->keySectors[j].y * 1200.0 + 600.0);
                            pcur = sub_4F6980(pcur, v23, v24, 0x8A, a4, a4);
                        }
                    }
                }
                else if ( v16->pcell->field_3A == 6 )
                {
                    if ( (1 << yw->field_1b80->owner) & v16->pcell->field_39 )
                    {
                        if ( yw->field_1614 / 300 & 1 )
                        {
                            float v25 = v16->sec_x * 1200.0 + 600.0;
                            float v26 = -(v16->sec_y * 1200.0 + 600.0);
                            pcur = sub_4F6980(pcur, v25, v26, -108, a4, a4);
                        }
                    }
                }
            }

            for (int i = 0; i < yw->field_2d90->supetItems_count; i++)
            {
                supetItemProto *v28 = &yw->field_2d90->supetItems[i];

                int v29 = 0;
                int v61 = 0;

                if ( v28->type == 1 )
                {
                    v29 = 0x8E;

                    if ( v28->field_4 == 0 )
                        v61 = 0x8B;
                    else if ( v28->field_4 <= 2 )
                        v61 = 0x8C;
                    else if ( v28->field_4 == 3 )
                        v61 = 0x8D;
                }
                else if ( v28->type == 2 )
                {
                    v29 = 0x92;

                    if ( v28->field_4 == 0 )
                        v61 = 0x8F;
                    else if ( v28->field_4 <= 2 )
                        v61 = 0x90;
                    else if ( v28->field_4 == 3 )
                        v61 = 0x91;
                }

                if ( v61 )
                {
                    int v34 = 0;

                    if ( v28->pcell->field_39 & (1 << yw->field_1b80->owner) )
                    {
                        if ( v28->pcell->owner == yw->field_1b80->owner || yw->field_1614 / 300 & 1 )
                            v34 = 1;
                    }

                    if ( v34 )
                    {
                        float v35 = v28->sec_x * 1200.0 + 600.0;
                        float v36 = -(v28->sec_y * 1200.0 + 600.0);
                        pcur = sub_4F6980(pcur, v35, v36, v61, a4, a4);
                    }

                    if ( v29 )
                    {
                        for (int j = 0; j < v28->keySectors_count; j++)
                        {
                            cellArea *v38 = v28->keySectors[j].cell;

                            int v60 = 0;

                            if ( v38 )
                            {
                                if ( (1 << yw->field_1b80->owner) & v38->field_39 )
                                {
                                    if ( v38->owner == yw->field_1b80->owner || yw->field_1614 / 500 & 1 )
                                        v60 = 1;
                                }
                            }

                            if ( v60 )
                            {
                                float v39 = v28->keySectors[j].x * 1200.0 + 600.0;
                                float v40 = -(v28->keySectors[j].y * 1200.0 + 600.0);

                                pcur = sub_4F6980(pcur, v39, v40, v61, a4, a4);
                            }
                        }
                    }
                }
            }
        }
    }

    return pcur;
}

char * sub_4F6DFC(_NC_STACK_ypaworld *yw, char *cur, int height, int width, __NC_STACK_ypabact *bact, int a6)
{
    char *pcur = cur;

    if ( bact->field_3D5 != 2 && bact->owner != 5 )
    {
        int v8;

        if ( a6 )
        {
            if ( bact->field_24 == 4 )
                v8 = bact->owner + 1;
            else
                v8 = yw->VhclProtos[bact->id].type_icon;
        }
        else
        {
            switch ( bact->field_24 )
            {
            case 1:
                v8 = bact->owner + 9;
                break;

            case 3:
                switch ( robo_map.field_1EE )
                {
                case 0:
                case 1:
                    v8 = bact->owner + 0x30;
                    break;

                case 2:
                    v8 = bact->owner + 0x28;
                    break;

                case 3:
                    v8 = bact->owner + 0x20;
                    break;

                case 4:
                    v8 = bact->owner + 0x18;
                    break;
                }
                break;

            case 4:
                v8 = bact->owner + 1;
                break;

            case 2:
            case 8:
                v8 = bact->owner + 0x19;
                break;

            case 6:
                v8 = bact->owner + 0x11;
                break;

            case 7:
                v8 = bact->owner + 0x21;
                break;

            case 9:
                v8 = bact->owner + 0x29;
                break;

            default:
                v8 = 0x41;
                break;
            }
        }

        pcur = sub_4F6980(pcur, bact->field_621.sx, bact->field_621.sz, v8, height, width);

        if ( bact->field_24 != 4 && robo_map.field_1EE > 2 )
        {
            DWORD clr = yw_GetColor(yw, bact->owner);

            int v31 = 0;
            float v33, v35;

            float v21;
            float v22;
            float v23;
            float v24;
            float v25;

            if ( bact == yw->field_1b80 )
            {
                v33 = bact->field_5C9.m02;
                v35 = bact->field_5C9.m22;

                if ( bact == yw->field_1b84 )
                {
                    v21 = -380.0;
                    v22 = 380.0;

                    v23 = -300.0;
                    v24 = 600.0;
                    v25 = -300.0;

                    v31 = 1;

                    clr = yw_GetColor(yw, 13);
                }
                else
                {
                    v21 = -85.0;
                    v22 = 85.0;

                    v23 = 0.0;
                    v24 = 700.0;
                    v25 = 0.0;
                }
            }
            else if ( bact == yw->field_1b84 )
            {
                v33 = yw->field_1340.m02;
                v35 = yw->field_1340.m22;

                v21 = -190.0;
                v22 = 190.0;

                v23 = -200.0;
                v24 = 400.0;
                v25 = -200.0;

                v31 = 1;

                clr = yw_GetColor(yw, 13);
            }
            else
            {
                v33 = bact->field_651.m02;
                v35 = bact->field_651.m22;

                v21 = -40.0;
                v22 = 40.0;

                v23 = 0.0;
                v24 = 350.0;
                v25 = 0.0;
            }

            float v30 = sqrt(POW2(v33) + POW2(v35));
            float v34, v36;

            if ( v30 <= 0.0 )
            {
                v36 = 1.0;
                v34 = 0.0;
            }
            else
            {
                float v12 = 1.0 / v30;
                v34 = v33 * v12;
                v36 = v35 * v12;
            }

            float v14 = -v34;

            float a3 = v36 * v21 + v14 * v23 + bact->field_621.sx;
            float v28 = v34 * v21 + v36 * v23 + bact->field_621.sz;
            float v16 = v36 * 0.0 + v14 * v24 + bact->field_621.sx;
            float v15 = v34 * 0.0 + v36 * v24 + bact->field_621.sz;

            sub_4F68FC(yw->win3d, a3, v28, v16, v15, clr);

            float v27 = v36 * v22 + v14 * v25 + bact->field_621.sx;
            float v29 = v34 * v22 + v36 * v25 + bact->field_621.sz;

            sub_4F68FC(yw->win3d, v27, v29, v16, v15, clr);

            if ( v31 )
                sub_4F68FC(yw->win3d, a3, v28, v27, v29, clr); // Also draw base of view triangle
        }

        if ( bact->field_24 != 4 && bact->field_24 != 3 && bact->owner == yw->field_1b80->owner )
        {
            if ( robo_map.field_1EC & 4 && robo_map.field_1EE > 2 )
            {
                float v32 = (float)bact->energy / (float)bact->energy_2;
                int v19;

                if ( v32 > 0.25 )
                {
                    if ( v32 > 0.5 )
                    {
                        if ( v32 > 0.75 )
                            v19 = 0x83;
                        else
                            v19 = 0x82;
                    }
                    else
                    {
                        v19 = 0x81;
                    }
                }
                else
                {
                    v19 = 0x80;
                }

                pcur = sub_4F6980(pcur, bact->field_621.sx, bact->field_621.sz, v19, height, width);
            }
        }
    }
    return pcur;
}

__NC_STACK_ypabact * sub_4D3C3C(__NC_STACK_ypabact *bact)
{
    __NC_STACK_ypabact *a4 = bact;

    if ( !bact )
        return NULL;

    size_t v1 = (size_t)bact->parent_bacto; // DIRT!!!

    if (v1 <= 2)
        return NULL;

    if ( bact->parent_bacto != bact->field_32 )
        call_vtbl(bact->parent_bacto, 3, 0x80001003, &a4, 0);

    return a4;
}

char * sub_4F7BE8(_NC_STACK_ypaworld *yw, char *cur, __NC_STACK_ypabact *bact, int a2, int a4, int a5, int a6)
{
    char *pcur = cur;

    if ( bact )
    {
        fntcmd_select_tileset(&pcur, a2);

        pcur = sub_4F6980(pcur, bact->field_621.sx, bact->field_621.sz, a4, a5, a6);

        bact_node *nd = (bact_node *)bact->list2.head;

        while(nd->next)
        {
            if ( nd->bact->field_3D5 != 2 )
                pcur = sub_4F6980(pcur, nd->bact->field_621.sx, nd->bact->field_621.sz, a4, a5, a6);

            nd = (bact_node *)nd->next;
        }
    }
    return pcur;
}

int sb_0x4f8f64__sub3__sub0(_NC_STACK_ypaworld *yw, cellArea *cell)
{
    int enrg = 0;

    __NC_STACK_ypabact *v2 = (__NC_STACK_ypabact *)cell->field_3C.head;

    while(v2->next)
    {
        if ( v2->field_3D5 != 2 && v2->field_3D5 != 5 )
        {
            if ( v2->field_24 != 3 && v2->field_24 != 4 && v2->field_24 != 9 )
                enrg += (v2->energy_2 + 99) / 100;
        }

        v2 = (__NC_STACK_ypabact *)v2->next;
    }

    return enrg;
}

// Get string width in pixel
int sub_4513E0(const char *strr, tiles_stru *charset)
{
    int i = 0;

    int wdth = 0;

    while (strr[i])
    {
        wdth += charset->chars[ (uint8_t)strr[i] ].width;
        i++;
    }

    return wdth;
}

char * sb_0x4f8f64__sub3__sub1(_NC_STACK_ypaworld *yw, const char *labl, int tileset_id, char *cur, int a4, int a5)
{
    int v8 = sub_4513E0(labl, yw->tiles[tileset_id]);

    float a2a = -(a5 * 1200.0);
    float a1a = a4 * 1200.0;

    int a4a;
    int a3a;
    sub_4F681C(a1a, a2a, &a3a, &a4a);

    int v9 = a3a - robo_map.field_200;
    int v10 = a4a - robo_map.field_204;

    char *pcur = cur;

    if ( a3a - robo_map.field_200 > 0 && v10 > 0 && a3a - robo_map.field_200 + v8 < robo_map.field_1F8 && a4a - robo_map.field_204 + yw->tiles[tileset_id]->font_height < robo_map.field_1FC )
    {
        fntcmd_select_tileset(&pcur, tileset_id);
        fntcmd_set_center_xpos(&pcur, robo_map.field_200 + v9);
        fntcmd_set_center_ypos(&pcur, robo_map.field_204 + v10);

        const char *vvv = labl;
        while (*vvv)
        {
            fntcmd_store_s8(&pcur, *vvv);
            vvv++;
        }

        fntcmd_store_s8(&pcur, *vvv);
    }

    return pcur;
}

char * sb_0x4f8f64__sub3(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int v111;
    int a5;
    int a4;
    int v114;
    int v110;
    int v7;
    int v99;

    switch ( robo_map.field_1EE )
    {
    case 0:
        v111 = 4;
        v114 = 59;
        v110 = 16;
        a5 = yw->tiles[59]->font_height;
        a4 = yw->tiles[59]->chars[1].width;
        v7 = 23;
        v99 = 19;
        break;

    case 1:
        v114 = 59;
        v111 = 8;
        v110 = 17;
        a5 = yw->tiles[59]->font_height;
        a4 = yw->tiles[59]->chars[1].width;
        v7 = 23;
        v99 = 19;
        break;

    case 2:
        v111 = 16;
        v114 = 60;
        v110 = 18;
        a5 = yw->tiles[60]->font_height;
        a4 = yw->tiles[60]->chars[1].width;
        v7 = 22;
        v99 = 18;
        break;

    case 3:
        v110 = 19;
        v111 = 32;
        v114 = 61;
        a5 = yw->tiles[61]->font_height;
        a4 = yw->tiles[61]->chars[1].width;
        v7 = 21;
        v99 = 17;
        break;

    case 4:
        v110 = 20;
        v111 = 64;
        v114 = 28;
        a5 = yw->tiles[28]->font_height;
        a4 = yw->tiles[28]->chars[1].width;
        v7 = 20;
        v99 = 16;
        break;
    }


    switch ( yw->field_1a5c )
    {
    case 2:
        if ( !(robo_map.field_1E8 & 0x200) )
        {
            if ( yw->field_1a58 & 0x20 )
            {
                fntcmd_select_tileset(&pcur, v114);

                pcur = sub_4F6980(pcur, yw->field_1a98->field_621.sx, yw->field_1a98->field_621.sz, 0x86, a4, a5);
            }
            else if ( yw->field_1a58 & 0x10 )
            {
                if ( yw->field_1a60->owner == yw->field_1b80->owner && ( (1 << yw->field_1b80->owner) & yw->field_1a60->field_39 ) )
                {
                    fntcmd_select_tileset(&pcur, v114);

                    pcur = sub_4F6980(pcur, yw->field_1a6c.sx, yw->field_1a6c.sz, 0x88, a4, a5);
                }
                else
                {
                    fntcmd_select_tileset(&pcur, v110);

                    pcur = sub_4F6980(pcur, yw->field_1a7c.sx, yw->field_1a7c.sz, 66, v111, v111);
                }
            }
        }
        break;

    case 5:
        fntcmd_select_tileset(&pcur, v114);

        pcur = sub_4F6980(pcur, yw->field_1a98->field_621.sx, yw->field_1a98->field_621.sz, 0x85, a4, a5);
        break;

    case 6:
        fntcmd_select_tileset(&pcur, v110);

        pcur = sub_4F6980(pcur, yw->field_1a7c.sx, yw->field_1a7c.sz, 65, v111, v111);
        break;

    case 8:
        if ( !(robo_map.field_1E8 & 0x200) )
        {
            if ( yw->field_1a98 )
            {
                if ( yw->field_1a98 == yw->field_1b80 )
                {
                    int v22 = yw->tiles[1]->font_height;

                    fntcmd_select_tileset(&pcur, 1);

                    pcur = sub_4F6980(pcur, yw->field_1a98->field_621.sx, yw->field_1a98->field_621.sz, v7, v22, v22);
                }
                else
                {
                    __NC_STACK_ypabact *v25 = sub_4D3C3C(yw->field_1a98);

                    if ( v25 )
                    {
                        pcur = sub_4F7BE8(yw, pcur, v25, v114, 0x85, a4, a5);
                    }

                }
            }
        }
        break;

    case 10:
        fntcmd_select_tileset(&pcur, v114);

        pcur = sub_4F6980(pcur, yw->field_1a6c.sx, yw->field_1a6c.sz, 0x88, a4, a5);
        break;

    default:
        break;
    }

    if ( bzda.field_1D0 & 0x20 )
    {
        fntcmd_select_tileset(&pcur, 1);

        int v44 = yw->tiles[1]->font_height;

        pcur = sub_4F6980(pcur, yw->field_1b80->field_621.sx, yw->field_1b80->field_621.sz, v99, v44, v44);
    }
    else
    {
        if ( yw->field_2410 != -1 )
        {
            __NC_STACK_ypabact *v47 = yw->field_1c0c[yw->field_2410];

            if ( v47 )
            {
                pcur = sub_4F7BE8(yw, pcur, v47, v114, 0x84, a4, a5);

                if ( v47->field_3DE == 1 )
                {
                    if ( v47->field_3e8->id != yw->field_1b80->owner )
                    {
                        int v101 = (v47->field_3ec.sx / 1200) * 1200.0 + 600.0;
                        int v100 = -(( -v47->field_3ec.sz / 1200) * 1200.0 + 600.0);

                        fntcmd_select_tileset(&pcur, v110);

                        pcur = sub_4F6980(pcur, v101, v100, 66, v111, v111);

                        fntcmd_select_tileset(&pcur, v114);

                        pcur = sub_4F6980(pcur, v47->field_3ec.sx, v47->field_3ec.sz, 0x88, a4, a5);
                    }
                    else
                    {
                        fntcmd_select_tileset(&pcur, v114);

                        pcur = sub_4F6980(pcur, v47->field_3ec.sx, v47->field_3ec.sz, 0x88, a4, a5);
                    }
                }
                else if ( v47->field_3DE == 2 )
                {
                    fntcmd_select_tileset(&pcur, v114);

                    pcur = sub_4F6980(pcur, v47->field_3e8->field_621.sx, v47->field_3e8->field_621.sz, 0x86, a4, a5);
                }

                if ( v47->field_3D6 & 0x4000000 )
                {
                    if ( v47->field_59A > 0 )
                    {
                        int v68;

                        if ( v47->field_3D6 & 0x8000000 )
                            v68 = 0;
                        else
                            v68 = v47->field_598;

                        for (int i = v68; i < v47->field_59A; i++)
                        {
                            fntcmd_select_tileset(&pcur, v114);

                            pcur = sub_4F6980(pcur, v47->field_418[i].sx, v47->field_418[i].sz, 0x88, a4, a5);
                        }
                    }
                }
            }
        }
    }

    if ( yw->field_1b84 )
    {
        if ( yw->field_1614 / 300 & 1 )
        {
            fntcmd_select_tileset(&pcur, v114);

            pcur = sub_4F6980(pcur, yw->field_1b84->field_621.sx, yw->field_1b84->field_621.sz, 0x89, a4, a5);
        }
    }

    if ( yw->field_2420 )
    {
        if ( yw->field_1614 / 300 & 1 )
        {
            fntcmd_select_tileset(&pcur, v114);

            pcur = sub_4F6980(pcur, yw->field_2420->field_621.sx, yw->field_2420->field_621.sz, 0x8A, a4, a5);
        }
    }

    for ( int i = 0; i < yw->field_2d90->gate_count ; i++ )
    {
        gateProto *v80 = &yw->field_2d90->gates[i];

        if ( v80->pcell->field_3A == 6 )
        {
            int v81 = sb_0x4f8f64__sub3__sub0(yw, v80->pcell);

            int v85 = 1;

            if ( 2 * yw->beamenergy < 2 * v81 )
            {
                v85 = (yw->field_1614 / 300) & 1; // FLASHING GATES TITLE
            }

            if ( v85 )
            {
                char a1a[32];

                sprintf(a1a, "%d / %d", 2 * v81, 2 * yw->beamenergy);

                pcur = sb_0x4f8f64__sub3__sub1(yw, a1a, 0x1F, pcur, v80->sec_x, v80->sec_y);
            }
        }
    }
    return pcur;
}

char * sb_0x4f8f64__sub0(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    fntcmd_select_tileset(&pcur, 10);
    fntcmd_set_center_xpos(&pcur, robo_map.frm_1.btn_xpos + robo_map.field_234 + robo_map.field_244 - (yw->screen_width / 2));
    fntcmd_set_center_ypos(&pcur, robo_map.frm_1.btn_ypos + robo_map.frm_1.btn_height - (robo_map.field_230 + robo_map.field_240) - (yw->screen_height / 2));

    if ( robo_map.field_1EC & 1 )
        fntcmd_store_u8(&pcur, 97);
    else
        fntcmd_store_u8(&pcur, 65);

    if ( robo_map.field_1EC & 2 )
        fntcmd_store_u8(&pcur, 98);
    else
        fntcmd_store_u8(&pcur, 66);

    if ( robo_map.field_1EC & 4 )
        fntcmd_store_u8(&pcur, 100);
    else
        fntcmd_store_u8(&pcur, 68);

    if ( robo_map.field_1ED == 1 )
        fntcmd_store_u8(&pcur, 102);
    else
        fntcmd_store_u8(&pcur, 70);

    if ( robo_map.field_1E8 & 0x40 )
        fntcmd_store_u8(&pcur, 106);
    else
        fntcmd_store_u8(&pcur, 74);

    if ( robo_map.field_1E8 & 0x80 )
        fntcmd_store_u8(&pcur, 107);
    else
        fntcmd_store_u8(&pcur, 75);

    return pcur;
}

void sb_0x4f8f64(_NC_STACK_ypaworld *yw)
{
    w3d_a209 arg;
    arg.cmdbuf = t1_cmdbuf_3;
    arg.includ = NULL;

    int v41 = 0;

    int v39 = robo_map.field_1EE + 2;

    int a6 = 0;
    int setid = 59;
    switch ( robo_map.field_1EE )
    {
    case 0:
    case 1:
        setid = 59;
        break;

    case 2:
        setid = 60;
        break;

    case 3:
        setid = 61;
        break;

    case 4:
        a6 = 1;
        setid = 28;
        break;
    }

    int height = yw->tiles[setid]->chars[128].width;
    int width = yw->tiles[setid]->font_height;

    robo_map.field_1F8 = robo_map.frm_1.btn_width - robo_map.field_24C;
    robo_map.field_1FC = robo_map.frm_1.btn_height - robo_map.field_250;

    robo_map.field_200 = robo_map.frm_1.btn_xpos + robo_map.field_244 - (yw->screen_width / 2);
    robo_map.field_204 = robo_map.frm_1.btn_ypos + robo_map.field_23C - (yw->screen_height / 2);

    ua_dRect rect;
    rect.x1 = robo_map.field_200;
    rect.x2 = robo_map.frm_1.btn_width - robo_map.field_24C + robo_map.field_200 - 1;
    rect.y1 = robo_map.frm_1.btn_ypos + robo_map.field_23C - (yw->screen_height / 2);
    rect.y2 = robo_map.frm_1.btn_height - robo_map.field_250 + rect.y1 - 1;

    robo_map.field_1F0 = robo_map.field_1D8 / robo_map.field_1E0 - (robo_map.frm_1.btn_width - robo_map.field_24C) / 2;
    robo_map.field_1F4 = -(robo_map.field_1DC / robo_map.field_1E4 + (robo_map.frm_1.btn_height - robo_map.field_250) / 2);

    call_method(yw->win3d, 211, &rect);

    int v33 = robo_map.field_1F0 >> v39;
    int v38 = robo_map.field_1F4 >> v39;
    int v43 = (robo_map.field_1F8 + robo_map.field_1F0) >> v39;
    int vii = (robo_map.field_1FC + robo_map.field_1F4) >> v39;

    if ( v33 < 1 )
        v33 = 1;

    if ( v38 < 1 )
        v38 = 1;

    if ( v43 >= yw->sectors_maxX2 )
        v43 = yw->sectors_maxX2 - 1;

    if ( vii >= yw->sectors_maxY2 )
        vii = yw->sectors_maxY2 - 1;

    char *pcur = sb_0x4f8f64__sub2(yw, t1_cmdbuf_3);

    fntcmd_select_tileset(&pcur, setid);

    sb_0x4f8f64__sub1(yw);

    for (int v42 = v38; v42 <= vii; v42++)
    {
        for (int j = v33; j <= v43; j++ )
        {
            if ( v41 < 2048 )
            {
                cellArea *v17 = &yw->cells[j + yw->sectors_maxX2 * v42];

                if ( robo_map.field_1EF & v17->field_39 )
                {
                    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v17->field_3C.head;
                    while ( bct->next )
                    {
                        if ( bct->field_24 == 3 )
                        {
                            fntcmd_select_tileset(&pcur, 1);

                            pcur = sub_4F6DFC(yw, pcur, yw->tiles[1]->font_height, yw->tiles[1]->chars[24].width, bct, 0);

                            fntcmd_select_tileset(&pcur, setid);

                            v41++;
                        }
                        else if ( bct->field_24 == 9 )
                        {
                            NC_STACK_ypagun *gn = (NC_STACK_ypagun *)bct->self;
                            __NC_STACK_ypagun *gact = &gn->stack__ypagun;

                            if ( !(gact->field_39 & 2) || bct == yw->field_1b84 )
                            {
                                pcur = sub_4F6DFC(yw, pcur, height, width, bct, a6);
                                v41++;
                            }
                        }
                        else
                        {
                            pcur = sub_4F6DFC(yw, pcur, height, width, bct, a6);
                            v41++;
                        }

                        bct = (__NC_STACK_ypabact *)bct->next;
                    }

                }
            }
        }
    }

    if ( robo_map.field_1E8 & 0x200 )
    {
        for (int l = v38; l <= vii; l++ )
        {
            for (int m = v33; m <= v43; m++ )
            {
                if ( v41 < 2048 )
                {
                    cellArea *v25 = &yw->cells[m + l * yw->sectors_maxX2];

                    if ( robo_map.field_1EF & v25->field_39 )
                    {
                        __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v25->field_3C.head;
                        while ( bct->next )
                        {
                            if ( bct->owner == yw->field_1b80->owner
                                    && bct->field_621.sx > flt_516524
                                    && bct->field_621.sz > flt_516528
                                    && bct->field_621.sx < flt_51652C
                                    && bct->field_621.sz < flt_516530 )
                            {
                                if ( bct->field_24 != 4 && bct->field_24 != 3 && bct->field_24 != 9 )
                                {
                                    if ( bct->field_3D5 != 4 && bct->field_3D5 != 5 && bct->field_3D5 != 2 )
                                    {
                                        pcur = sub_4F6980(pcur, bct->field_621.sx, bct->field_621.sz, 0x85, height, width);
                                        v41++;
                                    }
                                }
                            }

                            bct = (__NC_STACK_ypabact *)bct->next;
                        }
                    }
                }
            }
        }
    }
    pcur = sb_0x4f8f64__sub3(yw, pcur);
    pcur = sb_0x4f8f64__sub0(yw, pcur);
    fntcmd_set_end(&pcur);

    sub_423288(&arg);
}

void sub_4C157C(_NC_STACK_ypaworld *yw)
{
    float v1 = (robo_map.frm_1.btn_height - robo_map.field_250) * robo_map.field_1E4;
    float v3 = (robo_map.frm_1.btn_width - robo_map.field_24C) * robo_map.field_1E0;

    float v6 = 1.0 / (yw->sectors_maxX2 * 1200.0);
    int v20 = robo_map.frm_1.btn_width - robo_map.field_248;

    int v17 = v6 * ((robo_map.field_1D8 - v3 * 0.5) * v20);
    int v19 = v20 * v3 * v6;



    if ( v19 >= robo_map.field_228 )
    {
        if ( v19 > v20 )
            v19 = v20;
    }
    else
    {
        v17 -= (robo_map.field_228 - v19) >> 1;
        v19 = robo_map.field_228;
    }

    if ( v17 < 0 )
        v17 = 0;

    if ( v17 + v19 > v20 )
        v17 = v20 - v19;

    robo_map.field_1CC = v20;
    robo_map.field_1CE = v17;
    robo_map.field_1D0 = v19;




    float v15 = -(robo_map.field_1DC + v1 * 0.5);
    float v14 = 1.0 / (yw->sectors_maxY2 * 1200.0);

    v20 = robo_map.frm_1.btn_height - robo_map.field_250;
    v19 = (v20 * v1 * v14);

    int v18 = (v14 * (v20 * v15));


    if ( v19 >= robo_map.field_228 )
    {
        if ( v19 > v20 )
            v19 = v20;
    }
    else
    {
        v18 -= (robo_map.field_228 - v19) >> 1;
        v19 = robo_map.field_228;
    }

    if ( v18 < 0 )
        v18 = 0;

    if ( v19 + v18 > v20 )
        v18 = v20 - v19;

    robo_map.field_1D2 = v20;
    robo_map.field_1D4 = v18;
    robo_map.field_1D6 = v19;
}

void sub_4C0C00(_NC_STACK_ypaworld *yw)
{
    int v20 = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
    int v21 = robo_map.frm_1.btn_ypos - (yw->screen_height / 2);
    int v22 = robo_map.frm_1.btn_width;
    int v23 = robo_map.frm_1.btn_height;

    stru_5B2560.xpos = robo_map.frm_1.btn_width - yw->font_default_w__a;
    stru_5B2560.ypos = 0;
    stru_5B2560.width = yw->font_default_w__a;
    stru_5B2560.fnt_height = yw->font_default_h;

    stru_5B2568.xpos = robo_map.frm_1.btn_width - 2 * yw->font_default_w__a;
    stru_5B2568.ypos = 0;
    stru_5B2568.width = yw->font_default_w__a;
    stru_5B2568.fnt_height = yw->font_default_h;

    stru_5B2550.xpos = 0;
    stru_5B2550.ypos = 0;
    stru_5B2550.width = stru_5B2568.xpos - robo_map.field_238;
    stru_5B2550.fnt_height = yw->font_default_h;

    stru_5B2588.ypos = 0;
    stru_5B2588.xpos = stru_5B2568.xpos - robo_map.field_238;
    stru_5B2588.width = robo_map.field_238;
    stru_5B2588.fnt_height = yw->font_default_h;

    int v5 = robo_map.field_234 + robo_map.field_244;

    for (int i = 3; i < 9; i++)
    {
        robo_map.frm_1.field_18[i]->xpos = v5;
        robo_map.frm_1.field_18[i]->ypos = v23 - robo_map.field_240 - robo_map.field_230;
        robo_map.frm_1.field_18[i]->width = robo_map.field_22C;
        robo_map.frm_1.field_18[i]->fnt_height = robo_map.field_230;

        v5 += robo_map.field_22C;
    }

    int v7;

    if ( robo_map.field_1E8 & 0x20 )
        v7 = 115;
    else
        v7 = 113;

    const char *v10 = get_lang_string(yw->string_pointers_p2, 50, "MAP");
    char *pcur = lstvw_make_title(yw, v20, v21, v22, v10, robo_map.cmdstrm.cmdbuf, v7, robo_map.cmd_flag);

    fntcmd_next_line(&pcur);
    fntcmd_reset_tileset(&pcur, 13);

    fntcmd_store_u8(&pcur, 65);

    fntcmd_next_line(&pcur);
    fntcmd_reset_tileset(&pcur, 12);

    int v13 = v23 - robo_map.field_250 - 1;
    while (v13 > yw->font_yscrl_h)
    {
        fntcmd_store_u8(&pcur, 65);
        fntcmd_next_line(&pcur);

        v13 -= yw->font_yscrl_h;
    }

    if ( v13 > 1 )
    {
        fntcmd_set_yheight(&pcur, v13 - 1);
        fntcmd_store_u8(&pcur, 65);
        fntcmd_next_line(&pcur);
    }

    fntcmd_reset_tileset(&pcur, 13);
    fntcmd_store_u8(&pcur, 66);
    fntcmd_next_line(&pcur);

    fntcmd_include(&pcur, 0);
    fntcmd_include(&pcur, 1);
    fntcmd_include(&pcur, 2);
    fntcmd_set_end(&pcur);
}

void sub_4C0FEC(_NC_STACK_ypaworld *yw)
{
    int v1 = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
    int v3 = robo_map.frm_1.btn_ypos + robo_map.frm_1.btn_height - robo_map.field_240 - (yw->screen_height / 2);

    stru_5B25D0.xpos = 0;
    stru_5B25D0.ypos = robo_map.frm_1.btn_height - robo_map.field_240;
    stru_5B25D0.width = robo_map.field_1CE;
    stru_5B25D0.fnt_height = robo_map.field_240;

    stru_5B25B0.xpos = robo_map.field_1CE;
    stru_5B25B0.ypos = robo_map.frm_1.btn_height - robo_map.field_240;
    stru_5B25B0.width = robo_map.field_1D0;
    stru_5B25B0.fnt_height = robo_map.field_240;

    stru_5B25C8.xpos = robo_map.frm_1.btn_width - robo_map.field_248;
    stru_5B25C8.ypos = robo_map.frm_1.btn_height - robo_map.field_240;
    stru_5B25C8.width = robo_map.field_248;
    stru_5B25C8.fnt_height = robo_map.field_240;

    stru_5B25A0.xpos = robo_map.field_1D0 + robo_map.field_1CE;
    stru_5B25A0.ypos = robo_map.frm_1.btn_height - robo_map.field_240;
    stru_5B25A0.width = robo_map.field_1CC - robo_map.field_1CE - robo_map.field_1D0;
    stru_5B25A0.fnt_height = robo_map.field_240;

    char *pcur = t1_cmdbuf_2;

    fntcmd_select_tileset(&pcur, 11);
    fntcmd_set_center_xpos(&pcur, v1);
    fntcmd_set_center_ypos(&pcur, v3);

    int v4 = robo_map.field_1CE;

    if ( v4 > 0 )
    {
        fntcmd_store_u8(&pcur, 65);
        if ( v4 > 1 )
        {
            fntcmd_op17(&pcur, v4);
            fntcmd_store_u8(&pcur, 66);
        }
    }

    fntcmd_store_u8(&pcur, 68);

    fntcmd_op17(&pcur, robo_map.field_1D0 + v4 - 1);

    fntcmd_store_u8(&pcur, 69);
    fntcmd_store_u8(&pcur, 70);


    int v11 = v4 + robo_map.field_1D0;
    if ( v11 < robo_map.field_1CC )
    {
        if ( v11 < robo_map.field_1CC - 1 )
        {
            fntcmd_op17(&pcur, robo_map.field_1CC - 1);

            fntcmd_store_u8(&pcur, 66);
        }
        fntcmd_store_u8(&pcur, 67);
    }

    fntcmd_store_u8(&pcur, 71);

    fntcmd_set_end(&pcur);
}

void sub_4C1214(_NC_STACK_ypaworld *yw)
{
    int v2 = robo_map.frm_1.btn_xpos + robo_map.frm_1.btn_width - robo_map.field_248 - (yw->screen_width / 2);
    int v3 = robo_map.frm_1.btn_ypos + robo_map.field_23C - (yw->screen_height / 2);

    stru_5B2558.xpos = robo_map.frm_1.btn_width - robo_map.field_248;
    stru_5B2558.ypos = robo_map.field_23C;
    stru_5B2558.width = robo_map.field_248;
    stru_5B2558.fnt_height = robo_map.field_1D4;

    stru_5B2578.xpos = robo_map.frm_1.btn_width - robo_map.field_248;
    stru_5B2578.ypos = robo_map.field_1D4 + robo_map.field_23C;
    stru_5B2578.width = robo_map.field_248;
    stru_5B2578.fnt_height = robo_map.field_1D6;

    stru_5B25A8.xpos = robo_map.frm_1.btn_width - robo_map.field_248;
    stru_5B25A8.ypos = robo_map.field_1D6 + robo_map.field_1D4 + robo_map.field_23C;
    stru_5B25A8.width = robo_map.field_248;
    stru_5B25A8.fnt_height = robo_map.field_1D2 - robo_map.field_1D4 - robo_map.field_1D6;

    char *pcur = t1_cmdbuf_1;

    fntcmd_set_center_xpos(&pcur, v2);
    fntcmd_set_center_ypos(&pcur, v3);


    if ( robo_map.field_1D4 > 0 )
    {
        fntcmd_reset_tileset(&pcur, 13);

        fntcmd_store_u8(&pcur, 67);
        fntcmd_next_line(&pcur);

        fntcmd_reset_tileset(&pcur, 12);

        int v9 = robo_map.field_1D4 - 1;

        while (v9 >= yw->font_yscrl_h )
        {
            fntcmd_store_u8(&pcur, 66);
            fntcmd_next_line(&pcur);

            v9 -= yw->font_yscrl_h;
        }

        if ( v9 > 0 )
        {
            fntcmd_set_yheight(&pcur, v9);
            fntcmd_store_u8(&pcur, 66);
            fntcmd_next_line(&pcur);
        }
    }

    if ( robo_map.field_1D6 > 0 )
    {
        fntcmd_reset_tileset(&pcur, 13);
        fntcmd_store_u8(&pcur, 69);

        fntcmd_next_line(&pcur);

        fntcmd_reset_tileset(&pcur, 12);


        int v14 = robo_map.field_1D6 - 1;

        while ( v14 > yw->font_yscrl_h )
        {
            fntcmd_store_u8(&pcur, 67);
            fntcmd_next_line(&pcur);

            v14 -= yw->font_yscrl_h;
        }

        if ( v14 > 1 )
        {
            fntcmd_set_yheight(&pcur, v14 - 1);
            fntcmd_store_u8(&pcur, 67);
            fntcmd_next_line(&pcur);
        }

        fntcmd_reset_tileset(&pcur, 13);

        fntcmd_store_u8(&pcur, 70);
        fntcmd_next_line(&pcur);
    }

    int v21 = robo_map.field_1D2 - robo_map.field_1D4 - robo_map.field_1D6;

    if ( v21 > 0 )
    {
        fntcmd_reset_tileset(&pcur, 12);

        while ( v21 > yw->font_yscrl_h )
        {
            fntcmd_store_u8(&pcur, 66);
            fntcmd_next_line(&pcur);

            v21 -= yw->font_yscrl_h;
        }

        if ( v21 > 1 )
        {
            fntcmd_set_yheight(&pcur, v21 - 1);

            fntcmd_store_u8(&pcur, 66);
            fntcmd_next_line(&pcur);
        }
        fntcmd_reset_tileset(&pcur, 13);

        fntcmd_store_u8(&pcur, 68);
        fntcmd_next_line(&pcur);
    }
    fntcmd_set_end(&pcur);
}



char * sub_4F6114(_NC_STACK_ypaworld *yw, int a2, char *cmdbuf, int a3, int a4, int a5, int a6, int a7, mapFunc ffunc, int xpos, int ypos)
{
    char *pcur = cmdbuf;

    int v33;

    if ( a2 == 4)
        v33 = 2;
    else if ( a2 == 8)
        v33 = 3;
    else if ( a2 == 16 )
        v33 = 4;
    else if ( a2 == 32 )
        v33 = 5;
    else if ( a2 == 64 )
        v33 = 6;
    else
    {
        ypa_log_out("yw_maprnd.c/yw_RMapLego(): wrong size\n");

        fntcmd_set_end(&pcur);
        return pcur;
    }

    fntcmd_set_center_xpos(&pcur, xpos);
    fntcmd_set_center_ypos(&pcur, ypos);

    int msk = a2 - 1;

    fntcmd_reset_tileset(&pcur, a7);

    int row = a4;

    while (row < a6)
    {
        int v37 = 0;
        int col = a3;
        int v17 = row >> v33;

        if ( msk & row )
        {
            fntcmd_set_yoff(&pcur, row & msk);

            row += a2 - (msk & row);
        }
        else
        {
            if ( a6 - row >= a2 )
            {
                row += a2;
            }
            else
            {
                fntcmd_set_yheight(&pcur, a6 - row);

                row = a6;
            }
        }

        if ( col & msk )
        {
            int v37_4 = msk & col;

            int v21 = ffunc(yw, col >> v33, v17) & 0xFF;

            if ( v21 )
            {
                fntcmd_set_xoff(&pcur, v37_4);
                fntcmd_store_u8(&pcur, v21);

                v37 = 0;
            }
            else
            {
                v37 = a2 - v37_4;

                fntcmd_add_xpos(&pcur, v37);
            }

            col += a2 - v37_4;
        }

        while ( col < a5 - a2 )
        {
            int v24 = ffunc(yw, col >> v33, v17) & 0xFF;

            if ( v24 )
            {
                fntcmd_store_u8(&pcur, v24);

                v37 = 0;
            }
            else
            {
                if ( v37 <= 0 )
                {
                    v37 = a2;

                    fntcmd_add_xpos(&pcur, v37);
                }
                else
                {
                    v37 += a2;

                    pcur -= 2; //HACKY rewrite
                    fntcmd_store_s16(&pcur, v37);
                }
            }

            col += a2;
        }

        if ( a5 - col > 0 )
        {
            int v28 = ffunc(yw, col >> v33, v17) & 0xFF;
            if ( v28 )
            {
                fntcmd_set_xwidth(&pcur, a5 - col);
                fntcmd_store_u8(&pcur, v28);
            }
        }
        fntcmd_next_line(&pcur);

    }

    return pcur;
}



int sub_4F60A4(_NC_STACK_ypaworld *yw, int x, int y)
{
    if ( x >= 0 && x < yw->sectors_maxX2 && y >= 0 && y < yw->sectors_maxY2 )
    {
        cellArea *v5 = &yw->cells[yw->sectors_maxX2 * y + x];

        if ( (robo_map.field_1EC & 3) && (robo_map.field_1EF & v5->field_39) )
            return 0;
        else
            return 9;
    }
    else
        return 9;
}

int sub_4F6048(_NC_STACK_ypaworld *yw, int x, int y)
{
    if ( x >= 0 && x < yw->sectors_maxX2 && y >= 0 && y < yw->sectors_maxY2 )
    {
        cellArea *v4 = &yw->cells[yw->sectors_maxX2 * y + x];

        if ( robo_map.field_1EF & v4->field_39 )
        {
            if ( !v4->owner )
                return 8;
            else
                return v4->owner;
        }
    }

    return 0;
}

int sub_4F5FE0(_NC_STACK_ypaworld *yw, int x, int y)
{
    if ( x >= 0 && x < yw->sectors_maxX2 && y >= 0 && y < yw->sectors_maxY2 )
    {
        cellArea *v6 = &yw->cells[yw->sectors_maxX2 * y + x];

        if ( robo_map.field_1EF & v6->field_39 )
            return yw->secTypes[v6->sec_type].field_3;
    }

    return 0;
}

int sub_4F5CEC(_NC_STACK_ypaworld *yw, int x, int y)
{
    int v9 = x / 4;
    int v8 = x & 3;
    int v7 = y / 4;
    int v27 = y & 3;

    if ( v9 < 0 || v9 >= yw->sectors_maxX2 || v7 < 0 || v7 >= yw->sectors_maxY2 )
        return 0;

    cellArea *v12 = &yw->cells[v9 + v7 * yw->sectors_maxX2];

    if ( !(robo_map.field_1EF & v12->field_39) )
        return 0;

    if ( v8 && v27 )
    {
        if ( v12->field_2E == 1 )
        {
            cityBases *v26 = yw->legos + yw->secTypes[ v12->sec_type ].buildings[0][0]->health_models[  yw->build_hp_ref[  v12->buildings_health[0][0]  ]  ];
            int v25 = (16 * (v27 - 1) + v8 - 1 + v26->field_12) & 0xFF;
            return (v26->field_11 << 8) | v25;
        }
        else
        {
            cityBases *v26 = yw->legos + yw->secTypes[ v12->sec_type ].buildings[v8 - 1][2 - (v27 - 1)]->health_models[  yw->build_hp_ref[  v12->buildings_health[v8 - 1][2 - (v27 - 1)]  ]  ];
            return (v26->field_11 << 8) | v26->field_12;
        }
    }
    else
    {
        if ( v9 < 1 || v7 < 1 )
            return 1;

        int v13 = 0;

        cellArea *v15;
        cellArea *v16;
        cellArea *v17;

        if ( v8 )
        {
            v15 = v12;
            v16 = v12 - yw->sectors_maxX2;
            v17 = v12 - yw->sectors_maxX2;
        }
        else if ( v27 )
        {
            v17 = v12 - 1;
            v15 = v12 - 1;
            v16 = v12;
        }
        else
        {
            v15 = v12 - 1;
            v16 = v12 - yw->sectors_maxX2;
            v17 = v16 - 1;
        }


        if ( fabs(v17->sector_height_meters - v16->sector_height_meters) >= 500.0 )
            v13 = 1;

        if ( fabs(v15->sector_height_meters - v12->sector_height_meters) >= 500.0 )
            v13 |= 2;

        if ( fabs(v17->sector_height_meters - v15->sector_height_meters) >= 500.0 )
            v13 |= 4;

        if ( fabs(v16->sector_height_meters - v12->sector_height_meters) >= 500.0 )
            v13 |= 8;

        if ( v13 == 12 )
        {
            if ( v17->sector_height_meters <= v15->sector_height_meters )
                v13 = 0xFD;
            else
                v13 = 0xFC;
        }
        else if ( v13 == 3 )
        {
            if ( v17->sector_height_meters <= v16->sector_height_meters )
                v13 = 0xFF;
            else
                v13 = 0xFC;
        }

        int v22 = (v13 - 16) & 0xFF;
        return (v22 << 8) | v22;
    }
}

char * sb_0x4f6650__sub0(_NC_STACK_ypaworld *yw, char *cmdbuf, int xpos, int ypos, int w, int h)
{
    float v7 = robo_map.field_1D8 / robo_map.field_1E0;
    float v8 = -robo_map.field_1DC / robo_map.field_1E4;
    int v9 = v7 - (w / 2);
    int v23 = w + v9;
    int v10 = v8 - (h / 2);
    int v22 = h + v10;

    char *pcur = sub_4F6114(yw, 4 << robo_map.field_1EE, cmdbuf, v9, v10, v23, v22, robo_map.field_1EE + 50, sub_4F60A4, xpos, ypos);

    if ( robo_map.field_1EC & 2 )
        pcur = sub_4F6114(yw, 4 << robo_map.field_1EE, pcur, v9, v10, v23, v22, robo_map.field_1EE + 50, sub_4F6048, xpos, ypos);

    if ( robo_map.field_1EC & 1 )
    {
        switch ( robo_map.field_1EE )
        {
        case 0:
            pcur = sub_4F6114(yw, 4, pcur, v9, v10, v23, v22, 43, sub_4F5FE0, xpos, ypos);
            break;

        case 1:
            pcur = sub_4F6114(yw, 8, pcur, v9, v10, v23, v22, 44, sub_4F5FE0, xpos, ypos);
            break;

        case 2:
            pcur = sub_4F6114(yw, 4, pcur, v9 + 2, v10 + 2, v23 + 2, v22 + 2, 42, sub_4F5CEC, xpos, ypos);
            break;

        case 3:
            pcur = sub_4F6114(yw, 8, pcur, v9 + 4, v10 + 4, v23 + 4, v22 + 4, 41, sub_4F5CEC, xpos, ypos);
            break;

        case 4:
            pcur = sub_4F6114(yw, 16, pcur, v9 + 8, v10 + 8, v23 + 8, v22 + 8, 40, sub_4F5CEC, xpos, ypos);
            break;
        }
    }
    return pcur;
}

char *sb_0x4f6650(_NC_STACK_ypaworld *yw, char *cmdbuf, int x, int y)
{
    char *pcur = sb_0x4f6650__sub0(yw, cmdbuf, x, y, robo_map.frm_1.btn_width - robo_map.field_24C, robo_map.frm_1.btn_height - robo_map.field_250);
    fntcmd_set_end(&pcur);
    return pcur;
}

void sub_4C1814(_NC_STACK_ypaworld *yw, int a2, int a3)
{
    float v15 = yw->sectors_maxX2 * 1200.0;
    float v12 = yw->sectors_maxY2 * 1200.0;

    float v4 = a2 * robo_map.field_1E0;
    float v13 = v4 * 0.5;

    if ( v4 <= v15 )
    {
        if ( robo_map.field_1D8 - v13 >= 0.0 )
        {
            if ( robo_map.field_1D8 + v13 > v15 )
                robo_map.field_1D8 = v15 - v13;
        }
        else
        {
            robo_map.field_1D8 = v13;
        }
    }
    else
    {
        robo_map.field_1D8 = v15 * 0.5;
    }

    float v8 = a3 * robo_map.field_1E4;
    float v14 = v8 * 0.5;

    if ( v8 > v12 )
    {
        robo_map.field_1DC = -v12 * 0.5;
    }
    else
    {
        if ( robo_map.field_1DC + v14 > 0.0 )
        {
            robo_map.field_1DC = -v8 * 0.5;
        }
        else
        {
            if ( robo_map.field_1DC - v14 < -v12 )
                robo_map.field_1DC = -v12 + v14;
        }
    }
}

void  sb_0x451034__sub2(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    memset(byte_5BA6E8, 0, sizeof(byte_5BA6E8));
    memset(t1_cmdbuf_1, 0, sizeof(t1_cmdbuf_1));
    memset(t1_cmdbuf_2, 0, sizeof(t1_cmdbuf_2));
    memset(t1_cmdbuf_3, 0, sizeof(t1_cmdbuf_3));

    robo_map.cmd_flag = 0x138;
    robo_map.field_228 = 8;
    robo_map.field_22C = yw->tiles[10]->chars[65].width;
    robo_map.field_230 = yw->tiles[10]->font_height;
    robo_map.field_234 = 4;
    robo_map.field_238 = yw->tiles[0]->chars[113].width;
    robo_map.field_23C = yw->font_default_h;
    robo_map.field_240 = yw->font_xscrl_h;
    robo_map.field_244 = yw->tiles[12]->chars[65].width;
    robo_map.field_248 = yw->font_yscrl_bkg_w;
    robo_map.field_24C = robo_map.field_248 + robo_map.field_244;
    robo_map.field_250 = robo_map.field_240 + robo_map.field_23C;
    robo_map.field_210 = robo_map.field_248 + 7 * robo_map.field_22C + robo_map.field_244 + 8;
    robo_map.field_212 = robo_map.field_240 + robo_map.field_23C + 96;
    robo_map.field_214 = 32000;
    robo_map.field_216 = 32000;
    robo_map.frm_1.btn_xpos = 0;
    robo_map.frm_1.btn_ypos = yw->icon_energy__h;
    robo_map.frm_1.btn_width = 2 * yw->screen_width / 3;
    robo_map.frm_1.btn_height = 2 * yw->screen_height / 3;
    robo_map.field_1EE = 4;
    robo_map.field_1EC = 7;
    robo_map.frm_1.field_10 = 18;
    robo_map.frm_1.field_18[0] = &stru_5B2560;
    robo_map.frm_1.field_18[1] = &stru_5B2550;
    robo_map.frm_1.field_18[2] = &stru_5B2568;
    robo_map.frm_1.field_18[3] = &stru_5B25E0;
    robo_map.frm_1.field_18[4] = &stru_5B2580;
    robo_map.frm_1.field_18[5] = &stru_5B2598;
    robo_map.frm_1.field_18[6] = &stru_5B25C0;
    robo_map.frm_1.field_18[7] = &stru_5B2590;
    robo_map.frm_1.field_18[8] = &stru_5B2570;
    robo_map.frm_1.field_18[9] = &stru_5B2588;
    robo_map.frm_1.field_18[10] = &stru_5B2558;
    robo_map.frm_1.field_18[11] = &stru_5B2578;
    robo_map.frm_1.field_18[12] = &stru_5B25A8;
    robo_map.frm_1.field_18[13] = &stru_5B25D0;
    robo_map.frm_1.field_18[14] = &stru_5B25B0;
    robo_map.frm_1.field_18[15] = &stru_5B25A0;
    robo_map.frm_1.field_18[16] = &stru_5B25C8;
    robo_map.frm_1.field_18[17] = &stru_5B25D8;

    robo_map.cmdstrm.cmdbuf = byte_5BA6E8;
    robo_map.cmdstrm.includ = t1_cmdbufs;

    robo_map.field_1C8 = sb_0x4f8f64;

    if ( yw->field_1b80 )
    {
        robo_map.field_1D8 = yw->field_1b80->field_621.sx;
        robo_map.field_1DC = yw->field_1b80->field_621.sz;
    }
    else
    {
        robo_map.field_1DC = -39000.0;
        robo_map.field_1D8 = 39000.0;
    }

    switch ( robo_map.field_1EE )
    {
    case 0:
        robo_map.field_1E4 = 300.0;
        robo_map.field_1E0 = 300.0;
        break;

    case 1:
        robo_map.field_1E4 = 150.0;
        robo_map.field_1E0 = 150.0;
        break;

    case 2:
        robo_map.field_1E4 = 75.0;
        robo_map.field_1E0 = 75.0;
        break;

    case 3:
        robo_map.field_1E4 = 37.5;
        robo_map.field_1E0 = 37.5;
        break;

    case 4:
        robo_map.field_1E4 = 18.75;
        robo_map.field_1E0 = 18.75;
        break;
    }

    robo_map.field_1ED = 0;
    robo_map.field_1E8 = 0;
    robo_map.field_1EF = 2;

    robo_map.field_254 = 0.45;
    robo_map.field_258 = 0;
    robo_map.field_25C = 0.95;
    robo_map.field_260 = 0.6;

    robo_map.field_208 = yw->screen_width - robo_map.field_210;
    robo_map.field_20A = yw->screen_height - yw->icon0___h - robo_map.field_212;
    robo_map.field_20C = robo_map.field_210;
    robo_map.field_20E = robo_map.field_212;

    sub_4C157C(yw);
    sub_4C0C00(yw);
    sub_4C0FEC(yw);
    sub_4C1214(yw);

    int v15 = robo_map.frm_1.btn_xpos + robo_map.field_244 - (yw->screen_width / 2);
    int v14 = robo_map.frm_1.btn_ypos + robo_map.field_23C - (yw->screen_height / 2);

    stru_5B25D8.xpos = robo_map.field_244;
    stru_5B25D8.ypos = robo_map.field_23C;
    stru_5B25D8.width = robo_map.frm_1.btn_width - robo_map.field_24C;
    stru_5B25D8.fnt_height = robo_map.frm_1.btn_height - robo_map.field_250;

    sb_0x4f6650(yw, t1_cmdbuf_3, v15, v14);

    sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
}


void sub_4C32A8(_NC_STACK_ypaworld *yw, save_status *status)
{
    if ( status->p1 )
    {
        if ( status->p4 < yw->icon_energy__h
                || status->p3 + status->p5 > yw->screen_width
                || status->p4 + status->p6 > yw->screen_height - yw->icon0___h )
        {
            status->p1 = 0;
        }
    }

    if ( status->p1 )
    {
        if ( yw->robo_map_status.pX[4] < yw->icon_energy__h
                || yw->robo_map_status.pX[3] + yw->robo_map_status.pX[5] > yw->screen_width
                || yw->robo_map_status.pX[6] + yw->robo_map_status.pX[4] > yw->screen_height - yw->icon0___h )
            status->p1 = 0;
    }
}

void sub_4C1970(_NC_STACK_ypaworld *yw, int a2)
{
    if ( a2 == 1 )
        robo_map.field_1EE++;
    else if ( a2 == 2 )
        robo_map.field_1EE--;


    if ( robo_map.field_1EE > 4 )
        robo_map.field_1EE = 4;
    else if ( robo_map.field_1EE < 1 )
        robo_map.field_1EE = 1;

    switch ( robo_map.field_1EE )
    {
    case 0:
        robo_map.field_1E4 = 300.0;
        robo_map.field_1E0 = 300.0;
        break;

    case 1:
        robo_map.field_1E4 = 150.0;
        robo_map.field_1E0 = 150.0;
        break;

    case 2:
        robo_map.field_1E4 = 75.0;
        robo_map.field_1E0 = 75.0;
        break;

    case 3:
        robo_map.field_1E4 = 37.5;
        robo_map.field_1E0 = 37.5;
        break;

    case 4:
        robo_map.field_1E4 = 18.75;
        robo_map.field_1E0 = 18.75;
        break;
    }
}

int sb_0x451034__sub3(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    memset(&bzda, 0, sizeof(bzd));

    int v3 = yw->screen_width - 11 * yw->icon_order__w;
    bzda.field_8FC = v3 / 4;

    int v5;
    if ( yw->screen_width >= 512 )
        v5 = 7 * yw->tiles[0]->chars[87].width;
    else
        v5 = 9 * yw->tiles[0]->chars[87].width;

    bzda.field_900 = 2 * v5;
    bzda.field_904 = bzda.field_8FC / 2;
    bzda.field_908 = yw->icon_order__w + bzda.field_8FC + bzda.field_8FC / 2;
    bzda.field_90C = 2 * yw->icon_order__w + bzda.field_908 + bzda.field_8FC;
    bzda.field_910 = 5 * yw->icon_order__w + bzda.field_90C + bzda.field_8FC;
    bzda.field_914 = 1;
    bzda.field_918 = 0;
    bzda.cmd_flag = 0;
    bzda.frm_1.field_10 = 11;
    bzda.frm_1.field_18[0] = &create_btn;
    bzda.frm_1.field_18[1] = &into_vhcl_btn;
    bzda.frm_1.field_18[2] = &map_btn;
    bzda.frm_1.field_18[3] = &squad_btn;
    bzda.frm_1.field_18[4] = &to_host_btn;
    bzda.frm_1.field_18[5] = &to_leader_btn;
    bzda.frm_1.field_18[6] = &turrets_btn;
    bzda.frm_1.field_18[7] = &next_squad_btn;
    bzda.frm_1.field_18[8] = &analyzer_btn;
    bzda.frm_1.field_18[9] = &help_btn;
    bzda.frm_1.field_18[10] = &menu_btn;
    bzda.frm_1.btn_xpos = 0;
    bzda.frm_1.btn_ypos = yw->screen_height - yw->icon_order__h;
    bzda.frm_1.btn_width = yw->screen_width;
    bzda.frm_1.btn_height = yw->icon_order__h;

    for (int i = 0; i < 11; i++)
        memset(bzda.frm_1.field_18[i], 0, sizeof(button_str1));

    bzda.cmdstrm.cmdbuf = byte_516534;
    bzda.field_1D8 = 0;

    sub_4C3A54(yw);

    bzda.field_8EC = 0;
    bzda.field_8F0 = 0;
    bzda.field_8F4 = 0;
    bzda.field_8F8 = 0;

    if ( bzda.field_1D4 & 1 )
    {
        if ( !(bzda.field_1D8 & 1) )
        {
            bzda.field_1D8 |= 1;
            sub_412D48(&bzda.frm_1, 0);
        }
    }

    if ( !lstvw_init(
                yw,
                &gui_lstvw,
                0x80000002,
                0,
                0x80000003,
                8,
                0x80000004,
                8,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                16,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                bzda.field_900,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000011,
                1,
                0x80000017,
                1,
                0)
       )
        return 0;


    sub_4C32A8(yw, &yw->robo_map_status);
    if ( yw->robo_finder_status.p1 )
    {
        if ( yw->robo_finder_status.p4 < yw->icon_energy__h
                || yw->robo_finder_status.p3 + yw->robo_finder_status.p5 > yw->screen_width
                || yw->robo_finder_status.p6 + yw->robo_finder_status.p4 > yw->screen_height - yw->icon0___h )
        {
            yw->robo_finder_status.p1 = 0;
        }
    }

    sub_4C32A8(yw, &yw->vhcl_map_status);
    if ( yw->vhcl_finder_status.p1 )
    {
        if ( yw->vhcl_finder_status.p4 < yw->icon_energy__h
                || yw->vhcl_finder_status.p5 + yw->vhcl_finder_status.p3 > yw->screen_width
                || yw->vhcl_finder_status.p6 + yw->vhcl_finder_status.p4 > yw->screen_height - yw->icon0___h )
        {
            yw->vhcl_finder_status.p1 = 0;
        }
    }

    if ( !yw->robo_map_status.pX[5] || !yw->robo_map_status.pX[6] )
        yw->robo_map_status.p1 = 0;

    if ( !yw->vhcl_map_status.pX[5] || !yw->vhcl_map_status.pX[6] )
        yw->vhcl_map_status.p1 = 0;

    if ( yw->robo_map_status.p1 )
    {
        if ( yw->robo_map_status.p2 )
        {
            if ( robo_map.cmd_flag & 0x20 )
            {
                robo_map.cmd_flag &= 0xFFFFFFDF;

                sub_412D48(&robo_map.frm_1, 0);

                yw->field_17bc = 0;
            }

            if ( !(robo_map.cmd_flag & 0x20) )
            {
                Remove(&robo_map);
                AddHead(&yw->field_17a0, &robo_map);

                sub_412D9C(&robo_map.frm_1);

                sub_412D48(&robo_map.frm_1, 0);
            }
        }
        else if ( !(robo_map.cmd_flag & 0x20) )
        {
            robo_map.cmd_flag |= 0x20;

            sub_412D9C(&robo_map.frm_1);

            yw->field_17bc = 0;
        }

        robo_map.frm_1.btn_xpos = yw->robo_map_status.p3;
        robo_map.frm_1.btn_ypos = yw->robo_map_status.p4;
        robo_map.frm_1.btn_width = yw->robo_map_status.p5;
        robo_map.frm_1.btn_height = yw->robo_map_status.p6;
        robo_map.field_1EC = yw->robo_map_status.pX[0];
        robo_map.field_1ED = yw->robo_map_status.pX[1];
        robo_map.field_1EE = yw->robo_map_status.pX[2];
        robo_map.field_208 = yw->robo_map_status.pX[3];
        robo_map.field_20A = yw->robo_map_status.pX[4];
        robo_map.field_20C = yw->robo_map_status.pX[5];
        robo_map.field_20E = yw->robo_map_status.pX[6];

        sub_4C1970(yw, 0);
    }


    if ( yw->robo_finder_status.p1 )
    {
        if ( yw->robo_finder_status.p2 )
        {
            if ( squadron_manager.lstvw.cmd_flag & 0x20 )
            {
                squadron_manager.lstvw.cmd_flag &= 0xFFFFFFDF;
                sub_412D48(&squadron_manager.lstvw.frm_1, 0);
                yw->field_17bc = 0;
            }
            if ( !(squadron_manager.lstvw.cmd_flag & 0x20) )
            {
                Remove(&squadron_manager.lstvw);
                AddHead(&yw->field_17a0, &squadron_manager.lstvw);
                sub_412D9C(&squadron_manager.lstvw.frm_1);
                sub_412D48(&squadron_manager.lstvw.frm_1, 0);
            }
        }
        else if ( !(squadron_manager.lstvw.cmd_flag & 0x20) )
        {
            squadron_manager.lstvw.cmd_flag |= 0x20;
            sub_412D9C(&squadron_manager.lstvw.frm_1);
            yw->field_17bc = 0;
        }

        squadron_manager.lstvw.frm_1.btn_xpos = yw->robo_finder_status.p3;
        squadron_manager.lstvw.frm_1.btn_ypos = yw->robo_finder_status.p4;
        squadron_manager.lstvw.frm_1.btn_width = yw->robo_finder_status.p5;
        squadron_manager.lstvw.frm_1.btn_height = yw->robo_finder_status.p6;

        lstvw_updlimits(yw, &squadron_manager.lstvw, -2, -2);
    }

    return 1;
}

void sb_0x451034__sub1(_NC_STACK_ypaworld *yw)
{
    inputEngine__getter(0x80001009, &yw->input_class, 0);

    yw->field_7562 = 0;
    yw->field_7566 = -1;
    yw->field_756A = 0;
    yw->field_756E = 0;
}



void create_info_log(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    memset(&info_log, 0, sizeof(info_log));

    if ( lstvw_init(
                yw,
                &info_log.window,
                0x80000001,
                get_lang_string(yw->string_pointers_p2, 52, "MESSAGE LOG"),
                0x80000002,
                1,
                0x80000003,
                1,
                0x80000004,
                6,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                24,
                0x80000012,
                3,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                200,
                0x8000000E,
                32000,
                0x8000000D,
                2 * (5 * yw->tiles[0]->chars[48].width),
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000014,
                74,
                0) )
    {
        if ( yw->field_1614 )
        {
            char a1a[32];
            sprintf(a1a, "<%02d:%02d:%02d>", (yw->field_1614 >> 10) / 60 / 60 % 24, (yw->field_1614 >> 10) / 60 % 60, (yw->field_1614 >> 10) % 60);
            sprintf(info_log.msgs[0].txt, get_lang_string(yw->string_pointers_p2, 12, "GAME CONTINUED AT TIME INDEX %s."), a1a);
        }
        else
            strcpy(info_log.msgs[0].txt, get_lang_string(yw->string_pointers_p2, 6, "WELCOME TO YOUR PERSONAL AMOK!"));

        info_log.msgs[0].field_4 = yw->field_1614 + 1;
        info_log.field_255C = 0;
        info_log.field_2560 = yw->field_1614;
        info_log.field_2564 = 0;
        info_log.field_250 = 127;
        info_log.field_248 = 6 * yw->tiles[0]->chars[48].width + 2 * yw->tiles[0]->chars[58].width + 12;
    }
}




void create_exit_menu(_NC_STACK_ypaworld *yw)
{
    dword_5C8B7C = (yw->font_default_h / 2) + yw->font_default_h;
    dword_5C8B84 = yw->font_default_h / 4;

    int tmp = sub_4513E0("WWWWWWW", yw->tiles[0]);

    if ( lstvw_init(
                yw,
                &exit_menu,
                0x80000001,
                get_lang_string(yw->string_pointers_p2, 53, "GAME PAUSED"),
                0x80000002,
                0,
                0x80000003,
                5,
                0x80000004,
                5,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                5,
                0x80000012,
                5,
                0x80000008,
                0,
                0x8000000B,
                dword_5C8B7C,
                0x8000000C,
                (2 * tmp + 16),
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000013,
                1,
                0x80000014,
                85,
                0) )
    {
        int v5 = yw->font_default_h + exit_menu.field_1E8 + dword_5C8B84;

        dword_5C8B80 = 2 * yw->tiles[0]->chars[32].width + yw->font_default_w__b;
        dword_5C8B88 = exit_menu.width - 2 * dword_5C8B80;

        for (int i = 8; i < 13; i++)
        {
            exit_menu.frm_1.field_18[i]->xpos = dword_5C8B80;
            exit_menu.frm_1.field_18[i]->ypos = (i - 8) * dword_5C8B7C + v5;
            exit_menu.frm_1.field_18[i]->width = dword_5C8B88;
            exit_menu.frm_1.field_18[i]->fnt_height = yw->font_default_h;
        }
    }
}


void sb_0x451034__sub5(_NC_STACK_ypaworld *yw)
{
    int v2 = sub_4513E0("WWWWWWW", yw->tiles[0]) * 3.5 + 16;

    if ( lstvw_init(
                yw,
                &lstvw2,
                0x80000001,
                " ",
                0x80000002,
                0,
                0x80000003,
                3,
                0x80000004,
                3,
                0x80000005,
                0,
                0x80000006,
                0,
                0x80000007,
                3,
                0x80000012,
                3,
                0x80000008,
                0,
                0x8000000B,
                yw->font_default_h,
                0x8000000C,
                v2,
                0x8000000F,
                1,
                0x80000010,
                yw->field_1a38,
                0x80000013,
                1,
                0x80000014,
                85,
                0x80000018,
                0,
                0) )
    {
        int v1 = yw->font_default_h + lstvw2.field_1E8 + 2 * lstvw2.font_h;

        dword_5BAFA8 = 2 * yw->tiles[0]->chars[32].width + yw->font_default_w__b;
        dword_5BAFA0 = (lstvw2.width - 2 * dword_5BAFA8) / 3;

        lstvw2.frm_1.field_10 = 10;

        lstvw2.frm_1.field_18[8]->ypos = v1;
        lstvw2.frm_1.field_18[8]->xpos = dword_5BAFA8;
        lstvw2.frm_1.field_18[8]->width = dword_5BAFA0;
        lstvw2.frm_1.field_18[8]->fnt_height = lstvw2.font_h;

        lstvw2.frm_1.field_18[9]->ypos = v1;
        lstvw2.frm_1.field_18[9]->width = dword_5BAFA0;
        lstvw2.frm_1.field_18[9]->xpos = lstvw2.width - dword_5BAFA8 - dword_5BAFA0;
        lstvw2.frm_1.field_18[9]->fnt_height = lstvw2.font_h;

        dword_5BAFA4 = lstvw2.frm_1.field_18[9]->xpos - (dword_5BAFA0 + lstvw2.frm_1.field_18[8]->xpos);

        dword_5BAF9C = 0;
    }
}


void sb_0x451034__sub9(_NC_STACK_ypaworld *yw)
{
    const char *off_51825C[14] =
    {
        "wireless/bar.sklt",
        "wireless/compass.sklt",
        "wireless/frame.sklt",
        "wireless/arrow.sklt",
        "wireless/mgvis.sklt",
        "wireless/grenvis1.sklt",
        "wireless/rockvis1.sklt",
        "wireless/missvis1.sklt",
        "wireless/bombvis1.sklt",
        "wireless/grenvis2.sklt",
        "wireless/rockvis2.sklt",
        "wireless/missvis2.sklt",
        "wireless/bombvis2.sklt",
        "wireless/triangle.sklt"
    };

    sklt_wis *wis = &yw->wis_skeletons;

    memset(wis, 0, sizeof(sklt_wis));

    wis->field_0 = 1;

    set_prefix_replacement("rsrc", "data:");

    for (int i = 0; i < 14; i++)
    {
        wis->sklts[i] = (NC_STACK_sklt *)init_get_class("sklt.class", 0x80001000, off_51825C[i], 0);

        if ( wis->sklts[i] )
            call_vtbl(wis->sklts[i], 3, 0x80002000, &wis->sklts_intern[i], 0);
    }

    if ( yw->screen_width >= 512 )
    {
        wis->field_9E = 8;
        wis->field_96 = 28;
        wis->field_92 = (float)yw->tiles[0]->font_height / (float)yw->screen_width;
        wis->field_8E = wis->field_92 * 14.0;
        wis->field_9A = 28;
    }
    else
    {
        wis->field_9E = 6;
        wis->field_96 = 18;
        wis->field_92 = (float)yw->tiles[0]->font_height * 1.5 / (float)yw->screen_width;
        wis->field_8E = wis->field_92 * 18.0;
        wis->field_9A = 18;
    }

    wis->field_72 = 0;
    wis->field_8A = (float)(yw->tiles[51]->chars[1].width * wis->field_9E + wis->field_9A + wis->field_96) / (float)yw->screen_width;

    if ( dword_514EFC )
        wis->field_72 = 1;
}

void sub_4DA874(_NC_STACK_ypaworld *yw)
{
    sub_4E866C(&exit_menu);
}

void sub_46E16C(_NC_STACK_ypaworld *yw)
{
    sub_4E866C(&info_log.window);
}

void sub_4C706C(_NC_STACK_ypaworld *yw)
{
    sub_4E866C(&squadron_manager.lstvw);
}

int sb_0x451034(NC_STACK_ypaworld *obj, _NC_STACK_ypaworld *yw)
{
    if ( yw->field_162c )
        return yw->field_162c;

    yw->field_17b0 = 0;
    yw->field_17b4 = NULL;
    yw->field_17bc = 0;
    yw->field_1b1c = 0;
    yw->field_1aa0 = 0;
    yw->field_1aa4 = 0;
    yw->field_1aa8 = 0;
    yw->field_1aac = 0;

    sb_0x451034__sub1(yw);
    sb_0x451034__sub9(yw);
    sb_0x451034__sub8(yw);
    sb_0x451034__sub2(obj, yw);
    create_squad_man(obj, yw);
    create_info_log(obj, yw);
    create_exit_menu(yw);
    sb_0x451034__sub5(yw);
    sb_0x451034__sub6(yw);

    call_method(obj, 139, &robo_map);
    call_method(obj, 139, &squadron_manager.lstvw);
    call_method(obj, 139, &info_log.window);
    call_method(obj, 139, &exit_menu);
    call_method(obj, 139, &lstvw2);
    call_method(obj, 139, &netgame_wnd);

    if ( !sb_0x451034__sub3(obj, yw) )
    {
        call_method(obj, 140, &lstvw2);
        call_method(obj, 140, &exit_menu);
        call_method(obj, 140, &info_log.window);
        call_method(obj, 140, &squadron_manager.lstvw);
        call_method(obj, 140, &robo_map);
        call_method(obj, 140, &netgame_wnd);

        sub_4DA874(yw);
        sub_46E16C(yw);
        sub_4C706C(yw);
        return 0;
    }

    //call_vtbl(yw->tracyrmp_ilbm, 3, 0x80002000, &a4, 0);
    //call_vtbl(yw->shadermp_ilbm, 3, 0x80002000, &v8, 0);
    yw->field_162c = 1;

    if ( yw->snd__cdsound & 1 )
    {
        CDAUDIO_t v6;

        v6.command = 7;
        v6.track_id = yw->field_2d90->amb_track_p0;
        v6.field_8 = yw->field_2d90->amb_track_p1;
        v6.field_C = yw->field_2d90->amb_track_p2;

        sub_4444D4(&v6);

        v6.command = 2;
        sub_4444D4(&v6);
    }

    return 1;
}


void sb_0x4d7c08__sub0__sub1()
{
    if ( !(bzda.cmd_flag & 0x20) )
    {
        w3d_a209 v0;
        v0 = bzda.cmdstrm;

        sub_423288(&v0);

        if ( !(gui_lstvw.cmd_flag & 0x20) )
        {
            v0 = gui_lstvw.cmdstrm;

            sub_423288(&v0);
        }
    }
}

void sb_0x4d7c08__sub0__sub3(_NC_STACK_ypaworld *yw)
{
    w3d_a209 v4 = up_panel.cmdstrm;
    call_method(yw->win3d, 209, &v4);
}

void sb_0x4d7c08__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_1b84->field_24 != 4 )
    {
        sb_0x4d7c08__sub0__sub4(yw);

        listbase *lstnode = (listbase *)yw->field_17a0.tailpred;
        while (lstnode->prev)
        {
            int v6 = 0;
            if ( !(lstnode->cmd_flag & 0x20) )
            {
                w3d_a209 v8;

                if ( lstnode->cmd_flag & 1 )
                {
                    v8.includ = NULL;
                    v8.cmdbuf = lstnode->field_1BC;
                }
                else
                {
                    v8 = lstnode->cmdstrm;

                    if ( lstnode->field_1C8 )
                        v6 = 1;
                }

                sub_423288(&v8);

                if ( v6 )
                    lstnode->field_1C8(yw);

            }

            lstnode = (listbase *)lstnode->prev;
        }

        if ( yw->field_1b84->field_3D5 != 2 )
        {
            sb_0x4d7c08__sub0__sub0(yw);
            sb_0x4d7c08__sub0__sub2(yw);
        }
        draw_tooltip(yw);
        sb_0x4d7c08__sub0__sub3(yw);
        sb_0x4d7c08__sub0__sub1();
    }
}


char * buy_list_update_sub(_NC_STACK_ypaworld *yw, int a2, listview *lstvw, char *cur, char a5, const char *a6, int a7)
{
    int v33 = lstvw->width - 2 * yw->font_default_w__b;

    listview_t1 v24[3];
    memset(v24, 0, sizeof(v24));

    char *pcur = cur;

    int v14;
    int v15;
    int v16;
    int v17;

    if ( a2 )
    {
        fntcmd_set_txtColor(&pcur, yw->iniColors[62].r, yw->iniColors[62].g, yw->iniColors[62].b);

        v14 = 9;
        v15 = 98;
        v16 = 99;
        v17 = 100;
    }
    else
    {
        fntcmd_set_txtColor(&pcur, yw->iniColors[61].r, yw->iniColors[61].g, yw->iniColors[61].b);

        v14 = 0;
        v16 = 102;
        v15 = 102;
        v17 = 102;
    }

    char v28[8];
    v28[0] = a5;
    v28[1] = 0;

    int v19 = yw->tiles[v14]->chars[48].width;

    char a1a[32];
    sprintf(a1a, "%d", a7);

    v24[0].txt = v28;
    v24[0].tileset_id = 28;
    v24[0].field_width = squadron_manager.field_2CC;
    v24[0].left_tile = 0;
    v24[0].right_tile = 0;
    v24[0].bkg_tile = 64;
    v24[0].flags = 4;

    v24[1].tileset_id = v14;
    v24[1].bkg_tile = v16;
    v24[1].left_tile = v15;
    v24[1].txt = a6;
    v24[1].flags = 37;
    v24[1].field_width = v33 - squadron_manager.field_2CC - 5 * v19;
    v24[1].right_tile = 0;

    v24[2].txt = a1a;
    v24[2].tileset_id = v14;
    v24[2].bkg_tile = v16;
    v24[2].right_tile = v17;
    v24[2].field_width = 5 * v19;
    v24[2].flags = 42;
    v24[2].left_tile = 0;

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_store_u8(&pcur, 123);

    fntcmd_select_tileset(&pcur, v14);

    fntcmd_op10(&pcur, squadron_manager.field_2CC);

    fntcmd_store_u8(&pcur, v16);

    fntcmd_add_xpos(&pcur, -squadron_manager.field_2CC);

    pcur = lstvw_txt_line(yw, pcur, 3, v24);

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    return pcur;
}

float sub_4498F4(_NC_STACK_ypaworld *yw)
{
    float v3 = 1.0;

    if ( yw->field_757E )
    {
        if ( yw->unit_limit_type_1 == 2 )
        {
            int v1 = yw->field_1bac[yw->field_1b80->owner] - yw->unit_limit_1;
            if ( v1 > 0 )
                v3 = (yw->unit_limit_arg_1 * v1) * 0.01 + 1.0;
        }
    }

    return v3;
}

char * gui_update_create_btn__sub0(_NC_STACK_ypaworld *yw)
{
    char *pcur = gui_lstvw.data_cmdbuf;

    sub_412D9C(&gui_lstvw.frm_1);
    sub_412D48(&gui_lstvw.frm_1, 0);

    gui_lstvw.elements_for_scroll_size = bzda.field_8E8;
    gui_lstvw.field_1DE = bzda.field_8F8;

    if ( gui_lstvw.element_count_max >= bzda.field_8E8 )
        gui_lstvw.element_count = bzda.field_8E8;
    else
        gui_lstvw.element_count = gui_lstvw.element_count_max;

    if ( gui_lstvw.scroll_pos + gui_lstvw.element_count > gui_lstvw.elements_for_scroll_size )
        gui_lstvw.scroll_pos = gui_lstvw.elements_for_scroll_size - gui_lstvw.element_count;

    lstvw_updlimits(yw, &gui_lstvw, -2, -2);

    gui_lstvw.frm_1.btn_xpos = bzda.field_904 + bzda.frm_1.btn_xpos;
    gui_lstvw.frm_1.btn_ypos = bzda.field_918 + bzda.frm_1.btn_ypos - gui_lstvw.frm_1.btn_height;

    pcur = lstvw_up_border(yw, &gui_lstvw, pcur, 0, "uvw");

    for (int i = 0; i < gui_lstvw.element_count; i++ )
    {
        int v21 = 0;
        int v3 = i + gui_lstvw.scroll_pos;

        if ( bzda.field_4DC[ v3 ].i == 1 )
        {
            int v5 = bzda.field_4DC[ v3 ].d;

            VhclProto *v6 = &yw->VhclProtos[v5];

            int v17 = dround(sub_4498F4(yw) * 2 * v6->energy / 100.0);

            const char *v8 = get_lang_string(yw->string_pointers_p2, v5 + 1200, v6->name);

            if ( v3 == gui_lstvw.field_1DE )
                v21 = 1;

            pcur = buy_list_update_sub(yw, v21, &gui_lstvw, pcur, v6->type_icon, v8, v17);
        }
        else if ( bzda.field_4DC[ v3 ].i == 2 )
        {
            int v9 = bzda.field_4DC[ v3 ].d;
            BuildProto *v10 = &yw->BuildProtos[v9];

            int v18 = dround(sub_4498F4(yw) * v10->energy / 100.0);
            int v12;

            if ( yw->field_757E )
                v12 = v9 + 1700;
            else
                v12 = v9 + 1500;

            const char *v13 = get_lang_string(yw->string_pointers_p2, v12, v10->name);

            if ( v3 == gui_lstvw.field_1DE )
                v21 = 1;

            pcur = buy_list_update_sub(yw, v21, &gui_lstvw, pcur, v10->type_icon, v13, v18);
        }
    }

    pcur = lstvw_down_border(yw, &gui_lstvw, pcur, 0, "xyz");

    fntcmd_set_end(&pcur);

    return pcur;
}


char * gui_update_create_btn(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->field_1b84->field_3D5 == 2 )
    {
        memset(&create_btn, 0, sizeof(create_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 67);

        if ( !(gui_lstvw.cmd_flag & 0x20) )
        {
            gui_lstvw.cmd_flag |= 0x20;
            sub_412D9C(&gui_lstvw.frm_1);
            yw->field_17bc = 0;
        }
    }
    else
    {
        bzda.frm_1.field_18[0]->xpos = bzda.field_904;
        bzda.frm_1.field_18[0]->ypos = bzda.field_918;
        bzda.frm_1.field_18[0]->width = yw->icon_order__w;
        bzda.frm_1.field_18[0]->fnt_height = yw->icon_order__h;

        if ( !(bzda.field_1CC & 0x16) )
        {
            memset(&create_btn, 0, sizeof(create_btn));

            fntcmd_select_tileset(&pcur, 23);
            fntcmd_store_u8(&pcur, 67);
        }
        else
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                if ( !(gui_lstvw.cmd_flag & 0x20) )
                    gui_update_create_btn__sub0(yw);

                fntcmd_select_tileset(&pcur, 22);
            }
            else
            {
                fntcmd_select_tileset(&pcur, 21);
            }

            int v9 = bzda.field_1D0 & 0x16;

            if (v9 == 0x10)
                fntcmd_store_u8(&pcur, 70);
            else if (v9 == 4)
                fntcmd_store_u8(&pcur, 68);
            else if (v9 == 2)
                fntcmd_store_u8(&pcur, 67);
            else
                fntcmd_store_u8(&pcur, 67);
        }
    }
    return pcur;
}

char *gui_update_map_squad_btn(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->field_1b84->field_3D5 == 2 )
    {
        fntcmd_select_tileset(&pcur, 23);

        fntcmd_store_u8(&pcur, 72);
        fntcmd_store_u8(&pcur, 73);

        memset(&map_btn, 0, sizeof(map_btn));
        memset(&squad_btn, 0, sizeof(squad_btn));

        if ( !(robo_map.cmd_flag & 0x20) )
        {
            robo_map.cmd_flag |= 0x20;

            sub_412D9C(&robo_map.frm_1);

            yw->field_17bc = 0;
        }

        if ( !(squadron_manager.lstvw.cmd_flag & 0x20) )
        {
            squadron_manager.lstvw.cmd_flag |= 0x20;

            sub_412D9C(&squadron_manager.lstvw.frm_1);

            yw->field_17bc = 0;
        }

        if ( !(info_log.window.cmd_flag & 0x20) )
        {
            info_log.window.cmd_flag |= 0x20;

            sub_412D9C(&info_log.window.frm_1);

            yw->field_17bc = 0;
        }
    }
    else
    {
        int v8 = bzda.field_908;

        for (int i = 2; i < 4; i++)
        {
            bzda.frm_1.field_18[i]->xpos = v8;
            bzda.frm_1.field_18[i]->ypos = bzda.field_918;
            bzda.frm_1.field_18[i]->width = yw->icon_order__w;
            bzda.frm_1.field_18[i]->fnt_height = yw->icon_order__h;

            v8 += yw->icon_order__w;
        }

        if ( robo_map.cmd_flag & 0x20 )
            fntcmd_select_tileset(&pcur, 21);
        else
            fntcmd_select_tileset(&pcur, 22);

        fntcmd_store_u8(&pcur, 72);

        if ( squadron_manager.lstvw.cmd_flag & 0x20 )
            fntcmd_select_tileset(&pcur, 21);
        else
            fntcmd_select_tileset(&pcur, 22);

        fntcmd_store_u8(&pcur, 73);
    }
    return pcur;
}

char *gui_update_player_panel(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int v35 = 0;

    if ( yw->field_1b80 == yw->field_1b84 )
    {
        if ( yw->field_2414 > 0 )
            v35 = 0x80;

        if ( bzda.field_8DC )
            v35 |= 0x40;
    }
    else if ( yw->field_1b84->field_3D5 == 2 )
    {
        v35 = 0x90;
    }
    else if ( yw->field_1b70 )
    {
        v35 = 0x90;

        if ( bzda.field_8DC > 1 )
            v35 = 0xD0;
    }
    else if ( yw->field_1b84->field_32 == yw->field_1b84->parent_bacto )
    {
        v35 = 0x10;

        if ( yw->field_1b84->field_24 == 9 )
        {
            if ( yw->field_2414 > 0 )
                v35 = 0x90;
        }
        else if ( yw->field_2414 > 1 )
            v35 = 0x90;

        if ( &yw->field_1b84->list2 != (nlist *)yw->field_1b84->list2.tailpred )
            v35 |= 0x40;
    }
    else
    {
        v35 = 0x70;

        if ( yw->field_1b84->field_24 == 9 )
        {
            if ( yw->field_2414 > 0 )
                v35 = 0xF0;
        }
        else if ( yw->field_2414 > 1 )
            v35 = 0xF0;
    }

    if ( yw->field_1b80->field_3D5 == 2 )
        v35 = 0;

    if ( bzda.field_1CC & 8 )
    {
        into_vhcl_btn.xpos = bzda.field_90C;
        into_vhcl_btn.ypos = bzda.field_918;
        into_vhcl_btn.width = yw->icon_order__w;
        into_vhcl_btn.fnt_height = yw->icon_order__h;

        if ( bzda.field_1D0 & 8 )
            fntcmd_select_tileset(&pcur, 22);
        else
            fntcmd_select_tileset(&pcur, 21);

        fntcmd_store_u8(&pcur, 69);
    }
    else
    {
        memset(&into_vhcl_btn, 0, sizeof(into_vhcl_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 69);
    }


    if ( v35 & 0x10 )
    {
        to_host_btn.xpos = yw->icon_order__w + bzda.field_90C;
        to_host_btn.width = yw->icon_order__w;
        to_host_btn.ypos = bzda.field_918;
        to_host_btn.fnt_height = yw->icon_order__h;

        if ( bzda.field_91C & 0x10 )
            fntcmd_select_tileset(&pcur, 22);
        else
            fntcmd_select_tileset(&pcur, 21);

        fntcmd_store_u8(&pcur, 81);
    }
    else
    {
        memset(&to_host_btn, 0, sizeof(to_host_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 81);
    }

    if ( v35 & 0x20 )
    {
        to_leader_btn.xpos = bzda.field_90C + 2 * yw->icon_order__w;
        to_leader_btn.ypos = bzda.field_918;
        to_leader_btn.width = yw->icon_order__w;
        to_leader_btn.fnt_height = yw->icon_order__h;

        if ( bzda.field_91C & 0x20 )
            fntcmd_select_tileset(&pcur, 22);
        else
            fntcmd_select_tileset(&pcur, 21);

        fntcmd_store_u8(&pcur, 82);
    }
    else
    {
        memset(&to_leader_btn, 0, sizeof(to_leader_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 82);
    }

    if ( v35 & 0x40 )
    {
        if ( !bzda.field_8DC || (yw->field_1b80 != yw->field_1b84 && !yw->field_1b70) )
        {
            turrets_btn.xpos = bzda.field_90C + 3 * yw->icon_order__w;
            turrets_btn.ypos = bzda.field_918;
            turrets_btn.fnt_height = yw->icon_order__h;
            turrets_btn.width = yw->icon_order__w;

            if ( bzda.field_91C & 0x40 )
                fntcmd_select_tileset(&pcur, 22);
            else
                fntcmd_select_tileset(&pcur, 21);

            fntcmd_store_u8(&pcur, 83);
        }
        else
        {
            turrets_btn.width = yw->icon_order__w;
            turrets_btn.xpos = bzda.field_90C + 3 * turrets_btn.width;
            turrets_btn.ypos = bzda.field_918;
            turrets_btn.fnt_height = yw->icon_order__h;

            if ( bzda.field_91C & 0x40 )
                fntcmd_select_tileset(&pcur, 22);
            else
                fntcmd_select_tileset(&pcur, 21);

            fntcmd_store_u8(&pcur, 66);
        }
    }
    else
    {
        memset(&turrets_btn, 0, sizeof(turrets_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 83);
    }

    if ( v35 & 0x80 )
    {
        next_squad_btn.xpos = 4 * yw->icon_order__w + bzda.field_90C;
        next_squad_btn.ypos = bzda.field_918;
        next_squad_btn.width = yw->icon_order__w;
        next_squad_btn.fnt_height = yw->icon_order__h;

        if ( bzda.field_91C & 0x80 )
            fntcmd_select_tileset(&pcur, 22);
        else
            fntcmd_select_tileset(&pcur, 21);

        fntcmd_store_u8(&pcur, 84);
    }
    else
    {
        memset(&next_squad_btn, 0, sizeof(next_squad_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 84);
    }
    return pcur;
}

char *gui_update_tools(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->field_1b84->field_3D5 == 2 )
    {
        memset(&analyzer_btn, 0, sizeof(analyzer_btn));

        fntcmd_select_tileset(&pcur, 23);
        fntcmd_store_u8(&pcur, 63);
    }
    else
    {
        analyzer_btn.xpos = bzda.field_910;
        analyzer_btn.ypos = bzda.field_918;
        analyzer_btn.width = yw->icon_order__w;
        analyzer_btn.fnt_height = yw->icon_order__h;

        if ( bzda.field_91C & 0x100 )
            fntcmd_select_tileset(&pcur, 22);
        else
            fntcmd_select_tileset(&pcur, 21);

        fntcmd_store_u8(&pcur, 63);
    }

    help_btn.xpos = yw->icon_order__w + LOWORD(bzda.field_910);
    help_btn.ypos = bzda.field_918;
    help_btn.fnt_height = yw->icon_order__h;
    help_btn.width = yw->icon_order__w;

    if ( bzda.field_91C & 0x200 )
        fntcmd_select_tileset(&pcur, 22);
    else
        fntcmd_select_tileset(&pcur, 21);

    fntcmd_store_u8(&pcur, 76);

    menu_btn.xpos = bzda.field_910 + 2 * LOWORD(yw->icon_order__w);
    menu_btn.ypos = bzda.field_918;
    menu_btn.width = yw->icon_order__w;
    menu_btn.fnt_height = yw->icon_order__h;

    if ( bzda.field_91C & 0x400 )
        fntcmd_select_tileset(&pcur, 22);
    else
        fntcmd_select_tileset(&pcur, 21);

    fntcmd_store_u8(&pcur, 85);

    return pcur;
}

char * sub_449970(_NC_STACK_ypaworld *yw, char *cur, int a4, int a3, const char *a5, int a6, int a7)
{
    char *pcur = cur;

    int v10, v22;

    if ( yw->screen_width >= 512 )
    {
        v22 = a7 - 72;
        v10 = a4 + a7 - 72 + 16;
    }
    else
    {
        v22 = a7 - 56;
        v10 = a4 + a7 - 56 + 8;
    }

    fntcmd_select_tileset(&pcur, 15);

    fntcmd_set_xpos(&pcur, a4);
    fntcmd_set_ypos(&pcur, a3);

    int v14 = yw->font_default_h - yw->tiles[51]->font_height;

    listview_t1 arg;
    arg.txt = a5;
    arg.field_width = v22;
    arg.tileset_id = 15;
    arg.left_tile = 0;
    arg.bkg_tile = 32;
    arg.right_tile = 0;
    arg.flags = 40;

    pcur = lstvw_txt_line_textual(yw, pcur, &arg);

    fntcmd_select_tileset(&pcur, 51);

    fntcmd_set_xpos(&pcur, v10);
    fntcmd_set_ypos(&pcur, (v14 / 2) + a3);

    for (int i = 0; i < a6; i++)
        fntcmd_store_u8(&pcur, yw->field_1b80->owner);

    return pcur;
}

char *ypaworld_func64__sub7__sub2__sub1__sub0(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( bzda.field_1D0 & 6 )
    {
        if ( bzda.field_8EC != -1 && !yw->field_1b70 )
        {
            VhclProto *v4 = &yw->VhclProtos[ bzda.field_2DC[ bzda.field_8EC ] ];
            int a6 = v4->job_fighttank / 2;
            int v26 = v4->job_fightflyer / 2;
            int v25 = v4->job_fighthelicopter / 2;
            int v28 = v4->job_conquer / 2;
            int v5 = v4->job_fightrobo / 2;
            int v29 = v4->job_reconnoitre / 2;

            if ( !a6 )
                a6 = 1;

            if ( !v5 )
                v5 = 1;

            if ( !v26 )
                v26 = 1;

            if ( !v25 )
                v25 = 1;

            if ( !v28 )
                v28 = 1;

            if ( !v29 )
                v29 = 1;

            int v29_4 = 4 * yw->screen_width / 7;
            int v6 = yw->screen_width - v29_4;

            int v30 = -(yw->icon0___h + 7 * yw->font_default_h);

            fntcmd_set_txtColor(&pcur, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2474, "2474 == VS ROBO:"), v5, v6);

            v30 += yw->font_default_h;

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2475, "2475 == VS TANK:"), a6, v6);

            v30 += yw->font_default_h;

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2476, "2476 == VS PLANE:"), v26, v6);

            v30 += yw->font_default_h;

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2477, "2477 == VS HELI:"), v25, v6);

            v30 += yw->font_default_h;

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2479, "2479 == CAPTURE:"), v28, v6);

            v30 += yw->font_default_h;

            pcur = sub_449970(yw, pcur, v29_4, v30,  get_lang_string(yw->string_pointers_p2, 2478, "2478 == RECON:"), v29, v6);
        }
    }
    return pcur;
}


void ypaworld_func64__sub7__sub2__sub1(_NC_STACK_ypaworld *yw)
{
    sub_4C3A54(yw);

    char *pcur = byte_516534;

    if ( bzda.field_1D4 & 1 )
    {
        int v20 = bzda.frm_1.btn_xpos - (yw->screen_width / 2);
        int v21 = bzda.frm_1.btn_ypos - (yw->screen_height / 2);

        if ( !(bzda.field_1D8 & 1) )
        {
            bzda.field_1D8 |= 1;
            sub_412D48(&bzda.frm_1, 0);
        }

        sub_412D9C(&bzda.frm_1);
        sub_412D48(&bzda.frm_1, 0);

        fntcmd_select_tileset(&pcur, 21);
        fntcmd_set_center_xpos(&pcur, v20);
        fntcmd_set_center_ypos(&pcur, v21);


        if ( bzda.field_904 > 0 )
            fntcmd_add_xpos(&pcur, bzda.field_904);

        pcur = gui_update_create_btn(yw, pcur);

        if ( bzda.field_8FC > 0 )
            fntcmd_add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_map_squad_btn(yw, pcur);

        if ( bzda.field_8FC > 0 )
            fntcmd_add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_player_panel(yw, pcur);

        if ( bzda.field_8FC > 0 )
            fntcmd_add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_tools(yw, pcur);

        if ( !(gui_lstvw.cmd_flag & 0x20) )
            pcur = ypaworld_func64__sub7__sub2__sub1__sub0(yw, pcur);
    }

    fntcmd_set_end(&pcur);

    if ( !(gui_lstvw.cmd_flag & 0x20) )
        lstvw_update(yw, &gui_lstvw);
}


NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub6(_NC_STACK_ypaworld *yw)
{
    if ( !yw->field_241c )
        return NULL;

    if ( yw->field_241c == yw->field_1b80->ypabact__id )
        return yw->field_1b80->self;

    bact_node *v4 = (bact_node *)yw->field_1b80->list2.head;
    while( v4->next )
    {
        if ( v4->bact->field_3D5 != 2 && v4->bact->field_3D5 != 4 && v4->bact->field_3D5 != 5 )
        {
            if ( yw->field_241c == v4->bact->ypabact__id )
                return v4->bact->self;


            bact_node *v7 = (bact_node *)v4->bact->list2.head;
            while( v7->next )
            {
                if ( v7->bact->field_3D5 != 2 && v7->bact->field_3D5 != 4 && v7->bact->field_3D5 != 5 && yw->field_241c == v7->bact->ypabact__id )
                    return v7->bact->self;

                v7 = (bact_node *)v7->next;
            }
        }
        v4 = (bact_node *)v4->next;
    }

    return NULL;
}

void ypaworld_func64__sub7__sub2__sub7(_NC_STACK_ypaworld *yw)
{
    __NC_STACK_ypabact *v3 = yw->field_1b84->parent_bact;

    if ( !v3 || v3 != yw->field_1b80 )
    {
        if ( yw->field_1b84->field_24 != 3 && yw->field_1b84->field_24 != 4 && yw->field_1b84->field_24 != 9 )
        {
            bact_arg109 v5;
            v5.field_0 = 2;
            v5.field_4 = yw->field_1b84->parent_bact;

            call_method(yw->field_1b7c, 109, &v5);
        }
    }
}

int ypaworld_func64__sub7__sub2__sub3(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    winp_131arg *winpt = &inpt->winp131arg;

    switch ( inpt->dword8 - 0x80 )
    {
    case 0:
        if ( bzda.field_1CC & 1 )
            bzda.field_1D0 = 1;
        break;

    case 1:
        if ( yw->field_1b84 == yw->field_1b80 )
        {
            if ( turrets_btn.width )
            {
                winpt->selected_btnID = 6;
                winpt->selected_btn = &bzda.frm_1;
                winpt->flag |= 0x40;
            }
        }
        break;

    case 2:
        if ( bzda.field_1CC & 2 )
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                bzda.field_1D0 = bzda.field_1CC & 1;
            }
            else
            {
                if ( !(bzda.field_1D0 & 2) || gui_lstvw.cmd_flag & 0x20 )
                {
                    sub_4C31EC(yw, &gui_lstvw);
                    gui_lstvw.field_1D0 |= 0x280;
                }
                else
                {
                    sub_4C31C0(yw, &gui_lstvw);
                }

                if ( bzda.field_8E0 <= bzda.field_8F8 )
                    bzda.field_1D0 = 16;
                else
                    bzda.field_1D0 = 2;

                return 0;
            }
        }
        break;

    case 3:
        if ( bzda.field_1CC & 4 )
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                bzda.field_1D0 = bzda.field_1CC & 1;
            }
            else
            {
                if ( !(bzda.field_1D0 & 4) || gui_lstvw.cmd_flag & 0x20 )
                {
                    sub_4C31EC(yw, &gui_lstvw);
                    gui_lstvw.field_1D0 |= 0x280;
                }
                else
                {
                    sub_4C31C0(yw, &gui_lstvw);
                }

                bzda.field_1D0 = 4;
                return 0;
            }
        }
        break;

    case 4:
        if ( bzda.field_1CC & 8 )
        {
            winpt->selected_btn = &bzda.frm_1;
            winpt->selected_btnID = 1;
            winpt->flag |= 0x10;
        }
        break;

    case 7:
        if ( bzda.field_1CC & 0x20 )
        {
            if ( bzda.field_1D0 & 0x20 )
                bzda.field_1D0 = bzda.field_1CC & 1;
            else
                bzda.field_1D0 = 32;
        }
        break;

    case 8:
        if ( yw->field_1b84->field_3D5 != 2 )
        {
            winpt->selected_btnID = 2;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x10;
        }
        break;

    case 9:
        if ( yw->field_1b84->field_3D5 != 2 )
        {
            winpt->selected_btnID = 3;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x10;
        }
        break;

    case 20:
        if ( next_squad_btn.width )
        {
            winpt->selected_btnID = 7;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x40;
        }
        break;

    case 21:
        if ( to_host_btn.width )
        {
            winpt->selected_btnID = 4;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x40;
        }
        break;

    case 22:
        if ( turrets_btn.width )
        {
            winpt->selected_btnID = 6;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x40;
        }
        break;

    case 23:
        if ( to_leader_btn.width )
        {
            winpt->selected_btnID = 5;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x40;
        }
        break;

    case 24:
        if ( yw->field_1b80->field_3D5 == 2 || bzda.field_1D0 & 1 )
        {
            if ( yw->field_1b78 == yw->field_1b7c || yw->field_1b80->field_3D5 == 2 )
            {
                if ( menu_btn.width )
                {
                    winpt->selected_btnID = 10;
                    winpt->selected_btn = &bzda.frm_1;
                    winpt->flag |= 0x40;
                }
            }
            else
            {
                winpt->selected_btnID = 4;
                winpt->selected_btn = &bzda.frm_1;
                winpt->flag |= 0x40;
                inpt->dword8 = 0;
            }
        }
        else
        {
            bzda.field_1D0 = 1;
        }
        break;

    case 25:
        if ( yw->field_1b7c != yw->field_1b78 )
        {
            if ( yw->wis_skeletons.field_0 )
                yw->wis_skeletons.field_0 = 0;
            else
                yw->wis_skeletons.field_0 = 1;
        }
        break;

    case 27:
        if ( info_log.window.cmd_flag & 0x20 )
        {
            info_log.window.cmd_flag &= 0xFFFFFFDF;
            sub_412D48(&info_log.window.frm_1, 0);
            yw->field_17bc = 0;

            Remove(&info_log.window);
            AddHead(&yw->field_17a0, &info_log.window);
        }
        else if ( !(info_log.window.cmd_flag & 0x20) )
        {
            info_log.window.cmd_flag |= 0x20;
            sub_412D9C(&info_log.window.frm_1);
            yw->field_17bc = 0;
        }
        break;

    case 28:
        if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16 )
        {
            bzda.field_8F8++;
            if ( bzda.field_8F8 >= bzda.field_8E8 )
                bzda.field_8F8 = 0;

            if ( bzda.field_8F8 >= bzda.field_8E0 )
            {
                bzda.field_8F4 = bzda.field_8F8 - bzda.field_8E0;
                bzda.field_1D0 = 16;
                bzda.field_8EC = -1;
            }
            else
            {
                bzda.field_8EC = bzda.field_8F8;
                bzda.field_8F4 = -1;

                if ( !(bzda.field_1D0 & 4) )
                    bzda.field_1D0 = 2;
            }
        }
        return 0;

    case 29:
        if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16 )
        {
            bzda.field_8F8--;

            if ( bzda.field_8F8 < 0 )
                bzda.field_8F8 = bzda.field_8E8 - 1;

            if ( bzda.field_8F8 >= bzda.field_8E0 )
            {
                bzda.field_8F4 = bzda.field_8F8 - bzda.field_8E0;
                bzda.field_8EC = -1;
                bzda.field_1D0 = 16;
            }
            else
            {
                bzda.field_8EC = bzda.field_8F8;
                bzda.field_8F4 = -1;

                if ( !(bzda.field_1D0 & 4) )
                    bzda.field_1D0 = 2;
            }
        }
        return 0;

    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
        if ( yw->field_2410 != -1 )
        {
            int v16;

            switch ( inpt->dword8 )
            {
            case (0x80 | 0x26):
                v16 = 0;
                break;

            case (0x80 | 0x27):
                v16 = 25;
                break;

            case (0x80 | 0x28):
                v16 = 50;
                break;

            case (0x80 | 0x29):
                v16 = 75;
                break;

            case (0x80 | 0x2A):
                v16 = 100;
                break;
            }

            call_vtbl(yw->field_1c0c[ yw->field_2410 ]->self, 2, 0x8000100D, v16, 0);

            if ( yw->GameShell )
                sub_423F74(&yw->GameShell->samples1_info, 3);
        }
        break;

    case 43:
        sub_449DE8(yw, get_lang_string(yw->string_pointers_p2, 767, "help\\l17.html")); //MAKE ME
        break;

    case 46:
        if ( yw->field_1b84->field_3D5 != 2 )
        {
            winpt->selected_btnID = 8;
            winpt->selected_btn = &bzda.frm_1;
            winpt->flag |= 0x40;
        }
        break;

    default:
        break;
    }

    return 1;
}

NC_STACK_ypabact * sb_0x4c63d0__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_2410 == -1 )
    {
        if ( yw->field_2414 <= 0 )
            return yw->field_1b78;
        else
            return yw->field_1c0c[0]->self;
    }
    else
        return yw->field_1c0c[ yw->field_2410 ]->self;
}

NC_STACK_ypabact * sb_0x4c63d0__sub1(_NC_STACK_ypaworld *yw, int a2)
{
    for (int i = 0; i < yw->field_2414; i++)
    {
        if ( a2 == yw->field_1c0c[i]->field_2E )
        {
            if ( i + 1 < yw->field_2414 )
                return yw->field_1c0c[ i + 1 ]->self;

            return yw->field_1c0c[ 0 ]->self;
        }
    }

    if ( yw->field_2410 != -1 )
        return yw->field_1c0c[ yw->field_2410 ]->self;

    if ( yw->field_2414 > 0 )
        return yw->field_1c0c[0]->self;

    return yw->field_1b78;
}

NC_STACK_ypabact * sb_0x4c63d0(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_1b7c == yw->field_1b78 || yw->field_1b70 )
    {
        if ( yw->field_2410 == -1 )
        {
            if ( yw->field_2414 <= 0 )
                return yw->field_1b78;
            else
                return yw->field_1c0c[0]->self;
        }
        else
        {
            return yw->field_1c0c[ yw->field_2410 ]->self;
        }
    }
    else
    {
        if ( yw->field_1b84->field_3D5 == 2 )
        {
            for (int i = 0; i < yw->field_2414; i++)
            {
                if ( yw->field_1a10 == yw->field_1c0c[i]->field_2E )
                    return yw->field_1c0c[i]->self;
            }

            return sb_0x4c63d0__sub0(yw);
        }
        else if ( yw->field_1b84->field_24 == 9 )
        {
            if ( yw->field_2410 == -1 )
            {
                if ( yw->field_2414 <= 0 )
                    return yw->field_1b78;
                else
                    return yw->field_1c0c[0]->self;
            }
            else
            {
                return yw->field_1c0c[ yw->field_2410 ]->self;
            }
        }
        else
        {
            return sb_0x4c63d0__sub1(yw, yw->field_1a10);
        }
    }
    return NULL;
}

void ypaworld_func64__sub7__sub2__sub2(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_1b84->field_3D5 != 2 )
    {
        if ( yw->field_1b84 == yw->field_1b80 )
        {
            yw->robo_map_status.p1 = 1;
            yw->robo_map_status.p2 = (robo_map.cmd_flag & 0x20) == 0;
            yw->robo_map_status.p3 = robo_map.frm_1.btn_xpos;
            yw->robo_map_status.p4 = robo_map.frm_1.btn_ypos;
            yw->robo_map_status.p5 = robo_map.frm_1.btn_width;
            yw->robo_map_status.p6 = robo_map.frm_1.btn_height;

            yw->robo_map_status.pX[0] = robo_map.field_1EC;
            yw->robo_map_status.pX[1] = robo_map.field_1ED;
            yw->robo_map_status.pX[2] = robo_map.field_1EE;
            yw->robo_map_status.pX[3] = robo_map.field_208;
            yw->robo_map_status.pX[4] = robo_map.field_20A;
            yw->robo_map_status.pX[5] = robo_map.field_20C;
            yw->robo_map_status.pX[6] = robo_map.field_20E;

            yw->robo_finder_status.p1 = 1;
            yw->robo_finder_status.p2 = (squadron_manager.lstvw.cmd_flag & 0x20) == 0;
            yw->robo_finder_status.p3 = squadron_manager.lstvw.frm_1.btn_xpos;
            yw->robo_finder_status.p4 = squadron_manager.lstvw.frm_1.btn_ypos;
            yw->robo_finder_status.p5 = squadron_manager.lstvw.frm_1.btn_width;
            yw->robo_finder_status.p6 = squadron_manager.lstvw.frm_1.btn_height;
        }
        else
        {
            yw->vhcl_map_status.p1 = 1;
            yw->vhcl_map_status.p2 = (robo_map.cmd_flag & 0x20) == 0;
            yw->vhcl_map_status.p3 = robo_map.frm_1.btn_xpos;
            yw->vhcl_map_status.p4 = robo_map.frm_1.btn_ypos;
            yw->vhcl_map_status.p5 = robo_map.frm_1.btn_width;
            yw->vhcl_map_status.p6 = robo_map.frm_1.btn_height;

            yw->vhcl_map_status.pX[0] = robo_map.field_1EC;
            yw->vhcl_map_status.pX[1] = robo_map.field_1ED;
            yw->vhcl_map_status.pX[2] = robo_map.field_1EE;
            yw->vhcl_map_status.pX[3] = robo_map.field_208;
            yw->vhcl_map_status.pX[4] = robo_map.field_20A;
            yw->vhcl_map_status.pX[5] = robo_map.field_20C;
            yw->vhcl_map_status.pX[6] = robo_map.field_20E;

            yw->vhcl_finder_status.p1 = 1;
            yw->vhcl_finder_status.p2 = (squadron_manager.lstvw.cmd_flag & 0x20) == 0;
            yw->vhcl_finder_status.p3 = squadron_manager.lstvw.frm_1.btn_xpos;
            yw->vhcl_finder_status.p4 = squadron_manager.lstvw.frm_1.btn_ypos;
            yw->vhcl_finder_status.p5 = squadron_manager.lstvw.frm_1.btn_width;
            yw->vhcl_finder_status.p6 = squadron_manager.lstvw.frm_1.btn_height;
        }
    }
}

NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub5(_NC_STACK_ypaworld *yw)
{
    if ( bzda.field_8F0 != -1 )
    {
        if ( yw->field_1b70 )
        {
            bzda.field_8F0++;

            if ( bzda.field_8F0 >= bzda.field_8DC )
            {
                bzda.field_8F0 = 0;
                yw->field_1b70 = 0;
                return yw->field_1b78;
            }

            roboGun *a4;
            call_vtbl(yw->field_1b78, 3, 0x8000200E, &a4, 0);

            yw->field_1b70 = 1;

            return a4[ bzda.field_1DC[ bzda.field_8F0 ] ].gun_obj;
        }
        else
        {
            bzda.field_8F0 = 0;

            roboGun *v5;
            call_vtbl(yw->field_1b78, 3, 0x8000200E, &v5, 0);

            yw->field_1b70 = 1;

            return v5[ bzda.field_1DC[ bzda.field_8F0 ] ].gun_obj;
        }
    }
    return NULL;
}

NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub4(_NC_STACK_ypaworld *yw)
{
    bact_node *v4;

    if ( yw->field_1b84->parent_bacto == yw->field_1b84->field_32 )
        v4 = (bact_node *)yw->field_1b84->list2.head;
    else
        v4 = (bact_node *)yw->field_1b84->list_node.next;

    while (v4->next)
    {
        if ( v4->bact->field_3D5 != 4 && v4->bact->field_3D5 != 2 && v4->bact->field_3D5 != 5 )
            return v4->bact->self;

        v4 = (bact_node *)v4->next;
    }

    if ( yw->field_1b84->parent_bacto != yw->field_1b84->field_32 )
    {
        if ( ((size_t)yw->field_1b84->parent_bacto) < 0x10 ) // DIRTY!
            return NULL;
        else
            return yw->field_1b84->parent_bacto;
    }

    return NULL;
}

int sub_4C885C()
{
    if ( dword_5BAF9C == 3 )
    {
        if ( lstvw2.cmd_flag & 0x20 )
            dword_5BAF9C = 2;
    }
    return dword_5BAF9C;
}

void ypaworld_func64__sub7__sub2__sub8(_NC_STACK_ypaworld *yw)
{
    if ( sub_4C885C() != 3 )
    {
        sub_4C31EC(yw, &exit_menu);
        Remove(&exit_menu);
        AddHead(&yw->field_17a0, &exit_menu);
    }
}

void ypaworld_func64__sub7__sub2__sub9(_NC_STACK_ypaworld *yw)
{
    if ( sub_4C885C() != 3 )
        sub_4C31C0(yw, &exit_menu);
}

void ypaworld_func64__sub7__sub2__sub10(_NC_STACK_ypaworld *yw)
{
    yw_arg159 v38;
    v38.txt = "Make analyzer!";
    v38.unit = 0;
    v38.field_4 = 100;
    v38.field_C = 0;

    call_method(yw->self_full, 159, &v38);

    dprintf("MAKE ME %s\n", "ypaworld_func64__sub7__sub2__sub10");
}

void ypaworld_func64__sub7__sub2__sub0()
{
    bzda.field_8E8 = 0;

    for (int i = 0; i < bzda.field_8E0; i++)
    {
        bzda.field_4DC[i].i = 1;
        bzda.field_4DC[i].d = bzda.field_2DC[i];
    }

    bzda.field_8E8 += bzda.field_8E0;

    if ( !(bzda.field_1D0 & 4) )
    {
        for (int i = 0; i < bzda.field_8E4; i++)
        {
            bzda.field_4DC[i + bzda.field_8E0].i = 2;
            bzda.field_4DC[i + bzda.field_8E0].d = bzda.field_3DC[i];
        }

        bzda.field_8E8 += bzda.field_8E4;
    }

    if ( bzda.field_8E8 )
    {
        if ( bzda.field_8E8 == 1 )
        {
            bzda.field_8F8 = 0;
        }
        else if ( bzda.field_8E8 <= bzda.field_8F8 )
        {
            bzda.field_8F8 = bzda.field_8E8 - 1;
        }
    }
    else
    {
        bzda.field_8F8 = -1;
    }
}

void sb_0x4c66f8__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->field_1614 - yw->field_1a04 > 35000 )
    {
        yw_arg159 arg159;
        arg159.unit = yw->field_1b80;
        arg159.field_4 = 10;
        arg159.txt = NULL;
        arg159.field_C = 39;

        call_method(yw->self_full, 159, &arg159);
    }

    yw->field_1a04 = yw->field_1614;
}

void sb_0x4c66f8(_NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact1, NC_STACK_ypabact *bact2)
{
    if ( bact1 != bact2 )
    {
        __NC_STACK_ypabact *a4;
        call_vtbl(bact1, 3, 0x80001003, &a4, 0);

        __NC_STACK_ypabact *v9;
        call_vtbl(bact2, 3, 0x80001003, &v9, 0);

        if ( a4->field_3D5 != 4 && a4->field_3D5 != 2 && a4->field_3D5 != 5 )
        {
            yw->wis_skeletons.field_76 = yw->field_1614;

            call_vtbl(bact2, 2, 0x80001004, 0, 0);
            call_vtbl(bact2, 2, 0x80001005, 0, 0);

            call_vtbl(bact1, 2, 0x80001004, 1, 0);
            call_vtbl(bact1, 2, 0x80001005, 1, 0);

            if ( !(gui_lstvw.cmd_flag & 0x20) )
            {
                gui_lstvw.cmd_flag |= 0x20;
                sub_412D9C(&gui_lstvw.frm_1);

                yw->field_17bc = 0;
            }

            if ( bzda.field_1CC & 1 )
            {
                if ( bzda.field_1D0 == 8 )
                    bzda.field_1D0 = 1;
            }

            if ( bact1 == yw->field_1b78 )
            {
                yw->field_17c0 = 0;
                sb_0x4c66f8__sub0(yw);
            }
            else
            {
                yw_arg159 arg159;
                arg159.unit = yw->field_1b84;
                arg159.field_4 = 33;
                arg159.field_C = 17;
                arg159.txt = 0;

                call_method(yw->self_full, 159, &arg159);
            }

            yw->field_17bc = 0;
        }
    }
}



void  ypaworld_func64__sub7__sub2(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    winp_131arg *winpt = &inpt->winp131arg;

    NC_STACK_ypabact *bact1 = NULL;
    NC_STACK_ypabact *bact2 = yw->current_bact->self;

    bzda.field_91C = 0;

    int a2a = 1;
    if ( bzda.field_1D4 & 1 )
    {
        switch ( inpt->dword8 )
        {
        case (0x80 | 0x1F):
        {
            __NC_STACK_ypabact *v9 = sub_449518(yw);

            if ( v9 )
                bact1 = v9->self;
        }
        break;

        case (0x80 | 0x2C):
            bact1 = ypaworld_func64__sub7__sub2__sub6(yw);
            break;

        case (0x80 | 0x2D):
            ypaworld_func64__sub7__sub2__sub7(yw);
            break;

        default:
            if ( inpt->dword8 )
                a2a = ypaworld_func64__sub7__sub2__sub3(yw, inpt);

            break;
        }

        if ( yw->field_1b84->field_3D5 == 2 )
        {
            if ( yw->field_162A )
            {
                bact1 = sb_0x4c63d0(yw);
                inpt->but_flags &= 0xFFFFFFFE;
            }
        }

        if ( !(gui_lstvw.cmd_flag & 0x20) )
        {
            if ( yw->field_826F == 27 || yw->field_826F == 13 )
            {
                if ( !(gui_lstvw.cmd_flag & 0x20) )
                {
                    gui_lstvw.cmd_flag |= 0x20;
                    sub_412D9C(&gui_lstvw.frm_1);

                    yw->field_17bc = 0;
                }

                inpt->downed_key_2 = 0;
                inpt->dword8 = 0;
                inpt->downed_key = 0;
            }
        }

        if ( yw->field_1b84 == yw->field_1b80 )
        {
            if ( robo_map.field_1E8 & 8 )
                robo_map.field_1E8 &= 0xFFFFFFF7;
        }
        else if ( !(robo_map.field_1E8 & 8) )
        {
            robo_map.field_1E8 |= 8;
        }

        ypaworld_func64__sub7__sub2__sub2(yw);

        if ( winpt->selected_btn == &bzda.frm_1 )
        {
            if ( winpt->selected_btnID != -1 )
            {
                if ( yw->GameShell )
                {
                    if ( winpt->flag & 0x10 )
                        sub_423F74(&yw->GameShell->samples1_info, 3);
                }
            }

            switch ( winpt->selected_btnID )
            {
            case 0:
                if ( !(winpt->flag & 0x10) )
                {
                    if ( winpt->flag & 0x80 )
                    {
                        if ( bzda.field_1D0 & 0x16 )
                        {
                            bzda.field_1D0 = bzda.field_1CC & 1;
                            sub_481204(yw, 3, 130);
                        }
                        else
                        {

                            if ( !(bzda.field_1D0 & 4) || gui_lstvw.cmd_flag & 0x20 )
                            {
                                sub_4C31EC(yw, &gui_lstvw);
                                gui_lstvw.field_1D0 |= 0x280;
                            }
                            else
                            {
                                sub_4C31C0(yw, &gui_lstvw);
                            }

                            a2a = 0;
                            bzda.field_1D0 = 4;
                        }
                    }
                    else
                        sub_481204(yw, 3, 130);

                }
                else if ( bzda.field_1D0 & 0x16 )
                {
                    bzda.field_1D0 = bzda.field_1CC & 1;

                    sub_481204(yw, 3, 130);
                }
                else
                {
                    if ( !(bzda.field_1D0 & 2) || gui_lstvw.cmd_flag & 0x20 )
                    {
                        sub_4C31EC(yw, &gui_lstvw);
                        gui_lstvw.field_1D0 |= 0x280;
                    }
                    else
                    {
                        sub_4C31C0(yw, &gui_lstvw);
                    }

                    if ( bzda.field_8E0 <= bzda.field_8F8 )
                        bzda.field_1D0 = 16;
                    else
                        bzda.field_1D0 = 2;

                    a2a = 0;
                    sub_481204(yw, 3, 130);
                }
                break;

            case 1: // Jump into vehicle
                if ( winpt->flag & 0x10 )
                {
                    if ( bzda.field_1D0 & 8 )
                        bzda.field_1D0 = bzda.field_1CC & 1;
                    else
                        bzda.field_1D0 = 8;
                }

                sub_481204(yw, 5, 132);
                break;

            case 2: //MAP
            case 3: //SQUAD
                if ( winpt->flag & 0x10 )
                {
                    listbase *v2;

                    if ( winpt->selected_btnID == 2 )
                        v2 = &robo_map;
                    else if ( winpt->selected_btnID == 3 )
                        v2 = &squadron_manager.lstvw;

                    if ( v2->cmd_flag & 0x20 )
                    {
                        if ( v2->cmd_flag & 0x20 )
                        {
                            v2->cmd_flag &= 0xFFFFFFDF;

                            sub_412D48(&v2->frm_1, 0);
                            yw->field_17bc = 0;
                        }

                        Remove(v2);
                        AddHead(&yw->field_17a0, v2);
                    }
                    else if ( !(v2->cmd_flag & 0x20) )
                    {
                        v2->cmd_flag |= 0x20;

                        sub_412D9C(&v2->frm_1);
                        yw->field_17bc = 0;
                    }
                }

                if ( winpt->selected_btnID == 2 )
                    sub_481204(yw, 8, 136);
                else if ( winpt->selected_btnID == 3 )
                    sub_481204(yw, 9, 137);

                break;

            case 4: //Back to host station
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x10;

                if ( winpt->flag & 0x40 )
                {
                    bact1 = yw->field_1b78;

                    if ( yw->field_1b70 )
                        bzda.field_1D0 = 1;
                }

                sub_481204(yw, 17, 149);
                break;

            case 5:
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x20;

                if ( winpt->flag & 0x40 )
                {
                    bact1 = yw->field_1b84->parent_bacto;
                    if ( (size_t)bact1 < 0x10 ) // DIRT FROM ***
                        bact1 = NULL;
                }

                sub_481204(yw, 18, 151);
                break;

            case 6: // Cycle through units in squad
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x40;

                if ( yw->field_1b80 == yw->field_1b84 || yw->field_1b70 )
                {
                    if ( winpt->flag & 0x40 )
                        bact1 = ypaworld_func64__sub7__sub2__sub5(yw);

                    sub_481204(yw, 2, 150);
                }
                else
                {
                    if ( winpt->flag & 0x40 )
                        bact1 = ypaworld_func64__sub7__sub2__sub4(yw);

                    sub_481204(yw, 19, 150);
                }
                break;

            case 7: //Cycle through squad leader
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x80;

                if ( winpt->flag & 0x40 )
                    bact1 = sb_0x4c63d0(yw);

                sub_481204(yw, 20, 148);
                break;

            case 8:
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x100;

                if ( winpt->flag & 0x40 )
                    ypaworld_func64__sub7__sub2__sub10(yw);

                sub_481204(yw, 47, 174);
                break;

            case 9:
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x200;

                if ( winpt->flag & 0x40 )
                {
                    sub_449DE8(yw, get_lang_string(yw->string_pointers_p2, 767, "help\\l17.html")); //MAKE ME
                }

                sub_481204(yw, 48, 171);
                break;

            case 10:
                if ( winpt->flag & 0x30 )
                    bzda.field_91C |= 0x400;

                if ( winpt->flag & 0x40 )
                {
                    if ( exit_menu.cmd_flag & 0x20 )
                        ypaworld_func64__sub7__sub2__sub8(yw);
                    else
                        ypaworld_func64__sub7__sub2__sub9(yw);
                }

                sub_481204(yw, 21, 152);
                break;

            default:
                break;
            }
        }

        if ( bzda.field_1D0 & 0x29 )
        {
            if ( !(gui_lstvw.cmd_flag & 0x20) )
            {
                gui_lstvw.cmd_flag |= 0x20;
                sub_412D9C(&gui_lstvw.frm_1);
                yw->field_17bc = 0;
            }
        }

        if ( a2a )
        {
            lstvw_update_input(yw, &gui_lstvw, inpt);
            if ( !(gui_lstvw.cmd_flag & 0x20) )
            {
                if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16)
                {
                    bzda.field_8F8 = gui_lstvw.field_1DE;

                    if ( gui_lstvw.field_1DE >= bzda.field_8E0 )
                    {
                        bzda.field_8F4 = gui_lstvw.field_1DE - bzda.field_8E0;
                        bzda.field_8EC = -1;
                        bzda.field_1D0 = 16;
                    }
                    else
                    {
                        bzda.field_8F4 = -1;
                        bzda.field_8EC = gui_lstvw.field_1DE;

                        if ( !(bzda.field_1D0 & 4) )
                            bzda.field_1D0 = 2;
                    }
                }
            }
        }
    }

    if ( bact1 )
    {
        if ( bact1 != bact2 )
            sb_0x4c66f8(yw, bact1, bact2);
    }

    ypaworld_func64__sub7__sub2__sub0();
    ypaworld_func64__sub7__sub2__sub1(yw);
}







char * sub_4E1D6C(_NC_STACK_ypaworld *yw, char *cur, int x, int y, uint8_t icon, uint8_t icon2, uint8_t icon3, float a6, float a7, const char *a8)
{
    char *pcur = cur;

    int v26 = (a6 * up_panel.field_1E4);
    int v25 = (a7 * up_panel.field_1E4);

    if ( !v26 && a6 > 0.0 )
        v26 = 1;

    if ( !v25 && a7 > 0.0 )
        v25 = 1;

    fntcmd_select_tileset(&pcur, 30);
    fntcmd_set_center_xpos(&pcur, x);
    fntcmd_set_center_ypos(&pcur, y);

    fntcmd_store_u8(&pcur, icon);

    fntcmd_store_u8(&pcur, 84);

    if ( v26 > 0 || v25 > 0 )
    {
        int v13 = up_panel.field_1E0 + up_panel.field_1DC + x;

        fntcmd_set_center_xpos(&pcur, v13);

        if ( v26 > 0 )
        {
            if ( v26 > 1 )
                fntcmd_op11(&pcur, v26);

            fntcmd_store_u8(&pcur, icon2);
        }

        if ( v25 > v26 )
        {
            if ( v25 > 1 )
                fntcmd_op11(&pcur, v25);

            fntcmd_store_u8(&pcur, icon3);
        }
    }

    if ( a8 )
    {
        fntcmd_select_tileset(&pcur, 31);

        fntcmd_set_center_xpos(&pcur, x + up_panel.field_1DC + 4);
        fntcmd_set_ypos(&pcur, 0);

        strcpy(pcur, a8);

        pcur += strlen(a8);
    }
    return pcur;
}

char * ypaworld_func64__sub7__sub7__sub0__sub0(_NC_STACK_ypaworld *yw, char *cur, int x, int y, int a3, int a4, int a5, int a6, float a7)
{
    int icon;

    if ( a3 == -1 )
        icon = 82;
    else if ( a3 == 0 )
        icon = 83;
    else if ( a3 == 1 )
        icon = 81;

    int v11 = 0;
    switch ( yw->field_1b80->p_cell_area->owner )
    {
    case 1:
        v11 = 49;
        break;

    case 2:
        v11 = 50;
        break;

    case 3:
        v11 = 51;
        break;

    case 4:
        v11 = 52;
        break;

    case 5:
        v11 = 53;
        break;

    case 6:
        v11 = 54;
        break;

    case 7:
        v11 = 55;
        break;

    default:
        v11 = 0;
        break;
    }

    float a6a, a7a;

    if ( a4 <= 0 )
    {
        a7a = 0.0;
        a6a = 0.0;
    }
    else
    {
        a7a = a4 / 255.0;
        a6a = a7 * a7a;
    }

    char a1a[32];

    if ( a3 >= 0 )
    {
        if ( a5 )
        {
            sprintf(a1a, "+%d   (%d%%)", a5, (int)(a7 * 100.0) );
        }
        else
        {
            sprintf(a1a, "%d", 0);
        }
    }
    else
    {
        sprintf(a1a, "%d", -a5);
    }

    return sub_4E1D6C(yw, cur, x, y, icon, v11, 57, a6a, a7a, a1a);
}

char * ypaworld_func64__sub7__sub7__sub0__sub1(_NC_STACK_ypaworld *yw, char *cur, int x, int y, char a4, int a5, int a6, int a7)
{
    int v16;
    if ( a4 & 1 )
    {
        if ( a5 < 0 )
        {
            if ( a5 == -1 )
                v16 = 66;
        }
        else if ( a5 <= 0 )
        {
            v16 = 67;
        }
        else if ( a5 == 1 )
        {
            v16 = 65;
        }
    }
    else
    {
        v16 = 68;
    }

    char a1a[32];
    char *v9 = a1a;

    int v10;

    if ( a7 / 5 <= a6 )
    {
        v10 = 56;
    }
    else
    {
        v10 = 54;

        if ( yw->field_1614 / 300 & 1 )
            v9 = NULL;
    }

    float a6a = (float)a6 / (float)a7;
    sprintf(a1a, "%d", a6 / 100);

    return sub_4E1D6C(yw, cur, x, y, v16, v10, 0, a6a, 0.0, v9);
}


char * ypaworld_func64__sub7__sub7__sub0__sub2(_NC_STACK_ypaworld *yw, char *cur, int a3, int a2, char a4, int a5, int a6, int a7)
{
    int v18;
    if ( a4 & 4 )
    {
        if ( a5 < 0 )
        {
            if ( a5 == -1 )
                v18 = 70;
        }
        else if ( a5 <= 0 )
        {
            v18 = 71;
        }
        else if ( a5 == 1 )
        {
            v18 = 69;
        }
    }
    else
    {
        v18 = 72;
    }

    float v15 = 1.0 / (float)a7;
    int v9 = 57;
    float a7a = 0.0;
    float a6a = (float)a6 * v15;

    if ( bzda.field_1D0 & 0x16 )
    {
        if ( bzda.field_8F8 != -1 )
            a7a = yw->field_1b24.field_40 * v15;
    }

    if ( a7a <= a6a )
    {
        float v10 = a6a;
        a6a -= a7a;
        a7a = v10;
    }
    else
    {
        a7a = a6a;
        v9 = 54;
        a6a = 0.0;
    }

    char a1a[32];
    sprintf(a1a, "%d", a6 / 100);

    return sub_4E1D6C(yw, cur, a3, a2, v18, 56, v9, a6a, a7a, a1a);
}

char * ypaworld_func64__sub7__sub7__sub0__sub3(_NC_STACK_ypaworld *yw, char *cur, int x, int y, char a4, int a5, int a6, int a7)
{
    int v18;
    if ( a4 & 8 )
    {
        if ( a5 < 0 )
        {
            if ( a5 == -1 )
                v18 = 78;
        }
        else if ( a5 <= 0 )
        {
            v18 = 79;
        }
        else if ( a5 == 1 )
        {
            v18 = 77;
        }
    }
    else
    {
        v18 = 80;
    }

    float v15 = 1.0 / (float)a7;
    int v9 = 57;
    float a7a = 0.0;
    float a6a = (float)a6 * v15;

    if ( bzda.field_1D0 & 0x20 )
        a7a = yw->field_1b24.field_40 * v15;

    if ( a7a <= a6a )
    {
        float v10 = a6a;
        a6a -= a7a;
        a7a = v10;
    }
    else
    {
        a7a = a6a;
        v9 = 54;
        a6a = 0.0;
    }

    char a1a[32];
    sprintf(a1a, "%d", a6 / 100);

    return sub_4E1D6C(yw, cur, x, y, v18, 56, v9, a6a, a7a, a1a);
}

char * ypaworld_func64__sub7__sub7__sub0(_NC_STACK_ypaworld *yw)
{
    char *pcur = byte_51805C;

    if ( yw->field_1b80->field_3D5 != 2 )
    {
        int v3 = up_panel.frm_1.btn_xpos - yw->screen_width / 2;
        int y = up_panel.frm_1.btn_ypos - yw->screen_height / 2;
        int v41 = up_panel.field_1D8 + up_panel.field_1D0;

        int a4, v29, v28, v27, v26;
        call_vtbl(yw->field_1b78, 3, 0x8000200B, &a4, 0x80002008, &v29, 0x8000200A, &v28, 0x8000201C, &v27, 0x8000201D, &v26, 0);

        int v36 = yw->field_1b80->energy_2;

        yw_arg176 arg176;
        arg176.owner = yw->field_1b80->p_cell_area->owner;

        call_method(yw->self_full, 176, &arg176);

        int v32 = yw->field_1b80->p_cell_area->field_2F;
        int v33 = yw->field_1b80->p_cell_area->field_2F * arg176.field_4;

        int idx;
        call_vtbl(yw->field_1b78, 3, 0x8000201E, &idx, 0);

        int v11 = 10 * idx / 100;
        int v38 = 10 * ( idx * arg176.field_4 ) / 100;

        int v13, v34, v37, v35;

        if ( yw->field_1b80->owner == yw->field_1b80->p_cell_area->owner )
            v13 = v33 != 0;
        else if ( v33 )
            v13 = -1;
        else
            v13 = 0;

        if ( v27 & 1 )
            v34 = 1;
        else if ( v26 & 1 )
            v34 = -1;
        else
            v34 = 0;

        if ( v27 & 4 )
            v37 = 1;
        else if ( v26 & 4 )
            v37 = -1;
        else
            v37 = 0;

        if ( v27 & 8 )
            v35 = 1;
        else if ( v26 & 8 )
            v35 = -1;
        else
            v35 = 0;

        sub_412D9C(&up_panel.frm_1);
        sub_412D48(&up_panel.frm_1, 0);

        int x = LOWORD(up_panel.field_1D4) + v3;

        pcur = ypaworld_func64__sub7__sub7__sub0__sub0(yw, pcur, x, y, v13, v32, v38, v11, arg176.field_4);

        int v18 = v41 + x;
        pcur = ypaworld_func64__sub7__sub7__sub0__sub1(yw, pcur, v18, y, a4, v34, yw->field_1b80->energy, v36);

        int v20 = v41 + v18;
        pcur = ypaworld_func64__sub7__sub7__sub0__sub2(yw, pcur, v20, y, a4, v37, v29, v36);
        pcur = ypaworld_func64__sub7__sub7__sub0__sub3(yw, pcur, v41 + v20, y, a4, v35, v28, v36);
    }

    fntcmd_set_end(&pcur);

    return pcur;
}

void ypaworld_func64__sub7__sub7(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    winp_131arg *winpt = &inpt->winp131arg;

    if ( winpt->selected_btn == &up_panel.frm_1 )
    {
        if ( yw->GameShell )
        {
            if ( winpt->selected_btnID != -1 )
            {
                if ( winpt->flag & 0x10 )
                    sub_423F74(&yw->GameShell->samples1_info, 3);
            }
        }

        if ( winpt->flag & 0x40 )
        {
            int a4;
            call_vtbl(yw->field_1b78, 3, 0x8000200B, &a4, 0);

            if ( winpt->selected_btnID == 1 )
                a4 ^= 1;
            else if ( winpt->selected_btnID == 2 )
                a4 ^= 4;
            else if ( winpt->selected_btnID == 3 )
                a4 ^= 8;

            call_vtbl(yw->field_1b78, 2, 0x8000200B, a4, 0);
        }

        switch ( winpt->selected_btnID )
        {
        case 0:
            sub_4811E8(yw, 35);
            break;

        case 1:
            sub_4811E8(yw, 36);
            break;

        case 2:
            sub_4811E8(yw, 37);
            break;

        case 3:
            sub_4811E8(yw, 39);
            break;

        default:
            break;
        }
    }

    ypaworld_func64__sub7__sub7__sub0(yw);
}


uint8_t sub_4C7134(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact)
{
    uint8_t icon = yw->VhclProtos[ bact->id ].type_icon;

    if ( !icon )
        icon = 65;

    return icon;
}

char * sub_4C7950(_NC_STACK_ypaworld *yw, char *cur, int a4, int a3)
{
    char *pcur = cur;

    if ( a4 > 0 && a3 > 0 )
    {
        if ( a4 + squadron_manager.field_2CC < yw->screen_width )
        {
            if ( a3 + yw->font_default_h < yw->screen_height )
            {
                fntcmd_select_tileset(&pcur, 28);

                fntcmd_set_center_xpos(&pcur, a4 - (yw->screen_width / 2));
                fntcmd_set_center_ypos(&pcur, a3 - (yw->screen_height / 2));

                fntcmd_store_u8(&pcur, sub_4C7134(yw, squadron_manager.field_2BC));
            }
        }
    }
    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub2(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_store_u8(&pcur, 123);

    fntcmd_op17(&pcur, squadron_manager.lstvw.width - yw->font_default_w__b);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    return pcur;
}


char * sub_4514F0(tiles_stru *tyle, char *cur, char *a4, int a3, char a5)
{
    char *pcur = cur;

    int wdth = 0;
    int char_num = 0;
    int neg_wdth = 0;

    char *v6 = a4;

    while ( *v6 && neg_wdth == 0 )
    {
        int v18 = *v6;

        wdth += tyle->chars[v18].width;

        if ( wdth <= a3 )
            char_num++;
        else
            neg_wdth = tyle->chars[v18].width - (wdth - a3);

        v6++;
    }

    for (int i = 0; i < char_num; i++)
    {
        *pcur = a4[i];
        pcur++;
    }

    if ( neg_wdth <= 0 )
    {
        if ( wdth < a3 )
        {
            int v13 = a3 - wdth;

            while( v13 > 0 )
            {
                if ( v13 <= 255 )
                    fntcmd_op10(&pcur, v13);
                else
                    fntcmd_op10(&pcur, 255);

                v13 -= 255;

                fntcmd_store_u8(&pcur, a5);
            }
        }
    }
    else
    {
        fntcmd_set_xwidth(&pcur, neg_wdth);

        fntcmd_store_u8(&pcur, a4[char_num]);
    }

    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub1(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact, char *cur)
{
    char *pcur = cur;

    char v13[80];
    if ( bact == yw->field_1b84 && yw->field_1614 / 300 & 1 )
    {
        v13[0] = 33;
    }
    else if ( info_log.field_255C == bact->ypabact__id && yw->field_1614 - info_log.msgs[info_log.field_24C].field_4 < 10000 && yw->field_1614 / 300 & 1 )
    {
        v13[0] = 34;
    }
    else
    {
        v13[0] = sub_4C7134(yw, bact);
    }
    v13[1] = 0;

    int v4;

    if ( bzda.field_1D0 & 0x20 )
        v4 = 9;
    else
        v4 = 0;

    fntcmd_select_tileset(&pcur, 0);
    fntcmd_store_u8(&pcur, 123);

    fntcmd_select_tileset(&pcur, v4);
    fntcmd_op17(&pcur, squadron_manager.lstvw.width - yw->font_default_w__b);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_select_tileset(&pcur, 0);
    fntcmd_store_u8(&pcur, 125);
    fntcmd_add_xpos(&pcur, -(squadron_manager.lstvw.width - 2 * yw->font_default_w__b + 1));

    fntcmd_select_tileset(&pcur, 28);

    pcur = sub_4514F0(yw->tiles[28], pcur, v13, squadron_manager.lstvw.width - 2 * yw->font_default_w__b, 64);

    fntcmd_next_line(&pcur);

    return pcur;
}

uint8_t ypaworld_func64__sub7__sub3__sub0__sub0__sub0(__NC_STACK_ypabact *bact)
{
    if ( bact->field_3D5 == 1 )
    {
        if ( bact->field_3D6 & 0x4000 )
        {
            return 102;
        }
        else if ( bact->field_3DF )
        {
            return 99;
        }
        else if ( bact->field_3DE )
        {
            return 98;
        }
    }
    else if ( bact->field_3D5 == 3 )
    {
        if ( bact->field_3D6 & 0x4000 )
            return 102;
    }

    return 97;
}

char * ypaworld_func64__sub7__sub3__sub0__sub0(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact, char *cur)
{
    char *pcur = cur;

    char v39[64];
    char *pv = v39;

    int v46 = 0;
    __NC_STACK_ypabact *v44 = NULL;

    if ( yw->field_1a58 & 0x20 )
        v44 = yw->field_1a98;

    fntcmd_store_u8(&pv, ypaworld_func64__sub7__sub3__sub0__sub0__sub0(bact));
    fntcmd_store_u8(&pv, 64);

    int ttt = (yw->field_1614 / 300) & 1;

    if ( bact == yw->field_1b84 && ttt )
        fntcmd_store_u8(&pv, 33);
    else if ( yw->field_2420 == bact && ttt )
        fntcmd_store_u8(&pv, 34);
    else
        fntcmd_store_u8(&pv, sub_4C7134(yw, bact));

    fntcmd_store_u8(&pv, 64);

    if ( bact == v44 )
        v46 = squadron_manager.field_2D0;

    int v41 = squadron_manager.field_2D4;

    bact_node *nod = (bact_node *)bact->list2.head;
    while (nod->next)
    {
        if ( nod->bact->field_3D5 != 2 && nod->bact->field_3D5 != 4 && nod->bact->field_3D5 != 5 )
        {
            if ( nod->bact == yw->field_1b84 && ttt )
                fntcmd_store_u8(&pv, 33);
            else if ( nod->bact == yw->field_2420 && ttt )
                fntcmd_store_u8(&pv, 34);
            else
                fntcmd_store_u8(&pv, sub_4C7134(yw, nod->bact));

            if ( nod->bact == v44 )
                v46 = v41;

            v41 += squadron_manager.field_2CC;

            if ( pv - v39 >= 60 )
                break;
        }

        nod = (bact_node *)nod->next;
    }

    fntcmd_store_u8(&pv, 0);

    int v22;
    if ( yw->field_2410 == -1 || bact != yw->field_1c0c[ yw->field_2410 ] || bzda.field_1D0 & 0x20 )
        v22 = 0;
    else
        v22 = 9;

    fntcmd_select_tileset(&pcur, 0);
    fntcmd_store_u8(&pcur, 123);

    fntcmd_select_tileset(&pcur, v22);
    fntcmd_op17(&pcur, squadron_manager.lstvw.width - yw->font_default_w__b);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_select_tileset(&pcur, 0);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_add_xpos(&pcur, -(squadron_manager.lstvw.width - 2 * yw->font_default_w__b + 1));

    fntcmd_select_tileset(&pcur, 28);

    pcur = sub_4514F0(yw->tiles[28], pcur, v39, squadron_manager.lstvw.width - 2 * yw->font_default_w__b, 64);
    if ( v46 )
    {
        if ( squadron_manager.lstvw.width - squadron_manager.field_2CC - yw->font_default_w__b > v46 )
        {
            fntcmd_add_xpos(&pcur, -(squadron_manager.lstvw.width - 2 * yw->font_default_w__b));
            fntcmd_add_xpos(&pcur, v46);

            float v40 = (float)v44->energy / (float)v44->energy_2;

            if ( v40 > 0.25 )
            {
                if ( v40 > 0.5 )
                {
                    if ( v40 > 0.75 )
                        fntcmd_store_u8(&pcur, 131);
                    else
                        fntcmd_store_u8(&pcur, 130);
                }
                else
                {
                    fntcmd_store_u8(&pcur, 129);
                }
            }
            else
            {
                fntcmd_store_u8(&pcur, 128);
            }
        }
    }

    fntcmd_next_line(&pcur);
    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub3(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;
    int v23 = squadron_manager.lstvw.width - yw->font_default_w__b;

    if ( yw->field_2410 == -1 || bzda.field_1D0 & 0x20 )
    {
        fntcmd_select_tileset(&pcur, 0);
        fntcmd_store_u8(&pcur, 123);
    }
    else
    {
        int v5 = yw->field_1c0c[ yw->field_2410 ]->field_3D4;
        fntcmd_select_tileset(&pcur, 0);
        fntcmd_store_u8(&pcur, 123);
        fntcmd_store_u8(&pcur, 32);

        fntcmd_select_tileset(&pcur, 25);
        fntcmd_store_u8(&pcur, 49);

        if (v5 < 25)
            fntcmd_select_tileset(&pcur, 24);

        fntcmd_store_u8(&pcur, 50);

        if (v5 >= 25 && v5 < 50)
            fntcmd_select_tileset(&pcur, 24);

        fntcmd_store_u8(&pcur, 51);

        if (v5 >= 50 && v5 < 75)
            fntcmd_select_tileset(&pcur, 24);

        fntcmd_store_u8(&pcur, 52);

        if (v5 >= 75 && v5 < 100)
            fntcmd_select_tileset(&pcur, 24);

        fntcmd_store_u8(&pcur, 53);

        fntcmd_select_tileset(&pcur, 0);

        char a1a[32];
        sprintf(a1a, " %d", yw->field_2418 + 1);

        fntcmd_set_txtColor(&pcur, yw->iniColors[60].r, yw->iniColors[60].g, yw->iniColors[60].b);

        pcur = txtcmd_txt_w_bkg(yw->tiles[0], pcur, a1a, 4 * yw->tiles[0]->chars[65].width, 32);

        fntcmd_store_u8(&pcur, 32);

        fntcmd_select_tileset(&pcur, 0);
    }

    fntcmd_op17(&pcur, v23);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    fntcmd_reset_tileset(&pcur, 28);

    fntcmd_set_yoff(&pcur, yw->tiles[28]->font_height - yw->field_1a38);

    fntcmd_store_u8(&pcur, 38);

    fntcmd_op17(&pcur, squadron_manager.lstvw.width - yw->font_default_w__b);

    fntcmd_store_u8(&pcur, 47);
    fntcmd_store_u8(&pcur, 61);

    return pcur;
}



void ypaworld_func64__sub7__sub3__sub0(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    winp_131arg *winpt = &inpt->winp131arg;

    char *pcur = lstvw_up_border(yw, &squadron_manager.lstvw, squadron_manager.lstvw.data_cmdbuf, 0, "{ }");

    fntcmd_select_tileset(&pcur, 28);

    for (int i = 0; i < squadron_manager.lstvw.element_count; i++)
    {
        __NC_STACK_ypabact *v8 = squadron_manager.squads[i];

        if ( v8 )
        {
            if ( v8 == yw->field_1b80 )
            {
                pcur = ypaworld_func64__sub7__sub3__sub0__sub1(yw, v8, pcur);
            }
            else
            {
                if ( v8->parent_bacto == v8->field_32 )
                    pcur = ypaworld_func64__sub7__sub3__sub0__sub0(yw, v8, pcur);
                else
                    pcur = ypaworld_func64__sub7__sub3__sub0__sub2(yw, pcur);
            }
        }
        else
        {
            pcur = ypaworld_func64__sub7__sub3__sub0__sub2(yw, pcur);
        }
    }

    pcur = ypaworld_func64__sub7__sub3__sub0__sub3(yw, pcur);

    if ( squadron_manager.field_2A8 & 1 )
    {
        if ( squadron_manager.field_2C8 != winpt->move[0].x || squadron_manager.field_2CA != winpt->move[0].y )
        {
            pcur = sub_4C7950(yw, pcur, squadron_manager.field_2C4, squadron_manager.field_2C6);

            if ( squadron_manager.field_2A8 & 2 )
                pcur = sub_4C7950(yw, pcur, squadron_manager.field_2C4 + squadron_manager.field_2CC / 4, squadron_manager.field_2C6 + (yw->font_default_h / 4) );
        }
    }

    fntcmd_set_end(&pcur);
}

void sub_4C707C(_NC_STACK_ypaworld *yw)
{
    memset(squadron_manager.squads, 0, sizeof(squadron_manager.squads));

    if ( squadron_manager.lstvw.scroll_pos + squadron_manager.lstvw.element_count >= yw->field_2414 + 1 )
    {
        squadron_manager.lstvw.scroll_pos = yw->field_2414 + 1 - squadron_manager.lstvw.element_count;

        if ( squadron_manager.lstvw.scroll_pos < 0 )
            squadron_manager.lstvw.scroll_pos = 0;
    }

    int v5 = squadron_manager.lstvw.scroll_pos;
    for (int i = 0; i < yw->field_2414 + 1; i++ )
    {
        __NC_STACK_ypabact *v6;

        if ( i )
            v6 = yw->field_1c0c[i - 1];
        else
            v6 = yw->field_1b80;

        if ( i >= v5 && i - v5 < squadron_manager.lstvw.element_count )
            squadron_manager.squads[i - v5] = v6;
    }

    squadron_manager.lstvw.elements_for_scroll_size = yw->field_2414 + 1;
}


int ypaworld_func64__sub7__sub3__sub2(_NC_STACK_ypaworld *yw)
{
    __NC_STACK_ypabact *bct = yw->field_1c0c[squadron_manager.field_2B4];
    if ( !bct )
        return 0;

    if ( bct == squadron_manager.field_2BC )
    {
        if (bct->field_3D5 != 2 && bct->field_3D5 != 4 && bct->field_3D5 != 5 )
            return 1;
    }


    bact_node *bbct = (bact_node *)bct->list2.head;
    while (bbct->next)
    {
        if ( bbct->bact == squadron_manager.field_2BC )
        {
            int v5 = bbct->bact->field_3D5;
            if ( v5 != 2 && v5 != 4 && v5 != 5 )
                return 1;
        }

        bbct = (bact_node *)bbct->next;
    }
    return 0;
}

void ypaworld_func64__sub7__sub3__sub3(_NC_STACK_ypaworld *yw, winp_131arg *winpt)
{
    if ( winpt->selected_btn != &squadron_manager.lstvw.frm_1
            || winpt->selected_btnID < 8
            || squadron_manager.lstvw.field_1DE <= winpt->selected_btnID - 8 + squadron_manager.lstvw.scroll_pos
            || squadron_manager.squads[ winpt->selected_btnID - 8 ] == 0
            || squadron_manager.squads[ winpt->selected_btnID - 8 ] == yw->field_1b80 )
    {
        if ( squadron_manager.field_2A8 & 2 )
        {
            if ( squadron_manager.field_2BC->field_32 != squadron_manager.field_2BC->parent_bacto )
            {
                NC_STACK_ypabact *v3 = NULL;

                bact_node *v13 = &squadron_manager.field_2BC->list_node;
                while(v13->next)
                {
                    bact_arg109 arg109;

                    if ( v3 )
                    {
                        arg109.field_4 = v13->bact;
                        arg109.field_0 = 4;
                    }
                    else
                    {
                        arg109.field_4 = NULL;
                        arg109.field_0 = 3;

                        v3 = v13->bacto;
                    }
                    call_method(v3, 109, &arg109);

                    v13 = (bact_node *)v13->next;
                }
            }
        }
        else
        {
            bact_arg109 arg109;
            arg109.field_4 = NULL;
            arg109.field_0 = 3;

            call_method(squadron_manager.field_2BC->self, 109, &arg109);
        }
        yw->field_240c = squadron_manager.field_2BC->field_2E;
        sub_4C707C(yw);
        sub_4C40AC(yw);
    }
    else
    {
        __NC_STACK_ypabact *v6 = squadron_manager.squads[ winpt->selected_btnID - 8 ];
        int v7;
        if ( squadron_manager.field_2BC->field_32 == squadron_manager.field_2BC->parent_bacto )
            v7 = v6 == squadron_manager.field_2BC;
        else
            v7 = v6 == squadron_manager.field_2BC->parent_bact;

        if ( !v7 )
        {
            if ( squadron_manager.field_2A8 & 2 )
            {
                if ( squadron_manager.field_2BC->field_32 != squadron_manager.field_2BC->parent_bacto )
                {
                    bact_node *v9 = &squadron_manager.field_2BC->list_node;
                    while (v9->next)
                    {
                        bact_arg109 arg109;
                        arg109.field_0 = 1;
                        arg109.field_4 = v6;
                        call_method(v9->bacto, 109, &arg109);

                        v9 = (bact_node *)v9->next;
                    }
                }
                else
                {
                    bact_arg109 arg109;
                    arg109.field_0 = 1;
                    arg109.field_4 = v6;
                    call_method(squadron_manager.field_2BC->self, 109, &arg109);
                }
            }
            else
            {
                bact_arg109 arg109;
                arg109.field_0 = 4;
                arg109.field_4 = squadron_manager.field_2BC;
                call_method(v6->self, 109, &arg109);
            }
            yw->field_240c = v6->field_2E;
            sub_4C707C(yw);
            sub_4C40AC(yw);
        }
    }
}

void ypaworld_func64__sub7__sub3__sub4(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact)
{
    if ( bact->field_3D5 == 1 )
    {
        if ( bact->field_3D6 & 0x4000 )
            sub_4811E8(yw, 34);
        else if ( bact->field_3DF )
            sub_4811E8(yw, 32);
        else if ( bact->field_3DE )
            sub_4811E8(yw, 33);
    }
    else if (bact->field_3D5 == 3)
        sub_4811E8(yw, 31);
}

__NC_STACK_ypabact * sub_4C7B0C(_NC_STACK_ypaworld *yw, int sqid, int a3)
{
    __NC_STACK_ypabact *v3 = squadron_manager.squads[sqid];

    if ( !v3 )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return NULL;
    }

    if ( v3 == yw->field_1b80 )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return yw->field_1b80;
    }

    squadron_manager.field_2B0 = -1;
    squadron_manager.field_2AC = squadron_manager.lstvw.scroll_pos + sqid - 1;

    if ( a3 > squadron_manager.field_2D0 && a3 < squadron_manager.field_2D0 + squadron_manager.field_2CC )
        return v3;

    int v5 = a3 - squadron_manager.field_2D4;
    if ( v5 < 0 )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return NULL;
    }

    squadron_manager.field_2B0 = v5 / squadron_manager.field_2CC;

    int v6 = squadron_manager.field_2B0;

    bact_node *node = (bact_node *)v3->list2.head;

    while(node->next)
    {
        int v8 = node->bact->field_3D5;
        if (v8 != 2 && v8 != 4 && v8 != 5)
        {
            v6--;
            if (v6 == -1)
            {
                return node->bact;
            }
        }

        node = (bact_node *)node->next;
    }

    squadron_manager.field_2B0 = -1;
    squadron_manager.field_2AC = -1;
    return NULL;
}

int ypaworld_func64__sub7__sub3__sub1(_NC_STACK_ypaworld *yw, winp_131arg *winpt)
{
    if ( winpt->selected_btnID < 8 )
        return 0;
    if ( bzda.field_1D0 & 8 )
        return 0;
    if ( winpt->selected_btnID - 8 + squadron_manager.lstvw.scroll_pos >= squadron_manager.lstvw.elements_for_scroll_size )
        return 0;

    __NC_STACK_ypabact *v5 = sub_4C7B0C(yw, winpt->selected_btnID - 8, winpt->move[2].x);

    if ( !v5 )
        return 0;

    if ( !(robo_map.cmd_flag & 0x20) )
    {
        if ( winpt->flag & 0x80 )
        {
            robo_map.field_1D8 = v5->field_621.sx;
            robo_map.field_1ED = 0;
            robo_map.field_1DC = v5->field_621.sz;

            sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
        }
    }

    if ( v5 == yw->field_1b80 )
        return 0;

    squadron_manager.field_2B4 = squadron_manager.field_2AC;
    squadron_manager.field_2B8 = squadron_manager.field_2B0;
    squadron_manager.field_2C8 = winpt->move[0].x;
    squadron_manager.field_2BC = v5;

    if ( v5->parent_bacto == v5->field_32 )
    {
        squadron_manager.field_2C0 = -(winpt->move[2].x - squadron_manager.field_2D0);
    }
    else
    {
        int v11 = winpt->move[2].x - squadron_manager.field_2D4;
        squadron_manager.field_2C0 = -(v11 % squadron_manager.field_2CC);
    }

    squadron_manager.field_2C2 = -winpt->move[2].y;
    squadron_manager.field_2CA = winpt->move[0].y;
    return 1;
}

void ypaworld_func64__sub7__sub3(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    if ( squadron_manager.lstvw.cmd_flag & 0x21 )
    {
        squadron_manager.field_2A8 &= 0xFFFFFFFE;
        lstvw_update_input(yw, &squadron_manager.lstvw, inpt);
    }
    else
    {
        winp_131arg *winpt = &inpt->winp131arg;
        sub_4C707C(yw);

        if ( squadron_manager.field_2A8 & 1 )
        {
            if ( ypaworld_func64__sub7__sub3__sub2(yw) && (winpt->flag & 0x104) != 260 )
            {
                if ( winpt->flag & 0x104 )
                {
                    squadron_manager.field_2C4 = winpt->move[0].x + squadron_manager.field_2C0;
                    squadron_manager.field_2C6 = winpt->move[0].y + squadron_manager.field_2C2;
                }
                else
                {
                    ypaworld_func64__sub7__sub3__sub3(yw, winpt);
                    squadron_manager.field_2A8 &= 0xFFFFFFFC;
                }
            }
            else
            {
                squadron_manager.field_2A8 &= 0xFFFFFFFC;
            }
        }
        else if ( inpt->winp131arg.selected_btn == &squadron_manager.lstvw.frm_1 )
        {
            if ( inpt->winp131arg.selected_btnID == 7 )
            {
                if ( winpt->flag & 0x40 )
                {
                    sub_449DE8(yw, get_lang_string(yw->string_pointers_p2, 765, "help\\l15.html")); //MAKE ME
                }
                sub_4811E8(yw, 0x30);
            }
            if ( winpt->selected_btnID == 6 && yw->field_2410 != -1 && !(bzda.field_1D0 & 0x20) )
            {
                int v9 = (winpt->move[2].x - squadron_manager.field_2DC) / yw->tiles[24]->chars[49].width;

                if ( v9 >= 0 && v9 <= 4 )
                {
                    if ( winpt->flag & 0x10 )
                    {
                        call_vtbl(yw->field_1c0c[yw->field_2410]->self, 2, 0x8000100D, 25 * v9, 0);

                        if ( yw->GameShell )
                            sub_423F74(&yw->GameShell->samples1_info, 3);
                    }
                }

                switch ( v9 )
                {
                case 0:
                    sub_481204(yw, 12, 166);
                    break;

                case 1:
                    sub_481204(yw, 13, 167);
                    break;

                case 2:
                    sub_481204(yw, 14, 168);
                    break;

                case 3:
                    sub_481204(yw, 15, 169);
                    break;

                case 4:
                    sub_481204(yw, 16, 170);
                    break;

                default:
                    break;
                }

                if ( winpt->move[2].x >= squadron_manager.field_2D8 )
                    sub_4811E8(yw, 46);
            }

            if ( winpt->flag & 0x82 )
            {
                if ( ypaworld_func64__sub7__sub3__sub1(yw, winpt) )
                {
                    squadron_manager.field_2A8 &= 0xFFFFFFFC;
                    squadron_manager.field_2A8 |= 1;

                    if ( winpt->flag & 0x80 )
                        squadron_manager.field_2A8 |= 3;

                    squadron_manager.field_2C4 = winpt->move[0].x + squadron_manager.field_2C0;
                    squadron_manager.field_2C6 = winpt->move[0].y + squadron_manager.field_2C2;
                }
            }
            if ( squadron_manager.lstvw.field_1E4 != -1 )
            {
                int v17 = squadron_manager.lstvw.field_1E4 - squadron_manager.lstvw.scroll_pos;

                if ( v17 >= 0 && v17 < squadron_manager.lstvw.element_count_max )
                {
                    __NC_STACK_ypabact *v18 = squadron_manager.squads[v17];
                    if ( v18 )
                    {
                        if ( winpt->move[2].x < squadron_manager.field_2D0 )
                            ypaworld_func64__sub7__sub3__sub4(yw, v18);
                    }
                }
            }
        }

        lstvw_update_input(yw, &squadron_manager.lstvw, inpt);
        lstvw_update(yw, &squadron_manager.lstvw);
        ypaworld_func64__sub7__sub3__sub0(yw, inpt);
    }
}


void sub_4C1FBC()
{
    int v0 = -(stru_5B25D8.xpos + robo_map.frm_1.btn_xpos + (stru_5B25D8.width / 2));
    int v3 = -(stru_5B25D8.ypos + robo_map.frm_1.btn_ypos + (stru_5B25D8.fnt_height / 2));
    flt_516524 = (float)(v0 + dword_516510) * robo_map.field_1E0 + robo_map.field_1D8;
    flt_51652C = (float)(v0 + dword_516518) * robo_map.field_1E0 + robo_map.field_1D8;
    flt_516528 = robo_map.field_1DC - (float)(v3 + dword_516514) * robo_map.field_1E4;
    flt_516530 = robo_map.field_1DC - (float)(v3 + dword_51651C) * robo_map.field_1E4;

    if ( flt_516524 > flt_51652C )
    {
        float v4 = flt_516524;
        flt_516524 = flt_51652C;
        flt_51652C = v4;
    }

    if ( flt_516528 > flt_516530 )
    {
        float v5 = flt_516528;
        flt_516528 = flt_516530;
        flt_516530 = v5;
    }
}

void ypaworld_func64__sub7__sub1__sub0(_NC_STACK_ypaworld *yw)
{
    int v8 = 0;

    int v22 = flt_516524 / 1200;
    int v6 = -flt_516530 / 1200;
    int v24 = flt_51652C / 1200;
    int v23 = -flt_516528 / 1200;

    if ( v22 >= yw->sectors_maxX2 )
        v22 = yw->sectors_maxX2 - 1;
    else if ( v22 < 1 )
        v22 = 1;

    if ( v24 >= yw->sectors_maxX2 )
        v24 = yw->sectors_maxX2 - 1;
    else if ( v24 < 1 )
        v24 = 1;

    if ( v6 >= yw->sectors_maxY2 )
        v6 = yw->sectors_maxY2 - 1;
    else if ( v6 < 1 )
        v6 = 1;

    if ( v23 >= yw->sectors_maxY2 )
        v23 = yw->sectors_maxY2 - 1;
    else if ( v23 < 1 )
        v23 = 1;

    __NC_STACK_ypabact *v13 = NULL;

    for (int i = v6; i <= v23; i++)
    {
        for (int j = v22; j <= v24; j++)
        {
            cellArea *cll = &yw->cells[j + yw->sectors_maxX2 * i];

            if ( robo_map.field_1EF & cll->field_39 )
            {
                __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)cll->field_3C.head;

                while (bct->next)
                {
                    if ( bct->owner == yw->field_1b80->owner )
                    {
                        if ( bct->field_621.sx > flt_516524 && bct->field_621.sz > flt_516528 && bct->field_621.sx < flt_51652C && bct->field_621.sz < flt_516530 )
                        {
                            if ( bct->field_24 != 4 && bct->field_24 != 9 )
                            {
                                if ( bct->field_3D5 != 4 && bct->field_3D5 != 5 && bct->field_3D5 != 2 )
                                {

                                    if ( v13 )
                                    {
                                        if ( bct->field_24 != 3 )
                                        {
                                            bact_arg109 arg109;
                                            arg109.field_4 = bct;
                                            arg109.field_0 = 4;

                                            call_method(v13->self, 109, &arg109);

                                            v8 = 0;
                                        }
                                    }
                                    else
                                    {
                                        if ( bct->field_24 == 3 )
                                        {
                                            if ( bct == yw->field_1b80 )
                                                v8 = 1;
                                        }
                                        else
                                        {
                                            v13 = bct;

                                            bact_arg109 arg109;
                                            arg109.field_4 = NULL;
                                            arg109.field_0 = 3;

                                            call_method(v13->self, 109, &arg109);

                                            v8 = 0;
                                        }
                                    }


                                }
                            }
                        }
                    }
                    bct = (__NC_STACK_ypabact *)bct->next;
                }
            }

        }
    }

    if ( v8 )
        bzda.field_1D0 = bzda.field_1CC & 0x20;

    if ( v13 )
        yw->field_240c = v13->field_2E;

    sub_4C40AC(yw);
}

void  ypaworld_func64__sub7__sub1(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    if ( robo_map.cmd_flag & 0x20 )
    {
        robo_map.field_1E8 &= 0xFFFFFDE8;
    }
    else
    {
        robo_map.field_1EF = 1 << yw->field_1b80->owner;

        winp_131arg *winpt = &inpt->winp131arg;

        if ( yw->field_1b68 )
            robo_map.field_1EF = -1;

        switch ( inpt->dword8 )
        {
        case (0x80 | 0x0A):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 3;
            winpt->flag |= 0x10;
            break;

        case (0x80 | 0x0B):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 4;
            winpt->flag |= 0x10;
            break;

        case (0x80 | 0x0C):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 5;
            winpt->flag |= 0x10;
            break;

        case (0x80 | 0x0E):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 6;
            winpt->flag |= 0x10;
            break;

        case (0x80 | 0x10):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 7;
            winpt->flag |= 0x40;
            break;

        case (0x80 | 0x11):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 8;
            winpt->flag |= 0x40;
            break;

        case (0x80 | 0x12):
            winpt->selected_btn = &robo_map.frm_1;
            winpt->selected_btnID = 9;
            winpt->flag |= 0x40;
            break;

        default:
            break;
        }

        if ( robo_map.field_1E8 & 1 )
        {
            if ( winpt->flag & 4 )
            {
                int v15 = winpt->move[0].x + robo_map.field_218;
                int v16 = winpt->move[0].y + robo_map.field_21A;

                if ( v15 < robo_map.field_210 )
                    v15 = robo_map.field_210;
                else if ( v15 > robo_map.field_214 )
                    v15 = robo_map.field_214;

                if ( v16 < robo_map.field_212 )
                    v16 = robo_map.field_212;
                else if ( v16 > robo_map.field_216 )
                    v16 = robo_map.field_216;

                if ( v15 + robo_map.frm_1.btn_xpos >= yw->screen_width )
                    v15 = yw->screen_width - robo_map.frm_1.btn_xpos;

                if ( robo_map.frm_1.btn_ypos + v16 >= yw->screen_height - yw->icon0___h )
                    v16 = yw->screen_height - yw->icon0___h - robo_map.frm_1.btn_ypos;

                robo_map.frm_1.btn_height = v16;
                robo_map.frm_1.btn_width = v15;
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFE;
                yw->field_17bc = 0;
            }
        }
        else if ( robo_map.field_1E8 & 2 )
        {
            if ( winpt->flag & 4 )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1D8 = (yw->sectors_maxX2 * 1200.0) / (float)robo_map.field_1CC * (float)(winpt->move[0].x - robo_map.field_21C ) + robo_map.field_220;
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFD;
                yw->field_17bc = 0;
            }
        }
        else if ( robo_map.field_1E8 & 4 )
        {
            if ( winpt->flag & 4 )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1DC = robo_map.field_224 - (yw->sectors_maxY2 * 1200.0) / (float)robo_map.field_1D2 * (float)(winpt->move[0].y - robo_map.field_21E);
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFB;
                yw->field_17bc = 0;
            }
        }
        else if ( robo_map.field_1E8 & 0x10 )
        {
            if ( winpt->flag & 0x100 )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1D8 = robo_map.field_220 + (float)(robo_map.field_21C - winpt->move[0].x) * robo_map.field_1E0;
                robo_map.field_1DC = robo_map.field_224 - (float)(robo_map.field_21E - winpt->move[0].y) * robo_map.field_1E4;
            }
            else
            {
                int v25 = abs(robo_map.field_21C - winpt->move[0].x);
                int v27 = abs(robo_map.field_21E - winpt->move[0].y);

                if ( v25 <= 1 && v27 <= 1 )
                {
                    yw->field_2410 = -1;
                    yw->field_240c = 0;

                    if ( bzda.field_1D0 & 0x28 )
                        bzda.field_1D0 = bzda.field_1CC & 1;
                }
                robo_map.field_1E8 &= 0xFFFFFFEF;
                yw->field_17bc = 0;
            }
        }
        else if ( robo_map.field_1E8 & 0x200 )
        {
            dword_516518 = winpt->move[0].x;
            dword_51651C = winpt->move[0].y;

            sub_4C1FBC();

            if ( !(winpt->flag & 4) )
            {
                if ( abs(dword_516518 - dword_516510) <= 5 || abs(dword_51651C - dword_516514) <= 5 )
                {
                    robo_map.field_1E8 &= 0xFFFFFDFF;
                    winpt->flag |= 2;
                }
                else
                {
                    bzda.field_1D0 = 1;
                    robo_map.field_1E8 &= 0xFFFFFDFF;
                    ypaworld_func64__sub7__sub1__sub0(yw);
                }
            }
        }
        else if ( winpt->selected_btn == &robo_map.frm_1 )
        {
            if ( winpt->flag & 0x80 )
            {
                if ( winpt->selected_btnID == 17 )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_21C = winpt->move[0].x;
                    robo_map.field_21E = winpt->move[0].y;
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1E8 |= 0x10;
                    yw->field_17bc = 1;
                }
            }

            if ( winpt->flag & 2 )
            {
                if ( winpt->selected_btnID == 17 )
                {
                    robo_map.field_1E8 |= 0x200;
                    dword_516510 = winpt->move[0].x;
                    dword_516514 = winpt->move[0].y;
                    dword_516518 = dword_516510;
                    dword_51651C = dword_516514;

                    sub_4C1FBC();

                    winpt->flag &= 0xFFFFFFFD;
                }
            }

            if ( winpt->selected_btnID > 1 && winpt->selected_btnID <= 9 )
            {
                if ( winpt->flag & 0x10 )
                {
                    if ( yw->GameShell )
                        sub_423F74(&yw->GameShell->samples1_info, 3);
                }
            }

            if ( winpt->flag & 0x10 )
            {
                switch ( winpt->selected_btnID )
                {
                case 16:
                    robo_map.field_218 = robo_map.frm_1.btn_width - winpt->ldw_pos[0].x;
                    robo_map.field_21A = robo_map.frm_1.btn_height - winpt->ldw_pos[0].y;

                    robo_map.field_1E8 |= 1;
                    yw->field_17bc = 1;
                    break;

                case 14:
                    robo_map.field_21C = winpt->ldw_pos[0].x;
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1E8 |= 2;
                    yw->field_17bc = 1;
                    break;

                case 11:
                    robo_map.field_21E = winpt->ldw_pos[0].y;
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1E8 |= 4;
                    yw->field_17bc = 1;
                    break;

                case 3:
                    if ( robo_map.field_1EC & 1 )
                        robo_map.field_1EC &= 0xFE;
                    else
                        robo_map.field_1EC |= 1;

                    sub_4C1970(yw, 0);
                    break;

                case 4:
                    if ( robo_map.field_1EC & 2 )
                        robo_map.field_1EC &= 0xFD;
                    else
                        robo_map.field_1EC |= 2;

                    sub_4C1970(yw, 0);
                    break;

                case 5:
                    if ( robo_map.field_1EC & 4 )
                        robo_map.field_1EC &= 0xFB;
                    else
                        robo_map.field_1EC |= 4;

                    sub_4C1970(yw, 0);
                    break;

                case 6:
                    robo_map.field_1ED = robo_map.field_1ED != 1;
                    break;

                default:
                    break;
                }
            }

            switch ( winpt->selected_btnID )
            {
            case 13:
                if ( winpt->flag & 0x30 )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1ED = 0;
                    robo_map.field_1D8 = yw->sectors_maxX2 * 1200.0 / (-1.0 * (float)robo_map.field_1CC) + robo_map.field_1D8;
                }
                break;

            case 15:
                if ( winpt->flag & 0x30 )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1ED = 0;
                    robo_map.field_1D8 = yw->sectors_maxX2 * 1200.0 / (float)robo_map.field_1CC + robo_map.field_1D8;
                }
                break;

            case 10:
                if ( winpt->flag & 0x30 )
                {
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1ED = 0;
                    robo_map.field_1DC = robo_map.field_1DC - yw->sectors_maxY2 * 1200.0 / (-1.0 * (float)robo_map.field_1D2);
                }
                break;

            case 12:
                if ( winpt->flag & 0x30 )
                {
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1ED = 0;
                    robo_map.field_1DC = robo_map.field_1DC - yw->sectors_maxY2 * 1200.0 / (float)robo_map.field_1D2;
                }
                break;

            case 9:
                if ( winpt->flag & 0x30 )
                    robo_map.field_1E8 |= 0x20;

                if ( winpt->flag & 0x40 )
                {
                    int tmp1, tmp2, tmp3, tmp4;
                    tmp1 = robo_map.field_208;
                    tmp2 = robo_map.field_20A;
                    tmp3 = robo_map.field_20C;
                    tmp4 = robo_map.field_20E;

                    robo_map.field_208 = robo_map.frm_1.btn_xpos;
                    robo_map.field_20A = robo_map.frm_1.btn_ypos;
                    robo_map.field_20C = robo_map.frm_1.btn_width;
                    robo_map.field_20E = robo_map.frm_1.btn_height;

                    robo_map.frm_1.btn_xpos = tmp1;
                    robo_map.frm_1.btn_ypos = tmp2;
                    robo_map.frm_1.btn_width = tmp3;
                    robo_map.frm_1.btn_height = tmp4;
                }
                break;

            case 2:
                if ( winpt->flag & 0x30 )
                    robo_map.cmd_flag |= 0x80;

                if ( winpt->flag & 0x40 )
                {
                    robo_map.cmd_flag &= 0xFFFFFF7F;
                    sub_449DE8(yw, get_lang_string(yw->string_pointers_p2, 764, "help\\l14.html")); //MAKE ME
                }

                sub_4811E8(yw, 48);
                break;

            case 7:
                if ( winpt->flag & 0x30 )
                    robo_map.field_1E8 |= 0x40;

                if ( winpt->flag & 0x40 )
                    sub_4C1970(yw, 1);

                break;

            case 8:
                if ( winpt->flag & 0x30 )
                    robo_map.field_1E8 |= 0x80;

                if ( winpt->flag & 0x40 )
                    sub_4C1970(yw, 2);

                break;

            default:
                break;
            }

            switch ( winpt->selected_btnID )
            {
            case 3:
                sub_481204(yw, 22, 138);
                break;

            case 4:
                sub_481204(yw, 23, 139);
                break;

            case 5:
                sub_481204(yw, 24, 140);
                break;

            case 6:
                sub_481204(yw, 26, 142);
                break;

            case 7:
                sub_481204(yw, 29, 144);
                break;

            case 8:
                sub_481204(yw, 28, 145);
                break;

            case 9:
                sub_481204(yw, 30, 146);
                break;

            default:
                break;
            }
        }
    }
}

char * ypaworld_func64__sub7__sub0__sub0__sub0(_NC_STACK_ypaworld *yw, char *cur, int id)
{
    int v5 = id + info_log.field_254 + info_log.window.scroll_pos;

    if ( v5 >= 64 )
        v5 -= 64;
    else if ( v5 < 0 )
        v5 += 64;

    char *pcur = cur;

    fntcmd_store_u8(&pcur, 123);

    pcur = txtcmd_txt_w_bkg(yw->tiles[0], pcur, info_log.msgs[v5].txt, info_log.window.width - 2 * yw->font_default_w__b, 32);

    fntcmd_store_u8(&pcur, 125);
    fntcmd_next_line(&pcur);

    return pcur;
}

void ypaworld_func64__sub7__sub0__sub0(_NC_STACK_ypaworld *yw)
{
    char *pcur = lstvw_up_border(yw, &info_log.window, info_log.window.data_cmdbuf, 0, "{ }");

    for (int i = 0; i < info_log.window.element_count; i++ )
        pcur = ypaworld_func64__sub7__sub0__sub0__sub0(yw, pcur, i);

    pcur = lstvw_down_border(yw, &info_log.window, pcur, 0, "xyz");

    fntcmd_set_end(&pcur);
}

void ypaworld_func64__sub7__sub0(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    if ( info_log.window.cmd_flag & 0x21 )
    {
        lstvw_update_input(yw, &info_log.window, inpt);
    }
    else
    {
        lstvw_update_input(yw, &info_log.window, inpt);
        lstvw_update(yw, &info_log.window);
        ypaworld_func64__sub7__sub0__sub0(yw);
    }
}





int ypaworld_func64__sub7__sub6__sub0(int a1, const char *a2)
{
    char a1a[300];

    sprintf(a1a, "save:%s/%d.rst", a2, a1);

    FILE *fil = FOpen(a1a, "r");

    if ( !fil )
        return 0;

    FClose(fil);
    return 1;
}

listbase * ypaworld_func64__sub7__sub6__sub1()
{
    return dword_5BAFAC;
}

void sub_47DB04(_NC_STACK_ypaworld *yw, char a2)
{
    yw_arg181 v3;
    char v4[20];

//  *(_DWORD *)v4 = 1035;
//  v4[12] = yw->GameShell->field_0x1cd4;
//  v4[16] = a2;
    v3.field_10 = 0;
    v3.field_14 = 2;
    v3.value = v4;
    v3.val_size = 20;
    v3.field_18 = 1;

    call_method(yw->self_full, 181, &v3);

    yw->GameShell->field_283F = 0;
}

void ypaworld_func64__sub7__sub6__sub2(_NC_STACK_ypaworld *, int a2)
{
    dword_5BAF9C = a2;
}


char * sub_451714(tiles_stru *, char *cur, const char *a3, int a2, uint8_t a4)
{
    char *pcur = cur;

    if ( a2 > 0 )
    {
        fntcmd_copy_position(&pcur);

        int v6 = a2;
        while (v6 > 0)
        {
            if ( v6 <= 255 )
                fntcmd_op10(&pcur, v6);
            else
                fntcmd_op10(&pcur, 255);

            fntcmd_store_u8(&pcur, a4);

            v6 -= 255;
        }

        fntcmd_add_txt(&pcur, a2, 4, a3);
    }
    return pcur;
}


char *sub_4DA8DC(_NC_STACK_ypaworld *yw, char *cur, int a4, int a3, const char *a5)
{
    const char *v5 = a5;

    char *pcur = cur;

    fntcmd_set_yheight(&pcur, dword_5C8B84);

    fntcmd_store_u8(&pcur, 123);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_op17(&pcur, dword_5C8B80 + dword_5C8B88);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    fntcmd_store_u8(&pcur, 123);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    int v11;

    if ( a4 )
        v11 = 4;
    else if ( a3 )
        v11 = 8;
    else
        v11 = 3;

    fntcmd_select_tileset(&pcur, v11);
    fntcmd_store_u8(&pcur, 98);

    if ( a4 )
        v5 = " ";

    if ( a3 )
    {
        fntcmd_set_flag(&pcur, 0x10);
    }

    pcur = sub_451714(yw->tiles[v11], pcur, v5, dword_5C8B88 - 2 * yw->font_default_w__b, 32);

    fntcmd_unset_flag(&pcur, 0x10);

    fntcmd_store_u8(&pcur, 100);

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    fntcmd_set_yheight(&pcur, dword_5C8B84);

    fntcmd_store_u8(&pcur, 123);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_op17(&pcur, dword_5C8B80 + dword_5C8B88);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    return pcur;
}



void ypaworld_func64__sub7__sub6__sub3(_NC_STACK_ypaworld *yw, int a2, int a4)
{
    char *pcur = lstvw_up_border(yw, &exit_menu, exit_menu.data_cmdbuf, 0, "{ }");

    fntcmd_set_txtColor(&pcur, yw->iniColors[68].r, yw->iniColors[68].g, yw->iniColors[68].b);

    pcur = sub_4DA8DC(yw, pcur, a4 & 0x100, a2 & 0x100, get_lang_string(yw->string_pointers_p2, 7, "CANCEL MISSION"));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x200, a2 & 0x200, get_lang_string(yw->string_pointers_p2, 5, "SAVE"));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x400, a2 & 0x400, get_lang_string(yw->string_pointers_p2, 4, "LOAD"));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x800, a2 & 0x800, get_lang_string(yw->string_pointers_p2, 8, "RESTART"));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x1000, a2 & 0x1000, get_lang_string(yw->string_pointers_p2, 9, "RESUME"));

    pcur = lstvw_down_border(yw, &exit_menu, pcur, 0, "xyz");

    fntcmd_set_end(&pcur);
}


void ypaworld_func64__sub7__sub6(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    if ( exit_menu.cmd_flag & 0x20 )
    {
        if ( yw->field_2d90->field_40 )
        {
            if ( yw->field_2d90->field_40 == 6 )
            {
                yw_arg159 v18;
                v18.unit = 0;
                v18.field_4 = 10;
                v18.txt = get_lang_string(yw->string_pointers_p2, 259, "GAME SAVED OK.");
                v18.field_C = 0;

                call_method(yw->self_full, 159, &v18);
            }

            yw->field_2d90->field_40 = 0;
        }
    }
    else
    {
        if ( yw->field_2d90->field_40 != 3 )
        {
            yw->field_2d90->field_40 = 3;

            if ( yw->GameShell )
            {
                yw->field_1604 = sub_47B388(0, yw->GameShell->user_name) != 0;

                if ( ypaworld_func64__sub7__sub6__sub0(yw->field_2d90->levelID, yw->GameShell->user_name) )
                    yw->field_1608 = 1;
                else
                    yw->field_1608 = 0;
            }
        }
    }

    if ( ypaworld_func64__sub7__sub6__sub1() == &exit_menu )
    {
        int v7 = sub_4C885C();

        if ( v7 == 1 )
        {
            switch ( dword_5C8B78 )
            {
            case 8:
                yw->field_2d90->field_40 = 2;

                if ( yw->field_757E )
                    sub_47DB04(yw, 1);

                break;

            case 9:
                yw->field_2d90->field_40 = 6;
                break;

            case 10:
                yw->field_2d90->field_40 = 7;
                break;

            case 11:
                yw->field_2d90->field_40 = 4;
                break;

            case 13:
                yw->field_81AF = yw->field_81B3;
                yw->field_81B3 = NULL;
                break;

            default:
                break;
            }
        }
        else if ( v7 == 2 )
        {
            ypaworld_func64__sub7__sub6__sub2(yw, 0);
        }
    }

    if ( exit_menu.cmd_flag & 0x20 )
    {
        lstvw_update_input(yw, &exit_menu, inpt);
    }
    else
    {
        winp_131arg *winpt = &inpt->winp131arg;

        int a2 = 0;
        int a4 = 0;

        if ( !yw->field_1604 )
            a4 = 1024;

        if ( !yw->field_1608 )
            a4 |= 0x800;

        if ( yw->field_1b84->field_3D5 == 2 )
            a4 |= 0x200;

        if ( yw->field_757E )
            a4 |= 0xE00;

        if ( inpt->winp131arg.selected_btn == &exit_menu.frm_1 )
        {
            if ( yw->GameShell )
            {
                if ( inpt->winp131arg.selected_btnID >= 8 )
                {
                    if ( winpt->flag & 0x10 )
                        sub_423F74(&yw->GameShell->samples1_info, 3);
                }
            }

            switch ( inpt->winp131arg.selected_btnID )
            {
            case 7:
                if ( winpt->flag & 0x40 )
                    sub_449DE8(yw, get_lang_string(yw->string_pointers_p2, 766, "help\\l16.html"));

                sub_4811E8(yw, 0x30);
                break;

            case 8:
                if ( winpt->flag & 0x30 )
                    a2 |= 0x100;

                if ( winpt->flag & 0x40 )
                {
                    dword_5C8B78 = 8;

                    if ( sub_4C885C() != 3 )
                        sub_4C31C0(yw, &exit_menu);

                    const char *v13 = get_lang_string(yw->string_pointers_p2, 2480, "REALLY EXIT MISSION ?");
                    sb_0x4c87fc(yw, v13, &exit_menu);
                }
                break;

            case 9:
                if ( !(a4 & 0x200) )
                {
                    if ( winpt->flag & 0x30 )
                        a2 |= 0x200;

                    if ( winpt->flag & 0x40 )
                    {
                        dword_5C8B78 = 9;

                        if ( sub_4C885C() != 3 )
                            sub_4C31C0(yw, &exit_menu);

                        const char *v14 = get_lang_string(yw->string_pointers_p2, 2481, "REALLY SAVE GAME ?");
                        sb_0x4c87fc(yw, v14, &exit_menu);
                    }
                }
                break;

            case 10:
                if ( !(a4 & 0x400) )
                {
                    if ( winpt->flag & 0x30 )
                        a2 |= 0x400;

                    if ( winpt->flag & 0x40 )
                    {
                        dword_5C8B78 = 10;

                        if ( sub_4C885C() != 3 )
                            sub_4C31C0(yw, &exit_menu);

                        const char *v15 = get_lang_string(yw->string_pointers_p2, 2482, "REALLY LOAD GAME ?");
                        sb_0x4c87fc(yw, v15, &exit_menu);
                    }
                }
                break;

            case 11:
                if ( !(a4 & 0x800) )
                {
                    if ( winpt->flag & 0x30 )
                        a2 |= 0x800;

                    if ( winpt->flag & 0x40 )
                    {
                        dword_5C8B78 = 11;

                        if ( sub_4C885C() != 3 )
                            sub_4C31C0(yw, &exit_menu);

                        const char *v16 = get_lang_string(yw->string_pointers_p2, 2483, "REALLY RESTART MISSION ?");
                        sb_0x4c87fc(yw, v16, &exit_menu);
                    }
                }
                break;

            case 12:
                if ( winpt->flag & 0x30 )
                    a2 |= 0x1000;

                if ( winpt->flag & 0x40 )
                    sub_4C31C0(yw, &exit_menu);

                break;

            default:
                break;
            }
        }
        lstvw_update_input(yw, &exit_menu, inpt);
        lstvw_update(yw, &exit_menu);

        ypaworld_func64__sub7__sub6__sub3(yw, a2, a4);
    }
}




char * sub_4C8534(_NC_STACK_ypaworld *yw, char *cur, const char *a2)
{
    char *pcur = cur;

    fntcmd_store_u8(&pcur, 123);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    pcur = sub_451714(yw->tiles[0], pcur, a2, lstvw2.width - 2 * dword_5BAFA8, 32);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    return pcur;
}

char * ypaworld_func64__sub7__sub4__sub0__sub0(_NC_STACK_ypaworld *yw, char *cur, int a3)
{
    char *pcur = cur;

    fntcmd_store_u8(&pcur, 123);
    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    const char *v5 = get_lang_string(yw->string_pointers_p2, 2, "OK");


    int v8;

    if ( a3 & 0x100 )
    {
        fntcmd_set_flag(&pcur, 0x10);
        v8 = 8;
    }
    else
    {
        v8 = 3;
    }

    fntcmd_select_tileset(&pcur, v8);

    fntcmd_store_u8(&pcur, 98);

    pcur = sub_451714(yw->tiles[v8], pcur, v5, dword_5BAFA0 - 2 * yw->font_default_w__b, 32);

    fntcmd_unset_flag(&pcur, 0x10);

    fntcmd_store_u8(&pcur, 100);

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_op10(&pcur, dword_5BAFA4);

    fntcmd_store_u8(&pcur, 32);

    v5 = get_lang_string(yw->string_pointers_p2, 3, "CANCEL");

    if ( a3 & 0x200 )
    {
        fntcmd_set_flag(&pcur, 0x10);
        v8 = 8;
    }
    else
    {
        v8 = 3;
    }

    fntcmd_select_tileset(&pcur, v8);

    fntcmd_store_u8(&pcur, 98);

    pcur = sub_451714(yw->tiles[v8], pcur, v5, dword_5BAFA0 - 2 * yw->font_default_w__b, 32);

    fntcmd_unset_flag(&pcur, 0x10);

    fntcmd_store_u8(&pcur, 100);

    fntcmd_select_tileset(&pcur, 0);

    fntcmd_store_u8(&pcur, 32);
    fntcmd_store_u8(&pcur, 32);

    fntcmd_store_u8(&pcur, 125);

    fntcmd_next_line(&pcur);

    return pcur;
}

void ypaworld_func64__sub7__sub4__sub0(_NC_STACK_ypaworld *yw, int a2)
{
    char *pcur = lstvw_up_border(yw, &lstvw2, lstvw2.data_cmdbuf, 0, "{ }");

    fntcmd_set_txtColor(&pcur, yw->iniColors[60].r, yw->iniColors[60].g, yw->iniColors[60].b);

    pcur = sub_4C8534(yw, pcur, dword_5BAF98);
    pcur = sub_4C8534(yw, pcur, " ");

    fntcmd_set_txtColor(&pcur, yw->iniColors[68].r, yw->iniColors[68].g, yw->iniColors[68].b);

    pcur = ypaworld_func64__sub7__sub4__sub0__sub0(yw, pcur, a2);
    pcur = lstvw_down_border(yw, &lstvw2, pcur, 0, "xyz");

    fntcmd_set_end(&pcur);
}

void ypaworld_func64__sub7__sub4(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    if ( lstvw2.cmd_flag & 0x20 )
    {
        lstvw_update_input(yw, &lstvw2, inpt);
    }
    else
    {
        winp_131arg *winpt = &inpt->winp131arg;

        int a1_1 = 0;

        if ( yw->field_826F == VK_RETURN )
        {
            sub_4C31C0(yw, &lstvw2);

            dword_5BAF9C = 1;

            inpt->downed_key = 0;
            inpt->downed_key_2 = 0;
            inpt->dword8 = 0;
        }
        else if ( yw->field_826F == VK_ESCAPE )
        {
            sub_4C31C0(yw, &lstvw2);

            dword_5BAF9C = 2;

            inpt->downed_key = 0;
            inpt->downed_key_2 = 0;
            inpt->dword8 = 0;
        }
        else if ( inpt->winp131arg.selected_btn == &lstvw2.frm_1 )
        {
            if ( yw->GameShell )
            {
                if ( inpt->winp131arg.selected_btnID >= 8 )
                {
                    if ( winpt->flag & 0x10 )
                        sub_423F74(&yw->GameShell->samples1_info, 3);
                }
            }

            if ( winpt->selected_btnID == 8 )
            {
                if ( winpt->flag & 0x30 )
                    a1_1 = 0x100;

                if ( winpt->flag & 0x40 )
                {
                    sub_4C31C0(yw, &lstvw2);
                    dword_5BAF9C = 1;
                }
            }
            else if ( winpt->selected_btnID == 9 )
            {
                if ( winpt->flag & 0x30 )
                    a1_1 = 0x200;

                if ( winpt->flag & 0x40 )
                {
                    sub_4C31C0(yw, &lstvw2);
                    dword_5BAF9C = 2;
                }
            }
        }

        lstvw_update_input(yw, &lstvw2, inpt);
        lstvw_update(yw, &lstvw2);
        ypaworld_func64__sub7__sub4__sub0(yw, a1_1);
    }
}


void ypaworld_func64__sub7(_NC_STACK_ypaworld *yw, struC5 *inpt)
{
    winp_131arg *winpt = &inpt->winp131arg;

    if ( yw->field_1b84->field_24 != 4 )
    {
        if ( winpt->flag & 1 )
        {
            listbase *llst = (listbase *)yw->field_17a0.head;

            while(llst->next)
            {
                if ( llst->cmd_flag & 4 )
                {
                    if ( llst->cmd_flag & 1 )
                        llst->field_1BC[2] = 1;
                }

                if ( llst->cmd_flag & 0x10 )
                {
                    if ( !(llst->cmd_flag & 0x21) )
                        llst->cmd_flag &= 0xFFFFFFBF;
                }

                llst = (listbase *)llst->next;
            }

            if ( inpt->winp131arg.selected_btn )
            {
                listbase *v9 = (listbase *)inpt->winp131arg.selected_btn->pobject;
                if ( v9 )
                {
                    if ( v9->cmd_flag & 1 )
                    {
                        if ( winpt->flag & 0x30 )
                            v9->field_1BC[2] = 2;

                        if ( winpt->flag & 0x40 )
                        {
                            v9->cmd_flag &= 0xFFFFFFFE;

                            sub_412D9C(&v9->frm_2);
                            sub_412D48(&v9->frm_1, 0);

                            Remove(v9);

                            AddHead(&yw->field_17a0, v9);
                        }
                    }
                    else
                    {
                        if ( winpt->flag & 2 )
                        {
                            sub_412D9C(&v9->frm_1);
                            sub_412D48(&v9->frm_1, 0);

                            Remove(v9);

                            AddHead(&yw->field_17a0, v9);
                        }

                        if ( v9->cmd_flag & 0x10 )
                        {
                            if ( winpt->selected_btnID == 0)
                            {
                                if ( winpt->flag & 0x30 )
                                    v9->cmd_flag |= 0x40;

                                if ( winpt->flag & 0x10 )
                                {
                                    if ( yw->GameShell )
                                        sub_423F74(&yw->GameShell->samples1_info, 3);
                                }

                                if ( winpt->flag & 0x40 )
                                {
                                    sub_412D9C(&v9->frm_1);

                                    if ( v9->cmd_flag & 4 )
                                    {
                                        sub_412D48(&v9->frm_2, 0);

                                        v9->cmd_flag |= 1;
                                    }
                                    else
                                    {
                                        v9->cmd_flag |= 0x20;
                                    }

                                    Remove(v9);
                                    AddHead(&yw->field_17a0, v9);
                                }
                            }
                        }

                        if ( v9->cmd_flag & 8 )
                        {
                            if ( winpt->flag & 0x10 )
                            {
                                if ( winpt->selected_btnID == 1 )
                                {
                                    yw->field_17bc = 1;
                                    yw->field_17b4 = v9;
                                    yw->field_17b0 = 1;
                                    yw->field_17b8.x = winpt->ldw_pos[1].x;
                                    yw->field_17b8.y = winpt->ldw_pos[1].y;
                                }
                            }
                        }
                    }
                }
            }

            if ( yw->field_17b0 )
            {
                if ( winpt->flag & 4 )
                {
                    int v13 = winpt->move[0].x - yw->field_17b8.x;
                    int v15 = winpt->move[0].y - yw->field_17b8.y;

                    if ( v13 >= 0 )
                    {
                        if ( yw->field_17b4->frm_1.btn_width + v13 > yw->screen_width )
                            v13 = yw->screen_width - yw->field_17b4->frm_1.btn_width;
                    }
                    else
                    {
                        v13 = 0;
                    }

                    if ( v15 >= yw->icon_energy__h )
                    {
                        if ( yw->field_17b4->frm_1.btn_height + v15 > yw->screen_height - yw->icon0___h )
                            v15 = yw->screen_height - yw->icon0___h - yw->field_17b4->frm_1.btn_height;
                    }
                    else
                    {
                        v15 = yw->icon_energy__h;
                    }

                    yw->field_17b4->frm_1.btn_ypos = v15;
                    yw->field_17b4->frm_1.btn_xpos = v13;

                    char *tmp = &yw->field_17b4->cmdstrm.cmdbuf[5];
                    fntcmd_store_s16(&tmp, v13 - (yw->screen_width / 2)); //HACKY UPDATE!

                    tmp = &yw->field_17b4->cmdstrm.cmdbuf[9];
                    fntcmd_store_s16(&tmp, v15 - (yw->screen_height / 2)); //HACKY UPDATE!
                }
            }

            if ( yw->field_17b0 )
            {
                if ( !(winpt->flag & 4) )
                {
                    yw->field_17bc = 0;
                    yw->field_17b0 = 0;
                }
            }

            if ( yw->field_757E )
                ypaworld_func64__sub7__sub5(yw, inpt);

            ypaworld_func64__sub7__sub4(yw, inpt);
            ypaworld_func64__sub7__sub6(yw, inpt);
            ypaworld_func64__sub7__sub1(yw, inpt);
            ypaworld_func64__sub7__sub3(yw, inpt);
            ypaworld_func64__sub7__sub0(yw, inpt);
            ypaworld_func64__sub7__sub7(yw, inpt);
        }
        ypaworld_func64__sub7__sub2(yw, inpt);
    }
}










__NC_STACK_ypabact * sub_449518(_NC_STACK_ypaworld *yw)
{
    __NC_STACK_ypabact *v2 = NULL;

    if ( info_log.field_255C )
    {
        for (int i = 0; i < yw->field_2414; i++)
        {
            if ( v2 )
                break;

            if ( info_log.field_255C == yw->field_1c0c[i]->ypabact__id )
            {
                v2 = yw->field_1c0c[i];
            }
            else
            {
                bact_node *v5 = (bact_node *)yw->field_1c0c[i]->list2.head;
                while (v5->next)
                {
                    if ( v5->bact->field_3D5 != 2 && v5->bact->field_3D5 != 4 && v5->bact->field_3D5 != 5 && v5->bact->ypabact__id == info_log.field_255C )
                    {
                        v2 = v5->bact;
                        break;
                    }

                    v5 = (bact_node *)v5->next;
                }
            }
        }

        if ( v2 )
        {
            if ( (yw->field_1614 - info_log.field_2560) > 10000 )
                v2 = 0;
        }
    }
    return v2;
}

int sub_4C3E34(const void *a1, const void *a2)
{
    return ((* (__NC_STACK_ypabact **)a1)->field_2E & 0xFFF) - ((* (__NC_STACK_ypabact **)a2)->field_2E & 0xFFF);
}


void sub_4C40AC(_NC_STACK_ypaworld *yw)
{
    int v3 = 0;
    if ( yw->field_1b88 )
    {
        bact_node *v4 = (bact_node *)yw->field_1b88->tailpred;

        while (v4->prev)
        {
            if (v4->bact->field_3D5 != 2 && v4->bact->field_3D5 != 5 && v4->bact->field_24 != 9)
            {
                yw->field_1c0c[v3] = v4->bact;
                v3++;
            }

            v4 = (bact_node *)v4->prev;
        }
    }
    yw->field_2414 = v3;

    qsort(yw->field_1c0c, v3, sizeof(__NC_STACK_ypabact *), sub_4C3E34);

    if ( v3 )
    {
        if ( !yw->field_240c )
        {
            if ( bzda.field_1D0 & 6 )
            {
                yw->field_240c = yw->field_1c0c[v3 - 1]->field_2E;
                yw->field_2410 = v3 - 1;
            }
        }
        else
        {
            int v10 = 0;

            for (v10 = 0; v10 < v3; v10++)
            {
                if (yw->field_240c == yw->field_1c0c[v10]->field_2E)
                {
                    yw->field_2410 = v10;
                    break;
                }
            }

            if ( v10 == v3 )
                yw->field_2410 = -1;
        }
    }
    else
    {
        yw->field_2410 = -1;
    }

    if ( yw->field_2410 == -1 )
    {
        yw->field_2418 = 0;
    }
    else
    {
        bact_node *tmpnd = (bact_node *)yw->field_1c0c[ yw->field_2410 ]->list2.tailpred;
        int i = 0;

        while (tmpnd->prev)
        {
            i++;
            tmpnd = (bact_node *)tmpnd->prev;
        }

        yw->field_2418 = i;
    }
    yw->field_2420 = sub_449518(yw);
}

void ypaworld_func64__sub14(_NC_STACK_ypaworld *yw)
{
    if ( !(robo_map.cmd_flag & 0x20) )
    {
        sub_4C157C(yw);
        sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
        sub_4C0C00(yw);
        sub_4C0FEC(yw);
        sub_4C1214(yw);

        stru_5B25D8.xpos = robo_map.field_244;
        stru_5B25D8.ypos = robo_map.field_23C;
        stru_5B25D8.width = robo_map.frm_1.btn_width - robo_map.field_24C;
        stru_5B25D8.fnt_height = robo_map.frm_1.btn_height - robo_map.field_250;

        int v2 = robo_map.frm_1.btn_xpos + robo_map.field_244 - (yw->screen_width / 2);

        sb_0x4f6650(yw, t1_cmdbuf_3, v2, robo_map.frm_1.btn_ypos + robo_map.field_23C - yw->screen_height / 2 );
    }
}

void ypaworld_func64__sub8(NC_STACK_ypaworld *, _NC_STACK_ypaworld *yw)
{
    if ( !(robo_map.cmd_flag & 0x20) )
    {
        if ( !(robo_map.field_1E8 & 0x16) && robo_map.field_1ED == 1 )
        {
            if ( yw->current_bact )
            {
                robo_map.field_1D8 = yw->current_bact->field_621.sx;
                robo_map.field_1DC = yw->current_bact->field_621.sz;

                sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
            }
        }
        robo_map.field_1E8 &= 0x1F;
    }
}



int sub_4C3C88(const void *a1, const void *a2)
{
    uint8_t aa1 = *(const uint8_t *)a1;
    uint8_t aa2 = *(const uint8_t *)a2;

    VhclProto *v4 = &dword_5BAA60->VhclProtos[aa1];
    VhclProto *v5 = &dword_5BAA60->VhclProtos[aa2];

    int v7, v8;

    switch ( v4->model_id - 1 )
    {
    case 0:
        v7 = 1;
        break;

    case 1:
    case 7:
        v7 = 0;
        break;

    case 2:
        v7 = 5;
        break;

    case 5:
        v7 = 2;
        break;

    case 6:
        v7 = 3;
        break;

    case 8:
        v7 = 4;
        break;

    case 3:
    case 4:
        v7 = 10;
        break;

    default:
        v7 = 0;
        break;
    }

    switch ( v5->model_id - 1 )
    {
    case 0:
        v8 = 1;
        break;

    case 1:
    case 7:
        v8 = 0;
        break;

    case 2:
        v8 = 5;
        break;

    case 5:
        v8 = 2;
        break;

    case 6:
        v8 = 3;
        break;

    case 8:
        v8 = 4;
        break;

    case 3:
    case 4:
        v8 = 10;
        break;

    default:
        v8 = 0;
        break;
    }

    if ( v7 >= v8 )
    {
        if ( v7 <= v8 )
        {
            if ( v4->energy >= v5->energy )
                return v4->energy > v5->energy;
            else
                return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

void ypaworld_func64__sub16(_NC_STACK_ypaworld *yw)
{
    int v6 = 0;

    if ( yw->field_1b80 )
        v6 = 1 << yw->field_1b80->owner;

    int v3 = 0;

    for (int i = 0; i < 256; i++)
    {
        if ( v6 & yw->VhclProtos[i].disable_enable_bitmask )
        {
            bzda.field_2DC[v3] = i;
            v3++;
        }
    }

    bzda.field_8E0 = v3;

    dword_5BAA60 = yw;

    qsort(bzda.field_2DC, v3, 1, sub_4C3C88);

    if ( !v3 )
    {
        bzda.field_8EC = -1;
    }
    else
    {
        if ( v3 == 1 )
        {
            bzda.field_8EC = 0;
        }
        else if ( v3 <= bzda.field_8EC )
        {
            bzda.field_8EC = v3 - 1;
        }
    }
}

int sub_4C3D6C(const void *a1, const void *a2)
{

    uint8_t aa1 = *(const uint8_t *)a1;
    uint8_t aa2 = *(const uint8_t *)a2;

    BuildProto *v3 = &dword_5BAA60->BuildProtos[aa1];
    BuildProto *v4 = &dword_5BAA60->BuildProtos[aa2];

    int v6, v8;

    if ( v3->model_id == 1 )
        v6 = 0;
    else if ( v3->model_id == 2 )
        v6 = 2;
    else if ( v3->model_id == 3 )
        v6 = 1;
    else
        v6 = 10;

    if ( v4->model_id == 1 )
        v8 = 0;
    else if ( v4->model_id == 2 )
        v8 = 2;
    else if ( v4->model_id == 3 )
        v8 = 1;
    else
        v8 = 10;

    if ( v6 >= v8 )
    {
        if ( v6 <= v8 )
        {
            if ( v3->energy >= v4->energy )
                return v3->energy > v4->energy;
            else
                return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}


void ypaworld_func64__sub17(_NC_STACK_ypaworld *yw)
{
    int v6 = 0;

    if ( yw->field_1b80 )
        v6 = 1 << yw->field_1b80->owner;

    int v3 = 0;

    for (int i = 0; i < 128; i++)
    {
        if ( v6 & yw->BuildProtos[i].enable_mask )
        {
            bzda.field_3DC[v3] = i;
            v3++;
        }
    }

    bzda.field_8E4 = v3;

    dword_5BAA60 = yw;

    qsort(bzda.field_3DC, v3, 1, sub_4C3D6C);

    if ( !v3 )
    {
        bzda.field_8F4 = -1;
    }
    else
    {
        if ( v3 == 1 )
        {
            bzda.field_8F4 = 0;
        }
        else if ( v3 <= bzda.field_8F4 )
        {
            bzda.field_8F4 = v3 - 1;
        }
    }
}

void ypaworld_func64__sub15(_NC_STACK_ypaworld *yw)
{
    roboGun *a4;
    call_vtbl(yw->field_1b78, 3, 0x8000200E, &a4, 0);

    int v4 = 0;

    for(int i = 0; i < 8 ; i++ )
    {
        if ( a4[i].gun_obj )
        {
            bzda.field_1DC[v4] = i;

            if ( a4[i].gun_obj == yw->field_1b7c )
                bzda.field_8F0 = v4;

            v4++;
        }
    }

    bzda.field_8DC = v4;

    if ( !v4 )
    {
        bzda.field_8F0 = -1;
    }
    else
    {
        if ( v4 == 1 )
        {
            bzda.field_8F0 = 0;
        }
        else if ( v4 <= bzda.field_8F0 )
        {
            bzda.field_8F0 = v4 - 1;
        }
    }
}



void ypaworld_func159__real(NC_STACK_ypaworld *obj, class_stru *zis, yw_arg159 *arg)
{
    _NC_STACK_ypaworld *yw = &obj->stack__ypaworld;

    //if ( arg->field_C )
    //  ypaworld_func159__sub0(yw, arg->unit, arg->field_4, arg->field_C);

    if ( arg->unit )
        info_log.field_255C = arg->unit->ypabact__id;
    else
        info_log.field_255C = 0;

    info_log.field_2560 = yw->field_1614;
    info_log.field_2564 = arg->field_C;

    if ( arg->txt )
    {
        inflog_msg *v6;

        if ( info_log.field_250 >= 5 )
        {
            info_log.msg_count++;

            if ( info_log.msg_count >= 64 )
                info_log.msg_count = 0;

            if ( info_log.field_254 == info_log.msg_count )
            {
                info_log.field_254++;

                if ( info_log.field_254 >= 64 )
                    info_log.field_254 = 0;
            }

            info_log.window.elements_for_scroll_size++;;

            if ( info_log.window.elements_for_scroll_size > 64 )
                info_log.window.elements_for_scroll_size = 64;

            v6 = &info_log.msgs[info_log.msg_count];

            info_log.field_24C = info_log.msg_count;
        }
        else
        {
            info_log.msg_count = info_log.field_24C;

            v6 = &info_log.msgs[info_log.field_24C];
        }

        info_log.field_256C = 5000;
        info_log.field_2568 = 1;
        info_log.field_250 = arg->field_4;

        if ( arg->unit )
            v6->id = arg->unit->ypabact__id;
        else
            v6->id = 0;

        v6->field_8 = 7000;
        v6->field_4 = yw->field_1614;

        const char *v5 = arg->txt;

        int v10 = 0;

        while ( *v5 )
        {
            if ( *v5 == '\n' )
            {
                v6->txt[v10] = 0;

                v10 = 0;

                info_log.msg_count++;

                if ( info_log.msg_count >= 64 )
                    info_log.msg_count = 0;

                if ( info_log.field_254 == info_log.msg_count )
                {
                    info_log.field_254++;

                    if ( info_log.field_254 >= 64 )
                        info_log.field_254 = 0;
                }

                info_log.window.elements_for_scroll_size++;

                if ( info_log.window.elements_for_scroll_size > 64 )
                    info_log.window.elements_for_scroll_size = 64;

                info_log.field_256C += 5000;
                info_log.field_2568++;

                v6 = &info_log.msgs[ info_log.msg_count ];

                if ( arg->unit )
                    v6->id = arg->unit->ypabact__id;
                else
                    v6->id = 0;

                v6->field_8 = 7000;
                v6->field_4 = 0;
            }
            else if ( v10 < 125 )
            {
                v6->txt[v10] = *v5;
                v10++;
            }

            v5++;
        }

        v6->txt[v10] = 0;

        info_log.window.scroll_pos = info_log.window.elements_for_scroll_size - info_log.window.element_count;

        if ( info_log.window.scroll_pos < 0 )
            info_log.window.scroll_pos = 0;
    }
}

void sb_0x4d7c08__sub0__sub0(_NC_STACK_ypaworld *yw)
{
    char *pcur = byte_5A7650;

    char a1a[512];

    if ( yw->field_757E )
    {
        if ( yw->field_81CB.field_0 )
        {
            const char *v5;
            int v6;

            if ( yw->field_81CB.field_0 == 1 )
            {
                v5 = get_lang_string(yw->string_pointers_p2, 2468, "2468 == *** VICTORY IS YOURS ***");
                sprintf(a1a,  v5 );

                v6 = 40000;
            }
            else if ( yw->field_81CB.field_0 > 1 && yw->field_81CB.field_0 <= 4 )
            {
                v5 = get_lang_string(yw->string_pointers_p2, 2469, "2469 == *** %s HAS BEEN DEFEATED ***");
                sprintf(a1a, v5, yw->field_81CB.field_8);

                v6 = 20000;
            }
            else
            {
                v6 = 0;
                v5 = NULL;
            }

            if ( v5 && yw->field_1614 - yw->field_81CB.field_4 < v6 )
            {
                if ( yw->field_1614 / 300 & 1 )
                {
                    fntcmd_select_tileset(&pcur, 15);
                    fntcmd_set_xpos(&pcur, 0);
                    fntcmd_set_ypos(&pcur, yw->screen_height / 3);

                    fntcmd_set_txtColor(&pcur, 255, 255, 255);

                    pcur = sub_45148C(yw->tiles[15], pcur, a1a, yw->screen_width);
                }
            }
            else
            {
                yw->field_81CB.field_0 = 0;
            }
        }
    }

    if ( sb_0x4d7c08__sub0__sub0__sub0(yw) )
        return;

    int v10;

    if ( info_log.field_254 <= info_log.msg_count )
        v10 = info_log.msg_count - info_log.field_254;
    else
        v10 = info_log.msg_count + 64 - info_log.field_254;

    if ( v10 > 10 )
        v10 = 10;

    fntcmd_select_tileset(&pcur, 15);
    fntcmd_set_ypos(&pcur, up_panel.field_1CC + yw->font_default_h / 2);


    int v11 = info_log.msg_count - v10;
    int v28 = 0;
    int v27 = 0;

    while ( 1 )
    {
        if ( v11 >= 64 )
            v11 = 0;

        inflog_msg *v14 = &info_log.msgs[v11];

        if ( v14->field_8 > 0 )
        {
            if ( !v27 )
            {
                v27 = 1;

                int v15;

                if ( v11 > info_log.msg_count )
                    v15 = info_log.msg_count + 64 - v11;
                else
                    v15 = info_log.msg_count - v11;

                v15 = (v15 + 1) / 2;

                if ( !v15 )
                    v15 = 1;

                v14->field_8 -= yw->field_1618 * v15;
            }

            if ( v14->field_4 )
                v28 = v14->field_4;
            else
                v28 += 200;

            int v18 = yw->field_1614 - v28;

            int v19;

            if ( v18 < 200 )
            {
                v19 = 100 * v18 / 200;

                if ( v19 < 0 )
                    v19 = 0;
                else if ( v19 > 100 )
                    v19 = 100;
            }
            else
            {
                v19 = 100;
            }

            if ( v19 > 0 )
            {
                fntcmd_set_xpos(&pcur, 16);
                fntcmd_set_txtColor(&pcur, yw->iniColors[64].r, yw->iniColors[64].g, yw->iniColors[64].b);
                // Output ingame messages. From analyzer and other.
                pcur = sub_451E64(yw->tiles[15], pcur, v14->txt, v19, 4);

                fntcmd_next_line(&pcur);
            }
        }

        if ( v11 == info_log.msg_count )
            break;

        v11++;
    }

    fntcmd_set_end(&pcur);

    w3d_a209 arg;

    arg.cmdbuf = byte_5A7650;
    arg.includ = 0;

    sub_423288(&arg);
}



void wis_color(_NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, uint32_t *out1, uint32_t *out2)
{
    float v7 = yw->wis_skeletons.field_86 * 2.0 + -1.5;

    float v18 = fabs(v7 - x1);
    float v17 = fabs(v7 - x2);

    if ( v18 > 0.5 )
        v18 = 0.5;

    if ( v17 > 0.5 )
        v17 = 0.5;

    float v8 = v18 * 2.0;
    int v9 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v8 + yw->wis_skeletons.cl1_g;
    int v10 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v8 + yw->wis_skeletons.cl1_r;
    int v11 = (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v8 + yw->wis_skeletons.cl1_b;

    *out1 = ((v10 & 0xFF) << 16) | ((v9 & 0xFF) << 8) | (v11 & 0xFF);

    float v12 = 2.0 * v17;
    int v13 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v12 + yw->wis_skeletons.cl1_g;
    int v14 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v12 + yw->wis_skeletons.cl1_r;
    int v15 = (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v12 + yw->wis_skeletons.cl1_b;
    *out2 = ((v13 & 0xFF) << 16) | ((v14 & 0xFF) << 8) | (v15 & 0xFF);
}




void sub_4E332C(_NC_STACK_ypaworld *yw, skeleton_64_stru *wire, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, uint32_t coloooor, wis_color_func color_func, wis_color_func color_func2)
{
    if ( wire )
    {
        rstr_arg217 v30;
        v30.dword0 = coloooor;
        v30.dword4 = coloooor;
        v30.dword8 = 0xFFFFFFFF;

        call_method(yw->win3d, 217, &v30);

        for (int i = 0; i < wire->POO_NUM; i++)
        {
            float tmpx = wire->POO[i].pos3f.sx * 0.001;
            float tmpz = -wire->POO[i].pos3f.sz * 0.001;

            wire->type2[i].pos3f.sx = (tmpx * a5 + tmpz * a6) * a9 + a3;
            wire->type2[i].pos3f.sy = (tmpx * a7 + tmpz * a8) * a10 + a4;
        }

        for (int i = 0; i < wire->pol_count; i++)
        {
            pol_indixes *v20 = wire->pol_entries[i];

            if (v20->num_vertices >= 2)
            {
                int16_t *idxs = &v20->v1;

                w3d_func198arg v29;
                v29.x1 = wire->type2[ idxs[0] ].pos3f.sx;
                v29.y1 = wire->type2[ idxs[0] ].pos3f.sy;
                v29.x2 = wire->type2[ idxs[1] ].pos3f.sx;
                v29.y2 = wire->type2[ idxs[1] ].pos3f.sy;

                if ( color_func )
                {
                    uint32_t v32 = coloooor;
                    uint32_t v31 = coloooor;

                    color_func(yw,  v29.x1 - a3,   v29.y1 - a4,   v29.x2 - a3,   v29.y2 - a4, &v32, &v31);

                    v30.dword0 = v32;
                    v30.dword4 = v31;
                    v30.dword8 = 0xFFFFFFFF;

                    call_method(yw->win3d, 217, &v30);
                }
                else if ( color_func2 )
                {
                    uint32_t v34 = coloooor;
                    uint32_t v33 = coloooor;

                    color_func2(yw, v29.x1, v29.y1, v29.x2, v29.y2, &v34, &v33);

                    v30.dword0 = v34;
                    v30.dword4 = v33;
                    v30.dword8 = 0xFFFFFFFF;

                    call_method(yw->win3d, 217, &v30);
                }

                call_method(yw->win3d, 200, &v29);
            }
        }
    }
}

void yw_draw_vhcl_wireframe(_NC_STACK_ypaworld *yw, sklt_wis *wis, VhclProto *vhcl, float a4, float a5, float a6)
{
    uint32_t color_25 = yw_GetColor(yw, 25);
    uint32_t color_34 = yw_GetColor(yw, 34);

    skeleton_64_stru *wairufureimu = NULL;

    wis_color_func func = NULL;

    if ( wis->field_72 )
        func = wis_color;

    if ( vhcl->wireframe )
        call_vtbl(vhcl->wireframe, 3, 0x80002000, &wairufureimu, 0);

    if ( wairufureimu )
    {
        float v15 = wis->field_92 * 6.0 * 0.8;

        float a9 = 0.1;

        if ( a6 < 1.4 )
            a9 = a6 * 0.1;

        yw->wis_skeletons.cl1_r = (color_34 >> 16) & 0xFF;
        yw->wis_skeletons.cl1_g = (color_34 >> 8) & 0xFF;
        yw->wis_skeletons.cl1_b = color_34 & 0xFF;
        yw->wis_skeletons.cl2_r = (color_25 >> 16) & 0xFF;
        yw->wis_skeletons.cl2_b = color_25 & 0xFF;
        yw->wis_skeletons.cl2_g = (color_25 >> 8) & 0xFF;

        sub_4E332C(yw, wairufureimu, a4, a5, 1.0, 0.0, 0.0, 1.0, a9, v15, color_34, NULL, func);
    }
}



char * sub_4E4F80(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float xpos, float ypos, int value, int maxval, int a8, int a9, const char *a10, const char *a11)
{
    int wnd_xpos = 0;
    int wnd_xpos2 = 0;
    int wnd_ypos = 0;
    int wnd_ypos2 = 0;

    int wnd_vis = 0;

    if ( !(robo_map.cmd_flag & 0x20) )
    {
        wnd_xpos = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
        wnd_xpos2 = robo_map.frm_1.btn_width + wnd_xpos;
        wnd_ypos = robo_map.frm_1.btn_ypos - (yw->screen_height / 2);
        wnd_ypos2 = robo_map.frm_1.btn_height + wnd_ypos;

        wnd_vis = 1;
    }

    int v51 = (yw->screen_width / 2) * xpos;
    int v50 = (yw->screen_height / 2) * ypos;

    int v49 = yw->tiles[51]->chars[a8].width * wis->field_9E;


    char *pcur = cur;

    fntcmd_set_txtColor(&pcur, yw->iniColors[65].r, yw->iniColors[65].g, yw->iniColors[65].b);

    v51 -= (wis->field_9A + wis->field_96 + v49) / 2;

    if ( a10 )
    {
        int v19 = (wis->field_96 / 2) + v51;
        int v20 = v50;

        if ( !wnd_vis || v19 <= wnd_xpos || v19 >= wnd_xpos2 || v20 <= wnd_ypos || v20 >= wnd_ypos2 )
        {
            fntcmd_select_tileset(&pcur, 15);

            fntcmd_set_center_xpos(&pcur, v51);
            fntcmd_set_center_ypos(&pcur, v50 - (yw->font_default_h / 2));

            pcur = sub_451E64(yw->tiles[15], pcur, a10, 100, 4);
        }
    }

    fntcmd_select_tileset(&pcur, 51);

    v51 += wis->field_96;

    fntcmd_set_center_xpos(&pcur, v51);
    fntcmd_set_center_ypos(&pcur, v50 - (yw->tiles[51]->font_height / 2));

    int v29 = v51 + (yw->tiles[51]->chars[1].width / 2);
    int v30 = v50 + (yw->tiles[51]->font_height >> 1);

    for (int i = 1; i <= wis->field_9E; i++)
    {
        int v35 = i * (maxval / wis->field_9E) - (maxval / (wis->field_9E / 2));

        if ( !wnd_vis || v29 <= wnd_xpos || v29 >= wnd_xpos2 || v30 <= wnd_ypos || v30 >= wnd_ypos2 )
        {
            if ( v35 > value )
                fntcmd_store_u8(&pcur, a9);
            else
                fntcmd_store_u8(&pcur, a8);
        }

        v29 += yw->tiles[51]->chars[1].width;
    }

    v51 += wis->field_9A + v49;

    if ( a11 )
    {
        int v37 = v51 - (wis->field_9A / 2);
        int v38 = v50;

        if ( !wnd_vis || v37 <= wnd_xpos || v37 >= wnd_xpos2 || v38 <= wnd_ypos || v38 >= wnd_ypos2 )
        {
            fntcmd_select_tileset(&pcur, 15);

            fntcmd_set_center_xpos(&pcur, v51);
            fntcmd_set_center_ypos(&pcur, v50 - (yw->font_default_h / 2));

            pcur = sub_451E64(yw->tiles[15], pcur, a11, 100, 8);
        }
    }

    return pcur;
}

char * sb_0x4e5a84__sub2(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, __NC_STACK_ypabact *bact, VhclProto *vhcl, float xpos, float ypos)
{
    int a6a;
    int v10;

    if ( bact )
    {
        a6a = bact->energy;
        v10 = bact->energy_2;
    }
    else
    {
        v10 = vhcl->energy;
        a6a = vhcl->energy;
    }

    char a1a[32];
    sprintf(a1a, "%d", (a6a + 99) / 100);

    const char *v11 = get_lang_string(yw->string_pointers_p2, 35, "HP");

    return sub_4E4F80(yw, wis, cur, xpos, ypos, a6a, v10, 2, 6, v11, a1a);
}


char * sb_0x4e5a84__sub3(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, __NC_STACK_ypabact *bact, VhclProto *vhcl, float xpos, float ypos)
{
    int v10;

    if ( bact )
        v10 = bact->shield;
    else
        v10 = vhcl->shield;

    char a1a[32];
    sprintf(a1a, "%d%%", v10);

    const char *v11 = get_lang_string(yw->string_pointers_p2, 36, "AMR");

    return sub_4E4F80(yw, wis, cur, xpos, ypos, v10, 100, 1, 5, v11, a1a);
}



char * sub_4E5698(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, const char *name, float xpos, float ypos)
{
    char *pcur = cur;

    int v29 = (yw->screen_width / 2) * xpos;
    int v33 = (yw->screen_height / 2) * ypos;

    int v31 = yw->screen_width * wis->field_8A;

    uint32_t v21 = yw_GetColor(yw, 25);
    uint32_t a11 = yw_GetColor(yw, 34);

    wis_color_func a13;

    if ( wis->field_72 )
        a13 = wis_color;
    else
        a13 = NULL;

    int wnd_xpos = 0;
    int wnd_xpos2 = 0;
    int wnd_ypos = 0;
    int wnd_ypos2 = 0;

    int wnd_vis = 0;

    if ( !(robo_map.cmd_flag & 0x20) )
    {
        wnd_xpos = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
        wnd_xpos2 = robo_map.frm_1.btn_width + wnd_xpos;
        wnd_ypos = robo_map.frm_1.btn_ypos - (yw->screen_height / 2);
        wnd_ypos2 = robo_map.frm_1.btn_height + wnd_ypos;

        wnd_vis = 1;
    }

    int v30 = v29 - (v31 / 2);
    int v34 = v33 - (yw->font_default_h / 2);

    if ( !wnd_vis || v29 <= wnd_xpos || v29 >= wnd_xpos2 || v33 <= wnd_ypos || v33 >= wnd_ypos2 )
    {
        fntcmd_select_tileset(&pcur, 15);

        fntcmd_set_center_xpos(&pcur, v30);
        fntcmd_set_center_ypos(&pcur, v34);

        fntcmd_set_txtColor(&pcur,  yw->iniColors[65].r,  yw->iniColors[65].g,  yw->iniColors[65].b);

        pcur = sub_451714(yw->tiles[15], pcur, name, v31, 32);
    }

    if ( wis->sklts_intern[2] )
    {
        yw->wis_skeletons.cl1_r = (a11 >> 16) & 0xFF;
        yw->wis_skeletons.cl1_g = (a11 >> 8) & 0xFF;
        yw->wis_skeletons.cl1_b = a11 & 0xFF;
        yw->wis_skeletons.cl2_r = (v21 >> 16) & 0xFF;
        yw->wis_skeletons.cl2_b = v21 & 0xFF;
        yw->wis_skeletons.cl2_g = (v21 >> 8) & 0xFF;

        float a4 = (1.0 / (float)yw->screen_height) * 2.0 + ypos;

        float a10 = wis->field_92 * 1.2;

        sub_4E332C(yw, wis->sklts_intern[2], xpos, a4, 1.0, 0.0, 0.0, 1.0, wis->field_8A, a10, a11, NULL, a13);
    }

    return pcur;
}


void sb_0x4e5a84__sub1(_NC_STACK_ypaworld *yw, sklt_wis *wis, WeapProto *wpn, float xpos, float ypos)
{
    skeleton_64_stru *wairufureimu = NULL;

    wis_color_func func;

    if ( wis->field_72 )
        func = wis_color;
    else
        func = NULL;

    if ( wpn )
    {
        if ( wpn->wireframe )
            call_vtbl(wpn->wireframe, 3, 0x80002000, &wairufureimu, 0);

        if ( wairufureimu )
        {
            uint32_t v9 = yw_GetColor(yw, 20);
            uint32_t v10 = yw_GetColor(yw, 29);

            yw->wis_skeletons.cl1_r = (v10 >> 16) & 0xFF;
            yw->wis_skeletons.cl1_g = (v10 >> 8) & 0xFF;
            yw->wis_skeletons.cl1_b = v10 & 0xFF;
            yw->wis_skeletons.cl2_r = (v9 >> 16) & 0xFF;
            yw->wis_skeletons.cl2_b = v9 & 0xFF;
            yw->wis_skeletons.cl2_g = (v9 >> 8) & 0xFF;

            sub_4E332C(yw, wairufureimu, xpos, ypos, 1.0, 0.0, 0.0, 1.0, 0.0415, 0.05, v10, NULL, func);
        }
    }
}

char * sb_0x4e5a84__sub4(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, __NC_STACK_ypabact *bact, WeapProto *wpn, float xpos, float ypos)
{
    char *pcur = cur;

    if ( wpn )
    {
        int v12;

        if ( bact )
        {

            int v10 = wpn->shot_time_user;

            if ( wpn->salve_shots )
            {
                if ( bact->field_9A5 >= wpn->salve_shots )
                    v10 = wpn->salve_delay;
            }

            v12 = 100 * (bact->field_915 - bact->field_97D) / v10;

            if ( v12 >= 100 )
                v12 = 100;
        }
        else
            v12 = 100;

        char a1a[32];

        if ( v12 == 100 )
            sprintf(a1a, get_lang_string(yw->string_pointers_p2, 34, "OK"));
        else
            sprintf(a1a, "%d%%", v12);

        const char *v14 = get_lang_string(yw->string_pointers_p2, 33, "RLD");
        pcur = sub_4E4F80(yw, wis, pcur, xpos, ypos, v12, 100, 1, 3, v14, a1a);
    }
    return pcur;
}

char * sb_0x4e5a84__sub5(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, __NC_STACK_ypabact *bact, VhclProto *vhcl, WeapProto *weap, float xpos, float ypos)
{
    char *pcur = cur;

    if ( weap )
    {
        char buf[64];

        if ( vhcl->num_weapons <= 1 )
            sprintf(buf, "%d", weap->energy / 100);
        else
            sprintf(buf, "%d x%d", weap->energy / 100, vhcl->num_weapons);

        const char *v12 = get_lang_string(yw->string_pointers_p2, 32, "DMG");

        pcur = sub_4E4F80(yw, wis, pcur, xpos, ypos, weap->energy, 100, 7, 7, v12, buf);
    }

    return pcur;
}


char * sb_0x4e5a84(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float xpos, float ypos, __NC_STACK_ypabact *bact, int vhclid, int flag)
{
    char *pcur = cur;
//  yw_GetColor(yw, 25);
//  yw_GetColor(yw, 34);
    int v23 = 1;
    int v11 = 1;
    int v22 = 1;
    int v25 = 1;

    if ( flag & 1 )
        v23 = ((yw->field_1614 / 200) & 1) != 0;

    if ( flag & 2 )
        v11 = ((yw->field_1614 / 200) & 1) != 0;

    if ( flag & 4 )
        v22 = ((yw->field_1614 / 200) & 1) != 0;

    if ( flag & 8 )
        v25 = ((yw->field_1614 / 200) & 1) != 0;

    float a6a;

    if ( flag & 0x10 )
    {
        a6a = (yw->field_1614 - wis->field_76 - 200) / 180.0;

        if ( a6a <= 0.0 )
            return pcur;
    }
    else
    {
        a6a = 1.0;
    }

    if ( vhclid == -1 )
        vhclid = bact->id;

    VhclProto *vhcl = &yw->VhclProtos[vhclid];


    WeapProto *weap;

    if ( vhcl->weapon == -1 )
        weap = NULL;
    else
        weap = &yw->WeaponProtos[vhcl->weapon];

    if ( v25 )
        yw_draw_vhcl_wireframe(yw, wis, vhcl, xpos, ypos, a6a);

    if ( v11 )
        pcur = sb_0x4e5a84__sub2(yw, wis, pcur, bact, vhcl, xpos,   wis->field_92 * 7.0 + ypos  );

    if ( v22 )
        pcur = sb_0x4e5a84__sub3(yw, wis, pcur, bact, vhcl, xpos,   wis->field_92 * 9.0 + ypos  );

    if ( v25 )
    {
        if ( vhcl->model_id != 9 )
        {
            float v15 = wis->field_92 * 12.0 + ypos;
            const char *v16 = get_lang_string(yw->string_pointers_p2, vhclid + 1200, vhcl->name);

            pcur = sub_4E5698(yw, wis, pcur, v16, xpos, v15);
        }
    }

    if ( weap )
    {
        if ( v23 )
        {
            sb_0x4e5a84__sub1(yw, wis, weap, xpos,   ypos - wis->field_92 * 9.0);

            pcur = sb_0x4e5a84__sub4(yw, wis, pcur, bact, weap, xpos,  ypos - wis->field_92 * 7.0);

            pcur = sb_0x4e5a84__sub5(yw, wis, pcur, bact, vhcl, weap, xpos,  ypos - wis->field_92 * 9.0);
        }
    }

    return pcur;
}

char *sb_0x4d7c08__sub0__sub0__sub0__sub0(_NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float a4, float a5, int a6, int a7)
{
    char *pcur = cur;
    //yw_GetColor(yw, 25);
    //yw_GetColor(yw, 34);

    if ( !(a7 & 0x10) || (yw->field_1614 - wis->field_76 - 200) / 180.0 > 0.0 )
    {
        if ( yw->field_1614 / 200 & 1 )
        {
            int v11;
            if ( yw->field_757E )
                v11 = a6 + 1700;
            else
                v11 = a6 + 1500;

            const char *v12 = get_lang_string(yw->string_pointers_p2, v11, yw->BuildProtos[a6].name);

            pcur = sub_4E5698(yw, wis, pcur, v12, a4, a5);
        }
    }
    return pcur;
}

int sb_0x4d7c08__sub0__sub0__sub0(_NC_STACK_ypaworld *yw)
{
    //Tech update draw
    ua_fRect v12;
    v12.x1 = -1.0;
    v12.x2 = 1.0;
    v12.y1 = -1.0;
    v12.y2 = 1.0;

    call_method(yw->win3d, 210, &v12);

    sklt_wis *wis = &yw->wis_skeletons;

    char *pcur = byte_5C8DB0;

    if ( yw->field_2b78 == -1 || yw->field_1614 - yw->field_2b7c >= 10000 || (!yw->last_modify_vhcl && !yw->last_modify_weapon && !yw->last_modify_build) )
        return 0;

    int a8 = 0;
    int v6 = yw->last_modify_vhcl;
    int v4 = yw->last_modify_weapon;
    int v14 = yw->last_modify_build;

    if ( !v6 )
    {
        if ( v4 )
        {
            for (int i = 0; i <= 256; i++)
            {
                if (i == 256)
                    return 0;

                if ( yw->VhclProtos[i].weapon == v4)
                {
                    v6 = i;
                    break;
                }
            }
        }
    }

    wis->field_86 = 1.0;

    int v10 = yw->gems[ yw->field_2b78 ].type;

    if ( v10 == 25 )
        a8 |= 1;
    else if ( v10 == 26 )
        a8 |= 4;
    else if ( v10 == 27 )
        a8 |= 8;

    if ( v6 )
        pcur = sb_0x4e5a84(yw, wis, pcur, 0.0, -0.5, 0, v6, a8);
    else if ( v14 )
        pcur = sb_0x4d7c08__sub0__sub0__sub0__sub0(yw, wis, pcur, 0.0, -0.5, v14, a8);

    fntcmd_set_end(&pcur);

    w3d_a209 v13;
    v13.cmdbuf = byte_5C8DB0;
    v13.includ = NULL;

    call_method(yw->win3d, 209, &v13);

    return 1;
}




void wis_color2(_NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, uint32_t *out1, uint32_t *out2)
{
    float v7 = yw->wis_skeletons.field_86 * 6.28;

    float v24 = cos(v7);
    float v22 = sin(v7);

    float v23 = sqrt(POW2(x1) + POW2(y1));
    if ( v23 > 0.0 )
    {
        float v9 = 1.0 / v23;
        x1 *= v9;
        y1 *= v9;
    }

    float v21 = sqrt(POW2(x2) + POW2(y2));
    if ( v21 > 0.0 )
    {
        float v11 = 1.0 / v21;
        x2 *= v11;
        y2 *= v11;
    }

    float v12 = (v24 * x1 + v22 * y1 + 1.0) * 0.5;
    int v13 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v12 + yw->wis_skeletons.cl1_r;
    int v16 = (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v12 + yw->wis_skeletons.cl1_b;
    int v15 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v12 + yw->wis_skeletons.cl1_g;

    *out1 = ((v13 & 0xFF) << 16) | ((v15 & 0xFF) << 8) | (v16 & 0xFF);



    float v17 = (v24 * x2 + v22 * y2 + 1.0) * 0.5;
    int v18 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v17 + yw->wis_skeletons.cl1_g;
    int v20 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v17 + yw->wis_skeletons.cl1_r;
    int v19 = (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v17 + yw->wis_skeletons.cl1_b;
    *out2 = ((v20 & 0xFF) << 16) | ((v18 & 0xFF) << 8) | (v19 & 0xFF);
}



void sb_0x4d7c08__sub0__sub4__sub1__sub0(_NC_STACK_ypaworld *yw, sklt_wis *wis)
{
    float v90 = (yw->field_1614 - wis->field_76) / 180.0;

    wis_color_func func = NULL;

    if ( wis->field_72 )
        func = wis_color2;

    float xx = yw->field_1340.m02;
    float yy = yw->field_1340.m22;

    float v70 = sqrt( POW2(xx) + POW2(yy) );

    if ( v70 > 0.0 )
    {
        float v8 = 1.0 / v70;
        xx *= v8;
        yy *= v8;
    }

    uint32_t v9 = yw_GetColor(yw, 21);
    uint32_t v73 = yw_GetColor(yw, 30);

    float tx = yw->field_1b84->parent_bact->field_621.sx - yw->field_1b84->field_621.sx;
    float ty = yw->field_1b84->parent_bact->field_621.sz - yw->field_1b84->field_621.sz;

    float v13 = tx * yy + ty * xx;
    float v15 = tx * -xx + ty * yy;

    float v17 = sqrt( POW2(v13) + POW2(v15) );

    if ( v17 > 30.0 )
    {
        float v20 = v13 * (1.0 / v17);
        float v23 = v15 * (1.0 / v17);

        yw->wis_skeletons.cl1_r = (v9 >> 16) & 0xFF;
        yw->wis_skeletons.cl1_g = (v9 >> 8) & 0xFF;
        yw->wis_skeletons.cl1_b = v9 & 0xFF;
        yw->wis_skeletons.cl2_r = (v73 >> 16) & 0xFF;
        yw->wis_skeletons.cl2_b = v73 & 0xFF;
        yw->wis_skeletons.cl2_g = (v73 >> 8) & 0xFF;

        sub_4E332C(yw, wis->sklts_intern[3], 0.7, 0.3, v23, -v20, v20, v23, 0.25, 0.3, v9, func, 0);
    }

    if ( yw->field_1b84->field_32 == yw->field_1b84->parent_bacto )
    {
        int v27 = 0;

        if ( yw->field_1b84->field_3DE == 1 )
        {
            v27 = 1;
            ty = yw->field_1b84->field_3ec.sz - yw->field_1b84->field_621.sz;
            tx = yw->field_1b84->field_3ec.sx - yw->field_1b84->field_621.sx;
        }
        else if ( yw->field_1b84->field_3DE == 2 )
        {
            v27 = 1;
            tx = yw->field_1b84->field_3e8->field_621.sx - yw->field_1b84->field_621.sx;
            ty = yw->field_1b84->field_3e8->field_621.sz - yw->field_1b84->field_621.sz;
        }

        if ( v27 )
        {
            uint32_t v31 = yw_GetColor(yw, 22);
            uint32_t v74 = yw_GetColor(yw, 31);

            float v88 = tx * yy + ty * xx;
            float a8 = tx * -xx + ty * yy;

            float v36 = sqrt( POW2(v88) + POW2(a8) );

            if ( v36 > 30.0 )
            {
                float v40 = v88 * (1.0 / v36);
                float v42 = a8 * (1.0 / v36);

                yw->wis_skeletons.cl1_r = (v31 >> 16) & 0xFF;
                yw->wis_skeletons.cl1_g = (v31 >> 8) & 0xFF;
                yw->wis_skeletons.cl1_b = v31 & 0xFF;
                yw->wis_skeletons.cl2_r = (v74 >> 16) & 0xFF;
                yw->wis_skeletons.cl2_b = v74 & 0xFF;
                yw->wis_skeletons.cl2_g = (v74 >> 8) & 0xFF;

                sub_4E332C(yw, wis->sklts_intern[3], 0.7, 0.3, v42, -v40, v40, v42, 0.25, 0.3, v31, func, 0);
            }
        }
    }

    if ( yw->hudi.field_18 )
    {
        uint32_t a11 = yw_GetColor(yw, 23);
        uint32_t v50 = yw_GetColor(yw, 32);

        float v45 = yw->hudi.field_18->field_621.sx - yw->field_1b84->field_621.sx;
        float v46 = yw->hudi.field_18->field_621.sz - yw->field_1b84->field_621.sz;

        float v81 = v45 * yy + v46 * xx;
        float v79 = v45 * -xx + v46 * yy;


        float v52 = sqrt( POW2(v81) + POW2(v79) );

        if ( v52 > 30.0 )
        {
            float v55 = v81 * (1.0 / v52);
            float v58 = v79 * (1.0 / v52);

            yw->wis_skeletons.cl1_r = (a11 >> 16) & 0xFF;
            yw->wis_skeletons.cl1_g = (a11 >> 8) & 0xFF;
            yw->wis_skeletons.cl1_b = a11 & 0xFF;
            yw->wis_skeletons.cl2_r = (v50 >> 16) & 0xFF;
            yw->wis_skeletons.cl2_b = v50 & 0xFF;
            yw->wis_skeletons.cl2_g = (v50 >> 8) & 0xFF;

            sub_4E332C(yw, wis->sklts_intern[3], 0.7, 0.3, v58, -v55, v55, v58, 0.25, 0.3, a11, func, 0);
        }
    }

    uint32_t v60 = yw_GetColor(yw, 24);
    uint32_t v75 = yw_GetColor(yw, 33);

    float v72 = 0.25;
    float v71 = 0.3;

    if ( v90 < 1.4 )
    {
        v71 = v90 * 0.3;
        v72 = v90 * 0.25;
    }

    yw->wis_skeletons.cl1_r = (v60 >> 16) & 0xFF;
    yw->wis_skeletons.cl1_g = (v60 >> 8) & 0xFF;
    yw->wis_skeletons.cl1_b = v60 & 0xFF;
    yw->wis_skeletons.cl2_r = (v75 >> 16) & 0xFF;
    yw->wis_skeletons.cl2_b = v75 & 0xFF;
    yw->wis_skeletons.cl2_g = (v75 >> 8) & 0xFF;

    sub_4E332C(yw, wis->sklts_intern[1], 0.7, 0.3, yy, -xx, xx, yy, v72, v71, v60, func, 0);

    int v64;

    switch ( yw->field_1b84->p_cell_area->owner )
    {
    case 0:
        v64 = 0;
        break;

    case 1:
        v64 = 1;
        break;

    case 2:
        v64 = 2;
        break;

    case 3:
        v64 = 3;
        break;

    case 4:
        v64 = 4;
        break;

    case 5:
        v64 = 5;
        break;

    case 6:
        v64 = 6;
        break;

    case 7:
        v64 = 7;
        break;

    default:
        v64 = 0;
        break;
    }

    uint32_t v65 = yw_GetColor(yw, v64);

    yw->wis_skeletons.cl1_r = (v65 >> 16) & 0xFF;
    yw->wis_skeletons.cl1_g = (v65 >> 8) & 0xFF;
    yw->wis_skeletons.cl1_b = v65 & 0xFF;
    yw->wis_skeletons.cl2_r = (v65 >> 16) & 0xFF;
    yw->wis_skeletons.cl2_b = v65 & 0xFF;
    yw->wis_skeletons.cl2_g = (v65 >> 8) & 0xFF;

    sub_4E332C(yw, wis->sklts_intern[2], 0.7, 0.3, yy, -xx, xx, yy, 0.07, 0.08, v65, func, 0);
}


char * sb_0x4d7c08__sub0__sub4__sub1(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    ua_fRect v9;
    v9.x1 = -1.0;
    v9.x2 = 1.0;
    v9.y1 = -1.0;
    v9.y2 = 1.0;

    call_method(yw->win3d, 210, &v9);

    ua_dRect v7;
    ua_dRect v8;

    v8.x1 = 0;
    v8.x2 = 0;
    v8.y1 = 0;
    v8.y2 = 0;

    if ( robo_map.cmd_flag & 0x20 )
    {
        v7.x2 = 0;
        v7.y1 = 0;
        v7.y2 = 0;
        v7.x1 = 0;
    }
    else
    {
        v7.x1 = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
        v7.y1 = robo_map.frm_1.btn_ypos - (yw->screen_height / 2);
        v7.x2 = robo_map.frm_1.btn_width + v7.x1;
        v7.y2 = robo_map.frm_1.btn_height + v7.y1;
    }

    call_method(yw->win3d, 221, &v7);

    sklt_wis *wis = &yw->wis_skeletons;

    sb_0x4d7c08__sub0__sub4__sub1__sub0(yw, wis);

    call_method(yw->win3d, 221, &v8);

    sb_0x4d7c08__sub0__sub4__sub1__sub1(yw, wis);

    call_method(yw->win3d, 221, &v7);

    pcur = sb_0x4e5a84(yw, wis, pcur, -0.7, 0.3, yw->field_1b84, -1, 0x10);

    if ( !(robo_map.cmd_flag & 0x20) )
    {
        v8.x1 = 0;
        v8.x2 = 0;
        v8.y1 = 0;
        v8.y2 = 0;

        call_method(yw->win3d, 221, &v8);
    }

    return pcur;
}

void sb_0x4d7c08__sub0__sub4(_NC_STACK_ypaworld *yw)
{
    char *pcur = byte_5C8DB0;

    if ( yw->field_1b84->field_3D5 == 2 )
    {
        const char *msg;

        if ( yw->field_1b80->field_3D6 & 0x2000000 )
        {
            msg = NULL;
        }
        else
        {
            int strid;
            const char *defMsg;

            if ( yw->field_1b80->field_3D5 == 2 )
            {
                defMsg = "* * *  H O S T  S T A T I O N  D E S T R O Y E D  * * *";
                strid = 11;
            }
            else
            {
                defMsg = "* * *  D R O N E  D E S T R O Y E D  * * *";
                strid = 10;
            }
            msg = get_lang_string(yw->string_pointers_p2, strid, defMsg);
        }

        if ( msg && yw->field_1614 / 500 & 1 )
        {
            fntcmd_select_tileset(&pcur, 15);
            fntcmd_set_xpos(&pcur, 0);
            fntcmd_set_center_ypos(&pcur, -(yw->font_default_h / 2));

            fntcmd_set_txtColor(&pcur, 255, 255, 255);

            pcur = sub_45148C(yw->tiles[15], pcur, msg, yw->screen_width);
        }
    }
    else if ( yw->field_1b7c != yw->field_1b78 && yw->wis_skeletons.field_0 )
    {
        sb_0x4d7c08__sub0__sub4__sub2(yw);

        if ( yw->field_1b84->energy >= yw->field_1b84->energy_2 )
        {
            yw->wis_skeletons.field_82 = 3000;
        }
        else
        {
            yw->wis_skeletons.field_82 = 3000 * yw->field_1b84->energy / yw->field_1b84->energy_2;

            if ( yw->wis_skeletons.field_82 < 200 )
                yw->wis_skeletons.field_82 = 200;
        }

        yw->wis_skeletons.field_86 += (float)yw->field_1618 / (float)yw->wis_skeletons.field_82;

        if ( yw->wis_skeletons.field_86 >= 1.0 )
            yw->wis_skeletons.field_86 = 0;

        pcur = sb_0x4d7c08__sub0__sub4__sub1(yw, pcur);
    }

    if ( yw->field_1a58 & 0x20 )
    {
        sub_4E3570(yw, yw->field_1a98);
        pcur = sub_4E2B5C(yw, pcur, yw->field_1a98);
    }

    if ( yw->hudi.field_18 )
        pcur = sub_4E2B5C(yw, pcur, yw->hudi.field_18);

    pcur = sb_0x4d7c08__sub0__sub4__sub0(yw, pcur);

    fntcmd_set_end(&pcur);

    w3d_a209 v20;
    v20.cmdbuf = byte_5C8DB0;
    v20.includ = 0;

    sub_423288(&v20);

    yw->hudi.field_18 = NULL;
}



void sub_4E3B80(_NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, uint32_t *out1, uint32_t *out2)
{
    float v15 = yw->wis_skeletons.field_86 * 2.0 - 1.0;

    float v17 = fabs( v15 - sqrt(POW2(x1) + POW2(y1)) );
    float v16 = fabs( v15 - sqrt(POW2(x2) + POW2(y2)) );

    if ( v17 > 1.0)
        v17 = 1.0;
    else if ( v17 < 0.0)
        v17 = 0.0;

    if ( v16 > 1.0)
        v16 = 1.0;
    else if ( v16 < 0.0)
        v16 = 0.0;

    int v9 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v17 + yw->wis_skeletons.cl1_g;
    int v10 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v17 + yw->wis_skeletons.cl1_r;
    int v11 =  (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v17 + yw->wis_skeletons.cl1_b;
    *out1 = ((v9 & 0xFF) << 8) | ((v10 & 0xFF) << 16) | (v11 & 0xFF);

    int v12 = (yw->wis_skeletons.cl2_g - yw->wis_skeletons.cl1_g) * v16 + yw->wis_skeletons.cl1_g;
    int v13 = (yw->wis_skeletons.cl2_r - yw->wis_skeletons.cl1_r) * v16 + yw->wis_skeletons.cl1_r;
    int v14 = (yw->wis_skeletons.cl2_b - yw->wis_skeletons.cl1_b) * v16 + yw->wis_skeletons.cl1_b;
    *out2 = ((v12 & 0xFF) << 8) | ((v13 & 0xFF) << 16) | (v14 & 0xFF);
}

void sub_4E3D98(_NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, uint32_t *out1, uint32_t *out2)
{
    int v7 = yw->field_1614 % 300;
    float v9 = v7 / 30000.0;

    sklt_wis *v8 = &yw->wis_skeletons;

    int v10 = (v8->cl2_g - v8->cl1_g) * v9 + v8->cl1_g;
    int v11 = (v8->cl2_r - v8->cl1_r) * v9 + v8->cl1_r;
    int v12 = (v8->cl2_b - v8->cl1_b) * v9 + v8->cl1_b;
    *out1 = (v12 & 0xFF) | ((v10 & 0xFF) << 8) | ((v11 & 0xFF) << 16);

    int v13 = (v8->cl2_r - v8->cl1_r) * v9 + v8->cl1_r;
    int v16 = (v8->cl2_b - v8->cl1_b) * v9 + v8->cl1_b;
    int v15 = (v8->cl2_g - v8->cl1_g) * v9 + v8->cl1_g;
    *out2 = ((v13 & 0xFF) << 16) | ((v15 & 0xFF) << 8) | (v16 & 0xFF);
}

void sb_0x4d7c08__sub0__sub4__sub1__sub1(_NC_STACK_ypaworld *yw, sklt_wis *wis)
{
    skeleton_64_stru *mg_wure = NULL;
    skeleton_64_stru *hud_wure = NULL;
    skeleton_64_stru *wpn_wure = NULL;
    skeleton_64_stru *wpn_wure2 = NULL;

    float v86 = (yw->field_1614 - wis->field_76 - 350) / 200.0;

    if ( v86 > 0.0 )
    {
        VhclProto *vhcl;

        if ( yw->field_1b84 && yw->VhclProtos != NULL )
            vhcl = &yw->VhclProtos[ yw->field_1b84->id ];
        else
            vhcl = NULL;

        if ( vhcl->hud_wireframe )
            call_vtbl(vhcl->hud_wireframe, 3, 0x80002000, &hud_wure, 0);
        else
            hud_wure = NULL;

        if ( vhcl->mg_wireframe )
            call_vtbl(vhcl->mg_wireframe, 3, 0x80002000, &mg_wure, 0);
        else
            mg_wure = wis->sklts_intern[4];

        if ( vhcl->wpn_wireframe_1 )
            call_vtbl(vhcl->wpn_wireframe_1, 3, 0x80002000, &wpn_wure, 0);
        else if ( yw->hudi.field_4 == 2)
            wpn_wure = wis->sklts_intern[5];
        else if ( yw->hudi.field_4 == 3 )
            wpn_wure = wis->sklts_intern[6];
        else if ( yw->hudi.field_4 == 4 )
            wpn_wure = wis->sklts_intern[7];

        if ( vhcl->wpn_wireframe_2 )
            call_vtbl(vhcl->wpn_wireframe_2, 3, 0x80002000, &wpn_wure2, 0);
        else if ( yw->hudi.field_4 == 2)
            wpn_wure2 = wis->sklts_intern[9];
        else if ( yw->hudi.field_4 == 3 )
            wpn_wure2 = wis->sklts_intern[10];
        else if ( yw->hudi.field_4 == 4 )
            wpn_wure2 = wis->sklts_intern[11];

        if ( yw->hudi.field_18 )
        {
            if ( yw->hudi.field_18 != wis->field_7E )
            {
                wis->field_7E = yw->hudi.field_18;
                wis->field_7A = yw->field_1614;
            }
        }
        else
        {
            wis->field_7E = NULL;
        }

        if ( hud_wure )
        {
            uint32_t v14 = yw_GetColor(yw, 34);
            sub_4E332C(yw, hud_wure, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.99, 0.99, v14, NULL, NULL);
        }

        if ( yw->hudi.field_0 )
        {
            if ( mg_wure )
            {
                uint32_t v15 = yw_GetColor(yw, 26);
                uint32_t v16 = yw_GetColor(yw, 35);

                float a9 = 0.3;

                if ( v86 < 1.4 )
                    a9 = v86 * 0.3;

                wis_color_func func;

                if ( wis->field_72 )
                    func = sub_4E3B80;
                else
                    func = NULL;

                sklt_wis *v19 = &yw->wis_skeletons;

                v19->cl1_r = (v16 >> 16) & 0xFF;
                v19->cl1_g = (v16 >> 8) & 0xFF;
                v19->cl1_b = v16 & 0xFF;
                v19->cl2_r = (v15 >> 16) & 0xFF;
                v19->cl2_g = (v15 >> 8) & 0xFF;
                v19->cl2_b = v15 & 0xFF;

                sub_4E332C(yw, mg_wure, yw->hudi.field_8, yw->hudi.field_C, 1.0, 0.0, 0.0, 1.0, a9, 0.4, v16, func, NULL);
            }
        }

        if ( yw->hudi.field_4 )
        {
            float v51[2];
            wis_color_func func;

            uint32_t a11;
            uint32_t v27;

            if ( yw->hudi.field_18 )
            {
                for (int i = 0; i < 2; i++)
                {
                    v51[i] = (yw->field_1614 - wis->field_7A) / 200.0;

                    if ( v51[i] > 1.0 )
                        v51[i] = 1.0;
                }

                a11 = yw_GetColor(yw, 27);
                v27 = yw_GetColor(yw, 36);

                if ( wis->field_72 )
                    func = sub_4E3D98;
                else
                    func = NULL;
            }
            else
            {
                v51[0] = 0;
                v51[1] = 0;

                v27 = yw_GetColor(yw, 28);
                a11 = yw_GetColor(yw, 37);

                if ( wis->field_72 )
                    func = sub_4E3B80;
                else
                    func = NULL;
            }

            int v63 = (a11 >> 16) & 0xFF;
            int v67 = (a11 >> 8) & 0xFF;
            int v66 = a11 & 0xFF;
            int v68 = (v27 >> 16) & 0xFF;
            int v64 = v27 & 0xFF;
            int v65 = (v27 >> 8) & 0xFF;

            for (int i = 0; i < 2; i++)
            {
                float v77, v78;

                if ( yw->hudi.field_18 )
                {
                    v77 = (yw->hudi.field_10 - yw->hudi.field_8) * v51[i] + yw->hudi.field_8;
                    v78 = (yw->hudi.field_14 - yw->hudi.field_C) * v51[i] + yw->hudi.field_C;
                }
                else
                {
                    v77 = yw->hudi.field_10;
                    v78 = yw->hudi.field_14;
                }

                float v80 = v51[i] * -0.23 + 0.4;
                float v74 = v51[i] * -0.17 + 0.3;

                if ( v86 < 1.4 )
                    v80 = v80 * v86;

                float v85;

                if ( i & 1 )
                {
                    v85 = v51[i] * 1.571;
                }
                else if ( v51[i] >= 1.0 )
                {
                    v85 = (yw->field_1614 - wis->field_7A) * 0.001 * 6.282;
                }
                else
                {
                    v85 = v51[i] * -3.141;
                }

                float v35 = sin(v85);
                float v83 = cos(v85);

                skeleton_64_stru *v36;

                if ( i & 1 )
                    v36 = wpn_wure;
                else
                    v36 = wpn_wure2;

                if ( v36 )
                {
                    yw->wis_skeletons.cl1_r = v63;
                    yw->wis_skeletons.cl1_g = v67;
                    yw->wis_skeletons.cl1_b = v66;
                    yw->wis_skeletons.cl2_r = v68;
                    yw->wis_skeletons.cl2_g = v65;
                    yw->wis_skeletons.cl2_b = v64;

                    sub_4E332C(yw, v36, v77, v78, v83, -v35, v35, v83, v74, v80, a11, func, 0);
                }
            }
        }
    }
}

void sub_4E3570(_NC_STACK_ypaworld *yw, __NC_STACK_ypabact *bact)
{
    float v6 = bact->field_621.sx - yw->field_1334.sx;
    float v4 = bact->field_621.sy - yw->field_1334.sy;
    float v8 = bact->field_621.sz - yw->field_1334.sz;

    float a3  = yw->field_1340.m00 * v6 + yw->field_1340.m01 * v4 + yw->field_1340.m02 * v8;
    float v33 = yw->field_1340.m10 * v6 + yw->field_1340.m11 * v4 + yw->field_1340.m12 * v8;
    float v30 = yw->field_1340.m20 * v6 + yw->field_1340.m21 * v4 + yw->field_1340.m22 * v8;

    if ( v30 > 30.0 && a3 < v30 )
    {
        if ( a3 > -v30 && v33 < v30 && v33 > -v30 )
        {
            float v10 = 1.0 / v30;
            uint32_t v11 = yw_GetColor(yw, bact->owner);
            skeleton_64_stru *v12 = yw->wis_skeletons.sklts_intern[13];

            float a3a = a3 * v10;
            float v34 = v33 * v10;

            if ( v12 )
            {
                float a4 = v34 - 0.08;

                if ( bact->parent_bacto != bact->field_32 || bact->owner != yw->field_1b80->owner )
                {
                    sub_4E332C(yw, yw->wis_skeletons.sklts_intern[13], a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.0075, 0.01, v11, NULL, NULL);
                }
                else
                {
                    sub_4E332C(yw, v12, a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.015, 0.02, v11, NULL, NULL);
                    sub_4E332C(yw, v12, a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.005, 0.00666, v11, NULL, NULL);
                }
            }
        }
    }
}


char *sb_0x4d7c08__sub0__sub4__sub0__sub0(_NC_STACK_ypaworld *yw, char *cur, __NC_STACK_ypabact *bact)
{
    char *pcur = cur;

    if ( yw->GameShell )
    {
        if ( yw->field_757E )
        {
            if ( *yw->GameShell->netTP2[bact->owner].field_0 )
            {
                float v5 = bact->field_621.sx - yw->field_1334.sx;
                float v6 = bact->field_621.sy - yw->field_1334.sy;
                float v10 = bact->field_621.sz - yw->field_1334.sz;

                float v32 = yw->field_1340.m00 * v5 + yw->field_1340.m01 * v6 + yw->field_1340.m02 * v10;
                float v31 = yw->field_1340.m10 * v5 + yw->field_1340.m11 * v6 + yw->field_1340.m12 * v10;
                float v26 = yw->field_1340.m20 * v5 + yw->field_1340.m21 * v6 + yw->field_1340.m22 * v10;

                if ( v26 > 30.0 && v32 < v26 )
                {
                    if ( v32 > -v26 && v31 < v26 && v31 > -v26 )
                    {
                        int v30 = yw->tiles[15]->font_height;
                        float v13 = 1.0 / v26;

                        int v27_4 = ((yw->screen_width / 2) * (v32 * v13 + 1.0));
                        int v15 = (yw->screen_height / 2) * (v31 * v13 + 1.0);

                        int v28 = 96;

                        if ( v27_4 + 96 >= yw->screen_width )
                            v28 = yw->screen_width - v27_4 - 1;

                        if ( v28 > 0 && v27_4 >= 0 )
                        {
                            if ( v28 + v27_4 <  yw->screen_width && v15 >= 0 )
                            {
                                if ( v30 + v15 < yw->screen_height )
                                {
                                    fntcmd_select_tileset(&pcur, 15);
                                    fntcmd_set_center_xpos(&pcur, v27_4 - (yw->screen_width / 2) );
                                    fntcmd_set_center_ypos(&pcur, v15 - (yw->screen_height / 2) );

                                    fntcmd_set_txtColor(&pcur, yw->iniColors[ bact->owner ].r, yw->iniColors[ bact->owner ].g, yw->iniColors[ bact->owner ].b);

                                    pcur = txtcmd_txt_w_bkg(yw->tiles[15], pcur,  yw->GameShell->netTP2[bact->owner].field_0, v28, 32);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return pcur;
}

char * sb_0x4d7c08__sub0__sub4__sub0(_NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->field_73CE & 0x20 )
    {
        int v5 = yw->field_1b84->field_c - 1;
        int v16 = yw->field_1b84->field_c + 1;
        int v17 = yw->field_1b84->field_10 - 1;
        int v14 = yw->field_1b84->field_10 + 1;

        if ( v5 < 1 )
            v5 = 1;

        if ( v17 < 1 )
            v17 = 1;

        if ( v16 >= yw->sectors_maxX2 )
            v16 = yw->sectors_maxX2 - 1;

        if ( v14 >= yw->sectors_maxY2 )
            v14 = yw->sectors_maxY2 - 1;

        for (int i = v17; i <= v14; i++ )
        {
            for (int  j = v5; j <= v16; j++ )
            {
                cellArea *v8 = &yw->cells[j + yw->sectors_maxX2 * i];

                if ( robo_map.field_1EF & v8->field_39 )
                {
                    __NC_STACK_ypabact *bct = (__NC_STACK_ypabact *)v8->field_3C.head;

                    while (bct->next)
                    {

                        if ( bct->field_24 != 4 && bct->field_24 != 3 )
                        {

                            if ( bct->field_3D5 != 4 && bct->field_3D5 != 5 && bct->field_3D5 != 2 )
                            {
                                if ( bct->field_24 != 9 )
                                    sub_4E3570(yw, bct);
                                else
                                {
                                    NC_STACK_ypagun *gun = (NC_STACK_ypagun *)bct->self;

                                    if ( !(gun->stack__ypagun.field_39 & 2) )
                                        sub_4E3570(yw, bct);
                                }

                                if ( yw->field_757E )
                                {
                                    if ( bct->field_3D6 & 0x800000 )
                                    {
                                        if ( yw->GameShell )
                                            pcur = sb_0x4d7c08__sub0__sub4__sub0__sub0(yw, pcur, bct);
                                    }
                                }
                            }

                        }

                        bct = (__NC_STACK_ypabact *)bct->next;
                    }



                }
            }
        }
    }
    return pcur;
}


char * sub_4E2B5C(_NC_STACK_ypaworld *yw, char *cur, __NC_STACK_ypabact *bact)
{
    // Render fraction triangles above units
    char *pcur = cur;

    float v6 = bact->field_621.sx - yw->field_1334.sx;
    float v5 = bact->field_621.sy - yw->field_1334.sy;
    float v9 = bact->field_621.sz - yw->field_1334.sz;

    float v44 = yw->field_1340.m00 * v6 + yw->field_1340.m01 * v5 + yw->field_1340.m02 * v9;
    float v46 = yw->field_1340.m10 * v6 + yw->field_1340.m11 * v5 + yw->field_1340.m12 * v9;
    float v37 = yw->field_1340.m20 * v6 + yw->field_1340.m21 * v5 + yw->field_1340.m22 * v9;

    if ( v37 > 30.0 && v44 < v37 )
    {
        if ( v44 > -v37 && v46 < v37 && v46 > -v37 )
        {
            int v11 = 0;
            int v33 = 0;
            int v34 = 0;
            int v36 = 0;
            int v30 = 0;

            if ( !(robo_map.cmd_flag & 0x20) )
            {
                v30 = 1;
                v11 = robo_map.frm_1.btn_xpos;
                v33 = v11 + robo_map.frm_1.btn_width;
                v36 = robo_map.frm_1.btn_ypos;
                v34 = v36 + robo_map.frm_1.btn_height;
            }

            float v45 = v44 * (1.0 / v37);
            float v47 = v46 * (1.0 / v37);

            int v13;

            if ( yw->screen_width >= 512 )
                v13 = 8;
            else
                v13 = 4;

            int v43 = v13 * yw->tiles[50]->chars->width;

            int v42 = (yw->screen_width / 2) * (v45 + 1.0);
            int v41 = (yw->screen_height / 2) * (v47 + 1.0);

            if ( !v30 || v42 <= v11 || v42 >= v33 || v41 <= v36 || v41 >= v34 )
            {
                v42 -= v43 / 2;
                v41 -= (yw->tiles[50]->font_height / 2) + (yw->screen_height / 16);

                if ( v42 >= 0 )
                {
                    if ( v42 + v43 < yw->screen_width && v41 >= 0 )
                    {
                        if ( yw->tiles[50]->font_height + v41 < yw->screen_height )
                        {
                            fntcmd_select_tileset(&pcur, 50);

                            fntcmd_set_center_xpos(&pcur, v42 - (yw->screen_width / 2) );
                            fntcmd_set_center_ypos(&pcur, v41 - (yw->screen_height / 2) );

                            int v38 = bact->energy_2 / v13;

                            int v27 = v38 / 2; //v38 - (v38 / 2);

                            for (int i = 1; i <= v13; i++)
                            {
                                if ( v27 > bact->energy )
                                    fntcmd_store_u8(&pcur, 6);
                                else
                                    fntcmd_store_u8(&pcur, 2);

                                v27 += v38;
                            }
                        }
                    }
                }
            }
        }
    }
    return pcur;
}

void sb_0x4d7c08__sub0__sub4__sub2__sub0(_NC_STACK_ypaworld *yw)
{
    // Render units on radare
    w3d_a209 post_rndr;
    post_rndr.cmdbuf = byte_5FFF80;
    post_rndr.includ = NULL;

    char *pcur = byte_5FFF80;
    fntcmd_select_tileset(&pcur, 61);

    float v6 = yw->screen_width / 2;
    float v7 = yw->screen_height / 2;

    robo_map.field_1F8 = dround((robo_map.field_25C - robo_map.field_254) * v6);
    robo_map.field_1FC = dround((robo_map.field_260 - robo_map.field_258) * v7);

    robo_map.field_1F0 = dround(robo_map.field_1D8 / robo_map.field_1E0) - (robo_map.field_1F8 / 2);
    robo_map.field_1F4 = -dround(robo_map.field_1DC / robo_map.field_1E4) - (robo_map.field_1FC / 2);

    robo_map.field_200 = dround(v6 * robo_map.field_254);
    robo_map.field_204 = dround(v7 * robo_map.field_258);

    ua_dRect drect;
    drect.x1 = robo_map.field_200;
    drect.y1 = robo_map.field_204;
    drect.x2 = robo_map.field_1F8 + robo_map.field_200 - 1;
    drect.y2 = robo_map.field_1FC + robo_map.field_204 - 1;

    call_method(yw->win3d, 211, &drect);

    int v14 = dround(robo_map.field_1F0 * robo_map.field_1E0) / 1200;
    int v29 = dround(robo_map.field_1F4 * robo_map.field_1E4) / 1200;
    int v28 = dround( (robo_map.field_1F8 + robo_map.field_1F0) * robo_map.field_1E0 ) / 1200;
    int v30 = dround( (robo_map.field_1FC + robo_map.field_1F4) * robo_map.field_1E4 ) / 1200;

    if ( v14 < 1 )
        v14 = 1;

    if ( v29 < 1 )
        v29 = 1;

    if ( v28 >= yw->sectors_maxX2 )
        v28 = yw->sectors_maxX2 - 1;

    if ( v30 >= yw->sectors_maxY2 )
        v30 = yw->sectors_maxY2 - 1;

    int v38 = 0;

    for (int i = v29; i <= v30; i++ )
    {
        for (int j = v14; j <= v28; j++ )
        {
            if ( v38 < 512 ) // Units maximum?
            {
                cellArea *v17 = &yw->cells[j + yw->sectors_maxX2 * i];

                if ( robo_map.field_1EF & v17->field_39 )
                {
                    __NC_STACK_ypabact *bact = (__NC_STACK_ypabact *)v17->field_3C.head;

                    while(bact->next)
                    {
                        int v19 = 0;
                        if ( !(robo_map.cmd_flag & 0x20) )
                        {
                            int v20 = robo_map.frm_1.btn_xpos - (yw->screen_width / 2);
                            int v34 = v20 + robo_map.frm_1.btn_width;
                            int v37 = robo_map.frm_1.btn_ypos - (yw->screen_height / 2);
                            int v35 = v37 + robo_map.frm_1.btn_height;

                            int a3, a4;
                            sub_4F681C(bact->field_621.sx, bact->field_621.sz, &a3, &a4);

                            if ( v20 < a3 && a3 < v34 && a4 > v37 && a4 < v35 )
                                v19 = 1;
                        }

                        if ( bact->field_24 == 3 )
                        {
                            if ( !v19 )
                            {
                                fntcmd_select_tileset(&pcur, 1);

                                pcur = sub_4F6DFC(yw, pcur, yw->tiles[1]->font_height, yw->tiles[1]->chars[24].width, bact, 0);

                                fntcmd_select_tileset(&pcur, 61);

                                v38++;
                            }
                        }
                        else if ( bact->field_24 == 9 )
                        {
                            NC_STACK_ypagun *gun = (NC_STACK_ypagun *)bact->self;

                            if ( (!(gun->stack__ypagun.field_39 & 2) || bact == yw->field_1b84) && !v19 )
                            {
                                pcur = sub_4F6DFC(yw, pcur, 7, 7, bact, 0);

                                v38++;
                            }
                        }
                        else if ( !v19 )
                        {
                            pcur = sub_4F6DFC(yw, pcur, 7, 7, bact, 0);

                            v38++;
                        }

                        bact = (__NC_STACK_ypabact *)bact->next;
                    }
                }
            }
        }
    }

    fntcmd_set_end(&pcur);

    sub_423288(&post_rndr);
}

void sb_0x4d7c08__sub0__sub4__sub2(_NC_STACK_ypaworld *yw)
{
    float v12 = robo_map.field_1D8;
    float v15 = robo_map.field_1DC;
    float v14 = robo_map.field_1E0;
    float v13 = robo_map.field_1E4;
    float v7 = robo_map.field_1EC;
    float v8 = robo_map.field_1EF;
    float v9 = robo_map.field_1EE;

    robo_map.field_1EC = 1;
    robo_map.field_1EF = -1;
    robo_map.field_1E8 |= 0x100;
    robo_map.field_1EE = 3;
    robo_map.field_1D8 = yw->current_bact->field_621.sx;
    robo_map.field_1DC = yw->current_bact->field_621.sz;
    robo_map.field_1E0 = 4800.0 / ((float)(yw->screen_width / 2) * (robo_map.field_25C - robo_map.field_254));
    robo_map.field_1E4 = 4800.0 / ((float)(yw->screen_height / 2) * (robo_map.field_260 - robo_map.field_258));

    float v19 = yw->field_1340.m20;
    float v17 = yw->field_1340.m22;

    float v18, v20;

    float v16 = sqrt( POW2(v17) + POW2(v19));
    if ( v16 <= 0.0 )
    {
        v18 = 1.0;
        v20 = 0.0;
    }
    else
    {
        float v11 = 1.0 / v16;
        v20 = v19 * v11;
        v18 = v17 * v11;
    }
    robo_map.field_264 = v18;

    flt_5164F4 = -v20;
    flt_516504 = v20;
    flt_51650C = v18;

    sb_0x4d7c08__sub0__sub4__sub2__sub0(yw);

    robo_map.field_1EC = v7;
    robo_map.field_1EE = v9;
    robo_map.field_1D8 = v12;
    robo_map.field_1EF = v8;
    robo_map.field_1DC = v15;
    robo_map.field_1E0 = v14;
    robo_map.field_1E8 &= 0xFFFFFEFF;
    robo_map.field_1E4 = v13;
}


struct aab
{
    int a;
    int b;
};

int sub_47F360(const void *a1, const void *a2)
{
    return (((const aab *)a2)->b) - (((const aab *)a1)->b);
}

char *sb_0x4d7c08__sub0__sub2__sub1(_NC_STACK_ypaworld *yw, char *cur, player_status *statuses, int a3)
{
    char *pcur = cur;

    if ( yw->field_757E )
    {
        aab v25[8];

        int nums = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( (1 << i) & yw->field_2d90->ownerMap__has_vehicles )
            {
                v25[nums].a = i;
                v25[nums].b = statuses[i].p5;
                nums++;
            }
        }

        qsort(v25, nums, sizeof(aab), sub_47F360);

        for (int i = 0; i < nums; i++)
        {
            const char *v12;
            int clrid;

            switch ( v25[i].a )
            {
            case 1:
                v12 = get_lang_string(yw->string_pointers_p2, 2411, "RESISTANCE");
                clrid = 1;
                break;

            case 2:
                v12 = get_lang_string(yw->string_pointers_p2, 2412, "SULGOGARS");
                clrid = 2;
                break;

            case 3:
                v12 = get_lang_string(yw->string_pointers_p2, 2413, "MYKONIANS");
                clrid = 3;
                break;

            case 4:
                v12 = get_lang_string(yw->string_pointers_p2, 2414, "TAERKASTEN");
                clrid = 4;
                break;

            case 5:
                v12 = get_lang_string(yw->string_pointers_p2, 2415, "BLACK SECT");
                clrid = 5;
                break;

            case 6:
                v12 = get_lang_string(yw->string_pointers_p2, 2416, "GHORKOV");
                clrid = 6;
                break;

            default:
                v12 = get_lang_string(yw->string_pointers_p2, 2417, "NEUTRAL");
                clrid = 7;
                break;
            }

            fntcmd_set_txtColor(&pcur, yw->iniColors[clrid].r, yw->iniColors[clrid].g, yw->iniColors[clrid].b);

            listview_t1 a4a[2];

            a4a[0].txt = v12;
            a4a[0].bkg_tile = 32;
            a4a[0].left_tile = 0;
            a4a[0].right_tile = 0;
            a4a[0].tileset_id = 15;
            a4a[0].flags = 36;
            a4a[0].field_width = a3 * 0.5;

            char a1a[32];
            sprintf(a1a, "%d", statuses[ v25[i].a ].p5);

            a4a[1].txt = a1a;
            a4a[1].field_width = a3 * 0.5;
            a4a[1].tileset_id = 15;
            a4a[1].bkg_tile = 32;
            a4a[1].flags = 36;
            a4a[1].left_tile = 0;
            a4a[1].right_tile = 0;

            pcur = lstvw_txt_line(yw, pcur, 2, a4a);

            fntcmd_next_line(&pcur);
        }

        fntcmd_next_line(&pcur);
    }
    return pcur;
}


char * sb_0x4d7c08__sub0__sub2__sub0(_NC_STACK_ypaworld *yw, char *cur, int a3)
{
    char *pcur = cur;

    if ( yw->field_757E )
    {
        if ( yw->field_1bac[ yw->field_1b80->owner ] <= yw->unit_limit_1 )
            fntcmd_set_txtColor(&pcur, yw->iniColors[63].r, yw->iniColors[63].g, yw->iniColors[63].b);
        else
            fntcmd_set_txtColor(&pcur, yw->iniColors[6].r, yw->iniColors[6].g, yw->iniColors[6].b);

        listview_t1 a4[2];

        a4[0].txt = get_lang_string(yw->string_pointers_p2, 2473, "2473 == Units:");
        a4[0].field_width = a3 * 0.5;
        a4[0].bkg_tile = 32;
        a4[0].tileset_id = 15;
        a4[0].left_tile = 0;
        a4[0].right_tile = 0;
        a4[0].flags = 36;

        char a1a[64];
        sprintf(a1a, "%d / %d", yw->field_1bac[ yw->field_1b80->owner ], yw->unit_limit_1);

        a4[1].txt = a1a;
        a4[1].field_width = a3 * 0.5;
        a4[1].tileset_id = 15;
        a4[1].right_tile = 0;
        a4[1].bkg_tile = 32;
        a4[1].flags = 36;
        a4[1].left_tile = 0;

        pcur = lstvw_txt_line(yw, pcur, 2, a4);

        fntcmd_next_line(&pcur);
    }
    return pcur;
}

void sb_0x4d7c08__sub0__sub2(_NC_STACK_ypaworld *yw)
{
    char buf[1024];
    char *pcur = buf;

    int v25 = 2 * yw->screen_width / 3;
    int v2 = yw->screen_width - v25;

    fntcmd_select_tileset(&pcur, 15);
    fntcmd_set_xpos(&pcur, v25);
    fntcmd_set_ypos(&pcur, up_panel.field_1CC + (yw->font_default_h / 2));

    pcur = sb_0x4d7c08__sub0__sub2__sub0(yw, pcur, v2);
    pcur = sb_0x4d7c08__sub0__sub2__sub1(yw, pcur, yw->field_7796, v2);

    for (int i = 0; i < yw->field_2d90->supetItems_count; i++)
    {
        supetItemProto *item = &yw->field_2d90->supetItems[i];

        if ( item->type )
        {
            char a1[128];

            const char *v8 = "SUPER ITEM";

            if (item->type == 1)
                v8 = get_lang_string(yw->string_pointers_p2, 18, "18 == STOUDSON BOMB");
            else if ( item->type == 2 )
                v8 = get_lang_string(yw->string_pointers_p2, 19, "19 == STOUDSON WAVE");

            int v23 = 0;

            if ( item->field_4 == 1 || item->field_4 == 2 )
            {
                int v10 = (item->field_F8 + 1023) / 1024;

                if ( v10 < 0 )
                    v10 = 0;

                sprintf(a1, "%s: %02d:%02d", v8, v10 / 60 % 60, v10 % 60);

                v23 = 1;
            }
            else if ( item->field_4 == 3 )
            {
                sprintf(a1, "%s: %s", v8,  get_lang_string(yw->string_pointers_p2, 2471, "2471 == TRIGGERED") );

                v23 = 1;
            }

            if ( v23 )
            {
                fntcmd_set_xpos(&pcur, v25);

                fntcmd_set_txtColor(&pcur, yw->iniColors[item->field_F4].r, yw->iniColors[item->field_F4].g, yw->iniColors[item->field_F4].b);

                pcur = txtcmd_txt_w_bkg(yw->tiles[15], pcur, a1, yw->screen_width - v25, 32);

                fntcmd_next_line(&pcur);
            }
        }
    }

    fntcmd_set_end(&pcur);

    w3d_a209 a209;
    a209.cmdbuf = buf;
    a209.includ = NULL;

    call_method(yw->win3d, 209, &a209);
}
