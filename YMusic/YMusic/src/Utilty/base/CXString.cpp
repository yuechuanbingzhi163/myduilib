#include "stdafx.h"
#include "CXString.h"
#include <algorithm>
//#include <string>
//#include <codecvt>
#ifdef _WIN32
#include <direct.h>
#endif

namespace string{
	void TrimLeftChar(XString& str,char c)
	{
		str.erase(0,str.find_first_not_of(c));
	}
	void TrimLeft(XString& str)
	{
		for (XString::iterator i = str.begin();i !=str.end();++i)
		{
			if(!isspace(*i))
			{
				str.erase(str.begin(),i);
				return;
			}
		}
	}
	void TrimRightChar(XString& str,char c)
	{
		str.erase(str.find_last_not_of(c)+1);
	}
	void TrimRight(XString& str)
	{
		if(str.empty()) return;
		for (XString::iterator i = str.end()-1;i!=str.begin();--i)
		{
			if(!isspace(*i))
			{
				str.erase(i+1,str.end());
				return;
			}
		}
	}
	void Trim(XString& str)
	{
		TrimLeft(str);
		TrimRight(str);
	}
	void ToLower(XString& str)
	{
		std::transform(str.begin(),str.end(),str.begin(),tolower);
	}
	void ToUpper(XString& str)
	{
		std::transform(str.begin(),str.end(),str.begin(),toupper);
	}
	bool StartsWith(const XString& str,const XString& substr)
	{
		if(substr.length() > str.length()) return false;
		for (XString::size_type i=0;i<substr.length();++i)
		{
			if(substr[i] != str[i]) return false;
		}
		return true;
	}
	bool EndsWith(const XString& str,const XString& substr)
	{
		if(substr.length() > str.length()) return false;
		for (XString::size_type i=str.length()-1,j=substr.length()-1;j>0;--i,--j)
		{
			if(substr[j] != str[i]) return false;
		}
		return true;
	}
	bool IsEqual(const XString& str1,const XString& str2)
	{
		if(str1.length() != str2.length()) return false;
		XString v1(str1);
		XString v2(str2);
		ToLower(v1);
		ToLower(v2);
		return v1 == v2;
	}
	XString Root(const XString& path)
	{
		size_t index = path.find_last_of("/\\");
		return (XString::npos == index) ? XString() : path.substr(0,index+1);
	}
	bool createdirs(const char* _dir)
	{
		assert(_dir);
		if(!_dir) return false;
		XString dir(_dir);
		Trim(dir);
		if(dir.empty()) return false;
		std::replace(dir.begin(),dir.end(),'\\','/');
		while('/' == *dir.rbegin())
			dir.erase(dir.end() -1);

		XString::size_type pos = dir.find('/');
		if(XString::npos == pos)
			return false;

		XString path;
		while (true)
		{
			pos = dir.find('/', pos + 1);
			if (pos == XString::npos)
			{
				path = dir;
#if _WIN32
				_mkdir(path.c_str());
#else
				mkdir(path.c_str(), S_IRWXU);
#endif
				break;
			}
			path = dir.substr(0, pos);
#if _WIN32
			_mkdir(path.c_str());
#else
			mkdir(path.c_str(), S_IRWXU);
#endif
			if (pos >= dir.size())
				break;
		}
		return true;
	}
	bool makeSurePath(XString& s)
	{
		if(s.empty()) return false;
		std::replace(s.begin(),s.end(),'\\','/');
		Trim(s);
		TrimLeftChar(s,'/');
		if(s.empty()) return false;
		while('/' != *s.rbegin()) s += '/';
		return !s.empty();
	}

#ifdef _WIN32
	CString UTF8ToCString(const char* str)
	{
		CString rs;
		if (str && ::strlen(str) > 0)
		{
			CA2W input(str, CP_UTF8);
			rs = input;
		}
		return rs;
	}

	CString AnsiToCString(const char* str)
	{
		CString rs;
		if (str && ::strlen(str) > 0)
		{
			CA2W input(str, CP_ACP);
			rs = input;
		}
		return rs;
	}

	XString UnicodeToUTF8(const wchar_t* str)
	{
// 		typedef std::wstring_convert<std::codecvt_utf8<wchar_t> > w2utf8CoderT;
// 		w2utf8CoderT coder;
// 
// 		std::wstring s(str);
// 		return coder.to_bytes(s);

		XString rs;
		if (str && ::wcslen(str) > 0)
		{
			CW2A input(str, CP_UTF8);
			rs = input;
		}
		return rs;
	}

	XString UnicodeToAnsi(const wchar_t* str)
	{
		XString rs;
		if (str && ::wcslen(str) > 0)
		{
			CW2A input(str,CP_ACP);
			rs = input;
		}
		return rs;
	}

	XString UTF8ToAnsi(const char* str)
	{
		XString rs;
		if (str && ::strlen(str))
		{
			CString s_uni = UTF8ToCString(str);
			rs = UnicodeToAnsi(s_uni);
		}
		return rs;
	}

	XString AnsiToUTF8(const char* str)
	{
		XString rs;
		if (str && (::strlen(str) > 0))
		{
			CA2W input(str,CP_ACP);
			rs = UnicodeToUTF8(input);
		}
		return rs;
	}
#endif
}

