#ifndef _Lua_CWebBrowserUI_h
#define _Lua_CWebBrowserUI_h
#pragma once


namespace DuiLib
{
	class LuaCWebBrowserUI
	{
	public:
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetHomePage)
		DECL_LUA_FUNC(GetHomePage)
		DECL_LUA_FUNC(SetAutoNavigation)
		DECL_LUA_FUNC(IsAutoNavigation)
		DECL_LUA_FUNC(Navigate2)
		DECL_LUA_FUNC(Refresh)
		DECL_LUA_FUNC(Refresh2)
		DECL_LUA_FUNC(GoBack)
		DECL_LUA_FUNC(NavigateHomePage)
		DECL_LUA_FUNC(NavigateUrl)
		DECL_LUA_FUNC(DoCreateControl)

		static void Register(lua_State* l);
	};
}

#endif//_Lua_CWebBrowserUI_h