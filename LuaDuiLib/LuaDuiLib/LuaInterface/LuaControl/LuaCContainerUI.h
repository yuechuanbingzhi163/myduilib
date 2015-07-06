#ifndef _Lua_CContainerUI_h
#define _Lua_CContainerUI_h
#pragma once


namespace DuiLib
{
	class LuaCContainerUI
	{
	public:
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(New)
		//
		DECL_LUA_FUNC(GetItemAt)
		DECL_LUA_FUNC(GetItemIndex)
		DECL_LUA_FUNC(SetItemIndex)
		DECL_LUA_FUNC(GetCount)
		DECL_LUA_FUNC(Add)
		DECL_LUA_FUNC(AddAt)
		DECL_LUA_FUNC(Remove)
		DECL_LUA_FUNC(RemoveAt)
		DECL_LUA_FUNC(RemoveAll)

		DECL_LUA_FUNC(GetInset)
		DECL_LUA_FUNC(SetInset)
		DECL_LUA_FUNC(GetChildPadding)
		DECL_LUA_FUNC(SetChildPadding)
		DECL_LUA_FUNC(IsAutoDestroy)
		DECL_LUA_FUNC(SetAutoDestroy)
		DECL_LUA_FUNC(IsDelayedDestroy)
		DECL_LUA_FUNC(SetDelayedDestroy)
		DECL_LUA_FUNC(IsMouseChildEnabled)
		DECL_LUA_FUNC(SetMouseChildEnabled)
		DECL_LUA_FUNC(FindSelectable)

		DECL_LUA_FUNC(SetSubControlText)
		DECL_LUA_FUNC(SetSubControlFixedHeight)
		DECL_LUA_FUNC(SetSubControlFixedWdith)
		DECL_LUA_FUNC(SetSubControlUserData)
		DECL_LUA_FUNC(GetSubControlText)
		DECL_LUA_FUNC(GetSubControlFixedHeight)
		DECL_LUA_FUNC(GetSubControlFixedWdith)
		DECL_LUA_FUNC(GetSubControlUserData)
		DECL_LUA_FUNC(FindSubControl)

		DECL_LUA_FUNC(GetScrollPos)
		DECL_LUA_FUNC(GetScrollRange)
		DECL_LUA_FUNC(SetScrollPos)
		DECL_LUA_FUNC(LineUp)
		DECL_LUA_FUNC(LineDown)
		DECL_LUA_FUNC(PageUp)
		DECL_LUA_FUNC(PageDown)
		DECL_LUA_FUNC(HomeUp)
		DECL_LUA_FUNC(EndDown)
		DECL_LUA_FUNC(LineLeft)
		DECL_LUA_FUNC(LineRight)
		DECL_LUA_FUNC(PageLeft)
		DECL_LUA_FUNC(PageRight)
		DECL_LUA_FUNC(HomeLeft)
		DECL_LUA_FUNC(EndRight)
		DECL_LUA_FUNC(EnableScrollBar)
		DECL_LUA_FUNC(GetVerticalScrollBar)
		DECL_LUA_FUNC(GetHorizontalScrollBar)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CContainerUI_h