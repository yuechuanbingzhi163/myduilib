
#include "StdAfx.h"

#include "LuaRegister.h"

#include "LuaControl/LuaControlInclude.h"
#include "LuaCore/LuaCoreInclude.h"
#include "LuaUtils/LuaUtilsInclude.h"

namespace DuiLib
{
	void LuaRegister::Register(lua_State* l)
	{
		//lua_getglobal(l, "_G");
		lua_newtable(l);
		
		RegDuiControls2Lua::RegModule(l);
		RegDuiCore2Lua::RegModule(l);
		RegDuiUtils2Lua::RegModule(l);

		//lua_pop(l, 1);
		lua_setglobal(l, "DuiLib");
	}
}