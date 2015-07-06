#ifndef _LuaCWindowWnd_H
#define _LuaCWindowWnd_H
#pragma once

namespace DuiLib
{
	class LuaCNotifyPump
	{
	public:
		DECL_LUA_FUNC(AddVirtualWnd)
		DECL_LUA_FUNC(RemoveVirtualWnd)
		DECL_LUA_FUNC(NotifyPump)
		DECL_LUA_FUNC(LoopDispatch)

		static void Register(lua_State* l);
	};

	class LuaCWindowWnd
	{
	public:
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		
		DECL_LUA_FUNC(GetHWND)
		DECL_LUA_FUNC(RegisterWindowClass)
		DECL_LUA_FUNC(RegisterSuperclass)
		DECL_LUA_FUNC(Create)
		DECL_LUA_FUNC(Create2)
		DECL_LUA_FUNC(CreateDuiWindow)
		DECL_LUA_FUNC(Subclass)
		DECL_LUA_FUNC(Unsubclass)

		DECL_LUA_FUNC(ShowWindow)
		DECL_LUA_FUNC(ShowModal)
		DECL_LUA_FUNC(Close)
		DECL_LUA_FUNC(CenterWindow)

		DECL_LUA_FUNC(SetIcon)
		DECL_LUA_FUNC(SendMessage)
		DECL_LUA_FUNC(PostMessage)
		DECL_LUA_FUNC(ResizeClient)

		static void Register(lua_State* l);
	};
}

#endif//_LuaCWindowWnd_H