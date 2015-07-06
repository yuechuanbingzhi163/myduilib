#include "StdAfx.h"

#include "LuaCTreeViewUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCTreeNodeUI, New)
	{
		CTreeNodeUI  *pParentNode = NULL;
		if(lua_isuserdata(l,1))
			pParentNode = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
		CTreeNodeUI  *self = new CTreeNodeUI(pParentNode);
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetClassName)
	{
		CDuiString pstrText = CTreeNodeUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetVisibleTag)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetVisibleTag(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetVisibleTag"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetVisibleTag)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetVisibleTag() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetVisibleTag"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetItemText)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetItemText(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetItemText"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetItemText)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetItemText();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetItemText"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, CheckBoxSelected)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			self->CheckBoxSelected(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::CheckBoxSelected"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, IsCheckBoxSelected)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsCheckBoxSelected() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::IsCheckBoxSelected"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, IsHasChild)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsHasChild() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::IsHasChild"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetTreeLevel)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));	
			long nLevel = self->GetTreeLevel();
			lua_pushnumber(l, (lua_Number)nLevel);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetTreeLevel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, AddChildNode)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeNodeUI* pTreeNodeUI;
			pTreeNodeUI = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 2));
			if(self->AddChildNode(pTreeNodeUI))
				lua_pushboolean(l,1);
			else
				lua_pushboolean(l,0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::AddChildNode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetParentNode)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeNodeUI* pTreeNodeUI;
			pTreeNodeUI = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 2));
			self->SetParentNode(pTreeNodeUI);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetParentNode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetParentNode)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeNodeUI* pTreeNodeUI = self->GetParentNode();
			if (pTreeNodeUI)
				LuaStatic::AddObject2Lua(l, pTreeNodeUI, METATABLE_NAME(pTreeNodeUI));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetParentNode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetCountChild)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetCountChild());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetCountChild"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetTreeView)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeViewUI* pTreeUI;
			pTreeUI = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 2));
			self->SetTreeView(pTreeUI);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetTreeView"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetTreeView)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeViewUI* pTreeUI = self->GetTreeView();
			if (pTreeUI)
				LuaStatic::AddObject2Lua(l, pTreeUI, METATABLE_NAME(pTreeUI));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetTreeView"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetChildNode)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = lua_tointeger(l,2);
			CTreeNodeUI* pTreeNodeUI = self->GetChildNode(iIndex);
			if (pTreeNodeUI)
				LuaStatic::AddObject2Lua(l, pTreeNodeUI, METATABLE_NAME(pTreeNodeUI));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetChildNode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetVisibleFolderBtn)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetVisibleFolderBtn(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetVisibleFolderBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetVisibleFolderBtn)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetVisibleFolderBtn() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetVisibleFolderBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetVisibleCheckBtn)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetVisibleCheckBtn(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetVisibleCheckBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetVisibleCheckBtn)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetVisibleCheckBtn() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetVisibleCheckBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetItemTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetItemTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetItemHotTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemHotTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetItemHotTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetItemHotTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetItemHotTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetItemHotTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetSelItemTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetSelItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetSelItemTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetSelItemTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetSelItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, SetSelItemHotTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelItemHotTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::SetSelItemHotTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeNodeUI, GetSelItemHotTextColor)
	{
		try
		{
			CTreeNodeUI* self;
			self = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WriteRGBA2Lua(l, self->GetSelItemHotTextColor());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeNodeUI::GetSelItemHotTextColor"));
			return 0;
		}
	}

	void LuaCTreeNodeUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "TreeNodeUI");

		luaL_newmetatable(l, "CTreeNodeUI");
		luaL_getmetatable(l, "CListContainerElementUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetVisibleTag", SetVisibleTag },
			{ "GetVisibleTag", GetVisibleTag },
			{ "SetItemText", SetItemText },
			{ "GetItemText", GetItemText },
			{ "CheckBoxSelected", CheckBoxSelected },
			{ "IsCheckBoxSelected", IsCheckBoxSelected },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
};
namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCTreeViewUI, New)
	{
		CTreeViewUI  *self = new CTreeViewUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, GetClassName)
	{
		CDuiString pstrText = CTreeViewUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCTreeViewUI, AddAt2)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			CTreeNodeUI* pControl;
			pControl = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 2));
			CTreeNodeUI* pIndexNode = NULL;
			pIndexNode = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 3));
			lua_pushboolean(l,self->AddAt(pControl,pIndexNode) ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::AddAt2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetItemCheckBox)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			bool bSelected = lua_toboolean(l,2) != 0;
			CTreeNodeUI* pTreeNode = NULL;
			pTreeNode = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 3));
			lua_pushboolean(l,self->SetItemCheckBox(bSelected,pTreeNode) ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetItemCheckBox"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetItemExpand)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			bool bExpand = lua_toboolean(l,2) != 0;
			CTreeNodeUI* pTreeNode = NULL;
			pTreeNode = static_cast<CTreeNodeUI*>(LuaStatic::CheckUserData(l, 3));
			self->SetItemExpand(bExpand, pTreeNode);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetItemExpand"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetVisibleFolderBtn)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));

			bool bVisible = true;
			if(!lua_isnone(l,2))
				bVisible = lua_toboolean(l,2) != 0;

			self->SetVisibleFolderBtn(bVisible);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetVisibleFolderBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, GetVisibleFolderBtn)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetVisibleFolderBtn() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::GetVisibleFolderBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetVisibleCheckBtn)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));

			bool bVisible = true;
			if(!lua_isnone(l,2))
				bVisible = lua_toboolean(l,2) != 0;

			self->SetVisibleCheckBtn(bVisible);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetVisibleCheckBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, GetVisibleCheckBtn)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetVisibleCheckBtn() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::GetVisibleCheckBtn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetItemMinWidth)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetItemMinWidth(nVal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetItemMinWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, GetItemMinWidth)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetItemMinWidth());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::GetItemMinWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetItemHotTextColor)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetItemHotTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetItemHotTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetSelItemTextColor)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelItemTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetSelItemTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCTreeViewUI, SetSelItemHotTextColor)
	{
		try
		{
			CTreeViewUI* self;
			self = static_cast<CTreeViewUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelItemHotTextColor(dwCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTreeViewUI::SetSelItemHotTextColor"));
			return 0;
		}
	}

	void LuaCTreeViewUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "TreeViewUI");

		luaL_newmetatable(l, "CTreeViewUI");
		luaL_getmetatable(l, "CListUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "AddAt2", AddAt2 },
			{ "SetItemCheckBox", SetItemCheckBox },
			{ "SetItemExpand", SetItemExpand },
			{ "SetVisibleFolderBtn", SetVisibleFolderBtn },
			{ "GetVisibleFolderBtn", GetVisibleFolderBtn },
			{ "SetVisibleCheckBtn", SetVisibleCheckBtn },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}