#include "StdAfx.h"
#include "DuiString.h"
#include <algorithm>
#include <atlconv.h>  
#include <wchar.h> 
#include <atlbase.h>
#include <atlstr.h>

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static char _HexToChar[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};

	const dui_String DuiString::BLANK;

	DuiString::DuiString()
	{
	}

	DuiString::~DuiString()
	{
	}

	dui_String DuiString::Replace(const dui_String& str, const dui_String& src, const dui_String& dst)
	{
		if (src == dst)
			return str;

		dui_String out = str;
		size_t pos = str.find(src, 0);
		while (pos != dui_String::npos)
		{
			out.replace(pos, src.size(), dst);
			pos = out.find(src, pos + dst.size());
		}

		return out;
	}

	dui_String DuiString::Replace(const dui_String& str, char src, char dst)
	{
		dui_String out = str;
		char* buf = &(*out.begin());
		while (*buf)
		{
			if (*buf == src)
				*buf = dst;
			buf++;
		}

		return out;
	}

	bool DuiString::ReplaceRet(dui_String& str, const dui_String& src, const dui_String& dst)
	{
		if (src == dst)
			return false;

		bool bReplaced = false;
		size_t pos = str.find(src, 0);

		if (pos != dui_String::npos)
		{
			str.replace(pos, src.size(), dst);
			bReplaced = true;
		}

		return bReplaced;
	}

	void DuiString::Trim(dui_String& str, bool bLeft, bool bRight)
	{
		/*
		size_t lspaces, rspaces, len = length(), i;

		lspaces = rspaces = 0;

		if( left )
		{
		// Find spaces / tabs on the left
		for( i = 0;
		i < len && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
		++lspaces, ++i );
		}

		if( right && lspaces < len )
		{
		// Find spaces / tabs on the right
		for( i = len - 1;
		i >= 0 && ( at(i) == ' ' || at(i) == '\t' || at(i) == '\r');
		rspaces++, i-- );
		}

		*this = substr(lspaces, len-lspaces-rspaces);
		*/
		static const dui_String delims = " \t\r\n";
		if (bRight)
			str.erase(str.find_last_not_of(delims) + 1); // trim right
		if (bLeft)
			str.erase(0, str.find_first_not_of(delims)); // trim left
	}

	dui_StringArray DuiString::Split(const dui_String& str, const dui_String& delims, dui_uint maxSplits)
	{
		dui_StringArray ret;

		if (str.empty())
			return ret;

		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

		dui_uint numSplits = 0;

		// Use STL methods
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delims, start);
			if (pos == start)
			{
				// Do nothing
				start = pos + 1;
			}
			else if (pos == dui_String::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back(str.substr(start));
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back(str.substr(start, pos - start));
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != dui_String::npos);

		return ret;
	}

	void DuiString::LowerCase(dui_String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) tolower);
	}

	void DuiString::UpperCase(dui_String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) toupper);
	}

	dui_String DuiString::Format(const char* formats, ...)
	{
		dui_String sReturn;
		char szBuffer[4096] = { 0 };

		va_list args;
		va_start(args, formats);
		vsprintf(szBuffer, formats, args);
		va_end(args);

		sReturn.assign(szBuffer);
		return sReturn;
	}

	bool DuiString::StartWith(const dui_String& str, const dui_String& pattern, bool lowCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		dui_String startOfThis = str.substr(0, patternLen);
		if (lowCase)
			DuiString::LowerCase(startOfThis);

		return (startOfThis == pattern);
	}

	bool DuiString::EndWith(const dui_String& str, const dui_String& pattern)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		dui_String endOfThis = str.substr(thisLen - patternLen, patternLen);

		return (endOfThis == pattern);
	}

	bool DuiString::Equal(const dui_String& str1, const dui_String& str2, bool bCaseSensitive/*= true*/)
	{
		if (bCaseSensitive)
		{
			return (str1 == str2);
		}
		else
		{
			dui_String lstr1 = str1;
			dui_String lstr2 = str2;
			LowerCase(lstr1);
			LowerCase(lstr2);
			return (lstr1 == lstr2);
		}
	}

	dui_String DuiString::WCS2MBS(const dui_WString &str)
	{
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		int size = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
#	else
		size_t size = wcstombs(NULL, str.c_str(), 0);
#	endif

		dui_String mbstr(size, wchar_t(0));
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, const_cast<char*>(mbstr.c_str()), size, NULL, NULL);
#	else
		wcstombs(const_cast<char*>(mbstr.c_str()), const_cast<wchar_t*>(str.c_str()), (size + 1) * 4);
#	endif

		return mbstr;
	}

	dui_WString DuiString::MBS2WCS(const dui_String& str)
	{
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0) - 1;
#	else
		size_t size = mbstowcs(NULL, str.c_str(), 0);
#	endif

		dui_WString wstr(size, wchar_t(0));
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), const_cast<wchar_t*>(wstr.c_str()), size);
#	else
		mbstowcs(const_cast<wchar_t*>(wstr.c_str()), str.c_str(), size);
#	endif
		return wstr;
	}

	dui_String DuiString::ToString(float val, dui_ushort precision, dui_ushort width, char fill)
	{
		dui_StringStream ss;
		ss.precision(precision);
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	dui_String DuiString::ToString(int val, dui_ushort width, char fill)
	{
		dui_StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	dui_String DuiString::ToString(long val, dui_ushort width, char fill)
	{
		dui_StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	dui_String DuiString::ToString(unsigned int val, dui_ushort width, char fill)
	{
		dui_StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	dui_String DuiString::ToString(unsigned long val, dui_ushort width, char fill)
	{
		dui_StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	dui_String DuiString::ToString(bool val, bool bYesNo)
	{
		if (val)
		{
			if (bYesNo)
				return "yes";
			else
				return "true";
		}
		else
		{
			if (bYesNo)
				return "no";
			else
				return "false";
		}
	}

	float DuiString::ParseFloat(const dui_String& val, float defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		float ret = defVal;
		str >> ret;

		return ret;
	}

	double DuiString::ParseDouble(const dui_String& val, double defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		double ret = defVal;
		str >> ret;

		return ret;
	}

	int DuiString::ParseInt(const dui_String& val, int defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		int ret = defVal;
		str >> ret;

		return ret;
	}

	long DuiString::ParseLong(const dui_String& val, long defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		long ret = defVal;
		str >> ret;

		return ret;
	}

	bool DuiString::ParseBool(const dui_String& val, bool defVal)
	{
		if (DuiString::Equal(val, "true", false) ||
			DuiString::Equal(val, "yes", false) ||
			DuiString::Equal(val, "1", false))
			return true;
		else if (DuiString::Equal(val, "false", false) ||
			DuiString::Equal(val, "no", false) ||
			DuiString::Equal(val, "0", false))
			return false;
		else
			return defVal;
	}

	dui_ushort DuiString::ParseUShort(const dui_String& val, dui_ushort defVal /*= 0xffff*/)
	{
		dui_StringStream ss(val.c_str());
		dui_ushort ret = defVal;
		ss >> std::hex >> ret;
		return ret;
	}

	dui_uint DuiString::ParseUInt(const dui_String& val, dui_uint defVal /*= 0xffffffff*/)
	{
		dui_StringStream ss(val.c_str());
		dui_uint ret = defVal;
		ss >> std::hex >> ret;
		return ret;
	}

	dui_int8 DuiString::ParseI8(const dui_String& val, dui_int8 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_int8 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_int16 DuiString::ParseI16(const dui_String& val, dui_int16 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_int16 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_int32 DuiString::ParseI32(const dui_String& val, dui_int32 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_int32 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_int64 DuiString::ParseI64(const dui_String& val, dui_int64 defaultValue)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_int64 ret = defaultValue;
		str >> ret;

		return ret;
	}

	dui_uint8 DuiString::ParseUI8(const dui_String& val, dui_uint8 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_uint8 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_uint16 DuiString::ParseUI16(const dui_String& val, dui_uint16 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_uint16 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_uint32 DuiString::ParseUI32(const dui_String& val, dui_uint32 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_uint32 ret = defVal;
		str >> ret;

		return ret;
	}

	dui_uint64 DuiString::ParseUI64(const dui_String& val, dui_uint64 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		dui_StringStream str(val);
		dui_uint64 ret = defVal;
		str >> ret;

		return ret;
	}


	bool DuiString::IsNumber(const dui_String& val)
	{
		dui_StringStream str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
	}

	dui_String DuiString::Hex2Char(dui_uint val)
	{
		dui_String out;
		out.resize(4 * 2 + 1);// one byte - two characters

		char* to = (char*)out.c_str();
		dui_byte *from = (dui_byte*)(&val) + 3;

		for (int i = 0; i < 4; ++i)
		{
			*(to++) = _HexToChar[(*from) >> 4];		// 4 high bits.
			*(to++) = _HexToChar[(*from) & 0x0f];	// 4 low bits.

			--from;
		}

		return out;
	}
	////////////////////
	dui_String DuiString::UTF8ToGB2312(const char* ptext, int nlen)
	{
		dui_String sResult;

		char buf[4];
		memset(buf, 0, 4);
		char* rst = new char[nlen + (nlen >> 2) + 2];
		memset(rst, 0, nlen + (nlen >> 2) + 2);

		int i = 0;
		int j = 0;

		while (i < nlen)
		{
			if (*(ptext + i) >= 0)
			{
				rst[j++] = ptext[i++];
			}
			else
			{
				wchar_t Wtemp;

				_UTF8ToUTF16ofChar(&Wtemp, ptext + i);
				_UTF16ToGB2312ofChar(buf, Wtemp);

				unsigned short int tmp = 0;
				tmp = rst[j] = buf[0];
				tmp = rst[j + 1] = buf[1];
				tmp = rst[j + 2] = buf[2];

				i += 3;
				j += 2;
			}
		}
		rst[j] = '\0';
		sResult = rst;
		delete[]rst;

		return sResult;
	}
	dui_String DuiString::GB2312ToUTF8(const char* ptext, int len)
	{
		dui_String sResult;
		char buf[4];
		memset(buf, 0, 4);

		int i = 0;
		while (i < len)
		{
			//如果是英文直接复制就可以
			if (ptext[i] >= 0)
			{
				char asciistr[2] = { 0 };
				asciistr[0] = (ptext[i++]);
				sResult.append(asciistr);
			}
			else
			{
				wchar_t pbuffer;
				_Gb2312ToUTF16ofChar(&pbuffer, *(ptext + i));

				_UTF16ToUTF8ofChar(buf, &pbuffer);

				sResult.append(buf);

				i += 2;
			}
		}

		return sResult;
	}

	dui_WString DuiString::UTF8ToUTF16(const char* ptext, int len)
	{
		dui_String stmp = UTF8ToGB2312(ptext, len);
		return GB2312ToUTF16(stmp.c_str(), stmp.length());
	}
	dui_String DuiString::UTF16ToUTF8(const wchar_t* ptext, int len)
	{
		dui_String sResult;

#ifdef WIN32
		int nBufSize = ::WideCharToMultiByte(CP_UTF8, 0, ptext, -1, NULL, 0, 0, FALSE);
		char *szBuf = new char[nBufSize];
		::WideCharToMultiByte(CP_UTF8, 0, ptext, -1, szBuf, nBufSize, 0, FALSE);
		sResult = szBuf;
		delete[]szBuf;
		szBuf = NULL;
#else
		for (int i = 0; i < len; ++i)
		{
			char c;
			_UTF16ToUTF8ofChar(&c, ptext[i]);
			sResult.append(c);
		}
#endif
		return sResult;
	}

	dui_WString DuiString::GB2312ToUTF16(const char* ptext, int len)
	{
		dui_WString sResult;

		dui_String curLocale = setlocale(LC_ALL, NULL);// curLocale = "C";
		setlocale(LC_ALL, "chs");

		size_t size = len + 1;
		wchar_t *p = new wchar_t[size];
		wmemset(p, 0, size);
		mbstowcs(p, ptext, size);
		sResult = p;
		delete[]p;

		setlocale(LC_ALL, curLocale.c_str());

		return sResult;
	}
	dui_String DuiString::UTF16ToGB2312(const wchar_t* ptext, int len)
	{
		dui_String sResult;
		// 
		// 		for (int i=0;i<len;++i)
		// 		{
		// 			char c;
		// 			_UTF16ToGB2312ofChar(&c,ptext[i]);
		// 			sResult.append(c);
		// 		}
		// 
		// 		return sResult;

		dui_String curLocale = setlocale(LC_ALL, NULL);// curLocale = "C";
		setlocale(LC_ALL, "chs");

		size_t size = 2 * len + 1;
		char* p = new char[size];
		memset(p, 0, size);
		wcstombs(p, ptext, size);

		sResult = p;
		delete[]p;

		setlocale(LC_ALL, curLocale.c_str());

		return sResult;
	}
	/////
	void DuiString::_UTF16ToGB2312ofChar(char* pOut, const wchar_t& uData)
	{
#ifdef WIN32
		::WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(uData), NULL, NULL);
#else
		char* pcurLocale = setlocale(LC_ALL, NULL);// curLocale = "C";
		setlocale(LC_ALL, "chs");
		wcstombs(pOut, &uData, sizeof(uData));
		setlocale(LC_ALL, pcurLocale);
#endif
	}

	void DuiString::_Gb2312ToUTF16ofChar(wchar_t* pOut, const char &gbBuffer)
	{
#ifdef WIN32
		::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, &gbBuffer, 2, pOut, 1);
#else
		char* pcurLocale = setlocale(LC_ALL, NULL);// curLocale = "C";
		setlocale(LC_ALL, "chs");
		mbstowcs(pOut, &gbBuffer, sizeof(gbBuffer));
		setlocale(LC_ALL, pcurLocale);
#endif
	}

	void DuiString::_UTF8ToUTF16ofChar(wchar_t* pOut, const char *pText)
	{
		char* uchar = (char *)pOut;

		uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
		uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
	}

	void DuiString::_UTF16ToUTF8ofChar(char* pOut, wchar_t* pText)
	{
		// 注意 WCHAR高低字的顺序,低字节在前，高字节在后
		char* pchar = (char *)pText;

		pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
		pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
		pOut[2] = (0x80 | (pchar[0] & 0x3F));
	}

	int DuiString::_IsTextUTF8(char* str, ULONGLONG length)
	{
		int i;
		DWORD nBytes = 0;                       //UFT8可用1-6个字节编码,ASCII用一个字节   
		UCHAR chr;
		BOOL bAllAscii = TRUE; //如果全部都是ASCII, 说明不是UTF-8  
		for (i = 0; i < length; i++)
		{
			chr = *(str + i);
			if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
				bAllAscii = FALSE;
			if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if (chr >= 0x80)
				{
					if (chr >= 0xFC && chr <= 0xFD)
						nBytes = 6;
					else if (chr >= 0xF8)
						nBytes = 5;
					else if (chr >= 0xF0)
						nBytes = 4;
					else if (chr >= 0xE0)
						nBytes = 3;
					else if (chr >= 0xC0)
						nBytes = 2;
					else
					{
						return FALSE;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx   
			{
				if ((chr & 0xC0) != 0x80)
				{
					return FALSE;
				}
				nBytes--;
			}
		}
		if (nBytes > 0) //违返规则
		{
			return FALSE;
		}
		if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8 
		{
			return FALSE;
		}
		return TRUE;
	}

	////////////////////////////////
	CDuiString DuiString::UTF8ToUnicode(const char* p_input)
	{
		try
		{
			CString s_result("");
			if (p_input != NULL && strlen(p_input) > 0)
			{
				CA2W input(p_input, CP_UTF8);
				s_result = input;
			}
			int nLen = s_result.GetLength();
			CDuiString sret(s_result.GetBuffer(nLen));
			s_result.ReleaseBuffer();
			return sret;
		}
		catch (...)
		{
			throw "UTF8ToUnicode";
		}
	}
	CDuiString DuiString::AnsiToUnicode(const char* p_input)
	{
		try
		{
			CString s_result;
			if (p_input != NULL && strlen(p_input) > 0)
			{
				CA2W input(p_input, CP_ACP);
				s_result = input;
			}
			int nLen = s_result.GetLength();
			CDuiString sret(s_result.GetBuffer(nLen));
			s_result.ReleaseBuffer();
			return sret;
		}
		catch (...)
		{
			throw "AnsiToUnicode";
		}
	}
	dui_String DuiString::UnicodeToUTF8(const wchar_t* p_input)
	{
		try
		{
			dui_String s_result("");
			if (p_input != NULL && wcslen(p_input) > 0)
			{
				CW2A input(p_input, CP_UTF8);
				s_result = input;
			}
			return s_result;
		}
		catch (...)
		{
			throw "UnicodeToUTF8";
		}
	}
	dui_String DuiString::UnicodeToAnsi(const wchar_t* p_input)
	{
		try
		{
			dui_String s_result("");
			if (p_input != NULL && wcslen(p_input) > 0)
			{
				CW2A input(p_input);
				s_result = input;
			}
			return s_result;
		}
		catch (...)
		{
			throw "UnicodeToAnsi";
		}
	}
	dui_String DuiString::UTF8ToAnsi(const char* p_input)
	{
		try
		{
			dui_String s_result;
			if (p_input != NULL && strlen(p_input) > 0)
			{
				CA2W input(p_input, CP_UTF8);
				s_result = UnicodeToAnsi(input);
			}			
			return s_result;
		}
		catch (...)
		{
			throw "UTF8ToAnsi";
		}
	}
	dui_String DuiString::AnsiToUTF8(const char* p_input)
	{
		try
		{
			dui_String s_result;
			if (p_input != NULL && strlen(p_input) > 0)
			{
				CA2W input(p_input);
				s_result = UnicodeToUTF8(input);
			}
			return s_result;
		}
		catch (...)
		{
			throw "AnsiToUTF8";
		}
	}
	dui_String DuiString::UTF8ToGBK(const char* p_input)
	{
		dui_String s_result;
		if (NULL != p_input && strlen(p_input) > 0)
		{
			int len = strlen(p_input);
			s_result = UTF8ToGB2312(p_input, len);
		}
		return s_result;
	}
	dui_String DuiString::GBKToUTF8(const char* p_input)
	{
		dui_String s_result;
		if (NULL != p_input && strlen(p_input) > 0)
		{
			int len = strlen(p_input);
			s_result = GB2312ToUTF8(p_input, len);
		}
		return s_result;
	}
}