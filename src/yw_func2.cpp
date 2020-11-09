#include <inttypes.h>
#include <string.h>

#include "includes.h"
#include "yw.h"
#include "yw_internal.h"
#include "font.h"
#include "button.h"

#include "yw_net.h"
#include "windp.h"

#include <math.h>

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

void sb_0x4eb94c__sub0(NC_STACK_ypaworld *yw, bool clockwise, int a3, vec3d *pos, baseRender_msg *arg)
{
    //brf_obj *brobj = &yw->brief.brf_objs + obj_id; // Only one object

    NC_STACK_base *model_base = yw->vhcls_models[ yw->VhclProtos[ yw->brief.ViewingObject.ID ].vp_normal ].base;

    model_base->setBASE_visLimit(16000);
    model_base->setBASE_fadeLength(100);

    flag_xyz tmp;
    tmp.flag = 7;
    tmp.v = *pos;

    model_base->base_func68(&tmp);

    if (clockwise)
    {
        yw->brief.ViewingObjectAngle += (arg->frameTime / 5);
        if ( yw->brief.ViewingObjectAngle >= 360 )
            yw->brief.ViewingObjectAngle -= 360;
    }
    else
    {
        yw->brief.ViewingObjectAngle -= (arg->frameTime / 5);
        if ( yw->brief.ViewingObjectAngle < 0 )
            yw->brief.ViewingObjectAngle += 360;
    }

    flag_xyz2 rot;
    rot.flag = 7;
    rot.x = a3 + 10;
    rot.y = yw->brief.ViewingObjectAngle;
    rot.z = 0;

    model_base->base_func70(&rot);
    //printf("Try DRAW %d\n", (int)model_base);
    model_base->base_func77(arg); //Draw vehicle
}

void sb_0x4eb94c__sub1(NC_STACK_ypaworld *yw, bool clockwise, int rot, vec3d *pos, baseRender_msg *arg)
{
    secType *scType = &yw->secTypes[yw->brief.ViewingObject.ID];

    NC_STACK_base *v7 = yw->vhcls_models[0].base;

    if (clockwise)
    {
        yw->brief.ViewingObjectAngle += (arg->frameTime / 5);
        if ( yw->brief.ViewingObjectAngle >= 360 )
            yw->brief.ViewingObjectAngle -= 360;
    }
    else
    {
        yw->brief.ViewingObjectAngle -= (arg->frameTime / 5);
        if ( yw->brief.ViewingObjectAngle < 0 )
            yw->brief.ViewingObjectAngle += 360;
    }

    flag_xyz2 v17;
    v17.flag = 7;
    v17.x = rot + 10;
    v17.y = yw->brief.ViewingObjectAngle;
    v17.z = 0;

    v7->base_func70(&v17);

    TFEngine::TForm3D *p3d = v7->getBASE_pTransform();

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
            vec3d v13 = vec3d(v30, 0.0, v22) * 300.0;

            flag_xyz v16;
            v16.flag = 7;
            v16.v = *pos + p3d->locSclRot.Transform( v13 );

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

void sb_0x4eb94c(NC_STACK_ypaworld *yw, BriefengScreen *brf, InputState *struc, int a5)
{
    brf->ObjRenderParams.frameTime = struc->Period;
    brf->ObjRenderParams.globTime = brf->CurrTime;
    brf->ObjRenderParams.ownerID = 1;

    TFEngine::TForm3D v14;
    memset(&v14, 0, sizeof(TFEngine::TForm3D));
    v14.scale = vec3d(1.0, 1.0, 1.0);
    v14.locSclRot = mat3x3::Ident();

    TFEngine::Engine.SetViewPoint(&v14);
    v14.CalcGlobal();

    vec3d pos;

    if ( brf->ViewingObject ) // Not none
    {
        pos.x = (brf->ViewingObjectRect.x2 + brf->ViewingObjectRect.x1) / 2.0;
        pos.y = (brf->ViewingObjectRect.y2 + brf->ViewingObjectRect.y1) / 2.0;

        float v16;
        float v17;
        float v18;
        int rot;

        if ( brf->ViewingObject.ObjType == BriefObject::TYPE_SECTOR )
        {
            v16 = 9600.0;
            v17 = 3600.0;
        }
        else if ( brf->ViewingObject.ObjType == BriefObject::TYPE_VEHICLE )
        {
            float radius = yw->VhclProtos[brf->ViewingObject.ID].radius;

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

        pos.z = v18;
        pos.y = pos.y * v18;
        pos.x = pos.x * v18;

        if ( brf->ViewingObject.ObjType == BriefObject::TYPE_SECTOR )
        {
            sb_0x4eb94c__sub1(yw, true, rot, &pos, &brf->ObjRenderParams);
        }
        else if ( brf->ViewingObject.ObjType == BriefObject::TYPE_VEHICLE )
        {
            sb_0x4eb94c__sub0(yw, true, rot, &pos, &brf->ObjRenderParams);
        }
    }
}

void ypaworld_func158__DrawVehicle(NC_STACK_ypaworld *yw, BriefengScreen *brf, InputState *struc)
{
    yw->_win3d->BeginScene();

    brf->ObjRenderParams.frameTime = 1;
    brf->ObjRenderParams.globTime = 1;
    brf->ObjRenderParams.adeCount = 0;
    brf->ObjRenderParams.ownerID = 1;
    brf->ObjRenderParams.minZ = 17.0;
    brf->ObjRenderParams.maxZ = 32000.0;
    brf->ObjRenderParams.flags = NC_STACK_display::RFLAGS_IGNORE_FALLOFF;
    brf->ObjRenderParams.rndrStack = &NC_STACK_base::renderStack;

    if ( brf->ViewingObject ) // Not none
    {
        int v7 = brf->CurrTime - brf->ViewingObjectStartTime;
        if ( v7 > 50 )
            sb_0x4eb94c(yw, brf, struc, v7 - 50);
    }

    brf->ObjRenderParams.rndrStack->render();

    yw->_win3d->EndScene();
}



void yw_draw_input_list(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->input_listview.SetRect(yw, -2, -2);
    GFXEngine::GFXe.getTileset(0);

    char *v4 = usr->input_listview.ItemsPreLayout(yw, usr->input_listview.itemBlock, 0, "uvw");

    for (int i = 1; i <= usr->input_listview.shownEntries; i++ )
    {
        int v24 = i + usr->input_listview.firstShownEntries;
        if ( !usr->InputConfigTitle[v24].empty() )
        {
            FontUA::ColumnItem a1a[2];

            int v33;
            int v31;
            int v32;
            int v30;

            if ( (size_t)v24 == usr->field_D36 )
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

            int v34 = usr->input_listview.entryWidth - 2 * usr->p_ypaworld->font_default_w__b + 1;

            std::string v19;

            if ( usr->InputConfig[ v24 ].Type == World::INPUT_BIND_TYPE_SLIDER )
            {
                std::string negKeyName, posKeyName;
                if ( usr->InputConfig[ v24 ].NKeyCode )
                    negKeyName = NC_STACK_input::KeyTitle.at( usr->InputConfig[ v24 ].NKeyCode );
                else
                    negKeyName = "-";

                if ( usr->InputConfig[ v24 ].PKeyCode )
                    posKeyName = NC_STACK_input::KeyTitle.at( usr->InputConfig[ v24 ].PKeyCode );
                else
                    posKeyName = "-";

                if ( usr->InputConfig[ v24 ].SetFlags & 1 )
                    posKeyName = get_lang_string(ypaworld__string_pointers, 308, "?");

                if ( usr->InputConfig[ v24 ].SetFlags & 2 )
                    negKeyName = get_lang_string(ypaworld__string_pointers, 308, "?");

                v19 = fmt::sprintf("%s/%s", negKeyName, posKeyName);
            }
            else
            {
                if ( usr->InputConfig[ v24 ].PKeyCode )
                    v19 = NC_STACK_input::KeyTitle.at( usr->InputConfig[ v24 ].PKeyCode );
                else
                    v19 = "-";

                if ( usr->InputConfig[ v24 ].SetFlags & 1 )
                    v19 = get_lang_string(ypaworld__string_pointers, 308, "?");
            }

            a1a[0].txt = usr->InputConfigTitle[v24];
            a1a[0].width = v34 * 0.68;
            a1a[0].fontID = v32;
            a1a[0].spaceChar = v33;
            a1a[0].prefixChar = v30;
            a1a[0].postfixChar = 0;
            a1a[0].flags = 37;

            a1a[1].txt = v19;
            a1a[1].width = v34 - v34 * 0.68;
            a1a[1].fontID = v32;
            a1a[1].spaceChar = v33;
            a1a[1].prefixChar = 0;
            a1a[1].postfixChar = v31;
            a1a[1].flags = 38;

            FontUA::select_tileset(&v4, 0);
            FontUA::store_s8(&v4, '{'); // Left wnd border

            if ( (size_t)v24 == usr->field_D36 )
            {
                FontUA::set_txtColor(&v4, usr->p_ypaworld->iniColors[62].r, usr->p_ypaworld->iniColors[62].g, usr->p_ypaworld->iniColors[62].b);
            }
            else
            {
                FontUA::set_txtColor(&v4, usr->p_ypaworld->iniColors[61].r, usr->p_ypaworld->iniColors[61].g, usr->p_ypaworld->iniColors[61].b);
            }

            v4 = FormateColumnItem(yw, v4, 2, a1a);

            FontUA::select_tileset(&v4, 0);
            FontUA::store_s8(&v4, '}'); // Right wnd border
            FontUA::next_line(&v4);
        }
    }
    v4 = usr->input_listview.ItemsPostLayout(yw, v4, 0, "xyz");
    FontUA::set_end(&v4);

    w3d_a209 v21;
    v21 = usr->input_listview.cmdstrm;

    GFXEngine::GFXe.drawText(&v21);
}


void NC_STACK_ypaworld::LoadKeyNames()
{
    for (std::string &a: NC_STACK_input::KeyTitle)
        a.clear();

    NC_STACK_input::KeyTitle[Input::KC_NONE]       = "*";
    NC_STACK_input::KeyTitle[Input::KC_ESCAPE]     = get_lang_string(string_pointers_p2, 1001, "ESC");
    NC_STACK_input::KeyTitle[Input::KC_SPACE]      = get_lang_string(string_pointers_p2, 1002, "SPACE");
    NC_STACK_input::KeyTitle[Input::KC_UP]         = get_lang_string(string_pointers_p2, 1003, "UP");
    NC_STACK_input::KeyTitle[Input::KC_DOWN]       = get_lang_string(string_pointers_p2, 1004, "DOWN");
    NC_STACK_input::KeyTitle[Input::KC_LEFT]       = get_lang_string(string_pointers_p2, 1005, "LEFT");
    NC_STACK_input::KeyTitle[Input::KC_RIGHT]      = get_lang_string(string_pointers_p2, 1006, "RIGHT");
    NC_STACK_input::KeyTitle[Input::KC_F1]         = get_lang_string(string_pointers_p2, 1007, "F1");
    NC_STACK_input::KeyTitle[Input::KC_F2]         = get_lang_string(string_pointers_p2, 1008, "F2");
    NC_STACK_input::KeyTitle[Input::KC_F3]         = get_lang_string(string_pointers_p2, 1009, "F3");
    NC_STACK_input::KeyTitle[Input::KC_F4]         = get_lang_string(string_pointers_p2, 1010, "F4");
    NC_STACK_input::KeyTitle[Input::KC_F5]         = get_lang_string(string_pointers_p2, 1011, "F5");
    NC_STACK_input::KeyTitle[Input::KC_F6]         = get_lang_string(string_pointers_p2, 1012, "F6");
    NC_STACK_input::KeyTitle[Input::KC_F7]         = get_lang_string(string_pointers_p2, 1013, "F7");
    NC_STACK_input::KeyTitle[Input::KC_F8]         = get_lang_string(string_pointers_p2, 1014, "F8");
    NC_STACK_input::KeyTitle[Input::KC_F9]         = get_lang_string(string_pointers_p2, 1015, "F9");
    NC_STACK_input::KeyTitle[Input::KC_F10]        = get_lang_string(string_pointers_p2, 1016, "F10");
    NC_STACK_input::KeyTitle[Input::KC_F11]        = get_lang_string(string_pointers_p2, 1017, "F11");
    NC_STACK_input::KeyTitle[Input::KC_F12]        = get_lang_string(string_pointers_p2, 1018, "F12");
    NC_STACK_input::KeyTitle[Input::KC_BACKSPACE]  = get_lang_string(string_pointers_p2, 1019, "BACK");
    NC_STACK_input::KeyTitle[Input::KC_TAB]        = get_lang_string(string_pointers_p2, 1020, "TAB");
    NC_STACK_input::KeyTitle[Input::KC_CLEAR]      = get_lang_string(string_pointers_p2, 1021, "CLEAR");
    NC_STACK_input::KeyTitle[Input::KC_RETURN]     = get_lang_string(string_pointers_p2, 1022, "RETURN");
    NC_STACK_input::KeyTitle[Input::KC_CTRL]       = get_lang_string(string_pointers_p2, 1023, "CTRL");
    NC_STACK_input::KeyTitle[Input::KC_SHIFT]      = get_lang_string(string_pointers_p2, 1024, "SHIFT");
    NC_STACK_input::KeyTitle[Input::KC_ALT]        = get_lang_string(string_pointers_p2, 1025, "ALT");
    NC_STACK_input::KeyTitle[Input::KC_PAUSE]      = get_lang_string(string_pointers_p2, 1026, "PAUSE");
    NC_STACK_input::KeyTitle[Input::KC_PGUP]       = get_lang_string(string_pointers_p2, 1027, "PGUP");
    NC_STACK_input::KeyTitle[Input::KC_PGDOWN]     = get_lang_string(string_pointers_p2, 1028, "PGDOWN");
    NC_STACK_input::KeyTitle[Input::KC_END]        = get_lang_string(string_pointers_p2, 1029, "END");
    NC_STACK_input::KeyTitle[Input::KC_HOME]       = get_lang_string(string_pointers_p2, 1030, "HOME");
    NC_STACK_input::KeyTitle[Input::KC_SELECT]     = get_lang_string(string_pointers_p2, 1031, "SELECT");
    NC_STACK_input::KeyTitle[Input::KC_EXECUTE]    = get_lang_string(string_pointers_p2, 1032, "EXEC");
    NC_STACK_input::KeyTitle[Input::KC_SNAPSHOT]   = get_lang_string(string_pointers_p2, 1033, "PRINT");
    NC_STACK_input::KeyTitle[Input::KC_INSERT]     = get_lang_string(string_pointers_p2, 1034, "INS");
    NC_STACK_input::KeyTitle[Input::KC_DELETE]     = get_lang_string(string_pointers_p2, 1035, "DEL");
    NC_STACK_input::KeyTitle[Input::KC_HELP]       = get_lang_string(string_pointers_p2, 1036, "HELP");
    NC_STACK_input::KeyTitle[Input::KC_1]          = get_lang_string(string_pointers_p2, 1037, "1");
    NC_STACK_input::KeyTitle[Input::KC_2]          = get_lang_string(string_pointers_p2, 1038, "2");
    NC_STACK_input::KeyTitle[Input::KC_3]          = get_lang_string(string_pointers_p2, 1039, "3");
    NC_STACK_input::KeyTitle[Input::KC_4]          = get_lang_string(string_pointers_p2, 1040, "4");
    NC_STACK_input::KeyTitle[Input::KC_5]          = get_lang_string(string_pointers_p2, 1041, "5");
    NC_STACK_input::KeyTitle[Input::KC_6]          = get_lang_string(string_pointers_p2, 1042, "6");
    NC_STACK_input::KeyTitle[Input::KC_7]          = get_lang_string(string_pointers_p2, 1043, "7");
    NC_STACK_input::KeyTitle[Input::KC_8]          = get_lang_string(string_pointers_p2, 1044, "8");
    NC_STACK_input::KeyTitle[Input::KC_9]          = get_lang_string(string_pointers_p2, 1045, "9");
    NC_STACK_input::KeyTitle[Input::KC_0]          = get_lang_string(string_pointers_p2, 1046, "0");
    NC_STACK_input::KeyTitle[Input::KC_A]          = get_lang_string(string_pointers_p2, 1047, "A");
    NC_STACK_input::KeyTitle[Input::KC_B]          = get_lang_string(string_pointers_p2, 1048, "B");
    NC_STACK_input::KeyTitle[Input::KC_C]          = get_lang_string(string_pointers_p2, 1049, "C");
    NC_STACK_input::KeyTitle[Input::KC_D]          = get_lang_string(string_pointers_p2, 1050, "D");
    NC_STACK_input::KeyTitle[Input::KC_E]          = get_lang_string(string_pointers_p2, 1051, "E");
    NC_STACK_input::KeyTitle[Input::KC_F]          = get_lang_string(string_pointers_p2, 1052, "F");
    NC_STACK_input::KeyTitle[Input::KC_G]          = get_lang_string(string_pointers_p2, 1053, "G");
    NC_STACK_input::KeyTitle[Input::KC_H]          = get_lang_string(string_pointers_p2, 1054, "H");
    NC_STACK_input::KeyTitle[Input::KC_I]          = get_lang_string(string_pointers_p2, 1055, "I");
    NC_STACK_input::KeyTitle[Input::KC_J]          = get_lang_string(string_pointers_p2, 1056, "J");
    NC_STACK_input::KeyTitle[Input::KC_K]          = get_lang_string(string_pointers_p2, 1057, "K");
    NC_STACK_input::KeyTitle[Input::KC_L]          = get_lang_string(string_pointers_p2, 1058, "L");
    NC_STACK_input::KeyTitle[Input::KC_M]          = get_lang_string(string_pointers_p2, 1059, "M");
    NC_STACK_input::KeyTitle[Input::KC_N]          = get_lang_string(string_pointers_p2, 1060, "N");
    NC_STACK_input::KeyTitle[Input::KC_O]          = get_lang_string(string_pointers_p2, 1061, "O");
    NC_STACK_input::KeyTitle[Input::KC_P]          = get_lang_string(string_pointers_p2, 1062, "P");
    NC_STACK_input::KeyTitle[Input::KC_Q]          = get_lang_string(string_pointers_p2, 1063, "Q");
    NC_STACK_input::KeyTitle[Input::KC_R]          = get_lang_string(string_pointers_p2, 1064, "R");
    NC_STACK_input::KeyTitle[Input::KC_S]          = get_lang_string(string_pointers_p2, 1065, "S");
    NC_STACK_input::KeyTitle[Input::KC_T]          = get_lang_string(string_pointers_p2, 1066, "T");
    NC_STACK_input::KeyTitle[Input::KC_U]          = get_lang_string(string_pointers_p2, 1067, "U");
    NC_STACK_input::KeyTitle[Input::KC_V]          = get_lang_string(string_pointers_p2, 1068, "V");
    NC_STACK_input::KeyTitle[Input::KC_W]          = get_lang_string(string_pointers_p2, 1069, "W");
    NC_STACK_input::KeyTitle[Input::KC_X]          = get_lang_string(string_pointers_p2, 1070, "X");
    NC_STACK_input::KeyTitle[Input::KC_Y]          = get_lang_string(string_pointers_p2, 1071, "Y");
    NC_STACK_input::KeyTitle[Input::KC_Z]          = get_lang_string(string_pointers_p2, 1072, "Z");
    NC_STACK_input::KeyTitle[Input::KC_NUM0]       = get_lang_string(string_pointers_p2, 1073, "NUM 0");
    NC_STACK_input::KeyTitle[Input::KC_NUM1]       = get_lang_string(string_pointers_p2, 1074, "NUM 1");
    NC_STACK_input::KeyTitle[Input::KC_NUM2]       = get_lang_string(string_pointers_p2, 1075, "NUM 2");
    NC_STACK_input::KeyTitle[Input::KC_NUM3]       = get_lang_string(string_pointers_p2, 1076, "NUM 3");
    NC_STACK_input::KeyTitle[Input::KC_NUM4]       = get_lang_string(string_pointers_p2, 1077, "NUM 4");
    NC_STACK_input::KeyTitle[Input::KC_NUM5]       = get_lang_string(string_pointers_p2, 1078, "NUM 5");
    NC_STACK_input::KeyTitle[Input::KC_NUM6]       = get_lang_string(string_pointers_p2, 1079, "NUM 6");
    NC_STACK_input::KeyTitle[Input::KC_NUM7]       = get_lang_string(string_pointers_p2, 1080, "NUM 7");
    NC_STACK_input::KeyTitle[Input::KC_NUM8]       = get_lang_string(string_pointers_p2, 1081, "NUM 8");
    NC_STACK_input::KeyTitle[Input::KC_NUM9]       = get_lang_string(string_pointers_p2, 1082, "NUM 9");
    NC_STACK_input::KeyTitle[Input::KC_NUMMUL]     = get_lang_string(string_pointers_p2, 1083, "MUL");
    NC_STACK_input::KeyTitle[Input::KC_NUMPLUS]    = get_lang_string(string_pointers_p2, 1084, "ADD");
    NC_STACK_input::KeyTitle[Input::KC_NUMDOT]     = get_lang_string(string_pointers_p2, 1085, "DOT");
    NC_STACK_input::KeyTitle[Input::KC_NUMMINUS]   = get_lang_string(string_pointers_p2, 1086, "SUB");
    NC_STACK_input::KeyTitle[Input::KC_NUMENTER]   = get_lang_string(string_pointers_p2, 1087, "ENTER");
    NC_STACK_input::KeyTitle[Input::KC_NUMDIV]     = get_lang_string(string_pointers_p2, 1088, "DIV");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA1]     = get_lang_string(string_pointers_p2, 1089, "EXTRA_1");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA2]     = get_lang_string(string_pointers_p2, 1090, "EXTRA_2");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA3]     = get_lang_string(string_pointers_p2, 1091, "EXTRA_3");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA4]     = get_lang_string(string_pointers_p2, 1092, "EXTRA_4");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA5]     = get_lang_string(string_pointers_p2, 1093, "EXTRA_5");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA6]     = get_lang_string(string_pointers_p2, 1094, "EXTRA_6");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA7]     = get_lang_string(string_pointers_p2, 1095, "EXTRA_7");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA8]     = get_lang_string(string_pointers_p2, 1096, "EXTRA_8");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA9]     = get_lang_string(string_pointers_p2, 1097, "EXTRA_9");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA10]    = get_lang_string(string_pointers_p2, 1098, "EXTRA_10");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA11]    = get_lang_string(string_pointers_p2, 1099, "EXTRA_11");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA12]    = get_lang_string(string_pointers_p2, 1100, "EXTRA_12");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA13]    = get_lang_string(string_pointers_p2, 1101, "EXTRA_13");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA14]    = get_lang_string(string_pointers_p2, 1102, "EXTRA_14");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA15]    = get_lang_string(string_pointers_p2, 1103, "EXTRA_15");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA16]    = get_lang_string(string_pointers_p2, 1104, "EXTRA_16");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA17]    = get_lang_string(string_pointers_p2, 1105, "EXTRA_17");
    NC_STACK_input::KeyTitle[Input::KC_EXTRA18]    = get_lang_string(string_pointers_p2, 1106, "EXTRA_18");
    
    NC_STACK_input::KeyTitle[Input::KC_MMB]        = get_lang_string(string_pointers_p2, 1121, "MIDDLE MOUSE");

    NC_STACK_input::KeyTitle[Input::KC_JOYB0]      = get_lang_string(string_pointers_p2, 1123, "JOYB0");
    NC_STACK_input::KeyTitle[Input::KC_JOYB1]      = get_lang_string(string_pointers_p2, 1124, "JOYB1");
    NC_STACK_input::KeyTitle[Input::KC_JOYB2]      = get_lang_string(string_pointers_p2, 1125, "JOYB2");
    NC_STACK_input::KeyTitle[Input::KC_JOYB3]      = get_lang_string(string_pointers_p2, 1126, "JOYB3");
    NC_STACK_input::KeyTitle[Input::KC_JOYB4]      = get_lang_string(string_pointers_p2, 1127, "JOYB4");
    NC_STACK_input::KeyTitle[Input::KC_JOYB5]      = get_lang_string(string_pointers_p2, 1128, "JOYB5");
    NC_STACK_input::KeyTitle[Input::KC_JOYB6]      = get_lang_string(string_pointers_p2, 1129, "JOYB6");
    NC_STACK_input::KeyTitle[Input::KC_JOYB7]      = get_lang_string(string_pointers_p2, 1130, "JOYB7");
}


int yw_loadSky(NC_STACK_ypaworld *yw, const std::string &skyname)
{
    std::string skyfilename = fmt::sprintf("data:%s", skyname);

    NC_STACK_base *sky = NC_STACK_base::READ_BAS_FILE(skyfilename);
    yw->sky_loaded_base = sky;
    if ( !sky )
    {
        ypa_log_out("Couldn't create %s\n", skyfilename.c_str());
        return 0;
    }

    sky->setBASE_static(1); // Don't rotate sky
    sky->setBASE_visLimit(yw->field_15ec);
    sky->setBASE_fadeLength(yw->field_15f0);
    return 1;
}

void fill_videmodes_list(UserData *usr)
{
    usr->video_mode_list.clear();

    usr->p_ypaworld->_win3d = GFXEngine::GFXe.getC3D();

    windd_arg256 warg_256;
    warg_256.sort_id = 0;

    int id = -1;

    while( id )
    {
        id = usr->p_ypaworld->_win3d->display_func256(&warg_256);

        usr->video_mode_list.emplace_back();
        video_mode_node &vnode = usr->video_mode_list.back();

        vnode.sort_id = warg_256.sort_id;
        vnode.width = warg_256.width;
        vnode.height = warg_256.height;
        vnode.name = warg_256.name;

        warg_256.sort_id = id;
    }
}


void NC_STACK_ypaworld::listSaveDir(const std::string &saveDir)
{
    auto savedStatuses = playerstatus;
    auto savedCallsign = GameShell->callSIGN;
    auto savedMaxroboenrgy = _maxRoboEnergy;
    auto savedMaxreloadconst = _maxReloadConst;

    FSMgr::DirIter dir = uaOpenDir(saveDir);
    if ( dir )
    {
        FSMgr::iNode *dirNode;
        while ( dir.getNext(&dirNode) )
        {
            if ( dirNode->getType() == FSMgr::iNode::NTYPE_DIR )
            {
                if ( StriCmp(dirNode->getName(), ".") && StriCmp(dirNode->getName(), "..") )
                {
                    GameShell->profiles.emplace_back();
                    ProfilesNode &profile = GameShell->profiles.back();

                    profile.name = dirNode->getName();

                    ScriptParser::HandlersList hndls{
                        new World::Parsers::UserParser(this)
                    };

                    std::string buf = fmt::sprintf("%s/%s/user.txt\n", saveDir, dirNode->getName());

                    if ( !ScriptParser::ParseFile(buf, hndls,0) )
                        ypa_log_out("Warning, cannot parse %s for time scanning\n", buf.c_str());

                    profile.fraction = 1;
                    profile.totalElapsedTime = playerstatus[1].elapsedTime;
                }
            }
        }
    }
    else
    {
        ypa_log_out("Unknown Game-Directory %s\n", saveDir.c_str());
    }

    playerstatus = savedStatuses;
    _maxReloadConst = savedMaxreloadconst;
    _maxRoboEnergy = savedMaxroboenrgy;
    GameShell->callSIGN = savedCallsign;
}


void listLocaleDir(UserData *usr, const char *dirname)
{
    usr->lang_dlls_count = 0;

    std::string *deflng = NULL;
    FSMgr::DirIter dir = uaOpenDir(dirname);
    if ( dir )
    {
        FSMgr::iNode *v18;
        while ( dir.getNext(&v18) )
        {
            std::string tmp = v18->getName();
            size_t v3 = tmp.rfind(".LNG");
            if ( v3 == std::string::npos )
                v3 = tmp.rfind(".lng");;
            /*if ( !v3 )
              v3 = strstr(v18.e_name, ".dll");
            if ( !v3 )
              v3 = strstr(v18.e_name, ".DLL");*/

            if ( v18->getType() == FSMgr::iNode::NTYPE_FILE && v3 != std::string::npos )
            {
                tmp[v3] = 0;

                for (size_t i = 0; i < tmp.length(); i++)
                    tmp[i] = std::toupper(tmp[i]);

                bool finded = false;

                for(const auto &x : usr->lang_dlls)
                {
                    if ( !StriCmp( x, tmp ) )
                    {
                        finded = true;
                        break;
                    }
                }

                if ( !finded )
                {
                    usr->lang_dlls_count++;

                    usr->lang_dlls.emplace_back();
                    std::string &elm = usr->lang_dlls.back();

                    elm = tmp;

                    if ( !strcasecmp(elm.c_str(), "language") )
                        deflng = &elm;
                }
            }
        }
    }
    else
    {
        ypa_log_out("Unknown Locale-Directory %s\n", dirname);
    }

    if ( deflng )
        usr->default_lang_dll = deflng;
}

void UserData::sub_46A7F8()
{
    disk_button->Hide();

    p_YW->GuiWinClose( &disk_listvw );

    if ( field_0x1760 )
    {
        envMode = ENVMODE_SINGLEPLAY;
        sub_bar_button->Show();
    }
    else
    {
        envMode = ENVMODE_TITLE;
        titel_button->Show();
    }

    NC_STACK_button::button_66arg v3;
    v3.field_4 = 2;
    v3.butID = 1156;
    video_button->button_func73(&v3);

    field_0x1744 = 0;
}



void ypaworld_func154__sub0(NC_STACK_ypaworld *yw)
{
    if ( !yw->movies[World::MOVIE_INTRO].empty() )
    {
        yw->_win3d = GFXEngine::GFXe.getC3D();

        std::string buf;
        sub_412810(yw->movies[World::MOVIE_INTRO], buf);
        const char *v5 = buf.c_str();

        yw->_win3d->windd_func323(&v5);

        INPe.QueryInput(&input_states);
        input_states.KbdLastHit = Input::KC_NONE;
        input_states.KbdLastDown = Input::KC_NONE;
        input_states.HotKeyID = -1;
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
            usr->map_descriptions[v2].pstring = get_lang_string(ypaworld__string_pointers, i + 1800, usr->p_ypaworld->LevelNet->mapInfos[i].map_name.c_str());
            v2++;
        }
    }

    usr->map_descriptions_count = v2;

    qsort(usr->map_descriptions, v2, sizeof(usr_str), sub_4DE9F4);
}


void UserData::GameShellUiOpenNetwork()
{
    titel_button->Hide();

    network_button->Show();

    envMode = ENVMODE_NETPLAY;

    p_YW->GuiWinClose( &network_listvw );
    p_YW->GuiWinOpen( &network_listvw );
}



void sb_0x46ca74__sub0(const std::string &a1, const std::string &a2)
{
    FSMgr::FileHandle *f1 = uaOpenFile(a1, "r");
    if ( f1 )
    {
        FSMgr::FileHandle *f2 = uaOpenFile(a2, "w");

        if ( f2 )
        {
            char v9[300];

            while ( f1->gets(v9, 299) )
                f2->puts(v9);

            delete f2;
        }

        delete f1;
    }
}

void  UserData::sb_0x46ca74()
{
    std::string oldsave;

    if ( field_1612 )
    {
        if ( StriCmp(usernamedir, user_name) )
            sub_46D0F8(fmt::sprintf("save:%s", usernamedir));
    }
    else
    {
        profiles.emplace_back();
        ProfilesNode &profile = profiles.back();

        profile.name = usernamedir;

        std::string tmp = fmt::sprintf("save:%s", usernamedir);
        if ( !uaCreateDir(tmp) )
        {
            ypa_log_out("Unable to create directory %s\n", tmp.c_str());
            return;
        }

        disk_listvw.numEntries++;
        field_1612 = disk_listvw.numEntries;
    }

    oldsave = fmt::sprintf("%s/user.txt", usernamedir);

    yw_arg172 v15;
    v15.usertxt = oldsave.c_str();
    v15.field_4 = usernamedir.c_str();
    v15.usr = this;
    v15.field_8 = 255;
    v15.field_10 = 0;

    if ( ! p_YW->ypaworld_func171(&v15) )
        ypa_log_out("Warning! Error while saving user data for %s\n", usernamedir.c_str());

    oldsave = fmt::sprintf("save:%s", user_name);

    if ( StriCmp(usernamedir, user_name) )
    {
        FSMgr::DirIter dir = uaOpenDir(oldsave);
        if ( dir )
        {
            FSMgr::iNode *a2a;
            while ( dir.getNext(&a2a) )
            {
                std::string tmp = a2a->getName();
                if ( a2a->getType() == FSMgr::iNode::NTYPE_FILE
                        && (tmp.rfind(".sgm") != std::string::npos
                            || tmp.rfind(".SGM") != std::string::npos
                            || tmp.rfind(".rst") != std::string::npos
                            || tmp.rfind(".RST") != std::string::npos
                            || tmp.rfind(".fin") != std::string::npos
                            || tmp.rfind(".FIN") != std::string::npos
                            || tmp.rfind(".def") != std::string::npos
                            || tmp.rfind(".DEF") != std::string::npos) )
                {
                    std::string v11 = fmt::sprintf("%s/%s", oldsave, tmp);
                    std::string v12 = fmt::sprintf("save:%s/%s", usernamedir, tmp);
                    sb_0x46ca74__sub0(v11.c_str(), v12.c_str());
                }
            }
        }
    }

    field_0x1744 = 0;
    user_name = usernamedir;

    disk_button->Hide();

    p_YW->GuiWinClose( &disk_listvw );

    if ( field_0x1760 )
    {
        envMode = ENVMODE_SINGLEPLAY;

        sub_bar_button->Show();
    }
    else
    {
        envMode = ENVMODE_TITLE;

        titel_button->Show();
    }
}

void sb_0x47f810__sub0(NC_STACK_ypaworld *yw)
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

void sb_0x47f810(NC_STACK_ypaworld *yw)
{
    sb_0x47f810__sub0(yw);

    yw->RoboProtos.clear();
    yw->BuildProtos.clear();

    if ( yw->WeaponProtos )
    {
        delete[] yw->WeaponProtos;
        yw->WeaponProtos = NULL;
    }

    if ( yw->VhclProtos )
    {
        delete[] yw->VhclProtos;
        yw->VhclProtos = NULL;
    }
}

void sub_44A1FC(NC_STACK_ypaworld *yw)
{
    int v2 = 0;

    if ( yw->GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFile("env:levels.def", "r");

        if ( fil )
        {
            std::string line;
            if ( fil->ReadLine(&line) )
            {
                Stok parse(line, "\t ,");
                std::string token;
                while( parse.GetNext(&token) )
                {
                    uint32_t tmp = std::stol(token, 0, 10);

                    if (tmp < 256)
                        yw->LevelNet->mapInfos[tmp].field_0 = 2;
                }
            }

            v2 = 1;
            delete fil;
        }
    }

    if ( !v2 )
    {
        yw->LevelNet->mapInfos[1].field_0 = 2;
        yw->LevelNet->mapInfos[25].field_0 = 2;
        yw->LevelNet->mapInfos[26].field_0 = 2;
        yw->LevelNet->mapInfos[27].field_0 = 2;
    }
}

void UserData::sb_0x46cdf8()
{
    std::string a1a = fmt::sprintf("%s/user.txt", user_name);

    yw_arg172 v12;
    v12.usertxt = a1a.c_str();
    v12.field_4 = user_name.c_str();
    v12.usr = this;
    v12.field_8 = 255;
    v12.field_10 = 0;

    if ( ! p_YW->ypaworld_func171(&v12) )
        ypa_log_out("Warning! Error while saving user data for %s\n", user_name.c_str());

    if ( field_1612 )
    {
        sub_46D0F8(fmt::sprintf("save:%s", usernamedir));
    }
    else
    {
        profiles.emplace_back();
        ProfilesNode &profile = profiles.back();

        profile.name = usernamedir;

        std::string v10 = fmt::sprintf("save:%s", usernamedir);

        if ( !uaCreateDir(v10) )
        {
            ypa_log_out("Unable to create directory %s\n", v10.c_str());
            return;
        }

        user_name = usernamedir;
        disk_listvw.numEntries++;
        field_1612 = disk_listvw.numEntries;
    }

    p_ypaworld->_levelInfo->Buddies.clear();

    sb_0x47f810(p_ypaworld);

    if ( init_prototypes(p_ypaworld) )
    {
        for (int i = 0; i < 256; i++)
        {
            mapINFO *mp = &p_ypaworld->LevelNet->mapInfos[i];
            if ( mp->field_0 && mp->field_0 != 4 )
                mp->field_0 = 1;
        }

        sub_44A1FC(p_ypaworld);

        p_ypaworld->_maxRoboEnergy = 0;
        p_ypaworld->_maxReloadConst = 0;

        for(auto &x : p_ypaworld->playerstatus)
			x.clear();

        field_0x1744 = 0;

        p_ypaworld->beamenergy = p_ypaworld->beam_energy_start;

        for(auto &x : p_ypaworld->_levelInfo->JodieFoster)
			x = 0;

        field_3426 = 0;

        disk_button->Hide();

        p_YW->GuiWinClose( &disk_listvw );

        envMode = ENVMODE_SINGLEPLAY;

        sub_bar_button->Show();
    }
    else
    {
        ypa_log_out("Warning, error while parsing prototypes\n");
    }
}

void UserData::sub_46D960()
{
    NC_STACK_button::button_66arg v4;
    v4.butID = 1300;
    confirm_button->button_func67(&v4);

    v4.butID = 1301;
    confirm_button->button_func67(&v4);

    confirm_button->Hide();

    field_0x2fb4 = 0;
}

void NC_STACK_ypaworld::SetFarView(bool farvw)
{
    if ( farvw )
    {
        setYW_visSectors(9);
        setYW_normVisLimit(2100);
        setYW_fadeLength(900);
    }
    else
    {
        setYW_visSectors(5);
        setYW_normVisLimit(1400);
        setYW_fadeLength(600);
    }
}

//Options OK
void UserData::sb_0x46aa8c()
{
    NC_STACK_ypaworld *yw = p_ypaworld;

    int v3 = 0;

    yw_174arg v40;
    v40.make_changes = 0;
    int v5 = 0;
    int resolution;

    if ( field_13C2 & 0x200 )
    {
        if ( field_0x13b0 & 0x10 )
        {
            snd__flags2 |= 0x10;
            yw->field_73CE |= World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(true);
            if ( shelltrack )
            {
                SFXEngine::SFXe.SetMusicTrack(shelltrack, shelltrack__adv.min_delay, shelltrack__adv.max_delay);
                SFXEngine::SFXe.PlayMusicTrack();
            }
        }
        else
        {
            snd__flags2 &= 0xEF;
            yw->field_73CE &= ~World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.StopMusicTrack();
            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(false);
        }

    }

    if ( field_13C2 & 2 )
    {
        if ( field_0x13b0 & 1 )
        {
            snd__flags2 |= 1;
            SFXEngine::SFXe.setReverseStereo(1);
        }
        else
        {
            snd__flags2 &= ~1;
            SFXEngine::SFXe.setReverseStereo(0);
        }
    }

    if ( field_13C2 & 0x10 )
    {
        if ( field_0x13a8 & 1 )
        {
            GFX_flags |= 1;
            p_YW->SetFarView(true);
        }
        else
        {
            GFX_flags &= 0xFE;
            p_YW->SetFarView(false);
        }
    }

    if ( field_13C2 & 8 )
    {
        if ( field_0x13a8 & 2 )
        {
            GFX_flags |= 2;
            yw->setYW_skyRender(1);
        }
        else
        {
            GFX_flags &= ~2;
            yw->setYW_skyRender(0);
        }

    }

    if ( field_13C2 & 0x800 )
    {
        if ( field_0x13a8 & 4 )
        {
            GFX_flags |= 4;
            yw->field_73CE |= 0x40;
            yw->_win3d->setWDD_cursor(1);
        }
        else
        {
            GFX_flags &= 0xFB;
            yw->field_73CE &= 0xBF;
            yw->_win3d->setWDD_cursor(0);
        }

    }

    if ( field_13C2 & 0x20 )
    {
        enemyindicator = field_13BE;

        if ( enemyindicator )
            p_ypaworld->field_73CE |= 0x20;
        else
            p_ypaworld->field_73CE &= 0xDF;
    }

    if ( field_13C2 & 0x40 )
    {
        fxnumber = field_0x13a4;
        yw->fxnumber = fxnumber;
    }

    if ( field_13C2 & 0x100 )
    {
        snd__cdvolume = field_0x13b8;

        SFXEngine::SFXe.SetMusicVolume(field_0x13b8);
    }

    if ( field_13C2 & 0x80 )
    {
        snd__volume = field_0x13b4;
        SFXEngine::SFXe.setMasterVolume(snd__volume);
    }

    if ( field_13C2 & 1 )
    {
        if ( game_default_res != yw->game_default_res && game_default_res )
        {
            v3 = 1;
            resolution = yw->game_default_res;

            yw->game_default_res = game_default_res;
        }
    }

    if ( field_13C2 & 0x1000 )
    {
        if ( field_139A )
        {
            if ( strcasecmp(field_139A, win3d_guid) )
            {
                strcpy(win3d_name, field_139E);

                strcpy(win3d_guid, field_139A);

                wdd_func324arg v37;

                v37.name = win3d_name;
                v37.guid = win3d_guid;
                v37.currr = 0;

                yw->_win3d->windd_func325(&v37); //Save to file new resolution

                yw->game_default_res = 0x2801E0; //640 x 480
                resolution = 0x2801E0; //640 x 480
                v5 = 1;
                v40.make_changes = 1;
            }
        }
    }

    if ( field_13C2 & 4 )
    {
        if ( field_0x13a8 & 0x10 )
        {
            GFX_flags |= 0x10;
            yw->_win3d->setWDD_16bitTex(1);
        }
        else
        {
            GFX_flags &= 0xEF;
            yw->_win3d->setWDD_16bitTex(0);
        }

        resolution = yw->game_default_res;
        v5 = 1;
        v40.make_changes = 1;
    }

    if ( field_13C2 & 0x400 )
    {
        if ( field_0x13a8 & 8 )
        {
            GFX_flags |= 8;
            yw->_win3d->setWDD_drawPrim(1);
        }
        else
        {
            GFX_flags &= 0xF7;
            yw->_win3d->setWDD_drawPrim(0);
        }


        resolution = yw->game_default_res;
        v5 = 1;
        v40.make_changes = 1;
    }

    if ( (v3 && p_ypaworld->one_game_res) || v5 )
    {
        if ( p_ypaworld->one_game_res || !v3 )
            v40.resolution = p_ypaworld->game_default_res;
        else
            v40.resolution = resolution;

        yw->ypaworld_func174(&v40);

        int v24 = 0;
        for (auto const &nod : video_mode_list)
        {
            if (yw->game_default_res == nod.sort_id)
            {
                field_FBE = v24;
                video_button->button_func71(1156, nod.name);

                break;
            }

            v24++;
        }
    }

    field_13C2 = 0;
    envMode = ENVMODE_TITLE;

    video_button->Hide();

    if ( video_listvw.IsOpen() )
        p_YW->GuiWinClose( &video_listvw );

    if ( d3d_listvw.IsOpen() )
        p_YW->GuiWinClose( &d3d_listvw );

    NC_STACK_button::button_66arg v38;
    v38.field_4 = 2;
    v38.butID = 1156;

    video_button->button_func73(&v38);

    v38.butID = 1172;
    video_button->button_func73(&v38);

    titel_button->Show();
}


void UserData::sub_46DC1C()
{
    uamessage_load lvlMsg;
    lvlMsg.msgID = UAMSG_LOAD;
    lvlMsg.owner = 0;
    lvlMsg.level = netLevelID;

    yw_arg181 v5;
    v5.data = &lvlMsg;
    v5.dataSize = sizeof(lvlMsg);
    v5.recvID = 0;
    v5.recvFlags = 2;
    v5.garant = 1;

    p_YW->ypaworld_func181(&v5);

    windp_arg82 v6;
    v6.senderID = callSIGN.c_str();
    v6.senderFlags = 1;
    v6.receiverID = 0;
    v6.receiverFlags = 2;
    v6.guarant = 1;

    p_ypaworld->windp->FlushBuffer(v6);

    envAction.action = EnvAction::ACTION_NETPLAY;
    envAction.params[0] = netLevelID;
    network_listvw.firstShownEntries = 0;
    envAction.params[1] = netLevelID;

    int v12 = 1;
    p_ypaworld->windp->LockSession(&v12);

    yw_NetPrintStartInfo();
}

int sub_4EDCC4(NC_STACK_ypaworld *yw)
{
    return yw->_levelInfo->State != 8;
}

int sub_47B388(int a1, const std::string &a2)
{
    FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("save:%s/%d.sgm", a2, a1), "r");
    if ( !fil )
        return 0;

    delete fil;
    return 1;
}

int UserData::ypaworld_func158__sub0__sub7()
{
    FSMgr::FileHandle *fl = uaOpenFile(fmt::sprintf("save:%s/sgisold.txt",user_name), "r");
    if ( !fl )
        return 0;

    delete fl;
    return 1;
}

void NC_STACK_ypaworld::sub_4811E8(int id)
{
    if ( id > field_17c4 )
    {
        field_17c8 = -1;
        field_17c4 = id;
    }
}

void UserData::sub_4DE248(int id)
{
    switch (id)
    {
    default:
        break;

    case 1001:
        p_ypaworld->sub_4811E8(0x7B);
        break;

    case 1003:
        p_ypaworld->sub_4811E8(0x78);
        break;

    case 1004:
        p_ypaworld->sub_4811E8(0x79);
        break;

    case 1005:
        p_ypaworld->sub_4811E8(0x7A);
        break;

    case 1007:
        p_ypaworld->sub_4811E8(0x83);
        break;

    case 1008:
        p_ypaworld->sub_4811E8(0x7C);
        break;

    case 1011:
        p_ypaworld->sub_4811E8(0x81);
        break;

    case 1013:
        p_ypaworld->sub_4811E8(0x80);
        break;

    case 1014:
        p_ypaworld->sub_4811E8(0x82);
        break;

    case 1015:
        p_ypaworld->sub_4811E8(0x7F);
        break;

    case 1016:
        p_ypaworld->sub_4811E8(0x88);
        break;

    case 1017:
    case 1052:
    case 1107:
    case 1167:
    case 1218:
        p_ypaworld->sub_4811E8(0x89);
        break;

    case 1018:
        p_ypaworld->sub_4811E8(0x6A);
        break;

    case 1019:
        if ( sub_4EDCC4(p_ypaworld) )
        {
            if ( p_ypaworld->_levelInfo->State == 9 )
                p_ypaworld->sub_4811E8(0x49);
            else
                p_ypaworld->sub_4811E8(0x8B);
        }
        else
        {
            p_ypaworld->sub_4811E8(0x8A);
        }
        break;

    case 1020:
        p_ypaworld->sub_4811E8(0x4A);
        break;

    case 1050:
        p_ypaworld->sub_4811E8(0xC4);
        break;

    case 1051:
        p_ypaworld->sub_4811E8(0xC1);
        break;

    case 1053:
        p_ypaworld->sub_4811E8(0xC3);
        break;

    case 1054:
        p_ypaworld->sub_4811E8(0xC2);
        break;

    case 1055:
        p_ypaworld->sub_4811E8(0xC5);
        break;

    case 1056:
        p_ypaworld->sub_4811E8(0xC6);
        break;

    case 1061:
        p_ypaworld->sub_4811E8(0x4D);
        break;

    case 1101:
        p_ypaworld->sub_4811E8(0x76);
        break;

    case 1102:
        p_ypaworld->sub_4811E8(0xA8);
        break;

    case 1103:
        p_ypaworld->sub_4811E8(0xA9);
        break;

    case 1104:
        p_ypaworld->sub_4811E8(0x77);
        break;

    case 1105:
        switch ( field_0x1744 )
        {
        case 1:
            p_ypaworld->sub_4811E8(0xAB);
            break;
        case 2:
            p_ypaworld->sub_4811E8(0xAA);
            break;
        case 3:
            p_ypaworld->sub_4811E8(0xAD);
            break;
        case 4:
            p_ypaworld->sub_4811E8(0xAC);
            break;
        default:
            break;
        }
        break;

    case 1106:
        switch ( field_0x1744 )
        {
        case 0:
            p_ypaworld->sub_4811E8(0xAE);
            break;
        case 1:
            p_ypaworld->sub_4811E8(0xB0);
            break;
        case 2:
            p_ypaworld->sub_4811E8(0xAF);
            break;
        case 3:
            p_ypaworld->sub_4811E8(0xB2);
            break;
        case 4:
            p_ypaworld->sub_4811E8(0xB1);
            break;
        default:
            break;
        }
        break;
    case 1150:
        p_ypaworld->sub_4811E8(0x4E);
        break;

    case 1151:
        p_ypaworld->sub_4811E8(0xB7);
        break;

    case 1152:
        p_ypaworld->sub_4811E8(0xBE);
        break;

    case 1154:
        p_ypaworld->sub_4811E8(0xBF);
        break;

    case 1156:
        p_ypaworld->sub_4811E8(0xB5);
        break;

    case 1157:
        p_ypaworld->sub_4811E8(0xB8);
        break;

    case 1159:
        p_ypaworld->sub_4811E8(0xC0);
        break;

    case 1160:
        p_ypaworld->sub_4811E8(0xB9);
        break;

    case 1161:
        p_ypaworld->sub_4811E8(0xB4);
        break;

    case 1162:
        p_ypaworld->sub_4811E8(0xB3);
        break;

    case 1163:
        p_ypaworld->sub_4811E8(0xBD);
        break;

    case 1164:
        p_ypaworld->sub_4811E8(0xBB);
        break;

    case 1165:
        p_ypaworld->sub_4811E8(0xBC);
        break;

    case 1166:
        p_ypaworld->sub_4811E8(0x4F);
        break;

    case 1172:
        p_ypaworld->sub_4811E8(0x4B);
        break;

    case 1201:
        switch ( netSelMode )
        {
        case 0:
            p_ypaworld->sub_4811E8(0x8E);
            break;
        case 1:
            p_ypaworld->sub_4811E8(0x91);
            break;
        case 2:
            p_ypaworld->sub_4811E8(0x8F);
            break;
        case 3:
            p_ypaworld->sub_4811E8(0x90);
            break;
        case 4:
            p_ypaworld->sub_4811E8(0x92);
            break;
        default:
            break;
        }
        break;

    case 1202:
        if ( netSelMode == 1 )
        {
            p_ypaworld->sub_4811E8(0x6E);
        }
        else if ( netSelMode == 4 )
        {
            p_ypaworld->sub_4811E8(0x6F);
        }
        break;

    case 1203:
        p_ypaworld->sub_4811E8(0x8C);
        break;

    case 1205:
        switch ( netSelMode )
        {
        case 1:
            p_ypaworld->sub_4811E8(0x94);
            break;
        case 2:
            p_ypaworld->sub_4811E8(0x93);
            break;
        case 3:
        case 4:
            p_ypaworld->sub_4811E8(0x95);
            break;
        default:
            break;
        }
        break;

    case 1206:
        p_ypaworld->sub_4811E8(0x70);
        break;

    case 1207:
        p_ypaworld->sub_4811E8(0x71);
        break;

    case 1208:
        p_ypaworld->sub_4811E8(0x72);
        break;

    case 1209:
        p_ypaworld->sub_4811E8(0x73);
        break;

    case 1219:
        if ( rdyStart )
            p_ypaworld->sub_4811E8(0x75);
        else
            p_ypaworld->sub_4811E8(0x74);
        break;

    case 1225:
        p_ypaworld->sub_4811E8(0x4C);
        break;

    case 1250:
        p_ypaworld->sub_4811E8(0x6C);
        break;

    case 1251:
        p_ypaworld->sub_4811E8(0x6D);
        break;

    case 1252:
        p_ypaworld->sub_4811E8(0x89);
        break;
    }
}

// Go to options menu
void UserData::ypaworld_func158__sub0__sub2()
{
    titel_button->Hide();

    video_button->Show();

    envMode = ENVMODE_SETTINGS;

    if ( video_listvw.IsOpen() )
    {
        p_YW->GuiWinClose( &video_listvw );
        p_YW->GuiWinOpen( &video_listvw );
    }

    video_listvw.selectedEntry = field_FBE;
}

void UserData::sub_46C3E4()
{
    titel_button->Hide();

    disk_button->Show();

    envMode = ENVMODE_SELPLAYER;

    for( ProfileList::iterator it = profiles.begin(); it != profiles.end(); it++ )
    {
        if ( !StriCmp(it->name.c_str(), user_name))
        {
            it->totalElapsedTime = p_ypaworld->playerstatus[1].elapsedTime;
            break;
        }
    }
    p_YW->GuiWinClose( &disk_listvw );
    p_YW->GuiWinOpen( &disk_listvw );
}

void UserData::ypaworld_func158__sub0__sub1()
{
    titel_button->Hide();

    button_input_button->Show();

    envMode = ENVMODE_INPUT;
    p_YW->GuiWinClose( &input_listview );
    p_YW->GuiWinOpen( &input_listview );
}

void sub_4D9550(NC_STACK_ypaworld *yw, int arg)
{
    UserData *usr = yw->GameShell;

    std::string oldRsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", "data:");

    std::string wavName;
    if ( usr->default_lang_dll )
        wavName = fmt::sprintf("sounds/speech/%s/9%d.wav", usr->default_lang_dll, arg);
    else
        wavName = fmt::sprintf("sounds/speech/language/9%d.wav", arg);

    if ( !uaFileExist(std::string("rsrc:") + wavName) )
        wavName = fmt::sprintf("sounds/speech/language/9%d.wav", arg);

    if ( usr->field_ADA )
    {
        SFXEngine::SFXe.sub_424000(&usr->field_782, 0);
        SFXEngine::SFXe.sub_423DD8(&usr->field_782);
        delete_class_obj(usr->field_ADA);
        usr->field_ADA = 0;
    }

    usr->field_ADA = Nucleus::CInit<NC_STACK_wav>({{NC_STACK_rsrc::RSRC_ATT_NAME, wavName}});
    if ( usr->field_ADA )
    {
        SFXEngine::SFXe.sub_423DB0(&usr->field_782);

        usr->field_782.field_C = vec3d(0.0, 0.0, 0.0);
        usr->field_782.field_0 = vec3d(0.0, 0.0, 0.0);
        usr->field_782.samples_data[0].volume = 500;
        usr->field_782.samples_data[0].pitch = 0;

        usr->field_782.samples_data[0].psampl = usr->field_ADA->getSMPL_pSample();
        SFXEngine::SFXe.startSound(&usr->field_782, 0);
    }

    set_prefix_replacement("rsrc", oldRsrc);
}

void sub_4D0C24(NC_STACK_ypaworld *yw, const char *a1, const char *a2)
{
    UserData *usr = yw->GameShell;

    if ( strcasecmp(a1, usr->lastSender) )
    {
        if ( usr->msgBuffLine >= 31 )
        {
            usr->msgBuffLine = 31;

            for (int i = 0; i < 31; i++ )
                strcpy(usr->msgBuffers[i], usr->msgBuffers[i + 1]);
        }

        memset(usr->msgBuffers[usr->msgBuffLine], 0, 64);
        sprintf(usr->msgBuffers[usr->msgBuffLine], "> %s:", a1);
        memset(usr->lastSender, 0, 64);
        strncpy(usr->lastSender, a1, 63);

        usr->msgBuffLine++;
    }

    if ( usr->msgBuffLine >= 31 )
    {
        usr->msgBuffLine = 31;

        for (int i = 0; i < 31; i++ )
            strcpy(usr->msgBuffers[i], usr->msgBuffers[i + 1]);
    }

    memset(usr->msgBuffers[usr->msgBuffLine], 0, 64);
    strncpy(usr->msgBuffers[usr->msgBuffLine], a2, 63);

    usr->msgBuffLine++;

    if ( usr->netSelMode == 4 )
    {
        int v22 = usr->msgBuffLine - 6;

        if ( v22 < 0 )
            v22 = 0;

        yw->GameShell->network_listvw.firstShownEntries = v22;


        yw->GameShell->network_listvw.numEntries = yw->GameShell->msgBuffLine;

        int v24;

        if ( usr->network_listvw.numEntries >= 6 )
            v24 = 6;
        else
            v24 = usr->network_listvw.numEntries;

        yw->GameShell->network_listvw.shownEntries = v24;
    }
}


void UserData::yw_returnToTitle()
{
    yw_calcPlayerScore(p_ypaworld);
    yw_freeDebrief(p_ypaworld);

    sub_bar_button->Hide();

    titel_button->Show();

    envMode = ENVMODE_TITLE;
    returnToTitle = false;
}

void UserData::ypaworld_func158__sub0__sub3()
{
    titel_button->Hide();

    locale_button->Show();

    envMode = ENVMODE_SELLOCALE;
    p_YW->GuiWinClose( &local_listvw );
    p_YW->GuiWinOpen( &local_listvw );

    int i = 0;
    for(const auto &x : lang_dlls)
    {
        if ( &x == default_lang_dll )
            break;

        i++;
    }

    local_listvw.selectedEntry = i;
}

void sub_4EDCD8(NC_STACK_ypaworld *yw)
{
    yw->brief.Stage = 2;
}

void UserData::sub_46D9E0( int a2, const char *txt1, const char *txt2, int a5)
{
    field_0x2fb4 = a2;

    NC_STACK_button::button_66arg v12;
    v12.butID = 1300;
    confirm_button->button_func66(&v12);


    NC_STACK_button::button_arg76 v10;

    if ( a5 )
    {
        v10.butID = 1300;
        v10.xpos = p_ypaworld->screen_width * 0.4375;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        confirm_button->button_func76(&v10);
    }
    else
    {
        v12.butID = 1301;
        confirm_button->button_func66(&v12);

        v10.butID = 1300;
        v10.xpos = p_ypaworld->screen_width * 0.25;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        confirm_button->button_func76(&v10);

        v10.butID = 1301;
        v10.xpos = p_ypaworld->screen_width * 0.625;
        confirm_button->button_func76(&v10);
    }

    confirm_button->button_func71(1302, txt1);

    if ( txt2 )
        confirm_button->button_func71(1303, txt2);
    else
        confirm_button->button_func71(1303, " ");

    confirm_button->Show();
}

void ypaworld_func158__sub0__sub9(NC_STACK_ypaworld *yw)
{
    yw->brief.Stage = 1;
}

void ypaworld_func158__sub0__sub12(NC_STACK_ypaworld *yw)
{
    yw->brief.TimerStatus = 3;
}

void ypaworld_func158__sub0__sub11(NC_STACK_ypaworld *yw)
{
    yw->brief.TimerStatus = 2;
}


void UserData::InputConfCancel()
{
    for(TInputConf &konf : InputConfig)
    {
        konf.PKeyCode = konf.PKeyCodeBkp;
        konf.NKeyCode = konf.NKeyCodeBkp;
    }
}

void UserData::InputPageCancel()
{
    envMode = ENVMODE_TITLE;
    InputConfCancel();

    NC_STACK_button::button_66arg v6;
    v6.butID = 1003;
    v6.field_4 = 2;
    sub_bar_button->button_func73(&v6);

    field_D5E = 0;

    v6.butID = 1050;
    v6.field_4 = (inp_joystick == false) + 1;
    button_input_button->button_func73(&v6);

    v6.field_4 = (inp_altjoystick == false) + 1;
    v6.butID = 1061;
    button_input_button->button_func73(&v6);

    v6.butID = 1055;
    v6.field_4 = ((p_ypaworld->field_73CE & 8) != 0) + 1;
    button_input_button->button_func73(&v6);

    button_input_button->Hide();

    p_YW->GuiWinClose( &input_listview );

    titel_button->Show();
}

void UserData::InputConfCopyToBackup()
{
    for(TInputConf &konf : InputConfig)
    {
        konf.PKeyCodeBkp = konf.PKeyCode;
        konf.NKeyCodeBkp = konf.NKeyCode;
    }
}

int NC_STACK_ypaworld::InputConfigLoadDefault()
{
    std::string file = fmt::sprintf("data:settings/%s/input.def", lang_name);

    ScriptParser::HandlersList hndls
    {
        new World::Parsers::InputParser(this)
    };

    return ScriptParser::ParseFile(file, hndls, 0);
}

void UserData::InputConfigRestoreDefault()
{
    if ( p_YW->InputConfigLoadDefault() )
    {
        for(TInputConf &konf : InputConfig)
        {
            konf.PKeyCodeDef = konf.PKeyCode;
            konf.NKeyCodeDef = konf.NKeyCode;
        }
    }
    for(TInputConf &konf : InputConfig)
    {
        konf.PKeyCode = konf.PKeyCodeDef;
        konf.NKeyCode = konf.NKeyCodeDef;
    }
}

void UserData::sub_46C5F0(int a2)
{
    if ( video_listvw.selectedEntry != field_FBE && !a2 )
    {

        field_FBE = video_listvw.selectedEntry;

        VideoModesList::const_iterator it = std::next(video_mode_list.cbegin(), video_listvw.selectedEntry);

        game_default_res = it->sort_id;
        video_button->button_func71(1156, it->name);
    }
}

//Options Cancel
void UserData::sub_46A3C0()
{
    field_13C2 = 0;
    envMode = ENVMODE_TITLE;

    std::string name;
    int i = 0;
    for( auto const &nod: video_mode_list)
    {
        if (nod.sort_id == p_ypaworld->game_default_res )
        {
            name = nod.name;
            break;
        }
        i++;
    }

    video_listvw.selectedEntry = i;
    field_FBE = i;
    game_default_res = p_ypaworld->game_default_res;

    video_button->button_func71(1156, name);

    video_button->button_func71(1172, win3d_name);

    field_139A = win3d_guid;
    field_139E = win3d_name;

    NC_STACK_button::button_66arg v10;
    v10.butID = 1151;
    v10.field_4 = ((snd__flags2 & 1) == 0) + 1;
    video_button->button_func73(&v10);

    v10.field_4 = ((snd__flags2 & 0x10) == 0) + 1;
    v10.butID = 1164;
    video_button->button_func73(&v10);

    v10.butID = 1157;
    v10.field_4 = ((GFX_flags & 1) == 0) + 1;
    video_button->button_func73(&v10);

    v10.field_4 = ((GFX_flags & 2) == 0) + 1;
    v10.butID = 1160;
    video_button->button_func73(&v10);

    v10.butID = 1150;
    v10.field_4 = ((GFX_flags & 0x10) == 0) + 1;
    video_button->button_func73(&v10);

    v10.butID = 1166;
    v10.field_4 = ((GFX_flags & 8) == 0) + 1;
    video_button->button_func73(&v10);

    v10.butID = 1165;
    v10.field_4 = ((GFX_flags & 4) == 0) + 1;
    video_button->button_func73(&v10);

    v10.field_4 = (enemyindicator == 0) + 1;
    v10.butID = 1163;
    video_button->button_func73(&v10);

    NC_STACK_button::Slider *tmp = video_button->button_func74(1159);
    tmp->value = fxnumber;
    video_button->button_func75(1159);

    tmp = video_button->button_func74(1152);
    tmp->value = snd__volume;
    video_button->button_func75(1152);

    tmp = video_button->button_func74(1154);
    tmp->value = snd__cdvolume;
    video_button->button_func75(1154);

    video_button->Hide();

    if ( video_listvw.IsOpen() )
        p_YW->GuiWinClose( &video_listvw );

    if ( d3d_listvw.IsOpen() )
        p_YW->GuiWinClose( &d3d_listvw );

    titel_button->Show();

    v10.field_4 = 2;
    v10.butID = 1156;
    video_button->button_func73(&v10);

    v10.butID = 1172;
    video_button->button_func73(&v10);
}

void  UserData::ypaworld_func158__sub0__sub5(int a2)
{
    int v4 = 0;

    wdd_func324arg a1;
    a1.guid = 0;
    a1.currr = 0;
    a1.name = (const char *)-1;

    const char *v2;
    const char *v12;

    while ( a1.name )
    {
        p_ypaworld->_win3d->windd_func324(&a1);
        if ( a1.name )
        {
            if ( v4 == d3d_listvw.selectedEntry )
            {
                if ( !strcmp(a1.name, "software") )
                    v2 = get_lang_string(p_ypaworld->string_pointers_p2, 2472, "2472 = Software");
                else
                    v2 = a1.name;

                v12 = a1.guid;
            }
        }
        v4++;
    }

    if ( !a2 )
    {
        field_139E = v2;
        field_139A = v12;

        video_button->button_func71(1172, v2);
    }
}

void UserData::sub_46C914()
{
    if ( field_1612 )
    {
        ProfileList::iterator it = profiles.begin();

        for (int i = 0; i < field_1612 - 1; i++) // check usr->field_1612 - 1
            it++;

        std::string a1a = fmt::sprintf("%s/user.txt", it->name);

        yw_arg172 arg172;

        arg172.usertxt = a1a.c_str();
        arg172.field_4 = it->name.c_str();
        arg172.field_8 = 255;
        arg172.usr = this;
        arg172.field_10 = 1;

        p_YW->ypaworld_func172(&arg172);

        user_name = it->name;
        usernamedir = it->name;


        field_0x1744 = 0;
        field_3426 = 0;

        disk_button->Hide();

        p_YW->GuiWinClose( &disk_listvw );

        envMode = ENVMODE_SINGLEPLAY;

        sub_bar_button->Show();
    }
}

void sub_46D0F8(const std::string &path)
{
    FSMgr::DirIter dir = uaOpenDir(path);
    if ( dir )
    {
        FSMgr::iNode *v5;

        while ( dir.getNext(&v5) )
        {
            if ( v5->getType() == FSMgr::iNode::NTYPE_FILE || (v5->getName().compare(".") && v5->getName().compare("..")) )
                uaDeleteFile(fmt::sprintf("%s/%s", path, v5->getName()));
        }
    }
}

void UserData::sub_46C748()
{
    if ( field_1612 )
    {
        if ( StriCmp(usernamedir, user_name) )
        {

            ProfileList::iterator it = profiles.begin();

            for (int i = 0; i < field_1612 - 1; i++) // check usr->field_1612 - 1
                it++;

            ProfileList::iterator nextIt = std::next(it);

            bool HasElements;

            if ( nextIt != profiles.end() )
                HasElements = true;
            else
            {
                nextIt = std::prev(it);
                HasElements = false;
            }

            std::string a1 = fmt::sprintf("save:%s", it->name);

            sub_46D0F8(a1);

            uaDeleteDir(a1);

            profiles.erase(it);

            disk_listvw.numEntries--;
            if ( profiles.empty() )
            {
                field_1612 = 0;
                usernamedir = "NEWUSER";
            }
            else
            {
                if ( !HasElements )
                    field_1612--;

                usernamedir = nextIt->name;
            }

            usernamedir_len = usernamedir.size();

            if ( field_1612 )
                disk_listvw.PosOnSelected(field_1612 - 1);

            field_0x1744 = 0;

            disk_button->Hide();

            p_YW->GuiWinClose( &disk_listvw );

            if ( field_0x1760 )
            {
                envMode = ENVMODE_SINGLEPLAY;
                sub_bar_button->Show();
            }
            else
            {
                envMode = ENVMODE_TITLE;
                titel_button->Show();
            }
        }
    }
}

void UserData::sub_46B0E0()
{
    Engine::StringList::iterator lang = std::next(lang_dlls.begin(), local_listvw.selectedEntry);

    prev_lang = &(*lang);

    if ( prev_lang != default_lang_dll )
    {
        if ( prev_lang )
        {
            default_lang_dll = prev_lang;
            p_YW->ypaworld_func175( this );
        }
    }

    field_19CA = 0;
    envMode = ENVMODE_TITLE;
    field_19CA = 0;
    prev_lang = default_lang_dll;

    locale_button->Hide();

    p_YW->GuiWinClose( &local_listvw );

    titel_button->Show();
}

void UserData::sub_46AA0C()
{
    envMode = ENVMODE_TITLE;
    field_19CA = 0;
    prev_lang = default_lang_dll;

    locale_button->Hide();

    p_YW->GuiWinClose( &local_listvw );

    titel_button->Show();
}


int NC_STACK_ypaworld::sub_449678(InputState *struc, int kkode)
{
    return struc->KbdLastHit == kkode && ( (struc->ClickInf.flag & ClickBoxInf::FLAG_RM_HOLD) || easy_cheat_keys );
}

void UserData::ypaworld_func158__sub0__sub4()
{
    titel_button->Hide();

    about_button->Show();

    envMode = ENVMODE_ABOUT;
}


int ypaworld_func158__sub0__sub6(char a1)
{
    if (a1 == '/' || a1 == '\\' || a1 == ':' || a1 == '*' || a1 == '?' || a1 == '"' || a1 == '<' || a1 == '>' || a1 == '|')
        return 0;

    return 1;
}







void UserData::GameShellUiHandleInput()
{
    int v3 = 0;

    if ( _input->ClickInf.flag & ClickBoxInf::FLAG_BTN_DOWN )
        SFXEngine::SFXe.startSound(&samples1_info, 3);

    if ( netSelMode )
        yw_HandleNetMsg(p_ypaworld);

    NC_STACK_win3d *windd = dynamic_cast<NC_STACK_win3d *>(p_ypaworld->_win3d);

    if ( netSelMode == 1 )
    {
        if ( p_ypaworld->windp->GetProvType() == 4 )
        {
            if ( modemAskSession )
            {
                if ( _input->KbdLastHit == Input::KC_SPACE )
                {
                    windd->windd_func320(NULL);
                    p_ypaworld->windp->EnumSessions(NULL);
                    windd->windd_func321(NULL);
                }
            }
        }
        else if ( p_ypaworld->windp->GetProvType() != 3 || _input->KbdLastHit == Input::KC_SPACE )
        {
            p_ypaworld->windp->EnumSessions(NULL);
        }
    }

    NC_STACK_button::button_66arg v408;
    NC_STACK_button::button_66arg v410;

    v410.field_4 = 0;
    v410.butID = 1015;
    sub_bar_button->button_func67(&v410);
    v410.butID = 1011;
    sub_bar_button->button_func67(&v410);
    v410.butID = 1019;
    sub_bar_button->button_func67(&v410);
    v410.butID = 1014;
    sub_bar_button->button_func67(&v410);
    v410.butID = 1013;
    sub_bar_button->button_func67(&v410);
    v410.butID = 1020;
    sub_bar_button->button_func67(&v410);

    v410.butID = 1003;
    titel_button->button_func67(&v410);
    v410.butID = 1004;
    titel_button->button_func67(&v410);
    v410.butID = 1001;
    titel_button->button_func67(&v410);
    v410.butID = 1008;
    titel_button->button_func67(&v410);
    v410.butID = 1007;
    titel_button->button_func67(&v410);
    v410.butID = 1018;
    titel_button->button_func67(&v410);
    v410.butID = 1017;
    titel_button->button_func67(&v410);
    v410.butID = 1016;
    titel_button->button_func67(&v410);

    NC_STACK_button::button_arg76 v393;
    v393.ypos = -1;
    v393.width = -1;
    //v394 = -1;
    v393.butID = 1014;
    v393.xpos = 0;
    sub_bar_button->button_func76(&v393);

    v393.butID = 1019;
    v393.xpos = p_ypaworld->screen_width - dword_5A50B6_h;
    sub_bar_button->button_func76(&v393);

    v393.butID = 1011;
    v393.xpos = word_5A50C0 + dword_5A50B6_h;
    sub_bar_button->button_func76(&v393);

    sub_bar_button->button_func71(1019,  get_lang_string(ypaworld__string_pointers, 644, "GO BACK"));

    if ( sub_4EDCC4(p_ypaworld) )
    {
        if ( p_ypaworld->_levelInfo->State != 9 && !field_0xc )
        {
            v410.butID = 1014;
            sub_bar_button->button_func66(&v410);

            v393.butID = 1014;
            v393.xpos = p_ypaworld->screen_width - dword_5A50B6_h;
            sub_bar_button->button_func76(&v393);

            v393.butID = 1019;
            v393.xpos = 0;
            sub_bar_button->button_func76(&v393);

            sub_bar_button->button_func71(1019, get_lang_string(ypaworld__string_pointers, 2438, "2438 == BACK"));
        }

        if ( p_ypaworld->_levelInfo->State == 9 )
        {
            v410.butID = 1011;
            sub_bar_button->button_func66(&v410);

            v393.butID = 1011;
            v393.xpos = 0;
            sub_bar_button->button_func76(&v393);

            sub_bar_button->button_func71(1019, get_lang_string(ypaworld__string_pointers, 2420, "CONTINUE"));
        }

        v410.butID = 1019;
        sub_bar_button->button_func66(&v410);

        button_input_button->Hide();
        video_button->Hide();
        disk_button->Hide();
        locale_button->Hide();
        network_button->Hide();

        p_YW->GuiWinClose( &input_listview );
        p_YW->GuiWinClose( &video_listvw );
        p_YW->GuiWinClose( &disk_listvw );
        p_YW->GuiWinClose( &local_listvw );
        p_YW->GuiWinClose( &network_listvw );
    }
    else if ( envMode == ENVMODE_TITLE )
    {
        v410.butID = 1003;
        titel_button->button_func66(&v410);

        v410.butID = 1004;
        titel_button->button_func66(&v410);

        v410.butID = 1001;
        titel_button->button_func66(&v410);

        v410.butID = 1008;
        if ( lang_dlls_count > 1 )
            titel_button->button_func66(&v410);
        else
            titel_button->button_func67(&v410);

        v410.butID = 1007;
        titel_button->button_func66(&v410);

        v410.butID = 1017;
        titel_button->button_func66(&v410);

        v410.butID = 1018;
        titel_button->button_func66(&v410);

        v410.butID = 1016;
        titel_button->button_func66(&v410);
    }
    else if ( envMode == ENVMODE_SINGLEPLAY || envMode == ENVMODE_TUTORIAL )
    {
        if ( !field_3426 )
        {
            if ( sub_47B388(0, user_name) )
                field_3426 = 1;
            else
                field_3426 = 2;
        }

        if ( field_3426 == 1 )
        {
            v410.butID = 1015;
            sub_bar_button->button_func66(&v410);
        }

        v410.butID = 1019;
        sub_bar_button->button_func66(&v410);

        v410.butID = 1020;
        sub_bar_button->button_func66(&v410);
    }

    if ( field_0x2fb4 )
        v3 = 1;

    NC_STACK_button::ResCode r = confirm_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 1350 ) // OK
        {
            switch ( field_0x2fb4 )
            {
            case 1:
            {
                field_3426 = 0;
                p_ypaworld->isNetGame = 0;

                sub_bar_button->Hide();

                envAction.action = EnvAction::ACTION_LOAD;
                envAction.params[0] = 0;
            }
            break;

            case 2:
                sub_46DC1C();
                break;
            case 3:
                sb_0x46ca74();
                break;
            case 4:
                sub_46D960();
                break;
            case 5:
                sb_0x46aa8c();
                break;
            case 6:
                sb_0x46cdf8();
                break;
            default:
                break;
            }
            sub_46D960();
        }
        else if ( r.code == 1351 ) // Cancel
        {
            if ( field_0x2fb4 == 3 || field_0x2fb4 == 6 )
                field_0x1744 = 0;

            sub_46D960();
        }
    }

    if ( field_0x2fb4 )
    {
        if ( _input->HotKeyID == 24 )
        {
            if ( field_0x2fb4 == 3 || field_0x2fb4 == 6 )
                field_0x1744 = 0;
            sub_46D960();
        }
        if ( _input->KbdLastHit == Input::KC_RETURN )
        {
            switch ( field_0x2fb4 )
            {
            case 1:
            {
                field_3426 = 0;
                p_ypaworld->isNetGame = 0;
                sub_bar_button->Hide();
                envAction.action = EnvAction::ACTION_LOAD;
                envAction.params[0] = 0;
            }
            break;
            case 2:
                sub_46DC1C();
                break;
            case 3:
                sb_0x46ca74();
                break;
            case 4:
                sub_46D960();
                break;
            case 5:
                sb_0x46aa8c();
                break;
            case 6:
                sb_0x46cdf8();
                break;
            default:
                break;
            }
            sub_46D960();
        }
    }

    if ( v3 )
    {
        _input->ClickInf.flag = 0;
        _input->KbdLastHit = Input::KC_NONE;
        _input->KbdLastDown = Input::KC_NONE;
        _input->chr = 0;
        _input->HotKeyID = -1;
    }

    if ( envMode == ENVMODE_TITLE && _input->HotKeyID == 43 )
        p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 750, "help\\start.html");

    r = titel_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 1001 )
        {
            sub_46C3E4();
            field_0x1760 = 0;
        }
        else if ( r.code == 1005 ) // Options button
        {
            ypaworld_func158__sub0__sub2();
        }
        else if ( r.code == 1007 )
        {
            ypaworld_func158__sub0__sub1();
        }
        else if ( r.code == 1011 )
        {
            ypaworld_func158__sub0__sub3();
        }
        else if ( r.code == 1013 )
        {
            titel_button->Hide();

            envAction.action = EnvAction::ACTION_QUIT;
            SFXEngine::SFXe.startSound(&samples1_info, 4);
        }
        else if ( r.code == 1022 )
        {
            GameShellUiOpenNetwork();
        }
        else if ( r.code == 1024 )
        {
            sub_bar_button->Show();

            titel_button->Hide();

            envMode = ENVMODE_SINGLEPLAY;
        }
        else if ( r.code == 1025 )
        {
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 750, "help\\start.html");
        }
    }

    if ( (envMode == ENVMODE_SINGLEPLAY || envMode == ENVMODE_TUTORIAL) && _input->HotKeyID == 24 )
    {
        if ( sub_4EDCC4(p_ypaworld) )
        {
            sub_4EDCD8(p_ypaworld);
            if ( p_ypaworld->field_73CE & World::PREF_CDMUSICDISABLE )
            {
                SFXEngine::SFXe.StopMusicTrack();
                if ( shelltrack )
                {
                    SFXEngine::SFXe.SetMusicTrack(shelltrack, shelltrack__adv.min_delay, shelltrack__adv.max_delay);
                    SFXEngine::SFXe.PlayMusicTrack();
                }
            }
            if ( returnToTitle )
                yw_returnToTitle();
        }
        else
        {
            sub_bar_button->Hide();

            titel_button->Show();

            envMode = ENVMODE_TITLE;
        }
    }

    r = sub_bar_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);
        switch ( r.code )
        {
        case 1013:
            if ( sub_4EDCC4(p_ypaworld) )
            {
                sub_4EDCD8(p_ypaworld);
                if ( p_ypaworld->field_73CE & World::PREF_CDMUSICDISABLE )
                {
                    SFXEngine::SFXe.StopMusicTrack();
                    if ( shelltrack )
                    {
                        SFXEngine::SFXe.SetMusicTrack(shelltrack, shelltrack__adv.min_delay, shelltrack__adv.max_delay);
                        SFXEngine::SFXe.PlayMusicTrack();
                    }
                }
                if ( returnToTitle )
                    yw_returnToTitle();
            }
            else
            {
                sub_bar_button->Hide();

                titel_button->Show();

                envMode = ENVMODE_TITLE;
            }
            break;

        case 1019:
        {
            field_3426 = 0;
            p_ypaworld->isNetGame = 0;

            sub_bar_button->Hide();

            ypaworld_func158__sub0__sub9(p_ypaworld);
        }
        break;

        case 1021:
            if ( ypaworld_func158__sub0__sub7() )
            {
                const char *v18 = get_lang_string(ypaworld__string_pointers, 2439, "2439");
                const char *v19 = get_lang_string(ypaworld__string_pointers, 2434, "DO YOU WANT TO LOAD >>>OLDER<<< SAVEGAME?");
                sub_46D9E0(1, v19, v18, 0);
            }
            else
            {
                const char *v20 = get_lang_string(ypaworld__string_pointers, 2440, "2440");
                const char *v21 = get_lang_string(ypaworld__string_pointers, 2482, "DO YOU WANT TO LOAD INGAME SAVEGAME?");
                sub_46D9E0(1, v21, v20, 0);
            }
            break;

        case 1016:
            ypaworld_func158__sub0__sub12(p_ypaworld);
            break;

        case 1020:
            ypaworld_func158__sub0__sub11(p_ypaworld);
            break;

        case 1026:
            sub_46C3E4();
            field_0x1760 = 1;
            break;

        default:
            break;
        }
    }

    if ( envMode == ENVMODE_INPUT )
    {
        if ( !field_D52 && _input->HotKeyID == 43 )
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 759, "help\\19.html");

        if ( _input->KbdLastHit != Input::KC_NONE )
        {
            if ( field_D52 )
            {
                input_listview.listFlags &= ~GuiList::GLIST_FLAG_KEYB_INPUT;

                if ( !NC_STACK_input::KeyTitle.at( _input->KbdLastHit ).empty() )
                {
                    if ( field_D3A )
                    {
                        InputConfig[field_D36].PKeyCode = _input->KbdLastHit;

                        if ( InputConfig[field_D36].Type == World::INPUT_BIND_TYPE_SLIDER )
                            field_D3A = 0;

                        field_D52 = 0;
                        InputConfig[field_D36].SetFlags = 0;
                    }
                    else
                    {
                        InputConfig[field_D36].NKeyCode = _input->KbdLastHit;
                        InputConfig[field_D36].SetFlags &= ~2;
                        field_D3A = 1;
                    }
                }
                _input->KbdLastHit = Input::KC_NONE;
            }
            else
            {
                input_listview.listFlags |= GuiList::GLIST_FLAG_KEYB_INPUT;

                if ( _input->KbdLastHit == Input::KC_BACKSPACE  || _input->KbdLastHit == Input::KC_DELETE)
                {
                    if (InputConfig[field_D36].Type != World::INPUT_BIND_TYPE_SLIDER)
                        InputConfig[field_D36].PKeyCode = 0;
                }
                else if ( _input->KbdLastHit == Input::KC_RETURN )
                {
                    InputConfig[field_D36].SetFlags = 3;
                    field_D52 = 1;
                    if ( InputConfig[field_D36].Type == World::INPUT_BIND_TYPE_SLIDER )
                        field_D3A = 0;
                }
                else if ( _input->KbdLastHit == Input::KC_ESCAPE )
                {
                    InputPageCancel();
                }
            }
        }
    }

    if ( InputConfig[field_D36].Type == World::INPUT_BIND_TYPE_SLIDER )
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        button_input_button->button_func67(&v410);
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        button_input_button->button_func66(&v410);
    }

    r = button_input_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if (r.code == 1050)
        {
            field_D42 = 1;
            field_D5E |= 1;
        }
        else if (r.code == 1051)
        {
            field_D42 = 0;
            field_D5E |= 1;
        }
        else if (r.code == 1052)
        {
            if ( field_D5E & 1 )
            {
                inp_joystick = field_D42;
                if ( field_D42 )
                    p_ypaworld->field_73CE &= ~World::PREF_JOYDISABLE;
                else
                    p_ypaworld->field_73CE |= World::PREF_JOYDISABLE;
            }

            if ( field_D5E & 4 )
            {
                inp_altjoystick = field_D4A;
                if ( field_D4A )
                    p_ypaworld->field_73CE |= World::PREF_ALTJOYSTICK;
                else
                    p_ypaworld->field_73CE &= ~World::PREF_ALTJOYSTICK;
            }

            if ( field_D5E & 2 )
            {
                if ( field_D4E )
                    p_ypaworld->field_73CE &= 0xF7;
                else
                    p_ypaworld->field_73CE |= 8;
            }

            field_D5E = 0;
            sub_46D2B4();
            InputConfCopyToBackup();

            button_input_button->Hide();

            titel_button->Show();

            p_YW->GuiWinClose( &input_listview );
            envMode = ENVMODE_TITLE;
        }
        else if (r.code == 1053)
        {
            InputConfigRestoreDefault();
        }
        else if (r.code == 1054)
        {
            InputPageCancel();
        }
        else if ( r.code == 1055 )
        {
            field_D4E = 0;
            field_D5E |= 2;
        }
        else if ( r.code == 1056 )
        {
            field_D4E = 1;
            field_D5E |= 2;
        }
        else if ( r.code == 1057 )
        {
            InputConfig[ field_D36 ].PKeyCode = 0;
        }
        else if ( r.code == 1058 )
        {
            field_D4A = 1;
            field_D5E |= 4;
        }
        else if ( r.code == 1059 )
        {
            field_D4A = 0;
            field_D5E |= 4;
        }
        else if ( r.code == 1250 )
        {
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 759, "help\\19.html");
            field_D52 = 0;
        }
    }

    if ( envMode == ENVMODE_INPUT )
    {
        input_listview.InputHandle(p_ypaworld, _input);

        field_D36 = input_listview.selectedEntry + 1;

        if ( input_listview.listFlags & GuiList::GLIST_FLAG_IN_SELECT )
        {
            InputConfig[ field_D36 ].SetFlags = 0;
            field_D3A = 1;
            field_D52 = 0;
        }
        input_listview.Formate(p_ypaworld);
    }

    if ( envMode == ENVMODE_SETTINGS )
    {
        if ( _input->KbdLastHit == Input::KC_RETURN )
        {
            if ( video_listvw.IsClosed() && d3d_listvw.IsClosed() )
            {
                if ( field_13C2 & 1 && game_default_res != p_ypaworld->game_default_res && game_default_res )
                {
                    const char *v44 = get_lang_string(ypaworld__string_pointers, 342, "THIS CAN ... PROBLEMS");
                    const char *v45 = get_lang_string(ypaworld__string_pointers, 341, "DO YOU WANT TO CHANGE VIDEOMODE?");

                    sub_46D9E0(5, v45, v44, 0);
                }
                else
                {
                    sb_0x46aa8c();
                }
            }

            if ( video_listvw.IsOpen() )
            {
                p_YW->GuiWinClose( &video_listvw );

                if ( video_listvw.IsClosed() )
                {
                    v408.butID = 1156;
                    v408.field_4 = 2;

                    video_button->button_func73(&v408);
                }

                field_13C2 |= 1;
                sub_46C5F0(0);
            }

        }
        else if ( _input->KbdLastHit == Input::KC_ESCAPE )
        {
            sub_46A3C0();
            envMode = ENVMODE_TITLE;
        }
        if ( _input->HotKeyID == 43 )
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 760, "help\\110.html");
    }


    r = video_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 1100 )
        {
            p_YW->GuiWinOpen( &video_listvw );
            SFXEngine::SFXe.startSound(&samples1_info, 7);

            _input->ClickInf.flag &= ~ClickBoxInf::FLAG_LM_DOWN;
        }
        else if ( r.code == 1101 )
        {
            p_YW->GuiWinClose( &video_listvw );
        }
        else if ( r.code == 1102 )
        {
            field_13C2 |= 0x10;
            field_0x13a8 |= 1;
        }
        else if ( r.code == 1103 )
        {
            field_0x13a8 &= 0xFFFE;
            field_13C2 |= 0x10;
        }
        else if ( r.code == 1106 )
        {
            field_13C2 |= 8;
            field_0x13a8 |= 2;
        }
        else if ( r.code == 1107 )
        {
            field_0x13a8 &= 0xFFFD;
            field_13C2 |= 8;
        }
        else if ( r.code == 1108 )
        {
            field_13C2 |= 0x40;
        }
        else if ( r.code == 1111 )
        {
            field_13C2 |= 2;
            field_0x13b0 &= 0xFFFE;
        }
        else if ( r.code == 1112 )
        {
            field_0x13b0 |= 1;
            field_13C2 |= 2;
        }
        else if ( r.code == 1113 )
        {
            field_0x13a8 |= 0x10;
            field_13C2 |= 4;
        }
        else if ( r.code == 1114 )
        {
            field_0x13a8 &= 0xEF;
            field_13C2 |= 4;
        }
        else if ( r.code == 1115 )
        {
            SFXEngine::SFXe.startSound(&samples1_info, 0);
            field_13C2 |= 0x80;
        }
        else if ( r.code == 1117 )
        {
            SFXEngine::SFXe.sub_424000(&samples1_info, 0);
        }
        else if ( r.code == 1118 )
        {
            field_13C2 |= 0x100;
        }
        else if ( r.code == 1124 )
        {
            if ( (field_13C2 & 1) &&  game_default_res != p_ypaworld->game_default_res && game_default_res )
            {
                const char *v51 = get_lang_string(ypaworld__string_pointers, 342, "THIS CAN ... PROBLEMS");
                const char *v52 = get_lang_string(ypaworld__string_pointers, 341, "DO YOU WANT TO CHANGE VIDEOMODE?");
                sub_46D9E0(5, v52, v51, 0);
            }
            else
            {
                sb_0x46aa8c();
            }
        }
        else if ( r.code == 1125 ) // Options CANCEL
        {
            sub_46A3C0();
        }
        else if ( r.code == 1126 )
        {
            field_13BE = 1;
            field_13C2 |= 0x20;
        }
        else if ( r.code == 1127 )
        {
            field_13BE = 0;
            field_13C2 |= 0x20;
        }
        else if ( r.code == 1128 )
        {
            field_13C2 |= 0x200;
            field_0x13b0 |= 0x10;
        }
        else if ( r.code == 1129 )
        {
            field_0x13b0 &= 0xFFEF;
            field_13C2 |= 0x200;
        }
        else if ( r.code == 1130 )
        {
            field_13C2 |= 0x400;
            field_0x13a8 |= 8;
        }
        else if ( r.code == 1131 )
        {
            field_0x13a8 &= 0xFFF7;
            field_13C2 |= 0x400;
        }
        else if ( r.code == 1132 )
        {
            field_13C2 |= 0x800;
            field_0x13a8 |= 4;
        }
        else if ( r.code == 1133 )
        {
            field_0x13a8 &= 0xFFFB;
            field_13C2 |= 0x800;
        }
        else if ( r.code == 1134 )
        {
            p_YW->GuiWinOpen( &d3d_listvw );
            SFXEngine::SFXe.startSound(&samples1_info, 7);

            _input->ClickInf.flag &= ~ClickBoxInf::FLAG_LM_DOWN;
        }
        else if ( r.code == 1135 )
        {
            p_YW->GuiWinClose( &d3d_listvw );
        }
        else if ( r.code == 1250 )
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 760, "help\\110.html");
    }

    if ( envMode == ENVMODE_SETTINGS && video_listvw.IsOpen() )
    {
        video_listvw.InputHandle(p_ypaworld, _input);

        if ( video_listvw.listFlags & GuiList::GLIST_FLAG_SEL_DONE )
        {
            int v65 = 0;

            if ( remoteMode )
                v65 = 1;

            field_13C2 |= 1;
            sub_46C5F0(v65);
        }

        if ( video_listvw.IsClosed() )
        {
            v408.field_4 = 2;
            v408.butID = 1156;

            video_button->button_func73(&v408);
        }

        video_listvw.Formate(p_ypaworld);
    }

    if ( envMode == ENVMODE_SETTINGS && d3d_listvw.IsOpen() )
    {
        d3d_listvw.InputHandle(p_ypaworld, _input);

        if ( d3d_listvw.listFlags & GuiList::GLIST_FLAG_SEL_DONE )
        {
            int v66 = 0;

            if ( remoteMode )
                v66 = 1;

            field_13C2 |= 0x1000;
            ypaworld_func158__sub0__sub5(v66);
        }

        if ( d3d_listvw.IsClosed() )
        {
            v408.field_4 = 2;
            v408.butID = 1172;

            video_button->button_func73(&v408);
        }

        d3d_listvw.Formate(p_ypaworld);
    }

    NC_STACK_button::Slider *v67 = video_button->button_func74(1159);
    
    video_button->button_func71(1158, fmt::sprintf("%d", v67->value));
    field_0x13a4 = v67->value;

    v67 = video_button->button_func74(1152);

    video_button->button_func71(1153, fmt::sprintf("%d", v67->value));
    field_0x13b4 = v67->value;

    SFXEngine::SFXe.setMasterVolume(field_0x13b4);

    v67 = video_button->button_func74(1154);

    video_button->button_func71(1155, fmt::sprintf("%d", v67->value));
    field_0x13b8 = v67->value;

    SFXEngine::SFXe.SetMusicVolume(field_0x13b8);

    if ( envMode == ENVMODE_SELPLAYER ) //Load/Save
    {
        if ( _input->KbdLastHit != Input::KC_NONE || _input->chr )
        {
            if ( field_0x1744 )
            {
                if ( _input->KbdLastHit == Input::KC_BACKSPACE )
                {
                    if ( usernamedir_len > 0 )
                    {
                        usernamedir.erase( usernamedir_len - 1, 1 );
                        usernamedir_len--;
                    }
                }
                else if ( _input->KbdLastHit == Input::KC_RETURN )
                {
                    switch ( field_0x1744 )
                    {
                    case 1:
                        if ( field_1612 )
                        {
                            const char *v88 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                            const char *v89 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                            sub_46D9E0(3, v89, v88, 0);
                        }
                        else if (usernamedir.size() > 0)
                        {
                            sb_0x46ca74();
                        }
                        break;

                    case 2:
                        sub_46C914();
                        break;

                    case 3:
                        if ( field_1612 )
                        {
                            const char *v90 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                            const char *v91 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                            sub_46D9E0(6, v91, v90, 0);
                        }
                        else if (usernamedir.size() > 0)
                        {
                            sb_0x46cdf8();
                        }
                        break;

                    case 4:
                        sub_46C748();
                        break;

                    default:
                        break;
                    }
                }
                else if ( _input->KbdLastHit == Input::KC_ESCAPE )
                {
                    field_0x1744 = 0;
                }
                else if ( _input->KbdLastHit == Input::KC_LEFT )
                {
                    if ( usernamedir_len > 0 )
                        usernamedir_len--;
                }
                else if ( _input->KbdLastHit == Input::KC_RIGHT )
                {
                    if ( usernamedir_len < (int)usernamedir.size() )
                        usernamedir_len++;
                }
                else if ( _input->KbdLastHit == Input::KC_DELETE )
                {
                    if ( usernamedir_len < (int)usernamedir.size() )
                        usernamedir.erase(usernamedir_len, 1);
                }

                if ( usernamedir.size() < 32 )
                {
                    if ( _input->chr >= ' ' )
                    {
                        if ( ypaworld_func158__sub0__sub6(_input->chr) )
                        {
                            usernamedir.insert(usernamedir_len, 1, _input->chr);
                            usernamedir_len++;
                        }
                    }
                }
            }
            else
            {
                if ( _input->KbdLastHit == Input::KC_ESCAPE )
                    sub_46A7F8();

                if ( _input->HotKeyID == 43 )
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 758, "help\\18.html");

            }

            if ( field_1612 )
                disk_listvw.PosOnSelected(field_1612 - 1);
        }
    }


    field_1612 = 0;
    int v108 = 1;

    for ( ProfileList::iterator it = profiles.begin(); it != profiles.end(); it++)
    {
        if ( !StriCmp(it->name, usernamedir) )
        {
            field_1612 = v108;
            break;
        }
        v108++;
    }

    r = disk_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 103 )
        {
            sub_46A7F8();
        }
        else if ( r.code == 1160 )
        {
            field_0x1744 = 2;

            if ( !field_1612 )
            {
                usernamedir = get_lang_string(ypaworld__string_pointers, 366, "NEW GAME");
            }
            
            usernamedir_len = usernamedir.size();
                        
            disk_button->button_func71(1100, usernamedir + 'h');
        }
        else if ( r.code == 1161 )
        {
            field_0x1744 = 4;
            if ( !field_1612 )
            {
                usernamedir = get_lang_string(ypaworld__string_pointers, 366, "NEW GAME");
            }
            
            usernamedir_len = usernamedir.size();

            disk_button->button_func71(1100, usernamedir + 'h');
        }
        else if ( r.code == 1162 )
        {
            field_0x1744 = 3;

            std::string tmp = get_lang_string(ypaworld__string_pointers, 366, "NEW GAME");

            int maxN = 0;

            for (ProfileList::iterator it = profiles.begin(); it != profiles.end(); it++)
            {
                if ( !StriCmp(tmp, it->name.substr(0, tmp.size())) )
                {
                    int n = std::stoi( it->name.substr(tmp.size()) );

                    if ( n > maxN )
                        maxN = n;
                }
            }

            usernamedir = fmt::sprintf("%s%d", tmp, maxN + 1);
            
            usernamedir_len = usernamedir.size();

            disk_button->button_func71(1100, usernamedir + 'h');
        }
        else if ( r.code == 1163 )
        {
            field_0x1744 = 1;
            if ( !field_1612 )
            {
                usernamedir = get_lang_string(ypaworld__string_pointers, 366, "NEW GAME");
            }

            usernamedir_len = usernamedir.size();

            disk_button->button_func71(1100, usernamedir + 'h');
        }
        else if ( r.code == 1164)
        {
            switch ( field_0x1744 )
            {
            case 1:
                if ( field_1612 )
                {
                    const char *v112 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                    const char *v113 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                    sub_46D9E0(3, v113, v112, 0);
                }
                else
                {
                    sb_0x46ca74();
                }
                break;
            case 2:
                sub_46C914();
                break;
            case 3:
                if ( field_1612 )
                {
                    const char *v114 = get_lang_string(ypaworld__string_pointers, 2441, "2441");
                    const char *v115 = get_lang_string(ypaworld__string_pointers, 2436, "DO YOU WANT TO OVERWRITE THIS PLAYER STATUS?");
                    sub_46D9E0(6, v115, v114, 0);
                }
                else
                {
                    sb_0x46cdf8();
                }
                break;
            case 4:
                sub_46C748();
                break;
            default:
                break;
            }
        }
        else if (r.code == 1165)
        {
            if ( field_0x1744 )
            {
                field_0x1744 = 0;
            }
            else
                sub_46A7F8();
        }
        else if (r.code == 1250)
        {
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 758, "help\\18.html");
            field_0x1744 = 0;
        }
    }

    if ( envMode == ENVMODE_SELPLAYER ) // Multiplayer
    {
        disk_listvw.InputHandle(p_ypaworld, _input);

        if ( disk_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT || _input->KbdLastHit == Input::KC_UP || _input->KbdLastHit == Input::KC_DOWN )
        {
            field_1612 = disk_listvw.selectedEntry + 1;

            if ( field_1612 < 1 )
                field_1612 = 1;

            if ( field_1612 > disk_listvw.numEntries  )
                field_1612 = disk_listvw.numEntries;


            ProfileList::iterator it = profiles.begin();

            for (int i = 0; i < field_1612 - 1; i++) // check field_1612 - 1
            {
                if ( it == profiles.end() )
                {
                    field_1612 = 0;
                    break;
                }

                it++;
            }

            if (it != profiles.end())
            {
                usernamedir = it->name;
                usernamedir_len = usernamedir.size();
            }
        }
        disk_listvw.Formate(p_ypaworld);
    }

    if ( field_0x1744 )
    {
        v410.butID = 1105;
        disk_button->button_func66(&v410);

        v410.field_4 = 0;
        v410.butID = 1104;
        disk_button->button_func67(&v410);

        v410.butID = 1101;
        disk_button->button_func67(&v410);

        v410.butID = 1102;
        disk_button->button_func67(&v410);

        v410.butID = 1103;
        disk_button->button_func67(&v410);

        v410.butID = 1100;
        disk_button->button_func67(&v410);

        if ( field_0x1744 == 4 )
        {
            v410.field_4 = 0;
            v410.butID = 1105;

            if ( !field_1612 || !StriCmp(usernamedir, user_name) )
                disk_button->button_func67(&v410);
            else
                disk_button->button_func66(&v410);
        }

        if ( field_0x1744 == 2 && !field_1612 )
        {
            v410.field_4 = 0;
            v410.butID = 1105;
            disk_button->button_func67(&v410);
        }

        if ( field_0x1744 == 1 || field_0x1744 == 3 )
        {
            v410.butID = 1100;
            disk_button->button_func66(&v410);
        }

        std::string tmp = usernamedir;
        tmp.insert(usernamedir_len, 1, '_');
        
        disk_button->button_func71(1100, tmp);
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1105;
        disk_button->button_func67(&v410);

        v410.butID = 1104;
        disk_button->button_func66(&v410);

        v410.butID = 1101;
        disk_button->button_func66(&v410);

        v410.butID = 1103;
        disk_button->button_func66(&v410);

        v410.butID = 1100;
        disk_button->button_func67(&v410);

        v410.butID = 1102;
        if ( !StriCmp(usernamedir, user_name) )
            disk_button->button_func67(&v410);
        else
            disk_button->button_func66(&v410);

        disk_button->button_func71(1100, usernamedir);
    }

    if ( envMode == ENVMODE_SELLOCALE )
    {

        if ( _input->KbdLastHit == Input::KC_RETURN )
        {
            sub_46B0E0();
        }
        else if ( _input->KbdLastHit == Input::KC_ESCAPE )
        {
            sub_46AA0C();
        }

        if ( _input->HotKeyID == 43 )
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 761, "help\\111.html");
    }


    r = locale_button->button_func69(_input);

    if (r)
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 103 )
        {
            sub_46AA0C();
        }
        else if ( r.code == 1250 )
        {
            p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 761, "help\\111.html");
        }
        else if ( r.code == 1300 )
        {
            sub_46B0E0();
        }
        else if ( r.code == 1301 )
        {
            sub_46AA0C();
        }
    }

    if ( envMode == ENVMODE_SELLOCALE ) //Locale
    {
        local_listvw.InputHandle(p_ypaworld, _input);

        if ( local_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT )
        {
            field_19CA |= 1;

            Engine::StringList::iterator it = std::next(lang_dlls.begin(), local_listvw.selectedEntry);

            prev_lang = &(*it);
        }

        local_listvw.Formate(p_ypaworld);
    }

    r = about_button->button_func69(_input);

    if ( r )
    {
        if ( r.code == 103 )
        {
            envMode = ENVMODE_TITLE;

            about_button->Hide();
        }
    }

    if ( envMode == ENVMODE_ABOUT )
    {
        if ( _input->KbdLastHit == Input::KC_RETURN || _input->KbdLastHit == Input::KC_ESCAPE )
        {
            envMode = ENVMODE_TITLE;

            about_button->Hide();

            titel_button->Show();
        }
    }

    if ( envMode == ENVMODE_TITLE )
    {
        if ( field_19DA && glblTime - field_19D6 >= 700 )
        {
            field_19DA = 0;
        }
        else
        {
            switch ( field_19DA )
            {
            case 0:
                if ( p_ypaworld->sub_449678(_input, Input::KC_A) ) // VK_A
                {
                    field_19D6 = glblTime;
                    field_19DA++;
                }
                else
                {
                    if ( _input->KbdLastHit != Input::KC_NONE )
                        field_19DA = 0;
                }
                break;

            case 1:
                if ( p_ypaworld->sub_449678(_input, Input::KC_M) )
                {
                    field_19D6 = glblTime;
                    field_19DA++;
                }
                else
                {
                    if ( _input->KbdLastHit != Input::KC_NONE )
                        field_19DA = 0;
                }
                break;

            case 2:
                if ( p_ypaworld->sub_449678(_input, Input::KC_O) )
                {
                    field_19D6 = glblTime;
                    field_19DA++;
                }
                else
                {
                    if ( _input->KbdLastHit != Input::KC_NONE )
                        field_19DA = 0;
                }
                break;

            case 3:
                if ( p_ypaworld->sub_449678(_input, Input::KC_K) )
                {
                    ypaworld_func158__sub0__sub4();
                    SFXEngine::SFXe.startSound(&samples2_info, 3);
                }
                else
                {
                    if ( _input->KbdLastHit != Input::KC_NONE )
                        field_19DA = 0;
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        field_19DA = 0;
    }

    switch ( netSelMode )
    {
    case 0:
        nInputMode = 0;
        field_1C36 = 1;
        network_listvw.maxShownEntries = 12;
        field_0x1c30 = 3 * (p_ypaworld->font_default_h + word_5A50C2);
        break;
    case 1:
        nInputMode = 0;
        field_1C36 = 1;
        network_listvw.maxShownEntries = 12;
        field_0x1c30 = 3 * (p_ypaworld->font_default_h + word_5A50C2);
        break;
    case 3:
        nInputMode = 0;
        field_1C36 = 1;
        network_listvw.maxShownEntries = 12;
        field_0x1c30 = 3 * (p_ypaworld->font_default_h + word_5A50C2);
        break;
    case 2:
        field_1C36 = 0;
        network_listvw.maxShownEntries = 12;
        nInputMode = 1;
        break;
    case 4:
        nInputMode = 1;
        field_1C36 = 1;
        network_listvw.maxShownEntries = 6;
        field_0x1c30 = p_ypaworld->font_default_h * 9.5 + 2 * word_5A50C2;
        break;
    default:
        break;
    }


    yw_arg181 v346;
    uamessage_fraction fracMsg;

    windp_arg79 v368;

    r = network_button->button_func69(_input);

    if ( r )
    {
        if ( r.btn )
            sub_4DE248(r.btn);

        if ( r.code == 1204 || r.code == 1205 || r.code == 1206 || r.code == 1207 )
        {
            fracMsg.msgID = UAMSG_FRACTION;
            fracMsg.owner = 0;

            v346.recvID = 0;
            v346.garant = 1;
            v346.data = &fracMsg;
            v346.dataSize = sizeof(fracMsg);
            v346.recvFlags = 2;
        }

        if ( r.code == 103 || r.code == 1202 )
        {
            sub_46D698();
        }
        else if ( r.code == 1204 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = 1;
            SelectedFraction = 1;
            FreeFraction &= 0xFE;

            p_ypaworld->ypaworld_func181(&v346);
        }
        else if ( r.code == 1205 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = 2;
            FreeFraction &= 0xFD;
            SelectedFraction = 2;

            p_ypaworld->ypaworld_func181(&v346);
        }
        else if ( r.code == 1206 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = 4;
            SelectedFraction = 4;
            FreeFraction &= 0xFB;

            p_ypaworld->ypaworld_func181(&v346);
        }
        else if ( r.code == 1207 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = 8;
            SelectedFraction = 8;
            FreeFraction &= 0xF7;

            p_ypaworld->ypaworld_func181(&v346);
        }

        switch ( netSelMode )
        {
        case 0:
            if ( r.code == 1200 )
            {
                yw_NetOKProvider();
            }
            else if ( r.code == 1250 )
            {
                p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 753, "help\\13.html");
            }
            break;

        case 1:
            if ( r.code == 1200 )
            {
                yw_JoinNetGame();
            }
            else if ( r.code == 1201 )
            {
                isHost = 1;
                netSel = -1;
                network_listvw.firstShownEntries = 0;
                netSelMode = 3;
            }
            else if ( r.code == 1250 )
            {
                p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 755, "help\\15.html");
            }
            break;

        case 2:
            if ( r.code == 1200 )
            {
                if ( !netName.empty() )
                {
                    callSIGN = netName;

                    netSelMode = 1;
                    netSel = -1;
                    network_listvw.firstShownEntries = 0;
                    netName = "";

                    p_YW->GuiWinOpen( &network_listvw );
                }
            }
            else if ( r.code == 1201 )
            {
//                if ( str17_NOT_FALSE )
//                {
//                    windd_dlgBox v339;
//                    memset(&v339, 0, sizeof(windd_dlgBox));
//
//                    v339.title = get_lang_string(ypaworld__string_pointers, 413, "ENTER CALLSIGN");
//                    v339.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
//                    v339.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
//                    v339.maxLen = 32;
//                    v339.timer_func = NULL;
//                    v339.startText = netName;
//
//                    windd->windd_func322(&v339);
//
//                    if ( v339.result )
//                    {
//                        strncpy(netName, v339.result, 64);
//                        netName[63] = 0;
//                    }
//                }
            }
            else if ( r.code == 1250 )
            {
                p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 754, "help\\14.html");
            }
            break;

        case 3:
            if ( r.code == 1200 )
            {
                sub_46B328();
            }
            else if ( r.code == 1250 )
            {
                p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 756, "help\\16.html");
            }
            break;

        case 4:
            if ( r.code == 1200 )
            {
                if ( isHost )
                {
                    const char *v425;
                    const char *v425_1;

                    if ( p_ypaworld->windp->CountPlayers(NULL) <= 1 )
                    {
                        const char *v217 = get_lang_string(ypaworld__string_pointers, 2442, "2442");
                        const char *v218 = get_lang_string(ypaworld__string_pointers, 2435, "DO YOU REALLY WANT TO START WITHOUT OTHER PLAYERS?");
                        sub_46D9E0(2, v218, v217, 0);
                    }
                    else if ( ypaworld_func158__sub0__sub8(&v425, &v425_1) )
                    {
                        sub_46D9E0(4, v425, v425_1, 1);
                    }
                    else
                    {
                        sub_46DC1C();
                    }
                }
            }
            else if ( r.code == 1203 )
            {
                if ( isHost )
                {
                    netSel = -1;
                    network_listvw.firstShownEntries = 0;
                    msgBuffLine = 0;
                    lastSender[0] = 0;
                    netName = "";
                    netSelMode = 3;
                }
            }
            else if ( r.code == 1208 )
            {
                v368.mode = 0;
                v368.ID = 0;
                while ( p_ypaworld->windp->GetPlayerData(&v368) && StriCmp(v368.name, callSIGN) )
                    v368.ID++;

                yw_arg181 v353;
                uamessage_ready rdyMsg;

                rdyStart = 1;
                players2[v368.ID].rdyStart = 1;

                rdyMsg.msgID = UAMSG_READY;
                rdyMsg.owner = 0;
                rdyMsg.rdy = 1;

                v353.dataSize = sizeof(rdyMsg);
                v353.recvFlags = 2;
                v353.data = &rdyMsg;
                v353.recvID = 0;
                v353.garant = 1;

                p_ypaworld->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.receiverFlags = 2;
                v387.receiverID = 0;
                v387.senderFlags = 1;
                v387.senderID = callSIGN.c_str();
                v387.guarant = 1;

                p_ypaworld->windp->FlushBuffer(v387);
            }
            else if ( r.code == 1209 )
            {
                v368.mode = 0;
                v368.ID = 0;
                while ( p_ypaworld->windp->GetPlayerData(&v368) && StriCmp(v368.name, callSIGN) )
                    v368.ID++;


                yw_arg181 v353;
                uamessage_ready rdyMsg;

                rdyStart = 0;
                players2[v368.ID].rdyStart = 0;

                rdyMsg.msgID = UAMSG_READY;
                rdyMsg.owner = 0;
                rdyMsg.rdy = 0;

                v353.recvFlags = 2;
                v353.recvID = 0;
                v353.data = &rdyMsg;
                v353.dataSize = sizeof(rdyMsg);
                v353.garant = 1;

                p_ypaworld->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.receiverFlags = 2;
                v387.receiverID = 0;
                v387.senderFlags = 1;
                v387.senderID = callSIGN.c_str();
                v387.guarant = 1;

                p_ypaworld->windp->FlushBuffer(v387);
            }
            else if ( r.code == 1210 )
            {
//                if ( str17_NOT_FALSE )
//                {
//                    windd_dlgBox v316;
//                    memset(&v316, 0, sizeof(windd_dlgBox));
//
//                    v316.title = get_lang_string(ypaworld__string_pointers, 422, "ENTER MESSAGE");
//                    v316.ok = get_lang_string(ypaworld__string_pointers, 2, "OK");
//                    v316.cancel = get_lang_string(ypaworld__string_pointers, 3, "CANCEL");
//                    v316.startText = netName;
//                    v316.timer_func = NULL;
//                    v316.maxLen = 64;
//
//                    windd->windd_func322(&v316);
//
//                    if ( v316.result )
//                    {
//                        strncpy(netName, v316.result, 64);
//                        netName[63] = 0;
//                    }
//                }

                if ( !netName.empty() )
                {
                    uamessage_message msgMsg;
                    msgMsg.msgID = UAMSG_MESSAGE;
                    msgMsg.owner = 0;

                    strncpy(msgMsg.message, netName.c_str(), 64);

                    v346.senderFlags = 1;
                    v346.data = &msgMsg;
                    v346.dataSize = sizeof(msgMsg);
                    v346.recvFlags = 2;
                    v346.recvID = 0;
                    v346.senderID = callSIGN.c_str();
                    v346.garant = 1;

                    p_ypaworld->ypaworld_func181(&v346);

                    sub_4D0C24(p_ypaworld, callSIGN.c_str(), msgMsg.message);

                    netName = "";
                    netNameCurPos = 0;

                    int v223 = strtol(msgMsg.message, NULL, 0);
                    if ( v223 > 0 )
                        sub_4D9550(p_ypaworld, v223);
                }
            }
            else if ( r.code == 1250 )
            {
                p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 757, "help\\17.html");
            }
            break;
        default:
            break;
        }
    }

    if ( envMode == ENVMODE_NETPLAY )
    {
        int a4 = network_button->getBTN_y();

        network_listvw.y = field_0x1c30 + a4;

        network_listvw.InputHandle(p_ypaworld, _input);

        if ( (network_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT) || _input->KbdLastHit == Input::KC_UP || _input->KbdLastHit == Input::KC_DOWN )
        {
            netSel = network_listvw.selectedEntry;

            switch ( netSelMode )
            {
            case 0:
            case 1:
                nInputMode = 0;
                break;
            case 3:
            {
                int v227 = p_ypaworld->windp->CountPlayers(NULL);

                int v228 = 0;

                for (int i = 0; i < 256; i++)
                {
                    if ( v227 <= 1 || v227 <= p_ypaworld->LevelNet->mapInfos[ map_descriptions[i].id ].robos_count)
                    {
                        if ( v228 == netSel )
                        {
                            netLevelName = map_descriptions[i].pstring;
                            netLevelID = map_descriptions[i].id;
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

            netNameCurPos = netName.size();
        }

        network_listvw.Formate(p_ypaworld);
    }

    if ( envMode == ENVMODE_NETPLAY )
    {
        if ( _input->KbdLastHit != Input::KC_NONE || _input->chr || _input->HotKeyID >= 0 )
        {
            if ( nInputMode )
            {
                uint32_t v233;

                if ( netSelMode == 2 )
                    v233 = 32;
                else
                    v233 = 38;

                if ( netName.size() < v233 )
                {
                    if ( _input->chr > ' ' && _input->chr != '*' )
                    {
                        if (netNameCurPos <= (int)netName.size())
                        {
                            netName.insert(netNameCurPos, 1, _input->chr);
                            netNameCurPos++;
                        }                           
                    }
                }

                if ( _input->KbdLastHit == Input::KC_BACKSPACE && netNameCurPos > 0 )
                {
                    if (netNameCurPos > 0 && (int)netName.size() >= netNameCurPos)
                    {
                        netName.erase(netNameCurPos - 1, 1);                        
                        netNameCurPos--;
                    }
                }
                else if ( _input->KbdLastHit == Input::KC_LEFT )
                {
                    if ( netNameCurPos > 0 )
                        netNameCurPos--;
                }
                else if ( _input->KbdLastHit == Input::KC_RIGHT )
                {
                    if ( netNameCurPos < (int)netName.size() )
                        netNameCurPos++;
                }
                else if ( _input->KbdLastHit == Input::KC_DELETE && netNameCurPos < (int)netName.size() )
                {
                    if ( netNameCurPos < (int)netName.size() )
                        netName.erase(netNameCurPos, 1);
                }
            }

            if ( _input->KbdLastHit == Input::KC_RETURN )
            {
                switch ( netSelMode )
                {
                case 0:
                    yw_NetOKProvider();
                    break;

                case 1:
                    if ( network_listvw.numEntries )
                    {
                        yw_JoinNetGame();
                    }
                    else
                    {
                        isHost = 1;
                        netSel = -1;
                        netSelMode = 3;
                        network_listvw.firstShownEntries = 0;
                    }
                    break;

                case 2:
                    if ( !netName.empty() )
                    {
                        callSIGN = netName;

                        netSelMode = 1;
                        netSel = -1;
                        network_listvw.firstShownEntries = 0;
                        netName.clear();
                        p_YW->GuiWinOpen( &network_listvw );
                    }
                    break;

                case 3:
                    sub_46B328();
                    break;
                case 4:
                    if ( !netName.empty() )
                    {
                        uamessage_message msgMsg;
                        msgMsg.msgID = UAMSG_MESSAGE;
                        msgMsg.owner = 0;

                        strncpy(msgMsg.message, netName.c_str(), 64);

                        yw_arg181 v325;

                        v325.garant = 1;
                        v325.data = &msgMsg;
                        v325.dataSize = sizeof(msgMsg);
                        v325.recvFlags = 2;
                        v325.recvID = 0;

                        p_ypaworld->ypaworld_func181(&v325);

                        sub_4D0C24(p_ypaworld, callSIGN.c_str(), msgMsg.message);
                        netName.clear();
                        netNameCurPos = 0;

                        int v271 = strtol(msgMsg.message, NULL, 0);
                        if ( v271 > 0 )
                            sub_4D9550(p_ypaworld, v271);
                    }
                    break;
                default:
                    break;
                }
            }
            else if ( _input->KbdLastHit == Input::KC_ESCAPE )
            {
                sub_46D698();
            }

            if ( _input->HotKeyID == 43 && !nInputMode )
            {
                switch ( netSelMode )
                {
                case 0:
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 753, "help\\13.html");
                    break;
                case 1:
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 755, "help\\15.html");
                    break;
                case 2:
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 754, "help\\14.html");
                    break;
                case 3:
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 756, "help\\16.html");
                    break;
                case 4:
                    p_ypaworld->field_81AF = get_lang_string(ypaworld__string_pointers, 757, "help\\17.html");
                    break;
                default:
                    break;
                }
            }

            if ( netSel != -1 )
                network_listvw.PosOnSelected(netSel);

            _input->KbdLastHit = Input::KC_NONE;
        }
    }

    if ( isHost )
    {
        if ( netSelMode == 4 && envAction.action != EnvAction::ACTION_NETPLAY )
        {
            if ( (int)p_ypaworld->windp->CountPlayers(NULL)   <   p_ypaworld->LevelNet->mapInfos[ netLevelID ].robos_count )
            {
                if ( blocked )
                {
                    int v357 = 0;
                    p_ypaworld->windp->LockSession(&v357);

                    blocked = 0;
                }
            }
            else if ( !blocked )
            {
                int v357 = 1;
                p_ypaworld->windp->LockSession(&v357);

                blocked = 1;
            }
        }
    }

    v410.butID = 1201;
    network_button->button_func66(&v410);

    v410.butID = 1205;
    network_button->button_func67(&v410);

    v410.butID = 1202;
    network_button->button_func67(&v410);

    v410.butID = 1203;
    network_button->button_func66(&v410);

    v410.butID = 1225;
    network_button->button_func67(&v410);

    v410.butID = 1226;
    network_button->button_func67(&v410);

    v410.butID = 1227;
    network_button->button_func67(&v410);

    network_button->button_func71(1201, get_lang_string(ypaworld__string_pointers, 2, "OK"));

    v410.butID = 1220;
    network_button->button_func67(&v410);

    v410.butID = 1206;
    network_button->button_func67(&v410);

    v410.butID = 1207;
    network_button->button_func67(&v410);

    v410.butID = 1208;
    network_button->button_func67(&v410);

    v410.butID = 1209;
    network_button->button_func67(&v410);

    v410.butID = 1219;
    network_button->button_func67(&v410);

    v410.butID = 1221;
    network_button->button_func67(&v410);

    v410.field_4 = 0;
    v410.butID = 1210;
    network_button->button_func67(&v410);

    v410.butID = 1211;
    network_button->button_func67(&v410);

    v410.butID = 1212;
    network_button->button_func67(&v410);

    v410.butID = 1213;
    network_button->button_func67(&v410);

    v410.butID = 1214;
    network_button->button_func67(&v410);

    v410.butID = 1215;
    network_button->button_func67(&v410);

    v410.butID = 1216;
    network_button->button_func67(&v410);

    v410.butID = 1217;
    network_button->button_func67(&v410);

    if ( (netSelMode != 1 || p_ypaworld->windp->GetProvType() != 3)
            && (netSelMode != 1 || modemAskSession != 1 || p_ypaworld->windp->GetProvType() != 4)
            && netSelMode )
    {
        v410.butID = 1228;
        network_button->button_func67(&v410);
    }
    else
    {
        std::string v280;

        if ( netSelMode )
        {
            v280 = get_lang_string(ypaworld__string_pointers, 2402, "PRESS SPACEBAR TO UPDATE SESSION LIST");
        }
        else
        {
            if ( p_ypaworld->field_75E2[0] )
            {
                char *v278 = p_ypaworld->field_75E2;
                const char *v279 = get_lang_string(ypaworld__string_pointers, 2437, "YOUR TCP/IP ADDRESS");
                v280 = fmt::sprintf("%s  %s", v279, v278);
            }
            else
                v280 = " ";
        }

        network_button->button_func71(1228, v280);

        v410.butID = 1228;
        network_button->button_func66(&v410);
    }

    if ( !nInputMode && netSelMode != 2 )
    {
        v410.butID = 1200;
        network_button->button_func67(&v410);
    }
    else
    {
        v410.butID = 1200;
        network_button->button_func66(&v410);

        v393.xpos = -1;
        v393.butID = 1200;

        if ( netSelMode == 2 )
        {
            v393.width = dword_5A50B6;
            v393.ypos = 3 * (word_5A50C2 + p_ypaworld->font_default_h);
        }
        else
        {
            v393.width = dword_5A50B6 * 0.8;
            v393.ypos = 14 * (word_5A50C2 + p_ypaworld->font_default_h);
        }

        network_button->button_func76(&v393);

        std::string tmp = netName;
        if (tmp.size() >= (size_t)netNameCurPos)
            tmp.insert(netNameCurPos, 1, '_');
        
        network_button->button_func71(1200, tmp);
    }

    v393.xpos = -1;
    v393.width = -1;
    v393.butID = 1202;

    if ( netSelMode == 2 )
    {
        v393.ypos = 4 * (word_5A50C2 + p_ypaworld->font_default_h);
    }
    else
    {
        v393.ypos = (word_5A50C2 + p_ypaworld->font_default_h) * 15.2;
    }

    network_button->button_func76(&v393);

    switch ( netSelMode )
    {
    case 0:
        v410.field_4 = 0;
        v410.butID = 1205;

        network_button->button_func67(&v410);

        network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 410, "SELECT PROVIDER"));

        network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 425, "2"));

        network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 426, "3"));
        break;

    case 1:
        if ( p_ypaworld->windp->GetProvType() != 4 || !modemAskSession )
        {
            network_button->button_func71(1202, get_lang_string(ypaworld__string_pointers, 402, "NEW"));

            v410.butID = 1202;
            network_button->button_func66(&v410);
        }

        network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 411, "SELECT SESSION"));

        network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 428, "5"));

        network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 429, "6"));

        windp_getNameMsg msg;
        msg.id = 0;

        if ( p_ypaworld->windp->GetSessionName(&msg) )
        {
            network_button->button_func71(1201, get_lang_string(ypaworld__string_pointers, 406, "JOIN"));
        }
        else if ( p_ypaworld->windp->GetProvType() != 4 || modemAskSession )
        {
            v410.butID = 1201;
            network_button->button_func67(&v410);
        }
        else
        {
            network_button->button_func71(1201, get_lang_string(ypaworld__string_pointers, 421, "SEARCH"));
        }
        break;

    case 2:
        network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 413, "ENTER PLAYER"));

        network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 434, "11"));

        network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 435, "12"));

//        if ( str17_NOT_FALSE )
//        {
//            v410.butID = 1202;
//            network_button->button_func66(&v410);
//
//            network_button->button_func71(1202, get_lang_string(ypaworld__string_pointers, 423, "CHANGE"));
//        }
        break;

    case 3:
        if ( remoteMode )
        {
            v410.butID = 1205;
            network_button->button_func67(&v410);
        }

        network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 412, "SELECT LEVE"));

        network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 431, "8"));

        network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 432, "9"));
        break;

    case 4:
        v410.butID = 1225;
        network_button->button_func66(&v410);

        network_button->button_func71(1225, get_lang_string(ypaworld__string_pointers, 405, "SEND"));

        v410.butID = 1226;
        network_button->button_func66(&v410);

        v410.butID = 1227;
        network_button->button_func66(&v410);

        if ( netLevelID )
            network_button->button_func71(1226, netLevelName);
        else
            network_button->button_func71(1226, " ");

        if ( isHost )
        {
            v410.butID = 1205;
            network_button->button_func66(&v410);
        }

        if ( rdyStart )
        {
            if ( !isHost )
            {
                v410.butID = 1205;
                network_button->button_func67(&v410);
            }
        }

        v410.butID = 1220;
        network_button->button_func66(&v410);

        if ( netLevelID > 0 && netLevelID < 256 )
        {

            if ( !isHost && rdyStart )
            {
                v410.butID = 1220;
                network_button->button_func67(&v410);
            }
            else
            {
                v410.butID = 1206;
                if ( p_ypaworld->LevelNet->mapInfos[ netLevelID ].fractions_mask & 2 )
                    network_button->button_func66(&v410);
                else
                    network_button->button_func67(&v410);

                v410.butID = 1207;
                if ( p_ypaworld->LevelNet->mapInfos[ netLevelID ].fractions_mask & 0x40 )
                    network_button->button_func66(&v410);
                else
                    network_button->button_func67(&v410);

                v410.butID = 1208;
                if ( p_ypaworld->LevelNet->mapInfos[ netLevelID ].fractions_mask & 8 )
                    network_button->button_func66(&v410);
                else
                    network_button->button_func67(&v410);

                v410.butID = 1209;
                if ( p_ypaworld->LevelNet->mapInfos[ netLevelID ].fractions_mask & 0x10 )
                    network_button->button_func66(&v410);
                else
                    network_button->button_func67(&v410);
            }

            v408.butID = 0;

            switch ( SelectedFraction - 1 )
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
                network_button->button_func73(&v408);
            }

            int v298 = 0;

            v368.mode = 0;
            v368.ID = 0;

            field_1CD7 = 1;
            field_1CE8 = 1;

            if ( !(p_ypaworld->LevelNet->mapInfos[netLevelID].fractions_mask & 2) )
                v298 = 1;

            if ( !(p_ypaworld->LevelNet->mapInfos[netLevelID].fractions_mask & 0x40) )
                v298 |= 2;

            if ( !(p_ypaworld->LevelNet->mapInfos[netLevelID].fractions_mask & 8) )
                v298 |= 4;

            if ( !(p_ypaworld->LevelNet->mapInfos[netLevelID].fractions_mask & 0x10) )
                v298 |= 8;

            int v373, v374, v375, v376;

            while ( p_ypaworld->windp->GetPlayerData(&v368) )
            {
                int v299;

                if ( v368.flags & 1 )
                    v299 = SelectedFraction;
                else
                    v299 = players2[v368.ID].Fraction;

                if ( v299 & v298 )
                {
                    players2[v368.ID].trbl = 1;
                    field_1CD7 = 0;

                    switch ( v299 - 1 )
                    {
                    case 0:
                        players2[v375].trbl = 1;
                        break;
                    case 1:
                        players2[v373].trbl = 1;
                        break;
                    case 3:
                        players2[v376].trbl = 1;
                        break;
                    case 7:
                        players2[v374].trbl = 1;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    players2[v368.ID].trbl = 0;

                    switch ( v299 - 1 )
                    {
                    case 0:
                        v375 = v368.ID;
                        break;
                    case 1:
                        v373 = v368.ID;
                        break;
                    case 3:
                        v376 = v368.ID;
                        break;
                    case 7:
                        v374 = v368.ID;
                        break;
                    default:
                        break;
                    }

                }

                v298 |= v299;
                v368.ID++;
            }
        }

        v368.mode = 0;
        v368.ID = 0;

        while ( p_ypaworld->windp->GetPlayerData(&v368) )
        {
            if ( !(v368.flags & 1) )
            {
                if ( !players2[v368.ID].rdyStart )
                    field_1CD7 = 0;

                if ( !players2[v368.ID].welcmd )
                    field_1CE8 = 0;
            }
            v368.ID++;
        }

        if ( isHost )
        {
            network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 414, "START GAME OR ENTER MESSAGE TO THE PLAYERS"));

            network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 437, "14"));

            network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 438, "15"));

            network_button->button_func71(1201, get_lang_string(ypaworld__string_pointers, 407, "START"));

            if ( !field_1CD7 )
            {
                v410.field_4 = 0;
                v410.butID = 1201;
                network_button->button_func67(&v410);
            }
        }
        else
        {
            network_button->button_func71(1204, get_lang_string(ypaworld__string_pointers, 415, "WAIT FOR START OR SEND MESSAGES"));

            network_button->button_func71(1222, get_lang_string(ypaworld__string_pointers, 440, "17"));

            network_button->button_func71(1223, get_lang_string(ypaworld__string_pointers, 441, "18"));

            if ( field_1CE8 )
            {
                v410.butID = 1219;
                network_button->button_func66(&v410);

                v410.butID = 1221;
                network_button->button_func66(&v410);
            }

            v410.butID = 1201;
            network_button->button_func67(&v410);
        }
        v410.butID = 1210;
        network_button->button_func66(&v410);

        v410.butID = 1211;
        network_button->button_func66(&v410);

        v410.butID = 1212;
        network_button->button_func66(&v410);

        v410.butID = 1213;
        network_button->button_func66(&v410);

        v410.butID = 1214;
        network_button->button_func66(&v410);

        v410.butID = 1215;
        network_button->button_func66(&v410);

        v410.butID = 1216;
        network_button->button_func66(&v410);

        v410.butID = 1217;
        network_button->button_func66(&v410);

        v368.ID = 0;
        v368.mode = 0;


        for (int i = 0; i < 4; i++)
        {
            int v370;

            int v304 = p_ypaworld->windp->GetPlayerData(&v368);

            std::string name = " ";
            int btID;

            switch ( i )
            {
            case 0:
                v370 = 1214;
                btID = 1210;
                if ( v304 )
                    name = v368.name;
                break;

            case 1:
                v370 = 1215;
                btID = 1211;
                if ( v304 )
                    name = v368.name;
                break;

            case 2:
                v370 = 1216;
                btID = 1212;
                if ( v304 )
                    name = v368.name;
                break;

            case 3:
                v370 = 1217;
                btID = 1213;
                if ( v304 )
                    name = v368.name;
                break;

            default:
                break;
            }

            network_button->button_func71(btID, name);

            std::string v339("     "); // 5 spaces
            
            if ( v304 )
            {
                int v305;

                if ( v368.flags & 1 )
                {
                    v305 = SelectedFraction;
                }
                else
                {
                    v305 = players2[v368.ID].Fraction;
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
                    break;
                }
                if ( players2[v368.ID].trbl && ((glblTime / 300) & 1) )
                    v339[1] = 'f';

                if ( players2[v368.ID].rdyStart )
                    v339[2] = 'h';

                if ( players2[v368.ID].cd )
                    v339[3] = 'i';
            }

            network_button->button_func71(v370, v339);

            v368.ID++;
        }
        break;

    default:
        break;
    }

}

bool UserData::LoadSample(int block, int sampleID, const std::string &file)
{
    if (block < 0 || block >= 2)
        return false;

    std::string rsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", "data:");

    NC_STACK_wav *wav = Nucleus::CInit<NC_STACK_wav>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(file)}});
    if ( !wav )
        return false;

    if (block == 0)
    {
        samples1[sampleID] = wav;
        samples1_info.samples_data[sampleID].psampl = wav->getSMPL_pSample();

        if (sampleID == World::SOUND_ID_VOLUME ||
            sampleID == World::SOUND_ID_SLIDER ||
            sampleID == World::SOUND_ID_TEXTAPPEAR ||
            sampleID == World::SOUND_ID_TIMERCOUNT)
            samples1_info.samples_data[sampleID].flags |= 1;

    }
    else if (block == 1)
    {
        samples2[sampleID] = wav;
        samples2_info.samples_data[sampleID].psampl = wav->getSMPL_pSample();
    }

    set_prefix_replacement("rsrc", rsrc);
    return true;
}

bool UserData::ShellSoundsLoad()
{
    ScriptParser::HandlersList hndls {
        new World::Parsers::ShellSoundParser(this),
        new World::Parsers::ShellTracksParser(this),
    };

    return ScriptParser::ParseFile("env:world.ini", hndls, 0) || ScriptParser::ParseFile("data:world.ini", hndls, 0);
}

int UserData::InputIndexFromConfig(uint32_t type, uint32_t index)
{
    static const std::array<int, 5> BUTTON
    {
        World::INPUT_BIND_FIRE,       World::INPUT_BIND_CAMFIRE,
        World::INPUT_BIND_GUN,        World::INPUT_BIND_BRAKE,
        World::INPUT_BIND_WAPOINT
    };

    static const std::array<int, 6> SLIDER
    {
        World::INPUT_BIND_FLY_DIR,    World::INPUT_BIND_FLY_HEIGHT,
        World::INPUT_BIND_FLY_SPEED,  World::INPUT_BIND_DRIVE_DIR,
        World::INPUT_BIND_DRIVE_SPEED,World::INPUT_BIND_GUN_HEIGHT,
    };

    static const std::array<int, 47> HOTKEY
    {
        World::INPUT_BIND_ORDER,      World::INPUT_BIND_ATTACK,
        World::INPUT_BIND_NEW,        World::INPUT_BIND_ADD,
        World::INPUT_BIND_CONTROL,    -1,
        -1,                           World::INPUT_BIND_AUTOPILOT,
        World::INPUT_BIND_MAP,        World::INPUT_BIND_SQ_MANAGE,

        // 10
        World::INPUT_BIND_LANDLAYER,  World::INPUT_BIND_OWNER,
        World::INPUT_BIND_HEIGHT,     -1,
        World::INPUT_BIND_LOCKVIEW,   -1,
        World::INPUT_BIND_ZOOMIN,     World::INPUT_BIND_ZOOMOUT,
        World::INPUT_BIND_MINIMAP,    -1,

        // 20
        World::INPUT_BIND_NEXT_COMM,  World::INPUT_BIND_TO_HOST,
        World::INPUT_BIND_NEXT_UNIT,  World::INPUT_BIND_TO_COMM,
        World::INPUT_BIND_QUIT,       World::INPUT_BIND_HUD,
        -1,                           World::INPUT_BIND_LOG_WND,
        -1,                           -1,

        // 30
        -1,                           World::INPUT_BIND_LAST_MSG,
        World::INPUT_BIND_PAUSE,      -1,
        -1,                           -1,
        -1,                           World::INPUT_BIND_TO_ALL,
        World::INPUT_BIND_AGGR_1,     World::INPUT_BIND_AGGR_2,

        // 40
        World::INPUT_BIND_AGGR_3,     World::INPUT_BIND_AGGR_4,
        World::INPUT_BIND_AGGR_5,     World::INPUT_BIND_HELP,
        World::INPUT_BIND_LAST_SEAT,  World::INPUT_BIND_SET_COMM,
        World::INPUT_BIND_ANALYZER
    };

    if ( type == World::INPUT_BIND_TYPE_BUTTON && index < BUTTON.size())
        return BUTTON[index];
    else if ( type == World::INPUT_BIND_TYPE_SLIDER && index < SLIDER.size() )
        return SLIDER[index];
    else if ( type == World::INPUT_BIND_TYPE_HOTKEY && index < HOTKEY.size() )
        return HOTKEY[index];
    return -1;
}
