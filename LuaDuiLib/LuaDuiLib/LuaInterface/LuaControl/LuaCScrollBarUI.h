#ifndef _Lua_CScrollBarUI_h
#define _Lua_CScrollBarUI_h
#pragma once


namespace DuiLib
{
	class LuaCScrollBarUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetOwner)
		DECL_LUA_FUNC(SetOwner)
		DECL_LUA_FUNC(IsHorizontal)
		DECL_LUA_FUNC(SetHorizontal)
		DECL_LUA_FUNC(GetScrollRange)
		DECL_LUA_FUNC(SetScrollRange)
		DECL_LUA_FUNC(GetScrollPos)
		DECL_LUA_FUNC(SetScrollPos)
		DECL_LUA_FUNC(GetLineSize)
		DECL_LUA_FUNC(SetLineSize)

		DECL_LUA_FUNC(GetShowButton1)
		DECL_LUA_FUNC(SetShowButton1)
		DECL_LUA_FUNC(GetButton1NormalImage)
		DECL_LUA_FUNC(SetButton1NormalImage)
		DECL_LUA_FUNC(GetButton1HotImage)
		DECL_LUA_FUNC(SetButton1HotImage)
		DECL_LUA_FUNC(GetButton1PushedImage)
		DECL_LUA_FUNC(SetButton1PushedImage)
		DECL_LUA_FUNC(GetButton1DisabledImage)
		DECL_LUA_FUNC(SetButton1DisabledImage)

		DECL_LUA_FUNC(GetShowButton2)
		DECL_LUA_FUNC(SetShowButton2)
		DECL_LUA_FUNC(GetButton2NormalImage)
		DECL_LUA_FUNC(SetButton2NormalImage)
		DECL_LUA_FUNC(GetButton2HotImage)
		DECL_LUA_FUNC(SetButton2HotImage)
		DECL_LUA_FUNC(GetButton2PushedImage)
		DECL_LUA_FUNC(SetButton2PushedImage)
		DECL_LUA_FUNC(GetButton2DisabledImage)
		DECL_LUA_FUNC(SetButton2DisabledImage)

		DECL_LUA_FUNC(GetThumbNormalImage)
		DECL_LUA_FUNC(SetThumbNormalImage)
		DECL_LUA_FUNC(GetThumbHotImage)
		DECL_LUA_FUNC(SetThumbHotImage)
		DECL_LUA_FUNC(GetThumbPushedImage)
		DECL_LUA_FUNC(SetThumbPushedImage)
		DECL_LUA_FUNC(GetThumbDisabledImage)
		DECL_LUA_FUNC(SetThumbDisabledImage)

		DECL_LUA_FUNC(GetRailNormalImage)
		DECL_LUA_FUNC(SetRailNormalImage)
		DECL_LUA_FUNC(GetRailHotImage)
		DECL_LUA_FUNC(SetRailHotImage)
		DECL_LUA_FUNC(GetRailPushedImage)
		DECL_LUA_FUNC(SetRailPushedImage)
		DECL_LUA_FUNC(GetRailDisabledImage)
		DECL_LUA_FUNC(SetRailDisabledImage)

		DECL_LUA_FUNC(GetBkNormalImage)
		DECL_LUA_FUNC(SetBkNormalImage)
		DECL_LUA_FUNC(GetBkHotImage)
		DECL_LUA_FUNC(SetBkHotImage)
		DECL_LUA_FUNC(GetBkPushedImage)
		DECL_LUA_FUNC(SetBkPushedImage)
		DECL_LUA_FUNC(GetBkDisabledImage)
		DECL_LUA_FUNC(SetBkDisabledImage)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CScrollBarUI_h