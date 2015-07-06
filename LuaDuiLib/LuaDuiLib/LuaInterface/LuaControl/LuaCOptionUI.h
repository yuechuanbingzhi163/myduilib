#ifndef _Lua_COptionUI_h
#define _Lua_COptionUI_h
#pragma once


namespace DuiLib
{
	class LuaCOptionUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetSelectedImage)
		DECL_LUA_FUNC(SetSelectedImage)
		DECL_LUA_FUNC(SetSelectedTextColor)
		DECL_LUA_FUNC(GetSelectedTextColor)
		DECL_LUA_FUNC(GetSelectedHotImage)
		DECL_LUA_FUNC(SetSelectedHotImage)
		DECL_LUA_FUNC(GetSelectedPushedImage)
		DECL_LUA_FUNC(SetSelectedPushedImage)
		DECL_LUA_FUNC(GetGroup)
		DECL_LUA_FUNC(SetGroup)
		DECL_LUA_FUNC(IsSelected)
		DECL_LUA_FUNC(Selected)
		DECL_LUA_FUNC(SetNotFilterSelected)
		DECL_LUA_FUNC(IsNotFilterSelected)
		DECL_LUA_FUNC(SetSelectedToolTip)
		DECL_LUA_FUNC(GetSelectedToolTip)
		DECL_LUA_FUNC(SetNormalTooltip)
		DECL_LUA_FUNC(GetNormalTooltip)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_COptionUI_h