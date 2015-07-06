#ifndef _LuaCoreInclude_h
#define _LuaCoreInclude_h
#pragma once

#include "LuaStruct.h"
#include "LuaCDialogBuilder.h"
#include "LuaCMarkup.h"
#include "LuaCPaintManagerUI.h"
#include "LuaCRender.h"
#include "LuaCWindowWnd.h"

namespace DuiLib
{
	class RegDuiCore2Lua
	{
	public:
		static void RegModule(lua_State* l)
		{
			REG_MODULE(LuaTNotifyUI)
			REG_MODULE(LuaTFontInfo)
			REG_MODULE(LuaTImageInfo)
			REG_MODULE(LuaTEventUI)
			REG_MODULE(LuaEventMarco)
			REG_MODULE(LuaMsgMarco)
			REG_MODULE(LuaDuiLib)
			REG_MODULE(LuaCDialogBuilder)
			REG_MODULE(LuaCMarkup)
			REG_MODULE(LuaCMarkupNode)
			REG_MODULE(LuaCPaintManagerUI)
			REG_MODULE(LuaCRenderClip)
			REG_MODULE(LuaCRenderEngine)
			REG_MODULE(LuaCNotifyPump)
			REG_MODULE(LuaCWindowWnd)
		}
	};
}

#endif//_LuaCoreInclude_h