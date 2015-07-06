#include "StdAfx.h"

#include "LuaCButtonUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCButtonUI, New)
	{
		CButtonUI  *ctrl = new CButtonUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetClassName)
	{
		CDuiString pstrText = CButtonUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCButtonUI, SetNormalImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetNormalImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetNormalImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetNormalImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetNormalImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetNormalImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetHotImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetHotImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, SetHotImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetHotImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetHotImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetPushedImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetPushedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetPushedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, SetPushedImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetPushedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetPushedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetFocusedImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetFocusedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetFocusedImage"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetFocusedImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetFocusedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetFocusedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetDisabledImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetDisabledImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetDisabledImage"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetDisabledImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetDisabledImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetDisabledImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetForeImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetForeImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetForeImage"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetForeImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetForeImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetForeImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetHotForeImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetHotForeImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetHotForeImage"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetHotForeImage)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetHotForeImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetHotForeImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, SetHotBkColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetHotBkColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetHotBkColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetHotBkColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = self->GetHotBkColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetHotBkColor"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetHotTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetHotTextColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetHotTextColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCButtonUI, GetHotTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = self->GetHotTextColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetHotTextColor"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetPushedTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetPushedTextColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetPushedTextColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetPushedTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = self->GetPushedTextColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetPushedTextColor"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCButtonUI, SetFocusedTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			self->SetFocusedTextColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::SetFocusedTextColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCButtonUI, GetFocusedTextColor)
	{
		try
		{
			CButtonUI* self;
			self = static_cast<CButtonUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = self->GetFocusedTextColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCButtonUI::GetFocusedTextColor"));
			return 0;
		}
	}

	void LuaCButtonUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ButtonUI");

		luaL_newmetatable(l, "CButtonUI");
		luaL_getmetatable(l, "CControlUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{"SetNormalImage", SetNormalImage },
			{"GetNormalImage", GetNormalImage},
			{"GetHotImage", GetHotImage},
			{"SetHotImage", SetHotImage},
			{"GetPushedImage", GetPushedImage},
			{"SetPushedImage", SetPushedImage },
			{"GetFocusedImage", GetFocusedImage},
			{"SetFocusedImage", SetFocusedImage},
			{"GetDisabledImage", GetDisabledImage},
			{"SetDisabledImage", SetDisabledImage},
			{"GetForeImage", GetForeImage},
			{"SetForeImage", SetForeImage},
			{"GetHotForeImage", GetHotForeImage},
			{"SetHotForeImage", SetHotForeImage},
			{"SetHotBkColor", SetHotBkColor},
			{"GetHotBkColor", GetHotBkColor},
			{"SetHotTextColor", SetHotTextColor},
			{"GetHotTextColor", GetHotTextColor},
			{"SetPushedTextColor", SetPushedTextColor},
			{"GetPushedTextColor", GetPushedTextColor},
			{"SetFocusedTextColor", SetFocusedTextColor},
			{"GetFocusedTextColor", GetFocusedTextColor},
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}
