#ifndef _Lua_CTreeViewUI_h
#define _Lua_CTreeViewUI_h
#pragma once


namespace DuiLib
{
	class LuaCTreeNodeUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetVisibleTag)
		DECL_LUA_FUNC(GetVisibleTag)
		DECL_LUA_FUNC(SetItemText)
		DECL_LUA_FUNC(GetItemText)
		DECL_LUA_FUNC(CheckBoxSelected)
		DECL_LUA_FUNC(IsCheckBoxSelected)
		DECL_LUA_FUNC(IsHasChild)
		DECL_LUA_FUNC(GetTreeLevel)
		DECL_LUA_FUNC(AddChildNode)
		DECL_LUA_FUNC(SetParentNode)
		DECL_LUA_FUNC(GetParentNode)
		DECL_LUA_FUNC(GetCountChild)
		DECL_LUA_FUNC(SetTreeView)
		DECL_LUA_FUNC(GetTreeView)
		DECL_LUA_FUNC(GetChildNode)
		DECL_LUA_FUNC(SetVisibleFolderBtn)
		DECL_LUA_FUNC(GetVisibleFolderBtn)
		DECL_LUA_FUNC(SetVisibleCheckBtn)
		DECL_LUA_FUNC(GetVisibleCheckBtn)
		DECL_LUA_FUNC(SetItemTextColor)
		DECL_LUA_FUNC(GetItemTextColor)
		DECL_LUA_FUNC(SetItemHotTextColor)
		DECL_LUA_FUNC(GetItemHotTextColor)
		DECL_LUA_FUNC(SetSelItemTextColor)
		DECL_LUA_FUNC(GetSelItemTextColor)
		DECL_LUA_FUNC(SetSelItemHotTextColor)
		DECL_LUA_FUNC(GetSelItemHotTextColor)

		static void Register(lua_State* l);		
	};

	class LuaCTreeViewUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(AddAt2)
		DECL_LUA_FUNC(SetItemCheckBox)
		DECL_LUA_FUNC(SetItemExpand)
		DECL_LUA_FUNC(SetVisibleFolderBtn)
		DECL_LUA_FUNC(GetVisibleFolderBtn)
		DECL_LUA_FUNC(SetVisibleCheckBtn)
		DECL_LUA_FUNC(GetVisibleCheckBtn)
		DECL_LUA_FUNC(SetItemMinWidth)
		DECL_LUA_FUNC(GetItemMinWidth)
		DECL_LUA_FUNC(SetItemHotTextColor)
		DECL_LUA_FUNC(SetSelItemTextColor)
		DECL_LUA_FUNC(SetSelItemHotTextColor)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CTreeViewUI_h