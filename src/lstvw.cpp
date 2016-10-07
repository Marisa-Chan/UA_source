#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "utils.h"

#include "lstvw.h"
#include "font.h"



char *GuiBase::FormateTitle(_NC_STACK_ypaworld *yw, int xpos, int ypos, int w, const char *title, char *in, uint8_t postf_char, int flag)
{
    int v27 = 0;
    if ( flag & FLAG_WITH_CLOSE )
        v27 = yw->tiles[24]->chars[65].width; // Help button

    int v26 = 0;
    if ( flag & FLAG_WITH_HELP )
        v26 = yw->tiles[24]->chars[66].width; // Close button

    int v29 = 0;
    if ( postf_char )
        v29 = yw->tiles[0]->chars[postf_char].width;

    char buf[128];
    strcpy(buf, " ");
    strcat(buf, title);

    char *tmp = in;

    FontUA::select_tileset(&tmp, 6);
    FontUA::set_center_xpos(&tmp, xpos);
    FontUA::set_center_ypos(&tmp, ypos);

    FontUA::store_s8(&tmp, 98);

    FontUA::set_txtColor(&tmp, yw->iniColors[60].r, yw->iniColors[60].g, yw->iniColors[60].b);

    tmp = FontUA::FormateClippedText(yw->tiles[6], tmp, buf, w - v27 - v26 - v29 - yw->font_default_w__b, 99);

    if ( postf_char )
    {
        FontUA::select_tileset(&tmp, 0);
        FontUA::store_u8(&tmp, postf_char);
    }

    if ( flag & FLAG_WITH_HELP )
    {
        int ts = ((flag & FLAG_HELP_DOWN) != 0) + 24;

        FontUA::select_tileset(&tmp, ts);
        FontUA::store_s8(&tmp, 65);
    }

    if ( flag & FLAG_WITH_CLOSE )
    {
        int ts = ((flag & FLAG_CLOSE_DOWN) != 0) + 24;

        FontUA::select_tileset(&tmp, ts);
        FontUA::store_s8(&tmp, 85);
    }
    return tmp;
}


int GuiList::initDialogStrings(_NC_STACK_ypaworld *yw)
{
    char *draw_cmd = (char *)AllocVec(512, 1);

    if ( !draw_cmd )
        return 0;

    int xpos = dialogBox.xpos - (yw->screen_width / 2);
    int ypos = dialogBox.ypos - (yw->screen_height / 2);

    cmdstrm.cmdbuf = draw_cmd;

    char *v7 = draw_cmd;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
    {
        v7 = GuiBase::FormateTitle(yw, xpos, ypos, dialogBox.btn_width, title, draw_cmd, 0, flags);

        FontUA::next_line(&v7);
    }
    else
    {
        FontUA::select_tileset(&v7, 0);
        FontUA::set_center_xpos(&v7, xpos);
        FontUA::set_center_ypos(&v7, ypos);
    }
    FontUA::include(&v7, 1); // Include data part
    FontUA::include(&v7, 0); // Include slider
    FontUA::set_end(&v7);

    return 1;
}

int GuiList::InitBuffers(_NC_STACK_ypaworld *yw)
{
    char *p = (char *)AllocVec(32, 1);

    if ( !p )
        return 0;

    iconString = p;
    FontUA::set_end(&p);

    if ( !initDialogStrings(yw) )
        return 0;


    p = (char *)AllocVec(0x2000, 1);

    if ( !p )
        return 0;

    itemBlock = p;
    FontUA::set_end(&p);

    p = (char *)AllocVec(256, 1);

    if ( !p )
        return 0;

    scrollbar = p;
    FontUA::set_end(&p);

    char **z = (char **)AllocVec(sizeof(char *) * 2, 1);

    if ( !z )
        return 0;

    cmdstrm.includ = z;

    z[0] = scrollbar; // Slider
    z[1] = itemBlock; // Data

    return 1;
}

void GuiList::SetRect(_NC_STACK_ypaworld *yw, int xpos, int ypos)
{
    if ( entryWidth < minEntryWidth)
        entryWidth = minEntryWidth;

    if ( entryWidth > maxEntryWidth)
        entryWidth = maxEntryWidth;

    if ( shownEntries < minShownEntries)
        shownEntries = minShownEntries;

    if ( shownEntries > maxShownEntries)
        shownEntries = maxShownEntries;

    int w = entryWidth;

    if ( (listFlags & GLIST_FLAG_RESIZEABLE) || numEntries > maxShownEntries )
        w += yw->font_yscrl_bkg_w;

    int h = upperVborder + lowerVborder + entryHeight * shownEntries;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        h += yw->font_default_h;

    if ( xpos == -1 )
    {
        xpos = (yw->screen_width / 2) - (w / 2);
    }
    else if ( xpos == -2 )
    {
        xpos = dialogBox.xpos;
    }

    if ( ypos == -1 )
    {
        ypos = (yw->screen_height / 2) - (h / 2);
    }
    else if ( ypos == -2 )
    {
        ypos = dialogBox.ypos;
    }

    if ( xpos < 0 )
        xpos = 0;

    if ( ypos < yw->icon_energy__h )
        ypos = yw->icon_energy__h;

    if ( w + xpos >= yw->screen_width )
        xpos = yw->screen_width - w;

    if ( h + ypos >= yw->screen_height - yw->icon0___h )
        ypos = yw->screen_height - yw->icon0___h - h;

    dialogBox.ypos = ypos;
    dialogBox.btn_width = w;
    dialogBox.btn_height = h;
    dialogBox.xpos = xpos;
}

int GuiList::initButtons()
{
    dialogBox.field_10 = shownEntries + 8;

    ButtonBox* v4 = (ButtonBox *)AllocVec(sizeof(ButtonBox) * 33, 65537);

    if (!v4)
        return 0;

    for (int i = 0; i < 32; i++)
        dialogBox.buttons[i] = &v4[i];


    if ( listFlags & GLIST_FLAG_WITH_ICON )
    {
        iconBox.field_10 = 1;
        iconBox.buttons[0] = &v4[32];
        iconBox.buttons[0]->x = 0;
        iconBox.buttons[0]->y = 0;
        iconBox.buttons[0]->w = 16;
        iconBox.buttons[0]->h = 16;
    }
    else
    {
        iconBox.field_10 = 0;
    }

    return 1;
}

void GuiList::FormateTitle(_NC_STACK_ypaworld *yw)
{
    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
    {
        char *v5 = cmdstrm.cmdbuf;

        int v6 = (dialogBox.xpos) - (yw->screen_width / 2);
        int v9 = (dialogBox.ypos) - (yw->screen_height / 2);

        int v17, v16;
        if ( flags & FLAG_WITH_CLOSE )
            v17 = yw->font_default_w__a;
        else
            v17 = 0;

        if ( flags & FLAG_WITH_HELP )
            v16 = yw->font_default_w__a;
        else
            v16 = 0;

        if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        {
            v5 = GuiBase::FormateTitle(yw, v6, v9, dialogBox.btn_width, title, v5, 0, flags);
            FontUA::next_line(&v5);
        }
        else
        {
            FontUA::select_tileset(&v5, 0);
            FontUA::set_center_xpos(&v5, v6);
            FontUA::set_center_ypos(&v5, v9);
        }

        FontUA::include(&v5, 1); // Data
        FontUA::include(&v5, 0); // Slider
        FontUA::set_end(&v5);

        int v14 = dialogBox.btn_width - v17 - v16;

        dialogBox.buttons[1]->x = 0;
        dialogBox.buttons[1]->y = 0;
        dialogBox.buttons[1]->w = v14;
        dialogBox.buttons[1]->h = yw->font_default_h;

        if ( flags & FLAG_WITH_HELP )
        {
            dialogBox.buttons[7]->x = v14;
            dialogBox.buttons[7]->y = 0;
            dialogBox.buttons[7]->w = v17;
            dialogBox.buttons[7]->h = yw->font_default_h;
        }
        else
        {
            dialogBox.buttons[7]->x = 0;
            dialogBox.buttons[7]->y = 0;
            dialogBox.buttons[7]->w = 0;
            dialogBox.buttons[7]->h = 0;
        }

        if ( flags & FLAG_WITH_CLOSE )
        {
            dialogBox.buttons[0]->x = dialogBox.btn_width - v17;
            dialogBox.buttons[0]->y = 0;
            dialogBox.buttons[0]->w = v17;
            dialogBox.buttons[0]->h = yw->font_default_h;
        }
        else
        {
            dialogBox.buttons[0]->x = 0;
            dialogBox.buttons[0]->y = 0;
            dialogBox.buttons[0]->w = 0;
            dialogBox.buttons[0]->h = 0;
        }
    }
}

void GuiList::ScrollParamsFromEntries(_NC_STACK_ypaworld *yw)
{
    int v3 = dialogBox.btn_height;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        v3 -= yw->font_default_h;

    if ( listFlags & GLIST_FLAG_RESIZEABLE )
        v3 -= yw->font_xscrl_h;

    int v4, v5;

    if ( numEntries )
    {
        v4 = v3 * shownEntries / numEntries;
        v5 = v3 * firstShownEntries / numEntries;
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

    btnSize = v4;
    btnStart = v5;
    scrlSize = v3;
}

void GuiList::FormateScrollbar(_NC_STACK_ypaworld *yw)
{
    char *v5 = scrollbar;

    if ( listFlags & GLIST_FLAG_RESIZEABLE || numEntries > maxShownEntries )
    {

        if ( listFlags & GLIST_FLAG_IN_SCROLLING )
        {
            firstShownEntries = numEntries * btnStart  / scrlSize;

            if ( shownEntries + firstShownEntries > numEntries)
            {
                firstShownEntries = numEntries - shownEntries;

                if ( firstShownEntries < 0 )
                    firstShownEntries = 0;
            }
        }
        else
        {
            ScrollParamsFromEntries(yw);
        }

        int v34 = dialogBox.btn_width - yw->font_yscrl_bkg_w;
        int v35 = 0;

        if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
            v35 = yw->font_default_h;

        dialogBox.buttons[2]->x = v34; // Scroll up part
        dialogBox.buttons[2]->y = v35;
        dialogBox.buttons[2]->w = yw->font_yscrl_bkg_w;
        dialogBox.buttons[2]->h = btnStart;
        dialogBox.buttons[3]->x = v34; // Scroll position
        dialogBox.buttons[3]->y = btnStart + v35;
        dialogBox.buttons[3]->w = yw->font_yscrl_bkg_w;
        dialogBox.buttons[3]->h = btnSize;
        dialogBox.buttons[4]->x = v34; // Scroll down part
        dialogBox.buttons[4]->y = btnSize + btnStart + v35;
        dialogBox.buttons[4]->w = yw->font_yscrl_bkg_w;
        dialogBox.buttons[4]->h = scrlSize - (btnSize + btnStart);
        dialogBox.buttons[6]->x = 0;
        dialogBox.buttons[6]->y = dialogBox.btn_height - lowerVborder;
        dialogBox.buttons[6]->w = dialogBox.btn_width - yw->font_yscrl_bkg_w;
        dialogBox.buttons[6]->h = lowerVborder;

        if ( listFlags & GLIST_FLAG_RESIZEABLE )
        {
            dialogBox.buttons[5]->x = v34; // Resize button
            dialogBox.buttons[5]->y = scrlSize + v35;
            dialogBox.buttons[5]->w = yw->font_yscrl_bkg_w;
            dialogBox.buttons[5]->h = yw->font_xscrl_h;
        }
        else
        {
            memset(dialogBox.buttons[5], 0, sizeof(ButtonBox));
        }

        int v7 = dialogBox.xpos + v34 - (yw->screen_width / 2);
        int v11 = dialogBox.ypos + v35 - (yw->screen_height / 2);

        FontUA::set_center_xpos(&v5, v7);
        FontUA::set_center_ypos(&v5, v11);

        if ( btnStart > 0 )
        {
            FontUA::reset_tileset(&v5, 13);
            FontUA::store_u8(&v5, 67);
            FontUA::next_line(&v5);
            FontUA::reset_tileset(&v5, 12);

            int v15 = btnStart - 1;

            while (v15 >= yw->font_yscrl_h)
            {
                FontUA::store_u8(&v5, 66);
                FontUA::next_line(&v5);
                v15 -= yw->font_yscrl_h;
            }

            if ( v15 > 0 )
            {
                FontUA::set_yheight(&v5, v15);

                FontUA::store_u8(&v5, 66);
                FontUA::next_line(&v5);
            }
        }


        if ( btnSize > 0 )
        {
            FontUA::reset_tileset(&v5, 13);
            FontUA::store_u8(&v5, 69);
            FontUA::next_line(&v5);
            FontUA::reset_tileset(&v5, 12);

            int v15 = btnSize - 1;

            while (v15 > yw->font_yscrl_h)
            {
                FontUA::store_u8(&v5, 67);
                FontUA::next_line(&v5);
                v15 -= yw->font_yscrl_h;
            }

            if ( v15 > 1 )
            {
                FontUA::set_yheight(&v5, v15 - 1);

                FontUA::store_u8(&v5, 67);
                FontUA::next_line(&v5);
            }
        }

        FontUA::reset_tileset(&v5, 13);
        FontUA::store_u8(&v5, 70);
        FontUA::next_line(&v5);

        int v26 = scrlSize - (btnStart  + btnSize);

        if ( v26 > 0 )
        {
            FontUA::reset_tileset(&v5, 12);

            while (v26 > yw->font_yscrl_h)
            {
                FontUA::store_u8(&v5, 66);
                FontUA::next_line(&v5);
                v26 -= yw->font_yscrl_h;
            }

            if ( v26 > 1 )
            {
                FontUA::set_yheight(&v5, v26 - 1);

                FontUA::store_u8(&v5, 66);
                FontUA::next_line(&v5);
            }

            FontUA::reset_tileset(&v5, 13);
            FontUA::store_u8(&v5, 68);
            FontUA::next_line(&v5);
        }

        if ( listFlags & GLIST_FLAG_RESIZEABLE )
        {
            FontUA::reset_tileset(&v5, 11);
            FontUA::store_u8(&v5, 71);
        }
    }
    else
    {
        memset(dialogBox.buttons[2], 0, sizeof(ButtonBox));
        memset(dialogBox.buttons[3], 0, sizeof(ButtonBox));
        memset(dialogBox.buttons[4], 0, sizeof(ButtonBox));
        memset(dialogBox.buttons[5], 0, sizeof(ButtonBox));
        memset(dialogBox.buttons[6], 0, sizeof(ButtonBox));
    }
    FontUA::set_end(&v5);
}

void GuiList::FormateItemBlock(_NC_STACK_ypaworld *yw)
{
    dialogBox.field_10 = shownEntries + 8;

    int v3 = upperVborder;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        v3 += yw->font_default_h;

    for (int i = 0; i < shownEntries; i++)
    {
        dialogBox.buttons[8 + i]->x = 0;
        dialogBox.buttons[8 + i]->y = v3 + i * entryHeight;
        dialogBox.buttons[8 + i]->w = entryWidth;
        dialogBox.buttons[8 + i]->h = entryHeight;
    }
}


int GuiList::Init(_NC_STACK_ypaworld *yw, tInit &in)
{
    closeChar = in.closeChar;
    numEntries = in.numEntries;
    shownEntries = in.shownEntries;
    firstShownEntries = in.firstShownEntry;
    selectedEntry = in.selectedEntry;
    maxShownEntries = in.maxShownEntries;
    minShownEntries = in.minShownEntries;
    entryHeight = in.entryHeight;
    entryWidth = in.entryWidth;
    minEntryWidth = in.minEntryWidth;
    maxEntryWidth = in.maxEntryWidth;
    upperVborder = in.upperVborder;
    lowerVborder = in.lowerVborder;



    if ( !upperVborder && !lowerVborder )
    {
        lowerVborder = upperVborder = in.vborder;
    }

    listFlags = 0;
    mouseItem = -1;

    if ( in.enabled )
        listFlags |= GLIST_FLAG_ENABLED;

    if ( in.title )
    {
        listFlags |= GLIST_FLAG_WITH_TITLEBAR;
        strncpy(title, in.title, 64);
    }


    if ( in.resizeable )
        listFlags |= GLIST_FLAG_RESIZEABLE;

    if ( in.instantInput )
        listFlags |= GLIST_FLAG_INSTANT_INPUT;

    if ( in.staticItems )
        listFlags |= GLIST_FLAG_STATIC;

    if ( in.keyboardInput )
        listFlags |= GLIST_FLAG_KEYB_INPUT;

    if ( in.withIcon )
    {
        if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
            listFlags |= GLIST_FLAG_WITH_ICON;
    }

    flags = 0;

    if ( listFlags & GLIST_FLAG_WITH_ICON )
        flags |= (FLAG_ICONIFED | FLAG_WITH_ICON);
    else
        flags |= FLAG_CLOSED;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
    {
        flags |= (FLAG_WITH_CLOSE | FLAG_WITH_DRAGBAR);

        if ( in.withHelp )
            flags |= FLAG_WITH_HELP;
    }


    if ( listFlags & GLIST_FLAG_WITH_ICON )
    {
        iconBox.xpos = 16 * in.iconPos;
        iconBox.btn_width = 16;
        iconBox.btn_height = 16;
        iconBox.field_10 = 1;
        iconBox.ypos = yw->screen_height - 32;
    }
    else
    {
        iconBox.ypos = 0;
        iconBox.btn_width = 0;
        iconBox.btn_height = 0;
        iconBox.field_10 = 0;
        iconBox.xpos = 0;
    }


    if ( !InitBuffers(yw) )
    {
        return 0;
    }



    SetRect(yw, -1, -1);

    if ( !initButtons() )
    {
        return 0;
    }


    FormateTitle(yw);
    FormateScrollbar(yw);
    FormateItemBlock(yw);

    return 1;
}


char *GuiList::ItemsPreLayout(_NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5)
{
    int v14 = dialogBox.xpos - (yw->screen_width / 2);
    int v12 = dialogBox.ypos - (yw->screen_height / 2);

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        v12 += yw->font_default_h;

    char *tmp = cmdbuf;
    FontUA::reset_tileset(&tmp, tileset);
    FontUA::set_center_xpos(&tmp, v14);
    FontUA::set_center_ypos(&tmp, v12);

    FontUA::set_yheight(&tmp, upperVborder);

    FontUA::store_s8(&tmp, a5[0]);

    FontUA::op17(&tmp, entryWidth - yw->font_default_w__b);

    FontUA::store_s8(&tmp, a5[1]);
    FontUA::store_s8(&tmp, a5[2]);
    FontUA::next_line(&tmp);

    return tmp;
}

char *GuiList::ItemsPostLayout(_NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5)
{
    char *tmp = cmdbuf;
    FontUA::reset_tileset(&tmp, tileset);
    FontUA::set_yoff(&tmp, yw->tiles[tileset]->font_height - lowerVborder);
    FontUA::store_s8(&tmp, a5[0]);
    FontUA::op17(&tmp, entryWidth - yw->font_default_w__b);
    FontUA::store_s8(&tmp, a5[1]);
    FontUA::store_s8(&tmp, a5[2]);
    return tmp;
}


void GuiList::InputHandle(_NC_STACK_ypaworld *yw, struC5 *struc)
{
    if ( flags & (FLAG_CLOSED | FLAG_ICONIFED) )
    {
        listFlags &= ~(GLIST_FLAG_IN_RESIZING | GLIST_FLAG_IN_SCROLLING | GLIST_FLAG_IN_SELECT);
        return;
    }

    winp_131arg *v6 = &struc->winp131arg;

    int v3 = dialogBox.btn_width;
    int v41 = dialogBox.btn_height;

    listFlags &= ~GLIST_FLAG_SEL_DONE;

    int xpos = dialogBox.xpos;

    int ypos = dialogBox.ypos;

    mouseItem = -1;



    if ( listFlags & GLIST_FLAG_KEYB_INPUT )
    {
        if ( struc->downed_key == UAVK_UP )
        {
            selectedEntry--;
            if ( selectedEntry < 0 )
            {
                selectedEntry = numEntries - 1;
                firstShownEntries = numEntries - shownEntries;
            }
            if ( selectedEntry < firstShownEntries )
            {
                firstShownEntries = selectedEntry;
            }
        }
        else if ( struc->downed_key == UAVK_DOWN )
        {
            selectedEntry++;
            if ( selectedEntry >= numEntries )
            {
                firstShownEntries = 0;
                selectedEntry = 0;
            }
            if ( firstShownEntries + shownEntries - 1 <= selectedEntry )
            {
                firstShownEntries = selectedEntry - shownEntries + 1;
            }
        }
    }

    yw->field_17bc = (listFlags & (GLIST_FLAG_IN_RESIZING | GLIST_FLAG_IN_SCROLLING | GLIST_FLAG_IN_SELECT)) && (v6->flag & 4);

    //printf("%x %x %d, \n", (int)&dialogBox , (int)struc->winp131arg.selected_btn , struc->winp131arg.selected_btnID);

    if ( &dialogBox == struc->winp131arg.selected_btn && struc->winp131arg.selected_btnID >= 8 )
        listFlags &= ~GLIST_FLAG_NO_SCROLL;

    if ( listFlags & GLIST_FLAG_IN_RESIZING )
    {
        if ( v6->flag & 4 )
        {
            int xps = v6->move[0].x + rszX;
            int yps = v6->move[0].y + rszY;

            int v43 = lowerVborder + upperVborder + entryHeight * maxShownEntries;
            int v44 = lowerVborder + upperVborder + entryHeight * minShownEntries;

            if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
            {
                v44 += yw->font_default_h;
                v43 += yw->font_default_h;
            }

            int v16 = yw->font_yscrl_bkg_w + maxEntryWidth;

            if ( xps > v16 )
                xps = v16;
            else
            {
                v16 = yw->font_yscrl_bkg_w + minEntryWidth;

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

            if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
                shownEntries = (yps - lowerVborder - upperVborder - yw->font_default_h) / entryHeight;
            else
                shownEntries = (yps - lowerVborder - upperVborder ) / entryHeight;

            if ( firstShownEntries + shownEntries > numEntries )
            {
                firstShownEntries = numEntries - shownEntries;
                if ( firstShownEntries < 0 )
                    firstShownEntries = 0;
            }

            entryWidth = xps - yw->font_yscrl_bkg_w;

            SetRect(yw, -2, -2);
        }
        else
        {
            listFlags &= ~GLIST_FLAG_IN_RESIZING;
        }
    }
    else if ( listFlags & GLIST_FLAG_IN_SCROLLING )
    {
        if ( v6->flag & 4 )
        {
            btnStart += struc->winp131arg.move[0].y - scrlY;
            scrlY = struc->winp131arg.move[0].y;
            if ( btnStart >= 0 )
            {
                if ( btnStart + btnSize > scrlSize )
                    btnStart = scrlSize - btnSize;
            }
            else
            {
                btnStart = 0;
            }
        }
        else
        {
            listFlags &= ~GLIST_FLAG_IN_SCROLLING;
        }
    }
    else if ( listFlags & GLIST_FLAG_IN_SELECT )
    {
        if ( (&dialogBox == struc->winp131arg.selected_btn) && (struc->winp131arg.selected_btnID >= 8) )
        {
            if ( v6->flag & 0xC )
            {
                selectedEntry = firstShownEntries + struc->winp131arg.selected_btnID - 8;
                mouseItem = selectedEntry;

                if ( v6->flag & 8 )
                {
                    listFlags &= ~(GLIST_FLAG_SEL_DONE | GLIST_FLAG_IN_SELECT);
                    listFlags |= GLIST_FLAG_SEL_DONE;

                    if ( listFlags & GLIST_FLAG_INSTANT_INPUT )
                    {
                        flags |= FLAG_CLOSED;
                        INPe.RemClickBox(struc->winp131arg.selected_btn);
                    }
                }
            }
            else
            {
                listFlags &= ~GLIST_FLAG_IN_SELECT;

            }
        }
        else if ( !(v6->flag & 4) || listFlags & GLIST_FLAG_NO_SCROLL )
        {
            if ( !(v6->flag & 4) )
            {
                listFlags &= ~GLIST_FLAG_IN_SELECT;
            }
        }
        else
        {
            ButtonBox *v40 = dialogBox.buttons[8];
            ButtonBox *v27 = dialogBox.buttons[shownEntries + 7];

            if ( scrollTimer > 0 )
            {
                scrollTimer -= struc->period;
            }
            else
            {
                scrollTimer = 70;

                if ( struc->winp131arg.move[0].y >= ypos + v40->y )
                {
                    if ( struc->winp131arg.move[0].y > ypos + v27->y + v27->h )
                    {
                        firstShownEntries++;

                        if ( shownEntries + firstShownEntries > numEntries )
                        {
                            firstShownEntries = numEntries - shownEntries;
                            if ( firstShownEntries < 0 )
                                firstShownEntries = 0;
                        }
                        selectedEntry = shownEntries + firstShownEntries - 1;
                    }
                }
                else
                {
                    firstShownEntries--;

                    if ( firstShownEntries < 0 )
                        firstShownEntries = 0;

                    selectedEntry = firstShownEntries;
                }
            }

            if ( struc->winp131arg.move[0].y >= ypos + v40->y )
            {
                if ( struc->winp131arg.move[0].y > ypos + v27->y + v27->h )
                    selectedEntry = shownEntries + firstShownEntries - 1;
            }
            else
            {
                selectedEntry = firstShownEntries;
            }
        }
    }
    else if ( &dialogBox == struc->winp131arg.selected_btn )
    {
        if ( struc->winp131arg.selected_btnID >= 8 )
            mouseItem = struc->winp131arg.selected_btnID + firstShownEntries - 8;

        if ( struc->winp131arg.selected_btnID == 7 )
        {
            if ( v6->flag & 0x30 )
                flags |= FLAG_HELP_DOWN;
            if ( v6->flag & 0x40 )
                flags &= ~FLAG_HELP_DOWN;
        }

        if ( v6->flag & 0x20 )
        {
            if ( struc->winp131arg.selected_btnID == 2 )
            {
                if ( scrollTimer > 0 )
                {
                    scrollTimer -= struc->period;
                }
                else
                {
                    scrollTimer = 70;

                    firstShownEntries--;
                    if ( firstShownEntries < 0 )
                        firstShownEntries = 0;
                }
            }
            else if ( struc->winp131arg.selected_btnID == 4 )
            {
                if ( scrollTimer > 0 )
                {
                    scrollTimer -= struc->period;
                }
                else
                {
                    firstShownEntries++;
                    scrollTimer = 70;
                    if ( shownEntries + firstShownEntries > numEntries )
                    {
                        firstShownEntries = numEntries - shownEntries;
                        if ( firstShownEntries < 0 )
                            firstShownEntries = 0;
                    }
                }
            }
        }

        if ( v6->flag & 0x10 )
        {
            if ( v6->selected_btnID == 3 )
            {
                listFlags |= GLIST_FLAG_IN_SCROLLING;
                scrlY = v6->ldw_pos[0].y;
            }
            else if ( v6->selected_btnID == 5 )
            {
                listFlags |= GLIST_FLAG_IN_RESIZING;
                rszX = v3 - v6->ldw_pos[0].x;
                rszY = v41 - v6->ldw_pos[0].y;
            }

            if ( v6->selected_btnID >= 8 )
            {
                listFlags |= GLIST_FLAG_IN_SELECT;
                selectedEntry = mouseItem;
            }
        }
    }
    else if ( listFlags & GLIST_FLAG_INSTANT_INPUT )
    {
        if ( v6->flag & 2 )
        {
            flags |= FLAG_CLOSED;
            INPe.RemClickBox(&dialogBox);
        }
    }
}

void GuiBase::OpenDialog(_NC_STACK_ypaworld *yw)
{
    if ( flags & FLAG_CLOSED )
    {
        flags &= ~FLAG_CLOSED;
        INPe.AddClickBox(&dialogBox, 0);
        yw->field_17bc = 0;
    }
}

void GuiBase::CloseDialog(_NC_STACK_ypaworld *yw)
{
    if ( !(flags & FLAG_CLOSED) )
    {
        flags |= FLAG_CLOSED;
        INPe.RemClickBox(&dialogBox);
        yw->field_17bc = 0;
    }
}

void GuiList::Formate(_NC_STACK_ypaworld *yw)
{
    FormateTitle(yw);
    FormateScrollbar(yw);
    if ( !(listFlags & GLIST_FLAG_STATIC) )
        FormateItemBlock(yw);
}

void GuiList::PosOnSelected(int a2)
{
    int v2 = maxShownEntries + firstShownEntries;

    if ( a2 >= firstShownEntries && a2 <= (v2 - 1) )
    {
        if ( v2 > numEntries )
            firstShownEntries = numEntries - maxShownEntries;
        if ( firstShownEntries < 0 )
            firstShownEntries = 0;
    }
    else if ( numEntries - a2 <= maxShownEntries )
    {
        firstShownEntries = numEntries - maxShownEntries;
    }
    else
    {
        firstShownEntries = a2;
    }
}


void GuiList::Free()
{
    if ( cmdstrm.includ )
    {
        nc_FreeMem(cmdstrm.includ);
        cmdstrm.includ = NULL;
    }

    if ( itemBlock )
    {
        nc_FreeMem(itemBlock);
        itemBlock = NULL;
    }

    if ( scrollbar )
    {
        nc_FreeMem(scrollbar);
        scrollbar = NULL;
    }

    if ( cmdstrm.cmdbuf )
    {
        nc_FreeMem(cmdstrm.cmdbuf);
        cmdstrm.cmdbuf = NULL;
    }

    if ( iconString )
    {
        nc_FreeMem(iconString);
        iconString = NULL;
    }

    if ( dialogBox.buttons[0] )
    {
        nc_FreeMem(dialogBox.buttons[0]);
        dialogBox.buttons[0] = NULL;
    }
}
