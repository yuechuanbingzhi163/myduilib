#ifndef _Lua_CTextUI_h
#define _Lua_CTextUI_h
#pragma once


namespace DuiLib
{
	class LuaCTextUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetLinkContent)
		
		static void Register(lua_State* l);
	};
}


#endif//_Lua_CTextUI_h