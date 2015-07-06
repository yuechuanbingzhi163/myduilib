#include "StdAfx.h"

#include "LuaCWindowWnd.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCNotifyPump, AddVirtualWnd)
	{
		try
		{
			CNotifyPump* self;
			self = static_cast<CNotifyPump*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			CNotifyPump* pObject = static_cast<CNotifyPump*>(LuaStatic::CheckUserData(l, 2));
			lua_pushboolean(l, self->AddVirtualWnd(pstrText, pObject) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCNotifyPump::AddVirtualWnd"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCNotifyPump, RemoveVirtualWnd)
	{
		try
		{
			CNotifyPump* self;
			self = static_cast<CNotifyPump*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			lua_pushboolean(l, self->RemoveVirtualWnd(pstrText) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCNotifyPump::RemoveVirtualWnd"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCNotifyPump, NotifyPump)
	{
		try
		{
			CNotifyPump* self;
			self = static_cast<CNotifyPump*>(LuaStatic::CheckUserData(l, 1));
			TNotifyUI* pMsg = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 2));
			self->NotifyPump(*pMsg);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCNotifyPump::NotifyPump"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCNotifyPump, LoopDispatch)
	{
		try
		{
			CNotifyPump* self;
			self = static_cast<CNotifyPump*>(LuaStatic::CheckUserData(l, 1));
			TNotifyUI* pMsg = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 2));
			lua_pushboolean(l, self->LoopDispatch(*pMsg) ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCNotifyPump::LoopDispatch"));
			return 0;
		}
	}
	void LuaCNotifyPump::Register(lua_State* l)
	{
		lua_newtable(l); // t,

		lua_setfield(l, -2, "NotifyPump");
		
		luaL_newmetatable(l, "CNotifyPump");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "AddVirtualWnd",AddVirtualWnd },
			{ "RemoveVirtualWnd",RemoveVirtualWnd },
			{ "NotifyPump",NotifyPump },
			{ "LoopDispatch", LoopDispatch },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCWindowWnd, GetClassName)
	{
		lua_pushstring(l, "WindowWnd");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, isnil)
	{
		CWindowWnd* self;
		self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, _gc)
	{
		CWindowWnd* self;
		self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		if (::IsWindow(self->GetHWND()))
			SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, _eq)
	{
		try
		{
			CWindowWnd* left;
			left = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));

			CWindowWnd* right;
			right = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWindowWnd::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, _tostring)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "WindowWnd", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "WindowWnd", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCWindowWnd::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Delete)
	{
		CWindowWnd* self;
		self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		if (::IsWindow(self->GetHWND()) )
			SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, GetHWND)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::AddObject2Lua(l, self->GetHWND(), "HWND");
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::GetHWND"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, RegisterWindowClass)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->RegisterWindowClass() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::RegisterWindowClass"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, RegisterSuperclass)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->RegisterSuperclass() ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::RegisterSuperclass"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Create)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			HWND hwndParent = NULL;
			if (lua_isuserdata(l,2))
				hwndParent = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
			DWORD dwStyle = (DWORD)lua_tonumber(l,4);
			DWORD dwStyleEx = (DWORD)lua_tonumber(l,5);
			RECT rc = LuaStatic::ReadRectFromLua(l,6);
			HMENU hMenu = NULL;
			if(lua_isuserdata(l,7))
				hMenu = static_cast<HMENU>(LuaStatic::CheckUserData(l, 7));
			HWND hWnd = self->Create(hwndParent, pstrText, dwStyle, dwStyleEx, rc, hMenu);
			LuaStatic::AddObject2Lua(l, hWnd, "HWND");
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::Create"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Create2)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			HWND hwndParent = NULL;
			if (lua_isuserdata(l, 2))
				hwndParent = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
			DWORD dwStyle = (DWORD)lua_tonumber(l,4);
			DWORD dwStyleEx = (DWORD)lua_tonumber(l,5);
			int x = CW_USEDEFAULT;
			if(lua_isnumber(l,6))
				x = lua_tointeger(l,6);
			int y = CW_USEDEFAULT;
			if(lua_isnumber(l,7))
				y = lua_tointeger(l,7);
			int cx = CW_USEDEFAULT;
			if(lua_isnumber(l,8))
				cx = lua_tointeger(l,8);
			int cy = CW_USEDEFAULT;
			if(lua_isnumber(l,9))
				cy = lua_tointeger(l,9);
			HMENU hMenu = NULL;
			if(lua_isuserdata(l,10))
				hMenu = static_cast<HMENU>(LuaStatic::CheckUserData(l, 10));
			HWND hWnd = self->Create(hwndParent, pstrText, dwStyle, dwStyleEx, x, y, cx, cy, hMenu);
			LuaStatic::AddObject2Lua(l, hWnd, "HWND");
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::Create2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, CreateDuiWindow)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			HWND hwndParent = NULL;
			if (lua_isuserdata(l, 2))
				hwndParent = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
			DWORD dwStyle = 0;
			if(lua_isnumber(l,4))
				dwStyle = (DWORD)lua_tonumber(l,4);
			DWORD dwStyleEx = 0;
			if(lua_isnumber(l,5))
				dwStyleEx = (DWORD)lua_tonumber(l,5);
			HWND hWnd = self->CreateDuiWindow(hwndParent, pstrText, dwStyle, dwStyleEx);
			LuaStatic::AddObject2Lua(l, hWnd, "HWND");
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::CreateDuiWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Subclass)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			HWND hwndParent = static_cast<HWND>(LuaStatic::CheckUserData(l, 2));		
			HWND hWnd = self->Subclass(hwndParent);
			LuaStatic::AddObject2Lua(l, hWnd, "HWND");
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::Subclass"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Unsubclass)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			self->Unsubclass();
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::Unsubclass"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCWindowWnd, ShowWindow)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			bool bShow = true;
			if(lua_isboolean(l,2))
				bShow = lua_toboolean(l,2) != 0;
			bool bFocus = true;
			if(lua_isboolean(l,3))
				bFocus = lua_toboolean(l,3) != 0;
			self->ShowWindow(bShow,bFocus);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::ShowWindow"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, ShowModal)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->ShowModal());
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::ShowModal"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, Close)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			UINT nRet = IDOK;
			if(lua_isnumber(l,2))
				nRet = (UINT)lua_tonumber(l,2);
			self->Close(nRet);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::Close"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, CenterWindow)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			self->CenterWindow();
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::CenterWindow"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCWindowWnd, SetIcon)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			UINT nRes = (UINT)lua_tonumber(l,2);
			self->SetIcon(nRes);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::SetIcon"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, SendMessage)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			UINT uMsg = (UINT)lua_tonumber(l,2);
			WPARAM wParam = 0;
			if (lua_isuserdata(l, 3))
				wParam = (WPARAM)LuaStatic::CheckUserData(l, 3);
			else if (!lua_isnone(l, 4))
			{
				dui_uint32 wp = 0;
				lua_op_t<dui_uint32>::get_ret_value(l, 3, wp);
				wParam = wp;
			}
			LPARAM lParam = 0;
			if (lua_isuserdata(l, 4))
				lParam = (LPARAM)LuaStatic::CheckUserData(l, 4);
			else if (!lua_isnone(l, 4))
			{
				dui_uint64 lp = 0;
				lua_op_t<dui_uint64>::get_ret_value(l, 4, lp);
				lParam = lp;
			}
			LRESULT ret = self->SendMessage(uMsg,wParam,lParam);
			lua_pushnumber(l,ret);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::SendMessage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, PostMessage)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			UINT uMsg = (UINT)lua_tonumber(l,2);
			WPARAM wParam = 0;
			if (lua_isuserdata(l, 3))
				wParam = (WPARAM)LuaStatic::CheckUserData(l, 3);
			else if (!lua_isnone(l, 4))
			{
				dui_uint32 wp = 0;
				lua_op_t<dui_uint32>::get_ret_value(l, 3, wp);
				wParam = wp;
			}
			LPARAM lParam = 0;
			if (lua_isuserdata(l, 4))
				lParam = (LPARAM)LuaStatic::CheckUserData(l, 4);
			else if (!lua_isnone(l, 4))
			{
				dui_uint64 lp = 0;
				lua_op_t<dui_uint64>::get_ret_value(l, 4, lp);
				lParam = lp;
			}
			LRESULT ret = self->PostMessage(uMsg,wParam,lParam);
			lua_pushnumber(l,ret);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::PostMessage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCWindowWnd, ResizeClient)
	{
		try
		{
			CWindowWnd* self;
			self = static_cast<CWindowWnd*>(LuaStatic::CheckUserData(l, 1));
			int cx = -1;
			if(!lua_isnumber(l,2))
				cx = lua_tointeger(l,2);
			int cy = -1;
			if(!lua_isnumber(l,3))
				cy = lua_tointeger(l,3);
			self->ResizeClient(cx,cy);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCWindowWnd::PostMessage"));
			return 0;
		}
	}

	void LuaCWindowWnd::Register(lua_State* l)
	{
		lua_newtable(l); // t,
		static LuaReg StaticFuncs[] =
		{
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "WindowWnd");
		
		luaL_newmetatable(l, "CWindowWnd");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "GetHWND",GetHWND },
			{ "RegisterWindowClass", RegisterWindowClass },
			{ "RegisterSuperclass",RegisterSuperclass},
			{ "Create",Create },
			{ "Create2",Create2 },
			{ "CreateDuiWindow",CreateDuiWindow },
			{ "Subclass",Subclass},
			{ "Unsubclass", Unsubclass },
			{ "ShowWindow",ShowWindow},
			{ "ShowModal",ShowModal },
			{ "Close",Close },
			{ "CenterWindow",CenterWindow },
			{ "SetIcon",SetIcon},
			{ "SendMessage",SendMessage },
			{ "PostMessage",PostMessage },
			{ "ResizeClient",ResizeClient},
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
}