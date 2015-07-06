#include "stdafx.h"

#include "LuaEntry.h"
#include "LuaRegister.h"

namespace DuiLib
{
	//////////////////////////////
	std::map<int, void*> LuaStatic::objs;
	std::map<void*, int> LuaStatic::userdataRefs;
	std::string LuaStatic::luaPath;
	int LuaStatic::mtindexRef;
	int LuaStatic::mobjWeakTableRef = LUA_NOREF;
	int LuaStatic::errorFuncRef = LUA_NOREF;

	int LuaStatic::panic(lua_State* l)
	{
		std::string reason;
		reason += "unprotected error in call to Lua API (";
		const char* s = lua_tostring(l, -1);
		reason += s;
		reason += ")";
		throw reason;
		return 0;
	}
	int LuaStatic::print(lua_State* l)
	{
		std::string s;
		int n = lua_gettop(l);
		lua_getglobal(l, "tostring");

		for (int i = 1; i <= n; ++i)
		{
			lua_pushvalue(l, -1); // function to be called
			lua_pushvalue(l, i);  // value to print
			lua_call(l, 1, 1);

			const char* ret = lua_tostring(l, -1);
			if (!ret)
			{
				//log error
			}
			else
			{
				s.append(ret);
			}
			if (i < n)
			{
				s.append("\t");
			}
			else
			{
				s.append("\n");
			}
			lua_pop(l, 1); //pop result
		}

		//CDuiString strlog = CDuiString::FormatString(_T("%s"), s.c_str());
		CDuiString strlog;
		lua_cstring_tool::toDuiString(s, strlog);
#ifdef _VSOUTPUT
		VSOutput::Write(strlog);
#endif
//#ifdef _CONSOLE
		Console::Write(strlog);
//#endif
		return 0;
	}
	int LuaStatic::pcall(lua_State* l)
	{
		int n = lua_gettop(l);
		int ret = lua_pcall(l, n - 1, -1, 0);
		int nn = lua_gettop(l);
		lua_pushboolean(l, ret == 0);
		lua_insert(l, 1);
		return nn + 1;
	}
	int LuaStatic::warn(lua_State* l)
	{
		return 0;
	}
	int LuaStatic::load(lua_State* l)
	{
		//const char* fileName = lua_tostring(l, 1);
		//1: replace '.' with '/'
		//2: + '.lua'
		//3:
		//4: readbytes
		//char* buff;
		//size_t size;
		//luaL_loadbuffer(l, buff, size, strcat("@", fileName));
		return 1;
	}

	int LuaStatic::err_traceback(lua_State* l)
	{
		if(!lua_isstring(l,1))
			return 1;
		lua_getfield(l, LUA_GLOBALSINDEX, "debug");
		if(!lua_istable(l,-1))
		{
			lua_pop(l,1);
			return 1;
		}
		lua_getfield(l, -1, "traceback");
		if(!lua_isfunction(l,-1))
		{
			lua_pop(l,1);
			return 1;
		}
		lua_pushvalue(l,1);
		lua_pushinteger(l,2);
		lua_call(l,2,1);
		return 1;
	}

	void* LuaStatic::GetUserData(lua_State* l,void* ptr)
	{
		std::map<int, void*>::iterator it;
		it = objs.find((int)ptr);
		if ( it != objs.end())
		{
			return (it->second);
		}
		else
		{
			//lua_error(l);
			return NULL;
		}
	}
	void* LuaStatic::CheckUserData(lua_State* l, int index)
	{
		if (!lua_isuserdata(l, index))
		{
			CDuiString str = CDuiString::FormatString(_T("param #%d must be a userdata"),index);
			lua_pushstring(l,str.str().c_str());
			lua_error(l);
			return NULL;
		}
		return GetUserData(l, lua_touserdata(l, index));
	}
	
	void LuaStatic::InitObjsWeakTable(lua_State* l)
	{
		lua_newtable(l); // t
		lua_newtable(l); // t,mt
		lua_pushstring(l, "v"); // t,mt,"v"
		lua_setfield(l, -2, "__mode"); //t,mt
		lua_setmetatable(l, -2); //t
		mobjWeakTableRef = luaL_ref(l, LUA_REGISTRYINDEX);
	}
	int LuaStatic::AddObject2Lua(lua_State* l, const void* const_obj, std::string metatable)
	{
		if (!const_obj)
		{
			lua_pushnil(l);
			return 1;
		}
		void* obj = const_cast<void*>(const_obj);
		if (mobjWeakTableRef != LUA_NOREF)
		{
			int goRefInLua = LUA_NOREF;
			std::map<void*, int>::iterator it;
			it = userdataRefs.find(obj);
			if (it != userdataRefs.end())
			{
				goRefInLua = it->second;
				lua_rawgeti(l, LUA_REGISTRYINDEX, mobjWeakTableRef);
				lua_rawgeti(l, -1, goRefInLua);
				if (lua_isuserdata(l, -1) == false)
				{
					// log error
				}
				lua_insert(l, -2); // obj,t
				lua_pop(l, 1); //obj;
				return 1;
			}
		}
		size_t size = sizeof(void*);
		void* userdata = lua_newuserdata(l, size);
		objs[(int)userdata] = obj;

		//lua_getglobal(l, metatable);
		// 		if (lua_isnil(l, -1))
		// 		{
		// 			lua_pop(l, 1);
		// 		}
		// 		else
		// 		{
		// 			lua_setmetatable(l, -2);
		// 		}
		luaL_getmetatable(l, metatable.c_str());
		lua_setmetatable(l, -2);

		if (mobjWeakTableRef != LUA_NOREF)
		{
			lua_rawgeti(l, LUA_REGISTRYINDEX, mobjWeakTableRef);
			lua_pushvalue(l, -2);//obj,t,obj
			if (lua_isuserdata(l, -1) == false)
			{
				//log error
			}
			int objref = luaL_ref(l, -2); // obj,t
			lua_pop(l, 1); //obj
			userdataRefs[obj] = objref;
		}
		return 1;
	}
	int LuaStatic::AddObject2Lua(lua_State* l, void* obj, std::string metatable)
	{
		if (!obj)
		{
			lua_pushnil(l);
			return 1;
		}
		if (mobjWeakTableRef != LUA_NOREF)
		{
			int goRefInLua = LUA_NOREF;
			std::map<void*,int>::iterator it;
			it = userdataRefs.find(obj);
			if (it != userdataRefs.end())
			{
				goRefInLua = it->second;
				lua_rawgeti(l, LUA_REGISTRYINDEX, mobjWeakTableRef);
				lua_rawgeti(l, -1, goRefInLua);
				if (lua_isuserdata(l, -1) == false)
				{
					// log error
				}
				lua_insert(l, -2); // obj,t
				lua_pop(l, 1); //obj;
				return 1;
			}
		}
		size_t size = sizeof(void*);
		void* userdata = lua_newuserdata(l, size);
		objs[(int)userdata] = obj;

		//lua_getglobal(l, metatable);
// 		if (lua_isnil(l, -1))
// 		{
// 			lua_pop(l, 1);
// 		}
// 		else
// 		{
// 			lua_setmetatable(l, -2);
// 		}
		luaL_getmetatable(l, metatable.c_str());
		lua_setmetatable(l, -2);

		if (mobjWeakTableRef != LUA_NOREF)
		{
			lua_rawgeti(l, LUA_REGISTRYINDEX, mobjWeakTableRef);
			lua_pushvalue(l, -2);//obj,t,obj
			if (lua_isuserdata(l, -1) == false)
			{
				//log error
			}
			int objref = luaL_ref(l, -2); // obj,t
			lua_pop(l, 1); //obj
			userdataRefs[obj] = objref;
		}
		return 1;
	}
	
	int LuaStatic::RemoveObjectInLua(lua_State* l)
	{
		void* ptr = lua_touserdata(l, 1);
		std::map<int, void*>::iterator it;
		it = objs.find((int)ptr);
		if (it != objs.end())
		{
			void* obj = it->second;
			objs.erase(it);
			if (/*false && */mobjWeakTableRef != LUA_NOREF)
			{
				int goRefInLua = LUA_NOREF;
				std::map<void*, int>::iterator it2;
				it2 = userdataRefs.find(obj);
				if (it2 != userdataRefs.end())
				{
					goRefInLua = it2->second;
					lua_rawgeti(l, LUA_REGISTRYINDEX, mobjWeakTableRef);
					luaL_unref(l, -1, goRefInLua);
					lua_pop(l, 1);
					userdataRefs.erase(it2);
				}
			}
		}
		return 0;
	}
	void LuaStatic::LuaSetFuncsInTable(lua_State* l, const LuaReg funcs[],int n)
	{
		for (int i = 0; i < n; ++i)
		{
			lua_pushcfunction(l, funcs[i].func);
			lua_setfield(l, -2, funcs[i].name);
		}
	}

	void LuaStatic::LuaSetFuncs(lua_State* l, const char* package, const LuaReg funcs[],int n)
	{
		bool exist = true;
		lua_getglobal(l, package);
		if (lua_isnil(l, -1))
		{
			exist = false;
			lua_newtable(l);
		}
		LuaSetFuncsInTable(l, funcs,n);
		if (!exist)
			lua_setglobal(l, package);
		else
			lua_pop(l, 1);
	}

	void LuaStatic::LuaSetArgsInTable(lua_State* l, const LuaArgs args[], int n)
	{
		for (int i = 0; i < n; ++i)
		{
			lua_pushinteger(l, args[i].v);
			lua_setfield(l, -2, args[i].name);
		}
	}

	void LuaStatic::WritePoint2Lua(lua_State* l, POINT pt)
	{

	}
	POINT LuaStatic::ReadPointFromLua(lua_State* l, int index)
	{
		POINT pt = { 0, 0 };
		return pt;
	}

	void LuaStatic::WriteSize2Lua(lua_State* l, SIZE pt)
	{

	}
	SIZE LuaStatic::ReadSizeFromLua(lua_State* l, int index)
	{
		SIZE sz = { 0, 0 };
		return sz;
	}

	void LuaStatic::WriteRect2Lua(lua_State* l, RECT pt)
	{

	}
	RECT LuaStatic::ReadRectFromLua(lua_State* l, int index)
	{
		RECT rt = { 0, 0, 0, 0 };
		return rt;
	}

	void LuaStatic::WriteRGBA2Lua(lua_State* l, DWORD col)
	{

	}
	DWORD LuaStatic::ReadRGBAFromLua(lua_State* l, int index)
	{
		DWORD dwCol = 0;
		return dwCol;
	}

	/////////////////////////////////

	lua_State* LuaState::L = NULL;

	/////////////////////////////////

	const char* Lua::mtindex = " \
		function createctable(mt,ct,cmt)\
			ct.__index = function(t,key) \
					local v = mt[key] \
					if v then return v end \
					v = mt['get_' .. key]	\
					if v then return v(t) end	\
					return nil \
			end \
			ct.__newindex = function(t,key,value) \
				local v = mt['set_'..key] \
				if v then v(t,value); return t end \
			end \
			cmt.__index = function(t,key) \
				local v = mt[key] \
				if v then return v end \
				v = mt['get_' .. key] \
				if v then return v() end \
				return nil \
			end \
			cmt.__newindex = function(t,key,value) \
				local v = mt['set_' .. key] \
				if v then v(value); return t end \
			end \
		end \
		";

	const char* Lua::mtindex2 = " \
		function createctable(mt,ct,cmt) \
			ct.__index = c_index(mt) \
			ct.__newindex = c_newindex(mt) \
			cmt.__index = cmt_index(mt) \
			cmt.__newindex = cmt_newindex(mt) \
		end\
		";

	lua_State* Lua::newState()
	{
		static lua_State* l = luaL_newstate();
		return l;
	}

	void Lua::closeState(lua_State*& l)
	{
		if (l)
		{
			lua_close(l);
			l = NULL;
		}
	}

	bool Lua::CoInitialize()
	{
		if (NULL == LuaState::L)
		{
			LuaState::L = newState();
			Register(LuaState::L);
		}
		return LuaState::L != NULL;
	}

	void Lua::CoUninitialize()
	{
		closeState(LuaState::L);
	}

	void Lua::StaticReg(lua_State* l)
	{
		lua_atpanic(l, LuaStatic::panic);
		luaL_openlibs(l);

		lua_getglobal(l, "_G");

		static const LuaReg funcs[] =
		{
			{ "warn", LuaStatic::warn },
			{ "print", LuaStatic::print },
			{ "pcall", LuaStatic::pcall },
		};

		LuaStatic::LuaSetFuncsInTable(l, funcs, sizeof(funcs) / sizeof(funcs[0]));
		lua_pushcfunction(l,LuaStatic::err_traceback);
		LuaStatic::errorFuncRef = luaL_ref(l,LUA_REGISTRYINDEX);
		//if (luaL_loadbuffer(l, mtindex,sizeof(mtindex), "mtindex") != 0 || lua_pcall(l, 0, -1, 0) != 0)
		//{
			//log here
		//	return;
		//}
		/*
		lua_pushcfunction(l, LuaStatic::load);
		int loaderFunc = lua_gettop(l);

		lua_getfield(l, LUA_GLOBALSINDEX, "package");
		lua_getfield(l, -1, "loaders");
		int loaderTable = lua_gettop(l);

		for (int e = lua_objlen(l, loaderTable) + 1; e > 1; e--)
		{
			lua_rawgeti(l, loaderTable, e - 1);
			lua_rawseti(l, loaderTable, e);
		}
		lua_pushvalue(l, loaderFunc);
		lua_rawseti(l, loaderTable, 1);
		lua_settop(l, 0);
		*/

		LuaStatic::InitObjsWeakTable(l);
		lua_pop(l, 1);
	}

	void Lua::Register(lua_State* l)
	{
		StaticReg(l);
		LuaRegister::Register(l);
	}

	void Lua::AddPackagePath(const char* path)
	{
		if (NULL != LuaState::L)
		{
			std::string new_path = "package.path = package.path .. \"";
			if (!path || strlen(path) == 0)
			{
				return;
			}

			if (path[0] != ';')
			{
				new_path += ";";
			}

			new_path.append(path);

			if (path[strlen(path) - 1] != '/')
			{
				new_path.append("/");
			}

			new_path.append("?.lua\" ");

			if (luaL_dostring(LuaState::L, new_path.c_str()))
			{
				char* err = (char*)lua_tostring(LuaState::L, -1);
				::lua_pop(LuaState::L, 1);
			}
		}
	}
	bool Lua::LoadFile(const char* file)
	{
		if (NULL != LuaState::L)
		{
			return 0 == luaL_dofile(LuaState::L, file);
		}
		return false;
	}

	bool Lua::RunString(const char* str)
	{
		if (NULL != LuaState::L)
		{
			return 0 == luaL_dostring(LuaState::L, str);
		}
		return false;
	}

	bool Lua::GetGlobal(const char* name)
	{
		if (NULL != LuaState::L)
		{
			lua_getglobal(LuaState::L, name);
			return true;
		}
		return false;
	}

	void Lua::Pop(int n)
	{
		if (NULL != LuaState::L)
		{
			lua_pop(LuaState::L, n);
		}
	}

	bool Lua::PCall(int narg, int nret, int err /* = 0 */)
	{
		if (NULL != LuaState::L)
		{
			if (0 != lua_pcall(LuaState::L, narg, nret, err))
			{
				::MessageBox(NULL, lua_tostring(LuaState::L, -1), "", MB_OK);
				lua_pop(LuaState::L, 1);
				return false;
			}
			return true;
		}
		return false;
	}

	bool Lua::RequireModule(const char* mod_name)
	{
		lua_State* l = LuaState::L;
		if (!l) return false;
		lua_getglobal(l, "require");
		lua_pushstring(l, mod_name);

		if (lua_pcall(l, 1, 1, 0) == 0) {
			return true;
		}

		::MessageBox(NULL, lua_tostring(l, -1), "", MB_OK);
		lua_pop(l, 1);

		return false;
	}

	bool Lua::LuaFuncExists(const char *func)
	{
		return false;
	}

	bool Lua::CallLuaFuncBegin(const char* func)
	{
		return false;
	}

		
	void Lua::CallLuaFuncEnd(int nArgs, int nRets)
	{
		lua_State* l = LuaState::L;
		if (!l) return;

		if (lua_pcall(l, nArgs, nRets, 0) != 0) {
			::MessageBox(NULL, lua_tostring(l, -1), "", MB_OK);
			lua_pop(l, 1);
		}
	}


	bool Lua::CallLuaFuncInTableBegin(int nRefLua, const char* func)
	{
		lua_State* l = LuaState::L;
		lua_rawgeti(l, LUA_REGISTRYINDEX, nRefLua); //obj_t,
		if (!lua_istable(l, -1))
		{
			lua_pop(l, -1);
			return false;
		}
		lua_getfield(l, -1, func);
		lua_remove(l, -2);
		if (lua_isnil(l, -1) != 0) {
			lua_pop(l, 1);
			return false;
		}
		lua_rawgeti(l, LUA_REGISTRYINDEX, nRefLua); //obj_t,
		return true;
	}

	bool Lua::CallLuaPlusObjectBegin(int nLuaMsgRef, int nLuaObjRef, const char* func)
	{
		lua_State* l = LuaState::L;
		lua_rawgeti(l, LUA_REGISTRYINDEX, nLuaMsgRef); //msg_t,
		const char* name = lua_typename(l, -1);
		if (!lua_istable(l, -1))
		{

			lua_pop(l, -1);
			return false;
		}
		lua_getfield(l, -1, func);
		lua_remove(l, -2);
		if (lua_isnil(l, -1) != 0) {
			lua_pop(l, 1);
			return false;
		}

		if (!lua_isfunction(l, -1))
		{
			lua_pop(l, 1);
			return false;
		}

		lua_rawgeti(l, LUA_REGISTRYINDEX, nLuaObjRef); //obj_t,
		return true;
	}
}