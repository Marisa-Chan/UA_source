#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <inttypes.h>

#include "system/gfx.h"

class NC_STACK_ypaworld;

namespace FontUA
{
void store_u32(CmdStream *pos, uint32_t val);
void store_s32(CmdStream *pos, int32_t val);
void store_u16(CmdStream *pos, uint16_t val);
void store_s16(CmdStream *pos, int16_t val);
void store_u8(CmdStream *pos, uint8_t val);
void store_s8(CmdStream *pos, int8_t val);
void store_string(CmdStream *pos, const std::string &string);

uint16_t get_u16(const CmdStream &stream, int32_t *pos);
int16_t get_s16(const CmdStream &stream, int32_t *pos);
uint8_t get_u8(const CmdStream &stream, int32_t *pos);

//Text output opcode-generators
//Op 0:
void set_end(CmdStream *pos);
//Op 1: Set x position from center
void set_center_xpos(CmdStream *pos, int16_t xpos);
//Op 2: Set y position from center
void set_center_ypos(CmdStream *pos, int16_t ypos);
//Op 3: Set x position for output
void set_xpos(CmdStream *pos, int16_t xpos);
//Op 4: Set y position for output
void set_ypos(CmdStream *pos, int16_t ypos);
//Op 5: Add x position
void add_xpos(CmdStream *pos, int16_t xpos);
//Op 6: Add x position
void add_ypos(CmdStream *pos, int16_t ypos);
//Op 7: Next line
void next_line(CmdStream *pos);
//Op 8: Select tileset
void select_tileset(CmdStream *pos, uint8_t tileset);
//Op 9:
void include(CmdStream *pos, uint8_t cmdstream_id);
//Op 10:
void op10(CmdStream *pos, uint8_t arg);
//Op 11:
void op11(CmdStream *pos, uint8_t arg);
//Op 12: set x off
void set_xoff(CmdStream *pos, uint8_t xoff);
//Op 13: set x width
void set_xwidth(CmdStream *pos, uint8_t xwidth);
//Op 14: set y off
void set_yoff(CmdStream *pos, uint8_t yoff);
//Op 15: set y height
void set_yheight(CmdStream *pos, uint8_t yheight);
//Op 16:
void reset_tileset(CmdStream *pos, uint8_t tileset);
//Op 17:
void op17(CmdStream *pos, int16_t arg);
//Op 18: Add text for output
void add_txt(CmdStream *pos, int16_t block_width, uint16_t flag, const std::string &string);
//Op 19: Copy current output xy position for text output
void copy_position(CmdStream *pos);
//Op 20: Set flag bitfield for text output
void set_flag(CmdStream *pos, uint16_t flag);
//Op 21: Unset flag bitfield for text output
void unset_flag(CmdStream *pos, uint16_t flag);
//Op 22: Set text color
void set_txtColor(CmdStream *pos, uint8_t r, uint8_t g, uint8_t b);





struct ColumnItem
{
    std::string txt;
    int width;
    uint8_t fontID;
    uint8_t spaceChar;
    uint8_t prefixChar;
    uint8_t postfixChar;
    int16_t flags;

    ColumnItem()
    {
        width = 0;
        fontID = 0;
        spaceChar = 0;
        prefixChar = 0;
        postfixChar = 0;
        flags = 0;
    }
};


// funcs related
void FormateClippedText(TileMap *, CmdStream *curpos, const std::string &str, int width_space, char chr);
void FormateCenteredSkipableItem(TileMap *, CmdStream *in, const std::string &str, int width);
void TextRelWidthItem(TileMap *, CmdStream *pos, const std::string &txt, int wdth, int arg);

void FormateColumnItem(NC_STACK_ypaworld *yw, CmdStream *cmdbuf, int num, ColumnItem *a4);
void FormateTextAlignedClippedString(NC_STACK_ypaworld *yw, CmdStream *cmdbuf, ColumnItem *arg);
void FormateAlignedClippedString(NC_STACK_ypaworld *yw, CmdStream *cmdbuf, ColumnItem *arg);
}

#endif //FONT_H_INCLUDED
