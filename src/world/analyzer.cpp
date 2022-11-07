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
    NC_STACK_ypabact *hs = world->_userRobo;
            
    for ( int sY = -1; sY <= 1; sY++ )
    {
        for ( int sX = -1; sX <= 1; sX++ )
        {
            cellArea *s = world->GetSector( hs->_cellId + Common::Point(sX, sY) );
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
    if (world->_userRobo->_energy < world->_userRobo->_energy_max / 8)
        return true;
    return false;
}
    
bool GameAnalyzer::IsHasBackupPS(NC_STACK_ypaworld *world)
{
    for(const auto it : world->_powerStations)
    {
        if (it.second.pCell)
        {
            if (it.second.pCell->owner == world->_userRobo->_owner)
            {
                if ( Common::ABS(it.second.CellId.x - world->_userRobo->_cellId.x) >= 4
                  || Common::ABS(it.second.CellId.y - world->_userRobo->_cellId.y) >= 4)
                    return true;
            }
        }
    }
    return false;
}
    
bool GameAnalyzer::IsAnySquadInFight(NC_STACK_ypaworld *world)
{
    for (NC_STACK_ypabact* const bact : world->_cmdrsRemap) 
    {
        if (bact->_secndTtype == BACT_TGT_TYPE_UNIT) 
            return true;
    }
    return false;
}
    
bool GameAnalyzer::IsOwnPowerStation(NC_STACK_ypaworld *world)
{
    for(const auto it : world->_powerStations)
    {
        if (it.second.pCell)
        {
            if (it.second.pCell->owner == world->_userRobo->_owner)
                return true;
        }
    }
    return false;
}
    
bool GameAnalyzer::IsOnHighPowerStation(NC_STACK_ypaworld *world)
{
    const TPowerStationInfo *maxPwr = NULL;
    for(const auto it : world->_powerStations)
    {
        if (it.second.pCell)
        {
            if (it.second.pCell->owner == world->_userRobo->_owner
             && (!maxPwr || maxPwr->EffectivePower < it.second.EffectivePower) )
                maxPwr = &it.second;
        }
    }
    
    if (maxPwr && world->_userRobo->_pSector == maxPwr->pCell)
        return true;
    
    return false;
}
    
bool GameAnalyzer::IsPowerEffLow(NC_STACK_ypaworld *world)
{
    if (world->_reloadRatioClamped[ world->_userRobo->_owner ] < 0.9)
        return true;
    return false;
}
    
bool GameAnalyzer::IsSpottedEnemyPS(NC_STACK_ypaworld *world)
{
    for(const auto it : world->_powerStations)
    {
        if (it.second.pCell)
        {
            if (it.second.pCell->owner != world->_userRobo->_owner &&
                it.second.pCell->IsCanSee(world->_userRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsCanBuildPowerStation(NC_STACK_ypaworld *world)
{
    int32_t enrg = ((NC_STACK_yparobo *)world->_userRobo)->getROBO_battVehicle();
    for(int i = 0; i < bzda.field_8E4; i++)
    {
        TBuildingProto &proto = world->_buildProtos.at( bzda.field_3DC[i] );
        if (proto.ModelID == 1 && enrg >= proto.Energy)
            return true;            
    }
    return false;
}

bool GameAnalyzer::IsSpottedTek(NC_STACK_ypaworld *world)
{
    for (const TMapGem &gem : world->_techUpgrades)
    {
        cellArea *cell = world->GetSector(gem.CellId);
        if ( cell && cell->PurposeType == cellArea::PT_TECHUPGRADE )
        {
            if ( world->_userRobo->_owner != cell->owner 
              && cell->IsCanSee(world->_userRobo->_owner) )
                return true;
        }
    }
    return false;
}
    
bool GameAnalyzer::IsSpottedBeamGate(NC_STACK_ypaworld *world)
{
    for (const TMapGate &gate : world->_levelInfo.Gates)
    {
        if ( gate.PCell->PurposeType == cellArea::PT_GATECLOSED )
        {
            if ( world->_userRobo->_owner != gate.PCell->owner 
              && gate.PCell->IsCanSee(world->_userRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsSpottedKeySector(NC_STACK_ypaworld *world)
{
    for (const TMapGate &gate : world->_levelInfo.Gates)
    {
        for( const TMapKeySector &ks : gate.KeySectors )
        {
            if ( world->_userRobo->_owner != ks.PCell->owner 
              && ks.PCell->IsCanSee(world->_userRobo->_owner) )
                return true;
        }
    }
    return false;
}

bool GameAnalyzer::IsSpottedEnemyHS(NC_STACK_ypaworld *world)
{
    for (const NC_STACK_ypabact *hs : world->_unitsList)
    {
        if ( hs != world->_userRobo
         &&  hs->_bact_type == BACT_TYPES_ROBO
         &&  hs->_owner != world->_userRobo->_owner
         &&  hs->_status != BACT_STATUS_DEAD
         &&  hs->_pSector->IsCanSee( world->_userRobo->_owner) )
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
        if ( hs != world->_userRobo
         &&  hs->_bact_type == BACT_TYPES_ROBO
         &&  hs->_owner != world->_userRobo->_owner
         &&  hs->_status != BACT_STATUS_DEAD
         &&  hs->_pSector->IsCanSee( world->_userRobo->_owner ) )
        {
            for(const auto it : world->_powerStations)
            {
                if (it.second.pCell && it.second.pCell->owner == hs->_owner 
                 && it.second.pCell->IsCanSee(world->_userRobo->_owner))
                {
                    if (Common::ABS(it.second.CellId.x - hs->_cellId.x) < 3
                     || Common::ABS(it.second.CellId.y - hs->_cellId.y) < 3)
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
        if (cell.IsCanSee(world->_userRobo->_owner))
            see++;
    }
    
    float prc = ((float)see) / ((float)(world->_mapSize.x - 2) * (world->_mapSize.y - 2));
    if (prc < 0.75)
        return true;
    return false;        
}

bool GameAnalyzer::IsFewOwnSectors(NC_STACK_ypaworld *world)
{
    if (world->_countSectorsPerOwner[world->_userRobo->_owner] <= 0)
        return true;
    
    int32_t see = 0;
    for( const cellArea &cell : world->_cells )
    {
        if (cell.IsCanSee(world->_userRobo->_owner))
            see++;
    }

    float prc = ((float)see) / ((float)world->_countSectorsPerOwner[world->_userRobo->_owner]);
    if (prc < 0.75)
        return true;
    return false; 
}

bool GameAnalyzer::IsReadyToBeam(NC_STACK_ypaworld *world)
{
    for (const TMapGate &gate : world->_levelInfo.Gates)
    {
        if ( gate.PCell->PurposeType == cellArea::PT_GATEOPENED )
        {
            return true;
        }
    }
    return false;
}


}