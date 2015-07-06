#include "StdAfx.h"

#include "LuaCCheckBoxUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCCheckBoxUI, New)
	{
		CCheckBoxUI  *ctrl = new CCheckBoxUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCCheckBoxUI, GetClassName)
	{
		CDuiString pstrText = CCheckBoxUI::GetClassName();
		
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCCheckBoxUI,GetCheck)
	{
		try
		{
			CCheckBoxUI* self;
			self = static_cast<CCheckBoxUI*>(LuaStatic::CheckUserData(l, 1));
			lua_op_t<bool>::push_stack(l,self->GetCheck());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCCheckBoxUI::GetCheck"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCCheckBoxUI,SetCheck)
	{
		try
		{
			CCheckBoxUI* self;
			self = static_cast<CCheckBoxUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetCheck(lua_toboolean(l,2) != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCCheckBoxUI::SetCheck"));
			return 0;
		}
	}

	void LuaCCheckBoxUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "CheckBoxUI");

		luaL_newmetatable(l, "CCheckBoxUI");
		luaL_getmetatable(l, "COptionUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetCheck", GetCheck },
			{ "SetCheck", SetCheck },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}