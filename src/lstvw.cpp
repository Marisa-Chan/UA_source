#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "utils.h"

#include "lstvw.h"
#include "font.h"



char * lstvw_make_title(_NC_STACK_ypaworld *yw, int xpos, int ypos, int a3, const char *a5, char *a6, uint8_t a7, int flag)
{
    int v27 = 0;
    if ( flag & 0x10 )
        v27 = yw->tiles[24]->chars[65].width; // Help button

    int v26 = 0;
    if ( flag & 0x100 )
        v26 = yw->tiles[24]->chars[66].width; // Close button

    int v29 = 0;
    if ( a7 )
        v29 = yw->tiles[0]->chars[a7].width;

    char buf[128];
    strcpy(buf, " ");
    strcat(buf, a5);

    char *tmp = a6;

    fntcmd_select_tileset(&tmp, 6);
    fntcmd_set_center_xpos(&tmp, xpos);
    fntcmd_set_center_ypos(&tmp, ypos);

    fntcmd_store_s8(&tmp, 98);

    fntcmd_set_txtColor(&tmp, yw->iniColors[60].r, yw->iniColors[60].g, yw->iniColors[60].b);

    tmp = txtcmd_txt_w_bkg(yw->tiles[6], tmp, buf, a3 - v27 - v26 - v29 - yw->font_default_w__b, 99);

    if ( a7 )
    {
        fntcmd_select_tileset(&tmp, 0);
        fntcmd_store_u8(&tmp, a7);
    }

    if ( flag & 0x100 )
    {
        int ts = ((flag & 0x80) != 0) + 24;

        fntcmd_select_tileset(&tmp, ts);
        fntcmd_store_s8(&tmp, 65);
    }

    if ( flag & 0x10 )
    {
        int ts = ((flag & 0x40) != 0) + 24;

        fntcmd_select_tileset(&tmp, ts);
        fntcmd_store_s8(&tmp, 85);
    }
    return tmp;
}


int sb_0x4e947c__sub0__sub0(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    char *draw_cmd = (char *)AllocVec(512, 1);

    if ( !draw_cmd )
        return 0;

    int xpos = lstvw->frm_1.btn_xpos - (yw->screen_width / 2);
    int ypos = lstvw->frm_1.btn_ypos - (yw->screen_height / 2);

    lstvw->draw_cmd = draw_cmd;

    char *v7 = draw_cmd;

    if ( lstvw->field_1D0 & 2 )
    {
        v7 = lstvw_make_title(yw, xpos, ypos, lstvw->frm_1.btn_width, lstvw->wnd_title, draw_cmd, 0, lstvw->cmd_flag);

        fntcmd_next_line(&v7);
    }
    else
    {
        fntcmd_select_tileset(&v7, 0);
        fntcmd_set_center_xpos(&v7, xpos);
        fntcmd_set_center_ypos(&v7, ypos);
    }
    fntcmd_include(&v7, 1); // Include data part
    fntcmd_include(&v7, 0); // Include slider
    fntcmd_set_end(&v7);

    return 1;
}

int sb_0x4e947c__sub0(_NC_STACK_ypaworld *yw, listview *lstvw, stack_vals *stak)
{

    //find_id_in_stack_def_val(0x80000009, 0, stak);
    //find_id_in_stack_def_val(0x8000000A, 0, stak);

    char *p = (char *)AllocVec(32, 1);

    if ( !p )
        return 0;

    lstvw->field_1BC = p;
    fntcmd_set_end(&p);

    if ( !sb_0x4e947c__sub0__sub0(yw, lstvw) )
        return 0;


    p = (char *)AllocVec(0x2000, 1);

    if ( !p )
        return 0;

    lstvw->data_cmdbuf = p;
    fntcmd_set_end(&p);

    p = (char *)AllocVec(256, 1);

    if ( !p )
        return 0;

    lstvw->slider_cmdbuf = p;
    fntcmd_set_end(&p);

    char **z = (char **)AllocVec(sizeof(char *) * 2, 1);

    if ( !z )
        return 0;

    lstvw->field_1C4 = z;

    z[0] = lstvw->slider_cmdbuf; // Slider
    z[1] = lstvw->data_cmdbuf; // Data

    return 1;
}

void lstvw_updlimits(_NC_STACK_ypaworld *yw, listview *lstvw, int a3, int a4)
{
    if ( lstvw->width < lstvw->width_min)
        lstvw->width = lstvw->width_min;

    if ( lstvw->width > lstvw->width_max)
        lstvw->width = lstvw->width_max;

    if ( lstvw->element_count < lstvw->element_count_min)
        lstvw->element_count = lstvw->element_count_min;

    if ( lstvw->element_count > lstvw->element_count_max)
        lstvw->element_count = lstvw->element_count_max;

    int v8 = lstvw->width;

    if ( (lstvw->field_1D0 & 4) || lstvw->elements_for_scroll_size > lstvw->element_count_max )
        v8 += yw->font_yscrl_bkg_w;

    int v9 = lstvw->field_1E8 + lstvw->field_1EA + lstvw->font_h * lstvw->element_count;

    if ( lstvw->field_1D0 & 2 )
        v9 += yw->font_default_h;

    if ( a3 == -1 )
    {
        a3 = (yw->screen_width / 2) - (v8 / 2);
    }
    else if ( a3 == -2 )
    {
        a3 = lstvw->frm_1.btn_xpos;
    }

    if ( a4 == -1 )
    {
        a4 = (yw->screen_height / 2) - (v9 / 2);
    }
    else if ( a4 == -2 )
    {
        a4 = lstvw->frm_1.btn_ypos;
    }

    if ( a3 < 0 )
        a3 = 0;

    if ( a4 < yw->icon_energy__h )
        a4 = yw->icon_energy__h;

    if ( v8 + a3 >= yw->screen_width )
        a3 = yw->screen_width - v8;

    if ( v9 + a4 >= yw->screen_height - yw->icon0___h )
        a4 = yw->screen_height - yw->icon0___h - v9;

    lstvw->frm_1.btn_ypos = a4;
    lstvw->frm_1.btn_width = v8;
    lstvw->frm_1.btn_height = v9;
    lstvw->frm_1.btn_xpos = a3;
}

int sb_0x4e947c__sub1(_NC_STACK_ypaworld *, listview *lstvw)
{
    lstvw->frm_1.field_10 = lstvw->element_count + 8;

    button_str1* v4 = (button_str1 *)AllocVec(sizeof(button_str1) * 33, 65537);

    if (!v4)
        return 0;

    for (int i = 0; i < 32; i++)
        lstvw->frm_1.field_18[i] = &v4[i];


    if ( lstvw->field_1D0 & 8 )
    {
        lstvw->frm_2.field_10 = 1;
        lstvw->frm_2.field_18[0] = &v4[32];
        lstvw->frm_2.field_18[0]->xpos = 0;
        lstvw->frm_2.field_18[0]->ypos = 0;
        lstvw->frm_2.field_18[0]->width = 16;
        lstvw->frm_2.field_18[0]->fnt_height = 16;
    }
    else
    {
        lstvw->frm_2.field_10 = 0;
    }

    return 1;
}

void lstvw_update_main(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    if ( lstvw->field_1D0 & 2 )
    {
        char *v5 = lstvw->draw_cmd;

        int v6 = (lstvw->frm_1.btn_xpos) - (yw->screen_width / 2);
        int v8 = (lstvw->frm_1.btn_ypos) - (yw->screen_height / 2);

        if ( lstvw->cmd_flag & 0x10 )
            v8 = yw->font_default_w__a;
        else
            v8 = 0;

        int v17 = v8;

        if ( lstvw->cmd_flag & 0x100 )
            v8 = yw->font_default_w__a;
        else
            v8 = 0;

        if ( lstvw->field_1D0 & 2 )
        {
            v5 = lstvw_make_title(yw, v6, v8, lstvw->frm_1.btn_width, lstvw->wnd_title, v5, 0, lstvw->cmd_flag);
            fntcmd_next_line(&v5);
        }
        else
        {
            fntcmd_select_tileset(&v5, 0);
            fntcmd_set_center_xpos(&v5, v6);
            fntcmd_set_center_ypos(&v5, v8);
        }

        fntcmd_include(&v5, 1); // Data
        fntcmd_include(&v5, 0); // Slider
        fntcmd_set_end(&v5);

        int v14 = lstvw->frm_1.btn_width - v17 - v8;

        lstvw->frm_1.field_18[1]->xpos = 0;
        lstvw->frm_1.field_18[1]->ypos = 0;
        lstvw->frm_1.field_18[1]->width = v14;
        lstvw->frm_1.field_18[1]->fnt_height = yw->font_default_h;

        if ( lstvw->cmd_flag & 0x100 )
        {
            lstvw->frm_1.field_18[7]->xpos = v14;
            lstvw->frm_1.field_18[7]->ypos = 0;
            lstvw->frm_1.field_18[7]->width = v17;
            lstvw->frm_1.field_18[7]->fnt_height = yw->font_default_h;
        }
        else
        {
            lstvw->frm_1.field_18[7]->xpos = 0;
            lstvw->frm_1.field_18[7]->ypos = 0;
            lstvw->frm_1.field_18[7]->width = 0;
            lstvw->frm_1.field_18[7]->fnt_height = 0;
        }

        if ( lstvw->cmd_flag & 0x10 )
        {
            lstvw->frm_1.field_18[0]->xpos = lstvw->frm_1.btn_width - v17;
            lstvw->frm_1.field_18[0]->ypos = 0;
            lstvw->frm_1.field_18[0]->width = v17;
            lstvw->frm_1.field_18[0]->fnt_height = yw->font_default_h;
        }
        else
        {
            lstvw->frm_1.field_18[0]->xpos = 0;
            lstvw->frm_1.field_18[0]->ypos = 0;
            lstvw->frm_1.field_18[0]->width = 0;
            lstvw->frm_1.field_18[0]->fnt_height = 0;
        }
    }
}

void lstvw_update_slider__sub0(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    int v3 = lstvw->frm_1.btn_height;

    if ( lstvw->field_1D0 & 2 )
        v3 -= yw->font_default_h;

    if ( lstvw->field_1D0 & 4 )
        v3 -= yw->font_xscrl_h;

    int v4, v5;

    if ( lstvw->elements_for_scroll_size )
    {
        v4 = v3 * lstvw->element_count / lstvw->elements_for_scroll_size;
        v5 = v3 * lstvw->scroll_pos / lstvw->elements_for_scroll_size;
    }
    else
    {
        v4 = v3;
        v5 = 0;
    }

    if ( v4 >= 2 )
    {
        if ( v4 > v3 )
            v4 = v3;
    }
    else
    {
        v5 -= (2 - v4) / 2;
        v4 = 2;
    }

    if ( v5 < 0 )
        v5 = 0;
    if ( v5 + v4 > v3 )
        v5 = v3 - v4;

    lstvw->field_244 = v4;
    lstvw->field_246 = v5;
    lstvw->field_242 = v3;
}

void lstvw_update_slider(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    char *v5 = lstvw->slider_cmdbuf;

    if ( lstvw->field_1D0 & 4 || lstvw->elements_for_scroll_size > lstvw->element_count_max )
    {

        if ( lstvw->field_1D0 & 0x40 )
        {
            lstvw->scroll_pos = lstvw->elements_for_scroll_size * lstvw->field_246  / lstvw->field_242;

            if ( lstvw->element_count + lstvw->scroll_pos > lstvw->elements_for_scroll_size)
            {
                lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count;

                if ( lstvw->scroll_pos < 0 )
                    lstvw->scroll_pos = 0;
            }
        }
        else
        {
            lstvw_update_slider__sub0(yw, lstvw);
        }

        int v34 = lstvw->frm_1.btn_width - yw->font_yscrl_bkg_w;
        int v35 = 0;

        if ( lstvw->field_1D0 & 2 )
            v35 = yw->font_default_h;

        lstvw->frm_1.field_18[2]->xpos = v34; // Scroll up part
        lstvw->frm_1.field_18[2]->ypos = v35;
        lstvw->frm_1.field_18[2]->width = yw->font_yscrl_bkg_w;
        lstvw->frm_1.field_18[2]->fnt_height = lstvw->field_246;
        lstvw->frm_1.field_18[3]->xpos = v34; // Scroll position
        lstvw->frm_1.field_18[3]->ypos = lstvw->field_246 + v35;
        lstvw->frm_1.field_18[3]->width = yw->font_yscrl_bkg_w;
        lstvw->frm_1.field_18[3]->fnt_height = lstvw->field_244;
        lstvw->frm_1.field_18[4]->xpos = v34; // Scroll down part
        lstvw->frm_1.field_18[4]->ypos = lstvw->field_244 + lstvw->field_246 + v35;
        lstvw->frm_1.field_18[4]->width = yw->font_yscrl_bkg_w;
        lstvw->frm_1.field_18[4]->fnt_height = lstvw->field_242 - (lstvw->field_244 + lstvw->field_246);
        lstvw->frm_1.field_18[6]->xpos = 0;
        lstvw->frm_1.field_18[6]->ypos = lstvw->frm_1.btn_height - lstvw->field_1EA;
        lstvw->frm_1.field_18[6]->width = lstvw->frm_1.btn_width - yw->font_yscrl_bkg_w;
        lstvw->frm_1.field_18[6]->fnt_height = lstvw->field_1EA;

        if ( lstvw->field_1D0 & 4 )
        {
            lstvw->frm_1.field_18[5]->xpos = v34; // Resize button
            lstvw->frm_1.field_18[5]->ypos = lstvw->field_242 + v35;
            lstvw->frm_1.field_18[5]->width = yw->font_yscrl_bkg_w;
            lstvw->frm_1.field_18[5]->fnt_height = yw->font_xscrl_h;
        }
        else
        {
            memset(lstvw->frm_1.field_18[5], 0, sizeof(button_str1));
        }

        int v7 = lstvw->frm_1.btn_xpos + v34 - (yw->screen_width / 2);
        int v11 = lstvw->frm_1.btn_ypos + v35 - (yw->screen_height / 2);

        fntcmd_set_center_xpos(&v5, v7);
        fntcmd_set_center_ypos(&v5, v11);

        if ( lstvw->field_246 > 0 )
        {
            fntcmd_reset_tileset(&v5, 13);
            fntcmd_store_u8(&v5, 67);
            fntcmd_next_line(&v5);
            fntcmd_reset_tileset(&v5, 12);

            int v15 = lstvw->field_246 - 1;

            while (v15 >= yw->font_yscrl_h)
            {
                fntcmd_store_u8(&v5, 66);
                fntcmd_next_line(&v5);
                v15 -= yw->font_yscrl_h;
            }

            if ( v15 > 0 )
            {
                fntcmd_set_yheight(&v5, v15);

                fntcmd_store_u8(&v5, 66);
                fntcmd_next_line(&v5);
            }
        }


        if ( lstvw->field_244 > 0 )
        {
            fntcmd_reset_tileset(&v5, 13);
            fntcmd_store_u8(&v5, 69);
            fntcmd_next_line(&v5);
            fntcmd_reset_tileset(&v5, 12);

            int v15 = lstvw->field_244 - 1;

            while (v15 > yw->font_yscrl_h)
            {
                fntcmd_store_u8(&v5, 67);
                fntcmd_next_line(&v5);
                v15 -= yw->font_yscrl_h;
            }

            if ( v15 > 1 )
            {
                fntcmd_set_yheight(&v5, v15 - 1);

                fntcmd_store_u8(&v5, 67);
                fntcmd_next_line(&v5);
            }
        }

        fntcmd_reset_tileset(&v5, 13);
        fntcmd_store_u8(&v5, 70);
        fntcmd_next_line(&v5);

        int v26 = lstvw->field_242 - (lstvw->field_246  + lstvw->field_244);

        if ( v26 > 0 )
        {
            fntcmd_reset_tileset(&v5, 12);

            while (v26 > yw->font_yscrl_h)
            {
                fntcmd_store_u8(&v5, 66);
                fntcmd_next_line(&v5);
                v26 -= yw->font_yscrl_h;
            }

            if ( v26 > 1 )
            {
                fntcmd_set_yheight(&v5, v26 - 1);

                fntcmd_store_u8(&v5, 66);
                fntcmd_next_line(&v5);
            }

            fntcmd_reset_tileset(&v5, 13);
            fntcmd_store_u8(&v5, 68);
            fntcmd_next_line(&v5);
        }

        if ( lstvw->field_1D0 & 4 )
        {
            fntcmd_reset_tileset(&v5, 11);
            fntcmd_store_u8(&v5, 71);
        }
    }
    else
    {
        memset(lstvw->frm_1.field_18[2], 0, sizeof(button_str1));
        memset(lstvw->frm_1.field_18[3], 0, sizeof(button_str1));
        memset(lstvw->frm_1.field_18[4], 0, sizeof(button_str1));
        memset(lstvw->frm_1.field_18[5], 0, sizeof(button_str1));
        memset(lstvw->frm_1.field_18[6], 0, sizeof(button_str1));
    }
    fntcmd_set_end(&v5);
}

void lstvw_update_elements_areas(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    lstvw->frm_1.field_10 = lstvw->element_count + 8;

    int v3 = lstvw->field_1E8;

    if ( lstvw->field_1D0 & 2 )
        v3 += yw->font_default_h;

    for (int i = 0; i < lstvw->element_count; i++)
    {
        lstvw->frm_1.field_18[8 + i]->xpos = 0;
        lstvw->frm_1.field_18[8 + i]->ypos = v3 + i * lstvw->font_h;
        lstvw->frm_1.field_18[8 + i]->width = lstvw->width;
        lstvw->frm_1.field_18[8 + i]->fnt_height = lstvw->font_h;
    }
}


int lstvw_init(_NC_STACK_ypaworld *yw, listview *lstvw, ...)
{
    stack_vals vals[128];

    va_list va;
    va_start(va, lstvw);

    va_to_arr(vals, 128, va);

    va_end(va);

    const char *v2 = (const char *)find_id_in_stack_def_val(0x80000001, 0, vals);
    int v9 = find_id_in_stack_def_val(0x80000002, 0, vals);
    int v13 = find_id_in_stack_def_val(0x80000008, 0, vals);
    int v15 = find_id_in_stack_def_val(0x80000009, 0, vals);
    int v8 = find_id_in_stack_def_val(0x8000000F, 0, vals);
    int v11 = find_id_in_stack_def_val(0x80000011, 0, vals);
    int v12 = find_id_in_stack_def_val(0x80000013, 0, vals);
    int v10 = find_id_in_stack_def_val(0x80000017, 0, vals);
    int v14 = find_id_in_stack_def_val(0x80000018, 1, vals);

    lstvw->field_1D4 = find_id_in_stack_def_val(0x80000014, 65, vals);
    lstvw->elements_for_scroll_size = find_id_in_stack_def_val(0x80000003, 0, vals);
    lstvw->element_count = find_id_in_stack_def_val(0x80000004, 0, vals);
    lstvw->scroll_pos = find_id_in_stack_def_val(0x80000005, 0, vals);
    lstvw->field_1DE = find_id_in_stack_def_val(0x80000006, 0, vals);
    lstvw->element_count_max = find_id_in_stack_def_val(0x80000007, 0, vals);
    lstvw->element_count_min = find_id_in_stack_def_val(0x80000012, 0, vals);
    lstvw->font_h = find_id_in_stack_def_val(0x8000000B, yw->font_default_h, vals);
    lstvw->width = find_id_in_stack_def_val(0x8000000C, 80, vals);
    lstvw->width_min = find_id_in_stack_def_val(0x8000000D, 60, vals);
    lstvw->width_max = find_id_in_stack_def_val(0x8000000E, 1024, vals);
    lstvw->field_1E8 = find_id_in_stack_def_val(0x80000015, 0, vals);
    lstvw->field_1EA = find_id_in_stack_def_val(0x80000016, 0, vals);



    if ( !lstvw->field_1E8 && !lstvw->field_1EA )
    {
        lstvw->field_1E8 = find_id_in_stack_def_val(0x80000010, 0, vals);;
        lstvw->field_1EA = lstvw->field_1E8;
    }

    lstvw->field_1D0 = 0;
    lstvw->field_1E4 = -1;

    if ( v8 )
        lstvw->field_1D0 |= 1;

    if ( v2 )
    {
        lstvw->field_1D0 |= 2;
        strncpy(lstvw->wnd_title, v2, 64);
    }


    if ( v9 )
        lstvw->field_1D0 |= 4;

    if ( v11 )
        lstvw->field_1D0 |= 0x10;

    if ( v12 )
        lstvw->field_1D0 |= 0x100;

    if ( v10 )
        lstvw->field_1D0 |= 0x800;

    if ( v13 )
    {
        if ( lstvw->field_1D0 & 2 )
            lstvw->field_1D0 |= 8;
    }

    lstvw->cmd_flag = 0;

    if ( lstvw->field_1D0 & 8 )
        lstvw->cmd_flag |= 5;
    else
        lstvw->cmd_flag |= 0x20;

    if ( lstvw->field_1D0 & 2 )
    {
        lstvw->cmd_flag |= 0x18;

        if ( v14 )
            lstvw->cmd_flag |= 0x100;
    }


    if ( lstvw->field_1D0 & 8 )
    {
        lstvw->frm_2.btn_xpos = 16 * v15;
        lstvw->frm_2.btn_width = 16;
        lstvw->frm_2.btn_height = 16;
        lstvw->frm_2.field_10 = 1;
        lstvw->frm_2.btn_ypos = yw->screen_height - 32;
    }
    else
    {
        lstvw->frm_2.btn_ypos = 0;
        lstvw->frm_2.btn_width = 0;
        lstvw->frm_2.btn_height = 0;
        lstvw->frm_2.field_10 = 0;
        lstvw->frm_2.btn_xpos = 0;
    }


    if ( !sb_0x4e947c__sub0(yw, lstvw, vals) )
    {
        return 0;
    }



    lstvw_updlimits(yw, lstvw, -1, -1);

    if ( !sb_0x4e947c__sub1(yw, lstvw) )
    {
        return 0;
    }


    lstvw_update_main(yw, lstvw);
    lstvw_update_slider(yw, lstvw);
    lstvw_update_elements_areas(yw, lstvw);

    return 1;
}


char *lstvw_up_border(_NC_STACK_ypaworld *yw, listview *lstvw, char *cmdbuf, int tileset, const char *a5)
{
    int v14 = lstvw->frm_1.btn_xpos - (yw->screen_width / 2);
    int v12 = lstvw->frm_1.btn_ypos - (yw->screen_height / 2);

    if ( lstvw->field_1D0 & 2 )
        v12 += yw->font_default_h;

    char *tmp = cmdbuf;
    fntcmd_reset_tileset(&tmp, tileset);
    fntcmd_set_center_xpos(&tmp, v14);
    fntcmd_set_center_ypos(&tmp, v12);

    fntcmd_set_yheight(&tmp, lstvw->field_1E8);

    fntcmd_store_s8(&tmp, a5[0]);

    fntcmd_op17(&tmp, lstvw->width - yw->font_default_w__b);

    fntcmd_store_s8(&tmp, a5[1]);
    fntcmd_store_s8(&tmp, a5[2]);
    fntcmd_next_line(&tmp);

    return tmp;
}

char * lstvw_txt_line_textual(_NC_STACK_ypaworld *yw, char *cmdbuf, listview_t1 *arg)
{
    tiles_stru *v3 = yw->tiles[ arg->tileset_id ];

    int v4 = arg->field_width;

    if ( arg->flags & 1 )
        v4 -= v3->chars[arg->left_tile].width;

    if ( arg->flags & 2 )
        v4 -= v3->chars[arg->right_tile].width;

    char *tmp = cmdbuf;
    fntcmd_select_tileset(&tmp, arg->tileset_id);


    if ( arg->flags & 1 )
        fntcmd_store_s8(&tmp, arg->left_tile);

    if ( v4 > 0 )
    {
        fntcmd_copy_position(&tmp);

        int v8 = v4;

        while (v8 > 0)
        {
            if ( v8 <= 255 )
            {
                fntcmd_op10(&tmp, v8);
            }
            else
            {
                fntcmd_op10(&tmp, 255);
            }

            fntcmd_store_u8(&tmp, arg->bkg_tile);
            v8 -= 255;
        }

        int v20 = 0;

        if ( arg->flags & 4 )
        {
            v20 = 1;
        }
        else if ( arg->flags & 8 )
        {
            v20 = 2;
        }
        else
        {
            v20 = 4;
        }

        fntcmd_add_txt(&tmp, v4, v20, arg->txt);
    }

    if ( arg->flags & 2 )
    {
        fntcmd_store_s8(&tmp, arg->right_tile);
    }
    return tmp;
}

char *lstvw_txt_line_tiles(_NC_STACK_ypaworld *yw, char *cmdbuf, listview_t1 *arg)
{
    tiles_stru *v30 = yw->tiles[ arg->tileset_id ];

    int v28 = 0;
    int v29 = 0;
    int v27 = 0;
    int v26 = 0;

    int v6 = 0;
    const char *v8 = arg->txt;
    while ( *v8 )
    {
        v6 += v30->chars[ (uint8_t)*v8 ].width;
        v8++;
    }

    int v10 = arg->field_width - v6;
    if ( arg->flags & 1 )
        v10 -= v30->chars[arg->left_tile].width;
    if ( arg->flags & 2 )
        v10 -= v30->chars[arg->right_tile].width;

    char *tmp = cmdbuf;
    fntcmd_select_tileset(&tmp, arg->tileset_id);

    if ( v10 <= 0 )
    {
        if ( v10 < 0 )
        {
            if ( arg->flags & 4 )
            {
                v29 = -v10;
            }
            else if ( arg->flags & 8 )
            {
                v28 = -v10;
            }
            else if ( arg->flags & 0x10 )
            {
                v28 = (-v10) / 2;
                v29 = (-v10) - ((-v10) / 2);
            }
        }
    }
    else
    {
        if ( arg->flags & 4 )
        {
            v26 = v10;
        }
        else if ( arg->flags & 8 )
        {
            v27 = v10;
        }
        else if ( arg->flags & 0x10 )
        {
            v27 = v10 / 2;
            v26 = v10 - (v10 / 2);
        }
    }

    int v16 = v6 - v29 - v28;

    if ( arg->flags & 1 )
        fntcmd_store_u8(&tmp, arg->left_tile);

    int chr = 0;

    const char *v7 = arg->txt;

    while (v28 > 0)
    {
        chr = *v7;
        v7++;
        v28 -= v30->chars[(uint8_t)chr].width;
    }

    if ( v28 < 0 )
    {
        fntcmd_set_xoff(&tmp, v28 + v30->chars[(uint8_t)chr].width);
        fntcmd_store_u8(&tmp, chr);
    }

    if ( v27 > 0 )
    {
        while ( v27 > 255 )
        {
            fntcmd_op10(&tmp, 255);
            fntcmd_store_u8(&tmp, arg->bkg_tile);
            v27 -= 255;
        }

        if ( v27 > 0 )
        {
            fntcmd_op10(&tmp, v27);
            fntcmd_store_u8(&tmp, arg->bkg_tile);
        }
    }

    if (v16 > 0)
    {
        chr = 0;
        while ( v16 >= 0 )
        {
            chr = *v7;
            v7++;
            if ( chr == 0)
                break;

            v16 -= v30->chars[(uint8_t)chr].width;
            fntcmd_store_u8(&tmp, chr);
        }
        if (chr)
        {
            fntcmd_set_xwidth(&tmp, v16 + v30->chars[(uint8_t)chr].width);
            fntcmd_store_u8(&tmp, chr);
        }
    }

    if ( v26 > 0 )
    {
        while ( v26 > 255 )
        {
            fntcmd_op10(&tmp, 255);
            fntcmd_store_u8(&tmp, arg->bkg_tile);
            v26 -= 255;
        }
        if ( v26 > 0 )
        {
            fntcmd_op10(&tmp, v26);
            fntcmd_store_u8(&tmp, arg->bkg_tile);
        }
    }
    if ( arg->flags & 2 )
        fntcmd_store_u8(&tmp, arg->right_tile);

    return tmp;
}


char * lstvw_txt_line(_NC_STACK_ypaworld *yw, char *a2, int a3, listview_t1 *a4)
{
    char *tmp = a2;
    for (int i = 0; i < a3; i++)
        if (a4[i].flags & 0x20)
            tmp = lstvw_txt_line_textual(yw, tmp, &a4[i]);
        else
            tmp = lstvw_txt_line_tiles(yw, tmp, &a4[i]);

    return tmp;
}


char * lstvw_down_border(_NC_STACK_ypaworld *yw, listview *lstvw, char *a4, int tileset, const char *a5)
{
    char *tmp = a4;
    fntcmd_reset_tileset(&tmp, tileset);
    fntcmd_set_yoff(&tmp, yw->tiles[tileset]->font_height - lstvw->field_1EA);
    fntcmd_store_s8(&tmp, a5[0]);
    fntcmd_op17(&tmp, lstvw->width - yw->font_default_w__b);
    fntcmd_store_s8(&tmp, a5[1]);
    fntcmd_store_s8(&tmp, a5[2]);
    return tmp;
}


void lstvw_update_input(_NC_STACK_ypaworld *yw, listview *lstvw, struC5 *struc)
{
    if ( lstvw->cmd_flag & 0x21 )
    {
        lstvw->field_1D0 &= 0xFFFFFF1F;
        return;
    }

    winp_131arg *v6 = &struc->winp131arg;

    int v3 = lstvw->frm_1.btn_width;
    int v41 = lstvw->frm_1.btn_height;

    lstvw->field_1D0 &= 0xFFFFFBFF;

    int xpos = lstvw->frm_1.btn_xpos;

    int ypos = lstvw->frm_1.btn_ypos;

    lstvw->field_1E4 = -1;



    if ( lstvw->field_1D0 & 0x800 )
    {
        if ( struc->downed_key == VK_UP )
        {
            lstvw->field_1DE--;
            if ( lstvw->field_1DE < 0 )
            {
                lstvw->field_1DE = lstvw->elements_for_scroll_size - 1;
                lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count;
            }
            if ( lstvw->field_1DE < lstvw->scroll_pos )
            {
                lstvw->scroll_pos = lstvw->field_1DE;
            }
        }
        else if ( struc->downed_key == VK_DOWN )
        {
            lstvw->field_1DE++;
            if ( lstvw->field_1DE >= lstvw->elements_for_scroll_size )
            {
                lstvw->scroll_pos = 0;
                lstvw->field_1DE = 0;
            }
            if ( lstvw->scroll_pos + lstvw->element_count - 1 <= lstvw->field_1DE )
            {
                lstvw->scroll_pos = lstvw->field_1DE - lstvw->element_count + 1;
            }
        }
    }

    yw->field_17bc = (lstvw->field_1D0 & 0xE0) && (v6->flag & 4);

    //printf("%x %x %d, \n", (int)&lstvw->frm_1 , (int)struc->winp131arg.selected_btn , struc->winp131arg.selected_btnID);

    if ( &lstvw->frm_1 == struc->winp131arg.selected_btn && struc->winp131arg.selected_btnID >= 8 )
        lstvw->field_1D0 &= 0xFFFFFDFF;

    if ( lstvw->field_1D0 & 0x20 )
    {
        if ( v6->flag & 4 )
        {
            int xps = v6->move[0].x + lstvw->field_23A;
            int yps = v6->move[0].y + lstvw->field_23C;

            int v43 = lstvw->field_1EA + lstvw->field_1E8 + lstvw->font_h * lstvw->element_count_max;
            int v44 = lstvw->field_1EA + lstvw->field_1E8 + lstvw->font_h * lstvw->element_count_min;

            if ( lstvw->field_1D0 & 2 )
            {
                v44 += yw->font_default_h;
                v43 += yw->font_default_h;
            }

            int v16 = yw->font_yscrl_bkg_w + lstvw->width_max;

            if ( xps > v16 )
                xps = v16;
            else
            {
                v16 = yw->font_yscrl_bkg_w + lstvw->width_min;

                if ( xps < v16 )
                    xps = v16;
            }

            if ( yps > v43 )
                yps = v43;
            else if ( yps < v44)
                yps = v44;

            if ( xps + xpos >= yw->screen_width )
                xps = yw->screen_width - xpos;

            if ( ypos + yps >= yw->screen_height - yw->icon0___h )
                yps = yw->screen_height - yw->icon0___h - ypos;

            if ( lstvw->field_1D0 & 2 )
                lstvw->element_count = (yps - lstvw->field_1EA - lstvw->field_1E8 - yw->font_default_h) / lstvw->font_h;
            else
                lstvw->element_count = (yps - lstvw->field_1EA - lstvw->field_1E8 ) / lstvw->font_h;

            if ( lstvw->scroll_pos + lstvw->element_count > lstvw->elements_for_scroll_size )
            {
                lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count;
                if ( lstvw->scroll_pos < 0 )
                    lstvw->scroll_pos = 0;
            }

            lstvw->width = xps - yw->font_yscrl_bkg_w;

            lstvw_updlimits(yw, lstvw, -2, -2);
        }
        else
        {
            lstvw->field_1D0 &= 0xFFFFFFDF;
        }
    }
    else if ( lstvw->field_1D0 & 0x40 )
    {
        if ( v6->flag & 4 )
        {
            lstvw->field_246 += struc->winp131arg.move[0].y - lstvw->field_240;
            lstvw->field_240 = struc->winp131arg.move[0].y;
            if ( lstvw->field_246 >= 0 )
            {
                if ( lstvw->field_246 + lstvw->field_244 > lstvw->field_242 )
                    lstvw->field_246 = lstvw->field_242 - lstvw->field_244;
            }
            else
            {
                lstvw->field_246 = 0;
            }
        }
        else
        {
            lstvw->field_1D0 &= 0xFFFFFFBF;
        }
    }
    else if ( lstvw->field_1D0 & 0x80 )
    {
        if ( (&lstvw->frm_1 == struc->winp131arg.selected_btn) && (struc->winp131arg.selected_btnID >= 8) )
        {
            if ( v6->flag & 0xC )
            {
                lstvw->field_1DE = lstvw->scroll_pos + struc->winp131arg.selected_btnID - 8;
                lstvw->field_1E4 = lstvw->field_1DE;

                if ( v6->flag & 8 )
                {
                    lstvw->field_1D0 &= 0xFFFFFB7Fu;
                    lstvw->field_1D0 |= 0x400;

                    if ( lstvw->field_1D0 & 0x10 )
                    {
                        lstvw->cmd_flag |= 0x20;
                        sub_412D9C(struc->winp131arg.selected_btn);
                    }
                }
            }
            else
            {
                lstvw->field_1D0 &= 0xFFFFFF7F;

            }
        }
        else if ( !(v6->flag & 4) || lstvw->field_1D0 & 0x200 )
        {
            if ( !(v6->flag & 4) )
            {
                lstvw->field_1D0 &= 0xFFFFFF7F;
            }
        }
        else
        {
            button_str1 *v40 = lstvw->frm_1.field_18[8];
            button_str1 *v27 = lstvw->frm_1.field_18[lstvw->element_count + 7];

            if ( lstvw->field_1CC > 0 )
            {
                lstvw->field_1CC -= struc->period;
            }
            else
            {
                lstvw->field_1CC = 70;

                if ( struc->winp131arg.move[0].y >= ypos + v40->ypos )
                {
                    if ( struc->winp131arg.move[0].y > ypos + v27->ypos + v27->fnt_height )
                    {
                        lstvw->scroll_pos++;

                        if ( lstvw->element_count + lstvw->scroll_pos > lstvw->elements_for_scroll_size )
                        {
                            lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count;
                            if ( lstvw->scroll_pos < 0 )
                                lstvw->scroll_pos = 0;
                        }
                        lstvw->field_1DE = lstvw->element_count + lstvw->scroll_pos - 1;
                    }
                }
                else
                {
                    lstvw->scroll_pos--;

                    if ( lstvw->scroll_pos < 0 )
                        lstvw->scroll_pos = 0;

                    lstvw->field_1DE = lstvw->scroll_pos;
                }
            }

            if ( struc->winp131arg.move[0].y >= ypos + v40->ypos )
            {
                if ( struc->winp131arg.move[0].y > ypos + v27->ypos + v27->fnt_height )
                    lstvw->field_1DE = lstvw->element_count + lstvw->scroll_pos - 1;
            }
            else
            {
                lstvw->field_1DE = lstvw->scroll_pos;
            }
        }
    }
    else if ( &lstvw->frm_1 == struc->winp131arg.selected_btn )
    {
        if ( struc->winp131arg.selected_btnID >= 8 )
            lstvw->field_1E4 = struc->winp131arg.selected_btnID + lstvw->scroll_pos - 8;

        if ( struc->winp131arg.selected_btnID == 7 )
        {
            if ( v6->flag & 0x30 )
                lstvw->cmd_flag |= 0x80;
            if ( v6->flag & 0x40 )
                lstvw->cmd_flag &= 0xFFFFFF7F;
        }

        if ( v6->flag & 0x20 )
        {
            if ( struc->winp131arg.selected_btnID == 2 )
            {
                if ( lstvw->field_1CC > 0 )
                {
                    lstvw->field_1CC -= struc->period;
                }
                else
                {
                    lstvw->field_1CC = 70;

                    lstvw->scroll_pos--;
                    if ( lstvw->scroll_pos < 0 )
                        lstvw->scroll_pos = 0;
                }
            }
            else if ( struc->winp131arg.selected_btnID == 4 )
            {
                if ( lstvw->field_1CC > 0 )
                {
                    lstvw->field_1CC -= struc->period;
                }
                else
                {
                    lstvw->scroll_pos++;
                    lstvw->field_1CC = 70;
                    if ( lstvw->element_count + lstvw->scroll_pos > lstvw->elements_for_scroll_size )
                    {
                        lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count;
                        if ( lstvw->scroll_pos < 0 )
                            lstvw->scroll_pos = 0;
                    }
                }
            }
        }

        if ( v6->flag & 0x10 )
        {
            if ( v6->selected_btnID == 3 )
            {
                lstvw->field_1D0 |= 0x40;
                lstvw->field_240 = v6->ldw_pos[0].y;
            }
            else if ( v6->selected_btnID == 5 )
            {
                lstvw->field_1D0 |= 0x20;
                lstvw->field_23A = v3 - v6->ldw_pos[0].x;
                lstvw->field_23C = v41 - v6->ldw_pos[0].y;
            }

            if ( v6->selected_btnID >= 8 )
            {
                lstvw->field_1D0 |= 0x80;
                lstvw->field_1DE = lstvw->field_1E4;
            }
        }
    }
    else if ( lstvw->field_1D0 & 0x10 )
    {
        if ( v6->flag & 2 )
        {
            lstvw->cmd_flag |= 0x20;
            sub_412D9C(&lstvw->frm_1);
        }
    }
}

void sub_4C31EC(_NC_STACK_ypaworld *yw, listview *lstview)
{
    if ( lstview->cmd_flag & 0x20 )
    {
        lstview->cmd_flag &= 0xFFFFFFDF;
        sub_412D48(&lstview->frm_1, 0);
        yw->field_17bc = 0;
    }
}

void lstvw_update(_NC_STACK_ypaworld *yw, listview *lstview)
{
    lstvw_update_main(yw, lstview);
    lstvw_update_slider(yw, lstview);
    if ( !(lstview->field_1D0 & 0x100) )
        lstvw_update_elements_areas(yw, lstview);
}

void  sub_4C31C0(_NC_STACK_ypaworld *yw, listview *lstvw)
{
    if ( !(lstvw->cmd_flag & 0x20) )
    {
        lstvw->cmd_flag |= 0x20;
        sub_412D9C(&lstvw->frm_1);
        yw->field_17bc = 0;
    }
}

void sub_4DDFA4(listview *lstvw, int a2)
{
    int v2 = lstvw->element_count_max + lstvw->scroll_pos;

    if ( a2 >= lstvw->scroll_pos && a2 <= (v2 - 1) )
    {
        if ( v2 > lstvw->elements_for_scroll_size )
            lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count_max;
        if ( lstvw->scroll_pos < 0 )
            lstvw->scroll_pos = 0;
    }
    else if ( lstvw->elements_for_scroll_size - a2 <= lstvw->element_count_max )
    {
        lstvw->scroll_pos = lstvw->elements_for_scroll_size - lstvw->element_count_max;
    }
    else
    {
        lstvw->scroll_pos = a2;
    }
}


void sub_4E866C(listview *lstvw)
{
    if ( lstvw->field_1C4 )
    {
        nc_FreeMem(lstvw->field_1C4);
        lstvw->field_1C4 = NULL;
    }

    if ( lstvw->data_cmdbuf )
    {
        nc_FreeMem(lstvw->data_cmdbuf);
        lstvw->data_cmdbuf = NULL;
    }

    if ( lstvw->slider_cmdbuf )
    {
        nc_FreeMem(lstvw->slider_cmdbuf);
        lstvw->slider_cmdbuf = NULL;
    }

    if ( lstvw->draw_cmd )
    {
        nc_FreeMem(lstvw->draw_cmd);
        lstvw->draw_cmd = NULL;
    }

    if ( lstvw->field_1BC )
    {
        nc_FreeMem(lstvw->field_1BC);
        lstvw->field_1BC = NULL;
    }

    if ( lstvw->frm_1.field_18[0] )
    {
        nc_FreeMem(lstvw->frm_1.field_18[0]);
        lstvw->frm_1.field_18[0] = NULL;
    }
}
