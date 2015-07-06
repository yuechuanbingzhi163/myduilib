#ifndef _Lua_CComboBoxUI_h
#define _Lua_CComboBoxUI_h
#pragma once


namespace DuiLib
{
	class LuaCComboBoxUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetArrowImage)
		DECL_LUA_FUNC(GetArrowImage)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CComboBoxUI_h