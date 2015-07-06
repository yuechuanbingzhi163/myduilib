#ifndef __STRINGUTIL_H__
#define __STRINGUTIL_H__
#pragma once
#include <string>
#include <vector>
#include "RzType.h"

#define _ENCODING_LOWER        1
#define _ENCODING_UPPER        2
#define _ENCODING_HALF         4
#define _ENCODING_SIMPLIFIED   8

namespace em_utility
{
	inline void _EncoderInit()
	{
		
	}

	inline void _NormalizeUTF8(char* text, unsigned options)
	{

	}
	inline void _NormalizeGBK(char* text, unsigned options)
	{

	}

	inline void _GBK_TO_UTF8(const char* from, unsigned int from_len, char** to, unsigned int* to_len)
	{

	}
	inline void _UTF8_TO_GBK(const char* from, unsigned int from_len, char** to, unsigned int* to_len)
	{

	}
};


namespace em_utility
{
	typedef std::string String;
	typedef std::wstring WString;
	typedef std::vector<String> StringArray;
	typedef std::stringstream	StringStream;
	typedef String UTF8String;
	typedef std::basic_string<UTF32, std::char_traits<UTF32> >	UTF32String;

	class  RZ_DLL_API RString
	{
	private:
		RString();
		~RString();

	public:
		static const String		BLANK;
		
	public:
		static String			Replace(const String& str, const String& src, const String& dst);
		static String			Replace(const String& str, char src, char dst);
		static bool				ReplaceRet(String& str, const String& src, const String& dst);
		static void				Trim(String& str, bool bLeft = true, bool bRight = true);
		static StringArray		Split(const String& str, const String& delims = ", ", uint maxSplits = 0);
		static void				LowerCase(String& str);
		static void				UpperCase(String& str);
		static String			Format(const char* formats, ...);
		static bool				StartWith(const String& str, const String& pattern, bool lowCase = false);
		static bool				EndWith(const String& str, const String& pattern);
		static bool				Equal(const String& str1, const String& str2, bool bCaseSensitive = true);

		static String			WCS2MBS(const WString& str);
		static WString			MBS2WCS(const String& str);

		static String			ToString(float val, ushort precision = 6, ushort width = 0, char fill = ' ');
		static String			ToString(int val, ushort width = 0, char fill = ' ');
		static String			ToString(long val, ushort width = 0, char fill = ' ');
		static String			ToString(unsigned int val, ushort width = 0, char fill = ' ');
		static String			ToString(unsigned long val, ushort width = 0, char fill = ' ');
		static String			ToString(bool val, bool bYesNo = false);

		static float			ParseFloat(const String& val, float defVal = 0.0f);
		static double			ParseDouble(const String& val, double defVal = 0.0);
		static int				ParseInt(const String& val, int defVal = 0);
		static long				ParseLong(const String& val, long defVal = 0);
		static bool				ParseBool(const String& val, bool defVal = false);
		static ushort			ParseUShort(const String& val, ushort defVal = 0xffff);
		static uint			ParseUInt(const String& val, uint defVal = 0xffffffff);
		static int8				ParseI8(const String& val, int8 defVal = 0);
		static int16				ParseI16(const String& val, int16 defVal = 0);
		static int32				ParseI32(const String& val, int32 defVal = 0);
		static int64				ParseI64(const String& val, int64 defVal = 0);
		static uint8				ParseUI8(const String& val, uint8 defVal = 0);
		static uint16				ParseUI16(const String& val, uint16 defVal = 0);
		static uint32				ParseUI32(const String& val, uint32 defVal = 0);
		static uint64				ParseUI64(const String& val, uint64 defVal = 0);
	

		static bool				IsNumber(const String& val);
		static String			Hex2Char(uint val);

		static void				GBKToUTF8(char* dst, uint& dstLen, const char* src, uint srcLen);
		static void				UTF8ToGBK(char* dst, uint& dstLen, const char* src, uint srcLen);
		static void				UTF8ToUTF32(UTF32String& utf32Str, const UTF8String& utf8Str);
		static void				UTF32ToUTF8(UTF8String& utf8Str, const UTF32String& utf32Str);
	};
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static char _HexToChar[] =
	{
		'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
	};

	const String RString::BLANK;

	RString::RString()
	{
	}

	RString::~RString()
	{
	}

	String RString::Replace(const String& str, const String& src, const String& dst)
	{    
		if (src == dst)
			return str;

		String out = str;
		size_t pos = str.find(src, 0);
		while (pos != String::npos)
		{
			out.replace(pos, src.size(), dst);
			pos = out.find(src, pos+dst.size());
		}

		return out;
	}

	String RString::Replace(const String& str, char src, char dst)
	{
		String out = str;
		char* buf = &(*out.begin());
		while (*buf)
		{
			if (*buf == src)
				*buf = dst;
			buf++;
		}

		return out;
	}

	bool RString::ReplaceRet(String& str, const String& src, const String& dst)
	{    
		if (src == dst)
			return false;

		bool bReplaced = false;
		size_t pos = str.find(src, 0);

		if(pos != String::npos)
		{
			str.replace(pos, src.size(), dst);
			bReplaced = true;
		}

		return bReplaced;
	}

	void RString::Trim(String& str, bool bLeft, bool bRight)
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
		static const String delims = " \t\r\n";
		if(bRight)
			str.erase(str.find_last_not_of(delims)+1); // trim right
		if(bLeft)
			str.erase(0, str.find_first_not_of(delims)); // trim left
	}

	StringArray RString::Split(const String& str, const String& delims, uint maxSplits)
	{
		StringArray ret;

		if(str.empty())
			return ret;

		// Pre-allocate some space for performance
		ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

		uint numSplits = 0;

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
			else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
			{
				// Copy the rest of the string
				ret.push_back( str.substr(start) );
				break;
			}
			else
			{
				// Copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos + 1;
			}
			// parse up to next real data
			start = str.find_first_not_of(delims, start);
			++numSplits;

		} while (pos != String::npos);

		return ret;
	}

	void RString::LowerCase(String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) tolower);
	}

	void RString::UpperCase(String& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) toupper);
	}

	String RString::Format(const char* formats, ... )
	{
		String sReturn;
		char szBuffer[4096] = {0};

		va_list args;
		va_start(args, formats);
		vsprintf(szBuffer, formats, args);
		va_end(args);

		sReturn.assign(szBuffer);
		return sReturn;
	}

	bool RString::StartWith(const String& str, const String& pattern, bool lowCase)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		String startOfThis = str.substr(0, patternLen);
		if(lowCase)
			RString::LowerCase(startOfThis);

		return (startOfThis == pattern);
	}

	bool RString::EndWith(const String& str, const String& pattern)
	{
		size_t thisLen = str.length();
		size_t patternLen = pattern.length();
		if (thisLen < patternLen || patternLen == 0)
			return false;

		String endOfThis = str.substr(thisLen - patternLen, patternLen);

		return (endOfThis == pattern);
	}

	bool RString::Equal(const String& str1, const String& str2, bool bCaseSensitive/*= true*/)
	{
		if (bCaseSensitive)
		{
			return (str1 == str2);
		}
		else
		{
			String lstr1 = str1;
			String lstr2 = str2;
			LowerCase(lstr1);
			LowerCase(lstr2);
			return (lstr1 == lstr2);
		}
	}

	String RString::WCS2MBS(const WString &str)
	{	
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		int size = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);
#	else
		size_t size = wcstombs(NULL, str.c_str(), 0);
#	endif

		String mbstr(size, wchar_t(0));
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, const_cast<char*>(mbstr.c_str()), size, NULL, NULL);
#	else
		wcstombs(const_cast<char*>(mbstr.c_str()), const_cast<wchar_t*>(str.c_str()), (size+1)*4);
#	endif

		return mbstr;
	}

	WString RString::MBS2WCS(const String& str)
	{
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		int size = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0)-1;
#	else
		size_t size = mbstowcs(NULL, str.c_str(), 0);
#	endif

		WString wstr(size, wchar_t(0));
#    if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN64__)
		MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), const_cast<wchar_t*>(wstr.c_str()), size);
#	else
		mbstowcs(const_cast<wchar_t*>(wstr.c_str()), str.c_str(), size);
#	endif
		return wstr;
	}

	String RString::ToString(float val, ushort precision, ushort width, char fill)
	{
		StringStream ss;
		ss.precision(precision);
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	String RString::ToString(int val, ushort width, char fill)
	{
		StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	String RString::ToString(long val, ushort width, char fill)
	{
		StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	String RString::ToString(unsigned int val, ushort width, char fill)
	{
		StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	String RString::ToString(unsigned long val, ushort width, char fill)
	{
		StringStream ss;
		ss.width(width);
		ss.fill(fill);
		ss << val;
		return ss.str();
	}

	String RString::ToString(bool val, bool bYesNo)
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

	float RString::ParseFloat(const String& val, float defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		float ret = defVal;
		str >> ret;

		return ret;
	}

	double RString::ParseDouble(const String& val, double defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		double ret = defVal;
		str >> ret;

		return ret;
	}

	int RString::ParseInt(const String& val, int defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		int ret = defVal;
		str >> ret;

		return ret;
	}

	long RString::ParseLong(const String& val, long defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		long ret = defVal;
		str >> ret;

		return ret;
	}

	bool RString::ParseBool(const String& val, bool defVal)
	{
		if (RString::Equal(val, "true", false) ||
			RString::Equal(val, "yes", false) ||
			RString::Equal(val, "1", false))
			return true;
		else if(RString::Equal(val, "false", false) ||
			RString::Equal(val, "no", false) ||
			RString::Equal(val, "0", false))
			return false;
		else
			return defVal;
	}

	ushort RString::ParseUShort(const String& val, ushort defVal /*= 0xffff*/)
	{
		StringStream ss(val.c_str());
		ushort ret = defVal;
		ss >> std::hex >> ret;
		return ret;
	}

	uint RString::ParseUInt(const String& val, uint defVal /*= 0xffffffff*/)
	{
		StringStream ss(val.c_str());
		uint ret = defVal;
		ss >> std::hex >> ret;
		return ret;
	}

	int8 RString::ParseI8(const String& val, int8 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		int8 ret = defVal;
		str >> ret;

		return ret;
	}

	int16 RString::ParseI16(const String& val, int16 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		int16 ret = defVal;
		str >> ret;

		return ret;
	}

	int32 RString::ParseI32(const String& val, int32 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		int32 ret = defVal;
		str >> ret;

		return ret;
	}

	int64 RString::ParseI64(const String& val, int64 defaultValue)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		int64 ret = defaultValue;
		str >> ret;

		return ret;
	}

	uint8 RString::ParseUI8(const String& val, uint8 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		uint8 ret = defVal;
		str >> ret;

		return ret;
	}

	uint16 RString::ParseUI16(const String& val, uint16 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		uint16 ret = defVal;
		str >> ret;

		return ret;
	}

	uint32 RString::ParseUI32(const String& val, uint32 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		uint32 ret = defVal;
		str >> ret;

		return ret;
	}

	uint64 RString::ParseUI64(const String& val, uint64 defVal)
	{
		// Use istringstream for direct correspondence with to_string
		StringStream str(val);
		uint64 ret = defVal;
		str >> ret;

		return ret;
	}

	
	bool RString::IsNumber(const String& val)
	{
		StringStream str(val);
		float tst;
		str >> tst;
		return !str.fail() && str.eof();
	}

	String RString::Hex2Char(uint val)
	{
		String out;
		out.resize(4*2+1);// one byte - two characters

		char* to = (char*)out.c_str();
		Byte *from = (Byte*)(&val) + 3;

		for (int i = 0; i < 4; ++i)
		{
			*(to++) = _HexToChar[(*from) >> 4];		// 4 high bits.
			*(to++) = _HexToChar[(*from) & 0x0f];	// 4 low bits.

			--from;
		}

		return out;
	}

	void RString::GBKToUTF8(char* dst, uint& dstLen, const char* src, uint srcLen)
	{
		char* *pdst = &dst;
		_GBK_TO_UTF8(src, srcLen, pdst, &dstLen);
	}

	void RString::UTF8ToGBK(char* dst, uint& dstLen, const char* src, uint srcLen)
	{
		char* *pdst = &dst;
		_UTF8_TO_GBK(src, srcLen, pdst, &dstLen);
	}

	// utf-8 to utf-32 
	// return the utf-32 count
	size_t encoded_size(const unsigned char* buf, size_t len)
	{
		unsigned char tcp;
		size_t count = 0;

		while (len--)
		{
			tcp = *buf++;
			++count;
			size_t size = 0;

			if (tcp < 0x80)
			{
			}
			else if (tcp < 0xE0)
			{
				size = 1;
				++buf;
			}
			else if (tcp < 0xF0)
			{
				size = 2;
				buf += 2;
			}
			else
			{
				size = 3;
				buf += 3;
			}

			if (len >= size)
				len -= size;
			else 
				break;
		}

		return count;
	}

	size_t utf_length(const unsigned char* utf8_str)
	{
		size_t cnt = 0;
		while (*utf8_str++)
			cnt++;

		return cnt;
	}

	void RString::UTF8ToUTF32(UTF32String& utf32Str, const UTF8String& utf8Str)
	{
		if (utf8Str.empty())
			return ;

		unsigned char* src = (unsigned char*)utf8Str.c_str();

		size_t nUTF32Len = encoded_size(src, utf_length(src));
		if (nUTF32Len == 0)
			return ;

		UTF32* pTempBuffer = (UTF32*)malloc(nUTF32Len+1);
		UTF32* pStartBuffer = pTempBuffer;
		pTempBuffer[nUTF32Len] = 0;

		// encode
		//----------------------------------------------------------------------

		size_t destCapacity = nUTF32Len;
		// while there is data in the source buffer, and space in the dest buffer
		for (uint idx = 0; (idx < utf8Str.length() && destCapacity > 0);)
		{
			UTF32	cp;
			unsigned char cu = src[idx++];

			if (cu < 0x80)
			{
				cp = (UTF32)(cu);
			}
			else if (cu < 0xE0)
			{
				cp = ((cu & 0x1F) << 6);
				cp |= (src[idx++] & 0x3F);
			}
			else if (cu < 0xF0)
			{
				cp = ((cu & 0x0F) << 12);
				cp |= ((src[idx++] & 0x3F) << 6);
				cp |= (src[idx++] & 0x3F);
			}
			else
			{
				cp = ((cu & 0x07) << 18);
				cp |= ((src[idx++] & 0x3F) << 12);
				cp |= ((src[idx++] & 0x3F) << 6);
				cp |= (src[idx++] & 0x3F);
			}

			*pTempBuffer++ = cp;
			--destCapacity;
		}

		// --------------------------------------------------------------------

		utf32Str = pStartBuffer;
		free(pStartBuffer);
	}

	// -----utf 32 to utf 8
	// calculate the buff size utf8 required
	size_t utf_length(const UTF32* utf32_str)
	{
		size_t cnt = 0;
		while (*utf32_str++)
			cnt++;

		return cnt;
	}

	// return the number of utf8 code units required to encode the given utf32 code point
	size_t encoded_size(UTF32 code_point)
	{
		if (code_point < 0x80)
			return 1;
		else if (code_point < 0x0800)
			return 2;
		else if (code_point < 0x10000)
			return 3;
		else
			//return 4;
			return 0;
	}

	// return number of code units required to re-encode given utf32 data as utf8.   len is number of code units in 'buf'.
	size_t encoded_size(const UTF32* buf, size_t len)
	{
		size_t count = 0;

		while (len--)
		{
			count += encoded_size(*buf++);
		}

		return count;
	}

	// return number of code units required to re-encode given null-terminated utf32 data as utf8.  return does not include terminating null.
	size_t encoded_size(const UTF32* buf)
	{
		return encoded_size(buf, utf_length(buf));
	}

	void RString::UTF32ToUTF8( UTF8String& utf8Str, const UTF32String& utf32Str )
	{
		if (utf32Str.empty())
		{
			return;
		}

		UTF32* utf32buffer = (UTF32*)utf32Str.c_str();
		size_t utf8Size = encoded_size(utf32buffer);
		size_t src_len = utf32Str.size();

		if (utf8Size == 0)
		{
			return;
		}

		UTF8* pTempBuffer = (UTF8*)malloc(utf8Size+1);
		UTF8* pStartBuffer = pTempBuffer;
		pTempBuffer[utf8Size] = 0;

		// while there is data in the source buffer,
		for (uint idx = 0; idx < src_len; ++idx)
		{
			UTF32 cp = utf32buffer[idx];

			if (cp < 0x80)
			{
				*pTempBuffer++ = (UTF8)cp;
			}
			else if (cp < 0x0800)
			{
				*pTempBuffer++ = (UTF8)((cp >> 6) | 0xC0);
				*pTempBuffer++ = (UTF8)((cp & 0x3F) | 0x80);
			}
			else if (cp < 0x10000)
			{
				*pTempBuffer++ = (UTF8)((cp >> 12) | 0xE0);
				*pTempBuffer++ = (UTF8)(((cp >> 6) & 0x3F) | 0x80);
				*pTempBuffer++ = (UTF8)((cp & 0x3F) | 0x80);
			}
			else
			{
				*pTempBuffer++ = (UTF8)((cp >> 18) | 0xF0);
				*pTempBuffer++ = (UTF8)(((cp >> 12) & 0x3F) | 0x80);
				*pTempBuffer++ = (UTF8)(((cp >> 6) & 0x3F) | 0x80);
				*pTempBuffer++ = (UTF8)((cp & 0x3F) | 0x80);
			}
		}

		utf8Str = (char*)pStartBuffer;
		free(pStartBuffer);
	}
	
}

#endif
