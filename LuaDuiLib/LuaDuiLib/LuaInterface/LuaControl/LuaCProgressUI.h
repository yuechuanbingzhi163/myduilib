#ifndef _Lua_CProgressUI_h
#define _Lua_CProgressUI_h
#pragma once


namespace DuiLib
{
	class LuaCProgressUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(IsHorizontal)
		DECL_LUA_FUNC(SetHorizontal)
		DECL_LUA_FUNC(IsStretchForeImage)
		DECL_LUA_FUNC(SetStretchForeImage)
		DECL_LUA_FUNC(GetMinValue)
		DECL_LUA_FUNC(SetMinValue)
		DECL_LUA_FUNC(GetMaxValue)
		DECL_LUA_FUNC(SetMaxValue)
		DECL_LUA_FUNC(GetValue)
		DECL_LUA_FUNC(SetValue)
		DECL_LUA_FUNC(GetForeImage)
		DECL_LUA_FUNC(SetForeImage)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CProgressUI_h