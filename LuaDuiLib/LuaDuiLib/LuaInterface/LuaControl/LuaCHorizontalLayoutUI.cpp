#include "StdAfx.h"

#include "LuaCHorizontalLayoutUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI, New)
	{
		CHorizontalLayoutUI  *ctrl = new CHorizontalLayoutUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI, GetClassName)
	{
		CDuiString pstrText = CHorizontalLayoutUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI,SetSepWidth)
	{
		try
		{
			CHorizontalLayoutUI* self;
			self = static_cast<CHorizontalLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			int w = (int)lua_tointeger(l, 2);
			self->SetSepWidth(w);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHorizontalLayoutUI::SetSepWidth"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI,GetSepWidth)
	{
		try
		{
			CHorizontalLayoutUI* self;
			self = static_cast<CHorizontalLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			int w = self->GetSepWidth();
			lua_pushinteger(l,w);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHorizontalLayoutUI::GetSepWidth"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI,SetSepImmMode)
	{
		try
		{
			CHorizontalLayoutUI* self;
			self = static_cast<CHorizontalLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			bool bAuto = lua_toboolean(l, 2) != 0;
			self->SetSepImmMode(bAuto);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCHorizontalLayoutUI::SetSepImmMode"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI,IsSepImmMode)
	{
		try
		{
			CHorizontalLayoutUI* self;
			self = static_cast<CHorizontalLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			bool bRet = self->IsSepImmMode();
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHorizontalLayoutUI::IsSepImmMode"));
			return 0;
		}
	}	

	IMPL_LUA_FUNC(LuaCHorizontalLayoutUI,GetThumbRect)
	{
		try
		{
			CHorizontalLayoutUI* self;
			self = static_cast<CHorizontalLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			bool bUseNew = false;
			if (!lua_isnone(l,2))
				bUseNew = lua_toboolean(l,2) != 0;
			RECT rt = self->GetThumbRect(bUseNew);

			LuaStatic::WriteRect2Lua(l, rt);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCHorizontalLayoutUI::GetThumbRect"));
			return 0;
		}
	}


	void LuaCHorizontalLayoutUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "HorizontalLayoutUI");

		luaL_newmetatable(l, "CHorizontalLayoutUI");
		luaL_getmetatable(l, "CContainerUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetSepWidth", SetSepWidth },
			{ "GetSepWidth", GetSepWidth },
			{ "SetSepImmMode", SetSepImmMode },
			{ "IsSepImmMode", IsSepImmMode },
			{ "GetThumbRect", GetThumbRect },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}