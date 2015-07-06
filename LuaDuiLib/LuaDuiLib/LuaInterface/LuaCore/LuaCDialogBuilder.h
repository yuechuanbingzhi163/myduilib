#ifndef _LuaCDialogBuilder_H
#define _LuaCDialogBuilder_H
#pragma once

namespace DuiLib
{
	class LuaCDialogBuilder
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetClass)
		DECL_LUA_FUNC(Create)
		DECL_LUA_FUNC(Create2)
		DECL_LUA_FUNC(GetMarkup)
		DECL_LUA_FUNC(GetLastErrorMessage)
		DECL_LUA_FUNC(GetLastErrorLocation)

		static void Register(lua_State* l);
	};
}

#endif//_LuaCDialogBuilder_H