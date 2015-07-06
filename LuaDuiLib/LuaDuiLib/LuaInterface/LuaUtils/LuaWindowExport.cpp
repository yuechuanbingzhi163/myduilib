#include "StdAfx.h"

#include "../LuaCWindow.h"

#include "LuaWindowExport.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaWindowExport, New)
	{
		CDuiString pstrText;
		lua_op_t<CDuiString>::lua_to_value(l, 1, pstrText);
		UINT nClassStyle = CS_DBLCLKS;
		if(lua_isnumber(l,2))
			nClassStyle = (UINT)lua_tonumber(l,2);
		CLuaWindow* self = new CLuaWindow(pstrText, nClassStyle);
		LuaStatic::AddObject2Lua(l, self, "CLuaWindow");
		return 1;
	}
	IMPL_LUA_FUNC(LuaWindowExport, _tostring)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "LuaWindow", (void*)self);
				lua_op_t<CDuiString>::push_stack(l, str);
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "LuaWindow", "nil");
				lua_op_t<CDuiString>::push_stack(l, str);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWindowWnd::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowClassName)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetWindowClassName();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetWindowClassName"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetClassStyle)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->GetClassStyle());
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetClassStyle"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, GetStyle)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->GetStyle());
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetExStyle)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->GetExStyle());
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetExStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ModifyStyle)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwRemove = (DWORD)lua_tonumber(l,2);
			DWORD dwAdd = (DWORD)lua_tonumber(l,3);
			UINT nFlag = 0;
			if(lua_isnumber(l,4))
				nFlag = (UINT)lua_tonumber(l,4);
			lua_pushboolean(l,self->ModifyStyle(dwRemove,dwAdd,nFlag) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ModifyStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ModifyStyleEx)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwRemove = (DWORD)lua_tonumber(l,2);
			DWORD dwAdd = (DWORD)lua_tonumber(l,3);
			UINT nFlag = 0;
			if(lua_isnumber(l,4))
				nFlag = (UINT)lua_tonumber(l,4);
			lua_pushboolean(l,self->ModifyStyleEx(dwRemove,dwAdd,nFlag) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ModifyStyleEx"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, DestroyWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->DestroyWindow() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::DestroyWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MoveWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			int x = lua_tointeger(l,2);
			int y = lua_tointeger(l, 3);
			int w = lua_tointeger(l, 4);
			int h = lua_tointeger(l, 5);
			BOOL bRepaint = TRUE;
			if(lua_isboolean(l,6))
				bRepaint = lua_toboolean(l,6) != 0;
			lua_pushboolean(l,self->MoveWindow(x,y,w,h,bRepaint) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MoveWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MoveWindowEx)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			BOOL bRepaint = TRUE;
			if(lua_isboolean(l,3))
				bRepaint = lua_toboolean(l,3) != 0;
			lua_pushboolean(l,self->MoveWindow(&rc,bRepaint) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MoveWindowEx"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MoveWindowToPoint)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			POINT pt = LuaStatic::ReadPointFromLua(l,2);
			lua_pushboolean(l,self->MoveWindowToPoint(&pt) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MoveWindowToPoint"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetWindowPos)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			HWND hWndInsertAfter = NULL;
			if (lua_isuserdata(l, 2))
				hWndInsertAfter = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));
			int x = lua_tointeger(l, 3);
			int y = lua_tointeger(l, 4);
			int w = lua_tointeger(l, 5);
			int h = lua_tointeger(l, 6);
			UINT nFlags = (UINT)lua_tonumber(l,6) ;
			lua_pushboolean(l,self->SetWindowPos(hWndInsertAfter,x,y,w,h,nFlags) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::SetWindowPos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetWindowPosEx)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			HWND hWndInsertAfter = NULL;
			if (lua_isuserdata(l, 2))
				hWndInsertAfter = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));
			RECT rc = LuaStatic::ReadRectFromLua(l,3);
			UINT nFlags = (UINT)lua_tonumber(l,4) ;
			lua_pushboolean(l,self->SetWindowPos(hWndInsertAfter,&rc,nFlags) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::SetWindowPosEx"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ArrangeIconicWindows)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->ArrangeIconicWindows());
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ArrangeIconicWindows"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ForegroundWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->ForegroundWindow() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ForegroundWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, BringWindowToTop)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->BringWindowToTop() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::BringWindowToTop"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc;
			self->GetWindowRect(&rc);
			LuaStatic::WriteRect2Lua(l,rc);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetWindowRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetClientRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc;
			self->GetClientRect(&rc);
			LuaStatic::WriteRect2Lua(l,rc);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetClientRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ClientToScreenForPoint)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			POINT pt = LuaStatic::ReadPointFromLua(l,2);
			BOOL bRet = self->ClientToScreen(&pt);
			lua_pushboolean(l,bRet ? 1 : 0);
			LuaStatic::WritePoint2Lua(l,pt);
			return 2;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ClientToScreenForPoint"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ClientToScreenForRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			BOOL bRet = self->ClientToScreen(&rc);
			lua_pushboolean(l,bRet ? 1 : 0);
			LuaStatic::WriteRect2Lua(l, rc);
			return 2;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ClientToScreenForRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ScreenToClientForPoint)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			POINT pt = LuaStatic::ReadPointFromLua(l,2);
			BOOL bRet = self->ScreenToClient(&pt);
			lua_pushboolean(l,bRet ? 1 : 0);
			LuaStatic::WritePoint2Lua(l,pt);
			return 2;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ScreenToClientForPoint"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ScreenToClientForRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			BOOL bRet = self->ScreenToClient(&rc);
			lua_pushboolean(l,bRet ? 1 : 0);
			LuaStatic::WriteRect2Lua(l, rc);
			return 2;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ScreenToClientForRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MapWindowPoints)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MapWindowPoints2)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, UpdateWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			BOOL bRet = self->UpdateWindow();
			lua_pushboolean(l,bRet ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::UpdateWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetRedraw)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			BOOL bRet = TRUE;
			if(lua_isboolean(l,2))
				bRet = lua_toboolean(l, 2) != 0;
			self->SetRedraw(bRet);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::SetRedraw"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, Invalidate)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			BOOL bRet = TRUE;
			if(lua_isboolean(l,2))
				bRet = lua_toboolean(l,2) != 0;
			self->Invalidate(bRet);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::Invalidate"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, InvalidateRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			BOOL bErase = TRUE;
			if(lua_isboolean(l,3))
				bErase = lua_toboolean(l,3) != 0;
			self->InvalidateRect(&rc,bErase);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::InvalidateRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ValidateRect)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			self->ValidateRect(&rc);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ValidateRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, InvalidateRgn)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			HRGN hRgn = static_cast<HRGN>(LuaStatic::CheckUserData(l, 2));
			BOOL bErase = TRUE;
			if(lua_isboolean(l,3))
				bErase = lua_toboolean(l,3) != 0;
			self->InvalidateRgn(hRgn,bErase);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::InvalidateRgn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ValidateRgn)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			HRGN hRgn = static_cast<HRGN>(LuaStatic::CheckUserData(l, 2));
			self->ValidateRgn(hRgn);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::ValidateRgn"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsWindow() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsIconic)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsIconic() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsIconic"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsZoomed)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsZoomed() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsZoomed"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsWindowEnabled)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsWindowEnabled() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsWindowEnabled"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, EnableWindow)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			BOOL bEnable = TRUE;
			if(lua_isboolean(l,2))
				bEnable = lua_toboolean(l,2) != 0;
			self->EnableWindow(bEnable);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::EnableWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsWindowVisible)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsWindowVisible() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsWindowVisible"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsWindowUnicode)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsWindowUnicode() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsWindowUnicode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsParentDialog)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsParentDialog() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::IsParentDialog"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetTimer)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			long nID = (long)lua_tonumber(l, 2);
			UINT uElpase = (UINT)lua_tonumber(l, 3);
			long nTimerID = self->SetTimer(nID, uElpase);
			lua_pushnumber(l, nTimerID);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::SetTimer"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, KillTimer)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			long nID = (long)lua_tonumber(l, 2);
			self->KillTimer(nID);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetActiveWindow)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetCapture)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetFocus)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsChild)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetTopLevelWindow)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetTopLevelParent)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetParent)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetParent)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, FlashWindow)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, AnimateWindow)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, FadeInOut)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ShakeWindow)
	{
		try
		{

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, FindControl)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			CControlUI* pControl = NULL;
			self->FindControl(pControl,pstrText);
			if (pControl)
				LuaStatic::AddObject2Lua(l, pControl, METATABLE_NAME(pControl));
			else
				lua_pushnil(l);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::FindControl"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, MsgBox)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			CDuiString lpCaption;
			if (lua_isstring(l, 3))
				lua_op_t<CDuiString>::lua_to_value(l, 3, lpCaption);
			UINT nType = MBBT_OKCANCLE;
			if (lua_isnumber(l, 4))
				nType = (UINT)lua_tonumber(l, 4);
			int lua_callback = -1;
			if (lua_isfunction(l, 5))
			{
				lua_callback = luaL_ref(l, LUA_REGISTRYINDEX);
			}
			if (true)//self->ExistsSkinFile("msgbox.xml"))
			{
				UINT nRet = self->MsgBox(pstrText, lpCaption, nType);
				if (lua_callback != -1)
				{
					lua_rawgeti(l, LUA_REGISTRYINDEX, lua_callback);
					lua_pushnumber(l, nRet);
					lua_call(l, 1, 0);
				}
			}
			else
			{
				int Type = MB_OK;
				if ((nType & MBT_OK) > 0)
					Type |= MB_OK;
				if ((nType & MBT_INFO) > 0)
					Type |= MB_ICONINFORMATION;
				if ((nType & MBT_WARN) > 0)
					Type |= MB_ICONWARNING;
				if ((nType & MBBT_CANCEL) >0)
					Type |= MB_OKCANCEL &(~MB_OK);
				if ((nType & MBBT_OK) >0)
					Type |= MB_OK;
				if ((nType & MBBT_OKCANCLE) > 0)
					Type |= MB_OKCANCEL;
				
				UINT nRet = ::MessageBox(*self, pstrText, lpCaption, Type);
				if (lua_callback != -1)
				{
					lua_rawgeti(l, LUA_REGISTRYINDEX, lua_callback);
					lua_pushnumber(l, nRet);
					lua_call(l, 1, 0);
					lua_pop(l,1);
				}
			}
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MsgBox"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetHotKey)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetHotKey)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, IsMainThread)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowThreadID)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowProcessID)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetFont)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetWindowText)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowText)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetWindowTextLength)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, LoadIcon)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, LoadIconEx)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetIcon)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetIcon)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, RegisterSkin)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			STRINGorID xml(_T(""));
			if (lua_isnumber(l, 2))
				xml = STRINGorID(lua_tointeger(l, 2));
			else if (lua_isstring(l, 2))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
				xml = STRINGorID(pstrText.GetData());
			}
			LPCTSTR type = NULL;
			if (lua_isstring(l, 3))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
				type = pstrText.GetData();
			}
			IDialogBuilderCallback* pCallBack = static_cast<IDialogBuilderCallback*>(self);
			CControlUI* pParent = NULL;
			if (lua_isuserdata(l,5))
				pParent = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 5));
			lua_pushboolean(l, self->RegisterSkin(xml, type, pCallBack, pParent) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, UnRegisterSkin)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, GetTag)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetTag)
	{
		try
		{
			
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::MapWindowPoints2"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, GetPaintMgr)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::AddObject2Lua(l, self->PaintMgr(), METATABLE_NAME(self->PaintMgr()));
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaWindowExport::GetPaintMgr"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ExistsSkinFile)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			STRINGorID xml(_T(""));
			if (lua_isnumber(l, 2))
				xml = STRINGorID(lua_tointeger(l, 2));
			else if (lua_isstring(l, 2))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
				xml = STRINGorID(pstrText.GetData());
			}
			LPCTSTR type = NULL;
			if (lua_isstring(l, 3))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
				type = pstrText.GetData();
			}
			lua_pushboolean(l, self->ExistsSkinFile(xml,type) ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::ExistsSkinFile"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, IsValid)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			lua_op_t<BOOL>::push_stack(l, self->IsValid());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::IsValid"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, ControlFromPtr)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			dui_int32 ptr = 0;
			lua_op_t<dui_int32>::get_ret_value(l, 2, ptr);
			IDuiObject* pControl = (IDuiObject*)ptr;
			LuaStatic::AddObject2Lua(l, pControl, METATABLE_NAME(pControl));
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::ControlFromPtr"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, SetListCallback)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CListUI* pList = static_cast<CListUI*>(LuaStatic::CheckUserData(l, 2));
			pList->SetTextCallback(self);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::SetListCallback"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, AddPreMessageFilter)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			self->PaintMgr()->AddPreMessageFilter(self);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::AddPreMessageFilter"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, AddMessageFilter)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			self->PaintMgr()->AddMessageFilter(self);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::AddMessageFilter"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, AddNotifier)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			self->PaintMgr()->AddNotifier(self);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::AddNotifier"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaWindowExport, ListenUIEvent)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			pControl->OnEvent += MakeDelegate(self, &CLuaWindow::onUIEvent);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::ListenUIEvent"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaWindowExport, ListenUIDestroy)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			pControl->OnDestroy += MakeDelegate(self, &CLuaWindow::onUIDestroy);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::ListenUIEvent"));
			return 0;
		}
	}


	IMPL_LUA_FUNC(LuaWindowExport,TouchMsgTable)
	{
		try
		{
			CLuaWindow* self;
			self = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
			
			//创建索引并从栈顶弹出该对象
			lua_pushvalue(l, 2);
			int nLuaObjRef = luaL_ref(l, LUA_REGISTRYINDEX);
			lua_pushvalue(l, 3);
			int nLuaMsgRef = luaL_ref(l, LUA_REGISTRYINDEX);
			self->TouchMsgTable(nLuaObjRef, nLuaMsgRef);

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::TouchMsgTable"));
			return 0;
		}
	}
	
	void LuaWindowExport::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "LuaWindow");

		luaL_newmetatable(l, "CLuaWindow");
		luaL_getmetatable(l, "CWindowWnd");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "__tostring", _tostring },
			{ "GetWindowClassName", GetWindowClassName },
			{ "GetClassStyle", GetClassStyle },
			{ "GetStyle", GetStyle },
			{ "GetExStyle", GetExStyle },
			{ "ModifyStyle", ModifyStyle },
			{ "ModifyStyleEx", ModifyStyleEx },
			{ "DestroyWindow", DestroyWindow },
			{ "MoveWindow", MoveWindow },
			{ "MoveWindowEx", MoveWindowEx },
			{ "MoveWindowToPoint", MoveWindowToPoint },
			{ "RegisterSkin", RegisterSkin },
			{ "IsZoomed", IsZoomed },
			{ "FindControl", FindControl },
			{ "GetPaintMgr", GetPaintMgr },
			{ "MsgBox", MsgBox },
			{ "ExistsSkinFile", ExistsSkinFile },
			{ "IsWindow", IsWindow },
			{ "IsValid", IsValid },
			{ "SetTimer", SetTimer },
			{ "KillTimer", KillTimer },
			{ "ControlFromPtr", ControlFromPtr },
			{ "SetListCallback", SetListCallback },
			{ "AddPreMessageFilter", AddPreMessageFilter },
			{ "AddMessageFilter", AddMessageFilter },
			{ "AddNotifier", AddNotifier },
			{ "ListenUIEvent", ListenUIEvent },
			{ "ListenUIDestroy", ListenUIDestroy },
			{ "TouchMsgTable", TouchMsgTable },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
}