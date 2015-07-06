#include "StdAfx.h"

#include "LuaCProgressUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCProgressUI, New)
	{
		CProgressUI  *ctrl = new CProgressUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCProgressUI, GetClassName)
	{
		CDuiString pstrText = CProgressUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCProgressUI,IsHorizontal)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			bool bHori = self->IsHorizontal();
			lua_pushboolean(l,bHori ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::IsHorizontal"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCProgressUI,SetHorizontal)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			bool bHori = true;
			if (!lua_isnone(l, 2))
				bHori = lua_toboolean(l,2) != 0;
			self->SetHorizontal(bHori);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetHorizontal"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCProgressUI,IsStretchForeImage)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			bool bStretchForeImage = self->IsStretchForeImage();
			lua_pushboolean(l,bStretchForeImage ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::IsStretchForeImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCProgressUI,SetStretchForeImage)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			bool bStretchForeImage = true;
			if (!lua_isnone(l, 2))
				bStretchForeImage = lua_toboolean(l,2) != 0;
			self->SetStretchForeImage(bStretchForeImage);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetStretchForeImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCProgressUI,GetMinValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l, self->GetMinValue());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::GetMinValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,SetMinValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetMinValue(nVal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetMinValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,GetMaxValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l, self->GetMaxValue());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::GetMaxValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,SetMaxValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetMaxValue(nVal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetMaxValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,GetValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l, self->GetValue());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::GetValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,SetValue)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetValue(nVal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetValue"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCProgressUI,SetForeImage)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetForeImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::SetForeImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCProgressUI,GetForeImage)
	{
		try
		{
			CProgressUI* self;
			self = static_cast<CProgressUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetForeImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCProgressUI::GetForeImage"));
			return 0;
		}
	}

	void LuaCProgressUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ProgressUI");

		luaL_newmetatable(l, "CProgressUI");
		luaL_getmetatable(l, "CLabelUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "IsHorizontal", IsHorizontal },
			{ "SetHorizontal", SetHorizontal },
			{ "IsStretchForeImage", IsStretchForeImage },
			{ "SetStretchForeImage", SetStretchForeImage },
			{ "GetMinValue", GetMinValue },
			{ "SetMinValue", SetMinValue },
			{ "GetMaxValue", GetMaxValue },
			{ "SetMaxValue", SetMaxValue },
			{ "GetValue", GetValue },
			{ "SetValue", SetValue },
			{ "GetForeImage", GetForeImage },
			{ "SetForeImage", SetForeImage },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}