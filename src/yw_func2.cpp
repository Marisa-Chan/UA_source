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

#include "gui/uamsgbox.h"
#include "env.h"
#include "system/movie.h"
#include "system/inivals.h"
#include "loaders.h"
#include "obj3d.h"

extern int vertMenuSpace;
extern int dword_5A50B2;
extern int dword_5A50B2_h;
extern int word_5A50AE;
extern int word_5A50BC;
extern int word_5A50BA;
extern int word_5A50BE;
extern int buttonsSpace;

extern int dword_5A50B6;
extern int dword_5A50B6_h;

void sb_0x4eb94c__sub0(NC_STACK_ypaworld *yw, bool clockwise, int a3, vec3d *pos, baseRender_msg *arg)
{
    //brf_obj *brobj = &yw->brief.brf_objs + obj_id; // Only one object

    NC_STACK_base *model_base = yw->_vhclModels.at( yw->_vhclProtos[ yw->_briefScreen.ViewingObject.ID ].vp_normal );
    
    model_base->SetVizLimit(16000);
    model_base->SetFadeLength(100);

    model_base->SetPosition(*pos);

    if (clockwise)
    {
        yw->_briefScreen.ViewingObjectAngle += (arg->frameTime / 5);
        if ( yw->_briefScreen.ViewingObjectAngle >= 360 )
            yw->_briefScreen.ViewingObjectAngle -= 360;
    }
    else
    {
        yw->_briefScreen.ViewingObjectAngle -= (arg->frameTime / 5);
        if ( yw->_briefScreen.ViewingObjectAngle < 0 )
            yw->_briefScreen.ViewingObjectAngle += 360;
    }

    model_base->SetEulerRotation( a3 + 10, yw->_briefScreen.ViewingObjectAngle, 0);
    //printf("Try DRAW %d\n", (int)model_base);
    
    NC_STACK_base::CheckOpts(&yw->_briefScreen.ViewingObject.VP, model_base);
    
    model_base->Render(arg, yw->_briefScreen.ViewingObject.VP); //Draw vehicle
}

void sb_0x4eb94c__sub1(NC_STACK_ypaworld *yw, bool clockwise, int rot, vec3d *pos, baseRender_msg *arg)
{
    TSectorDesc *scType = &yw->_secTypeArray[yw->_briefScreen.ViewingObject.ID];

    NC_STACK_base *v7 = yw->_vhclModels.at(0);

    if (clockwise)
    {
        yw->_briefScreen.ViewingObjectAngle += (arg->frameTime / 5);
        if ( yw->_briefScreen.ViewingObjectAngle >= 360 )
            yw->_briefScreen.ViewingObjectAngle -= 360;
    }
    else
    {
        yw->_briefScreen.ViewingObjectAngle -= (arg->frameTime / 5);
        if ( yw->_briefScreen.ViewingObjectAngle < 0 )
            yw->_briefScreen.ViewingObjectAngle += 360;
    }

    v7->SetEulerRotation(rot + 10, yw->_briefScreen.ViewingObjectAngle, 0);

    int first;
    int demens;

    if ( scType->SectorType == 1 )
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
    for (int y = 0; y < demens; y++)
    {
        int v30 = first;
        for (int x = 0; x < demens; x++)
        {
            vec3d inSectorPos = vec3d(v30, 0.0, v22) * 300.0;

            NC_STACK_base *lego = yw->_legoArray[ scType->SubSectors.At(x, y)->HPModels[0] ].Base;
            lego->SetStatic(false);
            lego->SetVizLimit(16000);
            lego->SetFadeLength(100);

            lego->SetEulerRotation(rot + 10, yw->_briefScreen.ViewingObjectAngle, 0);
            lego->SetPosition( *pos + v7->TForm().SclRot.Transform( inSectorPos ) );
            
            NC_STACK_base::CheckOpts(&yw->_briefScreen.ViewingObject.VP, lego);
            
            lego->Render(arg, yw->_briefScreen.ViewingObject.VP);
            
            v30++;
        }
        v22++;
    }
}

void sb_0x4eb94c(NC_STACK_ypaworld *yw, TBriefengScreen *brf, TInputState *struc, int a5)
{
    brf->ObjRenderParams.frameTime = struc->Period;
    brf->ObjRenderParams.globTime = brf->CurrTime;

    TF::TForm3D v14;
    v14.Scale = vec3d(1.0, 1.0, 1.0);
    v14.SclRot = mat3x3::Ident();

    TF::Engine.SetViewPoint(&v14);
    v14.CalcGlobal();

    vec3d pos;

    if ( brf->ViewingObject ) // Not none
    {
        pos.x = (brf->ViewingObjectRect.right + brf->ViewingObjectRect.left) / 2.0;
        pos.y = (brf->ViewingObjectRect.bottom + brf->ViewingObjectRect.top) / 2.0;

        float v16;
        float v17;
        float v18;
        int rot;

        if ( brf->ViewingObject.ObjType == TBriefObject::TYPE_SECTOR )
        {
            v16 = 9600.0;
            v17 = 3600.0;
        }
        else if ( brf->ViewingObject.ObjType == TBriefObject::TYPE_VEHICLE )
        {
            float radius = yw->_vhclProtos[brf->ViewingObject.ID].radius;

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

        if ( brf->ViewingObject.ObjType == TBriefObject::TYPE_SECTOR )
        {
            sb_0x4eb94c__sub1(yw, true, rot, &pos, &brf->ObjRenderParams);
        }
        else if ( brf->ViewingObject.ObjType == TBriefObject::TYPE_VEHICLE )
        {
            sb_0x4eb94c__sub0(yw, true, rot, &pos, &brf->ObjRenderParams);
        }
    }
}

void ypaworld_func158__DrawVehicle(NC_STACK_ypaworld *yw, TBriefengScreen *brf, TInputState *struc)
{
    GFX::Engine.SetFBOBlending(1);
    GFX::Engine.BeginScene();

    brf->ObjRenderParams.frameTime = 1;
    brf->ObjRenderParams.globTime = 1;
    brf->ObjRenderParams.adeCount = 0;
    brf->ObjRenderParams.minZ = 17.0;
    brf->ObjRenderParams.maxZ = 32000.0;
    brf->ObjRenderParams.flags = GFX::RFLAGS_IGNORE_FALLOFF;

    if ( brf->ViewingObject ) // Not none
    {
        int v7 = brf->CurrTime - brf->ViewingObjectStartTime;
        if ( v7 > 50 )
            sb_0x4eb94c(yw, brf, struc, v7 - 50);
    }
    
    GFX::Engine.Rasterize();

    GFX::Engine.EndScene();
    GFX::Engine.SetFBOBlending(0);
}



void yw_draw_input_list(NC_STACK_ypaworld *yw, UserData *usr)
{
    usr->input_listview.SetRect(yw, -2, -2);
    GFX::Engine.GetTileset(0);

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

            if ( v24 == usr->inpListActiveElement )
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

            int v34 = usr->input_listview.entryWidth - 2 * usr->p_YW->_fontBorderW + 1;

            std::string v19;

            if ( usr->InputConfig[ v24 ].Type == World::INPUT_BIND_TYPE_SLIDER )
            {
                std::string negKeyName, posKeyName;
                if ( usr->InputConfig[ v24 ].NKeyCode )
                    negKeyName = Input::Engine.KeyTitle.at( usr->InputConfig[ v24 ].NKeyCode );
                else
                    negKeyName = "-";

                if ( usr->InputConfig[ v24 ].PKeyCode )
                    posKeyName = Input::Engine.KeyTitle.at( usr->InputConfig[ v24 ].PKeyCode );
                else
                    posKeyName = "-";

                if ( usr->InputConfig[ v24 ].SetFlags & UserData::TInputConf::IF_FIRST )
                    posKeyName = Locale::Text::Dialogs(Locale::DLG_I_UNK);

                if ( usr->InputConfig[ v24 ].SetFlags & UserData::TInputConf::IF_SECOND )
                    negKeyName = Locale::Text::Dialogs(Locale::DLG_I_UNK);

                v19 = fmt::sprintf("%s/%s", negKeyName, posKeyName);
            }
            else
            {
                if ( usr->InputConfig[ v24 ].PKeyCode )
                    v19 = Input::Engine.KeyTitle.at( usr->InputConfig[ v24 ].PKeyCode );
                else
                    v19 = "-";

                if ( usr->InputConfig[ v24 ].SetFlags & UserData::TInputConf::IF_FIRST )
                    v19 = Locale::Text::Dialogs(Locale::DLG_I_UNK);
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

            if ( v24 == usr->inpListActiveElement )
            {
                FontUA::set_txtColor(&v4, usr->p_YW->_iniColors[62].r, usr->p_YW->_iniColors[62].g, usr->p_YW->_iniColors[62].b);
            }
            else
            {
                FontUA::set_txtColor(&v4, usr->p_YW->_iniColors[61].r, usr->p_YW->_iniColors[61].g, usr->p_YW->_iniColors[61].b);
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

    GFX::Engine.DrawText(&v21);
}


void NC_STACK_ypaworld::LoadKeyNames()
{
    for (std::string &a: Input::Engine.KeyTitle)
        a.clear();

    Input::Engine.KeyTitle[Input::KC_NONE]       = "*"; // Locale::Text::GetKeyNameString(Locale::KEYNAME_NOP);
    Input::Engine.KeyTitle[Input::KC_ESCAPE]     = Locale::Text::KeyName(Locale::KEYNAME_ESC);
    Input::Engine.KeyTitle[Input::KC_SPACE]      = Locale::Text::KeyName(Locale::KEYNAME_SPACE);
    Input::Engine.KeyTitle[Input::KC_UP]         = Locale::Text::KeyName(Locale::KEYNAME_UP);
    Input::Engine.KeyTitle[Input::KC_DOWN]       = Locale::Text::KeyName(Locale::KEYNAME_DOWN);
    Input::Engine.KeyTitle[Input::KC_LEFT]       = Locale::Text::KeyName(Locale::KEYNAME_LEFT);
    Input::Engine.KeyTitle[Input::KC_RIGHT]      = Locale::Text::KeyName(Locale::KEYNAME_RIGHT);
    Input::Engine.KeyTitle[Input::KC_F1]         = Locale::Text::KeyName(Locale::KEYNAME_F1);
    Input::Engine.KeyTitle[Input::KC_F2]         = Locale::Text::KeyName(Locale::KEYNAME_F2);
    Input::Engine.KeyTitle[Input::KC_F3]         = Locale::Text::KeyName(Locale::KEYNAME_F3);
    Input::Engine.KeyTitle[Input::KC_F4]         = Locale::Text::KeyName(Locale::KEYNAME_F4);
    Input::Engine.KeyTitle[Input::KC_F5]         = Locale::Text::KeyName(Locale::KEYNAME_F5);
    Input::Engine.KeyTitle[Input::KC_F6]         = Locale::Text::KeyName(Locale::KEYNAME_F6);
    Input::Engine.KeyTitle[Input::KC_F7]         = Locale::Text::KeyName(Locale::KEYNAME_F7);
    Input::Engine.KeyTitle[Input::KC_F8]         = Locale::Text::KeyName(Locale::KEYNAME_F8);
    Input::Engine.KeyTitle[Input::KC_F9]         = Locale::Text::KeyName(Locale::KEYNAME_F9);
    Input::Engine.KeyTitle[Input::KC_F10]        = Locale::Text::KeyName(Locale::KEYNAME_F10);
    Input::Engine.KeyTitle[Input::KC_F11]        = Locale::Text::KeyName(Locale::KEYNAME_F11);
    Input::Engine.KeyTitle[Input::KC_F12]        = Locale::Text::KeyName(Locale::KEYNAME_F12);
    Input::Engine.KeyTitle[Input::KC_BACKSPACE]  = Locale::Text::KeyName(Locale::KEYNAME_BACK);
    Input::Engine.KeyTitle[Input::KC_TAB]        = Locale::Text::KeyName(Locale::KEYNAME_TAB);
    Input::Engine.KeyTitle[Input::KC_CLEAR]      = Locale::Text::KeyName(Locale::KEYNAME_CLEAR);
    Input::Engine.KeyTitle[Input::KC_RETURN]     = Locale::Text::KeyName(Locale::KEYNAME_RETURN);
    Input::Engine.KeyTitle[Input::KC_CTRL]       = Locale::Text::KeyName(Locale::KEYNAME_CTRL);
    Input::Engine.KeyTitle[Input::KC_SHIFT]      = Locale::Text::KeyName(Locale::KEYNAME_SHIFT);
    Input::Engine.KeyTitle[Input::KC_ALT]        = Locale::Text::KeyName(Locale::KEYNAME_ALT);
    Input::Engine.KeyTitle[Input::KC_PAUSE]      = Locale::Text::KeyName(Locale::KEYNAME_PAUSE);
    Input::Engine.KeyTitle[Input::KC_PGUP]       = Locale::Text::KeyName(Locale::KEYNAME_PGUP);
    Input::Engine.KeyTitle[Input::KC_PGDOWN]     = Locale::Text::KeyName(Locale::KEYNAME_PGDOWN);
    Input::Engine.KeyTitle[Input::KC_END]        = Locale::Text::KeyName(Locale::KEYNAME_END);
    Input::Engine.KeyTitle[Input::KC_HOME]       = Locale::Text::KeyName(Locale::KEYNAME_HOME);
    Input::Engine.KeyTitle[Input::KC_SELECT]     = Locale::Text::KeyName(Locale::KEYNAME_SELECT);
    Input::Engine.KeyTitle[Input::KC_EXECUTE]    = Locale::Text::KeyName(Locale::KEYNAME_EXEC);
    Input::Engine.KeyTitle[Input::KC_SNAPSHOT]   = Locale::Text::KeyName(Locale::KEYNAME_PRINT);
    Input::Engine.KeyTitle[Input::KC_INSERT]     = Locale::Text::KeyName(Locale::KEYNAME_INS);
    Input::Engine.KeyTitle[Input::KC_DELETE]     = Locale::Text::KeyName(Locale::KEYNAME_DEL);
    Input::Engine.KeyTitle[Input::KC_HELP]       = Locale::Text::KeyName(Locale::KEYNAME_HELP);
    Input::Engine.KeyTitle[Input::KC_1]          = Locale::Text::KeyName(Locale::KEYNAME_1);
    Input::Engine.KeyTitle[Input::KC_2]          = Locale::Text::KeyName(Locale::KEYNAME_2);
    Input::Engine.KeyTitle[Input::KC_3]          = Locale::Text::KeyName(Locale::KEYNAME_3);
    Input::Engine.KeyTitle[Input::KC_4]          = Locale::Text::KeyName(Locale::KEYNAME_4);
    Input::Engine.KeyTitle[Input::KC_5]          = Locale::Text::KeyName(Locale::KEYNAME_5);
    Input::Engine.KeyTitle[Input::KC_6]          = Locale::Text::KeyName(Locale::KEYNAME_6);
    Input::Engine.KeyTitle[Input::KC_7]          = Locale::Text::KeyName(Locale::KEYNAME_7);
    Input::Engine.KeyTitle[Input::KC_8]          = Locale::Text::KeyName(Locale::KEYNAME_8);
    Input::Engine.KeyTitle[Input::KC_9]          = Locale::Text::KeyName(Locale::KEYNAME_9);
    Input::Engine.KeyTitle[Input::KC_0]          = Locale::Text::KeyName(Locale::KEYNAME_0);
    Input::Engine.KeyTitle[Input::KC_A]          = Locale::Text::KeyName(Locale::KEYNAME_A);
    Input::Engine.KeyTitle[Input::KC_B]          = Locale::Text::KeyName(Locale::KEYNAME_B);
    Input::Engine.KeyTitle[Input::KC_C]          = Locale::Text::KeyName(Locale::KEYNAME_C);
    Input::Engine.KeyTitle[Input::KC_D]          = Locale::Text::KeyName(Locale::KEYNAME_D);
    Input::Engine.KeyTitle[Input::KC_E]          = Locale::Text::KeyName(Locale::KEYNAME_E);
    Input::Engine.KeyTitle[Input::KC_F]          = Locale::Text::KeyName(Locale::KEYNAME_F);
    Input::Engine.KeyTitle[Input::KC_G]          = Locale::Text::KeyName(Locale::KEYNAME_G);
    Input::Engine.KeyTitle[Input::KC_H]          = Locale::Text::KeyName(Locale::KEYNAME_H);
    Input::Engine.KeyTitle[Input::KC_I]          = Locale::Text::KeyName(Locale::KEYNAME_I);
    Input::Engine.KeyTitle[Input::KC_J]          = Locale::Text::KeyName(Locale::KEYNAME_J);
    Input::Engine.KeyTitle[Input::KC_K]          = Locale::Text::KeyName(Locale::KEYNAME_K);
    Input::Engine.KeyTitle[Input::KC_L]          = Locale::Text::KeyName(Locale::KEYNAME_L);
    Input::Engine.KeyTitle[Input::KC_M]          = Locale::Text::KeyName(Locale::KEYNAME_M);
    Input::Engine.KeyTitle[Input::KC_N]          = Locale::Text::KeyName(Locale::KEYNAME_N);
    Input::Engine.KeyTitle[Input::KC_O]          = Locale::Text::KeyName(Locale::KEYNAME_O);
    Input::Engine.KeyTitle[Input::KC_P]          = Locale::Text::KeyName(Locale::KEYNAME_P);
    Input::Engine.KeyTitle[Input::KC_Q]          = Locale::Text::KeyName(Locale::KEYNAME_Q);
    Input::Engine.KeyTitle[Input::KC_R]          = Locale::Text::KeyName(Locale::KEYNAME_R);
    Input::Engine.KeyTitle[Input::KC_S]          = Locale::Text::KeyName(Locale::KEYNAME_S);
    Input::Engine.KeyTitle[Input::KC_T]          = Locale::Text::KeyName(Locale::KEYNAME_T);
    Input::Engine.KeyTitle[Input::KC_U]          = Locale::Text::KeyName(Locale::KEYNAME_U);
    Input::Engine.KeyTitle[Input::KC_V]          = Locale::Text::KeyName(Locale::KEYNAME_V);
    Input::Engine.KeyTitle[Input::KC_W]          = Locale::Text::KeyName(Locale::KEYNAME_W);
    Input::Engine.KeyTitle[Input::KC_X]          = Locale::Text::KeyName(Locale::KEYNAME_X);
    Input::Engine.KeyTitle[Input::KC_Y]          = Locale::Text::KeyName(Locale::KEYNAME_Y);
    Input::Engine.KeyTitle[Input::KC_Z]          = Locale::Text::KeyName(Locale::KEYNAME_Z);
    Input::Engine.KeyTitle[Input::KC_NUM0]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_0);
    Input::Engine.KeyTitle[Input::KC_NUM1]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_1);
    Input::Engine.KeyTitle[Input::KC_NUM2]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_2);
    Input::Engine.KeyTitle[Input::KC_NUM3]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_3);
    Input::Engine.KeyTitle[Input::KC_NUM4]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_4);
    Input::Engine.KeyTitle[Input::KC_NUM5]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_5);
    Input::Engine.KeyTitle[Input::KC_NUM6]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_6);
    Input::Engine.KeyTitle[Input::KC_NUM7]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_7);
    Input::Engine.KeyTitle[Input::KC_NUM8]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_8);
    Input::Engine.KeyTitle[Input::KC_NUM9]       = Locale::Text::KeyName(Locale::KEYNAME_NUM_9);
    Input::Engine.KeyTitle[Input::KC_NUMMUL]     = Locale::Text::KeyName(Locale::KEYNAME_MUL);
    Input::Engine.KeyTitle[Input::KC_NUMPLUS]    = Locale::Text::KeyName(Locale::KEYNAME_ADD);
    Input::Engine.KeyTitle[Input::KC_NUMDOT]     = Locale::Text::KeyName(Locale::KEYNAME_DOT);
    Input::Engine.KeyTitle[Input::KC_NUMMINUS]   = Locale::Text::KeyName(Locale::KEYNAME_SUB);
    Input::Engine.KeyTitle[Input::KC_NUMENTER]   = Locale::Text::KeyName(Locale::KEYNAME_ENTER);
    Input::Engine.KeyTitle[Input::KC_NUMDIV]     = Locale::Text::KeyName(Locale::KEYNAME_DIV);
    Input::Engine.KeyTitle[Input::KC_EXTRA1]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_1);
    Input::Engine.KeyTitle[Input::KC_EXTRA2]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_2);
    Input::Engine.KeyTitle[Input::KC_EXTRA3]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_3);
    Input::Engine.KeyTitle[Input::KC_EXTRA4]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_4);
    Input::Engine.KeyTitle[Input::KC_EXTRA5]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_5);
    Input::Engine.KeyTitle[Input::KC_EXTRA6]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_6);
    Input::Engine.KeyTitle[Input::KC_EXTRA7]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_7);
    Input::Engine.KeyTitle[Input::KC_EXTRA8]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_8);
    Input::Engine.KeyTitle[Input::KC_EXTRA9]     = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_9);
    Input::Engine.KeyTitle[Input::KC_EXTRA10]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_10);
    Input::Engine.KeyTitle[Input::KC_EXTRA11]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_11);
    Input::Engine.KeyTitle[Input::KC_EXTRA12]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_12);
    Input::Engine.KeyTitle[Input::KC_EXTRA13]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_13);
    Input::Engine.KeyTitle[Input::KC_EXTRA14]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_14);
    Input::Engine.KeyTitle[Input::KC_EXTRA15]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_15);
    Input::Engine.KeyTitle[Input::KC_EXTRA16]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_16);
    Input::Engine.KeyTitle[Input::KC_EXTRA17]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_17);
    Input::Engine.KeyTitle[Input::KC_EXTRA18]    = Locale::Text::KeyName(Locale::KEYNAME_EXTRA_18);
    
    Input::Engine.KeyTitle[Input::KC_MMB]        = Locale::Text::KeyName(Locale::KEYNAME_MIDDLE_MOUSE);

    Input::Engine.KeyTitle[Input::KC_JOYB0]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB0);
    Input::Engine.KeyTitle[Input::KC_JOYB1]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB1);
    Input::Engine.KeyTitle[Input::KC_JOYB2]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB2);
    Input::Engine.KeyTitle[Input::KC_JOYB3]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB3);
    Input::Engine.KeyTitle[Input::KC_JOYB4]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB4);
    Input::Engine.KeyTitle[Input::KC_JOYB5]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB5);
    Input::Engine.KeyTitle[Input::KC_JOYB6]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB6);
    Input::Engine.KeyTitle[Input::KC_JOYB7]      = Locale::Text::KeyName(Locale::KEYNAME_JOYB7);
}


int yw_loadSky(NC_STACK_ypaworld *yw, const std::string &skyname)
{
    std::string tmprsrc = Common::Env.SetPrefix("rsrc", "data:");
    
    std::string skyfilename = fmt::sprintf("data:%s", skyname);

    NC_STACK_base *sky = Utils::ProxyLoadBase(skyfilename);
    yw->_skyObject = sky;
    if ( !sky )
    {
        ypa_log_out("Couldn't create %s\n", skyfilename.c_str());
        return 0;
    }
    
    Common::Env.SetPrefix("rsrc", tmprsrc);
    
    sky->SetStatic(true); // Don't rotate sky
    sky->SetVizLimit(yw->_skyVizLimit);
    sky->SetFadeLength(yw->_skyFadeLength);
    sky->ComputeStaticFog();
    sky->MakeVBO();
    return 1;
}

void NC_STACK_ypaworld::listSaveDir(const std::string &saveDir)
{
    auto savedStatuses = _playersStats;
    auto savedCallsign = _GameShell->netPlayerName;
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
                    _GameShell->profiles.emplace_back();
                    ProfilesNode &profile = _GameShell->profiles.back();

                    profile.name = dirNode->getName();

                    ScriptParser::HandlersList hndls{
                        new World::Parsers::UserParser(this)
                    };
                    std::string buf = fmt::sprintf("%s/%s/user.txt\n", saveDir, dirNode->getName());

                    if ( !ScriptParser::ParseFile(buf, hndls,0) )
                        ypa_log_out("Warning, cannot parse %s for time scanning\n", buf.c_str());
                    profile.fraction = 1;
                    profile.totalElapsedTime = _playersStats[1].ElapsedTime;
                }
            }
        }
    }
    else
    {
        ypa_log_out("Unknown Game-Directory %s\n", saveDir.c_str());
    }

    _playersStats = savedStatuses;
    _maxReloadConst = savedMaxreloadconst;
    _maxRoboEnergy = savedMaxroboenrgy;
    _GameShell->netPlayerName = savedCallsign;
}


void listLocaleDir(UserData *usr, const char *dirname)
{
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
                tmp.erase(v3);

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
                    usr->lang_dlls.emplace_back();
                    std::string &elm = usr->lang_dlls.back();

                    elm = tmp;

                    if ( !StriCmp(elm, "language") )
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
    disk_button->HideScreen();

    p_YW->GuiWinClose( &disk_listvw );

    if ( diskEnterFromMapSelect )
    {
        EnvMode = ENVMODE_SINGLEPLAY;
        sub_bar_button->ShowScreen();
    }
    else
    {
        EnvMode = ENVMODE_TITLE;
        titel_button->ShowScreen();
    }

    NC_STACK_button::button_66arg v3;
    v3.field_4 = 2;
    v3.butID = 1156;
    video_button->SetState(&v3);

    diskScreenMode = 0;
}



void NC_STACK_ypaworld::PlayIntroMovie()
{
    if ( !_movies[World::MOVIE_INTRO].empty() )
    {
        std::string buf = correctSeparatorAndExt( Common::Env.ApplyPrefix(_movies[World::MOVIE_INTRO]) );
        
        if ( System::IniConf::GfxMoviePlayer.Get<bool>() )
        {
            GFX::Engine.EndFrame();
            System::Movie.PlayMovie(buf, _GameShell->soundVolume);
            GFX::Engine.BeginFrame();
        }

        Input::Engine.QueryInput(&input_states);
        input_states.KbdLastHit = Input::KC_NONE;
        input_states.KbdLastDown = Input::KC_NONE;
        input_states.HotKeyID = -1;
    }
}



void ypaworld_func156__sub1(UserData *usr)
{
    usr->mapDescriptions.clear();
    
    for (size_t i = 0; i < usr->p_YW->_globalMapRegions.MapRegions.size(); i++)
    {
        if (usr->p_YW->_globalMapRegions.MapRegions[i].Status == TMapRegionInfo::STATUS_NETWORK)
            usr->mapDescriptions.push_back( UserData::TMapDescription(i, usr->p_YW->GetLevelName(i)) );
    }

    std::stable_sort(usr->mapDescriptions.begin(), usr->mapDescriptions.end(), UserData::TMapDescription::compare);
}


void UserData::GameShellUiOpenNetwork()
{
    titel_button->HideScreen();

    network_button->ShowScreen();

    EnvMode = ENVMODE_NETPLAY;

    p_YW->GuiWinClose( &network_listvw );
    p_YW->GuiWinOpen( &network_listvw );
}



void sb_0x46ca74__sub0(const std::string &a1, const std::string &a2)
{
    FSMgr::FileHandle *f1 = uaOpenFileAlloc(a1, "r");
    if ( f1 )
    {
        FSMgr::FileHandle *f2 = uaOpenFileAlloc(a2, "w");

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

    if ( diskListActiveElement )
    {
        if ( StriCmp(userNameDir, UserName) )
            sub_46D0F8(fmt::sprintf("save:%s", userNameDir));
    }
    else
    {
        profiles.emplace_back();
        ProfilesNode &profile = profiles.back();

        profile.name = userNameDir;

        std::string tmp = fmt::sprintf("save:%s", userNameDir);
        if ( !uaCreateDir(tmp) )
        {
            ypa_log_out("Unable to create directory %s\n", tmp.c_str());
            return;
        }

        disk_listvw.numEntries++;
        diskListActiveElement = disk_listvw.numEntries;
    }

    if ( ! p_YW->SaveSettings(this, fmt::sprintf("%s/user.txt", userNameDir), World::SDF_ALL) )
        ypa_log_out("Warning! Error while saving user data for %s\n", userNameDir.c_str());

    oldsave = fmt::sprintf("save:%s", UserName);

    if ( StriCmp(userNameDir, UserName) )
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
                    std::string v12 = fmt::sprintf("save:%s/%s", userNameDir, tmp);
                    sb_0x46ca74__sub0(v11.c_str(), v12.c_str());
                }
            }
        }
    }

    diskScreenMode = 0;
    UserName = userNameDir;

    disk_button->HideScreen();

    p_YW->GuiWinClose( &disk_listvw );

    if ( diskEnterFromMapSelect )
    {
        EnvMode = ENVMODE_SINGLEPLAY;

        sub_bar_button->ShowScreen();
    }
    else
    {
        EnvMode = ENVMODE_TITLE;

        titel_button->ShowScreen();
    }
}

void sb_0x47f810(NC_STACK_ypaworld *yw)
{
    yw->_roboProtos.clear();
    yw->_buildProtos.clear();
    
    yw->_weaponProtos.clear();

    yw->_vhclProtos.clear();
}

void sub_44A1FC(NC_STACK_ypaworld *yw)
{
    int v2 = 0;

    if ( yw->_GameShell )
    {
        FSMgr::FileHandle *fil = uaOpenFileAlloc("env:levels.def", "r");

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
                        yw->_globalMapRegions.MapRegions[tmp].Status = TMapRegionInfo::STATUS_ENABLED;
                }
            }

            v2 = 1;
            delete fil;
        }
    }

    if ( !v2 )
    {
        yw->_globalMapRegions.MapRegions[1].Status = TMapRegionInfo::STATUS_ENABLED;
        yw->_globalMapRegions.MapRegions[25].Status = TMapRegionInfo::STATUS_ENABLED;
        yw->_globalMapRegions.MapRegions[26].Status = TMapRegionInfo::STATUS_ENABLED;
        yw->_globalMapRegions.MapRegions[27].Status = TMapRegionInfo::STATUS_ENABLED;
    }
}

void UserData::sb_0x46cdf8()
{
    if ( ! p_YW->SaveSettings(this, fmt::sprintf("%s/user.txt", UserName), World::SDF_ALL) )
        ypa_log_out("Warning! Error while saving user data for %s\n", UserName.c_str());

    if ( diskListActiveElement )
    {
        sub_46D0F8(fmt::sprintf("save:%s", userNameDir));
    }
    else
    {
        profiles.emplace_back();
        ProfilesNode &profile = profiles.back();

        profile.name = userNameDir;

        std::string v10 = fmt::sprintf("save:%s", userNameDir);

        if ( !uaCreateDir(v10) )
        {
            ypa_log_out("Unable to create directory %s\n", v10.c_str());
            return;
        }

        UserName = userNameDir;
        disk_listvw.numEntries++;
        diskListActiveElement = disk_listvw.numEntries;
    }

    p_YW->_levelInfo.Buddies.clear();

    sb_0x47f810(p_YW);
    
    Common::DeleteAndNull(&p_YW->_script);

    if ( p_YW->ProtosInit() )
    {
        for (TMapRegionInfo &mp : p_YW->_globalMapRegions.MapRegions)
        {
            if ( mp.Status != TMapRegionInfo::STATUS_NONE && mp.Status != TMapRegionInfo::STATUS_NETWORK )
                mp.Status = TMapRegionInfo::STATUS_DISABLED;
        }

        sub_44A1FC(p_YW);

        p_YW->_maxRoboEnergy = 0;
        p_YW->_maxReloadConst = 0;

        p_YW->_playersStats.fill(World::TPlayerStatus());

        diskScreenMode = 0;

        p_YW->_beamEnergyCapacity = p_YW->_beamEnergyStart;

        p_YW->_levelInfo.JodieFoster.fill(0);

        sgmSaveExist = 0;

        disk_button->HideScreen();

        p_YW->GuiWinClose( &disk_listvw );

        EnvMode = ENVMODE_SINGLEPLAY;

        sub_bar_button->ShowScreen();
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
    confirm_button->Disable(&v4);

    v4.butID = 1301;
    confirm_button->Disable(&v4);

    confirm_button->HideScreen();

    confirmMode = 0;
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
    NC_STACK_ypaworld *yw = p_YW;

    bool forceChange = false;
    Common::Point resolution;

    if ( _settingsChangeOptions & 0x200 )
    {
        if ( confSoundFlags & World::SF_CDSOUND )
        {
            soundFlags |= World::SF_CDSOUND;
            yw->_preferences |= World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(true);
            if ( shelltrack )
            {
                SFXEngine::SFXe.SetMusicTrack(shelltrack, shelltrack__adv.min_delay, shelltrack__adv.max_delay);
                SFXEngine::SFXe.PlayMusicTrack();
            }
        }
        else
        {
            soundFlags &= ~World::SF_CDSOUND;
            yw->_preferences &= ~World::PREF_CDMUSICDISABLE;

            SFXEngine::SFXe.StopMusicTrack();
            SFXEngine::SFXe.SetMusicIgnoreCommandsFlag(false);
        }

    }

    if ( _settingsChangeOptions & 2 )
    {
        if ( confSoundFlags & World::SF_INVERTLR )
        {
            soundFlags |= World::SF_INVERTLR;
            SFXEngine::SFXe.setReverseStereo(1);
        }
        else
        {
            soundFlags &= ~World::SF_INVERTLR;
            SFXEngine::SFXe.setReverseStereo(0);
        }
    }

    if ( _settingsChangeOptions & 0x10 )
    {
        if ( confGFXFlags & World::GFX_FLAG_FARVIEW )
        {
            GFXFlags |= World::GFX_FLAG_FARVIEW;
            p_YW->SetFarView(true);
        }
        else
        {
            GFXFlags &= ~World::GFX_FLAG_FARVIEW;
            p_YW->SetFarView(false);
        }
    }

    if ( _settingsChangeOptions & 8 )
    {
        if ( confGFXFlags & World::GFX_FLAG_SKYRENDER )
        {
            GFXFlags |= World::GFX_FLAG_SKYRENDER;
            yw->setYW_skyRender(1);
        }
        else
        {
            GFXFlags &= ~World::GFX_FLAG_SKYRENDER;
            yw->setYW_skyRender(0);
        }

    }

    if ( _settingsChangeOptions & 0x800 )
    {
        if ( confGFXFlags & World::GFX_FLAG_SOFTMOUSE )
        {
            GFXFlags |= World::GFX_FLAG_SOFTMOUSE;
            yw->_preferences |= World::PREF_SOFTMOUSE;
            GFX::Engine.setWDD_cursor(1);
        }
        else
        {
            GFXFlags &= ~World::GFX_FLAG_SOFTMOUSE;
            yw->_preferences &= ~World::PREF_SOFTMOUSE;
            GFX::Engine.setWDD_cursor(0);
        }

    }

    if ( _settingsChangeOptions & 0x20 )
    {
        enemyIndicator = confEnemyIndicator;

        if ( enemyIndicator )
            p_YW->_preferences |= World::PREF_ENEMYINDICATOR;
        else
            p_YW->_preferences &= ~World::PREF_ENEMYINDICATOR;
    }

    if ( _settingsChangeOptions & 0x40 )
    {
        fxnumber = confFxNumber;
        yw->_fxLimit = fxnumber;
    }

    if ( _settingsChangeOptions & 0x100 )
    {
        musicVolume = confMusicVolume;

        SFXEngine::SFXe.SetMusicVolume(confMusicVolume);
    }

    if ( _settingsChangeOptions & 0x80 )
    {
        soundVolume = confSoundVolume;
        SFXEngine::SFXe.setMasterVolume(soundVolume);
    }

    if ( _settingsChangeOptions & 1 )
    {
        if ( _gfxMode != p_YW->_gfxMode && _gfxMode)
            p_YW->_gfxMode = _gfxMode;
    }

    if ( _settingsChangeOptions & 0x1000 )
    {
        if ( !conf3DGuid.empty() )
        {
            if ( StriCmp(conf3DGuid, win3d_guid) )
            {
                win3d_name = conf3DName;
                win3d_guid = conf3DGuid;

                GFX::Engine.SetDeviceByGUID(win3d_guid); //Save to file current gfx device

                p_YW->_gfxMode = Common::Point(GFX::DEFAULT_WIDTH, GFX::DEFAULT_HEIGHT);
                forceChange = true;
            }
        }
    }

    if ( _settingsChangeOptions & 4 )
    {
        if ( confGFXFlags & World::GFX_FLAG_16BITTEXTURE )
        {
            GFXFlags |= World::GFX_FLAG_16BITTEXTURE;
            GFX::Engine.setWDD_16bitTex(1);
        }
        else
        {
            GFXFlags &= ~World::GFX_FLAG_16BITTEXTURE;
            GFX::Engine.setWDD_16bitTex(0);
        }

        forceChange = true;
    }

    if ( _settingsChangeOptions & 0x400 )
    {
        if ( confGFXFlags & World::GFX_FLAG_WINDOWED )
            GFXFlags |= World::GFX_FLAG_WINDOWED;
        else
            GFXFlags &= ~World::GFX_FLAG_WINDOWED;

        forceChange = true;
    }

    if ( forceChange )
    {
        yw->SetGameShellVideoMode( IsWindowedFlag() );

        int v24 = 0;
        for (const GFX::GfxMode &nod : GFX::GFXEngine::Instance.GetAvailableModes())
        {
            if ( yw->_gfxMode == nod )
            {
                _gfxModeIndex = v24;
                video_button->SetText(1156, nod.name);

                break;
            }

            v24++;
        }
    }

    _settingsChangeOptions = 0;
    EnvMode = ENVMODE_TITLE;

    video_button->HideScreen();

    if ( video_listvw.IsOpen() )
        p_YW->GuiWinClose( &video_listvw );

    if ( d3d_listvw.IsOpen() )
        p_YW->GuiWinClose( &d3d_listvw );

    NC_STACK_button::button_66arg v38;
    v38.field_4 = 2;
    v38.butID = 1156;

    video_button->SetState(&v38);

    v38.butID = 1172;
    video_button->SetState(&v38);

    titel_button->ShowScreen();
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
    v6.senderID = netPlayerName.c_str();
    v6.senderFlags = 1;
    v6.receiverID = 0;
    v6.receiverFlags = 2;
    v6.guarant = 1;

    p_YW->_netDriver->FlushBuffer(v6);

    envAction.action = EnvAction::ACTION_NETPLAY;
    envAction.params[0] = netLevelID;
    network_listvw.firstShownEntries = 0;
    envAction.params[1] = netLevelID;

    int v12 = 1;
    p_YW->_netDriver->LockSession(&v12);

    yw_NetPrintStartInfo();
}


int UserData::ypaworld_func158__sub0__sub7()
{
    FSMgr::FileHandle *fl = uaOpenFileAlloc(fmt::sprintf("save:%s/sgisold.txt",UserName), "r");
    if ( !fl )
        return 0;

    delete fl;
    return 1;
}



void UserData::ShowToolTip(int id)
{
    switch (id)
    {
    default:
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_SAVE_LOAD:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_PROFILESCREEN);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_INPUT_SETTINGS:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_INPUTSCREEN);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_OPTIONS:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_VIDEOSCREEN);
        break;

    case 1005:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SOUNDSCREEN);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_QUIT:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_QUIT);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_LANGUAGE:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LANGSCREEN);
        break;

    case 1011:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_BRIEFRESET);
        break;

    case 1013:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_BRIEFFAST);
        break;

    case 1014:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_PLAYLEVEL);
        break;

    case 1015:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LOADLAST);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_MULTIPLAYER:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_NETSCREEN);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_HELP:
    case 1052:
    case 1107:
    case 1167:
    case 1218:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_HELP);
        break;

    case UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_SINGLE_PLAYER:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LVLSELECTMAP);
        break;

    case 1019:
        if ( p_YW->_levelInfo.State == TLevelInfo::STATE_MENU )
            p_YW->SetShowingTooltip(Locale::TIP_MENU_TOMAIN);
        else if ( p_YW->_levelInfo.State == TLevelInfo::STATE_DEBRIEFING )
            p_YW->SetShowingTooltip(Locale::TIP_MENU_EXITDEBRIEF);
        else
            p_YW->SetShowingTooltip(Locale::TIP_MENU_BACKTOLVLSEL);
        break;

    case 1020:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_GOTOLOADSAVE);
        break;

    case 1050:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_USEJOY);
        break;

    case 1051:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_APPLY);
        break;

    case 1053:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_RESET);
        break;

    case 1054:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_CANCEL);
        break;

    case 1055:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_FFJOY);
        break;

    case 1056:
        p_YW->SetShowingTooltip(Locale::TIP_INPUT_REMOVEKEY);
        break;

    case 1061:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_ALTJOY);
        break;

    case 1101:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LOADUSER);
        break;

    case 1102:
        p_YW->SetShowingTooltip(Locale::TIP_PROFILE_DEL);
        break;

    case 1103:
        p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_NEW);
        break;

    case 1104:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SAVEUSER);
        break;

    case 1105:
        switch ( diskScreenMode )
        {
        case 1:
            p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_SAVETHIS);
            break;
        case 2:
            p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_LOADTHIS);
            break;
        case 3:
            p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_NEWTHIS);
            break;
        case 4:
            p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_DELTHIS);
            break;
        default:
            break;
        }
        break;

    case 1106:
        switch ( diskScreenMode )
        {
        case 0:
            p_YW->SetShowingTooltip(Locale::TIP_PFOFILE_CLOSE);
            break;
        case 1:
            p_YW->SetShowingTooltip(Locale::TIP_PROFILE_CANCELSAVE);
            break;
        case 2:
            p_YW->SetShowingTooltip(Locale::TIP_PROFILE_CANCELLOAD);
            break;
        case 3:
            p_YW->SetShowingTooltip(Locale::TIP_PROFILE_CANCELNEW);
            break;
        case 4:
            p_YW->SetShowingTooltip(Locale::TIP_PROFILE_CANCELDEL);
            break;
        default:
            break;
        }
        break;
    case 1150:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_16BITTEX);
        break;

    case 1151:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_CHANNELS);
        break;

    case 1152:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_FXVOLUME);
        break;

    case 1154:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_MUSICVOLUME);
        break;

    case 1156:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_RESOLUTION);
        break;

    case 1157:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_FARDEPTH);
        break;

    case 1159:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_EXPLODES);
        break;

    case 1160:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_SKY);
        break;

    case 1161:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_APPLY);
        break;

    case 1162:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_CLOSE);
        break;

    case 1163:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_ENEMYSIGNS);
        break;

    case 1164:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_MUSIC);
        break;

    case 1165:
        p_YW->SetShowingTooltip(Locale::TIP_CONF_HWSWMOUSE);
        break;

    case 1166:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_WINDOWED); // windowed
        break;

    case 1172:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SEL3DDEV);
        break;

    case 1201:
        switch ( netSelMode )
        {
        case NETSCREEN_MODE_SELECT:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETSELPROV);
            break;
        case NETSCREEN_SESSION_SELECT:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETSELSESS);
            break;
        case NETSCREEN_ENTER_NAME:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETPLNAME);
            break;
        case NETSCREEN_CHOOSE_MAP:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETSELLVL);
            break;
        case NETSCREEN_INSESSION:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETSTART);
            break;
        default:
            break;
        }
        break;

    case 1202:
        if ( netSelMode == NETSCREEN_SESSION_SELECT )
        {
            p_YW->SetShowingTooltip(Locale::TIP_MENU_SELNEWLEVEL);
        }
        else if ( netSelMode == NETSCREEN_INSESSION )
        {
            p_YW->SetShowingTooltip(Locale::TIP_MENU_SENDTOALL);
        }
        break;

    case 1203:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_NETCLOSE);
        break;

    case 1205:
        switch ( netSelMode )
        {
        case NETSCREEN_SESSION_SELECT:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETBACKPLAYER);
            break;
        case NETSCREEN_ENTER_NAME:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETBACKPROV);
            break;
        case NETSCREEN_CHOOSE_MAP:
        case NETSCREEN_INSESSION:
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NETBACKSESS);
            break;
        default:
            break;
        }
        break;

    case 1206:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SELRESIST);
        break;

    case 1207:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SELGHOR);
        break;

    case 1208:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SELMYKO);
        break;

    case 1209:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SELTAER);
        break;

    case 1219:
        if ( rdyStart )
            p_YW->SetShowingTooltip(Locale::TIP_MENU_NOTREADY);
        else
            p_YW->SetShowingTooltip(Locale::TIP_MENU_READY);
        break;

    case 1225:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_SENDMSG);
        break;

    case 1250:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LANGCHOOSE);
        break;

    case 1251:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_LANGEXIT);
        break;

    case 1252:
        p_YW->SetShowingTooltip(Locale::TIP_MENU_HELP);
        break;
    }
}

// Go to options menu
void UserData::ShowOptionsMenu()
{
    titel_button->HideScreen();

    video_button->ShowScreen();

    EnvMode = ENVMODE_SETTINGS;

    if ( video_listvw.IsOpen() )
    {
        p_YW->GuiWinClose( &video_listvw );
        p_YW->GuiWinOpen( &video_listvw );
    }

    video_listvw.selectedEntry = _gfxModeIndex;
}

void UserData::ShowSaveLoadMenu()
{
    titel_button->HideScreen();

    disk_button->ShowScreen();

    EnvMode = ENVMODE_SELPLAYER;

    for( ProfileList::iterator it = profiles.begin(); it != profiles.end(); it++ )
    {
        if ( !StriCmp(it->name, UserName))
        {
            it->totalElapsedTime = p_YW->_playersStats[1].ElapsedTime;
            break;
        }
    }
    p_YW->GuiWinClose( &disk_listvw );
    p_YW->GuiWinOpen( &disk_listvw );
}

void UserData::ShowInputSettings()
{
    titel_button->HideScreen();

    button_input_button->ShowScreen();

    EnvMode = ENVMODE_INPUT;
    p_YW->GuiWinClose( &input_listview );
    p_YW->GuiWinOpen( &input_listview );
}

void sub_4D9550(NC_STACK_ypaworld *yw, int arg)
{
    UserData *usr = yw->_GameShell;

    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:");

    std::string wavName;
    if ( usr->default_lang_dll )
        wavName = fmt::sprintf("sounds/speech/%s/9%d.wav", *usr->default_lang_dll, arg);
    else
        wavName = fmt::sprintf("sounds/speech/language/9%d.wav", arg);

    if ( !uaFileExist(std::string("rsrc:") + wavName) )
        wavName = fmt::sprintf("sounds/speech/language/9%d.wav", arg);
    
    NC_STACK_sample *&pSmpl = usr->samples1.at(World::SOUND_ID_CHAT);

    if ( pSmpl )
    {
        SFXEngine::SFXe.sub_424000(&usr->samples1_info, World::SOUND_ID_CHAT);
        SFXEngine::SFXe.ForceStopSource(&usr->samples1_info, World::SOUND_ID_CHAT);
        pSmpl->Delete();
        pSmpl = NULL;
    }

    pSmpl = Nucleus::CInit<NC_STACK_wav>({{NC_STACK_rsrc::RSRC_ATT_NAME, wavName}});
    if ( pSmpl )
    {
        TSoundSource &pSnd = usr->samples1_info.Sounds.at(World::SOUND_ID_CHAT);
        pSnd.Volume = 500;
        pSnd.Pitch = 0;
        pSnd.PSample = pSmpl->GetSampleData();
        
        SFXEngine::SFXe.startSound(&usr->samples1_info, World::SOUND_ID_CHAT);
    }

    Common::Env.SetPrefix("rsrc", oldRsrc);
}

void sub_4D0C24(NC_STACK_ypaworld *yw, const std::string &a1, const std::string &a2)
{
    UserData *usr = yw->_GameShell;

    if ( StriCmp(a1, usr->lastSender) )
    {
        if ( usr->msgBuffers.size() )
            usr->msgBuffers.pop_front();

        usr->msgBuffers.push_back( fmt::sprintf("> %s:", a1));
        usr->lastSender = a1;
    }

    if ( usr->msgBuffers.size() >= 31 )
        usr->msgBuffers.pop_front();

    usr->msgBuffers.push_back( a2 );
   
    if ( usr->netSelMode == UserData::NETSCREEN_INSESSION )
    {
        int v22 = usr->msgBuffers.size() - 6;

        if ( v22 < 0 )
            v22 = 0;

        yw->_GameShell->network_listvw.firstShownEntries = v22;


        yw->_GameShell->network_listvw.numEntries = yw->_GameShell->msgBuffers.size();

        int v24;

        if ( usr->network_listvw.numEntries >= 6 )
            v24 = 6;
        else
            v24 = usr->network_listvw.numEntries;

        yw->_GameShell->network_listvw.shownEntries = v24;
    }
}


void UserData::yw_returnToTitle()
{
    yw_calcPlayerScore(p_YW);
    p_YW->FreeDebrief();

    sub_bar_button->HideScreen();

    titel_button->ShowScreen();

    EnvMode = ENVMODE_TITLE;
    returnToTitle = false;
}

void UserData::ShowLanguageMenu()
{
    titel_button->HideScreen();

    locale_button->ShowScreen();

    EnvMode = ENVMODE_SELLOCALE;
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
    yw->_briefScreen.Stage = TBriefengScreen::STAGE_CANCEL;
}

void UserData::ShowMenuMsgBox(int code, const std::string &txt1, const std::string &txt2, bool okOnly)
{
    _menuMsgBoxCode = code;
    
    Gui::UAMessageBox *bx = _menuMsgBox->GetMsgBox();
    bx->SetInform(okOnly);
    bx->Result = 0;
    bx->SetTexts(txt1, txt2);
    
    _menuMsgBox->ToFront();
    _menuMsgBox->SetEnable(true);
}


void UserData::ShowConfirmDialog( int a2, const std::string &txt1, const std::string &txt2, int a5)
{
    confirmMode = a2;

    NC_STACK_button::button_66arg v12;
    v12.butID = 1300;
    confirm_button->Enable(&v12);


    NC_STACK_button::button_arg76 v10;

    if ( a5 )
    {
        v10.butID = 1300;
        v10.xpos = p_YW->_screenSize.x * 0.4375;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        confirm_button->setXYWidth(&v10);
    }
    else
    {
        v12.butID = 1301;
        confirm_button->Enable(&v12);

        v10.butID = 1300;
        v10.xpos = p_YW->_screenSize.x * 0.25;
        v10.ypos = -1;
        v10.width = -1;
        //v11 = -1;
        confirm_button->setXYWidth(&v10);

        v10.butID = 1301;
        v10.xpos = p_YW->_screenSize.x * 0.625;
        confirm_button->setXYWidth(&v10);
    }

    confirm_button->SetText(1302, txt1);

    if ( !txt2.empty() )
        confirm_button->SetText(1303, txt2);
    else
        confirm_button->SetText(1303, " ");

    confirm_button->ShowScreen();
}

void ypaworld_func158__sub0__sub9(NC_STACK_ypaworld *yw)
{
    yw->_briefScreen.Stage = TBriefengScreen::STAGE_PLAYLEVEL;
}

void ShowInputSettings2(NC_STACK_ypaworld *yw)
{
    yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_RESTART;
}

void ShowInputSettings1(NC_STACK_ypaworld *yw)
{
    yw->_briefScreen.TimerStatus = TBriefengScreen::TIMER_FAST;
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
    EnvMode = ENVMODE_TITLE;
    InputConfCancel();

    NC_STACK_button::button_66arg v6;
    v6.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_INPUT_SETTINGS;
    v6.field_4 = 2;
    sub_bar_button->SetState(&v6);

    inputChangedParts = 0;

    v6.butID = 1050;
    v6.field_4 = (joystickEnabled == false) + 1;
    button_input_button->SetState(&v6);

    v6.field_4 = (altJoystickEnabled == false) + 1;
    v6.butID = 1061;
    button_input_button->SetState(&v6);

    v6.butID = 1055;
    v6.field_4 = ((p_YW->_preferences & World::PREF_FFDISABLE) != 0) + 1;
    button_input_button->SetState(&v6);

    button_input_button->HideScreen();

    p_YW->GuiWinClose( &input_listview );

    titel_button->ShowScreen();
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
    std::string file = fmt::sprintf("data:settings/%s/input.def", Locale::Text::GetLocaleName());

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

void UserData::sub_46C5F0()
{
    if ( video_listvw.selectedEntry != _gfxModeIndex)
    {
        _gfxModeIndex = video_listvw.selectedEntry;

        _gfxMode = GFX::GFXEngine::Instance.GetAvailableModes().at(_gfxModeIndex);
        video_button->SetText(1156, _gfxMode.name);
    }
}

//Options Cancel
void UserData::sub_46A3C0()
{
    _settingsChangeOptions = 0;
    EnvMode = ENVMODE_TITLE;

    int gfxId = GFX::GFXEngine::Instance.GetGfxModeIndex(p_YW->_gfxMode);
    
    if (gfxId < 0)
        gfxId = 0;

    video_listvw.selectedEntry = gfxId;
    _gfxModeIndex = gfxId;
    _gfxMode = p_YW->_gfxMode;

    video_button->SetText(1156, _gfxMode.name);

    video_button->SetText(1172, win3d_name);

    conf3DGuid = win3d_guid;
    conf3DName = win3d_name;

    NC_STACK_button::button_66arg v10;
    v10.butID = 1151;
    v10.field_4 = ((soundFlags & World::SF_INVERTLR) == 0) + 1;
    video_button->SetState(&v10);

    v10.field_4 = ((soundFlags & World::SF_CDSOUND) == 0) + 1;
    v10.butID = 1164;
    video_button->SetState(&v10);

    v10.butID = 1157;
    v10.field_4 = ((GFXFlags & World::GFX_FLAG_FARVIEW) == 0) + 1;
    video_button->SetState(&v10);

    v10.field_4 = ((GFXFlags & World::GFX_FLAG_SKYRENDER) == 0) + 1;
    v10.butID = 1160;
    video_button->SetState(&v10);

    v10.butID = 1150;
    v10.field_4 = ((GFXFlags & World::GFX_FLAG_16BITTEXTURE) == 0) + 1;
    video_button->SetState(&v10);

    v10.butID = 1166;
    v10.field_4 = (!IsWindowedFlag()) + 1;
    video_button->SetState(&v10);

    v10.butID = 1165;
    v10.field_4 = ((GFXFlags & World::GFX_FLAG_SOFTMOUSE) == 0) + 1;
    video_button->SetState(&v10);

    v10.field_4 = (enemyIndicator == 0) + 1;
    v10.butID = 1163;
    video_button->SetState(&v10);

    NC_STACK_button::Slider *tmp = video_button->GetSliderData(1159);
    tmp->value = fxnumber;
    video_button->Refresh(1159);

    tmp = video_button->GetSliderData(1152);
    tmp->value = soundVolume;
    video_button->Refresh(1152);

    tmp = video_button->GetSliderData(1154);
    tmp->value = musicVolume;
    video_button->Refresh(1154);

    video_button->HideScreen();

    if ( video_listvw.IsOpen() )
        p_YW->GuiWinClose( &video_listvw );

    if ( d3d_listvw.IsOpen() )
        p_YW->GuiWinClose( &d3d_listvw );

    titel_button->ShowScreen();

    v10.field_4 = 2;
    v10.butID = 1156;
    video_button->SetState(&v10);

    v10.butID = 1172;
    video_button->SetState(&v10);
}

void  UserData::UpdateSelected3DDevFromList()
{
    std::string name;
    std::string guid;

    const std::vector<GFX::TGFXDeviceInfo> &devices = GFX::Engine.GetDevices();
    
    if ((size_t)d3d_listvw.selectedEntry < devices.size())
    {
        const GFX::TGFXDeviceInfo &dev = devices.at(d3d_listvw.selectedEntry);
        if ( !StriCmp(dev.name, "software") )
            name = Locale::Text::Advanced(Locale::ADV_SOFTWARE);
        else
            name = dev.name;

        guid = dev.guid;
    }

    conf3DName = name;
    conf3DGuid = guid;

    video_button->SetText(1172, name);
}

void UserData::sub_46C914()
{
    if ( diskListActiveElement )
    {
        ProfileList::iterator it = profiles.begin();

        for (int i = 0; i < diskListActiveElement - 1; i++) // check usr->field_1612 - 1
            it++;

        EnvMode = ENVMODE_SINGLEPLAY;
printf("%s, %d\n",__FILE__, __LINE__);
        p_YW->LoadSettings(fmt::sprintf("%s/user.txt", it->name),
                           it->name,
                           World::SDF_ALL,
                           true);
printf("%s, %d\n",__FILE__, __LINE__);
        UserName = it->name;
        userNameDir = it->name;


        diskScreenMode = 0;
        sgmSaveExist = 0;

        disk_button->HideScreen();

        p_YW->GuiWinClose( &disk_listvw );

        sub_bar_button->ShowScreen();
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
    if ( diskListActiveElement )
    {
        if ( StriCmp(userNameDir, UserName) )
        {

            ProfileList::iterator it = profiles.begin();

            for (int i = 0; i < diskListActiveElement - 1; i++) // check usr->field_1612 - 1
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
                diskListActiveElement = 0;
                userNameDir = "NEWUSER";
            }
            else
            {
                if ( !HasElements )
                    diskListActiveElement--;

                userNameDir = nextIt->name;
            }

            userNameDirCursor = userNameDir.size();

            if ( diskListActiveElement )
                disk_listvw.PosOnSelected(diskListActiveElement - 1);

            diskScreenMode = 0;

            disk_button->HideScreen();

            p_YW->GuiWinClose( &disk_listvw );

            if ( diskEnterFromMapSelect )
            {
                EnvMode = ENVMODE_SINGLEPLAY;
                sub_bar_button->ShowScreen();
            }
            else
            {
                EnvMode = ENVMODE_TITLE;
                titel_button->ShowScreen();
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
            p_YW->ReloadLanguage();
        }
    }

    EnvMode = ENVMODE_TITLE;

    prev_lang = default_lang_dll;

    locale_button->HideScreen();

    p_YW->GuiWinClose( &local_listvw );

    titel_button->ShowScreen();
}

void UserData::ExitFromLanguageMenu()
{
    EnvMode = ENVMODE_TITLE;

    prev_lang = default_lang_dll;

    locale_button->HideScreen();

    p_YW->GuiWinClose( &local_listvw );

    titel_button->ShowScreen();
}


int NC_STACK_ypaworld::sub_449678(TInputState *struc, int kkode)
{
    return struc->KbdLastHit == kkode && ( (struc->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD) || _easyCheatKeys );
}

void UserData::ShowAbout()
{
    titel_button->HideScreen();

    about_button->ShowScreen();

    EnvMode = ENVMODE_ABOUT;
}


int ypaworld_func158__sub0__sub6(char a1)
{
    if (a1 == '/' || a1 == '\\' || a1 == ':' || a1 == '*' || a1 == '?' || a1 == '"' || a1 == '<' || a1 == '>' || a1 == '|')
        return 0;

    return 1;
}




void UserData::ConnectToServer(std::string connStr){
    fmt::printf("Connectiong to: %s\n", connStr);
    if ( p_YW->_netDriver->Connect(connStr) )
    {
        if (p_YW->_netDriver->HasLobby())
        {
            netSelMode = NETSCREEN_SESSION_SELECT;
            netSel = -1;
            network_listvw.firstShownEntries = 0;

            p_YW->GuiWinOpen( &network_listvw );
        }
        else
        {
            JoinLobbyLessGame();
        }
    }
    else
    {
        printf("Can't connect: Time OUT\n");
    }
}


void UserData::GameShellUiHandleInput()
{
    int v3 = 0;

    if ( Input->ClickInf.flag & TClickBoxInf::FLAG_BTN_DOWN )
        SFXEngine::SFXe.startSound(&samples1_info, 3);

    if ( netSelMode != NETSCREEN_MODE_SELECT )
        yw_HandleNetMsg(p_YW);

    if ( netSelMode == NETSCREEN_SESSION_SELECT )
    {
        if ( p_YW->_netDriver->GetProvType() == 4 )
        {
            if ( modemAskSession )
            {
                if ( Input->KbdLastHit == Input::KC_SPACE )
                {
                    GFX::Engine.windd_func320(NULL);
                    p_YW->_netDriver->EnumSessions(NULL);
                    GFX::Engine.windd_func321(NULL);
                }
            }
        }
        else if ( p_YW->_netDriver->GetProvType() != 3 || Input->KbdLastHit == Input::KC_SPACE )
        {
            p_YW->_netDriver->EnumSessions(NULL);
        }
    }

    NC_STACK_button::button_66arg v408;
    NC_STACK_button::button_66arg v410;

    v410.field_4 = 0;
    v410.butID = 1015;
    sub_bar_button->Disable(&v410);
    v410.butID = 1011;
    sub_bar_button->Disable(&v410);
    v410.butID = 1019;
    sub_bar_button->Disable(&v410);
    v410.butID = 1014;
    sub_bar_button->Disable(&v410);
    v410.butID = 1013;
    sub_bar_button->Disable(&v410);
    v410.butID = 1020;
    sub_bar_button->Disable(&v410);

    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_INPUT_SETTINGS;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_OPTIONS;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_SAVE_LOAD;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_LANGUAGE;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_QUIT;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_SINGLE_PLAYER;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_HELP;
    titel_button->Disable(&v410);
    v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_MULTIPLAYER;
    titel_button->Disable(&v410);

    NC_STACK_button::button_arg76 v393;
    v393.ypos = -1;
    v393.width = -1;
    //v394 = -1;
    v393.butID = 1014;
    v393.xpos = 0;
    sub_bar_button->setXYWidth(&v393);

    v393.butID = 1019;
    v393.xpos = p_YW->_screenSize.x - dword_5A50B6_h;
    sub_bar_button->setXYWidth(&v393);

    v393.butID = 1011;
    v393.xpos = buttonsSpace + dword_5A50B6_h;
    sub_bar_button->setXYWidth(&v393);

    sub_bar_button->SetText(1019,  Locale::Text::GlobMap(Locale::GLOBMAP_GOBACK));

    if ( p_YW->_levelInfo.State != TLevelInfo::STATE_MENU )
    {
        if ( p_YW->_levelInfo.State != TLevelInfo::STATE_DEBRIEFING && !GameIsOver )
        {
            v410.butID = 1014;
            sub_bar_button->Enable(&v410);

            v393.butID = 1014;
            v393.xpos = p_YW->_screenSize.x - dword_5A50B6_h;
            sub_bar_button->setXYWidth(&v393);

            v393.butID = 1019;
            v393.xpos = 0;
            sub_bar_button->setXYWidth(&v393);

            sub_bar_button->SetText(1019, Locale::Text::Advanced(Locale::ADV_BACK));
        }

        if ( p_YW->_levelInfo.State == TLevelInfo::STATE_DEBRIEFING )
        {
            v410.butID = 1011;
            sub_bar_button->Enable(&v410);

            v393.butID = 1011;
            v393.xpos = 0;
            sub_bar_button->setXYWidth(&v393);

            sub_bar_button->SetText(1019, Locale::Text::Advanced(Locale::ADV_CONTINUE));
        }

        v410.butID = 1019;
        sub_bar_button->Enable(&v410);

        button_input_button->HideScreen();
        video_button->HideScreen();
        disk_button->HideScreen();
        locale_button->HideScreen();
        network_button->HideScreen();

        p_YW->GuiWinClose( &input_listview );
        p_YW->GuiWinClose( &video_listvw );
        p_YW->GuiWinClose( &disk_listvw );
        p_YW->GuiWinClose( &local_listvw );
        p_YW->GuiWinClose( &network_listvw );
    }
    else if ( EnvMode == ENVMODE_TITLE )
    {
        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_INPUT_SETTINGS;
        titel_button->Enable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_OPTIONS;
        titel_button->Enable(&v410);

        v410.butID = 1001;
        titel_button->Enable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_LANGUAGE;
        if ( lang_dlls.size() > 1 )
            titel_button->Enable(&v410);
        else
            titel_button->Disable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_QUIT;
        titel_button->Enable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_HELP;
        titel_button->Enable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_SINGLE_PLAYER;
        titel_button->Enable(&v410);

        v410.butID = UIWidgets::MAIN_MENU_WIDGET_IDS::BTN_MULTIPLAYER;
        titel_button->Enable(&v410);
    }
    else if ( EnvMode == ENVMODE_SINGLEPLAY || EnvMode == ENVMODE_TUTORIAL )
    {
        if ( !sgmSaveExist )
        {
            if ( IsHasSGM(UserName, 0) )
                sgmSaveExist = 1;
            else
                sgmSaveExist = 2; // No, and We check it, so do not check it twice
        }

        if ( sgmSaveExist == 1 )
        {
            v410.butID = 1015;
            sub_bar_button->Enable(&v410);
        }

        v410.butID = 1019;
        sub_bar_button->Enable(&v410);

        v410.butID = 1020;
        sub_bar_button->Enable(&v410);
    }

    if ( confirmMode )
        v3 = 1;

    NC_STACK_button::ResCode r = confirm_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if ( r.code == 1350 ) // OK
        {
            switch ( confirmMode )
            {
            case 1:
            {
                sgmSaveExist = 0;
                p_YW->_isNetGame = false;

                sub_bar_button->HideScreen();

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
            if ( confirmMode == 3 || confirmMode == 6 )
                diskScreenMode = 0;

            sub_46D960();
        }
    }

    if ( confirmMode )
    {
        if ( Input->HotKeyID == 24 )
        {
            if ( confirmMode == 3 || confirmMode == 6 )
                diskScreenMode = 0;
            sub_46D960();
        }
        if ( Input->KbdLastHit == Input::KC_RETURN )
        {
            printf("Line = %d\n", __LINE__);
            switch ( confirmMode )
            {
            case 1:
            {
                sgmSaveExist = 0;
                p_YW->_isNetGame = false;
                sub_bar_button->HideScreen();
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
        Input->ClickInf.flag = 0;
        Input->KbdLastHit = Input::KC_NONE;
        Input->KbdLastDown = Input::KC_NONE;
        Input->chr = 0;
        Input->HotKeyID = -1;
    }    

    if ( EnvMode == ENVMODE_TITLE && Input->HotKeyID == 43 )
        p_YW->_helpURL = Locale::Text::Help(Locale::HELP_MAIN);

    r = titel_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_SAVE_LOAD_UP )
        {
            ShowSaveLoadMenu();
            diskEnterFromMapSelect = false;
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_OPTIONS_UP )
        {
            ShowOptionsMenu();
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_INPUT_SETTINGS_UP )
        {
            ShowInputSettings();
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_LANGUAGE_UP )
        {
            ShowLanguageMenu();
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_QUIT_UP )
        {
            titel_button->HideScreen();

            envAction.action = EnvAction::ACTION_QUIT;
            SFXEngine::SFXe.startSound(&samples1_info, 4);
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_MULTIPLAYER_UP )
        {
            GameShellUiOpenNetwork();
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_SINGLE_PLAYER_UP )
        {
            sub_bar_button->ShowScreen();

            titel_button->HideScreen();

            EnvMode = ENVMODE_SINGLEPLAY;
        }
        else if ( r.code == UIWidgets::MAIN_MENU_EVENT_IDS::BTN_HELP_UP )
        {
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_MAIN);
        }
    }

    if ( (EnvMode == ENVMODE_SINGLEPLAY || EnvMode == ENVMODE_TUTORIAL) && Input->HotKeyID == 24 )
    {
        if ( p_YW->_levelInfo.State != TLevelInfo::STATE_MENU )
        {
            sub_4EDCD8(p_YW);
            if ( p_YW->_preferences & World::PREF_CDMUSICDISABLE )
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
            sub_bar_button->HideScreen();

            titel_button->ShowScreen();

            EnvMode = ENVMODE_TITLE;
        }
    }

    r = sub_bar_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);
        switch ( r.code )
        {
        case 1013:
            if ( p_YW->_levelInfo.State != TLevelInfo::STATE_MENU )
            {
                sub_4EDCD8(p_YW);
                if ( p_YW->_preferences & World::PREF_CDMUSICDISABLE )
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
                sub_bar_button->HideScreen();

                titel_button->ShowScreen();

                EnvMode = ENVMODE_TITLE;
            }
            break;

        case 1019:
        {
            sgmSaveExist = 0;
            p_YW->_isNetGame = false;

            sub_bar_button->HideScreen();

            ypaworld_func158__sub0__sub9(p_YW);
        }
        break;

        case 1021:
            if ( ypaworld_func158__sub0__sub7() )
            {
                ShowConfirmDialog(1, Locale::Text::Advanced(Locale::ADV_WANTTOLOAD)
                            , Locale::Text::Advanced(Locale::ADV_DISCARDPROGRESS), 0);
            }
            else
            {
                ShowConfirmDialog(1, Locale::Text::Advanced(Locale::ADV_RLYLOAD)
                            , Locale::Text::Advanced(Locale::ADV_2440), 0);
            }
            break;

        case 1016:
            ShowInputSettings2(p_YW);
            break;

        case 1020:
            ShowInputSettings1(p_YW);
            break;

        case 1026:
            ShowSaveLoadMenu();
            diskEnterFromMapSelect = true;
            break;

        default:
            break;
        }
    }

    if ( EnvMode == ENVMODE_INPUT )
    {
        if ( !keyCatchMode && Input->HotKeyID == 43 )
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_INPUTCONF);

        if ( Input->KbdLastHit != Input::KC_NONE )
        {
            if ( keyCatchMode )
            {
                input_listview.listFlags &= ~GuiList::GLIST_FLAG_KEYB_INPUT;

                if ( !Input::Engine.KeyTitle.at( Input->KbdLastHit ).empty() )
                {
                    if ( confFirstKey )
                    {
                        InputConfig[inpListActiveElement].PKeyCode = Input->KbdLastHit;

                        if ( InputConfig[inpListActiveElement].Type == World::INPUT_BIND_TYPE_SLIDER )
                            confFirstKey = false;

                        keyCatchMode = false;
                        InputConfig[inpListActiveElement].SetFlags = 0;
                    }
                    else
                    {
                        InputConfig[inpListActiveElement].NKeyCode = Input->KbdLastHit;
                        InputConfig[inpListActiveElement].SetFlags &= ~TInputConf::IF_SECOND;
                        confFirstKey = true;
                    }
                }
                Input->KbdLastHit = Input::KC_NONE;
            }
            else
            {
                input_listview.listFlags |= GuiList::GLIST_FLAG_KEYB_INPUT;

                if ( Input->KbdLastHit == Input::KC_BACKSPACE  || Input->KbdLastHit == Input::KC_DELETE)
                {
                    if (InputConfig[inpListActiveElement].Type != World::INPUT_BIND_TYPE_SLIDER)
                        InputConfig[inpListActiveElement].PKeyCode = 0;
                }
                else if ( Input->KbdLastHit == Input::KC_RETURN )
                {
                                printf("Line = %d\n", __LINE__);

                    InputConfig[inpListActiveElement].SetFlags = (TInputConf::IF_FIRST | TInputConf::IF_SECOND);
                    keyCatchMode = true;
                    if ( InputConfig[inpListActiveElement].Type == World::INPUT_BIND_TYPE_SLIDER )
                        confFirstKey = false;
                }
                else if ( Input->KbdLastHit == Input::KC_ESCAPE )
                {
                    InputPageCancel();
                }
            }
        }
    }

    if ( InputConfig[inpListActiveElement].Type == World::INPUT_BIND_TYPE_SLIDER )
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        button_input_button->Disable(&v410);
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1056;
        button_input_button->Enable(&v410);
    }

    r = button_input_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if (r.code == 1050)
        {
            confJoystickEnabled = true;
            inputChangedParts |= ICHG_JOYSTICK;
        }
        else if (r.code == 1051)
        {
            confJoystickEnabled = false;
            inputChangedParts |= ICHG_JOYSTICK;
        }
        else if (r.code == 1052) // on OK press
        {
            if ( inputChangedParts & ICHG_JOYSTICK )
            {
                joystickEnabled = confJoystickEnabled;
                if ( confJoystickEnabled )
                    p_YW->_preferences &= ~World::PREF_JOYDISABLE;
                else
                    p_YW->_preferences |= World::PREF_JOYDISABLE;
            }

            if ( inputChangedParts & ICHG_ALTJOYSTICK )
            {
                altJoystickEnabled = confAltJoystickEnabled;
                if ( confAltJoystickEnabled )
                    p_YW->_preferences |= World::PREF_ALTJOYSTICK;
                else
                    p_YW->_preferences &= ~World::PREF_ALTJOYSTICK;
            }

            if ( inputChangedParts & ICHG_FORCEFEEDBACK )
            {
                if ( confFFEnabled )
                    p_YW->_preferences &= ~World::PREF_FFDISABLE;
                else
                    p_YW->_preferences |= World::PREF_FFDISABLE;
            }

            inputChangedParts = 0;
            sub_46D2B4();
            InputConfCopyToBackup();

            button_input_button->HideScreen();

            titel_button->ShowScreen();

            p_YW->GuiWinClose( &input_listview );
            EnvMode = ENVMODE_TITLE;
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
            confFFEnabled = false;
            inputChangedParts |= ICHG_FORCEFEEDBACK;
        }
        else if ( r.code == 1056 )
        {
            confFFEnabled = true;
            inputChangedParts |= ICHG_FORCEFEEDBACK;
        }
        else if ( r.code == 1057 )
        {
            InputConfig[ inpListActiveElement ].PKeyCode = 0;
        }
        else if ( r.code == 1058 )
        {
            confAltJoystickEnabled = true;
            inputChangedParts |= ICHG_ALTJOYSTICK;
        }
        else if ( r.code == 1059 )
        {
            confAltJoystickEnabled = false;
            inputChangedParts |= ICHG_ALTJOYSTICK;
        }
        else if ( r.code == 1250 )
        {
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_INPUTCONF);
            keyCatchMode = false;
        }
    }

    if ( EnvMode == ENVMODE_INPUT )
    {
        input_listview.InputHandle(p_YW, Input);

        inpListActiveElement = input_listview.selectedEntry + 1;

        if ( input_listview.listFlags & GuiList::GLIST_FLAG_IN_SELECT )
        {
            InputConfig[ inpListActiveElement ].SetFlags = 0;
            confFirstKey = true;
            keyCatchMode = false;
        }
        input_listview.Formate(p_YW);
    }

    if ( EnvMode == ENVMODE_SETTINGS )
    {
        if ( Input->KbdLastHit == Input::KC_RETURN )
        {
                        printf("Line = %d\n", __LINE__);

            if ( video_listvw.IsClosed() && d3d_listvw.IsClosed() )
            {
                if ( _settingsChangeOptions & 1 && _gfxMode != p_YW->_gfxMode && _gfxMode )
                {
                    ShowConfirmDialog(5, Locale::Text::Dialogs(Locale::DLG_S_RESCHANGE)
                                , Locale::Text::Dialogs(Locale::DLG_S_RESWARN), 0);
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

                    video_button->SetState(&v408);
                }

                _settingsChangeOptions |= 1;
                sub_46C5F0();
            }

        }
        else if ( Input->KbdLastHit == Input::KC_ESCAPE )
        {
            sub_46A3C0();
            EnvMode = ENVMODE_TITLE;
        }
        if ( Input->HotKeyID == 43 )
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_SETTINGS);
    }


    r = video_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if ( r.code == 1100 )
        {
            p_YW->GuiWinOpen( &video_listvw );
            SFXEngine::SFXe.startSound(&samples1_info, 7);

            Input->ClickInf.flag &= ~TClickBoxInf::FLAG_LM_DOWN;
        }
        else if ( r.code == 1101 )
        {
            p_YW->GuiWinClose( &video_listvw );
        }
        else if ( r.code == 1102 )
        {
            _settingsChangeOptions |= 0x10;
            confGFXFlags |= World::GFX_FLAG_FARVIEW;
        }
        else if ( r.code == 1103 )
        {
            confGFXFlags &= ~World::GFX_FLAG_FARVIEW;
            _settingsChangeOptions |= 0x10;
        }
        else if ( r.code == 1106 )
        {
            _settingsChangeOptions |= 8;
            confGFXFlags |= World::GFX_FLAG_SKYRENDER;
        }
        else if ( r.code == 1107 )
        {
            confGFXFlags &= ~World::GFX_FLAG_SKYRENDER;
            _settingsChangeOptions |= 8;
        }
        else if ( r.code == 1108 )
        {
            _settingsChangeOptions |= 0x40;
        }
        else if ( r.code == 1111 )
        {
            _settingsChangeOptions |= 2;
            confSoundFlags &= ~World::SF_INVERTLR;
        }
        else if ( r.code == 1112 )
        {
            confSoundFlags |= World::SF_INVERTLR;
            _settingsChangeOptions |= 2;
        }
        else if ( r.code == 1113 )
        {
            confGFXFlags |= World::GFX_FLAG_16BITTEXTURE;
            _settingsChangeOptions |= 4;
        }
        else if ( r.code == 1114 )
        {
            confGFXFlags &= ~World::GFX_FLAG_16BITTEXTURE;
            _settingsChangeOptions |= 4;
        }
        else if ( r.code == 1115 )
        {
            SFXEngine::SFXe.startSound(&samples1_info, 0);
            _settingsChangeOptions |= 0x80;
        }
        else if ( r.code == 1117 )
        {
            SFXEngine::SFXe.sub_424000(&samples1_info, 0);
        }
        else if ( r.code == 1118 )
        {
            _settingsChangeOptions |= 0x100;
        }
        else if ( r.code == 1124 )
        {
            if ( (_settingsChangeOptions & 1) &&  _gfxMode != p_YW->_gfxMode && _gfxMode )
            {
                ShowConfirmDialog(5, Locale::Text::Dialogs(Locale::DLG_S_RESCHANGE)
                            , Locale::Text::Dialogs(Locale::DLG_S_RESWARN), 0);
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
            confEnemyIndicator = true;
            _settingsChangeOptions |= 0x20;
        }
        else if ( r.code == 1127 )
        {
            confEnemyIndicator = false;
            _settingsChangeOptions |= 0x20;
        }
        else if ( r.code == 1128 )
        {
            _settingsChangeOptions |= 0x200;
            confSoundFlags |= World::SF_CDSOUND;
        }
        else if ( r.code == 1129 )
        {
            confSoundFlags &= ~World::SF_CDSOUND;
            _settingsChangeOptions |= 0x200;
        }
        else if ( r.code == 1130 )
        {
            _settingsChangeOptions |= 0x400;
            confGFXFlags |= World::GFX_FLAG_WINDOWED;
        }
        else if ( r.code == 1131 )
        {
            confGFXFlags &= ~World::GFX_FLAG_WINDOWED;
            _settingsChangeOptions |= 0x400;
        }
        else if ( r.code == 1132 )
        {
            _settingsChangeOptions |= 0x800;
            confGFXFlags |= World::GFX_FLAG_SOFTMOUSE;
        }
        else if ( r.code == 1133 )
        {
            confGFXFlags &= ~World::GFX_FLAG_SOFTMOUSE;
            _settingsChangeOptions |= 0x800;
        }
        else if ( r.code == 1134 )
        {
            p_YW->GuiWinOpen( &d3d_listvw );
            SFXEngine::SFXe.startSound(&samples1_info, 7);

            Input->ClickInf.flag &= ~TClickBoxInf::FLAG_LM_DOWN;
        }
        else if ( r.code == 1135 )
        {
            p_YW->GuiWinClose( &d3d_listvw );
        }
        else if ( r.code == 1250 )
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_SETTINGS);
    }

    if ( EnvMode == ENVMODE_SETTINGS && video_listvw.IsOpen() )
    {
        video_listvw.InputHandle(p_YW, Input);

        if ( video_listvw.listFlags & GuiList::GLIST_FLAG_SEL_DONE )
        {
            _settingsChangeOptions |= 1;
            
            if (!remoteMode)
                sub_46C5F0();
        }

        if ( video_listvw.IsClosed() )
        {
            v408.field_4 = 2;
            v408.butID = 1156;

            video_button->SetState(&v408);
        }

        video_listvw.Formate(p_YW);
    }

    if ( EnvMode == ENVMODE_SETTINGS && d3d_listvw.IsOpen() )
    {
        d3d_listvw.InputHandle(p_YW, Input);

        if ( d3d_listvw.listFlags & GuiList::GLIST_FLAG_SEL_DONE )
        {
            _settingsChangeOptions |= 0x1000;
            
            if (!remoteMode)
                UpdateSelected3DDevFromList();
        }

        if ( d3d_listvw.IsClosed() )
        {
            v408.field_4 = 2;
            v408.butID = 1172;

            video_button->SetState(&v408);
        }

        d3d_listvw.Formate(p_YW);
    }

    NC_STACK_button::Slider *v67 = video_button->GetSliderData(1159);
    
    video_button->SetText(1158, fmt::sprintf("%d", v67->value));
    confFxNumber = v67->value;

    v67 = video_button->GetSliderData(1152);

    video_button->SetText(1153, fmt::sprintf("%d", v67->value));
    confSoundVolume = v67->value;

    SFXEngine::SFXe.setMasterVolume(confSoundVolume);

    v67 = video_button->GetSliderData(1154);

    video_button->SetText(1155, fmt::sprintf("%d", v67->value));
    confMusicVolume = v67->value;

    SFXEngine::SFXe.SetMusicVolume(confMusicVolume);

    if ( EnvMode == ENVMODE_SELPLAYER ) //Load/Save
    {
        if ( Input->KbdLastHit != Input::KC_NONE || Input->chr )
        {
            if ( diskScreenMode )
            {
                if ( Input->KbdLastHit == Input::KC_BACKSPACE )
                {
                    if ( userNameDirCursor > 0 )
                    {
                        userNameDir.erase( userNameDirCursor - 1, 1 );
                        userNameDirCursor--;
                    }
                }
                else if ( Input->KbdLastHit == Input::KC_RETURN )
                {
                                printf("Line = %d\n", __LINE__);

                    switch ( diskScreenMode )
                    {
                    case 1:
                        if ( diskListActiveElement )
                        {
                            ShowConfirmDialog(3, Locale::Text::Advanced(Locale::ADV_WANTOVERWRITE)
                                        , Locale::Text::Advanced(Locale::ADV_EXISTSAVEDGAME), 0);
                        }
                        else if (userNameDir.size() > 0)
                        {
                            sb_0x46ca74();
                        }
                        break;

                    case 2:
                        sub_46C914();
                        break;

                    case 3:
                        if ( diskListActiveElement )
                        {
                            ShowConfirmDialog(6, Locale::Text::Advanced(Locale::ADV_WANTOVERWRITE)
                                        , Locale::Text::Advanced(Locale::ADV_EXISTSAVEDGAME), 0);
                        }
                        else if (userNameDir.size() > 0)
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
                else if ( Input->KbdLastHit == Input::KC_ESCAPE )
                {
                    diskScreenMode = 0;
                }
                else if ( Input->KbdLastHit == Input::KC_LEFT )
                {
                    if ( userNameDirCursor > 0 )
                        userNameDirCursor--;
                }
                else if ( Input->KbdLastHit == Input::KC_RIGHT )
                {
                    if ( userNameDirCursor < (int)userNameDir.size() )
                        userNameDirCursor++;
                }
                else if ( Input->KbdLastHit == Input::KC_DELETE )
                {
                    if ( userNameDirCursor < (int)userNameDir.size() )
                        userNameDir.erase(userNameDirCursor, 1);
                }

                if ( userNameDir.size() < 32 )
                {
                    if ( Input->chr >= ' ' )
                    {
                        if ( ypaworld_func158__sub0__sub6(Input->chr) )
                        {
                            userNameDir.insert(userNameDirCursor, 1, Input->chr);
                            userNameDirCursor++;
                        }
                    }
                }
            }
            else
            {
                if ( Input->KbdLastHit == Input::KC_ESCAPE )
                    sub_46A7F8();

                if ( Input->HotKeyID == 43 )
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_SAVEGAME);

            }

            if ( diskListActiveElement )
                disk_listvw.PosOnSelected(diskListActiveElement - 1);
        }
    }


    diskListActiveElement = 0;
    int v108 = 1;

    for ( ProfileList::iterator it = profiles.begin(); it != profiles.end(); it++)
    {
        if ( !StriCmp(it->name, userNameDir) )
        {
            diskListActiveElement = v108;
            break;
        }
        v108++;
    }

    r = disk_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if ( r.code == 103 )
        {
            sub_46A7F8();
        }
        else if ( r.code == 1160 )
        {
            diskScreenMode = 2;

            if ( !diskListActiveElement )
            {
                userNameDir = Locale::Text::Dialogs(Locale::DLG_P_UNNAMED);
            }
            
            userNameDirCursor = userNameDir.size();
                        
            disk_button->SetText(1100, userNameDir + 'h');
        }
        else if ( r.code == 1161 )
        {
            diskScreenMode = 4;
            if ( !diskListActiveElement )
            {
                userNameDir = Locale::Text::Dialogs(Locale::DLG_P_UNNAMED);
            }
            
            userNameDirCursor = userNameDir.size();

            disk_button->SetText(1100, userNameDir + 'h');
        }
        else if ( r.code == 1162 )
        {
            diskScreenMode = 3;

            std::string tmp = Locale::Text::Dialogs(Locale::DLG_P_UNNAMED);

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

            userNameDir = fmt::sprintf("%s%d", tmp, maxN + 1);
            
            userNameDirCursor = userNameDir.size();

            disk_button->SetText(1100, userNameDir + 'h');
        }
        else if ( r.code == 1163 )
        {
            diskScreenMode = 1;
            if ( !diskListActiveElement )
            {
                userNameDir = Locale::Text::Dialogs(Locale::DLG_P_UNNAMED);
            }

            userNameDirCursor = userNameDir.size();

            disk_button->SetText(1100, userNameDir + 'h');
        }
        else if ( r.code == 1164)
        {
            switch ( diskScreenMode )
            {
            case 1:
                if ( diskListActiveElement )
                {
                    ShowConfirmDialog(3, Locale::Text::Advanced(Locale::ADV_WANTOVERWRITE)
                                , Locale::Text::Advanced(Locale::ADV_EXISTSAVEDGAME), 0);
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
                if ( diskListActiveElement )
                {
                    ShowConfirmDialog(6, Locale::Text::Advanced(Locale::ADV_WANTOVERWRITE)
                                , Locale::Text::Advanced(Locale::ADV_EXISTSAVEDGAME), 0);
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
            if ( diskScreenMode )
            {
                diskScreenMode = 0;
            }
            else
                sub_46A7F8();
        }
        else if (r.code == 1250)
        {
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_SAVEGAME);
            diskScreenMode = 0;
        }
    }

    if ( EnvMode == ENVMODE_SELPLAYER ) // Multiplayer
    {
        disk_listvw.InputHandle(p_YW, Input);

        if ( disk_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT || Input->KbdLastHit == Input::KC_UP || Input->KbdLastHit == Input::KC_DOWN )
        {
            diskListActiveElement = disk_listvw.selectedEntry + 1;

            if ( diskListActiveElement < 1 )
                diskListActiveElement = 1;

            if ( diskListActiveElement > disk_listvw.numEntries  )
                diskListActiveElement = disk_listvw.numEntries;


            ProfileList::iterator it = profiles.begin();

            for (int i = 0; i < diskListActiveElement - 1; i++) // check field_1612 - 1
            {
                if ( it == profiles.end() )
                {
                    diskListActiveElement = 0;
                    break;
                }

                it++;
            }

            if (it != profiles.end())
            {
                userNameDir = it->name;
                userNameDirCursor = userNameDir.size();
            }
        }
        disk_listvw.Formate(p_YW);
    }

    if ( diskScreenMode )
    {
        v410.butID = 1105;
        disk_button->Enable(&v410);

        v410.field_4 = 0;
        v410.butID = 1104;
        disk_button->Disable(&v410);

        v410.butID = 1101;
        disk_button->Disable(&v410);

        v410.butID = 1102;
        disk_button->Disable(&v410);

        v410.butID = 1103;
        disk_button->Disable(&v410);

        v410.butID = 1100;
        disk_button->Disable(&v410);

        if ( diskScreenMode == 4 )
        {
            v410.field_4 = 0;
            v410.butID = 1105;

            if ( !diskListActiveElement || !StriCmp(userNameDir, UserName) )
                disk_button->Disable(&v410);
            else
                disk_button->Enable(&v410);
        }

        if ( diskScreenMode == 2 && !diskListActiveElement )
        {
            v410.field_4 = 0;
            v410.butID = 1105;
            disk_button->Disable(&v410);
        }

        if ( diskScreenMode == 1 || diskScreenMode == 3 )
        {
            v410.butID = 1100;
            disk_button->Enable(&v410);
        }

        std::string tmp = userNameDir;
        tmp.insert(userNameDirCursor, 1, '_');
        
        disk_button->SetText(1100, tmp);
    }
    else
    {
        v410.field_4 = 0;
        v410.butID = 1105;
        disk_button->Disable(&v410);

        v410.butID = 1104;
        disk_button->Enable(&v410);

        v410.butID = 1101;
        disk_button->Enable(&v410);

        v410.butID = 1103;
        disk_button->Enable(&v410);

        v410.butID = 1100;
        disk_button->Disable(&v410);

        v410.butID = 1102;
        if ( !StriCmp(userNameDir, UserName) )
            disk_button->Disable(&v410);
        else
            disk_button->Enable(&v410);

        disk_button->SetText(1100, userNameDir);
    }

    if ( EnvMode == ENVMODE_SELLOCALE )
    {

        if ( Input->KbdLastHit == Input::KC_RETURN )
        {
                        printf("Line = %d\n", __LINE__);

            sub_46B0E0();
        }
        else if ( Input->KbdLastHit == Input::KC_ESCAPE )
        {
            ExitFromLanguageMenu();
        }

        if ( Input->HotKeyID == 43 )
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_LOCALE);
    }


    r = locale_button->ProcessWidgetsEvents(Input);

    if (r)
    {
        if ( r.btn )
            ShowToolTip(r.btn);

        if ( r.code == 103 )
        {
            ExitFromLanguageMenu();
        }
        else if ( r.code == 1250 )
        {
            p_YW->_helpURL = Locale::Text::Help(Locale::HELP_LOCALE);
        }
        else if ( r.code == 1300 )
        {
            sub_46B0E0();
        }
        else if ( r.code == 1301 )
        {
            ExitFromLanguageMenu();
        }
    }

    if ( EnvMode == ENVMODE_SELLOCALE ) //Locale
    {
        local_listvw.InputHandle(p_YW, Input);

        if ( local_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT )
        {
            Engine::StringList::iterator it = std::next(lang_dlls.begin(), local_listvw.selectedEntry);

            prev_lang = &(*it);
        }

        local_listvw.Formate(p_YW);
    }

    r = about_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.code == 103 )
        {
            EnvMode = ENVMODE_TITLE;

            about_button->HideScreen();
        }
    }

    if ( EnvMode == ENVMODE_ABOUT )
    {
        if ( Input->KbdLastHit == Input::KC_RETURN || Input->KbdLastHit == Input::KC_ESCAPE )
        {
                        printf("Line = %d\n", __LINE__);

            EnvMode = ENVMODE_TITLE;

            about_button->HideScreen();

            titel_button->ShowScreen();
        }
    }

    if ( EnvMode == ENVMODE_TITLE )
    {
        if ( aboutDlgKeyCount && GlobalTime - aboutDlgLastKeyTime >= 700 )
        {
            aboutDlgKeyCount = 0;
        }
        else
        {
            switch ( aboutDlgKeyCount )
            {
            case 0:
                if ( p_YW->sub_449678(Input, Input::KC_A) ) // VK_A
                {
                    aboutDlgLastKeyTime = GlobalTime;
                    aboutDlgKeyCount++;
                }
                else
                {
                    if ( Input->KbdLastHit != Input::KC_NONE )
                        aboutDlgKeyCount = 0;
                }
                break;

            case 1:
                if ( p_YW->sub_449678(Input, Input::KC_M) )
                {
                    aboutDlgLastKeyTime = GlobalTime;
                    aboutDlgKeyCount++;
                }
                else
                {
                    if ( Input->KbdLastHit != Input::KC_NONE )
                        aboutDlgKeyCount = 0;
                }
                break;

            case 2:
                if ( p_YW->sub_449678(Input, Input::KC_O) )
                {
                    aboutDlgLastKeyTime = GlobalTime;
                    aboutDlgKeyCount++;
                }
                else
                {
                    if ( Input->KbdLastHit != Input::KC_NONE )
                        aboutDlgKeyCount = 0;
                }
                break;

            case 3:
                if ( p_YW->sub_449678(Input, Input::KC_K) )
                {
                    ShowAbout();
                    SFXEngine::SFXe.startSound(&samples1_info, World::SOUND_ID_SECRET);
                }
                else
                {
                    if ( Input->KbdLastHit != Input::KC_NONE )
                        aboutDlgKeyCount = 0;
                }
                break;
            default:
                break;
            }
        }
    }
    else
    {
        aboutDlgKeyCount = 0;
    }

    switch ( netSelMode )
    {
    case NETSCREEN_MODE_SELECT:
        nInputMode = 0;
        network_listvw.maxShownEntries = 12;
        netListY = 3 * (p_YW->_fontH + vertMenuSpace);
        break;
    case NETSCREEN_SESSION_SELECT:
        nInputMode = 0;
        network_listvw.maxShownEntries = 12;
        netListY = 3 * (p_YW->_fontH + vertMenuSpace);
        break;
    case NETSCREEN_CHOOSE_MAP:
        nInputMode = 0;
        network_listvw.maxShownEntries = 12;
        netListY = 3 * (p_YW->_fontH + vertMenuSpace);
        break;
    case NETSCREEN_ENTER_NAME:
        network_listvw.maxShownEntries = 12;
        nInputMode = 1;
        break;
    case NETSCREEN_ENTER_IP:
        network_listvw.maxShownEntries = 12;
        nInputMode = 1;
        break;
    case NETSCREEN_INSESSION:
        nInputMode = 1;
        network_listvw.maxShownEntries = 6;
        netListY = p_YW->_fontH * 9.5 + 2 * vertMenuSpace;
        break;
    default:
        break;
    }


    yw_arg181 v346;
    uamessage_fraction fracMsg;

    r = network_button->ProcessWidgetsEvents(Input);

    if ( r )
    {
        if ( r.btn )
            ShowToolTip(r.btn);

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
            ExitFromNetworkToMain();
        }
        else if ( r.code == 1204 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = NET_FRACTION_RESISTANCE;
            SelectedFraction = NET_FRACTION_RESISTANCE;
            FreeFraction &= ~NET_FRACTION_RESISTANCE;

            p_YW->ypaworld_func181(&v346);
        }
        else if ( r.code == 1205 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = NET_FRACTION_GHORKOV;
            FreeFraction &= ~NET_FRACTION_GHORKOV;
            SelectedFraction = NET_FRACTION_GHORKOV;

            p_YW->ypaworld_func181(&v346);
        }
        else if ( r.code == 1206 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = NET_FRACTION_MIKO;
            SelectedFraction = NET_FRACTION_MIKO;
            FreeFraction &= ~NET_FRACTION_MIKO;

            p_YW->ypaworld_func181(&v346);
        }
        else if ( r.code == 1207 )
        {
            fracMsg.freefrac = SelectedFraction;
            FreeFraction |= SelectedFraction;
            fracMsg.newfrac = NET_FRACTION_TAER;
            SelectedFraction = NET_FRACTION_TAER;
            FreeFraction &= ~NET_FRACTION_TAER;

            p_YW->ypaworld_func181(&v346);
        }

        switch ( netSelMode )
        {
        case NETSCREEN_MODE_SELECT:
            if ( r.code == 1200 )
            {
                yw_NetOKProvider();
            }
            else if ( r.code == 1250 )
            {
                p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELPROV);
            }
            break;

        case NETSCREEN_SESSION_SELECT:
            if ( r.code == 1200 )
            {
                yw_JoinNetGame();
            }
            else if ( r.code == 1201 )
            {
                isHost = true;
                netSel = -1;
                network_listvw.firstShownEntries = 0;
                netSelMode = NETSCREEN_CHOOSE_MAP;
            }
            else if ( r.code == 1250 )
            {
                p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELSESS);
            }
            break;

        case NETSCREEN_ENTER_IP:
            if ( r.code == 1200 ){
                if ( !netName.empty() )
                {
                    std::string ip = netName;
                    netName = "";
                    ConnectToServer(ip);
                }
                else
                {
                    netName = "127.0.0.1";
                    netNameCurPos = netName.size();
                }
            }
            break;

        case NETSCREEN_ENTER_NAME:
            if ( r.code == 1200 )
            {
                if ( !netName.empty() )
                {
                    netPlayerName = netName;
                    netName = "";
                }
                p_YW->_netDriver->SetWantedName(netPlayerName);
                switch ( p_YW->_netDriver->GetMode() )
                {
                    case 1:
                        isHost = true;
                        netSel = -1;
                        network_listvw.firstShownEntries = 0;
                        netSelMode = NETSCREEN_CHOOSE_MAP;
                        p_YW->GuiWinOpen( &network_listvw );
                        break;

                    case 2:
                    {
                        netSelMode = NETSCREEN_ENTER_IP;
                        netName = "";
                        netNameCurPos = 0;
                    }
                        break;

                    default:
                        break;
                }
            }
            else if ( r.code == 1201 )
            {
//                if ( str17_NOT_FALSE )
//                {
//                    windd_dlgBox v339;
//                    memset(&v339, 0, sizeof(windd_dlgBox));
//
//                    v339.title = get_lang_string(413, "ENTER CALLSIGN");
//                    v339.ok = get_lang_string(2, "OK");
//                    v339.cancel = get_lang_string(3, "CANCEL");
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
                p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETNAME);
            }
            break;

        case NETSCREEN_CHOOSE_MAP:
            if ( r.code == 1200 )
            {
                AfterMapChoose();
            }
            else if ( r.code == 1250 )
            {
                p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELLVL);
            }
            break;

        case NETSCREEN_INSESSION:
            if ( r.code == 1200 )
            {
                if ( isHost )
                {
                    std::string v425;
                    std::string v425_1;

                    if ( p_YW->_netDriver->GetPlayerCount() <= 1 )
                    {
                        ShowConfirmDialog(2, Locale::Text::Advanced(Locale::ADV_WANTSTART)
                                    , Locale::Text::Advanced(Locale::ADV_NETALONE), 0);
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
                    msgBuffers.clear();
                    lastSender.clear();
                    netName = "";
                    netSelMode = NETSCREEN_CHOOSE_MAP;
                }
            }
            else if ( r.code == 1208 )
            {
                yw_arg181 v353;
                uamessage_ready rdyMsg;

                rdyStart = true;
                
                int myIndex = p_YW->_netDriver->GetMyIndex();
                
                if (myIndex >= 0)
                    lobbyPlayers[myIndex].Ready = true;

                rdyMsg.msgID = UAMSG_READY;
                rdyMsg.owner = 0;
                rdyMsg.rdy = 1;

                v353.dataSize = sizeof(rdyMsg);
                v353.recvFlags = 2;
                v353.data = &rdyMsg;
                v353.recvID = 0;
                v353.garant = 1;

                p_YW->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.receiverFlags = 2;
                v387.receiverID = 0;
                v387.senderFlags = 1;
                v387.senderID = netPlayerName.c_str();
                v387.guarant = 1;

                p_YW->_netDriver->FlushBuffer(v387);
            }
            else if ( r.code == 1209 )
            {
                yw_arg181 v353;
                uamessage_ready rdyMsg;

                rdyStart = false;
                
                int myIndex = p_YW->_netDriver->GetMyIndex();
                
                if (myIndex >= 0)
                    lobbyPlayers[myIndex].Ready = false;

                rdyMsg.msgID = UAMSG_READY;
                rdyMsg.owner = 0;
                rdyMsg.rdy = 0;

                v353.recvFlags = 2;
                v353.recvID = 0;
                v353.data = &rdyMsg;
                v353.dataSize = sizeof(rdyMsg);
                v353.garant = 1;

                p_YW->ypaworld_func181(&v353);

                windp_arg82 v387;
                v387.receiverFlags = 2;
                v387.receiverID = 0;
                v387.senderFlags = 1;
                v387.senderID = netPlayerName.c_str();
                v387.guarant = 1;

                p_YW->_netDriver->FlushBuffer(v387);
            }
            else if ( r.code == 1210 )
            {
//                if ( str17_NOT_FALSE )
//                {
//                    windd_dlgBox v316;
//                    memset(&v316, 0, sizeof(windd_dlgBox));
//
//                    v316.title = get_lang_string(422, "ENTER MESSAGE");
//                    v316.ok = get_lang_string(2, "OK");
//                    v316.cancel = get_lang_string(3, "CANCEL");
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
                    v346.senderID = netPlayerName.c_str();
                    v346.garant = 1;

                    p_YW->ypaworld_func181(&v346);

                    sub_4D0C24(p_YW, netPlayerName, msgMsg.message);

                    netName = "";
                    netNameCurPos = 0;

                    int v223 = strtol(msgMsg.message, NULL, 0);
                    if ( v223 > 0 )
                        sub_4D9550(p_YW, v223);
                }
            }
            else if ( r.code == 1250 )
            {
                p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSTRTSCR);
            }
            break;
        default:
            break;
        }
    }

    if ( EnvMode == ENVMODE_NETPLAY )
    {
        int a4 = network_button->getBTN_y();

        network_listvw.y = netListY + a4;

        network_listvw.InputHandle(p_YW, Input);

        if ( (network_listvw.listFlags & GuiList::GLIST_FLAG_IN_SELECT) || Input->KbdLastHit == Input::KC_UP || Input->KbdLastHit == Input::KC_DOWN )
        {
            netSel = network_listvw.selectedEntry;

            switch ( netSelMode )
            {
            case NETSCREEN_MODE_SELECT:
            case NETSCREEN_SESSION_SELECT:
                nInputMode = 0;
                break;
            case NETSCREEN_CHOOSE_MAP:
            {
                uint32_t playerCount = p_YW->_netDriver->GetPlayerCount();

                int filteredID = 0;

                for (const UserData::TMapDescription &desc : mapDescriptions)
                {
                    if ( playerCount <= 1 || playerCount <= p_YW->_globalMapRegions.MapRegions.at( desc.id ).RoboCount)
                    {
                        if ( filteredID == netSel )
                        {
                            netLevelName = desc.pstring;
                            netLevelID = desc.id;
                            break;
                        }

                        filteredID++;
                    }
                }
            }
            break;

            default:
                break;
            }

            netNameCurPos = netName.size();
        }

        network_listvw.Formate(p_YW);
    }

    if ( EnvMode == ENVMODE_NETPLAY )
    {
        if ( Input->KbdLastHit != Input::KC_NONE || Input->chr || Input->HotKeyID >= 0 )
        {
            if ( nInputMode )
            {
                
                uint32_t v233;

                if ( netSelMode == NETSCREEN_ENTER_NAME )
                    v233 = 32;
                else
                    v233 = 38;

                if ( netName.size() < v233 )
                {
                    if ( Input->chr > ' ' && Input->chr != '*' )
                    {
                        if (netNameCurPos <= (int)netName.size())
                        {
                            netName.insert(netNameCurPos, 1, Input->chr);
                            netNameCurPos++;
                        }                           
                    }
                }

                if ( Input->KbdLastHit == Input::KC_BACKSPACE && netNameCurPos > 0 )
                {
                    if (netNameCurPos > 0 && (int)netName.size() >= netNameCurPos)
                    {
                        netName.erase(netNameCurPos - 1, 1);                        
                        netNameCurPos--;
                    }
                }
                else if ( Input->KbdLastHit == Input::KC_LEFT )
                {
                    if ( netNameCurPos > 0 )
                        netNameCurPos--;
                }
                else if ( Input->KbdLastHit == Input::KC_RIGHT )
                {
                    if ( netNameCurPos < (int)netName.size() )
                        netNameCurPos++;
                }
                else if ( Input->KbdLastHit == Input::KC_DELETE && netNameCurPos < (int)netName.size() )
                {
                    if ( netNameCurPos < (int)netName.size() )
                        netName.erase(netNameCurPos, 1);
                }
            }

            if ( Input->KbdLastHit == Input::KC_RETURN )
            {
                switch ( netSelMode )
                {
                case NETSCREEN_MODE_SELECT:
                    yw_NetOKProvider();
                    break;

                case NETSCREEN_SESSION_SELECT:
                    if ( network_listvw.numEntries )
                    {
                        yw_JoinNetGame();
                    }
                    else
                    {
                        isHost = true;
                        netSel = -1;
                        netSelMode = NETSCREEN_CHOOSE_MAP;
                        network_listvw.firstShownEntries = 0;
                    }
                    break;

                case NETSCREEN_ENTER_NAME:
                    if ( !netName.empty() )
                    {
                        netPlayerName = netName;
                        netName = "";

                        p_YW->_netDriver->SetWantedName(netPlayerName);
                        switch ( p_YW->_netDriver->GetMode() )
                        {
                            case 1:
                                isHost = true;
                                netSel = -1;
                                network_listvw.firstShownEntries = 0;
                                netSelMode = NETSCREEN_CHOOSE_MAP;
                                p_YW->GuiWinOpen( &network_listvw );
                                break;

                            case 2:
                            {
                                netSelMode = NETSCREEN_ENTER_IP;
                                netName = "";
                                netNameCurPos = 0;
                            }
                                break;

                            default:
                                break;
                        }
                    }
                    break;
                
                case NETSCREEN_ENTER_IP:
                    {
                        if ( !netName.empty() )
                        {
                            std::string ip = netName;
                            netName = "";
                            ConnectToServer(ip);
                        }
                        else
                        {
                            netName = "127.0.0.1";
                            netNameCurPos = netName.size();
                        }
                    }
                    break;

                case NETSCREEN_CHOOSE_MAP:
                    AfterMapChoose();
                    break;
                case NETSCREEN_INSESSION:
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

                        p_YW->ypaworld_func181(&v325);

                        sub_4D0C24(p_YW, netPlayerName, msgMsg.message);
                        netName.clear();
                        netNameCurPos = 0;

                        int v271 = strtol(msgMsg.message, NULL, 0);
                        if ( v271 > 0 )
                            sub_4D9550(p_YW, v271);
                    }
                    break;
                default:
                    break;
                }
            }
            else if ( Input->KbdLastHit == Input::KC_ESCAPE )
            {
                ExitFromNetworkToMain();
            }

            if ( Input->HotKeyID == 43 && !nInputMode )
            {
                switch ( netSelMode )
                {
                case NETSCREEN_MODE_SELECT:
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELPROV);
                    break;
                case NETSCREEN_SESSION_SELECT:
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELSESS);
                    break;
                case NETSCREEN_ENTER_NAME:
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETNAME);
                    break;
                case NETSCREEN_CHOOSE_MAP:
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSELLVL);
                    break;
                case NETSCREEN_INSESSION:
                    p_YW->_helpURL = Locale::Text::Help(Locale::HELP_NETSTRTSCR);
                    break;
                default:
                    break;
                }
            }
            
            if ( netSelMode == NETSCREEN_ENTER_IP && Input->KbdLastHit == Input::KC_V && Input::Engine.GetKeyState(Input::KC_CTRL) )
            {
                char * clpbrd = SDL_GetClipboardText();
                if (clpbrd)
                {
                    IPaddress tmp;
                    if (SDLNet_ResolveHost(&tmp, clpbrd, 0) == 0)
                    {
                        netName = clpbrd;
                        netNameCurPos = netName.size();
                    }
                    SDL_free(clpbrd);
                }
            }

            if ( netSel != -1 ){
                network_listvw.PosOnSelected(netSel);
            }

            Input->KbdLastHit = Input::KC_NONE;
        }
    }

    if ( isHost )
    {
        if ( netSelMode == NETSCREEN_INSESSION && envAction.action != EnvAction::ACTION_NETPLAY )
        {
            if ( p_YW->_netDriver->GetPlayerCount()   <   p_YW->_globalMapRegions.MapRegions[ netLevelID ].RoboCount )
            {
                if ( blocked )
                {
                    int v357 = 0;
                    p_YW->_netDriver->LockSession(&v357);

                    blocked = false;
                }
            }
            else if ( !blocked )
            {
                int v357 = 1;
                p_YW->_netDriver->LockSession(&v357);

                blocked = true;
            }
        }
    }

    v410.butID = 1201;
    network_button->Enable(&v410);

    v410.butID = 1205;
    network_button->Disable(&v410);

    v410.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::BTN_CREATE_SESSTION;
    network_button->Disable(&v410);

    v410.butID = 1203;
    network_button->Enable(&v410);

    v410.butID = 1225;
    network_button->Disable(&v410);

    v410.butID = 1226;
    network_button->Disable(&v410);

    v410.butID = 1227;
    network_button->Disable(&v410);

    network_button->SetText(1201, Locale::Text::Common(Locale::CMN_OK));

    v410.butID = 1220;
    network_button->Disable(&v410);

    v410.butID = 1206;
    network_button->Disable(&v410);

    v410.butID = 1207;
    network_button->Disable(&v410);

    v410.butID = 1208;
    network_button->Disable(&v410);

    v410.butID = 1209;
    network_button->Disable(&v410);

    v410.butID = 1219;
    network_button->Disable(&v410);

    v410.butID = 1221;
    network_button->Disable(&v410);

    v410.field_4 = 0;
    v410.butID = 1210;
    network_button->Disable(&v410);

    v410.butID = 1211;
    network_button->Disable(&v410);

    v410.butID = 1212;
    network_button->Disable(&v410);

    v410.butID = 1213;
    network_button->Disable(&v410);

    v410.butID = 1214;
    network_button->Disable(&v410);

    v410.butID = 1215;
    network_button->Disable(&v410);

    v410.butID = 1216;
    network_button->Disable(&v410);

    v410.butID = 1217;
    network_button->Disable(&v410);

    if ( (netSelMode != NETSCREEN_SESSION_SELECT || p_YW->_netDriver->GetProvType() != 3)
            && (netSelMode != NETSCREEN_SESSION_SELECT || modemAskSession != 1 || p_YW->_netDriver->GetProvType() != 4)
            && netSelMode != NETSCREEN_MODE_SELECT )
    {
        v410.butID = 1228;
        network_button->Disable(&v410);
    }
    else
    {
        std::string v280;

        if ( netSelMode != NETSCREEN_MODE_SELECT )
        {
            v280 = Locale::Text::Advanced(Locale::ADV_REFRESHSESS);
        }
        else
        {
            if ( !p_YW->_netTcpAddress.empty() )
            {
                v280 = fmt::sprintf("%s  %s",  Locale::Text::Advanced(Locale::ADV_YOURIP) , p_YW->_netTcpAddress);
            }
            else
                v280 = " ";
        }

        network_button->SetText(1228, v280);

        v410.butID = 1228;
        network_button->Enable(&v410);
    }

    if ( !nInputMode && (netSelMode != NETSCREEN_ENTER_NAME || netSelMode != NETSCREEN_ENTER_IP) )
    {
        v410.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX;
        network_button->Disable(&v410);
    }
    else
    {
        v410.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX;
        network_button->Enable(&v410);

        v393.xpos = -1;
        v393.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX;

        if ( netSelMode == NETSCREEN_ENTER_NAME || netSelMode == NETSCREEN_ENTER_IP)
        {
            v393.width = dword_5A50B6;
            v393.ypos = 3 * (vertMenuSpace + p_YW->_fontH);
        }
        else
        {
            v393.width = dword_5A50B6 * 0.8;
            v393.ypos = 14 * (vertMenuSpace + p_YW->_fontH);
        }

        network_button->setXYWidth(&v393);

        std::string tmp = netName;
        if (tmp.size() >= (size_t)netNameCurPos)
            tmp.insert(netNameCurPos, 1, '_');
        
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXTBOX, tmp);
    }

    v393.xpos = -1;
    v393.width = -1;
    v393.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::BTN_CREATE_SESSTION;

    if ( netSelMode == 2 )
    {
        v393.ypos = 4 * (vertMenuSpace + p_YW->_fontH);
    }
    else
    {
        v393.ypos = (vertMenuSpace + p_YW->_fontH) * 15.2;
    }

    network_button->setXYWidth(&v393);

    switch ( netSelMode )
    {
    case NETSCREEN_MODE_SELECT:
        v410.field_4 = 0;
        v410.butID = 1205;

        network_button->Disable(&v410);

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_SELPROV));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT2));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT3));
        break;

    case NETSCREEN_SESSION_SELECT:
    {
        if ( p_YW->_netDriver->GetProvType() != 4 || !modemAskSession )
        {
            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::BTN_CREATE_SESSTION, Locale::Text::Netdlg(Locale::NETDLG_NEW));

            v410.butID = UIWidgets::NETWORK_MENU_WIDGET_IDS::BTN_CREATE_SESSTION;
            network_button->Enable(&v410);
        }

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_SELSESS));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT5));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT6));

        windp_getNameMsg msg;
        msg.id = 0;

        if ( p_YW->_netDriver->GetSessionName(&msg) )
        {
            network_button->SetText(1201, Locale::Text::Netdlg(Locale::NETDLG_JOIN));
        }
        else if ( p_YW->_netDriver->GetProvType() != 4 || modemAskSession )
        {
            v410.butID = 1201;
            network_button->Disable(&v410);
        }
        else
        {
            network_button->SetText(1201, Locale::Text::Netdlg(Locale::NETDLG_CONNECT));
        }
    }
    break;

    case NETSCREEN_ENTER_NAME:
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_ENTERPL));
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT11));
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT12));
        break;

    case NETSCREEN_ENTER_IP:
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, "Server address");
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, "Please enter server IP");
        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, " ");
        break;

    case NETSCREEN_CHOOSE_MAP:
        if ( remoteMode )
        {
            v410.butID = 1205;
            network_button->Disable(&v410);
        }

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_SELLVL));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT8));

        network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT9));
        break;

    case NETSCREEN_INSESSION:
        v410.butID = 1225;
        network_button->Enable(&v410);

        network_button->SetText(1225, Locale::Text::Netdlg(Locale::NETDLG_SEND));

        v410.butID = 1226;
        network_button->Enable(&v410);

        v410.butID = 1227;
        network_button->Enable(&v410);

        if ( netLevelID )
            network_button->SetText(1226, netLevelName);
        else
            network_button->SetText(1226, " ");

        if ( isHost )
        {  // Change map 
            v410.butID = 1205;
            //network_button->show(&v410);
        }

        if ( rdyStart )
        {
            if ( !isHost )
            {
                v410.butID = 1205;
                network_button->Disable(&v410);
            }
        }

        v410.butID = 1220;
        network_button->Enable(&v410);

        if ( netLevelID > 0 && netLevelID < 256 )
        {

            if ( !isHost && rdyStart )
            {
                v410.butID = 1220;
                network_button->Disable(&v410);
            }
            else
            {
                v410.butID = 1206;
                if ( p_YW->_globalMapRegions.MapRegions[ netLevelID ].FractionsBits & 2 )
                    network_button->Enable(&v410);
                else
                    network_button->Disable(&v410);

                v410.butID = 1207;
                if ( p_YW->_globalMapRegions.MapRegions[ netLevelID ].FractionsBits & 0x40 )
                    network_button->Enable(&v410);
                else
                    network_button->Disable(&v410);

                v410.butID = 1208;
                if ( p_YW->_globalMapRegions.MapRegions[ netLevelID ].FractionsBits & 8 )
                    network_button->Enable(&v410);
                else
                    network_button->Disable(&v410);

                v410.butID = 1209;
                if ( p_YW->_globalMapRegions.MapRegions[ netLevelID ].FractionsBits & 0x10 )
                    network_button->Enable(&v410);
                else
                    network_button->Disable(&v410);
            }

            v408.butID = 0;

            switch ( SelectedFraction )
            {
            case NET_FRACTION_RESISTANCE:
                v408.butID = 1206;
                break;
            case NET_FRACTION_GHORKOV:
                v408.butID = 1207;
                break;
                case NET_FRACTION_MIKO:
                v408.butID = 1208;
                break;
            case NET_FRACTION_TAER:
                v408.butID = 1209;
                break;
                
            default:
                break;
            }

            if ( v408.butID )
            {
                v408.field_4 = 1;
                network_button->SetState(&v408);
            }

            int v298 = 0;

            netGameCanStart = true;
            isWelcmd = true;

            if ( !(p_YW->_globalMapRegions.MapRegions[netLevelID].FractionsBits & 2) )
                v298 = NET_FRACTION_RESISTANCE;

            if ( !(p_YW->_globalMapRegions.MapRegions[netLevelID].FractionsBits & 0x40) )
                v298 |= NET_FRACTION_GHORKOV;

            if ( !(p_YW->_globalMapRegions.MapRegions[netLevelID].FractionsBits & 8) )
                v298 |= NET_FRACTION_MIKO;

            if ( !(p_YW->_globalMapRegions.MapRegions[netLevelID].FractionsBits & 0x10) )
                v298 |= NET_FRACTION_TAER;

            int v373, v374, v375, v376;

            for ( TDPPlayerData &p : p_YW->_netDriver->GetPlayersData() )
            {
                int v299;

                if ( p.IsItMe() )
                    v299 = SelectedFraction;
                else
                    v299 = lobbyPlayers[p.Index].NetFraction;

                if ( v299 & v298 )
                {
                    lobbyPlayers[p.Index].IsTrouble = true;
                    netGameCanStart = false;

                    switch ( v299 )
                    {
                    case NET_FRACTION_RESISTANCE:
                        lobbyPlayers[v375].IsTrouble = true;
                        break;
                    case NET_FRACTION_GHORKOV:
                        lobbyPlayers[v373].IsTrouble = true;
                        break;
                    case NET_FRACTION_MIKO:
                        lobbyPlayers[v376].IsTrouble = true;
                        break;
                    case NET_FRACTION_TAER:
                        lobbyPlayers[v374].IsTrouble = true;
                        break;
                    default:
                        break;
                    }
                }
                else
                {
                    lobbyPlayers[p.Index].IsTrouble = false;

                    switch ( v299 )
                    {
                    case NET_FRACTION_RESISTANCE:
                        v375 = p.Index;
                        break;
                    case NET_FRACTION_GHORKOV:
                        v373 = p.Index;
                        break;
                    case NET_FRACTION_MIKO:
                        v376 = p.Index;
                        break;
                    case NET_FRACTION_TAER:
                        v374 = p.Index;
                        break;
                    default:
                        break;
                    }

                }
            }
        }

        for ( TDPPlayerData &p : p_YW->_netDriver->GetPlayersData() )
        {
            if ( !p.IsItMe() )
            {
                if ( !lobbyPlayers[p.Index].Ready )
                    netGameCanStart = false;

                if ( !lobbyPlayers[p.Index].Welcomed )
                    isWelcmd = false;
            }
        }

        if ( isHost )
        {
            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_STARTOR));

            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT14));

            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT15));

            network_button->SetText(1201, Locale::Text::Netdlg(Locale::NETDLG_START));

            if ( !netGameCanStart )
            {
                v410.field_4 = 0;
                v410.butID = 1201;
                network_button->Disable(&v410);
            }
        }
        else
        {
            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_TITLE, Locale::Text::Netdlg(Locale::NETDLG_WAITOR));

            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE1, Locale::Text::Netdlg(Locale::NETDLG_TXT17));

            network_button->SetText(UIWidgets::NETWORK_MENU_WIDGET_IDS::TXT_MENU_DESCR_LINE2, Locale::Text::Netdlg(Locale::NETDLG_TXT18));

            if ( isWelcmd )
            {
                v410.butID = 1219;
                network_button->Enable(&v410);

                v410.butID = 1221;
                network_button->Enable(&v410);
            }

            v410.butID = 1201;
            network_button->Disable(&v410);
        }
        v410.butID = 1210;
        network_button->Enable(&v410);

        v410.butID = 1211;
        network_button->Enable(&v410);

        v410.butID = 1212;
        network_button->Enable(&v410);

        v410.butID = 1213;
        network_button->Enable(&v410);

        v410.butID = 1214;
        network_button->Enable(&v410);

        v410.butID = 1215;
        network_button->Enable(&v410);

        v410.butID = 1216;
        network_button->Enable(&v410);

        v410.butID = 1217;
        network_button->Enable(&v410);

        for (size_t i = 0; i < World::CVMaxNetPlayers; i++)
        {
            int v370;
            TDPPlayerData pData;
            bool v304 = p_YW->_netDriver->GetPlayerData(i, &pData);

            std::string name = " ";
            int btID;

            switch ( i )
            {
            case 0:
                v370 = 1214;
                btID = 1210;
                if ( v304 )
                    name = pData.name;
                break;

            case 1:
                v370 = 1215;
                btID = 1211;
                if ( v304 )
                    name = pData.name;
                break;

            case 2:
                v370 = 1216;
                btID = 1212;
                if ( v304 )
                    name = pData.name;
                break;

            case 3:
                v370 = 1217;
                btID = 1213;
                if ( v304 )
                    name = pData.name;
                break;

            default:
                break;
            }

            network_button->SetText(btID, name);

            std::string v339("     "); // 5 spaces
            
            if ( v304 )
            {
                int v305;

                if ( pData.IsItMe() )
                {
                    v305 = SelectedFraction;
                }
                else
                {
                    v305 = lobbyPlayers[pData.Index].NetFraction;
                }

                switch ( v305 )
                {
                case NET_FRACTION_RESISTANCE:
                    v339[0] = 'P';
                    break;
                case NET_FRACTION_GHORKOV:
                    v339[0] = 'R';
                    break;
                case NET_FRACTION_MIKO:
                    v339[0] = 'T';
                    break;
                case NET_FRACTION_TAER:
                    v339[0] = 'V';
                    break;
                default:
                    break;
                }
                if ( lobbyPlayers[pData.Index].IsTrouble && ((GlobalTime / 300) & 1) )
                    v339[1] = 'f';

                if ( lobbyPlayers[pData.Index].Ready )
                    v339[2] = 'h';
            }

            network_button->SetText(v370, v339);
        }
        break;

    default:
        break;
    }

}

bool UserData::LoadSample(int sampleID, const std::string &file)
{
    std::string rsrc = Common::Env.SetPrefix("rsrc", "data:");

    NC_STACK_wav *wav = Nucleus::CInit<NC_STACK_wav>({{NC_STACK_rsrc::RSRC_ATT_NAME, std::string(file)}});
    if ( !wav )
        return false;

    samples1[sampleID] = wav;
    samples1_info.Sounds[sampleID].PSample = wav->GetSampleData();

    if (sampleID == World::SOUND_ID_VOLUME ||
        sampleID == World::SOUND_ID_SLIDER ||
        sampleID == World::SOUND_ID_TEXTAPPEAR ||
        sampleID == World::SOUND_ID_TIMERCOUNT)
        samples1_info.Sounds[sampleID].SetLoop(true);

    Common::Env.SetPrefix("rsrc", rsrc);
    return true;
}

bool UserData::SaveBuildProtoState()
{
    return p_YW->SaveSettings(this, "settings.tmp", (World::SDF_BUDDY | World::SDF_PROTO | World::SDF_USER));
}

void UserData::SaveSettings()
{
    p_YW->SaveSettings(this, fmt::sprintf("%s/user.txt", UserName), World::SDF_ALL);

    FSMgr::FileHandle fil = uaOpenFile("env:user.def", "w");
    if ( fil.OK() )
        fil.write(UserName.c_str(), UserName.size());
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

bool UserData::IsHasSGM(const std::string &username, int id)
{
    return uaFileExist( fmt::sprintf("save:%s/%d.sgm", username, id) );
}

bool UserData::IsHasRestartForLevel(const std::string &username, int id)
{
    return uaFileExist( fmt::sprintf("save:%s/%d.rst", username, id) );
}

