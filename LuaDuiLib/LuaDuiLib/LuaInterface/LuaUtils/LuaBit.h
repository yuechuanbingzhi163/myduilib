#ifndef _LuaBit_H
#define _LuaBit_H
#pragma once

namespace DuiLib
{
	class LuaBit
	{
		DECL_LUA_FUNC(Bnot)
		DECL_LUA_FUNC(Band)
		DECL_LUA_FUNC(Bor)
		DECL_LUA_FUNC(Bxor)
		DECL_LUA_FUNC(Lshirt)
		DECL_LUA_FUNC(Rshirt)
		DECL_LUA_FUNC(ARshirt)
		DECL_LUA_FUNC(Xor)
		DECL_LUA_FUNC(XAnd)
		
	public:
		static void Register(lua_State* l);
	};
}

#endif//_LuaBit_H