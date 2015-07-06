#ifndef _LuaRegister_h
#define _LuaRegister_h
#pragma once


namespace DuiLib
{
	class LuaRegister
	{
	public:
		static void Register(lua_State* l);
	};
}

#endif//_LuaRegister_h