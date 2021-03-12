#ifndef SITUATION_ANALYZER_H
#define SITUATION_ANALYZER_H

#include <string>

class NC_STACK_ypaworld;

namespace World
{

// Static method class
class GameAnalyzer
{
public:
    static std::string Analyze(NC_STACK_ypaworld *world);
    
protected:
    // Is host station attacked by enemy
    static bool IsHSAttacked(NC_STACK_ypaworld *world);
    
    // Is host station HP low
    static bool IsHSLowHP(NC_STACK_ypaworld *world);
    
    // If has any backup power station
    static bool IsHasBackupPS(NC_STACK_ypaworld *world);
    
    // Is any squad in figt with enemy units
    static bool IsAnySquadInFight(NC_STACK_ypaworld *world);
    
    // Is own power station
    static bool IsOwnPowerStation(NC_STACK_ypaworld *world);
    
    // Is on highest power station
    static bool IsOnHighPowerStation(NC_STACK_ypaworld *world);

    // Is power effectivity low
    static bool IsPowerEffLow(NC_STACK_ypaworld *world);
    
    // Is enemy power station spotted
    static bool IsSpottedEnemyPS(NC_STACK_ypaworld *world);
    
    // Is can build power station
    static bool IsCanBuildPowerStation(NC_STACK_ypaworld *world);
    
    // Is tech upgrades spotted
    static bool IsSpottedTek(NC_STACK_ypaworld *world);
    
    // Is beam gate spotted
    static bool IsSpottedBeamGate(NC_STACK_ypaworld *world);
    
    // Is key sector spotted
    static bool IsSpottedKeySector(NC_STACK_ypaworld *world);
    
    // Is enemy HS spotted
    static bool IsSpottedEnemyHS(NC_STACK_ypaworld *world);
    
    // Is enemy HS spotted with PS
    static bool IsSpottedEnemyHSwithPS(NC_STACK_ypaworld *world);
    
    // Is see too few sectors
    static bool IsFewSeeSectors(NC_STACK_ypaworld *world);
    
    // Is own too few sectors
    static bool IsFewOwnSectors(NC_STACK_ypaworld *world);
    
    // Is ready to beam
    static bool IsReadyToBeam(NC_STACK_ypaworld *world);
    
};


}


#endif