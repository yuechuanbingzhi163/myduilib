#ifndef _Lua_CAnimationTabLayoutUI_h
#define _Lua_CAnimationTabLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCAnimationTabLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CAnimationTabLayoutUI_h