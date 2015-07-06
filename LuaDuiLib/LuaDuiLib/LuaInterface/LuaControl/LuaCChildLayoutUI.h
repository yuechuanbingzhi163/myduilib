#ifndef _Lua_CChildLayoutUI_h
#define _Lua_CChildLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCChildLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(Init)
		DECL_LUA_FUNC(SetChildLayoutXML)
		DECL_LUA_FUNC(GetChildLayoutXML)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CChildLayoutUI_h