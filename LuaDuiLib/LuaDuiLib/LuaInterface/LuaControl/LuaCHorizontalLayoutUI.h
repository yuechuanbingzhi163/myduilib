#ifndef _Lua_CHorizontalLayoutUI_h
#define _Lua_CHorizontalLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCHorizontalLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetSepWidth)
		DECL_LUA_FUNC(GetSepWidth)
		DECL_LUA_FUNC(SetSepImmMode)
		DECL_LUA_FUNC(IsSepImmMode)
		DECL_LUA_FUNC(GetThumbRect)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CHorizontalLayoutUI_h