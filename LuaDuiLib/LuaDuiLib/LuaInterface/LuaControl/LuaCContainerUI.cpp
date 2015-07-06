#include "StdAfx.h"

#include "LuaCContainerUI.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCContainerUI, New)
	{
		CContainerUI  *ctrl = new CContainerUI();
		LuaStatic::AddObject2Lua(l,ctrl,METATABLE_NAME(ctrl));
		return 1;
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetClassName)
	{
		CDuiString pstrText = CContainerUI::GetClassName();
		lua_op_t<CDuiString>::push_stack(l, pstrText);
		return 1;
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetItemAt)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = (int)lua_tointeger(l, 2);

			CControlUI* pControl = self->GetItemAt(iIndex);
			if (pControl)
			{
				LuaStatic::AddObject2Lua(l, pControl,METATABLE_NAME(pControl));
			}
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetItemAt"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetItemIndex)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl;
			pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			int iIndex = self->GetItemIndex(pControl);
			lua_pushinteger(l, (lua_Integer)iIndex);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetItemIndex"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetItemIndex)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl;
			pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			int iIndex = (int)lua_tointeger(l, 3);
			lua_op_t<bool>::push_stack(l,self->SetItemIndex(pControl, iIndex));
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetItemIndex"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetCount)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			
			int iCount = self->GetCount();
			lua_pushinteger(l, (lua_Integer)iCount);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetCount"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,Add)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl;
			pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			lua_op_t<bool>::push_stack(l,self->Add(pControl));
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::Add"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,AddAt)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl;
			pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			int iIndex = (int)lua_tointeger(l, 3);
			lua_op_t<bool>::push_stack(l,self->AddAt(pControl, iIndex));
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::AddAt"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,Remove)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl;
			pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			bool bRet = self->Remove(pControl);
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::Remove"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,RemoveAt)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = (int)lua_tointeger(l, 2);
			bool bRet = self->RemoveAt(iIndex);
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::RemoveAt"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,RemoveAll)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			self->RemoveAll();
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::RemoveAll"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetInset)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			RECT rt = self->GetInset();

			LuaStatic::WriteRect2Lua(l, rt);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetInset"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetInset)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			RECT rt = LuaStatic::ReadRectFromLua(l, 2);

			self->SetInset(rt);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetInset"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetChildPadding)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			int iPadding = self->GetChildPadding();
			lua_pushinteger(l,iPadding);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetChildPadding"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetChildPadding)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			int iPadding = lua_tointeger(l,2);
			self->SetChildPadding(iPadding);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetChildPadding"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,IsAutoDestroy)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bRet = self->IsAutoDestroy();
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::IsAutoDestroy"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetAutoDestroy)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bAuto = lua_toboolean(l, 2) != 0;
			self->SetAutoDestroy(bAuto);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetAutoDestroy"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,IsDelayedDestroy)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bRet = self->IsDelayedDestroy();
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::IsDelayedDestroy"));
			return 0;
		}
	}	

	IMPL_LUA_FUNC(LuaCContainerUI,SetDelayedDestroy)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bAuto = lua_toboolean(l, 2) != 0;
			self->SetDelayedDestroy(bAuto);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetDelayedDestroy"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,IsMouseChildEnabled)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bRet = self->IsMouseChildEnabled();
			lua_pushboolean(l, bRet ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::IsMouseChildEnabled"));
			return 0;
		}
	}	

	IMPL_LUA_FUNC(LuaCContainerUI,SetMouseChildEnabled)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bAuto = lua_toboolean(l, 2) != 0;
			self->SetMouseChildEnabled(bAuto);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetMouseChildEnabled"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,FindSelectable)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			int iIndex = (int)lua_tointeger(l, 2);
			bool bForward = true;
			if (!lua_isnone(l, 3))
				bForward = lua_toboolean(l, 3) != 0;
			int iRet = self->FindSelectable(iIndex,bForward);
			lua_pushinteger(l,iRet);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::FindSelectable"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetSubControlText)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrSubControlName;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrSubControlName);
			
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);

			bool bRet = self->SetSubControlText(pstrSubControlName.GetData(),pstrText.GetData());
			lua_pushboolean(l,bRet ? 1 : 0);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetSubControlText"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetSubControlFixedHeight)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			int cy = lua_tointeger(l,3);

			bool bRet = self->SetSubControlFixedHeight(pstrText.GetData(), cy);
			lua_pushboolean(l,bRet ? 1 : 0);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetSubControlFixedHeight"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetSubControlFixedWdith)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			int cx = lua_tointeger(l,3);

			bool bRet = self->SetSubControlFixedWdith(pstrText.GetData(), cx);
			lua_pushboolean(l,bRet ? 1 : 0);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetSubControlFixedWdith"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,SetSubControlUserData)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
		
			CDuiString pstrSubControlName;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrSubControlName);
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);

			bool bRet = self->SetSubControlUserData(pstrSubControlName.GetData(),pstrText.GetData());
			lua_pushboolean(l,bRet ? 1 : 0);

			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetSubControlUserData"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetSubControlText)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);

			CDuiString pstrText2 = self->GetSubControlText(pstrText.GetData());

			lua_op_t<CDuiString>::push_stack(l, pstrText2);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetSubControlText"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetSubControlFixedHeight)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			int cy = self->GetSubControlFixedHeight(pstrText.GetData());

			lua_pushinteger(l,cy);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetSubControlFixedHeight"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetSubControlFixedWdith)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			int cx = self->GetSubControlFixedWdith(pstrText.GetData());

			lua_pushinteger(l,cx);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetSubControlFixedWdith"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetSubControlUserData)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			
			CDuiString pstrText2 = self->GetSubControlUserData(pstrText.GetData());

			lua_op_t<CDuiString>::push_stack(l, pstrText2);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetSubControlUserData"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, FindSubControl)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			
			CControlUI* pControl = self->FindSubControl(pstrText.GetData());

			if (pControl)
			{
				LuaStatic::AddObject2Lua(l, pControl,METATABLE_NAME(pControl));
			}
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::FindSubControl"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetScrollPos)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE sz = self->GetScrollPos();

			LuaStatic::WriteSize2Lua(l, sz);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetScrollPos"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, GetScrollRange)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE sz = self->GetScrollRange();

			LuaStatic::WriteSize2Lua(l, sz);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetScrollRange"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, SetScrollPos)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			SIZE sz = LuaStatic::ReadSizeFromLua(l, 2);

			self->SetScrollPos(sz);

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::SetScrollPos"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, LineUp)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->LineUp();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::LineUp"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, LineDown)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->LineDown();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::LineDown"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, PageUp)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->PageUp();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::PageUp"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, PageDown)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->PageDown();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::PageDown"));
			return 0;
		}
	}	

	IMPL_LUA_FUNC(LuaCContainerUI, HomeUp)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->HomeUp();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::HomeUp"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, EndDown)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->EndDown();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::EndDown"));
			return 0;
		}
	}
	
	IMPL_LUA_FUNC(LuaCContainerUI, LineLeft)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->LineLeft();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::LineLeft"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, LineRight)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->LineRight();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::LineRight"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, PageLeft)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->PageLeft();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::PageLeft"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, PageRight)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->PageRight();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::PageRight"));
			return 0;
		}
	}		

	IMPL_LUA_FUNC(LuaCContainerUI, HomeLeft)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->HomeLeft();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::HomeLeft"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI, EndRight)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));

			self->EndRight();

			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::EndRight"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,EnableScrollBar)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			bool bEnableVertical = true;
			bool bEnableHorizontal = true;
			if (!lua_isnone(l, 2))
				bEnableVertical = lua_toboolean(l, 2) != 0;
			if (!lua_isnone(l, 3))
				bEnableHorizontal = lua_toboolean(l, 3) != 0;

			self->EnableScrollBar(bEnableVertical,bEnableHorizontal);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::EnableScrollBar"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetVerticalScrollBar)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CScrollBarUI* pBar = self->GetVerticalScrollBar();
			if (pBar)
			{
				LuaStatic::AddObject2Lua(l, pBar,METATABLE_NAME(pBar));
			}
			else
				lua_pushnil(l);
				
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetVerticalScrollBar"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaCContainerUI,GetHorizontalScrollBar)
	{
		try
		{
			CContainerUI* self;
			self = static_cast<CContainerUI*>(LuaStatic::CheckUserData(l, 1));
			CScrollBarUI* pBar = self->GetHorizontalScrollBar();
			if (pBar)
			{
				LuaStatic::AddObject2Lua(l, pBar,METATABLE_NAME(pBar));
			}
			else
				lua_pushnil(l);
				
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCContainerUI::GetHorizontalScrollBar"));
			return 0;
		}
	}

	void LuaCContainerUI::Register(lua_State* l)
	{
		lua_newtable(l);
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },
			{ "GetClassName", GetClassName },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ContainerUI");

		luaL_newmetatable(l, "CContainerUI");
		luaL_getmetatable(l, "CControlUI");
		lua_setmetatable(l, -2);
		
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");
		static LuaReg MethodFuncs[] =
		{
			{ "GetItemAt", GetItemAt },
			{ "GetItemIndex", GetItemIndex },
			{ "SetItemIndex", SetItemIndex },
			{ "GetCount", GetCount },
			{ "Add", Add },
			{ "AddAt", AddAt },
			{ "Remove", Remove },
			{ "RemoveAt", RemoveAt },
			{ "RemoveAll", RemoveAll },
			{ "GetInset", GetInset },
			{ "SetInset", SetInset },
			{ "GetChildPadding", GetChildPadding },
			{ "SetChildPadding", SetChildPadding },
			{ "IsAutoDestroy", IsAutoDestroy },
			{ "SetAutoDestroy", SetAutoDestroy },
			{ "IsDelayedDestroy", IsDelayedDestroy },
			{ "SetDelayedDestroy", SetDelayedDestroy },
			{ "IsMouseChildEnabled", IsMouseChildEnabled },
			{ "SetMouseChildEnabled", SetMouseChildEnabled },
			{ "FindSelectable", FindSelectable },
			{ "SetSubControlText", SetSubControlText },
			{ "SetSubControlFixedHeight", SetSubControlFixedHeight },
			{ "SetSubControlFixedWdith", SetSubControlFixedWdith },
			{ "SetSubControlUserData", SetSubControlUserData },
			{ "GetSubControlText", GetSubControlText },
			{ "GetSubControlFixedHeight", GetSubControlFixedHeight },
			{ "GetSubControlFixedWdith", GetSubControlFixedWdith },
			{ "GetSubControlUserData", GetSubControlUserData },
			{ "FindSubControl", FindSubControl },
			{ "GetScrollPos", GetScrollPos },
			{ "GetScrollRange", GetScrollRange },
			{ "SetScrollPos", SetScrollPos },
			{ "LineUp", LineUp },
			{ "LineDown", LineDown },
			{ "PageUp", PageUp },
			{ "PageDown", PageDown },
			{ "HomeUp", HomeUp },
			{ "EndDown", EndDown },
			{ "LineLeft", LineLeft },
			{ "LineRight", LineRight },
			{ "PageLeft", PageLeft },
			{ "PageRight", PageRight },
			{ "HomeLeft", HomeLeft },
			{ "EndRight", EndRight },
			{ "EnableScrollBar", EnableScrollBar },
			{ "GetVerticalScrollBar", GetVerticalScrollBar },
			{ "GetHorizontalScrollBar", GetHorizontalScrollBar },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		
		lua_pop(l, 1);
	}
}
