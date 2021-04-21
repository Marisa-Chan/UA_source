#include "luascript.h"
#include "../fmtlib/printf.h"

namespace System
{
    
LuaScript::LuaScript()
{
    _lua = luaL_newstate();
    
    luaL_openlibs(_lua);
    
    Register();
}

LuaScript::~LuaScript()
{
    lua_close(_lua);
}

void LuaScript::Register()
{
    static luaL_Reg Funcs[] = 
    { {"RunScript", ExportRunScript}
    , {NULL, NULL} };
    
    luaL_newlib(_lua, Funcs);
    lua_setglobal(_lua, "System");
}
    
bool LuaScript::LoadFile(const std::string &fname)
{
    RunScript(_lua, fname);
    
    return true;
}

void LuaScript::CallFunc(const std::string &name)
{
    int t = lua_getglobal(_lua, name.c_str());
    if (t != LUA_TFUNCTION)
    {
        lua_pop(_lua, 1);
        return;    
    }
    
    if ( lua_pcall(_lua, 0, 0, 0) != 0 )
        printf("Err CallFunc %s\n", name.c_str());
}

void LuaScript::RunScript(lua_State *l, const std::string &fname)
{
    int top = lua_gettop(l);
    
    
    std::string tmp = fmt::sprintf("res/%s", fname);
    if ( luaL_loadfile(l, tmp.c_str()) != LUA_OK )
    {
        printf("Err on loading script %s\n %s\n", fname.c_str(), lua_tostring(l, -1));
        return;
    }
    
    if ( lua_pcall(l, 0, LUA_MULTRET, 0) != 0 )
        printf("Err on run script %s\n %s\n", fname.c_str(), lua_tostring(l, -1));
    
    lua_settop(l, -(lua_gettop(l) - top) - 1);
}

int LuaScript::ExportRunScript(lua_State *l)
{
    RunScript(l, lua_tostring(l, 1));    
    return 0;
}

void LuaScript::PushPointer(lua_State *l, void *data)
{
    if (data)
        lua_pushlightuserdata(l, data);
    else
        lua_pushnil(l);
}


}