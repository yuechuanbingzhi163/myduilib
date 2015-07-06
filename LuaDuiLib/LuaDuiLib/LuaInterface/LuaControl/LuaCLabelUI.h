#ifndef _Lua_CLabelUI_h
#define _Lua_CLabelUI_h
#pragma once


namespace DuiLib
{
	class LuaCLabelUI
	{
	public:
		
		DECL_LUA_FUNC(New)
		DECL_LUA_FUNC(GetClassName)

		DECL_LUA_FUNC(SetTextStyle)
		DECL_LUA_FUNC(GetTextStyle)
		DECL_LUA_FUNC(SetTextColor)
		DECL_LUA_FUNC(GetTextColor)
		DECL_LUA_FUNC(SetDisabledTextColor)
		DECL_LUA_FUNC(GetDisabledTextColor)
		DECL_LUA_FUNC(SetFont)
		DECL_LUA_FUNC(GetFont)
		DECL_LUA_FUNC(GetTextPadding)
		DECL_LUA_FUNC(SetTextPadding)
		DECL_LUA_FUNC(IsShowHtml)
		DECL_LUA_FUNC(SetShowHtml)

		DECL_LUA_FUNC(SetEnabledEffect)
		DECL_LUA_FUNC(GetEnabledEffect)
		DECL_LUA_FUNC(SetTransShadow)
		DECL_LUA_FUNC(GetTransShadow)
		DECL_LUA_FUNC(SetTransShadow1)
		DECL_LUA_FUNC(GetTransShadow1)
		DECL_LUA_FUNC(SetTransText)
		DECL_LUA_FUNC(GetTransText)
		DECL_LUA_FUNC(SetTransText1)
		DECL_LUA_FUNC(GetTransText1)
		DECL_LUA_FUNC(SetTransStroke)
		DECL_LUA_FUNC(GetTransStroke)
		DECL_LUA_FUNC(SetGradientLength)
		DECL_LUA_FUNC(GetGradientLength)
		DECL_LUA_FUNC(SetTextRenderingHintAntiAlias)
		DECL_LUA_FUNC(GetTextRenderingHintAntiAlias)
		DECL_LUA_FUNC(SetShadowOffset)
		DECL_LUA_FUNC(GetShadowOffset)
		DECL_LUA_FUNC(SetTextColor1)
		DECL_LUA_FUNC(GetTextColor1)
		DECL_LUA_FUNC(SetTextShadowColorA)
		DECL_LUA_FUNC(GetTextShadowColorA)
		DECL_LUA_FUNC(SetTextShadowColorB)
		DECL_LUA_FUNC(GetTextShadowColorB)
		DECL_LUA_FUNC(SetStrokeColor)
		DECL_LUA_FUNC(GetStrokeColor)
		DECL_LUA_FUNC(SetGradientAngle)
		DECL_LUA_FUNC(GetGradientAngle)
		DECL_LUA_FUNC(SetEnabledStroke)
		DECL_LUA_FUNC(GetEnabledStroke)
		DECL_LUA_FUNC(SetEnabledShadow)
		DECL_LUA_FUNC(GetEnabledShadow)

		static void Register(lua_State* l);
	};
}


#endif//_Lua_CLabelUI_h