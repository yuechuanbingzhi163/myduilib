#ifndef _LuaOp_H_
#define _LuaOp_H_


#ifndef  _WIN32
	#include <stdint.h>
	#define SPRINTF_F snprintf
#else
	#define SPRINTF_F _snprintf_s

struct strtoll_tool_t
{
	static long do_strtoll(const char* s, const char*, int)
	{
		return atol(s);
	}
};
	#define strtoll strtoll_tool_t::do_strtoll
	#define strtoull (unsigned long)strtoll_tool_t::do_strtoll
#endif

#include <sstream>
#include <vector>
#include <list>
#include <set>

#include <atlconv.h>  
#include <atlbase.h>
#include <atlstr.h>

namespace DuiLib
{
	struct lua_convert_tool
	{
		// 多字节编码转为UTF8编码  
		inline static std::string MBToUTF8(const char* pmb, dui_int32 mLen)
		{
			std::string pOut;
			// convert an MBCS string to widechar   
			dui_int32 nLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);

			WCHAR* lpszW = NULL;
			try
			{
				lpszW = new WCHAR[nLen];
			}
			catch (bad_alloc)
			{
				return pOut;
			}

			dui_int32 nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, lpszW, nLen);

			if (nRtn != nLen)
			{
				delete[] lpszW;
				return pOut;
			}
			// convert an widechar string to utf8  
			dui_int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, NULL, 0, NULL, NULL);
			if (utf8Len <= 0)
			{
				delete[] lpszW;
				return pOut;
			}

			CW2A input(lpszW, CP_UTF8);
			pOut = input;
			return pOut;
			//vector<char> pu8;
			pOut.resize(utf8Len);
			nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, &*pOut.begin(), utf8Len, NULL, NULL);
			delete[] lpszW;

			if (nRtn != utf8Len)
			{
				pOut.clear();
				return pOut;
			}
			
			return pOut;
		}

		// UTF8编码转为多字节编码  
		inline static std::string UTF8ToMB(const char* pu8, dui_int32 utf8Len)
		{
			std::string pOut;
			// convert an UTF8 string to widechar   
			dui_int32 nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);

			WCHAR* lpszW = NULL;
			try
			{
				lpszW = new WCHAR[nLen];
			}
			catch (bad_alloc)
			{
				return pOut;
			}

			dui_int32 nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, lpszW, nLen);

			if (nRtn != nLen)
			{
				delete[] lpszW;
				return pOut;
			}

			// convert an widechar string to Multibyte   
			dui_int32 MBLen = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, NULL, 0, NULL, NULL);
			if (MBLen <= 0)
			{
				delete[] lpszW;
				return pOut;
			}
			pOut.resize(MBLen);
			nRtn = WideCharToMultiByte(CP_ACP, 0, lpszW, nLen, &*pOut.begin(), MBLen, NULL, NULL);
			delete[] lpszW;

			if (nRtn != MBLen)
			{
				pOut.clear();
				return pOut;
			}
			return pOut;
		}

		// 多字节编码转为Unicode编码  
		inline static std::wstring MBToUnicode(const char* pmb, dui_int32 mLen)
		{
			std::wstring pOut;
			// convert an MBCS string to widechar   
			dui_int32 uLen = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, NULL, 0);

			if (uLen <= 0)
			{
				return pOut;
			}
			pOut.resize(uLen);

			dui_int32 nRtn = MultiByteToWideChar(CP_ACP, 0, pmb, mLen, &*pOut.begin(), uLen);

			if (nRtn != uLen)
			{
				pOut.clear();
				return pOut;
			}
			return pOut;
		}

		//Unicode编码转为多字节编码  
		inline static std::string UnicodeToMB(const wchar_t* pun, dui_int32 uLen)
		{
			std::string pOut;
			// convert an widechar string to Multibyte   
			dui_int32 MBLen = WideCharToMultiByte(CP_ACP, 0, pun, uLen, NULL, 0, NULL, NULL);
			if (MBLen <= 0)
			{
				return pOut;
			}
			pOut.resize(MBLen);
			int nRtn = WideCharToMultiByte(CP_ACP, 0, pun, uLen, &*pOut.begin(), MBLen, NULL, NULL);

			if (nRtn != MBLen)
			{
				pOut.clear();
				return pOut;
			}
			return pOut;
		}

		// UTF8编码转为Unicode  
		inline static std::wstring UTF8ToUnicode(const char* pu8, dui_int32 utf8Len)
		{
			std::wstring pOut;
			// convert an UTF8 string to widechar   
			dui_int32 nLen = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, NULL, 0);
			if (nLen <= 0)
			{
				return pOut;
			}
			pOut.resize(nLen);
			dui_int32 nRtn = MultiByteToWideChar(CP_UTF8, 0, pu8, utf8Len, &*pOut.begin(), nLen);

			if (nRtn != nLen)
			{
				pOut.clear();
				return pOut;
			}

			return pOut;
		}

		// Unicode编码转为UTF8  
		inline static std::string UnicodeToUTF8(const wchar_t* pun, dui_int32 uLen)
		{
			std::string pOut;
			// convert an widechar string to utf8  
			dui_int32 utf8Len = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, NULL, 0, NULL, NULL);
			if (utf8Len <= 0)
			{
				return pOut;
			}
			pOut.resize(utf8Len);
			dui_int32 nRtn = WideCharToMultiByte(CP_UTF8, 0, pun, uLen, &*pOut.begin(), utf8Len, NULL, NULL);

			if (nRtn != utf8Len)
			{
				pOut.clear();
				return pOut;
			}
			return pOut;
		}
	};

	struct lua_cstring_tool
	{
		inline static std::string toLuaString(const CDuiString& arg)
		{
			LPCTSTR pstr = arg.GetData();
			dui_int32 nLen = arg.GetLength();
			std::string s_out;
#ifdef _UNICODE
			s_out = lua_convert_tool::UnicodeToUTF8((wchar_t*)pstr,-1);
#else
			//s_out.assign((char*)pstr);
			s_out = lua_convert_tool::MBToUTF8((char*)pstr, -1);
#endif
			return s_out;
		}

#ifdef _UNICODE
		inline static std::wstring _toDuiString(const std::string& arg)
		{
			std::wstring puni = lua_convert_tool::UTF8ToUnicode(arg.c_str(), arg.length());
			return puni;
		}
#else
		inline static std::string _toDuiString(const std::string& arg)
		{
			std::string pmb = lua_convert_tool::UTF8ToMB(arg.c_str(), arg.length());
			return pmb;
		}
#endif	
		inline static void toDuiString(const std::string& arg, OUT CDuiString& s_out)
		{
#ifdef _UNICODE
			std::wstring puni = lua_convert_tool::UTF8ToUnicode(arg.c_str(), arg.length());
			s_out.Assign((LPCTSTR)puni.c_str());
#else
			std::string pmb = lua_convert_tool::UTF8ToMB(arg.c_str(), arg.length());
			s_out.Assign((LPCTSTR)pmb.c_str());
#endif
		}
	};

	struct cpp_void_t{};

	struct lua_string_tool_t
	{
		inline static const char* c_str(const std::string& s_) { return s_.c_str(); }
		inline static const char* c_str(const char* s_)   { return s_; }
	};

	class lua_exception_t : public exception
	{
	public:
		explicit lua_exception_t(const char* err_) :
			m_err(err_)
		{}
		explicit lua_exception_t(const std::string& err_) :
			m_err(err_)
		{
		}
		~lua_exception_t() throw (){}

		const char* what() const throw () { return m_err.c_str(); }
	private:
		std::string m_err;
	};

	class lua_tool_t
	{
	public:
		static void dump_stack(lua_State* ls_)
		{
			int i;
			int top = lua_gettop(ls_);

			for (i = 1; i <= top; i++)
			{
				int t = lua_type(ls_, i);
				switch (t)
				{
				case LUA_TSTRING:
				{
					printf("`%s'", lua_tostring(ls_, i));
				}
					break;
				case LUA_TBOOLEAN:
				{
					printf(lua_toboolean(ls_, i) ? "true" : "false");
				}
					break;
				case LUA_TNUMBER:
				{
					printf("`%g`", lua_tonumber(ls_, i));
				}
					break;
				case LUA_TTABLE:
				{
					printf("table end\n");
					lua_pushnil(ls_);
					while (lua_next(ls_, i) != 0) {
						printf("	%s - %s\n",
							lua_typename(ls_, lua_type(ls_, -2)),
							lua_typename(ls_, lua_type(ls_, -1)));
						lua_pop(ls_, 1);
					}
					printf("table end");
				}
					break;
				default:
				{
					printf("`%s`", lua_typename(ls_, t));
				}
					break;
				}
				printf(" ");
			}
			printf("\n");
		}
		static string dump_error(lua_State* ls_, const char *fmt, ...)
		{
			string ret;
			char buff[1024];

			va_list argp;
			va_start(argp, fmt);
#ifndef _WIN32
			vsnprintf(buff, sizeof(buff), fmt, argp);
#else
			vsnprintf_s(buff, sizeof(buff), sizeof(buff), fmt, argp);
#endif
			va_end(argp);

			ret = buff;
			SPRINTF_F(buff, sizeof(buff), " tracback:%s", lua_tostring(ls_, -1));
			ret += buff;

			return ret;
		}
	};

	class lua_nil_t{};

	template<typename T>
	struct lua_op_t;

	template<>
	struct lua_op_t < char* >
	{
		static void push_stack(lua_State* ls_, char* arg_)
		{
			lua_pushstring(ls_, arg_);
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT char*& param_,char* default_ = NULL)
		{
			const char* str = luaL_checkstring(ls_, pos_);
			param_ = (char*)str;
			return 0;
		}
	};

	template<>
	struct lua_op_t<const char*>
	{
		static void push_stack(lua_State* ls_, const char* arg_)
		{
			lua_pushstring(ls_, arg_);
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT char*& param_,char* default_ = NULL)
		{
			const char* str = luaL_checkstring(ls_, pos_);
			param_ = (char*)str;
			return 0;
		}
	};

	template<>
	struct lua_op_t<lua_nil_t>
	{
		static void push_stack(lua_State* ls_, const lua_nil_t& arg_)
		{
			lua_pushnil(ls_);
		}
	};

	template<>
	struct lua_op_t<cpp_void_t>
	{
		static int get_ret_value(lua_State* ls_, int pos_, OUT  cpp_void_t& param_, cpp_void_t& default_ = cpp_void_t())
		{
			return 0;
		}
	};

	template<>
	struct lua_op_t< dui_int64 >
	{
		static void push_stack(lua_State* ls_, dui_int64 arg_)
		{
			stringstream ss;
			ss << arg_;
			std::string str = ss.str();
			lua_pushlstring(ls_, str.c_str(), str.length());
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_int64& param_,dui_int64 default_ = 0)
		{
			if (!lua_isstring(ls_, pos_))
			{
				param_ = default_;
				return -1;
			}

			size_t len = 0;
			const char* src = lua_tolstring(ls_, pos_, &len);
			param_ = (dui_int64)strtoll(src, NULL, 10);
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT dui_int64& param_, dui_int64 default_ = 0)
		{
			size_t len = 0;
			const char* str = luaL_checklstring(ls_, pos_, &len);
			param_ = (dui_int64)strtoll(str, NULL, 10);
			return 0;
		}
	};

	template<> 
	struct lua_op_t< dui_uint64 >
	{
		static void push_stack(lua_State* ls_, dui_uint64 arg_)
		{
			stringstream ss;
			ss << arg_;
			std::string str = ss.str();
			lua_pushlstring(ls_, str.c_str(), str.length());
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_uint64& param_ ,dui_uint64 default_ = 0)
		{
			if (!lua_isstring(ls_, pos_))
			{
				return -1;
			}

			size_t len = 0;
			const char* src = lua_tolstring(ls_, pos_, &len);
			param_ = (dui_uint64)strtoull(src, NULL, 10);
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_uint64& param_, dui_uint64 default_ = 0)
		{
			size_t len = 0;
			const char* str = luaL_checklstring(ls_, pos_, &len);
			param_ = (dui_uint64)strtoull(str, NULL, 10);
			return 0;
		}
	};

	template<> 
	struct lua_op_t < dui_int8 >
	{
		static void push_stack(lua_State* ls_, dui_int8 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_int8& param_, dui_int8 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_int8)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_int8& param_, dui_int8 default_ = 0)
		{
			param_ = (dui_int8)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};

	template<>
	struct lua_op_t< dui_uint8 >
	{
		static void push_stack(lua_State* ls_, dui_uint8 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_uint8& param_, dui_uint8 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_uint8)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_uint8& param_, dui_uint8 default_ = 0)
		{
			param_ = (dui_uint8)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};
	
	template<> 
	struct lua_op_t< dui_int16 >
	{
		static void push_stack(lua_State* ls_, dui_int16 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_int16& param_, dui_int16 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_int16)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_int16& param_, dui_int16 default_ = 0)
		{
			param_ = (dui_int16)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};
	
	template<> 
	struct lua_op_t< dui_uint16 >
	{

		static void push_stack(lua_State* ls_, dui_uint16 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT dui_uint16& param_, dui_uint16 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_uint16)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT dui_uint16& param_, dui_uint16 default_ = 0)
		{
			param_ = (dui_uint16)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};
					
	template<> 
	struct lua_op_t < dui_int32 >
	{
		static void push_stack(lua_State* ls_, dui_int32 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_int32& param_, dui_int32 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_int32)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_int32& param_, dui_int32 default_ = 0)
		{
			param_ = (dui_int32)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};
		
	template<> 
	struct lua_op_t < dui_uint32 >
	{
		static void push_stack(lua_State* ls_, dui_uint32 arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  dui_uint32& param_, dui_uint32 default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (dui_uint32)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  dui_uint32& param_, dui_uint32 default_ = 0)
		{
			param_ = (dui_uint32)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};

	template<>
	struct lua_op_t < bool >
	{
		static void push_stack(lua_State* ls_, bool arg_)
		{
			lua_pushboolean(ls_, arg_ ? 1 : 0 );
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT  bool& param_,bool default_ = false)
		{
			//! nil 自动转换为false
			if (lua_isnil(ls_, pos_))
			{
				param_ = false;
				return 0;
			}
			if (!lua_isboolean(ls_, pos_))
			{
				return -1;
			}

			param_ = (0 != lua_toboolean(ls_, pos_));
			return 0;
		}
		
		static int lua_to_value(lua_State* ls_, int pos_, OUT bool& param_,bool default_ = false)
		{
			luaL_checktype(ls_, pos_, LUA_TBOOLEAN);
			param_ = (0 != lua_toboolean(ls_, pos_));
			return 0;
		}
	};


	template<>
	struct lua_op_t < BOOL >
	{
		static void push_stack(lua_State* ls_, BOOL arg_)
		{
			lua_pushboolean(ls_, arg_ ? 1 : 0);
		}

		static int get_ret_value(lua_State* ls_, int pos_,  OUT BOOL& param_,BOOL default_ = FALSE)
		{
			//! nil 自动转换为false
			if (lua_isnil(ls_, pos_))
			{
				param_ = false;
				return 0;
			}
			if (!lua_isboolean(ls_, pos_))
			{
				return -1;
			}

			param_ = (0 != lua_toboolean(ls_, pos_));
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT  BOOL& param_, BOOL default_ = FALSE)
		{
			luaL_checktype(ls_, pos_, LUA_TBOOLEAN);
			param_ = (0 != lua_toboolean(ls_, pos_));
			return 0;
		}
	};

	template<>
	struct lua_op_t < std::string >
	{
		static void push_stack(lua_State* ls_, const std::string& arg_)
		{
			lua_pushlstring(ls_, arg_.c_str(), arg_.length());
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT std::string& param_, std::string& default_ = std::string(""))
		{
			if (!lua_isstring(ls_, pos_))
			{
				return -1;
			}

			lua_pushvalue(ls_, pos_);
			size_t len = 0;
			const char* src = lua_tolstring(ls_, -1, &len);
			param_.assign(src, len);
			lua_pop(ls_, 1);

			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT std::string& param_, std::string& default_ = std::string(""))
		{
			size_t len = 0;
			const char* str = luaL_checklstring(ls_, pos_, &len);
			param_.assign(str, len);
			return 0;
		}
	};

	template<>
	struct lua_op_t < const std::string& >
	{
		static void push_stack(lua_State* ls_, const std::string& arg_)
		{
			lua_pushlstring(ls_, arg_.c_str(), arg_.length());
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT std::string& param_, const std::string& default_ = std::string(""))
		{
			if (!lua_isstring(ls_, pos_))
			{
				return -1;
			}

			lua_pushvalue(ls_, pos_);
			size_t len = 0;
			const char* src = lua_tolstring(ls_, -1, &len);
			param_.assign(src, len);
			lua_pop(ls_, 1);

			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT std::string& param_, const std::string& default_ = std::string(""))
		{
			size_t len = 0;
			const char* str = luaL_checklstring(ls_, pos_, &len);
			param_.assign(str, len);
			return 0;
		}
	};
			
	template<>
	struct lua_op_t < float >
	{
		static void push_stack(lua_State* ls_, float arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT  float& param_,float default_ = 0.f)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (float)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  float& param_,float default_ = 0.f)
		{
			param_ = (float)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};
				
	template<>
	struct lua_op_t < double >
	{
		static void push_stack(lua_State* ls_, double arg_)
		{
			lua_pushnumber(ls_, (lua_Number)arg_);
		}
		static int get_ret_value(lua_State* ls_, int pos_, OUT double& param_,double default_ = 0)
		{
			if (!lua_isnumber(ls_, pos_))
			{
				return -1;
			}
			param_ = (double)lua_tonumber(ls_, pos_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  double& param_,double default_ = 0)
		{
			param_ = (double)luaL_checknumber(ls_, pos_);
			return 0;
		}
	};

	template<>
	struct lua_op_t < CDuiString >
	{
		static void push_stack(lua_State* ls_, const CDuiString& arg_)
		{
			std::string s = lua_cstring_tool::toLuaString(arg_);
			lua_op_t<std::string>::push_stack(ls_, s);
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT CDuiString& param_, CDuiString& default_ = CDuiString())
		{
			if (!lua_isstring(ls_, pos_))
			{
				return -1;
			}

			std::string s;
			lua_op_t<std::string>::get_ret_value(ls_, pos_, s);
			lua_cstring_tool::toDuiString(s, param_);
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT CDuiString& param_, CDuiString& default_ = CDuiString())
		{
			std::string s;
			lua_op_t<std::string>::lua_to_value(ls_, pos_, s);
			lua_cstring_tool::toDuiString(s, param_);
			return 0;
		}
	};

	template<>
	struct lua_op_t < const CDuiString& >
	{
		static void push_stack(lua_State* ls_, const CDuiString& arg_)
		{
			std::string s = arg_.str();
			lua_op_t<std::string>::push_stack(ls_, s);
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT CDuiString& param_, const CDuiString& default_ = CDuiString())
		{
			if (!lua_isstring(ls_, pos_))
			{
				return -1;
			}

			std::string s;
			lua_op_t<std::string>::get_ret_value(ls_, pos_, s);
			param_.AssignChar(s.c_str(),s.length());
			return 0;
		}
		static int lua_to_value(lua_State* ls_, int pos_, OUT  CDuiString& param_, const CDuiString& default_ = CDuiString())
		{
			std::string s;
			lua_op_t<std::string>::lua_to_value(ls_, pos_, s);
			param_.AssignChar(s.c_str(), s.length());
			return 0;
		}
	};

	template<>
	struct lua_op_t < void* >
	{
		static void push_stack(lua_State* ls_, void* arg_)
		{
			lua_pushlightuserdata(ls_, arg_);
		}

		static int get_ret_value(lua_State* ls_, int pos_, void* & param_, void* default_ = NULL)
		{
			if (!lua_isuserdata(ls_, pos_))
			{
				char buff[128];
				SPRINTF_F(buff, sizeof(buff), "userdata param expected, but type<%s> provided",
					lua_typename(ls_, lua_type(ls_, pos_)));
				printf("%s\n", buff);
				return -1;
			}

			param_ = lua_touserdata(ls_, pos_);
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, void*& param_, void* default_ = NULL)
		{
			if (!lua_isuserdata(ls_, pos_))
			{
				luaL_argerror(ls_, 1, "userdata param expected");
				return -1;
			}
			param_ = lua_touserdata(ls_, pos_);
			return 0;
		}
	};

	template<typename T>
	struct lua_op_t < T* >
	{
		static void push_stack(lua_State* ls_, T* arg_)
		{
			
		}

		static int get_ret_value(lua_State* ls_, int pos_, T* & param_, T* default_ = NULL)
		{
			
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, T*& param_, T* default_ = NULL)
		{
			
			return 0;
		}
	};

	template<typename T>
	struct lua_op_t < const T* >
	{
		static void push_stack(lua_State* ls_, const T* arg_)
		{
			lua_op_t<T*>::push_stack(ls_, (T*)arg_);
		}

		static int get_ret_value(lua_State* ls_, int pos_, T* & param_,const T* default_ = NULL)
		{
			return lua_op_t<T*>::get_ret_value(ls_, pos_, param_,default_);
		}

		static int lua_to_value(lua_State* ls_, int pos_, T*& param_, const T* default_ = NULL)
		{
			return lua_op_t<T*>::lua_to_value(ls_, pos_, param_,default_);
		}
	};

	template<typename T>
	struct lua_op_t < std::vector<T> >
	{
		static void push_stack(lua_State* ls_, const std::vector<T>& arg_)
		{
			lua_newtable(ls_);
			typename std::vector<T>::const_iterator it = arg_.begin();
			for (int i = 1; it != arg_.end(); ++it, ++i)
			{
				lua_op_t<int>::push_stack(ls_, i);
				lua_op_t<T>::push_stack(ls_, *it);
				lua_settable(ls_, -3);
			}
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT  std::vector<T>& param_, std::vector<T>& default_ = std::vector<T>())
		{
			if (0 == lua_istable(ls_, pos_))
			{
				return -1;
			}
			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;

			while ( lua_next(ls_, real_pos) != 0 )
			{
				param_.push_back(T());
				if (lua_op_t<T>::get_ret_value(ls_, -1, param_[param_.size() - 1]) < 0)
				{
					return -1;
				}
				lua_pop(ls_, 1);
			}
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT std::vector<T>& param_, std::vector<T>& default_ = std::vector<T>())
		{
			luaL_checktype(ls_, pos_, LUA_TTABLE);

			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;
			while (lua_next(ls_, real_pos) != 0)
			{
				param_.push_back(T());
				if (lua_op_t<T>::lua_to_value(ls_, -1, param_[param_.size() - 1]) < 0)
				{
					luaL_argerror(ls_, pos_ > 0 ? pos_ : -pos_, "convert to vector failed");
				}
				lua_pop(ls_, 1);
			}
			return 0;
		}
	};

	template<typename T>
	struct lua_op_t < std::list<T> >
	{
		static void push_stack(lua_State* ls_, const std::list<T>& arg_)
		{
			lua_newtable(ls_);
			typename std::list<T>::const_iterator it = arg_.begin();
			for (int i = 1; it != arg_.end(); ++it, ++i)
			{
				lua_op_t<int>::push_stack(ls_, i);
				lua_op_t<T>::push_stack(ls_, *it);
				lua_settable(ls_, -3);
			}
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT std::list<T>& param_, std::list<T>& default_ = std::list<T>())
		{
			if (0 == lua_istable(ls_, pos_))
			{
				return -1;
			}
			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;

			while (lua_next(ls_, real_pos) != 0)
			{
				param_.push_back(T());
				if (lua_op_t<T>::get_ret_value(ls_, -1, (param_.back())) < 0)
				{
					return -1;
				}
				lua_pop(ls_, 1);
			}
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT  std::list<T>& param_, std::list<T>& default_ = std::list<T>())
		{
			luaL_checktype(ls_, pos_, LUA_TTABLE);

			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;
			while (lua_next(ls_, real_pos) != 0)
			{
				param_.push_back(T());
				if (lua_op_t<T>::lua_to_value(ls_, -1, (param_.back())) < 0)
				{
					luaL_argerror(ls_, pos_ > 0 ? pos_ : -pos_, "convert to vector failed");
				}
				lua_pop(ls_, 1);
			}
			return 0;
		}
	};

	template<typename T>
	struct lua_op_t < std::set<T> >
	{
		static void push_stack(lua_State* ls_, const std::set<T>& arg_)
		{
			lua_newtable(ls_);
			typename std::set<T>::const_iterator it = arg_.begin();
			for (int i = 1; it != arg_.end(); ++it, ++i)
			{
				lua_op_t<int>::push_stack(ls_, i);
				lua_op_t<T>::push_stack(ls_, *it);
				lua_settable(ls_, -3);
			}
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT  std::set<T>& param_, std::set<T>& default_ = std::set<T>())
		{
			if (0 == lua_istable(ls_, pos_))
			{
				return -1;
			}
			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;

			while (lua_next(ls_, real_pos) != 0)
			{
				T val = init_value_traits_t<T>::value();
				if (lua_op_t<T>::get_ret_value(ls_, -1, val) < 0)
				{
					return -1;
				}
				param_.insert(val);
				lua_pop(ls_, 1);
			}
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT  std::set<T>& param_, std::set<T>& default_ = std::set<T>())
		{
			luaL_checktype(ls_, pos_, LUA_TTABLE);

			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;
			while (lua_next(ls_, real_pos) != 0)
			{
				T val = init_value_traits_t<T>::value();
				if (lua_op_t<T>::lua_to_value(ls_, -1, val) < 0)
				{
					luaL_argerror(ls_, pos_ > 0 ? pos_ : -pos_, "convert to vector failed");
				}
				param_.insert(val);
				lua_pop(ls_, 1);
			}
			return 0;
		}
	};
	
	template<typename K, typename V>
	struct lua_op_t < std::map<K, V> >
	{
		static void push_stack(lua_State* ls_, const std::map<K, V>& arg_)
		{
			lua_newtable(ls_);
			typename std::map<K, V>::const_iterator it = arg_.begin();
			for (; it != arg_.end(); ++it)
			{
				lua_op_t<K>::push_stack(ls_, it->first);
				lua_op_t<V>::push_stack(ls_, it->second);
				lua_settable(ls_, -3);
			}
		}

		static int get_ret_value(lua_State* ls_, int pos_, OUT std::map<K, V>& param_, std::map<K, V>& default_ = std::map<K, V>())
		{
			if (0 == lua_istable(ls_, pos_))
			{
				return -1;
			}
			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;

			while (lua_next(ls_, real_pos) != 0)
			{
				K key = init_value_traits_t<K>::value();
				V val = init_value_traits_t<V>::value();

				if (lua_op_t<K>::get_ret_value(ls_, -2, key) < 0 ||
					lua_op_t<V>::get_ret_value(ls_, -1, val) < 0)
				{
					return -1;
				}
				param_.insert(make_pair(key, val));
				lua_pop(ls_, 1);
			}
			return 0;
		}

		static int lua_to_value(lua_State* ls_, int pos_, OUT std::map<K, V>& param_, std::map<K, V>& default_ = std::map<K, V>())
		{
			luaL_checktype(ls_, pos_, LUA_TTABLE);

			lua_pushnil(ls_);
			int real_pos = pos_;
			if (pos_ < 0) real_pos = real_pos - 1;
			while (lua_next(ls_, real_pos) != 0)
			{
				K key = init_value_traits_t<K>::value();
				V val = init_value_traits_t<V>::value();
				if (lua_op_t<K>::get_ret_value(ls_, -2, key) < 0 ||
					lua_op_t<V>::get_ret_value(ls_, -1, val) < 0)
				{
					luaL_argerror(ls_, pos_ > 0 ? pos_ : -pos_, "convert to vector failed");
				}
				param_.insert(make_pair(key, val));
				lua_pop(ls_, 1);
			}
			return 0;
		}
	};

};

#endif
