#ifndef _Lua_CComboUI_h
#define _Lua_CComboUI_h
#pragma once


namespace DuiLib
{
	class LuaCComboUI
	{
	public:
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(New)
		//
		DECL_LUA_FUNC(GetDropBoxAttributeList)
		DECL_LUA_FUNC(SetDropBoxAttributeList)
		DECL_LUA_FUNC(GetDropBoxSize)
		DECL_LUA_FUNC(SetDropBoxSize)
		DECL_LUA_FUNC(GetCurSel)
		DECL_LUA_FUNC(SelectItem)
		DECL_LUA_FUNC(SetMultiSelect)
		DECL_LUA_FUNC(IsMultiSelected)
		DECL_LUA_FUNC(GetTextPadding)
		DECL_LUA_FUNC(SetTextPadding)
		DECL_LUA_FUNC(GetNormalImage)
		DECL_LUA_FUNC(SetNormalImage)
		DECL_LUA_FUNC(GetHotImage)
		DECL_LUA_FUNC(SetHotImage)
		DECL_LUA_FUNC(GetPushedImage)
		DECL_LUA_FUNC(SetPushedImage)
		DECL_LUA_FUNC(GetFocusedImage)
		DECL_LUA_FUNC(SetFocusedImage)
		DECL_LUA_FUNC(GetDisabledImage)
		DECL_LUA_FUNC(SetDisabledImage)
		DECL_LUA_FUNC(GetListInfo)
		DECL_LUA_FUNC(SetItemFont)
		DECL_LUA_FUNC(SetItemTextStyle)
		DECL_LUA_FUNC(GetItemTextPadding)
		DECL_LUA_FUNC(SetItemTextPadding)
		DECL_LUA_FUNC(GetItemTextColor)
		DECL_LUA_FUNC(SetItemTextColor)
		DECL_LUA_FUNC(GetItemBkColor)
		DECL_LUA_FUNC(SetItemBkColor)
		DECL_LUA_FUNC(GetItemBkImage)
		DECL_LUA_FUNC(SetItemBkImage)
		DECL_LUA_FUNC(IsAlternateBk)
		DECL_LUA_FUNC(SetAlternateBk)
		DECL_LUA_FUNC(GetSelectedItemTextColor)
		DECL_LUA_FUNC(SetSelectedItemTextColor)
		DECL_LUA_FUNC(GetSelectedItemBkColor)
		DECL_LUA_FUNC(SetSelectedItemBkColor)
		DECL_LUA_FUNC(GetSelectedItemImage)
		DECL_LUA_FUNC(SetSelectedItemImage)
		DECL_LUA_FUNC(GetHotItemTextColor)
		DECL_LUA_FUNC(SetHotItemTextColor)
		DECL_LUA_FUNC(GetHotItemBkColor)
		DECL_LUA_FUNC(SetHotItemBkColor)
		DECL_LUA_FUNC(GetHotItemImage)
		DECL_LUA_FUNC(SetHotItemImage)
		DECL_LUA_FUNC(GetDisabledItemTextColor)
		DECL_LUA_FUNC(SetDisabledItemTextColor)
		DECL_LUA_FUNC(GetDisabledItemBkColor)
		DECL_LUA_FUNC(SetDisabledItemBkColor)
		DECL_LUA_FUNC(GetDisabledItemImage)
		DECL_LUA_FUNC(SetDisabledItemImage)
		DECL_LUA_FUNC(GetItemLineColor)
		DECL_LUA_FUNC(SetItemLineColor)
		DECL_LUA_FUNC(IsItemShowHtml)
		DECL_LUA_FUNC(SetItemShowHtml)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CComboUI_h