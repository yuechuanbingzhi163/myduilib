#ifndef _Lua_H
#define _Lua_H

#pragma once
#include <map>

#include "LuaOp.h"

namespace DuiLib
{
	
	inline std::string ControlClass2MetatableName(IDuiObject* pControl)
	{
		CDuiString sName(pControl->GetClass());
		std::string mt;

		mt.append("C");
		mt.append(lua_cstring_tool::toLuaString(sName).c_str());
		
		return mt;
	}
		
#define METATABLE_NAME(pCtrl)    ControlClass2MetatableName(pCtrl)

#define DECL_LUA_FUNC(func) \
	static int func(lua_State* l);

#define IMPL_LUA_FUNC(clsname,func) \
	int clsname::func(lua_State* l)

#define REG_MODULE(clsname) \
	clsname::Register(l);

	typedef int(*LuaCFunction)(lua_State* l);

	struct LuaReg
	{
		const char* name;
		LuaCFunction func;
	};//end of LuaReg

	struct LuaArgs 
	{
		const char* name;
		unsigned int v;
	};

	class LuaStatic
	{
	public:
		static std::map<int, void*> objs;
		static std::map<void*, int> userdataRefs;
		static std::string luaPath;
		static int mtindexRef;
		static int mobjWeakTableRef;
		static int errorFuncRef;
	public:
		static int panic(lua_State* l);
		static int print(lua_State* l);
		static int pcall(lua_State* l);
		static int warn(lua_State* l);
		static int load(lua_State* l);
		static int err_traceback(lua_State* l);
	public: 
		static void* GetUserData(lua_State* l,void* ptr);

		static void* CheckUserData(lua_State* l, int index);

		static void InitObjsWeakTable(lua_State* l);

		static int AddObject2Lua(lua_State* l, const void* obj, std::string metatable);
		static int AddObject2Lua(lua_State* l, void* obj,std::string metatable);
		static int RemoveObjectInLua(lua_State* l);

		static void WritePoint2Lua(lua_State* l, POINT pt);
		static POINT ReadPointFromLua(lua_State* l, int index);

		static void WriteSize2Lua(lua_State* l, SIZE pt);
		static SIZE ReadSizeFromLua(lua_State* l, int index);

		static void WriteRect2Lua(lua_State* l, RECT pt);
		static RECT ReadRectFromLua(lua_State* l, int index);

		static void WriteRGBA2Lua(lua_State* l, DWORD col);
		static DWORD ReadRGBAFromLua(lua_State* l, int index);

		static void LuaSetFuncsInTable(lua_State* l, const LuaReg funcs[],int n);
		static void LuaSetFuncs(lua_State* l, const char* package, const LuaReg funcs[],int n);

		static void LuaSetArgsInTable(lua_State* l, const LuaArgs args[], int n);

	};//end of LuaStatic

	class LUAUILIB_API LuaState
	{
	public:
		static lua_State* L;
	};

	class LUAUILIB_API Lua
	{
	public:
		static const char* mtindex;
		static const char* mtindex2;
	public:
		static lua_State* newState();
		static void closeState(lua_State*& l);

		static bool CoInitialize();
		static void CoUninitialize();
	protected:
		static void StaticReg(lua_State* l);
		static void Register(lua_State* l);
	public:
		static void AddPackagePath(const char* path);
		static bool LoadFile(const char* file);
		static bool RunString(const char* str);

		static bool GetGlobal(const char* name);
		static void Pop(int n);
		static bool PCall(int narg, int nret, int err = 0);

		static bool RequireModule(const char* mod_name);
		static bool LuaFuncExists(const char *func);
		static bool CallLuaFuncBegin(const char* func);
		static void CallLuaFuncEnd(int nArgs, int nRets = 0);

		static bool CallLuaFuncInTableBegin(int nRefLua, const char* func);

		static bool CallLuaPlusObjectBegin(int nLuaMsgRef,int nLuaObjRef, const char* func);
		
	};

	class LUAUILIB_API LuaStackOpt
	{
	public:
		template<typename T>
		static void PushStack(T& value)
		{
			lua_op_t<T>::push_stack(LuaState::L, value);
		}

		template<typename T>
		static void ToLuaValue(int pos_, OUT T& value, T& default_ = T())
		{
			lua_op_t<T>::lua_to_value(LuaState::L, pos_, value,default_);
		}

		static void PushObject(void* obj, const char* metatable)
		{
			LuaStatic::AddObject2Lua(LuaState::L, obj, metatable);
		}
	};
}


#endif