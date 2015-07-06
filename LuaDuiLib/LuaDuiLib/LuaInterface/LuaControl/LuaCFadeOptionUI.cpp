#include "StdAfx.h"

#include "LuaCFadeOptionUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCFadeOptionUI, New)
	{
		CFadeOptionUI  *ctrl = new CFadeOptionUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCFadeOptionUI, GetClassName)
	{
		CDuiString pstrText = CFadeOptionUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCFadeOptionUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "FadeOptionUI");

		luaL_newmetatable(l, "CFadeOptionUI");
		luaL_getmetatable(l, "COptionUI");
		lua_setmetatable(l, -2);

		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		lua_pop(l, 1);
	}
}