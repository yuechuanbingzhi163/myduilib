#ifndef _Lua_CTileLayoutUI_h
#define _Lua_CTileLayoutUI_h
#pragma once


namespace DuiLib
{
	class LuaCTileLayoutUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetItemSize)
		DECL_LUA_FUNC(SetItemSize)
		DECL_LUA_FUNC(GetColumns)
		DECL_LUA_FUNC(SetColumns)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CTileLayoutUI_h