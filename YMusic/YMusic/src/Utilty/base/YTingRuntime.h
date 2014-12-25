#pragma once

//////////////////////////////////////////////////////////////////////////
#ifdef	WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef UNICODE
//#define  _T(type)  L##type
#define  _W(fun)   w##fun

#define Rzstrftime           wcsftime
#define Rzstrcat             wcscat      
#define Rzfopen              _wfopen

#define Rzfprintf            fwprintf
#define Rzstrrchr            wcsrchr
#define Rzstrncpy            wcsncpy
#define Rzstrlen             wcslen
#define Rzstrcmp             wcscmp
#define Rzstrcpy             lstrcpy
#define Rzvfprintf           vfwprintf
#else
#define  _T(type)  type
#define  _W(fun)   fun

#define Rzstrftime           strftime
#define Rzstrcat             strcat
#define Rzfopen              fopen
#define Rzfprintf            fprintf
#define Rzstrrchr            strrchr
#define Rzstrncpy            strncpy
#define Rzstrlen             strlen
#define Rzstrcmp             strcmp
#define Rzstrcpy             strcpy
#define Rzvfprintf           vfprintf
#endif    
//STL 
#define  Rzcout            _W(cout)
#define  Rzcin             _W(cin)
#define  Rzcerr            _W(cerr)
#define  Rzclog            _W(clog)
#define  Rzstring          _W(string)
#define  Rzfstream         _W(fstream)
#define  Rzifstream        _W(ifstream)
#define  Rzofstream        _W(ofstream)
#define  Rzstringstream    _W(stringstream)
#define  Rzsprintf         _W(sprintf) 
#define  Rzprintf          _W(printf)
//end STL
#else  //not win32
#endif //end 

#if defined(__cplusplus)
#define	__Begin_Decls       	extern "C" {
#define	__End_Decls         	}
#else
#define	__Begin_Decls
#define	__End_Decls
#endif

#if defined(__cplusplus)
#define _CFun                    extern "C"
#else
#define _CFun                    extern
#endif


#define  __NameSpaceBegin(name)   namespace name {
#define  __NameSpaceEnd           }
#define  __Using(name)            using namespace name;


#define  __MyNameSpaceBegin       __NameSpaceBegin(YTing)
#define  __MyNameSpaceEnd         __NameSpaceEnd
#define  __UsingMyNameSpace		  __Using(YTing)
#define  __MyNameSpace(class)     YTing::class



#ifdef _DLL_API_ 
#define _DLL_API  __declspec(dllexport)
#else
#define _DLL_API // __declspec(dllimport)
#endif

#define  TOSTR(s)               (#s)
//////////////////////////////////////////////////////////////////////////
/********************************************************************************
 
 Base integer types for all target OS's and CPU's
 
 UInt8            8-bit unsigned integer 
 SInt8            8-bit signed integer
 UInt16          16-bit unsigned integer 
 SInt16          16-bit signed integer           
 UInt32          32-bit unsigned integer 
 SInt32          32-bit signed integer   
 UInt64          64-bit unsigned integer 
 SInt64          64-bit signed integer   
 
 *********************************************************************************/
typedef unsigned char                   uchar;
typedef unsigned short                  ushort;
typedef unsigned int                    uint;
typedef unsigned long                   ulong;

typedef signed char                     int8;
typedef uchar                           uint8;
typedef signed short                    int16;
typedef ushort                          uint16;

#ifdef __LP64__
typedef signed int                      int32;
typedef uint                            uint32;
#else
typedef signed long                     int32;
typedef unsigned long                   uint32;
#endif

#if defined(_MSC_VER) && !defined(__MWERKS__) && defined(_M_IX86)
typedef   signed __int64                int64;
typedef unsigned __int64                uint64;
#else
typedef   signed long long              int64;
typedef unsigned long long              uint64;
#endif


#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
// Microsoft visual studio, version 2005 and higher.
#define YTING_SNPRINTF _snprintf_s
#define YTING_SSCANF   sscanf_s
#elif defined(_MSC_VER) && (_MSC_VER >= 1200 )
// Microsoft visual studio, version 6 and higher.
//#pragma message( "Using _sn* functions." )
#define YTING_SNPRINTF _snprintf
#define YTING_SSCANF   sscanf
#elif defined(__GNUC__) && (__GNUC__ >= 3 )
// GCC version 3 and higher.s
//#warning( "Using sn* functions." )
#define YTING_SNPRINTF snprintf
#define YTING_SSCANF   sscanf
#else
#define YTING_SNPRINTF snprintf
#define YTING_SSCANF   sscanf
#endif


/*********************************************************************************/

    


#include <cassert>

#ifndef MinVersion
#define MinVersion
#endif


void* memcpyFromRing(void * dst,
					 const size_t cnt,
					 const void * src,
					 size_t b,
					 size_t size);
void* memcpyToRing(const void *src,
				   const size_t cnt,
				   void * dst,
				   size_t b,
				   size_t size) ;
int memfind(const void* src,
			size_t srcsize,
			const void* dst,
			size_t dstsize);
void* PoolMalloc(size_t nSize) ;
void  PoolFree(void* p) ;

#include "../encrypt/encrypt.h"
#include "Buffer.h"