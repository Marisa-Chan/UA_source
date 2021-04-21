#ifndef WORLD_LUA_H_INCLUDED
#define WORLD_LUA_H_INCLUDED

#include "../system/luascript.h"
#include "../types.h"

class NC_STACK_ypaworld;
class NC_STACK_ypabact;

namespace World
{
    
class LuaEvents: public System::LuaScript
{
public:
    struct UnitIter
    {
        RefBactList &Lst;
        RefBactList::iterator It;
        
        UnitIter(RefBactList &lst);
        bool IsEnd() const;
        NC_STACK_ypabact *GetUnit() const;
    };
    
public:
    LuaEvents(NC_STACK_ypaworld *wrld);
    virtual ~LuaEvents();
    
    void CallInit(int32_t ts);
    void CallUpdate(int32_t ts, int32_t delta);

protected:
    void RegisterWorld();
    void RegisterBact();
    void RegisterUnitIter();

protected:
    static int Lua_WrldShowMessageID(lua_State *l);
    static int Lua_WrldStartIterateUnits(lua_State *l);    
    static int Lua_WrldGetUnits(lua_State *l);
    static int Lua_WrldGetUserRobo(lua_State *l);
    static int Lua_WrldGetUserUnit(lua_State *l);
    static int Lua_WrldIsInRoboGun(lua_State *l);
    static int Lua_WrldGetUserSquadsCount(lua_State *l);
    static int Lua_WrldGetSector(lua_State *l);
    static int Lua_WrldGetSectorInfo(lua_State *l);
    static int Lua_WrldGetPowerStations(lua_State *l);
    static int Lua_WrldGetGates(lua_State *l);
    static int Lua_WrldExitTutorialWindow(lua_State *l);
    static int Lua_WrldIsRoboMapOpen(lua_State *l);

    static int Lua_BactStartIterateKids(lua_State *l);
    static int Lua_BactGetStatus(lua_State *l);
    static int Lua_BactGetVehicleId(lua_State *l);
    static int Lua_BactGetOwner(lua_State *l);
    static int Lua_BactGetType(lua_State *l);
    static int Lua_BactGetUnits(lua_State *l);
    static int Lua_BactGetPSector(lua_State *l);
    
    static int Lua_IterUnitsNext(lua_State *l);
    static int Lua_IterUnitsFree(lua_State *l);
    static int Lua_IterUnitsGet(lua_State *l);
    
protected:
    NC_STACK_ypaworld *_wrld = NULL;
};
    
}

#endif