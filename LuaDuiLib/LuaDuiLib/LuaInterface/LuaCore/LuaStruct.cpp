#include "StdAfx.h"

#include "LuaStruct.h"
#include "../LuaCWindow.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaTNotifyUI, isnil)
	{
		TNotifyUI* self;
		self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, _gc)
	{
		TNotifyUI* self;
		self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		//SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, _eq)
	{
		try
		{
			TNotifyUI* left;
			left = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));

			TNotifyUI* right;
			right = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, _tostring)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "TNotifyUI", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "TNotifyUI", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, Delete)
	{
		TNotifyUI* self;
		self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, New)
	{
		TNotifyUI* self = new TNotifyUI();
		LuaStatic::AddObject2Lua(l, self, "TNotifyUI");
		return 1;
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getType)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->sType;
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getVirtualWnd)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->sVirtualWnd;
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getVirtualWnd"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getSender)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pSender = static_cast<CControlUI*>(self->pSender);
			if (pSender)
				LuaStatic::AddObject2Lua(l, pSender, METATABLE_NAME(pSender));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getSender"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getTimestamp)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->dwTimestamp);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getTimestamp"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getPtMouse)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WritePoint2Lua(l,self->ptMouse);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getPtMouse"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getWParam)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->wParam);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getWParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, getLParam)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->lParam);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getLParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setType)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->sType = pstrText;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::getType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setVirtualWnd)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->sVirtualWnd = pstrText;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setVirtualWnd"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setSender)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pSender = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			self->pSender = pSender;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setSender"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setTimestamp)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			DWORD dwTimestamp = (DWORD)lua_tonumber(l,2);
			self->dwTimestamp = dwTimestamp;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setTimestamp"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setPtMouse)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			POINT ptMouse = LuaStatic::ReadPointFromLua(l,2);
			self->ptMouse = ptMouse;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setPtMouse"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setWParam)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			WPARAM wParam = (WPARAM) luaL_optint(l, 2, 0);
			self->wParam = wParam;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setWParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTNotifyUI, setLParam)
	{
		try
		{
			TNotifyUI* self;
			self = static_cast<TNotifyUI*>(LuaStatic::CheckUserData(l, 1));
			LPARAM lParam = (LPARAM) luaL_optstring(l, 2, "");
			self->lParam = lParam;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTNotifyUI::setLParam"));
			return 0;
		}
	}
	void LuaTNotifyUI::Register(lua_State* l)
	{
		lua_newtable(l); // t,
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },			
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "NotifyUI");
		
		luaL_newmetatable(l, "TNotifyUI");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "getType", getType },
			{ "getVirtualWnd", getVirtualWnd },
			{ "getSender", getSender },
			{ "getTimestamp", getTimestamp },
			{ "getPtMouse", getPtMouse },
			{ "getWParam", getWParam },
			{ "getLParam", getLParam },
			{ "setType", setType },
			{ "setVirtualWnd", setVirtualWnd },
			{ "setSender", setSender },
			{ "setTimestamp", setTimestamp },
			{ "setPtMouse", setPtMouse },
			{ "setWParam", setWParam },
			{ "setLParam", setLParam },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaTFontInfo, isnil)
	{
		TFontInfo* self;
		self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaTFontInfo, _gc)
	{
		TFontInfo* self;
		self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		//SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTFontInfo, _eq)
	{
		try
		{
			TFontInfo* left;
			left = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));

			TFontInfo* right;
			right = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, _tostring)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "TFontInfo", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "TFontInfo", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, Delete)
	{
		TFontInfo* self;
		self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTFontInfo, New)
	{
		TFontInfo* self = new TFontInfo();
		LuaStatic::AddObject2Lua(l, self, "TFontInfo");
		return 1;
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getFont)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::AddObject2Lua(l,self->hFont,"HFONT");
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getFont"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getFontName)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->sFontName;
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getFontName"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getSize)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->iSize);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getBold)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->bBold ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getBold"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getUnderline)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->bUnderline ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getUnderline"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getItalic)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->bItalic);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getItalic"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, getTextmetric)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::AddObject2Lua(l, &(self->tm), "TEXTMETRIC");
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::getTextmetric"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setFont)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			HFONT hFont = static_cast<HFONT>(LuaStatic::CheckUserData(l, 2));
			self->hFont = hFont;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setFont"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setFontName)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->sFontName = pstrText;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setFontName"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setSize)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			int iSize = lua_tointeger(l,2);
			self->iSize = iSize;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setSize"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setBold)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			self->bBold = lua_toboolean(l,2) != 0;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setBold"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setUnderline)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			self->bUnderline = lua_toboolean(l,2) != 0;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setUnderline"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setItalic)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			self->bItalic = lua_toboolean(l,2) != 0;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setItalic"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTFontInfo, setTextmetric)
	{
		try
		{
			TFontInfo* self;
			self = static_cast<TFontInfo*>(LuaStatic::CheckUserData(l, 1));
			TEXTMETRIC* tm = static_cast<TEXTMETRIC*>(LuaStatic::CheckUserData(l, 2));
			self->tm = *tm;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTFontInfo::setTextmetric"));
			return 0;
		}
	}
	void LuaTFontInfo::Register(lua_State* l)
	{
		lua_newtable(l); // t,
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },			
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "FontInfo");
		
		luaL_newmetatable(l, "TFontInfo");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "getFont", getFont },
			{ "getFontName", getFontName },
			{ "getSize", getSize },
			{ "getBold", getBold },
			{ "getUnderline", getUnderline },
			{ "getItalic", getItalic },
			{ "getTextmetric", getTextmetric },
			{ "setFont", setFont },
			{ "setFontName", setFontName },
			{ "setSize", setSize },
			{ "setBold", setBold },
			{ "setUnderline", setUnderline },
			{ "setItalic", setItalic },
			{ "setTextmetric", setTextmetric },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaTImageInfo, isnil)
	{
		TImageInfo* self;
		self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaTImageInfo, _gc)
	{
		TImageInfo* self;
		self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		//SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTImageInfo, _eq)
	{
		try
		{
			TImageInfo* left;
			left = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));

			TImageInfo* right;
			right = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, _tostring)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "TImageInfo", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "TImageInfo", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, Delete)
	{
		TImageInfo* self;
		self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		//SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTImageInfo, New)
	{
		TImageInfo* self = new TImageInfo();
		LuaStatic::AddObject2Lua(l, self, "TImageInfo");
		return 1;
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getBitmap)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::AddObject2Lua(l,self->hBitmap,"HBITMAP");
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getBitmap"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getX)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->nX);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getX"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getY)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->nY);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getY"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getAlphaChannel)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushboolean(l,self->alphaChannel ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getAlphaChannel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getResType)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText = self->sResType;
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getResType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, getMask)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->dwMask);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::getMask"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setBitmap)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			HBITMAP hBitmap = static_cast<HBITMAP>(LuaStatic::CheckUserData(l, 2));
			self->hBitmap = hBitmap;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setBitmap"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setX)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			self->nX = lua_tointeger(l,2);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setX"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setY)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			self->nY = lua_tointeger(l,2);;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setY"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setAlphaChannel)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			self->alphaChannel = lua_toboolean(l,2) != 0;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setAlphaChannel"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setResType)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			self->sResType = pstrText;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setResType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTImageInfo, setMask)
	{
		try
		{
			TImageInfo* self;
			self = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			self->dwMask = (DWORD)lua_tonumber(l,2);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTImageInfo::setMask"));
			return 0;
		}
	}
	void LuaTImageInfo::Register(lua_State* l)
	{
		lua_newtable(l); // t,
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },			
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "ImageInfo");
		
		luaL_newmetatable(l, "TImageInfo");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "getBitmap", getBitmap },
			{ "getX", getX },
			{ "getY", getY },
			{ "getAlphaChannel", getAlphaChannel },
			{ "getResType", getResType },
			{ "getMask", getMask },
			{ "setBitmap", setBitmap },
			{ "setX", setX },
			{ "setY", setY },
			{ "setAlphaChannel", setAlphaChannel },
			{ "setResType", setResType },
			{ "setMask", setMask },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaTEventUI, isnil)
	{
		TEventUI* self;
		self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaTEventUI, _gc)
	{
		TEventUI* self;
		self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		//SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTEventUI, _eq)
	{
		try
		{
			TEventUI* left;
			left = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));

			TEventUI* right;
			right = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, _tostring)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "TEventUI", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "TEventUI", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, Delete)
	{
		TEventUI* self;
		self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaTEventUI, New)
	{
		TEventUI* self = new TEventUI();
		LuaStatic::AddObject2Lua(l, self, "TEventUI");
		return 1;
	}
	IMPL_LUA_FUNC(LuaTEventUI, getType)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->Type);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getSender)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			if(self->pSender)
				LuaStatic::AddObject2Lua(l,self->pSender,METATABLE_NAME(self->pSender));
			else
				lua_pushnil(l);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getSender"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getTimestamp)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->dwTimestamp);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getTimestamp"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getPtMouse)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			LuaStatic::WritePoint2Lua(l,self->ptMouse);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getPtMouse"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getChKey)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushinteger(l,self->chKey);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getChKey"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getKeyState)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->wKeyState);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getKeyState"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getWParam)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->wParam);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getWParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, getLParam)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			lua_pushnumber(l,self->lParam);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::getLParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setType)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			self->Type = lua_tointeger(l,2);;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setType"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setSender)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l, 2));
			self->pSender = pControl;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setSender"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setTimestamp)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			self->dwTimestamp = (DWORD)lua_tonumber(l,2);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setTimestamp"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setPtMouse)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			POINT pt = LuaStatic::ReadPointFromLua(l,2);
			self->ptMouse = pt;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setPtMouse"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setChKey)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			TCHAR key = (TCHAR)(lua_tointeger(l,2));
			self->chKey = key;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setChKey"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setKeyState)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			WORD s = (WORD)lua_tonumber(l,2);
			self->wKeyState = s;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setKeyState"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setWParam)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			WPARAM wParam = (WPARAM) luaL_optint(l, 2, 0);
			self->wParam = wParam;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setWParam"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaTEventUI, setLParam)
	{
		try
		{
			TEventUI* self;
			self = static_cast<TEventUI*>(LuaStatic::CheckUserData(l, 1));
			LPARAM lParam = (LPARAM) luaL_optstring(l, 2, "");
			self->lParam = lParam;
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaTEventUI::setLParam"));
			return 0;
		}
	}
	void LuaTEventUI::Register(lua_State* l)
	{
		lua_newtable(l); // t,
		static LuaReg StaticFuncs[] =
		{
			{ "New", New },			
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "EventUI");
		
		luaL_newmetatable(l, "TEventUI");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "getType", getType },
			{ "getSender", getSender },
			{ "getTimestamp", getTimestamp },
			{ "getPtMouse", getPtMouse },
			{ "getChKey", getChKey },
			{ "getKeyState", getKeyState },
			{ "getWParam", getWParam },
			{ "getLParam", getLParam },
			{ "setType", setType },
			{ "setSender", setSender },
			{ "setTimestamp", setTimestamp },
			{ "setPtMouse", setPtMouse },
			{ "setChKey", setChKey },
			{ "setKeyState", setKeyState },
			{ "setWParam", setWParam },
			{ "setLParam", setLParam },
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};

namespace DuiLib
{
	void LuaMsgMarco::Register(lua_State* l)
	{
		lua_newtable(l);

		static LuaArgs StaticArgs[] =
		{
			{ "WM_USER", WM_USER},
			{ "WM_CREATE", WM_CREATE },
			{ "WM_CLOSE", WM_CLOSE },
			{ "WM_DESTROY", WM_DESTROY },
			{ "WM_QUIT",WM_QUIT },
#if defined(WIN32) && !defined(UNDER_CE)
			{ "WM_NCCREATE", WM_NCCREATE },
			{ "WM_NCMOUSEMOVE", WM_NCMOUSEMOVE },
			{ "WM_NCMOUSELEAVE", WM_NCMOUSELEAVE },
			{ "WM_NCACTIVATE", WM_NCACTIVATE },
			{ "WM_NCCALCSIZE", WM_NCCALCSIZE },
			{ "WM_NCPAINT", WM_NCPAINT },
			{ "WM_NCHITTEST", WM_NCHITTEST },
			{ "WM_GETMINMAXINFO", WM_GETMINMAXINFO },
			{ "WM_MOUSEWHEEL", WM_MOUSEWHEEL },
#endif
			{ "WM_TIMER",WM_TIMER },
			{ "WM_SYSCOMMAND",WM_SYSCOMMAND },
			{ "WM_MENUCOMMAND",WM_MENUCOMMAND },
		};
		LuaStatic::LuaSetArgsInTable(l, StaticArgs, sizeof(StaticArgs) / sizeof(StaticArgs[0]));

		lua_setfield(l, -2, "MsgArgs");
	}
}

namespace DuiLib
{
	void LuaEventMarco::Register(lua_State* l)
	{
		lua_newtable(l);

		static LuaArgs StaticArgs[] =
		{
			{ "KEYDOWN", UIEVENT_KEYDOWN },
			{ "KEYUP", UIEVENT_KEYUP },
			{ "CHAR", UIEVENT_CHAR },
			{ "SYSKEY", UIEVENT_SYSKEY },
			{ "MOUSEMOVE", UIEVENT_MOUSEMOVE },
			{ "MOUSELEAVE", UIEVENT_MOUSELEAVE },
			{ "MOUSEENTER", UIEVENT_MOUSEENTER },
			{ "MOUSEHOVER", UIEVENT_MOUSEHOVER },
			{ "BUTTONDOWN", UIEVENT_BUTTONDOWN },
			{ "BUTTONUP", UIEVENT_BUTTONUP },
			{ "RBUTTONDOWN", UIEVENT_RBUTTONDOWN },
			{ "DBLCLICK", UIEVENT_DBLCLICK },
			{ "CONTEXTMENU", UIEVENT_CONTEXTMENU },
			{ "SCROLLWHEEL", UIEVENT_SCROLLWHEEL },
			{ "KILLFOCUS", UIEVENT_KILLFOCUS },
			{ "SETFOCUS", UIEVENT_SETFOCUS },
			{ "WINDOWSIZE", UIEVENT_WINDOWSIZE },
			{ "SETCURSOR", UIEVENT_SETCURSOR },
			{ "TIMER", UIEVENT_TIMER },
			{ "NOTIFY", UIEVENT_NOTIFY },
			{ "COMMAND", UIEVENT_COMMAND },
		};
		LuaStatic::LuaSetArgsInTable(l, StaticArgs, sizeof(StaticArgs) / sizeof(StaticArgs[0]));

		lua_setfield(l, -2, "EventArgs");
	}
}

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaDuiLib, MsgBox)
	{
		try
		{
			HWND hWnd = NULL;
			if (lua_isuserdata(l, 1))
			{
				CLuaWindow* pWind = static_cast<CLuaWindow*>(LuaStatic::CheckUserData(l, 1));
				if (pWind)
					hWnd = pWind->GetHWND();
			}
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
			CDuiString lpCaption;
			if (lua_isstring(l, 3))
				lua_op_t<CDuiString>::lua_to_value(l, 3, lpCaption);
			UINT nType = MBBT_OKCANCLE;
			if (lua_isnumber(l, 4))
				nType = (UINT)lua_tonumber(l, 4);
			int lua_callback = -1;
			if (lua_isfunction(l, 5))
			{
				lua_callback = luaL_ref(l, LUA_REGISTRYINDEX);
			}
			if (CPaintManagerUI::ExistsSkinFile("msgbox.xml"))
			{
				UINT nRet =DuiLib::MsgBox(hWnd,pstrText, lpCaption, nType);
				if (lua_callback != -1)
				{
					lua_rawgeti(l, LUA_REGISTRYINDEX, lua_callback);
					lua_pushnumber(l, nRet);
					lua_call(l, 1, 0);
				}
			}
			else
			{
				int Type = MB_OK;
				if ((nType & MBT_OK) > 0)
					Type |= MB_OK;
				if ((nType & MBT_INFO) > 0)
					Type |= MB_ICONINFORMATION;
				if ((nType & MBT_WARN) > 0)
					Type |= MB_ICONWARNING;
				if ((nType & MBBT_CANCEL) > 0)
					Type |= MB_OKCANCEL &(~MB_OK);
				if ((nType & MBBT_OK) > 0)
					Type |= MB_OK;
				if ((nType & MBBT_OKCANCLE) > 0)
					Type |= MB_OKCANCEL;

				UINT nRet = ::MessageBox(hWnd, pstrText, lpCaption, Type);
				if (lua_callback != -1)
				{
					lua_rawgeti(l, LUA_REGISTRYINDEX, lua_callback);
					lua_pushnumber(l, nRet);
					lua_call(l, 1, 0);
					lua_pop(l, 1);
				}
			}
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::MsgBox"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaDuiLib, TraceMsg)
	{
		try
		{
			UINT uMsg = (UINT)lua_tonumber(l, 1);
			LPCTSTR sMsg = DUI__TraceMsg(uMsg);
			CDuiString pstrText(sMsg);
			lua_op_t<CDuiString>::push_stack(l, pstrText);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::TraceMsg"));
			return 0;
		}
	}

	IMPL_LUA_FUNC(LuaDuiLib, QuitApp)
	{
		try
		{
			::PostQuitMessage(0L);
			return 0;
		}
		catch (...)
		{
			DuiException(_T("LuaWindowExport::QuitApp"));
			return 0;
		}
	}

	void LuaDuiLib::Register(lua_State* l)
	{
	//	lua_newtable(l);

		static LuaArgs StaticArgs[] =
		{
			{ "WNDSTYLE_CONTAINER", UI_WNDSTYLE_CONTAINER },
			{ "WNDSTYLE_FRAME", UI_WNDSTYLE_FRAME },
			{ "VISIBLE", WS_VISIBLE },
			{ "OVERLAPPEDWINDOW", WS_OVERLAPPEDWINDOW },
			{ "EX_OVERLAPPEDWINDOW", WS_EX_OVERLAPPEDWINDOW },
			{ "WNDSTYLE_CHILD", UI_WNDSTYLE_CHILD },
			{ "CHILD", WS_CHILD },
			{ "CLIPSIBLINGS", WS_CLIPSIBLINGS },
			{ "CLIPCHILDREN", WS_CLIPCHILDREN },
			{ "WNDSTYLE_DIALOG", UI_WNDSTYLE_DIALOG },
			{ "POPUPWINDOW", WS_POPUPWINDOW },
			{ "CAPTION", WS_CAPTION },
			{ "DLGFRAME", WS_DLGFRAME },
			{ "WNDSTYLE_EX_FRAME", UI_WNDSTYLE_EX_FRAME },
			{ "EX_WINDOWEDGE", WS_EX_WINDOWEDGE },
			{ "WNDSTYLE_EX_DIALOG", UI_WNDSTYLE_EX_DIALOG },
			{ "EX_TOOLWINDOW", WS_EX_TOOLWINDOW },
			{ "EX_DLGMODALFRAME", WS_EX_DLGMODALFRAME },
			{ "CLASSSTYLE_CONTAINER", UI_CLASSSTYLE_CONTAINER },
			{ "CLASSSTYLE_FRAME", UI_CLASSSTYLE_FRAME },
			{ "CLASSSTYLE_CHILD", UI_CLASSSTYLE_CHILD },
			{ "CLASSSTYLE_DIALOG", UI_CLASSSTYLE_DIALOG },
			{ "EX_STATICEDGE", WS_EX_STATICEDGE },
			{ "EX_APPWINDOW", WS_EX_APPWINDOW },
			{ "SC_MINIMIZE", SC_MINIMIZE },
			{ "SC_MAXIMIZE", SC_MAXIMIZE },
			{ "SC_RESTORE", SC_RESTORE },
			{ "SC_CLOSE",SC_CLOSE },
		};
		LuaStatic::LuaSetArgsInTable(l, StaticArgs, sizeof(StaticArgs) / sizeof(StaticArgs[0]));

		static LuaReg StaticFuncs[] =
		{
			{ "MsgBox", MsgBox },
			{ "TraceMsg", TraceMsg },
			{ "QuitApp",QuitApp },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs, sizeof(StaticFuncs) / sizeof(StaticFuncs[0]));

		//lua_setfield(l, -2, "DuiLib");
	}
}