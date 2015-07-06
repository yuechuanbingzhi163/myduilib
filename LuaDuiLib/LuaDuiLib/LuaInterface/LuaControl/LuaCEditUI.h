#ifndef _Lua_CEditUI_h
#define _Lua_CEditUI_h
#pragma once


namespace DuiLib
{
	class LuaCEditUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetMaxChar)
		DECL_LUA_FUNC(GetMaxChar)
		DECL_LUA_FUNC(SetReadOnly)
		DECL_LUA_FUNC(IsReadOnly)
		DECL_LUA_FUNC(SetPasswordMode)
		DECL_LUA_FUNC(IsPasswordMode)
		DECL_LUA_FUNC(SetPasswordChar)
		DECL_LUA_FUNC(GetPasswordChar)
		DECL_LUA_FUNC(SetNumberOnly)
		DECL_LUA_FUNC(IsNumberOnly)
		DECL_LUA_FUNC(GetWindowStyls)
		DECL_LUA_FUNC(GetNormalImage)
		DECL_LUA_FUNC(SetNormalImage)
		DECL_LUA_FUNC(GetHotImage)
		DECL_LUA_FUNC(SetHotImage)
		DECL_LUA_FUNC(GetFocusedImage)
		DECL_LUA_FUNC(SetFocusedImage)
		DECL_LUA_FUNC(GetDisabledImage)
		DECL_LUA_FUNC(SetDisabledImage)
		DECL_LUA_FUNC(SetNativeEditBkColor)
		DECL_LUA_FUNC(GetNativeEditBkColor)
		DECL_LUA_FUNC(SetSel)
		DECL_LUA_FUNC(SetSelAll)
		DECL_LUA_FUNC(SetReplaceSel)
		DECL_LUA_FUNC(SetPrompt)
		DECL_LUA_FUNC(GetPrompt)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CEditUI_h