#ifndef _Lua_CVerticalLayoutUI_h
#define _Lua_CVerticalLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCVerticalLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetSepHeight)
		DECL_LUA_FUNC(GetSepHeight)
		DECL_LUA_FUNC(SetSepImmMode)
		DECL_LUA_FUNC(IsSepImmMode)
		DECL_LUA_FUNC(GetThumbRect)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CVerticalLayoutUI_h