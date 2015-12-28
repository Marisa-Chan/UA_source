#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include <inttypes.h>

void fntcmd_store_u32(char **pos, uint32_t val);
void fntcmd_store_s32(char **pos, int32_t val);
void fntcmd_store_u16(char **pos, uint16_t val);
void fntcmd_store_s16(char **pos, int16_t val);
void fntcmd_store_u8(char **pos, uint8_t val);
void fntcmd_store_s8(char **pos, int8_t val);
void fntcmd_store_string(char **pos, const char *string);

uint32_t fntcmd_get_u32(char **pos);
int32_t fntcmd_get_s32(char **pos);
uint16_t fntcmd_get_u16(char **pos);
int16_t fntcmd_get_s16(char **pos);
uint8_t fntcmd_get_u8(char **pos);
int8_t fntcmd_get_s8(char **pos);
void fntcmd_get_string(char **pos, char *buf);

//Text output opcode-generators
//Op 0:
void fntcmd_set_end(char **pos);
//Op 1: Set x position from center
void fntcmd_set_center_xpos(char **pos, int16_t xpos);
//Op 2: Set y position from center
void fntcmd_set_center_ypos(char **pos, int16_t ypos);
//Op 3: Set x position for output
void fntcmd_set_xpos(char **pos, int16_t xpos);
//Op 4: Set y position for output
void fntcmd_set_ypos(char **pos, int16_t ypos);
//Op 5: Add x position
void fntcmd_add_xpos(char **pos, int16_t xpos);
//Op 6: Add x position
void fntcmd_add_ypos(char **pos, int16_t ypos);
//Op 7: Next line
void fntcmd_next_line(char **pos);
//Op 8: Select tileset
void fntcmd_select_tileset(char **pos, uint8_t tileset);
//Op 9:
void fntcmd_include(char **pos, uint8_t cmdstream_id);
//Op 10:
void fntcmd_op10(char **pos, uint8_t arg);
//Op 11:
void fntcmd_op11(char **pos, uint8_t arg);
//Op 12: set x off
void fntcmd_set_xoff(char **pos, uint8_t xoff);
//Op 13: set x width
void fntcmd_set_xwidth(char **pos, uint8_t xwidth);
//Op 14: set y off
void fntcmd_set_yoff(char **pos, uint8_t yoff);
//Op 15: set y height
void fntcmd_set_yheight(char **pos, uint8_t yheight);
//Op 16:
void fntcmd_reset_tileset(char **pos, uint8_t tileset);
//Op 17:
void fntcmd_op17(char **pos, int16_t arg);
//Op 18: Add text for output
void fntcmd_add_txt(char **pos, int16_t block_width, uint16_t flag, const char *string);
//Op 19: Copy current output xy position for text output
void fntcmd_copy_position(char **pos);
//Op 20: Set flag bitfield for text output
void fntcmd_set_flag(char **pos, uint16_t flag);
//Op 21: Unset flag bitfield for text output
void fntcmd_unset_flag(char **pos, uint16_t flag);
//Op 22: Set text color
void fntcmd_set_txtColor(char **pos, uint8_t r, uint8_t g, uint8_t b);


// funcs related
char * txtcmd_txt_w_bkg(tiles_stru *, char *curpos, const char *str, int width_space, char chr);


#endif //FONT_H_INCLUDED
