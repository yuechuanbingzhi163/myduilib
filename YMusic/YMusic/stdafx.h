// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "targetver.h"

#ifdef _MSC_VER
#pragma warning (disable : 4511) // copy operator could not be generated
#pragma warning (disable : 4512) // assignment operator could not be generated
#pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
#pragma warning (disable : 4786) // identifier was truncated
#pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
#endif
#ifndef D_SCL_SECURE_NO_WARNINGS
#define D_SCL_SECURE_NO_WARNINGS
#endif
#endif // _MSC_VER



#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include <cassert>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <atlbase.h>
#include <atlstr.h>
#include <atltrace.h>

#include <shlobj.h>   //BROSEINFO
#include <shellapi.h> //对应Shell库

// TODO: 在此处引用程序需要的其他头文件
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <string>
#include <xstring>
#include <memory>
#include <wchar.h>



using namespace std;

#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#pragma comment(lib,"ws2_32.lib")

#pragma  comment(lib,"Comctl32.lib")
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")
#include "YMusic.h"

#define CONSOLE_TRACE
#if defined(_MSC_VER) && defined(CONSOLE_TRACE) && defined(DEBUG)
	#pragma comment( linker, "/subsystem:console /entry:wWinMainCRTStartup" )
	#define console_trace printf
#else
	#define console_trace
#endif

/*

// ------------------------------------------------------------ 
// ---------------------- new and delete ---------------------- 
// ------------------------------------------------------------ 
// ---------------------------------------- operator new 
inline void * operator new (size_t size, const char* file, const size_t line,const char* func)
{
	void *p = malloc(size);
	if (p == NULL) throw std::bad_alloc();

	DuiLib::MemLooker::Instance()->Alloc(size);
	
	DuiLib::MemLooker::Instance()->AddMemInfo((long)p,DuiLib::MemInfo((file==NULL ? __FILE__:file),line,(func==NULL?__FUNCTION__:func),size));
	return p;
}
// ---------------------------------------- operator new[] 
inline void * operator new [](size_t size, const char* file, const size_t line,const char* func)
{
	void *p = malloc(size);
	if (p == NULL) throw std::bad_alloc();
	DuiLib::MemLooker::Instance()->Alloc(size);

	DuiLib::MemLooker::Instance()->AddMemInfo((long)p,DuiLib::MemInfo((file==NULL ? __FILE__:file),line,(func==NULL?__FUNCTION__:func),size));
	return p;
}
// inline void * operator new(size_t size)
// {
// 	void *p = malloc(size);
// 	if (p == NULL) throw std::bad_alloc();
// 	DuiLib::MemLooker::Instance()->Alloc(size);
// 
// 	DuiLib::MemLooker::Instance()->AddMemInfo((long)p,DuiLib::MemInfo(__FILE__,__LINE__,__FUNCTION__,size));
// 	return p;
// }
// inline void * operator new[](size_t size)
// {
// 	void *p = malloc(size);
// 	if (p == NULL) throw std::bad_alloc();
// 	DuiLib::MemLooker::Instance()->Alloc(size);
// 
// 	DuiLib::MemLooker::Instance()->AddMemInfo((long)p,DuiLib::MemInfo(__FILE__,__LINE__,__FUNCTION__,size));
// 	return p;
// }
// ---------------------------------------- operator delete 
inline void operator delete (void * pointer, size_t size)
{
	//	size_t size = sizeof(pointer);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)pointer,size);
	free(pointer);
	OutputDebugString(_T("delete\n"));
}
inline void operator delete(void* pointer)
{
	OutputDebugString(_T("delete1\n"));
	byte* p = (byte*)pointer;
	size_t size = sizeof(p);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)pointer,size);
	free(pointer);
}

inline void operator delete[](void* pointer)
{
	OutputDebugString(_T("delete2\n"));
	byte* p = (byte*)pointer;
	size_t size = sizeof(p);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)pointer,size);
	free(pointer);
}
// ---------------------------------------- operator delete[] 
inline void operator delete[](void * pointer,size_t size)
{
	OutputDebugString(_T("delete3\n"));
//	size_t size = sizeof(pointer);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)pointer,size);
	free(pointer);

}
inline void operator delete(void *p ,size_t size, const char *file, const size_t line,const char* func)
{
	OutputDebugString(_T("delete4\n"));
//	size_t size = sizeof(p);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)p,size);
	free(p);
}

inline void operator delete[](void *p,size_t size, const char *file, const size_t line,const char* func)
{
	OutputDebugString(_T("delete5\n"));
//	size_t size = sizeof(p);
	DuiLib::MemLooker::Instance()->Delloc(size);
	DuiLib::MemLooker::Instance()->UpdateMemInfo((long)p,size);
	free(p);
}
*/

// #define MC_NEW  new(__FILE__, __LINE__,__FUNCTION__)
// #define new  MC_NEW


