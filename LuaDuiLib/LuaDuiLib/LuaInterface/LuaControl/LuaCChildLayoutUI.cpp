#include "StdAfx.h"

#include "LuaCChildLayoutUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCChildLayoutUI, New)
	{
		CChildLayoutUI  *ctrl = new CChildLayoutUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCChildLayoutUI, GetClassName)
	{
		CDuiString pstrText = CChildLayoutUI::GetClassName();
		
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCChildLayoutUI,Init)
	{
		try
		{
			CChildLayoutUI* self;
			self = static_cast<CChildLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			self->Init();			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCChildLayoutUI::Init"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCChildLayoutUI,SetChildLayoutXML)
	{
		try
		{
			CChildLayoutUI* self;
			self = static_cast<CChildLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->SetChildLayoutXML(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCChildLayoutUI::SetChildLayoutXML"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCChildLayoutUI,GetChildLayoutXML)
	{
		try
		{
			CChildLayoutUI* self;
			self = static_cast<CChildLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetChildLayoutXML();
			
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCChildLayoutUI::GetChildLayoutXML"));
			return 0;
		}
	}


	void LuaCChildLayoutUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ChildLayoutUI");

		luaL_newmetatable(l, "CChildLayoutUI");
		luaL_getmetatable(l, "CContainerUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "Init", Init },
			{ "SetChildLayoutXML", SetChildLayoutXML },
			{ "GetChildLayoutXML", GetChildLayoutXML },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}