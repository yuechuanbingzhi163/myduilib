#ifndef _Lua_CButtonUI_h
#define _Lua_CButtonUI_h
#pragma once


namespace DuiLib
{
	class LuaCButtonUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetNormalImage)
		DECL_LUA_FUNC(GetNormalImage)
		DECL_LUA_FUNC(GetHotImage)
		DECL_LUA_FUNC(SetHotImage)
		DECL_LUA_FUNC(GetPushedImage)
		DECL_LUA_FUNC(SetPushedImage)
		DECL_LUA_FUNC(GetFocusedImage)
		DECL_LUA_FUNC(SetFocusedImage)
		DECL_LUA_FUNC(GetDisabledImage)
		DECL_LUA_FUNC(SetDisabledImage)
		DECL_LUA_FUNC(GetForeImage)
		DECL_LUA_FUNC(SetForeImage)
		DECL_LUA_FUNC(GetHotForeImage)
		DECL_LUA_FUNC(SetHotForeImage)
		DECL_LUA_FUNC(SetHotBkColor)
		DECL_LUA_FUNC(GetHotBkColor)
		DECL_LUA_FUNC(SetHotTextColor)
		DECL_LUA_FUNC(GetHotTextColor)
		DECL_LUA_FUNC(SetPushedTextColor)
		DECL_LUA_FUNC(GetPushedTextColor)
		DECL_LUA_FUNC(SetFocusedTextColor)
		DECL_LUA_FUNC(GetFocusedTextColor)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CButtonUI_h