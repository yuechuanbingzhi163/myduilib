#include "StdAfx.h"

#include "LuaCHotKeyUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCHotKeyUI, New)
	{
		CHotKeyUI  *ctrl = new CHotKeyUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCHotKeyUI, GetClassName)
	{
		CDuiString pstrText = CHotKeyUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCHotKeyUI,SetNormalImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetNormalImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::SetNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,GetNormalImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetNormalImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::GetNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,SetHotImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetHotImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::SetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,GetHotImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::GetHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,SetFocusedImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetFocusedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::SetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,GetFocusedImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetFocusedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::GetFocusedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,SetDisabledImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetDisabledImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::SetDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI,GetDisabledImage)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetDisabledImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::GetDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI, SetNativeBkColor)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetNativeBkColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::SetNativeBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCHotKeyUI, GetNativeBkColor)
	{
		try
		{
			CHotKeyUI* self;
			self = static_cast<CHotKeyUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = self->GetNativeBkColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHotKeyUI::GetNativeBkColor"));
			return 0;
		}
	}

	void LuaCHotKeyUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "HotKeyUI");

		luaL_newmetatable(l, "CHotKeyUI");
		luaL_getmetatable(l, "CLabelUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetNormalImage", GetNormalImage },
			{ "SetNormalImage", SetNormalImage },
			{ "GetHotImage", GetHotImage },
			{ "SetHotImage", SetHotImage },
			{ "GetFocusedImage", GetFocusedImage },
			{ "SetFocusedImage", SetFocusedImage },
			{ "GetDisabledImage", GetDisabledImage },
			{ "SetDisabledImage", SetDisabledImage },
			{ "SetNativeBkColor", SetNativeBkColor },
			{ "GetNativeBkColor", GetNativeBkColor },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}