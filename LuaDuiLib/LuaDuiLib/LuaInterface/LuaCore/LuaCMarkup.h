#ifndef _LuaCMarkup_H
#define _LuaCMarkup_H
#pragma once

namespace DuiLib
{
	class LuaCMarkup
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
		DECL_LUA_FUNC(Load)
		DECL_LUA_FUNC(LoadFromMem)
		DECL_LUA_FUNC(LoadFromFile)
		DECL_LUA_FUNC(Release)
		DECL_LUA_FUNC(IsValid)
		DECL_LUA_FUNC(SetPreserveWhitespace)
		DECL_LUA_FUNC(GetLastErrorMessage)
		DECL_LUA_FUNC(GetLastErrorLocation)
		DECL_LUA_FUNC(GetRoot)

		static void Register(lua_State* l);
	};

	class LuaCMarkupNode
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetClass)
		DECL_LUA_FUNC(GetParent)
		DECL_LUA_FUNC(GetSibling)
		DECL_LUA_FUNC(GetChild)
		DECL_LUA_FUNC(HasSiblings)
		DECL_LUA_FUNC(HasChildren)
		DECL_LUA_FUNC(GetName)
		DECL_LUA_FUNC(GetValue)
		DECL_LUA_FUNC(HasAttributes)
		DECL_LUA_FUNC(HasAttribute)
		DECL_LUA_FUNC(GetAttributeCount)
		DECL_LUA_FUNC(GetAttributeName)
		DECL_LUA_FUNC(GetAttributeValue1)
		DECL_LUA_FUNC(GetAttributeValue2)
		DECL_LUA_FUNC(GetAttributeValue3)
		DECL_LUA_FUNC(GetAttributeValue4)

		static void Register(lua_State* l);
	};
}

#endif//_LuaCMarkup_H