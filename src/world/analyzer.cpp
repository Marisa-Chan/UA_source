#include "analyzer.h"
#include "../yw.h"
#include "../yparobo.h"
#include "../yw_internal.h"

extern bzd bzda;

namespace World
{

std::string GameAnalyzer::Analyze(NC_STACK_ypaworld *world)
{
    if (IsHSAttacked(world)) 
    {
        if (IsHSLowHP(world)) 
        {
            if (IsHasBackupPS(world)) 
                return world->GetLocaleString(702, "ADVICE #3: DEFEND HOSTSTATION");
            else 
                return world->GetLocaleString(700, "ADVICE #1: ESCAPE TO ANOTHER POWER STATION");
        } 
        else 
        {
            return world->GetLocaleString(701, "ADVICE #2: YOUR SCREWED");
        }
    } 
    else if (IsAnySquadInFight(world))
    {
        return world->GetLocaleString(703, "ADVICE #4: SEND REINFORMCEMENT OR HELP BY HAND");
    }
    else if (IsOwnPowerStation(world) && IsOnHighPowerStation(world) && IsPowerEffLow(world))
    {
        return world->GetLocaleString(704, "ADVICE #5: CONQUER SECTORS TO BUMP UP EFFICIENCY");
    } 
    else if (IsOwnPowerStation(world) && !IsOnHighPowerStation(world))
    {
        return world->GetLocaleString(705, "ADVICE #6: BEAM TO YOUR STRONGEST POWER STATION");
    } 
    else if (IsSpottedEnemyPS(world))
    {
        return world->GetLocaleString(706, "ADVICE #7: CONQUER ENEMY POWER STATION");
    }
    else if (!IsOwnPowerStation(world) && IsCanBuildPowerStation(world)) 
    {
        return world->GetLocaleString(707, "ADVICE #8: BUILD A POWER STATION");
    } 
    else if (IsSpottedTek(world))
    {
        return world->GetLocaleString(708, "ADVICE #9: CONQUER TECH UPGRADE");
    }
    else if (IsSpottedBeamGate(world))
    {
        return world->GetLocaleString(709, "ADVICE #10: CONQUER BEAM GATE");
    }
    else if (IsSpottedKeySector(world))
    {
        return world->GetLocaleString(710, "ADVICE #11: CONQUER KEY SECTORS TO OPEN BEAM GATE");
    }
    else if (IsSpottedEnemyHS(world)) 
    {
        if (IsSpottedEnemyHSwithPS(world)) 
        {
            return world->GetLocaleString(711, "ADVICE #12: COLLECT FORCES FOR POWER STATION ATTACK");
        } 
        else 
        {
            return world->GetLocaleString(712, "ADVICE #13: COLLECT FORCES FOR ROBO ATTACK");
        }
    } 
    else if (IsFewSeeSectors(world))
    {
        return world->GetLocaleString(713, "ADVICE #14: EXPLORE LAND");
    }
    else if (IsFewOwnSectors(world))
    {
        return world->GetLocaleString(714, "ADVICE #15: CONQUER AND STABILIZE LAND");
    }
    else if (IsReadyToBeam(world))
    {
        return world->GetLocaleString(715, "ADVICE #16: PLACE FORCES ONTO BEAM GATE AND LEAVE LEVEL");
    }
    
    return world->GetLocaleString(716, "ADVICE #17: NO PROBLEM IDENTIFIED");
}

bool GameAnalyzer::IsHSAttacked(NC_STACK_ypaworld *world)
{
    NC_STACK_ypabact *hs = world->UserRobo;
            
    for ( int sY = -1; sY <= 1; sY++ )
    {
        for ( int sX = -1; sX <= 1; sX++ )
        {
            cellArea *s = world->GetSector(hs->_sectX + sX, hs->_sectY + sY);
            if (s)
            {
                for (NC_STACK_ypabact *bact : s->unitsList)
                {
                    if (bact->_status != BACT_STATUS_CREATE
                     && bact->_status != BACT_STATUS_DEAD
                     && bact->_bact_type != BACT_TYPES_UFO
                     && bact->_owner != hs->_owner)
                        return true;
                }
            }
        }
    }
    return false;
}
    
bool GameAnalyzer::IsHSLowHP(NC_STACK_ypaworld *world)
{
    if (world->UserRobo->_energy < world->UserRobo->_energy_max / 8)
        return true;
    return false;
}
    
bool GameAnalyzer::IsHasBackupPS(NC_STACK_ypaworld *world)
{
    for(const PowerStationRef &ps : world->_powerStations)
    {
        if (ps.pCell)
        {
            if (ps.pCell->owner == world->UserRobo->_owner)
            {
                if ( Common::ABS(ps.Cell.x - world->UserRobo->_sectX) >= 4
                  || Common::ABS(ps.Cell.y - world->UserRobo->_sectY) >= 4)
                    return true;
            }
        }
    }
    return false;
}
    
bool GameAnalyzer::IsAnySquadInFight(NC_STACK_ypaworld *world)
{
    for (int i = 0; i < world->_cmdrsCount; i++) 
    {
        if (world->_cmdrsRemap[i]->_secndTtype == BACT_TGT_TYPE_UNIT) 
            return true;
    }
    return false;
}
    
bool GameAnalyzer::IsOwnPowerStation(NC_STACK_ypaworld *world)
{
    for(const PowerStationRef &ps : world->_powerStations)
    {
        if (ps.pCell)
        {
            if (ps.pCell->owner == world->UserRobo->_owner)
                return true;
        }
    }
    return false;
}
    
bool GameAnalyzer::IsOnHighPowerStation(NC_STACK_ypaworld *world)
{
    const PowerStationRef *maxPwr = NULL;
    for(const PowerStationRef &ps : world->_powerStations)
    {
        if (ps.pCell)
        {
            if (ps.pCell->owner == world->UserRobo->_owner
             && (!maxPwr || maxPwr->EffectivePower < ps.EffectivePower) )
                maxPwr = &ps;
        }
    }
    
    if (maxPwr && world->UserRobo->_pSector == maxPwr->pCell)
        return true;
    
    return false;
}
    
bool GameAnalyzer::IsPowerEffLow(NC_STACK_ypaworld *world)
{
    if (world->field_1bcc[ world->UserRobo->_owner ] < 0.9)
        return true;
    return false;
}
    
bool GameAnalyzer::IsSpottedEnemyPS(NC_STACK_ypaworld *world)
{
    for(const PowerStationRef &ps : world->_powerStations)
    {
        if (ps.pCell)
        {
            if (ps.pCell->owner != world->UserRobo->_owner &&
                ps.pCell->IsCanSee(world->UserRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsCanBuildPowerStation(NC_STACK_ypaworld *world)
{
    int32_t enrg = ((NC_STACK_yparobo *)world->UserRobo)->getROBO_battVehicle();
    for(int i = 0; i < bzda.field_8E4; i++)
    {
        TBuildingProto &proto = world->BuildProtos.at( bzda.field_3DC[i] );
        if (proto.ModelID == 1 && enrg >= proto.Energy)
            return true;            
    }
    return false;
}

bool GameAnalyzer::IsSpottedTek(NC_STACK_ypaworld *world)
{
    for (const MapGem &gem : world->_Gems)
    {
        cellArea *cell = world->GetSector(gem);
        if ( cell && cell->w_type == 4 )
        {
            if ( world->UserRobo->_owner != cell->owner 
              && cell->IsCanSee(world->UserRobo->_owner) )
                return true;
        }
    }
    return false;
}
    
bool GameAnalyzer::IsSpottedBeamGate(NC_STACK_ypaworld *world)
{
    for (const MapGate &gate : world->_levelInfo->Gates)
    {
        if ( gate.PCell->w_type == 5 )
        {
            if ( world->UserRobo->_owner != gate.PCell->owner 
              && gate.PCell->IsCanSee(world->UserRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsSpottedKeySector(NC_STACK_ypaworld *world)
{
    for (const MapGate &gate : world->_levelInfo->Gates)
    {
        for( const MapKeySector &ks : gate.KeySectors )
        {
            if ( world->UserRobo->_owner != ks.PCell->owner 
              && ks.PCell->IsCanSee(world->UserRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsSpottedEnemyHS(NC_STACK_ypaworld *world)
{
    for (const NC_STACK_ypabact *hs : world->_unitsList)
    {
        if ( hs != world->UserRobo
         &&  hs->_bact_type == BACT_TYPES_ROBO
         &&  hs->_owner != world->UserRobo->_owner
         &&  hs->_status != BACT_STATUS_DEAD
         &&  hs->_pSector->IsCanSee( world->UserRobo->_owner) )
        {
            return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsSpottedEnemyHSwithPS(NC_STACK_ypaworld *world)
{
    for (const NC_STACK_ypabact *hs : world->_unitsList)
    {
        if ( hs != world->UserRobo
         &&  hs->_bact_type == BACT_TYPES_ROBO
         &&  hs->_owner != world->UserRobo->_owner
         &&  hs->_status != BACT_STATUS_DEAD
         &&  hs->_pSector->IsCanSee( world->UserRobo->_owner ) )
        {
            for(const PowerStationRef &ps : world->_powerStations)
            {
                if (ps.pCell && ps.pCell->owner == hs->_owner 
                 && ps.pCell->IsCanSee(world->UserRobo->_owner))
                {
                    if (Common::ABS(ps.Cell.x - hs->_sectX) < 3
                     || Common::ABS(ps.Cell.y - hs->_sectY) < 3)
                        return true;
                }
            }
        }
    }
    return false;
}

bool GameAnalyzer::IsFewSeeSectors(NC_STACK_ypaworld *world)
{
    int32_t see = 0;
    for( const cellArea &cell : world->_cells )
    {
        if (cell.IsCanSee(world->UserRobo->_owner))
            see++;
    }
    
    float prc = ((float)see) / ((float)(world->_mapWidth - 2) * (world->_mapHeight - 2));
    if (prc < 0.75)
        return true;
    return false;        
}

bool GameAnalyzer::IsFewOwnSectors(NC_STACK_ypaworld *world)
{
    if (world->sectors_count_by_owner[world->UserRobo->_owner] <= 0)
        return true;
    
    int32_t see = 0;
    for( const cellArea &cell : world->_cells )
    {
        if (cell.IsCanSee(world->UserRobo->_owner))
            see++;
    }

    float prc = ((float)see) / ((float)world->sectors_count_by_owner[world->UserRobo->_owner]);
    if (prc < 0.75)
        return true;
    return false; 
}

bool GameAnalyzer::IsReadyToBeam(NC_STACK_ypaworld *world)
{
    for (const MapGate &gate : world->_levelInfo->Gates)
    {
        if ( gate.PCell->w_type == 6 )
        {
            return true;
        }
    }
    return false;
}


}