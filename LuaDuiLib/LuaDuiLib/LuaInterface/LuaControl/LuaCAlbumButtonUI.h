#ifndef _Lua_CAlbumButtonUI_h
#define _Lua_CAlbumButtonUI_h
#pragma once


namespace DuiLib
{
	class LuaCAlbumButtonUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetHotBkImage)
		DECL_LUA_FUNC(GetHotBkImage)
		DECL_LUA_FUNC(SetAlbumImage)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CAlbumButtonUI_h