#include "StdAfx.h"

#include "LuaCRender.h"

namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCRenderClip, isnil)
	{
		CRenderClip* self;
		self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));

		lua_pushboolean(l, self == NULL ? 1 : 0);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, _gc)
	{	
		CRenderClip* self;
		self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, _eq)
	{
		try
		{
			CRenderClip* left;
			left = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));

			CRenderClip* right;
			right = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 2));

			lua_pushboolean(l, left == right ? 1 : 0);
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCRenderClip::_eq"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderClip, _tostring)
	{
		try
		{
			CRenderClip* self;
			self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
			if (self)
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%p)"), "RenderClip", (void*)self);
				lua_pushstring(l, str.str().c_str());
			}
			else
			{
				CDuiString str = CDuiString::FormatString(_T("%s(addr:%s)"), "RenderClip", "nil");
				lua_pushstring(l, str.str().c_str());
			}
			return 1;
		}
		catch (...)
		{
			DuiException(_T("LuaCRenderClip::_tostring"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderClip, Delete)
	{
		CRenderClip* self;
		self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::RemoveObjectInLua(l);
		SafeDelete(self);
		return 0;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, New)
	{
		CRenderClip  *self = new CRenderClip();
		LuaStatic::AddObject2Lua(l, self, "CRenderClip");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, GetClassName)
	{
		lua_pushstring(l, "RenderClip");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, GenerateClip)
	{
		HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
		RECT rc = LuaStatic::ReadRectFromLua(l,2);
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 3));
		CRenderClip::GenerateClip(hDC,rc,*self);
		LuaStatic::AddObject2Lua(l, self, "CRenderClip");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, GenerateRoundClip)
	{
		HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
		RECT rc = LuaStatic::ReadRectFromLua(l,2);
		RECT rcItem = LuaStatic::ReadRectFromLua(l,3);
		int width = lua_tointeger(l,4);
		int height = lua_tointeger(l,5);
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 6));
		CRenderClip::GenerateRoundClip(hDC,rc,rcItem,width,height,*self);
		LuaStatic::AddObject2Lua(l, self, "CRenderClip");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, UseOldClipBegin)
	{
		HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 2));
		CRenderClip::UseOldClipBegin(hDC,*self);
		LuaStatic::AddObject2Lua(l, self, "CRenderClip");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, UseOldClipEnd)
	{
		HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 2));
		CRenderClip::UseOldClipEnd(hDC,*self);
		LuaStatic::AddObject2Lua(l, self, "CRenderClip");
		return 1;
	}

	IMPL_LUA_FUNC(LuaCRenderClip, getRectItem)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::WriteRect2Lua(l,self->rcItem);
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, getHDC)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::AddObject2Lua(l, self->hDC, "HDC");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, getHRGN)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::AddObject2Lua(l, self->hRgn, "HRGN");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, getOldHRGN)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		LuaStatic::AddObject2Lua(l, self->hOldRgn, "HRGN");
		return 1;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, setRectItem)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		RECT rcItem = LuaStatic::ReadRectFromLua(l,2);
		self->rcItem = rcItem;
		return 0;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, setHDC)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 2));
		self->hDC = hDC;
		return 0;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, setHRGN)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		HRGN hRgn = static_cast<HRGN>(LuaStatic::CheckUserData(l, 2));
		self->hRgn = hRgn;
		return 0;
	}
	IMPL_LUA_FUNC(LuaCRenderClip, setOldHRGN)
	{
		CRenderClip *self = static_cast<CRenderClip*>(LuaStatic::CheckUserData(l, 1));
		HRGN hOldRgn = static_cast<HRGN>(LuaStatic::CheckUserData(l, 2));
		self->hOldRgn = hOldRgn;
		return 0;
	}

	void LuaCRenderClip::Register(lua_State* l)
	{
		lua_newtable(l); // t,

		static LuaReg StaticFuncs[] =
		{
			{ "GetClassName", GetClassName },
			{ "New", New },
			{ "GenerateClip", GenerateClip },
			{ "GenerateRoundClip", GenerateRoundClip },
			{ "UseOldClipBegin", UseOldClipBegin },
			{ "UseOldClipEnd", UseOldClipEnd },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "RenderClip");
		
		luaL_newmetatable(l, "CRenderClip");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		static LuaReg MethodFuncs[] =
		{
			{ "isnil",isnil },
			{ "__eq",_eq },
			{ "__gc",_gc },
			{ "__tostring", _tostring },
			{ "Delete",Delete},
			{ "getRectItem",getRectItem },
			{ "getHDC",getHDC },
			{ "getHRGN", getHRGN },
			{ "getOldHRGN",getOldHRGN},
			{ "setRectItem",setRectItem },
			{ "setHDC",setHDC },
			{ "setHRGN",setHRGN },
			{ "setOldHRGN",setOldHRGN},
		};
		LuaStatic::LuaSetFuncsInTable(l, MethodFuncs, sizeof(MethodFuncs) / sizeof(MethodFuncs[0]));
		lua_pop(l, 1);
	}
};
namespace DuiLib
{
	IMPL_LUA_FUNC(LuaCRenderEngine, AdjustColor)
	{
		try
		{
			DWORD dwColor = LuaStatic::ReadRGBAFromLua(l,2);
			short H = (short)lua_tonumber(l,2);
			short S = (short)lua_tonumber(l,3);
			short L = (short)lua_tonumber(l,4);
			DWORD dwCol = CRenderEngine::AdjustColor(dwColor,H,S,L);
			LuaStatic::WriteRGBA2Lua(l,dwCol);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::AdjustColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, LoadBitmap)
	{
		try
		{
			STRINGorID bitmap(_T(""));
			if (lua_isstring(l, 1))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 1, pstrText);
				bitmap = STRINGorID(pstrText.GetData());
			}
			else if(lua_isnumber(l,1))
				bitmap = STRINGorID(lua_tointeger(l,1));
			LPCTSTR type = NULL;
			if (lua_isstring(l,2))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
				type = pstrText.GetData();
			}	
			DWORD mask = 0;
			if(lua_isnumber(l,3))
				mask = (DWORD)lua_tonumber(l,3);
			HBITMAP hBitmap = CRenderEngine::LoadBitmap(bitmap,type,mask);
			if (hBitmap)
				LuaStatic::AddObject2Lua(l, hBitmap, "HBITMAP");
			else
				lua_pushnil(l);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::LoadBitmap"));
			return 0;
		}
		
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, LoadImage)
	{
		try
		{
			STRINGorID bitmap(_T(""));
			if (lua_isstring(l, 1))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 1, pstrText);
				bitmap = STRINGorID(pstrText.GetData());
			}
			else if(lua_isnumber(l,1))
				bitmap = STRINGorID(lua_tointeger(l,1));
			LPCTSTR type = NULL;
			if (lua_isstring(l,2))
			{
				CDuiString pstrText;
				lua_op_t<CDuiString>::lua_to_value(l, 2, pstrText);
				type = pstrText.GetData();
			}
			DWORD mask = 0;
			if(lua_isnumber(l,3))
				mask = (DWORD)lua_tonumber(l,3);	
			TImageInfo* pImageInfo = CRenderEngine::LoadImage(bitmap,type,mask);
			if (pImageInfo)
				LuaStatic::AddObject2Lua(l, pImageInfo, "TImageInfo");
			else
				lua_pushnil(l);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::LoadImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, FreeImage)
	{
		try
		{
			TImageInfo* pImageInfo;
			pImageInfo = static_cast<TImageInfo*>(LuaStatic::CheckUserData(l, 1));
			CRenderEngine::FreeImage(pImageInfo);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::FreeImage"));
			return 0;
		}	
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawImage)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			HBITMAP hBitmap = static_cast<HBITMAP>(LuaStatic::CheckUserData(l, 2));
			RECT rc = LuaStatic::ReadRectFromLua(l,3);
			RECT rcPaint = LuaStatic::ReadRectFromLua(l, 4);
			RECT rcBmpPart = LuaStatic::ReadRectFromLua(l, 5);
			RECT rcCorners = LuaStatic::ReadRectFromLua(l, 6);
			bool alphaChannel = lua_toboolean(l,7) != 0;
			BYTE uFade = 255;
			if(lua_isnumber(l,8))
				uFade = lua_tointeger(l,8);
			bool hole = false;
			if(lua_isboolean(l,9))
				hole = lua_toboolean(l,9) != 0;
			bool xtiled = false;
			if(lua_isboolean(l,10))
				xtiled = lua_toboolean(l,10) != 0;
			bool ytiled = false;
			if(lua_isboolean(l,11))
				ytiled = lua_toboolean(l,11) != 0;

			CRenderEngine::DrawImage(hDC,hBitmap,rc,rcPaint,rcBmpPart,rcCorners,alphaChannel,uFade,hole,xtiled,ytiled);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawImage"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawImageString)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 2));
			RECT rcItem = LuaStatic::ReadRectFromLua(l,3);
			RECT rcPaint = LuaStatic::ReadRectFromLua(l, 4);
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 5, pstrText);
			LPCTSTR pStrImage = pstrText.GetData();

			LPCTSTR pStrModify = NULL;
			if (lua_isstring(l, 6))
			{
				CDuiString pstrText2;
				lua_op_t<CDuiString>::lua_to_value(l, 6, pstrText2);
				pStrModify = pstrText2.GetData();
			}
			bool bNeedAlpha = FALSE;
			if(lua_isboolean(l,7))
				bNeedAlpha = lua_toboolean(l,7) != 0;
			BYTE bNewFade = 255;
			if(lua_isboolean(l,8))
				bNewFade = lua_toboolean(l,8) != 0;
			int hIndex = 0;
			if(lua_isnumber(l,9))
				hIndex = lua_tointeger(l,9);
			int vIndex = 0;
			if(lua_isnumber(l,10))
				vIndex = lua_tointeger(l,10);
			bool bSuss = CRenderEngine::DrawImageString(hDC,pManager,rcItem,rcPaint,pStrImage,pStrModify,bNeedAlpha,bNewFade,hIndex,vIndex);
			lua_pushboolean(l,bSuss ? 1 : 0);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawImageString"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawColor)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			DWORD color = LuaStatic::ReadRGBAFromLua(l,3);
			CRenderEngine::DrawColor(hDC,rc,color);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawColor"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawGradient)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			DWORD dwFirst = (DWORD)lua_tonumber(l,3);
			DWORD dwSecond = (DWORD)lua_tonumber(l,4);
			bool bVertical = lua_toboolean(l,5) != 0;
			int nSteps = lua_tointeger(l,6);
			CRenderEngine::DrawGradient(hDC,rc,dwFirst,dwSecond,bVertical,nSteps);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawGradient"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawLine)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			int nSize = lua_tointeger(l,3);
			DWORD dwPenColor = LuaStatic::ReadRGBAFromLua(l,4);
			int nPenStyle = PS_SOLID;
			if (lua_isnumber(l,5))
				nPenStyle = lua_tointeger(l,5);
			CRenderEngine::DrawLine(hDC,rc,nSize,dwPenColor,nPenStyle);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawLine"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawRect)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			int nSize = lua_tointeger(l,3);
			DWORD dwPenColor = LuaStatic::ReadRGBAFromLua(l,4);
			CRenderEngine::DrawRect(hDC,rc,nSize,dwPenColor);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawRoundRect)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			RECT rc = LuaStatic::ReadRectFromLua(l,2);
			int width = lua_tointeger(l,3);
			int height = lua_tointeger(l,4);
			int nSize = lua_tointeger(l,5);
			DWORD dwPenColor = LuaStatic::ReadRGBAFromLua(l,6);
			CRenderEngine::DrawRoundRect(hDC,rc,width,height,nSize,dwPenColor);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawRoundRect"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawText)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 2));
			RECT rc = LuaStatic::ReadRectFromLua(l,3);
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 4, pstrText);
			DWORD dwTextColor = LuaStatic::ReadRGBAFromLua(l,5);
			int iFont = lua_tointeger(l,6);
			UINT uStyle = lua_tointeger(l,7);
			CRenderEngine::DrawText(hDC,pManager,rc,pstrText,dwTextColor,iFont,uStyle);
			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawText"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, DrawHtmlText)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 2));
			RECT rc = LuaStatic::ReadRectFromLua(l,3);
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 4, pstrText);
			DWORD dwTextColor = LuaStatic::ReadRGBAFromLua(l,5);

			return 0;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::DrawHtmlText"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, GenerateBitmap)
	{
		try
		{
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 1));
			CControlUI* pControl = static_cast<CControlUI*>(LuaStatic::CheckUserData(l,2));
			RECT rc = LuaStatic::ReadRectFromLua(l,3);
			HBITMAP hBitmap = CRenderEngine::GenerateBitmap(pManager,pControl,rc);
			if (hBitmap)
				LuaStatic::AddObject2Lua(l, hBitmap, "HBITMAP");
			else
				lua_pushnil(l);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::GenerateBitmap"));
			return 0;
		}
	}
	IMPL_LUA_FUNC(LuaCRenderEngine, GetTextSize)
	{
		try
		{
			HDC hDC = static_cast<HDC>(LuaStatic::CheckUserData(l, 1));
			CPaintManagerUI* pManager = static_cast<CPaintManagerUI*>(LuaStatic::CheckUserData(l, 2));
			CDuiString pstrText;
			lua_op_t<CDuiString>::lua_to_value(l, 3, pstrText);
			int iFont = lua_tointeger(l,4);
			UINT uStyle = (UINT)lua_tonumber(l,5);
			SIZE sz = CRenderEngine::GetTextSize(hDC,pManager,pstrText,iFont,uStyle);
			LuaStatic::WriteSize2Lua(l,sz);
			return 1;
		}
		catch(...)
		{
			DuiException(_T("LuaCRenderEngine::GenerateBitmap"));
			return 0;
		}
	}

	void LuaCRenderEngine::Register(lua_State* l)
	{
		lua_newtable(l); // t,

		static LuaReg StaticFuncs[] =
		{
			{ "AdjustColor", AdjustColor },
			{ "LoadBitmap", LoadBitmap },
			{ "LoadImage", LoadImage },
			{ "FreeImage", FreeImage },
			{ "DrawImage", DrawImage },
			{ "DrawImageString", DrawImageString },
			{ "DrawColor", DrawColor },
			{ "DrawGradient", DrawGradient },
			{ "DrawLine", DrawLine },
			{ "DrawRect", DrawRect },
			{ "DrawRoundRect", DrawRoundRect },
			{ "DrawText", DrawText },
			{ "DrawHtmlText", DrawHtmlText },
			{ "GenerateBitmap", GenerateBitmap },
			{ "GetTextSize", GetTextSize },
		};
		LuaStatic::LuaSetFuncsInTable(l, StaticFuncs,sizeof(StaticFuncs)/sizeof(StaticFuncs[0]));
		lua_setfield(l, -2, "RenderEngine");
		
		luaL_newmetatable(l, "CRenderEngine");
			
		lua_pushvalue(l, -1);
		lua_setfield(l, -2, "__index");

		lua_pop(l, 1);
	}
}