#ifndef _Lua_CListUI_h
#define _Lua_CListUI_h
#pragma once


namespace DuiLib
{
	class LuaCListUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetScrollSelect)
		DECL_LUA_FUNC(SetScrollSelect)
		DECL_LUA_FUNC(GetCurSel)
		DECL_LUA_FUNC(SelectItem)
		DECL_LUA_FUNC(GetSelCount)
		DECL_LUA_FUNC(GetHeader)
		DECL_LUA_FUNC(GetList)
		DECL_LUA_FUNC(GetListInfo)
		DECL_LUA_FUNC(EnsureVisible)
		DECL_LUA_FUNC(Scroll)
		DECL_LUA_FUNC(SetItemFont)
		DECL_LUA_FUNC(SetItemTextStyle)
		DECL_LUA_FUNC(SetItemTextPadding)
		DECL_LUA_FUNC(SetItemTextColor)
		DECL_LUA_FUNC(SetItemBkColor)
		DECL_LUA_FUNC(SetItemBkImage)
		DECL_LUA_FUNC(SetAlternateBk)
		DECL_LUA_FUNC(SetSelectedItemTextColor)
		DECL_LUA_FUNC(SetSelectedItemBkColor)
		DECL_LUA_FUNC(SetSelectedItemImage)
		DECL_LUA_FUNC(SetHotItemTextColor)
		DECL_LUA_FUNC(SetHotItemBkColor)
		DECL_LUA_FUNC(SetHotItemImage)
		DECL_LUA_FUNC(SetDisabledItemTextColor)
		DECL_LUA_FUNC(SetDisabledItemBkColor)
		DECL_LUA_FUNC(SetDisabledItemImage)
		DECL_LUA_FUNC(SetItemLineColor)
		DECL_LUA_FUNC(IsItemShowHtml)
		DECL_LUA_FUNC(SetItemShowHtml)
		DECL_LUA_FUNC(GetItemTextPadding)
		DECL_LUA_FUNC(GetItemTextColor)
		DECL_LUA_FUNC(GetItemBkColor)
		DECL_LUA_FUNC(GetItemBkImage)
		DECL_LUA_FUNC(IsAlternateBk)
		DECL_LUA_FUNC(GetSelectedItemTextColor)
		DECL_LUA_FUNC(GetSelectedItemBkColor)
		DECL_LUA_FUNC(GetSelectedItemImage)
		DECL_LUA_FUNC(GetHotItemTextColor)
		DECL_LUA_FUNC(GetHotItemBkColor)
		DECL_LUA_FUNC(GetHotItemImage)
		DECL_LUA_FUNC(GetDisabledItemTextColor)
		DECL_LUA_FUNC(GetDisabledItemBkColor)
		DECL_LUA_FUNC(GetDisabledItemImage)
		DECL_LUA_FUNC(GetItemLineColor)
		DECL_LUA_FUNC(SetMultiSelect)
		DECL_LUA_FUNC(IsMultiSelected)
		DECL_LUA_FUNC(SetMultiExpanding)
		DECL_LUA_FUNC(GetExpandedItem)
		DECL_LUA_FUNC(ExpandItem)
		DECL_LUA_FUNC(GetTextCallback)
		DECL_LUA_FUNC(SetTextCallback)

		static void Register(lua_State* l);
	};

	class LuaCListBodyUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		static void Register(lua_State* l);
	};

	class LuaCListHeaderUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		
		static void Register(lua_State* l);
	};

	class LuaCListHeaderItemUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(IsDragable)
		DECL_LUA_FUNC(SetDragable)
		DECL_LUA_FUNC(GetSepWidth)
		DECL_LUA_FUNC(SetSepWidth)
		DECL_LUA_FUNC(GetTextStyle)
		DECL_LUA_FUNC(SetTextStyle)
		DECL_LUA_FUNC(GetTextColor)
		DECL_LUA_FUNC(SetTextColor)
		DECL_LUA_FUNC(SetTextPadding)
		DECL_LUA_FUNC(GetTextPadding)
		DECL_LUA_FUNC(SetFont)
		DECL_LUA_FUNC(IsShowHtml)
		DECL_LUA_FUNC(SetShowHtml)
		DECL_LUA_FUNC(GetNormalImage)
		DECL_LUA_FUNC(SetNormalImage)
		DECL_LUA_FUNC(GetHotImage)
		DECL_LUA_FUNC(SetHotImage)
		DECL_LUA_FUNC(GetPushedImage)
		DECL_LUA_FUNC(SetPushedImage)
		DECL_LUA_FUNC(GetFocusedImage)
		DECL_LUA_FUNC(SetFocusedImage)
		DECL_LUA_FUNC(GetSepImage)
		DECL_LUA_FUNC(SetSepImage)
		DECL_LUA_FUNC(GetThumbRect)

		static void Register(lua_State* l);
	};

	class LuaCListElementUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetIndex)
		DECL_LUA_FUNC(SetIndex)
		DECL_LUA_FUNC(GetOwner)
		DECL_LUA_FUNC(SetOwner)
		DECL_LUA_FUNC(IsSelected)
		DECL_LUA_FUNC(Select)
		DECL_LUA_FUNC(IsExpanded)
		DECL_LUA_FUNC(Expand)

		static void Register(lua_State* l);
	};

	class LuaCListLabelElementUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		static void Register(lua_State* l);
	};

	class LuaCListTextElementUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetTextAt)
		DECL_LUA_FUNC(SetTextAt)
		DECL_LUA_FUNC(SetOwner)
		DECL_LUA_FUNC(GetLinkContent)

		static void Register(lua_State* l);
	};

	class LuaCListContainerElementUI
	{
	public:		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetOwner)
		DECL_LUA_FUNC(SetOwner)
		DECL_LUA_FUNC(IsSelected)
		DECL_LUA_FUNC(Select)
		DECL_LUA_FUNC(IsExpanded)
		DECL_LUA_FUNC(Expand)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CListUI_h