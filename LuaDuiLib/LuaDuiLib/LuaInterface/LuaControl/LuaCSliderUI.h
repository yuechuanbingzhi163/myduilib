#ifndef _Lua_CSliderUI_h
#define _Lua_CSliderUI_h
#pragma once


namespace DuiLib
{
	class LuaCSliderUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetChangeStep)
		DECL_LUA_FUNC(SetChangeStep)
		DECL_LUA_FUNC(SetThumbSize)
		DECL_LUA_FUNC(GetThumbRect)
		DECL_LUA_FUNC(GetThumbImage)
		DECL_LUA_FUNC(SetThumbImage)
		DECL_LUA_FUNC(GetThumbHotImage)
		DECL_LUA_FUNC(SetThumbHotImage)
		DECL_LUA_FUNC(GetThumbPushedImage)
		DECL_LUA_FUNC(SetThumbPushedImage)
		
		static void Register(lua_State* l);
	};
}


#endif//_Lua_CSliderUI_h