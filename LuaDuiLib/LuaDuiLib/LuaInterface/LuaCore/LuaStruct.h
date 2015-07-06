#ifndef _LuaStruct_H
#define _LuaStruct_H
#pragma once

namespace DuiLib
{
	class LuaTNotifyUI
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(getType)
		DECL_LUA_FUNC(getVirtualWnd)
		DECL_LUA_FUNC(getSender)
		DECL_LUA_FUNC(getTimestamp)
		DECL_LUA_FUNC(getPtMouse)
		DECL_LUA_FUNC(getWParam)
		DECL_LUA_FUNC(getLParam)
		DECL_LUA_FUNC(setType)
		DECL_LUA_FUNC(setVirtualWnd)
		DECL_LUA_FUNC(setSender)
		DECL_LUA_FUNC(setTimestamp)
		DECL_LUA_FUNC(setPtMouse)
		DECL_LUA_FUNC(setWParam)
		DECL_LUA_FUNC(setLParam)

		static void Register(lua_State* l);
	};

	class LuaTFontInfo
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(getFont)
		DECL_LUA_FUNC(getFontName)
		DECL_LUA_FUNC(getSize)
		DECL_LUA_FUNC(getBold)
		DECL_LUA_FUNC(getUnderline)
		DECL_LUA_FUNC(getItalic)
		DECL_LUA_FUNC(getTextmetric)
		DECL_LUA_FUNC(setFont)
		DECL_LUA_FUNC(setFontName)
		DECL_LUA_FUNC(setSize)
		DECL_LUA_FUNC(setBold)
		DECL_LUA_FUNC(setUnderline)
		DECL_LUA_FUNC(setItalic)
		DECL_LUA_FUNC(setTextmetric)

		static void Register(lua_State* l);
	};

	class LuaTImageInfo
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(getBitmap)
		DECL_LUA_FUNC(getX)
		DECL_LUA_FUNC(getY)
		DECL_LUA_FUNC(getAlphaChannel)
		DECL_LUA_FUNC(getResType)
		DECL_LUA_FUNC(getMask)
		DECL_LUA_FUNC(setBitmap)
		DECL_LUA_FUNC(setX)
		DECL_LUA_FUNC(setY)
		DECL_LUA_FUNC(setAlphaChannel)
		DECL_LUA_FUNC(setResType)
		DECL_LUA_FUNC(setMask)
		
		static void Register(lua_State* l);
	};

	class LuaTEventUI
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(getType)
		DECL_LUA_FUNC(getSender)
		DECL_LUA_FUNC(getTimestamp)
		DECL_LUA_FUNC(getPtMouse)
		DECL_LUA_FUNC(getChKey)
		DECL_LUA_FUNC(getKeyState)
		DECL_LUA_FUNC(getWParam)
		DECL_LUA_FUNC(getLParam)
		DECL_LUA_FUNC(setType)
		DECL_LUA_FUNC(setSender)
		DECL_LUA_FUNC(setTimestamp)
		DECL_LUA_FUNC(setPtMouse)
		DECL_LUA_FUNC(setChKey)
		DECL_LUA_FUNC(setKeyState)
		DECL_LUA_FUNC(setWParam)
		DECL_LUA_FUNC(setLParam)
		
		static void Register(lua_State* l);
	};

	class LuaDuiLib
	{
		DECL_LUA_FUNC(MsgBox)
		DECL_LUA_FUNC(TraceMsg)
		DECL_LUA_FUNC(QuitApp)
	public:
		static void Register(lua_State* l);
	};

	class LuaMsgMarco
	{
	public:
		static void Register(lua_State* l);
	};

	class LuaEventMarco
	{
	public:
		static void Register(lua_State* l);
	};
}

#endif//_LuaStruct_H