#ifndef SYSTEM_LUA_H_INCLUDED
#define SYSTEM_LUA_H_INCLUDED

#include <string>

extern "C" {
# include <lua5.3/lua.h>
# include <lua5.3/lauxlib.h>
# include <lua5.3/lualib.h>
}

namespace System
{
    
class LuaScript
{
public:
    LuaScript();
    virtual ~LuaScript();

    virtual bool LoadFile(const std::string &fname);
    void CallFunc(const std::string &name);
    
protected:
    void Register();
    
    static void RunScript(lua_State *l, const std::string &fname);
    static void PushPointer(lua_State *l, void *data);
    
protected:
    static int ExportRunScript(lua_State *l);
    
protected:
    lua_State* _lua = NULL;
};
    
}

#endif