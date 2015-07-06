#ifndef _Lua_CWkeWebkitUI_h
#define _Lua_CWkeWebkitUI_h
#pragma once


namespace DuiLib
{
	class LuaCWkeWebkitUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(WkeWebkit_Init)
		DECL_LUA_FUNC(WkeWebkit_Shutdown)
		DECL_LUA_FUNC(SetURL)
		DECL_LUA_FUNC(SetFile)
		DECL_LUA_FUNC(RunJS)
		DECL_LUA_FUNC(SetClientHandler)
		DECL_LUA_FUNC(GoBack)
		DECL_LUA_FUNC(GoForward)
		DECL_LUA_FUNC(GoHome)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CWkeWebkitUI_h