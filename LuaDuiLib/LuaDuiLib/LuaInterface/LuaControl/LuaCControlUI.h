#ifndef _Lua_CControlUI_h
#define _Lua_CControlUI_h
#pragma once


namespace DuiLib
{
	class LuaCControlUI
	{
	public:
		DECL_LUA_FUNC(isnil)
		DECL_LUA_FUNC(_gc)
		DECL_LUA_FUNC(_eq)
		DECL_LUA_FUNC(_tostring)
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(Delete)
		DECL_LUA_FUNC(IsClass)
		DECL_LUA_FUNC(IsName)
		DECL_LUA_FUNC(GetClassName)
		DECL_LUA_FUNC(GetName)
		DECL_LUA_FUNC(SetName)
		DECL_LUA_FUNC(GetClass)
		DECL_LUA_FUNC(GetInterface)
		DECL_LUA_FUNC(GetControlFlags)
		DECL_LUA_FUNC(Activate)
		DECL_LUA_FUNC(GetManager)
		DECL_LUA_FUNC(SetManager)
		DECL_LUA_FUNC(GetParent)
		DECL_LUA_FUNC(SetCustomStyle)
		DECL_LUA_FUNC(SetCustomStyle2)
		DECL_LUA_FUNC(GetText)
		DECL_LUA_FUNC(SetText)
		DECL_LUA_FUNC(GetBkColor)
		DECL_LUA_FUNC(SetBkColor)
		DECL_LUA_FUNC(GetBkColor2)
		DECL_LUA_FUNC(SetBkColor2)
		DECL_LUA_FUNC(GetBkColor3)
		DECL_LUA_FUNC(SetBkColor3)
		DECL_LUA_FUNC(GetBkImage)
		DECL_LUA_FUNC(SetBkImage)
		DECL_LUA_FUNC(GetFocusBorderColor)
		DECL_LUA_FUNC(SetFocusBorderColor)
		DECL_LUA_FUNC(IsColorHSL)
		DECL_LUA_FUNC(SetColorHSL)
		DECL_LUA_FUNC(GetBorderRound)
		DECL_LUA_FUNC(SetBorderRound)
		//DECL_LUA_FUNC(DrawImage)
		DECL_LUA_FUNC(GetBorderSize)
		DECL_LUA_FUNC(SetBorderSize)
		DECL_LUA_FUNC(GetBorderColor)
		DECL_LUA_FUNC(SetBorderColor)
		DECL_LUA_FUNC(SetBorderRect)
		DECL_LUA_FUNC(GetLeftBorderSize)
		DECL_LUA_FUNC(SetLeftBorderSize)
		DECL_LUA_FUNC(GetTopBorderSize)
		DECL_LUA_FUNC(SetTopBorderSize)
		DECL_LUA_FUNC(GetRightBorderSize)
		DECL_LUA_FUNC(SetRightBorderSize)
		DECL_LUA_FUNC(GetBottomBorderSize)
		DECL_LUA_FUNC(SetBottomBorderSize)
		DECL_LUA_FUNC(GetBorderStyle)
		DECL_LUA_FUNC(SetBorderStyle)
		DECL_LUA_FUNC(GetPos)
		DECL_LUA_FUNC(SetPos)
		DECL_LUA_FUNC(GetWidth)
		DECL_LUA_FUNC(GetHeight)
		DECL_LUA_FUNC(GetX)
		DECL_LUA_FUNC(GetY)
		DECL_LUA_FUNC(GetPadding)
		DECL_LUA_FUNC(SetPadding)
		DECL_LUA_FUNC(GetFixedXY)
		DECL_LUA_FUNC(SetFixedXY)
		DECL_LUA_FUNC(GetFixedWidth)
		DECL_LUA_FUNC(SetFixedWidth)
		DECL_LUA_FUNC(GetFixedHeight)
		DECL_LUA_FUNC(SetFixedHeight)
		DECL_LUA_FUNC(GetMinWidth)
		DECL_LUA_FUNC(SetMinWidth)
		DECL_LUA_FUNC(GetMaxWidth)
		DECL_LUA_FUNC(SetMaxWidth)
		DECL_LUA_FUNC(GetMinHeight)
		DECL_LUA_FUNC(SetMinHeight)
		DECL_LUA_FUNC(GetMaxHeight)
		DECL_LUA_FUNC(SetMaxHeight)
		DECL_LUA_FUNC(SetRelativePos)
		DECL_LUA_FUNC(SetRelativeParentSize)
		DECL_LUA_FUNC(GetRelativePos)
		DECL_LUA_FUNC(IsRelativePos)
		DECL_LUA_FUNC(GetSoundNameOver)
		DECL_LUA_FUNC(SetSoundNameOver)
		DECL_LUA_FUNC(GetSoundNameDown)
		DECL_LUA_FUNC(SetSoundNameDown)
		DECL_LUA_FUNC(GetStyleName)
		DECL_LUA_FUNC(SetStyleName)
		DECL_LUA_FUNC(GetToolTip)
		DECL_LUA_FUNC(SetToolTip)
		DECL_LUA_FUNC(SetToolTipWidth)
		DECL_LUA_FUNC(GetToolTipWidth)
		DECL_LUA_FUNC(GetShortcut)
		DECL_LUA_FUNC(SetShortcut)
		DECL_LUA_FUNC(IsContextMenuUsed)
		DECL_LUA_FUNC(SetContextMenuUsed)
		DECL_LUA_FUNC(GetUserData)
		DECL_LUA_FUNC(SetUserData)
		DECL_LUA_FUNC(GetTag)
		DECL_LUA_FUNC(SetTag)
		DECL_LUA_FUNC(GetTagCtrl)
		DECL_LUA_FUNC(SetTagCtrl)
		DECL_LUA_FUNC(IsVisible)
		DECL_LUA_FUNC(SetVisible)
		DECL_LUA_FUNC(SetInternVisible)
		DECL_LUA_FUNC(IsEnabled)
		DECL_LUA_FUNC(SetEnabled)
		DECL_LUA_FUNC(IsMouseEnabled)
		DECL_LUA_FUNC(SetMouseEnabled)
		DECL_LUA_FUNC(IsKeyboardEnabled)
		DECL_LUA_FUNC(SetKeyboardEnabled)
		DECL_LUA_FUNC(IsFocused)
		DECL_LUA_FUNC(SetFocus)
		DECL_LUA_FUNC(IsFloat)
		DECL_LUA_FUNC(SetFloat)
		DECL_LUA_FUNC(Invalidate)
		DECL_LUA_FUNC(IsUpdateNeeded)
		DECL_LUA_FUNC(NeedUpdate)
		DECL_LUA_FUNC(NeedParentUpdate)
		DECL_LUA_FUNC(GetAdjustColor)
		DECL_LUA_FUNC(SetAttribute)
		DECL_LUA_FUNC(ApplyAttributeList)
		DECL_LUA_FUNC(EstimateSize)
		DECL_LUA_FUNC(SetVirtualWnd)
		DECL_LUA_FUNC(GetVirtualWnd)


		static void Register(lua_State* l);
	};
}


#endif//_Lua_CControlUI_h