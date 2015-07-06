#include "StdAfx.h"

#include "LuaCTextUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCTextUI, New)
	{
		CTextUI  *ctrl = new CTextUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCTextUI, GetClassName)
	{
		CDuiString pstrText = CTextUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCTextUI,GetLinkContent)
	{
		try
		{
			CTextUI* self;
			self = static_cast<CTextUI*>(LuaStatic::CheckUserData(l, 1));
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
			DuiException(_T("LuaCTextUI::GetLinkContent"));
			return 0;
		}
	}

	void LuaCTextUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "TextUI");

		luaL_newmetatable(l, "CTextUI");
		luaL_getmetatable(l, "CLabelUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetLinkContent", GetLinkContent },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}