#include "StdAfx.h"

#include "LuaCListUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListUI, New)
	{
		CListUI  *ctrl = new CListUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListUI, GetClassName)
	{
		CDuiString pstrText = CListUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListUI, GetScrollSelect)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetScrollSelect() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetScrollSelect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetScrollSelect)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetScrollSelect(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetScrollSelect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetCurSel)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetCurSel());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetCurSel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SelectItem)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_tointeger(l,2);
			bool bTakeFocus = false;
			bool bSelected = true;
			if(!lua_isnone(l,3))
				bTakeFocus = lua_toboolean(l,3) != 0 ;
			if(!lua_isnone(l,4))
				bSelected = lua_toboolean(l,4) != 0;
			self->SelectItem(iIndex,bTakeFocus,bSelected);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SelectItem"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetSelCount)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetSelCount());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetSelCount"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetHeader)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CListHeaderUI* pHeader = self->GetHeader();
			if (pHeader)
				LuaStatic::AddObject2Lua(l, pHeader, METATABLE_NAME(pHeader));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetHeader"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetList)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CContainerUI* pList = self->GetList();
			if (pList)
				LuaStatic::AddObject2Lua(l, pList, METATABLE_NAME(pList));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetList"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetListInfo)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			TListInfoUI* pInfo = self->GetListInfo();
			if (pInfo)
				;//LuaStatic::AddObject2Lua(l, pInfo, METATABLE_NAME(pList));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetListInfo"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, EnsureVisible)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->EnsureVisible(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::EnsureVisible"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, Scroll)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			int dx = lua_tointeger(l,2);
			int dy = lua_tointeger(l,3);
			self->Scroll(dx,dy);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::EnsureVisible"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemFont)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetItemFont(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemFont"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemTextStyle)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetItemTextStyle(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemTextStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemTextPadding)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			RECT rt = LuaStatic::ReadRectFromLua(l, 2);
			self->SetItemTextPadding(rt);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemTextPadding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemBkColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemBkImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetItemBkImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemBkImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetAlternateBk)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetAlternateBk(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetSelectedItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelectedItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetSelectedItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetSelectedItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelectedItemBkColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetSelectedItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetSelectedItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSelectedItemImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetSelectedItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetHotItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetHotItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetHotItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetHotItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetHotItemBkColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetHotItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetHotItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetHotItemImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetHotItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetDisabledItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetDisabledItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetDisabledItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetDisabledItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetDisabledItemBkColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetDisabledItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetDisabledItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetDisabledItemImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetDisabledItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemLineColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemLineColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemLineColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, IsItemShowHtml)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsItemShowHtml() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::IsItemShowHtml"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetItemShowHtml)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetItemShowHtml(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetItemShowHtml"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetItemTextPadding)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRect2Lua(l, self->GetItemTextPadding());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetItemTextPadding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetItemBkColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetItemBkImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetItemBkImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetItemBkImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, IsAlternateBk)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_op_t<bool>::push_stack(l, self->IsAlternateBk());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::IsAlternateBk"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetSelectedItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetSelectedItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetSelectedItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetSelectedItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetSelectedItemBkColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetSelectedItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetSelectedItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSelectedItemImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetSelectedItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetHotItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetHotItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetHotItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetHotItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetHotItemBkColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetHotItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetHotItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotItemImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetHotItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetDisabledItemTextColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetDisabledItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetDisabledItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetDisabledItemBkColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetDisabledItemBkColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetDisabledItemBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetDisabledItemImage)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetDisabledItemImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetDisabledItemImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetItemLineColor)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetItemLineColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetItemLineColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetMultiSelect)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetMultiSelect(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetMultiSelect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, IsMultiSelected)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsMultiSelected() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::IsMultiSelected"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetMultiExpanding)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetMultiExpanding(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetMultiExpanding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetExpandedItem)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetExpandedItem());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetExpandedItem"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, ExpandItem)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_toboolean(l,2) != 0;
			bool bExpand = true;
			if(!lua_isnone(l,3))
				bExpand = lua_toboolean(l,3) != 0;
			self->ExpandItem(iIndex,bExpand);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::ExpandItem"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, GetTextCallback)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			//
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::GetExpandedItem"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListUI, SetTextCallback)
	{
		try
		{
			CListUI* self;
			self = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
			IListCallbackUI* pCallback = static_cast<IListCallbackUI*>(LuaStatic::CheckUserData(l, 2));
			self->SetTextCallback(pCallback);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListUI::SetTextCallback"));
			return 0;
		}
	}

	void LuaCListUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListUI");

		luaL_newmetatable(l, "CListUI");
		luaL_getmetatable(l, "CVerticalLayoutUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "GetScrollSelect", GetScrollSelect },
			{ "SetTextCallback", SetTextCallback },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListBodyUI, New)
	{
		CListUI* pOwner = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 1));
		CListBodyUI  *self = new CListBodyUI(pOwner);
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListBodyUI, GetClassName)
	{
		CDuiString pstrText = CListBodyUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCListBodyUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListBodyUI");

		luaL_newmetatable(l, "CListBodyUI");
		luaL_getmetatable(l, "CVerticalLayoutUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
				
		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListHeaderUI, New)
	{
		CListHeaderUI  *self = new CListHeaderUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListHeaderUI, GetClassName)
	{
		CDuiString pstrText = CListHeaderUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCListHeaderUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListHeaderUI");

		luaL_newmetatable(l, "CListHeaderUI");
		luaL_getmetatable(l, "CHorizontalLayoutUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
				
		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, New)
	{
		CListHeaderItemUI  *self = new CListHeaderItemUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetClassName)
	{
		CDuiString pstrText = CListHeaderItemUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListHeaderItemUI, IsDragable)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsDragable() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::IsDragable"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetDragable)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetDragable(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetDragable"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetSepWidth)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetSepWidth());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetSepWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetSepWidth)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			int iWidth = lua_tointeger(l,2);
			self->SetSepWidth(iWidth);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetSepWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetTextStyle)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetTextStyle());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetTextStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetTextStyle)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			int iWidth = lua_tointeger(l,2);
			self->SetTextStyle(iWidth);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetTextStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetTextColor)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetTextColor)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetTextPadding)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			RECT rt = LuaStatic::ReadRectFromLua(l, 2);
			self->SetTextPadding(rt);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetTextPadding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetTextPadding)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRect2Lua(l, self->GetTextPadding());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetTextPadding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetFont)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			int iWidth = lua_tointeger(l,2);
			self->SetFont(iWidth);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetFont"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, IsShowHtml)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsShowHtml() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::IsShowHtml"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetShowHtml)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetShowHtml(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetShowHtml"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetNormalImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetNormalImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetNormalImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetNormalImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetHotImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetHotImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetHotImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetPushedImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetPushedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetPushedImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetPushedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetFocusedImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetFocusedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetFocusedImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetFocusedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetSepImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSepImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetSepImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, SetSepImage)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSepImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetSepImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListHeaderItemUI, GetThumbRect)
	{
		try
		{
			CListHeaderItemUI* self;
			self = static_cast<CListHeaderItemUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRect2Lua(l, self->GetThumbRect());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::GetThumbRect"));
			return 0;
		}
	}

	void LuaCListHeaderItemUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListHeaderItemUI");

		luaL_newmetatable(l, "CListHeaderItemUI");
		luaL_getmetatable(l, "CControlUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "IsDragable", IsDragable },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));		
		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListElementUI, New)
	{
		CListElementUI  *self = new CListElementUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListElementUI, GetClassName)
	{
		CDuiString pstrText = CListElementUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListElementUI, GetIndex)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetIndex());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::GetIndex"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, SetIndex)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			int iWidth = lua_tointeger(l,2);
			self->SetIndex(iWidth);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListHeaderItemUI::SetIndex"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, GetOwner)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			IListOwnerUI* pOwner;
			pOwner = static_cast<IListOwnerUI*>(self->GetOwner());

			if (pOwner)
			{
				CDuiString sName = pOwner->GetClass();
				LuaStatic::AddObject2Lua(l,pOwner, sName.str());
			}
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::GetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, SetOwner)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));

			CControlUI* pOwner;
			pOwner = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));

			self->SetOwner(pOwner);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::SetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, IsSelected)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsSelected() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::IsSelected"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, Select)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			bool bSelected = true;
			if(!lua_isnone(l,2))
				bSelected = lua_toboolean(l,2) != 0;
			self->Select(bSelected);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::Select"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, IsExpanded)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsExpanded() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::IsExpanded"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListElementUI, Expand)
	{
		try
		{
			CListElementUI* self;
			self = static_cast<CListElementUI*>(LuaStatic::CheckUserData(l, 1));
			bool bExpand = true;
			if(!lua_isnone(l,2))
				bExpand = lua_toboolean(l,2) != 0;
			self->Expand(bExpand);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListElementUI::Expand"));
			return 0;
		}
	}
	void LuaCListElementUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListElementUI");

		luaL_newmetatable(l, "CListElementUI");
		luaL_getmetatable(l, "CControlUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetIndex", GetIndex },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));		
		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListLabelElementUI, New)
	{
		CListLabelElementUI  *self = new CListLabelElementUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListLabelElementUI, GetClassName)
	{
		CDuiString pstrText = CListLabelElementUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	
	void LuaCListLabelElementUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListLabelElementUI");

		luaL_newmetatable(l, "CListLabelElementUI");
		luaL_getmetatable(l, "CListElementUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListTextElementUI, New)
	{
		CListTextElementUI  *self = new CListTextElementUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListTextElementUI, GetClassName)
	{
		CDuiString pstrText = CListTextElementUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListTextElementUI, GetTextAt)
	{
		try
		{
			CListTextElementUI* self;
			self = static_cast<CListTextElementUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_tointeger(l,2);
			CDuiString pstrText = self->GetText(iIndex);

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListTextElementUI::GetTextAt"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListTextElementUI, SetTextAt)
	{
		try
		{
			CListTextElementUI* self;
			self = static_cast<CListTextElementUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_tointeger(l,2);
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);

			self->SetText(iIndex, pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListTextElementUI::SetTextAt"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListTextElementUI, SetOwner)
	{
		try
		{
			CListTextElementUI* self;
			self = static_cast<CListTextElementUI*>(LuaStatic::CheckUserData(l, 1));

			CControlUI* pOwner;
			pOwner = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));

			self->SetOwner(pOwner);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListTextElementUI::SetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListTextElementUI, GetLinkContent)
	{
		try
		{
			CListTextElementUI* self;
			self = static_cast<CListTextElementUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_tointeger(l,2);
			CDuiString* pStr = self->GetLinkContent(iIndex);
			if (pStr)
				lua_pushstring(l, pStr->str().c_str());
			else
				lua_pushnil(l);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListTextElementUI::GetLinkContent"));
			return 0;
		}
	}
	
	void LuaCListTextElementUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListTextElementUI");

		luaL_newmetatable(l, "CListTextElementUI");
		luaL_getmetatable(l, "CListLabelElementUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetTextAt", GetTextAt },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));		
		lua_pop(l,1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCListContainerElementUI, New)
	{
		CListContainerElementUI  *ctrl = new CListContainerElementUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCListContainerElementUI, GetClassName)
	{
		CDuiString pstrText = CListContainerElementUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, GetOwner)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));
			IListOwnerUI* pOwner;
			pOwner = static_cast<IListOwnerUI*>(self->GetOwner());

			if (pOwner)
			{
				CDuiString sName = pOwner->GetClass();
				LuaStatic::AddObject2Lua(l,pOwner, sName.str());
			}
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::GetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, SetOwner)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));

			CControlUI* pOwner;
			pOwner = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));

			self->SetOwner(pOwner);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::SetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, IsSelected)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsSelected() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::IsSelected"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, Select)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));

			bool bSelect = true;
			if(!lua_isnone(l,2))
				bSelect = lua_toboolean(l,2) != 0;

			self->Select(bSelect);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::Select"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, IsExpanded)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsExpanded() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::IsExpanded"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCListContainerElementUI, Expand)
	{
		try
		{
			CListContainerElementUI* self;
			self = static_cast<CListContainerElementUI*>(LuaStatic::CheckUserData(l, 1));

			bool bExpand = true;
			if(!lua_isnone(l,2))
				bExpand = lua_toboolean(l,2) != 0;

			self->Expand(bExpand);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCListContainerElementUI::Expand"));
			return 0;
		}
	}

	void LuaCListContainerElementUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ListContainerElementUI");

		luaL_newmetatable(l, "CListContainerElementUI");
		luaL_getmetatable(l, "CContainerUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetOwner", GetOwner },
			{ "SetOwner", SetOwner },
			{ "IsSelected", IsSelected },
			{ "Select", Select },
			{ "IsExpanded", IsExpanded },
			{ "Expand", Expand },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}
