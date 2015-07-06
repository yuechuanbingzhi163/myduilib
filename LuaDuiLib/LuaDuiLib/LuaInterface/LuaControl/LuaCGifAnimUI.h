#ifndef _Lua_CGifAnimUI_h
#define _Lua_CGifAnimUI_h
#pragma once


namespace DuiLib
{
	class LuaCGifAnimUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetGifImage)
		DECL_LUA_FUNC(SetGifImage)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CGifAnimUI_h