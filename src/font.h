#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <inttypes.h>

namespace FontUA
{
void store_u32(char **pos, uint32_t val);
void store_s32(char **pos, int32_t val);
void store_u16(char **pos, uint16_t val);
void store_s16(char **pos, int16_t val);
void store_u8(char **pos, uint8_t val);
void store_s8(char **pos, int8_t val);
void store_string(char **pos, const char *string);

uint32_t get_u32(char **pos);
int32_t get_s32(char **pos);
uint16_t get_u16(char **pos);
int16_t get_s16(char **pos);
uint8_t get_u8(char **pos);
int8_t get_s8(char **pos);
void get_string(char **pos, char *buf);

//Text output opcode-generators
//Op 0:
void set_end(char **pos);
//Op 1: Set x position from center
void set_center_xpos(char **pos, int16_t xpos);
//Op 2: Set y position from center
void set_center_ypos(char **pos, int16_t ypos);
//Op 3: Set x position for output
void set_xpos(char **pos, int16_t xpos);
//Op 4: Set y position for output
void set_ypos(char **pos, int16_t ypos);
//Op 5: Add x position
void add_xpos(char **pos, int16_t xpos);
//Op 6: Add x position
void add_ypos(char **pos, int16_t ypos);
//Op 7: Next line
void next_line(char **pos);
//Op 8: Select tileset
void select_tileset(char **pos, uint8_t tileset);
//Op 9:
void include(char **pos, uint8_t cmdstream_id);
//Op 10:
void op10(char **pos, uint8_t arg);
//Op 11:
void op11(char **pos, uint8_t arg);
//Op 12: set x off
void set_xoff(char **pos, uint8_t xoff);
//Op 13: set x width
void set_xwidth(char **pos, uint8_t xwidth);
//Op 14: set y off
void set_yoff(char **pos, uint8_t yoff);
//Op 15: set y height
void set_yheight(char **pos, uint8_t yheight);
//Op 16:
void reset_tileset(char **pos, uint8_t tileset);
//Op 17:
void op17(char **pos, int16_t arg);
//Op 18: Add text for output
void add_txt(char **pos, int16_t block_width, uint16_t flag, const std::string &string);
//Op 19: Copy current output xy position for text output
void copy_position(char **pos);
//Op 20: Set flag bitfield for text output
void set_flag(char **pos, uint16_t flag);
//Op 21: Unset flag bitfield for text output
void unset_flag(char **pos, uint16_t flag);
//Op 22: Set text color
void set_txtColor(char **pos, uint8_t r, uint8_t g, uint8_t b);





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
char *FormateClippedText(TileMap *, char *curpos, const std::string &str, int width_space, char chr);
char *FormateCenteredSkipableItem(TileMap *, char *in, const std::string &str, int width);
char *TextRelWidthItem(TileMap *, char *pos, const std::string &txt, int wdth, int arg);

char *FormateColumnItem(NC_STACK_ypaworld *yw, char *cmdbuf, int num, ColumnItem *a4);
char *FormateTextAlignedClippedString(NC_STACK_ypaworld *yw, char *cmdbuf, ColumnItem *arg);
char *FormateAlignedClippedString(NC_STACK_ypaworld *yw, char *cmdbuf, ColumnItem *arg);
}

#endif //FONT_H_INCLUDED
