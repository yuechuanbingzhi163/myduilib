#ifndef _Lua_CRadioUI_h
#define _Lua_CRadioUI_h
#pragma once


namespace DuiLib
{
	class LuaCRadioUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		
		static void Register(lua_State* l);
	};
}


#endif//_Lua_CRadioUI_h