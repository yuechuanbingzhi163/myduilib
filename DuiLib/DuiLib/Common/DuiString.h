#ifndef _DuiString_h
#define _DuiString_h
#pragma once
#include <string>
#include <vector>

#define _ENCODING_LOWER        1
#define _ENCODING_UPPER        2
#define _ENCODING_HALF         4
#define _ENCODING_SIMPLIFIED   8

namespace DuiLib
{
	class  UILIB_API DuiString
	{
	private:
		DuiString();
		~DuiString();
	public:
		static const dui_String		BLANK;
	public:
		static dui_String			Replace(const dui_String& str, const dui_String& src, const dui_String& dst);
		static dui_String			Replace(const dui_String& str, char src, char dst);
		static bool				ReplaceRet(dui_String& str, const dui_String& src, const dui_String& dst);
		static void				Trim(dui_String& str, bool bLeft = true, bool bRight = true);
		static dui_StringArray		Split(const dui_String& str, const dui_String& delims = ", ", dui_uint maxSplits = 0);
		static void				LowerCase(dui_String& str);
		static void				UpperCase(dui_String& str);
		static dui_String			Format(const char* formats, ...);
		static bool				StartWith(const dui_String& str, const dui_String& pattern, bool lowCase = false);
		static bool				EndWith(const dui_String& str, const dui_String& pattern);
		static bool				Equal(const dui_String& str1, const dui_String& str2, bool bCaseSensitive = true);

		static dui_String			WCS2MBS(const dui_WString& str);
		static dui_WString			MBS2WCS(const dui_String& str);

		static dui_String			ToString(float val, dui_ushort precision = 6, dui_ushort width = 0, char fill = ' ');
		static dui_String			ToString(int val, dui_ushort width = 0, char fill = ' ');
		static dui_String			ToString(long val, dui_ushort width = 0, char fill = ' ');
		static dui_String			ToString(unsigned int val, dui_ushort width = 0, char fill = ' ');
		static dui_String			ToString(unsigned long val, dui_ushort width = 0, char fill = ' ');
		static dui_String			ToString(bool val, bool bYesNo = false);

		static float			ParseFloat(const dui_String& val, float defVal = 0.0f);
		static double			ParseDouble(const dui_String& val, double defVal = 0.0);
		static int				ParseInt(const dui_String& val, int defVal = 0);
		static long				ParseLong(const dui_String& val, long defVal = 0);
		static bool				ParseBool(const dui_String& val, bool defVal = false);
		static dui_ushort			ParseUShort(const dui_String& val, dui_ushort defVal = 0xffff);
		static dui_uint			    ParseUInt(const dui_String& val, dui_uint defVal = 0xffffffff);
		static dui_int8				ParseI8(const dui_String& val, dui_int8 defVal = 0);
		static dui_int16			ParseI16(const dui_String& val, dui_int16 defVal = 0);
		static dui_int32			ParseI32(const dui_String& val, dui_int32 defVal = 0);
		static dui_int64			ParseI64(const dui_String& val, dui_int64 defVal = 0);
		static dui_uint8			ParseUI8(const dui_String& val, dui_uint8 defVal = 0);
		static dui_uint16			ParseUI16(const dui_String& val, dui_uint16 defVal = 0);
		static dui_uint32			ParseUI32(const dui_String& val, dui_uint32 defVal = 0);
		static dui_uint64			ParseUI64(const dui_String& val, dui_uint64 defVal = 0);


		static bool				IsNumber(const dui_String& val);
		static dui_String			Hex2Char(dui_uint val);

		static dui_String           UTF8ToGB2312(const char* ptext, int nlen);
		static dui_String           GB2312ToUTF8(const char* ptext, int len);
		static dui_WString          UTF8ToUTF16(const char* ptext, int len);
		static dui_String           UTF16ToUTF8(const wchar_t* ptext, int len);
		static dui_WString          GB2312ToUTF16(const char* ptext, int len);
		static dui_String           UTF16ToGB2312(const wchar_t* ptext, int len);

		static CDuiString       UTF8ToUnicode(const char* p_input);
		static CDuiString       AnsiToUnicode(const char* p_input);
		static dui_String			UnicodeToUTF8(const wchar_t* p_input);
		static dui_String           UnicodeToAnsi(const wchar_t* p_input);
		static dui_String           UTF8ToAnsi(const char* p_input);
		static dui_String           AnsiToUTF8(const char* p_input);
		static dui_String           UTF8ToGBK(const char* p_input);
		static dui_String           GBKToUTF8(const char* p_input);
	protected:
		static void _UTF16ToGB2312ofChar(char* pOut, const wchar_t& uData);
		static void _Gb2312ToUTF16ofChar(wchar_t* pOut, const char &gbBuffer);
		static void _UTF8ToUTF16ofChar(wchar_t* pOut, const char *pText);
		static void _UTF16ToUTF8ofChar(char* pOut, wchar_t* pText);
		static int _IsTextUTF8(char* str, ULONGLONG length);
	};
}



#endif//_DuiString_h