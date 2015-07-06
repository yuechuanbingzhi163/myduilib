#ifndef _LuaWindowExport_H
#define _LuaWindowExport_H
#pragma once

namespace DuiLib
{
	class LuaWindowExport
	{
	public:
			DECL_LUA_FUNC(New)

			DECL_LUA_FUNC(_tostring)
	
			DECL_LUA_FUNC(GetWindowClassName)
			DECL_LUA_FUNC(GetClassStyle)
			DECL_LUA_FUNC(GetStyle)
			DECL_LUA_FUNC(GetExStyle)
			DECL_LUA_FUNC(ModifyStyle)
			DECL_LUA_FUNC(ModifyStyleEx)

			DECL_LUA_FUNC(DestroyWindow)
			DECL_LUA_FUNC(MoveWindow)
			DECL_LUA_FUNC(MoveWindowEx)
			DECL_LUA_FUNC(MoveWindowToPoint)
			DECL_LUA_FUNC(SetWindowPos)
			DECL_LUA_FUNC(SetWindowPosEx)
			DECL_LUA_FUNC(ArrangeIconicWindows)

			DECL_LUA_FUNC(ForegroundWindow)
			DECL_LUA_FUNC(BringWindowToTop)
			DECL_LUA_FUNC(GetWindowRect)
			DECL_LUA_FUNC(GetClientRect)
			DECL_LUA_FUNC(ClientToScreenForPoint)
			DECL_LUA_FUNC(ClientToScreenForRect)
			DECL_LUA_FUNC(ScreenToClientForPoint)
			DECL_LUA_FUNC(ScreenToClientForRect)
			DECL_LUA_FUNC(MapWindowPoints)
			DECL_LUA_FUNC(MapWindowPoints2)
			DECL_LUA_FUNC(UpdateWindow)

			DECL_LUA_FUNC(SetRedraw)
			DECL_LUA_FUNC(Invalidate)
			DECL_LUA_FUNC(InvalidateRect)
			DECL_LUA_FUNC(ValidateRect)
			DECL_LUA_FUNC(InvalidateRgn)
			DECL_LUA_FUNC(ValidateRgn)
			DECL_LUA_FUNC(IsWindow)
			DECL_LUA_FUNC(IsIconic)
			DECL_LUA_FUNC(IsZoomed)
			DECL_LUA_FUNC(IsWindowEnabled)
			DECL_LUA_FUNC(EnableWindow)
			DECL_LUA_FUNC(IsWindowVisible)
			DECL_LUA_FUNC(IsWindowUnicode)
			DECL_LUA_FUNC(IsParentDialog)

			DECL_LUA_FUNC(SetTimer)
			DECL_LUA_FUNC(KillTimer)
			DECL_LUA_FUNC(SetActiveWindow)
			DECL_LUA_FUNC(SetCapture)
			DECL_LUA_FUNC(SetFocus)
			DECL_LUA_FUNC(IsChild)
			DECL_LUA_FUNC(GetTopLevelWindow)
			DECL_LUA_FUNC(GetTopLevelParent)
			DECL_LUA_FUNC(GetParent)
			DECL_LUA_FUNC(SetParent)
			DECL_LUA_FUNC(FlashWindow)
			DECL_LUA_FUNC(AnimateWindow)
			DECL_LUA_FUNC(FadeInOut)
			DECL_LUA_FUNC(ShakeWindow)
			DECL_LUA_FUNC(FindControl)
			DECL_LUA_FUNC(MsgBox)

			DECL_LUA_FUNC(SetHotKey)
			DECL_LUA_FUNC(GetHotKey)
			DECL_LUA_FUNC(IsMainThread)
			DECL_LUA_FUNC(GetWindowThreadID)
			DECL_LUA_FUNC(GetWindowProcessID)
			DECL_LUA_FUNC(GetFont)
			DECL_LUA_FUNC(SetWindowText)
			DECL_LUA_FUNC(GetWindowText)
			DECL_LUA_FUNC(GetWindowTextLength)
			DECL_LUA_FUNC(LoadIcon)
			DECL_LUA_FUNC(LoadIconEx)
			DECL_LUA_FUNC(SetIcon)
			DECL_LUA_FUNC(GetIcon)
			DECL_LUA_FUNC(RegisterSkin)
			DECL_LUA_FUNC(UnRegisterSkin)
			DECL_LUA_FUNC(GetTag)
			DECL_LUA_FUNC(SetTag)

			DECL_LUA_FUNC(ExistsSkinFile)
			DECL_LUA_FUNC(GetPaintMgr)
			DECL_LUA_FUNC(IsValid)
			DECL_LUA_FUNC(ControlFromPtr)

			DECL_LUA_FUNC(SetListCallback)
			DECL_LUA_FUNC(AddPreMessageFilter)
			DECL_LUA_FUNC(AddMessageFilter)
			DECL_LUA_FUNC(AddNotifier)
			DECL_LUA_FUNC(ListenUIEvent)
			DECL_LUA_FUNC(ListenUIDestroy)
			DECL_LUA_FUNC(TouchMsgTable)


			static void LuaWindowExport::Register(lua_State* l);
	};
};

#endif//_LuaWindowExport_H