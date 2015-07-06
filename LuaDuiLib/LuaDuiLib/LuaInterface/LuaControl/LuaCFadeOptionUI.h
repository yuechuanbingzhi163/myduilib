#ifndef _Lua_CFadeOptionUI_h
#define _Lua_CFadeOptionUI_h
#pragma once


namespace DuiLib
{
	class LuaCFadeOptionUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CFadeOptionUI_h