#include "luaevents.h"
#include "../yw.h"
#include "../yw_internal.h"

extern int dword_5C8B78;
extern GuiList exit_menu;
extern tehMap robo_map;

namespace World
{
    
LuaEvents:: UnitIter::UnitIter(RefBactList &lst)
: Lst(lst)
{
    It = Lst.begin();
}

bool LuaEvents:: UnitIter::IsEnd() const
{
    return It == Lst.end();
}

NC_STACK_ypabact *LuaEvents:: UnitIter::GetUnit() const
{
    if (IsEnd())
        return NULL;
    return *It;
}


    
LuaEvents::LuaEvents(NC_STACK_ypaworld *wrld)
{
    _wrld = wrld;
    
    RegisterWorld();
    RegisterBact();
    RegisterUnitIter();
}

LuaEvents::~LuaEvents()
{
}

void LuaEvents::CallInit(int32_t ts)
{
    int t = lua_getglobal(_lua, "init");
    if (t != LUA_TFUNCTION)
    {
        lua_pop(_lua, 1);
        return;    
    }
    
    lua_pushinteger(_lua, ts);
    
    if ( lua_pcall(_lua, 1, 0, 0) != 0 )
        printf("Err CallFunc init:\n %s\n", lua_tostring(_lua, -1));
}

void LuaEvents::CallUpdate(int32_t ts, int32_t delta)
{
    int t = lua_getglobal(_lua, "update");
    if (t != LUA_TFUNCTION)
    {
        lua_pop(_lua, 1);
        return;    
    }
    
    lua_pushinteger(_lua, ts);
    lua_pushinteger(_lua, delta);
    
    if ( lua_pcall(_lua, 2, 0, 0) != 0 )
        printf("Err CallFunc update:\n %s\n", lua_tostring(_lua, -1));

}

std::string LuaEvents::GetSaveString()
{
    std::string buf;
    
    int t = lua_getglobal(_lua, "makesave");
    if (t != LUA_TFUNCTION)
    {
        lua_pop(_lua, 1);
        return "";
    }
        
    if ( lua_pcall(_lua, 0, 1, 0) != 0 )
        printf("Err CallFunc makesave:\n %s\n", lua_tostring(_lua, -1));
    
    buf = lua_tostring(_lua, -1);
    lua_pop(_lua, 1);
    return buf;    
}

void LuaEvents::RegisterWorld()
{
    static luaL_Reg WorldFuncs[] = 
    {
        {"ShowMessageID", Lua_WrldShowMessageID}
    ,   {"StartIterateUnits", Lua_WrldStartIterateUnits}
    ,   {"GetUnits", Lua_WrldGetUnits}
    ,   {"GetUserRobo", Lua_WrldGetUserRobo}
    ,   {"GetUserUnit", Lua_WrldGetUserUnit}
    ,   {"IsInRoboGun", Lua_WrldIsInRoboGun}
    ,   {"GetUserSquadsCount", Lua_WrldGetUserSquadsCount}
    ,   {"GetSector", Lua_WrldGetSector}
    ,   {"GetSectorInfo", Lua_WrldGetSectorInfo}
    ,   {"GetPowerStations", Lua_WrldGetPowerStations}
    ,   {"GetGates", Lua_WrldGetGates}
    ,   {"ExitTutorialWindow", Lua_WrldExitTutorialWindow}
    ,   {"IsRoboMapOpen", Lua_WrldIsRoboMapOpen}
    ,   {NULL, NULL} };
    
    luaL_newlib(_lua, WorldFuncs);
    lua_setglobal(_lua, "World");
    
    lua_getglobal(_lua, "World");
    lua_pushlightuserdata(_lua, _wrld);
    lua_setfield(_lua, -2, "Handle");
    lua_pop(_lua, 1);
}

void LuaEvents::RegisterBact()
{
    static luaL_Reg BactFuncs[] = 
    {
        {"GetStatus", Lua_BactGetStatus}
    ,   {"GetVehicleId", Lua_BactGetVehicleId}
    ,   {"GetOwner", Lua_BactGetOwner}
    ,   {"GetType", Lua_BactGetType}
    ,   {"StartIterateKids", Lua_BactStartIterateKids}
    ,   {"GetKids", Lua_BactGetUnits}
    ,   {"GetPSector", Lua_BactGetPSector}
    ,   {NULL, NULL} };
    
    luaL_newlib(_lua, BactFuncs);
    lua_setglobal(_lua, "Bact");

    
    lua_getglobal(_lua, "Bact");
    
    // Status
    lua_pushinteger(_lua, BACT_STATUS_NOPE);
    lua_setfield(_lua, -2, "STATUS_NOPE");
    
    lua_pushinteger(_lua, BACT_STATUS_NORMAL);
    lua_setfield(_lua, -2, "STATUS_NORMAL");
    
    lua_pushinteger(_lua, BACT_STATUS_DEAD);
    lua_setfield(_lua, -2, "STATUS_DEAD");
    
    lua_pushinteger(_lua, BACT_STATUS_IDLE);
    lua_setfield(_lua, -2, "STATUS_IDLE");
    
    lua_pushinteger(_lua, BACT_STATUS_CREATE);
    lua_setfield(_lua, -2, "STATUS_CREATE");
    
    lua_pushinteger(_lua, BACT_STATUS_BEAM);
    lua_setfield(_lua, -2, "STATUS_BEAM");
    
    // Type
    lua_pushinteger(_lua, BACT_TYPES_NOPE);
    lua_setfield(_lua, -2, "TYPE_NOPE");
    
    lua_pushinteger(_lua, BACT_TYPES_BACT);
    lua_setfield(_lua, -2, "TYPE_BACT");
    
    lua_pushinteger(_lua, BACT_TYPES_TANK);
    lua_setfield(_lua, -2, "TYPE_TANK");
    
    lua_pushinteger(_lua, BACT_TYPES_ROBO);
    lua_setfield(_lua, -2, "TYPE_ROBO");
    
    lua_pushinteger(_lua, BACT_TYPES_MISSLE);
    lua_setfield(_lua, -2, "TYPE_MISSLE");
    
    lua_pushinteger(_lua, BACT_TYPES_ZEPP);
    lua_setfield(_lua, -2, "TYPE_ZEPP");
    
    lua_pushinteger(_lua, BACT_TYPES_FLYER);
    lua_setfield(_lua, -2, "TYPE_FLYER");
    
    lua_pushinteger(_lua, BACT_TYPES_UFO);
    lua_setfield(_lua, -2, "TYPE_UFO");
    
    lua_pushinteger(_lua, BACT_TYPES_CAR);
    lua_setfield(_lua, -2, "TYPE_CAR");
    
    lua_pushinteger(_lua, BACT_TYPES_GUN);
    lua_setfield(_lua, -2, "TYPE_GUN");
    
    lua_pushinteger(_lua, BACT_TYPES_HOVER);
    lua_setfield(_lua, -2, "TYPE_HOVER");
    
    lua_pop(_lua, 1);
}

void LuaEvents::RegisterUnitIter()
{
    static luaL_Reg IterUnits[] = 
    {
        {"Next", Lua_IterUnitsNext}
    ,   {"Free", Lua_IterUnitsFree}
    ,   {"Get", Lua_IterUnitsGet}
    ,   {NULL, NULL} };
    
    luaL_newlib(_lua, IterUnits);
    lua_setglobal(_lua, "IterUnits");
}

int LuaEvents::Lua_WrldShowMessageID(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    int32_t msgId = lua_tointeger(l, 2);
    
    yw_arg159 arg159;
    arg159.Priority = 100;
    arg159.unit = NULL;
    arg159.MsgID = msgId;
    
    wrld->ypaworld_func159(&arg159);
    return 0;
}

int LuaEvents::Lua_WrldStartIterateUnits(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    
    UnitIter *it = new UnitIter(wrld->_unitsList);
    
    lua_pushlightuserdata(l, it);
    if (it->It != it->Lst.end())
        lua_pushlightuserdata(l, *it->It);
    else
        lua_pushnil(l);
    return 2;
}

int LuaEvents::Lua_BactStartIterateKids(lua_State* l)
{
    NC_STACK_ypabact *bact = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    
    UnitIter *it = new UnitIter(bact->_kidList);
    
    lua_pushlightuserdata(l, it);
    if (it->It != it->Lst.end())
        lua_pushlightuserdata(l, *it->It);
    else
        lua_pushnil(l);
    return 2;
}

int LuaEvents::Lua_IterUnitsNext(lua_State *l)
{
    UnitIter *it = (UnitIter *)lua_touserdata(l, 1);
    it->It++;
    if (it->It != it->Lst.end())
        lua_pushlightuserdata(l, *it->It);
    else
        lua_pushnil(l);
    return 1;
}

int LuaEvents::Lua_IterUnitsFree(lua_State *l)
{
    UnitIter *it = (UnitIter *)lua_touserdata(l, 1);
    delete it;
    return 0;
}

int LuaEvents::Lua_IterUnitsGet(lua_State *l)
{
    UnitIter *it = (UnitIter *)lua_touserdata(l, 1);
    if (it->It != it->Lst.end())
        lua_pushlightuserdata(l, *it->It);
    else
        lua_pushnil(l);
    return 1;
}

int LuaEvents::Lua_WrldGetUnits(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    
    lua_newtable(l);
    
    int32_t i = 0;
    for (NC_STACK_ypabact *bact : wrld->_unitsList)
    {
        lua_pushinteger(l, i);
        lua_pushlightuserdata(l, bact);
        lua_settable(l, -3);
        i++;
    }

    return 1;
}

int LuaEvents::Lua_BactGetStatus(lua_State *l)
{
    NC_STACK_ypabact *bact = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    lua_pushinteger(l, bact->_status);
    return 1;
}

int LuaEvents::Lua_BactGetVehicleId(lua_State *l)
{
    NC_STACK_ypabact *bact = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    lua_pushinteger(l, bact->_vehicleID);
    return 1;
}

int LuaEvents::Lua_BactGetOwner(lua_State *l)
{
    NC_STACK_ypabact *bact = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    lua_pushinteger(l, bact->_owner);
    return 1;
}

int LuaEvents::Lua_BactGetType(lua_State *l)
{
    NC_STACK_ypabact *bact = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    lua_pushinteger(l, bact->_bact_type);
    return 1;
}

int LuaEvents::Lua_BactGetUnits(lua_State *l)
{
    NC_STACK_ypabact *master = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    
    lua_newtable(l);
    
    int32_t i = 0;
    for (NC_STACK_ypabact *bact : master->_kidList)
    {
        lua_pushinteger(l, i);
        lua_pushlightuserdata(l, bact);
        lua_settable(l, -3);
        i++;
    }

    return 1;
}

int LuaEvents::Lua_BactGetPSector(lua_State *l)
{
    NC_STACK_ypabact *master = (NC_STACK_ypabact *)lua_touserdata(l, 1);
    
    PushPointer(l, master->_pSector);
    return 1;
}

int LuaEvents::Lua_WrldGetUserRobo(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    lua_pushlightuserdata(l, wrld->UserRobo);
    return 1;
}

int LuaEvents::Lua_WrldGetUserUnit(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    lua_pushlightuserdata(l, wrld->UserUnit);
    return 1;
}

int LuaEvents::Lua_WrldIsInRoboGun(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    lua_pushboolean(l, wrld->field_1b70 != 0);
    return 1;
}

int LuaEvents::Lua_WrldGetUserSquadsCount(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    lua_pushinteger(l, wrld->_cmdrsCount);
    return 1;
}

int LuaEvents::Lua_WrldGetSector(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    int32_t x = lua_tointeger(l, 2);
    int32_t y = lua_tointeger(l, 3);

    cellArea *area = wrld->GetSector(x, y);
    
    if (area)
        lua_pushlightuserdata(l, area);
    else
        lua_pushnil(l);
    return 1;
}

int LuaEvents::Lua_WrldGetSectorInfo(lua_State *l)
{
    cellArea *area = (cellArea *)lua_touserdata(l, 1);

    lua_newtable(l);
    
    if (area)
    {
        lua_pushinteger(l, area->owner);
        lua_setfield(l, -2, "owner");

        lua_pushinteger(l, area->w_type);
        lua_setfield(l, -2, "w_type");

        lua_pushinteger(l, area->Pos.x);
        lua_setfield(l, -2, "x");

        lua_pushinteger(l, area->Pos.y);
        lua_setfield(l, -2, "y");

        lua_pushinteger(l, area->type_id);
        lua_setfield(l, -2, "type_id");
    }
    
    return 1;
}

int LuaEvents::Lua_WrldGetPowerStations(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    
    lua_newtable(l);
    
    int32_t i = 0;
    for(const PowerStationRef &ps : wrld->_powerStations)
    {
        lua_pushinteger(l, i);
        lua_newtable(l);
        
        PushPointer(l, ps.pCell);
        lua_setfield(l, -2, "sector");
        
        lua_pushinteger(l, ps.Power);
        lua_setfield(l, -2, "power");
        
        lua_pushinteger(l, ps.EffectivePower);
        lua_setfield(l, -2, "effpwr");
        
        lua_settable(l, -3);
        
        i++;
    }
    return 1;
}

int LuaEvents::Lua_WrldGetGates(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    
    lua_newtable(l);
    
    int32_t i = 0;
    for(const MapGate &gate : wrld->_levelInfo->Gates)
    {
        lua_pushinteger(l, i);
        lua_newtable(l);
        
        PushPointer(l, gate.PCell);
        lua_setfield(l, -2, "sector");
        
        lua_pushinteger(l, gate.MbStatus);
        lua_setfield(l, -2, "mbstatus");
        
        lua_newtable(l);
        int32_t j = 0;
        for(const MapKeySector &sctr : gate.KeySectors)
        {
            lua_pushinteger(l, j);
            PushPointer(l, sctr.PCell);
            lua_settable(l, -3);
            
            j++;
        }
        lua_setfield(l, -2, "keysectors");
        
        lua_settable(l, -3);
        
        i++;
    }

    return 1;
}

int LuaEvents::Lua_WrldExitTutorialWindow(lua_State *l)
{
    NC_STACK_ypaworld *wrld = (NC_STACK_ypaworld *)lua_touserdata(l, 1);
    
    if ( sub_4C885C() != 3 )
    {
        dword_5C8B78 = 8;
        wrld->sb_0x4c87fc( wrld->GetLocaleString(2470, "2470 == EXIT TUTORIAL MISSION ?") , &exit_menu);
    }
    
    return 0;
}

int LuaEvents::Lua_WrldIsRoboMapOpen(lua_State *l)
{
    lua_pushboolean(l, robo_map.IsOpen());
    return 1;
}




}