#ifndef _SYSTEMLUA_systemlua_H_
#define _SYSTEMLUA_systemlua_H_

 
#include "lua/lua.hpp"
////�ű��ӿ� 
//Lua_Funcs g_GameFunc[] = { 
//  { "Include", LuaInclude },
//  { "ReloadAllScript", LuaReloadAllScript },
//  { "SayHello", LuaSayHello },
//  { "StopGame", LuaStopGame },
//};
extern int lua_import(lua_State *L);

#endif /*_SYSTEMLUA_systemlua_H_*/