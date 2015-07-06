#ifndef _Lua_CDateTimeUI_h
#define _Lua_CDateTimeUI_h
#pragma once


namespace DuiLib
{
	class LuaCDateTimeUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		//DECL_LUA_FUNC(GetTime)
		//DECL_LUA_FUNC(SetTime)
		DECL_LUA_FUNC(SetReadOnly)
		DECL_LUA_FUNC(IsReadOnly)
		DECL_LUA_FUNC(UpdateText)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CDateTimeUI_h