#ifndef _DuiType_H
#define _DuiType_H
#pragma once

#include <sstream>
#include <vector>
#include <string>

namespace DuiLib
{

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
	typedef unsigned char                   dui_uchar;
	typedef unsigned short                  dui_ushort;
	typedef unsigned int                    dui_uint;
	typedef unsigned long                   dui_ulong;

	typedef signed char                     dui_int8;
	typedef dui_uchar                       dui_uint8;
	typedef signed short                    dui_int16;
	typedef dui_ushort                      dui_uint16;

#ifdef __LP64__
	typedef signed int                      dui_int32;
	typedef dui_uint                        dui_uint32;
#else
	typedef signed long                     dui_int32;
	typedef unsigned long                   dui_uint32;
#endif

#if defined(_MSC_VER) && !defined(__MWERKS__) && defined(_M_IX86)
	typedef   signed __int64                dui_int64;
	typedef unsigned __int64                dui_uint64;
#else
	typedef   signed long long              dui_int64;
	typedef unsigned long long              dui_uint64;
#endif

	typedef unsigned int		dui_dword;
	typedef int					dui_bool;
	typedef unsigned char		dui_byte;
	typedef unsigned short		dui_word;

	typedef dui_uint8		dui_UTF8;
	typedef dui_uint32	    dui_UTF32;
//////////////////////////////////////////////////////////////////////////
	typedef std::string dui_String;
	typedef std::wstring dui_WString;
	typedef std::vector<dui_String> dui_StringArray;
	typedef std::stringstream	dui_StringStream;
	typedef dui_String dui_UTF8String;
	typedef std::basic_string<dui_UTF32, std::char_traits<dui_UTF32> >	dui_UTF32String;

}

#endif