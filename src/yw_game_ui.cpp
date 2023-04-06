#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <iterator>
#include "env.h"
#include "includes.h"
#include "yw_internal.h"
#include "yw.h"
#include "yw_net.h"

#include "font.h"

#include "yparobo.h"
#include "ypagun.h"
#include "windp.h"
#include "world/analyzer.h"


extern uint32_t bact_id;


////////////// Draw wireframe
char byte_5C8DB0[1024];

////////////// Draw
char byte_5FFF80[8192];


////////////////////////////////////////
GuiBase *dword_5BAFAC = NULL;
std::string dword_5BAF98;

////////////////////////////////////////

int dword_5BAF9C;




/////////////Asking//////////////////
GuiList lstvw2;

int dword_5BAFA0;
int dword_5BAFA4;
int dword_5BAFA8;



////////// exit menu/////////
GuiList exit_menu;

int dword_5C8B78;
int dword_5C8B7C;
int dword_5C8B80;
int dword_5C8B84;
int dword_5C8B88;



////////////////////////////////////////


squadMan squadron_manager;


/////////// INFO LOG //////////////

yw_infolog info_log;

char byte_5A7650[1024];




//////////// Map //////////////

int dword_516510;
int dword_516514;
int dword_516518;
int dword_51651C;

float flt_5164F4 = 0.0;
float flt_516504 = 0.0;
float flt_51650C = 0.0;

float flt_516524;
float flt_51652C;
float flt_516528;
float flt_516530;

tehMap robo_map;

char byte_5BA6E8[512];
char t1_cmdbuf_1[256];
char t1_cmdbuf_2[256];
char t1_cmdbuf_3[32768];
char *t1_cmdbufs[3] = {t1_cmdbuf_1, t1_cmdbuf_2, t1_cmdbuf_3};

typedef int (*mapFunc)(NC_STACK_ypaworld *yw, int x, int y);

NC_STACK_ypaworld *dword_5BAA60; // For sort func


///////// up panel ///////////
energPanel up_panel;

char byte_51805C[512];




///////// down panel /////////
GuiList gui_lstvw;

char byte_516534[1088];

bzd bzda;

////////////////////////////////////////
////////////////////////////////////////





void NC_STACK_ypaworld::sb_0x4c87fc(const std::string &a2, GuiBase *lstvw)
{
    GuiWinOpen(&lstvw2);
    GuiWinToFront(&lstvw2);

    dword_5BAF98 = a2;
    dword_5BAFAC = lstvw;
    dword_5BAF9C = 3;
}

void NC_STACK_ypaworld::sub_449DE8(const std::string &a2)
{
    if ( !_isNetGame )
    {
        sb_0x4c87fc( Locale::Text::Advanced(Locale::ADV_RLYHELP) , &exit_menu);

        dword_5C8B78 = 13;

        _helpURL.clear();
        _helpSavedURL = a2;
    }
}


void create_squad_man(NC_STACK_ypaworld *yw)
{
    auto &fnt0 = yw->_guiTiles[0]->map;
    auto &fnt24 = yw->_guiTiles[24]->map;
    auto &fnt28 = yw->_guiTiles[28]->map;

    int f0c32_w = fnt0[32].w;
    int v6 = f0c32_w + fnt0[123].w;

    int v9 = yw->_fontBorderH + yw->_fontH;
    int v10 = yw->_fontBorderH;

    int v8 = f0c32_w + v6 + 5 * fnt24[49].w;
    int v11 = 4 * fnt0[65].w + v8 + fnt28[97].w + f0c32_w + 8;

    GuiList::tInit args;
    args.title = Locale::Text::WinName(Locale::WINNAME_FINDER);
    args.resizeable = true;
    args.numEntries = 0;
    args.shownEntries = 12;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 24;
    args.minShownEntries = 3;
    args.withIcon = false;
    args.entryHeight = yw->_fontH;
    args.entryWidth = v11;
    args.minEntryWidth = v11;
    args.maxEntryWidth = 32000;
    args.enabled = true;
    args.upperVborder = v10;
    args.lowerVborder = v9;
    args.closeChar = 73;

    if ( squadron_manager.Init(yw, args) )
    {
        squadron_manager.field_2CC = fnt28[65].w;
        squadron_manager.field_2D0 = fnt28[64].w + fnt28[97].w + yw->_fontBorderW;
        squadron_manager.field_2D8 = v8;
        squadron_manager.field_2DC = v6;
        squadron_manager.field_2D4 = fnt28[64].w + squadron_manager.field_2CC + squadron_manager.field_2D0;
    }
}



void sub_4C3A54(NC_STACK_ypaworld *yw)
{
    if ( yw->_userUnit )
    {
        if ( yw->_userUnit == yw->_userRobo || yw->_playerInHSGun )
        {
            bzda.field_1CC = 0x29;

            if ( bzda.field_8E0 )
            {
                bzda.field_1CC = 0x2B;

                if ( yw->_activeCmdrRemapIndex != -1 )
                    bzda.field_1CC = 0x2F;
            }

            if ( bzda.field_8E4 )
                bzda.field_1CC |= 0x10;

            bzda.field_1D4 = 1;
        }
        else
        {
            bzda.field_1D4 = 1;
            bzda.field_1CC = 41;
        }

        if ( yw->_userRobo->_status == BACT_STATUS_DEAD )
            bzda.field_1CC = 0;
    }
    else
    {
        bzda.field_1D4 = 0;
        bzda.field_1CC = 0;
    }

    bzda.field_1D0 &= bzda.field_1CC;
    bzda.field_1D8 &= bzda.field_1D4;

    if ( !bzda.field_1D0 )
        bzda.field_1D0 = bzda.field_1CC & 1;
}

void sb_0x451034__sub8(NC_STACK_ypaworld *yw)
{
    up_panel = energPanel();

    TileMap *f30 = yw->_guiTiles[30];

    up_panel.field_1CC = f30->h;
    up_panel.field_1DC = f30->map[65].w;
    up_panel.flags = 0;
    up_panel.field_1D0 = up_panel.field_1DC + f30->map[84].w;
    up_panel.buttons.resize(4);


    int v8 = yw->_screenSize.x - 4 * (up_panel.field_1DC + f30->map[84].w);

    up_panel.field_1D8 = v8 / 4;
    up_panel.field_1D4 = v8 / 8;
    up_panel.field_1E0 = 2;
    up_panel.field_1E4 = up_panel.field_1D0 - up_panel.field_1DC - 4;

    up_panel.x = 0;
    up_panel.y = 0;
    up_panel.w = yw->_screenSize.x;
    up_panel.h = up_panel.field_1CC;

    for (int i = 0; i < 4; i++)
        up_panel.buttons[i] = ButtonBox(up_panel.field_1D4 + (up_panel.field_1D0 + up_panel.field_1D8) * i,    0,
                                        up_panel.field_1D0,   up_panel.field_1CC);

    up_panel.cmdstrm.cmdbuf = byte_51805C;

    Input::Engine.AddClickBoxFront(&up_panel);
}


Common::Point sub_4F681C(vec2d in)
{
    if ( robo_map.field_1E8 & 0x100 )
    {
        float a1a = in.x - robo_map.field_1D8;
        float a2a = in.y - robo_map.field_1DC;

        in.x = robo_map.field_1D8 + robo_map.field_264 * a1a + flt_5164F4 * a2a;
        in.y = robo_map.field_1DC + flt_516504 * a1a + flt_51650C * a2a;
    }

    return Common::Point( robo_map.field_200 + dround(in.x / robo_map.field_1E0) - robo_map.field_1F0
                        , robo_map.field_204 + dround(-in.y / robo_map.field_1E4) - robo_map.field_1F4 );
}

void sub_4F68FC(float a3, float a4, float a5, float a6, SDL_Color a7)
{
    GFX::Engine.raster_func217(a7);
    GFX::Engine.raster_func201( Common::Line( sub_4F681C({a3, a4}), sub_4F681C({a5, a6}) )  );
}


char * sub_4F6980(char *cur, float a1, float a2, char a3, int a4, int a5)
{
    char *pcur = cur;

    Common::Point tmp = sub_4F681C( {a1, a2} );

    int v7 = tmp.x - a4 / 2 - robo_map.field_200;
    int v8 = tmp.y - a5 / 2 - robo_map.field_204;
    int v9 = v7 + a4;
    int v10 = v8 + a5;

    if ( v7 < robo_map.field_1F8 && v8 < robo_map.field_1FC && v9 > 0 && v10 > 0 )
    {
        int v11;

        if ( v7 >= 0 )
        {
            v11 = 0;
        }
        else
        {
            v11 = -v7;
            v7 = 0;
        }

        int v33;

        if ( v8 >= 0 )
        {
            v33 = 0;
        }
        else
        {
            v33 = -v8;
            v8 = 0;
        }

        int v34;

        if ( v9 <= robo_map.field_1F8 )
            v34 = 0;
        else
            v34 = robo_map.field_1F8 - v7;

        int v35;

        if ( v10 <= robo_map.field_1FC )
            v35 = 0;
        else
            v35 = robo_map.field_1FC - v8;

        FontUA::set_center_xpos(&pcur, robo_map.field_200 + v7);
        FontUA::set_center_ypos(&pcur, robo_map.field_204 + v8);

        if ( v33 )
        {
            FontUA::set_yoff(&pcur, v33);
        }
        else if ( v35 )
        {
            FontUA::set_yheight(&pcur, v35);
        }

        if ( v11 )
        {
            FontUA::set_xoff(&pcur, v11);
        }
        else if ( v34 )
        {
            FontUA::set_xwidth(&pcur, v34);
        }

        FontUA::store_u8(&pcur, a3);
    }

    return pcur;
}

void sub_4F72E8(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact)
{
    if ( bact != yw->_userRobo && bact->_host_station != bact->_parent && bact->_parent )
    {
        SDL_Color clr;
        NC_STACK_ypabact *bct;

        if ( bact->_secndTtype == BACT_TGT_TYPE_UNIT )
        {
            clr = yw->GetColor(10);
            bct = bact->_secndT.pbact;
        }
        else
        {
            if ( !bact->_parent )
                return;

            clr = yw->GetColor(11);
            bct = bact->_parent;
        }

        sub_4F68FC(bact->_position.x, bact->_position.z, bct->_position.x, bct->_position.z, clr);
        return;
    }

    if ( bact->_secndTtype == BACT_TGT_TYPE_UNIT )
    {
        NC_STACK_ypabact *bct = bact->_secndT.pbact;

        sub_4F68FC(bact->_position.x, bact->_position.z, bct->_position.x, bct->_position.z,  yw->GetColor(10));
        return;
    }

    if ( bact->_secndTtype == BACT_TGT_TYPE_CELL )
    {
        sub_4F68FC(bact->_position.x, bact->_position.z, bact->_sencdTpos.x, bact->_sencdTpos.z, yw->GetColor(10));
    }
    else
    {
        if ( bact->_primTtype == BACT_TGT_TYPE_CELL )
        {
            sub_4F68FC(bact->_position.x, bact->_position.z, bact->_primTpos.x, bact->_primTpos.z, yw->GetColor(9));
        }
        else if ( bact->_primTtype == BACT_TGT_TYPE_UNIT )
        {
            sub_4F68FC(bact->_position.x, bact->_position.z, bact->_primT.pbact->_position.x, bact->_primT.pbact->_position.z, yw->GetColor(9));
        }

        if ( bact->_status_flg & BACT_STFLAG_WAYPOINT )
        {
            if ( bact->_status_flg & BACT_STFLAG_WAYPOINTCCL )
            {
                SDL_Color clr = yw->GetColor(9);

                for (int i = 1; i < bact->_waypoints_count; i++)
                    sub_4F68FC(bact->_waypoints[i - 1].x, bact->_waypoints[i - 1].z, bact->_waypoints[i].x, bact->_waypoints[i].z, clr);

                sub_4F68FC(bact->_waypoints[bact->_waypoints_count - 1].x, bact->_waypoints[bact->_waypoints_count - 1].z, bact->_waypoints[0].x, bact->_waypoints[0].z, clr);
            }
            else if ( bact->_waypoints_count > 0 )
            {
                SDL_Color clr = yw->GetColor(9);

                for (int i = bact->_current_waypoint; i < bact->_waypoints_count - 1; i++)
                    sub_4F68FC(bact->_waypoints[i].x, bact->_waypoints[i].z, bact->_waypoints[i + 1].x, bact->_waypoints[i + 1].z, clr);
            }
        }
    }
}

void  sb_0x4f8f64__sub1__sub0(NC_STACK_ypaworld *yw)
{
    for( NC_STACK_ypabact* &bact : yw->_unitsList )
    {
        if ( bact->_bact_type == BACT_TYPES_ROBO && bact != yw->_userRobo )
        {
            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(bact);

            for (int i = 0; i < 8; i++)
            {
                int v10 = 0;

                switch ( i )
                {
                case 0:
                    if ( robo->_roboRadarValue > 0 )
                        v10 = robo->_roboRadarCellID;
                    break;

                case 1:
                    if ( robo->_roboSafetyValue > 0 )
                        v10 = robo->_roboSafetyCellID;
                    break;

                case 2:
                    if ( robo->_roboPowerValue > 0 )
                        v10 = robo->_roboPowerCellID;
                    break;

                case 3:
                    if ( robo->_roboEnemyValue > 0 )
                        v10 = robo->_roboEnemyCellID;
                    break;

                case 4:
                    if ( robo->_roboConqValue > 0 )
                        v10 = robo->_roboConqCellID;
                    break;

                case 5:
                    if ( robo->_roboPositionValue > 0 )
                        v10 = robo->_roboPositionCellID;
                    break;

                case 6:
                    if ( robo->_roboExploreValue > 0 )
                        v10 = robo->_roboExploreCellID;
                    break;

                case 7:
                    if ( robo->_roboDangerValue > 0 )
                        v10 = robo->_roboDangerCellID;
                    break;

                default:
                    break;
                }

                if ( v10 )
                {
                    vec2d secC = World::SectorIDToCenterPos2( {v10 % yw->_mapSize.x, v10 / yw->_mapSize.x} );

                    sub_4F68FC(bact->_position.x, bact->_position.z, secC.x, secC.y, yw->GetColor(i));
                }
            }


            if ( robo->_roboBuildingDuty )
            {
                vec2d secC = World::SectorIDToCenterPos2( {robo->_roboBuildingCellID % yw->_mapSize.x, robo->_roboBuildingCellID / yw->_mapSize.x} );

                if ( (yw->_timeStamp / 300) & 1 )
                    sub_4F68FC(bact->_position.x, bact->_position.z, secC.x, secC.y, yw->GetColor(0));
            }

            if ( robo->_roboVehicleDuty )
            {
                vec2d secC = World::SectorIDToCenterPos2( {robo->_roboVehicleCellID % yw->_mapSize.x, robo->_roboVehicleCellID / yw->_mapSize.x} );

                if ( (yw->_timeStamp / 300) & 1 )
                {
                    sub_4F68FC(bact->_position.x, bact->_position.z, secC.x, secC.y, yw->GetColor(7));
                }
            }
        }
    }
}

void sb_0x4f8f64__sub1(NC_STACK_ypaworld *yw)
{
    if ( robo_map.field_1E8 & 0x100 )
    {
        if ( yw->_userRobo != yw->_userUnit && yw->_userUnit->_parent )
        {
            sub_4F68FC(yw->_userUnit->_position.x, yw->_userUnit->_position.z, yw->_userUnit->_parent->_position.x, yw->_userUnit->_parent->_position.z, yw->GetColor(11));

            if ( yw->_userUnit->IsParentMyRobo() )
            {
                int v7 = 0;
                float a5, a6;

                if ( yw->_userUnit->_primTtype == BACT_TGT_TYPE_CELL )
                {
                    a5 = yw->_userUnit->_primTpos.x;
                    a6 = yw->_userUnit->_primTpos.z;
                    v7 = 1;
                }
                else if ( yw->_userUnit->_primTtype == BACT_TGT_TYPE_UNIT )
                {
                    a5 = yw->_userUnit->_primT.pbact->_position.x;
                    a6 = yw->_userUnit->_primT.pbact->_position.z;
                    v7 = 1;
                }

                if ( v7 )
                    sub_4F68FC(yw->_userUnit->_position.x, yw->_userUnit->_position.z, a5, a6, yw->GetColor(9));
            }
        }
    }
    else
    {
        if ( robo_map.field_1EC & 4 )
        {
            for (NC_STACK_ypabact* const comm: yw->_cmdrsRemap)
            {
                if (comm->_status != BACT_STATUS_DEAD)
                    sub_4F72E8(yw, comm);

                for( NC_STACK_ypabact* &kid : comm->_kidList )
                {
                    if ( kid->_status != BACT_STATUS_DEAD )
                        sub_4F72E8(yw, kid);
                }
            }
        }
        else
        {
            if ( yw->_activeCmdrRemapIndex != -1 )
            {
                NC_STACK_ypabact *tmp = yw->_cmdrsRemap[yw->_activeCmdrRemapIndex];

                if ( tmp->_status != BACT_STATUS_DEAD )
                    sub_4F72E8(yw, tmp);
            }
        }

        if ( yw->_userUnit )
        {
            sub_4F68FC(yw->_userUnit->_position.x, 0.0, yw->_userUnit->_position.x, -yw->_mapLength.y, yw->GetColor(13));
            sub_4F68FC(0.0, yw->_userUnit->_position.z, yw->_mapLength.x, yw->_userUnit->_position.z, yw->GetColor(13));
        }

        if ( robo_map.field_1E8 & 0x200 )
        {
            int wh = yw->_screenSize.x / 2;
            int hh = yw->_screenSize.y / 2;

            GFX::Engine.raster_func217( yw->GetColor(12) );
            
            GFX::Engine.raster_func201( {dword_516510 - wh,
                                         dword_516514 - hh,
                                         dword_516518 - wh,
                                         dword_516514 - hh} );
            GFX::Engine.raster_func201( {dword_516518 - wh,
                                         dword_516514 - hh,
                                         dword_516518 - wh,
                                         dword_51651C - hh} );
            GFX::Engine.raster_func201( {dword_516518 - wh,
                                         dword_51651C - hh,
                                         dword_516510 - wh,
                                         dword_51651C - hh} );
            GFX::Engine.raster_func201( {dword_516510 - wh,
                                         dword_51651C - hh,
                                         dword_516510 - wh,
                                         dword_516514 - hh} );
        }
    }

    if ( yw->_showDebugMode )
        sb_0x4f8f64__sub1__sub0(yw);
}


char * sb_0x4f8f64__sub2__sub0(char *cur, float a1, float a2, char a3, int a4, int a5)
{
    char *pcur = cur;

    Common::Point tmp = sub_4F681C( {a1, a2} );

    int v8 = tmp.x - a4 / 2 - robo_map.field_200;
    int v9 = tmp.y - a4 / 2 - robo_map.field_204;
    int v10 = v8 + a4;
    int v11 = v9 + a5;

    if ( v8 < robo_map.field_1F8 && v9 < robo_map.field_1FC && v10 > 0 && v11 > 0 )
    {
        int v31;
        if ( v8 >= 0 )
        {
            v31 = 0;
        }
        else
        {
            v31 = -v8;
            v8 = 0;
        }

        int v12;
        if ( v9 >= 0 )
        {
            v12 = 0;
        }
        else
        {
            v12 = -v9;
            v9 = 0;
        }

        int v30;
        if ( v10 <= robo_map.field_1F8 )
            v30 = 0;
        else
            v30 = robo_map.field_1F8 - v8;

        int v32;
        if ( v11 <= robo_map.field_1FC )
            v32 = a5;
        else
            v32 = robo_map.field_1FC - v9;

        FontUA::set_center_xpos(&pcur, robo_map.field_200 + v8);
        FontUA::set_center_ypos(&pcur, robo_map.field_204 + v9);


        if ( v12 )
            FontUA::set_yoff(&pcur, v12);

        if ( v32 )
            FontUA::set_yheight(&pcur, v32);

        if ( v31 )
        {
            FontUA::set_xoff(&pcur, v31);
        }
        else if ( v30 )
        {
            FontUA::set_xwidth(&pcur, v30);
        }

        FontUA::store_u8(&pcur, a3);
    }
    return pcur;
}


char * sb_0x4f8f64__sub2(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int a4, v4;

    switch ( robo_map.field_1EE )
    {
    case 0:
        v4 = 16;
        a4 = 4;
        break;

    case 1:
        v4 = 17;
        a4 = 8;
        break;

    case 2:
        v4 = 18;
        a4 = 16;
        break;

    case 3:
        v4 = 19;
        a4 = 32;
        break;

    case 4:
        v4 = 20;
        a4 = 64;
        break;

    default:
        break;
    }

    if ( a4 >= 8 )
    {
        if ( robo_map.field_1EC & 1 )
        {
            FontUA::select_tileset(&pcur, v4);

            for ( const auto &ps : yw->_powerStations )
            {
                if ( ps.second.pCell )
                {
                    if ( ps.second.pCell->IsCanSee(yw->_userRobo->_owner) )
                    {
                        if ( ps.second.EffectivePower > 0 )
                        {
                            int v9 = 0;
                            
                            vec2d tmp = World::SectorIDToCenterPos2(ps.second.CellId);

                            if ( ps.second.EffectivePower <= 32)
                                v9 = 0x80;
                            else if ( ps.second.EffectivePower <= 64)
                                v9 = 0x81;
                            else if ( ps.second.EffectivePower <= 96)
                                v9 = 0x82;
                            else if ( ps.second.EffectivePower <= 128)
                                v9 = 0x83;
                            else if ( ps.second.EffectivePower <= 160)
                                v9 = 0x84;
                            else if ( ps.second.EffectivePower <= 192)
                                v9 = 0x85;
                            else if ( ps.second.EffectivePower <= 224)
                                v9 = 0x86;
                            else if ( ps.second.EffectivePower <= 256)
                                v9 = 0x87;

                            pcur = sub_4F6980(pcur, tmp.x, tmp.y, 0x89, a4, a4);
                            pcur = sb_0x4f8f64__sub2__sub0(pcur, tmp.x, tmp.y, v9, a4, a4 / 8);
                        }
                    }
                }
            }

            for ( const TMapGem &gem : yw->_techUpgrades )
            {
                cellArea &cell = yw->SectorAt(gem.CellId);
                if ( cell.IsCanSee(yw->_userRobo->_owner) )
                {
                    int v13 = 1;

                    if ( cell.PurposeType == cellArea::PT_TECHUPGRADE )
                    {
                        if ( yw->_timeStamp / 300 & 1 )
                            v13 = 0;
                    }

                    if ( v13 )
                    {
                        vec2d tmp = World::SectorIDToCenterPos2( gem.CellId );
                        pcur = sub_4F6980(pcur, tmp.x, tmp.y, 0x88, a4, a4);
                    }
                }
            }

            for ( const TMapGate &gate : yw->_levelInfo.Gates )
            {
                if ( gate.PCell->PurposeType == cellArea::PT_GATECLOSED )
                {
                    if ( gate.PCell->IsCanSee(yw->_userRobo->_owner) )
                    {
                        vec2d tmp = World::SectorIDToCenterPos2( gate.CellId );
                        pcur = sub_4F6980(pcur, tmp.x, tmp.y, 0x93, a4, a4);
                    }

                    for ( const TMapKeySector &ks : gate.KeySectors )
                    {
                        if ( ks.PCell )
                        {
                            if ( ks.PCell->IsCanSee(yw->_userRobo->_owner) )
                            {
                                if ( ks.PCell->owner == yw->_userRobo->_owner || yw->_timeStamp / 300 & 1 )
                                {
                                    vec2d tmp = World::SectorIDToCenterPos2( ks.CellId );
                                    pcur = sub_4F6980(pcur, tmp.x, tmp.y, 0x8A, a4, a4);
                                }
                            }
                        }
                    }
                }
                else if ( gate.PCell->PurposeType == cellArea::PT_GATEOPENED )
                {
                    if ( gate.PCell->IsCanSee(yw->_userRobo->_owner) )
                    {
                        if ( yw->_timeStamp / 300 & 1 )
                        {
                            vec2d tmp = World::SectorIDToCenterPos2( gate.CellId );
                            pcur = sub_4F6980(pcur, tmp.x, tmp.y, 0x94, a4, a4);
                        }
                    }
                }
            }

            for (const TMapSuperItem &sitem : yw->_levelInfo.SuperItems)
            {
                int v29 = 0;
                int v61 = 0;

                if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
                {
                    v29 = 0x8E;

                    if ( sitem.State == TMapSuperItem::STATE_INACTIVE )
                        v61 = 0x8B;
                    else if ( sitem.State == TMapSuperItem::STATE_ACTIVE ||
                              sitem.State == TMapSuperItem::STATE_STOPPED )
                        v61 = 0x8C;
                    else if ( sitem.State == TMapSuperItem::STATE_TRIGGED )
                        v61 = 0x8D;
                }
                else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
                {
                    v29 = 0x92;

                    if ( sitem.State == TMapSuperItem::STATE_INACTIVE )
                        v61 = 0x8F;
                    else if ( sitem.State == TMapSuperItem::STATE_ACTIVE ||
                              sitem.State == TMapSuperItem::STATE_STOPPED )
                        v61 = 0x90;
                    else if ( sitem.State == TMapSuperItem::STATE_TRIGGED )
                        v61 = 0x91;
                }

                if ( v61 )
                {
                    int v34 = 0;

                    if ( sitem.PCell->IsCanSee(yw->_userRobo->_owner) )
                    {
                        if ( sitem.PCell->owner == yw->_userRobo->_owner || yw->_timeStamp / 300 & 1 )
                            v34 = 1;
                    }

                    if ( v34 )
                    {
                        vec2d tmp = World::SectorIDToCenterPos2( sitem.CellId );
                        pcur = sub_4F6980(pcur, tmp.x, tmp.y, v61, a4, a4);
                    }

                    if ( v29 )
                    {
                        for ( const TMapKeySector &ks : sitem.KeySectors )
                        {
                            if ( ks.PCell )
                            {
                                if ( ks.PCell->IsCanSee(yw->_userRobo->_owner) )
                                {
                                    if ( ks.PCell->owner == yw->_userRobo->_owner || yw->_timeStamp / 500 & 1 )
                                    {
                                        vec2d tmp = World::SectorIDToCenterPos2( ks.CellId );
                                        pcur = sub_4F6980(pcur, tmp.x, tmp.y, v61, a4, a4);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return pcur;
}

char * sub_4F6DFC(NC_STACK_ypaworld *yw, char *cur, int height, int width, NC_STACK_ypabact *bact, int a6)
{
    char *pcur = cur;

    if ( bact->_status != BACT_STATUS_DEAD && !bact->IsHidden() )
    {
        int v8;

        if ( a6 )
        {
            if ( bact->_bact_type == BACT_TYPES_MISSLE )
                v8 = bact->_owner + 1;
            else
                v8 = yw->_vhclProtos[bact->_vehicleID].type_icon;
        }
        else
        {
            switch ( bact->_bact_type )
            {
            case BACT_TYPES_BACT:
                v8 = bact->_owner + 9;
                break;

            case BACT_TYPES_ROBO:
                switch ( robo_map.field_1EE )
                {
                case 0:
                case 1:
                    v8 = bact->_owner + 0x30;
                    break;

                case 2:
                    v8 = bact->_owner + 0x28;
                    break;

                case 3:
                    v8 = bact->_owner + 0x20;
                    break;

                case 4:
                    v8 = bact->_owner + 0x18;
                    break;
                }
                break;

            case BACT_TYPES_MISSLE:
                v8 = bact->_owner + 1;
                break;

            case BACT_TYPES_TANK:
            case BACT_TYPES_CAR:
                v8 = bact->_owner + 0x19;
                break;

            case BACT_TYPES_FLYER:
                v8 = bact->_owner + 0x11;
                break;

            case BACT_TYPES_UFO:
                v8 = bact->_owner + 0x21;
                break;

            case BACT_TYPES_GUN:
                v8 = bact->_owner + 0x29;
                break;

            default:
                v8 = 0x41;
                break;
            }
        }

        pcur = sub_4F6980(pcur, bact->_position.x, bact->_position.z, v8, height, width);

        if ( bact->_bact_type != BACT_TYPES_MISSLE && robo_map.field_1EE > 2 )
        {
            SDL_Color clr = yw->GetColor(bact->_owner);

            int v31 = 0;
            float v33, v35;

            float v21;
            float v22;
            float v23;
            float v24;
            float v25;

            if ( bact == yw->_userRobo )
            {
                v33 = bact->_viewer_rotation.m02;
                v35 = bact->_viewer_rotation.m22;

                if ( bact == yw->_userUnit )
                {
                    v21 = -380.0;
                    v22 = 380.0;

                    v23 = -300.0;
                    v24 = 600.0;
                    v25 = -300.0;

                    v31 = 1;

                    clr = yw->GetColor(13);
                }
                else
                {
                    v21 = -85.0;
                    v22 = 85.0;

                    v23 = 0.0;
                    v24 = 700.0;
                    v25 = 0.0;
                }
            }
            else if ( bact == yw->_userUnit )
            {
                v33 = yw->_viewerRotation.m02;
                v35 = yw->_viewerRotation.m22;

                v21 = -190.0;
                v22 = 190.0;

                v23 = -200.0;
                v24 = 400.0;
                v25 = -200.0;

                v31 = 1;

                clr = yw->GetColor(13);
            }
            else
            {
                v33 = bact->_rotation.m02;
                v35 = bact->_rotation.m22;

                v21 = -40.0;
                v22 = 40.0;

                v23 = 0.0;
                v24 = 350.0;
                v25 = 0.0;
            }

            float v30 = sqrt(POW2(v33) + POW2(v35));
            float v34, v36;

            if ( v30 <= 0.0 )
            {
                v36 = 1.0;
                v34 = 0.0;
            }
            else
            {
                v34 = v33 / v30;
                v36 = v35 / v30;
            }

            float v14 = -v34;

            float a3 = v36 * v21 + v14 * v23 + bact->_position.x;
            float v28 = v34 * v21 + v36 * v23 + bact->_position.z;
            float v16 = v36 * 0.0 + v14 * v24 + bact->_position.x;
            float v15 = v34 * 0.0 + v36 * v24 + bact->_position.z;

            sub_4F68FC(a3, v28, v16, v15, clr);

            float v27 = v36 * v22 + v14 * v25 + bact->_position.x;
            float v29 = v34 * v22 + v36 * v25 + bact->_position.z;

            sub_4F68FC(v27, v29, v16, v15, clr);

            if ( v31 )
                sub_4F68FC(a3, v28, v27, v29, clr); // Also draw base of view triangle
        }

        if ( bact->_bact_type != BACT_TYPES_MISSLE && bact->_bact_type != BACT_TYPES_ROBO && bact->_owner == yw->_userRobo->_owner )
        {
            if ( robo_map.field_1EC & 4 && robo_map.field_1EE > 2 )
            {
                float v32 = (float)bact->_energy / (float)bact->_energy_max;
                int v19;

                if ( v32 > 0.25 )
                {
                    if ( v32 > 0.5 )
                    {
                        if ( v32 > 0.75 )
                            v19 = 0x83;
                        else
                            v19 = 0x82;
                    }
                    else
                    {
                        v19 = 0x81;
                    }
                }
                else
                {
                    v19 = 0x80;
                }

                pcur = sub_4F6980(pcur, bact->_position.x, bact->_position.z, v19, height, width);
            }
        }
    }
    return pcur;
}

NC_STACK_ypabact * sub_4D3C3C(NC_STACK_ypabact *bact)
{
    if ( !bact )
        return NULL;

    if ( !bact->_parent )
        return NULL;

    if ( bact->_parent != bact->_host_station )
        return bact->_parent;

    return bact;
}

char * sub_4F7BE8(NC_STACK_ypaworld *yw, char *cur, NC_STACK_ypabact *bact, int a2, int a4, int a5, int a6)
{
    char *pcur = cur;

    if ( bact )
    {
        FontUA::select_tileset(&pcur, a2);

        pcur = sub_4F6980(pcur, bact->_position.x, bact->_position.z, a4, a5, a6);

        for( NC_STACK_ypabact* &kid : bact->_kidList )
        {
            if ( kid->_status != BACT_STATUS_DEAD )
                pcur = sub_4F6980(pcur, kid->_position.x, kid->_position.z, a4, a5, a6);
        }
    }
    return pcur;
}

int NC_STACK_ypaworld::sb_0x4f8f64__sub3__sub0(const Common::Point &sector)
{
    int enrg = 0;

    for( NC_STACK_ypabact* &v2 : _cells(sector).unitsList )
    {
        if ( v2->_status != BACT_STATUS_DEAD && v2->_status != BACT_STATUS_BEAM )
        {
            if ( v2->_bact_type != BACT_TYPES_ROBO && v2->_bact_type != BACT_TYPES_MISSLE && v2->_bact_type != BACT_TYPES_GUN )
                enrg += (v2->_energy_max + 99) / 100;
        }
    }

    return enrg;
}

char * sb_0x4f8f64__sub3__sub1(NC_STACK_ypaworld *yw, const std::string &labl, int tileset_id, char *cur, int a4, int a5)
{
    int v8 = yw->_guiTiles[tileset_id]->GetWidth(labl);

    Common::Point tmp = sub_4F681C( World::SectorIDToPos2( {a4, a5} ) );

    int v9 = tmp.x - robo_map.field_200;
    int v10 = tmp.y - robo_map.field_204;

    char *pcur = cur;

    if ( tmp.x - robo_map.field_200 > 0 && v10 > 0 && tmp.x - robo_map.field_200 + v8 < robo_map.field_1F8 && tmp.y - robo_map.field_204 + yw->_guiTiles[tileset_id]->h < robo_map.field_1FC )
    {
        FontUA::select_tileset(&pcur, tileset_id);
        FontUA::set_center_xpos(&pcur, robo_map.field_200 + v9);
        FontUA::set_center_ypos(&pcur, robo_map.field_204 + v10);

        for ( uint8_t c : labl )
            FontUA::store_s8(&pcur, c);
    }

    return pcur;
}

char * sb_0x4f8f64__sub3(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int v111;
    int a5;
    int a4;
    int v114;
    int v110;
    int v7;
    int v99;

    switch ( robo_map.field_1EE )
    {
    case 0:
        v111 = 4;
        v114 = 59;
        v110 = 16;
        a5 = yw->_guiTiles[59]->h;
        a4 = yw->_guiTiles[59]->map[1].w;
        v7 = 23;
        v99 = 19;
        break;

    case 1:
        v114 = 59;
        v111 = 8;
        v110 = 17;
        a5 = yw->_guiTiles[59]->h;
        a4 = yw->_guiTiles[59]->map[1].w;
        v7 = 23;
        v99 = 19;
        break;

    case 2:
        v111 = 16;
        v114 = 60;
        v110 = 18;
        a5 = yw->_guiTiles[60]->h;
        a4 = yw->_guiTiles[60]->map[1].w;
        v7 = 22;
        v99 = 18;
        break;

    case 3:
        v110 = 19;
        v111 = 32;
        v114 = 61;
        a5 = yw->_guiTiles[61]->h;
        a4 = yw->_guiTiles[61]->map[1].w;
        v7 = 21;
        v99 = 17;
        break;

    case 4:
        v110 = 20;
        v111 = 64;
        v114 = 28;
        a5 = yw->_guiTiles[28]->h;
        a4 = yw->_guiTiles[28]->map[1].w;
        v7 = 20;
        v99 = 16;
        break;
    }


    switch ( yw->_doAction )
    {
    case World::DOACTION_2:
        if ( !(robo_map.field_1E8 & 0x200) )
        {
            if ( yw->_guiActFlags & 0x20 )
            {
                FontUA::select_tileset(&pcur, v114);

                pcur = sub_4F6980(pcur, yw->_bactOnMouse->_position.x, yw->_bactOnMouse->_position.z, 0x86, a4, a5);
            }
            else if ( yw->_guiActFlags & 0x10 && yw->_mouseGrabbed == false)
            {
                if ( yw->_cellOnMouse->owner == yw->_userRobo->_owner && yw->_cellOnMouse->IsCanSee(yw->_userRobo->_owner) )
                {
                    FontUA::select_tileset(&pcur, v114);

                    pcur = sub_4F6980(pcur, yw->_cellMouseIsectPos.x, yw->_cellMouseIsectPos.z, 0x88, a4, a5);
                }
                else
                {
                    FontUA::select_tileset(&pcur, v110);

                    pcur = sub_4F6980(pcur, yw->_cellOnMouse->CenterPos.x, yw->_cellOnMouse->CenterPos.z, 66, v111, v111);
                }
            }
        }
        break;

    case World::DOACTION_5:
        FontUA::select_tileset(&pcur, v114);

        pcur = sub_4F6980(pcur, yw->_bactOnMouse->_position.x, yw->_bactOnMouse->_position.z, 0x85, a4, a5);
        break;

    case World::DOACTION_6:
        FontUA::select_tileset(&pcur, v110);

        pcur = sub_4F6980(pcur, yw->_cellOnMouse->CenterPos.x, yw->_cellOnMouse->CenterPos.z, 65, v111, v111);
        break;

    case World::DOACTION_8:
        if ( !(robo_map.field_1E8 & 0x200) )
        {
            if ( yw->_bactOnMouse )
            {
                if ( yw->_bactOnMouse == yw->_userRobo )
                {
                    int v22 = yw->_guiTiles[1]->h;

                    FontUA::select_tileset(&pcur, 1);

                    pcur = sub_4F6980(pcur, yw->_bactOnMouse->_position.x, yw->_bactOnMouse->_position.z, v7, v22, v22);
                }
                else
                {
                    NC_STACK_ypabact *v25 = sub_4D3C3C(yw->_bactOnMouse);

                    if ( v25 )
                    {
                        pcur = sub_4F7BE8(yw, pcur, v25, v114, 0x85, a4, a5);
                    }

                }
            }
        }
        break;

    case World::DOACTION_10:
        FontUA::select_tileset(&pcur, v114);

        pcur = sub_4F6980(pcur, yw->_cellMouseIsectPos.x, yw->_cellMouseIsectPos.z, 0x88, a4, a5);
        break;

    default:
        break;
    }

    if ( bzda.field_1D0 & 0x20 )
    {
        FontUA::select_tileset(&pcur, 1);

        int v44 = yw->_guiTiles[1]->h;

        pcur = sub_4F6980(pcur, yw->_userRobo->_position.x, yw->_userRobo->_position.z, v99, v44, v44);
    }
    else
    {
        if ( yw->_activeCmdrRemapIndex != -1 )
        {
            NC_STACK_ypabact *v47 = yw->_cmdrsRemap[yw->_activeCmdrRemapIndex];

            if ( v47 )
            {
                pcur = sub_4F7BE8(yw, pcur, v47, v114, 0x84, a4, a5);

                if ( v47->_primTtype == BACT_TGT_TYPE_CELL )
                {
                    if ( v47->_primT.pcell->owner != yw->_userRobo->_owner )
                    {
                        vec2d tmp = World::SectorIDToCenterPos2( World::PositionToSectorID(v47->_primTpos) );

                        FontUA::select_tileset(&pcur, v110);

                        pcur = sub_4F6980(pcur, tmp.x, tmp.y, 66, v111, v111);

                        FontUA::select_tileset(&pcur, v114);

                        pcur = sub_4F6980(pcur, v47->_primTpos.x, v47->_primTpos.z, 0x88, a4, a5);
                    }
                    else
                    {
                        FontUA::select_tileset(&pcur, v114);

                        pcur = sub_4F6980(pcur, v47->_primTpos.x, v47->_primTpos.z, 0x88, a4, a5);
                    }
                }
                else if ( v47->_primTtype == BACT_TGT_TYPE_UNIT )
                {
                    FontUA::select_tileset(&pcur, v114);

                    pcur = sub_4F6980(pcur, v47->_primT.pbact->_position.x, v47->_primT.pbact->_position.z, 0x86, a4, a5);
                }

                if ( v47->_status_flg & BACT_STFLAG_WAYPOINT )
                {
                    if ( v47->_waypoints_count > 0 )
                    {
                        int v68;

                        if ( v47->_status_flg & BACT_STFLAG_WAYPOINTCCL )
                            v68 = 0;
                        else
                            v68 = v47->_current_waypoint;

                        for (int i = v68; i < v47->_waypoints_count; i++)
                        {
                            FontUA::select_tileset(&pcur, v114);

                            pcur = sub_4F6980(pcur, v47->_waypoints[i].x, v47->_waypoints[i].z, 0x88, a4, a5);
                        }
                    }
                }
            }
        }
    }

    if ( yw->_userUnit )
    {
        if ( yw->_timeStamp / 300 & 1 )
        {
            FontUA::select_tileset(&pcur, v114);

            pcur = sub_4F6980(pcur, yw->_userUnit->_position.x, yw->_userUnit->_position.z, 0x89, a4, a5);
        }
    }

    if ( yw->_lastMsgSender )
    {
        if ( yw->_timeStamp / 300 & 1 )
        {
            FontUA::select_tileset(&pcur, v114);

            pcur = sub_4F6980(pcur, yw->_lastMsgSender->_position.x, yw->_lastMsgSender->_position.z, 0x8A, a4, a5);
        }
    }

    for ( const TMapGate &gate : yw->_levelInfo.Gates )
    {
        if ( gate.PCell->PurposeType == cellArea::PT_GATEOPENED )
        {
            int v81 = yw->sb_0x4f8f64__sub3__sub0(gate.CellId);

            int v85 = 1;

            if ( 2 * yw->_beamEnergyCapacity < 2 * v81 )
            {
                v85 = (yw->_timeStamp / 300) & 1; // FLASHING GATES TITLE
            }

            if ( v85 )
                pcur = sb_0x4f8f64__sub3__sub1(yw,  fmt::sprintf("%d / %d", 2 * v81, 2 * yw->_beamEnergyCapacity), 0x1F, pcur, gate.CellId.x, gate.CellId.y);
        }
    }
    return pcur;
}

char * sb_0x4f8f64__sub0(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    FontUA::select_tileset(&pcur, 10);
    FontUA::set_center_xpos(&pcur, robo_map.x + robo_map.field_234 + robo_map.field_244 - (yw->_screenSize.x / 2));
    FontUA::set_center_ypos(&pcur, robo_map.y + robo_map.h - (robo_map.field_230 + robo_map.field_240) - (yw->_screenSize.y / 2));

    if ( robo_map.field_1EC & 1 )
        FontUA::store_u8(&pcur, 97);
    else
        FontUA::store_u8(&pcur, 65);

    if ( robo_map.field_1EC & 2 )
        FontUA::store_u8(&pcur, 98);
    else
        FontUA::store_u8(&pcur, 66);

    if ( robo_map.field_1EC & 4 )
        FontUA::store_u8(&pcur, 100);
    else
        FontUA::store_u8(&pcur, 68);

    if ( robo_map.field_1ED == 1 )
        FontUA::store_u8(&pcur, 102);
    else
        FontUA::store_u8(&pcur, 70);

    if ( robo_map.field_1E8 & 0x40 )
        FontUA::store_u8(&pcur, 106);
    else
        FontUA::store_u8(&pcur, 74);

    if ( robo_map.field_1E8 & 0x80 )
        FontUA::store_u8(&pcur, 107);
    else
        FontUA::store_u8(&pcur, 75);

    return pcur;
}

void sb_0x4f8f64(NC_STACK_ypaworld *yw)
{
    w3d_a209 arg;
    arg.cmdbuf = t1_cmdbuf_3;
    arg.includ = NULL;

    int v41 = 0;

    int v39 = robo_map.field_1EE + 2;

    int a6 = 0;
    int setid = 59;
    switch ( robo_map.field_1EE )
    {
    case 0:
    case 1:
        setid = 59;
        break;

    case 2:
        setid = 60;
        break;

    case 3:
        setid = 61;
        break;

    case 4:
        a6 = 1;
        setid = 28;
        break;
    }

    int height = yw->_guiTiles[setid]->map[128].w;
    int width = yw->_guiTiles[setid]->h;

    robo_map.field_1F8 = robo_map.w - robo_map.field_24C;
    robo_map.field_1FC = robo_map.h - robo_map.field_250;

    robo_map.field_200 = robo_map.x + robo_map.field_244 - (yw->_screenSize.x / 2);
    robo_map.field_204 = robo_map.y + robo_map.field_23C - (yw->_screenSize.y / 2);

    Common::Rect rect;
    rect.left = robo_map.field_200;
    rect.right = robo_map.w - robo_map.field_24C + robo_map.field_200 - 1;
    rect.top = robo_map.y + robo_map.field_23C - (yw->_screenSize.y / 2);
    rect.bottom = robo_map.h - robo_map.field_250 + rect.top - 1;

    robo_map.field_1F0 = robo_map.field_1D8 / robo_map.field_1E0 - (robo_map.w - robo_map.field_24C) / 2;
    robo_map.field_1F4 = -(robo_map.field_1DC / robo_map.field_1E4 + (robo_map.h - robo_map.field_250) / 2);

    GFX::Engine.raster_func211(rect);

    int v33 = robo_map.field_1F0 >> v39;
    int v38 = robo_map.field_1F4 >> v39;
    int v43 = (robo_map.field_1F8 + robo_map.field_1F0) >> v39;
    int vii = (robo_map.field_1FC + robo_map.field_1F4) >> v39;

    if ( v33 < 1 )
        v33 = 1;

    if ( v38 < 1 )
        v38 = 1;

    if ( v43 >= yw->_mapSize.x )
        v43 = yw->_mapSize.x - 1;

    if ( vii >= yw->_mapSize.y )
        vii = yw->_mapSize.y - 1;

    char *pcur = sb_0x4f8f64__sub2(yw, t1_cmdbuf_3);

    FontUA::select_tileset(&pcur, setid);

    sb_0x4f8f64__sub1(yw);

    for (int v42 = v38; v42 <= vii; v42++)
    {
        for (int j = v33; j <= v43; j++ )
        {
            if ( v41 < 2048 )
            {
                cellArea &cell = yw->_cells(j, v42);

                if ( robo_map.MapViewMask & cell.view_mask )
                {
                    for ( NC_STACK_ypabact* &bct : cell.unitsList )
                    {
                        if ( bct->_bact_type == BACT_TYPES_ROBO )
                        {
                            FontUA::select_tileset(&pcur, 1);

                            pcur = sub_4F6DFC(yw, pcur, yw->_guiTiles[1]->h, yw->_guiTiles[1]->map[24].w, bct, 0);

                            FontUA::select_tileset(&pcur, setid);

                            v41++;
                        }
                        else if ( bct->_bact_type == BACT_TYPES_GUN )
                        {
                            NC_STACK_ypagun *gn = (NC_STACK_ypagun *)bct;

                            if ( !gn->IsRoboGun() || bct == yw->_userUnit )
                            {
                                pcur = sub_4F6DFC(yw, pcur, height, width, bct, a6);
                                v41++;
                            }
                        }
                        else
                        {
                            pcur = sub_4F6DFC(yw, pcur, height, width, bct, a6);
                            v41++;
                        }
                    }

                }
            }
        }
    }

    if ( robo_map.field_1E8 & 0x200 )
    {
        for (int l = v38; l <= vii; l++ )
        {
            for (int m = v33; m <= v43; m++ )
            {
                if ( v41 < 2048 )
                {
                    cellArea &cell = yw->_cells(m, l);

                    if ( robo_map.MapViewMask & cell.view_mask )
                    {
                        for ( NC_STACK_ypabact* &bct : cell.unitsList )
                        {
                            if ( bct->_owner == yw->_userRobo->_owner
                                    && bct->_position.x > flt_516524
                                    && bct->_position.z > flt_516528
                                    && bct->_position.x < flt_51652C
                                    && bct->_position.z < flt_516530 )
                            {
                                if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_ROBO && bct->_bact_type != BACT_TYPES_GUN )
                                {
                                    if ( bct->_status != BACT_STATUS_CREATE && bct->_status != BACT_STATUS_BEAM && bct->_status != BACT_STATUS_DEAD )
                                    {
                                        pcur = sub_4F6980(pcur, bct->_position.x, bct->_position.z, 0x85, height, width);
                                        v41++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    pcur = sb_0x4f8f64__sub3(yw, pcur);
    pcur = sb_0x4f8f64__sub0(yw, pcur);
    FontUA::set_end(&pcur);

    GFX::Engine.DrawText(&arg);
}

void sub_4C157C(NC_STACK_ypaworld *yw)
{
    float v1 = (robo_map.h - robo_map.field_250) * robo_map.field_1E4;
    float v3 = (robo_map.w - robo_map.field_24C) * robo_map.field_1E0;

    float v6 = (yw->_mapSize.x * World::CVSectorLength);
    int v20 = robo_map.w - robo_map.field_248;

    int v17 = ((robo_map.field_1D8 - v3 * 0.5) * v20) / v6;
    int v19 = v20 * v3 / v6;



    if ( v19 >= robo_map.field_228 )
    {
        if ( v19 > v20 )
            v19 = v20;
    }
    else
    {
        v17 -= (robo_map.field_228 - v19) >> 1;
        v19 = robo_map.field_228;
    }

    if ( v17 < 0 )
        v17 = 0;

    if ( v17 + v19 > v20 )
        v17 = v20 - v19;

    robo_map.field_1CC = v20;
    robo_map.field_1CE = v17;
    robo_map.field_1D0 = v19;




    float v15 = -(robo_map.field_1DC + v1 * 0.5);
    float v14 = (yw->_mapSize.y * World::CVSectorLength);

    v20 = robo_map.h - robo_map.field_250;
    v19 = (v20 * v1 / v14);

    int v18 = ((v20 * v15) / v14);


    if ( v19 >= robo_map.field_228 )
    {
        if ( v19 > v20 )
            v19 = v20;
    }
    else
    {
        v18 -= (robo_map.field_228 - v19) >> 1;
        v19 = robo_map.field_228;
    }

    if ( v18 < 0 )
        v18 = 0;

    if ( v19 + v18 > v20 )
        v18 = v20 - v19;

    robo_map.field_1D2 = v20;
    robo_map.field_1D4 = v18;
    robo_map.field_1D6 = v19;
}

void sub_4C0C00(NC_STACK_ypaworld *yw)
{
    int v20 = robo_map.x - (yw->_screenSize.x / 2);
    int v21 = robo_map.y - (yw->_screenSize.y / 2);
    int v22 = robo_map.w;
    int v23 = robo_map.h;

    robo_map.buttons[0] = ButtonBox(robo_map.w - yw->_fontDefCloseW,     0,
                                    yw->_fontDefCloseW,    yw->_fontH);

    robo_map.buttons[2] = ButtonBox(robo_map.w - 2 * yw->_fontDefCloseW, 0,
                                    yw->_fontDefCloseW,    yw->_fontH);

    robo_map.buttons[1] = ButtonBox(0, 0,
                                    robo_map.buttons[2].x - robo_map.field_238, yw->_fontH);

    robo_map.buttons[9] = ButtonBox(robo_map.buttons[2].x - robo_map.field_238, 0,
                                    robo_map.field_238,       yw->_fontH);

    int v5 = robo_map.field_234 + robo_map.field_244;

    for (int i = 3; i < 9; i++)
    {
        robo_map.buttons[i] = ButtonBox(v5,  v23 - robo_map.field_240 - robo_map.field_230,
                                        robo_map.field_22C,  robo_map.field_230);

        v5 += robo_map.field_22C;
    }

    int v7;

    if ( robo_map.field_1E8 & 0x20 )
        v7 = 's'; //default fon
    else
        v7 = 'q'; //default fon (

    char *pcur = GuiBase::FormateTitle(yw, v20, v21, v22, Locale::Text::WinName(Locale::WINNAME_MAP), robo_map.cmdstrm.cmdbuf, v7, robo_map.flags);

    FontUA::next_line(&pcur);
    FontUA::reset_tileset(&pcur, 13);

    FontUA::store_u8(&pcur, 65);

    FontUA::next_line(&pcur);
    FontUA::reset_tileset(&pcur, 12);

    int v13 = v23 - robo_map.field_250 - 1;
    while (v13 > yw->_fontVScrollH)
    {
        FontUA::store_u8(&pcur, 65);
        FontUA::next_line(&pcur);

        v13 -= yw->_fontVScrollH;
    }

    if ( v13 > 1 )
    {
        FontUA::set_yheight(&pcur, v13 - 1);
        FontUA::store_u8(&pcur, 65);
        FontUA::next_line(&pcur);
    }

    FontUA::reset_tileset(&pcur, 13);
    FontUA::store_u8(&pcur, 66);
    FontUA::next_line(&pcur);

    FontUA::include(&pcur, 0);
    FontUA::include(&pcur, 1);
    FontUA::include(&pcur, 2);
    FontUA::set_end(&pcur);
}

void sub_4C0FEC(NC_STACK_ypaworld *yw)
{
    int v1 = robo_map.x - (yw->_screenSize.x / 2);
    int v3 = robo_map.y + robo_map.h - robo_map.field_240 - (yw->_screenSize.y / 2);

    robo_map.buttons[13] = ButtonBox(0, robo_map.h - robo_map.field_240,
                                     robo_map.field_1CE, robo_map.field_240);

    robo_map.buttons[14] = ButtonBox(robo_map.field_1CE, robo_map.h - robo_map.field_240,
                                     robo_map.field_1D0, robo_map.field_240);

    robo_map.buttons[16] = ButtonBox(robo_map.w - robo_map.field_248,  robo_map.h - robo_map.field_240,
                                     robo_map.field_248,  robo_map.field_240);

    robo_map.buttons[15] = ButtonBox(robo_map.field_1D0 + robo_map.field_1CE,   robo_map.h - robo_map.field_240,
                                     robo_map.field_1CC - robo_map.field_1CE - robo_map.field_1D0,   robo_map.field_240);

    char *pcur = t1_cmdbuf_2;

    FontUA::select_tileset(&pcur, 11);
    FontUA::set_center_xpos(&pcur, v1);
    FontUA::set_center_ypos(&pcur, v3);

    int v4 = robo_map.field_1CE;

    if ( v4 > 0 )
    {
        FontUA::store_u8(&pcur, 65);
        if ( v4 > 1 )
        {
            FontUA::op17(&pcur, v4);
            FontUA::store_u8(&pcur, 66);
        }
    }

    FontUA::store_u8(&pcur, 68);

    FontUA::op17(&pcur, robo_map.field_1D0 + v4 - 1);

    FontUA::store_u8(&pcur, 69);
    FontUA::store_u8(&pcur, 70);


    int v11 = v4 + robo_map.field_1D0;
    if ( v11 < robo_map.field_1CC )
    {
        if ( v11 < robo_map.field_1CC - 1 )
        {
            FontUA::op17(&pcur, robo_map.field_1CC - 1);

            FontUA::store_u8(&pcur, 66);
        }
        FontUA::store_u8(&pcur, 67);
    }

    FontUA::store_u8(&pcur, 71);

    FontUA::set_end(&pcur);
}

void sub_4C1214(NC_STACK_ypaworld *yw)
{
    int v2 = robo_map.x + robo_map.w - robo_map.field_248 - (yw->_screenSize.x / 2);
    int v3 = robo_map.y + robo_map.field_23C - (yw->_screenSize.y / 2);

    robo_map.buttons[10] = ButtonBox(robo_map.w - robo_map.field_248,    robo_map.field_23C,
                                     robo_map.field_248,   robo_map.field_1D4);

    robo_map.buttons[11] = ButtonBox(robo_map.w - robo_map.field_248,    robo_map.field_1D4 + robo_map.field_23C,
                                     robo_map.field_248,   robo_map.field_1D6);

    robo_map.buttons[12] = ButtonBox(robo_map.w - robo_map.field_248,    robo_map.field_1D6 + robo_map.field_1D4 + robo_map.field_23C,
                                     robo_map.field_248,   robo_map.field_1D2 - robo_map.field_1D4 - robo_map.field_1D6);

    char *pcur = t1_cmdbuf_1;

    FontUA::set_center_xpos(&pcur, v2);
    FontUA::set_center_ypos(&pcur, v3);


    if ( robo_map.field_1D4 > 0 )
    {
        FontUA::reset_tileset(&pcur, 13);

        FontUA::store_u8(&pcur, 67);
        FontUA::next_line(&pcur);

        FontUA::reset_tileset(&pcur, 12);

        int v9 = robo_map.field_1D4 - 1;

        while (v9 >= yw->_fontVScrollH )
        {
            FontUA::store_u8(&pcur, 66);
            FontUA::next_line(&pcur);

            v9 -= yw->_fontVScrollH;
        }

        if ( v9 > 0 )
        {
            FontUA::set_yheight(&pcur, v9);
            FontUA::store_u8(&pcur, 66);
            FontUA::next_line(&pcur);
        }
    }

    if ( robo_map.field_1D6 > 0 )
    {
        FontUA::reset_tileset(&pcur, 13);
        FontUA::store_u8(&pcur, 69);

        FontUA::next_line(&pcur);

        FontUA::reset_tileset(&pcur, 12);


        int v14 = robo_map.field_1D6 - 1;

        while ( v14 > yw->_fontVScrollH )
        {
            FontUA::store_u8(&pcur, 67);
            FontUA::next_line(&pcur);

            v14 -= yw->_fontVScrollH;
        }

        if ( v14 > 1 )
        {
            FontUA::set_yheight(&pcur, v14 - 1);
            FontUA::store_u8(&pcur, 67);
            FontUA::next_line(&pcur);
        }

        FontUA::reset_tileset(&pcur, 13);

        FontUA::store_u8(&pcur, 70);
        FontUA::next_line(&pcur);
    }

    int v21 = robo_map.field_1D2 - robo_map.field_1D4 - robo_map.field_1D6;

    if ( v21 > 0 )
    {
        FontUA::reset_tileset(&pcur, 12);

        while ( v21 > yw->_fontVScrollH )
        {
            FontUA::store_u8(&pcur, 66);
            FontUA::next_line(&pcur);

            v21 -= yw->_fontVScrollH;
        }

        if ( v21 > 1 )
        {
            FontUA::set_yheight(&pcur, v21 - 1);

            FontUA::store_u8(&pcur, 66);
            FontUA::next_line(&pcur);
        }
        FontUA::reset_tileset(&pcur, 13);

        FontUA::store_u8(&pcur, 68);
        FontUA::next_line(&pcur);
    }
    FontUA::set_end(&pcur);
}



char * sub_4F6114(NC_STACK_ypaworld *yw, int a2, char *cmdbuf, int a3, int a4, int a5, int a6, int a7, mapFunc ffunc, int xpos, int ypos)
{
    char *pcur = cmdbuf;

    int v33;

    if ( a2 == 4)
        v33 = 2;
    else if ( a2 == 8)
        v33 = 3;
    else if ( a2 == 16 )
        v33 = 4;
    else if ( a2 == 32 )
        v33 = 5;
    else if ( a2 == 64 )
        v33 = 6;
    else
    {
        ypa_log_out("yw_maprnd.c/yw_RMapLego(): wrong size\n");

        FontUA::set_end(&pcur);
        return pcur;
    }

    FontUA::set_center_xpos(&pcur, xpos);
    FontUA::set_center_ypos(&pcur, ypos);

    int msk = a2 - 1;

    FontUA::reset_tileset(&pcur, a7);

    int row = a4;

    while (row < a6)
    {
        int v37 = 0;
        int col = a3;
        int v17 = row >> v33;

        if ( msk & row )
        {
            FontUA::set_yoff(&pcur, row & msk);

            row += a2 - (msk & row);
        }
        else
        {
            if ( a6 - row >= a2 )
            {
                row += a2;
            }
            else
            {
                FontUA::set_yheight(&pcur, a6 - row);

                row = a6;
            }
        }

        if ( col & msk )
        {
            int v37_4 = msk & col;

            int v21 = ffunc(yw, col >> v33, v17) & 0xFF;

            if ( v21 )
            {
                FontUA::set_xoff(&pcur, v37_4);
                FontUA::store_u8(&pcur, v21);

                v37 = 0;
            }
            else
            {
                v37 = a2 - v37_4;

                FontUA::add_xpos(&pcur, v37);
            }

            col += a2 - v37_4;
        }

        while ( col < a5 - a2 )
        {
            int v24 = ffunc(yw, col >> v33, v17) & 0xFF;

            if ( v24 )
            {
                FontUA::store_u8(&pcur, v24);

                v37 = 0;
            }
            else
            {
                if ( v37 <= 0 )
                {
                    v37 = a2;

                    FontUA::add_xpos(&pcur, v37);
                }
                else
                {
                    v37 += a2;

                    pcur -= 2; //HACKY rewrite
                    FontUA::store_s16(&pcur, v37);
                }
            }

            col += a2;
        }

        if ( a5 - col > 0 )
        {
            int v28 = ffunc(yw, col >> v33, v17) & 0xFF;
            if ( v28 )
            {
                FontUA::set_xwidth(&pcur, a5 - col);
                FontUA::store_u8(&pcur, v28);
            }
        }
        FontUA::next_line(&pcur);

    }

    return pcur;
}



int sub_4F60A4(NC_STACK_ypaworld *yw, int x, int y)
{
    if ( yw->IsSector( {x, y} ) )
    {
        if ( (robo_map.field_1EC & 3) && (robo_map.MapViewMask & yw->_cells(x, y).view_mask) )
            return 0;
        else
            return 9;
    }
    else
        return 9;
}

int sub_4F6048(NC_STACK_ypaworld *yw, int x, int y)
{
    if ( yw->IsSector( {x, y} ) )
    {
        cellArea &v4 = yw->_cells(x, y);

        if ( robo_map.MapViewMask & v4.view_mask )
        {
            if ( !v4.owner )
                return 8;
            else
                return v4.owner;
        }
    }

    return 0;
}

int sub_4F5FE0(NC_STACK_ypaworld *yw, int x, int y)
{
    if ( yw->IsSector( {x, y} ) )
    {
        cellArea &v6 = yw->_cells(x, y);

        if ( robo_map.MapViewMask & v6.view_mask )
            return yw->_secTypeArray[v6.type_id].GUIElementID;
    }

    return 0;
}

int sub_4F5CEC(NC_STACK_ypaworld *yw, int x, int y)
{
    int v9 = x / 4;
    int v8 = x & 3;
    int v7 = y / 4;
    int v27 = y & 3;

    if ( !yw->IsSector( {v9, v7} ) )
        return 0;

    cellArea &v12 = yw->_cells(v9, v7);

    if ( !(robo_map.MapViewMask & v12.view_mask) )
        return 0;

    if ( v8 && v27 )
    {
        if ( v12.SectorType == 1 )
        {
            TLego *v26 = &yw->_legoArray[ yw->GetLegoBld(&v12, 0, 0) ];
            int v25 = (16 * (v27 - 1) + v8 - 1 + v26->GUIElementID) & 0xFF;
            return v25;
        }
        else
        {
            TLego *v26 = &yw->_legoArray[ yw->GetLegoBld(&v12, v8 - 1, 2 - (v27 - 1))  ];
            return v26->GUIElementID;
        }
    }
    else
    {
        if ( v9 < 1 || v7 < 1 )
            return 1;

        int v13 = 0;

        cellArea *v15;
        cellArea *v16;
        cellArea *v17;

        if ( v8 )
        {
            v15 = &v12;
            v16 = &yw->_cells(v9, v7 - 1);
            v17 = &yw->_cells(v9, v7 - 1);
        }
        else if ( v27 )
        {
            v17 = &yw->_cells(v9 - 1, v7);
            v15 = &yw->_cells(v9 - 1, v7);
            v16 = &v12;
        }
        else
        {
            v15 = &yw->_cells(v9 - 1, v7);
            v16 = &yw->_cells(v9, v7 - 1);
            v17 = &yw->_cells(v9 - 1, v7 - 1);
        }


        if ( fabs(v17->height - v16->height) >= 500.0 )
            v13 = 1;

        if ( fabs(v15->height - v12.height) >= 500.0 )
            v13 |= 2;

        if ( fabs(v17->height - v15->height) >= 500.0 )
            v13 |= 4;

        if ( fabs(v16->height - v12.height) >= 500.0 )
            v13 |= 8;

        if ( v13 == 12 )
        {
            if ( v17->height <= v15->height )
                v13 = 0xFD;
            else
                v13 = 0xFC;
        }
        else if ( v13 == 3 )
        {
            if ( v17->height <= v16->height )
                v13 = 0xFF;
            else
                v13 = 0xFE;
        }

        int v22 = (v13 - 16) & 0xFF;
        return (v22 << 8) | v22;
    }
}

char * sb_0x4f6650__sub0(NC_STACK_ypaworld *yw, char *cmdbuf, int xpos, int ypos, int w, int h)
{
    float v7 = robo_map.field_1D8 / robo_map.field_1E0;
    float v8 = -robo_map.field_1DC / robo_map.field_1E4;
    int v9 = v7 - (w / 2);
    int v23 = w + v9;
    int v10 = v8 - (h / 2);
    int v22 = h + v10;

    char *pcur = sub_4F6114(yw, 4 << robo_map.field_1EE, cmdbuf, v9, v10, v23, v22, robo_map.field_1EE + 50, sub_4F60A4, xpos, ypos);

    if ( robo_map.field_1EC & 2 )
        pcur = sub_4F6114(yw, 4 << robo_map.field_1EE, pcur, v9, v10, v23, v22, robo_map.field_1EE + 50, sub_4F6048, xpos, ypos);

    if ( robo_map.field_1EC & 1 )
    {
        switch ( robo_map.field_1EE )
        {
        case 0:
            pcur = sub_4F6114(yw, 4, pcur, v9, v10, v23, v22, 43, sub_4F5FE0, xpos, ypos);
            break;

        case 1:
            pcur = sub_4F6114(yw, 8, pcur, v9, v10, v23, v22, 44, sub_4F5FE0, xpos, ypos);
            break;

        case 2:
            pcur = sub_4F6114(yw, 4, pcur, v9 + 2, v10 + 2, v23 + 2, v22 + 2, 42, sub_4F5CEC, xpos, ypos);
            break;

        case 3:
            pcur = sub_4F6114(yw, 8, pcur, v9 + 4, v10 + 4, v23 + 4, v22 + 4, 41, sub_4F5CEC, xpos, ypos);
            break;

        case 4:
            pcur = sub_4F6114(yw, 16, pcur, v9 + 8, v10 + 8, v23 + 8, v22 + 8, 40, sub_4F5CEC, xpos, ypos);
            break;
        }
    }
    return pcur;
}

char *sb_0x4f6650(NC_STACK_ypaworld *yw, char *cmdbuf, int x, int y)
{
    char *pcur = sb_0x4f6650__sub0(yw, cmdbuf, x, y, robo_map.w - robo_map.field_24C, robo_map.h - robo_map.field_250);
    FontUA::set_end(&pcur);
    return pcur;
}

void sub_4C1814(NC_STACK_ypaworld *yw, int a2, int a3)
{
    float v15 = yw->_mapSize.x * World::CVSectorLength;
    float v12 = yw->_mapSize.y * World::CVSectorLength;

    float v4 = a2 * robo_map.field_1E0;
    float v13 = v4 * 0.5;

    if ( v4 <= v15 )
    {
        if ( robo_map.field_1D8 - v13 >= 0.0 )
        {
            if ( robo_map.field_1D8 + v13 > v15 )
                robo_map.field_1D8 = v15 - v13;
        }
        else
        {
            robo_map.field_1D8 = v13;
        }
    }
    else
    {
        robo_map.field_1D8 = v15 * 0.5;
    }

    float v8 = a3 * robo_map.field_1E4;
    float v14 = v8 * 0.5;

    if ( v8 > v12 )
    {
        robo_map.field_1DC = -v12 * 0.5;
    }
    else
    {
        if ( robo_map.field_1DC + v14 > 0.0 )
        {
            robo_map.field_1DC = -v8 * 0.5;
        }
        else
        {
            if ( robo_map.field_1DC - v14 < -v12 )
                robo_map.field_1DC = -v12 + v14;
        }
    }
}

void MapRoboUpdateMapInt(NC_STACK_ypaworld *yw)
{
    robo_map.buttons[17] = ButtonBox(robo_map.field_244,
                                     robo_map.field_23C,
                                     robo_map.w - robo_map.field_24C,
                                     robo_map.h - robo_map.field_250);

    int v15 = robo_map.x + robo_map.field_244 - (yw->_screenSize.x / 2);
    int v14 = robo_map.y + robo_map.field_23C - (yw->_screenSize.y / 2);

    sb_0x4f6650(yw, t1_cmdbuf_3, v15, v14);
}

void  sb_0x451034__sub2(NC_STACK_ypaworld *yw)
{
    memset(byte_5BA6E8, 0, sizeof(byte_5BA6E8));
    memset(t1_cmdbuf_1, 0, sizeof(t1_cmdbuf_1));
    memset(t1_cmdbuf_2, 0, sizeof(t1_cmdbuf_2));
    memset(t1_cmdbuf_3, 0, sizeof(t1_cmdbuf_3));

    robo_map.flags = (GuiBase::FLAG_WITH_HELP | GuiBase::FLAG_CLOSED | GuiBase::FLAG_WITH_CLOSE | GuiBase::FLAG_WITH_DRAGBAR);
    robo_map.field_228 = 8;
    robo_map.field_22C = yw->_guiTiles[10]->map[65].w;
    robo_map.field_230 = yw->_guiTiles[10]->h;
    robo_map.field_234 = 4;
    robo_map.field_238 = yw->_guiTiles[0]->map[113].w;
    robo_map.field_23C = yw->_fontH;
    robo_map.field_240 = yw->_fontHScrollH;
    robo_map.field_244 = yw->_guiTiles[12]->map[65].w;
    robo_map.field_248 = yw->_fontVBScrollW;
    robo_map.field_24C = robo_map.field_248 + robo_map.field_244;
    robo_map.field_250 = robo_map.field_240 + robo_map.field_23C;
    robo_map.field_210 = robo_map.field_248 + 7 * robo_map.field_22C + robo_map.field_244 + 8;
    robo_map.field_212 = robo_map.field_240 + robo_map.field_23C + 96;
    robo_map.field_214 = 32000;
    robo_map.field_216 = 32000;
    robo_map.x = 0;
    robo_map.y = yw->_upScreenBorder;
    robo_map.w = 2 * yw->_screenSize.x / 3;
    robo_map.h = 2 * yw->_screenSize.y / 3;
    robo_map.field_1EE = 4;
    robo_map.field_1EC = 7;
    robo_map.buttons.resize(18);

    robo_map.cmdstrm.cmdbuf = byte_5BA6E8;
    robo_map.cmdstrm.includ = t1_cmdbufs;

    robo_map.postDraw = sb_0x4f8f64;

    if ( yw->_userRobo )
    {
        robo_map.field_1D8 = yw->_userRobo->_position.x;
        robo_map.field_1DC = yw->_userRobo->_position.z;
    }
    else
    {
        robo_map.field_1DC = -39000.0;
        robo_map.field_1D8 = 39000.0;
    }

    switch ( robo_map.field_1EE )
    {
    case 0:
        robo_map.field_1E4 = 300.0;
        robo_map.field_1E0 = 300.0;
        break;

    case 1:
        robo_map.field_1E4 = 150.0;
        robo_map.field_1E0 = 150.0;
        break;

    case 2:
        robo_map.field_1E4 = 75.0;
        robo_map.field_1E0 = 75.0;
        break;

    case 3:
        robo_map.field_1E4 = 37.5;
        robo_map.field_1E0 = 37.5;
        break;

    case 4:
        robo_map.field_1E4 = 18.75;
        robo_map.field_1E0 = 18.75;
        break;
    }

    robo_map.field_1ED = 0;
    robo_map.field_1E8 = 0;
    robo_map.MapViewMask = cellArea::ViewMask(1);

    robo_map.field_254 = 0.45;
    robo_map.field_258 = 0;
    robo_map.field_25C = 0.95;
    robo_map.field_260 = 0.6;

    robo_map.field_208 = yw->_screenSize.x - robo_map.field_210;
    robo_map.field_20A = yw->_screenSize.y - yw->_downScreenBorder - robo_map.field_212;
    robo_map.field_20C = robo_map.field_210;
    robo_map.field_20E = robo_map.field_212;

    sub_4C157C(yw);
    sub_4C0C00(yw);
    sub_4C0FEC(yw);
    sub_4C1214(yw);
    MapRoboUpdateMapInt(yw);
    sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
}


void sub_4C32A8(NC_STACK_ypaworld *yw, TMFWinStatus *status)
{
    if ( status->Valid )
    {
        if ( status->Rect.y < yw->_upScreenBorder
                || status->Rect.x + status->Rect.w > yw->_screenSize.x
                || status->Rect.y + status->Rect.h > yw->_screenSize.y - yw->_downScreenBorder )
        {
            status->Valid = false;
        }
    }

    if ( status->Valid )
    {
        if ( yw->_roboMapStatus.Data[4] < yw->_upScreenBorder
                || yw->_roboMapStatus.Data[3] + yw->_roboMapStatus.Data[5] > yw->_screenSize.x
                || yw->_roboMapStatus.Data[6] + yw->_roboMapStatus.Data[4] > yw->_screenSize.y - yw->_downScreenBorder )
            status->Valid = false;
    }
}

void sub_4C1970(NC_STACK_ypaworld *yw, int a2)
{
    if ( a2 == 1 )
        robo_map.field_1EE++;
    else if ( a2 == 2 )
        robo_map.field_1EE--;


    if ( robo_map.field_1EE > 4 )
        robo_map.field_1EE = 4;
    else if ( robo_map.field_1EE < 1 )
        robo_map.field_1EE = 1;

    switch ( robo_map.field_1EE )
    {
    case 0:
        robo_map.field_1E4 = 300.0;
        robo_map.field_1E0 = 300.0;
        break;

    case 1:
        robo_map.field_1E4 = 150.0;
        robo_map.field_1E0 = 150.0;
        break;

    case 2:
        robo_map.field_1E4 = 75.0;
        robo_map.field_1E0 = 75.0;
        break;

    case 3:
        robo_map.field_1E4 = 37.5;
        robo_map.field_1E0 = 37.5;
        break;

    case 4:
        robo_map.field_1E4 = 18.75;
        robo_map.field_1E0 = 18.75;
        break;
    }
}

int sb_0x451034__sub3(NC_STACK_ypaworld *yw)
{
    bzda = bzd();

    int v3 = yw->_screenSize.x - 11 * yw->_iconOrderW;
    bzda.field_8FC = v3 / 4;

    int v5;
    if ( yw->_screenSize.x >= 512 )
        v5 = 7 * yw->_guiTiles[0]->map[87].w;
    else
        v5 = 9 * yw->_guiTiles[0]->map[87].w;

    bzda.field_900 = 2 * v5;
    bzda.field_904 = bzda.field_8FC / 2;
    bzda.field_908 = yw->_iconOrderW + bzda.field_8FC + bzda.field_8FC / 2;
    bzda.field_90C = 2 * yw->_iconOrderW + bzda.field_908 + bzda.field_8FC;
    bzda.field_910 = 5 * yw->_iconOrderW + bzda.field_90C + bzda.field_8FC;
    bzda.field_914 = 1;
    bzda.field_918 = 0;
    bzda.flags = 0;
    bzda.buttons.resize(11);

    bzda.x = 0;
    bzda.y = yw->_screenSize.y - yw->_iconOrderH;
    bzda.w = yw->_screenSize.x;
    bzda.h = yw->_iconOrderH;

    for (int i = 0; i < 11; i++)
        bzda.buttons[i] = ButtonBox();

    bzda.cmdstrm.cmdbuf = byte_516534;
    bzda.field_1D8 = 0;

    sub_4C3A54(yw);

    bzda.field_8EC = 0;
    bzda.field_8F0 = 0;
    bzda.field_8F4 = 0;
    bzda.field_8F8 = 0;

    if ( bzda.field_1D4 & 1 )
    {
        if ( !(bzda.field_1D8 & 1) )
        {
            bzda.field_1D8 |= 1;
            Input::Engine.AddClickBoxFront(&bzda);
        }
    }

    GuiList::tInit args;
    args.resizeable = false;
    args.numEntries = 8;
    args.shownEntries = 8;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 16;
    args.withIcon = false;
    args.entryHeight = yw->_fontH;
    args.entryWidth = bzda.field_900;
    args.enabled = true;
    args.vborder = yw->_fontBorderH;
    args.instantInput = true;
    args.keyboardInput = true;

    if ( !gui_lstvw.Init(yw, args) )
        return 0;


    sub_4C32A8(yw, &yw->_roboMapStatus);
    if ( yw->_roboFinderStatus.Valid )
    {
        if ( yw->_roboFinderStatus.Rect.y < yw->_upScreenBorder
                || yw->_roboFinderStatus.Rect.x + yw->_roboFinderStatus.Rect.w > yw->_screenSize.x
                || yw->_roboFinderStatus.Rect.h + yw->_roboFinderStatus.Rect.y > yw->_screenSize.y - yw->_downScreenBorder )
        {
            yw->_roboFinderStatus.Valid = false;
        }
    }

    sub_4C32A8(yw, &yw->_vhclMapStatus);
    if ( yw->_vhclFinderStatus.Valid )
    {
        if ( yw->_vhclFinderStatus.Rect.y < yw->_upScreenBorder
                || yw->_vhclFinderStatus.Rect.w + yw->_vhclFinderStatus.Rect.x > yw->_screenSize.x
                || yw->_vhclFinderStatus.Rect.h + yw->_vhclFinderStatus.Rect.y > yw->_screenSize.y - yw->_downScreenBorder )
        {
            yw->_vhclFinderStatus.Valid = false;
        }
    }

    if ( !yw->_roboMapStatus.Data[5] || !yw->_roboMapStatus.Data[6] )
        yw->_roboMapStatus.Valid = false;

    if ( !yw->_vhclMapStatus.Data[5] || !yw->_vhclMapStatus.Data[6] )
        yw->_vhclMapStatus.Valid = false;

    if ( yw->_roboMapStatus.Valid )
    {
        if ( yw->_roboMapStatus.IsOpen )
        {
            yw->GuiWinOpen(&robo_map);
            yw->GuiWinToFront(&robo_map);
        }
        else
            yw->GuiWinClose(&robo_map);

        robo_map.x = yw->_roboMapStatus.Rect.x;
        robo_map.y = yw->_roboMapStatus.Rect.y;
        robo_map.w = yw->_roboMapStatus.Rect.w;
        robo_map.h = yw->_roboMapStatus.Rect.h;
        robo_map.field_1EC = yw->_roboMapStatus.Data[0];
        robo_map.field_1ED = yw->_roboMapStatus.Data[1];
        robo_map.field_1EE = yw->_roboMapStatus.Data[2];
        robo_map.field_208 = yw->_roboMapStatus.Data[3];
        robo_map.field_20A = yw->_roboMapStatus.Data[4];
        robo_map.field_20C = yw->_roboMapStatus.Data[5];
        robo_map.field_20E = yw->_roboMapStatus.Data[6];

        sub_4C1970(yw, 0);
    }


    if ( yw->_roboFinderStatus.Valid )
    {
        if ( yw->_roboFinderStatus.IsOpen )
        {
            yw->GuiWinOpen(&squadron_manager);
            yw->GuiWinToFront(&squadron_manager);
        }
        else
            yw->GuiWinClose(&squadron_manager);

        squadron_manager.x = yw->_roboFinderStatus.Rect.x;
        squadron_manager.y = yw->_roboFinderStatus.Rect.y;
        squadron_manager.w = yw->_roboFinderStatus.Rect.w;
        squadron_manager.h = yw->_roboFinderStatus.Rect.h;

        squadron_manager.SetRect(yw, -2, -2);
    }

    return 1;
}

void NC_STACK_ypaworld::FFeedback_Init()
{
    _ffTimeStamp = 0;
    _ffEffectType = -1;
    _ffPeriod = 0;
    _ffMagnitude = 0;
}



void create_info_log(NC_STACK_ypaworld *yw)
{
    info_log = yw_infolog();

    GuiList::tInit args;
    args.title = Locale::Text::WinName(Locale::WINNAME_LOG);
    args.resizeable = true;
    args.numEntries = 1;
    args.shownEntries = 6;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 24;
    args.minShownEntries = 3;
    args.withIcon = false;
    args.entryHeight = yw->_fontH;
    args.entryWidth = 200;
    args.maxEntryWidth = 32000;
    args.minEntryWidth = 2 * (5 * yw->_guiTiles[0]->map[48].w);
    args.enabled = true;
    args.vborder = yw->_fontBorderH;
    args.closeChar = 74;

    if ( info_log.Init(yw, args) )
    {
        if ( yw->_timeStamp )
        {
            std::string tmp = fmt::sprintf("<%02d:%02d:%02d>", (yw->_timeStamp >> 10) / 60 / 60 % 24, (yw->_timeStamp >> 10) / 60 % 60, (yw->_timeStamp >> 10) % 60);
            std::string tmp2 = Locale::Text::Common(Locale::CMN_MISNCONT);
            sprintf(info_log.msgs[0].txt, tmp2.c_str(), tmp.c_str());
        }
        else
        {
            std::string tmp = Locale::Text::Common(Locale::CMN_WELCOME);
            strcpy(info_log.msgs[0].txt, tmp.c_str());
        }

        info_log.msgs[0].field_4 = yw->_timeStamp + 1;
        info_log.field_255C = 0;
        info_log.field_2560 = yw->_timeStamp;
        info_log.field_2564 = 0;
        info_log.field_250 = 127;
        info_log.field_248 = 6 * yw->_guiTiles[0]->map[48].w + 2 * yw->_guiTiles[0]->map[58].w + 12;
    }
}




void create_exit_menu(NC_STACK_ypaworld *yw)
{
    dword_5C8B7C = (yw->_fontH / 2) + yw->_fontH;
    dword_5C8B84 = yw->_fontH / 4;

    int tmp = yw->_guiTiles[0]->GetWidth("WWWWWWW");

    GuiList::tInit args;
    args.title = Locale::Text::WinName(Locale::WINNAME_EXIT);
    args.resizeable = false;
    args.numEntries = 5;
    args.shownEntries = 5;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 5;
    args.minShownEntries = 5;
    args.withIcon = false;
    args.entryHeight = dword_5C8B7C;
    args.entryWidth = (2 * tmp + 16);
    args.enabled = true;
    args.vborder = yw->_fontBorderH;
    args.staticItems = true;
    args.closeChar = 85;

    if ( exit_menu.Init(yw, args) )
    {
        int v5 = yw->_fontH + exit_menu.upperVborder + dword_5C8B84;

        dword_5C8B80 = 2 * yw->_guiTiles[0]->map[32].w + yw->_fontBorderW;
        dword_5C8B88 = exit_menu.entryWidth - 2 * dword_5C8B80;

        for (int i = 8; i < 13; i++)
        {
            exit_menu.buttons[i] = ButtonBox(dword_5C8B80, (i - 8) * dword_5C8B7C + v5,
                                             dword_5C8B88, yw->_fontH);
        }
    }
}


void sb_0x451034__sub5(NC_STACK_ypaworld *yw)
{
    int v2 = yw->_guiTiles[0]->GetWidth("WWWWWWW") * 3.5 + 16;

    GuiList::tInit args;
    args.title = " ";
    args.resizeable = false;
    args.numEntries = 3;
    args.shownEntries = 3;
    args.firstShownEntry = 0;
    args.selectedEntry = 0;
    args.maxShownEntries = 3;
    args.minShownEntries = 3;
    args.withIcon = false;
    args.entryHeight = yw->_fontH;
    args.entryWidth = v2;
    args.enabled = true;
    args.vborder = yw->_fontBorderH;
    args.staticItems = true;
    args.closeChar = 85;
    args.withHelp = false;

    if ( lstvw2.Init(yw, args) )
    {
        int v1 = yw->_fontH + lstvw2.upperVborder + 2 * lstvw2.entryHeight;

        dword_5BAFA8 = 2 * yw->_guiTiles[0]->map[32].w + yw->_fontBorderW;
        dword_5BAFA0 = (lstvw2.entryWidth - 2 * dword_5BAFA8) / 3;

        lstvw2.buttons.resize(10);

        lstvw2.buttons[8] = ButtonBox(dword_5BAFA8, v1, dword_5BAFA0, lstvw2.entryHeight);

        lstvw2.buttons[9] = ButtonBox(  lstvw2.entryWidth - dword_5BAFA8 - dword_5BAFA0,
                                        v1,
                                        dword_5BAFA0,
                                        lstvw2.entryHeight);

        dword_5BAFA4 = lstvw2.buttons[9].x - (dword_5BAFA0 + lstvw2.buttons[8].x);

        dword_5BAF9C = 0;
    }
}


void sb_0x451034__sub9(NC_STACK_ypaworld *yw)
{
    const std::array<std::string, 14> off_51825C
    {
        "wireless/bar.sklt",
        "wireless/compass.sklt",
        "wireless/frame.sklt",
        "wireless/arrow.sklt",
        "wireless/mgvis.sklt",
        "wireless/grenvis1.sklt",
        "wireless/rockvis1.sklt",
        "wireless/missvis1.sklt",
        "wireless/bombvis1.sklt",
        "wireless/grenvis2.sklt",
        "wireless/rockvis2.sklt",
        "wireless/missvis2.sklt",
        "wireless/bombvis2.sklt",
        "wireless/triangle.sklt"
    };

    sklt_wis *wis = &yw->_hud;

    *wis = sklt_wis();

    wis->field_0 = 1;

    Common::Env.SetPrefix("rsrc", "data:");

    for (int i = 0; i < 14; i++)
    {
        wis->sklts[i] = Nucleus::CInit<NC_STACK_sklt>({{NC_STACK_rsrc::RSRC_ATT_NAME, off_51825C.at(i)}});

        if ( wis->sklts[i] )
            wis->sklts_intern[i] = wis->sklts[i]->GetSkelet();
    }

    float CW, CH;
    GFX::Engine.getAspectCorrection(CW, CH, false);

    if ( yw->_screenSize.x >= 512 )
    {
        wis->field_9E = 8;
        wis->field_96 = 28;
        wis->field_92 = (float)yw->_guiTiles[0]->h / ((float)yw->_screenSize.y * CH);
        wis->field_8E = wis->field_92 * 14.0;
        wis->field_9A = 28;
    }
    else
    {
        wis->field_9E = 6;
        wis->field_96 = 18;
        wis->field_92 = (float)yw->_guiTiles[0]->h * 1.5 / ((float)yw->_screenSize.y * CH);
        wis->field_8E = wis->field_92 * 18.0;
        wis->field_9A = 18;
    }

    wis->field_72 = 0;
    wis->field_8A = (float)(yw->_guiTiles[51]->map[1].w * wis->field_9E + wis->field_9A + wis->field_96) / ((float)yw->_screenSize.x * CW);

    wis->field_72 = 1;
}

void sub_4DA874(NC_STACK_ypaworld *yw)
{
    exit_menu.Free();
}

void sub_46E16C(NC_STACK_ypaworld *yw)
{
    info_log.Free();
}

void sub_4C706C(NC_STACK_ypaworld *yw)
{
    squadron_manager.Free();
}

void sub_4C39A4(NC_STACK_ypaworld *yw)
{
    yw->GuiWinClose( &gui_lstvw );

    if ( bzda.field_1D8 & 1 )
    {
        bzda.field_1D8 &= 0xFFFFFFFE;
        Input::Engine.RemClickBox(&bzda);
    }

    gui_lstvw.Free();
}

void sub_4E1D24()
{
    Input::Engine.RemClickBox(&up_panel);
}

void sub_4E2B24(NC_STACK_ypaworld *yw)
{
    for (int i = 0; i < 14; i++)
    {
        if (yw->_hud.sklts[i])
        {
            yw->_hud.sklts[i]->Delete();
            yw->_hud.sklts[i] = NULL;
            yw->_hud.sklts_intern[i] = NULL;
        }
    }
}

void sub_4C8524(NC_STACK_ypaworld *yw)
{
    lstvw2.Free();
}

void NC_STACK_ypaworld::FFeedback_StopAll()
{
    Input::Engine.ForceFeedback(Input::FF_STATE_STOP, Input::FF_TYPE_ALL);
}

int sb_0x451034(NC_STACK_ypaworld *yw)
{
    if ( yw->_guiLoaded )
        return yw->_guiLoaded;

    yw->_guiDragging = false;
    yw->_guiDragElement = NULL;
    yw->_guiDragDefaultMouse = false;
    yw->_mouseCursorHidden = false;
    yw->_prevMousePos = Common::Point();
    yw->_bactPrevClicked = NULL;

    sb_0x451034__sub9(yw);
    sb_0x451034__sub8(yw);
    sb_0x451034__sub2(yw);
    create_squad_man(yw);
    create_info_log(yw);
    create_exit_menu(yw);
    sb_0x451034__sub5(yw);
    sb_0x451034__sub6(yw);

    yw->ypaworld_func139(&robo_map);
    yw->ypaworld_func139(&squadron_manager);
    yw->ypaworld_func139(&info_log);
    yw->ypaworld_func139(&exit_menu);
    yw->ypaworld_func139(&lstvw2);
    yw->ypaworld_func139(&netgame_wnd);

    if ( !sb_0x451034__sub3(yw) )
    {
        yw->ypaworld_func140(&lstvw2);
        yw->ypaworld_func140(&exit_menu);
        yw->ypaworld_func140(&info_log);
        yw->ypaworld_func140(&squadron_manager);
        yw->ypaworld_func140(&robo_map);
        yw->ypaworld_func140(&netgame_wnd);

        sub_4DA874(yw);
        sub_46E16C(yw);
        sub_4C706C(yw);
        return 0;
    }

    yw->_guiLoaded = true;

    if ( yw->_preferences & World::PREF_CDMUSICDISABLE )
    {
        SFXEngine::SFXe.SetMusicTrack(yw->_levelInfo.MusicTrack, yw->_levelInfo.MusicTrackMinDelay, yw->_levelInfo.MusicTrackMaxDelay);
        SFXEngine::SFXe.PlayMusicTrack();
    }

    return 1;
}


void sb_0x4d7c08__sub0__sub1()
{
    if ( bzda.IsOpen() )
    {
        w3d_a209 v0;
        v0 = bzda.cmdstrm;

        GFX::Engine.DrawText(&v0);

        if ( gui_lstvw.IsOpen() )
        {
            v0 = gui_lstvw.cmdstrm;

            GFX::Engine.DrawText(&v0);
        }
    }
}

void sb_0x4d7c08__sub0__sub3(NC_STACK_ypaworld *yw)
{
    w3d_a209 v4 = up_panel.cmdstrm;
    GFX::Engine.raster_func209(&v4);
}

void sb_0x4d7c08__sub0(NC_STACK_ypaworld *yw)
{
    if ( yw->_userUnit->_bact_type != BACT_TYPES_MISSLE )
    {
        sb_0x4d7c08__sub0__sub4(yw);

        for(GuiBaseList::iterator it = yw->_guiActive.begin(); it != yw->_guiActive.end(); it++)
        {
            GuiBase *lstnode = *it;
            if ( !(lstnode->flags & 0x20) )
            {
                int v6 = 0;
                w3d_a209 v8;

                if ( lstnode->flags & GuiBase::FLAG_ICONIFED )
                {
                    v8.includ = NULL;
                    v8.cmdbuf = lstnode->iconString;
                }
                else
                {
                    v8 = lstnode->cmdstrm;

                    if ( lstnode->postDraw )
                        v6 = 1;
                }

                GFX::Engine.DrawText(&v8);

                if ( v6 )
                    lstnode->postDraw(yw);
            }
        }

        if ( yw->_userUnit->_status != BACT_STATUS_DEAD )
        {
            sb_0x4d7c08__sub0__sub0(yw);
            sb_0x4d7c08__sub0__sub2(yw);
        }
        draw_tooltip(yw);
        sb_0x4d7c08__sub0__sub3(yw);
        sb_0x4d7c08__sub0__sub1();
    }
}


char * buy_list_update_sub(NC_STACK_ypaworld *yw, int a2, GuiList *lstvw, char *cur, char a5, const std::string &a6, int a7)
{
    int v33 = lstvw->entryWidth - 2 * yw->_fontBorderW;

    FontUA::ColumnItem v24[3];

    char *pcur = cur;

    int v14;
    int v15;
    int v16;
    int v17;

    if ( a2 )
    {
        FontUA::set_txtColor(&pcur, yw->_iniColors[62].r, yw->_iniColors[62].g, yw->_iniColors[62].b);

        v14 = 9;
        v15 = 98;
        v16 = 99;
        v17 = 100;
    }
    else
    {
        FontUA::set_txtColor(&pcur, yw->_iniColors[61].r, yw->_iniColors[61].g, yw->_iniColors[61].b);

        v14 = 0;
        v16 = 102;
        v15 = 102;
        v17 = 102;
    }

    int v19 = yw->_guiTiles[v14]->map[48].w;

    v24[0].txt = a5;
    v24[0].fontID = 28;
    v24[0].width = squadron_manager.field_2CC;
    v24[0].prefixChar = 0;
    v24[0].postfixChar = 0;
    v24[0].spaceChar = 64;
    v24[0].flags = 4;

    v24[1].fontID = v14;
    v24[1].spaceChar = v16;
    v24[1].prefixChar = v15;
    v24[1].txt = a6;
    v24[1].flags = 37;
    v24[1].width = v33 - squadron_manager.field_2CC - 5 * v19;
    v24[1].postfixChar = 0;

    v24[2].txt = fmt::sprintf("%d", a7);
    v24[2].fontID = v14;
    v24[2].spaceChar = v16;
    v24[2].postfixChar = v17;
    v24[2].width = 5 * v19;
    v24[2].flags = 42;
    v24[2].prefixChar = 0;

    FontUA::select_tileset(&pcur, 0);

    FontUA::store_u8(&pcur, 123);

    FontUA::select_tileset(&pcur, v14);

    FontUA::op10(&pcur, squadron_manager.field_2CC);

    FontUA::store_u8(&pcur, v16);

    FontUA::add_xpos(&pcur, -squadron_manager.field_2CC);

    pcur = FormateColumnItem(yw, pcur, 3, v24);

    FontUA::select_tileset(&pcur, 0);

    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    return pcur;
}

float NC_STACK_ypaworld::sub_4498F4()
{
    float v3 = 1.0;

    if ( _isNetGame )
    {
        if ( _levelUnitLimitType == 2 )
        {
            int v1 = _countUnitsPerOwner[_userRobo->_owner] - _levelUnitLimit;
            if ( v1 > 0 )
                v3 = (_levelUnitLimitArg * v1) * 0.01 + 1.0;
        }
    }

    return v3;
}

char * gui_update_create_btn__sub0(NC_STACK_ypaworld *yw)
{
    char *pcur = gui_lstvw.itemBlock;

    Input::Engine.RemClickBox(&gui_lstvw);
    Input::Engine.AddClickBoxFront(&gui_lstvw);

    gui_lstvw.numEntries = bzda.field_8E8;
    gui_lstvw.selectedEntry = bzda.field_8F8;

    if ( gui_lstvw.maxShownEntries >= bzda.field_8E8 )
        gui_lstvw.shownEntries = bzda.field_8E8;
    else
        gui_lstvw.shownEntries = gui_lstvw.maxShownEntries;

    if ( gui_lstvw.firstShownEntries + gui_lstvw.shownEntries > gui_lstvw.numEntries )
        gui_lstvw.firstShownEntries = gui_lstvw.numEntries - gui_lstvw.shownEntries;

    gui_lstvw.SetRect(yw, -2, -2);

    gui_lstvw.x = bzda.field_904 + bzda.x;
    gui_lstvw.y = bzda.field_918 + bzda.y - gui_lstvw.h;

    pcur = gui_lstvw.ItemsPreLayout(yw, pcur, 0, "uvw");

    for (int i = 0; i < gui_lstvw.shownEntries; i++ )
    {
        int v21 = 0;
        int v3 = i + gui_lstvw.firstShownEntries;

        if ( bzda.field_4DC[ v3 ].i == 1 )
        {
            int v5 = bzda.field_4DC[ v3 ].d;

            const World::TVhclProto &proto = yw->_vhclProtos[v5];

            int v17 = dround(yw->sub_4498F4() * 2 * proto.energy / 100.0);

            const std::string v8 = yw->GetVehicleName(proto);

            if ( v3 == gui_lstvw.selectedEntry )
                v21 = 1;

            pcur = buy_list_update_sub(yw, v21, &gui_lstvw, pcur, proto.type_icon, v8, v17);
        }
        else if ( bzda.field_4DC[ v3 ].i == 2 )
        {
            int v9 = bzda.field_4DC[ v3 ].d;
            const World::TBuildingProto &v10 = yw->_buildProtos[v9];

            int v18 = dround(yw->sub_4498F4() * v10.Energy / 100.0);
            
            std::string v13;
            if ( yw->_isNetGame )
                v13 = yw->GetBuildingName(v10, true);
            else
                v13 = yw->GetBuildingName(v10, false);
                
            if ( v3 == gui_lstvw.selectedEntry )
                v21 = 1;

            pcur = buy_list_update_sub(yw, v21, &gui_lstvw, pcur, v10.TypeIcon, v13, v18);
        }
    }

    pcur = gui_lstvw.ItemsPostLayout(yw, pcur, 0, "xyz");

    FontUA::set_end(&pcur);

    return pcur;
}


char * gui_update_create_btn(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
    {
        bzda.buttons[0] = ButtonBox(); //create_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 67);

        yw->GuiWinClose( &gui_lstvw );
    }
    else
    {
        bzda.buttons[0] = ButtonBox(bzda.field_904, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH);

        if ( !(bzda.field_1CC & 0x16) )
        {
            bzda.buttons[0] = ButtonBox(); //create_btn

            FontUA::select_tileset(&pcur, 23);
            FontUA::store_u8(&pcur, 67);
        }
        else
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                if ( gui_lstvw.IsOpen() )
                    gui_update_create_btn__sub0(yw);

                FontUA::select_tileset(&pcur, 22);
            }
            else
            {
                FontUA::select_tileset(&pcur, 21);
            }

            int v9 = bzda.field_1D0 & 0x16;

            if (v9 == 0x10)
                FontUA::store_u8(&pcur, 70);
            else if (v9 == 4)
                FontUA::store_u8(&pcur, 68);
            else if (v9 == 2)
                FontUA::store_u8(&pcur, 67);
            else
                FontUA::store_u8(&pcur, 67);
        }
    }
    return pcur;
}

char *gui_update_map_squad_btn(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
    {
        FontUA::select_tileset(&pcur, 23);

        FontUA::store_u8(&pcur, 72);
        FontUA::store_u8(&pcur, 73);

        bzda.buttons[2] = ButtonBox(); //map_btn
        bzda.buttons[3] = ButtonBox(); //squad_btn

        yw->GuiWinClose( &robo_map );
        yw->GuiWinClose( &squadron_manager );
        yw->GuiWinClose( &info_log );
    }
    else
    {
        int v8 = bzda.field_908;

        for (int i = 2; i < 4; i++)
        {
            bzda.buttons[i] = ButtonBox(v8, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH);

            v8 += yw->_iconOrderW;
        }

        if ( robo_map.IsClosed() )
            FontUA::select_tileset(&pcur, 21);
        else
            FontUA::select_tileset(&pcur, 22);

        FontUA::store_u8(&pcur, 72);

        if ( squadron_manager.IsClosed() )
            FontUA::select_tileset(&pcur, 21);
        else
            FontUA::select_tileset(&pcur, 22);

        FontUA::store_u8(&pcur, 73);
    }
    return pcur;
}

char *gui_update_player_panel(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    int v35 = 0;

    if ( yw->_userRobo == yw->_userUnit )
    {
        if ( !yw->_cmdrsRemap.empty() )
            v35 = 0x80;

        if ( bzda.field_8DC )
            v35 |= 0x40;
    }
    else if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
    {
        v35 = 0x90;
    }
    else if ( yw->_playerInHSGun )
    {
        v35 = 0x90;

        if ( bzda.field_8DC > 1 )
            v35 = 0xD0;
    }
    else if ( yw->_userUnit->IsParentMyRobo() )
    {
        v35 = 0x10;

        if ( yw->_userUnit->_bact_type == BACT_TYPES_GUN )
        {
            if ( !yw->_cmdrsRemap.empty() )
                v35 = 0x90;
        }
        else if ( yw->_cmdrsRemap.size() > 1 )
            v35 = 0x90;

        if ( !yw->_userUnit->_kidList.empty() )
            v35 |= 0x40;
    }
    else
    {
        v35 = 0x70;

        if ( yw->_userUnit->_bact_type == BACT_TYPES_GUN )
        {
            if ( !yw->_cmdrsRemap.empty() )
                v35 = 0xF0;
        }
        else if ( yw->_cmdrsRemap.size() > 1 )
            v35 = 0xF0;
    }

    if ( yw->_userRobo->_status == BACT_STATUS_DEAD )
        v35 = 0;

    if ( bzda.field_1CC & 8 )
    {
        bzda.buttons[1] = ButtonBox(bzda.field_90C, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //into_vhcl_btn

        if ( bzda.field_1D0 & 8 )
            FontUA::select_tileset(&pcur, 22);
        else
            FontUA::select_tileset(&pcur, 21);

        FontUA::store_u8(&pcur, 69);
    }
    else
    {
        bzda.buttons[1] = ButtonBox();

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 69);
    }


    if ( v35 & 0x10 )
    {
        bzda.buttons[4] = ButtonBox(yw->_iconOrderW + bzda.field_90C, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //to_host_btn

        if ( bzda.field_91C & 0x10 )
            FontUA::select_tileset(&pcur, 22);
        else
            FontUA::select_tileset(&pcur, 21);

        FontUA::store_u8(&pcur, 81);
    }
    else
    {
        bzda.buttons[4] = ButtonBox(); //to_host_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 81);
    }

    if ( v35 & 0x20 )
    {
        bzda.buttons[5] = ButtonBox(bzda.field_90C + 2 * yw->_iconOrderW, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //to_leader_btn

        if ( bzda.field_91C & 0x20 )
            FontUA::select_tileset(&pcur, 22);
        else
            FontUA::select_tileset(&pcur, 21);

        FontUA::store_u8(&pcur, 82);
    }
    else
    {
        bzda.buttons[5] = ButtonBox(); // to_leader_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 82);
    }

    if ( v35 & 0x40 )
    {
        if ( !bzda.field_8DC || (yw->_userRobo != yw->_userUnit && !yw->_playerInHSGun) )
        {
            bzda.buttons[6] = ButtonBox(bzda.field_90C + 3 * yw->_iconOrderW, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //turrets_btn

            if ( bzda.field_91C & 0x40 )
                FontUA::select_tileset(&pcur, 22);
            else
                FontUA::select_tileset(&pcur, 21);

            FontUA::store_u8(&pcur, 83);
        }
        else
        {
            bzda.buttons[6] = ButtonBox(bzda.field_90C + 3 * yw->_iconOrderW, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //turrets_btn

            if ( bzda.field_91C & 0x40 )
                FontUA::select_tileset(&pcur, 22);
            else
                FontUA::select_tileset(&pcur, 21);

            FontUA::store_u8(&pcur, 66);
        }
    }
    else
    {
        bzda.buttons[6] = ButtonBox(); // turrets_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 83);
    }

    if ( v35 & 0x80 )
    {
        bzda.buttons[7] = ButtonBox(4 * yw->_iconOrderW + bzda.field_90C, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); //next_squad_btn

        if ( bzda.field_91C & 0x80 )
            FontUA::select_tileset(&pcur, 22);
        else
            FontUA::select_tileset(&pcur, 21);

        FontUA::store_u8(&pcur, 84);
    }
    else
    {
        bzda.buttons[7] = ButtonBox(); //next_squad_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 84);
    }
    return pcur;
}

char *gui_update_tools(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
    {
        bzda.buttons[8] = ButtonBox(); // analyzer_btn

        FontUA::select_tileset(&pcur, 23);
        FontUA::store_u8(&pcur, 63);
    }
    else
    {
        bzda.buttons[8] = ButtonBox(bzda.field_910, bzda.field_918, yw->_iconOrderW, yw->_iconOrderH); // analyzer_btn

        if ( bzda.field_91C & 0x100 )
            FontUA::select_tileset(&pcur, 22);
        else
            FontUA::select_tileset(&pcur, 21);

        FontUA::store_u8(&pcur, 63);
    }

    bzda.buttons[9] = ButtonBox(yw->_iconOrderW + bzda.field_910, bzda.field_918, yw->_iconOrderH, yw->_iconOrderW); // help_btn

    if ( bzda.field_91C & 0x200 )
        FontUA::select_tileset(&pcur, 22);
    else
        FontUA::select_tileset(&pcur, 21);

    FontUA::store_u8(&pcur, 76);

    bzda.buttons[10] = ButtonBox(bzda.field_910 + 2 * yw->_iconOrderW,  bzda.field_918,  yw->_iconOrderW,  yw->_iconOrderH); // menu_btn

    if ( bzda.field_91C & 0x400 )
        FontUA::select_tileset(&pcur, 22);
    else
        FontUA::select_tileset(&pcur, 21);

    FontUA::store_u8(&pcur, 85);

    return pcur;
}

char * sub_449970(NC_STACK_ypaworld *yw, char *cur, int a4, int a3, const std::string &a5, int a6, int a7)
{
    char *pcur = cur;

    int v10, v22;

    if ( yw->_screenSize.x >= 512 )
    {
        v22 = a7 - 72;
        v10 = a4 + a7 - 72 + 16;
    }
    else
    {
        v22 = a7 - 56;
        v10 = a4 + a7 - 56 + 8;
    }

    FontUA::select_tileset(&pcur, 15);

    FontUA::set_xpos(&pcur, a4);
    FontUA::set_ypos(&pcur, a3);

    int v14 = yw->_fontH - yw->_guiTiles[51]->h;

    FontUA::ColumnItem arg;
    arg.txt = a5;
    arg.width = v22;
    arg.fontID = 15;
    arg.prefixChar = 0;
    arg.spaceChar = 32;
    arg.postfixChar = 0;
    arg.flags = 40;

    pcur = FormateTextAlignedClippedString(yw, pcur, &arg);

    FontUA::select_tileset(&pcur, 51);

    FontUA::set_xpos(&pcur, v10);
    FontUA::set_ypos(&pcur, (v14 / 2) + a3);

    for (int i = 0; i < a6; i++)
        FontUA::store_u8(&pcur, yw->_userRobo->_owner);

    return pcur;
}

char *ypaworld_func64__sub7__sub2__sub1__sub0(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( bzda.field_1D0 & 6 )
    {
        if ( bzda.field_8EC != -1 && !yw->_playerInHSGun )
        {
            World::TVhclProto *v4 = &yw->_vhclProtos[ bzda.field_2DC[ bzda.field_8EC ] ];
            int a6 = v4->job_fighttank / 2;
            int v26 = v4->job_fightflyer / 2;
            int v25 = v4->job_fighthelicopter / 2;
            int v28 = v4->job_conquer / 2;
            int v5 = v4->job_fightrobo / 2;
            int v29 = v4->job_reconnoitre / 2;

            if ( !a6 )
                a6 = 1;

            if ( !v5 )
                v5 = 1;

            if ( !v26 )
                v26 = 1;

            if ( !v25 )
                v25 = 1;

            if ( !v28 )
                v28 = 1;

            if ( !v29 )
                v29 = 1;

            int v29_4 = 4 * yw->_screenSize.x / 7;
            int v6 = yw->_screenSize.x - v29_4;

            int v30 = -(yw->_downScreenBorder + 7 * yw->_fontH);

            FontUA::set_txtColor(&pcur, yw->_iniColors[63].r, yw->_iniColors[63].g, yw->_iniColors[63].b);

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_VSROBO), v5, v6);

            v30 += yw->_fontH;

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_VSTANK), a6, v6);

            v30 += yw->_fontH;

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_VSPLANE), v26, v6);

            v30 += yw->_fontH;

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_VSHELI), v25, v6);

            v30 += yw->_fontH;

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_CAPTURE), v28, v6);

            v30 += yw->_fontH;

            pcur = sub_449970(yw, pcur, v29_4, v30,  Locale::Text::Advanced(Locale::ADV_RECON), v29, v6);
        }
    }
    return pcur;
}


void ypaworld_func64__sub7__sub2__sub1(NC_STACK_ypaworld *yw)
{
    sub_4C3A54(yw);

    char *pcur = byte_516534;

    if ( bzda.field_1D4 & 1 )
    {
        int v20 = bzda.x - (yw->_screenSize.x / 2);
        int v21 = bzda.y - (yw->_screenSize.y / 2);

        if ( !(bzda.field_1D8 & 1) )
        {
            bzda.field_1D8 |= 1;
            Input::Engine.AddClickBoxFront(&bzda);
        }

        Input::Engine.RemClickBox(&bzda);
        Input::Engine.AddClickBoxFront(&bzda);

        FontUA::select_tileset(&pcur, 21);
        FontUA::set_center_xpos(&pcur, v20);
        FontUA::set_center_ypos(&pcur, v21);


        if ( bzda.field_904 > 0 )
            FontUA::add_xpos(&pcur, bzda.field_904);

        pcur = gui_update_create_btn(yw, pcur);

        if ( bzda.field_8FC > 0 )
            FontUA::add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_map_squad_btn(yw, pcur);

        if ( bzda.field_8FC > 0 )
            FontUA::add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_player_panel(yw, pcur);

        if ( bzda.field_8FC > 0 )
            FontUA::add_xpos(&pcur, bzda.field_8FC);

        pcur = gui_update_tools(yw, pcur);

        if ( gui_lstvw.IsOpen() )
            pcur = ypaworld_func64__sub7__sub2__sub1__sub0(yw, pcur);
    }

    FontUA::set_end(&pcur);

    if ( gui_lstvw.IsOpen() )
        gui_lstvw.Formate(yw);
}


NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub6(NC_STACK_ypaworld *yw)
{
    if ( !yw->_prevUnitId )
        return NULL;

    if ( yw->_prevUnitId == yw->_userRobo->_gid )
        return yw->_userRobo;

    for( NC_STACK_ypabact* &comm : yw->_userRobo->_kidList )
    {
        if ( comm->_status != BACT_STATUS_DEAD && comm->_status != BACT_STATUS_CREATE && comm->_status != BACT_STATUS_BEAM )
        {
            if ( yw->_prevUnitId == comm->_gid )
                return comm;

            for( NC_STACK_ypabact* &unit : comm->_kidList )
            {
                if ( unit->_status != BACT_STATUS_DEAD && unit->_status != BACT_STATUS_CREATE && unit->_status != BACT_STATUS_BEAM && yw->_prevUnitId == unit->_gid )
                    return unit;
            }
        }
    }

    return NULL;
}

void ypaworld_func64__sub7__sub2__sub7(NC_STACK_ypaworld *yw)
{
    if ( !yw->_userUnit->_parent || yw->_userUnit->_parent != yw->_userRobo )
    {
        if ( yw->_userUnit->_bact_type != BACT_TYPES_ROBO && yw->_userUnit->_bact_type != BACT_TYPES_MISSLE && yw->_userUnit->_bact_type != BACT_TYPES_GUN )
        {
            bact_arg109 v5;
            v5.field_0 = 2;
            v5.field_4 = yw->_userUnit->_parent;

            yw->_userUnit->ReorganizeGroup(&v5);
        }
    }
}

int ypaworld_func64__sub7__sub2__sub3(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    TClickBoxInf *winpt = &inpt->ClickInf;

    switch ( inpt->HotKeyID )
    {
    case 0:
        if ( bzda.field_1CC & 1 )
            bzda.field_1D0 = 1;
        break;

    case 1:
        if ( yw->_userUnit == yw->_userRobo )
        {
            if ( bzda.buttons[6] ) //turrets_btn
            {
                winpt->selected_btnID = 6;
                winpt->selected_btn = &bzda;
                winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
            }
        }
        break;

    case 2:
        if ( bzda.field_1CC & 2 )
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                bzda.field_1D0 = bzda.field_1CC & 1;
            }
            else
            {
                if ( !(bzda.field_1D0 & 2) || gui_lstvw.IsClosed() )
                {
                    yw->GuiWinOpen( &gui_lstvw );
                    gui_lstvw.listFlags |= (GuiList::GLIST_FLAG_IN_SELECT | GuiList::GLIST_FLAG_NO_SCROLL);
                }
                else
                {
                    yw->GuiWinClose( &gui_lstvw );
                }

                if ( bzda.field_8E0 <= bzda.field_8F8 )
                    bzda.field_1D0 = 16;
                else
                    bzda.field_1D0 = 2;

                return 0;
            }
        }
        break;

    case 3:
        if ( bzda.field_1CC & 4 )
        {
            if ( bzda.field_1D0 & 0x16 )
            {
                bzda.field_1D0 = bzda.field_1CC & 1;
            }
            else
            {
                if ( !(bzda.field_1D0 & 4) || gui_lstvw.IsClosed() )
                {
                    yw->GuiWinOpen( &gui_lstvw );
                    gui_lstvw.listFlags |= (GuiList::GLIST_FLAG_IN_SELECT | GuiList::GLIST_FLAG_NO_SCROLL);
                }
                else
                {
                    yw->GuiWinClose( &gui_lstvw );
                }

                bzda.field_1D0 = 4;
                return 0;
            }
        }
        break;

    case 4:
        if ( bzda.field_1CC & 8 )
        {
            winpt->selected_btn = &bzda;
            winpt->selected_btnID = 1;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
        }
        break;

    case 7:
        if ( bzda.field_1CC & 0x20 )
        {
            if ( bzda.field_1D0 & 0x20 )
                bzda.field_1D0 = bzda.field_1CC & 1;
            else
                bzda.field_1D0 = 32;
        }
        break;

    case 8:
        if ( yw->_userUnit->_status != BACT_STATUS_DEAD )
        {
            winpt->selected_btnID = 2;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
        }
        break;

    case 9:
        if ( yw->_userUnit->_status != BACT_STATUS_DEAD )
        {
            winpt->selected_btnID = 3;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
        }
        break;

    case 20:
        if ( bzda.buttons[7] ) //next_squad_btn
        {
            winpt->selected_btnID = 7;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
        }
        break;

    case 21:
        if ( bzda.buttons[4] ) //to_host_btn
        {
            winpt->selected_btnID = 4;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
        }
        break;

    case 22:
        if ( bzda.buttons[6] ) //turrets_btn
        {
            winpt->selected_btnID = 6;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
        }
        break;

    case 23:
        if ( bzda.buttons[5] ) //to_leader_btn
        {
            winpt->selected_btnID = 5;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
        }
        break;

    case 24:
        if ( yw->_userRobo->_status == BACT_STATUS_DEAD || bzda.field_1D0 & 1 )
        {
            if ( yw->_userRobo == yw->_userUnit || yw->_userRobo->_status == BACT_STATUS_DEAD )
            {
                if ( bzda.buttons[10] ) //menu_btn
                {
                    winpt->selected_btnID = 10;
                    winpt->selected_btn = &bzda;
                    winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
                }
            }
            else
            {
                winpt->selected_btnID = 4;
                winpt->selected_btn = &bzda;
                winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
                inpt->HotKeyID = -1;
            }
        }
        else
        {
            bzda.field_1D0 = 1;
        }
        break;

    case 25:
        if ( yw->_userUnit != yw->_userRobo )
        {
            if ( yw->_hud.field_0 )
                yw->_hud.field_0 = 0;
            else
                yw->_hud.field_0 = 1;
        }
        break;

    case 27:
        if ( info_log.IsClosed() )
        {
            yw->GuiWinOpen(&info_log);
            yw->GuiWinToFront(&info_log);
        }
        else
            yw->GuiWinClose(&info_log);
        break;

    case 28:
        if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16 )
        {
            bzda.field_8F8++;
            if ( bzda.field_8F8 >= bzda.field_8E8 )
                bzda.field_8F8 = 0;

            if ( bzda.field_8F8 >= bzda.field_8E0 )
            {
                bzda.field_8F4 = bzda.field_8F8 - bzda.field_8E0;
                bzda.field_1D0 = 16;
                bzda.field_8EC = -1;
            }
            else
            {
                bzda.field_8EC = bzda.field_8F8;
                bzda.field_8F4 = -1;

                if ( !(bzda.field_1D0 & 4) )
                    bzda.field_1D0 = 2;
            }
        }
        return 0;

    case 29:
        if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16 )
        {
            bzda.field_8F8--;

            if ( bzda.field_8F8 < 0 )
                bzda.field_8F8 = bzda.field_8E8 - 1;

            if ( bzda.field_8F8 >= bzda.field_8E0 )
            {
                bzda.field_8F4 = bzda.field_8F8 - bzda.field_8E0;
                bzda.field_8EC = -1;
                bzda.field_1D0 = 16;
            }
            else
            {
                bzda.field_8EC = bzda.field_8F8;
                bzda.field_8F4 = -1;

                if ( !(bzda.field_1D0 & 4) )
                    bzda.field_1D0 = 2;
            }
        }
        return 0;

    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
        if ( yw->_activeCmdrRemapIndex != -1 )
        {
            int v16;

            switch ( inpt->HotKeyID )
            {
            case 38:
                v16 = 0;
                break;

            case 39:
                v16 = 25;
                break;

            case 40:
                v16 = 50;
                break;

            case 41:
                v16 = 75;
                break;

            case 42:
                v16 = 100;
                break;
            }

            yw->_cmdrsRemap[ yw->_activeCmdrRemapIndex ]->setBACT_aggression(v16);

            if ( yw->_GameShell )
                SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
        }
        break;

    case 43:
        yw->sub_449DE8(Locale::Text::Help(Locale::HELP_GAMEPLAY));
        break;

    case 46:
        if ( yw->_userUnit->_status != BACT_STATUS_DEAD )
        {
            winpt->selected_btnID = 8;
            winpt->selected_btn = &bzda;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
        }
        break;

    default:
        break;
    }

    return 1;
}

NC_STACK_ypabact * NC_STACK_ypaworld::GetDefaultCmdr() const
{
    if ( _activeCmdrRemapIndex != -1 )
        return _cmdrsRemap.at( _activeCmdrRemapIndex );

    if ( !_cmdrsRemap.empty() )
        return _cmdrsRemap.front(); 
    
    return _userRobo;
}

NC_STACK_ypabact * NC_STACK_ypaworld::GetNextCmdrByCmdId(uint32_t cmdrId) const
{
    for (auto it = _cmdrsRemap.begin(); it != _cmdrsRemap.end(); ++it)
    {
        if ( cmdrId == (*it)->_commandID )
        {
            ++it;
            if (it != _cmdrsRemap.end())
                return *it;

            return _cmdrsRemap.front();
        }
    }
    
    return GetDefaultCmdr();
}

NC_STACK_ypabact * NC_STACK_ypaworld::GetNextSquad() const
{
    if ( _userUnit == _userRobo || _playerInHSGun )
        return GetDefaultCmdr();
    
    if ( _userUnit->_status == BACT_STATUS_DEAD )
    {
        for (NC_STACK_ypabact *comm : _cmdrsRemap)
        {
            if ( _vehicleTakenCommandId == comm->_commandID )
                return comm;
        }

        return GetDefaultCmdr();
    }
    
    if ( _userUnit->_bact_type == BACT_TYPES_GUN )
        return GetDefaultCmdr();

    return GetNextCmdrByCmdId(_vehicleTakenCommandId);
}

void ypaworld_func64__sub7__sub2__sub2(NC_STACK_ypaworld *yw)
{
    if ( yw->_userUnit->_status != BACT_STATUS_DEAD )
    {
        if ( yw->_userUnit == yw->_userRobo )
        {
            yw->_roboMapStatus.Valid = true;
            yw->_roboMapStatus.IsOpen = robo_map.IsOpen();
            yw->_roboMapStatus.Rect.x = robo_map.x;
            yw->_roboMapStatus.Rect.y = robo_map.y;
            yw->_roboMapStatus.Rect.w = robo_map.w;
            yw->_roboMapStatus.Rect.h = robo_map.h;

            yw->_roboMapStatus.Data[0] = robo_map.field_1EC;
            yw->_roboMapStatus.Data[1] = robo_map.field_1ED;
            yw->_roboMapStatus.Data[2] = robo_map.field_1EE;
            yw->_roboMapStatus.Data[3] = robo_map.field_208;
            yw->_roboMapStatus.Data[4] = robo_map.field_20A;
            yw->_roboMapStatus.Data[5] = robo_map.field_20C;
            yw->_roboMapStatus.Data[6] = robo_map.field_20E;

            yw->_roboFinderStatus.Valid = true;
            yw->_roboFinderStatus.IsOpen = squadron_manager.IsOpen();
            yw->_roboFinderStatus.Rect.x = squadron_manager.x;
            yw->_roboFinderStatus.Rect.y = squadron_manager.y;
            yw->_roboFinderStatus.Rect.w = squadron_manager.w;
            yw->_roboFinderStatus.Rect.h = squadron_manager.h;
        }
        else
        {
            yw->_vhclMapStatus.Valid = true;
            yw->_vhclMapStatus.IsOpen = robo_map.IsOpen();
            yw->_vhclMapStatus.Rect.x = robo_map.x;
            yw->_vhclMapStatus.Rect.y = robo_map.y;
            yw->_vhclMapStatus.Rect.w = robo_map.w;
            yw->_vhclMapStatus.Rect.h = robo_map.h;

            yw->_vhclMapStatus.Data[0] = robo_map.field_1EC;
            yw->_vhclMapStatus.Data[1] = robo_map.field_1ED;
            yw->_vhclMapStatus.Data[2] = robo_map.field_1EE;
            yw->_vhclMapStatus.Data[3] = robo_map.field_208;
            yw->_vhclMapStatus.Data[4] = robo_map.field_20A;
            yw->_vhclMapStatus.Data[5] = robo_map.field_20C;
            yw->_vhclMapStatus.Data[6] = robo_map.field_20E;

            yw->_vhclFinderStatus.Valid = true;
            yw->_vhclFinderStatus.IsOpen = squadron_manager.IsOpen();
            yw->_vhclFinderStatus.Rect.x = squadron_manager.x;
            yw->_vhclFinderStatus.Rect.y = squadron_manager.y;
            yw->_vhclFinderStatus.Rect.w = squadron_manager.w;
            yw->_vhclFinderStatus.Rect.h = squadron_manager.h;
        }
    }
}

NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub5(NC_STACK_ypaworld *yw)
{
    if ( bzda.field_8F0 != -1 )
    {
        if ( yw->_playerInHSGun )
        {
            bzda.field_8F0++;

            if ( bzda.field_8F0 >= bzda.field_8DC )
            {
                bzda.field_8F0 = 0;
                yw->_playerInHSGun = false;
                return yw->_userRobo;
            }

            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

            yw->_playerInHSGun = true;

            return robo->GetGuns().at(  bzda.field_1DC[ bzda.field_8F0 ]  ).gun_obj;
        }
        else
        {
            bzda.field_8F0 = 0;

            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

            yw->_playerInHSGun = true;

            return robo->GetGuns().at( bzda.field_1DC[ bzda.field_8F0 ] ).gun_obj;
        }
    }
    return NULL;
}

NC_STACK_ypabact * ypaworld_func64__sub7__sub2__sub4(NC_STACK_ypaworld *yw)
{
    if ( yw->_userUnit->IsParentMyRobo() )
    {
        for ( NC_STACK_ypabact* &unit : yw->_userUnit->_kidList )
        {
            if ( unit->_status != BACT_STATUS_CREATE && unit->_status != BACT_STATUS_DEAD && unit->_status != BACT_STATUS_BEAM )
            return unit;
        }
    }
    else
    {
        World::RefBactList *lst = yw->_userUnit->_kidRef.PList();
        if (lst)
        {
            for( World::RefBactList::iterator it = std::next(yw->_userUnit->_kidRef.iter(), 1); it != lst->end(); it++) // Start from current unit in this list
            {
               if ( (*it)->_status != BACT_STATUS_CREATE && (*it)->_status != BACT_STATUS_DEAD && (*it)->_status != BACT_STATUS_BEAM )
                    return *it;
            }
        }
    }

    if ( yw->_userUnit->_parent != yw->_userUnit->_host_station && yw->_userUnit->_parent )
        return yw->_userUnit->_parent;

    return NULL;
}

int sub_4C885C()
{
    if ( dword_5BAF9C == 3 )
    {
        if ( lstvw2.IsClosed() )
            dword_5BAF9C = 2;
    }
    return dword_5BAF9C;
}

void ypaworld_func64__sub7__sub2__sub8(NC_STACK_ypaworld *yw)
{
    if ( sub_4C885C() != 3 )
    {
        yw->GuiWinOpen( &exit_menu );
        exit_menu.Attach(yw->_guiActive);
    }
}

void ypaworld_func64__sub7__sub2__sub9(NC_STACK_ypaworld *yw)
{
    if ( sub_4C885C() != 3 )
        yw->GuiWinClose( &exit_menu );
}

void NC_STACK_ypaworld::SituationAnalyzer()
{
    yw_arg159 v38;
    v38.txt = World::GameAnalyzer::Analyze(this);
    v38.unit = 0;
    v38.Priority = 100;
    v38.MsgID = 0;

    ypaworld_func159(&v38);
}

void ypaworld_func64__sub7__sub2__sub0()
{
    bzda.field_8E8 = 0;

    for (int i = 0; i < bzda.field_8E0; i++)
    {
        bzda.field_4DC[i].i = 1;
        bzda.field_4DC[i].d = bzda.field_2DC[i];
    }

    bzda.field_8E8 += bzda.field_8E0;

    if ( !(bzda.field_1D0 & 4) )
    {
        for (int i = 0; i < bzda.field_8E4; i++)
        {
            bzda.field_4DC[i + bzda.field_8E0].i = 2;
            bzda.field_4DC[i + bzda.field_8E0].d = bzda.field_3DC[i];
        }

        bzda.field_8E8 += bzda.field_8E4;
    }

    if ( bzda.field_8E8 )
    {
        if ( bzda.field_8E8 == 1 )
        {
            bzda.field_8F8 = 0;
        }
        else if ( bzda.field_8E8 <= bzda.field_8F8 )
        {
            bzda.field_8F8 = bzda.field_8E8 - 1;
        }
    }
    else
    {
        bzda.field_8F8 = -1;
    }
}

void sb_0x4c66f8__sub0(NC_STACK_ypaworld *yw)
{
    if ( yw->_timeStamp - yw->_msgTimestampHSReturn > 35000 )
    {
        yw_arg159 arg159;
        arg159.unit = yw->_userRobo;
        arg159.Priority = 10;
        arg159.MsgID = 39;

        yw->ypaworld_func159(&arg159);
    }

    yw->_msgTimestampHSReturn = yw->_timeStamp;
}

void sb_0x4c66f8(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact1, NC_STACK_ypabact *bact2)
{
    if ( bact1 != bact2 )
    {
        if ( bact1->_status != BACT_STATUS_CREATE && bact1->_status != BACT_STATUS_DEAD && bact1->_status != BACT_STATUS_BEAM )
        {
            yw->_hud.field_76 = yw->_timeStamp;

            bact2->setBACT_viewer(false);
            bact2->setBACT_inputting(false);

            bact1->setBACT_viewer(true);
            bact1->setBACT_inputting(true);

            yw->GuiWinClose( &gui_lstvw );

            if ( bzda.field_1CC & 1 )
            {
                if ( bzda.field_1D0 == 8 )
                    bzda.field_1D0 = 1;
            }

            if ( bact1 == yw->_userRobo )
            {
                yw->_mouseGrabbed = false;
                sb_0x4c66f8__sub0(yw);
            }
            else
            {
                yw_arg159 arg159;
                arg159.unit = yw->_userUnit;
                arg159.Priority = 33;
                arg159.MsgID = 17;

                yw->ypaworld_func159(&arg159);
            }

            yw->_guiDragDefaultMouse = false;
        }
    }
}



void  ypaworld_func64__sub7__sub2(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    TClickBoxInf *winpt = &inpt->ClickInf;

    NC_STACK_ypabact *bact1 = NULL;
    NC_STACK_ypabact *bact2 = yw->_viewerBact;

    bzda.field_91C = 0;

    int a2a = 1;
    if ( bzda.field_1D4 & 1 )
    {
        switch ( inpt->HotKeyID )
        {
            case 31:
            {
                NC_STACK_ypabact *v9 = yw->GetLastMsgSender();

                if ( v9 )
                    bact1 = v9;
            }
            break;

            case 44:
                bact1 = ypaworld_func64__sub7__sub2__sub6(yw);
                break;

            case 45:
                ypaworld_func64__sub7__sub2__sub7(yw);
                break;

            default:
                if ( inpt->HotKeyID >= 0 )
                    a2a = ypaworld_func64__sub7__sub2__sub3(yw, inpt);

                break;
        }

        if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
        {
            if ( yw->_fireBtnDownHappen )
            {
                bact1 = yw->GetNextSquad();
                inpt->Buttons.UnSet(0);
            }
        }

        if ( gui_lstvw.IsOpen() )
        {
            if ( yw->_kbdLastKeyHit == Input::KC_ESCAPE || yw->_kbdLastKeyHit == Input::KC_RETURN )
            {
                yw->GuiWinClose(&gui_lstvw);

                inpt->KbdLastDown = Input::KC_NONE;
                inpt->HotKeyID = -1;
                inpt->KbdLastHit = Input::KC_NONE;
            }
        }

        if ( yw->_userUnit == yw->_userRobo )
        {
            if ( robo_map.field_1E8 & 8 )
                robo_map.field_1E8 &= 0xFFFFFFF7;
        }
        else if ( !(robo_map.field_1E8 & 8) )
        {
            robo_map.field_1E8 |= 8;
        }

        ypaworld_func64__sub7__sub2__sub2(yw);

        if ( winpt->selected_btn == &bzda )
        {
            if ( winpt->selected_btnID != -1 )
            {
                if ( yw->_GameShell )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                        SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
                }
            }

            switch ( winpt->selected_btnID )
            {
            case 0:
                if ( !(winpt->flag & TClickBoxInf::FLAG_BTN_DOWN) )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_RM_DOWN )
                    {
                        if ( bzda.field_1D0 & 0x16 )
                        {
                            bzda.field_1D0 = bzda.field_1CC & 1;
                            yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEWSQUAD, 2);
                        }
                        else
                        {

                            if ( !(bzda.field_1D0 & 4) || gui_lstvw.IsClosed() )
                            {
                                yw->GuiWinOpen( &gui_lstvw );
                                gui_lstvw.listFlags |= (GuiList::GLIST_FLAG_IN_SELECT | GuiList::GLIST_FLAG_NO_SCROLL);
                            }
                            else
                            {
                                yw->GuiWinClose( &gui_lstvw );
                            }

                            a2a = 0;
                            bzda.field_1D0 = 4;
                        }
                    }
                    else
                        yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEWSQUAD, 2);

                }
                else if ( bzda.field_1D0 & 0x16 )
                {
                    bzda.field_1D0 = bzda.field_1CC & 1;

                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEWSQUAD, 2);
                }
                else
                {
                    if ( !(bzda.field_1D0 & 2) || gui_lstvw.IsClosed() )
                    {
                        yw->GuiWinOpen( &gui_lstvw );
                        gui_lstvw.listFlags |= (GuiList::GLIST_FLAG_IN_SELECT | GuiList::GLIST_FLAG_NO_SCROLL);
                    }
                    else
                    {
                        yw->GuiWinClose( &gui_lstvw );
                    }

                    if ( bzda.field_8E0 <= bzda.field_8F8 )
                        bzda.field_1D0 = 16;
                    else
                        bzda.field_1D0 = 2;

                    a2a = 0;
                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEWSQUAD, 2);
                }
                break;

            case 1: // Jump into vehicle
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                {
                    if ( bzda.field_1D0 & 8 )
                        bzda.field_1D0 = bzda.field_1CC & 1;
                    else
                        bzda.field_1D0 = 8;
                }

                yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_TAKECONTROL, 4);
                break;

            case 2: //MAP
            case 3: //SQUAD
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                {
                    if ( winpt->selected_btnID == 2 )
                    {
                        if (robo_map.IsClosed())
                        {
                            yw->GuiWinOpen(&robo_map);
                            yw->GuiWinToFront(&robo_map);
                        }
                        else
                            yw->GuiWinClose(&robo_map);
                    }
                    else if ( winpt->selected_btnID == 3 )
                    {
                        if (squadron_manager.IsClosed())
                        {
                            yw->GuiWinOpen(&squadron_manager);
                            yw->GuiWinToFront(&squadron_manager);
                        }
                        else
                            yw->GuiWinClose(&squadron_manager);
                    }
                }

                if ( winpt->selected_btnID == 2 )
                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_MAPWND, 8);
                else if ( winpt->selected_btnID == 3 )
                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_FINDERWND, 9);

                break;

            case 4: //Back to host station
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x10;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    bact1 = yw->_userRobo;

                    if ( yw->_playerInHSGun )
                        bzda.field_1D0 = 1;
                }

                yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_GOTOHS, 21);
                break;

            case 5:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x20;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    bact1 = yw->_userUnit->_parent;

                yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_GOTOCMDR, 23);
                break;

            case 6: // Cycle through units in squad
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x40;

                if ( yw->_userRobo == yw->_userUnit || yw->_playerInHSGun )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                        bact1 = ypaworld_func64__sub7__sub2__sub5(yw);

                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_GOINTOGUNS, 22);
                }
                else
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                        bact1 = ypaworld_func64__sub7__sub2__sub4(yw);

                    yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEXTUNIT, 22);
                }
                break;

            case 7: //Cycle through squad leader
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x80;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    bact1 = yw->GetNextSquad();

                yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_NEXTCMDR, 20);
                break;

            case 8:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x100;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    yw->SituationAnalyzer();

                yw->SetShowingTooltipWithHotkey(Locale::TIP_ANALYZER, 46);
                break;

            case 9:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x200;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    yw->sub_449DE8(Locale::Text::Help(Locale::HELP_GAMEPLAY));
                }

                yw->SetShowingTooltipWithHotkey(Locale::TIP_ONLINEHELP, 43);
                break;

            case 10:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    bzda.field_91C |= 0x400;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    if ( exit_menu.IsClosed() )
                        ypaworld_func64__sub7__sub2__sub8(yw);
                    else
                        ypaworld_func64__sub7__sub2__sub9(yw);
                }

                yw->SetShowingTooltipWithHotkey(Locale::TIP_GUI_PAUSECANCEL, 24);
                break;

            default:
                break;
            }
        }

        if ( bzda.field_1D0 & 0x29 )
            yw->GuiWinClose(&gui_lstvw);

        if ( a2a )
        {
            gui_lstvw.InputHandle(yw, inpt);
            if ( gui_lstvw.IsOpen() )
            {
                if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16)
                {
                    bzda.field_8F8 = gui_lstvw.selectedEntry;

                    if ( gui_lstvw.selectedEntry >= bzda.field_8E0 )
                    {
                        bzda.field_8F4 = gui_lstvw.selectedEntry - bzda.field_8E0;
                        bzda.field_8EC = -1;
                        bzda.field_1D0 = 16;
                    }
                    else
                    {
                        bzda.field_8F4 = -1;
                        bzda.field_8EC = gui_lstvw.selectedEntry;

                        if ( !(bzda.field_1D0 & 4) )
                            bzda.field_1D0 = 2;
                    }
                }
            }
        }
    }

    if ( bact1 )
    {
        if ( bact1 != bact2 )
            sb_0x4c66f8(yw, bact1, bact2);
    }

    ypaworld_func64__sub7__sub2__sub0();
    ypaworld_func64__sub7__sub2__sub1(yw);
}







char * sub_4E1D6C(NC_STACK_ypaworld *yw, char *cur, int x, int y, uint8_t icon, uint8_t icon2, uint8_t icon3, float a6, float a7, const std::string &a8)
{
    char *pcur = cur;

    int v26 = (a6 * up_panel.field_1E4);
    int v25 = (a7 * up_panel.field_1E4);

    if ( !v26 && a6 > 0.0 )
        v26 = 1;

    if ( !v25 && a7 > 0.0 )
        v25 = 1;

    FontUA::select_tileset(&pcur, 30);
    FontUA::set_center_xpos(&pcur, x);
    FontUA::set_center_ypos(&pcur, y);

    FontUA::store_u8(&pcur, icon);

    FontUA::store_u8(&pcur, 84);

    if ( v26 > 0 || v25 > 0 )
    {
        int v13 = up_panel.field_1E0 + up_panel.field_1DC + x;

        FontUA::set_center_xpos(&pcur, v13);

        if ( v26 > 0 )
        {
            if ( v26 > 1 )
                FontUA::op11(&pcur, v26);

            FontUA::store_u8(&pcur, icon2);
        }

        if ( v25 > v26 )
        {
            if ( v25 > 1 )
                FontUA::op11(&pcur, v25);

            FontUA::store_u8(&pcur, icon3);
        }
    }

    if ( !a8.empty() )
    {
        FontUA::select_tileset(&pcur, 31);

        FontUA::set_center_xpos(&pcur, x + up_panel.field_1DC + 4);
        FontUA::set_ypos(&pcur, 0);

        memcpy(pcur, a8.data(), a8.size());

        pcur += a8.size();
    }
    return pcur;
}

char * ypaworld_func64__sub7__sub7__sub0__sub0(NC_STACK_ypaworld *yw, char *cur, int x, int y, int a3, int a4, int a5, int a6, float a7)
{
    int icon;

    if ( a3 == -1 )
        icon = 82;
    else if ( a3 == 0 )
        icon = 83;
    else if ( a3 == 1 )
        icon = 81;

    int v11 = 0;
    switch ( yw->_userRobo->_pSector->owner )
    {
    case 1:
        v11 = 49;
        break;

    case 2:
        v11 = 50;
        break;

    case 3:
        v11 = 51;
        break;

    case 4:
        v11 = 52;
        break;

    case 5:
        v11 = 53;
        break;

    case 6:
        v11 = 54;
        break;

    case 7:
        v11 = 55;
        break;

    default:
        v11 = 0;
        break;
    }

    float a6a, a7a;

    if ( a4 <= 0 )
    {
        a7a = 0.0;
        a6a = 0.0;
    }
    else
    {
        a7a = a4 / 255.0;
        a6a = a7 * a7a;
    }

    std::string a1a;

    if ( a3 >= 0 )
    {
        if ( a5 )
        {
            a1a = fmt::sprintf("+%d   (%d%%)", a5, (int)(a7 * 100.0) );
        }
        else
        {
            a1a = fmt::sprintf("%d", 0);
        }
    }
    else
    {
        a1a = fmt::sprintf("%d", -a5);
    }

    return sub_4E1D6C(yw, cur, x, y, icon, v11, 57, a6a, a7a, a1a);
}

char * ypaworld_func64__sub7__sub7__sub0__sub1(NC_STACK_ypaworld *yw, char *cur, int x, int y, char a4, int a5, int a6, int a7)
{
    int v16;
    if ( a4 & 1 )
    {
        if ( a5 == -1 )
        {
            v16 = 66;
        }
        else if ( a5 == 0 )
        {
            v16 = 67;
        }
        else if ( a5 == 1 )
        {
            v16 = 65;
        }
    }
    else
    {
        v16 = 68;
    }

    float a6a = (float)a6 / (float)a7;
    std::string a1a = fmt::sprintf("%d", a6 / 100);

    int v10;

    if ( a7 / 5 <= a6 )
    {
        v10 = 56;
    }
    else
    {
        v10 = 54;

        if ( yw->_timeStamp / 300 & 1 )
            a1a = "";
    }

    return sub_4E1D6C(yw, cur, x, y, v16, v10, 0, a6a, 0.0, a1a);
}


char * ypaworld_func64__sub7__sub7__sub0__sub2(NC_STACK_ypaworld *yw, char *cur, int a3, int a2, char a4, int a5, int a6, int a7)
{
    int v18;
    if ( a4 & 4 )
    {
        if ( a5 == -1 )
            v18 = 70;
        else if ( a5 == 0 )
            v18 = 71;
        else if ( a5 == 1 )
            v18 = 69;
    }
    else
    {
        v18 = 72;
    }

    float v15 = (float)a7;
    int v9 = 57;
    float a7a = 0.0;
    float a6a = (float)a6 / v15;

    if ( bzda.field_1D0 & 0x16 )
    {
        if ( bzda.field_8F8 != -1 )
            a7a = yw->_updateMessage.energy / v15;
    }

    if ( a7a <= a6a )
    {
        float v10 = a6a;
        a6a -= a7a;
        a7a = v10;
    }
    else
    {
        a7a = a6a;
        v9 = 54;
        a6a = 0.0;
    }

    return sub_4E1D6C(yw, cur, a3, a2, v18, 56, v9, a6a, a7a,  fmt::sprintf("%d", a6 / 100) );
}

char * ypaworld_func64__sub7__sub7__sub0__sub3(NC_STACK_ypaworld *yw, char *cur, int x, int y, char a4, int a5, int a6, int a7)
{
    int v18;
    if ( a4 & 8 )
    {
        if ( a5 == -1 )
            v18 = 78;
        else if ( a5 == 0 )
            v18 = 79;
        else if ( a5 == 1 )
            v18 = 77;
    }
    else
    {
        v18 = 80;
    }

    float v15 = (float)a7;
    int v9 = 57;
    float a7a = 0.0;
    float a6a = (float)a6 / v15;

    if ( bzda.field_1D0 & 0x20 )
        a7a = yw->_updateMessage.energy / v15;

    if ( a7a <= a6a )
    {
        float v10 = a6a;
        a6a -= a7a;
        a7a = v10;
    }
    else
    {
        a7a = a6a;
        v9 = 54;
        a6a = 0.0;
    }

    return sub_4E1D6C(yw, cur, x, y, v18, 56, v9, a6a, a7a,  fmt::sprintf("%d", a6 / 100) );
}

char * ypaworld_func64__sub7__sub7__sub0(NC_STACK_ypaworld *yw)
{
    char *pcur = byte_51805C;

    if ( yw->_userRobo->_status != BACT_STATUS_DEAD )
    {
        int v3 = up_panel.x - yw->_screenSize.x / 2;
        int y = up_panel.y - yw->_screenSize.y / 2;
        int v41 = up_panel.field_1D8 + up_panel.field_1D0;

        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

        int a4 = robo->getROBO_fillMode();
        int v29 = robo->getROBO_battVehicle();
        int v28 = robo->getROBO_battBeam();
        int v27 = robo->getROBO_loadFlags();
        int v26 = robo->getROBO_lossFlags();

        int v36 = yw->_userRobo->_energy_max;

        yw_arg176 arg176;
        arg176.owner = yw->_userRobo->_pSector->owner;

        yw->ypaworld_func176(&arg176);

        int v32 = yw->_userRobo->_pSector->energy_power;
        int v33 = yw->_userRobo->_pSector->energy_power * arg176.field_4;

        int idx = robo->getROBO_absReload();

        int v11 = 10 * idx / 100;
        int v38 = 10 * ( idx * arg176.field_4 ) / 100;

        int v13, v34, v37, v35;

        if ( yw->_userRobo->_owner == yw->_userRobo->_pSector->owner )
            v13 = v33 != 0;
        else if ( v33 )
            v13 = -1;
        else
            v13 = 0;

        if ( v27 & 1 )
            v34 = 1;
        else if ( v26 & 1 )
            v34 = -1;
        else
            v34 = 0;

        if ( v27 & 4 )
            v37 = 1;
        else if ( v26 & 4 )
            v37 = -1;
        else
            v37 = 0;

        if ( v27 & 8 )
            v35 = 1;
        else if ( v26 & 8 )
            v35 = -1;
        else
            v35 = 0;

        Input::Engine.RemClickBox(&up_panel);
        Input::Engine.AddClickBoxFront(&up_panel);

        int x = (up_panel.field_1D4 & 0xFFFF) + v3;

        pcur = ypaworld_func64__sub7__sub7__sub0__sub0(yw, pcur, x, y, v13, v32, v38, v11, arg176.field_4);

        int v18 = v41 + x;
        pcur = ypaworld_func64__sub7__sub7__sub0__sub1(yw, pcur, v18, y, a4, v34, yw->_userRobo->_energy, v36);

        int v20 = v41 + v18;
        pcur = ypaworld_func64__sub7__sub7__sub0__sub2(yw, pcur, v20, y, a4, v37, v29, v36);
        pcur = ypaworld_func64__sub7__sub7__sub0__sub3(yw, pcur, v41 + v20, y, a4, v35, v28, v36);
    }

    FontUA::set_end(&pcur);

    return pcur;
}

void ypaworld_func64__sub7__sub7(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    TClickBoxInf *winpt = &inpt->ClickInf;

    if ( winpt->selected_btn == &up_panel )
    {
        if ( yw->_GameShell )
        {
            if ( winpt->selected_btnID != -1 )
            {
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                    SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
            }
        }

        if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
        {
            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

            int a4 = robo->getROBO_fillMode();

            if ( winpt->selected_btnID == 1 )
                a4 ^= 1;
            else if ( winpt->selected_btnID == 2 )
                a4 ^= 4;
            else if ( winpt->selected_btnID == 3 )
                a4 ^= 8;

            robo->setROBO_fillMode(a4);
        }

        switch ( winpt->selected_btnID )
        {
        case 0:
            yw->SetShowingTooltip(Locale::TIP_ENERGY_RELOAD);
            break;

        case 1:
            yw->SetShowingTooltip(Locale::TIP_ENERGY_MAIN);
            break;

        case 2:
            yw->SetShowingTooltip(Locale::TIP_ENERGY_CREATION);
            break;

        case 3:
            yw->SetShowingTooltip(Locale::TIP_ENERGY_TELEPORT);
            break;

        default:
            break;
        }
    }

    ypaworld_func64__sub7__sub7__sub0(yw);
}


uint8_t sub_4C7134(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact)
{
    uint8_t icon = yw->_vhclProtos[ bact->_vehicleID ].type_icon;

    if ( !icon )
        icon = 65;

    return icon;
}

char * sub_4C7950(NC_STACK_ypaworld *yw, char *cur, int a4, int a3)
{
    char *pcur = cur;

    if ( a4 > 0 && a3 > 0 )
    {
        if ( a4 + squadron_manager.field_2CC < yw->_screenSize.x )
        {
            if ( a3 + yw->_fontH < yw->_screenSize.y )
            {
                FontUA::select_tileset(&pcur, 28);

                FontUA::set_center_xpos(&pcur, a4 - (yw->_screenSize.x / 2));
                FontUA::set_center_ypos(&pcur, a3 - (yw->_screenSize.y / 2));

                FontUA::store_u8(&pcur, sub_4C7134(yw, squadron_manager.field_2BC));
            }
        }
    }
    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub2(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    FontUA::select_tileset(&pcur, 0);

    FontUA::store_u8(&pcur, 123);

    FontUA::op17(&pcur, squadron_manager.entryWidth - yw->_fontBorderW);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    return pcur;
}


char * sub_4514F0(TileMap *tyle, char *cur, char *a4, int a3, char a5)
{
    char *pcur = cur;

    int wdth = 0;
    int char_num = 0;
    int neg_wdth = 0;

    char *v6 = a4;

    while ( *v6 && neg_wdth == 0 )
    {
        int v18 = (uint8_t)*v6;

        wdth += tyle->map[v18].w;

        if ( wdth <= a3 )
            char_num++;
        else
            neg_wdth = tyle->map[v18].w - (wdth - a3);

        v6++;
    }

    for (int i = 0; i < char_num; i++)
    {
        *pcur = a4[i];
        pcur++;
    }

    if ( neg_wdth <= 0 )
    {
        if ( wdth < a3 )
        {
            int v13 = a3 - wdth;

            while( v13 > 0 )
            {
                if ( v13 <= 255 )
                    FontUA::op10(&pcur, v13);
                else
                    FontUA::op10(&pcur, 255);

                v13 -= 255;

                FontUA::store_u8(&pcur, a5);
            }
        }
    }
    else
    {
        FontUA::set_xwidth(&pcur, neg_wdth);

        FontUA::store_u8(&pcur, a4[char_num]);
    }

    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub1(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact, char *cur)
{
    char *pcur = cur;

    char v13[80];
    if ( bact == yw->_userUnit && yw->_timeStamp / 300 & 1 )
    {
        v13[0] = 33;
    }
    else if ( info_log.field_255C == bact->_gid && yw->_timeStamp - info_log.msgs[info_log.field_24C].field_4 < 10000 && yw->_timeStamp / 300 & 1 )
    {
        v13[0] = 34;
    }
    else
    {
        v13[0] = sub_4C7134(yw, bact);
    }
    v13[1] = 0;

    int v4;

    if ( bzda.field_1D0 & 0x20 )
        v4 = 9;
    else
        v4 = 0;

    FontUA::select_tileset(&pcur, 0);
    FontUA::store_u8(&pcur, 123);

    FontUA::select_tileset(&pcur, v4);
    FontUA::op17(&pcur, squadron_manager.entryWidth - yw->_fontBorderW);
    FontUA::store_u8(&pcur, 32);

    FontUA::select_tileset(&pcur, 0);
    FontUA::store_u8(&pcur, 125);
    FontUA::add_xpos(&pcur, -(squadron_manager.entryWidth - 2 * yw->_fontBorderW + 1));

    FontUA::select_tileset(&pcur, 28);

    pcur = sub_4514F0(yw->_guiTiles[28], pcur, v13, squadron_manager.entryWidth - 2 * yw->_fontBorderW, 64);

    FontUA::next_line(&pcur);

    return pcur;
}

uint8_t ypaworld_func64__sub7__sub3__sub0__sub0__sub0(NC_STACK_ypabact *bact)
{
    if ( bact->_status == BACT_STATUS_NORMAL )
    {
        if ( bact->_status_flg & BACT_STFLAG_ESCAPE )
        {
            return 102;
        }
        else if ( bact->_secndTtype )
        {
            return 99;
        }
        else if ( bact->_primTtype )
        {
            return 98;
        }
    }
    else if ( bact->_status == BACT_STATUS_IDLE )
    {
        if ( bact->_status_flg & BACT_STFLAG_ESCAPE )
            return 102;
    }

    return 97;
}

char * ypaworld_func64__sub7__sub3__sub0__sub0(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact, char *cur)
{
    char *pcur = cur;

    char v39[64];
    char *pv = v39;

    int v46 = 0;
    NC_STACK_ypabact *v44 = NULL;

    if ( yw->_guiActFlags & 0x20 )
        v44 = yw->_bactOnMouse;

    FontUA::store_u8(&pv, ypaworld_func64__sub7__sub3__sub0__sub0__sub0(bact));
    FontUA::store_u8(&pv, 64);

    int ttt = (yw->_timeStamp / 300) & 1;

    if ( bact == yw->_userUnit && ttt )
        FontUA::store_u8(&pv, 33);
    else if ( yw->_lastMsgSender == bact && ttt )
        FontUA::store_u8(&pv, 34);
    else
        FontUA::store_u8(&pv, sub_4C7134(yw, bact));

    FontUA::store_u8(&pv, 64);

    if ( bact == v44 )
        v46 = squadron_manager.field_2D0;

    int v41 = squadron_manager.field_2D4;

    for ( NC_STACK_ypabact* &kid : bact->_kidList )
    {
        if ( kid->_status != BACT_STATUS_DEAD && kid->_status != BACT_STATUS_CREATE && kid->_status != BACT_STATUS_BEAM )
        {
            if ( kid == yw->_userUnit && ttt )
                FontUA::store_u8(&pv, 33);
            else if ( kid == yw->_lastMsgSender && ttt )
                FontUA::store_u8(&pv, 34);
            else
                FontUA::store_u8(&pv, sub_4C7134(yw, kid));

            if ( kid == v44 )
                v46 = v41;

            v41 += squadron_manager.field_2CC;

            if ( pv - v39 >= 60 )
                break;
        }
    }

    FontUA::store_u8(&pv, 0);

    int v22;
    if ( yw->_activeCmdrRemapIndex == -1 || bact != yw->_cmdrsRemap[ yw->_activeCmdrRemapIndex ] || bzda.field_1D0 & 0x20 )
        v22 = 0;
    else
        v22 = 9;

    FontUA::select_tileset(&pcur, 0);
    FontUA::store_u8(&pcur, 123);

    FontUA::select_tileset(&pcur, v22);
    FontUA::op17(&pcur, squadron_manager.entryWidth - yw->_fontBorderW);
    FontUA::store_u8(&pcur, 32);

    FontUA::select_tileset(&pcur, 0);
    FontUA::store_u8(&pcur, 125);

    FontUA::add_xpos(&pcur, -(squadron_manager.entryWidth - 2 * yw->_fontBorderW + 1));

    FontUA::select_tileset(&pcur, 28);

    pcur = sub_4514F0(yw->_guiTiles[28], pcur, v39, squadron_manager.entryWidth - 2 * yw->_fontBorderW, 64);
    if ( v46 )
    {
        if ( squadron_manager.entryWidth - squadron_manager.field_2CC - yw->_fontBorderW > v46 )
        {
            FontUA::add_xpos(&pcur, -(squadron_manager.entryWidth - 2 * yw->_fontBorderW));
            FontUA::add_xpos(&pcur, v46);

            float v40 = (float)v44->_energy / (float)v44->_energy_max;

            if ( v40 > 0.25 )
            {
                if ( v40 > 0.5 )
                {
                    if ( v40 > 0.75 )
                        FontUA::store_u8(&pcur, 131);
                    else
                        FontUA::store_u8(&pcur, 130);
                }
                else
                {
                    FontUA::store_u8(&pcur, 129);
                }
            }
            else
            {
                FontUA::store_u8(&pcur, 128);
            }
        }
    }

    FontUA::next_line(&pcur);
    return pcur;
}

char * ypaworld_func64__sub7__sub3__sub0__sub3(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;
    int v23 = squadron_manager.entryWidth - yw->_fontBorderW;

    if ( yw->_activeCmdrRemapIndex == -1 || bzda.field_1D0 & 0x20 )
    {
        FontUA::select_tileset(&pcur, 0);
        FontUA::store_u8(&pcur, '{');
    }
    else
    {
        int v5 = yw->_cmdrsRemap[ yw->_activeCmdrRemapIndex ]->_aggr;
        FontUA::select_tileset(&pcur, 0);
        FontUA::store_u8(&pcur, '{');
        FontUA::store_u8(&pcur, ' ');

        FontUA::select_tileset(&pcur, 25);
        FontUA::store_u8(&pcur, 49);

        if (v5 < 25)
            FontUA::select_tileset(&pcur, 24);

        FontUA::store_u8(&pcur, 50);

        if (v5 >= 25 && v5 < 50)
            FontUA::select_tileset(&pcur, 24);

        FontUA::store_u8(&pcur, 51);

        if (v5 >= 50 && v5 < 75)
            FontUA::select_tileset(&pcur, 24);

        FontUA::store_u8(&pcur, 52);

        if (v5 >= 75 && v5 < 100)
            FontUA::select_tileset(&pcur, 24);

        FontUA::store_u8(&pcur, 53);

        FontUA::select_tileset(&pcur, 0);

        FontUA::set_txtColor(&pcur, yw->_iniColors[60].r, yw->_iniColors[60].g, yw->_iniColors[60].b);

        pcur = FontUA::FormateClippedText(yw->_guiTiles[0], pcur, fmt::sprintf(" %d", yw->_activeCmdrKidsCount + 1) , 4 * yw->_guiTiles[0]->map[65].w, 32);

        FontUA::store_u8(&pcur, ' ');

        FontUA::select_tileset(&pcur, 0);
    }

    FontUA::op17(&pcur, v23);
    FontUA::store_u8(&pcur, ' ');
    FontUA::store_u8(&pcur, '}');

    FontUA::next_line(&pcur);

    FontUA::reset_tileset(&pcur, 28);

    FontUA::set_yoff(&pcur, yw->_guiTiles[28]->h - yw->_fontBorderH);

    FontUA::store_u8(&pcur, 38);

    FontUA::op17(&pcur, squadron_manager.entryWidth - yw->_fontBorderW);

    FontUA::store_u8(&pcur, 47);
    FontUA::store_u8(&pcur, 61);

    return pcur;
}



void ypaworld_func64__sub7__sub3__sub0(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    TClickBoxInf *winpt = &inpt->ClickInf;

    char *pcur = squadron_manager.ItemsPreLayout(yw, squadron_manager.itemBlock, 0, "{ }");

    FontUA::select_tileset(&pcur, 28);

    for (int i = 0; i < squadron_manager.shownEntries; i++)
    {
        NC_STACK_ypabact *v8 = squadron_manager.squads[i];

        if ( v8 )
        {
            if ( v8 == yw->_userRobo )
            {
                pcur = ypaworld_func64__sub7__sub3__sub0__sub1(yw, v8, pcur);
            }
            else
            {
                if ( v8->IsParentMyRobo() )
                    pcur = ypaworld_func64__sub7__sub3__sub0__sub0(yw, v8, pcur);
                else
                    pcur = ypaworld_func64__sub7__sub3__sub0__sub2(yw, pcur);
            }
        }
        else
        {
            pcur = ypaworld_func64__sub7__sub3__sub0__sub2(yw, pcur);
        }
    }

    pcur = ypaworld_func64__sub7__sub3__sub0__sub3(yw, pcur);

    if ( squadron_manager.field_2A8 & 1 )
    {
        if ( squadron_manager.field_2C8 != winpt->move.ScreenPos.x || squadron_manager.field_2CA != winpt->move.ScreenPos.y )
        {
            pcur = sub_4C7950(yw, pcur, squadron_manager.field_2C4, squadron_manager.field_2C6);

            if ( squadron_manager.field_2A8 & 2 )
                pcur = sub_4C7950(yw, pcur, squadron_manager.field_2C4 + squadron_manager.field_2CC / 4, squadron_manager.field_2C6 + (yw->_fontH / 4) );
        }
    }

    FontUA::set_end(&pcur);
}

void sub_4C707C(NC_STACK_ypaworld *yw)
{
    squadron_manager.squads.fill(NULL);

    if ( (size_t)(squadron_manager.firstShownEntries + squadron_manager.shownEntries) >= yw->_cmdrsRemap.size() + 1 )
    {
        squadron_manager.firstShownEntries = yw->_cmdrsRemap.size() + 1 - squadron_manager.shownEntries;

        if ( squadron_manager.firstShownEntries < 0 )
            squadron_manager.firstShownEntries = 0;
    }

    int v5 = squadron_manager.firstShownEntries;
    for (int i = 0; i < (int)yw->_cmdrsRemap.size() + 1; i++ )
    {
        NC_STACK_ypabact *v6;

        if ( i )
            v6 = yw->_cmdrsRemap.at(i - 1);
        else
            v6 = yw->_userRobo;

        if ( i >= v5 && i - v5 < squadron_manager.shownEntries )
            squadron_manager.squads[i - v5] = v6;
    }

    squadron_manager.numEntries = yw->_cmdrsRemap.size() + 1;
}


bool NC_STACK_ypaworld::ypaworld_func64__sub7__sub3__sub2()
{
    NC_STACK_ypabact *bct = _cmdrsRemap[squadron_manager.field_2B4];
    if ( !bct )
        return false;

    if ( bct == squadron_manager.field_2BC )
    {
        if (bct->_status != BACT_STATUS_DEAD && bct->_status != BACT_STATUS_CREATE && bct->_status != BACT_STATUS_BEAM )
            return true;
    }


    for ( NC_STACK_ypabact* &kid : bct->_kidList )
    {
        if ( kid == squadron_manager.field_2BC )
        {
            if ( kid->_status != BACT_STATUS_DEAD && kid->_status != BACT_STATUS_CREATE && kid->_status != BACT_STATUS_BEAM )
                return true;
        }
    }
    return false;
}

void ypaworld_func64__sub7__sub3__sub3(NC_STACK_ypaworld *yw, TClickBoxInf *winpt)
{
    if ( winpt->selected_btn != &squadron_manager
            || winpt->selected_btnID < 8
            || squadron_manager.numEntries <= (int32_t)(winpt->selected_btnID - 8 + squadron_manager.firstShownEntries)
            || squadron_manager.squads[ winpt->selected_btnID - 8 ] == 0
            || squadron_manager.squads[ winpt->selected_btnID - 8 ] == yw->_userRobo )
    {
        if ( squadron_manager.field_2A8 & 2 )
        {
            if ( squadron_manager.field_2BC->_host_station != squadron_manager.field_2BC->_parent )
            {
                NC_STACK_ypabact *v3 = NULL;

                World::RefBactList *kidlist = squadron_manager.field_2BC->_kidRef.PList();
                if (kidlist)
                {
                    World::RefBactList::iterator it = squadron_manager.field_2BC->_kidRef;

                    while(it != kidlist->end())
                    {
                        NC_STACK_ypabact *itbact = *it;
                        it++;

                        bact_arg109 arg109;

                        if ( v3 )
                        {
                            arg109.field_4 = itbact;
                            arg109.field_0 = 4;
                        }
                        else
                        {
                            arg109.field_4 = NULL;
                            arg109.field_0 = 3;

                            v3 = itbact;
                        }
                        v3->ReorganizeGroup(&arg109);
                    }
                }
            }
        }
        else
        {
            bact_arg109 arg109;
            arg109.field_4 = NULL;
            arg109.field_0 = 3;

            squadron_manager.field_2BC->ReorganizeGroup(&arg109);
        }
        yw->_activeCmdrID = squadron_manager.field_2BC->_commandID;
        sub_4C707C(yw);
        yw->sub_4C40AC();
    }
    else
    {
        NC_STACK_ypabact *v6 = squadron_manager.squads[ winpt->selected_btnID - 8 ];
        int v7;
        if ( squadron_manager.field_2BC->IsParentMyRobo() )
            v7 = v6 == squadron_manager.field_2BC;
        else
            v7 = v6 == squadron_manager.field_2BC->_parent;

        if ( !v7 )
        {
            if ( squadron_manager.field_2A8 & 2 )
            {
                if ( squadron_manager.field_2BC->_host_station != squadron_manager.field_2BC->_parent )
                {
                    World::RefBactList *kidlist = squadron_manager.field_2BC->_kidRef.PList();
                    if (kidlist)
                    {
                        World::RefBactList::iterator it = squadron_manager.field_2BC->_kidRef;
                        while ( it != kidlist->end() )
                        {
                            NC_STACK_ypabact *itbact = *it;
                            it++;

                            bact_arg109 arg109;
                            arg109.field_0 = 1;
                            arg109.field_4 = v6;
                            itbact->ReorganizeGroup(&arg109);
                        }
                    }
                }
                else
                {
                    bact_arg109 arg109;
                    arg109.field_0 = 1;
                    arg109.field_4 = v6;
                    squadron_manager.field_2BC->ReorganizeGroup(&arg109);
                }
            }
            else
            {
                bact_arg109 arg109;
                arg109.field_0 = 4;
                arg109.field_4 = squadron_manager.field_2BC;
                v6->ReorganizeGroup(&arg109);
            }
            yw->_activeCmdrID = v6->_commandID;
            sub_4C707C(yw);
            yw->sub_4C40AC();
        }
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub7__sub3__sub4(NC_STACK_ypabact *bact)
{
    if ( bact->_status == BACT_STATUS_NORMAL )
    {
        if ( bact->_status_flg & BACT_STFLAG_ESCAPE )
            SetShowingTooltip(Locale::TIP_FINDER_ESCAPE);
        else if ( bact->_secndTtype )
            SetShowingTooltip(Locale::TIP_FINDER_FIGHTING);
        else if ( bact->_primTtype )
            SetShowingTooltip(Locale::TIP_FINDER_MOVE);
    }
    else if (bact->_status == BACT_STATUS_IDLE)
        SetShowingTooltip(Locale::TIP_FINDER_WAITORDER);
}

NC_STACK_ypabact * NC_STACK_ypaworld::sub_4C7B0C(int sqid, int a3)
{
    NC_STACK_ypabact *v3 = squadron_manager.squads[sqid];

    if ( !v3 )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return NULL;
    }

    if ( v3 == _userRobo )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return _userRobo;
    }

    squadron_manager.field_2B0 = -1;
    squadron_manager.field_2AC = squadron_manager.firstShownEntries + sqid - 1;

    if ( a3 > squadron_manager.field_2D0 && a3 < squadron_manager.field_2D0 + squadron_manager.field_2CC )
        return v3;

    int v5 = a3 - squadron_manager.field_2D4;
    if ( v5 < 0 )
    {
        squadron_manager.field_2B0 = -1;
        squadron_manager.field_2AC = -1;
        return NULL;
    }

    squadron_manager.field_2B0 = v5 / squadron_manager.field_2CC;

    int v6 = squadron_manager.field_2B0;

    for(NC_STACK_ypabact * &bact : v3->_kidList)
    {
        if (bact->_status != BACT_STATUS_DEAD && bact->_status != BACT_STATUS_CREATE && bact->_status != BACT_STATUS_BEAM)
        {
            v6--;
            if (v6 == -1)
            {
                return bact;
            }
        }
    }

    squadron_manager.field_2B0 = -1;
    squadron_manager.field_2AC = -1;
    return NULL;
}

int NC_STACK_ypaworld::ypaworld_func64__sub7__sub3__sub1(TClickBoxInf *winpt)
{
    if ( winpt->selected_btnID < 8 )
        return 0;
    if ( bzda.field_1D0 & 8 )
        return 0;
    if ( (int32_t)(winpt->selected_btnID - 8 + squadron_manager.firstShownEntries) >= squadron_manager.numEntries )
        return 0;

    NC_STACK_ypabact *v5 = sub_4C7B0C(winpt->selected_btnID - 8, winpt->move.BtnPos.x);

    if ( !v5 )
        return 0;

    if ( robo_map.IsOpen() )
    {
        if ( winpt->flag & TClickBoxInf::FLAG_RM_DOWN )
        {
            robo_map.field_1D8 = v5->_position.x;
            robo_map.field_1ED = 0;
            robo_map.field_1DC = v5->_position.z;

            sub_4C1814(this, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
        }
    }

    if ( v5 == _userRobo )
        return 0;

    squadron_manager.field_2B4 = squadron_manager.field_2AC;
    squadron_manager.field_2B8 = squadron_manager.field_2B0;
    squadron_manager.field_2C8 = winpt->move.ScreenPos.x;
    squadron_manager.field_2BC = v5;

    if ( v5->IsParentMyRobo() )
    {
        squadron_manager.field_2C0 = -(winpt->move.BtnPos.x - squadron_manager.field_2D0);
    }
    else
    {
        int v11 = winpt->move.BtnPos.x - squadron_manager.field_2D4;
        squadron_manager.field_2C0 = -(v11 % squadron_manager.field_2CC);
    }

    squadron_manager.field_2C2 = -winpt->move.BtnPos.y;
    squadron_manager.field_2CA = winpt->move.ScreenPos.y;
    return 1;
}

void NC_STACK_ypaworld::SquadManager_InputHandle(TInputState *inpt)
{
    if ( squadron_manager.flags & (GuiBase::FLAG_CLOSED | GuiBase::FLAG_ICONIFED) )
    {
        squadron_manager.field_2A8 &= 0xFFFFFFFE;
        squadron_manager.InputHandle(this, inpt);
    }
    else
    {
        TClickBoxInf *winpt = &inpt->ClickInf;
        sub_4C707C(this);

        if ( squadron_manager.field_2A8 & 1 )
        {
            if ( ypaworld_func64__sub7__sub3__sub2() && (winpt->flag & (TClickBoxInf::FLAG_LM_HOLD | TClickBoxInf::FLAG_RM_HOLD)) != (TClickBoxInf::FLAG_LM_HOLD | TClickBoxInf::FLAG_RM_HOLD) )
            {
                if ( winpt->flag & (TClickBoxInf::FLAG_LM_HOLD | TClickBoxInf::FLAG_RM_HOLD) )
                {
                    squadron_manager.field_2C4 = winpt->move.ScreenPos.x + squadron_manager.field_2C0;
                    squadron_manager.field_2C6 = winpt->move.ScreenPos.y + squadron_manager.field_2C2;
                }
                else
                {
                    ypaworld_func64__sub7__sub3__sub3(this, winpt);
                    squadron_manager.field_2A8 &= 0xFFFFFFFC;
                }
            }
            else
            {
                squadron_manager.field_2A8 &= 0xFFFFFFFC;
            }
        }
        else if ( inpt->ClickInf.selected_btn == &squadron_manager )
        {
            if ( inpt->ClickInf.selected_btnID == 7 )
            {
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    sub_449DE8(Locale::Text::Help(Locale::HELP_FINDERWND));
                }
                SetShowingTooltip(Locale::TIP_ONLINEHELP);
            }
            if ( winpt->selected_btnID == 6 && this->_activeCmdrRemapIndex != -1 && !(bzda.field_1D0 & 0x20) )
            {
                int v9 = (winpt->move.BtnPos.x - squadron_manager.field_2DC) / _guiTiles[24]->map[49].w;

                if ( v9 >= 0 && v9 <= 4 )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                    {
                        _cmdrsRemap[_activeCmdrRemapIndex]->setBACT_aggression(25 * v9);

                        if ( _GameShell )
                            SFXEngine::SFXe.startSound(&_GameShell->samples1_info, 3);
                    }
                }

                switch ( v9 )
                {
                case 0:
                    SetShowingTooltipWithHotkey(Locale::TIP_GUI_AGGR_0, 38);
                    break;

                case 1:
                    SetShowingTooltipWithHotkey(Locale::TIP_GUI_AGGR_1, 39);
                    break;

                case 2:
                    SetShowingTooltipWithHotkey(Locale::TIP_GUI_AGGR_2, 40);
                    break;

                case 3:
                    SetShowingTooltipWithHotkey(Locale::TIP_GUI_AGGR_3, 41);
                    break;

                case 4:
                    SetShowingTooltipWithHotkey(Locale::TIP_GUI_AGGR_4, 42);
                    break;

                default:
                    break;
                }

                if ( winpt->move.BtnPos.x >= squadron_manager.field_2D8 )
                    SetShowingTooltip(Locale::TIP_COUNTINSQUAD);
            }

            if ( winpt->flag & (TClickBoxInf::FLAG_RM_DOWN | TClickBoxInf::FLAG_LM_DOWN) )
            {
                if ( ypaworld_func64__sub7__sub3__sub1(winpt) )
                {
                    squadron_manager.field_2A8 &= 0xFFFFFFFC;
                    squadron_manager.field_2A8 |= 1;

                    if ( winpt->flag & TClickBoxInf::FLAG_RM_DOWN )
                        squadron_manager.field_2A8 |= 3;

                    squadron_manager.field_2C4 = winpt->move.ScreenPos.x + squadron_manager.field_2C0;
                    squadron_manager.field_2C6 = winpt->move.ScreenPos.y + squadron_manager.field_2C2;
                }
            }
            if ( squadron_manager.mouseItem != -1 )
            {
                int v17 = squadron_manager.mouseItem - squadron_manager.firstShownEntries;

                if ( v17 >= 0 && v17 < squadron_manager.maxShownEntries )
                {
                    NC_STACK_ypabact *v18 = squadron_manager.squads[v17];
                    if ( v18 )
                    {
                        if ( winpt->move.BtnPos.x < squadron_manager.field_2D0 )
                            ypaworld_func64__sub7__sub3__sub4(v18);
                    }
                }
            }
        }

        squadron_manager.InputHandle(this, inpt);
        squadron_manager.Formate(this);
        ypaworld_func64__sub7__sub3__sub0(this, inpt);
    }
}


void sub_4C1FBC()
{
    ButtonBox &pMapInter = robo_map.buttons[17];

    int v0 = -(pMapInter.x + robo_map.x + (pMapInter.w / 2));
    int v3 = -(pMapInter.y + robo_map.y + (pMapInter.h / 2));
    flt_516524 = (float)(v0 + dword_516510) * robo_map.field_1E0 + robo_map.field_1D8;
    flt_51652C = (float)(v0 + dword_516518) * robo_map.field_1E0 + robo_map.field_1D8;
    flt_516528 = robo_map.field_1DC - (float)(v3 + dword_516514) * robo_map.field_1E4;
    flt_516530 = robo_map.field_1DC - (float)(v3 + dword_51651C) * robo_map.field_1E4;

    if ( flt_516524 > flt_51652C )
    {
        float v4 = flt_516524;
        flt_516524 = flt_51652C;
        flt_51652C = v4;
    }

    if ( flt_516528 > flt_516530 )
    {
        float v5 = flt_516528;
        flt_516528 = flt_516530;
        flt_516530 = v5;
    }
}

void ypaworld_func64__sub7__sub1__sub0(NC_STACK_ypaworld *yw)
{
    int v8 = 0;

    int v22 = flt_516524 / World::CVSectorLength;
    int v6 = -flt_516530 / World::CVSectorLength;
    int v24 = flt_51652C / World::CVSectorLength;
    int v23 = -flt_516528 / World::CVSectorLength;

    if ( v22 >= yw->_mapSize.x )
        v22 = yw->_mapSize.x - 1;
    else if ( v22 < 1 )
        v22 = 1;

    if ( v24 >= yw->_mapSize.x )
        v24 = yw->_mapSize.x - 1;
    else if ( v24 < 1 )
        v24 = 1;

    if ( v6 >= yw->_mapSize.y )
        v6 = yw->_mapSize.y - 1;
    else if ( v6 < 1 )
        v6 = 1;

    if ( v23 >= yw->_mapSize.y )
        v23 = yw->_mapSize.y - 1;
    else if ( v23 < 1 )
        v23 = 1;

    NC_STACK_ypabact *v13 = NULL;

    for (int i = v6; i <= v23; i++)
    {
        for (int j = v22; j <= v24; j++)
        {
            cellArea &cll = yw->_cells(j, i);

            if ( robo_map.MapViewMask & cll.view_mask )
            {
                for ( NC_STACK_ypabact* &bct : cll.unitsList )
                {
                    if ( bct->_owner == yw->_userRobo->_owner )
                    {
                        if ( bct->_position.x > flt_516524 && bct->_position.z > flt_516528 && bct->_position.x < flt_51652C && bct->_position.z < flt_516530 )
                        {
                            if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_GUN )
                            {
                                if ( bct->_status != BACT_STATUS_CREATE && bct->_status != BACT_STATUS_BEAM && bct->_status != BACT_STATUS_DEAD )
                                {

                                    if ( v13 )
                                    {
                                        if ( bct->_bact_type != BACT_TYPES_ROBO )
                                        {
                                            bact_arg109 arg109;
                                            arg109.field_4 = bct;
                                            arg109.field_0 = 4;

                                            v13->ReorganizeGroup(&arg109);

                                            v8 = 0;
                                        }
                                    }
                                    else
                                    {
                                        if ( bct->_bact_type == BACT_TYPES_ROBO )
                                        {
                                            if ( bct == yw->_userRobo )
                                                v8 = 1;
                                        }
                                        else
                                        {
                                            v13 = bct;

                                            bact_arg109 arg109;
                                            arg109.field_4 = NULL;
                                            arg109.field_0 = 3;

                                            v13->ReorganizeGroup(&arg109);

                                            v8 = 0;
                                        }
                                    }


                                }
                            }
                        }
                    }
                }
            }

        }
    }

    if ( v8 )
        bzda.field_1D0 = bzda.field_1CC & 0x20;

    if ( v13 )
        yw->_activeCmdrID = v13->_commandID;

    yw->sub_4C40AC();
}

void  RoboMap_InputHandle(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    if ( robo_map.IsClosed() )
    {
        robo_map.field_1E8 &= 0xFFFFFDE8;
    }
    else
    {
        robo_map.MapViewMask = cellArea::ViewMask( yw->_userRobo->_owner );

        TClickBoxInf *winpt = &inpt->ClickInf;

        if ( yw->_showDebugMode )
            robo_map.MapViewMask = -1;

        switch ( inpt->HotKeyID )
        {
        case 10:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 3;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
            break;

        case 11:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 4;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
            break;

        case 12:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 5;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
            break;

        case 14:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 6;
            winpt->flag |= TClickBoxInf::FLAG_BTN_DOWN;
            break;

        case 16:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 7;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
            break;

        case 17:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 8;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
            break;

        case 18:
            winpt->selected_btn = &robo_map;
            winpt->selected_btnID = 9;
            winpt->flag |= TClickBoxInf::FLAG_BTN_UP;
            break;

        default:
            break;
        }

        if ( robo_map.field_1E8 & 1 )
        {
            if ( winpt->flag & TClickBoxInf::FLAG_LM_HOLD )
            {
                int v15 = winpt->move.ScreenPos.x + robo_map.field_218;
                int v16 = winpt->move.ScreenPos.y + robo_map.field_21A;

                if ( v15 < robo_map.field_210 )
                    v15 = robo_map.field_210;
                else if ( v15 > robo_map.field_214 )
                    v15 = robo_map.field_214;

                if ( v16 < robo_map.field_212 )
                    v16 = robo_map.field_212;
                else if ( v16 > robo_map.field_216 )
                    v16 = robo_map.field_216;

                if ( v15 + robo_map.x >= yw->_screenSize.x )
                    v15 = yw->_screenSize.x - robo_map.x;

                if ( robo_map.y + v16 >= yw->_screenSize.y - yw->_downScreenBorder )
                    v16 = yw->_screenSize.y - yw->_downScreenBorder - robo_map.y;

                robo_map.h = v16;
                robo_map.w = v15;
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFE;
                yw->_guiDragDefaultMouse = false;
            }
        }
        else if ( robo_map.field_1E8 & 2 )
        {
            if ( winpt->flag & TClickBoxInf::FLAG_LM_HOLD )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1D8 = (yw->_mapSize.x * World::CVSectorLength) / (float)robo_map.field_1CC * (float)(winpt->move.ScreenPos.x - robo_map.field_21C ) + robo_map.field_220;
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFD;
                yw->_guiDragDefaultMouse = false;
            }
        }
        else if ( robo_map.field_1E8 & 4 )
        {
            if ( winpt->flag & TClickBoxInf::FLAG_LM_HOLD )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1DC = robo_map.field_224 - (yw->_mapSize.y * World::CVSectorLength) / (float)robo_map.field_1D2 * (float)(winpt->move.ScreenPos.y - robo_map.field_21E);
            }
            else
            {
                robo_map.field_1E8 &= 0xFFFFFFFB;
                yw->_guiDragDefaultMouse = false;
            }
        }
        else if ( robo_map.field_1E8 & 0x10 )
        {
            if ( winpt->flag & TClickBoxInf::FLAG_RM_HOLD )
            {
                robo_map.field_1ED = 0;
                robo_map.field_1D8 = robo_map.field_220 + (float)(robo_map.field_21C - winpt->move.ScreenPos.x) * robo_map.field_1E0;
                robo_map.field_1DC = robo_map.field_224 - (float)(robo_map.field_21E - winpt->move.ScreenPos.y) * robo_map.field_1E4;
            }
            else
            {
                int v25 = abs(robo_map.field_21C - winpt->move.ScreenPos.x);
                int v27 = abs(robo_map.field_21E - winpt->move.ScreenPos.y);

                if ( v25 <= 1 && v27 <= 1 )
                {
                    yw->_activeCmdrRemapIndex = -1;
                    yw->_activeCmdrID = 0;

                    if ( bzda.field_1D0 & 0x28 )
                        bzda.field_1D0 = bzda.field_1CC & 1;
                }
                robo_map.field_1E8 &= 0xFFFFFFEF;
                yw->_guiDragDefaultMouse = false;
            }
        }
        else if ( robo_map.field_1E8 & 0x200 )
        {
            dword_516518 = winpt->move.ScreenPos.x;
            dword_51651C = winpt->move.ScreenPos.y;

            sub_4C1FBC();

            if ( !(winpt->flag & TClickBoxInf::FLAG_LM_HOLD) )
            {
                if ( abs(dword_516518 - dword_516510) <= 5 || abs(dword_51651C - dword_516514) <= 5 )
                {
                    robo_map.field_1E8 &= 0xFFFFFDFF;
                    winpt->flag |= TClickBoxInf::FLAG_LM_DOWN;
                }
                else
                {
                    bzda.field_1D0 = 1;
                    robo_map.field_1E8 &= 0xFFFFFDFF;
                    ypaworld_func64__sub7__sub1__sub0(yw);
                }
            }
        }
        else if ( winpt->selected_btn == &robo_map )
        {
            if ( winpt->flag & TClickBoxInf::FLAG_RM_DOWN )
            {
                if ( winpt->selected_btnID == 17 )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_21C = winpt->move.ScreenPos.x;
                    robo_map.field_21E = winpt->move.ScreenPos.y;
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1E8 |= 0x10;
                    yw->_guiDragDefaultMouse = true;
                }
            }

            if ( winpt->flag & TClickBoxInf::FLAG_LM_DOWN )
            {
                if ( winpt->selected_btnID == 17 )
                {
                    robo_map.field_1E8 |= 0x200;
                    dword_516510 = winpt->move.ScreenPos.x;
                    dword_516514 = winpt->move.ScreenPos.y;
                    dword_516518 = dword_516510;
                    dword_51651C = dword_516514;

                    sub_4C1FBC();

                    winpt->flag &= ~TClickBoxInf::FLAG_LM_DOWN;
                }
            }

            if ( winpt->selected_btnID > 1 && winpt->selected_btnID <= 9 )
            {
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                {
                    if ( yw->_GameShell )
                        SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
                }
            }

            if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
            {
                switch ( winpt->selected_btnID )
                {
                case 16:
                    robo_map.field_218 = robo_map.w - winpt->ldw_pos.ScreenPos.x;
                    robo_map.field_21A = robo_map.h - winpt->ldw_pos.ScreenPos.y;

                    robo_map.field_1E8 |= 1;
                    yw->_guiDragDefaultMouse = true;
                    break;

                case 14:
                    robo_map.field_21C = winpt->ldw_pos.ScreenPos.x;
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1E8 |= 2;
                    yw->_guiDragDefaultMouse = true;
                    break;

                case 11:
                    robo_map.field_21E = winpt->ldw_pos.ScreenPos.y;
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1E8 |= 4;
                    yw->_guiDragDefaultMouse = true;
                    break;

                case 3:
                    if ( robo_map.field_1EC & 1 )
                        robo_map.field_1EC &= 0xFE;
                    else
                        robo_map.field_1EC |= 1;

                    sub_4C1970(yw, 0);
                    break;

                case 4:
                    if ( robo_map.field_1EC & 2 )
                        robo_map.field_1EC &= 0xFD;
                    else
                        robo_map.field_1EC |= 2;

                    sub_4C1970(yw, 0);
                    break;

                case 5:
                    if ( robo_map.field_1EC & 4 )
                        robo_map.field_1EC &= 0xFB;
                    else
                        robo_map.field_1EC |= 4;

                    sub_4C1970(yw, 0);
                    break;

                case 6:
                    robo_map.field_1ED = robo_map.field_1ED != 1;
                    break;

                default:
                    break;
                }
            }

            switch ( winpt->selected_btnID )
            {
            case 13:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1ED = 0;
                    robo_map.field_1D8 = yw->_mapSize.x * World::CVSectorLength / (-1.0 * (float)robo_map.field_1CC) + robo_map.field_1D8;
                }
                break;

            case 15:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                {
                    robo_map.field_220 = robo_map.field_1D8;
                    robo_map.field_1ED = 0;
                    robo_map.field_1D8 = yw->_mapSize.x * World::CVSectorLength / (float)robo_map.field_1CC + robo_map.field_1D8;
                }
                break;

            case 10:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                {
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1ED = 0;
                    robo_map.field_1DC = robo_map.field_1DC - yw->_mapSize.y * World::CVSectorLength / (-1.0 * (float)robo_map.field_1D2);
                }
                break;

            case 12:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                {
                    robo_map.field_224 = robo_map.field_1DC;
                    robo_map.field_1ED = 0;
                    robo_map.field_1DC = robo_map.field_1DC - yw->_mapSize.y * World::CVSectorLength / (float)robo_map.field_1D2;
                }
                break;

            case 9:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    robo_map.field_1E8 |= 0x20;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    int tmp1, tmp2, tmp3, tmp4;
                    tmp1 = robo_map.field_208;
                    tmp2 = robo_map.field_20A;
                    tmp3 = robo_map.field_20C;
                    tmp4 = robo_map.field_20E;

                    robo_map.field_208 = robo_map.x;
                    robo_map.field_20A = robo_map.y;
                    robo_map.field_20C = robo_map.w;
                    robo_map.field_20E = robo_map.h;

                    robo_map.x = tmp1;
                    robo_map.y = tmp2;
                    robo_map.w = tmp3;
                    robo_map.h = tmp4;
                }
                break;

            case 2:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    robo_map.flags |= GuiBase::FLAG_HELP_DOWN;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    robo_map.flags &= ~GuiBase::FLAG_HELP_DOWN;
                    yw->sub_449DE8(Locale::Text::Help(Locale::HELP_MAPWND)); //MAKE ME
                }

                yw->SetShowingTooltip(Locale::TIP_ONLINEHELP);
                break;

            case 7:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    robo_map.field_1E8 |= 0x40;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    sub_4C1970(yw, 1);

                break;

            case 8:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    robo_map.field_1E8 |= 0x80;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    sub_4C1970(yw, 2);

                break;

            default:
                break;
            }

            switch ( winpt->selected_btnID )
            {
            case 3:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_DETAIL, 10);
                break;

            case 4:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_OWNERS, 11);
                break;

            case 5:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_EINFO, 12);
                break;

            case 6:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_LOCKVIEWER, 14);
                break;

            case 7:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_ZOOMIN, 16);
                break;

            case 8:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_ZOOMOUT, 17);
                break;

            case 9:
                yw->SetShowingTooltipWithHotkey(Locale::TIP_MAP_SIZE, 18);
                break;

            default:
                break;
            }
        }
    }
}

char * ypaworld_func64__sub7__sub0__sub0__sub0(NC_STACK_ypaworld *yw, char *cur, int id)
{
    int v5 = id + info_log.field_254 + info_log.firstShownEntries;

    if ( v5 >= 64 )
        v5 -= 64;
    else if ( v5 < 0 )
        v5 += 64;

    char *pcur = cur;

    FontUA::store_u8(&pcur, 123);

    pcur = FontUA::FormateClippedText(yw->_guiTiles[0], pcur, info_log.msgs[v5].txt, info_log.entryWidth - 2 * yw->_fontBorderW, 32);

    FontUA::store_u8(&pcur, 125);
    FontUA::next_line(&pcur);

    return pcur;
}

void ypaworld_func64__sub7__sub0__sub0(NC_STACK_ypaworld *yw)
{
    char *pcur = info_log.ItemsPreLayout(yw, info_log.itemBlock, 0, "{ }");

    for (int i = 0; i < info_log.shownEntries; i++ )
        pcur = ypaworld_func64__sub7__sub0__sub0__sub0(yw, pcur, i);

    pcur = info_log.ItemsPostLayout(yw, pcur, 0, "xyz");

    FontUA::set_end(&pcur);
}

void ypaworld_func64__sub7__sub0(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    if ( info_log.flags & (GuiBase::FLAG_CLOSED | GuiBase::FLAG_ICONIFED) )
    {
        info_log.InputHandle(yw, inpt);
    }
    else
    {
        info_log.InputHandle(yw, inpt);
        info_log.Formate(yw);
        ypaworld_func64__sub7__sub0__sub0(yw);
    }
}


GuiBase * ypaworld_func64__sub7__sub6__sub1()
{
    return dword_5BAFAC;
}

void NC_STACK_ypaworld::NetSendExitMsg(uint8_t normExit)
{
    uamessage_exit exMsg;
    exMsg.msgID = UAMSG_EXIT;
    exMsg.owner = _GameShell->netPlayerOwner;
    exMsg.norm = normExit;

    NetBroadcastMessage(&exMsg, sizeof(exMsg), true);

    _GameShell->sentAQ = false;
}

char * sub_451714(TileMap *, char *cur, const std::string &txt, int a2, uint8_t a4)
{
    char *pcur = cur;

    if ( a2 > 0 )
    {
        FontUA::copy_position(&pcur);

        int v6 = a2;
        while (v6 > 0)
        {
            if ( v6 <= 255 )
                FontUA::op10(&pcur, v6);
            else
                FontUA::op10(&pcur, 255);

            FontUA::store_u8(&pcur, a4);

            v6 -= 255;
        }

        FontUA::add_txt(&pcur, a2, 4, txt);
    }
    return pcur;
}


char *sub_4DA8DC(NC_STACK_ypaworld *yw, char *cur, int a4, int a3, const std::string &a5)
{
    std::string v5 = a5;

    char *pcur = cur;

    FontUA::set_yheight(&pcur, dword_5C8B84);

    FontUA::store_u8(&pcur, 123);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    FontUA::op17(&pcur, dword_5C8B80 + dword_5C8B88);
    FontUA::store_u8(&pcur, 32);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    FontUA::store_u8(&pcur, 123);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    int v11;

    if ( a4 )
        v11 = 4;
    else if ( a3 )
        v11 = 8;
    else
        v11 = 3;

    FontUA::select_tileset(&pcur, v11);
    FontUA::store_u8(&pcur, 98);

    if ( a4 )
        v5 = " ";

    if ( a3 )
    {
        FontUA::set_flag(&pcur, 0x10);
    }

    pcur = sub_451714(yw->_guiTiles[v11], pcur, v5, dword_5C8B88 - 2 * yw->_fontBorderW, 32);

    FontUA::unset_flag(&pcur, 0x10);

    FontUA::store_u8(&pcur, 100);

    FontUA::select_tileset(&pcur, 0);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    FontUA::set_yheight(&pcur, dword_5C8B84);

    FontUA::store_u8(&pcur, 123);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    FontUA::op17(&pcur, dword_5C8B80 + dword_5C8B88);
    FontUA::store_u8(&pcur, 32);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    return pcur;
}



void ypaworld_func64__sub7__sub6__sub3(NC_STACK_ypaworld *yw, int a2, int a4)
{
    char *pcur = exit_menu.ItemsPreLayout(yw, exit_menu.itemBlock, 0, "{ }");

    FontUA::set_txtColor(&pcur, yw->_iniColors[68].r, yw->_iniColors[68].g, yw->_iniColors[68].b);

    pcur = sub_4DA8DC(yw, pcur, a4 & 0x100, a2 & 0x100, Locale::Text::Common(Locale::CMN_EXITMISN));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x200, a2 & 0x200, Locale::Text::Common(Locale::CMN_SAVE));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x400, a2 & 0x400, Locale::Text::Common(Locale::CMN_LOAD));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x800, a2 & 0x800, Locale::Text::Common(Locale::CMN_RESTART));
    pcur = sub_4DA8DC(yw, pcur, a4 & 0x1000, a2 & 0x1000, Locale::Text::Common(Locale::CMN_CONTINUE));

    pcur = exit_menu.ItemsPostLayout(yw, pcur, 0, "xyz");

    FontUA::set_end(&pcur);
}


void NC_STACK_ypaworld::ypaworld_func64__sub7__sub6(TInputState *inpt)
{
    if ( exit_menu.IsClosed() )
    {
        if ( _levelInfo.State != TLevelInfo::STATE_PLAYING )
        {
            if ( _levelInfo.State == TLevelInfo::STATE_SAVE )
            {
                yw_arg159 v18;
                v18.unit = 0;
                v18.Priority = 10;
                v18.txt = Locale::Text::Feedback(Locale::FEEDBACK_GAMESAVED);
                v18.MsgID = 0;

                ypaworld_func159(&v18);
            }

            _levelInfo.State = TLevelInfo::STATE_PLAYING;
        }
    }
    else
    {
        if ( _levelInfo.State != TLevelInfo::STATE_PAUSED )
        {
            _levelInfo.State = TLevelInfo::STATE_PAUSED;

            if ( _GameShell )
            {
                _hasSGMSave = UserData::IsHasSGM(_GameShell->UserName, 0);

                if ( UserData::IsHasRestartForLevel(_GameShell->UserName, _levelInfo.LevelID) )
                    _hasRSTSave = true;
                else
                    _hasRSTSave = false;
            }
        }
    }

    if ( ypaworld_func64__sub7__sub6__sub1() == &exit_menu )
    {
        int v7 = sub_4C885C();

        if ( v7 == 1 )
        {
            switch ( dword_5C8B78 )
            {
            case 8:
                _levelInfo.State = TLevelInfo::STATE_ABORTED;

                if ( _isNetGame )
                    NetSendExitMsg(1);

                break;

            case 9:
                _levelInfo.State = TLevelInfo::STATE_SAVE;
                break;

            case 10:
                _levelInfo.State = TLevelInfo::STATE_LOAD;
                break;

            case 11:
                _levelInfo.State = TLevelInfo::STATE_RESTART;
                break;

            case 13:
                _helpURL = _helpSavedURL;
                _helpSavedURL.clear();
                break;

            default:
                break;
            }

            dword_5BAF9C = 0;
        }
        else if ( v7 == 2 )
        {
            dword_5BAF9C = 0;
        }
    }

    if ( exit_menu.IsClosed() )
    {
        exit_menu.InputHandle(this, inpt);
    }
    else
    {
        TClickBoxInf *winpt = &inpt->ClickInf;

        int a2 = 0;
        int a4 = 0;

        if ( !_hasSGMSave )
            a4 = 1024;

        if ( !_hasRSTSave )
            a4 |= 0x800;

        if ( _userUnit->_status == BACT_STATUS_DEAD )
            a4 |= 0x200;

        if ( _isNetGame )
            a4 |= 0xE00;

        if ( inpt->ClickInf.selected_btn == &exit_menu )
        {
            if ( _GameShell )
            {
                if ( inpt->ClickInf.selected_btnID >= 8 )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                        SFXEngine::SFXe.startSound(&_GameShell->samples1_info, 3);
                }
            }

            switch ( inpt->ClickInf.selected_btnID )
            {
            case 7:
                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    sub_449DE8(Locale::Text::Help(Locale::HELP_GAMEMENU));

                SetShowingTooltip(Locale::TIP_ONLINEHELP);
                break;

            case 8:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    a2 |= 0x100;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    dword_5C8B78 = 8;

                    if ( sub_4C885C() != 3 )
                        GuiWinClose( &exit_menu );

                    sb_0x4c87fc( Locale::Text::Advanced(Locale::ADV_RLYEXIT) , &exit_menu);
                }
                break;

            case 9:
                if ( !(a4 & 0x200) )
                {
                    if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                        a2 |= 0x200;

                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    {
                        dword_5C8B78 = 9;

                        if ( sub_4C885C() != 3 )
                            GuiWinClose( &exit_menu );

                        sb_0x4c87fc( Locale::Text::Advanced(Locale::ADV_RLYSAVE) , &exit_menu);
                    }
                }
                break;

            case 10:
                if ( !(a4 & 0x400) )
                {
                    if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                        a2 |= 0x400;

                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    {
                        dword_5C8B78 = 10;

                        if ( sub_4C885C() != 3 )
                            GuiWinClose( &exit_menu );

                        sb_0x4c87fc( Locale::Text::Advanced(Locale::ADV_RLYLOAD) , &exit_menu);
                    }
                }
                break;

            case 11:
                if ( !(a4 & 0x800) )
                {
                    if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                        a2 |= 0x800;

                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    {
                        dword_5C8B78 = 11;

                        if ( sub_4C885C() != 3 )
                            GuiWinClose( &exit_menu );

                        sb_0x4c87fc(Locale::Text::Advanced(Locale::ADV_RLYRESTART) , &exit_menu);
                    }
                }
                break;

            case 12:
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    a2 |= 0x1000;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                    GuiWinClose( &exit_menu );

                break;

            default:
                break;
            }
        }
        exit_menu.InputHandle(this, inpt);
        exit_menu.Formate(this);

        ypaworld_func64__sub7__sub6__sub3(this, a2, a4);
    }
}




char * sub_4C8534(NC_STACK_ypaworld *yw, char *cur, const std::string &a2)
{
    char *pcur = cur;

    FontUA::store_u8(&pcur, 123);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    pcur = sub_451714(yw->_guiTiles[0], pcur, a2, lstvw2.entryWidth - 2 * dword_5BAFA8, 32);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    return pcur;
}

char * ypaworld_func64__sub7__sub4__sub0__sub0(NC_STACK_ypaworld *yw, char *cur, int a3)
{
    char *pcur = cur;

    FontUA::store_u8(&pcur, 123);
    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    int v8;

    if ( a3 & 0x100 )
    {
        FontUA::set_flag(&pcur, 0x10);
        v8 = 8;
    }
    else
    {
        v8 = 3;
    }

    FontUA::select_tileset(&pcur, v8);

    FontUA::store_u8(&pcur, 98);

    pcur = sub_451714(yw->_guiTiles[v8], pcur, Locale::Text::Common(Locale::CMN_OK), dword_5BAFA0 - 2 * yw->_fontBorderW, 32);

    FontUA::unset_flag(&pcur, 0x10);

    FontUA::store_u8(&pcur, 100);

    FontUA::select_tileset(&pcur, 0);

    FontUA::op10(&pcur, dword_5BAFA4);

    FontUA::store_u8(&pcur, 32);

    if ( a3 & 0x200 )
    {
        FontUA::set_flag(&pcur, 0x10);
        v8 = 8;
    }
    else
    {
        v8 = 3;
    }

    FontUA::select_tileset(&pcur, v8);

    FontUA::store_u8(&pcur, 98);

    pcur = sub_451714(yw->_guiTiles[v8], pcur, Locale::Text::Common(Locale::CMN_CANCEL), dword_5BAFA0 - 2 * yw->_fontBorderW, 32);

    FontUA::unset_flag(&pcur, 0x10);

    FontUA::store_u8(&pcur, 100);

    FontUA::select_tileset(&pcur, 0);

    FontUA::store_u8(&pcur, 32);
    FontUA::store_u8(&pcur, 32);

    FontUA::store_u8(&pcur, 125);

    FontUA::next_line(&pcur);

    return pcur;
}

void NC_STACK_ypaworld::ypaworld_func64__sub7__sub4__sub0(int a2)
{
    char *pcur = lstvw2.ItemsPreLayout(this, lstvw2.itemBlock, 0, "{ }");

    FontUA::set_txtColor(&pcur, _iniColors[60].r, _iniColors[60].g, _iniColors[60].b);

    pcur = sub_4C8534(this, pcur, dword_5BAF98);
    pcur = sub_4C8534(this, pcur, " ");

    FontUA::set_txtColor(&pcur, _iniColors[68].r, _iniColors[68].g, _iniColors[68].b);

    pcur = ypaworld_func64__sub7__sub4__sub0__sub0(this, pcur, a2);
    pcur = lstvw2.ItemsPostLayout(this, pcur, 0, "xyz");

    FontUA::set_end(&pcur);
}

void NC_STACK_ypaworld::ypaworld_func64__sub7__sub4(TInputState *inpt)
{
    if ( lstvw2.IsClosed() )
    {
        lstvw2.InputHandle(this, inpt);
    }
    else
    {
        TClickBoxInf *winpt = &inpt->ClickInf;

        int a1_1 = 0;

        if ( _kbdLastKeyHit == Input::KC_RETURN )
        {
            GuiWinClose( &lstvw2 );

            dword_5BAF9C = 1;

            inpt->KbdLastHit = Input::KC_NONE;
            inpt->KbdLastDown = Input::KC_NONE;
            inpt->HotKeyID = -1;
        }
        else if ( _kbdLastKeyHit == Input::KC_ESCAPE )
        {
            GuiWinClose( &lstvw2 );

            dword_5BAF9C = 2;

            inpt->KbdLastHit = Input::KC_NONE;
            inpt->KbdLastDown = Input::KC_NONE;
            inpt->HotKeyID = -1;
        }
        else if ( inpt->ClickInf.selected_btn == &lstvw2 )
        {
            if ( _GameShell )
            {
                if ( inpt->ClickInf.selected_btnID >= 8 )
                {
                    if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                        SFXEngine::SFXe.startSound(&_GameShell->samples1_info, 3);
                }
            }

            if ( winpt->selected_btnID == 8 )
            {
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    a1_1 = 0x100;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    GuiWinClose( &lstvw2 );
                    dword_5BAF9C = 1;
                }
            }
            else if ( winpt->selected_btnID == 9 )
            {
                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                    a1_1 = 0x200;

                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                {
                    GuiWinClose( &lstvw2 );
                    dword_5BAF9C = 2;
                }
            }
        }

        lstvw2.InputHandle(this, inpt);
        lstvw2.Formate(this);
        ypaworld_func64__sub7__sub4__sub0(a1_1);
    }
}


void NC_STACK_ypaworld::ypaworld_func64__sub7(TInputState *inpt)
{
    TClickBoxInf *winpt = &inpt->ClickInf;

    if ( _userUnit->_bact_type != BACT_TYPES_MISSLE )
    {
        if ( winpt->flag & TClickBoxInf::FLAG_OK )
        {
            for(GuiBaseList::iterator it = _guiActive.begin(); it != _guiActive.end(); it++)
            {
                GuiBase *llst = *it;

                if ( llst->flags & GuiBase::FLAG_WITH_ICON )
                {
                    if ( llst->flags & GuiBase::FLAG_ICONIFED )
                        llst->iconString[2] = 1;
                }

                if ( llst->flags & GuiBase::FLAG_WITH_CLOSE )
                {
                    if ( !( llst->flags & (GuiBase::FLAG_ICONIFED | GuiBase::FLAG_CLOSED) ) )
                        llst->flags &= ~GuiBase::FLAG_CLOSE_DOWN;
                }
            }

            if ( inpt->ClickInf.selected_btn )
            {
                GuiBase *v9 = (GuiBase *)inpt->ClickInf.selected_btn->pobject;
                if ( v9 )
                {
                    if ( v9->flags & 1 )
                    {
                        if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                            v9->iconString[2] = 2;

                        if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                        {
                            v9->flags &= 0xFFFFFFFE;

                            Input::Engine.RemClickBox(&v9->iconBox);
                            Input::Engine.AddClickBoxFront(v9);

                            v9->Attach(_guiActive);
                        }
                    }
                    else
                    {
                        if ( winpt->flag & TClickBoxInf::FLAG_LM_DOWN )
                        {
                            Input::Engine.RemClickBox(v9);
                            Input::Engine.AddClickBoxFront(v9);

                            v9->Attach(_guiActive);
                        }

                        if ( v9->flags & 0x10 )
                        {
                            if ( winpt->selected_btnID == 0)
                            {
                                if ( winpt->flag & (TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD) )
                                    v9->flags |= 0x40;

                                if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                                {
                                    if ( _GameShell )
                                        SFXEngine::SFXe.startSound(&_GameShell->samples1_info, 3);
                                }

                                if ( winpt->flag & TClickBoxInf::FLAG_BTN_UP )
                                {
                                    Input::Engine.RemClickBox(v9);

                                    if ( v9->flags & 4 )
                                    {
                                        Input::Engine.AddClickBoxFront(&v9->iconBox);

                                        v9->flags |= 1;
                                    }
                                    else
                                    {
                                        v9->flags |= 0x20;
                                    }

                                    v9->Attach(_guiActive);
                                }
                            }
                        }

                        if ( v9->flags & 8 )
                        {
                            if ( winpt->flag & TClickBoxInf::FLAG_BTN_DOWN )
                            {
                                if ( winpt->selected_btnID == 1 )
                                {
                                    _guiDragDefaultMouse = true;
                                    _guiDragElement = v9;
                                    _guiDragging = true;
                                    _guiDragPos.x = winpt->ldw_pos.BoxPos.x;
                                    _guiDragPos.y = winpt->ldw_pos.BoxPos.y;
                                }
                            }
                        }
                    }
                }
            }

            if ( _guiDragging )
            {
                if ( winpt->flag & TClickBoxInf::FLAG_LM_HOLD )
                {
                    int v13 = winpt->move.ScreenPos.x - _guiDragPos.x;
                    int v15 = winpt->move.ScreenPos.y - _guiDragPos.y;

                    if ( v13 >= 0 )
                    {
                        if ( _guiDragElement->w + v13 > _screenSize.x )
                            v13 = _screenSize.x - _guiDragElement->w;
                    }
                    else
                    {
                        v13 = 0;
                    }

                    if ( v15 >= _upScreenBorder )
                    {
                        if ( _guiDragElement->h + v15 > _screenSize.y - _downScreenBorder )
                            v15 = _screenSize.y - _downScreenBorder - _guiDragElement->h;
                    }
                    else
                    {
                        v15 = _upScreenBorder;
                    }

                    _guiDragElement->y = v15;
                    _guiDragElement->x = v13;

                    char *tmp = &_guiDragElement->cmdstrm.cmdbuf[5];
                    FontUA::store_s16(&tmp, v13 - (_screenSize.x / 2)); //HACKY UPDATE!

                    tmp = &_guiDragElement->cmdstrm.cmdbuf[9];
                    FontUA::store_s16(&tmp, v15 - (_screenSize.y / 2)); //HACKY UPDATE!
                }
            }

            if ( _guiDragging )
            {
                if ( !(winpt->flag & TClickBoxInf::FLAG_LM_HOLD) )
                {
                    _guiDragDefaultMouse = false;
                    _guiDragging = false;
                }
            }

            if ( _isNetGame )
                ypaworld_func64__sub7__sub5(this, inpt);

            ypaworld_func64__sub7__sub4(inpt);
            ypaworld_func64__sub7__sub6(inpt);
            RoboMap_InputHandle(this, inpt);
            SquadManager_InputHandle(inpt);
            ypaworld_func64__sub7__sub0(this, inpt);
            ypaworld_func64__sub7__sub7(this, inpt);
        }
        ypaworld_func64__sub7__sub2(this, inpt);
    }
}










NC_STACK_ypabact * NC_STACK_ypaworld::GetLastMsgSender()
{
    if ( !info_log.field_255C )
        return NULL;

    for (NC_STACK_ypabact *comm : _cmdrsRemap)
    {
        NC_STACK_ypabact *found = NULL;
        
        if ( info_log.field_255C == comm->_gid )
        {
            found = comm;
        }
        else
        {
            for (NC_STACK_ypabact *bact : comm->_kidList)
            {
                if ( bact->_status != BACT_STATUS_DEAD && 
                     bact->_status != BACT_STATUS_CREATE && 
                     bact->_status != BACT_STATUS_BEAM && 
                     bact->_gid == info_log.field_255C )
                {
                    found = bact;
                    break;
                }
            }
        }

        if (found)
        {
            if ( (_timeStamp - info_log.field_2560) > 10000 )
                return NULL;
            
            return found;
        }
    }

    return NULL;
}

bool sub_4C3E34(NC_STACK_ypabact* const &a1, NC_STACK_ypabact* const &a2)
{
    return (a1->_commandID & 0xFFFFFF) < (a2->_commandID & 0xFFFFFF);
}


void NC_STACK_ypaworld::sub_4C40AC()
{
    _cmdrsRemap.clear();
    _cmdrsRemap.reserve(128);
    if ( _userRobo )
    {
        for ( NC_STACK_ypabact * &bact : _userRobo->GetKidList())
        {
            if (bact->_status != BACT_STATUS_DEAD && bact->_status != BACT_STATUS_BEAM && bact->_bact_type != BACT_TYPES_GUN)
                _cmdrsRemap.push_back(bact);
        }
    }

    std::sort(_cmdrsRemap.begin(), _cmdrsRemap.end(), sub_4C3E34);

    if ( !_cmdrsRemap.empty() )
    {
        bool selected = false;
        if (_cmdrIdToSelect != -1)
        {
            for (size_t i = 0; i < _cmdrsRemap.size(); i++)
            {
                if (_cmdrIdToSelect == (int32_t)_cmdrsRemap[i]->_commandID)
                {
                    _activeCmdrRemapIndex = i;
                    _activeCmdrID = _cmdrIdToSelect;
                    selected = true;
                    break;
                }
            }     
        }
        
        if (!selected)
        {
            if ( _activeCmdrID == 0 )
            {
                if ( bzda.field_1D0 & 6 )
                {
                    _activeCmdrID = _cmdrsRemap.back()->_commandID;
                    _activeCmdrRemapIndex = _cmdrsRemap.size() - 1;
                }
            }
            else
            {
                _activeCmdrRemapIndex = -1;

                for (size_t i = 0; i < _cmdrsRemap.size(); i++)
                {
                    if (_activeCmdrID == _cmdrsRemap[i]->_commandID)
                    {
                        _activeCmdrRemapIndex = i;
                        break;
                    }
                }                
            }
        }
    }
    else
    {
        _activeCmdrRemapIndex = -1;
    }
    
    _cmdrIdToSelect = -1;

    if ( _activeCmdrRemapIndex == -1 )
    {
        _activeCmdrKidsCount = 0;
    }
    else
    {
        _activeCmdrKidsCount = 0;
        for ( NC_STACK_ypabact* &bact : _cmdrsRemap[ _activeCmdrRemapIndex ]->_kidList )
        {
            if (bact->_status != BACT_STATUS_DEAD && bact->_status != BACT_STATUS_BEAM && bact->_bact_type != BACT_TYPES_GUN)
                _activeCmdrKidsCount++;
        }
    }
    _lastMsgSender = GetLastMsgSender();
}

void ypaworld_func64__sub14(NC_STACK_ypaworld *yw)
{
    if ( robo_map.IsOpen() )
    {
        sub_4C157C(yw);
        sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
        sub_4C0C00(yw);
        sub_4C0FEC(yw);
        sub_4C1214(yw);
        MapRoboUpdateMapInt(yw);
    }
}

void ypaworld_func64__sub8(NC_STACK_ypaworld *yw)
{
    if ( robo_map.IsOpen() )
    {
        if ( !(robo_map.field_1E8 & 0x16) && robo_map.field_1ED == 1 )
        {
            if ( yw->_viewerBact )
            {
                robo_map.field_1D8 = yw->_viewerBact->_position.x;
                robo_map.field_1DC = yw->_viewerBact->_position.z;

                sub_4C1814(yw, robo_map.field_1CC - robo_map.field_244, robo_map.field_1D2);
            }
        }
        robo_map.field_1E8 &= 0xFFFFFF1F;
    }
}



int sub_4C3C88(const void *a1, const void *a2)
{
    uint8_t aa1 = *(const uint8_t *)a1;
    uint8_t aa2 = *(const uint8_t *)a2;

    const World::TVhclProto &v4 = dword_5BAA60->_vhclProtos[aa1];
    const World::TVhclProto &v5 = dword_5BAA60->_vhclProtos[aa2];

    int v7, v8;

    switch ( v4.model_id )
    {
    case BACT_TYPES_BACT:
        v7 = 1;
        break;

    case BACT_TYPES_TANK:
    case BACT_TYPES_CAR:
        v7 = 0;
        break;

    case BACT_TYPES_ROBO:
        v7 = 5;
        break;

    case BACT_TYPES_FLYER:
        v7 = 2;
        break;

    case BACT_TYPES_UFO:
        v7 = 3;
        break;

    case BACT_TYPES_GUN:
        v7 = 4;
        break;

    case BACT_TYPES_MISSLE:
    case BACT_TYPES_ZEPP:
        v7 = 10;
        break;

    default:
        v7 = 0;
        break;
    }

    switch ( v5.model_id )
    {
    case BACT_TYPES_BACT:
        v8 = 1;
        break;

    case BACT_TYPES_TANK:
    case BACT_TYPES_CAR:
        v8 = 0;
        break;

    case BACT_TYPES_ROBO:
        v8 = 5;
        break;

    case BACT_TYPES_FLYER:
        v8 = 2;
        break;

    case BACT_TYPES_UFO:
        v8 = 3;
        break;

    case BACT_TYPES_GUN:
        v8 = 4;
        break;

    case BACT_TYPES_MISSLE:
    case BACT_TYPES_ZEPP:
        v8 = 10;
        break;

    default:
        v8 = 0;
        break;
    }

    if ( v7 < v8 )
        return -1;
    else if ( v7 > v8)
        return 1;
    else if ( v7 == v8 )
    {
        if ( v4.energy > v5.energy )
            return 1;
        else if (v4.energy < v5.energy)
            return -1;
    }

    return 0;
}

void ypaworld_func64__sub16(NC_STACK_ypaworld *yw)
{
    int v6 = 0;

    if ( yw->_userRobo )
        v6 = 1 << yw->_userRobo->_owner;

    int v3 = 0;
    int i = 0;
    for (World::TVhclProto &proto : yw->_vhclProtos)
    {
        if ( v6 & proto.disable_enable_bitmask )
        {
            bzda.field_2DC[v3] = i;
            v3++;
        }
        i++;
    }

    bzda.field_8E0 = v3;

    dword_5BAA60 = yw;

    qsort(bzda.field_2DC, v3, 1, sub_4C3C88);

    if ( !v3 )
    {
        bzda.field_8EC = -1;
    }
    else
    {
        if ( v3 == 1 )
        {
            bzda.field_8EC = 0;
        }
        else if ( v3 <= bzda.field_8EC )
        {
            bzda.field_8EC = v3 - 1;
        }
    }
}

int sub_4C3D6C(const void *a1, const void *a2)
{

    uint8_t aa1 = *(const uint8_t *)a1;
    uint8_t aa2 = *(const uint8_t *)a2;

    World::TBuildingProto *v3 = &dword_5BAA60->_buildProtos[aa1];
    World::TBuildingProto *v4 = &dword_5BAA60->_buildProtos[aa2];

    int v6, v8;

    if ( v3->ModelID == 1 )
        v6 = 0;
    else if ( v3->ModelID == 2 )
        v6 = 2;
    else if ( v3->ModelID == 3 )
        v6 = 1;
    else
        v6 = 10;

    if ( v4->ModelID == 1 )
        v8 = 0;
    else if ( v4->ModelID == 2 )
        v8 = 2;
    else if ( v4->ModelID == 3 )
        v8 = 1;
    else
        v8 = 10;

    if ( v6 >= v8 )
    {
        if ( v6 <= v8 )
        {
            if ( v3->Energy >= v4->Energy )
                return v3->Energy > v4->Energy;
            else
                return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        return -1;
    }
    return 0;
}


void ypaworld_func64__sub17(NC_STACK_ypaworld *yw)
{
    int v6 = 0;

    if ( yw->_userRobo )
        v6 = 1 << yw->_userRobo->_owner;

    int v3 = 0;

    int i = 0;
    for (const World::TBuildingProto &proto : yw->_buildProtos)
    {
        if ( v6 & proto.EnableMask )
        {
            bzda.field_3DC[v3] = i;
            v3++;
        }
        i++;
    }

    bzda.field_8E4 = v3;

    dword_5BAA60 = yw;

    qsort(bzda.field_3DC, v3, 1, sub_4C3D6C);

    if ( !v3 )
    {
        bzda.field_8F4 = -1;
    }
    else
    {
        if ( v3 == 1 )
        {
            bzda.field_8F4 = 0;
        }
        else if ( v3 <= bzda.field_8F4 )
        {
            bzda.field_8F4 = v3 - 1;
        }
    }
}

void ypaworld_func64__sub15(NC_STACK_ypaworld *yw)
{
    NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

    int v4 = 0;
    int i = 0;

    for(World::TRoboGun &gun : robo->GetGuns())
    {
        if ( gun.gun_obj )
        {
            bzda.field_1DC[v4] = i;

            if ( gun.gun_obj == yw->_userUnit )
                bzda.field_8F0 = v4;

            v4++;
        }
        i++;
    }

    bzda.field_8DC = v4;

    if ( !v4 )
    {
        bzda.field_8F0 = -1;
    }
    else
    {
        if ( v4 == 1 )
        {
            bzda.field_8F0 = 0;
        }
        else if ( v4 <= bzda.field_8F0 )
        {
            bzda.field_8F0 = v4 - 1;
        }
    }
}

void NC_STACK_ypaworld::VoiceMessagePlayFile(const std::string &flname, NC_STACK_ypabact *unit, int priority)
{
    _voiceMessage.Reset();

    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:");

    std::string filename = "sounds/speech/";
    std::string locName = Locale::Text::GetLocaleName();

    if ( !locName.empty() )
        filename += locName + "/";
    else
        filename += "language/";

    filename += flname;

    if ( !uaFileExist("rsrc:" + filename) )
    {
        filename = "sounds/speech/language/";
        filename += flname;
    }

    NC_STACK_wav *v23 = Nucleus::CInit<NC_STACK_wav>( {{NC_STACK_rsrc::RSRC_ATT_NAME, filename}} );

    Common::Env.SetPrefix("rsrc", oldRsrc);

    if ( v23 )
    {        
        _voiceMessage.Carrier.Sounds[0].Pitch = 0;
        _voiceMessage.Carrier.Sounds[0].Volume = priority + 500;

        if ( v23 )
            _voiceMessage.Carrier.Sounds[0].PSample = v23->GetSampleData();
        else
            _voiceMessage.Carrier.Sounds[0].PSample = NULL;

        _voiceMessage.Priority = priority;
        _voiceMessage.Sample = v23;
        _voiceMessage.Unit = unit;

        VoiceMessageCalcPositionToUnit();

        _voiceMessage.Carrier.Vector = _userUnit->_fly_dir * _userUnit->_fly_dir_length;
        SFXEngine::SFXe.startSound(&_voiceMessage.Carrier, 0);
    }
}
void NC_STACK_ypaworld::VoiceMessagePlayMsg(NC_STACK_ypabact *unit, int priority, int msgID)
{
    if ( _voiceMessage.Priority < 0 || _voiceMessage.Priority < priority )
    {

        struct TMsgVals
        {
            int v1 = 0;
            int type = 0;
            int v3 = 0;
            int v4 = 0;
            int num = 0;
            
            TMsgVals() {};
            
            TMsgVals(int p1, int t, int p2, int p3, int n)
            : v1(p1), type(t), v3(p2), v4(p3), num(n) {};
            
        } msgvals;

        switch ( msgID )
        {
        case 1:
            msgvals = TMsgVals(1, 0, 1, 2, 2);
            break;
        case 5:
            msgvals = TMsgVals(1, 0, 3, 2, 2);
            break;
        case 6:
            msgvals = TMsgVals(1, 0, 2, 3, 1);
            break;
        case 7:
            msgvals = TMsgVals(1, 0, 2, 2, 2);
            break;
        case 8:
            msgvals = TMsgVals(1, 0, 2, 6, 2);
            break;
        case 9:
            msgvals = TMsgVals(1, 0, 2, 5, 1);
            break;
        case 10:
            msgvals = TMsgVals(1, 0, 2, 1, 1);
            break;
        case 11:
            msgvals = TMsgVals(1, 0, 3, 3, 1);
            break;
        case 12:
            msgvals = TMsgVals(2, 1, 2, 6, 2);
            break;
        case 13:
            msgvals = TMsgVals(2, 1, 2, 1, 1);
            break;
        case 14:
            msgvals = TMsgVals(1, 0, 1, 1, 2);
            break;
        case 15:
            msgvals = TMsgVals(1, 0, 1, 3, 2);
            break;
        case 16:
            msgvals = TMsgVals(1, 0, 1, 4, 2);
            break;
        case 17:
            msgvals = TMsgVals(1, 0, 1, 5, 1);
            break;
        case 18:
            msgvals = TMsgVals(1, 0, 2, 4, 1);
            break;
        case 19:
            msgvals = TMsgVals(1, 0, 3, 1, 1);
            break;
        case 22:
            msgvals = TMsgVals(1, 0, 1, 6, 1);
            break;
        case 23:
            msgvals = TMsgVals(2, 1, 1, 1, 1);
            break;
        case 24:
            msgvals = TMsgVals(2, 1, 1, 2, 1);
            break;
        case 25:
            msgvals = TMsgVals(2, 1, 1, 3, 1);
            break;
        case 26:
            msgvals = TMsgVals(2, 1, 1, 4, 1);
            break;
        case 27:
            msgvals = TMsgVals(2, 1, 1, 5, 1);
            break;
        case 28:
            msgvals = TMsgVals(2, 1, 1, 6, 1);
            break;
        case 29:
            msgvals = TMsgVals(2, 1, 1, 7, 1);
            break;
        case 30:
            msgvals = TMsgVals(2, 1, 1, 8, 1);
            break;
        case 31:
            msgvals = TMsgVals(2, 2, 1, 8, 1);
            break;
        case 32:
            msgvals = TMsgVals(2, 3, 1, 8, 1);
            break;
        case 33:
            msgvals = TMsgVals(2, 1, 1, 9, 1);
            break;
        case 34:
            msgvals = TMsgVals(2, 2, 1, 9, 1);
            break;
        case 35:
            msgvals = TMsgVals(2, 3, 1, 9, 1);
            break;
        case 36:
            msgvals = TMsgVals(2, 1, 1, 10, 1);
            break;
        case 37:
            msgvals = TMsgVals(2, 2, 1, 10, 1);
            break;
        case 38:
            msgvals = TMsgVals(2, 3, 1, 10, 1);
            break;
        case 39:
            msgvals = TMsgVals(3, 0, 1, 7, 1);
            break;
        case 40:
            msgvals = TMsgVals(2, 1, 2, 3, 1);
            break;
        case 41:
            msgvals = TMsgVals(2, 1, 2, 4, 2);
            break;
        case 42:
            msgvals = TMsgVals(2, 1, 2, 5, 2);
            break;
        case 43:
            msgvals = TMsgVals(2, 1, 2, 7, 3);
            break;
        case 44:
            msgvals = TMsgVals(2, 1, 2, 8, 2);
            break;
        case 45:
            msgvals = TMsgVals(1, 0, 3, 4, 1);
            if ( !unit )
                return;
            break;
        case 46:
            msgvals = TMsgVals(2, 4, 1, 1, 1);
            break;
        case 47:
            msgvals = TMsgVals(3, 0, 0, 1, 1);
            priority++;
            break;
        case 48:
            msgvals = TMsgVals(3, 0, 0, 2, 2);
            priority += 2;
            break;
        case 49:
            msgvals = TMsgVals(3, 0, 0, 3, 3);
            priority += 3;
            break;
        case 50:
            msgvals = TMsgVals(3, 0, 0, 5, 1);
            priority++;
            break;
        case 51:
            msgvals = TMsgVals(3, 0, 0, 7, 1);
            priority += 3;
            break;
        case 52:
            msgvals = TMsgVals(3, 0, 0, 8, 1);
            priority += 4;
            break;
        case 53:
            msgvals = TMsgVals(3, 0, 0, 9, 1);
            priority += 5;
            break;
        case 54:
            msgvals = TMsgVals(3, 0, 1, 1, 1);
            priority++;
            break;
        case 55:
            msgvals = TMsgVals(3, 0, 1, 2, 1);
            priority += 2;
            break;
        case 56:
            msgvals = TMsgVals(3, 0, 1, 3, 1);
            priority += 3;
            break;
        case 59:
            msgvals = TMsgVals(3, 0, 1, 8, 1);
            priority++;
            break;
        case 60:
            msgvals = TMsgVals(3, 0, 0, 4, 1);
            priority += 4;
            break;
        case 61:
            msgvals = TMsgVals(3, 0, 1, 0, 1);
            priority += 6;
            break;
        case 63:
            msgvals = TMsgVals(3, 0, 3, 0, 1);
            priority += 13;
            break;
        case 64:
            msgvals = TMsgVals(3, 0, 0, 6, 1);
            priority += 2;
            break;
        case 65:
            msgvals = TMsgVals(3, 0, 1, 4, 1);
            priority += 4;
            break;
        case 66:
            msgvals = TMsgVals(3, 0, 1, 6, 1);
            priority += 5;
            break;
        case 67:
            msgvals = TMsgVals(2, 1, 1, 11, 1);
            break;
        case 68:
            msgvals = TMsgVals(2, 2, 1, 8, 1);
            break;
        case 69:
            msgvals = TMsgVals(2, 3, 1, 8, 1);
            break;
        case 70:
            msgvals = TMsgVals(2, 4, 1, 5, 1);
            break;
        case 71:
            msgvals = TMsgVals(2, 4, 1, 8, 1);
            break;
        case 72:
            msgvals = TMsgVals(2, 4, 1, 6, 1);
            break;
        case 73:
            msgvals = TMsgVals(2, 4, 1, 7, 1);
            break;
        case 74:
            msgvals = TMsgVals(2, 4, 1, 9, 1);
            break;
        case 75:
            msgvals = TMsgVals(2, 4, 1, 12, 1);
            break;
        case 76:
            msgvals = TMsgVals(2, 4, 1, 10, 1);
            break;
        case 77:
            msgvals = TMsgVals(2, 4, 1, 11, 1);
            break;
        case 78:
            msgvals = TMsgVals(2, 1, 1, 12, 1);
            break;
        case 79:
            msgvals = TMsgVals(2, 4, 1, 2, 1);
            break;
        case 80:
            msgvals = TMsgVals(2, 1, 1, 13, 1);
            break;
        case 81:
            msgvals = TMsgVals(2, 4, 1, 3, 1);
            break;
        case 82:
            msgvals = TMsgVals(2, 4, 1, 4, 1);
            break;
        case 83:
            msgvals = TMsgVals(2, 5, 1, 2, 1);
            break;
        case 84:
            msgvals = TMsgVals(2, 5, 1, 1, 1);
            break;
        case 85:
            msgvals = TMsgVals(2, 5, 1, 3, 1);
            break;
        case 86:
            msgvals = TMsgVals(2, 5, 1, 4, 1);
            break;
        case 87:
            msgvals = TMsgVals(2, 5, 1, 7, 1);
            break;
        case 88:
            msgvals = TMsgVals(2, 5, 1, 6, 1);
            break;
        case 89:
            msgvals = TMsgVals(2, 5, 1, 8, 1);
            break;
        case 90:
            msgvals = TMsgVals(2, 5, 1, 9, 1);
            break;
        case 91:
            msgvals = TMsgVals(2, 5, 1, 5, 1);
            break;
        case 92:
            msgvals = TMsgVals(2, 5, 1, 10, 1);
            break;
        case 93:
            msgvals = TMsgVals(4, 1, 1, 1, 1);
            break;
        case 94:
            msgvals = TMsgVals(3, 0, 1, 9, 1);
            priority += 2;
            break;
        case 95:
            msgvals = TMsgVals(3, 0, 2, 0, 1);
            priority += 3;
            break;
        case 96:
            msgvals = TMsgVals(3, 0, 2, 1, 1);
            priority += 4;
            break;
        case 97:
            msgvals = TMsgVals(3, 0, 2, 2, 1);
            priority += 5;
            break;
        case 98:
            msgvals = TMsgVals(3, 0, 2, 3, 1);
            priority += 6;
            break;
        case 99:
            msgvals = TMsgVals(3, 0, 2, 4, 1);
            priority += 7;
            break;
        case 100:
            msgvals = TMsgVals(3, 0, 2, 5, 1);
            priority += 8;
            break;
        case 101:
            msgvals = TMsgVals(3, 0, 2, 6, 1);
            priority += 9;
            break;
        case 102:
            msgvals = TMsgVals(3, 0, 2, 7, 1);
            priority += 10;
            break;
        case 103:
            msgvals = TMsgVals(3, 0, 2, 8, 1);
            priority += 11;
            break;
        case 104:
            msgvals = TMsgVals(3, 0, 2, 9, 1);
            priority += 12;
            break;
        
        default:
            break;
        }
        
        if ( msgvals.v1 )
        {
            if ( !unit )
                unit = _userRobo;
            
            int vo_type = msgvals.type;

            if ( msgvals.v1 == 1 )
            {
                vo_type = _vhclProtos[unit->_vehicleID].vo_type;

                if ( !vo_type )
                    vo_type = 11;
            }

            int v16 = 1;
            
            if ( msgvals.num > 1 )
                v16 = rand() % msgvals.num + 1;

            VoiceMessagePlayFile(fmt::sprintf("%x%x%x%x%x.wav", msgvals.v1, vo_type, msgvals.v3, msgvals.v4, v16), unit, priority);
        }
    }
}

void sb_0x4d7c08__sub0__sub0(NC_STACK_ypaworld *yw)
{
    char *pcur = byte_5A7650;

    if ( yw->_isNetGame )
    {
        if ( yw->_netEvent.EventType )
        {
            uint32_t timeDelay;
            std::string str;
            
            if ( yw->_netEvent.EventType == 1 )
            {
                str = Locale::Text::Advanced(Locale::ADV_VICTORY);
                timeDelay = 40000;
            }
            else if ( yw->_netEvent.EventType > 1 && yw->_netEvent.EventType <= 4 )
            {
                str = fmt::sprintf( Locale::Text::Advanced(Locale::ADV_DEFEATED) , yw->_netEvent.PlayerName);

                timeDelay = 20000;
            }
            else
            {
                timeDelay = 0;
            }

            if ( !str.empty() && yw->_timeStamp - yw->_netEvent.TimeStamp < timeDelay )
            {
                if ( yw->_timeStamp / 300 & 1 )
                {
                    FontUA::select_tileset(&pcur, 15);
                    FontUA::set_xpos(&pcur, 0);
                    FontUA::set_ypos(&pcur, yw->_screenSize.y / 3);

                    FontUA::set_txtColor(&pcur, 255, 255, 255);

                    pcur = FontUA::FormateCenteredSkipableItem(yw->_guiTiles[15], pcur, str, yw->_screenSize.x);
                }
            }
            else
            {
                yw->_netEvent.EventType = 0;
            }
        }
    }

    if ( sb_0x4d7c08__sub0__sub0__sub0(yw) )
        return;

    int v10;

    if ( info_log.field_254 <= info_log.msg_count )
        v10 = info_log.msg_count - info_log.field_254;
    else
        v10 = info_log.msg_count + 64 - info_log.field_254;

    if ( v10 > 10 )
        v10 = 10;

    FontUA::select_tileset(&pcur, 15);
    FontUA::set_ypos(&pcur, up_panel.field_1CC + yw->_fontH / 2);


    int v11 = (info_log.msg_count - v10) % 64;

    if ( v11 < 0 )
        v11 += 64;

    int v28 = 0;
    int v27 = 0;

    while ( 1 )
    {
        if ( v11 >= 64 )
            v11 = 0;

        inflog_msg *v14 = &info_log.msgs[v11];

        if ( v14->field_8 > 0 )
        {
            if ( !v27 )
            {
                v27 = 1;

                int v15;

                if ( v11 > info_log.msg_count )
                    v15 = info_log.msg_count + 64 - v11;
                else
                    v15 = info_log.msg_count - v11;

                v15 = (v15 + 1) / 2;

                if ( !v15 )
                    v15 = 1;

                v14->field_8 -= yw->_frameTime * v15;
            }

            if ( v14->field_4 )
                v28 = v14->field_4;
            else
                v28 += 200;

            int v18 = yw->_timeStamp - v28;

            int v19;

            if ( v18 < 200 )
            {
                v19 = 100 * v18 / 200;

                if ( v19 < 0 )
                    v19 = 0;
                else if ( v19 > 100 )
                    v19 = 100;
            }
            else
            {
                v19 = 100;
            }

            if ( v19 > 0 )
            {
                FontUA::set_xpos(&pcur, 16);
                FontUA::set_txtColor(&pcur, yw->_iniColors[64].r, yw->_iniColors[64].g, yw->_iniColors[64].b);
                // Output ingame messages. From analyzer and other.
                pcur = FontUA::TextRelWidthItem(yw->_guiTiles[15], pcur, v14->txt, v19, 4);

                FontUA::next_line(&pcur);
            }
        }

        if ( v11 == info_log.msg_count )
            break;

        v11++;
    }

    FontUA::set_end(&pcur);

    w3d_a209 arg;

    arg.cmdbuf = byte_5A7650;
    arg.includ = 0;

    GFX::Engine.DrawText(&arg);
}



void wis_color(NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, SDL_Color *out1, SDL_Color *out2)
{
    float v7 = yw->_hud.field_86 * 2.0 + -1.5;

    float v18 = fabs(v7 - x1);
    float v17 = fabs(v7 - x2);

    if ( v18 > 0.5 )
        v18 = 0.5;

    if ( v17 > 0.5 )
        v17 = 0.5;

    float v8 = v18 * 2.0;
    int v9 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v8 + yw->_hud.cl1_g;
    int v10 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v8 + yw->_hud.cl1_r;
    int v11 = (yw->_hud.cl2_b - yw->_hud.cl1_b) * v8 + yw->_hud.cl1_b;

    *out1 = GFX::Engine.Color(v10, v9, v11);

    float v12 = 2.0 * v17;
    int v13 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v12 + yw->_hud.cl1_g;
    int v14 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v12 + yw->_hud.cl1_r;
    int v15 = (yw->_hud.cl2_b - yw->_hud.cl1_b) * v12 + yw->_hud.cl1_b;
    *out2 = GFX::Engine.Color(v13, v14, v15);
}




void yw_RenderVector2D(NC_STACK_ypaworld *yw, UAskeleton::Data *wire, float posX, float posY, float m00, float m01, float m10, float m11, float scaleX, float scaleY, SDL_Color coloooor, wis_color_func color_func, wis_color_func color_func2, bool aspectCorrection)
{
    float CW = 1.0, CH = 1.0;

    if (aspectCorrection)
        GFX::Engine.getAspectCorrection(CW, CH, false);

    if ( wire )
    {
        GFX::Engine.raster_func217(coloooor);

        for (size_t i = 0; i < wire->POO.size(); i++)
        {
            float tmpx = wire->POO[i].x * 0.001;
            float tmpz = -wire->POO[i].z * 0.001;

            wire->tformedVertex[i].x = (tmpx * m00 + tmpz * m01) * scaleX * CW + posX;
            wire->tformedVertex[i].y = (tmpx * m10 + tmpz * m11) * scaleY * CH + posY;
        }

        for (size_t i = 0; i < wire->polygons.size(); i++)
        {
            UAskeleton::Polygon *poly = &wire->polygons[i];

            if (poly->num_vertices >= 2)
            {
                Common::FLine v29;
                v29.x1 = wire->tformedVertex[ poly->v[0] ].x;
                v29.y1 = wire->tformedVertex[ poly->v[0] ].y;
                v29.x2 = wire->tformedVertex[ poly->v[1] ].x;
                v29.y2 = wire->tformedVertex[ poly->v[1] ].y;

                if ( color_func )
                {
                    SDL_Color v32 = coloooor;
                    SDL_Color v31 = coloooor;

                    color_func(yw,  v29.x1 - posX,   v29.y1 - posY,   v29.x2 - posX,   v29.y2 - posY, &v32, &v31);

                    GFX::Engine.raster_func217(v32);
                }
                else if ( color_func2 )
                {
                    SDL_Color v34 = coloooor;
                    SDL_Color v33 = coloooor;

                    color_func2(yw, v29.x1, v29.y1, v29.x2, v29.y2, &v34, &v33);


                    GFX::Engine.raster_func217(v34);
                }

                GFX::Engine.raster_func200(v29);
            }
        }
    }
}

void yw_RenderInfoVehicleWire(NC_STACK_ypaworld *yw, sklt_wis *wis, World::TVhclProto *vhcl, float a4, float a5, float a6)
{
    SDL_Color color_25 = yw->GetColor(25);
    SDL_Color color_34 = yw->GetColor(34);

    UAskeleton::Data *wairufureimu = NULL;

    wis_color_func func = NULL;

    if ( wis->field_72 )
        func = wis_color;

    if ( vhcl->wireframe )
        wairufureimu = vhcl->wireframe->GetSkelet();

    if ( wairufureimu )
    {
        float v15 = wis->field_92 * 6.0 * 0.8;

        float a9 = 0.1;

        if ( a6 < 1.4 )
            a9 = a6 * 0.1;

        yw->_hud.cl1_r = color_34.r;
        yw->_hud.cl1_g = color_34.g;
        yw->_hud.cl1_b = color_34.b;
        yw->_hud.cl2_r = color_25.r;
        yw->_hud.cl2_b = color_25.b;
        yw->_hud.cl2_g = color_25.g;

        yw_RenderVector2D(yw, wairufureimu, a4, a5, 1.0, 0.0, 0.0, 1.0, a9, v15, color_34, NULL, func, true);
    }
}



char * sub_4E4F80(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float x, float y, int value, int maxval, int valCH, int valBG, const std::string &txt1, const std::string &txt2, int flag)
{
    int wnd_xpos = 0;
    int wnd_xpos2 = 0;
    int wnd_ypos = 0;
    int wnd_ypos2 = 0;

    int wnd_vis = 0;

    if ( robo_map.IsOpen() )
    {
        wnd_xpos = robo_map.x - (yw->_screenSize.x / 2);
        wnd_xpos2 = robo_map.w + wnd_xpos;
        wnd_ypos = robo_map.y - (yw->_screenSize.y / 2);
        wnd_ypos2 = robo_map.h + wnd_ypos;

        wnd_vis = 1;
    }

    int v51 = (yw->_screenSize.x / 2) * x;
    int v50 = (yw->_screenSize.y / 2) * y;

    int v49 = yw->_guiTiles[51]->map[valCH].w * wis->field_9E;


    char *pcur = cur;

    FontUA::set_txtColor(&pcur, yw->_iniColors[65].r, yw->_iniColors[65].g, yw->_iniColors[65].b);

    v51 -= (wis->field_9A + wis->field_96 + v49) / 2;

    if ( !txt1.empty() )
    {
        int v19 = (wis->field_96 / 2) + v51;
        int v20 = v50;

        if ( !wnd_vis || v19 <= wnd_xpos || v19 >= wnd_xpos2 || v20 <= wnd_ypos || v20 >= wnd_ypos2 )
        {
            FontUA::select_tileset(&pcur, 15);

            FontUA::set_center_xpos(&pcur, v51);
            FontUA::set_center_ypos(&pcur, v50 - (yw->_fontH / 2));

            pcur = FontUA::TextRelWidthItem(yw->_guiTiles[15], pcur, txt1, 100, 4);
        }
    }

    FontUA::select_tileset(&pcur, 51);

    v51 += wis->field_96;

    FontUA::set_center_xpos(&pcur, v51);
    FontUA::set_center_ypos(&pcur, v50 - (yw->_guiTiles[51]->h / 2));

    int v29 = v51 + (yw->_guiTiles[51]->map[1].w / 2);
    int v30 = v50 + (yw->_guiTiles[51]->h >> 1);

    for (int i = 1; i <= wis->field_9E; i++)
    {
        int v35 = i * (maxval / wis->field_9E) - (maxval / wis->field_9E) / 2;

        if ( !wnd_vis || v29 <= wnd_xpos || v29 >= wnd_xpos2 || v30 <= wnd_ypos || v30 >= wnd_ypos2 )
        {
            if ( v35 > value )
            {
                if ( (flag & 2) == 0 )
                    FontUA::store_u8(&pcur, valBG);
                else
                    FontUA::add_xpos(&pcur, yw->_guiTiles[51]->map[1].w);
            }
            else
            {
                if ( (flag & 1) == 0 )
                    FontUA::store_u8(&pcur, valCH);
                else
                    FontUA::add_xpos(&pcur, yw->_guiTiles[51]->map[1].w);
            }
        }

        v29 += yw->_guiTiles[51]->map[1].w;
    }

    v51 += wis->field_9A + v49;

    if ( !txt2.empty() )
    {
        int v37 = v51 - (wis->field_9A / 2);
        int v38 = v50;

        if ( !wnd_vis || v37 <= wnd_xpos || v37 >= wnd_xpos2 || v38 <= wnd_ypos || v38 >= wnd_ypos2 )
        {
            FontUA::select_tileset(&pcur, 15);

            FontUA::set_center_xpos(&pcur, v51);
            FontUA::set_center_ypos(&pcur, v50 - (yw->_fontH / 2));

            pcur = FontUA::TextRelWidthItem(yw->_guiTiles[15], pcur, txt2, 100, 8);
        }
    }

    return pcur;
}

char * yw_RenderInfoLifebar(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, NC_STACK_ypabact *bact, World::TVhclProto *vhcl, float xpos, float ypos)
{
    int a6a;
    int v10;

    if ( bact )
    {
        a6a = bact->_energy;
        v10 = bact->_energy_max;
    }
    else
    {
        v10 = vhcl->energy;
        a6a = vhcl->energy;
    }

    return sub_4E4F80(yw, wis, cur, xpos, ypos, a6a, v10, 2, 6, Locale::Text::HUD(Locale::HUDSTR_EGY), fmt::sprintf("%d", (a6a + 99) / 100));
}


char * yw_RenderInfoShieldbar(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, NC_STACK_ypabact *bact, World::TVhclProto *vhcl, float xpos, float ypos)
{
    int v10;

    if ( bact )
        v10 = bact->_shield;
    else
        v10 = vhcl->shield;

    return sub_4E4F80(yw, wis, cur, xpos, ypos, v10, 100, 1, 5, Locale::Text::HUD(Locale::HUDSTR_AMR), fmt::sprintf("%d%%", v10), 2);
}



char * yw_RenderInfoVehicleName(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, const std::string &name, float xpos, float ypos)
{
    char *pcur = cur;

    int v29 = (yw->_screenSize.x / 2) * xpos;
    int v33 = (yw->_screenSize.y / 2) * ypos;

    int v31 = yw->_screenSize.x * wis->field_8A;

    SDL_Color v21 = yw->GetColor(25);
    SDL_Color a11 = yw->GetColor(34);

    wis_color_func a13;

    if ( wis->field_72 )
        a13 = wis_color;
    else
        a13 = NULL;

    int wnd_xpos = 0;
    int wnd_xpos2 = 0;
    int wnd_ypos = 0;
    int wnd_ypos2 = 0;

    int wnd_vis = 0;

    if ( robo_map.IsOpen() )
    {
        wnd_xpos = robo_map.x - (yw->_screenSize.x / 2);
        wnd_xpos2 = robo_map.w + wnd_xpos;
        wnd_ypos = robo_map.y - (yw->_screenSize.y / 2);
        wnd_ypos2 = robo_map.h + wnd_ypos;

        wnd_vis = 1;
    }

    int v30 = v29 - (v31 / 2);
    int v34 = v33 - (yw->_fontH / 2);

    if ( !wnd_vis || v29 <= wnd_xpos || v29 >= wnd_xpos2 || v33 <= wnd_ypos || v33 >= wnd_ypos2 )
    {
        FontUA::select_tileset(&pcur, 15);

        FontUA::set_center_xpos(&pcur, v30);
        FontUA::set_center_ypos(&pcur, v34);

        FontUA::set_txtColor(&pcur,  yw->_iniColors[65].r,  yw->_iniColors[65].g,  yw->_iniColors[65].b);

        pcur = sub_451714(yw->_guiTiles[15], pcur, name, v31, 32);
    }

    if ( wis->sklts_intern[2] )
    {
        yw->_hud.cl1_r = a11.r;
        yw->_hud.cl1_g = a11.g;
        yw->_hud.cl1_b = a11.b;
        yw->_hud.cl2_r = v21.r;
        yw->_hud.cl2_b = v21.b;
        yw->_hud.cl2_g = v21.g;

        float a4 = (1.0 / (float)yw->_screenSize.y) * 2.0 + ypos;

        float a10 = wis->field_92 * 1.2;

        yw_RenderVector2D(yw, wis->sklts_intern[2], xpos, a4, 1.0, 0.0, 0.0, 1.0, wis->field_8A, a10, a11, NULL, a13, true);
    }

    return pcur;
}


void yw_RenderInfoWeaponWire(NC_STACK_ypaworld *yw, sklt_wis *wis, World::TWeapProto *wpn, float xpos, float ypos)
{
    UAskeleton::Data *wairufureimu = NULL;

    wis_color_func func;

    if ( wis->field_72 )
        func = wis_color;
    else
        func = NULL;

    if ( wpn )
    {
        if ( wpn->wireframe )
            wairufureimu = wpn->wireframe->GetSkelet();

        if ( wairufureimu )
        {
            SDL_Color v9 = yw->GetColor(20);
            SDL_Color v10 = yw->GetColor(29);

            yw->_hud.cl1_r = v10.r;
            yw->_hud.cl1_g = v10.g;
            yw->_hud.cl1_b = v10.b;
            yw->_hud.cl2_r = v9.r;
            yw->_hud.cl2_b = v9.b;
            yw->_hud.cl2_g = v9.g;

            yw_RenderVector2D(yw, wairufureimu, xpos, ypos, 1.0, 0.0, 0.0, 1.0, 0.0415, 0.05, v10, NULL, func, true);
        }
    }
}

char * yw_RenderInfoReloadbar(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, NC_STACK_ypabact *bact, World::TWeapProto *wpn, float xpos, float ypos)
{
    char *pcur = cur;

    if ( wpn )
    {
        int v12;

        if ( bact )
        {

            int v10 = wpn->shot_time_user;

            if ( wpn->salve_shots )
            {
                if ( bact->_salve_counter >= wpn->salve_shots )
                    v10 = wpn->salve_delay;
            }

            v12 = 100 * (bact->_clock - bact->_weapon_time) / v10;

            if ( v12 >= 100 )
                v12 = 100;
        }
        else
            v12 = 100;

        std::string txt2;

        if ( v12 == 100 )
            txt2 = Locale::Text::HUD(Locale::HUDSTR_OK);
        else
            txt2 = fmt::sprintf("%d%%", v12);

        pcur = sub_4E4F80(yw, wis, pcur, xpos, ypos, v12, 100, 1, 3, Locale::Text::HUD(Locale::HUDSTR_RLD), txt2);
    }
    return pcur;
}

char * yw_RenderInfoWeaponInf(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, NC_STACK_ypabact *bact, World::TVhclProto *vhcl, World::TWeapProto *weap, float xpos, float ypos)
{
    char *pcur = cur;

    if ( weap )
    {
        std::string txt2;

        if ( vhcl->num_weapons <= 1 )
            txt2 = fmt::sprintf("%d", weap->energy / 100);
        else
            txt2 = fmt::sprintf("%d x%d", weap->energy / 100, vhcl->num_weapons);

        pcur = sub_4E4F80(yw, wis, pcur, xpos, ypos, weap->energy, 100, 7, 7, Locale::Text::HUD(Locale::HUDSTR_DMG), txt2, 1 | 2);
    }

    return pcur;
}


char * yw_RenderHUDInfo(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float xpos, float ypos, NC_STACK_ypabact *bact, int vhclid, int flag)
{
    char *pcur = cur;
//  yw->GetColor(25);
//  yw->GetColor(34);
    int v23 = 1;
    int v11 = 1;
    int v22 = 1;
    int v25 = 1;

    if ( flag & 1 )
        v23 = ((yw->_timeStamp / 200) & 1) != 0;

    if ( flag & 2 )
        v11 = ((yw->_timeStamp / 200) & 1) != 0;

    if ( flag & 4 )
        v22 = ((yw->_timeStamp / 200) & 1) != 0;

    if ( flag & 8 )
        v25 = ((yw->_timeStamp / 200) & 1) != 0;

    float a6a;

    if ( flag & 0x10 )
    {
        a6a = (yw->_timeStamp - wis->field_76 - 200) / 180.0;

        if ( a6a <= 0.0 )
            return pcur;
    }
    else
    {
        a6a = 1.0;
    }

    if ( vhclid == -1 )
        vhclid = bact->_vehicleID;

    World::TVhclProto *vhcl = &yw->_vhclProtos[vhclid];


    World::TWeapProto *weap;

    if ( vhcl->weapon == -1 )
        weap = NULL;
    else
        weap = &yw->_weaponProtos.at(vhcl->weapon);

    if ( v25 )
        yw_RenderInfoVehicleWire(yw, wis, vhcl, xpos, ypos, a6a);

    if ( v11 )
        pcur = yw_RenderInfoLifebar(yw, wis, pcur, bact, vhcl, xpos,   wis->field_92 * 7.0 + ypos  );

    if ( v22 )
        pcur = yw_RenderInfoShieldbar(yw, wis, pcur, bact, vhcl, xpos,   wis->field_92 * 9.0 + ypos  );

    if ( v25 )
    {
        if ( vhcl->model_id != BACT_TYPES_GUN )
        {
            float v15 = wis->field_92 * 12.0 + ypos;

            pcur = yw_RenderInfoVehicleName(yw, wis, pcur, yw->GetVehicleName(*vhcl), xpos, v15);
        }
    }

    if ( weap )
    {
        if ( v23 )
        {
            yw_RenderInfoWeaponWire(yw, wis, weap, xpos,   ypos - wis->field_92 * 9.0);

            pcur = yw_RenderInfoReloadbar(yw, wis, pcur, bact, weap, xpos,  ypos - wis->field_92 * 7.0);

            pcur = yw_RenderInfoWeaponInf(yw, wis, pcur, bact, vhcl, weap, xpos,  ypos - wis->field_92 * 9.0);
        }
    }

    return pcur;
}

char *sb_0x4d7c08__sub0__sub0__sub0__sub0(NC_STACK_ypaworld *yw, sklt_wis *wis, char *cur, float a4, float a5, int bldId, int a7)
{
    char *pcur = cur;
    //yw->GetColor(25);
    //yw->GetColor(34);

    if ( !(a7 & 0x10) || (yw->_timeStamp - wis->field_76 - 200) / 180.0 > 0.0 )
    {
        if ( yw->_timeStamp / 200 & 1 )
        {
            std::string bldName;
            if ( yw->_isNetGame )
                bldName = yw->GetBuildingName(bldId, true);
            else
                bldName = yw->GetBuildingName(bldId, false);

            pcur = yw_RenderInfoVehicleName(yw, wis, pcur, bldName, a4, a5);
        }
    }
    return pcur;
}

int sb_0x4d7c08__sub0__sub0__sub0(NC_STACK_ypaworld *yw)
{
    //Tech update draw
    GFX::Engine.raster_func210( Common::FRect(-1.0, -1.0, 1.0, 1.0) );

    sklt_wis *wis = &yw->_hud;

    char *pcur = byte_5C8DB0;

    if ( yw->_upgradeId == -1 || yw->_timeStamp - yw->_upgradeTimeStamp >= 10000 || (!yw->_upgradeVehicleId && !yw->_upgradeWeaponId && !yw->_upgradeBuildId) )
        return 0;

    int a8 = 0;
    int v6 = yw->_upgradeVehicleId;
    int v14 = yw->_upgradeBuildId;

    if ( !v6 && yw->_upgradeWeaponId)
    {
        bool notOk = true;
        int i = 0;
        for (const World::TVhclProto &proto : yw->_vhclProtos)
        {
            if ( proto.weapon == yw->_upgradeWeaponId)
            {
                v6 = i;
                notOk = false;
                break;
            }
            i++;
        }
        
        if (notOk)
            return 0;
    }

    wis->field_86 = 1.0;

    int v10 = yw->_techUpgrades[ yw->_upgradeId ].Type;

    if ( v10 == World::UPGRADE_WEAPON )
        a8 |= 1;
    else if ( v10 == World::UPGRADE_ARMOR )
        a8 |= 4;
    else if ( v10 == World::UPGRADE_VEHICLE )
        a8 |= 8;

    if ( v6 )
        pcur = yw_RenderHUDInfo(yw, wis, pcur, 0.0, -0.5, 0, v6, a8);
    else if ( v14 )
        pcur = sb_0x4d7c08__sub0__sub0__sub0__sub0(yw, wis, pcur, 0.0, -0.5, v14, a8);

    FontUA::set_end(&pcur);

    w3d_a209 v13;
    v13.cmdbuf = byte_5C8DB0;
    v13.includ = NULL;

    GFX::Engine.raster_func209(&v13);

    return 1;
}




void wis_color2(NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, SDL_Color *out1, SDL_Color *out2)
{
    float v7 = yw->_hud.field_86 * 6.28;

    float v24 = cos(v7);
    float v22 = sin(v7);

    float v23 = sqrt(POW2(x1) + POW2(y1));
    if ( v23 > 0.0 )
    {
        x1 /= v23;
        y1 /= v23;
    }

    float v21 = sqrt(POW2(x2) + POW2(y2));
    if ( v21 > 0.0 )
    {
        x2 /= v21;
        y2 /= v21;
    }

    float v12 = (v24 * x1 + v22 * y1 + 1.0) * 0.5;
    int v13 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v12 + yw->_hud.cl1_r;
    int v16 = (yw->_hud.cl2_b - yw->_hud.cl1_b) * v12 + yw->_hud.cl1_b;
    int v15 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v12 + yw->_hud.cl1_g;
    *out1 = GFX::Engine.Color(v13, v15, v16);

    float v17 = (v24 * x2 + v22 * y2 + 1.0) * 0.5;
    int v18 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v17 + yw->_hud.cl1_g;
    int v20 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v17 + yw->_hud.cl1_r;
    int v19 = (yw->_hud.cl2_b - yw->_hud.cl1_b) * v17 + yw->_hud.cl1_b;
    *out2 = GFX::Engine.Color(v20, v18, v19);
}



void yw_RenderHUDCompass(NC_STACK_ypaworld *yw, sklt_wis *wis)
{
    float v90 = (yw->_timeStamp - wis->field_76) / 180.0;

    wis_color_func func = NULL;

    if ( wis->field_72 )
        func = wis_color2;

    float xx = yw->_viewerRotation.m02;
    float yy = yw->_viewerRotation.m22;

    float v70 = sqrt( POW2(xx) + POW2(yy) );

    if ( v70 > 0.0 )
    {
        xx /= v70;
        yy /= v70;
    }

    SDL_Color v9 = yw->GetColor(21);
    SDL_Color v73 = yw->GetColor(30);

    float tx = yw->_userUnit->_parent->_position.x - yw->_userUnit->_position.x;
    float ty = yw->_userUnit->_parent->_position.z - yw->_userUnit->_position.z;

    float v13 = tx * yy + ty * xx;
    float v15 = tx * -xx + ty * yy;

    float v17 = sqrt( POW2(v13) + POW2(v15) );

    if ( v17 > 30.0 )
    {
        float v20 = v13 * (1.0 / v17);
        float v23 = v15 * (1.0 / v17);

        yw->_hud.cl1_r = v9.r;
        yw->_hud.cl1_g = v9.g;
        yw->_hud.cl1_b = v9.b;
        yw->_hud.cl2_r = v73.r;
        yw->_hud.cl2_b = v73.b;
        yw->_hud.cl2_g = v73.g;

        yw_RenderVector2D(yw, wis->sklts_intern[3], 0.7, 0.3, v23, -v20, v20, v23, 0.25, 0.3, v9, func, 0, true);
    }

    if ( yw->_userUnit->IsParentMyRobo() )
    {
        int v27 = 0;

        if ( yw->_userUnit->_primTtype == BACT_TGT_TYPE_CELL )
        {
            v27 = 1;
            ty = yw->_userUnit->_primTpos.z - yw->_userUnit->_position.z;
            tx = yw->_userUnit->_primTpos.x - yw->_userUnit->_position.x;
        }
        else if ( yw->_userUnit->_primTtype == BACT_TGT_TYPE_UNIT )
        {
            v27 = 1;
            tx = yw->_userUnit->_primT.pbact->_position.x - yw->_userUnit->_position.x;
            ty = yw->_userUnit->_primT.pbact->_position.z - yw->_userUnit->_position.z;
        }

        if ( v27 )
        {
            SDL_Color v31 = yw->GetColor(22);
            SDL_Color v74 = yw->GetColor(31);

            float v88 = tx * yy + ty * xx;
            float a8 = tx * -xx + ty * yy;

            float v36 = sqrt( POW2(v88) + POW2(a8) );

            if ( v36 > 30.0 )
            {
                float v40 = v88 * (1.0 / v36);
                float v42 = a8 * (1.0 / v36);

                yw->_hud.cl1_r = v31.r;
                yw->_hud.cl1_g = v31.g;
                yw->_hud.cl1_b = v31.b;
                yw->_hud.cl2_r = v74.r;
                yw->_hud.cl2_b = v74.b;
                yw->_hud.cl2_g = v74.g;

                yw_RenderVector2D(yw, wis->sklts_intern[3], 0.7, 0.3, v42, -v40, v40, v42, 0.25, 0.3, v31, func, 0, true);
            }
        }
    }

    if ( yw->_guiVisor.field_18 )
    {
        SDL_Color a11 = yw->GetColor(23);
        SDL_Color v50 = yw->GetColor(32);

        float v45 = yw->_guiVisor.field_18->_position.x - yw->_userUnit->_position.x;
        float v46 = yw->_guiVisor.field_18->_position.z - yw->_userUnit->_position.z;

        float v81 = v45 * yy + v46 * xx;
        float v79 = v45 * -xx + v46 * yy;


        float v52 = sqrt( POW2(v81) + POW2(v79) );

        if ( v52 > 30.0 )
        {
            float v55 = v81 * (1.0 / v52);
            float v58 = v79 * (1.0 / v52);

            yw->_hud.cl1_r = a11.r;
            yw->_hud.cl1_g = a11.g;
            yw->_hud.cl1_b = a11.b;
            yw->_hud.cl2_r = v50.r;
            yw->_hud.cl2_b = v50.b;
            yw->_hud.cl2_g = v50.g;

            yw_RenderVector2D(yw, wis->sklts_intern[3], 0.7, 0.3, v58, -v55, v55, v58, 0.25, 0.3, a11, func, 0, true);
        }
    }

    SDL_Color v60 = yw->GetColor(24);
    SDL_Color v75 = yw->GetColor(33);

    float v72 = 0.25;
    float v71 = 0.3;

    if ( v90 < 1.4 )
    {
        v71 = v90 * 0.3;
        v72 = v90 * 0.25;
    }

    yw->_hud.cl1_r = v60.r;
    yw->_hud.cl1_g = v60.g;
    yw->_hud.cl1_b = v60.b;
    yw->_hud.cl2_r = v75.r;
    yw->_hud.cl2_b = v75.b;
    yw->_hud.cl2_g = v75.g;

    yw_RenderVector2D(yw, wis->sklts_intern[1], 0.7, 0.3, yy, -xx, xx, yy, v72, v71, v60, func, 0, true);

    int v64;

    switch ( yw->_userUnit->_pSector->owner )
    {
    case 0:
        v64 = 0;
        break;

    case 1:
        v64 = 1;
        break;

    case 2:
        v64 = 2;
        break;

    case 3:
        v64 = 3;
        break;

    case 4:
        v64 = 4;
        break;

    case 5:
        v64 = 5;
        break;

    case 6:
        v64 = 6;
        break;

    case 7:
        v64 = 7;
        break;

    default:
        v64 = 0;
        break;
    }

    SDL_Color v65 = yw->GetColor(v64);

    yw->_hud.cl1_r = v65.r;
    yw->_hud.cl1_g = v65.g;
    yw->_hud.cl1_b = v65.b;
    yw->_hud.cl2_r = v65.r;
    yw->_hud.cl2_b = v65.b;
    yw->_hud.cl2_g = v65.g;

    yw_RenderVector2D(yw, wis->sklts_intern[2], 0.7, 0.3, yy, -xx, xx, yy, 0.07, 0.08, v65, func, 0, true);
}


char * yw_RenderHUDVectorGFX(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;
    GFX::Engine.raster_func210( Common::FRect(-1.0, -1.0, 1.0, 1.0) );

    Common::Rect v7;

    if ( !robo_map.IsClosed() )
    {
        v7.left = robo_map.x - (yw->_screenSize.x / 2);
        v7.top = robo_map.y - (yw->_screenSize.y / 2);
        v7.right = robo_map.w + v7.left;
        v7.bottom = robo_map.h + v7.top;
    }

    GFX::Engine.raster_func221(v7);

    sklt_wis *wis = &yw->_hud;

    yw_RenderHUDCompass(yw, wis);

    GFX::Engine.raster_func221(Common::Rect());

    yw_RenderHUDTarget(yw, wis);

    GFX::Engine.raster_func221(v7);

    pcur = yw_RenderHUDInfo(yw, wis, pcur, -0.7, 0.3, yw->_userUnit, -1, 0x10);

    if ( robo_map.IsOpen() )
        GFX::Engine.raster_func221(Common::Rect());

    return pcur;
}

void sb_0x4d7c08__sub0__sub4(NC_STACK_ypaworld *yw)
{
    char *pcur = byte_5C8DB0;

    if ( yw->_userUnit->_status == BACT_STATUS_DEAD )
    {
        std::string msg;

        if ( !(yw->_userRobo->_status_flg & BACT_STFLAG_CLEAN) )
        {
            if ( yw->_userRobo->_status == BACT_STATUS_DEAD )
                msg = Locale::Text::Common(Locale::CMN_HSDESTR);
            else
                msg = Locale::Text::Common(Locale::CMN_UNITDESTR);
        }

        if ( !msg.empty() && yw->_timeStamp / 500 & 1 )
        {
            FontUA::select_tileset(&pcur, 15);
            FontUA::set_xpos(&pcur, 0);
            FontUA::set_center_ypos(&pcur, -(yw->_fontH / 2));

            FontUA::set_txtColor(&pcur, 255, 255, 255);

            pcur = FontUA::FormateCenteredSkipableItem(yw->_guiTiles[15], pcur, msg, yw->_screenSize.x);
        }
    }
    else if ( yw->_userUnit != yw->_userRobo && yw->_hud.field_0 )
    {
        yw_RenderHUDRadare(yw);

        if ( yw->_userUnit->_energy >= yw->_userUnit->_energy_max )
        {
            yw->_hud.field_82 = 3000;
        }
        else
        {
            yw->_hud.field_82 = 3000 * yw->_userUnit->_energy / yw->_userUnit->_energy_max;

            if ( yw->_hud.field_82 < 200 )
                yw->_hud.field_82 = 200;
        }

        yw->_hud.field_86 += (float)yw->_frameTime / (float)yw->_hud.field_82;

        if ( yw->_hud.field_86 >= 1.0 )
            yw->_hud.field_86 = 0;

        pcur = yw_RenderHUDVectorGFX(yw, pcur);
    }

    if ( yw->_guiActFlags & 0x20 )
    {
        yw_RenderCursorOverUnit(yw, yw->_bactOnMouse);
        pcur = yw_RenderUnitLifeBar(yw, pcur, yw->_bactOnMouse);
    }

    if ( yw->_guiVisor.field_18 )
        pcur = yw_RenderUnitLifeBar(yw, pcur, yw->_guiVisor.field_18);

    pcur = yw_RenderOverlayCursors(yw, pcur);

    FontUA::set_end(&pcur);

    w3d_a209 v20;
    v20.cmdbuf = byte_5C8DB0;
    v20.includ = 0;

    GFX::Engine.DrawText(&v20);

    yw->_guiVisor.field_18 = NULL;
}



void sub_4E3B80(NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, SDL_Color *out1, SDL_Color *out2)
{
    float v15 = yw->_hud.field_86 * 2.0 - 1.0;

    float v17 = fabs( v15 - sqrt(POW2(x1) + POW2(y1)) );
    float v16 = fabs( v15 - sqrt(POW2(x2) + POW2(y2)) );

    if ( v17 > 1.0)
        v17 = 1.0;
    else if ( v17 < 0.0)
        v17 = 0.0;

    if ( v16 > 1.0)
        v16 = 1.0;
    else if ( v16 < 0.0)
        v16 = 0.0;

    int v9 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v17 + yw->_hud.cl1_g;
    int v10 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v17 + yw->_hud.cl1_r;
    int v11 =  (yw->_hud.cl2_b - yw->_hud.cl1_b) * v17 + yw->_hud.cl1_b;
    *out1 = GFX::Engine.Color(v10, v9, v11);

    int v12 = (yw->_hud.cl2_g - yw->_hud.cl1_g) * v16 + yw->_hud.cl1_g;
    int v13 = (yw->_hud.cl2_r - yw->_hud.cl1_r) * v16 + yw->_hud.cl1_r;
    int v14 = (yw->_hud.cl2_b - yw->_hud.cl1_b) * v16 + yw->_hud.cl1_b;
    *out2 = GFX::Engine.Color(v13, v12, v14);
}

void sub_4E3D98(NC_STACK_ypaworld *yw, float x1, float y1, float x2, float y2, SDL_Color *out1, SDL_Color *out2)
{
    int v7 = yw->_timeStamp % 300;
    float v9 = v7 / 30000.0;

    sklt_wis *v8 = &yw->_hud;

    int v10 = (v8->cl2_g - v8->cl1_g) * v9 + v8->cl1_g;
    int v11 = (v8->cl2_r - v8->cl1_r) * v9 + v8->cl1_r;
    int v12 = (v8->cl2_b - v8->cl1_b) * v9 + v8->cl1_b;
    *out1 = GFX::Engine.Color(v11, v10, v12);

    int v13 = (v8->cl2_r - v8->cl1_r) * v9 + v8->cl1_r;
    int v16 = (v8->cl2_b - v8->cl1_b) * v9 + v8->cl1_b;
    int v15 = (v8->cl2_g - v8->cl1_g) * v9 + v8->cl1_g;
    *out2 = GFX::Engine.Color(v13, v15, v16);
}

void yw_RenderHUDTarget(NC_STACK_ypaworld *yw, sklt_wis *wis)
{
    UAskeleton::Data *mg_wure = NULL;
    UAskeleton::Data *hud_wure = NULL;
    UAskeleton::Data *wpn_wure = NULL;
    UAskeleton::Data *wpn_wure2 = NULL;

    float v86 = (yw->_timeStamp - wis->field_76 - 350) / 200.0;

    if ( v86 > 0.0 )
    {
        World::TVhclProto *vhcl;

        if ( yw->_userUnit )
            vhcl = &yw->_vhclProtos[ yw->_userUnit->_vehicleID ];
        else
            vhcl = NULL;

        if ( vhcl->hud_wireframe )
            hud_wure = vhcl->hud_wireframe->GetSkelet();
        else
            hud_wure = NULL;

        if ( vhcl->mg_wireframe )
            mg_wure = vhcl->mg_wireframe->GetSkelet();
        else
            mg_wure = wis->sklts_intern[4];

        if ( vhcl->wpn_wireframe_1 )
            wpn_wure = vhcl->wpn_wireframe_1->GetSkelet();
        else if ( yw->_guiVisor.field_4 == 2)
            wpn_wure = wis->sklts_intern[5];
        else if ( yw->_guiVisor.field_4 == 3 )
            wpn_wure = wis->sklts_intern[6];
        else if ( yw->_guiVisor.field_4 == 4 )
            wpn_wure = wis->sklts_intern[7];

        if ( vhcl->wpn_wireframe_2 )
            wpn_wure2 = vhcl->wpn_wireframe_2->GetSkelet();
        else if ( yw->_guiVisor.field_4 == 2)
            wpn_wure2 = wis->sklts_intern[9];
        else if ( yw->_guiVisor.field_4 == 3 )
            wpn_wure2 = wis->sklts_intern[10];
        else if ( yw->_guiVisor.field_4 == 4 )
            wpn_wure2 = wis->sklts_intern[11];

        if ( yw->_guiVisor.field_18 )
        {
            if ( yw->_guiVisor.field_18 != wis->field_7E )
            {
                wis->field_7E = yw->_guiVisor.field_18;
                wis->field_7A = yw->_timeStamp;
            }
        }
        else
        {
            wis->field_7E = NULL;
        }

        if ( hud_wure )
        {
            SDL_Color v14 = yw->GetColor(34);
            yw_RenderVector2D(yw, hud_wure, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.99, 0.99, v14, NULL, NULL, true);
        }

        if ( yw->_guiVisor.field_0 )
        {
            if ( mg_wure )
            {
                SDL_Color v15 = yw->GetColor(26);
                SDL_Color v16 = yw->GetColor(35);

                float a9 = 0.3;

                if ( v86 < 1.4 )
                    a9 = v86 * 0.3;

                wis_color_func func;

                if ( wis->field_72 )
                    func = sub_4E3B80;
                else
                    func = NULL;

                sklt_wis *v19 = &yw->_hud;

                v19->cl1_r = v16.r;
                v19->cl1_g = v16.g;
                v19->cl1_b = v16.b;
                v19->cl2_r = v15.r;
                v19->cl2_g = v15.g;
                v19->cl2_b = v15.b;

                yw_RenderVector2D(yw, mg_wure, yw->_guiVisor.field_8, yw->_guiVisor.field_C, 1.0, 0.0, 0.0, 1.0, a9, 0.4, v16, func, NULL, true);
            }
        }

        if ( yw->_guiVisor.field_4 )
        {
            float v51[2];
            wis_color_func func;

            SDL_Color a11;
            SDL_Color v27;

            if ( yw->_guiVisor.field_18 )
            {
                for (int i = 0; i < 2; i++)
                {
                    v51[i] = (yw->_timeStamp - wis->field_7A) / 200.0;

                    if ( v51[i] > 1.0 )
                        v51[i] = 1.0;
                }

                a11 = yw->GetColor(27);
                v27 = yw->GetColor(36);

                if ( wis->field_72 )
                    func = sub_4E3D98;
                else
                    func = NULL;
            }
            else
            {
                v51[0] = 0;
                v51[1] = 0;

                v27 = yw->GetColor(28);
                a11 = yw->GetColor(37);

                if ( wis->field_72 )
                    func = sub_4E3B80;
                else
                    func = NULL;
            }

            int v63 = a11.r;
            int v67 = a11.g;
            int v66 = a11.b;
            int v68 = v27.r;
            int v64 = v27.b;
            int v65 = v27.g;

            for (int i = 0; i < 2; i++)
            {
                float v77, v78;

                if ( yw->_guiVisor.field_18 )
                {
                    v77 = (yw->_guiVisor.field_10 - yw->_guiVisor.field_8) * v51[i] + yw->_guiVisor.field_8;
                    v78 = (yw->_guiVisor.field_14 - yw->_guiVisor.field_C) * v51[i] + yw->_guiVisor.field_C;
                }
                else
                {
                    v77 = yw->_guiVisor.field_10;
                    v78 = yw->_guiVisor.field_14;
                }

                float v80 = v51[i] * -0.23 + 0.4;
                float v74 = v51[i] * -0.17 + 0.3;

                if ( v86 < 1.4 )
                    v80 = v80 * v86;

                float v85;

                if ( i & 1 )
                {
                    v85 = v51[i] * 1.571;
                }
                else if ( v51[i] >= 1.0 )
                {
                    v85 = (yw->_timeStamp - wis->field_7A) * 0.001 * 6.282;
                }
                else
                {
                    v85 = v51[i] * -3.141;
                }

                float v35 = sin(v85);
                float v83 = cos(v85);

                UAskeleton::Data *v36;

                if ( i & 1 )
                    v36 = wpn_wure;
                else
                    v36 = wpn_wure2;

                if ( v36 )
                {
                    yw->_hud.cl1_r = v63;
                    yw->_hud.cl1_g = v67;
                    yw->_hud.cl1_b = v66;
                    yw->_hud.cl2_r = v68;
                    yw->_hud.cl2_g = v65;
                    yw->_hud.cl2_b = v64;

                    yw_RenderVector2D(yw, v36, v77, v78, v83, -v35, v35, v83, v74, v80, a11, func, 0, true);
                }
            }
        }
    }
}

void yw_RenderCursorOverUnit(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact)
{
    float v6 = bact->_position.x - yw->_viewerPosition.x;
    float v4 = bact->_position.y - yw->_viewerPosition.y;
    float v8 = bact->_position.z - yw->_viewerPosition.z;

    mat3x3 corrected = yw->_viewerRotation;
    GFX::Engine.matrixAspectCorrection(corrected, false);

    float a3  = corrected.m00 * v6 + corrected.m01 * v4 + corrected.m02 * v8;
    float v33 = corrected.m10 * v6 + corrected.m11 * v4 + corrected.m12 * v8;
    float v30 = corrected.m20 * v6 + corrected.m21 * v4 + corrected.m22 * v8;

    if ( v30 > 30.0 && a3 < v30 )
    {
        if ( a3 > -v30 && v33 < v30 && v33 > -v30 )
        {
            SDL_Color v11 = yw->GetColor(bact->_owner);
            UAskeleton::Data *v12 = yw->_hud.sklts_intern[13];

            float a3a = a3 / v30;
            float v34 = v33 / v30;

            if ( v12 )
            {
                float a4 = v34 - 0.08;

                if ( bact->IsParentMyRobo() && bact->_owner == yw->_userRobo->_owner )
                {
                    yw_RenderVector2D(yw, v12, a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.015, 0.02, v11, NULL, NULL, true);
                    yw_RenderVector2D(yw, v12, a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.005, 0.00666, v11, NULL, NULL, true);
                }
                else
                {
                    yw_RenderVector2D(yw, v12, a3a, a4, 1.0, 0.0, 0.0, 1.0, 0.0075, 0.01, v11, NULL, NULL, true);
                }
            }
        }
    }
}


char *sb_0x4d7c08__sub0__sub4__sub0__sub0(NC_STACK_ypaworld *yw, char *cur, NC_STACK_ypabact *bact)
{
    char *pcur = cur;

    if ( yw->_GameShell )
    {
        if ( yw->_isNetGame )
        {
            if ( !yw->_GameShell->netPlayers[bact->_owner].Name.empty() )
            {
                float v5 = bact->_position.x - yw->_viewerPosition.x;
                float v6 = bact->_position.y - yw->_viewerPosition.y;
                float v10 = bact->_position.z - yw->_viewerPosition.z;

                mat3x3 corrected = yw->_viewerRotation;
                GFX::Engine.matrixAspectCorrection(corrected, false);

                float v32 = corrected.m00 * v5 + corrected.m01 * v6 + corrected.m02 * v10;
                float v31 = corrected.m10 * v5 + corrected.m11 * v6 + corrected.m12 * v10;
                float v26 = corrected.m20 * v5 + corrected.m21 * v6 + corrected.m22 * v10;

                if ( v26 > 30.0 && v32 < v26 )
                {
                    if ( v32 > -v26 && v31 < v26 && v31 > -v26 )
                    {
                        int v30 = yw->_guiTiles[15]->h;

                        int v27_4 = ((yw->_screenSize.x / 2) * (v32 / v26 + 1.0));
                        int v15 = (yw->_screenSize.y / 2) * (v31 / v26 + 1.0);

                        int v28 = 96;

                        if ( v27_4 + 96 >= yw->_screenSize.x )
                            v28 = yw->_screenSize.x - v27_4 - 1;

                        if ( v28 > 0 && v27_4 >= 0 )
                        {
                            if ( v28 + v27_4 <  yw->_screenSize.x && v15 >= 0 )
                            {
                                if ( v30 + v15 < yw->_screenSize.y )
                                {
                                    FontUA::select_tileset(&pcur, 15);
                                    FontUA::set_center_xpos(&pcur, v27_4 - (yw->_screenSize.x / 2) );
                                    FontUA::set_center_ypos(&pcur, v15 - (yw->_screenSize.y / 2) );

                                    FontUA::set_txtColor(&pcur, yw->_iniColors[ bact->_owner ].r, yw->_iniColors[ bact->_owner ].g, yw->_iniColors[ bact->_owner ].b);

                                    pcur = FontUA::FormateClippedText(yw->_guiTiles[15], pcur,  yw->_GameShell->netPlayers[bact->_owner].Name, v28, 32);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return pcur;
}

char * yw_RenderOverlayCursors(NC_STACK_ypaworld *yw, char *cur)
{
    char *pcur = cur;

    if ( yw->_preferences & World::PREF_ENEMYINDICATOR )
    {
        int v5 = yw->_userUnit->_cellId.x - 1;
        int v16 = yw->_userUnit->_cellId.x + 1;
        int v17 = yw->_userUnit->_cellId.y - 1;
        int v14 = yw->_userUnit->_cellId.y + 1;

        if ( v5 < 1 )
            v5 = 1;

        if ( v17 < 1 )
            v17 = 1;

        if ( v16 >= yw->_mapSize.x )
            v16 = yw->_mapSize.x - 1;

        if ( v14 >= yw->_mapSize.y )
            v14 = yw->_mapSize.y - 1;

        for (int i = v17; i <= v14; i++ )
        {
            for (int  j = v5; j <= v16; j++ )
            {
                cellArea &v8 = yw->_cells(j, i);

                if ( robo_map.MapViewMask & v8.view_mask )
                {
                    for ( NC_STACK_ypabact* &bct : v8.unitsList )
                    {

                        if ( bct->_bact_type != BACT_TYPES_MISSLE && bct->_bact_type != BACT_TYPES_ROBO )
                        {

                            if ( bct->_status != BACT_STATUS_CREATE && bct->_status != BACT_STATUS_BEAM && bct->_status != BACT_STATUS_DEAD )
                            {
                                if ( bct->_bact_type != BACT_TYPES_GUN )
                                    yw_RenderCursorOverUnit(yw, bct);
                                else
                                {
                                    NC_STACK_ypagun *gun = (NC_STACK_ypagun *)bct;

                                    if ( !gun->IsRoboGun() )
                                        yw_RenderCursorOverUnit(yw, bct);
                                }

                                if ( yw->_isNetGame )
                                {
                                    if ( bct->_status_flg & BACT_STFLAG_ISVIEW )
                                    {
                                        if ( yw->_GameShell )
                                            pcur = sb_0x4d7c08__sub0__sub4__sub0__sub0(yw, pcur, bct);
                                    }
                                }
                            }

                        }
                    }



                }
            }
        }
    }
    return pcur;
}


char * yw_RenderUnitLifeBar(NC_STACK_ypaworld *yw, char *cur, NC_STACK_ypabact *bact)
{
    // Render fraction triangles above units
    char *pcur = cur;

    float v6 = bact->_position.x - yw->_viewerPosition.x;
    float v5 = bact->_position.y - yw->_viewerPosition.y;
    float v9 = bact->_position.z - yw->_viewerPosition.z;

    mat3x3 corrected = yw->_viewerRotation;
    GFX::Engine.matrixAspectCorrection(corrected, false);

    float v44 = corrected.m00 * v6 + corrected.m01 * v5 + corrected.m02 * v9;
    float v46 = corrected.m10 * v6 + corrected.m11 * v5 + corrected.m12 * v9;
    float v37 = corrected.m20 * v6 + corrected.m21 * v5 + corrected.m22 * v9;

    if ( v37 > 30.0 && v44 < v37 )
    {
        if ( v44 > -v37 && v46 < v37 && v46 > -v37 )
        {
            int v11 = 0;
            int v33 = 0;
            int v34 = 0;
            int v36 = 0;
            int v30 = 0;

            if ( robo_map.IsOpen() )
            {
                v30 = 1;
                v11 = robo_map.x;
                v33 = v11 + robo_map.w;
                v36 = robo_map.y;
                v34 = v36 + robo_map.h;
            }

            float v45 = v44 * (1.0 / v37);
            float v47 = v46 * (1.0 / v37);

            int v13;

            if ( yw->_screenSize.x >= 512 )
                v13 = 8;
            else
                v13 = 4;

            int v43 = v13 * yw->_guiTiles[50]->map[0].w;

            int v42 = (yw->_screenSize.x / 2) * (v45 + 1.0);
            int v41 = (yw->_screenSize.y / 2) * (v47 + 1.0);

            if ( !v30 || v42 <= v11 || v42 >= v33 || v41 <= v36 || v41 >= v34 )
            {
                v42 -= v43 / 2;
                v41 -= (yw->_guiTiles[50]->h / 2) + (yw->_screenSize.y / 16);

                if ( v42 >= 0 )
                {
                    if ( v42 + v43 < yw->_screenSize.x && v41 >= 0 )
                    {
                        if ( yw->_guiTiles[50]->h + v41 < yw->_screenSize.y )
                        {
                            FontUA::select_tileset(&pcur, 50);

                            FontUA::set_center_xpos(&pcur, v42 - (yw->_screenSize.x / 2) );
                            FontUA::set_center_ypos(&pcur, v41 - (yw->_screenSize.y / 2) );

                            int v38 = bact->_energy_max / v13;

                            int v27 = v38 / 2; //v38 - (v38 / 2);

                            for (int i = 1; i <= v13; i++)
                            {
                                if ( v27 > bact->_energy )
                                    FontUA::store_u8(&pcur, 6);
                                else
                                    FontUA::store_u8(&pcur, 2);

                                v27 += v38;
                            }
                        }
                    }
                }
            }
        }
    }
    return pcur;
}

void sb_0x4d7c08__sub0__sub4__sub2__sub0(NC_STACK_ypaworld *yw)
{
    // Render units on radare
    w3d_a209 post_rndr;
    post_rndr.cmdbuf = byte_5FFF80;
    post_rndr.includ = NULL;

    char *pcur = byte_5FFF80;
    FontUA::select_tileset(&pcur, 61);

    float v6 = yw->_screenSize.x / 2;
    float v7 = yw->_screenSize.y / 2;

    robo_map.field_1F8 = dround((robo_map.field_25C - robo_map.field_254) * v6);
    robo_map.field_1FC = dround((robo_map.field_260 - robo_map.field_258) * v7);

    robo_map.field_1F0 = dround(robo_map.field_1D8 / robo_map.field_1E0) - (robo_map.field_1F8 / 2);
    robo_map.field_1F4 = -dround(robo_map.field_1DC / robo_map.field_1E4) - (robo_map.field_1FC / 2);

    robo_map.field_200 = dround(v6 * robo_map.field_254);
    robo_map.field_204 = dround(v7 * robo_map.field_258);

    Common::Rect drect;
    drect.left = robo_map.field_200;
    drect.top = robo_map.field_204;
    drect.right = robo_map.field_1F8 + robo_map.field_200 - 1;
    drect.bottom = robo_map.field_1FC + robo_map.field_204 - 1;

    GFX::Engine.raster_func211(drect);

    int v14 = dround(robo_map.field_1F0 * robo_map.field_1E0) / World::CVSectorLength;
    int v29 = dround(robo_map.field_1F4 * robo_map.field_1E4) / World::CVSectorLength;
    int v28 = dround( (robo_map.field_1F8 + robo_map.field_1F0) * robo_map.field_1E0 ) / World::CVSectorLength;
    int v30 = dround( (robo_map.field_1FC + robo_map.field_1F4) * robo_map.field_1E4 ) / World::CVSectorLength;

    if ( v14 < 1 )
        v14 = 1;

    if ( v29 < 1 )
        v29 = 1;

    if ( v28 >= yw->_mapSize.x )
        v28 = yw->_mapSize.x - 1;

    if ( v30 >= yw->_mapSize.y )
        v30 = yw->_mapSize.y - 1;

    int v38 = 0;

    for (int i = v29; i <= v30; i++ )
    {
        for (int j = v14; j <= v28; j++ )
        {
            if ( v38 < 512 ) // Units maximum?
            {
                cellArea &v17 = yw->_cells(j, i);

                if ( robo_map.MapViewMask & v17.view_mask )
                {
                    for( NC_STACK_ypabact* &bact : v17.unitsList )
                    {
                        int v19 = 0;
                        if ( robo_map.IsOpen() )
                        {
                            Common::PointRect r { robo_map.x - (yw->_screenSize.x / 2)
                                                , robo_map.y - (yw->_screenSize.y / 2)
                                                , robo_map.w
                                                , robo_map.h };

                            if ( r.IsIn( sub_4F681C( bact->_position.XZ() ) ) )
                                v19 = 1;
                        }

                        if ( bact->_bact_type == BACT_TYPES_ROBO )
                        {
                            if ( !v19 )
                            {
                                FontUA::select_tileset(&pcur, 1);

                                pcur = sub_4F6DFC(yw, pcur, yw->_guiTiles[1]->h, yw->_guiTiles[1]->map[24].w, bact, 0);

                                FontUA::select_tileset(&pcur, 61);

                                v38++;
                            }
                        }
                        else if ( bact->_bact_type == BACT_TYPES_GUN )
                        {
                            NC_STACK_ypagun *gun = (NC_STACK_ypagun *)bact;

                            if ( (!gun->IsRoboGun() || bact == yw->_userUnit) && !v19 )
                            {
                                pcur = sub_4F6DFC(yw, pcur, 7, 7, bact, 0);

                                v38++;
                            }
                        }
                        else if ( !v19 )
                        {
                            pcur = sub_4F6DFC(yw, pcur, 7, 7, bact, 0);

                            v38++;
                        }
                    }
                }
            }
        }
    }

    FontUA::set_end(&pcur);

    GFX::Engine.DrawText(&post_rndr);
}

void yw_RenderHUDRadare(NC_STACK_ypaworld *yw)
{
    float v12 = robo_map.field_1D8;
    float v15 = robo_map.field_1DC;
    float v14 = robo_map.field_1E0;
    float v13 = robo_map.field_1E4;
    int v7 = robo_map.field_1EC;
    int32_t v8 = robo_map.MapViewMask;
    int v9 = robo_map.field_1EE;

    robo_map.field_1EC = 1;
    robo_map.MapViewMask = -1;
    robo_map.field_1E8 |= 0x100;
    robo_map.field_1EE = 3;
    robo_map.field_1D8 = yw->_viewerBact->_position.x;
    robo_map.field_1DC = yw->_viewerBact->_position.z;
    robo_map.field_1E0 = 4800.0 / ((float)(yw->_screenSize.x / 2) * (robo_map.field_25C - robo_map.field_254));
    robo_map.field_1E4 = 4800.0 / ((float)(yw->_screenSize.y / 2) * (robo_map.field_260 - robo_map.field_258));

    float v19 = yw->_viewerRotation.m20;
    float v17 = yw->_viewerRotation.m22;

    float v18, v20;

    float v16 = sqrt( POW2(v17) + POW2(v19));
    if ( v16 <= 0.0 )
    {
        v18 = 1.0;
        v20 = 0.0;
    }
    else
    {
        v20 = v19 / v16;
        v18 = v17 / v16;
    }
    robo_map.field_264 = v18;

    flt_5164F4 = -v20;
    flt_516504 = v20;
    flt_51650C = v18;

    sb_0x4d7c08__sub0__sub4__sub2__sub0(yw);

    robo_map.field_1EC = v7;
    robo_map.field_1EE = v9;
    robo_map.field_1D8 = v12;
    robo_map.MapViewMask = v8;
    robo_map.field_1DC = v15;
    robo_map.field_1E0 = v14;
    robo_map.field_1E8 &= 0xFFFFFEFF;
    robo_map.field_1E4 = v13;
}


struct aab
{
    int a;
    int b;
};

int sub_47F360(const void *a1, const void *a2)
{
    return (((const aab *)a2)->b) - (((const aab *)a1)->b);
}

char *sb_0x4d7c08__sub0__sub2__sub1(NC_STACK_ypaworld *yw, char *cur, const std::array<World::TPlayerStatus, World::CVFractionsCount> &statuses, int a3)
{
    char *pcur = cur;

    if ( yw->_isNetGame )
    {
        aab v25[8];

        int nums = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( (1 << i) & yw->_levelInfo.OwnerMask )
            {
                v25[nums].a = i;
                v25[nums].b = statuses[i].Score;
                nums++;
            }
        }

        qsort(v25, nums, sizeof(aab), sub_47F360);

        for (int i = 0; i < nums; i++)
        {
            int clrid;
            
            FontUA::ColumnItem a4a[2];

            switch ( v25[i].a )
            {
            case 1:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_RESIST);
                clrid = 1;
                break;

            case 2:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_SULG);
                clrid = 2;
                break;

            case 3:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_MYKO);
                clrid = 3;
                break;

            case 4:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_TAER);
                clrid = 4;
                break;

            case 5:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_BLKSCT);
                clrid = 5;
                break;

            case 6:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_GHOR);
                clrid = 6;
                break;

            default:
                a4a[0].txt = Locale::Text::Advanced(Locale::ADV_NEUTRAL);
                clrid = 7;
                break;
            }

            FontUA::set_txtColor(&pcur, yw->_iniColors[clrid].r, yw->_iniColors[clrid].g, yw->_iniColors[clrid].b);

            a4a[0].spaceChar = 32;
            a4a[0].prefixChar = 0;
            a4a[0].postfixChar = 0;
            a4a[0].fontID = 15;
            a4a[0].flags = 36;
            a4a[0].width = a3 * 0.5;

            a4a[1].txt = fmt::sprintf("%d", statuses[ v25[i].a ].Score);
            a4a[1].width = a3 * 0.5;
            a4a[1].fontID = 15;
            a4a[1].spaceChar = 32;
            a4a[1].flags = 36;
            a4a[1].prefixChar = 0;
            a4a[1].postfixChar = 0;

            pcur = FormateColumnItem(yw, pcur, 2, a4a);

            FontUA::next_line(&pcur);
        }

        FontUA::next_line(&pcur);
    }
    return pcur;
}


char * sb_0x4d7c08__sub0__sub2__sub0(NC_STACK_ypaworld *yw, char *cur, int a3)
{
    char *pcur = cur;

    if ( yw->_isNetGame )
    {
        if ( yw->_countUnitsPerOwner[ yw->_userRobo->_owner ] <= yw->_levelUnitLimit )
            FontUA::set_txtColor(&pcur, yw->_iniColors[63].r, yw->_iniColors[63].g, yw->_iniColors[63].b);
        else
            FontUA::set_txtColor(&pcur, yw->_iniColors[6].r, yw->_iniColors[6].g, yw->_iniColors[6].b);

        FontUA::ColumnItem a4[2];

        a4[0].txt = Locale::Text::Advanced(Locale::ADV_UNITS);
        a4[0].width = a3 * 0.5;
        a4[0].spaceChar = 32;
        a4[0].fontID = 15;
        a4[0].prefixChar = 0;
        a4[0].postfixChar = 0;
        a4[0].flags = 36;

        a4[1].txt = fmt::sprintf("%d / %d", yw->_countUnitsPerOwner[ yw->_userRobo->_owner ], yw->_levelUnitLimit);
        a4[1].width = a3 * 0.5;
        a4[1].fontID = 15;
        a4[1].postfixChar = 0;
        a4[1].spaceChar = 32;
        a4[1].flags = 36;
        a4[1].prefixChar = 0;

        pcur = FormateColumnItem(yw, pcur, 2, a4);

        FontUA::next_line(&pcur);
    }
    return pcur;
}

void sb_0x4d7c08__sub0__sub2(NC_STACK_ypaworld *yw)
{
    char buf[1024];
    char *pcur = buf;

    int v25 = 2 * yw->_screenSize.x / 3;
    int v2 = yw->_screenSize.x - v25;

    FontUA::select_tileset(&pcur, 15);
    FontUA::set_xpos(&pcur, v25);
    FontUA::set_ypos(&pcur, up_panel.field_1CC + (yw->_fontH / 2));

    pcur = sb_0x4d7c08__sub0__sub2__sub0(yw, pcur, v2);
    pcur = sb_0x4d7c08__sub0__sub2__sub1(yw, pcur, yw->_gameplayStats, v2);

    for (const TMapSuperItem &sitem : yw->_levelInfo.SuperItems)
    {
        if ( sitem.Type != 0 )
        {
            std::string timeStr;
            std::string typeStr = "SUPER ITEM";

            if (sitem.Type == TMapSuperItem::TYPE_BOMB)
                typeStr = Locale::Text::Common(Locale::CMN_BOMBNAME);
            else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
                typeStr = Locale::Text::Common(Locale::CMN_WAVENAME);

            int v23 = 0;

            if ( sitem.State == TMapSuperItem::STATE_ACTIVE || sitem.State == TMapSuperItem::STATE_STOPPED )
            {
                int v10 = (sitem.CountDown + 1023) / 1024;

                if ( v10 < 0 )
                    v10 = 0;

                timeStr = fmt::sprintf("%s: %02d:%02d", typeStr, v10 / 60 % 60, v10 % 60);

                v23 = 1;
            }
            else if ( sitem.State == TMapSuperItem::STATE_TRIGGED )
            {
                timeStr = fmt::sprintf("%s: %s", typeStr,  Locale::Text::Advanced(Locale::ADV_TRIGGERED) );

                v23 = 1;
            }

            if ( v23 )
            {
                FontUA::set_xpos(&pcur, v25);

                FontUA::set_txtColor(&pcur, yw->_iniColors[sitem.ActivateOwner].r, yw->_iniColors[sitem.ActivateOwner].g, yw->_iniColors[sitem.ActivateOwner].b);

                pcur = FontUA::FormateClippedText(yw->_guiTiles[15], pcur, timeStr.c_str(), yw->_screenSize.x - v25, 32);

                FontUA::next_line(&pcur);
            }
        }
    }

    FontUA::set_end(&pcur);

    w3d_a209 a209;
    a209.cmdbuf = buf;
    a209.includ = NULL;

    GFX::Engine.raster_func209(&a209);
}




void ypaworld_func2__sub0__sub1(NC_STACK_ypaworld *yw, NC_STACK_ypabact *bact1, NC_STACK_ypabact *bact2)
{
    if ( bact2->_bact_type == BACT_TYPES_ROBO )
    {
        if ( yw->_roboMapStatus.Valid )
        {
            if ( yw->_roboMapStatus.IsOpen )
            {
                yw->GuiWinOpen( &robo_map );
                yw->GuiWinToFront( &robo_map );
            }
            else
                yw->GuiWinClose( &robo_map );

            robo_map.x = yw->_roboMapStatus.Rect.x;
            robo_map.y = yw->_roboMapStatus.Rect.y;
            robo_map.w = yw->_roboMapStatus.Rect.w;
            robo_map.h = yw->_roboMapStatus.Rect.h;
            robo_map.field_1EC = yw->_roboMapStatus.Data[0];
            robo_map.field_1ED = yw->_roboMapStatus.Data[1];
            robo_map.field_1EE = yw->_roboMapStatus.Data[2];
            robo_map.field_208 = yw->_roboMapStatus.Data[3];
            robo_map.field_20A = yw->_roboMapStatus.Data[4];
            robo_map.field_20C = yw->_roboMapStatus.Data[5];
            robo_map.field_20E = yw->_roboMapStatus.Data[6];

            sub_4C1970(yw, 0);
        }

        if ( yw->_roboFinderStatus.Valid )
        {
            if ( yw->_roboFinderStatus.IsOpen )
            {
                yw->GuiWinOpen( &squadron_manager );
                yw->GuiWinToFront( &squadron_manager );
            }
            else
                yw->GuiWinClose( &squadron_manager );

            squadron_manager.x = yw->_roboFinderStatus.Rect.x;
            squadron_manager.y = yw->_roboFinderStatus.Rect.y;
            squadron_manager.w = yw->_roboFinderStatus.Rect.w;
            squadron_manager.h = yw->_roboFinderStatus.Rect.h;

            squadron_manager.SetRect(yw, -2, -2);
        }
    }
    else if ( bact1->_bact_type == BACT_TYPES_ROBO )
    {
        if ( yw->_vhclMapStatus.Valid )
        {
            if ( yw->_vhclMapStatus.IsOpen )
            {
                yw->GuiWinOpen( &robo_map );
                yw->GuiWinToFront( &robo_map );
            }
            else
                yw->GuiWinClose( &robo_map );

            robo_map.x = yw->_vhclMapStatus.Rect.x;
            robo_map.y = yw->_vhclMapStatus.Rect.y;
            robo_map.w = yw->_vhclMapStatus.Rect.w;
            robo_map.h = yw->_vhclMapStatus.Rect.h;
            robo_map.field_1EC = yw->_vhclMapStatus.Data[0];
            robo_map.field_1ED = yw->_vhclMapStatus.Data[1];
            robo_map.field_1EE = yw->_vhclMapStatus.Data[2];
            robo_map.field_208 = yw->_vhclMapStatus.Data[3];
            robo_map.field_20A = yw->_vhclMapStatus.Data[4];
            robo_map.field_20C = yw->_vhclMapStatus.Data[5];
            robo_map.field_20E = yw->_vhclMapStatus.Data[6];

            sub_4C1970(yw, 0);
        }

        if ( yw->_vhclFinderStatus.Valid )
        {
            if ( yw->_vhclFinderStatus.IsOpen )
            {
                yw->GuiWinOpen( &squadron_manager );
                yw->GuiWinToFront( &squadron_manager );
            }
            else
                yw->GuiWinClose( &squadron_manager );

            squadron_manager.x = yw->_vhclFinderStatus.Rect.x;
            squadron_manager.y = yw->_vhclFinderStatus.Rect.y;
            squadron_manager.w = yw->_vhclFinderStatus.Rect.w;
            squadron_manager.h = yw->_vhclFinderStatus.Rect.h;

            squadron_manager.SetRect(yw, -2, -2);
        }
    }
    else if ( bact1->_status == BACT_STATUS_DEAD )
    {
        if ( yw->_vhclMapStatus.Valid )
        {
            if ( yw->_vhclMapStatus.IsOpen )
            {
                yw->GuiWinOpen( &robo_map );
                yw->GuiWinToFront( &robo_map );
            }
        }

        if ( yw->_vhclFinderStatus.Valid )
        {
            if ( yw->_vhclFinderStatus.IsOpen )
            {
                yw->GuiWinOpen( &squadron_manager );
                yw->GuiWinToFront( &squadron_manager );
            }
        }

        if ( bact2->_bact_type != BACT_TYPES_ROBO )
            yw->_invulnerable = 1;
    }

    if ( bact2->_bact_type != BACT_TYPES_ROBO )
        robo_map.field_1ED = 1;
}










int NC_STACK_ypaworld::yw_MouseFindCreationPoint(TClickBoxInf *winp)
{
    const World::TVhclProto &vhcl = _vhclProtos[bzda.field_2DC[bzda.field_8EC]];

    vec3d v47( (float)(winp->move.ScreenPos.x - (_screenSize.x / 2)) / (float)(_screenSize.x / 2),
               (float)(winp->move.ScreenPos.y - (_screenSize.y / 2)) / (float)(_screenSize.y / 2),
               1.0 );

    v47.normalise();

    float v63 = vhcl.radius * 4.0 + 200.0;

    float v62 = 0.0;

    if ( v63 > 0.0 )
    {
        vec3d v51 = v47 * v63;

        while ( v62 < v63 )
        {
            float v60 = 0.0;

            while ( v60 < C_2PI )
            {

                vec3d v55 = v51 + vec3d(v62 * cos(v60),  v62 * sin(v60),  0.0);

                mat3x3 corrected = _viewerRotation;
                GFX::Engine.matrixAspectCorrection(corrected, true);

                vec3d v37 = corrected.Transpose().Transform(v55);

                ypaworld_arg136 arg149;
                arg149.stPos = _viewerPosition;
                arg149.vect = v37;
                arg149.flags = 0;

                ypaworld_func149(&arg149);

                if ( !arg149.isect )
                {
                    float v48 = v37.length();

                    if ( v48 > 0.0 )
                        v37 /= v48;

                    _mouseVector = v37;
                    _mouseSelDistance = v48;

                    return 1;
                }

                v60 += 0.349;
            }

            v62 += v63 * 0.1;
        }
    }

    return 0;
}

int NC_STACK_ypaworld::sb_0x4d3d44(TClickBoxInf *winp)
{
    if ( _guiActFlags & 8 || bzda.field_8EC == -1 )
        return 4;

    NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(_userRobo);

    int a4 = robo->getROBO_battVehicle();

    if ( _updateMessage.energy > a4 )
        return 2;


    if ( !yw_MouseFindCreationPoint(winp) )
        return 1;

    if ( _isNetGame && !_levelUnitLimitType && _levelUnitLimit <= _countUnitsPerOwner[_userRobo->_owner] )
        return 3;

    return 0;
}

int NC_STACK_ypaworld::UserActBuildCheck()
{
    if ( !(_guiActFlags & 0x10) || bzda.field_8F4 == -1 )
        return 1; // NO with no tip

    NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(_userRobo);

    int a4 = robo->getROBO_battVehicle();

    if ( _updateMessage.energy > a4 )
        return 6; // TIP_NOENERGY

    if ( _cellOnMouse->owner && _cellOnMouse->owner != _userRobo->_owner )
        return 2; // TIP_NOTOWN

    Common::Point cellDist = _cellOnMouse->CellId.AbsDistance( _userRobo->_cellId );

    if ( cellDist.IsNull() )
        return 3; // TIP_BUILD_TOOCLOSE

    if ( cellDist.x > 1 || cellDist.y > 1 )
        return 4; // TIP_BUILD_TOOFAR

    if ( _cellOnMouse->PurposeType == cellArea::PT_CONSTRUCTING ||
         (!_allowMultiBuildLevel && IsAnyBuildingProcess(_playerOwner)) )
        return 5; // TIP_BUILD_INPROC

    if ( _cellOnMouse->PurposeType == cellArea::PT_TECHUPGRADE || 
         _cellOnMouse->PurposeType == cellArea::PT_GATECLOSED || 
         _cellOnMouse->PurposeType == cellArea::PT_GATEOPENED || 
         _cellOnMouse->PurposeType == cellArea::PT_STOUDSON || 
         (_cellOnMouse->PurposeType == cellArea::PT_TECHDEACTIVE && _isNetGame) )
        return 1; // NO with no tip

    return 0; // Can build
}


int NC_STACK_ypaworld::ypaworld_func64__sub21__sub3()
{
    if ( !(_guiActFlags & 0x10) )
        return 6;

    if ( !_cellOnMouse->IsCanSee(_userRobo->_owner) )
        return 3;

    if ( _cellOnMouse->PurposeType == cellArea::PT_CONSTRUCTING )
        return 6;

    float v5 = _cellMouseIsectPos.x - _userRobo->_position.x;
    float v6 = _cellMouseIsectPos.y - _userRobo->_position.y;
    float v8 = _cellMouseIsectPos.z - _userRobo->_position.z;

    float v15 = sqrt(POW2(v5) + POW2(v6) + POW2(v8));

    if ( v15 < 100.0 )
        return 4;

    int v18 = 0;

    if ( _cellOnMouse->PurposeType != cellArea::PT_GATEOPENED && (_cellOnMouse->PurposeType != cellArea::PT_POWERSTATION || _cellOnMouse->owner != _userRobo->_owner) )
    {
        v18 = (POW2(v15) / 230.4);
    }

    NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(_userRobo);

    int a4 = robo->getROBO_battBeam();

    _updateMessage.energy = v18;

    if ( v18 > a4 )
        return 1;

    for ( NC_STACK_ypabact* &v11 : _cellOnMouse->unitsList )
    {
        if ( v11->_bact_type == BACT_TYPES_ROBO && v11->_status != BACT_STATUS_DEAD && v11->_owner != _userRobo->_owner )
            return 5;
    }

    float v20 = 100000.0;
    float v19 = -100000.0;

    ypaworld_arg136 arg136;
    arg136.vect.x = 0;
    arg136.vect.y = 50000.0;
    arg136.vect.z = 0;
    arg136.flags = 0;

    for (int j = -3; j <= 3; j++)
    {
        for (int i = -3; i <= 3; i++)
        {
            arg136.stPos.y = -25000.0;
            arg136.stPos.x = _cellMouseIsectPos.x - j * 57.14285714285715;
            arg136.stPos.z = _cellMouseIsectPos.z - i * 57.14285714285715;

            ypaworld_func136(&arg136);

            if ( arg136.isect )
            {
                if ( arg136.isectPos.y < v20 )
                    v20 = arg136.isectPos.y;

                if ( arg136.isectPos.y > v19 )
                    v19 = arg136.isectPos.y;
            }
        }
    }

    if ( abs(v19 - v20) > 800 )
        return 2;

    if ( _cellMouseIsectPos.x < 1500.0 || _cellMouseIsectPos.z > -1500.0 || _cellMouseIsectPos.x > _mapLength.x - 1500.0 || _cellMouseIsectPos.z < 1500.0 - _mapLength.y )
        return 2;

    _cellMouseIsectPos.y = v20 - _userRobo->_height;
    return 0;
}

int NC_STACK_ypaworld::ypaworld_func64__sub21__sub4(TInputState *arg, int a3)
{
    if ( !_makingWaypointsMode && arg->Buttons.Is(4) )
    {
        _makingWaypointsMode = true;
        _waypointCount = 0;
        return a3;
    }

    if ( _makingWaypointsMode && !(arg->Buttons.Is(4)) )
    {
        _makingWaypointsMode = false;
        return a3;
    }

    if ( _makingWaypointsMode   &&   a3 == World::DOACTION_2 &&   arg->ClickInf.flag & TClickBoxInf::FLAG_LM_DOWN )
    {
        if ( _waypointCount )
        {
            if ( (_firstWaypointPosition.XZ() - _cellMouseIsectPos.XZ()).length() < 100.0 )
            {
                _waypointCount++;
                return World::DOACTION_WP_CYCLE;
            }

            _waypointCount++;

            return World::DOACTION_WP_ADD;
        }
        else
        {
            _firstWaypointPosition = _cellMouseIsectPos;

            _waypointCount++;

            return World::DOACTION_WP_BEGIN;
        }
    }

    return a3;
}

bool NC_STACK_ypaworld::ypaworld_func64__sub21__sub6(TClickBoxInf *winp)
{
    if ( winp->flag & TClickBoxInf::FLAG_DBL_CLICK
            && _bactPrevClicked
            && Common::ABS(winp->move.ScreenPos.x - _prevMousePos.x) < 4
            && Common::ABS(winp->move.ScreenPos.y - _prevMousePos.y) < 4 )
    {
        _prevMousePos = Common::Point();

        _bactOnMouse = _bactPrevClicked;
        _bactPrevClicked = NULL;

        _guiActFlags |= 0x20;

        return true;
    }
    else
    {
        if ( (winp->flag & TClickBoxInf::FLAG_LM_DOWN)   &&   _guiActFlags & 0x20   &&   _userRobo->_owner == _bactOnMouse->_owner )
        {
            _bactPrevClicked = _bactOnMouse;
            _prevMousePos = winp->move.ScreenPos;
        }
        else if ( winp->flag & TClickBoxInf::FLAG_LM_DOWN )
        {
            if ( !(_guiActFlags & 0x20) )
            {
                _bactPrevClicked = NULL;
                _prevMousePos = Common::Point();
            }
        }
    }
    return false;
}


void NC_STACK_ypaworld::ypaworld_func64__sub21__sub1__sub0(TInputState *arg)
{
    _guiActFlags = 0;

    TClickBoxInf *winp = &arg->ClickInf;

    if ( arg->ClickInf.flag & TClickBoxInf::FLAG_OK )
    {
        if ( winp->selected_btn )
        {
            _guiActFlags |= 4;

            if ( winp->selected_btn == &robo_map )
            {
                if ( winp->selected_btnID == 17 )
                    _guiActFlags |= 8;
            }
            else if ( winp->selected_btn == &squadron_manager )
            {
                if ( winp->selected_btnID >= 8 && (int32_t)(squadron_manager.firstShownEntries + winp->selected_btnID - 8) < squadron_manager.numEntries )
                    _guiActFlags |= 0x40;
            }
        }

        if ( bzda.field_1D0 == 8 )
            _guiActFlags |= 2;
        else if ( bzda.field_1D0 == 1 || bzda.field_1D0 == 2 || bzda.field_1D0 == 4 || bzda.field_1D0 == 16 || bzda.field_1D0 == 32 )
            _guiActFlags |= 0x83;
    }
}

void NC_STACK_ypaworld::yw_MAP_MouseSelect(TClickBoxInf *winp)
{
    int v23 = winp->move.BtnPos.x + robo_map.field_1D8 / robo_map.field_1E0 - (robo_map.field_1F8 / 2);
    int v24 = winp->move.BtnPos.y - (robo_map.field_1DC / robo_map.field_1E4) - (robo_map.field_1FC / 2);

    int v9 = robo_map.field_1EE + 2;

    Common::Point pt( v23 >> v9, v24 >> v9);

    if ( IsGamePlaySector( pt ) )
    {
        cellArea &cell = _cells( pt );

        if ( _guiActFlags & 1 )
        {
            _cellOnMouse = &cell;
            _guiActFlags |= 0x10;
            _cellMouseIsectPos.x = (float)v23 * robo_map.field_1E0 + 0.5;
            _cellMouseIsectPos.y = cell.height;
            _cellMouseIsectPos.z = -((float)v24 * robo_map.field_1E4 + 0.75);
        }

        if ( _guiActFlags & 2 && !(bzda.field_1D0 & 0x30) && cell.IsCanSee(_userRobo->_owner) )
        {

            int v16 = 0;

            while ( v16 < 2 )
            {
                for( NC_STACK_ypabact* &v17 : cell.unitsList )
                {
                    if ( (v16 != 1 || v17->_owner != _userRobo->_owner) && (v16 || v17->_owner == _userRobo->_owner) )
                    {
                        if ( v17->_status != BACT_STATUS_CREATE && v17->_status != BACT_STATUS_DEAD && v17->_status != BACT_STATUS_BEAM && v17->_bact_type != BACT_TYPES_MISSLE )
                        {
                            int v19 = v17->_position.x / robo_map.field_1E0;
                            int v20 = v17->_position.z / robo_map.field_1E4;

                            if ( abs(v19 - v23) <= 8 && abs(-v20 - v24) <= 8 )
                            {
                                _guiActFlags |= 0x20;

                                _bactOnMouse = v17;
                                _guiActFlags &= ~0x10;

                                v16 = 2;
                                break;
                            }
                        }
                    }
                }

                v16++;
            }
        }
    }
}

void NC_STACK_ypaworld::yw_SMAN_MouseSelect(TClickBoxInf *winp)
{
    if ( _guiActFlags & 2 )
    {
        int v3 = winp->selected_btnID - 8;

        NC_STACK_ypabact *v4 = sub_4C7B0C(v3, winp->move.BtnPos.x);

        if ( v4 )
        {
            if ( v4->_status != BACT_STATUS_CREATE && v4->_status != BACT_STATUS_DEAD && v4->_status != BACT_STATUS_BEAM )
            {
                _bactOnMouse = v4;
                _guiActFlags |= 0x20;
            }
        }
        else
        {
            v4 = squadron_manager.squads[v3];

            if ( v4 )
            {
                if ( v4->_status == BACT_STATUS_CREATE || v4->_status == BACT_STATUS_BEAM || v4->_status == BACT_STATUS_DEAD )
                    v4 = NULL;

                if ( v4 )
                {
                    _bactOnMouse = v4;
                    _guiActFlags |= 0x20;
                }
            }
        }
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub21__sub1__sub3__sub0(TClickBoxInf *winp)
{
    float v3 = (float)(winp->move.ScreenPos.x - (_screenSize.x / 2)) / (float)(_screenSize.x / 2);
    float v4 = (float)(winp->move.ScreenPos.y - (_screenSize.y / 2)) / (float)(_screenSize.y / 2);

    mat3x3 corrected = _viewerRotation;
    GFX::Engine.matrixAspectCorrection(corrected, true);

    float v16 = corrected.m00 * v3 + corrected.m10 * v4 + corrected.m20;
    float v17 = corrected.m01 * v3 + corrected.m11 * v4 + corrected.m21;
    float v18 = corrected.m02 * v3 + corrected.m12 * v4 + corrected.m22;

    float v14 = sqrt(POW2(v16) + POW2(v17) + POW2(v18));

    _mouseVector.x = v16 / v14;
    _mouseVector.y = v17 / v14;
    _mouseVector.z = v18 / v14;
}

void NC_STACK_ypaworld::yw_3D_MouseSelect(TClickBoxInf *winp)
{
    ypaworld_func64__sub21__sub1__sub3__sub0(winp);

    yw_arg150 arg150;
    arg150.field_24 = NULL;
    arg150.pos = _viewerPosition;
    arg150.field_18 = _mouseVector * _normalVizLimit;
    arg150.unit = _viewerBact;

    ypaworld_arg136 arg149;
    arg149.stPos = arg150.pos;
    arg149.vect = arg150.field_18;
    arg149.flags = 0;

    ypaworld_func149(&arg149);

    ypaworld_func150(&arg150);

    float v27, v28;

    if ( arg149.isect )
    {
        _guiActFlags |= 0x10;

        _mouseSelDistance = (arg149.isectPos - _viewerPosition).length();

        v27 = _mouseSelDistance;
    }

    if ( arg150.field_24 )
    {
        _guiActFlags |= 0x20;

        _mouseSelDistance = arg150.field_28;

        v28 = _mouseSelDistance;
    }

    if ( (_guiActFlags & 0x30) == 0x30 )
    {
        if ( v27 >= v28 )
        {
            _mouseSelDistance = v28;
            _guiActFlags &= ~0x10;
        }
        else
        {
            _mouseSelDistance = v27;
            _guiActFlags &= ~0x20;
        }
    }

    if ( _guiActFlags & 0x10 )
    {
        Common::Point tmp = World::PositionToSectorID(arg149.isectPos);

        _cellOnMouse = &_cells(tmp);
        _cellMouseIsectPos = arg149.isectPos;
    }

    if ( _guiActFlags & 0x20 )
    {
        int v20 = 0;

        if ( arg150.field_24->_status == BACT_STATUS_CREATE || arg150.field_24->_status == BACT_STATUS_DEAD || arg150.field_24->_status == BACT_STATUS_BEAM || arg150.field_24->_bact_type == BACT_TYPES_MISSLE )
            v20 = 1;

        if ( v20 )
            _guiActFlags &= ~0x20;
        else
            _bactOnMouse = arg150.field_24;
    }
}


void NC_STACK_ypaworld::yw_MouseSelect(TInputState *arg)
{
    ypaworld_func64__sub21__sub1__sub0(arg);

    if ( _guiActFlags & 4 )
    {
        if ( _guiActFlags & 8 )
        {
            yw_MAP_MouseSelect(&arg->ClickInf);
        }
        else if ( _guiActFlags & 0x40 )
        {
            yw_SMAN_MouseSelect(&arg->ClickInf);
        }
    }
    else
    {
        yw_3D_MouseSelect(&arg->ClickInf);
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub21__sub7()
{
    _updateMessage.energy = 0;

    if ( bzda.field_1D0 == 2 || bzda.field_1D0 == 4 )
    {
        if ( bzda.field_8EC != -1 )
        {
            _updateMessage.energy = dround(sub_4498F4() * (2 * _vhclProtos[bzda.field_2DC[bzda.field_8EC]].energy));
        }
    }
    else if ( bzda.field_1D0 == 16 && bzda.field_8F4 != -1 )
    {
        _updateMessage.energy = dround(sub_4498F4() * _buildProtos[bzda.field_3DC[bzda.field_8F4]].Energy);
    }
}

int sub_4D3C80(NC_STACK_ypaworld *yw)
{
    if ( !yw->_bactOnMouse )
        return 0;

    if ( yw->_bactOnMouse == yw->_userRobo )
        return 0;

    NC_STACK_ypabact *a4 = yw->_bactOnMouse;

    if ( !yw->_bactOnMouse || !yw->_bactOnMouse->_parent )
        return 0;

    if ( yw->_bactOnMouse->_parent != yw->_bactOnMouse->_host_station )
        a4 = yw->_bactOnMouse->_parent;

    if ( !a4 )
        return 0;
    
    yw->_bactOnMouse = a4;
    return 1;
}

void NC_STACK_ypaworld::ypaworld_func64__sub21__sub5(int arg)
{
    switch ( arg )
    {
    case World::DOACTION_8:
        if ( _bactOnMouse->_bact_type != BACT_TYPES_GUN && _bactOnMouse != _userRobo )
        {
            if ( sub_4D3C80(this) )
            {
                _activeCmdrID = _bactOnMouse->_commandID;
                sub_4C40AC();
            }
        }

        if ( _userRobo != _bactOnMouse )
            bzda.field_1D0 = bzda.field_1CC & 1;
        else
            bzda.field_1D0 = bzda.field_1CC & 0x20;
        break;

    case World::DOACTION_2:
        if (_activeCmdrRemapIndex >= 0)
        {
            _updateMessage.user_action = World::DOACTION_2;
            _updateMessage.selectBact = _cmdrsRemap[_activeCmdrRemapIndex];

            if ( _guiActFlags & 0x10 )
            {
                _updateMessage.target_Sect = _cellOnMouse;
                _updateMessage.target_point = _cellMouseIsectPos;
                _updateMessage.target_Bact = NULL;
            }
            else
            {
                _updateMessage.target_Sect = 0;
                _updateMessage.target_Bact = _bactOnMouse;
            }

            bzda.field_1D0 = bzda.field_1CC & 1;
        }
        break;

    case World::DOACTION_WP_BEGIN:
    case World::DOACTION_WP_ADD:
    case World::DOACTION_WP_CYCLE:
        if (_activeCmdrRemapIndex >= 0)
        {
            _updateMessage.user_action = arg;
            _updateMessage.selectBact = _cmdrsRemap[_activeCmdrRemapIndex];

            if ( _guiActFlags & 0x30 )
            {
                _updateMessage.target_Sect = _cellOnMouse;
                _updateMessage.target_point = _cellMouseIsectPos;
                _updateMessage.target_Bact = NULL;
            }
        }
        break;

    case World::DOACTION_ADD_UNIT1:
        _updateMessage.user_action = World::DOACTION_ADD_UNIT1;
        _updateMessage.protoID = bzda.field_2DC[bzda.field_8EC];
        _updateMessage.target_point = _mouseVector * _mouseSelDistance + _viewerPosition;

        bzda.field_1D0 = 4;

        _activeCmdrRemapIndex = -1;
        _activeCmdrID = 0;
        break;

    case World::DOACTION_ADD_UNIT2:
        if (_activeCmdrRemapIndex >= 0)
        {
            _updateMessage.user_action = World::DOACTION_ADD_UNIT2;
            _updateMessage.selectBact = _cmdrsRemap[_activeCmdrRemapIndex];

            _updateMessage.protoID = bzda.field_2DC[bzda.field_8EC];
            _updateMessage.target_point = _mouseVector * _mouseSelDistance + _viewerPosition;
        }
        break;

    case World::DOACTION_5:
        sb_0x4c66f8(this, _bactOnMouse, _viewerBact);

        if ( _bactOnMouse->_bact_type != BACT_TYPES_GUN && _bactOnMouse != _userRobo )
        {
            if ( sub_4D3C80(this) )
            {
                _activeCmdrID = _bactOnMouse->_commandID;
                sub_4C40AC();
            }
        }
        break;

    case World::DOACTION_6:
        _updateMessage.user_action = World::DOACTION_6;
        _updateMessage.protoID = bzda.field_3DC[bzda.field_8F4];
        _updateMessage.target_Sect = _cellOnMouse;
        _updateMessage.target_point = _cellMouseIsectPos;
        _updateMessage.target_Bact = NULL;
        bzda.field_1D0 = bzda.field_1CC & 1;
        break;

    case World::DOACTION_10:
        _updateMessage.user_action = World::DOACTION_10;
        _updateMessage.target_Sect = _cellOnMouse;
        _updateMessage.target_point = _cellMouseIsectPos;
        _updateMessage.target_Bact = NULL;
        bzda.field_1D0 = 1;
        break;

    case World::DOACTION_19:
        _viewerBact->setBACT_viewer(false);
        _viewerBact->setBACT_inputting(false);
        _bactOnMouse->setBACT_viewer(true);
        break;

    default:
        return;
    }
}


void NC_STACK_ypaworld::ypaworld_func64__sub21(TInputState *arg)
{
    if ( _userRobo && (arg->ClickInf.flag & TClickBoxInf::FLAG_OK) && _viewerBact )
    {
        yw_MouseSelect(arg);
        ypaworld_func64__sub21__sub7();

        int tooltip = 0;
        int mousePointer = 0;
        int doAction = World::DOACTION_0;

        if ( _mouseGrabbed )
        {
            mousePointer = -1;
        }
        else if ( _guiDragDefaultMouse || ((_guiActFlags & 4) && !(_guiActFlags & 0x48)) )
        {
            mousePointer = 0;
        }
        else if ( robo_map.field_1E8 & 0x200 )
        {
            mousePointer = 2;
        }
        else
        {

            mousePointer = 1;
            doAction = World::DOACTION_0;

            if ( bzda.field_1D0 == 1 )
            {
                if ( !(_guiActFlags & 0x10) )
                {
                    if ( (_guiActFlags & 0x20) )
                    {
                        if ( _userRobo->_owner == _bactOnMouse->_owner )
                        {
                            doAction = World::DOACTION_8;
                            mousePointer = 2;
                            tooltip = Locale::TIP_DO_SELECT;
                        }
                        else if ( _activeCmdrRemapIndex != -1 )
                        {
                            doAction = World::DOACTION_2;
                            mousePointer = 3;
                            tooltip = Locale::TIP_DO_ATKVHCL;
                        }
                    }
                }
                else if ( _activeCmdrRemapIndex != -1 )
                {
                    if ( _userRobo->_owner == _cellOnMouse->owner && _cellOnMouse->IsCanSee(_userRobo->_owner) )
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 4;
                        tooltip = Locale::TIP_DO_MOVE;
                    }
                    else
                    {
                        doAction = World::DOACTION_2;
                        tooltip = Locale::TIP_DO_ATKSECTOR;
                        mousePointer = 3;
                    }
                }
            }
            else if ( bzda.field_1D0 == 2 )
            {
                if ( !(_guiActFlags & 8) )
                {
                    if ( (_guiActFlags & 0x20) && _userRobo->_owner == _bactOnMouse->_owner )
                    {
                        doAction = World::DOACTION_8;
                        mousePointer = 2;
                        tooltip = Locale::TIP_DO_SELECT;
                    }
                    else
                    {
                        switch ( sb_0x4d3d44(&arg->ClickInf) )
                        {
                        case 0:
                            doAction = World::DOACTION_ADD_UNIT1;
                            mousePointer = 6;
                            tooltip = Locale::TIP_DO_NEWSQUAD;
                            break;

                        case 1:
                            tooltip = Locale::TIP_ERR_NOROOM;
                            break;

                        case 2:
                            tooltip = Locale::TIP_ERR_NOENERGY;
                            break;

                        case 3:
                            tooltip = Locale::TIP_UNITLIMIT;
                            break;

                        default:
                            break;
                        }
                    }
                }
                else if ( _activeCmdrRemapIndex != -1 )
                {
                    if ( !(_guiActFlags & 0x10) )
                    {
                        if ( _guiActFlags & 0x20 )
                        {
                            if ( _userRobo->_owner == _bactOnMouse->_owner )
                            {
                                doAction = World::DOACTION_8;
                                mousePointer = 2;
                                tooltip = Locale::TIP_DO_SELECT;
                            }
                            else
                            {
                                doAction = World::DOACTION_2;
                                mousePointer = 3;
                                tooltip = Locale::TIP_DO_ATKVHCL;
                            }
                        }
                    }
                    else if ( _userRobo->_owner == _cellOnMouse->owner && _cellOnMouse->IsCanSee(_userRobo->_owner) )
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 4;
                        tooltip = Locale::TIP_DO_MOVE;
                    }
                    else
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 3;
                        tooltip = Locale::TIP_DO_ATKSECTOR;
                    }
                }
            }
            else if ( bzda.field_1D0 == 4 )
            {
                if ( !(_guiActFlags & 8) )
                {
                    if ( (_guiActFlags & 0x20) && _userRobo->_owner == _bactOnMouse->_owner )
                    {
                        doAction = World::DOACTION_8;
                        mousePointer = 2;
                        tooltip = Locale::TIP_DO_SELECT;
                    }
                    else if ( _activeCmdrRemapIndex != -1 )
                    {
                        switch ( sb_0x4d3d44(&arg->ClickInf) )
                        {
                        case 0:
                            doAction = World::DOACTION_ADD_UNIT2;
                            mousePointer = 7;
                            tooltip = Locale::TIP_DO_ADDSQUAD;
                            break;

                        case 1:
                            tooltip = Locale::TIP_ERR_NOROOM;
                            break;

                        case 2:
                            tooltip = Locale::TIP_ERR_NOENERGY;
                            break;

                        case 3:
                            tooltip = Locale::TIP_UNITLIMIT;
                            break;

                        default:
                            break;
                        }
                    }
                }
                else if ( _activeCmdrRemapIndex != -1 )
                {

                    if ( !(_guiActFlags & 0x10) )
                    {
                        if ( _guiActFlags & 0x20 )
                        {
                            if ( _userRobo->_owner == _bactOnMouse->_owner )
                            {
                                doAction = World::DOACTION_8;
                                mousePointer = 2;
                                tooltip = Locale::TIP_DO_SELECT;
                            }
                            else
                            {
                                doAction = World::DOACTION_2;
                                mousePointer = 3;
                                tooltip = Locale::TIP_DO_ATKVHCL;
                            }
                        }
                    }
                    else if ( _userRobo->_owner == _cellOnMouse->owner && _cellOnMouse->IsCanSee(_userRobo->_owner) )
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 4;
                        tooltip = Locale::TIP_DO_MOVE;
                    }
                    else
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 3;
                        tooltip = Locale::TIP_DO_ATKSECTOR;
                    }
                }
            }
            else if (bzda.field_1D0 == 8 )
            {
                if ( _guiActFlags & 0x20 )
                {
                    if ( arg->KbdLastDown == Input::KC_F7 && (arg->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD) )
                    {
                        doAction = World::DOACTION_19;
                        mousePointer = 8;
                    }
                    else if ( _userRobo->_owner == _bactOnMouse->_owner )
                    {
                        doAction = World::DOACTION_5;
                        mousePointer = 8;
                        tooltip = Locale::TIP_DO_CONTROL;
                    }
                }
            }
            else if ( bzda.field_1D0 == 16 )
            {
                if ( _guiActFlags & 0x20 )
                {
                    if ( _userRobo->_owner == _bactOnMouse->_owner )
                    {
                        doAction = World::DOACTION_8;
                        mousePointer = 2;
                        tooltip = Locale::TIP_DO_SELECT;
                    }
                    else
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 3;
                        tooltip = Locale::TIP_DO_ATKVHCL;
                    }
                }
                else
                {
                    switch ( UserActBuildCheck() )
                    {
                    case 0:
                        doAction = World::DOACTION_6;
                        mousePointer = 10;
                        tooltip = Locale::TIP_DO_BUILD;
                        break;

                    case 6:
                        tooltip = Locale::TIP_ERR_NOENERGY;
                        break;

                    case 2:
                        tooltip = Locale::TIP_ERR_NOTOWNED;
                        break;

                    case 3:
                        tooltip = Locale::TIP_ERR_BUILD_TOOCLOSE;
                        break;

                    case 4:
                        tooltip = Locale::TIP_ERR_BUILD_TOOFAR;
                        break;

                    case 5:
                        tooltip = Locale::TIP_ERR_BUILD_INPROC;
                        break;

                    default:
                        break;
                    }
                }
            }
            else if ( bzda.field_1D0 == 32 )
            {
                if ( _guiActFlags & 0x20 )
                {
                    if ( _userRobo->_owner == _bactOnMouse->_owner )
                    {
                        doAction = World::DOACTION_8;
                        mousePointer = 2;
                        tooltip = Locale::TIP_DO_SELECT;
                    }
                    else
                    {
                        doAction = World::DOACTION_2;
                        mousePointer = 3;
                        tooltip = Locale::TIP_DO_ATKVHCL;
                    }
                }
                else
                {
                    switch ( ypaworld_func64__sub21__sub3() )
                    {
                    case 0:
                        doAction = World::DOACTION_10;
                        mousePointer = 9;
                        tooltip = Locale::TIP_DO_TELEPORT;
                        break;

                    case 5:
                        tooltip = Locale::TIP_ERR_UNITSINSECTOR;
                        break;

                    case 1:
                        tooltip = Locale::TIP_ERR_NOENERGY;
                        break;

                    case 2:
                        tooltip = Locale::TIP_ERR_SECTORJAGGY;
                        break;

                    case 3:
                        tooltip = Locale::TIP_ERR_LOCUNKNOWN;
                        break;

                    default:
                        break;
                    }
                }
            }

            _doAction = ypaworld_func64__sub21__sub4(arg, doAction);
            int v18 = _doAction;

            if ( ypaworld_func64__sub21__sub6(&arg->ClickInf) )
            {
                v18 = World::DOACTION_5;
                mousePointer = 8;
                arg->ClickInf.flag |= TClickBoxInf::FLAG_LM_DOWN;
            }

            if ( arg->ClickInf.flag & TClickBoxInf::FLAG_LM_DOWN )
                ypaworld_func64__sub21__sub5(v18);

            if ( (arg->ClickInf.flag & TClickBoxInf::FLAG_LM_HOLD) && v18 != World::DOACTION_0 )
                mousePointer = 0;
        }

        if ( _mouseCursorHidden )
            mousePointer = -1;

        GFX::displ_arg263 v19;

        if ( mousePointer == -1 )
            v19.bitm = NULL;
        else if (_mousePointers[mousePointer])
            v19.bitm = _mousePointers[mousePointer]->GetBitmap();

        switch ( mousePointer )
        {
        case 0:
            v19.pointer_id = 1;
            break;

        case 1:
            v19.pointer_id = 2;
            break;

        case 2:
            v19.pointer_id = 3;
            break;

        case 3:
            v19.pointer_id = 4;
            break;

        case 4:
            v19.pointer_id = 5;
            break;

        case 5:
            v19.pointer_id = 6;
            break;

        case 6:
            v19.pointer_id = 7;
            break;

        case 7:
            v19.pointer_id = 8;
            break;

        case 8:
            v19.pointer_id = 9;
            break;

        case 9:
            v19.pointer_id = 10;
            break;

        case 10:
            v19.pointer_id = 11;
            break;

        default:
            v19.pointer_id = 0;
            break;
        }

        if ( squadron_manager.field_2A8 & 1 )
            v19.pointer_id = 1;

        GFX::Engine.SetCursor(v19.pointer_id, 0);

        if ( tooltip )
            SetShowingTooltip(tooltip);
    }
}



void NC_STACK_ypaworld::ypaworld_func64__sub1(TInputState *inpt)
{
    //Precompute input
    TClickBoxInf *winp = &inpt->ClickInf;

    inpt->Buttons.UnSet(31);

    int v38 = 0;

    if ( _timeStamp - _vehicleTakenControlTimestamp < 500 )
        inpt->Buttons.UnSet(0);

    if ( _timeStamp - _vehicleTakenControlTimestamp > 5000 )
        _invulnerable = 0;

    if ( _shellConfIsParsed )
    {
        if ( _preferences & World::PREF_JOYDISABLE )
            v38 = 1;
    }

    _fireBtnDownHappen = false;

    if ( inpt->Buttons.Is(0) )
    {
        if ( !_fireBtnIsDown )
            _fireBtnDownHappen = true;

        _fireBtnIsDown = true;
    }
    else
    {
        _fireBtnIsDown = false;
    }

    if ( _userUnit->_status == BACT_STATUS_DEAD )
    {
        _mouseGrabbed = false;
    }
    else if ( winp->flag & TClickBoxInf::FLAG_RM_DOWN )
    {
        if ( _mouseGrabbed
                || winp->selected_btn == &robo_map
                || winp->selected_btn == &bzda
                || winp->selected_btn == &squadron_manager
                || (!(bzda.field_1D0 & 1) && _activeCmdrRemapIndex != -1) )
        {
            if ( _mouseGrabbed )
            {
                _mouseGrabbed = false;
            }
            else
            {
                if ( winp->selected_btn != &robo_map && winp->selected_btn != &bzda && !(bzda.field_1D0 & 1) )
                    bzda.field_1D0 = bzda.field_1CC & 1;
            }
        }
        else
        {
            _mouseGrabbed = true;

            Input::Engine.ResetSlider(10);
            Input::Engine.ResetSlider(11);
        }
    }

    if ( gui_lstvw.IsOpen() )
        inpt->Sliders[1] = 0;

    if ( _mouseGrabbed ) // If grabbed mouse
    {
        // Piu-piu mazafaka
        winp->selected_btnID = -1;
        winp->flag &= ~(TClickBoxInf::FLAG_BTN_DOWN | TClickBoxInf::FLAG_BTN_HOLD | TClickBoxInf::FLAG_BTN_UP);

        inpt->Sliders[0] += inpt->Sliders[10];
        inpt->Sliders[1] += inpt->Sliders[11];
        inpt->Sliders[3] += inpt->Sliders[10];
        inpt->Sliders[5] -= inpt->Sliders[11] * 1.5;

        if ( winp->flag & TClickBoxInf::FLAG_LM_HOLD )
            inpt->Buttons.Set(0);

        if ( winp->flag & TClickBoxInf::FLAG_MM_HOLD )
            inpt->Buttons.Set(3);

        SetShowingTooltip(Locale::TIP_MOUSECAPTURED);
    }

    if ( !v38 )
    {
        int v13 = _userUnit->_bact_type == BACT_TYPES_TANK || _userUnit->_bact_type == BACT_TYPES_CAR;
        int v34 = _userUnit->_bact_type == BACT_TYPES_ROBO;

        if ( !_framesElapsed )
        {
            _oldJoyX = inpt->Sliders[12];
            _oldJoyY = inpt->Sliders[13];
            _oldJoyZ = inpt->Sliders[14];
            _oldJoyHatX = inpt->Sliders[15];
            _oldJoyHatY = inpt->Sliders[16];
        }

        int v45 = fabs(_oldJoyX - inpt->Sliders[12]) >= 0.05;
        int v44 = fabs(_oldJoyY - inpt->Sliders[13]) >= 0.05;
        int v46 = fabs(_oldJoyZ - inpt->Sliders[14]) >= 0.05;
        int v43 = fabs(_oldJoyHatX - inpt->Sliders[15]) >= 0.05;
        int v47 = fabs(_oldJoyHatY - inpt->Sliders[16]) >= 0.05;

        if ( v45 || v44 || v46 || v43 || v47)
        {
            _oldJoyX = inpt->Sliders[12];
            _oldJoyY = inpt->Sliders[13];
            _oldJoyZ = inpt->Sliders[14];
            _oldJoyHatX = inpt->Sliders[15];
            _oldJoyHatY = inpt->Sliders[16];
        }

        if ( _joyIgnoreX )
        {
            if ( v45 )
                _joyIgnoreX = 0;
        }
        else
        {
            if ( ( v13 && inpt->Sliders[3] != 0.0 ) || inpt->Sliders[0] != 0.0 )
                _joyIgnoreX = 1;
        }

        if ( _joyIgnoreY )
        {
            if ( v44 || v43 || v47 )
                _joyIgnoreY = 0;
        }
        else
        {
            if ( v13 )
            {
                if ( (_preferences & World::PREF_ALTJOYSTICK ) && inpt->Sliders[5] != 0.0 )
                    _joyIgnoreY = 1;
                else if ( inpt->Sliders[4] != 0.0 )
                    _joyIgnoreY = 1;
            }
            else if ( inpt->Sliders[1] != 0.0 )
                _joyIgnoreY = 1;
        }

        if ( _joyIgnoreZ )
        {
            if ( v46 || v43 || v47 )
                _joyIgnoreZ = 0;
        }
        else
        {
            if ( v13 )
            {
                if ( (_preferences & World::PREF_ALTJOYSTICK) && inpt->Sliders[4] != 0.0 )
                    _joyIgnoreZ = 1;
            }
            else if ( !v34 && inpt->Sliders[2] != 0.0 )
                _joyIgnoreZ = 1;
        }

        if ( inpt->ClickInf.flag & TClickBoxInf::FLAG_BTN_DOWN )
        {
            if ( !(v43 || v44 || v45 || v46 || v47) )
            {
                _joyIgnoreY = 1;
                _joyIgnoreX = 1;
            }
        }

        if ( _joyIgnoreX )
            inpt->Sliders[12] = 0;

        if ( _joyIgnoreY )
            inpt->Sliders[13] = 0;

        if ( _joyIgnoreZ )
            inpt->Sliders[14] = 0;

        if ( !_joyIgnoreX )
        {
            if ( v13 )
                inpt->Sliders[3] += inpt->Sliders[12];
            else
                inpt->Sliders[0] += inpt->Sliders[12];
        }

        if ( !_joyIgnoreY )
        {
            if ( v13 )
            {
                if ( _preferences & World::PREF_ALTJOYSTICK )
                {
                    if ( inpt->Sliders[16] != 0.0 )
                        inpt->Sliders[5] += inpt->Sliders[16];
                    else
                        inpt->Sliders[5] += inpt->Sliders[13] * 2.0;
                }
                else
                {

                    if ( fabs(inpt->Sliders[13]) < 0.3 )
                        inpt->Buttons.Set(3);
                    else
                        inpt->Sliders[4] += -inpt->Sliders[13];

                    inpt->Buttons.Set(31);
                }
            }
            else
            {
                inpt->Sliders[1] += inpt->Sliders[13];
            }
        }

        if ( !_joyIgnoreZ )
        {
            if ( v13 )
            {
                if ( _preferences & World::PREF_ALTJOYSTICK )
                {
                    if ( inpt->Sliders[4] != 0.0 )
                    {
                    }
                    else if ( fabs(inpt->Sliders[14]) < 0.3 )
                    {
                        inpt->Buttons.Set(3);
                    }
                    else
                        inpt->Sliders[4] += inpt->Sliders[14];

                    inpt->Buttons.Set(31);
                }
                else
                {
                    if ( inpt->Sliders[5] == 0.0 )
                        inpt->Sliders[5] += inpt->Sliders[16];
                }
            }
            else
            {
                if ( inpt->Sliders[2] == 0.0 )
                    inpt->Sliders[2] += inpt->Sliders[14];
            }
        }
        if ( inpt->Buttons.Is(16) )
            inpt->Buttons.Set(0);

        if ( inpt->Buttons.Is(17) )
            inpt->Buttons.Set(2);

        if ( inpt->Buttons.Is(18) )
            inpt->Buttons.Set(1);

        if ( inpt->Buttons.Is(19) )
            inpt->Buttons.Set(3);
    }
}


void NC_STACK_ypaworld::GUI_Close()
{
    if ( _guiLoaded )
    {
        ypaworld_func140(&lstvw2);
        ypaworld_func140(&exit_menu);
        ypaworld_func140(&info_log);
        ypaworld_func140(&squadron_manager);
        ypaworld_func140(&robo_map);
        ypaworld_func140(&netgame_wnd);

        sub_4DA874(this);
        sub_46E16C(this);
        sub_4C706C(this);
        sub_4C39A4(this);
        sub_4E1D24();
        sub_4E2B24(this);
        sub_4C8524(this);

        FFeedback_StopAll();

        _mouseCursorHidden = false;
        _guiLoaded = false;
    }
}

void NC_STACK_ypaworld::GuiWinToFront(GuiBase *win)
{
    if ( !(win->flags & GuiBase::FLAG_CLOSED) )
    {
        win->Detach();
        win->Attach(_guiActive);
        Input::Engine.RemClickBox(win);
        Input::Engine.AddClickBoxFront(win);
    }
}

void NC_STACK_ypaworld::GuiWinOpen(GuiBase *win)
{
    if ( win->flags & GuiBase::FLAG_CLOSED )
    {
        win->flags &= ~GuiBase::FLAG_CLOSED;
        Input::Engine.AddClickBoxFront(win);
        _guiDragDefaultMouse = false;
    }
}

void NC_STACK_ypaworld::GuiWinClose(GuiBase *win)
{
    if ( !(win->flags & GuiBase::FLAG_CLOSED) )
    {
        win->flags |= GuiBase::FLAG_CLOSED;
        Input::Engine.RemClickBox(win);
        _guiDragDefaultMouse = false;
    }
}
