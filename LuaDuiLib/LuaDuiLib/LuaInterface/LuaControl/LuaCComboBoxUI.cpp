#include "StdAfx.h"

#include "LuaCComboBoxUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCComboBoxUI, New)
	{
		CComboBoxUI  *ctrl = new CComboBoxUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCComboBoxUI, GetClassName)
	{
		CDuiString pstrText = CComboBoxUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCComboBoxUI,GetArrowImage)
	{
		try
		{
			CComboBoxUI* self;
			self = static_cast<CComboBoxUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetArrowImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCComboBoxUI::GetArrowImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCComboBoxUI,SetArrowImage)
	{
		try
		{
			CComboBoxUI* self;
			self = static_cast<CComboBoxUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetArrowImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCComboBoxUI::SetArrowImage"));
			return 0;
		}
	}

	void LuaCComboBoxUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ComboBoxUI");

		luaL_newmetatable(l, "CComboBoxUI");
		luaL_getmetatable(l, "CComboUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetArrowImage", SetArrowImage },
			{ "GetArrowImage", GetArrowImage },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}