#include "StdAfx.h"
#include "LuaBit.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaBit, Bnot)
	{
		int n = lua_gettop(l);
		if (n == 0)
		{
			lua_pushinteger(l, 0);
			return 1;
		}
		
		return 1;
	}
	IMPL_LUA_FUNC(LuaBit, Band)
	{
		int n = lua_gettop(l);
		if (n == 0)
		{
			lua_pushinteger(l, 0);
			return 1;
		}
		int nT = lua_tointeger(l, 1);
		for (int i = 2; i <= n;++i)
		{
			int d = lua_tointeger(l, i);
			nT &= d;
		}
		lua_pushinteger(l, nT);
		return 1;
	}
	IMPL_LUA_FUNC(LuaBit, Bor)
	{
		int n = lua_gettop(l);
		if (n == 0)
		{
			lua_pushinteger(l, 0);
			return 1;
		}
		int nT = lua_tointeger(l, 1);
		for (int i = 2; i <= n; ++i)
		{
			int d = lua_tointeger(l, i);
			nT |= d;
		}
		lua_pushinteger(l, nT);
		return 1;
	}

	void LuaBit::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "Band", Band },
			{ "Bor", Bor },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));

		lua_setfield(l, -2, "Bit");
	}
};