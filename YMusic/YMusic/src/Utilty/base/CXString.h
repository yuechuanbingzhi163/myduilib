#ifndef _CSKINSTRING_H_
#define _CSKINSTRING_H_

#pragma once
#include <string>
#include "YTingRuntime.h"

#ifdef _WIN32
#include <atlstr.h>
#include <atlconv.h>
#endif


typedef std::string XString;
namespace xstring{
	void TrimLeftChar(XString& str,char c);
	void TrimLeft(XString& str);
	void TrimRightChar(XString& str,char c);
	void TrimRight(XString& str);
	void Trim(XString& str);
	void ToLower(XString& str);
	void ToUpper(XString& str);
	bool StartsWith(const XString& str,const XString& substr);
	bool EndsWith(const XString& str,const XString& substr);
	bool IsEqual(const XString& str1,const XString& str2);
	XString Root(const XString& path);
	bool createdirs(const char* _dir);
	bool makeSurePath(XString& s);

#ifdef _WIN32
	CString UTF8ToCString(const char* str);
	CString AnsiToCString(const char* str);
	XString UnicodeToUTF8(const wchar_t* str);
	XString UnicodeToAnsi(const wchar_t* str);
	XString UTF8ToAnsi(const char* str);
	XString AnsiToUTF8(const char* str);
#endif
};


#endif