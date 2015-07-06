#include "StdAfx.h"

#include "LuaCControlUI.h"

namespace DuiLib
{

	IMPL_LUA_FUNC(LuaCControlUI, isnil)
	{
		CControlUI* self;
		self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCControlUI, New)
	{
		CControlUI  *ctrl = new CControlUI();
		LuaStatic::AddObject2Lua(l,ctrl,METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCControlUI, Delete)
	{
		CControlUI* self;
		self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

		LuaStatic::RemoveObjectInLua(l);

		SafeDelete(self);

		return 0;
	}

	IMPL_LUA_FUNC(LuaCControlUI, _gc)
	{	
		CControlUI* self;
		self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

		LuaStatic::RemoveObjectInLua(l);

		SafeDelete(self);

		return 0;
	}

	IMPL_LUA_FUNC(LuaCControlUI, _eq)
	{
		try
		{
			CControlUI* left;
			left = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CControlUI* right;
			right = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));

			if (left == right) {
				lua_pushboolean(l, 1);
			}
			else {
				lua_pushboolean(l, 0);
			}

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::_eq"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, _tostring)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p,name:%s)"), self->GetClass(), (void*)self, self->GetName().GetData());
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "CControlUI", "nil");
				lua_pushstring(l, str.str().c_str());
			}

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::_tostring"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsClass)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			if (ctrl)
			{
				lua_pushboolean(l, ctrl->IsClass(pstrText.GetData()) ? 1 : 0);
			}
			else
			{
				lua_pushboolean(l, 0);
			}

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsClass"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsName)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			if (ctrl)
			{
				lua_pushboolean(l, ctrl->IsName(pstrText.GetData()) ? 1 : 0);
			}
			else
			{
				lua_pushboolean(l, 0);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsName"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetClassName)
	{
		CDuiString pstrText = CControlUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetName)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			if (ctrl)
			{
				CDuiString pstrText = ctrl->GetName();

				lua_op_t<CDuiString>::push_stack(l, pstrText);
			}
			else
			{
				lua_pushnil(l);
			}

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetName"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetName)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			if (ctrl)
			{
				ctrl->SetName(pstrText.GetData());
			}

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetName"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetClass)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			if (ctrl)
			{
				CDuiString pstrText = ctrl->GetClass();

				lua_op_t<CDuiString>::push_stack(l, pstrText);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetClass"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetInterface)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			LPVOID pInter = self->GetInterface(pstrText.GetData());

			if (pInter)
			{
				CControlUI* pControl = static_cast<CControlUI*>(pInter);
				LuaStatic::AddObject2Lua(l, pControl, METATABLE_NAME(pControl));
			}
			else
				lua_pushnil(l);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetInterface"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetControlFlags)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			if (ctrl)
			{
				UINT   flags;
				flags = ctrl->GetControlFlags();

				lua_pushinteger(l, (lua_Integer)flags);
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetControlFlags"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, Activate)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			if (ctrl)
			{
				lua_pushboolean(l, ctrl->Activate() ? 1 : 0);
			}
			else
			{
				lua_pushboolean(l, 0);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::Activate"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetManager)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			if (ctrl && ctrl->GetManager())
			{
				LuaStatic::AddObject2Lua(l, ctrl->GetManager(), METATABLE_NAME(ctrl));
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetManager"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetManager)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CPaintManagerUI* pm;
			pm = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 2));

			CControlUI* parent;
			parent = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 3));

			bool       bInit = true;
			if (!lua_isnil(l,4))
				bInit = lua_toboolean(l, 4) != 0;

			self->SetManager(pm, parent, bInit);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetManager"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetParent)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			if (self && self->GetParent())
			{
				LuaStatic::AddObject2Lua(l, self->GetParent(),METATABLE_NAME(self->GetParent()));
			}
			else
			{
				lua_pushnil(l);
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetParent"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetCustomStyle)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

		
			self->SetCustomStyle(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetCustomStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetCustomStyle2)
	{
		try
		{
			CControlUI* self;
			self = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			CStdStringPtrMap* pMap = NULL;
			pMap = static_cast<CStdStringPtrMap*>(LuaStatic::CheckUserData(l, 2));

			self->SetCustomStyle2(pMap);

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetCustomStyle2"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetText)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = ctrl->GetText();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetText"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetText)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			ctrl->SetText(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetText"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetBkColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			DWORD dwBkCor = ctrl->GetBkColor();
			
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBkColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBkColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			ctrl->SetBkColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBkColor"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCControlUI, GetBkColor2)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = ctrl->GetBkColor2();

			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBkColor2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBkColor2)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			ctrl->SetBkColor2(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBkColor2"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCControlUI, GetBkColor3)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = ctrl->GetBkColor3();
			LuaStatic::WriteRGBA2Lua(l, dwBkCor);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBkColor3"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBkColor3)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			ctrl->SetBkColor3(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBkColor3"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCControlUI, GetBkImage)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = ctrl->GetBkImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBkImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBkImage)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			ctrl->SetBkImage(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBkImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetFocusBorderColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = ctrl->GetFocusBorderColor();

			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetFocusBorderColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetFocusBorderColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			ctrl->SetFocusBorderColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFocusBorderColor"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCControlUI, IsColorHSL)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			bool isHSL = ctrl->IsColorHSL();

			lua_pushboolean(l, isHSL ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsColorHSL"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetColorHSL)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int isHSL = lua_toboolean(l, 2);

			ctrl->SetColorHSL(isHSL != 0);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetColorHSL"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetBorderRound)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE rd = ctrl->GetBorderRound();

			LuaStatic::WriteSize2Lua(l, rd);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBorderRound"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBorderRound)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE rd = LuaStatic::ReadSizeFromLua(l, 2);
			ctrl->SetBorderRound(rd);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBorderRound"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetBorderSize();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetBorderSize(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBorderSize"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetBorderColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = ctrl->GetBorderColor();

			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBorderColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBorderColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			DWORD dwBkCor = LuaStatic::ReadRGBAFromLua(l, 2);

			ctrl->SetBorderColor(dwBkCor);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBorderColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetBorderRect)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			RECT rd = LuaStatic::ReadRectFromLua(l, 2);
			
			ctrl->SetBorderSize(rd);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBorderRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetLeftBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetLeftBorderSize();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetLeftBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetLeftBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetLeftBorderSize(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetLeftBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetTopBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetTopBorderSize();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetTopBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetTopBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetTopBorderSize(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetTopBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetRightBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetRightBorderSize();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetRightBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetRightBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetRightBorderSize(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetRightBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetBottomBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetBottomBorderSize();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBottomBorderSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBottomBorderSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetBottomBorderSize(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBottomBorderSize"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetBorderStyle)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetBorderStyle();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetBorderStyle"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetBorderStyle)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetBorderStyle(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetBorderStyle"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetPos)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			RECT rt = ctrl->GetPos();

			LuaStatic::WriteRect2Lua(l, rt);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetPos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetPos)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			RECT rt = LuaStatic::ReadRectFromLua(l, 2);
			ctrl->SetPos(rt);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetPos"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetWidth();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetHeight();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetX)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetX();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetX"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetY)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetY();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetY"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetPadding)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			RECT rt = ctrl->GetPadding();

			LuaStatic::WriteRect2Lua(l, rt);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetPadding"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetPadding)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			RECT rt = LuaStatic::ReadRectFromLua(l, 2);
			ctrl->SetPadding(rt);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetPadding"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetFixedXY)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE size = ctrl->GetFixedXY();

			//
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetFixedXY"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetFixedXY)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			//
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFixedXY"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetFixedWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetFixedWidth();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetFixedWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetFixedWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetFixedWidth(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFixedWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetFixedHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetFixedHeight();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetFixedHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetFixedHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetFixedWidth(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFixedHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetMinWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetMinWidth();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetMinWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetMinWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetMinWidth(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetMinWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetMaxWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetMaxWidth();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetMaxWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetMaxWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetMaxWidth(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetMaxWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetMinHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetMinHeight();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetMinHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetMinHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetMinHeight(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetMinHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetMaxHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetMaxHeight();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetMaxHeight"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetMaxHeight)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetMaxHeight(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetMaxHeight"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetRelativePos)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE move = LuaStatic::ReadSizeFromLua(l, 2);
			SIZE zoom = LuaStatic::ReadSizeFromLua(l, 3);

			ctrl->SetRelativePos(move,zoom);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetRelativePos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetRelativeParentSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE sz = LuaStatic::ReadSizeFromLua(l, 2);

			ctrl->SetRelativeParentSize(sz);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetRelativeParentSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetRelativePos)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			//int size = ctrl->GetRelativePos();

			//lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetRelativePos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, IsRelativePos)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsRelativePos() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsRelativePos"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetSoundNameOver)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText = ctrl->GetSoundNameOver();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetSoundNameOver"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetSoundNameOver)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->SetSoundNameOver(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetSoundNameOver"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetSoundNameDown)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText = ctrl->GetSoundNameDown();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetSoundNameDown"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetSoundNameDown)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->SetSoundNameDown(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetSoundNameDown"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetStyleName)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText = ctrl->GetStyleName();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetStyleName"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetStyleName)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			
			ctrl->SetStyleName(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetStyleName"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetToolTip)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText = ctrl->GetToolTip();

			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetToolTip"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetToolTip)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->SetToolTip(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetToolTip"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, GetToolTipWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetToolTipWidth();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetToolTipWidth"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetToolTipWidth)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetToolTipWidth(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetToolTipWidth"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetShortcut)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = ctrl->GetShortcut();

			lua_pushinteger(l, (lua_Integer)size);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetShortcut"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetShortcut)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			int size = (int)lua_tointeger(l, 2);

			ctrl->SetShortcut(size);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetShortcut"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsContextMenuUsed)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsContextMenuUsed() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsContextMenuUsed"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetContextMenuUsed)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			bool m = lua_toboolean(l, 2) != 0;

			ctrl->SetContextMenuUsed(m);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetContextMenuUsed"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetUserData)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString sName = ctrl->GetUserData();
			lua_pushstring(l, sName.str().c_str());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetUserData"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetUserData)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->SetUserData(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetUserData"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetTag)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushinteger(l, (int)ctrl->GetTag());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetTag"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetTag)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetTag(lua_tointeger(l,2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetTag"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetTagCtrl)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* tagCtrl;
			tagCtrl = ctrl->GetTagCtrl();
			if (tagCtrl)
			{
				LuaStatic::AddObject2Lua(l, tagCtrl,METATABLE_NAME(tagCtrl));
			}
			else
				lua_pushnil(l);
			
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetTagCtrl"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetTagCtrl)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* tagCtrl;
			tagCtrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));

			ctrl->SetTagCtrl(tagCtrl);

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetTagCtrl"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsVisible)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsVisible() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsVisible"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetVisible)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetVisible(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetVisible"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetInternVisible)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetInternVisible(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetInternVisible"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsEnabled() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsEnabled"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetEnabled(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetEnabled"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsMouseEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsMouseEnabled() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsMouseEnabled"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetMouseEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetMouseEnabled(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetMouseEnabled"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsKeyboardEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsKeyboardEnabled() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsKeyboardEnabled"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetKeyboardEnabled)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetKeyboardEnabled(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetKeyboardEnabled"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsFocused)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsFocused() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsFocused"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetFocus)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			ctrl->SetFocus();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFocus"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, IsFloat)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsFloat() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsFloat"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, SetFloat)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			if (lua_isnone(l, 2))
				ctrl->SetFloat(true);
			else
				ctrl->SetFloat(0 != lua_toboolean(l, 2));

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetFloat"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, Invalidate)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			ctrl->Invalidate();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::Invalidate"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, IsUpdateNeeded)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			lua_pushboolean(l, ctrl->IsUpdateNeeded() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::IsUpdateNeeded"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, NeedUpdate)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			ctrl->NeedUpdate();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::NeedUpdate"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, NeedParentUpdate)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			ctrl->NeedParentUpdate();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::NeedParentUpdate"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, GetAdjustColor)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwCol = LuaStatic::ReadRGBAFromLua(l, 2);

			DWORD dwBkCor = ctrl->GetAdjustColor(dwCol);

			LuaStatic::WriteRGBA2Lua(l, dwBkCor);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetAdjustColor"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetAttribute)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));
			
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			CDuiString pstrText2;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText2);


			ctrl->SetAttribute(pstrText, pstrText2);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetAttribute"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCControlUI, ApplyAttributeList)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->ApplyAttributeList(pstrText);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::ApplyAttributeList"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, EstimateSize)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE szAva = LuaStatic::ReadSizeFromLua(l, 2);

			SIZE st = ctrl->EstimateSize(szAva);
			LuaStatic::WriteSize2Lua(l, st);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::EstimateSize"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCControlUI, GetVirtualWnd)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText = ctrl->GetVirtualWnd();

			lua_op_t<CDuiString>::push_stack(l, pstrText);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::GetVirtualWnd"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCControlUI, SetVirtualWnd)
	{
		try
		{
			CControlUI* ctrl;
			ctrl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 1));

			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			ctrl->SetVirtualWnd(pstrText.GetData());

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCControlUI::SetVirtualWnd"));
			return 0;
		}
	}

	void LuaCControlUI::Register(lua_State* l)
	{
		lua_newtable(l); // t,

		static LuaReg StaticFuncs[] =
		{
			{ "GetClassName", GetClassName },
			{ "New", New },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ControlUI");
		
		luaL_newmetatable(l, "CControlUI");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "IsClass", IsClass },
			{ "IsName", IsName },
			{ "GetName", GetName },
			{ "SetName", SetName },
			{ "Delete", Delete },
			{ "GetClass", GetClass },
			{ "GetInterface", GetInterface },
			{ "GetControlFlags", GetControlFlags },
			{ "Activate", Activate },
			{ "GetManager", GetManager },
			{ "SetManager", SetManager },
			{ "GetParent", GetParent },
			{ "SetCustomStyle", SetCustomStyle },
			{ "SetCustomStyle2", SetCustomStyle2 },
			{ "GetText", GetText },
			{ "SetText", SetText },
			{ "GetBkColor", GetBkColor },
			{ "SetBkColor", SetBkColor },
			{ "GetBkColor2", GetBkColor2 },
			{ "SetBkColor2", SetBkColor2 },
			{ "GetBkColor3", GetBkColor3 },
			{ "SetBkColor3", SetBkColor3 },
			{ "GetBkImage", GetBkImage },
			{ "SetBkImage", SetBkImage },
			{ "GetFocusBorderColor", GetFocusBorderColor },
			{ "SetFocusBorderColor", SetFocusBorderColor },
			{ "IsColorHSL", IsColorHSL },
			{ "SetColorHSL", SetColorHSL },
			{ "GetBorderRound", GetBorderRound },
			{ "SetBorderRound", SetBorderRound },
			//{ "DrawImage", DrawImage },
			{ "GetBorderSize", GetBorderSize },
			{ "SetBorderSize", SetBorderSize },
			{ "GetBorderColor", GetBorderColor },
			{ "SetBorderColor", SetBorderColor },
			{ "SetBorderRect", SetBorderRect },
			{ "GetLeftBorderSize", GetLeftBorderSize },
			{ "SetLeftBorderSize", SetLeftBorderSize },
			{ "GetTopBorderSize", GetTopBorderSize },
			{ "SetTopBorderSize", SetTopBorderSize },
			{ "GetRightBorderSize", GetRightBorderSize },
			{ "SetRightBorderSize", SetRightBorderSize },
			{ "GetBottomBorderSize", GetBottomBorderSize },
			{ "SetBottomBorderSize", SetBottomBorderSize },
			{ "GetBorderStyle", GetBorderStyle },
			{ "SetBorderStyle", SetBorderStyle },
			{ "GetPos", GetPos },
			{ "SetPos", SetPos },
			{ "GetWidth", GetWidth },
			{ "GetHeight", GetHeight },
			{ "GetX", GetX },
			{ "GetY", GetY },
			{ "GetPadding", GetPadding },
			{ "SetPadding", SetPadding },
			{ "GetFixedXY", GetFixedXY },
			{ "SetFixedXY", SetFixedXY },
			{ "GetFixedWidth", GetFixedWidth },
			{ "SetFixedWidth", SetFixedWidth },
			{ "GetFixedHeight", GetFixedHeight },
			{ "SetFixedHeight", SetFixedHeight },
			{ "GetMinWidth", GetMinWidth },
			{ "SetMinWidth", SetMinWidth },
			{ "GetMaxWidth", GetMaxWidth },
			{ "SetMaxWidth", SetMaxWidth },
			{ "GetMinHeight", GetMinHeight },
			{ "SetMinHeight", SetMinHeight },
			{ "GetMaxHeight", GetMaxHeight },
			{ "SetMaxHeight", SetMaxHeight },
			{ "SetRelativePos", SetRelativePos },
			{ "SetRelativeParentSize", SetRelativeParentSize },
			{ "GetRelativePos", GetRelativePos },
			{ "IsRelativePos", IsRelativePos },
			{ "GetSoundNameOver", GetSoundNameOver },
			{ "SetSoundNameOver", SetSoundNameOver },
			{ "GetSoundNameDown", GetSoundNameDown },
			{ "SetSoundNameDown", SetSoundNameDown },
			{ "GetStyleName", GetStyleName },
			{ "SetStyleName", SetStyleName },
			{ "GetToolTip", GetToolTip },
			{ "SetToolTip", SetToolTip },
			{ "SetToolTipWidth", SetToolTipWidth },
			{ "GetToolTipWidth", GetToolTipWidth },
			{ "GetShortcut", GetShortcut },
			{ "SetShortcut", SetShortcut },
			{ "IsContextMenuUsed", IsContextMenuUsed },
			{ "SetContextMenuUsed", SetContextMenuUsed },
			{ "GetUserData", GetUserData },
			{ "SetUserData", SetUserData },
			{ "GetTag", GetTag },
			{ "SetTag", SetTag },
			{ "GetTagCtrl", GetTagCtrl },
			{ "SetTagCtrl", SetTagCtrl },
			{ "IsVisible", IsVisible },
			{ "SetVisible", SetVisible },
			{ "SetInternVisible", SetInternVisible },
			{ "IsEnabled", IsEnabled },
			{ "SetEnabled", SetEnabled },
			{ "IsMouseEnabled", IsMouseEnabled },
			{ "SetMouseEnabled", SetMouseEnabled },
			{ "IsKeyboardEnabled", IsKeyboardEnabled },
			{ "SetKeyboardEnabled", SetKeyboardEnabled },
			{ "IsFocused", IsFocused },
			{ "SetFocus", SetFocus },
			{ "IsFloat", IsFloat },
			{ "SetFloat", SetFloat },
			{ "Invalidate", Invalidate },
			{ "IsUpdateNeeded", IsUpdateNeeded },
			{ "NeedUpdate", NeedUpdate },
			{ "NeedParentUpdate", NeedParentUpdate },
			{ "GetAdjustColor", GetAdjustColor },
			{ "SetAttribute", SetAttribute },
			{ "ApplyAttributeList", ApplyAttributeList },
			{ "EstimateSize", EstimateSize },
			{ "SetVirtualWnd", SetVirtualWnd },
			{ "GetVirtualWnd", GetVirtualWnd },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));

		lua_pop(l, 1);
	}
}
