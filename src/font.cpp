#include <string.h>
#include "font.h"



void fntcmd_store_u32(char **pos, uint32_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = (val >> 24) & 0xFF;
    ppos[1] = (val >> 16) & 0xFF;
    ppos[2] = (val >>  8) & 0xFF;
    ppos[3] = (val      ) & 0xFF;

    *pos += 4;
}


void fntcmd_store_s32(char **pos, int32_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = (val >> 24) & 0xFF;
    ppos[1] = (val >> 16) & 0xFF;
    ppos[2] = (val >>  8) & 0xFF;
    ppos[3] = (val      ) & 0xFF;

    *pos += 4;
}

void fntcmd_store_u16(char **pos, uint16_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = (val >> 8) & 0xFF;
    ppos[1] = (val     ) & 0xFF;

    *pos += 2;
}

void fntcmd_store_s16(char **pos, int16_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = (val >> 8) & 0xFF;
    ppos[1] = (val     ) & 0xFF;

    *pos += 2;
}

void fntcmd_store_u8(char **pos, uint8_t val)
{
    uint8_t *ppos = *(uint8_t **)pos;
    ppos[0] = val;

    *pos += 1;
}

void fntcmd_store_s8(char **pos, int8_t val)
{
    int8_t *ppos = *(int8_t **)pos;
    ppos[0] = val;

    *pos += 1;
}

void fntcmd_store_string(char **pos, const char *string)
{
    char *ppos = *pos;
    strcpy(ppos, string);

    *pos += strlen(string) + 1;
}

uint32_t fntcmd_get_u32(char **pos)
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

int32_t fntcmd_get_s32(char **pos)
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

uint16_t fntcmd_get_u16(char **pos)
{
    uint16_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] <<  8) & 0xFF00;
    tmp |= (ppos[1]      ) & 0xFF;

    *pos += 2;

    return tmp;
}

int16_t fntcmd_get_s16(char **pos)
{
    int16_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= (ppos[0] <<  8) & 0xFF00;
    tmp |= (ppos[1]      ) & 0xFF;

    *pos += 2;

    return tmp;
}

uint8_t fntcmd_get_u8(char **pos)
{
    uint8_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= ppos[0] & 0xFF;

    *pos += 1;

    return tmp;
}

int8_t fntcmd_get_s8(char **pos)
{
    int8_t tmp = 0;

    uint8_t *ppos = *(uint8_t **)pos;

    tmp |= ppos[0] & 0xFF;

    *pos += 1;

    return tmp;
}

void fntcmd_get_string(char **pos, char *buf)
{
    char *ppos = *pos;
    strcpy(buf, ppos);

    *pos += strlen(buf) + 1;
}

void fntcmd_set_end(char **pos)
{
    fntcmd_store_u16(pos, 0);
}

void fntcmd_set_center_xpos(char **pos, int16_t xpos)
{
    fntcmd_store_u16(pos, 1);
    fntcmd_store_s16(pos, xpos);
}

void fntcmd_set_center_ypos(char **pos, int16_t ypos)
{
    fntcmd_store_u16(pos, 2);
    fntcmd_store_s16(pos, ypos);
}

void fntcmd_set_xpos(char **pos, int16_t xpos)
{
    fntcmd_store_u16(pos, 3);
    fntcmd_store_s16(pos, xpos);
}

void fntcmd_set_ypos(char **pos, int16_t ypos)
{
    fntcmd_store_u16(pos, 4);
    fntcmd_store_s16(pos, ypos);
}

void fntcmd_add_xpos(char **pos, int16_t xpos)
{
    fntcmd_store_u16(pos, 5);
    fntcmd_store_s16(pos, xpos);
}

void fntcmd_add_ypos(char **pos, int16_t ypos)
{
    fntcmd_store_u16(pos, 6);
    fntcmd_store_s16(pos, ypos);
}

void fntcmd_next_line(char **pos)
{
    fntcmd_store_u16(pos, 7);
}

void fntcmd_select_tileset(char **pos, uint8_t tileset)
{
    fntcmd_store_u16(pos, 8);
    fntcmd_store_u8(pos, tileset);
}

void fntcmd_include(char **pos, uint8_t cmdstream_id)
{
    fntcmd_store_u16(pos, 9);
    fntcmd_store_u8(pos, cmdstream_id);
}

void fntcmd_op10(char **pos, uint8_t arg)
{
    fntcmd_store_u16(pos, 10);
    fntcmd_store_u8(pos, arg);
}

void fntcmd_op11(char **pos, uint8_t arg)
{
    fntcmd_store_u16(pos, 11);
    fntcmd_store_u8(pos, arg);
}

void fntcmd_set_xoff(char **pos, uint8_t xoff)
{
    fntcmd_store_u16(pos, 12);
    fntcmd_store_u8(pos, xoff);
}

void fntcmd_set_xwidth(char **pos, uint8_t xwidth)
{
    fntcmd_store_u16(pos, 13);
    fntcmd_store_u8(pos, xwidth);
}

void fntcmd_set_yoff(char **pos, uint8_t yoff)
{
    fntcmd_store_u16(pos, 14);
    fntcmd_store_u8(pos, yoff);
}

void fntcmd_set_yheight(char **pos, uint8_t yheight)
{
    fntcmd_store_u16(pos, 15);
    fntcmd_store_u8(pos, yheight);
}

void fntcmd_reset_tileset(char **pos, uint8_t tileset)
{
    fntcmd_store_u16(pos, 16);
    fntcmd_store_u8(pos, tileset);
}

void fntcmd_op17(char **pos, int16_t arg)
{
    fntcmd_store_u16(pos, 17);
    fntcmd_store_s16(pos, arg);
}

void fntcmd_add_txt(char **pos, int16_t block_width, uint16_t flag, const char *string)
{
    fntcmd_store_u16(pos, 18);
    fntcmd_store_s16(pos, block_width);
    fntcmd_store_u16(pos, flag);
    fntcmd_store_string(pos, string);
}

void fntcmd_copy_position(char **pos)
{
    fntcmd_store_u16(pos, 19);
}

void fntcmd_set_flag(char **pos, uint16_t flag)
{
    fntcmd_store_u16(pos, 20);
    fntcmd_store_u16(pos, flag);
}

void fntcmd_unset_flag(char **pos, uint16_t flag)
{
    fntcmd_store_u16(pos, 21);
    fntcmd_store_u16(pos, flag);
}

void fntcmd_set_txtColor(char **pos, uint8_t r, uint8_t g, uint8_t b)
{
    fntcmd_store_u16(pos, 22);
    fntcmd_store_u16(pos, r);
    fntcmd_store_u16(pos, g);
    fntcmd_store_u16(pos, b);
}


