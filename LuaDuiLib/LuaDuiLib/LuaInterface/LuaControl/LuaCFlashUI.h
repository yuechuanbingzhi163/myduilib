#ifndef _Lua_CFlashUI_h
#define _Lua_CFlashUI_h
#pragma once


namespace DuiLib
{
	class LuaCFlashUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetFlashEventHandler)
		DECL_LUA_FUNC(DoCreateControl)
		DECL_LUA_FUNC(get_ShockwaveFlash)
		DECL_LUA_FUNC(SetFlashUrl)
		DECL_LUA_FUNC(GetFlashUrl)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CFlashUI_h