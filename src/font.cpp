#include <string.h>
#include "includes.h"
#include "font.h"



void FontUA::store_u32(char **pos, uint32_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = (val >> 24) & 0xFF;
    ppos[1] = (val >> 16) & 0xFF;
    ppos[2] = (val >>  8) & 0xFF;
    ppos[3] = (val      ) & 0xFF;

    *pos += 4;
}


void FontUA::store_s32(char **pos, int32_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = (val >> 24) & 0xFF;
    ppos[1] = (val >> 16) & 0xFF;
    ppos[2] = (val >>  8) & 0xFF;
    ppos[3] = (val      ) & 0xFF;

    *pos += 4;
}

void FontUA::store_u16(char **pos, uint16_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = (val >> 8) & 0xFF;
    ppos[1] = (val     ) & 0xFF;

    *pos += 2;
}

void FontUA::store_s16(char **pos, int16_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = (val >> 8) & 0xFF;
    ppos[1] = (val     ) & 0xFF;

    *pos += 2;
}

void FontUA::store_u8(char **pos, uint8_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = val;

    *pos += 1;
}

void FontUA::store_s8(char **pos, int8_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = val;

    *pos += 1;
}

void FontUA::store_string(char **pos, const char *string)
{
    char *ppos = *pos;
    strcpy(ppos, string);

    *pos += strlen(string) + 1;
}

uint32_t FontUA::get_u32(char **pos)
{
    uint32_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] << 24) & 0xFF000000;
    tmp |= (ppos[1] << 16) & 0xFF0000;
    tmp |= (ppos[2] <<  8) & 0xFF00;
    tmp |= (ppos[3]      ) & 0xFF;

    *pos += 4;

    return tmp;
}

int32_t FontUA::get_s32(char **pos)
{
    int32_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] << 24) & 0xFF000000;
    tmp |= (ppos[1] << 16) & 0xFF0000;
    tmp |= (ppos[2] <<  8) & 0xFF00;
    tmp |= (ppos[3]      ) & 0xFF;

    *pos += 4;

    return tmp;
}

uint16_t FontUA::get_u16(char **pos)
{
    uint16_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] <<  8) & 0xFF00;
    tmp |= (ppos[1]      ) & 0xFF;

    *pos += 2;

    return tmp;
}

int16_t FontUA::get_s16(char **pos)
{
    int16_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] <<  8) & 0xFF00;
    tmp |= (ppos[1]      ) & 0xFF;

    *pos += 2;

    return tmp;
}

uint8_t FontUA::get_u8(char **pos)
{
    uint8_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= ppos[0] & 0xFF;

    *pos += 1;

    return tmp;
}

int8_t FontUA::get_s8(char **pos)
{
    int8_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= ppos[0] & 0xFF;

    *pos += 1;

    return tmp;
}

void FontUA::get_string(char **pos, char *buf)
{
    char *ppos = *pos;
    strcpy(buf, ppos);

    *pos += strlen(buf) + 1;
}

void FontUA::set_end(char **pos)
{
    FontUA::store_u16(pos, 0);
}

void FontUA::set_center_xpos(char **pos, int16_t xpos)
{
    FontUA::store_u16(pos, 1);
    FontUA::store_s16(pos, xpos);
}

void FontUA::set_center_ypos(char **pos, int16_t ypos)
{
    FontUA::store_u16(pos, 2);
    FontUA::store_s16(pos, ypos);
}

void FontUA::set_xpos(char **pos, int16_t xpos)
{
    FontUA::store_u16(pos, 3);
    FontUA::store_s16(pos, xpos);
}

void FontUA::set_ypos(char **pos, int16_t ypos)
{
    FontUA::store_u16(pos, 4);
    FontUA::store_s16(pos, ypos);
}

void FontUA::add_xpos(char **pos, int16_t xpos)
{
    FontUA::store_u16(pos, 5);
    FontUA::store_s16(pos, xpos);
}

void FontUA::add_ypos(char **pos, int16_t ypos)
{
    FontUA::store_u16(pos, 6);
    FontUA::store_s16(pos, ypos);
}

void FontUA::next_line(char **pos)
{
    FontUA::store_u16(pos, 7);
}

void FontUA::select_tileset(char **pos, uint8_t tileset)
{
    FontUA::store_u16(pos, 8);
    FontUA::store_u8(pos, tileset);
}

void FontUA::include(char **pos, uint8_t cmdstream_id)
{
    FontUA::store_u16(pos, 9);
    FontUA::store_u8(pos, cmdstream_id);
}

void FontUA::op10(char **pos, uint8_t arg)
{
    FontUA::store_u16(pos, 10);
    FontUA::store_u8(pos, arg);
}

void FontUA::op11(char **pos, uint8_t arg)
{
    FontUA::store_u16(pos, 11);
    FontUA::store_u8(pos, arg);
}

void FontUA::set_xoff(char **pos, uint8_t xoff)
{
    FontUA::store_u16(pos, 12);
    FontUA::store_u8(pos, xoff);
}

void FontUA::set_xwidth(char **pos, uint8_t xwidth)
{
    FontUA::store_u16(pos, 13);
    FontUA::store_u8(pos, xwidth);
}

void FontUA::set_yoff(char **pos, uint8_t yoff)
{
    FontUA::store_u16(pos, 14);
    FontUA::store_u8(pos, yoff);
}

void FontUA::set_yheight(char **pos, uint8_t yheight)
{
    FontUA::store_u16(pos, 15);
    FontUA::store_u8(pos, yheight);
}

void FontUA::reset_tileset(char **pos, uint8_t tileset)
{
    FontUA::store_u16(pos, 16);
    FontUA::store_u8(pos, tileset);
}

void FontUA::op17(char **pos, int16_t arg)
{
    FontUA::store_u16(pos, 17);
    FontUA::store_s16(pos, arg);
}

void FontUA::add_txt(char **pos, int16_t block_width, uint16_t flag, const std::string &string)
{
    FontUA::store_u16(pos, 18);
    FontUA::store_s16(pos, block_width);
    FontUA::store_u16(pos, flag);
    FontUA::store_string(pos, string.c_str());
}

void FontUA::copy_position(char **pos)
{
    FontUA::store_u16(pos, 19);
}

void FontUA::set_flag(char **pos, uint16_t flag)
{
    FontUA::store_u16(pos, 20);
    FontUA::store_u16(pos, flag);
}

void FontUA::unset_flag(char **pos, uint16_t flag)
{
    FontUA::store_u16(pos, 21);
    FontUA::store_u16(pos, flag);
}

void FontUA::set_txtColor(char **pos, uint8_t r, uint8_t g, uint8_t b)
{
    FontUA::store_u16(pos, 22);
    FontUA::store_u16(pos, r);
    FontUA::store_u16(pos, g);
    FontUA::store_u16(pos, b);
}







// Funcs

char * FontUA::FormateClippedText(TileMap *, char *curpos, const std::string &str, int width_space, char space_chr)
{
    char *tmp = curpos;

    if ( width_space > 0 )
    {
        FontUA::copy_position(&tmp);

        int v6 = width_space;

        while (v6 > 0)
        {
            if ( v6 <= 255 )
            {
                FontUA::op10(&tmp, v6);
            }
            else
            {
                FontUA::op10(&tmp, 255);
            }

            FontUA::store_s8(&tmp, space_chr);

            v6 -= 255;
        }

        FontUA::add_txt(&tmp, width_space, 1, str);
    }
    return tmp;
}

char * FontUA::FormateCenteredSkipableItem(TileMap *, char *in, const std::string &str, int width)
{
    char *cur = in;
    if ( width > 0 )
    {
        FontUA::copy_position(&cur);
        FontUA::add_txt(&cur, width, 4, str);
    }
    return cur;
}

char * FontUA::TextRelWidthItem(TileMap *, char *pos, const std::string &txt, int wdth, int arg)
{
    char *tmp = pos;

    FontUA::copy_position(&tmp);

    int flag;

    if ( arg == 4)
        flag = 9;
    else if ( arg == 8 )
        flag = 10;
    else
        flag = 12;

    FontUA::add_txt(&tmp, wdth, flag, txt);

    return tmp;
}



char * FontUA::FormateTextAlignedClippedString(NC_STACK_ypaworld *yw, char *cmdbuf, ColumnItem *arg)
{
    TileMap *v3 = yw->tiles[ arg->fontID ];

    int v4 = arg->width;

    if ( arg->flags & 1 )
        v4 -= v3->map[arg->prefixChar].w;

    if ( arg->flags & 2 )
        v4 -= v3->map[arg->postfixChar].w;

    char *tmp = cmdbuf;
    FontUA::select_tileset(&tmp, arg->fontID);


    if ( arg->flags & 1 )
        FontUA::store_s8(&tmp, arg->prefixChar);

    if ( v4 > 0 )
    {
        FontUA::copy_position(&tmp);

        int v8 = v4;

        while (v8 > 0)
        {
            if ( v8 <= 255 )
            {
                FontUA::op10(&tmp, v8);
            }
            else
            {
                FontUA::op10(&tmp, 255);
            }

            FontUA::store_u8(&tmp, arg->spaceChar);
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

        FontUA::add_txt(&tmp, v4, v20, arg->txt);
    }

    if ( arg->flags & 2 )
    {
        FontUA::store_s8(&tmp, arg->postfixChar);
    }
    return tmp;
}

char *FontUA::FormateAlignedClippedString(NC_STACK_ypaworld *yw, char *cmdbuf, ColumnItem *arg)
{
    TileMap *tset = yw->tiles[ arg->fontID ];

    int v28 = 0;
    int v29 = 0;
    int v27 = 0;
    int v26 = 0;

    int txtWidth = 0;
    
    txtWidth += tset->GetWidth(arg->txt);

    int freeWidth = arg->width - txtWidth;
    if ( arg->flags & 1 )
        freeWidth -= tset->GetWidth(arg->prefixChar);
    if ( arg->flags & 2 )
        freeWidth -= tset->GetWidth(arg->postfixChar);

    char *tmp = cmdbuf;
    FontUA::select_tileset(&tmp, arg->fontID);

    if ( freeWidth <= 0 )
    {
        if ( freeWidth < 0 )
        {
            if ( arg->flags & 4 )
            {
                v29 = -freeWidth;
            }
            else if ( arg->flags & 8 )
            {
                v28 = -freeWidth;
            }
            else if ( arg->flags & 0x10 )
            {
                v28 = (-freeWidth) / 2;
                v29 = (-freeWidth) - ((-freeWidth) / 2);
            }
        }
    }
    else
    {
        if ( arg->flags & 4 )
        {
            v26 = freeWidth;
        }
        else if ( arg->flags & 8 )
        {
            v27 = freeWidth;
        }
        else if ( arg->flags & 0x10 )
        {
            v27 = freeWidth / 2;
            v26 = freeWidth - (freeWidth / 2);
        }
    }

    int v16 = txtWidth - v29 - v28;

    if ( arg->flags & 1 )
        FontUA::store_u8(&tmp, arg->prefixChar);

    int chr = 0;

    size_t cI = 0;

    while (v28 > 0)
    {
        chr = arg->txt[cI];
        cI++;
        v28 -= tset->GetWidth(chr);
    }

    if ( v28 < 0 )
    {
        FontUA::set_xoff(&tmp, v28 + tset->GetWidth(chr));
        FontUA::store_u8(&tmp, chr);
    }

    if ( v27 > 0 )
    {
        while ( v27 > 255 )
        {
            FontUA::op10(&tmp, 255);
            FontUA::store_u8(&tmp, arg->spaceChar);
            v27 -= 255;
        }

        if ( v27 > 0 )
        {
            FontUA::op10(&tmp, v27);
            FontUA::store_u8(&tmp, arg->spaceChar);
        }
    }

    if (v16 > 0)
    {
        chr = 0;
        while ( v16 >= 0 )
        {
            if (cI >= arg->txt.size())
            {
                chr = 0;
                break;
            }
            
            chr = arg->txt[cI];
            cI++;

            v16 -= tset->GetWidth(chr);
            FontUA::store_u8(&tmp, chr);
        }
        if (chr)
        {
            FontUA::set_xwidth(&tmp, v16 + tset->GetWidth(chr));
            FontUA::store_u8(&tmp, chr);
        }
    }

    if ( v26 > 0 )
    {
        while ( v26 > 255 )
        {
            FontUA::op10(&tmp, 255);
            FontUA::store_u8(&tmp, arg->spaceChar);
            v26 -= 255;
        }
        if ( v26 > 0 )
        {
            FontUA::op10(&tmp, v26);
            FontUA::store_u8(&tmp, arg->spaceChar);
        }
    }
    if ( arg->flags & 2 )
        FontUA::store_u8(&tmp, arg->postfixChar);

    return tmp;
}


char * FontUA::FormateColumnItem(NC_STACK_ypaworld *yw, char *cmdbuf, int num, ColumnItem *a4)
{
    char *tmp = cmdbuf;
    for (int i = 0; i < num; i++)
        if (a4[i].flags & 0x20)
            tmp = FormateTextAlignedClippedString(yw, tmp, &a4[i]);
        else
            tmp = FormateAlignedClippedString(yw, tmp, &a4[i]);

    return tmp;
}


