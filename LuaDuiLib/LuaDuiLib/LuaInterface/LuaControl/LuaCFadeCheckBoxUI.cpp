#include "StdAfx.h"

#include "LuaCFadeCheckBoxUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCFadeCheckBoxUI, New)
	{
		CFadeCheckBoxUI  *ctrl = new CFadeCheckBoxUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCFadeCheckBoxUI, GetClassName)
	{
		CDuiString pstrText = CFadeCheckBoxUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCFadeCheckBoxUI,GetCheck)
	{
		try
		{
			CFadeCheckBoxUI* self;
			self = static_cast<CFadeCheckBoxUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetCheck() ? 1: 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCFadeCheckBoxUI::GetCheck"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCFadeCheckBoxUI,SetCheck)
	{
		try
		{
			CFadeCheckBoxUI* self;
			self = static_cast<CFadeCheckBoxUI*>(LuaStatic::CheckUserData(l, 1));
			bool bCheck = lua_toboolean(l,2) != 0;
			bool bSendMsg = true;
			if (!lua_isnone(l, 3))
				bSendMsg = lua_toboolean(l, 3) != 0;
			self->SetCheck(bCheck,bSendMsg);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCFadeCheckBoxUI::SetCheck"));
			return 0;
		}
	}

	void LuaCFadeCheckBoxUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "FadeCheckBoxUI");

		luaL_newmetatable(l, "CFadeCheckBoxUI");
		luaL_getmetatable(l, "CFadeOptionUI");
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