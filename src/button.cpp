#include <inttypes.h>
#include "includes.h"
#include "nucleas.h"
#include "button.h"
#include "utils.h"
#include "engine_gfx.h"
#include "engine_input.h"

#include "font.h"


const Nucleus::ClassDescr NC_STACK_button::description("button.class", &newinstance);

void NC_STACK_button::clear()
{
    x = 0;
    y = 0;
    w = 0;
    h = 0;
    buttons.clear();

    field_d8.clear();
    idd = 0;
    visible = false;
    field_19c = 0;
    field_19D = 0;
    field_19E = 0;
    field_19F = 0;
    field_1A0 = 0;
    field_1A1 = 0;
    screen_width = 0;
    screen_height = 0;
}

size_t NC_STACK_button::func0(IDVList &stak)
{
    if ( !NC_STACK_nucleus::func0(stak) )
        return 0;

    field_19F = 97;
    field_1A0 = 115;
    field_1A1 = 99;

    field_19c = 97;
    field_19D = 99;
    field_19E = 32;

    for( auto& it : stak )
    {
        IDVPair &val = it.second;

        if ( !val.Skip )
        {
            switch (val.ID)
            {
            case BTN_ATT_X:
                setBTN_x(val.Get<int32_t>());
                break;
            case BTN_ATT_Y:
                setBTN_y(val.Get<int32_t>());
                break;
            case BTN_ATT_W:
                setBTN_w(val.Get<int32_t>());
                break;
            case BTN_ATT_H:
                setBTN_h(val.Get<int32_t>());
                break;
            case BTN_ATT_CHARS:
                setBTN_chars( val.Get<std::string>() );
                break;

            default:
                break;
            }
        }
    }

    if ( w > 0 && h > 0 )
    {
        screen_width = GFXEngine::GFXe.getScreenW();
        screen_height = GFXEngine::GFXe.getScreenH();
    }
    else
    {
        func1();
        return 0;
    }

    return 1;
}

size_t NC_STACK_button::func1()
{
    for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
    {
        if (it->button_type == NC_STACK_button::TYPE_SLIDER)
            delete it->field_41C;
    }

    field_d8.clear();
    buttons.clear();

    return NC_STACK_nucleus::func1();
}


// Update slider
void NC_STACK_button::sub_436F58(NC_STACK_button *btn, button_str2 *sbt)
{
    Slider *sbttt = sbt->field_41C;

    if ( sbttt->value > sbttt->max )
        sbttt->value = sbttt->max;

    if ( sbttt->value < sbttt->min)
        sbttt->value = sbttt->min;

    int v7 = sbt->width / (sbttt->max - sbttt->min + 1) + 1;

    if ( v7 < 6 )
        v7 = 6;

    sbttt->field_6_ = (float)((sbttt->value - sbttt->min) * (sbt->width - v7) / (sbttt->max - sbttt->min)) + 0.5;

    if ( sbttt->field_6_ < 0 )
        sbttt->field_6_ = 0;

    sbttt->field_8_ = v7 + sbttt->field_6_;

    if ( sbttt->field_8_ > sbt->width )
        sbttt->field_8_ = sbt->width;

    char *cpt = sbt->caption;

    if ( sbttt->field_6_ > 0 )
    {
        if ( sbt->flags & FLAG_BORDER )
        {
            FontUA::store_u8(&cpt, btn->field_19F);

            if ( sbttt->field_6_ > 1 )
            {
                FontUA::op17(&cpt, sbttt->field_6_);
                FontUA::store_u8(&cpt, btn->field_1A0);
            }
        }
        else
        {
            FontUA::op17(&cpt, sbttt->field_6_);
            FontUA::store_u8(&cpt, btn->field_1A0);
        }
    }

    FontUA::store_u8(&cpt, btn->field_19c);

    FontUA::op17(&cpt, sbttt->field_8_);

    FontUA::store_u8(&cpt, btn->field_19E);
    FontUA::store_u8(&cpt, btn->field_19D);

    if ( sbt->flags & FLAG_BORDER )
    {
        if ( sbttt->field_8_ < sbt->width - 1 )
        {
            FontUA::op17(&cpt, sbt->width - 1);

            FontUA::store_u8(&cpt, btn->field_1A0);
            FontUA::store_u8(&cpt, btn->field_1A1);
        }
    }
    else if ( sbttt->field_8_ < sbt->width )
    {
        FontUA::op17(&cpt, sbt->width - 1);

        FontUA::store_u8(&cpt, btn->field_1A0);
    }
    FontUA::set_end(&cpt);
}

size_t NC_STACK_button::button_func64(button_64_arg *arg)
{
    /*if ( idd >= 48 )
        return 0;*/

    buttons.emplace_back();
    field_d8.emplace_back(); //field_d8[idd] = (button_str2 *)AllocVec(sizeof(button_str2), 65537);

    button_str2 &sbt = field_d8[idd];
    ButtonBox &bt = buttons[idd];

    if (   arg->button_type != TYPE_BUTTON
            && arg->button_type != TYPE_CHECKBX
            && arg->button_type != TYPE_CAPTION
            && arg->button_type != TYPE_RADIOBTN
            && arg->button_type != TYPE_SLIDER )
        return 0;

    sbt.button_type = arg->button_type;

    if ( arg->button_type != NC_STACK_button::TYPE_SLIDER )
    {
        if ( !arg->caption )
            return 0;

        strncpy(sbt.caption, arg->caption, 511);

        if ( arg->caption2 )
            strncpy(sbt.caption2, arg->caption2, 511);
        else
            strcpy(sbt.caption2, sbt.caption);
    }

    sbt.xpos = arg->xpos;
    sbt.ypos = arg->ypos;
    sbt.width = arg->width;

    if ( sbt.xpos < 0 || sbt.ypos < 0 || sbt.width < 0 || sbt.width + sbt.xpos > w )
        return 0;

    sbt.down_id = arg->down_id;
    sbt.up_id = arg->up_id;
    sbt.pressed_id = arg->pressed_id;
    sbt.txt_r = arg->txt_r;
    sbt.txt_g = arg->txt_g;
    sbt.txt_b = arg->txt_b;

    bt.x = sbt.xpos;
    bt.y = sbt.ypos;
    bt.w = sbt.width;

    if ( sbt.button_type == TYPE_CAPTION )
        bt.h = 0;
    else
        bt.h = GFXEngine::GFXe.getTileset( arg->tileset_up )->h;

    idd++;

    sbt.button_id = arg->button_id;
    sbt.tileset_down = arg->tileset_down;
    sbt.tileset_up = arg->tileset_up;
    sbt.field_42E = arg->field_3A;
    sbt.flags = arg->flags;

    sbt.flags |= FLAG_DRAW;

    if ( sbt.button_type == TYPE_SLIDER )
    {
        if ( !arg->field_34 )
            return 0;

        Slider *sbtt = new Slider;
        if ( sbtt )
        {
            *sbtt = *arg->field_34;

            sbt.field_41C = sbtt;

            sub_436F58(this, &sbt);
            return 1;
        }
    }
    else
        return 1;

    return 0;
}

size_t NC_STACK_button::button_func65(int butID)
{
    int id = button_func72(butID);

    if ( id >= 0 && id < idd )
    {
        if (field_d8[id].field_41C)
            delete field_d8[id].field_41C;

        field_d8.erase( field_d8.begin() + id );
        buttons.erase( buttons.begin() + id );

        idd--;

        return 1;
    }

    return 0;
}

size_t NC_STACK_button::button_func66(button_66arg *arg)
{
    int id = button_func72(arg->butID);

    if ( id >= 0 && id < idd )
    {
        if ( field_d8[id].button_type != TYPE_CAPTION )
        {
            buttons[id].w = field_d8[id].width;
            buttons[id].h = GFXEngine::GFXe.getTileset( field_d8[id].tileset_down )->h;
        }

        field_d8[id].flags &= ~FLAG_DISABLED;
        field_d8[id].flags |= FLAG_DRAW;
        return 1;
    }

    return 0;
}

size_t NC_STACK_button::button_func67(button_66arg *arg)
{
    int id = button_func72(arg->butID);

    if ( id >= 0 && id < idd )
    {
        buttons[id].w = 0;
        buttons[id].h = 0;
        field_d8[id].flags |= FLAG_DISABLED;

        if ( !arg->field_4 )
            field_d8[id].flags &= ~FLAG_DRAW;

        return 1;
    }

    return 0;
}

size_t NC_STACK_button::Show()
{
    if ( !visible )
    {
        visible = true;
        INPe.AddClickBoxFront(this);
    }

    return 1;
}

size_t NC_STACK_button::Hide()
{
    if ( visible )
    {
        visible = false;
        INPe.RemClickBox(this);
    }

    return 1;
}

NC_STACK_button::ResCode NC_STACK_button::button_func69(InputState *arg)
{
    ResCode result = ResCode(0);

    if ( !(arg->ClickInf.flag & ClickBoxInf::FLAG_OK) )
        return ResCode(0);

    if ( arg->ClickInf.flag & ClickBoxInf::FLAG_LM_DOWN )
    {
        for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
        {
            it->flags &= ~FLAG_DOWN;
        }
    }

    if (arg->ClickInf.flag & ClickBoxInf::FLAG_LM_UP)
    {
        for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
        {
            if ( it->button_type == TYPE_SLIDER )
            {
                if ( it->flags & FLAG_DOWN )
                {
                    it->flags &= ~(FLAG_PRESSED | FLAG_DOWN);
                    it->field_41C->pressPart = 0;

                    return ResCode(it->up_id, it->button_id);
                }
            }
        }
    }

    if ( arg->ClickInf.flag & ClickBoxInf::FLAG_LM_HOLD )
    {
        if ( this == arg->ClickInf.selected_btn && arg->ClickInf.selected_btnID != -1 && (field_d8[arg->ClickInf.selected_btnID].flags & FLAG_DOWN) )
        {
            button_str2 &sbt = field_d8[arg->ClickInf.selected_btnID];

            if ( sbt.button_type == TYPE_BUTTON )
                sbt.flags |= FLAG_PRESSED;
        }
        else
        {
            for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
            {
                if (it->button_type == TYPE_BUTTON)
                    it->flags &= ~FLAG_PRESSED;
            }
        }

        for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
        {
            if (it->button_type == TYPE_SLIDER && it->flags & FLAG_DOWN)
            {
                Slider *sbttt = it->field_41C;

                if (sbttt->pressPart == 2)
                {
                    if (it->flags & FLAG_PRESSED)
                        result = ResCode(it->pressed_id, it->button_id);

                    int v21 = (arg->ClickInf.move.ScreenPos.x - sbttt->scrDownX) * (sbttt->max - sbttt->min + 1);

                    sbttt->value = sbttt->oldValue + v21 / it->width;

                    if ( sbttt->value < sbttt->min )
                        sbttt->value = sbttt->min;

                    if ( sbttt->value > sbttt->max )
                        sbttt->value = sbttt->max;

                    sub_436F58(this, &(*it));
                }
            }
        }
    }

    if ( this == arg->ClickInf.selected_btn )
    {
        if ( arg->ClickInf.flag & ClickBoxInf::FLAG_LM_DOWN )
            result = ResCode(3);

        if ( arg->ClickInf.selected_btnID != -1 && arg->ClickInf.selected_btnID < (int)field_d8.size())
        {
            button_str2 &sbt = field_d8[arg->ClickInf.selected_btnID];
            result = ResCode(0, sbt.button_id);

            switch ( sbt.button_type )
            {
            case TYPE_BUTTON:
                if ( arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_DOWN )
                {
                    sbt.flags |= FLAG_PRESSED | FLAG_DOWN;
                    result = ResCode(sbt.down_id, sbt.button_id);
                }

                if ( arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_HOLD )
                {
                    if ( sbt.flags & FLAG_PRESSED )
                        result = ResCode(sbt.pressed_id, sbt.button_id);
                }

                if ( (arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_UP) )
                {
                    if ( sbt.flags & FLAG_DOWN )
                    {
                        sbt.flags &= ~(FLAG_PRESSED | FLAG_DOWN);
                        result = ResCode(sbt.up_id, sbt.button_id);
                    }
                }

                break;

            case TYPE_CHECKBX:
                if ( !(arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_DOWN) )
                    return ResCode(0, sbt.button_id);

                if ( sbt.flags & FLAG_PRESSED )
                {
                    sbt.flags &= ~FLAG_PRESSED;
                    return ResCode(sbt.up_id, sbt.button_id);
                }
                else
                {
                    sbt.flags |= FLAG_PRESSED;
                    return ResCode(sbt.down_id, sbt.button_id);
                }
                break;

            case TYPE_RADIOBTN:
                if ( arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_DOWN )
                {
                    UnsetRadioButtons();
                    sbt.flags |= FLAG_PRESSED | FLAG_DOWN;
                    result = ResCode(sbt.down_id, sbt.button_id);
                }
                break;

            case TYPE_SLIDER:
                if ( !(arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_UP) )
                {
                    if ( arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_DOWN )
                    {
                        Slider *sbttt = sbt.field_41C;
                        if ( arg->ClickInf.ldw_pos.BtnPos.x < sbttt->field_6_ )
                        {
                            sbttt->pressPart = 1;
                            if ( sbttt->value > sbttt->min )
                                sbttt->value -= 1;
                        }
                        else if ( arg->ClickInf.ldw_pos.BtnPos.x <= sbttt->field_8_ )
                        {
                            sbttt->pressPart = 2;
                            sbttt->oldValue = sbttt->value;
                            sbttt->scrDownX = arg->ClickInf.ldw_pos.ScreenPos.x;
                        }
                        else
                        {
                            sbttt->pressPart = 3;
                            if ( sbttt->value < sbttt->max )
                                sbttt->value += 1;
                        }

                        sbt.flags |= FLAG_PRESSED | FLAG_DOWN;
                        result = ResCode(sbt.down_id, sbt.button_id);
                    }
                    else if ( arg->ClickInf.flag & ClickBoxInf::FLAG_BTN_HOLD )
                    {
                        result = ResCode(sbt.pressed_id, sbt.button_id);
                    }
                }
                sub_436F58(this, &sbt);
                break;

            default:
                break;
            }
        }
    }
    return result;
}


void NC_STACK_button::button_func70__sub1(NC_STACK_button *btn, button_str2 *sbt, char **pbuf)
{
    int v6;

    if ( sbt->flags & FLAG_DISABLED )
    {
        v6 = sbt->field_42E;
    }
    else if ( !(sbt->flags & FLAG_PRESSED) || (sbt->flags & FLAG_NOPRESS) )
    {
        v6 = sbt->tileset_up;
    }
    else
    {
        v6 = sbt->tileset_down;
    }

    TileMap *v7 = GFXEngine::GFXe.getTileset(v6);


    char *v8 = *pbuf;

    FontUA::select_tileset(&v8, v6);

    FontUA::set_center_xpos(&v8, sbt->xpos + btn->x - (btn->screen_width / 2));

    FontUA::set_center_ypos(&v8, sbt->ypos + btn->y - (btn->screen_height / 2));

    int tmp = sbt->width;

    if ( sbt->button_type != TYPE_CAPTION )
    {
        if ( sbt->flags & FLAG_BORDER )
        {
            FontUA::store_u8(&v8, btn->field_19c); //Padding gfx
            tmp += -v7->map[btn->field_19c].w - v7->map[btn->field_19D].w;
        }
    }

    FontUA::copy_position(&v8);

    FontUA::op17(&v8, tmp);

    FontUA::store_u8(&v8, btn->field_19E); //Padding gfx

    if ( sbt->button_type != TYPE_CAPTION )
    {
        if ( sbt->flags & FLAG_BORDER )
        {
            FontUA::store_u8(&v8, btn->field_19D); //Padding gfx
        }
    }

    int v17;
    char *v18;

    if ( sbt->flags & FLAG_PRESSED )
    {
        if ( sbt->flags & FLAG_NOPRESS )
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

    if ( sbt->flags & FLAG_CENTER )
    {
        v17 |= 4;
    }
    else if ( sbt->flags & FLAG_RALIGN )
    {
        v17 |= 2;
    }
    else
    {
        v17 |= 1;
    }

    FontUA::set_txtColor(&v8, sbt->txt_r, sbt->txt_g, sbt->txt_b);

    FontUA::add_txt(&v8, tmp, v17, v18);

    *pbuf = v8;
}

void NC_STACK_button::button_func70__sub0(NC_STACK_button *btn, button_str2 *sbt, char **pbuf)
{
    char *v5 = *pbuf;
    int v6 = 0;
    int v7;

    if ( sbt->flags & FLAG_DISABLED )
    {
        v7 = sbt->field_42E;
    }
    else if ( sbt->flags & FLAG_PRESSED )
    {
        v7 = sbt->tileset_down;
    }
    else
    {
        v7 = sbt->tileset_up;
    }

    TileMap *v8 = GFXEngine::GFXe.getTileset(v7);

    int strwdth = 0;

    if ( sbt->flags & FLAG_PRESSED )
    {
        int caplen = strlen(sbt->caption2);

        for (int i = 0; i < caplen; i++)
            strwdth += v8->map[(uint8_t)sbt->caption2[i]].w;
    }
    else
    {
        int caplen = strlen(sbt->caption);

        for (int i = 0; i < caplen; i++)
            strwdth += v8->map[(uint8_t)sbt->caption[i]].w;
    }

    int v47 = (sbt->width - strwdth - v8->map[btn->field_19c].w) / 2;

    FontUA::select_tileset(&v5, v7);

    FontUA::set_center_xpos(&v5, btn->x + sbt->xpos - (btn->screen_width / 2));
    FontUA::set_center_ypos(&v5, btn->y + sbt->ypos - (btn->screen_height / 2));

    if ( sbt->button_type == TYPE_SLIDER )
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

        if ( sbt->button_type != TYPE_CAPTION )
        {
            if ( sbt->flags & FLAG_BORDER )
            {
                FontUA::store_u8(&v5, btn->field_19c);
                ttmp += -v8->map[btn->field_19c].w - v8->map[btn->field_19D].w;
            }
        }

        if ( v47 > 0 )
        {
            if ( sbt->flags & FLAG_CENTER )
            {
                FontUA::op10(&v5, v47);

                FontUA::store_u8(&v5, btn->field_19E);

                v6 = v47;
            }
        }

        char *capt;

        if ( sbt->flags & FLAG_PRESSED )
            capt = sbt->caption2;
        else
            capt = sbt->caption;

        int v26 = 0;
        while ( capt[v26] )
        {
            Common::PointRect &pr = v8->map[(uint8_t)capt[v26]];
            if ( pr.w + v6 > ttmp )
            {
                if ( (ttmp - v6) > 2 )
                {
                    FontUA::set_xwidth(&v5, ttmp - v6);

                    FontUA::store_s8(&v5, capt[v26]);

                    v6 = ttmp;
                }
                break;
            }

            v6 += pr.w;

            FontUA::store_s8(&v5, capt[v26]);

            v26++;
        }

        if ( v6 < ttmp )
        {
            if ( sbt->button_type != TYPE_CAPTION && sbt->flags & FLAG_BORDER )
            {
                FontUA::op17(&v5, sbt->width - v8->map[btn->field_19D].w);
            }
            else
            {
                FontUA::op17(&v5, sbt->width);
            }
            FontUA::store_u8(&v5, btn->field_19E);
        }

        if ( sbt->button_type != TYPE_CAPTION )
        {
            if ( sbt->flags & FLAG_BORDER )
            {
                FontUA::store_u8(&v5, btn->field_19D);
            }
        }
    }
    *pbuf = v5;
}

char button_tmpbuf[5008];

size_t NC_STACK_button::button_func70(void *)
{
    char *pbuf = button_tmpbuf;

    if ( visible )
    {
        for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
        {
            if ( it->flags & FLAG_DRAW )
            {
                if ( it->flags & FLAG_TEXT )
                    button_func70__sub1(this, &*it, &pbuf);
                else
                    button_func70__sub0(this, &*it, &pbuf);
            }
        }
        FontUA::set_end(&pbuf);

        w3d_a209 arg209;
        arg209.cmdbuf = button_tmpbuf;
        arg209.includ = NULL;

        GFXEngine::GFXe.drawText(&arg209);
    }

    return 1;
}

bool NC_STACK_button::button_func71(int butID, const std::string &field_4, const std::string &field_8)
{
    int v5 = button_func72(butID);

    if ( v5 >= 0 && v5 < idd &&  !field_4.empty() )
    {
        button_str2 &v7 = field_d8[v5];

        strncpy(v7.caption, field_4.c_str(), 511);

        if ( !field_8.empty() )
        {
            strncpy(v7.caption2, field_8.c_str(), 511);
        }
        else
        {
            strcpy(v7.caption2, v7.caption);
        }
        return true;
    }

    return false;
}

bool NC_STACK_button::button_func71(int butID, const std::string &field_4)
{
    int v5 = button_func72(butID);

    if ( v5 >= 0 && v5 < idd &&  !field_4.empty() )
    {
        button_str2 &v7 = field_d8[v5];

        strncpy(v7.caption, field_4.c_str(), 511);
        strncpy(v7.caption2, field_4.c_str(), 511);

        return true;
    }

    return false;
}

int NC_STACK_button::button_func72(int butid)
{
    for (unsigned int i = 0; i < field_d8.size(); i++)
    {
        if (field_d8[i].button_id == butid)
            return i;
    }
    return -1;
}

void NC_STACK_button::button_func73(button_66arg *arg)
{
    int id = button_func72(arg->butID);

    if ( id < 0 || id >= idd )
        return;

    button_str2 &b = field_d8[id];

    if ( b.button_type == TYPE_BUTTON || b.button_type == TYPE_CHECKBX || b.button_type == TYPE_RADIOBTN )
    {
        if ( b.button_type == TYPE_RADIOBTN )
            UnsetRadioButtons();

        if ( arg->field_4 == 1 )
            b.flags |= FLAG_PRESSED | FLAG_DOWN;
        else if ( arg->field_4 == 2 )
            b.flags &= ~(FLAG_PRESSED | FLAG_DOWN);
    }
}

NC_STACK_button::Slider * NC_STACK_button::button_func74(int butid)
{
    int id = button_func72(butid);

    if ( id != -1 && field_d8[id].button_type == TYPE_SLIDER)
        return field_d8[id].field_41C;

    return NULL;
}

size_t NC_STACK_button::button_func75(int butid)
{
    int id = button_func72(butid);

    if ( id != -1 && field_d8[id].button_type == TYPE_SLIDER)
        sub_436F58(this, &field_d8[id]);

    return 1;
}

size_t NC_STACK_button::button_func76(button_arg76 *arg)
{
    int id = button_func72(arg->butID);

    if ( id >= 0 && id < idd )
    {
        if (arg->xpos != -1)
        {
            field_d8[id].xpos = arg->xpos;
            buttons[id].x = arg->xpos;
        }

        if (arg->ypos != -1)
        {
            field_d8[id].ypos = arg->ypos;
            buttons[id].y = arg->ypos;
        }

        if (arg->width != -1)
        {
            field_d8[id].width = arg->width;
            buttons[id].w = arg->width;
        }
        return 1;
    }

    return 0;
}


void NC_STACK_button::UnsetRadioButtons()
{
    for (WidgetArr::iterator it = field_d8.begin(); it != field_d8.end(); it++)
    {
        if ( it->button_type == TYPE_RADIOBTN )
            it->flags &= ~(FLAG_DOWN | FLAG_PRESSED);
    }
}



void NC_STACK_button::setBTN_x(int _x)
{
    x = _x;
}

void NC_STACK_button::setBTN_y(int _y)
{
    y = _y;
}

void NC_STACK_button::setBTN_w(int _w)
{
    w = _w;
}

void NC_STACK_button::setBTN_h(int _h)
{
    h = _h;
}

void NC_STACK_button::setBTN_chars(const std::string &chrs)
{
    field_19c = (uint8_t)chrs[0];
    field_19D = (uint8_t)chrs[1];
    field_19E = (uint8_t)chrs[2];
}


int NC_STACK_button::getBTN_x()
{
    return x;
}

int NC_STACK_button::getBTN_y()
{
    return y;
}

int NC_STACK_button::getBTN_w()
{
    return w;
}

int NC_STACK_button::getBTN_h()
{
    return h;
}
