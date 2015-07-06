#ifndef _Lua_CFadeCheckBoxUI_h
#define _Lua_CFadeCheckBoxUI_h
#pragma once


namespace DuiLib
{
	class LuaCFadeCheckBoxUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetCheck)
		DECL_LUA_FUNC(GetCheck)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CFadeCheckBoxUI_h