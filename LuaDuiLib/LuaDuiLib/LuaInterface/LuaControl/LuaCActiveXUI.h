#ifndef _Lua_CActiveXUI_h
#define _Lua_CActiveXUI_h
#pragma once


namespace DuiLib
{
	class LuaCActiveXUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetHostWindow)
		DECL_LUA_FUNC(IsDelayCreate)
		DECL_LUA_FUNC(SetDelayCreate)
		DECL_LUA_FUNC(CreateControl)
		DECL_LUA_FUNC(CreateControl2)
		//DECL_LUA_FUNC(GetControl)
		//DECL_LUA_FUNC(GetClisd)
		DECL_LUA_FUNC(SetModuleName)
		DECL_LUA_FUNC(GetModuleName)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CActiveXUI_h