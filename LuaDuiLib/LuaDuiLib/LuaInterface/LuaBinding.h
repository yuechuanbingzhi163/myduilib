#ifndef _LuaBinding_H
#define _LuaBinding_H

// copy from: http://lua-users.org/wiki/CppBindingWithLunar
// a:size() <==> a.size(a)。

namespace DuiLib
{

	#define LUA_DECLARE_METHOD(Class, Name) {#Name, &Class::Name}

	template <typename T,const char* className=CDuiString(T::GetClassName()).str().c_str()> 
	class LuaBinding 
	{
		typedef struct { T *pT; } userdataType;
	public:
		// 导出给lua的函数必须是这个类型
		typedef int (T::*mfp)(lua_State *L);

		typedef struct { const char *name; mfp mfunc; } RegType;

		// 向lua注册导出类
		static void Register(lua_State *L) 
		{
			// 创建method table方法表
			lua_newtable(L);
			int methods = lua_gettop(L);

			// 创建meta元表
			// 说明：luaL_newmetatable函数创建一个新表(此表将被用作metatable), 将新表放
			// 到栈顶并建立表和registry中T::className的联系。
			luaL_newmetatable(L, className);
			int metatable = lua_gettop(L);

			// store method table in globals so that
			// scripts can add functions written in Lua.
			// 将method table拷贝压入栈顶，然后设置到全局索引中，名称为T::className
			lua_pushvalue(L, methods);

			// set函数的功能：如果set(A,B,C),
			// 那么，将当前栈顶的元素作为值,C作为key，添加到索引为B的表中.
			// 当B为负数时，-1，-2，被值、key占用，-3是压入值之前的元素......
			// 所以这行代码读作：在LUA_GLOBALSINDEX表中添加T::className为key的值。
			//                   值就是那个栈顶元素。
			// =========调用set后，栈回到压入值之前的状态，因为lua_settable弹出值和key。======
			set(L, LUA_GLOBALSINDEX, className);

			// hide metatable from Lua getmetatable()
			// 设置meta table的__metatable域
			lua_pushvalue(L, methods);
			set(L, metatable, "__metatable");

			// 设置meta table的__index域
			// 说明：__index域可以是一个函数，也可以是一个表
			// 当它是一个函数的时候，Lua将table和缺少的域作为
			// 参数调用这个函数；当它是一个表的时候，Lua将在这
			// 个表中看是否有缺少的域。
			lua_pushvalue(L, methods);
			set(L, metatable, "__index");

			// 设置meta table的__tostring域
			lua_pushcfunction(L, tostring_T);
			set(L, metatable, "__tostring");

			// 设置meta table的__gc域
			lua_pushcfunction(L, gc_T);
			set(L, metatable, "__gc");

			lua_newtable(L);                // mt for method table
			lua_pushcfunction(L, new_T);
			lua_pushvalue(L, -1);           // dup new_T function
			set(L, methods, "new");         // add new_T to method table
			set(L, -3, "__call");           // mt.__call = new_T
			lua_setmetatable(L, methods);

			// fill method table with methods from class T
			for (RegType *l = T::methods; l->name; l++)
			{
				lua_pushstring(L, l->name);

				// Pushes a light userdata onto the stack. 
				// Userdata represent C values in Lua. 
				// A light userdata represents a pointer. 
				// It is a value (like a number): 
				// you do not create it, it has no 
				// individual metatable, and it is 
				// not collected (as it was never created). 
				// A light userdata is equal to "any" 
				// light userdata with the same C address. 
				lua_pushlightuserdata(L, (void*)l);

				// 创建c函数thunk，带有1个upvalue，并从栈上弹出这个upvalue
				// 此处，upvalue是lua_pushlightuserdata(L, (void*)l)压入的l
				lua_pushcclosure(L, thunk, 1);

				lua_settable(L, methods);
			}

			lua_pop(L, 2);  // drop metatable and method table
		}

		// call named lua method from userdata method table
		static int call(lua_State *L, const char *method,int nargs=0, int nresults=LUA_MULTRET, int errfunc=0)
		{
			int base = lua_gettop(L) - nargs;  // userdata index
			if (!luaL_checkudata(L, base, T::className))
			{
				lua_settop(L, base-1);           // drop userdata and args
				lua_pushfstring(L, "not a valid %s userdata", T::className);
				return -1;
			}

			lua_pushstring(L, method);         // method name
			lua_gettable(L, base);             // get method from userdata
			if (lua_isnil(L, -1))
			{            // no method?
				lua_settop(L, base-1);           // drop userdata and args
				lua_pushfstring(L, "%s missing method '%s'", T::className, method);
				return -1;
			}
			lua_insert(L, base);               // put method under userdata, args

			int status = lua_pcall(L, 1+nargs, nresults, errfunc);  // call method
			if (status) 
			{
				const char *msg = lua_tostring(L, -1);
				if (msg == NULL) msg = "(error with no message)";
				lua_pushfstring(L, "%s:%s status = %d/n%s",
					T::className, method, status, msg);
				lua_remove(L, base);             // remove old message
				return -1;
			}
			return lua_gettop(L) - base + 1;   // number of results
		}

		// push onto the Lua stack a userdata containing a pointer to T object
		static int push(lua_State *L, T *obj, bool gc=false)
		{
			if (!obj)
			{ 
				lua_pushnil(L); return 0; 
			}
			luaL_getmetatable(L, T::className);  // lookup metatable in Lua registry
			if (lua_isnil(L, -1))
			{
				luaL_error(L, "%s missing metatable", T::className);
			}
			int mt = lua_gettop(L);
			
			// subtable首先在这个表上查找userdata的值
			// 如果表有userdata字段则返回该字段的值（是一个表）
			// 如果表没有userdata字段则新建一个表，设置这个新建表
			// 的meta表的__mode="v"。
			subtable(L, mt, "userdata", "v");

			// pushuserdata函数会在栈顶放置userdata
			// 这个userdata可能是根据key查找到的，也
			// 可能是新创建的。
			userdataType *ud =
				static_cast<userdataType*>(pushuserdata(L, obj, sizeof(userdataType)));
			if (ud)
			{
				ud->pT = obj;  // store pointer to object in userdata
				lua_pushvalue(L, mt);
				lua_setmetatable(L, -2);
				if (gc == false) 
				{
					lua_checkstack(L, 3);
					subtable(L, mt, "do not trash", "k");
					lua_pushvalue(L, -2);
					lua_pushboolean(L, 1);
					lua_settable(L, -3);
					lua_pop(L, 1);
				}
			}
			lua_replace(L, mt);
			lua_settop(L, mt);
			return mt;  // index of userdata containing pointer to T object
		}

		// get userdata from Lua stack and return pointer to T object
		static T *check(lua_State *L, int narg) 
		{
			userdataType *ud =
				static_cast<userdataType*>(luaL_checkudata(L, narg, T::className));
			if(!ud)
			{
				luaL_typerror(L, narg, T::className);
			}
			return ud->pT;  // pointer to T object
		}

	private:
		LuaBinding();  // hide default constructor

		// member function dispatcher
		static int thunk(lua_State *L) 
		{
			// stack has userdata, followed by method args
			T *obj = check(L, 1);  // get 'self', or if you prefer, 'this'
			lua_remove(L, 1);  // remove self so member function args start at index 1
			// get member function from upvalue
			RegType *l = static_cast<RegType*>(lua_touserdata(L, lua_upvalueindex(1)));
			return (obj->*(l->mfunc))(L);  // call member function
		}

		// create a new T object and
		// push onto the Lua stack a userdata containing a pointer to T object
		static int new_T(lua_State *L) 
		{
			lua_remove(L, 1);   // use classname:new(), instead of classname.new()
			T *obj = new T(L);  // call constructor for T objects
			push(L, obj, true); // gc_T will delete this object
			return 1;           // userdata containing pointer to T object
		}

		// garbage collection metamethod
		static int gc_T(lua_State *L)
		{
			if (luaL_getmetafield(L, 1, "do not trash"))
			{
				lua_pushvalue(L, 1);  // dup userdata
				lua_gettable(L, -2);
				if (!lua_isnil(L, -1)) return 0;  // do not delete object
			}
			userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
			T *obj = ud->pT;
			if (obj)
			{
				delete obj;  // call destructor for T objects
			}
			return 0;
		}

		static int tostring_T (lua_State *L) 
		{
			char buff[32];
			userdataType *ud = static_cast<userdataType*>(lua_touserdata(L, 1));
			T *obj = ud->pT;
			sprintf(buff, "%p", (void*)obj);
			lua_pushfstring(L, "%s (%s)", T::className, buff);

			return 1;
		}

		static void set(lua_State *L, int table_index, const char *key) 
		{
			lua_pushstring(L, key);
			lua_insert(L, -2);  // swap value and key
			lua_settable(L, table_index);
		}

		// lua weak table说明:
		// 表的weak性由它的metatable的__mode域来指定的。
		// 在这个域存在的时候，必须是个字符串：
		// 如果这个字符串包含小写字母‘k’，
		// 这个table中的keys就是weak的；
		// 如果这个字符串包含小写字母‘v’，
		// 这个table中的vaules就是weak的。
		// 这个字符串也可以是"kv".表示keys和vaules都是weak的。

		// 此函数返回一个table，该table的__mode被赋值
		// t = {__mode = "v", __metatable=t}
		static void weaktable(lua_State *L, const char *mode)
		{
			lua_newtable(L);
			lua_pushvalue(L, -1);  // table is its own metatable
			lua_setmetatable(L, -2);
			lua_pushliteral(L, "__mode");
			lua_pushstring(L, mode);
			lua_settable(L, -3);   // metatable.__mode = mode
		}

		static void subtable(lua_State *L, int tindex, const char *name, const char *mode) 
		{
			// 在指定的表上查找name的值
			// 如果没有对应值，则新建一个表，并将这个表作为自身的meta表
			// 这个表在栈顶
			lua_pushstring(L, name);
			lua_gettable(L, tindex);
			if (lua_isnil(L, -1))
			{
				lua_pop(L, 1);
				lua_checkstack(L, 3);
				weaktable(L, mode);
				lua_pushstring(L, name);
				lua_pushvalue(L, -2);
				lua_settable(L, tindex);
			}
		}

		// 检查表（即当前栈顶元素）中key为键的元素是否存在
		// 如果存在，则返回0，
		// 如果不存在，则创建一个userdata，将这个新建的userdata以key为键
		// 添加到表中，并返回这个userdata的指针
		// pushuserdata函数会在栈顶留下lookup[key]
		static void *pushuserdata(lua_State *L, void *key, size_t sz) 
		{
			void *ud = 0;
			lua_pushlightuserdata(L, key);
			lua_gettable(L, -2);     // lookup[key]
			if (lua_isnil(L, -1)) 
			{
				lua_pop(L, 1);         // drop nil
				lua_checkstack(L, 3);
				ud = lua_newuserdata(L, sz);  // create new userdata
				lua_pushlightuserdata(L, key);
				lua_pushvalue(L, -2);  // dup userdata

				lua_settable(L, -4);   // lookup[key] = userdata
			}
			return ud;
		}
	};
}

#endif//_LuaBinding_H

