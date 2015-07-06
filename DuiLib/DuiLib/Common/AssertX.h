#ifndef __DUI_ASSERTX_H__
#define __DUI_ASSERTX_H__

#pragma once

namespace DuiLib
{

	// Assert function return values
	enum ErrRet
	{
		ERRRET_IGNORE = 0,
		ERRRET_CONTINUE,
		ERRRET_BREAKPOINT,
		ERRRET_ABORT
	};

	//- Global functions ----------------------------------------------------------
	UILIB_API ErrRet NotifyAssert(const char* condition, const char* fileName, int lineNumber, const char* formats, ...);


#if defined(_DEBUG) || defined(DEBUG)
	#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
        #define DuiAssertX(x, formats, ...) { \
		static bool _ignoreAssert = false; \
		if (!_ignoreAssert && !(x)) \
		{ \
			ErrRet _err_result = NotifyAssert(#x, __FILE__, __LINE__, formats, ##__VA_ARGS__); \
			if (_err_result == ERRRET_IGNORE) \
			{ \
				_ignoreAssert = true; \
			} \
			else if (_err_result == ERRRET_BREAKPOINT) \
			{ \
			__debugbreak(); \
			} \
		}}
	#else
		#define DuiAssertX(x, formats, ...) { \
		if (!(x)) \
		{ \
			CDuiString comment = CDuiString::FormatString(formats, ##__VA_ARGS__); \
			CDuiString msg; \
			if(!comment.IsEmpty()){ msg = CDuiString("Assert comment:") + comment + CDuiString("\n"); } \
			fprintf (stderr, "%s", msg.c_str()); \
			assert(x); \
			exit(-1);\
		}}

#	endif
#else
#		define DuiAssertX(x, formats, ...)
#endif

#if defined(_DEBUG) || defined(DEBUG)
#	define DuiAssert(x)		DuiAssertX(x, "")
#else
#	define DuiAssert(x)
#endif
}

#endif
