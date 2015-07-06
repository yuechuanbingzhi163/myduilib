#include "StdAfx.h"

#include "LuaCGifAnimUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCGifAnimUI, New)
	{
		CGifAnimUI  *ctrl = new CGifAnimUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCGifAnimUI, GetClassName)
	{
		CDuiString pstrText = CGifAnimUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCGifAnimUI,SetGifImage)
	{
		try
		{
			CGifAnimUI* self;
			self = static_cast<CGifAnimUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetGifImage(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCGifAnimUI::SetGifImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCGifAnimUI,GetGifImage)
	{
		try
		{
			CGifAnimUI* self;
			self = static_cast<CGifAnimUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetGifImage();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCGifAnimUI::GetGifImage"));
			return 0;
		}
	}

	void LuaCGifAnimUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "GifAnimUI");

		luaL_newmetatable(l, "CGifAnimUI");
		luaL_getmetatable(l, "CLabelUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetGifImage", GetGifImage },
			{ "SetGifImage", SetGifImage },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}