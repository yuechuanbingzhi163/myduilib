#pragma once

#if defined(LUADUILIB_EXPORTS)
#if defined(_MSC_VER)
#define LUAUILIB_API __declspec(dllexport)
#else
#define LUAUILIB_API 
#endif
#else
#if defined(_MSC_VER)
#define LUAUILIB_API __declspec(dllimport)
#else
#define LUAUILIB_API 
#endif
#endif

#include <lua.hpp>

#include "DuiLib\UIlib.h"
using namespace DuiLib;
#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

#include "LuaInterface\LuaEntry.h"

