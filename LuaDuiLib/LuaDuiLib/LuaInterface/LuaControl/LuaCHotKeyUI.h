#ifndef _Lua_CHotKeyUI_h
#define _Lua_CHotKeyUI_h
#pragma once


namespace DuiLib
{
	class LuaCHotKeyUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetNormalImage)
		DECL_LUA_FUNC(SetNormalImage)
		DECL_LUA_FUNC(GetHotImage)
		DECL_LUA_FUNC(SetHotImage)
		DECL_LUA_FUNC(GetFocusedImage)
		DECL_LUA_FUNC(SetFocusedImage)
		DECL_LUA_FUNC(GetDisabledImage)
		DECL_LUA_FUNC(SetDisabledImage)
		DECL_LUA_FUNC(SetNativeBkColor)
		DECL_LUA_FUNC(GetNativeBkColor)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CHotKeyUI_h