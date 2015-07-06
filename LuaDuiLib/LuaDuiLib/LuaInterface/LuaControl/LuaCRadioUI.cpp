#include "StdAfx.h"

#include "LuaCRadioUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCRadioUI, New)
	{
		CRadioUI  *ctrl = new CRadioUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCRadioUI, GetClassName)
	{
		CDuiString pstrText = CRadioUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCRadioUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "RadioUI");

		luaL_newmetatable(l, "CRadioUI");
		luaL_getmetatable(l, "COptionUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
				
		lua_pop(l, 1);
	}
}