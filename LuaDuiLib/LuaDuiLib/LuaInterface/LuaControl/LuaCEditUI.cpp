#include "StdAfx.h"

#include "LuaCEditUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCEditUI, New)
	{
		CEditUI  *ctrl = new CEditUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCEditUI, GetClassName)
	{
		CDuiString pstrText = CEditUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCEditUI,SetMaxChar)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetMaxChar(nVal);		
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetMaxChar"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetMaxChar)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = self->GetMaxChar();
			lua_pushinteger(l,nVal);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetMaxChar"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetReadOnly)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			bool bReadOnly = lua_toboolean(l,2) != 0;
			self->SetReadOnly(bReadOnly);		
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetReadOnly"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,IsReadOnly)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsReadOnly() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::IsReadOnly"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetPasswordMode)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			bool bPassword = lua_toboolean(l,2) != 0;
			self->SetPasswordMode(bPassword);		
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetPasswordMode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,IsPasswordMode)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsPasswordMode() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::IsPasswordMode"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetPasswordChar)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			TCHAR ch = (TCHAR)lua_tointeger(l,2);
			self->SetPasswordChar(ch);		
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetPasswordChar"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetPasswordChar)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,(int)self->GetPasswordChar());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetPasswordChar"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetNumberOnly)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			bool bNumberOnly = lua_toboolean(l,2) != 0;
			self->SetNumberOnly(bNumberOnly);		
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetNumberOnly"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,IsNumberOnly)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsNumberOnly() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::IsNumberOnly"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetWindowStyls)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetWindowStyls());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetWindowStyls"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetNormalImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetNormalImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetNormalImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCEditUI,GetNormalImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetNormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetHotImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetHotImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetHotImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetFocusedImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetFocusedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetFocusedImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetFocusedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetDisabledImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetDisabledImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetDisabledImage)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetDisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI, SetNativeEditBkColor)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetNativeEditBkColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetNativeEditBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI, GetNativeEditBkColor)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = self->GetNativeEditBkColor();

			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetNativeEditBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetSel)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			int nStart = lua_tointeger(l,2);
			int nEnd = lua_tointeger(l,3);

			self->SetSel(nStart,nEnd);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetSel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetSelAll)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			self->SetSelAll();
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetSelAll"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetReplaceSel)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetReplaceSel(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetReplaceSel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,SetPrompt)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetPrompt(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::SetPrompt"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCEditUI,GetPrompt)
	{
		try
		{
			CEditUI* self;
			self = static_cast<CEditUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetPrompt();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCEditUI::GetPrompt"));
			return 0;
		}
	}

	void LuaCEditUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "EditUI");

		luaL_newmetatable(l, "CEditUI");
		luaL_getmetatable(l, "CLabelUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetMaxChar", SetMaxChar },
			{ "GetMaxChar", GetMaxChar },
			{ "SetReadOnly", SetReadOnly },
			{ "IsReadOnly", IsReadOnly },
			{ "SetPasswordMode", SetPasswordMode },
			{ "IsPasswordMode", IsPasswordMode },
			{ "SetPasswordChar", SetPasswordChar },
			{ "GetPasswordChar", GetPasswordChar },
			{ "SetNumberOnly", SetNumberOnly },
			{ "IsNumberOnly", IsNumberOnly },
			{ "GetWindowStyls", GetWindowStyls },
			{ "GetNormalImage", GetNormalImage },
			{ "SetNormalImage", SetNormalImage },
			{ "GetHotImage", GetHotImage },
			{ "SetHotImage", SetHotImage },
			{ "GetFocusedImage", GetFocusedImage },
			{ "SetFocusedImage", SetFocusedImage },
			{ "GetDisabledImage", GetDisabledImage },
			{ "SetDisabledImage", SetDisabledImage },
			{ "SetNativeEditBkColor", SetNativeEditBkColor },
			{ "GetNativeEditBkColor", GetNativeEditBkColor },
			{ "SetSel", SetSel },
			{ "SetSelAll", SetSelAll },
			{ "SetReplaceSel", SetReplaceSel },
			{ "SetPrompt", SetPrompt },
			{ "GetPrompt", GetPrompt },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}