#ifndef _Lua_CTabLayoutUI_h
#define _Lua_CTabLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCTabLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetCurSel)
		DECL_LUA_FUNC(SelectItem)
		DECL_LUA_FUNC(SelectItem2)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CTabLayoutUI_h