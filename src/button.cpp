#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "button.h"
#include "utils.h"
#include "engine_gfx.h"
#include "engine_input.h"

#include "font.h"


const NewClassDescr NC_STACK_button::description("button.class", &newinstance);

int NC_STACK_button::button_func0__sub0(stack_vals *stak)
{
    __NC_STACK_button *btn = &stack__button;
    stack_vals *stk = stak;

    btn->field_19F = 97;
    btn->field_1A0 = 115;
    btn->field_1A1 = 99;

    btn->field_19c = 97;
    btn->field_19D = 99;
    btn->field_19E = 32;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BTN_ATT_X:
                setBTN_x(stk->value.i_data);
                break;
            case BTN_ATT_Y:
                setBTN_y(stk->value.i_data);
                break;
            case BTN_ATT_W:
                setBTN_w(stk->value.i_data);
                break;
            case BTN_ATT_H:
                setBTN_h(stk->value.i_data);
                break;
            case BTN_ATT_CHARS:
                setBTN_chars((const char *)stk->value.p_data);
                break;
            }
            stk++;
        }
    }

    return 1;
}

size_t NC_STACK_button::func0(stack_vals *stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    __NC_STACK_button *btn = &stack__button;

    if ( button_func0__sub0(stak) )
    {
        if ( btn->btn_width > 0 && btn->btn_height > 0 )
        {
            btn->screen_width = GFXe.getScreenW();
            btn->screen_height = GFXe.getScreenH();
        }
        else
        {
            func1(NULL);
            return 0;
        }
    }
    else
    {
        func1(NULL);
        return 0;
    }

    return 1;
}

size_t NC_STACK_button::func1(stack_vals *stak)
{
    __NC_STACK_button *btn = &stack__button;

    for (int i = 0; i < 48; i++)
    {
        if (btn->field_d8[i])
        {
            if (btn->field_d8[i]->button_type == 5)
                nc_FreeMem(btn->field_d8[i]->field_41C);
            nc_FreeMem(btn->field_d8[i]);
        }

        if (btn->field_18[i])
            nc_FreeMem(btn->field_18[i]);
    }

    return NC_STACK_nucleus::func1(stak);
}

int NC_STACK_button::button_func2__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BTN_ATT_X:
                setBTN_x(stk->value.i_data);
                break;
            case BTN_ATT_Y:
                setBTN_y(stk->value.i_data);
                break;
            case BTN_ATT_W:
                setBTN_w(stk->value.i_data);
                break;
            case BTN_ATT_H:
                setBTN_h(stk->value.i_data);
                break;
            case BTN_ATT_CHARS:
                setBTN_chars((const char *)stk->value.p_data);
                break;
            }
            stk++;
        }
    }

    return 1;
}

size_t NC_STACK_button::func2(stack_vals *stak)
{
    NC_STACK_nucleus::func2(stak);
    button_func2__sub0(stak);

    return 1;
}


int NC_STACK_button::button_func3__sub0(stack_vals *stak)
{
    stack_vals *stk = stak;

    while ( 1 )
    {
        if (stk->id == stack_vals::TAG_END)
            break;
        else if (stk->id == stack_vals::TAG_PTAGS)
        {
            stk = (stack_vals *)stk->value.p_data;
        }
        else if ( stk->id == stack_vals::TAG_SKIP_N )
        {
            stk += stk->value.i_data;
            ////a2++; ////BUGFIX?
        }
        else
        {
            switch ( stk->id )
            {
            default:
                break;

            case BTN_ATT_X:
                *(int *)stk->value.p_data = getBTN_x();
                break;
            case BTN_ATT_Y:
                *(int *)stk->value.p_data = getBTN_y();
                break;
            case BTN_ATT_W:
                *(int *)stk->value.p_data = getBTN_w();
                break;
            case BTN_ATT_H:
                *(int *)stk->value.p_data = getBTN_h();
                break;
            case BTN_ATT_PBTN:
                *(__NC_STACK_button **)stk->value.p_data = getBTN_pButton();
                break;
            }
            stk++;
        }
    }

    return 1;
}

size_t NC_STACK_button::func3(stack_vals *stak)
{
    NC_STACK_nucleus::func3(stak);
    button_func3__sub0(stak);

    return 1;
}

// Update slider
void sub_436F58(__NC_STACK_button *btn, button_str2 *sbt)
{
    button_str2_t2 *sbttt = sbt->field_41C;

    if ( sbttt->field_0 > sbttt->field_2 )
        sbttt->field_0 = sbttt->field_2;

    if ( sbttt->field_0 < sbttt->field_4)
        sbttt->field_0 = sbttt->field_4;

    int v7 = sbt->width / (sbttt->field_2 - sbttt->field_4 + 1) + 1;

    if ( v7 < 6 )
        v7 = 6;

    sbttt->field_6 = (float)((sbttt->field_0 - sbttt->field_4) * (sbt->width - v7) / (sbttt->field_2 - sbttt->field_4)) + 0.5;

    if ( sbttt->field_6 < 0 )
        sbttt->field_6 = 0;

    sbttt->field_8 = v7 + sbttt->field_6;

    if ( sbttt->field_8 > sbt->width )
        sbttt->field_8 = sbt->width;

    char *cpt = sbt->caption;

    if ( sbttt->field_6 > 0 )
    {
        if ( sbt->state & 0x10 )
        {
            fntcmd_store_u8(&cpt, btn->field_19F);

            if ( sbttt->field_6 > 1 )
            {
                fntcmd_op17(&cpt, sbttt->field_6);
                fntcmd_store_u8(&cpt, btn->field_1A0);
            }
        }
        else
        {
            fntcmd_op17(&cpt, sbttt->field_6);
            fntcmd_store_u8(&cpt, btn->field_1A0);
        }
    }

    fntcmd_store_u8(&cpt, btn->field_19c);

    fntcmd_op17(&cpt, sbttt->field_8);

    fntcmd_store_u8(&cpt, btn->field_19E);
    fntcmd_store_u8(&cpt, btn->field_19D);

    if ( sbt->state & 0x10 )
    {
        if ( sbttt->field_8 < sbt->width - 1 )
        {
            fntcmd_op17(&cpt, sbt->width - 1);

            fntcmd_store_u8(&cpt, btn->field_1A0);
            fntcmd_store_u8(&cpt, btn->field_1A1);
        }
    }
    else if ( sbttt->field_8 < sbt->width )
    {
        fntcmd_op17(&cpt, sbt->width - 1);

        fntcmd_store_u8(&cpt, btn->field_1A0);
    }
    fntcmd_set_end(&cpt);
}

size_t NC_STACK_button::button_func64(button_64_arg *arg)
{
    __NC_STACK_button *btn = &stack__button;

    int idd = btn->idd;

    if ( btn->idd >= 48 )
        return 0;

    btn->field_18[idd] = (button_str1 *)AllocVec(sizeof(button_str1), 65537);

    if ( !btn->field_18[idd] )
        return 0;

    btn->field_d8[idd] = (button_str2 *)AllocVec(sizeof(button_str2), 65537);

    button_str2 *sbt = btn->field_d8[idd];

    if ( arg->button_type != 1 && arg->button_type != 2 && arg->button_type != 3 && arg->button_type != 4 && arg->button_type != 5 )
        return 0;

    sbt->button_type = arg->button_type;

    if ( arg->button_type != 5 )
    {
        if ( !arg->caption )
            return 0;

        strncpy(sbt->caption, arg->caption, 511);

        if ( arg->caption2 )
            strncpy(sbt->caption2, arg->caption2, 511);
        else
            strcpy(sbt->caption2, sbt->caption);
    }

    sbt->xpos = arg->xpos;
    sbt->ypos = arg->ypos;
    sbt->width = arg->width;

    if ( sbt->xpos < 0 || sbt->ypos < 0 || sbt->width < 0 || sbt->width + sbt->xpos > btn->btn_width )
        return 0;

    sbt->field_40A = arg->field_1C;
    sbt->down_id = arg->down_id;
    sbt->up_id = arg->up_id;
    sbt->pressed_id = arg->pressed_id;
    sbt->txt_r = arg->txt_r;
    sbt->txt_g = arg->txt_g;
    sbt->txt_b = arg->txt_b;

    btn->idd++;

    button_str1 *bt = btn->field_18[idd];

    bt->xpos = sbt->xpos;
    bt->ypos = sbt->ypos;
    bt->width = sbt->width;

    if ( sbt->button_type == 3 )
        bt->fnt_height = 0;
    else
        bt->fnt_height = GFXe.getTileset( arg->tileset_up )->font_height;

    btn->field_10++;

    sbt->button_id = arg->button_id;
    sbt->tileset_down = arg->tileset_down;
    sbt->tileset_up = arg->tileset_up;
    sbt->field_42E = arg->field_3A;
    sbt->state = arg->state;

    sbt->state |= 8;

    if ( sbt->button_type == 5 )
    {
        if ( !arg->field_34 )
            return 0;

        button_str2_t2 *sbtt = (button_str2_t2 *)AllocVec(sizeof(button_str2_t2), 65537);
        if ( sbtt )
        {
            *sbtt = *arg->field_34;

            sbt->field_41C = sbtt;

            sub_436F58(btn, sbt);
            return 1;
        }
    }
    else
        return 1;

    return 0;
}

size_t NC_STACK_button::button_func65(int *butID)
{
    __NC_STACK_button *btn = &stack__button;

    int id = button_func72(butID);

    if ( id >= 0 && id < btn->idd )
    {
        nc_FreeMem(btn->field_d8[id]);
        nc_FreeMem(btn->field_18[id]);

        for (int i = id; i < 47; i++)
        {
            btn->field_18[i] = btn->field_18[i + 1];
            btn->field_d8[i] = btn->field_d8[i + 1];
        }

        btn->idd--;
        btn->field_18[47] = NULL;
        btn->field_d8[47] = NULL;
        btn->field_10--;

        return 1;
    }

    return 0;
}

size_t NC_STACK_button::button_func66(button_66arg *arg)
{
    __NC_STACK_button *btn = &stack__button;

    int id = button_func72(&arg->butID);

    if ( id >= 0 && id < btn->idd )
    {
        if ( btn->field_d8[id]->button_type != 3 )
        {
            btn->field_18[id]->width = btn->field_d8[id]->width;
            btn->field_18[id]->fnt_height = GFXe.getTileset( btn->field_d8[id]->tileset_down )->font_height;
        }

        btn->field_d8[id]->state &= 0xFFFD;
        btn->field_d8[id]->state |= 8;
        return 1;
    }

    return 0;
}

size_t NC_STACK_button::button_func67(button_66arg *arg)
{
    __NC_STACK_button *btn = &stack__button;

    int id = button_func72(&arg->butID);

    if ( id >= 0 && id < btn->idd )
    {
        btn->field_18[id]->width = 0;
        btn->field_18[id]->fnt_height = 0;
        btn->field_d8[id]->state |= 2;

        if ( !arg->field_4 )
            btn->field_d8[id]->state &= 0xFFF7;

        return 1;
    }

    return 0;
}

size_t NC_STACK_button::button_func68(int *arg)
{
    __NC_STACK_button *btn = &stack__button;

    if ( *arg == 1 )
    {
        if ( !(btn->field_19A & 1) )
        {
            btn->field_19A |= 1;
            INPe.sub_412D48(btn, 0);
        }
    }
    else if ( *arg == 2 )
    {
        if ( btn->field_19A & 1 )
        {
            btn->field_19A &= 0xFFFE;
            INPe.sub_412D9C(btn);
        }
    }

    return 1;
}

size_t NC_STACK_button::button_func69(struC5 *arg)
{
    __NC_STACK_button *btn = &stack__button;

    int v46 = 0;

    if ( !(arg->winp131arg.flag & 1) )
        return 0;

    if ( arg->winp131arg.flag & 2 )
    {
        for (int i = 0; i < 48; i++)
        {
            if (btn->field_d8[i])
                btn->field_d8[i]->state &= 0xFFFB;
        }
    }

    if (arg->winp131arg.flag & 8)
    {
        for (int i = 0; i < 48; i++)
        {
            button_str2 *sbt = btn->field_d8[i];
            if ( sbt )
            {
                if ( sbt->button_type == 5 )
                {
                    if ( sbt->state & 4 )
                    {
                        sbt->state &= 0xFFFB;
                        sbt->state &= 0xFFFE;
                        sbt->field_41C->field_A = 0;

                        return (sbt->button_id << 16) | sbt->up_id;
                    }
                }
            }
        }
    }

    if ( arg->winp131arg.flag & 4 )
    {
        if ( btn == arg->winp131arg.selected_btn && arg->winp131arg.selected_btnID != -1 && (btn->field_d8[arg->winp131arg.selected_btnID]->state & 4) )
        {
            button_str2 *sbt = btn->field_d8[arg->winp131arg.selected_btnID];

            if ( sbt->button_type == 1 )
                sbt->state |= 1;
        }
        else
        {
            for (int i = 0; i < 48; i++)
            {
                if ( btn->field_d8[i] )
                {
                    if ( btn->field_d8[i]->button_type == 1 )
                        btn->field_d8[i]->state &= 0xFFFE;
                }
            }
        }

        for (int i = 0; i < 48; i++)
        {
            button_str2 *sbt = btn->field_d8[i];

            if ( sbt )
            {
                if (sbt->button_type == 5)
                {
                    if (sbt->state & 4)
                    {
                        button_str2_t2 *sbttt = sbt->field_41C;

                        if (sbttt->field_A == 2)
                        {
                            if (sbt->state & 1)
                                v46 = (sbt->button_id << 16) | sbt->pressed_id;

                            int v21 = (arg->winp131arg.move[0].x - sbttt->field_E) * (sbttt->field_2 - sbttt->field_4 + 1);

                            sbttt->field_0 = sbttt->field_C + v21 / sbt->width;

                            if ( sbttt->field_0 < sbttt->field_4 )
                                sbttt->field_0 = sbttt->field_4;

                            if ( sbttt->field_0 > sbttt->field_2 )
                                sbttt->field_0 = sbttt->field_2;

                            sub_436F58(btn, sbt);
                        }
                    }
                }
            }
        }
    }

    if ( btn == arg->winp131arg.selected_btn )
    {
        if ( arg->winp131arg.flag & 2 )
            v46 = 3;

        if ( arg->winp131arg.selected_btnID != -1 )
        {
            button_str2 *sbt = btn->field_d8[arg->winp131arg.selected_btnID];
            if ( sbt )
            {
                v46 = sbt->button_id << 16;

                switch ( sbt->button_type )
                {
                case 4:
                    if ( !(arg->winp131arg.flag & 0x10) )
                        return (sbt->button_id << 16);

                    for (int i = 0; i < 48; i++)
                    {
                        button_str2 *tmp = btn->field_d8[i];
                        if (tmp)
                        {
                            if (tmp->button_type == 4)
                                tmp->state &= 0xFFFA;
                        }
                    }
                    sbt->state |= 5;
                    return (sbt->button_id << 16) | sbt->down_id;

                case 2:
                    if ( !(arg->winp131arg.flag & 0x10) )
                        return (sbt->button_id << 16);

                    if ( sbt->state & 1 )
                    {
                        sbt->state &= 0xFFFE;
                        return sbt->up_id | (sbt->button_id << 16);
                    }
                    else
                    {
                        sbt->state |= 1;
                        return sbt->down_id | (sbt->button_id << 16);
                    }
                    break;

                case 1:
                    if ( arg->winp131arg.flag & 0x10 )
                    {
                        sbt->state |= 5;
                        v46 = (sbt->button_id << 16) | sbt->down_id;
                    }

                    if ( arg->winp131arg.flag & 0x20 )
                    {
                        if ( sbt->state & 1 )
                            v46 = (sbt->button_id << 16) | sbt->pressed_id;
                    }

                    if ( (arg->winp131arg.flag & 0x40) )
                    {
                        if ( sbt->state & 4 )
                        {
                            sbt->state &= 0xFFFA;
                            v46 = (sbt->button_id << 16) | sbt->up_id;
                        }
                    }

                    return v46;
                case 5:
                    if ( !(arg->winp131arg.flag & 0x40) )
                    {
                        if ( arg->winp131arg.flag & 0x10 )
                        {
                            button_str2_t2 *sbttt = sbt->field_41C;
                            if ( arg->winp131arg.ldw_pos[2].x < sbttt->field_6 )
                            {
                                sbttt->field_A = 1;
                                if ( sbttt->field_0 > sbttt->field_4 )
                                    sbttt->field_0 -= 1;
                            }
                            else if ( arg->winp131arg.ldw_pos[2].x <= sbttt->field_8 )
                            {
                                sbttt->field_A = 2;
                                sbttt->field_C = sbttt->field_0;
                                sbttt->field_E = arg->winp131arg.ldw_pos[0].x;
                            }
                            else
                            {
                                sbttt->field_A = 3;
                                if ( sbttt->field_0 < sbttt->field_2 )
                                    sbttt->field_0 += 1;
                            }

                            sbt->state |= 5;
                            v46 = (sbt->button_id << 16) | sbt->down_id;
                        }
                        else if ( arg->winp131arg.flag & 0x20 )
                        {
                            v46 |= sbt->pressed_id;
                        }
                    }
                    sub_436F58(btn, sbt);
                    return v46;
                default:
                    return v46;
                }
            }
        }
    }
    return v46;
}


void button_func70__sub1(__NC_STACK_button *btn, button_str2 *sbt, char **pbuf)
{
    int v6;

    if ( sbt->state & 2 )
    {
        v6 = sbt->field_42E;
    }
    else if ( !(sbt->state & 1) || sbt->state & 0x80 )
    {
        v6 = sbt->tileset_up;
    }
    else
    {
        v6 = sbt->tileset_down;
    }

    tiles_stru *v7 = GFXe.getTileset(v6);


    char *v8 = *pbuf;

    fntcmd_select_tileset(&v8, v6);

    fntcmd_set_center_xpos(&v8, sbt->xpos + btn->btn_xpos - (btn->screen_width / 2));

    fntcmd_set_center_ypos(&v8, sbt->ypos + btn->btn_ypos - (btn->screen_height / 2));

    int tmp = sbt->width;

    if ( sbt->button_type != 3 )
    {
        if ( sbt->state & 0x10 )
        {
            fntcmd_store_u8(&v8, btn->field_19c); //Padding gfx
            tmp += -v7->chars[btn->field_19c].width - v7->chars[btn->field_19D].width;
        }
    }

    fntcmd_copy_position(&v8);

    fntcmd_op17(&v8, tmp);

    fntcmd_store_u8(&v8, btn->field_19E); //Padding gfx

    if ( sbt->button_type != 3 )
    {
        if ( sbt->state & 0x10 )
        {
            fntcmd_store_u8(&v8, btn->field_19D); //Padding gfx
        }
    }

    int v17;
    char *v18;

    if ( sbt->state & 1 )
    {
        if ( sbt->state & 0x80 )
            v17 = 0;
        else
            v17 = 16;

        v18 = sbt->caption2;
    }
    else
    {
        v17 = 0;
        v18 = sbt->caption;
    }

    if ( sbt->state & 0x20 )
    {
        v17 |= 4;
    }
    else if ( sbt->state & 0x100 )
    {
        v17 |= 2;
    }
    else
    {
        v17 |= 1;
    }

    fntcmd_set_txtColor(&v8, sbt->txt_r, sbt->txt_g, sbt->txt_b);

    fntcmd_add_txt(&v8, tmp, v17, v18);

    *pbuf = v8;
}

void button_func70__sub0(__NC_STACK_button *btn, button_str2 *sbt, char **pbuf)
{
    char *v5 = *pbuf;
    int v6 = 0;
    int v7;

    if ( sbt->state & 2 )
    {
        v7 = sbt->field_42E;
    }
    else if ( sbt->state & 1 )
    {
        v7 = sbt->tileset_down;
    }
    else
    {
        v7 = sbt->tileset_up;
    }

    tiles_stru *v8 = GFXe.getTileset(v7);

    int strwdth = 0;

    if ( sbt->state & 1 )
    {
        int caplen = strlen(sbt->caption2);

        for (int i = 0; i < caplen; i++)
            strwdth += v8->chars[(BYTE)sbt->caption2[i]].width;
    }
    else
    {
        int caplen = strlen(sbt->caption);

        for (int i = 0; i < caplen; i++)
            strwdth += v8->chars[(BYTE)sbt->caption[i]].width;
    }

    int v47 = (sbt->width - strwdth - v8->chars[btn->field_19c].width) / 2;

    fntcmd_select_tileset(&v5, v7);

    fntcmd_set_center_xpos(&v5, btn->btn_xpos + sbt->xpos - (btn->screen_width / 2));
    fntcmd_set_center_ypos(&v5, btn->btn_ypos + sbt->ypos - (btn->screen_height / 2));

    if ( sbt->button_type == 5 )
    {
        int v22 = 0;
        while ( sbt->caption[v22] != 0 || sbt->caption[v22 + 1] != 0 )
        {
            *v5 = sbt->caption[v22];
            v22++;
            v5++;
        }
    }
    else
    {
        int16_t ttmp = sbt->width;

        if ( sbt->button_type != 3 )
        {
            if ( sbt->state & 0x10 )
            {
                fntcmd_store_u8(&v5, btn->field_19c);
                ttmp += -v8->chars[btn->field_19c].width - v8->chars[btn->field_19D].width;
            }
        }

        if ( v47 > 0 )
        {
            if ( sbt->state & 0x20 )
            {
                fntcmd_op10(&v5, v47);

                fntcmd_store_u8(&v5, btn->field_19E);

                v6 = v47;
            }
        }

        char *capt;

        if ( sbt->state & 1 )
            capt = sbt->caption2;
        else
            capt = sbt->caption;

        int v26 = 0;
        while ( capt[v26] )
        {
            tile_xy *v28 = &v8->chars[(uint8_t)capt[v26]];
            if ( v28->width + v6 > ttmp )
            {
                if ( (ttmp - v6) > 2 )
                {
                    fntcmd_set_xwidth(&v5, ttmp - v6);

                    fntcmd_store_s8(&v5, capt[v26]);

                    v6 = ttmp;
                }
                break;
            }

            v6 += v28->width;

            fntcmd_store_s8(&v5, capt[v26]);

            v26++;
        }

        if ( v6 < ttmp )
        {
            if ( sbt->button_type != 3 && sbt->state & 0x10 )
            {
                fntcmd_op17(&v5, sbt->width - v8->chars[btn->field_19D].width);
            }
            else
            {
                fntcmd_op17(&v5, sbt->width);
            }
            fntcmd_store_u8(&v5, btn->field_19E);
        }

        if ( sbt->button_type != 3 )
        {
            if ( sbt->state & 0x10 )
            {
                fntcmd_store_u8(&v5, btn->field_19D);
            }
        }
    }
    *pbuf = v5;
}

char button_tmpbuf[5008];

size_t NC_STACK_button::button_func70(void *)
{
    __NC_STACK_button *btn = &stack__button;

    char *pbuf = button_tmpbuf;

    if ( btn->field_19A & 1 )
    {
        for (int i = 0; i < 48 ; i++ )
        {
            if ( !btn->field_d8[i] )
                break;

            if ( btn->field_d8[i]->state & 8 )
            {
                if ( btn->field_d8[i]->state & 0x40 )
                    button_func70__sub1(btn, btn->field_d8[i], &pbuf);
                else
                    button_func70__sub0(btn, btn->field_d8[i], &pbuf);
            }
        }
        fntcmd_set_end(&pbuf);

        w3d_a209 arg209;
        arg209.cmdbuf = button_tmpbuf;
        arg209.includ = NULL;

        GFXe.drawText(&arg209);
    }

    return 1;
}

size_t NC_STACK_button::button_func71(button_71arg *arg)
{
    __NC_STACK_button *btn = &stack__button;
    int v5 = button_func72(&arg->butID);

    if ( v5 >= 0 && v5 < btn->idd &&  arg->field_4 )
    {
        button_str2 *v7 = btn->field_d8[v5];

        strncpy(v7->caption, arg->field_4, 511);

        if ( arg->field_8 )
        {
            strncpy(v7->caption2, arg->field_8, 511);
        }
        else
        {
            strcpy(v7->caption2, v7->caption);
        }
        return 1;
    }

    return 0;
}

int NC_STACK_button::button_func72(int *butid)
{
    __NC_STACK_button *btn = &stack__button;

    for (int i = 0; i < 48; i++)
    {
        if (btn->field_d8[i])
        {
            if (btn->field_d8[i]->button_id == *butid)
                return i;
        }
    }
    return -1;
}

size_t NC_STACK_button::button_func73(button_66arg *arg)
{
    __NC_STACK_button *btn = &stack__button;
    int id = button_func72(&arg->butID);

    if ( id >= 0 && id < btn->idd )
    {
        if ( btn->field_d8[id]->button_type >= 1 )
        {
            if ( btn->field_d8[id]->button_type > 2 )
            {
                if ( btn->field_d8[id]->button_type != 4 )
                    return 0;

                for (int i = 0; i < 48; i++)
                {
                    if (btn->field_d8[i])
                    {
                        if (btn->field_d8[i]->button_type == 4)
                            btn->field_d8[i]->state &= 0xFFFA;
                    }
                }
            }

            if ( arg->field_4 == 1 )
                btn->field_d8[id]->state |= 5;
            else if ( arg->field_4 == 2 )
                btn->field_d8[id]->state &= 0xFFFA;
        }
    }

    return 1;
}

button_str2_t2 * NC_STACK_button::button_func74(int *butid)
{
    __NC_STACK_button *btn = &stack__button;
    int id = button_func72(butid);

    if ( id != -1 && btn->field_d8[id]->button_type == 5)
    {
        return btn->field_d8[id]->field_41C;
    }
    return NULL;
}

size_t NC_STACK_button::button_func75(int *butid)
{
    __NC_STACK_button *btn = &stack__button;
    int id = button_func72(butid);

    if ( id != -1 && btn->field_d8[id]->button_type == 5)
    {
        sub_436F58(btn, btn->field_d8[id]);
    }

    return 1;
}

size_t NC_STACK_button::button_func76(button_arg76 *arg)
{
    __NC_STACK_button *btn = &stack__button;
    int id = button_func72(&arg->butID);

    if ( id >= 0 && id < btn->idd )
    {
        if (arg->xpos != -1)
        {
            btn->field_d8[id]->xpos = arg->xpos;
            btn->field_18[id]->xpos = arg->xpos;
        }

        if (arg->ypos != -1)
        {
            btn->field_d8[id]->ypos = arg->ypos;
            btn->field_18[id]->ypos = arg->ypos;
        }

        if (arg->width != -1)
        {
            btn->field_d8[id]->width = arg->width;
            btn->field_18[id]->width = arg->width;
        }
        return 1;
    }

    return 0;
}



void NC_STACK_button::setBTN_x(int x)
{
    stack__button.btn_xpos = x;
}

void NC_STACK_button::setBTN_y(int y)
{
    stack__button.btn_ypos = y;
}

void NC_STACK_button::setBTN_w(int w)
{
    stack__button.btn_width = w;
}

void NC_STACK_button::setBTN_h(int h)
{
    stack__button.btn_height = h;
}

void NC_STACK_button::setBTN_chars(const char *chrs)
{
    const uint8_t *v4 = (const uint8_t *)chrs;
    stack__button.field_19c = v4[0];
    stack__button.field_19D = v4[1];
    stack__button.field_19E = v4[2];
}


int NC_STACK_button::getBTN_x()
{
    return stack__button.btn_xpos;
}

int NC_STACK_button::getBTN_y()
{
    return stack__button.btn_ypos;
}

int NC_STACK_button::getBTN_w()
{
    return stack__button.btn_width;
}

int NC_STACK_button::getBTN_h()
{
    return stack__button.btn_height;
}

__NC_STACK_button *NC_STACK_button::getBTN_pButton()
{
    return &stack__button;
}


size_t NC_STACK_button::compatcall(int method_id, void *data)
{
    switch( method_id )
    {
    case 0:
        return (size_t)func0( (stack_vals *)data );
    case 1:
        return (size_t)func1( (stack_vals *)data );
    case 2:
        return (size_t)func2( (stack_vals *)data );
    case 3:
        return (size_t)func3( (stack_vals *)data );
    case 64:
        return (size_t)button_func64( (button_64_arg *)data );
    case 65:
        return (size_t)button_func65( (int *)data );
    case 66:
        return (size_t)button_func66( (button_66arg *)data );
    case 67:
        return (size_t)button_func67( (button_66arg *)data );
    case 68:
        return (size_t)button_func68( (int *)data );
    case 69:
        return (size_t)button_func69( (struC5 *)data );
    case 70:
        return (size_t)button_func70( (void *)data );
    case 71:
        return (size_t)button_func71( (button_71arg *)data );
    case 72:
        return (size_t)button_func72( (int *)data );
    case 73:
        return (size_t)button_func73( (button_66arg *)data );
    case 74:
        return (size_t)button_func74( (int *)data );
    case 75:
        return (size_t)button_func75( (int *)data );
    case 76:
        return (size_t)button_func76( (button_arg76 *)data );
    default:
        break;
    }
    return NC_STACK_nucleus::compatcall(method_id, data);
}
