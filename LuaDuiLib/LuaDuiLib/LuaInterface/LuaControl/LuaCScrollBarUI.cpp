#include "StdAfx.h"

#include "LuaCScrollBarUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCScrollBarUI, New)
	{
		CScrollBarUI  *self = new CScrollBarUI();
		LuaStatic::AddObject2Lua(l, self, METATABLE_NAME(self));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCScrollBarUI, GetClassName)
	{
		CDuiString pstrText = CScrollBarUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetOwner)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CContainerUI* pOwner;
			pOwner = static_cast<CContainerUI*>(self->GetOwner());

			if (pOwner)
				LuaStatic::AddObject2Lua(l,pOwner, METATABLE_NAME(pOwner));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetOwner)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));

			CContainerUI* pOwner;
			pOwner = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 2));

			self->SetOwner(pOwner);
			
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetOwner"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, IsHorizontal)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->IsHorizontal() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::IsHorizontal"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetHorizontal)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			bool bHorizontal = true;
			if(!lua_isnone(l,2))
				bHorizontal = lua_toboolean(l,2) != 0;
			self->SetHorizontal(bHorizontal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetHorizontal"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetScrollRange)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetScrollRange());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetScrollRange"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetScrollRange)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));			
			self->SetScrollRange(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetScrollRange"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetScrollPos)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetScrollPos());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetScrollPos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetScrollPos)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));			
			self->SetScrollPos(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetScrollPos"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetLineSize)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->GetLineSize());
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetLineSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetLineSize)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));			
			self->SetLineSize(lua_tointeger(l,2));
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetLineSize"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCScrollBarUI, GetShowButton1)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetShowButton1() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetShowButton1"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetShowButton1)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			bool bShow = true;
			if(!lua_isnone(l,2))
				bShow = lua_toboolean(l,2) != 0;
			self->SetShowButton1(bShow);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetShowButton1"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton1NormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton1NormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton1NormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton1NormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton1NormalImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton1NormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton1HotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton1HotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton1HotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton1HotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton1HotImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton1HotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton1PushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton1PushedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton1PushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton1PushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton1PushedImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton1PushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton1DisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton1DisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton1DisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton1DisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton1DisabledImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton1DisabledImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCScrollBarUI, GetShowButton2)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->GetShowButton2() ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetShowButton2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetShowButton2)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			bool bShow = true;
			if(!lua_isnone(l,2))
				bShow = lua_toboolean(l,2) != 0;
			self->SetShowButton2(bShow);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetShowButton2"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton2NormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton2NormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton2NormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton2NormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton2NormalImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton2NormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton2HotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton2HotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton2HotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton2HotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton2HotImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton2HotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton2PushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton2PushedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton2PushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton2PushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton2PushedImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton2PushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetButton2DisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetButton2DisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetButton2DisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetButton2DisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetButton2DisabledImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetButton2DisabledImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCScrollBarUI, GetThumbNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetThumbNormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetThumbNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetThumbNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetThumbNormalImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetThumbNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetThumbHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetThumbHotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetThumbHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetThumbHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetThumbHotImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetThumbHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetThumbPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetThumbPushedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetThumbPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetThumbPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetThumbPushedImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetThumbPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetThumbDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetThumbDisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetThumbDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetThumbDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetThumbDisabledImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetThumbDisabledImage"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCScrollBarUI, GetRailNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetRailNormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetRailNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetRailNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetRailNormalImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetRailNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetRailHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetRailHotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetRailHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetRailHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetRailHotImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetRailHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetRailPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetRailPushedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetRailPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetRailPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetRailPushedImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetRailPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetRailDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetRailDisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetRailDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetRailDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetRailDisabledImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetRailDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetBkNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetBkNormalImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetBkNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetBkNormalImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetBkNormalImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetBkNormalImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetBkHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetBkHotImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetBkHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetBkHotImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetBkHotImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetBkHotImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetBkPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetBkPushedImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetBkPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetBkPushedImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetBkPushedImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetBkPushedImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, GetBkDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->GetBkDisabledImage();
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::GetBkDisabledImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCScrollBarUI, SetBkDisabledImage)
	{
		try
		{
			CScrollBarUI* self;
			self = static_cast<CScrollBarUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			self->SetBkDisabledImage(pstrText.GetData());
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCScrollBarUI::SetBkDisabledImage"));
			return 0;
		}
	}

	void LuaCScrollBarUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ScrollBarUI");

		luaL_newmetatable(l, "CScrollBarUI");
		luaL_getmetatable(l, "CControlUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetOwner", GetOwner },
			{ "SetOwner", SetOwner },
			{ "IsHorizontal", IsHorizontal },
			{ "SetHorizontal", SetHorizontal },
			{ "GetScrollRange", GetScrollRange },
			{ "SetScrollRange", SetScrollRange },
			{ "GetScrollPos", GetScrollPos },
			{ "SetScrollPos", SetScrollPos },
			{ "GetLineSize", GetLineSize },
			{ "SetLineSize", SetLineSize },

			{ "GetShowButton1", GetShowButton1 },
			{ "SetShowButton1", SetShowButton1 },
			{ "GetButton1NormalImage", GetButton1NormalImage },
			{ "SetButton1NormalImage", SetButton1NormalImage },
			{ "GetButton1HotImage", GetButton1HotImage },
			{ "SetButton1HotImage", SetButton1HotImage },
			{ "GetButton1PushedImage", GetButton1PushedImage },
			{ "SetButton1PushedImage", SetButton1PushedImage },
			{ "GetButton1DisabledImage", GetButton1DisabledImage },
			{ "SetButton1DisabledImage", SetButton1DisabledImage },

			{ "GetShowButton2", GetShowButton2 },
			{ "SetShowButton2", SetShowButton2 },
			{ "GetButton2NormalImage", GetButton2NormalImage },
			{ "SetButton2NormalImage", SetButton2NormalImage },
			{ "GetButton2HotImage", GetButton2HotImage },
			{ "SetButton2HotImage", SetButton2HotImage },
			{ "GetButton2PushedImage", GetButton2PushedImage },
			{ "SetButton2PushedImage", SetButton2PushedImage },
			{ "GetButton2DisabledImage", GetButton2DisabledImage },
			{ "SetButton2DisabledImage", SetButton2DisabledImage },

			{ "GetThumbNormalImage", GetThumbNormalImage },
			{ "SetThumbNormalImage", SetThumbNormalImage },
			{ "GetThumbHotImage", GetThumbHotImage },
			{ "SetThumbHotImage", SetThumbHotImage },
			{ "GetThumbPushedImage", GetThumbPushedImage },
			{ "SetThumbPushedImage", SetThumbPushedImage },
			{ "GetThumbDisabledImage", GetThumbDisabledImage },
			{ "SetThumbDisabledImage", SetThumbDisabledImage },

			{ "GetRailNormalImage", GetRailNormalImage },
			{ "SetRailNormalImage", SetRailNormalImage },
			{ "GetRailHotImage", GetRailHotImage },
			{ "SetRailHotImage", SetRailHotImage },
			{ "GetRailPushedImage", GetRailPushedImage },
			{ "SetRailPushedImage", SetRailPushedImage },
			{ "GetRailDisabledImage", GetRailDisabledImage },
			{ "SetRailDisabledImage", SetRailDisabledImage },

			{ "GetBkNormalImage", GetBkNormalImage },
			{ "SetBkNormalImage", SetBkNormalImage },
			{ "GetBkHotImage", GetBkHotImage },
			{ "SetBkHotImage", SetBkHotImage },
			{ "GetBkPushedImage", GetBkPushedImage },
			{ "SetBkPushedImage", SetBkPushedImage },
			{ "GetBkDisabledImage", GetBkDisabledImage },
			{ "SetBkDisabledImage", SetBkDisabledImage },
		};

		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}