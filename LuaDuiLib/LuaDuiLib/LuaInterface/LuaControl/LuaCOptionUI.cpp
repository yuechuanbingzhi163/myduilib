#include "StdAfx.h"

#include "LuaCOptionUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCOptionUI, New)
	{
		COptionUI  *ctrl = new COptionUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetClassName)
	{
		CDuiString pstrText = COptionUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetSelectedImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSelectedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetSelectedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetSelectedImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSelectedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetSelectedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetSelectedTextColor)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwBkCor = self->GetSelectedTextColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetSelectedTextColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetSelectedTextColor)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);
			self->SetSelectedTextColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetSelectedTextColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetSelectedHotImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSelectedHotImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetSelectedHotImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetSelectedHotImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSelectedHotImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetSelectedHotImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetSelectedPushedImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSelectedPushedImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetSelectedPushedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetSelectedPushedImage)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSelectedPushedImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetSelectedPushedImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetGroup)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetGroup();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetGroup"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetGroup)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetGroup(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetGroup"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, IsSelected)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l, self->IsSelected() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::IsSelected"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, Selected)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			bool bSelected = lua_toboolean(l, 2) != 0;
			bool bSendMsg = true;
			if(!lua_isnone(l,3))
				bSendMsg = lua_toboolean(l,3) != 0;
			self->Selected(bSelected,bSendMsg);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::Selected"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, IsNotFilterSelected)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l, self->IsNotFilterSelected() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::IsNotFilterSelected"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetNotFilterSelected)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			bool bNotFilterSelected = lua_toboolean(l, 2) != 0;
			self->SetNotFilterSelected(bNotFilterSelected);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetNotFilterSelected"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetSelectedToolTip)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSelectedToolTip();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetSelectedToolTip"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetSelectedToolTip)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSelectedToolTip(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetSelectedToolTip"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, GetNormalTooltip)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetNormalTooltip();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::GetNormalTooltip"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCOptionUI, SetNormalTooltip)
	{
		try
		{
			COptionUI* self;
			self = static_cast<COptionUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetNormalTooltip(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCOptionUI::SetNormalTooltip"));
			return 0;
		}
	}

	void LuaCOptionUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "OptionUI");

		luaL_newmetatable(l, "COptionUI");
		luaL_getmetatable(l, "CButtonUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{"GetSelectedImage", GetSelectedImage },
			{"SetSelectedImage", SetSelectedImage},
			{"SetSelectedTextColor", SetSelectedTextColor},
			{"GetSelectedTextColor", GetSelectedTextColor},
			{"GetSelectedHotImage", GetSelectedHotImage},
			{"SetSelectedHotImage", SetSelectedHotImage},
			{"GetSelectedPushedImage", GetSelectedPushedImage},
			{"SetSelectedPushedImage", SetSelectedPushedImage},
			{"GetGroup", GetGroup},
			{"SetGroup", SetGroup},
			{"IsSelected", IsSelected},
			{"Selected", Selected},
			{"SetNotFilterSelected", SetNotFilterSelected},
			{"IsNotFilterSelected", IsNotFilterSelected},
			{"SetSelectedToolTip", SetSelectedToolTip},
			{"GetSelectedToolTip", GetSelectedToolTip},
			{"SetNormalTooltip", SetNormalTooltip},
			{"GetNormalTooltip", GetNormalTooltip},
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}