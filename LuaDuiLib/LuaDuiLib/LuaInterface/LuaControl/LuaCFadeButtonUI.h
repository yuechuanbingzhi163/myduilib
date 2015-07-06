#ifndef _Lua_CFadeButtonUI_h
#define _Lua_CFadeButtonUI_h
#pragma once


namespace DuiLib
{
	class LuaCFadeButtonUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CFadeButtonUI_h