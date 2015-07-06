/********************************************************************
    FileName :  RzType.h   
    Version  :  0.10
    Date     :	2010-2-1 19:41:30
    Author   :  小家伙
    Comment  : 

*********************************************************************/
#ifndef __RZTYPE_H__
#define __RZTYPE_H__
#pragma once

#include <cassert>

#ifdef	WIN32
      #include <Windows.h>
#define RzLock  CRITICAL_SECTION
//消除_T警告
#ifdef _T
#undef _T
#endif

  #ifdef UNICODE
     #define  _T(type)  L##type
     #define  _W(fun)   w##fun
  
     #define Rzchar              wchar_t
     #define Rzstring 	std::wstring
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
     #define Rzchar                char
     #define Rzstring			std::string
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
 //      #define  Rzstring          _W(string)
      #define  Rzfstream         _W(fstream)
      #define  Rzifstream        _W(ifstream)
      #define  Rzofstream        _W(ofstream)
      #define  Rzstringstream    _W(stringstream)
      #define  Rzsprintf         _W(sprintf) 
      #define  Rzprintf          _W(printf)
      //end STL
#else  //not win32
#endif //end 

#ifdef  _DEBUG
     #define  RzDeBugOut(type)   cout(type)
#else
     #define  RzDeBugOut(type)   //cout(type)
#endif

//////////////////////////////////////////////////////////////////////////

#ifdef   __cplusplus
      #define  _RzCFunBegin   extern "C" {
      #define  _RzCFunEnd     }
#endif



#define  _RzNameSpaceBegin(name)   namespace name {
#define  _RzNameSpaceEnd           }
#define  _RzUsing(name)            using namespace name;

#define _RzStdBegin  _RzNameSpaceBegin(RzStd)
#define _RzStdEnd    _RzNameSpaceEnd


#if defined(_RZ_DLL_)
	#if defined(_MSC_VER)
		#define RZ_DLL_API __declspec(dllexport)
	#else
		#define RZ_DLL_API 
	#endif
#else
	#if defined(_MSC_VER)
		#define RZ_DLL_API __declspec(dllimport)
	#else
		#define RZ_DLL_API 
	#endif
#endif

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

typedef unsigned int		Dword;
typedef int					Bool;
typedef unsigned char		Byte;
typedef unsigned short		Word;

typedef std::fstream			FStream;
typedef std::ifstream			IFStream;
typedef std::ofstream			OFStream;

typedef uint8		UTF8;
typedef uint32	    UTF32;











#endif




