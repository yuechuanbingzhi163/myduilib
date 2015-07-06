#include "StdAfx.h"

#include "LuaCAnimationTabLayoutUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCAnimationTabLayoutUI, New)
	{
		CAnimationTabLayoutUI  *ctrl = new CAnimationTabLayoutUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCAnimationTabLayoutUI, GetClassName)
	{
		CDuiString pstrText = CAnimationTabLayoutUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	void LuaCAnimationTabLayoutUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "AnimationTabLayoutUI");

		luaL_newmetatable(l, "CAnimationTabLayoutUI");
		luaL_getmetatable(l, "CTabLayoutUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
				
		lua_pop(l, 1);
	}
}