#ifndef _Lua_CAnimControlUI_h
#define _Lua_CAnimControlUI_h
#pragma once


namespace DuiLib
{
	class LuaCAnimControlUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetImages)
		DECL_LUA_FUNC(GetImages)
		DECL_LUA_FUNC(SetElapse)
		DECL_LUA_FUNC(GetElapse)
		DECL_LUA_FUNC(SetTotalFrameCnt)
		DECL_LUA_FUNC(GetTotalFrameCnt)
		DECL_LUA_FUNC(Start)
		DECL_LUA_FUNC(Stop)
		DECL_LUA_FUNC(IsRunning)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CAnimControlUI_h