#include "StdAfx.h"

#include "LuaCFadeButtonUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCFadeButtonUI, New)
	{
		CFadeButtonUI  *ctrl = new CFadeButtonUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCFadeButtonUI, GetClassName)
	{
		CDuiString pstrText = CFadeButtonUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCFadeButtonUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "FadeButtonUI");

		luaL_newmetatable(l, "CFadeButtonUI");
		luaL_getmetatable(l, "CButtonUI");
		lua_setmetatable(l, -2);

		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		lua_pop(l, 1);
	}
}