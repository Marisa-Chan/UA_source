#include <inttypes.h>
#include <string.h>
#include "includes.h"
#include "utils.h"

#include "lstvw.h"
#include "font.h"



char *GuiBase::FormateTitle(NC_STACK_ypaworld *yw, int xpos, int ypos, int w, const std::string &title, char *in, uint8_t postf_char, int flag)
{
    int v27 = 0;
    if ( flag & FLAG_WITH_CLOSE )
        v27 = yw->tiles[24]->map[65].w; // Help button

    int v26 = 0;
    if ( flag & FLAG_WITH_HELP )
        v26 = yw->tiles[24]->map[66].w; // Close button

    int v29 = 0;
    if ( postf_char )
        v29 = yw->tiles[0]->map[postf_char].w;

    std::string buf = " ";
    buf += title;

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


int GuiList::initDialogStrings(NC_STACK_ypaworld *yw)
{
    char *draw_cmd = (char *)AllocVec(512, 1);

    if ( !draw_cmd )
        return 0;

    int xpos = x - (yw->screen_width / 2);
    int ypos = y - (yw->screen_height / 2);

    cmdstrm.cmdbuf = draw_cmd;

    char *v7 = draw_cmd;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
    {
        v7 = GuiBase::FormateTitle(yw, xpos, ypos, w, title, draw_cmd, 0, flags);

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

int GuiList::InitBuffers(NC_STACK_ypaworld *yw)
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

void GuiList::SetRect(NC_STACK_ypaworld *yw, int xpos, int ypos)
{
    if ( entryWidth < minEntryWidth)
        entryWidth = minEntryWidth;

    if ( entryWidth > maxEntryWidth)
        entryWidth = maxEntryWidth;

    if ( shownEntries < minShownEntries)
        shownEntries = minShownEntries;

    if ( shownEntries > maxShownEntries)
        shownEntries = maxShownEntries;

    int ww = entryWidth;

    if ( (listFlags & GLIST_FLAG_RESIZEABLE) || numEntries > maxShownEntries )
        ww += yw->font_yscrl_bkg_w;

    int hh = upperVborder + lowerVborder + entryHeight * shownEntries;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        hh += yw->font_default_h;

    if ( xpos == -1 )
    {
        xpos = (yw->screen_width / 2) - (ww / 2);
    }
    else if ( xpos == -2 )
    {
        xpos = x;
    }

    if ( ypos == -1 )
    {
        ypos = (yw->screen_height / 2) - (hh / 2);
    }
    else if ( ypos == -2 )
    {
        ypos = y;
    }

    if ( xpos < 0 )
        xpos = 0;

    if ( ypos < yw->icon_energy__h )
        ypos = yw->icon_energy__h;

    if ( ww + xpos >= yw->screen_width )
        xpos = yw->screen_width - ww;

    if ( hh + ypos >= yw->screen_height - yw->icon0___h )
        ypos = yw->screen_height - yw->icon0___h - hh;

    x = xpos;
    y = ypos;
    w = ww;
    h = hh;
}

int GuiList::initButtons()
{
    buttons.resize(shownEntries + 8);

    if ( listFlags & GLIST_FLAG_WITH_ICON )
    {
        iconBox.buttons.resize(1);
        iconBox.buttons[0] = ButtonBox(0, 0, 16, 16);
    }
    else
    {
        iconBox.buttons.resize(0);
    }

    return 1;
}

void GuiList::FormateTitle(NC_STACK_ypaworld *yw)
{
    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
    {
        char *v5 = cmdstrm.cmdbuf;

        int v6 = x - (yw->screen_width / 2);
        int v9 = y - (yw->screen_height / 2);

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
            v5 = GuiBase::FormateTitle(yw, v6, v9, w, title, v5, 0, flags);
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

        int v14 = w - v17 - v16;

        buttons[1] = ButtonBox(0, 0, v14, yw->font_default_h);

        if ( flags & FLAG_WITH_HELP )
            buttons[7] = ButtonBox(v14, 0, v17, yw->font_default_h);
        else
            buttons[7] = ButtonBox();

        if ( flags & FLAG_WITH_CLOSE )
            buttons[0] = ButtonBox(w - v17, 0, v17, yw->font_default_h);
        else
            buttons[0] = ButtonBox();
    }
}

void GuiList::ScrollParamsFromEntries(NC_STACK_ypaworld *yw)
{
    int v3 = h;

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

void GuiList::FormateScrollbar(NC_STACK_ypaworld *yw)
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

        int v34 = w - yw->font_yscrl_bkg_w;
        int v35 = 0;

        if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
            v35 = yw->font_default_h;

        buttons[2] = ButtonBox(v34, v35, yw->font_yscrl_bkg_w, btnStart); // Scroll up part
        buttons[3] = ButtonBox(v34, btnStart + v35, yw->font_yscrl_bkg_w, btnSize); // Scroll position
        buttons[4] = ButtonBox(v34, btnSize + btnStart + v35,  yw->font_yscrl_bkg_w, scrlSize - (btnSize + btnStart));
        buttons[6] = ButtonBox(0, h - lowerVborder, w - yw->font_yscrl_bkg_w, lowerVborder);

        if ( listFlags & GLIST_FLAG_RESIZEABLE )
            buttons[5] = ButtonBox(v34, scrlSize + v35, yw->font_yscrl_bkg_w, yw->font_xscrl_h); // Resize button
        else
            buttons[5] = ButtonBox();

        int v7 = x + v34 - (yw->screen_width / 2);
        int v11 = y + v35 - (yw->screen_height / 2);

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
        buttons[2] = ButtonBox();
        buttons[3] = ButtonBox();
        buttons[4] = ButtonBox();
        buttons[5] = ButtonBox();
        buttons[6] = ButtonBox();
    }
    FontUA::set_end(&v5);
}

void GuiList::FormateItemBlock(NC_STACK_ypaworld *yw)
{
    buttons.resize(shownEntries + 8);

    int v3 = upperVborder;

    if ( listFlags & GLIST_FLAG_WITH_TITLEBAR )
        v3 += yw->font_default_h;

    for (int i = 0; i < shownEntries; i++)
        buttons[8 + i] = ButtonBox( 0, v3 + i * entryHeight, entryWidth, entryHeight );
}


int GuiList::Init(NC_STACK_ypaworld *yw, tInit &in)
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
        iconBox.x = 16 * in.iconPos;
        iconBox.y = yw->screen_height - 32;
        iconBox.w = 16;
        iconBox.h = 16;
        iconBox.buttons.resize(1);
    }
    else
    {
        iconBox.x = 0;
        iconBox.y = 0;
        iconBox.w = 0;
        iconBox.h = 0;
        iconBox.buttons.resize(0);
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


char *GuiList::ItemsPreLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5)
{
    int v14 = x - (yw->screen_width / 2);
    int v12 = y - (yw->screen_height / 2);

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

char *GuiList::ItemsPostLayout(NC_STACK_ypaworld *yw, char *cmdbuf, int tileset, const char *a5)
{
    char *tmp = cmdbuf;
    FontUA::reset_tileset(&tmp, tileset);
    FontUA::set_yoff(&tmp, yw->tiles[tileset]->h - lowerVborder);
    FontUA::store_s8(&tmp, a5[0]);
    FontUA::op17(&tmp, entryWidth - yw->font_default_w__b);
    FontUA::store_s8(&tmp, a5[1]);
    FontUA::store_s8(&tmp, a5[2]);
    return tmp;
}


void GuiList::InputHandle(NC_STACK_ypaworld *yw, InputState *struc)
{
    if ( flags & (FLAG_CLOSED | FLAG_ICONIFED) )
    {
        listFlags &= ~(GLIST_FLAG_IN_RESIZING | GLIST_FLAG_IN_SCROLLING | GLIST_FLAG_IN_SELECT);
        return;
    }

    ClickBoxInf *v6 = &struc->ClickInf;

    int v3 = w;
    int v41 = h;

    listFlags &= ~GLIST_FLAG_SEL_DONE;

    int xpos = x;

    int ypos = y;

    mouseItem = -1;



    if ( listFlags & GLIST_FLAG_KEYB_INPUT )
    {
        if ( struc->KbdLastHit == Input::KC_UP )
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
        else if ( struc->KbdLastHit == Input::KC_DOWN )
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

    yw->draggingLock = (listFlags & (GLIST_FLAG_IN_RESIZING | GLIST_FLAG_IN_SCROLLING | GLIST_FLAG_IN_SELECT)) && (v6->flag & ClickBoxInf::FLAG_LM_HOLD);

    //printf("%x %x %d, \n", (int)&dialogBox , (int)struc->winp131arg.selected_btn , struc->winp131arg.selected_btnID);

    if ( this == struc->ClickInf.selected_btn && struc->ClickInf.selected_btnID >= 8 )
        listFlags &= ~GLIST_FLAG_NO_SCROLL;

    if ( listFlags & GLIST_FLAG_IN_RESIZING )
    {
        if ( v6->flag & ClickBoxInf::FLAG_LM_HOLD )
        {
            int xps = v6->move.ScreenPos.x + rszX;
            int yps = v6->move.ScreenPos.y + rszY;

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
        if ( v6->flag & ClickBoxInf::FLAG_LM_HOLD )
        {
            btnStart += struc->ClickInf.move.ScreenPos.y - scrlY;
            scrlY = struc->ClickInf.move.ScreenPos.y;
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
        if ( (this == struc->ClickInf.selected_btn) && (struc->ClickInf.selected_btnID >= 8) )
        {
            if ( v6->flag & (ClickBoxInf::FLAG_LM_HOLD | ClickBoxInf::FLAG_LM_UP) )
            {
                selectedEntry = firstShownEntries + struc->ClickInf.selected_btnID - 8;
                mouseItem = selectedEntry;

                if ( v6->flag & ClickBoxInf::FLAG_LM_UP )
                {
                    listFlags &= ~(GLIST_FLAG_SEL_DONE | GLIST_FLAG_IN_SELECT);
                    listFlags |= GLIST_FLAG_SEL_DONE;

                    if ( listFlags & GLIST_FLAG_INSTANT_INPUT )
                    {
                        flags |= FLAG_CLOSED;
                        INPe.RemClickBox(struc->ClickInf.selected_btn);
                    }
                }
            }
            else
            {
                listFlags &= ~GLIST_FLAG_IN_SELECT;

            }
        }
        else if ( !(v6->flag & ClickBoxInf::FLAG_LM_HOLD) )
        {
            listFlags &= ~GLIST_FLAG_IN_SELECT;
        }
        else if ( !(listFlags & GLIST_FLAG_NO_SCROLL) )
        {
            ButtonBox &v40 = buttons[8];
            ButtonBox &v27 = buttons[shownEntries + 7];

            if ( scrollTimer > 0 )
            {
                scrollTimer -= struc->Period;
            }
            else
            {
                scrollTimer = 70;

                if ( struc->ClickInf.move.ScreenPos.y >= ypos + v40.y )
                {
                    if ( struc->ClickInf.move.ScreenPos.y > ypos + v27.y + v27.h )
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

            if ( struc->ClickInf.move.ScreenPos.y >= ypos + v40.y )
            {
                if ( struc->ClickInf.move.ScreenPos.y > ypos + v27.y + v27.h )
                    selectedEntry = shownEntries + firstShownEntries - 1;
            }
            else
            {
                selectedEntry = firstShownEntries;
            }
        }
    }
    else if ( this == struc->ClickInf.selected_btn )
    {
        if ( struc->ClickInf.selected_btnID >= 8 )
            mouseItem = struc->ClickInf.selected_btnID + firstShownEntries - 8;

        if ( struc->ClickInf.selected_btnID == 7 )
        {
            if ( v6->flag & (ClickBoxInf::FLAG_BTN_DOWN | ClickBoxInf::FLAG_BTN_HOLD) )
                flags |= FLAG_HELP_DOWN;
            if ( v6->flag & ClickBoxInf::FLAG_BTN_UP )
                flags &= ~FLAG_HELP_DOWN;
        }

        if ( v6->flag & ClickBoxInf::FLAG_BTN_HOLD )
        {
            if ( struc->ClickInf.selected_btnID == 2 )
            {
                if ( scrollTimer > 0 )
                {
                    scrollTimer -= struc->Period;
                }
                else
                {
                    scrollTimer = 70;

                    firstShownEntries--;
                    if ( firstShownEntries < 0 )
                        firstShownEntries = 0;
                }
            }
            else if ( struc->ClickInf.selected_btnID == 4 )
            {
                if ( scrollTimer > 0 )
                {
                    scrollTimer -= struc->Period;
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

        if ( v6->flag & ClickBoxInf::FLAG_BTN_DOWN )
        {
            if ( v6->selected_btnID == 3 )
            {
                listFlags |= GLIST_FLAG_IN_SCROLLING;
                scrlY = v6->ldw_pos.ScreenPos.y;
            }
            else if ( v6->selected_btnID == 5 )
            {
                listFlags |= GLIST_FLAG_IN_RESIZING;
                rszX = v3 - v6->ldw_pos.ScreenPos.x;
                rszY = v41 - v6->ldw_pos.ScreenPos.y;
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
        if ( v6->flag & ClickBoxInf::FLAG_LM_DOWN )
        {
            flags |= FLAG_CLOSED;
            INPe.RemClickBox(this);
        }
    }
}

void GuiBase::Detach()
{
    if (AttachedTo)
    {
        AttachedTo->remove(this);
        AttachedTo = NULL;
    }
}

void GuiBase::Attach(GuiBaseList &_lst)
{
    Detach();

    _lst.push_back(this);
    AttachedTo = &_lst;
}

void GuiList::Formate(NC_STACK_ypaworld *yw)
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

    buttons.clear();
}
