#ifndef LSTVW_H_INCLUDED
#define LSTVW_H_INCLUDED

struct _NC_STACK_ypaworld;


struct listview : public nnode
{
    int cmd_flag;
    inp_node frm_1;
    inp_node frm_2;
    char *field_1BC;
    char *draw_cmd;
    char **field_1C4;
    int field_1C8;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int16_t elements_for_scroll_size;
    int16_t element_count;
    int16_t scroll_pos;
    int16_t field_1DE;
    int16_t element_count_max;
    int16_t element_count_min;
    int16_t field_1E4;
    int16_t font_h;
    int16_t field_1E8;
    int16_t field_1EA;
    int16_t width;
    int16_t width_min;
    int16_t width_max;
    char wnd_title[64];
    char *slider_cmdbuf;
    char *data_cmdbuf;
    int16_t field_23A;
    int16_t field_23C;
    int16_t field_23E;
    int16_t field_240;
    int16_t field_242;
    int16_t field_244;
    int16_t field_246;
};

struct listview_t1
{
    const char *txt;
    int field_width;
    uint8_t tileset_id;
    uint8_t bkg_tile;
    uint8_t left_tile;
    uint8_t right_tile;
    int16_t flags;
};



char * lstvw_make_title(_NC_STACK_ypaworld *yw, int xpos, int ypos, int a3, const char *a5, char *a6, uint8_t a7, int flag);
void lstvw_updlimits(_NC_STACK_ypaworld *yw, listview *lstvw, int a3, int a4);
void lstvw_update_main(_NC_STACK_ypaworld *yw, listview *lstvw);
int lstvw_init(_NC_STACK_ypaworld *yw, listview *lstvw, ...);
char *lstvw_up_border(_NC_STACK_ypaworld *yw, listview *lstvw, char *cmdbuf, int tileset, const char *a5);
char * lstvw_down_border(_NC_STACK_ypaworld *yw, listview *lstvw, char *a4, int tileset, const char *a5);
char * lstvw_txt_line(_NC_STACK_ypaworld *yw, char *a2, int a3, listview_t1 *a4);
char * lstvw_txt_line_textual(_NC_STACK_ypaworld *yw, char *cmdbuf, listview_t1 *arg);
char *lstvw_txt_line_tiles(_NC_STACK_ypaworld *yw, char *cmdbuf, listview_t1 *arg);
void lstvw_update_input(_NC_STACK_ypaworld *yw, listview *lstvw, struC5 *struc);
void sub_4C31EC(_NC_STACK_ypaworld *yw, listview *lstview);
void lstvw_update(_NC_STACK_ypaworld *yw, listview *lstview);
void  sub_4C31C0(_NC_STACK_ypaworld *yw, listview *lstvw);

#endif //LSTVW_H_INCLUDED
