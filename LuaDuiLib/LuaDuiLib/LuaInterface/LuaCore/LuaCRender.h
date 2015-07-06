#ifndef _LuaCRender_H
#define _LuaCRender_H
#pragma once

namespace DuiLib
{
	class LuaCRenderClip
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GenerateClip)
		DECL_LUA_FUNC(GenerateRoundClip)
		DECL_LUA_FUNC(UseOldClipBegin)
		DECL_LUA_FUNC(UseOldClipEnd)

		DECL_LUA_FUNC(getRectItem)
		DECL_LUA_FUNC(getHDC)
		DECL_LUA_FUNC(getHRGN)
		DECL_LUA_FUNC(getOldHRGN)

		DECL_LUA_FUNC(setRectItem)
		DECL_LUA_FUNC(setHDC)
		DECL_LUA_FUNC(setHRGN)
		DECL_LUA_FUNC(setOldHRGN)

		static void Register(lua_State* l);
	};

	class LuaCRenderEngine
	{
	public:
		DECL_LUA_FUNC(AdjustColor)
		DECL_LUA_FUNC(LoadBitmap)
		DECL_LUA_FUNC(LoadImage)
		DECL_LUA_FUNC(FreeImage)
		DECL_LUA_FUNC(DrawImage)
		DECL_LUA_FUNC(DrawImageString)
		DECL_LUA_FUNC(DrawColor)
		DECL_LUA_FUNC(DrawGradient)
		DECL_LUA_FUNC(DrawLine)
		DECL_LUA_FUNC(DrawRect)
		DECL_LUA_FUNC(DrawRoundRect)
		DECL_LUA_FUNC(DrawText)
		DECL_LUA_FUNC(DrawHtmlText)
		DECL_LUA_FUNC(GenerateBitmap)
		DECL_LUA_FUNC(GetTextSize)

		static void Register(lua_State* l);
	};
}

#endif//_LuaCRender_H