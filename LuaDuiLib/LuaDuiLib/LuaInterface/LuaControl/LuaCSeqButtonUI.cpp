#include "StdAfx.h"

#include "LuaCSeqButtonUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCSeqButtonUI, New)
	{
		CSeqButtonUI  *ctrl = new CSeqButtonUI();
		LuaStatic::AddObject2Lua(l, ctrl, METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCSeqButtonUI, GetClassName)
	{
		CDuiString pstrText = CSeqButtonUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCSeqButtonUI,SetSeqImages)
	{
		try
		{
			CSeqButtonUI* self;
			self = static_cast<CSeqButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetSeqImages(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCSeqButtonUI::SetSeqImages"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCSeqButtonUI,GetSeqImages)
	{
		try
		{
			CSeqButtonUI* self;
			self = static_cast<CSeqButtonUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetSeqImages();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCSeqButtonUI::GetSeqImages"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCSeqButtonUI,SetTotalFrameCnt)
	{
		try
		{
			CSeqButtonUI* self;
			self = static_cast<CSeqButtonUI*>(LuaStatic::CheckUserData(l, 1));
			int nVal = lua_tointeger(l,2);
			self->SetTotalFrameCnt(nVal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCSeqButtonUI::SetTotalFrameCnt"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCSeqButtonUI,GetTotalFrameCnt)
	{
		try
		{
			CSeqButtonUI* self;
			self = static_cast<CSeqButtonUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetTotalFrameCnt());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCSeqButtonUI::GetTotalFrameCnt"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCSeqButtonUI,IsAnimationing)
	{
		try
		{
			CSeqButtonUI* self;
			self = static_cast<CSeqButtonUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsAnimationing() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCSeqButtonUI::IsAnimationing"));
			return 0;
		}
	}

	void LuaCSeqButtonUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "SeqButtonUI");

		luaL_newmetatable(l, "CSeqButtonUI");
		luaL_getmetatable(l, "CButtonUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "SetSeqImages", SetSeqImages },
			{ "GetSeqImages", GetSeqImages },
			{ "SetTotalFrameCnt", SetTotalFrameCnt },
			{ "GetTotalFrameCnt", GetTotalFrameCnt },
			{ "IsAnimationing", IsAnimationing },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}