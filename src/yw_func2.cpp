#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "def_parser.h"
#include "yw.h"
#include "yw_internal.h"
#include "font.h"
#include "button.h"

#include <math.h>

extern Key_stru keySS[256];
extern char **ypaworld__string_pointers;
extern int word_5A50C2;
extern int word_5A50AC;
extern int word_5A50B0;
extern int dword_5A50B2;
extern int dword_5A50B2_h;
extern int word_5A50AE;
extern int word_5A50BC;
extern int word_5A50BA;
extern int word_5A50BE;
extern int word_5A50C0;

extern int dword_5A50B6;
extern int dword_5A50B6_h;

void sb_0x4eb94c__sub0(_NC_STACK_ypaworld *yw, unsigned int obj_id, int a3, xyz *pos, base77Func *arg)
{
    //brf_obj *brobj = &yw->brief.brf_objs + obj_id; // Only one object
    brf_obj *brobj = &yw->brief.brf_objs;

    NC_STACK_base *model_base = yw->vhcls_models[ yw->VhclProtos[ brobj->object_id ].vp_normal ].base;
    model_base->setBASE_visLimit(16000);
    model_base->setBASE_fadeLength(100);

    flag_xyz tmp;
    tmp.flag = 7;
    tmp.x = pos->sx;
    tmp.y = pos->sy;
    tmp.z = pos->sz;

    model_base->base_func68(&tmp);

    if ( obj_id >= 3 )
    {
        brobj->field_8 -= (arg->field_0 / 5);
        if ( brobj->field_8 < 0 )
            brobj->field_8 += 360;
    }
    else
    {
        brobj->field_8 += (arg->field_0 / 5);
        if ( brobj->field_8 >= 360 )
            brobj->field_8 -= 360;
    }

    flag_xyz2 rot;
    rot.flag = 7;
    rot.x = a3 + 10;
    rot.y = brobj->field_8;
    rot.z = 0;

    model_base->base_func70(&rot);
    //printf("Try DRAW %d\n", (int)model_base);
    model_base->base_func77(arg); //Draw vehicle
}

void sb_0x4eb94c__sub1(_NC_STACK_ypaworld *yw, unsigned int obj_id, int rot, xyz *pos, base77Func *arg)
{
    //brf_obj *brobj = &yw->brief.brf_objs + obj_id; // Only one object
    brf_obj *brobj = &yw->brief.brf_objs;

    secType *scType = &yw->secTypes[brobj->object_id];

    NC_STACK_base *v7 = yw->vhcls_models[0].base;

    if ( obj_id >= 3 )
    {
        brobj->field_8 -= (arg->field_0 / 5);
        if ( brobj->field_8 < 0 )
            brobj->field_8 += 360;
    }
    else
    {
        brobj->field_8 += (arg->field_0 / 5);
        if ( brobj->field_8 >= 360 )
            brobj->field_8 -= 360;
    }

    flag_xyz2 v17;
    v17.flag = 7;
    v17.x = rot + 10;
    v17.y = brobj->field_8;
    v17.z = 0;

    v7->base_func70(&v17);

    base_1c_struct *p3d = v7->getBASE_pTransform();

    int first;
    int demens;

    if ( scType->field_0 == 1 )
    {
        first = 0;
        demens = 1;
    }
    else
    {
        first = -1;
        demens = 3;
    }

    int v22 = first;
    for (int i = 0; i < demens; i++)
    {
        int v30 = first;
        for (int j = 0; j < demens; j++)
        {
            float v13 = v30 * 300.0;
            float v14 = v22 * 300.0;

            flag_xyz v16;
            v16.flag = 7;
            v16.x = p3d->scale_rotation.m00 * v13 + pos->sx + 0.0 * p3d->scale_rotation.m01 + p3d->scale_rotation.m02 * v14;
            v16.y = p3d->scale_rotation.m10 * v13 + pos->sy + 0.0 * p3d->scale_rotation.m11 + p3d->scale_rotation.m12 * v14;
            v16.z = p3d->scale_rotation.m20 * v13 + pos->sz + 0.0 * p3d->scale_rotation.m21 + p3d->scale_rotation.m22 * v14;

            NC_STACK_base *lego = yw->legos[ scType->buildings[j][i]->health_models[0] ].base;
            lego->setBASE_static(0);
            lego->setBASE_visLimit(16000);
            lego->setBASE_fadeLength(100);

            lego->base_func70(&v17);
            lego->base_func68(&v16);
            lego->base_func77(arg);

            v30++;
        }
        v22++;
    }
}

void sb_0x4eb94c(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc, int object_id, int a5)
{
    //brf_obj *brobj = &brf->brf_objs + object_id; // Only one object
    brf_obj *brobj = &brf->brf_objs;

    brf->field_4174.field_0 = struc->period;
    brf->field_4174.field_4 = brf->field_2E7C;
    brf->field_4174.field_1C = 1;

    base_1c_struct v14;
    memset(&v14, 0, sizeof(base_1c_struct));
    v14.scale_x = 1.0;
    v14.scale_y = 1.0;
    v14.scale_z = 1.0;
    v14.scale_rotation.m00 = 1.0;
    v14.scale_rotation.m01 = 0;
    v14.scale_rotation.m02 = 0;
    v14.scale_rotation.m10 = 0;
    v14.scale_rotation.m11 = 1.0;
    v14.scale_rotation.m12 = 0;
    v14.scale_rotation.m20 = 0;
    v14.scale_rotation.m21 = 0;
    v14.scale_rotation.m22 = 1.0;

    sub_430A20(&v14);
    sub_430A38(&v14);

    xyz pos;

    if ( brobj->field_0 )
    {
        pos.sx = (brobj->field_18 + brobj->field_10) * 0.5;
        pos.sy = (brobj->field_1C + brobj->field_14) * 0.5;

        float v16;
        float v17;
        float v18;
        int rot;

        if ( brobj->field_0 == 1 )
        {
            v16 = 9600.0;
            v17 = 3600.0;
        }
        else if ( brobj->field_0 == 2 )
        {
            float radius = yw->VhclProtos[brobj->object_id].radius;

            v17 = radius * 7.0;
            v16 = radius * 32.0;
        }

        if ( a5 >= 500 )
        {
            v18 = v17;
            rot = 0;
        }
        else
        {
            v18 = v16 + (v17 - v16) * a5 / 500.0;
            rot = -90 * a5 / 500 + 90;
        }

        pos.sz = v18;
        pos.sy = pos.sy * v18;
        pos.sx = pos.sx * v18;

        if ( brobj->field_0 == 1 )
        {
            sb_0x4eb94c__sub1(yw, object_id, rot, &pos, &brf->field_4174);
        }
        else if ( brobj->field_0 == 2 )
        {
            sb_0x4eb94c__sub0(yw, object_id, rot, &pos, &brf->field_4174);
        }
    }
}

int sub_4D7BFC(const void *a1, const void *a2)
{
    return ((const polys*)a1)->range - ((const polys*)a2)->range;
}


void ypaworld_func158__DrawVehicle(_NC_STACK_ypaworld *yw, big_ypa_Brf *brf, struC5 *struc)
{
    yw->win3d->raster_func213(NULL);

    brf->field_4174.field_0 = 1;
    brf->field_4174.field_4 = 1;
    brf->field_4174.field_14 = 0;
    brf->field_4174.field_18 = 1200;
    brf->field_4174.field_1C = 1;
    brf->field_4174.field_20 = 17.0;
    brf->field_4174.field_24 = 32000.0;
    brf->field_4174.rndrSTK_cur = p_renderStack;
    brf->field_4174.argSTK_cur = p_renderARGstack;
    brf->field_4174.argSTK_end = p_renderARGstackEND;

    if ( brf->brf_objs.field_0 )
    {
        int v7 = brf->field_2E7C - brf->brf_objs.field_C;
        if ( v7 > 50 )
            sb_0x4eb94c(yw, brf, struc, 0, v7 - 50);
    }

    int v8 = brf->field_4174.rndrSTK_cur - p_renderStack;

    if ( v8 > 1 )
        qsort(p_renderStack, brf->field_4174.rndrSTK_cur - p_renderStack, sizeof(polys), sub_4D7BFC);

    for (int i = 0; i < v8; i++)
    {
        polysDat *pol = p_renderStack[i].data;
        pol->render_func(&pol->datSub);
    }

    yw->win3d->raster_func214(NULL);
}



void yw_draw_input_list(_NC_STACK_ypaworld *yw, UserData *usr)
{
    lstvw_updlimits(yw, &usr->input_listview, -2, -2);
    GFXe.getTileset(0);

    char *v4 = lstvw_up_border(yw, &usr->input_listview, usr->input_listview.data_cmdbuf, 0, "uvw");

    for (int i = 1; i <= usr->input_listview.element_count; i++ )
    {
        int v24 = i + usr->input_listview.scroll_pos;
        if ( usr->keyConfig[v24].slider_name )
        {
            listview_t1 a1a[2];

            //v5 = v24;
            memset(a1a, 0, sizeof(a1a));

            int v33;
            int v31;
            int v32;
            int v30;

            if ( v24 == usr->field_D36 )
            {
                v30 = 98;
                v31 = 100;
                v32 = 9;
                v33 = 99;
            }
            else
            {
                v30 = 102;
                v31 = 102;
                v32 = 0;
                v33 = 102;
            }

            int v34 = usr->input_listview.width - 2 * usr->p_ypaworld->font_default_w__b + 1;

            char v19[200];

            if ( usr->keyConfig[ v24 ].inp_type == 2 )
            {
                const char *nm1, *a4;
                if ( usr->keyConfig[ v24 ].slider_neg )
                    nm1 = keySS[usr->keyConfig[ v24 ].slider_neg].title_by_language;
                else
                    nm1 = "-";

                if ( usr->keyConfig[ v24 ].KeyCode )
                    a4 = keySS[ usr->keyConfig[ v24 ].KeyCode ].title_by_language;
                else
                    a4 = "-";

                if ( usr->keyConfig[ v24 ].field_10 & 1 )
                    a4 = get_lang_string(ypaworld__string_pointers, 308, "?");

                if ( usr->keyConfig[ v24 ].field_10 & 2 )
                    nm1 = get_lang_string(ypaworld__string_pointers, 308, "?");

                sprintf(v19, "%s/%s", nm1, a4);
            }
            else
            {
                const char *v8;

                if ( usr->keyConfig[ v24 ].KeyCode )
                    v8 = keySS[ usr->keyConfig[ v24 ].KeyCode ].title_by_language;
                else
                    v8 = "-";

                if ( usr->keyConfig[ v24 ].field_10 & 1 )
                    v8 = get_lang_string(ypaworld__string_pointers, 308, "?");

                sprintf(v19, "%s", v8);
            }

            a1a[0].txt = usr->keyConfig[v24].slider_name;
            a1a[0].field_width = v34 * 0.68;
            a1a[0].tileset_id = v32;
            a1a[0].bkg_tile = v33;
            a1a[0].left_tile = v30;
            a1a[0].right_tile = 0;
            a1a[0].flags = 37;

            a1a[1].txt = v19;
            a1a[1].field_width = v34 - v34 * 0.68;
            a1a[1].tileset_id = v32;
            a1a[1].bkg_tile = v33;
            a1a[1].left_tile = 0;
            a1a[1].right_tile = v31;
            a1a[1].flags = 38;

            fntcmd_select_tileset(&v4, 0);
            fntcmd_store_s8(&v4, '{'); // Left wnd border

            if ( v24 == usr->field_D36 )
            {
                fntcmd_set_txtColor(&v4, usr->p_ypaworld->iniColors[62].r, usr->p_ypaworld->iniColors[62].g, usr->p_ypaworld->iniColors[62].b);
            }
            else
            {
                fntcmd_set_txtColor(&v4, usr->p_ypaworld->iniColors[61].r, usr->p_ypaworld->iniColors[61].g, usr->p_ypaworld->iniColors[61].b);
            }

            v4 = lstvw_txt_line(yw, v4, 2, a1a);

            fntcmd_select_tileset(&v4, 0);
            fntcmd_store_s8(&v4, '}'); // Right wnd border
            fntcmd_next_line(&v4);
        }
    }
    v4 = lstvw_down_border(yw, &usr->input_listview, v4, 0, "xyz");
    fntcmd_set_end(&v4);

    w3d_a209 v21;
    v21 = usr->input_listview.cmdstrm;

    GFXe.drawText(&v21);
}


void set_keys_vals(_NC_STACK_ypaworld *yw)
{
    memset(keySS, 0, sizeof(keySS));

    keySS[0].title_by_language = "*";
    keySS[0].KEYCODE = '*';
    keySS[0].short_name = "nop";

    keySS[27].title_by_language = get_lang_string(yw->string_pointers_p2, 1001, "ESC");
    keySS[27].KEYCODE = 0;
    keySS[27].short_name = "esc";

    keySS[32].title_by_language = get_lang_string(yw->string_pointers_p2, 1002, "SPACE");
    keySS[32].KEYCODE = ' ';
    keySS[32].short_name = "space";

    keySS[38].title_by_language = get_lang_string(yw->string_pointers_p2, 1003, "UP");
    keySS[38].KEYCODE = 0;
    keySS[38].short_name = "up";

    keySS[40].title_by_language = get_lang_string(yw->string_pointers_p2, 1004, "DOWN");
    keySS[40].KEYCODE = 0;
    keySS[40].short_name = "down";

    keySS[37].title_by_language = get_lang_string(yw->string_pointers_p2, 1005, "LEFT");
    keySS[37].short_name = "left";
    keySS[37].KEYCODE = 0;

    keySS[39].title_by_language = get_lang_string(yw->string_pointers_p2, 1006, "RIGHT");
    keySS[39].KEYCODE = 0;
    keySS[39].short_name = "right";

    keySS[112].title_by_language = get_lang_string(yw->string_pointers_p2, 1007, "F1");
    keySS[112].KEYCODE = 0;
    keySS[112].short_name = "f1";

    keySS[113].title_by_language = get_lang_string(yw->string_pointers_p2, 1008, "F2");
    keySS[113].KEYCODE = 0;
    keySS[113].short_name = "f2";

    keySS[114].title_by_language = get_lang_string(yw->string_pointers_p2, 1009, "F3");
    keySS[114].KEYCODE = 0;
    keySS[114].short_name = "f3";

    keySS[115].title_by_language = get_lang_string(yw->string_pointers_p2, 1010, "F4");
    keySS[115].KEYCODE = 0;
    keySS[115].short_name = "f4";

    keySS[116].title_by_language = get_lang_string(yw->string_pointers_p2, 1011, "F5");
    keySS[116].short_name = "f5";
    keySS[116].KEYCODE = 0;

    keySS[117].title_by_language = get_lang_string(yw->string_pointers_p2, 1012, "F6");
    keySS[117].short_name = "f6";
    keySS[117].KEYCODE = 0;

    keySS[118].title_by_language = get_lang_string(yw->string_pointers_p2, 1013, "F7");
    keySS[118].KEYCODE = 0;
    keySS[118].short_name = "f7";

    keySS[119].title_by_language = get_lang_string(yw->string_pointers_p2, 1014, "F8");
    keySS[119].KEYCODE = 0;
    keySS[119].short_name = "f8";

    keySS[120].title_by_language = get_lang_string(yw->string_pointers_p2, 1015, "F9");
    keySS[120].KEYCODE = 0;
    keySS[120].short_name = "f9";

    keySS[121].title_by_language = get_lang_string(yw->string_pointers_p2, 1016, "F10");
    keySS[121].KEYCODE = 0;
    keySS[121].short_name = "f10";

    keySS[122].title_by_language = get_lang_string(yw->string_pointers_p2, 1017, "F11");
    keySS[122].short_name = "f11";
    keySS[122].KEYCODE = 0;

    keySS[123].title_by_language = get_lang_string(yw->string_pointers_p2, 1018, "F12");
    keySS[123].short_name = "f12";
    keySS[123].KEYCODE = 0;

    keySS[8].title_by_language = get_lang_string(yw->string_pointers_p2, 1019, "BACK");
    keySS[8].KEYCODE = 0;
    keySS[8].short_name = "bs";

    keySS[9].title_by_language = get_lang_string(yw->string_pointers_p2, 1020, "TAB");
    keySS[9].KEYCODE = 0;
    keySS[9].short_name = "tab";

    keySS[12].title_by_language = get_lang_string(yw->string_pointers_p2, 1021, "CLEAR");
    keySS[12].KEYCODE = 0;
    keySS[12].short_name = "clear";

    keySS[13].title_by_language = get_lang_string(yw->string_pointers_p2, 1022, "RETURN");
    keySS[13].KEYCODE = 0;
    keySS[13].short_name = "return";

    keySS[17].title_by_language = get_lang_string(yw->string_pointers_p2, 1023, "CTRL");
    keySS[17].short_name = "ctrl";
    keySS[17].KEYCODE = 0;

    keySS[16].title_by_language = get_lang_string(yw->string_pointers_p2, 1024, "SHIFT");
    keySS[16].short_name = "rshift";
    keySS[16].KEYCODE = 0;

    keySS[18].title_by_language = get_lang_string(yw->string_pointers_p2, 1025, "ALT");
    keySS[18].KEYCODE = 0;
    keySS[18].short_name = "alt";

    keySS[19].title_by_language = get_lang_string(yw->string_pointers_p2, 1026, "PAUSE");
    keySS[19].KEYCODE = 0;
    keySS[19].short_name = "pause";

    keySS[33].title_by_language = get_lang_string(yw->string_pointers_p2, 1027, "PGUP");
    keySS[33].KEYCODE = 0;
    keySS[33].short_name = "pageup";

    keySS[34].title_by_language = get_lang_string(yw->string_pointers_p2, 1028, "PGDOWN");
    keySS[34].KEYCODE = 0;
    keySS[34].short_name = "pagedown";

    keySS[35].title_by_language = get_lang_string(yw->string_pointers_p2, 1029, "END");
    keySS[35].short_name = "end";
    keySS[35].KEYCODE = 0;

    keySS[36].title_by_language = get_lang_string(yw->string_pointers_p2, 1030, "HOME");
    keySS[36].short_name = "home";
    keySS[36].KEYCODE = 0;

    keySS[41].title_by_language = get_lang_string(yw->string_pointers_p2, 1031, "SELECT");
    keySS[41].KEYCODE = 0;
    keySS[41].short_name = "select";

    keySS[43].title_by_language = get_lang_string(yw->string_pointers_p2, 1032, "EXEC");
    keySS[43].KEYCODE = 0;
    keySS[43].short_name = "execute";

    keySS[44].title_by_language = get_lang_string(yw->string_pointers_p2, 1033, "PRINT");
    keySS[44].KEYCODE = 0;
    keySS[44].short_name = "snapshot";

    keySS[45].title_by_language = get_lang_string(yw->string_pointers_p2, 1034, "INS");
    keySS[45].KEYCODE = 0;
    keySS[45].short_name = "ins";

    keySS[46].title_by_language = get_lang_string(yw->string_pointers_p2, 1035, "DEL");
    keySS[46].short_name = "del";
    keySS[46].KEYCODE = 0;

    keySS[49].title_by_language = get_lang_string(yw->string_pointers_p2, 1037, "1");
    keySS[49].short_name = "1";
    keySS[49].KEYCODE = '1';

    keySS[50].title_by_language = get_lang_string(yw->string_pointers_p2, 1038, "2");
    keySS[50].KEYCODE = '2';
    keySS[50].short_name = "2";

    keySS[51].title_by_language = get_lang_string(yw->string_pointers_p2, 1039, "3");
    keySS[51].KEYCODE = '3';
    keySS[51].short_name = "3";

    keySS[52].title_by_language = get_lang_string(yw->string_pointers_p2, 1040, "4");
    keySS[52].KEYCODE = 0x34;
    keySS[52].short_name = "4";

    keySS[53].title_by_language = get_lang_string(yw->string_pointers_p2, 1041, "5");
    keySS[53].KEYCODE = '5';
    keySS[53].short_name = "5";

    keySS[54].title_by_language = get_lang_string(yw->string_pointers_p2, 1042, "6");
    keySS[54].KEYCODE = '6';
    keySS[54].short_name = "6";

    keySS[55].title_by_language = get_lang_string(yw->string_pointers_p2, 1043, "7");
    keySS[55].short_name = "7";
    keySS[55].KEYCODE = '7';

    keySS[56].title_by_language = get_lang_string(yw->string_pointers_p2, 1044, "8");
    keySS[56].KEYCODE = '8';
    keySS[56].short_name = "8";

    keySS[57].title_by_language = get_lang_string(yw->string_pointers_p2, 1045, "9");
    keySS[57].KEYCODE = '9';
    keySS[57].short_name = "9";

    keySS[48].title_by_language = get_lang_string(yw->string_pointers_p2, 1046, "0");
    keySS[48].KEYCODE = '0';
    keySS[48].short_name = "0";

    keySS[65].title_by_language = get_lang_string(yw->string_pointers_p2, 1047, "A");
    keySS[65].KEYCODE = 'A';
    keySS[65].short_name = "a";

    keySS[66].title_by_language = get_lang_string(yw->string_pointers_p2, 1048, "B");
    keySS[66].short_name = "b";
    keySS[66].KEYCODE = 'B';

    keySS[67].title_by_language = get_lang_string(yw->string_pointers_p2, 1049, "C");
    keySS[67].KEYCODE = 'C';
    keySS[67].short_name = "c";

    keySS[68].title_by_language = get_lang_string(yw->string_pointers_p2, 1050, "D");
    keySS[68].KEYCODE = 68;
    keySS[68].short_name = "d";

    keySS[69].title_by_language = get_lang_string(yw->string_pointers_p2, 1051, "E");
    keySS[69].KEYCODE = 69;
    keySS[69].short_name = "e";

    keySS[70].title_by_language = get_lang_string(yw->string_pointers_p2, 1052, "F");
    keySS[70].KEYCODE = 70;
    keySS[70].short_name = "f";

    keySS[71].title_by_language = get_lang_string(yw->string_pointers_p2, 1053, "G");
    keySS[71].KEYCODE = 71;
    keySS[71].short_name = "g";

    keySS[72].title_by_language = get_lang_string(yw->string_pointers_p2, 1054, "H");
    keySS[72].short_name = "h";
    keySS[72].KEYCODE = 72;

    keySS[73].title_by_language = get_lang_string(yw->string_pointers_p2, 1055, "I");
    keySS[73].short_name = "i";
    keySS[73].KEYCODE = 73;

    keySS[74].title_by_language = get_lang_string(yw->string_pointers_p2, 1056, "J");
    keySS[74].KEYCODE = 74;
    keySS[74].short_name = "j";

    keySS[75].title_by_language = get_lang_string(yw->string_pointers_p2, 1057, "K");
    keySS[75].KEYCODE = 75;
    keySS[75].short_name = "k";

    keySS[76].title_by_language = get_lang_string(yw->string_pointers_p2, 1058, "L");
    keySS[76].KEYCODE = 76;
    keySS[76].short_name = "l";

    keySS[77].title_by_language = get_lang_string(yw->string_pointers_p2, 1059, "M");
    keySS[77].KEYCODE = 77;
    keySS[77].short_name = "m";

    keySS[78].title_by_language = get_lang_string(yw->string_pointers_p2, 1060, "N");
    keySS[78].short_name = "n";
    keySS[78].KEYCODE = 78;

    keySS[79].title_by_language = get_lang_string(yw->string_pointers_p2, 1061, "O");
    keySS[79].short_name = "o";
    keySS[79].KEYCODE = 79;

    keySS[80].title_by_language = get_lang_string(yw->string_pointers_p2, 1062, "P");
    keySS[80].KEYCODE = 80;
    keySS[80].short_name = "p";

    keySS[81].title_by_language = get_lang_string(yw->string_pointers_p2, 1063, "Q");
    keySS[81].KEYCODE = 81;
    keySS[81].short_name = "q";

    keySS[82].title_by_language = get_lang_string(yw->string_pointers_p2, 1064, "R");
    keySS[82].KEYCODE = 82;
    keySS[82].short_name = "r";

    keySS[83].title_by_language = get_lang_string(yw->string_pointers_p2, 1065, "S");
    keySS[83].KEYCODE = 83;
    keySS[83].short_name = "s";

    keySS[84].title_by_language = get_lang_string(yw->string_pointers_p2, 1066, "T");
    keySS[84].short_name = "t";
    keySS[84].KEYCODE = 84;

    keySS[85].title_by_language = get_lang_string(yw->string_pointers_p2, 1067, "U");
    keySS[85].short_name = "u";
    keySS[85].KEYCODE = 85;

    keySS[86].title_by_language = get_lang_string(yw->string_pointers_p2, 1068, "V");
    keySS[86].KEYCODE = 86;
    keySS[86].short_name = "v";

    keySS[87].title_by_language = get_lang_string(yw->string_pointers_p2, 1069, "W");
    keySS[87].KEYCODE = 87;
    keySS[87].short_name = "w";

    keySS[88].title_by_language = get_lang_string(yw->string_pointers_p2, 1070, "X");
    keySS[88].KEYCODE = 88;
    keySS[88].short_name = "x";

    keySS[89].title_by_language = get_lang_string(yw->string_pointers_p2, 1071, "Y");
    keySS[89].KEYCODE = 89;
    keySS[89].short_name = "y";

    keySS[90].title_by_language = get_lang_string(yw->string_pointers_p2, 1072, "Z");
    keySS[90].short_name = "z";
    keySS[90].KEYCODE = 90;

    keySS[96].title_by_language = get_lang_string(yw->string_pointers_p2, 1073, "NUM 0");
    keySS[96].short_name = "num0";
    keySS[96].KEYCODE = 0;

    keySS[97].title_by_language = get_lang_string(yw->string_pointers_p2, 1074, "NUM 1");
    keySS[97].KEYCODE = 0;
    keySS[97].short_name = "num1";

    keySS[98].title_by_language = get_lang_string(yw->string_pointers_p2, 1075, "NUM 2");
    keySS[98].KEYCODE = 0;
    keySS[98].short_name = "num2";

    keySS[99].title_by_language = get_lang_string(yw->string_pointers_p2, 1076, "NUM 3");
    keySS[99].KEYCODE = 0;
    keySS[99].short_name = "num3";

    keySS[100].title_by_language = get_lang_string(yw->string_pointers_p2, 1077, "NUM 4");
    keySS[100].KEYCODE = 0;
    keySS[100].short_name = "num4";

    keySS[101].title_by_language = get_lang_string(yw->string_pointers_p2, 1078, "NUM 5");
    keySS[101].short_name = "num5";
    keySS[101].KEYCODE = 0;

    keySS[102].title_by_language = get_lang_string(yw->string_pointers_p2, 1079, "NUM 6");
    keySS[102].short_name = "num6";
    keySS[102].KEYCODE = 0;

    keySS[103].title_by_language = get_lang_string(yw->string_pointers_p2, 1080, "NUM 7");
    keySS[103].KEYCODE = 0;
    keySS[103].short_name = "num7";

    keySS[104].title_by_language = get_lang_string(yw->string_pointers_p2, 1081, "NUM 8");
    keySS[104].KEYCODE = 0;
    keySS[104].short_name = "num8";

    keySS[105].title_by_language = get_lang_string(yw->string_pointers_p2, 1082, "NUM 9");
    keySS[105].KEYCODE = 0;
    keySS[105].short_name = "num9";

    keySS[106].title_by_language = get_lang_string(yw->string_pointers_p2, 1083, "MUL");
    keySS[106].KEYCODE = 0;
    keySS[106].short_name = "nummul";

    keySS[107].title_by_language = get_lang_string(yw->string_pointers_p2, 1084, "ADD");
    keySS[107].KEYCODE = 0;
    keySS[107].short_name = "numplus";

    keySS[110].title_by_language = get_lang_string(yw->string_pointers_p2, 1085, "DOT");
    keySS[110].short_name = "numdot";
    keySS[110].KEYCODE = 0;

    keySS[109].title_by_language = get_lang_string(yw->string_pointers_p2, 1086, "SUB");
    keySS[109].KEYCODE = 0;
    keySS[109].short_name = "numminus";

    keySS[108].title_by_language = get_lang_string(yw->string_pointers_p2, 1087, "ENTER");
    keySS[108].KEYCODE = 0;
    keySS[108].short_name = "enter";

    keySS[111].title_by_language = get_lang_string(yw->string_pointers_p2, 1088, "DIV");
    keySS[111].KEYCODE = 0;
    keySS[111].short_name = "numdiv";

    keySS[188].title_by_language = get_lang_string(yw->string_pointers_p2, 1089, "EXTRA_1");
    keySS[188].KEYCODE = 44;
    keySS[188].short_name = "extra1";

    keySS[190].title_by_language = get_lang_string(yw->string_pointers_p2, 1090, "EXTRA_2");
    keySS[190].short_name = "extra2";
    keySS[190].KEYCODE = 46;

    keySS[189].title_by_language = get_lang_string(yw->string_pointers_p2, 1091, "EXTRA_3");
    keySS[189].KEYCODE = 45;
    keySS[189].short_name = "extra3";

    keySS[226].title_by_language = get_lang_string(yw->string_pointers_p2, 1092, "EXTRA_4");
    keySS[226].KEYCODE = 60;
    keySS[226].short_name = "extra4";

    keySS[186].title_by_language = get_lang_string(yw->string_pointers_p2, 1093, "EXTRA_5");
    keySS[186].KEYCODE = 252;
    keySS[186].short_name = "extra5";

    keySS[187].title_by_language = get_lang_string(yw->string_pointers_p2, 1094, "EXTRA_6");
    keySS[187].KEYCODE = 43;
    keySS[187].short_name = "extra6";

    keySS[192].title_by_language = get_lang_string(yw->string_pointers_p2, 1095, "EXTRA_7");
    keySS[192].KEYCODE = 246;
    keySS[192].short_name = "extra7";

    keySS[222].title_by_language = get_lang_string(yw->string_pointers_p2, 1096, "EXTRA_8");
    keySS[222].short_name = "extra8";
    keySS[222].KEYCODE = 228;

    keySS[191].title_by_language = get_lang_string(yw->string_pointers_p2, 1097, "EXTRA_9");
    keySS[191].short_name = "extra9";
    keySS[191].KEYCODE = 35;

    keySS[221].title_by_language = get_lang_string(yw->string_pointers_p2, 1098, "EXTRA_10");
    keySS[221].KEYCODE = 96;
    keySS[221].short_name = "extra10";

    keySS[220].title_by_language = get_lang_string(yw->string_pointers_p2, 1099, "EXTRA_11");
    keySS[220].KEYCODE = 94;
    keySS[220].short_name = "extra11";

    keySS[219].title_by_language = get_lang_string(yw->string_pointers_p2, 1100, "EXTRA_12");
    keySS[219].KEYCODE = 0;
    keySS[219].short_name = "extra12";

    keySS[223].title_by_language = get_lang_string(yw->string_pointers_p2, 1101, "EXTRA_13");
    keySS[223].KEYCODE = 0;
    keySS[223].short_name = "extra13";

    keySS[145].title_by_language = get_lang_string(yw->string_pointers_p2, 1102, "EXTRA_14");
    keySS[145].short_name = "extra14";
    keySS[145].KEYCODE = 0;

    keySS[144].title_by_language = get_lang_string(yw->string_pointers_p2, 1103, "EXTRA_15");
    keySS[144].short_name = "extra15";
    keySS[144].KEYCODE = 0;

    keySS[124].title_by_language = get_lang_string(yw->string_pointers_p2, 1104, "EXTRA_16");
    keySS[124].KEYCODE = 0;
    keySS[124].short_name = "extra16";

    keySS[125].title_by_language = get_lang_string(yw->string_pointers_p2, 1105, "EXTRA_17");
    keySS[125].KEYCODE = 0;
    keySS[125].short_name = "extra17";

    keySS[126].title_by_language = get_lang_string(yw->string_pointers_p2, 1106, "EXTRA_18");
    keySS[126].KEYCODE = 0;
    keySS[126].short_name = "extra18";

    keySS[131].title_by_language = get_lang_string(yw->string_pointers_p2, 1121, "MIDDLE MOUSE");
    keySS[131].KEYCODE = 0;
    keySS[131].short_name = "mmb";

    keySS[134].title_by_language = get_lang_string(yw->string_pointers_p2, 1123, "JOYB0");
    keySS[134].short_name = "joyb0";
    keySS[134].KEYCODE = 0;

    keySS[135].title_by_language = get_lang_string(yw->string_pointers_p2, 1124, "JOYB1");
    keySS[135].short_name = "joyb1";
    keySS[135].KEYCODE = 0;

    keySS[136].title_by_language = get_lang_string(yw->string_pointers_p2, 1125, "JOYB2");
    keySS[136].KEYCODE = 0;
    keySS[136].short_name = "joyb2";

    keySS[137].title_by_language = get_lang_string(yw->string_pointers_p2, 1126, "JOYB3");
    keySS[137].KEYCODE = 0;
    keySS[137].short_name = "joyb3";

    keySS[138].title_by_language = get_lang_string(yw->string_pointers_p2, 1127, "JOYB4");
    keySS[138].KEYCODE = 0;
    keySS[138].short_name = "joyb4";

    keySS[139].title_by_language = get_lang_string(yw->string_pointers_p2, 1128, "JOYB5");
    keySS[139].KEYCODE = 0;
    keySS[139].short_name = "joyb5";

    keySS[140].title_by_language = get_lang_string(yw->string_pointers_p2, 1129, "JOYB6");
    keySS[140].short_name = "joyb6";
    keySS[140].KEYCODE = 0;

    keySS[141].title_by_language = get_lang_string(yw->string_pointers_p2, 1130, "JOYB7");
    keySS[141].short_name = "joyb7";
    keySS[141].KEYCODE = 0;
}


int yw_loadSky(_NC_STACK_ypaworld *yw, const char *skyname)
{
    char buf[256];
    strcpy(buf, "data:");
    strcat(buf, skyname);

    NC_STACK_base *sky = READ_BAS_FILE(buf);
    yw->sky_loaded_base = sky;
    if ( !sky )
    {
        ypa_log_out("Couldn't create %s\n", buf);
        return 0;
    }

    sky->setBASE_static(1); // Don't rotate sky
    sky->setBASE_visLimit(yw->field_15ec);
    sky->setBASE_fadeLength(yw->field_15f0);
    return 1;
}

void fill_videmodes_list(UserData *usr)
{
    while ( 1 )
    {
        nnode *nod = RemHead(&usr->video_mode_list);
        if ( !nod )
            break;
        nc_FreeMem(nod);
    }

    usr->p_ypaworld->win3d = GFXe.getC3D();

    windd_arg256 warg_256;
    warg_256.sort_id = 0;

    int id = -1;

    while( id )
    {
        id = usr->p_ypaworld->win3d->display_func256(&warg_256);

        video_mode_node *vnode = (video_mode_node *)AllocVec(sizeof(video_mode_node), 65536);

        if ( vnode )
        {
            vnode->sort_id = warg_256.sort_id;
            vnode->width = warg_256.width;
            vnode->height = warg_256.height;

            memset(vnode->name, 0, 128);
            strncpy(vnode->name, warg_256.name, 127);

            AddTail(&usr->video_mode_list, vnode);
        }
        warg_256.sort_id = id;
    }
}


void listSaveDir(UserData *usr, const char *saveDir)
{
    player_status statuses[8];

    memcpy(statuses, usr->p_ypaworld->playerstatus, sizeof(player_status) * 8);

    char v21[300];

    strcpy(v21, usr->callSIGN);

    int v5 = usr->p_ypaworld->maxroboenergy;
    int v27 = usr->p_ypaworld->maxreloadconst;

    usr->opened_dir = OpenDir(saveDir);
    if ( usr->opened_dir )
    {
        dirEntry a2a;
        while ( ReadDir(usr->opened_dir, &a2a) )
        {
            if ( a2a.field_0 & 1 )
            {
                if ( strcmp(a2a.e_name, ".") && strcmp(a2a.e_name, "..") )
                {
                    profilesNode *v10 = (profilesNode *)AllocVec(sizeof(profilesNode), 65537);
                    if ( v10 )
                    {
                        strcpy(v10->profile_subdir, a2a.e_name);

                        AddTail(&usr->files_list, v10);

                        scrCallBack v25;

                        v25.func = parseSaveUser;
                        v25.world = (_NC_STACK_ypaworld *)usr->p_ypaworld->self_full;
                        v25.world2 = usr->p_ypaworld;

                        char buf[300];

                        sprintf(buf, "%s/%s/user.txt\n", saveDir, a2a.e_name);

                        if ( !def_parseFile(buf, 1, &v25, 2) )
                            ypa_log_out("Warning, cannot parse %s for time scanning\n", buf);

                        v10->field_C = 1;
                        v10->pStatus_3 = usr->p_ypaworld->playerstatus[1].p3;
                    }
                }
            }
        }
        CloseDir(usr->opened_dir);
    }
    else
    {
        ypa_log_out("Unknown Game-Directory %s\n", saveDir);
    }

    memcpy(usr->p_ypaworld->playerstatus, statuses, sizeof(player_status) * 8);

    usr->p_ypaworld->maxreloadconst = v27;
    usr->p_ypaworld->maxroboenergy = v5;

    strcpy(usr->callSIGN, v21);
}


void listLocaleDir(UserData *usr, const char *dirname)
{
    usr->lang_dlls_count = 0;

    langDll_node *deflng = NULL;
    ncDir *dir = OpenDir(dirname);
    if ( dir )
    {
        dirEntry v18;

        while ( ReadDir(dir, &v18) )
        {
            char *v3 = strstr(v18.e_name, ".LNG");
            if ( !v3 )
                v3 = strstr(v18.e_name, ".lng");
            /*if ( !v3 )
              v3 = strstr(v18.e_name, ".dll");
            if ( !v3 )
              v3 = strstr(v18.e_name, ".DLL");*/

            if ( !(v18.field_0 & 1) && v3 )
            {
                char v19[256];
                memset(v19, 0, 256);

                for (int i = 0; i < 31; i++)
                {
                    char v5 = v18.e_name[i];
                    if (v5 <= ' ')
                        break;
                    if (v5 == '.')
                        break;

                    v19[i] = toupper(v5);
                }

                langDll_node *v7 = (langDll_node *)usr->lang_dlls.head;

                int finded = 0;

                while( v7->next )
                {
                    if ( !strcasecmp(v7->langDllName, v19) )
                    {
                        finded = 1;
                        break;
                    }
                    v7 = (langDll_node *)v7->next;
                }

                if ( !finded )
                {
                    usr->lang_dlls_count++;

                    langDll_node *v9 = (langDll_node *)AllocVec(sizeof(langDll_node), 65537);
                    if ( v9 )
                    {
                        strcpy(v9->langDllName, v19);
                        AddTail(&usr->lang_dlls, v9);

                        if ( !strcasecmp(v9->langDllName, "language") )
                            deflng = v9;
                    }
                }
            }
        }

        CloseDir(dir);
    }
    else
    {
        ypa_log_out("Unknown Locale-Directory %s\n", dirname);
    }

    if ( deflng )
        usr->default_lang_dll = deflng;
}

void sub_46A7F8(UserData *usr)
{
    int v4 = 2;
    usr->disk_button->button_func68( &v4 );

    sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);

    NC_STACK_button *v2;
    if ( usr->field_0x1760 )
    {
        usr->field_46 = 5;
        v4 = 1;
        v2 = usr->sub_bar_button;
    }
    else
    {
        usr->field_46 = 1;
        v4 = 1;
        v2 = usr->titel_button;
    }

    v2->button_func68(&v4);

    button_66arg v3;
    v3.field_4 = 2;
    v3.butID = 1156;
    usr->video_button->button_func73(&v3);

    usr->field_0x1744 = 0;
}



void ypaworld_func154__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->movies.movies_names_present[0] )
    {
        yw->win3d = GFXe.getC3D();
        char buf[256];

        sub_412810(yw->movies.game_intro, buf, 256);
        const char *v5 = buf;

        yw->win3d->windd_func323(&v5);

        sub_412D28(&input_states);
        input_states.downed_key = 0;
        input_states.downed_key_2 = 0;
        input_states.dword8 = 0;
    }
}

int sub_4DE9F4(const void *a1, const void *a2)
{
    return strcasecmp(((usr_str *)a1)->pstring, ((usr_str *)a2)->pstring);
}

void ypaworld_func156__sub1(UserData *usr)
{
    int v2 = 0;
    for (int i = 0; i < 256; i++)
    {
        if (usr->p_ypaworld->LevelNet->mapInfos[i].field_0 == 4)
        {
            usr->map_descriptions[v2].id = i;
            usr->map_descriptions[v2].pstring = get_lang_string(ypaworld__string_pointers, i + 1800, usr->p_ypaworld->LevelNet->mapInfos[i].map_name);
            v2++;
        }
    }

    usr->map_descriptions_count = v2;

    qsort(usr->map_descriptions, v2, sizeof(usr_str), sub_4DE9F4);
}


void sub_46C524(UserData *usr)
{
    int v4 = 2;
    usr->titel_button->button_func68(&v4);

    v4 = 1;
    usr->network_button->button_func68(&v4);

    usr->field_46 = 6;

    sub_4C31C0(usr->p_ypaworld, &usr->network_listvw);
    sub_4C31EC(usr->p_ypaworld, &usr->network_listvw);
}



void sb_0x46ca74__sub0(const char *a1, const char *a2)
{
    FILE *f1 = FOpen(a1, "r");
    if ( f1 )
    {
        FILE *f2 = FOpen(a2, "w");

        if ( f2 )
        {
            char v9[300];

            while ( fgets(v9, 299, f1) )
                fputs(v9, f2);

            FClose(f2);
        }

        FClose(f1);
    }
}

void sb_0x46ca74(UserData *usr)
{
    const char *usernamedir = usr->usernamedir;

    char oldsave[300];

    if ( usr->field_1612 )
    {
        if ( strcasecmp(usr->usernamedir, usr->user_name) )
        {
            sprintf(oldsave, "save:%s", usernamedir);
            sub_46D0F8(oldsave);
        }
    }
    else
    {
        profilesNode *v3 = (profilesNode *)AllocVec(sizeof(profilesNode), 65537);

        if ( !v3 )
        {
            ypa_log_out("Warning: No Memory!\n");
            return;
        }

        strncpy(v3->profile_subdir, usernamedir, 32);

        AddTail(&usr->files_list, v3);

        char a1a[300];

        sprintf(a1a, "save:%s", usernamedir);

        if ( !createDirectory(a1a) )
        {
            ypa_log_out("Unable to create directory %s\n", a1a);
            return;
        }

        usr->disk_listvw.elements_for_scroll_size++;
        usr->field_1612 = usr->disk_listvw.elements_for_scroll_size;
    }

    sprintf(oldsave, "%s/user.txt", usr->usernamedir);

    yw_arg172 v15;
    v15.usertxt = oldsave;
    v15.field_4 = usr->usernamedir;
    v15.usr = usr;
    v15.field_8 = 255;
    v15.field_10 = 0;

    if ( ! usr->p_ypaworld->self_full->ypaworld_func171(&v15) )
        ypa_log_out("Warning! Error while saving user data for %s\n", usr->usernamedir);

    sprintf(oldsave, "save:%s", usr->user_name);

    if ( strcasecmp(usr->usernamedir, usr->user_name) )
    {
        ncDir *v8 = OpenDir(oldsave);
        dirEntry a2a;

        if ( v8 )
        {
            while ( ReadDir(v8, &a2a) )
            {
                if ( !(a2a.field_0 & 1)
                        && (strstr(a2a.e_name, ".sgm")
                            || strstr(a2a.e_name, ".SGM")
                            || strstr(a2a.e_name, ".rst")
                            || strstr(a2a.e_name, ".RST")
                            || strstr(a2a.e_name, ".fin")
                            || strstr(a2a.e_name, ".FIN")
                            || strstr(a2a.e_name, ".def")
                            || strstr(a2a.e_name, ".DEF")) )
                {
                    char v11[300];
                    char v12[300];

                    sprintf(v11, "%s/%s", oldsave, a2a.e_name);
                    sprintf(v12, "save:%s/%s", usr->usernamedir, a2a.e_name);
                    sb_0x46ca74__sub0(v11, v12);
                }
            }
            CloseDir(v8);
        }
    }

    usr->field_0x1744 = 0;
    strncpy(usr->user_name, usr->usernamedir, 32);

    int v16 = 2;
    usr->disk_button->button_func68(&v16);

    sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);

    if ( usr->field_0x1760 )
    {
        usr->field_46 = 5;

        int v16 = 1;
        usr->sub_bar_button->button_func68(&v16);
    }
    else
    {
        usr->field_46 = 1;

        int v16 = 1;
        usr->titel_button->button_func68(&v16);
    }
}

void sb_0x47f810__sub0(_NC_STACK_ypaworld *yw)
{
    if ( yw->VhclProtos )
    {
        for (int i = 0; i < 256; i++)
        {
            VhclProto *v3 = &yw->VhclProtos[i];

            if ( v3->wireframe )
            {
                delete_class_obj(v3->wireframe);
                v3->wireframe = NULL;
            }

            if ( v3->hud_wireframe )
            {
                delete_class_obj(v3->hud_wireframe);
                v3->hud_wireframe = NULL;
            }

            if ( v3->mg_wireframe )
            {
                delete_class_obj(v3->mg_wireframe);
                v3->mg_wireframe = NULL;
            }

            if ( v3->wpn_wireframe_1 )
            {
                delete_class_obj(v3->wpn_wireframe_1);
                v3->wpn_wireframe_1 = NULL;
            }

            if ( v3->wpn_wireframe_2 )
            {
                delete_class_obj(v3->wpn_wireframe_2);
                v3->wpn_wireframe_2 = NULL;
            }
        }
    }

    if ( yw->WeaponProtos )
    {
        for (int i = 0; i < 128; i++)
        {
            WeapProto *v6 = &yw->WeaponProtos[i];
            if ( v6->wireframe )
            {
                delete_class_obj(v6->wireframe);
                v6->wireframe = NULL;
            }
        }
    }
}

void sb_0x47f810(_NC_STACK_ypaworld *yw)
{
    sb_0x47f810__sub0(yw);

    if ( yw->RoboProtos )
    {
        nc_FreeMem(yw->RoboProtos);
        yw->RoboProtos = NULL;
    }

    if ( yw->BuildProtos )
    {
        nc_FreeMem(yw->BuildProtos);
        yw->BuildProtos = NULL;
    }

    if ( yw->WeaponProtos )
    {
        nc_FreeMem(yw->WeaponProtos);
        yw->WeaponProtos = NULL;
    }

    if ( yw->VhclProtos )
    {
        nc_FreeMem(yw->VhclProtos);
        yw->VhclProtos = NULL;
    }
}

void sb_0x46cdf8(UserData *usr)
{
    char a1a[300];
    sprintf(a1a, "%s/user.txt", usr->user_name);

    yw_arg172 v12;
    v12.usertxt = a1a;
    v12.field_4 = usr->user_name;
    v12.usr = usr;
    v12.field_8 = 255;
    v12.field_10 = 0;

    if ( ! usr->p_ypaworld->self_full->ypaworld_func171(&v12) )
        ypa_log_out("Warning! Error while saving user data for %s\n", usr->user_name);

    if ( usr->field_1612 )
    {
        sprintf(a1a, "save:%s", usr->usernamedir);
        sub_46D0F8(a1a);
    }
    else
    {
        profilesNode *v4 = (profilesNode *)AllocVec(47, 65537);

        if ( !v4 )
        {
            ypa_log_out("Warning: No Memory!\n");
            return;
        }

        strncpy(v4->profile_subdir, usr->usernamedir, 32);

        AddTail(&usr->files_list, v4);

        char v10[300];
        sprintf(v10, "save:%s", usr->usernamedir);

        if ( !createDirectory(v10) )
        {
            ypa_log_out("Unable to create directory %s\n", v10);
            return;
        }

        strncpy(usr->user_name, usr->usernamedir, 32);
        usr->disk_listvw.elements_for_scroll_size++;
        usr->field_1612 = usr->disk_listvw.elements_for_scroll_size;
    }

    usr->p_ypaworld->field_2d90->buddies_count = 0;

    sb_0x47f810(usr->p_ypaworld);

    if ( init_prototypes(usr->p_ypaworld) )
    {
        for (int i = 0; i < 256; i++)
        {
            mapINFO *mp = &usr->p_ypaworld->LevelNet->mapInfos[i];
            if ( mp->field_0 && mp->field_0 != 4 )
                mp->field_0 = 1;
        }

        usr->p_ypaworld->LevelNet->mapInfos[1].field_0 = 2;
        usr->p_ypaworld->LevelNet->mapInfos[25].field_0 = 2;
        usr->p_ypaworld->LevelNet->mapInfos[26].field_0 = 2;
        usr->p_ypaworld->LevelNet->mapInfos[27].field_0 = 2;
        usr->p_ypaworld->maxroboenergy = 0;
        usr->p_ypaworld->maxreloadconst = 0;

        memset(usr->p_ypaworld->playerstatus, 0, sizeof(player_status) * 8);

        usr->field_0x1744 = 0;

        usr->p_ypaworld->field_2d90->field_74 = 128;
        usr->p_ypaworld->beamenergy = usr->p_ypaworld->beam_energy_start;

        memset(usr->p_ypaworld->field_2d90->jodiefoster, 0, sizeof(int) * 8);

        usr->field_3426 = 0;

        int v13 = 2;
        usr->disk_button->button_func68(&v13);

        sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);

        usr->field_46 = 5;

        v13 = 1;
        usr->sub_bar_button->button_func68(&v13);
    }
    else
    {
        ypa_log_out("Warning, error while parsing prototypes\n");
    }
}

void sub_46D960(UserData *usr)
{
    button_66arg v4;
    v4.butID = 1300;
    usr->confirm_button->button_func67(&v4);

    v4.butID = 1301;
    usr->confirm_button->button_func67(&v4);

    int v5 = 2;
    usr->confirm_button->button_func68(&v5);

    usr->field_0x2fb4 = 0;
}

void sub_46D370(NC_STACK_ypaworld *obj, int a2)
{
    if ( a2 )
    {
        obj->setYW_visSectors(9);
        obj->setYW_normVisLimit(2100);
        obj->setYW_fadeLength(900);
    }
    else
    {
        obj->setYW_visSectors(5);
        obj->setYW_normVisLimit(1400);
        obj->setYW_fadeLength(600);
    }
}

//Options OK
void sb_0x46aa8c(UserData *usr)
{
    _NC_STACK_ypaworld *yw = usr->p_ypaworld;

    int v3 = 0;

    yw_174arg v40;
    v40.make_changes = 0;
    int v5 = 0;
    int resolution;

    if ( usr->field_13C2 & 0x200 )
    {
        CDAUDIO_t v35;

        if ( usr->field_0x13b0 & 0x10 )
        {
            usr->snd__flags2 |= 0x10;
            yw->snd__cdsound |= 1;

            v35.track_id = 1;
            v35.command = 8;

            sub_4444D4(&v35);

            if ( usr->shelltrack )
            {

                v35.command = 7;
                v35.track_id = usr->shelltrack;
                v35.field_8 = usr->shelltrack__adv.field_0;
                v35.field_C = usr->shelltrack__adv.field_4;
                sub_4444D4(&v35);

                sub_4444D4(&v35);
            }
        }
        else
        {
            usr->snd__flags2 &= 0xEF;
            yw->snd__cdsound &= 0xFE;

            v35.command = 1;
            sub_4444D4(&v35);

            v35.command = 8;
            v35.track_id = 0;
            sub_4444D4(&v35);
        }

    }

    if ( usr->field_13C2 & 2 )
    {
        if ( usr->field_0x13b0 & 1 )
        {
            usr->snd__flags2 |= 1;
            milesEngine__setter(0x80004005, 1, 0);
        }
        else
        {
            usr->snd__flags2 &= 0xFE;
            milesEngine__setter(0x80004005, 0, 0);
        }
    }

    if ( usr->field_13C2 & 0x10 )
    {
        if ( usr->field_0x13a8 & 1 )
        {
            usr->GFX_flags |= 1;
            sub_46D370(yw->self_full, 1);
        }
        else
        {
            usr->GFX_flags &= 0xFE;
            sub_46D370(yw->self_full, 0);
        }
    }

    if ( usr->field_13C2 & 8 )
    {
        if ( usr->field_0x13a8 & 2 )
        {
            usr->GFX_flags |= 2;
            yw->self_full->setYW_skyRender(1);
        }
        else
        {
            usr->GFX_flags &= ~2;
            yw->self_full->setYW_skyRender(0);
        }

    }

    if ( usr->field_13C2 & 0x800 )
    {
        if ( usr->field_0x13a8 & 4 )
        {
            usr->GFX_flags |= 4;
            yw->field_73CE |= 0x40;
            yw->win3d->setWDD_cursor(1);
        }
        else
        {
            usr->GFX_flags &= 0xFB;
            yw->field_73CE &= 0xBF;
            yw->win3d->setWDD_cursor(0);
        }

    }

    if ( usr->field_13C2 & 0x20 )
    {
        usr->enemyindicator = usr->field_13BE;

        if ( usr->enemyindicator )
            usr->p_ypaworld->field_73CE |= 0x20;
        else
            usr->p_ypaworld->field_73CE &= 0xDF;
    }

    if ( usr->field_13C2 & 0x40 )
    {
        usr->fxnumber = usr->field_0x13a4;
        yw->fxnumber = usr->fxnumber;
    }

    if ( usr->field_13C2 & 0x100 )
    {
        usr->snd__cdvolume = usr->field_0x13b8;
        int a1a = usr->field_0x13b8;

        sub_4448C0(&a1a);
    }

    if ( usr->field_13C2 & 0x80 )
    {
        usr->snd__volume = usr->field_0x13b4;
        milesEngine__setter(0x80004003, usr->snd__volume, 0);
    }

    if ( usr->field_13C2 & 1 )
    {
        if ( usr->game_default_res != yw->game_default_res && usr->game_default_res )
        {
            v3 = 1;
            resolution = yw->game_default_res;

            yw->game_default_res = usr->game_default_res;
        }
    }

    if ( usr->field_13C2 & 0x1000 )
    {
        if ( usr->field_139A )
        {
            if ( strcasecmp(usr->field_139A, usr->win3d_guid) )
            {
                strcpy(usr->win3d_name, usr->field_139E);

                strcpy(usr->win3d_guid, usr->field_139A);

                NC_STACK_windd *windd = dynamic_cast<NC_STACK_windd *>(yw->win3d);
                wdd_func324arg v37;

                v37.name = usr->win3d_name;
                v37.guid = usr->win3d_guid;
                v37.currr = 0;

                windd->windd_func325(&v37); //Save to file new resolution

                yw->game_default_res = 0x2801E0; //640 x 480
                resolution = 0x2801E0; //640 x 480
                v5 = 1;
                v40.make_changes = 1;
            }
        }
    }

    if ( usr->field_13C2 & 4 )
    {
        if ( usr->field_0x13a8 & 0x10 )
        {
            usr->GFX_flags |= 0x10;
            yw->win3d->setWDD_16bitTex(1);
        }
        else
        {
            usr->GFX_flags &= 0xEF;
            yw->win3d->setWDD_16bitTex(0);
        }

        resolution = yw->game_default_res;
        v5 = 1;
        v40.make_changes = 1;
    }

    if ( usr->field_13C2 & 0x400 )
    {
        if ( usr->field_0x13a8 & 8 )
        {
            usr->GFX_flags |= 8;
            yw->win3d->setWDD_drawPrim(1);
        }
        else
        {
            usr->GFX_flags &= 0xF7;
            yw->win3d->setWDD_drawPrim(0);
        }


        resolution = yw->game_default_res;
        v5 = 1;
        v40.make_changes = 1;
    }

    if ( (v3 && usr->p_ypaworld->one_game_res) || v5 )
    {
        if ( usr->p_ypaworld->one_game_res || !v3 )
            v40.resolution = usr->p_ypaworld->game_default_res;
        else
            v40.resolution = resolution;

        yw->self_full->ypaworld_func174(&v40);

        video_mode_node *nod = (video_mode_node *)usr->video_mode_list.head;
        int v24 = 0;

        while (nod->next)
        {
            if (yw->game_default_res == nod->sort_id)
            {
                usr->field_FBE = v24;

                button_71arg v36;
                v36.field_4 = nod->name;
                v36.butID = 1156;
                v36.field_8 = 0;
                usr->video_button->button_func71(&v36);

                break;
            }

            v24++;
            nod = (video_mode_node *)nod->next;
        }
    }

    usr->field_13C2 = 0;
    usr->field_46 = 1;

    int v42 = 2;
    usr->video_button->button_func68(&v42);

    if ( !(usr->video_listvw.cmd_flag & 0x20) )
        sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);

    if ( !(usr->d3d_listvw.cmd_flag & 0x20) )
        sub_4C31C0(usr->p_ypaworld, &usr->d3d_listvw);

    button_66arg v38;
    v38.field_4 = 2;
    v38.butID = 1156;

    usr->video_button->button_func73(&v38);

    v38.butID = 1172;
    usr->video_button->button_func73(&v38);

    v42 = 1;
    usr->titel_button->button_func68(&v42);
}


void sub_46DC1C(UserData *usr)
{
    yw_arg181_a v11;
    yw_arg181 v5;

    v5.value = &v11;
    v5.val_size = 20;
    v5.field_10 = 0;
    v5.field_14 = 2;
    v5.field_18 = 1;

    v11.fld_0 = 1000;
    v11.fld_3 = 0;
    v11.fld_4_1 = usr->field_1C86;
    v11.fld_4_2 = 0; // HACK

    usr->p_ypaworld->self_full->ypaworld_func181(&v5);

    windp_arg82 v6;
    v6.field_0 = usr->callSIGN;
    v6.field_4 = 1;
    v6.field_8 = 0;
    v6.field_C = 2;
    v6.field_10 = 1;

    usr->p_ypaworld->windp->windp_func82(&v6);

    usr->field_0x2fbc = 4;
    usr->field_0x2fc0 = usr->field_1C86;
    usr->network_listvw.scroll_pos = 0;
    usr->field_0x2fc4 = usr->field_1C86;

    int v12 = 1;
    usr->p_ypaworld->windp->windp_func84(&v12);

    sb_0x4deac0(usr);
}

int sub_4EDCC4(_NC_STACK_ypaworld *yw)
{
    return yw->field_2d90->field_40 != 8;
}

int sub_47B388(int a1, const char *a2)
{
    char buf[300];

    sprintf(buf, "save:%s/%d.sgm", a2, a1);

    FILE *fil = FOpen(buf, "r");
    if ( !fil )
        return 0;

    FClose(fil);

    return 1;
}

int ypaworld_func158__sub0__sub7(UserData *usr)
{
    char buf[300];

    sprintf(buf, "save:%s/sgisold.txt", usr->user_name);
    FILE *fl = FOpen(buf, "r");
    if ( !fl )
        return 0;

    FClose(fl);
    return 1;
}

void sub_4811E8(_NC_STACK_ypaworld *yw, int id)
{
    if ( id > yw->field_17c4 )
    {
        yw->field_17c8 = 0;
        yw->field_17c4 = id;
    }
}

void sub_4DE248(UserData *usr, int id)
{
    switch (id)
    {
    default:
        break;

    case 1001:
        sub_4811E8(usr->p_ypaworld, 0x7B);
        break;

    case 1003:
        sub_4811E8(usr->p_ypaworld, 0x78);
        break;

    case 1004:
        sub_4811E8(usr->p_ypaworld, 0x79);
        break;

    case 1005:
        sub_4811E8(usr->p_ypaworld, 0x7A);
        break;

    case 1007:
        sub_4811E8(usr->p_ypaworld, 0x83);
        break;

    case 1008:
        sub_4811E8(usr->p_ypaworld, 0x7C);
        break;

    case 1011:
        sub_4811E8(usr->p_ypaworld, 0x81);
        break;

    case 1013:
        sub_4811E8(usr->p_ypaworld, 0x80);
        break;

    case 1014:
        sub_4811E8(usr->p_ypaworld, 0x82);
        break;

    case 1015:
        sub_4811E8(usr->p_ypaworld, 0x7F);
        break;

    case 1016:
        sub_4811E8(usr->p_ypaworld, 0x88);
        break;

    case 1017:
    case 1052:
    case 1107:
    case 1167:
    case 1218:
        sub_4811E8(usr->p_ypaworld, 0x89);
        break;

    case 1018:
        sub_4811E8(usr->p_ypaworld, 0x6A);
        break;

    case 1019:
        if ( sub_4EDCC4(usr->p_ypaworld) )
        {
            if ( usr->p_ypaworld->field_2d90->field_40 == 9 )
                sub_4811E8(usr->p_ypaworld, 0x49);
            else
                sub_4811E8(usr->p_ypaworld, 0x8B);
        }
        else
        {
            sub_4811E8(usr->p_ypaworld, 0x8A);
        }
        break;

    case 1020:
        sub_4811E8(usr->p_ypaworld, 0x4A);
        break;

    case 1050:
        sub_4811E8(usr->p_ypaworld, 0xC4);
        break;

    case 1051:
        sub_4811E8(usr->p_ypaworld, 0xC1);
        break;

    case 1053:
        sub_4811E8(usr->p_ypaworld, 0xC3);
        break;

    case 1054:
        sub_4811E8(usr->p_ypaworld, 0xC2);
        break;

    case 1055:
        sub_4811E8(usr->p_ypaworld, 0xC5);
        break;

    case 1056:
        sub_4811E8(usr->p_ypaworld, 0xC6);
        break;

    case 1061:
        sub_4811E8(usr->p_ypaworld, 0x4D);
        break;

    case 1101:
        sub_4811E8(usr->p_ypaworld, 0x76);
        break;

    case 1102:
        sub_4811E8(usr->p_ypaworld, 0xA8);
        break;

    case 1103:
        sub_4811E8(usr->p_ypaworld, 0xA9);
        break;

    case 1104:
        sub_4811E8(usr->p_ypaworld, 0x77);
        break;

    case 1105:
        switch ( usr->field_0x1744 )
        {
        case 1:
            sub_4811E8(usr->p_ypaworld, 0xAB);
            break;
        case 2:
            sub_4811E8(usr->p_ypaworld, 0xAA);
            break;
        case 3:
            sub_4811E8(usr->p_ypaworld, 0xAD);
            break;
        case 4:
            sub_4811E8(usr->p_ypaworld, 0xAC);
            break;
        default:
            break;
        }
        break;

    case 1106:
        switch ( usr->field_0x1744 )
        {
        case 0:
            sub_4811E8(usr->p_ypaworld, 0xAE);
            break;
        case 1:
            sub_4811E8(usr->p_ypaworld, 0xB0);
            break;
        case 2:
            sub_4811E8(usr->p_ypaworld, 0xAF);
            break;
        case 3:
            sub_4811E8(usr->p_ypaworld, 0xB2);
            break;
        case 4:
            sub_4811E8(usr->p_ypaworld, 0xB1);
            break;
        default:
            break;
        }
        break;
    case 1150:
        sub_4811E8(usr->p_ypaworld, 0x4E);
        break;

    case 1151:
        sub_4811E8(usr->p_ypaworld, 0xB7);
        break;

    case 1152:
        sub_4811E8(usr->p_ypaworld, 0xBE);
        break;

    case 1154:
        sub_4811E8(usr->p_ypaworld, 0xBF);
        break;

    case 1156:
        sub_4811E8(usr->p_ypaworld, 0xB5);
        break;

    case 1157:
        sub_4811E8(usr->p_ypaworld, 0xB8);
        break;

    case 1159:
        sub_4811E8(usr->p_ypaworld, 0xC0);
        break;

    case 1160:
        sub_4811E8(usr->p_ypaworld, 0xB9);
        break;

    case 1161:
        sub_4811E8(usr->p_ypaworld, 0xB4);
        break;

    case 1162:
        sub_4811E8(usr->p_ypaworld, 0xB3);
        break;

    case 1163:
        sub_4811E8(usr->p_ypaworld, 0xBD);
        break;

    case 1164:
        sub_4811E8(usr->p_ypaworld, 0xBB);
        break;

    case 1165:
        sub_4811E8(usr->p_ypaworld, 0xBC);
        break;

    case 1166:
        sub_4811E8(usr->p_ypaworld, 0x4F);
        break;

    case 1172:
        sub_4811E8(usr->p_ypaworld, 0x4B);
        break;

    case 1201:
        switch ( usr->field_1C3A )
        {
        case 0:
            sub_4811E8(usr->p_ypaworld, 0x8E);
            break;
        case 1:
            sub_4811E8(usr->p_ypaworld, 0x91);
            break;
        case 2:
            sub_4811E8(usr->p_ypaworld, 0x8F);
            break;
        case 3:
            sub_4811E8(usr->p_ypaworld, 0x90);
            break;
        case 4:
            sub_4811E8(usr->p_ypaworld, 0x92);
            break;
        default:
            break;
        }
        break;

    case 1202:
        if ( usr->field_1C3A == 1 )
        {
            sub_4811E8(usr->p_ypaworld, 0x6E);
        }
        else if ( usr->field_1C3A == 4 )
        {
            sub_4811E8(usr->p_ypaworld, 0x6F);
        }
        break;

    case 1203:
        sub_4811E8(usr->p_ypaworld, 0x8C);
        break;

    case 1205:
        switch ( usr->field_1C3A )
        {
        case 1:
            sub_4811E8(usr->p_ypaworld, 0x94);
            break;
        case 2:
            sub_4811E8(usr->p_ypaworld, 0x93);
            break;
        case 3:
        case 4:
            sub_4811E8(usr->p_ypaworld, 0x95);
            break;
        default:
            break;
        }
        break;

    case 1206:
        sub_4811E8(usr->p_ypaworld, 0x70);
        break;

    case 1207:
        sub_4811E8(usr->p_ypaworld, 0x71);
        break;

    case 1208:
        sub_4811E8(usr->p_ypaworld, 0x72);
        break;

    case 1209:
        sub_4811E8(usr->p_ypaworld, 0x73);
        break;

    case 1219:
        if ( usr->field_1CE9 )
            sub_4811E8(usr->p_ypaworld, 0x75);
        else
            sub_4811E8(usr->p_ypaworld, 0x74);
        break;

    case 1225:
        sub_4811E8(usr->p_ypaworld, 0x4C);
        break;

    case 1250:
        sub_4811E8(usr->p_ypaworld, 0x6C);
        break;

    case 1251:
        sub_4811E8(usr->p_ypaworld, 0x6D);
        break;

    case 1252:
        sub_4811E8(usr->p_ypaworld, 0x89);
        break;
    }
}

// Go to options menu
void ypaworld_func158__sub0__sub2(UserData *usr)
{
    int v5 = 2;
    usr->titel_button->button_func68(&v5);

    v5 = 1;
    usr->video_button->button_func68(&v5);

    usr->field_46 = 3;

    if ( !(usr->video_listvw.cmd_flag & 0x20) )
    {
        sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);
        sub_4C31EC(usr->p_ypaworld, &usr->video_listvw);
    }

    usr->video_listvw.field_1DE = usr->field_FBE;
}

void sub_46C3E4(UserData *usr)
{
    int v5 = 2;
    usr->titel_button->button_func68(&v5);

    v5 = 1;
    usr->disk_button->button_func68(&v5);

    usr->field_46 = 9;

    profilesNode *v4 = (profilesNode *)usr->files_list.head;
    while (v4->next)
    {
        if ( !strcasecmp(v4->profile_subdir, usr->user_name))
        {
            v4->pStatus_3 = usr->p_ypaworld->playerstatus[1].p3;
            break;
        }
        v4 = (profilesNode *)v4->next;
    }
    sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);
    sub_4C31EC(usr->p_ypaworld, &usr->disk_listvw);
}

void ypaworld_func158__sub0__sub1(UserData *usr)
{
    int v5 = 2;
    usr->titel_button->button_func68(&v5);

    v5 = 1;
    usr->button_input_button->button_func68(&v5);

    usr->field_46 = 2;
    sub_4C31C0(usr->p_ypaworld, &usr->input_listview);
    sub_4C31EC(usr->p_ypaworld, &usr->input_listview);
}

void sub_4D9550(_NC_STACK_ypaworld *yw, int arg)
{
    char a1a[260];

    UserData *usr = yw->GameShell;

    if ( usr->default_lang_dll )
        sprintf(a1a, "sounds/speech/%s/9%d.wav", usr->default_lang_dll->langDllName, arg);
    else
        sprintf(a1a, "sounds/speech/language/9%d.wav", arg);



    if ( usr->field_ADA )
    {
        sub_424000(&usr->field_782, 0);
        sub_423DD8(&usr->field_782);
        delete_class_obj(usr->field_ADA);
        usr->field_ADA = 0;
    }

    char rsr[256];
    strcpy(rsr, get_prefix_replacement("rsrc"));

    set_prefix_replacement("rsrc", "data:");

    usr->field_ADA = (NC_STACK_wav *)init_get_class("wav.class", 0x80001000, a1a, 0);
    if ( usr->field_ADA )
    {
        sub_423DB0(&usr->field_782);

        usr->field_782.field_C = 0;
        usr->field_782.field_10 = 0;
        usr->field_782.field_14 = 0;
        usr->field_782.field_0.sx = 0;
        usr->field_782.field_0.sy = 0;
        usr->field_782.field_0.sz = 0;
        usr->field_782.samples_data[0].volume = 500;
        usr->field_782.samples_data[0].pitch = 0;

        usr->field_782.samples_data[0].psampl = usr->field_ADA->getSMPL_pSample();
        sub_423F74(&usr->field_782, 0);
    }

    set_prefix_replacement("rsrc", rsr);
}

void sub_4D0C24(_NC_STACK_ypaworld *yw, const char *a1, const char *a2)
{
    UserData *usr = yw->GameShell;

    if ( strcasecmp(a1, usr->field_1CF9[32]) )
    {
        if ( usr->field_1CF7 >= 31 )
        {
            usr->field_1CF7 = 31;

            for (int i = 0; i < 31; i++ )
                strcpy(usr->field_1CF9[i], usr->field_1CF9[i + 1]);
        }

        memset(usr->field_1CF9[usr->field_1CF7], 0, 64);
        sprintf(usr->field_1CF9[usr->field_1CF7], "> %s:", a1);
        memset(usr->field_1CF9[32], 0, 64);
        strncpy(usr->field_1CF9[32], a1, 63);

        usr->field_1CF7++;
    }

    if ( usr->field_1CF7 >= 31 )
    {
        usr->field_1CF7 = 31;

        for (int i = 0; i < 31; i++ )
            strcpy(usr->field_1CF9[i], usr->field_1CF9[i + 1]);
    }

    memset(usr->field_1CF9[usr->field_1CF7], 0, 64);
    strncpy(usr->field_1CF9[usr->field_1CF7], a2, 63);

    usr->field_1CF7++;

    if ( usr->field_1C3A == 4 )
    {
        int v22 = usr->field_1CF7 - 6;

        if ( v22 < 0 )
            v22 = 0;

        yw->GameShell->network_listvw.scroll_pos = v22;


        yw->GameShell->network_listvw.elements_for_scroll_size = yw->GameShell->field_1CF7;

        int v24;

        if ( usr->network_listvw.elements_for_scroll_size >= 6 )
            v24 = 6;
        else
            v24 = usr->network_listvw.elements_for_scroll_size;

        yw->GameShell->network_listvw.element_count = v24;
    }
}


void sub_46D1E8(UserData *usr)
{
    sub_4F0FFC(usr->p_ypaworld);
    sub_4EE04C(usr->p_ypaworld);

    int v4 = 2;
    usr->sub_bar_button->button_func68(&v4);

    v4 = 1;
    usr->titel_button->button_func68(&v4);

    usr->field_46 = 1;
    usr->field_4E = 0;
}

void ypaworld_func158__sub0__sub3(UserData *usr)
{
    int v7 = 2;
    usr->titel_button->button_func68(&v7);

    v7 = 1;
    usr->locale_button->button_func68(&v7);

    usr->field_46 = 7;
    sub_4C31C0(usr->p_ypaworld, &usr->local_listvw);
    sub_4C31EC(usr->p_ypaworld, &usr->local_listvw);

    langDll_node *v5 = (langDll_node *)usr->lang_dlls.head;
    int i = 0;

    while ( v5 != usr->default_lang_dll )
    {
        i++;
        v5 = (langDll_node *)v5->next;
    }

    usr->local_listvw.field_1DE = i;
}

void sub_4EDCD8(_NC_STACK_ypaworld *yw)
{
    yw->brief.field_2E68 = 2;
}

void sub_46D9E0(UserData *usr, int a2, const char *txt1, const char *txt2, int a5)
{
    usr->field_0x2fb4 = a2;

    button_66arg v12;
    v12.butID = 1300;
    usr->confirm_button->button_func66(&v12);


    button_arg76 v10;

    if ( a5 )
    {
        v10.butID = 1300;
        v10.xpos = usr->p_ypaworld->screen_width * 0.4375;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        usr->confirm_button->button_func76(&v10);
    }
    else
    {
        v12.butID = 1301;
        usr->confirm_button->button_func66(&v12);

        v10.butID = 1300;
        v10.xpos = usr->p_ypaworld->screen_width * 0.25;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        usr->confirm_button->button_func76(&v10);

        v10.butID = 1301;
        v10.xpos = usr->p_ypaworld->screen_width * 0.625;
        usr->confirm_button->button_func76(&v10);
    }

    button_71arg v9;

    v9.field_4 = txt1;
    v9.field_8 = 0;
    v9.butID = 1302;
    usr->confirm_button->button_func71(&v9);

    if ( txt2 )
        v9.field_4 = txt2;
    else
        v9.field_4 = " ";

    v9.field_8 = 0;
    v9.butID = 1303;
    usr->confirm_button->button_func71(&v9);

    int v13 = 1;
    usr->confirm_button->button_func68(&v13);
}

void ypaworld_func158__sub0__sub9(_NC_STACK_ypaworld *yw)
{
    yw->brief.field_2E68 = 1;
}

void ypaworld_func158__sub0__sub10(_NC_STACK_ypaworld *yw)
{
    yw->brief.field_2E6C = 1;
}

void ypaworld_func158__sub0__sub12(_NC_STACK_ypaworld *yw)
{
    yw->brief.field_2E6C = 3;
}

void ypaworld_func158__sub0__sub11(_NC_STACK_ypaworld *yw)
{
    yw->brief.field_2E6C = 2;
}


void sb_0x46a8c0__sub0(UserData *usr)
{
    for (int i = 1; i < 46; i++)
    {
        usr->keyConfig[i].KeyCode = usr->keyConfig[i].field_8;
        usr->keyConfig[i].slider_neg = usr->keyConfig[i].field_A;
    }
}

void sb_0x46a8c0(UserData *usr)
{
    usr->field_46 = 1;
    sb_0x46a8c0__sub0(usr);

    button_66arg v6;
    v6.butID = 1003;
    v6.field_4 = 2;
    usr->sub_bar_button->button_func73(&v6);

    usr->field_D5E = 0;

    v6.butID = 1050;
    v6.field_4 = (usr->inp_joystick == 0) + 1;
    usr->button_input_button->button_func73(&v6);

    v6.field_4 = (usr->inp_altjoystick == 0) + 1;
    v6.butID = 1061;
    usr->button_input_button->button_func73(&v6);

    v6.butID = 1055;
    v6.field_4 = ((usr->p_ypaworld->field_73CE & 8) != 0) + 1;
    usr->button_input_button->button_func73(&v6);

    int v7 = 2;
    usr->button_input_button->button_func68(&v7);

    sub_4C31C0(usr->p_ypaworld, &usr->input_listview);

    v7 = 1;
    usr->titel_button->button_func68(&v7);
}

void sub_457BC0(UserData *usr)
{
    for(int i = 1; i < 46; i++)
    {
        usr->keyConfig[i].field_8 = usr->keyConfig[i].KeyCode;
        usr->keyConfig[i].field_A = usr->keyConfig[i].slider_neg;
    }
}

int ypaworld_func158__sub0__sub0__sub0(UserData *usr)
{
    char a1a[300];
    sprintf(a1a, "data:settings/%s/input.def", usr->p_ypaworld->lang_name);

    scrCallBack a3a;
    a3a.func = parseSaveInput;
    a3a.world = (_NC_STACK_ypaworld *)usr->p_ypaworld->self_full;
    a3a.dataForStore = usr;

    return def_parseFile(a1a, 1, &a3a, 2);
}

void ypaworld_func158__sub0__sub0(UserData *usr)
{
    if ( ypaworld_func158__sub0__sub0__sub0(usr) )
    {
        for (int i = 1; i < 46; i++)
        {
            usr->keyConfig[i].field_C = usr->keyConfig[i].KeyCode;
            usr->keyConfig[i].field_E = usr->keyConfig[i].slider_neg;
        }
    }
    for (int i = 1; i < 46; i++)
    {
        usr->keyConfig[i].KeyCode = usr->keyConfig[i].field_C;
        usr->keyConfig[i].slider_neg = usr->keyConfig[i].field_E;
    }
}

void sub_46C5F0(UserData *usr, int a2)
{
    if ( usr->video_listvw.field_1DE != usr->field_FBE && !a2 )
    {

        usr->field_FBE = usr->video_listvw.field_1DE;

        video_mode_node *v6 = (video_mode_node *)usr->video_mode_list.head;

        for (int i = 0; i < usr->video_listvw.field_1DE; i++)
        {
            v6 = (video_mode_node *)v6->next;
        }

        usr->game_default_res = v6->sort_id;

        button_71arg v7;
        v7.butID = 1156;
        v7.field_4 = v6->name;
        v7.field_8 = 0;

        usr->video_button->button_func71(&v7);
    }
}

//Options Cancel
void sub_46A3C0(UserData *usr)
{
    usr->field_13C2 = 0;
    usr->field_46 = 1;

    video_mode_node *v2 = (video_mode_node *)usr->video_mode_list.head;

    int i = 0;
    while( v2->sort_id != usr->p_ypaworld->game_default_res )
    {
        i++;
        v2 = (video_mode_node *)v2->next;
    }

    usr->video_listvw.field_1DE = i;
    usr->field_FBE = i;
    usr->game_default_res = usr->p_ypaworld->game_default_res;

    button_71arg v9;
    v9.field_4 = v2->name;
    v9.field_8 = 0;
    v9.butID = 1156;

    usr->video_button->button_func71(&v9);

    v9.butID = 1172;
    v9.field_8 = 0;
    v9.field_4 = usr->win3d_name;
    usr->video_button->button_func71(&v9);

    usr->field_139A = usr->win3d_guid;
    usr->field_139E = usr->win3d_name;

    button_66arg v10;
    v10.butID = 1151;
    v10.field_4 = ((usr->snd__flags2 & 1) == 0) + 1;
    usr->video_button->button_func73(&v10);

    v10.field_4 = ((usr->snd__flags2 & 0x10) == 0) + 1;
    v10.butID = 1164;
    usr->video_button->button_func73(&v10);

    v10.butID = 1157;
    v10.field_4 = ((usr->GFX_flags & 1) == 0) + 1;
    usr->video_button->button_func73(&v10);

    v10.field_4 = ((usr->GFX_flags & 2) == 0) + 1;
    v10.butID = 1160;
    usr->video_button->button_func73(&v10);

    v10.butID = 1150;
    v10.field_4 = ((usr->GFX_flags & 0x10) == 0) + 1;
    usr->video_button->button_func73(&v10);

    v10.butID = 1166;
    v10.field_4 = ((usr->GFX_flags & 8) == 0) + 1;
    usr->video_button->button_func73(&v10);

    v10.butID = 1165;
    v10.field_4 = ((usr->GFX_flags & 4) == 0) + 1;
    usr->video_button->button_func73(&v10);

    v10.field_4 = (usr->enemyindicator == 0) + 1;
    v10.butID = 1163;
    usr->video_button->button_func73(&v10);

    int v12 = 1159;

    button_str2_t2 *tmp = (button_str2_t2 *)usr->video_button->button_func74(&v12);
    tmp->field_0 = usr->fxnumber;

    usr->video_button->button_func75(&v12);

    v12 = 1152;

    tmp = (button_str2_t2 *)usr->video_button->button_func74(&v12);
    tmp->field_0 = usr->snd__volume;

    usr->video_button->button_func75(&v12);

    v12 = 1154;

    tmp = (button_str2_t2 *)usr->video_button->button_func74(&v12);
    tmp->field_0 = usr->snd__cdvolume;

    usr->video_button->button_func75(&v12);

    int v11 = 2;
    usr->video_button->button_func68(&v11);

    if ( !(usr->video_listvw.cmd_flag & 0x20) )
        sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);

    if ( !(usr->d3d_listvw.cmd_flag & 0x20) )
        sub_4C31C0(usr->p_ypaworld, &usr->d3d_listvw);

    v11 = 1;
    usr->titel_button->button_func68(&v11);

    v10.field_4 = 2;
    v10.butID = 1156;
    usr->video_button->button_func73(&v10);

    v10.butID = 1172;
    usr->video_button->button_func73(&v10);
}

void  ypaworld_func158__sub0__sub5(UserData *usr, int a2)
{
    int v4 = 0;

    wdd_func324arg a1;
    a1.guid = 0;
    a1.currr = 0;
    a1.name = (const char *)-1;

    const char *v2;
    const char *v12;

    NC_STACK_windd *windd = dynamic_cast<NC_STACK_windd *>(usr->p_ypaworld->win3d);

    while ( a1.name )
    {
        windd->windd_func324(&a1);
        if ( a1.name )
        {
            if ( v4 == usr->d3d_listvw.field_1DE )
            {
                if ( !strcmp(a1.name, "software") )
                    v2 = get_lang_string(usr->p_ypaworld->string_pointers_p2, 2472, "2472 = Software");
                else
                    v2 = a1.name;

                v12 = a1.guid;
            }
        }
        v4++;
    }

    if ( !a2 )
    {
        usr->field_139E = v2;
        usr->field_139A = v12;

        button_71arg v7;
        v7.butID = 1172;
        v7.field_4 = v2;
        v7.field_8 = 0;

        usr->video_button->button_func71(&v7);
    }
}

void sub_46C914(UserData *usr)
{
    if ( usr->field_1612 )
    {
        profilesNode *node = (profilesNode *)usr->files_list.head;

        for (int i = 0; i < usr->field_1612 - 1; i++) // check usr->field_1612 - 1
        {
            node = (profilesNode *)node->next;
        }

        char a1a[300];
        sprintf(a1a, "%s/user.txt", node->profile_subdir);

        yw_arg172 arg172;

        arg172.usertxt = a1a;
        arg172.field_4 = node->profile_subdir;
        arg172.field_8 = 255;
        arg172.usr = usr;
        arg172.field_10 = 1;

        usr->p_ypaworld->self_full->ypaworld_func172(&arg172);

        strcpy(usr->user_name, node->profile_subdir);
        strcpy(usr->usernamedir, node->profile_subdir);


        usr->field_0x1744 = 0;
        usr->field_3426 = 0;

        int v16 = 2;
        usr->disk_button->button_func68(&v16);

        sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);

        usr->field_46 = 5;

        v16 = 1;
        usr->sub_bar_button->button_func68(&v16);
    }
}

void sub_46D0F8(const char *path)
{
    char a1a[200];

    ncDir *v4 = OpenDir(path);
    if ( v4 )
    {
        dirEntry v5;

        while ( ReadDir(v4, &v5) )
        {
            if ( !(v5.field_0 & 1) || (strcmp(v5.e_name, ".") && strcmp(v5.e_name, "..")) )
            {
                sprintf(a1a, "%s/%s", path, v5.e_name);
                delete_file(a1a);
            }
        }
        CloseDir(v4);
    }
}

void sub_46C748(UserData *usr)
{
    if ( usr->field_1612 )
    {
        if ( strcasecmp(usr->usernamedir, usr->user_name) )
        {

            profilesNode *node = (profilesNode *)usr->files_list.head;

            for (int i = 0; i < usr->field_1612 - 1; i++) // check usr->field_1612 - 1
                node = (profilesNode *)node->next;

            profilesNode *v4 = (profilesNode *)node->next;

            int v5;

            if ( v4->next )
            {
                v5 = 1;
            }
            else
            {
                v4 = (profilesNode *)node->prev;
                v5 = 0;
            }

            char a1[300];
            sprintf(a1, "save:%s", node->profile_subdir);

            sub_46D0F8(a1);

            removeDirectory(a1);

            Remove(node);

            usr->disk_listvw.elements_for_scroll_size--;
            if ( usr->files_list.tail == usr->files_list.head )
            {
                usr->field_1612 = 0;
                strcpy(usr->usernamedir, "NEWUSER");
            }
            else
            {
                if ( !v5 )
                    usr->field_1612--;

                strcpy(usr->usernamedir, v4->profile_subdir);
            }

            usr->usernamedir_len = strlen(usr->usernamedir);

            if ( usr->field_1612 )
                sub_4DDFA4(&usr->disk_listvw, usr->field_1612 - 1);

            nc_FreeMem(node);

            usr->field_0x1744 = 0;

            int v14 = 2;
            usr->disk_button->button_func68(&v14);

            sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);

            if ( usr->field_0x1760 )
            {
                usr->field_46 = 5;
                v14 = 1;
                usr->sub_bar_button->button_func68(&v14);
            }
            else
            {
                usr->field_46 = 1;
                v14 = 1;
                usr->titel_button->button_func68(&v14);
            }
        }
    }
}

void sub_46B0E0(UserData *usr)
{
    langDll_node *node = (langDll_node *)usr->lang_dlls.head;

    for (int v5 = 0; v5 < usr->local_listvw.field_1DE; v5++)
    {
        node = (langDll_node *)node->next;
    }

    usr->prev_lang = node;

    if ( node != usr->default_lang_dll )
    {
        if ( node )
        {
            usr->default_lang_dll = node;
            usr->p_ypaworld->self_full->ypaworld_func175( usr );
        }
    }

    usr->field_19CA = 0;
    usr->field_46 = 1;
    usr->field_19CA = 0;
    usr->prev_lang = usr->default_lang_dll;

    int v7 = 2;
    usr->locale_button->button_func68(&v7);

    sub_4C31C0(usr->p_ypaworld, &usr->local_listvw);

    v7 = 1;
    usr->titel_button->button_func68(&v7);
}

void sub_46AA0C(UserData *usr)
{
    usr->field_46 = 1;
    usr->field_19CA = 0;
    usr->prev_lang = usr->default_lang_dll;

    int v5 = 2;
    usr->locale_button->button_func68(&v5);

    sub_4C31C0(usr->p_ypaworld, &usr->local_listvw);

    v5 = 1;
    usr->titel_button->button_func68(&v5);
}


int sub_449678(_NC_STACK_ypaworld *yw, struC5 *struc, int kkode)
{
    return struc->downed_key == kkode && ( (struc->winp131arg.flag & 0x100) || yw->easy_cheat_keys );
}

void ypaworld_func158__sub0__sub4(UserData *usr)
{
    int v4 = 2;
    usr->titel_button->button_func68(&v4);

    v4 = 1;
    usr->about_button->button_func68(&v4);

    usr->field_46 = 8;
}


int ypaworld_func158__sub0__sub6(char a1)
{
    if (a1 == '/' || a1 == '\\' || a1 == ':' || a1 == '*' || a1 == '?' || a1 == '"' || a1 == '<' || a1 == '>' || a1 == '|')
        return 0;

    return 1;
}







void ypaworld_func158__sub0(_NC_STACK_ypaworld *yw, UserData *usr)
{
    int v3 = 0;
    char v306[300];

    if ( usr->field_3A->winp131arg.flag & 0x10 )
        sub_423F74(&usr->samples1_info, 3);

    if ( usr->field_1C3A )
        sb_0x4c9f14(usr->p_ypaworld);

    NC_STACK_windd *windd = dynamic_cast<NC_STACK_windd *>(usr->p_ypaworld->win3d);

    if ( usr->field_1C3A == 1 )
    {
        if ( usr->p_ypaworld->windp->windp_func90(NULL) == 4 )
        {
            if ( usr->field_0x1cdc )
            {
                if ( usr->field_3A->downed_key == VK_SPACE )
                {
                    windd->windd_func320(NULL);
                    yw->windp->windp_func68(NULL);
                    windd->windd_func321(NULL);
                }
            }
        }
        else if ( yw->windp->windp_func90(NULL) != 3 || usr->field_3A->downed_key == VK_SPACE )
        {
            yw->windp->windp_func68(NULL);
        }
    }

    button_66arg v408;
    button_66arg v410;

    v410.field_4 = 0;
    v410.butID = 1015;
    usr->sub_bar_button->button_func67(&v410);
    v410.butID = 1011;
    usr->sub_bar_button->button_func67(&v410);
    v410.butID = 1019;
    usr->sub_bar_button->button_func67(&v410);
    v410.butID = 1014;
    usr->sub_bar_button->button_func67(&v410);
    v410.butID = 1013;
    usr->sub_bar_button->button_func67(&v410);
    v410.butID = 1020;
    usr->sub_bar_button->button_func67(&v410);

    v410.butID = 1003;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1004;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1001;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1008;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1007;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1018;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1017;
    usr->titel_button->button_func67(&v410);
    v410.butID = 1016;
    usr->titel_button->button_func67(&v410);

    button_arg76 v393;
    v393.ypos = -1;
    v393.width = -1;
    //v394 = -1;
    v393.butID = 1014;
    v393.xpos = 0;
    usr->sub_bar_button->button_func76(&v393);

    v393.butID = 1019;
    v393.xpos = yw->screen_width - dword_5A50B6_h;
    usr->sub_bar_button->button_func76(&v393);

    v393.butID = 1011;
    v393.xpos = word_5A50C0 + dword_5A50B6_h;
    usr->sub_bar_button->button_func76(&v393);

    button_71arg v395;
    v395.butID = 1019;
    v395.field_4 = get_lang_string(ypaworld__string_pointers, 644, "GO BACK");
    v395.field_8 = NULL;
    usr->sub_bar_button->button_func71(&v395);

    if ( sub_4EDCC4(yw) )
    {
        if ( yw->field_2d90->field_40 != 9 && !usr->field_0xc )
        {
            v410.butID = 1014;
            usr->sub_bar_button->button_func66(&v410);

            v393.butID = 1014;
            v393.xpos = yw->screen_width - dword_5A50B6_h;
            usr->sub_bar_button->button_func76(&v393);

            v393.butID = 1019;
            v393.xpos = 0;
            usr->sub_bar_button->button_func76(&v393);

            v395.field_4 = get_lang_string(ypaworld__string_pointers, 2438, "2438 == BACK");
            v395.butID = 1019;
            v395.field_8 = NULL;
            usr->sub_bar_button->button_func71(&v395);
        }

        if ( yw->field_2d90->field_40 == 9 )
        {
            v410.butID = 1011;
            usr->sub_bar_button->button_func66(&v410);

            v393.butID = 1011;
            v393.xpos = 0;
            usr->sub_bar_button->button_func76(&v393);

            v395.field_4 = get_lang_string(ypaworld__string_pointers, 2420, "CONTINUE");
            v395.butID = 1019;
            v395.field_8 = NULL;
            usr->sub_bar_button->button_func71(&v395);
        }

        v410.butID = 1019;
        usr->sub_bar_button->button_func66(&v410);

        int v416 = 2;
        usr->button_input_button->button_func68(&v416);
        usr->video_button->button_func68(&v416);
        usr->disk_button->button_func68(&v416);
        usr->locale_button->button_func68(&v416);
        usr->network_button->button_func68(&v416);

        sub_4C31C0(usr->p_ypaworld, &usr->input_listview);
        sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);
        sub_4C31C0(usr->p_ypaworld, &usr->disk_listvw);
        sub_4C31C0(usr->p_ypaworld, &usr->local_listvw);
        sub_4C31C0(usr->p_ypaworld, &usr->network_listvw);
    }
    else if ( usr->field_46 == 1 )
    {
        v410.butID = 1003;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1004;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1001;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1008;
        if ( usr->lang_dlls_count > 1 )
            usr->titel_button->button_func66(&v410);
        else
            usr->titel_button->button_func67(&v410);

        v410.butID = 1007;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1017;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1018;
        usr->titel_button->button_func66(&v410);

        v410.butID = 1016;
        usr->titel_button->button_func66(&v410);
    }
    else if ( usr->field_46 == 5 || usr->field_46 == 4 )
    {
        if ( !usr->field_3426 )
        {
            if ( sub_47B388(0, usr->user_name) )
                usr->field_3426 = 1;
            else
                usr->field_3426 = 2;
        }

        if ( usr->field_3426 == 1 )
        {
            v410.butID = 1015;
            usr->sub_bar_button->button_func66(&v410);
        }

        v410.butID = 1019;
        usr->sub_bar_button->button_func66(&v410);

        v410.butID = 1020;
        usr->sub_bar_button->button_func66(&v410);
    }

    if ( usr->field_0x2fb4 )
        v3 = 1;

    DWORD v6 = usr->confirm_button->button_func69(usr->field_3A);
    int v6_l = v6 & 0xFFFF;
    int v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 1350 ) // OK
        {
            switch ( usr->field_0x2fb4 )
            {
            case 1:
            {
                usr->field_AE2 = 0;
                usr->field_3426 = 0;
                usr->p_ypaworld->field_757E = 0;

                int v418 = 2;
                usr->sub_bar_button->button_func68(&v418);

                usr->field_0x2fbc = 3;
                usr->field_0x2fc0 = 0;
            }
            break;

            case 2:
                sub_46DC1C(usr);
                break;
            case 3:
                sb_0x46ca74(usr);
                break;
            case 4:
                sub_46D960(usr);
                break;
            case 5:
                sb_0x46aa8c(usr);
                break;
            case 6:
                sb_0x46cdf8(usr);
                break;
            default:
                break;
            }
            sub_46D960(usr);
        }
        else if ( v6_l == 1351 ) // Cancel
        {
            if ( usr->field_0x2fb4 == 3 || usr->field_0x2fb4 == 6 )
                usr->field_0x1744 = 0;

            sub_46D960(usr);
        }
    }

    if ( usr->field_0x2fb4 )
    {
        if ( usr->field_3A->dword8 == (0x80 | 0x18) )
        {
            if ( usr->field_0x2fb4 == 3 || usr->field_0x2fb4 == 6 )
                usr->field_0x1744 = 0;
            sub_46D960(usr);
        }
        if ( usr->field_3A->downed_key == VK_RETURN )
        {
            switch ( usr->field_0x2fb4 )
            {
            case 1:
            {
                usr->field_AE2 = 0;
                usr->field_3426 = 0;
                usr->p_ypaworld->field_757E = 0;
                int v419 = 2;
                usr->sub_bar_button->button_func68(&v419);
                usr->field_0x2fbc = 3;
                usr->field_0x2fc0 = 0;
            }
            break;
            case 2:
                sub_46DC1C(usr);
                break;
            case 3:
                sb_0x46ca74(usr);
                break;
            case 4:
                sub_46D960(usr);
                break;
            case 5:
                sb_0x46aa8c(usr);
                break;
            case 6:
                sb_0x46cdf8(usr);
                break;
            default:
                break;
            }
            sub_46D960(usr);
        }
    }

    if ( v3 )
    {
        usr->field_3A->winp131arg.flag = 0;
        usr->field_3A->downed_key = 0;
        usr->field_3A->downed_key_2 = 0;
        usr->field_3A->chr = 0;
        usr->field_3A->dword8 = 0;
    }

    if ( usr->field_46 == 1 && usr->field_3A->dword8 == (0x80 | 0x2B) )
        yw->field_81AF = get_lang_string(ypaworld__string_pointers, 750, "help\\start.html");

    v6 = usr->titel_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 1001 )
        {
            sub_46C3E4(usr);
            usr->field_0x1760 = 0;
        }
        else if ( v6_l == 1005 ) // Options button
        {
            ypaworld_func158__sub0__sub2(usr);
        }
        else if ( v6_l == 1007 )
        {
            ypaworld_func158__sub0__sub1(usr);
        }
        else if ( v6_l == 1011 )
        {
            ypaworld_func158__sub0__sub3(usr);
        }
        else if ( v6_l == 1013 )
        {
            int v416 = 2;
            usr->titel_button->button_func68(&v416);

            usr->field_0x2fbc = 1;
            sub_423F74(&usr->samples1_info, 4);
        }
        else if ( v6_l == 1022 )
        {
            sub_46C524(usr);
        }
        else if ( v6_l == 1024 )
        {
            int v416 = 1;
            usr->sub_bar_button->button_func68(&v416);

            v416 = 2;
            usr->titel_button->button_func68(&v416);

            usr->field_46 = 5;
        }
        else if ( v6_l == 1025 )
        {
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 750, "help\\start.html");
        }
    }

    if ( (usr->field_46 == 5 || usr->field_46 == 4) && usr->field_3A->dword8 == (0x80 | 0x18) )
    {
        if ( sub_4EDCC4(yw) )
        {
            sub_4EDCD8(yw);
            if ( usr->p_ypaworld->snd__cdsound & 1 )
            {
                CDAUDIO_t arg;
                arg.command = 1;
                sub_4444D4(&arg);

                if ( usr->shelltrack )
                {
                    arg.command = 7;
                    arg.track_id = usr->shelltrack;
                    arg.field_8 = usr->shelltrack__adv.field_0;
                    arg.field_C = usr->shelltrack__adv.field_4;
                    sub_4444D4(&arg);

                    arg.command = 2;
                    sub_4444D4(&arg);
                }
            }
            if ( usr->field_4E )
                sub_46D1E8(usr);
        }
        else
        {
            int v416 = 2;
            usr->sub_bar_button->button_func68(&v416);

            v416 = 1;
            usr->titel_button->button_func68(&v416);

            usr->field_46 = 1;
        }
    }

    v6 = usr->sub_bar_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);
        switch ( v6_l )
        {
        case 1013:
            if ( sub_4EDCC4(yw) )
            {
                sub_4EDCD8(yw);
                if ( usr->p_ypaworld->snd__cdsound & 1 )
                {
                    CDAUDIO_t arg;
                    arg.command = 1;
                    sub_4444D4(&arg);

                    if ( usr->shelltrack )
                    {
                        arg.command = 7;
                        arg.track_id = usr->shelltrack;
                        arg.field_8 = usr->shelltrack__adv.field_0;
                        arg.field_C = usr->shelltrack__adv.field_4;
                        sub_4444D4(&arg);

                        arg.command = 2;
                        sub_4444D4(&arg);
                    }
                }
                if ( usr->field_4E )
                    sub_46D1E8(usr);
            }
            else
            {
                int v416 = 2;
                usr->sub_bar_button->button_func68(&v416);

                v416 = 1;
                usr->titel_button->button_func68(&v416);

                usr->field_46 = 1;
            }
            break;

        case 1019:
        {
            usr->field_AE2 = 0;
            usr->field_3426 = 0;
            yw->field_757E = 0;

            int v416 = 2;
            usr->sub_bar_button->button_func68(&v416);

            ypaworld_func158__sub0__sub9(yw);
        }
        break;

        case 1021:
            if ( ypaworld_func158__sub0__sub7(usr) )
            {
                const char *v18 = get_lang_string(ypaworld__string_pointers, 2439, "2439");
                const char *v19 = get_lang_string(ypaworld__string_pointers, 2434, "DO YOU WANT TO LOAD >>>OLDER<<< SAVEGAME?");
                sub_46D9E0(usr, 1, v19, v18, 0);
            }
            else
            {
                const char *v20 = get_lang_string(ypaworld__string_pointers, 2440, "2440");
                const char *v21 = get_lang_string(ypaworld__string_pointers, 2482, "DO YOU WANT TO LOAD INGAME SAVEGAME?");
                sub_46D9E0(usr, 1, v21, v20, 0);
            }
            break;

        case 1016:
            usr->field_AE2 = 0;
            ypaworld_func158__sub0__sub12(yw);
            break;

        case 1020:
            if ( usr->field_AE2 )
                ypaworld_func158__sub0__sub10(yw);
            else
                ypaworld_func158__sub0__sub11(yw);
            break;

        case 1026:
            sub_46C3E4(usr);
            usr->field_0x1760 = 1;
            break;

        default:
            break;
        }
    }

    if ( usr->field_46 == 2 )
    {
        if ( usr->field_3A->dword8 )
        {
            if ( !usr->field_D52 && usr->field_3A->dword8 == (0x80 | 0x2B) )
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 759, "help\\19.html");
        }

        if ( usr->field_3A->downed_key )
        {
            if ( usr->field_D52 )
            {
                usr->input_listview.field_1D0 &= 0xFFFFF7FF;

                if ( keySS[ (int)usr->field_3A->downed_key ].short_name )
                {
                    if ( usr->field_D3A )
                    {
                        usr->keyConfig[usr->field_D36].KeyCode = usr->field_3A->downed_key;

                        if ( usr->keyConfig[usr->field_D36].inp_type == 2 )
                            usr->field_D3A = 0;

                        usr->field_D52 = 0;
                        usr->keyConfig[usr->field_D36].field_10 = 0;
                    }
                    else
                    {
                        usr->keyConfig[usr->field_D36].slider_neg = usr->field_3A->downed_key;
                        usr->keyConfig[usr->field_D36].field_10 &= 0xFFFFFFFD;
                        usr->field_D3A = 1;
                    }
                }
                usr->field_3A->downed_key = 0;
            }
            else
            {
                usr->input_listview.field_1D0 |= 0x800;

                if ( usr->field_3A->downed_key == VK_BACK  || usr->field_3A->downed_key == VK_DELETE)
                {
                    if (usr->keyConfig[usr->field_D36].inp_type != 2)
                        usr->keyConfig[usr->field_D36].KeyCode = 0;
                }
                else if ( usr->field_3A->downed_key == VK_RETURN )
                {
                    usr->keyConfig[usr->field_D36].field_10 = 3;
                    usr->field_D52 = 1;
                    if ( usr->keyConfig[usr->field_D36].inp_type == 2 )
                        usr->field_D3A = 0;
                }
                else if ( usr->field_3A->downed_key == VK_ESCAPE )
                {
                    sb_0x46a8c0(usr);
                }
            }
        }
    }

    if ( usr->keyConfig[usr->field_D36].inp_type == 2 )
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        usr->button_input_button->button_func67(&v410);
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        usr->button_input_button->button_func66(&v410);
    }

    v6 = usr->button_input_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if (v6_l == 1050)
        {
            usr->field_D42 = 1;
            usr->field_D5E |= 1;
        }
        else if (v6_l == 1051)
        {
            usr->field_D42 = 0;
            usr->field_D5E |= 1;
        }
        else if (v6_l == 1052)
        {
            if ( usr->field_D5E & 1 )
            {
                usr->inp_joystick = usr->field_D42;
                if ( usr->field_D42 )
                    usr->p_ypaworld->field_73CE &= 0xFB;
                else
                    usr->p_ypaworld->field_73CE |= 4;
            }

            if ( usr->field_D5E & 4 )
            {
                usr->inp_altjoystick = usr->field_D4A;
                if ( usr->field_D4A )
                    usr->p_ypaworld->snd__cdsound |= 2;
                else
                    usr->p_ypaworld->snd__cdsound &= 0xFD;
            }

            if ( usr->field_D5E & 2 )
            {
                if ( usr->field_D4E )
                    usr->p_ypaworld->field_73CE &= 0xF7;
                else
                    usr->p_ypaworld->field_73CE |= 8;
            }

            usr->field_D5E = 0;
            sub_46D2B4(usr->p_ypaworld->self_full, usr);
            sub_457BC0(usr);

            int v416 = 2;
            usr->button_input_button->button_func68(&v416);

            v416 = 1;
            usr->titel_button->button_func68(&v416);

            sub_4C31C0(usr->p_ypaworld, &usr->input_listview);
            usr->field_46 = 1;
        }
        else if (v6_l == 1053)
        {
            ypaworld_func158__sub0__sub0(usr);
        }
        else if (v6_l == 1054)
        {
            sb_0x46a8c0(usr);
        }
        else if ( v6_l == 1055 )
        {
            usr->field_D4E = 0;
            usr->field_D5E |= 2;
        }
        else if ( v6_l == 1056 )
        {
            usr->field_D4E = 1;
            usr->field_D5E |= 2;
        }
        else if ( v6_l == 1057 )
        {
            usr->keyConfig[ usr->field_D36 ].KeyCode = 0;
        }
        else if ( v6_l == 1058 )
        {
            usr->field_D4A = 1;
            usr->field_D5E |= 4;
        }
        else if ( v6_l == 1059 )
        {
            usr->field_D4A = 0;
            usr->field_D5E |= 4;
        }
        else if ( v6_l == 1250 )
        {
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 759, "help\\19.html");
            usr->field_D52 = 0;
        }
    }

    if ( usr->field_46 == 2 )
    {
        lstvw_update_input(yw, &usr->input_listview, usr->field_3A);

        usr->field_D36 = usr->input_listview.field_1DE + 1;

        if ( usr->input_listview.field_1D0 & 0x80 )
        {
            usr->keyConfig[ usr->field_D36 ].field_10 = 0;
            usr->field_D3A = 1;
            usr->field_D52 = 0;
        }
        lstvw_update(yw, &usr->input_listview);
    }

    if ( usr->field_46 == 3 )
    {
        if ( usr->field_3A->downed_key == VK_RETURN )
        {
            if ( usr->video_listvw.cmd_flag & 0x20 )
            {
                if ( usr->d3d_listvw.cmd_flag & 0x20 )
                {
                    if ( usr->field_13C2 & 1 && usr->game_default_res != usr->p_ypaworld->game_default_res && usr->game_default_res )
                    {
                        const char *v44 = get_lang_string(ypaworld__string_pointers, 342, "THIS CAN ... PROBLEMS");
                        const char *v45 = get_lang_string(ypaworld__string_pointers, 341, "DO YOU WANT TO CHANGE VIDEOMODE?");

                        sub_46D9E0(usr, 5, v45, v44, 0);
                    }
                    else
                    {
                        sb_0x46aa8c(usr);
                    }
                }
            }

            if ( !(usr->video_listvw.cmd_flag & 0x20) )
            {
                sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);

                if ( usr->video_listvw.cmd_flag & 0x20 )
                {
                    v408.butID = 1156;
                    v408.field_4 = 2;

                    usr->video_button->button_func73(&v408);
                }

                usr->field_13C2 |= 1;
                sub_46C5F0(usr, 0);
            }

        }
        else if ( usr->field_3A->downed_key == VK_ESCAPE )
        {
            sub_46A3C0(usr);
            usr->field_46 = 1;
        }
        if ( usr->field_3A->dword8 == (0x80 | 0x2B) )
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 760, "help\\110.html");
    }


    v6 = usr->video_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 1100 )
        {
            sub_4C31EC(yw, &usr->video_listvw);
            sub_423F74(&usr->samples1_info, 7);

            usr->field_3A->winp131arg.flag &= 0xFFFFFFFD;
        }
        else if ( v6_l == 1101 )
        {
            sub_4C31C0(usr->p_ypaworld, &usr->video_listvw);
        }
        else if ( v6_l == 1102 )
        {
            usr->field_13C2 |= 0x10;
            usr->field_0x13a8 |= 1;
        }
        else if ( v6_l == 1103 )
        {
            usr->field_0x13a8 &= 0xFFFE;
            usr->field_13C2 |= 0x10;
        }
        else if ( v6_l == 1106 )
        {
            usr->field_13C2 |= 8;
            usr->field_0x13a8 |= 2;
        }
        else if ( v6_l == 1107 )
        {
            usr->field_0x13a8 &= 0xFFFD;
            usr->field_13C2 |= 8;
        }
        else if ( v6_l == 1108 )
        {
            usr->field_13C2 |= 0x40;
        }
        else if ( v6_l == 1111 )
        {
            usr->field_13C2 |= 2;
            usr->field_0x13b0 &= 0xFFFE;
        }
        else if ( v6_l == 1112 )
        {
            usr->field_0x13b0 |= 1;
            usr->field_13C2 |= 2;
        }
        else if ( v6_l == 1113 )
        {
            usr->field_0x13a8 |= 0x10;
            usr->field_13C2 |= 4;
        }
        else if ( v6_l == 1114 )
        {
            usr->field_0x13a8 &= 0xEF;
            usr->field_13C2 |= 4;
        }
        else if ( v6_l == 1115 )
        {
            sub_423F74(&usr->samples1_info, 0);
            usr->field_13C2 |= 0x80;
        }
        else if ( v6_l == 1117 )
        {
            sub_424000(&usr->samples1_info, 0);
        }
        else if ( v6_l == 1118 )
        {
            usr->field_13C2 |= 0x100;
        }
        else if ( v6_l == 1124 )
        {
            if ( (usr->field_13C2 & 1) &&  usr->game_default_res != usr->p_ypaworld->game_default_res && usr->game_default_res )
            {
                const char *v51 = get_lang_string(ypaworld__string_pointers, 342, "THIS CAN ... PROBLEMS");
                const char *v52 = get_lang_string(ypaworld__string_pointers, 341, "DO YOU WANT TO CHANGE VIDEOMODE?");
                sub_46D9E0(usr, 5, v52, v51, 0);
            }
            else
            {
                sb_0x46aa8c(usr);
            }
        }
        else if ( v6_l == 1125 ) // Options CANCEL
        {
            sub_46A3C0(usr);
        }
        else if ( v6_l == 1126 )
        {
            usr->field_13BE = 1;
            usr->field_13C2 |= 0x20;
        }
        else if ( v6_l == 1127 )
        {
            usr->field_13BE = 0;
            usr->field_13C2 |= 0x20;
        }
        else if ( v6_l == 1128 )
        {
            usr->field_13C2 |= 0x200;
            usr->field_0x13b0 |= 0x10;
        }
        else if ( v6_l == 1129 )
        {
            usr->field_0x13b0 &= 0xFFEF;
            usr->field_13C2 |= 0x200;
        }
        else if ( v6_l == 1130 )
        {
            usr->field_13C2 |= 0x400;
            usr->field_0x13a8 |= 8;
        }
        else if ( v6_l == 1131 )
        {
            usr->field_0x13a8 &= 0xFFF7;
            usr->field_13C2 |= 0x400;
        }
        else if ( v6_l == 1132 )
        {
            usr->field_13C2 |= 0x800;
            usr->field_0x13a8 |= 4;
        }
        else if ( v6_l == 1133 )
        {
            usr->field_0x13a8 &= 0xFFFB;
            usr->field_13C2 |= 0x800;
        }
        else if ( v6_l == 1134 )
        {
            sub_4C31EC(yw, &usr->d3d_listvw);
            sub_423F74(&usr->samples1_info, 7);

            usr->field_3A->winp131arg.flag &= 0xFFFFFFFD;
        }
        else if ( v6_l == 1135 )
        {
            sub_4C31C0(usr->p_ypaworld, &usr->d3d_listvw);
        }
        else if ( v6_l == 1250 )
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 760, "help\\110.html");
    }

    if ( usr->field_46 == 3 && !(usr->video_listvw.cmd_flag & 0x20) )
    {
        lstvw_update_input(yw, &usr->video_listvw, usr->field_3A);

        if ( usr->video_listvw.field_1D0 & 0x400 )
        {
            int v65 = 0;

            if ( yw->GameShell->field_1CEA )
                v65 = 1;

            usr->field_13C2 |= 1;
            sub_46C5F0(usr, v65);
        }

        if ( usr->video_listvw.cmd_flag & 0x20 )
        {
            v408.field_4 = 2;
            v408.butID = 1156;

            usr->video_button->button_func73(&v408);
        }

        lstvw_update(yw, &usr->video_listvw);
    }

    if ( usr->field_46 == 3 && !(usr->d3d_listvw.cmd_flag & 0x20) )
    {
        lstvw_update_input(yw, &usr->d3d_listvw, usr->field_3A);

        if ( usr->d3d_listvw.field_1D0 & 0x400 )
        {
            int v66 = 0;

            if ( yw->GameShell->field_1CEA )
                v66 = 1;

            usr->field_13C2 |= 0x1000;
            ypaworld_func158__sub0__sub5(usr, v66);
        }

        if ( usr->d3d_listvw.cmd_flag & 0x20 )
        {
            v408.field_4 = 2;
            v408.butID = 1172;

            usr->video_button->button_func73(&v408);
        }

        lstvw_update(yw, &usr->d3d_listvw);
    }


    int v347 = 1159;
    button_str2_t2 *v67 = (button_str2_t2 *)usr->video_button->button_func74(&v347);
    sprintf(v306, "%d", v67->field_0);

    v395.field_4 = v306;
    v395.field_8 = v306;
    v395.butID = 1158;
    usr->video_button->button_func71(&v395);

    usr->field_0x13a4 = v67->field_0;


    v347 = 1152;
    v67 = (button_str2_t2 *)usr->video_button->button_func74(&v347);
    sprintf(v306, "%d", v67->field_0);

    v395.field_4 = v306;
    v395.field_8 = v306;
    v395.butID = 1153;
    usr->video_button->button_func71(&v395);
    usr->field_0x13b4 = v67->field_0;

    milesEngine__setter(0x80004003, usr->field_0x13b4, 0);


    v347 = 1154;
    v67 = (button_str2_t2 *)usr->video_button->button_func74(&v347);
    sprintf(v306, "%d", v67->field_0);

    v395.field_4 = v306;
    v395.field_8 = v306;
    v395.butID = 1155;
    usr->video_button->button_func71(&v395);
    usr->field_0x13b8 = v67->field_0;

    int ttmp = usr->field_0x13b8;
    sub_4448C0(&ttmp);


    char v308[300];

    if ( usr->field_46 == 9 ) //Load/Save
    {
        if ( (usr->field_3A->downed_key > 0 && usr->field_3A->downed_key < 0x81) || (usr->field_3A->downed_key >= 0xA0 && usr->field_3A->downed_key < 0xFF) || usr->field_3A->chr )
        {
            if ( usr->field_0x1744 )
            {
                if ( usr->field_3A->downed_key == VK_BACK )
                {
                    if ( usr->usernamedir_len > 0 && usr->p_ypaworld->str17_NOT_FALSE == 0 )
                    {
                        int ln = strlen(usr->usernamedir);
                        for (int i = usr->usernamedir_len - 1; i < ln - 1; i++)
                            usr->usernamedir[i] = usr->usernamedir[i + 1];

                        usr->usernamedir[ln - 1] = 0;
                        usr->usernamedir_len--;
                    }
                }
                else if ( usr->field_3A->downed_key == VK_RETURN )
                {
                    switch ( usr->field_0x1744 )
                    {
                    case 1:
                        if ( usr->field_1612 )
                        {
                            const char *v88 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                            const char *v89 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                            sub_46D9E0(usr, 3, v89, v88, 0);
                        }
                        else
                        {
                            sb_0x46ca74(usr);
                        }
                        break;

                    case 2:
                        sub_46C914(usr);
                        break;

                    case 3:
                        if ( usr->field_1612 )
                        {
                            const char *v90 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                            const char *v91 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                            sub_46D9E0(usr, 6, v91, v90, 0);
                        }
                        else
                        {
                            sb_0x46cdf8(usr);
                        }
                        break;

                    case 4:
                        sub_46C748(usr);
                        break;

                    default:
                        break;
                    }
                }
                else if ( usr->field_3A->downed_key == VK_ESCAPE )
                {
                    usr->field_0x1744 = 0;
                }
                else if ( usr->field_3A->downed_key == VK_LEFT )
                {
                    if ( usr->usernamedir_len > 0 && usr->p_ypaworld->str17_NOT_FALSE == 0 )
                        usr->usernamedir_len--;
                }
                else if ( usr->field_3A->downed_key == VK_RIGHT )
                {
                    if ( usr->usernamedir_len < (int)strlen(usr->usernamedir) && usr->p_ypaworld->str17_NOT_FALSE == 0 )
                        usr->usernamedir_len++;
                }
                else if ( usr->field_3A->downed_key == VK_DELETE && usr->usernamedir_len < (int)strlen(usr->usernamedir) && usr->p_ypaworld->str17_NOT_FALSE == 0 )
                {
                    int ln = strlen(usr->usernamedir);
                    for (int i = usr->usernamedir_len; i < ln - 1; i++)
                        usr->usernamedir[i] = usr->usernamedir[i + 1];

                    usr->usernamedir[ln - 1] = 0;
                }

                if ( strlen(usr->usernamedir) < 32 )
                {
                    if ( usr->field_3A->chr > 0x1F && usr->p_ypaworld->str17_NOT_FALSE == 0 )
                    {
                        char v337[6];
                        char a2a[6];

                        sprintf(v337, "%c", usr->field_3A->chr);

                        strcpy(a2a, v337);

                        if ( v337[0] == '*' || a2a[0] != '*' )
                        {
                            if ( ypaworld_func158__sub0__sub6(v337[0]) )
                            {
                                strncpy(v308, usr->usernamedir, usr->usernamedir_len);
                                strncpy(v308 + usr->usernamedir_len, a2a, 1);
                                strcpy(v308 + usr->usernamedir_len + 1, usr->usernamedir + usr->usernamedir_len);

                                strcpy(usr->usernamedir, v308);
                                usr->usernamedir_len++;
                            }
                        }
                    }
                }
            }
            else
            {
                if ( usr->field_3A->downed_key == VK_ESCAPE )
                    sub_46A7F8(usr);

                if ( usr->field_3A->dword8 == (0x80 | 0x2B) )
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 758, "help\\18.html");

            }

            if ( usr->field_1612 )
                sub_4DDFA4(&usr->disk_listvw, usr->field_1612 - 1);
        }
    }


    profilesNode *v107 = (profilesNode *)usr->files_list.head;
    usr->field_1612 = 0;
    int v108 = 1;

    while (v107->next)
    {
        if ( !strcasecmp(v107->profile_subdir, usr->usernamedir) )
        {
            usr->field_1612 = v108;
            break;
        }

        v108++;
        v107 = (profilesNode *)v107->next;
    }

    v6 = usr->disk_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 103 )
        {
            sub_46A7F8(usr);
        }
        else if ( v6_l == 1160 )
        {
            usr->field_0x1744 = 2;

            if ( !usr->field_1612 )
            {
                strcpy(usr->usernamedir, get_lang_string(ypaworld__string_pointers, 366, "NEW GAME"));
            }

            usr->usernamedir_len = strlen(usr->usernamedir);
            strncpy(v308, usr->usernamedir, usr->usernamedir_len);
            strncpy(&v308[usr->usernamedir_len], "h", 1);

            strcpy(&v308[usr->usernamedir_len + 1], &usr->usernamedir[usr->usernamedir_len]);

            v395.field_8 = NULL;
            v395.butID = 1100;
            v395.field_4 = v308;
            usr->disk_button->button_func71(&v395);
        }
        else if ( v6_l == 1161 )
        {
            usr->field_0x1744 = 4;
            if ( !usr->field_1612 )
            {
                strcpy(usr->usernamedir, get_lang_string(ypaworld__string_pointers, 366, "NEW GAME"));
            }
            usr->usernamedir_len = strlen(usr->usernamedir);
            strncpy(v308, usr->usernamedir, usr->usernamedir_len);
            strncpy(&v308[usr->usernamedir_len], "h", 1);

            strcpy(&v308[usr->usernamedir_len + 1], &usr->usernamedir[usr->usernamedir_len]);

            v395.field_8 = NULL;
            v395.butID = 1100;
            v395.field_4 = v308;
            usr->disk_button->button_func71(&v395);
        }
        else if ( v6_l == 1162 )
        {
            usr->field_0x1744 = 3;

            const char *v135 = get_lang_string(ypaworld__string_pointers, 366, "NEW GAME");

            profilesNode *v136 = (profilesNode *)usr->files_list.head;
            int v420 = 0;

            while (v136->next)
            {
                if ( !strnicmp(v135, v136->profile_subdir, strlen(v135)) )
                {
                    int v138 = atoi( v136->profile_subdir + strlen(v135) );

                    if ( v138 > v420 )
                        v420 = v138;
                }

                v136 = (profilesNode *)v136->next;
            }

            sprintf(usr->usernamedir, "%s%d", v135, v420 + 1);

            if ( usr->p_ypaworld->str17_NOT_FALSE )
            {
                windd_dlgBox a1a;
                memset(&a1a, 0, sizeof(windd_dlgBox));

                a1a.title = get_lang_string(ypaworld__string_pointers, 365, "ENTER NAME");
                a1a.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
                a1a.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                a1a.timer_func = 0;
                a1a.startText = usr->usernamedir;
                a1a.maxLen = 32;
                a1a.replace = 1;

                windd->windd_func322(&a1a);

                if ( !a1a.result )
                {
                    usr->field_0x1744 = 0;
                }
                else
                {
                    strcpy(usr->usernamedir, a1a.result);
                }
            }
            else
            {
                usr->usernamedir_len = strlen(usr->usernamedir);
                strncpy(v308, usr->usernamedir, usr->usernamedir_len);
                strncpy(&v308[usr->usernamedir_len], "h", 1);

                strcpy(&v308[usr->usernamedir_len + 1], &usr->usernamedir[usr->usernamedir_len]);

                v395.field_8 = NULL;
                v395.butID = 1100;
                v395.field_4 = v308;
                usr->disk_button->button_func71(&v395);
            }
        }
        else if ( v6_l == 1163 )
        {
            usr->field_0x1744 = 1;
            if ( !usr->field_1612 )
            {
                strcpy( usr->usernamedir, get_lang_string(ypaworld__string_pointers, 366, "NEW GAME") );
            }

            if ( usr->p_ypaworld->str17_NOT_FALSE )
            {
                windd_dlgBox a1a;
                memset(&a1a, 0, sizeof(windd_dlgBox));

                a1a.title = get_lang_string(ypaworld__string_pointers, 365, "ENTER NAME");
                a1a.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
                a1a.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                a1a.timer_func = 0;
                a1a.startText = usr->usernamedir;
                a1a.maxLen = 32;
                a1a.replace = 1;

                windd->windd_func322(&a1a);

                if ( !a1a.result )
                {
                    usr->field_0x1744 = 0;
                }
                else
                {
                    strcpy(usr->usernamedir, a1a.result);
                }
            }
            else
            {
                usr->usernamedir_len = strlen(usr->usernamedir);
                strncpy(v308, usr->usernamedir, usr->usernamedir_len);
                strncpy(&v308[usr->usernamedir_len], "h", 1);

                strcpy(&v308[usr->usernamedir_len + 1], &usr->usernamedir[usr->usernamedir_len]);

                v395.field_8 = NULL;
                v395.butID = 1100;
                v395.field_4 = v308;
                usr->disk_button->button_func71(&v395);
            }
        }
        else if ( v6_l == 1164)
        {
            switch ( usr->field_0x1744 )
            {
            case 1:
                if ( usr->field_1612 )
                {
                    const char *v112 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                    const char *v113 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                    sub_46D9E0(usr, 3, v113, v112, 0);
                }
                else
                {
                    sb_0x46ca74(usr);
                }
                break;
            case 2:
                sub_46C914(usr);
                break;
            case 3:
                if ( usr->field_1612 )
                {
                    const char *v114 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                    const char *v115 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                    sub_46D9E0(usr, 6, v115, v114, 0);
                }
                else
                {
                    sb_0x46cdf8(usr);
                }
                break;
            case 4:
                sub_46C748(usr);
                break;
            default:
                break;
            }
        }
        else if (v6_l == 1165)
        {
            if ( usr->field_0x1744 )
            {
                usr->field_0x1744 = 0;
            }
            else
                sub_46A7F8(usr);
        }
        else if (v6_l == 1250)
        {
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 758, "help\\18.html");
            usr->field_0x1744 = 0;
        }
    }

    if ( usr->field_46 == 9 ) // Multiplayer
    {
        lstvw_update_input(yw, &usr->disk_listvw, usr->field_3A);

        if ( usr->disk_listvw.field_1D0 & 0x80 || usr->field_3A->downed_key == VK_UP || usr->field_3A->downed_key == VK_DOWN )
        {
            usr->field_1612 = usr->disk_listvw.field_1DE + 1;

            if ( usr->field_1612 < 1 )
                usr->field_1612 = 1;

            if ( usr->field_1612 > usr->disk_listvw.elements_for_scroll_size  )
                usr->field_1612 = usr->disk_listvw.elements_for_scroll_size;


            profilesNode *node = (profilesNode *)usr->files_list.head;

            for (int i = 0; i < usr->field_1612 - 1; i++) // check usr->field_1612 - 1
            {
                if ( ! node->next)
                {
                    node = NULL;
                    usr->field_1612 = 0;
                    break;
                }
                node = (profilesNode *)node->next;
            }

            if (node)
            {
                strcpy(usr->usernamedir, node->profile_subdir);
                usr->usernamedir_len = strlen(usr->usernamedir);
            }
        }
        lstvw_update(yw, &usr->disk_listvw);
    }

    if ( usr->field_0x1744 )
    {
        v410.butID = 1105;
        usr->disk_button->button_func66(&v410);

        v410.field_4 = 0;
        v410.butID = 1104;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1101;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1102;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1103;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1100;
        usr->disk_button->button_func67(&v410);

        if ( usr->field_0x1744 == 4 )
        {
            v410.field_4 = 0;
            v410.butID = 1105;

            if ( !usr->field_1612 || !strcasecmp(usr->usernamedir, usr->user_name) )
                usr->disk_button->button_func67(&v410);
            else
                usr->disk_button->button_func66(&v410);
        }

        if ( usr->field_0x1744 == 2 && !usr->field_1612 )
        {
            v410.field_4 = 0;
            v410.butID = 1105;
            usr->disk_button->button_func67(&v410);
        }

        if ( usr->field_0x1744 == 1 || usr->field_0x1744 == 3 )
        {
            v410.butID = 1100;
            usr->disk_button->button_func66(&v410);
        }

        if ( usr->p_ypaworld->str17_NOT_FALSE )
        {
            strcpy(v308, usr->usernamedir);
        }
        else
        {
            char *v174 = usr->usernamedir;

            strncpy(v308, usr->usernamedir, usr->usernamedir_len);
            strncpy(&v308[usr->usernamedir_len], "_", 1);

            v174 += usr->usernamedir_len;

            strcpy(&v308[usr->usernamedir_len + 1], v174);
        }
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1105;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1104;
        usr->disk_button->button_func66(&v410);

        v410.butID = 1101;
        usr->disk_button->button_func66(&v410);

        v410.butID = 1103;
        usr->disk_button->button_func66(&v410);

        v410.butID = 1100;
        usr->disk_button->button_func67(&v410);

        v410.butID = 1102;
        if ( !strcasecmp(usr->usernamedir, usr->user_name) )
            usr->disk_button->button_func67(&v410);
        else
            usr->disk_button->button_func66(&v410);

        strcpy(v308, usr->usernamedir);
    }

    v395.field_8 = NULL;
    v395.field_4 = v308;
    v395.butID = 1100;
    usr->disk_button->button_func71(&v395);

    if ( usr->field_46 == 7 )
    {

        if ( usr->field_3A->downed_key == VK_RETURN )
        {
            sub_46B0E0(usr);
        }
        else if ( usr->field_3A->downed_key == VK_ESCAPE )
        {
            sub_46AA0C(usr);
        }

        if ( usr->field_3A->dword8 == (0x80 | 0x2B) )
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 761, "help\\111.html");
    }


    v6 = usr->locale_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if (v6)
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 103 )
        {
            sub_46AA0C(usr);
        }
        else if ( v6_l == 1250 )
        {
            yw->field_81AF = get_lang_string(ypaworld__string_pointers, 761, "help\\111.html");
        }
        else if ( v6_l == 1300 )
        {
            sub_46B0E0(usr);
        }
        else if ( v6_l == 1301 )
        {
            sub_46AA0C(usr);
        }
    }

    if ( usr->field_46 == 7 ) //Locale
    {
        lstvw_update_input(yw, &usr->local_listvw, usr->field_3A);

        if ( usr->local_listvw.field_1D0 & 0x80 )
        {
            langDll_node *node = (langDll_node *)usr->lang_dlls.head;

            usr->field_19CA |= 1;

            for(int i = 0; i < usr->local_listvw.field_1DE; i++)
                node = (langDll_node *)node->next;

            usr->prev_lang = node;
        }

        lstvw_update(yw, &usr->local_listvw);
    }

    v6 = usr->about_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_l == 103 )
        {
            usr->field_46 = 1;

            int v416 = 2;
            usr->about_button->button_func68(&v416);
        }
    }

    if ( usr->field_46 == 8 )
    {
        if ( usr->field_3A->downed_key == VK_RETURN || usr->field_3A->downed_key == VK_ESCAPE )
        {
            usr->field_46 = 1;

            int v416 = 2;
            usr->about_button->button_func68(&v416);

            v416 = 1;
            usr->titel_button->button_func68(&v416);
        }
    }

    if ( usr->field_46 == 1 )
    {
        if ( usr->field_19DA && usr->field_42 - usr->field_19D6 >= 700 )
        {
            usr->field_19DA = 0;
        }
        else
        {
            switch ( usr->field_19DA )
            {
            case 0:
                if ( sub_449678(usr->p_ypaworld, usr->field_3A, 'A') ) // VK_A
                {
                    usr->field_19D6 = usr->field_42;
                    usr->field_19DA++;
                }
                else
                {
                    if ( usr->field_3A->downed_key )
                        usr->field_19DA = 0;
                }
                break;

            case 1:
                if ( sub_449678(usr->p_ypaworld, usr->field_3A, 'M') )
                {
                    usr->field_19D6 = usr->field_42;
                    usr->field_19DA++;
                }
                else
                {
                    if ( usr->field_3A->downed_key )
                        usr->field_19DA = 0;
                }
                break;

            case 2:
                if ( sub_449678(usr->p_ypaworld, usr->field_3A, 'O') )
                {
                    usr->field_19D6 = usr->field_42;
                    usr->field_19DA++;
                }
                else
                {
                    if ( usr->field_3A->downed_key )
                        usr->field_19DA = 0;
                }
                break;

            case 3:
                if ( sub_449678(usr->p_ypaworld, usr->field_3A, 'K') )
                {
                    ypaworld_func158__sub0__sub4(usr);
                    sub_423F74(&usr->samples2_info, 3);
                }
                else
                {
                    if ( usr->field_3A->downed_key )
                        usr->field_19DA = 0;
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        usr->field_19DA = 0;
    }

    switch ( usr->field_1C3A )
    {
    case 0:
        usr->field_1C3E = 0;
        usr->field_1C36 = 1;
        usr->network_listvw.element_count_max = 12;
        usr->field_0x1c30 = 3 * (yw->font_default_h+ word_5A50C2);
        break;
    case 1:
        usr->field_1C3E = 0;
        usr->field_1C36 = 1;
        usr->network_listvw.element_count_max = 12;
        usr->field_0x1c30 = 3 * (yw->font_default_h + word_5A50C2);
        break;
    case 3:
        usr->field_1C3E = 0;
        usr->field_1C36 = 1;
        usr->network_listvw.element_count_max = 12;
        usr->field_0x1c30 = 3 * (yw->font_default_h + word_5A50C2);
        break;
    case 2:
        usr->field_1C36 = 0;
        usr->network_listvw.element_count_max = 12;
        usr->field_1C3E = 1;
        break;
    case 4:
        usr->field_1C3E = 1;
        usr->field_1C36 = 1;
        usr->network_listvw.element_count_max = 6;
        usr->field_0x1c30 = yw->font_default_h * 9.5 + 2 * word_5A50C2;
        break;
    default:
        break;
    }


    yw_arg181 v346;
    yw_arg181_a v378;

    windp_arg79 v368;

    v6 = usr->network_button->button_func69(usr->field_3A);
    v6_l = v6 & 0xFFFF;
    v6_h = (v6 >> 16) & 0xFFFF;

    if ( v6 )
    {
        if ( v6_h )
            sub_4DE248(usr, v6_h);

        if ( v6_l == 1204 || v6_l == 1205 || v6_l == 1206 || v6_l == 1207 )
        {
            v378.fld_3 = 0;
            v346.val_size = 20;
            v346.field_10 = 0;
            v346.field_18 = 1;
            v346.value = &v378;
            v378.fld_0 = 1021;
            v346.field_14 = 2;
        }

        if ( v6_l == 103 || v6_l == 1202 )
        {
            sub_46D698(usr);
        }
        else if ( v6_l == 1204 )
        {
            v378.fld_4_1 = usr->field_1CD6;
            usr->field_1CD5 |= usr->field_1CD6;
            v378.fld_4_2 = 1;
            usr->field_1CD6 = 1;
            usr->field_1CD5 &= 0xFE;

            usr->p_ypaworld->self_full->ypaworld_func181(&v346);
        }
        else if ( v6_l == 1205 )
        {
            v378.fld_4_1 = usr->field_1CD6;
            usr->field_1CD5 |= usr->field_1CD6;
            v378.fld_4_2 = 2;
            usr->field_1CD5 &= 0xFD;
            usr->field_1CD6 = 2;

            usr->p_ypaworld->self_full->ypaworld_func181(&v346);
        }
        else if ( v6_l == 1206 )
        {
            v378.fld_4_1 = usr->field_1CD6;
            usr->field_1CD5 |= usr->field_1CD6;
            v378.fld_4_2 = 4;
            usr->field_1CD6 = 4;
            usr->field_1CD5 &= 0xFB;

            usr->p_ypaworld->self_full->ypaworld_func181(&v346);
        }
        else if ( v6_l == 1207 )
        {
            v378.fld_4_1 = usr->field_1CD6;
            usr->field_1CD5 |= usr->field_1CD6;
            v378.fld_4_2 = 8;
            usr->field_1CD6 = 8;
            usr->field_1CD5 &= 0xF7;

            usr->p_ypaworld->self_full->ypaworld_func181(&v346);
        }

        switch ( usr->field_1C3A )
        {
        case 0:
            if ( v6_l == 1200 )
            {
                sub_46B1B8(usr);
            }
            else if ( v6_l == 1250 )
            {
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 753, "help\\13.html");
            }
            break;

        case 1:
            if ( v6_l == 1200 )
            {
                sb_0x46bb54(usr);
            }
            else if ( v6_l == 1201 )
            {
                usr->field_0x1cd8 = 1;
                usr->field_0x1c3c = -1;
                usr->network_listvw.scroll_pos = 0;
                usr->field_1C3A = 3;
            }
            else if ( v6_l == 1250 )
            {
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 755, "help\\15.html");
            }
            break;

        case 2:
            if ( v6_l == 1200 )
            {
                if ( usr->field_1C42[0] )
                {
                    strcpy(usr->callSIGN, usr->field_1C42);

                    usr->field_1C3A = 1;
                    usr->field_0x1c3c = -1;
                    usr->network_listvw.scroll_pos = 0;
                    usr->field_1C42[0] = 0;

                    sub_4C31EC(usr->p_ypaworld, &usr->network_listvw);
                }
            }
            else if ( v6_l == 1201 )
            {
                if ( yw->str17_NOT_FALSE )
                {
                    windd_dlgBox v339;
                    memset(&v339, 0, sizeof(windd_dlgBox));

                    v339.title = get_lang_string(ypaworld__string_pointers, 413, "ENTER CALLSIGN");
                    v339.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
                    v339.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                    v339.maxLen = 32;
                    v339.timer_func = NULL;
                    v339.startText = usr->field_1C42;

                    windd->windd_func322(&v339);

                    if ( v339.result )
                    {
                        strncpy(usr->field_1C42, v339.result, 64);
                        usr->field_1C42[63] = 0;
                    }
                }
            }
            else if ( v6_l == 1250 )
            {
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 754, "help\\14.html");
            }
            break;

        case 3:
            if ( v6_l == 1200 )
            {
                sub_46B328(usr);
            }
            else if ( v6_l == 1250 )
            {
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 756, "help\\16.html");
            }
            break;

        case 4:
            if ( v6_l == 1200 )
            {
                if ( usr->field_0x1cd8 )
                {
                    const char *v425;
                    const char *v425_1;

                    if ( usr->p_ypaworld->windp->windp_func86(NULL) <= 1 )
                    {
                        const char *v217 = get_lang_string(ypaworld__string_pointers, 2442, "2442");
                        const char *v218 = get_lang_string(ypaworld__string_pointers, 2435, "DO YOU REALLY WANT TO START WITHOUT OTHER PLAYERS?");
                        sub_46D9E0(usr, 2, v218, v217, 0);
                    }
                    else if ( ypaworld_func158__sub0__sub8(usr, &v425, &v425_1) )
                    {
                        sub_46D9E0(usr, 4, v425, v425_1, 1);
                    }
                    else
                    {
                        sub_46DC1C(usr);
                    }
                }
            }
            else if ( v6_l == 1203 )
            {
                if ( usr->field_0x1cd8 )
                {
                    usr->field_0x1c3c = -1;
                    usr->network_listvw.scroll_pos = 0;
                    usr->field_1CF7 = 0;
                    usr->field_1CF9[32][0] = 0;
                    usr->field_1C42[0] = 0;
                    usr->field_1C3A = 3;
                }
            }
            else if ( v6_l == 1208 )
            {
                v368.field_0 = 0;
                v368.field_4 = 0;
                while ( yw->windp->windp_func79(&v368) && strcasecmp(v368.field_8, usr->callSIGN) )
                    v368.field_4++;

                yw_arg181 v353;
                yw_arg181_a v375;

                usr->field_1CE9 = 1;
                usr->netTP1[v368.field_4].field_43 = 1;

                v375.fld_0 = 1023;
                v375.fld_3 = 0;
                v375.fld_4_1 = 1;

                v353.val_size = 20;
                v353.field_14 = 2;
                v353.value = &v375;
                v353.field_10 = 0;
                v353.field_18 = 1;

                usr->p_ypaworld->self_full->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.field_C = 2;
                v387.field_8 = 0;
                v387.field_4 = 1;
                v387.field_0 = usr->callSIGN;
                v387.field_10 = 1;

                usr->p_ypaworld->windp->windp_func82(&v387);
            }
            else if ( v6_l == 1209 )
            {
                v368.field_0 = 0;
                v368.field_4 = 0;
                while ( yw->windp->windp_func79(&v368) && strcasecmp(v368.field_8, usr->callSIGN) )
                    v368.field_4++;


                yw_arg181 v353;
                yw_arg181_a v375;

                usr->field_1CE9 = 0;
                usr->netTP1[v368.field_4].field_43 = 0;

                v375.fld_0 = 1023;
                v375.fld_3 = 0;

                v353.field_14 = 2;
                v353.field_10 = 0;
                v353.value = &v375;
                v353.val_size = 20;
                v353.field_18 = 1;

                usr->p_ypaworld->self_full->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.field_C = 2;
                v387.field_8 = 0;
                v387.field_4 = 1;
                v387.field_0 = usr->callSIGN;
                v387.field_10 = 1;

                usr->p_ypaworld->windp->windp_func82(&v387);
            }
            else if ( v6_l == 1210 )
            {
                if ( yw->str17_NOT_FALSE )
                {
                    windd_dlgBox v316;
                    memset(&v316, 0, sizeof(windd_dlgBox));

                    v316.title = get_lang_string(ypaworld__string_pointers, 422, "ENTER MESSAGE");
                    v316.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
                    v316.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
                    v316.startText = usr->field_1C42;
                    v316.timer_func = NULL;
                    v316.maxLen = 64;

                    windd->windd_func322(&v316);

                    if ( v316.result )
                    {
                        strncpy(usr->field_1C42, v316.result, 64);
                        usr->field_1C42[63] = 0;
                    }
                }

                if ( usr->field_1C42[0] )
                {
                    yw_arg181_b v312;

                    v312.fld_0 = 1018;
                    v312.fld_3 = 0;

                    strcpy(v312.fld_4, usr->field_1C42);

                    v346.field_C = 1;
                    v346.value = (yw_arg181_a *)&v312;
                    v346.val_size = 80;
                    v346.field_14 = 2;
                    v346.field_10 = 0;
                    v346.field_8 = usr->callSIGN;
                    v346.field_18 = 1;

                    usr->p_ypaworld->self_full->ypaworld_func181(&v346);

                    sub_4D0C24(usr->p_ypaworld, usr->callSIGN, v312.fld_4);

                    usr->field_1C42[0] = 0;
                    usr->field_1C84 = 0;

                    int v223 = strtol(v312.fld_4, NULL, 0);
                    if ( v223 > 0 )
                        sub_4D9550(yw, v223);
                }
            }
            else if ( v6_l == 1250 )
            {
                yw->field_81AF = get_lang_string(ypaworld__string_pointers, 757, "help\\17.html");
            }
            break;
        default:
            break;
        }
    }

    if ( usr->field_46 == 6 )
    {
        int a4 = usr->network_button->getBTN_h();

        usr->network_listvw.frm_1.btn_ypos = usr->field_0x1c30 + a4;

        lstvw_update_input(yw, &usr->network_listvw, usr->field_3A);

        if ( (usr->network_listvw.field_1D0 & 0x80) || usr->field_3A->downed_key == VK_UP || usr->field_3A->downed_key == VK_DOWN )
        {
            usr->field_0x1c3c = usr->network_listvw.field_1DE;

            switch ( usr->field_1C3A )
            {
            case 0:
            case 1:
                usr->field_1C3E = 0;
                break;
            case 3:
            {
                int v227 = usr->p_ypaworld->windp->windp_func86(NULL);

                int v228 = 0;

                for (int i = 0; i < 256; i++)
                {
                    if ( v227 <= 1 || v227 <= usr->p_ypaworld->LevelNet->mapInfos[ usr->map_descriptions[i].id ].robos_count)
                    {
                        if ( v228 == usr->field_0x1c3c )
                        {
                            usr->field_0x1c88 = usr->map_descriptions[i].pstring;
                            usr->field_1C86 = usr->map_descriptions[i].id;
                            break;
                        }

                        v228++;
                    }
                }
            }
            break;

            default:
                break;
            }

            usr->field_1C84 = strlen(usr->field_1C42);
        }

        lstvw_update(yw, &usr->network_listvw);
    }

    if ( usr->field_46 == 6 )
    {
        if ( usr->field_3A->downed_key || usr->field_3A->chr || usr->field_3A->dword8 )
        {
            if ( usr->field_1C3E )
            {
                uint32_t v233;

                if ( usr->field_1C3A == 2 )
                    v233 = 32;
                else
                    v233 = 38;

                if ( strlen(usr->field_1C42) < v233 )
                {
                    if ( usr->field_3A->chr > 0x1F && yw->str17_NOT_FALSE == 0 )
                    {
                        char v354[12];
                        char v345[12];

                        sprintf(v354, "%c", usr->field_3A->chr);

                        strcpy(v345, v354);

                        if ( v354[0] == '*' || v345[0] != '*' )
                        {
                            char v313[68];

                            strncpy(v313, usr->field_1C42, usr->field_1C84);
                            strncpy(v313 + usr->field_1C84, v345, 1);

                            strcpy(v313 + usr->field_1C84 + 1, usr->field_1C42 + usr->field_1C84 );

                            strcpy(usr->field_1C42, v313);

                            usr->field_1C84++;
                        }
                    }
                }

                if ( usr->field_3A->downed_key == VK_BACK && usr->field_1C84 > 0 && (yw->str17_NOT_FALSE == 0) )
                {
                    int ln = strlen(usr->field_1C42);

                    for (int i = usr->field_1C84 - 1; i < ln - 1; i++)
                        usr->field_1C42[i] = usr->field_1C42[i + 1];

                    usr->field_1C42[ln - 1] = 0;

                    usr->field_1C84--;
                }
                else if ( usr->field_3A->downed_key == VK_LEFT )
                {
                    if ( usr->field_1C84 > 0 && (yw->str17_NOT_FALSE == 0) )
                        usr->field_1C84--;
                }
                else if ( usr->field_3A->downed_key == VK_RIGHT )
                {
                    if ( usr->field_1C84 < (int32_t)strlen(usr->field_1C42) && (yw->str17_NOT_FALSE == 0) )
                        usr->field_1C84++;
                }
                else if ( usr->field_3A->downed_key == VK_DELETE && usr->field_1C84 < (int32_t)strlen(usr->field_1C42) && (yw->str17_NOT_FALSE == 0) )
                {
                    int ln = strlen(usr->field_1C42);

                    for (int i = usr->field_1C84; i < ln - 1; i++)
                        usr->field_1C42[i] = usr->field_1C42[i + 1];

                    usr->field_1C42[ln - 1] = 0;
                }
            }

            if ( usr->field_3A->downed_key == VK_RETURN )
            {
                switch ( usr->field_1C3A )
                {
                case 0:
                    sub_46B1B8(usr);
                    break;

                case 1:
                    if ( usr->network_listvw.elements_for_scroll_size )
                    {
                        sb_0x46bb54(usr);
                    }
                    else
                    {
                        usr->field_0x1cd8 = 1;
                        usr->field_0x1c3c = -1;
                        usr->field_1C3A = 3;
                        usr->network_listvw.scroll_pos = 0;
                    }
                    break;

                case 2:
                    if ( usr->field_1C42[0] )
                    {
                        strcpy(usr->callSIGN, usr->field_1C42);

                        usr->field_1C3A = 1;
                        usr->field_0x1c3c = -1;
                        usr->network_listvw.scroll_pos = 0;
                        usr->field_1C42[0] = 0;
                        sub_4C31EC(usr->p_ypaworld, &usr->network_listvw);
                    }
                    break;

                case 3:
                    sub_46B328(usr);
                    break;
                case 4:
                    if ( usr->field_1C42[0] )
                    {
                        char v311[64];

                        strcpy(v311, usr->field_1C42);

                        yw_arg181_b v309;

                        v309.fld_0 = 1018;
                        v309.fld_3 = 0;

                        strcpy(v309.fld_4, usr->field_1C42);

                        yw_arg181 v325;

                        v325.field_18 = 1;
                        v325.value = (yw_arg181_a *)&v309;
                        v325.val_size = 80;
                        v325.field_14 = 2;
                        v325.field_10 = 0;

                        usr->p_ypaworld->self_full->ypaworld_func181(&v325);

                        sub_4D0C24(usr->p_ypaworld, usr->callSIGN, v309.fld_4);
                        usr->field_1C42[0] = 0;
                        usr->field_1C84 = 0;

                        int v271 = strtol(v309.fld_4, NULL, 0);
                        if ( v271 > 0 )
                            sub_4D9550(yw, v271);
                    }
                    break;
                default:
                    break;
                }
            }
            else if ( usr->field_3A->downed_key == VK_ESCAPE )
            {
                sub_46D698(usr);
            }

            if ( usr->field_3A->dword8 == (0x80 | 0x2B) && !usr->field_1C3E )
            {
                switch ( usr->field_1C3A )
                {
                case 0:
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 753, "help\\13.html");
                    break;
                case 1:
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 755, "help\\15.html");
                    break;
                case 2:
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 754, "help\\14.html");
                    break;
                case 3:
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 756, "help\\16.html");
                    break;
                case 4:
                    yw->field_81AF = get_lang_string(ypaworld__string_pointers, 757, "help\\17.html");
                    break;
                default:
                    break;
                }
            }

            if ( usr->field_1C3E != -1 )
                sub_4DDFA4(&usr->network_listvw, usr->field_1C3E);

            usr->field_3A->downed_key = 0;
        }
    }

    if ( usr->field_0x1cd8 )
    {
        if ( usr->field_1C3A == 4 && usr->field_0x2fbc != 4 )
        {
            if ( (int)usr->p_ypaworld->windp->windp_func86(NULL)   <   usr->p_ypaworld->LevelNet->mapInfos[ usr->field_1C86 ].robos_count )
            {
                if ( usr->field_1CEF )
                {
                    int v357 = 0;
                    usr->p_ypaworld->windp->windp_func84(&v357);

                    usr->field_1CEF = 0;
                }
            }
            else if ( !usr->field_1CEF )
            {
                int v357 = 1;
                usr->p_ypaworld->windp->windp_func84(&v357);

                usr->field_1CEF = 1;
            }
        }
    }

    v410.butID = 1201;
    usr->network_button->button_func66(&v410);

    v410.butID = 1205;
    usr->network_button->button_func67(&v410);

    v410.butID = 1202;
    usr->network_button->button_func67(&v410);

    v410.butID = 1203;
    usr->network_button->button_func66(&v410);

    v410.butID = 1225;
    usr->network_button->button_func67(&v410);

    v410.butID = 1226;
    usr->network_button->button_func67(&v410);

    v410.butID = 1227;
    usr->network_button->button_func67(&v410);


    button_71arg v336;

    v336.butID = 1201;
    v336.field_4 = get_lang_string(ypaworld__string_pointers, 2, "OK");
    v336.field_8 = 0;

    usr->network_button->button_func71(&v336);

    v410.butID = 1220;
    usr->network_button->button_func67(&v410);

    v410.butID = 1206;
    usr->network_button->button_func67(&v410);

    v410.butID = 1207;
    usr->network_button->button_func67(&v410);

    v410.butID = 1208;
    usr->network_button->button_func67(&v410);

    v410.butID = 1209;
    usr->network_button->button_func67(&v410);

    v410.butID = 1219;
    usr->network_button->button_func67(&v410);

    v410.butID = 1221;
    usr->network_button->button_func67(&v410);

    v410.field_4 = 0;
    v410.butID = 1210;
    usr->network_button->button_func67(&v410);

    v410.butID = 1211;
    usr->network_button->button_func67(&v410);

    v410.butID = 1212;
    usr->network_button->button_func67(&v410);

    v410.butID = 1213;
    usr->network_button->button_func67(&v410);

    v410.butID = 1214;
    usr->network_button->button_func67(&v410);

    v410.butID = 1215;
    usr->network_button->button_func67(&v410);

    v410.butID = 1216;
    usr->network_button->button_func67(&v410);

    v410.butID = 1217;
    usr->network_button->button_func67(&v410);

    if ( (usr->field_1C3A != 1 || usr->p_ypaworld->windp->windp_func90(NULL) != 3)
            && (usr->field_1C3A != 1 || usr->field_0x1cdc != 1 || usr->p_ypaworld->windp->windp_func90(NULL) != 4)
            && usr->field_1C3A )
    {
        v410.butID = 1228;
        usr->network_button->button_func67(&v410);
    }
    else
    {
        const char *v280;

        if ( usr->field_1C3A )
        {
            v280 = get_lang_string(ypaworld__string_pointers, 2402, "PRESS SPACEBAR TO UPDATE SESSION LIST");
        }
        else
        {
            if ( usr->p_ypaworld->field_75E2[0] )
            {
                char *v278 = usr->p_ypaworld->field_75E2;
                const char *v279 = get_lang_string(ypaworld__string_pointers, 2437, "YOUR TCP/IP ADDRESS");
                sprintf(v306, "%s  %s", v279, v278);
            }
            else
            {
                strcpy(v306, " ");
            }
            v280 = v306;
        }

        v395.field_4 = v280;
        v395.field_8 = 0;
        v395.butID = 1228;
        usr->network_button->button_func71(&v395);

        v410.butID = 1228;
        usr->network_button->button_func66(&v410);
    }

    if ( (!usr->field_1C3E || yw->str17_NOT_FALSE) && usr->field_1C3A != 2 )
    {
        v410.butID = 1200;
        usr->network_button->button_func67(&v410);
    }
    else
    {
        v410.butID = 1200;
        usr->network_button->button_func66(&v410);

        v393.xpos = -1;
        v393.butID = 1200;

        if ( usr->field_1C3A == 2 )
        {
            v393.width = dword_5A50B6;
            v393.ypos = 3 * (word_5A50C2 + yw->font_default_h);
        }
        else
        {
            v393.width = dword_5A50B6 * 0.8;
            v393.ypos = 14 * (word_5A50C2 + yw->font_default_h);
        }

        usr->network_button->button_func76(&v393);


        if ( yw->str17_NOT_FALSE )
        {
            strcpy(v308, usr->field_1C42);
        }
        else
        {
            strncpy(v308, usr->field_1C42, usr->field_1C84);
            strncpy(v308 + usr->field_1C84, "_", 1);

            strcpy(v308 + usr->field_1C84 + 1, usr->field_1C42 + usr->field_1C84);
        }

        v395.butID = 1200;
        v395.field_4 = v308;
        v395.field_8 = 0;
        usr->network_button->button_func71(&v395);
    }

    v393.xpos = -1;
    v393.width = -1;
    v393.butID = 1202;

    if ( usr->field_1C3A == 2 )
    {
        v393.ypos = 4 * (word_5A50C2 + yw->font_default_h);
    }
    else
    {
        v393.ypos = (word_5A50C2 + yw->font_default_h) * 15.2;
    }

    usr->network_button->button_func76(&v393);

    switch ( usr->field_1C3A )
    {
    case 0:
        v410.field_4 = 0;
        v410.butID = 1205;

        usr->network_button->button_func67(&v410);

        v395.butID = 1204;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 410, "SELECT PROVIDER");
        v395.field_8 = 0;
        usr->network_button->button_func71(&v395);

        v395.butID = 1222;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 425, "2");
        usr->network_button->button_func71(&v395);

        v395.butID = 1223;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 426, "3");
        usr->network_button->button_func71(&v395);
        break;

    case 1:
        if ( usr->p_ypaworld->windp->windp_func90(NULL) != 4 || !usr->field_0x1cdc )
        {
            v395.butID = 1202;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 402, "NEW");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);

            v410.butID = 1202;
            usr->network_button->button_func66(&v410);
        }

        v395.butID = 1204;
        v395.field_8 = 0;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 411, "SELECT SESSION");
        usr->network_button->button_func71(&v395);

        v395.butID = 1222;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 428, "5");
        usr->network_button->button_func71(&v395);

        v395.butID = 1223;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 429, "6");
        usr->network_button->button_func71(&v395);

        int v349[2];
        v349[1] = 0;

        if ( usr->p_ypaworld->windp->windp_func69(v349) )
        {
            v395.butID = 1201;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 406, "JOIN");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);
        }
        else if ( usr->p_ypaworld->windp->windp_func90(NULL) != 4 || usr->field_0x1cdc )
        {
            v410.butID = 1201;
            usr->network_button->button_func67(&v410);
        }
        else
        {
            v395.butID = 1201;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 421, "SEARCH");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);
        }
        break;

    case 2:
        v395.butID = 1204;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 413, "ENTER PLAYER");
        v395.field_8 = 0;
        usr->network_button->button_func71(&v395);

        v395.butID = 1222;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 434, "11");
        usr->network_button->button_func71(&v395);

        v395.butID = 1223;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 435, "12");
        usr->network_button->button_func71(&v395);

        if ( yw->str17_NOT_FALSE )
        {
            v410.butID = 1202;
            usr->network_button->button_func66(&v410);

            v395.butID = 1202;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 423, "CHANGE");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);
        }
        break;

    case 3:
        if ( usr->field_1CEA )
        {
            v410.butID = 1205;
            usr->network_button->button_func67(&v410);
        }

        v395.butID = 1204;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 412, "SELECT LEVE");
        v395.field_8 = 0;
        usr->network_button->button_func71(&v395);

        v395.butID = 1222;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 431, "8");
        usr->network_button->button_func71(&v395);

        v395.butID = 1223;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 432, "9");
        usr->network_button->button_func71(&v395);
        break;

    case 4:
        v410.butID = 1225;
        usr->network_button->button_func66(&v410);

        v395.butID = 1225;
        v395.field_4 = get_lang_string(ypaworld__string_pointers, 405, "SEND");
        v395.field_8 = 0;
        usr->network_button->button_func71(&v395);

        v410.butID = 1226;
        usr->network_button->button_func66(&v410);

        v410.butID = 1227;
        usr->network_button->button_func66(&v410);

        if ( usr->field_1C86 )
            v395.field_4 = usr->field_0x1c88;
        else
            v395.field_4 = " ";

        v395.butID = 1226;
        v395.field_8 = 0;

        usr->network_button->button_func71(&v395);

        if ( usr->field_0x1cd8 )
        {
            v410.butID = 1205;
            usr->network_button->button_func66(&v410);
        }

        if ( usr->field_1CE9 )
        {
            if ( !usr->field_0x1cd8 )
            {
                v410.butID = 1205;
                usr->network_button->button_func67(&v410);
            }
        }

        v410.butID = 1220;
        usr->network_button->button_func66(&v410);

        if ( usr->field_1C86 > 0 && usr->field_1C86 < 256 )
        {

            if ( !usr->field_0x1cd8 && usr->field_1CE9 )
            {
                v410.butID = 1220;
                usr->network_button->button_func67(&v410);
            }
            else
            {
                v410.butID = 1206;
                if ( usr->p_ypaworld->LevelNet->mapInfos[ usr->field_1C86 ].fractions_mask & 2 )
                    usr->network_button->button_func66(&v410);
                else
                    usr->network_button->button_func67(&v410);

                v410.butID = 1207;
                if ( usr->p_ypaworld->LevelNet->mapInfos[ usr->field_1C86 ].fractions_mask & 0x40 )
                    usr->network_button->button_func66(&v410);
                else
                    usr->network_button->button_func67(&v410);

                v410.butID = 1208;
                if ( usr->p_ypaworld->LevelNet->mapInfos[ usr->field_1C86 ].fractions_mask & 8 )
                    usr->network_button->button_func66(&v410);
                else
                    usr->network_button->button_func67(&v410);

                v410.butID = 1209;
                if ( usr->p_ypaworld->LevelNet->mapInfos[ usr->field_1C86 ].fractions_mask & 0x10 )
                    usr->network_button->button_func66(&v410);
                else
                    usr->network_button->button_func67(&v410);
            }

            v408.butID = 0;

            switch ( usr->field_1CD6 - 1 )
            {
            case 0:
                v408.butID = 1206;
                break;
            case 1:
                v408.butID = 1207;
                break;
            case 3:
                v408.butID = 1208;
                break;
            case 7:
                v408.butID = 1209;
                break;
            case 2:
            case 4:
            case 5:
            case 6:
                break;
            }

            if ( v408.butID )
            {
                v408.field_4 = 1;
                usr->network_button->button_func73(&v408);
            }

            int v298 = 0;

            v368.field_0 = 0;
            v368.field_4 = 0;

            usr->field_1CD7 = 1;
            usr->field_1CE8 = 1;

            if ( !(usr->p_ypaworld->LevelNet->mapInfos[usr->field_1C86].fractions_mask & 2) )
                v298 = 1;

            if ( !(usr->p_ypaworld->LevelNet->mapInfos[usr->field_1C86].fractions_mask & 0x40) )
                v298 |= 2;

            if ( !(usr->p_ypaworld->LevelNet->mapInfos[usr->field_1C86].fractions_mask & 8) )
                v298 |= 4;

            if ( !(usr->p_ypaworld->LevelNet->mapInfos[usr->field_1C86].fractions_mask & 0x10) )
                v298 |= 8;

            int v373, v374, v375, v376;

            while ( usr->p_ypaworld->windp->windp_func79(&v368) )
            {
                int v299;

                if ( v368.field_C & 1 )
                    v299 = usr->field_1CD6;
                else
                    v299 = usr->netTP1[v368.field_4].field_40;

                if ( v299 & v298 )
                {
                    usr->netTP1[v368.field_4].field_41 = 1;
                    usr->field_1CD7 = 0;

                    switch ( v299 - 1 )
                    {
                    case 0:
                        usr->netTP1[v375].field_41 = 1;
                        break;
                    case 1:
                        usr->netTP1[v373].field_41 = 1;
                        break;
                    case 3:
                        usr->netTP1[v376].field_41 = 1;
                        break;
                    case 7:
                        usr->netTP1[v374].field_41 = 1;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    usr->netTP1[v368.field_4].field_41 = 0;

                    switch ( v299 - 1 )
                    {
                    case 0:
                        v375 = v368.field_4;
                        break;
                    case 1:
                        v373 = v368.field_4;
                        break;
                    case 3:
                        v376 = v368.field_4;
                        break;
                    case 7:
                        v374 = v368.field_4;
                        break;
                    default:
                        break;
                    }

                }

                v298 |= v299;
                v368.field_4++;
            }
        }

        v368.field_0 = 0;
        v368.field_4 = 0;

        while ( usr->p_ypaworld->windp->windp_func79(&v368) )
        {
            if ( !(v368.field_C & 1) )
            {
                if ( !usr->netTP1[v368.field_4].field_43 )
                    usr->field_1CD7 = 0;

                if ( !usr->netTP1[v368.field_4].field_44 )
                    usr->field_1CE8 = 0;
            }
            v368.field_4++;
        }

        if ( usr->field_0x1cd8 )
        {
            v395.butID = 1204;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 414, "START GAME OR ENTER MESSAGE TO THE PLAYERS");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);

            v395.butID = 1222;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 437, "14");
            usr->network_button->button_func71(&v395);

            v395.butID = 1223;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 438, "15");
            usr->network_button->button_func71(&v395);

            v395.butID = 1201;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 407, "START");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);

            if ( !usr->field_1CD7 )
            {
                v410.field_4 = 0;
                v410.butID = 1201;
                usr->network_button->button_func67(&v410);
            }
        }
        else
        {
            v395.butID = 1204;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 415, "WAIT FOR START OR SEND MESSAGES");
            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);

            v395.butID = 1222;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 440, "17");
            usr->network_button->button_func71(&v395);

            v395.butID = 1223;
            v395.field_4 = get_lang_string(ypaworld__string_pointers, 441, "18");
            usr->network_button->button_func71(&v395);

            if ( usr->field_1CE8 )
            {
                v410.butID = 1219;
                usr->network_button->button_func66(&v410);

                v410.butID = 1221;
                usr->network_button->button_func66(&v410);
            }

            v410.butID = 1201;
            usr->network_button->button_func67(&v410);
        }
        v410.butID = 1210;
        usr->network_button->button_func66(&v410);

        v410.butID = 1211;
        usr->network_button->button_func66(&v410);

        v410.butID = 1212;
        usr->network_button->button_func66(&v410);

        v410.butID = 1213;
        usr->network_button->button_func66(&v410);

        v410.butID = 1214;
        usr->network_button->button_func66(&v410);

        v410.butID = 1215;
        usr->network_button->button_func66(&v410);

        v410.butID = 1216;
        usr->network_button->button_func66(&v410);

        v410.butID = 1217;
        usr->network_button->button_func66(&v410);

        v368.field_4 = 0;
        v368.field_0 = 0;


        for (int i = 0; i < 4; i++)
        {
            char v339[12];
            int v370;

            int v304 = yw->windp->windp_func79(&v368);

            switch ( i )
            {
            case 0:
                v370 = 1214;
                v395.butID = 1210;
                if ( v304 )
                    v395.field_4 = v368.field_8;
                else
                    v395.field_4 = " ";
                break;

            case 1:
                v370 = 1215;
                v395.butID = 1211;
                if ( v304 )
                    v395.field_4 = v368.field_8;
                else
                    v395.field_4 = " ";
                break;

            case 2:
                v370 = 1216;
                v395.butID = 1212;
                if ( v304 )
                    v395.field_4 = v368.field_8;
                else
                    v395.field_4 = " ";
                break;

            case 3:
                v370 = 1217;
                v395.butID = 1213;
                if ( v304 )
                    v395.field_4 = v368.field_8;
                else
                    v395.field_4 = " ";
                break;

            default:
                break;
            }

            v395.field_8 = 0;
            usr->network_button->button_func71(&v395);

            if ( v304 )
            {
                int v305;

                if ( v368.field_C & 1 )
                {
                    v305 = usr->field_1CD6;
                }
                else
                {
                    v305 = usr->netTP1[v368.field_4].field_40;
                }

                switch ( v305 - 1 )
                {
                case 0:
                    v339[0] = 'P';
                    break;
                case 1:
                    v339[0] = 'R';
                    break;
                case 3:
                    v339[0] = 'T';
                    break;
                case 7:
                    v339[0] = 'V';
                    break;
                default:
                    v339[0] = ' ';
                    break;
                }
                if ( usr->netTP1[v368.field_4].field_41 && ((usr->field_42 / 300) & 1) )
                    v339[1] = 'f';
                else
                    v339[1] = ' ';

                if ( usr->netTP1[v368.field_4].field_43 )
                    v339[2] = 'h';
                else
                    v339[2] = ' ';

                if ( usr->netTP1[v368.field_4].field_44 & 0xFF00 )
                    v339[3] = 'i';
                else
                    v339[3] = ' ';

                v339[4] = ' ';
                v339[5] = 0;
            }
            else
            {
                strcpy(v339, "     ");
            }

            v395.butID = v370;
            v395.field_4 = v339;
            usr->network_button->button_func71(&v395);

            v368.field_4++;
        }
        break;

    default:
        break;
    }

}
