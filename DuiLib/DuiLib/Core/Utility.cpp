#include "StdAfx.h"
#include "Utility.h"
#include <iostream>

using namespace std;
#ifdef _DEBUG
#include <io.h>
#include <fcntl.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#ifndef _CONSOLE
#define _CONSOLE
#endif//_CONSOLE

#endif

namespace DuiLib
{
	bool Console::m_bConsoleInit = false;

	void Console::_RedirectIOToConsole()
	{
#if defined (_DEBUG) && defined(_CONSOLE)
		if(m_bConsoleInit)
			return ;
		// 分配一个控制台，以便于输出一些有用的信息
		AllocConsole();
		// 取得 STDOUT 的文件系统
		HANDLE stdOutputHandle = GetStdHandle( STD_OUTPUT_HANDLE );
		// Redirect STDOUT to the new console by associating STDOUT's file 
		// descriptor with an existing operating-system file handle.
		int hConsoleHandle = _open_osfhandle( (intptr_t)stdOutputHandle, _O_TEXT );
		FILE *pFile = _fdopen( hConsoleHandle, "w" );
		*stdout = *pFile;
		setvbuf( stdout, NULL, _IONBF, 0 );
		// 这个调用确保 iostream 和 C run-time library 的操作在源代码中有序。 
		ios::sync_with_stdio();

		m_bConsoleInit = true;
#endif//
	}
#ifdef _UNICODE
	void Console::Write(const TCHAR* format,...)
	{
#if defined (_DEBUG) && defined(_CONSOLE)
		if (!m_bConsoleInit)
			_RedirectIOToConsole();

		TCHAR szBuffer[300] = { 0 };
		va_list args;
		va_start(args, format);
		vswprintf(szBuffer, lengthof(szBuffer) - 2, format, args);
		va_end(args);
		
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		int size = WideCharToMultiByte(::GetACP(), 0, szBuffer, -1, NULL, 0, NULL, NULL);
#	else
		size_t size = wcstombs(NULL, m_pstr, 0);
#	endif

		std::string mbstr(size, wchar_t(0));
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		WideCharToMultiByte(::GetACP(), 0, szBuffer, -1, const_cast<char*>(mbstr.c_str()), size, NULL, NULL);
#	else
		wcstombs(const_cast<char*>(mbstr.c_str()), const_cast<wchar_t*>(m_pstr), (size+1)*4);
#	endif

		cout<<mbstr.c_str();
#endif//_DEBUG
	}
#else
	void Console::Write(const char* format,...)
	{
#if defined (_DEBUG) && defined(_CONSOLE)
		if (!m_bConsoleInit)
			_RedirectIOToConsole();

		char szBuffer[300] = { 0 };
		va_list args;
		va_start(args, format);
		vsprintf_s(szBuffer, lengthof(szBuffer) - 2, format, args);
		va_end(args);
		cout<<szBuffer;
#endif//_DEBUG
	}
#endif // _UNICODE
	////////////////////////////////
#ifdef _UNICODE
	void VSOutput::Write(const TCHAR* format,...)
	{
#if defined (_DEBUG) && defined(_VSOUTPUT)
		TCHAR szBuffer[300] = { 0 };
		va_list args;
		va_start(args, format);
		vswprintf(szBuffer, lengthof(szBuffer) - 2, format, args);
		va_end(args);
		OutputDebugString(szBuffer);
#endif
	}
#else
	void VSOutput::Write(const char* format,...)
	{
#if defined (_DEBUG) && defined(_VSOUTPUT)
		char szBuffer[300] = { 0 };
		va_list args;
		va_start(args, format);
		vsprintf_s(szBuffer, lengthof(szBuffer) - 2, format, args);
		va_end(args);

		OutputDebugString(szBuffer);
#endif // DEBUG
	}
#endif // _UNICODE
	///////////////////////////////
	// returns a random number
	FORCEINLINE float asm_rand()
	{

#if _MSC_VER == 1300

		static unsigned __int64 q = time( NULL );

		_asm {
			movq mm0, q

				// do the magic MMX thing
				pshufw mm1, mm0, 0x1E
				paddd mm0, mm1

				// move to integer memory location and free MMX
				movq q, mm0
				emms
		}

		return float( q );
#else
		// VC6 does not support pshufw
		return float( rand() );
#endif
	}
	// returns the maximum random number
	FORCEINLINE float asm_rand_max()
	{

#if _MSC_VER == 1300

		//	return std::numeric_limits< unsigned __int64 >::max();
		return 9223372036854775807.0f;
#else
		// VC6 does not support unsigned __int64
		return float( RAND_MAX );
#endif

	}
	/////
	float Math::UnitRandom()
	{
		return asm_rand() / asm_rand_max();
	}
	float Math::RangeRandom(float fLow, float fHigh)
	{
		return (fHigh-fLow)*UnitRandom() + fLow;
	}

	float Math::SymmetricRandom()
	{
		return 2.f * UnitRandom() - 1.f;
	}
}