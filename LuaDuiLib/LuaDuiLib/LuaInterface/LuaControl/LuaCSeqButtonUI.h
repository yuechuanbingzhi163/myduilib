#ifndef _Lua_CSeqButtonUI_h
#define _Lua_CSeqButtonUI_h
#pragma once


namespace DuiLib
{
	class LuaCSeqButtonUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(SetSeqImages)
		DECL_LUA_FUNC(GetSeqImages)
		DECL_LUA_FUNC(SetTotalFrameCnt)
		DECL_LUA_FUNC(GetTotalFrameCnt)
		DECL_LUA_FUNC(IsAnimationing)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CSeqButtonUI_h