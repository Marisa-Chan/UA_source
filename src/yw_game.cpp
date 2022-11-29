#include <inttypes.h>
#include <string.h>
#include <math.h>
#include "env.h"
#include "includes.h"
#include "yw_internal.h"
#include "yw.h"
#include "yw_net.h"
#include "input.h"
#include "windp.h"
#include "loaders.h"

#include "yparobo.h"
#include "font.h"
#include "gui/uacommon.h"
#include "system/inivals.h"

extern uint32_t bact_id;


NC_STACK_bitmap * loadDisk_screen(NC_STACK_ypaworld *yw)
{
    SFXEngine::SFXe.StopMusicTrack();

    const char *v3;

    if ( yw->_screenSize.x <= 360 )
        v3 = "disk320.ilbm";
    else if ( yw->_screenSize.x > 600 )
        v3 = "disk640.ilbm";
    else
        v3 = "disk512.ilbm";

    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:mc2res");

    NC_STACK_bitmap *disk = Utils::ProxyLoadImage({
        {NC_STACK_rsrc::RSRC_ATT_NAME, std::string(v3)},
        {NC_STACK_bitmap::BMD_ATT_CONVCOLOR, (int32_t)1}});

    Common::Env.SetPrefix("rsrc", oldRsrc);

    return disk;
}

void draw_splashScreen(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen)
{
    if ( splashScreen )
    {
        GFX::rstr_arg204 a4;

        a4.pbitm = splashScreen->GetBitmap();

        a4.float4  = Common::FRect(-1.0, -1.0, 1.0, 1.0);
        a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

        GFX::displ_arg263 v4;
        if (yw->_mousePointers[5])
            v4.bitm = yw->_mousePointers[5]->GetBitmap();
        v4.pointer_id = 6;

        GFX::Engine.SetCursor(v4.pointer_id, 0);

        GFX::Engine.BeginFrame();
        GFX::Engine.raster_func202(&a4);
        GFX::Engine.EndFrame();

        GFX::Engine.BeginFrame();
        GFX::Engine.raster_func202(&a4);
        GFX::Engine.EndFrame();
    }
}

void drawSplashScreenWithTOD(NC_STACK_ypaworld *yw, NC_STACK_bitmap *splashScreen, const std::string &text)
{
    if ( splashScreen )
    {
        GFX::rstr_arg204 a4;

        a4.pbitm = splashScreen->GetBitmap();

        a4.float4  = Common::FRect(-1.0, -1.0, 1.0, 1.0);
        a4.float14 = Common::FRect(-1.0, -1.0, 1.0, 1.0);

        GFX::displ_arg263 v4;
        if (yw->_mousePointers[5])
            v4.bitm = yw->_mousePointers[5]->GetBitmap();
        v4.pointer_id = 6;

        GFX::Engine.SetCursor(v4.pointer_id, 0);

        GFX::Engine.BeginFrame();
        GFX::Engine.raster_func202(&a4);
        splashScreen_OutText(yw, text, yw->_screenSize.x / 7, yw->_screenSize.y / 5);
        GFX::Engine.EndFrame();

        GFX::Engine.BeginFrame();
        GFX::Engine.raster_func202(&a4);
        splashScreen_OutText(yw, text, yw->_screenSize.x / 7, yw->_screenSize.y / 5);
        GFX::Engine.EndFrame();
    }
}

void NC_STACK_ypaworld::PowerStationErase(cellArea *cell)
{
    if (!cell) return;
    
    auto it = _powerStations.find(cell->Id);
    if (it == _powerStations.end())
    {
        /* Oh no, power station info for this sector does not exist.
           So just clear sector purpose field if it was PowerStation*/
        if (cell->PurposeType == cellArea::PT_POWERSTATION)
        {
            cell->PurposeIndex = 0;
            cell->PurposeType = cellArea::PT_NONE;
        }
        
        return;
    }
    
    _powerStations.erase(it);
    
    cell->PurposeIndex = 0;
    cell->PurposeType = cellArea::PT_NONE;
    
    _lvlBuildingsMap(cell->CellId) = 0;
}

void sb_0x44ca90__sub2(NC_STACK_ypaworld *yw, TLevelDescription *mapp)
{
    if (!mapp->Palettes.empty())
    {
        if (!mapp->Palettes[0].empty())
        {
            NC_STACK_bitmap *ilbm = Utils::ProxyLoadImage({
                {NC_STACK_rsrc::RSRC_ATT_NAME, mapp->Palettes[0]},
                {NC_STACK_bitmap::BMD_ATT_HAS_COLORMAP, (int32_t)1}});

            if (ilbm)
            {
                GFX::Engine.SetPalette(*ilbm->getBMD_palette());
                ilbm->Delete();
            }
            else
            {
                ypa_log_out("WARNING: slot #%d [%s] init failed!\n", 0, mapp->Palettes[0].c_str());
            }
        }
    }
}

int NC_STACK_ypaworld::LevelCommonLoader(TLevelDescription *mapp, int levelID, int a5)
{
    int ok = 0;

    *mapp = TLevelDescription();

    _gameplayStats.fill( World::TPlayerStatus() );

    _timeStamp = 0;
    _msgTimestampHSReturn = 0;
    _msgTimestampEnemySector = 0;
    _msgTimestampGates = 0;
    _msgTimestampPSUnderAtk = 0;
    _framesElapsed = 0;

    _levelInfo.LevelID = levelID;
    _levelInfo.Mode = a5;
    _levelInfo.State = TLevelInfo::STATE_PLAYING;
    _levelInfo.OwnerMask = 0;
    _levelInfo.UserMask = 0;

    _cellOnMouse = 0;
    _bactOnMouse = NULL;
    _bactPrevClicked = 0;
    _viewerBact = NULL;
    _userRobo = NULL;
    _userUnit = NULL;
    _makingWaypointsMode = false;
    _gamePaused = false;
    _gamePausedTimeStamp = 0;
    _joyIgnoreX = 1;
    _joyIgnoreY = 1;
    _joyIgnoreZ = 0;
    _helpURL.clear();
    _prevUnitId = 0;
    _fireBtnIsDown = false;
    _fireBtnDownHappen = false;
    _playerHSDestroyed = false;
    _vehicleSectorRatio = 0;
    _beamEnergyCurrent = 0;
    _invulnerable = 0;

    _levelInfo.Gates.clear();
    _levelInfo.SuperItems.clear();

    _techUpgrades.clear();
    
    _netEvent = TNetGameEvent();
    _countUnitsPerOwner.fill(0);

    _dbgTotalSquadCountMax = 0;
    _dbgTotalVehicleCountMax = 0;
    _dbgTotalFlakCountMax = 0;
    _dbgTotalWeaponCountMax = 0;
    _dbgTotalRoboCountMax = 0;
    
    _playerOwner = 0;

    if ( _gfxMode != GFX::Engine.GetGfxMode() || _GameShell->IsWindowedFlag() != GFX::Engine.GetGfxMode().windowed )
    {
        GFX::Engine.SetResolution(_gfxMode, _GameShell->IsWindowedFlag());

        _screenSize = GFX::Engine.GetScreenSize();

        GFX::Engine.setWDD_cursor( (_preferences & World::PREF_SOFTMOUSE) != 0 );

        if ( _screenSize.x >= 512 )
        {
            GFX::Engine.LoadFontByDescr( Locale::Text::Font() );
            Gui::UA::LoadFont( Locale::Text::Font() );
        }
        else
        {
            GFX::Engine.LoadFontByDescr( Locale::Text::SmallFont() );
            Gui::UA::LoadFont( Locale::Text::SmallFont() );
        }
    }

    NC_STACK_bitmap *diskScreenImage = loadDisk_screen(this);

    if ( diskScreenImage )
        draw_splashScreen(this, diskScreenImage);


    std::string oldRsrc = Common::Env.SetPrefix("rsrc", "data:fonts");

    int v19 = load_fonts_and_icons();

    Common::Env.SetPrefix("rsrc", oldRsrc);

    if ( !v19 )
        return 0;

    int tod = loadTOD(this, "tod.def");

    int next_tod = tod + 1;
    
    if ( next_tod + 2490 > 2512 )
        next_tod = 0;

    writeTOD(this, "tod.def", next_tod);

    if ( diskScreenImage )
    {
        drawSplashScreenWithTOD(this, diskScreenImage, Locale::Text::ToD(tod, " "));
        diskScreenImage->Delete();
    }

    _profileFramesCount = 0;
    for (int i = 0; i < PFID_MAX; i++)
    {
        _profileVals[i] = 0;
        _profileMax[i] = 0;
        _profileMin[i] = 100000;
        _profileTotal[i] = 0;
    }

    _history.Clear();
    _historyLastIsTimeStamp = false;

    audio_volume = SFXEngine::SFXe.getMasterVolume();

    _voiceMessage.Reset();

    Common::Env.SetPrefix("rsrc", "data:");

    if ( sub_4DA41C(mapp, _globalMapRegions.MapRegions[_levelInfo.LevelID].MapDirectory) && mapp->IsOk() )
    {       
        Common::DeleteAndNull(&_script);
        
        if (mapp->EventLoopID >= 1 && mapp->EventLoopID <= 3)
        {
            _script = new World::LuaEvents(this);
            _script->LoadFile(fmt::sprintf("lesson%d.lua", mapp->EventLoopID));
            _script->CallInit(_timeStamp);
        }
        else if (!_luaScriptName.empty())
        {
            _script = new World::LuaEvents(this);
            _script->LoadFile(_luaScriptName);
            _script->CallInit(_timeStamp);
        }

        _energyAccumMap.Clear();
        _nextPSForUpdate = 0;

        _inBuildProcess.clear();

        Common::Env.SetPrefix("rsrc", fmt::sprintf("data:set%d", mapp->SetID));

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
    if ( _lvlTypeMap.IsNull() )
        _lvlTypeMap = World::LoadMapDataFromImage(mapName);
    
    if ( _lvlTypeMap.IsNull() )
        return false;

    SetMapSize(_lvlTypeMap.Size());

    int32_t id = 0;
    for(int y = 0; y < _mapSize.y; y++)
    {
        for (int x = 0; x < _mapSize.x; x++)
        {
            cellArea &cell = _cells(x, y);
            
            TSectorDesc *sectp = &_secTypeArray[ _lvlTypeMap( cell.CellId ) ];

            cell.type_id = _lvlTypeMap( cell.CellId );
            cell.SectorType = sectp->SectorType;
            cell.energy_power = 0;

            if ( sectp->SectorType == 1)
            {
                cell.buildings_health.At(0, 0) = sectp->SubSectors.At(0, 0)->StartHealth;
            }
            else
            {
                for (int bldY = 0; bldY < 3; bldY++)
                    for (int bldX = 0; bldX < 3; bldX++)
                        cell.buildings_health.At(bldX, bldY) = sectp->SubSectors.At(bldX, bldY)->StartHealth;
            }
            
            id++;
        }
    }
    return true;
}

bool NC_STACK_ypaworld::LoadOwnerMap(const std::string &mapName)
{
    _countSectorsPerOwner.fill(0);

    if ( _lvlOwnMap.IsNull() )
        _lvlOwnMap = World::LoadMapDataFromImage(mapName);
    
    if ( _lvlOwnMap.IsNull() )
        return false;

    if ( _lvlOwnMap.Size() != _mapSize )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), _lvlOwnMap.Width(), _lvlOwnMap.Height(), _mapSize.x, _mapSize.y);
        _lvlOwnMap.Clear();
        return false;
    }


    for (uint32_t yy = 0; yy < _lvlOwnMap.Height(); yy++)
    {
        for (uint32_t xx = 0; xx < _lvlOwnMap.Width(); xx++)
        {
            Common::Point cellId(xx, yy);
            if ( IsGamePlaySector( cellId ) )
            {
                _cells(cellId).owner = _lvlOwnMap(cellId);
                _countSectorsPerOwner[ _lvlOwnMap(cellId) ]++;
            }
            else
            {
                _cells(cellId).owner = 0;
                _countSectorsPerOwner[ 0 ]++;
            }
        }
    }

    return true;
}

bool NC_STACK_ypaworld::LoadHightMap(const std::string &mapName)
{
    if ( _lvlHeightMap.IsNull() )
        _lvlHeightMap = World::LoadMapDataFromImage(mapName);
    
    if ( _lvlHeightMap.IsNull() )
        return false;

    if ( _lvlHeightMap.Size() != _mapSize )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), _lvlHeightMap.Width(), _lvlHeightMap.Height(), _mapSize.x, _mapSize.y);
        _lvlHeightMap.Clear();
        return false;
    }

    for (int y = 0; y < _mapSize.y; y++)
    {
        for (int x = 0; x < _mapSize.x; x++)
        {
            cellArea &cell = _cells(x, y);
            cell.height = (-100.0) * _lvlHeightMap( cell.CellId );
            cell.CenterPos = World::SectorIDToCenterPos3( cell.CellId );
            cell.CenterPos.y = cell.height;
        }
    }

    for (int y = 1; y < _mapSize.y; y++)
    {
        for (int x = 1; x < _mapSize.x; x++)
        {
            _cells(x, y).averg_height = (_cells(x    ,     y).height +
                                         _cells(x - 1,     y).height +
                                         _cells(x - 1, y - 1).height +
                                         _cells(x    , y - 1).height ) / 4.0;
        }
    }

    return true;
}

bool NC_STACK_ypaworld::yw_createRobos(const std::vector<MapRobo> &Robos)
{
    if ( _levelInfo.Mode != 1 )
    {
        _levelInfo.OwnerMask = 0;
        _levelInfo.UserMask = 2;
        
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

                _levelInfo.OwnerMask |= 1 << roboInf.Owner;

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
                    robo->setBACT_viewer(true);
                    robo->setBACT_inputting(true);
                }
            }
            
            first = false;
        }
    }
    return true;
}

bool NC_STACK_ypaworld::LoadBlgMap(const std::string &mapName)
{
    if ( _lvlBuildingsMap.IsNull() )
        _lvlBuildingsMap = World::LoadMapDataFromImage(mapName);
    
    if ( _lvlBuildingsMap.IsNull() )
        return false;

    if ( _lvlBuildingsMap.Size() != _mapSize )
    {
        ypa_log_out("Mapsize mismatch %s: is [%d,%d], should be [%d,%d].\n", mapName.c_str(), _lvlBuildingsMap.Width(), _lvlBuildingsMap.Height(), _mapSize.x, _mapSize.y);
        _lvlBuildingsMap.Clear();
        return false;
    }

    for ( int y = 0; y < _mapSize.y; y++)
    {
        for ( int x = 0; x < _mapSize.x; x++)
        {
            int blg = _lvlBuildingsMap(x, y);
            cellArea &cell = _cells(x, y);
            
            if (blg && cell.owner)
            {
                ypaworld_arg148 arg148;

                arg148.owner = cell.owner;
                arg148.blg_ID = blg;
                arg148.field_C = 1;
                arg148.field_18 = 0;
                arg148.CellId = cell.CellId;

                ypaworld_func148(&arg148);
            }
        }
    }

    return true;
}

void NC_STACK_ypaworld::yw_InitSquads(const std::vector<MapSquad> &squads)
{
    if ( _levelInfo.Mode != 1 )
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

                    if ( !GetSectorInfo(&sect_info) )
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
    if ( !_levelInfo.Buddies.empty() )
    {
        int squad_sn = 0;
        
        std::vector<TMapBuddy> buds = _levelInfo.Buddies;
        while ( 1 )
        {
            std::vector<int> VhclIDS;
            int wrkID = -1;
            for (std::vector<TMapBuddy>::iterator it = buds.begin(); it != buds.end(); )
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

            vec3d squadPos =    _userRobo->_position +  
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

            NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(_userRobo);

            robo->MakeSquad(VhclIDS, squadPos, true); //robo 133 method

            squad_sn++;
        }
    }
}

void NC_STACK_ypaworld::yw_InitTechUpgradeBuildings()
{
    _upgradeTimeStamp = 0;
    _upgradeBuildId = 0;
    _upgradeVehicleId = 0;
    _upgradeWeaponId = 0;
    _upgradeId = -1;

    for (size_t i = 0; i < _techUpgrades.size(); i++)
    {
        TMapGem &gem = _techUpgrades[i];
        cellArea &cell = _cells(gem.CellId);

        if (gem.BuildingID)
        {
            if ( cell.PurposeType != cellArea::PT_BUILDINGS || gem.BuildingID != cell.PurposeIndex )
            {
                ypaworld_arg148 arg148;
                arg148.owner = cell.owner;
                arg148.blg_ID = gem.BuildingID;
                arg148.CellId = gem.CellId;
                arg148.field_C = 1;
                arg148.field_18 = 0;

                ypaworld_func148(&arg148);
            }
        }

        cell.PurposeType = cellArea::PT_TECHUPGRADE;
        cell.PurposeIndex = i;
    }
}

void NC_STACK_ypaworld::InitGates()
{
    for (size_t i = 0; i < _levelInfo.Gates.size(); i++)
    {
        TMapGate &gate = _levelInfo.Gates[i];

        gate.PCell = &_cells(gate.CellId);

        ypaworld_arg148 arg148;
        arg148.owner = gate.PCell->owner;
        arg148.blg_ID = gate.ClosedBldID;
        arg148.field_C = 1;
        arg148.CellId = gate.CellId;
        arg148.field_18 = 0;

        ypaworld_func148(&arg148);

        gate.PCell->PurposeType = cellArea::PT_GATECLOSED;
        gate.PCell->PurposeIndex = i;

        for (TMapKeySector &ks : gate.KeySectors)
        {
            if ( IsGamePlaySector( ks.CellId ) )
            {
                ks.PCell = &_cells(ks.CellId);
            }
        }
    }
}

void NC_STACK_ypaworld::InitSuperItems()
{
    for ( size_t i = 0; i < _levelInfo.SuperItems.size(); i++ )
    {
        TMapSuperItem &sitem = _levelInfo.SuperItems[i];
        
        sitem.PCell = &_cells(sitem.CellId);

        ypaworld_arg148 arg148;
        arg148.owner = sitem.PCell->owner;
        arg148.blg_ID = sitem.InactiveBldID;
        arg148.field_C = 1;
        arg148.CellId = sitem.CellId;
        arg148.field_18 = 0;

        ypaworld_func148(&arg148);

        sitem.PCell->PurposeType = cellArea::PT_STOUDSON;
        sitem.PCell->PurposeIndex = i;

        for ( TMapKeySector &ks : sitem.KeySectors )
        {
            if ( IsGamePlaySector( ks.CellId ) )
                ks.PCell = &_cells(ks.CellId);
        }

        sitem.ActiveTime = 0;
        sitem.TriggerTime = 0;
        sitem.ActivateOwner = 0;
        sitem.State = TMapSuperItem::STATE_INACTIVE;
    }
}

void NC_STACK_ypaworld::UpdatePowerEnergy()
{
    // Apply power to sectors and clean power matrix for next compute iteration.

    for (int y = 0; y < _mapSize.y; y++)
    {
        for (int x = 0; x < _mapSize.x; x++)
        {
            cellArea &cell = _cells(x, y);
            EnergyAccum &accum = _energyAccumMap(x, y);

            accum.Owner = cell.owner;
            cell.energy_power = accum.Energy; // Apply power to cell
            accum.Energy = 0; // Clean matrix's power
        }
    }

    _nextPSForUpdate = 0; // Next power station for recompute power is first
}


void NC_STACK_ypaworld::CellSetOwner(cellArea *cell, uint8_t owner)
{
    if ( cell->owner != owner )
    {
        HistoryEventAdd( World::History::Conq(cell->CellId.x, cell->CellId.y, owner) );

        if ( cell->PurposeType == cellArea::PT_POWERSTATION )
            HistoryEventAdd( World::History::PowerST(cell->CellId.x, cell->CellId.y, owner) );

        _countSectorsPerOwner[cell->owner]--;
        _countSectorsPerOwner[owner]++;

        cell->owner = owner;
    }
}

void NC_STACK_ypaworld::CellSetNewOwner(cellArea *cell, NC_STACK_ypabact *a5, int newOwner)
{
    int energon[World::FRACTION_MAXCOUNT];

    if ( newOwner < World::OWNER_UNKNOW )
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
        if ( cell->PurposeType == cellArea::PT_POWERSTATION )
        {
            if ( _userRobo->_owner == newOwner )
            {
                if ( a5 )
                {
                    yw_arg159 v21;
                    v21.unit = a5;
                    v21.Priority = 78;
                    v21.MsgID = 45;

                    ypaworld_func159(&v21);
                }
            }
            else if ( cell->owner == _userRobo->_owner )
            {
                yw_arg159 v24;
                v24.unit = NULL;
                v24.Priority = 78;
                v24.MsgID = 67;

                ypaworld_func159(&v24);
            }
        }
        else
        {
            for ( const TMapGate &gate : _levelInfo.Gates )
            {
                for ( const TMapKeySector &ks : gate.KeySectors )
                {
                    if ( cell == ks.PCell )
                    {
                        if ( _userRobo->_owner == newOwner )
                        {
                            yw_arg159 v23;
                            v23.unit = NULL;
                            v23.Priority = 80;
                            v23.MsgID = 82;

                            ypaworld_func159(&v23);
                        }
                        else if ( _userRobo->_owner == cell->owner )
                        {
                            yw_arg159 v22;
                            v22.unit = NULL;
                            v22.Priority = 80;
                            v22.MsgID = 81;

                            ypaworld_func159(&v22);
                        }
                    }
                }
            }
        }
    }

    CellSetOwner(cell, newOwner);
}

void NC_STACK_ypaworld::CellCheckHealth(cellArea *cell, int newOwner, NC_STACK_ypabact *a6)
{
    if ( cell->IsGamePlaySector() )
    {
        int helth = 0;

        for (auto h : cell->buildings_health)
            helth += h;

        if ( cell->PurposeType == cellArea::PT_POWERSTATION )
        {
            if ( helth )
            {
                auto psIt = _powerStations.find(cell->Id);
                if (psIt != _powerStations.end())
                {
                    TPowerStationInfo &psInf = psIt->second;
                    int effPower = (helth * psInf.Power) / 255 ;

                    if ( effPower < 0 )
                        effPower = 0;
                    else if ( effPower > 255 )
                        effPower = 255;

                    psInf.EffectivePower = effPower;
                }
            }
            else
            {
                PowerStationErase(cell);
            }
        }

        /* Fix for vanilla game bug that recalc sector owner after loading
         * saved game. */
        if (newOwner != World::OWNER_NOCHANGE)
        {
            if ( cell->SectorType == 1 )
            {
                if ( helth < 224 )
                    CellSetNewOwner(cell, a6, newOwner);
            }
            else if ( helth < 1728 )
            {
                CellSetNewOwner(cell, a6, newOwner);
            }
        }
    }
    else
    {
        CellSetOwner(cell, 0);
    }
}



TSectorCollision NC_STACK_ypaworld::sub_44DBF8(int _dx, int _dz, int _dxx, int _dzz, int flags)
{
    int v8 = flags;
    
    TSectorCollision tmp;
    tmp.sklt = NULL;
    tmp.Flags = 0;
    tmp.CollisionType = 0;
    
    

    if ( _dxx > 0 && _dxx < 4 * _mapSize.x - 1 && _dzz > 0  &&  _dzz < 4 * _mapSize.y - 1)
    {
        tmp.Cell = Common::Point(_dxx / 4, _dzz / 4);
        
        cellArea &cell = _cells(tmp.Cell);

        if ( _dxx % 4 && _dzz % 4 )
        {
            tmp.CollisionType = 1;

            int v14, v16;

            if ( cell.SectorType == 1 )
            {
                v14 = 0;
                v16 = 0;

                if ( (_dxx / 4) == (_dx / 4)  &&  (_dz / 4) == (_dzz / 4) )
                    v8 = flags & ~1;

                tmp.pos = World::SectorIDToCenterPos3(tmp.Cell);
                tmp.pos.y = cell.height;
            }
            else
            {
                v16 = (_dxx % 4) - 1;
                v14 = 2 - ((_dzz % 4) - 1);

                if ( _dxx == _dx && _dzz == _dz )
                    v8 = flags & ~1;

                tmp.pos.z = -(_dzz * 300.0);
                tmp.pos.x = _dxx * 300.0;
                tmp.pos.y = cell.height;
            }

            tmp.Flags = v8;

            int model_id = GetLegoBld(&cell, v16, v14);

            if ( v8 & 1 )
                tmp.sklt = _legoArray[model_id].UseCollisionSkelet;
            else
                tmp.sklt = _legoArray[model_id].CollisionSkelet;
        }
        else
        {
            tmp.pos.y = 0;
            tmp.pos.x = _dxx * 300.0;
            tmp.pos.z = -(_dzz * 300.0);

            if ( _dxx == _dx && _dzz == _dz )
                v8 = flags & ~1;

            tmp.Flags = v8;

            if ( _dxx % 4 == 0 && _dzz % 4 == 0)
            {
                tmp.sklt = _fillerCross;
                tmp.CollisionType = 4;
            }
            else if ( _dxx % 4 == 0 && _dzz % 4 != 0 )
            {
                tmp.sklt = _fillerSide;
                tmp.CollisionType = 2;
            }
            else if ( _dxx % 4 != 0 && _dzz % 4 == 0 )
            {
                tmp.sklt = _fillerSide;
                tmp.CollisionType = 3;
            }
        }

        if ( tmp.CollisionType && !tmp.sklt )
        {
            ypa_log_out("yw_GetSklt: WARNING, not CZT_INVALID, but Sklt NULL!\n");

            const char *v17 = "UNKNOWN";

            switch ( tmp.CollisionType )
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
            ypa_log_out("    Type=%s, sec_x=%d, sec_y=%d.\n", v17, tmp.Cell.x, tmp.Cell.y);

            tmp.CollisionType = 0;
        }
    }
    
    return tmp;
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

void NC_STACK_ypaworld::sub_44E07C(TSectorCollision &arg)
{
    if ( arg.CollisionType == 2 )
    {
        cellArea &cur = _cells(arg.Cell);
        cellArea &left = _cells(arg.Cell + Common::Point(-1, 0));

        if ( !(arg.Flags & 1) || fabs( (int)(cur.height - left.height)) < 500.0 )
        {
            UAskeleton::Data *skel = arg.sklt->GetSkelet();
            skel->POO[0].y = left.height;
            skel->POO[1].y = cur.height;
            skel->POO[2].y = cur.height;
            skel->POO[3].y = left.height;

            sub_44DF60(skel, 0);
        }
        else
        {
            arg.sklt = _colsubSkeleton;

            if ( cur.height > left.height )
                arg.pos.y = cur.height;
            else
                arg.pos.y = left.height;
        }
    }
    else if ( arg.CollisionType == 3 )
    {
        cellArea &cur = _cells(arg.Cell);
        cellArea &up = _cells(arg.Cell + Common::Point(0, -1));

        if ( !(arg.Flags & 1) || fabs( (int)(cur.height - up.height)) < 500.0 )
        {
            UAskeleton::Data *skel = arg.sklt->GetSkelet();
            skel->POO[0].y = up.height;
            skel->POO[1].y = up.height;
            skel->POO[2].y = cur.height;
            skel->POO[3].y = cur.height;

            sub_44DF60(skel, 0);
        }
        else
        {
            arg.sklt = _colsubSkeleton;

            if ( cur.height > up.height )
                arg.pos.y = cur.height;
            else
                arg.pos.y = up.height;
        }
    }
    else if ( arg.CollisionType == 4 )
    {

        int kk = 0;

        cellArea &cur = _cells(arg.Cell);
        cellArea &left = _cells(arg.Cell + Common::Point(-1, 0));
        cellArea &up = _cells(arg.Cell + Common::Point(0, -1));
        cellArea &leftup = _cells(arg.Cell + Common::Point(-1, -1));

        if ( arg.Flags & 1 )
        {
            float cs = cur.height;
            float ls = left.height;
            float us = up.height;
            float lus = leftup.height;

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
                arg.sklt = _colsubSkeleton;
                arg.pos.y = v18;
                kk = 1;
            }
        }
        if ( !kk )
        {
            UAskeleton::Data *skel = arg.sklt->GetSkelet();
            skel->POO[0].y = leftup.height;
            skel->POO[1].y = up.height;
            skel->POO[2].y = cur.height;
            skel->POO[3].y = left.height;
            skel->POO[4].y = cur.averg_height;

            sub_44DF60(skel, 0);
            sub_44DF60(skel, 1);
            sub_44DF60(skel, 2);
            sub_44DF60(skel, 3);
        }
    }
}

int sub_44D36C(const vec3d &v, int id, NC_STACK_skeleton *skeleton)
{
    UAskeleton::Data *sklt = skeleton->GetSkelet();
    
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
            const UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            const UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

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
            const UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            const UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

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
            const UAskeleton::Vertex &cur = sklt->POO[ sklt->polygons[id].v[i] ];
            const UAskeleton::Vertex &prv = sklt->POO[ sklt->polygons[id].v[prev] ];

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

void NC_STACK_ypaworld::sub_44D8B8(ypaworld_arg136 *arg, const TSectorCollision &loc)
{
    UAskeleton::Data *skel = loc.sklt->GetSkelet();
    for ( size_t i = 0; i < skel->polygons.size(); i++)
    {
        UAskeleton::Polygon &tr = skel->polygons[i];
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
                    arg->skel = loc.sklt->GetSkelet();
                }
            }
        }
    }
}


void NC_STACK_ypaworld::FFeedback_VehicleChanged()
{
    NC_STACK_input *inpt = INPe.GetInput();
    
    if ( _shellConfIsParsed )
    {
        if ( _preferences & (World::PREF_JOYDISABLE | World::PREF_FFDISABLE) )
            return;
    }

    if ( _userUnit )
    {
        _ffTimeStamp = _timeStamp;

        if ( inpt )
            inpt->ForceFeedback(NC_STACK_winp::FF_STATE_STOP, NC_STACK_winp::FF_TYPE_ALL);

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


        switch ( _userUnit->_bact_type )
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
            float v4 = (_userUnit->_mass - v16) / (v13 - v16);
            float v5 = (_userUnit->_maxrot - v15) / (v14 - v15);

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

            _ffEffectType = effectType;
            _ffPeriod = v23;
            _ffMagnitude = v21;

            if ( inpt )
            {
                inpt->ForceFeedback(NC_STACK_winp::FF_STATE_START, effectType, v21, v23);
                inpt->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_ROTDAMPER, v20);
            }
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




void NC_STACK_ypaworld::RenderAdditionalBeeBox(Common::Point sect, TRenderingSector *sct, baseRender_msg *bs77)
{
    sct->dword8 = 0;
    sct->dword4 = 0;

    if ( IsSector( sect ) )
    {
        sct->dword4 = 1;
        sct->p_cell = &_cells( sect );

        vec3d pos = World::SectorIDToCenterPos3( sect );
        pos.y = sct->p_cell->height;

        _beeBox->SetPosition(pos);

        if ( _beeBox->Render(bs77, NULL) )
        {
            sct->dword8 = 1;
        }
    }
}

void NC_STACK_ypaworld::RenderSector(TRenderingSector *sct, baseRender_msg *bs77)
{
    if ( sct->dword8 )
    {
        cellArea *pcell = sct->p_cell;

        int v22 = 0;

        vec3d scel;
        if ( pcell->PurposeType == cellArea::PT_CONSTRUCTING )
        {
            auto it = _inBuildProcess.find(pcell->Id);
            if (it != _inBuildProcess.end())
            {
                TConstructInfo &bldProc = it->second;

                scel = vec3d::OY((float)bldProc.Time / (float)bldProc.EndTime);

                pcell->type_id = _buildProtos[ bldProc.BuildID ].SecType;
                pcell->SectorType = _secTypeArray[ pcell->type_id ].SectorType;

                v22 = 1;
            }
        }

        int v17, v20;

        if ( pcell->SectorType == 1 )
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
                vec3d pos = sct->p_cell->CenterPos + vec3d((v17 + xx) * 300.0, 0.0, (v17 + zz) * 300.0);

                if ( v22 )
                {
                    NC_STACK_base *bld = _legoArray[ _secTypeArray[ pcell->type_id ].SubSectors.At(xx, zz)->HPModels[0] ].Base;

                    bld->SetStatic(false);

                    bld->SetScale(scel, NC_STACK_base::UF_Y); //Scale only Y
                    bld->SetPosition(pos);
                    
                    NC_STACK_base::CheckOpts( &pcell->BldVPOpts.At(xx, zz), bld );
                    
                    bld->Render(bs77, pcell->BldVPOpts.At(xx, zz));

                    bld->SetStatic(true);
                }
                else
                {
                    NC_STACK_base *bld = _legoArray[ GetLegoBld(pcell, xx, zz) ].Base;

                    bld->SetPosition(pos);
                    
                    NC_STACK_base::CheckOpts( &pcell->BldVPOpts.At(xx, zz), bld );
                    
                    bld->Render(bs77, pcell->BldVPOpts.At(xx, zz));
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

void NC_STACK_ypaworld::yw_renderSky(baseRender_msg *rndr_params)
{
    if ( _skyObject )
    {
        float v6 = rndr_params->maxZ;
        uint32_t flags = rndr_params->flags;

        _skyObject->SetPosition( _viewerBact->_position + vec3d::OY(_skyHeight) );

        rndr_params->maxZ = 32000.0;
        rndr_params->flags = GFX::RFLAGS_SKY | GFX::RFLAGS_COMPUTED_COLOR;
        if ( System::IniConf::GfxNewSky.Get<bool>() )
            rndr_params->flags |= GFX::RFLAGS_DISABLE_ZWRITE;

        _skyObject->Render(rndr_params, NULL);

        rndr_params->maxZ = v6;
        rndr_params->flags = flags;
    }
}


bool NC_STACK_ypaworld::IsVisibleMapPos(vec2d pos)
{
    Common::Point pt( ((pos.x + 150) / 300) / 4, ((-pos.y + 150) / 300) / 4 );

    if ( !IsGamePlaySector( pt ) || !_viewerBact )
        return false;

    Common::Point dist = _viewerBact->_cellId.AbsDistance( pt ); 
    if ( dist.x + dist.y <= (_renderSectors - 1) / 2 )
        return true;

    return false;
}

void NC_STACK_ypaworld::RenderSuperWave(vec2d pos, vec2d fromPos, baseRender_msg *arg)
{
    if ( !_stoudsonWaveVehicleId )
        return;
    
    if ( pos.x > 0.0 && pos.y < 0.0 && pos.x < _mapLength.x && -_mapLength.y < pos.y )
    {
        if ( IsVisibleMapPos(pos) )
        {
            int v10 = _vhclProtos[_stoudsonWaveVehicleId].vp_normal;

            NC_STACK_base *wall_base = _vhclModels.at(v10);

            if ( wall_base )
            {
                float v28 = 0.0;

                int v23 = (pos.x + 150) / 300;
                int v26 = (-pos.y + 150) / 300;

                if ( (v23 & 3) && (v26 & 3) )
                {
                    v28 = _cells((v23 / 4), (v26 / 4)).height;
                }
                else
                {
                    ypaworld_arg136 v22;
                    v22.vect = vec3d::OY(50000.0);
                    v22.stPos.x = pos.x;
                    v22.stPos.y = -25000.0;
                    v22.stPos.z = pos.y;
                    v22.flags = 0;

                    ypaworld_func136(&v22);

                    if ( v22.isect )
                    {
                        v28 = v22.isectPos.y;
                    }
                }


                wall_base->TForm().Pos = vec3d(pos.x, v28, pos.y);

                vec2d delt = pos - fromPos;
                delt.normalise();

                wall_base->TForm().SclRot =  mat3x3(delt.y,   0, -delt.x,
                                                      0, 1.0,    0,
                                                    delt.x, 0.0,  delt.y);

                wall_base->Render(arg, NULL);
            }
        }
    }
}

void NC_STACK_ypaworld::RenderSuperItems(baseRender_msg *arg)
{
    // Render super items
    for ( const TMapSuperItem &sitem : _levelInfo.SuperItems )
    {
        if ( sitem.State == TMapSuperItem::STATE_TRIGGED )
        {
            vec2d pos = World::SectorIDToCenterPos2( sitem.CellId );

            float v14 = sqrt( POW2(_mapLength.x) + POW2(_mapLength.y) );

            if ( sitem.CurrentRadius > 300 && sitem.CurrentRadius < v14 )
            {
                float v17 = (2 * sitem.CurrentRadius) * C_PI / 300.0;

                if ( v17 > 2.0 )
                {
                    float v9 = 6.283 / v17;

                    for (float j = 0.0; j < 6.283; j = j + v9 )
                    {
                        vec2d wallpos = vec2d(cos(j), sin(j)) * sitem.CurrentRadius + pos;
                        RenderSuperWave(wallpos, pos, arg);
                    }
                }
            }
        }
    }
}

void NC_STACK_ypaworld::PrepareFiller(cellArea *sct, cellArea *sct2, float v9h, float v8h, bool vertical, TCellFillerCh *out, bool force)
{
    int x = _secTypeArray[ sct->type_id ].SurfaceType;
    int y = _secTypeArray[ sct2->type_id ].SurfaceType;
    
    if (!force && (out->Id1 == x && out->Id2 == y && 
        out->Heights[0] == sct->height && out->Heights[1] == sct2->height && 
        out->Heights[2] == v8h && out->Heights[3] == v9h))
        return;

    NC_STACK_base *bs;
    if (vertical)
        bs = _fillersVertical(x, y);
    else
        bs = _fillersHorizontal(x, y);
    
    UAskeleton::Data *skel = bs->GetSkeleton()->GetSkelet();
    
    vec2d pos = World::SectorIDToCenterPos2( sct2->CellId );

    bs->SetPosition( vec3d::X0Z( pos ), NC_STACK_base::UF_XZ );

    for (int i = 0; i < 4; i++)
        skel->POO[i].y = sct->height;

    for (int i = 4; i < 8; i++)
        skel->POO[i].y = sct2->height;

    skel->POO[8].y = v8h;
    skel->POO[9].y = v9h;
    
    bs->RecalcInternal(true);
    bs->MakeCoordsCache();
    
    out->FreeVBO();
    
    bs->MakeCache(out);
    
    out->MakeVBO();
    
    out->Id1 = x;
    out->Id2 = y;
    out->Heights[0] = sct->height;
    out->Heights[1] = sct2->height;
    out->Heights[2] = v8h;
    out->Heights[3] = v9h;
}

void NC_STACK_ypaworld::PrepareAllFillers()
{
    for (int i = 0; i < _mapSize.x - 1; i++)
    {
        for (int j = 0; j < _mapSize.y - 2; j++)
        {
            cellArea *sct = &_cells(i, j);
            cellArea *sct2 = &_cells(i, j + 1);
            
            float h;
            if (i == _mapSize.x - 1)
                h = sct2->averg_height;
            else
                h = _cells(i + 1, j + 1).averg_height;
            
            PrepareFiller(sct, sct2, sct2->averg_height, h, false, &_cellsHFCache(i, j), true);
        }
    }
    
    for (int i = 0; i < _mapSize.x - 2; i++)
    {
        for (int j = 0; j < _mapSize.y - 1; j++)
        {
            cellArea *sct = &_cells(i, j);
            cellArea *sct2 = &_cells(i + 1, j);
            
            float h;
            if (i == _mapSize.x - 1)
                h = sct2->averg_height;
            else
                h = _cells(i + 1, j + 1).averg_height;
            
            PrepareFiller(sct, sct2, sct2->averg_height, h, true, &_cellsVFCache(i, j), true);
        }
    }
}




TRenderingSector rendering_sectors[YW_RENDER_SECTORS_DEF * 2][ YW_RENDER_SECTORS_DEF * 2];

void NC_STACK_ypaworld::RenderFillers(baseRender_msg *arg)
{
    //Render landscape linking parts
    for (int i = 0; i < _renderSectors; i++)
    {
        for (int j = 0; j < _renderSectors - 1; j++)
        {
            TRenderingSector &sct = rendering_sectors[j][i];
            TRenderingSector &sct2 = rendering_sectors[j + 1][i];
            
            if (sct.dword4 == 1 && sct2.dword4 == 1 && (sct.dword8 == 1 || sct2.dword8 == 1))
            {
                float h;
                if (rendering_sectors[j + 1][i + 1].dword4 == 1)
                    h = rendering_sectors[j + 1][i + 1].p_cell->averg_height;
                else
                    h = sct2.p_cell->averg_height;
                    
                TCellFillerCh &filler = _cellsVFCache( sct2.p_cell->CellId.x - 1, sct2.p_cell->CellId.y );
                PrepareFiller(sct.p_cell, sct2.p_cell, sct2.p_cell->averg_height, h, true, &filler);
                filler.Render(arg);
            }
        }
    }

    for (int i = 0; i < _renderSectors - 1; i++)
    {
        for (int j = 0; j < _renderSectors; j++)
        {
            TRenderingSector &sct = rendering_sectors[j][i];
            TRenderingSector &sct2 = rendering_sectors[j][i + 1];
            
            if (sct.dword4 == 1 && sct2.dword4 == 1 && (sct.dword8 == 1 || sct2.dword8 == 1))
            {
                float h;
                if (rendering_sectors[j + 1][i + 1].dword4 == 1)
                    h = rendering_sectors[j + 1][i + 1].p_cell->averg_height;
                else
                    h = sct2.p_cell->averg_height;
                    
                TCellFillerCh &filler = _cellsHFCache( sct2.p_cell->CellId.x, sct2.p_cell->CellId.y - 1 );
                PrepareFiller(sct.p_cell, sct2.p_cell, sct2.p_cell->averg_height, h, false, &filler);
                filler.Render(arg);
            }
        }
    }
}

void NC_STACK_ypaworld::RenderGame(base_64arg *bs64, int a2)
{
    if ( !_viewerBact )
        return;

    TF::TForm3D *v5 = TF::Engine.GetViewPoint();

    if ( v5 )
        v5->CalcGlobal();

    baseRender_msg rndrs;

    rndrs.flags = 0;
    rndrs.frameTime = bs64->DTime;
    rndrs.globTime = bs64->TimeStamp;
    rndrs.adeCount = 0;

    rndrs.minZ = 1.0;

    if ( _renderSectors == 5 )
        rndrs.maxZ = 1500.0;
    else
        rndrs.maxZ = 3500.0;

    int v6 = _renderSectors - 1;

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
            TRenderingSector *sct = &rendering_sectors[v29 + j][v29 - v28];

            RenderAdditionalBeeBox( _viewerBact->_cellId + Common::Point(j, -v28), 
                                    sct, &rndrs);

            if ( sct->dword4 )
                RenderSector(sct, &rndrs);

        }

        if ( -v28 != v28 )
        {
            for (int j = -i; j <= i; j++)
            {
                TRenderingSector *sct = &rendering_sectors[v29 + j][v29 + v28];

                RenderAdditionalBeeBox( _viewerBact->_cellId + Common::Point(j, v28), 
                                        sct, &rndrs);

                if ( sct->dword4 )
                    RenderSector(sct, &rndrs);
            }
        }
    }

    
    RenderSuperItems(&rndrs);

    RenderFillers(&rndrs);

    bs64->field_C = rndrs.adeCount;

    _polysCount = rndrs.adeCount;
    _polysDraw = 7777;


    area_arg_65 rrg;
    rrg.timeStamp = bs64->TimeStamp;
    rrg.frameTime = bs64->DTime;
    rrg.minZ = 1.0;
    rrg.maxZ = rndrs.maxZ;
    rrg.ViewTForm = TF::Engine.GetViewPoint();
    rrg.OwnerTForm = NULL;
    rrg.flags = 0;


    ParticleSystem().UpdateRender(&rrg, bs64->DTime);

    GFX::Engine.BeginScene();
    
    if ( _skyRender )
        yw_renderSky(&rndrs);

    GFX::Engine.Rasterize();

    GFX::Engine.EndScene();

    if ( a2 )
    {
        uint32_t tpm = profiler_begin();
        sb_0x4d7c08__sub0(this);
        _profileVals[PFID_NEWGUITIME] = profiler_end(tpm);
    }
}


void NC_STACK_ypaworld::ResetAccumMap()
{
    for( int y = 0; y < _mapSize.y; y++ )
    {
        for( int x = 0; x < _mapSize.x; x++ )
        {
            EnergyAccum &accum = _energyAccumMap(x, y);
            accum.Energy = 0;
            accum.Owner = _cells(x, y).owner;
        }
    }

    _nextPSForUpdate = 0;
}

void NC_STACK_ypaworld::SetupPowerStationInfo(cellArea *cell, int power)
{
    if (!cell) return;
    
    TPowerStationInfo &ps = _powerStations[cell->Id];

    ps.CellId = cell->CellId;
    ps.Power = power;
    ps.EffectivePower = power;
    ps.pCell = cell;
    
    cell->PurposeType = cellArea::PT_POWERSTATION;
    cell->PurposeIndex = 0;
    
    ResetAccumMap();
}


void NC_STACK_ypaworld::sb_0x456384(const Common::Point &cellId, int ownerid2, int blg_id, int a7)
{
    uamessage_bldVhcl bvMsg;

    cellArea &cell = _cells(cellId);
    World::TBuildingProto *bld = &_buildProtos[ blg_id ];
    TSectorDesc *sectp = &_secTypeArray[ bld->SecType ];

    int v43 = 1;

    NC_STACK_yparobo *robo = NULL;

    if ( cell.IsGamePlaySector() )
    {
        _lvlBuildingsMap(cellId) = blg_id;
        _lvlTypeMap(cellId) = bld->SecType;

        cell.type_id = bld->SecType;
        cell.energy_power = 0;
        cell.PurposeType = cellArea::PT_BUILDINGS;
        cell.SectorType = sectp->SectorType;
        cell.PurposeIndex = blg_id;

        int v49;

        if ( sectp->SectorType == 1 )
        {
            cell.buildings_health.fill(0);
            
            v49 = 1;
        }
        else
        {
            v49 = 3;
        }

        for (int yy = 0; yy < v49; yy++)
        {
            for (int xx = 0; xx < v49; xx++)
                cell.buildings_health.At(xx, yy) = sectp->SubSectors.At(xx, yy)->StartHealth;
        }

        if ( bld->ModelID == 1 )
            SetupPowerStationInfo(&cell, bld->Power);

        CellSetOwner(&cell, ownerid2);

        for( NC_STACK_ypabact * &unit: _unitsList )
        {
            if (unit->_bact_type == BACT_TYPES_ROBO && unit->_owner == ownerid2)
            {
                robo = dynamic_cast<NC_STACK_yparobo *>(unit);
                break;
            }
        }

        if ( _isNetGame )
        {
            if ( robo != _userRobo )
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
                    World::TBuildingProto::TGun &GunProto = bld->Guns[i];
                    
                    if ( !GunProto.VhclID )
                        break;

                    ypaworld_arg146 v33;
                    v33.vehicle_id = GunProto.VhclID;
                    v33.pos = GunProto.Pos + World::SectorIDToCenterPos3( cellId );

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

                        if ( _isNetGame && i < 8 )
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

                if ( _isNetGame )
                {
                    bvMsg.msgID = UAMSG_BUILDINGVHCL;
                    bvMsg.tstamp = _timeStamp;
                    bvMsg.owner = ownerid2;

                    yw_arg181 v31;
                    v31.recvFlags = 2;
                    v31.recvID = 0;
                    v31.senderID = _GameShell->netPlayerName.c_str();
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


void NC_STACK_ypaworld::DestroyAllGunsInSector(cellArea *cell)
{
    /*
     * Destroy all GUN units in sector     
     */
    
    // Safe iterator, because it will call ModifyEnergy->Die for units
    for ( NC_STACK_ypabact* itUnit : cell->unitsList.safe_iter() )
    {
        int v5 = 0;

        if ( _isNetGame )
        {
            // In netgame only destroy own units
            if ( itUnit->_owner == _userUnit->_owner )
            {
                if ( itUnit->_status != BACT_STATUS_DEAD && itUnit->_status != BACT_STATUS_BEAM && itUnit->_status != BACT_STATUS_CREATE )
                {
                    if ( itUnit->_bact_type == BACT_TYPES_GUN )
                    {
                        NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( itUnit );

                        if (!gun->IsRoboGun())
                            v5 = 1;
                    }
                }
            }
        }
        else
        {
            if ( itUnit->_status != BACT_STATUS_DEAD && itUnit->_status != BACT_STATUS_BEAM && itUnit->_status != BACT_STATUS_CREATE )
            {
                if ( itUnit->_bact_type == BACT_TYPES_GUN )
                {
                    NC_STACK_ypagun *gun = dynamic_cast<NC_STACK_ypagun *>( itUnit );

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

            itUnit->ModifyEnergy(&v8);
        }
    }
}

bool NC_STACK_ypaworld::BuildingConstructBegin(cellArea *cell, uint8_t buildID, int owner, int cTime)
{
    if (!cell)
        return false;
    
    if (cell->IsBorder())
        return false;
    
    auto it = _inBuildProcess.find(cell->Id);
    if (it != _inBuildProcess.end())
        return false;
    
    TConstructInfo &bldProc = _inBuildProcess[cell->Id];
    bldProc.CellID = cell->CellId;
    bldProc.Time = 0;
    bldProc.EndTime = cTime;
    bldProc.BuildID = buildID;
    bldProc.Owner = owner;
    
    cell->PurposeType = cellArea::PT_CONSTRUCTING;
    
    for (NC_STACK_ypabact * &unit : _unitsList)
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && owner == unit->_owner )
        {
            SFXEngine::SFXe.startSound(&unit->_soundcarrier, 11);
            break;
        }
    }
    
    return true;
}

int ypaworld_func137__sub0__sub0(UAskeleton::Data *skl, int id, const vec3d &v, float r, vec3d &out)
{
    UAskeleton::Polygon &pg = skl->polygons[id];
    vec3d tmp(0.0, 0.0, 0.0);

    for (int i = 0; i < pg.num_vertices; i++)
    {
        int16_t idd = pg.v[i];
        tmp += static_cast<vec3d> (skl->POO[ idd ]);
    }

    vec3d tmp2 = tmp / pg.num_vertices - v;

    float v26 = tmp2.length();

    if ( v26 <= r )
        return 0;

    out = tmp2 * (r / v26) + v;

    return 1;
}

void NC_STACK_ypaworld::ypaworld_func137__sub0(ypaworld_arg137 *arg, const TSectorCollision &a2)
{
    for (size_t i = 0; i < a2.sklt->GetSkelet()->polygons.size(); i++)
    {
        const UAskeleton::Polygon &tria = a2.sklt->GetSkelet()->polygons[i];

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

                if ( ypaworld_func137__sub0__sub0(a2.sklt->GetSkelet(), i, tx, arg->radius, v18) )
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

NC_STACK_ypabact * NC_STACK_ypaworld::FindBactByCmdOwn(uint32_t commandID, char owner)
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


void NC_STACK_ypaworld::BuildingConstructUpdate(int dtime)
{
    for(auto it  = _inBuildProcess.begin(); it != _inBuildProcess.end(); )
    {
        TConstructInfo &bldProc = it->second;
        bldProc.Time += dtime;

        if ( bldProc.Time >= bldProc.EndTime )
        {
            cellArea &rCell = _cells( bldProc.CellID );
            rCell.PurposeType = cellArea::PT_NONE;
            rCell.PurposeIndex = 0;

            sb_0x456384(bldProc.CellID, bldProc.Owner, bldProc.BuildID, 0);

            if ( bldProc.Owner == _userRobo->_owner )
            {
                if ( _buildProtos[ bldProc.BuildID ].ModelID )
                {
                    yw_arg159 arg159;

                    arg159.unit = _userRobo;
                    arg159.Priority = 65;

                    switch( _buildProtos[ bldProc.BuildID ].ModelID )
                    { 
                        case 1:
                        arg159.MsgID = 36;
                        break;
                        
                        case 2:
                        arg159.MsgID = 38;
                        break;
                    
                        case 3:
                        arg159.MsgID = 37;
                        break;
                        
                        default:
                        arg159.MsgID = 0;
                        break;
                    }

                    ypaworld_func159(&arg159);
                }
            }
            it = _inBuildProcess.erase(it);
        }
        else
            ++it;
    }
}

bool NC_STACK_ypaworld::IsAnyBuildingProcess(int owner) const
{
    for(const auto &it : _inBuildProcess)
    {
        if (it.second.Owner == owner)
            return true;
    }
    return false;
}

void NC_STACK_ypaworld::ypaworld_func64__sub6__sub0()
{
    for(int i = 0; i < 8; i++)
        _countUnitsPerOwner[i] = 0;

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
                        _countUnitsPerOwner[ comnd->_owner ]++;

                        for( NC_STACK_ypabact * &unit : comnd->_kidList )
                        {
                            if ( unit->_status != BACT_STATUS_DEAD && unit->_status != BACT_STATUS_BEAM )
                                _countUnitsPerOwner[ unit->_owner ]++;
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

    for (const auto &ps : yw->_powerStations)
    {
        if (ps.second.pCell)
            v13[ ps.second.pCell->owner ] += ps.second.EffectivePower;
    }

    yw->ypaworld_func64__sub6__sub0();

    for (int i = 0; i < 8; i++)
    {
        v13[i] /= 2;

        if ( v13[i] <= 0 )
        {
            yw->_reloadRatioPositive[i] = 0;
            yw->_reloadRatioClamped[i] = 0;
        }
        else
        {
            int v15 = yw->_countSectorsPerOwner[i];

            if ( v15 < 0 )
                v15 = 0;

            yw->_reloadRatioClamped[i] = (float)v15 / (float)v13[i];
            yw->_reloadRatioPositive[i] = (float)v15 / (float)v13[i];

            if ( yw->_isNetGame )
            {
                if ( yw->_levelUnitLimitType == 1 )
                {
                    int v16 = yw->_countUnitsPerOwner[yw->_userRobo->_owner] - yw->_levelUnitLimit;

                    if ( v16 > 0 )
                    {
                        int v10 = (float)yw->_levelUnitLimitArg * 0.01 * (float)v16;

                        yw->_reloadRatioClamped[i] -= v10;
                        yw->_reloadRatioPositive[i] -= v10;
                    }
                }
            }

            if ( yw->_reloadRatioClamped[i] > 1.0 )
                yw->_reloadRatioClamped[i] = 1.0;
            else if ( yw->_reloadRatioClamped[i] < 0.0 )
                yw->_reloadRatioClamped[i] = 0;

            if ( yw->_reloadRatioPositive[i] < 0.0 )
                yw->_reloadRatioPositive[i] = 0;
        }
    }
}


void NC_STACK_ypaworld::RecalcSectorsPowerForPS(const TPowerStationInfo &ps)
{
    int pwrTmp = ps.EffectivePower;
    int powsCount = 0;

    while (pwrTmp > 0)
    {
        pwrTmp >>= 1;
        powsCount++;
    }

    int sdx = -powsCount;
    int edx = powsCount + 1;

    int sdy = -powsCount;
    int edy = powsCount + 1;

    if ( ps.CellId.x + sdx < 1 )
        sdx = 1 - ps.CellId.x;

    if ( ps.CellId.y + sdy < 1 )
        sdy = 1 - ps.CellId.y;

    if ( ps.CellId.x + edx >= _mapSize.x )
        edx = _mapSize.x - ps.CellId.x - 1;

    if ( ps.CellId.y + edy >= _mapSize.y )
        edy = _mapSize.y - ps.CellId.y - 1;

    for (int dy = sdy; dy < edy; dy++)
    {
        for (int dx = sdx; dx < edx; dx++)
        {
            int v17 = ps.EffectivePower  >>  _sqrtTable(abs(dx), abs(dy));

            EnergyAccum &accum = _energyAccumMap(dx + ps.CellId.x, dy + ps.CellId.y);

            if ( accum.Owner == ps.pCell->owner )
            {
                accum.Energy += v17; // Add power to this cell

                if ( accum.Energy > 255 )
                    accum.Energy = 255;
            }

        }
    }
}

void NC_STACK_ypaworld::DoSectorsEnergyRecalc()
{
    // Recompute power on sectors
    if ( !_powerStations.empty() ) // If we have powerstations
    {
        auto itPs = _powerStations.lower_bound(_nextPSForUpdate);
        
        if (itPs == _powerStations.end()) // If we reach end of power stations list, apply power to sectors
        {
            UpdatePowerEnergy(); // Apply power to sectors and clean power matrix for next compute iteration.
        }
        else
        {
            if ( itPs->second.EffectivePower ) // if this power station has power
                RecalcSectorsPowerForPS(itPs->second); // Add power to power matrix

            _nextPSForUpdate = itPs->first + 1; // go to next station in next update loop
        }
    }
}


void NC_STACK_ypaworld::sub_4D12D8(int id, int a3)
{
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    sitem.State = TMapSuperItem::STATE_ACTIVE;
    sitem.TriggerTime = 0;
    sitem.ActivateOwner = sitem.PCell->owner;

    if ( !a3 )
    {
        sitem.ActiveTime = _timeStamp;
        sitem.LastTenSec = 0;
        sitem.LastSec = 0;
        sitem.CountDown = sitem.TimerValue;
    }

    ypaworld_arg148 arg148;
    arg148.owner = sitem.PCell->owner;
    arg148.blg_ID = sitem.ActiveBldID;
    arg148.field_C = 1;
    arg148.CellId = sitem.CellId;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->PurposeType = cellArea::PT_STOUDSON;
    sitem.PCell->PurposeIndex = id;

    yw_arg159 arg159;
    arg159.unit = 0;
    arg159.Priority = 94;

    if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_BOMBACT);
        arg159.MsgID = 70;
    }
    else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_WAVEACT);
        arg159.MsgID = 74;
    }
    else
    {
        arg159.MsgID = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::sub_4D1594(int id)
{
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    sitem.State = TMapSuperItem::STATE_STOPPED;

    ypaworld_arg148 arg148;
    arg148.owner = sitem.PCell->owner;
    arg148.blg_ID = sitem.InactiveBldID;
    arg148.field_C = 1;
    arg148.CellId = sitem.CellId;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->PurposeType = cellArea::PT_STOUDSON;
    sitem.PCell->PurposeIndex = id;

    yw_arg159 arg159;
    arg159.unit = 0;
    arg159.Priority = 93;

    if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_BOMBFROZ);
        arg159.MsgID = 72;
    }
    else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_WAVEFROZ);
        arg159.MsgID = 76;
    }
    else
    {
        arg159.MsgID = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::sub_4D1444(int id)
{
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];
    sitem.State = TMapSuperItem::STATE_TRIGGED;
    sitem.TriggerTime = _timeStamp;

    ypaworld_arg148 arg148;
    arg148.owner = sitem.PCell->owner;
    arg148.blg_ID = sitem.TriggerBldID;
    arg148.field_C = 1;
    arg148.CellId = sitem.CellId;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->PurposeType = cellArea::PT_STOUDSON;
    sitem.PCell->PurposeIndex = id;

    sitem.LastRadius = 0;

    yw_arg159 arg159;
    arg159.Priority = 95;
    arg159.unit = 0;

    if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_BOMBTRIG);
        arg159.MsgID = 71;
    }
    else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_WAVETRIG);
        arg159.MsgID = 75;
    }
    else
    {
        arg159.MsgID = 0;
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
    if ( yw->_isNetGame )
        return 0;

    if ( !yw->_gamePaused )
    {
        if ( arg->field_8->HotKeyID == 32 || arg->field_8->KbdLastHit == Input::KC_PAUSE )
        {
            yw->_gamePaused = true;
            yw->_gamePausedTimeStamp = arg->TimeStamp;
        }
        return 0;
    }

    if ( arg->field_8->KbdLastHit != Input::KC_NONE )
    {
        yw->_gamePaused = false;
        arg->TimeStamp = yw->_gamePausedTimeStamp;
    }
    else
    {
        GFX::Engine.BeginFrame();

        /*yw->_win3d->setRSTR_BGpen(0);

        yw->_win3d->raster_func192(NULL);*/

        vec3d a2a = yw->_viewerPosition + vec3d::OY(50000.0);

        SFXEngine::SFXe.sub_423EFC(1, a2a, vec3d(0.0), mat3x3::Ident());

        if ( arg->TimeStamp / 500 & 1 )
        {
            const std::string v6 = Locale::Text::Common(Locale::CMN_PAUSED);

            char v10[256];
            char *pcur = v10;

            FontUA::select_tileset(&pcur, 15);

            FontUA::set_xpos(&pcur, 0);
            FontUA::set_center_ypos(&pcur, -yw->_fontH / 2);

            pcur = FontUA::FormateCenteredSkipableItem(yw->_guiTiles[15], pcur, v6, yw->_screenSize.x);

            FontUA::set_end(&pcur);

            w3d_a209 arg209;
            arg209.includ = 0;
            arg209.cmdbuf = v10;

            GFX::Engine.raster_func209(&arg209);
        }

        SFXEngine::SFXe.sb_0x424c74();

        GFX::Engine.EndFrame();
    }
    return 1;
}


void ypaworld_func64__sub2(NC_STACK_ypaworld *yw)
{
    yw->_playerInHSGun = false;

    if ( yw->_userRobo != yw->_userUnit )
    {
        NC_STACK_yparobo *robo = dynamic_cast<NC_STACK_yparobo *>(yw->_userRobo);

        for (World::TRoboGun &gun : robo->GetGuns())
        {
            if ( yw->_userUnit == gun.gun_obj )
                yw->_playerInHSGun = true;
        }
    }
}


void ypaworld_func64__sub9(NC_STACK_ypaworld *yw)
{
    for ( size_t i = 0; i < yw->_levelInfo.Gates.size(); i++ )
    {
        const TMapGate &gate = yw->_levelInfo.Gates[i];
        int gateState = cellArea::PT_GATEOPENED;

        if ( gate.PCell->owner == yw->_userRobo->_owner )
        {
            for( const TMapKeySector &ks : gate.KeySectors )
            {
                if (ks.PCell)
                {
                    if (ks.PCell->owner != yw->_userRobo->_owner)
                    {
                        gateState = cellArea::PT_GATECLOSED;
                        break;
                    }
                }
            }
        }
        else
        {
            gateState = cellArea::PT_GATECLOSED;
        }

        if ( gate.PCell->PurposeType != gateState )
        {
            ypaworld_arg148 arg148;
            arg148.owner = gate.PCell->owner;
            arg148.field_C = 1;
            arg148.CellId = gate.CellId;
            arg148.field_18 = 0;

            if ( gateState == cellArea::PT_GATEOPENED )
            {
                arg148.blg_ID = gate.OpenBldID;
            }
            else
            {
                arg148.blg_ID = gate.ClosedBldID;

                yw_arg159 arg159;
                arg159.unit = 0;
                arg159.Priority = 65;
                arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_GATECLOSE);
                arg159.MsgID = 24;

                yw->ypaworld_func159(&arg159);
            }

            yw->ypaworld_func148(&arg148);

            gate.PCell->PurposeType = gateState;
            gate.PCell->PurposeIndex = i;
        }

        if ( gateState == cellArea::PT_GATEOPENED )
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

            if ( energ <= yw->_beamEnergyCapacity )
            {
                if ( yw->_timeStamp - yw->_msgTimestampGates > 60000 )
                {
                    yw_arg159 arg159_1;
                    arg159_1.unit = 0;
                    arg159_1.Priority = 49;
                    arg159_1.txt = Locale::Text::Feedback(Locale::FEEDBACK_GATEOPEN);
                    arg159_1.MsgID = 23;

                    yw->ypaworld_func159(&arg159_1);
                    yw->_msgTimestampGates = yw->_timeStamp;
                }
            }
            else
            {
                if ( yw->_timeStamp - yw->_msgTimestampGates > 40000 )
                {
                    yw_arg159 arg159_2;
                    arg159_2.unit = 0;
                    arg159_2.Priority = 10;
                    arg159_2.txt = Locale::Text::Feedback(Locale::FEEDBACK_GATEFULL);
                    arg159_2.MsgID = 46;

                    yw->ypaworld_func159(&arg159_2);
                    yw->_msgTimestampGates = yw->_timeStamp;
                }
            }
        }
    }
}


bool NC_STACK_ypaworld::sub_4D11C0(int id, int owner)
{
    const TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    if ( sitem.PCell->owner != owner )
        return false;

    if ( sitem.KeySectors.empty() )
        return true;

    for ( const TMapKeySector &ks : sitem.KeySectors )
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
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    sitem.State = TMapSuperItem::STATE_INACTIVE;
    sitem.ActiveTime = 0;
    sitem.TriggerTime = 0;
    sitem.ActivateOwner = 0;
    sitem.CountDown = 0;

    ypaworld_arg148 arg148;
    arg148.owner = sitem.PCell->owner;
    arg148.blg_ID = sitem.InactiveBldID;
    arg148.field_C = 1;
    arg148.CellId = sitem.CellId;
    arg148.field_18 = 0;

    ypaworld_func148(&arg148);

    sitem.PCell->PurposeType = cellArea::PT_STOUDSON;
    sitem.PCell->PurposeIndex = id;

    yw_arg159 arg159;
    arg159.unit = NULL;
    arg159.Priority = 92;

    if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_BOMBDEACT);
        arg159.MsgID = 73;
    }
    else if ( sitem.Type == TMapSuperItem::TYPE_WAVE )
    {
        arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_WAVEDEACT);
        arg159.MsgID = 77;
    }
    else
    {
        arg159.MsgID = 0;
        arg159.txt = "Cant happen.";
    }

    ypaworld_func159(&arg159);
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub0(int id)
{
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    for( NC_STACK_ypabact * &unit : _unitsList )
    {
        if ( unit->_bact_type == BACT_TYPES_ROBO && sitem.ActivateOwner == unit->_owner )
        {
            if ( sub_4D11C0(id, sitem.ActivateOwner) )
            {
                if ( sitem.CountDown > 0 )
                    sitem.CountDown = sitem.CountDown - _frameTime;
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
    if ( yw->_GameShell )
    {
        TMapSuperItem &sitem = yw->_levelInfo.SuperItems[id];

        int v4 = sitem.CountDown / 1024;

        if ( v4 < 10 && v4 != sitem.LastSec )
        {
            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
            sitem.LastSec = v4;
        }

        int v5 = v4 / 10;
        if ( v5 != sitem.LastTenSec )
        {
            SFXEngine::SFXe.startSound(&yw->_GameShell->samples1_info, 3);
            sitem.LastTenSec = v5;
        }
    }
}

bool NC_STACK_ypaworld::sub_4D1230(int id, int a3)
{
    const TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    if ( sitem.PCell->owner == a3 )
        return false;

    if ( sitem.KeySectors.empty() )
        return true;

    for ( const TMapKeySector &ks : sitem.KeySectors )
    {
        if (ks.PCell->owner == a3)
            return false;
    }

    return true;
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub1(int id)
{
    const TMapSuperItem &sitem = _levelInfo.SuperItems[id];

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
    for(cellArea &cell : _cells)
    {
        for( NC_STACK_ypabact* &bct : cell.unitsList )
        {
            int v9 = 1;

            if ( _isNetGame )
            {
                if ( bct->_owner != _userUnit->_owner || bct->_owner == activate || bct->_status == BACT_STATUS_DEAD )
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
    TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    sitem.CurrentRadius = (_timeStamp - sitem.TriggerTime) * World::CVSectorLength / 2400.0;

    vec2d tmp = World::SectorIDToCenterPos2( sitem.CellId );

    float v19 = sqrt(POW2(_mapLength.x) + POW2(_mapLength.y));

    if ( sitem.CurrentRadius > 300 && sitem.CurrentRadius - sitem.LastRadius > 200 && sitem.CurrentRadius < v19 )
    {
        float v9 = (2 * sitem.CurrentRadius) * C_PI / 150.0;

        sitem.LastRadius = sitem.CurrentRadius;

        if ( v9 > 2.0 )
        {
            for (float v25 = 0.0; v25 < 6.283 ; v25 += 6.283 / v9 )
            {
                float v10 = sitem.CurrentRadius;
                float v26 = cos(v25) * v10 + tmp.x;
                float v21 = sin(v25) * v10 + tmp.y;

                if ( v26 > 600.0 && v21 < -600.0 && v26 < _mapLength.x - 600.0 && v21 > -(_mapLength.y - 600.0) )
                {
                    int v12 = _fxLimit;

                    _fxLimit = 2;

                    yw_arg129 arg129;
                    arg129.pos.x = v26;
                    arg129.pos.y = sitem.PCell->height;
                    arg129.pos.z = v21;
                    arg129.field_10 = 200000;
                    arg129.OwnerID = sitem.ActivateOwner;
                    arg129.unit = 0;

                    ypaworld_func129(&arg129);

                    _fxLimit = v12;
                }
            }
        }
    }

    ypaworld_func64__sub19__sub2__sub0__sub0(sitem.ActivateOwner, tmp.x, tmp.y, sitem.CurrentRadius);
}

void NC_STACK_ypaworld::ypaworld_func64__sub19__sub2(int id)
{
    const TMapSuperItem &sitem = _levelInfo.SuperItems[id];

    if ( !sub_4D1230(id, sitem.ActivateOwner) && sub_4D12A0(sitem.ActivateOwner) )
    {
        if ( sitem.Type == TMapSuperItem::TYPE_BOMB )
            ypaworld_func64__sub19__sub2__sub0(id);
    }
    else
    {
        sub_4D16C4(id);
    }
}

void NC_STACK_ypaworld::ypaworld_func64__sub19()
{
    for (size_t i = 0; i < _levelInfo.SuperItems.size(); i++)
    {
        const TMapSuperItem &sitem = _levelInfo.SuperItems[i];

        if ( sitem.Type != 0 )
        {
            switch ( sitem.State )
            {
            case TMapSuperItem::STATE_INACTIVE:
                if ( sub_4D11C0(i, sitem.PCell->owner) )
                {
                    if ( sub_4D12A0(sitem.PCell->owner) )
                        sub_4D12D8(i, 0);
                }
                break;

            case TMapSuperItem::STATE_ACTIVE:
                ypaworld_func64__sub19__sub0(i);
                ypaworld_func64__sub19__sub3(this, i);
                break;

            case TMapSuperItem::STATE_STOPPED:
                ypaworld_func64__sub19__sub1(i);
                break;

            case TMapSuperItem::STATE_TRIGGED:
                ypaworld_func64__sub19__sub2(i);
                break;

            default:
                break;
            }
        }
    }
}

void NC_STACK_ypaworld::VoiceMessageCalcPositionToUnit()
{
    if ( _voiceMessage.Unit == _userRobo )
    {
        _voiceMessage.Carrier.Position = _userUnit->_position;
    }
    else
    {
        vec3d tmp = _voiceMessage.Unit->_position - _userUnit->_position;

        float v11 = tmp.length();

        if ( v11 > 0.0 )
            tmp *= (100.0 / v11);

        _voiceMessage.Carrier.Position = _userUnit->_position + tmp;
    }
}

void NC_STACK_ypaworld::VoiceMessageUpdate()
{
    if ( _voiceMessage.Priority >= 0 )
    {
        if ( _voiceMessage.Unit->_status != BACT_STATUS_DEAD )
        {
            VoiceMessageCalcPositionToUnit();

            _voiceMessage.Carrier.Vector = _userUnit->_fly_dir * _userUnit->_fly_dir_length;
        }

        if ( _voiceMessage.Carrier.Sounds[0].IsEnabled() )
        {
            SFXEngine::SFXe.UpdateSoundCarrier(&_voiceMessage.Carrier);
        }
        else
            _voiceMessage.Reset();
    }
}

void ypaworld_func64__sub3(NC_STACK_ypaworld *yw)
{
    if ( yw->_userUnit->_pSector->owner != yw->_userRobo->_owner )
    {
        if ( yw->_userUnit->_pSector->owner )
        {
            if ( yw->_ownerOldCellUserUnit == yw->_userRobo->_owner || !yw->_ownerOldCellUserUnit )
            {
                if ( yw->_timeStamp - yw->_msgTimestampEnemySector > 10000 )
                {
                    yw_arg159 arg159;
                    arg159.unit = yw->_userUnit;
                    arg159.Priority = 24;
                    arg159.txt = Locale::Text::Feedback(Locale::FEEDBACK_ESECTENTER);
                    arg159.MsgID = 22;

                    yw->ypaworld_func159(&arg159);
                }

                yw->_msgTimestampEnemySector = yw->_timeStamp;
            }
        }
    }
}

void NC_STACK_ypaworld::ProfileCalcValues()
{
    _profileFramesCount++;

    if ( _profileFramesCount >= 5 )
    {
        if ( _profileVals[PFID_FPS] > 200 )
            _profileVals[PFID_FPS] = 0;

        for (size_t i = 0; i < PFID_MAX; i++)
        {
            if ( _profileVals[i] != 0 )
            {
                if ( _profileVals[i] < _profileMin[i] )
                    _profileMin[i] = _profileVals[i];

                if ( _profileVals[i] > _profileMax[i] )
                    _profileMax[i] = _profileVals[i];

                _profileTotal[i] += _profileVals[i];
            }
        }
    }
}

int NC_STACK_ypaworld::yw_RestoreVehicleData()
{
    std::string buf = fmt::sprintf("save:%s/%d.rst", _GameShell->UserName, _levelInfo.LevelID);

    ScriptParser::HandlersList parsers {
        new World::Parsers::VhclProtoParser(this),
        new World::Parsers::WeaponProtoParser(this),
        new World::Parsers::BuildProtoParser(this)
    };

    return ScriptParser::ParseFile(buf, parsers, 0);
}

void NC_STACK_ypaworld::EnableLevelPasses()
{
    if ( _levelInfo.State == TLevelInfo::STATE_COMPLETED )
    {
        TMapGate &gate = _levelInfo.Gates[ _levelInfo.GateCompleteID ];

        _globalMapRegions.MapRegions[ _levelInfo.LevelID ].Status = TMapRegionInfo::STATUS_COMPLETED;

        for (int lvl : gate.PassToLevels)
        {
            if ( _globalMapRegions.MapRegions[ lvl ].Status == TMapRegionInfo::STATUS_DISABLED )
                _globalMapRegions.MapRegions[ lvl ].Status = TMapRegionInfo::STATUS_ENABLED;
        }
    }
    else if ( _levelInfo.State == TLevelInfo::STATE_ABORTED && !yw_RestoreVehicleData() )
    {
        ypa_log_out("yw_RestoreVehicleData() failed.\n");
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
            for (World::TRoboGun &pgun : bct->_host_station->GetGuns())
            {
                if ( bct == pgun.gun_obj )
                    pgun.gun_obj = NULL;
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

        for (World::TRoboGun &gun : robo->GetGuns())
            gun.gun_obj = NULL;
    }

    bct->CleanAttackersTarget();
    NetReleaseMissiles(bct);

    bct->_status = BACT_STATUS_DEAD;

    ypaworld_func144(bct);

    bct->_status_flg |= BACT_STFLAG_DEATH1;
}

void NC_STACK_ypaworld::ProtosFreeSounds()
{
    SFXEngine::SFXe.StopPlayingSounds();

    for (World::TVhclProto &vhcl : _vhclProtos)
    {
        for (World::TVhclSound &sfx : vhcl.sndFX)
            sfx.ClearSounds();
    }

    for (World::TWeapProto &wep : _weaponProtos)
    {
        for (World::TVhclSound &fx : wep.sndFXes)
            fx.ClearSounds();
    }

    for (World::TBuildingProto &proto : _buildProtos)
        proto.SndFX.ClearSounds();
}


void NC_STACK_ypaworld::FFeedback_Update()
{
    NC_STACK_input *inpt = INPe.GetInput();
    
    if ( _shellConfIsParsed )
    {
        if ( _preferences & (World::PREF_JOYDISABLE | World::PREF_FFDISABLE) )
            return;
    }

    if ( _userUnit )
    {
        if ( _timeStamp - _ffTimeStamp > 250 )
        {
            _ffTimeStamp = _timeStamp;

            if ( _userUnit->_status == BACT_STATUS_DEAD )
            {
                if ( inpt )
                    inpt->ForceFeedback(NC_STACK_winp::FF_STATE_UPDATE, _ffEffectType);
            }
            else
            {
                if ( _ffEffectType != -1 )
                {
                    float a1 = POW2(_userUnit->_force) - POW2(_userUnit->_mass) * 100.0;

                    if (a1 < 0.0)
                        a1 = 0.0;

                    float v17 = fabs(_userUnit->_fly_dir_length) / ( sqrt(a1) / _userUnit->_airconst_static );
                    if ( v17 >= 1.0 )
                        v17 = 1.0;
                    else if (v17 < 0.0)
                        v17 = 0.0;

                    if ( inpt )
                        inpt->ForceFeedback(NC_STACK_winp::FF_STATE_UPDATE, _ffEffectType,
                                                        _ffMagnitude, _ffPeriod * v17);
                }
            }
        }

        TSoundSource *top = SFXEngine::SFXe.SndGetTopShake();
        if ( top )
        {
            if ( top->StartTime == SFXEngine::SFXe.currentTime )
            {
                float p1 = top->ShkMag;
                if ( p1 > 1.0 )
                    p1 = 1.0;

                vec3d tmp = top->PCarrier->Position - _userUnit->_position;

                float p2 = top->PShkFx->time;
                float p3 = _userUnit->_rotation.AxisX().dot( tmp );
                float p4 = -_userUnit->_rotation.AxisZ().dot( tmp );

                if ( p2 > 0.0 )
                {
                    if ( inpt )
                        inpt->ForceFeedback(NC_STACK_winp::FF_STATE_START, NC_STACK_winp::FF_TYPE_SHAKE,
                                                        p1, p2, p3, p4);
                }
            }
        }
    }
}

int recorder_startrec(NC_STACK_ypaworld *yw)
{
    recorder *rcrd = yw->_replayRecorder;

    rcrd->do_record = 0;
    rcrd->field_40 = 0;
    rcrd->seqn++;
    rcrd->level_id = yw->_levelInfo.LevelID;
    rcrd->frame_id = 0;
    rcrd->time = 0;
    rcrd->bacts_count = 0;
    rcrd->field_34 = 0;
    rcrd->ainf_size = 0;

    rcrd->mfile = IFFile::UAOpenIFFile(fmt::sprintf("env:snaps/m%02d%04d.raw", yw->_levelInfo.LevelID, rcrd->seqn), "wb");

    if ( !rcrd->mfile.OK() )
    {
        return 0;
    }

    rcrd->mfile.pushChunk(TAG_SEQN, TAG_FORM, -1);
    rcrd->mfile.pushChunk(0, TAG_SINF, 4);

    rcrd->mfile.writeU16L(rcrd->seqn);
    rcrd->mfile.writeU16L(rcrd->level_id);

    rcrd->mfile.popChunk();

    rcrd->do_record = 1;
    return 1;
}

void recorder_stoprec(NC_STACK_ypaworld *yw)
{
    recorder *rcrd = yw->_replayRecorder;
    rcrd->do_record = 0;

    rcrd->mfile.popChunk();

    rcrd->mfile.close();
}

void sb_0x447720(NC_STACK_ypaworld *yw, TInputState *inpt)
{
    if ( inpt->KbdLastHit == Input::KC_NUMMUL && (inpt->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD || yw->_easyCheatKeys) )
    {
        sub_4476AC(yw);

        yw_arg159 info_msg;
        info_msg.txt = "Screenshot saved.";
        info_msg.unit = NULL;
        info_msg.Priority = 100;
        info_msg.MsgID = 0;

        yw->ypaworld_func159(&info_msg);
    }


    if ( yw->_screenShotSeq )
    {
        if ( inpt->KbdLastHit == Input::KC_NUMDIV && (inpt->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD || yw->_easyCheatKeys) )
        {
            yw->_screenShotSeq = false;

            yw_arg159 info_msg;
            info_msg.txt = "Screenshotting: stopped.";
            info_msg.unit = NULL;
            info_msg.Priority = 100;
            info_msg.MsgID = 0;

            yw->ypaworld_func159(&info_msg);
        }

        GFX::Engine.SaveScreenshot( fmt::sprintf("env:snaps/s%d_%04d", yw->_screenShotSeqId, yw->_screenShotSeqFrame) );
        
        yw->_screenShotSeqFrame++;
    }
    else if ( inpt->KbdLastHit == Input::KC_NUMDIV && (inpt->ClickInf.flag & 0x100 || yw->_easyCheatKeys) )
    {
        yw->_screenShotSeqFrame = 0;
        yw->_screenShotSeq = true;
        yw->_screenShotSeqId++;

        yw_arg159 info_msg;
        info_msg.txt = "Screenshotting: started.";
        info_msg.unit = NULL;
        info_msg.Priority = 100;
        info_msg.MsgID = 0;

        yw->ypaworld_func159(&info_msg);
    }

    if ( yw->_replayRecorder->do_record )
    {
        if ( inpt->KbdLastHit == Input::KC_NUMMINUS && (inpt->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD || yw->_easyCheatKeys) )
        {
            recorder_stoprec(yw);

            yw_arg159 info_msg;
            info_msg.txt = "Replay recordering: stopped.";
            info_msg.unit = NULL;
            info_msg.Priority = 100;
            info_msg.MsgID = 0;

            yw->ypaworld_func159(&info_msg);
        }

    }
    else
    {
        if ( inpt->KbdLastHit == Input::KC_NUMMINUS && (inpt->ClickInf.flag & TClickBoxInf::FLAG_RM_HOLD || yw->_easyCheatKeys) )
        {
            recorder_startrec(yw);

            yw_arg159 info_msg;
            info_msg.txt = "Replay recordering: started.";
            info_msg.unit = NULL;
            info_msg.Priority = 100;
            info_msg.MsgID = 0;

            yw->ypaworld_func159(&info_msg);
        }
    }
}

void recorder_update_time(NC_STACK_ypaworld *yw, int dtime)
{
    yw->_replayRecorder->time += dtime;
    yw->_replayRecorder->field_40 -= dtime;
}


void NC_STACK_ypaworld::recorder_store_bact( recorder *rcrd, World::MissileList &bct_lst)
{
    for( NC_STACK_ypamissile * &bact : bct_lst )
    {
        if ( bact->_gid >= 0xFFFF || bact == _userRobo )
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
        if ( bact->_gid >= 0xFFFF || bact == _userRobo )
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

        NC_STACK_base *a4 = bact->GetVP();

        if ( a4 == bact->_vp_normal )
        {
            oinf->vp_id = 1;
        }
        else if ( a4 == bact->_vp_fire )
        {
            oinf->vp_id = 2;
        }
        else if ( a4 == bact->_vp_wait )
        {
            oinf->vp_id = 3;
        }
        else if ( a4 == bact->_vp_dead )
        {
            oinf->vp_id = 4;
        }
        else if ( a4 == bact->_vp_megadeth )
        {
            oinf->vp_id = 5;
        }
        else if ( a4 == bact->_vp_genesis )
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
            if (bact->_soundcarrier.Sounds[j].IsEnabled() || 
                bact->_soundcarrier.Sounds[j].IsPFxEnabled() || 
                bact->_soundcarrier.Sounds[j].IsShkEnabled())
                ssnd[0] |= 1 << j;
        }

        ssnd[1] = bact->_soundcarrier.Sounds[0].Pitch;
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
    recorder *rcrd = _replayRecorder;

    if ( rcrd->field_40 < 0 )
    {
        recorder_world_to_frame(rcrd);
        rcrd->ctrl_bact_id = _userUnit->_gid;
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
        rcrd->mfile.pushChunk(TAG_FRAM, TAG_FORM, frame_size);
        rcrd->mfile.pushChunk(0, TAG_FINF, 12);

        rcrd->mfile.writeS32L(rcrd->frame_id);
        rcrd->mfile.writeS32L(rcrd->time);
        rcrd->mfile.writeU32L(rcrd->ctrl_bact_id);

        rcrd->mfile.popChunk();

        if ( oinf_size )
        {
            rcrd->mfile.pushChunk(0, TAG_OINF, oinf_size);

            for (int i = 0; i < rcrd->bacts_count; i++)
            {
                trec_bct *oinf = &rcrd->oinf[i];

                rcrd->mfile.writeU32L(oinf->bact_id);
                TF::Engine.Vec3dWriteIFF(oinf->pos, &rcrd->mfile, false);
                rcrd->mfile.writeS8(oinf->rot_x);
                rcrd->mfile.writeS8(oinf->rot_y);
                rcrd->mfile.writeS8(oinf->rot_z);
                rcrd->mfile.writeU8(oinf->vp_id);
                rcrd->mfile.writeU8(oinf->objType);
                rcrd->mfile.writeU8(oinf->vhcl_id);
            }

            rcrd->mfile.popChunk();
        }

        if ( rcrd->ainf_size )
        {
            rcrd->mfile.pushChunk(0, TAG_AINF, rcrd->ainf_size);
            rcrd->mfile.write(rcrd->ainf, rcrd->ainf_size);
            rcrd->mfile.popChunk();
        }

        if ( v5 )
        {
            rcrd->mfile.pushChunk(0, TAG_MODE, v5);
            rcrd->mfile.write(rcrd->field_20, v5);
            rcrd->mfile.popChunk();
        }

        rcrd->mfile.popChunk();

        rcrd->field_34 = 0;
        rcrd->field_40 += 250;
        rcrd->frame_id += 1;
    }
}


int recorder_open_replay(recorder *rcrd)
{
    rcrd->mfile = IFFile( uaOpenFile(rcrd->filename, "rb") );

    if ( !rcrd->mfile.OK() )
    {
        return 0;
    }

    if ( rcrd->mfile.parse() != IFFile::IFF_ERR_OK )
    {
        rcrd->mfile.close();
        return 0;
    }

    if ( rcrd->mfile.GetCurrentChunk().Is(TAG_FORM, TAG_SEQN) )
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

    bacto->_soundcarrier.Clear();

    ypaworld_func134(bacto);

    bacto->setBACT_viewer(true);
    bacto->setBACT_inputting(true);

    _userRobo = bacto;

    TF::Engine.SetViewPoint(&bacto->_tForm);

    return true;
}



void recorder_read_framedata(recorder *rcrd)
{
    while ( rcrd->mfile.parse() != IFFile::IFF_ERR_EOC )
    {
        const IFFile::Context &v3 = rcrd->mfile.GetCurrentChunk();

        switch ( v3.TAG )
        {
        case TAG_FLSH:
            rcrd->field_78 |= 1;
            rcrd->mfile.parse();
            break;

        case TAG_FINF:
            rcrd->frame_id = rcrd->mfile.readS32L();
            rcrd->time = rcrd->mfile.readS32L();
            rcrd->ctrl_bact_id = rcrd->mfile.readU32L();
            rcrd->mfile.parse();
            break;

        case TAG_OINF:
        {
            rcrd->bacts_count = v3.TAG_SIZE / 22;

            for (int i = 0; i < rcrd->bacts_count; i++)
            {
                trec_bct *oinf = &rcrd->oinf[i];

                oinf->bact_id = rcrd->mfile.readU32L();
                TF::Engine.Vec3dReadIFF(&oinf->pos, &rcrd->mfile, false);
                oinf->rot_x = rcrd->mfile.readS8();
                oinf->rot_y = rcrd->mfile.readS8();
                oinf->rot_z = rcrd->mfile.readS8();

                oinf->vp_id = rcrd->mfile.readU8();
                oinf->objType = rcrd->mfile.readU8();
                oinf->vhcl_id = rcrd->mfile.readU8();
            }

            rcrd->mfile.parse();
        }
        break;

        case TAG_AINF:
            rcrd->mfile.read(rcrd->ainf, v3.TAG_SIZE);
            rcrd->ainf_size = v3.TAG_SIZE;

            recorder_unpack_soundstates(rcrd);

            rcrd->mfile.parse();
            break;

        case TAG_MODE:
            rcrd->mfile.read(rcrd->field_20, v3.TAG_SIZE);
            rcrd->field_34 = v3.TAG_SIZE / 16;

            rcrd->mfile.parse();
            break;

        default:
            rcrd->mfile.skipChunk();
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

            World::TVhclProto *prot = &_vhclProtos[ oinf->vhcl_id ];

            int v6 = prot->model_id;

            prot->model_id = BACT_TYPES_BACT;

            bacto = ypaworld_func146(&arg146);

            _vhclProtos[oinf->vhcl_id].model_id = v6;
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
        bacto->_host_station = (NC_STACK_yparobo *)_userUnit;
        bacto->_parent = _userUnit;
    }

    return bacto;
}

void NC_STACK_ypaworld::recorder_set_bact_pos(NC_STACK_ypabact *bact, const vec3d &pos)
{
    yw_130arg arg130;
    arg130.pos_x = pos.x;
    arg130.pos_z = pos.z;

    if ( GetSectorInfo(&arg130) )
    {
        if ( bact->_pSector )
            bact->_cellRef.Detach();

        bact->_cellRef = arg130.pcell->unitsList.push_back(bact);

        bact->_pSector = arg130.pcell;
        bact->_old_pos = bact->_position;
        bact->_position = pos;
        bact->_cellId = arg130.CellId;
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

    switch ( oinf->vp_id )
    {
    case 1:
        bact->SetVP(bact->_vp_normal);
        break;

    case 2:
        bact->SetVP(bact->_vp_fire);
        break;

    case 3:
        bact->SetVP(bact->_vp_wait);
        break;

    case 4:
        bact->SetVP(bact->_vp_dead);
        break;

    case 5:
        bact->SetVP(bact->_vp_megadeth);
        break;

    case 6:
        bact->SetVP(bact->_vp_genesis);
        break;

    default:
        break;
    }

    bact->_soundcarrier.Sounds[0].Pitch = ssnd[1];

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

                if ( bact->_soundcarrier.Sounds[i].IsLoop() )
                    SFXEngine::SFXe.sub_424000(&bact->_soundcarrier, i);
            }
        }
    }
}


void NC_STACK_ypaworld::recorder_updateObjectList(recorder *rcrd, float a5, int period)
{
    float fperiod = period / 1000.0;
    World::RefBactList::iterator it = _userUnit->_kidList.begin();

    int i = 0;

    while ( i < rcrd->bacts_count )
    {
        trec_bct *oinf = &rcrd->oinf[i];
        uint16_t *ssnd = &rcrd->sound_status[2 * i];

        if ( it != _userUnit->_kidList.end() )
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
                    
                    v10->_kidRef = _userUnit->_kidList.insert(it, v10);

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

                v13->_kidRef = _userUnit->_kidList.push_back(v13);
                it = v13->_kidRef;
                it++;

                i++;
            }
        }
    }

    while ( it != _userUnit->_kidList.end() )
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

    rcrd->mfile.close();

    if ( recorder_open_replay(rcrd) )
    {
        while ( rcrd->mfile.parse() != IFFile::IFF_ERR_EOC )
        {
            if ( rcrd->mfile.GetCurrentChunk().Is(TAG_FORM, TAG_FRAM) )
            {
                if ( cur_frame_id == frame_id )
                {
                    recorder_read_framedata(rcrd);

                    _timeStamp = rcrd->time;

                    recorder_updateObjectList(rcrd, 1.0, 0);
                    return 1;
                }

                cur_frame_id++;
                rcrd->mfile.skipChunk();
            }
            else
            {
                rcrd->mfile.skipChunk();
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

        while ( rcrd->field_74 - 1 != rcrd->frame_id  &&  (int32_t)(dTime + _timeStamp) > rcrd->time )
        {
            if ( rcrd->mfile.parse() != IFFile::IFF_ERR_EOF )
            {
                if ( rcrd->mfile.GetCurrentChunk().Is(TAG_FORM, TAG_FRAM) )
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
                _timeStamp = rcrd->time;
                recorder_updateObjectList(rcrd, 1.0, dTime);
            }
            else
            {
                float v9 = (float)dTime / (float)(rcrd->time - _timeStamp);

                _timeStamp += dTime;

                recorder_updateObjectList(rcrd, v9, dTime);
            }
        }
    }
}

void ypaworld_func163__sub2__sub1(NC_STACK_ypaworld *yw, float fperiod, TInputState *inpt)
{
    recorder *rcrd = yw->_replayPlayer;

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

void ypaworld_func163__sub2__sub0(NC_STACK_ypaworld *yw, float fperiod, TInputState *inpt)
{
    float v3 = inpt->Sliders[10] * 2.5 * fperiod;

    if ( fabs(v3) > 0.001 )
        yw->_replayPlayer->rotation_matrix = mat3x3::RotateY(-v3) * yw->_replayPlayer->rotation_matrix;

    float v5 = inpt->Sliders[11] * 2.5 * fperiod;

    if ( fabs(v5) > 0.001 )
    {
        yw->_replayPlayer->rotation_matrix = mat3x3::RotateX(v5) * yw->_replayPlayer->rotation_matrix;
    }
}

void NC_STACK_ypaworld::CameraPrepareRender(recorder *rcrd, NC_STACK_ypabact *bact, TInputState *inpt)
{
    extern tehMap robo_map;
    extern squadMan squadron_manager;

    float fperiod = inpt->Period / 1000.0;

    if ( inpt->ClickInf.flag & TClickBoxInf::FLAG_RM_DOWN )
    {
        
        if ( _mouseGrabbed )
            _mouseGrabbed = false;
        else if ( inpt->ClickInf.selected_btn != &robo_map  &&  inpt->ClickInf.selected_btn != &squadron_manager )
            _mouseGrabbed = true;
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
        for ( NC_STACK_ypabact * &unit : _userRobo->GetKidList() )
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
        for ( NC_STACK_ypabact * &unit : _userRobo->GetKidList() )
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

    bact->_tForm.Pos = bact->_position;
    bact->_tForm.SclRot = bact->_rotation;
}

char *sub_445654(NC_STACK_ypaworld *yw, char *in, char *buf, const char *fmt, ...)
{
    char *cur = in;

    FontUA::copy_position(&cur);

    va_list va;
    va_start(va, fmt);

    vsprintf(buf, fmt, va);

    va_end(va);

    FontUA::add_txt(&cur, yw->_screenSize.x, 1, buf);

    return cur;
}

void NC_STACK_ypaworld::debug_count_units()
{
    for (int i = 0; i < 8; i++)
    {
        _dbgSquadCounter[i] = 0;
        _dbgVehicleCounter[i] = 0;
        _dbgFlakCounter[i] = 0;
        _dbgRoboCounter[i] = 0;
        _dbgWeaponCounter[i] = 0;
    }

    _dbgTotalSquadCount = 0;
    _dbgTotalVehicleCount = 0;
    _dbgTotalFlakCount = 0;
    _dbgTotalWeaponCount = 0;
    _dbgTotalRoboCount = 0;

    for ( NC_STACK_ypabact * &robo : _unitsList )
    {
        _dbgRoboCounter[ robo->_owner ]++;

        if ( robo->_owner )
        {
            for ( NC_STACK_ypabact * &commander : robo->_kidList )
            {
                bool v5 = false;

                if ( commander->_bact_type == BACT_TYPES_GUN )
                {
                    v5 = true;
                    _dbgFlakCounter[ commander->_owner ]++;
                }
                else
                {
                    _dbgSquadCounter[ commander->_owner ]++;
                    _dbgVehicleCounter[ commander->_owner ]++;
                }

                _dbgWeaponCounter[ commander->_owner ] += commander->_missiles_list.size();

                for ( NC_STACK_ypabact * &unit : commander->_kidList )
                {
                    if ( v5 )
                        _dbgFlakCounter[ unit->_owner ]++;
                    else
                        _dbgVehicleCounter[ commander->_owner ]++;


                    _dbgWeaponCounter[ commander->_owner ] += unit->_missiles_list.size();
                }
            }
        }
    }

    for (int i = 0; i < 8; i++)
    {
        _dbgTotalSquadCount  += _dbgSquadCounter[i];
        _dbgTotalVehicleCount += _dbgVehicleCounter[i];
        _dbgTotalFlakCount  += _dbgFlakCounter[i];
        _dbgTotalWeaponCount  += _dbgWeaponCounter[i];
        _dbgTotalRoboCount += _dbgRoboCounter[i];
    }

    if ( _dbgTotalSquadCount > _dbgTotalSquadCountMax )
        _dbgTotalSquadCountMax = _dbgTotalSquadCount;

    if ( _dbgTotalVehicleCount > _dbgTotalVehicleCountMax )
        _dbgTotalVehicleCountMax = _dbgTotalVehicleCount;

    if ( _dbgTotalFlakCount > _dbgTotalFlakCountMax )
        _dbgTotalFlakCountMax = _dbgTotalFlakCount;

    if ( _dbgTotalWeaponCount > _dbgTotalWeaponCountMax )
        _dbgTotalWeaponCountMax = _dbgTotalWeaponCount;

    if ( _dbgTotalRoboCount > _dbgTotalRoboCountMax )
        _dbgTotalRoboCountMax = _dbgTotalRoboCount;
}

void NC_STACK_ypaworld::debug_info_draw(TInputState *inpt)
{
    if ( _showDebugMode == 0)
    {
        if ( sub_449678(inpt, Input::KC_F9) )
            _showDebugMode++;
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

        if ( _showDebugMode == 1 )
        {
            debug_count_units();

            if ( !_buildDate.empty() )
            {
                cmd = sub_445654(this, cmd, buf_sprintf, "build id: %s", _buildDate.c_str());

                FontUA::next_line(&cmd);
            }

            int this_time = _timeStamp / 1024;
            int all_time;

            if ( _isNetGame )
                all_time = 0;
            else
                all_time = (_timeStamp + _playersStats[1].ElapsedTime) / 1024;

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

            cmd = sub_445654(this, cmd, buf_sprintf, "prof all: %d", _profileVals[PFID_FRAMETIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof fprint: %d", _profileVals[PFID_MARKTIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof gui: %d", _profileVals[PFID_GUITIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof ai: %d", _profileVals[PFID_UPDATETIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof rend: %d", _profileVals[PFID_RENDERTIME]);
            FontUA::next_line(&cmd);
            
            cmd = sub_445654(this, cmd, buf_sprintf, "prof 2d rend: %d", _profileVals[PFID_NEWGUITIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "prof net: %d", _profileVals[PFID_NETTIME]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "sec type/wtype: %d/%d", _userUnit->_pSector->type_id, _userUnit->_pSector->PurposeType);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "beam energy: %d", _beamEnergyCapacity);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num sqd: %d,%d", _dbgTotalSquadCount, _dbgTotalSquadCountMax);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num vhcl: %d,%d", _dbgTotalVehicleCount, _dbgTotalVehicleCountMax);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num flk: %d,%d", _dbgTotalFlakCount, _dbgTotalFlakCountMax);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num robo: %d,%d", _dbgTotalRoboCount, _dbgTotalRoboCountMax);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "num wpn: %d,%d", _dbgTotalWeaponCount, _dbgTotalWeaponCountMax);
            FontUA::next_line(&cmd);

            cmd = sub_445654(this, cmd, buf_sprintf, "reload const: %d", _userRobo->_reload_const);
            FontUA::next_line(&cmd);

            cmd = sub_445654(
                      this,
                      cmd,
                      buf_sprintf,
                      "num all vhcl: %d,%d,%d,%d,%d,%d,%d,%d",
                      _countUnitsPerOwner[0],
                      _countUnitsPerOwner[1],
                      _countUnitsPerOwner[2],
                      _countUnitsPerOwner[3],
                      _countUnitsPerOwner[4],
                      _countUnitsPerOwner[5],
                      _countUnitsPerOwner[6],
                      _countUnitsPerOwner[7]);
            FontUA::next_line(&cmd);

            cmd = sub_445654(
                      this,
                      cmd,
                      buf_sprintf,
                      "rld ratio: %8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f",
                      _reloadRatioPositive[0],
                      _reloadRatioPositive[1],
                      _reloadRatioPositive[2],
                      _reloadRatioPositive[3],
                      _reloadRatioPositive[4],
                      _reloadRatioPositive[5],
                      _reloadRatioPositive[6],
                      _reloadRatioPositive[7]);
            FontUA::next_line(&cmd);

            if ( _invulnerable )
                cmd = sub_445654(this, cmd, buf_sprintf, "invulnerable: %s", "YES");
            else
                cmd = sub_445654(this, cmd, buf_sprintf, "invulnerable: %s", "NO");
            FontUA::next_line(&cmd);
        }
        else if ( _showDebugMode == 2 )
        {
            if ( _GameShell && _isNetGame )
            {
                FontUA::next_line(&cmd);
                FontUA::next_line(&cmd);

                for (UserData::TNetPlayerData &pl : _GameShell->netPlayers)
                {
                    if ( pl.Status )
                    {
                        const char *v35;
                        const char *v36;

                        switch ( pl.Owner )
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

                        switch ( pl.Status )
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

                        cmd = sub_445654(this, cmd, buf_sprintf, "%s status: %s latency: %d", v35, v36, pl.Latency);

                        FontUA::next_line(&cmd);
                    }
                }

                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "net send: %d bytes/sec", _GameShell->netsend_speed);
                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "net rcv: %d bytes/sec", _GameShell->netrecv_speed);
                FontUA::next_line(&cmd);

                cmd = sub_445654(this, cmd, buf_sprintf, "packet: %d bytes", _GameShell->net_packet_size);
                FontUA::next_line(&cmd);

                if ( _netInfoOverkill )
                    cmd = sub_445654(this, cmd, buf_sprintf, "WARNING: INFO OVERKILL");

                FontUA::next_line(&cmd);

                if ( _netDriver )
                {
                    int v100[7];
                    _netDriver->GetStats(v100);

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
        else if ( _showDebugMode == 3 )
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

                    if ( _showDebugMode - 3 <= v110 )
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

        cmd = sub_445654(this, cmd, buf_sprintf, "fps: %d", _FPS);
        FontUA::next_line(&cmd);

        cmd = sub_445654(this, cmd, buf_sprintf, "polys: %d,%d", _polysCount, _polysDraw);
        FontUA::next_line(&cmd);

        FontUA::set_end(&cmd);

        w3d_a209 arg209;
        arg209.cmdbuf = dbg_txt;
        arg209.includ = 0;

        GFX::Engine.raster_func209(&arg209);


        if ( v104 )
        {
            _showDebugMode = 0;
        }
        else
        {
            if ( sub_449678(inpt, Input::KC_F9) )
                _showDebugMode++;
        }
    }
}





void NC_STACK_ypaworld::HistoryAktCreate(NC_STACK_ypabact *bact)
{
    HistoryEventAdd( World::History::VhclCreate(bact->_owner, bact->_vehicleID, bact->_position.x * 256.0 / bact->_wrldSize.x, bact->_position.z * 256.0 / bact->_wrldSize.y) );
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

        HistoryEventAdd( World::History::VhclKill(owners, vp, bact->_position.x * 256.0 / bact->_wrldSize.x, bact->_position.z * 256.0 / bact->_wrldSize.y) );
    }
}
