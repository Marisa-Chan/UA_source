#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "includes.h"
#include "yw_internal.h"
#include "yw.h"
#include "yw_net.h"
#include "input.h"
#include "windp.h"

#include "yparobo.h"
#include "font.h"
#include "gui/uacommon.h"

extern uint32_t bact_id;


NC_STACK_bitmap * loadDisk_screen(NC_STACK_ypaworld *yw)
{
    SFXEngine::SFXe.StopMusicTrack();

    const char *v3;

    if ( yw->screen_width <= 360 )
        v3 = "disk320.ilbm";
    else if ( yw->screen_width > 600 )
        v3 = "disk640.ilbm";
    else
        v3 = "disk512.ilbm";

    std::string oldRsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", "data:mc2res");

    NC_STACK_bitmap *disk = Nucleus::CInit<NC_STACK_ilbm>({
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(v3)},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});

    set_prefix_replacement("rsrc", oldRsrc);

    return disk;
}

void draw_splashScreen(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen)
{
    if ( splashScreen )
    {
        rstr_arg204 a4;

        a4.pbitm = splashScreen->GetResBmp();

        a4.float4  = -1.0;
        a4.float8  = -1.0;
        a4.floatC  =  1.0;
        a4.float10 =  1.0;
        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C =  1.0;
        a4.float20 =  1.0;

        NC_STACK_display *win3d = GFXEngine::GFXe.getC3D();

        if ( win3d )
        {
            displ_arg263 v4;
            v4.bitm = yw->pointers__bitm[5];
            v4.pointer_id = 6;

            win3d->display_func263(&v4);

            win3d->BeginFrame();
            win3d->raster_func202(&a4);
            win3d->EndFrame();

            win3d->BeginFrame();
            win3d->raster_func202(&a4);
            win3d->EndFrame();
        }
    }
}

void drawSplashScreenWithTOD(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen, const char *text)
{
    if ( splashScreen )
    {
        rstr_arg204 a4;

        a4.pbitm = splashScreen->GetResBmp();

        a4.float4  = -1.0;
        a4.float8  = -1.0;
        a4.floatC  =  1.0;
        a4.float10 =  1.0;
        a4.float14 = -1.0;
        a4.float18 = -1.0;
        a4.float1C =  1.0;
        a4.float20 =  1.0;

        NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

        if ( win3d )
        {
            displ_arg263 v4;
            v4.bitm = yw->pointers__bitm[5];
            v4.pointer_id = 6;

            win3d->display_func263(&v4);

            win3d->BeginFrame();
            win3d->raster_func202(&a4);
            splashScreen_OutText(yw, win3d, text, yw->screen_width / 7, yw->screen_height / 5);
            win3d->EndFrame();

            win3d->BeginFrame();
            win3d->raster_func202(&a4);
            splashScreen_OutText(yw, win3d, text, yw->screen_width / 7, yw->screen_height / 5);
            win3d->EndFrame();
        }
    }
}

void deleteSplashScreen(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen)
{
    if ( splashScreen )
        delete_class_obj(splashScreen);
}


int sb_0x44ca90__sub6(NC_STACK_ypaworld *yw)
{
    yw->map_events = (map_event *)AllocVec(sizeof(map_event), 65537);

    return yw->map_events != NULL;
}

int NC_STACK_ypaworld::sb_0x44ca90__sub7(int event_loop_id)
{
    //Event funcs

    map_event *mevnts = map_events;
    mevnts->field_8 = -1;
    mevnts->field_C = 0;
    mevnts->field_4 = 0;
    mevnts->event_count = 0;
    mevnts->event_loop_id = event_loop_id;

    if (event_loop_id == 1)
    {
        evnt *ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 5000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 0;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D51A4;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 47;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D5218;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 48;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D5160;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 49;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 6;
        ev->field_10 = 15000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 60;
        ev->field_18++;
    }
    else if ( event_loop_id == 2 )
    {
        evnt *ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 5000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 0;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D51A4;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 47;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D528C;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 50;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 2;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D5160;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 64;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 51;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 52;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 20000;
        ev->field_4 = sub_4D5160;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 53;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 6;
        ev->field_10 = 15000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 61;
        ev->field_18++;
    }
    else if ( event_loop_id == 3 )
    {
        evnt *ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 3000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 0;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 8000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 59;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 60000;
        ev->field_4 = sub_4D5300;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 54;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 94;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 3000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 95;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 60000;
        ev->field_4 = sub_4D5348;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 97;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 96;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 3000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 98;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 9000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 99;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 5000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 0;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 60000;
        ev->field_4 = sub_4D5160;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 101;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 100;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 4000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 102;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 3;
        ev->field_10 = 60000;
        ev->field_4 = sub_4D5360;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 55;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 3000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 103;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 5;
        ev->field_10 = 45000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 0;
        ev->field_18++;


        ev = &mevnts->evnts[ mevnts->event_count ];
        mevnts->event_count++;

        memset(ev, 0, sizeof(evnt));

        ev->field_0 = 4;
        ev->field_10 = 60000;
        ev->field_4 = NULL;
        ev->field_14 = 0;
        ev->field_1C[ ev->field_18 ] = 104;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 56;
        ev->field_18++;
        ev->field_1C[ ev->field_18 ] = 65;
        ev->field_18++;
    }

    return 1;
}

void NC_STACK_ypaworld::PowerStationErase(size_t id)
{
    yw_field34 *station = &_powerStations[id];

    cellArea *cell = station->p_cell;

    if ( cell )
    {
        station->p_cell = NULL;

        if ( id == _powerStationsCount - 1 )
            _powerStationsCount--;

        cell->w_id = 0;
        cell->w_type = 0;

        if ( blg_map )
            (*blg_map)(station->x, station->y) = 0;
    }
}

int sb_0x44ca90__sub3(NC_STACK_ypaworld *yw)
{
    yw->field_30 = (yw_f30 *)AllocVec(sizeof(yw_f30) * 64 * 64, 65537);

    if ( yw->field_30 )
    {
        yw->_powerStations = (yw_field34 *)AllocVec(sizeof(yw_field34) * 256, 65537);

        if ( yw->_powerStations )
        {
            for (int i = 0; i < 256; i++)
                yw->PowerStationErase(i);

            yw->_lastUpdatedPowerStationID = 0;
            yw->_powerStationsCount = 0;
        }
        else
        {
            if ( yw->field_30 )
            {
                nc_FreeMem(yw->field_30);
                yw->field_30 = NULL;
            }
            return 0;
        }
    }
    else
    {
        if ( yw->_powerStations )
        {
            nc_FreeMem(yw->_powerStations);
            yw->_powerStations = NULL;
            yw->_powerStationsCount = 0;
        }

        if ( yw->field_30 )
        {
            nc_FreeMem(yw->field_30);
            yw->field_30 = NULL;
        }
        return 0;
    }
    return 1;
}

void sb_0x44ca90__sub5(NC_STACK_ypaworld *yw)
{
    memset(yw->field_80, 0, sizeof(yw_f80) * 8);
}

void sb_0x44ca90__sub2(NC_STACK_ypaworld *yw, LevelDesc *mapp)
{
    NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

    for (size_t i = 0; i < mapp->Palettes.size(); i++)
    {
        if (!mapp->Palettes[i].empty())
        {
            NC_STACK_bitmap *ilbm = Nucleus::CInit<NC_STACK_ilbm>({
                {NC_STACK_rsrc::RSRC_ATT_NAME, mapp->Palettes[i]},
                {NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, (int32_t)1}});

            if (ilbm)
            {
                if ( i )
                    win3d->display_func261(i, *ilbm->getBMD_palette());
                else
                    win3d->SetPalette(*ilbm->getBMD_palette());

                delete_class_obj(ilbm);
            }
            else
            {
                ypa_log_out("WARNING: slot #%d [%s] init failed!\n", i, mapp->Palettes[i].c_str());
            }
        }
    }
}

int sb_0x44ca90__sub8(NC_STACK_ypaworld *yw)
{
    yw->samples = (yw_samples *)AllocVec(sizeof(yw_samples), 65537);
    if ( !yw->samples )
        return 0;

    yw->samples->field_0 = -1;
    SFXEngine::SFXe.sub_423DB0(&yw->samples->field_4);

    return 1;
}

int NC_STACK_ypaworld::LevelCommonLoader(LevelDesc *mapp, int levelID, int a5)
{
    int ok = 0;

    mapp->clear();

    for (auto &p : ingamePlayerStatus)
        p.clear();

    timeStamp = 0;
    field_1a04 = 0;
    field_1a08 = 0;
    field_1a00 = 0;
    field_1a1c = 0;
    field_161c = 0;

    _levelInfo->LevelID = levelID;
    _levelInfo->Mode = a5;
    _levelInfo->State = 0;
    _levelInfo->OwnerMask = 0;
    _levelInfo->UserMask = 0;

    field_1a60 = 0;
    field_1a98 = NULL;
    field_1aac = 0;
    current_bact = NULL;
    UserRobo = NULL;
    UserUnit = NULL;
    _UserRoboKidsList = NULL;
    field_1ab0 = 0;
    field_160c = 0;
    field_1610 = 0;
    field_7882 = 1;
    field_7886 = 1;
    field_788A = 0;
    field_81AF = 0;
    field_241c = 0;
    field_1628 = 0;
    field_162A = 0;
    field_1624 = 0;
    vehicle_sector_ratio_1 = 0;
    field_8283 = 0;
    field_1a20 = 0;

    _levelInfo->Gates.clear();

	_levelInfo->SuperItems.clear();

    _Gems.clear();
    
    memset(&field_81CB, 0, sizeof(yw_81cb));
    memset(field_1bac, 0, sizeof(int) * 8);

    dbg_num_sqd_max = 0;
    dbg_num_vhcl_max = 0;
    dbg_num_flk_max = 0;
    dbg_num_wpn_max = 0;
    dbg_num_robo_max = 0;

    if ( !one_game_res )
    {
        if ( game_default_res != shell_default_res )
        {
            GFXEngine::GFXe.setResolution(game_default_res);

            screen_width = GFXEngine::GFXe.getScreenW();
            screen_height = GFXEngine::GFXe.getScreenH();
            _win3d = GFXEngine::GFXe.getC3D();

            _win3d->setWDD_cursor( (field_73CE & 0x40) != 0 );

            if ( screen_width >= 512 )
            {
                _win3d->load_font( get_lang_string(string_pointers_p2, 15, "MS Sans Serif,12,400,0") );
                Gui::UA::LoadFont( get_lang_string(string_pointers_p2, 15, "MS Sans Serif,12,400,0") );
            }
            else
            {
                _win3d->load_font( get_lang_string(string_pointers_p2, 16, "Arial,8,400,0") );
                Gui::UA::LoadFont( get_lang_string(string_pointers_p2, 16, "Arial,8,400,0") );
            }
        }
    }

    NC_STACK_bitmap *diskScreenImage = loadDisk_screen(this);

    if ( diskScreenImage )
        draw_splashScreen(this, diskScreenImage);


    std::string oldRsrc = get_prefix_replacement("rsrc");

    set_prefix_replacement("rsrc", "data:fonts");

    int v19 = load_fonts_and_icons();

    set_prefix_replacement("rsrc", oldRsrc);

    if ( !v19 )
        return 0;

    int tod = loadTOD(this, "tod.def");

    const char *text = get_lang_string(string_pointers_p2, tod + 2490, " ");

    tod++;
    if ( tod + 2490 > 2512 )
        tod = 0;

    writeTOD(this, "tod.def", tod);

    if ( diskScreenImage )
    {
        drawSplashScreenWithTOD(this, diskScreenImage, text);
        deleteSplashScreen(this, diskScreenImage);
    }

    p_1_grp_cnt = 0;
    for (int i = 0; i < 8; i++)
    {
        p_1_grp[0][i] = 0;
        p_1_grp[1][i] = 0;
        p_1_grp[2][i] = 100000;
        p_1_grp[3][i] = 0;
    }

    _history.Clear();
    _historyLastIsTimeStamp = false;

    audio_volume = SFXEngine::SFXe.getMasterVolume();

    sb_0x44ca90__sub8(this);
    sb_0x44ca90__sub6(this);

    set_prefix_replacement("rsrc", "data:");

    if ( sub_4DA41C(mapp, LevelNet->mapInfos[_levelInfo->LevelID].mapPath) && (mapp->Flags & 0x7F) == 0x7F )
    {
        sb_0x44ca90__sub7(mapp->EventLoopID);

        if ( !sb_0x44ca90__sub3(this) )
            return 0;

        sb_0x44ca90__sub5(this);

        set_prefix_replacement("rsrc", fmt::sprintf("data:set%d", mapp->SetID));

        sb_0x44ca90__sub2(this, mapp);

        if ( yw_LoadSet(mapp->SetID) )
        {
            if ( yw_loadSky(this, mapp->SkyStr) )
                ok = 1;
        }
    }

    FFeedback_Init();

    return ok;
}

bool NC_STACK_ypaworld::LoadTypeMap(const std::string &mapName)
{
    if ( !typ_map )
        return false;

    uint8_t *typMap = typ_map->data();

    _mapWidth = typ_map->Width();
    _mapHeight = typ_map->Height();

    map_Width_meters = _mapWidth * 1200.0;
    map_Height_meters = _mapHeight * 1200.0;

    int sectors_count = _mapHeight * _mapWidth;

    for (int i = 0; i < sectors_count; i++)
    {
        cellArea *cell = &_cells[i];
        cell->clear();
        secType *sectp = &secTypes[ typMap[i] ];

        cell->type_id = typMap[i];
        cell->comp_type = sectp->field_0;
        cell->energy_power = 0;

        if ( sectp->field_0 == 1)
        {
            cell->buildings_health[0][0] = sectp->buildings[0][0]->build_health;
        }
        else
        {
            for (int j = 0; j < 3; j++)
                for (int k = 0; k < 3; k++)
                    cell->buildings_health[j][k] = sectp->buildings[j][k]->build_health;
        }
    }
    return true;
}

bool NC_STACK_ypaworld::LoadOwnerMap(const std::string &mapName)
{
    for ( auto &cnt : sectors_count_by_owner )
        cnt = 0;

    if ( !own_map )
        return 0;

    if ( (int)own_map->Width() != _mapWidth || (int)own_map->Height() != _mapHeight )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), own_map->Width(), own_map->Height(), _mapWidth, _mapHeight);
        delete own_map;
        own_map = NULL;
        return 0;
    }


    for (uint32_t yy = 0; yy < own_map->Height(); yy++)
    {
        uint8_t *ownmapp = own_map->Line(yy);
        cellArea *pcells = _cells + yy * _mapWidth;

        for (uint32_t xx = 0; xx < own_map->Width(); xx++)
        {
            if ( xx > 0 && yy > 0 && xx != (uint32_t)_mapWidth - 1 && yy != (uint32_t)_mapHeight - 1 )
                pcells[xx].owner = ownmapp[xx];
            else
                pcells[xx].owner = 0;

            sectors_count_by_owner[ (int)pcells[xx].owner ]++;
        }
    }

    return 1;
}

bool NC_STACK_ypaworld::LoadHightMap(const std::string &mapName)
{
    if ( !hgt_map )
        return false;

    if ( (int)hgt_map->Width() != _mapWidth || (int)hgt_map->Height() != _mapHeight )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), hgt_map->Width(), hgt_map->Height(), _mapWidth, _mapHeight);
        delete hgt_map;
        hgt_map = NULL;
        return false;
    }

    for (uint32_t i = 0; i < hgt_map->size(); i++)
    {
        _cells[i].height = (*hgt_map)[i] * -100.0;
    }

    for (int y = 1; y < _mapHeight; y++)
    {
        for (int x = 1; x < _mapWidth; x++)
        {
            int yy = y - 1;
            int xx = x - 1;

            int cur_sector_id = x + y * _mapWidth;
            int left_sector = xx + y * _mapWidth;
            int left_up_sector = xx + yy * _mapWidth;
            int up_sector = x + yy * _mapWidth;

            _cells[cur_sector_id].averg_height = (_cells[cur_sector_id].height +
                                                    _cells[left_sector].height +
                                                    _cells[left_up_sector].height +
                                                    _cells[up_sector].height ) / 4.0;
        }
    }

    return true;
}

bool NC_STACK_ypaworld::yw_createRobos(const std::vector<MapRobo> &Robos)
{
    if ( _levelInfo->Mode != 1 )
    {
        _levelInfo->OwnerMask = 0;
        _levelInfo->UserMask = 2;
        
        bool first = true;

        for ( const MapRobo &roboInf : Robos)
        {
            ypaworld_arg136 v14;
            v14.stPos = vec3d::X0Z(roboInf.Pos) - vec3d::OY(30000.0);
            v14.vect = vec3d::OY(50000.0);
            v14.flags = 0;

            ypaworld_arg146 v15;
            v15.vehicle_id = roboInf.VhclID;
            v15.pos = roboInf.Pos;

            ypaworld_func136(&v14);

            if ( v14.isect )
                v15.pos.y += v14.isectPos.y;

            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>( ypaworld_func146(&v15) );

            if ( robo )
            {
                int v20 = 0;

                ypaworld_func134(robo);

                int v12;

                if ( !first )
                {
                    v12 = roboInf.Energy;
                }
                else
                {
                    v12 = roboInf.Energy / 4;

                    if ( v12 < _maxRoboEnergy )
                    {
                        v12 = _maxRoboEnergy;
                        v20 = _maxReloadConst;
                    }

                }

                robo->_owner = roboInf.Owner;
                robo->_energy = v12;
                robo->_energy_max = v12;

                if ( !v20 )
                {
                    if ( roboInf.ReloadConst )
                        v20 = roboInf.ReloadConst;
                    else
                        v20 = robo->_energy_max;
                }

                robo->_reload_const = v20;

                robo->setBACT_bactCollisions(1);
                robo->setROBO_fillMode(15);
                robo->setROBO_battVehicle(v12);
                robo->setROBO_battBeam(v12);

                _levelInfo->OwnerMask |= 1 << roboInf.Owner;

                robo->setROBO_epConquer(roboInf.ConBudget);
                robo->setROBO_epDefense(roboInf.DefBudget);
                robo->setROBO_epRadar(roboInf.RadBudget);
                robo->setROBO_epPower(roboInf.PowBudget);
                robo->setROBO_epSafety(roboInf.SafBudget);
                robo->setROBO_epChangeplace(roboInf.CplBudget);
                robo->setROBO_epRobo(roboInf.RobBudget);
                robo->setROBO_epReconnoitre(roboInf.RecBudget);
                robo->setROBO_viewAngle(roboInf.ViewAngle);
                robo->setROBO_safDelay(roboInf.SafDelay);
                robo->setROBO_powDelay(roboInf.PowDelay);
                robo->setROBO_cplDelay(roboInf.CplDelay);
                robo->setROBO_radDelay(roboInf.RadDelay);
                robo->setROBO_defDelay(roboInf.DefDelay);
                robo->setROBO_conDelay(roboInf.ConDelay);
                robo->setROBO_recDelay(roboInf.RecDelay);
                robo->setROBO_robDelay(roboInf.RobDelay);

                if ( first )
                {
                    robo->setBACT_viewer(1);
                    robo->setBACT_inputting(1);
                }
            }
            
            first = false;
        }
    }
    return true;
}

bool NC_STACK_ypaworld::LoadBlgMap(const std::string &mapName)
{
    if ( !blg_map )
        return false;

    if ( (int)blg_map->Width() != _mapWidth || (int)blg_map->Height() != _mapHeight )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), blg_map->Width(), blg_map->Height(), _mapWidth, _mapHeight);
        delete blg_map;
        blg_map = NULL;
        return false;
    }

    for ( uint32_t y = 0; y < blg_map->Height(); y++)
    {
        uint8_t *blgln = blg_map->Line(y);
        cellArea *cellln = _cells + y * _mapWidth;

        for ( uint32_t x = 0; x < blg_map->Width(); x++)
        {
            if (blgln[x] && cellln[x].owner)
            {
                ypaworld_arg148 arg148;

                arg148.ownerID = cellln[x].owner;
                arg148.ownerID2 = cellln[x].owner;
                arg148.blg_ID = blgln[x];
                arg148.field_C = 1;
                arg148.field_18 = 0;
                arg148.x = x;
                arg148.y = y;

                ypaworld_func148(&arg148);
            }
        }
    }

    return true;
}

void NC_STACK_ypaworld::yw_InitSquads(const std::vector<MapSquad> &squads)
{
    if ( _levelInfo->Mode != 1 )
    {
        size_t i = 0;
        for ( const MapSquad &squad : squads )
        {
            NC_STACK_yparobo *robo = NULL;

            for( NC_STACK_ypabact *unit : _unitsList )
            {
                if ( unit->_bact_type == BACT_TYPES_ROBO && unit->_owner == squad.Owner)
                {
                    robo = dynamic_cast<NC_STACK_yparobo *>(unit);
                    break;
                }
            }

            if ( !robo )
            {
                ypa_log_out("WARNING: yw_InitSquads(): no host robo for squad[%d], owner %d!\n", i, squad.Owner);
            }
            else
            {
                vec3d squadPos;

                ypaworld_arg136 arg136;
                arg136.stPos.x = squad.X;
                arg136.stPos.y = -50000.0;
                arg136.stPos.z = squad.Z;
                arg136.vect = vec3d::OY(100000.0);
                arg136.flags = 0;
                ypaworld_func136(&arg136);

                if ( arg136.isect )
                    squadPos = arg136.isectPos - vec3d::OY(50.0);
                else
                {
                    yw_130arg sect_info;
                    sect_info.pos_x = squad.X;
                    sect_info.pos_z = squad.Z;

                    if ( !ypaworld_func130(&sect_info) )
                    {
                        ypa_log_out("yw_InitSquads(): no valid position for squad[%d]!\n", i);
                        return;
                    }

                    squadPos.x = squad.X;
                    squadPos.y = sect_info.pcell->height;
                    squadPos.z = squad.Z;
                }
                // Create squad by robo method
                robo->MakeSquad( std::vector<int>(squad.Count, squad.VhclID), squadPos, squad.Useable); // yparobo_func133
            }
            
            i++;
        }
    }
}

void NC_STACK_ypaworld::InitBuddies()
{
    if ( !_levelInfo->Buddies.empty() )
    {
        int squad_sn = 0;
        
        std::vector<MapBuddy> buds = _levelInfo->Buddies;
        while ( 1 )
        {
            std::vector<int> VhclIDS;
            int wrkID = -1;
            for (std::vector<MapBuddy>::iterator it = buds.begin(); it != buds.end(); )
            {
                if (wrkID == -1 || wrkID == it->CommandID )
                {
                    wrkID = it->CommandID;
                    VhclIDS.push_back(it->Type);
                    it = buds.erase(it);
                }
                else
                    it++;
            }

            if ( wrkID == -1 )
                break;

            vec3d squadPos =    UserRobo->_position +  
                                vec3d(  sin(squad_sn * 1.745) * 500.0,
                                        0.0,
                                        cos(squad_sn * 1.745) * 500.0 );

            ypaworld_arg136 arg136;
            arg136.stPos = squadPos.X0Z() + vec3d(0.5, -50000.0, 0.75);
            arg136.vect = vec3d::OY(100000.0);
            arg136.flags = 0;

            ypaworld_func136(&arg136);

            if ( arg136.isect )
                squadPos.y = arg136.isectPos.y + -100.0;

            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(UserRobo);

            robo->MakeSquad(VhclIDS, squadPos, true); //robo 133 method

            squad_sn++;
        }
    }
}

void NC_STACK_ypaworld::yw_InitTechUpgradeBuildings()
{
    field_2b7c = 0;
    last_modify_build = 0;
    last_modify_vhcl = 0;
    last_modify_weapon = 0;
    field_2b78 = -1;

    for (size_t i = 0; i < _Gems.size(); i++)
    {
        MapGem &gem = _Gems[i];
        cellArea *cell = &_cells[gem.SecX + gem.SecY * _mapWidth];

        if (gem.BuildingID)
        {
            if ( cell->w_type != 3 || gem.BuildingID != cell->w_id )
            {
                ypaworld_arg148 arg148;
                arg148.ownerID = cell->owner;
                arg148.ownerID2 = cell->owner;
                arg148.blg_ID = gem.BuildingID;
                arg148.x = gem.SecX;
                arg148.y = gem.SecY;
                arg148.field_C = 1;
                arg148.field_18 = 0;

                ypaworld_func148(&arg148);
            }
        }

        cell->w_type = 4;
        cell->w_id = i;
    }
}

void NC_STACK_ypaworld::InitGates()
{
    for (size_t i = 0; i < _levelInfo->Gates.size(); i++)
    {
        MapGate &gate = _levelInfo->Gates[i];

        gate.PCell = &_cells[gate.SecX + _mapWidth * gate.SecY];

        ypaworld_arg148 arg148;
        arg148.ownerID = gate.PCell->owner;
        arg148.ownerID2 = gate.PCell->owner;
        arg148.blg_ID = gate.ClosedBldID;
        arg148.field_C = 1;
        arg148.x = gate.SecX;
        arg148.y = gate.SecY;
        arg148.field_18 = 0;

        ypaworld_func148(&arg148);

        gate.PCell->w_type = 5;
        gate.PCell->w_id = i;

        for (MapKeySector &ks : gate.KeySectors)
        {
            if ( ks.x && ks.x < _mapWidth - 1 && ks.y && ks.y < _mapHeight - 1 )
            {
                ks.PCell = &_cells[ks.x + _mapWidth * ks.y];
            }
        }
    }
}

void NC_STACK_ypaworld::InitSuperItems()
{
    for ( size_t i = 0; i < _levelInfo->SuperItems.size(); i++ )
    {
        MapSuperItem &sitem = _levelInfo->SuperItems[i];
        
        sitem.PCell = &_cells[sitem.SecX + _mapWidth * sitem.SecY];

        ypaworld_arg148 arg148;
        arg148.ownerID = sitem.PCell->owner;
        arg148.ownerID2 = sitem.PCell->owner;
        arg148.blg_ID = sitem.InactiveBldID;
        arg148.field_C = 1;
        arg148.x = sitem.SecX;
        arg148.y = sitem.SecY;
        arg148.field_18 = 0;

        ypaworld_func148(&arg148);

        sitem.PCell->w_type = 8;
        sitem.PCell->w_id = i;

        for ( MapKeySector &ks : sitem.KeySectors )
        {
            if ( ks.x && ks.x < _mapWidth - 1 && ks.y && ks.y < _mapHeight - 1 )
                ks.PCell = &_cells[ks.x + _mapWidth * ks.y];

        }

        sitem.ActiveTime = 0;
        sitem.TriggerTime = 0;
        sitem.ActivateOwner = 0;
        sitem.State = 0;
    }
}

void NC_STACK_ypaworld::UpdatePowerEnergy()
{
    // Apply power to sectors and clean power matrix for next compute iteration.

    for (int y = 0; y < _mapHeight; y++)
    {
        for (int x = 0; x < _mapWidth; x++)
        {
            cellArea *cell = _cells + x + y * _mapWidth;
            yw_f30 *tt = &field_30[ x + y * 64 ];

            tt->owner = cell->owner;
            cell->energy_power = tt->field_1; // Apply power to cell
            tt->field_1 = 0; // Clean matrix's power
        }
    }

    _lastUpdatedPowerStationID = 0; // Next power station for recompute power is first
}


void NC_STACK_ypaworld::CellSetOwner(cellArea *cell, char secX, char secY, uint8_t owner)
{
    if ( cell->owner != owner )
    {
        HistoryEventAdd( World::History::Conq(secX, secY, owner) );

        if ( cell->w_type == 2 )
            HistoryEventAdd( World::History::PowerST(secX, secY, owner) );

        sectors_count_by_owner[cell->owner]--;
        sectors_count_by_owner[owner]++;

        cell->owner = owner;
    }
}

void NC_STACK_ypaworld::CellSetNewOwner(int secX, int secY, cellArea *cell, yw_arg129 *a5, int newOwner)
{
    int energon[World::FRACTION_MAXCOUNT];

    if ( newOwner == 255 )
    {
        newOwner = cell->owner;

        for( int &e : energon )
            e = 0;

        for ( NC_STACK_ypabact* &nod : cell->unitsList )
            energon[nod->_owner] += nod->_energy;

        energon[0] = 0;

        for (int i = 0; i < World::FRACTION_MAXCOUNT; i++)
        {
            if ( energon[i] > energon[newOwner] )
                newOwner = i;
        }
    }

    if ( cell->owner != newOwner )
    {
        if ( cell->w_type == 2 )
        {
            if ( UserRobo->_owner == newOwner )
            {
                if ( a5 && a5->unit )
                {
                    yw_arg159 v21;
                    v21.unit = a5->unit;
                    v21.field_4 = 78;
                    v21.txt = NULL;
                    v21.field_C = 45;

                    ypaworld_func159(&v21);
                }
            }
            else if ( cell->owner == UserRobo->_owner )
            {
                yw_arg159 v24;
                v24.unit = NULL;
                v24.field_4 = 78;
                v24.txt = NULL;
                v24.field_C = 67;

                ypaworld_func159(&v24);
            }
        }
        else
        {
            for ( const MapGate &gate : _levelInfo->Gates )
            {
                for ( const MapKeySector &ks : gate.KeySectors )
                {
                    if ( cell == ks.PCell )
                    {
                        if ( UserRobo->_owner == newOwner )
                        {
                            yw_arg159 v23;
                            v23.unit = NULL;
                            v23.field_4 = 80;
                            v23.txt = NULL;
                            v23.field_C = 82;

                            ypaworld_func159(&v23);
                        }
                        else if ( UserRobo->_owner == cell->owner )
                        {
                            yw_arg159 v22;
                            v22.unit = NULL;
                            v22.field_4 = 80;
                            v22.txt = NULL;
                            v22.field_C = 81;

                            ypaworld_func159(&v22);
                        }
                    }
                }
            }
        }
    }

    CellSetOwner(cell, secX, secY, newOwner);
}

void NC_STACK_ypaworld::CellCheckHealth(cellArea *cell, int secX, int secY, int a5, yw_arg129 *a6)
{
    if ( secX && secY && _mapWidth - 1 != secX && _mapHeight - 1 != secY )
    {
        int helth = 0;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                helth += cell->buildings_health[i][j];
            }
        }

        if ( cell->w_type == 2 )
        {
            if ( helth )
            {
                int v13 = (helth * _powerStations[ cell->w_id ].power) / 256 ;

                if ( v13 < 0 )
                    v13 = 0;
                else if ( v13 > 255 )
                    v13 = 255;

                _powerStations[cell->w_id].power_2 = v13;
            }
            else
            {
                PowerStationErase(cell->w_id);
            }
        }

        if ( cell->comp_type == 1 )
        {
            if ( helth < 224 )
                CellSetNewOwner(secX, secY, cell, a6, a5);
        }
        else if ( helth < 1728 )
        {
            CellSetNewOwner(secX, secY, cell, a6, a5);
        }
    }
    else
    {
        CellSetOwner(cell, secX, secY, 0);
    }
}



void sub_44DBF8(NC_STACK_ypaworld *yw, int _dx, int _dz, int _dxx, int _dzz, struct_44dbf8 &a6, int flags)
{
    int v8 = flags;

    a6.sklt = NULL;
    a6.field_1E = 0;
    a6.field_1C = 0;

    if ( _dxx >= 1  &&  _dxx < 4 * yw->_mapWidth - 1  &&  _dzz >= 1  &&  _dzz < 4 * yw->_mapHeight - 1 )
    {
        a6.sec_x = _dxx / 4;
        a6.sec_y = _dzz / 4;

        a6.p_cell = &yw->_cells[a6.sec_x + yw->_mapWidth * (_dzz / 4)];

        if ( _dxx % 4 && _dzz % 4 )
        {
            a6.field_1C = 1;

            int v14, v16;

            if ( a6.p_cell->comp_type == 1 )
            {
                v14 = 0;
                v16 = 0;

                if ( (_dxx / 4) == (_dx / 4)  &&  (_dz / 4) == (_dzz / 4) )
                    v8 = flags & 0xFFFE;

                a6.pos.x =   1200.0 * a6.sec_x + 600.0;
                a6.pos.y = a6.p_cell->height;
                a6.pos.z = -(1200.0 * a6.sec_y + 600.0);
            }
            else
            {
                v16 = (_dxx % 4) - 1;
                v14 = 2 - ((_dzz % 4) - 1);

                if ( _dxx == _dx && _dzz == _dz )
                    v8 = flags & 0xFFFE;

                a6.pos.z = -(_dzz * 300.0);
                a6.pos.x = _dxx * 300.0;
                a6.pos.y = a6.p_cell->height;
            }

            a6.field_1E = v8;

            int model_id = yw->secTypes[a6.p_cell->type_id].buildings[v16][v14]->health_models [   yw->build_hp_ref[    a6.p_cell->buildings_health[v16][v14]    ]    ];

            if ( v8 & 1 )
                a6.sklt = yw->legos[model_id].selected_sklt_intern;
            else
                a6.sklt = yw->legos[model_id].sklt_obj_intern;
        }
        else
        {
            a6.pos.y = 0;
            a6.pos.x = _dxx * 300.0;
            a6.pos.z = -(_dzz * 300.0);

            if ( _dxx == _dx && _dzz == _dz )
                v8 = flags & 0xFE;

            a6.field_1E = v8;

            if ( _dxx % 4 == 0 && _dzz % 4 == 0)
            {
                a6.sklt = yw->ColCross.skeleton_internal;
                a6.field_1C = 4;
            }
            else if ( _dxx % 4 == 0 && _dzz % 4 != 0 )
            {
                a6.sklt = yw->ColSide.skeleton_internal;
                a6.field_1C = 2;
            }
            else if ( _dxx % 4 != 0 && _dzz % 4 == 0 )
            {
                a6.sklt = yw->ColSide.skeleton_internal;
                a6.field_1C = 3;
            }
        }

        if ( a6.field_1C && !a6.sklt )
        {
            ypa_log_out("yw_GetSklt: WARNING, not CZT_INVALID, but Sklt NULL!\n");

            const char *v17 = "UNKNOWN";

            switch ( a6.field_1C )
            {
            case 4:
                v17 = "czt_cross_slurp";
                break;
            case 2:
                v17 = "czt_vside_slurp";
                break;
            case 3:
                v17 = "czt_hside_slurp";
                break;
            case 1:
                v17 = "czt_lego";
                break;
            default:
                break;
            }
            ypa_log_out("    Type=%s, sec_x=%d, sec_y=%d.\n", v17, a6.sec_x, a6.sec_y);

            a6.field_1C = 0;
        }
    }
}

void sub_44DF60(UAskeleton::Data *arg, int id)
{
    UAskeleton::Polygon &tr = arg->polygons[id];

    int vtx1 = arg->polygons[id].v[0];
    int vtx2 = arg->polygons[id].v[1];
    int vtx3 = arg->polygons[id].v[2];

    vec3d tmp  = arg->POO[vtx2] - arg->POO[vtx1];
    vec3d tmp2 = arg->POO[vtx3] - arg->POO[vtx2];

    vec3d tmp3 = tmp * tmp2;
    tmp3.normalise();

    tr.A = tmp3.x;
    tr.B = tmp3.y;
    tr.C = tmp3.z;

    tr.D = -tmp3.dot( arg->POO[vtx1] );
}

void sub_44E07C(NC_STACK_ypaworld *yw, struct_44dbf8 &arg)
{
    if ( arg.field_1C == 2 )
    {
        cellArea *cur = arg.p_cell;
        cellArea *left = arg.p_cell - 1;

        if ( !(arg.field_1E & 1) || fabs( (int)(cur->height - left->height)) < 500.0 )
        {

            arg.sklt->POO[0].y = left->height;
            arg.sklt->POO[1].y = cur->height;
            arg.sklt->POO[2].y = cur->height;
            arg.sklt->POO[3].y = left->height;

            sub_44DF60(arg.sklt, 0);
        }
        else
        {
            arg.sklt = yw->colsub_sklt_intrn;

            if ( cur->height > left->height )
                arg.pos.y = cur->height;
            else
                arg.pos.y = left->height;
        }
    }
    else if ( arg.field_1C == 3 )
    {
        cellArea *cur = arg.p_cell;
        cellArea *up = arg.p_cell - yw->_mapWidth;

        if ( !(arg.field_1E & 1) || fabs( (int)(cur->height - up->height)) < 500.0 )
        {
            arg.sklt->POO[0].y = up->height;
            arg.sklt->POO[1].y = up->height;
            arg.sklt->POO[2].y = cur->height;
            arg.sklt->POO[3].y = cur->height;

            sub_44DF60(arg.sklt, 0);
        }
        else
        {
            arg.sklt = yw->colsub_sklt_intrn;

            if ( cur->height > up->height )
                arg.pos.y = cur->height;
            else
                arg.pos.y = up->height;
        }
    }
    else if ( arg.field_1C == 4 )
    {

        int kk = 0;

        cellArea *cur = arg.p_cell;
        cellArea *left = arg.p_cell - 1;
        cellArea *up = arg.p_cell - yw->_mapWidth;
        cellArea *leftup = arg.p_cell - yw->_mapWidth - 1;

        if ( arg.field_1E & 1 )
        {
            float cs = cur->height;
            float ls = left->height;
            float us = up->height;
            float lus = leftup->height;

            float v15, v16, v17, v18;

            if ( cs >= ls )
                v15 = ls;
            else
                v15 = cs;

            if ( us >= lus )
                v16 = lus;
            else
                v16 = us;

            if ( v15 < v16 )
                v16 = v15;

            if ( cs <= ls )
                v17 = ls;
            else
                v17 = cs;

            if ( us <= lus )
                v18 = lus;
            else
                v18 = us;

            if ( v17 > v18 )
                v18 = v17;

            if ( fabs( (int)(v18 - v16) ) > 300.0 )
            {
                arg.sklt = yw->colsub_sklt_intrn;
                arg.pos.y = v18;
                kk = 1;
            }
        }
        if ( !kk )
        {
            arg.sklt->POO[0].y = leftup->height;
            arg.sklt->POO[1].y = up->height;
            arg.sklt->POO[2].y = cur->height;
            arg.sklt->POO[3].y = left->height;
            arg.sklt->POO[4].y = cur->averg_height;

            sub_44DF60(arg.sklt, 0);
            sub_44DF60(arg.sklt, 1);
            sub_44DF60(arg.sklt, 2);
            sub_44DF60(arg.sklt, 3);
        }
    }
}

int sub_44D36C(const vec3d &v, int id, const UAskeleton::Data *sklt)
{
    int v7 = 0;

    const UAskeleton::Polygon &tr = sklt->polygons[id];
    float nX = fabs(tr.A);
    float nY = fabs(tr.B);
    float nZ = fabs(tr.C);

    float maxAx = (nX <= nY ? nY : nX );
    maxAx = (maxAx <= nZ ? nZ : maxAx);

    if ( maxAx == nX )
    {
        int prev = sklt->polygons[id].num_vertices - 1;

        for (int i = 0; i < sklt->polygons[id].num_vertices; i++)
        {
            UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

            if ( ( (prv.z <= v.z && v.z < cur.z) ||
                    (cur.z <= v.z && v.z < prv.z) ) &&
                    prv.y + (cur.y - prv.y) * (v.z - prv.z) / (cur.z - prv.z) > v.y )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    else if ( maxAx == nY )
    {
        int prev = sklt->polygons[id].num_vertices - 1;

        for (int i = 0; i < sklt->polygons[id].num_vertices; i++)
        {
            UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

            if ( ( (prv.z <= v.z && v.z < cur.z) ||
                    (cur.z <= v.z && v.z < prv.z) ) &&
                    prv.x + (cur.x - prv.x) * (v.z - prv.z) / (cur.z - prv.z) > v.x )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    else if ( maxAx == nZ )
    {
        int prev = sklt->polygons[id].num_vertices - 1;

        for (int i = 0; i < sklt->polygons[id].num_vertices; i++)
        {
            UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

            if ( ( (prv.y <= v.y && v.y < cur.y) ||
                    (cur.y <= v.y && v.y < prv.y) ) &&
                    prv.x + (cur.x - prv.x) * (v.y - prv.y) / (cur.y - prv.y) > v.x )
            {
                v7 = v7 == 0;
            }

            prev = i;
        }
    }
    return v7;
}

void sub_44D8B8(ypaworld_arg136 *arg, const struct_44dbf8 &loc)
{
    for ( int i = 0; i < loc.sklt->polygonsCount; i++)
    {
        UAskeleton::Polygon &tr = loc.sklt->polygons[i];
        vec3d norm = tr.Normal();

        float v11 = norm.dot(arg->vect);
        if ( v11 > 0.0 )
        {
            float v19 = -(norm.dot( arg->stPos ) + tr.D) / v11;
            if ( v19 > 0.0 && v19 <= 1.0 && v19 < arg->tVal )
            {
                vec3d px = arg->vect * v19 + arg->stPos;

                if ( sub_44D36C(px, i, loc.sklt) )
                {
                    arg->isect = 1;
                    arg->tVal = v19;
                    arg->isectPos = loc.pos + px;
                    arg->polyID = i;
                    arg->skel = loc.sklt;
                }
            }
        }
    }
}


void NC_STACK_ypaworld::FFeedback_VehicleChanged()
{
    if ( field_739A )
    {
        if ( field_73CE & (4 | 8) )
            return;
    }

    if ( UserUnit )
    {
        field_7562 = timeStamp;

        if ( input_class )
            input_class->ForceFeedback(NC_STACK_winp::FF_STATE_STOP, NC_STACK_winp::FF_TYPE_ALL);

        int effectType;
        float v13;
        float v14;
        float v15;
        float v16;
        float v17;
        float v18;
        float v19;
        float v22;
        float v24;
        float v25;


        switch ( UserUnit->_bact_type )
        {
        case BACT_TYPES_BACT:
            effectType = NC_STACK_winp::FF_TYPE_HELIENGINE;
            v16 = 300.0;
            v13 = 800.0;
            v15 = 1.0;
            v14 = 2.0;
            v25 = 1.0;
            v18 = 0.0;
            v17 = 0.7;
            v24 = 0.3;
            v22 = 1.0;
            v19 = 0.5;
            break;

        case BACT_TYPES_TANK:
        case BACT_TYPES_CAR:
            effectType = NC_STACK_winp::FF_TYPE_TANKENGINE;
            v16 = 200.0;
            v13 = 500.0;
            v15 = 0.6;
            v14 = 1.0;
            v25 = 1.0;
            v22 = 1.0;
            v17 = 0.1;
            v24 = 0.3;
            v19 = 0.4;
            v18 = 0.0;
            break;

        case BACT_TYPES_FLYER:
            effectType = NC_STACK_winp::FF_TYPE_JETENGINE;
            v16 = 200.0;
            v13 = 500.0;
            v15 = 1.0;
            v14 = 2.0;
            v25 = 1.0;
            v18 = 0.0;
            v17 = 0.1;
            v24 = 0.3;
            v22 = 1.0;
            v19 = 0.75;
            break;

        default:
            effectType = -1;
            break;
        }

        if ( effectType != -1 )
        {
            float v4 = (UserUnit->_mass - v16) / (v13 - v16);
            float v5 = (UserUnit->_maxrot - v15) / (v14 - v15);

            float v21 = (v19 - v24) * v4 + v24;
            float v23 = (v18 - v22) * v4 + v22;

            float v20 = (v17 - v25) * v5 + v25;

            if ( v21 < v24)
                v21 = v24;
            else if (v21 > v19)
                v21 = v19;

            if ( v23 < v18 )
                v23 = v18;
            else if ( v23 > v22)
                v23 = v22;

            if ( v20 < v17)
                v20 = v17;
            else if ( v20 > v25)
                v20 = v25;

            field_7566 = effectType;
            field_756A = v23;
            field_756E = v21;

            if ( input_class )
                input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, effectType, v21, v23);

            if ( input_class )
                input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_ROTDAMPER, v20);
        }
    }
}




NC_STACK_ypabact *NC_STACK_ypaworld::yw_createUnit( int model_id)
{
    std::array<const std::string, 11> unit_classes_names
    {
        "dummy.class",      // 0
        "ypabact.class",    // 1
        "ypatank.class",    // 2
        "yparobo.class",    // 3
        "ypamissile.class", // 4
        "ypazepp.class",    // 5
        "ypaflyer.class",   // 6
        "ypaufo.class",     // 7
        "ypacar.class",     // 8
        "ypagun.class",     // 9
        "ypahovercraft.class" //10
    };

    NC_STACK_ypabact *bacto = NULL;

    // Find dead units
    for ( NC_STACK_ypabact * &unit : _deadCacheList )
    {
        if (unit->_bact_type == model_id)
        {
            bacto = unit;
            break;
        }
    }

    if ( !bacto )
    {
        bacto = Nucleus::CTFInit<NC_STACK_ypabact>(unit_classes_names[model_id], 
            {{NC_STACK_ypabact::BACT_ATT_WORLD, this}} );

        if ( !bacto )
            return NULL;
    }

    bacto->Renew(); // Reset bact

    bacto->_gid = bact_id;
    bacto->_owner = 0;

    bacto->_rotation = mat3x3::Ident();

    bact_id++;

    return bacto;
}


void sub_44BF34(vhclSndFX *sndfx)
{
    if ( !sndfx->wavs[0] && !sndfx->single_sample )
    {
        std::string oldRsrc = get_prefix_replacement("rsrc");

        set_prefix_replacement("rsrc", "data:");

        if ( sndfx->extS.cnt )
        {
            for (int i = 0; i < sndfx->extS.cnt; i++)
            {
                sndfx->wavs[i] = Nucleus::CInit<NC_STACK_wav>( {{NC_STACK_rsrc::RSRC_ATT_NAME, sndfx->extSampleNames[i]}} );

                if ( sndfx->wavs[i] )
                {
                    sampl *sample = sndfx->wavs[i]->getSMPL_pSample();

                    sndfx->extS.sndExts[i].sample = sample;
                    sndfx->extS.sndExts[i].rlOffset = sample->SampleRate * sndfx->extS.sndExts[i].offset / 11000;
                    sndfx->extS.sndExts[i].rlSmplCnt = sample->SampleRate * sndfx->extS.sndExts[i].smplCnt / 11000;

                    if ( sndfx->extS.sndExts[i].rlOffset > sample->bufsz )
                        sndfx->extS.sndExts[i].rlOffset = sample->bufsz;

                    if ( !sndfx->extS.sndExts[i].rlSmplCnt )
                        sndfx->extS.sndExts[i].rlSmplCnt = sample->bufsz;

                    if ( sndfx->extS.sndExts[i].rlSmplCnt + sndfx->extS.sndExts[i].rlOffset > sample->bufsz )
                        sndfx->extS.sndExts[i].rlSmplCnt = sample->bufsz - sndfx->extS.sndExts[i].rlOffset;
                }
                else
                {
                    ypa_log_out("Warning: Could not load sample %s.\n", sndfx->extSampleNames[i].c_str());
                }
            }
        }
        else if ( sndfx->sample_name[0] )
        {
            sndfx->single_sample = Nucleus::CInit<NC_STACK_wav>( {{NC_STACK_rsrc::RSRC_ATT_NAME, sndfx->sample_name}} );

            if ( !sndfx->single_sample )
                ypa_log_out("Warning: Could not load sample %s.\n", sndfx->sample_name.c_str());
        }

        set_prefix_replacement("rsrc", oldRsrc);
    }
}


void sub_4D7F60(NC_STACK_ypaworld *yw, int x, int y, stru_a3 *sct, baseRender_msg *bs77)
{
    sct->dword8 = 0;
    sct->dword4 = 0;

    if ( x >= 0  &&  x < yw->_mapWidth  &&  y >= 0  &&  y < yw->_mapHeight )
    {
        sct->dword4 = 1;
        sct->p_cell = yw->_cells + x + yw->_mapWidth * y;
        sct->smooth_height = sct->p_cell->averg_height;

        flag_xyz grp_1;
        grp_1.flag = 7;
        grp_1.v.x = x * 1200.0 + 600.0;
        grp_1.v.z = -(y * 1200.0 + 600.0);
        grp_1.v.y = sct->p_cell->height;

        sct->x = grp_1.v.x;
        sct->y = grp_1.v.y;
        sct->z = grp_1.v.z;

        yw->additionalBeeBox->base_func68(&grp_1);

        if ( yw->additionalBeeBox->base_func77(bs77) )
        {
            sct->dword8 = 1;
        }
    }
}

void sub_4D806C(NC_STACK_ypaworld *yw, stru_a3 *sct, baseRender_msg *bs77)
{
    if ( sct->dword8 )
    {
        cellArea *pcell = sct->p_cell;

        flag_xyz grp_1;
        grp_1.flag = 7;
        grp_1.v.x = sct->x;
        grp_1.v.y = sct->y;
        grp_1.v.z = sct->z;

        int v22 = 0;

        flag_xyz scel;

        if ( pcell->w_type == 1 )
        {
            yw_f80 *v5 = &yw->field_80[ pcell->w_id ];

            scel.flag = 2;
            scel.v.y = (float)v5->field_4 / (float)v5->field_8;

            pcell->type_id = yw->BuildProtos[ v5->blg_ID ].SecType;
            pcell->comp_type = yw->secTypes[ pcell->type_id ].field_0;

            v22 = 1;
        }

        int v17, v20;

        if ( pcell->comp_type == 1 )
        {
            v17 = 0;
            v20 = 1;
        }
        else
        {
            v17 = -1;
            v20 = 3;
        }

        for (int zz = 0; zz < v20; zz++)
        {
            for (int xx = 0; xx < v20; xx++)
            {
                grp_1.v.x = (v17 + xx) * 300.0 + sct->x;
                grp_1.v.z = (v17 + zz) * 300.0 + sct->z;

                if ( v22 )
                {
                    NC_STACK_base *bld = yw->legos[ yw->secTypes[ pcell->type_id ].buildings[xx][zz]->health_models[0] ].base;

                    bld->setBASE_static(0);

                    bld->base_func72(&scel);
                    bld->base_func68(&grp_1);
                    bld->base_func77(bs77);

                    bld->setBASE_static(1);
                }
                else
                {
                    NC_STACK_base *bld = yw->legos[ yw->secTypes[ pcell->type_id ].buildings[xx][zz]->health_models[ yw->build_hp_ref[ pcell->buildings_health[xx][zz] ] ] ].base;

                    bld->base_func68(&grp_1);
                    bld->base_func77(bs77);
                }
            }
        }
    }

    for ( NC_STACK_ypabact* &bact : sct->p_cell->unitsList )
    {
        if ( sct->dword8 || bact->_bact_type == BACT_TYPES_ROBO)
            bact->Render(bs77);
    }
}

void yw_renderSky(NC_STACK_ypaworld *yw, baseRender_msg *rndr_params)
{
    if ( yw->sky_loaded_base )
    {
        float v6 = rndr_params->maxZ;
        uint32_t flags = rndr_params->flags;

        flag_xyz v5;
        v5.v = yw->current_bact->_position + vec3d::OY(yw->field_15f4);
        v5.flag = 7;

        yw->sky_loaded_base->base_func68(&v5);

        rndr_params->maxZ = 32000.0;
        if (NC_STACK_win3d::win3d_keys[18].Get<bool>())
            rndr_params->flags = NC_STACK_display::RFLAGS_SKY;

        yw->sky_loaded_base->base_func77(rndr_params);

        rndr_params->maxZ = v6;
        rndr_params->flags = flags;
    }
}


int sb_0x4d7c08__sub1__sub0__sub0(NC_STACK_ypaworld *yw, float xx, float yy)
{
    int v7 = ((xx + 150) / 300) / 4;
    int v8 = ((-yy + 150) / 300) / 4;

    if ( v7 <= 0 || v7 >= yw->_mapWidth || v8 <= 0 || v8 >= yw->_mapHeight || !yw->current_bact )
        return 0;

    int v11 = abs(yw->current_bact->_sectX - v7);
    int v12 = abs(yw->current_bact->_sectY - v8);

    if ( v11 + v12 <= (yw->field_1368 - 1) / 2 )
        return 1;

    return 0;
}

void sb_0x4d7c08__sub1__sub0(NC_STACK_ypaworld *yw, float xx, float yy, float posx, float posy, baseRender_msg *arg)
{
    if ( yw->superbomb_wall_vproto )
    {
        if ( xx > 0.0 && yy < 0.0 && xx < yw->map_Width_meters && -yw->map_Height_meters < yy )
        {
            if ( sb_0x4d7c08__sub1__sub0__sub0(yw, xx, yy) )
            {
                int v10 = yw->VhclProtos[yw->superbomb_wall_vproto].vp_normal;

                NC_STACK_base *wall_base = yw->vhcls_models[v10].base;
                TFEngine::TForm3D *wall_trigo = yw->vhcls_models[v10].trigo;

                if ( wall_base && wall_trigo )
                {
                    float v28 = 0.0;

                    int v23 = (xx + 150) / 300;
                    int v26 = (-yy + 150) / 300;

                    if ( (v23 & 3) && (v26 & 3) )
                    {
                        v28 = yw->_cells[ (v26 / 4) * yw->_mapWidth + (v23 / 4) ].height;
                    }
                    else
                    {
                        ypaworld_arg136 v22;
                        v22.vect = vec3d::OY(50000.0);
                        v22.stPos.x = xx;
                        v22.stPos.y = -25000.0;
                        v22.stPos.z = yy;
                        v22.flags = 0;

                        yw->ypaworld_func136(&v22);

                        if ( v22.isect )
                        {
                            v28 = v22.isectPos.y;
                        }
                    }


                    wall_trigo->locPos = vec3d(xx, v28, yy);

                    float v29 = xx - posx;
                    float v30 = yy - posy;

                    float v27 = sqrt( POW2(v29) + POW2(v30) );
                    if ( v27 > 0.0 )
                    {
                        v29 /= v27;
                        v30 /= v27;
                    }

                    wall_trigo->locSclRot.m00 = v30;
                    wall_trigo->locSclRot.m01 = 0;
                    wall_trigo->locSclRot.m02 = -v29;
                    wall_trigo->locSclRot.m10 = 0;
                    wall_trigo->locSclRot.m11 = 1.0;
                    wall_trigo->locSclRot.m12 = 0;
                    wall_trigo->locSclRot.m20 = v29;
                    wall_trigo->locSclRot.m21 = 0.0;
                    wall_trigo->locSclRot.m22 = v30;

                    wall_base->base_func77(arg);
                }
            }
        }
    }
}

void sb_0x4d7c08__sub1(NC_STACK_ypaworld *yw, baseRender_msg *arg)
{
    // Render super items
    for ( const MapSuperItem &sitem : yw->_levelInfo->SuperItems )
    {
        if ( sitem.State == 3 )
        {
            float a4 = sitem.SecX * 1200.0 + 600.0;
            float a5 = -(sitem.SecY * 1200.0 + 600.0);


            float v14 = sqrt( POW2(yw->map_Width_meters) + POW2(yw->map_Height_meters) );

            if ( sitem.CurrentRadius > 300 && sitem.CurrentRadius < v14 )
            {
                float v17 = (2 * sitem.CurrentRadius) * C_PI / 300.0;

                if ( v17 > 2.0 )
                {
                    float v9 = 6.283 / v17;

                    for (float j = 0.0; j < 6.283; j = j + v9 )
                    {
                        float v10 = sitem.CurrentRadius;
                        float a3 = v10 * sin(j) + a5;
                        float a2 = v10 * cos(j) + a4;

                        sb_0x4d7c08__sub1__sub0(yw, a2, a3, a4, a5, arg);
                    }
                }
            }
        }
    }
}


NC_STACK_base * sb_0x4d7c08__sub3__sub0(NC_STACK_ypaworld *yw, stru_a3 *sct, stru_a3 *sct2, float a4, float a5)
{
    if ( sct->dword4 != 1 || sct2->dword4 != 1 || (sct->dword8 != 1 && sct2->dword8 != 1) )
        return 0;

    int x = yw->secTypes[ sct->p_cell->type_id ].field_1;
    int y = yw->secTypes[ sct2->p_cell->type_id ].field_1;

    NC_STACK_base *bs = yw->slurps2[x][y].skeletons_bas;
    UAskeleton::Data *skel = yw->slurps2[x][y].skeleton_internal;

    flag_xyz grp_1;
    grp_1.flag = 5;
    grp_1.v.x = sct2->x;
    grp_1.v.z = sct2->z;

    bs->base_func68(&grp_1);

    for (int i = 0; i < 4; i++)
        skel->POO[i].y = sct->y;

    for (int i = 4; i < 8; i++)
        skel->POO[i].y = sct2->y;

    skel->POO[8].y = a5;
    skel->POO[9].y = a4;

    return bs;
}

NC_STACK_base * sb_0x4d7c08__sub3__sub1(NC_STACK_ypaworld *yw, stru_a3 *sct, stru_a3 *sct2, float a4, float a5)
{
    if ( sct->dword4 != 1 || sct2->dword4 != 1 || (sct->dword8 != 1 && sct2->dword8 != 1) )
        return NULL;

    int x = yw->secTypes[ sct->p_cell->type_id ].field_1;
    int y = yw->secTypes[ sct2->p_cell->type_id ].field_1;

    NC_STACK_base *bs = yw->slurps1[x][y].skeletons_bas;
    UAskeleton::Data *skel = yw->slurps1[x][y].skeleton_internal;

    flag_xyz grp_1;
    grp_1.flag = 5;
    grp_1.v.x = sct2->x;
    grp_1.v.z = sct2->z;

    bs->base_func68(&grp_1);

    for (int i = 0; i < 4; i++)
        skel->POO[i].y = sct->y;

    for (int i = 4; i < 8; i++)
        skel->POO[i].y = sct2->y;

    skel->POO[8].y = a5;
    skel->POO[9].y = a4;

    return bs;
}




stru_a3 rendering_sectors[YW_RENDER_SECTORS_DEF * 2][ YW_RENDER_SECTORS_DEF * 2];

void sb_0x4d7c08__sub3(NC_STACK_ypaworld *yw, baseRender_msg *arg)
{
    //Render empty sectors and modify landscape linking parts
    for (int i = 0; i < yw->field_1368; i++)
    {
        for (int j = 0; j < yw->field_1368 - 1; j++)
        {
            stru_a3 *sct = &rendering_sectors[j][i];
            stru_a3 *sct2 = &rendering_sectors[j + 1][i];

            float h = rendering_sectors[j + 1][i].smooth_height;
            float h2 = rendering_sectors[j + 1][i + 1].smooth_height;

            NC_STACK_base *bs = sb_0x4d7c08__sub3__sub0(yw, sct, sct2, h, h2);
            if ( bs )
                bs->base_func77(arg);
        }
    }

    for (int i = 0; i < yw->field_1368 - 1; i++)
    {
        for (int j = 0; j < yw->field_1368; j++)
        {
            stru_a3 *sct = &rendering_sectors[j][i];
            stru_a3 *sct2 = &rendering_sectors[j][i + 1];

            float h = rendering_sectors[j][i + 1].smooth_height;
            float h2 = rendering_sectors[j + 1][i + 1].smooth_height;

            NC_STACK_base *bs = sb_0x4d7c08__sub3__sub1(yw, sct, sct2, h, h2);
            if ( bs )
                bs->base_func77(arg);

        }
    }
}

void sb_0x4d7c08(NC_STACK_ypaworld *yw, base_64arg *bs64, int a2)
{
    if ( yw->current_bact )
    {
        TFEngine::TForm3D *v5 = TFEngine::Engine.GetViewPoint();

        if ( v5 )
            v5->CalcGlobal();

        baseRender_msg rndrs;

        rndrs.flags = 0;
        rndrs.frameTime = bs64->DTime;
        rndrs.globTime = bs64->TimeStamp;
        rndrs.adeCount = 0;
        rndrs.ownerID = 1;
        rndrs.rndrStack = &NC_STACK_base::renderStack;

        rndrs.minZ = 1.0;

        if ( yw->field_1368 == 5 )
            rndrs.maxZ = 1500.0;
        else
            rndrs.maxZ = 3500.0;

        int v6 = yw->field_1368 - 1;

        for (int j = 0; j < v6; j++)
        {
            for (int i = 0; i < v6; i++)
            {
                rendering_sectors[j][i].dword4 = 0;
                rendering_sectors[j][i].dword8 = 0;
            }
        }

        int v29 = v6 / 2;
        for (int i = 0; i <= v29; i++)
        {
            int v28 = v29 - i;

            for (int j = -i; j <= i; j++)
            {
                stru_a3 *sct = &rendering_sectors[v29 + j][v29 - v28];

                sub_4D7F60(yw, j + yw->current_bact->_sectX, -v28 + yw->current_bact->_sectY, sct, &rndrs);

                if ( sct->dword4 )
                    sub_4D806C(yw, sct, &rndrs);

            }

            if ( -v28 != v28 )
            {
                for (int j = -i; j <= i; j++)
                {
                    stru_a3 *sct = &rendering_sectors[v29 + j][v29 + v28];

                    sub_4D7F60(yw, j + yw->current_bact->_sectX, v28 + yw->current_bact->_sectY, sct, &rndrs);

                    if ( sct->dword4 )
                        sub_4D806C(yw, sct, &rndrs);
                }
            }
        }

        sb_0x4d7c08__sub3(yw, &rndrs);
        sb_0x4d7c08__sub1(yw, &rndrs);

        if ( yw->field_15f8 )
            yw_renderSky(yw, &rndrs);

        bs64->field_C = rndrs.adeCount;

        yw->field_1B6A = rndrs.adeCount;
        yw->field_1b6c = rndrs.rndrStack->getSize();

        yw->_win3d->BeginScene();

        if (NC_STACK_win3d::win3d_keys[18].Get<bool>())
            rndrs.rndrStack->render(true, RenderStack::comparePrio);
        else
            rndrs.rndrStack->render(false);

        yw->_win3d->EndScene();

        if ( a2 )
        {
            sb_0x4d7c08__sub0(yw);
        }
    }
}


void NC_STACK_ypaworld::sb_0x456384__sub0__sub0()
{
    for( int y = 0; y < _mapHeight; y++ )
    {
        for( int x = 0; x < _mapWidth; x++ )
        {
            yw_f30 *f30 = &field_30[y * 64 + x];
            cellArea *cell = &_cells[x + y * _mapWidth];

            f30->field_1 = 0;
            f30->owner = cell->owner;

        }
    }

    _lastUpdatedPowerStationID = 0;
}

int NC_STACK_ypaworld::sb_0x456384__sub0(int x, int y, int power)
{
    int v13 = 0;
    cellArea *cell = &_cells[x + y * _mapWidth];

    size_t v7;
    for (v7 = 0; v7 < _powerStationsCount; v7++)
    {
        if ( !_powerStations[v7].p_cell )
        {
            v13 = 1;
            break;
        }
    }

    if ( v7 >= 256 )
        return -1;

    if ( !v13 )
        _powerStationsCount = v7 + 1;

    yw_field34 *v9 = &_powerStations[v7];
    v9->x = x;
    v9->y = y;
    v9->power = power;
    v9->power_2 = power;
    v9->p_cell = cell;

    cell->w_type = 2;
    cell->w_id = v7;

    sb_0x456384__sub0__sub0();

    return v7;
}


void NC_STACK_ypaworld::sb_0x456384(int x, int y, int ownerid2, int blg_id, int a7)
{
    uamessage_bldVhcl bvMsg;
    memset(&bvMsg, 0, sizeof(bvMsg));

    cellArea *cell = &_cells[ _mapWidth * y + x ];
    TBuildingProto *bld = &BuildProtos[ blg_id ];
    secType *sectp = &secTypes[ bld->SecType ];

    int v43 = 1;

    NC_STACK_yparobo *robo = NULL;

    if ( x && y && _mapWidth - 1 != x && _mapHeight - 1 != y )
    {
        (*blg_map)(x, y) = blg_id;
        (*typ_map)(x, y) = bld->SecType;

        cell->type_id = bld->SecType;
        cell->energy_power = 0;
        cell->w_type = 3;
        cell->comp_type = sectp->field_0;
        cell->w_id = blg_id;

        int v49;

        if ( sectp->field_0 == 1 )
        {
            memset(cell->buildings_health, 0, sizeof(cell->buildings_health));
            v49 = 1;
        }
        else
        {
            v49 = 3;
        }

        for (int i = 0; i < v49; i++)
        {
            for (int j = 0; j < v49; j++)
            {
                cell->buildings_health[i][j] = sectp->buildings[i][j]->build_health;
            }
        }

        if ( bld->ModelID == 1 )
            sb_0x456384__sub0(x, y, bld->Power);

        CellSetOwner(cell, x, y, ownerid2);

        for( NC_STACK_ypabact * &unit: _unitsList )
        {
            if (unit->_bact_type == BACT_TYPES_ROBO && unit->_owner == ownerid2)
            {
                robo = dynamic_cast<NC_STACK_yparobo *>(unit);
                break;
            }
        }

        if ( isNetGame )
        {
            if ( robo != UserRobo )
                v43 = 0;
        }

        if ( !a7 )
        {
            if ( robo && robo->_status != BACT_STATUS_DEAD && v43 )
            {
                NC_STACK_ypagun *commander = NULL;

                int v39 = robo->getROBO_commCount();

                v39++;

                robo->setROBO_commCount(v39);

                for ( size_t i = 0; i < bld->Guns.size(); i++)
                {
                    TBuildingProto::TGun &GunProto = bld->Guns[i];
                    
                    if ( !GunProto.VhclID )
                        break;

                    ypaworld_arg146 v33;
                    v33.vehicle_id = GunProto.VhclID;
                    v33.pos = GunProto.Pos + vec3d(x * 1200.0 + 600.0, 0.0, -(y * 1200.0 + 600.0));

                    NC_STACK_ypabact *gun_obj = ypaworld_func146(&v33);
                    NC_STACK_ypagun *gunn = dynamic_cast<NC_STACK_ypagun *>(gun_obj);

                    if ( gun_obj )
                    {
                        gun_obj->_owner = ownerid2;

                        if (gunn)
                            gunn->ypagun_func128(GunProto.Dir, false);

                        setState_msg v34;
                        v34.newStatus = BACT_STATUS_CREATE;
                        v34.unsetFlags = 0;
                        v34.setFlags = 0;

                        gunn->SetStateInternal(&v34);

                        gun_obj->_scale_time = 500;
                        gun_obj->_scale = vec3d(1.0, 1.0, 1.0);

                        gun_obj->_host_station = robo;
                        gun_obj->_commandID = v39;

                        if ( isNetGame )
                        {
                            gun_obj->_gid |= ownerid2 << 24;

                            bvMsg.vhcl[i].id = gun_obj->_gid;
                            bvMsg.vhcl[i].base = GunProto.Dir;
                            bvMsg.vhcl[i].pos = gun_obj->_position;
                            bvMsg.vhcl[i].protoID = GunProto.VhclID;
                        }

                        if ( commander )
                        {
                            commander->AddSubject(gunn);
                        }
                        else
                        {
                            commander = gunn;

                            robo->AddSubject(gunn);
                        }
                    }
                }

                if ( isNetGame )
                {
                    bvMsg.msgID = UAMSG_BUILDINGVHCL;
                    bvMsg.tstamp = timeStamp;
                    bvMsg.owner = ownerid2;

                    yw_arg181 v31;
                    v31.recvFlags = 2;
                    v31.recvID = 0;
                    v31.senderID = GameShell->callSIGN.c_str();
                    v31.senderFlags = 1;
                    v31.data = &bvMsg;
                    v31.dataSize = sizeof(bvMsg);
                    v31.garant = 1;

                    ypaworld_func181(&v31);
                }
            }
        }
    }
}

void ypaworld_func148__sub0(NC_STACK_ypaworld *yw, int x, int y)
{
    for ( NC_STACK_ypabact* &node : yw->_cells[yw->_mapWidth * y + x].unitsList )
    {
        int v5 = 0;

        if ( yw->isNetGame )
        {
            if ( node->_owner == yw->UserUnit->_owner )
            {
                if ( node->_status != BACT_STATUS_DEAD && node->_status != BACT_STATUS_BEAM && node->_status != BACT_STATUS_CREATE )
                {
                    if ( node->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( node );

                        if (!gun->IsRoboGun())
                            v5 = 1;
                    }
                }
            }
        }
        else
        {
            if ( node->_status != BACT_STATUS_DEAD && node->_status != BACT_STATUS_BEAM && node->_status != BACT_STATUS_CREATE )
            {
                if ( node->_bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( node );

                    if (!gun->IsRoboGun())
                        v5 = 1;
                }
            }
        }

        if ( v5 )
        {
            bact_arg84 v8;
            v8.energy = -22000000;
            v8.unit = NULL;

            node->ModifyEnergy(&v8);
        }
    }
}

int NC_STACK_ypaworld::ypaworld_func148__sub1(int id, int a4, int x, int y, int ownerID2, char blg_ID)
{
    if ( id < 8 && !field_80[id].field_0 && x && y && x != _mapWidth - 1 && y != _mapHeight - 1 )
    {
        field_80[id].field_4 = 0;
        field_80[id].field_0 = 1;
        field_80[id].field_8 = a4;
        field_80[id].x = x;
        field_80[id].y = y;
        field_80[id].ownerID2 = ownerID2;
        field_80[id].blg_ID = blg_ID;

        cellArea *cell = &_cells[_mapWidth * y + x];
        cell->w_type = 1;
        cell->w_id = id;

        for (NC_STACK_ypabact * &unit : _unitsList)
        {
            if ( unit->_bact_type == BACT_TYPES_ROBO && ownerID2 == unit->_owner )
            {
                SFXEngine::SFXe.startSound(&unit->_soundcarrier, 11);
                break;
            }
        }

        return 1;
    }

    return 0;
}

int ypaworld_func137__sub0__sub0(UAskeleton::Data *skl, int id, const vec3d &v, float r, vec3d &out)
{
    int num = skl->polygons[id].num_vertices;
    int16_t *vtx = skl->polygons[id].v;

    vec3d tmp(0.0, 0.0, 0.0);

    for (int i = 0; i < num; i++)
    {
        int16_t idd = vtx[i];
        tmp += static_cast<vec3d> (skl->POO[ idd ]);
    }

    vec3d tmp2 = tmp / num - v;

    float v26 = tmp2.length();

    if ( v26 <= r )
        return 0;

    out = tmp2 * (r / v26) + v;

    return 1;
}

void ypaworld_func137__sub0(ypaworld_arg137 *arg, const struct_44dbf8 &a2)
{
    for (int i = 0; i < a2.sklt->polygonsCount; i++)
    {
        const UAskeleton::Polygon &tria = a2.sklt->polygons[i];

        vec3d t0 = tria.Normal();

        float v9 = t0.dot( arg->pos2 );

        if ( v9 > 0.0 )
        {
            float v26 = -( t0.dot( arg->pos ) + tria.D) / ( t0.dot( t0 ) * arg->radius);

            if ( v26 > 0.0 && v26 <= 1.0 )
            {
                vec3d tx = arg->pos + t0 * (arg->radius * v26);

                int v27 = 0;

                vec3d v18;

                if ( ypaworld_func137__sub0__sub0(a2.sklt, i, tx, arg->radius, v18) )
                {
                    if ( sub_44D36C(v18, i, a2.sklt) )
                        v27 = 1;
                }
                else
                    v27 = 1;

                if ( v27 )
                {
                    if ( arg->coll_count < arg->coll_max )
                    {
                        int pos = arg->coll_count;

                        arg->collisions[pos].pos1 = a2.pos + tx;
                        arg->collisions[pos].pos2 = tria.Normal();

                        arg->coll_count++;
                    }
                }
            }
        }
    }
}

NC_STACK_ypabact * NC_STACK_ypaworld::FindBactByCmdOwn(int commandID, char owner)
{
    for ( NC_STACK_ypabact * &robo : _unitsList )
    {
        if ( robo->_bact_type == BACT_TYPES_ROBO && robo->_owner == owner)
        {
            if ( robo->_commandID == commandID )
            {
                if ( robo->_status == BACT_STATUS_DEAD )
                    return NULL;
                else
                    return robo;
            }
            else
            {
                for ( NC_STACK_ypabact * &unit : robo->_kidList )
                {
                    if ( unit->_commandID == commandID )
                    {
                        if ( unit->_status == BACT_STATUS_DEAD )
                            return NULL;
                        else
                            return unit;
                    }
                }
            }
        }
    }

    return NULL;
}


void ypaworld_func64__sub20(NC_STACK_ypaworld *yw, int dtime)
{
    for(int i = 0; i < 8; i++)
    {
        if ( yw->field_80[i].field_0 )
        {
            yw->field_80[i].field_4 += dtime;

            if ( yw->field_80[i].field_4 >= yw->field_80[i].field_8 )
            {
                yw->field_80[i].field_0 = 0;

                int v8 = yw->field_80[i].y;
                int v10 = yw->field_80[i].x;

                cellArea * v11 = &yw->_cells[v10 + v8 * yw->_mapWidth];

                int a6 = yw->field_80[i].blg_ID;

                v11->w_type = 0;
                v11->w_id = 0;

                yw->sb_0x456384(v10, v8, yw->field_80[i].ownerID2, a6, 0);

                if ( yw->field_80[i].ownerID2 == yw->UserRobo->_owner )
                {
                    if ( yw->BuildProtos[a6].ModelID )
                    {
                        yw_arg159 arg159;

                        arg159.unit = yw->UserRobo;
                        arg159.txt = 0;
                        arg159.field_4 = 65;

                        if ( yw->BuildProtos[a6].ModelID == 1 )
                            arg159.field_C = 36;
                        else if ( yw->BuildProtos[a6].ModelID == 2 )
                            arg159.field_C = 38;
                        else if ( yw->BuildProtos[a6].ModelID == 3 )
                            arg159.field_C = 37;
                        else
                            arg159.field_C = 0;

                        yw->ypaworld_func159(&arg159);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub6__sub0()
{
    for(int i = 0; i < 8; i++)
        field_1bac[i] = 0;

    for ( NC_STACK_ypabact * &robo : _unitsList )
    {
        if ( robo->_bact_type == BACT_TYPES_ROBO && robo->_status != BACT_STATUS_DEAD && robo->_status != BACT_STATUS_BEAM )
        {
            for( NC_STACK_ypabact * &comnd : robo->_kidList )
            {
                if ( comnd->_status != BACT_STATUS_DEAD && comnd->_status != BACT_STATUS_BEAM )
                {
                    bool a4 = false;

                    if ( comnd->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( comnd );
                        a4 = gun->IsRoboGun();
                    }

                    if ( !a4 )
                    {
                        field_1bac[ comnd->_owner ]++;

                        for( NC_STACK_ypabact * &unit : comnd->_kidList )
                        {
                            if ( unit->_status != BACT_STATUS_DEAD && unit->_status != BACT_STATUS_BEAM )
                                field_1bac[ unit->_owner ]++;
                        }
                    }
                }
            }
        }
    }
}


void ypaworld_func64__sub6(NC_STACK_ypaworld *yw)
{
    int v13[8];

    for (int i = 0; i < 8; i++)
        v13[i] = 0;

    for (size_t i = 0; i < yw->_powerStationsCount; i++)
    {
        yw_field34 *v4 = &yw->_powerStations[i];

        if (v4->p_cell)
            v13[ v4->p_cell->owner ] += v4->power_2;
    }

    yw->ypaworld_func64__sub6__sub0();

    for (int i = 0; i < 8; i++)
    {
        v13[i] /= 2;

        if ( v13[i] <= 0 )
        {
            yw->field_1bec[i] = 0;
            yw->field_1bcc[i] = 0;
        }
        else
        {
            int v15 = yw->sectors_count_by_owner[i];

            if ( v15 < 0 )
                v15 = 0;

            yw->field_1bcc[i] = (float)v15 / (float)v13[i];
            yw->field_1bec[i] = (float)v15 / (float)v13[i];

            if ( yw->isNetGame )
            {
                if ( yw->unit_limit_type_1 == 1 )
                {
                    int v16 = yw->field_1bac[yw->UserRobo->_owner] - yw->unit_limit_1;

                    if ( v16 > 0 )
                    {
                        int v10 = (float)yw->unit_limit_arg_1 * 0.01 * (float)v16;

                        yw->field_1bcc[i] -= v10;
                        yw->field_1bec[i] -= v10;
                    }
                }
            }

            if ( yw->field_1bcc[i] >= 0.0 )
            {
                if ( yw->field_1bcc[i] > 1.0 )
                    yw->field_1bcc[i] = 1.0;
            }
            else
            {
                yw->field_1bcc[i] = 0;
            }

            if ( yw->field_1bec[i] < 0.0 )
                yw->field_1bec[i] = 0;
        }
    }
}


void ypaworld_func64__sub5__sub0(NC_STACK_ypaworld *yw, int a2)
{
    yw_field34 *v3 = &yw->_powerStations[a2];

    int v7 = v3->power_2;
    int v10 = 0;

    while (v7 > 0)
    {
        v7 >>= 1;
        v10++;
    }

    int v9 = v3->power_2;

    int v11 = -v10;
    int v12 = v10 + 1;

    int v13 = -v10;
    int v21 = v10 + 1;

    if ( v3->x + v11 < 1 )
        v11 = 1 - v3->x;

    if ( v3->y + v13 < 1 )
        v13 = 1 - v3->y;

    if ( v3->x + v12 >= yw->_mapWidth )
        v12 = yw->_mapWidth - v3->x - 1;

    if ( v3->y + v21 >= yw->_mapHeight )
        v21 = yw->_mapHeight - v3->y - 1;

    for (int i = v13; i < v21; i++)
    {
        for (int j = v11; j < v12; j++)
        {
            int v17 = v9  >>  yw->sqrt_table[abs(j)][abs(i)];

            yw_f30 *v14 = &yw->field_30[j + v3->x + ((i + v3->y) * 64) ];

            if ( v14->owner == v3->p_cell->owner )
            {
                int v18 = v17 + v14->field_1; // Add power to this cell

                if ( v18 > 255 )
                    v18 = 255;

                v14->field_1 = v18;
            }

        }
    }
}

void ypaworld_func64__sub5(NC_STACK_ypaworld *yw)
{
    // Recompute power on sectors
    if ( yw->_powerStationsCount ) // If we have powerstations
    {
        size_t pID = yw->_lastUpdatedPowerStationID; // current power station

        while ( !yw->_powerStations[pID].p_cell && pID < yw->_powerStationsCount ) //If current power station is null - go to first not null or to end
            pID++;

        if ( pID < yw->_powerStationsCount ) // if we found any power station
        {
            if ( yw->_powerStations[pID].power_2 ) // if this power station has power
                ypaworld_func64__sub5__sub0(yw, pID); // Add power to power matrix

            yw->_lastUpdatedPowerStationID = pID + 1; // go to next station in next update loop
        }
        else // If we reach end of power stations list, apply power to sectors
        {
            yw->UpdatePowerEnergy(); // Apply power to sectors and clean power matrix for next compute iteration.
        }
    }
}


void NC_STACK_ypaworld::sub_4D12D8(int id, int a3)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];

    sitem.State = 1;
    sitem.TriggerTime = 0;
    sitem.ActivateOwner = sitem.PCell->owner;

    if ( !a3 )
    {
        sitem.ActiveTime = timeStamp;
        sitem.LastTenSec = 0;
        sitem.LastSec = 0;
        sitem.CountDown = sitem.TimerValue;
    }

    ypaworld_arg148 arg148;
    arg148.ownerID = sitem.PCell->owner;
    arg148.ownerID2 = sitem.PCell->owner;
    arg148.blg_ID = sitem.ActiveBldID;
    arg148.field_C = 1;
    arg148.x = sitem.SecX;
    arg148.y = sitem.SecY;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->w_type = 8;
    sitem.PCell->w_id = id;

    yw_arg159 arg159;
    arg159.unit = 0;
    arg159.field_4 = 94;

    if ( sitem.Type == 1 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 250, "Superbomb activated.");
        arg159.field_C = 70;
    }
    else if ( sitem.Type == 2 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 254, "Superwave activated.");
        arg159.field_C = 74;
    }
    else
    {
        arg159.field_C = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::sub_4D1594(int id)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];

    sitem.State = 2;

    ypaworld_arg148 arg148;
    arg148.ownerID = sitem.PCell->owner;
    arg148.ownerID2 = sitem.PCell->owner;
    arg148.blg_ID = sitem.InactiveBldID;
    arg148.field_C = 1;
    arg148.x = sitem.SecX;
    arg148.y = sitem.SecY;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->w_type = 8;
    sitem.PCell->w_id = id;

    yw_arg159 arg159;
    arg159.unit = 0;
    arg159.field_4 = 93;

    if ( sitem.Type == 1 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 252, "Superbomb frozen.");
        arg159.field_C = 72;
    }
    else if ( sitem.Type == 2 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 256, "Superwave frozen.");
        arg159.field_C = 76;
    }
    else
    {
        arg159.field_C = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::sub_4D1444(int id)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];
    sitem.State = 3;
    sitem.TriggerTime = timeStamp;

    ypaworld_arg148 arg148;
    arg148.ownerID = sitem.PCell->owner;
    arg148.ownerID2 = sitem.PCell->owner;
    arg148.blg_ID = sitem.TriggerBldID;
    arg148.field_C = 1;
    arg148.x = sitem.SecX;
    arg148.y = sitem.SecY;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->w_type = 8;
    sitem.PCell->w_id = id;

    sitem.LastRadius = 0;

    yw_arg159 arg159;
    arg159.field_4 = 95;
    arg159.unit = 0;

    if ( sitem.Type == 1 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 251, "Superbomb triggered.");
        arg159.field_C = 71;
    }
    else if ( sitem.Type == 2 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 255, "Superwave triggered.");
        arg159.field_C = 75;
    }
    else
    {
        arg159.field_C = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}


NC_STACK_ypabact * NC_STACK_ypaworld::sb_0x47b028__sub0(uint32_t bactid)
{
    for ( NC_STACK_ypabact * &station : _unitsList )
    {
        if ( bactid == station->_gid )
            return station;

        for ( NC_STACK_ypabact * &commander : station->_kidList )
        {
            if ( bactid == commander->_gid )
                return station; // CHECK IT

            for ( NC_STACK_ypabact * &slave : commander->_kidList )
            {
                if ( bactid == slave->_gid )
                    return station;  // CHECK IT
            }
        }
    }

    return NULL;
}

void NC_STACK_ypaworld::RefreshUnitPRT(NC_STACK_ypabact *unit, NC_STACK_ypabact *robo, bool isRobo)
{
    bact_arg80 arg80;
    arg80.pos = unit->_position;

    if ( unit->_bact_type == BACT_TYPES_GUN )
        arg80.field_C = 4;
    else
        arg80.field_C = 0;

    unit->SetPosition(&arg80);

    if ( unit->_bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *guno = (NC_STACK_ypagun *)unit;

        guno->ypagun_func128(guno->_gunBasis, false);
    }

    setState_msg arg78;
    arg78.newStatus = unit->_status;
    arg78.setFlags = 0;
    arg78.unsetFlags = 0;
    unit->SetState(&arg78);

    if ( unit->_status_flg & BACT_STFLAG_DEATH2 )
    {
        arg78.newStatus = BACT_STATUS_NOPE;
        arg78.unsetFlags = 0;
        arg78.setFlags = BACT_STFLAG_DEATH2;
        unit->SetState(&arg78);
    }

    if ( unit->_status_flg & BACT_STFLAG_FIRE )
    {
        arg78.newStatus = BACT_STATUS_NOPE;
        arg78.unsetFlags = 0;
        arg78.setFlags = BACT_STFLAG_FIRE;
        unit->SetState(&arg78);
    }

    if ( !isRobo )
    {
        unit->_host_station = dynamic_cast<NC_STACK_yparobo *>(robo);
        unit->_owner = robo->_owner;
    }

    if ( unit->_primTtype == BACT_TGT_TYPE_UNIT )
    {
        unit->_primTtype = BACT_TGT_TYPE_NONE;

        setTarget_msg arg67;
        arg67.tgt.pbact = sb_0x47b028__sub0((int)(size_t)unit->_primT.pbact);
        arg67.tgt_type = BACT_TGT_TYPE_UNIT_IND;
        arg67.priority = 0;
        unit->SetTarget(&arg67);
    }

    if ( unit->_primTtype == BACT_TGT_TYPE_CELL )
    {
        unit->_primTtype = BACT_TGT_TYPE_NONE;

        setTarget_msg arg67_1;
        arg67_1.tgt_type = BACT_TGT_TYPE_CELL_IND;
        arg67_1.tgt_pos = unit->_primTpos;
        arg67_1.priority = 0;

        unit->SetTarget(&arg67_1);
    }
}


int ypaworld_func64__sub4(NC_STACK_ypaworld *yw, base_64arg *arg)
{
    if ( yw->isNetGame )
        return 0;

    if ( !yw->field_160c )
    {
        if ( arg->field_8->HotKeyID == 32 || arg->field_8->KbdLastHit == Input::KC_PAUSE )
        {
            yw->field_160c = 1;
            yw->field_1610 = arg->TimeStamp;
        }
        return 0;
    }

    if ( arg->field_8->KbdLastHit != Input::KC_NONE )
    {
        yw->field_160c = 0;
        arg->TimeStamp = yw->field_1610;
    }
    else
    {
        yw->_win3d->BeginFrame();

        yw->_win3d->setRSTR_BGpen(0);

        yw->_win3d->raster_func192(NULL);

        vec3d a2a = yw->field_1334 + vec3d::OY(50000.0);

        SFXEngine::SFXe.sub_423EFC(1, a2a, vec3d(0.0), mat3x3::Ident());

        if ( arg->TimeStamp / 500 & 1 )
        {
            const char *v6 = get_lang_string(yw->string_pointers_p2, 14, "*** GAME PAUSED, HIT KEY TO CONTINUE ***");

            char v10[256];
            char *pcur = v10;

            FontUA::select_tileset(&pcur, 15);

            FontUA::set_xpos(&pcur, 0);
            FontUA::set_center_ypos(&pcur, -yw->font_default_h / 2);

            pcur = FontUA::FormateCenteredSkipableItem(yw->tiles[15], pcur, v6, yw->screen_width);

            FontUA::set_end(&pcur);

            w3d_a209 arg209;
            arg209.includ = 0;
            arg209.cmdbuf = v10;

            yw->_win3d->raster_func209(&arg209);
        }

        SFXEngine::SFXe.sb_0x424c74();

        yw->_win3d->EndFrame();
    }
    return 1;
}


void ypaworld_func64__sub2(NC_STACK_ypaworld *yw)
{
    yw->field_1b70 = 0;

    if ( yw->UserRobo != yw->UserUnit )
    {
        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->UserRobo);

        roboGun *guns = robo->getROBO_guns();

        if ( guns )
        {
            for (int i = 0; i < 8; i++)
            {
                if ( yw->UserUnit == guns[i].gun_obj )
                    yw->field_1b70 = 1;
            }
        }
    }
}


void ypaworld_func64__sub9(NC_STACK_ypaworld *yw)
{
    for ( size_t i = 0; i < yw->_levelInfo->Gates.size(); i++ )
    {
        const MapGate &gate = yw->_levelInfo->Gates[i];
        int v21 = 6;

        if ( gate.PCell->owner == yw->UserRobo->_owner )
        {
            for( const MapKeySector &ks : gate.KeySectors )
            {
                if (ks.PCell)
                {
                    if (ks.PCell->owner != yw->UserRobo->_owner)
                    {
                        v21 = 5;
                        break;
                    }
                }
            }
        }
        else
        {
            v21 = 5;
        }

        if ( gate.PCell->w_type != v21 )
        {
            ypaworld_arg148 arg148;
            arg148.ownerID = gate.PCell->owner;
            arg148.ownerID2 = gate.PCell->owner;
            arg148.field_C = 1;
            arg148.x = gate.SecX;
            arg148.y = gate.SecY;
            arg148.field_18 = 0;

            if ( v21 == 6 )
            {
                arg148.blg_ID = gate.OpenBldID;
            }
            else
            {
                arg148.blg_ID = gate.ClosedBldID;

                yw_arg159 arg159;
                arg159.unit = 0;
                arg159.field_4 = 65;
                arg159.txt = get_lang_string(yw->string_pointers_p2, 224, "TRANSPORTER GATE CLOSED!");
                arg159.field_C = 24;

                yw->ypaworld_func159(&arg159);
            }

            yw->ypaworld_func148(&arg148);

            gate.PCell->w_type = v21;
            gate.PCell->w_id = i;
        }

        if ( v21 == 6 )
        {
            int energ = 0;

            for ( NC_STACK_ypabact* &v8 : gate.PCell->unitsList )
            {
                if ( v8->_status != BACT_STATUS_DEAD && v8->_status != BACT_STATUS_BEAM )
                {
                    if ( v8->_bact_type != BACT_TYPES_ROBO && v8->_bact_type != BACT_TYPES_MISSLE && v8->_bact_type != BACT_TYPES_GUN )
                        energ += (v8->_energy_max + 99) / 100;
                }
            }

            if ( energ <= yw->beamenergy )
            {
                if ( yw->timeStamp - yw->field_1a00 > 60000 )
                {
                    yw_arg159 arg159_1;
                    arg159_1.unit = 0;
                    arg159_1.field_4 = 49;
                    arg159_1.txt = get_lang_string(yw->string_pointers_p2, 223, "TRANSPORTER GATE OPENED!");
                    arg159_1.field_C = 23;

                    yw->ypaworld_func159(&arg159_1);
                    yw->field_1a00 = yw->timeStamp;
                }
            }
            else
            {
                if ( yw->timeStamp - yw->field_1a00 > 40000 )
                {
                    yw_arg159 arg159_2;
                    arg159_2.unit = 0;
                    arg159_2.field_4 = 10;
                    arg159_2.txt = get_lang_string(yw->string_pointers_p2, 258, "WARNING: BEAM GATE FULL!");
                    arg159_2.field_C = 46;

                    yw->ypaworld_func159(&arg159_2);
                    yw->field_1a00 = yw->timeStamp;
                }
            }
        }
    }
}


bool NC_STACK_ypaworld::sub_4D11C0(int id, int owner)
{
    const MapSuperItem &sitem = _levelInfo->SuperItems[id];

    if ( sitem.PCell->owner != owner )
        return false;

    if ( sitem.KeySectors.empty() )
        return true;

    for ( const MapKeySector &ks : sitem.KeySectors )
    {
        if ( ks.PCell->owner != owner )
            return false;
    }
    return true;
}

bool NC_STACK_ypaworld::sub_4D12A0(int owner)
{
    for ( NC_STACK_ypabact * &unit : _unitsList )
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && owner == unit->_owner )
            return true;
    }

    return false;
}

void NC_STACK_ypaworld::sub_4D16C4(int id)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];

    sitem.State = 0;
    sitem.ActiveTime = 0;
    sitem.TriggerTime = 0;
    sitem.ActivateOwner = 0;
    sitem.CountDown = 0;

    ypaworld_arg148 arg148;
    arg148.ownerID = sitem.PCell->owner;
    arg148.ownerID2 = sitem.PCell->owner;
    arg148.blg_ID = sitem.InactiveBldID;
    arg148.field_C = 1;
    arg148.x = sitem.SecX;
    arg148.y = sitem.SecY;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->w_type = 8;
    sitem.PCell->w_id = id;

    yw_arg159 arg159;
    arg159.unit = NULL;
    arg159.field_4 = 92;

    if ( sitem.Type == 1 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 253, "Superbomb deactivated.");
        arg159.field_C = 73;
    }
    else if ( sitem.Type == 2 )
    {
        arg159.txt = get_lang_string(string_pointers_p2, 257, "Superwave deactivated.");
        arg159.field_C = 77;
    }
    else
    {
        arg159.field_C = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub0(int id)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];

    for( NC_STACK_ypabact * &unit : _unitsList )
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && sitem.ActivateOwner == unit->_owner )
        {
            if ( sub_4D11C0(id, sitem.ActivateOwner) )
            {
                if ( sitem.CountDown > 0 )
                    sitem.CountDown = sitem.CountDown - field_1618;
                else
                    sub_4D1444(id);
            }
            else if ( !sitem.KeySectors.empty() )
            {
                sub_4D1594(id);
            }
            else
            {
                sub_4D12D8(id, 0);
            }
            return;
        }
    }

    sub_4D16C4(id);
}


void ypaworld_func64__sub19__sub3(NC_STACK_ypaworld *yw, int id)
{
    if ( yw->GameShell )
    {
        MapSuperItem &sitem = yw->_levelInfo->SuperItems[id];

        int v4 = sitem.CountDown / 1024;

        if ( v4 < 10 && v4 != sitem.LastSec )
        {
            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 3);
            sitem.LastSec = v4;
        }

        int v5 = v4 / 10;
        if ( v5 != sitem.LastTenSec )
        {
            SFXEngine::SFXe.startSound(&yw->GameShell->samples1_info, 3);
            sitem.LastTenSec = v5;
        }
    }
}

bool NC_STACK_ypaworld::sub_4D1230(int id, int a3)
{
    const MapSuperItem &sitem = _levelInfo->SuperItems[id];

    if ( sitem.PCell->owner == a3 )
        return false;

    if ( sitem.KeySectors.empty() )
        return true;

    for ( const MapKeySector &ks : sitem.KeySectors )
    {
        if (ks.PCell->owner == a3)
            return false;
    }

    return true;
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub1(int id)
{
    const MapSuperItem &sitem = _levelInfo->SuperItems[id];

    for ( NC_STACK_ypabact * &unit : _unitsList )
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && sitem.ActivateOwner == unit->_owner )
        {
            if ( sub_4D11C0(id, sitem.ActivateOwner) )
                sub_4D12D8(id, 1);
            else if ( sub_4D1230(id, sitem.ActivateOwner) )
                sub_4D16C4(id);
            return;
        }
    }

    sub_4D16C4(id);
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub2__sub0__sub0(uint8_t activate, float a5, float a6, float a7)
{
    int lastt = _mapHeight * _mapWidth;

    for(int i = 0; i < lastt; i++)
    {
        cellArea *cell = &_cells[i];

        for( NC_STACK_ypabact* &bct : cell->unitsList )
        {
            int v9 = 1;

            if ( isNetGame )
            {
                if ( bct->_owner != UserUnit->_owner || bct->_owner == activate || bct->_status == BACT_STATUS_DEAD )
                    v9 = 0;
            }
            else if ( bct->_owner == activate || bct->_status == BACT_STATUS_DEAD )
            {
                v9 = 0;
            }

            if ( v9 )
            {
                float v10 = a5 - bct->_position.x;
                float v11 = a6 - bct->_position.z;

                if ( sqrt(POW2(v10) + POW2(v11)) < a7 )
                {
                    bact_arg84 arg84;
                    arg84.energy = -22000000;
                    arg84.unit = NULL;

                    bct->ModifyEnergy(&arg84);
                }
            }
        }

    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub2__sub0(int id)
{
    MapSuperItem &sitem = _levelInfo->SuperItems[id];

    sitem.CurrentRadius = (timeStamp - sitem.TriggerTime) * 1200.0 / 2400.0;

    float a5 = sitem.SecX * 1200.0 + 600.0;
    float a6 = -(sitem.SecY * 1200.0 + 600.0);

    float v19 = sqrt(POW2(map_Width_meters) + POW2(map_Height_meters));

    if ( sitem.CurrentRadius > 300 && sitem.CurrentRadius - sitem.LastRadius > 200 && sitem.CurrentRadius < v19 )
    {
        float v9 = (2 * sitem.CurrentRadius) * C_PI / 150.0;

        sitem.LastRadius = sitem.CurrentRadius;

        if ( v9 > 2.0 )
        {
            for (float v25 = 0.0; v25 < 6.283 ; v25 += 6.283 / v9 )
            {
                float v10 = sitem.CurrentRadius;
                float v26 = cos(v25) * v10 + a5;
                float v21 = sin(v25) * v10 + a6;

                if ( v26 > 600.0 && v21 < -600.0 && v26 < map_Width_meters - 600.0 && v21 > -(map_Height_meters - 600.0) )
                {
                    int v12 = fxnumber;

                    fxnumber = 2;

                    yw_arg129 arg129;
                    arg129.pos.x = v26;
                    arg129.pos.y = sitem.PCell->height;
                    arg129.pos.z = v21;
                    arg129.field_10 = 200000;
                    arg129.field_14 = sitem.ActivateOwner;
                    arg129.unit = 0;

                    ypaworld_func129(&arg129);

                    fxnumber = v12;
                }
            }
        }
    }

    ypaworld_func64__sub19__sub2__sub0__sub0(sitem.ActivateOwner, a5, a6, sitem.CurrentRadius);
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub2(int id)
{
    const MapSuperItem &sitem = _levelInfo->SuperItems[id];

    if ( !sub_4D1230(id, sitem.ActivateOwner) && sub_4D12A0(sitem.ActivateOwner) )
    {
        if ( sitem.Type == 1 )
            ypaworld_func64__sub19__sub2__sub0(id);
    }
    else
    {
        sub_4D16C4(id);
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub19()
{
    for (size_t i = 0; i < _levelInfo->SuperItems.size(); i++)
    {
        const MapSuperItem &sitem = _levelInfo->SuperItems[i];

        if ( sitem.Type )
        {
            switch ( sitem.State )
            {
            case 0:
                if ( sub_4D11C0(i, sitem.PCell->owner) )
                {
                    if ( sub_4D12A0(sitem.PCell->owner) )
                        sub_4D12D8(i, 0);
                }
                break;

            case 1:
                ypaworld_func64__sub19__sub0(i);
                ypaworld_func64__sub19__sub3(this, i);
                break;

            case 2:
                ypaworld_func64__sub19__sub1(i);
                break;

            case 3:
                ypaworld_func64__sub19__sub2(i);
                break;

            default:
                break;
            }
        }
    }
}

void sub_4D6958(NC_STACK_ypaworld *yw, NC_STACK_ypabact *unit, samples_collection1 *collection)
{
    if ( unit == yw->UserRobo )
    {
        collection->field_0 = yw->UserUnit->_position;
    }
    else
    {
        vec3d tmp = unit->_position - yw->UserUnit->_position;

        float v11 = tmp.length();

        if ( v11 > 0.0 )
            tmp *= (100.0 / v11);

        collection->field_0 = yw->UserUnit->_position + tmp;
    }
}

void ypaworld_func64__sub23(NC_STACK_ypaworld *yw)
{
    yw_samples *smpls = yw->samples;

    if ( smpls->field_0 >= 0 )
    {
        NC_STACK_ypabact *unit = smpls->field_360;

        if ( unit->_status != BACT_STATUS_DEAD )
        {
            sub_4D6958(yw, unit, &smpls->field_4);

            smpls->field_4.field_C = yw->UserUnit->_fly_dir * yw->UserUnit->_fly_dir_length;
        }

        if ( smpls->field_4.samples_data[0].flags & 2 )
        {
            SFXEngine::SFXe.sb_0x4242e0(&smpls->field_4);
        }
        else
        {
            SFXEngine::SFXe.sub_423DD8(&smpls->field_4);

            if ( smpls->field_35C )
                delete_class_obj(smpls->field_35C);

            memset(smpls, 0, sizeof(yw_samples));
            smpls->field_0 = -1;
        }
    }
}

void ypaworld_func64__sub3(NC_STACK_ypaworld *yw)
{
    if ( yw->UserUnit->_pSector->owner != yw->UserRobo->_owner )
    {
        if ( yw->UserUnit->_pSector->owner )
        {
            if ( yw->field_1b74 == yw->UserRobo->_owner || !yw->field_1b74 )
            {
                if ( yw->timeStamp - yw->field_1a08 > 10000 )
                {
                    yw_arg159 arg159;
                    arg159.unit = yw->UserUnit;
                    arg159.field_4 = 24;
                    arg159.txt = get_lang_string(yw->string_pointers_p2, 222, "ENEMY SECTOR ENTERED");
                    arg159.field_C = 22;

                    yw->ypaworld_func159(&arg159);
                }

                yw->field_1a08 = yw->timeStamp;
            }
        }
    }
}

void sub_44A094(NC_STACK_ypaworld *yw)
{
    yw->p_1_grp_cnt++;

    if ( yw->p_1_grp_cnt >= 5 )
    {
        if ( yw->p_1_grp[0][0] > 200 )
            yw->p_1_grp[0][0] = 0;

        for (int i = 0; i < 8; i++)
        {
            if ( yw->p_1_grp[0][i] )
            {
                if ( yw->p_1_grp[0][i] < yw->p_1_grp[2][i] )
                    yw->p_1_grp[2][i] = yw->p_1_grp[0][i];

                if ( yw->p_1_grp[0][i] > yw->p_1_grp[1][i] )
                    yw->p_1_grp[1][i] = yw->p_1_grp[0][i];

                yw->p_1_grp[3][i] += yw->p_1_grp[0][i];
            }
        }
    }
}

int NC_STACK_ypaworld::yw_RestoreVehicleData()
{
    std::string buf = fmt::sprintf("save:%s/%d.rst", GameShell->user_name, _levelInfo->LevelID);

    ScriptParser::HandlersList parsers {
        new World::Parsers::VhclProtoParser(this),
        new World::Parsers::WeaponProtoParser(this),
        new World::Parsers::BuildProtoParser(this)
    };

    return ScriptParser::ParseFile(buf, parsers, 0);
}

void NC_STACK_ypaworld::sub_471AB8()
{
    if ( _levelInfo->State == 1 )
    {
        MapGate &gate = _levelInfo->Gates[ _levelInfo->GateCompleteID ];

        LevelNet->mapInfos[ _levelInfo->LevelID ].field_0 = 3;

        for (int lvl : gate.PassToLevels)
        {
            if ( LevelNet->mapInfos[ lvl ].field_0 == 1 )
                LevelNet->mapInfos[ lvl ].field_0 = 2;
        }
    }
    else if ( _levelInfo->State == 2 && !yw_RestoreVehicleData() )
    {
        ypa_log_out("yw_RestoreVehicleData() failed.\n");
    }
}

void ypaworld_func151__sub6(NC_STACK_ypaworld *yw)
{
    if ( yw->samples )
    {
        SFXEngine::SFXe.sub_423DD8(&yw->samples->field_4);

        if ( yw->samples->field_35C )
            delete_class_obj(yw->samples->field_35C);

        memset(yw->samples, 0, sizeof(yw_samples));

        yw->samples->field_0 = -1;

        nc_FreeMem(yw->samples);
        yw->samples = NULL;
    }
}

void NC_STACK_ypaworld::NetReleaseMissiles(NC_STACK_ypabact *bact)
{
    while(!bact->_missiles_list.empty())
    {
        NC_STACK_ypamissile *misl = bact->_missiles_list.front();
        bact->_missiles_list.pop_front();
        
        if ( misl->_primTtype == BACT_TGT_TYPE_UNIT )
        {
            misl->_primT.pbact->_attackersList.remove( misl );
            misl->_primTtype = BACT_TGT_TYPE_NONE;
        }

        misl->CleanAttackersTarget();

        misl->_parent = NULL;

        ypaworld_func144(misl);

        misl->_status_flg |= BACT_STFLAG_DEATH1;
    }
}

void NC_STACK_ypaworld::sub_4F1BE8(NC_STACK_ypabact *bct)
{
    if ( bct->_bact_type == BACT_TYPES_GUN )
    {
        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( bct );

        if ( gun->IsRoboGun() )
        {
            roboGun *v4 = bct->_host_station->getROBO_guns();

            for (int i = 0; i < 8; i++)
            {
                if ( bct == v4[i].gun_obj )
                    v4[i].gun_obj = NULL;
            }
        }
    }
}

void NC_STACK_ypaworld::NetRemove(NC_STACK_ypabact *bct)
{
    while(!bct->_kidList.empty())
    {
        NC_STACK_ypabact *cmnder = bct->_kidList.front();

        while ( !cmnder->_kidList.empty() )
        {
            NC_STACK_ypabact *slave = cmnder->_kidList.front();

            NetReleaseMissiles(slave);
            slave->CleanAttackersTarget();
            sub_4F1BE8(slave);

            slave->_status_flg |= BACT_STFLAG_DEATH1;
            slave->_status = BACT_STATUS_DEAD;

            ypaworld_func144(slave);
        }

        NetReleaseMissiles(cmnder);
        cmnder->CleanAttackersTarget();
        sub_4F1BE8(cmnder);

        cmnder->_status_flg |= BACT_STFLAG_DEATH1;
        cmnder->_status = BACT_STATUS_DEAD;

        ypaworld_func144(cmnder);
    }

    if ( bct->_bact_type == BACT_TYPES_ROBO )
    {
        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(bct);

        roboGun *a4 = robo->getROBO_guns();

        for (int i = 0; i < 8; i++)
            a4[i].gun_obj = NULL;
    }

    bct->CleanAttackersTarget();
    NetReleaseMissiles(bct);

    bct->_status = BACT_STATUS_DEAD;

    ypaworld_func144(bct);

    bct->_status_flg |= BACT_STFLAG_DEATH1;
}

void sub_44C144(vhclSndFX *sndfx)
{
    for (int i = 0; i < sndfx->extS.cnt; i++)
    {
        if ( sndfx->wavs[i] )
        {
            delete_class_obj(sndfx->wavs[i]);
            sndfx->wavs[i] = NULL;
        }
    }

    if ( sndfx->single_sample )
    {
        delete_class_obj(sndfx->single_sample);
        sndfx->single_sample = NULL;
    }
}

void ypaworld_func151__sub0(NC_STACK_ypaworld *yw)
{
    SFXEngine::SFXe.sub_424CC8();

    for (int i = 0; i < 256; i++)
    {
        VhclProto *vhcl = &yw->VhclProtos[i];
        for (int j = 0; j < 12; j++)
        {
            sub_44C144(&vhcl->sndFX[j]);
        }
    }

    for (int i = 0; i < 128; i++)
    {
        WeapProto *weap = &yw->WeaponProtos[i];
        for (int j = 0; j < 3; j++)
        {
            sub_44C144(&weap->sndFXes[j]);
        }
    }

    for (int i = 0; i < 128; i++)
    {
        sub_44C144(&yw->BuildProtos[i].SndFX);
    }
}

void ypaworld_func151__sub1(NC_STACK_ypaworld *yw)
{
    if ( yw->_powerStations )
    {
        nc_FreeMem(yw->_powerStations);
        yw->_powerStations = NULL;
        yw->_powerStationsCount = 0;
    }

    if ( yw->field_30 )
    {
        nc_FreeMem(yw->field_30);
        yw->field_30 = NULL;
    }
}

void ypaworld_func151__sub5(NC_STACK_ypaworld *yw)
{
    if ( yw->map_events )
    {
        nc_FreeMem(yw->map_events);
        yw->map_events = NULL;
    }
}

void NC_STACK_ypaworld::FFeedback_Update()
{
    if ( field_739A )
    {
        if ( field_73CE & (4 | 8) )
            return;
    }

    if ( UserUnit )
    {
        if ( timeStamp - field_7562 > 250 )
        {
            field_7562 = timeStamp;

            if ( UserUnit->_status == BACT_STATUS_DEAD )
            {
                if ( input_class )
                    input_class->ForceFeedback(NC_STACK_winp::FF_STATE_UPDATE, field_7566);
            }
            else
            {
                if ( field_7566 != -1 )
                {
                    float a1 = POW2(UserUnit->_force) - POW2(UserUnit->_mass) * 100.0;

                    if (a1 < 0.0)
                        a1 = 0.0;

                    float v17 = fabs(UserUnit->_fly_dir_length) / ( sqrt(a1) / UserUnit->_airconst_static );
                    if ( v17 >= 1.0 )
                        v17 = 1.0;
                    else if (v17 < 0.0)
                        v17 = 0.0;

                    if ( input_class )
                        input_class->ForceFeedback(NC_STACK_winp::FF_STATE_UPDATE, field_7566,
                                                        field_756E, field_756A * v17);
                }
            }
        }

        userdata_sample_info *top = SFXEngine::SFXe.SndGetTopShake();
        if ( top )
        {
            field_7572 = top;
            if ( top->startTime == SFXEngine::SFXe.currentTime )
            {
                float p1 = top->shkMag;
                if ( p1 > 1.0 )
                    p1 = 1.0;

                vec3d tmp = top->parent_sample_collection->field_0 - UserUnit->_position;

                float p2 = top->shakeFX->time;
                float p3 = UserUnit->_rotation.AxisX().dot( tmp );
                float p4 = -UserUnit->_rotation.AxisZ().dot( tmp );

                if ( p2 > 0.0 )
                {
                    if ( input_class )
                        input_class->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_SHAKE,
                                                        p1, p2, p3, p4);
                }
            }
        }
        else
        {
            field_7572 = NULL;
        }
    }
}

int recorder_startrec(NC_STACK_ypaworld *yw)
{
    recorder *rcrd = yw->sceneRecorder;

    rcrd->do_record = 0;
    rcrd->field_40 = 0;
    rcrd->seqn++;
    rcrd->level_id = yw->_levelInfo->LevelID;
    rcrd->frame_id = 0;
    rcrd->time = 0;
    rcrd->bacts_count = 0;
    rcrd->field_34 = 0;
    rcrd->ainf_size = 0;

    FSMgr::FileHandle *fil = uaOpenFile(fmt::sprintf("env:snaps/m%02d%04d.raw", yw->_levelInfo->LevelID, rcrd->seqn), "wb");
    if ( !fil )
    {
        rcrd->mfile = NULL;
        return 0;
    }

    rcrd->mfile = new IFFile(fil, true, true);

    if ( !rcrd->mfile )
    {
        delete fil;
        rcrd->mfile = NULL;
        return 0;
    }

    rcrd->mfile->pushChunk(TAG_SEQN, TAG_FORM, -1);
    rcrd->mfile->pushChunk(0, TAG_SINF, 4);

    rcrd->mfile->writeU16L(rcrd->seqn);
    rcrd->mfile->writeU16L(rcrd->level_id);

    rcrd->mfile->popChunk();

    rcrd->do_record = 1;
    return 1;
}

void recorder_stoprec(NC_STACK_ypaworld *yw)
{
    recorder *rcrd = yw->sceneRecorder;
    rcrd->do_record = 0;

    if ( rcrd->mfile )
    {
        rcrd->mfile->popChunk();

        delete rcrd->mfile;

        rcrd->mfile = NULL;
    }
}

void sb_0x447720(NC_STACK_ypaworld *yw, InputState *inpt)
{
    if ( inpt->KbdLastHit == Input::KC_NUMMUL && (inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_HOLD || yw->easy_cheat_keys) )
    {
        sub_4476AC(yw);

        yw_arg159 info_msg;
        info_msg.txt = "Screenshot saved.";
        info_msg.unit = NULL;
        info_msg.field_4 = 100;
        info_msg.field_C = 0;

        yw->ypaworld_func159(&info_msg);
    }


    if ( yw->do_screenshooting )
    {
        if ( inpt->KbdLastHit == Input::KC_NUMDIV && (inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_HOLD || yw->easy_cheat_keys) )
        {
            yw->do_screenshooting = 0;

            yw_arg159 info_msg;
            info_msg.txt = "Screenshotting: stopped.";
            info_msg.unit = NULL;
            info_msg.field_4 = 100;
            info_msg.field_C = 0;

            yw->ypaworld_func159(&info_msg);
        }

        NC_STACK_win3d *win3d = GFXEngine::GFXe.getC3D();

        win3d->SaveScreenshot( fmt::sprintf("env:snaps/s%d_%04d", yw->screenshot_seq_id, yw->screenshot_seq_frame_id) );
        
        yw->screenshot_seq_frame_id++;
    }
    else if ( inpt->KbdLastHit == Input::KC_NUMDIV && (inpt->ClickInf.flag & 0x100 || yw->easy_cheat_keys) )
    {
        yw->screenshot_seq_frame_id = 0;
        yw->do_screenshooting = 1;
        yw->screenshot_seq_id++;

        yw_arg159 info_msg;
        info_msg.txt = "Screenshotting: started.";
        info_msg.unit = NULL;
        info_msg.field_4 = 100;
        info_msg.field_C = 0;

        yw->ypaworld_func159(&info_msg);
    }

    if ( yw->sceneRecorder->do_record )
    {
        if ( inpt->KbdLastHit == Input::KC_NUMMINUS && (inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_HOLD || yw->easy_cheat_keys) )
        {
            recorder_stoprec(yw);

            yw_arg159 info_msg;
            info_msg.txt = "Replay recordering: stopped.";
            info_msg.unit = NULL;
            info_msg.field_4 = 100;
            info_msg.field_C = 0;

            yw->ypaworld_func159(&info_msg);
        }

    }
    else
    {
        if ( inpt->KbdLastHit == Input::KC_NUMMINUS && (inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_HOLD || yw->easy_cheat_keys) )
        {
            recorder_startrec(yw);

            yw_arg159 info_msg;
            info_msg.txt = "Replay recordering: started.";
            info_msg.unit = NULL;
            info_msg.field_4 = 100;
            info_msg.field_C = 0;

            yw->ypaworld_func159(&info_msg);
        }
    }
}

void recorder_update_time(NC_STACK_ypaworld *yw, int dtime)
{
    yw->sceneRecorder->time += dtime;
    yw->sceneRecorder->field_40 -= dtime;
}


void NC_STACK_ypaworld::recorder_store_bact( recorder *rcrd, World::MissileList &bct_lst)
{
    for( NC_STACK_ypamissile * &bact : bct_lst )
    {
        if ( bact->_gid >= 0xFFFF || bact == UserRobo )
        {
            if ( rcrd->bacts_count < rcrd->max_bacts )
            {
                rcrd->bacts[ rcrd->bacts_count ] = bact;
                rcrd->bacts_count++;
            }

            recorder_store_bact(rcrd, bact->_missiles_list);
            recorder_store_bact(rcrd, bact->_kidList);
        }
    }
}

void NC_STACK_ypaworld::recorder_store_bact( recorder *rcrd, World::RefBactList &bct_lst)
{
    for( NC_STACK_ypabact * &bact : bct_lst )
    {
        if ( bact->_gid >= 0xFFFF || bact == UserRobo )
        {
            if ( rcrd->bacts_count < rcrd->max_bacts )
            {
                rcrd->bacts[ rcrd->bacts_count ] = bact;
                rcrd->bacts_count++;
            }

            recorder_store_bact(rcrd, bact->_missiles_list);
            recorder_store_bact(rcrd, bact->_kidList);
        }
    }
}

int recorder_sort_bact(const void *a1, const void *a2)
{
    return (*(NC_STACK_ypabact **)a1)->_gid - (*(NC_STACK_ypabact **)a2)->_gid;
}

void NC_STACK_ypaworld::recorder_world_to_frame(recorder *rcrd)
{
    rcrd->bacts_count = 0;
    recorder_store_bact(rcrd, _unitsList);

    qsort(rcrd->bacts, rcrd->bacts_count, sizeof(NC_STACK_ypabact *), recorder_sort_bact);

    for (int i = 0; i < rcrd->bacts_count; i++)
    {
        NC_STACK_ypabact *bact = rcrd->bacts[i];

        trec_bct *oinf = &rcrd->oinf[i];

        oinf->bact_id = bact->_gid;
        oinf->pos = bact->_position;

        vec3d euler = bact->_rotation.GetEuler();

        oinf->rot_x = dround(euler.x * 127.0 / C_2PI);
        oinf->rot_y = dround(euler.y * 127.0 / C_2PI);
        oinf->rot_z = dround(euler.z * 127.0 / C_2PI);

        NC_STACK_base *a4 = bact->getBACT_visProto();

        if ( a4 == bact->_vp_normal.base )
        {
            oinf->vp_id = 1;
        }
        else if ( a4 == bact->_vp_fire.base )
        {
            oinf->vp_id = 2;
        }
        else if ( a4 == bact->_vp_wait.base )
        {
            oinf->vp_id = 3;
        }
        else if ( a4 == bact->_vp_dead.base )
        {
            oinf->vp_id = 4;
        }
        else if ( a4 == bact->_vp_megadeth.base )
        {
            oinf->vp_id = 5;
        }
        else if ( a4 == bact->_vp_genesis.base )
        {
            oinf->vp_id = 6;
        }
        else
        {
            oinf->vp_id = 0;
        }

        if (bact->_bact_type == BACT_TYPES_MISSLE)
            oinf->objType = recorder::OBJ_TYPE_MISSILE;
        else
            oinf->objType = recorder::OBJ_TYPE_VEHICLE;

        oinf->vhcl_id = bact->_vehicleID;

        uint16_t *ssnd = &rcrd->sound_status[i * 2];
        ssnd[0] = 0;

        for (int j = 0; j < 16; j++)
        {
            if (bact->_soundcarrier.samples_data[j].flags & 0x92)
                ssnd[0] |= 1 << j;
        }

        ssnd[1] = bact->_soundcarrier.samples_data[0].pitch;
    }
}

void recorder_pack_soundstates(recorder *rcrd)
{
    uint8_t *in = (uint8_t *)rcrd->sound_status;
    int in_pos = 0;

    uint8_t *output = (uint8_t *)rcrd->ainf;
    int out_pos = 0;

    int max_bytes_count = 4 * rcrd->bacts_count;

    while ( in_pos < max_bytes_count )
    {
        if ( in_pos >= max_bytes_count - 1 || in[in_pos] != in[in_pos + 1] )
        {
            int ctrl_byte_pos = out_pos;
            int cnt_bytes = 0;

            while (cnt_bytes < 0x80)
            {
                if ( in_pos >= max_bytes_count )
                    break;
                else if ( in_pos < max_bytes_count - 2 && in[in_pos] == in[in_pos + 1] && in[in_pos] == in[in_pos + 2] )
                    break;

                output[out_pos] = in[in_pos];
                in_pos++;
                out_pos++;

                cnt_bytes++;
            }

            output[ctrl_byte_pos] = cnt_bytes - 1;
        }
        else
        {
            int cnt_bytes = 0;

            uint8_t smplbyte = in[in_pos];
            while ( in_pos < max_bytes_count )
            {
                if ( in[in_pos] != smplbyte )
                    break;

                if ( cnt_bytes >= 0x80 )
                    break;

                in_pos++;
                cnt_bytes++;
            }
            output[out_pos] = 0x101 - cnt_bytes;
            output[out_pos + 1] = smplbyte;
            out_pos += 2;
        }
    }

    rcrd->ainf_size = out_pos;
}

void recorder_unpack_soundstates(recorder *rcrd)
{
    uint8_t *out = (uint8_t *)rcrd->sound_status;
    uint8_t *in = (uint8_t *)rcrd->ainf;
    uint8_t *in_end = ((uint8_t *)rcrd->ainf) + rcrd->ainf_size;

    while ( in < in_end )
    {
        uint8_t bt = *in;
        in++;

        if ( bt > 0x80 )
        {
            for (int i = 0; i < 0x101 - bt; i++)
            {
                *out = *in;
                out++;
            }

            in++;
        }
        else if ( bt < 0x80 )
        {
            bt += 1;

            memcpy(out, in, bt);

            out += bt;
            in += bt;
        }
    }
}

void NC_STACK_ypaworld::recorder_write_frame()
{
    recorder *rcrd = sceneRecorder;

    if ( rcrd->field_40 < 0 )
    {
        recorder_world_to_frame(rcrd);
        rcrd->ctrl_bact_id = UserUnit->_gid;
        recorder_pack_soundstates(rcrd);


        int frame_size = 24;
        int oinf_size = 22 * rcrd->bacts_count;
        int v5 = 16 * rcrd->field_34;

        if ( oinf_size )
        {
            frame_size = oinf_size + 32;

            if ( frame_size & 1 )
                frame_size++;
        }

        if ( rcrd->ainf_size )
        {
            frame_size += rcrd->ainf_size + 8;

            if ( frame_size & 1 )
                frame_size++;
        }

        if ( v5 )
        {
            frame_size += v5 + 8;

            if ( frame_size & 1 )
                frame_size++;
        }
        rcrd->mfile->pushChunk(TAG_FRAM, TAG_FORM, frame_size);
        rcrd->mfile->pushChunk(0, TAG_FINF, 12);

        rcrd->mfile->writeS32L(rcrd->frame_id);
        rcrd->mfile->writeS32L(rcrd->time);
        rcrd->mfile->writeU32L(rcrd->ctrl_bact_id);

        rcrd->mfile->popChunk();

        if ( oinf_size )
        {
            rcrd->mfile->pushChunk(0, TAG_OINF, oinf_size);

            for (int i = 0; i < rcrd->bacts_count; i++)
            {
                trec_bct *oinf = &rcrd->oinf[i];

                rcrd->mfile->writeU32L(oinf->bact_id);
                TFEngine::Vec3dWriteIFF(oinf->pos, rcrd->mfile, false);
                rcrd->mfile->writeS8(oinf->rot_x);
                rcrd->mfile->writeS8(oinf->rot_y);
                rcrd->mfile->writeS8(oinf->rot_z);
                rcrd->mfile->writeU8(oinf->vp_id);
                rcrd->mfile->writeU8(oinf->objType);
                rcrd->mfile->writeU8(oinf->vhcl_id);
            }

            rcrd->mfile->popChunk();
        }

        if ( rcrd->ainf_size )
        {
            rcrd->mfile->pushChunk(0, TAG_AINF, rcrd->ainf_size);
            rcrd->mfile->write(rcrd->ainf, rcrd->ainf_size);
            rcrd->mfile->popChunk();
        }

        if ( v5 )
        {
            rcrd->mfile->pushChunk(0, TAG_MODE, v5);
            rcrd->mfile->write(rcrd->field_20, v5);
            rcrd->mfile->popChunk();
        }

        rcrd->mfile->popChunk();

        rcrd->field_34 = 0;
        rcrd->field_40 += 250;
        rcrd->frame_id += 1;
    }
}


int ypaworld_func64__sub22__sub0(NC_STACK_ypaworld *yw, int event_id)
{
    evnt *ev = &yw->map_events->evnts[event_id];

    if ( !ev->field_8 )
        ev->field_8 = yw->timeStamp;

    if ( ev->field_0 == 1 )
    {
        if ( ev->field_C )
            return 3;
        else
        {
            ev->field_C = 1;

            if ( ev->field_4 )
            {
                if ( ev->field_4(yw) )
                    return 3;
            }
        }

        return 1;
    }
    else if ( ev->field_0 == 5 )
    {
        if ( (yw->timeStamp - ev->field_8) > ev->field_10 )
        {
            if ( ev->field_C )
                return 3;

            ev->field_C = 1;

            if ( ev->field_4 )
            {
                if ( ev->field_4(yw) )
                    return 3;
            }

            return 1;
        }
        return 2;
    }
    else if ( ev->field_0 == 2 )
    {
        if ( (yw->timeStamp - ev->field_8) <= ev->field_10 )
            return 3;

        ev->field_8 = yw->timeStamp;

        if ( ev->field_4 )
        {
            if ( ev->field_4(yw) )
                return 3;
        }
        return 1;
    }
    else if ( ev->field_0 == 4 )
    {
        if ( !ev->field_C )
        {
            ev->field_C = 1;

            if ( ev->field_4 )
            {
                if ( ev->field_4(yw) )
                    return 3;
            }
            return 1;
        }

        if ( (yw->timeStamp - ev->field_8) <= ev->field_10 )
            return 3;

        ev->field_8 = yw->timeStamp;

        if ( ev->field_4 )
        {
            if ( ev->field_4(yw) )
                return 3;
        }

        return 1;
    }
    else if ( ev->field_0 == 3 )
    {
        if ( ev->field_4 )
        {
            if ( ev->field_4(yw) )
                return 3;

            return 1;
        }
        return 3;
    }
    else if ( ev->field_0 == 6 )
    {
        if ( (yw->timeStamp - ev->field_8) <= ev->field_10 )
            return 2;

        if ( ev->field_C )
            return 3;

        ev->field_C = 1;

        if ( ev->field_4 )
            ev->field_4(yw);

        if ( sub_4C885C() != 3 )
        {
            extern int dword_5C8B78;
            extern GuiList exit_menu;

            dword_5C8B78 = 8;
            const char *v14 = get_lang_string(yw->string_pointers_p2, 2470, "2470 == EXIT TUTORIAL MISSION ?");
            yw->sb_0x4c87fc(v14, &exit_menu);
        }
        return 6;
    }

    return 1;
}

void ypaworld_func64__sub22__sub1(NC_STACK_ypaworld *yw, int evnt_id)
{
    map_event *mevent = yw->map_events;
    mevent->field_8 = evnt_id;

    evnt *ev = &mevent->evnts[evnt_id];

    mevent->field_C = yw->timeStamp;

    yw_arg159 arg159;
    arg159.field_4 = 100;
    arg159.unit = NULL;
    arg159.field_C = ev->field_1C[ ev->field_14 ];

    ev->field_14++;

    if ( ev->field_14 >= ev->field_18 )
        ev->field_14 = 0;

    arg159.txt = NULL;

    yw->ypaworld_func159(&arg159);
}

void ypaworld_func64__sub22(NC_STACK_ypaworld *yw)
{
    map_event *mevent = yw->map_events;
    if ( mevent->event_loop_id )
    {
        int v3 = 3;
        int i;

        for (i = 0; i < mevent->event_count; i++ )
        {
            v3 = ypaworld_func64__sub22__sub0(yw, i);

            if ( v3 != 3 )
                break;
        }

        if ( v3 == 1 && i < mevent->event_count )
        {
            if ( i == mevent->field_8 )
            {
                if ( (yw->timeStamp - mevent->field_C) > mevent->evnts[ mevent->field_8 ].field_10 )
                    ypaworld_func64__sub22__sub1(yw, mevent->field_8);
            }
            else
            {
                ypaworld_func64__sub22__sub1(yw, i);
            }

        }
    }
}

int recorder_open_replay(recorder *rcrd)
{
    FSMgr::FileHandle *fil = uaOpenFile(rcrd->filename, "rb");

    if ( !fil )
    {
        rcrd->mfile = NULL;
        return 0;
    }

    rcrd->mfile = new IFFile(fil, false, true);

    if ( !rcrd->mfile )
    {
        delete fil;
        return 0;
    }

    if ( rcrd->mfile->parse() != IFFile::IFF_ERR_OK )
    {
        delete rcrd->mfile;

        rcrd->mfile = NULL;
        return 0;
    }

    IFFile::Context *v3 = rcrd->mfile->getCurrentChunk();

    if ( v3->TAG == TAG_FORM && v3->TAG_EXTENSION == TAG_SEQN )
        return 1;

    return 0;
}


bool NC_STACK_ypaworld::recorder_create_camera()
{
    NC_STACK_ypabact *bacto = Nucleus::CInit<NC_STACK_ypabact>( {{NC_STACK_ypabact::BACT_ATT_WORLD, this}} );

    if ( !bacto )
        return false;

    bacto->Renew();

    bacto->_gid = 0;
    bacto->_owner = 1;

    bacto->_rotation = mat3x3::Ident();

    SFXEngine::SFXe.sub_423DB0(&bacto->_soundcarrier);

    ypaworld_func134(bacto);

    bacto->setBACT_viewer(1);
    bacto->setBACT_inputting(1);

    UserRobo = bacto;
    _UserRoboKidsList = &bacto->_kidList;

    TFEngine::Engine.SetViewPoint(&bacto->_tForm);

    return true;
}



void recorder_read_framedata(recorder *rcrd)
{
    while ( rcrd->mfile->parse() != IFFile::IFF_ERR_EOC )
    {
        IFFile::Context *v3 = rcrd->mfile->getCurrentChunk();

        switch ( v3->TAG )
        {
        case TAG_FLSH:
            rcrd->field_78 |= 1;
            rcrd->mfile->parse();
            break;

        case TAG_FINF:
            rcrd->mfile->readS32L(rcrd->frame_id);
            rcrd->mfile->readS32L(rcrd->time);
            rcrd->mfile->readU32L(rcrd->ctrl_bact_id);
            rcrd->mfile->parse();
            break;

        case TAG_OINF:
        {
            rcrd->bacts_count = v3->TAG_SIZE / 22;

            for (int i = 0; i < rcrd->bacts_count; i++)
            {
                trec_bct *oinf = &rcrd->oinf[i];

                rcrd->mfile->readU32L(oinf->bact_id);
                TFEngine::Vec3dReadIFF(oinf->pos, rcrd->mfile, false);
                rcrd->mfile->readS8(oinf->rot_x);
                rcrd->mfile->readS8(oinf->rot_y);
                rcrd->mfile->readS8(oinf->rot_z);

                uint8_t tmp;
                rcrd->mfile->readU8(tmp);
                oinf->vp_id = tmp;
                rcrd->mfile->readU8(tmp);
                oinf->objType = tmp;
                rcrd->mfile->readU8(tmp);
                oinf->vhcl_id = tmp;
            }

            rcrd->mfile->parse();
        }
        break;

        case TAG_AINF:
            rcrd->mfile->read(rcrd->ainf, v3->TAG_SIZE);
            rcrd->ainf_size = v3->TAG_SIZE;

            recorder_unpack_soundstates(rcrd);

            rcrd->mfile->parse();
            break;

        case TAG_MODE:
            rcrd->mfile->read(rcrd->field_20, v3->TAG_SIZE);
            rcrd->field_34 = v3->TAG_SIZE / 16;

            rcrd->mfile->parse();
            break;

        default:
            rcrd->mfile->skipChunk();
            break;
        }
    }
}

NC_STACK_ypabact *NC_STACK_ypaworld::recorder_newObject(trec_bct *oinf)
{
    NC_STACK_ypabact *bacto = NULL;

    if ( oinf->objType == recorder::OBJ_TYPE_VEHICLE )
    {
        if ( oinf->vhcl_id )
        {
            ypaworld_arg146 arg146;
            arg146.vehicle_id = oinf->vhcl_id;
            arg146.pos = vec3d(0.0, 0.0, 0.0);

            VhclProto *prot = &VhclProtos[ oinf->vhcl_id ];

            int v6 = prot->model_id;

            prot->model_id = 1;

            bacto = ypaworld_func146(&arg146);

            VhclProtos[oinf->vhcl_id].model_id = v6;
        }
        else
        {

            bacto = Nucleus::CInit<NC_STACK_ypabact>( {{NC_STACK_ypabact::BACT_ATT_WORLD, this}} );
            if ( bacto )
            {
                bacto->Renew();

                bacto->_gid = 0;
                bacto->_owner = 1;

                bacto->_rotation = mat3x3::Ident();
            }
        }
    }
    else
    {
        ypaworld_arg146 arg147;
        arg147.vehicle_id = oinf->vhcl_id;
        arg147.pos = vec3d(0.0, 0.0, 0.0);

        bacto = ypaworld_func147(&arg147);
    }

    if ( bacto )
    {
        bacto->_kidRef.Detach();

        bacto->_gid = oinf->bact_id;
        bacto->_host_station = (NC_STACK_yparobo *)UserUnit;
        bacto->_parent = UserUnit;
    }

    return bacto;
}

void NC_STACK_ypaworld::recorder_set_bact_pos(NC_STACK_ypabact *bact, const vec3d &pos)
{
    yw_130arg arg130;
    arg130.pos_x = pos.x;
    arg130.pos_z = pos.z;

    if ( ypaworld_func130(&arg130) )
    {
        if ( bact->_pSector )
            bact->_cellRef.Detach();

        bact->_cellRef = arg130.pcell->unitsList.push_back(bact);

        bact->_pSector = arg130.pcell;
        bact->_old_pos = bact->_position;
        bact->_position = pos;
        bact->_sectX = arg130.sec_x;
        bact->_sectY = arg130.sec_z;
    }
}

void NC_STACK_ypaworld::recorder_updateObject(NC_STACK_ypabact *bact, trec_bct *oinf, uint16_t *ssnd, float a5, float a6)
{
    vec3d bct_pos;
    bct_pos = (oinf->pos - bact->_position) * a5 + bact->_position;

    recorder_set_bact_pos(bact, bct_pos);

    bact->_fly_dir = bact->_position - bact->_old_pos;

    float ln = bact->_fly_dir.length();
    if ( ln > 0.0 )
    {
        bact->_fly_dir /= ln;

        if ( a6 <= 0.0 )
            bact->_fly_dir_length = 0;
        else
            bact->_fly_dir_length = (ln / a6) / 6.0;
    }
    else
    {
        bact->_fly_dir = vec3d::OX(1.0);

        bact->_fly_dir_length = 0;
    }

    mat3x3 tmp = mat3x3::Euler( vec3d(oinf->rot_x, oinf->rot_y, oinf->rot_z) / 127.0 * C_2PI );

    vec3d axisX = (tmp.AxisX() - bact->_rotation.AxisX()) * a5 + bact->_rotation.AxisX();

    if ( axisX.normalise() == 0.0 )
        axisX = vec3d::OX(1.0);

    vec3d axisY = (tmp.AxisY() - bact->_rotation.AxisY()) * a5 + bact->_rotation.AxisY();

    if ( axisY.normalise() == 0.0 )
        axisY = vec3d::OY(1.0);

    vec3d axisZ = (tmp.AxisZ() - bact->_rotation.AxisZ()) * a5 + bact->_rotation.AxisZ();

    if ( axisZ.normalise() == 0.0 )
        axisZ = vec3d::OZ(1.0);

    bact->_rotation = mat3x3::Basis(axisX, axisY, axisZ);

    TFEngine::TForm3D *v43 = NULL;
    NC_STACK_base *v44 = NULL;

    switch ( oinf->vp_id )
    {
    case 1:
        v43 = bact->_vp_normal.trigo;
        v44 = bact->_vp_normal.base;
        break;

    case 2:
        v43 = bact->_vp_fire.trigo;
        v44 = bact->_vp_fire.base;
        break;

    case 3:
        v43 = bact->_vp_wait.trigo;
        v44 = bact->_vp_wait.base;
        break;

    case 4:
        v43 = bact->_vp_dead.trigo;
        v44 = bact->_vp_dead.base;
        break;

    case 5:
        v43 = bact->_vp_megadeth.trigo;
        v44 = bact->_vp_megadeth.base;
        break;

    case 6:
        v43 = bact->_vp_genesis.trigo;
        v44 = bact->_vp_genesis.base;
        break;

    default:
        break;
    }

    if ( v44 && v43 )
    {
        bact->setBACT_visProto(v44);
        bact->setBACT_vpTransform(v43);
    }

    bact->_soundcarrier.samples_data[0].pitch = ssnd[1];

    for(int i = 0; i < 16; i++)
    {
        int v48 = 1 << i;
        if ( v48 & ssnd[0] )
        {
            if ( !(bact->_soundFlags & v48) )
            {
                bact->_soundFlags |= v48;
                SFXEngine::SFXe.startSound(&bact->_soundcarrier, i);
            }
        }
        else
        {
            if ( bact->_soundFlags & v48 )
            {
                bact->_soundFlags &= ~v48;

                if ( bact->_soundcarrier.samples_data[i].flags & 1 )
                    SFXEngine::SFXe.sub_424000(&bact->_soundcarrier, i);
            }
        }
    }
}


void NC_STACK_ypaworld::recorder_updateObjectList(recorder *rcrd, float a5, int period)
{
    float fperiod = period / 1000.0;
    World::RefBactList::iterator it = UserUnit->_kidList.begin();

    int i = 0;

    while ( i < rcrd->bacts_count )
    {
        trec_bct *oinf = &rcrd->oinf[i];
        uint16_t *ssnd = &rcrd->sound_status[2 * i];

        if ( it != UserUnit->_kidList.end() )
        {
            NC_STACK_ypabact *bact = *it;
            
            if ( oinf->bact_id > bact->_gid )
            {
                it++;

                ypaworld_func144(bact);
            }
            else if ( oinf->bact_id < bact->_gid )
            {
                NC_STACK_ypabact *v10 = recorder_newObject(oinf);

                if ( v10 )
                {
                    recorder_updateObject(v10, oinf, ssnd, 1.0, fperiod);
                    
                    v10->_kidRef = UserUnit->_kidList.insert(it, v10);

                    i++;
                }
            }
            else // ==
            {
                recorder_updateObject(bact, oinf, ssnd, a5, fperiod);
                it++;

                i++;
            }
        }
        else
        {
            NC_STACK_ypabact *v13 = recorder_newObject(oinf);

            if ( v13 )
            {
                recorder_updateObject(v13, oinf, ssnd, 1.0, fperiod);

                v13->_kidRef = UserUnit->_kidList.push_back(v13);
                it = v13->_kidRef;
                it++;

                i++;
            }
        }
    }

    while ( it != UserUnit->_kidList.end() )
    {
        NC_STACK_ypabact *bact = *it;
        it++;

        ypaworld_func144(bact);
    }
}

int NC_STACK_ypaworld::recorder_go_to_frame(recorder *rcrd, int wanted_frame_id)
{
    int frame_id = wanted_frame_id;
    int cur_frame_id = 0;

    if ( frame_id >= 0 )
    {
        if ( frame_id >= rcrd->field_74 )
            frame_id = rcrd->field_74 - 1;
    }
    else
    {
        frame_id = 0;
    }

    if ( rcrd->mfile )
    {
        delete rcrd->mfile;
        rcrd->mfile = NULL;
    }

    if ( recorder_open_replay(rcrd) )
    {
        while ( rcrd->mfile->parse() != IFFile::IFF_ERR_EOC )
        {
            IFFile::Context *v7 = rcrd->mfile->getCurrentChunk();

            if ( v7->TAG != TAG_FORM || v7->TAG_EXTENSION != TAG_FRAM )
            {
                rcrd->mfile->skipChunk();
            }
            else
            {
                if ( cur_frame_id == frame_id )
                {
                    recorder_read_framedata(rcrd);

                    timeStamp = rcrd->time;

                    recorder_updateObjectList(rcrd, 1.0, 0);
                    return 1;
                }

                cur_frame_id++;
                rcrd->mfile->skipChunk();
            }
        }
    }
    return 0;
}


void NC_STACK_ypaworld::ypaworld_func163__sub1(recorder *rcrd, int dTime)
{
    if ( dTime )
    {
        rcrd->field_78 &= 0xFFFFFFFE;

        while ( rcrd->field_74 - 1 != rcrd->frame_id  &&  (dTime + timeStamp) > rcrd->time )
        {
            if ( rcrd->mfile->parse() != IFFile::IFF_ERR_EOF )
            {
                IFFile::Context *v5 = rcrd->mfile->getCurrentChunk();

                if ( v5->TAG == TAG_FORM && v5->TAG_EXTENSION == TAG_FRAM )
                    recorder_read_framedata(rcrd);
            }
        }


        if ( rcrd->field_74 - 1 == rcrd->frame_id )
        {
            recorder_go_to_frame(rcrd, 0);
        }
        else
        {
            if ( rcrd->field_78 & 1 )
            {
                timeStamp = rcrd->time;
                recorder_updateObjectList(rcrd, 1.0, dTime);
            }
            else
            {
                float v9 = (float)dTime / (float)(rcrd->time - timeStamp);

                timeStamp += dTime;

                recorder_updateObjectList(rcrd, v9, dTime);
            }
        }
    }
}

void ypaworld_func163__sub2__sub1(NC_STACK_ypaworld *yw, float fperiod, InputState *inpt)
{
    recorder *rcrd = yw->replayer;

    float v20 = rcrd->rotation_matrix.m20;
    float v18 = rcrd->rotation_matrix.m22;

    float v13 = inpt->Sliders[0] * 250.0 * fperiod;
    float v14 = -inpt->Sliders[2] * 250.0 * fperiod;
    float v15 = -inpt->Sliders[1] * 150.0 * fperiod;

    float v17 = sqrt( POW2(v20) + POW2(v18) );
    if ( v17 > 0.0 )
    {
        v20 /= v17;
        v18 /= v17;
    }

    rcrd->field_44.z += v15 * v18;
    rcrd->field_44.x += v15 * v20;

    float v21 = rcrd->rotation_matrix.m00;
    float v19 = rcrd->rotation_matrix.m02;

    float v16 = sqrt( POW2(v21) + POW2(v19) );
    if ( v16 > 0.0 )
    {
        v21 /= v16;
        v19 /= v16;
    }

    rcrd->field_44.y += v14;
    rcrd->field_44.z += v19 * v13;
    rcrd->field_44.x += v21 * v13;
}

void ypaworld_func163__sub2__sub0(NC_STACK_ypaworld *yw, float fperiod, InputState *inpt)
{
    float v3 = inpt->Sliders[10] * 2.5 * fperiod;

    if ( fabs(v3) > 0.001 )
        yw->replayer->rotation_matrix = mat3x3::RotateY(-v3) * yw->replayer->rotation_matrix;

    float v5 = inpt->Sliders[11] * 2.5 * fperiod;

    if ( fabs(v5) > 0.001 )
    {
        yw->replayer->rotation_matrix = mat3x3::RotateX(v5) * yw->replayer->rotation_matrix;
    }
}

void NC_STACK_ypaworld::CameraPrepareRender(recorder *rcrd, NC_STACK_ypabact *bact, InputState *inpt)
{
    extern tehMap robo_map;
    extern squadMan squadron_manager;

    float fperiod = inpt->Period / 1000.0;

    if ( _mouseGrabbed || !(inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_DOWN) )
    {
        if ( _mouseGrabbed )
        {
            if ( inpt->ClickInf.flag & ClickBoxInf::FLAG_RM_DOWN )
                _mouseGrabbed = 0;
        }
    }
    else
    {
        if ( inpt->ClickInf.selected_btn != &robo_map  &&  inpt->ClickInf.selected_btn != &squadron_manager )
            _mouseGrabbed = 1;
    }

    if ( inpt->Buttons.Is(0) )
        rcrd->rotation_matrix = mat3x3::Ident();

    ypaworld_func163__sub2__sub1(this, fperiod, inpt);

    if ( _mouseGrabbed )
        ypaworld_func163__sub2__sub0(this, fperiod, inpt);

    if ( rcrd->field_80 == 16 )
    {
        recorder_set_bact_pos(bact, rcrd->field_44);
        bact->_rotation = rcrd->rotation_matrix;
    }
    else if ( rcrd->field_80 == 18 )
    {
        for ( NC_STACK_ypabact * &unit : *_UserRoboKidsList )
        {
            if ( rcrd->field_84 == unit->_gid )
            {
                vec3d v35 = unit->_position + unit->_rotation.Transpose().Transform(rcrd->field_44);
                recorder_set_bact_pos(bact, v35);

                bact->_rotation = rcrd->rotation_matrix * unit->_rotation;
                break;
            }
        } 
    }
    else if ( rcrd->field_80 == 20 )
    {
        for ( NC_STACK_ypabact * &unit : *_UserRoboKidsList )
        {
            if ( rcrd->ctrl_bact_id == unit->_gid )
            {
                vec3d a3a = unit->_position + unit->_rotation.Transpose().Transform(rcrd->field_44);
                recorder_set_bact_pos(bact, a3a);

                bact->_rotation = rcrd->rotation_matrix * unit->_rotation;
                break;
            }
        }
    }

    bact->_fly_dir = bact->_old_pos - bact->_position;

    float v39 = bact->_fly_dir.length();
    if ( v39 <= 0.0 )
    {
        bact->_fly_dir = vec3d::OX(1.0);
        bact->_fly_dir_length = 0;
    }
    else
    {
        bact->_fly_dir /= v39;

        if ( fperiod <= 0.0 )
            bact->_fly_dir_length = 0;
        else
            bact->_fly_dir_length = (v39 / fperiod) / 6.0;
    }

    bact->_tForm.locPos = bact->_position;
    bact->_tForm.locSclRot = bact->_rotation;
}

char *sub_445654(NC_STACK_ypaworld *yw, char *in, char *buf, const char *fmt, ...)
{
    char *cur = in;

    FontUA::copy_position(&cur);

    va_list va;
    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    va_end(va);

    FontUA::add_txt(&cur, yw->screen_width, 1, buf);

    return cur;
}

void NC_STACK_ypaworld::debug_count_units()
{
    for (int i = 0; i < 8; i++)
    {
        dbg_num_sqd_counter[i] = 0;
        dbg_num_vhcl_counter[i] = 0;
        dbg_num_flk_counter[i] = 0;
        dbg_num_robo_counter[i] = 0;
        dbg_num_wpn_counter[i] = 0;
    }

    dbg_num_sqd = 0;
    dbg_num_vhcl = 0;
    dbg_num_flk = 0;
    dbg_num_wpn = 0;
    dbg_num_robo = 0;

    for ( NC_STACK_ypabact * &robo : _unitsList )
    {
        dbg_num_robo_counter[ robo->_owner ]++;

        if ( robo->_owner )
        {
            for ( NC_STACK_ypabact * &commander : robo->_kidList )
            {
                bool v5 = false;

                if ( commander->_bact_type == BACT_TYPES_GUN )
                {
                    v5 = true;
                    dbg_num_flk_counter[ commander->_owner ]++;
                }
                else
                {
                    dbg_num_sqd_counter[ commander->_owner ]++;
                    dbg_num_vhcl_counter[ commander->_owner ]++;
                }

                dbg_num_wpn_counter[ commander->_owner ] += commander->_missiles_list.size();

                for ( NC_STACK_ypabact * &unit : commander->_kidList )
                {
                    if ( v5 )
                        dbg_num_flk_counter[ unit->_owner ]++;
                    else
                        dbg_num_vhcl_counter[ commander->_owner ]++;


                    dbg_num_wpn_counter[ commander->_owner ] += unit->_missiles_list.size();
                }
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        dbg_num_sqd  += dbg_num_sqd_counter[i];
        dbg_num_vhcl += dbg_num_vhcl_counter[i];
        dbg_num_flk  += dbg_num_flk_counter[i];
        dbg_num_wpn  += dbg_num_wpn_counter[i];
        dbg_num_robo += dbg_num_robo_counter[i];
    }

    if ( dbg_num_sqd > dbg_num_sqd_max )
        dbg_num_sqd_max = dbg_num_sqd;

    if ( dbg_num_vhcl > dbg_num_vhcl_max )
        dbg_num_vhcl_max = dbg_num_vhcl;

    if ( dbg_num_flk > dbg_num_flk_max )
        dbg_num_flk_max = dbg_num_flk;

    if ( dbg_num_wpn > dbg_num_wpn_max )
        dbg_num_wpn_max = dbg_num_wpn;

    if ( dbg_num_robo > dbg_num_robo_max )
        dbg_num_robo_max = dbg_num_robo;
}

void NC_STACK_ypaworld::debug_info_draw(InputState *inpt)
{
    if ( field_1b68 == 0)
    {
        if ( sub_449678(inpt, Input::KC_F9) )
            field_1b68++;
    }
    else
    {

        char dbg_txt[4096];
        char buf_sprintf[2048];

        char *cmd = dbg_txt;

        FontUA::select_tileset(&cmd, 15);
        FontUA::set_xpos(&cmd, 8);
        FontUA::set_ypos(&cmd, 16);

        int v104 = 0;

        if ( field_1b68 == 1 )
        {
            debug_count_units();

            if ( !buildDate.empty() )
            {
                cmd = sub_445654(this, cmd, buf_sprintf, "build id: %s", buildDate.c_str());

                FontUA::next_line(&cmd);
            }

            int this_time = timeStamp / 1024;
            int all_time;

            if ( isNetGame )
                all_time = 0;
            else
                all_time = (timeStamp + playerstatus[1].elapsedTime) / 1024;

            cmd = sub_445654(
                      this,
                      cmd,
                      buf_sprintf,
                      "time: (this: %02d:%02d:%02d) (all: %02d:%02d:%02d)",
                      this_time / 60 / 60,
                      this_time / 60 % 60,
                      this_time % 60,
                      all_time / 60 / 60,
                      all_time / 60 % 60,
                      all_time % 60 );

            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof all: %d", p_1_grp[0][1]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof fprint: %d", p_1_grp[0][2]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof gui: %d", p_1_grp[0][3]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof ai: %d", p_1_grp[0][4]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof rend: %d", p_1_grp[0][5]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof net: %d", p_1_grp[0][6]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "sec type/wtype: %d/%d", UserUnit->_pSector->type_id, UserUnit->_pSector->w_type);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "beam energy: %d", beamenergy);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num sqd: %d,%d", dbg_num_sqd, dbg_num_sqd_max);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num vhcl: %d,%d", dbg_num_vhcl, dbg_num_vhcl_max);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num flk: %d,%d", dbg_num_flk, dbg_num_flk_max);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num robo: %d,%d", dbg_num_robo, dbg_num_robo_max);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num wpn: %d,%d", dbg_num_wpn, dbg_num_wpn_max);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "reload const: %d", UserRobo->_reload_const);
            FontUA::next_line(&cmd);

            cmd = sub_445654(
                      this,
                      cmd,
                      buf_sprintf,
                      "num all vhcl: %d,%d,%d,%d,%d,%d,%d,%d",
                      field_1bac[0],
                      field_1bac[1],
                      field_1bac[2],
                      field_1bac[3],
                      field_1bac[4],
                      field_1bac[5],
                      field_1bac[6],
                      field_1bac[7]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(
                      this,
                      cmd,
                      buf_sprintf,
                      "rld ratio: %8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f",
                      field_1bec[0],
                      field_1bec[1],
                      field_1bec[2],
                      field_1bec[3],
                      field_1bec[4],
                      field_1bec[5],
                      field_1bec[6],
                      field_1bec[7]);
            FontUA::next_line(&cmd);

            if ( field_1a20 )
                cmd = sub_445654(this, cmd, buf_sprintf, "invulnerable: %s", "YES");
            else
                cmd = sub_445654(this, cmd, buf_sprintf, "invulnerable: %s", "NO");
            FontUA::next_line(&cmd);
        }
        else if ( field_1b68 == 2 )
        {
            if ( GameShell && isNetGame )
            {
                FontUA::next_line(&cmd);
                FontUA::next_line(&cmd);

                for (int i = 0; i < 8; i++)
                {
                    if ( GameShell->players[i].status )
                    {
                        const char *v35;
                        const char *v36;

                        switch ( i )
                        {
                        case 1:
                            v35 = "Resistance";
                            break;

                        case 3:
                            v35 = "Mykonier  ";
                            break;

                        case 4:
                            v35 = "Taerkasten";
                            break;

                        case 6:
                            v35 = "Ghorkov   ";
                            break;

                        default:
                            v35 = "Hae?!     ";
                            break;
                        }

                        switch ( GameShell->players[i].status )
                        {
                        case 1:
                            v36 = "OK";
                            break;

                        case 2:
                            v36 = "makes trouble";
                            break;

                        case 3:
                            v36 = "left the game";
                            break;

                        case 4:
                            v36 = "Removed";
                            break;

                        default:
                            v36 = "Hae?!     ";
                            break;
                        }

                        cmd = sub_445654(this, cmd, buf_sprintf, "%s status: %s latency: %d", v35, v36, GameShell->players[i].latency);

                        FontUA::next_line(&cmd);
                    }
                }

                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "net send: %d bytes/sec", GameShell->netsend_speed);
                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "net rcv: %d bytes/sec", GameShell->netrecv_speed);
                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "packet: %d bytes", GameShell->net_packet_size);
                FontUA::next_line(&cmd);

                if ( netInfoOverkill )
                    cmd = sub_445654(this, cmd, buf_sprintf, "WARNING: INFO OVERKILL");

                FontUA::next_line(&cmd);

                if ( windp )
                {
                    int v100[7];
                    windp->GetStats(v100);

                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "thread send list now: %d", v100[0]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "thread recv list now: %d", v100[1]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "thread send list max: %d", v100[3]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "thread recv list max: %d", v100[2]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "send call now: %d", v100[4]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "send call max: %d", v100[5]);
                    FontUA::next_line(&cmd);

                    cmd = sub_445654(this, cmd, buf_sprintf, "send bugs: %d", v100[6]);
                    FontUA::next_line(&cmd);
                }
            }
            else
            {
                cmd = sub_445654(this, cmd, buf_sprintf, "not a network game");
                FontUA::next_line(&cmd);
            }
        }
        else if ( field_1b68 == 3 )
        {
            for (int i = 0; i < 17; i++)
            {
                cmd = sub_445654(this, cmd, buf_sprintf, "slider[%d] = %f", i, inpt->Sliders[i]);
                FontUA::next_line(&cmd);
            }

            std::string buf;

            for (int i = 0; i < 32; i++)
            {
                if ( inpt->Buttons.Is(i) )
                    buf += 'O';
                else
                    buf += '_';
            }

            cmd = sub_445654(this, cmd, buf_sprintf, buf.c_str());
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "keycode = %d", inpt->KbdLastDown);
            FontUA::next_line(&cmd);
        }
        else
        {
            int v109 = 0;
            int v110 = 0;

            for ( NC_STACK_ypabact * &bact : _unitsList )
            {
                if (v109)
                    break;

                if ( bact->_bact_type == BACT_TYPES_ROBO && bact->_owner != 1 ) // FIXME owner
                {
                    v110++;

                    if ( field_1b68 - 3 <= v110 )
                    {
                        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(bact);

                        v109 = 1;

                        cmd = sub_445654(this, cmd, buf_sprintf, "robo owner %d with energy %d / %d / %d / %d", robo->_owner, robo->_energy, robo->_roboBuildSpare, robo->_roboVehicleSpare, robo->_energy_max);
                        FontUA::next_line(&cmd);

                        const char *v71;
                        const char *v73;

                        switch (robo->_roboBuildingDuty)
                        {
                        case 0x10:
                            v71 = "radar";
                            break;

                        case 0x20:
                            v71 = "powerstation";
                            break;

                        case 0x100:
                            v71 = "flak";
                            break;

                        case 0x20000:
                            v71 = "location";
                            break;

                        default:
                            v71 = "nothing";
                            break;
                        }

                        switch (robo->_roboVehicleDuty)
                        {
                        case 0:
                            v73 = "nothing";
                            break;

                        case 0x40:
                            v73 = "conquer";
                            break;

                        case 0x80:
                            v73 = "defense";
                            break;

                        case 0x80000:
                            v73 = "recon";
                            break;

                        case 0x200000:
                            v73 = "robo";
                            break;

                        default:
                            v73 = "powerstation";
                            break;
                        }

                        cmd = sub_445654(this, cmd, buf_sprintf, "    do build job   >%s<   and vhcl job   >%s<", v71, v73);
                        FontUA::next_line(&cmd);

                        cmd = sub_445654(this, cmd, buf_sprintf, "    wait power %d, radar %d, flak %d, location %d",
                                         robo->_roboPowerDelay / 1000,
                                         robo->_roboRadarDelay / 1000,
                                         robo->_roboSafetyDelay / 1000,
                                         robo->_roboPositionDelay / 1000);

                        FontUA::next_line(&cmd);

                        cmd = sub_445654(this, cmd, buf_sprintf, "    wait conquer %d, defense %d, recon %d, robo %d",
                                         robo->_roboConqDelay / 1000,
                                         robo->_roboEnemyDelay / 1000,
                                         robo->_roboExploreDelay / 1000,
                                         robo->_roboDangerDelay / 1000);
                        FontUA::next_line(&cmd);

                        cmd = sub_445654(this, cmd, buf_sprintf, "    values  ");
                        FontUA::next_line(&cmd);

                        if ( robo->_roboPowerDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "power -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "power %d, ", robo->_roboPowerValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboRadarDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "radar -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "radar %d, ", robo->_roboRadarValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboSafetyDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "flak -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "flak %d, ", robo->_roboSafetyValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboPositionDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "power -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "power %d, ", robo->_roboPositionValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboEnemyDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "defense -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "defense %d, ", robo->_roboEnemyValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboConqDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "conquer -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "conquer %d, ", robo->_roboConqValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboExploreDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "recon -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "recon %d, ", robo->_roboExploreValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboDangerDelay > 0 )
                            cmd = sub_445654(this, cmd, buf_sprintf, "robo -1, ");
                        else
                            cmd = sub_445654(this, cmd, buf_sprintf, "robo %d, ", robo->_roboDangerValue);

                        FontUA::next_line(&cmd);

                        if ( robo->_roboState & NC_STACK_yparobo::ROBOSTATE_DOCKINUSE )
                            cmd = sub_445654(this, cmd, buf_sprintf, "dock energy %d time %d", robo->_roboDockEnerg, robo->_roboDockTime);
                    }
                }
            }

            if ( !v109 )
                v104 = 1;
        }


        FontUA::next_line(&cmd);

        cmd = sub_445654(this, cmd, buf_sprintf, "fps: %d", field_1B6E);
        FontUA::next_line(&cmd);

        cmd = sub_445654(this, cmd, buf_sprintf, "polys: %d,%d", field_1B6A, field_1b6c);
        FontUA::next_line(&cmd);

        FontUA::set_end(&cmd);

        w3d_a209 arg209;
        arg209.cmdbuf = dbg_txt;
        arg209.includ = 0;

        _win3d->raster_func209(&arg209);


        if ( v104 )
        {
            field_1b68 = 0;
        }
        else
        {
            if ( sub_449678(inpt, Input::KC_F9) )
                field_1b68++;
        }
    }
}





//Scene events funcs

int NC_STACK_ypaworld::sub_4D51A4(NC_STACK_ypaworld *yw)
{
    for ( NC_STACK_ypabact * &unit : yw->_unitsList )
    {
        if ( unit != yw->UserRobo && unit->_bact_type == BACT_TYPES_ROBO && unit->_status != BACT_STATUS_DEAD )
            return (yw->UserRobo != yw->UserUnit && !yw->field_1b70) || yw->_cmdrsCount;
    }

    return 1;
}

int NC_STACK_ypaworld::sub_4D5218(NC_STACK_ypaworld *yw)
{
    for ( NC_STACK_ypabact * &unit : yw->_unitsList )
    {
        if ( unit != yw->UserRobo && unit->_bact_type == BACT_TYPES_ROBO && unit->_status != BACT_STATUS_DEAD )
            return (yw->UserRobo != yw->UserUnit && !yw->field_1b70) || yw->_cmdrsCount <= 0;
    }

    return 1;
}

int NC_STACK_ypaworld::sub_4D5160(NC_STACK_ypaworld *yw)
{
    for ( NC_STACK_ypabact * &unit : yw->_unitsList )
    {
        if ( unit != yw->UserRobo && unit->_bact_type == BACT_TYPES_ROBO && unit->_status != BACT_STATUS_DEAD )
            return 0;
    }

    return 1;
}

int NC_STACK_ypaworld::sub_4D528C(NC_STACK_ypaworld *yw)
{
    extern tehMap robo_map;

    for ( NC_STACK_ypabact * &unit : yw->_unitsList )
    {
        if ( unit != yw->UserRobo && unit->_bact_type == BACT_TYPES_ROBO && unit->_status != BACT_STATUS_DEAD )
            return (yw->UserRobo != yw->UserUnit && !yw->field_1b70) || robo_map.IsOpen();
    }

    return 1;
}

int NC_STACK_ypaworld::sub_4D5300(NC_STACK_ypaworld *yw)
{
    if ( !yw->_powerStationsCount )
        return 0;

    for (size_t i = 0; i < yw->_powerStationsCount; i++)
    {
        if ( yw->_powerStations[i].p_cell )
        {
            if ( yw->_powerStations[i].p_cell->owner == yw->UserRobo->_owner )
            {
                return 1;
            }
        }
    }

    return 0;
}

int NC_STACK_ypaworld::sub_4D5348(NC_STACK_ypaworld *yw)
{
    return yw->UserRobo->_pSector->w_type == 2;
}

int NC_STACK_ypaworld::sub_4D5360(NC_STACK_ypaworld *yw)
{
    for (const MapGate &gate : yw->_levelInfo->Gates)
    {
        if ( gate.PCell->w_type == 6 )
            return 1;
    }

    return 0;
}

void NC_STACK_ypaworld::HistoryAktCreate(NC_STACK_ypabact *bact)
{
    HistoryEventAdd( World::History::VhclCreate(bact->_owner, bact->_vehicleID, bact->_position.x * 256.0 / bact->_wrldX, bact->_position.z * 256.0 / bact->_wrldY) );
}

void NC_STACK_ypaworld::HistoryAktKill(NC_STACK_ypabact *bact)
{
    if ( bact->_killer )
    {
        uint8_t owners = (bact->_killer->_owner << 3) | bact->_owner;

        if ( bact->_killer->getBACT_viewer() || (bact->_killer->_status_flg & BACT_STFLAG_ISVIEW) )
            owners |= 0x80;

        if ( bact->getBACT_viewer() || (bact->_status_flg & BACT_STFLAG_ISVIEW) )
            owners |= 0x40;

        uint16_t vp = bact->_vehicleID;

        if ( bact->_bact_type == BACT_TYPES_ROBO )
            vp |= 0x8000;

        HistoryEventAdd( World::History::VhclKill(owners, vp, bact->_position.x * 256.0 / bact->_wrldX, bact->_position.z * 256.0 / bact->_wrldY) );
    }
}
