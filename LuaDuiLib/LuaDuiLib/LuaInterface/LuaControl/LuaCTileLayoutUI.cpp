#include "StdAfx.h"

#include "LuaCTileLayoutUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCTileLayoutUI, New)
	{
		CTileLayoutUI  *ctrl = new CTileLayoutUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCTileLayoutUI, GetClassName)
	{
		CDuiString pstrText = CTileLayoutUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}


	IMPL_LUA_FUNC(LuaCTileLayoutUI,GetItemSize)
	{
		try
		{
			CTileLayoutUI* self;
			self = static_cast<CTileLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			SIZE sz = self->GetItemSize();
			LuaStatic::WriteSize2Lua(l, sz);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTileLayoutUI::GetItemSize"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCTileLayoutUI,SetItemSize)
	{
		try
		{
			CTileLayoutUI* self;
			self = static_cast<CTileLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			SIZE sz = LuaStatic::ReadSizeFromLua(l, 2);
			self->SetItemSize(sz);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTileLayoutUI::SetItemSize"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCTileLayoutUI,GetColumns)
	{
		try
		{
			CTileLayoutUI* self;
			self = static_cast<CTileLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			int w = self->GetColumns();
			lua_pushinteger(l,w);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCTileLayoutUI::GetColumns"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCTileLayoutUI,SetColumns)
	{
		try
		{
			CTileLayoutUI* self;
			self = static_cast<CTileLayoutUI*>(LuaStatic::CheckUserData(l, 1));
			int w = (int)lua_tointeger(l, 2);
			self->SetColumns(w);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCTileLayoutUI::SetColumns"));
			return 0;
		}
	}
	

	void LuaCTileLayoutUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "TileLayoutUI");

		luaL_newmetatable(l, "CTileLayoutUI");
		luaL_getmetatable(l, "CContainerUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetItemSize", GetItemSize },
			{ "SetItemSize", SetItemSize },
			{ "GetColumns", GetColumns },
			{ "SetColumns", SetColumns },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}