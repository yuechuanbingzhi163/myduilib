#include "StdAfx.h"

#include "LuaCWebBrowserUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCWebBrowserUI, New)
	{
		CWebBrowserUI  *self = new CWebBrowserUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, GetClassName)
	{
		CDuiString pstrText = CWebBrowserUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, SetHomePage)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetHomePage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::SetHomePage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, GetHomePage)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHomePage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::GetHomePage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, SetAutoNavigation)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			bool bAuto = true;
			if(!lua_isnone(l,2))
				bAuto = lua_toboolean(l,2) != 0;
			self->SetAutoNavigation(bAuto);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::SetAutoNavigation"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, IsAutoNavigation)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsAutoNavigation() ? 1 :0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::IsAutoNavigation"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, Navigate2)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->Navigate2(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::Navigate2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, Refresh)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			self->Refresh();
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::Refresh"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, Refresh2)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			int nLevel = lua_tointeger(l,2);
			self->Refresh2(nLevel);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::Refresh2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, GoBack)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			self->GoBack();
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::GoBack"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, NavigateHomePage)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			self->NavigateHomePage();
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::NavigateHomePage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, NavigateUrl)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->NavigateUrl(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::NavigateUrl"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWebBrowserUI, DoCreateControl)
	{
		try
		{
			CWebBrowserUI* self;
			self = static_cast<CWebBrowserUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->DoCreateControl() ? 1 :0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWebBrowserUI::DoCreateControl"));
			return 0;
		}
	}

	void LuaCWebBrowserUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "WebBrowserUI");

		luaL_newmetatable(l, "CWebBrowserUI");
		luaL_getmetatable(l, "CActiveXUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetHomePage", SetHomePage },
			{ "GetHomePage", GetHomePage },
			{ "SetAutoNavigation", SetAutoNavigation },
			{ "IsAutoNavigation", IsAutoNavigation },
			{ "Navigate2", Navigate2 },
			{ "Refresh", Refresh },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}